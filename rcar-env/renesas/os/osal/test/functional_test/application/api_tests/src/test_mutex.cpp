/***********************************************************************************************************************
* Copyright [2021-2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    test_mutex.cpp
* Version :      3.2.1
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Threadsync(Mutex) Manager API Functional Test (From the perspective of OSAL API Specification)
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           2.0.0     29.10.2021    Create New
 *           2.0.1     15.11.2021    Update Test Name
 *                                   Mutex -> Mutex_TC_12_001_003
 *           2.0.2     18.11.2021    Fix Typo
 *           3.0.0     08.03.2022    Apply new/updated device definition related to interrupt context test
 *                                   Apply updated allocate_setup_data_memory, setup_hw_register and deallocate_data_memory
 *           3.0.1     11.03.2022    Remove "#if defined" used to disable test on SIL
 *           3.1.0     04.01.2023    Add ThsyncMutexUnlock_TC_02_006_009
 *                                   Add handle value initialize for below tests:
 *                                      - ThsyncMutexCreate_TC_02_001_004
 *           3.2.0     01.02.2023    Add new boundary test cases as below:
 *                                      - ThsyncMutexLockUntilTimeStamp_TC_02_004_016
 *                                      - ThsyncMutexLockUntilTimeStamp_TC_02_004_017
 *                                      - ThsyncMutexLockUntilTimeStamp_TC_02_004_018
 *                                      - ThsyncMutexLockUntilTimeStamp_TC_02_004_019
 *           3.2.1     10.04.2023    Update expected value for median time for SIL Windows performance test
***********************************************************************************************************************/
#include "test_common.hpp"
#if !defined(OSAL_TEST_FRAMEWORK)
using namespace std;
#else
using namespace OsalFWHelper;
#endif

/* armclang will be warnig, when specified OsalApiStateTest to TEST_F */
/* W.A Derive a class and replace it by define */
class OsalApiStateTest_MTX : public OsalApiStateTest
{
    /* There is no need for member variables and member functions.
       We added them just for the sake of adding a test class. */
};
#define OsalApiStateTest OsalApiStateTest_MTX

/* armclang will be warnig, when specified OSALINTTESTS to TEST_F */
/* W.A Derive a class and replace it by define */
class OSALINTTESTS_MTX : public OSALINTTESTS
{
    /* There is no need for member variables and member functions.
       We added them just for the sake of adding a test class. */
};
#define OSALINTTESTS OSALINTTESTS_MTX

#define MAX_THREAD 10 // maximum depends on platform
#define MAX_ITERATIONS 0xf // maximum = RAND_MAX

osal_mutex_handle_t mtx_threads;
osal_cond_handle_t  cv_mtx;

// [non_reentrant_function]
void non_reentrant_function(void)
{
    static volatile int count = 0;
    count++;
    EXPECT_EQ(R_OSAL_ThreadSleepForTimePeriod(rand() % 1001),
              OSAL_RETURN_OK); // procrastinate here for a random time between 0 and 1 sec
    EXPECT_EQ(count, 1);       // check if any other thread entered the function while sleeping
    count--;
    return;
}
// [non_reentrant_function]

// [workerFunc]
int64_t workerFunc(void * userArg)
{
    (void) userArg;
    e_osal_return_t osal_ret;
    st_osal_time_t  timestamp;

    for (int i = 0; i < (rand() % MAX_ITERATIONS) + 1; i++)
    {
        // wait on mutex an lock it
        osal_ret = OSAL_RETURN_FAIL;
        while (osal_ret != OSAL_RETURN_OK)
        {
            if (i % 2 == 0)
            {
                osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(mtx_threads, 1);
            }
            else
            {
                EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_PRECISION, &timestamp), OSAL_RETURN_OK);
                timestamp.tv_nsec += 1 * 1000 * 1000; //+1ms
                if(timestamp.tv_nsec >= (1 * 1000 * 1000 * 1000))
                {
                    timestamp.tv_sec += 1;
                    timestamp.tv_nsec %= (1 * 1000 * 1000 * 1000);
                }
                osal_ret = R_OSAL_ThsyncMutexLockUntilTimeStamp(mtx_threads, &timestamp);
            }
            EXPECT_TRUE((osal_ret == OSAL_RETURN_OK) || (osal_ret == OSAL_RETURN_TIME));
        }

        // mutex is locked, change the mutual resource or execute non-reentrant operation
        non_reentrant_function();

        // unlock the mutex
        EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_threads), OSAL_RETURN_OK);
    }

    return 0;
}
// [workerFunc]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @test
 * @test @link ThreadSyncMutex_Mutex_TC_12_001_003 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexCreate_TC_02_001_001 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexCreate_TC_02_001_002 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexCreate_TC_02_001_003 @endlink
 * @test @link OsalApiStateTest_ThsyncMutexCreate_TC_02_001_004 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexDestroy_TC_02_002_001 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexDestroy_TC_02_002_002 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexDestroy_TC_02_002_003 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexDestroy_TC_02_002_004 @endlink
 * @test @link OsalApiStateTest_ThsyncMutexDestroy_TC_02_002_005 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexDestroy_TC_02_002_006 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexDestroy_TC_02_002_007 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexLockForTimePeriod_TC_02_003_001 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexLockForTimePeriod_TC_02_003_002 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexLockForTimePeriod_TC_02_003_003 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexLockForTimePeriod_TC_02_003_004 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexLockForTimePeriod_TC_02_003_005 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexLockForTimePeriod_TC_02_003_006 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexLockForTimePeriod_TC_02_003_007 @endlink
 * @test @link OsalApiStateTest_ThsyncMutexLockForTimePeriod_TC_02_003_008 @endlink
 * @test @link OSALINTTESTS_ThsyncMutexLockForTimePeriod_TC_02_003_009 @endlink
 * @test @link OSALINTTESTS_ThsyncMutexLockForTimePeriod_TC_02_003_010 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexLockForTimePeriod_TC_02_003_011 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexLockForTimePeriod_TC_02_003_012 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexLockUntilTimeStamp_TC_02_004_001 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexLockUntilTimeStamp_TC_02_004_002 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexLockUntilTimeStamp_TC_02_004_003 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexLockUntilTimeStamp_TC_02_004_004 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexLockUntilTimeStamp_TC_02_004_005 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexLockUntilTimeStamp_TC_02_004_006 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexLockUntilTimeStamp_TC_02_004_007 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexLockUntilTimeStamp_TC_02_004_008 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexLockUntilTimeStamp_TC_02_004_009 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexLockUntilTimeStamp_TC_02_004_010 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexLockUntilTimeStamp_TC_02_004_011 @endlink
 * @test @link OsalApiStateTest_ThsyncMutexLockUntilTimeStamp_TC_02_004_012 @endlink
 * @test @link OSALINTTESTS_ThsyncMutexLockUntilTimeStamp_TC_02_004_013 @endlink
 * @test @link OSALINTTESTS_ThsyncMutexLockUntilTimeStamp_TC_02_004_014 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexLockUntilTimeStamp_TC_02_004_015 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexLockUntilTimeStamp_TC_02_004_016 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexLockUntilTimeStamp_TC_02_004_017 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexLockUntilTimeStamp_TC_02_004_018 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexLockUntilTimeStamp_TC_02_004_019 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexTryLock_TC_02_005_001 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexTryLock_TC_02_005_002 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexTryLock_TC_02_005_003 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexTryLock_TC_02_005_004 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexTryLock_TC_02_005_005 @endlink
 * @test @link OsalApiStateTest_ThsyncMutexTryLock_TC_02_005_006 @endlink
 * @test @link OSALINTTESTS_ThsyncMutexTryLock_TC_02_005_007 @endlink
 * @test @link OSALINTTESTS_ThsyncMutexTryLock_TC_02_005_008 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexUnlock_TC_02_006_001 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexUnlock_TC_02_006_002 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexUnlock_TC_02_006_003 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexUnlock_TC_02_006_004 @endlink
 * @test @link OsalApiStateTest_ThsyncMutexUnlock_TC_02_006_005 @endlink
 * @test @link OSALINTTESTS_ThsyncMutexUnlock_TC_02_006_006 @endlink
 * @test @link OSALINTTESTS_ThsyncMutexUnlock_TC_02_006_007 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexUnlock_TC_02_006_008 @endlink
 * @test @link ThreadSyncMutexApiTest_ThsyncMutexUnlock_TC_02_006_009 @endlink
 * @section ThreadSync_Mutex ThreadSync_Mutex function
 * @subsection ThreadSyncMutex_SetUp ThreadSyncMutex Setup
 * Common to Test Suit of ThreadSync_Mutex, prepare process to be executed before executing the test. <br>
 * For example:
 * @snippet test_mutex.cpp ThreadSyncMutex_SetUp
 * @snippet test_common.hpp OSALTESTS_SetUp
 * @subsection ThreadSyncMutex_TearDown ThreadSyncMutex TearDown
 * Common to Test Suit of ThreadSync_Mutex, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_mutex.cpp ThreadSyncMutex_TearDown
 * @snippet test_common.hpp OSALTESTS_TearDown
 * @section ThreadSyncMutexApiTest ThreadSyncMutexApiTest function
 * @subsection ThreadSyncMutexApiTest_SetUp ThreadSyncMutexApiTest Setup
 * Common to Test Suit of ThreadSyncMutexApiTest, prepare process to be executed before executing the test. <br>
 * For example:
 * @snippet test_common.hpp OSALTESTS_SetUp
 * @subsection ThreadSyncMutexApiTest_TearDown ThreadSyncMutexApiTest TearDown
 * Common to Test Suit of ThreadSyncMutexApiTest, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_common.hpp OSALTESTS_TearDown
 * @section OSALINTTESTS_ThreadSyncMutexApiTest OSALINTTESTS of ThreadSyncMutexApiTest function
 * @subsection OSALINTTESTS_ThreadSyncMutexApiTest_SetUp OSALINTTESTS of ThreadSyncMutexApiTest Setup
 * Common to Test Suit of OSALINTTESTS ThreadSyncMutexApiTest, prepare process to be executed before executing the test. <br>
 * This test suite generates interrupts. These process is preparation<br>
 * For example:
 * @snippet test_common.hpp OSALINTTESTS_SetUp
 * @subsection OSALINTTESTS_ThreadSyncMutexApiTest_TearDown OSALINTTESTS of ThreadSyncMutexApiTest TearDown
 * Common to Test Suit of OSALINTTESTS ThreadSyncMutexApiTest, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_common.hpp OSALINTTESTS_TearDown
 * @section OsalApiStateTest_ThreadSyncMutexApiTest OsalApiStateTest of ThreadSyncMutexApiTest function
 * @subsection OsalApiStateTest_ThreadSyncMutexApiTest_SetUp OsalApiStateTest of ThreadSyncMutexApiTest Setup
 * Common to Test Suit of OsalApiStateTest ThreadSyncMutexApiTest, prepare process to be executed before executing the test. <br>
 * For example:
 * @snippet test_common.hpp OsalApiStateTest_SetUp
 * @subsection OsalApiStateTest_ThreadSyncMutexApiTest_TearDown OsalApiStateTest of ThreadSyncMutexApiTest TearDown
 * Common to Test Suit of OsalApiStateTest ThreadSyncMutexApiTest, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_common.hpp OsalApiStateTest_TearDown
 */
class ThreadSync : public OSALTESTS
{
protected:
    osal_thread_handle_t    threads[MAX_THREAD];
    int                     numOfThreads;
    char                    thread_name[100];
    st_osal_thread_config_t th_config;

// [ThreadSyncMutex_SetUp]
    void SetUp() override
    {
        OSALTESTS::SetUp();

        ASSERT_EQ(R_OSAL_ThsyncMutexCreate(TEST_MUTEX_ID1, &mtx_threads), OSAL_RETURN_OK);
        ASSERT_EQ(R_OSAL_ThsyncCondCreate(TEST_COND_ID1, &cv_mtx), OSAL_RETURN_OK);

        numOfThreads = MAX_THREAD;

        th_config.func     = workerFunc;
        th_config.userarg  = NULL_PTR;
        th_config.priority = OSAL_THREAD_PRIORITY_LOWEST;
        th_config.stack_size = THREAD_STACK_SIZE;
    }
// [ThreadSyncMutex_SetUp]

// [ThreadSyncMutex_TearDown]
    void TearDown() override
    {
        ASSERT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_threads), OSAL_RETURN_OK);
        ASSERT_EQ(R_OSAL_ThsyncCondDestroy(cv_mtx), OSAL_RETURN_OK);

        OSALTESTS::TearDown();
    }
// [ThreadSyncMutex_TearDown]
};

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutex_Mutex_TC_12_001_003 ThreadSyncMutex.Mutex_TC_12_001_003
 * @subsection Mutex_tsi Test Suite ID
 * ThreadSyncMutex
 * @subsection Mutex_tci Test Case ID
 * Mutex_S3_N_TC_12_001_003
 * @subsection Mutex_rr Related Requirement(s)
 * \#199150, \#199153, \#199154, \#199156, \#199157
 * @subsection Mutex_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection Mutex_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to create/destroy a mutex
 * @subsection Mutex_tpts Test Procedure/Test Steps
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate function with the argument as below:
 *      - Mutex id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Create a new condition variable using R_OSAL_ThsyncCondCreate function with the argument as below:
 *      -# condition variable id is TEST_COND_ID1
 *      -# Address of condition handle
 * -# Create 10 threads using R_OSAL_ThreadCreate function with the arguments as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_LOWEST
 *          -# task_name is "osal_thread_" + Unique number got by Thread id
 *          -# func is workerFunc function with functionality:
 *              - Start a for loop with loop number is a random number.
 *              - If loop count is a even number, lock a mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 *              - Otherwise, if loop count is odd number, get the curent time stamp with R_OSAL_ClockTimeGetTimeStamp. <br>
 *                Lock the mutex by using R_OSAL_ThsyncMutexLockUntilTimeStamp.
 *              - Now mutex is locked, try to execute non-reentrant operation by calling to non_reentrant_function():
 *                  1. Increase "count" which is a static volatile int type by one.
 *                  2. Sleep a random time between 0 and 1 sec by using R_OSAL_ThreadSleepForTimePeriod
 *                  3. Comapre "count" with 1.<br>
 *                    Decrease "count" by 1.
 *              - Unlock the mutex with R_OSAL_ThsyncMutexUnlock.
 *          -# userarg is Null pointer
 *          -# stack_size is THREAD_STACK_SIZE
 *      - Thread id is from TEST_THREAD_ID1 to TEST_THREAD_ID10
 *      - Address of thread handle
 * -# Join all created thread with R_OSAL_ThreadJoin.
 * -# Destroy the mutex using R_OSAL_ThsyncMutexDestroy function.
 * -# Destroy the condition variable using R_OSAL_ThsyncCondDestroy function.
 * @subsection Mutex_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK for each thread.<br>
 * In workerFunc function of each thread. <br>
 *  - Return value of R_OSAL_ThsyncMutexLockForTimePeriod is equal to OSAL_RETURN_OK or OSAL_RETURN_TIME.
 *  - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK. <br>
 * Return value of R_OSAL_ThsyncMutexLockUntilTimeStamp is equal to OSAL_RETURN_OK or OSAL_RETURN_TIME.
 *  - In non_reentrant_function():
 *      -# R_OSAL_ThreadSleepForTimePeriod return OSAL_RETURN_OK
 *      -# "count" is equal to 1
 *  - R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK. <br>
 * Value of "ret_val" is equal to 0.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK.
 * @subsection Mutex_testcase Test Case Example
 * @snippet test_mutex.cpp Mutex_TC_12_001_003
 */
// [Mutex_TC_12_001_003]
TEST_F(ThreadSync, Mutex_TC_12_001_003)
{
    osal_thread_id_t id;
    int64_t          expected_value = 0;
    int64_t          ret_val;
    char             index_str[10];

    for (auto i = 0; i < numOfThreads; i++)
    {
        id                  = TEST_THREAD_ID1 + i;
        strcpy(thread_name, "osal_thread_");
        sprintf(index_str, "%d", i);
        strcat(thread_name, index_str);
        th_config.task_name = thread_name;

        ASSERT_EQ(R_OSAL_ThreadCreate(&th_config, id, &threads[i]), OSAL_RETURN_OK);
    }

    for (auto i = 0; i < numOfThreads; i++)
    {
        EXPECT_EQ(R_OSAL_ThreadJoin(threads[i], &ret_val), OSAL_RETURN_OK);
        EXPECT_EQ(ret_val, expected_value);
    }
}
// [Mutex_TC_12_001_003]

class ThreadSyncMutexApiTest : public OSALTESTS
{
protected:
    /* RVC implementation base on Linux OSAL IT*/
    osal_mutex_handle_t mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
    osal_mutex_handle_t mtx_handle1 = OSAL_MUTEX_HANDLE_INVALID;
    osal_thread_handle_t thread_handle = OSAL_THREAD_HANDLE_INVALID;
};

/* RVC implementation base on Linux OSAL IT*/
struct mutexlock_worker_arg
{
    mutexlock_worker_arg(osal_mutex_handle_t * handle) : mutex_handle(handle)
    {
    }
    osal_mutex_handle_t * mutex_handle;
};

struct mutexperiod_worker_arg_timemeasurement1
{
    mutexperiod_worker_arg_timemeasurement1(osal_mutex_handle_t * mtx_handle, osal_nano_sec_t* d_time_10ms)
    {
        mutex_handle = mtx_handle;
        diff_time_10ms = d_time_10ms;
    }
    osal_mutex_handle_t* mutex_handle;
    osal_nano_sec_t* diff_time_10ms;
};

struct mutexperiod_worker_arg_timemeasurement2
{
    mutexperiod_worker_arg_timemeasurement2(osal_mutex_handle_t * mtx_handle, osal_mutex_handle_t * mtx_handle1, 
                                        osal_nano_sec_t* d_time_0ms_TIME, osal_nano_sec_t* d_time_0ms_OK)
    {
        mutex_handle = mtx_handle;
        mutex_handle1 = mtx_handle1;
        diff_time_0ms_TIME = d_time_0ms_TIME;
        diff_time_0ms_OK = d_time_0ms_OK;
    }
    osal_mutex_handle_t* mutex_handle;
    osal_mutex_handle_t* mutex_handle1;
    osal_nano_sec_t* diff_time_0ms_TIME;
    osal_nano_sec_t* diff_time_0ms_OK;
};

// [mutextimedlock_workerfunction]
int64_t mutextimedlock_workerfunction(void * arg)
{
    mutexlock_worker_arg *testStruct = (mutexlock_worker_arg *)arg;
    osal_milli_sec_t time = 10;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(*(testStruct->mutex_handle), time);
    EXPECT_EQ(osal_ret, OSAL_RETURN_TIME);
    return (osal_ret == OSAL_RETURN_TIME)? (int64_t) 0: (int64_t) -1;
}
// [mutextimedlock_workerfunction]

// [mutexabsolutelock_workerfunction]
int64_t mutexabsolutelock_workerfunction(void * arg)
{
    mutexlock_worker_arg *testStruct = (mutexlock_worker_arg *)arg;
    st_osal_time_t abs_time;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time), OSAL_RETURN_OK);
    if(abs_time.tv_nsec + 2 * MSEC_TO_NSEC <= MAX_NSEC)
    {
        abs_time.tv_nsec = abs_time.tv_nsec + 2 * MSEC_TO_NSEC;
        abs_time.tv_sec += 1;
    }
    else
    {
        abs_time.tv_nsec = 2 * MSEC_TO_NSEC - (MAX_NSEC - abs_time.tv_nsec);
        abs_time.tv_sec += 2;
    }
    osal_ret = R_OSAL_ThsyncMutexLockUntilTimeStamp(*(testStruct->mutex_handle), &abs_time);
    EXPECT_EQ(osal_ret, OSAL_RETURN_TIME) << "Test02_004_009 failed";
    return (osal_ret == OSAL_RETURN_TIME)? (int64_t) 0: (int64_t) -1;
}
// [mutexabsolutelock_workerfunction]

