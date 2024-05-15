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
* File Name :    test_thread.cpp
* Version :      3.2.2
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Thread Manager API Functional Test (From the perspective of OSAL API Specification)
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           2.0.0     29.10.2021    Create New
 *           2.0.1     18.11.2021    Fix Typo
 *           2.0.2     21.01.2022    Update Test Sequence of ThreadEqual_TC_01_005_002
 *           2.0.3     11.03.2022    Remove "#if defined" used to disable test on SIL
 *           3.0.0     23.03.2022    Add ThreadSelf_TC_01_004_004
 *           3.0.1     04.04.2022    Update synchronization method for ThreadEqual_TC_01_005_001
 *           3.1.0     04.01.2023    Add ThreadCreate_TC_01_001_012
 *                                   Add handle value initialize for below tests:
 *                                      - ThreadCreate_TC_01_001_005
 *                                      - ThreadCreate_TC_01_001_007
 *                                      - ThreadCreate_TC_01_001_008
 *                                      - ThreadCreate_TC_01_001_010
 *                                      - ThreadInitializeThreadConfigSt_TC_01_007_003
 *           3.2.0     01.02.2023    Add new boundary test cases as below:
 *                                      - ThreadSleepUntilTimeStamp_TC_01_002_010
 *                                      - ThreadSleepUntilTimeStamp_TC_01_002_011
 *                                      - ThreadSleepUntilTimeStamp_TC_01_002_012
 *                                      - ThreadSleepUntilTimeStamp_TC_01_002_013
 *           3.2.1     10.04.2023    Update the thread synchronization for test ThreadJoin_TC_01_006_006
 *                                   Update expected value for median time for SIL Windows performance test
 *           3.2.2     17.05.2023    Update wait time of ThreadJoin_TC_01_006_006
***********************************************************************************************************************/
#include "test_common.hpp"
#if !defined(OSAL_TEST_FRAMEWORK)
using namespace std;
#else
using namespace OsalFWHelper;
#endif

/* armclang will be warnig, when specified OsalApiStateTest to TEST_F */
/* W.A Derive a class and replace it by define */
class OsalApiStateTest_TH : public OsalApiStateTest
{
    /* There is no need for member variables and member functions.
       We added them just for the sake of adding a test class. */
};
#define OsalApiStateTest OsalApiStateTest_TH

/**
 * @page Thread_TEST Thread API Test
 * @test
 * @test @link ThreadApiTest_ThreadCreate_TC_01_001_001 @endlink
 * @test @link ThreadApiTest_ThreadCreate_TC_01_001_002 @endlink
 * @test @link ThreadApiTest_ThreadCreate_TC_01_001_003 @endlink
 * @test @link ThreadApiTest_ThreadCreate_TC_01_001_004 @endlink
 * @test @link ThreadApiTest_ThreadCreate_TC_01_001_005 @endlink
 * @test @link ThreadApiTest_ThreadCreate_TC_01_001_006 @endlink
 * @test @link ThreadApiTest_ThreadCreate_TC_01_001_007 @endlink
 * @test @link ThreadApiTest_ThreadCreate_TC_01_001_008 @endlink
 * @test @link ThreadApiTest_ThreadCreate_TC_01_001_009 @endlink
 * @test @link OsalApiStateTest_ThreadCreate_TC_01_001_010 @endlink
 * @test @link ThreadApiTest_ThreadCreate_TC_01_001_011 @endlink
 * @test @link ThreadApiTest_ThreadCreate_TC_01_001_012 @endlink
 * @test @link ThreadApiTest_ThreadSleepUntilTimeStamp_TC_01_002_001 @endlink
 * @test @link ThreadApiTest_ThreadSleepUntilTimeStamp_TC_01_002_002 @endlink
 * @test @link ThreadApiTest_ThreadSleepUntilTimeStamp_TC_01_002_003 @endlink
 * @test @link ThreadApiTest_ThreadSleepUntilTimeStamp_TC_01_002_004 @endlink
 * @test @link ThreadApiTest_ThreadSleepUntilTimeStamp_TC_01_002_005 @endlink
 * @test @link ThreadApiTest_ThreadSleepUntilTimeStamp_TC_01_002_006 @endlink
 * @test @link ThreadApiTest_ThreadSleepUntilTimeStamp_TC_01_002_007 @endlink
 * @test @link OsalApiStateTest_ThreadSleepUntilTimeStamp_TC_01_002_008 @endlink
 * @test @link ThreadApiTest_ThreadSleepUntilTimeStamp_TC_01_002_009 @endlink
 * @test @link ThreadApiTest_ThreadSleepUntilTimeStamp_TC_01_002_010 @endlink
 * @test @link ThreadApiTest_ThreadSleepUntilTimeStamp_TC_01_002_011 @endlink
 * @test @link ThreadApiTest_ThreadSleepUntilTimeStamp_TC_01_002_012 @endlink
 * @test @link ThreadApiTest_ThreadSleepUntilTimeStamp_TC_01_002_013 @endlink
 * @test @link ThreadApiTest_ThreadSleepForTimePeriod_TC_01_003_001 @endlink
 * @test @link ThreadApiTest_ThreadSleepForTimePeriod_TC_01_003_002 @endlink
 * @test @link ThreadApiTest_ThreadSleepForTimePeriod_TC_01_003_003 @endlink
 * @test @link OsalApiStateTest_ThreadSleepForTimePeriod_TC_01_003_004 @endlink
 * @test @link ThreadApiTest_ThreadSleepForTimePeriod_TC_01_003_005 @endlink
 * @test @link ThreadApiTest_ThreadSleepForTimePeriod_TC_01_003_006 @endlink
 * @test @link ThreadApiTest_ThreadSelf_TC_01_004_001 @endlink
 * @test @link ThreadApiTest_ThreadSelf_TC_01_004_002 @endlink
 * @test @link OsalApiStateTest_ThreadSelf_TC_01_004_003 @endlink
 * @test @link ThreadApiTest_ThreadSelf_TC_01_004_004 @endlink
 * @test @link ThreadApiTest_ThreadEqual_TC_01_005_001 @endlink
 * @test @link ThreadApiTest_ThreadEqual_TC_01_005_002 @endlink
 * @test @link ThreadApiTest_ThreadEqual_TC_01_005_003 @endlink
 * @test @link ThreadApiTest_ThreadEqual_TC_01_005_004 @endlink
 * @test @link ThreadApiTest_ThreadEqual_TC_01_005_005 @endlink
 * @test @link OsalApiStateTest_ThreadEqual_TC_01_005_006 @endlink
 * @test @link ThreadApiTest_ThreadJoin_TC_01_006_001 @endlink
 * @test @link ThreadApiTest_ThreadJoin_TC_01_006_002 @endlink
 * @test @link ThreadApiTest_ThreadJoin_TC_01_006_003 @endlink
 * @test @link ThreadApiTest_ThreadJoin_TC_01_006_004 @endlink
 * @test @link ThreadApiTest_ThreadJoin_TC_01_006_005 @endlink
 * @test @link ThreadApiTest_ThreadJoin_TC_01_006_006 @endlink
 * @test @link OsalApiStateTest_ThreadJoin_TC_01_006_007 @endlink
 * @test @link ThreadApiTest_ThreadInitializeThreadConfigSt_TC_01_007_001 @endlink
 * @test @link ThreadApiTest_ThreadInitializeThreadConfigSt_TC_01_007_002 @endlink
 * @test @link ThreadApiTest_ThreadInitializeThreadConfigSt_TC_01_007_003 @endlink
 * @section ThreadApiTest ThreadApiTest function
 * @subsection ThreadApiTest_SetUp ThreadApiTest Setup
 * Common to Test Suit of ThreadApiTest, prepare process to be executed before executing the test. <br>
 * For example:
 * @snippet test_thread.cpp ThreadApiTest_SetUp
 * @subsection ThreadApiTest_TearDown ThreadApiTest TearDown
 * Common to Test Suit of ThreadApiTest, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_thread.cpp ThreadApiTest_TearDown
 * @section OSALINTTESTS_ThreadApiTest OSALINTTESTS of ThreadApiTest function
 * @subsection OSALINTTESTS_ThreadApiTest_SetUp OSALINTTESTS of ThreadApiTest Setup
 * Common to Test Suit of OSALINTTESTS ThreadApiTest, prepare process to be executed before executing the test. <br>
 * This test suite generates interrupts. These process is preparation<br>
 * For example:
 * @snippet test_common.hpp OSALINTTESTS_SetUp
 * @subsection OSALINTTESTS_ThreadApiTest_TearDown OSALINTTESTS of ThreadApiTest TearDown
 * Common to Test Suit of OSALINTTESTS ThreadApiTest, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_common.hpp OSALINTTESTS_TearDown
 * @section OsalApiStateTest_ThreadApiTest OsalApiStateTest of ThreadApiTest function
 * @subsection OsalApiStateTest_ThreadApiTest_SetUp OsalApiStateTest of ThreadApiTest Setup
 * Common to Test Suit of OsalApiStateTest ThreadApiTest, prepare process to be executed before executing the test. <br>
 * For example:
 * @snippet test_common.hpp OsalApiStateTest_SetUp
 * @subsection OsalApiStateTest_ThreadApiTest_TearDown OsalApiStateTest of ThreadApiTest TearDown
 * Common to Test Suit of OsalApiStateTest ThreadApiTest, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_common.hpp OsalApiStateTest_TearDown
 */
class ThreadApiTest : public OSALTESTS
{
protected:
    /* RVC implementation base on Linux OSAL IT*/
    osal_thread_handle_t thread_handle = OSAL_THREAD_HANDLE_INVALID;
    osal_thread_handle_t thread_handle1 = OSAL_THREAD_HANDLE_INVALID;
    osal_thread_handle_t thread_handle2 = OSAL_THREAD_HANDLE_INVALID;
    osal_mq_handle_t sync_mq;
    st_osal_mq_config mq_config = {10, 4};
    int send_msg = 0xFFFF;
    int receive_msg = 0;

// [ThreadApiTest_SetUp]
    void SetUp() override{
        EXPECT_EQ(R_OSAL_MqCreate(&mq_config, SYNC_MQ_ID, &sync_mq), OSAL_RETURN_OK);
        EXPECT_NE(sync_mq, OSAL_MQ_HANDLE_INVALID);
        EXPECT_EQ(R_OSAL_MqReset(sync_mq), OSAL_RETURN_OK);
    }
// [ThreadApiTest_SetUp]

// [ThreadApiTest_TearDown]
    void TearDown() override{
        EXPECT_EQ(R_OSAL_MqReset(sync_mq), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_MqDelete(sync_mq), OSAL_RETURN_OK);
    }
// [ThreadApiTest_TearDown]
};

/* RVC implementation base on Linux OSAL IT*/
struct threadequal_worker_arg
{
    threadequal_worker_arg(osal_thread_handle_t *handle1, osal_thread_handle_t *handle2) : first_handle(handle1), second_handle(handle2)
    {
    }
    osal_thread_handle_t * first_handle;
    osal_thread_handle_t * second_handle;
    osal_mq_handle_t mq;
    int send_msg;
};
struct threadjoin_worker_arg
{
    threadjoin_worker_arg(osal_thread_handle_t *handle) : th_handle(handle)
    {
    }
    osal_thread_id_t uid;
    osal_thread_handle_t * th_handle;
    int64_t return_value1;
    int64_t return_value2;
    osal_cond_handle_t cond_handle = OSAL_COND_HANDLE_INVALID;
    osal_mutex_handle_t mutex_handle = OSAL_MUTEX_HANDLE_INVALID;
    osal_mq_handle_t mq;
    int send_msg;
};

typedef struct st_threadequal_isr_arg
{
    osal_thread_handle_t thr_handle1;
    osal_thread_handle_t thr_handle2;
    osal_thread_handle_t* self_thr_handle1;
} st_threadequal_isr_arg_t;


// [simple_workerfunction]
int64_t simple_workerfunction(void * arg)
{
    (void)arg;
    osal_milli_sec_t time = 500;
    //std::cout << "Simple worker function is created" << std::endl;
    EXPECT_EQ(R_OSAL_ThreadSleepForTimePeriod(time), OSAL_RETURN_OK);
    return (int64_t) 0;
}
// [simple_workerfunction]

// [threadself_workerfunction]
int64_t threadself_workerfunction(void * arg)
{
    osal_thread_handle_t threadself_handle;

// Test01_004_001
// @brief: normal case: all input/output param is valid
    EXPECT_EQ(R_OSAL_ThreadSelf(&threadself_handle), OSAL_RETURN_OK)  << "Test01_004_001 failed";
    return (int64_t) 0;
}
// [threadself_workerfunction]

// [threadequal_workerfunction1]
int64_t threadequal_workerfunction1(void * arg)
{
    threadequal_worker_arg *testStruct = (threadequal_worker_arg *)arg;
    osal_thread_handle_t threadself_handle;
    bool                 same;
    int receive_msg = 0;
    st_osal_mq_config local_mq_config;

// Test01_005_001
// @brief: normal case: checking with the same thread handle
    EXPECT_EQ(R_OSAL_MqGetConfig(testStruct->mq, &local_mq_config), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(testStruct->mq, SYNC_MQ_TIME, &receive_msg, local_mq_config.msg_size), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, testStruct->send_msg);
    EXPECT_EQ(R_OSAL_ThreadSelf(&threadself_handle), OSAL_RETURN_OK) << "Test01_005_001 failed";
    EXPECT_EQ(R_OSAL_ThreadEqual(threadself_handle, *(testStruct->second_handle), &same), OSAL_RETURN_OK) << "Test01_005_001 failed";
    EXPECT_TRUE(same) << "Test01_005_001 failed";
    return (int64_t) 0;
}
// [threadequal_workerfunction1]

// [threadequal_workerfunction2]
int64_t threadequal_workerfunction2(void * arg)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    return (int64_t) osal_ret;
}
// [threadequal_workerfunction2]

// [threadjoin_workerfunction_joinitself]
int64_t threadjoin_workerfunction_joinitself(void * arg)
{
    osal_thread_handle_t threadself_handle;
    int64_t result;
    EXPECT_EQ(R_OSAL_ThreadSelf(&threadself_handle), OSAL_RETURN_OK) << "Test01_006_003 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(threadself_handle, &result), OSAL_RETURN_HANDLE)  << "Test01_006_003 failed";
    return (int64_t) 0;
}
// [threadjoin_workerfunction_joinitself]

// [threadjoin_workerfunction_joinedthread1]
int64_t threadjoin_workerfunction_joinedthread1(void * arg)
{
    threadjoin_worker_arg *testStruct = (threadjoin_worker_arg *)arg;
    return (int64_t) R_OSAL_ThreadJoin(*(testStruct->th_handle), &testStruct->return_value1);
}
// [threadjoin_workerfunction_joinedthread1]

// [threadjoin_workerfunction_joinedthread2]
int64_t threadjoin_workerfunction_joinedthread2(void * arg)
{
    threadjoin_worker_arg *testStruct = (threadjoin_worker_arg *)arg;
    return (int64_t) R_OSAL_ThreadJoin(*(testStruct->th_handle), &testStruct->return_value2);
}
// [threadjoin_workerfunction_joinedthread2]

// [threadjoin_workerfunction_thread_handle]
int64_t threadjoin_workerfunction_thread_handle(void * arg)
{
    threadjoin_worker_arg *testStruct = (threadjoin_worker_arg *)arg;
    osal_milli_sec_t time = 1000;
    int64_t result;
    st_osal_mq_config local_mq_config;

    EXPECT_EQ(R_OSAL_ThsyncMutexTryLock(testStruct->mutex_handle), OSAL_RETURN_OK) << "Test01_006_006 failed";
    EXPECT_EQ(R_OSAL_MqGetConfig(testStruct->mq, &local_mq_config), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(testStruct->mq, SYNC_MQ_TIME, &testStruct->send_msg, local_mq_config.msg_size), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncCondWaitForTimePeriod(testStruct->cond_handle, testStruct->mutex_handle, time), OSAL_RETURN_OK) << "Test01_006_006 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(testStruct->mutex_handle), OSAL_RETURN_OK) << "Test01_006_006 failed";
    EXPECT_NE(*(testStruct->th_handle), NULL_PTR) << "Test01_006_006 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(*(testStruct->th_handle), &result), OSAL_RETURN_OK) << "Test01_006_006 failed";
    EXPECT_EQ(result, 0)<< "Test01_006_006 failed";
    return (int64_t) 0;
}
// [threadjoin_workerfunction_thread_handle]

// [threadjoin_workerfunction_thread_handle1]
int64_t threadjoin_workerfunction_thread_handle1(void * arg)
{
    threadjoin_worker_arg *testStruct = (threadjoin_worker_arg *)arg;
    osal_milli_sec_t time = 10;
    osal_thread_id_t uid = testStruct->uid;
    st_osal_thread_config_t config;

    config.priority  = OSAL_THREAD_PRIORITY_TYPE5;
    config.task_name = (char *)"Simple testing thread";
    config.func      = simple_workerfunction;
    config.userarg   = NULL;
    config.stack_size = THREAD_STACK_SIZE;

    EXPECT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(testStruct->mutex_handle, time), OSAL_RETURN_OK) << "Test01_006_006 failed";
    EXPECT_EQ(R_OSAL_ThreadCreate(&config, uid, testStruct->th_handle), OSAL_RETURN_OK) << "Test01_006_006 failed";
    EXPECT_NE(*(testStruct->th_handle), NULL_PTR) << "Test01_006_006 failed";
    EXPECT_EQ(R_OSAL_ThsyncCondSignal(testStruct->cond_handle), OSAL_RETURN_OK) << "Test01_006_006 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(testStruct->mutex_handle), OSAL_RETURN_OK) << "Test01_006_006 failed";
    return (int64_t) 0;
}
// [threadjoin_workerfunction_thread_handle1]

