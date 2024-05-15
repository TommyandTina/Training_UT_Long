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
* File Name :    test_cond_var.cpp
* Version :      3.2.2
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL ThreadSync(Cond) Manager API Functional Test (From the perspective of OSAL API Specification)
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           2.0.0     29.10.2021    Create New
 *           2.0.1     15.11.2021    Update Test Name
 *                                   CondVar -> CondVar_TC_12_001_002
 *           2.0.2     18.11.2021    Fix Typo
 *           3.0.0     08.03.2022    Apply new/updated device definition related to interrupt context test
 *                                   Apply updated allocate_setup_data_memory, setup_hw_register and deallocate_data_memory
 *           3.0.1     11.03.2022    Remove disabled test on SIL
 *           3.0.2     04.04.2022    Update synchronization method for the below test:
 *                                      - CondVar_TC_12_001_002
 *                                      - ThsyncCondDestroy_TC_02_008_004
 *           3.0.3     19.04.2022    Update synchronization method for the below test:
 *                                      - ThsyncCondBroadcast_TC_02_011_006
 *                                      - ThsyncCondSignal_TC_02_012_005
 *           3.1.0     04.01.2023    Add handle value initialize for below test:
 *                                      - ThsyncCondCreate_TC_02_007_004
 *           3.2.0     01.02.2023    Add new boundary test cases as below:
 *                                      - ThsyncCondWaitUntilTimeStamp_TC_02_010_016
 *                                      - ThsyncCondWaitUntilTimeStamp_TC_02_010_017
 *                                      - ThsyncCondWaitUntilTimeStamp_TC_02_010_018
 *                                      - ThsyncCondWaitUntilTimeStamp_TC_02_010_019
 *           3.2.1     10.04.2023    Update expected value for median time for SIL Windows performance test
 *           3.2.2     19.05.2023    Added MQReset to sync mechanisum
***********************************************************************************************************************/
#include "test_common.hpp"
#if !defined(OSAL_TEST_FRAMEWORK)
using namespace std;
#else
using namespace OsalFWHelper;
#endif

osal_mutex_handle_t mtx;
osal_cond_handle_t  cv;
static osal_mq_handle_t mq_handle;
static const int32_t send_msg = 0x0001;

enum order
{
    BEFORE,
    DURING,
    AFTER
};

/* armclang will be warnig, when specified OsalApiStateTest to TEST_F */
/* W.A Derive a class and replace it by define */
class OsalApiStateTest_COND : public OsalApiStateTest
{
    /* There is no need for member variables and member functions.
       We added them just for the sake of adding a test class. */
};
#define OsalApiStateTest OsalApiStateTest_COND

/* armclang will be warnig, when specified OSALINTTESTS to TEST_F */
/* W.A Derive a class and replace it by define */
class OSALINTTESTS_COND : public OSALINTTESTS
{
    /* There is no need for member variables and member functions.
       We added them just for the sake of adding a test class. */
};
#define OSALINTTESTS OSALINTTESTS_COND


// [order_chk]
bool order_chk(order ordr)
{
    static volatile bool before = false, during = false, after = false;
    bool                 ret;

    switch (ordr)
    {
        case BEFORE:
            before = !before;
            return (before && !during && !after);
            break;

        case DURING:
            during = !during;
            return (before && during && !after);
            break;

        case AFTER:
            after = !after;
            ret = (before && during && after);
            before = false;
            during = false;
            after = false;
            return ret;
            break;

        default:
            return false;
            break;
    }
    return false;
}
// [order_chk]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @test
 * @test @link ThreadSyncCond_CondVar_TC_12_001_002 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondCreate_TC_02_007_001 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondCreate_TC_02_007_002 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondCreate_TC_02_007_003 @endlink
 * @test @link OsalApiStateTest_ThsyncCondCreate_TC_02_007_004 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondDestroy_TC_02_008_001 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondDestroy_TC_02_008_002 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondDestroy_TC_02_008_003 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondDestroy_TC_02_008_004 @endlink
 * @test @link OsalApiStateTest_ThsyncCondDestroy_TC_02_008_005 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondDestroy_TC_02_008_006 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondDestroy_TC_02_008_007 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondWaitForTimePeriod_TC_02_009_001 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondWaitForTimePeriod_TC_02_009_002 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondWaitForTimePeriod_TC_02_009_003 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondWaitForTimePeriod_TC_02_009_004 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondWaitForTimePeriod_TC_02_009_005 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondWaitForTimePeriod_TC_02_009_006 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondWaitForTimePeriod_TC_02_009_007 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondWaitForTimePeriod_TC_02_009_009 @endlink
 * @test @link OsalApiStateTest_ThsyncCondWaitForTimePeriod_TC_02_009_010 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondWaitForTimePeriod_TC_02_009_011 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondWaitForTimePeriod_TC_02_009_012 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_001 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_002 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_003 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_004 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_005 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_006 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_007 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_008 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_009 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_010 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_011 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_013 @endlink
 * @test @link OsalApiStateTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_014 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_015 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_016 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_017 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_018 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_019 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondBroadcast_TC_02_011_001 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondBroadcast_TC_02_011_002 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondBroadcast_TC_02_011_003 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondBroadcast_TC_02_011_004 @endlink
 * @test @link OsalApiStateTest_ThsyncCondBroadcast_TC_02_011_005 @endlink
 * @test @link OSALINTTESTS_ThsyncCondBroadcast_TC_02_011_006 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondSignal_TC_02_012_001 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondSignal_TC_02_012_002 @endlink
 * @test @link ThreadSyncCondApiTest_ThsyncCondSignal_TC_02_012_003 @endlink
 * @test @link OsalApiStateTest_ThsyncCondSignal_TC_02_012_004 @endlink
 * @test @link OSALINTTESTS_ThsyncCondSignal_TC_02_012_005 @endlink
 * @section ThreadSyncCond ThreadSync function
 * @subsection ThreadSyncCond_SetUp ThreadSyncCond Setup
 * Common to Test Suit of ThreadSyncCond, prepare process to be executed before executing the test. <br>
 * For example:
 * @snippet test_cond_var.cpp ThreadSyncCond_SetUp
 * @snippet test_common.hpp OSALTESTS_SetUp
 * @subsection ThreadSyncCond_TearDown ThreadSyncCond TearDown
 * Common to Test Suit of ThreadSyncCond, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_cond_var.cpp ThreadSyncCond_TearDown
 * @snippet test_common.hpp OSALTESTS_TearDown
 * @section ThreadSyncCondApiTest ThreadSyncCondApiTest function
 * @subsection ThreadSyncCondApiTest_SetUp ThreadSyncCondApiTest Setup
 * Common to Test Suit of ThreadSyncCondApiTest, prepare process to be executed before executing the test. <br>
 * For example:
 * @snippet test_common.hpp OSALTESTS_SetUp
 * @subsection ThreadSyncCondApiTest_TearDown ThreadSyncCondApiTest TearDown
 * Common to Test Suit of ThreadSyncCondApiTest, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_common.hpp OSALTESTS_TearDown
 * @section OSALINTTESTS_ThreadSyncCondApiTest OSALINTTESTS of ThreadSyncCondApiTest function
 * @subsection OSALINTTESTS_ThreadSyncCondApiTest_SetUp OSALINTTESTS of ThreadSyncCondApiTest Setup
 * Common to Test Suit of OSALINTTESTS ThreadSyncCondApiTest, prepare process to be executed before executing the test. <br>
 * This test suite generates interrupts. These process is preparation<br>
 * For example:
 * @snippet test_common.hpp OSALINTTESTS_SetUp
 * @subsection OSALINTTESTS_ThreadSyncCondApiTest_TearDown OSALINTTESTS of ThreadSyncCondApiTest TearDown
 * Common to Test Suit of OSALINTTESTS ThreadSyncCondApiTest, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_common.hpp OSALINTTESTS_TearDown
 * @section OsalApiStateTest_ThreadSyncCondApiTest OsalApiStateTest of ThreadSyncCondApiTest function
 * @subsection OsalApiStateTest_ThreadSyncCondApiTest_SetUp OsalApiStateTest of ThreadSyncCondApiTest Setup
 * Common to Test Suit of OsalApiStateTest ThreadSyncCondApiTest, prepare process to be executed before executing the test. <br>
 * For example:
 * @snippet test_common.hpp OsalApiStateTest_SetUp
 * @subsection OsalApiStateTest_ThreadSyncCondApiTest_TearDown OsalApiStateTest of ThreadSyncCondApiTest TearDown
 * Common to Test Suit of OsalApiStateTest ThreadSyncCondApiTest, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_common.hpp OsalApiStateTest_TearDown
 */
class ThreadSyncCond : public OSALTESTS {
protected:
    osal_thread_handle_t    th_high, th_low;
    st_osal_thread_config_t th_config_l, th_config_h;

// [ThreadSyncCond_SetUp]
    void SetUp() override
    {
        st_osal_mq_config mq_config = {1, 4};
        OSALTESTS::SetUp();

        ASSERT_EQ(R_OSAL_ThsyncMutexCreate(TEST_MUTEX_ID1, &mtx), OSAL_RETURN_OK);
        ASSERT_EQ(R_OSAL_ThsyncCondCreate(TEST_COND_ID1, &cv), OSAL_RETURN_OK);
        ASSERT_EQ(R_OSAL_MqCreate(&mq_config, TEST_MQ_COND_ID, &mq_handle), OSAL_RETURN_OK);
    }
// [ThreadSyncCond_SetUp]

// [ThreadSyncCond_TearDown]
    void TearDown() override
    {
        ASSERT_EQ(R_OSAL_ThsyncMutexDestroy(mtx), OSAL_RETURN_OK);
        ASSERT_EQ(R_OSAL_ThsyncCondDestroy(cv), OSAL_RETURN_OK);
        ASSERT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK);
        ASSERT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK);

        OSALTESTS::TearDown();
    }
// [ThreadSyncCond_TearDown]
};

// [workerFuncDuring]
int64_t workerFuncDuring(void* userArg)
{
    st_osal_mq_config_t local_mq_config;

    EXPECT_EQ(R_OSAL_MqGetConfig(mq_handle, &local_mq_config), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(mtx, 100), OSAL_RETURN_OK);

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(mq_handle, SYNC_MQ_TIME, &send_msg, local_mq_config.msg_size), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncCondWaitForTimePeriod(cv, mtx, 1000), OSAL_RETURN_OK);

    EXPECT_TRUE(order_chk(DURING));

    EXPECT_EQ(R_OSAL_ThsyncCondSignal(cv), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(mtx), OSAL_RETURN_OK);
    return 0;
}
// [workerFuncDuring]

// [workerFuncBeforeAfter]
int64_t workerFuncBeforeAfter(void* userArg)
{
    EXPECT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(mtx, 1000), OSAL_RETURN_OK);

    EXPECT_TRUE(order_chk(BEFORE));

    EXPECT_EQ(R_OSAL_ThsyncCondSignal(cv), OSAL_RETURN_OK);
    //EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(mtx), OSAL_RETURN_OK);

    EXPECT_EQ(R_OSAL_ThsyncCondWaitForTimePeriod(cv, mtx, 1000), OSAL_RETURN_OK);

    EXPECT_TRUE(order_chk(AFTER));

    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(mtx), OSAL_RETURN_OK);
    return 0;
}
// [workerFuncBeforeAfter]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCond_CondVar_TC_12_001_002 ThreadSyncCond.CondVar_TC_12_001_002
 * @subsection CondVar_tsi Test Suite ID
 * ThreadSyncCond
 * @subsection CondVar_tci Test Case ID
 * CondVar_S3_N_TC_12_001_002
 * @subsection CondVar_rr Related Requirement(s)
 * \#199158, \#199159, \#199160, \#199163
 * @subsection CondVar_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection CondVar_tcs Test Case Summary
 * This test shall force one thread with higher prio to wait for another thread with lower prio using condition variables.
 * @subsection CondVar_tpts Test Procedure/Test Steps
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate function.
 * -# Create a new condition variable using R_OSAL_ThsyncCondCreate function.
 * -# Create "cond_var_test_Highest" thread using R_OSAL_ThreadCreate function with the arguments as below:
 *      - p_config: must be configured with following specified values
 *           -# priority is OSAL_THREAD_PRIORITY_HIGHEST
 *           -# task_name is "cond_var_test_Highest"
 *           -# func is workerFuncDuring function with functionality:
 *               - Get the config of synchronization queue by using R_OSAL_MqGetConfig
 *               - Lock a mutex by using R_OSAL_ThsyncMutexLockForTimePeriod.
 *               - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *               - Wait on a condition variable by using R_OSAL_ThsyncCondWaitForTimePeriod.
 *               - Call to order_chk(DURING)
 *               - Unlock thread that's waiting on a condition variable with R_OSAL_ThsyncCondSignal.
 *               - Unlock the mutex with R_OSAL_ThsyncMutexUnlock.
 *           -# userarg is Null pointer
 *           -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1
 *      - Address of thread handle
 * -# Get the config of synchronization queue by using R_OSAL_MqGetConfig
 * -# Receive message from synchronization queue by using R_OSAL_MqReceiveForTimePeriod
 * -# Create "cond_var_test_Lowest" thread using R_OSAL_ThreadCreate function with the arguments as below:
 *      - p_config: must be configured with following specified values
 *           -# priority is OSAL_THREAD_PRIORITY_LOWEST
 *           -# task_name is "cond_var_test_Lowest"
 *           -# func is workerFuncBeforeAfter function with functionality:
 *               - Lock a mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 *               - Call to order_chk(BEFORE)
 *               - Unlock thread that's waiting on a condition variable with R_OSAL_ThsyncCondSignal.<br>
 *               - Unlock the mutex with R_OSAL_ThsyncMutexUnlock.
 *               - Wait on a condition variable with R_OSAL_ThsyncCondWaitForTimePeriod.
 *               - Call to order_chk(AFTER)
 *               - Unlock the mutex again with R_OSAL_ThsyncMutexUnlock.
 *           -# userarg is Null pointer
 *           -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2
 *      - Address of "p_handle"
 * -# Join "cond_var_test_Highest" thread with R_OSAL_ThreadJoin.<br>
 * -# Join "cond_var_test_Lowest" thread with R_OSAL_ThreadJoin.<br>
 * -# Destroy the mutex using R_OSAL_ThsyncMutexDestroy function.
 * -# Destroy the condition variable using R_OSAL_ThsyncCondDestroy.
 * @subsection CondVar_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK, "cond_var_test_Highest" thread is created successfully.<br>
 * In workerFuncDuring function.<br>
 *  - R_OSAL_MqGetConfig return OSAL_RETURN_OK
 *  - The expected return code of R_OSAL_ThsyncMutexLockForTimePeriod is OSAL_RETURN_OK.
 *  - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 *  - The expected return code of R_OSAL_ThsyncCondWaitForTimePeriod is OSAL_RETURN_OK.
 *  - Return value of order_chk is true.
 *  - R_OSAL_ThsyncCondSignal return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_MqGetConfig return OSAL_RETURN_OK
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK "cond_var_test_Lowest" thread is created successfully.<br>
 * In workerFuncBeforeAfter function.<br>
 *  - The expected return code of R_OSAL_ThsyncMutexLockForTimePeriod is OSAL_RETURN_OK.
 *  - Return value of order_chk is true.
 *  - R_OSAL_ThsyncCondSignal return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 *  - The expected return code of R_OSAL_ThsyncCondWaitForTimePeriod is OSAL_RETURN_OK.
 *  - Return value of order_chk is true.
 *  - R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK for "cond_var_test_Highest" thread.<br>
 * Value of "ret_val" is equal to return value of "cond_var_test_Highest" thread.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK for "cond_var_test_Lowest" thread.<br>
 * Value of "ret_val" is equal to return value of "cond_var_test_Lowest" thread.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK.
 * @subsection CondVar_testcase Test Case Example
 * @snippet test_cond_var.cpp CondVar_TC_12_001_002
 * @snippet test_cond_var.cpp order_chk
 * @snippet test_cond_var.cpp workerFuncDuring
 * @snippet test_cond_var.cpp workerFuncBeforeAfter
 */
//the idea behind this test is to force one thread with higher prio to wait for another thread with lower prio using condition variables
// [CondVar_TC_12_001_002]
TEST_F(ThreadSyncCond, CondVar_TC_12_001_002)
{
    int64_t         ret_val;
    e_osal_return_t osal_ret;
    st_osal_mq_config_t local_mq_config;
    int receive_msg = 0;

    th_config_l.userarg = NULL_PTR;
    th_config_l.stack_size = THREAD_STACK_SIZE;
    th_config_l.func      = workerFuncDuring;
    th_config_l.priority  = OSAL_THREAD_PRIORITY_HIGHEST;;
    th_config_l.task_name = (char*)"cond_var_test_Highest";
    ASSERT_EQ(R_OSAL_ThreadCreate(&th_config_l, TEST_THREAD_ID1, &th_high), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqGetConfig(mq_handle, &local_mq_config), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(mq_handle, SYNC_MQ_TIME, &receive_msg, local_mq_config.msg_size), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

    th_config_h.userarg = NULL_PTR;
    th_config_h.stack_size = THREAD_STACK_SIZE;
    th_config_h.func      = workerFuncBeforeAfter;
    th_config_h.priority  = OSAL_THREAD_PRIORITY_LOWEST;
    th_config_h.task_name = (char*)"cond_var_test_Lowest";
    ASSERT_EQ(R_OSAL_ThreadCreate(&th_config_h, TEST_THREAD_ID2, &th_low), OSAL_RETURN_OK);

    EXPECT_EQ(R_OSAL_ThreadJoin(th_high, &ret_val), OSAL_RETURN_OK);
    EXPECT_EQ(ret_val, 0);

    EXPECT_EQ(R_OSAL_ThreadJoin(th_low, &ret_val), OSAL_RETURN_OK);
    EXPECT_EQ(ret_val, 0);
}
// [CondVar_TC_12_001_002]

class ThreadSyncCondApiTest : public OSALTESTS {
protected:
    /* RVC implementation base on Linux OSAL IT*/
    osal_mutex_handle_t mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
    osal_mutex_handle_t mtx_handle1 = OSAL_MUTEX_HANDLE_INVALID;
    osal_cond_handle_t cond_handle = OSAL_COND_HANDLE_INVALID;
    osal_cond_handle_t cond_handle1 = OSAL_COND_HANDLE_INVALID;
    osal_thread_handle_t thread_handle = OSAL_THREAD_HANDLE_INVALID;
    osal_thread_handle_t thread_handle1 = OSAL_THREAD_HANDLE_INVALID;
    osal_thread_handle_t thread_handle2 = OSAL_THREAD_HANDLE_INVALID;
    osal_thread_handle_t thread_handle3 = OSAL_THREAD_HANDLE_INVALID;

// [ThreadSyncCondApiTest_SetUp]
    void SetUp() override
    {
        st_osal_mq_config mq_config = {1, 4};

        OSALTESTS::SetUp();
        ASSERT_EQ(R_OSAL_MqCreate(&mq_config, TEST_MQ_COND_ID, &mq_handle), OSAL_RETURN_OK);
    }
// [ThreadSyncCondApiTest_SetUp]

// [ThreadSyncCondApiTest_TearDown]
    void TearDown() override
    {
        ASSERT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK);
        ASSERT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK);
        OSALTESTS::TearDown();
    }
// [ThreadSyncCondApiTest_TearDown]
};

/* RVC implementation base on Linux OSAL IT*/
struct condperiod_worker_arg
{
    condperiod_worker_arg(osal_cond_handle_t * c_handle, osal_mutex_handle_t * mtx_handle) : cond_handle(c_handle), mutex_handle(mtx_handle)
    {
    }
    osal_mutex_handle_t *mutex_handle;
    osal_cond_handle_t *cond_handle;
};

struct condstamp_worker_arg
{
    condstamp_worker_arg(osal_cond_handle_t * c_handle, osal_mutex_handle_t * mtx_handle, st_osal_time_t *time_stamp)
                        : cond_handle(c_handle), mutex_handle(mtx_handle), abs_time(time_stamp)
    {
    }
    osal_mutex_handle_t *mutex_handle;
    osal_cond_handle_t *cond_handle;
    st_osal_time_t *abs_time;
};

// [conddestroy_first_workerfunction]
int64_t conddestroy_first_workerfunction(void * arg)
{
    condperiod_worker_arg *testStruct = (condperiod_worker_arg *)arg;
    osal_milli_sec_t mutex_timeout = 1000;
    osal_milli_sec_t cond_timeout = 1000;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_mq_config_t local_mq_config;
    int64_t ret;

    EXPECT_EQ(R_OSAL_MqGetConfig(mq_handle, &local_mq_config), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(*(testStruct->mutex_handle), mutex_timeout), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(mq_handle, SYNC_MQ_TIME, &send_msg, local_mq_config.msg_size), OSAL_RETURN_OK);
    osal_ret = R_OSAL_ThsyncCondWaitForTimePeriod(*(testStruct->cond_handle), *(testStruct->mutex_handle), cond_timeout);
    ret = (osal_ret == OSAL_RETURN_TIME)? 0 : -1;
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(*(testStruct->mutex_handle)), OSAL_RETURN_OK);
    return ret;
}
// [conddestroy_first_workerfunction]

