/***********************************************************************************************************************
* Copyright [2021] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    test_resource_measurement.cpp
* Version :      2.0.1
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  About Create API, performance measurement and memory leak measurement when Open/Close is repeated
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           2.0.0     29.10.2021    Create New
 *           2.0.1     18.11.2021    Fix Typo
 *           3.0.0     07.04.2022    Enable the disabled test case in SIL
***********************************************************************************************************************/
#include "rcar-xos/osal/r_osal.h"
#if !defined(OSAL_TEST_FRAMEWORK)
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <vector>
#include <cstdint>
using ::testing::Each;
using ::testing::Ge;
using ::testing::Not;
using namespace std;
#else
#include "osal_test_framework.hpp"
#include "osal_test_mock_framework.hpp"
#include <stdint.h>
using namespace OsalTestFrameworkMatcher;
using namespace OsalFWHelper;
#endif

#include "test_common.hpp"

#ifdef HAVE_MALLINFO
#include <malloc.h>
#endif // HAVE_MALLINFO

#if defined(ENABLE_LONGTIME_TEST) /* Long time test */
constexpr size_t NO_OF_THREADS          = 500; /* Number of threads to use in the runs */
constexpr size_t NO_OF_MUTEXS           = 500;
constexpr size_t NO_OF_CONDS            = 500;
constexpr size_t NO_OF_MQS              = 500;
constexpr size_t NO_OF_IO               = 500;
constexpr size_t NO_OF_MM_ALLOC_DEALLOC = 500;  /* Maximum number of runs */
constexpr size_t NO_OF_MAX_RUNS         = 100; /* Maximum number of runs */
constexpr size_t NO_OF_MM_OC            = 500;
constexpr size_t NO_OF_MM_ALLOC_DEALLOC_MAX_RUNS = 1000;  /* Maximum number of runs */
#else
constexpr size_t NO_OF_THREADS          = 10; /* Number of threads to use in the runs */
constexpr size_t NO_OF_MUTEXS           = 10;
constexpr size_t NO_OF_CONDS            = 10;
constexpr size_t NO_OF_MQS              = 10;
constexpr size_t NO_OF_IO               = 10;
constexpr size_t NO_OF_MM_ALLOC_DEALLOC = 10; /* Maximum number of runs */
constexpr size_t NO_OF_MAX_RUNS         = 10; /* Maximum number of runs */
constexpr size_t NO_OF_MM_OC            = 10;
constexpr size_t NO_OF_MM_ALLOC_DEALLOC_MAX_RUNS = 10;  /* Maximum number of runs */
#endif

constexpr size_t NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME =
    10; /*!< amount of consecutive raise in execution time */

#ifdef HAVE_MALLINFO
static size_t getTotalAllocatedSpace()
{
#ifdef HAVE_MALLINFO2
    struct mallinfo2 mi;

    mi = mallinfo2();
    return mi.uordblks;
#else
    struct mallinfo mi;

    mi = mallinfo();
    return mi.uordblks;
#endif
}
#endif

/**
 * @page Performance_TEST OSAL Resource Performance Test
 * @test
 * @test @link OsalResourcePerformance_ThreadJoin_TC_01_006_008 @endlink
 * @test @link OsalResourcePerformance_MutexDestroy_TC_02_002_008 @endlink
 * @test @link OsalResourcePerformance_CondDestroy_TC_02_008_008 @endlink
 * @test @link OsalResourcePerformance_MqDelete_TC_03_002_010 @endlink
 * @test @link OsalResourcePerformance_IoDeviceClose_TC_06_004_007 @endlink
 * @test @link OsalResourcePerformance_MemoryClose_TC_09_002_015 @endlink
 * @test @link OsalResourcePerformance_MemoryClose_TC_09_002_016 @endlink
 * @test @link OsalResourcePerformance_MemoryClose_TC_09_002_017 @endlink
 * @test @link OsalResourcePerformance_MemoryClose_TC_09_002_018 @endlink
 * @test @link OsalResourcePerformance_MemoryDealloc_TC_09_004_035 @endlink
 * @test @link OsalResourcePerformance_MemoryDealloc_TC_09_004_036 @endlink
 * @test @link OsalResourcePerformance_MemoryDealloc_TC_09_004_037 @endlink
 * @test @link OsalResourcePerformance_MemoryDealloc_TC_09_004_038 @endlink
 * @section OsalResourcePerformance OsalResourcePerformance function
 * @subsection OsalResourcePerformance_SetUp OsalResourcePerformance Setup
 * Common to Test Suit of OsalResourcePerformance, prepare process to be executed before executing the test. <br>
 * For example:
 * @snippet test_resource_measurement.cpp OsalResourcePerformance_SetUp
 * @subsection OsalResourcePerformance_TearDown OsalResourcePerformance TearDown
 * Common to Test Suit of OsalResourcePerformance, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_resource_measurement.cpp OsalResourcePerformance_TearDown
 */

class OsalResourcePerformance : public PARENT_TEST_FW_CLASS
{
protected:
// [OsalResourcePerformance_SetUp]
    void SetUp() override
    {
        m_thrd_hdnl_list.resize(NO_OF_THREADS, OSAL_THREAD_HANDLE_INVALID);
        m_mtx_hdnl_list.resize(NO_OF_MUTEXS, OSAL_MUTEX_HANDLE_INVALID);
        m_cond_hdnl_list.resize(NO_OF_CONDS, OSAL_COND_HANDLE_INVALID);
        m_mq_hdnl_list.resize(NO_OF_MQS, OSAL_MQ_HANDLE_INVALID);
        m_device_hdnl = OSAL_DEVICE_HANDLE_INVALID;
        m_mm_hdnl = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
        m_mm_buf_list.resize(NO_OF_MQS, OSAL_MEMORY_BUFFER_HANDLE_INVALID);
    }
// [OsalResourcePerformance_SetUp]

// [OsalResourcePerformance_TearDown]
    void TearDown() override
    {
        for(auto it = m_thrd_hdnl_list.begin(); it != m_thrd_hdnl_list.end(); it++)
        {
            auto func = [](osal_thread_handle_t & el) {
                if (OSAL_THREAD_HANDLE_INVALID != el)
                {
                    int64_t ret_val = -1;
                    R_OSAL_ThreadJoin(el, &ret_val);
                    el = OSAL_THREAD_HANDLE_INVALID;
                }
            };
            func(*it);
        }

        for(auto it = m_mtx_hdnl_list.begin(); it != m_mtx_hdnl_list.end(); it++)
        {
            auto func = [](osal_mutex_handle_t & el) {
                if (OSAL_MUTEX_HANDLE_INVALID != el)
                {
                    R_OSAL_ThsyncMutexDestroy(el);
                    el = OSAL_MUTEX_HANDLE_INVALID;
                }
            };
            func(*it);
        }

        for(auto it = m_cond_hdnl_list.begin(); it != m_cond_hdnl_list.end(); it++)
        {
            auto func = [](osal_cond_handle_t & el) {
                if (OSAL_COND_HANDLE_INVALID != el)
                {
                    R_OSAL_ThsyncCondDestroy(el);
                    el = OSAL_COND_HANDLE_INVALID;
                }
            };
            func(*it);
        }

        for(auto it = m_mq_hdnl_list.begin(); it != m_mq_hdnl_list.end(); it++)
        {
            auto func = [](osal_mq_handle_t & el) {
                if (OSAL_MQ_HANDLE_INVALID != el)
                {
                    R_OSAL_MqDelete(el);
                    el = OSAL_MQ_HANDLE_INVALID;
                }
            };
            func(*it);
        }
    }
// [OsalResourcePerformance_TearDown]

    vector<osal_thread_handle_t> m_thrd_hdnl_list;
    vector<osal_mutex_handle_t> m_mtx_hdnl_list;
    vector<osal_cond_handle_t> m_cond_hdnl_list;
    vector<osal_mq_handle_t> m_mq_hdnl_list;
    osal_device_handle_t m_device_hdnl;
    osal_memory_manager_handle_t m_mm_hdnl;
    vector<osal_memory_buffer_handle_t> m_mm_buf_list;
};

/**
 * @page Performance_TEST OSAL Resource Performance Test
 * @section OsalResourcePerformance_ThreadJoin_TC_01_006_008 OsalResourcePerformance.ThreadJoin_TC_01_006_008
 * @subsection ThreadJoin_TC_01_006_008_tsi Test Suite ID
 * OsalResourcePerformance
 * @subsection ThreadJoin_TC_01_006_008_tci Test Case ID
 * ThreadJoin_S1_N_TC_01_006_008
 * @subsection ThreadJoin_TC_01_006_008_rr Related Requirement(s)
 * \#199135, \#199139, \#199140, \#199145
 * @subsection ThreadJoin_TC_01_006_008_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadJoin_TC_01_006_008_tcs Test Case Summary
 * - Test01_006_008: This test shall verify the performance of thread creation/destroying through its execution time and memory consumption
 * @subsection ThreadJoin_TC_01_006_008_tpts Test Procedure/Test Steps
 * -# Use a for loop with loop counter is 100 with the sequence call as below:
 *      - Get the total allocated memory before execution time measurement start
 *      - Get the start timestamp by using R_OSAL_ClockTimeGetTimeStamp
 *      - Create 500 new threads by using R_OSAL_ThreadCreate
 *      - Join 500 created threads by using R_OSAL_ThreadJoin
 *      - Get the end timestamp by using R_OSAL_ClockTimeGetTimeStamp
 *      - Calculate the execution time by using R_OSAL_ClockTimeCalculateTimeDifference
 *      - Get the total allocated memory after execution time measurement end
 *      - Calculate the gap value of execution time/allocated memory of current loop with the previous loop (only in last 10 loop times).<br>
 *        Store these gap values in a gap list.
 * @subsection ThreadJoin_TC_01_006_008_ereb Expected Result/Behavior
 * -# This is the expectation in for loop:
 *      - Get the total allocated memory successful
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK
 *      - R_OSAL_ThreadCreate return OSAL_RETURN_OK
 *      - R_OSAL_ThreadJoin return OSAL_RETURN_OK
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK
 *      - R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK
 *      - Get the total allocated memory successful
 *      - All elements in gap list must not greater than 0.
 * @subsection ThreadJoin_TC_01_006_008_testcase Test Case Example
 * @snippet test_resource_measurement.cpp ThreadJoin_TC_01_006_008
 */