// [threadself_worker_isr]
int64_t threadself_worker_isr(void * arg)
{
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)arg;
    st_threadequal_isr_arg_t* threadequal_arg = (st_threadequal_isr_arg_t*)local_arg->additional_arg;
    osal_thread_handle_t threadself_handle;
    osal_milli_sec_t time = 100;

    EXPECT_EQ(R_OSAL_ThreadSleepForTimePeriod(time), OSAL_RETURN_OK); // Make a change for main thread wait the condition first
    EXPECT_EQ(R_OSAL_ThreadSelf(&threadself_handle), OSAL_RETURN_OK);
    *(threadequal_arg->self_thr_handle1) = threadself_handle;
    EXPECT_EQ(R_OSAL_ThsyncMutexTryLock(local_arg->mtx_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncCondSignal(local_arg->cond_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(local_arg->mtx_handle), OSAL_RETURN_OK);

    return (int64_t) 0;
}
// [threadself_worker_isr]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadCreate_TC_01_001_001 ThreadApiTest.ThreadCreate_TC_01_001_001
 * @subsection ThreadCreate_TC_01_001_001_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadCreate_TC_01_001_001_tci Test Case ID
 * ThreadCreate_S1_N_TC_01_001_001
 * @subsection ThreadCreate_TC_01_001_001_rr Related Requirement(s)
 * \#199129, \#199135, \#199136, \#199137, \#199138, \#294297
 * @subsection ThreadCreate_TC_01_001_001_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadCreate_TC_01_001_001_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to create a thread with normal condition:
 * - Test01_001_001: all input/output param is valid.
 * @subsection ThreadCreate_TC_01_001_001_tpts Test Procedure/Test Steps
 * -# Create a thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is simple_workerfunction function with functionality:
 *              - Suspend thread 500ms with R_OSAL_ThreadSleepForTimePeriod.
 *          -# userarg is 1
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Address of thread handle.
 * -# Join thread by using R_OSAL_ThreadJoin.
 * @subsection ThreadCreate_TC_01_001_001_ereb Expected Result/Behavior
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK.<br>
 * "thread_handle" is not NULL_PTR.
 * In simple_workerfunction function: <br>
 *  - R_OSAL_ThreadSleepForTimePeriod return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK.<br>
 * Value of "result" is equal to 0.
 * @subsection ThreadCreate_TC_01_001_001_testcase Test Case Example
 * @snippet test_thread.cpp ThreadCreate_TC_01_001_001
 * @snippet test_thread.cpp simple_workerfunction
 */
// [ThreadCreate_TC_01_001_001]
TEST_F(ThreadApiTest, ThreadCreate_TC_01_001_001)
{
    osal_thread_id_t            uid;
    st_osal_thread_config_t     config;
    int                         arg;
    int64_t                     result;

// Test01_001_001
// @brief: normal case: all input/output param is valid
    uid              = TEST_THREAD_ID1;
    arg              = 1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = (char *)"Simple testing thread";
    config.func      = simple_workerfunction;
    config.userarg   = &arg;
    config.stack_size = THREAD_STACK_SIZE;

    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK) << "Test01_001_001 failed";
    ASSERT_NE(thread_handle, NULL_PTR);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK);
    EXPECT_EQ(result, 0);
    thread_handle = OSAL_THREAD_HANDLE_INVALID;
}
// [ThreadCreate_TC_01_001_001]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadCreate_TC_01_001_002 ThreadApiTest.ThreadCreate_TC_01_001_002
 * @subsection ThreadCreate_TC_01_001_002_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadCreate_TC_01_001_002_tci Test Case ID
 * ThreadCreate_S1_N_TC_01_001_002
 * @subsection ThreadCreate_TC_01_001_002_rr Related Requirement(s)
 * \#199129, \#199132, \#199135, \#199136, \#199137, \#199138
 * @subsection ThreadCreate_TC_01_001_002_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadCreate_TC_01_001_002_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to create a thread with normal condition:
 * - Test01_001_002: func is NULL and userarg is NULL.
 * @subsection ThreadCreate_TC_01_001_002_tpts Test Procedure/Test Steps
 * -# Create a thread using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE2
 *          -# task_name is "Simple testing thread"
 *          -# func is NULL
 *          -# userarg is NULL
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Address of thread handle.
 * -# Join thread by using R_OSAL_ThreadJoin.
 * @subsection ThreadCreate_TC_01_001_002_ereb Expected Result/Behavior
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK.<br>
 * "thread_handle" is not NULL_PTR.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK.<br>
 * Value of "result" is equal to 0.
 * @subsection ThreadCreate_TC_01_001_002_testcase Test Case Example
 * @snippet test_thread.cpp ThreadCreate_TC_01_001_002
 */
// [ThreadCreate_TC_01_001_002]
TEST_F(ThreadApiTest, ThreadCreate_TC_01_001_002)
{
    osal_thread_id_t            uid;
    st_osal_thread_config_t     config;
    int64_t                     result;

// Test01_001_002
// @brief: normal case: func = NULL and userarg = NULL
    uid              = TEST_THREAD_ID1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE2;
    config.task_name = (char *)"Simple testing thread";
    config.func      = NULL;
    config.userarg   = NULL;
    config.stack_size = THREAD_STACK_SIZE;

    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK) << "Test01_001_002 failed";
    ASSERT_NE(thread_handle, NULL_PTR);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK);
    EXPECT_EQ(result, 0);
    thread_handle = OSAL_THREAD_HANDLE_INVALID;
}
// [ThreadCreate_TC_01_001_002]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadCreate_TC_01_001_003 ThreadApiTest.ThreadCreate_TC_01_001_003
 * @subsection ThreadCreate_TC_01_001_003_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadCreate_TC_01_001_003_tci Test Case ID
 * ThreadCreate_S1_BN_TC_01_001_003
 * @subsection ThreadCreate_TC_01_001_003_rr Related Requirement(s)
 * \#199129, \#199135, \#199136, \#199137, \#199138, \#294297
 * @subsection ThreadCreate_TC_01_001_003_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadCreate_TC_01_001_003_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to create a thread with normal condition:
 * - Test01_001_003: priority boundary checking with OSAL_THREAD_PRIORITY_TYPE14.
 * @subsection ThreadCreate_TC_01_001_003_tpts Test Procedure/Test Steps
 * -# Create a thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE14
 *          -# task_name is "Simple testing thread"
 *          -# func is simple_workerfunction function with functionality:
 *              - Suspend thread 500ms with R_OSAL_ThreadSleepForTimePeriod.
 *          -# userarg is NULL
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Address of thread handle.
 * -# Join thread by using R_OSAL_ThreadJoin.
 * @subsection ThreadCreate_TC_01_001_003_ereb Expected Result/Behavior
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK.<br>
 * "thread_handle" is not NULL_PTR.<br>
 * In simple_workerfunction function: <br>
 *  - R_OSAL_ThreadSleepForTimePeriod return OSAL_RETURN_OK.<br>
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK.<br>
 * Value of "result" is equal to 0.
 * @subsection ThreadCreate_TC_01_001_003_testcase Test Case Example
 * @snippet test_thread.cpp ThreadCreate_TC_01_001_003
 * @snippet test_thread.cpp simple_workerfunction
 */
// [ThreadCreate_TC_01_001_003]
TEST_F(ThreadApiTest, ThreadCreate_TC_01_001_003)
{
    osal_thread_id_t            uid;
    st_osal_thread_config_t     config;
    int64_t                     result;

// Test01_001_003
// @brief: boundary normal case: priority boundary checking (OSAL_THREAD_PRIORITY_TYPE14)
    uid              = TEST_THREAD_ID1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE14;
    config.task_name = (char *)"Simple testing thread";
    config.func      = simple_workerfunction;
    config.userarg   = NULL;
    config.stack_size = THREAD_STACK_SIZE;

    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK) << "Test01_001_003 failed";
    ASSERT_NE(thread_handle, NULL_PTR);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK);
    EXPECT_EQ(result, 0);
    thread_handle = OSAL_THREAD_HANDLE_INVALID;
}
// [ThreadCreate_TC_01_001_003]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadCreate_TC_01_001_004 ThreadApiTest.ThreadCreate_TC_01_001_004
 * @subsection ThreadCreate_TC_01_001_004_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadCreate_TC_01_001_004_tci Test Case ID
 * ThreadCreate_S1_BN_TC_01_001_004
 * @subsection ThreadCreate_TC_01_001_004_rr Related Requirement(s)
 * \#199129, \#199135, \#199136, \#199137, \#199138, \#294297
 * @subsection ThreadCreate_TC_01_001_004_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadCreate_TC_01_001_004_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to create a thread with normal condition:
 * - Test01_001_004: priority boundary checking with OSAL_THREAD_PRIORITY_TYPE0.
 * @subsection ThreadCreate_TC_01_001_004_tpts Test Procedure/Test Steps
 * -# Create a thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE0
 *          -# task_name is "Simple testing thread"
 *          -# func is simple_workerfunction function with functionality:
 *              - Suspend thread for 500ms by using R_OSAL_ThreadSleepForTimePeriod.
 *          -# userarg is NULL
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Address of thread handle.
 * -# Join thread by using R_OSAL_ThreadJoin.
 * @subsection ThreadCreate_TC_01_001_004_ereb Expected Result/Behavior
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK.<br>
 * "thread_handle" is not NULL_PTR.<br>
 * In simple_workerfunction function: <br>
 *  - R_OSAL_ThreadSleepForTimePeriod return OSAL_RETURN_OK.<br>
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK.<br>
 * Value of "result" is equal to 0.
 * @subsection ThreadCreate_TC_01_001_004_testcase Test Case Example
 * @snippet test_thread.cpp ThreadCreate_TC_01_001_004
 * @snippet test_thread.cpp simple_workerfunction
 */
// [ThreadCreate_TC_01_001_004]
TEST_F(ThreadApiTest, ThreadCreate_TC_01_001_004)
{
    osal_thread_id_t            uid;
    st_osal_thread_config_t     config;
    int64_t                     result;

// Test01_001_004
// @brief: boundary normal case: priority boundary checking (OSAL_THREAD_PRIORITY_TYPE0)
    uid              = TEST_THREAD_ID1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE0;
    config.task_name = (char *)"Simple testing thread";
    config.func      = simple_workerfunction;
    config.userarg   = NULL;
    config.stack_size = THREAD_STACK_SIZE;

    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK) << "Test01_001_004 failed";
    ASSERT_NE(thread_handle, NULL_PTR);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK);
    EXPECT_EQ(result, 0);
    thread_handle = OSAL_THREAD_HANDLE_INVALID;
}
// [ThreadCreate_TC_01_001_004]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadCreate_TC_01_001_005 ThreadApiTest.ThreadCreate_TC_01_001_005
 * @subsection ThreadCreate_TC_01_001_005_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadCreate_TC_01_001_005_tci Test Case ID
 * ThreadCreate_S0_A_TC_01_001_005
 * @subsection ThreadCreate_TC_01_001_005_rr Related Requirement(s)
 * \#199129, \#199135, \#199136, \#199138
 * @subsection ThreadCreate_TC_01_001_005_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadCreate_TC_01_001_005_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to create a thread with abnormal condition:
 * - Test01_001_005: p_config is NULL.
 * @subsection ThreadCreate_TC_01_001_005_tpts Test Procedure/Test Steps
 * -# Initialize the thread_handle with THREAD_HANDLE_INITIALIZED_VALUE by using memset.<br>
 * THREAD_HANDLE_INITIALIZED_VALUE must be different from OSAL_THREAD_HANDLE_INVALID.
 * -# Create a thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config is NULL
 *      - thread_id is TEST_THREAD_ID1.
 *      - thread_handle is THREAD_HANDLE_INITIALIZED_VALUE.
 * @subsection ThreadCreate_TC_01_001_005_ereb Expected Result/Behavior
 * -# memset a value for thread_handle successfully.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_PAR.<br>
 * Value of "thread_handle" is NULL.
 * @subsection ThreadCreate_TC_01_001_005_testcase Test Case Example
 * @snippet test_thread.cpp ThreadCreate_TC_01_001_005
 */
// [ThreadCreate_TC_01_001_005]
TEST_F(ThreadApiTest, ThreadCreate_TC_01_001_005)
{
    osal_thread_id_t            uid;
    st_osal_thread_config_t* p_config = NULL;

// Test01_001_005
// @brief: abnormal case: p_config is NULL
    uid              = TEST_THREAD_ID1;
    memset(&thread_handle, THREAD_HANDLE_INITIALIZED_VALUE, sizeof(thread_handle));
    ASSERT_EQ(R_OSAL_ThreadCreate(p_config, uid, &thread_handle), OSAL_RETURN_PAR) << "Test01_001_005 failed";
    ASSERT_EQ(thread_handle, NULL_PTR);
}
// [ThreadCreate_TC_01_001_005]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadCreate_TC_01_001_006 ThreadApiTest.ThreadCreate_TC_01_001_006
 * @subsection ThreadCreate_TC_01_001_006_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadCreate_TC_01_001_006_tci Test Case ID
 * ThreadCreate_S0_A_TC_01_001_006
 * @subsection ThreadCreate_TC_01_001_006_rr Related Requirement(s)
 * \#199129, \#199133, \#199135, \#199136, \#199137, \#199138
 * @subsection ThreadCreate_TC_01_001_006_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadCreate_TC_01_001_006_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to create a thread with abnormal condition:
 * - Test01_001_006: p_handle is NULL.
 * @subsection ThreadCreate_TC_01_001_006_tpts Test Procedure/Test Steps
 * -# Create a thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE6
 *          -# task_name is "Simple testing thread"
 *          -# func is simple_workerfunction function with functionality:
 *              - Suspend thread for 500ms by using R_OSAL_ThreadSleepForTimePeriod.
 *          -# userarg is NULL
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Thread handle is NULL.
 * @subsection ThreadCreate_TC_01_001_006_ereb Expected Result/Behavior
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_PAR.<br>
 * @subsection ThreadCreate_TC_01_001_006_testcase Test Case Example
 * @snippet test_thread.cpp ThreadCreate_TC_01_001_006
 * @snippet test_thread.cpp simple_workerfunction
 */
// [ThreadCreate_TC_01_001_006]
TEST_F(ThreadApiTest, ThreadCreate_TC_01_001_006)
{
    osal_thread_id_t            uid;
    st_osal_thread_config_t     config;

// Test01_001_006
// @brief: abnormal case: p_handle is NULL
    uid              = TEST_THREAD_ID1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE6;
    config.task_name = (char *)"Simple testing thread";
    config.func      = simple_workerfunction;
    config.userarg   = NULL;
    config.stack_size = THREAD_STACK_SIZE;

    osal_thread_handle_t* p_handle = NULL;
    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, p_handle), OSAL_RETURN_PAR) << "Test01_001_006 failed";
}
// [ThreadCreate_TC_01_001_006]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadCreate_TC_01_001_007 ThreadApiTest.ThreadCreate_TC_01_001_007
 * @subsection ThreadCreate_TC_01_001_007_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadCreate_TC_01_001_007_tci Test Case ID
 * ThreadCreate_S0_BA_TC_01_001_007
 * @subsection ThreadCreate_TC_01_001_007_rr Related Requirement(s)
 * \#199129, \#199135, \#199136, \#199138, \#294297
 * @subsection ThreadCreate_TC_01_001_007_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadCreate_TC_01_001_007_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to create a thread with abnormal condition:
 * - Test01_001_007: priority is equal to OSAL_THREAD_PRIORITY_HIGHEST + 1.
 * @subsection ThreadCreate_TC_01_001_007_tpts Test Procedure/Test Steps
 * -# Initialize the thread_handle with THREAD_HANDLE_INITIALIZED_VALUE by using memset.<br>
 * THREAD_HANDLE_INITIALIZED_VALUE must be different from OSAL_THREAD_HANDLE_INVALID.
 * -# Create a thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_HIGHEST + 1
 *          -# task_name is "Simple testing thread"
 *          -# func is simple_workerfunction function with functionality:
 *              - Suspend thread for 500ms by using R_OSAL_ThreadSleepForTimePeriod.
 *          -# userarg is NULL
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - thread_handle is THREAD_HANDLE_INITIALIZED_VALUE.
 * @subsection ThreadCreate_TC_01_001_007_ereb Expected Result/Behavior
 * -# memset a value for thread_handle successfully
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_PAR.<br>
 * Value of "thread_handle" is NULL.<br>
 * In simple_workerfunction function: <br>
 *  - R_OSAL_ThreadSleepForTimePeriod return OSAL_RETURN_OK.
 * @subsection ThreadCreate_TC_01_001_007_testcase Test Case Example
 * @snippet test_thread.cpp ThreadCreate_TC_01_001_007
 * @snippet test_thread.cpp simple_workerfunction
 */