// [conddestroy_second_workerfunction]
int64_t conddestroy_second_workerfunction(void * arg)
{
    condperiod_worker_arg *testStruct = (condperiod_worker_arg *)arg;
    osal_milli_sec_t mutex_timeout = 1000;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int64_t ret;

    EXPECT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(*(testStruct->mutex_handle), mutex_timeout), OSAL_RETURN_OK);
    osal_ret = R_OSAL_ThsyncCondDestroy(*(testStruct->cond_handle));
    ret = (osal_ret == OSAL_RETURN_BUSY)? 0 : -1;
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(*(testStruct->mutex_handle)), OSAL_RETURN_OK);
    return ret;
}
// [conddestroy_second_workerfunction]

// [condperiodwait_workerfunction]
int64_t condperiodwait_workerfunction(void * arg)
{
    condperiod_worker_arg *testStruct = (condperiod_worker_arg *)arg;
    osal_milli_sec_t cond_timeout = 2000;
    osal_milli_sec_t mutex_timeout = 1000;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int64_t ret;

    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(*(testStruct->mutex_handle), mutex_timeout), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(mq_handle, 0, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    osal_ret = R_OSAL_ThsyncCondWaitForTimePeriod(*(testStruct->cond_handle), *(testStruct->mutex_handle), cond_timeout);
    ret = (osal_ret == OSAL_RETURN_OK)? 0 : -1;
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(*(testStruct->mutex_handle)), OSAL_RETURN_OK);
    return ret;
}
// [condperiodwait_workerfunction]

// [condstampwait_passedtime_workerfunction]
int64_t condstampwait_passedtime_workerfunction(void * arg)
{
    condstamp_worker_arg *testStruct = (condstamp_worker_arg *)arg;
    osal_milli_sec_t mutex_timeout = 1000;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int64_t ret;

    EXPECT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(*(testStruct->mutex_handle), mutex_timeout), OSAL_RETURN_OK);
    osal_ret = R_OSAL_ThsyncCondWaitUntilTimeStamp(*(testStruct->cond_handle), *(testStruct->mutex_handle), (testStruct->abs_time));
    ret = (osal_ret == OSAL_RETURN_TIME)? 0 : -1;
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(*(testStruct->mutex_handle)), OSAL_RETURN_OK);
    return ret;
}
// [condstampwait_passedtime_workerfunction]

// [condperiodwait_wakeup_workerfunction]
int64_t condperiodwait_wakeup_workerfunction(void * arg)
{
    condperiod_worker_arg *testStruct = (condperiod_worker_arg *)arg;
    osal_milli_sec_t mutex_timeout = 1000;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int64_t ret;

    EXPECT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(*(testStruct->mutex_handle), mutex_timeout), OSAL_RETURN_OK);
    osal_ret = R_OSAL_ThsyncCondSignal(*(testStruct->cond_handle));
    ret = (osal_ret == OSAL_RETURN_OK)? 0 : -1;
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(*(testStruct->mutex_handle)), OSAL_RETURN_OK);
    return ret;
}
// [condperiodwait_wakeup_workerfunction]

// [condperiodwait_timeout_workerfunction]
int64_t condperiodwait_timeout_workerfunction(void * arg)
{
    condperiod_worker_arg *testStruct = (condperiod_worker_arg *)arg;
    osal_milli_sec_t mutex_timeout = 1000;
    osal_milli_sec_t cond_timeout = 1000;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int64_t ret;

    EXPECT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(*(testStruct->mutex_handle), mutex_timeout), OSAL_RETURN_OK);
    while(OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_ThsyncCondWaitForTimePeriod(*(testStruct->cond_handle), *(testStruct->mutex_handle), cond_timeout);
        if(OSAL_RETURN_OK == osal_ret)
        {
            /* spurious wakeup. Continue to wait. */
            printf("Spurious wakeup. Continue to wait.\n");
        }
    }
    ret = (osal_ret == OSAL_RETURN_TIME)? 0 : -1;
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(*(testStruct->mutex_handle)), OSAL_RETURN_OK);

    return ret;
}
// [condperiodwait_timeout_workerfunction]

// [condstampwait_workerfunction]
int64_t condstampwait_workerfunction(void * arg)
{
    condstamp_worker_arg *testStruct = (condstamp_worker_arg *)arg;
    osal_milli_sec_t mutex_timeout = 1000;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int64_t ret;

    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(*(testStruct->mutex_handle), mutex_timeout), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(mq_handle, 0, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    osal_ret = R_OSAL_ThsyncCondWaitUntilTimeStamp(*(testStruct->cond_handle), *(testStruct->mutex_handle), (testStruct->abs_time));
    ret = (osal_ret == OSAL_RETURN_OK)? 0 : -1;
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(*(testStruct->mutex_handle)), OSAL_RETURN_OK);

    return ret;
}
// [condstampwait_workerfunction]

// [condstampwait_wakeup_workerfunction]
int64_t condstampwait_wakeup_workerfunction(void * arg)
{
    condstamp_worker_arg *testStruct = (condstamp_worker_arg *)arg;
    osal_milli_sec_t mutex_timeout = 1000;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int64_t ret;

    EXPECT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(*(testStruct->mutex_handle), mutex_timeout), OSAL_RETURN_OK);
    osal_ret = R_OSAL_ThsyncCondSignal(*(testStruct->cond_handle));
    ret = (osal_ret == OSAL_RETURN_OK)? 0 : -1;
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(*(testStruct->mutex_handle)), OSAL_RETURN_OK);

    return ret;
}
// [condstampwait_wakeup_workerfunction]

// [condstampwait_timeout_workerfunction]
int64_t condstampwait_timeout_workerfunction(void * arg)
{
    condstamp_worker_arg *testStruct = (condstamp_worker_arg *)arg;
    osal_milli_sec_t mutex_timeout = 1000;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int64_t ret;

    EXPECT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(*(testStruct->mutex_handle), mutex_timeout), OSAL_RETURN_OK);
    while(OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_ThsyncCondWaitUntilTimeStamp(*(testStruct->cond_handle), *(testStruct->mutex_handle), (testStruct->abs_time));
        if(OSAL_RETURN_OK == osal_ret)
        {
            /* spurious wakeup. Continue to wait. */
            printf("Spurious wakeup. Continue to wait.\n");
        }
    }
    ret = (osal_ret == OSAL_RETURN_TIME)? 0 : -1;
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(*(testStruct->mutex_handle)), OSAL_RETURN_OK);

    return ret;
}
// [condstampwait_timeout_workerfunction]

// [cond_wakeup_broadcast_workerfunction]
int64_t cond_wakeup_broadcast_workerfunction(void * arg)
{
    condperiod_worker_arg *testStruct = (condperiod_worker_arg *)arg;
    osal_milli_sec_t mutex_timeout = 1000;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int64_t ret;

    EXPECT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(*(testStruct->mutex_handle), mutex_timeout), OSAL_RETURN_OK);
    osal_ret = R_OSAL_ThsyncCondBroadcast(*(testStruct->cond_handle));
    ret = (osal_ret == OSAL_RETURN_OK)? 0 : -1;
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(*(testStruct->mutex_handle)), OSAL_RETURN_OK);

    return ret;
}
// [cond_wakeup_broadcast_workerfunction]

// [osal_cond_isr_test_TC_02_011_006]
void osal_cond_isr_test_TC_02_011_006(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    uintptr_t offset = TEST_REG_CLEAR_REGISTER;
    uint32_t write_data = TEST_CLEAR_REGISTER_VALUE;
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, offset, write_data), OSAL_RETURN_OK);

    // Test02_011_006
    // @brief R_OSAL_ThsyncCondBroadcast to two remaining waiting threads
    EXPECT_EQ(R_OSAL_ThsyncMutexTryLock(local_arg->mtx_handle), OSAL_RETURN_OK) << "Test02_011_006 failed";
    EXPECT_EQ(R_OSAL_ThsyncCondBroadcast(local_arg->cond_handle), OSAL_RETURN_OK) << "Test02_011_006 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(local_arg->mtx_handle), OSAL_RETURN_OK) << "Test02_011_006 failed";
}
// [osal_cond_isr_test_TC_02_011_006]

// [osal_cond_isr_test_TC_02_012_005]
void osal_cond_isr_test_TC_02_012_005(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    uintptr_t offset = TEST_REG_CLEAR_REGISTER;
    uint32_t write_data = TEST_CLEAR_REGISTER_VALUE;
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, offset, write_data), OSAL_RETURN_OK);

    // Test02_012_005
    // @brief R_OSAL_ThsyncCondSignal to one of three waiting threads
    EXPECT_EQ(R_OSAL_ThsyncMutexTryLock(local_arg->mtx_handle), OSAL_RETURN_OK) << "Test02_012_005 failed";
    EXPECT_EQ(R_OSAL_ThsyncCondSignal(local_arg->cond_handle), OSAL_RETURN_OK) << "Test02_012_005 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(local_arg->mtx_handle), OSAL_RETURN_OK) << "Test02_012_005 failed";
}
// [osal_cond_isr_test_TC_02_012_005]

// [osal_cond_isr_test]
void osal_cond_isr_test(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    uintptr_t offset = TEST_REG_CLEAR_REGISTER;
    uint32_t write_data = TEST_CLEAR_REGISTER_VALUE;
    osal_milli_sec_t time = 1;
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, offset, write_data), OSAL_RETURN_OK);

    // Test02_012_005
    // @brief R_OSAL_ThsyncCondSignal to one of three waiting threads
    EXPECT_EQ(R_OSAL_ThsyncCondSignal(local_arg->cond_handle), OSAL_RETURN_OK) << "Test02_012_005 failed";

    // Test02_011_006
    // @brief R_OSAL_ThsyncCondBroadcast to two remaining waiting threads
    EXPECT_EQ(R_OSAL_ThsyncCondBroadcast(local_arg->cond_handle), OSAL_RETURN_OK) << "Test02_011_006 failed";
}
// [osal_cond_isr_test]

// [isr_test_waiting_thread]
int64_t isr_test_waiting_thread(void * arg)
{
    osal_milli_sec_t mutex_timeout = 1000;
    osal_milli_sec_t cond_timeout = 2000;
    osal_milli_sec_t msg_timeout = 1000;
    int32_t send_msg = 0;

    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)arg;
    EXPECT_EQ(R_OSAL_MqReset(local_arg->mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(local_arg->mtx_handle, mutex_timeout), OSAL_RETURN_OK);

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, msg_timeout, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);

    EXPECT_EQ(R_OSAL_ThsyncCondWaitForTimePeriod(local_arg->cond_handle, local_arg->mtx_handle, cond_timeout), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(local_arg->mtx_handle), OSAL_RETURN_OK);

    return (int64_t) 0;
}
// [isr_test_waiting_thread]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondCreate_TC_02_007_001 ThreadSyncCondApiTest.ThsyncCondCreate_TC_02_007_001
 * @subsection ThsyncCondCreate_TC_02_007_001_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondCreate_TC_02_007_001_tci Test Case ID
 * ThsyncCondCreate_S1_N_TC_02_007_001
 * @subsection ThsyncCondCreate_TC_02_007_001_rr Related Requirement(s)
 * \#199158
 * @subsection ThsyncCondCreate_TC_02_007_001_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondCreate_TC_02_007_001_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to create a new condition variable with normal condition.
 * - Test02_007_001: all input/output param is valid.
 * - post-process: destroy created condition variable
 * @subsection ThsyncCondCreate_TC_02_007_001_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_THREAD_ID1
 *      - Address of "cond_handle"
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy.
 * @subsection ThsyncCondCreate_TC_02_007_001_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.<br>
 * "cond_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncCondCreate_TC_02_007_001_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondCreate_TC_02_007_001
 */
// [ThsyncCondCreate_TC_02_007_001]
TEST_F(ThreadSyncCondApiTest, ThsyncCondCreate_TC_02_007_001)
{
    osal_cond_id_t cond_id;

// Test02_007_001
// @brief: normal case: all input/output param is valid
    cond_id = TEST_COND_ID1;
    ASSERT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "Test02_007_001 failed";
    ASSERT_NE(cond_handle, NULL_PTR) << "Test02_007_001 failed";

// post-process: destroy created cond
    EXPECT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK) << "post-process failed";
    cond_handle = OSAL_COND_HANDLE_INVALID;
}
// [ThsyncCondCreate_TC_02_007_001]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondCreate_TC_02_007_002 ThreadSyncCondApiTest.ThsyncCondCreate_TC_02_007_002
 * @subsection ThsyncCondCreate_TC_02_007_002_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondCreate_TC_02_007_002_tci Test Case ID
 * ThsyncCondCreate_S0_A_TC_02_007_002
 * @subsection ThsyncCondCreate_TC_02_007_002_rr Related Requirement(s)
 * \#199149, \#199158
 * @subsection ThsyncCondCreate_TC_02_007_002_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondCreate_TC_02_007_002_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to create a new condition variable with abnormal condition
 * - Test02_007_002: "cond_handle" is NULL.
 * @subsection ThsyncCondCreate_TC_02_007_002_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - "cond_handle" is NULL
 * @subsection ThsyncCondCreate_TC_02_007_002_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_PAR.
 * @subsection ThsyncCondCreate_TC_02_007_002_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondCreate_TC_02_007_002
 */
// [ThsyncCondCreate_TC_02_007_002]
TEST_F(ThreadSyncCondApiTest, ThsyncCondCreate_TC_02_007_002)
{
    osal_cond_id_t cond_id;
    osal_cond_handle_t* p_handle = NULL;

// Test02_007_002
// @brief: abnormal case: cond handler is NULL
    ASSERT_EQ(R_OSAL_ThsyncCondCreate(cond_id, p_handle), OSAL_RETURN_PAR) << "Test02_007_002 failed";
}
// [ThsyncCondCreate_TC_02_007_002]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondCreate_TC_02_007_003 ThreadSyncCondApiTest.ThsyncCondCreate_TC_02_007_003
 * @subsection ThsyncCondCreate_TC_02_007_003_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondCreate_TC_02_007_003_tci Test Case ID
 * ThsyncCondCreate_S2_A_TC_02_007_003
 * @subsection ThsyncCondCreate_TC_02_007_003_rr Related Requirement(s)
 * \#199158
 * @subsection ThsyncCondCreate_TC_02_007_003_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondCreate_TC_02_007_003_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to create a new condition variable with abnormal condition
 * - pre-process: create condition variable
 * - Test02_007_003: create new the second cond which id is same the previous one.
 * - post-process: destroy created condition variable
 * @subsection ThsyncCondCreate_TC_02_007_003_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Create second condition variable which id is the same as first created condition variable by using R_OSAL_ThsyncCondCreate.
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy.
 * @subsection ThsyncCondCreate_TC_02_007_003_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.<br>
 * "cond_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_BUSY.
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncCondCreate_TC_02_007_003_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondCreate_TC_02_007_003
 */
// [ThsyncCondCreate_TC_02_007_003]
TEST_F(ThreadSyncCondApiTest, ThsyncCondCreate_TC_02_007_003)
{
    osal_cond_id_t cond_id;

// pre-process: create new cond
    cond_id = TEST_COND_ID1;
    ASSERT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(cond_handle, NULL_PTR) << "pre-process failed";

// Test02_007_003
// @brief: abnormal case: create new the second cond which id is same the previous one.
    ASSERT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle1), OSAL_RETURN_BUSY) << "Test02_007_003 failed";

// post-process: destroy created cond
    EXPECT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK) << "post-process failed";
    cond_handle = OSAL_COND_HANDLE_INVALID;
}
// [ThsyncCondCreate_TC_02_007_003]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section OsalApiStateTest_ThsyncCondCreate_TC_02_007_004 OsalApiStateTest.ThsyncCondCreate_TC_02_007_004
 * @subsection ThsyncCondCreate_TC_02_007_004_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection ThsyncCondCreate_TC_02_007_004_tci Test Case ID
 * ThsyncCondCreate_S2_A_TC_02_007_004
 * @subsection ThsyncCondCreate_TC_02_007_004_rr Related Requirement(s)
 * \#199158
 * @subsection ThsyncCondCreate_TC_02_007_004_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection ThsyncCondCreate_TC_02_007_004_tcs Test Case Summary
 * This test shall verify the state transition of ThreadSync Condition APIs with abnormal condition:
 * - Test02_007_004: state transition error of R_OSAL_ThsyncCondCreate
 * @subsection ThsyncCondCreate_TC_02_007_004_tpts Test Procedure/Test Steps
 * -# Initialize the cond_handle with COND_HANDLE_INITIALIZED_VALUE by using memset.<br>
 * COND_HANDLE_INITIALIZED_VALUE must be different from OSAL_COND_HANDLE_INVALID
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - cond_handle is COND_HANDLE_INITIALIZED_VALUE
 * @subsection ThsyncCondCreate_TC_02_007_004_ereb Expected Result/Behavior
 * -# memset a value for cond_handle successfully
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_STATE.<br>
 * "cond_handle" is OSAL_COND_HANDLE_INVALID.
 * @subsection ThsyncCondCreate_TC_02_007_004_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondCreate_TC_02_007_004
 */
// [ThsyncCondCreate_TC_02_007_004]
TEST_F(OsalApiStateTest, ThsyncCondCreate_TC_02_007_004)
{
    osal_cond_id_t cond_id = TEST_COND_ID1;
    osal_cond_handle_t cond_handle = OSAL_COND_HANDLE_INVALID;
    memset(&cond_handle, COND_HANDLE_INITIALIZED_VALUE, sizeof(cond_handle));

    // Test02_007_004
    // @brief: abnormal case: R_OSAL_ThsyncCondCreate state transition error
    EXPECT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_STATE) << "Test02_007_004 failed";
    EXPECT_EQ(cond_handle, OSAL_COND_HANDLE_INVALID) << "Test02_007_004 failed";
}
// [ThsyncCondCreate_TC_02_007_004]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondDestroy_TC_02_008_001 ThreadSyncCondApiTest.ThsyncCondDestroy_TC_02_008_001
 * @subsection ThsyncCondDestroy_TC_02_008_001_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondDestroy_TC_02_008_001_tci Test Case ID
 * ThsyncCondDestroy_S1_N_TC_02_008_001
 * @subsection ThsyncCondDestroy_TC_02_008_001_rr Related Requirement(s)
 * \#199159
 * @subsection ThsyncCondDestroy_TC_02_008_001_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondDestroy_TC_02_008_001_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to destroy a condition variable with normal condition:
 * - pre-process: create condition variable object
 * - Test02_008_001: all input/output param is valid.
 * @subsection ThsyncCondDestroy_TC_02_008_001_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy.
 * @subsection ThsyncCondDestroy_TC_02_008_001_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.<br>
 * "cond_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncCondDestroy_TC_02_008_001_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondDestroy_TC_02_008_001
 */
// [ThsyncCondDestroy_TC_02_008_001]
TEST_F(ThreadSyncCondApiTest, ThsyncCondDestroy_TC_02_008_001)
{
    osal_cond_id_t cond_id;

// pre-process: create new cond
    cond_id = TEST_COND_ID1;
    ASSERT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(cond_handle, NULL_PTR) << "pre-process failed";

// Test02_008_001
// @brief: normal case: all input/output param is valid
    ASSERT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK) << "Test02_008_001 failed";
    cond_handle = OSAL_COND_HANDLE_INVALID;
}
// [ThsyncCondDestroy_TC_02_008_001]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondDestroy_TC_02_008_002 ThreadSyncCondApiTest.ThsyncCondDestroy_TC_02_008_002
 * @subsection ThsyncCondDestroy_TC_02_008_002_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondDestroy_TC_02_008_002_tci Test Case ID
 * ThsyncCondDestroy_S0_A_TC_02_008_002
 * @subsection ThsyncCondDestroy_TC_02_008_002_rr Related Requirement(s)
 * \#199149, \#199151, \#199159
 * @subsection ThsyncCondDestroy_TC_02_008_002_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondDestroy_TC_02_008_002_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to destroy a condition variable with abnormal condition:
 * - Test02_008_002: "cond_handle" is NULL.
 * @subsection ThsyncCondDestroy_TC_02_008_002_tpts Test Procedure/Test Steps
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy with the argument as below:
 *      - "cond_handle" is NULL
 * @subsection ThsyncCondDestroy_TC_02_008_002_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_HANDLE.
 * @subsection ThsyncCondDestroy_TC_02_008_002_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondDestroy_TC_02_008_002
 */
// [ThsyncCondDestroy_TC_02_008_002]
TEST_F(ThreadSyncCondApiTest, ThsyncCondDestroy_TC_02_008_002)
{
// Test02_008_002
// @brief: abnormal case: cond handler is NULL
    ASSERT_EQ(R_OSAL_ThsyncCondDestroy(OSAL_COND_HANDLE_INVALID), OSAL_RETURN_HANDLE) << "Test02_008_002 failed";
}
// [ThsyncCondDestroy_TC_02_008_002]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondDestroy_TC_02_008_003 ThreadSyncCondApiTest.ThsyncCondDestroy_TC_02_008_003
 * @subsection ThsyncCondDestroy_TC_02_008_003_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondDestroy_TC_02_008_003_tci Test Case ID
 * ThsyncCondDestroy_S2_A_TC_02_008_003
 * @subsection ThsyncCondDestroy_TC_02_008_003_rr Related Requirement(s)
 * \#199159
 * @subsection ThsyncCondDestroy_TC_02_008_003_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondDestroy_TC_02_008_003_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to destroy a condition variable with abnormal condition:
 * - pre-process: create new condition variable oject
 * - Test02_008_003: "cond_handle" has already been destroyed.
 * @subsection ThsyncCondDestroy_TC_02_008_003_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy.
 * -# Destroy condition variable which handle was destroyed by using R_OSAL_ThsyncCondDestroy.
 * @subsection ThsyncCondDestroy_TC_02_008_003_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.<br>
 * "cond_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_HANDLE.
 * @subsection ThsyncCondDestroy_TC_02_008_003_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondDestroy_TC_02_008_003
 */
