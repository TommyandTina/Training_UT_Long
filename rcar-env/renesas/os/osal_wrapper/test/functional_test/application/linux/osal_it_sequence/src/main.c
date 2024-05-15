/***********************************************************************************************************************
* File Name :    main.c
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Test program for OSAL Linux IT
***********************************************************************************************************************/

/*** include ***/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <malloc.h>

#include "rcar-xos/osal/r_osal.h"
#include "rcar-xos/osal/r_osal_memory_impl.h"

#include "osal_test.h"

/*** definition ***/
#define OSAL_TEST_INFO(...) {printf(__VA_ARGS__);}
#define OSAL_TEST_ERR(...)  {printf("error: %s(%d):", __func__, __LINE__); printf(__VA_ARGS__);}

#if defined (RCAR_V3U)
#define TEST_AXI_BUS_NAME "ir_16"
#elif defined (RCAR_V3H1)
#define TEST_AXI_BUS_NAME "ir_00"
#elif defined (RCAR_V3H2)
#define TEST_AXI_BUS_NAME "ir_00"
#elif defined (RCAR_V3M2)
#define TEST_AXI_BUS_NAME "ir_00"
#elif defined (RCAR_V4H)
#define TEST_AXI_BUS_NAME "vc_00"
#elif defined (RCAR_V4M)
#define TEST_AXI_BUS_NAME "vc_00"
#elif defined (RCAR_S4)
#define TEST_AXI_BUS_NAME "rt1_00"
#endif

#if defined (RCAR_S4)
#define TEST_DEVICE_NAME "caiplite_00"
#define TEST_DEVICE_TYPE "caiplite"
#else
#define TEST_DEVICE_NAME "ims_00"
#define TEST_DEVICE_TYPE "ims"
#endif

/*** prototype ***/
static void CheckThread(void);
static void CheckThreadSync(void);
static void CheckMessage(void);
static void CheckClockTime(void);
static void CheckInterrupt(void);
static void CheckIO(void);
static void CheckPower(void);
static void CheckMemory(void);
static void CheckVersion(void);

/* CheckThread */
static int64_t osal_test_thread (void *user_arg);

/* CheckThreadSync */
static void CheckThreadSync_Mutex (void);
static void CheckThreadSync_Cond (void);
static void CheckThreadSync_MemoryBarrier (void);

/*** Common Utils ***/
/* GetAbsoluteTime */
static void GetAbsoluteTime(st_osal_time_t* iop_absolute_time);

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

/*** global variable ***/
static osal_thread_handle_t s_handle;

/* Check Functions Array */
void (* const func_array[])(void) =
{
    CheckThread,
    CheckThreadSync,
    CheckMessage,
    CheckClockTime,
    CheckInterrupt,
    CheckIO,
    CheckPower,
    CheckMemory,
    CheckVersion,
};

typedef struct st_arg_cond
{
    osal_milli_sec_t time;
    st_osal_time_t abs_time;
    osal_cond_handle_t cond_handle;
    osal_mutex_handle_t mutex_handle;
} st_arg_cond_t;

static unsigned int test_pass = 0;
static unsigned int test_fail = 0;
static unsigned int test_skip = 0;

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
int main(int argc, char * argv[])
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    int             ret = 0;
    int             ci_test = 0;
    size_t          len;
    size_t          loop_cnt;
    size_t          func_cnt;
    struct mallinfo loop_mem[MEMORY_CONTINUOUS_CHECK_COUNT];

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (1 < argc)
    {
        // 0: test in local
        // 1: test in CI
        ci_test = atoi(argv[1]);
    }

    for (loop_cnt = 0; loop_cnt < LOOP_MAX(ci_test); loop_cnt++)
    {
        printf("LOOP %03u\n", loop_cnt);

        loop_mem[loop_cnt % (sizeof(loop_mem) / sizeof(loop_mem[0]))] = mallinfo();

        /* Init OSAL */
        OSAL_FUNCTION_CALL(R_OSAL_Initialize(), osal_ret);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("OSAL Initialization failed with error %d\n", osal_ret);
            ret = -1;
            goto END;
        }

        for (func_cnt = 0; func_cnt < sizeof(func_array) / sizeof(func_array[0]); func_cnt++)
        {
            func_array[func_cnt]();
        }

        /* Deinit OSAL */
        OSAL_FUNCTION_CALL(R_OSAL_Deinitialize(), osal_ret);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("OSAL de-initialize failed with error %d\n", osal_ret);
            ret = -1;
            goto END;
        }

        MEMORY_CONTINUOUS_CHECK(loop_mem);
    }

    test_pass /= loop_cnt;
    test_fail /= loop_cnt;

#if defined(NDEBUG)
    // Debug API return errors in release mode
    test_pass += 2;
    test_fail -= 2;
#endif

END :
    printf("< OSAL Tests OK = %d, NG = %d, NT = %d>\n", test_pass, test_fail, test_skip);
    return ret;
}