// [ThreadJoin_TC_01_006_008]
TEST_F(OsalResourcePerformance, ThreadJoin_TC_01_006_008)
{
    constexpr osal_milli_sec_t NOTHING_TASK_WAIT_TIME = 1;
    static auto                doNothingTask          = [](void * user_arg) -> int64_t {
        R_OSAL_ThreadSleepForTimePeriod(reinterpret_cast<osal_milli_sec_t>(user_arg));
        return 42;
    }; /*!< Dummy task just waiting for some time */

    st_osal_thread_config_t thrd_cfg;
    thrd_cfg.func       = doNothingTask;
    thrd_cfg.priority   = OSAL_THREAD_PRIORITY_TYPE0;
    thrd_cfg.stack_size = 64 * 1024 * 1024;
    thrd_cfg.task_name  = "do_nothing_tsk";
    thrd_cfg.userarg    = reinterpret_cast<void *>(NOTHING_TASK_WAIT_TIME);

    auto run = [this, &thrd_cfg]() -> osal_nano_sec_t {
        /* this is a run, it will re-use Thread IDs if called multiple times */
        st_osal_time_t  start_time;
        st_osal_time_t  end_time;
        osal_nano_sec_t exec_time;
        int64_t         thrd_return_value = -1;

        /* get starting timestamp */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &start_time));

        /* create and start NO_OF_THREADS threads */
        for (int i = 0; i < NO_OF_THREADS; i++)
        {
            /* start thread */
            EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ThreadCreate(&thrd_cfg, TEST_THREAD_ID1 + i, &m_thrd_hdnl_list[i]));
        }

        /* wait until all threads finished */
        for (int i = 0; i < NO_OF_THREADS; i++)
        {
            if (OSAL_THREAD_HANDLE_INVALID != m_thrd_hdnl_list[i])
            {
                EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ThreadJoin(m_thrd_hdnl_list[i], &thrd_return_value));
                m_thrd_hdnl_list[i] = OSAL_THREAD_HANDLE_INVALID;
            }
        }
        /* get end timestamp */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &end_time));

        /* calculate diff */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeCalculateTimeDifference(&end_time, &start_time, &exec_time));

        return exec_time;
    };

    //vector<osal_nano_sec_t> exec_time_diff_list;
    vector<osal_nano_sec_t> exec_time_diff_list;
    size_t                 max_exec_time_exceeded_cnt = 0;
    /* fill the list with NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME elements with minimal values for osal_nano_sec_t */
    exec_time_diff_list.resize(NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, INT64_MIN);

#ifdef HAVE_MALLINFO
    vector<int64_t> heap_usage_diff_list;
    heap_usage_diff_list.resize(NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, INT64_MIN);
#endif // HAVE_MALLINFO

    osal_nano_sec_t last_exec_time = NOTHING_TASK_WAIT_TIME * 1e6; /* start with NOTHING_TASK_WAIT_TIME*/
    for (size_t j = 0; j < NO_OF_MAX_RUNS; j++)
    {
#ifdef HAVE_MALLINFO
        int64_t heap_usage_before = getTotalAllocatedSpace();
#endif //#ifdef HAVE_MALLINFO
        /* execute run */
        osal_nano_sec_t exec_time = run();
        printf("%d/%d\n", j + 1, NO_OF_MAX_RUNS);
#ifdef HAVE_MALLINFO
        int64_t heap_usage_after = getTotalAllocatedSpace();
#endif //#ifdef HAVE_MALLINFO

        // At the beginning of measurement, measured value is not stable. Hence, the latest NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME times is used.
        // The increase of memory consumption and execution time will saturate after many times(NO_OF_MAX_RUNS(100) times may be enough) of executions.
        // If those are not saturated, there is memory leaking or performance degradation caused by wrapper implementation.
        if(j > NO_OF_MAX_RUNS - NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME)
        {
            /* kind of ring buffer for exec_time_dif_list */
            exec_time_diff_list.erase(exec_time_diff_list.begin());                           /* remove first element */
            exec_time_diff_list.push_back(exec_time - last_exec_time); /* add new at the end */

            /* if we measure a raise in execution for a NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, the next line while fail*/
            ASSERT_THAT(exec_time_diff_list, Not(Each(Ge(0))))
                << "We measured for " << NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME
                << " consecutive runs an increase in execution time --> Performance degradation over time detected";

#ifdef HAVE_MALLINFO
        /* kind of ring buffer for heap_usage_diff_list */
        heap_usage_diff_list.erase(heap_usage_diff_list.begin());                                     /* remove first element */
        heap_usage_diff_list.push_back(heap_usage_after - heap_usage_before); /* add new at the end */
        /* if we measure a raise in heap usage for a NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, the next line while
         * fail*/
        ASSERT_THAT(heap_usage_diff_list, Not(Each(Ge(0))))
            << "We measured for " << NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME
            << " consecutive runs an increase in heap usage --> 'Memory Leak' detected";
#endif //#ifdef HAVE_MALLINFO
        }

        last_exec_time = exec_time;
    }
}
// [ThreadJoin_TC_01_006_008]

/**
 * @page Performance_TEST OSAL Resource Performance Test
 * @section OsalResourcePerformance_MutexDestroy_TC_02_002_008 OsalResourcePerformance.MutexDestroy_TC_02_002_008
 * @subsection MutexDestroy_TC_02_002_008_tsi Test Suite ID
 * OsalResourcePerformance
 * @subsection MutexDestroy_TC_02_002_008_tci Test Case ID
 * MutexDestroy_S1_N_TC_02_002_008
 * @subsection MutexDestroy_TC_02_002_008_rr Related Requirement(s)
 * \#199150, \#199157
 * @subsection MutexDestroy_TC_02_002_008_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MutexDestroy_TC_02_002_008_tcs Test Case Summary
 * - Test02_002_008: This test shall verify the performance of mutex creation/destroying through its execution time and memory consumption
 * @subsection MutexDestroy_TC_02_002_008_tpts Test Procedure/Test Steps
 * -# Use a for loop with loop counter is 100 with the sequence call as below:
 *      - Get the total allocated memory before execution time measurement start
 *      - Get the start timestamp by using R_OSAL_ClockTimeGetTimeStamp
 *      - Create 500 new mutexs by using R_OSAL_ThsyncMutexCreate
 *      - Destroy 500 created mutexs by using R_OSAL_ThsyncMutexDestroy
 *      - Get the end timestamp by using R_OSAL_ClockTimeGetTimeStamp
 *      - Calculate the execution time by using R_OSAL_ClockTimeCalculateTimeDifference
 *      - Get the total allocated memory after execution time measurement end
 *      - Calculate the gap value of execution time/allocated memory of current loop with the previous loop (only in last 10 loop times).<br>
 *        Store these gap values in a gap list.
 * @subsection MutexDestroy_TC_02_002_008_ereb Expected Result/Behavior
 * -# This is the expectation in for loop:
 *      - Get the total allocated memory successful
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK
 *      - R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK
 *      - R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK
 *      - R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK
 *      - Get the total allocated memory successful
 *      - All elements in gap list must not greater than 0.
 * @subsection MutexDestroy_TC_02_002_008_testcase Test Case Example
 * @snippet test_resource_measurement.cpp MutexDestroy_TC_02_002_008
 */
// [MutexDestroy_TC_02_002_008]
TEST_F(OsalResourcePerformance, MutexDestroy_TC_02_002_008)
{
    auto run = [this]() -> osal_nano_sec_t {
        st_osal_time_t  start_time;
        st_osal_time_t  end_time;
        osal_nano_sec_t exec_time;

        /* get starting timestamp */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &start_time));

        for (int i = 0; i < NO_OF_MUTEXS; i++)
        {
            EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ThsyncMutexCreate(TEST_MUTEX_ID1 + i, &m_mtx_hdnl_list[i]));
        }

        for (int i = 0; i < NO_OF_MUTEXS; i++)
        {
            EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ThsyncMutexDestroy(m_mtx_hdnl_list[i]));
        }

        /* get end timestamp */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &end_time));

        /* calculate diff */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeCalculateTimeDifference(&end_time, &start_time, &exec_time));

        return exec_time;
    };

    vector<osal_nano_sec_t> exec_time_diff_list;
    size_t                 max_exec_time_exceeded_cnt = 0;
    /* fill the list with NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME elements with minimal values for osal_nano_sec_t */
    exec_time_diff_list.resize(NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, INT64_MIN);

#ifdef HAVE_MALLINFO
    vector<int64_t> heap_usage_diff_list;
    heap_usage_diff_list.resize(NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, INT64_MIN);
#endif // HAVE_MALLINFO

    osal_nano_sec_t last_exec_time;
    for (size_t j = 0; j < NO_OF_MAX_RUNS; j++)
    {
#ifdef HAVE_MALLINFO
        int64_t heap_usage_before = getTotalAllocatedSpace();
#endif //#ifdef HAVE_MALLINFO
        /* execute run */
        osal_nano_sec_t exec_time = run();
        printf("%d/%d\n", j + 1, NO_OF_MAX_RUNS);
#ifdef HAVE_MALLINFO
        int64_t heap_usage_after = getTotalAllocatedSpace();
#endif //#ifdef HAVE_MALLINFO

        // At the beginning of measurement, measured value is not stable. Hence, the latest NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME times is used.
        // The increase of memory consumption and execution time will saturate after many times(NO_OF_MAX_RUNS(100) times may be enough) of executions.
        // If those are not saturated, there is memory leaking or performance degradation caused by wrapper implementation.
        if(j > NO_OF_MAX_RUNS - NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME)
        {
            /* kind of ring buffer for exec_time_dif_list */
            exec_time_diff_list.erase(exec_time_diff_list.begin());                           /* remove first element */
            exec_time_diff_list.push_back(exec_time - last_exec_time); /* add new at the end */

            /* if we measure a raise in execution for a NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, the next line while fail*/
            ASSERT_THAT(exec_time_diff_list, Not(Each(Ge(0))))
                << "We measured for " << NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME
                << " consecutive runs an increase in execution time --> Performance degradation over time detected";

#ifdef HAVE_MALLINFO
        /* kind of ring buffer for heap_usage_diff_list */
        heap_usage_diff_list.erase(heap_usage_diff_list.begin());                                     /* remove first element */
        heap_usage_diff_list.push_back(heap_usage_after - heap_usage_before); /* add new at the end */
        /* if we measure a raise in heap usage for a NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, the next line while
         * fail*/
        ASSERT_THAT(heap_usage_diff_list, Not(Each(Ge(0))))
            << "We measured for " << NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME
            << " consecutive runs an increase in heap usage --> 'Memory Leak' detected";
#endif //#ifdef HAVE_MALLINFO
        }
        last_exec_time = exec_time;
    }
}
// [MutexDestroy_TC_02_002_008]

/**
 * @page Performance_TEST OSAL Resource Performance Test
 * @section OsalResourcePerformance_CondDestroy_TC_02_008_008 OsalResourcePerformance.CondDestroy_TC_02_008_008
 * @subsection CondDestroy_TC_02_008_008_tsi Test Suite ID
 * OsalResourcePerformance
 * @subsection CondDestroy_TC_02_008_008_tci Test Case ID
 * CondDestroy_S1_N_TC_02_008_008
 * @subsection CondDestroy_TC_02_008_008_rr Related Requirement(s)
 * \#199158, \#199159
 * @subsection CondDestroy_TC_02_008_008_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection CondDestroy_TC_02_008_008_tcs Test Case Summary
 * - Test02_008_008: This test shall verify the performance of condition variable creation/destroying through its execution time and memory consumption
 * @subsection CondDestroy_TC_02_008_008_tpts Test Procedure/Test Steps
 * -# Use a for loop with loop counter is 100 with the sequence call as below:
 *      - Get the total allocated memory before execution time measurement start
 *      - Get the start timestamp by using R_OSAL_ClockTimeGetTimeStamp
 *      - Create 500 new condition variables by using R_OSAL_ThsyncCondCreate
 *      - Destroy 500 created condition variables by using R_OSAL_ThsyncCondDestroy
 *      - Get the end timestamp by using R_OSAL_ClockTimeGetTimeStamp
 *      - Calculate the execution time by using R_OSAL_ClockTimeCalculateTimeDifference
 *      - Get the total allocated memory after execution time measurement end
 *      - Calculate the gap value of execution time/allocated memory of current loop with the previous loop (only in last 10 loop times).<br>
 *        Store these gap values in a gap list.
 * @subsection CondDestroy_TC_02_008_008_ereb Expected Result/Behavior
 * -# This is the expectation in for loop:
 *      - Get the total allocated memory successful
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK
 *      - R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK
 *      - R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK
 *      - R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK
 *      - Get the total allocated memory successful
 *      - All elements in gap list must not greater than 0.
 * @subsection CondDestroy_TC_02_008_008_testcase Test Case Example
 * @snippet test_resource_measurement.cpp CondDestroy_TC_02_008_008
 */