// [mutextrylock_workerfunction]
int64_t mutextrylock_workerfunction(void * arg)
{
    mutexlock_worker_arg *testStruct = (mutexlock_worker_arg *)arg;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_ret = R_OSAL_ThsyncMutexTryLock(*(testStruct->mutex_handle));
    EXPECT_EQ(osal_ret, OSAL_RETURN_BUSY) << "Test02_005_003 failed";
    return (osal_ret == OSAL_RETURN_BUSY)? (int64_t) 0: (int64_t) -1;
}
// [mutextrylock_workerfunction]

// [mutexunlock_workerfunction]
int64_t mutexunlock_workerfunction(void * arg)
{
    mutexlock_worker_arg *testStruct = (mutexlock_worker_arg *)arg;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_ret = R_OSAL_ThsyncMutexUnlock(*(testStruct->mutex_handle));
    EXPECT_EQ(osal_ret, OSAL_RETURN_FAIL);
    return (int64_t)osal_ret;
}
// [mutexunlock_workerfunction]

// [osal_mutex_isr_test_normal_TC_02_003_009]
void osal_mutex_isr_test_normal_TC_02_003_009(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    osal_mutex_handle_t* local_mutex = (osal_mutex_handle_t*)local_arg->additional_arg;
    osal_milli_sec_t time_0ms = 0;
    uintptr_t offset = TEST_REG_CLEAR_REGISTER;
    uint32_t write_data = TEST_CLEAR_REGISTER_VALUE;
    osal_milli_sec_t time = 1;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, offset, write_data), OSAL_RETURN_OK);

    // Test02_003_009
    // @brief call R_OSAL_ThsyncMutexLockForTimePeriod in ISR
    time = 0;
    EXPECT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(*local_mutex, time), OSAL_RETURN_OK) << "Test02_003_009 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(*local_mutex), OSAL_RETURN_OK);

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, time_0ms, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_mutex_isr_test_normal_TC_02_003_009]

// [osal_mutex_isr_test_normal_TC_02_004_013]
void osal_mutex_isr_test_normal_TC_02_004_013(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    osal_mutex_handle_t* local_mutex = (osal_mutex_handle_t*)local_arg->additional_arg;
    osal_milli_sec_t time_0ms = 0;
    st_osal_time_t abs_time;
    uintptr_t offset = TEST_REG_CLEAR_REGISTER;
    uint32_t write_data = TEST_CLEAR_REGISTER_VALUE;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, offset, write_data), OSAL_RETURN_OK);

    // Test02_004_013
    // @brief call R_OSAL_ThsyncMutexLockUntilTimeStamp in ISR
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncMutexLockUntilTimeStamp(*local_mutex, &abs_time), OSAL_RETURN_OK) << "Test02_004_013 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(*local_mutex), OSAL_RETURN_OK);

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, time_0ms, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_mutex_isr_test_normal_TC_02_004_013]

// [osal_mutex_isr_test_normal_TC_02_005_007]
void osal_mutex_isr_test_normal_TC_02_005_007(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    osal_mutex_handle_t* local_mutex = (osal_mutex_handle_t*)local_arg->additional_arg;
    osal_milli_sec_t time_0ms = 0;
    uintptr_t offset = TEST_REG_CLEAR_REGISTER;
    uint32_t write_data = TEST_CLEAR_REGISTER_VALUE;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, offset, write_data), OSAL_RETURN_OK);

    // Test02_005_007
    // @brief call R_OSAL_ThsyncMutexTryLock in ISR
    EXPECT_EQ(R_OSAL_ThsyncMutexTryLock(*local_mutex), OSAL_RETURN_OK) << "Test02_005_007 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(*local_mutex), OSAL_RETURN_OK);

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, time_0ms, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_mutex_isr_test_normal_TC_02_005_007]

// [osal_mutex_isr_test_normal_TC_02_006_006]
void osal_mutex_isr_test_normal_TC_02_006_006(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    osal_mutex_handle_t* local_mutex = (osal_mutex_handle_t*)local_arg->additional_arg;
    osal_milli_sec_t time_0ms = 0;
    uintptr_t offset = TEST_REG_CLEAR_REGISTER;
    uint32_t write_data = TEST_CLEAR_REGISTER_VALUE;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, offset, write_data), OSAL_RETURN_OK);

    // Test02_006_006
    // @brief call R_OSAL_ThsyncMutexUnlock in ISR
    EXPECT_EQ(R_OSAL_ThsyncMutexTryLock(*local_mutex), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(*local_mutex), OSAL_RETURN_OK) << "Test02_006_006 failed";

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, time_0ms, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_mutex_isr_test_normal_TC_02_006_006]

// [osal_mutex_isr_test_abnormal1]
void osal_mutex_isr_test_abnormal1(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    osal_mutex_handle_t* local_mutex = (osal_mutex_handle_t*)local_arg->additional_arg;
    uintptr_t offset = TEST_REG_CLEAR_REGISTER;
    uint32_t write_data = TEST_CLEAR_REGISTER_VALUE;
    osal_milli_sec_t time = 1;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, offset, write_data), OSAL_RETURN_OK);

    // Test02_003_010
    // @brief call R_OSAL_ThsyncMutexLockForTimePeriod in ISR with a locked mutex
    time = 0;
    EXPECT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(*local_mutex, time), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(*local_mutex, time), OSAL_RETURN_BUSY) << "Test02_003_010 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(*local_mutex), OSAL_RETURN_OK);

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, 0, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_mutex_isr_test_abnormal1]

// [osal_mutex_isr_test_abnormal2]
void osal_mutex_isr_test_abnormal2(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    osal_mutex_handle_t* local_mutex = (osal_mutex_handle_t*)local_arg->additional_arg;
    st_osal_time_t abs_time;
    uintptr_t offset = TEST_REG_CLEAR_REGISTER;
    uint32_t write_data = TEST_CLEAR_REGISTER_VALUE;
    osal_milli_sec_t time = 1;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, offset, write_data), OSAL_RETURN_OK);

    // Test02_004_014
    // @brief call R_OSAL_ThsyncMutexLockUntilTimeStamp in ISR with a locked mutex
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncMutexLockUntilTimeStamp(*local_mutex, &abs_time), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncMutexLockUntilTimeStamp(*local_mutex, &abs_time), OSAL_RETURN_BUSY) << "Test02_004_014 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(*local_mutex), OSAL_RETURN_OK);

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, 0, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_mutex_isr_test_abnormal2]

// [osal_mutex_isr_test_abnormal3]
void osal_mutex_isr_test_abnormal3(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    osal_mutex_handle_t* local_mutex = (osal_mutex_handle_t*)local_arg->additional_arg;
    uintptr_t offset = TEST_REG_CLEAR_REGISTER;
    uint32_t write_data = TEST_CLEAR_REGISTER_VALUE;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, offset, write_data), OSAL_RETURN_OK);

    // Test02_005_008
    // @brief call R_OSAL_ThsyncMutexTryLock in ISR with a locked mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexTryLock(*local_mutex), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncMutexTryLock(*local_mutex), OSAL_RETURN_BUSY) << "Test02_005_008 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(*local_mutex), OSAL_RETURN_OK);

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, 0, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_mutex_isr_test_abnormal3]

// [osal_mutex_isr_test_abnormal4]
void osal_mutex_isr_test_abnormal4(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    osal_mutex_handle_t* local_mutex = (osal_mutex_handle_t*)local_arg->additional_arg;
    uintptr_t offset = TEST_REG_CLEAR_REGISTER;
    uint32_t write_data = TEST_CLEAR_REGISTER_VALUE;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, offset, write_data), OSAL_RETURN_OK);

    // Test02_006_007
    // @brief call R_OSAL_ThsyncMutexUnlock in ISR with unlocked mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexTryLock(*local_mutex), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(*local_mutex), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(*local_mutex), OSAL_RETURN_FAIL) << "Test02_006_007 failed";

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, 0, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_mutex_isr_test_abnormal4]

// [mutexperiod_workerfunction_timemeasurement1]
int64_t mutexperiod_workerfunction_timemeasurement1(void * arg)
{
    mutexperiod_worker_arg_timemeasurement1 *testStruct = (mutexperiod_worker_arg_timemeasurement1 *)arg;
    osal_milli_sec_t time;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_time_t current_time_1st, current_time_2nd;

// Test02_003_011
// @brief: normal case: timeout is 10ms
    time = 10;
    for(int i = 0; i <= 100; i++)
    {
        EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_1st), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(*(testStruct->mutex_handle), time), OSAL_RETURN_TIME) << "Test02_003_011 failed";
        EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_2nd), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &(testStruct->diff_time_10ms[i])), OSAL_RETURN_OK);
    }
    return 0;
}
// [mutexperiod_workerfunction_timemeasurement1]

// [mutexperiod_workerfunction_timemeasurement2]
int64_t mutexperiod_workerfunction_timemeasurement2(void * arg)
{
    mutexperiod_worker_arg_timemeasurement2 *testStruct = (mutexperiod_worker_arg_timemeasurement2 *)arg;
    osal_milli_sec_t time;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_time_t current_time_1st, current_time_2nd;

// Test02_003_012
// @brief: boundary normal case: timeout is 0ms
    time = 0;
    for(int i = 0; i <= 100; i++)
    {
        EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_1st), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(*(testStruct->mutex_handle), time), OSAL_RETURN_TIME) << "Test02_003_012 failed";
        EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_2nd), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &(testStruct->diff_time_0ms_TIME[i])), OSAL_RETURN_OK);
    }

    for(int i = 0; i <= 100; i++)
    {
        EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_1st), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(*(testStruct->mutex_handle1), time), OSAL_RETURN_OK) << "Test02_003_012 failed";
        EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_2nd), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &(testStruct->diff_time_0ms_OK[i])), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(*(testStruct->mutex_handle1)), OSAL_RETURN_OK) << "Test02_003_012 failed";
    }
    return 0;
}
// [mutexperiod_workerfunction_timemeasurement2]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexCreate_TC_02_001_001 ThreadSyncMutexApiTest.ThsyncMutexCreate_TC_02_001_001
 * @subsection ThsyncMutexCreate_TC_02_001_001_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexCreate_TC_02_001_001_tci Test Case ID
 * ThsyncMutexCreate_S1_N_TC_02_001_001
 * @subsection ThsyncMutexCreate_TC_02_001_001_rr Related Requirement(s)
 * \#199150
 * @subsection ThsyncMutexCreate_TC_02_001_001_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexCreate_TC_02_001_001_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to create a new mutex with normal condition:
 * - Test02_001_001: all input/output param is valid
 * - post-process: destroy created mutex
 * @subsection ThsyncMutexCreate_TC_02_001_001_tpts Test Procedure/Test Steps
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1.
 *      - Address of "mtx_handle"
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexCreate_TC_02_001_001_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncMutexCreate_TC_02_001_001_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexCreate_TC_02_001_001
 */
// [ThsyncMutexCreate_TC_02_001_001]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexCreate_TC_02_001_001)
{
    osal_mutex_id_t mutex_id;

// Test02_001_001
// @brief: normal case: all input/output param is valid
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "Test02_001_001 failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// post-process: destroy created mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexCreate_TC_02_001_001]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexCreate_TC_02_001_002 ThreadSyncMutexApiTest.ThsyncMutexCreate_TC_02_001_002
 * @subsection ThsyncMutexCreate_TC_02_001_002_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexCreate_TC_02_001_002_tci Test Case ID
 * ThsyncMutexCreate_S0_A_TC_02_001_002
 * @subsection ThsyncMutexCreate_TC_02_001_002_rr Related Requirement(s)
 * \#199148, \#199150
 * @subsection ThsyncMutexCreate_TC_02_001_002_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexCreate_TC_02_001_002_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to create a new mutex with abnormal condition:
 * - Test02_001_002: mutex handle is NULL
 * @subsection ThsyncMutexCreate_TC_02_001_002_tpts Test Procedure/Test Steps
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1.
 *      - Mutex handle is NULL
 * @subsection ThsyncMutexCreate_TC_02_001_002_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_PAR.
 * @subsection ThsyncMutexCreate_TC_02_001_002_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexCreate_TC_02_001_002
 */
// [ThsyncMutexCreate_TC_02_001_002]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexCreate_TC_02_001_002)
{
    osal_mutex_id_t mutex_id = TEST_MUTEX_ID1;
    osal_mutex_handle_t*  p_mtx_handle = NULL;

// Test02_001_002
// @brief: abnormal case: mutex handle is NULL
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, p_mtx_handle), OSAL_RETURN_PAR) << "Test02_001_002 failed";
}
// [ThsyncMutexCreate_TC_02_001_002]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexCreate_TC_02_001_003 ThreadSyncMutexApiTest.ThsyncMutexCreate_TC_02_001_003
 * @subsection ThsyncMutexCreate_TC_02_001_003_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexCreate_TC_02_001_003_tci Test Case ID
 * ThsyncMutexCreate_S2_A_TC_02_001_003
 * @subsection ThsyncMutexCreate_TC_02_001_003_rr Related Requirement(s)
 * \#199150
 * @subsection ThsyncMutexCreate_TC_02_001_003_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexCreate_TC_02_001_003_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to create a new mutex with abnormal condition:
 * - pre-process: create a mutex
 * - Test02_001_003: create 2 mutexes with the same mutex ID
 * - post-process: destroy created mutex
 * @subsection ThsyncMutexCreate_TC_02_001_003_tpts Test Procedure/Test Steps
 * -# Create first mutex by using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1.
 *      - Address of "mtx_handle"
 * -# Create second mutex which mutex id is the same as first created mutex by using R_OSAL_ThsyncMutexCreate.
 * -# Destroy first mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexCreate_TC_02_001_003_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK in first mutex.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_BUSY in second mutex.<br>
 * Value of "mtx_handle1" is NULL.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK in first mutex.
 * @subsection ThsyncMutexCreate_TC_02_001_003_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexCreate_TC_02_001_003
 */
// [ThsyncMutexCreate_TC_02_001_003]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexCreate_TC_02_001_003)
{
    osal_mutex_id_t mutex_id;

// pre-process: Create a mutex
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_001_003
// @brief: abnormal case: create 2 mutexes with the same mutex ID
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle1), OSAL_RETURN_BUSY) << "Test02_001_003 failed";
    ASSERT_EQ(mtx_handle1, NULL_PTR);

// post-process: Destroy created mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexCreate_TC_02_001_003]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section OsalApiStateTest_ThsyncMutexCreate_TC_02_001_004 OsalApiStateTest.ThsyncMutexCreate_TC_02_001_004
 * @subsection ThsyncMutexCreate_TC_02_001_004_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection ThsyncMutexCreate_TC_02_001_004_tci Test Case ID
 * ThsyncMutexCreate_S2_A_TC_02_001_004
 * @subsection ThsyncMutexCreate_TC_02_001_004_rr Related Requirement(s)
 * \#199150
 * @subsection ThsyncMutexCreate_TC_02_001_004_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection ThsyncMutexCreate_TC_02_001_004_tcs Test Case Summary
 * This test shall verify the state transition of Thread Synchronization Mutex of OSAL API
 * - Test02_001_004: State error of R_OSAL_ThsyncMutexCreate.
 * @subsection ThsyncMutexCreate_TC_02_001_004_tpts Test Procedure/Test Steps
 * -# Initialize the mtx_handle with MUTEX_HANDLE_INITIALIZED_VALUE by using memset.<br>
 * MUTEX_HANDLE_INITIALIZED_VALUE must be different from OSAL_MUTEX_HANDLE_INVALID
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - mtx_handle is MUTEX_HANDLE_INITIALIZED_VALUE
 * @subsection ThsyncMutexCreate_TC_02_001_004_ereb Expected Result/Behavior
 * -# memset a value for mtx_handle successfully
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_STATE.<br>
 * Value of "mtx_handle" is OSAL_MUTEX_HANDLE_INVALID.
 * @subsection ThsyncMutexCreate_TC_02_001_004_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexCreate_TC_02_001_004
 */
// [ThsyncMutexCreate_TC_02_001_004]
TEST_F(OsalApiStateTest, ThsyncMutexCreate_TC_02_001_004)
{
    osal_mutex_id_t mutex_id = TEST_MUTEX_ID1;
    osal_mutex_handle_t mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
    memset(&mtx_handle, MUTEX_HANDLE_INITIALIZED_VALUE, sizeof(mtx_handle));

// Test02_001_004
// @brief: abnormal case: state transition error
    EXPECT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_STATE) << "Test02_001_004 failed";
    EXPECT_EQ(mtx_handle, OSAL_MUTEX_HANDLE_INVALID) << "Test02_001_004 failed";
}
// [ThsyncMutexCreate_TC_02_001_004]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexDestroy_TC_02_002_001 ThreadSyncMutexApiTest.ThsyncMutexDestroy_TC_02_002_001
 * @subsection ThsyncMutexDestroy_TC_02_002_001_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexDestroy_TC_02_002_001_tci Test Case ID
 * ThsyncMutexDestroy_S1_N_TC_02_002_001
 * @subsection ThsyncMutexDestroy_TC_02_002_001_rr Related Requirement(s)
 * \#199157
 * @subsection ThsyncMutexDestroy_TC_02_002_001_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexDestroy_TC_02_002_001_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to destroy a mutex with normal condition:
 * - pre-process: Create a mutex
 * - Test02_002_001: all input/output param is valid
 * @subsection ThsyncMutexDestroy_TC_02_002_001_tpts Test Procedure/Test Steps
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexDestroy_TC_02_002_001_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncMutexDestroy_TC_02_002_001_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexDestroy_TC_02_002_001
 */
// [ThsyncMutexDestroy_TC_02_002_001]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexDestroy_TC_02_002_001)
{
    osal_mutex_id_t mutex_id;
    osal_milli_sec_t time;

// pre-process: Create a mutex
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_002_001
// @brief: normal case: all input/output param is valid
    ASSERT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "Test02_002_001 failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexDestroy_TC_02_002_001]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexDestroy_TC_02_002_002 ThreadSyncMutexApiTest.ThsyncMutexDestroy_TC_02_002_002
 * @subsection ThsyncMutexDestroy_TC_02_002_002_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexDestroy_TC_02_002_002_tci Test Case ID
 * ThsyncMutexDestroy_S0_A_TC_02_002_002
 * @subsection ThsyncMutexDestroy_TC_02_002_002_rr Related Requirement(s)
 * \#199148, \#199151, \#199157
 * @subsection ThsyncMutexDestroy_TC_02_002_002_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexDestroy_TC_02_002_002_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to destroy a mutex with abnormal condition:
 * - Test02_002_002: mutex handle is NULL
 * @subsection ThsyncMutexDestroy_TC_02_002_002_tpts Test Procedure/Test Steps
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy with the argument as below:
 *  - mutex handle is NUL
 * @subsection ThsyncMutexDestroy_TC_02_002_002_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_HANDLE.
 * @subsection ThsyncMutexDestroy_TC_02_002_002_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexDestroy_TC_02_002_002
 */
// [ThsyncMutexDestroy_TC_02_002_002]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexDestroy_TC_02_002_002)
{
// Test02_002_002
// @brief: abnormal case: mutex handle is NULL
    ASSERT_EQ(R_OSAL_ThsyncMutexDestroy(OSAL_MUTEX_HANDLE_INVALID), OSAL_RETURN_HANDLE) << "Test02_002_002 failed";
}
// [ThsyncMutexDestroy_TC_02_002_002]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexDestroy_TC_02_002_003 ThreadSyncMutexApiTest.ThsyncMutexDestroy_TC_02_002_003
 * @subsection ThsyncMutexDestroy_TC_02_002_003_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexDestroy_TC_02_002_003_tci Test Case ID
 * ThsyncMutexDestroy_S2_A_TC_02_002_003
 * @subsection ThsyncMutexDestroy_TC_02_002_003_rr Related Requirement(s)
 * \#199157
 * @subsection ThsyncMutexDestroy_TC_02_002_003_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexDestroy_TC_02_002_003_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to destroy a mutex with abnormal condition:
 * - pre-process: create a mutex
 * - Test02_002_003: destroy a mutex which has already destroyed.
 * @subsection ThsyncMutexDestroy_TC_02_002_003_tpts Test Procedure/Test Steps
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * -# Destroy the mutex which handle was destroyed above by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexDestroy_TC_02_002_003_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_HANDLE.
 * @subsection ThsyncMutexDestroy_TC_02_002_003_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexDestroy_TC_02_002_003
 */
// [ThsyncMutexDestroy_TC_02_002_003]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexDestroy_TC_02_002_003)
{
    osal_mutex_id_t mutex_id;

// pre-process: create a mutex
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_002_003
// @brief: abnormal case: destroy a mutex which has already destroyed.
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK);
    ASSERT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_HANDLE) << "Test02_002_003 failed";
}
// [ThsyncMutexDestroy_TC_02_002_003]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexDestroy_TC_02_002_004 ThreadSyncMutexApiTest.ThsyncMutexDestroy_TC_02_002_004
 * @subsection ThsyncMutexDestroy_TC_02_002_004_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexDestroy_TC_02_002_004_tci Test Case ID
 * ThsyncMutexDestroy_S2_A_TC_02_002_004
 * @subsection ThsyncMutexDestroy_TC_02_002_004_rr Related Requirement(s)
 * \#199157
 * @subsection ThsyncMutexDestroy_TC_02_002_004_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexDestroy_TC_02_002_004_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to destroy a mutex with abnormal condition:
 * - pre-process: create a mutex
 * - Test02_002_004: destroy a mutex which is under referencing (locked by R_OSAL_ThsyncMutexLockForTimePeriod).
 * - post-process: destroy created mutex
 * @subsection ThsyncMutexDestroy_TC_02_002_004_tpts Test Procedure/Test Steps
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Lock a mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * -# Unlock the mutex by using R_OSAL_ThsyncMutexUnlock.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexDestroy_TC_02_002_004_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_BUSY.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncMutexDestroy_TC_02_002_004_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexDestroy_TC_02_002_004
 */