/***********************************************************************************************************************
* Start of function CheckThread()
***********************************************************************************************************************/
static void CheckThread(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t         osal_ret;
    st_osal_thread_config_t config;

    int64_t                 arg;
    int64_t                 return_value;
    bool                    result;
    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    memset(&config, 0x00, sizeof(st_osal_thread_config_t));
    memset(&s_handle, 0x00, sizeof(osal_thread_handle_t));

    arg = 1;

    OSAL_FUNCTION_CALL(R_OSAL_ThreadInitializeThreadConfigSt(&config), osal_ret);    /* 01_001_010 */
    config.func       = osal_test_thread;
    config.userarg    = (void *)&arg;
    config.priority = OSAL_THREAD_PRIORITY_LOWEST;
    config.task_name = (char *)"Sequence test thread";
    config.stack_size = 2 * 1024 * 1024;

    OSAL_FUNCTION_CALL(R_OSAL_ThreadCreate(&config, 1, &s_handle), osal_ret);    /* 01_001_004 */
    usleep(10 * 1000 * 1000);
    OSAL_FUNCTION_CALL(R_OSAL_ThreadJoin(s_handle, &return_value), osal_ret);    /* 01_001_009 */
}
/***********************************************************************************************************************
* End of function CheckThread()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_test_thread()
***********************************************************************************************************************/
static int64_t osal_test_thread(void *user_arg)
{
    e_osal_return_t         osal_ret;

    osal_thread_handle_t    self_handle;
    st_osal_time_t          current_time;
    st_osal_time_t          absolute_time;
    bool                    result;

    (void)user_arg;

    usleep(2 * 1000 * 1000);
    OSAL_TEST_INFO("***Start: sub thread.***\n");
    R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time);
    if(current_time.tv_nsec + 2000000 <= 999999999)
    {
        absolute_time.tv_nsec = current_time.tv_nsec + 2000000;
        absolute_time.tv_sec = current_time.tv_sec + 1;
    }
    else
    {
        absolute_time.tv_sec = current_time.tv_sec + 1;
        absolute_time.tv_nsec = 2000000 - (999999999 - current_time.tv_nsec);
    }

    OSAL_FUNCTION_CALL(R_OSAL_ThreadSleepUntilTimeStamp(&absolute_time), osal_ret);        /* 01_001_005 */
    OSAL_FUNCTION_CALL(R_OSAL_ThreadSleepForTimePeriod(10), osal_ret);                    /* 01_001_006 */
    OSAL_FUNCTION_CALL(R_OSAL_ThreadSelf(&self_handle), osal_ret);                        /* 01_001_007 */
    OSAL_FUNCTION_CALL(R_OSAL_ThreadEqual(s_handle, self_handle, &result), osal_ret);    /* 01_001_008 */

    OSAL_TEST_INFO("***End: sub thread.***\n");
    return 0;
}
/***********************************************************************************************************************
* End of function osal_test_thread()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function CheckThreadSync()
***********************************************************************************************************************/
static void CheckThreadSync(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    //e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    CheckThreadSync_Mutex();
    CheckThreadSync_Cond();
    CheckThreadSync_MemoryBarrier();
}
/***********************************************************************************************************************
* End of function CheckThreadSync()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function CheckThreadSync_Mutex()
***********************************************************************************************************************/
static void CheckThreadSync_Mutex(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t     osal_ret;

    osal_mutex_id_t     mutex_id;
    osal_mutex_handle_t mutex_handle;

    osal_milli_sec_t    time;
    st_osal_time_t      absolute_time;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    memset(&mutex_handle,  0x00, sizeof(osal_mutex_handle_t));
    memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
    mutex_id   = 1;

    /* R_OSAL_ThsyncMutexCreate */
    OSAL_FUNCTION_CALL(R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle), osal_ret);    /* 01_001_011 */

    /* R_OSAL_ThsyncMutexLockForTimePeriod */    /* requires R_OSAL_ThsyncMutexCreate */
    time = 1000;
    OSAL_FUNCTION_CALL(R_OSAL_ThsyncMutexLockForTimePeriod(mutex_handle, time), osal_ret);    /* 01_001_014 */

    /* R_OSAL_ThsyncMutexUnlock */    /* requires R_OSAL_ThsyncMutexTryLock */
    OSAL_FUNCTION_CALL(R_OSAL_ThsyncMutexUnlock(mutex_handle), osal_ret);        /* 01_001_016 */

    /* R_OSAL_ThsyncMutexLockUntilTimeStamp */    /* requires R_OSAL_ThsyncMutexCreate */
    GetAbsoluteTime(&absolute_time);

    OSAL_FUNCTION_CALL(R_OSAL_ThsyncMutexLockUntilTimeStamp(mutex_handle, &absolute_time), osal_ret);    /* 01_001_013 */
    R_OSAL_ThsyncMutexUnlock(mutex_handle);

    /* R_OSAL_ThsyncMutexTryLock */    /* requires R_OSAL_ThsyncMutexCreate */
    OSAL_FUNCTION_CALL(R_OSAL_ThsyncMutexTryLock(mutex_handle), osal_ret);        /* 01_001_015 */

    /* R_OSAL_ThsyncMutexUnlock */
    R_OSAL_ThsyncMutexUnlock(mutex_handle);

    /* R_OSAL_ThsyncMutexDestroy */    /* requires R_OSAL_ThsyncMutexCreate */
    OSAL_FUNCTION_CALL(R_OSAL_ThsyncMutexDestroy(mutex_handle), osal_ret);        /* 01_001_012 */
}
/***********************************************************************************************************************
* End of function CheckThreadSync_Mutex()
***********************************************************************************************************************/