// [CondDestroy_TC_02_008_008]
TEST_F(OsalResourcePerformance, CondDestroy_TC_02_008_008)
{
    auto run = [this]() -> osal_nano_sec_t {
        st_osal_time_t  start_time;
        st_osal_time_t  end_time;
        osal_nano_sec_t exec_time;

        /* get starting timestamp */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &start_time));

        for (int i = 0; i < NO_OF_CONDS; i++)
        {
            EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ThsyncCondCreate(TEST_COND_ID1 + i, &m_cond_hdnl_list[i]));
        }

        for (int i = 0; i < NO_OF_CONDS; i++)
        {
            EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ThsyncCondDestroy(m_cond_hdnl_list[i]));
        }

        /* get end timestamp */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &end_time));

        /* calculate diff */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeCalculateTimeDifference(&end_time, &start_time, &exec_time));

        return exec_time;
    };

    vector<osal_nano_sec_t> exec_time_diff_list;
    size_t                 max_exec_time_exceeded_cnt = 0;
    /* fill the list with NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME elements with minimal values for osal_nano_sec_t */
    exec_time_diff_list.resize(NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, INT64_MIN);

#ifdef HAVE_MALLINFO
    vector<int64_t> heap_usage_diff_list;
    heap_usage_diff_list.resize(NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, INT64_MIN);
#endif // HAVE_MALLINFO

    osal_nano_sec_t last_exec_time;
    for (size_t j = 0; j < NO_OF_MAX_RUNS; j++)
    {
#ifdef HAVE_MALLINFO
        int64_t heap_usage_before = getTotalAllocatedSpace();
#endif //#ifdef HAVE_MALLINFO
        /* execute run */
        osal_nano_sec_t exec_time = run();
        printf("%d/%d\n", j + 1, NO_OF_MAX_RUNS);
#ifdef HAVE_MALLINFO
        int64_t heap_usage_after = getTotalAllocatedSpace();
#endif //#ifdef HAVE_MALLINFO

        // At the beginning of measurement, measured value is not stable. Hence, the latest NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME times is used.
        // The increase of memory consumption and execution time will saturate after many times(NO_OF_MAX_RUNS(100) times may be enough) of executions.
        // If those are not saturated, there is memory leaking or performance degradation caused by wrapper implementation.
        if(j > NO_OF_MAX_RUNS - NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME)
        {
            /* kind of ring buffer for exec_time_dif_list */
            exec_time_diff_list.erase(exec_time_diff_list.begin());                           /* remove first element */
            exec_time_diff_list.push_back(exec_time - last_exec_time); /* add new at the end */

            /* if we measure a raise in execution for a NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, the next line while fail*/
            ASSERT_THAT(exec_time_diff_list, Not(Each(Ge(0))))
                << "We measured for " << NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME
                << " consecutive runs an increase in execution time --> Performance degradation over time detected";

#ifdef HAVE_MALLINFO
        /* kind of ring buffer for heap_usage_diff_list */
        heap_usage_diff_list.erase(heap_usage_diff_list.begin());                                     /* remove first element */
        heap_usage_diff_list.push_back(heap_usage_after - heap_usage_before); /* add new at the end */
        /* if we measure a raise in heap usage for a NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, the next line while
         * fail*/
        ASSERT_THAT(heap_usage_diff_list, Not(Each(Ge(0))))
            << "We measured for " << NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME
            << " consecutive runs an increase in heap usage --> 'Memory Leak' detected";
#endif //#ifdef HAVE_MALLINFO
        }

        last_exec_time = exec_time;
    }
}
// [CondDestroy_TC_02_008_008]

/**
 * @page Performance_TEST OSAL Resource Performance Test
 * @section OsalResourcePerformance_MqDelete_TC_03_002_010 OsalResourcePerformance.MqDelete_TC_03_002_010
 * @subsection MqDelete_TC_03_002_010_tsi Test Suite ID
 * OsalResourcePerformance
 * @subsection MqDelete_TC_03_002_010_tci Test Case ID
 * MqDelete_S1_N_TC_03_002_010
 * @subsection MqDelete_TC_03_002_010_rr Related Requirement(s)
 * \#199170, \#199177
 * @subsection MqDelete_TC_03_002_010_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqDelete_TC_03_002_010_tcs Test Case Summary
 * - Test03_002_010: This test shall verify the performance of message queue creation/destroying through its execution time and memory consumption
 * @subsection MqDelete_TC_03_002_010_tpts Test Procedure/Test Steps
 * -# Use a for loop with loop counter is 100 with the sequence call as below:
 *      - Get the total allocated memory before execution time measurement start
 *      - Get the start timestamp by using R_OSAL_ClockTimeGetTimeStamp
 *      - Create 500 new message queues by using R_OSAL_MqCreate
 *      - Delete 500 created message queues by using R_OSAL_MqDelete
 *      - Get the end timestamp by using R_OSAL_ClockTimeGetTimeStamp
 *      - Calculate the execution time by using R_OSAL_ClockTimeCalculateTimeDifference
 *      - Get the total allocated memory after execution time measurement end
 *      - Calculate the gap value of execution time/allocated memory of current loop with the previous loop (only in last 10 loop times).<br>
 *        Store these gap values in a gap list.
 * @subsection MqDelete_TC_03_002_010_ereb Expected Result/Behavior
 * -# This is the expectation in for loop:
 *      - Get the total allocated memory successful
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK
 *      - R_OSAL_MqCreate return OSAL_RETURN_OK
 *      - R_OSAL_MqDelete return OSAL_RETURN_OK
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK
 *      - R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK
 *      - Get the total allocated memory successful
 *      - All elements in gap list must not greater than 0.
 * @subsection MqDelete_TC_03_002_010_testcase Test Case Example
 * @snippet test_resource_measurement.cpp MqDelete_TC_03_002_010
 */
// [MqDelete_TC_03_002_010]
TEST_F(OsalResourcePerformance, MqDelete_TC_03_002_010)
{
    st_osal_mq_config_t mq_config;
    mq_config.max_num_msg = 1;
    mq_config.msg_size = 4;
    auto run = [this, &mq_config]() -> osal_nano_sec_t {
        st_osal_time_t  start_time;
        st_osal_time_t  end_time;
        osal_nano_sec_t exec_time;

        /* get starting timestamp */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &start_time));

        for (int i = 0; i < NO_OF_MQS; i++)
        {
            EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_MqCreate(&mq_config, TEST_MQ_ID1 + i, &m_mq_hdnl_list[i]));
        }

        for (int i = 0; i < NO_OF_MQS; i++)
        {
            EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_MqDelete(m_mq_hdnl_list[i]));
        }

        /* get end timestamp */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &end_time));

        /* calculate diff */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeCalculateTimeDifference(&end_time, &start_time, &exec_time));

        return exec_time;
    };

    vector<osal_nano_sec_t> exec_time_diff_list;
    size_t                 max_exec_time_exceeded_cnt = 0;
    /* fill the list with NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME elements with minimal values for osal_nano_sec_t */
    exec_time_diff_list.resize(NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, INT64_MIN);

#ifdef HAVE_MALLINFO
    vector<int64_t> heap_usage_diff_list;
    heap_usage_diff_list.resize(NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, INT64_MIN);
#endif // HAVE_MALLINFO

    osal_nano_sec_t last_exec_time;
    for (size_t j = 0; j < NO_OF_MAX_RUNS; j++)
    {
#ifdef HAVE_MALLINFO
        int64_t heap_usage_before = getTotalAllocatedSpace();
#endif //#ifdef HAVE_MALLINFO
        /* execute run */
        osal_nano_sec_t exec_time = run();
        printf("%d/%d\n", j + 1, NO_OF_MAX_RUNS);
#ifdef HAVE_MALLINFO
        int64_t heap_usage_after = getTotalAllocatedSpace();
#endif //#ifdef HAVE_MALLINFO

        // At the beginning of measurement, measured value is not stable. Hence, the latest NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME times is used.
        // The increase of memory consumption and execution time will saturate after many times(NO_OF_MAX_RUNS(100) times may be enough) of executions.
        // If those are not saturated, there is memory leaking or performance degradation caused by wrapper implementation.
        if(j > NO_OF_MAX_RUNS - NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME)
        {
            /* kind of ring buffer for exec_time_dif_list */
            exec_time_diff_list.erase(exec_time_diff_list.begin());                           /* remove first element */
            exec_time_diff_list.push_back(exec_time - last_exec_time); /* add new at the end */

            /* if we measure a raise in execution for a NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, the next line while fail*/
            ASSERT_THAT(exec_time_diff_list, Not(Each(Ge(0))))
                << "We measured for " << NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME
                << " consecutive runs an increase in execution time --> Performance degradation over time detected";

#ifdef HAVE_MALLINFO
        /* kind of ring buffer for heap_usage_diff_list */
        heap_usage_diff_list.erase(heap_usage_diff_list.begin());                                     /* remove first element */
        heap_usage_diff_list.push_back(heap_usage_after - heap_usage_before); /* add new at the end */
        /* if we measure a raise in heap usage for a NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, the next line while
         * fail*/
        ASSERT_THAT(heap_usage_diff_list, Not(Each(Ge(0))))
            << "We measured for " << NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME
            << " consecutive runs an increase in heap usage --> 'Memory Leak' detected";
#endif //#ifdef HAVE_MALLINFO
        }

        last_exec_time = exec_time;
    }
}
// [MqDelete_TC_03_002_010]

/**
 * @page Performance_TEST OSAL Resource Performance Test
 * @section OsalResourcePerformance_IoDeviceClose_TC_06_004_007 OsalResourcePerformance.IoDeviceClose_TC_06_004_007
 * @subsection IoDeviceClose_TC_06_004_007_tsi Test Suite ID
 * OsalResourcePerformance
 * @subsection IoDeviceClose_TC_06_004_007_tci Test Case ID
 * IoDeviceClose_S1_N_TC_06_004_007
 * @subsection IoDeviceClose_TC_06_004_007_rr Related Requirement(s)
 * \#199212
 * @subsection IoDeviceClose_TC_06_004_007_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection IoDeviceClose_TC_06_004_007_tcs Test Case Summary
 * - Test06_004_007: This test shall verify the performance of message queue creation/destroying through its execution time and memory consumption
 * @subsection IoDeviceClose_TC_06_004_007_tpts Test Procedure/Test Steps
 * -# Use a for loop with loop counter is 100 with the sequence call as below:
 *      - Get the total allocated memory before execution time measurement start
 *      - Get the start timestamp by using R_OSAL_ClockTimeGetTimeStamp
 *      - Open and Close 500 device handle of test target device<br>
 *         by using R_OSAL_IoDeviceOpen for test target device, by using R_OSAL_IoDeviceClose
 *      - Get the end timestamp by using R_OSAL_ClockTimeGetTimeStamp
 *      - Calculate the execution time by using R_OSAL_ClockTimeCalculateTimeDifference
 *      - Get the total allocated memory after execution time measurement end
 *      - Calculate the gap value of execution time/allocated memory of current loop with the previous loop (only in last 10 loop times).<br>
 *        Store these gap values in a gap list.
 * @subsection IoDeviceClose_TC_06_004_007_ereb Expected Result/Behavior
 * -# This is the expectation in for loop:
 *      - Get the total allocated memory successful
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK
 *      - R_OSAL_IoDeviceOpen return OSAL_RETURN_OK
 *      - R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK
 *      - R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK
 *      - Get the total allocated memory successful
 *      - All elements in gap list must not greater than 0.
 * @subsection IoDeviceClose_TC_06_004_007_testcase Test Case Example
 * @snippet test_resource_measurement.cpp IoDeviceClose_TC_06_004_007
 */
