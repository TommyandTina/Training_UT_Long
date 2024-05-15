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
* File Name :    test_init_deinit.cpp
* Version :      3.1.1
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Initialize/Deinitialize API Functional Test (From the perspective of OSAL API Specification)
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           2.0.0     29.10.2021    Create New
 *           2.0.1     11.11.2021    Add R_OSAL_PmSetRequiredState with OSAL_PM_RESET_STATE_RELEASED
 *                                   for "osal_test_init_deinit_pm_waitstate"
 *           2.0.2     18.11.2021    Fix Typo
 *           3.0.0     15.02.2022    Apply new device and its setting definition
 *           3.0.1     07.04.2022    Enable the disabled test cases in SIL
 *           3.0.2     15.04.2022    Add synchronization method for test cases using multi-threads
 *           3.1.0     28.07.2022    Add test InitDeinit_TC_10_001_015 and InitDeinit_TC_10_001_016
 *           3.1.1     17.05.2023    Update waiting time of InitDeinit_TC_10_001_008
***********************************************************************************************************************/
#include "test_common.hpp"
#if !defined(OSAL_TEST_FRAMEWORK)
using namespace std;
#else
using namespace OsalFWHelper;
#endif

#if defined(ENABLE_LONGTIME_TEST)
#define INIT_DEINIT_LOOP_CNT 2000
#else
#define INIT_DEINIT_LOOP_CNT 1
#endif

/**
 * @page InitDeinit_TEST Initialization and deinitialization API Test
 * @test
 * @test @link InitDeinitApiTest_InitDeinit_TC_10_001_001 @endlink
 * @test @link InitDeinitApiTest_InitDeinit_TC_10_001_002 @endlink
 * @test @link InitDeinitApiTest_InitDeinit_TC_10_001_003 @endlink
 * @test @link InitDeinitApiTest_InitDeinit_TC_10_001_004 @endlink
 * @test @link InitDeinitApiTest_InitDeinit_TC_10_001_005 @endlink
 * @test @link InitDeinitApiTest_InitDeinit_TC_10_001_006 @endlink
 * @test @link InitDeinitApiTest_InitDeinit_TC_10_001_007 @endlink
 * @test @link InitDeinitApiTest_InitDeinit_TC_10_001_008 @endlink
 * @test @link InitDeinitApiTest_InitDeinit_TC_10_001_009 @endlink
 * @test @link InitDeinitApiTest_InitDeinit_TC_10_001_010 @endlink
 * @test @link InitDeinitApiTest_InitDeinit_TC_10_001_011 @endlink
 * @test @link InitDeinitApiTest_InitDeinit_TC_10_001_012 @endlink
 * @test @link InitDeinitApiTest_InitDeinit_TC_10_001_013 @endlink
 * @test @link InitDeinitApiTest_InitDeinit_TC_10_001_014 @endlink
 * @test @link InitDeinitApiTest_InitDeinit_TC_10_001_015 @endlink
 * @test @link InitDeinitApiTest_InitDeinit_TC_10_001_016 @endlink
 * @section InitDeinitApiTest Initialize/Deinitialize test function
 * @subsection InitDeinitApiTest_SetUp InitDeinitApiTest Setup
 * Common to Test Suite of InitDeinitApiTest, prepare process to be executed before executing the test. <br>
 * For example:
 * @snippet test_init_deinit.cpp InitDeinitApiTest_SetUpTestSuite
 * @snippet test_common.hpp OSALTESTS_SetUp
 * @subsection InitDeinitApiTest_TearDown InitDeinitApiTest TearDown
 * Common to Test Suite of InitDeinitApiTest, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_init_deinit.cpp InitDeinitApiTest_TearDownTestSuite
 * @snippet test_common.hpp OSALTESTS_TearDown
 */
class InitDeinitApiTest : public OSALTESTS
{
protected:
    void SetUp() override {}
    void TearDown() override {}

// [InitDeinitApiTest_SetUpTestSuite]
    static void SetUpTestSuite()
    {
        EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_Deinitialize());
    }
// [InitDeinitApiTest_SetUpTestSuite]

// [InitDeinitApiTest_TearDownTestSuite]
    static void TearDownTestSuite()
    {
        EXPECT_EQ(R_OSAL_Initialize(), OSAL_RETURN_OK);
    }
// [InitDeinitApiTest_TearDownTestSuite]
};

struct osal_test_init_deinit_condperiod_worker_arg
{
    osal_test_init_deinit_condperiod_worker_arg(osal_cond_handle_t * c_handle, osal_mutex_handle_t * mtx_handle) : cond_handle(c_handle), mutex_handle(mtx_handle)
    {
    }
    osal_mutex_handle_t *mutex_handle;
    osal_cond_handle_t *cond_handle;
    osal_mq_handle_t sync_mq;
    int sync_send_msg;
};

typedef struct st_osal_test_init_deinit_threadsleep {
    osal_mutex_handle_t mutex_handle;
    osal_cond_handle_t cond_handle;
} st_osal_test_init_deinit_threadsleep_t;

typedef struct st_osal_test_init_deinit_send_timeperiod {
    osal_milli_sec_t time;
    osal_mq_handle_t mq_handle;
    void *p_buffer;
    size_t buffer_size;
    osal_mq_handle_t sync_mq;
    int sync_send_msg;
} st_osal_test_init_deinit_send_timeperiod_t;

typedef struct st_osal_test_init_deinit_receive_timeperiod {
    osal_milli_sec_t time;
    osal_mq_handle_t mq_handle;
    size_t buffer_size;
    osal_mq_handle_t sync_mq;
    int sync_send_msg;
} st_osal_test_init_deinit_receive_timeperiod_t;


typedef struct st_osal_test_init_deinit_pm {
    osal_milli_sec_t time;
    osal_device_handle_t io_handle;
    osal_mutex_handle_t mutex_handle;
    osal_mq_handle_t sync_mq;
    int sync_send_msg;
} st_osal_test_init_deinit_pm_t;

// [osal_test_init_deinit_thread_simple_workerfunction]
int64_t osal_test_init_deinit_thread_simple_workerfunction(void *user_arg)
{
    st_osal_test_init_deinit_threadsleep *arg;
    arg = (st_osal_test_init_deinit_threadsleep *) user_arg;
    osal_milli_sec_t time = 500;

    EXPECT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(arg->mutex_handle, time), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncCondSignal(arg->cond_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(arg->mutex_handle), OSAL_RETURN_OK);

    EXPECT_EQ(R_OSAL_ThreadSleepForTimePeriod(time), OSAL_RETURN_OK);

    return (int64_t) 0;
}
// [osal_test_init_deinit_thread_simple_workerfunction]

// [osal_test_init_deinit_condwait_workerfunction]
int64_t osal_test_init_deinit_condwait_workerfunction(void * arg)
{
    osal_test_init_deinit_condperiod_worker_arg *testStruct = (osal_test_init_deinit_condperiod_worker_arg *)arg;
    osal_milli_sec_t time = 5000;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_mq_config local_mq_config;

    EXPECT_EQ(R_OSAL_MqGetConfig(testStruct->sync_mq, &local_mq_config), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncMutexTryLock(*(testStruct->mutex_handle)), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(testStruct->sync_mq, SYNC_MQ_TIME, &testStruct->sync_send_msg, local_mq_config.msg_size), OSAL_RETURN_OK);
    while(OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_ThsyncCondWaitForTimePeriod(*(testStruct->cond_handle), *(testStruct->mutex_handle), time);
        if(OSAL_RETURN_OK == osal_ret)
        {
            /* spurious wakeup. Continue wait. */
            printf("Spurious wakeup. Continue wait.\n");
        }
    }
    EXPECT_EQ(osal_ret, OSAL_RETURN_TIME);
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(*(testStruct->mutex_handle)), OSAL_RETURN_OK);
    return (int64_t) osal_ret;
}
// [osal_test_init_deinit_condwait_workerfunction]

// [osal_test_init_deinit_thread_send_timeperiod]
int64_t osal_test_init_deinit_thread_send_timeperiod(void *user_arg)
{
    e_osal_return_t osal_ret    = OSAL_RETURN_OK;
    st_osal_test_init_deinit_send_timeperiod_t *arg;
    arg = (st_osal_test_init_deinit_send_timeperiod_t *) user_arg;
    st_osal_mq_config local_mq_config;

    EXPECT_EQ(R_OSAL_MqGetConfig(arg->sync_mq, &local_mq_config), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(arg->sync_mq, SYNC_MQ_TIME, &arg->sync_send_msg, local_mq_config.msg_size), OSAL_RETURN_OK);
    osal_ret = R_OSAL_MqSendForTimePeriod(arg->mq_handle, arg->time, arg->p_buffer, arg->buffer_size);
    EXPECT_EQ(osal_ret, OSAL_RETURN_TIME);
    return (int64_t)osal_ret;
}
// [osal_test_init_deinit_thread_send_timeperiod]

// [osal_test_init_deinit_thread_receive_timeperiod]
int64_t osal_test_init_deinit_thread_receive_timeperiod(void *user_arg)
{
    e_osal_return_t osal_ret    = OSAL_RETURN_OK;
    st_osal_test_init_deinit_receive_timeperiod_t *arg;
    arg = (st_osal_test_init_deinit_receive_timeperiod_t *) user_arg;
    uint32_t rcv_data;
    st_osal_mq_config local_mq_config;

    EXPECT_EQ(R_OSAL_MqGetConfig(arg->sync_mq, &local_mq_config), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(arg->sync_mq, SYNC_MQ_TIME, &arg->sync_send_msg, local_mq_config.msg_size), OSAL_RETURN_OK);
    osal_ret = R_OSAL_MqReceiveForTimePeriod(arg->mq_handle, arg->time, &rcv_data, arg->buffer_size);
    EXPECT_EQ(osal_ret, OSAL_RETURN_TIME);
    return (int64_t) osal_ret;
}
// [osal_test_init_deinit_thread_receive_timeperiod]

// [osal_test_init_deinit_pm_waitstate]
int64_t osal_test_init_deinit_pm_waitstate(void *user_arg)
{
    e_osal_return_t osal_ret    = OSAL_RETURN_OK;
    st_osal_test_init_deinit_pm_t *arg;
    arg = (st_osal_test_init_deinit_pm_t *) user_arg;
    osal_milli_sec_t time_period_10ms = 10;
    st_osal_mq_config local_mq_config;

    EXPECT_EQ(R_OSAL_MqGetConfig(arg->sync_mq, &local_mq_config), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(arg->mutex_handle, time_period_10ms), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(arg->sync_mq, SYNC_MQ_TIME, &arg->sync_send_msg, local_mq_config.msg_size), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetRequiredState(arg->io_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true), OSAL_RETURN_OK);

    EXPECT_EQ(R_OSAL_PmSetPolicy(arg->io_handle, OSAL_PM_POLICY_HP, true), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(arg->mutex_handle), OSAL_RETURN_OK);
    osal_ret = R_OSAL_PmWaitForState(arg->io_handle, OSAL_PM_STATE_PG, arg->time);
    EXPECT_EQ(osal_ret, OSAL_RETURN_TIME);

    EXPECT_EQ(R_OSAL_PmSetRequiredState(arg->io_handle, OSAL_PM_REQUIRED_STATE_RELEASED, false), OSAL_RETURN_OK);
    return (int64_t) osal_ret;
}
// [osal_test_init_deinit_pm_waitstate]