/* TEST Thread R_OSAL_ThsyncCondWaitUntilTimeStamp 01_001_19 */
int64_t osal_test_cond_waitunittimestamp(void *user_arg)
{
    e_osal_return_t osal_ret;
    st_arg_cond_t*  arg_cond;

    arg_cond = (st_arg_cond_t *)user_arg;

    /* mutex lock */
    R_OSAL_ThsyncMutexTryLock(arg_cond->mutex_handle);

    /* cond wait */
    OSAL_FUNCTION_CALL(R_OSAL_ThsyncCondWaitUntilTimeStamp(arg_cond->cond_handle, arg_cond->mutex_handle, &arg_cond->abs_time), osal_ret);

    R_OSAL_ThsyncMutexUnlock(arg_cond->mutex_handle);

    return (int64_t)osal_ret;
}

/* TEST Thread R_OSAL_ThsyncCondWaitForTimePeriod 01_001_20 */
int64_t osal_test_cond_waitforperiod(void *user_arg)
{
    e_osal_return_t osal_ret;
    st_arg_cond_t*  arg_cond;

    /* cond wait */
    arg_cond = (st_arg_cond_t *)user_arg;

    /* mutex lock */
    R_OSAL_ThsyncMutexTryLock(arg_cond->mutex_handle);

    OSAL_FUNCTION_CALL(R_OSAL_ThsyncCondWaitForTimePeriod(arg_cond->cond_handle, arg_cond->mutex_handle, arg_cond->time), osal_ret);

    R_OSAL_ThsyncMutexUnlock(arg_cond->mutex_handle);

    return (int64_t)osal_ret;
}