// [ThsyncCondDestroy_TC_02_008_003]
TEST_F(ThreadSyncCondApiTest, ThsyncCondDestroy_TC_02_008_003)
{
    osal_cond_id_t cond_id;

// pre-process: create new cond
    cond_id = TEST_COND_ID1;
    ASSERT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(cond_handle, NULL_PTR) << "pre-process failed";

// Test02_008_003
// @brief: abnormal case: cond handler has already been destroyed
    ASSERT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK) << "Test02_008_003 failed";
    ASSERT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_HANDLE) << "Test02_008_003 failed";
    cond_handle = OSAL_COND_HANDLE_INVALID;
}
// [ThsyncCondDestroy_TC_02_008_003]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondDestroy_TC_02_008_004 ThreadSyncCondApiTest.ThsyncCondDestroy_TC_02_008_004
 * @subsection ThsyncCondDestroy_TC_02_008_004_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondDestroy_TC_02_008_004_tci Test Case ID
 * ThsyncCondDestroy_S2_A_TC_02_008_004
 * @subsection ThsyncCondDestroy_TC_02_008_004_rr Related Requirement(s)
 * \#199159
 * @subsection ThsyncCondDestroy_TC_02_008_004_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondDestroy_TC_02_008_004_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to destroy a condition variable with abnormal condition:
 * - pre-process: create new mutex and condition variable oject
 * - Test02_008_004: cond handler is under referencing (R_OSAL_ThsyncCondTimedWaitPeriod).
 * - post-process: destroy created mutex and condition variable
 * @subsection ThsyncCondDestroy_TC_02_008_004_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Create first thread using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is conddestroy_first_workerfunction function with functionality:
 *              - Get the config of synchronization queue by using R_OSAL_MqGetConfig
 *              - Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod.
 *              - Wait on a condition variable by using R_OSAL_ThsyncCondWaitForTimePeriod.
 *              - Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 *          -# userarg includes condition variable handle and mutex thread
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Address of first thread handle.
 * -# Get the config of synchronization queue by using R_OSAL_MqGetConfig
 * -# Receive a message from synchronization queue by using R_OSAL_MqReceiveForTimePeriod.
 * -# Create second thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is conddestroy_second_workerfunction function with functionality:
 *              - Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 *              - Destroy condition variable by using R_OSAL_ThsyncCondDestroy.
 *              - Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 *          -# userarg includes condition variable handle and mutex thread
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2.
 *      - Address of second thread handle.
 * -# Join first thread by using R_OSAL_ThreadJoin.
 * -# Join second thread by using R_OSAL_ThreadJoin.
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncCondDestroy_TC_02_008_004_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.<br>
 * "cond_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL_PTR
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with first thread.<br>
 * "thread_handle" is not NULL_PTR.<br>
 * In conddestroy_first_workerfunction function:<br>
 *  - R_OSAL_MqGetConfig return OSAL_RETURN_OK
 *  - R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 *  - R_OSAL_ThsyncCondWaitForTimePeriod return OSAL_RETURN_TIME.
 *  - R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_MqGetConfig return OSAL_RETURN_OK
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK.<br>
 * Value of "receive_msg" is equal to "send_msg".
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with second thread.<br>
 * "thread_handle1" is not NULL_PTR.<br>
 * In conddestroy_second_workerfunction function:
 *  - R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncCondDestroy return OSAL_RETURN_BUSY.
 *  - R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with first thread.<br>
 * Value of "result" is equal to 0.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with second thread.<br>
 * Value of "result" is equal to 0.
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncCondDestroy_TC_02_008_004_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondDestroy_TC_02_008_004
 * @snippet test_cond_var.cpp conddestroy_first_workerfunction
 * @snippet test_cond_var.cpp conddestroy_second_workerfunction
 */
// [ThsyncCondDestroy_TC_02_008_004]
TEST_F(ThreadSyncCondApiTest, ThsyncCondDestroy_TC_02_008_004)
{
    osal_cond_id_t cond_id;
    osal_mutex_id_t mutex_id;
    osal_thread_id_t uid;
    st_osal_thread_config_t config;
    st_osal_mq_config_t local_mq_config;
    int64_t result;
    int receive_msg = 0;

// pre-process: create new mutex and cond
    cond_id = TEST_COND_ID1;
    ASSERT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(cond_handle, NULL_PTR) << "pre-process failed";
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_008_004
// @brief: abnormal case: cond handler is under referencing (R_OSAL_ThsyncCondTimedWaitPeriod)
    condperiod_worker_arg user_arg(&cond_handle, &mtx_handle);
    uid              = TEST_THREAD_ID1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = (char *)"Simple testing thread";
    config.func      = conddestroy_first_workerfunction;
    config.userarg   = &user_arg;
    config.stack_size = THREAD_STACK_SIZE;
    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK);
    ASSERT_NE(thread_handle, NULL_PTR);
    EXPECT_EQ(R_OSAL_MqGetConfig(mq_handle, &local_mq_config), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(mq_handle, SYNC_MQ_TIME, &receive_msg, local_mq_config.msg_size), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

    uid              = TEST_THREAD_ID2;
    config.func      = conddestroy_second_workerfunction;
    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle1), OSAL_RETURN_OK);
    ASSERT_NE(thread_handle1, NULL_PTR);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK);
    EXPECT_EQ(result, 0) << "Test02_008_004 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &result), OSAL_RETURN_OK);
    EXPECT_EQ(result, 0) << "Test02_008_004 failed";
    thread_handle = OSAL_THREAD_HANDLE_INVALID;
    thread_handle1 = OSAL_THREAD_HANDLE_INVALID;

// post-process: destroy created mutex and cond
    EXPECT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
    cond_handle = OSAL_COND_HANDLE_INVALID;
}
// [ThsyncCondDestroy_TC_02_008_004]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section OsalApiStateTest_ThsyncCondDestroy_TC_02_008_005 OsalApiStateTest.ThsyncCondDestroy_TC_02_008_005
 * @subsection ThsyncCondDestroy_TC_02_008_005_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection ThsyncCondDestroy_TC_02_008_005_tci Test Case ID
 * ThsyncCondDestroy_S2_A_TC_02_008_005
 * @subsection ThsyncCondDestroy_TC_02_008_005_rr Related Requirement(s)
 * \#199159
 * @subsection ThsyncCondDestroy_TC_02_008_005_pp Prerequisite/Precondition
  * - R_OSAL_Deinitialize is already called
 * @subsection ThsyncCondDestroy_TC_02_008_005_tcs Test Case Summary
 * This test shall verify the state transition of ThreadSync Condition APIs with abnormal condition:
 * - Test02_008_005: state transition error of R_OSAL_ThsyncCondDestroy
 * @subsection ThsyncCondDestroy_TC_02_008_005_tpts Test Procedure/Test Steps
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy.
 * @subsection ThsyncCondDestroy_TC_02_008_005_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_STATE.
 * @subsection ThsyncCondDestroy_TC_02_008_005_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondDestroy_TC_02_008_005
 */
// [ThsyncCondDestroy_TC_02_008_005]
TEST_F(OsalApiStateTest, ThsyncCondDestroy_TC_02_008_005)
{
    osal_cond_handle_t cond_handle = OSAL_COND_HANDLE_INVALID;

    // Test02_008_005
    // @brief: abnormal case: R_OSAL_ThsyncCondDestroy state transition error
    EXPECT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_STATE) << "Test02_008_005 failed";
}
// [ThsyncCondDestroy_TC_02_008_005]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondDestroy_TC_02_008_006 ThreadSyncCondApiTest.ThsyncCondDestroy_TC_02_008_006
 * @subsection ThsyncCondDestroy_TC_02_008_006_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondDestroy_TC_02_008_006_tci Test Case ID
 * ThsyncCondDestroy_S1_N_TC_02_008_006
 * @subsection ThsyncCondDestroy_TC_02_008_006_rr Related Requirement(s)
 * \#199159
 * @subsection ThsyncCondDestroy_TC_02_008_006_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondDestroy_TC_02_008_006_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to destroy a condition variable with normal condition:
 * - pre-process: create condition variable and mutex objects
 * - Test02_008_006: confirm R_OSAL_ThsyncCondDestroy with the sequence call as below:
 *      -# CondWait
 *      -# CondSignal from another thread
 *      -# CondDestroy
 * - post-process: destroy created mutex object
 * @subsection ThsyncCondDestroy_TC_02_008_006_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Create first thread using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is condperiodwait_workerfunction function with functionality:
 *              - Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 *              - Wait on a condition variable by using R_OSAL_ThsyncCondWaitForTimePeriod.
 *              - Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 *          -# userarg includes condition variable handle and mutex handle
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1
 *      - Address of first thread handle.
 * -# Create second thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is condperiodwait_wakeup_workerfunction function with functionality:
 *              - Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 *              - Unblock thread that are blocked on the condition variable by using R_OSAL_ThsyncCondSignal.
 *              - Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 *          -# userarg includes condition variable handle and mutex handle
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2
 *      - Address of second thread handle.
 * -# Join first thread by using R_OSAL_ThreadJoin.
 * -# Join second thread by using R_OSAL_ThreadJoin.
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy.
 * -# Destroy mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncCondDestroy_TC_02_008_006_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.<br>
 * "cond_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * "mtx_handle" is not NULL_PTR.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with first thread.<br>
 * "thread_handle" is not NULL_PTR.<br>
 * In condperiodwait_workerfunction function:
 *  - R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncCondWaitForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with second thread.<br>
 * "thread_handle1" is not NULL_PTR.<br>
 * In condperiodwait_wakeup_workerfunction function:
 *  - R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncCondSignal return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with first thread.<br>
 * Value of "result" is equal to 0.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with second thread.<br>
 * Value of "result" is equal to 0.
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncCondDestroy_TC_02_008_006_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondDestroy_TC_02_008_006
 * @snippet test_cond_var.cpp condperiodwait_workerfunction
 * @snippet test_cond_var.cpp condperiodwait_wakeup_workerfunction
 */
// [ThsyncCondDestroy_TC_02_008_006]
TEST_F(ThreadSyncCondApiTest, ThsyncCondDestroy_TC_02_008_006)
{
    osal_cond_id_t cond_id;
    osal_mutex_id_t mutex_id;
    osal_thread_id_t uid;
    st_osal_thread_config_t config;
    int64_t result;
    osal_milli_sec_t time_period = 2000;
    int32_t receive_msg = 0;

// pre-process: create new mutex and cond
    cond_id = TEST_COND_ID1;
    EXPECT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(cond_handle, NULL_PTR) << "pre-process failed";
    mutex_id = TEST_MUTEX_ID1;
    EXPECT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_008_006
// @brief: abnormal case: sending a signal on the condition from another thread. 
    condperiod_worker_arg user_arg(&cond_handle, &mtx_handle);
    uid              = TEST_THREAD_ID1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = (char *)"Simple testing thread";
    config.func      = condperiodwait_workerfunction;
    config.userarg   = &user_arg;
    config.stack_size = THREAD_STACK_SIZE;
    EXPECT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK) << "Test02_008_006 failed";
    EXPECT_NE(thread_handle, NULL_PTR);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(mq_handle, time_period, &receive_msg, sizeof(receive_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

    uid              = TEST_THREAD_ID2;
    config.func      = condperiodwait_wakeup_workerfunction;
    EXPECT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle1), OSAL_RETURN_OK) << "Test02_008_006 failed";
    EXPECT_NE(thread_handle1, NULL_PTR);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK) << "Test02_008_006 failed";
    EXPECT_EQ(result, 0) << "Test02_008_006 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &result), OSAL_RETURN_OK) << "Test02_008_006 failed";
    EXPECT_EQ(result, 0) << "Test02_008_006 failed";
    thread_handle = OSAL_THREAD_HANDLE_INVALID;
    thread_handle1 = OSAL_THREAD_HANDLE_INVALID;
    EXPECT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK) << "Test02_008_006 failed";

// post-process: destroy created mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
    cond_handle = OSAL_COND_HANDLE_INVALID;
}
// [ThsyncCondDestroy_TC_02_008_006]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondDestroy_TC_02_008_007 ThreadSyncCondApiTest.ThsyncCondDestroy_TC_02_008_007
 * @subsection ThsyncCondDestroy_TC_02_008_007_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondDestroy_TC_02_008_007_tci Test Case ID
 * ThsyncCondDestroy_S1_N_TC_02_008_007
 * @subsection ThsyncCondDestroy_TC_02_008_007_rr Related Requirement(s)
 * \#199159
 * @subsection ThsyncCondDestroy_TC_02_008_007_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondDestroy_TC_02_008_007_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to destroy a condition variable with normal condition:
 * - pre-process: create mutex and condition variable objects
 * - Test02_008_007: confirm R_OSAL_ThsyncCondDestroy with the sequence call as below:
 *      -# CondWait (timeout)
 *      -# CondDestroy
 * - post-process: destroy created mutex 
 * @subsection ThsyncCondDestroy_TC_02_008_007_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Lock mutex by using R_OSAL_ThsyncMutexTryLock.
 * -# Wait on a condition variable by using R_OSAL_ThsyncCondWaitForTimePeriod.
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncCondDestroy_TC_02_008_007_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.<br>
 * "cond_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondWaitForTimePeriod return OSAL_RETURN_TIME.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncCondDestroy_TC_02_008_007_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondDestroy_TC_02_008_007
 */
// [ThsyncCondDestroy_TC_02_008_007]
TEST_F(ThreadSyncCondApiTest, ThsyncCondDestroy_TC_02_008_007)
{
    osal_cond_id_t cond_id;
    osal_mutex_id_t mutex_id;
    osal_milli_sec_t time = 10;

// pre-process: create new cond and mutex
    cond_id = TEST_COND_ID1;
    ASSERT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(cond_handle, NULL_PTR) << "pre-process failed";
    mutex_id = TEST_MUTEX_ID1;
    EXPECT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_008_007
// @brief: normal case: destroy condition variable after no thread signal the cond cause the timeout 
    EXPECT_EQ(R_OSAL_ThsyncMutexTryLock(mtx_handle), OSAL_RETURN_OK) << "Test02_008_007 failed";
    EXPECT_EQ(R_OSAL_ThsyncCondWaitForTimePeriod(cond_handle, mtx_handle, time), OSAL_RETURN_TIME) << "Test02_008_007 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK) << "Test02_008_007 failed";
    EXPECT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK) << "Test02_008_007 failed";

// post-process: destroy created cond and mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
    cond_handle = OSAL_COND_HANDLE_INVALID;
}
// [ThsyncCondDestroy_TC_02_008_007]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondWaitForTimePeriod_TC_02_009_001 ThreadSyncCondApiTest.ThsyncCondWaitForTimePeriod_TC_02_009_001
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_001_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_001_tci Test Case ID
 * ThsyncCondWaitForTimePeriod_S1_N_TC_02_009_001
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_001_rr Related Requirement(s)
 * \#199160
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_001_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_001_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to wait on a condition variable with normal condition:
 * - pre-process: create new mutex and condition variable object
 * - Test02_009_001: all input/output param is valid.
 * - post-process: destroy created mutex and condition variable
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_001_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Create first thread using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is condperiodwait_workerfunction function with functionality:
 *              - Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 *              - Wait on a condition variable by using R_OSAL_ThsyncCondWaitForTimePeriod.
 *              - Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 *          -# userarg includes condition variable handle and mutex thread
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Address of first thread handle.
 * -# Create second thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is condperiodwait_wakeup_workerfunction function with functionality:
 *              - Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 *              - Unblock thread that are blocked on the condition variable by using R_OSAL_ThsyncCondSignal.
 *              - Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 *          -# userarg includes condition variable handle and mutex thread
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2.
 *      - Address of second thread handle.
 * -# Join first thread by using R_OSAL_ThreadJoin.
 * -# Join second thread by using R_OSAL_ThreadJoin.
 * -# Destroy mutex by using R_OSAL_ThsyncMutexDestroy
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_001_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.<br>
 * "cond_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * "mtx_handle" is not NULL_PTR.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with first thread.<br>
 * "thread_handle" is not NULL_PTR.
 * In condperiodwait_workerfunction function:<br>
 *  - R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncCondWaitForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with second thread.<br>
 * "thread_handle1" is not NULL_PTR.
 * In condperiodwait_wakeup_workerfunction function:<br>
 *  - R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncCondSignal return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with first thread.<br>
 * Value of "result" is equal to 0.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with second thread.<br>
 * Value of "result" is equal to 0.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_001_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondWaitForTimePeriod_TC_02_009_001
 * @snippet test_cond_var.cpp condperiodwait_workerfunction
 * @snippet test_cond_var.cpp condperiodwait_wakeup_workerfunction
 */
// [ThsyncCondWaitForTimePeriod_TC_02_009_001]
TEST_F(ThreadSyncCondApiTest, ThsyncCondWaitForTimePeriod_TC_02_009_001)
{
    osal_cond_id_t cond_id;
    osal_mutex_id_t mutex_id;
    osal_thread_id_t uid;
    st_osal_thread_config_t config;
    int64_t result;
    osal_milli_sec_t time_period = 2000;
    int32_t receive_msg = 0;

// pre-process: create new cond and mutex
    cond_id = TEST_COND_ID1;
    ASSERT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(cond_handle, NULL_PTR) << "pre-process failed";
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_009_001
// @brief: normal case: all input/output param is valid
    condperiod_worker_arg user_arg(&cond_handle, &mtx_handle);
    uid              = TEST_THREAD_ID1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = (char *)"Simple testing thread";
    config.func      = condperiodwait_workerfunction;
    config.userarg   = &user_arg;
    config.stack_size = THREAD_STACK_SIZE;
    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK);
    ASSERT_NE(thread_handle, NULL_PTR);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(mq_handle, time_period, &receive_msg, sizeof(receive_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

    uid              = TEST_THREAD_ID2;
    config.func      = condperiodwait_wakeup_workerfunction;
    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle1), OSAL_RETURN_OK);
    ASSERT_NE(thread_handle1, NULL_PTR);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK);
    EXPECT_EQ(result, 0) << "Test02_009_001 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &result), OSAL_RETURN_OK);
    EXPECT_EQ(result, 0) << "Test02_009_001 failed";
    thread_handle = OSAL_THREAD_HANDLE_INVALID;
    thread_handle1 = OSAL_THREAD_HANDLE_INVALID;

// post-process: destroy created cond and mutex
    ASSERT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    ASSERT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK) << "post-process failed";
    cond_handle = OSAL_COND_HANDLE_INVALID;
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncCondWaitForTimePeriod_TC_02_009_001]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondWaitForTimePeriod_TC_02_009_002 ThreadSyncCondApiTest.ThsyncCondWaitForTimePeriod_TC_02_009_002
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_002_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_002_tci Test Case ID
 * ThsyncCondWaitForTimePeriod_S1_BN_TC_02_009_002
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_002_rr Related Requirement(s)
 * \#199160
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_002_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_002_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to wait on a condition variable with normal condition:
 * - pre-process: create new mutex and condition variable object
 * - Test02_009_002: timeout is 0.
 * - post-process: destroy created mutex and condition variable
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_002_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Lock mutex with R_OSAL_ThsyncMutexTryLock.
 * -# Wait on a condition variable by using R_OSAL_ThsyncCondWaitForTimePeriod with the argument as below:
 *      - timeout is 0
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 * -# Destroy mutex by using R_OSAL_ThsyncMutexDestroy.
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy.
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_002_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.<br>
 * "cond_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * "mtx_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondWaitForTimePeriod return OSAL_RETURN_TIME.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_002_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondWaitForTimePeriod_TC_02_009_002
 */
// [ThsyncCondWaitForTimePeriod_TC_02_009_002]
TEST_F(ThreadSyncCondApiTest, ThsyncCondWaitForTimePeriod_TC_02_009_002)
{
    osal_cond_id_t cond_id;
    osal_mutex_id_t mutex_id;
    osal_milli_sec_t time;

// pre-process: create new cond and mutex
    cond_id = TEST_COND_ID1;
    ASSERT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(cond_handle, NULL_PTR) << "pre-process failed";
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_009_002
// @brief: boundary normal case: timeout is 0
    time = 0;
    ASSERT_EQ(R_OSAL_ThsyncMutexTryLock(mtx_handle), OSAL_RETURN_OK);
    ASSERT_EQ(R_OSAL_ThsyncCondWaitForTimePeriod(cond_handle, mtx_handle, time), OSAL_RETURN_TIME) << "Test02_009_002 failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK);

// post-process: destroy created cond and mutex
    ASSERT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    ASSERT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK) << "post-process failed";
    cond_handle = OSAL_COND_HANDLE_INVALID;
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncCondWaitForTimePeriod_TC_02_009_002]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondWaitForTimePeriod_TC_02_009_003 ThreadSyncCondApiTest.ThsyncCondWaitForTimePeriod_TC_02_009_003
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_003_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_003_tci Test Case ID
 * ThsyncCondWaitForTimePeriod_S0_A_TC_02_009_003
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_003_rr Related Requirement(s)
 * \#199149, \#199151, \#199160
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_003_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_003_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to wait on a condition variable with abnormal condition:
 * - pre-process: create mutex object
 * - Test02_009_003: "cond_handle" is NULL.
 * - post-process: destroy created mutex
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_003_tpts Test Procedure/Test Steps
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Lock mutex with R_OSAL_ThsyncMutexTryLock.
 * -# Wait on a condition variable by using R_OSAL_ThsyncCondWaitForTimePeriod with the argument as below:
 *      - "cond_handle" is NULL
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_003_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * "mtx_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondWaitForTimePeriod return OSAL_RETURN_HANDLE.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_003_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondWaitForTimePeriod_TC_02_009_003
 */