// [osal_init_deinit_test_isr_cb_sample]
void osal_init_deinit_test_isr_cb_sample(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    (void)dev_handle;
    (void)irq_channel;
    (void)irq_arg;
}
// [osal_init_deinit_test_isr_cb_sample]

/**
 * @page InitDeinit_TEST Initialization and deinitialization API Test
 * @section InitDeinitApiTest_InitDeinit_TC_10_001_001 InitDeinitApiTest.InitDeinit_TC_10_001_001
 * @subsection InitDeinit_tsi Test Suite ID
 * InitDeinitApiTest
 * @subsection InitDeinit_tci Test Case ID
 * InitDeinit_S1_N_TC_10_001_001
 * @subsection InitDeinit_TC_10_001_001_rr Related Requirement(s)
 * \#199120, \#260098
 * @subsection InitDeinit_TC_10_001_001_pp Prerequisite/Precondition
 * None
 * @subsection InitDeinit_TC_10_001_001_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_Initialize/R_OSAL_Deinitialize in stress test with normal condition:
 * - Test10_001_001: stress test (2000 times)
 * @subsection InitDeinit_TC_10_001_001_tpts Test Procedure/Test Steps
 * -# Call R_OSAL_Initialize and then R_OSAL_Deinitialize in a for loop with loop count is 2000
 * @subsection InitDeinit_TC_10_001_001_ereb Expected Result/Behavior
 * -# Both R_OSAL_Initialize and R_OSAL_Deinitialize need return OSAL_RETURN_OK.
 * @subsection InitDeinit_TC_10_001_001_testcase Test Case Example
 * @snippet test_init_deinit.cpp InitDeinit_TC_10_001_001
 */
// [InitDeinit_TC_10_001_001]
TEST_F(InitDeinitApiTest, InitDeinit_TC_10_001_001)
{
    // Test10_001_001
    // @brief: stress test (2000 times)
    for (int i = 0; i < INIT_DEINIT_LOOP_CNT; i++)
    {
        EXPECT_EQ(R_OSAL_Initialize(), OSAL_RETURN_OK) << "Test10_001_001 failed";
        EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_OK) << "Test10_001_001 failed";
        if(i % 10 == 9)
        {
            printf("loop: %d/%d\n", (i + 1), INIT_DEINIT_LOOP_CNT);
        }
    }
}
// [InitDeinit_TC_10_001_001]

/**
 * @page InitDeinit_TEST Initialization and deinitialization API Test
 * @section InitDeinitApiTest_InitDeinit_TC_10_001_002 InitDeinitApiTest.InitDeinit_TC_10_001_002
 * @subsection InitDeinit_TC_10_001_002_tsi Test Suite ID
 * InitDeinitApiTest
 * @subsection InitDeinit_TC_10_001_002_tci Test Case ID
 * InitDeinit_S2_A_TC_10_001_002
 * @subsection InitDeinit_TC_10_001_002_rr Related Requirement(s)
 * \#199120, \#260098
 * @subsection InitDeinit_TC_10_001_002_pp Prerequisite/Precondition
 * None
 * @subsection InitDeinit_TC_10_001_002_tcs Test Case Summary
 * This test shall verify the state transition of R_OSAL_Initialize/R_OSAL_Deinitialize with abnormal condition:
 * - Test10_001_002: call R_OSAL_Deinitialize without calling R_OSAL_Initialize
 * @subsection InitDeinit_TC_10_001_002_tpts Test Procedure/Test Steps
 * -# Call R_OSAL_Deinitialize without calling R_OSAL_Initialize
 * @subsection InitDeinit_TC_10_001_002_ereb Expected Result/Behavior
 * -# R_OSAL_Deinitialize return OSAL_RETURN_STATE
 * @subsection InitDeinit_TC_10_001_002_testcase Test Case Example
 * @snippet test_init_deinit.cpp InitDeinit_TC_10_001_002
 */
// [InitDeinit_TC_10_001_002]
TEST_F(InitDeinitApiTest, InitDeinit_TC_10_001_002)
{
    // Test10_001_002
    // @brief: call R_OSAL_Deinitialize without calling R_OSAL_Initialize
    EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_STATE) << "Test10_001_002 failed";
}
// [InitDeinit_TC_10_001_002]

/**
 * @page InitDeinit_TEST Initialization and deinitialization API Test
 * @section InitDeinitApiTest_InitDeinit_TC_10_001_003 InitDeinitApiTest.InitDeinit_TC_10_001_003
 * @subsection InitDeinit_TC_10_001_003_tsi Test Suite ID
 * InitDeinitApiTest
 * @subsection InitDeinit_TC_10_001_003_tci Test Case ID
 * InitDeinit_S2_A_TC_10_001_003
 * @subsection InitDeinit_TC_10_001_003_rr Related Requirement(s)
 * \#199120, \#260098
 * @subsection InitDeinit_TC_10_001_003_pp Prerequisite/Precondition
 * None
 * @subsection InitDeinit_TC_10_001_003_tcs Test Case Summary
 * This test shall verify the state transition of R_OSAL_Initialize/R_OSAL_Deinitialize with abnormal condition:
 * - Test10_001_003: call R_OSAL_Initialize 2 times
 * @subsection InitDeinit_TC_10_001_003_tpts Test Procedure/Test Steps
 * -# Call R_OSAL_Initialize to init OSAL APIs
 * -# Continue to call R_OSAL_Initialize to init OSAL APIs
 * -# Call R_OSAL_Deinitialize to de-init OSAL APIs
 * @subsection InitDeinit_TC_10_001_003_ereb Expected Result/Behavior
 * -# R_OSAL_Initialize return OSAL_RETURN_OK
 * -# R_OSAL_Initialize return OSAL_RETURN_STATE
 * -# R_OSAL_Deinitialize return OSAL_RETURN_OK.
 * @subsection InitDeinit_TC_10_001_003_testcase Test Case Example
 * @snippet test_init_deinit.cpp InitDeinit_TC_10_001_003
 */
// [InitDeinit_TC_10_001_003]
TEST_F(InitDeinitApiTest, InitDeinit_TC_10_001_003)
{
    // Test10_001_003
    // @brief: call R_OSAL_Initialize 2 times
    EXPECT_EQ(R_OSAL_Initialize(), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_Initialize(), OSAL_RETURN_STATE) << "Test10_001_003 failed";
    EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_OK);
}
// [InitDeinit_TC_10_001_003]

/**
 * @page InitDeinit_TEST Initialization and deinitialization API Test
 * @section InitDeinitApiTest_InitDeinit_TC_10_001_004 InitDeinitApiTest.InitDeinit_TC_10_001_004
 * @subsection InitDeinit_TC_10_001_004_tsi Test Suite ID
 * InitDeinitApiTest
 * @subsection InitDeinit_TC_10_001_004_tci Test Case ID
 * InitDeinit_S2_A_TC_10_001_004
 * @subsection InitDeinit_TC_10_001_004_rr Related Requirement(s)
 * \#199120, \#260098
 * @subsection InitDeinit_TC_10_001_004_pp Prerequisite/Precondition
 * None
 * @subsection InitDeinit_TC_10_001_004_tcs Test Case Summary
 * This test shall verify for the function R_OSAL_Deinitialize when the OSAL Thread resource is not joined in multi thread.
 * - Test10_001_004: Thread is not joined
 * @subsection InitDeinit_TC_10_001_004_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_Initialize.
 * -# Create a mutex using R_OSAL_ThsyncMutexCreate.
 * -# Create a cond using R_OSAL_ThsyncCondCreate:
 * -# Lock the mutex using R_OSAL_ThsyncMutexLockForTimePeriod with 1000ms.
 * -# Create a thread using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *           -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *           -# task_name is "Test10_001_004"
 *           -# userarg is &thread_sleep_arg included:
 *              - mutex handle
 *              - cond handle
 *           -# func is osal_test_init_deinit_thread_simple_workerfunction with functionality:
 *              - Lock the mutex using R_OSAL_ThsyncMutexLockForTimePeriod with 500ms.
 *              - Send signal using R_OSAL_ThsyncCondSignal.
 *              - Unlock the mutex by R_OSAL_ThsyncMutexUnlock.
 *              - Call R_OSAL_ThreadSleepForTimePeriod to sleep in 500ms.
 *           -# stack_size is THREAD_STACK_SIZE
 *      - uid is TEST_THREAD_ID1.
 *      - Address of thread handle.
 * -# Wait the cond using R_OSAL_ThsyncCondWaitForTimePeriod with 1000ms.
 * -# Unlock the mutex by R_OSAL_ThsyncMutexUnlcok.
 * -# Destory the cond by R_OSAL_ThsyncCondDestroy.
 * -# Destory the mutex by R_OSAL_ThsyncMutexDestroy.
 * -# Call function R_OSAL_Deinitialize.
 * -# Call function R_OSAL_ThreadJoin.
 * -# Call function R_OSAL_Deinitialize.
 * @subsection InitDeinit_TC_10_001_004_ereb Expected Result/Behavior
 * -# Function R_OSAL_Initialize return OSAL_RETURN_OK.
 * -# Function R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.
 * -# Function R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 * -# Function R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.
 * -# Function R_OSAL_ThreadCreate return OSAL_RETURN_OK. <br>
 * Thread handle is not nulptr. <br>
 * In osal_test_init_deinit_thread_simple_workerfunction function:
 *  - Function R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 *  - Function R_OSAL_ThsyncCondSignal return OSAL_RETURN_OK.
 *  - Function R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 *  - Function R_OSAL_ThreadSleepForTimePeriod return OSAL_RETURN_OK.
 * -# Function R_OSAL_ThsyncCondWaitForTimePeriod return OSAL_RETURN_OK.
 * -# Function R_OSAL_ThsyncMutexUnlcok return OSAL_RETURN_OK.
 * -# Function R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK.
 * -# Function R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * -# Function R_OSAL_Deinitialize return OSAL_RETURN_BUSY.
 * -# Function R_OSAL_ThreadJoin return OSAL_RETURN_OK. <br>
 * The returned result is (int64_t)OSAL_RETURN_OK.
 * -# Function R_OSAL_Deinitialize return OSAL_RETURN_OK.
 * @subsection InitDeinit_TC_10_001_004_testcase Test Case Example
 * @snippet test_init_deinit.cpp InitDeinit_TC_10_001_004
 * @snippet test_init_deinit.cpp osal_test_init_deinit_thread_simple_workerfunction
 */