// [IoDeviceClose_TC_06_004_007]
TEST_F(OsalResourcePerformance, IoDeviceClose_TC_06_004_007)
{
    auto run = [this]() -> osal_nano_sec_t {
        st_osal_mmngr_config_t             config;
        config.memory_limit                = 10 * 1024 * 1024;
        config.max_allowed_allocations     = 2;
        config.max_registered_monitors_cbs = 2;
        config.mode = OSAL_MMNGR_ALLOC_MODE_STACK;
        st_osal_time_t  start_time;
        st_osal_time_t  end_time;
        osal_nano_sec_t exec_time;

        /* get starting timestamp */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &start_time));

        for (int i = 0; i < NO_OF_IO; i++)
        {
            EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_IoDeviceOpen(TEST_PERFORMANCE_DEVICE_1, &m_device_hdnl));
            EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_IoDeviceClose(m_device_hdnl));
        }

        /* get end timestamp */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &end_time));

        /* calculate diff */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeCalculateTimeDifference(&end_time, &start_time, &exec_time));

        return exec_time;
    };

    vector<osal_nano_sec_t> exec_time_diff_list;
    size_t                 max_exec_time_exceeded_cnt = 0;
    /* fill the list with NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME elements with minimal values for osal_nano_sec_t */
    exec_time_diff_list.resize(NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, INT64_MIN);

#ifdef HAVE_MALLINFO
    vector<int64_t> heap_usage_diff_list;
    heap_usage_diff_list.resize(NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, INT64_MIN);
#endif // HAVE_MALLINFO

    osal_nano_sec_t last_exec_time;
    for (size_t j = 0; j < NO_OF_MAX_RUNS; j++)
    {
#ifdef HAVE_MALLINFO
        int64_t heap_usage_before = getTotalAllocatedSpace();
#endif //#ifdef HAVE_MALLINFO
        /* execute run */
        osal_nano_sec_t exec_time = run();
        printf("%d/%d\n", j + 1, NO_OF_MAX_RUNS);
#ifdef HAVE_MALLINFO
        int64_t heap_usage_after = getTotalAllocatedSpace();
#endif //#ifdef HAVE_MALLINFO

        // At the beginning of measurement, measured value is not stable. Hence, the latest NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME times is used.
        // The increase of memory consumption and execution time will saturate after many times(NO_OF_MAX_RUNS(100) times may be enough) of executions.
        // If those are not saturated, there is memory leaking or performance degradation caused by wrapper implementation.
        if(j > NO_OF_MAX_RUNS - NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME)
        {
            /* kind of ring buffer for exec_time_dif_list */
            exec_time_diff_list.erase(exec_time_diff_list.begin());                           /* remove first element */
            exec_time_diff_list.push_back(exec_time - last_exec_time); /* add new at the end */

            /* if we measure a raise in execution for a NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, the next line while fail*/
            ASSERT_THAT(exec_time_diff_list, Not(Each(Ge(0))))
                << "We measured for " << NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME
                << " consecutive runs an increase in execution time --> Performance degradation over time detected";

#ifdef HAVE_MALLINFO
        /* kind of ring buffer for heap_usage_diff_list */
        heap_usage_diff_list.erase(heap_usage_diff_list.begin());                                     /* remove first element */
        heap_usage_diff_list.push_back(heap_usage_after - heap_usage_before); /* add new at the end */
        /* if we measure a raise in heap usage for a NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, the next line while
         * fail*/
        ASSERT_THAT(heap_usage_diff_list, Not(Each(Ge(0))))
            << "We measured for " << NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME
            << " consecutive runs an increase in heap usage --> 'Memory Leak' detected";
#endif //#ifdef HAVE_MALLINFO
        }
        last_exec_time = exec_time;
    }
}
// [IoDeviceClose_TC_06_004_007]

/**
 * @page Performance_TEST OSAL Resource Performance Test
 * @section OsalResourcePerformance_MemoryClose_TC_09_002_015 OsalResourcePerformance.MemoryClose_TC_09_002_015
 * @subsection MemoryClose_TC_09_002_015_tsi Test Suite ID
 * OsalResourcePerformance
 * @subsection MemoryClose_TC_09_002_015_tci Test Case ID
 * MemoryClose_S1_N_TC_09_002_015
 * @subsection MemoryClose_TC_09_002_015_rr Related Requirement(s)
 * \#276732, \#276408, \#276409
 * @subsection MemoryClose_TC_09_002_015_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MemoryClose_TC_09_002_015_tcs Test Case Summary
 * - Test09_002_015: This test shall verify the performance of message queue creation/destroying through its execution time and memory consumption
 * @subsection MemoryClose_TC_09_002_015_tpts Test Procedure/Test Steps
 * -# Use a for loop with loop counter is 100 with the sequence call as below:
 *      - Get the total allocated memory before execution time measurement start
 *      - Get the start timestamp by using R_OSAL_ClockTimeGetTimeStamp
 *      - Open and Close 500 new Memory object<br>
 *           - by using R_OSAL_MmngrOpen<br>
 *             config.mode is OSAL_MMNGR_ALLOC_MODE_STACK<br>
 *             config.memory_limit is 10MByte<br>
 *           - by using R_OSAL_MmngrClose
 *      - Get the end timestamp by using R_OSAL_ClockTimeGetTimeStamp
 *      - Calculate the execution time by using R_OSAL_ClockTimeCalculateTimeDifference
 *      - Get the total allocated memory after execution time measurement end
 *      - Calculate the gap value of execution time/allocated memory of current loop with the previous loop (only in last 10 loop times).<br>
 *        Store these gap values in a gap list.
 * @subsection MemoryClose_TC_09_002_015_ereb Expected Result/Behavior
 * -# This is the expectation in for loop:
 *      - Get the total allocated memory successful
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK
 *      - R_OSAL_MmngrOpen return OSAL_RETURN_OK
 *      - R_OSAL_MmngrClose return OSAL_RETURN_OK
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK
 *      - R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK
 *      - Get the total allocated memory successful
 *      - All elements in gap list must not greater than 0.
 * @subsection MemoryClose_TC_09_002_015_testcase Test Case Example
 * @snippet test_resource_measurement.cpp MemoryClose_TC_09_002_015
 */
// [MemoryClose_TC_09_002_015]
TEST_F(OsalResourcePerformance, MemoryClose_TC_09_002_015)
{
    auto run = [this]() -> osal_nano_sec_t {
        st_osal_mmngr_config_t             config;
        config.memory_limit                = 10 * 1024 * 1024;
        config.max_allowed_allocations     = 2;
        config.max_registered_monitors_cbs = 2;
        config.mode = OSAL_MMNGR_ALLOC_MODE_STACK;
        st_osal_time_t  start_time;
        st_osal_time_t  end_time;
        osal_nano_sec_t exec_time;

        /* get starting timestamp */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &start_time));

        for (int i = 0; i < NO_OF_MM_OC; i++)
        {
            EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_MmngrOpen(&config, &m_mm_hdnl));
            EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_MmngrClose(m_mm_hdnl));
        }

        /* get end timestamp */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &end_time));

        /* calculate diff */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeCalculateTimeDifference(&end_time, &start_time, &exec_time));

        return exec_time;
    };

    vector<osal_nano_sec_t> exec_time_diff_list;
    size_t                 max_exec_time_exceeded_cnt = 0;
    /* fill the list with NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME elements with minimal values for osal_nano_sec_t */
    exec_time_diff_list.resize(NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, INT64_MIN);

#ifdef HAVE_MALLINFO
    vector<int64_t> heap_usage_diff_list;
    heap_usage_diff_list.resize(NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, INT64_MIN);
#endif // HAVE_MALLINFO

    osal_nano_sec_t last_exec_time;
    for (size_t j = 0; j < NO_OF_MAX_RUNS; j++)
    {
#ifdef HAVE_MALLINFO
        int64_t heap_usage_before = getTotalAllocatedSpace();
#endif //#ifdef HAVE_MALLINFO
        /* execute run */
        osal_nano_sec_t exec_time = run();
        printf("%d/%d\n", j + 1, NO_OF_MAX_RUNS);
#ifdef HAVE_MALLINFO
        int64_t heap_usage_after = getTotalAllocatedSpace();
#endif //#ifdef HAVE_MALLINFO

        // At the beginning of measurement, measured value is not stable. Hence, the latest NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME times is used.
        // The increase of memory consumption and execution time will saturate after many times(NO_OF_MAX_RUNS(100) times may be enough) of executions.
        // If those are not saturated, there is memory leaking or performance degradation caused by wrapper implementation.
        if(j > NO_OF_MAX_RUNS - NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME)
        {
            /* kind of ring buffer for exec_time_dif_list */
            exec_time_diff_list.erase(exec_time_diff_list.begin());                           /* remove first element */
            exec_time_diff_list.push_back(exec_time - last_exec_time); /* add new at the end */

            /* if we measure a raise in execution for a NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, the next line while fail*/
            ASSERT_THAT(exec_time_diff_list, Not(Each(Ge(0))))
                << "We measured for " << NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME
                << " consecutive runs an increase in execution time --> Performance degradation over time detected";

#ifdef HAVE_MALLINFO
        /* kind of ring buffer for heap_usage_diff_list */
        heap_usage_diff_list.erase(heap_usage_diff_list.begin());                                     /* remove first element */
        heap_usage_diff_list.push_back(heap_usage_after - heap_usage_before); /* add new at the end */
        /* if we measure a raise in heap usage for a NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, the next line while
         * fail*/
        ASSERT_THAT(heap_usage_diff_list, Not(Each(Ge(0))))
            << "We measured for " << NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME
            << " consecutive runs an increase in heap usage --> 'Memory Leak' detected";
#endif //#ifdef HAVE_MALLINFO
        }
        last_exec_time = exec_time;
    }
}
// [MemoryClose_TC_09_002_015]