// [ThreadCreate_TC_01_001_007]
TEST_F(ThreadApiTest, ThreadCreate_TC_01_001_007)
{
    osal_thread_id_t            uid;
    st_osal_thread_config_t     config;

// Test01_001_007
// @brief: boundary abnormal case: priority = OSAL_THREAD_PRIORITY_HIGHEST + 1
    uid              = TEST_THREAD_ID1;
    config.priority  = (e_osal_thread_priority_t)(OSAL_THREAD_PRIORITY_HIGHEST + 1);
    config.task_name = (char *)"Simple testing thread";
    config.func      = simple_workerfunction;
    config.userarg   = NULL;
    config.stack_size = THREAD_STACK_SIZE;
    memset(&thread_handle, THREAD_HANDLE_INITIALIZED_VALUE, sizeof(thread_handle));

    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_PAR) << "Test01_001_007 failed";
    ASSERT_EQ(thread_handle, NULL_PTR);
}
// [ThreadCreate_TC_01_001_007]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadCreate_TC_01_001_008 ThreadApiTest.ThreadCreate_TC_01_001_008
 * @subsection ThreadCreate_TC_01_001_008_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadCreate_TC_01_001_008_tci Test Case ID
 * ThreadCreate_S0_BA_TC_01_001_008
 * @subsection ThreadCreate_TC_01_001_008_rr Related Requirement(s)
 * \#199129, \#199135, \#199136, \#199138, \#294297
 * @subsection ThreadCreate_TC_01_001_008_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadCreate_TC_01_001_008_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to create a thread with abnormal condition:
 * - Test01_001_008: priority is equal to OSAL_THREAD_PRIORITY_LOWEST - 1.
 * @subsection ThreadCreate_TC_01_001_008_tpts Test Procedure/Test Steps
 * -# Initialize the thread_handle with THREAD_HANDLE_INITIALIZED_VALUE by using memset.<br>
 * THREAD_HANDLE_INITIALIZED_VALUE must be different from OSAL_THREAD_HANDLE_INVALID.
 * -# Create a thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_LOWEST - 1
 *          -# task_name is "Simple testing thread"
 *          -# func is simple_workerfunction function with functionality:
 *              - Suspend thread for 500ms by using R_OSAL_ThreadSleepForTimePeriod.
 *          -# userarg is NULL
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - thread_handle is THREAD_HANDLE_INITIALIZED_VALUE.
 * @subsection ThreadCreate_TC_01_001_008_ereb Expected Result/Behavior
 * -# memset a value for thread_handle successfully.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_PAR.<br>
 * Value of "thread_handle" is NULL.<br>
 * In simple_workerfunction function: <br>
 *  - R_OSAL_ThreadSleepForTimePeriod return OSAL_RETURN_OK.
 * @subsection ThreadCreate_TC_01_001_008_testcase Test Case Example
 * @snippet test_thread.cpp ThreadCreate_TC_01_001_008
 * @snippet test_thread.cpp simple_workerfunction
 */
// [ThreadCreate_TC_01_001_008]
TEST_F(ThreadApiTest, ThreadCreate_TC_01_001_008)
{
    osal_thread_id_t            uid;
    st_osal_thread_config_t     config;

// Test01_001_008
// @brief: boundary abnormal case: priority = OSAL_THREAD_PRIORITY_LOWEST - 1
    uid              = TEST_THREAD_ID1;
    config.priority  = (e_osal_thread_priority_t)(OSAL_THREAD_PRIORITY_LOWEST - 1);
    config.task_name = (char *)"Simple testing thread";
    config.func      = simple_workerfunction;
    config.userarg   = NULL;
    config.stack_size = THREAD_STACK_SIZE;
    memset(&thread_handle, THREAD_HANDLE_INITIALIZED_VALUE, sizeof(thread_handle));

    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_PAR) << "Test01_001_008 failed";
    ASSERT_EQ(thread_handle, NULL_PTR);
}
// [ThreadCreate_TC_01_001_008]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadCreate_TC_01_001_009 ThreadApiTest.ThreadCreate_TC_01_001_009
 * @subsection ThreadCreate_TC_01_001_009_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadCreate_TC_01_001_009_tci Test Case ID
 * ThreadCreate_S2_A_TC_01_001_009
 * @subsection ThreadCreate_TC_01_001_009_rr Related Requirement(s)
 * \#199129, \#199135, \#199136, \#199137, \#199138
 * @subsection ThreadCreate_TC_01_001_009_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadCreate_TC_01_001_009_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to create a thread with abnormal condition:
 * - Test01_001_009: create 2 threads with the same thread ID.
 * @subsection ThreadCreate_TC_01_001_009_tpts Test Procedure/Test Steps
 * -# Create first thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE7
 *          -# task_name is "Simple testing thread"
 *          -# func is simple_workerfunction function with functionality:
 *              - Suspend thread 500ms with R_OSAL_ThreadSleepForTimePeriod.
 *          -# userarg is NULL
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Address of thread handle.
 * -# Create second thread which thread id is the same as first thread by using R_OSAL_ThreadCreate.
 * -# Join first thread by using R_OSAL_ThreadJoin.
 * @subsection ThreadCreate_TC_01_001_009_ereb Expected Result/Behavior
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with first thread.<br>
 * "thread_handle" is not NULL_PTR.<br>
 * In simple_workerfunction function: <br>
 *  - R_OSAL_ThreadSleepForTimePeriod return OSAL_RETURN_OK.<br>
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_BUSY with second thread.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK.<br>
 * Value of "result" is equal to 0.
 * @subsection ThreadCreate_TC_01_001_009_testcase Test Case Example
 * @snippet test_thread.cpp ThreadCreate_TC_01_001_009
 * @snippet test_thread.cpp simple_workerfunction
 */
// [ThreadCreate_TC_01_001_009]
TEST_F(ThreadApiTest, ThreadCreate_TC_01_001_009)
{
    osal_thread_id_t            uid;
    st_osal_thread_config_t     config;
    int64_t                     result;

// Test01_001_009
// @brief: abnormal case: create 2 threads with the same thread ID.
    uid              = TEST_THREAD_ID1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE8;
    config.task_name = (char *)"Simple testing thread";
    config.func      = simple_workerfunction;
    config.userarg   = NULL;
    config.stack_size = THREAD_STACK_SIZE;

    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK);
    ASSERT_NE(thread_handle, NULL_PTR);
    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle1), OSAL_RETURN_BUSY) << "Test01_001_009 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK);
    EXPECT_EQ(result, 0);
    thread_handle = OSAL_THREAD_HANDLE_INVALID;
    thread_handle1 = OSAL_THREAD_HANDLE_INVALID;
}
// [ThreadCreate_TC_01_001_009]

/**
 * @page Thread_TEST Thread API Test
 * @section OsalApiStateTest_ThreadCreate_TC_01_001_010 OsalApiStateTest.ThreadCreate_TC_01_001_010
 * @subsection ThreadCreate_TC_01_001_010_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection ThreadCreate_TC_01_001_010_tci Test Case ID
 * ThreadCreate_S2_A_TC_01_001_010
 * @subsection ThreadCreate_TC_01_001_010_rr Related Requirement(s)
 * \#199135
 * @subsection ThreadCreate_TC_01_001_010_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection ThreadCreate_TC_01_001_010_tcs Test Case Summary
 * This test shall verify the state transition of Thread APIs with abnormal condition:
 * - Test01_001_010: state transition error with R_OSAL_ThreadCreate.
 * @subsection ThreadCreate_TC_01_001_010_tpts Test Procedure/Test Steps
 * -# Initialize the thread_handle with THREAD_HANDLE_INITIALIZED_VALUE by using memset.<br>
 * THREAD_HANDLE_INITIALIZED_VALUE must be different from OSAL_THREAD_HANDLE_INVALID
 * -# Create a thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config is valid pointer
 *      - thread_id is TEST_THREAD_ID1
 *      - thread handle is THREAD_HANDLE_INITIALIZED_VALUE
 * @subsection ThreadCreate_TC_01_001_010_ereb Expected Result/Behavior
 * -# memset a value for thread_handle successfully
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_STATE.<br>
 * "thread_handle" is OSAL_THREAD_HANDLE_INVALID.
 * @subsection ThreadCreate_TC_01_001_010_testcase Test Case Example
 * @snippet test_thread.cpp ThreadCreate_TC_01_001_010
 */
// [ThreadCreate_TC_01_001_010]
TEST_F(OsalApiStateTest, ThreadCreate_TC_01_001_010)
{
    st_osal_thread_config_t     config;
    osal_thread_id_t            uid = TEST_THREAD_ID1;
    osal_thread_handle_t        thread_handle = OSAL_THREAD_HANDLE_INVALID;
    memset(&thread_handle, THREAD_HANDLE_INITIALIZED_VALUE, sizeof(thread_handle));

// Test01_001_010
// @brief: abnormal case: state transition error with R_OSAL_ThreadCreate
    EXPECT_EQ(R_OSAL_ThreadInitializeThreadConfigSt(&config), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_STATE) << "Test01_001_010 failed";
    EXPECT_EQ(thread_handle, OSAL_THREAD_HANDLE_INVALID);
}
// [ThreadCreate_TC_01_001_010]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadCreate_TC_01_001_011 ThreadApiTest.ThreadCreate_TC_01_001_011
 * @subsection ThreadCreate_TC_01_001_011_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadCreate_TC_01_001_011_tci Test Case ID
 * ThreadCreate_S1_A_TC_01_001_011
 * @subsection ThreadCreate_TC_01_001_011_rr Related Requirement(s)
 * \#199129, \#199135, \#199136, \#199137, \#199138
 * @subsection ThreadCreate_TC_01_001_011_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadCreate_TC_01_001_011_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to create a thread with normal condition:
 * - Test01_001_011: task_name is NULL, and otherwise all input/output param is valid.
 * @subsection ThreadCreate_TC_01_001_011_tpts Test Procedure/Test Steps
 * -# Create a thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is NULL
 *          -# func is simple_workerfunction function with functionality:
 *              - Suspend thread 500ms with R_OSAL_ThreadSleepForTimePeriod.
 *          -# userarg is 1
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Address of thread handle.
 * -# Join thread by using R_OSAL_ThreadJoin.
 * @subsection ThreadCreate_TC_01_001_011_ereb Expected Result/Behavior
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK.<br>
 * "thread_handle" is not NULL_PTR.
 * In simple_workerfunction function: <br>
 *  - R_OSAL_ThreadSleepForTimePeriod return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK.<br>
 * Value of "result" is equal to 0.
 * @subsection ThreadCreate_TC_01_001_011_testcase Test Case Example
 * @snippet test_thread.cpp ThreadCreate_TC_01_001_011
 * @snippet test_thread.cpp simple_workerfunction
 */
// [ThreadCreate_TC_01_001_011]
TEST_F(ThreadApiTest, ThreadCreate_TC_01_001_011)
{
    osal_thread_id_t            uid;
    st_osal_thread_config_t     config;
    int                         arg;
    int64_t                     result;

// Test01_001_011
// @brief: task_name is NULL, and otherwise all input/output param is valid.
    uid              = TEST_THREAD_ID1;
    arg              = 1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = NULL;
    config.func      = simple_workerfunction;
    config.userarg   = &arg;
    config.stack_size = THREAD_STACK_SIZE;

    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK) << "Test01_001_011 failed";
    ASSERT_NE(thread_handle, NULL_PTR);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK);
    EXPECT_EQ(result, 0);
    thread_handle = OSAL_THREAD_HANDLE_INVALID;
}
// [ThreadCreate_TC_01_001_011]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadCreate_TC_01_001_012 ThreadApiTest.ThreadCreate_TC_01_001_012
 * @subsection ThreadCreate_TC_01_001_012_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadCreate_TC_01_001_012_tci Test Case ID
 * ThreadCreate_S0_A_TC_01_001_012
 * @subsection ThreadCreate_TC_01_001_012_rr Related Requirement(s)
 * \#199135, \#199136
 * @subsection ThreadCreate_TC_01_001_012_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadCreate_TC_01_001_012_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to create a thread with abnormal condition:
 * - Test01_001_012: stack size of thread configurations is 0
 * @subsection ThreadCreate_TC_01_001_012_tpts Test Procedure/Test Steps
 * -# Initialize the thread_handle with THREAD_HANDLE_INITIALIZED_VALUE by using memset.<br>
 * THREAD_HANDLE_INITIALIZED_VALUE must be different from OSAL_THREAD_HANDLE_INVALID
 * -# Create a thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is simple_workerfunction function with functionality:
 *              - Suspend thread for 500ms by using R_OSAL_ThreadSleepForTimePeriod.
 *          -# userarg is NULL.
 *          -# stack_size is 0.
 *      - thread_id is TEST_THREAD_ID1.
 *      - thread_handle is THREAD_HANDLE_INITIALIZED_VALUE.
 * @subsection ThreadCreate_TC_01_001_012_ereb Expected Result/Behavior
 * -# memset a value for thread_handle successfully.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_PAR.<br>
 * Value of "thread_handle" is OSAL_THREAD_HANDLE_INVALID.<br>
 * @subsection ThreadCreate_TC_01_001_012_testcase Test Case Example
 * @snippet test_thread.cpp ThreadCreate_TC_01_001_012
 * @snippet test_thread.cpp simple_workerfunction
 */
// [ThreadCreate_TC_01_001_012]
TEST_F(ThreadApiTest, ThreadCreate_TC_01_001_012)
{
    osal_thread_id_t            uid;
    st_osal_thread_config_t     config;
    int                         arg;

// Test01_001_012
// @brief: abnormal case: stack size is 0.
    uid              = TEST_THREAD_ID1;
    arg              = NULL;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = (char *)"Simple testing thread";
    config.func      = simple_workerfunction;
    config.userarg   = &arg;
    config.stack_size = 0;
    memset(&thread_handle, THREAD_HANDLE_INITIALIZED_VALUE, sizeof(thread_handle));

    EXPECT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_PAR) << "Test01_001_012 failed";
    EXPECT_EQ(thread_handle, OSAL_THREAD_HANDLE_INVALID) << "Test01_001_012 failed";
}
// [ThreadCreate_TC_01_001_012]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadSleepUntilTimeStamp_TC_01_002_001 ThreadApiTest.ThreadSleepUntilTimeStamp_TC_01_002_001
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_001_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_001_tci Test Case ID
 * ThreadSleepUntilTimeStamp_S1_N_TC_01_002_001
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_001_rr Related Requirement(s)
 * \#199129, \#199131, \#199142
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_001_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_001_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to suspend thread until time stamp with normal condition:
 * - Test01_002_001: sleep 1sec and 2000000ns.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_001_tpts Test Procedure/Test Steps
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Suspend thread with R_OSAL_ThreadSleepUntilTimeStamp (around 1sec and 2000000nsec).
 * -# Get the current time stamp again by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Calculate the difference between two times (at step 1 and step 3) using R_OSAL_ClockTimeCalculateTimeDifference.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_001_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadSleepUntilTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK. <br>
 * The returned timestamp is equal or bigger than the previous timestamp (at step 1) plus with 1002000000nsec.
 * -# R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK.<br>
 * The different time is equal or bigger than 1002000000nsec.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_001_testcase Test Case Example
 * @snippet test_thread.cpp ThreadSleepUntilTimeStamp_TC_01_002_001
 */
// [ThreadSleepUntilTimeStamp_TC_01_002_001]
TEST_F(ThreadApiTest, ThreadSleepUntilTimeStamp_TC_01_002_001)
{
    st_osal_time_t  current_time, abs_time;
    osal_nano_sec_t  diff_time;

// Test01_002_001
// @brief: normal case: sleep 1sec and 2000000ns
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    if(current_time.tv_nsec + 2 * MSEC_TO_NSEC <= MAX_NSEC)
    {
        abs_time.tv_nsec = current_time.tv_nsec + 2 * MSEC_TO_NSEC;
        abs_time.tv_sec = current_time.tv_sec + 1;
    }
    else
    {
        abs_time.tv_nsec = 2 * MSEC_TO_NSEC - (MAX_NSEC - current_time.tv_nsec);
        abs_time.tv_sec = current_time.tv_sec + 2;
    }
    ASSERT_EQ(R_OSAL_ThreadSleepUntilTimeStamp(&abs_time), OSAL_RETURN_OK) << "Test01_002_001 failed";
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time), OSAL_RETURN_OK);
    ASSERT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&abs_time, &current_time, &diff_time), OSAL_RETURN_OK);
    ASSERT_GE(diff_time, (1 * SEC_TO_NSEC + 2 * MSEC_TO_NSEC)) << "Test01_002_001 failed";
}
// [ThreadSleepUntilTimeStamp_TC_01_002_001]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadSleepUntilTimeStamp_TC_01_002_002 ThreadApiTest.ThreadSleepUntilTimeStamp_TC_01_002_002
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_002_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_002_tci Test Case ID
 * ThreadSleepUntilTimeStamp_S1_BN_TC_01_002_002
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_002_rr Related Requirement(s)
 * \#199129, \#199131, \#199142
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_002_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_002_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to suspend thread until time stamp with normal condition:
 * - Test01_002_002: checking the boundary of "tv_nsec".
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_002_tpts Test Procedure/Test Steps
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Suspend thread with R_OSAL_ThreadSleepUntilTimeStamp (around 1sec and 2000000nsec).
 * -# Get the current time stamp again by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Calculate the difference between two times (at step 1 and step 3) using R_OSAL_ClockTimeCalculateTimeDifference.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_002_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadSleepUntilTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK. <br>
 * The returned timestamp is equal or bigger than the previous timestamp (at step 1) plus with 1001000000nsec.
 * -# R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK. <br>
 * The different time is equal or bigger than (1001000000 - current_time.tv_nsec).
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_002_testcase Test Case Example
 * @snippet test_thread.cpp ThreadSleepUntilTimeStamp_TC_01_002_002
 */
// [ThreadSleepUntilTimeStamp_TC_01_002_002]
TEST_F(ThreadApiTest, ThreadSleepUntilTimeStamp_TC_01_002_002)
{
    st_osal_time_t  current_time, abs_time;
    osal_nano_sec_t  diff_time;

// Test01_002_002
// @brief: boundary normal case: checking the boundary of tv_nsec
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    abs_time.tv_sec = current_time.tv_sec + 1;
    abs_time.tv_nsec = 1 * MSEC_TO_NSEC;
    ASSERT_EQ(R_OSAL_ThreadSleepUntilTimeStamp(&abs_time), OSAL_RETURN_OK) << "Test01_002_002 failed";
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time), OSAL_RETURN_OK);
    ASSERT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&abs_time, &current_time, &diff_time), OSAL_RETURN_OK);
    ASSERT_GE(diff_time, (1 * SEC_TO_NSEC + 1 * MSEC_TO_NSEC) - current_time.tv_nsec) << "Test01_002_002 failed";
}
// [ThreadSleepUntilTimeStamp_TC_01_002_002]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadSleepUntilTimeStamp_TC_01_002_003 ThreadApiTest.ThreadSleepUntilTimeStamp_TC_01_002_003
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_003_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_003_tci Test Case ID
 * ThreadSleepUntilTimeStamp_S1_BN_TC_01_002_003
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_003_rr Related Requirement(s)
 * \#199129, \#199131, \#199142
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_003_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_003_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to suspend thread until time stamp with normal condition:
 * - Test01_002_003: checking the boundary of "tv_nsec" (sleep 1sec and 999999999nsec).
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_003_tpts Test Procedure/Test Steps
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Suspend thread with R_OSAL_ThreadSleepUntilTimeStamp (around 1sec and 999999999nsec).
 * -# Get the current time stamp again by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Calculate the difference between two times (at step 1 and step 3) using R_OSAL_ClockTimeCalculateTimeDifference.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_003_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadSleepUntilTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK. <br>
 * The returned timestamp is equal or bigger than the previous timestamp (at step 1) plus with 1999999999nsec.
 * -# R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK. <br>
 * The different time is equal or bigger than (1999999999 - current_time.tv_nsec).
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_003_testcase Test Case Example
 * @snippet test_thread.cpp ThreadSleepUntilTimeStamp_TC_01_002_003
 */