// [InitDeinit_TC_10_001_004]
TEST_F(InitDeinitApiTest, InitDeinit_TC_10_001_004)
{
    osal_thread_id_t thread_id = TEST_THREAD_ID1;
    osal_mutex_id_t mutex_id   = TEST_MUTEX_ID1;
    osal_cond_id_t cond_id     = TEST_COND_ID1;
    st_osal_thread_config_t config;
    osal_thread_handle_t thread_handle = OSAL_THREAD_HANDLE_INVALID;
    osal_mutex_handle_t mutex_handle = OSAL_MUTEX_HANDLE_INVALID;
    osal_cond_handle_t cond_handle = OSAL_COND_HANDLE_INVALID;
    st_osal_test_init_deinit_threadsleep_t thread_sleep_arg;
    osal_milli_sec_t time = 1000;
    int64_t result = 0;

    // Test10_001_004
    // @brief Abnormal: Thread is not joined
    EXPECT_EQ(R_OSAL_Initialize(), OSAL_RETURN_OK) << "Test10_001_004 failed";

    EXPECT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle), OSAL_RETURN_OK) << "Test10_001_004 failed";
    EXPECT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "Test10_001_004 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(mutex_handle, time), OSAL_RETURN_OK) << "Test10_001_004 failed";

    thread_sleep_arg.mutex_handle = mutex_handle;
    thread_sleep_arg.cond_handle = cond_handle;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = (char *)"Test10_001_004";
    config.func      = osal_test_init_deinit_thread_simple_workerfunction;
    config.userarg    = (void *)&thread_sleep_arg;
    config.stack_size = THREAD_STACK_SIZE;

    EXPECT_EQ(R_OSAL_ThreadCreate(&config, thread_id, &thread_handle), OSAL_RETURN_OK) << "Test10_001_004 failed";
    EXPECT_NE(thread_handle, NULL_PTR) << "Test10_001_004 failed";

    EXPECT_EQ(R_OSAL_ThsyncCondWaitForTimePeriod(cond_handle, mutex_handle, time), OSAL_RETURN_OK) << "Test10_001_004 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(mutex_handle), OSAL_RETURN_OK);

    EXPECT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mutex_handle), OSAL_RETURN_OK);

    EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_BUSY) << "Test10_001_004 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK) << "Test10_001_004 failed";
    EXPECT_EQ(result, (int64_t)OSAL_RETURN_OK) << "Test10_001_004 failed";
    thread_handle = OSAL_THREAD_HANDLE_INVALID;
    EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_OK) << "Test10_001_004 failed";
}
// [InitDeinit_TC_10_001_004]

/**
 * @page InitDeinit_TEST Initialization and deinitialization API Test
 * @section InitDeinitApiTest_InitDeinit_TC_10_001_005 InitDeinitApiTest.InitDeinit_TC_10_001_005
 * @subsection InitDeinit_TC_10_001_005_tsi Test Suite ID
 * InitDeinitApiTest
 * @subsection InitDeinit_TC_10_001_005_tci Test Case ID
 * InitDeinit_S2_A_TC_10_001_005
 * @subsection InitDeinit_TC_10_001_005_rr Related Requirement(s)
 * \#199120, \#260098
 * @subsection InitDeinit_TC_10_001_005_pp Prerequisite/Precondition
 * None
 * @subsection InitDeinit_TC_10_001_005_tcs Test Case Summary
 * This test shall verify for the function R_OSAL_Deinitialize when the OSAL Mutex resource is not clean in single thread.
 * - Test10_001_005: Mutex is not destroyed.
 * @subsection InitDeinit_TC_10_001_005_tpts Test Procedure/Test Steps
 * -# Call R_OSAL_Initialize to init OSAL APIs.
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate function with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Call R_OSAL_Deinitialize to de-init OSAL APIs.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * -# Continue to call R_OSAL_Deinitialize to de-init OSAL APIs.
 * @subsection InitDeinit_TC_10_001_005_ereb Expected Result/Behavior
 * -# R_OSAL_Initialize return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_Deinitialize return following:
 *     - Dynamic Environment case: OSAL_RETURN_BUSY
 *     - Static Environment case: OSAL_RETURN_OK
 * -# R_OSAL_ThsyncMutexDestroy return following:
 *     - Dynamic Environment case: OSAL_RETURN_OK
 *     - Static Environment case: OSAL_RETURN_STATE
 * -# R_OSAL_Deinitialize return OSAL_RETURN_OK.
 * @subsection InitDeinit_TC_10_001_005_testcase Test Case Example
 * @snippet test_init_deinit.cpp InitDeinit_TC_10_001_005
 */
// [InitDeinit_TC_10_001_005]
TEST_F(InitDeinitApiTest, InitDeinit_TC_10_001_005)
{
    osal_mutex_id_t mutex_id = TEST_MUTEX_ID1;
    osal_mutex_handle_t mutex_handle = OSAL_MUTEX_HANDLE_INVALID;

    // Test10_001_005
    // @brief Abnormal: Mutex is not destroy
    EXPECT_EQ(R_OSAL_Initialize(), OSAL_RETURN_OK) << "Test10_001_005 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle), OSAL_RETURN_OK) << "Test10_001_005 failed";
    EXPECT_NE(mutex_handle, NULL_PTR) << "Test10_001_005 failed";
#if defined(SUPPORT_STATIC_ENV)
    EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_OK) << "Test10_001_005 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mutex_handle), OSAL_RETURN_STATE)  << "Test10_001_005 failed";

    EXPECT_EQ(R_OSAL_Initialize(), OSAL_RETURN_OK) << "Test10_001_005 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mutex_handle), OSAL_RETURN_HANDLE)  << "Test10_001_005 failed";
    EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_OK) << "Test10_001_005 failed";
#else
    EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_BUSY) << "Test10_001_005 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mutex_handle), OSAL_RETURN_OK)  << "Test10_001_005 failed";
    EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_OK) << "Test10_001_005 failed";
#endif
}
// [InitDeinit_TC_10_001_005]

/**
 * @page InitDeinit_TEST Initialization and deinitialization API Test
 * @section InitDeinitApiTest_InitDeinit_TC_10_001_006 InitDeinitApiTest.InitDeinit_TC_10_001_006
 * @subsection InitDeinit_TC_10_001_006_tsi Test Suite ID
 * InitDeinitApiTest
 * @subsection InitDeinit_TC_10_001_006_tci Test Case ID
 * InitDeinit_S2_A_TC_10_001_006
 * @subsection InitDeinit_TC_10_001_006_rr Related Requirement(s)
 * \#199120, \#260098
 * @subsection InitDeinit_TC_10_001_006_pp Prerequisite/Precondition
 * None
 * @subsection InitDeinit_TC_10_001_006_tcs Test Case Summary
 * This test shall verify for the function R_OSAL_Deinitialize when the OSAL Mutex resource is not released in single thread.
 * - Test10_001_006: Mutex is not unlocked.
 * @subsection InitDeinit_TC_10_001_006_tpts Test Procedure/Test Steps
 * -# Call R_OSAL_Initialize to init OSAL APIs.
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate function with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Lock a mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 * -# Call R_OSAL_Deinitialize to de-init OSAL APIs.
 * -# Unlock the mutex by using R_OSAL_ThsyncMutexUnlock.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * -# Continue to call R_OSAL_Deinitialize to de-init OSAL APIs.
 * @subsection InitDeinit_TC_10_001_006_ereb Expected Result/Behavior
 * -# R_OSAL_Initialize return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 * -# R_OSAL_Deinitialize return OSAL_RETURN_BUSY.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * -# R_OSAL_Deinitialize return OSAL_RETURN_OK.
 * @subsection InitDeinit_TC_10_001_006_testcase Test Case Example
 * @snippet test_init_deinit.cpp InitDeinit_TC_10_001_006
 */
// [InitDeinit_TC_10_001_006]
TEST_F(InitDeinitApiTest, InitDeinit_TC_10_001_006)
{
    osal_mutex_id_t mutex_id = TEST_MUTEX_ID1;
    osal_mutex_handle_t mutex_handle = OSAL_MUTEX_HANDLE_INVALID;
    osal_milli_sec_t time_1000ms = 1000;

    // Test10_001_006
    // @brief Abnormal: Mutex is lock
    EXPECT_EQ(R_OSAL_Initialize(), OSAL_RETURN_OK) << "Test10_001_006 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle), OSAL_RETURN_OK) << "Test10_001_006 failed";
    EXPECT_NE(mutex_handle, NULL_PTR) << "Test10_001_006 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(mutex_handle, time_1000ms), OSAL_RETURN_OK) << "Test10_001_006 failed";
    EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_BUSY) << "Test10_001_006 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(mutex_handle), OSAL_RETURN_OK) << "Test10_001_006 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mutex_handle), OSAL_RETURN_OK)  << "Test10_001_006 failed";
    EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_OK) << "Test10_001_006 failed";
}
// [InitDeinit_TC_10_001_006]

/**
 * @page InitDeinit_TEST Initialization and deinitialization API Test
 * @section InitDeinitApiTest_InitDeinit_TC_10_001_007 InitDeinitApiTest.InitDeinit_TC_10_001_007
 * @subsection InitDeinit_TC_10_001_007_tsi Test Suite ID
 * InitDeinitApiTest
 * @subsection InitDeinit_TC_10_001_007_tci Test Case ID
 * InitDeinit_S2_A_TC_10_001_007
 * @subsection InitDeinit_TC_10_001_007_rr Related Requirement(s)
 * \#199120, \#260098
 * @subsection InitDeinit_TC_10_001_007_pp Prerequisite/Precondition
 * None
 * @subsection InitDeinit_TC_10_001_007_tcs Test Case Summary
 * This test shall verify for the function R_OSAL_Deinitialize when the OSAL Condition variable resource is not clean in single thread.
 * - Test10_001_007: Condition variable is not destroyed.
 * @subsection InitDeinit_TC_10_001_007_tpts Test Procedure/Test Steps
 * -# Call R_OSAL_Initialize to init OSAL APIs.
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Call R_OSAL_Deinitialize to de-init OSAL APIs.
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy.
 * -# Continue to call R_OSAL_Deinitialize to de-init OSAL APIs.
 * @subsection InitDeinit_TC_10_001_007_ereb Expected Result/Behavior
 * -# R_OSAL_Initialize return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.<br>
 * "cond_handle" is not NULL_PTR.
 * -# R_OSAL_Deinitialize return following:
 *     - Dynamic Environment case: OSAL_RETURN_BUSY
 *     - Static Environment case: OSAL_RETURN_OK
 * -# R_OSAL_ThsyncCondDestroy return following:
 *     - Dynamic Environment case: OSAL_RETURN_OK
 *     - Static Environment case: OSAL_RETURN_STATE
 * -# R_OSAL_Deinitialize return OSAL_RETURN_OK.
 * @subsection InitDeinit_TC_10_001_007_testcase Test Case Example
 * @snippet test_init_deinit.cpp InitDeinit_TC_10_001_007
 */