// [ThsyncMutexDestroy_TC_02_002_004]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexDestroy_TC_02_002_004)
{
    osal_mutex_id_t mutex_id;
    osal_milli_sec_t time = 10;

// pre-process: create a mutex
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_002_004
// @brief: abnormal case: destroy a mutex which is under referencing (locked by R_OSAL_ThsyncMutexLockForTimePeriod)
    ASSERT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(mtx_handle, time), OSAL_RETURN_OK);
    ASSERT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_BUSY) << "Test02_002_004 failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK);

// post-process: destroy created mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexDestroy_TC_02_002_004]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section OsalApiStateTest_ThsyncMutexDestroy_TC_02_002_005 OsalApiStateTest.ThsyncMutexDestroy_TC_02_002_005
 * @subsection ThsyncMutexDestroy_TC_02_002_005_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection ThsyncMutexDestroy_TC_02_002_005_tci Test Case ID
 * ThsyncMutexDestroy_S2_A_TC_02_002_005
 * @subsection ThsyncMutexDestroy_TC_02_002_005_rr Related Requirement(s)
 * \#199157
 * @subsection ThsyncMutexDestroy_TC_02_002_005_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection ThsyncMutexDestroy_TC_02_002_005_tcs Test Case Summary
 * This test shall verify the state transition of Thread Synchronization Mutex of OSAL API
 * - Test02_002_005: State error of R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexDestroy_TC_02_002_005_tpts Test Procedure/Test Steps
 * -# Destroy mutex by calling R_OSAL_ThsyncMutexDestroy
 * @subsection ThsyncMutexDestroy_TC_02_002_005_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_STATE.
 * @subsection ThsyncMutexDestroy_TC_02_002_005_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexDestroy_TC_02_002_005
 */
// [ThsyncMutexDestroy_TC_02_002_005]
TEST_F(OsalApiStateTest, ThsyncMutexDestroy_TC_02_002_005)
{
    osal_mutex_handle_t mtx_handle = OSAL_MUTEX_HANDLE_INVALID;

// Test02_002_005
// @brief: abnormal case: state transition error
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_STATE) << "Test02_002_005 failed";
}
// [ThsyncMutexDestroy_TC_02_002_005]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexDestroy_TC_02_002_006 ThreadSyncMutexApiTest.ThsyncMutexDestroy_TC_02_002_006
 * @subsection ThsyncMutexDestroy_TC_02_002_006_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexDestroy_TC_02_002_006_tci Test Case ID
 * ThsyncMutexDestroy_S1_N_TC_02_002_006
 * @subsection ThsyncMutexDestroy_TC_02_002_006_rr Related Requirement(s)
 * \#199157
 * @subsection ThsyncMutexDestroy_TC_02_002_006_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexDestroy_TC_02_002_006_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to destroy a mutex with normal condition:
 * - pre-process: Create a mutex
 * - Test02_002_006: confirm R_OSAL_ThsyncMutexDestroy with the sequence call as below:
 *      -# MutexLock (success)
 *      -# MutexUnlock (success)
 *      -# MutexDestroy
 * @subsection ThsyncMutexDestroy_TC_02_002_006_tpts Test Procedure/Test Steps
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 * -# Unlock the mutex with R_OSAL_ThsyncMutexUnlock.
 * -# Destroy the mutex using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexDestroy_TC_02_002_006_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncMutexDestroy_TC_02_002_006_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexDestroy_TC_02_002_006
 */
// [ThsyncMutexDestroy_TC_02_002_006]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexDestroy_TC_02_002_006)
{
    osal_mutex_id_t mutex_id;
    osal_milli_sec_t time;

// pre-process: Create a mutex
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_002_006
// @brief: normal case: destroy a mutex by lock -> unlock -> destroy
    time = 1000;
    EXPECT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(mtx_handle, time), OSAL_RETURN_OK) << "Test02_002_006 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK) << "Test02_002_006 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "Test02_002_006 failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexDestroy_TC_02_002_006]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexDestroy_TC_02_002_007 ThreadSyncMutexApiTest.ThsyncMutexDestroy_TC_02_002_007
 * @subsection ThsyncMutexDestroy_TC_02_002_007_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexDestroy_TC_02_002_007_tci Test Case ID
 * ThsyncMutexDestroy_S1_N_TC_02_002_007
 * @subsection ThsyncMutexDestroy_TC_02_002_007_rr Related Requirement(s)
 * \#199157
 * @subsection ThsyncMutexDestroy_TC_02_002_007_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexDestroy_TC_02_002_007_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to destroy a mutex with normal condition:
 * - pre-process: Create a mutex
 * - Test02_002_007: confirm R_OSAL_ThsyncMutexDestroy with the sequence call as below:
 *      -# MutexLock (timeout)
 *      -# MutexDestroy
 * @subsection ThsyncMutexDestroy_TC_02_002_007_tpts Test Procedure/Test Steps
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 * -# Create a thread using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is "mutextimedlock_workerfunction" function with functionality:
 *              - Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 *          -# userarg includes mutex handle
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1
 *      - Address of thread handle.
 * -# Join "Simple testing thread" by using R_OSAL_ThreadJoin.
 * -# Unlock the mutex with R_OSAL_ThsyncMutexUnlock.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexDestroy_TC_02_002_007_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK.<br>
 * Value of "thread_handle" is not NULL.<br>
 * In mutextimedlock_workerfunction function: <br>
 *  - R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_TIME.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK.<br>
 * Value of "result" is equal to 0.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncMutexDestroy_TC_02_002_007_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexDestroy_TC_02_002_007
 * @snippet test_mutex.cpp mutextimedlock_workerfunction
 */
// [ThsyncMutexDestroy_TC_02_002_007]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexDestroy_TC_02_002_007)
{
    osal_mutex_id_t mutex_id;
    osal_milli_sec_t time;
    osal_thread_id_t uid;
    st_osal_thread_config_t config;
    int64_t result;

// pre-process: Create a mutex
    mutex_id = TEST_MUTEX_ID1;
    EXPECT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_002_007
// @brief: normal case: destroy a mutex by lock -> timeout -> destroy
    time = 1000;
    EXPECT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(mtx_handle, time), OSAL_RETURN_OK) << "Test02_002_007 failed";
    mutexlock_worker_arg user_arg(&mtx_handle);
    uid              = TEST_THREAD_ID1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = (char *)"Simple testing thread";
    config.func      = mutextimedlock_workerfunction;
    config.userarg   = &user_arg;
    config.stack_size = THREAD_STACK_SIZE;

    EXPECT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK) << "Test02_002_007 failed";
    EXPECT_NE(thread_handle, NULL_PTR) << "Test02_002_007 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK) << "Test02_002_007 failed";
    EXPECT_EQ(result, 0) << "Test02_002_007 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK) << "Test02_002_007 failed";
    thread_handle = OSAL_THREAD_HANDLE_INVALID;
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "Test02_002_007 failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexDestroy_TC_02_002_007]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexLockForTimePeriod_TC_02_003_001 ThreadSyncMutexApiTest.ThsyncMutexLockForTimePeriod_TC_02_003_001
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_001_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_001_tci Test Case ID
 * ThsyncMutexLockForTimePeriod_S1_N_TC_02_003_001
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_001_rr Related Requirement(s)
 * \#199153
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_001_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_001_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to lock a mutex with timeout for specified time in normal condition.
 * - pre-process: create a mutex
 * - Test02_003_001: all input/output param is valid
 * - post-process: destroy created mutex
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_001_tpts Test Procedure/Test Steps
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod with all input/output param is valid.
 * -# Unlock the mutex with R_OSAL_ThsyncMutexUnlock.
 * -# Destroy the mutex using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_001_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_001_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexLockForTimePeriod_TC_02_003_001
 */
// [ThsyncMutexLockForTimePeriod_TC_02_003_001]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexLockForTimePeriod_TC_02_003_001)
{
    osal_mutex_id_t mutex_id;
    osal_milli_sec_t time = 1000;

// pre-process: create a mutex
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_003_001
// @brief: normal case: all input/output param is valid
    ASSERT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(mtx_handle, time), OSAL_RETURN_OK) << "Test02_003_001 failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK);

// post-process: destroy created mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexLockForTimePeriod_TC_02_003_001]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexLockForTimePeriod_TC_02_003_002 ThreadSyncMutexApiTest.ThsyncMutexLockForTimePeriod_TC_02_003_002
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_002_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_002_tci Test Case ID
 * ThsyncMutexLockForTimePeriod_S1_BN_TC_02_003_002
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_002_rr Related Requirement(s)
 * \#199153
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_002_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_002_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to lock a mutex with timeout for specified time in normal condition.
 * - pre-process: create a mutex
 * - Test02_003_002: timeout is 0ms
 * - post-process: destroy created mutex
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_002_tpts Test Procedure/Test Steps
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod with the argument as below:
 *      - "time" is equal to 0ms
 * -# Unlock the mutex with R_OSAL_ThsyncMutexUnlock.
 * -# Destroy the mutex using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_002_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_002_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexLockForTimePeriod_TC_02_003_002
 */
// [ThsyncMutexLockForTimePeriod_TC_02_003_002]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexLockForTimePeriod_TC_02_003_002)
{
    osal_mutex_id_t mutex_id;
    osal_milli_sec_t time;

// pre-process: create a mutex
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_003_002
// @brief: boundary normal case: timeout is 0ms
    time = 0;
    ASSERT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(mtx_handle, time), OSAL_RETURN_OK) << "Test02_003_002 failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK);

// post-process: destroy created mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexLockForTimePeriod_TC_02_003_002]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexLockForTimePeriod_TC_02_003_003 ThreadSyncMutexApiTest.ThsyncMutexLockForTimePeriod_TC_02_003_003
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_003_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_003_tci Test Case ID
 * ThsyncMutexLockForTimePeriod_S0_A_TC_02_003_003
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_003_rr Related Requirement(s)
 * \#199148, \#199151, \#199153
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_003_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_003_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to lock a mutex with timeout for specified time in abnormal condition.
 * - Test02_003_003: mutex handle is NULL.
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_003_tpts Test Procedure/Test Steps
 * -# Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod with the argument as below:
 *      - mutex handle is NULL
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_003_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_HANDLE.
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_003_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexLockForTimePeriod_TC_02_003_003
 */
// [ThsyncMutexLockForTimePeriod_TC_02_003_003]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexLockForTimePeriod_TC_02_003_003)
{
    osal_mutex_id_t mutex_id;
    osal_milli_sec_t time;

// Test02_003_003
// @brief: abnormal case: mutex handle is NULL
    time = 10;
    ASSERT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_MUTEX_HANDLE_INVALID, time), OSAL_RETURN_HANDLE) << "Test02_003_003 failed";
}
// [ThsyncMutexLockForTimePeriod_TC_02_003_003]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexLockForTimePeriod_TC_02_003_004 ThreadSyncMutexApiTest.ThsyncMutexLockForTimePeriod_TC_02_003_004
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_004_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_004_tci Test Case ID
 * ThsyncMutexLockForTimePeriod_S2_A_TC_02_003_004
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_004_rr Related Requirement(s)
 * \#199153
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_004_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_004_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to lock a mutex with timeout for specified time in abnormal condition.
 * - pre-process: create a mutex
 * - Test02_003_004: lock a mutex which has already been owned by another thread.
 * - post-process: destroy created mutex
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_004_tpts Test Procedure/Test Steps
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 * -# Create a thread using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is mutextimedlock_workerfunction function with functionality:
 *              - Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 *          -# userarg includes mutex handle
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Address of thread handle.
 * -# Join "Simple testing thread" by using R_OSAL_ThreadJoin.
 * -# Unlock the mutex with R_OSAL_ThsyncMutexUnlock.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_004_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK.<br>
 * Value of "thread_handle" is not NULL.<br>
 * In mutextimedlock_workerfunction function: <br>
 *  - R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK.<br>
 * Value of "result" is equal to 0.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_004_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexLockForTimePeriod_TC_02_003_004
 * @snippet test_mutex.cpp mutextimedlock_workerfunction
 */
// [ThsyncMutexLockForTimePeriod_TC_02_003_004]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexLockForTimePeriod_TC_02_003_004)
{
    osal_mutex_id_t mutex_id;
    osal_milli_sec_t time;
    osal_thread_id_t uid;
    st_osal_thread_config_t config;
    e_osal_return_t osal_ret;
    int64_t result;

// pre-process: create a mutex
    mutex_id = TEST_MUTEX_ID1;
    time = 1000;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_003_004
// @brief: abnormal case: lock a mutex which has already been owned by another thread.
    time = 10;
    ASSERT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(mtx_handle, time), OSAL_RETURN_OK) << "Test02_003_004 failed";
    mutexlock_worker_arg user_arg(&mtx_handle);
    uid              = TEST_THREAD_ID1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = (char *)"Simple testing thread";
    config.func      = mutextimedlock_workerfunction;
    config.userarg   = &user_arg;
    config.stack_size = THREAD_STACK_SIZE;

    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK) << "Test02_003_004 failed";
    ASSERT_NE(thread_handle, NULL_PTR);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK) << "Test02_003_004 failed";
    EXPECT_EQ(result, 0) << "Test02_003_004 failed";;
    ASSERT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK) << "Test02_003_004 failed";
    thread_handle = OSAL_THREAD_HANDLE_INVALID;

// post-process: destroy created mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexLockForTimePeriod_TC_02_003_004]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexLockForTimePeriod_TC_02_003_005 ThreadSyncMutexApiTest.ThsyncMutexLockForTimePeriod_TC_02_003_005
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_005_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_005_tci Test Case ID
 * ThsyncMutexLockForTimePeriod_S2_A_TC_02_003_005
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_005_rr Related Requirement(s)
 * \#199153
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_005_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_005_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to lock a mutex with timeout for specified time in abnormal condition.
 * - pre-process: create a mutex
 * - Test02_003_005: lock a mutex in a thread which has already owned this mutex.
 * - post-process: destroy created mutex
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_005_tpts Test Procedure/Test Steps
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 * -# Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod again.
 * -# Unlock the mutex with R_OSAL_ThsyncMutexUnlock.
 * -# Destroy the mutex using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_005_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 * -# Return value of R_OSAL_ThsyncMutexLockForTimePeriod is equal to OSAL_RETURN_FAIL or OSAL_RETURN_TIME.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_005_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexLockForTimePeriod_TC_02_003_005
 */
// [ThsyncMutexLockForTimePeriod_TC_02_003_005]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexLockForTimePeriod_TC_02_003_005)
{
    osal_mutex_id_t mutex_id;
    osal_milli_sec_t time;
    e_osal_return_t osal_ret;

// pre-process: create a mutex
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_003_005
// @brief: abnormal case: lock a mutex in a thread which has already owned this mutex
    time = 10;
    ASSERT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(mtx_handle, time), OSAL_RETURN_OK);
    osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(mtx_handle, time);
    ASSERT_TRUE((osal_ret == OSAL_RETURN_FAIL) || (osal_ret == OSAL_RETURN_TIME)) << "Test02_003_005 failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK);

// post-process: destroy created mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexLockForTimePeriod_TC_02_003_005]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexLockForTimePeriod_TC_02_003_006 ThreadSyncMutexApiTest.ThsyncMutexLockForTimePeriod_TC_02_003_006
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_006_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_006_tci Test Case ID
 * ThsyncMutexLockForTimePeriod_S2_A_TC_02_003_006
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_006_rr Related Requirement(s)
 * \#199153
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_006_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_006_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to lock a mutex with timeout for specified time in abnormal condition.
 * - pre-process: create a mutex
 * - Test02_003_006: mutex handle has already been destroyed.
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_006_tpts Test Procedure/Test Steps
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Destroy the mutex using R_OSAL_ThsyncMutexDestroy.
 * -# Lock mutex which handle was destroyed with R_OSAL_ThsyncMutexLockForTimePeriod.
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_006_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_HANDLE.
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_006_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexLockForTimePeriod_TC_02_003_006
 */
// [ThsyncMutexLockForTimePeriod_TC_02_003_006]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexLockForTimePeriod_TC_02_003_006)
{
    osal_mutex_id_t mutex_id;
    osal_milli_sec_t time;

// pre-process: create a mutex
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_003_006
// @brief: abnormal case: mutex handle has already been destroyed
    time = 10;
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK);
    ASSERT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(mtx_handle, time), OSAL_RETURN_HANDLE) << "Test02_003_006 failed";
}
// [ThsyncMutexLockForTimePeriod_TC_02_003_006]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexLockForTimePeriod_TC_02_003_007 ThreadSyncMutexApiTest.ThsyncMutexLockForTimePeriod_TC_02_003_007
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_007_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_007_tci Test Case ID
 * ThsyncMutexLockForTimePeriod_S0_BA_TC_02_003_007
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_007_rr Related Requirement(s)
 * \#199153
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_007_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_007_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to lock a mutex with timeout for specified time in abnormal condition.
 * - pre-process: create a mutex
 * - Test02_003_007: timeout is negative number.
 * - post-process: destroy created mutex
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_007_tpts Test Procedure/Test Steps
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod with the argument as below:
 *      - "time" is equal to -10msec
 * -# Destroy the mutex using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_007_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_PAR.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_007_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexLockForTimePeriod_TC_02_003_007
 */
// [ThsyncMutexLockForTimePeriod_TC_02_003_007]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexLockForTimePeriod_TC_02_003_007)
{
    osal_mutex_id_t mutex_id;
    osal_milli_sec_t time;

// pre-process: create a mutex
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_003_007
// @brief: abnormal case: timeout is negative number
    time = -10;
    ASSERT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(mtx_handle, time), OSAL_RETURN_PAR) << "Test02_003_007 failed";

// post-process: destroy created mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexLockForTimePeriod_TC_02_003_007]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section OsalApiStateTest_ThsyncMutexLockForTimePeriod_TC_02_003_008 OsalApiStateTest.ThsyncMutexLockForTimePeriod_TC_02_003_008
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_008_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_008_tci Test Case ID
 * ThsyncMutexLockForTimePeriod_S2_A_TC_02_003_008
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_008_rr Related Requirement(s)
 * \#199153
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_008_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_008_tcs Test Case Summary
 * This test shall verify the state transition of Thread Synchronization Mutex of OSAL API
 * - Test02_003_008: State error of R_OSAL_ThsyncMutexLockForTimePeriod.
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_008_tpts Test Procedure/Test Steps
 * -# Lock mutex by calling R_OSAL_ThsyncMutexLockForTimePeriod
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_008_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_STATE.
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_008_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexLockForTimePeriod_TC_02_003_008
 */