// [ThsyncCondWaitForTimePeriod_TC_02_009_003]
TEST_F(ThreadSyncCondApiTest, ThsyncCondWaitForTimePeriod_TC_02_009_003)
{
    osal_mutex_id_t mutex_id;
    osal_milli_sec_t time = 10;

// pre-process: create mutex
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_009_003
// @brief: abnormal case: cond handle is NULL
    ASSERT_EQ(R_OSAL_ThsyncMutexTryLock(mtx_handle), OSAL_RETURN_OK);
    ASSERT_EQ(R_OSAL_ThsyncCondWaitForTimePeriod(OSAL_COND_HANDLE_INVALID, mtx_handle, time), OSAL_RETURN_HANDLE) << "Test02_009_003 failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK);

// post-process: destroy created mutex
    ASSERT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncCondWaitForTimePeriod_TC_02_009_003]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondWaitForTimePeriod_TC_02_009_004 ThreadSyncCondApiTest.ThsyncCondWaitForTimePeriod_TC_02_009_004
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_004_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_004_tci Test Case ID
 * ThsyncCondWaitForTimePeriod_S0_A_TC_02_009_004
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_004_rr Related Requirement(s)
 * \#199149, \#199151, \#199160
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_004_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_004_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to wait on a condition variable with abnormal condition:
 * - pre-process: create mutex and condition variable object
 * - Test02_009_004: "mtx_handle" is NULL.
 * - post-process: destroy created mutex and condition variable object
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_004_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Lock mutex with R_OSAL_ThsyncMutexTryLock.
 * -# Wait on a condition variable by using R_OSAL_ThsyncCondWaitForTimePeriod with the argument as below:
 *      - "mtx_handle" is NULL
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_004_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.<br>
 * "cond_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * "mtx_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondWaitForTimePeriod return OSAL_RETURN_HANDLE.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_004_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondWaitForTimePeriod_TC_02_009_004
 */
// [ThsyncCondWaitForTimePeriod_TC_02_009_004]
TEST_F(ThreadSyncCondApiTest, ThsyncCondWaitForTimePeriod_TC_02_009_004)
{
    osal_cond_id_t cond_id;
    osal_mutex_id_t mutex_id;
    osal_milli_sec_t time = 10;

// pre-process: create mutex and cond
    cond_id = TEST_COND_ID1;
    ASSERT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(cond_handle, NULL_PTR) << "pre-process failed";
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_009_004
// @brief: abnormal case: mutex handle is NULL
    ASSERT_EQ(R_OSAL_ThsyncMutexTryLock(mtx_handle), OSAL_RETURN_OK);
    ASSERT_EQ(R_OSAL_ThsyncCondWaitForTimePeriod(cond_handle, OSAL_MUTEX_HANDLE_INVALID, time), OSAL_RETURN_HANDLE) << "Test02_009_004 failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK);

// post-process: destroy created mutex and cond
    ASSERT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK) << "post-process failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    cond_handle = OSAL_COND_HANDLE_INVALID;
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncCondWaitForTimePeriod_TC_02_009_004]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondWaitForTimePeriod_TC_02_009_005 ThreadSyncCondApiTest.ThsyncCondWaitForTimePeriod_TC_02_009_005
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_005_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_005_tci Test Case ID
 * ThsyncCondWaitForTimePeriod_S0_BA_TC_02_009_005
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_005_rr Related Requirement(s)
 * \#199160
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_005_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_005_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to wait on a condition variable with abnormal condition:
 * - pre-process: create mutex and condition variable object
 * - Test02_009_005: timeout is negative number.
 * - post-process: destroy created mutex and condition variable object
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_005_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Lock mutex with R_OSAL_ThsyncMutexTryLock.
 * -# Wait on a condition variable with timeout is negative number by using R_OSAL_ThsyncCondWaitForTimePeriod.
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_005_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.<br>
 * "cond_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * "mtx_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondWaitForTimePeriod return OSAL_RETURN_PAR.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_005_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondWaitForTimePeriod_TC_02_009_005
 */
// [ThsyncCondWaitForTimePeriod_TC_02_009_005]
TEST_F(ThreadSyncCondApiTest, ThsyncCondWaitForTimePeriod_TC_02_009_005)
{
    osal_cond_id_t cond_id;
    osal_mutex_id_t mutex_id;
    osal_milli_sec_t time;

// pre-process: create mutex and cond
    cond_id = TEST_COND_ID1;
    ASSERT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(cond_handle, NULL_PTR) << "pre-process failed";
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_009_005
// @brief: abnormal case: timeout is negative number
    time = -10;
    ASSERT_EQ(R_OSAL_ThsyncMutexTryLock(mtx_handle), OSAL_RETURN_OK);
    ASSERT_EQ(R_OSAL_ThsyncCondWaitForTimePeriod(cond_handle, mtx_handle, time), OSAL_RETURN_PAR) << "Test02_009_005 failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK);

// post-process: destroy created mutex and cond
    ASSERT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK) << "post-process failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    cond_handle = OSAL_COND_HANDLE_INVALID;
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncCondWaitForTimePeriod_TC_02_009_005]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondWaitForTimePeriod_TC_02_009_006 ThreadSyncCondApiTest.ThsyncCondWaitForTimePeriod_TC_02_009_006
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_006_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_006_tci Test Case ID
 * ThsyncCondWaitForTimePeriod_S2_A_TC_02_009_006
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_006_rr Related Requirement(s)
 * \#199160
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_006_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_006_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to wait on a condition variable with abnormal condition:
 * - pre-process: create mutex and condition variable object
 * - Test02_009_006: Not sending a signal on the condition.
 * - post-process: destroy created mutex and condition variable object.
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_006_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Create a thread using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is condperiodwait_timeout_workerfunction function with functionality:
 *              - Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 *              - Wait on a condition variable by using R_OSAL_ThsyncCondWaitForTimePeriod.
 *              - Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 *          -# userarg includes condition variable handle and mutex thread
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Address of thread handle.
 * -# Join "Simple testing thread" by using R_OSAL_ThreadJoin.
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_006_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.<br>
 * "cond_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * "mtx_handle" is not NULL_PTR.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK.<br>
 * "thread_handle" is not NULL_PTR.<br>
 * In condperiodwait_timeout_workerfunction function: <br>
 *  - R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncCondWaitForTimePeriod return OSAL_RETURN_TIME.
 *  - R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK.<br>
 * Value of "result" is equal to 0.
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_006_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondWaitForTimePeriod_TC_02_009_006
 * @snippet test_cond_var.cpp condperiodwait_timeout_workerfunction
 */
// [ThsyncCondWaitForTimePeriod_TC_02_009_006]
TEST_F(ThreadSyncCondApiTest, ThsyncCondWaitForTimePeriod_TC_02_009_006)
{
    osal_cond_id_t cond_id;
    osal_mutex_id_t mutex_id;
    osal_thread_id_t uid;
    st_osal_thread_config_t config;
    int64_t result;

// pre-process: create mutex and cond
    cond_id = TEST_COND_ID1;
    ASSERT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(cond_handle, NULL_PTR) << "pre-process failed";
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";
    condperiod_worker_arg user_arg(&cond_handle, &mtx_handle);

// Test02_009_006
// @brief: abnormal case: Not sending a signal on the condition.
    uid              = TEST_THREAD_ID1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = (char *)"Simple testing thread";
    config.func      = condperiodwait_timeout_workerfunction;
    config.userarg   = &user_arg;
    config.stack_size = THREAD_STACK_SIZE;
    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK);
    ASSERT_NE(thread_handle, NULL_PTR);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK);
    EXPECT_EQ(result, 0) << "Test02_009_006 failed";
    thread_handle = OSAL_THREAD_HANDLE_INVALID;

// post-process: destroy created mutex and cond
    ASSERT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK) << "post-process failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    cond_handle = OSAL_COND_HANDLE_INVALID;
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncCondWaitForTimePeriod_TC_02_009_006]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondWaitForTimePeriod_TC_02_009_007 ThreadSyncCondApiTest.ThsyncCondWaitForTimePeriod_TC_02_009_007
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_007_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_007_tci Test Case ID
 * ThsyncCondWaitForTimePeriod_S4_A_TC_02_009_007
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_007_rr Related Requirement(s)
 * \#199160
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_007_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_007_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to wait on a condition variable with abnormal condition:
 * - pre-process: create mutex and condition variable object
 * - Test02_009_007: the mutex is not owned by calling thread.
 * - post-process: destroy created mutex and condition variable object
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_007_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Wait on a condition variable with mutex handle is not locked by using R_OSAL_ThsyncCondWaitForTimePeriod.
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_007_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.<br>
 * "cond_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * "mtx_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncCondWaitForTimePeriod return OSAL_RETURN_FAIL
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_007_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondWaitForTimePeriod_TC_02_009_007
 */
// [ThsyncCondWaitForTimePeriod_TC_02_009_007]
TEST_F(ThreadSyncCondApiTest, ThsyncCondWaitForTimePeriod_TC_02_009_007)
{
    osal_cond_id_t cond_id;
    osal_mutex_id_t mutex_id;
    osal_milli_sec_t time;

// pre-process: create mutex and cond
    cond_id = TEST_COND_ID1;
    ASSERT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(cond_handle, NULL_PTR) << "pre-process failed";
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_009_007
// @brief: abnormal case: the mutex is not owned by calling thread.
    time = 10;
    ASSERT_EQ(R_OSAL_ThsyncCondWaitForTimePeriod(cond_handle, mtx_handle, time), OSAL_RETURN_FAIL) << "Test02_009_007 failed";

// post-process: destroy created mutex and cond
    ASSERT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK) << "post-process failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    cond_handle = OSAL_COND_HANDLE_INVALID;
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncCondWaitForTimePeriod_TC_02_009_007]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondWaitForTimePeriod_TC_02_009_009 ThreadSyncCondApiTest.ThsyncCondWaitForTimePeriod_TC_02_009_009
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_009_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_009_tci Test Case ID
 * ThsyncCondWaitForTimePeriod_S2_A_TC_02_009_009
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_009_rr Related Requirement(s)
 * \#199160
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_009_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_009_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to wait on a condition variable with abnormal condition:
 * - pre-process: create mutex and condition variable object
 * - Test02_009_009: cond handle has already been destroyed before the waiting.
 * - post-process: destroy created mutex (condition variable object has already been destroyed)
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_009_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy.
 * -# Lock mutex with R_OSAL_ThsyncMutexTryLock.
 * -# Wait on a condition variable which handle has already been destroyed by using R_OSAL_ThsyncCondWaitForTimePeriod.
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_009_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.<br>
 * "cond_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * "mtx_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondWaitForTimePeriod return OSAL_RETURN_HANDLE.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_009_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondWaitForTimePeriod_TC_02_009_009
 */
// [ThsyncCondWaitForTimePeriod_TC_02_009_009]
TEST_F(ThreadSyncCondApiTest, ThsyncCondWaitForTimePeriod_TC_02_009_009)
{
    osal_cond_id_t cond_id;
    osal_mutex_id_t mutex_id;
    osal_milli_sec_t time = 10;

// pre-process: create mutex and cond
    cond_id = TEST_COND_ID1;
    ASSERT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(cond_handle, NULL_PTR) << "pre-process failed";
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_009_009
// @brief: abnormal case: Cond handle has already been destroyed before the waiting
    ASSERT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK);
    ASSERT_EQ(R_OSAL_ThsyncMutexTryLock(mtx_handle), OSAL_RETURN_OK);
    ASSERT_EQ(R_OSAL_ThsyncCondWaitForTimePeriod(cond_handle, mtx_handle, time), OSAL_RETURN_HANDLE) << "Test02_009_009 failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK);

// post-process: destroy created mutex (condition variable object has already been destroyed)
    ASSERT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    cond_handle = OSAL_COND_HANDLE_INVALID;
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncCondWaitForTimePeriod_TC_02_009_009]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section OsalApiStateTest_ThsyncCondWaitForTimePeriod_TC_02_009_010 OsalApiStateTest.ThsyncCondWaitForTimePeriod_TC_02_009_010
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_010_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_010_tci Test Case ID
 * ThsyncCondWaitForTimePeriod_S2_A_TC_02_009_010
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_010_rr Related Requirement(s)
 * \#199160
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_010_pp Prerequisite/Precondition
  * - R_OSAL_Deinitialize is already called
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_010_tcs Test Case Summary
 * This test shall verify the state transition of ThreadSync Condition APIs with abnormal condition:
 * - Test02_009_010: state transition error of R_OSAL_ThsyncCondWaitForTimePeriod
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_010_tpts Test Procedure/Test Steps
 * -# Wait on a condition variable by using R_OSAL_ThsyncCondWaitForTimePeriod
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_010_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondWaitForTimePeriod return OSAL_RETURN_STATE.
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_010_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondWaitForTimePeriod_TC_02_009_010
 */
// [ThsyncCondWaitForTimePeriod_TC_02_009_010]
TEST_F(OsalApiStateTest, ThsyncCondWaitForTimePeriod_TC_02_009_010)
{
    osal_milli_sec_t time = 10;
    osal_cond_handle_t cond_handle = OSAL_COND_HANDLE_INVALID;
    osal_mutex_handle_t mtx_handle = OSAL_MUTEX_HANDLE_INVALID;

    // Test02_009_010
    // @brief: abnormal case: R_OSAL_ThsyncCondWaitForTimePeriod state transition error
    EXPECT_EQ(R_OSAL_ThsyncCondWaitForTimePeriod(cond_handle, mtx_handle, time), OSAL_RETURN_STATE) << "Test02_009_010 failed";
}
// [ThsyncCondWaitForTimePeriod_TC_02_009_010]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondWaitForTimePeriod_TC_02_009_011 ThreadSyncCondApiTest.ThsyncCondWaitForTimePeriod_TC_02_009_011
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_011_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_011_tci Test Case ID
 * ThsyncCondWaitForTimePeriod_S1_N_TC_02_009_011
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_011_rr Related Requirement(s)
 * \#199160
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_011_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_011_tcs Test Case Summary
 * This test shall measure the time execution of R_OSAL_ThsyncCondWaitForTimePeriod with normal condition:
 * - pre-process: create new mutex and condition variable object. Lock the created mutex
 * - Test02_009_011: measure execution time of R_OSAL_ThsyncCondWaitForTimePeriod wit timeout is 0ms
 * - post-process: Unlock mutex, destroy created mutex and condition variable
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_011_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Lock mutex with R_OSAL_ThsyncMutexTryLock
 * -# Use the "for" loop in which we repeat 101 times with 4 steps as below:
 *      - Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *      - Call to R_OSAL_ThsyncCondWaitForTimePeriod with the timeout is 0ms.
 *      - Get the current time stamp again by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *      - Get the execution time of R_OSAL_ThsyncCondWaitForTimePeriod by calculating the different time using R_OSAL_ClockTimeCalculateTimeDifference.<br>
 *        Execution time is stored in an array "diff_time"
 * -# Find the median of "diff_time" array.<br>
 * Compare it with "delta_time" (TEST_MEDIAN_EXPECT)
 * -# Unlock the mutex with R_OSAL_ThsyncMutexUnlock.
 * -# Destroy mutex by using R_OSAL_ThsyncMutexDestroy
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_011_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.<br>
 * "cond_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * "mtx_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_OK
 * -# The expected result of 4 steps in "for" loop are as below:
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 *      - R_OSAL_ThsyncCondWaitForTimePeriod return OSAL_RETURN_TIME.
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 *      - R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK.
 * -# Getting the median of "diff_time" array successfully.<br>
 * Median value need to be not exceed "delta_time"
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_011_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondWaitForTimePeriod_TC_02_009_011
 * @snippet test_common.hpp findMedian
 */
// [ThsyncCondWaitForTimePeriod_TC_02_009_011]
TEST_F(ThreadSyncCondApiTest, ThsyncCondWaitForTimePeriod_TC_02_009_011)
{
    osal_cond_id_t cond_id;
    osal_mutex_id_t mutex_id;
    osal_milli_sec_t time;
    st_osal_time_t current_time_1st, current_time_2nd;
    osal_nano_sec_t diff_time[101];
    osal_nano_sec_t delta_time = TEST_MEDIAN_EXPECT;
    osal_nano_sec_t median = 0;
    int elements = 0;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

// pre-process: create new cond and mutex
    cond_id = TEST_COND_ID1;
    ASSERT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(cond_handle, NULL_PTR) << "pre-process failed";
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexTryLock(mtx_handle), OSAL_RETURN_OK) << "pre-process failed";

// Test02_009_011
// @brief: timeout is 0
    time = 0;
    for(int i = 0; i <= 100; i++)
    {
        while (OSAL_RETURN_OK == osal_ret)
        {
            EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_1st), OSAL_RETURN_OK);
            osal_ret = R_OSAL_ThsyncCondWaitForTimePeriod(cond_handle, mtx_handle, time);
            EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_2nd), OSAL_RETURN_OK);
            EXPECT_TRUE((OSAL_RETURN_TIME == osal_ret || OSAL_RETURN_OK == osal_ret)); // work-around for spurious wakeup on Windows SIL
        }
        osal_ret = OSAL_RETURN_OK;
        EXPECT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &diff_time[i]), OSAL_RETURN_OK);
    }
    elements = sizeof(diff_time) / sizeof(diff_time[0]);
    median = (osal_nano_sec_t)findMedian(diff_time, elements);
    delta_time = TEST_MEDIAN_EXPECT;
    EXPECT_LE(median, delta_time) << "Test02_009_011 failed";

// post-process: destroy created cond and mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    ASSERT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK) << "post-process failed";
    cond_handle = OSAL_COND_HANDLE_INVALID;
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncCondWaitForTimePeriod_TC_02_009_011]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondWaitForTimePeriod_TC_02_009_012 ThreadSyncCondApiTest.ThsyncCondWaitForTimePeriod_TC_02_009_012
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_012_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_012_tci Test Case ID
 * ThsyncCondWaitForTimePeriod_S1_N_TC_02_009_012
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_012_rr Related Requirement(s)
 * \#199160
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_012_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_012_tcs Test Case Summary
 * This test shall measure the time execution of R_OSAL_ThsyncCondWaitForTimePeriod with normal condition:
 * - pre-process: create new mutex and condition variable object. Lock the created mutex
 * - Test02_009_012: measure execution time of R_OSAL_ThsyncCondWaitForTimePeriod wit timeout is 10ms
 * - post-process: Unlock mutex, destroy created mutex and condition variable
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_012_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Lock mutex with R_OSAL_ThsyncMutexTryLock
 * -# Use the "for" loop in which we repeat 101 times with 4 steps as below:
 *      - Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *      - Call to R_OSAL_ThsyncCondWaitForTimePeriod with the timeout is 10ms.
 *      - Get the current time stamp again by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *      - Get the execution time of R_OSAL_ThsyncCondWaitForTimePeriod by calculating the different time using R_OSAL_ClockTimeCalculateTimeDifference.<br>
 *        Execution time is stored in an array "diff_time"
 * -# Find the median of "diff_time" array.<br>
 * Compare it with "delta_time" (TEST_MEDIAN_EXPECT) plus 10ms
 * -# Unlock the mutex with R_OSAL_ThsyncMutexUnlock.
 * -# Destroy mutex by using R_OSAL_ThsyncMutexDestroy
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_012_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.<br>
 * "cond_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * "mtx_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_OK
 * -# The expected result of 4 steps in "for" loop are as below:
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 *      - R_OSAL_ThsyncCondWaitForTimePeriod return OSAL_RETURN_TIME.
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 *      - R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK.
 * -# Getting the median of "diff_time" array successfully.<br>
 * Median value need to be not exceed "delta_time" plus 10ms
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK
 * @subsection ThsyncCondWaitForTimePeriod_TC_02_009_012_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondWaitForTimePeriod_TC_02_009_012
 * @snippet test_common.hpp findMedian
 */
// [ThsyncCondWaitForTimePeriod_TC_02_009_012]
TEST_F(ThreadSyncCondApiTest, ThsyncCondWaitForTimePeriod_TC_02_009_012)
{
    osal_cond_id_t cond_id;
    osal_mutex_id_t mutex_id;
    osal_milli_sec_t time;
    st_osal_time_t current_time_1st, current_time_2nd;
    osal_nano_sec_t diff_time[101];
    osal_nano_sec_t delta_time = TEST_MEDIAN_EXPECT;
    osal_nano_sec_t median = 0;
    int elements = 0;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

// pre-process: create new cond and mutex
    cond_id = TEST_COND_ID1;
    ASSERT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(cond_handle, NULL_PTR) << "pre-process failed";
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexTryLock(mtx_handle), OSAL_RETURN_OK) << "pre-process failed";

// Test02_009_012
// @brief: timeout is 10
    time = 10;
    for(int i = 0; i <= 100; i++)
    {
        while (OSAL_RETURN_OK == osal_ret)
        {
            EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_1st), OSAL_RETURN_OK);
            osal_ret = R_OSAL_ThsyncCondWaitForTimePeriod(cond_handle, mtx_handle, time);
            EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_2nd), OSAL_RETURN_OK);
            EXPECT_TRUE((OSAL_RETURN_TIME == osal_ret || OSAL_RETURN_OK == osal_ret)); // work-around for spurious wakeup on Windows SIL
        }
        osal_ret = OSAL_RETURN_OK;
        EXPECT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &diff_time[i]), OSAL_RETURN_OK);
    }

    elements = sizeof(diff_time) / sizeof(diff_time[0]);
    median = (osal_nano_sec_t)findMedian(diff_time, elements);
    delta_time = TEST_MEDIAN_EXPECT;
    EXPECT_LE(median, time * MSEC_TO_NSEC + delta_time) << "Test02_009_012 failed";