// [InitDeinit_TC_10_001_007]
TEST_F(InitDeinitApiTest, InitDeinit_TC_10_001_007)
{
    osal_cond_id_t cond_id = TEST_COND_ID1;
    osal_cond_handle_t cond_handle = OSAL_COND_HANDLE_INVALID;

    // Test10_001_007
    // @brief Abnormal: Condition variable is not destroy
    EXPECT_EQ(R_OSAL_Initialize(), OSAL_RETURN_OK) << "Test10_001_007 failed";
    EXPECT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "Test10_001_007 failed";
    EXPECT_NE(cond_handle, NULL_PTR) << "Test10_001_007 failed";
#if defined(SUPPORT_STATIC_ENV)
    EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_OK) << "Test10_001_007 failed";
    EXPECT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_STATE)<< "Test10_001_007 failed";

    EXPECT_EQ(R_OSAL_Initialize(), OSAL_RETURN_OK) << "Test10_001_007 failed";
    EXPECT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_HANDLE)<< "Test10_001_007 failed";
    EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_OK) << "Test10_001_007 failed";
#else
    EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_BUSY) << "Test10_001_007 failed";
    EXPECT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK)<< "Test10_001_007 failed";
    EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_OK) << "Test10_001_007 failed";
#endif
}
// [InitDeinit_TC_10_001_007]

/**
 * @page InitDeinit_TEST Initialization and deinitialization API Test
 * @section InitDeinitApiTest_InitDeinit_TC_10_001_008 InitDeinitApiTest.InitDeinit_TC_10_001_008
 * @subsection InitDeinit_TC_10_001_008_tsi Test Suite ID
 * InitDeinitApiTest
 * @subsection InitDeinit_TC_10_001_008_tci Test Case ID
 * InitDeinit_S2_A_TC_10_001_008
 * @subsection InitDeinit_TC_10_001_008_rr Related Requirement(s)
 * \#199120, \#260098
 * @subsection InitDeinit_TC_10_001_008_pp Prerequisite/Precondition
 * None
 * @subsection InitDeinit_TC_10_001_008_tcs Test Case Summary
 * This test shall verify for the function R_OSAL_Deinitialize when the OSAL Condition variable resources is not released in multi thread.
 * - Test10_001_008: Condition variable is being waited by another thread
 * @subsection InitDeinit_TC_10_001_008_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_Initialize.
 * -# Create synchronization queue by using R_OSAL_MqCreate
 * -# Create a condition variable by calling R_OSAL_ThsyncCondCreate.
 * -# Create a mutex by calling R_OSAL_ThsyncMutexCreate.
 * -# Create a thread using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *           -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *           -# task_name is "Test10_001_008"
 *           -# func is osal_test_init_deinit_condwait_workerfunction with functionality:
 *              - Get the synchronization queue config by using R_OSAL_MqGetConfig
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *              - Lock the mutex by calling R_OSAL_ThsyncMutexTryLock.
 *              - Wait for condition variable by calling R_OSAL_ThsyncCondWaitForTimePeriod.
 *              - Unlock the mutex by calling R_OSAL_ThsyncMutexUnlock
 *           -# userarg is user_arg.get() which include condition variable, mutex handle, synchronization queue, synchronization send message.
 *           -# stack_size is THREAD_STACK_SIZE
 *      - uid is TEST_THREAD_ID1.
 *      - Address of thread handle.
 * -# Receive message from synchronization queue by using R_OSAL_MqReceiveForTimePeriod
 * -# Lock mutex by using R_OSAL_ThsyncMutexLockForTimePeriod
 * -# Call function R_OSAL_Deinitialize.
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock
 * -# Call function R_OSAL_ThreadJoin.
 * -# Destroy the condition variable by calling R_OSAL_ThsyncCondDestroy.
 * -# Destroy the mutex by calling R_OSAL_ThsyncMutexDestroy.
 * -# Reset synchronization queue by using R_OSAL_MqReset
 * -# Delete synchronization queue by using R_OSAL_MqDelete
 * -# Call function R_OSAL_Deinitialize.
 * @subsection InitDeinit_TC_10_001_008_ereb Expected Result/Behavior
 * -# Function R_OSAL_Initialize return OSAL_RETURN_OK.
 * -# Function R_OSAL_MqCreate return OSAL_RETURN_OK.
 * -# Function R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.
 * -# Function R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.
 * -# Function R_OSAL_ThreadCreate return OSAL_RETURN_OK. <br>
 * Thread handle is not nulptr. <br>
 * In osal_test_init_deinit_condwait_workerfunction function: <br>
 *  - R_OSAL_MqGetConfig return OSAL_RETURN_OK
 *  - Function R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_OK.
 *  - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK.
 *  - Function R_OSAL_ThsyncCondWaitForTimePeriod return OSAL_RETURN_TIME.
 *  - Function R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# Function R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK.
 * -# Function R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 * -# Function R_OSAL_Deinitialize return OSAL_RETURN_BUSY.
 * -# Function R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# Function R_OSAL_ThreadJoin return OSAL_RETURN_OK. <br>
 * The returned result is (int64_t)OSAL_RETURN_TIME.
 * -# Function R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK.
 * -# Function R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * -# Function R_OSAL_MqReset return OSAL_RETURN_OK.
 * -# Function R_OSAL_MqDelete return OSAL_RETURN_OK.
 * -# Function R_OSAL_Deinitialize return OSAL_RETURN_OK.
 * @subsection InitDeinit_TC_10_001_008_testcase Test Case Example
 * @snippet test_init_deinit.cpp InitDeinit_TC_10_001_008
 * @snippet test_init_deinit.cpp osal_test_init_deinit_condwait_workerfunction
 */
// [InitDeinit_TC_10_001_008]
TEST_F(InitDeinitApiTest, InitDeinit_TC_10_001_008)
{
    osal_thread_id_t thread_id = TEST_THREAD_ID1;
    osal_mutex_id_t mutex_id = TEST_MUTEX_ID1;
    osal_cond_id_t cond_id = TEST_COND_ID1;
    st_osal_thread_config_t config;
    osal_thread_handle_t thread_handle = OSAL_THREAD_HANDLE_INVALID;
    osal_mutex_handle_t mutex_handle = OSAL_MUTEX_HANDLE_INVALID;
    osal_cond_handle_t cond_handle = OSAL_COND_HANDLE_INVALID;
    int64_t result = 0;
    osal_milli_sec_t cond_time = 10000;
    osal_milli_sec_t time_10ms = 10;
    osal_mq_handle_t sync_mq;
    st_osal_mq_config sync_mq_config = {10, 4};
    int sync_send_msg = 0xFFFF;
    int sync_receive_msg = 0;

    // Test10_001_008
    // @brief Abnormal: Waiting for Condition vairiable
    EXPECT_EQ(R_OSAL_Initialize(), OSAL_RETURN_OK) << "Test10_001_008 failed";
    EXPECT_EQ(R_OSAL_MqCreate(&sync_mq_config, SYNC_MQ_ID, &sync_mq), OSAL_RETURN_OK);
    EXPECT_NE(sync_mq, OSAL_MQ_HANDLE_INVALID);
    EXPECT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "Test10_001_008 failed";
    EXPECT_NE(cond_handle, NULL_PTR) << "Test10_001_008 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle), OSAL_RETURN_OK) << "Test10_001_008 failed";
    EXPECT_NE(mutex_handle, NULL_PTR) << "Test10_001_008 failed";

    osal_test_init_deinit_condperiod_worker_arg user_arg(&cond_handle, &mutex_handle);
    user_arg.sync_send_msg = sync_send_msg;
    user_arg.sync_mq = sync_mq;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = (char *)"Test10_001_008";
    config.func      = osal_test_init_deinit_condwait_workerfunction;
    config.userarg   = &user_arg;
    config.stack_size = THREAD_STACK_SIZE;

    EXPECT_EQ(R_OSAL_ThreadCreate(&config, thread_id, &thread_handle), OSAL_RETURN_OK) << "Test10_001_008 failed";
    EXPECT_NE(thread_handle, NULL_PTR) << "Test10_001_008 failed";
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &sync_receive_msg, sync_mq_config.msg_size), OSAL_RETURN_OK);
    EXPECT_EQ(sync_receive_msg, sync_send_msg);
    EXPECT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(mutex_handle, cond_time), OSAL_RETURN_OK) << "Test10_001_008 failed";
    EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_BUSY) << "Test10_001_008 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(mutex_handle), OSAL_RETURN_OK) << "Test10_001_008 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK) << "Test10_001_008 failed";
    EXPECT_EQ(result, (int64_t)OSAL_RETURN_TIME) << "Test10_001_008 failed";
    EXPECT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK) << "Test10_001_008 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mutex_handle), OSAL_RETURN_OK) << "Test10_001_008 failed";
    mutex_handle = OSAL_MUTEX_HANDLE_INVALID;
    cond_handle = OSAL_COND_HANDLE_INVALID;
    EXPECT_EQ(R_OSAL_MqReset(sync_mq), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqDelete(sync_mq), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_OK) << "Test10_001_008 failed";
}
// [InitDeinit_TC_10_001_008]