// [ThsyncMutexLockForTimePeriod_TC_02_003_008]
TEST_F(OsalApiStateTest, ThsyncMutexLockForTimePeriod_TC_02_003_008)
{
    osal_mutex_handle_t mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
    osal_milli_sec_t time = 10;

// Test02_003_008
// @brief: abnormal case: state transition error
    EXPECT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(mtx_handle, time), OSAL_RETURN_STATE) << "Test02_003_008 failed";
}
// [ThsyncMutexLockForTimePeriod_TC_02_003_008]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section OSALINTTESTS_ThsyncMutexLockForTimePeriod_TC_02_003_009 OSALINTTESTS.ThsyncMutexLockForTimePeriod_TC_02_003_009
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_009_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_009_tci Test Case ID
 * ThsyncMutexLockForTimePeriod_S3_N_TC_02_003_009
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_009_rr Related Requirement(s)
 * \#199153
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_009_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_009_tcs Test Case Summary
 * @subsubsection ThsyncMutexLockForTimePeriod_TC_02_003_009_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Create a mutex.
 * - Setup required memory for interrupt generation
 * - Register callack function and enable interrupt
 * - Wait for interrupt callback function finish
 * @subsubsection ThsyncMutexLockForTimePeriod_TC_02_003_009_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * - Destroy created mutex
 * @subsubsection ThsyncMutexLockForTimePeriod_TC_02_003_009_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality with normal condition of the ThreadSync Mutex APIs which are called in the ISR.
 * - Disable and clear interrupt status
 * - Test02_003_009: call R_OSAL_ThsyncMutexLockForTimePeriod in ISR
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_009_tpts Test Procedure/Test Steps
 * @subsubsection ThsyncMutexLockForTimePeriod_TC_02_003_009_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Create new mutex handle by using R_OSAL_ThsyncMutexCreate.<br>
 * Pass this mutex handle to interrupt callback function as the argument through R_OSAL_InterruptRegisterIsr.
 * -# Allocate required memory for generate interrupt from hardware and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function by using R_OSAL_InterruptRegisterIsr
 * -# Enable interrupt by using R_OSAL_InterruptEnableIsr
 * -# Setup hardware specific register to generate the interrupt by calling to setup_hw_register
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection ThsyncMutexLockForTimePeriod_TC_02_003_009_post_tpts Test Procedure/Test Steps of Post-process
 * -# Unregister interrupt by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * -# Destroy created mutex by calling R_OSAL_ThsyncMutexDestroy
 * @subsubsection ThsyncMutexLockForTimePeriod_TC_02_003_009_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Lock mutex by using R_OSAL_ThsyncMutexLockForTimePeriod
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock
 * -# Send Message interrupt of context finished by using R_OSAL_MqSendForTimePeriod
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_009_ereb Expected Result/Behavior
 * @subsubsection ThsyncMutexLockForTimePeriod_TC_02_003_009_pre_ereb Expected Result/Behavior of Pre-process
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mutex_handle" is not NULL_PTR
 * -# allocate_setup_data_memory run successfully
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * The value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK
 * -# setup_hw_register run successfully
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection ThsyncMutexLockForTimePeriod_TC_02_003_009_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK
 * @subsubsection ThsyncMutexLockForTimePeriod_TC_02_003_009_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_009_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexLockForTimePeriod_TC_02_003_009
 * @snippet test_mutex.cpp osal_mutex_isr_test_normal_TC_02_003_009
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [ThsyncMutexLockForTimePeriod_TC_02_003_009]
TEST_F(OSALINTTESTS, ThsyncMutexLockForTimePeriod_TC_02_003_009)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    osal_mutex_handle_t mutex_handle = OSAL_MUTEX_HANDLE_INVALID;
    osal_mutex_id_t mutex_id = TEST_MUTEX_ID1;
    osal_milli_sec_t time_period;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};

/* pre-process:
 * create a mutex
 * Setup required memory for interrupt generation.
 * Register callack function and enable interrupt 
 * Wait for interrupt callback function finish */

    EXPECT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(mutex_handle, NULL_PTR);
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    arg.additional_arg = (void*)&mutex_handle;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_mutex_isr_test_normal_TC_02_003_009, (void *)&arg), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK) << "pre-process failed";
    setup_hw_register(dev_arr, hw_addr);
    time_period = 2000;
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory
 * Destroy create mutex */
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_mutex_isr_test_normal_TC_02_003_009), OSAL_RETURN_OK) << "post-process failed";
    deallocate_data_memory(hndl_mmngr, buffer_obj);
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mutex_handle), OSAL_RETURN_OK) << "post-process failed";
    mutex_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexLockForTimePeriod_TC_02_003_009]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section OSALINTTESTS_ThsyncMutexLockForTimePeriod_TC_02_003_010 OSALINTTESTS.ThsyncMutexLockForTimePeriod_TC_02_003_010
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_010_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_010_tci Test Case ID
 * ThsyncMutexLockForTimePeriod_S2_A_TC_02_003_010
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_010_rr Related Requirement(s)
 * \#199153
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_010_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_010_tcs Test Case Summary
 * @subsubsection ThsyncMutexLockForTimePeriod_TC_02_003_010_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Create a mutex.
 * - Setup required memory for interrupt generation
 * - Register callack function and enable interrupt
 * - Wait for interrupt callback function finish
 * @subsubsection ThsyncMutexLockForTimePeriod_TC_02_003_010_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * - Destroy created mutex
 * @subsubsection ThsyncMutexLockForTimePeriod_TC_02_003_010_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality with abnormal condition of the ThreadSync Mutex APIs which are called in the ISR.
 * - Disable and clear interrupt status
 * - Test02_003_010: call R_OSAL_ThsyncMutexLockForTimePeriod in ISR with a locked mutex
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_010_tpts Test Procedure/Test Steps
 * @subsubsection ThsyncMutexLockForTimePeriod_TC_02_003_010_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Create new mutex handle by using R_OSAL_ThsyncMutexCreate.<br>
 * Pass this mutex handle to interrupt callback function as the argument through R_OSAL_InterruptRegisterIsr.
 * -# Allocate required memory for generate interrupt from hardware and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function by using R_OSAL_InterruptRegisterIsr
 * -# Enable interrupt by using R_OSAL_InterruptEnableIsr
 * -# Setup hardware specific register to generate the interrupt by calling to setup_hw_register
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection ThsyncMutexLockForTimePeriod_TC_02_003_010_post_tpts Test Procedure/Test Steps of Post-process
 * -# Unregister interrupt by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * -# Destroy created mutex by calling R_OSAL_ThsyncMutexDestroy
 * @subsubsection ThsyncMutexLockForTimePeriod_TC_02_003_010_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Lock mutex by using R_OSAL_ThsyncMutexLockForTimePeriod
 * -# Continule lock the same mutex by using R_OSAL_ThsyncMutexLockForTimePeriod
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock
 * -# Send Message interrupt of context finished by using R_OSAL_MqSendForTimePeriod
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_010_ereb Expected Result/Behavior
 * @subsubsection ThsyncMutexLockForTimePeriod_TC_02_003_010_pre_ereb Expected Result/Behavior of Pre-process
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mutex_handle" is not NULL_PTR
 * -# allocate_setup_data_memory run successfully
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * The value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK
 * -# setup_hw_register run successfully
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection ThsyncMutexLockForTimePeriod_TC_02_003_010_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK
 * @subsubsection ThsyncMutexLockForTimePeriod_TC_02_003_010_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK
 * -# R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_BUSY
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_010_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexLockForTimePeriod_TC_02_003_010
 * @snippet test_mutex.cpp osal_mutex_isr_test_abnormal1
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [ThsyncMutexLockForTimePeriod_TC_02_003_010]
TEST_F(OSALINTTESTS, ThsyncMutexLockForTimePeriod_TC_02_003_010)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    osal_mutex_handle_t mutex_handle = OSAL_MUTEX_HANDLE_INVALID;
    osal_mutex_id_t mutex_id = TEST_MUTEX_ID1;
    osal_milli_sec_t time_period;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};

/* pre-process:
 * create a mutex
 * Setup required memory for interrupt generation.
 * Register callack function and enable interrupt 
 * Wait for interrupt callback function finish */

    EXPECT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(mutex_handle, NULL_PTR);
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    arg.additional_arg = (void*)&mutex_handle;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_mutex_isr_test_abnormal1, (void *)&arg), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK) << "pre-process failed";
    setup_hw_register(dev_arr, hw_addr);
    time_period = 2000;
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory
 * Destroy create mutex */

    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_mutex_isr_test_abnormal1), OSAL_RETURN_OK) << "post-process failed";
    deallocate_data_memory(hndl_mmngr, buffer_obj);
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mutex_handle), OSAL_RETURN_OK) << "post-process failed";
    mutex_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexLockForTimePeriod_TC_02_003_010]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexLockForTimePeriod_TC_02_003_011 ThreadSyncMutexApiTest.ThsyncMutexLockForTimePeriod_TC_02_003_011
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_011_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_011_tci Test Case ID
 * ThsyncMutexLockForTimePeriod_S1_N_TC_02_003_011
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_011_rr Related Requirement(s)
 * \#199153
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_011_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_011_tcs Test Case Summary
 * This test shall measure the time execution of R_OSAL_ThsyncMutexLockForTimePeriod with normal condition:
 * - pre-process: create and lock the mutex.
 * - Test02_003_011: measure the time execution of R_OSAL_ThsyncMutexLockForTimePeriod when locking a mutex with timeout is 10ms
 * - post-process: unlock and destroy created mutex
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_011_tpts Test Procedure/Test Steps
 * -# Create a mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Lock the mutex by using R_OSAL_ThsyncMutexTryLock
 * -# Create new thread using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "testing thread"
 *          -# func is mutexperiod_workerfunction_timemeasurement1 function with functionality:
 *              - Use the "for" loop in which we repeat 101 times with 4 steps as below:
 *                  -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *                  -# Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod with the timeout is 10ms.
 *                  -# Get the current time stamp again by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *                  -# Get the execution time of R_OSAL_ThsyncMutexLockForTimePeriod by calculating the different time using R_OSAL_ClockTimeCalculateTimeDifference.<br>
 *                    Execution time is stored in an array "diff_time_10ms"
 *          -# userarg includes mutex handle, the address of "diff_time_10ms".
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Address of thread handle.
 * -# Join created thread by using R_OSAL_ThreadJoin
 * -# Find the median of "diff_time_10ms" array.<br>
 * Compare it with 10ms plus "delta_time" (TEST_MEDIAN_EXPECT)
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock
 * -# Destroy mutex using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_011_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_OK
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK.<br>
 * "thread_handle" is not NULL_PTR.
 *      - In worker function, the expected result of 4 steps in "for" loop are as below:
 *          -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 *          -# R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_TIME.
 *          -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 *          -# R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK
 * -# Getting the median of "diff_time_10ms" array successfully.<br>
 * Median value need to be not exceed 10ms plus with "delta_time"
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_011_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexLockForTimePeriod_TC_02_003_011
 * @snippet test_mutex.cpp mutexperiod_workerfunction_timemeasurement1
 * @snippet test_common.hpp findMedian
 */
// [ThsyncMutexLockForTimePeriod_TC_02_003_011]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexLockForTimePeriod_TC_02_003_011)
{
    osal_mutex_id_t mutex_id;
    osal_milli_sec_t time = 10;
    osal_nano_sec_t diff_time_10ms[101];
    osal_nano_sec_t delta_time = TEST_MEDIAN_EXPECT;
    osal_nano_sec_t median = 0;
    int elements = 0;
    osal_thread_id_t uid;
    st_osal_thread_config_t config;
    int64_t result;

// pre-process: create a mutex
    mutex_id = TEST_MUTEX_ID1;
    EXPECT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(mtx_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexTryLock(mtx_handle), OSAL_RETURN_OK) << "pre-process failed";

    mutexperiod_worker_arg_timemeasurement1 user_arg(&mtx_handle, diff_time_10ms);
    uid              = TEST_THREAD_ID1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = (char *)"testing thread";
    config.func      = mutexperiod_workerfunction_timemeasurement1;
    config.userarg   = &user_arg;
    config.stack_size = THREAD_STACK_SIZE;
    EXPECT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK);
    EXPECT_NE(thread_handle, NULL_PTR);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK);
    EXPECT_EQ(result, 0);

    elements = sizeof(diff_time_10ms) / sizeof(diff_time_10ms[0]);
    median = (osal_nano_sec_t)findMedian(diff_time_10ms, elements);
    delta_time = TEST_MEDIAN_EXPECT;
    EXPECT_LE(median, time * MSEC_TO_NSEC + delta_time) << "Test02_003_011 failed";

// post-process: destroy created mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK) << "post-process failed";;
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexLockForTimePeriod_TC_02_003_011]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexLockForTimePeriod_TC_02_003_012 ThreadSyncMutexApiTest.ThsyncMutexLockForTimePeriod_TC_02_003_012
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_012_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_012_tci Test Case ID
 * ThsyncMutexLockForTimePeriod_S1_N_TC_02_003_012
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_012_rr Related Requirement(s)
 * \#199153
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_012_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_012_tcs Test Case Summary
 * This test shall measure the time execution of R_OSAL_ThsyncMutexLockForTimePeriod with normal condition:
 * - pre-process: create 2 mutex and lock the first mutex.
 * - Test02_003_012: measure the time execution of R_OSAL_ThsyncMutexLockForTimePeriod when locking a mutex with timeout is 0ms
 * - post-process: unlock and destroy created mutexs
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_012_tpts Test Procedure/Test Steps
 * -# Create 1st mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Create 2nd mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID2
 *      - Address of "mtx_handle1"
 * -# Lock the mutex by using R_OSAL_ThsyncMutexTryLock
 * -# Create new thread using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "testing thread"
 *          -# func is mutexperiod_workerfunction_timemeasurement function with functionality:
 *              - Use 1st "for" loop in which we repeat 101 times with 4 steps as below:
 *                  -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *                  -# Lock 1st mutex with R_OSAL_ThsyncMutexLockForTimePeriod with the timeout is 0ms.
 *                  -# Get the current time stamp again by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *                  -# Get the execution time of R_OSAL_ThsyncMutexLockForTimePeriod by calculating the different time using R_OSAL_ClockTimeCalculateTimeDifference.<br>
 *                    Execution time is stored in an array "diff_time_0ms_TIME"
 *              - Continue to use 2nd "for" loop in which we repeat 101 times with 5 steps as below:
 *                  -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *                  -# Lock 2nd mutex with R_OSAL_ThsyncMutexLockForTimePeriod with the timeout is 0ms.
 *                  -# Get the current time stamp again by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *                  -# Get the execution time of R_OSAL_ThsyncMutexLockForTimePeriod by calculating the different time using R_OSAL_ClockTimeCalculateTimeDifference.<br>
 *                    Execution time is stored in an array "diff_time_0ms_OK"
 *                  -# Unlock 2nd mutex by using R_OSAL_ThsyncMutexUnlock
 *          -# userarg includes mutex handle, the address of "diff_time_0ms_TIME" and "diff_time_0ms_OK"
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Address of first thread handle.
 * -# Join created thread by using R_OSAL_ThreadJoin
 * -# Find the median of "diff_time_0ms_TIME" array.<br>
 * Compare it with "delta_time" (TEST_MEDIAN_EXPECT)
 * -# Find the median of "diff_time_0ms_OK" array.<br>
 * Compare it with "delta_time" (typically 100usec)
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock
 * -# Destroy 1st mutex using R_OSAL_ThsyncMutexDestroy.
 * -# Destroy 2nd mutex using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_012_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle1" is not NULL.
 * -# R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_OK
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with first thread.<br>
 * "thread_handle" is not NULL_PTR.
 *      - In worker function, the expected result of 4 steps in first "for" loop are as below:
 *          -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 *          -# R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_TIME.
 *          -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 *          -# R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK.
 *      - In the 2nd "for" loop, the expected result of 5 steps are:
 *          -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 *          -# R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 *          -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 *          -# R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK.
 *          -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK
 * -# Getting the median of "diff_time_0ms_TIME" array successfully.<br>
 * Median value need to be not exceed "delta_time"
 * -# Getting the median of "diff_time_0ms_OK" array successfully.<br>
 * Median value need to be not exceed "delta_time"
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncMutexLockForTimePeriod_TC_02_003_012_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexLockForTimePeriod_TC_02_003_012
 * @snippet test_mutex.cpp mutexperiod_workerfunction_timemeasurement2
 * @snippet test_common.hpp findMedian
 */
// [ThsyncMutexLockForTimePeriod_TC_02_003_012]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexLockForTimePeriod_TC_02_003_012)
{
    osal_mutex_id_t mutex_id;
    osal_nano_sec_t diff_time_0ms_TIME[101];
    osal_nano_sec_t diff_time_0ms_OK[101];
    osal_nano_sec_t delta_time = TEST_MEDIAN_EXPECT;
    osal_nano_sec_t median = 0;
    int elements = 0;
    osal_thread_id_t uid;
    st_osal_thread_config_t config;
    int64_t result;

// pre-process: create a mutex
    mutex_id = TEST_MUTEX_ID1;
    EXPECT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(mtx_handle, NULL_PTR) << "pre-process failed";
    mutex_id = TEST_MUTEX_ID2;
    EXPECT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle1), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(mtx_handle1, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexTryLock(mtx_handle), OSAL_RETURN_OK) << "pre-process failed";

    mutexperiod_worker_arg_timemeasurement2 user_arg(&mtx_handle, &mtx_handle1, diff_time_0ms_TIME, diff_time_0ms_OK);
    uid              = TEST_THREAD_ID1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = (char *)"testing thread";
    config.func      = mutexperiod_workerfunction_timemeasurement2;
    config.userarg   = &user_arg;
    config.stack_size = THREAD_STACK_SIZE;
    EXPECT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK);
    EXPECT_NE(thread_handle, NULL_PTR);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK);
    EXPECT_EQ(result, 0);

    elements = sizeof(diff_time_0ms_TIME) / sizeof(diff_time_0ms_TIME[0]);
    median = (osal_nano_sec_t)findMedian(diff_time_0ms_TIME, elements);
    delta_time = TEST_MEDIAN_EXPECT;
    EXPECT_LE(median, delta_time) << "Test02_003_012 failed";

    elements = sizeof(diff_time_0ms_OK) / sizeof(diff_time_0ms_OK[0]);
    median = (osal_nano_sec_t)findMedian(diff_time_0ms_OK, elements);
    delta_time = 100000; //100usec
    EXPECT_LE(median, delta_time) << "Test02_003_012 failed";

// post-process: destroy created mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK) << "post-process failed";;
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle1), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
    mtx_handle1 = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexLockForTimePeriod_TC_02_003_012]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexLockUntilTimeStamp_TC_02_004_001 ThreadSyncMutexApiTest.ThsyncMutexLockUntilTimeStamp_TC_02_004_001
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_001_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_001_tci Test Case ID
 * ThsyncMutexLockUntilTimeStamp_S1_N_TC_02_004_001
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_001_rr Related Requirement(s)
 * \#199154
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_001_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_001_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to Lock a mutex with timeout until time stamp in normal condition:
 * - pre-process: create a mutex
 * - Test02_004_001: all input/output param is valid.
 * - post-process: destroy created mutex
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_001_tpts Test Procedure/Test Steps
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Lock mutex by using R_OSAL_ThsyncMutexLockUntilTimeStamp with all input/output param is valid.
 * -# Unlock the mutex with R_OSAL_ThsyncMutexUnlock.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_001_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexLockUntilTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_001_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexLockUntilTimeStamp_TC_02_004_001
 */
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_001]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexLockUntilTimeStamp_TC_02_004_001)
{
    osal_mutex_id_t mutex_id;
    st_osal_time_t abs_time;

// pre-process: create a mutex
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_004_001
// @brief: normal case: all input/output param is valid
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time), OSAL_RETURN_OK);
    if(abs_time.tv_nsec + 2 * MSEC_TO_NSEC <= MAX_NSEC)
    {
        abs_time.tv_nsec = abs_time.tv_nsec + 2 * MSEC_TO_NSEC;
        abs_time.tv_sec += 1;
    }
    else
    {
        abs_time.tv_nsec = 2 * MSEC_TO_NSEC - (MAX_NSEC - abs_time.tv_nsec);
        abs_time.tv_sec += 2;
    }
    ASSERT_EQ(R_OSAL_ThsyncMutexLockUntilTimeStamp(mtx_handle, &abs_time), OSAL_RETURN_OK) << "Test02_004_001 failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK);