/**
 * @page Performance_TEST OSAL Resource Performance Test
 * @section OsalResourcePerformance_MemoryClose_TC_09_002_016 OsalResourcePerformance.MemoryClose_TC_09_002_016
 * @subsection MemoryClose_TC_09_002_016_tsi Test Suite ID
 * OsalResourcePerformance
 * @subsection MemoryClose_TC_09_002_016_tci Test Case ID
 * MemoryClose_S1_N_TC_09_002_016
 * @subsection MemoryClose_TC_09_002_016_rr Related Requirement(s)
 * \#276732, \#276408, \#276409
 * @subsection MemoryClose_TC_09_002_016_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MemoryClose_TC_09_002_016_tcs Test Case Summary
 * - Test09_002_016: This test shall verify the performance of message queue creation/destroying through its execution time and memory consumption
 * @subsection MemoryClose_TC_09_002_016_tpts Test Procedure/Test Steps
 * -# Use a for loop with loop counter is 100 with the sequence call as below:
 *      - Get the total allocated memory before execution time measurement start
 *      - Get the start timestamp by using R_OSAL_ClockTimeGetTimeStamp
 *      - Open and Close 500 new Memory object<br>
 *           - by using R_OSAL_MmngrOpen<br>
 *             config.mode is OSAL_MMNGR_ALLOC_MODE_STACK_PROT<br>
 *             config.memory_limit is 10MByte<br>
 *           - by using R_OSAL_MmngrClose
 *      - Get the end timestamp by using R_OSAL_ClockTimeGetTimeStamp
 *      - Calculate the execution time by using R_OSAL_ClockTimeCalculateTimeDifference
 *      - Get the total allocated memory after execution time measurement end
 *      - Calculate the gap value of execution time/allocated memory of current loop with the previous loop (only in last 10 loop times).<br>
 *        Store these gap values in a gap list.
 * @subsection MemoryClose_TC_09_002_016_ereb Expected Result/Behavior
 * -# This is the expectation in for loop:
 *      - Get the total allocated memory successful
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK
 *      - R_OSAL_MmngrOpen return OSAL_RETURN_OK
 *      - R_OSAL_MmngrClose return OSAL_RETURN_OK
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK
 *      - R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK
 *      - Get the total allocated memory successful
 *      - All elements in gap list must not greater than 0.
 * @subsection MemoryClose_TC_09_002_016_testcase Test Case Example
 * @snippet test_resource_measurement.cpp MemoryClose_TC_09_002_016
 */
// [MemoryClose_TC_09_002_016]
TEST_F(OsalResourcePerformance, MemoryClose_TC_09_002_016)
{
    auto run = [this]() -> osal_nano_sec_t {
        st_osal_mmngr_config_t             config;
        config.memory_limit                = 10 * 1024 * 1024;
        config.max_allowed_allocations     = 2;
        config.max_registered_monitors_cbs = 2;
        config.mode = OSAL_MMNGR_ALLOC_MODE_STACK_PROT;
        st_osal_time_t  start_time;
        st_osal_time_t  end_time;
        osal_nano_sec_t exec_time;

        /* get starting timestamp */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &start_time));

        for (int i = 0; i < NO_OF_MM_OC; i++)
        {
            EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_MmngrOpen(&config, &m_mm_hdnl));
            EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_MmngrClose(m_mm_hdnl));
        }

        /* get end timestamp */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &end_time));

        /* calculate diff */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeCalculateTimeDifference(&end_time, &start_time, &exec_time));

        return exec_time;
    };

    vector<osal_nano_sec_t> exec_time_diff_list;
    size_t                 max_exec_time_exceeded_cnt = 0;
    /* fill the list with NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME elements with minimal values for osal_nano_sec_t */
    exec_time_diff_list.resize(NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, INT64_MIN);

#ifdef HAVE_MALLINFO
    vector<int64_t> heap_usage_diff_list;
    heap_usage_diff_list.resize(NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, INT64_MIN);
#endif // HAVE_MALLINFO

    osal_nano_sec_t last_exec_time;
    for (size_t j = 0; j < NO_OF_MAX_RUNS; j++)
    {
#ifdef HAVE_MALLINFO
        int64_t heap_usage_before = getTotalAllocatedSpace();
#endif //#ifdef HAVE_MALLINFO
        /* execute run */
        osal_nano_sec_t exec_time = run();
        printf("%d/%d\n", j + 1, NO_OF_MAX_RUNS);
#ifdef HAVE_MALLINFO
        int64_t heap_usage_after = getTotalAllocatedSpace();
#endif //#ifdef HAVE_MALLINFO

        // At the beginning of measurement, measured value is not stable. Hence, the latest NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME times is used.
        // The increase of memory consumption and execution time will saturate after many times(NO_OF_MAX_RUNS(100) times may be enough) of executions.
        // If those are not saturated, there is memory leaking or performance degradation caused by wrapper implementation.
        if(j > NO_OF_MAX_RUNS - NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME)
        {
            /* kind of ring buffer for exec_time_dif_list */
            exec_time_diff_list.erase(exec_time_diff_list.begin());                           /* remove first element */
            exec_time_diff_list.push_back(exec_time - last_exec_time); /* add new at the end */

            /* if we measure a raise in execution for a NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, the next line while fail*/
            ASSERT_THAT(exec_time_diff_list, Not(Each(Ge(0))))
                << "We measured for " << NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME
                << " consecutive runs an increase in execution time --> Performance degradation over time detected";

#ifdef HAVE_MALLINFO
        /* kind of ring buffer for heap_usage_diff_list */
        heap_usage_diff_list.erase(heap_usage_diff_list.begin());                                     /* remove first element */
        heap_usage_diff_list.push_back(heap_usage_after - heap_usage_before); /* add new at the end */
        /* if we measure a raise in heap usage for a NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, the next line while
         * fail*/
        ASSERT_THAT(heap_usage_diff_list, Not(Each(Ge(0))))
            << "We measured for " << NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME
            << " consecutive runs an increase in heap usage --> 'Memory Leak' detected";
#endif //#ifdef HAVE_MALLINFO
        }
        last_exec_time = exec_time;
    }
}
// [MemoryClose_TC_09_002_016]

/**
 * @page Performance_TEST OSAL Resource Performance Test
 * @section OsalResourcePerformance_MemoryClose_TC_09_002_017 OsalResourcePerformance.MemoryClose_TC_09_002_017
 * @subsection MemoryClose_TC_09_002_017_tsi Test Suite ID
 * OsalResourcePerformance
 * @subsection MemoryClose_TC_09_002_017_tci Test Case ID
 * MemoryClose_S1_N_TC_09_002_017
 * @subsection MemoryClose_TC_09_002_017_rr Related Requirement(s)
 * \#276732, \#276408, \#276409
 * @subsection MemoryClose_TC_09_002_017_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MemoryClose_TC_09_002_017_tcs Test Case Summary
 * - Test09_002_017: This test shall verify the performance of message queue creation/destroying through its execution time and memory consumption
 * @subsection MemoryClose_TC_09_002_017_tpts Test Procedure/Test Steps
 * -# Use a for loop with loop counter is 100 with the sequence call as below:
 *      - Get the total allocated memory before execution time measurement start
 *      - Get the start timestamp by using R_OSAL_ClockTimeGetTimeStamp
 *      - Open and Close 500 new Memory object<br>
 *           - by using R_OSAL_MmngrOpen<br>
 *             config.mode is OSAL_MMNGR_ALLOC_MODE_FREE_LIST<br>
 *             config.memory_limit is 10MByte<br>
 *           - by using R_OSAL_MmngrClose
 *      - Get the end timestamp by using R_OSAL_ClockTimeGetTimeStamp
 *      - Calculate the execution time by using R_OSAL_ClockTimeCalculateTimeDifference
 *      - Get the total allocated memory after execution time measurement end
 *      - Calculate the gap value of execution time/allocated memory of current loop with the previous loop (only in last 10 loop times).<br>
 *        Store these gap values in a gap list.
 * @subsection MemoryClose_TC_09_002_017_ereb Expected Result/Behavior
 * -# This is the expectation in for loop:
 *      - Get the total allocated memory successful
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK
 *      - R_OSAL_MmngrOpen return OSAL_RETURN_OK
 *      - R_OSAL_MmngrClose return OSAL_RETURN_OK
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK
 *      - R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK
 *      - Get the total allocated memory successful
 *      - All elements in gap list must not greater than 0.
 * @subsection MemoryClose_TC_09_002_017_testcase Test Case Example
 * @snippet test_resource_measurement.cpp MemoryClose_TC_09_002_017
 */
// [MemoryClose_TC_09_002_017]
TEST_F(OsalResourcePerformance, MemoryClose_TC_09_002_017)
{
    auto run = [this]() -> osal_nano_sec_t {
        st_osal_mmngr_config_t             config;
        config.memory_limit                = 10 * 1024 * 1024;
        config.max_allowed_allocations     = 2;
        config.max_registered_monitors_cbs = 2;
        config.mode = OSAL_MMNGR_ALLOC_MODE_FREE_LIST;
        st_osal_time_t  start_time;
        st_osal_time_t  end_time;
        osal_nano_sec_t exec_time;

        /* get starting timestamp */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &start_time));

        for (int i = 0; i < NO_OF_MM_OC; i++)
        {
            EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_MmngrOpen(&config, &m_mm_hdnl));
            EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_MmngrClose(m_mm_hdnl));
        }

        /* get end timestamp */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &end_time));

        /* calculate diff */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeCalculateTimeDifference(&end_time, &start_time, &exec_time));

        return exec_time;
    };

    vector<osal_nano_sec_t> exec_time_diff_list;
    size_t                 max_exec_time_exceeded_cnt = 0;
    /* fill the list with NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME elements with minimal values for osal_nano_sec_t */
    exec_time_diff_list.resize(NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, INT64_MIN);

#ifdef HAVE_MALLINFO
    vector<int64_t> heap_usage_diff_list;
    heap_usage_diff_list.resize(NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, INT64_MIN);
#endif // HAVE_MALLINFO

    osal_nano_sec_t last_exec_time;
    for (size_t j = 0; j < NO_OF_MAX_RUNS; j++)
    {
#ifdef HAVE_MALLINFO
        int64_t heap_usage_before = getTotalAllocatedSpace();
#endif //#ifdef HAVE_MALLINFO
        /* execute run */
        osal_nano_sec_t exec_time = run();
        printf("%d/%d\n", j + 1, NO_OF_MAX_RUNS);
#ifdef HAVE_MALLINFO
        int64_t heap_usage_after = getTotalAllocatedSpace();
#endif //#ifdef HAVE_MALLINFO

        // At the beginning of measurement, measured value is not stable. Hence, the latest NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME times is used.
        // The increase of memory consumption and execution time will saturate after many times(NO_OF_MAX_RUNS(100) times may be enough) of executions.
        // If those are not saturated, there is memory leaking or performance degradation caused by wrapper implementation.
        if(j > NO_OF_MAX_RUNS - NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME)
        {
            /* kind of ring buffer for exec_time_dif_list */
            exec_time_diff_list.erase(exec_time_diff_list.begin());                           /* remove first element */
            exec_time_diff_list.push_back(exec_time - last_exec_time); /* add new at the end */

            /* if we measure a raise in execution for a NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, the next line while fail*/
            ASSERT_THAT(exec_time_diff_list, Not(Each(Ge(0))))
                << "We measured for " << NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME
                << " consecutive runs an increase in execution time --> Performance degradation over time detected";

#ifdef HAVE_MALLINFO
        /* kind of ring buffer for heap_usage_diff_list */
        heap_usage_diff_list.erase(heap_usage_diff_list.begin());                                     /* remove first element */
        heap_usage_diff_list.push_back(heap_usage_after - heap_usage_before); /* add new at the end */
        /* if we measure a raise in heap usage for a NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, the next line while
         * fail*/
        ASSERT_THAT(heap_usage_diff_list, Not(Each(Ge(0))))
            << "We measured for " << NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME
            << " consecutive runs an increase in heap usage --> 'Memory Leak' detected";
#endif //#ifdef HAVE_MALLINFO
        }
        last_exec_time = exec_time;
    }
}
// [MemoryClose_TC_09_002_017]