// post-process: destroy created cond and mutex
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    ASSERT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK) << "post-process failed";
    cond_handle = OSAL_COND_HANDLE_INVALID;
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncCondWaitForTimePeriod_TC_02_009_012]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_001 ThreadSyncCondApiTest.ThsyncCondWaitUntilTimeStamp_TC_02_010_001
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_001_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_001_tci Test Case ID
 * ThsyncCondWaitUntilTimeStamp_S1_N_TC_02_010_001
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_001_rr Related Requirement(s)
 * \#199161
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_001_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_001_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to wait on a condition variable with timeout until time stamp with normal condition:
 * - pre-process: create new mutex and condition variable object
 * - Test02_010_001: all input/output param is valid.
 * - post-process: destroy created mutex and condition variable object
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_001_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Create first thread using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is condstampwait_workerfunction function with functionality:
 *              - Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 *              - Wait on a condition variable by using R_OSAL_ThsyncCondWaitUntilTimeStamp.
 *              - Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 *          -# userarg includes condition variable handle, mutex thread and time stamp
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Address of first thread handle.
 * -# Create second thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is condstampwait_wakeup_workerfunction function with functionality:
 *              - Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 *              - Unblock thread that are blocked on the condition variable by using R_OSAL_ThsyncCondSignal.
 *              - Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 *          -# userarg includes condition variable handle, mutex thread and time stamp
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2.
 *      - Address of second thread handle.
 * -# Join first thread by using R_OSAL_ThreadJoin.
 * -# Join second thread by using R_OSAL_ThreadJoin.
 * -# Destroy mutex by using R_OSAL_ThsyncMutexDestroy
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_001_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.<br>
 * "cond_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * "mtx_handle" is not NULL_PTR.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with first thread.<br>
 * "thread_handle" is not NULL_PTR.
 * In condstampwait_workerfunction function:<br>
 *  - R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncCondWaitUntilTimeStamp return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with second thread.<br>
 * "thread_handle1" is not NULL_PTR.
 * In condstampwait_wakeup_workerfunction function:<br>
 *  - R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncCondSignal return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with first thread.<br>
 * Value of "result" is equal to 0.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with second thread.<br>
 * Value of "result" is equal to 0.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_001_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondWaitUntilTimeStamp_TC_02_010_001
 * @snippet test_cond_var.cpp condstampwait_workerfunction
 * @snippet test_cond_var.cpp condstampwait_wakeup_workerfunction
 */
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_001]
TEST_F(ThreadSyncCondApiTest, ThsyncCondWaitUntilTimeStamp_TC_02_010_001)
{
    osal_cond_id_t cond_id;
    osal_mutex_id_t mutex_id;
    osal_thread_id_t uid;
    st_osal_thread_config_t config;
    int64_t result;
    st_osal_time_t abs_time;
    osal_milli_sec_t time_period = 2000;
    int32_t receive_msg = 0;

// pre-process: Create mutex and cond
    cond_id = TEST_COND_ID1;
    ASSERT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(cond_handle, NULL_PTR) << "pre-process failed";
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_010_001
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
    condstamp_worker_arg user_arg(&cond_handle, &mtx_handle, &abs_time);
    uid              = TEST_THREAD_ID1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = (char *)"Simple testing thread";
    config.func      = condstampwait_workerfunction;
    config.userarg   = &user_arg;
    config.stack_size = THREAD_STACK_SIZE;
    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK);
    ASSERT_NE(thread_handle, NULL_PTR);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(mq_handle, time_period, &receive_msg, sizeof(receive_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

    uid              = TEST_THREAD_ID2;
    config.func      = condstampwait_wakeup_workerfunction;
    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle1), OSAL_RETURN_OK);
    ASSERT_NE(thread_handle1, NULL_PTR);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK);
    EXPECT_EQ(result, 0) << "Test02_010_001 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &result), OSAL_RETURN_OK);
    EXPECT_EQ(result, 0) << "Test02_010_001 failed";
    thread_handle = OSAL_THREAD_HANDLE_INVALID;
    thread_handle1 = OSAL_THREAD_HANDLE_INVALID;

// post-process: destroy created mutex and cond
    ASSERT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    ASSERT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK) << "post-process failed";
    cond_handle = OSAL_COND_HANDLE_INVALID;
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_001]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_002 ThreadSyncCondApiTest.ThsyncCondWaitUntilTimeStamp_TC_02_010_002
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_002_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_002_tci Test Case ID
 * ThsyncCondWaitUntilTimeStamp_S1_BN_TC_02_010_002
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_002_rr Related Requirement(s)
 * \#199161
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_002_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_002_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to wait on a condition variable with timeout until time stamp with normal condition:
 * - pre-process: create new mutex and condition variable object
 * - Test02_010_002: checking with "tv_nsec" is 1000000nsec.
 * - post-process: destroy created mutex and condition variable object
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_002_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION and set tv_nsec to 1ms.
 * -# Create first thread using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is condstampwait_workerfunction function with functionality:
 *              - Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 *              - Wait on a condition variable by using R_OSAL_ThsyncCondWaitUntilTimeStamp.
 *              - Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 *          -# userarg includes condition variable handle, mutex thread and time stamp
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Address of first thread handle.
 * -# Create second thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is condstampwait_wakeup_workerfunction function with functionality:
 *              - Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 *              - Unblock thread that are blocked on the condition variable by using R_OSAL_ThsyncCondSignal.
 *              - Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 *          -# userarg includes condition variable handle, mutex thread and time stamp
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2.
 *      - Address of second thread handle.
 * -# Join first thread by using R_OSAL_ThreadJoin.
 * -# Join second thread by using R_OSAL_ThreadJoin.
 * -# Destroy mutex by using R_OSAL_ThsyncMutexDestroy
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_002_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.<br>
 * "cond_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * "mtx_handle" is not NULL_PTR.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with first thread.<br>
 * "thread_handle" is not NULL_PTR.
 * In condstampwait_workerfunction function:<br>
 *  - R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncCondWaitUntilTimeStamp return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with second thread.<br>
 * "thread_handle1" is not NULL_PTR.
 * In condstampwait_wakeup_workerfunction function:<br>
 *  - R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncCondSignal return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with first thread.<br>
 * Value of "result" is equal to 0.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with second thread.<br>
 * Value of "result" is equal to 0.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_002_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondWaitUntilTimeStamp_TC_02_010_002
 * @snippet test_cond_var.cpp condstampwait_workerfunction
 * @snippet test_cond_var.cpp condstampwait_wakeup_workerfunction
 */
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_002]
TEST_F(ThreadSyncCondApiTest, ThsyncCondWaitUntilTimeStamp_TC_02_010_002)
{
    osal_cond_id_t cond_id;
    osal_mutex_id_t mutex_id;
    osal_thread_id_t uid;
    st_osal_thread_config_t config;
    int64_t result;
    st_osal_time_t abs_time;
    osal_milli_sec_t time_period = 2000;
    int32_t receive_msg = 0;

// pre-process: Create mutex and cond
    cond_id = TEST_COND_ID1;
    ASSERT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(cond_handle, NULL_PTR) << "pre-process failed";
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_010_002
// @brief: boundary normal case: checking with tv_nsec is 1ms 
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time), OSAL_RETURN_OK);
    abs_time.tv_sec = abs_time.tv_sec + 1;
    abs_time.tv_nsec = 1 * MSEC_TO_NSEC;
    condstamp_worker_arg user_arg(&cond_handle, &mtx_handle, &abs_time);
    uid              = TEST_THREAD_ID1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = (char *)"Simple testing thread";
    config.func      = condstampwait_workerfunction;
    config.userarg   = &user_arg;
    config.stack_size = THREAD_STACK_SIZE;
    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK);
    ASSERT_NE(thread_handle, NULL_PTR);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(mq_handle, time_period, &receive_msg, sizeof(receive_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

    uid              = TEST_THREAD_ID2;
    config.func      = condstampwait_wakeup_workerfunction;
    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle1), OSAL_RETURN_OK);
    ASSERT_NE(thread_handle1, NULL_PTR);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK);
    EXPECT_EQ(result, 0) << "Test02_010_002 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &result), OSAL_RETURN_OK);
    EXPECT_EQ(result, 0) << "Test02_010_002 failed";
    thread_handle = OSAL_THREAD_HANDLE_INVALID;
    thread_handle1 = OSAL_THREAD_HANDLE_INVALID;

// post-process: destroy created mutex and cond
    ASSERT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    ASSERT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK) << "post-process failed";
    cond_handle = OSAL_COND_HANDLE_INVALID;
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_002]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_003 ThreadSyncCondApiTest.ThsyncCondWaitUntilTimeStamp_TC_02_010_003
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_003_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_003_tci Test Case ID
 * ThsyncCondWaitUntilTimeStamp_S1_BN_TC_02_010_003
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_003_rr Related Requirement(s)
 * \#199161
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_003_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_003_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to wait on a condition variable with timeout until time stamp with normal condition:
 * - pre-process: create new mutex and condition variable object
 * - Test02_010_003: checking with "tv_nsec" is 999999999nsec.
 * - post-process: destroy created mutex and condition variable object
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_003_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION and set tv_nsec to MAX_NSEC. 
 * -# Create first thread using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is condstampwait_workerfunction function with functionality:
 *              - Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 *              - Wait on a condition variable by using R_OSAL_ThsyncCondWaitUntilTimeStamp.
 *              - Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 *          -# userarg includes condition variable handle, mutex thread and time stamp
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Address of first thread handle.
 * -# Create second thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is condstampwait_wakeup_workerfunction function with functionality:
 *              - Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 *              - Unblock thread that are blocked on the condition variable by using R_OSAL_ThsyncCondSignal.
 *              - Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 *          -# userarg includes condition variable handle, mutex thread and time stamp
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2.
 *      - Address of second thread handle.
 * -# Join first thread by using R_OSAL_ThreadJoin.
 * -# Join second thread by using R_OSAL_ThreadJoin.
 * -# Destroy mutex by using R_OSAL_ThsyncMutexDestroy
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_003_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.<br>
 * "cond_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * "mtx_handle" is not NULL_PTR.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with first thread.<br>
 * "thread_handle" is not NULL_PTR.
 * In condstampwait_workerfunction function:<br>
 *  - R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncCondWaitUntilTimeStamp return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with second thread.<br>
 * "thread_handle1" is not NULL_PTR.
 * In condstampwait_wakeup_workerfunction function:<br>
 *  - R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncCondSignal return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with first thread.<br>
 * Value of "result" is equal to 0.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with second thread.<br>
 * Value of "result" is equal to 0.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_003_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondWaitUntilTimeStamp_TC_02_010_003
 * @snippet test_cond_var.cpp condstampwait_workerfunction
 * @snippet test_cond_var.cpp condstampwait_wakeup_workerfunction
 */
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_003]
TEST_F(ThreadSyncCondApiTest, ThsyncCondWaitUntilTimeStamp_TC_02_010_003)
{
    osal_cond_id_t cond_id;
    osal_mutex_id_t mutex_id;
    osal_thread_id_t uid;
    st_osal_thread_config_t config;
    int64_t result;
    st_osal_time_t abs_time;
    osal_milli_sec_t time_period = 2000;
    int32_t receive_msg = 0;

// pre-process: Create mutex and cond
    cond_id = TEST_COND_ID1;
    ASSERT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(cond_handle, NULL_PTR) << "pre-process failed";
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_010_003
// @brief: boundary normal case: checking with tv_nsec is 999999999 
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time), OSAL_RETURN_OK);
    abs_time.tv_sec = abs_time.tv_sec + 1;
    abs_time.tv_nsec = MAX_NSEC;
    condstamp_worker_arg user_arg(&cond_handle, &mtx_handle, &abs_time);
    uid              = TEST_THREAD_ID1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = (char *)"Simple testing thread";
    config.func      = condstampwait_workerfunction;
    config.userarg   = &user_arg;
    config.stack_size = THREAD_STACK_SIZE;
    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK);
    ASSERT_NE(thread_handle, NULL_PTR);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(mq_handle, time_period, &receive_msg, sizeof(receive_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

    uid              = TEST_THREAD_ID2;
    config.func      = condstampwait_wakeup_workerfunction;
    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle1), OSAL_RETURN_OK);
    ASSERT_NE(thread_handle1, NULL_PTR);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK);
    EXPECT_EQ(result, 0) << "Test02_010_003 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &result), OSAL_RETURN_OK);
    EXPECT_EQ(result, 0) << "Test02_010_003 failed";
    thread_handle = OSAL_THREAD_HANDLE_INVALID;
    thread_handle1 = OSAL_THREAD_HANDLE_INVALID;

// post-process: destroy created mutex and cond
    ASSERT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    ASSERT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK) << "post-process failed";
    cond_handle = OSAL_COND_HANDLE_INVALID;
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_003]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_004 ThreadSyncCondApiTest.ThsyncCondWaitUntilTimeStamp_TC_02_010_004
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_004_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_004_tci Test Case ID
 * ThsyncCondWaitUntilTimeStamp_S1_N_TC_02_010_004
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_004_rr Related Requirement(s)
 * \#199161
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_004_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_004_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to wait on a condition variable with timeout until time stamp with normal condition:
 * - pre-process: create new mutex and condition variable object
 * - Test02_010_004: checking the round up feature with tv_nsec is less than 1ms.
 * - post-process: destroy created mutex and condition variable object
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_004_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION and set tv_nsec to less than 1ms.
 * -# Create first thread using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is condstampwait_workerfunction function with functionality:
 *              - Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 *              - Wait on a condition variable by using R_OSAL_ThsyncCondWaitUntilTimeStamp.
 *              - Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 *          -# userarg includes condition variable handle, mutex thread and time stamp
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Address of first thread handle.
 * -# Create second thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is condstampwait_wakeup_workerfunction function with functionality:
 *              - Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 *              - Unblock thread that are blocked on the condition variable by using R_OSAL_ThsyncCondSignal.
 *              - Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 *          -# userarg includes condition variable handle, mutex thread and time stamp
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2.
 *      - Address of second thread handle.
 * -# Join first thread by using R_OSAL_ThreadJoin.
 * -# Join second thread by using R_OSAL_ThreadJoin.
 * -# Destroy mutex by using R_OSAL_ThsyncMutexDestroy
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_004_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.<br>
 * "cond_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * "mtx_handle" is not NULL_PTR.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with first thread.<br>
 * "thread_handle" is not NULL_PTR.
 * In condstampwait_workerfunction function:<br>
 *  - R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncCondWaitUntilTimeStamp return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with second thread.<br>
 * "thread_handle1" is not NULL_PTR.
 * In condstampwait_wakeup_workerfunction function:<br>
 *  - R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncCondSignal return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with first thread.<br>
 * Value of "result" is equal to 0.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with second thread.<br>
 * Value of "result" is equal to 0.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_004_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondWaitUntilTimeStamp_TC_02_010_004
 * @snippet test_cond_var.cpp condstampwait_workerfunction
 * @snippet test_cond_var.cpp condstampwait_wakeup_workerfunction
 */
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_004]
TEST_F(ThreadSyncCondApiTest, ThsyncCondWaitUntilTimeStamp_TC_02_010_004)
{
    osal_cond_id_t cond_id;
    osal_mutex_id_t mutex_id;
    osal_thread_id_t uid;
    st_osal_thread_config_t config;
    int64_t result;
    st_osal_time_t abs_time;
    osal_milli_sec_t time_period = 2000;
    int32_t receive_msg = 0;

// pre-process: Create mutex and cond
    cond_id = TEST_COND_ID1;
    ASSERT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(cond_handle, NULL_PTR) << "pre-process failed";
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_010_004
// @brief: normal case: checking the round up feature with tv_nsec is less than 1ms
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time), OSAL_RETURN_OK);
    abs_time.tv_sec = abs_time.tv_sec + 5;
    abs_time.tv_nsec = 111111;
    condstamp_worker_arg user_arg(&cond_handle, &mtx_handle, &abs_time);
    uid              = TEST_THREAD_ID1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = (char *)"Simple testing thread";
    config.func      = condstampwait_workerfunction;
    config.userarg   = &user_arg;
    config.stack_size = THREAD_STACK_SIZE;
    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK);
    ASSERT_NE(thread_handle, NULL_PTR);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(mq_handle, time_period, &receive_msg, sizeof(receive_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

    uid              = TEST_THREAD_ID2;
    config.func      = condstampwait_wakeup_workerfunction;
    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle1), OSAL_RETURN_OK);
    ASSERT_NE(thread_handle1, NULL_PTR);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK);
    EXPECT_EQ(result, 0) << "Test02_010_004 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &result), OSAL_RETURN_OK);
    EXPECT_EQ(result, 0) << "Test02_010_004 failed";
    thread_handle = OSAL_THREAD_HANDLE_INVALID;
    thread_handle1 = OSAL_THREAD_HANDLE_INVALID;

// post-process: destroy created mutex and cond
    ASSERT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    ASSERT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK) << "post-process failed";
    cond_handle = OSAL_COND_HANDLE_INVALID;
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_004]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_005 ThreadSyncCondApiTest.ThsyncCondWaitUntilTimeStamp_TC_02_010_005
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_005_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_005_tci Test Case ID
 * ThsyncCondWaitUntilTimeStamp_S0_N_TC_02_010_005
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_005_rr Related Requirement(s)
 * \#199161
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_005_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_005_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to wait on a condition variable with timeout until time stamp with normal condition:
 * - pre-process: create new mutex and condition variable object
 * - Test02_010_005: checking with pass time stamp.
 * - post-process: destroy created mutex and condition variable object
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_005_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# In case the "tv_sec" of the current time is less than 1s:
 *  - Suspend thread 2000ms with R_OSAL_ThreadSleepForTimePeriod.
 *  - Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION again.
 * -# Create new thread using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is condstampwait_passedtime_workerfunction function with functionality:
 *              - Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 *              - Wait on a condition variable by using R_OSAL_ThsyncCondWaitUntilTimeStamp.
 *              - Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 *          -# userarg includes condition variable handle, mutex thread and time stamp
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Address of first thread handle.
 * -# Join "condstampwait_passedtime_workerfunction" thread by using R_OSAL_ThreadJoin.
 * -# Destroy mutex by using R_OSAL_ThsyncMutexDestroy
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_005_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.<br>
 * "cond_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * "mtx_handle" is not NULL_PTR.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# In case the "tv_sec" of the current time is less than 1s:<br>
 * R_OSAL_ThreadSleepForTimePeriod return OSAL_RETURN_OK.
 * R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK.<br>
 * "thread_handle" is not NULL_PTR.
 * In condstampwait_passedtime_workerfunction function:<br>
 *  - R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncCondWaitUntilTimeStamp return OSAL_RETURN_TIME.
 *  - R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK.<br>
 * Value of "result" is equal to 0.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_005_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondWaitUntilTimeStamp_TC_02_010_005
 * @snippet test_cond_var.cpp condstampwait_passedtime_workerfunction
 */
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_005]
TEST_F(ThreadSyncCondApiTest, ThsyncCondWaitUntilTimeStamp_TC_02_010_005)
{
    osal_cond_id_t cond_id;
    osal_mutex_id_t mutex_id;
    osal_thread_id_t uid;
    st_osal_thread_config_t config;
    int64_t result;
    st_osal_time_t abs_time;
    osal_milli_sec_t time;

// pre-process: Create mutex and cond
    cond_id = TEST_COND_ID1;
    ASSERT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(cond_handle, NULL_PTR) << "pre-process failed";
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_010_005
// @brief: normal case: checking with pass time stamp
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time), OSAL_RETURN_OK);
    if (abs_time.tv_sec < 1)
    {
        time = 2000;
        ASSERT_EQ(R_OSAL_ThreadSleepForTimePeriod(time), OSAL_RETURN_OK);
        ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time), OSAL_RETURN_OK);
    }
    abs_time.tv_sec = abs_time.tv_sec - 1;
    condstamp_worker_arg user_arg(&cond_handle, &mtx_handle, &abs_time);
    uid              = TEST_THREAD_ID1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = (char *)"Simple testing thread";
    config.func      = condstampwait_passedtime_workerfunction;
    config.userarg   = &user_arg;
    config.stack_size = THREAD_STACK_SIZE;
    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK);
    ASSERT_NE(thread_handle, NULL_PTR);

    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK);
    EXPECT_EQ(result, 0) << "Test02_010_005 failed";
    thread_handle = OSAL_THREAD_HANDLE_INVALID;