// post-process: destroy created mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_001]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexLockUntilTimeStamp_TC_02_004_002 ThreadSyncMutexApiTest.ThsyncMutexLockUntilTimeStamp_TC_02_004_002
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_002_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_002_tci Test Case ID
 * ThsyncMutexLockUntilTimeStamp_S1_BN_TC_02_004_002
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_002_rr Related Requirement(s)
 * \#199154
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_002_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_002_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to Lock a mutex with timeout until time stamp in normal condition:
 * - pre-process: create a mutex
 * - Test02_004_002: checking with "tv_nsec" is 1000000nsec.
 * - post-process: destroy created mutex
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_002_tpts Test Procedure/Test Steps
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Lock mutex by using R_OSAL_ThsyncMutexLockUntilTimeStamp with tv_nsec is 1000000 (1ms).
 * -# Unlock the mutex with R_OSAL_ThsyncMutexUnlock.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_002_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexLockUntilTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_002_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexLockUntilTimeStamp_TC_02_004_002
 */
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_002]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexLockUntilTimeStamp_TC_02_004_002)
{
    osal_mutex_id_t mutex_id;
    st_osal_time_t abs_time;

// pre-process: create a mutex
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_004_002
// @brief: boundary normal case: tv_nsec is 1000000 (1ms)
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time), OSAL_RETURN_OK);
    abs_time.tv_sec = abs_time.tv_sec + 1;
    abs_time.tv_nsec = 1 * MSEC_TO_NSEC;
    ASSERT_EQ(R_OSAL_ThsyncMutexLockUntilTimeStamp(mtx_handle, &abs_time), OSAL_RETURN_OK) << "Test02_004_002 failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK);

// post-process: destroy created mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_002]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexLockUntilTimeStamp_TC_02_004_003 ThreadSyncMutexApiTest.ThsyncMutexLockUntilTimeStamp_TC_02_004_003
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_003_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_003_tci Test Case ID
 * ThsyncMutexLockUntilTimeStamp_S1_BN_TC_02_004_003
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_003_rr Related Requirement(s)
 * \#199154
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_003_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_003_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to Lock a mutex with timeout until time stamp in normal condition:
 * - pre-process: create a mutex
 * - Test02_004_003: checking with "tv_nsec" is 999999999nsec.
 * - post-process: destroy created mutex
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_003_tpts Test Procedure/Test Steps
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Lock mutex by using R_OSAL_ThsyncMutexLockUntilTimeStamp with tv_nsec is 999999999 (1ms).
 * -# Unlock the mutex with R_OSAL_ThsyncMutexUnlock.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_003_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexLockUntilTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_003_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexLockUntilTimeStamp_TC_02_004_003
 */
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_003]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexLockUntilTimeStamp_TC_02_004_003)
{
    osal_mutex_id_t mutex_id;
    st_osal_time_t abs_time;

// pre-process: create a mutex
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_004_003
// @brief: boundary normal case: tv_nsec is 999999999 (1ms)
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time), OSAL_RETURN_OK);
    abs_time.tv_sec = abs_time.tv_sec + 1;
    abs_time.tv_nsec = MAX_NSEC;
    ASSERT_EQ(R_OSAL_ThsyncMutexLockUntilTimeStamp(mtx_handle, &abs_time), OSAL_RETURN_OK) << "Test02_004_003 failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK);

// post-process: destroy created mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_003]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexLockUntilTimeStamp_TC_02_004_004 ThreadSyncMutexApiTest.ThsyncMutexLockUntilTimeStamp_TC_02_004_004
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_004_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_004_tci Test Case ID
 * ThsyncMutexLockUntilTimeStamp_S1_N_TC_02_004_004
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_004_rr Related Requirement(s)
 * \#199154
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_004_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_004_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to Lock a mutex with timeout until time stamp in normal condition:
 * - pre-process: create a mutex
 * - Test02_004_004: checking round up feature with "tv_nsec" is less than 1000000nsec.
 * - post-process: destroy created mutex
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_004_tpts Test Procedure/Test Steps
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Lock mutex by using R_OSAL_ThsyncMutexLockUntilTimeStamp with tv_nsec is less than 1000000 (1ms).
 * -# Unlock the mutex with R_OSAL_ThsyncMutexUnlock.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_004_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexLockUntilTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_004_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexLockUntilTimeStamp_TC_02_004_004
 */
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_004]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexLockUntilTimeStamp_TC_02_004_004)
{
    osal_mutex_id_t mutex_id;
    st_osal_time_t abs_time;

// pre-process: create a mutex
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_004_004
// @brief: normal case: checking round up feature with tv_nsec is less than 1000000 (1ms)
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time), OSAL_RETURN_OK);
    abs_time.tv_sec = abs_time.tv_sec + 1;
    abs_time.tv_nsec = 111111;
    ASSERT_EQ(R_OSAL_ThsyncMutexLockUntilTimeStamp(mtx_handle, &abs_time), OSAL_RETURN_OK) << "Test02_004_004 failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK);

// post-process: destroy created mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_004]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexLockUntilTimeStamp_TC_02_004_005 ThreadSyncMutexApiTest.ThsyncMutexLockUntilTimeStamp_TC_02_004_005
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_005_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_005_tci Test Case ID
 * ThsyncMutexLockUntilTimeStamp_S0_A_TC_02_004_005
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_005_rr Related Requirement(s)
 * \#199148, \#199151, \#199154
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_005_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_005_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to Lock a mutex with timeout until time stamp in abnormal condition:
 * - Test02_004_005: mutex handler is NULL.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_005_tpts Test Procedure/Test Steps
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Lock mutex by using R_OSAL_ThsyncMutexLockUntilTimeStamp with the argument as below:
 *      - mutex handle is NULL
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_005_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexLockUntilTimeStamp return OSAL_RETURN_HANDLE.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_005_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexLockUntilTimeStamp_TC_02_004_005
 */
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_005]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexLockUntilTimeStamp_TC_02_004_005)
{
    st_osal_time_t abs_time;

// Test02_004_005
// @brief: abnormal case: mutex handler is NULL
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time), OSAL_RETURN_OK);
    if(abs_time.tv_nsec + 2 * MSEC_TO_NSEC <= MAX_NSEC)
    {
        abs_time.tv_nsec = abs_time.tv_nsec + 2 * MSEC_TO_NSEC;
        abs_time.tv_sec += 1;
    }
    else
    {
        abs_time.tv_nsec = 2 * MSEC_TO_NSEC - (MAX_NSEC - abs_time.tv_nsec);
        abs_time.tv_sec += 2;
    }
    ASSERT_EQ(R_OSAL_ThsyncMutexLockUntilTimeStamp(OSAL_MUTEX_HANDLE_INVALID, &abs_time), OSAL_RETURN_HANDLE) << "Test02_004_005 failed";
}
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_005]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexLockUntilTimeStamp_TC_02_004_006 ThreadSyncMutexApiTest.ThsyncMutexLockUntilTimeStamp_TC_02_004_006
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_006_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_006_tci Test Case ID
 * ThsyncMutexLockUntilTimeStamp_S0_A_TC_02_004_006
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_006_rr Related Requirement(s)
 * \#199154
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_006_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_006_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to Lock a mutex with timeout until time stamp in abnormal condition:
 * - pre-process: create a mutex
 * - Test02_004_006: p_time_stamp is NULL.
 * - post-process: destroy created mutex
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_006_tpts Test Procedure/Test Steps
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Lock mutex by using R_OSAL_ThsyncMutexLockUntilTimeStamp with the argument as below:
 *      - "p_time_stamp" is NULL
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_006_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ThsyncMutexLockUntilTimeStamp return OSAL_RETURN_PAR.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_006_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexLockUntilTimeStamp_TC_02_004_006
 */
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_006]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexLockUntilTimeStamp_TC_02_004_006)
{
    osal_mutex_id_t mutex_id;
    st_osal_time_t* p_abs_time = NULL;

// pre-process: create a mutex
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_004_006
// @brief: abnormal case: p_time_stamp is NULL
    ASSERT_EQ(R_OSAL_ThsyncMutexLockUntilTimeStamp(mtx_handle, p_abs_time), OSAL_RETURN_PAR) << "Test02_004_006 failed";

// post-process: destroy created mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_006]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexLockUntilTimeStamp_TC_02_004_007 ThreadSyncMutexApiTest.ThsyncMutexLockUntilTimeStamp_TC_02_004_007
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_007_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_007_tci Test Case ID
 * ThsyncMutexLockUntilTimeStamp_S0_BA_TC_02_004_007
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_007_rr Related Requirement(s)
 * \#199154
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_007_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_007_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to Lock a mutex with timeout until time stamp in abnormal condition:
 * - pre-process: create a mutex
 * - Test02_004_007: tv_nsec is invalid (bigger than 1ms).
 * - post-process: destroy created mutex
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_007_tpts Test Procedure/Test Steps
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Lock mutex by using R_OSAL_ThsyncMutexLockUntilTimeStamp with tv_nsec is invalid (bigger than 1sec).
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_007_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexLockUntilTimeStamp return OSAL_RETURN_PAR.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_007_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexLockUntilTimeStamp_TC_02_004_007
 */
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_007]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexLockUntilTimeStamp_TC_02_004_007)
{
    osal_mutex_id_t mutex_id;
    st_osal_time_t abs_time;

// pre-process: create a mutex
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_004_007
// @brief: abnormal case: tv_nsec is invalid (bigger than 1sec)
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time), OSAL_RETURN_OK);
    abs_time.tv_sec = abs_time.tv_sec + 1;
    abs_time.tv_nsec = 1 * SEC_TO_NSEC;
    ASSERT_EQ(R_OSAL_ThsyncMutexLockUntilTimeStamp(mtx_handle, &abs_time), OSAL_RETURN_PAR) << "Test02_004_007 failed";

// post-process: destroy created mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_007]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexLockUntilTimeStamp_TC_02_004_008 ThreadSyncMutexApiTest.ThsyncMutexLockUntilTimeStamp_TC_02_004_008
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_008_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_008_tci Test Case ID
 * ThsyncMutexLockUntilTimeStamp_S0_A_TC_02_004_008
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_008_rr Related Requirement(s)
 * \#199154
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_008_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_008_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to Lock a mutex with timeout until time stamp in abnormal condition:
 * - pre-process: create a mutex
 * - Test02_004_008: checking with passed timestamp.
 * - post-process: destroy created mutex
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_008_tpts Test Procedure/Test Steps
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# In case the "tv_sec" of the current time is less than 1s:
 *      - Suspend thread 2000ms with R_OSAL_ThreadSleepForTimePeriod.
 *      - Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION again.
 * -# Lock mutex by using R_OSAL_ThsyncMutexLockUntilTimeStamp with passed timestamp.
 * -# Unlock the mutex with R_OSAL_ThsyncMutexUnlock.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_008_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# In case the "tv_sec" of the current time is less than 1s:<br>
 * R_OSAL_ThreadSleepForTimePeriod return OSAL_RETURN_OK.<br>
 * R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexLockUntilTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_008_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexLockUntilTimeStamp_TC_02_004_008
 */
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_008]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexLockUntilTimeStamp_TC_02_004_008)
{
    osal_mutex_id_t mutex_id;
    st_osal_time_t abs_time;
    osal_milli_sec_t time;

// pre-process: create a mutex
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_004_008
// @brief: abnormal case: checking with passed timestamp
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time), OSAL_RETURN_OK);
    if (abs_time.tv_sec < 1)
    {
        time = 2000;
        ASSERT_EQ(R_OSAL_ThreadSleepForTimePeriod(time), OSAL_RETURN_OK);
        ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time), OSAL_RETURN_OK);
    }
    abs_time.tv_sec = abs_time.tv_sec - 1;
    ASSERT_EQ(R_OSAL_ThsyncMutexLockUntilTimeStamp(mtx_handle, &abs_time), OSAL_RETURN_OK) << "Test02_004_008 failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK);

// post-process: destroy created mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_008]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexLockUntilTimeStamp_TC_02_004_009 ThreadSyncMutexApiTest.ThsyncMutexLockUntilTimeStamp_TC_02_004_009
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_009_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_009_tci Test Case ID
 * ThsyncMutexLockUntilTimeStamp_S2_A_TC_02_004_009
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_009_rr Related Requirement(s)
 * \#199154
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_009_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_009_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to Lock a mutex with timeout until time stamp in abnormal condition:
 * - pre-process: create a mutex
 * - Test02_004_009: lock a mutex which has already been owned by another thread.
 * - post-process: destroy created mutex
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_009_tpts Test Procedure/Test Steps
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Lock mutex by using R_OSAL_ThsyncMutexLockUntilTimeStamp.
 * -# Create a thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is mutexabsolutelock_workerfunction function with functionality:
 *              - Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *              - Lock mutex by using R_OSAL_ThsyncMutexLockUntilTimeStamp.
 *          -# userarg includes mutex handle
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Address of thread handle.
 * -# Join thread by using R_OSAL_ThreadJoin.
 * -# Unlock the mutex with R_OSAL_ThsyncMutexUnlock.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_009_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexLockUntilTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK.<br>
 * Value of "thread_handle" is not NULL.<br>
 * In mutexabsolutelock_workerfunction function:<br>
 *  - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK
 *  - R_OSAL_ThsyncMutexLockUntilTimeStamp return OSAL_RETURN_TIME.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK.<br>
 * Value of "result" is equal to 0.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_009_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexLockUntilTimeStamp_TC_02_004_009
 * @snippet test_mutex.cpp mutexabsolutelock_workerfunction
 */
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_009]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexLockUntilTimeStamp_TC_02_004_009)
{
    osal_mutex_id_t mutex_id;
    st_osal_time_t abs_time;
    osal_thread_id_t uid;
    st_osal_thread_config_t config;
    int64_t result;

// pre-process: create a mutex
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_004_009
// @brief: abnormal case: lock a mutex which has already been owned by another thread.
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time), OSAL_RETURN_OK) << "Test02_004_009 failed";
    if(abs_time.tv_nsec + 2 * MSEC_TO_NSEC <= MAX_NSEC)
    {
        abs_time.tv_nsec = abs_time.tv_nsec + 2 * MSEC_TO_NSEC;
        abs_time.tv_sec += 1;
    }
    else
    {
        abs_time.tv_nsec = 2 * MSEC_TO_NSEC - (MAX_NSEC - abs_time.tv_nsec);
        abs_time.tv_sec += 2;
    }
    ASSERT_EQ(R_OSAL_ThsyncMutexLockUntilTimeStamp(mtx_handle, &abs_time), OSAL_RETURN_OK) << "Test02_004_009 failed";
    mutexlock_worker_arg user_arg(&mtx_handle);
    uid              = TEST_THREAD_ID1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = (char *)"Simple testing thread";
    config.func      = mutexabsolutelock_workerfunction;
    config.userarg   = &user_arg;
    config.stack_size = THREAD_STACK_SIZE;

    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK) << "Test02_004_009 failed";
    ASSERT_NE(thread_handle, NULL_PTR) << "Test02_004_009 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK) << "Test02_004_009 failed";
    EXPECT_EQ(result, 0) << "Test02_004_009 failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK) << "Test02_004_009 failed";
    thread_handle = OSAL_THREAD_HANDLE_INVALID;

// post-process: destroy created mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_009]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexLockUntilTimeStamp_TC_02_004_010 ThreadSyncMutexApiTest.ThsyncMutexLockUntilTimeStamp_TC_02_004_010
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_010_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_010_tci Test Case ID
 * ThsyncMutexLockUntilTimeStamp_S2_A_TC_02_004_010
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_010_rr Related Requirement(s)
 * \#199154
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_010_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_010_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to Lock a mutex with timeout until time stamp in abnormal condition:
 * - pre-process: create a mutex
 * - Test02_004_010: lock a mutex in a thread which has already owned this mutex.
 * - post-process: destroy created mutex
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_010_tpts Test Procedure/Test Steps
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Lock mutex by using R_OSAL_ThsyncMutexLockUntilTimeStamp.
 * -# Lock mutex by using R_OSAL_ThsyncMutexLockUntilTimeStamp again.
 * -# Unlock the mutex with R_OSAL_ThsyncMutexUnlock.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_010_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexLockUntilTimeStamp return OSAL_RETURN_OK.
 * -# Return value of R_OSAL_ThsyncMutexLockUntilTimeStamp is equal to OSAL_RETURN_FAIL or OSAL_RETURN_TIME.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_010_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexLockUntilTimeStamp_TC_02_004_010
 */
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_010]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexLockUntilTimeStamp_TC_02_004_010)
{
    osal_mutex_id_t mutex_id;
    st_osal_time_t abs_time;
    e_osal_return_t osal_ret;

// pre-process: create a mutex
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_004_010
// @brief: abnormal case: lock a mutex in a thread which has already owned this mutex
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time), OSAL_RETURN_OK);
    if(abs_time.tv_nsec + 2 * MSEC_TO_NSEC <= MAX_NSEC)
    {
        abs_time.tv_nsec = abs_time.tv_nsec + 2 * MSEC_TO_NSEC;
        abs_time.tv_sec += 1;
    }
    else
    {
        abs_time.tv_nsec = 2 * MSEC_TO_NSEC - (MAX_NSEC - abs_time.tv_nsec);
        abs_time.tv_sec += 2;
    }
    ASSERT_EQ(R_OSAL_ThsyncMutexLockUntilTimeStamp(mtx_handle, &abs_time), OSAL_RETURN_OK);
    osal_ret = R_OSAL_ThsyncMutexLockUntilTimeStamp(mtx_handle, &abs_time);
    ASSERT_TRUE((osal_ret == OSAL_RETURN_FAIL) || (osal_ret == OSAL_RETURN_TIME)) << "Test02_004_010 failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK);