/**
 * @page InitDeinit_TEST Initialization and deinitialization API Test
 * @section InitDeinitApiTest_InitDeinit_TC_10_001_009 InitDeinitApiTest.InitDeinit_TC_10_001_009
 * @subsection InitDeinit_TC_10_001_009_tsi Test Suite ID
 * InitDeinitApiTest
 * @subsection InitDeinit_TC_10_001_009_tci Test Case ID
 * InitDeinit_S2_A_TC_10_001_009
 * @subsection InitDeinit_TC_10_001_009_rr Related Requirement(s)
 * \#199120, \#260098
 * @subsection InitDeinit_TC_10_001_009_pp Prerequisite/Precondition
 * None
 * @subsection InitDeinit_TC_10_001_009_tcs Test Case Summary
 * This test shall verify for the function R_OSAL_Deinitialize when the OSAL Message Queue resource is not clean in single thread.
 * - Test10_001_009: Queue is not destroyed.
 * @subsection InitDeinit_TC_10_001_009_tpts Test Procedure/Test Steps
 * -# Call R_OSAL_Initialize to init OSAL APIs.
 * -# Create a new queue by calling R_OSAL_MqCreate.
 * -# Call R_OSAL_Deinitialize to de-init OSAL APIs.
 * -# Delete created queue by calling R_OSAL_MqDelete.
 * -# Continue to call R_OSAL_Deinitialize to de-init OSAL APIs.
 * @subsection InitDeinit_TC_10_001_009_ereb Expected Result/Behavior
 * -# R_OSAL_Initialize return OSAL_RETURN_OK.
 * -# R_OSAL_MqCreate return OSAL_RETURN_OK.
 * -# R_OSAL_Deinitialize return following:
 *     - Dynamic Environment case: OSAL_RETURN_BUSY
 *     - Static Environment case: OSAL_RETURN_OK
 * -# R_OSAL_MqDelete return following:
 *     - Dynamic Environment case: OSAL_RETURN_OK
 *     - Static Environment case: OSAL_RETURN_STATE
 * -# R_OSAL_Deinitialize return OSAL_RETURN_OK.
 * @subsection InitDeinit_TC_10_001_009_testcase Test Case Example
 * @snippet test_init_deinit.cpp InitDeinit_TC_10_001_009
 */
// [InitDeinit_TC_10_001_009]
TEST_F(InitDeinitApiTest, InitDeinit_TC_10_001_009)
{
    osal_mq_id_t mq_id = TEST_MQ_ID1;
    osal_mq_handle_t mq_handle = OSAL_MQ_HANDLE_INVALID;
    st_osal_mq_config_t mq_config;

    // Test10_001_009
    // @brief Abnormal: Queue is not destroy
    EXPECT_EQ(R_OSAL_Initialize(), OSAL_RETURN_OK) << "Test10_001_009 failed";
    mq_config.max_num_msg = 2;
    mq_config.msg_size = 4;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config, mq_id, &mq_handle), OSAL_RETURN_OK) << "Test10_001_009 failed";
#if defined(SUPPORT_STATIC_ENV)
    EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_OK) << "Test10_001_009 failed";
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_STATE) << "Test10_001_009 failed";

    EXPECT_EQ(R_OSAL_Initialize(), OSAL_RETURN_OK) << "Test10_001_009 failed";
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_HANDLE) << "Test10_001_009 failed";
    EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_OK) << "Test10_001_009 failed";
#else
    EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_BUSY) << "Test10_001_009 failed";
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK) << "Test10_001_009 failed";
    EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_OK) << "Test10_001_009 failed";
#endif
}
// [InitDeinit_TC_10_001_009]

/**
 * @page InitDeinit_TEST Initialization and deinitialization API Test
 * @section InitDeinitApiTest_InitDeinit_TC_10_001_010 InitDeinitApiTest.InitDeinit_TC_10_001_010
 * @subsection InitDeinit_TC_10_001_010_tsi Test Suite ID
 * InitDeinitApiTest
 * @subsection InitDeinit_TC_10_001_010_tci Test Case ID
 * InitDeinit_S2_A_TC_10_001_010
 * @subsection InitDeinit_TC_10_001_010_rr Related Requirement(s)
 * \#199120, \#260098
 * @subsection InitDeinit_TC_10_001_010_pp Prerequisite/Precondition
 * None
 * @subsection InitDeinit_TC_10_001_010_tcs Test Case Summary
 * This test shall verify for the function R_OSAL_Deinitialize when the message transmission process is happening in OSAL Message Queue resource.
 * - Test10_001_010: A message is sending to the Queue
 * @subsection InitDeinit_TC_10_001_010_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_Initialize.
 * -# Create synchronization queue by using R_OSAL_MqCreate
 * -# Create a message queue with max_num_msg is 2 and msg_size is 4 by calling function R_OSAL_MqCreate.
 * -# Call R_OSAL_MqGetConfig the get the config from the created queue.
 * -# Make the queue full by sending 2 message 0x1234 and 0x5678 by calling R_OSAL_MqSendForTimePeriod twice.
 * -# Create a thread using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *           -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *           -# task_name is "Test10_001_010"
 *           -# func is osal_test_init_deinit_thread_send_timeperiod with functionality:
 *              - Get the synchronization queue config by using R_OSAL_MqGetConfig
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *              - Send the message to the queue by calling R_OSAL_MqSendForTimePeriod.
 *           -# userarg is &send_timeperiod_arg included:
 *              - queue handle.
 *              - time is 5000
 *              - p_buffer is address if 0x9012 message.
 *              - buffer_size is mq_config.msg_size
 *              - sync_mq is handle of synchronization queue
 *              - sync_send_msg is the sending message for synchronization purpose
 *           -# stack_size is THREAD_STACK_SIZE
 *      - uid is TEST_THREAD_ID1.
 *      - Address of thread handle.
 * -# Receive message from synchronization queue by using R_OSAL_MqReceiveForTimePeriod
 * -# Sleep 10ms to make sure new thread has already been blocked by R_OSAL_MqSendForTimePeriod
 * -# Call function R_OSAL_Deinitialize.
 * -# Call function R_OSAL_ThreadJoin.
 * -# Call function R_OSAL_MqReset.
 * -# Call function R_OSAL_MqDelete.
 * -# Reset synchronization queue by using R_OSAL_MqReset
 * -# Delete synchronization queue by using R_OSAL_MqDelete
 * -# Call function R_OSAL_Deinitialize.
 * @subsection InitDeinit_TC_10_001_010_ereb Expected Result/Behavior
 * -# Function R_OSAL_Initialize return OSAL_RETURN_OK.
 * -# Function R_OSAL_MqCreate return OSAL_RETURN_OK.
 * -# Function R_OSAL_MqCreate return OSAL_RETURN_OK.<br>
 * The return message queue handle is not nulptr.
 * -# Function R_OSAL_MqGetConfig return OSAL_RETURN_OK.
 * -# 2 times calling function R_OSAL_MqSendForTimePeriod both return OSAL_RETURN_OK.
 * -# Function R_OSAL_ThreadCreate return OSAL_RETURN_OK. <br>
 * Thread handle is not nulptr. <br>
 * In osal_test_init_deinit_thread_send_timeperiod function: <br>
 *  - Function R_OSAL_MqGetConfig return OSAL_RETURN_OK.
 *  - Function R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK.
 *  - Function R_OSAL_MqSendForTimePeriod return OSAL_RETURN_TIME.
 * -# Function R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK.
 * -# Function R_OSAL_ThreadSleepForTimePeriod return OSAL_RETURN_OK.
 * -# Function R_OSAL_Deinitialize return OSAL_RETURN_BUSY.
 * -# Function R_OSAL_ThreadJoin return OSAL_RETURN_OK. <br>
 * The returned result is (int64_t)OSAL_RETURN_TIME.
 * -# Function R_OSAL_MqReset return OSAL_RETURN_OK.
 * -# Function R_OSAL_MqDelete return OSAL_RETURN_OK.
 * -# Function R_OSAL_MqReset return OSAL_RETURN_OK.
 * -# Function R_OSAL_MqDelete return OSAL_RETURN_OK.
 * -# Function R_OSAL_Deinitialize return OSAL_RETURN_OK.
 * @subsection InitDeinit_TC_10_001_010_testcase Test Case Example
 * @snippet test_init_deinit.cpp InitDeinit_TC_10_001_010
 * @snippet test_init_deinit.cpp osal_test_init_deinit_thread_send_timeperiod
 */
// [InitDeinit_TC_10_001_010]
TEST_F(InitDeinitApiTest, InitDeinit_TC_10_001_010)
{
    osal_thread_id_t thread_id = TEST_THREAD_ID1;
    osal_mq_id_t mq_id = TEST_MQ_ID1;
    st_osal_thread_config_t config;
    osal_thread_handle_t thread_handle = OSAL_THREAD_HANDLE_INVALID;
    osal_mq_handle_t mq_handle = OSAL_MQ_HANDLE_INVALID;
    st_osal_mq_config_t mq_config;
    int64_t result = 0;
    osal_milli_sec_t time_1000ms = 1000;
    osal_milli_sec_t time_10ms = 10;
    int32_t send_msg;
    st_osal_test_init_deinit_send_timeperiod_t send_timeperiod_arg;
    osal_mq_handle_t sync_mq;
    st_osal_mq_config sync_mq_config = {10, 4};
    int sync_send_msg = 0xFFFF;
    int sync_receive_msg = 0;

    // Test10_001_010
    // @brief Abnormal: Waiting for transmission with queue
    EXPECT_EQ(R_OSAL_Initialize(), OSAL_RETURN_OK) << "Test10_001_010 failed";
    EXPECT_EQ(R_OSAL_MqCreate(&sync_mq_config, SYNC_MQ_ID, &sync_mq), OSAL_RETURN_OK);
    EXPECT_NE(sync_mq, OSAL_MQ_HANDLE_INVALID);
    mq_config.max_num_msg = 2;
    mq_config.msg_size = 4;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config, mq_id, &mq_handle), OSAL_RETURN_OK) << "Test10_001_010 failed";
    EXPECT_EQ(R_OSAL_MqGetConfig(mq_handle, &mq_config), OSAL_RETURN_OK) << "Test10_001_010 failed";
    //make the queue full
    send_msg = 0x1234;
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(mq_handle,time_1000ms, &send_msg, mq_config.msg_size), OSAL_RETURN_OK) << "Test10_001_010 failed";
    send_msg = 0x5678;
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(mq_handle,time_1000ms, &send_msg, mq_config.msg_size), OSAL_RETURN_OK) << "Test10_001_010 failed";

    send_msg = 0x9012;
    send_timeperiod_arg.mq_handle = mq_handle;
    send_timeperiod_arg.time = 5000;
    send_timeperiod_arg.p_buffer =  &send_msg;
    send_timeperiod_arg.buffer_size = mq_config.msg_size;
    send_timeperiod_arg.sync_mq = sync_mq;
    send_timeperiod_arg.sync_send_msg = sync_send_msg;

    config.func       = osal_test_init_deinit_thread_send_timeperiod;
    config.userarg    = (void *)&send_timeperiod_arg;
    config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config.task_name  = (char *)"Test10_001_010";
    config.stack_size = THREAD_STACK_SIZE;

    EXPECT_EQ(R_OSAL_ThreadCreate(&config, thread_id, &thread_handle),OSAL_RETURN_OK) << "Test10_001_010 failed";
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &sync_receive_msg, sync_mq_config.msg_size), OSAL_RETURN_OK);
    EXPECT_EQ(sync_receive_msg, sync_send_msg);
    EXPECT_EQ(R_OSAL_ThreadSleepForTimePeriod(time_10ms), OSAL_RETURN_OK) << "Test10_001_010 failed";
    EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_BUSY) << "Test10_001_010 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result),OSAL_RETURN_OK) << "Test10_001_010 failed";
    EXPECT_EQ(result, (int64_t)OSAL_RETURN_TIME) << "Test03_004_010 failed";
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK) << "Test10_001_010 failed";
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK) << "Test10_001_010 failed";
    EXPECT_EQ(R_OSAL_MqReset(sync_mq), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqDelete(sync_mq), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_OK) << "Test10_001_010 failed";
}
// [InitDeinit_TC_10_001_010]