// [ThreadSleepUntilTimeStamp_TC_01_002_003]
TEST_F(ThreadApiTest, ThreadSleepUntilTimeStamp_TC_01_002_003)
{
    st_osal_time_t  current_time, abs_time;
    osal_nano_sec_t  diff_time;

// Test01_002_003
// @brief: boundary normal case: checking the boundary of tv_nsec (sleep 1sec and 999999999ns)
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    abs_time.tv_sec = current_time.tv_sec + 1;
    abs_time.tv_nsec = MAX_NSEC;
    ASSERT_EQ(R_OSAL_ThreadSleepUntilTimeStamp(&abs_time), OSAL_RETURN_OK) << "Test01_002_003 failed";
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time), OSAL_RETURN_OK);
    ASSERT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&abs_time, &current_time, &diff_time), OSAL_RETURN_OK);
    ASSERT_GE(diff_time, (1 * SEC_TO_NSEC + MAX_NSEC) - current_time.tv_nsec) << "Test01_002_003 failed";
}
// [ThreadSleepUntilTimeStamp_TC_01_002_003]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadSleepUntilTimeStamp_TC_01_002_004 ThreadApiTest.ThreadSleepUntilTimeStamp_TC_01_002_004
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_004_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_004_tci Test Case ID
 * ThreadSleepUntilTimeStamp_S1_N_TC_01_002_004
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_004_rr Related Requirement(s)
 * \#199129, \#199131, \#199142
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_004_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_004_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to suspend thread until time stamp with normal condition:
 * - Test01_002_004: checking round up when "tv_nsec" is less than 1000000nsec (1msec)
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_004_tpts Test Procedure/Test Steps
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Suspend thread with R_OSAL_ThreadSleepUntilTimeStamp (less then 1msec).
 * -# Get the current time stamp again by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Calculate the difference between two times (at step 1 and step 3) using R_OSAL_ClockTimeCalculateTimeDifference.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_004_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadSleepUntilTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK. <br>
 * The returned timestamp is equal or bigger than the previous timestamp (at step 1).
 * -# R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK. <br>
 * The different time is equal or bigger than (1001000000 - current_time.tv_nsec).
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_004_testcase Test Case Example
 * @snippet test_thread.cpp ThreadSleepUntilTimeStamp_TC_01_002_004
 */
// [ThreadSleepUntilTimeStamp_TC_01_002_004]
TEST_F(ThreadApiTest, ThreadSleepUntilTimeStamp_TC_01_002_004)
{
    st_osal_time_t  current_time, abs_time;
    osal_nano_sec_t  diff_time;

// Test01_002_004
// @brief: normal case: checking round up when tv_nsec is less than 1000000ns (1ms)
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK) << "Test01_002_004 failed";
    abs_time.tv_sec = current_time.tv_sec + 1;
    abs_time.tv_nsec = current_time.tv_nsec;
    ASSERT_EQ(R_OSAL_ThreadSleepUntilTimeStamp(&abs_time), OSAL_RETURN_OK) << "Test01_002_004 failed";
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time), OSAL_RETURN_OK);
    ASSERT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&abs_time, &current_time, &diff_time), OSAL_RETURN_OK);
    ASSERT_GE(diff_time, (1 * SEC_TO_NSEC + 1 * MSEC_TO_NSEC) - current_time.tv_nsec) << "Test01_002_004 failed";
}
// [ThreadSleepUntilTimeStamp_TC_01_002_004]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadSleepUntilTimeStamp_TC_01_002_005 ThreadApiTest.ThreadSleepUntilTimeStamp_TC_01_002_005
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_005_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_005_tci Test Case ID
 * ThreadSleepUntilTimeStamp_S0_A_TC_01_002_005
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_005_rr Related Requirement(s)
 * \#199129, \#199131, \#199142
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_005_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_005_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to suspend thread until time stamp with abnormal condition:
 * - Test01_002_005: "p_time_stamp" is NULL.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_005_tpts Test Procedure/Test Steps
 * -# Suspend thread with R_OSAL_ThreadSleepUntilTimeStamp with the time stamp is NULL.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_005_ereb Expected Result/Behavior
 * -# R_OSAL_ThreadSleepUntilTimeStamp return OSAL_RETURN_PAR.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_005_testcase Test Case Example
 * @snippet test_thread.cpp ThreadSleepUntilTimeStamp_TC_01_002_005
 */
// [ThreadSleepUntilTimeStamp_TC_01_002_005]
TEST_F(ThreadApiTest, ThreadSleepUntilTimeStamp_TC_01_002_005)
{
    st_osal_time_t* p_time = NULL;

// Test01_002_005
// @brief: abnormal case: p_time_stamp is NULL
    ASSERT_EQ(R_OSAL_ThreadSleepUntilTimeStamp(p_time), OSAL_RETURN_PAR) << "Test01_002_005 failed";
}
// [ThreadSleepUntilTimeStamp_TC_01_002_005]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadSleepUntilTimeStamp_TC_01_002_006 ThreadApiTest.ThreadSleepUntilTimeStamp_TC_01_002_006
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_006_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_006_tci Test Case ID
 * ThreadSleepUntilTimeStamp_S0_BA_TC_01_002_006
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_006_rr Related Requirement(s)
 * \#199129, \#199131, \#199142
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_006_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_006_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to suspend thread until time stamp with abnormal condition:
 * - Test01_002_006: "tv_nsec" is bigger than 1000000000nsec (1sec).
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_006_tpts Test Procedure/Test Steps
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Suspend thread with R_OSAL_ThreadSleepUntilTimeStamp (bigger than 1sec).
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_006_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadSleepUntilTimeStamp return OSAL_RETURN_PAR.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_006_testcase Test Case Example
 * @snippet test_thread.cpp ThreadSleepUntilTimeStamp_TC_01_002_006
 */
// [ThreadSleepUntilTimeStamp_TC_01_002_006]
TEST_F(ThreadApiTest, ThreadSleepUntilTimeStamp_TC_01_002_006)
{
    st_osal_time_t  current_time, abs_time;

// Test01_002_006
// @brief: boundary abnormal case: tv_nsec is bigger than 1000000000ns (1sec)
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    abs_time.tv_sec = current_time.tv_sec + 1;
    abs_time.tv_nsec = 1 * SEC_TO_NSEC;
    ASSERT_EQ(R_OSAL_ThreadSleepUntilTimeStamp(&abs_time), OSAL_RETURN_PAR) << "Test01_002_006 failed";
}
// [ThreadSleepUntilTimeStamp_TC_01_002_006]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadSleepUntilTimeStamp_TC_01_002_007 ThreadApiTest.ThreadSleepUntilTimeStamp_TC_01_002_007
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_007_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_007_tci Test Case ID
 * ThreadSleepUntilTimeStamp_S0_A_TC_01_002_007
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_007_rr Related Requirement(s)
 * \#199129, \#199131, \#199142
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_007_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_007_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to suspend thread until time stamp with normal condition:
 * - Test01_002_007: checking with the passed time stamp.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_007_tpts Test Procedure/Test Steps
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Suspend thread 2000ms with R_OSAL_ThreadSleepForTimePeriod if current time stamp is less than 1sec.
 * -# Get the current time stamp again by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Suspend thread with R_OSAL_ThreadSleepUntilTimeStamp with the past time stamp.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_007_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadSleepForTimePeriod return OSAL_RETURN_OK.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK. <br>
 * The returned timestamp is equal or bigger than the previous timestamp (at step 1).
 * -# R_OSAL_ThreadSleepUntilTimeStamp return OSAL_RETURN_OK.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_007_testcase Test Case Example
 * @snippet test_thread.cpp ThreadSleepUntilTimeStamp_TC_01_002_007
 */
// [ThreadSleepUntilTimeStamp_TC_01_002_007]
TEST_F(ThreadApiTest, ThreadSleepUntilTimeStamp_TC_01_002_007)
{
    st_osal_time_t  current_time, abs_time;
    osal_milli_sec_t time;

// Test01_002_007
// @brief: normal case: checking with the passed time stamp
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    if (current_time.tv_sec < 1)
    {
        time = 2000;
        ASSERT_EQ(R_OSAL_ThreadSleepForTimePeriod(time), OSAL_RETURN_OK);
        ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    }
    abs_time.tv_sec = current_time.tv_sec - 1;
    abs_time.tv_nsec = current_time.tv_nsec;
    ASSERT_EQ(R_OSAL_ThreadSleepUntilTimeStamp(&abs_time), OSAL_RETURN_OK) << "Test01_002_007 failed";
}
// [ThreadSleepUntilTimeStamp_TC_01_002_007]

/**
 * @page Thread_TEST Thread API Test
 * @section OsalApiStateTest_ThreadSleepUntilTimeStamp_TC_01_002_008 OsalApiStateTest.ThreadSleepUntilTimeStamp_TC_01_002_008
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_008_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_008_tci Test Case ID
 * ThreadSleepUntilTimeStamp_S2_A_TC_01_002_008
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_008_rr Related Requirement(s)
 * \#199142
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_008_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * - Current timestamp "abs_time_state" has been got
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_008_tcs Test Case Summary
 * This test shall verify the state transition of Thread APIs with abnormal condition:
 * - Test01_002_008: state transition error with R_OSAL_ThreadSleepUntilTimeStamp.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_008_tpts Test Procedure/Test Steps
 * -# Suspend thread with R_OSAL_ThreadSleepUntilTimeStamp.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_008_ereb Expected Result/Behavior
 * -# R_OSAL_ThreadSleepUntilTimeStamp return OSAL_RETURN_STATE.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_008_testcase Test Case Example
 * @snippet test_thread.cpp ThreadSleepUntilTimeStamp_TC_01_002_008
 */
// [ThreadSleepUntilTimeStamp_TC_01_002_008]
TEST_F(OsalApiStateTest, ThreadSleepUntilTimeStamp_TC_01_002_008)
{
    st_osal_time_t  abs_time;

// Test01_002_008
// @brief: abnormal case: state transition error with R_OSAL_ThreadSleepUntilTimeStamp
    abs_time.tv_sec = abs_time_state.tv_sec + 1;
    abs_time.tv_nsec = abs_time_state.tv_nsec;
    EXPECT_EQ(R_OSAL_ThreadSleepUntilTimeStamp(&abs_time), OSAL_RETURN_STATE) << "Test01_002_008 failed";
}
// [ThreadSleepUntilTimeStamp_TC_01_002_008]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadSleepUntilTimeStamp_TC_01_002_009 ThreadApiTest.ThreadSleepUntilTimeStamp_TC_01_002_009
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_009_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_009_tci Test Case ID
 * ThreadSleepUntilTimeStamp_S0_A_TC_01_002_009
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_009_rr Related Requirement(s)
 * \#199129, \#199131, \#199142
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_009_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_009_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to suspend thread until time stamp with abnormal condition:
 * - Test01_002_009: checking with "tv_sec" is negative number.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_009_tpts Test Procedure/Test Steps
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Suspend thread with R_OSAL_ThreadSleepUntilTimeStamp with "tv_sec" is -1.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_009_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadSleepUntilTimeStamp return OSAL_RETURN_PAR.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_009_testcase Test Case Example
 * @snippet test_thread.cpp ThreadSleepUntilTimeStamp_TC_01_002_009
 */
// [ThreadSleepUntilTimeStamp_TC_01_002_009]
TEST_F(ThreadApiTest, ThreadSleepUntilTimeStamp_TC_01_002_009)
{
    st_osal_time_t  abs_time;

// Test01_002_009
// @brief: boundary abnormal case: tv_sec is negative number
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time), OSAL_RETURN_OK) << "Test01_002_009 failed";
    abs_time.tv_sec = -1;
    EXPECT_EQ(R_OSAL_ThreadSleepUntilTimeStamp(&abs_time), OSAL_RETURN_PAR) << "Test01_002_009 failed";
}
// [ThreadSleepUntilTimeStamp_TC_01_002_009]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadSleepUntilTimeStamp_TC_01_002_010 ThreadApiTest.ThreadSleepUntilTimeStamp_TC_01_002_010
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_010_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_010_tci Test Case ID
 * ThreadSleepUntilTimeStamp_S0_BN_TC_01_002_010
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_010_rr Related Requirement(s)
 * \#199129, \#199131, \#199142
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_010_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_010_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to suspend thread until time stamp:
 * - Test01_002_010: tv_sec is equal to zero
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_010_tpts Test Procedure/Test Steps
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Suspend thread with R_OSAL_ThreadSleepUntilTimeStamp (tv_sec is equal to zero).
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_010_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadSleepUntilTimeStamp return OSAL_RETURN_OK.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_010_testcase Test Case Example
 * @snippet test_thread.cpp ThreadSleepUntilTimeStamp_TC_01_002_010
 */
// [ThreadSleepUntilTimeStamp_TC_01_002_010]
TEST_F(ThreadApiTest, ThreadSleepUntilTimeStamp_TC_01_002_010)
{
    st_osal_time_t time_stamp;

// Test01_002_010
// @brief: boundary normal case: tv_sec is equal to zero
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &time_stamp), OSAL_RETURN_OK);
    time_stamp.tv_sec = 0;
    EXPECT_EQ(R_OSAL_ThreadSleepUntilTimeStamp(&time_stamp), OSAL_RETURN_OK) << "Test01_002_010 failed";
}
// [ThreadSleepUntilTimeStamp_TC_01_002_010]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadSleepUntilTimeStamp_TC_01_002_011 ThreadApiTest.ThreadSleepUntilTimeStamp_TC_01_002_011
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_011_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_011_tci Test Case ID
 * ThreadSleepUntilTimeStamp_S0_BN_TC_01_002_011
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_011_rr Related Requirement(s)
 * \#199129, \#199131, \#199142
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_011_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_011_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to suspend thread until time stamp:
 * - Test01_002_011: tv_nsec is equal to zero
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_011_tpts Test Procedure/Test Steps
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Suspend thread with R_OSAL_ThreadSleepUntilTimeStamp (tv_nsec is equal to zero).
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_011_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadSleepUntilTimeStamp return OSAL_RETURN_OK.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_011_testcase Test Case Example
 * @snippet test_thread.cpp ThreadSleepUntilTimeStamp_TC_01_002_011
 */
// [ThreadSleepUntilTimeStamp_TC_01_002_011]
TEST_F(ThreadApiTest, ThreadSleepUntilTimeStamp_TC_01_002_011)
{
    st_osal_time_t time_stamp;

// Test01_002_011
// @brief: boundary normal case: tv_nsec is equal to zero
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &time_stamp), OSAL_RETURN_OK);
    time_stamp.tv_nsec = 0;
    EXPECT_EQ(R_OSAL_ThreadSleepUntilTimeStamp(&time_stamp), OSAL_RETURN_OK) << "Test01_002_011 failed";
}
// [ThreadSleepUntilTimeStamp_TC_01_002_011]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadSleepUntilTimeStamp_TC_01_002_012 ThreadApiTest.ThreadSleepUntilTimeStamp_TC_01_002_012
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_012_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_012_tci Test Case ID
 * ThreadSleepUntilTimeStamp_S0_BA_TC_01_002_012
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_012_rr Related Requirement(s)
 * \#199129, \#199131, \#199142
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_012_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_012_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to suspend thread until time stamp:
 * - Test01_002_012: tv_nsec is less than zero
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_012_tpts Test Procedure/Test Steps
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Suspend thread with R_OSAL_ThreadSleepUntilTimeStamp (tv_nsec is less than zero).
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_012_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadSleepUntilTimeStamp return OSAL_RETURN_PAR.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_012_testcase Test Case Example
 * @snippet test_thread.cpp ThreadSleepUntilTimeStamp_TC_01_002_012
 */
// [ThreadSleepUntilTimeStamp_TC_01_002_012]
TEST_F(ThreadApiTest, ThreadSleepUntilTimeStamp_TC_01_002_012)
{
    st_osal_time_t time_stamp;

// Test01_002_012
// @brief: boundary abnormal case: tv_nsec is less than zero
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &time_stamp), OSAL_RETURN_OK);
    time_stamp.tv_nsec = -1;
    EXPECT_EQ(R_OSAL_ThreadSleepUntilTimeStamp(&time_stamp), OSAL_RETURN_PAR) << "Test01_002_012 failed";
}
// [ThreadSleepUntilTimeStamp_TC_01_002_012]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadSleepUntilTimeStamp_TC_01_002_013 ThreadApiTest.ThreadSleepUntilTimeStamp_TC_01_002_013
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_013_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_013_tci Test Case ID
 * ThreadSleepUntilTimeStamp_S0_BN_TC_01_002_013
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_013_rr Related Requirement(s)
 * \#199129, \#199131, \#199142
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_013_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_013_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to suspend thread until time stamp:
 * - Test01_002_013: tv_nsec and tv_sec are equal to zero
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_013_tpts Test Procedure/Test Steps
 * -# Suspend thread with R_OSAL_ThreadSleepUntilTimeStamp (tv_nsec and tv_sec are equal to zero).
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_013_ereb Expected Result/Behavior
 * -# R_OSAL_ThreadSleepUntilTimeStamp return OSAL_RETURN_PAR.
 * @subsection ThreadSleepUntilTimeStamp_TC_01_002_013_testcase Test Case Example
 * @snippet test_thread.cpp ThreadSleepUntilTimeStamp_TC_01_002_013
 */