/**
 * @page Performance_TEST OSAL Resource Performance Test
 * @section OsalResourcePerformance_MemoryClose_TC_09_002_018 OsalResourcePerformance.MemoryClose_TC_09_002_018
 * @subsection MemoryClose_TC_09_002_018_tsi Test Suite ID
 * OsalResourcePerformance
 * @subsection MemoryClose_TC_09_002_018_tci Test Case ID
 * MemoryClose_S1_N_TC_09_002_018
 * @subsection MemoryClose_TC_09_002_018_rr Related Requirement(s)
 * \#276732, \#276410, \#276409
 * @subsection MemoryClose_TC_09_002_018_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MemoryClose_TC_09_002_018_tcs Test Case Summary
 * - Test09_002_018: This test shall verify the performance of message queue creation/destroying through its execution time and memory consumption
 * @subsection MemoryClose_TC_09_002_018_tpts Test Procedure/Test Steps
 * -# Use a for loop with loop counter is 100 with the sequence call as below:
 *      - Get the total allocated memory before execution time measurement start
 *      - Get the start timestamp by using R_OSAL_ClockTimeGetTimeStamp
 *      - Open and Close 500 new Memory object<br>
 *           - by using R_OSAL_MmngrOpen<br>
 *             config.mode is OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT<br>
 *             config.memory_limit is 10MByte<br>
 *           - by using R_OSAL_MmngrClose
 *      - Get the end timestamp by using R_OSAL_ClockTimeGetTimeStamp
 *      - Calculate the execution time by using R_OSAL_ClockTimeCalculateTimeDifference
 *      - Get the total allocated memory after execution time measurement end
 *      - Calculate the gap value of execution time/allocated memory of current loop with the previous loop (only in last 10 loop times).<br>
 *        Store these gap values in a gap list.
 * @subsection MemoryClose_TC_09_002_018_ereb Expected Result/Behavior
 * -# This is the expectation in for loop:
 *      - Get the total allocated memory successful
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK
 *      - R_OSAL_MmngrOpen return OSAL_RETURN_OK
 *      - R_OSAL_MmngrClose return OSAL_RETURN_OK
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK
 *      - R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK
 *      - Get the total allocated memory successful
 *      - All elements in gap list must not greater than 0.
 * @subsection MemoryClose_TC_09_002_018_testcase Test Case Example
 * @snippet test_resource_measurement.cpp MemoryClose_TC_09_002_018
 */
// [MemoryClose_TC_09_002_018]
TEST_F(OsalResourcePerformance, MemoryClose_TC_09_002_018)
{
    auto run = [this]() -> osal_nano_sec_t {
        st_osal_mmngr_config_t             config;
        config.memory_limit                = 10 * 1024 * 1024;
        config.max_allowed_allocations     = 2;
        config.max_registered_monitors_cbs = 2;
        config.mode = OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT;
        st_osal_time_t  start_time;
        st_osal_time_t  end_time;
        osal_nano_sec_t exec_time;

        /* get starting timestamp */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &start_time));

        for (int i = 0; i < NO_OF_MM_OC; i++)
        {
            EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_MmngrOpen(&config, &m_mm_hdnl));
            EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_MmngrClose(m_mm_hdnl));
        }

        /* get end timestamp */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &end_time));

        /* calculate diff */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeCalculateTimeDifference(&end_time, &start_time, &exec_time));

        return exec_time;
    };

    vector<osal_nano_sec_t> exec_time_diff_list;
    size_t                 max_exec_time_exceeded_cnt = 0;
    /* fill the list with NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME elements with minimal values for osal_nano_sec_t */
    exec_time_diff_list.resize(NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, INT64_MIN);

#ifdef HAVE_MALLINFO
    vector<int64_t> heap_usage_diff_list;
    heap_usage_diff_list.resize(NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, INT64_MIN);
#endif // HAVE_MALLINFO

    osal_nano_sec_t last_exec_time;
    for (size_t j = 0; j < NO_OF_MAX_RUNS; j++)
    {
#ifdef HAVE_MALLINFO
        int64_t heap_usage_before = getTotalAllocatedSpace();
#endif //#ifdef HAVE_MALLINFO
        /* execute run */
        osal_nano_sec_t exec_time = run();
        printf("%d/%d\n", j + 1, NO_OF_MM_ALLOC_DEALLOC_MAX_RUNS);
#ifdef HAVE_MALLINFO
        int64_t heap_usage_after = getTotalAllocatedSpace();
#endif //#ifdef HAVE_MALLINFO

        // At the beginning of measurement, measured value is not stable. Hence, the latest NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME times is used.
        // The increase of memory consumption and execution time will saturate after many times(NO_OF_MAX_RUNS(100) times may be enough) of executions.
        // If those are not saturated, there is memory leaking or performance degradation caused by wrapper implementation.
        if(j > NO_OF_MAX_RUNS - NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME)
        {
            /* kind of ring buffer for exec_time_dif_list */
            exec_time_diff_list.erase(exec_time_diff_list.begin());                           /* remove first element */
            exec_time_diff_list.push_back(exec_time - last_exec_time); /* add new at the end */

            /* if we measure a raise in execution for a NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, the next line while fail*/
            ASSERT_THAT(exec_time_diff_list, Not(Each(Ge(0))))
                << "We measured for " << NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME
                << " consecutive runs an increase in execution time --> Performance degradation over time detected";

#ifdef HAVE_MALLINFO
        /* kind of ring buffer for heap_usage_diff_list */
        heap_usage_diff_list.erase(heap_usage_diff_list.begin());                                     /* remove first element */
        heap_usage_diff_list.push_back(heap_usage_after - heap_usage_before); /* add new at the end */
        /* if we measure a raise in heap usage for a NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, the next line while
         * fail*/
        ASSERT_THAT(heap_usage_diff_list, Not(Each(Ge(0))))
            << "We measured for " << NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME
            << " consecutive runs an increase in heap usage --> 'Memory Leak' detected";
#endif //#ifdef HAVE_MALLINFO
        }
        last_exec_time = exec_time;
    }
}
// [MemoryClose_TC_09_002_018]

/**
 * @page Performance_TEST OSAL Resource Performance Test
 * @section OsalResourcePerformance_MemoryDealloc_TC_09_004_035 OsalResourcePerformance.MemoryDealloc_TC_09_004_035
 * @subsection MemoryDealloc_TC_09_004_035_tsi Test Suite ID
 * OsalResourcePerformance
 * @subsection MemoryDealloc_TC_09_004_035_tci Test Case ID
 * MemoryDealloc_S3_N_TC_09_004_035
 * @subsection MemoryDealloc_TC_09_004_035_rr Related Requirement(s)
 * \#276732, \#276410, \#276412
 * @subsection MemoryDealloc_TC_09_004_035_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MemoryDealloc_TC_09_004_035_tcs Test Case Summary
 * - Test09_004_035: This test shall verify the performance of message queue creation/destroying through its execution time and memory consumption
 * @subsection MemoryDealloc_TC_09_004_035_tpts Test Procedure/Test Steps
 * -# Open new Memory Object by using R_OSAL_MmngrOpen
 *      - config.mode is OSAL_MMNGR_ALLOC_MODE_STACK
 *      - config.memory_limit is 10MByte
 *      - config.max_allowed_allocations is 500
 * -# Use a for loop with loop counter is 500 with the sequence call as below:
 *      - Get the total allocated memory before execution time measurement start
 *      - Get the start timestamp by using R_OSAL_ClockTimeGetTimeStamp
 *      - Allocate Memory buffer(4096byte) 500times by using R_OSAL_MmngrAlloc
 *      - Deallocate Memory buffer 500times by using R_OSAL_MmngrDealloc
 *      - Get the end timestamp by using R_OSAL_ClockTimeGetTimeStamp
 *      - Calculate the execution time by using R_OSAL_ClockTimeCalculateTimeDifference
 *      - Get the total allocated memory after execution time measurement end
 *      - Calculate the gap value of execution time/allocated memory of current loop with the previous loop (only in last 10 loop times).<br>
 *        Store these gap values in a gap list.
 *      - Close Memory Object by using R_OSAL_MmngrClose
 * @subsection MemoryDealloc_TC_09_004_035_ereb Expected Result/Behavior
 * -# R_OSAL_MmngrOpen return OSAL_RETURN_OK
 * -# This is the expectation in for loop:
 *      - Get the total allocated memory successful
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK
 *      - R_OSAL_MmngrAlloc return OSAL_RETURN_OK
 *      - R_OSAL_MmngrDealloc return OSAL_RETURN_OK
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK
 *      - R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK
 *      - Get the total allocated memory successful
 *      - All elements in gap list must not greater than 0.
 * -# R_OSAL_MmngrClose return OSAL_RETURN_OK
 * @subsection MemoryDealloc_TC_09_004_035_testcase Test Case Example
 * @snippet test_resource_measurement.cpp MemoryDealloc_TC_09_004_035
 */
// [MemoryDealloc_TC_09_004_035]
TEST_F(OsalResourcePerformance, MemoryDealloc_TC_09_004_035)
{
    st_osal_mmngr_config_t             config;
    config.memory_limit                = 10 * 1024 * 1024;
    config.max_allowed_allocations     = NO_OF_MM_ALLOC_DEALLOC;
    config.max_registered_monitors_cbs = NO_OF_MM_ALLOC_DEALLOC;
    config.mode = OSAL_MMNGR_ALLOC_MODE_STACK;
    EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_MmngrOpen(&config, &m_mm_hdnl));

    auto run = [this]() -> osal_nano_sec_t {
        size_t size = 4096;
        size_t alignment  = 64;
        st_osal_time_t  start_time;
        st_osal_time_t  end_time;
        osal_nano_sec_t exec_time;

        /* get starting timestamp */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &start_time));

        for (int i = 0; i < NO_OF_MM_ALLOC_DEALLOC; i++)
        {
            EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_MmngrAlloc(m_mm_hdnl, size, alignment, &m_mm_buf_list[i]));
        }
        for (int i = 0; i < NO_OF_MM_ALLOC_DEALLOC; i++)
        {
            EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_MmngrDealloc(m_mm_hdnl, m_mm_buf_list[NO_OF_MM_ALLOC_DEALLOC - i - 1]));
        }
        /* get end timestamp */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &end_time));

        /* calculate diff */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeCalculateTimeDifference(&end_time, &start_time, &exec_time));

        return exec_time;
    };

    vector<osal_nano_sec_t> exec_time_diff_list;
    size_t                 max_exec_time_exceeded_cnt = 0;
    /* fill the list with NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME elements with minimal values for osal_nano_sec_t */
    exec_time_diff_list.resize(NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, INT64_MIN);

#ifdef HAVE_MALLINFO
    vector<int64_t> heap_usage_diff_list;
    heap_usage_diff_list.resize(NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, INT64_MIN);