/**
 * @page InitDeinit_TEST Initialization and deinitialization API Test
 * @section InitDeinitApiTest_InitDeinit_TC_10_001_011 InitDeinitApiTest.InitDeinit_TC_10_001_011
 * @subsection InitDeinit_TC_10_001_011_tsi Test Suite ID
 * InitDeinitApiTest
 * @subsection InitDeinit_TC_10_001_011_tci Test Case ID
 * InitDeinit_S2_A_TC_10_001_011
 * @subsection InitDeinit_TC_10_001_011_rr Related Requirement(s)
 * \#199120, \#260098
 * @subsection InitDeinit_TC_10_001_011_pp Prerequisite/Precondition
 * None
 * @subsection InitDeinit_TC_10_001_011_tcs Test Case Summary
 * This test shall verify for the function R_OSAL_Deinitialize when the message reception process is happening in OSAL Message Queue resource.
 * - Test10_001_011: A message is receiving from the Queue
 * @subsection InitDeinit_TC_10_001_011_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_Initialize.
 * -# Create synchronization queue by using R_OSAL_MqCreate
 * -# Create a message queue with max_num_msg is 2 and msg_size is 4 by calling function R_OSAL_MqCreate.
 * -# Check current message queue is empty or not by calling R_OSAL_MqIsEmpty.
 * -# Call R_OSAL_MqGetConfig the get the config from the created queue.
 * -# Create a thread using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *           -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *           -# task_name is "Test10_001_011"
 *           -# func is osal_test_init_deinit_thread_receive_timeperiod with functionality:
 *              - Get the synchronization queue config by using R_OSAL_MqGetConfig
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *              - Receive the message from the queue by calling R_OSAL_MqReceiveForTimePeriod.
 *           -# userarg is &receive_timeperiod_arg included:
 *              - queue handle
 *              - time is 5000
 *              - buffer_size is mq_config.msg_size
 *              - sync_mq is handle of synchronization queue
 *              - sync_send_msg is the sending message for synchronization purpose
 *           -# stack_size is THREAD_STACK_SIZE
 *      - uid is TEST_THREAD_ID1.
 *      - Address of thread handle.
 * -# Receive message from synchronization queue by using R_OSAL_MqReceiveForTimePeriod
 * -# Sleep 10ms to make sure new thread has already been blocked by R_OSAL_MqReceiveForTimePeriod
 * -# Call function R_OSAL_Deinitialize.
 * -# Call function R_OSAL_ThreadJoin.
 * -# Call function R_OSAL_MqDelete.
 * -# Reset synchronization queue by using R_OSAL_MqReset
 * -# Delete synchronization queue by using R_OSAL_MqDelete
 * -# Call function R_OSAL_Deinitialize.
 * @subsection InitDeinit_TC_10_001_011_ereb Expected Result/Behavior
 * -# Function R_OSAL_Initialize return OSAL_RETURN_OK.
 * -# Function R_OSAL_MqCreate return OSAL_RETURN_OK.
 * -# Function R_OSAL_MqCreate return OSAL_RETURN_OK.<br>
 * The return message queue handle is not nulptr.
 * -# Function R_OSAL_MqIsEmpty return OSAL_RETURN_OK.<br>
 * The return result is true.
 * -# Function R_OSAL_MqGetConfig return OSAL_RETURN_OK.
 * -# Function R_OSAL_ThreadCreate return OSAL_RETURN_OK. <br>
 * Thread handle is not nulptr. <br>
 * In osal_test_init_deinit_thread_receive_timeperiod function: <br>
 *  - Function R_OSAL_MqGetConfig return OSAL_RETURN_OK.
 *  - Function R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK.
 *  - Function R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_TIME.
 * -# Function R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK.
 * -# Function R_OSAL_ThreadSleepForTimePeriod return OSAL_RETURN_OK.
 * -# Function R_OSAL_Deinitialize return OSAL_RETURN_BUSY.
 * -# Function R_OSAL_ThreadJoin return OSAL_RETURN_OK. <br>
 * The returned result is (int64_t)OSAL_RETURN_TIME.
 * -# Function R_OSAL_MqDelete return OSAL_RETURN_OK.
 * -# Function R_OSAL_MqReset return OSAL_RETURN_OK.
 * -# Function R_OSAL_MqDelete return OSAL_RETURN_OK.
 * -# Function R_OSAL_Deinitialize return OSAL_RETURN_OK.
 * @subsection InitDeinit_TC_10_001_011_testcase Test Case Example
 * @snippet test_init_deinit.cpp InitDeinit_TC_10_001_011
 * @snippet test_init_deinit.cpp osal_test_init_deinit_thread_receive_timeperiod
 */
// [InitDeinit_TC_10_001_011]
TEST_F(InitDeinitApiTest, InitDeinit_TC_10_001_011)
{
    osal_thread_id_t thread_id = TEST_THREAD_ID1;
    osal_mq_id_t mq_id = TEST_MQ_ID1;
    st_osal_thread_config_t config;
    osal_thread_handle_t thread_handle = OSAL_THREAD_HANDLE_INVALID;
    osal_mq_handle_t mq_handle = OSAL_MQ_HANDLE_INVALID;
    st_osal_mq_config_t mq_config;
    int64_t result = 0;
    osal_milli_sec_t time_10ms = 10;
    st_osal_test_init_deinit_receive_timeperiod_t receive_timeperiod_arg;
    bool isEmpty = false;
    osal_mq_handle_t sync_mq;
    st_osal_mq_config sync_mq_config = {10, 4};
    int sync_send_msg = 0xFFFF;
    int sync_receive_msg = 0;

    // Test10_001_011
    // @brief Abnormal: Waiting for reception with queue
    EXPECT_EQ(R_OSAL_Initialize(), OSAL_RETURN_OK) << "Test10_001_011 failed";
    EXPECT_EQ(R_OSAL_MqCreate(&sync_mq_config, SYNC_MQ_ID, &sync_mq), OSAL_RETURN_OK);
    EXPECT_NE(sync_mq, OSAL_MQ_HANDLE_INVALID);
    mq_config.max_num_msg = 2;
    mq_config.msg_size = 4;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config, mq_id, &mq_handle), OSAL_RETURN_OK) << "Test10_001_011 failed";
    EXPECT_EQ(R_OSAL_MqIsEmpty(mq_handle, &isEmpty), OSAL_RETURN_OK) << "Test10_001_011 failed";
    EXPECT_EQ(isEmpty, true) << "Test10_001_011 failed";
    EXPECT_EQ(R_OSAL_MqGetConfig(mq_handle, &mq_config), OSAL_RETURN_OK) << "Test10_001_011 failed";

    receive_timeperiod_arg.mq_handle = mq_handle;
    receive_timeperiod_arg.time = 5000;
    receive_timeperiod_arg.buffer_size = mq_config.msg_size;
    receive_timeperiod_arg.sync_mq = sync_mq;
    receive_timeperiod_arg.sync_send_msg = sync_send_msg;
    config.func       = osal_test_init_deinit_thread_receive_timeperiod;
    config.userarg    = (void *)&receive_timeperiod_arg;
    config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config.task_name  = (char *)"Test10_001_011";
    config.stack_size = THREAD_STACK_SIZE;
    EXPECT_EQ(R_OSAL_ThreadCreate(&config, thread_id, &thread_handle),OSAL_RETURN_OK) << "Test10_001_011 failed";
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &sync_receive_msg, sync_mq_config.msg_size), OSAL_RETURN_OK);
    EXPECT_EQ(sync_receive_msg, sync_send_msg);
    EXPECT_EQ(R_OSAL_ThreadSleepForTimePeriod(time_10ms), OSAL_RETURN_OK) << "Test10_001_011 failed";
    EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_BUSY) << "Test10_001_011 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result),OSAL_RETURN_OK) << "Test10_001_011 failed";
    EXPECT_EQ(result, (int64_t)OSAL_RETURN_TIME) << "Test10_001_011 failed";
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK) << "Test10_001_011 failed";
    EXPECT_EQ(R_OSAL_MqReset(sync_mq), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqDelete(sync_mq), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_OK) << "Test10_001_011 failed";
}
// [InitDeinit_TC_10_001_011]

/**
 * @page InitDeinit_TEST Initialization and deinitialization API Test
 * @section InitDeinitApiTest_InitDeinit_TC_10_001_012 InitDeinitApiTest.InitDeinit_TC_10_001_012
 * @subsection InitDeinit_TC_10_001_012_tsi Test Suite ID
 * InitDeinitApiTest
 * @subsection InitDeinit_TC_10_001_012_tci Test Case ID
 * InitDeinit_S2_A_TC_10_001_012
 * @subsection InitDeinit_TC_10_001_012_rr Related Requirement(s)
 * \#199120, \#260098
 * @subsection InitDeinit_TC_10_001_012_pp Prerequisite/Precondition
 * None
 * @subsection InitDeinit_TC_10_001_012_tcs Test Case Summary
 * This test shall verify for the function R_OSAL_Deinitialize when the OSAL Device resource is not clean in single thread.
 * - Test10_001_012: Device is not closed.
 * @subsection InitDeinit_TC_10_001_012_tpts Test Procedure/Test Steps
 * -# Call R_OSAL_Initialize to init OSAL APIs.
 * -# Open test target device by using R_OSAL_IoDeviceOpen.
 * -# Call R_OSAL_Deinitialize to de-init OSAL APIs.
 * -# Close test target device by using R_OSAL_IoDeviceClose.
 * -# Continue to call R_OSAL_Deinitialize to de-init OSAL APIs.
 * @subsection InitDeinit_TC_10_001_012_ereb Expected Result/Behavior
 * -# R_OSAL_Initialize return OSAL_RETURN_OK.
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.
 * -# R_OSAL_Deinitialize return OSAL_RETURN_BUSY.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * -# R_OSAL_Deinitialize return OSAL_RETURN_OK.
 * @subsection InitDeinit_TC_10_001_012_testcase Test Case Example
 * @snippet test_init_deinit.cpp InitDeinit_TC_10_001_012
 */