// [ThreadSleepUntilTimeStamp_TC_01_002_013]
TEST_F(ThreadApiTest, ThreadSleepUntilTimeStamp_TC_01_002_013)
{
    st_osal_time_t time_stamp;

// Test01_002_013
// @brief: boundary normal case: tv_nsec and tv_sec are equal to zero
    time_stamp.tv_sec = 0;
    time_stamp.tv_nsec = 0;
    EXPECT_EQ(R_OSAL_ThreadSleepUntilTimeStamp(&time_stamp), OSAL_RETURN_OK) << "Test01_002_013 failed";
}
// [ThreadSleepUntilTimeStamp_TC_01_002_013]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadSleepForTimePeriod_TC_01_003_001 ThreadApiTest.ThreadSleepForTimePeriod_TC_01_003_001
 * @subsection ThreadSleepForTimePeriod_TC_01_003_001_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadSleepForTimePeriod_TC_01_003_001_tci Test Case ID
 * ThreadSleepForTimePeriod_S1_N_TC_01_003_001
 * @subsection ThreadSleepForTimePeriod_TC_01_003_001_rr Related Requirement(s)
 * \#199129, \#199130, \#199141
 * @subsection ThreadSleepForTimePeriod_TC_01_003_001_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadSleepForTimePeriod_TC_01_003_001_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to suspend a thread for specified time with normal condition:
 * - Test01_003_001: sleep 10ms
 * @subsection ThreadSleepForTimePeriod_TC_01_003_001_tpts Test Procedure/Test Steps
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Suspend thread 10ms with R_OSAL_ThreadSleepForTimePeriod.
 * -# Get the current time stamp again by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Calculate the difference between two times (at step 1 and step 3) using R_OSAL_ClockTimeCalculateTimeDifference.
 * @subsection ThreadSleepForTimePeriod_TC_01_003_001_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadSleepForTimePeriod return OSAL_RETURN_OK.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK. <br>
 * The returned timestamp is equal or bigger than the previous timestamp (at step 1).
 * -# R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK. <br>
 * The different time is equal or bigger than 10000000nsec.
 * @subsection ThreadSleepForTimePeriod_TC_01_003_001_testcase Test Case Example
 * @snippet test_thread.cpp ThreadSleepForTimePeriod_TC_01_003_001
 */
// [ThreadSleepForTimePeriod_TC_01_003_001]
TEST_F(ThreadApiTest, ThreadSleepForTimePeriod_TC_01_003_001)
{
    st_osal_time_t  current_time_1st, current_time_2nd;
    osal_milli_sec_t time;
    osal_nano_sec_t diff_time;

// Test01_003_001
// @brief: normal case: sleep 10ms
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_1st), OSAL_RETURN_OK);
    time = 10;
    ASSERT_EQ(R_OSAL_ThreadSleepForTimePeriod(time), OSAL_RETURN_OK) << "Test01_003_001 failed";
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_2nd), OSAL_RETURN_OK);
    ASSERT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &diff_time), OSAL_RETURN_OK);
    ASSERT_GE(diff_time, 10 * MSEC_TO_NSEC);
}
// [ThreadSleepForTimePeriod_TC_01_003_001]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadSleepForTimePeriod_TC_01_003_002 ThreadApiTest.ThreadSleepForTimePeriod_TC_01_003_002
 * @subsection ThreadSleepForTimePeriod_TC_01_003_002_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadSleepForTimePeriod_TC_01_003_002_tci Test Case ID
 * ThreadSleepForTimePeriod_S1_BN_TC_01_003_002
 * @subsection ThreadSleepForTimePeriod_TC_01_003_002_rr Related Requirement(s)
 * \#199129, \#199130, \#199141
 * @subsection ThreadSleepForTimePeriod_TC_01_003_002_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadSleepForTimePeriod_TC_01_003_002_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to suspend a thread for specified time with normal condition:
 * - Test01_003_002: sleep 0ms
 * @subsection ThreadSleepForTimePeriod_TC_01_003_002_tpts Test Procedure/Test Steps
 * -# Suspend thread 0ms with R_OSAL_ThreadSleepForTimePeriod.
 * @subsection ThreadSleepForTimePeriod_TC_01_003_002_ereb Expected Result/Behavior
 * -# R_OSAL_ThreadSleepForTimePeriod return OSAL_RETURN_OK.
 * @subsection ThreadSleepForTimePeriod_TC_01_003_002_testcase Test Case Example
 * @snippet test_thread.cpp ThreadSleepForTimePeriod_TC_01_003_002
 */
// [ThreadSleepForTimePeriod_TC_01_003_002]
TEST_F(ThreadApiTest, ThreadSleepForTimePeriod_TC_01_003_002)
{
    osal_milli_sec_t time;

// Test01_003_002
// @brief: normal case: sleep 0ms
    time = 0;
    ASSERT_EQ(R_OSAL_ThreadSleepForTimePeriod(time), OSAL_RETURN_OK) << "Test01_003_002 failed";
}
// [ThreadSleepForTimePeriod_TC_01_003_002]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadSleepForTimePeriod_TC_01_003_003 ThreadApiTest.ThreadSleepForTimePeriod_TC_01_003_003
 * @subsection ThreadSleepForTimePeriod_TC_01_003_003_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadSleepForTimePeriod_TC_01_003_003_tci Test Case ID
 * ThreadSleepForTimePeriod_S0_BA_TC_01_003_003
 * @subsection ThreadSleepForTimePeriod_TC_01_003_003_rr Related Requirement(s)
 * \#199129, \#199130, \#199141
 * @subsection ThreadSleepForTimePeriod_TC_01_003_003_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadSleepForTimePeriod_TC_01_003_003_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to suspend a thread for specified time with abnormal condition:
 * - Test01_003_003: sleep with negative number
 * @subsection ThreadSleepForTimePeriod_TC_01_003_003_tpts Test Procedure/Test Steps
 * -# Suspend thread -10ms with R_OSAL_ThreadSleepForTimePeriod.
 * @subsection ThreadSleepForTimePeriod_TC_01_003_003_ereb Expected Result/Behavior
 * -# R_OSAL_ThreadSleepForTimePeriod return OSAL_RETURN_PAR.
 * @subsection ThreadSleepForTimePeriod_TC_01_003_003_testcase Test Case Example
 * @snippet test_thread.cpp ThreadSleepForTimePeriod_TC_01_003_003
 */
// [ThreadSleepForTimePeriod_TC_01_003_003]
TEST_F(ThreadApiTest, ThreadSleepForTimePeriod_TC_01_003_003)
{
    osal_milli_sec_t time;

// Test01_003_003
// @brief: abnormal case: sleep with negative number
    time = -10;
    ASSERT_EQ(R_OSAL_ThreadSleepForTimePeriod(time), OSAL_RETURN_PAR) << "Test01_003_003 failed";
}
// [ThreadSleepForTimePeriod_TC_01_003_003]

/**
 * @page Thread_TEST Thread API Test
 * @section OsalApiStateTest_ThreadSleepForTimePeriod_TC_01_003_004 OsalApiStateTest.ThreadSleepForTimePeriod_TC_01_003_004
 * @subsection ThreadSleepForTimePeriod_TC_01_003_004_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection ThreadSleepForTimePeriod_TC_01_003_004_tci Test Case ID
 * ThreadSleepForTimePeriod_S2_A_TC_01_003_004
 * @subsection ThreadSleepForTimePeriod_TC_01_003_004_rr Related Requirement(s)
 * \#199141
 * @subsection ThreadSleepForTimePeriod_TC_01_003_004_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection ThreadSleepForTimePeriod_TC_01_003_004_tcs Test Case Summary
 * This test shall verify the state transition of Thread APIs with abnormal condition:
 * - Test01_003_004: state transition error with R_OSAL_ThreadSleepForTimePeriod.
 * @subsection ThreadSleepForTimePeriod_TC_01_003_004_tpts Test Procedure/Test Steps
 * -# Suspend thread with R_OSAL_ThreadSleepForTimePeriod.
 * @subsection ThreadSleepForTimePeriod_TC_01_003_004_ereb Expected Result/Behavior
 * -# R_OSAL_ThreadSleepForTimePeriod return OSAL_RETURN_STATE.
 * @subsection ThreadSleepForTimePeriod_TC_01_003_004_testcase Test Case Example
 * @snippet test_thread.cpp ThreadSleepForTimePeriod_TC_01_003_004
 */
// [ThreadSleepForTimePeriod_TC_01_003_004]
TEST_F(OsalApiStateTest, ThreadSleepForTimePeriod_TC_01_003_004)
{
    osal_milli_sec_t        time = 0;

// Test01_003_004
// @brief: abnormal case: state transition error with R_OSAL_ThreadSleepForTimePeriod
    EXPECT_EQ(R_OSAL_ThreadSleepForTimePeriod(time), OSAL_RETURN_STATE) << "Test01_003_004 failed";
}
// [ThreadSleepForTimePeriod_TC_01_003_004]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadSleepForTimePeriod_TC_01_003_005 ThreadApiTest.ThreadSleepForTimePeriod_TC_01_003_005
 * @subsection ThreadSleepForTimePeriod_TC_01_003_005_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadSleepForTimePeriod_TC_01_003_005_tci Test Case ID
 * ThreadSleepForTimePeriod_S1_N_TC_01_003_005
 * @subsection ThreadSleepForTimePeriod_TC_01_003_005_rr Related Requirement(s)
 * \#199130, \#199141
 * @subsection ThreadSleepForTimePeriod_TC_01_003_005_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadSleepForTimePeriod_TC_01_003_005_tcs Test Case Summary
 * This test shall measure the time execution of R_OSAL_ThreadSleepForTimePeriod with normal condition:
 * - Test01_003_005: measure the time execution of R_OSAL_ThreadSleepForTimePeriod when sleeping 10ms
 * @subsection ThreadSleepForTimePeriod_TC_01_003_005_tpts Test Procedure/Test Steps
 * -# Use the "for" loop in which we repeat 101 times with 4 steps as below:
 *      - Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *      - Suspend thread 10ms with R_OSAL_ThreadSleepForTimePeriod.
 *      - Get the current time stamp again by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *      - Get the execution time of R_OSAL_ThreadSleepForTimePeriod by calculating the different time using R_OSAL_ClockTimeCalculateTimeDifference.<br>
 *        Execution time is stored in an array "diff_time"
 * -# Find the median of "diff_time" array.<br>
 * Compare it with 10ms plus "delta_time" (TEST_MEDIAN_EXPECT)
 * @subsection ThreadSleepForTimePeriod_TC_01_003_005_ereb Expected Result/Behavior
 * -# The expected result of 4 steps in "for" loop are as below:
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 *      - R_OSAL_ThreadSleepForTimePeriod return OSAL_RETURN_OK.
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 *      - R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK.
 * -# Getting the median of "diff_time" array successfully.<br>
 * Median value need to be not exceed 10ms plus with "delta_time"
 * @subsection ThreadSleepForTimePeriod_TC_01_003_005_testcase Test Case Example
 * @snippet test_thread.cpp ThreadSleepForTimePeriod_TC_01_003_005
 * @snippet test_common.hpp findMedian
 */
// [ThreadSleepForTimePeriod_TC_01_003_005]
TEST_F(ThreadApiTest, ThreadSleepForTimePeriod_TC_01_003_005)
{
    st_osal_time_t  current_time_1st, current_time_2nd;
    osal_milli_sec_t time;
    osal_nano_sec_t diff_time[101];
    osal_nano_sec_t delta_time = TEST_MEDIAN_EXPECT;
    osal_nano_sec_t median = 0;
    int elements = 0;

// Test01_003_005
// @brief: normal case: measure execution time of R_OSAL_ThreadSleepForTimePeriod(10)
    time = 10;
    for(int i = 0; i <= 100; i++)
    {
        ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_1st), OSAL_RETURN_OK);
        ASSERT_EQ(R_OSAL_ThreadSleepForTimePeriod(time), OSAL_RETURN_OK) << "Test01_003_005 failed";
        ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_2nd), OSAL_RETURN_OK);
        ASSERT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &diff_time[i]), OSAL_RETURN_OK);
    }
    elements = sizeof(diff_time) / sizeof(diff_time[0]);
    median = (osal_nano_sec_t)findMedian(diff_time, elements);
    delta_time = TEST_MEDIAN_EXPECT;
    EXPECT_LE(median, time * MSEC_TO_NSEC + delta_time) << "Test01_003_005 failed";
}
// [ThreadSleepForTimePeriod_TC_01_003_005]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadSleepForTimePeriod_TC_01_003_006 ThreadApiTest.ThreadSleepForTimePeriod_TC_01_003_006
 * @subsection ThreadSleepForTimePeriod_TC_01_003_006_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadSleepForTimePeriod_TC_01_003_006_tci Test Case ID
 * ThreadSleepForTimePeriod_S1_N_TC_01_003_006
 * @subsection ThreadSleepForTimePeriod_TC_01_003_006_rr Related Requirement(s)
 * \#199130, \#199141
 * @subsection ThreadSleepForTimePeriod_TC_01_003_006_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadSleepForTimePeriod_TC_01_003_006_tcs Test Case Summary
 * This test shall measure the time execution of R_OSAL_ThreadSleepForTimePeriod with normal condition:
 * - Test01_003_006: measure the time execution of R_OSAL_ThreadSleepForTimePeriod when sleeping 0ms
 * @subsection ThreadSleepForTimePeriod_TC_01_003_006_tpts Test Procedure/Test Steps
 * -# Use the "for" loop in which we repeat 101 times with 4 steps as below:
 *      - Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *      - Suspend thread 0ms with R_OSAL_ThreadSleepForTimePeriod.
 *      - Get the current time stamp again by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *      - Get the execution time of R_OSAL_ThreadSleepForTimePeriod by calculating the different time using R_OSAL_ClockTimeCalculateTimeDifference.<br>
 *        Execution time is stored in an array "diff_time"
 * -# Find the median of "diff_time" array.<br>
 * Compare it with "delta_time" (TEST_MEDIAN_EXPECT)
 * @subsection ThreadSleepForTimePeriod_TC_01_003_006_ereb Expected Result/Behavior
 * -# The expected result of 4 steps in "for" loop are as below:
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 *      - R_OSAL_ThreadSleepForTimePeriod return OSAL_RETURN_OK.
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 *      - R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK.
 * -# Getting the median of "diff_time" array successfully.<br>
 * Median value need to be not exceed "delta_time"
 * @subsection ThreadSleepForTimePeriod_TC_01_003_006_testcase Test Case Example
 * @snippet test_thread.cpp ThreadSleepForTimePeriod_TC_01_003_006
 * @snippet test_common.hpp findMedian
 */
// [ThreadSleepForTimePeriod_TC_01_003_006]
TEST_F(ThreadApiTest, ThreadSleepForTimePeriod_TC_01_003_006)
{
    st_osal_time_t  current_time_1st, current_time_2nd;
    osal_milli_sec_t time;
    osal_nano_sec_t diff_time[101];
    osal_nano_sec_t delta_time = TEST_MEDIAN_EXPECT;
    osal_nano_sec_t median = 0;
    int elements = 0;

// Test01_003_006
// @brief: normal case: measure execution time of R_OSAL_ThreadSleepForTimePeriod(0)
    time = 0;
    for(int i = 0; i <= 100; i++)
    {
        ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_1st), OSAL_RETURN_OK);
        ASSERT_EQ(R_OSAL_ThreadSleepForTimePeriod(time), OSAL_RETURN_OK) << "Test01_003_006 failed";
        ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_2nd), OSAL_RETURN_OK);
        ASSERT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &diff_time[i]), OSAL_RETURN_OK);
    }
    elements = sizeof(diff_time) / sizeof(diff_time[0]);
    median = (osal_nano_sec_t)findMedian(diff_time, elements);
    delta_time = TEST_MEDIAN_EXPECT;
    EXPECT_LE(median, delta_time) << "Test01_003_006 failed";
}
// [ThreadSleepForTimePeriod_TC_01_003_006]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadSelf_TC_01_004_001 ThreadApiTest.ThreadSelf_TC_01_004_001
 * @subsection ThreadSelf_TC_01_004_001_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadSelf_TC_01_004_001_tci Test Case ID
 * ThreadSelf_S1_N_TC_01_004_001
 * @subsection ThreadSelf_TC_01_004_001_rr Related Requirement(s)
 * \#199143
 * @subsection ThreadSelf_TC_01_004_001_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadSelf_TC_01_004_001_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to obtain the handle of thread with normal condition:
 * - pre-process: create new thread.<br>
 * In worker function of "Simple testing thread":<br> 
 *      - Test01_004_001: get itself thread handle
 * - post-process: join created thread
 * @subsection ThreadSelf_TC_01_004_001_tpts Test Procedure/Test Steps
 * -# Create a thread by using R_OSAL_ThreadCreate  with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is threadself_workerfunction function with functionality:
 *              - Get the current thread handle by calling to R_OSAL_ThreadSelf.
 *          -# userarg is NULL
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Address of thread handle.
 * -# Join thread by using R_OSAL_ThreadJoin.
 * @subsection ThreadSelf_TC_01_004_001_ereb Expected Result/Behavior
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK. <br>
 * "thread_handle" is not NULL_PTR.
 * In creation_workerfunction function: <br>
 *  - R_OSAL_ThreadSelf return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK.
 * @subsection ThreadSelf_TC_01_004_001_testcase Test Case Example
 * @snippet test_thread.cpp ThreadSelf_TC_01_004_001
 * @snippet test_thread.cpp threadself_workerfunction
 */