#endif // HAVE_MALLINFO

    osal_nano_sec_t last_exec_time;
    for (size_t j = 0; j < NO_OF_MM_ALLOC_DEALLOC_MAX_RUNS; j++)
    {
#ifdef HAVE_MALLINFO
        int64_t heap_usage_before = getTotalAllocatedSpace();
#endif //#ifdef HAVE_MALLINFO
        /* execute run */
        osal_nano_sec_t exec_time = run();
        printf("%d/%d\n", j + 1, NO_OF_MM_ALLOC_DEALLOC_MAX_RUNS);
#ifdef HAVE_MALLINFO
        int64_t heap_usage_after = getTotalAllocatedSpace();
#endif //#ifdef HAVE_MALLINFO

        // At the beginning of measurement, measured value is not stable. Hence, the latest NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME times is used.
        // The increase of memory consumption and execution time will saturate after many times(NO_OF_MAX_RUNS(100) times may be enough) of executions.
        // If those are not saturated, there is memory leaking or performance degradation caused by wrapper implementation.
        if(j > NO_OF_MM_ALLOC_DEALLOC_MAX_RUNS - NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME)
        {
            /* kind of ring buffer for exec_time_dif_list */
            exec_time_diff_list.erase(exec_time_diff_list.begin());                           /* remove first element */
            exec_time_diff_list.push_back(exec_time - last_exec_time); /* add new at the end */

            /* if we measure a raise in execution for a NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, the next line while fail*/
            ASSERT_THAT(exec_time_diff_list, Not(Each(Ge(0))))
                << "We measured for " << NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME
                << " consecutive runs an increase in execution time --> Performance degradation over time detected";

#ifdef HAVE_MALLINFO
        /* kind of ring buffer for heap_usage_diff_list */
        heap_usage_diff_list.erase(heap_usage_diff_list.begin());                                     /* remove first element */
        heap_usage_diff_list.push_back(heap_usage_after - heap_usage_before); /* add new at the end */
        /* if we measure a raise in heap usage for a NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, the next line while
         * fail*/
        ASSERT_THAT(heap_usage_diff_list, Not(Each(Ge(0))))
            << "We measured for " << NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME
            << " consecutive runs an increase in heap usage --> 'Memory Leak' detected";
#endif //#ifdef HAVE_MALLINFO
        }
        last_exec_time = exec_time;
    }
    EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_MmngrClose(m_mm_hdnl));
}
// [MemoryDealloc_TC_09_004_035]

/** @page Performance_TEST OSAL Resource Performance Test
 * @section OsalResourcePerformance_MemoryDealloc_TC_09_004_036 OsalResourcePerformance.MemoryDealloc_TC_09_004_036
 * @subsection MemoryDealloc_TC_09_004_036_tsi Test Suite ID
 * OsalResourcePerformance
 * @subsection MemoryDealloc_TC_09_004_036_tci Test Case ID
 * MemoryDealloc_S3_N_TC_09_004_036
 * @subsection MemoryDealloc_TC_09_004_036_rr Related Requirement(s)
 * \#276732, \#276410, \#276412
 * @subsection MemoryDealloc_TC_09_004_036_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MemoryDealloc_TC_09_004_036_tcs Test Case Summary
 * - Test09_004_036: This test shall verify the performance of message queue creation/destroying through its execution time and memory consumption
 * @subsection MemoryDealloc_TC_09_004_036_tpts Test Procedure/Test Steps
 * -# Open new Memory Object by using R_OSAL_MmngrOpen
 *      - config.mode is OSAL_MMNGR_ALLOC_MODE_STACK_PROT
 *      - config.memory_limit is 10MByte
 *      - config.max_allowed_allocations is 500
 * -# Use a for loop with loop counter is 500 with the sequence call as below:
 *      - Get the total allocated memory before execution time measurement start
 *      - Get the start timestamp by using R_OSAL_ClockTimeGetTimeStamp
 *      - Allocate Memory buffer(4096byte) 500times by using R_OSAL_MmngrAlloc
 *      - Deallocate Memory buffer 500times by using R_OSAL_MmngrDealloc
 *      - Get the end timestamp by using R_OSAL_ClockTimeGetTimeStamp
 *      - Calculate the execution time by using R_OSAL_ClockTimeCalculateTimeDifference
 *      - Get the total allocated memory after execution time measurement end
 *      - Calculate the gap value of execution time/allocated memory of current loop with the previous loop (only in last 10 loop times).<br>
 *        Store these gap values in a gap list.
 *      - Close Memory Object by using R_OSAL_MmngrClose
 * @subsection MemoryDealloc_TC_09_004_036_ereb Expected Result/Behavior
 * -# R_OSAL_MmngrOpen return OSAL_RETURN_OK
 * -# This is the expectation in for loop:
 *      - Get the total allocated memory successful
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK
 *      - R_OSAL_MmngrAlloc return OSAL_RETURN_OK
 *      - R_OSAL_MmngrDealloc return OSAL_RETURN_OK
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK
 *      - R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK
 *      - Get the total allocated memory successful
 *      - All elements in gap list must not greater than 0.
 * -# R_OSAL_MmngrClose return OSAL_RETURN_OK
 * @subsection MemoryDealloc_TC_09_004_036_testcase Test Case Example
 * @snippet test_resource_measurement.cpp MemoryDealloc_TC_09_004_036
 */
// [MemoryDealloc_TC_09_004_036]
TEST_F(OsalResourcePerformance, MemoryDealloc_TC_09_004_036)
{
    st_osal_mmngr_config_t             config;
    config.memory_limit                = 10 * 1024 * 1024;
    config.max_allowed_allocations     = NO_OF_MM_ALLOC_DEALLOC;
    config.max_registered_monitors_cbs = NO_OF_MM_ALLOC_DEALLOC;
    config.mode = OSAL_MMNGR_ALLOC_MODE_STACK_PROT;
    EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_MmngrOpen(&config, &m_mm_hdnl));

    auto run = [this]() -> osal_nano_sec_t {

        size_t size = 4096;
        size_t alignment  = 64;
        st_osal_time_t  start_time;
        st_osal_time_t  end_time;
        osal_nano_sec_t exec_time;

        /* get starting timestamp */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &start_time));

        for (int i = 0; i < NO_OF_MM_ALLOC_DEALLOC; i++)
        {
            EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_MmngrAlloc(m_mm_hdnl, size, alignment, &m_mm_buf_list[i]));
        }
        for (int i = 0; i < NO_OF_MM_ALLOC_DEALLOC; i++)
        {
            EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_MmngrDealloc(m_mm_hdnl, m_mm_buf_list[NO_OF_MM_ALLOC_DEALLOC - i - 1]));
        }
        /* get end timestamp */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &end_time));

        /* calculate diff */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeCalculateTimeDifference(&end_time, &start_time, &exec_time));

        return exec_time;
    };

    vector<osal_nano_sec_t> exec_time_diff_list;
    size_t                 max_exec_time_exceeded_cnt = 0;
    /* fill the list with NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME elements with minimal values for osal_nano_sec_t */
    exec_time_diff_list.resize(NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, INT64_MIN);

#ifdef HAVE_MALLINFO
    vector<int64_t> heap_usage_diff_list;
    heap_usage_diff_list.resize(NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, INT64_MIN);
#endif // HAVE_MALLINFO

    osal_nano_sec_t last_exec_time;
    for (size_t j = 0; j < NO_OF_MM_ALLOC_DEALLOC_MAX_RUNS; j++)
    {
#ifdef HAVE_MALLINFO
        int64_t heap_usage_before = getTotalAllocatedSpace();
#endif //#ifdef HAVE_MALLINFO
        /* execute run */
        osal_nano_sec_t exec_time = run();
        printf("%d/%d\n", j + 1, NO_OF_MM_ALLOC_DEALLOC_MAX_RUNS);
#ifdef HAVE_MALLINFO
        int64_t heap_usage_after = getTotalAllocatedSpace();
#endif //#ifdef HAVE_MALLINFO

        // At the beginning of measurement, measured value is not stable. Hence, the latest NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME times is used.
        // The increase of memory consumption and execution time will saturate after many times(NO_OF_MAX_RUNS(100) times may be enough) of executions.
        // If those are not saturated, there is memory leaking or performance degradation caused by wrapper implementation.
        if(j > NO_OF_MM_ALLOC_DEALLOC_MAX_RUNS - NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME)
        {
            /* kind of ring buffer for exec_time_dif_list */
            exec_time_diff_list.erase(exec_time_diff_list.begin());                           /* remove first element */
            exec_time_diff_list.push_back(exec_time - last_exec_time); /* add new at the end */

            /* if we measure a raise in execution for a NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, the next line while fail*/
            ASSERT_THAT(exec_time_diff_list, Not(Each(Ge(0))))
                << "We measured for " << NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME
                << " consecutive runs an increase in execution time --> Performance degradation over time detected";

#ifdef HAVE_MALLINFO
        /* kind of ring buffer for heap_usage_diff_list */
        heap_usage_diff_list.erase(heap_usage_diff_list.begin());                                     /* remove first element */
        heap_usage_diff_list.push_back(heap_usage_after - heap_usage_before); /* add new at the end */
        /* if we measure a raise in heap usage for a NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, the next line while
         * fail*/
        ASSERT_THAT(heap_usage_diff_list, Not(Each(Ge(0))))
            << "We measured for " << NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME
            << " consecutive runs an increase in heap usage --> 'Memory Leak' detected";
#endif //#ifdef HAVE_MALLINFO
        }
        last_exec_time = exec_time;
    }
    EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_MmngrClose(m_mm_hdnl));
}
// [MemoryDealloc_TC_09_004_036]

/**
 * @page Performance_TEST OSAL Resource Performance Test
 * @section OsalResourcePerformance_MemoryDealloc_TC_09_004_037 OsalResourcePerformance.MemoryDealloc_TC_09_004_037
 * @subsection MemoryDealloc_TC_09_004_037_tsi Test Suite ID
 * OsalResourcePerformance
 * @subsection MemoryDealloc_TC_09_004_037_tci Test Case ID
 * MemoryDealloc_S3_N_TC_09_004_037
 * @subsection MemoryDealloc_TC_09_004_037_rr Related Requirement(s)
 * \#276732, \#276410, \#276412
 * @subsection MemoryDealloc_TC_09_004_037_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MemoryDealloc_TC_09_004_037_tcs Test Case Summary
 * - Test09_004_037: This test shall verify the performance of message queue creation/destroying through its execution time and memory consumption
 * @subsection MemoryDealloc_TC_09_004_037_tpts Test Procedure/Test Steps
 * -# Open new Memory Object by using R_OSAL_MmngrOpen
 *      - config.mode is OSAL_MMNGR_ALLOC_MODE_FREE_LIST
 *      - config.memory_limit is 10MByte
 *      - config.max_allowed_allocations is 500
 * -# Use a for loop with loop counter is 500 with the sequence call as below:
 *      - Get the total allocated memory before execution time measurement start
 *      - Get the start timestamp by using R_OSAL_ClockTimeGetTimeStamp
 *      - Allocate Memory buffer(4096byte) 500times by using R_OSAL_MmngrAlloc
 *      - Deallocate Memory buffer 500times by using R_OSAL_MmngrDealloc
 *      - Get the end timestamp by using R_OSAL_ClockTimeGetTimeStamp
 *      - Calculate the execution time by using R_OSAL_ClockTimeCalculateTimeDifference
 *      - Get the total allocated memory after execution time measurement end
 *      - Calculate the gap value of execution time/allocated memory of current loop with the previous loop (only in last 10 loop times).<br>
 *        Store these gap values in a gap list.
 *      - Close Memory Object by using R_OSAL_MmngrClose
 * @subsection MemoryDealloc_TC_09_004_037_ereb Expected Result/Behavior
 * -# This is the expectation in for loop:
 *      - Get the total allocated memory successful
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK
 *      - R_OSAL_MmngrOpen return OSAL_RETURN_OK
 *      - R_OSAL_MmngrClose return OSAL_RETURN_OK
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK
 *      - R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK
 *      - Get the total allocated memory successful
 *      - All elements in gap list must not greater than 0.
 * @subsection MemoryDealloc_TC_09_004_037_testcase Test Case Example
 * @snippet test_resource_measurement.cpp MemoryDealloc_TC_09_004_037
 */