// post-process: destroy created mutex and cond
    ASSERT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    ASSERT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK) << "post-process failed";
    cond_handle = OSAL_COND_HANDLE_INVALID;
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_005]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_006 ThreadSyncCondApiTest.ThsyncCondWaitUntilTimeStamp_TC_02_010_006
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_006_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_006_tci Test Case ID
 * ThsyncCondWaitUntilTimeStamp_S0_A_TC_02_010_006
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_006_rr Related Requirement(s)
 * \#199149, \#199151, \#199161
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_006_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_006_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to wait on a condition variable with timeout until time stamp with abnormal condition:
 * - pre-process: create new mutex
 * - Test02_010_006: "cond_handle" is NULL.
 * - post-process: destroy created mutex object
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_006_tpts Test Procedure/Test Steps
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Lock mutex with R_OSAL_ThsyncMutexTryLock.
 * -# Wait on a condition variable by using R_OSAL_ThsyncCondWaitUntilTimeStamp with the argument as below:
 *      - "cond_handle" is NULL.
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_006_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * "mtx_handle" is not NULL_PTR.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondWaitUntilTimeStamp return OSAL_RETURN_HANDLE.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_006_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondWaitUntilTimeStamp_TC_02_010_006
 */
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_006]
TEST_F(ThreadSyncCondApiTest, ThsyncCondWaitUntilTimeStamp_TC_02_010_006)
{
    osal_mutex_id_t mutex_id;
    st_osal_time_t abs_time;

// pre-process: create new mutex
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_010_006
// @brief: abnormal case: cond handler is NULL
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
    ASSERT_EQ(R_OSAL_ThsyncMutexTryLock(mtx_handle), OSAL_RETURN_OK);
    ASSERT_EQ(R_OSAL_ThsyncCondWaitUntilTimeStamp(OSAL_COND_HANDLE_INVALID, mtx_handle, &abs_time), OSAL_RETURN_HANDLE) << "Test02_010_006 failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK);

// post-process: destroy created mutex
    ASSERT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_006]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_007 ThreadSyncCondApiTest.ThsyncCondWaitUntilTimeStamp_TC_02_010_007
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_007_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_007_tci Test Case ID
 * ThsyncCondWaitUntilTimeStamp_S0_A_TC_02_010_007
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_007_rr Related Requirement(s)
 * \#199149, \#199151, \#199161
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_007_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_007_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to wait on a condition variable with timeout until time stamp with abnormal condition:
 * - pre-process: create new mutex and condition variable object
 * - Test02_010_007: "mtx_handle" is NULL.
 * - post-process: destroy created mutex and condition variable object
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_007_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Lock mutex with R_OSAL_ThsyncMutexTryLock.
 * -# Wait on a condition variable by using R_OSAL_ThsyncCondWaitUntilTimeStamp with the argument as below:
 *      - "mtx_handle" is NULL.
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_007_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.<br>
 * "cond_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * "mtx_handle" is not NULL_PTR.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondWaitUntilTimeStamp return OSAL_RETURN_HANDLE.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_007_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondWaitUntilTimeStamp_TC_02_010_007
 */
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_007]
TEST_F(ThreadSyncCondApiTest, ThsyncCondWaitUntilTimeStamp_TC_02_010_007)
{
    osal_cond_id_t cond_id;
    osal_mutex_id_t mutex_id;
    st_osal_time_t abs_time;

// pre-process: create mutex and cond
    cond_id = TEST_COND_ID1;
    ASSERT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(cond_handle, NULL_PTR) << "pre-process failed";
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_010_007
// @brief: abnormal case: Mutex handler is NULL
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
    ASSERT_EQ(R_OSAL_ThsyncMutexTryLock(mtx_handle), OSAL_RETURN_OK);
    ASSERT_EQ(R_OSAL_ThsyncCondWaitUntilTimeStamp(cond_handle, OSAL_MUTEX_HANDLE_INVALID, &abs_time), OSAL_RETURN_HANDLE) << "Test02_010_007 failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK);

// post-process: destroy created mutex and cond_handle
    ASSERT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK) << "post-process failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    cond_handle = OSAL_COND_HANDLE_INVALID;
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_007]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_008 ThreadSyncCondApiTest.ThsyncCondWaitUntilTimeStamp_TC_02_010_008
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_008_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_008_tci Test Case ID
 * ThsyncCondWaitUntilTimeStamp_S0_A_TC_02_010_008
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_008_rr Related Requirement(s)
 * \#199161
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_008_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_008_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to wait on a condition variable with timeout until time stamp with abnormal condition:
 * - pre-process: create new mutex and condition variable object
 * - Test02_010_008: "p_time_stamp" is NULL.
 * - post-process: destroy created mutex object (condition variable has already been destroyed in previous test)
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_008_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Lock mutex with R_OSAL_ThsyncMutexTryLock.
 * -# Wait on a condition variable by using R_OSAL_ThsyncCondWaitUntilTimeStamp with the argument as below:
 *      - "p_time_stamp" is NULL.
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_008_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.<br>
 * "cond_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * "mtx_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondWaitUntilTimeStamp return OSAL_RETURN_PAR.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_008_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondWaitUntilTimeStamp_TC_02_010_008
 */
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_008]
TEST_F(ThreadSyncCondApiTest, ThsyncCondWaitUntilTimeStamp_TC_02_010_008)
{
    osal_cond_id_t cond_id;
    osal_mutex_id_t mutex_id;
    st_osal_time_t* p_abs_time = NULL;

// pre-process: create mutex and cond
    cond_id = TEST_COND_ID1;
    ASSERT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(cond_handle, NULL_PTR) << "pre-process failed";
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_010_008
// @brief: abnormal case: p_time_stamp is NULL
    ASSERT_EQ(R_OSAL_ThsyncMutexTryLock(mtx_handle), OSAL_RETURN_OK);
    ASSERT_EQ(R_OSAL_ThsyncCondWaitUntilTimeStamp(cond_handle, mtx_handle, p_abs_time), OSAL_RETURN_PAR) << "Test02_010_008 failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK);

// post-process: destroy created mutex and cond_handle
    ASSERT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK) << "post-process failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    cond_handle = OSAL_COND_HANDLE_INVALID;
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_008]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_009 ThreadSyncCondApiTest.ThsyncCondWaitUntilTimeStamp_TC_02_010_009
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_009_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_009_tci Test Case ID
 * ThsyncCondWaitUntilTimeStamp_S0_BA_TC_02_010_009
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_009_rr Related Requirement(s)
 * \#199161
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_009_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_009_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to wait on a condition variable with timeout until time stamp with abnormal condition:
 * - pre-process: create new mutex and condition variable object
 * - Test02_010_009: checking with "tv_nsec" is 1sec.
 * - post-process: destroy created mutex and condition variable object
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_009_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION and set tv_nsec to 1sec.
 * -# Lock mutex with R_OSAL_ThsyncMutexTryLock.
 * -# Wait on a condition variable by using R_OSAL_ThsyncCondWaitUntilTimeStamp with "tv_nsec" is 1000000000nsec.
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_009_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.<br>
 * "cond_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * "mtx_handle" is not NULL_PTR.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondWaitUntilTimeStamp return OSAL_RETURN_PAR.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_009_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondWaitUntilTimeStamp_TC_02_010_009
 */
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_009]
TEST_F(ThreadSyncCondApiTest, ThsyncCondWaitUntilTimeStamp_TC_02_010_009)
{
    osal_cond_id_t cond_id;
    osal_mutex_id_t mutex_id;
    st_osal_time_t abs_time;

// pre-process: create mutex and cond
    cond_id = TEST_COND_ID1;
    ASSERT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(cond_handle, NULL_PTR) << "pre-process failed";
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_010_009
// @brief: boundary abnormal case: tv_nsec is 1sec
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time), OSAL_RETURN_OK);
    abs_time.tv_sec += 1;
    abs_time.tv_nsec = 1 * SEC_TO_NSEC;
    ASSERT_EQ(R_OSAL_ThsyncMutexTryLock(mtx_handle), OSAL_RETURN_OK);
    ASSERT_EQ(R_OSAL_ThsyncCondWaitUntilTimeStamp(cond_handle, mtx_handle, &abs_time), OSAL_RETURN_PAR) << "Test02_010_009 failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK);

// post-process: destroy created mutex and cond
    ASSERT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK) << "post-process failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    cond_handle = OSAL_COND_HANDLE_INVALID;
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_009]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_010 ThreadSyncCondApiTest.ThsyncCondWaitUntilTimeStamp_TC_02_010_010
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_010_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_010_tci Test Case ID
 * ThsyncCondWaitUntilTimeStamp_S2_A_TC_02_010_010
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_010_rr Related Requirement(s)
 * \#199161
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_010_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_010_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to wait on a condition variable with timeout until time stamp with abnormal condition:
 * - pre-process: create new mutex and condition variable object
 * - Test02_010_010: No singal on the condition
 * - post-process: destroy created mutex object and condition
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_010_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Create a thread using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is condstampwait_timeout_workerfunction function with functionality:
 *              - Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 *              - Wait on a condition variable by using R_OSAL_ThsyncCondWaitUntilTimeStamp.
 *              - Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 *          -# userarg includes condition variable handle, mutex thread and time stamp
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Address of thread handle.
 * -# Join "Simple testing thread" thread by using R_OSAL_ThreadJoin.
 * -# Destroy condition by using R_OSAL_ThsyncCondDestroy
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_010_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.<br>
 * "cond_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * "mtx_handle" is not NULL_PTR.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with first thread.<br>
 * "thread_handle" is not NULL_PTR.
 * In condstampwait_workerfunction function:<br>
 *  - R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncCondWaitUntilTimeStamp return OSAL_RETURN_TIME.
 *  - R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with first thread.<br>
 * Value of "result" is equal to 0.
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_010_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondWaitUntilTimeStamp_TC_02_010_010
 * @snippet test_cond_var.cpp condstampwait_timeout_workerfunction
 */
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_010]
TEST_F(ThreadSyncCondApiTest, ThsyncCondWaitUntilTimeStamp_TC_02_010_010)
{
    osal_cond_id_t cond_id;
    osal_mutex_id_t mutex_id;
    osal_thread_id_t uid;
    st_osal_thread_config_t config;
    int64_t result;
    st_osal_time_t abs_time;

// pre-process: create mutex and cond
    cond_id = TEST_COND_ID1;
    ASSERT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(cond_handle, NULL_PTR) << "pre-process failed";
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_010_010
// @brief: abnormal case: no signal on waiting condition.
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
    condstamp_worker_arg user_arg(&cond_handle, &mtx_handle, &abs_time);
    uid              = TEST_THREAD_ID1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = (char *)"Simple testing thread";
    config.func      = condstampwait_timeout_workerfunction;
    config.userarg   = &user_arg;
    config.stack_size = THREAD_STACK_SIZE;
    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK);
    ASSERT_NE(thread_handle, NULL_PTR);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK);
    EXPECT_EQ(result, 0) << "Test02_010_010 failed";
    thread_handle = OSAL_THREAD_HANDLE_INVALID;

// post-process: destroy created mutex and cond
    ASSERT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK) << "post-process failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    cond_handle = OSAL_COND_HANDLE_INVALID;
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_010]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_011 ThreadSyncCondApiTest.ThsyncCondWaitUntilTimeStamp_TC_02_010_011
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_011_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_011_tci Test Case ID
 * ThsyncCondWaitUntilTimeStamp_S4_A_TC_02_010_011
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_011_rr Related Requirement(s)
 * \#199161
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_011_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_011_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to wait on a condition variable with timeout until time stamp with abnormal condition:
 * - pre-process: create new mutex and condition variable object
 * - Test02_010_011: the mutex is not owned by calling thread.
 * - post-process: destroy created mutex and condition variable object
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_011_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Wait on a condition variable by using R_OSAL_ThsyncCondWaitUntilTimeStamp with the mutex is not owned by calling thread.
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_011_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.<br>
 * "cond_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * "mtx_handle" is not NULL_PTR.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondWaitUntilTimeStamp return OSAL_RETURN_FAIL.
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_011_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondWaitUntilTimeStamp_TC_02_010_011
 */
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_011]
TEST_F(ThreadSyncCondApiTest, ThsyncCondWaitUntilTimeStamp_TC_02_010_011)
{
    osal_cond_id_t cond_id;
    osal_mutex_id_t mutex_id;
    st_osal_time_t abs_time;

// pre-process: create mutex and cond
    cond_id = TEST_COND_ID1;
    ASSERT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(cond_handle, NULL_PTR) << "pre-process failed";
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_010_011
// @brief: abnormal case: the mutex is not owned by calling thread.
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
    ASSERT_EQ(R_OSAL_ThsyncCondWaitUntilTimeStamp(cond_handle, mtx_handle, &abs_time), OSAL_RETURN_FAIL) << "Test02_010_011 failed";

// post-process: destroy created mutex and cond
    ASSERT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK) << "post-process failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    cond_handle = OSAL_COND_HANDLE_INVALID;
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_011]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_013 ThreadSyncCondApiTest.ThsyncCondWaitUntilTimeStamp_TC_02_010_013
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_013_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_013_tci Test Case ID
 * ThsyncCondWaitUntilTimeStamp_S2_A_TC_02_010_013
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_013_rr Related Requirement(s)
 * \#199161
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_013_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_013_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to wait on a condition variable with timeout until time stamp with abnormal condition:
 * - pre-process: create new mutex and condition variable object
 * - Test02_010_013: Cond handle has already been destroyed before the waiting.
 * - post-process: destroy created mutex object (condition variable has already been destroyed)
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_013_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy.
 * -# Lock mutex with R_OSAL_ThsyncMutexTryLock.
 * -# Wait on a condition variable which handle has already been destroyed by using R_OSAL_ThsyncCondWaitUntilTimeStamp.    
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_013_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.<br>
 * "cond_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * "mtx_handle" is not NULL_PTR.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondWaitUntilTimeStamp return OSAL_RETURN_HANDLE.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_013_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondWaitUntilTimeStamp_TC_02_010_013
 */
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_013]
TEST_F(ThreadSyncCondApiTest, ThsyncCondWaitUntilTimeStamp_TC_02_010_013)
{
    osal_cond_id_t cond_id;
    osal_mutex_id_t mutex_id;
    st_osal_time_t abs_time;

// pre-process: create mutex and cond
    cond_id = TEST_COND_ID1;
    ASSERT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(cond_handle, NULL_PTR) << "pre-process failed";
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_010_013
// @brief: abnormal case: Cond handle has already been destroyed before the waiting
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
    ASSERT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK);
    ASSERT_EQ(R_OSAL_ThsyncMutexTryLock(mtx_handle), OSAL_RETURN_OK);
    ASSERT_EQ(R_OSAL_ThsyncCondWaitUntilTimeStamp(cond_handle, mtx_handle, &abs_time), OSAL_RETURN_HANDLE) << "Test02_010_013 failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK);

// post-process: destroy created mutex (cond_handle has already been destroyed)
    ASSERT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    cond_handle = OSAL_COND_HANDLE_INVALID;
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_013]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section OsalApiStateTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_014 OsalApiStateTest.ThsyncCondWaitUntilTimeStamp_TC_02_010_014
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_014_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_014_tci Test Case ID
 * ThsyncCondWaitUntilTimeStamp_S2_A_TC_02_010_014
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_014_rr Related Requirement(s)
 * \#199161
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_014_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * - Current timestamp "abs_time_state" has been got
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_014_tcs Test Case Summary
 * This test shall verify the state transition of ThreadSync Condition APIs with abnormal condition:
 * - Test02_010_014: state transition error of R_OSAL_ThsyncCondWaitUntilTimeStamp
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_014_tpts Test Procedure/Test Steps
 * -# Wait on a condition variable by using R_OSAL_ThsyncCondWaitUntilTimeStamp
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_014_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondWaitUntilTimeStamp return OSAL_RETURN_STATE.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_014_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondWaitUntilTimeStamp_TC_02_010_014
 */
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_014]
TEST_F(OsalApiStateTest, ThsyncCondWaitUntilTimeStamp_TC_02_010_014)
{
    st_osal_time_t abs_time;
    osal_cond_handle_t cond_handle = OSAL_COND_HANDLE_INVALID;
    osal_mutex_handle_t mtx_handle = OSAL_MUTEX_HANDLE_INVALID;

    // Test02_010_014
    // @brief: abnormal case: R_OSAL_ThsyncCondWaitUntilTimeStamp state transition error
    abs_time.tv_sec = abs_time_state.tv_sec + 1;
    abs_time.tv_nsec = abs_time_state.tv_nsec;
    EXPECT_EQ(R_OSAL_ThsyncCondWaitUntilTimeStamp(cond_handle, mtx_handle, &abs_time), OSAL_RETURN_STATE) << "Test02_010_014 failed";
}
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_014]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_015 ThreadSyncCondApiTest.ThsyncCondWaitUntilTimeStamp_TC_02_010_015
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_015_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_015_tci Test Case ID
 * ThsyncCondWaitUntilTimeStamp_S0_A_TC_02_010_015
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_015_rr Related Requirement(s)
 * \#199161
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_015_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_015_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to wait on a condition variable with timeout until time stamp with abnormal condition:
 * - pre-process: create new mutex and condition variable object
 * - Test02_010_015: checking with "tv_sec" is negative number.
 * - post-process: destroy created mutex and condition variable object
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_015_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Lock mutex with R_OSAL_ThsyncMutexTryLock.
 * -# Wait on a condition variable by using R_OSAL_ThsyncCondWaitUntilTimeStamp with "tv_sec" is -1.
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_015_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.<br>
 * "cond_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * "mtx_handle" is not NULL_PTR.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondWaitUntilTimeStamp return OSAL_RETURN_PAR.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_015_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondWaitUntilTimeStamp_TC_02_010_015
 */
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_015]
TEST_F(ThreadSyncCondApiTest, ThsyncCondWaitUntilTimeStamp_TC_02_010_015)
{
    osal_cond_id_t cond_id;
    osal_mutex_id_t mutex_id;
    st_osal_time_t abs_time;

// pre-process: create mutex and cond
    cond_id = TEST_COND_ID1;
    EXPECT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(cond_handle, NULL_PTR) << "pre-process failed";
    mutex_id = TEST_MUTEX_ID1;
    EXPECT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_010_015
// @brief: abnormal case: tv_sec is negative number
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time), OSAL_RETURN_OK) << "Test02_010_015 failed";
    abs_time.tv_sec = -1;
    EXPECT_EQ(R_OSAL_ThsyncMutexTryLock(mtx_handle), OSAL_RETURN_OK) << "Test02_010_015 failed";
    EXPECT_EQ(R_OSAL_ThsyncCondWaitUntilTimeStamp(cond_handle, mtx_handle, &abs_time), OSAL_RETURN_PAR) << "Test02_010_015 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK) << "Test02_010_015 failed";