// [ThreadSelf_TC_01_004_001]
TEST_F(ThreadApiTest, ThreadSelf_TC_01_004_001)
{
    osal_thread_id_t            uid;
    st_osal_thread_config_t     config;
    int64_t                     result;

// pre-process: create new thread
    uid              = TEST_THREAD_ID1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = (char *)"Simple testing thread";
    config.func      = threadself_workerfunction;
    config.userarg   = NULL;
    config.stack_size = THREAD_STACK_SIZE;

    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(thread_handle, NULL_PTR) << "pre-process failed";

// post-process: join created thread
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(result, 0) << "post-process failed";
}
// [ThreadSelf_TC_01_004_001]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadSelf_TC_01_004_002 ThreadApiTest.ThreadSelf_TC_01_004_002
 * @subsection ThreadSelf_TC_01_004_002_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadSelf_TC_01_004_002_tci Test Case ID
 * ThreadSelf_S0_A_TC_01_004_002
 * @subsection ThreadSelf_TC_01_004_002_rr Related Requirement(s)
 * \#199133, \#199143, \#199151
 * @subsection ThreadSelf_TC_01_004_002_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadSelf_TC_01_004_002_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to obtain the handle of thread with abnormal condition:
 * - Test01_004_002: p_handle is NULL
 * @subsection ThreadSelf_TC_01_004_002_tpts Test Procedure/Test Steps
 * -# Get the current thread handle by calling to R_OSAL_ThreadSelf with the argument as below:
 *      - thread handle is NULL
 * @subsection ThreadSelf_TC_01_004_002_ereb Expected Result/Behavior
 * -# R_OSAL_ThreadSelf return OSAL_RETURN_PAR.
 * @subsection ThreadSelf_TC_01_004_002_testcase Test Case Example
 * @snippet test_thread.cpp ThreadSelf_TC_01_004_002
 */
// [ThreadSelf_TC_01_004_002]
TEST_F(ThreadApiTest, ThreadSelf_TC_01_004_002)
{
    osal_thread_handle_t* p_handle = NULL;

// Test01_004_002
// @brief: abnormal case: p_handle is NULL
    ASSERT_EQ(R_OSAL_ThreadSelf(p_handle), OSAL_RETURN_PAR) << "Test01_004_002 failed";
}
// [ThreadSelf_TC_01_004_002]

/**
 * @page Thread_TEST Thread API Test
 * @section OsalApiStateTest_ThreadSelf_TC_01_004_003 OsalApiStateTest.ThreadSelf_TC_01_004_003
 * @subsection ThreadSelf_TC_01_004_003_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection ThreadSelf_TC_01_004_003_tci Test Case ID
 * ThreadSelf_S2_A_TC_01_004_003
 * @subsection ThreadSelf_TC_01_004_003_rr Related Requirement(s)
 * \#199143
 * @subsection ThreadSelf_TC_01_004_003_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection ThreadSelf_TC_01_004_003_tcs Test Case Summary
 * This test shall verify the state transition of Thread APIs with abnormal condition:
 * - Test01_004_003: state transition error with R_OSAL_ThreadSelf.
 * @subsection ThreadSelf_TC_01_004_003_tpts Test Procedure/Test Steps
 * -# Get the current thread handle by calling to R_OSAL_ThreadSelf.
 * @subsection ThreadSelf_TC_01_004_003_ereb Expected Result/Behavior
 * -# R_OSAL_ThreadSelf return OSAL_RETURN_STATE.
 * @subsection ThreadSelf_TC_01_004_003_testcase Test Case Example
 * @snippet test_thread.cpp ThreadSelf_TC_01_004_003
 */
// [ThreadSelf_TC_01_004_003]
TEST_F(OsalApiStateTest, ThreadSelf_TC_01_004_003)
{
    osal_thread_handle_t        *thread_self = NULL;

// Test01_004_003
// @brief: abnormal case: state transition error with R_OSAL_ThreadSelf
    EXPECT_EQ(R_OSAL_ThreadSelf(thread_self), OSAL_RETURN_STATE) << "Test01_004_003 failed";
}
// [ThreadSelf_TC_01_004_003]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadSelf_TC_01_004_004 ThreadApiTest.ThreadSelf_TC_01_004_004
 * @subsection ThreadSelf_TC_01_004_004_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadSelf_TC_01_004_004_tci Test Case ID
 * ThreadSelf_S2_A_TC_01_004_004
 * @subsection ThreadSelf_TC_01_004_004_rr Related Requirement(s)
 * \#199143
 * @subsection ThreadSelf_TC_01_004_004_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadSelf_TC_01_004_004_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to obtain the handle of the thread that is not created by the OSAL function.
 * - Test01_004_004: Getting thread handle of thread which is not created by using OSAL APIs.
 * @subsection ThreadSelf_TC_01_004_004_tpts Test Procedure/Test Steps
 * -# Getting thread handle of thread which is not created by using OSAL APIs.
 * @subsection ThreadSelf_TC_01_004_004_ereb Expected Result/Behavior
 * -# R_OSAL_ThreadSelf return OSAL_RETURN_FAIL.
 * @subsection ThreadSelf_TC_01_004_004_testcase Test Case Example
 * @snippet test_thread.cpp ThreadSelf_TC_01_004_004
 */
// [ThreadSelf_TC_01_004_004]
TEST_F(ThreadApiTest, ThreadSelf_TC_01_004_004)
{
// Test01_004_004
// @brief: abnormal case: Getting thread handle of thread which is not created by using OSAL APIs
    EXPECT_EQ(R_OSAL_ThreadSelf(&thread_handle), OSAL_RETURN_FAIL)  << "Test01_004_004 failed";
}
// [ThreadSelf_TC_01_004_004]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadEqual_TC_01_005_001 ThreadApiTest.ThreadEqual_TC_01_005_001
 * @subsection ThreadEqual_TC_01_005_001_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadEqual_TC_01_005_001_tci Test Case ID
 * ThreadEqual_S3_N_TC_01_005_001
 * @subsection ThreadEqual_TC_01_005_001_rr Related Requirement(s)
 * \#199144
 * @subsection ThreadEqual_TC_01_005_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - The synchronization message queue was created.
 * @subsection ThreadEqual_TC_01_005_001_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to compare two thread handles with normal condition:
 * - pre-process: create new thread named "Testing thread"
 * - Test01_005_001: checking with the same thread handle
 * - post-process: join created thread
 * @subsection ThreadEqual_TC_01_005_001_tpts Test Procedure/Test Steps
 * -# Create new thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE2
 *          -# task_name is "Testing thread"
 *          -# func is threadequal_workerfunction1 function with functionality:
 *              - Receive the message from synchronization MQ by using R_OSAL_MqReceiveForTimePeriod
 *              - Get the current thread handle by calling to R_OSAL_ThreadSelf.
 *              - Compare the thread handles by using R_OSAL_ThreadEqual:
 *                  -# First handle: Got from R_OSAL_ThreadSelf
 *                  -# Second handle: Got from R_OSAL_ThreadCreate after new thread is created successfully.
 *          -# userarg includes thread handle of new thread
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2.
 *      - Address of new thread handle.
 * -# Send a message to synchronization MQ by using R_OSAL_MqSendForTimePeriod
 * -# Join created thread by using R_OSAL_ThreadJoin.
 * @subsection ThreadEqual_TC_01_005_001_ereb Expected Result/Behavior
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with second thread.<br>
 * "thread_handle1" is not NULL_PTR.<br>
 * In threadequal_workerfunction1 function: <br>
 *  - R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK.<br>
 * the value of "receive_msg" is equal to value of "send_msg"
 *  - R_OSAL_ThreadSelf return OSAL_RETURN_OK.<br>
 *  - R_OSAL_ThreadEqual return OSAL_RETURN_OK.<br>
 * The value of "same" is true.
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK.<br>
 * Value of "result" is equal to 0.
 * @subsection ThreadEqual_TC_01_005_001_testcase Test Case Example
 * @snippet test_thread.cpp ThreadEqual_TC_01_005_001
 * @snippet test_thread.cpp threadequal_workerfunction1
 */
// [ThreadEqual_TC_01_005_001]
TEST_F(ThreadApiTest, ThreadEqual_TC_01_005_001)
{
    osal_thread_id_t            uid;
    st_osal_thread_config_t     config;
    int64_t                     result;

// pre-process: 1 new thread
    threadequal_worker_arg user_arg(NULL_PTR, &thread_handle1);
    user_arg.mq = sync_mq;
    user_arg.send_msg = send_msg;
    uid              = TEST_THREAD_ID2;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE2;
    config.task_name = (char *)"Testing thread";
    config.func      = threadequal_workerfunction1;
    config.userarg   = &user_arg;
    config.stack_size = THREAD_STACK_SIZE;

    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle1), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(thread_handle1, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(sync_mq, SYNC_MQ_TIME, &send_msg, mq_config.msg_size), OSAL_RETURN_OK);

// post-process: join created thread
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &result), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(result, 0) << "post-process failed";
    thread_handle1 = OSAL_THREAD_HANDLE_INVALID;
}
// [ThreadEqual_TC_01_005_001]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadEqual_TC_01_005_002 ThreadApiTest.ThreadEqual_TC_01_005_002
 * @subsection ThreadEqual_TC_01_005_002_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadEqual_TC_01_005_002_tci Test Case ID
 * ThreadEqual_S1_N_TC_01_005_002
 * @subsection ThreadEqual_TC_01_005_002_rr Related Requirement(s)
 * \#199144
 * @subsection ThreadEqual_TC_01_005_002_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadEqual_TC_01_005_002_tcs Test Case Summary
 * This test verifies the functionality of R_OSAL_ThreadEqual when two different thread handles are entered.
 * - pre-process: create 2 new threads named "Testing thread".
 * - Test01_005_002: compare two thread handles by using R_OSAL_ThreadEqual
 * - post-process: join 2 created threads
 * @subsection ThreadEqual_TC_01_005_002_tpts Test Procedure/Test Steps
 * -# Create threads by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE2
 *          -# task_name is "Testing thread"
 *          -# func is threadequal_workerfunction2 function with functionality:
 *              - None.
 *          -# userarg is NULL.
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Address of first thread handle.
 * -# Create second thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: second thread config is same to first thread config.
 *      - thread_id is TEST_THREAD_ID2.
 *      - Address of second thread handle.
 * -# compare thread1 handle and thread2 handle whether same by using R_OSAL_ThreadEqual.
 * -# Join first thread by using R_OSAL_ThreadJoin.
 * -# Join second thread by using R_OSAL_ThreadJoin.
 * @subsection ThreadEqual_TC_01_005_002_ereb Expected Result/Behavior
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with first thread. <br>
 * "thread_handle" is not NULL_PTR.<br>
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with second thread.<br>
 * "thread_handle1" is not NULL_PTR.<br>
 * -# R_OSAL_ThreadEqual with "thread_handle" and "thread_handle1" return OSAL_RETURN_OK and "same" is false.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with first thread.<br>
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with second thread. <br>
 * @subsection ThreadEqual_TC_01_005_002_testcase Test Case Example
 * @snippet test_thread.cpp ThreadEqual_TC_01_005_002
 * @snippet test_thread.cpp threadequal_workerfunction2
 */
// [ThreadEqual_TC_01_005_002]
TEST_F(ThreadApiTest, ThreadEqual_TC_01_005_002)
{
    st_osal_thread_config_t     config;
    int64_t                     result;
    bool                        same;

// pre-process: set test thread configuration
    config.priority  = OSAL_THREAD_PRIORITY_TYPE2;
    config.task_name = (char *)"Testing thread";
    config.func      = threadequal_workerfunction2;
    config.userarg   = NULL;
    config.stack_size = THREAD_STACK_SIZE;

// pre-process: create 2 new threads
    ASSERT_EQ(R_OSAL_ThreadCreate(&config, TEST_THREAD_ID1, &thread_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(thread_handle, NULL_PTR) << "pre-process failed";
    ASSERT_EQ(R_OSAL_ThreadCreate(&config, TEST_THREAD_ID2, &thread_handle1), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(thread_handle1, NULL_PTR) << "pre-process failed";

// Test01_005_002
// @brief: normal case: checking with the different thread handle
    EXPECT_EQ(R_OSAL_ThreadEqual(thread_handle, thread_handle1, &same), OSAL_RETURN_OK) << "Test01_005_002 failed";
    EXPECT_FALSE(same) << "Test01_005_002 failed";

// post-process: check return value from each thread
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &result), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK) << "post-process failed";

// post-process: Delete Threadsync hanldes
    thread_handle = OSAL_THREAD_HANDLE_INVALID;
    thread_handle1 = OSAL_THREAD_HANDLE_INVALID;
}
// [ThreadEqual_TC_01_005_002]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadEqual_TC_01_005_003 ThreadApiTest.ThreadEqual_TC_01_005_003
 * @subsection ThreadEqual_TC_01_005_003_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadEqual_TC_01_005_003_tci Test Case ID
 * ThreadEqual_S0_A_TC_01_005_003
 * @subsection ThreadEqual_TC_01_005_003_rr Related Requirement(s)
 * \#199133, \#199144, \#199151
 * @subsection ThreadEqual_TC_01_005_003_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadEqual_TC_01_005_003_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to compare two thread handles with abnormal condition:
 * - pre-proces: create new thread
 * - Test01_005_003: the first thread handle is NULL.
 * - post-process: join created thread
 * @subsection ThreadEqual_TC_01_005_003_tpts Test Procedure/Test Steps
 * -# Create new thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -#  priority is OSAL_THREAD_PRIORITY_TYPE3
 *          -# task_name is "Simple testing thread"
 *          -# func is simple_workerfunction function with functionality:
 *              - Suspend thread 500ms with R_OSAL_ThreadSleepForTimePeriod.
 *          -# userarg is NULL
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2.
 *      - Address of "thread_handle1".
 * -# Compare the thread handles by using R_OSAL_ThreadEqual with the argument as below:
 *      - First handle is NULL
 *      - Second handle: Got from R_OSAL_ThreadCreate after thread is created successfully.
 * -# Join thread by using R_OSAL_ThreadJoin.
 * @subsection ThreadEqual_TC_01_005_003_ereb Expected Result/Behavior
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK.<br>
 * "thread_handle1" is not NULL_PTR.<br>
 * In simple_workerfunction function: <br>
 *  - R_OSAL_ThreadSleepForTimePeriod return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadEqual return OSAL_RETURN_HANDLE.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK.<br>
 * Value of "result" is equal to 0. <br>
 * @subsection ThreadEqual_TC_01_005_003_testcase Test Case Example
 * @snippet test_thread.cpp ThreadEqual_TC_01_005_003
 * @snippet test_thread.cpp simple_workerfunction
 */
// [ThreadEqual_TC_01_005_003]
TEST_F(ThreadApiTest, ThreadEqual_TC_01_005_003)
{
    osal_thread_id_t            uid;
    st_osal_thread_config_t     config;
    int64_t                     result;
    bool                        same;

// pre-process: create new thread
    uid              = TEST_THREAD_ID2;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE3;
    config.task_name = (char *)"Simple testing thread";
    config.func      = simple_workerfunction;
    config.userarg   = NULL;
    config.stack_size = THREAD_STACK_SIZE;

    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle1), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(thread_handle1, NULL_PTR) << "pre-process failed";

// Test01_005_003
// @brief: abnormal case: the first thread handle is NULL
    ASSERT_EQ(R_OSAL_ThreadEqual(OSAL_THREAD_HANDLE_INVALID, thread_handle1, &same), OSAL_RETURN_HANDLE) << "Test01_005_003 failed";

// post-process: join created thread
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &result), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(result, 0) << "post-process failed";
}
// [ThreadEqual_TC_01_005_003]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadEqual_TC_01_005_004 ThreadApiTest.ThreadEqual_TC_01_005_004
 * @subsection ThreadEqual_TC_01_005_004_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadEqual_TC_01_005_004_tci Test Case ID
 * ThreadEqual_S0_A_TC_01_005_004
 * @subsection ThreadEqual_TC_01_005_004_rr Related Requirement(s)
 * \#199133, \#199144, \#199151
 * @subsection ThreadEqual_TC_01_005_004_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadEqual_TC_01_005_004_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to compare two thread handles with abnormal condition:
 * - pre-proces: create new thread
 * - Test01_005_004: the second thread handle is NULL.
 * - post-process: join created thread
 * @subsection ThreadEqual_TC_01_005_004_tpts Test Procedure/Test Steps
 * -# Create new thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -#  priority is OSAL_THREAD_PRIORITY_TYPE3
 *          -# task_name is "Simple testing thread"
 *          -# func is simple_workerfunction function with functionality:
 *              - Suspend thread 500ms with R_OSAL_ThreadSleepForTimePeriod.
 *          -# userarg is NULL
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Address of "thread_handle".
 * -# Compare the thread handles by using R_OSAL_ThreadEqual with the argument as below:
 *      - First handle: Got from R_OSAL_ThreadCreate after thread is created successfully
 *      - Second handle is NULL
 * -# Join thread by using R_OSAL_ThreadJoin.
 * @subsection ThreadEqual_TC_01_005_004_ereb Expected Result/Behavior
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK.<br>
 * "thread_handle" is not NULL_PTR.<br>
 * In simple_workerfunction function: <br>
 *  - R_OSAL_ThreadSleepForTimePeriod return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadEqual return OSAL_RETURN_HANDLE.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK.<br>
 * Value of "result" is equal to 0. <br>
 * @subsection ThreadEqual_TC_01_005_004_testcase Test Case Example
 * @snippet test_thread.cpp ThreadEqual_TC_01_005_004
 * @snippet test_thread.cpp simple_workerfunction
 */