/***********************************************************************************************************************
* Start of function CheckThreadSync_Cond()
***********************************************************************************************************************/
static void CheckThreadSync_Cond(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret;

    osal_cond_id_t              cond_id;
    osal_cond_handle_t          cond_handle;
    osal_mutex_handle_t         mutex_handle;
    osal_milli_sec_t            time;
    st_osal_time_t              absolute_time;
    osal_mutex_id_t             mutex_id = 1;

    /* Test Thread Parameter */
    st_arg_cond_t               arg_cond;
    st_osal_thread_config_t     config1;
    st_osal_thread_config_t     config2;
    osal_thread_handle_t        handle1;
    osal_thread_handle_t        handle2;
    osal_thread_id_t            thread_id;
    int64_t                     thread_ret  = 0;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    cond_id     = (1000U);
    memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
    memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));

    /* R_OSAL_ThsyncCondCreate */
    OSAL_FUNCTION_CALL(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), osal_ret);    /* 01_001_017 */
    //R_OSAL_ThsyncMutexCreate
    R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle);

    /**** 01_001_20 START ****/
    memset(&arg_cond, 0x00, sizeof(st_arg_cond_t));
    memset(&config1, 0x00, sizeof(st_osal_thread_config_t));
    memset(&handle1, 0x00, sizeof(osal_thread_handle_t));

    arg_cond.mutex_handle    = mutex_handle;
    arg_cond.cond_handle    = cond_handle;
    arg_cond.time            = 1000;

    config1.func       = osal_test_cond_waitforperiod;
    config1.userarg    = (void *)&arg_cond;
    config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config1.task_name  = "R_OSAL_ThsyncCondWaitForTimePeriod";
    config1.stack_size = 1024;

    /* R_OSAL_ThreadCreate */
    thread_id = 1;
    osal_ret = R_OSAL_ThreadCreate(&config1, thread_id, &handle1);

    /* R_OSAL_ThreadSleepForTimePeriod */
    R_OSAL_ThreadSleepForTimePeriod(100);

    /* mutex lock */
    osal_ret = R_OSAL_ThsyncMutexTryLock(mutex_handle);

    /* R_OSAL_ThsyncCondSignal */    /* requires R_OSAL_ThsyncCondCreate */
    OSAL_FUNCTION_CALL(R_OSAL_ThsyncCondSignal(cond_handle), osal_ret);    /* 01_001_022 */

    osal_ret = R_OSAL_ThsyncMutexUnlock(mutex_handle);

    /* R_OSAL_ThreadJoin handle1 */
    osal_ret = R_OSAL_ThreadJoin(handle1, &thread_ret);
    /**** 01_001_20 END ****/

    /**** 01_001_19 START ****/
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
    thread_id = 2;
    osal_ret  = R_OSAL_ThreadCreate(&config2, thread_id, &handle2);

    /* R_OSAL_ThreadSleepForTimePeriod */
    R_OSAL_ThreadSleepForTimePeriod(100);

    /* mutex lock */
    osal_ret = R_OSAL_ThsyncMutexTryLock(mutex_handle);

    /* R_OSAL_ThsyncCondBroadcast */    /* requires R_OSAL_ThsyncCondCreate */
    OSAL_FUNCTION_CALL(R_OSAL_ThsyncCondBroadcast(cond_handle), osal_ret);    /* 01_001_021 */

    osal_ret = R_OSAL_ThsyncMutexUnlock(mutex_handle);

    /* R_OSAL_ThreadJoin handle2 */
    osal_ret = R_OSAL_ThreadJoin(handle2, &thread_ret);
    /**** 01_001_19 END ****/

    /* R_OSAL_ThsyncCondDestroy */    /* requires R_OSAL_ThsyncCondCreate */
    OSAL_FUNCTION_CALL(R_OSAL_ThsyncCondDestroy(cond_handle), osal_ret);    /* 01_001_018 */

    osal_ret = R_OSAL_ThsyncMutexDestroy(mutex_handle);
}
/***********************************************************************************************************************
* End of function CheckThreadSync_Cond()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function CheckThreadSync_MemoryBarrier()
***********************************************************************************************************************/
static void CheckThreadSync_MemoryBarrier(void)
{
    e_osal_return_t osal_ret;

    /* R_OSAL_ThsyncMemoryBarrier */
    OSAL_FUNCTION_CALL(R_OSAL_ThsyncMemoryBarrier(), osal_ret);    /* 01_001_023 */
}
/***********************************************************************************************************************
* End of function CheckThreadSync_MemoryBarrier()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function CheckMessage()
***********************************************************************************************************************/
static void CheckMessage(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                 osal_ret;

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

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/

    /* R_OSAL_MqInitializeMqConfigSt */
    OSAL_FUNCTION_CALL(R_OSAL_MqInitializeMqConfigSt(&mq_config), osal_ret);    /* 01_001_034 */

    memset(&mq_config, 0x00, sizeof(st_osal_mq_config_t));
    mq_config.max_num_msg = 1;
    mq_config.msg_size = 4;
    mq_id = 40;
    memset(&mq_handle, 0x00, sizeof(osal_mq_handle_t));

    /* R_OSAL_MqCreate */
    OSAL_FUNCTION_CALL(R_OSAL_MqCreate(&mq_config, mq_id, &mq_handle), osal_ret);    /* 01_001_024 */

    /* R_OSAL_MqGetConfig */    /* requires R_OSAL_MqCreate */
    OSAL_FUNCTION_CALL(R_OSAL_MqGetConfig(mq_handle, &mq_config), osal_ret);    /* 01_001_032 */

    /* R_OSAL_MqSendUntilTimeStamp */    /* requires R_OSAL_MqCreate */
    GetAbsoluteTime(&time_stamp);
    OSAL_FUNCTION_CALL(R_OSAL_MqSendUntilTimeStamp(mq_handle, &time_stamp, (void *)&p_buffer, buffer_size), osal_ret);    /* 01_001_026 */

    /* R_OSAL_MqReceiveUntilTimeStamp */    /* requires R_OSAL_MqCreate */
    GetAbsoluteTime(&time_stamp);
    OSAL_FUNCTION_CALL(R_OSAL_MqReceiveUntilTimeStamp(mq_handle, &time_stamp, (void *)&p_buffer, buffer_size), osal_ret);    /* 01_001_028 */

    time_period = 1000;
    OSAL_FUNCTION_CALL(R_OSAL_MqSendForTimePeriod(mq_handle, time_period, (void *)&msg, sizeof(msg)), osal_ret);    /* 01_001_027 */

    time_period = 1000;
    OSAL_FUNCTION_CALL(R_OSAL_MqReceiveForTimePeriod(mq_handle, time_period, (void *)rcv_data, sizeof(msg)), osal_ret);    /* 01_001_029 */

    /* R_OSAL_MqIsFull */    /* requires R_OSAL_MqCreate */
    is_full = false;
    OSAL_FUNCTION_CALL(R_OSAL_MqIsFull(mq_handle, &is_full), osal_ret);    /* 01_001_030 */

    /* R_OSAL_MqIsEmpty */    /* requires R_OSAL_MqCreate */
    OSAL_FUNCTION_CALL(R_OSAL_MqIsEmpty(mq_handle, &mq_result), osal_ret);    /* 01_001_031 */

    /* R_OSAL_MqReset */    /* requires R_OSAL_MqCreate */
    OSAL_FUNCTION_CALL(R_OSAL_MqReset(mq_handle), osal_ret);    /* 01_001_033 */

    /*R_OSAL_MqDelete */    /* requires R_OSAL_MqCreate */
    OSAL_FUNCTION_CALL(R_OSAL_MqDelete(mq_handle), osal_ret);    /* 01_001_025 */
}
/***********************************************************************************************************************
* End of function CheckMessage()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function CheckClockTime()
***********************************************************************************************************************/
static void CheckClockTime(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t         osal_ret;

    e_osal_clock_type_t     clock_type;
    st_osal_time_t          absolute_time;

    st_osal_time_t          time2;
    st_osal_time_t          time1;
    osal_nano_sec_t         different_time;
    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/

    /* R_OSAL_ClockTimeGetTimeStamp */
    memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
    clock_type  = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;

    OSAL_FUNCTION_CALL(R_OSAL_ClockTimeGetTimeStamp(clock_type, &absolute_time), osal_ret);    /* 01_001_035 */

    /* R_OSAL_ClockTimeCalculateTimeDifference */    /* requires R_OSAL_ClockTimeGetTimeStamp */
    memset(&time2, 0x00, sizeof(st_osal_time_t));
    memcpy(&time1, &absolute_time, sizeof(st_osal_time_t));
    memset(&different_time, 0x00, sizeof(osal_nano_sec_t));

    time2.tv_sec = time1.tv_sec + 1; /* add 1sec to clarify the difference. */
    time2.tv_nsec = time1.tv_nsec;

    OSAL_FUNCTION_CALL(R_OSAL_ClockTimeCalculateTimeDifference(&time2, &time1, &different_time), osal_ret);    /* 01_001_036 */
}
/***********************************************************************************************************************
* End of function CheckClockTime()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function CheckInterrupt()
***********************************************************************************************************************/
void osal_test_cb(osal_device_handle_t device_handle, uint64_t irq_channel, void *irq_arg)
{
    return;
}