// post-process: destroy created mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_010]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexLockUntilTimeStamp_TC_02_004_011 ThreadSyncMutexApiTest.ThsyncMutexLockUntilTimeStamp_TC_02_004_011
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_011_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_011_tci Test Case ID
 * ThsyncMutexLockUntilTimeStamp_S2_A_TC_02_004_011
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_011_rr Related Requirement(s)
 * \#199154
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_011_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_011_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to Lock a mutex with timeout until time stamp in abnormal condition:
 * - pre-process: create a mutex
 * - Test02_004_011: mutex handle has already been destroyed.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_011_tpts Test Procedure/Test Steps
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Lock mutex which mutex handle was destroyed using R_OSAL_ThsyncMutexLockUntilTimeStamp.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_011_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexLockUntilTimeStamp return OSAL_RETURN_HANDLE
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_011_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexLockUntilTimeStamp_TC_02_004_011
 */
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_011]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexLockUntilTimeStamp_TC_02_004_011)
{
    osal_mutex_id_t mutex_id;
    st_osal_time_t abs_time;

// pre-process: create a mutex
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_004_011
// @brief: abnormal case: mutex handle has already been destroyed
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK);
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time), OSAL_RETURN_OK);
    if(abs_time.tv_nsec + 2 * MSEC_TO_NSEC <= MAX_NSEC)
    {
        abs_time.tv_nsec = abs_time.tv_nsec + 2 * MSEC_TO_NSEC;
        abs_time.tv_sec += 1;
    }
    else
    {
        abs_time.tv_nsec = 2 * MSEC_TO_NSEC - (MAX_NSEC - abs_time.tv_nsec);
        abs_time.tv_sec += 2;
    }
    ASSERT_EQ(R_OSAL_ThsyncMutexLockUntilTimeStamp(mtx_handle, &abs_time), OSAL_RETURN_HANDLE) << "Test02_004_011 failed";
}
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_011]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section OsalApiStateTest_ThsyncMutexLockUntilTimeStamp_TC_02_004_012 OsalApiStateTest.ThsyncMutexLockUntilTimeStamp_TC_02_004_012
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_012_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_012_tci Test Case ID
 * ThsyncMutexLockUntilTimeStamp_S2_A_TC_02_004_012
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_012_rr Related Requirement(s)
 * \#199154
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_012_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * - Current timestamp "abs_time_state" has been got
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_012_tcs Test Case Summary
 * This test shall verify the state transition of Thread Synchronization Mutex of OSAL API
 * - Test02_004_012: State error of R_OSAL_ThsyncMutexLockUntilTimeStamp.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_012_tpts Test Procedure/Test Steps
 * -# Lock mutex by calling R_OSAL_ThsyncMutexLockUntilTimeStamp
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_012_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexLockUntilTimeStamp return OSAL_RETURN_STATE.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_012_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexLockUntilTimeStamp_TC_02_004_012
 */
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_012]
TEST_F(OsalApiStateTest, ThsyncMutexLockUntilTimeStamp_TC_02_004_012)
{
    osal_mutex_handle_t mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
    st_osal_time_t abs_time;

// Test02_004_012
// @brief: abnormal case: state transition error
    abs_time.tv_sec = abs_time_state.tv_sec + 1;
    abs_time.tv_nsec = abs_time_state.tv_nsec;
    EXPECT_EQ(R_OSAL_ThsyncMutexLockUntilTimeStamp(mtx_handle, &abs_time), OSAL_RETURN_STATE) << "Test02_004_012 failed";
}
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_012]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section OSALINTTESTS_ThsyncMutexLockUntilTimeStamp_TC_02_004_013 OSALINTTESTS.ThsyncMutexLockUntilTimeStamp_TC_02_004_013
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_013_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_013_tci Test Case ID
 * ThsyncMutexLockUntilTimeStamp_S3_N_TC_02_004_013
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_013_rr Related Requirement(s)
 * \#199154
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_013_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_013_tcs Test Case Summary
 * @subsubsection ThsyncMutexLockUntilTimeStamp_TC_02_004_013_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Create a mutex.
 * - Setup required memory for interrupt generation
 * - Register callack function and enable interrupt
 * - Wait for interrupt callback function finish
 * @subsubsection ThsyncMutexLockUntilTimeStamp_TC_02_004_013_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * - Destroy created mutex
 * @subsubsection ThsyncMutexLockUntilTimeStamp_TC_02_004_013_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality with normal condition of the ThreadSync Mutex APIs which are called in the ISR.
 * - Disable and clear interrupt status
 * - Test02_004_013: call R_OSAL_ThsyncMutexLockUntilTimeStamp in ISR
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_013_tpts Test Procedure/Test Steps
 * @subsubsection ThsyncMutexLockUntilTimeStamp_TC_02_004_013_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Create new mutex handle by using R_OSAL_ThsyncMutexCreate.<br>
 * Pass this mutex handle to interrupt callback function as the argument through R_OSAL_InterruptRegisterIsr.
 * -# Allocate required memory for generate interrupt from hardware and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function by using R_OSAL_InterruptRegisterIsr
 * -# Enable interrupt by using R_OSAL_InterruptEnableIsr
 * -# Setup hardware specific register to generate the interrupt by calling to setup_hw_register
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection ThsyncMutexLockUntilTimeStamp_TC_02_004_013_post_tpts Test Procedure/Test Steps of Post-process
 * -# Unregister interrupt by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * -# Destroy created mutex by calling R_OSAL_ThsyncMutexDestroy
 * @subsubsection ThsyncMutexLockUntilTimeStamp_TC_02_004_013_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Get the current time stamp by using R_OSAL_ClockTimeGetTimeStamp
 * -# Lock mutex by using R_OSAL_ThsyncMutexLockUntilTimeStamp
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock
 * -# Send Message interrupt of context finished by using R_OSAL_MqSendForTimePeriod
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_013_ereb Expected Result/Behavior
 * @subsubsection ThsyncMutexLockUntilTimeStamp_TC_02_004_013_pre_ereb Expected Result/Behavior of Pre-process
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mutex_handle" is not NULL_PTR
 * -# allocate_setup_data_memory run successfully
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * The value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK
 * -# setup_hw_register run successfully
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection ThsyncMutexLockUntilTimeStamp_TC_02_004_013_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK
 * @subsubsection ThsyncMutexLockUntilTimeStamp_TC_02_004_013_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK
 * -# R_OSAL_ThsyncMutexLockUntilTimeStamp return OSAL_RETURN_OK
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_013_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexLockUntilTimeStamp_TC_02_004_013
 * @snippet test_mutex.cpp osal_mutex_isr_test_normal_TC_02_004_013
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_013]
TEST_F(OSALINTTESTS, ThsyncMutexLockUntilTimeStamp_TC_02_004_013)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    osal_mutex_handle_t mutex_handle = OSAL_MUTEX_HANDLE_INVALID;
    osal_mutex_id_t mutex_id = TEST_MUTEX_ID1;
    osal_milli_sec_t time_period;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};

/* pre-process:
 * create a mutex
 * Setup required memory for interrupt generation.
 * Register callack function and enable interrupt 
 * Wait for interrupt callback function finish */

    EXPECT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(mutex_handle, NULL_PTR);
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    arg.additional_arg = (void*)&mutex_handle;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_mutex_isr_test_normal_TC_02_004_013, (void *)&arg), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK) << "pre-process failed";
    setup_hw_register(dev_arr, hw_addr);
    time_period = 2000;
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory
 * Destroy create mutex */
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_mutex_isr_test_normal_TC_02_004_013), OSAL_RETURN_OK) << "post-process failed";
    deallocate_data_memory(hndl_mmngr, buffer_obj);
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mutex_handle), OSAL_RETURN_OK) << "post-process failed";
    mutex_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_013]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section OSALINTTESTS_ThsyncMutexLockUntilTimeStamp_TC_02_004_014 OSALINTTESTS.ThsyncMutexLockUntilTimeStamp_TC_02_004_014
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_014_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_014_tci Test Case ID
 * ThsyncMutexLockUntilTimeStamp_S2_A_TC_02_004_014
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_014_rr Related Requirement(s)
 * \#199154
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_014_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_014_tcs Test Case Summary
 * @subsubsection ThsyncMutexLockUntilTimeStamp_TC_02_004_014_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Create a mutex.
 * - Setup required memory for interrupt generation
 * - Register callack function and enable interrupt
 * - Wait for interrupt callback function finish
 * @subsubsection ThsyncMutexLockUntilTimeStamp_TC_02_004_014_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * - Destroy created mutex
 * @subsubsection ThsyncMutexLockUntilTimeStamp_TC_02_004_014_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality with abnormal condition of the ThreadSync Mutex APIs which are called in the ISR.
 * - Disable and clear interrupt status
 * - Test02_004_014: call R_OSAL_ThsyncMutexLockUntilTimeStamp in ISR with a locked mutex
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_014_tpts Test Procedure/Test Steps
 * @subsubsection ThsyncMutexLockUntilTimeStamp_TC_02_004_014_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Create new mutex handle by using R_OSAL_ThsyncMutexCreate.<br>
 * Pass this mutex handle to interrupt callback function as the argument through R_OSAL_InterruptRegisterIsr.
 * -# Allocate required memory for generate interrupt from hardware and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function by using R_OSAL_InterruptRegisterIsr
 * -# Enable interrupt by using R_OSAL_InterruptEnableIsr
 * -# Setup hardware specific register to generate the interrupt by calling to setup_hw_register
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection ThsyncMutexLockUntilTimeStamp_TC_02_004_014_post_tpts Test Procedure/Test Steps of Post-process
 * -# Unregister interrupt by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * -# Destroy created mutex by calling R_OSAL_ThsyncMutexDestroy
 * @subsubsection ThsyncMutexLockUntilTimeStamp_TC_02_004_014_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Get the current time stamp by using R_OSAL_ClockTimeGetTimeStamp
 * -# Lock mutex by using R_OSAL_ThsyncMutexLockUntilTimeStamp
 * -# Continue lock the same mutex by using R_OSAL_ThsyncMutexLockUntilTimeStamp
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock
 * -# Send Message interrupt of context finished by using R_OSAL_MqSendForTimePeriod
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_014_ereb Expected Result/Behavior
 * @subsubsection ThsyncMutexLockUntilTimeStamp_TC_02_004_014_pre_ereb Expected Result/Behavior of Pre-process
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mutex_handle" is not NULL_PTR
 * -# allocate_setup_data_memory run successfully
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * The value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK
 * -# setup_hw_register run successfully
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection ThsyncMutexLockUntilTimeStamp_TC_02_004_014_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK
 * @subsubsection ThsyncMutexLockUntilTimeStamp_TC_02_004_014_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK
 * -# R_OSAL_ThsyncMutexLockUntilTimeStamp return OSAL_RETURN_OK
 * -# R_OSAL_ThsyncMutexLockUntilTimeStamp return OSAL_RETURN_BUSY
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_014_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexLockUntilTimeStamp_TC_02_004_014
 * @snippet test_mutex.cpp osal_mutex_isr_test_abnormal2
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_014]
TEST_F(OSALINTTESTS, ThsyncMutexLockUntilTimeStamp_TC_02_004_014)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    osal_mutex_handle_t mutex_handle = OSAL_MUTEX_HANDLE_INVALID;
    osal_mutex_id_t mutex_id = TEST_MUTEX_ID1;
    osal_milli_sec_t time_period;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};

/* pre-process:
 * create a mutex
 * Setup required memory for interrupt generation.
 * Register callack function and enable interrupt 
 * Wait for interrupt callback function finish */

    EXPECT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(mutex_handle, NULL_PTR);
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    arg.additional_arg = (void*)&mutex_handle;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_mutex_isr_test_abnormal2, (void *)&arg), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK) << "pre-process failed";
    setup_hw_register(dev_arr, hw_addr);
    time_period = 2000;
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory
 * Destroy create mutex */

    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_mutex_isr_test_abnormal2), OSAL_RETURN_OK) << "post-process failed";
    deallocate_data_memory(hndl_mmngr, buffer_obj);
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mutex_handle), OSAL_RETURN_OK) << "post-process failed";
    mutex_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_014]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexLockUntilTimeStamp_TC_02_004_015 ThreadSyncMutexApiTest.ThsyncMutexLockUntilTimeStamp_TC_02_004_015
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_015_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_015_tci Test Case ID
 * ThsyncMutexLockUntilTimeStamp_S0_A_TC_02_004_015
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_015_rr Related Requirement(s)
 * \#199154
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_015_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_015_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to Lock a mutex with timeout until time stamp in abnormal condition:
 * - pre-process: create a mutex
 * - Test02_004_015: checking with "tv_sec" is negative number.
 * - post-process: destroy created mutex
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_015_tpts Test Procedure/Test Steps
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate.
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Lock mutex by using R_OSAL_ThsyncMutexLockUntilTimeStamp with "tv_sec" is -1.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_015_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexLockUntilTimeStamp return OSAL_RETURN_PAR.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_015_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexLockUntilTimeStamp_TC_02_004_015
 */
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_015]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexLockUntilTimeStamp_TC_02_004_015)
{
    osal_mutex_id_t mutex_id;
    st_osal_time_t abs_time;

// pre-process: create a mutex
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_004_015
// @brief: abnormal case: tv_sec is negative number
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time), OSAL_RETURN_OK) << "Test02_004_015 failed";
    abs_time.tv_sec = -1;
    EXPECT_EQ(R_OSAL_ThsyncMutexLockUntilTimeStamp(mtx_handle, &abs_time), OSAL_RETURN_PAR) << "Test02_004_015 failed";

// post-process: destroy created mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_015]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexLockUntilTimeStamp_TC_02_004_016 ThreadSyncMutexApiTest.ThsyncMutexLockUntilTimeStamp_TC_02_004_016
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_016_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_016_tci Test Case ID
 * ThsyncMutexLockUntilTimeStamp_S0_BN_TC_02_004_016
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_016_rr Related Requirement(s)
 * \#199154
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_016_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_016_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to Lock a mutex with timeout until time stamp:
 * - pre-process: create a mutex
 * - Test02_004_016: checking with tv_sec is equal to zero
 * - post-process: destroy created mutex
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_016_tpts Test Procedure/Test Steps
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate.
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Lock mutex by using R_OSAL_ThsyncMutexLockUntilTimeStamp with tv_sec is equal to zero
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_016_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexLockUntilTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_016_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexLockUntilTimeStamp_TC_02_004_016
 */
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_016]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexLockUntilTimeStamp_TC_02_004_016)
{
    osal_mutex_id_t mutex_id;
    st_osal_time_t abs_time;

// pre-process: create a mutex
    mutex_id = TEST_MUTEX_ID1;
    EXPECT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_004_016
// @brief: boundary normal case: tv_sec is equal to zero
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time), OSAL_RETURN_OK) << "Test02_004_016 failed";
    abs_time.tv_sec = 0;
    EXPECT_EQ(R_OSAL_ThsyncMutexLockUntilTimeStamp(mtx_handle, &abs_time), OSAL_RETURN_OK) << "Test02_004_016 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK) << "Test02_004_016 failed";

// post-process: destroy created mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_016]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexLockUntilTimeStamp_TC_02_004_017 ThreadSyncMutexApiTest.ThsyncMutexLockUntilTimeStamp_TC_02_004_017
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_017_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_017_tci Test Case ID
 * ThsyncMutexLockUntilTimeStamp_S0_BN_TC_02_004_017
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_017_rr Related Requirement(s)
 * \#199154
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_017_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_017_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to Lock a mutex with timeout until time stamp:
 * - pre-process: create a mutex
 * - Test02_004_017: checking with tv_nsec is equal to zero
 * - post-process: destroy created mutex
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_017_tpts Test Procedure/Test Steps
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate.
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Lock mutex by using R_OSAL_ThsyncMutexLockUntilTimeStamp with tv_nsec is equal to zero
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_017_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexLockUntilTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_017_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexLockUntilTimeStamp_TC_02_004_017
 */
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_017]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexLockUntilTimeStamp_TC_02_004_017)
{
    osal_mutex_id_t mutex_id;
    st_osal_time_t abs_time;

// pre-process: create a mutex
    mutex_id = TEST_MUTEX_ID1;
    EXPECT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_004_017
// @brief: boundary normal case: tv_nsec is equal to zero
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time), OSAL_RETURN_OK) << "Test02_004_017 failed";
    abs_time.tv_nsec = 0;
    EXPECT_EQ(R_OSAL_ThsyncMutexLockUntilTimeStamp(mtx_handle, &abs_time), OSAL_RETURN_OK) << "Test02_004_017 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK) << "Test02_004_016 failed";

// post-process: destroy created mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_017]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexLockUntilTimeStamp_TC_02_004_018 ThreadSyncMutexApiTest.ThsyncMutexLockUntilTimeStamp_TC_02_004_018
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_018_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_018_tci Test Case ID
 * ThsyncMutexLockUntilTimeStamp_S0_BA_TC_02_004_018
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_018_rr Related Requirement(s)
 * \#199154
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_018_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_018_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to Lock a mutex with timeout until time stamp:
 * - pre-process: create a mutex
 * - Test02_004_018: checking with tv_nsec is less than zero
 * - post-process: destroy created mutex
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_018_tpts Test Procedure/Test Steps
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate.
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Lock mutex by using R_OSAL_ThsyncMutexLockUntilTimeStamp with tv_nsec is less than zero
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_018_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexLockUntilTimeStamp return OSAL_RETURN_PAR.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_018_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexLockUntilTimeStamp_TC_02_004_018
 */
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_018]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexLockUntilTimeStamp_TC_02_004_018)
{
    osal_mutex_id_t mutex_id;
    st_osal_time_t abs_time;

// pre-process: create a mutex
    mutex_id = TEST_MUTEX_ID1;
    EXPECT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_004_018
// @brief: boundary abnormal case: tv_nsec is less than zero
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time), OSAL_RETURN_OK) << "Test02_004_018 failed";
    abs_time.tv_nsec = -1;
    EXPECT_EQ(R_OSAL_ThsyncMutexLockUntilTimeStamp(mtx_handle, &abs_time), OSAL_RETURN_PAR) << "Test02_004_018 failed";