// [ThreadEqual_TC_01_005_004]
TEST_F(ThreadApiTest, ThreadEqual_TC_01_005_004)
{
    osal_thread_id_t            uid;
    st_osal_thread_config_t     config;
    int64_t                     result;
    bool                        same;

// pre-process: create new thread
    uid              = TEST_THREAD_ID1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE3;
    config.task_name = (char *)"Simple testing thread";
    config.func      = simple_workerfunction;
    config.userarg   = NULL;
    config.stack_size = THREAD_STACK_SIZE;

    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(thread_handle, NULL_PTR) << "pre-process failed";

// Test01_005_004
// @brief: abnormal case: the second thread handle is NULL
    ASSERT_EQ(R_OSAL_ThreadEqual(thread_handle, OSAL_THREAD_HANDLE_INVALID, &same), OSAL_RETURN_HANDLE) << "Test01_005_004 failed";

// post-process: join created thread
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(result, 0) << "post-process failed";
}
// [ThreadEqual_TC_01_005_004]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadEqual_TC_01_005_005 ThreadApiTest.ThreadEqual_TC_01_005_005
 * @subsection ThreadEqual_TC_01_005_005_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadEqual_TC_01_005_005_tci Test Case ID
 * ThreadEqual_S0_A_TC_01_005_005
 * @subsection ThreadEqual_TC_01_005_005_rr Related Requirement(s)
 * \#199144
 * @subsection ThreadEqual_TC_01_005_005_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadEqual_TC_01_005_005_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to compare two thread handles with abnormal condition:
 * - pre-proces: create 2 new threads
 * - Test01_005_005: p_result is NULL.
 * - post-process: join 2 created threads
 * @subsection ThreadEqual_TC_01_005_005_tpts Test Procedure/Test Steps
 * -# Create 1st thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -#  priority is OSAL_THREAD_PRIORITY_TYPE3
 *          -# task_name is "Simple testing thread"
 *          -# func is simple_workerfunction function with functionality:
 *              - Suspend thread 500ms with R_OSAL_ThreadSleepForTimePeriod.
 *          -# userarg is NULL
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Address of thread handle.
 * -# Create 2nd thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE3
 *          -# task_name is "Simple testing thread"
 *          -# func is simple_workerfunction function with functionality:
 *              - Suspend thread 500ms with R_OSAL_ThreadSleepForTimePeriod.
 *          -# userarg is NULL
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2.
 *      - Address of thread handle.
 * -# Compare the thread handles by using R_OSAL_ThreadEqual with the argument as below:
 *      - First handle: Got from R_OSAL_ThreadCreate after first thread is created successfully
 *      - Second handle: Got from R_OSAL_ThreadCreate after second thread is created successfully
 *      - The result of comparison is NULL
 * -# Join first thread by using R_OSAL_ThreadJoin.
 * -# Join second thread by using R_OSAL_ThreadJoin.
 * @subsection ThreadEqual_TC_01_005_005_ereb Expected Result/Behavior
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with first thread.<br>
 * "thread_handle" is not NULL_PTR.<br>
 * In simple_workerfunction function: <br>
 *  - R_OSAL_ThreadSleepForTimePeriod return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with second thread.<br>
 * "thread_handle1" is not NULL_PTR.<br>
 * In simple_workerfunction function: <br>
 *  - R_OSAL_ThreadSleepForTimePeriod return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadEqual return OSAL_RETURN_PAR.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with first thread.<br>
 * Value of "result" is equal to 0. <br>
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with second thread. <br>
 * Value of "result" is equal to 0.
 * @subsection ThreadEqual_TC_01_005_005_testcase Test Case Example
 * @snippet test_thread.cpp ThreadEqual_TC_01_005_005
 * @snippet test_thread.cpp simple_workerfunction
 */
// [ThreadEqual_TC_01_005_005]
TEST_F(ThreadApiTest, ThreadEqual_TC_01_005_005)
{
    osal_thread_id_t            uid;
    st_osal_thread_config_t     config;
    int64_t                     result;
    bool                        same;
    bool*                       p_result = NULL;

// pre-process: create 2 new threads
    uid              = TEST_THREAD_ID1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE3;
    config.task_name = (char *)"Simple testing thread";
    config.func      = simple_workerfunction;
    config.userarg   = NULL;
    config.stack_size = THREAD_STACK_SIZE;

    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(thread_handle, NULL_PTR) << "pre-process failed";

    uid              = TEST_THREAD_ID2;
    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle1), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(thread_handle1, NULL_PTR) << "pre-process failed";

// Test01_005_005
// @brief: abnormal case: p_result is NULL
    ASSERT_EQ(R_OSAL_ThreadEqual(thread_handle, thread_handle1, p_result), OSAL_RETURN_PAR)  << "Test01_005_005 failed";

// post-process: join 2 created threads
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &result), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(result, 0) << "post-process failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(result, 0) << "post-process failed";
}
// [ThreadEqual_TC_01_005_005]

/**
 * @page Thread_TEST Thread API Test
 * @section OsalApiStateTest_ThreadEqual_TC_01_005_006 OsalApiStateTest.ThreadEqual_TC_01_005_006
 * @subsection ThreadEqual_TC_01_005_006_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection ThreadEqual_TC_01_005_006_tci Test Case ID
 * ThreadEqual_S2_A_TC_01_005_006
 * @subsection ThreadEqual_TC_01_005_006_rr Related Requirement(s)
 * \#199144
 * @subsection ThreadEqual_TC_01_005_006_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection ThreadEqual_TC_01_005_006_tcs Test Case Summary
 * This test shall verify the state transition of Thread APIs with abnormal condition:
 * - Test01_005_006: state transition error with R_OSAL_ThreadEqual.
 * @subsection ThreadEqual_TC_01_005_006_tpts Test Procedure/Test Steps
 * -# Compare two thread handles by using R_OSAL_ThreadEqual.
 * @subsection ThreadEqual_TC_01_005_006_ereb Expected Result/Behavior
 * -# R_OSAL_ThreadEqual return OSAL_RETURN_STATE.
 * @subsection ThreadEqual_TC_01_005_006_testcase Test Case Example
 * @snippet test_thread.cpp ThreadEqual_TC_01_005_006
 */
// [ThreadEqual_TC_01_005_006]
TEST_F(OsalApiStateTest, ThreadEqual_TC_01_005_006)
{
    bool                        same;
    osal_thread_handle_t        thread_handle = OSAL_THREAD_HANDLE_INVALID;

// Test01_005_006
// @brief: abnormal case: state transition error with R_OSAL_ThreadEqual
    EXPECT_EQ(R_OSAL_ThreadEqual(OSAL_THREAD_HANDLE_INVALID, thread_handle, &same), OSAL_RETURN_STATE) << "Test01_005_006 failed";
}
// [ThreadEqual_TC_01_005_006]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadJoin_TC_01_006_001 ThreadApiTest.ThreadJoin_TC_01_006_001
 * @subsection ThreadJoin_TC_01_006_001_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadJoin_TC_01_006_001_tci Test Case ID
 * ThreadJoin_S1_N_TC_01_006_001
 * @subsection ThreadJoin_TC_01_006_001_rr Related Requirement(s)
 * \#199129, \#199145
 * @subsection ThreadJoin_TC_01_006_001_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadJoin_TC_01_006_001_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to join a thread with normal condition:
 * - pre-process: create new thread
 * - Test01_006_001: all input/output param is valid.
 * @subsection ThreadJoin_TC_01_006_001_tpts Test Procedure/Test Steps
 * -# Create a thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is simple_workerfunction function with functionality:
 *              - Suspend thread 500ms with R_OSAL_ThreadSleepForTimePeriod.
 *          -# userarg is NULL
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Address of thread handle.
 * -# Join thread by using R_OSAL_ThreadJoin.
 * @subsection ThreadJoin_TC_01_006_001_ereb Expected Result/Behavior
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK.<br>
 * "thread_handle" is not NULL_PTR.<br>
 * In simple_workerfunction function:<br>
 *  - R_OSAL_ThreadSleepForTimePeriod return OSAL_RETURN_OK.<br>
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK.<br>
 * Value of "result" is equal to 0.
 * @subsection ThreadJoin_TC_01_006_001_testcase Test Case Example
 * @snippet test_thread.cpp ThreadJoin_TC_01_006_001
 * @snippet test_thread.cpp simple_workerfunction
 */
// [ThreadJoin_TC_01_006_001]
TEST_F(ThreadApiTest, ThreadJoin_TC_01_006_001)
{
    osal_thread_id_t            uid;
    st_osal_thread_config_t     config;
    int64_t                     result;

// pre-process: create new thread
    uid              = TEST_THREAD_ID1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = (char *)"Simple testing thread";
    config.func      = simple_workerfunction;
    config.userarg   = NULL;
    config.stack_size = THREAD_STACK_SIZE;

    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(thread_handle, NULL_PTR) << "pre-process failed";

// Test01_006_001
// @brief: normal case: all input/output param is valid
    ASSERT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK) << "Test01_006_001 failed";
    ASSERT_EQ(result, 0) << "Test01_006_001 failed";
    thread_handle = OSAL_THREAD_HANDLE_INVALID;
}
// [ThreadJoin_TC_01_006_001]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadJoin_TC_01_006_002 ThreadApiTest.ThreadJoin_TC_01_006_002
 * @subsection ThreadJoin_TC_01_006_002_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadJoin_TC_01_006_002_tci Test Case ID
 * ThreadJoin_S0_A_TC_01_006_002
 * @subsection ThreadJoin_TC_01_006_002_rr Related Requirement(s)
 * \#199129, \#199133, \#199145, \#199151
 * @subsection ThreadJoin_TC_01_006_002_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadJoin_TC_01_006_002_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to join a thread with abnormal condition:
 * - Test01_006_002: thread handle is NULL.
 * @subsection ThreadJoin_TC_01_006_002_tpts Test Procedure/Test Steps
 * -# Join thread by using R_OSAL_ThreadJoin with the argument as below:
 *      - thread handle is NULL
 * @subsection ThreadJoin_TC_01_006_002_ereb Expected Result/Behavior
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_HANDLE.
 * @subsection ThreadJoin_TC_01_006_002_testcase Test Case Example
 * @snippet test_thread.cpp ThreadJoin_TC_01_006_002
 */
// [ThreadJoin_TC_01_006_002]
TEST_F(ThreadApiTest, ThreadJoin_TC_01_006_002)
{
    int64_t                     result;

// Test01_006_002
// @brief: abnormal case: thread handle is NULL
    ASSERT_EQ(R_OSAL_ThreadJoin(OSAL_THREAD_HANDLE_INVALID, &result), OSAL_RETURN_HANDLE) << "Test01_006_002 failed";
}
// [ThreadJoin_TC_01_006_002]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadJoin_TC_01_006_003 ThreadApiTest.ThreadJoin_TC_01_006_003
 * @subsection ThreadJoin_TC_01_006_003_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadJoin_TC_01_006_003_tci Test Case ID
 * ThreadJoin_S4_A_TC_01_006_003
 * @subsection ThreadJoin_TC_01_006_003_rr Related Requirement(s)
 * \#199129, \#199145
 * @subsection ThreadJoin_TC_01_006_003_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadJoin_TC_01_006_003_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to join a thread with abnormal condition:
 * - Test01_006_003: join itself.
 * @subsection ThreadJoin_TC_01_006_003_tpts Test Procedure/Test Steps
 * -# Create a thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE2
 *          -# task_name is "Simple testing thread"
 *          -# func is threadjoin_workerfunction_joinitself function with functionality:
 *              - Get the current thread handle by calling to R_OSAL_ThreadSelf.
 *              - Join thread by itself by using R_OSAL_ThreadJoin with the argument as below:
 *                  -# "thread_handle" got from R_OSAL_ThreadSelf
 *          -# userarg is NULL
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Address of thread handle.
 * -# Join thread by using R_OSAL_ThreadJoin.
 * @subsection ThreadJoin_TC_01_006_003_ereb Expected Result/Behavior
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK.<br>
 * Value of "result" is equal to 0.<br>
 * In threadjoin_workerfunction_joinitself function:<br>
 *  - R_OSAL_ThreadSelf return OSAL_RETURN_OK.
 *  - R_OSAL_ThreadJoin return OSAL_RETURN_HANDLE.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK.
 * @subsection ThreadJoin_TC_01_006_003_testcase Test Case Example
 * @snippet test_thread.cpp ThreadJoin_TC_01_006_003
 * @snippet test_thread.cpp threadjoin_workerfunction_joinitself
 */
// [ThreadJoin_TC_01_006_003]
TEST_F(ThreadApiTest, ThreadJoin_TC_01_006_003)
{
    osal_thread_id_t            uid;
    st_osal_thread_config_t     config;
    int64_t                     result;

// Test01_006_003
// @brief: abnormal case: join itself
    uid              = TEST_THREAD_ID1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE2;
    config.task_name = (char *)"Simple testing thread";
    config.func      = threadjoin_workerfunction_joinitself;
    config.userarg   = NULL;
    config.stack_size = THREAD_STACK_SIZE;

    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK);
    ASSERT_NE(thread_handle, NULL_PTR);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK) << "Test01_006_003 failed";
    EXPECT_EQ(result, 0);
    thread_handle = OSAL_THREAD_HANDLE_INVALID;
}
// [ThreadJoin_TC_01_006_003]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadJoin_TC_01_006_004 ThreadApiTest.ThreadJoin_TC_01_006_004
 * @subsection ThreadJoin_TC_01_006_004_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadJoin_TC_01_006_004_tci Test Case ID
 * ThreadJoin_S4_N_TC_01_006_004
 * @subsection ThreadJoin_TC_01_006_004_rr Related Requirement(s)
 * \#199129, \#199145
 * @subsection ThreadJoin_TC_01_006_004_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadJoin_TC_01_006_004_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to join a thread with normal condition:
 * - pre-process: create new thread
 * - Test01_006_004: join a thread which has already terminated.
 * @subsection ThreadJoin_TC_01_006_004_tpts Test Procedure/Test Steps
 * -# Create a thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is simple_workerfunction function with functionality:
 *              - Suspend thread 500ms with R_OSAL_ThreadSleepForTimePeriod.
 *          -# userarg is NULL
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Address of thread handle.
 * -# Join thread by using R_OSAL_ThreadJoin.
 * -# Join thread which has already terminated by using R_OSAL_ThreadJoin.
 * @subsection ThreadJoin_TC_01_006_004_ereb Expected Result/Behavior
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK.<br>
 * "thread_handle" is not NULL_PTR.<br>
 * In simple_workerfunction function:<br>
 *  - R_OSAL_ThreadSleepForTimePeriod return OSAL_RETURN_OK.<br>
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK.<br>
 * Value of "result" is equal to 0.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_HANDLE.
 * @subsection ThreadJoin_TC_01_006_004_testcase Test Case Example
 * @snippet test_thread.cpp ThreadJoin_TC_01_006_004
 * @snippet test_thread.cpp simple_workerfunction
 */
// [ThreadJoin_TC_01_006_004]
TEST_F(ThreadApiTest, ThreadJoin_TC_01_006_004)
{
    osal_thread_id_t            uid;
    st_osal_thread_config_t     config;
    int64_t                     result;

// pre-process: create new thread
    uid              = TEST_THREAD_ID1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = (char *)"Simple testing thread";
    config.func      = simple_workerfunction;
    config.userarg   = NULL;
    config.stack_size = THREAD_STACK_SIZE;

    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(thread_handle, NULL_PTR) << "pre-process failed";

// Test01_006_004
// @brief: abnormal case: join a thread which has already terminated.
    ASSERT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK);
    ASSERT_EQ(result, 0);
    thread_handle = OSAL_THREAD_HANDLE_INVALID;
    ASSERT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_HANDLE) << "Test01_006_004 failed";
    thread_handle = OSAL_THREAD_HANDLE_INVALID;
}
// [ThreadJoin_TC_01_006_004]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadJoin_TC_01_006_005 ThreadApiTest.ThreadJoin_TC_01_006_005
 * @subsection ThreadJoin_TC_01_006_005_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadJoin_TC_01_006_005_tci Test Case ID
 * ThreadJoin_S4_A_TC_01_006_005
 * @subsection ThreadJoin_TC_01_006_005_rr Related Requirement(s)
 * \#199129, \#199145
 * @subsection ThreadJoin_TC_01_006_005_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadJoin_TC_01_006_005_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to join a thread with abnormal condition:
 * - Test01_006_005: join thread which under joining by another thread.
 * @subsection ThreadJoin_TC_01_006_005_tpts Test Procedure/Test Steps
 * -# Create first thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is simple_workerfunction function with functionality:
 *              - Suspend thread 500ms with R_OSAL_ThreadSleepForTimePeriod.
 *          -# userarg is NULL
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Address of first thread handle.
 * -# Create second thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE2
 *          -# task_name is "Simple testing thread"
 *          -# func is threadjoin_workerfunction_joinedthread1 function with functionality:
 *              - Join "Simple testing thread" thread by using R_OSAL_ThreadJoin.
 *          -# userarg includes thread handle and return value of the second thread.
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2.
 *      - Address of second thread handle.
 * -# Create third thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE3
 *          -# task_name is "Simple testing thread"
 *          -# func is threadjoin_workerfunction_joinedthread2 function with functionality:
 *              - Join "Simple testing thread" thread by using R_OSAL_ThreadJoin.
 *          -# userarg includes thread handle and return value of third thread
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID3.
 *      - Address of third thread handle.
 * -# Join second thread by using R_OSAL_ThreadJoin.
 * -# Join third thread by using R_OSAL_ThreadJoin.
 * @subsection ThreadJoin_TC_01_006_005_ereb Expected Result/Behavior
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with first thread.<br>
 * "thread_handle" is not NULL_PTR. <br>
 * In simple_workerfunction function:<br>
 *  - R_OSAL_ThreadSleepForTimePeriod return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with second thread.<br>
 * Value of "result" is equal to 0.<br> 
 * "thread_handle1" is not NULL_PTR.<br>
 * In threadjoin_workerfunction_joinedthread1 function:<br>
 *  - R_OSAL_ThreadJoin return OSAL_RETURN_OK or OSAL_RETURN_HANDLE
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK for third thread.<br>
 * Value of "thread_handle2" is not NULL.<br>
 * In threadjoin_workerfunction_joinedthread2 function:<br>
 *  - R_OSAL_ThreadJoin return OSAL_RETURN_OK or OSAL_RETURN_HANDLE
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK for second thread.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK for third thread.
 * @subsection ThreadJoin_TC_01_006_005_testcase Test Case Example
 * @snippet test_thread.cpp ThreadJoin_TC_01_006_005
 * @snippet test_thread.cpp simple_workerfunction
 * @snippet test_thread.cpp threadjoin_workerfunction_joinedthread1
 * @snippet test_thread.cpp threadjoin_workerfunction_joinedthread2
 */