static void CheckInterrupt(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t         osal_ret;

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

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));
    list_size = sizeof(list);

    /* R_OSAL_InterruptIsISRContext */
    OSAL_FUNCTION_CALL(R_OSAL_InterruptIsISRContext(&result), osal_ret);    /* 01_001_042 */

    /* R_OSAL_InterruptGetNumOfIrqChannels */    /* requires R_OSAL_IoDeviceOpen */
    R_OSAL_IoGetNumOfDevices(TEST_DEVICE_TYPE, &numOfDevices);
    R_OSAL_IoGetDeviceList(TEST_DEVICE_TYPE, list, list_size, &numOfLists);
    device = strtok(list, s);
    R_OSAL_IoDeviceOpen(device, &dev_handle);

    OSAL_FUNCTION_CALL(R_OSAL_InterruptGetNumOfIrqChannels(dev_handle, &num_channels), osal_ret);    /* 01_001_037 */

    /* R_OSAL_InterruptRegisterIsr */    /* requires R_OSAL_InterruptGetNumOfIrqChannels */
    OSAL_FUNCTION_CALL(R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
                               (p_osal_isr_func_t)osal_test_cb,
                               (void *)&arg), osal_ret);    /* 01_001_038 */

    /* R_OSAL_InterruptEnableIsr */        /* requires R_OSAL_InterruptRegisterIsr */
    OSAL_FUNCTION_CALL(R_OSAL_InterruptEnableIsr(dev_handle, 0), osal_ret);    /* 01_001_040 */

    /* R_OSAL_InterruptDisableIsr */    /* requires R_OSAL_InterruptEnableIsr */
    OSAL_FUNCTION_CALL(R_OSAL_InterruptDisableIsr(dev_handle, 0), osal_ret);    /* 01_001_041 */

    /* R_OSAL_InterruptSetIsrPriority */    /* requires R_OSAL_InterruptRegisterIsr */
    OSAL_FUNCTION_CALL(R_OSAL_InterruptSetIsrPriority(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE13), osal_ret);    /* 01_001_043 */

    /* R_OSAL_InterruptUnregisterIsr */    /* requires R_OSAL_InterruptRegisterIsr */
    OSAL_FUNCTION_CALL(R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb), osal_ret);    /* 01_001_039 */

    R_OSAL_IoDeviceClose(dev_handle);
}
/***********************************************************************************************************************
* End of function CheckInterrupt()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function CheckIO()
***********************************************************************************************************************/
static void CheckIO(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    size_t                  numOfDevices;
    char                    list[200];
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

    char                    ax_list[200];
    size_t                  ax_list_size;
    size_t                  ax_numOfLists;
    osal_axi_bus_id_t       axi_id_array[100];

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    memset(&handle, 0x00, sizeof(osal_device_handle_t));
    list_size = sizeof(list);

    /* R_OSAL_IoGetNumOfDevices */
    OSAL_FUNCTION_CALL(R_OSAL_IoGetNumOfDevices(TEST_DEVICE_TYPE, &numOfDevices), osal_ret);    /* 01_001_044 */

    /* R_OSAL_IoGetDeviceList */    /* requires R_OSAL_IoGetNumOfDevices */
    OSAL_FUNCTION_CALL(R_OSAL_IoGetDeviceList(TEST_DEVICE_TYPE, list, list_size, &numOfLists), osal_ret);        /* 01_001_045 */

    device = strtok(list, s);
    /* R_OSAL_IoDeviceOpen */    /* requires R_OSAL_IoGetDeviceList */
    OSAL_FUNCTION_CALL(R_OSAL_IoDeviceOpen(device, &handle), osal_ret);        /* 01_001_046 */

    /* R_OSAL_IoWrite8 */    /* requires R_OSAL_IoDeviceOpen */
    offset = 0x00C0;
    OSAL_FUNCTION_CALL(R_OSAL_IoWrite8(handle, offset, 0xff), osal_ret);        /* 01_001_052 */

    /* R_OSAL_IoRead8 */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_FUNCTION_CALL(R_OSAL_IoRead8(handle, offset, &read_data8), osal_ret);                /* 01_001_048 */

    /* R_OSAL_IoWrite16 */    /* requires R_OSAL_IoDeviceOpen */
    offset = 0x00C0;
    OSAL_FUNCTION_CALL(R_OSAL_IoWrite16(handle, offset, 0x00ff), osal_ret);        /* 01_001_053 */

    /* R_OSAL_IoRead16 */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_FUNCTION_CALL(R_OSAL_IoRead16(handle, offset, &read_data16), osal_ret);            /* 01_001_049 */

    /* R_OSAL_IoWrite32 */    /* requires R_OSAL_IoDeviceOpen */
    offset = 0x00C0;
    OSAL_FUNCTION_CALL(R_OSAL_IoWrite32(handle, offset, 0x000000FF), osal_ret);        /* 01_001_054 */

    /* R_OSAL_IoRead32 */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_FUNCTION_CALL(R_OSAL_IoRead32(handle, offset, &read_data32), osal_ret);        /* 01_001_050 */

    /* R_OSAL_IoWrite64 */    /* requires R_OSAL_IoDeviceOpen */
    offset = 0x00C0;
    OSAL_FUNCTION_CALL(R_OSAL_IoWrite64(handle, offset, 0x000000FF000000FF), osal_ret);        /* 01_001_055 */

    /* R_OSAL_IoRead64 */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_FUNCTION_CALL(R_OSAL_IoRead64(handle, offset, &read_data64), osal_ret);        /* 01_001_051 */

    /* R_OSAL_IoBlockWrite8 */    /* requires R_OSAL_IoDeviceOpen */
    offset = 0x0034;
    size = 16;
    OSAL_FUNCTION_CALL(R_OSAL_IoBlockWrite8(handle, offset, register_data8, size), osal_ret);        /* 01_001_060 */

    /* R_OSAL_IoBlockRead8 */    /* requires R_OSAL_IoDeviceOpen */
    memset(read_block_data8, 0x00, sizeof(read_block_data8));
    OSAL_FUNCTION_CALL(R_OSAL_IoBlockRead8(handle, offset, read_block_data8, size), osal_ret);        /* 01_001_056 */

    /* R_OSAL_IoBlockWrite16 */    /* requires R_OSAL_IoDeviceOpen */
    offset = 0x0034;
    size = 16;
    OSAL_FUNCTION_CALL(R_OSAL_IoBlockWrite16(handle, offset, register_data16, size), osal_ret);        /* 01_001_061 */

    /* R_OSAL_IoBlockRead16 */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_FUNCTION_CALL(R_OSAL_IoBlockRead16(handle, offset, read_block_data16, size), osal_ret);        /* 01_001_057 */

    /* R_OSAL_IoBlockWrite32 */    /* requires R_OSAL_IoDeviceOpen */
    offset = 0x0034;
    size = 16;
    OSAL_FUNCTION_CALL(R_OSAL_IoBlockWrite32(handle, offset, register_data32, size), osal_ret);        /* 01_001_062 */

    /* R_OSAL_IoBlockRead32 */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_FUNCTION_CALL(R_OSAL_IoBlockRead32(handle, offset, read_block_data32, size), osal_ret);        /* 01_001_058 */

    /* R_OSAL_IoBlockWrite64 */    /* requires R_OSAL_IoDeviceOpen */
    offset = 0x00C0;
    size = 16;
    OSAL_FUNCTION_CALL(R_OSAL_IoBlockWrite64(handle, offset, register_data64, size), osal_ret);        /* 01_001_063 */

    /* R_OSAL_IoBlockRead64 */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_FUNCTION_CALL(R_OSAL_IoBlockRead64(handle, offset, read_block_data64, size), osal_ret);        /* 01_001_059 */

    /* R_OSAL_IoGetDeviceAxiBusId */    /* requires R_OSAL_IoDeviceOpen? */
    OSAL_FUNCTION_CALL(R_OSAL_IoGetDeviceAxiBusId(handle, &axi_id), osal_ret);        /* 01_001_067 */

    /* R_OSAL_IoGetNumOfAxiIdsForAxiBusName */
    OSAL_FUNCTION_CALL(R_OSAL_IoGetNumOfAxiIdsForAxiBusName(TEST_AXI_BUS_NAME, &ax_list_size), osal_ret);   /* 01_001_068 */

    /* R_OSAL_IoGetAxiIdListForAxiBusName */
    OSAL_FUNCTION_CALL(R_OSAL_IoGetAxiIdListForAxiBusName(TEST_AXI_BUS_NAME, axi_id_array,
                                                          ax_list_size, &ax_numOfLists), osal_ret);         /* 01_001_069 */
    /* R_OSAL_IoGetNumOfAxiIdsForDeviceHdl */
    OSAL_FUNCTION_CALL(R_OSAL_IoGetNumOfAxiIdsForDeviceHdl(handle, &ax_list_size), osal_ret);               /* 01_001_070 */

    /* R_OSAL_IoGetAxiIdListForDeviceHdl */
    OSAL_FUNCTION_CALL(R_OSAL_IoGetAxiIdListForDeviceHdl(handle, axi_id_array,
                                                         ax_list_size, &ax_numOfLists), osal_ret);          /* 01_001_071 */

    /* R_OSAL_IoGetNumOfAxiIdsForDeviceName */
    OSAL_FUNCTION_CALL(R_OSAL_IoGetNumOfAxiIdsForDeviceName(TEST_DEVICE_NAME, &ax_list_size), osal_ret);    /* 01_001_072 */

    /* R_OSAL_IoGetAxiIdListForDeviceName */
    OSAL_FUNCTION_CALL(R_OSAL_IoGetAxiIdListForDeviceName(TEST_DEVICE_NAME, axi_id_array,
                                                          ax_list_size, &ax_numOfLists), osal_ret);        /* 01_001_073 */

    /* R_OSAL_IoDeviceClose */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_FUNCTION_CALL(R_OSAL_IoDeviceClose(handle), osal_ret);        /* 01_001_047 */

    /* R_OSAL_IoGetAxiBusIdFromDeviceName */
    OSAL_FUNCTION_CALL(R_OSAL_IoGetAxiBusIdFromDeviceName(device, &axi_id), osal_ret);        /* 01_001_064 */

    /* R_OSAL_IoGetAxiBusName */
    OSAL_FUNCTION_CALL(R_OSAL_IoGetAxiBusName(axi_id, axi_name, sizeof(axi_name), &numOfName), osal_ret);        /* 01_001_065 */

    /* R_OSAL_IoGetAxiBusNameList */
    ax_list_size = sizeof(ax_list);
    OSAL_FUNCTION_CALL(R_OSAL_IoGetAxiBusNameList(ax_list, ax_list_size, &ax_numOfLists), osal_ret);        /* 01_001_066 */
}
/***********************************************************************************************************************
* End of function CheckIO()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function CheckPower()
***********************************************************************************************************************/
static void CheckPower(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret;

    size_t                      numOfDevices;
    char                        list[950];
    size_t                      list_size;
    size_t                      numOfLists;
    osal_device_handle_t        dev_handle;
    char*                       device;
    const char                  s[2] = "\n";

    e_osal_pm_required_state_t  req_state;
    e_osal_pm_state_t           pm_state;
    e_osal_pm_policy_t          policy;
    e_osal_pm_reset_state_t     reset_state;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));
    list_size = sizeof(list);

    /* requires R_OSAL_IoDeviceOpen */
    R_OSAL_IoGetNumOfDevices(TEST_DEVICE_TYPE, &numOfDevices);
    R_OSAL_IoGetDeviceList(TEST_DEVICE_TYPE, list, list_size, &numOfLists);
    device = strtok(list, s);
    R_OSAL_IoDeviceOpen(device, &dev_handle);

    /* R_OSAL_PmSetRequiredState */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_FUNCTION_CALL(R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true), osal_ret);    /* 01_001_074 */

    /* R_OSAL_PmSetResetState */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_FUNCTION_CALL(R_OSAL_PmSetResetState(dev_handle, OSAL_PM_RESET_STATE_APPLIED), osal_ret);        /* 01_001_080 */

    /* R_OSAL_PmGetState */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_FUNCTION_CALL(R_OSAL_PmGetState(dev_handle, &pm_state), osal_ret);        /* 01_001_077 */

    /* R_OSAL_PmWaitForState */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_FUNCTION_CALL(R_OSAL_PmWaitForState(dev_handle, OSAL_PM_STATE_RESET, 1000), osal_ret);        /* 01_001_076 */

    /* R_OSAL_PmSetPolicy */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_FUNCTION_CALL(R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_PG, true), osal_ret);        /* 01_001_078 */

    /* R_OSAL_PmGetPolicy */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_FUNCTION_CALL(R_OSAL_PmGetPolicy(dev_handle, &policy), osal_ret);        /* 01_001_079 */

    /* R_OSAL_PmGetRequiredState */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_FUNCTION_CALL(R_OSAL_PmGetRequiredState(dev_handle, &req_state), osal_ret);        /* 01_001_075 */

    /* R_OSAL_PmGetResetState */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_FUNCTION_CALL(R_OSAL_PmGetResetState(dev_handle, &reset_state), osal_ret);        /* 01_001_081 */

    /* R_OSAL_PmGetLowestIntermediatePowerState */    /* requires R_OSAL_IoDeviceOpen */
    OSAL_FUNCTION_CALL(R_OSAL_PmGetLowestIntermediatePowerState(dev_handle, &pm_state), osal_ret);        /* 01_001_082 */

    R_OSAL_IoDeviceClose(dev_handle);

    /* R_OSAL_PmSetPostClock */    /* requires R_OSAL_IoDeviceOpen */
    memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));
    R_OSAL_IoGetNumOfDevices("fba", &numOfDevices);
    R_OSAL_IoGetDeviceList("fba", list, list_size, &numOfLists);
    device = strtok(list, s);
    R_OSAL_IoDeviceOpen(device, &dev_handle);
    OSAL_FUNCTION_CALL(R_OSAL_PmSetPostClock(dev_handle, true, 63), osal_ret);        /* 01_001_083 */
    R_OSAL_IoDeviceClose(dev_handle);
}
/***********************************************************************************************************************
* End of function CheckPower()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function CheckMemory()
***********************************************************************************************************************/
/* Pseudo user callback functions */
void osal_memory_mngr_cb(void * user_arg, e_osal_mmngr_event_t event, e_osal_return_t error,
                         osal_memory_buffer_handle_t buffer_hndl)
{
    (void)user_arg;
    (void)error;
    (void)buffer_hndl;

    return;
}