// post-process: destroy created mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_018]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexLockUntilTimeStamp_TC_02_004_019 ThreadSyncMutexApiTest.ThsyncMutexLockUntilTimeStamp_TC_02_004_019
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_019_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_019_tci Test Case ID
 * ThsyncMutexLockUntilTimeStamp_S0_BN_TC_02_004_019
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_019_rr Related Requirement(s)
 * \#199154
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_019_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_019_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to Lock a mutex with timeout until time stamp:
 * - pre-process: create a mutex
 * - Test02_004_019: checking with tv_sec and tv_nsec are equal to zero
 * - post-process: destroy created mutex
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_019_tpts Test Procedure/Test Steps
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate.
 * -# Lock mutex by using R_OSAL_ThsyncMutexLockUntilTimeStamp with tv_sec and tv_nsec are equal to zero
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_019_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ThsyncMutexLockUntilTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncMutexLockUntilTimeStamp_TC_02_004_019_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexLockUntilTimeStamp_TC_02_004_019
 */
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_019]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexLockUntilTimeStamp_TC_02_004_019)
{
    osal_mutex_id_t mutex_id;
    st_osal_time_t abs_time;

// pre-process: create a mutex
    mutex_id = TEST_MUTEX_ID1;
    EXPECT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_004_019
// @brief: boundary normal case: tv_sec and tv_nsec are equal to zero
    abs_time.tv_sec = 0;
    abs_time.tv_nsec = 0;
    EXPECT_EQ(R_OSAL_ThsyncMutexLockUntilTimeStamp(mtx_handle, &abs_time), OSAL_RETURN_OK) << "Test02_004_019 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK) << "Test02_004_019 failed";

// post-process: destroy created mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexLockUntilTimeStamp_TC_02_004_019]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexTryLock_TC_02_005_001 ThreadSyncMutexApiTest.ThsyncMutexTryLock_TC_02_005_001
 * @subsection ThsyncMutexTryLock_TC_02_005_001_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexTryLock_TC_02_005_001_tci Test Case ID
 * ThsyncMutexTryLock_S1_N_TC_02_005_001
 * @subsection ThsyncMutexTryLock_TC_02_005_001_rr Related Requirement(s)
 * \#199155
 * @subsection ThsyncMutexTryLock_TC_02_005_001_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexTryLock_TC_02_005_001_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to Lock a mutex with normal condition:
 * - pre-process: create a mutex
 * - Test02_005_001: all input/output param is valid
 * - pre-process: create a mutex
 * @subsection ThsyncMutexTryLock_TC_02_005_001_tpts Test Procedure/Test Steps
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Lock mutex by using R_OSAL_ThsyncMutexTryLock with all input/output param is valid.
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexTryLock_TC_02_005_001_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncMutexTryLock_TC_02_005_001_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexTryLock_TC_02_005_001
 */
// [ThsyncMutexTryLock_TC_02_005_001]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexTryLock_TC_02_005_001)
{
    osal_mutex_id_t mutex_id;

// pre-process: create a mutex
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_005_001
// @brief: normal case: all input/output param is valid
    ASSERT_EQ(R_OSAL_ThsyncMutexTryLock(mtx_handle), OSAL_RETURN_OK) << "Test02_005_001 failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK);

// post-process: destroy create mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexTryLock_TC_02_005_001]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexTryLock_TC_02_005_002 ThreadSyncMutexApiTest.ThsyncMutexTryLock_TC_02_005_002
 * @subsection ThsyncMutexTryLock_TC_02_005_002_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexTryLock_TC_02_005_002_tci Test Case ID
 * ThsyncMutexTryLock_S0_A_TC_02_005_002
 * @subsection ThsyncMutexTryLock_TC_02_005_002_rr Related Requirement(s)
 * \#199148, \#199151, \#199155
 * @subsection ThsyncMutexTryLock_TC_02_005_002_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexTryLock_TC_02_005_002_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to Lock a mutex with abnormal condition:
 * - Test02_005_002: mutex handle is NULL
 * @subsection ThsyncMutexTryLock_TC_02_005_002_tpts Test Procedure/Test Steps
 * -# Lock mutex with R_OSAL_ThsyncMutexTryLock with the argument as below:
 *      - mutex handler is NULL
 * @subsection ThsyncMutexTryLock_TC_02_005_002_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_HANDLE.
 * @subsection ThsyncMutexTryLock_TC_02_005_002_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexTryLock_TC_02_005_002
 */
// [ThsyncMutexTryLock_TC_02_005_002]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexTryLock_TC_02_005_002)
{
// Test02_005_002
// @brief: abnormal case: mutex handler is NULL
    ASSERT_EQ(R_OSAL_ThsyncMutexTryLock(OSAL_MUTEX_HANDLE_INVALID), OSAL_RETURN_HANDLE) << "Test02_005_002 failed";
}
// [ThsyncMutexTryLock_TC_02_005_002]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexTryLock_TC_02_005_003 ThreadSyncMutexApiTest.ThsyncMutexTryLock_TC_02_005_003
 * @subsection ThsyncMutexTryLock_TC_02_005_003_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexTryLock_TC_02_005_003_tci Test Case ID
 * ThsyncMutexTryLock_S2_A_TC_02_005_003
 * @subsection ThsyncMutexTryLock_TC_02_005_003_rr Related Requirement(s)
 * \#199155
 * @subsection ThsyncMutexTryLock_TC_02_005_003_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexTryLock_TC_02_005_003_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to Lock a mutex with abnormal condition:
 * - pre-process: create a mutex
 * - Test02_005_003: lock a mutex which has already been owned by another thread.
 * - post-process: destroy created mutex
 * @subsection ThsyncMutexTryLock_TC_02_005_003_tpts Test Procedure/Test Steps
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Lock mutex by using R_OSAL_ThsyncMutexTryLock.
 * -# Create a thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is mutextrylock_workerfunction function with functionality:
 *              - Lock mutex by using R_OSAL_ThsyncMutexTryLock.
 *          -# userarg includes mutex handle
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Address of thread handle.
 * -# Join thread by using R_OSAL_ThreadJoin.
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexTryLock_TC_02_005_003_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK.<br>
 * Value of "thread_handle" is not NULL.<br>
 * In mutextrylock_workerfunction function:
 *  - R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_BUSY.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK. <br>
 * Value of "result" is equal to 0.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncMutexTryLock_TC_02_005_003_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexTryLock_TC_02_005_003
 * @snippet test_mutex.cpp mutextrylock_workerfunction
 */
// [ThsyncMutexTryLock_TC_02_005_003]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexTryLock_TC_02_005_003)
{
    osal_mutex_id_t mutex_id;
    osal_thread_id_t uid;
    st_osal_thread_config_t config;
    int64_t result;

// pre-process: create a mutex
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_005_003
// @brief: abnormal case: lock a mutex which has already been owned by another thread.
    ASSERT_EQ(R_OSAL_ThsyncMutexTryLock(mtx_handle), OSAL_RETURN_OK) << "Test02_005_003 failed";
    mutexlock_worker_arg user_arg(&mtx_handle);
    uid              = TEST_THREAD_ID1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = (char *)"Simple testing thread";
    config.func      = mutextrylock_workerfunction;
    config.userarg   = &user_arg;
    config.stack_size = THREAD_STACK_SIZE;

    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK) << "Test02_005_003 failed";
    ASSERT_NE(thread_handle, NULL_PTR) << "Test02_005_003 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK) << "Test02_005_003 failed";
    EXPECT_EQ(result, 0) << "Test02_005_003 failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK) << "Test02_005_003 failed";
    thread_handle = OSAL_THREAD_HANDLE_INVALID;

// post-process: destroy create mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK);
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexTryLock_TC_02_005_003]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexTryLock_TC_02_005_004 ThreadSyncMutexApiTest.ThsyncMutexTryLock_TC_02_005_004
 * @subsection ThsyncMutexTryLock_TC_02_005_004_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexTryLock_TC_02_005_004_tci Test Case ID
 * ThsyncMutexTryLock_S2_A_TC_02_005_004
 * @subsection ThsyncMutexTryLock_TC_02_005_004_rr Related Requirement(s)
 * \#199155
 * @subsection ThsyncMutexTryLock_TC_02_005_004_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexTryLock_TC_02_005_004_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to Lock a mutex with abnormal condition:
 * - pre-process: create a mutex
 * - Test02_005_004: lock a mutex in a thread which has already owned this mutex.
 * - pre-process: create a mutex
 * @subsection ThsyncMutexTryLock_TC_02_005_004_tpts Test Procedure/Test Steps
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Lock mutex by using R_OSAL_ThsyncMutexTryLock.
 * -# Lock mutex with R_OSAL_ThsyncMutexTryLock again.
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexTryLock_TC_02_005_004_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_OK.
 * -# Return value of R_OSAL_ThsyncMutexTryLock is equal to OSAL_RETURN_FAIL or OSAL_RETURN_BUSY.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncMutexTryLock_TC_02_005_004_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexTryLock_TC_02_005_004
 */
// [ThsyncMutexTryLock_TC_02_005_004]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexTryLock_TC_02_005_004)
{
    osal_mutex_id_t mutex_id;
    e_osal_return_t osal_ret;

// pre-process: create a mutex
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_005_004
// @brief: abnormal case: lock a mutex in a thread which has already owned this mutex
    ASSERT_EQ(R_OSAL_ThsyncMutexTryLock(mtx_handle), OSAL_RETURN_OK);
    osal_ret = R_OSAL_ThsyncMutexTryLock(mtx_handle);
    ASSERT_TRUE((osal_ret == OSAL_RETURN_FAIL) || (osal_ret == OSAL_RETURN_BUSY)) << "Test02_005_004 failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK);

// post-process: destroy create mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexTryLock_TC_02_005_004]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexTryLock_TC_02_005_005 ThreadSyncMutexApiTest.ThsyncMutexTryLock_TC_02_005_005
 * @subsection ThsyncMutexTryLock_TC_02_005_005_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexTryLock_TC_02_005_005_tci Test Case ID
 * ThsyncMutexTryLock_S2_A_TC_02_005_005
 * @subsection ThsyncMutexTryLock_TC_02_005_005_rr Related Requirement(s)
 * \#199155
 * @subsection ThsyncMutexTryLock_TC_02_005_005_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexTryLock_TC_02_005_005_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to Lock a mutex with abnormal condition:
 * - pre-process: create a mutex
 * - Test02_005_005: mutex handle has already been destroyed.
 * @subsection ThsyncMutexTryLock_TC_02_005_005_tpts Test Procedure/Test Steps
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * -# Lock mutex which handle has already been destroyed by using R_OSAL_ThsyncMutexTryLock.
 * @subsection ThsyncMutexTryLock_TC_02_005_005_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_HANDLE.
 * @subsection ThsyncMutexTryLock_TC_02_005_005_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexTryLock_TC_02_005_005
 */
// [ThsyncMutexTryLock_TC_02_005_005]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexTryLock_TC_02_005_005)
{
    osal_mutex_id_t mutex_id;

// pre-process: create a mutex
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_005_005
// @brief: abnormal case: mutex handle has already been destroyed
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK);
    ASSERT_EQ(R_OSAL_ThsyncMutexTryLock(mtx_handle), OSAL_RETURN_HANDLE) << "Test02_005_005 failed";
}
// [ThsyncMutexTryLock_TC_02_005_005]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section OsalApiStateTest_ThsyncMutexTryLock_TC_02_005_006 OsalApiStateTest.ThsyncMutexTryLock_TC_02_005_006
 * @subsection ThsyncMutexTryLock_TC_02_005_006_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection ThsyncMutexTryLock_TC_02_005_006_tci Test Case ID
 * ThsyncMutexTryLock_S2_A_TC_02_005_006
 * @subsection ThsyncMutexTryLock_TC_02_005_006_rr Related Requirement(s)
 * \#199155
 * @subsection ThsyncMutexTryLock_TC_02_005_006_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection ThsyncMutexTryLock_TC_02_005_006_tcs Test Case Summary
 * This test shall verify the state transition of Thread Synchronization Mutex of OSAL API
 * - Test02_005_006: State error of R_OSAL_ThsyncMutexTryLock.
 * @subsection ThsyncMutexTryLock_TC_02_005_006_tpts Test Procedure/Test Steps
 * -# Lock mutex by calling R_OSAL_ThsyncMutexTryLock
 * @subsection ThsyncMutexTryLock_TC_02_005_006_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_STATE.
 * @subsection ThsyncMutexTryLock_TC_02_005_006_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexTryLock_TC_02_005_006
 */
// [ThsyncMutexTryLock_TC_02_005_006]
TEST_F(OsalApiStateTest, ThsyncMutexTryLock_TC_02_005_006)
{
    osal_mutex_handle_t mtx_handle = OSAL_MUTEX_HANDLE_INVALID;

// Test02_005_006
// @brief: abnormal case: state transition error
    EXPECT_EQ(R_OSAL_ThsyncMutexTryLock(mtx_handle), OSAL_RETURN_STATE) << "Test02_005_006 failed";
}
// [ThsyncMutexTryLock_TC_02_005_006]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section OSALINTTESTS_ThsyncMutexTryLock_TC_02_005_007 OSALINTTESTS.ThsyncMutexTryLock_TC_02_005_007
 * @subsection ThsyncMutexTryLock_TC_02_005_007_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection ThsyncMutexTryLock_TC_02_005_007_tci Test Case ID
 * ThsyncMutexTryLock_S3_N_TC_02_005_007
 * @subsection ThsyncMutexTryLock_TC_02_005_007_rr Related Requirement(s)
 * \#199155
 * @subsection ThsyncMutexTryLock_TC_02_005_007_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection ThsyncMutexTryLock_TC_02_005_007_tcs Test Case Summary
 * @subsubsection ThsyncMutexTryLock_TC_02_005_007_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Create a mutex.
 * - Setup required memory for interrupt generation
 * - Register callack function and enable interrupt
 * - Wait for interrupt callback function finish
 * @subsubsection ThsyncMutexTryLock_TC_02_005_007_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * - Destroy created mutex
 * @subsubsection ThsyncMutexTryLock_TC_02_005_007_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality with normal condition of the ThreadSync Mutex APIs which are called in the ISR.
 * - Disable and clear interrupt status
 * - Test02_005_007: call R_OSAL_ThsyncMutexTryLock in ISR
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection ThsyncMutexTryLock_TC_02_005_007_tpts Test Procedure/Test Steps
 * @subsubsection ThsyncMutexTryLock_TC_02_005_007_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Create new mutex handle by using R_OSAL_ThsyncMutexCreate.<br>
 * Pass this mutex handle to interrupt callback function as the argument through R_OSAL_InterruptRegisterIsr.
 * -# Allocate required memory for generate interrupt from hardware and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function by using R_OSAL_InterruptRegisterIsr
 * -# Enable interrupt by using R_OSAL_InterruptEnableIsr
 * -# Setup hardware specific register to generate the interrupt by calling to setup_hw_register
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection ThsyncMutexTryLock_TC_02_005_007_post_tpts Test Procedure/Test Steps of Post-process
 * -# Unregister interrupt by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * -# Destroy created mutex by calling R_OSAL_ThsyncMutexDestroy
 * @subsubsection ThsyncMutexTryLock_TC_02_005_007_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Lock mutex by using R_OSAL_ThsyncMutexTryLock
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock
 * -# Send Message interrupt of context finished by using R_OSAL_MqSendForTimePeriod
 * @subsection ThsyncMutexTryLock_TC_02_005_007_ereb Expected Result/Behavior
 * @subsubsection ThsyncMutexTryLock_TC_02_005_007_pre_ereb Expected Result/Behavior of Pre-process
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mutex_handle" is not NULL_PTR
 * -# allocate_setup_data_memory run successfully
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * The value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK
 * -# setup_hw_register run successfully
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection ThsyncMutexTryLock_TC_02_005_007_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK
 * @subsubsection ThsyncMutexTryLock_TC_02_005_007_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_OK
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * @subsection ThsyncMutexTryLock_TC_02_005_007_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexTryLock_TC_02_005_007
 * @snippet test_mutex.cpp osal_mutex_isr_test_normal_TC_02_005_007
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [ThsyncMutexTryLock_TC_02_005_007]
TEST_F(OSALINTTESTS, ThsyncMutexTryLock_TC_02_005_007)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    osal_mutex_handle_t mutex_handle = OSAL_MUTEX_HANDLE_INVALID;
    osal_mutex_id_t mutex_id = TEST_MUTEX_ID1;
    osal_milli_sec_t time_period;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};

/* pre-process:
 * create a mutex
 * Setup required memory for interrupt generation.
 * Register callack function and enable interrupt 
 * Wait for interrupt callback function finish */

    EXPECT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(mutex_handle, NULL_PTR);
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    arg.additional_arg = (void*)&mutex_handle;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_mutex_isr_test_normal_TC_02_005_007, (void *)&arg), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK) << "pre-process failed";
    setup_hw_register(dev_arr, hw_addr);
    time_period = 2000;
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory
 * Destroy create mutex */
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_mutex_isr_test_normal_TC_02_005_007), OSAL_RETURN_OK) << "post-process failed";
    deallocate_data_memory(hndl_mmngr, buffer_obj);
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mutex_handle), OSAL_RETURN_OK) << "post-process failed";
    mutex_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexTryLock_TC_02_005_007]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section OSALINTTESTS_ThsyncMutexTryLock_TC_02_005_008 OSALINTTESTS.ThsyncMutexTryLock_TC_02_005_008
 * @subsection ThsyncMutexTryLock_TC_02_005_008_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection ThsyncMutexTryLock_TC_02_005_008_tci Test Case ID
 * ThsyncMutexTryLock_S2_A_TC_02_005_008
 * @subsection ThsyncMutexTryLock_TC_02_005_008_rr Related Requirement(s)
 * \#199155
 * @subsection ThsyncMutexTryLock_TC_02_005_008_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection ThsyncMutexTryLock_TC_02_005_008_tcs Test Case Summary
 * @subsubsection ThsyncMutexTryLock_TC_02_005_008_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Create a mutex.
 * - Setup required memory for interrupt generation
 * - Register callack function and enable interrupt
 * - Wait for interrupt callback function finish
 * @subsubsection ThsyncMutexTryLock_TC_02_005_008_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * - Destroy created mutex
 * @subsubsection ThsyncMutexTryLock_TC_02_005_008_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality with abnormal condition of the ThreadSync Mutex APIs which are called in the ISR.
 * - Disable and clear interrupt status
 * - Test02_005_008: call R_OSAL_ThsyncMutexTryLock in ISR with a locked mutex
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection ThsyncMutexTryLock_TC_02_005_008_tpts Test Procedure/Test Steps
 * @subsubsection ThsyncMutexTryLock_TC_02_005_008_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Create new mutex handle by using R_OSAL_ThsyncMutexCreate.<br>
 * Pass this mutex handle to interrupt callback function as the argument through R_OSAL_InterruptRegisterIsr.
 * -# Allocate required memory for generate interrupt from hardware and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function by using R_OSAL_InterruptRegisterIsr
 * -# Enable interrupt by using R_OSAL_InterruptEnableIsr
 * -# Setup hardware specific register to generate the interrupt by calling to setup_hw_register
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection ThsyncMutexTryLock_TC_02_005_008_post_tpts Test Procedure/Test Steps of Post-process
 * -# Unregister interrupt by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * -# Destroy created mutex by calling R_OSAL_ThsyncMutexDestroy
 * @subsubsection ThsyncMutexTryLock_TC_02_005_008_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Lock mutex by using R_OSAL_ThsyncMutexTryLock
 * -# Continue lock the same mutex by using R_OSAL_ThsyncMutexTryLock
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock
 * -# Send Message interrupt of context finished by using R_OSAL_MqSendForTimePeriod
 * @subsection ThsyncMutexTryLock_TC_02_005_008_ereb Expected Result/Behavior
 * @subsubsection ThsyncMutexTryLock_TC_02_005_008_pre_ereb Expected Result/Behavior of Pre-process
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mutex_handle" is not NULL_PTR
 * -# allocate_setup_data_memory run successfully
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * The value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK
 * -# setup_hw_register run successfully
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection ThsyncMutexTryLock_TC_02_005_008_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK
 * @subsubsection ThsyncMutexTryLock_TC_02_005_008_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_OK
 * -# R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_BUSY
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * @subsection ThsyncMutexTryLock_TC_02_005_008_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexTryLock_TC_02_005_008
 * @snippet test_mutex.cpp osal_mutex_isr_test_abnormal3
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [ThsyncMutexTryLock_TC_02_005_008]
TEST_F(OSALINTTESTS, ThsyncMutexTryLock_TC_02_005_008)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    osal_mutex_handle_t mutex_handle = OSAL_MUTEX_HANDLE_INVALID;
    osal_mutex_id_t mutex_id = TEST_MUTEX_ID1;
    osal_milli_sec_t time_period;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};

/* pre-process:
 * create a mutex
 * Setup required memory for interrupt generation.
 * Register callack function and enable interrupt 
 * Wait for interrupt callback function finish */

    EXPECT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(mutex_handle, NULL_PTR);
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    arg.additional_arg = (void*)&mutex_handle;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_mutex_isr_test_abnormal3, (void *)&arg), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK) << "pre-process failed";
    setup_hw_register(dev_arr, hw_addr);
    time_period = 2000;
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory
 * Destroy create mutex */

    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_mutex_isr_test_abnormal3), OSAL_RETURN_OK) << "post-process failed";
    deallocate_data_memory(hndl_mmngr, buffer_obj);
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mutex_handle), OSAL_RETURN_OK) << "post-process failed";
    mutex_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexTryLock_TC_02_005_008]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexUnlock_TC_02_006_001 ThreadSyncMutexApiTest.ThsyncMutexUnlock_TC_02_006_001
 * @subsection ThsyncMutexUnlock_TC_02_006_001_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexUnlock_TC_02_006_001_tci Test Case ID
 * ThsyncMutexUnlock_S1_N_TC_02_006_001
 * @subsection ThsyncMutexUnlock_TC_02_006_001_rr Related Requirement(s)
 * \#199156
 * @subsection ThsyncMutexUnlock_TC_02_006_001_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexUnlock_TC_02_006_001_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to unlock a mutex with normal condition:
 * - pre-process: create a mutex
 * - Test02_006_001: all input/output param is valid.
 * - post-process: destroy created mutex
 * @subsection ThsyncMutexUnlock_TC_02_006_001_tpts Test Procedure/Test Steps
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Lock mutex by using R_OSAL_ThsyncMutexTryLock.
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock all input/output param is valid.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexUnlock_TC_02_006_001_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncMutexUnlock_TC_02_006_001_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexUnlock_TC_02_006_001
 */
// [ThsyncMutexUnlock_TC_02_006_001]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexUnlock_TC_02_006_001)
{
    osal_mutex_id_t mutex_id;

// pre-process: create a mutex
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_006_001
// @brief: normal case: all input/output param is valid
    ASSERT_EQ(R_OSAL_ThsyncMutexTryLock(mtx_handle), OSAL_RETURN_OK);
    ASSERT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK) << "Test02_006_001 failed";