// [ThreadJoin_TC_01_006_005]
TEST_F(ThreadApiTest, ThreadJoin_TC_01_006_005)
{
    osal_thread_id_t            uid;
    st_osal_thread_config_t     config;
    int64_t                     result1, result2;

// Test01_006_005
// @brief: abnormal case: join thread which under joining by another thread
    uid              = TEST_THREAD_ID1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = (char *)"Simple testing thread";
    config.func      = simple_workerfunction;
    config.userarg   = NULL;
    config.stack_size = THREAD_STACK_SIZE;

    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK);
    ASSERT_NE(thread_handle, NULL_PTR);

    threadjoin_worker_arg user_arg(&thread_handle);
    uid              = TEST_THREAD_ID2;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE2;
    config.task_name = (char *)"Simple testing thread";
    config.func      = threadjoin_workerfunction_joinedthread1;
    config.userarg   = &user_arg;
    config.stack_size = THREAD_STACK_SIZE;

    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle1), OSAL_RETURN_OK);
    ASSERT_NE(thread_handle1, NULL_PTR);

    uid              = TEST_THREAD_ID3;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE3;
    config.task_name = (char *)"Simple testing thread";
    config.func      = threadjoin_workerfunction_joinedthread2;
    config.userarg   = &user_arg;
    config.stack_size = THREAD_STACK_SIZE;

    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle2), OSAL_RETURN_OK);
    ASSERT_NE(thread_handle2, NULL_PTR);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &result1), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle2, &result2), OSAL_RETURN_OK);

    if (OSAL_RETURN_OK == result1)
    {
        EXPECT_EQ(user_arg.return_value1, 0)  << "Test01_006_005 failed";
        EXPECT_EQ(result2, OSAL_RETURN_HANDLE) << "Test01_006_005 failed";
    }
    else
    {
        EXPECT_EQ(result1, OSAL_RETURN_HANDLE) << "Test01_006_005 failed";
        EXPECT_EQ(result2, OSAL_RETURN_OK)     << "Test01_006_005 failed";
        EXPECT_EQ(user_arg.return_value2, 0)  << "Test01_006_005 failed";
    }

    thread_handle = OSAL_THREAD_HANDLE_INVALID;
    thread_handle1 = OSAL_THREAD_HANDLE_INVALID;
    thread_handle2 = OSAL_THREAD_HANDLE_INVALID;
}
// [ThreadJoin_TC_01_006_005]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadJoin_TC_01_006_006 ThreadApiTest.ThreadJoin_TC_01_006_006
 * @subsection ThreadJoin_TC_01_006_006_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadJoin_TC_01_006_006_tci Test Case ID
 * ThreadJoin_S3_N_TC_01_006_006
 * @subsection ThreadJoin_TC_01_006_006_rr Related Requirement(s)
 * \#199129, \#199145
 * @subsection ThreadJoin_TC_01_006_006_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadJoin_TC_01_006_006_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to join a thread with normal condition:
 * - Test01_006_006: create two threads: thread_handle and thread_handle1. Create thread_handle2 from thread_handle1. Join thread_handle2 from thread_handle.
 * @subsection ThreadJoin_TC_01_006_006_tpts Test Procedure/Test Steps
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate function.
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate function.
 * -# Create first thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE3
 *          -# task_name is "Simple testing thread"
 *          -# func is threadjoin_workerfunction_thread_handle function with functionality:
 *              - Lock a mutex by using R_OSAL_ThsyncMutexTryLock.
 *              - Wait on a condition variable by using R_OSAL_ThsyncCondWaitForTimePeriod until this calling is success.
 *              - Unlock the mutex with R_OSAL_ThsyncMutexUnlock.
 *              - Join a thread by using R_OSAL_ThreadJoin.
 *          -# userarg includes thread handle and return value of first thread
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Address of "Simple testing thread"'s thread handle.
 * -# Create second thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE4
 *          -# task_name is "Simple testing thread"
 *          -# func is threadjoin_workerfunction_thread_handle1 function with functionality:
 *              - Lock a mutex by using R_OSAL_ThsyncMutexLockForTimePeriod until this calling is success.
 *              - Create thread using R_OSAL_ThreadCreate with the argument as below:
 *                  -# p_config: must be configured with following specified values:<br>
 *                     priority is OSAL_THREAD_PRIORITY_TYPE5.<br>
 *                     task_name is "Simple testing thread".<br>
 *                     func is "simple_workerfunction" function which call to R_OSAL_ThreadSleepForTimePeriod to suspend thread 500ms.<br>
 *                     userarg is NULL.<br>
 *                     stack_size is THREAD_STACK_SIZE.<br>
 *                  -# Thread id is TEST_THREAD_ID3.
 *                  -# Address of thread handle.
 *              - Unlock thread that's waiting on a condition variable with R_OSAL_ThsyncCondSignal.
 *              - Unlock the mutex with R_OSAL_ThsyncMutexUnlock.
 *          -# userarg includes thread handle and return value of the second thread.
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2.
 *      - Address of second thread handle.
 * -# Join second thread by using R_OSAL_ThreadJoin.
 * -# Join first thread by using R_OSAL_ThreadJoin.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * -# Destroy the condition variable by using R_OSAL_ThsyncCondDestroy.
 * @subsection ThreadJoin_TC_01_006_006_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with first thread. <br>
 * "thread_handle" is not NULL_PTR.<br>
 * In threadjoin_workerfunction_thread_handle function:<br>
 *  - R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncCondWaitForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 *  - Value of thread handle is not NULL.
 *  - R_OSAL_ThreadJoin return OSAL_RETURN_OK.
 *  - Value of "result" is equal to 0.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with second thread.<br>
 * "thread_handle1" is not NULL.<br>
 * In threadjoin_workerfunction_thread_handle1 function: <br>
 *  - R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_ThreadCreate return OSAL_RETURN_OK
 *      -# In simple_workerfunction function:
 *          - R_OSAL_ThreadSleepForTimePeriod return OSAL_RETURN_OK.
 *  - "thread_handle" is not NULL.
 *  - R_OSAL_ThsyncCondSignal return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with second thread.<br>
 * Value of "result" is equal to 0.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with first thread.<br>
 * Value of "result" is equal to 0.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK.
 * @subsection ThreadJoin_TC_01_006_006_testcase Test Case Example
 * @snippet test_thread.cpp ThreadJoin_TC_01_006_006
 * @snippet test_thread.cpp threadjoin_workerfunction_thread_handle
 * @snippet test_thread.cpp threadjoin_workerfunction_thread_handle1
 */
// [ThreadJoin_TC_01_006_006]
TEST_F(ThreadApiTest, ThreadJoin_TC_01_006_006)
{
    osal_thread_id_t            uid;
    st_osal_thread_config_t     config;
    int64_t                     result;
    osal_cond_id_t              cond_id;
    osal_mutex_id_t             mutex_id;

// Test01_006_006
// @brief: normal case: create two threads: thread_handle and thread_handle1.
// Create thread_handle2 from thread_handle1. Join thread_handle2 from thread_handle.
    cond_id = TEST_COND_ID1;
    mutex_id = TEST_MUTEX_ID1;
    threadjoin_worker_arg user_arg1(&thread_handle2);
    user_arg1.mq = sync_mq;
    user_arg1.send_msg = send_msg;

    ASSERT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &user_arg1.cond_handle), OSAL_RETURN_OK);
    ASSERT_NE(user_arg1.cond_handle, NULL_PTR);
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &user_arg1.mutex_handle), OSAL_RETURN_OK);
    ASSERT_NE(user_arg1.mutex_handle, NULL_PTR);
    uid              = TEST_THREAD_ID1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE3;
    config.task_name = (char *)"Simple testing thread";
    config.func      = threadjoin_workerfunction_thread_handle;
    config.userarg   = &user_arg1;
    config.stack_size = THREAD_STACK_SIZE;

    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK);
    ASSERT_NE(thread_handle, NULL_PTR);

    uid              = TEST_THREAD_ID2;
    user_arg1.uid   = uid++;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE4;
    config.task_name = (char *)"Simple testing thread";
    config.func      = threadjoin_workerfunction_thread_handle1;
    config.userarg   = &user_arg1;
    config.stack_size = THREAD_STACK_SIZE;

    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &receive_msg, mq_config.msg_size), OSAL_RETURN_OK);
    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle1), OSAL_RETURN_OK);
    ASSERT_NE(thread_handle1, NULL_PTR);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &result), OSAL_RETURN_OK);
    EXPECT_EQ(result, 0);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK);
    EXPECT_EQ(result, 0);
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(user_arg1.mutex_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncCondDestroy(user_arg1.cond_handle), OSAL_RETURN_OK);
    thread_handle1 = OSAL_THREAD_HANDLE_INVALID;
    thread_handle2 = OSAL_THREAD_HANDLE_INVALID;
}
// [ThreadJoin_TC_01_006_006]

/**
 * @page Thread_TEST Thread API Test
 * @section OsalApiStateTest_ThreadJoin_TC_01_006_007 OsalApiStateTest.ThreadJoin_TC_01_006_007
 * @subsection ThreadJoin_TC_01_006_007_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection ThreadJoin_TC_01_006_007_tci Test Case ID
 * ThreadJoin_S2_A_TC_01_006_007
 * @subsection ThreadJoin_TC_01_006_007_rr Related Requirement(s)
 * \#199145
 * @subsection ThreadJoin_TC_01_006_007_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection ThreadJoin_TC_01_006_007_tcs Test Case Summary
 * This test shall verify the state transition of Thread APIs with abnormal condition:
 * - Test01_006_007: state transition error with R_OSAL_ThreadJoin.
 * @subsection ThreadJoin_TC_01_006_007_tpts Test Procedure/Test Steps
 * -# Join thread by using R_OSAL_ThreadJoin.
 * @subsection ThreadJoin_TC_01_006_007_ereb Expected Result/Behavior
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_STATE.
 * @subsection ThreadJoin_TC_01_006_007_testcase Test Case Example
 * @snippet test_thread.cpp ThreadJoin_TC_01_006_007
 */
// [ThreadJoin_TC_01_006_007]
TEST_F(OsalApiStateTest, ThreadJoin_TC_01_006_007)
{
    int64_t                     result;
    osal_thread_handle_t        thread_handle = OSAL_THREAD_HANDLE_INVALID;

// Test01_006_007
// @brief: abnormal case: state transition error with R_OSAL_ThreadJoin
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_STATE) << "Test01_006_007 failed";
}
// [ThreadJoin_TC_01_006_007]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadInitializeThreadConfigSt_TC_01_007_001 ThreadApiTest.ThreadInitializeThreadConfigSt_TC_01_007_001
 * @subsection ThreadInitializeThreadConfigSt_TC_01_007_001_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadInitializeThreadConfigSt_TC_01_007_001_tci Test Case ID
 * ThreadInitializeThreadConfigSt_S1_N_TC_01_007_001
 * @subsection ThreadInitializeThreadConfigSt_TC_01_007_001_rr Related Requirement(s)
 * \#276547
 * @subsection ThreadInitializeThreadConfigSt_TC_01_007_001_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadInitializeThreadConfigSt_TC_01_007_001_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API R_OSAL_ThreadInitializeThreadConfigSt function with normal condition:
 * - Test01_007_001: all input/output param is valid
 * @subsection ThreadInitializeThreadConfigSt_TC_01_007_001_tpts Test Procedure/Test Steps
 * -# Initialize and configure a thread by using R_OSAL_ThreadInitializeThreadConfigSt.
 * @subsection ThreadInitializeThreadConfigSt_TC_01_007_001_ereb Expected Result/Behavior
 * -# R_OSAL_ThreadInitializeThreadConfigSt return OSAL_RETURN_OK.<br>
 * The content of "config.task_name" is not NULL.<br>
 * Value of "config.stack_size" is equal to 0.
 * @subsection ThreadInitializeThreadConfigSt_TC_01_007_001_testcase Test Case Example
 * @snippet test_thread.cpp ThreadInitializeThreadConfigSt_TC_01_007_001
 */
// [ThreadInitializeThreadConfigSt_TC_01_007_001]
TEST_F(ThreadApiTest, ThreadInitializeThreadConfigSt_TC_01_007_001)
{
    st_osal_thread_config_t     config;

// Test01_007_001
// @brief: normal case: all input/output param is valid
    ASSERT_EQ(R_OSAL_ThreadInitializeThreadConfigSt(&config), OSAL_RETURN_OK) << "Test01_007_001 failed";
    ASSERT_STRNE(config.task_name, NULL) << "Test01_007_001 failed";
    ASSERT_EQ(config.stack_size, 0) << "Test01_007_001 failed";
}
// [ThreadInitializeThreadConfigSt_TC_01_007_001]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadInitializeThreadConfigSt_TC_01_007_002 ThreadApiTest.ThreadInitializeThreadConfigSt_TC_01_007_002
 * @subsection ThreadInitializeThreadConfigSt_TC_01_007_002_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadInitializeThreadConfigSt_TC_01_007_002_tci Test Case ID
 * ThreadInitializeThreadConfigSt_S0_A_TC_01_007_002
 * @subsection ThreadInitializeThreadConfigSt_TC_01_007_002_rr Related Requirement(s)
 * \#276547
 * @subsection ThreadInitializeThreadConfigSt_TC_01_007_002_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadInitializeThreadConfigSt_TC_01_007_002_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API R_OSAL_ThreadInitializeThreadConfigSt function with abnormal condition:
 * - Test01_007_002: config is NULL
 * @subsection ThreadInitializeThreadConfigSt_TC_01_007_002_tpts Test Procedure/Test Steps
 * -# Initialize and configure a thread by using R_OSAL_ThreadInitializeThreadConfigSt with the argument as below:
 *  - config is NULL
 * @subsection ThreadInitializeThreadConfigSt_TC_01_007_002_ereb Expected Result/Behavior
 * -# R_OSAL_ThreadInitializeThreadConfigSt return OSAL_RETURN_PAR.
 * @subsection ThreadInitializeThreadConfigSt_TC_01_007_002_testcase Test Case Example
 * @snippet test_thread.cpp ThreadInitializeThreadConfigSt_TC_01_007_002
 */
// [ThreadInitializeThreadConfigSt_TC_01_007_002]
TEST_F(ThreadApiTest, ThreadInitializeThreadConfigSt_TC_01_007_002)
{
    st_osal_thread_config_t* p_config = NULL;

// Test01_007_002
// @brief: abnormal case: config is NULL
    ASSERT_EQ(R_OSAL_ThreadInitializeThreadConfigSt(p_config), OSAL_RETURN_PAR) << "Test01_007_002 failed";
}
// [ThreadInitializeThreadConfigSt_TC_01_007_002]

/**
 * @page Thread_TEST Thread API Test
 * @section ThreadApiTest_ThreadInitializeThreadConfigSt_TC_01_007_003 ThreadApiTest.ThreadInitializeThreadConfigSt_TC_01_007_003
 * @subsection ThreadInitializeThreadConfigSt_TC_01_007_003_tsi Test Suite ID
 * ThreadApiTest
 * @subsection ThreadInitializeThreadConfigSt_TC_01_007_003_tci Test Case ID
 * ThreadInitializeThreadConfigSt_S2_A_TC_01_007_003
 * @subsection ThreadInitializeThreadConfigSt_TC_01_007_003_rr Related Requirement(s)
 * \#276547
 * @subsection ThreadInitializeThreadConfigSt_TC_01_007_003_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThreadInitializeThreadConfigSt_TC_01_007_003_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API R_OSAL_ThreadInitializeThreadConfigSt function with abnormal condition:
 * - Test01_007_003: create new thread by using config got from R_OSAL_ThreadInitializeThreadConfigSt
 * @subsection ThreadInitializeThreadConfigSt_TC_01_007_003_tpts Test Procedure/Test Steps
 * -# Initialize the thread_handle with THREAD_HANDLE_INITIALIZED_VALUE by using memset.<br>
 * THREAD_HANDLE_INITIALIZED_VALUE must be different from OSAL_THREAD_HANDLE_INVALID.
 * -# Initialize the thread configuration by using R_OSAL_ThreadInitializeThreadConfigSt
 * -# Create new thread by using R_OSAL_ThreadCreate with the configuration above
 * @subsection ThreadInitializeThreadConfigSt_TC_01_007_003_ereb Expected Result/Behavior
 * -# memset a value for thread_handle successfully.
 * -# R_OSAL_ThreadInitializeThreadConfigSt return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_PAR.<br>
 * Value of "thread_handle" is OSAL_THREAD_HANDLE_INVALID.
 * @subsection ThreadInitializeThreadConfigSt_TC_01_007_003_testcase Test Case Example
 * @snippet test_thread.cpp ThreadInitializeThreadConfigSt_TC_01_007_003
 */
// [ThreadInitializeThreadConfigSt_TC_01_007_003]
TEST_F(ThreadApiTest, ThreadInitializeThreadConfigSt_TC_01_007_003)
{
    osal_thread_id_t uid;
    st_osal_thread_config_t config;

// Test01_007_003
// @brief: abnormal case: create new thread by using config got from R_OSAL_ThreadInitializeThreadConfigSt
    uid              = TEST_THREAD_ID1;
    memset(&thread_handle, THREAD_HANDLE_INITIALIZED_VALUE, sizeof(thread_handle));
    EXPECT_EQ(R_OSAL_ThreadInitializeThreadConfigSt(&config), OSAL_RETURN_OK) << "Test01_007_003 failed";
    EXPECT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_PAR) << "Test01_007_003 failed";
    EXPECT_EQ(thread_handle, OSAL_THREAD_HANDLE_INVALID) << "Test01_007_003 failed";
}
// [ThreadInitializeThreadConfigSt_TC_01_007_003]