// post-process: destroy created mutex and cond
    EXPECT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    cond_handle = OSAL_COND_HANDLE_INVALID;
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_015]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_016 ThreadSyncCondApiTest.ThsyncCondWaitUntilTimeStamp_TC_02_010_016
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_016_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_016_tci Test Case ID
 * ThsyncCondWaitUntilTimeStamp_S0_BN_TC_02_010_016
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_016_rr Related Requirement(s)
 * \#199161
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_016_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_016_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to wait on a condition variable with timeout until time stamp:
 * - pre-process: create new mutex and condition variable object
 * - Test02_010_016: checking with tv_sec is equal to zero
 * - post-process: destroy created mutex and condition variable object
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_016_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION and set tv_sec to 0sec.
 * -# Lock mutex with R_OSAL_ThsyncMutexTryLock.
 * -# Wait on a condition variable by using R_OSAL_ThsyncCondWaitUntilTimeStamp with tv_sec is equal to zero
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_016_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.<br>
 * "cond_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * "mtx_handle" is not NULL_PTR.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondWaitUntilTimeStamp return OSAL_RETURN_TIME.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_016_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondWaitUntilTimeStamp_TC_02_010_016
 */
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_016]
TEST_F(ThreadSyncCondApiTest, ThsyncCondWaitUntilTimeStamp_TC_02_010_016)
{
    osal_cond_id_t cond_id;
    osal_mutex_id_t mutex_id;
    st_osal_time_t abs_time;

// pre-process: create mutex and cond
    cond_id = TEST_COND_ID1;
    EXPECT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(cond_handle, NULL_PTR) << "pre-process failed";
    mutex_id = TEST_MUTEX_ID1;
    EXPECT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_010_016
// @brief: boundary normal case: tv_sec is equal to zero
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time), OSAL_RETURN_OK);
    abs_time.tv_sec = 0;
    EXPECT_EQ(R_OSAL_ThsyncMutexTryLock(mtx_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncCondWaitUntilTimeStamp(cond_handle, mtx_handle, &abs_time), OSAL_RETURN_TIME) << "Test02_010_016 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK);

// post-process: destroy created mutex and cond
    EXPECT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    cond_handle = OSAL_COND_HANDLE_INVALID;
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_016]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_017 ThreadSyncCondApiTest.ThsyncCondWaitUntilTimeStamp_TC_02_010_017
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_017_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_017_tci Test Case ID
 * ThsyncCondWaitUntilTimeStamp_S0_BN_TC_02_010_017
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_017_rr Related Requirement(s)
 * \#199161
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_017_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_017_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to wait on a condition variable with timeout until time stamp:
 * - pre-process: create new mutex and condition variable object
 * - Test02_010_017: checking with tv_nsec is equal to zero
 * - post-process: destroy created mutex and condition variable object
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_017_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION and set tv_nsec to 0sec.
 * -# Lock mutex with R_OSAL_ThsyncMutexTryLock.
 * -# Wait on a condition variable by using R_OSAL_ThsyncCondWaitUntilTimeStamp with tv_nsec is equal to zero
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_017_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.<br>
 * "cond_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * "mtx_handle" is not NULL_PTR.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondWaitUntilTimeStamp return OSAL_RETURN_TIME.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_017_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondWaitUntilTimeStamp_TC_02_010_017
 */
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_017]
TEST_F(ThreadSyncCondApiTest, ThsyncCondWaitUntilTimeStamp_TC_02_010_017)
{
    osal_cond_id_t cond_id;
    osal_mutex_id_t mutex_id;
    st_osal_time_t abs_time;

// pre-process: create mutex and cond
    cond_id = TEST_COND_ID1;
    EXPECT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(cond_handle, NULL_PTR) << "pre-process failed";
    mutex_id = TEST_MUTEX_ID1;
    EXPECT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_010_017
// @brief: boundary normal case: tv_nsec is equal to zero
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time), OSAL_RETURN_OK);
    abs_time.tv_nsec = 0;
    EXPECT_EQ(R_OSAL_ThsyncMutexTryLock(mtx_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncCondWaitUntilTimeStamp(cond_handle, mtx_handle, &abs_time), OSAL_RETURN_TIME) << "Test02_010_017 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK);

// post-process: destroy created mutex and cond
    EXPECT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    cond_handle = OSAL_COND_HANDLE_INVALID;
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_017]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_018 ThreadSyncCondApiTest.ThsyncCondWaitUntilTimeStamp_TC_02_010_018
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_018_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_018_tci Test Case ID
 * ThsyncCondWaitUntilTimeStamp_S0_BA_TC_02_010_018
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_018_rr Related Requirement(s)
 * \#199161
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_018_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_018_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to wait on a condition variable with timeout until time stamp:
 * - pre-process: create new mutex and condition variable object
 * - Test02_010_018: checking with tv_nsec is less than zero
 * - post-process: destroy created mutex and condition variable object
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_018_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION and set tv_nsec to -1sec.
 * -# Lock mutex with R_OSAL_ThsyncMutexTryLock.
 * -# Wait on a condition variable by using R_OSAL_ThsyncCondWaitUntilTimeStamp with tv_nsec is less than zero
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_018_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.<br>
 * "cond_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * "mtx_handle" is not NULL_PTR.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondWaitUntilTimeStamp return OSAL_RETURN_PAR.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_018_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondWaitUntilTimeStamp_TC_02_010_018
 */
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_018]
TEST_F(ThreadSyncCondApiTest, ThsyncCondWaitUntilTimeStamp_TC_02_010_018)
{
    osal_cond_id_t cond_id;
    osal_mutex_id_t mutex_id;
    st_osal_time_t abs_time;

// pre-process: create mutex and cond
    cond_id = TEST_COND_ID1;
    EXPECT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(cond_handle, NULL_PTR) << "pre-process failed";
    mutex_id = TEST_MUTEX_ID1;
    EXPECT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_010_018
// @brief: boundary abnormal case: tv_nsec is less than zero
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time), OSAL_RETURN_OK);
    abs_time.tv_nsec = -1;
    EXPECT_EQ(R_OSAL_ThsyncMutexTryLock(mtx_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncCondWaitUntilTimeStamp(cond_handle, mtx_handle, &abs_time), OSAL_RETURN_PAR) << "Test02_010_018 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK);

// post-process: destroy created mutex and cond
    EXPECT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    cond_handle = OSAL_COND_HANDLE_INVALID;
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_018]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondWaitUntilTimeStamp_TC_02_010_019 ThreadSyncCondApiTest.ThsyncCondWaitUntilTimeStamp_TC_02_010_019
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_019_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_019_tci Test Case ID
 * ThsyncCondWaitUntilTimeStamp_S0_BN_TC_02_010_019
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_019_rr Related Requirement(s)
 * \#199161
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_019_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_019_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to wait on a condition variable with timeout until time stamp:
 * - pre-process: create new mutex and condition variable object
 * - Test02_010_019: checking with tv_sec and tv_nsec are equal to zero
 * - post-process: destroy created mutex and condition variable object
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_019_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Lock mutex with R_OSAL_ThsyncMutexTryLock.
 * -# Wait on a condition variable by using R_OSAL_ThsyncCondWaitUntilTimeStamp with tv_sec and tv_nsec are equal to zero
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_019_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.<br>
 * "cond_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * "mtx_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondWaitUntilTimeStamp return OSAL_RETURN_TIME.
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * @subsection ThsyncCondWaitUntilTimeStamp_TC_02_010_019_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondWaitUntilTimeStamp_TC_02_010_019
 */
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_019]
TEST_F(ThreadSyncCondApiTest, ThsyncCondWaitUntilTimeStamp_TC_02_010_019)
{
    osal_cond_id_t cond_id;
    osal_mutex_id_t mutex_id;
    st_osal_time_t abs_time;

// pre-process: create mutex and cond
    cond_id = TEST_COND_ID1;
    EXPECT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(cond_handle, NULL_PTR) << "pre-process failed";
    mutex_id = TEST_MUTEX_ID1;
    EXPECT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_010_019
// @brief: boundary normal case: tv_sec and tv_nsec are equal to zero
    abs_time.tv_sec = 0;
    abs_time.tv_nsec = 0;
    EXPECT_EQ(R_OSAL_ThsyncMutexTryLock(mtx_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncCondWaitUntilTimeStamp(cond_handle, mtx_handle, &abs_time), OSAL_RETURN_TIME) << "Test02_010_019 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(mtx_handle), OSAL_RETURN_OK);

// post-process: destroy created mutex and cond
    EXPECT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    cond_handle = OSAL_COND_HANDLE_INVALID;
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncCondWaitUntilTimeStamp_TC_02_010_019]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondBroadcast_TC_02_011_001 ThreadSyncCondApiTest.ThsyncCondBroadcast_TC_02_011_001
 * @subsection ThsyncCondBroadcast_TC_02_011_001_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondBroadcast_TC_02_011_001_tci Test Case ID
 * ThsyncCondBroadcast_S3_N_TC_02_011_001
 * @subsection ThsyncCondBroadcast_TC_02_011_001_rr Related Requirement(s)
 * \#199162
 * @subsection ThsyncCondBroadcast_TC_02_011_001_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondBroadcast_TC_02_011_001_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to unblock all threads that's waiting on a condition variable with normal condition.
 * - pre-process: create new mutex and condition variable object
 * - Test02_011_001: broadcast on one condition.
 * - post-process: destroy created mutex and condition variable object
 * @subsection ThsyncCondBroadcast_TC_02_011_001_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Create first thread using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is condperiodwait_workerfunction function with functionality:
 *              - Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 *              - Wait on a condition variable by using R_OSAL_ThsyncCondWaitForTimePeriod.
 *              - Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 *          -# userarg includes condition variable handle and mutex handle
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Address of first thread handle.
 * -# Create second thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is cond_wakeup_broadcast_workerfunction function with functionality:
 *              - Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 *              - Unblock all threads blocked on condition variable by using R_OSAL_ThsyncCondBroadcast.
 *              - Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 *          -# userarg includes condition variable handle and mutex handle
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2.
 *      - Address of second thread handle.
 * -# Join first thread by using R_OSAL_ThreadJoin.
 * -# Join second thread by using R_OSAL_ThreadJoin.
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy
 * -# Destroy mutex by using R_OSAL_ThsyncMutexDestroy
 * @subsection ThsyncCondBroadcast_TC_02_011_001_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with first thread.<br>
 * "thread_handle" is not NULL_PTR.<br>
 * In condperiodwait_workerfunction function:
 *  - R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncCondWaitForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with second thread. <br>
 * Value of "thread_handle1" is not NULL.<br>
 * In cond_wakeup_broadcast_workerfunction function:
 *  - R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncCondBroadcast return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with first thread.<br>
 * Value of "result" is equal to 0.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with second thread.<br>
 * Value of "result" is equal to 0.
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK
 * @subsection ThsyncCondBroadcast_TC_02_011_001_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondBroadcast_TC_02_011_001
 * @snippet test_cond_var.cpp condperiodwait_workerfunction
 * @snippet test_cond_var.cpp cond_wakeup_broadcast_workerfunction
 */
// [ThsyncCondBroadcast_TC_02_011_001]
TEST_F(ThreadSyncCondApiTest, ThsyncCondBroadcast_TC_02_011_001)
{
    osal_cond_id_t cond_id;
    osal_mutex_id_t mutex_id;
    osal_thread_id_t uid;
    st_osal_thread_config_t config;
    int64_t result;
    osal_milli_sec_t time_period = 2000;
    int32_t receive_msg = 0;

// pre-process: create mutex and cond
    cond_id = TEST_COND_ID1;
    ASSERT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(cond_handle, NULL_PTR) << "pre-process failed";
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_011_001
// @brief: normal case: broadcast on one condition.
    condperiod_worker_arg user_arg(&cond_handle, &mtx_handle);
    uid              = TEST_THREAD_ID1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = (char *)"Simple testing thread";
    config.func      = condperiodwait_workerfunction;
    config.userarg   = &user_arg;
    config.stack_size = THREAD_STACK_SIZE;
    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK);
    ASSERT_NE(thread_handle, NULL_PTR);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(mq_handle, time_period, &receive_msg, sizeof(receive_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

    uid              = TEST_THREAD_ID2;
    config.func      = cond_wakeup_broadcast_workerfunction;
    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle1), OSAL_RETURN_OK);
    ASSERT_NE(thread_handle1, NULL_PTR);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK);
    EXPECT_EQ(result, 0) << "Test02_011_001 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &result), OSAL_RETURN_OK);
    EXPECT_EQ(result, 0) << "Test02_011_001 failed";
    thread_handle = OSAL_THREAD_HANDLE_INVALID;
    thread_handle1 = OSAL_THREAD_HANDLE_INVALID;

// post-process: destroy created mutex and cond
    ASSERT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK) << "post-process failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    cond_handle = OSAL_COND_HANDLE_INVALID;
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncCondBroadcast_TC_02_011_001]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondBroadcast_TC_02_011_002 ThreadSyncCondApiTest.ThsyncCondBroadcast_TC_02_011_002
 * @subsection ThsyncCondBroadcast_TC_02_011_002_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondBroadcast_TC_02_011_002_tci Test Case ID
 * ThsyncCondBroadcast_S3_A_TC_02_011_002
 * @subsection ThsyncCondBroadcast_TC_02_011_002_rr Related Requirement(s)
 * \#199162
 * @subsection ThsyncCondBroadcast_TC_02_011_002_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondBroadcast_TC_02_011_002_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to unblock all threads that's waiting on a condition variable with normal condition.
 * - pre-process: create new mutex and condition variable object
 * - Test02_011_002: broadcast on 3 condition.
 * - post-process: destroy created mutex and condition variable
 * @subsection ThsyncCondBroadcast_TC_02_011_002_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Create first thread using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is condperiodwait_workerfunction function with functionality:
 *              - Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 *              - Wait on a condition variable by using R_OSAL_ThsyncCondWaitForTimePeriod.
 *              - Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 *          -# userarg includes condition variable handle and mutex handle
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Address of first thread handle.
 * -# Create second thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is condperiodwait_workerfunction function with functionality:
 *              - Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 *              - Wait on a condition variable by using R_OSAL_ThsyncCondWaitForTimePeriod.
 *              - Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 *          -# userarg includes condition variable handle and mutex handle
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2.
 *      - Address of second thread handle.
 * -# Create third thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is condperiodwait_workerfunction function with functionality:
 *              - Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 *              - Wait on a condition variable by using R_OSAL_ThsyncCondWaitForTimePeriod.
 *              - Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 *          -# userarg includes condition variable handle and mutex handle
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID3.
 *      - Address of third thread handle.
 * -# Create fourth thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is cond_wakeup_broadcast_workerfunction function with functionality:
 *              - Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 *              - Unblock all threads blocked on condition variable by using R_OSAL_ThsyncCondBroadcast.
 *              - Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 *          -# userarg includes condition variable handle and mutex handle
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID4.
 *      - Address of fourth thread handle.
 * -# Join first thread by using R_OSAL_ThreadJoin.
 * -# Join second thread by using R_OSAL_ThreadJoin.
 * -# Join third thread by using R_OSAL_ThreadJoin.
 * -# Join fourth thread by using R_OSAL_ThreadJoin.
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy
 * -# Destroy mutex by using R_OSAL_ThsyncMutexDestroy
 * @subsection ThsyncCondBroadcast_TC_02_011_002_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with first thread.<br>
 * "thread_handle" is not NULL_PTR.<br>
 * In condperiodwait_workerfunction function:
 *  - R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncCondWaitForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with second thread.<br>
 * "thread_handle1" is not NULL_PTR.<br>
 * In condperiodwait_workerfunction function:
 *  - R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncCondWaitForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with third thread.<br>
 * "thread_handle2" is not NULL_PTR.<br>
 * In condperiodwait_workerfunction function:
 *  - R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncCondWaitForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with fourth thread.<br>
 * "thread_handle3" is not NULL_PTR.<br>
 * In cond_wakeup_broadcast_workerfunction function:
 *  - R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncCondBroadcast return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with first thread.<br>
 * Value of "result" is equal to 0.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with second thread.<br>
 * Value of "result" is equal to 0.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with third thread.<br>
 * Value of "result" is equal to 0.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with fourth thread.<br>
 * Value of "result" is equal to 0.
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK
 * @subsection ThsyncCondBroadcast_TC_02_011_002_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondBroadcast_TC_02_011_002
 * @snippet test_cond_var.cpp condperiodwait_workerfunction
 * @snippet test_cond_var.cpp cond_wakeup_broadcast_workerfunction
 */
// [ThsyncCondBroadcast_TC_02_011_002]
TEST_F(ThreadSyncCondApiTest, ThsyncCondBroadcast_TC_02_011_002)
{
    osal_cond_id_t cond_id;
    osal_mutex_id_t mutex_id;
    osal_thread_id_t uid;
    st_osal_thread_config_t config;
    int64_t result;
    osal_milli_sec_t time_period = 2000;
    int32_t receive_msg = 0;

// pre-process: create mutex and cond
    cond_id = TEST_COND_ID1;
    ASSERT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(cond_handle, NULL_PTR) << "pre-process failed";
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_011_002
// @brief: normal case: broadcast on 3 condition.
    condperiod_worker_arg user_arg(&cond_handle, &mtx_handle);
    uid              = TEST_THREAD_ID1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = (char *)"Simple testing thread";
    config.func      = condperiodwait_workerfunction;
    config.userarg   = &user_arg;
    config.stack_size = THREAD_STACK_SIZE;
    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK);
    ASSERT_NE(thread_handle, NULL_PTR);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(mq_handle, time_period, &receive_msg, sizeof(receive_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

    uid              = TEST_THREAD_ID2;
    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle1), OSAL_RETURN_OK);
    ASSERT_NE(thread_handle1, NULL_PTR);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(mq_handle, time_period, &receive_msg, sizeof(receive_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

    uid              = TEST_THREAD_ID3;
    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle2), OSAL_RETURN_OK);
    ASSERT_NE(thread_handle2, NULL_PTR);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(mq_handle, time_period, &receive_msg, sizeof(receive_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

    uid              = TEST_THREAD_ID4;
    config.func      = cond_wakeup_broadcast_workerfunction;
    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle3), OSAL_RETURN_OK);
    ASSERT_NE(thread_handle3, NULL_PTR);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK);
    EXPECT_EQ(result, 0) << "Test02_011_002 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &result), OSAL_RETURN_OK);
    EXPECT_EQ(result, 0) << "Test02_011_002 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle2, &result), OSAL_RETURN_OK);
    EXPECT_EQ(result, 0) << "Test02_011_002 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle3, &result), OSAL_RETURN_OK);
    EXPECT_EQ(result, 0) << "Test02_011_002 failed";
    thread_handle = OSAL_THREAD_HANDLE_INVALID;
    thread_handle1 = OSAL_THREAD_HANDLE_INVALID;
    thread_handle2 = OSAL_THREAD_HANDLE_INVALID;
    thread_handle3 = OSAL_THREAD_HANDLE_INVALID;

// post-process: destroy created mutex and cond
    ASSERT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK) << "post-process failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    cond_handle = OSAL_COND_HANDLE_INVALID;
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncCondBroadcast_TC_02_011_002]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondBroadcast_TC_02_011_003 ThreadSyncCondApiTest.ThsyncCondBroadcast_TC_02_011_003
 * @subsection ThsyncCondBroadcast_TC_02_011_003_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondBroadcast_TC_02_011_003_tci Test Case ID
 * ThsyncCondBroadcast_S0_A_TC_02_011_003
 * @subsection ThsyncCondBroadcast_TC_02_011_003_rr Related Requirement(s)
 * \#199149, \#199151, \#199162
 * @subsection ThsyncCondBroadcast_TC_02_011_003_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondBroadcast_TC_02_011_003_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to unblock all threads that's waiting on a condition variable with abnormal condition.
 * - Test02_011_003: "cond_handle" is NULL.
 * @subsection ThsyncCondBroadcast_TC_02_011_003_tpts Test Procedure/Test Steps
 * -# Unblock all threads blocked on condition variable by using R_OSAL_ThsyncCondBroadcast with the argument as below:
 *      - "cond_handle" is NULL
 * @subsection ThsyncCondBroadcast_TC_02_011_003_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondBroadcast return OSAL_RETURN_HANDLE.
 * @subsection ThsyncCondBroadcast_TC_02_011_003_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondBroadcast_TC_02_011_003
 */
// [ThsyncCondBroadcast_TC_02_011_003]
TEST_F(ThreadSyncCondApiTest, ThsyncCondBroadcast_TC_02_011_003)
{
// Test02_011_003
// @brief: abnormal case: cond handle is NULL
    ASSERT_EQ(R_OSAL_ThsyncCondBroadcast(OSAL_COND_HANDLE_INVALID), OSAL_RETURN_HANDLE) << "Test02_011_003 failed";
}
// [ThsyncCondBroadcast_TC_02_011_003]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondBroadcast_TC_02_011_004 ThreadSyncCondApiTest.ThsyncCondBroadcast_TC_02_011_004
 * @subsection ThsyncCondBroadcast_TC_02_011_004_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondBroadcast_TC_02_011_004_tci Test Case ID
 * ThsyncCondBroadcast_S2_A_TC_02_011_004
 * @subsection ThsyncCondBroadcast_TC_02_011_004_rr Related Requirement(s)
 * \#199162
 * @subsection ThsyncCondBroadcast_TC_02_011_004_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondBroadcast_TC_02_011_004_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to unblock all threads that's waiting on a condition variable with abnormal condition.
 * - pre-process: create new condition variable object
 * - Test02_011_004: "cond_handle" has already been destroyed.
 * @subsection ThsyncCondBroadcast_TC_02_011_004_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy.
 * -# Unblock all threads blocked on condition variable which has already been destroyed by using R_OSAL_ThsyncCondBroadcast.
 * @subsection ThsyncCondBroadcast_TC_02_011_004_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondBroadcast return OSAL_RETURN_HANDLE.
 * @subsection ThsyncCondBroadcast_TC_02_011_004_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondBroadcast_TC_02_011_004
 */
// [ThsyncCondBroadcast_TC_02_011_004]
TEST_F(ThreadSyncCondApiTest, ThsyncCondBroadcast_TC_02_011_004)
{
    osal_cond_id_t cond_id;

// pre-process: create new cond
    cond_id = TEST_COND_ID1;
    ASSERT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(cond_handle, NULL_PTR) << "pre-process failed";

// Test02_011_004
// @brief: abnormal case: cond handle has already been destroyed
    ASSERT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK);
    ASSERT_EQ(R_OSAL_ThsyncCondBroadcast(cond_handle), OSAL_RETURN_HANDLE) << "Test02_011_004 failed";
    cond_handle = OSAL_COND_HANDLE_INVALID;
}
// [ThsyncCondBroadcast_TC_02_011_004]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section OsalApiStateTest_ThsyncCondBroadcast_TC_02_011_005 OsalApiStateTest.ThsyncCondBroadcast_TC_02_011_005
 * @subsection ThsyncCondBroadcast_TC_02_011_005_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection ThsyncCondBroadcast_TC_02_011_005_tci Test Case ID
 * ThsyncCondBroadcast_S2_A_TC_02_011_005
 * @subsection ThsyncCondBroadcast_TC_02_011_005_rr Related Requirement(s)
 * \#199162
 * @subsection ThsyncCondBroadcast_TC_02_011_005_pp Prerequisite/Precondition
  * - R_OSAL_Deinitialize is already called
 * @subsection ThsyncCondBroadcast_TC_02_011_005_tcs Test Case Summary
 * This test shall verify the state transition of ThreadSync Condition APIs with abnormal condition:
 * - Test02_011_005: state transition error of R_OSAL_ThsyncCondBroadcast
 * @subsection ThsyncCondBroadcast_TC_02_011_005_tpts Test Procedure/Test Steps
 * -# Unblock all thread by using R_OSAL_ThsyncCondBroadcast
 * @subsection ThsyncCondBroadcast_TC_02_011_005_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondBroadcast return OSAL_RETURN_STATE.
 * @subsection ThsyncCondBroadcast_TC_02_011_005_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondBroadcast_TC_02_011_005
 */