// post-process: destroy created mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexUnlock_TC_02_006_001]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexUnlock_TC_02_006_002 ThreadSyncMutexApiTest.ThsyncMutexUnlock_TC_02_006_002
 * @subsection ThsyncMutexUnlock_TC_02_006_002_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexUnlock_TC_02_006_002_tci Test Case ID
 * ThsyncMutexUnlock_S0_A_TC_02_006_002
 * @subsection ThsyncMutexUnlock_TC_02_006_002_rr Related Requirement(s)
 * \#199148, \#199151, \#199156
 * @subsection ThsyncMutexUnlock_TC_02_006_002_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexUnlock_TC_02_006_002_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to unlock a mutex with abnormal condition:
 * - Test02_006_002: mutex handle is NULL.
 * @subsection ThsyncMutexUnlock_TC_02_006_002_tpts Test Procedure/Test Steps
 * -# Unlock mutex mtx_handle by using R_OSAL_ThsyncMutexUnlock with the argument as below:
 *      - mutex handler is NULL
 * @subsection ThsyncMutexUnlock_TC_02_006_002_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_HANDLE.
 * @subsection ThsyncMutexUnlock_TC_02_006_002_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexUnlock_TC_02_006_002
 */
// [ThsyncMutexUnlock_TC_02_006_002]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexUnlock_TC_02_006_002)
{
// Test02_006_002
// @brief: abnormal case: mutex handler is NULL
    ASSERT_EQ(R_OSAL_ThsyncMutexUnlock(OSAL_MUTEX_HANDLE_INVALID), OSAL_RETURN_HANDLE) << "Test02_006_002 failed";
}
// [ThsyncMutexUnlock_TC_02_006_002]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexUnlock_TC_02_006_003 ThreadSyncMutexApiTest.ThsyncMutexUnlock_TC_02_006_003
 * @subsection ThsyncMutexUnlock_TC_02_006_003_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexUnlock_TC_02_006_003_tci Test Case ID
 * ThsyncMutexUnlock_S4_A_TC_02_006_003
 * @subsection ThsyncMutexUnlock_TC_02_006_003_rr Related Requirement(s)
 * \#199156
 * @subsection ThsyncMutexUnlock_TC_02_006_003_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexUnlock_TC_02_006_003_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to unlock a mutex with abnormal condition:
 * - pre-process: create a mutex
 * - Test02_006_003: unlock a mutex which has already unlocked.
 * - post-process: destroy created mutex.
 * @subsection ThsyncMutexUnlock_TC_02_006_003_tpts Test Procedure/Test Steps
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Lock mutex mtx_handle using R_OSAL_ThsyncMutexTryLock.
 * -# Unlock mutex mtx_handle by using R_OSAL_ThsyncMutexUnlock.
 * -# Unlock mutex mtx_handle again by using R_OSAL_ThsyncMutexUnlock.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexUnlock_TC_02_006_003_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_FAIL.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncMutexUnlock_TC_02_006_003_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexUnlock_TC_02_006_003
 */
// [ThsyncMutexUnlock_TC_02_006_003]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexUnlock_TC_02_006_003)
{
    osal_mutex_id_t mutex_id;

// pre-process: create a mutex
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_006_003
// @brief: abnormal case: unlock a mutex which has already unlocked
    EXPECT_EQ(R_OSAL_ThsyncMutexTryLock(mtx_handle), OSAL_RETURN_OK) << "Test02_006_003 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK) << "Test02_006_003 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_FAIL) << "Test02_006_003 failed";

// post-process: destroy created mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexUnlock_TC_02_006_003]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexUnlock_TC_02_006_004 ThreadSyncMutexApiTest.ThsyncMutexUnlock_TC_02_006_004
 * @subsection ThsyncMutexUnlock_TC_02_006_004_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexUnlock_TC_02_006_004_tci Test Case ID
 * ThsyncMutexUnlock_S2_A_TC_02_006_004
 * @subsection ThsyncMutexUnlock_TC_02_006_004_rr Related Requirement(s)
 * \#199152, \#199156
 * @subsection ThsyncMutexUnlock_TC_02_006_004_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexUnlock_TC_02_006_004_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to unlock a mutex with abnormal condition:
 * - pre-process: create a mutex
 * - Test02_006_004: unlock a mutex which is not locked.
 * - post-process: destroy created mutex.
 * @subsection ThsyncMutexUnlock_TC_02_006_004_tpts Test Procedure/Test Steps
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Unlock mutex "mtx_handle" by using R_OSAL_ThsyncMutexUnlock.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexUnlock_TC_02_006_004_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_FAIL.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncMutexUnlock_TC_02_006_004_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexUnlock_TC_02_006_004
 */
// [ThsyncMutexUnlock_TC_02_006_004]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexUnlock_TC_02_006_004)
{
    osal_mutex_id_t mutex_id;

// pre-process
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_006_004
// @brief: abnormal case: unlock a mutex which is not locked
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_FAIL) << "Test02_006_004 failed";

// post-process: destroy created mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexUnlock_TC_02_006_004]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section OsalApiStateTest_ThsyncMutexUnlock_TC_02_006_005 OsalApiStateTest.ThsyncMutexUnlock_TC_02_006_005
 * @subsection ThsyncMutexUnlock_TC_02_006_005_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection ThsyncMutexUnlock_TC_02_006_005_tci Test Case ID
 * ThsyncMutexUnlock_S2_A_TC_02_006_005
 * @subsection ThsyncMutexUnlock_TC_02_006_005_rr Related Requirement(s)
 * \#199156
 * @subsection ThsyncMutexUnlock_TC_02_006_005_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection ThsyncMutexUnlock_TC_02_006_005_tcs Test Case Summary
 * This test shall verify the state transition of Thread Synchronization Mutex of OSAL API
 * - Test02_006_005: State error of R_OSAL_ThsyncMutexUnlock.
 * @subsection ThsyncMutexUnlock_TC_02_006_005_tpts Test Procedure/Test Steps
 * -# Unlock mutex by caliing R_OSAL_ThsyncMutexUnlock
 * @subsection ThsyncMutexUnlock_TC_02_006_005_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_STATE.
 * @subsection ThsyncMutexUnlock_TC_02_006_005_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexUnlock_TC_02_006_005
 */
// [ThsyncMutexUnlock_TC_02_006_005]
TEST_F(OsalApiStateTest, ThsyncMutexUnlock_TC_02_006_005)
{
    osal_mutex_handle_t mtx_handle = OSAL_MUTEX_HANDLE_INVALID;

// Test02_006_005
// @brief: abnormal case: state transition error
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_STATE) << "Test02_006_005 failed";
}
// [ThsyncMutexUnlock_TC_02_006_005]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section OSALINTTESTS_ThsyncMutexUnlock_TC_02_006_006 OSALINTTESTS.ThsyncMutexUnlock_TC_02_006_006
 * @subsection ThsyncMutexUnlock_TC_02_006_006_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection ThsyncMutexUnlock_TC_02_006_006_tci Test Case ID
 * ThsyncMutexUnlock_S1_N_TC_02_006_006
 * @subsection ThsyncMutexUnlock_TC_02_006_006_rr Related Requirement(s)
 * \#199156
 * @subsection ThsyncMutexUnlock_TC_02_006_006_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection ThsyncMutexUnlock_TC_02_006_006_tcs Test Case Summary
 * @subsubsection ThsyncMutexUnlock_TC_02_006_006_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Create a mutex.
 * - Setup required memory for interrupt generation
 * - Register callack function and enable interrupt
 * - Wait for interrupt callback function finish
 * @subsubsection ThsyncMutexUnlock_TC_02_006_006_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * - Destroy created mutex
 * @subsubsection ThsyncMutexUnlock_TC_02_006_006_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality with normal condition of the ThreadSync Mutex APIs which are called in the ISR.
 * - Disable and clear interrupt status
 * - Test02_006_006: call R_OSAL_ThsyncMutexUnlock in IS
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection ThsyncMutexUnlock_TC_02_006_006_tpts Test Procedure/Test Steps
 * @subsubsection ThsyncMutexUnlock_TC_02_006_006_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Create new mutex handle by using R_OSAL_ThsyncMutexCreate.<br>
 * Pass this mutex handle to interrupt callback function as the argument through R_OSAL_InterruptRegisterIsr.
 * -# Allocate required memory for generate interrupt from hardware and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function by using R_OSAL_InterruptRegisterIsr
 * -# Enable interrupt by using R_OSAL_InterruptEnableIsr
 * -# Setup hardware specific register to generate the interrupt by calling to setup_hw_register
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection ThsyncMutexUnlock_TC_02_006_006_post_tpts Test Procedure/Test Steps of Post-process
 * -# Unregister interrupt by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * -# Destroy created mutex by calling R_OSAL_ThsyncMutexDestroy
 * @subsubsection ThsyncMutexUnlock_TC_02_006_006_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Lock mutex by using R_OSAL_ThsyncMutexTryLock
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock
 * -# Send Message interrupt of context finished by using R_OSAL_MqSendForTimePeriod
 * @subsection ThsyncMutexUnlock_TC_02_006_006_ereb Expected Result/Behavior
 * @subsubsection ThsyncMutexUnlock_TC_02_006_006_pre_ereb Expected Result/Behavior of Pre-process
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mutex_handle" is not NULL_PTR
 * -# allocate_setup_data_memory run successfully
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * The value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK
 * -# setup_hw_register run successfully
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection ThsyncMutexUnlock_TC_02_006_006_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK
 * @subsubsection ThsyncMutexUnlock_TC_02_006_006_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_OK
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * @subsection ThsyncMutexUnlock_TC_02_006_006_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexUnlock_TC_02_006_006
 * @snippet test_mutex.cpp osal_mutex_isr_test_normal_TC_02_006_006
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [ThsyncMutexUnlock_TC_02_006_006]
TEST_F(OSALINTTESTS, ThsyncMutexUnlock_TC_02_006_006)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    osal_mutex_handle_t mutex_handle = OSAL_MUTEX_HANDLE_INVALID;
    osal_mutex_id_t mutex_id = TEST_MUTEX_ID1;
    osal_milli_sec_t time_period;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};

/* pre-process:
 * create a mutex
 * Setup required memory for interrupt generation.
 * Register callack function and enable interrupt 
 * Wait for interrupt callback function finish */

    EXPECT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(mutex_handle, NULL_PTR);
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    arg.additional_arg = (void*)&mutex_handle;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_mutex_isr_test_normal_TC_02_006_006, (void *)&arg), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK) << "pre-process failed";
    setup_hw_register(dev_arr, hw_addr);
    time_period = 2000;
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory
 * Destroy create mutex */
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_mutex_isr_test_normal_TC_02_006_006), OSAL_RETURN_OK) << "post-process failed";
    deallocate_data_memory(hndl_mmngr, buffer_obj);
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mutex_handle), OSAL_RETURN_OK) << "post-process failed";
    mutex_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexUnlock_TC_02_006_006]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section OSALINTTESTS_ThsyncMutexUnlock_TC_02_006_007 OSALINTTESTS.ThsyncMutexUnlock_TC_02_006_007
 * @subsection ThsyncMutexUnlock_TC_02_006_007_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection ThsyncMutexUnlock_TC_02_006_007_tci Test Case ID
 * R_OSAL_ThsyncMutexUnlock_S2_A_TC_02_006_007
 * @subsection ThsyncMutexUnlock_TC_02_006_007_rr Related Requirement(s)
 * \#199156
 * @subsection ThsyncMutexUnlock_TC_02_006_007_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection ThsyncMutexUnlock_TC_02_006_007_tcs Test Case Summary
 * @subsubsection ThsyncMutexUnlock_TC_02_006_007_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Create a mutex.
 * - Setup required memory for interrupt generation
 * - Register callack function and enable interrupt
 * - Wait for interrupt callback function finish
 * @subsubsection ThsyncMutexUnlock_TC_02_006_007_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * - Destroy created mutex
 * @subsubsection ThsyncMutexUnlock_TC_02_006_007_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality with abnormal condition of the ThreadSync Mutex APIs which are called in the ISR.
 * - Disable and clear interrupt status
 * - Test02_006_007: call R_OSAL_ThsyncMutexUnlock in ISR with unlocked mutex
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection ThsyncMutexUnlock_TC_02_006_007_tpts Test Procedure/Test Steps
 * @subsubsection ThsyncMutexUnlock_TC_02_006_007_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Create new mutex handle by using R_OSAL_ThsyncMutexCreate.<br>
 * Pass this mutex handle to interrupt callback function as the argument through R_OSAL_InterruptRegisterIsr.
 * -# Allocate required memory for generate interrupt from hardware and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function by using R_OSAL_InterruptRegisterIsr
 * -# Enable interrupt by using R_OSAL_InterruptEnableIsr
 * -# Setup hardware specific register to generate the interrupt by calling to setup_hw_register
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection ThsyncMutexUnlock_TC_02_006_007_post_tpts Test Procedure/Test Steps of Post-process
 * -# Unregister interrupt by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * -# Destroy created mutex by calling R_OSAL_ThsyncMutexDestroy
 * @subsubsection ThsyncMutexUnlock_TC_02_006_007_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Lock mutex by using R_OSAL_ThsyncMutexTryLock
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock
 * -# Continue unlock mutex by using R_OSAL_ThsyncMutexUnlock
 * -# Send Message interrupt of context finished by using R_OSAL_MqSendForTimePeriod
 * @subsection ThsyncMutexUnlock_TC_02_006_007_ereb Expected Result/Behavior
 * @subsubsection ThsyncMutexUnlock_TC_02_006_007_pre_ereb Expected Result/Behavior of Pre-process
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mutex_handle" is not NULL_PTR
 * -# allocate_setup_data_memory run successfully
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * The value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK
 * -# setup_hw_register run successfully
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection ThsyncMutexUnlock_TC_02_006_007_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK
 * @subsubsection ThsyncMutexUnlock_TC_02_006_007_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_OK
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_FAIL
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * @subsection ThsyncMutexUnlock_TC_02_006_007_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexUnlock_TC_02_006_007
 * @snippet test_mutex.cpp osal_mutex_isr_test_abnormal4
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [ThsyncMutexUnlock_TC_02_006_007]
TEST_F(OSALINTTESTS, ThsyncMutexUnlock_TC_02_006_007)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    osal_mutex_handle_t mutex_handle = OSAL_MUTEX_HANDLE_INVALID;
    osal_mutex_id_t mutex_id = TEST_MUTEX_ID1;
    osal_milli_sec_t time_period;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};

/* pre-process:
 * create a mutex
 * Setup required memory for interrupt generation.
 * Register callack function and enable interrupt 
 * Wait for interrupt callback function finish */

    EXPECT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(mutex_handle, NULL_PTR);
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    arg.additional_arg = (void*)&mutex_handle;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_mutex_isr_test_abnormal4, (void *)&arg), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK) << "pre-process failed";
    setup_hw_register(dev_arr, hw_addr);
    time_period = 2000;
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory
 * Destroy create mutex */

    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_mutex_isr_test_abnormal4), OSAL_RETURN_OK) << "post-process failed";
    deallocate_data_memory(hndl_mmngr, buffer_obj);
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mutex_handle), OSAL_RETURN_OK) << "post-process failed";
    mutex_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexUnlock_TC_02_006_007]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexUnlock_TC_02_006_008 ThreadSyncMutexApiTest.ThsyncMutexUnlock_TC_02_006_008
 * @subsection ThsyncMutexUnlock_TC_02_006_008_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexUnlock_TC_02_006_008_tci Test Case ID
 * ThsyncMutexUnlock_S2_A_TC_02_006_008
 * @subsection ThsyncMutexUnlock_TC_02_006_008_rr Related Requirement(s)
 * \#199156
 * @subsection ThsyncMutexUnlock_TC_02_006_008_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexUnlock_TC_02_006_008_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to unlock a mutex with abnormal condition:
 * - pre-process: create a mutex
 * - Test02_006_008: Unlock a mutex which not owned by the calling thread.
 * - post-process: destroy created mutex
 * @subsection ThsyncMutexUnlock_TC_02_006_008_tpts Test Procedure/Test Steps
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate.
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 * -# Create a thread using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is "mutexunlock_workerfunction" function with functionality:
 *              - Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 *          -# userarg includes mutex handle
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1
 *      - Address of thread handle.
 * -# Join "Simple testing thread" by using R_OSAL_ThreadJoin.
 * -# Unlock the mutex with R_OSAL_ThsyncMutexUnlock.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncMutexUnlock_TC_02_006_008_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK.<br>
 * Value of "thread_handle" is not NULL.<br>
 * In mutexunlock_workerfunction function:<br>
 *  - R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_FAIL.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncMutexUnlock_TC_02_006_008_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexUnlock_TC_02_006_008
 * @snippet test_mutex.cpp mutexunlock_workerfunction
 */
// [ThsyncMutexUnlock_TC_02_006_008]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexUnlock_TC_02_006_008)
{
    osal_mutex_id_t mutex_id;
    osal_milli_sec_t time;
    osal_thread_id_t uid;
    st_osal_thread_config_t config;
    int64_t result;

// pre-process: create a mutex
    mutex_id = TEST_MUTEX_ID1;
    EXPECT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_006_008
// @brief: abnormal case: Unlock a mutex which not owned by the calling thread.
    time = 10;
    EXPECT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(mtx_handle, time), OSAL_RETURN_OK) << "Test02_006_008 failed";
    mutexlock_worker_arg user_arg(&mtx_handle);
    uid              = TEST_THREAD_ID1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = (char *)"Simple testing thread";
    config.func      = mutexunlock_workerfunction;
    config.userarg   = &user_arg;
    config.stack_size = THREAD_STACK_SIZE;

    EXPECT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK) << "Test02_006_008 failed";
    EXPECT_NE(thread_handle, NULL_PTR);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK) << "Test02_006_008 failed";
    EXPECT_EQ(result, (int64_t)OSAL_RETURN_FAIL) << "Test02_006_008 failed";;
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK) << "Test02_006_008 failed";
    thread_handle = OSAL_THREAD_HANDLE_INVALID;

// post-process: destroy created mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncMutexUnlock_TC_02_006_008]

/**
 * @page ThreadSync_TEST ThreadSync Mutex API Test
 * @section ThreadSyncMutexApiTest_ThsyncMutexUnlock_TC_02_006_009 ThreadSyncMutexApiTest.ThsyncMutexUnlock_TC_02_006_009
 * @subsection ThsyncMutexUnlock_TC_02_006_009_tsi Test Suite ID
 * ThreadSyncMutexApiTest
 * @subsection ThsyncMutexUnlock_TC_02_006_009_tci Test Case ID
 * ThsyncMutexUnlock_S2_A_TC_02_006_009
 * @subsection ThsyncMutexUnlock_TC_02_006_009_rr Related Requirement(s)
 * \#199156, \#199157 
 * @subsection ThsyncMutexUnlock_TC_02_006_009_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMutexUnlock_TC_02_006_009_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to unlock a mutex with abnormal condition:
 * - pre-process: create a mutex and destroy the created mutex
 * - Test02_006_009: Unlock a mutex which has already been destroyed
 * @subsection ThsyncMutexUnlock_TC_02_006_009_tpts Test Procedure/Test Steps
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * -# Unlock mutex "mtx_handle" by using R_OSAL_ThsyncMutexUnlock.
 * @subsection ThsyncMutexUnlock_TC_02_006_009_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_HANDLE.
 * @subsection ThsyncMutexUnlock_TC_02_006_009_testcase Test Case Example
 * @snippet test_mutex.cpp ThsyncMutexUnlock_TC_02_006_009
 */
// [ThsyncMutexUnlock_TC_02_006_009]
TEST_F(ThreadSyncMutexApiTest, ThsyncMutexUnlock_TC_02_006_009)
{
    osal_mutex_id_t mutex_id;

/* pre-process:
 * Create a mutex
 * Destroy the created mutex
*/
    mutex_id = TEST_MUTEX_ID1;
    EXPECT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(mtx_handle, OSAL_MUTEX_HANDLE_INVALID) << "pre-process failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "pre-process failed";

// Test02_006_009
// @brief: abnormal case: Unlock a mutex which has already been destroyed
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_HANDLE) << "Test02_006_009 failed";
}
// [ThsyncMutexUnlock_TC_02_006_009]