// [InitDeinit_TC_10_001_012]
TEST_F(InitDeinitApiTest, InitDeinit_TC_10_001_012)
{
    osal_device_handle_t io_handle = OSAL_DEVICE_HANDLE_INVALID;

    // Test10_001_012
    // @brief Abnormal: Device is not close
    EXPECT_EQ(R_OSAL_Initialize(), OSAL_RETURN_OK) << "Test10_001_012 failed";
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_INIT_DEINT_DEVICE_1, &io_handle), OSAL_RETURN_OK) << "Test10_001_012 failed";
    EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_BUSY) << "Test10_001_012 failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(io_handle), OSAL_RETURN_OK) << "Test10_001_012 failed";
    EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_OK) << "Test10_001_012 failed";
}
// [InitDeinit_TC_10_001_012]

/**
 * @page InitDeinit_TEST Initialization and deinitialization API Test
 * @section InitDeinitApiTest_InitDeinit_TC_10_001_013 InitDeinitApiTest.InitDeinit_TC_10_001_013
 * @subsection InitDeinit_TC_10_001_013_tsi Test Suite ID
 * InitDeinitApiTest
 * @subsection InitDeinit_TC_10_001_013_tci Test Case ID
 * InitDeinit_S4_A_TC_10_001_013
 * @subsection InitDeinit_TC_10_001_013_rr Related Requirement(s)
 * \#199120, \#260098
 * @subsection InitDeinit_TC_10_001_013_pp Prerequisite/Precondition
 * None
 * @subsection InitDeinit_TC_10_001_013_tcs Test Case Summary
 * This test shall verify for the function R_OSAL_Deinitialize when the OSAL Interrupt resource is not released in multi thread.
 * - Test10_001_013: Interrupt is not unregistered.
 * @subsection InitDeinit_TC_10_001_013_tpts Test Procedure/Test Steps
 * -# Call R_OSAL_Initialize to init OSAL APIs.
 * -# Open test target device by using R_OSAL_IoDeviceOpen.
 * -# Register an interrupt callback by using R_OSAL_InterruptRegisterIsr with the arguments as below:
 *      - Device handle of test target device
 *      - Interrupt channel 0
 *      - Priority is OSAL_INTERRUPT_PRIORITY_TYPE0
 *      - Callback function is "osal_init_deinit_test_isr_cb_sample"
 *      - Address of "arg"
 * -# Call R_OSAL_Deinitialize to de-init OSAL APIs.
 * -# Unregister interrupt callback by using to R_OSAL_InterruptUnregisterIsr with the argument as below:
 *      - Device handle of test target device
 *      - Interrupt channel 0
 *      - Callback function is "osal_init_deinit_test_isr_cb_sample"
 * -# Close test target device by using R_OSAL_IoDeviceClose.
 * -# Continue to call R_OSAL_Deinitialize to de-init OSAL APIs.
 * @subsection InitDeinit_TC_10_001_013_ereb Expected Result/Behavior
 * -# R_OSAL_Initialize return OSAL_RETURN_OK.
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK.
 * -# R_OSAL_Deinitialize return OSAL_RETURN_BUSY.
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * -# R_OSAL_Deinitialize return OSAL_RETURN_OK.
 * @subsection InitDeinit_TC_10_001_013_testcase Test Case Example
 * @snippet test_init_deinit.cpp InitDeinit_TC_10_001_013
 */
// [InitDeinit_TC_10_001_013]
TEST_F(InitDeinitApiTest, InitDeinit_TC_10_001_013)
{
    osal_device_handle_t io_handle = OSAL_DEVICE_HANDLE_INVALID;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    int arg = 10;

    // Test10_001_013
    // @brief Abnormal: Interrupts are not unregister.
    EXPECT_EQ(R_OSAL_Initialize(), OSAL_RETURN_OK) << "Test10_001_013 failed";
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_INIT_DEINT_DEVICE_1, &io_handle), OSAL_RETURN_OK) << "Test10_001_013 failed";
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(io_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, priority, osal_init_deinit_test_isr_cb_sample, (void *)&arg), OSAL_RETURN_OK) << "Test10_001_013 failed";
    EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_BUSY) << "Test10_001_013 failed";
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(io_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, osal_init_deinit_test_isr_cb_sample), OSAL_RETURN_OK) << "Test10_001_013 failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(io_handle), OSAL_RETURN_OK) << "Test10_001_013 failed";
    EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_OK) << "Test10_001_013 failed";
}
// [InitDeinit_TC_10_001_013]

/**
 * @page InitDeinit_TEST Initialization and deinitialization API Test
 * @section InitDeinitApiTest_InitDeinit_TC_10_001_014 InitDeinitApiTest.InitDeinit_TC_10_001_014
 * @subsection InitDeinit_TC_10_001_014_tsi Test Suite ID
 * InitDeinitApiTest
 * @subsection InitDeinit_TC_10_001_014_tci Test Case ID
 * InitDeinit_S2_A_TC_10_001_014
 * @subsection InitDeinit_TC_10_001_014_rr Related Requirement(s)
 * \#199120, \#260098
 * @subsection InitDeinit_TC_10_001_014_pp Prerequisite/Precondition
 * None
 * @subsection InitDeinit_TC_10_001_014_tcs Test Case Summary
 * This test shall verify for the function R_OSAL_Deinitialize when the OSAL Power resource is not released in multi thread.
 * - Test10_001_014: Under waiting for a Power Manager state.
 * @subsection InitDeinit_TC_10_001_014_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_Initialize.
 * -# Create synchronization queue by using R_OSAL_MqCreate
 * -# Open device test target device by callung function R_OSAL_IoDeviceOpen.
 * -# Create a mutex by calling R_OSAL_ThsyncMutexCreate
 * -# Create a thread using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *           -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *           -# task_name is "Test10_001_014"
 *           -# func is osal_test_init_deinit_pm_waitstate with functionality:
 *              - Get the synchronization queue config by using R_OSAL_MqGetConfig
 *              - Lock the mutex by calling R_OSAL_ThsyncMutexLockForTimePeriod.
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *              - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by calling R_OSAL_PmSetRequiredState with applyImmediate is true
 *              - Set policy to OSAL_PM_POLICY_HP by calling R_OSAL_PmSetPolicy.
 *              - Unlock the mutex by calling R_OSAL_ThsyncMutexUnlock.
 *              - Wait for OSAL_PM_STATE_PG state by calling R_OSAL_PmWaitForState.
 *           -# userarg is &io_pm_arg included:
 *              - IO handle
 *              - mutex handle
 *              - time is 2000
 *              - sync_mq is handle of synchronization queue
 *              - sync_send_msg is the sending message for synchronization purpose
 *           -# stack_size is THREAD_STACK_SIZE
 *      - uid is TEST_THREAD_ID1.
 *      - Address of thread handle.
 * -# Receive message from synchronization queue by using R_OSAL_MqReceiveForTimePeriod
 * -# Sleep for 10ms to make sure new thread has already been blocked by R_OSAL_PmWaitForState
 * -# Lock the mutex by calling R_OSAL_ThsyncMutexLockForTimePeriod.
 * -# Sleep for 1ms by calling R_OSAL_ThreadSleepForTimePeriod to make sure R_OSAL_PmWaitForState in new thread was called first
 * -# Call function R_OSAL_Deinitialize.
 * -# Unlock the mutex by calling R_OSAL_ThsyncMutexUnlock.
 * -# Call function R_OSAL_ThreadJoin.
 * -# Call function R_OSAL_IoDeviceClose.
 * -# Destroy the mutex by calling R_OSAL_ThsyncMutexDestroy.
 * -# Reset synchronization queue by using R_OSAL_MqReset
 * -# Delete synchronization queue by using R_OSAL_MqDelete
 * -# Call function R_OSAL_Deinitialize.
 * @subsection InitDeinit_TC_10_001_014_ereb Expected Result/Behavior
 * -# Function R_OSAL_Initialize return OSAL_RETURN_OK.
 * -# Function R_OSAL_MqCreate return OSAL_RETURN_OK.
 * -# Function R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.
 * -# Function R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.
 * -# Function R_OSAL_ThreadCreate return OSAL_RETURN_OK. <br>
 * Thread handle is not nulptr. <br>
 * In osal_test_init_deinit_pm_waitstate function: <br>
 *  - Function R_OSAL_MqGetConfig return OSAL_RETURN_OK.
 *  - Function R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 *  - Function R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK.
 *  - Function R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 *  - Function R_OSAL_PmSetPolicy return OSAL_RETURN_OK.
 *  - Function R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 *  - Function R_OSAL_PmWaitForState return OSAL_RETURN_TIME.
 * -# Function R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK.
 * -# Function R_OSAL_ThreadSleepForTimePeriod return OSAL_RETURN_OK.
 * -# Function R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 * -# Function R_OSAL_ThreadSleepForTimePeriod return OSAL_RETURN_OK.
 * -# Function R_OSAL_Deinitialize return OSAL_RETURN_BUSY.
 * -# Function R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# Function R_OSAL_ThreadJoin return OSAL_RETURN_OK.<br>
 * The returned result is (int64_t)OSAL_RETURN_TIME.
 * -# Function R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * -# Function R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * -# Function R_OSAL_MqReset return OSAL_RETURN_OK.
 * -# Function R_OSAL_MqDelete return OSAL_RETURN_OK.
 * -# Function R_OSAL_Deinitialize return OSAL_RETURN_OK.
 * @subsection InitDeinit_TC_10_001_014_testcase Test Case Example
 * @snippet test_init_deinit.cpp InitDeinit_TC_10_001_014
 * @snippet test_init_deinit.cpp osal_test_init_deinit_pm_waitstate
 */