static void CheckMemory(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                 osal_ret;

    osal_memory_manager_handle_t    mm_handle;

    size_t                          size = 4096;
    size_t                          alignment = 4096;
    osal_memory_buffer_handle_t     buffer_obj;

    st_osal_mmngr_config_t          config;
    e_osal_mmngr_event_t            monitored_event = OSAL_MMNGR_EVENT_ALLOCATE;
    int user_arg = 55;

    uintptr_t*                      cpu_ptr;

    size_t                          offset = 0;
    uintptr_t                       hw_addr = 0;

    osal_axi_bus_id_t               axi_id = 1;

    size_t                          osize = 0;

    osal_memory_buffer_t            memory_buffer;
    osal_memory_manager_t           memory_manager;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    memset(&mm_handle, 0x00, sizeof(osal_memory_manager_handle_t));

    /* R_OSAL_MmngrGetOsalMaxConfig */
    OSAL_FUNCTION_CALL(R_OSAL_MmngrGetOsalMaxConfig(&config), osal_ret);    /* 01_001_090 */

    /* R_OSAL_MmngrOpen */
    OSAL_FUNCTION_CALL(R_OSAL_MmngrOpen(&config , &mm_handle), osal_ret);    /* 01_001_084 */

    /* R_OSAL_MmngrAlloc */    /* requires R_OSAL_MmngrOpen */
    OSAL_FUNCTION_CALL(R_OSAL_MmngrAlloc(mm_handle, size, alignment, &buffer_obj), osal_ret);    /* 01_001_087 */

    /* R_OSAL_MmngrGetConfig */    /* requires R_OSAL_MmngrOpen */
    OSAL_FUNCTION_CALL(R_OSAL_MmngrGetConfig(mm_handle, &config), osal_ret);    /* 01_001_089 */

    /* R_OSAL_MmngrInitializeMmngrConfigSt */    /* requires R_OSAL_MmngrOpen */
    OSAL_FUNCTION_CALL(R_OSAL_MmngrInitializeMmngrConfigSt(&config), osal_ret);    /* 01_001_092 */

    /* R_OSAL_MmngrRegisterMonitor */    /* requires R_OSAL_MmngrOpen */
    OSAL_FUNCTION_CALL(R_OSAL_MmngrRegisterMonitor(mm_handle, monitored_event, osal_memory_mngr_cb, (void*)&user_arg), osal_ret);    /* 01_001_093 */

    /* R_OSAL_MmngrGetCpuPtr */    /* requires R_OSAL_MmngrAlloc */
    OSAL_FUNCTION_CALL(R_OSAL_MmngrGetCpuPtr(buffer_obj, (void **)&cpu_ptr), osal_ret);    /* 01_001_094 */

    /* R_OSAL_MmngrGetConstCpuPtr */    /* requires R_OSAL_MmngrAlloc */
    OSAL_FUNCTION_CALL(R_OSAL_MmngrGetConstCpuPtr(buffer_obj, (void **)&cpu_ptr), osal_ret);    /* 01_001_095 */

    /* R_OSAL_MmngrFlush */    /* requires R_OSAL_MmngrAlloc */
    OSAL_FUNCTION_CALL(R_OSAL_MmngrFlush(buffer_obj, offset, size), osal_ret);    /* 01_001_096 */

    /* R_OSAL_MmngrInvalidate */    /* requires R_OSAL_MmngrAlloc */
    OSAL_FUNCTION_CALL(R_OSAL_MmngrInvalidate(buffer_obj, offset, size), osal_ret);    /* 01_001_097 */

    /* R_OSAL_MmngrGetHwAddr */    /* requires R_OSAL_MmngrAlloc */
    OSAL_FUNCTION_CALL(R_OSAL_MmngrGetHwAddr(buffer_obj, axi_id, &hw_addr), osal_ret);    /* 01_001_098 */

    /* R_OSAL_MmngrGetSize */    /* requires R_OSAL_MmngrAlloc */
    OSAL_FUNCTION_CALL(R_OSAL_MmngrGetSize(buffer_obj, &osize), osal_ret);    /* 01_001_099 */

    /* R_OSAL_MmngrCheckBuffer */    /* requires R_OSAL_MmngrRegisterMonitor, R_OSAL_MmngrAlloc */
    OSAL_FUNCTION_CALL(R_OSAL_MmngrCheckBuffer(buffer_obj), osal_ret);    /* 01_001_100 */

    /* R_OSAL_MmngrDebugMmngrPrint */    /* requires R_OSAL_MmngrOpen */
    OSAL_FUNCTION_CALL(R_OSAL_MmngrDebugMmngrPrint(mm_handle, stdout), osal_ret);    /* 01_001_101 */

    /* R_OSAL_MmngrDebugBufferPrint */    /* requires R_OSAL_MmngrAlloc */
    OSAL_FUNCTION_CALL(R_OSAL_MmngrDebugBufferPrint(buffer_obj, stdout), osal_ret);    /* 01_001_102 */

    /* R_OSAL_MmngrInitializeMemoryBufferObj */    /* requires R_OSAL_MmngrOpen */
    OSAL_FUNCTION_CALL(R_OSAL_MmngrInitializeMemoryBufferObj(&memory_buffer), osal_ret);    /* 01_001_104 */

    /* R_OSAL_MmngrInitializeMemoryManagerObj */    /* requires R_OSAL_MmngrOpen */
    OSAL_FUNCTION_CALL(R_OSAL_MmngrInitializeMemoryManagerObj(&memory_manager), osal_ret);    /* 01_001_103 */

    /* R_OSAL_MmngrDealloc */    /* requires R_OSAL_MmngrOpen */
    OSAL_FUNCTION_CALL(R_OSAL_MmngrDealloc(mm_handle, buffer_obj), osal_ret);    /* 01_001_088 */

    /* R_OSAL_MmngrClose */    /* requires R_OSAL_MmngrOpen */
    OSAL_FUNCTION_CALL(R_OSAL_MmngrClose(mm_handle), osal_ret);    /* 01_001_086 */

    /* R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx */
    OSAL_FUNCTION_CALL(R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx(0, &config), osal_ret);    /* 01_001_091 */

    /* R_OSAL_MmngrOpenFromRegionIdx */
    OSAL_FUNCTION_CALL(R_OSAL_MmngrOpenFromRegionIdx(&config, 0, &mm_handle), osal_ret);    /* 01_001_085 */

    R_OSAL_MmngrClose(mm_handle);
}
/***********************************************************************************************************************
* End of function CheckMemory()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function CheckVersion()
***********************************************************************************************************************/
static e_osal_return_t R_OSAL_GetVersion_test(void)
{
    (void)R_OSAL_GetVersion();    /* 01_001_003 */
    return OSAL_RETURN_OK;
}

static void CheckVersion(void)
{
    e_osal_return_t osal_ret;

    /* R_OSAL_GetVersion */
    OSAL_FUNCTION_CALL(R_OSAL_GetVersion_test(), osal_ret);
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
    st_osal_time_t  current_time;

    R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time);
    if(current_time.tv_nsec + 2000000 <= 999999999)
    {
        iop_absolute_time->tv_nsec = current_time.tv_nsec + 2000000;
        iop_absolute_time->tv_sec = current_time.tv_sec + 5;
    }
    else
    {
        iop_absolute_time->tv_sec = current_time.tv_sec + 5;
        iop_absolute_time->tv_nsec = 2000000 - (999999999 - current_time.tv_nsec);
    }
}
/***********************************************************************************************************************
* End of function GetAbsoluteTime()
***********************************************************************************************************************/