// [MemoryDealloc_TC_09_004_037]
TEST_F(OsalResourcePerformance, MemoryDealloc_TC_09_004_037)
{
    st_osal_mmngr_config_t             config;
    config.memory_limit                = 10 * 1024 * 1024;
    config.max_allowed_allocations     = NO_OF_MM_ALLOC_DEALLOC;
    config.max_registered_monitors_cbs = NO_OF_MM_ALLOC_DEALLOC;
    config.mode = OSAL_MMNGR_ALLOC_MODE_FREE_LIST;
    EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_MmngrOpen(&config, &m_mm_hdnl));

    auto run = [this]() -> osal_nano_sec_t {
        size_t size = 4096;
        size_t alignment  = 64;
        st_osal_time_t  start_time;
        st_osal_time_t  end_time;
        osal_nano_sec_t exec_time;

        /* get starting timestamp */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &start_time));

        for (int i = 0; i < NO_OF_MM_ALLOC_DEALLOC; i++)
        {
            EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_MmngrAlloc(m_mm_hdnl, size, alignment, &m_mm_buf_list[i]));
        }
        for (int i = 0; i < NO_OF_MM_ALLOC_DEALLOC; i++)
        {
            EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_MmngrDealloc(m_mm_hdnl, m_mm_buf_list[NO_OF_MM_ALLOC_DEALLOC - i - 1]));
        }
        /* get end timestamp */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &end_time));

        /* calculate diff */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeCalculateTimeDifference(&end_time, &start_time, &exec_time));

        return exec_time;
    };

    vector<osal_nano_sec_t> exec_time_diff_list;
    size_t                 max_exec_time_exceeded_cnt = 0;
    /* fill the list with NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME elements with minimal values for osal_nano_sec_t */
    exec_time_diff_list.resize(NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, INT64_MIN);

#ifdef HAVE_MALLINFO
    vector<int64_t> heap_usage_diff_list;
    heap_usage_diff_list.resize(NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, INT64_MIN);
#endif // HAVE_MALLINFO

    osal_nano_sec_t last_exec_time;
    for (size_t j = 0; j < NO_OF_MM_ALLOC_DEALLOC_MAX_RUNS; j++)
    {
#ifdef HAVE_MALLINFO
        int64_t heap_usage_before = getTotalAllocatedSpace();
#endif //#ifdef HAVE_MALLINFO
        /* execute run */
        osal_nano_sec_t exec_time = run();
        printf("%d/%d\n", j + 1, NO_OF_MM_ALLOC_DEALLOC_MAX_RUNS);
#ifdef HAVE_MALLINFO
        int64_t heap_usage_after = getTotalAllocatedSpace();
#endif //#ifdef HAVE_MALLINFO

        // At the beginning of measurement, measured value is not stable. Hence, the latest NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME times is used.
        // The increase of memory consumption and execution time will saturate after many times(NO_OF_MAX_RUNS(100) times may be enough) of executions.
        // If those are not saturated, there is memory leaking or performance degradation caused by wrapper implementation.
        if(j > NO_OF_MM_ALLOC_DEALLOC_MAX_RUNS - NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME)
        {
            /* kind of ring buffer for exec_time_dif_list */
            exec_time_diff_list.erase(exec_time_diff_list.begin());                           /* remove first element */
            exec_time_diff_list.push_back(exec_time - last_exec_time); /* add new at the end */

            /* if we measure a raise in execution for a NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, the next line while fail*/
            ASSERT_THAT(exec_time_diff_list, Not(Each(Ge(0))))
                << "We measured for " << NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME
                << " consecutive runs an increase in execution time --> Performance degradation over time detected";

#ifdef HAVE_MALLINFO
        /* kind of ring buffer for heap_usage_diff_list */
        heap_usage_diff_list.erase(heap_usage_diff_list.begin());                                     /* remove first element */
        heap_usage_diff_list.push_back(heap_usage_after - heap_usage_before); /* add new at the end */
        /* if we measure a raise in heap usage for a NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, the next line while
         * fail*/
        ASSERT_THAT(heap_usage_diff_list, Not(Each(Ge(0))))
            << "We measured for " << NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME
            << " consecutive runs an increase in heap usage --> 'Memory Leak' detected";
#endif //#ifdef HAVE_MALLINFO
        }
        last_exec_time = exec_time;
    }
    EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_MmngrClose(m_mm_hdnl));
}
// [MemoryDealloc_TC_09_004_037]

/**
 * @page Performance_TEST OSAL Resource Performance Test
 * @section OsalResourcePerformance_MemoryDealloc_TC_09_004_038 OsalResourcePerformance.MemoryDealloc_TC_09_004_038
 * @subsection MemoryDealloc_TC_09_004_038_tsi Test Suite ID
 * OsalResourcePerformance
 * @subsection MemoryDealloc_TC_09_004_038_tci Test Case ID
 * MemoryDealloc_S3_N_TC_09_004_038
 * @subsection MemoryDealloc_TC_09_004_038_rr Related Requirement(s)
 * \#276732, \#276410, \#276412
 * @subsection MemoryDealloc_TC_09_004_038_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MemoryDealloc_TC_09_004_038_tcs Test Case Summary
 * - Test09_004_038: This test shall verify the performance of message queue creation/destroying through its execution time and memory consumption
 * @subsection MemoryDealloc_TC_09_004_038_tpts Test Procedure/Test Steps
 * -# Open new Memory Object by using R_OSAL_MmngrOpen
 *      - config.mode is OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT
 *      - config.memory_limit is 10MByte
 *      - config.max_allowed_allocations is 500
 * -# Use a for loop with loop counter is 500 with the sequence call as below:
 *      - Get the total allocated memory before execution time measurement start
 *      - Get the start timestamp by using R_OSAL_ClockTimeGetTimeStamp
 *      - Allocate Memory buffer(4096byte) 500times by using R_OSAL_MmngrAlloc
 *      - Deallocate Memory buffer 500times by using R_OSAL_MmngrDealloc
 *      - Get the end timestamp by using R_OSAL_ClockTimeGetTimeStamp
 *      - Calculate the execution time by using R_OSAL_ClockTimeCalculateTimeDifference
 *      - Get the total allocated memory after execution time measurement end
 *      - Calculate the gap value of execution time/allocated memory of current loop with the previous loop (only in last 10 loop times).<br>
 *        Store these gap values in a gap list.
 *      - Close Memory Object by using R_OSAL_MmngrClose
 * @subsection MemoryDealloc_TC_09_004_038_ereb Expected Result/Behavior
 * -# R_OSAL_MmngrOpen return OSAL_RETURN_OK
 * -# This is the expectation in for loop:
 *      - Get the total allocated memory successful
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK
 *      - R_OSAL_MmngrAlloc return OSAL_RETURN_OK
 *      - R_OSAL_MmngrDealloc return OSAL_RETURN_OK
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK
 *      - R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK
 *      - Get the total allocated memory successful
 *      - All elements in gap list must not greater than 0.
 * -# R_OSAL_MmngrClose return OSAL_RETURN_OK
 * @subsection MemoryDealloc_TC_09_004_038_testcase Test Case Example
 * @snippet test_resource_measurement.cpp MemoryDealloc_TC_09_004_038
 */
// [MemoryDealloc_TC_09_004_038]
TEST_F(OsalResourcePerformance, MemoryDealloc_TC_09_004_038)
{
    st_osal_mmngr_config_t             config;
    config.memory_limit                = 10 * 1024 * 1024;
    config.max_allowed_allocations     = NO_OF_MM_ALLOC_DEALLOC;
    config.max_registered_monitors_cbs = NO_OF_MM_ALLOC_DEALLOC;
    config.mode = OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT;
    EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_MmngrOpen(&config, &m_mm_hdnl));

    auto run = [this]() -> osal_nano_sec_t {
        size_t size = 4096;
        size_t alignment  = 64;
        st_osal_time_t  start_time;
        st_osal_time_t  end_time;
        osal_nano_sec_t exec_time;

        /* get starting timestamp */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &start_time));

        for (int i = 0; i < NO_OF_MM_ALLOC_DEALLOC; i++)
        {
            EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_MmngrAlloc(m_mm_hdnl, size, alignment, &m_mm_buf_list[i]));
        }
        for (int i = 0; i < NO_OF_MM_ALLOC_DEALLOC; i++)
        {
            EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_MmngrDealloc(m_mm_hdnl, m_mm_buf_list[NO_OF_MM_ALLOC_DEALLOC - i - 1]));
        }
        /* get end timestamp */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &end_time));

        /* calculate diff */
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_ClockTimeCalculateTimeDifference(&end_time, &start_time, &exec_time));

        return exec_time;
    };

    vector<osal_nano_sec_t> exec_time_diff_list;
    size_t                 max_exec_time_exceeded_cnt = 0;
    /* fill the list with NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME elements with minimal values for osal_nano_sec_t */
    exec_time_diff_list.resize(NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, INT64_MIN);

#ifdef HAVE_MALLINFO
    vector<int64_t> heap_usage_diff_list;
    heap_usage_diff_list.resize(NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, INT64_MIN);
#endif // HAVE_MALLINFO

    osal_nano_sec_t last_exec_time;
    for (size_t j = 0; j < NO_OF_MM_ALLOC_DEALLOC_MAX_RUNS; j++)
    {
#ifdef HAVE_MALLINFO
        int64_t heap_usage_before = getTotalAllocatedSpace();
#endif //#ifdef HAVE_MALLINFO
        /* execute run */
        osal_nano_sec_t exec_time = run();
        printf("%d/%d\n", j + 1, NO_OF_MM_ALLOC_DEALLOC_MAX_RUNS);
#ifdef HAVE_MALLINFO
        int64_t heap_usage_after = getTotalAllocatedSpace();
#endif //#ifdef HAVE_MALLINFO

        // At the beginning of measurement, measured value is not stable. Hence, the latest NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME times is used.
        // The increase of memory consumption and execution time will saturate after many times(NO_OF_MAX_RUNS(100) times may be enough) of executions.
        // If those are not saturated, there is memory leaking or performance degradation caused by wrapper implementation.
        if(j > NO_OF_MM_ALLOC_DEALLOC_MAX_RUNS - NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME)
        {
            /* kind of ring buffer for exec_time_dif_list */
            exec_time_diff_list.erase(exec_time_diff_list.begin());                           /* remove first element */
            exec_time_diff_list.push_back(exec_time - last_exec_time); /* add new at the end */

            /* if we measure a raise in execution for a NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, the next line while fail*/
            ASSERT_THAT(exec_time_diff_list, Not(Each(Ge(0))))
                << "We measured for " << NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME
                << " consecutive runs an increase in execution time --> Performance degradation over time detected";

#ifdef HAVE_MALLINFO
        /* kind of ring buffer for heap_usage_diff_list */
        heap_usage_diff_list.erase(heap_usage_diff_list.begin());                                     /* remove first element */
        heap_usage_diff_list.push_back(heap_usage_after - heap_usage_before); /* add new at the end */
        /* if we measure a raise in heap usage for a NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME, the next line while
         * fail*/
        ASSERT_THAT(heap_usage_diff_list, Not(Each(Ge(0))))
            << "We measured for " << NO_OF_MAX_CONSECUTIVE_RAISE_IN_EXEC_TIME
            << " consecutive runs an increase in heap usage --> 'Memory Leak' detected";
#endif //#ifdef HAVE_MALLINFO
        }
        last_exec_time = exec_time;
    }
    EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_MmngrClose(m_mm_hdnl));
}
// [MemoryDealloc_TC_09_004_038]