// [ThsyncCondBroadcast_TC_02_011_005]
TEST_F(OsalApiStateTest, ThsyncCondBroadcast_TC_02_011_005)
{
    osal_cond_handle_t cond_handle = OSAL_COND_HANDLE_INVALID;

    // Test02_011_005
    // @brief: abnormal case: R_OSAL_ThsyncCondBroadcast state transition error
    EXPECT_EQ(R_OSAL_ThsyncCondBroadcast(cond_handle), OSAL_RETURN_STATE) << "Test02_011_005 failed";
}
// [ThsyncCondBroadcast_TC_02_011_005]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section OSALINTTESTS_ThsyncCondBroadcast_TC_02_011_006 OSALINTTESTS.ThsyncCondBroadcast_TC_02_011_006
 * @subsection ThsyncCondBroadcast_TC_02_011_006_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection ThsyncCondBroadcast_TC_02_011_006_tci Test Case ID
 * ThsyncCondBroadcast_S3_N_TC_02_011_006
 * @subsection ThsyncCondBroadcast_TC_02_011_006_rr Related Requirement(s)
 * \#199162
 * @subsection ThsyncCondBroadcast_TC_02_011_006_pp Prerequisite/Precondition
 * -# OSAL is initialized.<br>
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL mutex handle which ID is ISR_MUTEX_ID
 * -# Created an OSAL condition handle which ID is ISR_COND_ID
 * @subsection ThsyncCondBroadcast_TC_02_011_006_tcs Test Case Summary
 * @subsubsection ThsyncCondBroadcast_TC_02_011_006_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Create 2 new threads.
 * - Setup required memory for interrupt generation
 * - Register callack function and enable interrupt
 * @subsubsection ThsyncCondBroadcast_TC_02_011_006_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Join 2 waiting threads
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * @subsubsection ThsyncCondBroadcast_TC_02_011_006_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality of the ThreadSync Condition APIs which are called in the ISR with normal condition:
 * - Disable and clear interrupt status
 * - Test02_012_006: unblock two waiting threads by using R_OSAL_ThsyncCondBroadcast
 * @subsection ThsyncCondBroadcast_TC_02_011_006_tpts Test Procedure/Test Steps
 * @subsubsection ThsyncCondBroadcast_TC_02_011_006_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Create 2 threads by using R_OSAL_ThreadCreate.<br>
 * In the worker functions 2 threads above are blocked on a condition variable.
 * -# Allocate required memory for generate interrupt from hardware and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function by using R_OSAL_InterruptRegisterIsr
 * -# Enable interrupt by using R_OSAL_InterruptEnableIsr
 * -# Setup hardware specific register to generate the interrupt by calling to setup_hw_register
 * @subsubsection ThsyncCondBroadcast_TC_02_011_006_post_tpts Test Procedure/Test Steps of Post-process
 * -# Wait for the 2 threads finish by using R_OSAL_ThreadJoin
 * -# Unregister interrupt by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * @subsubsection ThsyncCondBroadcast_TC_02_011_006_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Lock mutex by using R_OSAL_ThsyncMutexTryLock
 * -# Signal all waiting threads by using R_OSAL_ThsyncCondBroadcast
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock
 * @subsection ThsyncCondBroadcast_TC_02_011_006_ereb Expected Result/Behavior
 * @subsubsection ThsyncCondBroadcast_TC_02_011_006_pre_ereb Expected Result/Behavior of Pre-
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK for 2 threads.
 * -# allocate_setup_data_memory run successfully
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * The value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK
 * -# setup_hw_register run successfully
 * @subsubsection ThsyncCondBroadcast_TC_02_011_006_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK for 2 threads.<br>
 * The value of "result" is equal to 0
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * @subsubsection ThsyncCondBroadcast_TC_02_011_006_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_ThsyncCondBroadcast return OSAL_RETURN_OK
 * -# R_OSAL_ThsyncCondBroadcast return OSAL_RETURN_OK
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK
 * @subsection ThsyncCondBroadcast_TC_02_011_006_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondBroadcast_TC_02_011_006
 * @snippet test_cond_var.cpp isr_test_waiting_thread
 * @snippet test_cond_var.cpp osal_cond_isr_test_TC_02_011_006
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [ThsyncCondBroadcast_TC_02_011_006]
TEST_F(OSALINTTESTS, ThsyncCondBroadcast_TC_02_011_006)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    st_osal_thread_config_t config;
    osal_thread_handle_t thread_handle1 = OSAL_THREAD_HANDLE_INVALID;
    osal_thread_handle_t thread_handle2 = OSAL_THREAD_HANDLE_INVALID;
    int64_t result;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};
    osal_milli_sec_t time_period = 2000;
    int32_t receive_msg = 0;

    arg.cond_handle = cond;
    arg.mtx_handle = mtx;
	arg.mq_handle = isr_mq;
/* pre-process:
 * create 2 new thread.
 * Setup required memory for interrupt generation.
 * Register callack function and enable interrupt */

    /* Create 1st thread */
    config.priority  = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = (char *)"Simple testing thread";
    config.func      = isr_test_waiting_thread;
    config.userarg   = (void*)&arg;
    config.stack_size = THREAD_STACK_SIZE;
    EXPECT_EQ(R_OSAL_MqReset(isr_mq), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThreadCreate(&config, ISR_THREAD_ID_NO1, &thread_handle1), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(thread_handle1, NULL_PTR) << "pre-process failed";

	/* sync with broadcast thread */
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(receive_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(mtx, time_period), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(mtx), OSAL_RETURN_OK);

    /* Create 2nd thread */
    EXPECT_EQ(R_OSAL_MqReset(isr_mq), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThreadCreate(&config, ISR_THREAD_ID_NO2, &thread_handle2), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(thread_handle2, NULL_PTR) << "pre-process failed";

	/* sync with broadcast thread */
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(receive_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(mtx, time_period), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(mtx), OSAL_RETURN_OK);

    allocate_setup_data_memory(&hndl_mmngr, hw_addr, &buffer_obj[0]);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_GT(numOfChannel, 0) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_cond_isr_test_TC_02_011_006, (void *)&arg), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK) << "pre-process failed";
    setup_hw_register(dev_arr, hw_addr);

/* post-process:
 * join 2 waiting threads.
 * Unregister interrupt
 * Deallocate allocated memory*/
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &result), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(result, 0) << "post-process failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle2, &result), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(result, 0) << "post-process failed";
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_cond_isr_test_TC_02_011_006), OSAL_RETURN_OK) << "post-process failed";
    deallocate_data_memory(hndl_mmngr, &buffer_obj[0]);
}
// [ThsyncCondBroadcast_TC_02_011_006]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondSignal_TC_02_012_001 ThreadSyncCondApiTest.ThsyncCondSignal_TC_02_012_001
 * @subsection ThsyncCondSignal_TC_02_012_001_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondSignal_TC_02_012_001_tci Test Case ID
 * ThsyncCondSignal_S3_N_TC_02_012_001
 * @subsection ThsyncCondSignal_TC_02_012_001_rr Related Requirement(s)
 * \#199163
 * @subsection ThsyncCondSignal_TC_02_012_001_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondSignal_TC_02_012_001_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to unblock one thread that's waiting on a condition variable with normal condition:
 * - pre-process: create new mutex and condition variable object
 * - Test02_012_001: signal on one condition.
 * - post-proces: destroy created mutex and condition variable
 * @subsection ThsyncCondSignal_TC_02_012_001_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Create a new mutex using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Create first thread using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is condperiodwait_workerfunction function with functionality:
 *              - Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 *              - Wait on a condition variable by using R_OSAL_ThsyncCondWaitForTimePeriod.
 *              - Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 *          -# userarg includes condition variable and mutex handle
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Address of first thread handle.
 * -# Create second thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is condstampwait_wakeup_workerfunction function with functionality:
 *              - Lock mutex with R_OSAL_ThsyncMutexLockForTimePeriod.
 *              - Unblock a thread blocked on condition variable by using R_OSAL_ThsyncCondSignal.
 *              - Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 *          -# userarg includes condition variable and mutex handle
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2.
 *      - Address of second thread handle.
 * -# Join first thread by using R_OSAL_ThreadJoin.
 * -# Join second thread by using R_OSAL_ThreadJoin.
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy
 * -# Destroy mutex by using R_OSAL_ThsyncMutexDestroy
 * @subsection ThsyncCondSignal_TC_02_012_001_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with first thread.<br>
 * "thread_handle" is not NULL_PTR.<br>
 * In condperiodwait_workerfunction function:
 *  - R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncCondWaitForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with second thread.<br>
 * "thread_handle1" is not NULL_PTR. <br>
 * In condstampwait_wakeup_workerfunction function:
 *  - R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncCondSignal return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with first thread.<br>
 * Value of "result" is equal to 0.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with second thread.<br>
 * Value of "result" is equal to 0.
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK
 * @subsection ThsyncCondSignal_TC_02_012_001_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondSignal_TC_02_012_001
 * @snippet test_cond_var.cpp condperiodwait_workerfunction
 * @snippet test_cond_var.cpp condstampwait_wakeup_workerfunction
 */
// [ThsyncCondSignal_TC_02_012_001]
TEST_F(ThreadSyncCondApiTest, ThsyncCondSignal_TC_02_012_001)
{
    osal_cond_id_t cond_id;
    osal_mutex_id_t mutex_id;
    osal_thread_id_t uid;
    st_osal_thread_config_t config;
    int64_t result;
    osal_milli_sec_t time_period = 2000;
    int32_t receive_msg = 0;

// pre-process: create mutex and cond
    cond_id = TEST_COND_ID1;
    ASSERT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(cond_handle, NULL_PTR) << "pre-process failed";
    mutex_id = TEST_MUTEX_ID1;
    ASSERT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test02_012_001
// @brief: normal case: signal on one condition.
    condperiod_worker_arg user_arg(&cond_handle, &mtx_handle);
    uid              = TEST_THREAD_ID1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = (char *)"Simple testing thread";
    config.func      = condperiodwait_workerfunction;
    config.userarg   = &user_arg;
    config.stack_size = THREAD_STACK_SIZE;
    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK);
    ASSERT_NE(thread_handle, NULL_PTR);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(mq_handle, time_period, &receive_msg, sizeof(receive_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

    uid              = TEST_THREAD_ID2;
    config.func      = condstampwait_wakeup_workerfunction;
    ASSERT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle1), OSAL_RETURN_OK);
    ASSERT_NE(thread_handle1, NULL_PTR);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK);
    EXPECT_EQ(result, 0) << "Test02_012_001 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &result), OSAL_RETURN_OK);
    EXPECT_EQ(result, 0) << "Test02_012_001 failed";
    thread_handle = OSAL_THREAD_HANDLE_INVALID;
    thread_handle1 = OSAL_THREAD_HANDLE_INVALID;

// post-process: destroy created mutex and cond
    ASSERT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK) << "post-process failed";
    ASSERT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    cond_handle = OSAL_COND_HANDLE_INVALID;
    mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
}
// [ThsyncCondSignal_TC_02_012_001]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondSignal_TC_02_012_002 ThreadSyncCondApiTest.ThsyncCondSignal_TC_02_012_002
 * @subsection ThsyncCondSignal_TC_02_012_002_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondSignal_TC_02_012_002_tci Test Case ID
 * ThsyncCondSignal_S0_A_TC_02_012_002
 * @subsection ThsyncCondSignal_TC_02_012_002_rr Related Requirement(s)
 * \#199149, \#199151, \#199163
 * @subsection ThsyncCondSignal_TC_02_012_002_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondSignal_TC_02_012_002_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to unblock one thread that's waiting on a condition variable with abnormal condition:
 * - Test02_012_002: "cond_handle" is NULL.
 * @subsection ThsyncCondSignal_TC_02_012_002_tpts Test Procedure/Test Steps
 * -# Unblock a thread blocked on condition variable by using R_OSAL_ThsyncCondSignal  with the argument as below:
 *      - "cond_handle" is NULL
 * @subsection ThsyncCondSignal_TC_02_012_002_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondSignal return OSAL_RETURN_HANDLE.
 * @subsection ThsyncCondSignal_TC_02_012_002_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondSignal_TC_02_012_002
 */
// [ThsyncCondSignal_TC_02_012_002]
TEST_F(ThreadSyncCondApiTest, ThsyncCondSignal_TC_02_012_002)
{
// Test02_012_002
// @brief: abnormal case: cond handle is NULL
    ASSERT_EQ(R_OSAL_ThsyncCondSignal(OSAL_COND_HANDLE_INVALID), OSAL_RETURN_HANDLE) << "Test02_012_002 failed";
}
// [ThsyncCondSignal_TC_02_012_002]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section ThreadSyncCondApiTest_ThsyncCondSignal_TC_02_012_003 ThreadSyncCondApiTest.ThsyncCondSignal_TC_02_012_003
 * @subsection ThsyncCondSignal_TC_02_012_003_tsi Test Suite ID
 * ThreadSyncCondApiTest
 * @subsection ThsyncCondSignal_TC_02_012_003_tci Test Case ID
 * ThsyncCondSignal_S2_A_TC_02_012_003
 * @subsection ThsyncCondSignal_TC_02_012_003_rr Related Requirement(s)
 * \#199163
 * @subsection ThsyncCondSignal_TC_02_012_003_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncCondSignal_TC_02_012_003_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to unblock one thread that's waiting on a condition variable with abnormal condition:
 * - pre-process: create new condition variable object
 * - Test02_012_003: "cond_handle" has already been destroyed.
 * @subsection ThsyncCondSignal_TC_02_012_003_tpts Test Procedure/Test Steps
 * -# Create a new condition variable by using R_OSAL_ThsyncCondCreate with the argument as below:
 *      - cond_id is TEST_COND_ID1
 *      - Address of "cond_handle"
 * -# Destroy condition variable by using R_OSAL_ThsyncCondDestroy.
 * -# Unblock a thread blocked on condition variable which has already been destroyed by using R_OSAL_ThsyncCondSignal.
 * @subsection ThsyncCondSignal_TC_02_012_003_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondCreate return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondDestroy return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncCondSignal return OSAL_RETURN_HANDLE.
 * @subsection ThsyncCondSignal_TC_02_012_003_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondSignal_TC_02_012_003
 */
// [ThsyncCondSignal_TC_02_012_003]
TEST_F(ThreadSyncCondApiTest, ThsyncCondSignal_TC_02_012_003)
{
    osal_cond_id_t cond_id;

// pre-process: create new cond
    cond_id = TEST_COND_ID1;
    ASSERT_EQ(R_OSAL_ThsyncCondCreate(cond_id, &cond_handle), OSAL_RETURN_OK) << "pre-process failed";
    ASSERT_NE(cond_handle, NULL_PTR) << "pre-process failed";

// Test02_012_003
// @brief: abnormal case: cond handle has already been destroyed
    ASSERT_EQ(R_OSAL_ThsyncCondDestroy(cond_handle), OSAL_RETURN_OK);
    ASSERT_EQ(R_OSAL_ThsyncCondSignal(cond_handle), OSAL_RETURN_HANDLE) << "Test02_012_003 failed";
    cond_handle = OSAL_COND_HANDLE_INVALID;
}
// [ThsyncCondSignal_TC_02_012_003]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section OsalApiStateTest_ThsyncCondSignal_TC_02_012_004 OsalApiStateTest.ThsyncCondSignal_TC_02_012_004
 * @subsection ThsyncCondSignal_TC_02_012_004_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection ThsyncCondSignal_TC_02_012_004_tci Test Case ID
 * ThsyncCondSignal_S2_A_TC_02_012_004
 * @subsection ThsyncCondSignal_TC_02_012_004_rr Related Requirement(s)
 * \#199163
 * @subsection ThsyncCondSignal_TC_02_012_004_pp Prerequisite/Precondition
  * - R_OSAL_Deinitialize is already called
 * @subsection ThsyncCondSignal_TC_02_012_004_tcs Test Case Summary
 * This test shall verify the state transition of ThreadSync Condition APIs with abnormal condition:
 * - Test02_012_004: state transition error of R_OSAL_ThsyncCondSignal
 * @subsection ThsyncCondSignal_TC_02_012_004_tpts Test Procedure/Test Steps
 * -# Unblock one thread by using R_OSAL_ThsyncCondSignal
 * @subsection ThsyncCondSignal_TC_02_012_004_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncCondSignal return OSAL_RETURN_STATE.
 * @subsection ThsyncCondSignal_TC_02_012_004_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondSignal_TC_02_012_004
 */
// [ThsyncCondSignal_TC_02_012_004]
TEST_F(OsalApiStateTest, ThsyncCondSignal_TC_02_012_004)
{
    osal_cond_handle_t cond_handle = OSAL_COND_HANDLE_INVALID;

    // Test02_012_004
    // @brief: abnormal case: R_OSAL_ThsyncCondSignal state transition error
    EXPECT_EQ(R_OSAL_ThsyncCondSignal(cond_handle), OSAL_RETURN_STATE) << "Test02_011_004 failed";
}
// [ThsyncCondSignal_TC_02_012_004]

/**
 * @page ThreadSyncCondApiTest_TEST ThreadSync Condition Variable API Test
 * @section OSALINTTESTS_ThsyncCondSignal_TC_02_012_005 OSALINTTESTS.ThsyncCondSignal_TC_02_012_005
 * @subsection ThsyncCondSignal_TC_02_012_005_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection ThsyncCondSignal_TC_02_012_005_tci Test Case ID
 * ThsyncCondSignal_S3_N_TC_02_012_005
 * @subsection ThsyncCondSignal_TC_02_012_005_rr Related Requirement(s)
 * \#199163
 * @subsection ThsyncCondSignal_TC_02_012_005_pp Prerequisite/Precondition
 * -# OSAL is initialized.<br>
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL mutex handle which ID is ISR_MUTEX_ID
 * -# Created an OSAL condition handle which ID is ISR_COND_ID
 * @subsection ThsyncCondSignal_TC_02_012_005_tcs Test Case Summary
 * @subsubsection ThsyncCondSignal_TC_02_012_005_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Create new thread.
 * - Setup required memory for interrupt generation
 * - Register callack function and enable interrupt
 * @subsubsection ThsyncCondSignal_TC_02_012_005_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Join waiting thread
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * @subsubsection ThsyncCondSignal_TC_02_012_005_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality of the ThreadSync Condition APIs which are called in the ISR with normal condition:
 * - Disable and clear interrupt status
 * - Test02_012_005: unblock a waiting thread by using R_OSAL_ThsyncCondSignal
 * @subsection ThsyncCondSignal_TC_02_012_005_tpts Test Procedure/Test Steps
 * @subsubsection ThsyncCondSignal_TC_02_012_005_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Create new thread by using R_OSAL_ThreadCreate.<br>
 * In the worker functions thread above is blocked on a condition variable.
 * -# Allocate required memory for generate interrupt from hardware and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function by using R_OSAL_InterruptRegisterIsr
 * -# Enable interrupt by using R_OSAL_InterruptEnableIsr
 * -# Setup hardware specific register to generate the interrupt by calling to setup_hw_register
 * @subsubsection ThsyncCondSignal_TC_02_012_005_post_tpts Test Procedure/Test Steps of Post-process
 * -# Wait for the thread finish by using R_OSAL_ThreadJoin
 * -# Unregister interrupt by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * @subsubsection ThsyncCondSignal_TC_02_012_005_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Lock mutex by using R_OSAL_ThsyncMutexTryLock
 * -# Signal waiting thread by using R_OSAL_ThsyncCondSignal
 * -# Unlock mutex by using R_OSAL_ThsyncMutexUnlock
 * @subsection ThsyncCondSignal_TC_02_012_005_ereb Expected Result/Behavior
 * @subsubsection ThsyncCondSignal_TC_02_012_005_pre_ereb Expected Result/Behavior of Pre-process
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK.
 * -# allocate_setup_data_memory run successfully
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * The value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK
 * -# setup_hw_register run successfully
 * @subsubsection ThsyncCondSignal_TC_02_012_005_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK.<br>
 * The value of "result" is equal to 0
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * @subsubsection ThsyncCondSignal_TC_02_012_005_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_OK
 * -# R_OSAL_ThsyncCondSignal return OSAL_RETURN_OK
 * -# R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK
 * @subsection ThsyncCondSignal_TC_02_012_005_testcase Test Case Example
 * @snippet test_cond_var.cpp ThsyncCondSignal_TC_02_012_005
 * @snippet test_cond_var.cpp isr_test_waiting_thread
 * @snippet test_cond_var.cpp osal_cond_isr_test_TC_02_012_005
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [ThsyncCondSignal_TC_02_012_005]
TEST_F(OSALINTTESTS, ThsyncCondSignal_TC_02_012_005)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    st_osal_thread_config_t config;
    osal_thread_handle_t thread_handle1 = OSAL_THREAD_HANDLE_INVALID;
    int64_t result;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};
    osal_milli_sec_t time_period = 2000;
    int32_t receive_msg = 0;

    arg.cond_handle = cond;
    arg.mtx_handle = mtx;
    arg.mq_handle = isr_mq;

/* pre-process:
 * create new thread.
 * Setup required memory for interrupt generation.
 * Register callack function and enable interrupt */

    /* Create 1st thread */
    config.priority  = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = (char *)"Simple testing thread";
    config.func      = isr_test_waiting_thread;
    config.userarg   = (void*)&arg;
    config.stack_size = THREAD_STACK_SIZE;
    EXPECT_EQ(R_OSAL_MqReset(isr_mq), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThreadCreate(&config, ISR_THREAD_ID_NO1, &thread_handle1), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(thread_handle1, NULL_PTR) << "pre-process failed";

	/* sync with broadcast thread */
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(receive_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(mtx, time_period), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(mtx), OSAL_RETURN_OK);

    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_GT(numOfChannel, 0) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_cond_isr_test_TC_02_012_005, (void *)&arg), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK) << "pre-process failed";
    setup_hw_register(dev_arr, hw_addr);

/* post-process:
 * join waiting thread.
 * Unregister interrupt
 * Deallocate allocated memory*/
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &result), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(result, 0) << "post-process failed";
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_cond_isr_test_TC_02_012_005), OSAL_RETURN_OK) << "post-process failed";
    deallocate_data_memory(hndl_mmngr, buffer_obj);
}
// [ThsyncCondSignal_TC_02_012_005]