// [InitDeinit_TC_10_001_014]
TEST_F(InitDeinitApiTest, InitDeinit_TC_10_001_014)
{
    osal_thread_id_t thread_id = TEST_THREAD_ID1;
    osal_mutex_id_t mutex_id = TEST_MUTEX_ID1;
    st_osal_thread_config_t config;
    osal_thread_handle_t thread_handle = OSAL_THREAD_HANDLE_INVALID;
    osal_mutex_handle_t mutex_handle = OSAL_MUTEX_HANDLE_INVALID;
    int64_t result = 0;
    osal_milli_sec_t time_100ms = 100;
    osal_milli_sec_t time_10ms = 10;
    osal_milli_sec_t time_1ms = 1;
    osal_device_handle_t io_handle = OSAL_DEVICE_HANDLE_INVALID;
    st_osal_test_init_deinit_pm_t io_pm_arg;
    osal_mq_handle_t sync_mq;
    st_osal_mq_config sync_mq_config = {10, 4};
    int sync_send_msg = 0xFFFF;
    int sync_receive_msg = 0;

    // Test10_001_014
    // @brief Abnormal: Waiting at PM
    EXPECT_EQ(R_OSAL_Initialize(), OSAL_RETURN_OK) << "Test10_001_014 failed";
    EXPECT_EQ(R_OSAL_MqCreate(&sync_mq_config, SYNC_MQ_ID, &sync_mq), OSAL_RETURN_OK);
    EXPECT_NE(sync_mq, OSAL_MQ_HANDLE_INVALID);
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_INIT_DEINT_DEVICE_2, &io_handle), OSAL_RETURN_OK) << "Test10_001_014 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle), OSAL_RETURN_OK) << "Test10_001_014 failed";

    io_pm_arg.io_handle = io_handle;
    io_pm_arg.mutex_handle = mutex_handle;
    io_pm_arg.time = 2000;
    io_pm_arg.sync_mq = sync_mq;
    io_pm_arg.sync_send_msg = sync_send_msg;
    config.func       = osal_test_init_deinit_pm_waitstate;
    config.userarg    = &io_pm_arg;
    config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config.task_name  = (char *)"Test10_001_014";
    config.stack_size = THREAD_STACK_SIZE;

    EXPECT_EQ(R_OSAL_ThreadCreate(&config, thread_id, &thread_handle),OSAL_RETURN_OK) << "Test10_001_014 failed";
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &sync_receive_msg, sync_mq_config.msg_size), OSAL_RETURN_OK);
    EXPECT_EQ(sync_receive_msg, sync_send_msg);
    EXPECT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(mutex_handle, time_100ms), OSAL_RETURN_OK) << "Test10_001_014 failed";
    EXPECT_EQ(R_OSAL_ThreadSleepForTimePeriod(time_1ms), OSAL_RETURN_OK) << "Test10_001_014 failed";
    EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_BUSY) << "Test10_001_014 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(mutex_handle), OSAL_RETURN_OK) << "Test10_001_014 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result),OSAL_RETURN_OK) << "Test10_001_014 failed";
    EXPECT_EQ(result, (int64_t)OSAL_RETURN_TIME) << "Test10_001_014 failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(io_handle), OSAL_RETURN_OK) << "Test10_001_014 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mutex_handle), OSAL_RETURN_OK) << "Test10_001_014 failed";
    EXPECT_EQ(R_OSAL_MqReset(sync_mq), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqDelete(sync_mq), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_OK) << "Test10_001_014 failed";
}
// [InitDeinit_TC_10_001_014]

/**
 * @page InitDeinit_TEST Initialization and deinitialization API Test
 * @section InitDeinitApiTest_InitDeinit_TC_10_001_015 InitDeinitApiTest.InitDeinit_TC_10_001_015
 * @subsection InitDeinit_TC_10_001_015_tsi Test Suite ID
 * InitDeinitApiTest
 * @subsection InitDeinit_TC_10_001_015_tci Test Case ID
 * InitDeinit_S1_N_TC_10_001_015
 * @subsection InitDeinit_TC_10_001_015_rr Related Requirement(s)
 * \#260098
 * @subsection InitDeinit_TC_10_001_015_pp Prerequisite/Precondition
 * OSAL is initialized
 * @subsection InitDeinit_TC_10_001_015_tcs Test Case Summary
 * This test shall verify the functionality of the OSAL APIs in case memory resource is not clean
 * - Test10_001_015: Deinitialize should be success without Dealloc and Close, and Re-Initialize is success.
 * @subsection InitDeinit_TC_10_001_015_tpts Test Procedure/Test Steps
 * -# Init OSAL APIs by using R_OSAL_Initialize
 * -# Init memory configuration object by using R_OSAL_MmngrInitializeMmngrConfigSt
 * -# Get the memory max configuration by using R_OSAL_MmngrGetOsalMaxConfig
 * -# Open memory manager with the max config by uisng R_OSAL_MmngrOpen
 * -# Allocate buffer with size is 64 and alignment is 64 by using R_OSAL_MmngrAlloc
 * -# Deinit OSAL APIs by using R_OSAL_Deinitialize
 * @subsection InitDeinit_TC_10_001_015_ereb Expected Result/Behavior
 * -# R_OSAL_Initialize return OSAL_RETURN_OK
 * -# R_OSAL_MmngrInitializeMmngrConfigSt return OSAL_RETURN_OK
 * -# R_OSAL_MmngrGetOsalMaxConfig return OSAL_RETURN_OK
 * -# R_OSAL_MmngrOpen return OSAL_RETURN_OK.<br>
 * Value of "hndl_mmngr" is not OSAL_MEMORY_MANAGER_HANDLE_INVALID
 * -# R_OSAL_MmngrAlloc return OSAL_RETURN_OK.<br>
 * Value of "buffer_obj" is not OSAL_MEMORY_BUFFER_HANDLE_INVALID
 * -# R_OSAL_Deinitialize return OSAL_RETURN_OK
 * @subsection InitDeinit_TC_10_001_015_testcase Test Case Example
 * @snippet test_init_deinit.cpp InitDeinit_TC_10_001_015
 */
// [InitDeinit_TC_10_001_015]
TEST_F(InitDeinitApiTest, InitDeinit_TC_10_001_015)
{
    st_osal_mmngr_config_t       config;
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    osal_memory_buffer_handle_t  buffer_obj = OSAL_MEMORY_BUFFER_HANDLE_INVALID;
    size_t size;
    size_t alignment;

    // Test10_001_015
    // Deinitialize should be success without Dealloc and Close, and Re-Initialize is success.
    EXPECT_EQ(R_OSAL_Initialize(), OSAL_RETURN_OK) << "Test10_001_015 failed";
    EXPECT_EQ(R_OSAL_MmngrInitializeMmngrConfigSt(&config), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MmngrGetOsalMaxConfig(&config), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MmngrOpen(&config, &hndl_mmngr), OSAL_RETURN_OK);
    EXPECT_NE(hndl_mmngr, OSAL_MEMORY_MANAGER_HANDLE_INVALID);
    size = 64;
    alignment = 64;
    EXPECT_EQ(R_OSAL_MmngrAlloc(hndl_mmngr, size, alignment, &buffer_obj), OSAL_RETURN_OK);
    EXPECT_NE(buffer_obj, OSAL_MEMORY_BUFFER_HANDLE_INVALID);
    EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_OK) << "Test10_001_015 failed"
    << R_OSAL_MmngrDealloc(hndl_mmngr, buffer_obj)
    << R_OSAL_MmngrClose(hndl_mmngr)
    << R_OSAL_Deinitialize();
}
// [InitDeinit_TC_10_001_015]

/**
 * @page MM_API Memory API Test
 * @section InitDeinitApiTest_InitDeinit_TC_10_001_016 InitDeinitApiTest.InitDeinit_TC_10_001_016
 * @subsection InitDeinit_TC_10_001_016_tsi Test Suite ID
 * InitDeinitApiTest
 * @subsection InitDeinit_TC_10_001_016_tci Test Case ID
 * InitDeinit_S1_N_TC_10_001_016
 * @subsection InitDeinit_TC_10_001_016_rr Related Requirement(s)
 * \#260098
 * @subsection InitDeinit_TC_10_001_016_pp Prerequisite/Precondition
 * OSAL is initialized
 * @subsection InitDeinit_TC_10_001_016_tcs Test Case Summary
 * This test shall verify the functionality of the OSAL APIs in case memory resource is not clean
 * - Test10_001_016: Deinitialize should be success without Dealloc and Re-Initialize is success.
 * @subsection InitDeinit_TC_10_001_016_tpts Test Procedure/Test Steps
 * -# Init OSAL APIs by using R_OSAL_Initialize
 * -# Init memory configuration object by using R_OSAL_MmngrInitializeMmngrConfigSt
 * -# Get the memory max configuration by using R_OSAL_MmngrGetOsalMaxConfig
 * -# Open memory manager with the max config by uisng R_OSAL_MmngrOpen
 * -# Allocate buffer with size is 64 and alignment is 64 by using R_OSAL_MmngrAlloc
 * -# Close memory manager by using R_OSAL_MmngrClose
 * -# Deinit OSAL APIs by using R_OSAL_Deinitialize
 * @subsection InitDeinit_TC_10_001_016_ereb Expected Result/Behavior
 * -# R_OSAL_Initialize return OSAL_RETURN_OK
 * -# R_OSAL_MmngrInitializeMmngrConfigSt return OSAL_RETURN_OK
 * -# R_OSAL_MmngrGetOsalMaxConfig return OSAL_RETURN_OK
 * -# R_OSAL_MmngrOpen return OSAL_RETURN_OK.<br>
 * Value of "hndl_mmngr" is not OSAL_MEMORY_MANAGER_HANDLE_INVALID
 * -# R_OSAL_MmngrAlloc return OSAL_RETURN_OK.<br>
 * Value of "buffer_obj" is not OSAL_MEMORY_BUFFER_HANDLE_INVALID
 * -# R_OSAL_MmngrClose return OSAL_RETURN_OK
 * -# R_OSAL_Deinitialize return OSAL_RETURN_OK
 * @subsection InitDeinit_TC_10_001_016_testcase Test Case Example
 * @snippet test_init_deinit.cpp InitDeinit_TC_10_001_016
 */
// [InitDeinit_TC_10_001_016]
TEST_F(InitDeinitApiTest, InitDeinit_TC_10_001_016)
{
    st_osal_mmngr_config_t       config;
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    osal_memory_buffer_handle_t  buffer_obj = OSAL_MEMORY_BUFFER_HANDLE_INVALID;
    size_t size;
    size_t alignment;

    // Test10_001_016
    // Deinitialize should be success without Dealloc and Re-Initialize is success.
    EXPECT_EQ(R_OSAL_Initialize(), OSAL_RETURN_OK) << "Test10_001_016 failed";
    EXPECT_EQ(R_OSAL_MmngrInitializeMmngrConfigSt(&config), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MmngrGetOsalMaxConfig(&config), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MmngrOpen(&config, &hndl_mmngr), OSAL_RETURN_OK);
    size = 64;
    alignment = 64;
    EXPECT_NE(hndl_mmngr, OSAL_MEMORY_MANAGER_HANDLE_INVALID);
    EXPECT_EQ(R_OSAL_MmngrAlloc(hndl_mmngr, size, alignment, &buffer_obj), OSAL_RETURN_OK);
    EXPECT_NE(buffer_obj, OSAL_MEMORY_BUFFER_HANDLE_INVALID);
    EXPECT_EQ(R_OSAL_MmngrClose(hndl_mmngr), OSAL_RETURN_OK) << "Test10_001_016 failed";
    EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_OK) << "Test10_001_016 failed";
}
// [InitDeinit_TC_10_001_016]
