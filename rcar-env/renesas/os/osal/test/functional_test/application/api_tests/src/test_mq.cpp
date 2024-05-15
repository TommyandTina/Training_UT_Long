/***********************************************************************************************************************
* Copyright [2020-2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    test_mq.cpp
* Version :      3.3.1
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Message Queue Manager API Functional Test
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           2.0.0     29.10.2021    Create New
 *           2.1.0     11.11.2021    Add Tests (supplement Test perspective of existing Tests case)
 *                                   - Add Tests of MqIsAPI and MqIsFull by using UntilTimeStamp API
 *                                      - MqIsFull_TC_03_007_011, MqIsEmpty_TC_03_008_010
 *                                   - Add Tests of parameter about MqSend/Receive API (Boundary Test)
 *                                      - MqSendUntilTimeStamp_TC_03_003_022, MqSendForTimePeriod_TC_03_004_020
 *                                      - MqReceiveUntilTimeStamp_TC_03_005_021, MqReceiveForTimePeriod_TC_03_006_019
 *           2.1.1     15.11.2021    Update Test Name
 *                                   ConfigReset -> ConfigReset_TC_12_001_005
 *           2.1.2     18.11.2021    Fix Typo
 *           3.0.0     08.03.2022    Apply new/updated device definition related to interrupt context test
 *                                   Apply updated allocate_setup_data_memory, setup_hw_register and deallocate_data_memory
 *           3.0.1     23.03.2022    Update MqReceiveForTimePeriod_TC_03_006_005   
 *           3.0.2     30.03.2022    Enable disabled test on SIL
 *           3.0.3     07.04.2022    Add synchronization method for test cases using multi-threads
 *           3.1.0     04.01.2023    Add MqCreate_TC_03_001_006, MqCreate_TC_03_001_007, MqCreate_TC_03_001_008
 *                                   Add handle value initialize for below tests:
 *                                      - MqCreate_TC_03_001_002
 *                                      - MqCreate_TC_03_001_005
 *                                      - MqInitializeMqConfigSt_TC_03_011_003
 *           3.2.0     01.02.2023    Add new boundary test cases as below:
 *                                      - MqSendUntilTimeStamp_TC_03_003_023
 *                                      - MqSendUntilTimeStamp_TC_03_003_024
 *                                      - MqSendUntilTimeStamp_TC_03_003_025
 *                                      - MqSendUntilTimeStamp_TC_03_003_026
 *                                      - MqReceiveUntilTimeStamp_TC_03_005_022
 *                                      - MqReceiveUntilTimeStamp_TC_03_005_023
 *                                      - MqReceiveUntilTimeStamp_TC_03_005_024
 *                                      - MqReceiveUntilTimeStamp_TC_03_005_025
 *           3.3.0     07.03.2023    Add new interrupt context test as below:
 *                                      - MqReceiveUntilTimeStamp_TC_03_005_026
 *                                      - MqReceiveForTimePeriod_TC_03_006_020
 *           3.3.1     10.04.2023    Update expected value for median time for SIL Windows performance test
***********************************************************************************************************************/
#include "test_common.hpp"

/* armclang will be warnig, when specified OsalApiStateTest to TEST_F */
/* W.A Derive a class and replace it by define */
class OsalApiStateTest_MQ : public OsalApiStateTest
{
    /* There is no need for member variables and member functions.
       We added them just for the sake of adding a test class. */
};
#define OsalApiStateTest OsalApiStateTest_MQ

/* armclang will be warnig, when specified OSALINTTESTS to TEST_F */
/* W.A Derive a class and replace it by define */
class OSALINTTESTS_MQ : public OSALINTTESTS
{
    /* There is no need for member variables and member functions.
       We added them just for the sake of adding a test class. */
};
#define OSALINTTESTS OSALINTTESTS_MQ

#if defined(ENABLE_LONGTIME_TEST)
#define Num_OF_MQ_REPEATS 100
#else
#define Num_OF_MQ_REPEATS 10
#endif

#if !defined(OSAL_TEST_FRAMEWORK)
#include <memory>
using namespace std;
#else
using namespace OsalFWHelper;
#endif

struct message
{
    int content;
};

const int max_num_msg = 10;

/**
 * @page MQ_TEST MQ API Test
 * @test
 * @test @link testMQ_ConfigReset_TC_12_001_005 @endlink
 * @test @link MQApiTest_MqCreate_TC_03_001_001 @endlink
 * @test @link MQApiTest_MqCreate_TC_03_001_002 @endlink
 * @test @link MQApiTest_MqCreate_TC_03_001_003 @endlink
 * @test @link MQApiTest_MqCreate_TC_03_001_004 @endlink
 * @test @link OsalApiStateTest_MqCreate_TC_03_001_005 @endlink
 * @test @link MQApiTest_MqCreate_TC_03_001_006 @endlink
 * @test @link MQApiTest_MqCreate_TC_03_001_007 @endlink
 * @test @link MQApiTest_MqCreate_TC_03_001_008 @endlink
 * @test @link MQApiTest_MqDelete_TC_03_002_001 @endlink
 * @test @link MQApiTest_MqDelete_TC_03_002_002 @endlink
 * @test @link MQApiTest_MqDelete_TC_03_002_003 @endlink
 * @test @link MQApiTest_MqDelete_TC_03_002_004 @endlink
 * @test @link MQApiTest_MqDelete_TC_03_002_005 @endlink
 * @test @link OsalApiStateTest_MqDelete_TC_03_002_006 @endlink
 * @test @link MQApiTest_MqDelete_TC_03_002_007 @endlink
 * @test @link MQApiTest_MqDelete_TC_03_002_008 @endlink
 * @test @link MQApiTest_MqDelete_TC_03_002_009 @endlink
 * @test @link MQApiTest_MqSendUntilTimeStamp_TC_03_003_001 @endlink
 * @test @link MQApiTest_MqSendUntilTimeStamp_TC_03_003_002 @endlink
 * @test @link MQApiTest_MqSendUntilTimeStamp_TC_03_003_003 @endlink
 * @test @link MQApiTest_MqSendUntilTimeStamp_TC_03_003_004 @endlink
 * @test @link MQApiTest_MqSendUntilTimeStamp_TC_03_003_005 @endlink
 * @test @link MQApiTest_MqSendUntilTimeStamp_TC_03_003_006 @endlink
 * @test @link MQApiTest_MqSendUntilTimeStamp_TC_03_003_007 @endlink
 * @test @link MQApiTest_MqSendUntilTimeStamp_TC_03_003_008 @endlink
 * @test @link MQApiTest_MqSendUntilTimeStamp_TC_03_003_009 @endlink
 * @test @link MQApiTest_MqSendUntilTimeStamp_TC_03_003_010 @endlink
 * @test @link MQApiTest_MqSendUntilTimeStamp_TC_03_003_011 @endlink
 * @test @link MQApiTest_MqSendUntilTimeStamp_TC_03_003_012 @endlink
 * @test @link MQApiTest_MqSendUntilTimeStamp_TC_03_003_013 @endlink
 * @test @link MQApiTest_MqSendUntilTimeStamp_TC_03_003_014 @endlink
 * @test @link OsalApiStateTest_MqSendUntilTimeStamp_TC_03_003_015 @endlink
 * @test @link OSALINTTESTS_MqSendUntilTimeStamp_TC_03_003_016 @endlink
 * @test @link OSALINTTESTS_MqSendUntilTimeStamp_TC_03_003_017 @endlink
 * @test @link MQApiTest_MqSendUntilTimeStamp_TC_03_003_018 @endlink
 * @test @link MQApiTest_MqSendUntilTimeStamp_TC_03_003_019 @endlink
 * @test @link MQApiTest_MqSendUntilTimeStamp_TC_03_003_020 @endlink
 * @test @link MQApiTest_MqSendUntilTimeStamp_TC_03_003_021 @endlink
 * @test @link MQSizeParameterizedTest_MqSendUntilTimeStamp_TC_03_003_022 @endlink
 * @test @link MQApiTest_MqSendUntilTimeStamp_TC_03_003_023 @endlink
 * @test @link MQApiTest_MqSendUntilTimeStamp_TC_03_003_024 @endlink
 * @test @link MQApiTest_MqSendUntilTimeStamp_TC_03_003_025 @endlink
 * @test @link MQApiTest_MqSendUntilTimeStamp_TC_03_003_026 @endlink
 * @test @link MQApiTest_MqSendForTimePeriod_TC_03_004_001 @endlink
 * @test @link MQApiTest_MqSendForTimePeriod_TC_03_004_002 @endlink
 * @test @link MQApiTest_MqSendForTimePeriod_TC_03_004_003 @endlink
 * @test @link MQApiTest_MqSendForTimePeriod_TC_03_004_004 @endlink
 * @test @link MQApiTest_MqSendForTimePeriod_TC_03_004_005 @endlink
 * @test @link MQApiTest_MqSendForTimePeriod_TC_03_004_006 @endlink
 * @test @link MQApiTest_MqSendForTimePeriod_TC_03_004_007 @endlink
 * @test @link MQApiTest_MqSendForTimePeriod_TC_03_004_008 @endlink
 * @test @link MQApiTest_MqSendForTimePeriod_TC_03_004_009 @endlink
 * @test @link MQApiTest_MqSendForTimePeriod_TC_03_004_010 @endlink
 * @test @link OsalApiStateTest_MqSendForTimePeriod_TC_03_004_011 @endlink
 * @test @link MQApiTest_MqSendForTimePeriod_TC_03_004_012 @endlink
 * @test @link OSALINTTESTS_MqSendForTimePeriod_TC_03_004_013 @endlink
 * @test @link OSALINTTESTS_MqSendForTimePeriod_TC_03_004_014 @endlink
 * @test @link MQApiTest_MqSendForTimePeriod_TC_03_004_015 @endlink
 * @test @link MQApiTest_MqSendForTimePeriod_TC_03_004_016 @endlink
 * @test @link MQApiTest_MqSendForTimePeriod_TC_03_004_017 @endlink
 * @test @link MQApiTest_MqSendForTimePeriod_TC_03_004_018 @endlink
 * @test @link MQApiTest_MqSendForTimePeriod_TC_03_004_019 @endlink
 * @test @link MQSizeParameterizedTest_MqSendForTimePeriod_TC_03_004_020 @endlink
 * @test @link MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_001 @endlink
 * @test @link MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_002 @endlink
 * @test @link MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_003 @endlink
 * @test @link MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_004 @endlink
 * @test @link MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_005 @endlink
 * @test @link MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_006 @endlink
 * @test @link MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_007 @endlink
 * @test @link MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_008 @endlink
 * @test @link MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_009 @endlink
 * @test @link MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_010 @endlink
 * @test @link MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_011 @endlink
 * @test @link MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_012 @endlink
 * @test @link MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_013 @endlink
 * @test @link MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_014 @endlink
 * @test @link OsalApiStateTest_MqReceiveUntilTimeStamp_TC_03_005_015 @endlink
 * @test @link OSALINTTESTS_MqReceiveUntilTimeStamp_TC_03_005_016 @endlink
 * @test @link OSALINTTESTS_MqReceiveUntilTimeStamp_TC_03_005_017 @endlink
 * @test @link MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_018 @endlink
 * @test @link MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_019 @endlink
 * @test @link MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_020 @endlink
 * @test @link MQSizeParameterizedTest_MqReceiveUntilTimeStamp_TC_03_005_021 @endlink
 * @test @link MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_022 @endlink
 * @test @link MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_023 @endlink
 * @test @link MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_024 @endlink
 * @test @link MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_025 @endlink
 * @test @link OSALINTTESTS_MqReceiveUntilTimeStamp_TC_03_005_026 @endlink
 * @test @link MQApiTest_MqReceiveForTimePeriod_TC_03_006_001 @endlink
 * @test @link MQApiTest_MqReceiveForTimePeriod_TC_03_006_002 @endlink
 * @test @link MQApiTest_MqReceiveForTimePeriod_TC_03_006_003 @endlink
 * @test @link MQApiTest_MqReceiveForTimePeriod_TC_03_006_004 @endlink
 * @test @link MQApiTest_MqReceiveForTimePeriod_TC_03_006_005 @endlink
 * @test @link MQApiTest_MqReceiveForTimePeriod_TC_03_006_006 @endlink
 * @test @link MQApiTest_MqReceiveForTimePeriod_TC_03_006_007 @endlink
 * @test @link MQApiTest_MqReceiveForTimePeriod_TC_03_006_008 @endlink
 * @test @link MQApiTest_MqReceiveForTimePeriod_TC_03_006_009 @endlink
 * @test @link MQApiTest_MqReceiveForTimePeriod_TC_03_006_010 @endlink
 * @test @link OsalApiStateTest_MqReceiveForTimePeriod_TC_03_006_011 @endlink
 * @test @link MQApiTest_MqReceiveForTimePeriod_TC_03_006_012 @endlink
 * @test @link OSALINTTESTS_MqReceiveForTimePeriod_TC_03_006_013 @endlink
 * @test @link OSALINTTESTS_MqReceiveForTimePeriod_TC_03_006_014 @endlink
 * @test @link MQApiTest_MqReceiveForTimePeriod_TC_03_006_015 @endlink
 * @test @link MQApiTest_MqReceiveForTimePeriod_TC_03_006_016 @endlink
 * @test @link MQApiTest_MqReceiveForTimePeriod_TC_03_006_017 @endlink
 * @test @link MQApiTest_MqReceiveForTimePeriod_TC_03_006_018 @endlink
 * @test @link MQSizeParameterizedTest_MqReceiveForTimePeriod_TC_03_006_019 @endlink
 * @test @link OSALINTTESTS_MqReceiveForTimePeriod_TC_03_006_020 @endlink
 * @test @link MQApiTest_MqIsFull_TC_03_007_001 @endlink
 * @test @link MQApiTest_MqIsFull_TC_03_007_002 @endlink
 * @test @link MQApiTest_MqIsFull_TC_03_007_003 @endlink
 * @test @link MQApiTest_MqIsFull_TC_03_007_004 @endlink
 * @test @link MQApiTest_MqIsFull_TC_03_007_005 @endlink
 * @test @link OsalApiStateTest_MqIsFull_TC_03_007_006 @endlink
 * @test @link OSALINTTESTS_MqIsFull_TC_03_007_007 @endlink
 * @test @link OSALINTTESTS_MqIsFull_TC_03_007_008 @endlink
 * @test @link MQApiTest_MqIsFull_TC_03_007_009 @endlink
 * @test @link MQApiTest_MqIsFull_TC_03_007_010 @endlink
 * @test @link MQApiTest_MqIsFull_TC_03_007_011 @endlink
 * @test @link MQApiTest_MqIsEmpty_TC_03_008_001 @endlink
 * @test @link MQApiTest_MqIsEmpty_TC_03_008_002 @endlink
 * @test @link MQApiTest_MqIsEmpty_TC_03_008_003 @endlink
 * @test @link MQApiTest_MqIsEmpty_TC_03_008_004 @endlink
 * @test @link MQApiTest_MqIsEmpty_TC_03_008_005 @endlink
 * @test @link OsalApiStateTest_MqIsEmpty_TC_03_008_006 @endlink
 * @test @link OSALINTTESTS_MqIsEmpty_TC_03_008_007 @endlink
 * @test @link OSALINTTESTS_MqIsEmpty_TC_03_008_008 @endlink
 * @test @link MQApiTest_MqIsEmpty_TC_03_008_009 @endlink
 * @test @link MQApiTest_MqIsEmpty_TC_03_008_010 @endlink
 * @test @link MQApiTest_MqGetConfig_TC_03_009_001 @endlink
 * @test @link MQApiTest_MqGetConfig_TC_03_009_002 @endlink
 * @test @link MQApiTest_MqGetConfig_TC_03_009_003 @endlink
 * @test @link MQApiTest_MqGetConfig_TC_03_009_004 @endlink
 * @test @link OsalApiStateTest_MqGetConfig_TC_03_009_005 @endlink
 * @test @link MQApiTest_MqReset_TC_03_010_001 @endlink
 * @test @link MQApiTest_MqReset_TC_03_010_002 @endlink
 * @test @link MQApiTest_MqReset_TC_03_010_003 @endlink
 * @test @link MQApiTest_MqReset_TC_03_010_004 @endlink
 * @test @link MQApiTest_MqReset_TC_03_010_005 @endlink
 * @test @link OsalApiStateTest_MqReset_TC_03_010_006 @endlink
 * @test @link MQApiTest_MqReset_TC_03_010_008 @endlink
 * @test @link MQApiTest_MqReset_TC_03_010_009 @endlink
 * @test @link MQApiTest_MqInitializeMqConfigSt_TC_03_011_001 @endlink
 * @test @link MQApiTest_MqInitializeMqConfigSt_TC_03_011_002 @endlink
 * @test @link MQApiTest_MqInitializeMqConfigSt_TC_03_011_003 @endlink
 * @section testMQ testMQ function
 * @subsection testMQ_SetUp testMQ Setup
 * Common to Test Suit of testMQ, prepare process to be executed before executing the test. <br>
 * For example:
 * @snippet test_mq.cpp testMQ_SetUp
 * @subsection testMQ_TearDown testMQ TearDown
 * Common to Test Suit of testMQ, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_mq.cpp testMQ_TearDown
 * @section MQApiTest MQApiTest function
 * @subsection MQApiTest_SetUp MQApiTest Setup
 * Common to Test Suit of MQApiTest, prepare process to be executed before executing the test. <br>
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.<br>
 * This queue name is "shared_queue", msg_size: 4 num: 2. Please do not change these.
 * For example:
 * @snippet test_mq.cpp MQApiTest_SetUp
 * @snippet test_mq.cpp MQApiTest_SetUpTestSuite
 * @subsection MQApiTest_TearDown MQApiTest TearDown
 * Common to Test Suit of MQApiTest, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_mq.cpp MQApiTest_TearDown
 * @snippet test_mq.cpp MQApiTest_TearDownTestSuite
 * @section OSALINTTESTS_MQApiTest OSALINTTESTS of MQApiTest function
 * @subsection OSALINTTESTS_MQApiTest_SetUp OSALINTTESTS of MQApiTest Setup
 * Common to Test Suit of OSALINTTESTS MQApiTest, prepare process to be executed before executing the test. <br>
 * This test suite generates interrupts. These process is preparation<br>
 * For example:
 * @snippet test_common.hpp OSALINTTESTS_SetUp
 * @subsection OSALINTTESTS_MQApiTest_TearDown OSALINTTESTS of MQApiTest TearDown
 * Common to Test Suit of OSALINTTESTS MQApiTest, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_common.hpp OSALINTTESTS_TearDown
 * @section OsalApiStateTest_MQApiTest OsalApiStateTest of MQApiTest function
 * @subsection OsalApiStateTest_MQApiTest_SetUp OsalApiStateTest of MQApiTest Setup
 * Common to Test Suit of OsalApiStateTest MQApiTest, prepare process to be executed before executing the test. <br>
 * For example:
 * @snippet test_common.hpp OsalApiStateTest_SetUp
 * @subsection OsalApiStateTest_MQApiTest_TearDown OsalApiStateTest of MQApiTest TearDown
 * Common to Test Suit of OsalApiStateTest, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_common.hpp OsalApiStateTest_TearDown
 * @section MQSizeParameterizedTest MQSizeParameterizedTest function
 * @subsection MQSizeParameterizedTest_SetUp MQSizeParameterizedTest Setup
 * Common to Test Suit of MQSizeParameterizedTest, prepare process to be executed before executing the test. <br>
 * For example:
 * @snippet test_mq.cpp MQSizeParameterizedTest_SetUp
 * @subsection MQSizeParameterizedTest_TearDown MQSizeParameterizedTest TearDown
 * Common to Test Suit of MQSizeParameterizedTest, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_mq.cpp MQSizeParameterizedTest_TearDown
 */
class testMQ : public OSALTESTS
{
protected:
    shared_ptr<osal_mq_handle_t> queue;

public:
// [testMQ_SetUp]
    void SetUp() override
    {
        OSALTESTS::SetUp();

        queue = shared_ptr<osal_mq_handle_t>(new osal_mq_handle_t());

        st_osal_mq_config_t config_in;
        config_in.max_num_msg = max_num_msg;
        config_in.msg_size    = sizeof(message);

        ASSERT_EQ(R_OSAL_MqCreate(&config_in, TEST_MQ_COMMON_ID, queue.get()), OSAL_RETURN_OK);
    }
// [testMQ_SetUp]

// [testMQ_TearDown]
    void TearDown() override
    {
        ASSERT_EQ(R_OSAL_MqDelete(*queue), OSAL_RETURN_OK);
        OSALTESTS::TearDown();
    }
// [testMQ_TearDown]
};

/**
 * @page MQ_TEST MQ API Test
 * @section testMQ_ConfigReset_TC_12_001_005 testMQ.ConfigReset_TC_12_001_005
 * @subsection ConfigReset_tsi Test Suite ID
 * testMQ
 * @subsection ConfigReset_tci Test Case ID
 * ConfigReset_S3_N_TC_12_001_005
 * @subsection ConfigReset_rr Related Requirement(s)
 * \#199176, \#199178, \#199179, \#199180
 * @subsection ConfigReset_pp Prerequisite/Precondition
 * OSAL is initialized and Message Queue has been created.
 * @subsection ConfigReset_tcs Test Case Summary
 * This test shall verify the functionality of the OSAL Reset/GetConfig API on a created queue.
 * @subsection ConfigReset_tpts Test Procedure/Test Steps
 * -# Reset the message queue with R_OSAL_MqReset.
 * -# Check queue is full or not by using R_OSAL_MqIsFull.
 * -# Check queue is empty or not by using R_OSAL_MqIsEmpty.
 * -# Send a random number of messages to queue using R_OSAL_MqSendForTimePeriod.
 * -# Reset all messages in queue again with R_OSAL_MqReset.
 * -# Check queue is full or not by using R_OSAL_MqIsFull.
 * -# Check queue is empty or not by using R_OSAL_MqIsEmpty.
 * -# Get configuration of queue using R_OSAL_MqGetConfig.
 * @subsection ConfigReset_ereb Expected Result/Behavior
 * -# R_OSAL_MqReset return OSAL_RETURN_OK.
 * -# R_OSAL_MqIsFull return OSAL_RETURN_OK.<br>
 * "full" is false
 * -# R_OSAL_MqIsEmpty return OSAL_RETURN_OK.<br>
 * "empty" is true.
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK.
 * -# R_OSAL_MqReset return OSAL_RETURN_OK.
 * -# R_OSAL_MqIsFull return OSAL_RETURN_OK.<br>
 * "full" is false
 * -# R_OSAL_MqIsEmpty return OSAL_RETURN_OK.<br>
 * "empty" is true.
 * -# R_OSAL_MqGetConfig return OSAL_RETURN_OK.<br>
 * Value of "config_out.max_num_msg" is equal to "config_in.max_num_msg".<br>
 * Value of "config_out.msg_size" is equal to "config_in.msg_size".
 * @subsection ConfigReset_testcase Test Case Example
 * @snippet test_mq.cpp ConfigReset_TC_12_001_005
 */
// [ConfigReset_TC_12_001_005]
TEST_F(testMQ, ConfigReset_TC_12_001_005)
{
    bool full = true, empty = false;
    EXPECT_EQ(R_OSAL_MqReset(*queue), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqIsFull(*queue, &full), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqIsEmpty(*queue, &empty), OSAL_RETURN_OK);
    EXPECT_FALSE(full);
    EXPECT_TRUE(empty);

    int random_value = rand() % max_num_msg + 1;
    for (int i = 0; i < random_value; i++)
    {
        // send a rendom number of messages
        message msg = {i};
        EXPECT_EQ(R_OSAL_MqSendForTimePeriod(*queue, 10, &msg, sizeof(msg)), OSAL_RETURN_OK);
    }

    EXPECT_EQ(R_OSAL_MqReset(*queue), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqIsFull(*queue, &full), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqIsEmpty(*queue, &empty), OSAL_RETURN_OK);
    EXPECT_FALSE(full);
    EXPECT_TRUE(empty);

    st_osal_mq_config_t config_out;
    EXPECT_EQ(R_OSAL_MqGetConfig(*queue, &config_out), OSAL_RETURN_OK);
    EXPECT_EQ(config_out.max_num_msg, max_num_msg);
    EXPECT_EQ(config_out.msg_size, sizeof(message));
}
// [ConfigReset_TC_12_001_005]

typedef struct st_arg_mq_send_timestamp {
    st_osal_time_t time;
    osal_mq_handle_t mq_handle;
    void *p_buffer;
    size_t buffer_size;
    osal_mq_handle_t sync_mq;
    int sync_send_msg;
    bool needToSync;
} st_arg_mq_send_timestamp_t;

typedef struct st_arg_mq_send_timeperiod {
    osal_milli_sec_t time;
    osal_mq_handle_t mq_handle;
    void *p_buffer;
    size_t buffer_size;
    osal_mq_handle_t sync_mq;
    int sync_send_msg;
    bool needToSync;
} st_arg_mq_send_timeperiod_t;

typedef struct st_arg_mq_receive_timeperiod {
    osal_milli_sec_t time;
    osal_mq_handle_t mq_handle;
    osal_mq_handle_t sync_mq;
    int sync_send_msg;
    bool needToSync;
} st_arg_mq_receive_timeperiod_t;

typedef struct st_arg_mq_receive_timestamp {
    st_osal_time_t time;
    osal_mq_handle_t mq_handle;
    osal_mq_handle_t sync_mq;
    int sync_send_msg;
    bool needToSync;
} st_arg_mq_receive_timestamp_t;

// [osal_test_thread_send_timestamp]
int64_t osal_test_thread_send_timestamp(void *user_arg)
{
    e_osal_return_t osal_ret    = OSAL_RETURN_OK;
    st_arg_mq_send_timestamp_t *arg;
    arg = (st_arg_mq_send_timestamp_t *) user_arg;
    st_osal_mq_config local_mq_config;

    if(arg->needToSync == true)
    {
        EXPECT_EQ(R_OSAL_MqGetConfig(arg->sync_mq, &local_mq_config), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_MqSendForTimePeriod(arg->sync_mq, SYNC_MQ_TIME, &arg->sync_send_msg, local_mq_config.msg_size), OSAL_RETURN_OK);
    }

    osal_ret = R_OSAL_MqSendUntilTimeStamp(arg->mq_handle, &arg->time, arg->p_buffer, arg->buffer_size);
    return (int64_t)osal_ret;
}
// [osal_test_thread_send_timestamp]

// [osal_test_thread_send_timeperiod]
int64_t osal_test_thread_send_timeperiod(void *user_arg)
{
    e_osal_return_t osal_ret    = OSAL_RETURN_OK;
    st_arg_mq_send_timeperiod_t *arg;
    arg = (st_arg_mq_send_timeperiod_t *) user_arg;
    st_osal_mq_config local_mq_config;

    if(arg->needToSync == true)
    {
        EXPECT_EQ(R_OSAL_MqGetConfig(arg->sync_mq, &local_mq_config), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_MqSendForTimePeriod(arg->sync_mq, SYNC_MQ_TIME, &arg->sync_send_msg, local_mq_config.msg_size), OSAL_RETURN_OK);
    }

    osal_ret = R_OSAL_MqSendForTimePeriod(arg->mq_handle, arg->time, arg->p_buffer, arg->buffer_size);
    return (int64_t)osal_ret;
}
// [osal_test_thread_send_timeperiod]

// [osal_test_thread_receive_timeperiod]
int64_t osal_test_thread_receive_timeperiod(void *user_arg)
{
    e_osal_return_t osal_ret    = OSAL_RETURN_OK;
    st_arg_mq_receive_timeperiod_t *arg;
    arg = (st_arg_mq_receive_timeperiod_t *) user_arg;
    uint32_t rcv_data;
    st_osal_mq_config local_mq_config;

    if(arg->needToSync == true)
    {
        EXPECT_EQ(R_OSAL_MqGetConfig(arg->sync_mq, &local_mq_config), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_MqSendForTimePeriod(arg->sync_mq, SYNC_MQ_TIME, &arg->sync_send_msg, local_mq_config.msg_size), OSAL_RETURN_OK);
    }

    osal_ret = R_OSAL_MqReceiveForTimePeriod(arg->mq_handle, arg->time, &rcv_data, sizeof(rcv_data));
    if ((rcv_data == 0xFFFF) && (osal_ret == OSAL_RETURN_OK))
    {
        osal_ret = OSAL_RETURN_OK;
        return (int64_t)osal_ret;
    }
    else
    {
        return (int64_t)osal_ret;
    }
}
// [osal_test_thread_receive_timeperiod]

// [osal_test_thread_receive_timestamp]
int64_t osal_test_thread_receive_timestamp(void *user_arg)
{
    e_osal_return_t osal_ret    = OSAL_RETURN_OK;
    st_arg_mq_receive_timestamp_t *arg;
    arg = (st_arg_mq_receive_timestamp_t *) user_arg;
    uint32_t rcv_data;
    st_osal_mq_config local_mq_config;

    if(arg->needToSync == true)
    {
        EXPECT_EQ(R_OSAL_MqGetConfig(arg->sync_mq, &local_mq_config), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_MqSendForTimePeriod(arg->sync_mq, SYNC_MQ_TIME, &arg->sync_send_msg, local_mq_config.msg_size), OSAL_RETURN_OK);
    }

    osal_ret = R_OSAL_MqReceiveUntilTimeStamp(arg->mq_handle, &arg->time, &rcv_data, sizeof(rcv_data));
    if ((rcv_data == 0xFFFF) && (osal_ret == OSAL_RETURN_OK))
    {
        osal_ret = OSAL_RETURN_OK;
        return (int64_t)osal_ret;
    }
    else
    {
        return (int64_t)osal_ret;
    }
}
// [osal_test_thread_receive_timestamp]

// [osal_test_thread_delete_mq]
int64_t osal_test_thread_delete_mq(void *user_arg)
{
    e_osal_return_t osal_ret    = OSAL_RETURN_OK;
    osal_mq_handle_t *arg;
    arg = (osal_mq_handle_t *) user_arg;
    osal_ret = R_OSAL_MqDelete(*(arg));
    return (int64_t)osal_ret;
}
// [osal_test_thread_delete_mq]

// [osal_test_mq1]
int64_t osal_test_mq1(void *user_arg)
{

    e_osal_return_t  osal_ret1;
    e_osal_return_t  osal_ret2;
    osal_milli_sec_t time;
    uint32_t         send_data;
    st_arg_mq_send_timeperiod_t *arg;
    arg = (st_arg_mq_send_timeperiod_t *) user_arg;
    st_osal_mq_config local_mq_config;

    if(arg->needToSync == true)
    {
        EXPECT_EQ(R_OSAL_MqGetConfig(arg->sync_mq, &local_mq_config), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_MqSendForTimePeriod(arg->sync_mq, SYNC_MQ_TIME, &arg->sync_send_msg, local_mq_config.msg_size), OSAL_RETURN_OK);
    }

    time = 5000;

    R_OSAL_ThreadSleepForTimePeriod(1);

    /* message send */
    send_data = 0x1111;
    osal_ret1 = R_OSAL_MqSendForTimePeriod(arg->mq_handle, time, &send_data, sizeof(send_data));
    if (osal_ret1 != OSAL_RETURN_OK)
    {
        return 1;
    }
    send_data = 0x2222;
    osal_ret2 = R_OSAL_MqSendForTimePeriod(arg->mq_handle, time, &send_data, sizeof(send_data));
    if (osal_ret2 != OSAL_RETURN_OK)
    {
        return 1;
    }

    return 0;
}
// [osal_test_mq1]

// [osal_test_mq2]
int64_t osal_test_mq2(void *user_arg)
{

    osal_mq_handle_t mq_handle = NULL;
    e_osal_return_t   osal_ret;
    bool             diff_msg = false;
    osal_milli_sec_t time;
    uint32_t         recv_data;

    R_OSAL_ThreadSleepForTimePeriod(1);

    memcpy(&mq_handle, (osal_mq_handle_t *)user_arg, sizeof(mq_handle));
    time = 5000;

    /* wait for message send osal_test_mq2(). */
    R_OSAL_ThreadSleepForTimePeriod(1000);

    /* message receive */

    osal_ret = R_OSAL_MqReceiveForTimePeriod(mq_handle, time, &recv_data, sizeof(recv_data));
    if ((osal_ret != OSAL_RETURN_OK) || (recv_data != 0x1111))
    {
        diff_msg = true;
    }
    osal_ret = R_OSAL_MqReceiveForTimePeriod(mq_handle, time, &recv_data, sizeof(recv_data));
    if ((osal_ret != OSAL_RETURN_OK) || (recv_data != 0x2222))
    {
        diff_msg = true;
    }

    if (diff_msg == true)
    {
        return 1;
    }
    return 0;
}
// [osal_test_mq2]


// [osal_test_mq5]
int64_t osal_test_mq5(void *user_arg)
{

    e_osal_return_t  osal_ret1;
    osal_milli_sec_t time;
    uint32_t         send_data;
    st_arg_mq_send_timeperiod_t *arg;
    arg = (st_arg_mq_send_timeperiod_t *) user_arg;
    st_osal_mq_config local_mq_config;

    if(arg->needToSync == true)
    {
        EXPECT_EQ(R_OSAL_MqGetConfig(arg->sync_mq, &local_mq_config), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_MqSendForTimePeriod(arg->sync_mq, SYNC_MQ_TIME, &arg->sync_send_msg, local_mq_config.msg_size), OSAL_RETURN_OK);
    }

    time = 10000;

    /* message send */
    send_data = 0x2222;
    osal_ret1 = R_OSAL_MqSendForTimePeriod(arg->mq_handle, time, &send_data, sizeof(send_data));
    return (int64_t)osal_ret1;
}
// [osal_test_mq5]

// [osal_test_mq6]
int64_t osal_test_mq6(void *user_arg)
{

    e_osal_return_t  osal_ret1;
    osal_milli_sec_t time;
    uint32_t         send_data;
    st_arg_mq_send_timeperiod_t *arg;
    arg = (st_arg_mq_send_timeperiod_t *) user_arg;
    st_osal_mq_config local_mq_config;

    if(arg->needToSync == true)
    {
        EXPECT_EQ(R_OSAL_MqGetConfig(arg->sync_mq, &local_mq_config), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_MqSendForTimePeriod(arg->sync_mq, SYNC_MQ_TIME, &arg->sync_send_msg, local_mq_config.msg_size), OSAL_RETURN_OK);
    }

    time = 10000;

    /* message send */
    send_data = 0x3333;
    osal_ret1 = R_OSAL_MqSendForTimePeriod(arg->mq_handle, time, &send_data, sizeof(send_data));
    return (int64_t)osal_ret1;
}
// [osal_test_mq6]

// [osal_test_mq7]
int64_t osal_test_mq7(void *user_arg)
{

    osal_mq_handle_t mq_handle = NULL;
    e_osal_return_t  osal_ret1;

    memcpy(&mq_handle, (osal_mq_handle_t *)user_arg, sizeof(mq_handle));

    /* message send */
    osal_ret1 = R_OSAL_MqReset(mq_handle);
    return (int64_t)osal_ret1;
}
// [osal_test_mq7]

//Callback function interrupt context test case
// [mq_test_int_cb1_normal_TC_03_003_016]
void mq_test_int_cb1_normal_TC_03_003_016(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    st_osal_time_t  current_time;
    int send_msg = ISR_MQ_MSG_CONTENT;
    int receive_msg = 0;
    osal_mq_handle_t* local_mq_handle = (osal_mq_handle_t*)arg->additional_arg;
    osal_milli_sec_t time_period_0ms = 0;
    uint32_t reg = TEST_REG_CLEAR_REGISTER;
    uint32_t value = TEST_CLEAR_REGISTER_VALUE;

     /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, reg, value), OSAL_RETURN_OK);

    // Test03_003_016
    // @brief: normal case: call R_OSAL_MqSendUntilTimeStamp in ISR
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqSendUntilTimeStamp(*local_mq_handle, &current_time, &send_msg, ISR_MQ_MSG_SIZE), OSAL_RETURN_OK) << "Test03_003_016 failed";
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(*local_mq_handle, time_period_0ms, &receive_msg, ISR_MQ_MSG_SIZE), OSAL_RETURN_OK);

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(arg->mq_handle, time_period_0ms, &arg->send_msg, sizeof(arg->send_msg)), OSAL_RETURN_OK);
}
// [mq_test_int_cb1_normal_TC_03_003_016]

//Callback function interrupt context test case
// [mq_test_int_cb1_normal_TC_03_004_013]
void mq_test_int_cb1_normal_TC_03_004_013(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    int send_msg = ISR_MQ_MSG_CONTENT;
    int receive_msg = 0;
    osal_mq_handle_t* local_mq_handle = (osal_mq_handle_t*)arg->additional_arg;
    osal_milli_sec_t time_period_0ms = 0;
    uint32_t reg = TEST_REG_CLEAR_REGISTER;
    uint32_t value = TEST_CLEAR_REGISTER_VALUE;

     /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, reg, value), OSAL_RETURN_OK);

    // Test03_004_013
    // @brief: normal case: call R_OSAL_MqSendForTimePeriod in ISR
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(*local_mq_handle, time_period_0ms, &send_msg, ISR_MQ_MSG_SIZE), OSAL_RETURN_OK) << "Test03_004_013 failed";
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(*local_mq_handle, time_period_0ms, &receive_msg, ISR_MQ_MSG_SIZE), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, ISR_MQ_MSG_CONTENT);

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(arg->mq_handle, time_period_0ms, &arg->send_msg, sizeof(arg->send_msg)), OSAL_RETURN_OK);
}
// [mq_test_int_cb1_normal_TC_03_004_013]

//Callback function interrupt context test case
// [mq_test_int_cb1_normal_TC_03_005_016]
void mq_test_int_cb1_normal_TC_03_005_016(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    st_osal_time_t  current_time;
    int send_msg = ISR_MQ_MSG_CONTENT;
    int receive_msg = 0;
    osal_mq_handle_t* local_mq_handle = (osal_mq_handle_t*)arg->additional_arg;
    osal_milli_sec_t time_period_0ms = 0;
    uint32_t reg = TEST_REG_CLEAR_REGISTER;
    uint32_t value = TEST_CLEAR_REGISTER_VALUE;

     /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, reg, value), OSAL_RETURN_OK);

    // Test03_005_016
    // @brief: normal case: call R_OSAL_MqReceiveUntilTimeStamp in ISR
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(*local_mq_handle, time_period_0ms, &send_msg, ISR_MQ_MSG_SIZE), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReceiveUntilTimeStamp(*local_mq_handle, &current_time, &receive_msg, ISR_MQ_MSG_SIZE), OSAL_RETURN_OK) << "Test03_005_016 failed";
    EXPECT_EQ(receive_msg,ISR_MQ_MSG_CONTENT );

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(arg->mq_handle, time_period_0ms, &arg->send_msg, sizeof(arg->send_msg)), OSAL_RETURN_OK);
}
// [mq_test_int_cb1_normal_TC_03_005_016]

//Callback function interrupt context test case
// [mq_test_int_cb1_normal_TC_03_006_013]
void mq_test_int_cb1_normal_TC_03_006_013(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    int send_msg = ISR_MQ_MSG_CONTENT;
    int receive_msg = 0;
    osal_mq_handle_t* local_mq_handle = (osal_mq_handle_t*)arg->additional_arg;
    osal_milli_sec_t time_period_0ms = 0;
    uint32_t reg = TEST_REG_CLEAR_REGISTER;
    uint32_t value = TEST_CLEAR_REGISTER_VALUE;

     /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, reg, value), OSAL_RETURN_OK);

    // Test03_006_013
    // @brief: normal case: call R_OSAL_MqReceiveForTimePeriod in ISR
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(*local_mq_handle, time_period_0ms, &send_msg, ISR_MQ_MSG_SIZE), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(*local_mq_handle, time_period_0ms, &receive_msg, ISR_MQ_MSG_SIZE), OSAL_RETURN_OK) << "Test03_006_013 failed";
    EXPECT_EQ(receive_msg, ISR_MQ_MSG_CONTENT);

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(arg->mq_handle, time_period_0ms, &arg->send_msg, sizeof(arg->send_msg)), OSAL_RETURN_OK);
}
// [mq_test_int_cb1_normal_TC_03_006_013]

//Callback function interrupt context test case
// [mq_test_int_cb1_normal_TC_03_007_007]
void mq_test_int_cb1_normal_TC_03_007_007(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    bool isFull = false;
    osal_mq_handle_t* local_mq_handle = (osal_mq_handle_t*)arg->additional_arg;
    osal_milli_sec_t time_period_0ms = 0;
    uint32_t reg = TEST_REG_CLEAR_REGISTER;
    uint32_t value = TEST_CLEAR_REGISTER_VALUE;

     /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, reg, value), OSAL_RETURN_OK);

    // Test03_007_007
    // @brief: normal case: call R_OSAL_MqIsFull in ISR, queue is not full
    EXPECT_EQ(R_OSAL_MqIsFull(*local_mq_handle, &isFull), OSAL_RETURN_OK) << "Test03_007_007 failed";
    EXPECT_EQ(isFull, false);

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(arg->mq_handle, time_period_0ms, &arg->send_msg, sizeof(arg->send_msg)), OSAL_RETURN_OK);
}
// [mq_test_int_cb1_normal_TC_03_007_007]

//Callback function interrupt context test case
// [mq_test_int_cb1_normal_TC_03_007_008]
void mq_test_int_cb1_normal_TC_03_007_008(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    int send_msg = ISR_MQ_MSG_CONTENT;
    int receive_msg = 0;
    bool isFull = false;
    osal_mq_handle_t* local_mq_handle = (osal_mq_handle_t*)arg->additional_arg;
    osal_milli_sec_t time_period_0ms = 0;
    uint32_t reg = TEST_REG_CLEAR_REGISTER;
    uint32_t value = TEST_CLEAR_REGISTER_VALUE;

     /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, reg, value), OSAL_RETURN_OK);

    // Test03_007_008
    // @brief: normal case: call R_OSAL_MqIsFull in ISR, queue is full
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(*local_mq_handle, time_period_0ms, &send_msg, ISR_MQ_MSG_SIZE), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqIsFull(*local_mq_handle, &isFull), OSAL_RETURN_OK) << "Test03_007_008 failed";
    EXPECT_EQ(isFull, true);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(*local_mq_handle, time_period_0ms, &receive_msg, ISR_MQ_MSG_SIZE), OSAL_RETURN_OK);

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(arg->mq_handle, time_period_0ms, &arg->send_msg, sizeof(arg->send_msg)), OSAL_RETURN_OK);
}
// [mq_test_int_cb1_normal_TC_03_007_008]

//Callback function interrupt context test case
// [mq_test_int_cb1_normal_TC_03_008_007]
void mq_test_int_cb1_normal_TC_03_008_007(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    bool isEmpty = false;
    osal_mq_handle_t* local_mq_handle = (osal_mq_handle_t*)arg->additional_arg;
    osal_milli_sec_t time_period_0ms = 0;
    uint32_t reg = TEST_REG_CLEAR_REGISTER;
    uint32_t value = TEST_CLEAR_REGISTER_VALUE;

     /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, reg, value), OSAL_RETURN_OK);

    // Test03_008_007
    // @brief: normal case: call R_OSAL_MqIsEmpty in ISR, queue is empty
    EXPECT_EQ(R_OSAL_MqIsEmpty(*local_mq_handle, &isEmpty), OSAL_RETURN_OK) << "Test03_008_007 failed";
    EXPECT_EQ(isEmpty, true);

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(arg->mq_handle, time_period_0ms, &arg->send_msg, sizeof(arg->send_msg)), OSAL_RETURN_OK);
}
// [mq_test_int_cb1_normal_TC_03_008_007]

//Callback function interrupt context test case
// [mq_test_int_cb1_normal_TC_03_008_008]
void mq_test_int_cb1_normal_TC_03_008_008(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    st_osal_time_t  current_time;
    int send_msg = ISR_MQ_MSG_CONTENT;
    int receive_msg = 0;
    bool isEmpty = false;
    osal_mq_handle_t* local_mq_handle = (osal_mq_handle_t*)arg->additional_arg;
    osal_milli_sec_t time_period_0ms = 0;
    uint32_t reg = TEST_REG_CLEAR_REGISTER;
    uint32_t value = TEST_CLEAR_REGISTER_VALUE;

     /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, reg, value), OSAL_RETURN_OK);

    // Test03_008_008
    // @brief: normal case: call R_OSAL_MqIsEmpty in ISR, queue is not empty
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(*local_mq_handle, time_period_0ms, &send_msg, ISR_MQ_MSG_SIZE), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqIsEmpty(*local_mq_handle, &isEmpty), OSAL_RETURN_OK) << "Test03_008_008 failed";
    EXPECT_EQ(isEmpty, false);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(*local_mq_handle, time_period_0ms, &receive_msg, ISR_MQ_MSG_SIZE), OSAL_RETURN_OK);

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(arg->mq_handle, time_period_0ms, &arg->send_msg, sizeof(arg->send_msg)), OSAL_RETURN_OK);
}
// [mq_test_int_cb1_normal_TC_03_008_008]

// Callback function interrupt context test case
// [mq_test_int_cb1_abnormal_TC_03_003_017]
void mq_test_int_cb1_abnormal_TC_03_003_017(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    st_osal_time_t  current_time;
    int send_msg = ISR_MQ_MSG_CONTENT;
    int receive_msg = 0;
    osal_mq_handle_t* local_mq_handle = (osal_mq_handle_t*)arg->additional_arg;;
    osal_milli_sec_t time_period_0ms = 0;
    uint32_t reg = TEST_REG_CLEAR_REGISTER;
    uint32_t value = TEST_CLEAR_REGISTER_VALUE;

     /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, reg, value), OSAL_RETURN_OK);

    // Test03_003_017
    // @brief: abnormal case: call R_OSAL_MqSendUntilTimeStamp in ISR with full queue
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(*local_mq_handle, time_period_0ms, &send_msg, ISR_MQ_MSG_SIZE), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqSendUntilTimeStamp(*local_mq_handle, &current_time, &send_msg, ISR_MQ_MSG_SIZE), OSAL_RETURN_BUSY) << "Test03_003_017 failed";

    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(*local_mq_handle,time_period_0ms, &receive_msg, ISR_MQ_MSG_SIZE), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, ISR_MQ_MSG_CONTENT);
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(arg->mq_handle, 0, &arg->send_msg, sizeof(arg->send_msg)), OSAL_RETURN_OK);
}
// [mq_test_int_cb1_abnormal_TC_03_003_017]

// Callback function interrupt context test case
// [mq_test_int_cb1_abnormal_TC_03_004_014]
void mq_test_int_cb1_abnormal_TC_03_004_014(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    int send_msg = ISR_MQ_MSG_CONTENT;
    int receive_msg = 0;
    osal_mq_handle_t* local_mq_handle = (osal_mq_handle_t*)arg->additional_arg;;
    osal_milli_sec_t time_period_0ms = 0;
    uint32_t reg = TEST_REG_CLEAR_REGISTER;
    uint32_t value = TEST_CLEAR_REGISTER_VALUE;

     /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, reg, value), OSAL_RETURN_OK);

    // Test03_004_014
    // @brief: abnormal case: call R_OSAL_MqSendForTimePeriod in ISR with full queue
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(*local_mq_handle, time_period_0ms, &send_msg, ISR_MQ_MSG_SIZE), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(*local_mq_handle, time_period_0ms, &send_msg, ISR_MQ_MSG_SIZE), OSAL_RETURN_BUSY) << "Test03_004_014 failed";

    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(*local_mq_handle,time_period_0ms, &receive_msg, ISR_MQ_MSG_SIZE), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, ISR_MQ_MSG_CONTENT);
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(arg->mq_handle, 0, &arg->send_msg, sizeof(arg->send_msg)), OSAL_RETURN_OK);
}
// [mq_test_int_cb1_abnormal_TC_03_004_014]

// Callback function interrupt context test case
// [mq_test_int_cb1_abnormal_TC_03_005_017]
void mq_test_int_cb1_abnormal_TC_03_005_017(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    st_osal_time_t  current_time;
    int receive_msg = 0;
    osal_mq_handle_t* local_mq_handle = (osal_mq_handle_t*)arg->additional_arg;;
    uint32_t reg = TEST_REG_CLEAR_REGISTER;
    uint32_t value = TEST_CLEAR_REGISTER_VALUE;

     /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, reg, value), OSAL_RETURN_OK);

    // Test03_005_017
    // @brief: abnormal case: call R_OSAL_MqReceiveUntilTimeStamp in ISR with empty queue
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReceiveUntilTimeStamp(*local_mq_handle, &current_time, &receive_msg, ISR_MQ_MSG_SIZE), OSAL_RETURN_BUSY) << "Test03_005_017 failed";
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(arg->mq_handle, 0, &arg->send_msg, sizeof(arg->send_msg)), OSAL_RETURN_OK);

}
// [mq_test_int_cb1_abnormal_TC_03_005_017]

// [mq_test_int_cb_TC_03_005_026]
void mq_test_int_cb_TC_03_005_026(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    st_osal_time_t  current_time;
    int receive_msg = 0;
    osal_mq_handle_t* local_mq_handle = (osal_mq_handle_t*)arg->additional_arg;;
    uint32_t reg = TEST_REG_CLEAR_REGISTER;
    uint32_t value = TEST_CLEAR_REGISTER_VALUE;

     /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, reg, value), OSAL_RETURN_OK);

    // Test03_005_026
    // @brief: abnormal case: call R_OSAL_MqReceiveUntilTimeStamp in ISR with full queue
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReceiveUntilTimeStamp(*local_mq_handle, &current_time, &receive_msg, ISR_MQ_MSG_SIZE), OSAL_RETURN_OK) << "Test03_005_026 failed";
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(arg->mq_handle, 0, &arg->send_msg, sizeof(arg->send_msg)), OSAL_RETURN_OK);
}
// [mq_test_int_cb_TC_03_005_026]

// Callback function interrupt context test case
// [mq_test_int_cb1_abnormal_TC_03_006_014]
void mq_test_int_cb1_abnormal_TC_03_006_014(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    int receive_msg = 0;
    osal_mq_handle_t* local_mq_handle = (osal_mq_handle_t*)arg->additional_arg;;
    osal_milli_sec_t time_period_0ms = 0;
    uint32_t reg = TEST_REG_CLEAR_REGISTER;
    uint32_t value = TEST_CLEAR_REGISTER_VALUE;

     /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, reg, value), OSAL_RETURN_OK);

    // Test03_006_014
    // @brief: abnormal case: call R_OSAL_MqReceiveForTimePeriod in ISR with empty queue
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(*local_mq_handle,time_period_0ms, &receive_msg, ISR_MQ_MSG_SIZE), OSAL_RETURN_BUSY) << "Test03_006_014 failed";

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(arg->mq_handle, 0, &arg->send_msg, sizeof(arg->send_msg)), OSAL_RETURN_OK);
}
// [mq_test_int_cb1_abnormal_TC_03_006_014]

// [mq_test_int_cb_TC_03_006_020]
void mq_test_int_cb_TC_03_006_020(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    int receive_msg = 0;
    osal_mq_handle_t* local_mq_handle = (osal_mq_handle_t*)arg->additional_arg;;
    osal_milli_sec_t time_period_0ms = 0;
    uint32_t reg = TEST_REG_CLEAR_REGISTER;
    uint32_t value = TEST_CLEAR_REGISTER_VALUE;

     /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, reg, value), OSAL_RETURN_OK);

    // Test03_006_020
    // @brief: abnormal case: call R_OSAL_MqReceiveForTimePeriod in ISR with empty queue
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(*local_mq_handle,time_period_0ms, &receive_msg, ISR_MQ_MSG_SIZE), OSAL_RETURN_OK) << "Test03_006_020 failed";

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(arg->mq_handle, 0, &arg->send_msg, sizeof(arg->send_msg)), OSAL_RETURN_OK);
}
// [mq_test_int_cb_TC_03_006_020]

class MQApiTest : public OSALTESTS
{
protected:
// [MQApiTest_SetUpTestSuite]
    static void SetUpTestSuite()
    {
        st_osal_mq_config_t config;
        config.max_num_msg = 2;
        config.msg_size    = 4;
        EXPECT_EQ(R_OSAL_MqCreate(&config, 9999, &shared_queue), OSAL_RETURN_OK);
        ASSERT_NE(shared_queue, NULL_PTR);
    }
// [MQApiTest_SetUpTestSuite]

// [MQApiTest_TearDownTestSuite]
    static void TearDownTestSuite() {
        bool isEmpty;

        EXPECT_EQ(R_OSAL_MqIsEmpty(shared_queue,&isEmpty), OSAL_RETURN_OK);
        if(isEmpty)
        EXPECT_EQ(R_OSAL_MqReset(shared_queue), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_MqDelete(shared_queue), OSAL_RETURN_OK);
    }
// [MQApiTest_TearDownTestSuite]

// [MQApiTest_SetUp]
    void SetUp() override{
        EXPECT_EQ(R_OSAL_MqCreate(&sync_mq_config, SYNC_MQ_ID, &sync_mq), OSAL_RETURN_OK);
        EXPECT_NE(sync_mq, OSAL_MQ_HANDLE_INVALID);
    }
// [MQApiTest_SetUp]

// [MQApiTest_TearDown]
    void TearDown() override{
        EXPECT_EQ(R_OSAL_MqReset(shared_queue), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_MqReset(sync_mq), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_MqDelete(sync_mq), OSAL_RETURN_OK);
    }
// [MQApiTest_TearDown]

    static osal_mq_handle_t shared_queue;
    st_osal_mq_config_t mq_config;  
    osal_mq_handle_t    mq_handle;
    int send_msg = 0xFFFF;
    int receive_msg = 0;
    int sync_send_msg = 0xFFFF;
    int sync_receive_msg = 0;
    osal_mq_handle_t sync_mq;
    st_osal_mq_config sync_mq_config = {10, 4};
};

osal_mq_handle_t MQApiTest::shared_queue;

#ifndef SKIP_SECTION_1 
/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqCreate_TC_03_001_001 MQApiTest.MqCreate_TC_03_001_001
 * @subsection MqCreate_TC_03_001_001_tsi Test Suite ID
 * MQApiTest
 * @subsection MqCreate_TC_03_001_001_tci Test Case ID
 * MqCreate_S1_N_TC_03_001_001
 * @subsection MqCreate_TC_03_001_001_rr Related Requirement(s)
 * \#199170
 * @subsection MqCreate_TC_03_001_001_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqCreate_TC_03_001_001_tcs Test Case Summary
 * This test shall verify the functionality of OSAL Create API in Normal behavior.
 * - Test03_001_001: Checking with all valid handle.
 * - Post-process: delete created queue
 * @subsection MqCreate_TC_03_001_001_tpts Test Procedure/Test Steps
 * -# Create new queue with all valid parameter using R_OSAL_MqCreate function. <br>
 * -# Delete that queue.
  * @subsection MqCreate_TC_03_001_001_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK. <br>
 * The return handle is not NULL. <br>
 * -# The return value of R_OSAL_MqDelete is OSAL_RETURN_OK.
 * @subsection MqCreate_TC_03_001_001_testcase Test Case Example
 * @snippet test_mq.cpp MqCreate_TC_03_001_001
 */
// [MqCreate_TC_03_001_001]
TEST_F(MQApiTest, MqCreate_TC_03_001_001)
{
    // Test03_001_001
    // @brief: normal case: all valid handle
    mq_config.max_num_msg = 1;
    mq_config.msg_size = 4;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config, TEST_MQ_ID2, &mq_handle), OSAL_RETURN_OK) << "Test03_001_001 failed";
    EXPECT_NE(mq_handle, NULL_PTR);

    // post-process: delete created queue
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK) << "post-process failed";
    mq_handle = OSAL_MQ_HANDLE_INVALID;
}
// [MqCreate_TC_03_001_001]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqCreate_TC_03_001_002 MQApiTest.MqCreate_TC_03_001_002
 * @subsection MqCreate_TC_03_001_002_tsi Test Suite ID
 * MQApiTest
 * @subsection MqCreate_TC_03_001_002_tci Test Case ID
 * MqCreate_S0_A_TC_03_001_002
 * @subsection MqCreate_TC_03_001_002_rr Related Requirement(s)
 * \#199166, \#199170
 * @subsection MqCreate_TC_03_001_002_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqCreate_TC_03_001_002_tcs Test Case Summary
 * This test shall verify the functionality of OSAL Create API in Abnormal behavior in single thread
 * - Test03_001_002: Checking with p_config is NULL.
 * @subsection MqCreate_TC_03_001_002_tpts Test Procedure/Test Steps
 * -# Initialize the mq_handle with MQ_HANDLE_INITIALIZED_VALUE by using memset.<br>
 * MQ_HANDLE_INITIALIZED_VALUE must be different from OSAL_MQ_HANDLE_INVALID
 * -# Create a queue by function R_OSAL_MqCreate with p_config is NULL.
 * @subsection MqCreate_TC_03_001_002_ereb Expected Result/Behavior
 * -# memset a value for mq_handle successfully
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_PAR.<br>
 * "mq_handle" is OSAL_MQ_HANDLE_INVALID.
 * @subsection MqCreate_TC_03_001_002_testcase Test Case Example
 * @snippet test_mq.cpp MqCreate_TC_03_001_002
 */
// [MqCreate_TC_03_001_002]
TEST_F(MQApiTest, MqCreate_TC_03_001_002)
{
    st_osal_mq_config_t * p_mq_config;

    // Test03_001_002
    // @brief: abnormal case: p_config = NULL
    mq_handle = NULL_PTR;
    p_mq_config = NULL;
    memset(&mq_handle, MQ_HANDLE_INITIALIZED_VALUE, sizeof(mq_handle));
    EXPECT_EQ(R_OSAL_MqCreate(p_mq_config, TEST_MQ_ID2, &mq_handle), OSAL_RETURN_PAR) << "Test03_001_002 failed";
    EXPECT_EQ(mq_handle, NULL_PTR);
}
// [MqCreate_TC_03_001_002]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqCreate_TC_03_001_003 MQApiTest.MqCreate_TC_03_001_003
 * @subsection MqCreate_TC_03_001_003_tsi Test Suite ID
 * MQApiTest
 * @subsection MqCreate_TC_03_001_003_tci Test Case ID
 * MqCreate_S0_A_TC_03_001_003
 * @subsection MqCreate_TC_03_001_003_rr Related Requirement(s)
 * \#199165, \#199170
 * @subsection MqCreate_TC_03_001_003_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqCreate_TC_03_001_003_tcs Test Case Summary
 * This test shall verify the functionality of OSAL Create API in Abnormal behavior in single thread
 * - Test03_001_003: Checking with p_handle is NULL.
 * @subsection MqCreate_TC_03_001_003_tpts Test Procedure/Test Steps
 * -# Create a queue by function R_OSAL_MqCreate with p_handle is NULL.
  * @subsection MqCreate_TC_03_001_003_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_PAR.
 * @subsection MqCreate_TC_03_001_003_testcase Test Case Example
 * @snippet test_mq.cpp MqCreate_TC_03_001_003
 */
// [MqCreate_TC_03_001_003]
TEST_F(MQApiTest, MqCreate_TC_03_001_003)
{
    osal_mq_handle_t * p_mq_handle;
    int mq_id;

    // Test03_001_003
    // @brief: abnormal case: p_handle = NULL
    mq_id = TEST_MQ_ID2;
    mq_config.max_num_msg = 1;
    mq_config.msg_size = 4;
    p_mq_handle = NULL;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config, mq_id, p_mq_handle), OSAL_RETURN_PAR) << "Test03_001_003 failed";

}
// [MqCreate_TC_03_001_003]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqCreate_TC_03_001_004 MQApiTest.MqCreate_TC_03_001_004
 * @subsection MqCreate_TC_03_001_004_tsi Test Suite ID
 * MQApiTest
 * @subsection MqCreate_TC_03_001_004_tci Test Case ID
 * MqCreate_S2_A_TC_03_001_004
 * @subsection MqCreate_TC_03_001_004_rr Related Requirement(s)
 * \#199170
 * @subsection MqCreate_TC_03_001_004_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqCreate_TC_03_001_004_tcs Test Case Summary
 * This test shall verify the functionality of OSAL Create API in Abnormal behavior in single thread
 * - Test03_001_004: Create with the same input ID twice.
 * - Post-process: delete created queue
 * @subsection MqCreate_TC_03_001_004_tpts Test Procedure/Test Steps
 * -# Create a queue by function R_OSAL_MqCreate with valid parameter. <br>
 * -# Create another queue by function R_OSAL_MqCreate with the same input ID. <br>
 * -# Delete created queue.
 * @subsection MqCreate_TC_03_001_004_ereb Expected Result/Behavior
 * -# The return value of the first R_OSAL_MqCreate is OSAL_RETURN_OK.<br>
 * The return handle is not NULL.
 * -# The return value of the second called R_OSAL_MqCreate is OSAL_RETURN_BUSY. <br>
 * The return value handle of the second called R_OSAL_MqCreate is NULL_PTR.
 * -# The return value of R_OSAL_MqDelete is OSAL_RETURN_OK.
 * @subsection MqCreate_TC_03_001_004_testcase Test Case Example
 * @snippet test_mq.cpp MqCreate_TC_03_001_004
 */
// [MqCreate_TC_03_001_004]
TEST_F(MQApiTest, MqCreate_TC_03_001_004)
{
    int mq_id;
    osal_mq_handle_t mq_handle1 = OSAL_MQ_HANDLE_INVALID;

    // Test03_001_004
    // @brief: abnormal case: Create same mq_id twice
    mq_config.max_num_msg = 1;
    mq_config.msg_size = 4;

    mq_id = TEST_MQ_ID2;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config, mq_id, &mq_handle), OSAL_RETURN_OK);
    EXPECT_NE(mq_handle, NULL_PTR);
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config, mq_id, &mq_handle1), OSAL_RETURN_BUSY) << "Test03_001_004 failed";
    EXPECT_EQ(mq_handle1, NULL_PTR);

    // post-process: delete created queue
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK) << "post-process failed";
    mq_handle = OSAL_MQ_HANDLE_INVALID;
}
// [MqCreate_TC_03_001_004]

/**
 * @page MQ_TEST MQ API Test
 * @section OsalApiStateTest_MqCreate_TC_03_001_005 OsalApiStateTest.MqCreate_TC_03_001_005
 * @subsection MqCreate_TC_03_001_005_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection MqCreate_TC_03_001_005_tci Test Case ID
 * MqCreate_S2_A_TC_03_001_005
 * @subsection MqCreate_TC_03_001_005_rr Related Requirement(s)
 * \#199170
 * @subsection MqCreate_TC_03_001_005_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection MqCreate_TC_03_001_005_tcs Test Case Summary
 * This test shall checking the state of all Message queue API in Abnormal behavior in single thread
 * - Test03_001_005: checking state of R_OSAL_MqCreate.
 * @subsection MqCreate_TC_03_001_005_tpts Test Procedure/Test Steps
 * -# Initialize the mq_handle with MQ_HANDLE_INITIALIZED_VALUE by using memset.<br>
 * MQ_HANDLE_INITIALIZED_VALUE must be different from OSAL_MQ_HANDLE_INVALID
 * -# Call function R_OSAL_MqCreate with valid parameter.
 * @subsection MqCreate_TC_03_001_005_ereb Expected Result/Behavior
 * -# memset a value for mq_handle successfully
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_STATE.<br>
 * "mq_handle" is OSAL_MQ_HANDLE_INVALID.
 * @subsection MqCreate_TC_03_001_005_testcase Test Case Example
 * @snippet test_mq.cpp MqCreate_TC_03_001_005
 */
// [MqCreate_TC_03_001_005]
TEST_F(OsalApiStateTest, MqCreate_TC_03_001_005)
{
    st_osal_mq_config mq_config;
    osal_mq_id_t mq_Id = TEST_MQ_ID2;
    osal_mq_handle_t mq_handle = OSAL_MQ_HANDLE_INVALID;
    memset(&mq_handle, MQ_HANDLE_INITIALIZED_VALUE, sizeof(mq_handle));

    // Test03_001_005
    // @brief: abnormal case: state transition error of R_OSAL_MqCreate
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config, mq_Id, &mq_handle), OSAL_RETURN_STATE) << "Test03_001_005 failed";
    EXPECT_EQ(mq_handle, OSAL_MQ_HANDLE_INVALID) << "Test03_001_005 failed";

}
// [MqCreate_TC_03_001_005]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqCreate_TC_03_001_006 MQApiTest.MqCreate_TC_03_001_006
 * @subsection MqCreate_TC_03_001_006_tsi Test Suite ID
 * MQApiTest
 * @subsection MqCreate_TC_03_001_006_tci Test Case ID
 * MqCreate_S0_A_TC_03_001_006
 * @subsection MqCreate_TC_03_001_006_rr Related Requirement(s)
 * \#199165, \#199170
 * @subsection MqCreate_TC_03_001_006_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqCreate_TC_03_001_006_tcs Test Case Summary
 * This test shall verify the functionality of OSAL Create API in Abnormal behavior in single thread
 * - Test03_001_006: create message queue with max_num_msg is 0.
 * @subsection MqCreate_TC_03_001_006_tpts Test Procedure/Test Steps
 * -# Initialize the mq_handle with MQ_HANDLE_INITIALIZED_VALUE by using memset.<br>
 * MQ_HANDLE_INITIALIZED_VALUE must be different from OSAL_MQ_HANDLE_INVALID
 * -# Create message queue by calling R_OSAL_MqCreate with the argument as below:
 *      - max_num_msg is 0
 *      - msg_size is 4
 *      - mq_handle is MQ_HANDLE_INITIALIZED_VALUE
 * @subsection MqCreate_TC_03_001_006_ereb Expected Result/Behavior
 * -# memset a value for mq_handle successfully
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_PAR.<br>
 * "mq_handle" is OSAL_MQ_HANDLE_INVALID.
 * @subsection MqCreate_TC_03_001_006_testcase Test Case Example
 * @snippet test_mq.cpp MqCreate_TC_03_001_006
 */
// [MqCreate_TC_03_001_006]
TEST_F(MQApiTest, MqCreate_TC_03_001_006)
{
    // Test03_001_006
    // @brief: abnormal case: max_num_msg = 0
    memset(&mq_handle, MQ_HANDLE_INITIALIZED_VALUE, sizeof(mq_handle));
    mq_config.max_num_msg = 0;
    mq_config.msg_size = 4;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config, TEST_MQ_ID2, &mq_handle), OSAL_RETURN_PAR) << "Test03_001_006 failed";
    EXPECT_EQ(mq_handle, OSAL_MQ_HANDLE_INVALID) << "Test03_001_006 failed";
}
// [MqCreate_TC_03_001_006]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqCreate_TC_03_001_007 MQApiTest.MqCreate_TC_03_001_007
 * @subsection MqCreate_TC_03_001_007_tsi Test Suite ID
 * MQApiTest
 * @subsection MqCreate_TC_03_001_007_tci Test Case ID
 * MqCreate_S0_A_TC_03_001_007
 * @subsection MqCreate_TC_03_001_007_rr Related Requirement(s)
 * \#199165, \#199170
 * @subsection MqCreate_TC_03_001_007_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqCreate_TC_03_001_007_tcs Test Case Summary
 * This test shall verify the functionality of OSAL Create API in Abnormal behavior in single thread
 * - Test03_001_007: create message queue with msg_size is 0.
 * @subsection MqCreate_TC_03_001_007_tpts Test Procedure/Test Steps
 * -# Initialize the mq_handle with MQ_HANDLE_INITIALIZED_VALUE by using memset.<br>
 * MQ_HANDLE_INITIALIZED_VALUE must be different from OSAL_MQ_HANDLE_INVALID
 * -# Create message queue by calling R_OSAL_MqCreate with the argument as below:
 *      - max_num_msg is 1
 *      - msg_size is 0
 *      - mq_handle is MQ_HANDLE_INITIALIZED_VALUE
 * @subsection MqCreate_TC_03_001_007_ereb Expected Result/Behavior
 * -# memset a value for mq_handle successfully
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_PAR.<br>
 * "mq_handle" is OSAL_MQ_HANDLE_INVALID.
 * @subsection MqCreate_TC_03_001_007_testcase Test Case Example
 * @snippet test_mq.cpp MqCreate_TC_03_001_007
 */
// [MqCreate_TC_03_001_007]
TEST_F(MQApiTest, MqCreate_TC_03_001_007)
{
    // Test03_001_007
    // @brief: abnormal case: msg_size = 0
    memset(&mq_handle, MQ_HANDLE_INITIALIZED_VALUE, sizeof(mq_handle));
    mq_config.max_num_msg = 1;
    mq_config.msg_size = 0;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config, TEST_MQ_ID2, &mq_handle), OSAL_RETURN_PAR) << "Test03_001_007 failed";
    EXPECT_EQ(mq_handle, OSAL_MQ_HANDLE_INVALID) << "Test03_001_007 failed";
}
// [MqCreate_TC_03_001_007]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqCreate_TC_03_001_008 MQApiTest.MqCreate_TC_03_001_008
 * @subsection MqCreate_TC_03_001_008_tsi Test Suite ID
 * MQApiTest
 * @subsection MqCreate_TC_03_001_008_tci Test Case ID
 * MqCreate_S0_A_TC_03_001_008
 * @subsection MqCreate_TC_03_001_008_rr Related Requirement(s)
 * \#199165, \#199170
 * @subsection MqCreate_TC_03_001_008_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqCreate_TC_03_001_008_tcs Test Case Summary
 * This test shall verify the functionality of OSAL Create API in Abnormal behavior in single thread
 * - Test03_001_008: create message queue with max_num_msg and msg_size is 0.
 * @subsection MqCreate_TC_03_001_008_tpts Test Procedure/Test Steps
 * -# Initialize the mq_handle with MQ_HANDLE_INITIALIZED_VALUE by using memset.<br>
 * MQ_HANDLE_INITIALIZED_VALUE must be different from OSAL_MQ_HANDLE_INVALID
 * -# Create message queue by calling R_OSAL_MqCreate with the argument as below:
 *      - max_num_msg is 0
 *      - msg_size is 0
 *      - mq_handle is MQ_HANDLE_INITIALIZED_VALUE
 * @subsection MqCreate_TC_03_001_008_ereb Expected Result/Behavior
 * -# memset a value for mq_handle successfully
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_PAR.<br>
 * "mq_handle" is OSAL_MQ_HANDLE_INVALID.
 * @subsection MqCreate_TC_03_001_008_testcase Test Case Example
 * @snippet test_mq.cpp MqCreate_TC_03_001_008
 */
// [MqCreate_TC_03_001_008]
TEST_F(MQApiTest, MqCreate_TC_03_001_008)
{
    // Test03_001_008
    // @brief: abnormal case: msg_size = 0 and max_num_msg = 0
    memset(&mq_handle, MQ_HANDLE_INITIALIZED_VALUE, sizeof(mq_handle));
    mq_config.max_num_msg = 0;
    mq_config.msg_size = 0;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config, TEST_MQ_ID2, &mq_handle), OSAL_RETURN_PAR) << "Test03_001_008 failed";
    EXPECT_EQ(mq_handle, OSAL_MQ_HANDLE_INVALID) << "Test03_001_008 failed";
}
// [MqCreate_TC_03_001_008]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqDelete_TC_03_002_001 MQApiTest.MqDelete_TC_03_002_001
 * @subsection MqDelete_TC_03_002_001_tsi Test Suite ID
 * MQApiTest
 * @subsection MqDelete_TC_03_002_001_tci Test Case ID
 * MqDelete_S1_N_TC_03_002_001
 * @subsection MqDelete_TC_03_002_001_rr Related Requirement(s)
 * \#199177
 * @subsection MqDelete_TC_03_002_001_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqDelete_TC_03_002_001_tcs Test Case Summary
 * This test shall verify the functionality of OSAL Delete API in Normal behavior in single thread
 * - Pre-process: create a queue.
 * - Test03_002_001:Checking with valid input parameter.
 * @subsection MqDelete_TC_03_002_001_tpts Test Procedure/Test Steps
 * -# Create new queue with all valid parameter using R_OSAL_MqCreate function.
 * -# Delete that queue.
 * @subsection MqDelete_TC_03_002_001_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK. <br>
 * The return handle after creating is not NULL_PTR.
 * -# The return value of R_OSAL_MqDelete is OSAL_RETURN_OK.
 * @subsection MqDelete_TC_03_002_001_testcase Test Case Example
 * @snippet test_mq.cpp MqDelete_TC_03_002_001
 */
// [MqDelete_TC_03_002_001]
TEST_F(MQApiTest, MqDelete_TC_03_002_001)
{
    //pre-process: create a handle
    mq_config.max_num_msg = 1;
    mq_config.msg_size = 4;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config, TEST_MQ_ID2, &mq_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(mq_handle, NULL_PTR) << "pre-process failed";

    // Test03_002_001
    // @brief: normal case: all valid handle
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK) << "Test03_002_001 failed";
    mq_handle = OSAL_MQ_HANDLE_INVALID;
}
// [MqDelete_TC_03_002_001]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqDelete_TC_03_002_002 MQApiTest.MqDelete_TC_03_002_002
 * @subsection MqDelete_TC_03_002_002_tsi Test Suite ID
 * MQApiTest
 * @subsection MqDelete_TC_03_002_002_tci Test Case ID
 * MqDelete_S0_A_TC_03_002_002
 * @subsection MqDelete_TC_03_002_002_rr Related Requirement(s)
 * \#199151, \#199165, \#199177
 * @subsection MqDelete_TC_03_002_002_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqDelete_TC_03_002_002_tcs Test Case Summary
 * This test shall verify the functionality of OSAL Delete API in Abnormal behavior in single thread
 * - Test03_002_002: Checking with handle NULL.
 * @subsection MqDelete_TC_03_002_002_tpts Test Procedure/Test Steps
 * -# Delete a queue by R_OSAL_MqDelete with input handle is NULL.
 * @subsection MqDelete_TC_03_002_002_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqDelete is OSAL_RETURN_HANDLE.
 * @subsection MqDelete_TC_03_002_002_testcase Test Case Example
 * @snippet test_mq.cpp MqDelete_TC_03_002_002
 */
// [MqDelete_TC_03_002_002]
TEST_F(MQApiTest, MqDelete_TC_03_002_002)
{
    // Test03_002_002
    // @brief: abnormal case: handle = NULL
    EXPECT_EQ(R_OSAL_MqDelete(OSAL_MQ_HANDLE_INVALID), OSAL_RETURN_HANDLE) << "Test03_002_002 failed";
}
// [MqDelete_TC_03_002_002]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqDelete_TC_03_002_003 MQApiTest.MqDelete_TC_03_002_003
 * @subsection MqDelete_TC_03_002_003_tsi Test Suite ID
 * MQApiTest
 * @subsection MqDelete_TC_03_002_003_tci Test Case ID
 * MqDelete_S2_A_TC_03_002_003
 * @subsection MqDelete_TC_03_002_003_rr Related Requirement(s)
 * \#199177
 * @subsection MqDelete_TC_03_002_003_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqDelete_TC_03_002_003_tcs Test Case Summary
 * This test shall verify the functionality of OSAL Delete API in Abnormal behavior in single thread
 * - Pre-process: Create a queue
 * - Test03_002_003: Checking with a not empty queue.
 * @subsection MqDelete_TC_03_002_003_tpts Test Procedure/Test Steps
 * -# Create new queue with all valid parameter using R_OSAL_MqCreate function.
 * -# Send a message to queue by R_OSAL_MqSendForTimePeriod.
 * -# Delete that queue by R_OSAL_MqDelete.
 * -# Receive a message from the queue by R_OSAL_MqReceiveForTimePeriod.
 * -# Delete that queue by R_OSAL_MqDelete.
 * @subsection MqDelete_TC_03_002_003_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK. <br>
 * The return handle after creating is not NULL_PTR.
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of the first called R_OSAL_MqDelete is OSAL_RETURN_BUSY.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of the second called R_OSAL_MqDelete is OSAL_RETURN_OK.
 * @subsection MqDelete_TC_03_002_003_testcase Test Case Example
 * @snippet test_mq.cpp MqDelete_TC_03_002_003
 */
// [MqDelete_TC_03_002_003]
TEST_F(MQApiTest, MqDelete_TC_03_002_003)
{
    osal_milli_sec_t time_period_10ms = 10;

    //pre-process: create a queue
    mq_config.max_num_msg = 1;
    mq_config.msg_size = 4;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config, TEST_MQ_ID2, &mq_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(mq_handle, NULL_PTR) << "pre-process failed";

    // Test03_002_003
    // @brief: abnormal case: the queue is not empty
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(mq_handle, time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_BUSY) << "Test03_002_003 failed";
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(mq_handle, time_period_10ms,  &receive_msg, sizeof(receive_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK);
    mq_handle = OSAL_MQ_HANDLE_INVALID;
}
// [MqDelete_TC_03_002_003]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqDelete_TC_03_002_004 MQApiTest.MqDelete_TC_03_002_004
 * @subsection MqDelete_TC_03_002_004_tsi Test Suite ID
 * MQApiTest
 * @subsection MqDelete_TC_03_002_004_tci Test Case ID
 * MqDelete_S2_A_TC_03_002_004
 * @subsection MqDelete_TC_03_002_004_rr Related Requirement(s)
 * \#199177
 * @subsection MqDelete_TC_03_002_004_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqDelete_TC_03_002_004_tcs Test Case Summary
 * This test shall verify the functionality of OSAL Delete API in Abnormal behavior in single thread
 * - Test03_002_004: Delete a deleted handle.
 * @subsection MqDelete_TC_03_002_004_tpts Test Procedure/Test Steps
 * -# Create new queue with all valid parameter using R_OSAL_MqCreate function.
 * -# Delete that queue.
 * -# Delete that queue again.
 * @subsection MqDelete_TC_03_002_004_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK. <br>
 * The return handle after creating is not NULL_PTR.
 * -# The return value of the first called R_OSAL_MqDelete is OSAL_RETURN_OK.
 * -# The return value of the second called R_OSAL_MqDelete is OSAL_RETURN_HANDLE.
 * @subsection MqDelete_TC_03_002_004_testcase Test Case Example
 * @snippet test_mq.cpp MqDelete_TC_03_002_004
 */
// [MqDelete_TC_03_002_004]
TEST_F(MQApiTest, MqDelete_TC_03_002_004)
{
    // Test03_002_004
    // @brief: abnormal case: delete a deleted handle
    mq_config.max_num_msg = 1;
    mq_config.msg_size = 4;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config, TEST_MQ_ID2, &mq_handle), OSAL_RETURN_OK);
    EXPECT_NE(mq_handle, NULL_PTR);
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_HANDLE) << "Test03_002_004 failed";
    mq_handle = OSAL_MQ_HANDLE_INVALID;
}
// [MqDelete_TC_03_002_004]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqDelete_TC_03_002_005 MQApiTest.MqDelete_TC_03_002_005
 * @subsection MqDelete_TC_03_002_005_tsi Test Suite ID
 * MQApiTest
 * @subsection MqDelete_TC_03_002_005_tci Test Case ID
 * MqDeleteTC_S2_A_003_002_005
 * @subsection MqDelete_TC_03_002_005_rr Related Requirement(s)
 * \#199177, \#199181
 * @subsection MqDelete_TC_03_002_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Synchronization queue was created.
 * @subsection MqDelete_TC_03_002_005_tcs Test Case Summary
 * This test shall verify the functionality of OSAL Delete API in Abnormal behavior in multi thread
 * - Pre-process: create a queue
 * - Test03_002_005: delete message queue while another thread is receiving the message on the same queue.
 * - Post-process: delete created queue
 * @subsection MqDelete_TC_03_002_005_tpts Test Procedure/Test Steps
 * -# Create new queue with all valid parameter using R_OSAL_MqCreate function.
 * -# Create 1st thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *          -# task_name is "task03_002_005_01"
 *          -# func is osal_test_thread_receive_timeperiod function with functionality:
 *              - Get the synchronization queue config by using R_OSAL_MqGetConfig
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *              - Receive message from the queue by R_OSAL_MqReceiveForTimePeriod.
 *          -# userarg is receive_timeperiod_arg with:
 *              - mq_handle
 *              - time is 20000ms
 *              - sync_mq is handle of synchronization queue
 *              - sync_send_msg is the sending message for synchronization purpose
 *              - needToSync is true
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1
 *      - Address of thread handle.
 * -# Receive a message from synchronization queue by R_OSAL_MqReceiveForTimePeriod.
 * -# Sleep to make sure that osal_test_thread_receive_timeperiod has already been blocked by R_OSAL_MqReceiveForTimePeriod
 * -# Create 2nd thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *          -# task_name is "task03_002_005_02"
 *          -# func is osal_test_thread_delete_mq function with functionality:
 *              - Delete message from the queue by R_OSAL_MqDelete.
 *          -# userarg is delete_mq_arg with:
 *              - mq_handle
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2
 *      - Address of thread handle.
 * -# Calling R_OSAL_ThreadJoin to joining the 1st thread.
 * -# Calling R_OSAL_ThreadJoin to joining the 2nd thread.
 * -# Calling R_OSAL_MqDelete to delete the created queue
 * @subsection MqDelete_TC_03_002_005_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK. <br>
 * The return handle is not NULL. <br>
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with 1st thread.<br>
 * In osal_test_thread_receive_timeperiod function: <br>
 *  - R_OSAL_MqGetConfig return OSAL_RETURN_OK.
 *  - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_TIME.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_ThreadSleepForTimePeriod is OSAL_RETURN_OK.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with 2nd thread.<br>
 * In osal_test_thread_delete_mq function: <br>
 *  - R_OSAL_MqDelete return OSAL_RETURN_BUSY.
 * -# The return value of the R_OSAL_ThreadJoin is OSAL_RETURN_OK with 1st thread.<br>
 * "return_value1" is OSAL_RETURN_TIME.
 * -# The return value of the R_OSAL_ThreadJoin is OSAL_RETURN_OK with 2nd thread.<br>
 * "return_value2" is OSAL_RETURN_BUSY.
 * -# R_OSAL_MqDelete return OSAL_RETURN_OK.
 * @subsection MqDelete_TC_03_002_005_testcase Test Case Example
 * @snippet test_mq.cpp MqDelete_TC_03_002_005
 * @snippet test_mq.cpp osal_test_thread_receive_timeperiod
 * @snippet test_mq.cpp osal_test_thread_delete_mq
 */
// [MqDelete_TC_03_002_005]
TEST_F(MQApiTest, MqDelete_TC_03_002_005)
{
    osal_thread_handle_t thread_handle1 = OSAL_THREAD_HANDLE_INVALID;
    osal_thread_handle_t thread_handle2 = OSAL_THREAD_HANDLE_INVALID;
    st_osal_thread_config_t config1;
    st_osal_thread_config_t config2;
    int64_t return_value1 = 0;
    int64_t return_value2 = 0;
    st_arg_mq_receive_timeperiod_t receive_timeperiod_arg;
    osal_mq_handle_t delete_mq_handle;
    osal_milli_sec_t time_period_1000ms = 1000;

    //pre-process: create a queue
    mq_config.max_num_msg = 1;
    mq_config.msg_size = 4;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config, TEST_MQ_ID2, &mq_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(mq_handle, NULL_PTR) << "pre-process failed";

    // Test03_002_005
    // @brief: abnormal case: delete message queue while another thread is receiving the message on the same queue
    receive_timeperiod_arg.mq_handle = mq_handle;
    receive_timeperiod_arg.time = 10000;
    receive_timeperiod_arg.sync_mq = sync_mq;
    receive_timeperiod_arg.sync_send_msg = sync_send_msg;
    receive_timeperiod_arg.needToSync = true;
    config1.func       = osal_test_thread_receive_timeperiod;
    config1.userarg    = (void *)&receive_timeperiod_arg;
    config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config1.task_name  = "task03_002_005_01";
    config1.stack_size = THREAD_STACK_SIZE;

    delete_mq_handle = mq_handle;
    config2.func       = osal_test_thread_delete_mq;
    config2.userarg    = (void *)&delete_mq_handle;
    config2.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config2.task_name  = "task03_002_005_02";
    config2.stack_size = THREAD_STACK_SIZE;

    EXPECT_EQ(R_OSAL_ThreadCreate(&config1, TEST_THREAD_ID1, &thread_handle1),OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &sync_receive_msg, sync_mq_config.msg_size), OSAL_RETURN_OK);
    EXPECT_EQ(sync_receive_msg, sync_send_msg);
    EXPECT_EQ(R_OSAL_ThreadSleepForTimePeriod(time_period_1000ms), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThreadCreate(&config2, TEST_THREAD_ID2, &thread_handle2),OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &return_value1),OSAL_RETURN_OK);
    EXPECT_EQ(return_value1, OSAL_RETURN_TIME) << "Test03_002_005 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle2, &return_value2),OSAL_RETURN_OK);
    EXPECT_EQ(return_value2, OSAL_RETURN_BUSY) << "Test03_002_005 failed";

    //post-process: delete the queue
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK) << "post-process failed";
}
// [MqDelete_TC_03_002_005]

/**
 * @page MQ_TEST MQ API Test
 * @section OsalApiStateTest_MqDelete_TC_03_002_006 OsalApiStateTest.MqDelete_TC_03_002_006
 * @subsection MqDelete_TC_03_002_006_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection MqDelete_TC_03_002_006_tci Test Case ID
 * MqDelete_S2_A_TC_03_002_006
 * @subsection MqDelete_TC_03_002_006_rr Related Requirement(s)
 * \#199177
 * @subsection MqDelete_TC_03_002_006_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection MqDelete_TC_03_002_006_tcs Test Case Summary
 * This test shall checking the state of all Message queue API in Abnormal behavior in single thread
 * - Test03_002_006: checking state of R_OSAL_MqDelete.
 * @subsection MqDelete_TC_03_002_006_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_MqDelete with valid paramter.
 * @subsection MqDelete_TC_03_002_006_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqDelete is OSAL_RETURN_STATE. 
 * @subsection MqDelete_TC_03_002_006_testcase Test Case Example
 * @snippet test_mq.cpp MqDelete_TC_03_002_006
 */
// [MqDelete_TC_03_002_006]
TEST_F(OsalApiStateTest, MqDelete_TC_03_002_006)
{
    osal_mq_handle_t mq_handle = OSAL_MQ_HANDLE_INVALID;

    // Test03_002_006
    // @brief: abnormal case: state transition error of R_OSAL_MqDelete
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_STATE) << "Test03_002_006 failed";
}
// [MqDelete_TC_03_002_006]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqDelete_TC_03_002_007 MQApiTest.MqDelete_TC_03_002_007
 * @subsection MqDelete_TC_03_002_007_tsi Test Suite ID
 * MQApiTest
 * @subsection MqDelete_TC_03_002_007_tci Test Case ID
 * MqDelete_S1_N_TC_03_002_007
 * @subsection MqDelete_TC_03_002_007_rr Related Requirement(s)
 * \#199177
 * @subsection MqDelete_TC_03_002_007_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqDelete_TC_03_002_007_tcs Test Case Summary
 * This test shall verify the functionality of OSAL Delete API in Normal behavior in single thread
 * - Test03_002_007: Confirm the MqDelete with the sequence call below:<br>
 *      -# MqSend (success)
 *      -# MqReceive (success)
 *      -# MqDelete
 * @subsection MqDelete_TC_03_002_007_tpts Test Procedure/Test Steps
 * -# Create new queue using R_OSAL_MqCreate function with max_num_msg is 1, msg_size is 4.
 * -# Get the queue config by calling R_OSAL_MqGetConfig.
 * -# Send a message to the queue by calling R_OSAL_MqSendForTimePeriod with all valid parameter.
 * -# Receive a message from the queue by calling R_OSAL_MqReceiveForTimePeriod with all valid parameter.
 * -# Delete the message queue by calling R_OSAL_MqDelete
 * @subsection MqDelete_TC_03_002_007_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK. <br>
 * The return handle after creating is not NULL_PTR.
 * -# The return value of R_OSAL_MqGetConfig is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK. <br>
 * The received message is the same as the sent message.
 * -# The return value of R_OSAL_MqDelete is OSAL_RETURN_OK.
 * @subsection MqDelete_TC_03_002_007_testcase Test Case Example
 * @snippet test_mq.cpp MqDelete_TC_03_002_007
 */
// [MqDelete_TC_03_002_007]
TEST_F(MQApiTest, MqDelete_TC_03_002_007)
{
    osal_mq_id_t mq_id;
    osal_milli_sec_t time_period_10ms = 10;

    // Test03_002_007
    // @brief: normal case: send -> receive-> delete
    mq_id = TEST_MQ_ID2;
    mq_config.max_num_msg = 1;
    mq_config.msg_size = 4;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config,mq_id, &mq_handle), OSAL_RETURN_OK) << "Test03_002_007 failed";
    EXPECT_NE(mq_handle, NULL_PTR) << "Test03_002_007 failed";
    EXPECT_EQ(R_OSAL_MqGetConfig(mq_handle, &mq_config), OSAL_RETURN_OK) << "Test03_002_007 failed";
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(mq_handle, time_period_10ms, &send_msg, mq_config.msg_size), OSAL_RETURN_OK) << "Test03_002_007 failed";
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(mq_handle, time_period_10ms, &receive_msg, mq_config.msg_size), OSAL_RETURN_OK) << "Test03_002_007 failed";
    EXPECT_EQ(receive_msg, send_msg) << "Test03_002_007 failed";
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK) << "Test03_002_007 failed";
    mq_handle = OSAL_MQ_HANDLE_INVALID;
}
// [MqDelete_TC_03_002_007]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqDelete_TC_03_002_008 MQApiTest.MqDelete_TC_03_002_008
 * @subsection MqDelete_TC_03_002_008_tsi Test Suite ID
 * MQApiTest
 * @subsection MqDelete_TC_03_002_008_tci Test Case ID
 * MqDelete_S1_N_TC_03_002_008
 * @subsection MqDelete_TC_03_002_008_rr Related Requirement(s)
 * \#199177
 * @subsection MqDelete_TC_03_002_008_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqDelete_TC_03_002_008_tcs Test Case Summary
 * This test shall verify the functionality of OSAL Delete API in Normal behavior in single thread
 * - Test03_002_008: Confirm the MqDelete with the sequence call below:<br>
 *      -# MqReceive (timeout)
 *      -# MqDelete
 * @subsection MqDelete_TC_03_002_008_tpts Test Procedure/Test Steps
 * -# Create new queue using R_OSAL_MqCreate function with max_num_msg is 1, msg_size is 4.
 * -# Get the queue config by calling R_OSAL_MqGetConfig.
 * -# Receive a message from the queue by calling R_OSAL_MqReceiveForTimePeriod with all valid parameter.
 * -# Delete the message queue by calling R_OSAL_MqDelete
 * @subsection MqDelete_TC_03_002_008_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK. <br>
 * The return handle after creating is not NULL_PTR.
 * -# The return value of R_OSAL_MqGetConfig is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_TIME.
 * -# The return value of R_OSAL_MqDelete is OSAL_RETURN_OK.
 * @subsection MqDelete_TC_03_002_008_testcase Test Case Example
 * @snippet test_mq.cpp MqDelete_TC_03_002_008
 */
// [MqDelete_TC_03_002_008]
TEST_F(MQApiTest, MqDelete_TC_03_002_008)
{
    osal_mq_id_t mq_id;
    osal_milli_sec_t time_period_10ms = 10;

    // Test03_002_008
    // @brief: normal case: receive -> timeout -> delete
    mq_id = TEST_MQ_ID2;
    mq_config.max_num_msg = 1;
    mq_config.msg_size = 4;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config,mq_id, &mq_handle), OSAL_RETURN_OK) << "Test03_002_008 failed";
    EXPECT_NE(mq_handle, NULL_PTR) << "Test03_002_008 failed";
    EXPECT_EQ(R_OSAL_MqGetConfig(mq_handle, &mq_config), OSAL_RETURN_OK) << "Test03_002_008 failed";
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(mq_handle, time_period_10ms, &receive_msg, mq_config.msg_size), OSAL_RETURN_TIME) << "Test03_002_008 failed";
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK) << "Test03_002_008 failed";
    mq_handle = OSAL_MQ_HANDLE_INVALID;
}
// [MqDelete_TC_03_002_008]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqDelete_TC_03_002_009 MQApiTest.MqDelete_TC_03_002_009
 * @subsection MqDelete_TC_03_002_009_tsi Test Suite ID
 * MQApiTest
 * @subsection MqDelete_TC_03_002_009_tci Test Case ID
 * MqDelete_S1_N_TC_03_002_009
 * @subsection MqDelete_TC_03_002_009_rr Related Requirement(s)
 * \#199177
 * @subsection MqDelete_TC_03_002_009_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqDelete_TC_03_002_009_tcs Test Case Summary
 * This test shall verify the functionality of OSAL Delete API in Normal behavior in single thread
 * - Test03_002_009: Confirm the MqDelete with the sequence call below:<br>
 *      -# MqSend (success)
 *      -# MqSend (timeout)
 *      -# MqReset (success)
 *      -# MqDelete
 * @subsection MqDelete_TC_03_002_009_tpts Test Procedure/Test Steps
 * -# Create new queue using R_OSAL_MqCreate function with max_num_msg is 1, msg_size is 4.
 * -# Get the queue config by calling R_OSAL_MqGetConfig.
 * -# Send the 1st message to the queue by calling R_OSAL_MqSendForTimePeriod with all valid parameter.
 * -# Send the 2nd message to the queue by calling R_OSAL_MqSendForTimePeriod with all valid parameter.
 * -# Reset the queue by calling R_OSAL_MqReset.
 * -# Delete the message queue by calling R_OSAL_MqDelete.
 * @subsection MqDelete_TC_03_002_009_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK. <br>
 * The return handle after creating is not NULL_PTR.
 * -# The return value of R_OSAL_MqGetConfig is OSAL_RETURN_OK.
 * -# The return value of the 1st R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of the 2nd R_OSAL_MqSendForTimePeriod is OSAL_RETURN_TIME.
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqDelete is OSAL_RETURN_OK.
 * @subsection MqDelete_TC_03_002_009_testcase Test Case Example
 * @snippet test_mq.cpp MqDelete_TC_03_002_009
 */
// [MqDelete_TC_03_002_009]
TEST_F(MQApiTest, MqDelete_TC_03_002_009)
{
    osal_mq_id_t mq_id;
    osal_milli_sec_t time_period_10ms = 10;

    // Test03_002_009
    // @brief: normal case: send -> timeout-> delete
    mq_id = TEST_MQ_ID2;
    mq_config.max_num_msg = 1;
    mq_config.msg_size = 4;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config,mq_id, &mq_handle), OSAL_RETURN_OK) << "Test03_002_009 failed";
    EXPECT_NE(mq_handle, NULL_PTR) << "Test03_002_009 failed";
    EXPECT_EQ(R_OSAL_MqGetConfig(mq_handle, &mq_config), OSAL_RETURN_OK) << "Test03_002_009 failed";
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(mq_handle, time_period_10ms, &send_msg, mq_config.msg_size), OSAL_RETURN_OK) << "Test03_002_009 failed";
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(mq_handle, time_period_10ms, &send_msg, mq_config.msg_size), OSAL_RETURN_TIME) << "Test03_002_009 failed";
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK) << "Test03_002_009 failed";
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK) << "Test03_002_009 failed";
    mq_handle = OSAL_MQ_HANDLE_INVALID;
}
// [MqDelete_TC_03_002_009]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendUntilTimeStamp_TC_03_003_001 MQApiTest.MqSendUntilTimeStamp_TC_03_003_001
 * @subsection MqSendUntilTimeStamp_TC_03_003_001_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendUntilTimeStamp_TC_03_003_001_tci Test Case ID
 * MqSendUntilTimeStamp_S1_N_TC_03_003_001
 * @subsection MqSendUntilTimeStamp_TC_03_003_001_rr Related Requirement(s)
 * \#199172
 * @subsection MqSendUntilTimeStamp_TC_03_003_001_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqSendUntilTimeStamp_TC_03_003_001_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqSendUntilTimeStamp API in Normal behavior in single thread
 * - Test03_003_001: send a message to queue within 1s and 2000000ns.
 * @subsection MqSendUntilTimeStamp_TC_03_003_001_tpts Test Procedure/Test Steps
 * -# Get current time stamp by R_OSAL_ClockTimeGetTimeStamp.
 * -# Send a message to queue using R_OSAL_MqSendUntilTimeStamp with the time stamp got from above plus with 1s and 2000000nsec.
 * -# Receive message by R_OSAL_MqReceiveForTimePeriod.
 * @subsection MqSendUntilTimeStamp_TC_03_003_001_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqSendUntilTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK. <br>
 * The sent message and received message have same value.
 * @subsection MqSendUntilTimeStamp_TC_03_003_001_testcase Test Case Example
 * @snippet test_mq.cpp MqSendUntilTimeStamp_TC_03_003_001
 */
// [MqSendUntilTimeStamp_TC_03_003_001]
TEST_F(MQApiTest, MqSendUntilTimeStamp_TC_03_003_001)
{
    st_osal_time_t  current_time;
    osal_milli_sec_t time_period_10ms = 10;

    // Test03_003_001
    // @brief: normal case: current_time_stamp.tv_nsec + 2000000
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    if(current_time.tv_nsec + 2 * MSEC_TO_NSEC <= MAX_NSEC)
    {
        current_time.tv_nsec = current_time.tv_nsec + 2 * MSEC_TO_NSEC;
        current_time.tv_sec = current_time.tv_sec + 1;
    }
    else
    {
        current_time.tv_nsec = 2 * MSEC_TO_NSEC - (MAX_NSEC - current_time.tv_nsec);
        current_time.tv_sec = current_time.tv_sec + 2;
    }
    EXPECT_EQ(R_OSAL_MqSendUntilTimeStamp(shared_queue, &current_time, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK) << "Test03_003_001 failed";
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(shared_queue, time_period_10ms,  &receive_msg, sizeof(receive_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);
    receive_msg = 0;
}
// [MqSendUntilTimeStamp_TC_03_003_001]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendUntilTimeStamp_TC_03_003_002 MQApiTest.MqSendUntilTimeStamp_TC_03_003_002
 * @subsection MqSendUntilTimeStamp_TC_03_003_002_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendUntilTimeStamp_TC_03_003_002_tci Test Case ID
 * MqSendUntilTimeStamp_S1_BN_TC_03_003_002
 * @subsection MqSendUntilTimeStamp_TC_03_003_002_rr Related Requirement(s)
 * \#199172
 * @subsection MqSendUntilTimeStamp_TC_03_003_002_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqSendUntilTimeStamp_TC_03_003_002_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqSendUntilTimeStamp API in Normal behavior in single thread
 * - Test03_003_002: send a message to queue within 1s and 1000000ns.
 * @subsection MqSendUntilTimeStamp_TC_03_003_002_tpts Test Procedure/Test Steps
 * -# Get current time stamp by R_OSAL_ClockTimeGetTimeStamp.
 * -# Send a message to queue using R_OSAL_MqSendUntilTimeStamp with the time stamp got from above plus with 1s and 1000000nsec.
 * -# Receive message by R_OSAL_MqReceiveForTimePeriod.
 * @subsection MqSendUntilTimeStamp_TC_03_003_002_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqSendUntilTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK. <br>
 * The sent message and received message have same value.
 * @subsection MqSendUntilTimeStamp_TC_03_003_002_testcase Test Case Example
 * @snippet test_mq.cpp MqSendUntilTimeStamp_TC_03_003_002
 */
// [MqSendUntilTimeStamp_TC_03_003_002]
TEST_F(MQApiTest, MqSendUntilTimeStamp_TC_03_003_002)
{
    st_osal_time_t  current_time;
    osal_milli_sec_t time_period_10ms = 10;

    // Test03_003_002
    // @brief: normal case: current_time_stamp.tv_nsec + 1000000
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    if(current_time.tv_nsec + 1 * MSEC_TO_NSEC <= MAX_NSEC)
    {
        current_time.tv_nsec = current_time.tv_nsec + 1 * MSEC_TO_NSEC;
        current_time.tv_sec = current_time.tv_sec + 1;
    }
    else
    {
        current_time.tv_nsec = 1 * MSEC_TO_NSEC - (MAX_NSEC - current_time.tv_nsec);
        current_time.tv_sec = current_time.tv_sec + 2;
    }
    EXPECT_EQ(R_OSAL_MqSendUntilTimeStamp(shared_queue, &current_time, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK) << "Test03_003_002 failed";
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(shared_queue, time_period_10ms,  &receive_msg, sizeof(receive_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);
    receive_msg = 0;
}
// [MqSendUntilTimeStamp_TC_03_003_002]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendUntilTimeStamp_TC_03_003_003 MQApiTest.MqSendUntilTimeStamp_TC_03_003_003
 * @subsection MqSendUntilTimeStamp_TC_03_003_003_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendUntilTimeStamp_TC_03_003_003_tci Test Case ID
 * MqSendUntilTimeStamp_S1_BN_TC_03_003_003
 * @subsection MqSendUntilTimeStamp_TC_03_003_003_rr Related Requirement(s)
 * \#199172
 * @subsection MqSendUntilTimeStamp_TC_03_003_003_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqSendUntilTimeStamp_TC_03_003_003_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqSendUntilTimeStamp API in Normal behavior in single thread
 * - Test03_003_003: send a message to queue with time stamp is plus with 1s and the nano second value is 999999999.
 * @subsection MqSendUntilTimeStamp_TC_03_003_003_tpts Test Procedure/Test Steps
 * -# Get current time stamp by R_OSAL_ClockTimeGetTimeStamp
 * -# Send a message to queue using R_OSAL_MqSendUntilTimeStamp with the time stamp got from above plus with 1s and the nano second value is 999999999.
 * -# Receive message by R_OSAL_MqReceiveForTimePeriod.
 * @subsection MqSendUntilTimeStamp_TC_03_003_003_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqSendUntilTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK. <br>
 * The sent message and received message have same value.
 * @subsection MqSendUntilTimeStamp_TC_03_003_003_testcase Test Case Example
 * @snippet test_mq.cpp MqSendUntilTimeStamp_TC_03_003_003
 */
// [MqSendUntilTimeStamp_TC_03_003_003]
TEST_F(MQApiTest, MqSendUntilTimeStamp_TC_03_003_003)
{
    st_osal_time_t  current_time;
    osal_milli_sec_t time_period_10ms = 10;

    // Test03_003_003
    // @brief: normal case: current_time_stamp.tv_nsec = 999999999
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    current_time.tv_sec += 1;
    current_time.tv_nsec = MAX_NSEC;
    EXPECT_EQ(R_OSAL_MqSendUntilTimeStamp(shared_queue, &current_time, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK) << "Test03_003_003 failed";
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(shared_queue, time_period_10ms,  &receive_msg, sizeof(receive_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);
    receive_msg = 0;
}
// [MqSendUntilTimeStamp_TC_03_003_003]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendUntilTimeStamp_TC_03_003_004 MQApiTest.MqSendUntilTimeStamp_TC_03_003_004
 * @subsection MqSendUntilTimeStamp_TC_03_003_004_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendUntilTimeStamp_TC_03_003_004_tci Test Case ID
 * MqSendUntilTimeStamp_S1_N_TC_03_003_004
 * @subsection MqSendUntilTimeStamp_TC_03_003_004_rr Related Requirement(s)
 * \#199172
 * @subsection MqSendUntilTimeStamp_TC_03_003_004_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqSendUntilTimeStamp_TC_03_003_004_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqSendUntilTimeStamp API in Normal behavior in single thread
 * - Test03_003_004: send a message to queue with time stamp is plus with 1s and the nano second value is 111111.
 * @subsection MqSendUntilTimeStamp_TC_03_003_004_tpts Test Procedure/Test Steps
 * -# Get current time stamp by R_OSAL_ClockTimeGetTimeStamp
 * -# Send a message to queue using R_OSAL_MqSendUntilTimeStamp with the time stamp got from above plus with 1s and the nano second value is 111111
 * -# Receive message by R_OSAL_MqReceiveForTimePeriod.
 * @subsection MqSendUntilTimeStamp_TC_03_003_004_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqSendUntilTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK. <br>
 * The sent message and received message have same value.
 * @subsection MqSendUntilTimeStamp_TC_03_003_004_testcase Test Case Example
 * @snippet test_mq.cpp MqSendUntilTimeStamp_TC_03_003_004
 */
// [MqSendUntilTimeStamp_TC_03_003_004]
TEST_F(MQApiTest, MqSendUntilTimeStamp_TC_03_003_004)
{
    st_osal_time_t  current_time;
    osal_milli_sec_t time_period_10ms = 10;

    // Test03_003_004
    // @brief: normal case: current_time_stamp.tv_nsec = 111111
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    current_time.tv_sec += 1;
    current_time.tv_nsec = 111111;
    EXPECT_EQ(R_OSAL_MqSendUntilTimeStamp(shared_queue, &current_time, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK) << "Test03_003_004 failed";
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(shared_queue, time_period_10ms,  &receive_msg, sizeof(receive_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);
    receive_msg = 0;
}
// [MqSendUntilTimeStamp_TC_03_003_004]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendUntilTimeStamp_TC_03_003_005 MQApiTest.MqSendUntilTimeStamp_TC_03_003_005
 * @subsection MqSendUntilTimeStamp_TC_03_003_005_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendUntilTimeStamp_TC_03_003_005_tci Test Case ID
 * MqSendUntilTimeStamp_S0_A_TC_03_003_005
 * @subsection MqSendUntilTimeStamp_TC_03_003_005_rr Related Requirement(s)
 * \#199151, \#199165, \#199172
 * @subsection MqSendUntilTimeStamp_TC_03_003_005_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqSendUntilTimeStamp_TC_03_003_005_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqSendUntilTimeStamp API in Abnormal behavior in single thread
 * - Test03_003_005: send a message to queue with with NULL handle.
 * @subsection MqSendUntilTimeStamp_TC_03_003_005_tpts Test Procedure/Test Steps
 * -# Get current time stamp by R_OSAL_ClockTimeGetTimeStamp.
 * -# Send a message to queue using R_OSAL_MqSendUntilTimeStamp with the valid time stamp but NULL handle.
 * @subsection MqSendUntilTimeStamp_TC_03_003_005_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK. 
 * -# The return value of R_OSAL_MqSendUntilTimeStamp is OSAL_RETURN_HANDLE.
 * @subsection MqSendUntilTimeStamp_TC_03_003_005_testcase Test Case Example
 * @snippet test_mq.cpp MqSendUntilTimeStamp_TC_03_003_005
 */
// [MqSendUntilTimeStamp_TC_03_003_005]
TEST_F(MQApiTest, MqSendUntilTimeStamp_TC_03_003_005)
{
    st_osal_time_t  current_time;

    // Test03_003_005
    // @brief: abnormal case: handle is NULL
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    if(current_time.tv_nsec + 2 * MSEC_TO_NSEC <= MAX_NSEC)
    {
        current_time.tv_nsec = current_time.tv_nsec + 2 * MSEC_TO_NSEC;
        current_time.tv_sec = current_time.tv_sec + 1;
    }
    else
    {
        current_time.tv_nsec = 2 * MSEC_TO_NSEC - (MAX_NSEC - current_time.tv_nsec);
        current_time.tv_sec = current_time.tv_sec + 2;
    }
    EXPECT_EQ(R_OSAL_MqSendUntilTimeStamp(OSAL_MQ_HANDLE_INVALID, &current_time, &send_msg, sizeof(send_msg)), OSAL_RETURN_HANDLE) << "Test03_003_005 failed";
}
// [MqSendUntilTimeStamp_TC_03_003_005]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendUntilTimeStamp_TC_03_003_006 MQApiTest.MqSendUntilTimeStamp_TC_03_003_006
 * @subsection MqSendUntilTimeStamp_TC_03_003_006_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendUntilTimeStamp_TC_03_003_006_tci Test Case ID
 * MqSendUntilTimeStamp_S0_A_TC_03_003_006
 * @subsection MqSendUntilTimeStamp_TC_03_003_006_rr Related Requirement(s)
 * \#199172
 * @subsection MqSendUntilTimeStamp_TC_03_003_006_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqSendUntilTimeStamp_TC_03_003_006_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqSendUntilTimeStamp API in Abnormal behavior in single thread
 * - Test03_003_006: send a message to a deleted handle.
 * @subsection MqSendUntilTimeStamp_TC_03_003_006_tpts Test Procedure/Test Steps
 * -# Create a queue by R_OSAL_MqCreate. 
 * -# Delete created queue.
 * -# Get current time stamp by R_OSAL_ClockTimeGetTimeStamp.
 * -# Send a message to queue using R_OSAL_MqSendUntilTimeStamp with the valid time stamp.
 * @subsection MqSendUntilTimeStamp_TC_03_003_006_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK. <br>
 * The return handle is not NULL_PTR.
 * -# The return value of R_OSAL_MqDelete is OSAL_RETURN_OK. 
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK. 
 * -# The return value of R_OSAL_MqSendUntilTimeStamp is OSAL_RETURN_HANDLE.
 * @subsection MqSendUntilTimeStamp_TC_03_003_006_testcase Test Case Example
 * @snippet test_mq.cpp MqSendUntilTimeStamp_TC_03_003_006
 */
// [MqSendUntilTimeStamp_TC_03_003_006]
TEST_F(MQApiTest, MqSendUntilTimeStamp_TC_03_003_006)
{
    st_osal_time_t  current_time;

    // Test03_003_006
    // @brief: abnormal case: send a message to a handle has been deleted
    mq_config.max_num_msg = 1;
    mq_config.msg_size = 4;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config, TEST_MQ_ID2, &mq_handle), OSAL_RETURN_OK);
    EXPECT_NE(mq_handle, NULL_PTR);
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    if(current_time.tv_nsec + 2 * MSEC_TO_NSEC <= MAX_NSEC)
    {
        current_time.tv_nsec = current_time.tv_nsec + 2 * MSEC_TO_NSEC;
        current_time.tv_sec = current_time.tv_sec + 1;
    }
    else
    {
        current_time.tv_nsec = 2 * MSEC_TO_NSEC - (MAX_NSEC - current_time.tv_nsec);
        current_time.tv_sec = current_time.tv_sec + 2;
    }
    EXPECT_EQ(R_OSAL_MqSendUntilTimeStamp(mq_handle, &current_time, &send_msg, sizeof(send_msg)), OSAL_RETURN_HANDLE) << "Test03_003_006 failed";
    mq_handle = OSAL_MQ_HANDLE_INVALID;
}
// [MqSendUntilTimeStamp_TC_03_003_006]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendUntilTimeStamp_TC_03_003_007 MQApiTest.MqSendUntilTimeStamp_TC_03_003_007
 * @subsection MqSendUntilTimeStamp_TC_03_003_007_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendUntilTimeStamp_TC_03_003_007_tci Test Case ID
 * MqSendUntilTimeStamp_S0_A_TC_03_003_007
 * @subsection MqSendUntilTimeStamp_TC_03_003_007_rr Related Requirement(s)
 * \#199172
 * @subsection MqSendUntilTimeStamp_TC_03_003_007_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqSendUntilTimeStamp_TC_03_003_007_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqSendUntilTimeStamp API in Abnormal behavior in single thread
 * - Test03_003_007: send a message to queue with time_stamp is NULL.
 * @subsection MqSendUntilTimeStamp_TC_03_003_007_tpts Test Procedure/Test Steps
 * -# Send a message to queue using R_OSAL_MqSendUntilTimeStamp with the NULL time stamp.
 * @subsection MqSendUntilTimeStamp_TC_03_003_007_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqSendUntilTimeStamp is OSAL_RETURN_PAR.
 * @subsection MqSendUntilTimeStamp_TC_03_003_007_testcase Test Case Example
 * @snippet test_mq.cpp MqSendUntilTimeStamp_TC_03_003_007
 */
// [MqSendUntilTimeStamp_TC_03_003_007]
TEST_F(MQApiTest, MqSendUntilTimeStamp_TC_03_003_007)
{
    st_osal_time_t* p_time_stamp;

    // Test03_003_007
    // @brief: abnormal case: input time_stamp is NULL
    p_time_stamp = NULL;
    EXPECT_EQ(R_OSAL_MqSendUntilTimeStamp(shared_queue, p_time_stamp, &send_msg, sizeof(send_msg)), OSAL_RETURN_PAR) << "Test03_003_007 failed";
}
// [MqSendUntilTimeStamp_TC_03_003_007]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendUntilTimeStamp_TC_03_003_008 MQApiTest.MqSendUntilTimeStamp_TC_03_003_008
 * @subsection MqSendUntilTimeStamp_TC_03_003_008_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendUntilTimeStamp_TC_03_003_008_tci Test Case ID
 * MqSendUntilTimeStamp_S0_A_TC_03_003_008
 * @subsection MqSendUntilTimeStamp_TC_03_003_008_rr Related Requirement(s)
 * \#199172
 * @subsection MqSendUntilTimeStamp_TC_03_003_008_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqSendUntilTimeStamp_TC_03_003_008_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqSendUntilTimeStamp API in Abnormal behavior in single thread
 * - Test03_003_008: send a message to queue with p_buffer is NULL.
 * @subsection MqSendUntilTimeStamp_TC_03_003_008_tpts Test Procedure/Test Steps
 * -# Get current time stamp by R_OSAL_ClockTimeGetTimeStamp.
 * -# Send a message to queue using R_OSAL_MqSendUntilTimeStamp with above time stamp and the NULL p_buffer.
 * @subsection MqSendUntilTimeStamp_TC_03_003_008_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK. 
 * -# The return value of R_OSAL_MqSendUntilTimeStamp is OSAL_RETURN_PAR. 
 * @subsection MqSendUntilTimeStamp_TC_03_003_008_testcase Test Case Example
 * @snippet test_mq.cpp MqSendUntilTimeStamp_TC_03_003_008
 */
// [MqSendUntilTimeStamp_TC_03_003_008]
TEST_F(MQApiTest, MqSendUntilTimeStamp_TC_03_003_008)
{
    st_osal_time_t  current_time;
    void* p_buffer;

    // Test03_003_008
    // @brief: abnormal case: p_buffer is NULL
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    if(current_time.tv_nsec + 2 * MSEC_TO_NSEC <= MAX_NSEC)
    {
        current_time.tv_nsec = current_time.tv_nsec + 2 * MSEC_TO_NSEC;
        current_time.tv_sec = current_time.tv_sec + 1;
    }
    else
    {
        current_time.tv_nsec = 2 * MSEC_TO_NSEC - (MAX_NSEC - current_time.tv_nsec);
        current_time.tv_sec = current_time.tv_sec + 2;
    }
    p_buffer = NULL;
    EXPECT_EQ(R_OSAL_MqSendUntilTimeStamp(shared_queue, &current_time, p_buffer, sizeof(send_msg)), OSAL_RETURN_PAR) << "Test03_003_008 failed";
}
// [MqSendUntilTimeStamp_TC_03_003_008]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendUntilTimeStamp_TC_03_003_009 MQApiTest.MqSendUntilTimeStamp_TC_03_003_009
 * @subsection MqSendUntilTimeStamp_TC_03_003_009_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendUntilTimeStamp_TC_03_003_009_tci Test Case ID
 * MqSendUntilTimeStamp_S1_A_TC_03_003_009
 * @subsection MqSendUntilTimeStamp_TC_03_003_009_rr Related Requirement(s)
 * \#199172
 * @subsection MqSendUntilTimeStamp_TC_03_003_009_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqSendUntilTimeStamp_TC_03_003_009_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqSendUntilTimeStamp API in Normal behavior in single thread
 * - Test03_003_009: send a message to queue with p_time_stamp parameter is the past time.
 * @subsection MqSendUntilTimeStamp_TC_03_003_009_tpts Test Procedure/Test Steps
 * -# Get current time stamp by R_OSAL_ClockTimeGetTimeStamp
 * -# Send a message to queue using R_OSAL_MqSendUntilTimeStamp with above time stamp - 1s
 * -# Receive message from queue using R_OSAL_MqReceiveForTimePeriod.
 * @subsection MqSendUntilTimeStamp_TC_03_003_009_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqSendUntilTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK. <br>
 * The sent and receive message is the same.
 * @subsection MqSendUntilTimeStamp_TC_03_003_009_testcase Test Case Example
 * @snippet test_mq.cpp MqSendUntilTimeStamp_TC_03_003_009
 */
// [MqSendUntilTimeStamp_TC_03_003_009]
TEST_F(MQApiTest, MqSendUntilTimeStamp_TC_03_003_009)
{
    st_osal_time_t  current_time;
    osal_milli_sec_t time_period_10ms = 10;
    osal_milli_sec_t time_period_2000ms = 2000;

    // Test03_003_009
    // @brief: normal case: p_time_stamp is the past time
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    if (current_time.tv_sec < 1)
    {
        EXPECT_EQ(R_OSAL_ThreadSleepForTimePeriod(time_period_2000ms), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    }
    current_time.tv_sec = current_time.tv_sec - 1;
    EXPECT_EQ(R_OSAL_MqSendUntilTimeStamp(shared_queue, &current_time, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK) << "Test03_003_009 failed";
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(shared_queue, time_period_10ms,  &receive_msg, sizeof(receive_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);
    receive_msg = 0;
}
// [MqSendUntilTimeStamp_TC_03_003_009]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendUntilTimeStamp_TC_03_003_010 MQApiTest.MqSendUntilTimeStamp_TC_03_003_010
 * @subsection MqSendUntilTimeStamp_TC_03_003_010_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendUntilTimeStamp_TC_03_003_010_tci Test Case ID
 * MqSendUntilTimeStamp_S0_A_TC_03_003_010
 * @subsection MqSendUntilTimeStamp_TC_03_003_010_rr Related Requirement(s)
 * \#199172
 * @subsection MqSendUntilTimeStamp_TC_03_003_010_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqSendUntilTimeStamp_TC_03_003_010_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqSendUntilTimeStamp API in Abnormal behavior in single thread
 * - Test03_003_010: send a message to queue with buffer_size is 0.
 * @subsection MqSendUntilTimeStamp_TC_03_003_010_tpts Test Procedure/Test Steps
 * -# Get current time stamp by R_OSAL_ClockTimeGetTimeStamp.
 * -# Send a message to queue using R_OSAL_MqSendUntilTimeStamp with valid time stamp but buffer_size is 0.
 * @subsection MqSendUntilTimeStamp_TC_03_003_010_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK. 
 * -# The return value of R_OSAL_MqSendUntilTimeStamp is OSAL_RETURN_PAR.
 * @subsection MqSendUntilTimeStamp_TC_03_003_010_testcase Test Case Example
 * @snippet test_mq.cpp MqSendUntilTimeStamp_TC_03_003_010
 */
// [MqSendUntilTimeStamp_TC_03_003_010]
TEST_F(MQApiTest, MqSendUntilTimeStamp_TC_03_003_010)
{
    st_osal_time_t  current_time;
    size_t buffer_size;

    // Test03_003_010
    // @brief: abnormal case: buffer_size is 0
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    if(current_time.tv_nsec + 2 * MSEC_TO_NSEC <= MAX_NSEC)
    {
        current_time.tv_nsec = current_time.tv_nsec + 2 * MSEC_TO_NSEC;
        current_time.tv_sec = current_time.tv_sec + 1;
    }
    else
    {
        current_time.tv_nsec = 2 * MSEC_TO_NSEC - (MAX_NSEC - current_time.tv_nsec);
        current_time.tv_sec = current_time.tv_sec + 2;
    }
    buffer_size = 0;
    EXPECT_EQ(R_OSAL_MqSendUntilTimeStamp(shared_queue, &current_time, &send_msg, buffer_size), OSAL_RETURN_PAR) << "Test03_003_010 failed";
}
// [MqSendUntilTimeStamp_TC_03_003_010]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendUntilTimeStamp_TC_03_003_011 MQApiTest.MqSendUntilTimeStamp_TC_03_003_011
 * @subsection MqSendUntilTimeStamp_TC_03_003_011_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendUntilTimeStamp_TC_03_003_011_tci Test Case ID
 * MqSendUntilTimeStamp_S0_BA_TC_03_003_011
 * @subsection MqSendUntilTimeStamp_TC_03_003_011_rr Related Requirement(s)
 * \#199172
 * @subsection MqSendUntilTimeStamp_TC_03_003_011_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqSendUntilTimeStamp_TC_03_003_011_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqSendUntilTimeStamp API in Abnormal behavior in single thread
 * - Test03_003_011: send a message to queue with tv_nsec is 1000000000ns.
 * @subsection MqSendUntilTimeStamp_TC_03_003_011_tpts Test Procedure/Test Steps
 * -# Get current time stamp by R_OSAL_ClockTimeGetTimeStamp.
 * -# Send a message to queue using R_OSAL_MqSendUntilTimeStamp with the time stamp got from above plus with 1s and the nano second value is 1000000000.
 * @subsection MqSendUntilTimeStamp_TC_03_003_011_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK. 
 * -# The return value of R_OSAL_MqSendUntilTimeStamp is OSAL_RETURN_PAR.
 * @subsection MqSendUntilTimeStamp_TC_03_003_011_testcase Test Case Example
 * @snippet test_mq.cpp MqSendUntilTimeStamp_TC_03_003_011
 */
// [MqSendUntilTimeStamp_TC_03_003_011]
TEST_F(MQApiTest, MqSendUntilTimeStamp_TC_03_003_011)
{
    st_osal_time_t  current_time;

    // Test03_003_011
    // @brief: abnormal case: time_stamp.tv_nsec = 1000000000
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    current_time.tv_sec += 1;
    current_time.tv_nsec = 1 * SEC_TO_NSEC;
    EXPECT_EQ(R_OSAL_MqSendUntilTimeStamp(shared_queue, &current_time, &send_msg, sizeof(send_msg)), OSAL_RETURN_PAR) << "Test03_003_011 failed";

}
// [MqSendUntilTimeStamp_TC_03_003_011]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendUntilTimeStamp_TC_03_003_012 MQApiTest.MqSendUntilTimeStamp_TC_03_003_012
 * @subsection MqSendUntilTimeStamp_TC_03_003_012_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendUntilTimeStamp_TC_03_003_012_tci Test Case ID
 * MqSendUntilTimeStamp_S2_A_TC_03_003_012
 * @subsection MqSendUntilTimeStamp_TC_03_003_012_rr Related Requirement(s)
 * \#199172
 * @subsection MqSendUntilTimeStamp_TC_03_003_012_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqSendUntilTimeStamp_TC_03_003_012_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqSendUntilTimeStamp API in Abnormal behavior in single thread
 * - Test03_003_012: send to a full queue.
 * @subsection MqSendUntilTimeStamp_TC_03_003_012_tpts Test Procedure/Test Steps
 * -# Call R_OSAL_MqReset to reset the queue. 
 * -# Get current time stamp by R_OSAL_ClockTimeGetTimeStamp.
 * -# Send 2 message to the queue to make the queue full by calling 2 times function R_OSAL_MqSendUntilTimeStamp with valid parameter. 
 * -# Get current time stamp by R_OSAL_ClockTimeGetTimeStamp.
 * -# Send another message to the queue by using R_OSAL_MqSendUntilTimeStamp with time stamp got from above plus with 1s and 2000000ns . 
 * -# Receive 2 message from the queue by calling 2 times function R_OSAL_MqReceiveForTimePeriod.
 * @subsection MqSendUntilTimeStamp_TC_03_003_012_ereb Expected Result/Behavior
 * -# The result value of R_OSAL_MqReset is OSAL_RETURN_OK. 
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK. 
 * -# The return value of 2 times calling function R_OSAL_MqSendUntilTimeStamp is OSAL_RETURN_OK. 
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK. 
 * -# The return value of R_OSAL_MqSendUntilTimeStamp is OSAL_RETURN_TIME. 
 * -# The return value of 2 times calling function R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK.
 * @subsection MqSendUntilTimeStamp_TC_03_003_012_testcase Test Case Example
 * @snippet test_mq.cpp MqSendUntilTimeStamp_TC_03_003_012
 */
// [MqSendUntilTimeStamp_TC_03_003_012]
TEST_F(MQApiTest, MqSendUntilTimeStamp_TC_03_003_012)
{
    st_osal_time_t  current_time;
    osal_milli_sec_t time_period_10ms = 10;

    // Test03_003_012
    // @brief: abnormal case: Send to a full queue
    EXPECT_EQ(R_OSAL_MqReset(shared_queue), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    current_time.tv_sec = current_time.tv_sec + 2;
    EXPECT_EQ(R_OSAL_MqSendUntilTimeStamp(shared_queue, &current_time, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);// make the queue full
    EXPECT_EQ(R_OSAL_MqSendUntilTimeStamp(shared_queue, &current_time, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK); // make the queue full
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    if(current_time.tv_nsec + 2 * MSEC_TO_NSEC <= MAX_NSEC)
    {
        current_time.tv_nsec = current_time.tv_nsec + 2 * MSEC_TO_NSEC;
        current_time.tv_sec = current_time.tv_sec + 1;
    }
    else
    {
        current_time.tv_nsec = 2 * MSEC_TO_NSEC - (MAX_NSEC - current_time.tv_nsec);
        current_time.tv_sec = current_time.tv_sec + 2;
    }
    EXPECT_EQ(R_OSAL_MqSendUntilTimeStamp(shared_queue, &current_time, &send_msg, sizeof(send_msg)), OSAL_RETURN_TIME) << "Test03_003_012 failed";
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(shared_queue,time_period_10ms,  &receive_msg, sizeof(receive_msg)), OSAL_RETURN_OK); //receive all message for next test cases
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(shared_queue,time_period_10ms,  &receive_msg, sizeof(receive_msg)), OSAL_RETURN_OK); //receive all message for next test cases
    receive_msg = 0;
}
// [MqSendUntilTimeStamp_TC_03_003_012]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendUntilTimeStamp_TC_03_003_013 MQApiTest.MqSendUntilTimeStamp_TC_03_003_013
 * @subsection MqSendUntilTimeStamp_TC_03_003_013_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendUntilTimeStamp_TC_03_003_013_tci Test Case ID
 * MqSendUntilTimeStamp_S3_N_TC_03_003_013
 * @subsection MqSendUntilTimeStamp_TC_03_003_013_rr Related Requirement(s)
 * \#199167, \#199172
 * @subsection MqSendUntilTimeStamp_TC_03_003_013_pp Prerequisite/Precondition
 * - OSAL is initialized and the shared queue has been created by R_OSAL_MqCreate.
 * - Synchronization queue was created.
 * @subsection MqSendUntilTimeStamp_TC_03_003_013_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqSendUntilTimeStamp API in Normal behavior in multi thread
 * - Test03_003_013: using multi thread with one queue.
 * - Post-process: reset the shared queue
 * @subsection MqSendUntilTimeStamp_TC_03_003_013_tpts Test Procedure/Test Steps
 * -# Send the 1st message to the queue by R_OSAL_MqSendForTimePeriod.
 * -# Get the current time stamp by calling R_OSAL_ClockTimeGetTimeStamp.
 * -# Create 1st thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *          -# task_name is "task03_003_013_01" 
 *          -# func is osal_test_thread_send_timeperiod function with functionality:
 *              - Get the synchronization queue config by using R_OSAL_MqGetConfig
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *              - Send message to the queue by R_OSAL_MqSendForTimePeriod.
 *          -# userarg is send_timeperiod_arg with:
 *              - mq_handle is shared_queue
 *              - time is 1000ms
 *              - p_buffer is send_msg (0xFFFF)
 *              - buffer_size is sizeof(send_msg)
 *              - sync_mq is handle of synchronization queue
 *              - sync_send_msg is the sending message for synchronization purpose
 *              - needToSync is true
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Address of thread handle.
 * -# Receive message from synchronization queue by using R_OSAL_MqReceiveForTimePeriod
 * -# Create 2nd thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *          -# task_name is "task03_003_013_02" 
 *          -# func is osal_test_thread_send_timestamp function with functionality:
 *              - Send message to the queue by R_OSAL_MqSendUntilTimeStamp.
 *          -# userarg is send_timestamp_arg with:
 *              - mq_handle is shared_queue
 *              - timestamp is the time stamp got from above plus with 2s and 2000000ns,
 *              - p_buffer is send_msg (0xFFFF)
 *              - buffer_size is sizeof(send_msg)
 *              - needToSync is false
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2.
 *      - Address of thread handle.
 * -# Calling R_OSAL_ThreadJoin to join the 1st thread.
 * -# Calling function R_OSAL_MqReceiveForTimePeriod to receive 1 message from the queue.
 * -# Calling R_OSAL_ThreadJoin to join the 2nd thread.
 * -# Calling R_OSAL_MqReset to reset the queue.
 * @subsection MqSendUntilTimeStamp_TC_03_003_013_ereb Expected Result/Behavior
 * -# The return value of the first R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of the 1st R_OSAL_ThreadCreate is OSAL_RETURN_OK.<br>
 * In osal_test_thread_send_timeperiod function:<br>
 *      - R_OSAL_MqGetConfig return OSAL_RETURN_OK
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of the 2nd R_OSAL_ThreadCreate is OSAL_RETURN_OK.<br>
 * In osal_test_thread_send_timestamp function:<br>
 *      - R_OSAL_MqSendUntilTimeStamp return OSAL_RETURN_OK
 * -# The return value of the 1st R_OSAL_ThreadJoin is OSAL_RETURN_OK.<br>
 *  "return_value1" is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of the 2nd R_OSAL_ThreadJoin is OSAL_RETURN_OK.<br>
 *  "return_value" is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_OK.
 * @subsection MqSendUntilTimeStamp_TC_03_003_013_testcase Test Case Example
 * @snippet test_mq.cpp MqSendUntilTimeStamp_TC_03_003_013
 * @snippet test_mq.cpp osal_test_thread_send_timeperiod
 * @snippet test_mq.cpp osal_test_thread_send_timestamp
 */
// [MqSendUntilTimeStamp_TC_03_003_013]
TEST_F(MQApiTest, MqSendUntilTimeStamp_TC_03_003_013)
{
    st_osal_time_t  current_time;
    osal_thread_handle_t thread_handle2;
    osal_thread_handle_t thread_handle1;
    st_osal_thread_config_t config2;
    st_osal_thread_config_t config1;
    int64_t return_value = 0;
    int64_t return_value1 = 0;
    st_arg_mq_send_timestamp_t send_timestamp_arg;
    st_arg_mq_send_timeperiod_t send_timeperiod_arg;
    osal_milli_sec_t time_period_10ms = 10;
    osal_milli_sec_t time_period_100ms = 100;
    osal_milli_sec_t time_period_1000ms = 1000;

    // Test03_003_013
    // @brief: normal case: Multi thread with one queue
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue, time_period_100ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK); // add one message to the queue
    send_timeperiod_arg.mq_handle = shared_queue;
    send_timeperiod_arg.time = 5*1000;
    send_timeperiod_arg.p_buffer =  &send_msg;
    send_timeperiod_arg.buffer_size = sizeof(send_msg);
    send_timeperiod_arg.sync_mq = sync_mq;
    send_timeperiod_arg.sync_send_msg = sync_send_msg;
    send_timeperiod_arg.needToSync = true;

    config1.func       = osal_test_thread_send_timeperiod;
    config1.userarg    = (void *)&send_timeperiod_arg;
    config1.priority   = OSAL_THREAD_PRIORITY_TYPE12;
    config1.task_name  = "task03_003_013_01";
    config1.stack_size = THREAD_STACK_SIZE;

    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    if(current_time.tv_nsec + 2 * MSEC_TO_NSEC <= MAX_NSEC)
    {
        current_time.tv_nsec = current_time.tv_nsec + 2 * MSEC_TO_NSEC;
        current_time.tv_sec = current_time.tv_sec + 2;
    }
    else
    {
        current_time.tv_nsec = 2 * MSEC_TO_NSEC - (MAX_NSEC - current_time.tv_nsec);
        current_time.tv_sec = current_time.tv_sec + 3;
    }
    send_timestamp_arg.mq_handle = shared_queue;
    send_timestamp_arg.time.tv_sec = current_time.tv_sec;
    send_timestamp_arg.time.tv_nsec = current_time.tv_nsec;
    send_timestamp_arg.p_buffer = &send_msg;
    send_timestamp_arg.buffer_size = sizeof(send_msg);
    send_timestamp_arg.needToSync = false;
    config2.func       = osal_test_thread_send_timestamp;
    config2.userarg    = (void *)&send_timestamp_arg;
    config2.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config2.task_name  = "task03_003_013_02";
    config2.stack_size = THREAD_STACK_SIZE;

    EXPECT_EQ(R_OSAL_ThreadCreate(&config1, TEST_THREAD_ID1, &thread_handle1),OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &sync_receive_msg, sync_mq_config.msg_size), OSAL_RETURN_OK);
    EXPECT_EQ(sync_receive_msg, sync_send_msg);
    EXPECT_EQ(R_OSAL_ThreadCreate(&config2, TEST_THREAD_ID2, &thread_handle2),OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &return_value1),OSAL_RETURN_OK);
    EXPECT_EQ(return_value1, OSAL_RETURN_OK) << "Test03_003_013 failed";
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(shared_queue, time_period_10ms, &receive_msg, sizeof(receive_msg)), OSAL_RETURN_OK); //receive one message to make thread 2 can send another message
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle2, &return_value), OSAL_RETURN_OK);
    EXPECT_EQ(return_value, OSAL_RETURN_OK) << "Test03_003_013 failed";
    receive_msg = 0;

    //post-process: reset the share queue
    EXPECT_EQ(R_OSAL_MqReset(shared_queue), OSAL_RETURN_OK) << "post-process failed";

}
// [MqSendUntilTimeStamp_TC_03_003_013]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendUntilTimeStamp_TC_03_003_014 MQApiTest.MqSendUntilTimeStamp_TC_03_003_014
 * @subsection MqSendUntilTimeStamp_TC_03_003_014_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendUntilTimeStamp_TC_03_003_014_tci Test Case ID
 * MqSendUntilTimeStamp_S2_A_TC_03_003_014
 * @subsection MqSendUntilTimeStamp_TC_03_003_014_rr Related Requirement(s)
 * \#199167, \#199172, \#199181
 * @subsection MqSendUntilTimeStamp_TC_03_003_014_pp Prerequisite/Precondition
 * - OSAL is initialized and the shared_queue has been created by R_OSAL_MqCreate.
 * - The synchronization queue was created.
 * @subsection MqSendUntilTimeStamp_TC_03_003_014_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqSendUntilTimeStamp API in Abnormal behavior in multi thread
 * - Pre-process: create 2nd queue
 * - Test03_003_014: using multi thread with multi queue (already full).
 * - Post-process: reset and deleted 2nd queue, reset the shared queue
 * @subsection MqSendUntilTimeStamp_TC_03_003_014_tpts Test Procedure/Test Steps
 * -# Make the 2nd queue created above full also by calling R_OSAL_MqSendForTimePeriod with valid parameter.
 * -# Send 2 messages to the queue (call 1st queue) to make the queue full by calling 2 times R_OSAL_MqSendForTimePeriod.
 * -# Create another queue (call 2nd queue) with max_num_msg = 1 by calling R_OSAL_MqCreate.
 * -# Get current time stamp by calling functnon R_OSAL_ClockTimeGetTimeStamp.
 * -# Create 1st thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *          -# task_name is "task03_003_014_01" 
 *          -# func is osal_test_thread_send_timestamp function with functionality:
 *              - Get the synchronization queue config by using R_OSAL_MqGetConfig
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *              - Send message to the queue by R_OSAL_MqSendUntilTimeStamp.
 *          -# userarg is send_timestamp_arg with:
 *              - mq_handle is 1st queue (shared_queue)
 *              - timestamp is the time stamp got from above plus with 5s and 2000000ns
 *              - p_buffer is send_msg (0xFFFF)
 *              - buffer_size is sizeof(send_msg)
 *              - sync_mq is handle of synchronization queue
 *              - sync_send_msg is the sending message for synchronization purpose
 *              - needToSync is true
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1.
 *      - Address of thread handle.
 * -# Create 2nd thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *          -# task_name is "task03_003_014_02" 
 *          -# func is osal_test_thread_send_timestamp function with functionality:
 *              - Get the synchronization queue config by using R_OSAL_MqGetConfig
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *              - Send message to the queue by R_OSAL_MqSendUntilTimeStamp.
 *          -# userarg is send_timestamp_arg with:
 *              - mq_handle is 2nd queue.
 *              - timestamp is the time stamp got from above plus with 5s and 2000000ns
 *              - p_buffer is send_msg (0xFFFF)
 *              - buffer_size is sizeof(send_msg)
 *              - sync_mq is handle of synchronization queue
 *              - sync_send_msg is the sending message for synchronization purpose
 *              - needToSync is true
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2.
 *      - Address of thread handle.
 * -# Receive 2 messages from synchronization queue by using R_OSAL_MqReceiveForTimePeriod
 * -# Create 3nd thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *          -# task_name is "task03_003_014_03" 
 *          -# func is osal_test_thread_receive_timeperiod function with functionality:
 *              - Receive message from the queue by R_OSAL_MqReceiveForTimePeriod.
 *          -# userarg is receive_timeperiod_arg with:
 *              - mq_handle is 1st queue (shared_queue)
 *              - time is 1000ms
 *              - needToSync is false
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID3.
 *      - Address of thread handle.
 * -# Calling R_OSAL_ThreadJoin to join the 3rd thread.
 * -# Calling R_OSAL_ThreadJoin to join the 1st thread.
 * -# Calling R_OSAL_ThreadJoin to join the 2nd thread.
 * -# Receive message from the 1st queue by R_OSAL_MqReceiveForTimePeriod.
 * -# Reset the 2nd queue by R_OSAL_MqReset.
 * -# Reset the 1st queue by R_OSAL_MqReset.
 * -# Delete the 2nd queue by R_OSAL_MqDelete.
 * @subsection MqSendUntilTimeStamp_TC_03_003_014_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK and the return of the 2nd queue handle is not NULL_PTR.
 * -# The return value of the first 2 times calling R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqSendForTimePeriod when sending to the 2nd queue if OSAL_RETURN_OK.
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of the 1st R_OSAL_ThreadCreate is OSAL_RETURN_OK.<br>
 * In osal_test_thread_send_timestamp function:<br>
 *      - R_OSAL_MqGetConfig return OSAL_RETURN_OK
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 *      - R_OSAL_MqSendUntilTimeStamp return OSAL_RETURN_OK
 * -# The return value of the 2nd R_OSAL_ThreadCreate is OSAL_RETURN_OK.<br>
 * In osal_test_thread_send_timestamp function:<br>
 *      - R_OSAL_MqGetConfig return OSAL_RETURN_OK
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 *      - R_OSAL_MqSendUntilTimeStamp return OSAL_RETURN_TIME
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of the 3rd R_OSAL_ThreadCreate is OSAL_RETURN_OK.<br>
 * In osal_test_thread_receive_timeperiod function:<br>
 *      - R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK
 * -# The return value of the 3rd R_OSAL_ThreadJoin is OSAL_RETURN_OK.<br>
 * The value of "return_value3" is equal to OSAL_RETURN_OK
 * -# The return value of the 1st R_OSAL_ThreadJoin is OSAL_RETURN_OK.<br>
 * The value of "return_value1" is equal to OSAL_RETURN_OK
 * -# The return value of the 2nd R_OSAL_ThreadJoin is OSAL_RETURN_OK.<br>
 * The value of "return_value2" is equal to OSAL_RETURN_TIME
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReset when reset 2nd queue is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReset when reset 1st queue is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqDelete when delete 2nd queue is OSAL_RETURN_OK.
 * @subsection MqSendUntilTimeStamp_TC_03_003_014_testcase Test Case Example
 * @snippet test_mq.cpp MqSendUntilTimeStamp_TC_03_003_014
 * @snippet test_mq.cpp osal_test_thread_send_timestamp
 * @snippet test_mq.cpp osal_test_thread_receive_timeperiod
 */
// [MqSendUntilTimeStamp_TC_03_003_014]
TEST_F(MQApiTest, MqSendUntilTimeStamp_TC_03_003_014)
{
    osal_mq_handle_t mq_handle1 = OSAL_MQ_HANDLE_INVALID;
    st_osal_time_t  current_time;
    osal_thread_handle_t thread_handle3;
    osal_thread_handle_t thread_handle2;
    osal_thread_handle_t thread_handle1;
    st_osal_thread_config_t config3;
    st_osal_thread_config_t config2;
    st_osal_thread_config_t config1;
    int64_t return_value1 = 0;
    int64_t return_value2 = 0;
    int64_t return_value3 = 0;
    st_arg_mq_send_timestamp_t send_timestamp_arg1;
    st_arg_mq_send_timestamp_t send_timestamp_arg2;
    st_arg_mq_receive_timeperiod_t receive_timeperiod_arg;
    osal_milli_sec_t time_period_1000ms = 1000;
    osal_milli_sec_t time_period_100ms = 100;
    osal_milli_sec_t time_period_10ms = 10;

    //pre-process: create 2nd queue
    mq_config.max_num_msg = 1;
    mq_config.msg_size = 4;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config, TEST_MQ_ID2, &mq_handle1), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(mq_handle1, NULL_PTR) << "pre-process failed";

    // Test03_003_014
    // @brief: abnormal case: Multi thread with multi queue(already full)
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue, time_period_100ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK); // add one message to the queue
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue, time_period_100ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK); // add one message to the queue -> share handle full

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(mq_handle1, time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK); // make handle1 full
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    if(current_time.tv_nsec + 2 * MSEC_TO_NSEC <= MAX_NSEC)
    {
        current_time.tv_nsec = current_time.tv_nsec + 2 * MSEC_TO_NSEC;
        current_time.tv_sec = current_time.tv_sec + 5;
    }
    else
    {
        current_time.tv_nsec = 2 * MSEC_TO_NSEC - (MAX_NSEC - current_time.tv_nsec);
        current_time.tv_sec = current_time.tv_sec + 6;
    }
    send_timestamp_arg1.mq_handle = shared_queue;
    send_timestamp_arg1.time.tv_sec = current_time.tv_sec;
    send_timestamp_arg1.time.tv_nsec = current_time.tv_nsec;
    send_timestamp_arg1.p_buffer = &send_msg;
    send_timestamp_arg1.buffer_size = sizeof(send_msg);
    send_timestamp_arg1.sync_mq = sync_mq;
    send_timestamp_arg1.sync_send_msg = sync_send_msg;
    send_timestamp_arg1.needToSync = true;
    config1.func       = osal_test_thread_send_timestamp;
    config1.userarg    = (void *)&send_timestamp_arg1;
    config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config1.task_name  = "task03_003_014_01";
    config1.stack_size = THREAD_STACK_SIZE;

    send_timestamp_arg2.mq_handle = mq_handle1;
    send_timestamp_arg2.time.tv_sec = current_time.tv_sec;
    send_timestamp_arg2.time.tv_nsec = current_time.tv_nsec;
    send_timestamp_arg2.p_buffer = &send_msg;
    send_timestamp_arg2.buffer_size = sizeof(send_msg);
    send_timestamp_arg2.sync_mq = sync_mq;
    send_timestamp_arg2.sync_send_msg = sync_send_msg;
    send_timestamp_arg2.needToSync = true;
    config2.func       = osal_test_thread_send_timestamp;
    config2.userarg    = (void *)&send_timestamp_arg2;
    config2.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config2.task_name  = "task03_003_014_02";
    config2.stack_size = THREAD_STACK_SIZE;

    receive_timeperiod_arg.mq_handle = shared_queue;
    receive_timeperiod_arg.time = 1000;
    receive_timeperiod_arg.needToSync = false;
    config3.func       = osal_test_thread_receive_timeperiod;
    config3.userarg    = (void *)&receive_timeperiod_arg;
    config3.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config3.task_name  = "task03_003_014_03";
    config3.stack_size = THREAD_STACK_SIZE;

    EXPECT_EQ(R_OSAL_ThreadCreate(&config1, TEST_THREAD_ID1, &thread_handle1),OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThreadCreate(&config2, TEST_THREAD_ID2, &thread_handle2),OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &sync_receive_msg, sync_mq_config.msg_size), OSAL_RETURN_OK);
    EXPECT_EQ(sync_receive_msg, sync_send_msg);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &sync_receive_msg, sync_mq_config.msg_size), OSAL_RETURN_OK);
    EXPECT_EQ(sync_receive_msg, sync_send_msg);
    EXPECT_EQ(R_OSAL_ThreadCreate(&config3, TEST_THREAD_ID3, &thread_handle3),OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle3, &return_value3),OSAL_RETURN_OK);
    EXPECT_EQ(return_value3, OSAL_RETURN_OK) << "Test03_003_014 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &return_value1),OSAL_RETURN_OK);
    EXPECT_EQ(return_value1, OSAL_RETURN_OK) << "Test03_003_014 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle2, &return_value2),OSAL_RETURN_OK);
    EXPECT_EQ(return_value2, OSAL_RETURN_TIME) << "Test03_003_014 failed";
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(shared_queue, time_period_10ms,  &receive_msg, sizeof(receive_msg)), OSAL_RETURN_OK);
    receive_msg = 0;

    //post-process: reset and delete 2nd queue, reset the shared queue
    EXPECT_EQ(R_OSAL_MqReset(mq_handle1), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_MqReset(shared_queue), OSAL_RETURN_OK) << "post-process failed"; // reset share queue for the next test cases
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle1), OSAL_RETURN_OK) << "post-process failed";
    mq_handle1 = OSAL_MQ_HANDLE_INVALID;

}
// [MqSendUntilTimeStamp_TC_03_003_014]

/**
 * @page MQ_TEST MQ API Test
 * @section OsalApiStateTest_MqSendUntilTimeStamp_TC_03_003_015 OsalApiStateTest.MqSendUntilTimeStamp_TC_03_003_015
 * @subsection MqSendUntilTimeStamp_TC_03_003_015_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection MqSendUntilTimeStamp_TC_03_003_015_tci Test Case ID
 * MqSendUntilTimeStamp_S2_A_TC_03_003_015
 * @subsection MqSendUntilTimeStamp_TC_03_003_015_rr Related Requirement(s)
 * \#199172
 * @subsection MqSendUntilTimeStamp_TC_03_003_015_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * - Current timestamp "abs_time_state" has been got
 * @subsection MqSendUntilTimeStamp_TC_03_003_015_tcs Test Case Summary
 * This test shall checking the state of all Message queue API in Abnormal behavior in single thread
 * - Test03_002_006: checking state of R_OSAL_MqSendUntilTimeStamp
 * @subsection MqSendUntilTimeStamp_TC_03_003_015_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_MqSendUntilTimeStamp with valid paramter.
 * @subsection MqSendUntilTimeStamp_TC_03_003_015_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqSendUntilTimeStamp is OSAL_RETURN_STATE. 
 * @subsection MqSendUntilTimeStamp_TC_03_003_015_testcase Test Case Example
 * @snippet test_mq.cpp MqSendUntilTimeStamp_TC_03_003_015
 */
// [MqSendUntilTimeStamp_TC_03_003_015]
TEST_F(OsalApiStateTest, MqSendUntilTimeStamp_TC_03_003_015)
{
    osal_mq_handle_t mq_handle = OSAL_MQ_HANDLE_INVALID;
    st_osal_time_t current_time;
    int send_msg;

    // Test03_003_015
    // @brief: abnormal case: state transition error of R_OSAL_MqSendUntilTimeStamp
    current_time.tv_sec = abs_time_state.tv_sec + 1;
    current_time.tv_nsec = abs_time_state.tv_nsec;
    EXPECT_EQ(R_OSAL_MqSendUntilTimeStamp(mq_handle, &current_time, &send_msg, sizeof(send_msg)), OSAL_RETURN_STATE) << "Test03_003_015 failed";

}
// [MqSendUntilTimeStamp_TC_03_003_015]

/**
 * @page MQ_TEST MQ API Test
 * @section OSALINTTESTS_MqSendUntilTimeStamp_TC_03_003_016 OSALINTTESTS.MqSendUntilTimeStamp_TC_03_003_016
 * @subsection MqSendUntilTimeStamp_TC_03_003_016_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection MqSendUntilTimeStamp_TC_03_003_016_tci Test Case ID
 * MqSendUntilTimeStamp_S3_N_TC_03_003_016
 * @subsection MqSendUntilTimeStamp_TC_03_003_016_rr Related Requirement(s)
 * \#199172
 * @subsection MqSendUntilTimeStamp_TC_03_003_016_pp Prerequisite/Precondition
 * -# OSAL is initialized.<br>
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection MqSendUntilTimeStamp_TC_03_003_016_tcs Test Case Summary
 * @subsubsection MqSendUntilTimeStamp_TC_03_003_016_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Create a queue.
 * - Setup required memory for interrupt generation
 * - Register callack function and enable interrupt
 * - Wait for interrupt callback function finish
 * @subsubsection MqSendUntilTimeStamp_TC_03_003_016_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * - Reset created queue
 * - Destroy created queue
 * @subsubsection MqSendUntilTimeStamp_TC_03_003_016_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality with normal condition of the Message Queue APIs which are called in the ISR.
 * - Disable and clear interrupt status
 * - Test03_003_016: call R_OSAL_MqSendUntilTimeStamp in ISR
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection MqSendUntilTimeStamp_TC_03_003_016_tpts Test Procedure/Test Steps
 * @subsubsection MqSendUntilTimeStamp_TC_03_003_016_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Create a queue by calling function R_OSAL_MqCreate with bellow config
 *      - max_num_msg: 1
 *      - msg_size: 4
 * -# Allocate required memory for generate interrupt from hardware and get hw address of allocated memory.
 * -# Call R_OSAL_InterruptGetNumOfIrqChannels to get current number of interrupt channel.
 * -# Call R_OSAL_InterruptRegisterIsr to regsiter an interrupt with a callback function.
 * -# Calling R_OSAL_InterruptEnableIsr to enable interrupt.
 * -# Setup hardware specific register to generate the interrupt.
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection MqSendUntilTimeStamp_TC_03_003_016_post_tpts Test Procedure/Test Steps of Post-process
 * -# Calling R_OSAL_InterruptUnregisterIsr to unregister the interrupt.
 * -# Deallocate required memory
 * -# Calling reset queue by using R_OSAL_MqReset.
 * -# Delete the message queue by caliing R_OSAL_MqDelete.
 * @subsubsection MqSendUntilTimeStamp_TC_03_003_016_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Call R_OSAL_InterruptDisableIsr to disable interrupt signal.
 * -# Clear the status register by calling function R_OSAL_IoWrite32 with offset is 10 and value is TEST_CLEAR_REGISTER_VALUE.
 * -# Calling function R_OSAL_ClockTimeGetTimeStamp to get current timestamp.
 * -# Calling R_OSAL_MqSendUntilTimeStamp to send a message to the queue with above timestamp.
 * -# Calling R_OSAL_MqReceiveForTimePeriod to receive a message to queue with time period is 0.
 * -# Calling R_OSAL_MqSendForTimePeriod for Send a message that the interrupt context has ended
 * @subsection MqSendUntilTimeStamp_TC_03_003_016_ereb Expected Result/Behavior
 * @subsubsection MqSendUntilTimeStamp_TC_03_003_016_pre_ereb Expected Result/Behavior of Pre-process
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK.
 * -# The allocation successfully without any error.
 * -# The return value of R_OSAL_InterruptGetNumOfIrqChannels is OSAL_RETURN_OK.<br>
 * The return number of channel is greater than 0.
 * -# The return value of R_OSAL_InterruptRegisterIsr is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_InterruptEnableIsr is OSAL_RETURN_OK.
 * -# Setup hardware specific register to generate the interrupt success without any error.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection MqSendUntilTimeStamp_TC_03_003_016_post_ereb Expected Result/Behavior of Post-process
 * -# The return value of R_OSAL_InterruptUnregisterIsr is OSAL_RETURN_OK.
 * -# Deallocate required memory success without any error.
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqDelete is OSAL_RETURN_OK.
 * @subsubsection MqSendUntilTimeStamp_TC_03_003_016_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# The return value of R_OSAL_InterruptDisableIsr is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_IoWrite32 is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqSendUntilTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK and the returned message is as expected.
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * @subsection MqSendUntilTimeStamp_TC_03_003_016_testcase Test Case Example
 * @snippet test_mq.cpp MqSendUntilTimeStamp_TC_03_003_016
 * @snippet test_mq.cpp mq_test_int_cb1_normal_TC_03_003_016
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [MqSendUntilTimeStamp_TC_03_003_016]
TEST_F(OSALINTTESTS, MqSendUntilTimeStamp_TC_03_003_016)
{
    osal_mq_handle_t mq_handle;
    size_t numOfChannel = 0;
    st_osal_mq_config mq_config1;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE10;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    st_callback_arg_t arg;
    osal_milli_sec_t time_period = 2000;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};

/* pre-process:
 * create a queue
 * Setup required memory for interrupt generation.
 * Register callack function and enable interrupt 
 * Wait for interrupt callback function finish */
    mq_config1.max_num_msg = ISR_MQ_NUM_OF_MSG;
    mq_config1.msg_size = ISR_MQ_MSG_SIZE;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config1, ISR_MQ_ID , &mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK);
    arg.additional_arg = (void *)&mq_handle;
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, mq_test_int_cb1_normal_TC_03_003_016, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory
 * Destroy created handle */
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, mq_test_int_cb1_normal_TC_03_003_016), OSAL_RETURN_OK);
    deallocate_data_memory(hndl_mmngr, buffer_obj);
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK);
}
// [MqSendUntilTimeStamp_TC_03_003_016]

/**
 * @page MQ_TEST MQ API Test
 * @section OSALINTTESTS_MqSendUntilTimeStamp_TC_03_003_017 OSALINTTESTS.MqSendUntilTimeStamp_TC_03_003_017
 * @subsection MqSendUntilTimeStamp_TC_03_003_017_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection MqSendUntilTimeStamp_TC_03_003_017_tci Test Case ID
 * MqSendUntilTimeStamp_S2_A_TC_03_003_017
 * @subsection MqSendUntilTimeStamp_TC_03_003_017_rr Related Requirement(s)
 * \#199172
 * @subsection MqSendUntilTimeStamp_TC_03_003_017_pp Prerequisite/Precondition
 * -# OSAL is initialized.<br>
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection MqSendUntilTimeStamp_TC_03_003_017_tcs Test Case Summary
 * @subsubsection MqSendUntilTimeStamp_TC_03_003_017_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 *      - Create a queue.
 *      - Setup required memory for interrupt generation
 *      - Register callack function and enable interrupt
 *      - Wait for interrupt callback function finish
 * @subsubsection MqSendUntilTimeStamp_TC_03_003_017_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 *      - Unregister interrupt callback function
 *      - Deallocate allocated memory
 *      - Reset created queue
 *      - Destroy created queue
 * @subsubsection MqSendUntilTimeStamp_TC_03_003_017_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality with Abnormal condition of the Message Queue APIs which are called in the ISR.
 * - Disable and clear interrupt status
 * - Test03_003_017: call R_OSAL_MqSendUntilTimeStamp in ISR with full queue
 * @subsection MqSendUntilTimeStamp_TC_03_003_017_tpts Test Procedure/Test Steps
 * @subsubsection MqSendUntilTimeStamp_TC_03_003_017_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Create a queue by calling function R_OSAL_MqCreate with bellow config
 *      - max_num_msg: 1
 *      - msg_size: 4
 * -# Allocate required memory for generate interrupt from hardware and get hw address of allocated memory.
 * -# Call R_OSAL_InterruptGetNumOfIrqChannels to get current number of interrupt channel.
 * -# Call R_OSAL_InterruptRegisterIsr to regsiter an interrupt with a callback function.
 * -# Calling R_OSAL_InterruptEnableIsr to enable interrupt.
 * -# Setup hardware specific register to generate the interrupt.
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection MqSendUntilTimeStamp_TC_03_003_017_post_tpts Test Procedure/Test Steps of Post-process
 * -# Calling R_OSAL_InterruptUnregisterIsr to unregister the interrupt.
 * -# Deallocate required memory
 * -# Calling reset queue by using R_OSAL_MqReset.
 * -# Delete the message queue by caliing R_OSAL_MqDelete.
 * @subsubsection MqSendUntilTimeStamp_TC_03_003_017_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Call R_OSAL_InterruptDisableIsr to disable interrupt signal.
 * -# Clear the status register by calling function R_OSAL_IoWrite32 with offset is 10 and value is TEST_CLEAR_REGISTER_VALUE.
 * -# Calling R_OSAL_MqSendForTimePeriod to send a message to queue with time period is 0.
 * -# Calling function R_OSAL_ClockTimeGetTimeStamp to get current timestamp.
 * -# Calling R_OSAL_MqSendUntilTimeStamp to send a message to the queue with above timestamp.
 * -# Calling R_OSAL_MqReceiveForTimePeriod to receive the message from the queue.
 * -# Calling R_OSAL_MqSendForTimePeriod for Send a message that the interrupt context has ended
 * @subsection MqSendUntilTimeStamp_TC_03_003_017_ereb Expected Result/Behavior
 * @subsubsection MqSendUntilTimeStamp_TC_03_003_017_pre_ereb Expected Result/Behavior of Pre-process
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK.
 * -# The allocation successfully without any error.
 * -# The return value of R_OSAL_InterruptGetNumOfIrqChannels is OSAL_RETURN_OK.<br>
 * The return number of channel is greater than 0.
 * -# The return value of R_OSAL_InterruptRegisterIsr is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_InterruptEnableIsr is OSAL_RETURN_OK.
 * -# Setup hardware specific register to generate the interrupt success without any error.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection MqSendUntilTimeStamp_TC_03_003_017_post_ereb Expected Result/Behavior of Post-process
 * -# The return value of R_OSAL_InterruptUnregisterIsr is OSAL_RETURN_OK.
 * -# Deallocate required memory success without any error.
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqDelete is OSAL_RETURN_OK.
 * @subsubsection MqSendUntilTimeStamp_TC_03_003_017_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# The return value of R_OSAL_InterruptDisableIsr is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_IoWrite32 is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqSendUntilTimeStamp is OSAL_RETURN_BUSY.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK and the returned message is as expected.
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * @subsection MqSendUntilTimeStamp_TC_03_003_017_testcase Test Case Example
 * @snippet test_mq.cpp MqSendUntilTimeStamp_TC_03_003_017
 * @snippet test_mq.cpp mq_test_int_cb1_abnormal_TC_03_003_017
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [MqSendUntilTimeStamp_TC_03_003_017]
TEST_F(OSALINTTESTS, MqSendUntilTimeStamp_TC_03_003_017)
{
    osal_mq_handle_t mq_handle;
    size_t numOfChannel = 0;
    st_osal_mq_config mq_config1;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE10;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    st_callback_arg_t arg;
    osal_milli_sec_t time_period = 2000;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};

/* pre-process:
 * create a queue
 * Setup required memory for interrupt generation.
 * Register callack function and enable interrupt 
 * Wait for interrupt callback function finish */
    mq_config1.max_num_msg = ISR_MQ_NUM_OF_MSG;
    mq_config1.msg_size = ISR_MQ_MSG_SIZE;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config1, ISR_MQ_ID , &mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK);
    arg.additional_arg = (void *)&mq_handle;
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, mq_test_int_cb1_abnormal_TC_03_003_017, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory
 * Destroy created handle */
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, mq_test_int_cb1_abnormal_TC_03_003_017), OSAL_RETURN_OK);
    deallocate_data_memory(hndl_mmngr, buffer_obj);
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK);
}
// [MqSendUntilTimeStamp_TC_03_003_017]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendUntilTimeStamp_TC_03_003_018 MQApiTest.MqSendUntilTimeStamp_TC_03_003_018
 * @subsection MqSendUntilTimeStamp_TC_03_003_018_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendUntilTimeStamp_TC_03_003_018_tci Test Case ID
 * MqSendUntilTimeStamp_S0_BA_TC_03_003_018
 * @subsection MqSendUntilTimeStamp_TC_03_003_018_rr Related Requirement(s)
 * \#199172
 * @subsection MqSendUntilTimeStamp_TC_03_003_018_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqSendUntilTimeStamp_TC_03_003_018_tcs Test Case Summary
 * - Test03_003_018: send message to queue with buffer_size parameter is bigger than message size
 * @subsection MqSendUntilTimeStamp_TC_03_003_018_tpts Test Procedure/Test Steps
 * -# Get mq config by using R_OSAL_MqGetConfig.<br>
 * Increase "mq_config.msg_size" by 1.
 * -# Get current time stamp by R_OSAL_ClockTimeGetTimeStamp.
 * -# Send a message to queue with size by using R_OSAL_MqSendUntilTimeStamp with "mq_config.msg_size" as above
 * @subsection MqSendUntilTimeStamp_TC_03_003_018_ereb Expected Result/Behavior
 * -# R_OSAL_MqGetConfig return OSAL_RETURN_OK
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK
 * -# R_OSAL_MqSendUntilTimeStamp return OSAL_RETURN_PAR
 * @subsection MqSendUntilTimeStamp_TC_03_003_018_testcase Test Case Example
 * @snippet test_mq.cpp MqSendUntilTimeStamp_TC_03_003_018
 */
// [MqSendUntilTimeStamp_TC_03_003_018]
TEST_F(MQApiTest, MqSendUntilTimeStamp_TC_03_003_018)
{
    st_osal_time_t  current_time;

    // Test03_003_018
    // @brief: abnormal case: message size is bigger than mq config message size
    EXPECT_EQ(R_OSAL_MqGetConfig(shared_queue, &mq_config), OSAL_RETURN_OK) << "Test03_003_018 failed";
    mq_config.msg_size += 1;
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK) << "Test03_003_018 failed";
    if(current_time.tv_nsec + 2 * MSEC_TO_NSEC <= MAX_NSEC)
    {
        current_time.tv_nsec = current_time.tv_nsec + 2 * MSEC_TO_NSEC;
        current_time.tv_sec = current_time.tv_sec + 1;
    }
    else
    {
        current_time.tv_nsec = 2 * MSEC_TO_NSEC - (MAX_NSEC - current_time.tv_nsec);
        current_time.tv_sec = current_time.tv_sec + 2;
    }
    EXPECT_EQ(R_OSAL_MqSendUntilTimeStamp(shared_queue, &current_time, &send_msg, mq_config.msg_size), OSAL_RETURN_PAR) << "Test03_003_018 failed";
}
// [MqSendUntilTimeStamp_TC_03_003_018]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendUntilTimeStamp_TC_03_003_019 MQApiTest.MqSendUntilTimeStamp_TC_03_003_019
 * @subsection MqSendUntilTimeStamp_TC_03_003_019_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendUntilTimeStamp_TC_03_003_019_tci Test Case ID
 * MqSendUntilTimeStamp_S0_BA_TC_03_003_019
 * @subsection MqSendUntilTimeStamp_TC_03_003_019_rr Related Requirement(s)
 * \#199172
 * @subsection MqSendUntilTimeStamp_TC_03_003_019_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqSendUntilTimeStamp_TC_03_003_019_tcs Test Case Summary
 * - Test03_003_019: send message to queue with buffer_size parameter is less than message size
 * @subsection MqSendUntilTimeStamp_TC_03_003_019_tpts Test Procedure/Test Steps
 * -# Get mq config by using R_OSAL_MqGetConfig.<br>
 * Decrease "mq_config.msg_size" by 1.
 * -# Get current time stamp by R_OSAL_ClockTimeGetTimeStamp.
 * -# Send a message to queue with size by using R_OSAL_MqSendUntilTimeStamp with "mq_config.msg_size" as above
 * @subsection MqSendUntilTimeStamp_TC_03_003_019_ereb Expected Result/Behavior
 * -# R_OSAL_MqGetConfig return OSAL_RETURN_OK
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK
 * -# R_OSAL_MqSendUntilTimeStamp return OSAL_RETURN_PAR
 * @subsection MqSendUntilTimeStamp_TC_03_003_019_testcase Test Case Example
 * @snippet test_mq.cpp MqSendUntilTimeStamp_TC_03_003_019
 */
// [MqSendUntilTimeStamp_TC_03_003_019]
TEST_F(MQApiTest, MqSendUntilTimeStamp_TC_03_003_019)
{
    st_osal_time_t  current_time;

    // Test03_003_019
    // @brief: abnormal case: message size is less than mq config message size
    EXPECT_EQ(R_OSAL_MqGetConfig(shared_queue, &mq_config), OSAL_RETURN_OK) << "Test03_003_019 failed";
    mq_config.msg_size -= 1;
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK) << "Test03_003_019 failed";
    if(current_time.tv_nsec + 2 * MSEC_TO_NSEC <= MAX_NSEC)
    {
        current_time.tv_nsec = current_time.tv_nsec + 2 * MSEC_TO_NSEC;
        current_time.tv_sec = current_time.tv_sec + 1;
    }
    else
    {
        current_time.tv_nsec = 2 * MSEC_TO_NSEC - (MAX_NSEC - current_time.tv_nsec);
        current_time.tv_sec = current_time.tv_sec + 2;
    }
    EXPECT_EQ(R_OSAL_MqSendUntilTimeStamp(shared_queue, &current_time, &send_msg, mq_config.msg_size), OSAL_RETURN_PAR) << "Test03_003_019 failed";
}
// [MqSendUntilTimeStamp_TC_03_003_019]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendUntilTimeStamp_TC_03_003_020 MQApiTest.MqSendUntilTimeStamp_TC_03_003_020
 * @subsection MqSendUntilTimeStamp_TC_03_003_020_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendUntilTimeStamp_TC_03_003_020_tci Test Case ID
 * MqSendUntilTimeStamp_S3_N_TC_03_003_020
 * @subsection MqSendUntilTimeStamp_TC_03_003_020_rr Related Requirement(s)
 * \#199169, \#199172
 * @subsection MqSendUntilTimeStamp_TC_03_003_020_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqSendUntilTimeStamp_TC_03_003_020_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqSendUntilTimeStamp API in Normal behavior in single thread
 * - Test03_003_020: confirm that the contents of MQ are not rewritten by R_OSAL_MqSendUntilTimeStamp timeout
 * @subsection MqSendUntilTimeStamp_TC_03_003_020_tpts Test Procedure/Test Steps
 * -# Get "shared_queue" config by calling R_OSAL_MqGetConfig.
 * -# Use the "for" loop to send 2 messages 0x1234 and 0x5678 to the "shared_queue" with steps as below:
 *      - Get the current timestamp by using R_OSAL_ClockTimeGetTimeStamp
 *      - Send 2 messages 0x1234 and 0x5678 by calling R_OSAL_MqSendUntilTimeStamp
 * -# Get current time stamp by calling R_OSAL_ClockTimeGetTimeStamp.
 * -# Send 0x3456 message to the "shared_queue" by calling R_OSAL_ClockTimeGetTimeStamp.
 * -# Call R_OSAL_MqReceiveForTimePeriod 2 times to receive 2 message from the "shared_queue".
 * @subsection MqSendUntilTimeStamp_TC_03_003_020_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqGetConfig is OSAL_RETURN_OK.
 * -# In "for" loop, the expected return code is:
 *      - The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 *      - The return value of 2 times call R_OSAL_MqSendUntilTimeStamp is OSAL_RETURN_OK.
 * -# The return of the 3rd R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return of the 3rd R_OSAL_MqSendUntilTimeStamp is OSAL_RETURN_TIME.
 * -# The return value of 2 times call R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK. <br>
 * The 1st receive message is 0x1234, the 2nd receive message is 0x5678.
 * @subsection MqSendUntilTimeStamp_TC_03_003_020_testcase Test Case Example
 * @snippet test_mq.cpp MqSendUntilTimeStamp_TC_03_003_020
 */
// [MqSendUntilTimeStamp_TC_03_003_020]
TEST_F(MQApiTest, MqSendUntilTimeStamp_TC_03_003_020)
{
    st_osal_time_t  current_time;
    osal_milli_sec_t time_period_10ms = 10;
    int send_msg_data[3] = {0x1234, 0x5678, 0x3456};

    // Test03_003_020
    // @brief: normal case: confirm that the contents of MQ are not rewritten by SendAPI
    EXPECT_EQ(R_OSAL_MqGetConfig(shared_queue, &mq_config), OSAL_RETURN_OK) << "Test03_003_020 failed";
    // send 2 message 0x1234 and 0x5678 to the queue
    for (int i = 0; i < mq_config.max_num_msg; i++)
    {
        EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK) << "Test03_003_020 failed";
        if(current_time.tv_nsec + 2 * MSEC_TO_NSEC <= MAX_NSEC)
        {
            current_time.tv_nsec = current_time.tv_nsec + 2 * MSEC_TO_NSEC;
            current_time.tv_sec = current_time.tv_sec + 1;
        }
        else
        {
            current_time.tv_nsec = 2 * MSEC_TO_NSEC - (MAX_NSEC - current_time.tv_nsec);
            current_time.tv_sec = current_time.tv_sec + 2;
        }
        EXPECT_EQ(R_OSAL_MqSendUntilTimeStamp(shared_queue, &current_time, &send_msg_data[i], mq_config.msg_size), OSAL_RETURN_OK) << "Test03_003_020 failed";
    }
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK) << "Test03_003_020 failed";
    if(current_time.tv_nsec + 2 * MSEC_TO_NSEC <= MAX_NSEC)
    {
        current_time.tv_nsec = current_time.tv_nsec + 2 * MSEC_TO_NSEC;
        current_time.tv_sec = current_time.tv_sec + 1;
    }
    else
    {
        current_time.tv_nsec = 2 * MSEC_TO_NSEC - (MAX_NSEC - current_time.tv_nsec);
        current_time.tv_sec = current_time.tv_sec + 2;
    }
    EXPECT_EQ(R_OSAL_MqSendUntilTimeStamp(shared_queue, &current_time, &send_msg_data[2], mq_config.msg_size), OSAL_RETURN_TIME) << "Test03_003_020 failed";
    //receive 2 messages from the queue
    for (int i = 0; i < mq_config.max_num_msg; i++)
    {
        EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(shared_queue, time_period_10ms, &receive_msg, mq_config.msg_size), OSAL_RETURN_OK) << "Test03_003_020 failed";
        EXPECT_EQ(receive_msg, send_msg_data[i]) << "Test03_003_020 failed";
    }
}
// [MqSendUntilTimeStamp_TC_03_003_020]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendUntilTimeStamp_TC_03_003_021 MQApiTest.MqSendUntilTimeStamp_TC_03_003_021
 * @subsection MqSendUntilTimeStamp_TC_03_003_021_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendUntilTimeStamp_TC_03_003_021_tci Test Case ID
 * MqSendUntilTimeStamp_S0_A_TC_03_003_021
 * @subsection MqSendUntilTimeStamp_TC_03_003_021_rr Related Requirement(s)
 * \#199172
 * @subsection MqSendUntilTimeStamp_TC_03_003_021_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqSendUntilTimeStamp_TC_03_003_021_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqSendUntilTimeStamp API in Abnormal behavior in single thread
 * - Test03_003_021: send message to queue with timestamp.tv_sec is a negative value.
 * @subsection MqSendUntilTimeStamp_TC_03_003_021_tpts Test Procedure/Test Steps
 * -# Get the queue config by calling R_OSAL_MqGetConfig.
 * -# Get current time stamp by calling R_OSAL_ClockTimeGetTimeStamp.
 * -# Call R_OSAL_MqSendUntilTimeStamp with timestamp.tv_sec is -1.
 * @subsection MqSendUntilTimeStamp_TC_03_003_021_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqGetConfig is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqSendUntilTimeStamp is OSAL_RETURN_PAR.
 * @subsection MqSendUntilTimeStamp_TC_03_003_021_testcase Test Case Example
 * @snippet test_mq.cpp MqSendUntilTimeStamp_TC_03_003_021
 */
// [MqSendUntilTimeStamp_TC_03_003_021]
TEST_F(MQApiTest, MqSendUntilTimeStamp_TC_03_003_021)
{
    st_osal_time_t  current_time;

    // Test03_003_021
    // @brief: abnormal case: tv_sec is negative number
    EXPECT_EQ(R_OSAL_MqGetConfig(shared_queue, &mq_config), OSAL_RETURN_OK) << "Test03_003_021 failed";
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK) << "Test03_003_021 failed";
    current_time.tv_sec = -1;
    EXPECT_EQ(R_OSAL_MqSendUntilTimeStamp(shared_queue, &current_time, &send_msg, mq_config.msg_size), OSAL_RETURN_PAR) << "Test03_003_021 failed";
}
// [MqSendUntilTimeStamp_TC_03_003_021]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendUntilTimeStamp_TC_03_003_023 MQApiTest.MqSendUntilTimeStamp_TC_03_003_023
 * @subsection MqSendUntilTimeStamp_TC_03_003_023_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendUntilTimeStamp_TC_03_003_023_tci Test Case ID
 * MqSendUntilTimeStamp_S0_BN_TC_03_003_023
 * @subsection MqSendUntilTimeStamp_TC_03_003_023_rr Related Requirement(s)
 * \#199172
 * @subsection MqSendUntilTimeStamp_TC_03_003_023_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqSendUntilTimeStamp_TC_03_003_023_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqSendUntilTimeStamp API
 * - Test03_003_023: send message to queue with tv_sec is equal to zero.
 * @subsection MqSendUntilTimeStamp_TC_03_003_023_tpts Test Procedure/Test Steps
 * -# Get the queue config by calling R_OSAL_MqGetConfig.
 * -# Get current time stamp by calling R_OSAL_ClockTimeGetTimeStamp.
 * -# Call R_OSAL_MqSendUntilTimeStamp with tv_sec is equal to zero.
 * -# Reset the queue by calling to R_OSAL_MqReset
 * @subsection MqSendUntilTimeStamp_TC_03_003_023_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqGetConfig is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqSendUntilTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_OK.
 * @subsection MqSendUntilTimeStamp_TC_03_003_023_testcase Test Case Example
 * @snippet test_mq.cpp MqSendUntilTimeStamp_TC_03_003_023
 */
// [MqSendUntilTimeStamp_TC_03_003_023]
TEST_F(MQApiTest, MqSendUntilTimeStamp_TC_03_003_023)
{
    st_osal_time_t  current_time;

    // Test03_003_023
    // @brief: boundary normal case: tv_sec is equal to zero
    EXPECT_EQ(R_OSAL_MqGetConfig(shared_queue, &mq_config), OSAL_RETURN_OK) << "Test03_003_023 failed";
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK) << "Test03_003_023 failed";
    current_time.tv_sec = 0;
    EXPECT_EQ(R_OSAL_MqSendUntilTimeStamp(shared_queue, &current_time, &send_msg, mq_config.msg_size), OSAL_RETURN_OK) << "Test03_003_023 failed";
    EXPECT_EQ(R_OSAL_MqReset(shared_queue), OSAL_RETURN_OK) << "Test03_003_023 failed";
}
// [MqSendUntilTimeStamp_TC_03_003_023]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendUntilTimeStamp_TC_03_003_024 MQApiTest.MqSendUntilTimeStamp_TC_03_003_024
 * @subsection MqSendUntilTimeStamp_TC_03_003_024_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendUntilTimeStamp_TC_03_003_024_tci Test Case ID
 * MqSendUntilTimeStamp_S0_BN_TC_03_003_024
 * @subsection MqSendUntilTimeStamp_TC_03_003_024_rr Related Requirement(s)
 * \#199172
 * @subsection MqSendUntilTimeStamp_TC_03_003_024_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqSendUntilTimeStamp_TC_03_003_024_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqSendUntilTimeStamp API
 * - Test03_003_024: send message to queue with tv_nsec is equal to zero.
 * @subsection MqSendUntilTimeStamp_TC_03_003_024_tpts Test Procedure/Test Steps
 * -# Get the queue config by calling R_OSAL_MqGetConfig.
 * -# Get current time stamp by calling R_OSAL_ClockTimeGetTimeStamp.
 * -# Call R_OSAL_MqSendUntilTimeStamp with tv_nsec is equal to zero.
 * -# Reset the queue by calling to R_OSAL_MqReset
 * @subsection MqSendUntilTimeStamp_TC_03_003_024_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqGetConfig is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqSendUntilTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_OK.
 * @subsection MqSendUntilTimeStamp_TC_03_003_024_testcase Test Case Example
 * @snippet test_mq.cpp MqSendUntilTimeStamp_TC_03_003_024
 */
// [MqSendUntilTimeStamp_TC_03_003_024]
TEST_F(MQApiTest, MqSendUntilTimeStamp_TC_03_003_024)
{
    st_osal_time_t  current_time;

    // Test03_003_024
    // @brief: boundary normal case: tv_nsec is equal to zero
    EXPECT_EQ(R_OSAL_MqGetConfig(shared_queue, &mq_config), OSAL_RETURN_OK) << "Test03_003_024 failed";
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK) << "Test03_003_024 failed";
    current_time.tv_nsec = 0;
    EXPECT_EQ(R_OSAL_MqSendUntilTimeStamp(shared_queue, &current_time, &send_msg, mq_config.msg_size), OSAL_RETURN_OK) << "Test03_003_024 failed";
    EXPECT_EQ(R_OSAL_MqReset(shared_queue), OSAL_RETURN_OK) << "Test03_003_024 failed";
}
// [MqSendUntilTimeStamp_TC_03_003_024]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendUntilTimeStamp_TC_03_003_025 MQApiTest.MqSendUntilTimeStamp_TC_03_003_025
 * @subsection MqSendUntilTimeStamp_TC_03_003_025_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendUntilTimeStamp_TC_03_003_025_tci Test Case ID
 * MqSendUntilTimeStamp_S0_BA_TC_03_003_025
 * @subsection MqSendUntilTimeStamp_TC_03_003_025_rr Related Requirement(s)
 * \#199172
 * @subsection MqSendUntilTimeStamp_TC_03_003_025_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqSendUntilTimeStamp_TC_03_003_025_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqSendUntilTimeStamp API
 * - Test03_003_025: send message to queue with tv_nsec is less than zero
 * @subsection MqSendUntilTimeStamp_TC_03_003_025_tpts Test Procedure/Test Steps
 * -# Get the queue config by calling R_OSAL_MqGetConfig.
 * -# Get current time stamp by calling R_OSAL_ClockTimeGetTimeStamp.
 * -# Call R_OSAL_MqSendUntilTimeStamp with tv_nsec is less than zero
 * @subsection MqSendUntilTimeStamp_TC_03_003_025_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqGetConfig is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqSendUntilTimeStamp is OSAL_RETURN_PAR.
 * @subsection MqSendUntilTimeStamp_TC_03_003_025_testcase Test Case Example
 * @snippet test_mq.cpp MqSendUntilTimeStamp_TC_03_003_025
 */
// [MqSendUntilTimeStamp_TC_03_003_025]
TEST_F(MQApiTest, MqSendUntilTimeStamp_TC_03_003_025)
{
    st_osal_time_t  current_time;

    // Test03_003_025
    // @brief: boundary abnormal case: tv_nsec is less than zero
    EXPECT_EQ(R_OSAL_MqGetConfig(shared_queue, &mq_config), OSAL_RETURN_OK) << "Test03_003_025 failed";
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK) << "Test03_003_025 failed";
    current_time.tv_nsec = -1;
    EXPECT_EQ(R_OSAL_MqSendUntilTimeStamp(shared_queue, &current_time, &send_msg, mq_config.msg_size), OSAL_RETURN_PAR) << "Test03_003_025 failed";
}
// [MqSendUntilTimeStamp_TC_03_003_025]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendUntilTimeStamp_TC_03_003_026 MQApiTest.MqSendUntilTimeStamp_TC_03_003_026
 * @subsection MqSendUntilTimeStamp_TC_03_003_026_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendUntilTimeStamp_TC_03_003_026_tci Test Case ID
 * MqSendUntilTimeStamp_S0_BN_TC_03_003_026
 * @subsection MqSendUntilTimeStamp_TC_03_003_026_rr Related Requirement(s)
 * \#199172
 * @subsection MqSendUntilTimeStamp_TC_03_003_026_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqSendUntilTimeStamp_TC_03_003_026_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqSendUntilTimeStamp API
 * - Test03_003_026: send message to queue with tv_sec and tv_nsec are equal to zero
 * @subsection MqSendUntilTimeStamp_TC_03_003_026_tpts Test Procedure/Test Steps
 * -# Get the queue config by calling R_OSAL_MqGetConfig.
 * -# Call R_OSAL_MqSendUntilTimeStamp with tv_sec and tv_nsec are equal to zero
 * -# Reset the queue by calling to R_OSAL_MqReset
 * @subsection MqSendUntilTimeStamp_TC_03_003_026_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqGetConfig is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqSendUntilTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_OK.
 * @subsection MqSendUntilTimeStamp_TC_03_003_026_testcase Test Case Example
 * @snippet test_mq.cpp MqSendUntilTimeStamp_TC_03_003_026
 */
// [MqSendUntilTimeStamp_TC_03_003_026]
TEST_F(MQApiTest, MqSendUntilTimeStamp_TC_03_003_026)
{
    st_osal_time_t  current_time;

    // Test03_003_026
    // @brief: boundary normal case: tv_sec and tv_nsec are equal to zero
    EXPECT_EQ(R_OSAL_MqGetConfig(shared_queue, &mq_config), OSAL_RETURN_OK) << "Test03_003_026 failed";
    current_time.tv_nsec = 0;
	current_time.tv_sec = 0;
    EXPECT_EQ(R_OSAL_MqSendUntilTimeStamp(shared_queue, &current_time, &send_msg, mq_config.msg_size), OSAL_RETURN_OK) << "Test03_003_026 failed";
    EXPECT_EQ(R_OSAL_MqReset(shared_queue), OSAL_RETURN_OK) << "Test03_003_026 failed";
}
// [MqSendUntilTimeStamp_TC_03_003_026]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendForTimePeriod_TC_03_004_001 MQApiTest.MqSendForTimePeriod_TC_03_004_001
 * @subsection MqSendForTimePeriod_TC_03_004_001_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendForTimePeriod_TC_03_004_001_tci Test Case ID
 * MqSendForTimePeriod_S1_N_TC_03_004_001
 * @subsection MqSendForTimePeriod_TC_03_004_001_rr Related Requirement(s)
 * \#199173
 * @subsection MqSendForTimePeriod_TC_03_004_001_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqSendForTimePeriod_TC_03_004_001_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqSendForTimePeriod API in Normal behavior in single thread
 * - Test03_004_001: send a message to queue within 10ms.
 * @subsection MqSendForTimePeriod_TC_03_004_001_tpts Test Procedure/Test Steps
 * -# Send a message to queue using R_OSAL_MqSendForTimePeriod with the time period = 10.
 * -# Receive message by R_OSAL_MqReceiveForTimePeriod.
 * @subsection MqSendForTimePeriod_TC_03_004_001_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK and the received message is the same as the sent message.
 * @subsection MqSendForTimePeriod_TC_03_004_001_testcase Test Case Example
 * @snippet test_mq.cpp MqSendForTimePeriod_TC_03_004_001
 */
// [MqSendForTimePeriod_TC_03_004_001]
TEST_F(MQApiTest, MqSendForTimePeriod_TC_03_004_001)
{
    osal_milli_sec_t time_period_10ms = 10;

    // Test03_004_001
    // @brief: normal case: time_period = 10
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue, time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK) << "Test03_004_001 failed";
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(shared_queue, time_period_10ms,  &receive_msg, sizeof(receive_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);
    receive_msg = 0;
}
// [MqSendForTimePeriod_TC_03_004_001]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendForTimePeriod_TC_03_004_002 MQApiTest.MqSendForTimePeriod_TC_03_004_002
 * @subsection MqSendForTimePeriod_TC_03_004_002_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendForTimePeriod_TC_03_004_002_tci Test Case ID
 * MqSendForTimePeriod_S1_BN_TC_03_004_002
 * @subsection MqSendForTimePeriod_TC_03_004_002_rr Related Requirement(s)
 * \#199173
 * @subsection MqSendForTimePeriod_TC_03_004_002_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqSendForTimePeriod_TC_03_004_002_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqSendForTimePeriod API in Normal behavior in single thread
 * - Test03_004_002: send a message to queue with time period is 0ms.
 * @subsection MqSendForTimePeriod_TC_03_004_002_tpts Test Procedure/Test Steps
 * -# Send a message to queue using R_OSAL_MqSendForTimePeriod with the time period = 0.
 * -# Receive message by R_OSAL_MqReceiveForTimePeriod.
 * @subsection MqSendForTimePeriod_TC_03_004_002_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK and the received message is the same as the sent message.
 * @subsection MqSendForTimePeriod_TC_03_004_002_testcase Test Case Example
 * @snippet test_mq.cpp MqSendForTimePeriod_TC_03_004_002
 */
// [MqSendForTimePeriod_TC_03_004_002]
TEST_F(MQApiTest, MqSendForTimePeriod_TC_03_004_002)
{
    osal_milli_sec_t time_period_10ms = 10;
    osal_milli_sec_t time_period_0ms = 0;

    // Test03_004_002
    // @brief: normal case: time_period = 0
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue, time_period_0ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK) << "Test03_004_002 failed";
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(shared_queue,time_period_10ms,  &receive_msg, sizeof(receive_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);
    receive_msg = 0;
}
// [MqSendForTimePeriod_TC_03_004_002]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendForTimePeriod_TC_03_004_003 MQApiTest.MqSendForTimePeriod_TC_03_004_003
 * @subsection MqSendForTimePeriod_TC_03_004_003_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendForTimePeriod_TC_03_004_003_tci Test Case ID
 * MqSendForTimePeriod_S0_A_TC_03_004_003
 * @subsection MqSendForTimePeriod_TC_03_004_003_rr Related Requirement(s)
 * \#199151, \#199165, \#199173
 * @subsection MqSendForTimePeriod_TC_03_004_003_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqSendForTimePeriod_TC_03_004_003_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqSendForTimePeriod API in Abnormal behavior in single thread
 * - Test03_004_003: send a message to queue with mq handle parameter is NULL.
 * @subsection MqSendForTimePeriod_TC_03_004_003_tpts Test Procedure/Test Steps
 * -# Send a message to queue using R_OSAL_MqSendForTimePeriod with mq_handle is NULL.
 * @subsection MqSendForTimePeriod_TC_03_004_003_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_HANDLE.
 * @subsection MqSendForTimePeriod_TC_03_004_003_testcase Test Case Example
 * @snippet test_mq.cpp MqSendForTimePeriod_TC_03_004_003
 */
// [MqSendForTimePeriod_TC_03_004_003]
TEST_F(MQApiTest, MqSendForTimePeriod_TC_03_004_003)
{

    osal_milli_sec_t time_period_10ms = 10;

    // Test03_004_003
    // @brief: abnormal case: mq_handle is NULL
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(OSAL_MQ_HANDLE_INVALID, time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_HANDLE) << "Test03_004_003 failed";

}
// [MqSendForTimePeriod_TC_03_004_003]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendForTimePeriod_TC_03_004_004 MQApiTest.MqSendForTimePeriod_TC_03_004_004
 * @subsection MqSendForTimePeriod_TC_03_004_004_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendForTimePeriod_TC_03_004_004_tci Test Case ID
 * MqSendForTimePeriod_S0_BA_TC_03_004_004
 * @subsection MqSendForTimePeriod_TC_03_004_004_rr Related Requirement(s)
 * \#199173
 * @subsection MqSendForTimePeriod_TC_03_004_004_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqSendForTimePeriod_TC_03_004_004_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqSendForTimePeriod API in Abnormal behavior in single thread
 * - Test03_004_004: send a message to queue with time period is negative number (-100ms).
 * @subsection MqSendForTimePeriod_TC_03_004_004_tpts Test Procedure/Test Steps
 * -# Send a message to queue using R_OSAL_MqSendForTimePeriod with the time period = -100.
 * @subsection MqSendForTimePeriod_TC_03_004_004_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_PAR.
 * @subsection MqSendForTimePeriod_TC_03_004_004_testcase Test Case Example
 * @snippet test_mq.cpp MqSendForTimePeriod_TC_03_004_004
 */
// [MqSendForTimePeriod_TC_03_004_004]
TEST_F(MQApiTest, MqSendForTimePeriod_TC_03_004_004)
{
    osal_milli_sec_t time_period_negative_100ms = -100;

    // Test03_004_004
    // @brief: abnormal case: time_period = -100
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_negative_100ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_PAR) << "Test03_004_004 failed";
}
// [MqSendForTimePeriod_TC_03_004_004]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendForTimePeriod_TC_03_004_005 MQApiTest.MqSendForTimePeriod_TC_03_004_005
 * @subsection MqSendForTimePeriod_TC_03_004_005_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendForTimePeriod_TC_03_004_005_tci Test Case ID
 * MqSendForTimePeriod_S0_A_TC_03_004_005
 * @subsection MqSendForTimePeriod_TC_03_004_005_rr Related Requirement(s)
 * \#199173
 * @subsection MqSendForTimePeriod_TC_03_004_005_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqSendForTimePeriod_TC_03_004_005_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqSendForTimePeriod API in Abnormal behavior in single thread
 * - Test03_004_005: send a message to queue with p_buffer parameter is NULL.
 * @subsection MqSendForTimePeriod_TC_03_004_005_tpts Test Procedure/Test Steps
 * -# Send a message to queue using R_OSAL_MqSendForTimePeriod with the p_buffer = NULL.
 * @subsection MqSendForTimePeriod_TC_03_004_005_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_PAR.
 * @subsection MqSendForTimePeriod_TC_03_004_005_testcase Test Case Example
 * @snippet test_mq.cpp MqSendForTimePeriod_TC_03_004_005
 */
// [MqSendForTimePeriod_TC_03_004_005]
TEST_F(MQApiTest, MqSendForTimePeriod_TC_03_004_005)
{
    osal_milli_sec_t time_period_10ms = 10;
    void * p_buffer;

    // Test03_004_005
    // @brief: abnormal case: p_buffer = NULL
    p_buffer = NULL;
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_10ms, p_buffer, sizeof(send_msg)), OSAL_RETURN_PAR) << "Test03_004_005 failed";
}
// [MqSendForTimePeriod_TC_03_004_005]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendForTimePeriod_TC_03_004_006 MQApiTest.MqSendForTimePeriod_TC_03_004_006
 * @subsection MqSendForTimePeriod_TC_03_004_006_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendForTimePeriod_TC_03_004_006_tci Test Case ID
 * MqSendForTimePeriod_S0_A_TC_03_004_006
 * @subsection MqSendForTimePeriod_TC_03_004_006_rr Related Requirement(s)
 * \#199173
 * @subsection MqSendForTimePeriod_TC_03_004_006_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqSendForTimePeriod_TC_03_004_006_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqSendForTimePeriod API in Abnormal behavior in single thread
 * - Test03_004_006: send a message to queue with buffer size parameter is 0.
 * @subsection MqSendForTimePeriod_TC_03_004_006_tpts Test Procedure/Test Steps
 * -# Send a message to queue using R_OSAL_MqSendForTimePeriod with the buffer size = 0.
 * @subsection MqSendForTimePeriod_TC_03_004_006_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_PAR.
 * @subsection MqSendForTimePeriod_TC_03_004_006_testcase Test Case Example
 * @snippet test_mq.cpp MqSendForTimePeriod_TC_03_004_006
 */
// [MqSendForTimePeriod_TC_03_004_006]
TEST_F(MQApiTest, MqSendForTimePeriod_TC_03_004_006)
{
    osal_milli_sec_t time_period_10ms = 10;
    size_t buffer_size;

    // Test03_004_006
    // @brief: abnormal case: buffer_size = 0
    buffer_size = 0;
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_10ms, &send_msg, buffer_size), OSAL_RETURN_PAR) << "Test03_004_006 failed";
}
// [MqSendForTimePeriod_TC_03_004_006]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendForTimePeriod_TC_03_004_007 MQApiTest.MqSendForTimePeriod_TC_03_004_007
 * @subsection MqSendForTimePeriod_TC_03_004_007_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendForTimePeriod_TC_03_004_007_tci Test Case ID
 * MqSendForTimePeriod_S2_A_TC_03_004_007
 * @subsection MqSendForTimePeriod_TC_03_004_007_rr Related Requirement(s)
 * \#199173
 * @subsection MqSendForTimePeriod_TC_03_004_007_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqSendForTimePeriod_TC_03_004_007_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqSendForTimePeriod API in Abnormal behavior in single thread
 * - Test03_004_007: send a message to queue that already deleted.
 * @subsection MqSendForTimePeriod_TC_03_004_007_tpts Test Procedure/Test Steps
 * -# Create a new queue by calling R_OSAL_MqCreate.
 * -# Delete created queue by calling R_OSAL_MqDelete. 
 * -# Send message to the queue by calling R_OSAL_MqSendForTimePeriod with all valid parameter.
 * @subsection MqSendForTimePeriod_TC_03_004_007_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK, and the return handle is not NULL_PTR. 
 * -# The return value of R_OSAL_MqDelete is OSAL_RETURN_OK. 
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_HANDLE.
 * @subsection MqSendForTimePeriod_TC_03_004_007_testcase Test Case Example
 * @snippet test_mq.cpp MqSendForTimePeriod_TC_03_004_007
 */
// [MqSendForTimePeriod_TC_03_004_007]
TEST_F(MQApiTest, MqSendForTimePeriod_TC_03_004_007)
{
    osal_milli_sec_t time_period_10ms = 10;

    // Test03_004_007
    // @brief: abnormal case: mq_handle has already deleted
    mq_config.max_num_msg = 1;
    mq_config.msg_size = 4;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config, TEST_MQ_ID2, &mq_handle), OSAL_RETURN_OK);
    EXPECT_NE(mq_handle, NULL_PTR);
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(mq_handle,time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_HANDLE) << "Test03_004_007 failed";
}
// [MqSendForTimePeriod_TC_03_004_007]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendForTimePeriod_TC_03_004_008 MQApiTest.MqSendForTimePeriod_TC_03_004_008
 * @subsection MqSendForTimePeriod_TC_03_004_008_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendForTimePeriod_TC_03_004_008_tci Test Case ID
 * MqSendForTimePeriod_S2_A_TC_03_004_008
 * @subsection MqSendForTimePeriod_TC_03_004_008_rr Related Requirement(s)
 * \#199173
 * @subsection MqSendForTimePeriod_TC_03_004_008_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqSendForTimePeriod_TC_03_004_008_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqSendForTimePeriod API in Abnormal behavior in single thread
 * - Test03_004_008: send message to a full queue.
 * @subsection MqSendForTimePeriod_TC_03_004_008_tpts Test Procedure/Test Steps
 * -# Reset the queue by calling R_OSAL_MqReset. 
 * -# Calling R_OSAL_MqSendForTimePeriod 3 times.
 * -# Reset the queue with R_OSAL_MqReset. 
 * @subsection MqSendForTimePeriod_TC_03_004_008_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_OK. 
 * -# The return value of the first 2 times call R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK, the 3rd call functional R_OSAL_MqSendForTimePeriod is OSAL_RETURN_TIME due to the queue has been fulled. 
 * -# The return value of the second R_OSAL_MqReset is OSAL_RETURN_OK.
 * @subsection MqSendForTimePeriod_TC_03_004_008_testcase Test Case Example
 * @snippet test_mq.cpp MqSendForTimePeriod_TC_03_004_008
 */
// [MqSendForTimePeriod_TC_03_004_008]
TEST_F(MQApiTest, MqSendForTimePeriod_TC_03_004_008)
{
    osal_milli_sec_t time_period_10ms = 10;

    // Test03_004_008
    // @brief: abnormal case: Send to a full queue
    EXPECT_EQ(R_OSAL_MqReset(shared_queue), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK); // 1st message
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK); // 2nd message -> full
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_TIME) << "Test03_004_008 failed";
    EXPECT_EQ(R_OSAL_MqReset(shared_queue), OSAL_RETURN_OK);
}
// [MqSendForTimePeriod_TC_03_004_008]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendForTimePeriod_TC_03_004_009 MQApiTest.MqSendForTimePeriod_TC_03_004_009
 * @subsection MqSendForTimePeriod_TC_03_004_009_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendForTimePeriod_TC_03_004_009_tci Test Case ID
 * MqSendForTimePeriod_S3_N_TC_03_004_009
 * @subsection MqSendForTimePeriod_TC_03_004_009_rr Related Requirement(s)
 * \#199167, \#199173, \#199181
 * @subsection MqSendForTimePeriod_TC_03_004_009_pp Prerequisite/Precondition
 * - OSAL is initialized and the shared_queue has been created by R_OSAL_MqCreate.
 * - The synchronization queue was created.
 * @subsection MqSendForTimePeriod_TC_03_004_009_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqSendForTimePeriod API in Normal behavior in multi thread
 * - Test03_004_009: using multi thread with one queue.
 * - Post-process: reset the shared queue
 * @subsection MqSendForTimePeriod_TC_03_004_009_tpts Test Procedure/Test Steps
 * -# Send the first message to the queue by calling R_OSAL_MqSendForTimePeriod.
 * -# Get current time stamp by calling R_OSAL_ClockTimeGetTimeStamp.
 * -# Create 1st thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *          -# task_name is "task03_004_009_02" 
 *          -# func is osal_test_thread_send_timestamp function with functionality:
 *              - Get the synchronization queue config by using R_OSAL_MqGetConfig
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *              - Send message to the queue by R_OSAL_MqSendUntilTimeStamp.
 *          -# userarg is send_timestamp_arg with:
 *              - mq_handle is shared_queue
 *              - timestamp is the time stamp got from above push with 1s and 2000000ns,
 *              - p_buffer is send_msg (0xFFFF)
 *              - buffer_size is sizeof(send_msg)
 *              - sync_mq is handle of synchronization queue
 *              - sync_send_msg is the sending message for synchronization purpose
 *              - needToSync is true
 *      - thread_id is TEST_THREAD_ID1
 *      - Address of thread handle.
 * -# Receive message from synchronization queue by using R_OSAL_MqReceiveForTimePeriod
 * -# Create 2bd thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *          -# task_name is "task03_004_009_01" 
 *          -# func is osal_test_thread_send_timeperiod function with functionality:
 *              - Send message to the queue by R_OSAL_MqSendForTimePeriod.
 *          -# userarg is send_timeperiod_arg with:
 *              - mq_handle is shared_queue
 *              - time is 2000ms
 *              - p_buffer is send_msg (0xFFFF)
 *              - buffer_size is sizeof(send_msg)
 *              - needToSync is false
 *      - thread_id is TEST_THREAD_ID2
 *      - Address of thread handle.
 * -# Calling R_OSAL_ThreadJoin to joining the 1st thread.
 * -# Calling function R_OSAL_MqReceiveForTimePeriod to receive 1 message from the queue.
 * -# Calling R_OSAL_ThreadJoin to joining the 2nd thread.
 * -# Calling R_OSAL_MqReset to reset the message queue.
 * @subsection MqSendForTimePeriod_TC_03_004_009_ereb Expected Result/Behavior
 * -# The return value the 1st R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of the 1st R_OSAL_ThreadCreate is OSAL_RETURN_OK.<br>
 * In function osal_test_thread_send_timestamp:<br>
 *      - R_OSAL_MqGetConfig return OSAL_RETURN_OK
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 *      - R_OSAL_MqSendUntilTimeStamp return OSAL_RETURN_OK
 * -# The return value of the R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of the 2nd R_OSAL_ThreadCreate is OSAL_RETURN_OK.<br>
 * In function osal_test_thread_send_timeperiod:<br>
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * -# The return value of the 1st R_OSAL_ThreadJoin is OSAL_RETURN_OK.<br>
 *  "return_value1" is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of the 2nd R_OSAL_ThreadJoin is OSAL_RETURN_OK.<br>
 *  "return_value" is OSAL_RETURN_OK.
 * -# The return value of function R_OSAL_MqReset is OSAL_RETURN_OK.
 * @subsection MqSendForTimePeriod_TC_03_004_009_testcase Test Case Example
 * @snippet test_mq.cpp MqSendForTimePeriod_TC_03_004_009
 * @snippet test_mq.cpp osal_test_thread_send_timestamp
 * @snippet test_mq.cpp osal_test_thread_send_timeperiod
 */
// [MqSendForTimePeriod_TC_03_004_009]
TEST_F(MQApiTest, MqSendForTimePeriod_TC_03_004_009)
{
    st_osal_time_t  current_time;
    osal_thread_handle_t thread_handle2;
    osal_thread_handle_t thread_handle1;
    st_osal_thread_config_t config2;
    st_osal_thread_config_t config1;
    int64_t return_value = 0;
    int64_t return_value1 = 0;
    st_arg_mq_send_timestamp_t send_timestamp_arg;
    st_arg_mq_send_timeperiod_t send_timeperiod_arg;
    osal_milli_sec_t time_period_10ms = 10;
    osal_milli_sec_t time_period_1000ms = 1000;

    // Test03_004_009
    // @brief: normal case: multi-thread with one queue
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK); // send the 1st message to the queue (queue have 1/2 mes)
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    current_time.tv_sec = current_time.tv_sec + 5; // timeout is 5 seconds

    send_timestamp_arg.mq_handle = shared_queue;
    send_timestamp_arg.time.tv_sec = current_time.tv_sec;
    send_timestamp_arg.time.tv_nsec = current_time.tv_nsec;
    send_timestamp_arg.p_buffer = &send_msg;
    send_timestamp_arg.buffer_size = sizeof(send_msg);
    send_timestamp_arg.sync_mq = sync_mq;
    send_timestamp_arg.sync_send_msg = sync_send_msg;
    send_timestamp_arg.needToSync = true;
    config1.func       = osal_test_thread_send_timestamp;
    config1.userarg    = (void *)&send_timestamp_arg;
    config1.priority   = OSAL_THREAD_PRIORITY_TYPE12;
    config1.task_name  = "task03_004_009_02";
    config1.stack_size = THREAD_STACK_SIZE;

    send_timeperiod_arg.mq_handle = shared_queue;
    send_timeperiod_arg.time = 2000;
    send_timeperiod_arg.p_buffer =  &send_msg;
    send_timeperiod_arg.buffer_size = sizeof(send_msg);
    send_timeperiod_arg.needToSync = false;

    config2.func       = osal_test_thread_send_timeperiod;
    config2.userarg    = (void *)&send_timeperiod_arg;
    config2.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config2.task_name  = "task03_004_009_01";
    config2.stack_size = THREAD_STACK_SIZE;

    EXPECT_EQ(R_OSAL_ThreadCreate(&config1, TEST_THREAD_ID1, &thread_handle1),OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &sync_receive_msg, sync_mq_config.msg_size), OSAL_RETURN_OK);
    EXPECT_EQ(sync_receive_msg, sync_send_msg);
    EXPECT_EQ(R_OSAL_ThreadCreate(&config2, TEST_THREAD_ID2, &thread_handle2),OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &return_value1),OSAL_RETURN_OK);
    EXPECT_EQ(return_value1, OSAL_RETURN_OK) << "Test03_004_009 failed";
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(shared_queue, time_period_10ms,  &receive_msg, sizeof(receive_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle2, &return_value), OSAL_RETURN_OK);
    EXPECT_EQ(return_value, OSAL_RETURN_OK) << "Test03_004_009 failed";
    receive_msg = 0;

    //post-process: reset the shared queue
    EXPECT_EQ(R_OSAL_MqReset(shared_queue), OSAL_RETURN_OK);

}
// [MqSendForTimePeriod_TC_03_004_009]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendForTimePeriod_TC_03_004_010 MQApiTest.MqSendForTimePeriod_TC_03_004_010
 * @subsection MqSendForTimePeriod_TC_03_004_010_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendForTimePeriod_TC_03_004_010_tci Test Case ID
 * MqSendForTimePeriod_S2_A_TC_03_004_010
 * @subsection MqSendForTimePeriod_TC_03_004_010_rr Related Requirement(s)
 * \#199167, \#199173, \#199181
 * @subsection MqSendForTimePeriod_TC_03_004_010_pp Prerequisite/Precondition
 * - OSAL is initialized and the shared_queue has been created by R_OSAL_MqCreate.
 * - The synchronization queue was created.
 * @subsection MqSendForTimePeriod_TC_03_004_010_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqSendForTimePeriod API in Abnormal behavior in multi thread
 * - Pre-process: create 2nd queue
 * - Test03_004_010: using multi thread with multi queue (already full).
 * - Post process: reset and delete 2nd queue.
 * @subsection MqSendForTimePeriod_TC_03_004_010_tpts Test Procedure/Test Steps
 * -# Create another queue (call 2nd queue) with max_num_msg = 1 by calling R_OSAL_MqCreate.
 * -# Send 2 messages to the queue (call 1st queue) to make the queue full by calling 2 times R_OSAL_MqSendForTimePeriod.
 * -# Make the 2nd queue created above full also by calling R_OSAL_MqSendForTimePeriod with valid parameter.
 * -# Create 1st thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *          -# task_name is "task03_004_010_01" 
 *          -# func is osal_test_thread_send_timeperiod function with functionality:
 *              - Get the synchronization queue config by using R_OSAL_MqGetConfig
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *              - Send message to the queue by R_OSAL_MqSendForTimePeriod.
 *          -# userarg is send_timeperiod_arg1 with:
 *              - mq_handle is shared_queue
 *              - time is 2000ms
 *              - p_buffer is send_msg (0xFFFF)
 *              - buffer_size is sizeof(send_msg)
 *              - sync_mq is handle of synchronization queue
 *              - sync_send_msg is the sending message for synchronization purpose
 *              - needToSync is true
 *      - thread_id is TEST_THREAD_ID1
 *      - Address of thread handle.
 * -# Create 2nd thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *          -# task_name is "task03_004_010_02" 
 *          -# func is osal_test_thread_send_timeperiod function with functionality:
 *              - Get the synchronization queue config by using R_OSAL_MqGetConfig
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *              - Send message to the queue by R_OSAL_MqSendForTimePeriod.
 *          -# userarg is send_timeperiod_arg1 with:
 *              - mq_handle is 2nd queue
 *              - time is 2000ms
 *              - p_buffer is send_msg (0xFFFF)
 *              - buffer_size is sizeof(send_msg)
 *              - sync_mq is handle of synchronization queue
 *              - sync_send_msg is the sending message for synchronization purpose
 *              - needToSync is true
 *      - thread_id is TEST_THREAD_ID2
 *      - Address of thread handle.
 * -# Receive 2 messages from synchronization queue by using R_OSAL_MqReceiveForTimePeriod
 * -# Create 3nd thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *          -# task_name is "task03_004_010_03" 
 *          -# func is osal_test_thread_receive_timeperiod function with functionality:
 *              - Receive message from the queue by R_OSAL_MqReceiveForTimePeriod.
 *          -# userarg is receive_timeperiod_arg with:
 *              - mq_handle is 1st queue (shared_queue)
 *              - time is 1000ms
 *              - needToSync is false
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID3
 *      - Address of thread handle.
 * -# Calling R_OSAL_ThreadJoin to join the 3rd thread.
 * -# Calling R_OSAL_ThreadJoin to join the 1st thread.
 * -# Calling R_OSAL_ThreadJoin to join the 2nd thread.
 * -# Reset the 1st queue by R_OSAL_MqReset.
 * -# Reset the 2nd queue by R_OSAL_MqReset.
 * -# Delete the 2nd queue by R_OSAL_MqDelete.
 * @subsection MqSendForTimePeriod_TC_03_004_010_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK and the return of the 2nd queue handle is not NULL_PTR.
 * -# The return value of the first 2 times calling R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqSendForTimePeriod when sending to the 2nd queue if OSAL_RETURN_OK.
 * -# The return value of the 1st R_OSAL_ThreadCreate is OSAL_RETURN_OK.<br>
 * In function osal_test_thread_send_timeperiod:<br>
 *      - R_OSAL_MqGetConfig return OSAL_RETURN_OK
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * -# The return value of the 2nd R_OSAL_ThreadCreate is OSAL_RETURN_OK.<br>
 * In function osal_test_thread_send_timeperiod:<br>
 *      - R_OSAL_MqGetConfig return OSAL_RETURN_OK
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_TIME
 * -# The return value of the R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of the 3rd R_OSAL_ThreadCreate is OSAL_RETURN_OK.<br>
 * In function osal_test_thread_receive_timeperiod:<br>
 *      - R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK
 * -# The return value of the 3rd R_OSAL_ThreadJoin is OSAL_RETURN_OK.<br>
 * "return_value3" is OSAL_RETURN_OK.
 * -# The return value of the 1st R_OSAL_ThreadJoin is OSAL_RETURN_OK.<br>
 * "return_value1" is OSAL_RETURN_OK.
 * -# The return value of the 2nd R_OSAL_ThreadJoin is OSAL_RETURN_OK.<br>
 * "return_value2" is OSAL_RETURN_TIME.
 * -# The return value of R_OSAL_MqReset when reset 1st queue is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReset when reset 2nd queue is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqDelete when delete 2nd queue is OSAL_RETURN_OK.
 * @subsection MqSendForTimePeriod_TC_03_004_010_testcase Test Case Example
 * @snippet test_mq.cpp MqSendForTimePeriod_TC_03_004_010
 * @snippet test_mq.cpp osal_test_thread_receive_timeperiod
 * @snippet test_mq.cpp osal_test_thread_send_timeperiod
 */
// [MqSendForTimePeriod_TC_03_004_010]
TEST_F(MQApiTest, MqSendForTimePeriod_TC_03_004_010)
{
    osal_mq_handle_t mq_handle1;
    osal_thread_handle_t thread_handle3;
    osal_thread_handle_t thread_handle2;
    osal_thread_handle_t thread_handle1;
    st_osal_thread_config_t config3;
    st_osal_thread_config_t config2;
    st_osal_thread_config_t config1;
    int64_t return_value1 = 0;
    int64_t return_value2 = 0;
    int64_t return_value3 = 0;
    st_arg_mq_send_timeperiod_t send_timeperiod_arg1;
    st_arg_mq_send_timeperiod_t send_timeperiod_arg2;
    st_arg_mq_receive_timeperiod_t receive_timeperiod_arg;
    osal_milli_sec_t time_period_10ms = 10;
    osal_milli_sec_t time_period_1000ms = 1000;

    //pre-process: create 2nd queue
    mq_config.max_num_msg = 1;
    mq_config.msg_size = 4;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config, TEST_MQ_ID2, &mq_handle1), OSAL_RETURN_OK);
    EXPECT_NE(mq_handle1, NULL_PTR);

    // Test03_004_010
    // @brief: abnormal case: Multi thread with multi queue(already full)
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK); //-> make the queue full

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(mq_handle1, time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK); // make the mq_handle1 full
    send_timeperiod_arg1.mq_handle = shared_queue;
    send_timeperiod_arg1.time = 2000;
    send_timeperiod_arg1.p_buffer =  &send_msg;
    send_timeperiod_arg1.buffer_size = sizeof(send_msg);
    send_timeperiod_arg1.sync_mq = sync_mq;
    send_timeperiod_arg1.sync_send_msg = sync_send_msg;
    send_timeperiod_arg1.needToSync = true;

    config1.func       = osal_test_thread_send_timeperiod;
    config1.userarg    = (void *)&send_timeperiod_arg1;
    config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config1.task_name  = "task03_004_010_01";
    config1.stack_size = THREAD_STACK_SIZE;

    send_timeperiod_arg2.mq_handle = mq_handle1;
    send_timeperiod_arg2.time = 2000;
    send_timeperiod_arg2.p_buffer =  &send_msg;
    send_timeperiod_arg2.buffer_size = sizeof(send_msg);
    send_timeperiod_arg2.sync_mq = sync_mq;
    send_timeperiod_arg2.sync_send_msg = sync_send_msg;
    send_timeperiod_arg2.needToSync = true;

    config2.func       = osal_test_thread_send_timeperiod;
    config2.userarg    = (void *)&send_timeperiod_arg2;
    config2.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config2.task_name  = "task03_004_010_01";
    config2.stack_size = THREAD_STACK_SIZE;

    receive_timeperiod_arg.mq_handle = shared_queue;
    receive_timeperiod_arg.time = 1000;
    receive_timeperiod_arg.needToSync = false;
    config3.func       = osal_test_thread_receive_timeperiod;
    config3.userarg    = (void *)&receive_timeperiod_arg;
    config3.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config3.task_name  = "task03_004_010_03";
    config3.stack_size = THREAD_STACK_SIZE;

    EXPECT_EQ(R_OSAL_ThreadCreate(&config1, TEST_THREAD_ID1, &thread_handle1),OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThreadCreate(&config2, TEST_THREAD_ID2, &thread_handle2),OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &sync_receive_msg, sync_mq_config.msg_size), OSAL_RETURN_OK);
    EXPECT_EQ(sync_receive_msg, sync_send_msg);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &sync_receive_msg, sync_mq_config.msg_size), OSAL_RETURN_OK);
    EXPECT_EQ(sync_receive_msg, sync_send_msg);
    EXPECT_EQ(R_OSAL_ThreadCreate(&config3, TEST_THREAD_ID3, &thread_handle3),OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle3, &return_value3),OSAL_RETURN_OK);
    EXPECT_EQ(return_value3, OSAL_RETURN_OK) << "Test03_004_010 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &return_value1),OSAL_RETURN_OK);
    EXPECT_EQ(return_value1, OSAL_RETURN_OK) << "Test03_004_010 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle2, &return_value2),OSAL_RETURN_OK);
    EXPECT_EQ(return_value2, OSAL_RETURN_TIME) << "Test03_004_010 failed";
    EXPECT_EQ(R_OSAL_MqReset(shared_queue), OSAL_RETURN_OK); // reset share queue for the next test cases

    //post-process: reset and delete 2nd queue, reset the shared queue
    EXPECT_EQ(R_OSAL_MqReset(mq_handle1), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle1), OSAL_RETURN_OK);
    mq_handle1 = OSAL_MQ_HANDLE_INVALID;
}
// [MqSendForTimePeriod_TC_03_004_010]

/**
 * @page MQ_TEST MQ API Test
 * @section OsalApiStateTest_MqSendForTimePeriod_TC_03_004_011 OsalApiStateTest.MqSendForTimePeriod_TC_03_004_011
 * @subsection MqSendForTimePeriod_TC_03_004_011_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection MqSendForTimePeriod_TC_03_004_011_tci Test Case ID
 * MqSendForTimePeriod_S2_A_TC_03_004_011
 * @subsection MqSendForTimePeriod_TC_03_004_011_rr Related Requirement(s)
 * \#199173
 * @subsection MqSendForTimePeriod_TC_03_004_011_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection MqSendForTimePeriod_TC_03_004_011_tcs Test Case Summary
 * This test shall checking the state of all Message queue API in Abnormal behavior in single thread
 * - Test03_004_011: checking state of R_OSAL_MqSendForTimePeriod
 * @subsection MqSendForTimePeriod_TC_03_004_011_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_MqSendForTimePeriod with valid paramter.
 * @subsection MqSendForTimePeriod_TC_03_004_011_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_STATE.
 * @subsection MqSendForTimePeriod_TC_03_004_011_testcase Test Case Example
 * @snippet test_mq.cpp MqSendForTimePeriod_TC_03_004_011
 */
// [MqSendForTimePeriod_TC_03_004_011]
TEST_F(OsalApiStateTest, MqSendForTimePeriod_TC_03_004_011)
{
    osal_mq_handle_t mq_handle = OSAL_MQ_HANDLE_INVALID;
    osal_milli_sec_t time_period_10ms = 10;
    int send_msg;

    // Test03_004_011
    // @brief: abnormal case: state transition error of R_OSAL_MqSendForTimePeriod
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(mq_handle,time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_STATE) << "Test03_004_011 failed";

}
// [MqSendForTimePeriod_TC_03_004_011]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendForTimePeriod_TC_03_004_012 MQApiTest.MqSendForTimePeriod_TC_03_004_012
 * @subsection MqSendForTimePeriod_TC_03_004_012_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendForTimePeriod_TC_03_004_0 12_tci Test Case ID
 * MqSendForTimePeriod_S2_A_TC_03_004_012
 * @subsection MqSendForTimePeriod_TC_03_004_012_rr Related Requirement(s)
 * \#199167
 * @subsection MqSendForTimePeriod_TC_03_004_012_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqSendForTimePeriod_TC_03_004_012_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqSendForTimePeriod API in Abnormal behavior in multi thread
 * - Test03_004_012: using multi thread with multi queue (already full).
 * - Post process: reset the shared queue
 * @subsection MqSendForTimePeriod_TC_03_004_012_tpts Test Procedure/Test Steps
 * -# Send 2 messages to the queue (call 1st queue) to make the queue full by calling 2 times R_OSAL_MqSendForTimePeriod.
 * -# Create Num_OF_MQ_REPEATS threads by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *          -# task_name is "task03_004_016_03" 
 *          -# func is osal_test_thread_send_timeperiod function with functionality:
 *              - Send message to the queue by R_OSAL_MqSendForTimePeriod.
 *          -# userarg is send_timeperiod_arg1 with:
 *              - mq_handle is shared_queue
 *              - time is 2000ms
 *              - p_buffer is send_msg (0xFFFF)
 *              - buffer_size is sizeof(send_msg)
 *              - needToSync is false
 *      - thead_id is from TEST_THREAD_ID1 to TEST_THREAD_ID1 + (Num_OF_MQ_REPEATS -1).
 *      - Address of thread handle.
 * -# Join all Num_OF_MQ_REPEATS thread by using R_OSAL_ThreadJoin.
 * -# Reset the queue by using R_OSAL_MqReset.
 * @subsection MqSendForTimePeriod_TC_03_004_012_ereb Expected Result/Behavior
 * -# The return value of the first 2 times calling R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of Num_OF_MQ_REPEATS times call R_OSAL_ThreadCreate is OSAL_RETURN_OK.
 * -# The return value of Num_OF_MQ_REPEATS times call R_OSAL_ThreadJoin is OSAL_RETURN_OK, and the return result of all 100 thread is OSAL_RETURN_TIME.
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_OK.
 * @subsection MqSendForTimePeriod_TC_03_004_012_testcase Test Case Example
 * @snippet test_mq.cpp MqSendForTimePeriod_TC_03_004_012
 * @snippet test_mq.cpp osal_test_thread_send_timeperiod
 */
// [MqSendForTimePeriod_TC_03_004_012]
TEST_F(MQApiTest, MqSendForTimePeriod_TC_03_004_012)
{
    st_osal_thread_config_t config1;
    int64_t return_value1 = 0;
    st_arg_mq_send_timeperiod_t send_timeperiod_arg1;
    osal_thread_handle_t thread_handle[Num_OF_MQ_REPEATS];
    bool test_flag[Num_OF_MQ_REPEATS];
    osal_milli_sec_t time_period_10ms = 10;

    // Test03_004_012
    // @brief: abnormal case: Create Num_OF_MQ_REPEATS thread to send message to a queue already full
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK); //-> make the queue full
    memset(&test_flag, true, Num_OF_MQ_REPEATS*sizeof(bool));
    for(uint64_t i = 0; i < Num_OF_MQ_REPEATS; i++)
    {
        send_timeperiod_arg1.mq_handle = shared_queue;
        send_timeperiod_arg1.time = 2000;
        send_timeperiod_arg1.p_buffer = &send_msg;
        send_timeperiod_arg1.buffer_size = sizeof(send_msg);
        send_timeperiod_arg1.needToSync = false;
        config1.func       = osal_test_thread_send_timeperiod;
        config1.userarg    = (void *)&send_timeperiod_arg1;
        config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
        config1.task_name  = "task03_004_016_03";
        config1.stack_size = THREAD_STACK_SIZE;

        EXPECT_EQ(R_OSAL_ThreadCreate(&config1, TEST_THREAD_ID1 + i, &thread_handle[i]),OSAL_RETURN_OK);
    }
    for(uint64_t i = 0; i < Num_OF_MQ_REPEATS; i++)
    {
        EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle[i], &return_value1),OSAL_RETURN_OK);
        if(return_value1 != OSAL_RETURN_TIME)
        {
            test_flag[i] = false;
        }
    }
    for(uint64_t i = 0; i < Num_OF_MQ_REPEATS; i++)
    {
        EXPECT_EQ(test_flag[i], true) << "Test03_004_012 failed";

    }

    //post-process: reset the shared queue
    EXPECT_EQ(R_OSAL_MqReset(shared_queue), OSAL_RETURN_OK);
}
// [MqSendForTimePeriod_TC_03_004_012]

/**
 * @page MQ_TEST MQ API Test
 * @section OSALINTTESTS_MqSendForTimePeriod_TC_03_004_013 OSALINTTESTS.MqSendForTimePeriod_TC_03_004_013
 * @subsection MqSendForTimePeriod_TC_03_004_013_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection MqSendForTimePeriod_TC_03_004_013_tci Test Case ID
 * MqSendForTimePeriod_S3_N_TC_03_004_013
 * @subsection MqSendForTimePeriod_TC_03_004_013_rr Related Requirement(s)
 * \#199173
 * @subsection MqSendForTimePeriod_TC_03_004_013_pp Prerequisite/Precondition
 * -# OSAL is initialized.<br>
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection MqSendForTimePeriod_TC_03_004_013_tcs Test Case Summary
 * @subsubsection MqSendForTimePeriod_TC_03_004_013_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Create a queue.
 * - Setup required memory for interrupt generation
 * - Register callack function and enable interrupt
 * - Wait for interrupt callback function finish
 * @subsubsection MqSendForTimePeriod_TC_03_004_013_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * - Reset created queue
 * - Destroy created queue
 * @subsubsection MqSendForTimePeriod_TC_03_004_013_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality with normal condition of the Message Queue APIs which are called in the ISR.
 * - Disable and clear interrupt status
 * - Test03_004_013: call R_OSAL_MqSendForTimePeriod in ISR
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection MqSendForTimePeriod_TC_03_004_013_tpts Test Procedure/Test Steps
 * @subsubsection MqSendForTimePeriod_TC_03_004_013_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Create a queue by calling function R_OSAL_MqCreate with bellow config
 *      - max_num_msg: 1
 *      - msg_size: 4
 * -# Allocate required memory for generate interrupt from hardware and get hw address of allocated memory.
 * -# Call R_OSAL_InterruptGetNumOfIrqChannels to get current number of interrupt channel.
 * -# Call R_OSAL_InterruptRegisterIsr to regsiter an interrupt with a callback function.
 * -# Calling R_OSAL_InterruptEnableIsr to enable interrupt.
 * -# Setup hardware specific register to generate the interrupt.
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection MqSendForTimePeriod_TC_03_004_013_post_tpts Test Procedure/Test Steps of Post-process
 * -# Calling R_OSAL_InterruptUnregisterIsr to unregister the interrupt.
 * -# Deallocate required memory
 * -# Calling reset queue by using R_OSAL_MqReset.
 * -# Delete the message queue by caliing R_OSAL_MqDelete.
 * @subsubsection MqSendForTimePeriod_TC_03_004_013_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Call R_OSAL_InterruptDisableIsr to disable interrupt signal.
 * -# Clear the status register by calling function R_OSAL_IoWrite32 with offset is 10 and value is TEST_CLEAR_REGISTER_VALUE.
 * -# Calling R_OSAL_MqSendForTimePeriod to send a message to queue with time period is 0.
 * -# Calling R_OSAL_MqReceiveForTimePeriod to receive a message to queue with time period is 0.
 * -# Calling R_OSAL_MqSendForTimePeriod for Send a message that the interrupt context has ended
 * @subsection MqSendForTimePeriod_TC_03_004_013_ereb Expected Result/Behavior
 * @subsubsection MqSendForTimePeriod_TC_03_004_013_pre_ereb Expected Result/Behavior of Pre-process
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK.
 * -# The allocation successfully without any error.
 * -# The return value of R_OSAL_InterruptGetNumOfIrqChannels is OSAL_RETURN_OK.<br>
 * The return number of channel is greater than 0.
 * -# The return value of R_OSAL_InterruptRegisterIsr is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_InterruptEnableIsr is OSAL_RETURN_OK.
 * -# Setup hardware specific register to generate the interrupt success without any error.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection MqSendForTimePeriod_TC_03_004_013_post_ereb Expected Result/Behavior of Post-process
 * -# The return value of R_OSAL_InterruptUnregisterIsr is OSAL_RETURN_OK.
 * -# Deallocate required memory success without any error.
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqDelete is OSAL_RETURN_OK.
 * @subsubsection MqSendForTimePeriod_TC_03_004_013_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# The return value of R_OSAL_InterruptDisableIsr is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_IoWrite32 is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK and the returned message is as expected.
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * @subsection MqSendForTimePeriod_TC_03_004_013_testcase Test Case Example
 * @snippet test_mq.cpp MqSendForTimePeriod_TC_03_004_013
 * @snippet test_mq.cpp mq_test_int_cb1_normal_TC_03_004_013
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [MqSendForTimePeriod_TC_03_004_013]
TEST_F(OSALINTTESTS, MqSendForTimePeriod_TC_03_004_013)
{
    osal_mq_handle_t mq_handle;
    size_t numOfChannel = 0;
    st_osal_mq_config mq_config1;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE10;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    st_callback_arg_t arg;
    osal_milli_sec_t time_period = 2000;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};

/* pre-process:
 * create a queue
 * Setup required memory for interrupt generation.
 * Register callack function and enable interrupt 
 * Wait for interrupt callback function finish */
    mq_config1.max_num_msg = ISR_MQ_NUM_OF_MSG;
    mq_config1.msg_size = ISR_MQ_MSG_SIZE;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config1, ISR_MQ_ID , &mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK);
    arg.additional_arg = (void *)&mq_handle;
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, mq_test_int_cb1_normal_TC_03_004_013, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory
 * Destroy created handle */
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, mq_test_int_cb1_normal_TC_03_004_013), OSAL_RETURN_OK);
    deallocate_data_memory(hndl_mmngr, buffer_obj);
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK);
}
// [MqSendForTimePeriod_TC_03_004_013]

/**
 * @page MQ_TEST MQ API Test
 * @section OSALINTTESTS_MqSendForTimePeriod_TC_03_004_014 OSALINTTESTS.MqSendForTimePeriod_TC_03_004_014
 * @subsection MqSendForTimePeriod_TC_03_004_014_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection MqSendForTimePeriod_TC_03_004_014_tci Test Case ID
 * MqSendForTimePeriod_S2_A_TC_03_004_014
 * @subsection MqSendForTimePeriod_TC_03_004_014_rr Related Requirement(s)
 * \#199173
 * @subsection MqSendForTimePeriod_TC_03_004_014_pp Prerequisite/Precondition
 * -# OSAL is initialized.<br>
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection MqSendForTimePeriod_TC_03_004_014_tcs Test Case Summary
 * @subsubsection MqSendForTimePeriod_TC_03_004_014_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 *      - Create a queue.
 *      - Setup required memory for interrupt generation
 *      - Register callack function and enable interrupt
 *      - Wait for interrupt callback function finish
 * @subsubsection MqSendForTimePeriod_TC_03_004_014_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 *      - Unregister interrupt callback function
 *      - Deallocate allocated memory
 *      - Reset created queue
 *      - Destroy created queue
 * @subsubsection MqSendForTimePeriod_TC_03_004_014_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality with Abnormal condition of the Message Queue APIs which are called in the ISR.
 * - Disable and clear interrupt status
 * - Test03_004_014: call R_OSAL_MqSendForTimePeriod in ISR with full queue
 * @subsection MqSendForTimePeriod_TC_03_004_014_tpts Test Procedure/Test Steps
 * @subsubsection MqSendForTimePeriod_TC_03_004_014_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Create a queue by calling function R_OSAL_MqCreate with bellow config
 *      - max_num_msg: 1
 *      - msg_size: 4
 * -# Allocate required memory for generate interrupt from hardware and get hw address of allocated memory.
 * -# Call R_OSAL_InterruptGetNumOfIrqChannels to get current number of interrupt channel.
 * -# Call R_OSAL_InterruptRegisterIsr to regsiter an interrupt with a callback function.
 * -# Calling R_OSAL_InterruptEnableIsr to enable interrupt.
 * -# Setup hardware specific register to generate the interrupt.
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection MqSendForTimePeriod_TC_03_004_014_post_tpts Test Procedure/Test Steps of Post-process
 * -# Calling R_OSAL_InterruptUnregisterIsr to unregister the interrupt.
 * -# Deallocate required memory
 * -# Calling reset queue by using R_OSAL_MqReset.
 * -# Delete the message queue by caliing R_OSAL_MqDelete.
 * @subsubsection MqSendForTimePeriod_TC_03_004_014_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Call R_OSAL_InterruptDisableIsr to disable interrupt signal.
 * -# Clear the status register by calling function R_OSAL_IoWrite32 with offset is 10 and value is TEST_CLEAR_REGISTER_VALUE.
 * -# Calling R_OSAL_MqSendForTimePeriod to send a message to queue with time period is 0.
 * -# Calling R_OSAL_MqSendForTimePeriod to send a message to queue with time period is 0.
 * -# Calling R_OSAL_MqReceiveForTimePeriod to receive the message from the queue.
 * -# Calling R_OSAL_MqSendForTimePeriod for Send a message that the interrupt context has ended
 * @subsection MqSendForTimePeriod_TC_03_004_014_ereb Expected Result/Behavior
 * @subsubsection MqSendForTimePeriod_TC_03_004_014_pre_ereb Expected Result/Behavior of Pre-process
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK.
 * -# The allocation successfully without any error.
 * -# The return value of R_OSAL_InterruptGetNumOfIrqChannels is OSAL_RETURN_OK.<br>
 * The return number of channel is greater than 0.
 * -# The return value of R_OSAL_InterruptRegisterIsr is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_InterruptEnableIsr is OSAL_RETURN_OK.
 * -# Setup hardware specific register to generate the interrupt success without any error.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection MqSendForTimePeriod_TC_03_004_014_post_ereb Expected Result/Behavior of Post-process
 * -# The return value of R_OSAL_InterruptUnregisterIsr is OSAL_RETURN_OK.
 * -# Deallocate required memory success without any error.
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqDelete is OSAL_RETURN_OK.
 * @subsubsection MqSendForTimePeriod_TC_03_004_014_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# The return value of R_OSAL_InterruptDisableIsr is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_IoWrite32 is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_BUSY.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK and the returned message is as expected.
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * @subsection MqSendForTimePeriod_TC_03_004_014_testcase Test Case Example
 * @snippet test_mq.cpp MqSendForTimePeriod_TC_03_004_014
 * @snippet test_mq.cpp mq_test_int_cb1_abnormal_TC_03_004_014
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [MqSendForTimePeriod_TC_03_004_014]
TEST_F(OSALINTTESTS, MqSendForTimePeriod_TC_03_004_014)
{
    osal_mq_handle_t mq_handle;
    size_t numOfChannel = 0;
    st_osal_mq_config mq_config1;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE10;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    st_callback_arg_t arg;
    osal_milli_sec_t time_period = 2000;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};

/* pre-process:
 * create a queue
 * Setup required memory for interrupt generation.
 * Register callack function and enable interrupt 
 * Wait for interrupt callback function finish */
    mq_config1.max_num_msg = ISR_MQ_NUM_OF_MSG;
    mq_config1.msg_size = ISR_MQ_MSG_SIZE;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config1, ISR_MQ_ID , &mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK);
    arg.additional_arg = (void *)&mq_handle;
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, mq_test_int_cb1_abnormal_TC_03_004_014, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory
 * Destroy created handle */
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, mq_test_int_cb1_abnormal_TC_03_004_014), OSAL_RETURN_OK);
    deallocate_data_memory(hndl_mmngr, buffer_obj);
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK);
}
// [MqSendForTimePeriod_TC_03_004_014]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendForTimePeriod_TC_03_004_015 MQApiTest.MqSendForTimePeriod_TC_03_004_015
 * @subsection MqSendForTimePeriod_TC_03_004_015_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendForTimePeriod_TC_03_004_015_tci Test Case ID
 * MqSendForTimePeriod_S0_BA_TC_03_004_015
 * @subsection MqSendForTimePeriod_TC_03_004_015_rr Related Requirement(s)
 * \#199173
 * @subsection MqSendForTimePeriod_TC_03_004_015_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqSendForTimePeriod_TC_03_004_015_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqSendForTimePeriod API in Abnormal behavior in single thread
 * - Test03_004_015: send message from queue with buffer_size parameter is bigger than message size
 * @subsection MqSendForTimePeriod_TC_03_004_015_tpts Test Procedure/Test Steps
 * -# Get the mq config by using R_OSAL_MqGetConfig.<br>
 * Increase the "mq_config.msg_size" by 1.
 * -# Send message to the queue by calling R_OSAL_MqSendForTimePeriod with the message size is "mq_config.msg_size" as above
 * @subsection MqSendForTimePeriod_TC_03_004_015_ereb Expected Result/Behavior
 * -# R_OSAL_MqGetConfig return OSAL_RETURN_OK
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_PAR
 * @subsection MqSendForTimePeriod_TC_03_004_015_testcase Test Case Example
 * @snippet test_mq.cpp MqSendForTimePeriod_TC_03_004_015
 */
// [MqSendForTimePeriod_TC_03_004_015]
TEST_F(MQApiTest, MqSendForTimePeriod_TC_03_004_015)
{

    osal_milli_sec_t time_period_10ms = 10;

    // Test03_004_015
    // @brief: abnormal case: message size is bigger than mq config message size
    EXPECT_EQ(R_OSAL_MqGetConfig(shared_queue, &mq_config), OSAL_RETURN_OK) << "Test03_004_015 failed";
    mq_config.msg_size += 1;
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue, time_period_10ms, &send_msg, mq_config.msg_size), OSAL_RETURN_PAR) << "Test03_004_015 failed";
}
// [MqSendForTimePeriod_TC_03_004_015]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendForTimePeriod_TC_03_004_016 MQApiTest.MqSendForTimePeriod_TC_03_004_016
 * @subsection MqSendForTimePeriod_TC_03_004_016_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendForTimePeriod_TC_03_004_016_tci Test Case ID
 * MqSendForTimePeriod_S0_BA_TC_03_004_016
 * @subsection MqSendForTimePeriod_TC_03_004_016_rr Related Requirement(s)
 * \#199173
 * @subsection MqSendForTimePeriod_TC_03_004_016_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqSendForTimePeriod_TC_03_004_016_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqSendForTimePeriod API in Abnormal behavior in single thread
 * - Test03_004_016: send message from queue with buffer_size parameter is less than message size
 * @subsection MqSendForTimePeriod_TC_03_004_016_tpts Test Procedure/Test Steps
 * -# Get the mq config by using R_OSAL_MqGetConfig.<br>
 * Decrease the "mq_config.msg_size"  by 1.
 * -# Send message to the queue by calling R_OSAL_MqSendForTimePeriod with the message size is "mq_config.msg_size" as above
 * @subsection MqSendForTimePeriod_TC_03_004_016_ereb Expected Result/Behavior
 * -# R_OSAL_MqGetConfig return OSAL_RETURN_OK
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_PAR
 * @subsection MqSendForTimePeriod_TC_03_004_016_testcase Test Case Example
 * @snippet test_mq.cpp MqSendForTimePeriod_TC_03_004_016
 */
// [MqSendForTimePeriod_TC_03_004_016]
TEST_F(MQApiTest, MqSendForTimePeriod_TC_03_004_016)
{

    osal_milli_sec_t time_period_10ms = 10;

    // Test03_004_016
    // @brief: abnormal case: message size is less than mq config message size
    EXPECT_EQ(R_OSAL_MqGetConfig(shared_queue, &mq_config), OSAL_RETURN_OK) << "Test03_004_016 failed";
    mq_config.msg_size -= 1;
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue, time_period_10ms, &send_msg, mq_config.msg_size), OSAL_RETURN_PAR) << "Test03_004_016 failed";
}
// [MqSendForTimePeriod_TC_03_004_016]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendForTimePeriod_TC_03_004_017 MQApiTest.MqSendForTimePeriod_TC_03_004_017
 * @subsection MqSendForTimePeriod_TC_03_004_017_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendForTimePeriod_TC_03_004_017_tci Test Case ID
 * MqSendForTimePeriod_S2_A_TC_03_004_017
 * @subsection MqSendForTimePeriod_TC_03_004_017_rr Related Requirement(s)
 * \#199173
 * @subsection MqSendForTimePeriod_TC_03_004_017_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqSendForTimePeriod_TC_03_004_017_tcs Test Case Summary
 * This test shall measure the time execution of R_OSAL_MqSendForTimePeriod with normal condition:
 * - Pre-prcess: make the "shared_queue" full
 * - Test03_004_017: measure the time execution of R_OSAL_MqSendForTimePeriod when sending a message with timeout is 10ms
 * - Post-process: reset the "shared_queue" for other test suite.
 * @subsection MqSendForTimePeriod_TC_03_004_017_tpts Test Procedure/Test Steps
 * -# Reset the "shared_queue" by using R_OSAL_MqReset
 * -# Get the config of "shared_queue" by using R_OSAL_MqGetConfig
 * -# Send two messages to "shared_queue" to make it to be full by using R_OSAL_MqSendForTimePeriod
 * -# Use the "for" loop in which we repeat 101 times with 4 steps as below:
 *      - Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *      - Send a message to queue using R_OSAL_MqSendForTimePeriod with the timeout is 10ms.
 *      - Get the current time stamp again by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *      - Get the execution time of R_OSAL_MqSendForTimePeriod by calculating the different time using R_OSAL_ClockTimeCalculateTimeDifference.<br>
 *        Execution time is stored in an array "diff_time"
 * -# Find the median of "diff_time" array.<br>
 * Compare it with 10ms plus "delta_time" (TEST_MEDIAN_EXPECT)
 * -# Reset the "shared_queue" by using R_OSAL_MqReset
 * @subsection MqSendForTimePeriod_TC_03_004_017_ereb Expected Result/Behavior
 * -# R_OSAL_MqReset return OSAL_RETURN_OK
 * -# R_OSAL_MqGetConfig return OSAL_RETURN_OK
 * -# Both R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * -# The expected result of 4 steps in "for" loop are as below:
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_TIME.
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 *      - R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK.
 * -# Getting the median of "diff_time" array successfully.<br>
 * Median value need to be not exceed 10ms plus with "delta_time"
 * -# R_OSAL_MqReset return OSAL_RETURN_OK
 * @subsection MqSendForTimePeriod_TC_03_004_017_testcase Test Case Example
 * @snippet test_mq.cpp MqSendForTimePeriod_TC_03_004_017
 * @snippet test_common.hpp findMedian
 */
// [MqSendForTimePeriod_TC_03_004_017]
TEST_F(MQApiTest, MqSendForTimePeriod_TC_03_004_017)
{
    osal_milli_sec_t time_period_10ms = 10;
    st_osal_time_t current_time_1st, current_time_2nd;
    osal_nano_sec_t diff_time[101];
    osal_nano_sec_t delta_time = TEST_MEDIAN_EXPECT;
    osal_nano_sec_t median = 0;
    int elements = 0;

    // pre-process: make the shared_queue full
    EXPECT_EQ(R_OSAL_MqReset(shared_queue), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_MqGetConfig(shared_queue, &mq_config), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue, time_period_10ms, &send_msg, mq_config.msg_size), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue, time_period_10ms, &send_msg, mq_config.msg_size), OSAL_RETURN_OK) << "pre-process failed";

    // Test03_004_017
    // @brief: time_period = 10
    for(int i = 0; i <= 100; i++)
    {
        EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_1st), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue, time_period_10ms, &send_msg, mq_config.msg_size), OSAL_RETURN_TIME) << "Test03_004_017 failed";
        EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_2nd), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &diff_time[i]), OSAL_RETURN_OK);
    }
    elements = sizeof(diff_time) / sizeof(diff_time[0]);
    median = (osal_nano_sec_t)findMedian(diff_time, elements);
    EXPECT_LE(median, time_period_10ms * MSEC_TO_NSEC + delta_time) << "Test03_004_017 failed";

    // post-process: reset the shared-queue
    EXPECT_EQ(R_OSAL_MqReset(shared_queue), OSAL_RETURN_OK) << "post-process failed";
}
// [MqSendForTimePeriod_TC_03_004_017]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendForTimePeriod_TC_03_004_018 MQApiTest.MqSendForTimePeriod_TC_03_004_018
 * @subsection MqSendForTimePeriod_TC_03_004_018_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendForTimePeriod_TC_03_004_018_tci Test Case ID
 * MqSendForTimePeriod_S2_A_TC_03_004_018
 * @subsection MqSendForTimePeriod_TC_03_004_018_rr Related Requirement(s)
 * \#199173
 * @subsection MqSendForTimePeriod_TC_03_004_018_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqSendForTimePeriod_TC_03_004_018_tcs Test Case Summary
 * This test shall measure the time execution of R_OSAL_MqSendForTimePeriod with normal condition:
 * - Pre-prcess: make the "shared_queue" full
 * - Test03_004_018: measure the time execution of R_OSAL_MqSendForTimePeriod when sending a message with timeout is 0ms
 * - Post-process: reset the "shared_queue" for other test suite.
 * @subsection MqSendForTimePeriod_TC_03_004_018_tpts Test Procedure/Test Steps
 * -# Reset the "shared_queue" by using R_OSAL_MqReset
 * -# Get the config of "shared_queue" by using R_OSAL_MqGetConfig
 * -# Send two messages to "shared_queue" to make it to be full by using R_OSAL_MqSendForTimePeriod
 * -# Use the "for" loop in which we repeat 101 times with 4 steps as below:
 *      - Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *      - Send a message to queue using R_OSAL_MqSendForTimePeriod with the timeout is 0ms.
 *      - Get the current time stamp again by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *      - Get the execution time of R_OSAL_MqSendForTimePeriod by calculating the different time using R_OSAL_ClockTimeCalculateTimeDifference.<br>
 *        Execution time is stored in an array "diff_time"
 * -# Find the median of "diff_time" array.<br>
 * Compare it with "delta_time" (TEST_MEDIAN_EXPECT)
 * -# Use the "for" loop in which we repeat 101 times with 5 steps as below:
 *      - Reset the "shared_queue" by using R_OSAL_MqReset
 *      - Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *      - Send a message to queue using R_OSAL_MqSendForTimePeriod with the timeout is 0ms.
 *      - Get the current time stamp again by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *      - Get the execution time of R_OSAL_MqSendForTimePeriod by calculating the different time using R_OSAL_ClockTimeCalculateTimeDifference.<br>
 *        Execution time is stored in an array "diff_time"
 * -# Find the median of "diff_time" array.<br>
 * Compare it with "delta_time" (typically 100usec)
 * -# Reset the "shared_queue" by using R_OSAL_MqReset
 * @subsection MqSendForTimePeriod_TC_03_004_018_ereb Expected Result/Behavior
 * -# R_OSAL_MqReset return OSAL_RETURN_OK
 * -# R_OSAL_MqGetConfig return OSAL_RETURN_OK
 * -# Both R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * -# The expected result of 4 steps in "for" loop are as below:
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_TIME.
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 *      - R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK.
 * -# Getting the median of "diff_time" array successfully.<br>
 * Median value need to be not exceed "delta_time"
 * -# The expected result of 5 steps in "for" loop are as below:
 *      - R_OSAL_MqReset return OSAL_RETURN_OK
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK.
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 *      - R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK.
 * -# Getting the median of "diff_time" array successfully.<br>
 * Median value need to be not exceed "delta_time"
 * -# R_OSAL_MqReset return OSAL_RETURN_OK
 * @subsection MqSendForTimePeriod_TC_03_004_018_testcase Test Case Example
 * @snippet test_mq.cpp MqSendForTimePeriod_TC_03_004_018
 * @snippet test_common.hpp findMedian
 */
// [MqSendForTimePeriod_TC_03_004_018]
TEST_F(MQApiTest, MqSendForTimePeriod_TC_03_004_018)
{
    osal_milli_sec_t time_period_10ms = 10;
    osal_milli_sec_t time_period_0ms = 0;
    st_osal_time_t current_time_1st, current_time_2nd;
    osal_nano_sec_t diff_time[101];
    osal_nano_sec_t delta_time = TEST_MEDIAN_EXPECT;
    osal_nano_sec_t median = 0;
    int elements = 0;

    // pre-process: make the shared_queue full
    EXPECT_EQ(R_OSAL_MqReset(shared_queue), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_MqGetConfig(shared_queue, &mq_config), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue, time_period_10ms, &send_msg, mq_config.msg_size), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue, time_period_10ms, &send_msg, mq_config.msg_size), OSAL_RETURN_OK) << "pre-process failed";

    // Test03_004_018
    // @brief: time_period = 0
    for(int i = 0; i <= 100; i++)
    {
        EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_1st), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue, time_period_0ms, &send_msg, mq_config.msg_size), OSAL_RETURN_TIME) << "Test03_004_018 failed";
        EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_2nd), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &diff_time[i]), OSAL_RETURN_OK);
    }

    elements = sizeof(diff_time) / sizeof(diff_time[0]);
    median = (osal_nano_sec_t)findMedian(diff_time, elements);
    EXPECT_LE(median, delta_time) << "Test03_004_018 failed";

    for(int i = 0; i <= 100; i++)
    {
        EXPECT_EQ(R_OSAL_MqReset(shared_queue), OSAL_RETURN_OK) << "Test03_004_018 failed";
        EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_1st), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue, time_period_0ms, &send_msg, mq_config.msg_size), OSAL_RETURN_OK) << "Test03_004_018 failed";
        EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_2nd), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &diff_time[i]), OSAL_RETURN_OK);
    }

    delta_time = 100000; //100usec
    elements = sizeof(diff_time) / sizeof(diff_time[0]);
    median = (osal_nano_sec_t)findMedian(diff_time, elements);
    EXPECT_LE(median, delta_time) << "Test03_004_018 failed";

    // post-process: reset the shared-queue
    EXPECT_EQ(R_OSAL_MqReset(shared_queue), OSAL_RETURN_OK) << "post-process failed";
}
// [MqSendForTimePeriod_TC_03_004_018]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqSendForTimePeriod_TC_03_004_019 MQApiTest.MqSendForTimePeriod_TC_03_004_019
 * @subsection MqSendForTimePeriod_TC_03_004_019_tsi Test Suite ID
 * MQApiTest
 * @subsection MqSendForTimePeriod_TC_03_004_019_tci Test Case ID
 * MqSendForTimePeriod_S3_N_TC_03_004_019
 * @subsection MqSendForTimePeriod_TC_03_004_019_rr Related Requirement(s)
 * \#199173
 * @subsection MqSendForTimePeriod_TC_03_004_019_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqSendForTimePeriod_TC_03_004_019_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqSendForTimePeriod API in Normal behavior in single thread
 * - Test03_004_019: confirm that the contents of MQ are not rewritten by R_OSAL_MqSendForTimePeriod timeout
 * @subsection MqSendForTimePeriod_TC_03_004_019_tpts Test Procedure/Test Steps
 * -# Get the configuration of "shared_queue" by using R_OSAL_MqGetConfig
 * -# Make sure "shared_queue" is empty by using R_OSAL_MqReset
 * -# Send two message to "shared_queue" by using R_OSAL_MqSendForTimePeriod
 * -# Continue send 3rd message to "shared_queue" by using R_OSAL_MqSendForTimePeriod
 * -# Receive two message from "shared_queue" by using R_OSAL_MqReceiveForTimePeriod
 * @subsection MqSendForTimePeriod_TC_03_004_019_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqGetConfig is OSAL_RETURN_OK
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_OK
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_TIME
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK.<br>
 * Received message is the same as sent message.
 * @subsection MqSendForTimePeriod_TC_03_004_019_testcase Test Case Example
 * @snippet test_mq.cpp MqSendForTimePeriod_TC_03_004_019
 */
// [MqSendForTimePeriod_TC_03_004_019]
TEST_F(MQApiTest, MqSendForTimePeriod_TC_03_004_019)
{
    osal_milli_sec_t time_period_10ms = 10;
    int send_msg_data[3] = {0x1234, 0x5678, 0x3456};

    // Test03_004_019
    // @brief: normal case: confirm that the contents of MQ are not rewritten by SendAPI
    EXPECT_EQ(R_OSAL_MqGetConfig(shared_queue, &mq_config), OSAL_RETURN_OK) << "Test03_004_019 failed";
    EXPECT_EQ(R_OSAL_MqReset(shared_queue), OSAL_RETURN_OK) << "Test03_004_019 failed";
    // send 2 message 0x1234 and 0x5678 to the queue
    for (int i = 0; i < mq_config.max_num_msg; i++)
    {
        EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue, time_period_10ms, &send_msg_data[i], mq_config.msg_size), OSAL_RETURN_OK) << "Test03_004_019 failed";
    }

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue, time_period_10ms, &send_msg_data[2], mq_config.msg_size), OSAL_RETURN_TIME) << "Test03_004_019 failed";
    //receive 2 messages from the queue
    for (int i = 0; i < mq_config.max_num_msg; i++)
    {
        EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(shared_queue, time_period_10ms, &receive_msg, mq_config.msg_size), OSAL_RETURN_OK) << "Test03_004_019 failed";
        EXPECT_EQ(receive_msg, send_msg_data[i]) << "Test03_004_019 failed";
    }
}
// [MqSendForTimePeriod_TC_03_004_019]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_001 MQApiTest.MqReceiveUntilTimeStamp
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_001_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_001_tci Test Case ID
 * MqReceiveUntilTimeStamp_S1_N_TC_03_005_001
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_001_rr Related Requirement(s)
 * \#199174
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_001_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_001_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqReceiveUntilTimeStamp API in Normal behavior in single thread
 * - Test03_005_001: receive message from queue within 1s and 2000000ns.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_001_tpts Test Procedure/Test Steps
 * -# Send a message to the queue by R_OSAL_MqSendForTimePeriod.
 * -# Get current time stamp by R_OSAL_ClockTimeGetTimeStamp.
 * -# Receive message by R_OSAL_MqReceiveUntilTimeStamp with time stamp got from above plus with 1s and 2000000ns.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_001_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveUntilTimeStamp is OSAL_RETURN_OK and the received message is the same as the sent message.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_001_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveUntilTimeStamp_TC_03_005_001
 */
// [MqReceiveUntilTimeStamp_TC_03_005_001]
TEST_F(MQApiTest, MqReceiveUntilTimeStamp_TC_03_005_001)
{
    st_osal_time_t  current_time;
    osal_milli_sec_t time_period_10ms = 10;
    size_t buffer_size = 4;

    // Test03_005_001
    // @brief: normal case: current_time_stamp.tv_nsec + 2000000
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    if(current_time.tv_nsec + 2 * MSEC_TO_NSEC <= MAX_NSEC)
    {
        current_time.tv_nsec = current_time.tv_nsec + 2 * MSEC_TO_NSEC;
        current_time.tv_sec = current_time.tv_sec + 1;
    }
    else
    {
        current_time.tv_nsec = 2 * MSEC_TO_NSEC - (MAX_NSEC - current_time.tv_nsec);
        current_time.tv_sec = current_time.tv_sec + 2;
    }
    EXPECT_EQ(R_OSAL_MqReceiveUntilTimeStamp(shared_queue,&current_time, &receive_msg, buffer_size), OSAL_RETURN_OK) << "Test03_005_001 failed";
    EXPECT_EQ(receive_msg, send_msg);
    receive_msg = 0;
}
// [MqReceiveUntilTimeStamp_TC_03_005_001]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_002 MQApiTest.MqReceiveUntilTimeStamp
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_002_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_002_tci Test Case ID
 * MqReceiveUntilTimeStamp_S1_BN_TC_03_005_002
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_002_rr Related Requirement(s)
 * \#199174
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_002_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_002_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqReceiveUntilTimeStamp API in Normal behavior in single thread
 * - Test03_005_002: receive message from queue within 1s and 1000000ns.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_002_tpts Test Procedure/Test Steps
 * -# Send a message to the queue by R_OSAL_MqSendForTimePeriod.
 * -# Get current time stamp by R_OSAL_ClockTimeGetTimeStamp.
 * -# Receive message by R_OSAL_MqReceiveUntilTimeStamp with time stamp got from above plus with 1s and 1000000ns. 
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_002_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK. 
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK. 
 * -# The return value of R_OSAL_MqReceiveUntilTimeStamp is OSAL_RETURN_OK and the received message is the same as the sent message.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_002_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveUntilTimeStamp_TC_03_005_002
 */
// [MqReceiveUntilTimeStamp_TC_03_005_002]
TEST_F(MQApiTest, MqReceiveUntilTimeStamp_TC_03_005_002)
{
    st_osal_time_t  current_time;
    osal_milli_sec_t time_period_10ms = 10;
    size_t buffer_size = 4;

    // Test03_005_002
    // @brief: normal case: current_time_stamp.tv_nsec + 1000000
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    if(current_time.tv_nsec + 2 * MSEC_TO_NSEC <= MAX_NSEC)
    {
        current_time.tv_nsec = current_time.tv_nsec + 2 * MSEC_TO_NSEC;
        current_time.tv_sec = current_time.tv_sec + 1;
    }
    else
    {
        current_time.tv_nsec = 2 * MSEC_TO_NSEC - (MAX_NSEC - current_time.tv_nsec);
        current_time.tv_sec = current_time.tv_sec + 2;
    }
    EXPECT_EQ(R_OSAL_MqReceiveUntilTimeStamp(shared_queue,&current_time, &receive_msg, buffer_size), OSAL_RETURN_OK) << "Test03_005_002 failed";
    EXPECT_EQ(receive_msg, send_msg);
    receive_msg = 0;
}
// [MqReceiveUntilTimeStamp_TC_03_005_002]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_003 MQApiTest.MqReceiveUntilTimeStamp
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_003_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_003_tci Test Case ID
 * MqReceiveUntilTimeStamp_S1_BN_TC_03_005_003
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_003_rr Related Requirement(s)
 * \#199174
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_003_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_003_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqReceiveUntilTimeStamp API in Normal behavior in single thread
 * - Test03_005_003: receive message from queue with time stamp is plus with 1s and the nano second is 999999999.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_003_tpts Test Procedure/Test Steps
 * -# Send a message to the queue by R_OSAL_MqSendForTimePeriod.
 * -# Get current time stamp by R_OSAL_ClockTimeGetTimeStamp.
 * -# Receive message by R_OSAL_MqReceiveUntilTimeStamp with time stamp got from above plus with 1s and the nano second is 999999999.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_003_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveUntilTimeStamp is OSAL_RETURN_OK and the received message is the same as the sent message.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_003_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveUntilTimeStamp_TC_03_005_003
 */
// [MqReceiveUntilTimeStamp_TC_03_005_003]
TEST_F(MQApiTest, MqReceiveUntilTimeStamp_TC_03_005_003)
{
    st_osal_time_t  current_time;
    osal_milli_sec_t time_period_10ms = 10;
    size_t buffer_size = 4;

    // Test03_005_003
    // @brief: normal case: current_time_stamp.tv_nsec = 999999999
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    current_time.tv_sec += 1;
    current_time.tv_nsec = 999999999;
    EXPECT_EQ(R_OSAL_MqReceiveUntilTimeStamp(shared_queue,&current_time, &receive_msg, buffer_size), OSAL_RETURN_OK) << "Test03_005_003 failed";
    EXPECT_EQ(receive_msg, send_msg);
    receive_msg = 0;
}
// [MqReceiveUntilTimeStamp_TC_03_005_003]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_004 MQApiTest.MqReceiveUntilTimeStamp
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_004_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_004_tci Test Case ID
 * MqReceiveUntilTimeStamp_S1_N_TC_03_005_004
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_004_rr Related Requirement(s)
 * \#199174
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_004_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_004_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqReceiveUntilTimeStamp API in Normal behavior in single thread
 * - Test03_005_004: receive message from queue with time stamp is plus with 1s and the nano second is 111111.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_004_tpts Test Procedure/Test Steps
 * -# Send a message to the queue by R_OSAL_MqSendForTimePeriod.
 * -# Get current time stamp by R_OSAL_ClockTimeGetTimeStamp.
 * -# Receive message by R_OSAL_MqReceiveUntilTimeStamp with time stamp got from above plus with 1s and the nano second is 111111.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_004_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveUntilTimeStamp is OSAL_RETURN_OK and the received message is the same as the sent message.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_004_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveUntilTimeStamp_TC_03_005_004
 */
// [MqReceiveUntilTimeStamp_TC_03_005_004]
TEST_F(MQApiTest, MqReceiveUntilTimeStamp_TC_03_005_004)
{
    st_osal_time_t  current_time;
    osal_milli_sec_t time_period_10ms = 10;
    size_t buffer_size = 4;

    // Test03_005_004
    // @brief: normal case: current_time_stamp.tv_nsec = 111111
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    current_time.tv_sec += 1;
    current_time.tv_nsec = 111111;
    EXPECT_EQ(R_OSAL_MqReceiveUntilTimeStamp(shared_queue,&current_time, &receive_msg, buffer_size), OSAL_RETURN_OK) << "Test03_005_004 failed";
    EXPECT_EQ(receive_msg, send_msg);
    receive_msg = 0;
}
// [MqReceiveUntilTimeStamp_TC_03_005_004]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_005 MQApiTest.MqReceiveUntilTimeStamp
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_005_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_005_tci Test Case ID
 * MqReceiveUntilTimeStamp_S0_A_TC_03_005_005
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_005_rr Related Requirement(s)
 * \#199174
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_005_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_005_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqReceiveUntilTimeStamp API in Normal behavior in single thread
 * - Test03_005_005: receive message from queue with the past time stamp.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_005_tpts Test Procedure/Test Steps
 * -# Send a message to the queue by R_OSAL_MqSendForTimePeriod.
 * -# Get current time stamp by R_OSAL_ClockTimeGetTimeStamp.
 * -# Receive message by R_OSAL_MqReceiveUntilTimeStamp with time stamp got from above minus with 1s
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_005_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveUntilTimeStamp is OSAL_RETURN_OK and the received message is the same as the sent message.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_005_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveUntilTimeStamp_TC_03_005_005
 */
// [MqReceiveUntilTimeStamp_TC_03_005_005]
TEST_F(MQApiTest, MqReceiveUntilTimeStamp_TC_03_005_005)
{
    st_osal_time_t  current_time;
    osal_milli_sec_t time_period_10ms = 10;
    osal_milli_sec_t time_period_2000ms = 2000;
    size_t buffer_size = 4;

    // Test03_005_005
    // @brief: normal case: current_time_stamp.tv_sec -1
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    if (current_time.tv_sec < 1)
    {
        EXPECT_EQ(R_OSAL_ThreadSleepForTimePeriod(time_period_2000ms), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    }
    current_time.tv_sec = current_time.tv_sec - 1;
    EXPECT_EQ(R_OSAL_MqReceiveUntilTimeStamp(shared_queue,&current_time, &receive_msg, buffer_size), OSAL_RETURN_OK) << "Test03_005_005 failed";
    receive_msg = 0;
}
// [MqReceiveUntilTimeStamp_TC_03_005_005]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_006 MQApiTest.MqReceiveUntilTimeStamp_TC_03_005_006
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_006_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_006_tci Test Case ID
 * MqReceiveUntilTimeStamp_S2_A_TC_03_005_006
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_006_rr Related Requirement(s)
 * \#199174
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_006_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_006_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqReceiveUntilTimeStamp API in Abnormal behavior in single thread
 * - Test03_005_006: receive message from deleted queue.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_006_tpts Test Procedure/Test Steps
 * -# Create a new queue by calling R_OSAL_MqCreate function.
 * -# Delete created queue by calling R_OSAL_MqDelete.
 * -# Get current time stamp by R_OSAL_ClockTimeGetTimeStamp.
 * -# Receive message by R_OSAL_MqReceiveUntilTimeStamp with valid time stamp.  
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_006_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK and the return handle is not NULL_PTR.
 * -# The return value of R_OSAL_MqDelete is OSAL_RETURN_OK. <br>
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveUntilTimeStamp is OSAL_RETURN_HANDLE.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_006_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveUntilTimeStamp_TC_03_005_006
 */
// [MqReceiveUntilTimeStamp_TC_03_005_006]
TEST_F(MQApiTest, MqReceiveUntilTimeStamp_TC_03_005_006)
{
    st_osal_time_t  current_time;
    size_t buffer_size = 4;

    // Test03_005_006
    // @brief: abnormal case: handle has already been deleted
    mq_config.max_num_msg = 1;
    mq_config.msg_size = 4;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config, TEST_MQ_ID2, &mq_handle), OSAL_RETURN_OK);
    EXPECT_NE(mq_handle, NULL_PTR);
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    if(current_time.tv_nsec + 2 * MSEC_TO_NSEC <= MAX_NSEC)
    {
        current_time.tv_nsec = current_time.tv_nsec + 2 * MSEC_TO_NSEC;
        current_time.tv_sec = current_time.tv_sec + 1;
    }
    else
    {
        current_time.tv_nsec = 2 * MSEC_TO_NSEC - (MAX_NSEC - current_time.tv_nsec);
        current_time.tv_sec = current_time.tv_sec + 2;
    }
    EXPECT_EQ(R_OSAL_MqReceiveUntilTimeStamp(mq_handle,&current_time, &receive_msg, buffer_size), OSAL_RETURN_HANDLE) << "Test03_005_006 failed";
}
// [MqReceiveUntilTimeStamp_TC_03_005_006]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_007 MQApiTest.MqReceiveUntilTimeStamp_TC_03_005_007
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_007_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_007_tci Test Case ID
 * MqReceiveUntilTimeStamp_S0_A_TC_03_005_007
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_007_rr Related Requirement(s)
 * \#199151, \#199165, \#199174
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_007_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_007_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqReceiveUntilTimeStamp API in Abnormal behavior in single thread
 * - Test03_005_007: receive message with handle is NULL.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_007_tpts Test Procedure/Test Steps
 * -# Get current time stamp by R_OSAL_ClockTimeGetTimeStamp.
 * -# Receive message by R_OSAL_MqReceiveUntilTimeStamp with valid time stamp but NULL handle.  
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_007_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveUntilTimeStamp is OSAL_RETURN_HANDLE.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_007_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveUntilTimeStamp_TC_03_005_007
 */
// [MqReceiveUntilTimeStamp_TC_03_005_007]
TEST_F(MQApiTest, MqReceiveUntilTimeStamp_TC_03_005_007)
{
    st_osal_time_t  current_time;
    size_t buffer_size = 4;

    // Test03_005_007
    // @brief: abnormal case: handle NULL
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    if(current_time.tv_nsec + 2 * MSEC_TO_NSEC <= MAX_NSEC)
    {
        current_time.tv_nsec = current_time.tv_nsec + 2 * MSEC_TO_NSEC;
        current_time.tv_sec = current_time.tv_sec + 1;
    }
    else
    {
        current_time.tv_nsec = 2 * MSEC_TO_NSEC - (MAX_NSEC - current_time.tv_nsec);
        current_time.tv_sec = current_time.tv_sec + 2;
    }
    EXPECT_EQ(R_OSAL_MqReceiveUntilTimeStamp(OSAL_MQ_HANDLE_INVALID,&current_time, &receive_msg, buffer_size), OSAL_RETURN_HANDLE) << "Test03_005_007 failed";
}
// [MqReceiveUntilTimeStamp_TC_03_005_007]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_008 MQApiTest.MqReceiveUntilTimeStamp_TC_03_005_008
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_008_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_008_tci Test Case ID
 * MqReceiveUntilTimeStamp_S0_A_TC_03_005_008
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_008_rr Related Requirement(s)
 * \#199174
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_008_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_008_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqReceiveUntilTimeStamp API in Abnormal behavior in single thread
 * - Test03_005_008: receive message with input time_stamp parameter is NULL.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_008_tpts Test Procedure/Test Steps
 * -# Receive message by R_OSAL_MqReceiveUntilTimeStamp with NULL time stamp.   
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_008_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqReceiveUntilTimeStamp is OSAL_RETURN_PAR.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_008_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveUntilTimeStamp_TC_03_005_008
 */
// [MqReceiveUntilTimeStamp_TC_03_005_008]
TEST_F(MQApiTest, MqReceiveUntilTimeStamp_TC_03_005_008)
{
    st_osal_time_t *p_time_stamp = NULL;
    size_t buffer_size = 4;

    // Test03_005_008
    // @brief: abnormal case: time_stamp is NULL
    EXPECT_EQ(R_OSAL_MqReceiveUntilTimeStamp(shared_queue, p_time_stamp, &receive_msg, buffer_size), OSAL_RETURN_PAR) << "Test03_005_008 failed";
}
// [MqReceiveUntilTimeStamp_TC_03_005_008]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_009 MQApiTest.MqReceiveUntilTimeStamp_TC_03_005_009
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_009_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_009_tci Test Case ID
 * MqReceiveUntilTimeStamp_S0_A_TC_03_005_009
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_009_rr Related Requirement(s)
 * \#199174
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_009_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_009_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqReceiveUntilTimeStamp API in Abnormal behavior in single thread
 * - Test03_005_009: receive message with input p_buffer parameter is NULL.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_009_tpts Test Procedure/Test Steps
 * -# Get current time stamp by R_OSAL_ClockTimeGetTimeStamp.
 * -# Receive message by R_OSAL_MqReceiveUntilTimeStamp with valid time stamp but NULL p_buffer.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_009_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveUntilTimeStamp is OSAL_RETURN_PAR.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_009_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveUntilTimeStamp_TC_03_005_009
 */
// [MqReceiveUntilTimeStamp_TC_03_005_009]
TEST_F(MQApiTest, MqReceiveUntilTimeStamp_TC_03_005_009)
{
    st_osal_time_t  current_time;
    size_t buffer_size = 4;
    void * p_buffer;

    // Test03_005_009
    // @brief: abnormal case: p_buffer is NULL
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    if(current_time.tv_nsec + 2 * MSEC_TO_NSEC <= MAX_NSEC)
    {
        current_time.tv_nsec = current_time.tv_nsec + 2 * MSEC_TO_NSEC;
        current_time.tv_sec = current_time.tv_sec + 1;
    }
    else
    {
        current_time.tv_nsec = 2 * MSEC_TO_NSEC - (MAX_NSEC - current_time.tv_nsec);
        current_time.tv_sec = current_time.tv_sec + 2;
    }
    p_buffer = NULL;
    EXPECT_EQ(R_OSAL_MqReceiveUntilTimeStamp(shared_queue,&current_time, p_buffer, buffer_size), OSAL_RETURN_PAR) << "Test03_005_009 failed";
}
// [MqReceiveUntilTimeStamp_TC_03_005_009]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_010 MQApiTest.MqReceiveUntilTimeStamp_TC_03_005_010
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_010_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_010_tci Test Case ID
 * MqReceiveUntilTimeStamp_S0_BA_TC_03_005_010
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_010_rr Related Requirement(s)
 * \#199174
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_010_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_010_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqReceiveUntilTimeStamp API in Abnormal behavior in single thread
 * - Test03_005_010: receive message from queue with time_stamp.tv_nsec is 1000000000.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_010_tpts Test Procedure/Test Steps
 * -# Get current time stamp by R_OSAL_ClockTimeGetTimeStamp.
 * -# Receive message by R_OSAL_MqReceiveUntilTimeStamp with time stamp got from above plus with 1s and the nano second is 1000000000.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_010_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveUntilTimeStamp is OSAL_RETURN_PAR. 
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_010_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveUntilTimeStamp_TC_03_005_010
 */
// [MqReceiveUntilTimeStamp_TC_03_005_010]
TEST_F(MQApiTest, MqReceiveUntilTimeStamp_TC_03_005_010)
{
    st_osal_time_t  current_time;
    size_t buffer_size = 4;

    // Test03_005_010
    // @brief: abnormal case: timestamp.tv_nsec is 1000000000
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    current_time.tv_sec += 1;
    current_time.tv_nsec = 1000000000;
    EXPECT_EQ(R_OSAL_MqReceiveUntilTimeStamp(shared_queue,&current_time, &receive_msg, buffer_size), OSAL_RETURN_PAR) << "Test03_005_010 failed";
}
// [MqReceiveUntilTimeStamp_TC_03_005_010]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_011 MQApiTest.MqReceiveUntilTimeStamp_TC_03_005_011
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_011_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_011_tci Test Case ID
 * MqReceiveUntilTimeStamp_S0_A_TC_03_005_011
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_011_rr Related Requirement(s)
 * \#199174
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_011_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_011_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqReceiveUntilTimeStamp API in Abnormal behavior in single thread
 * - Test03_005_011: receive message from queue with buffer_size parameter is less than message size
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_011_tpts Test Procedure/Test Steps
 * -# Get the mq config by using R_OSAL_MqGetConfig.<br>
 * Decrease "mq_config.msg_size" by 1
 * -# Send a message (0xFFFF) to the queue by R_OSAL_MqSendForTimePeriod. 
 * -# Get current time stamp by R_OSAL_ClockTimeGetTimeStamp.
 * -# Receive message by R_OSAL_MqReceiveUntilTimeStamp with "mq_config.msg_size" as above
 * -# Call R_OSAL_MqReset to reset the queue.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_011_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqGetConfig is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveUntilTimeStamp is OSAL_RETURN_PAR.
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_OK.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_011_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveUntilTimeStamp_TC_03_005_011
 */
// [MqReceiveUntilTimeStamp_TC_03_005_011]
TEST_F(MQApiTest, MqReceiveUntilTimeStamp_TC_03_005_011)
{
    st_osal_time_t  current_time;
    size_t buffer_size = 4;
    osal_milli_sec_t time_period_10ms = 10;

    // Test03_005_011
    // @brief: abnormal case: message size is less than mq config message size.
    EXPECT_EQ(R_OSAL_MqGetConfig(shared_queue, &mq_config), OSAL_RETURN_OK) << "Test03_005_011 failed";
    mq_config.msg_size -= 1;
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    if(current_time.tv_nsec + 2 * MSEC_TO_NSEC <= MAX_NSEC)
    {
        current_time.tv_nsec = current_time.tv_nsec + 2 * MSEC_TO_NSEC;
        current_time.tv_sec = current_time.tv_sec + 1;
    }
    else
    {
        current_time.tv_nsec = 2 * MSEC_TO_NSEC - (MAX_NSEC - current_time.tv_nsec);
        current_time.tv_sec = current_time.tv_sec + 2;
    }
    EXPECT_EQ(R_OSAL_MqReceiveUntilTimeStamp(shared_queue,&current_time, &receive_msg, mq_config.msg_size), OSAL_RETURN_PAR) << "Test03_005_011 failed";
    EXPECT_EQ(R_OSAL_MqReset(shared_queue), OSAL_RETURN_OK); // reset for the next test cases
}
// [MqReceiveUntilTimeStamp_TC_03_005_011]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_012 MQApiTest.MqReceiveUntilTimeStamp_TC_03_005_012
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_012_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_012_tci Test Case ID
 * MqReceiveUntilTimeStamp_S2_A_TC_03_005_012
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_012_rr Related Requirement(s)
 * \#199174
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_012_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_012_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqReceiveUntilTimeStamp API in Abnormal behavior in single thread
 * - Test03_005_012: receive a message from an empty queue.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_012_tpts Test Procedure/Test Steps
 * -# Calling function R_OSAL_MqIsEmpty. 
 * -# Get current time stamp by R_OSAL_ClockTimeGetTimeStamp.
 * -# Receive message by R_OSAL_MqReceiveUntilTimeStamp
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_012_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqIsEmpty is OSAL_RETURN_OK and the return result is the queue was empty.
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveUntilTimeStamp is OSAL_RETURN_TIME.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_012_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveUntilTimeStamp_TC_03_005_012
 */
// [MqReceiveUntilTimeStamp_TC_03_005_012]
TEST_F(MQApiTest, MqReceiveUntilTimeStamp_TC_03_005_012)
{
    bool isEmpty;
    st_osal_time_t  current_time;
    size_t buffer_size = 4;

    // Test03_005_012
    // @brief: abnormal case: R_OSAL_MqReceiveUntilTimeStamp a empty queue
    EXPECT_EQ(R_OSAL_MqIsEmpty(shared_queue, &isEmpty), OSAL_RETURN_OK);
    EXPECT_TRUE(isEmpty);
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    if(current_time.tv_nsec + 2 * MSEC_TO_NSEC <= MAX_NSEC)
    {
        current_time.tv_nsec = current_time.tv_nsec + 2 * MSEC_TO_NSEC;
        current_time.tv_sec = current_time.tv_sec + 1;
    }
    else
    {
        current_time.tv_nsec = 2 * MSEC_TO_NSEC - (MAX_NSEC - current_time.tv_nsec);
        current_time.tv_sec = current_time.tv_sec + 2;
    }
    buffer_size = 4;
    EXPECT_EQ(R_OSAL_MqReceiveUntilTimeStamp(shared_queue,&current_time, &receive_msg, buffer_size), OSAL_RETURN_TIME) << "Test03_005_012 failed";
}
// [MqReceiveUntilTimeStamp_TC_03_005_012]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_013 MQApiTest.MqReceiveUntilTimeStamp_TC_03_005_013
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_013_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_013_tci Test Case ID
 * MqReceiveUntilTimeStamp_S3_N_TC_03_005_013
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_013_rr Related Requirement(s)
 * \#199174,\#199181
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_013_pp Prerequisite/Precondition
 * - OSAL is initialized and the shared_queue has been created by R_OSAL_MqCreate.
 * - The synchronization queue was created.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_013_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqReceiveUntilTimeStamp API in Normal behavior in multi thread
 * - Test03_005_013: using multi thread with one queue.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_013_tpts Test Procedure/Test Steps
 * -# Send a message to the queue by calling function R_OSAL_MqSendForTimePeriod with valid parameter.
 * -# Get current time stamp by calling R_OSAL_ClockTimeGetTimeStamp.
 * -# Create 1st thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *          -# task_name is "task03_005_013_01" 
 *          -# func is osal_test_thread_receive_timeperiod function with functionality:
 *              - Get the synchronization queue config by using R_OSAL_MqGetConfig
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *              - Receive message from the queue by R_OSAL_MqReceiveForTimePeriod.
 *          -# userarg is receive_timeperiod_arg with:
 *              - mq_handle is shared_queue
 *              - time is 1000ms
 *              - sync_mq is handle of synchronization queue
 *              - sync_send_msg is the sending message for synchronization purpose
 *              - needToSync is true
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1
 *      - Address of thread handle.
 * -# Receive message from synchronization queue by using R_OSAL_MqReceiveForTimePeriod
 * -# Create 2nd thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *          -# task_name is "task03_005_013_02" 
 *          -# func is osal_test_thread_receive_timestamp function with functionality:
 *              - Receive message from the queue by R_OSAL_MqReceiveUntilTimeStamp.
 *          -# userarg is receive_timestamp_arg with:
 *              - mq_handle is shared_queue
 *              - time is time stamp got from above plus with 1s and 2000000ns.
 *              - needToSync is false
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2
 *      - Address of thread handle.
 * -# Calling R_OSAL_ThreadJoin to join the 1st thread.
 * -# Calling function R_OSAL_MqSendForTimePeriod to send 1 message to the queue.
 * -# Calling R_OSAL_ThreadJoin to join the 2nd thread.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_013_ereb Expected Result/Behavior
 * -# The return value of the first R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of the 1st R_OSAL_ThreadCreate is OSAL_RETURN_OK.<br>
 * In function osal_test_thread_receive_timeperiod:<br>
 *      - R_OSAL_MqGetConfig return OSAL_RETURN_OK
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 *      - R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK
 * -# The return value of the R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of the 2nd R_OSAL_ThreadCreate is OSAL_RETURN_OK.<br>
 * In function osal_test_thread_receive_timestamp:<br>
 *      - R_OSAL_MqReceiveUntilTimeStamp return OSAL_RETURN_OK
 * -# The return value of the 1st R_OSAL_ThreadJoin is OSAL_RETURN_OK.<br>
 * "return_value1" is OSAL_RETURN_OK
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of the 2nd R_OSAL_ThreadJoin is OSAL_RETURN_OK.<br>
 * "return_value2" is OSAL_RETURN_OK
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_013_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveUntilTimeStamp_TC_03_005_013
 * @snippet test_mq.cpp osal_test_thread_receive_timeperiod
 * @snippet test_mq.cpp osal_test_thread_receive_timestamp
 */
// [MqReceiveUntilTimeStamp_TC_03_005_013]
TEST_F(MQApiTest, MqReceiveUntilTimeStamp_TC_03_005_013)
{
    st_osal_time_t  current_time;
    osal_thread_handle_t thread_handle2;
    osal_thread_handle_t thread_handle1;
    st_osal_thread_config_t config2;
    st_osal_thread_config_t config1;
    int64_t return_value1 = 0;
    int64_t return_value2 = 0;
    st_arg_mq_receive_timeperiod_t receive_timeperiod_arg;
    st_arg_mq_receive_timestamp_t receive_timestamp_arg;
    osal_milli_sec_t time_period_10ms = 10;
    osal_milli_sec_t time_period_1000ms = 1000;

    // Test03_005_013
    // @brief: normal case: Multi-thread with one queue
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);

    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    if(current_time.tv_nsec + 2 * MSEC_TO_NSEC <= MAX_NSEC)
    {
        current_time.tv_nsec = current_time.tv_nsec + 2 * MSEC_TO_NSEC;
        current_time.tv_sec = current_time.tv_sec + 1;
    }
    else
    {
        current_time.tv_nsec = 2 * MSEC_TO_NSEC - (MAX_NSEC - current_time.tv_nsec);
        current_time.tv_sec = current_time.tv_sec + 2;
    }
    receive_timeperiod_arg.mq_handle = shared_queue;
    receive_timeperiod_arg.time = 1000;
    receive_timeperiod_arg.sync_mq = sync_mq;
    receive_timeperiod_arg.sync_send_msg = sync_send_msg;
    receive_timeperiod_arg.needToSync = true;

    config1.func       = osal_test_thread_receive_timeperiod;
    config1.userarg    = (void *)&receive_timeperiod_arg;
    config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config1.task_name  = "task03_005_013_01";
    config1.stack_size = THREAD_STACK_SIZE;

    receive_timestamp_arg.mq_handle = shared_queue;
    receive_timestamp_arg.time.tv_sec = current_time.tv_sec;
    receive_timestamp_arg.time.tv_nsec = current_time.tv_nsec;
    receive_timestamp_arg.needToSync = false;

    config2.func       = osal_test_thread_receive_timestamp;
    config2.userarg    = (void *)&receive_timestamp_arg;
    config2.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config2.task_name  = "task03_005_013_02";
    config2.stack_size = THREAD_STACK_SIZE;

    EXPECT_EQ(R_OSAL_ThreadCreate(&config1, TEST_THREAD_ID1, &thread_handle1),OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &sync_receive_msg, sync_mq_config.msg_size), OSAL_RETURN_OK);
    EXPECT_EQ(sync_receive_msg, sync_send_msg);
    EXPECT_EQ(R_OSAL_ThreadCreate(&config2, TEST_THREAD_ID2, &thread_handle2),OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &return_value1),OSAL_RETURN_OK);
    EXPECT_EQ(return_value1, OSAL_RETURN_OK) << "Test03_005_013 failed";
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle2, &return_value2),OSAL_RETURN_OK);
    EXPECT_EQ(return_value2, OSAL_RETURN_OK) << "Test03_005_013 failed";
}
// [MqReceiveUntilTimeStamp_TC_03_005_013]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_014 MQApiTest.MqReceiveUntilTimeStamp_TC_03_005_014
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_014_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_014_tci Test Case ID
 * MqReceiveUntilTimeStamp_S2_A_TC_03_005_014
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_014_rr Related Requirement(s)
 * \#199174, \#199181
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_014_pp Prerequisite/Precondition
 * - OSAL is initialized and the shared_queue has been created by R_OSAL_MqCreate.
 * - The synchronization queue was created.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_014_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqReceiveUntilTimeStamp API in Abnormal behavior in multi thread
 * - Pre-process: create 2nd queue
 * - Test03_005_014: using multi thread with multi queue (empty queue).
 * - Post-process: reset and delete 2nd queue.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_014_tpts Test Procedure/Test Steps
 * -# Create another queue (call 2nd queue) along with existed queue (call 1st queue) with max_num_msg = 1 by calling R_OSAL_MqCreate.
 * -# Get current time stamp by calling R_OSAL_ClockTimeGetTimeStamp.
 * -# Create 1st thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *          -# task_name is "task03_005_014_01" 
 *          -# func is osal_test_thread_receive_timestamp function with functionality:
 *              - Get the synchronization queue config by using R_OSAL_MqGetConfig
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *              - Receive message from the queue by R_OSAL_MqReceiveUntilTimeStamp.
 *          -# userarg is receive_timestamp_arg1 with:
 *              - mq_handle is 1st queue (shared_queue)
 *              - time is time stamp got from above plus with 1s and 2000000ns.
 *              - sync_mq is handle of synchronization queue
 *              - sync_send_msg is the sending message for synchronization purpose
 *              - needToSync is true
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1
 *      - Address of thread handle.
 * -# Create 2nd thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *          -# task_name is "task03_005_014_02" 
 *          -# func is osal_test_thread_receive_timestamp function with functionality:
 *              - Get the synchronization queue config by using R_OSAL_MqGetConfig
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *              - Receive message from the queue by R_OSAL_MqReceiveUntilTimeStamp.
 *          -# userarg is receive_timestamp_arg2 with:
 *              - mq_handle is 2nd queue.
 *              - time is time stamp got from above plus with 1s and 2000000ns.
 *              - sync_mq is handle of synchronization queue
 *              - sync_send_msg is the sending message for synchronization purpose
 *              - needToSync is true
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2
 *      - Address of thread handle.
 * -# Receive 2 messages from synchronization queue by using R_OSAL_MqReceiveForTimePeriod
 * -# Create 3rd thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *          -# task_name is "task03_005_014_03" 
 *          -# func is osal_test_thread_send_timeperiod function with functionality:
 *              - Send message to the queue by R_OSAL_MqSendForTimePeriod.
 *          -# userarg is send_timeperiod_arg with:
 *              - mq_handle is 1st queue (shared_queue)
 *              - time is 1000ms
 *              - p_buffer is send_msg (0xFFFF)
 *              - buffer_size is sizeof(send_msg)
 *              - needToSync is false
 *      - thead_id is TEST_THREAD_ID3
 *      - Address of thread handle.
 * -# Calling R_OSAL_ThreadJoin to join the 3rd thread.
 * -# Calling R_OSAL_ThreadJoin to join the 1st thread.
 * -# Calling R_OSAL_ThreadJoin to join the 2nd thread.
 * -# Reset the 2nd queue by R_OSAL_MqReset.
 * -# Delete the 2nd queue by R_OSAL_MqDelete.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_014_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK and the return of the 2nd queue handle is not NULL_PTR.
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of the 1st R_OSAL_ThreadCreate is OSAL_RETURN_OK.<br>
 * In function osal_test_thread_receive_timestamp:<br>
 *      - R_OSAL_MqGetConfig return OSAL_RETURN_OK
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 *      - R_OSAL_MqReceiveUntilTimeStamp return OSAL_RETURN_OK
 * -# The return value of the 2nd R_OSAL_ThreadCreate is OSAL_RETURN_OK.<br>
 * In function osal_test_thread_receive_timestamp:<br>
 *      - R_OSAL_MqGetConfig return OSAL_RETURN_OK
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 *      - R_OSAL_MqReceiveUntilTimeStamp return OSAL_RETURN_TIME
 * -# The return value of the R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of the 3rd R_OSAL_ThreadCreate is OSAL_RETURN_OK.<br>
 * In function osal_test_thread_send_timeperiod:<br>
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * -# The return value of the 3rd R_OSAL_ThreadJoin is OSAL_RETURN_OK.<br>
 * "return_value3" is OSAL_RETURN_OK
 * -# The return value of the 1st R_OSAL_ThreadJoin is OSAL_RETURN_OK.<br>
 * "return_value1" is OSAL_RETURN_OK
 * -# The return value of the 2nd R_OSAL_ThreadJoin is OSAL_RETURN_OK.<br>
 * "return_value2" is OSAL_RETURN_TIME
 * -# The return value of R_OSAL_MqReset when reset 2nd queue is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqDelete when delete 2nd queue is OSAL_RETURN_OK.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_014_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveUntilTimeStamp_TC_03_005_014
 * @snippet test_mq.cpp osal_test_thread_send_timeperiod
 * @snippet test_mq.cpp osal_test_thread_receive_timestamp
 */
// [MqReceiveUntilTimeStamp_TC_03_005_014]
TEST_F(MQApiTest, MqReceiveUntilTimeStamp_TC_03_005_014)
{
    bool isEmpty;
    osal_mq_handle_t mq_handle1;
    st_osal_time_t  current_time;
    osal_thread_handle_t thread_handle3;
    osal_thread_handle_t thread_handle2;
    osal_thread_handle_t thread_handle1;
    st_osal_thread_config_t config3;
    st_osal_thread_config_t config2;
    st_osal_thread_config_t config1;
    int64_t return_value1 = 0;
    int64_t return_value2 = 0;
    int64_t return_value3 = 0;
    st_arg_mq_send_timeperiod_t send_timeperiod_arg;
    st_arg_mq_receive_timestamp_t receive_timestamp_arg1;
    st_arg_mq_receive_timestamp_t receive_timestamp_arg2;
    osal_milli_sec_t time_period_1000ms = 1000;


    //post-process: create 2nd queue
    mq_config.max_num_msg = 1;
    mq_config.msg_size = 4;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config, TEST_MQ_ID2, &mq_handle1), OSAL_RETURN_OK);
    EXPECT_NE(mq_handle1, NULL_PTR);

    // Test03_005_014
    // @brief: abnormal case: Multi-thread with multi-queue (empty)
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    if(current_time.tv_nsec + 2 * MSEC_TO_NSEC <= MAX_NSEC)
    {
        current_time.tv_nsec = current_time.tv_nsec + 2 * MSEC_TO_NSEC;
        current_time.tv_sec = current_time.tv_sec + 1;
    }
    else
    {
        current_time.tv_nsec = 2 * MSEC_TO_NSEC - (MAX_NSEC - current_time.tv_nsec);
        current_time.tv_sec = current_time.tv_sec + 2;
    }

    receive_timestamp_arg1.mq_handle = shared_queue;
    receive_timestamp_arg1.time.tv_sec = current_time.tv_sec;
    receive_timestamp_arg1.time.tv_nsec = current_time.tv_nsec;
    receive_timestamp_arg1.sync_mq = sync_mq;
    receive_timestamp_arg1.sync_send_msg = sync_send_msg;
    receive_timestamp_arg1.needToSync = true;
    config1.func       = osal_test_thread_receive_timestamp;
    config1.userarg    = (void *)&receive_timestamp_arg1;
    config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config1.task_name  = "task03_005_014_01";
    config1.stack_size = THREAD_STACK_SIZE;

    receive_timestamp_arg2.mq_handle = mq_handle1;
    receive_timestamp_arg2.time.tv_sec = current_time.tv_sec;
    receive_timestamp_arg2.time.tv_nsec = current_time.tv_nsec;
    receive_timestamp_arg2.sync_mq = sync_mq;
    receive_timestamp_arg2.sync_send_msg = sync_send_msg;
    receive_timestamp_arg2.needToSync = true;
    config2.func       = osal_test_thread_receive_timestamp;
    config2.userarg    = (void *)&receive_timestamp_arg2;
    config2.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config2.task_name  = "task03_005_014_02";
    config2.stack_size = THREAD_STACK_SIZE;

    send_timeperiod_arg.mq_handle = shared_queue;
    send_timeperiod_arg.time = 1000;
    send_timeperiod_arg.p_buffer = &send_msg;
    send_timeperiod_arg.buffer_size = sizeof(send_msg);
    send_timeperiod_arg.needToSync = false;
    config3.func       = osal_test_thread_send_timeperiod;
    config3.userarg    = (void *)&send_timeperiod_arg;
    config3.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config3.task_name  = "task03_005_014_03";
    config3.stack_size = THREAD_STACK_SIZE;

    EXPECT_EQ(R_OSAL_ThreadCreate(&config1, TEST_THREAD_ID1, &thread_handle1),OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThreadCreate(&config2, TEST_THREAD_ID2, &thread_handle2),OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &sync_receive_msg, sync_mq_config.msg_size), OSAL_RETURN_OK);
    EXPECT_EQ(sync_receive_msg, sync_send_msg);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &sync_receive_msg, sync_mq_config.msg_size), OSAL_RETURN_OK);
    EXPECT_EQ(sync_receive_msg, sync_send_msg);
    EXPECT_EQ(R_OSAL_ThreadCreate(&config3, TEST_THREAD_ID3, &thread_handle3),OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle3, &return_value3),OSAL_RETURN_OK);
    EXPECT_EQ(return_value3, OSAL_RETURN_OK) << "Test03_005_014 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &return_value1),OSAL_RETURN_OK);
    EXPECT_EQ(return_value1, OSAL_RETURN_OK) << "Test03_005_014 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle2, &return_value2),OSAL_RETURN_OK);
    EXPECT_EQ(return_value2, OSAL_RETURN_TIME) << "Test03_005_014 failed";

    //post-process: reset and delete 2nd queue
    EXPECT_EQ(R_OSAL_MqReset(mq_handle1), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle1), OSAL_RETURN_OK);
    mq_handle1 = OSAL_MQ_HANDLE_INVALID;
}
// [MqReceiveUntilTimeStamp_TC_03_005_014]

/**
 * @page MQ_TEST MQ API Test
 * @section OsalApiStateTest_MqReceiveUntilTimeStamp_TC_03_005_015 OsalApiStateTest.MqReceiveUntilTimeStamp_TC_03_005_015
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_015_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_015_tci Test Case ID
 * MqReceiveUntilTimeStamp_S2_A_TC_03_005_015
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_015_rr Related Requirement(s)
 * \#199174
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_015_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * - Current timestamp "abs_time_state" has been got
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_015_tcs Test Case Summary
 * This test shall checking the state of all Message queue API in Abnormal behavior in single thread
 * - Test03_005_015: checking state of R_OSAL_MqReceiveUntilTimeStamp
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_015_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_MqReceiveUntilTimeStamp with valid paramter.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_015_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqReceiveUntilTimeStamp is OSAL_RETURN_STATE.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_015_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveUntilTimeStamp_TC_03_005_015
 */
// [MqReceiveUntilTimeStamp_TC_03_005_015]
TEST_F(OsalApiStateTest, MqReceiveUntilTimeStamp_TC_03_005_015)
{
    osal_mq_handle_t mq_handle = OSAL_MQ_HANDLE_INVALID;
    st_osal_time_t current_time;
    int receive_msg;
    size_t buffer_size = 4;

    // Test03_005_015
    // @brief: abnormal case: state transition error of R_OSAL_MqReceiveUntilTimeStamp
    current_time.tv_sec = abs_time_state.tv_sec + 1;
    current_time.tv_nsec = abs_time_state.tv_nsec;
    EXPECT_EQ(R_OSAL_MqReceiveUntilTimeStamp(mq_handle,&current_time, &receive_msg, buffer_size), OSAL_RETURN_STATE) << "Test03_005_015 failed";
}
// [MqReceiveUntilTimeStamp_TC_03_005_015]

/**
 * @page MQ_TEST MQ API Test
 * @section OSALINTTESTS_MqReceiveUntilTimeStamp_TC_03_005_016 OSALINTTESTS.MqReceiveUntilTimeStamp_TC_03_005_016
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_016_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_016_tci Test Case ID
 * MqReceiveUntilTimeStamp_S3_N_TC_03_005_016
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_016_rr Related Requirement(s)
 * \#199174
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_016_pp Prerequisite/Precondition
 * -# OSAL is initialized.<br>
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_016_tcs Test Case Summary
 * @subsubsection MqReceiveUntilTimeStamp_TC_03_005_016_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Create a queue.
 * - Setup required memory for interrupt generation
 * - Register callack function and enable interrupt
 * - Wait for interrupt callback function finish
 * @subsubsection MqReceiveUntilTimeStamp_TC_03_005_016_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * - Reset created queue
 * - Destroy created queue
 * @subsubsection MqReceiveUntilTimeStamp_TC_03_005_016_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality with normal condition of the Message Queue APIs which are called in the ISR.
 * - Disable and clear interrupt status
 * - Test03_005_016: call R_OSAL_MqReceiveUntilTimeStamp in ISR
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_016_tpts Test Procedure/Test Steps
 * @subsubsection MqReceiveUntilTimeStamp_TC_03_005_016_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Create a queue by calling function R_OSAL_MqCreate with bellow config
 *      - max_num_msg: 1
 *      - msg_size: 4
 * -# Allocate required memory for generate interrupt from hardware and get hw address of allocated memory.
 * -# Call R_OSAL_InterruptGetNumOfIrqChannels to get current number of interrupt channel.
 * -# Call R_OSAL_InterruptRegisterIsr to regsiter an interrupt with a callback function.
 * -# Calling R_OSAL_InterruptEnableIsr to enable interrupt.
 * -# Setup hardware specific register to generate the interrupt.
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection MqReceiveUntilTimeStamp_TC_03_005_016_post_tpts Test Procedure/Test Steps of Post-process
 * -# Calling R_OSAL_InterruptUnregisterIsr to unregister the interrupt.
 * -# Deallocate required memory
 * -# Calling reset queue by using R_OSAL_MqReset.
 * -# Delete the message queue by caliing R_OSAL_MqDelete.
 * @subsubsection MqReceiveUntilTimeStamp_TC_03_005_016_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Call R_OSAL_InterruptDisableIsr to disable interrupt signal.
 * -# Clear the status register by calling function R_OSAL_IoWrite32 with offset is 10 and value is TEST_CLEAR_REGISTER_VALUE.
 * -# Calling R_OSAL_MqSendForTimePeriod to send a message to queue with time period is 0.
 * -# Calling function R_OSAL_ClockTimeGetTimeStamp to get current timestamp.
 * -# Calling R_OSAL_MqReceiveUntilTimeStamp to receive the message from queue .
 * -# Calling R_OSAL_MqSendForTimePeriod for Send a message that the interrupt context has ended
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_016_ereb Expected Result/Behavior
 * @subsubsection MqReceiveUntilTimeStamp_TC_03_005_016_pre_ereb Expected Result/Behavior of Pre-process
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK.
 * -# The allocation successfully without any error.
 * -# The return value of R_OSAL_InterruptGetNumOfIrqChannels is OSAL_RETURN_OK.<br>
 * The return number of channel is greater than 0.
 * -# The return value of R_OSAL_InterruptRegisterIsr is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_InterruptEnableIsr is OSAL_RETURN_OK.
 * -# Setup hardware specific register to generate the interrupt success without any error.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection MqReceiveUntilTimeStamp_TC_03_005_016_post_ereb Expected Result/Behavior of Post-process
 * -# The return value of R_OSAL_InterruptUnregisterIsr is OSAL_RETURN_OK.
 * -# Deallocate required memory success without any error.
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqDelete is OSAL_RETURN_OK.
 * @subsubsection MqReceiveUntilTimeStamp_TC_03_005_016_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# The return value of R_OSAL_InterruptDisableIsr is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_IoWrite32 is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveUntilTimeStamp is OSAL_RETURN_OK and the returned message is as expected.
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_016_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveUntilTimeStamp_TC_03_005_016
 * @snippet test_mq.cpp mq_test_int_cb1_normal_TC_03_005_016
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [MqReceiveUntilTimeStamp_TC_03_005_016]
TEST_F(OSALINTTESTS, MqReceiveUntilTimeStamp_TC_03_005_016)
{
    osal_mq_handle_t mq_handle;
    size_t numOfChannel = 0;
    st_osal_mq_config mq_config1;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE10;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    st_callback_arg_t arg;
    osal_milli_sec_t time_period = 2000;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};

/* pre-process:
 * create a queue
 * Setup required memory for interrupt generation.
 * Register callack function and enable interrupt 
 * Wait for interrupt callback function finish */
    mq_config1.max_num_msg = ISR_MQ_NUM_OF_MSG;
    mq_config1.msg_size = ISR_MQ_MSG_SIZE;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config1, ISR_MQ_ID , &mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK);
    arg.additional_arg = (void *)&mq_handle;
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, mq_test_int_cb1_normal_TC_03_005_016, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory
 * Destroy created handle */
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, mq_test_int_cb1_normal_TC_03_005_016), OSAL_RETURN_OK);
    deallocate_data_memory(hndl_mmngr, buffer_obj);
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK);
}
// [MqReceiveUntilTimeStamp_TC_03_005_016]

/**
 * @page MQ_TEST MQ API Test
 * @section OSALINTTESTS_MqReceiveUntilTimeStamp_TC_03_005_017 OSALINTTESTS.MqReceiveUntilTimeStamp_TC_03_005_017
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_017_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_017_tci Test Case ID
 * MqReceiveUntilTimeStamp_S2_A_TC_03_005_017
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_017_rr Related Requirement(s)
 * \#199174
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_017_pp Prerequisite/Precondition
 * -# OSAL is initialized.<br>
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_017_tcs Test Case Summary
 * @subsubsection MqReceiveUntilTimeStamp_TC_03_005_017_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 *      - Create a queue.
 *      - Setup required memory for interrupt generation
 *      - Register callack function and enable interrupt
 *      - Wait for interrupt callback function finish
 * @subsubsection MqReceiveUntilTimeStamp_TC_03_005_017_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 *      - Unregister interrupt callback function
 *      - Deallocate allocated memory
 *      - Reset created queue
 *      - Destroy created queue
 * @subsubsection MqReceiveUntilTimeStamp_TC_03_005_017_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality with Abnormal condition of the Message Queue APIs which are called in the ISR.
 * - Disable and clear interrupt status
 * - Test03_005_017: call R_OSAL_MqReceiveUntilTimeStamp in ISR with empty queue
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_017_tpts Test Procedure/Test Steps
 * @subsubsection MqReceiveUntilTimeStamp_TC_03_005_017_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Create a queue by calling function R_OSAL_MqCreate with bellow config
 *      - max_num_msg: 1
 *      - msg_size: 4
 * -# Allocate required memory for generate interrupt from hardware and get hw address of allocated memory.
 * -# Call R_OSAL_InterruptGetNumOfIrqChannels to get current number of interrupt channel.
 * -# Call R_OSAL_InterruptRegisterIsr to regsiter an interrupt with a callback function.
 * -# Calling R_OSAL_InterruptEnableIsr to enable interrupt.
 * -# Setup hardware specific register to generate the interrupt.
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection MqReceiveUntilTimeStamp_TC_03_005_017_post_tpts Test Procedure/Test Steps of Post-process
 * -# Calling R_OSAL_InterruptUnregisterIsr to unregister the interrupt.
 * -# Deallocate required memory
 * -# Calling reset queue by using R_OSAL_MqReset.
 * -# Delete the message queue by caliing R_OSAL_MqDelete.
 * @subsubsection MqReceiveUntilTimeStamp_TC_03_005_017_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Call R_OSAL_InterruptDisableIsr to disable interrupt signal.
 * -# Clear the status register by calling function R_OSAL_IoWrite32 with offset is 10 and value is TEST_CLEAR_REGISTER_VALUE.
 * -# Calling R_OSAL_ClockTimeGetTimeStamp to get current timestamp.
 * -# Calling R_OSAL_MqReceiveUntilTimeStamp to receive the message from the queue.
 * -# Calling R_OSAL_MqSendForTimePeriod for Send a message that the interrupt context has ended
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_017_ereb Expected Result/Behavior
 * @subsubsection MqReceiveUntilTimeStamp_TC_03_005_017_pre_ereb Expected Result/Behavior of Pre-process
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK.
 * -# The allocation successfully without any error.
 * -# The return value of R_OSAL_InterruptGetNumOfIrqChannels is OSAL_RETURN_OK.<br>
 * The return number of channel is greater than 0.
 * -# The return value of R_OSAL_InterruptRegisterIsr is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_InterruptEnableIsr is OSAL_RETURN_OK.
 * -# Setup hardware specific register to generate the interrupt success without any error.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection MqReceiveUntilTimeStamp_TC_03_005_017_post_ereb Expected Result/Behavior of Post-process
 * -# The return value of R_OSAL_InterruptUnregisterIsr is OSAL_RETURN_OK.
 * -# Deallocate required memory success without any error.
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqDelete is OSAL_RETURN_OK.
 * @subsubsection MqReceiveUntilTimeStamp_TC_03_005_017_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# The return value of R_OSAL_InterruptDisableIsr is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_IoWrite32 is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveUntilTimeStamp is OSAL_RETURN_BUSY.
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_017_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveUntilTimeStamp_TC_03_005_017
 * @snippet test_mq.cpp mq_test_int_cb1_abnormal_TC_03_005_017
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [MqReceiveUntilTimeStamp_TC_03_005_017]
TEST_F(OSALINTTESTS, MqReceiveUntilTimeStamp_TC_03_005_017)
{
    osal_mq_handle_t mq_handle;
    size_t numOfChannel = 0;
    st_osal_mq_config mq_config1;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE10;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    st_callback_arg_t arg;
    osal_milli_sec_t time_period = 2000;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};

/* pre-process:
 * create a queue
 * Setup required memory for interrupt generation.
 * Register callack function and enable interrupt 
 * Wait for interrupt callback function finish */
    mq_config1.max_num_msg = ISR_MQ_NUM_OF_MSG;
    mq_config1.msg_size = ISR_MQ_MSG_SIZE;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config1, ISR_MQ_ID , &mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK);
    arg.additional_arg = (void *)&mq_handle;
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, mq_test_int_cb1_abnormal_TC_03_005_017, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory
 * Destroy created handle */
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, mq_test_int_cb1_abnormal_TC_03_005_017), OSAL_RETURN_OK);
    deallocate_data_memory(hndl_mmngr, buffer_obj);
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK);
}
// [MqReceiveUntilTimeStamp_TC_03_005_017]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_018 MQApiTest.MqReceiveUntilTimeStamp_TC_03_005_018
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_018_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_018_tci Test Case ID
 * MqReceiveUntilTimeStamp_S0_BA_TC_03_005_018
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_018_rr Related Requirement(s)
 * \#199174
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_018_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_018_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqReceiveUntilTimeStamp API in Abnormal behavior in single thread
 * - Test03_005_018: receive message from queue with buffer_size parameter is bigger than message size
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_018_tpts Test Procedure/Test Steps
 * -# Get the mq config by using R_OSAL_MqGetConfig.<br>
 * Increase "mq_config.msg_size" by 1
 * -# Get current time stamp by R_OSAL_ClockTimeGetTimeStamp.
 * -# Receive message by R_OSAL_MqReceiveUntilTimeStamp with "mq_config.msg_size" as above
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_018_ereb Expected Result/Behavior
 * -# R_OSAL_MqGetConfig return OSAL_RETURN_OK
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK
 * -# R_OSAL_MqReceiveUntilTimeStamp return OSAL_RETURN_PAR
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_018_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveUntilTimeStamp_TC_03_005_018
 */
// [MqReceiveUntilTimeStamp_TC_03_005_018]
TEST_F(MQApiTest, MqReceiveUntilTimeStamp_TC_03_005_018)
{
    st_osal_time_t  current_time;

    // Test03_005_018
    // @brief: abnormal case: message size is bigger than mq config message size
    EXPECT_EQ(R_OSAL_MqGetConfig(shared_queue, &mq_config), OSAL_RETURN_OK) << "Test03_005_018 failed";
    mq_config.msg_size += 1;
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK) << "Test03_005_018 failed";
    if(current_time.tv_nsec + 2 * MSEC_TO_NSEC <= MAX_NSEC)
    {
        current_time.tv_nsec = current_time.tv_nsec + 2 * MSEC_TO_NSEC;
        current_time.tv_sec = current_time.tv_sec + 1;
    }
    else
    {
        current_time.tv_nsec = 2 * MSEC_TO_NSEC - (MAX_NSEC - current_time.tv_nsec);
        current_time.tv_sec = current_time.tv_sec + 2;
    }
    EXPECT_EQ(R_OSAL_MqReceiveUntilTimeStamp(shared_queue, &current_time, &receive_msg, mq_config.msg_size), OSAL_RETURN_PAR) << "Test03_005_018 failed";

}
// [MqReceiveUntilTimeStamp_TC_03_005_018]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_019 MQApiTest.MqReceiveUntilTimeStamp_TC_03_005_019
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_019_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_019_tci Test Case ID
 * MqReceiveUntilTimeStamp_S0_A_TC_03_005_019
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_019_rr Related Requirement(s)
 * \#199174
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_019_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_019_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqReceiveUntilTimeStamp API in Abnormal behavior in single thread
 * - Test03_005_019: Receive message from queue with buffer_size parameter is 0
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_019_tpts Test Procedure/Test Steps
 * -# Get current time stamp by R_OSAL_ClockTimeGetTimeStamp.
 * -# Receive message by R_OSAL_MqReceiveUntilTimeStamp with the message size is 0.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_019_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_MqReceiveUntilTimeStamp return OSAL_RETURN_PAR.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_019_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveUntilTimeStamp_TC_03_005_019
 */
// [MqReceiveUntilTimeStamp_TC_03_005_019]
TEST_F(MQApiTest, MqReceiveUntilTimeStamp_TC_03_005_019)
{
    st_osal_time_t  current_time;
    size_t buffer_size;

    // Test03_005_019
    // @brief: abnormal case: message size is 0
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK) << "Test03_005_019 failed";
    if(current_time.tv_nsec + 2 * MSEC_TO_NSEC <= MAX_NSEC)
    {
        current_time.tv_nsec = current_time.tv_nsec + 2 * MSEC_TO_NSEC;
        current_time.tv_sec = current_time.tv_sec + 1;
    }
    else
    {
        current_time.tv_nsec = 2 * MSEC_TO_NSEC - (MAX_NSEC - current_time.tv_nsec);
        current_time.tv_sec = current_time.tv_sec + 2;
    }
    buffer_size = 0;
    EXPECT_EQ(R_OSAL_MqReceiveUntilTimeStamp(shared_queue, &current_time, &receive_msg, buffer_size), OSAL_RETURN_PAR) << "Test03_005_019 failed";
}
// [MqReceiveUntilTimeStamp_TC_03_005_019]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_020 MQApiTest.MqReceiveUntilTimeStamp_TC_03_005_020
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_020_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_020_tci Test Case ID
 * MqReceiveUntilTimeStamp_S0_A_TC_03_005_020
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_020_rr Related Requirement(s)
 * \#199174
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_020_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_020_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqReceiveUntilTimeStamp API in Abnormal behavior in single thread
 * - Test03_005_020: receive message from queue with tv_sec is negative number
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_020_tpts Test Procedure/Test Steps
 * -# Get the queue config by calling R_OSAL_MqGetConfig.
 * -# Get current time stamp by calling R_OSAL_ClockTimeGetTimeStamp.
 * -# Call R_OSAL_MqReceiveUntilTimeStamp with timestamp.tv_sec is -1.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_020_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqGetConfig is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveUntilTimeStamp is OSAL_RETURN_PAR.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_020_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveUntilTimeStamp_TC_03_005_020
 */
// [MqReceiveUntilTimeStamp_TC_03_005_020]
TEST_F(MQApiTest, MqReceiveUntilTimeStamp_TC_03_005_020)
{
    st_osal_time_t  current_time;

    // Test03_005_020
    // @brief: abnormal case: tv_sec is negative number
    EXPECT_EQ(R_OSAL_MqGetConfig(shared_queue, &mq_config), OSAL_RETURN_OK) << "Test03_005_020 failed";
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK) << "Test03_005_020 failed";
    current_time.tv_sec = -1;
    EXPECT_EQ(R_OSAL_MqReceiveUntilTimeStamp(shared_queue, &current_time, &send_msg, mq_config.msg_size), OSAL_RETURN_PAR) << "Test03_005_020 failed";
}
// [MqReceiveUntilTimeStamp_TC_03_005_020]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_022 MQApiTest.MqReceiveUntilTimeStamp_TC_03_005_022
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_022_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_022_tci Test Case ID
 * MqReceiveUntilTimeStamp_S0_BN_TC_03_005_022
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_022_rr Related Requirement(s)
 * \#199174
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_022_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_022_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqReceiveUntilTimeStamp API
 * - Test03_005_022: receive message from queue with tv_sec is equal to zero
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_022_tpts Test Procedure/Test Steps
 * -# Get the queue config by calling R_OSAL_MqGetConfig.
 * -# Send a message to the queue by using R_OSAL_MqSendForTimePeriod
 * -# Get current time stamp by calling R_OSAL_ClockTimeGetTimeStamp.
 * -# Call R_OSAL_MqReceiveUntilTimeStamp with tv_sec is equal to zero
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_022_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqGetConfig is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveUntilTimeStamp is OSAL_RETURN_OK.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_022_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveUntilTimeStamp_TC_03_005_022
 */
// [MqReceiveUntilTimeStamp_TC_03_005_022]
TEST_F(MQApiTest, MqReceiveUntilTimeStamp_TC_03_005_022)
{
    st_osal_time_t  current_time;
    int time_period_10ms = 10;

    // Test03_005_022
    // @brief: boundary normal case: tv_sec is equal to zero
    EXPECT_EQ(R_OSAL_MqGetConfig(shared_queue, &mq_config), OSAL_RETURN_OK) << "Test03_005_022 failed";
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue, time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK) << "Test03_005_022 failed";
    current_time.tv_sec = 0;
    EXPECT_EQ(R_OSAL_MqReceiveUntilTimeStamp(shared_queue, &current_time, &receive_msg, mq_config.msg_size), OSAL_RETURN_OK) << "Test03_005_022 failed";
    EXPECT_EQ(receive_msg, send_msg) << "Test03_005_022 failed";
}
// [MqReceiveUntilTimeStamp_TC_03_005_022]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_023 MQApiTest.MqReceiveUntilTimeStamp_TC_03_005_023
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_023_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_023_tci Test Case ID
 * MqReceiveUntilTimeStamp_S0_BN_TC_03_005_023
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_023_rr Related Requirement(s)
 * \#199174
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_023_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_023_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqReceiveUntilTimeStamp API
 * - Test03_005_023: receive message from queue with tv_nsec is equal to zero
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_023_tpts Test Procedure/Test Steps
 * -# Get the queue config by calling R_OSAL_MqGetConfig.
 * -# Send a message to the queue by using R_OSAL_MqSendForTimePeriod
 * -# Get current time stamp by calling R_OSAL_ClockTimeGetTimeStamp.
 * -# Call R_OSAL_MqReceiveUntilTimeStamp with tv_nsec is equal to zero
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_023_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqGetConfig is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveUntilTimeStamp is OSAL_RETURN_OK.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_023_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveUntilTimeStamp_TC_03_005_023
 */
// [MqReceiveUntilTimeStamp_TC_03_005_023]
TEST_F(MQApiTest, MqReceiveUntilTimeStamp_TC_03_005_023)
{
    st_osal_time_t  current_time;
    int time_period_10ms = 10;

    // Test03_005_023
    // @brief: boundary normal case: tv_nsec is equal to zero
    EXPECT_EQ(R_OSAL_MqGetConfig(shared_queue, &mq_config), OSAL_RETURN_OK) << "Test03_005_023 failed";
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue, time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK) << "Test03_005_023 failed";
    current_time.tv_nsec = 0;
    EXPECT_EQ(R_OSAL_MqReceiveUntilTimeStamp(shared_queue, &current_time, &receive_msg, mq_config.msg_size), OSAL_RETURN_OK) << "Test03_005_023 failed";
    EXPECT_EQ(receive_msg, send_msg) << "Test03_005_023 failed";
}
// [MqReceiveUntilTimeStamp_TC_03_005_023]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_024 MQApiTest.MqReceiveUntilTimeStamp_TC_03_005_024
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_024_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_024_tci Test Case ID
 * MqReceiveUntilTimeStamp_S0_BA_TC_03_005_024
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_024_rr Related Requirement(s)
 * \#199174
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_024_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_024_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqReceiveUntilTimeStamp API
 * - Test03_005_024: receive message from queue with tv_nsec is less than zero
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_024_tpts Test Procedure/Test Steps
 * -# Get the queue config by calling R_OSAL_MqGetConfig.
 * -# Get current time stamp by calling R_OSAL_ClockTimeGetTimeStamp.
 * -# Call R_OSAL_MqReceiveUntilTimeStamp with tv_nsec is less than zero
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_024_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqGetConfig is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveUntilTimeStamp is OSAL_RETURN_PAR.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_024_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveUntilTimeStamp_TC_03_005_024
 */
// [MqReceiveUntilTimeStamp_TC_03_005_024]
TEST_F(MQApiTest, MqReceiveUntilTimeStamp_TC_03_005_024)
{
    st_osal_time_t  current_time;
    int time_period_10ms = 10;

    // Test03_005_024
    // @brief: boundary abnormal case: tv_nsec is less than zero
    EXPECT_EQ(R_OSAL_MqGetConfig(shared_queue, &mq_config), OSAL_RETURN_OK) << "Test03_005_024 failed";
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK) << "Test03_005_024 failed";
    current_time.tv_nsec = -1;
    EXPECT_EQ(R_OSAL_MqReceiveUntilTimeStamp(shared_queue, &current_time, &receive_msg, mq_config.msg_size), OSAL_RETURN_PAR) << "Test03_005_024 failed";
}
// [MqReceiveUntilTimeStamp_TC_03_005_024]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveUntilTimeStamp_TC_03_005_025 MQApiTest.MqReceiveUntilTimeStamp_TC_03_005_025
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_025_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_025_tci Test Case ID
 * MqReceiveUntilTimeStamp_S0_BN_TC_03_005_025
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_025_rr Related Requirement(s)
 * \#199174
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_025_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_025_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqReceiveUntilTimeStamp API
 * - Test03_005_025: receive message from queue with tv_sec and tv_nsec are equal to zero
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_025_tpts Test Procedure/Test Steps
 * -# Get the queue config by calling R_OSAL_MqGetConfig.
 * -# Send a message to the queue by using R_OSAL_MqSendForTimePeriod
 * -# Call R_OSAL_MqReceiveUntilTimeStamp with tv_sec and tv_nsec are equal to zero
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_025_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqGetConfig is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveUntilTimeStamp is OSAL_RETURN_OK.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_025_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveUntilTimeStamp_TC_03_005_025
 */
// [MqReceiveUntilTimeStamp_TC_03_005_025]
TEST_F(MQApiTest, MqReceiveUntilTimeStamp_TC_03_005_025)
{
    st_osal_time_t  current_time;
    int time_period_10ms = 10;

    // Test03_005_025
    // @brief: boundary normal case: tv_sec and tv_nsec are equal to zero
    EXPECT_EQ(R_OSAL_MqGetConfig(shared_queue, &mq_config), OSAL_RETURN_OK) << "Test03_005_025 failed";
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue, time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    current_time.tv_sec = 0;
    current_time.tv_nsec = 0;
    EXPECT_EQ(R_OSAL_MqReceiveUntilTimeStamp(shared_queue, &current_time, &receive_msg, mq_config.msg_size), OSAL_RETURN_OK) << "Test03_005_025 failed";
    EXPECT_EQ(receive_msg, send_msg) << "Test03_005_025 failed";
}
// [MqReceiveUntilTimeStamp_TC_03_005_025]

/**
 * @page MQ_TEST MQ API Test
 * @section OSALINTTESTS_MqReceiveUntilTimeStamp_TC_03_005_026 OSALINTTESTS.MqReceiveUntilTimeStamp_TC_03_005_026
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_026_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_026_tci Test Case ID
 * MqReceiveUntilTimeStamp_S2_A_TC_03_005_026
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_026_rr Related Requirement(s)
 * \#199174
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_026_pp Prerequisite/Precondition
 * -# OSAL is initialized.<br>
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_026_tcs Test Case Summary
 * @subsubsection MqReceiveUntilTimeStamp_TC_03_005_026_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 *      - Create a queue and make this queue to be full
 *      - Create a waiting thread. This thread is blocked by the above queue
 *      - Setup required memory for interrupt generation
 *      - Register callack function and enable interrupt
 *      - Wait for interrupt callback function finish
 * @subsubsection MqReceiveUntilTimeStamp_TC_03_005_026_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 *      - Unregister interrupt callback function
 *      - Deallocate allocated memory
 *      - Reset created queue
 *      - Destroy created queue
 *      - Join the waiting thread
 * @subsubsection MqReceiveUntilTimeStamp_TC_03_005_026_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality with Abnormal condition of the Message Queue APIs which are called in the ISR.
 * - Disable and clear interrupt status
 * - Test03_005_026: call R_OSAL_MqReceiveUntilTimeStamp in ISR with full queue
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_026_tpts Test Procedure/Test Steps
 * @subsubsection MqReceiveUntilTimeStamp_TC_03_005_026_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Create a queue by calling function R_OSAL_MqCreate with bellow config
 *      - max_num_msg: 1
 *      - msg_size: 4
 * -# Send a message to this queue to make it full by using R_OSAL_MqSendForTimePeriod
 * -# Create waiting thread by using R_OSAL_ThreadCreate. Make this thread blocked by above queue by sending another message to this queue.
 * -# Allocate required memory for generate interrupt from hardware and get hw address of allocated memory.
 * -# Call R_OSAL_InterruptGetNumOfIrqChannels to get current number of interrupt channel.
 * -# Call R_OSAL_InterruptRegisterIsr to regsiter an interrupt with a callback function.
 * -# Calling R_OSAL_InterruptEnableIsr to enable interrupt.
 * -# Setup hardware specific register to generate the interrupt.
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection MqReceiveUntilTimeStamp_TC_03_005_026_post_tpts Test Procedure/Test Steps of Post-process
 * -# Calling R_OSAL_InterruptUnregisterIsr to unregister the interrupt.
 * -# Deallocate required memory
 * -# Calling reset queue by using R_OSAL_MqReset.
 * -# Delete the message queue by caliing R_OSAL_MqDelete.
 * -# Join thread by using R_OSAL_ThreadJoin
 * @subsubsection MqReceiveUntilTimeStamp_TC_03_005_026_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Call R_OSAL_InterruptDisableIsr to disable interrupt signal.
 * -# Clear the status register by calling function R_OSAL_IoWrite32 with offset is 10 and value is TEST_CLEAR_REGISTER_VALUE.
 * -# Calling R_OSAL_ClockTimeGetTimeStamp to get current timestamp.
 * -# Calling R_OSAL_MqReceiveUntilTimeStamp to receive the message from the queue.
 * -# Calling R_OSAL_MqSendForTimePeriod for Send a message that the interrupt context has ended
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_026_ereb Expected Result/Behavior
 * @subsubsection MqReceiveUntilTimeStamp_TC_03_005_026_pre_ereb Expected Result/Behavior of Pre-process
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_ThreadCreate is OSAL_RETURN_OK.
 * -# The allocation successfully without any error.
 * -# The return value of R_OSAL_InterruptGetNumOfIrqChannels is OSAL_RETURN_OK.<br>
 * The return number of channel is greater than 0.
 * -# The return value of R_OSAL_InterruptRegisterIsr is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_InterruptEnableIsr is OSAL_RETURN_OK.
 * -# Setup hardware specific register to generate the interrupt success without any error.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection MqReceiveUntilTimeStamp_TC_03_005_026_post_ereb Expected Result/Behavior of Post-process
 * -# The return value of R_OSAL_InterruptUnregisterIsr is OSAL_RETURN_OK.
 * -# Deallocate required memory success without any error.
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqDelete is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_ThreadJoin is OSAL_RETURN_OK.
 * @subsubsection MqReceiveUntilTimeStamp_TC_03_005_026_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# The return value of R_OSAL_InterruptDisableIsr is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_IoWrite32 is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveUntilTimeStamp is OSAL_RETURN_BUSY.
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_026_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveUntilTimeStamp_TC_03_005_026
 * @snippet test_mq.cpp mq_test_int_cb_TC_03_005_026
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [MqReceiveUntilTimeStamp_TC_03_005_026]
TEST_F(OSALINTTESTS, MqReceiveUntilTimeStamp_TC_03_005_026)
{
    osal_mq_handle_t mq_handle;
    size_t numOfChannel = 0;
    st_osal_mq_config mq_config1;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE10;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    st_callback_arg_t arg;
    osal_milli_sec_t time_period = 2000;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};
    osal_milli_sec_t time_period_10ms = 10;
    st_osal_thread_config_t config;
    osal_thread_handle_t thread_handle;
    st_arg_mq_send_timeperiod_t send_timeperiod_arg;
    int64_t return_value;

/* pre-process:
 * create a queue
 * Setup required memory for interrupt generation.
 * Register callack function and enable interrupt
 * Wait for interrupt callback function finish */
    mq_config1.max_num_msg = ISR_MQ_NUM_OF_MSG;
    mq_config1.msg_size = ISR_MQ_MSG_SIZE;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config1, ISR_MQ_ID , &mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(mq_handle, time_period_10ms, &send_msg, mq_config1.msg_size), OSAL_RETURN_OK);

    send_timeperiod_arg.mq_handle = mq_handle;
    send_timeperiod_arg.time = 5000;
    send_timeperiod_arg.p_buffer =  &send_msg;
    send_timeperiod_arg.buffer_size = mq_config1.msg_size;
    send_timeperiod_arg.needToSync = false;

    config.func       = osal_test_thread_send_timeperiod;
    config.userarg    = (void *)&send_timeperiod_arg;
    config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config.task_name  = "task03_005_026";
    config.stack_size = THREAD_STACK_SIZE;
    EXPECT_EQ(R_OSAL_ThreadCreate(&config, TEST_THREAD_ID1, &thread_handle),OSAL_RETURN_OK);

    arg.additional_arg = (void *)&mq_handle;
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, mq_test_int_cb_TC_03_005_026, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory
 * Destroy created handle */
    EXPECT_EQ(R_OSAL_ThreadSleepForTimePeriod(100), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, mq_test_int_cb_TC_03_005_026), OSAL_RETURN_OK);
    deallocate_data_memory(hndl_mmngr, buffer_obj);
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &return_value), OSAL_RETURN_OK);
    EXPECT_EQ(return_value, OSAL_RETURN_OK);
}
// [MqReceiveUntilTimeStamp_TC_03_005_026]
#endif

#ifndef SKIP_SECTION_2
/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveForTimePeriod_TC_03_006_001 MQApiTest.MqReceiveForTimePeriod_TC_03_006_001
 * @subsection MqReceiveForTimePeriod_TC_03_006_001_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveForTimePeriod_TC_03_006_001_tci Test Case ID
 * MqReceiveForTimePeriod_S1_N_TC_03_006_001
 * @subsection MqReceiveForTimePeriod_TC_03_006_001_rr Related Requirement(s)
 * \#199175
 * @subsection MqReceiveForTimePeriod_TC_03_006_001_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqReceiveForTimePeriod_TC_03_006_001_tcs Test Case Summary
 * This test shall verify the functionality of MqReceiveForTimePeriod API in Normal behavior in single thread
 * - Test03_006_001: receive a message from queue within 10ms.
 * @subsection MqReceiveForTimePeriod_TC_03_006_001_tpts Test Procedure/Test Steps
 * -# Send a message to queue using R_OSAL_MqSendForTimePeriod.
 * -# Receive message by R_OSAL_MqReceiveForTimePeriod with the time period = 10.
 * @subsection MqReceiveForTimePeriod_TC_03_006_001_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK and the received message is the same as the sent message.
 * @subsection MqReceiveForTimePeriod_TC_03_006_001_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveForTimePeriod_TC_03_006_001
 */
// [MqReceiveForTimePeriod_TC_03_006_001]
TEST_F(MQApiTest, MqReceiveForTimePeriod_TC_03_006_001)
{
    osal_milli_sec_t time_period_10ms = 10;
    size_t buffer_size = 4;

    // Test03_006_001
    // @brief: normal case: time_period = 10
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(shared_queue,time_period_10ms, &receive_msg, buffer_size), OSAL_RETURN_OK) << "Test03_006_001 failed";
    EXPECT_EQ(receive_msg, send_msg);
    receive_msg = 0;
}
// [MqReceiveForTimePeriod_TC_03_006_001]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveForTimePeriod_TC_03_006_002 MQApiTest.MqReceiveForTimePeriod_TC_03_006_002
 * @subsection MqReceiveForTimePeriod_TC_03_006_002_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveForTimePeriod_TC_03_006_002_tci Test Case ID
 * MqReceiveForTimePeriod_S1_BN_TC_03_006_002
 * @subsection MqReceiveForTimePeriod_TC_03_006_002_rr Related Requirement(s)
 * \#199175
 * @subsection MqReceiveForTimePeriod_TC_03_006_002_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqReceiveForTimePeriod_TC_03_006_002_tcs Test Case Summary
 * This test shall verify the functionality of MqReceiveForTimePeriod API in Normal behavior in single thread
 * - Test03_006_002: receive a message from queue within 0ms.
 * @subsection MqReceiveForTimePeriod_TC_03_006_002_tpts Test Procedure/Test Steps
 * -# Send a message to queue using R_OSAL_MqSendForTimePeriod.
 * -# Receive message by R_OSAL_MqReceiveForTimePeriod with the time period = 0.
 * @subsection MqReceiveForTimePeriod_TC_03_006_002_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK and the received message is the same as the sent message.
 * @subsection MqReceiveForTimePeriod_TC_03_006_002_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveForTimePeriod_TC_03_006_002
 */
// [MqReceiveForTimePeriod_TC_03_006_002]
TEST_F(MQApiTest, MqReceiveForTimePeriod_TC_03_006_002)
{
    osal_milli_sec_t time_period_10ms = 10;
    osal_milli_sec_t time_period_0ms = 0;
    size_t buffer_size = 4;

    // Test03_006_002
    // @brief: normal case: time_period = 0
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(shared_queue,time_period_0ms, &receive_msg, buffer_size), OSAL_RETURN_OK) << "Test03_006_002 failed";
    EXPECT_EQ(receive_msg, send_msg);
    receive_msg = 0;
}
// [MqReceiveForTimePeriod_TC_03_006_002]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveForTimePeriod_TC_03_006_003 MQApiTest.MqReceiveForTimePeriod_TC_03_006_003
 * @subsection MqReceiveForTimePeriod_TC_03_006_003_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveForTimePeriod_TC_03_006_003_tci Test Case ID
 * MqReceiveForTimePeriod_S0_A_TC_03_006_003
 * @subsection MqReceiveForTimePeriod_TC_03_006_003_rr Related Requirement(s)
 * \#199151, \#199165, \#199175
 * @subsection MqReceiveForTimePeriod_TC_03_006_003_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqReceiveForTimePeriod_TC_03_006_003_tcs Test Case Summary
 * This test shall verify the functionality of MqReceiveForTimePeriod API in Abnormal behavior in single thread
 * - Test03_006_003: receive a message with mq handle parameter is NULL.
 * @subsection MqReceiveForTimePeriod_TC_03_006_003_tpts Test Procedure/Test Steps
 * -# Receive a message from queue using R_OSAL_MqReceiveForTimePeriod with mq_handle is NULL.
 * @subsection MqReceiveForTimePeriod_TC_03_006_003_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_HANDLE.
 * @subsection MqReceiveForTimePeriod_TC_03_006_003_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveForTimePeriod_TC_03_006_003
 */
// [MqReceiveForTimePeriod_TC_03_006_003]
TEST_F(MQApiTest, MqReceiveForTimePeriod_TC_03_006_003)
{
    osal_milli_sec_t time_period_10ms = 10;
    size_t buffer_size = 4;

    // Test03_006_003
    // @brief: abnormal case: handle = NULL
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(OSAL_MQ_HANDLE_INVALID, time_period_10ms, &receive_msg, buffer_size), OSAL_RETURN_HANDLE) << "Test03_006_003 failed";
    EXPECT_EQ(receive_msg, 0);
}
// [MqReceiveForTimePeriod_TC_03_006_003]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveForTimePeriod_TC_03_006_004 MQApiTest.MqReceiveForTimePeriod_TC_03_006_004
 * @subsection MqReceiveForTimePeriod_TC_03_006_004_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveForTimePeriod_TC_03_006_004_tci Test Case ID
 * MqReceiveForTimePeriod_S2_A_TC_03_006_004
 * @subsection MqReceiveForTimePeriod_TC_03_006_004_rr Related Requirement(s)
 * \#199175
 * @subsection MqReceiveForTimePeriod_TC_03_006_004_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqReceiveForTimePeriod_TC_03_006_004_tcs Test Case Summary
 * This test shall verify the functionality of MqReceiveForTimePeriod API in Abnormal behavior in single thread
 * - Test03_006_004: receive a message from a deleted queue.
 * @subsection MqReceiveForTimePeriod_TC_03_006_004_tpts Test Procedure/Test Steps
 * -# Create a new queue by calling R_OSAL_MqCreate.
 * -# Delete created queue by calling R_OSAL_MqDelete.
 * -# Receive message from the queue by calling R_OSAL_MqReceiveForTimePeriod with all valid parameter.
 * @subsection MqReceiveForTimePeriod_TC_03_006_004_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK, and the return handle is not NULL_PTR.
 * -# The return value of R_OSAL_MqDelete is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_HANDLE.
 * @subsection MqReceiveForTimePeriod_TC_03_006_004_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveForTimePeriod_TC_03_006_004
 */
// [MqReceiveForTimePeriod_TC_03_006_004]
TEST_F(MQApiTest, MqReceiveForTimePeriod_TC_03_006_004)
{
    osal_milli_sec_t time_period_10ms = 10;
    size_t buffer_size = 4;

    // Test03_006_004
    // @brief: abnormal case: handle is deleted
    mq_config.max_num_msg = 1;
    mq_config.msg_size = 4;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config, TEST_MQ_ID2, &mq_handle), OSAL_RETURN_OK);
    EXPECT_NE(mq_handle, NULL_PTR);
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(mq_handle,time_period_10ms, &receive_msg, buffer_size), OSAL_RETURN_HANDLE) << "Test03_006_004 failed";
}
// [MqReceiveForTimePeriod_TC_03_006_004]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveForTimePeriod_TC_03_006_005 MQApiTest.MqReceiveForTimePeriod_TC_03_006_005
 * @subsection MqReceiveForTimePeriod_TC_03_006_005_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveForTimePeriod_TC_03_006_005_tci Test Case ID
 * MqReceiveForTimePeriod_S0_BA_TC_03_006_005
 * @subsection MqReceiveForTimePeriod_TC_03_006_005_rr Related Requirement(s)
 * \#199175
 * @subsection MqReceiveForTimePeriod_TC_03_006_005_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqReceiveForTimePeriod_TC_03_006_005_tcs Test Case Summary
 * This test shall verify the functionality of MqReceiveForTimePeriod API in Abnormal behavior in single thread
 * - Test03_006_005: receive a message from queue with time period is a negative value (NEGATIVE_TIME).
 * @subsection MqReceiveForTimePeriod_TC_03_006_005_tpts Test Procedure/Test Steps
 * -# Receive a message from queue using R_OSAL_MqReceiveForTimePeriod with the time period is NEGATIVE_TIME.
 * @subsection MqReceiveForTimePeriod_TC_03_006_005_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_PAR.
 * @subsection MqReceiveForTimePeriod_TC_03_006_005_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveForTimePeriod_TC_03_006_005
 */
// [MqReceiveForTimePeriod_TC_03_006_005]
TEST_F(MQApiTest, MqReceiveForTimePeriod_TC_03_006_005)
{
    size_t buffer_size = 4;

    // Test03_006_005
    // @brief: abnormal case: time_period is a negative value
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(shared_queue, NEGATIVE_TIME, &receive_msg, buffer_size), OSAL_RETURN_PAR) << "Test03_006_005 failed";
}
// [MqReceiveForTimePeriod_TC_03_006_005]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveForTimePeriod_TC_03_006_006 MQApiTest.MqReceiveForTimePeriod_TC_03_006_006
 * @subsection MqReceiveForTimePeriod_TC_03_006_006_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveForTimePeriod_TC_03_006_006_tci Test Case ID
 * MqReceiveForTimePeriod_S0_A_TC_03_006_006
 * @subsection MqReceiveForTimePeriod_TC_03_006_006_rr Related Requirement(s)
 * \#199175
 * @subsection MqReceiveForTimePeriod_TC_03_006_006_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqReceiveForTimePeriod_TC_03_006_006_tcs Test Case Summary
 * This test shall verify the functionality of MqReceiveForTimePeriod API in Abnormal behavior in single thread
 * - Test03_006_006: receive a message from queue with with with p_buffer parameter is NULL
 * @subsection MqReceiveForTimePeriod_TC_03_006_006_tpts Test Procedure/Test Steps
 * -# Receive a message from queue using R_OSAL_MqReceiveForTimePeriod with the p_buffer = NULL.
 * @subsection MqReceiveForTimePeriod_TC_03_006_006_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_PAR.
 * @subsection MqReceiveForTimePeriod_TC_03_006_006_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveForTimePeriod_TC_03_006_006
 */
// [MqReceiveForTimePeriod_TC_03_006_006]
TEST_F(MQApiTest, MqReceiveForTimePeriod_TC_03_006_006)
{
    osal_milli_sec_t time_period_10ms = 10;
    size_t buffer_size = 4;
    void * p_buffer;

    // Test03_006_006
    // @brief: abnormal case: p_buffer = NULL
    p_buffer = NULL;
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(shared_queue,time_period_10ms, p_buffer, buffer_size), OSAL_RETURN_PAR) << "Test03_006_006 failed";
}
// [MqReceiveForTimePeriod_TC_03_006_006]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveForTimePeriod_TC_03_006_007 MQApiTest.MqReceiveForTimePeriod_TC_03_006_007
 * @subsection MqReceiveForTimePeriod_TC_03_006_007_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveForTimePeriod_TC_03_006_007_tci Test Case ID
 * MqReceiveForTimePeriod_S0_A_TC_03_006_007
 * @subsection MqReceiveForTimePeriod_TC_03_006_007_rr Related Requirement(s)
 * \#199151, \#199165, \#199175
 * @subsection MqReceiveForTimePeriod_TC_03_006_007_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqReceiveForTimePeriod_TC_03_006_007_tcs Test Case Summary
 * This test shall verify the functionality of MqReceiveForTimePeriod API in Abnormal behavior in single thread
 * - Test03_006_007: receive a message from queue with buffer_size (is 1) is less than message size in osal config of handle.
 * @subsection MqReceiveForTimePeriod_TC_03_006_007_tpts Test Procedure/Test Steps
 * -# Get mq config by using R_OSAL_MqGetConfig.<br>
 * Decrease "mq_config.msg_size" by 1.
 * -# Receive a message from queue using R_OSAL_MqReceiveForTimePeriod with "mq_config.msg_size" as above
 * @subsection MqReceiveForTimePeriod_TC_03_006_007_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqGetConfig is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_PAR.
 * @subsection MqReceiveForTimePeriod_TC_03_006_007_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveForTimePeriod_TC_03_006_007
 */
// [MqReceiveForTimePeriod_TC_03_006_007]
TEST_F(MQApiTest, MqReceiveForTimePeriod_TC_03_006_007)
{
    osal_milli_sec_t time_period_10ms = 10;

    // Test03_006_007
    // @brief: abnormal case: message size is less than mq config message size
    EXPECT_EQ(R_OSAL_MqGetConfig(shared_queue, &mq_config), OSAL_RETURN_OK) << "Test03_006_007 failed";
    mq_config.msg_size -= 1;
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(shared_queue,time_period_10ms, &receive_msg, mq_config.msg_size), OSAL_RETURN_PAR) << "Test03_006_007 failed";
    EXPECT_EQ(receive_msg, 0);
}
// [MqReceiveForTimePeriod_TC_03_006_007]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveForTimePeriod_TC_03_006_008 MQApiTest.MqReceiveForTimePeriod_TC_03_006_008
 * @subsection MqReceiveForTimePeriod_TC_03_006_008_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveForTimePeriod_TC_03_006_008_tci Test Case ID
 * MqReceiveForTimePeriod_S2_A_TC_03_006_008
 * @subsection MqReceiveForTimePeriod_TC_03_006_008_rr Related Requirement(s)
 * \#199175
 * @subsection MqReceiveForTimePeriod_TC_03_006_008_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqReceiveForTimePeriod_TC_03_006_008_tcs Test Case Summary
 * This test shall verify the functionality of MqReceiveForTimePeriod API in Abnormal behavior in single thread
 * - Test03_006_008: receive message from an empty queue.
 * @subsection MqReceiveForTimePeriod_TC_03_006_008_tpts Test Procedure/Test Steps
 * -# Call R_OSAL_MqIsEmpty to check whether the queue is empty or not.
 * -# Receive the message from the queue by calling R_OSAL_MqReceiveForTimePeriod.
 * @subsection MqReceiveForTimePeriod_TC_03_006_008_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqIsEmpty is OSAL_RETURN_OK and the return of empty check is TRUE.
 * -# The return value of tR_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_TIME due to the queue was empty.
 * @subsection MqReceiveForTimePeriod_TC_03_006_008_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveForTimePeriod_TC_03_006_008
 */
// [MqReceiveForTimePeriod_TC_03_006_008]
TEST_F(MQApiTest, MqReceiveForTimePeriod_TC_03_006_008)
{
    bool isEmpty = false;
    osal_milli_sec_t time_period_10ms = 10;
    size_t buffer_size;

    // Test03_006_008
    // @brief: abnormal case: Receive an empty queue
    EXPECT_EQ(R_OSAL_MqIsEmpty(shared_queue, &isEmpty), OSAL_RETURN_OK);
    EXPECT_TRUE(isEmpty);
    buffer_size = 4;
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(shared_queue,time_period_10ms, &receive_msg, buffer_size), OSAL_RETURN_TIME) << "Test03_006_008 failed";
    EXPECT_EQ(receive_msg, 0);
}
// [MqReceiveForTimePeriod_TC_03_006_008]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveForTimePeriod_TC_03_006_009 MQApiTest.MqReceiveForTimePeriod_TC_03_006_009
 * @subsection MqReceiveForTimePeriod_TC_03_006_009_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveForTimePeriod_TC_03_006_009_tci Test Case ID
 * MqReceiveForTimePeriod_S3_N_TC_03_006_009
 * @subsection MqReceiveForTimePeriod_TC_03_006_009_rr Related Requirement(s)
 * \#199175, \#199181
 * @subsection MqReceiveForTimePeriod_TC_03_006_009_pp Prerequisite/Precondition
 * - OSAL is initialized and the shared_queue has been created by R_OSAL_MqCreate.
 * - The synchronization queue was created.
 * @subsection MqReceiveForTimePeriod_TC_03_006_009_tcs Test Case Summary
 * This test shall verify the functionality of MqReceiveForTimePeriod API in Normal behavior in multi thread
 * - Test03_006_009: using multi thread with one queue.
 * @subsection MqReceiveForTimePeriod_TC_03_006_009_tpts Test Procedure/Test Steps
 * -# Send the 1st message to the queue by calling R_OSAL_MqSendForTimePeriod.
 * -# Create 1st thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *          -# task_name is "task03_006_009_01" 
 *          -# func is osal_test_thread_receive_timeperiod function with functionality:
 *              - Get the synchronization queue config by using R_OSAL_MqGetConfig
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *              - Receive message from the queue by R_OSAL_MqReceiveForTimePeriod.
 *          -# userarg is receive_timeperiod_arg1 with:
 *              - mq_handle is shared_queue.
 *              - time is 2000ms
 *              - sync_mq is handle of synchronization queue
 *              - sync_send_msg is the sending message for synchronization purpose
 *              - needToSync is true
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1
 *      - Address of thread handle.
 * -# Receive message from synchronization queue by using R_OSAL_MqReceiveForTimePeriod
 * -# Create 2nd thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *          -# task_name is "task03_006_009_02" 
 *          -# func is osal_test_thread_receive_timeperiod function with functionality:
 *              - Receive message from the queue by R_OSAL_MqReceiveForTimePeriod.
 *          -# userarg is receive_timeperiod_arg2 with:
 *              - mq_handle is shared_queue.
 *              - time is 2000ms
 *              - needToSync is false
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2
 *      - Address of thread handle.
 * -# Calling R_OSAL_ThreadJoin to joining the 1st thread.
 * -# Calling function R_OSAL_MqSendForTimePeriod to send 1 message to the queue.
 * -# Calling R_OSAL_ThreadJoin to joining the 2nd thread.
 * @subsection MqReceiveForTimePeriod_TC_03_006_009_ereb Expected Result/Behavior
 * -# The return value the 1st R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of the 1st R_OSAL_ThreadCreate is OSAL_RETURN_OK.<br>
 * In function osal_test_thread_receive_timeperiod:<br>
 *      - R_OSAL_MqGetConfig return OSAL_RETURN_OK
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 *      - R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of the 2nd R_OSAL_ThreadCreate is OSAL_RETURN_OK.<br>
 * In function osal_test_thread_receive_timeperiod:<br>
 *      - R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK
 * -# The return value of the 1st R_OSAL_ThreadJoin is OSAL_RETURN_OK.<br>
 * "return_value1" is OSAL_RETURN_OK
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of the 2nd R_OSAL_ThreadJoin is OSAL_RETURN_OK.<br>
 * "return_value2" is OSAL_RETURN_OK
 * @subsection MqReceiveForTimePeriod_TC_03_006_009_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveForTimePeriod_TC_03_006_009
 * @snippet test_mq.cpp osal_test_thread_receive_timeperiod
 */
// [MqReceiveForTimePeriod_TC_03_006_009]
TEST_F(MQApiTest, MqReceiveForTimePeriod_TC_03_006_009)
{
    osal_thread_handle_t thread_handle2 = OSAL_THREAD_HANDLE_INVALID;
    osal_thread_handle_t thread_handle1 = OSAL_THREAD_HANDLE_INVALID;
    st_osal_thread_config_t config2;
    st_osal_thread_config_t config1;
    int64_t return_value1 = 0;
    int64_t return_value2 = 0;
    st_arg_mq_receive_timeperiod_t receive_timeperiod_arg1;
    st_arg_mq_receive_timeperiod_t receive_timeperiod_arg2;
    osal_milli_sec_t time_period_10ms = 10;
    osal_milli_sec_t time_period_1000ms = 1000;

    // Test03_006_009
    // @brief: abnormal case: multi-thread with one-queue
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);

    receive_timeperiod_arg1.mq_handle = shared_queue;
    receive_timeperiod_arg1.time = 2000;
    receive_timeperiod_arg1.sync_mq = sync_mq;
    receive_timeperiod_arg1.sync_send_msg = sync_send_msg;
    receive_timeperiod_arg1.needToSync = true;
    config1.func       = osal_test_thread_receive_timeperiod;
    config1.userarg    = (void *)&receive_timeperiod_arg1;
    config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config1.task_name  = "task03_006_009_01";
    config1.stack_size = THREAD_STACK_SIZE;

    receive_timeperiod_arg2.mq_handle = shared_queue;
    receive_timeperiod_arg2.time = 2000;
    receive_timeperiod_arg2.needToSync = false;
    config2.func       = osal_test_thread_receive_timeperiod;
    config2.userarg    = (void *)&receive_timeperiod_arg2;
    config2.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config2.task_name  = "task03_006_009_02";
    config2.stack_size = THREAD_STACK_SIZE;

    EXPECT_EQ(R_OSAL_ThreadCreate(&config1, TEST_THREAD_ID1, &thread_handle1),OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &sync_receive_msg, sync_mq_config.msg_size), OSAL_RETURN_OK);
    EXPECT_EQ(sync_receive_msg, sync_send_msg);
    EXPECT_EQ(R_OSAL_ThreadCreate(&config2, TEST_THREAD_ID2, &thread_handle2),OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &return_value1),OSAL_RETURN_OK);
    EXPECT_EQ(return_value1, OSAL_RETURN_OK) << "Test03_006_009 failed";
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle2, &return_value2),OSAL_RETURN_OK);
    EXPECT_EQ(return_value2, OSAL_RETURN_OK) << "Test03_006_009 failed";
}
// [MqReceiveForTimePeriod_TC_03_006_009]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveForTimePeriod_TC_03_006_010 MQApiTest.MqReceiveForTimePeriod_TC_03_006_010
 * @subsection MqReceiveForTimePeriod_TC_03_006_010_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveForTimePeriod_TC_03_006_010_tci Test Case ID
 * MqReceiveForTimePeriod_S2_A_TC_03_006_010
 * @subsection MqReceiveForTimePeriod_TC_03_006_010_rr Related Requirement(s)
 * \#199175, \#199181
 * @subsection MqReceiveForTimePeriod_TC_03_006_010_pp Prerequisite/Precondition
 * - OSAL is initialized and the shared_queue has been created by R_OSAL_MqCreate.
 * - The synchronization queue was createad
 * @subsection MqReceiveForTimePeriod_TC_03_006_010_tcs Test Case Summary
 * This test shall verify the functionality of MqReceiveForTimePeriod API in Abnormal behavior in multi thread
 * - Pre-process: create 2nd queue
 * - Test03_006_010: using multi thread with multi queue (empty queue).
 * - Post-process: delete 2nd queue
 * @subsection MqReceiveForTimePeriod_TC_03_006_010_tpts Test Procedure/Test Steps
 * -# Create another queue (call 2nd queue) with max_num_msg = 1 by calling R_OSAL_MqCreate.
 * -# Checking whether the existed queue (call 1st queue) is empty or not by function R_OSAL_MqIsEmpty.
 * -# Create 1st thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *          -# task_name is "task03_006_010_01" 
 *          -# func is osal_test_thread_receive_timeperiod function with functionality:
 *              - Get the synchronization queue config by using R_OSAL_MqGetConfig
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *              - Receive message from the queue by R_OSAL_MqReceiveForTimePeriod.
 *          -# userarg is receive_timeperiod_arg1 with:
 *              - mq_handle is 1st queue (shared_queue).
 *              - time is 2000ms
 *              - sync_mq is handle of synchronization queue
 *              - sync_send_msg is the sending message for synchronization purpose
 *              - needToSync is true
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1
 *      - Address of thread handle.
 * -# Create 2nd thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *          -# task_name is "task03_006_010_02" 
 *          -# func is osal_test_thread_receive_timeperiod function with functionality:
 *              - Get the synchronization queue config by using R_OSAL_MqGetConfig
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *              - Receive message from the queue by R_OSAL_MqReceiveForTimePeriod.
 *          -# userarg is receive_timeperiod_arg2 with:
 *              - mq_handle is 2nd queue.
 *              - time is 2000ms
 *              - sync_mq is handle of synchronization queue
 *              - sync_send_msg is the sending message for synchronization purpose
 *              - needToSync is true
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2
 *      - Address of thread handle.
 * -# Receive 2 messages from synchronization queue by using R_OSAL_MqReceiveForTimePeriod
 * -# Create 3rd thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *          -# task_name is "task03_006_010_03" 
 *          -# func is osal_test_thread_send_timeperiod function with functionality:
 *              - Send message to the queue by R_OSAL_MqSendForTimePeriod.
 *          -# userarg is send_timeperiod_arg with:
 *              - mq_handle is 1st queue (shared_queue).
 *              - time is 1000ms
 *              - p_buffer is send_msg (0xFFFF)
 *              - buffer_size is sizeof(send_msg)
 *              - needToSync is false
 *      - thead_id is TEST_THREAD_ID3
 *      - Address of thread handle.
 * -# Calling R_OSAL_ThreadJoin to join the 3rd thread.
 * -# Calling R_OSAL_ThreadJoin to join the 1st thread.
 * -# Calling R_OSAL_ThreadJoin to join the 2nd thread.
 * -# Delete the 2nd queue by R_OSAL_MqDelete.
 * @subsection MqReceiveForTimePeriod_TC_03_006_010_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK and the return of the 2nd queue handle is not NULL_PTR (along with existed queue - call 1st queue).
 * -# The return value of R_OSAL_MqIsEmpty is OSAL_RETURN_OK and the return of empty check is TRUE.
 * -# The return value of the 1st R_OSAL_ThreadCreate is OSAL_RETURN_OK.<br>
 * In function osal_test_thread_receive_timeperiod:<br>
 *      - R_OSAL_MqGetConfig return OSAL_RETURN_OK
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 *      - R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK
 * -# The return value of the 2nd R_OSAL_ThreadCreate is OSAL_RETURN_OK.<br>
 * In function osal_test_thread_receive_timeperiod:<br>
 *      - R_OSAL_MqGetConfig return OSAL_RETURN_OK
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 *      - R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of the 3rd R_OSAL_ThreadCreate is OSAL_RETURN_OK.<br>
 * In function osal_test_thread_send_timeperiod:<br>
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * -# The return value of the 3rd R_OSAL_ThreadJoin is OSAL_RETURN_OK.<br>
 * "return_value3" is OSAL_RETURN_OK
 * -# The return value of the 1st R_OSAL_ThreadJoin is OSAL_RETURN_OK.<br>
 * "return_value1" is OSAL_RETURN_OK
 * -# The return value of the 2nd R_OSAL_ThreadJoin is OSAL_RETURN_OK.<br>
 * "return_value2" is OSAL_RETURN_TIME
 * -# The return value of R_OSAL_MqDelete when delete 2nd queue is OSAL_RETURN_OK.
 * @subsection MqReceiveForTimePeriod_TC_03_006_010_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveForTimePeriod_TC_03_006_010
 * @snippet test_mq.cpp osal_test_thread_send_timeperiod
 * @snippet test_mq.cpp osal_test_thread_receive_timeperiod
 */
// [MqReceiveForTimePeriod_TC_03_006_010]
TEST_F(MQApiTest, MqReceiveForTimePeriod_TC_03_006_010)
{
    osal_thread_handle_t thread_handle3 = OSAL_THREAD_HANDLE_INVALID;
    osal_thread_handle_t thread_handle2 = OSAL_THREAD_HANDLE_INVALID;
    osal_thread_handle_t thread_handle1 = OSAL_THREAD_HANDLE_INVALID;
    st_osal_thread_config_t config3;
    st_osal_thread_config_t config2;
    st_osal_thread_config_t config1;
    int64_t return_value1 = 0;
    int64_t return_value2 = 0;
    int64_t return_value3 = 0;
    st_arg_mq_send_timeperiod_t send_timeperiod_arg;
    st_arg_mq_receive_timeperiod_t receive_timeperiod_arg1;
    st_arg_mq_receive_timeperiod_t receive_timeperiod_arg2;
    bool isEmpty = false;
    osal_milli_sec_t time_period_100ms = 100;

    //pre-process: create 2nd queue
    mq_config.max_num_msg = 1;
    mq_config.msg_size = 4;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config, TEST_MQ_ID2, &mq_handle), OSAL_RETURN_OK);
    EXPECT_NE(mq_handle, NULL_PTR);

    // Test03_006_010
    // @brief: abnormal case: multi-thread with multi-queue (empty)
    EXPECT_EQ(R_OSAL_MqIsEmpty(shared_queue, &isEmpty), OSAL_RETURN_OK);
    EXPECT_TRUE(isEmpty);
    
    receive_timeperiod_arg1.mq_handle = shared_queue;
    receive_timeperiod_arg1.time = 2000;
    receive_timeperiod_arg1.sync_mq = sync_mq;
    receive_timeperiod_arg1.sync_send_msg = sync_send_msg;
    receive_timeperiod_arg1.needToSync = true;
    config1.func       = osal_test_thread_receive_timeperiod;
    config1.userarg    = (void *)&receive_timeperiod_arg1;
    config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config1.task_name  = "task03_006_010_01";
    config1.stack_size = THREAD_STACK_SIZE;

    receive_timeperiod_arg2.mq_handle = mq_handle;
    receive_timeperiod_arg2.time = 2000;
    receive_timeperiod_arg2.sync_mq = sync_mq;
    receive_timeperiod_arg2.sync_send_msg = sync_send_msg;
    receive_timeperiod_arg2.needToSync = true;
    config2.func       = osal_test_thread_receive_timeperiod;
    config2.userarg    = (void *)&receive_timeperiod_arg2;
    config2.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config2.task_name  = "task03_006_010_02";
    config2.stack_size = THREAD_STACK_SIZE;

    send_timeperiod_arg.mq_handle = shared_queue;
    send_timeperiod_arg.time = 1000;
    send_timeperiod_arg.buffer_size = sizeof(send_msg);
    send_timeperiod_arg.p_buffer = &send_msg;
    send_timeperiod_arg.needToSync = false;
    config3.func       = osal_test_thread_send_timeperiod;
    config3.userarg    = (void *)&send_timeperiod_arg;
    config3.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config3.task_name  = "task03_006_010_03";
    config3.stack_size = THREAD_STACK_SIZE;

    EXPECT_EQ(R_OSAL_ThreadCreate(&config1, TEST_THREAD_ID1, &thread_handle1),OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThreadCreate(&config2, TEST_THREAD_ID2, &thread_handle2),OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &sync_receive_msg, sync_mq_config.msg_size), OSAL_RETURN_OK);
    EXPECT_EQ(sync_receive_msg, sync_send_msg);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &sync_receive_msg, sync_mq_config.msg_size), OSAL_RETURN_OK);
    EXPECT_EQ(sync_receive_msg, sync_send_msg);
    EXPECT_EQ(R_OSAL_ThreadCreate(&config3, TEST_THREAD_ID3, &thread_handle3),OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle3, &return_value3),OSAL_RETURN_OK);
    EXPECT_EQ(return_value3, OSAL_RETURN_OK) << "Test03_006_010 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &return_value1),OSAL_RETURN_OK);
    EXPECT_EQ(return_value1, OSAL_RETURN_OK) << "Test03_006_010 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle2, &return_value2),OSAL_RETURN_OK);
    EXPECT_EQ(return_value2, OSAL_RETURN_TIME) << "Test03_006_010 failed";

    //post-process: delete 2nd queue
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK);
}
// [MqReceiveForTimePeriod_TC_03_006_010]

/**
 * @page MQ_TEST MQ API Test
 * @section OsalApiStateTest_MqReceiveForTimePeriod_TC_03_006_011 OsalApiStateTest.MqReceiveForTimePeriod_TC_03_006_011
 * @subsection MqReceiveForTimePeriod_TC_03_006_011_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection MqReceiveForTimePeriod_TC_03_006_011_tci Test Case ID
 * MqReceiveForTimePeriod_S2_A_TC_03_006_011
 * @subsection MqReceiveForTimePeriod_TC_03_006_011_rr Related Requirement(s)
 * \#199175
 * @subsection MqReceiveForTimePeriod_TC_03_006_011_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection MqReceiveForTimePeriod_TC_03_006_011_tcs Test Case Summary
 * This test shall checking the state of all Message queue API in Abnormal behavior in single thread
 * - Test03_005_015: checking state of R_OSAL_MqReceiveForTimePeriod
 * @subsection MqReceiveForTimePeriod_TC_03_006_011_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_MqReceiveForTimePeriod with valid paramter.
 * @subsection MqReceiveForTimePeriod_TC_03_006_011_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_STATE.
 * @subsection MqReceiveForTimePeriod_TC_03_006_011_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveForTimePeriod_TC_03_006_011
 */
// [MqReceiveForTimePeriod_TC_03_006_011]
TEST_F(OsalApiStateTest, MqReceiveForTimePeriod_TC_03_006_011)
{
    osal_mq_handle_t mq_handle = OSAL_MQ_HANDLE_INVALID;
    osal_milli_sec_t time_period_10ms = 10;
    int receive_msg;
    size_t buffer_size = 4;

    // Test03_006_011
    // @brief: abnormal case: state transition error of R_OSAL_MqReceiveForTimePeriod
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(mq_handle,time_period_10ms, &receive_msg, buffer_size), OSAL_RETURN_STATE) << "Test03_006_011 failed";
}
// [MqReceiveForTimePeriod_TC_03_006_011]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveForTimePeriod_TC_03_006_012 MQApiTest.MqReceiveForTimePeriod_TC_03_006_012
 * @subsection MqReceiveForTimePeriod_TC_03_006_012_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveForTimePeriod_TC_03_006_012_tci Test Case ID
 * MqReceiveForTimePeriod_S3_N_TC_03_006_012
 * @subsection MqReceiveForTimePeriod_TC_03_006_012_rr Related Requirement(s)
 * \#199169, \#199175, \#199181
 * @subsection MqReceiveForTimePeriod_TC_03_006_012_pp Prerequisite/Precondition
 * - OSAL is initialized and the shared_queue has been created by R_OSAL_MqCreate.
 * - The synchronization queue was created
 * @subsection MqReceiveForTimePeriod_TC_03_006_012_tcs Test Case Summary
 * This test shall verify the functionality of MqReceiveForTimePeriod API in Normal behavior in multi thread
 * - Test03_006_012: Checking the FIFO of the queue.
 * @subsection MqReceiveForTimePeriod_TC_03_006_012_tpts Test Procedure/Test Steps
 * -# Create 1st thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *          -# task_name is "sub_thread1" 
 *          -# func is osal_test_mq1 function with functionality:
 *              - Get the synchronization queue config by using R_OSAL_MqGetConfig
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *              - Send 2 messages 0x1111 first and 0x2222 second to the queue by R_OSAL_MqSendForTimePeriod.
 *          -# userarg is send_timeperiod_arg with:
 *              - mq_handle is shared_queue
 *              - sync_mq is handle of synchronization queue
 *              - sync_send_msg is the sending message for synchronization purpose
 *              - needToSync is true
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1
 *      - Address of thread handle.
 * -# Receive message from synchronization queue by using R_OSAL_MqReceiveForTimePeriod
 * -# Create 2nd thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *          -# task_name is "sub_thread2" 
 *          -# func is osal_test_mq2 function with functionality:
 *              - Receive 2 messages from the queue by R_OSAL_MqReceiveForTimePeriod.
 *          -# userarg is shared_queue.
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2
 *      - Address of thread handle.
 * -# Calling R_OSAL_ThreadJoin to join the 1st thread.
 * -# Calling R_OSAL_ThreadJoin to join the 2nd thread.
 * @subsection MqReceiveForTimePeriod_TC_03_006_012_ereb Expected Result/Behavior
 * -# The return value of the 1st R_OSAL_ThreadCreate is OSAL_RETURN_OK.<br>
 * In function osal_test_mq1:<br>
 *      - R_OSAL_MqGetConfig return OSAL_RETURN_OK
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * -# The return value of R_OSAL_ThreadSleepForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of the 2nd R_OSAL_ThreadCreate is OSAL_RETURN_OK.<br>
 * In function osal_test_mq2:<br>
 *      - R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of the 1st R_OSAL_ThreadJoin is OSAL_RETURN_OK.<br>
 * "return_value1" is OSAL_RETURN_OK
 * -# The return value of the 2nd R_OSAL_ThreadJoin is OSAL_RETURN_OK.<br>
 * "return_value2" is OSAL_RETURN_OK
 * @subsection MqReceiveForTimePeriod_TC_03_006_012_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveForTimePeriod_TC_03_006_012
 * @snippet test_mq.cpp osal_test_mq1
 * @snippet test_mq.cpp osal_test_mq2
 */
// [MqReceiveForTimePeriod_TC_03_006_012]
TEST_F(MQApiTest, MqReceiveForTimePeriod_TC_03_006_012)
{
    osal_thread_handle_t thread_handle2 = OSAL_THREAD_HANDLE_INVALID;
    osal_thread_handle_t thread_handle1 = OSAL_THREAD_HANDLE_INVALID;
    st_osal_thread_config_t config2;
    st_osal_thread_config_t config1;
    int64_t return_value1 = 0;
    int64_t return_value2 = 0;
    st_arg_mq_send_timeperiod_t send_timeperiod_arg;

    // Test03_006_012
    // @brief: normal case: checking FIFO of the queue
    send_timeperiod_arg.mq_handle = shared_queue;
    send_timeperiod_arg.sync_mq = sync_mq;
    send_timeperiod_arg.sync_send_msg = sync_send_msg;
    send_timeperiod_arg.needToSync = true;
    config1.func       = osal_test_mq1;
    config1.userarg    = (void *)&send_timeperiod_arg;
    config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config1.task_name  = "sub_thread1";
    config1.stack_size = THREAD_STACK_SIZE;

    config2.func       = osal_test_mq2;
    config2.userarg    = (void *)&shared_queue;
    config2.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config2.task_name  = "sub_thread2";
    config2.stack_size = THREAD_STACK_SIZE;

    EXPECT_EQ(R_OSAL_ThreadCreate(&config1, TEST_THREAD_ID1, &thread_handle1),OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &sync_receive_msg, sync_mq_config.msg_size), OSAL_RETURN_OK);
    EXPECT_EQ(sync_receive_msg, sync_send_msg);
    EXPECT_EQ(R_OSAL_ThreadCreate(&config2, TEST_THREAD_ID2, &thread_handle2),OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &return_value1),OSAL_RETURN_OK);
    EXPECT_EQ(return_value1, 0) << "Test03_006_012 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle2, &return_value2),OSAL_RETURN_OK);
    EXPECT_EQ(return_value2, 0) << "Test03_006_012 failed";
}
// [MqReceiveForTimePeriod_TC_03_006_012]

/**
 * @page MQ_TEST MQ API Test
 * @section OSALINTTESTS_MqReceiveForTimePeriod_TC_03_006_013 OSALINTTESTS.MqReceiveForTimePeriod_TC_03_006_013
 * @subsection MqReceiveForTimePeriod_TC_03_006_013_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection MqReceiveForTimePeriod_TC_03_006_013_tci Test Case ID
 * MqReceiveForTimePeriod_S3_N_TC_03_006_013
 * @subsection MqReceiveForTimePeriod_TC_03_006_013_rr Related Requirement(s)
 * \#199175
 * @subsection MqReceiveForTimePeriod_TC_03_006_013_pp Prerequisite/Precondition
 * -# OSAL is initialized.<br>
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection MqReceiveForTimePeriod_TC_03_006_013_tcs Test Case Summary
 * @subsubsection MqReceiveForTimePeriod_TC_03_006_013_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Create a queue.
 * - Setup required memory for interrupt generation
 * - Register callack function and enable interrupt
 * - Wait for interrupt callback function finish
 * @subsubsection MqReceiveForTimePeriod_TC_03_006_013_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * - Reset created queue
 * - Destroy created queue
 * @subsubsection MqReceiveForTimePeriod_TC_03_006_013_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality with normal condition of the Message Queue APIs which are called in the ISR.
 * - Disable and clear interrupt status
 * - Test03_006_013: call R_OSAL_MqReceiveForTimePeriod in ISR
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection MqReceiveForTimePeriod_TC_03_006_013_tpts Test Procedure/Test Steps
 * @subsubsection MqReceiveForTimePeriod_TC_03_006_013_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Create a queue by calling function R_OSAL_MqCreate with bellow config
 *      - max_num_msg: 1
 *      - msg_size: 4
 * -# Allocate required memory for generate interrupt from hardware and get hw address of allocated memory.
 * -# Call R_OSAL_InterruptGetNumOfIrqChannels to get current number of interrupt channel.
 * -# Call R_OSAL_InterruptRegisterIsr to regsiter an interrupt with a callback function.
 * -# Calling R_OSAL_InterruptEnableIsr to enable interrupt.
 * -# Setup hardware specific register to generate the interrupt.
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection MqReceiveForTimePeriod_TC_03_006_013_post_tpts Test Procedure/Test Steps of Post-process
 * -# Calling R_OSAL_InterruptUnregisterIsr to unregister the interrupt.
 * -# Deallocate required memory
 * -# Calling reset queue by using R_OSAL_MqReset.
 * -# Delete the message queue by caliing R_OSAL_MqDelete.
 * @subsubsection MqReceiveForTimePeriod_TC_03_006_013_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Call R_OSAL_InterruptDisableIsr to disable interrupt signal.
 * -# Clear the status register by calling function R_OSAL_IoWrite32 with offset is 10 and value is TEST_CLEAR_REGISTER_VALUE.
 * -# Calling R_OSAL_MqSendForTimePeriod to send a message to queue with time period is 0.
 * -# Calling R_OSAL_MqReceiveForTimePeriod to receive a message to queue with time period is 0.
 * -# Calling R_OSAL_MqSendForTimePeriod for Send a message that the interrupt context has ended
 * @subsection MqReceiveForTimePeriod_TC_03_006_013_ereb Expected Result/Behavior
 * @subsubsection MqReceiveForTimePeriod_TC_03_006_013_pre_ereb Expected Result/Behavior of Pre-process
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK.
 * -# The allocation successfully without any error.
 * -# The return value of R_OSAL_InterruptGetNumOfIrqChannels is OSAL_RETURN_OK.<br>
 * The return number of channel is greater than 0.
 * -# The return value of R_OSAL_InterruptRegisterIsr is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_InterruptEnableIsr is OSAL_RETURN_OK.
 * -# Setup hardware specific register to generate the interrupt success without any error.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection MqReceiveForTimePeriod_TC_03_006_013_post_ereb Expected Result/Behavior of Post-process
 * -# The return value of R_OSAL_InterruptUnregisterIsr is OSAL_RETURN_OK.
 * -# Deallocate required memory success without any error.
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqDelete is OSAL_RETURN_OK.
 * @subsubsection MqReceiveForTimePeriod_TC_03_006_013_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# The return value of R_OSAL_InterruptDisableIsr is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_IoWrite32 is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK and the returned message is as expected.
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * @subsection MqReceiveForTimePeriod_TC_03_006_013_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveForTimePeriod_TC_03_006_013
 * @snippet test_mq.cpp mq_test_int_cb1_normal_TC_03_006_013
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [MqReceiveForTimePeriod_TC_03_006_013]
TEST_F(OSALINTTESTS, MqReceiveForTimePeriod_TC_03_006_013)
{
    osal_mq_handle_t mq_handle;
    size_t numOfChannel = 0;
    st_osal_mq_config mq_config1;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE10;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    st_callback_arg_t arg;
    osal_milli_sec_t time_period = 2000;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};

/* pre-process:
 * create a queue
 * Setup required memory for interrupt generation.
 * Register callack function and enable interrupt 
 * Wait for interrupt callback function finish */
    mq_config1.max_num_msg = ISR_MQ_NUM_OF_MSG;
    mq_config1.msg_size = ISR_MQ_MSG_SIZE;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config1, ISR_MQ_ID , &mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK);
    arg.additional_arg = (void *)&mq_handle;
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, mq_test_int_cb1_normal_TC_03_006_013, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory
 * Destroy created handle */
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, mq_test_int_cb1_normal_TC_03_006_013), OSAL_RETURN_OK);
    deallocate_data_memory(hndl_mmngr, buffer_obj);
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK);
}
// [MqReceiveForTimePeriod_TC_03_006_013]

/**
 * @page MQ_TEST MQ API Test
 * @section OSALINTTESTS_MqReceiveForTimePeriod_TC_03_006_014 OSALINTTESTS.MqReceiveForTimePeriod_TC_03_006_014
 * @subsection MqReceiveForTimePeriod_TC_03_006_014_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection MqReceiveForTimePeriod_TC_03_006_014_tci Test Case ID
 * MqReceiveForTimePeriod_S2_A_TC_03_006_014
 * @subsection MqReceiveForTimePeriod_TC_03_006_014_rr Related Requirement(s)
 * \#199175
 * @subsection MqReceiveForTimePeriod_TC_03_006_014_pp Prerequisite/Precondition
 * -# OSAL is initialized.<br>
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection MqReceiveForTimePeriod_TC_03_006_014_tcs Test Case Summary
 * @subsubsection MqReceiveForTimePeriod_TC_03_006_014_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 *      - Create a queue.
 *      - Setup required memory for interrupt generation
 *      - Register callack function and enable interrupt
 *      - Wait for interrupt callback function finish
 * @subsubsection MqReceiveForTimePeriod_TC_03_006_014_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 *      - Unregister interrupt callback function
 *      - Deallocate allocated memory
 *      - Reset created queue
 *      - Destroy created queue
 * @subsubsection MqReceiveForTimePeriod_TC_03_006_014_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality with Abnormal condition of the Message Queue APIs which are called in the ISR.
 * - Disable and clear interrupt status
 * - Test03_006_014: call R_OSAL_MqReceiveForTimePeriod in ISR with empty queue
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection MqReceiveForTimePeriod_TC_03_006_014_tpts Test Procedure/Test Steps
 * @subsubsection MqReceiveForTimePeriod_TC_03_006_014_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Create a queue by calling function R_OSAL_MqCreate with bellow config
 *      - max_num_msg: 1
 *      - msg_size: 4
 * -# Allocate required memory for generate interrupt from hardware and get hw address of allocated memory.
 * -# Call R_OSAL_InterruptGetNumOfIrqChannels to get current number of interrupt channel.
 * -# Call R_OSAL_InterruptRegisterIsr to regsiter an interrupt with a callback function.
 * -# Calling R_OSAL_InterruptEnableIsr to enable interrupt.
 * -# Setup hardware specific register to generate the interrupt.
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection MqReceiveForTimePeriod_TC_03_006_014_post_tpts Test Procedure/Test Steps of Post-process
 * -# Calling R_OSAL_InterruptUnregisterIsr to unregister the interrupt.
 * -# Deallocate required memory
 * -# Calling reset queue by using R_OSAL_MqReset.
 * -# Delete the message queue by caliing R_OSAL_MqDelete.
 * @subsubsection MqReceiveForTimePeriod_TC_03_006_014_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Call R_OSAL_InterruptDisableIsr to disable interrupt signal.
 * -# Clear the status register by calling function R_OSAL_IoWrite32 with offset is 10 and value is TEST_CLEAR_REGISTER_VALUE.
 * -# Calling R_OSAL_MqReceiveForTimePeriod to receive the message from the queue.
 * -# Calling R_OSAL_MqSendForTimePeriod for Send a message that the interrupt context has ended
 * @subsection MqReceiveForTimePeriod_TC_03_006_014_ereb Expected Result/Behavior
 * @subsubsection MqReceiveForTimePeriod_TC_03_006_014_pre_ereb Expected Result/Behavior of Pre-process
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK.
 * -# The allocation successfully without any error.
 * -# The return value of R_OSAL_InterruptGetNumOfIrqChannels is OSAL_RETURN_OK.<br>
 * The return number of channel is greater than 0.
 * -# The return value of R_OSAL_InterruptRegisterIsr is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_InterruptEnableIsr is OSAL_RETURN_OK.
 * -# Setup hardware specific register to generate the interrupt success without any error.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection MqReceiveForTimePeriod_TC_03_006_014_post_ereb Expected Result/Behavior of Post-process
 * -# The return value of R_OSAL_InterruptUnregisterIsr is OSAL_RETURN_OK.
 * -# Deallocate required memory success without any error.
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqDelete is OSAL_RETURN_OK.
 * @subsubsection MqReceiveForTimePeriod_TC_03_006_014_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# The return value of R_OSAL_InterruptDisableIsr is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_IoWrite32 is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_BUSY.
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * @subsection MqReceiveForTimePeriod_TC_03_006_014_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveForTimePeriod_TC_03_006_014
 * @snippet test_mq.cpp mq_test_int_cb1_abnormal_TC_03_006_014
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [MqReceiveForTimePeriod_TC_03_006_014]
TEST_F(OSALINTTESTS, MqReceiveForTimePeriod_TC_03_006_014)
{
    osal_mq_handle_t mq_handle;
    size_t numOfChannel = 0;
    st_osal_mq_config mq_config1;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE10;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    st_callback_arg_t arg;
    osal_milli_sec_t time_period = 2000;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};

/* pre-process:
 * create a queue
 * Setup required memory for interrupt generation.
 * Register callack function and enable interrupt 
 * Wait for interrupt callback function finish */
    mq_config1.max_num_msg = ISR_MQ_NUM_OF_MSG;
    mq_config1.msg_size = ISR_MQ_MSG_SIZE;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config1, ISR_MQ_ID , &mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK);
    arg.additional_arg = (void *)&mq_handle;
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, mq_test_int_cb1_abnormal_TC_03_006_014, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory
 * Destroy created handle */
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, mq_test_int_cb1_abnormal_TC_03_006_014), OSAL_RETURN_OK);
    deallocate_data_memory(hndl_mmngr, buffer_obj);
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK);
}
// [MqReceiveForTimePeriod_TC_03_006_014]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveForTimePeriod_TC_03_006_015 MQApiTest.MqReceiveForTimePeriod_TC_03_006_015
 * @subsection MqReceiveForTimePeriod_TC_03_006_015_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveForTimePeriod_TC_03_006_015_tci Test Case ID
 * MqReceiveForTimePeriod_S0_BA_TC_03_006_015
 * @subsection MqReceiveForTimePeriod_TC_03_006_015_rr Related Requirement(s)
 * \#199175
 * @subsection MqReceiveForTimePeriod_TC_03_006_015_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqReceiveForTimePeriod_TC_03_006_015_tcs Test Case Summary
 * This test shall verify the functionality of MqReceiveForTimePeriod API in Abnormal behavior in single thread
 * - Test03_006_015: receive message from queue with buffer_size parameter is bigger than message size
 * @subsection MqReceiveForTimePeriod_TC_03_006_015_tpts Test Procedure/Test Steps
 * -# Get the mq config by using R_OSAL_MqGetConfig.<br>
 * Increase "mq_config.msg_size" by 1
 * -# Receive the message from the queue by calling R_OSAL_MqReceiveForTimePeriod. with the "mq_config.msg_size" as above
 * @subsection MqReceiveForTimePeriod_TC_03_006_015_ereb Expected Result/Behavior
 * -# R_OSAL_MqGetConfig return OSAL_RETURN_OK
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_PAR
 * @subsection MqReceiveForTimePeriod_TC_03_006_015_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveForTimePeriod_TC_03_006_015
 */
// [MqReceiveForTimePeriod_TC_03_006_015]
TEST_F(MQApiTest, MqReceiveForTimePeriod_TC_03_006_015)
{
    osal_milli_sec_t time_period_10ms = 10;

    // Test03_006_015
    // @brief: abnormal case: message size is bigger than mq config message size
    EXPECT_EQ(R_OSAL_MqGetConfig(shared_queue, &mq_config), OSAL_RETURN_OK) << "Test03_006_015 failed";
    mq_config.msg_size += 1;
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(shared_queue, time_period_10ms, &receive_msg, mq_config.msg_size), OSAL_RETURN_PAR) << "Test03_006_015 failed";
}
// [MqReceiveForTimePeriod_TC_03_006_015]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveForTimePeriod_TC_03_006_016 MQApiTest.MqReceiveForTimePeriod_TC_03_006_016
 * @subsection MqReceiveForTimePeriod_TC_03_006_016_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveForTimePeriod_TC_03_006_016_tci Test Case ID
 * MqReceiveForTimePeriod_S0_BA_TC_03_006_016
 * @subsection MqReceiveForTimePeriod_TC_03_006_016_rr Related Requirement(s)
 * \#199175
 * @subsection MqReceiveForTimePeriod_TC_03_006_016_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqReceiveForTimePeriod_TC_03_006_016_tcs Test Case Summary
 * This test shall verify the functionality of MqReceiveForTimePeriod API in Abnormal behavior in single thread
 * - Test03_006_016: receive message from queue with buffer_size parameter is 0
 * @subsection MqReceiveForTimePeriod_TC_03_006_016_tpts Test Procedure/Test Steps
 * -# Receive the message from the queue by calling R_OSAL_MqReceiveForTimePeriod. with the message size is 0
 * @subsection MqReceiveForTimePeriod_TC_03_006_016_ereb Expected Result/Behavior
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_PAR
 * @subsection MqReceiveForTimePeriod_TC_03_006_016_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveForTimePeriod_TC_03_006_016
 */
// [MqReceiveForTimePeriod_TC_03_006_016]
TEST_F(MQApiTest, MqReceiveForTimePeriod_TC_03_006_016)
{
    osal_milli_sec_t time_period_10ms = 10;
    size_t buffer_size;

    // Test03_006_016
    // @brief: abnormal case: message size is 0
    buffer_size = 0;
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(shared_queue, time_period_10ms, &receive_msg, buffer_size), OSAL_RETURN_PAR) << "Test03_006_016 failed";
}
// [MqReceiveForTimePeriod_TC_03_006_016]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveForTimePeriod_TC_03_006_017 MQApiTest.MqReceiveForTimePeriod_TC_03_006_017
 * @subsection MqReceiveForTimePeriod_TC_03_006_017_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveForTimePeriod_TC_03_006_017_tci Test Case ID
 * MqReceiveForTimePeriod_S1_A_TC_03_006_017
 * @subsection MqReceiveForTimePeriod_TC_03_006_017_rr Related Requirement(s)
 * \#199175
 * @subsection MqReceiveForTimePeriod_TC_03_006_017_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqReceiveForTimePeriod_TC_03_006_017_tcs Test Case Summary
 * This test shall measure the time execution of R_OSAL_MqReceiveForTimePeriod with normal condition:
 * - Pre-process: make the "shared_queue" to be empty
 * - Test03_006_017: measure the time execution of R_OSAL_MqReceiveForTimePeriod when receiving a message with timeout is 10ms
 * @subsection MqReceiveForTimePeriod_TC_03_006_017_tpts Test Procedure/Test Steps
 * -# Reset the "shared_queue" by using R_OSAL_MqReset
 * -# Get the config of "shared_queue" by using R_OSAL_MqGetConfig
 * -# Use the "for" loop in which we repeat 101 times with 4 steps as below:
 *      - Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *      - Receive message by R_OSAL_MqReceiveForTimePeriod with the timeout is 10ms
 *      - Get the current time stamp again by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *      - Get the execution time of R_OSAL_MqSendForTimePeriod by calculating the different time using R_OSAL_ClockTimeCalculateTimeDifference.<br>
 *        Execution time is stored in an array "diff_time"
 * -# Find the median of "diff_time" array.<br>
 * Compare it with 10ms plus "delta_time" (TEST_MEDIAN_EXPECT)
 * @subsection MqReceiveForTimePeriod_TC_03_006_017_ereb Expected Result/Behavior
 * -# R_OSAL_MqReset return OSAL_RETURN_OK
 * -# R_OSAL_MqGetConfig return OSAL_RETURN_OK
 * -# The expected result of 4 steps in "for" loop are as below:
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 *      - R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_TIME.
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 *      - R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK.
 * -# Getting the median of "diff_time" array successfully.<br>
 * Median value need to be not exceed 10ms plus with "delta_time"
 * @subsection MqReceiveForTimePeriod_TC_03_006_017_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveForTimePeriod_TC_03_006_017
 * @snippet test_common.hpp findMedian
 */
// [MqReceiveForTimePeriod_TC_03_006_017]
TEST_F(MQApiTest, MqReceiveForTimePeriod_TC_03_006_017)
{
    osal_milli_sec_t time_period_10ms = 10;
    st_osal_time_t current_time_1st, current_time_2nd;
    osal_nano_sec_t diff_time[101];
    osal_nano_sec_t delta_time = TEST_MEDIAN_EXPECT;
    osal_nano_sec_t median = 0;
    int elements = 0;

    // pre-process: make the shared_queue empty
    EXPECT_EQ(R_OSAL_MqReset(shared_queue), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_MqGetConfig(shared_queue, &mq_config), OSAL_RETURN_OK) << "pre-process failed";

    // Test03_006_017
    // @brief: normal case: time_period = 10
    for(int i = 0; i <= 100; i++)
    {
        EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_1st), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(shared_queue, time_period_10ms,  &receive_msg, mq_config.msg_size), OSAL_RETURN_TIME) << "Test03_006_017 failed";
        EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_2nd), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &diff_time[i]), OSAL_RETURN_OK);
    }
    elements = sizeof(diff_time) / sizeof(diff_time[0]);
    median = (osal_nano_sec_t)findMedian(diff_time, elements);
    delta_time = TEST_MEDIAN_EXPECT;
    EXPECT_LE(median, time_period_10ms * MSEC_TO_NSEC + delta_time) << "Test03_006_017 failed";
    receive_msg = 0;
}
// [MqReceiveForTimePeriod_TC_03_006_017]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReceiveForTimePeriod_TC_03_006_018 MQApiTest.MqReceiveForTimePeriod_TC_03_006_018
 * @subsection MqReceiveForTimePeriod_TC_03_006_018_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReceiveForTimePeriod_TC_03_006_018_tci Test Case ID
 * MqReceiveForTimePeriod_S1_A_TC_03_006_018
 * @subsection MqReceiveForTimePeriod_TC_03_006_018_rr Related Requirement(s)
 * \#199175
 * @subsection MqReceiveForTimePeriod_TC_03_006_018_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqReceiveForTimePeriod_TC_03_006_018_tcs Test Case Summary
 * This test shall measure the time execution of R_OSAL_MqReceiveForTimePeriod with normal condition:
 * - Pre-process: make the "shared_queue" to be empty
 * - Test03_006_018: measure the time execution of R_OSAL_MqReceiveForTimePeriod when receiving a message with timeout is 0ms
 * @subsection MqReceiveForTimePeriod_TC_03_006_018_tpts Test Procedure/Test Steps
 * -# Reset the "shared_queue" by using R_OSAL_MqReset
 * -# Get the config of "shared_queue" by using R_OSAL_MqGetConfig
 * -# Use the "for" loop in which we repeat 101 times with 4 steps as below:
 *      - Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *      - Receive message by R_OSAL_MqReceiveForTimePeriod with the timeout is 0ms
 *      - Get the current time stamp again by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *      - Get the execution time of R_OSAL_MqSendForTimePeriod by calculating the different time using R_OSAL_ClockTimeCalculateTimeDifference.<br>
 *        Execution time is stored in an array "diff_time"
 * -# Find the median of "diff_time" array.<br>
 * Compare it with "delta_time" (TEST_MEDIAN_EXPECT)
 * -# Use the "for" loop in which we repeat 101 times with 5 steps as below:
 *      - Send a message to "shared_queue" by using R_OSAL_MqSendForTimePeriod
 *      - Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *      - Receive message by R_OSAL_MqReceiveForTimePeriod with the timeout is 0ms
 *      - Get the current time stamp again by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *      - Get the execution time of R_OSAL_MqSendForTimePeriod by calculating the different time using R_OSAL_ClockTimeCalculateTimeDifference.<br>
 *        Execution time is stored in an array "diff_time"
 * -# Find the median of "diff_time" array.<br>
 * Compare it with "delta_time" (typically 100usec)
 * @subsection MqReceiveForTimePeriod_TC_03_006_018_ereb Expected Result/Behavior
 * -# R_OSAL_MqReset return OSAL_RETURN_OK
 * -# R_OSAL_MqGetConfig return OSAL_RETURN_OK
 * -# The expected result of 4 steps in "for" loop are as below:
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 *      - R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_TIME.
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 *      - R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK.
 * -# Getting the median of "diff_time" array successfully.<br>
 * Median value need to be not exceed "delta_time"
 * -# The expected result of 5 steps in "for" loop are as below:
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK.
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 *      - R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK.
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 *      - R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK.
 * -# Getting the median of "diff_time" array successfully.<br>
 * Median value need to be not exceed "delta_time"
 * @subsection MqReceiveForTimePeriod_TC_03_006_018_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveForTimePeriod_TC_03_006_018
 * @snippet test_common.hpp findMedian
 */
// [MqReceiveForTimePeriod_TC_03_006_018]
TEST_F(MQApiTest, MqReceiveForTimePeriod_TC_03_006_018)
{
    osal_milli_sec_t time_period_10ms = 10;
    osal_milli_sec_t time_period_0ms = 0;
    st_osal_time_t current_time_1st, current_time_2nd;
    osal_nano_sec_t diff_time[101];
    osal_nano_sec_t delta_time = TEST_MEDIAN_EXPECT;
    osal_nano_sec_t median = 0;
    int elements = 0;

    // pre-process: make the shared_queue empty
    EXPECT_EQ(R_OSAL_MqReset(shared_queue), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_MqGetConfig(shared_queue, &mq_config), OSAL_RETURN_OK) << "pre-process failed";

    // Test03_006_018
    // @brief: normal case: time_period = 0
    for(int i = 0; i <= 100; i++)
    {
        EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_1st), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(shared_queue, time_period_0ms,  &receive_msg, mq_config.msg_size), OSAL_RETURN_TIME) << "Test03_006_018 failed";
        EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_2nd), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &diff_time[i]), OSAL_RETURN_OK);
    }

    elements = sizeof(diff_time) / sizeof(diff_time[0]);
    median = (osal_nano_sec_t)findMedian(diff_time, elements);
    delta_time = TEST_MEDIAN_EXPECT;
    EXPECT_LE(median, delta_time) << "Test03_004_018 failed";
    receive_msg = 0;

    for(int i = 0; i <= 100; i++)
    {
        EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue, time_period_10ms, &send_msg, mq_config.msg_size), OSAL_RETURN_OK) << "Test03_006_018 failed";
        EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_1st), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(shared_queue, time_period_0ms,  &receive_msg, mq_config.msg_size), OSAL_RETURN_OK) << "Test03_006_018 failed";
        EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_2nd), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &diff_time[i]), OSAL_RETURN_OK);
    }

    elements = sizeof(diff_time) / sizeof(diff_time[0]);
    median = (osal_nano_sec_t)findMedian(diff_time, elements);
    delta_time = 100000; //100usec
    EXPECT_LE(median, delta_time) << "Test03_004_018 failed";
    receive_msg = 0;
}
// [MqReceiveForTimePeriod_TC_03_006_018]

/**
 * @page MQ_TEST MQ API Test
 * @section OSALINTTESTS_MqReceiveForTimePeriod_TC_03_006_020 OSALINTTESTS.MqReceiveForTimePeriod_TC_03_006_020
 * @subsection MqReceiveForTimePeriod_TC_03_006_020_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection MqReceiveForTimePeriod_TC_03_006_020_tci Test Case ID
 * MqReceiveForTimePeriod_S2_A_TC_03_006_020
 * @subsection MqReceiveForTimePeriod_TC_03_006_020_rr Related Requirement(s)
 * \#199175
 * @subsection MqReceiveForTimePeriod_TC_03_006_020_pp Prerequisite/Precondition
 * -# OSAL is initialized.<br>
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection MqReceiveForTimePeriod_TC_03_006_020_tcs Test Case Summary
 * @subsubsection MqReceiveForTimePeriod_TC_03_006_020_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 *      - Create a queue and make this queue to be full
 *      - Create a waiting thread. This thread is blocked by the above queue
 *      - Setup required memory for interrupt generation
 *      - Register callack function and enable interrupt
 *      - Wait for interrupt callback function finish
 * @subsubsection MqReceiveForTimePeriod_TC_03_006_020_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 *      - Unregister interrupt callback function
 *      - Deallocate allocated memory
 *      - Reset created queue
 *      - Destroy created queue
 *      - Join the waiting thread
 * @subsubsection MqReceiveForTimePeriod_TC_03_006_020_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality with Abnormal condition of the Message Queue APIs which are called in the ISR.
 * - Disable and clear interrupt status
 * - Test03_006_020: call R_OSAL_MqReceiveForTimePeriod in ISR with full queue
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection MqReceiveForTimePeriod_TC_03_006_020_tpts Test Procedure/Test Steps
 * @subsubsection MqReceiveForTimePeriod_TC_03_006_020_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Create a queue by calling function R_OSAL_MqCreate with bellow config
 *      - max_num_msg: 1
 *      - msg_size: 4
 * -# Send a message to this queue to make it full by using R_OSAL_MqSendForTimePeriod
 * -# Create waiting thread by using R_OSAL_ThreadCreate. Make this thread blocked by above queue by sending another message to this queue.
 * -# Allocate required memory for generate interrupt from hardware and get hw address of allocated memory.
 * -# Call R_OSAL_InterruptGetNumOfIrqChannels to get current number of interrupt channel.
 * -# Call R_OSAL_InterruptRegisterIsr to regsiter an interrupt with a callback function.
 * -# Calling R_OSAL_InterruptEnableIsr to enable interrupt.
 * -# Setup hardware specific register to generate the interrupt.
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection MqReceiveForTimePeriod_TC_03_006_020_post_tpts Test Procedure/Test Steps of Post-process
 * -# Calling R_OSAL_InterruptUnregisterIsr to unregister the interrupt.
 * -# Deallocate required memory
 * -# Calling reset queue by using R_OSAL_MqReset.
 * -# Delete the message queue by caliing R_OSAL_MqDelete.
 * -# Join thread by using R_OSAL_ThreadJoin
 * @subsubsection MqReceiveForTimePeriod_TC_03_006_020_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Call R_OSAL_InterruptDisableIsr to disable interrupt signal.
 * -# Clear the status register by calling function R_OSAL_IoWrite32 with offset is 10 and value is TEST_CLEAR_REGISTER_VALUE.
 * -# Calling R_OSAL_MqReceiveForTimePeriod to receive the message from the queue.
 * -# Calling R_OSAL_MqSendForTimePeriod for Send a message that the interrupt context has ended
 * @subsection MqReceiveForTimePeriod_TC_03_006_020_ereb Expected Result/Behavior
 * @subsubsection MqReceiveForTimePeriod_TC_03_006_020_pre_ereb Expected Result/Behavior of Pre-process
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_ThreadCreate is OSAL_RETURN_OK.
 * -# The allocation successfully without any error.
 * -# The return value of R_OSAL_InterruptGetNumOfIrqChannels is OSAL_RETURN_OK.<br>
 * The return number of channel is greater than 0.
 * -# The return value of R_OSAL_InterruptRegisterIsr is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_InterruptEnableIsr is OSAL_RETURN_OK.
 * -# Setup hardware specific register to generate the interrupt success without any error.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection MqReceiveForTimePeriod_TC_03_006_020_post_ereb Expected Result/Behavior of Post-process
 * -# The return value of R_OSAL_InterruptUnregisterIsr is OSAL_RETURN_OK.
 * -# Deallocate required memory success without any error.
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqDelete is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_ThreadJoin is OSAL_RETURN_OK.
 * @subsubsection MqReceiveForTimePeriod_TC_03_006_020_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# The return value of R_OSAL_InterruptDisableIsr is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_IoWrite32 is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_BUSY.
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * @subsection MqReceiveForTimePeriod_TC_03_006_020_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveForTimePeriod_TC_03_006_020
 * @snippet test_mq.cpp mq_test_int_cb_TC_03_006_020
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [MqReceiveForTimePeriod_TC_03_006_020]
TEST_F(OSALINTTESTS, MqReceiveForTimePeriod_TC_03_006_020)
{
    osal_mq_handle_t mq_handle;
    size_t numOfChannel = 0;
    st_osal_mq_config mq_config1;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE10;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    st_callback_arg_t arg;
    osal_milli_sec_t time_period = 2000;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};
    osal_milli_sec_t time_period_10ms = 10;
    st_osal_thread_config_t config;
    osal_thread_handle_t thread_handle;
    st_arg_mq_send_timeperiod_t send_timeperiod_arg;
    int64_t return_value;

/* pre-process:
 * create a queue
 * Setup required memory for interrupt generation.
 * Register callack function and enable interrupt
 * Wait for interrupt callback function finish */
    mq_config1.max_num_msg = ISR_MQ_NUM_OF_MSG;
    mq_config1.msg_size = ISR_MQ_MSG_SIZE;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config1, ISR_MQ_ID , &mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(mq_handle, time_period_10ms, &send_msg, mq_config1.msg_size), OSAL_RETURN_OK);

    send_timeperiod_arg.mq_handle = mq_handle;
    send_timeperiod_arg.time = 5000;
    send_timeperiod_arg.p_buffer =  &send_msg;
    send_timeperiod_arg.buffer_size = mq_config1.msg_size;
    send_timeperiod_arg.needToSync = false;

    config.func       = osal_test_thread_send_timeperiod;
    config.userarg    = (void *)&send_timeperiod_arg;
    config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config.task_name  = "task03_006_020";
    config.stack_size = THREAD_STACK_SIZE;
    EXPECT_EQ(R_OSAL_ThreadCreate(&config, TEST_THREAD_ID1, &thread_handle),OSAL_RETURN_OK);

    arg.additional_arg = (void *)&mq_handle;
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, mq_test_int_cb_TC_03_006_020, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory
 * Destroy created handle */
    EXPECT_EQ(R_OSAL_ThreadSleepForTimePeriod(100), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, mq_test_int_cb_TC_03_006_020), OSAL_RETURN_OK);
    deallocate_data_memory(hndl_mmngr, buffer_obj);
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &return_value), OSAL_RETURN_OK);
    EXPECT_EQ(return_value, OSAL_RETURN_OK);
}
// [MqReceiveForTimePeriod_TC_03_006_020]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqIsFull_TC_03_007_001 MQApiTest.MqIsFull_TC_03_007_001
 * @subsection MqIsFull_TC_03_007_001_tsi Test Suite ID
 * MQApiTest
 * @subsection MqIsFull_TC_03_007_001_tci Test Case ID
 * MqIsFull_S1_N_TC_03_007_001
 * @subsection MqIsFull_TC_03_007_001_rr Related Requirement(s)
 * \#199178
 * @subsection MqIsFull_TC_03_007_001_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqIsFull_TC_03_007_001_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqIsFull API in Normal behavior in single thread
 * - Test03_007_001: with queue is empty.
 * @subsection MqIsFull_TC_03_007_001_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_MqIsFull with valid paramter.
 * @subsection MqIsFull_TC_03_007_001_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqIsFull is OSAL_RETURN_OK and the return result of checking full is false.
 * @subsection MqIsFull_TC_03_007_001_testcase Test Case Example
 * @snippet test_mq.cpp MqIsFull_TC_03_007_001
 */
// [MqIsFull_TC_03_007_001]
TEST_F(MQApiTest, MqIsFull_TC_03_007_001)
{
    bool isFull = false;

    // Test03_007_001
    // @brief: normal case: call Queue is empty
    EXPECT_EQ(R_OSAL_MqIsFull(shared_queue, &isFull), OSAL_RETURN_OK) << "Test03_007_001 failed";
    EXPECT_EQ(isFull, false);
}
// [MqIsFull_TC_03_007_001]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqIsFull_TC_03_007_002 MQApiTest.MqIsFull_TC_03_007_002
 * @subsection MqIsFull_TC_03_007_002_tsi Test Suite ID
 * MQApiTest
 * @subsection MqIsFull_TC_03_007_002_tci Test Case ID
 * MqIsFull_S1_N_TC_03_007_002
 * @subsection MqIsFull_TC_03_007_002_rr Related Requirement(s)
 * \#199178
 * @subsection MqIsFull_TC_03_007_002_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqIsFull_TC_03_007_002_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqIsFull API in Normal behavior in single thread
 * - Test03_007_002: Send 2 message to queue by using R_OSAL_MqSendForTimePeriod and check the queue is full or not
 * @subsection MqIsFull_TC_03_007_002_tpts Test Procedure/Test Steps
 * -# Send 2 message to the queue by calling function R_OSAL_MqSendForTimePeriod 2 times.
 * -# Call function R_OSAL_MqIsFull with valid parameter.
 * -# Call function R_OSAL_MqReset to reset the queue.
 * @subsection MqIsFull_TC_03_007_002_ereb Expected Result/Behavior
 * -# The return value of 2 times call function R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqIsFull is OSAL_RETURN_OK and the return result of checking full is true.
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_OK.
 * @subsection MqIsFull_TC_03_007_002_testcase Test Case Example
 * @snippet test_mq.cpp MqIsFull_TC_03_007_002
 */
// [MqIsFull_TC_03_007_002]
TEST_F(MQApiTest, MqIsFull_TC_03_007_002)
{
    bool isFull = false;
    osal_milli_sec_t time_period_10ms = 10;

    // Test03_007_002
    // @brief: normal case: call Queue is full
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqIsFull(shared_queue, &isFull), OSAL_RETURN_OK) << "Test03_007_002 failed";
    EXPECT_EQ(isFull, true);
    EXPECT_EQ(R_OSAL_MqReset(shared_queue), OSAL_RETURN_OK); //reset shared queue for the next test case
    isFull = false;
}
// [MqIsFull_TC_03_007_002]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqIsFull_TC_03_007_003 MQApiTest.MqIsFull_TC_03_007_003
 * @subsection MqIsFull_TC_03_007_003_tsi Test Suite ID
 * MQApiTest
 * @subsection MqIsFull_TC_03_007_003_tci Test Case ID
 * MqIsFull_S0_A_TC_03_007_003
 * @subsection MqIsFull_TC_03_007_003_rr Related Requirement(s)
 * \#199178
 * @subsection MqIsFull_TC_03_007_003_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqIsFull_TC_03_007_003_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqIsFull API in Abnormal behavior in single thread
 * - Test03_007_003: with p_result parameter is NULL.
 * @subsection MqIsFull_TC_03_007_003_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_MqIsFull with p_result is NULL .
 * @subsection MqIsFull_TC_03_007_003_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqIsFull is OSAL_RETURN_PAR.
 * @subsection MqIsFull_TC_03_007_003_testcase Test Case Example
 * @snippet test_mq.cpp MqIsFull_TC_03_007_003
 */
// [MqIsFull_TC_03_007_003]
TEST_F(MQApiTest, MqIsFull_TC_03_007_003)
{
    bool * p_result = NULL;

    // Test03_007_003
    // @brief: abnormal case: p_result is NULL
    EXPECT_EQ(R_OSAL_MqIsFull(shared_queue, p_result), OSAL_RETURN_PAR) << "Test03_007_003 failed";
}
// [MqIsFull_TC_03_007_003]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqIsFull_TC_03_007_004 MQApiTest.MqIsFull_TC_03_007_004
 * @subsection MqIsFull_TC_03_007_004_tsi Test Suite ID
 * MQApiTest
 * @subsection MqIsFull_TC_03_007_004_tci Test Case ID
 * MqIsFull_S0_A_TC_03_007_004
 * @subsection MqIsFull_TC_03_007_004_rr Related Requirement(s)
 * \#199151, \#199165, \#199178
 * @subsection MqIsFull_TC_03_007_004_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqIsFull_TC_03_007_004_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqIsFull API in Abnormal behavior in single thread
 * - Test03_007_004: mq_handle parameter is NULL
 * @subsection MqIsFull_TC_03_007_004_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_MqIsFull with mq_handle is NULL.
 * @subsection MqIsFull_TC_03_007_004_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqIsFull is OSAL_RETURN_HANDLE.
 * @subsection MqIsFull_TC_03_007_004_testcase Test Case Example
 * @snippet test_mq.cpp MqIsFull_TC_03_007_004
 */
// [MqIsFull_TC_03_007_004]
TEST_F(MQApiTest, MqIsFull_TC_03_007_004)
{
    bool isFull = false;

    // Test03_007_004
    // @brief: abnormal case: handle is NULL
    EXPECT_EQ(R_OSAL_MqIsFull(OSAL_MQ_HANDLE_INVALID, &isFull), OSAL_RETURN_HANDLE) << "Test03_007_004 failed";
}
// [MqIsFull_TC_03_007_004]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqIsFull_TC_03_007_005 MQApiTest.MqIsFull_TC_03_007_005
 * @subsection MqIsFull_TC_03_007_005_tsi Test Suite ID
 * MQApiTest
 * @subsection MqIsFull_TC_03_007_005_tci Test Case ID
 * MqIsFull_S2_A_TC_03_007_005
 * @subsection MqIsFull_TC_03_007_005_rr Related Requirement(s)
 * \#199178
 * @subsection MqIsFull_TC_03_007_005_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqIsFull_TC_03_007_005_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqIsFull API in Abnormal behavior in single thread
 * - Test03_007_005: with mq_handle has been deleted.
 * @subsection MqIsFull_TC_03_007_005_tpts Test Procedure/Test Steps
 * -# Create a new message queue by calling R_OSAL_MqCreate.
 * -# Delete created queue by calling R_OSAL_MqDelete.
 * -# Call function R_OSAL_MqIsFull.
 * @subsection MqIsFull_TC_03_007_005_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK and the return handle is not NULL_PTR.
 * -# The return value of R_OSAL_MqDelete is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqIsFull is OSAL_RETURN_HANDLE.
 * @subsection MqIsFull_TC_03_007_005_testcase Test Case Example
 * @snippet test_mq.cpp MqIsFull_TC_03_007_005
 */
// [MqIsFull_TC_03_007_005]
TEST_F(MQApiTest, MqIsFull_TC_03_007_005)
{
    bool isFull = false;

    // Test03_007_005
    // @brief: abnormal case: handle is deleted
    mq_config.max_num_msg = 1;
    mq_config.msg_size = 4;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config, TEST_MQ_ID2, &mq_handle), OSAL_RETURN_OK);
    EXPECT_NE(mq_handle, NULL_PTR);
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqIsFull(mq_handle, &isFull), OSAL_RETURN_HANDLE) << "Test03_007_005 failed";
}
// [MqIsFull_TC_03_007_005]

/**
 * @page MQ_TEST MQ API Test
 * @section OsalApiStateTest_MqIsFull_TC_03_007_006 OsalApiStateTest.MqIsFull_TC_03_007_006
 * @subsection MqIsFull_TC_03_007_006_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection MqIsFull_TC_03_007_006_tci Test Case ID
 * MqIsFull_S2_A_TC_03_007_006
 * @subsection MqIsFull_TC_03_007_006_rr Related Requirement(s)
 * \#199178
 * @subsection MqIsFull_TC_03_007_006_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection MqIsFull_TC_03_007_006_tcs Test Case Summary
 * This test shall checking the state of all Message queue API in Abnormal behavior in single thread
 * - Test03_007_006: checking state of R_OSAL_MqIsFull
 * @subsection MqIsFull_TC_03_007_006_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_MqIsFull with valid paramter.
 * @subsection MqIsFull_TC_03_007_006_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqIsFull is OSAL_RETURN_STATE.
 * @subsection MqIsFull_TC_03_007_006_testcase Test Case Example
 * @snippet test_mq.cpp MqIsFull_TC_03_007_006
 */
// [MqIsFull_TC_03_007_006]
TEST_F(OsalApiStateTest, MqIsFull_TC_03_007_006)
{
    osal_mq_handle_t mq_handle = OSAL_MQ_HANDLE_INVALID;
    bool IsFull;

    // Test03_007_006
    // @brief: abnormal case: state transition error of R_OSAL_MqIsFull
    EXPECT_EQ(R_OSAL_MqIsFull(mq_handle, &IsFull), OSAL_RETURN_STATE) << "Test03_007_006 failed";
}
// [MqIsFull_TC_03_007_006]

/**
 * @page MQ_TEST MQ API Test
 * @section OSALINTTESTS_MqIsFull_TC_03_007_007 OSALINTTESTS.MqIsFull_TC_03_007_007
 * @subsection MqIsFull_TC_03_007_007_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection MqIsFull_TC_03_007_007_tci Test Case ID
 * MqIsFull_S1_N_TC_03_007_007
 * @subsection MqIsFull_TC_03_007_007_rr Related Requirement(s)
 * \#199178
 * @subsection MqIsFull_TC_03_007_007_pp Prerequisite/Precondition
 * -# OSAL is initialized.<br>
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection MqIsFull_TC_03_007_007_tcs Test Case Summary
 * @subsubsection MqIsFull_TC_03_007_007_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Create a queue.
 * - Setup required memory for interrupt generation
 * - Register callack function and enable interrupt
 * - Wait for interrupt callback function finish
 * @subsubsection MqIsFull_TC_03_007_007_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * - Reset created queue
 * - Destroy created queue
 * @subsubsection MqIsFull_TC_03_007_007_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality with normal condition of the Message Queue APIs which are called in the ISR.
 * - Disable and clear interrupt status
 * - Test03_007_007: call R_OSAL_MqIsFull in ISR, queue is not full.
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection MqIsFull_TC_03_007_007_tpts Test Procedure/Test Steps
 * @subsubsection MqIsFull_TC_03_007_007_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Create a queue by calling function R_OSAL_MqCreate with bellow config
 *      - max_num_msg: 1
 *      - msg_size: 4
 * -# Allocate required memory for generate interrupt from hardware and get hw address of allocated memory.
 * -# Call R_OSAL_InterruptGetNumOfIrqChannels to get current number of interrupt channel.
 * -# Call R_OSAL_InterruptRegisterIsr to regsiter an interrupt with a callback function.
 * -# Calling R_OSAL_InterruptEnableIsr to enable interrupt.
 * -# Setup hardware specific register to generate the interrupt.
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection MqIsFull_TC_03_007_007_post_tpts Test Procedure/Test Steps of Post-process
 * -# Calling R_OSAL_InterruptUnregisterIsr to unregister the interrupt.
 * -# Deallocate required memory
 * -# Calling reset queue by using R_OSAL_MqReset.
 * -# Delete the message queue by caliing R_OSAL_MqDelete.
 * @subsubsection MqIsFull_TC_03_007_007_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Call R_OSAL_InterruptDisableIsr to disable interrupt signal.
 * -# Clear the status register by calling function R_OSAL_IoWrite32 with offset is 10 and value is TEST_CLEAR_REGISTER_VALUE.
 * -# Calling R_OSAL_MqIsFull.
 * -# Calling R_OSAL_MqSendForTimePeriod for Send a message that the interrupt context has ended
 * @subsection MqIsFull_TC_03_007_007_ereb Expected Result/Behavior
 * @subsubsection MqIsFull_TC_03_007_007_pre_ereb Expected Result/Behavior of Pre-process
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK.
 * -# The allocation successfully without any error.
 * -# The return value of R_OSAL_InterruptGetNumOfIrqChannels is OSAL_RETURN_OK.<br>
 * The return number of channel is greater than 0.
 * -# The return value of R_OSAL_InterruptRegisterIsr is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_InterruptEnableIsr is OSAL_RETURN_OK.
 * -# Setup hardware specific register to generate the interrupt success without any error.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection MqIsFull_TC_03_007_007_post_ereb Expected Result/Behavior of Post-process
 * -# The return value of R_OSAL_InterruptUnregisterIsr is OSAL_RETURN_OK.
 * -# Deallocate required memory success without any error.
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqDelete is OSAL_RETURN_OK.
 * @subsubsection MqIsFull_TC_03_007_007_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# The return value of R_OSAL_InterruptDisableIsr is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_IoWrite32 is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqIsFull is OSAL_RETURN_OK and the return result is false.
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * @subsection MqIsFull_TC_03_007_007_testcase Test Case Example
 * @snippet test_mq.cpp MqIsFull_TC_03_007_007
 * @snippet test_mq.cpp mq_test_int_cb1_normal_TC_03_007_007
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [MqIsFull_TC_03_007_007]
TEST_F(OSALINTTESTS, MqIsFull_TC_03_007_007)
{
    osal_mq_handle_t mq_handle;
    size_t numOfChannel = 0;
    st_osal_mq_config mq_config1;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE10;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    st_callback_arg_t arg;
    osal_milli_sec_t time_period = 2000;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};

/* pre-process:
 * create a queue
 * Setup required memory for interrupt generation.
 * Register callack function and enable interrupt 
 * Wait for interrupt callback function finish */
    mq_config1.max_num_msg = ISR_MQ_NUM_OF_MSG;
    mq_config1.msg_size = ISR_MQ_MSG_SIZE;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config1, ISR_MQ_ID , &mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK);
    arg.additional_arg = (void *)&mq_handle;
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, mq_test_int_cb1_normal_TC_03_007_007, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory
 * Destroy created handle */
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, mq_test_int_cb1_normal_TC_03_007_007), OSAL_RETURN_OK);
    deallocate_data_memory(hndl_mmngr, buffer_obj);
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK);
}
// [MqIsFull_TC_03_007_007]

/**
 * @page MQ_TEST MQ API Test
 * @section OSALINTTESTS_MqIsFull_TC_03_007_008 OSALINTTESTS.MqIsFull_TC_03_007_008
 * @subsection MqIsFull_TC_03_007_008_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection MqIsFull_TC_03_007_008_tci Test Case ID
 * MqIsFull_S1_N_TC_03_007_008
 * @subsection MqIsFull_TC_03_007_008_rr Related Requirement(s)
 * \#199178
 * @subsection MqIsFull_TC_03_007_008_pp Prerequisite/Precondition
 * -# OSAL is initialized.<br>
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection MqIsFull_TC_03_007_008_tcs Test Case Summary
 * @subsubsection MqIsFull_TC_03_007_008_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Create a queue.
 * - Setup required memory for interrupt generation
 * - Register callack function and enable interrupt
 * - Wait for interrupt callback function finish
 * @subsubsection MqIsFull_TC_03_007_008_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * - Reset created queue
 * - Destroy created queue
 * @subsubsection MqIsFull_TC_03_007_008_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality with normal condition of the Message Queue APIs which are called in the ISR.
 * - Disable and clear interrupt status
 * - Test03_007_008: call R_OSAL_MqIsFull in ISR, queue is full.
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection MqIsFull_TC_03_007_008_tpts Test Procedure/Test Steps
 * @subsubsection MqIsFull_TC_03_007_008_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Create a queue by calling function R_OSAL_MqCreate with bellow config
 *      - max_num_msg: 1
 *      - msg_size: 4
 * -# Allocate required memory for generate interrupt from hardware and get hw address of allocated memory.
 * -# Call R_OSAL_InterruptGetNumOfIrqChannels to get current number of interrupt channel.
 * -# Call R_OSAL_InterruptRegisterIsr to regsiter an interrupt with a callback function.
 * -# Calling R_OSAL_InterruptEnableIsr to enable interrupt.
 * -# Setup hardware specific register to generate the interrupt.
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection MqIsFull_TC_03_007_008_post_tpts Test Procedure/Test Steps of Post-process
 * -# Calling R_OSAL_InterruptUnregisterIsr to unregister the interrupt.
 * -# Deallocate required memory
 * -# Calling reset queue by using R_OSAL_MqReset.
 * -# Delete the message queue by caliing R_OSAL_MqDelete.
 * @subsubsection MqIsFull_TC_03_007_008_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Call R_OSAL_InterruptDisableIsr to disable interrupt signal.
 * -# Clear the status register by calling function R_OSAL_IoWrite32 with offset is 10 and value is TEST_CLEAR_REGISTER_VALUE.
 * -# Calling R_OSAL_MqSendForTimePeriod to send a message to queue with time period is 0.
 * -# Calling R_OSAL_MqIsFull.
 * -# Calling R_OSAL_MqReceiveForTimePeriod to receive a message to queue with time period is 0.
 * -# Calling R_OSAL_MqSendForTimePeriod for Send a message that the interrupt context has ended
 * @subsection MqIsFull_TC_03_007_008_ereb Expected Result/Behavior
 * @subsubsection MqIsFull_TC_03_007_008_pre_ereb Expected Result/Behavior of Pre-process
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK.
 * -# The allocation successfully without any error.
 * -# The return value of R_OSAL_InterruptGetNumOfIrqChannels is OSAL_RETURN_OK.<br>
 * The return number of channel is greater than 0.
 * -# The return value of R_OSAL_InterruptRegisterIsr is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_InterruptEnableIsr is OSAL_RETURN_OK.
 * -# Setup hardware specific register to generate the interrupt success without any error.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection MqIsFull_TC_03_007_008_post_ereb Expected Result/Behavior of Post-process
 * -# The return value of R_OSAL_InterruptUnregisterIsr is OSAL_RETURN_OK.
 * -# Deallocate required memory success without any error.
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqDelete is OSAL_RETURN_OK.
 * @subsubsection MqIsFull_TC_03_007_008_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# The return value of R_OSAL_InterruptDisableIsr is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_IoWrite32 is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqIsFull is OSAL_RETURN_OK and the return result is true.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK and the returned message is as expected.
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * @subsection MqIsFull_TC_03_007_008_testcase Test Case Example
 * @snippet test_mq.cpp MqIsFull_TC_03_007_008
 * @snippet test_mq.cpp mq_test_int_cb1_normal_TC_03_007_008
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [MqIsFull_TC_03_007_008]
TEST_F(OSALINTTESTS, MqIsFull_TC_03_007_008)
{
    osal_mq_handle_t mq_handle;
    size_t numOfChannel = 0;
    st_osal_mq_config mq_config1;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE10;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    st_callback_arg_t arg;
    osal_milli_sec_t time_period = 2000;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};

/* pre-process:
 * create a queue
 * Setup required memory for interrupt generation.
 * Register callack function and enable interrupt 
 * Wait for interrupt callback function finish */
    mq_config1.max_num_msg = ISR_MQ_NUM_OF_MSG;
    mq_config1.msg_size = ISR_MQ_MSG_SIZE;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config1, ISR_MQ_ID , &mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK);
    arg.additional_arg = (void *)&mq_handle;
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, mq_test_int_cb1_normal_TC_03_007_008, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory
 * Destroy created handle */
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, mq_test_int_cb1_normal_TC_03_007_008), OSAL_RETURN_OK);
    deallocate_data_memory(hndl_mmngr, buffer_obj);
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK);
}
// [MqIsFull_TC_03_007_008]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqIsFull_TC_03_007_009 MQApiTest.MqIsFull_TC_03_007_009
 * @subsection MqIsFull_TC_03_007_009_tsi Test Suite ID
 * MQApiTest
 * @subsection MqIsFull_TC_03_007_009_tci Test Case ID
 * MqIsFull_S1_N_TC_03_007_009
 * @subsection MqIsFull_TC_03_007_009_rr Related Requirement(s)
 * \#199178
 * @subsection MqIsFull_TC_03_007_009_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqIsFull_TC_03_007_009_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqIsFull API in Normal behavior in single thread
 * - Test03_007_009: evaluate that the queue is no longer full due to the reset API
 * @subsection MqIsFull_TC_03_007_009_tpts Test Procedure/Test Steps
 * -# Get the config of "shared_queue" by calling R_OSAL_MqGetConfig
 * -# Send a message to the "shared_queue" by calling R_OSAL_MqSendForTimePeriod.
 * -# Call function R_OSAL_MqIsFull with valid parameter to check "shared_queue" is full or not
 * -# Call function R_OSAL_MqReset to reset the "shared_queue".
 * -# Call function R_OSAL_MqIsFull with valid parameter to check "shared_queue" is full or not
 * @subsection MqIsFull_TC_03_007_009_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqGetConfig is OSAL_RETURN_OK.
 * -# The return value of function R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqIsFull is OSAL_RETURN_OK and the return result of checking full is true.
 * -# The return value of function R_OSAL_MqReset is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqIsFull is OSAL_RETURN_OK and the return result of checking full is false.
 * @subsection MqIsFull_TC_03_007_009_testcase Test Case Example
 * @snippet test_mq.cpp MqIsFull_TC_03_007_009
 */
// [MqIsFull_TC_03_007_009]
TEST_F(MQApiTest, MqIsFull_TC_03_007_009)
{
    bool isFull = false;
    osal_milli_sec_t time_period_10ms = 10;

    // Test03_007_009
    // @brief: normal case: evaluate that the queue is no longer full due to the reset API.
    EXPECT_EQ(R_OSAL_MqGetConfig(shared_queue, &mq_config), OSAL_RETURN_OK) << "Test03_007_009 failed";
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue, time_period_10ms, &send_msg, mq_config.msg_size), OSAL_RETURN_OK) << "Test03_007_009 failed";
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue, time_period_10ms, &send_msg, mq_config.msg_size), OSAL_RETURN_OK) << "Test03_007_009 failed";
    EXPECT_EQ(R_OSAL_MqIsFull(shared_queue, &isFull), OSAL_RETURN_OK) << "Test03_007_009 failed";
    EXPECT_EQ(isFull, true) << "Test03_007_009 failed";
    EXPECT_EQ(R_OSAL_MqReset(shared_queue), OSAL_RETURN_OK) << "Test03_007_009 failed";
    EXPECT_EQ(R_OSAL_MqIsFull(shared_queue, &isFull), OSAL_RETURN_OK) << "Test03_007_009 failed";
    EXPECT_EQ(isFull, false) << "Test03_007_009 failed";
}
// [MqIsFull_TC_03_007_009]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqIsFull_TC_03_007_010 MQApiTest.MqIsFull_TC_03_007_010
 * @subsection MqIsFull_tsiNormalSingleThread Test Suite ID
 * MQApiTest
 * @subsection MqIsFull_TC_03_007_010_tci Test Case ID
 * MqIsFull_S1_N_TC_03_007_010
 * @subsection MqIsFull_TC_03_007_010_rr Related Requirement(s)
 * \#199178
 * @subsection MqIsFull_TC_03_007_010_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqIsFull_TC_03_007_010_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqIsFull API in Normal behavior in single thread
 * - Test03_007_010: evaluate that the queue is no longer full due to the receive API
 * - Post-process: reset the shared queue.
 * @subsection MqIsFull_TC_03_007_010_tpts Test Procedure/Test Steps
 * -# Get the config of "shared_queue" by calling R_OSAL_MqGetConfig
 * -# Send 2 message to "shared_queue" by calling function R_OSAL_MqSendForTimePeriod 2 times.
 * -# Call function R_OSAL_MqIsFull with valid parameter.
 * -# Call function R_OSAL_MqReceiveForTimePeriod with valid parameter.
 * -# Call function R_OSAL_MqIsFull with valid parameter.
 * -# Call function R_OSAL_MqReset to reset the "shared_queue".
 * @subsection MqIsFull_TC_03_007_010_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqGetConfig is OSAL_RETURN_OK.
 * -# The return value of 2 times call function R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqIsFull is OSAL_RETURN_OK and the return result of checking full is true.
 * -# The return value of function R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqIsFull is OSAL_RETURN_OK and the return result of checking full is false.
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_OK.
 * @subsection MqIsFull_TC_03_007_010_testcase Test Case Example
 * @snippet test_mq.cpp MqIsFull_TC_03_007_010
 */
// [MqIsFull_TC_03_007_010]
TEST_F(MQApiTest, MqIsFull_TC_03_007_010)
{
    bool isFull = false;
    osal_milli_sec_t time_period_10ms = 10;

    // Test03_007_010
    // @brief: normal case: evaluate that the queue is no longer full due to the receive API.
    EXPECT_EQ(R_OSAL_MqGetConfig(shared_queue, &mq_config), OSAL_RETURN_OK) << "Test03_007_010 failed";
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_10ms, &send_msg, mq_config.msg_size), OSAL_RETURN_OK) << "Test03_007_010 failed";
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_10ms, &send_msg, mq_config.msg_size), OSAL_RETURN_OK) << "Test03_007_010 failed";
    EXPECT_EQ(R_OSAL_MqIsFull(shared_queue, &isFull), OSAL_RETURN_OK) << "Test03_007_010 failed";
    EXPECT_EQ(isFull, true) << "Test03_007_010 failed";
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(shared_queue,time_period_10ms, &receive_msg, mq_config.msg_size), OSAL_RETURN_OK) << "Test03_007_010 failed";
    EXPECT_EQ(R_OSAL_MqIsFull(shared_queue, &isFull), OSAL_RETURN_OK) << "Test03_007_010 failed";
    EXPECT_EQ(isFull, false) << "Test03_007_010 failed";

    //post-process: reset the shared queue
    EXPECT_EQ(R_OSAL_MqReset(shared_queue), OSAL_RETURN_OK);
}
// [MqIsFull_TC_03_007_010]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqIsFull_TC_03_007_011 MQApiTest.MqIsFull_TC_03_007_011
 * @subsection MqIsFull_TC_03_007_011_tsi Test Suite ID
 * MQApiTest
 * @subsection MqIsFull_TC_03_007_011_tci Test Case ID
 * MqIsFull_S1_N_TC_03_007_011
 * @subsection MqIsFull_TC_03_007_011_rr Related Requirement(s)
 * \#199178
 * @subsection MqIsFull_TC_03_007_011_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqIsFull_TC_03_007_011_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqIsFull API in Normal behavior in single thread
 * - Test03_007_011: Send 2 message to queue by using R_OSAL_MqSendUntilTimeStamp and check the queue is full or not
 * @subsection MqIsFull_TC_03_007_011_tpts Test Procedure/Test Steps
 * -# Get the current timestamp by using R_OSAL_ClockTimeGetTimeStamp
 * -# Send 2 message to the queue by calling function R_OSAL_MqSendUntilTimeStamp 2 times.
 * -# Call function R_OSAL_MqIsFull with valid parameter.
 * -# Call function R_OSAL_MqReset to reset the queue.
 * @subsection MqIsFull_TC_03_007_011_ereb Expected Result/Behavior
 * -# The return value of function R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of 2 times call function R_OSAL_MqSendUntilTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqIsFull is OSAL_RETURN_OK and the return result of checking full is true.
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_OK.
 * @subsection MqIsFull_TC_03_007_011_testcase Test Case Example
 * @snippet test_mq.cpp MqIsFull_TC_03_007_011
 */
// [MqIsFull_TC_03_007_011]
TEST_F(MQApiTest, MqIsFull_TC_03_007_011)
{
    bool isFull = false;
    st_osal_time_t timestamp;

    // Test03_007_011
    // @brief: normal case: call Queue is full
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &timestamp), OSAL_RETURN_OK);
    timestamp.tv_sec += 1;
    EXPECT_EQ(R_OSAL_MqSendUntilTimeStamp(shared_queue, &timestamp, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqSendUntilTimeStamp(shared_queue, &timestamp, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqIsFull(shared_queue, &isFull), OSAL_RETURN_OK) << "Test03_007_011 failed";
    EXPECT_EQ(isFull, true);
    EXPECT_EQ(R_OSAL_MqReset(shared_queue), OSAL_RETURN_OK); //reset shared queue for the next test case
    isFull = false;
}
// [MqIsFull_TC_03_007_011]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqIsEmpty_TC_03_008_001 MQApiTest.MqIsEmpty_TC_03_008_001
 * @subsection MqIsEmpty_TC_03_008_001_tsi Test Suite ID
 * MQApiTest
 * @subsection MqIsEmpty_TC_03_008_001_tci Test Case ID
 * MqIsEmpty_S1_N_TC_03_008_001
 * @subsection MqIsEmpty_TC_03_008_001_rr Related Requirement(s)
 * \#199179
 * @subsection MqIsEmpty_TC_03_008_001_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqIsEmpty_TC_03_008_001_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqIsEmpty API in Normal behavior in single thread
 * - Test03_008_001: with queue is empty.
 * @subsection MqIsEmpty_TC_03_008_001_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_MqIsEmpty with valid paramter.
 * @subsection MqIsEmpty_TC_03_008_001_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqIsEmpty is OSAL_RETURN_OK and the return result of checking empty is true.
 * @subsection MqIsEmpty_TC_03_008_001_testcase Test Case Example
 * @snippet test_mq.cpp MqIsEmpty_TC_03_008_001
 */
// [MqIsEmpty_TC_03_008_001]
TEST_F(MQApiTest, MqIsEmpty_TC_03_008_001)
{
    bool IsEmpty = false;

    // Test03_008_001
    // @brief: normal case: call Queue is empty
    EXPECT_EQ(R_OSAL_MqIsEmpty(shared_queue, &IsEmpty), OSAL_RETURN_OK) << "Test03_008_001 failed";
    EXPECT_EQ(IsEmpty, true);
}
// [MqIsEmpty_TC_03_008_001]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqIsEmpty_TC_03_008_002 MQApiTest.MqIsEmpty_TC_03_008_002
 * @subsection MqIsEmpty_TC_03_008_002_tsi Test Suite ID
 * MQApiTest
 * @subsection MqIsEmpty_TC_03_008_002_tci Test Case ID
 * MqIsEmpty_S1_N_TC_03_008_002
 * @subsection MqIsEmpty_TC_03_008_002_rr Related Requirement(s)
 * \#199179
 * @subsection MqIsEmpty_TC_03_008_002_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqIsEmpty_TC_03_008_002_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqIsEmpty API in Normal behavior in single thread
 * - Test03_008_002: send message by using R_OSAL_MqSendForTimePeriod and check queue is empty or not.
 * @subsection MqIsEmpty_TC_03_008_002_tpts Test Procedure/Test Steps
 * -# Send a message to the queue by calling function R_OSAL_MqSendForTimePeriod.
 * -# Call function R_OSAL_MqIsEmpty with valid parameter.
 * -# Call function R_OSAL_MqReset to reset the queue.
 * @subsection MqIsEmpty_TC_03_008_002_ereb Expected Result/Behavior
 * -# The return value of function R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqIsEmpty is OSAL_RETURN_OK and the return result of checking empty is false.
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_OK.
 * @subsection MqIsEmpty_TC_03_008_002_testcase Test Case Example
 * @snippet test_mq.cpp MqIsEmpty_TC_03_008_002
 */
// [MqIsEmpty_TC_03_008_002]
TEST_F(MQApiTest, MqIsEmpty_TC_03_008_002)
{
    bool IsEmpty = false;
    osal_milli_sec_t time_period_10ms = 10;

    // Test03_008_002
    // @brief: normal case: call Queue is not empty
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqIsEmpty(shared_queue, &IsEmpty), OSAL_RETURN_OK) << "Test03_008_002 failed";
    EXPECT_EQ(IsEmpty, false);
    EXPECT_EQ(R_OSAL_MqReset(shared_queue), OSAL_RETURN_OK);
}
// [MqIsEmpty_TC_03_008_002]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqIsEmpty_TC_03_008_003 MQApiTest.MqIsEmpty_TC_03_008_003
 * @subsection MqIsEmpty_TC_03_008_003_tsi Test Suite ID
 * MQApiTest
 * @subsection MqIsEmpty_TC_03_008_003_tci Test Case ID
 * MqIsEmpty_S0_A_TC_03_008_003
 * @subsection MqIsEmpty_TC_03_008_003_rr Related Requirement(s)
 * \#199179
 * @subsection MqIsEmpty_TC_03_008_003_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqIsEmpty_TC_03_008_003_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqIsEmpty API in Abnormal behavior in single thread
 * - Test03_008_003: with p_result parameter is NULL.
 * @subsection MqIsEmpty_TC_03_008_003_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_MqIsEmpty with p_result is NULL.
 * @subsection MqIsEmpty_TC_03_008_003_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqIsEmpty is OSAL_RETURN_PAR.
 * @subsection MqIsEmpty_TC_03_008_003_testcase Test Case Example
 * @snippet test_mq.cpp MqIsEmpty_TC_03_008_003
 */
// [MqIsEmpty_TC_03_008_003]
TEST_F(MQApiTest, MqIsEmpty_TC_03_008_003)
{
    bool * p_result = NULL;

    // Test03_008_003
    // @brief: abnormal case: p_result is NULL
    EXPECT_EQ(R_OSAL_MqIsEmpty(shared_queue, p_result), OSAL_RETURN_PAR) << "Test03_008_003 failed";
}
// [MqIsEmpty_TC_03_008_003]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqIsEmpty_TC_03_008_004 MQApiTest.MqIsEmpty_TC_03_008_004
 * @subsection MqIsEmpty_TC_03_008_004_tsi Test Suite ID
 * MQApiTest
 * @subsection MqIsEmpty_TC_03_008_004_tci Test Case ID
 * MqIsEmpty_S0_A_TC_03_008_004
 * @subsection MqIsEmpty_TC_03_008_004_rr Related Requirement(s)
 * \#199151, \#199165, \#199179
 * @subsection MqIsEmpty_TC_03_008_004_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqIsEmpty_TC_03_008_004_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqIsEmpty API in Abnormal behavior in single thread
 * - Test03_008_004: mq_handle parameter is NULL.
 * @subsection MqIsEmpty_TC_03_008_004_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_MqIsEmpty with mq_handle is NULL.
 * @subsection MqIsEmpty_TC_03_008_004_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqIsEmpty is OSAL_RETURN_HANDLE.
 * @subsection MqIsEmpty_TC_03_008_004_testcase Test Case Example
 * @snippet test_mq.cpp MqIsEmpty_TC_03_008_004
 */
// [MqIsEmpty_TC_03_008_004]
TEST_F(MQApiTest, MqIsEmpty_TC_03_008_004)
{
    bool IsEmpty = false;

    // Test03_008_004
    // @brief: abnormal case: handle is NULL
    EXPECT_EQ(R_OSAL_MqIsEmpty(OSAL_MQ_HANDLE_INVALID, &IsEmpty), OSAL_RETURN_HANDLE) << "Test03_008_004 failed";
}
// [MqIsEmpty_TC_03_008_004]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqIsEmpty_TC_03_008_005 MQApiTest.MqIsEmpty_TC_03_008_005
 * @subsection MqIsEmpty_TC_03_008_005_tsi Test Suite ID
 * MQApiTest
 * @subsection MqIsEmpty_TC_03_008_005_tci Test Case ID
 * MqIsEmpty_S2_A_TC_03_008_005
 * @subsection MqIsEmpty_TC_03_008_005_rr Related Requirement(s)
 * \#199179
 * @subsection MqIsEmpty_TC_03_008_005_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqIsEmpty_TC_03_008_005_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqIsEmpty API in Abnormal behavior in single thread
 * - Test03_008_005: with mq_handle has been deleted.
 * @subsection MqIsEmpty_TC_03_008_005_tpts Test Procedure/Test Steps
 * -# Create a new message queue by calling R_OSAL_MqCreate.
 * -# Delete created queue by calling R_OSAL_MqDelete.
 * -# Call function R_OSAL_MqIsEmpty.
 * @subsection MqIsEmpty_TC_03_008_005_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK and the return handle is not NULL_PTR.
 * -# The return value of R_OSAL_MqDelete is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqIsEmpty is OSAL_RETURN_HANDLE.
 * @subsection MqIsEmpty_TC_03_008_005_testcase Test Case Example
 * @snippet test_mq.cpp MqIsEmpty_TC_03_008_005
 */
// [MqIsEmpty_TC_03_008_005]
TEST_F(MQApiTest, MqIsEmpty_TC_03_008_005)
{
    bool IsEmpty = false;

    // Test03_008_005
    // @brief: abnormal case: handle is deleted
    mq_config.max_num_msg = 1;
    mq_config.msg_size = 4;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config, TEST_MQ_ID2, &mq_handle), OSAL_RETURN_OK);
    EXPECT_NE(mq_handle, NULL_PTR);
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqIsEmpty(mq_handle, &IsEmpty), OSAL_RETURN_HANDLE) << "Test03_008_005 failed";
}
// [MqIsEmpty_TC_03_008_005]

/**
 * @page MQ_TEST MQ API Test
 * @section OsalApiStateTest_MqIsEmpty_TC_03_008_006 OsalApiStateTest.MqIsEmpty_TC_03_008_006
 * @subsection MqIsEmpty_TC_03_008_006_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection MqIsEmpty_TC_03_008_006_tci Test Case ID
 * MqIsEmpty_S2_A_TC_03_008_006
 * @subsection MqIsEmpty_TC_03_008_006_rr Related Requirement(s)
 * \#199179
 * @subsection MqIsEmpty_TC_03_008_006_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection MqIsEmpty_TC_03_008_006_tcs Test Case Summary
 * This test shall checking the state of all Message queue API in Abnormal behavior in single thread
 * - Test03_008_006: checking state of R_OSAL_MqIsEmpty
 * @subsection MqIsEmpty_TC_03_008_006_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_MqIsEmpty with valid paramter.
 * @subsection MqIsEmpty_TC_03_008_006_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqIsEmpty is OSAL_RETURN_STATE.
 * @subsection MqIsEmpty_TC_03_008_006_testcase Test Case Example
 * @snippet test_mq.cpp MqIsEmpty_TC_03_008_006
 */
// [MqIsEmpty_TC_03_008_006]
TEST_F(OsalApiStateTest, MqIsEmpty_TC_03_008_006)
{
    osal_mq_handle_t mq_handle = OSAL_MQ_HANDLE_INVALID;
    bool IsEmpty;

    // Test03_008_006
    // @brief: abnormal case: state transition error of R_OSAL_MqIsEmpty
    EXPECT_EQ(R_OSAL_MqIsEmpty(mq_handle, &IsEmpty), OSAL_RETURN_STATE) << "Test03_008_006 failed";
}
// [MqIsEmpty_TC_03_008_006]

/**
 * @page MQ_TEST MQ API Test
 * @section OSALINTTESTS_MqIsEmpty_TC_03_008_007 OSALINTTESTS.MqIsEmpty_TC_03_008_007
 * @subsection MqIsEmpty_TC_03_008_007_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection MqIsEmpty_TC_03_008_007_tci Test Case ID
 * MqIsEmpty_S1_N_TC_03_008_007
 * @subsection MqIsEmpty_TC_03_008_007_rr Related Requirement(s)
 * \#199179
 * @subsection MqIsEmpty_TC_03_008_007_pp Prerequisite/Precondition
 * -# OSAL is initialized.<br>
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection MqIsEmpty_TC_03_008_007_tcs Test Case Summary
 * @subsubsection MqIsEmpty_TC_03_008_007_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Create a queue.
 * - Setup required memory for interrupt generation
 * - Register callack function and enable interrupt
 * - Wait for interrupt callback function finish
 * @subsubsection MqIsEmpty_TC_03_008_007_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * - Reset created queue
 * - Destroy created queue
 * @subsubsection MqIsEmpty_TC_03_008_007_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality with normal condition of the Message Queue APIs which are called in the ISR.
 * - Disable and clear interrupt status
 * - Test03_008_007: call R_OSAL_MqIsEmpty in ISR, queue is empty.
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection MqIsEmpty_TC_03_008_007_tpts Test Procedure/Test Steps
 * @subsubsection MqIsEmpty_TC_03_008_007_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Create a queue by calling function R_OSAL_MqCreate with bellow config
 *      - max_num_msg: 1
 *      - msg_size: 4
 * -# Allocate required memory for generate interrupt from hardware and get hw address of allocated memory.
 * -# Call R_OSAL_InterruptGetNumOfIrqChannels to get current number of interrupt channel.
 * -# Call R_OSAL_InterruptRegisterIsr to regsiter an interrupt with a callback function.
 * -# Calling R_OSAL_InterruptEnableIsr to enable interrupt.
 * -# Setup hardware specific register to generate the interrupt.
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection MqIsEmpty_TC_03_008_007_post_tpts Test Procedure/Test Steps of Post-process
 * -# Calling R_OSAL_InterruptUnregisterIsr to unregister the interrupt.
 * -# Deallocate required memory
 * -# Calling reset queue by using R_OSAL_MqReset.
 * -# Delete the message queue by caliing R_OSAL_MqDelete.
 * @subsubsection MqIsEmpty_TC_03_008_007_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Call R_OSAL_InterruptDisableIsr to disable interrupt signal.
 * -# Clear the status register by calling function R_OSAL_IoWrite32 with offset is 10 and value is TEST_CLEAR_REGISTER_VALUE.
 * -# Calling R_OSAL_MqIsEmpty.
 * -# Calling R_OSAL_MqSendForTimePeriod for Send a message that the interrupt context has ended
 * @subsection MqIsEmpty_TC_03_008_007_ereb Expected Result/Behavior
 * @subsubsection MqIsEmpty_TC_03_008_007_pre_ereb Expected Result/Behavior of Pre-process
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK.
 * -# The allocation successfully without any error.
 * -# The return value of R_OSAL_InterruptGetNumOfIrqChannels is OSAL_RETURN_OK.<br>
 * The return number of channel is greater than 0.
 * -# The return value of R_OSAL_InterruptRegisterIsr is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_InterruptEnableIsr is OSAL_RETURN_OK.
 * -# Setup hardware specific register to generate the interrupt success without any error.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection MqIsEmpty_TC_03_008_007_post_ereb Expected Result/Behavior of Post-process
 * -# The return value of R_OSAL_InterruptUnregisterIsr is OSAL_RETURN_OK.
 * -# Deallocate required memory success without any error.
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqDelete is OSAL_RETURN_OK.
 * @subsubsection MqIsEmpty_TC_03_008_007_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# The return value of R_OSAL_InterruptDisableIsr is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_IoWrite32 is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqIsEmpty is OSAL_RETURN_OK and the return result is true.
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * @subsection MqIsEmpty_TC_03_008_007_testcase Test Case Example
 * @snippet test_mq.cpp MqIsEmpty_TC_03_008_007
 * @snippet test_mq.cpp mq_test_int_cb1_normal_TC_03_008_007
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [MqIsEmpty_TC_03_008_007]
TEST_F(OSALINTTESTS, MqIsEmpty_TC_03_008_007)
{
    osal_mq_handle_t mq_handle;
    size_t numOfChannel = 0;
    st_osal_mq_config mq_config1;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE10;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    st_callback_arg_t arg;
    osal_milli_sec_t time_period = 2000;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};

/* pre-process:
 * create a queue
 * Setup required memory for interrupt generation.
 * Register callack function and enable interrupt 
 * Wait for interrupt callback function finish */
    mq_config1.max_num_msg = ISR_MQ_NUM_OF_MSG;
    mq_config1.msg_size = ISR_MQ_MSG_SIZE;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config1, ISR_MQ_ID , &mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK);
    arg.additional_arg = (void *)&mq_handle;
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, mq_test_int_cb1_normal_TC_03_008_007, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory
 * Destroy created handle */
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, mq_test_int_cb1_normal_TC_03_008_007), OSAL_RETURN_OK);
    deallocate_data_memory(hndl_mmngr, buffer_obj);
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK);
}
// [MqIsEmpty_TC_03_008_007]

/**
 * @page MQ_TEST MQ API Test
 * @section OSALINTTESTS_MqIsEmpty_TC_03_008_008 OSALINTTESTS.MqIsEmpty_TC_03_008_008
 * @subsection MqIsEmpty_TC_03_008_008_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection MqIsEmpty_TC_03_008_008_tci Test Case ID
 * MqIsEmpty_S1_N_TC_03_008_008
 * @subsection MqIsEmpty_TC_03_008_008_rr Related Requirement(s)
 * \#199179
 * @subsection MqIsEmpty_TC_03_008_008_pp Prerequisite/Precondition
 * -# OSAL is initialized.<br>
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection MqIsEmpty_TC_03_008_008_tcs Test Case Summary
 * @subsubsection MqIsEmpty_TC_03_008_008_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Create a queue.
 * - Setup required memory for interrupt generation
 * - Register callack function and enable interrupt
 * - Wait for interrupt callback function finish
 * @subsubsection MqIsEmpty_TC_03_008_008_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * - Reset created queue
 * - Destroy created queue
 * @subsubsection MqIsEmpty_TC_03_008_008_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality with normal condition of the Message Queue APIs which are called in the ISR.
 * - Disable and clear interrupt status
 * - Test03_008_008: call R_OSAL_MqIsEmpty in ISR, queue is not empty.
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection MqIsEmpty_TC_03_008_008_tpts Test Procedure/Test Steps
 * @subsubsection MqIsEmpty_TC_03_008_008_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Create a queue by calling function R_OSAL_MqCreate with bellow config
 *      - max_num_msg: 1
 *      - msg_size: 4
 * -# Allocate required memory for generate interrupt from hardware and get hw address of allocated memory.
 * -# Call R_OSAL_InterruptGetNumOfIrqChannels to get current number of interrupt channel.
 * -# Call R_OSAL_InterruptRegisterIsr to regsiter an interrupt with a callback function.
 * -# Calling R_OSAL_InterruptEnableIsr to enable interrupt.
 * -# Setup hardware specific register to generate the interrupt.
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection MqIsEmpty_TC_03_008_008_post_tpts Test Procedure/Test Steps of Post-process
 * -# Calling R_OSAL_InterruptUnregisterIsr to unregister the interrupt.
 * -# Deallocate required memory
 * -# Calling reset queue by using R_OSAL_MqReset.
 * -# Delete the message queue by caliing R_OSAL_MqDelete.
 * @subsubsection MqIsEmpty_TC_03_008_008_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Call R_OSAL_InterruptDisableIsr to disable interrupt signal.
 * -# Clear the status register by calling function R_OSAL_IoWrite32 with offset is 10 and value is TEST_CLEAR_REGISTER_VALUE.
 * -# Calling R_OSAL_MqSendForTimePeriod to send a message to the queue with time period is 0.
 * -# Calling R_OSAL_MqIsEmpty.
 * -# Calling R_OSAL_MqReceiveForTimePeriod to receive a message to queue with time period is 0.
 * -# Calling R_OSAL_MqSendForTimePeriod for Send a message that the interrupt context has ended
 * @subsection MqIsEmpty_TC_03_008_008_ereb Expected Result/Behavior
 * @subsubsection MqIsEmpty_TC_03_008_008_pre_ereb Expected Result/Behavior of Pre-process
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK.
 * -# The allocation successfully without any error.
 * -# The return value of R_OSAL_InterruptGetNumOfIrqChannels is OSAL_RETURN_OK.<br>
 * The return number of channel is greater than 0.
 * -# The return value of R_OSAL_InterruptRegisterIsr is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_InterruptEnableIsr is OSAL_RETURN_OK.
 * -# Setup hardware specific register to generate the interrupt success without any error.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection MqIsEmpty_TC_03_008_008_post_ereb Expected Result/Behavior of Post-process
 * -# The return value of R_OSAL_InterruptUnregisterIsr is OSAL_RETURN_OK.
 * -# Deallocate required memory success without any error.
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqDelete is OSAL_RETURN_OK.
 * @subsubsection MqIsEmpty_TC_03_008_008_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# The return value of R_OSAL_InterruptDisableIsr is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_IoWrite32 is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqIsEmpty is OSAL_RETURN_OK and the return result is false.
 * -# The return value of R_OSAL_MqReceiveUntilTimeStamp is OSAL_RETURN_OK and the returned message is as expected.
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * @subsection MqIsEmpty_TC_03_008_008_testcase Test Case Example
 * @snippet test_mq.cpp MqIsEmpty_TC_03_008_008
 * @snippet test_mq.cpp mq_test_int_cb1_normal_TC_03_008_008
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [MqIsEmpty_TC_03_008_008]
TEST_F(OSALINTTESTS, MqIsEmpty_TC_03_008_008)
{
    osal_mq_handle_t mq_handle;
    size_t numOfChannel = 0;
    st_osal_mq_config mq_config1;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE10;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    st_callback_arg_t arg;
    osal_milli_sec_t time_period = 2000;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};

/* pre-process:
 * create a queue
 * Setup required memory for interrupt generation.
 * Register callack function and enable interrupt 
 * Wait for interrupt callback function finish */
    mq_config1.max_num_msg = ISR_MQ_NUM_OF_MSG;
    mq_config1.msg_size = ISR_MQ_MSG_SIZE;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config1, ISR_MQ_ID , &mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK);
    arg.additional_arg = (void *)&mq_handle;
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, mq_test_int_cb1_normal_TC_03_008_008, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory
 * Destroy created handle */
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, mq_test_int_cb1_normal_TC_03_008_008), OSAL_RETURN_OK);
    deallocate_data_memory(hndl_mmngr, buffer_obj);
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK);
}
// [MqIsEmpty_TC_03_008_008]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqIsEmpty_TC_03_008_009 MQApiTest.MqIsEmpty_TC_03_008_009
 * @subsection MqIsEmpty_TC_03_008_009_tsi Test Suite ID
 * MQApiTest
 * @subsection MqIsEmpty_TC_03_008_009_tci Test Case ID
 * MqIsEmpty_S1_N_TC_03_008_009
 * @subsection MqIsEmpty_TC_03_008_009_rr Related Requirement(s)
 * \#199179
 * @subsection MqIsEmpty_TC_03_008_009_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqIsEmpty_TC_03_008_009_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqIsEmpty API in Normal behavior in single thread
 * - Test03_008_009: evaluate that it is going to become not empty by Send API
 * - Post-process: reset the shared queue.
 * @subsection MqIsEmpty_TC_03_008_009_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_MqIsEmpty with valid paramter.
 * -# Get the config of handle by calling R_OSAL_MqGetConfig with valid parameter.
 * -# Send a message to the queue by calling function R_OSAL_MqSendForTimePeriod.
 * -# Call function R_OSAL_MqIsEmpty with valid parameter.
 * -# Call function R_OSAL_MqReset to reset the queue.
 * @subsection MqIsEmpty_TC_03_008_009_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqIsEmpty is OSAL_RETURN_OK and the return result of checking empty is true.
 * -# The return value of R_OSAL_MqGetConfig is OSAL_RETURN_OK
 * -# The return value of function R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqIsEmpty is OSAL_RETURN_OK and the return result of checking empty is false.
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_OK.
 * @subsection MqIsEmpty_TC_03_008_009_testcase Test Case Example
 * @snippet test_mq.cpp MqIsEmpty_TC_03_008_009
 */
// [MqIsEmpty_TC_03_008_009]
TEST_F(MQApiTest, MqIsEmpty_TC_03_008_009)
{
    bool IsEmpty = false;
    osal_milli_sec_t time_period_10ms = 10;

    // Test03_008_009
    // @brief: normal case: evaluate that it is going to became not empty by Send API
    EXPECT_EQ(R_OSAL_MqIsEmpty(shared_queue, &IsEmpty), OSAL_RETURN_OK) << "Test03_008_009 failed";
    EXPECT_EQ(IsEmpty, true)  << "Test03_008_009 failed";
    EXPECT_EQ(R_OSAL_MqGetConfig(shared_queue, &mq_config), OSAL_RETURN_OK) << "Test03_008_009 failed";
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_10ms, &send_msg, mq_config.msg_size), OSAL_RETURN_OK) << "Test03_008_009 failed";
    EXPECT_EQ(R_OSAL_MqIsEmpty(shared_queue, &IsEmpty), OSAL_RETURN_OK) << "Test03_008_009 failed";
    EXPECT_EQ(IsEmpty, false)  << "Test03_008_009 failed";

    //post-process: reset the share queue.
    EXPECT_EQ(R_OSAL_MqReset(shared_queue), OSAL_RETURN_OK);
}
// [MqIsEmpty_TC_03_008_009]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqIsEmpty_TC_03_008_010 MQApiTest.MqIsEmpty_TC_03_008_010
 * @subsection MqIsEmpty_TC_03_008_010_tsi Test Suite ID
 * MQApiTest
 * @subsection MqIsEmpty_TC_03_008_010_tci Test Case ID
 * MqIsEmpty_S1_N_TC_03_008_010
 * @subsection MqIsEmpty_TC_03_008_010_rr Related Requirement(s)
 * \#199179
 * @subsection MqIsEmpty_TC_03_008_010_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqIsEmpty_TC_03_008_010_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqIsEmpty API in Normal behavior in single thread
 * - Test03_008_010: send message by using R_OSAL_MqSendUntilTimeStamp and check queue is empty or not.
 * @subsection MqIsEmpty_TC_03_008_010_tpts Test Procedure/Test Steps
 * -# Get the current timestamp by using R_OSAL_ClockTimeGetTimeStamp
 * -# Send a message to the queue by calling function R_OSAL_MqSendUntilTimeStamp.
 * -# Call function R_OSAL_MqIsEmpty with valid parameter.
 * -# Call function R_OSAL_MqReset to reset the queue.
 * @subsection MqIsEmpty_TC_03_008_010_ereb Expected Result/Behavior
 * -# The return value of function R_OSAL_ClockTimeGetTimeStamp is OSAL_RETURN_OK.
 * -# The return value of function R_OSAL_MqSendUntilTimeStamp is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqIsEmpty is OSAL_RETURN_OK and the return result of checking empty is false.
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_OK.
 * @subsection MqIsEmpty_TC_03_008_010_testcase Test Case Example
 * @snippet test_mq.cpp MqIsEmpty_TC_03_008_010
 */
// [MqIsEmpty_TC_03_008_010]
TEST_F(MQApiTest, MqIsEmpty_TC_03_008_010)
{
    bool IsEmpty = false;
    st_osal_time_t  current_time;

    // Test03_008_010
    // @brief: normal case: call Queue is not empty
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK);
    current_time.tv_sec += 1;
    EXPECT_EQ(R_OSAL_MqSendUntilTimeStamp(shared_queue, &current_time, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqIsEmpty(shared_queue, &IsEmpty), OSAL_RETURN_OK) << "Test03_008_010 failed";
    EXPECT_EQ(IsEmpty, false);
    EXPECT_EQ(R_OSAL_MqReset(shared_queue), OSAL_RETURN_OK);
}
// [MqIsEmpty_TC_03_008_010]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqGetConfig_TC_03_009_001 MQApiTest.MqGetConfig_TC_03_009_001
 * @subsection MqGetConfig_TC_03_009_001_tsi Test Suite ID
 * MQApiTest
 * @subsection MqGetConfig_TC_03_009_001_tci Test Case ID
 * MqGetConfig_S1_N_TC_03_009_001
 * @subsection MqGetConfig_TC_03_009_001_rr Related Requirement(s)
 * \#199180
 * @subsection MqGetConfig_TC_03_009_001_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqGetConfig_TC_03_009_001_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqGetConfig API in Normal behavior in single thread
 * - Test03_009_001: with all valid parameter.
 * @subsection MqGetConfig_TC_03_009_001_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_MqGetConfig with valid paramter.
 * @subsection MqGetConfig_TC_03_009_001_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqGetConfig is OSAL_RETURN_OK, the return p_config.max_num_msg is 2 and p_config.msg_size is 4.
 * @subsection MqGetConfig_TC_03_009_001_testcase Test Case Example
 * @snippet test_mq.cpp MqGetConfig_TC_03_009_001
 */
// [MqGetConfig_TC_03_009_001]
TEST_F(MQApiTest, MqGetConfig_TC_03_009_001)
{
    st_osal_mq_config p_config = {0,0};

    // Test03_009_001
    // @brief: normal case: all valid argument
    EXPECT_EQ(R_OSAL_MqGetConfig(shared_queue, &p_config), OSAL_RETURN_OK) << "Test03_009_001 failed";
    EXPECT_EQ(p_config.max_num_msg, 2);
    EXPECT_EQ(p_config.msg_size, 4);
}
// [MqGetConfig_TC_03_009_001]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqGetConfig_TC_03_009_002 MQApiTest.MqGetConfig_TC_03_009_002
 * @subsection MqGetConfig_TC_03_009_002_tsi Test Suite ID
 * MQApiTest
 * @subsection MqGetConfig_TC_03_009_002_tci Test Case ID
 * MqGetConfig_S0_A_TC_03_009_002
 * @subsection MqGetConfig_TC_03_009_002_rr Related Requirement(s)
 * \#199166, \#199180
 * @subsection MqGetConfig_TC_03_009_002_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqGetConfig_TC_03_009_002_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqGetConfig API in Abnormal behavior in single thread
 * - Test03_009_002: with p_config parameter is NULL.
 * @subsection MqGetConfig_TC_03_009_002_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_MqGetConfig with p_config is NULL .
 * @subsection MqGetConfig_TC_03_009_002_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqGetConfig is OSAL_RETURN_PAR.
 * @subsection MqGetConfig_TC_03_009_002_testcase Test Case Example
 * @snippet test_mq.cpp MqGetConfig_TC_03_009_002
 */
// [MqGetConfig_TC_03_009_002]
TEST_F(MQApiTest, MqGetConfig_TC_03_009_002)
{
    st_osal_mq_config* p_config_null = NULL;

    // Test03_009_002
    // @brief: abnormal case: p_config is NULL
    EXPECT_EQ(R_OSAL_MqGetConfig(shared_queue, p_config_null), OSAL_RETURN_PAR) << "Test03_009_002 failed";
}
// [MqGetConfig_TC_03_009_002]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqGetConfig_TC_03_009_003 MQApiTest.MqGetConfig_TC_03_009_003
 * @subsection MqGetConfig_TC_03_009_003_tsi Test Suite ID
 * MQApiTest
 * @subsection MqGetConfig_TC_03_009_003_tci Test Case ID
 * MqGetConfig_S0_A_TC_03_009_003
 * @subsection MqGetConfig_TC_03_009_003_rr Related Requirement(s)
 * \#199151, \#199165, \#199180
 * @subsection MqGetConfig_TC_03_009_003_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqGetConfig_TC_03_009_003_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqGetConfig API in Abnormal behavior in single thread
 * - Test03_009_003: mq_handle parameter is NULL.
 * @subsection MqGetConfig_TC_03_009_003_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_MqGetConfig with mq_handle is NULL.
 * @subsection MqGetConfig_TC_03_009_003_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqGetConfig is OSAL_RETURN_HANDLE and the return p_config is not the same as the queue config.
 * @subsection MqGetConfig_TC_03_009_003_testcase Test Case Example
 * @snippet test_mq.cpp MqGetConfig_TC_03_009_003
 */
// [MqGetConfig_TC_03_009_003]
TEST_F(MQApiTest, MqGetConfig_TC_03_009_003)
{
    st_osal_mq_config p_config = {0,0};

    // Test03_009_003
    // @brief: abnormal case: handle is NULL
    EXPECT_EQ(R_OSAL_MqGetConfig(OSAL_MQ_HANDLE_INVALID, &p_config), OSAL_RETURN_HANDLE) << "Test03_009_003 failed";
    EXPECT_EQ(p_config.max_num_msg, 0);
    EXPECT_EQ(p_config.msg_size, 0);
}
// [MqGetConfig_TC_03_009_003]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqGetConfig_TC_03_009_004 MQApiTest.MqGetConfig_TC_03_009_004
 * @subsection MqGetConfig_TC_03_009_004_tsi Test Suite ID
 * MQApiTest
 * @subsection MqGetConfig_TC_03_009_004_tci Test Case ID
 * MqGetConfig_S2_A_TC_03_009_004
 * @subsection MqGetConfig_TC_03_009_004_rr Related Requirement(s)
 * \#199180
 * @subsection MqGetConfig_TC_03_009_004_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqGetConfig_TC_03_009_004_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqGetConfig API in Abnormal behavior in single thread
 * - Test03_009_004: with mq_handle has been deleted.
 * @subsection MqGetConfig_TC_03_009_004_tpts Test Procedure/Test Steps
 * -# Create a new message queue by calling R_OSAL_MqCreate.
 * -# Delete created queue by calling R_OSAL_MqDelete.
 * -# Call function R_OSAL_MqGetConfig.
 * @subsection MqGetConfig_TC_03_009_004_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK and the return handle is not NULL_PTR.
 * -# The return value of R_OSAL_MqDelete is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqGetConfig is OSAL_RETURN_HANDLE and the return p_config is not the same as the queue config.
 * @subsection MqGetConfig_TC_03_009_004_testcase Test Case Example
 * @snippet test_mq.cpp MqGetConfig_TC_03_009_004
 */
// [MqGetConfig_TC_03_009_004]
TEST_F(MQApiTest, MqGetConfig_TC_03_009_004)
{
    st_osal_mq_config p_config = {0,0};

    // Test03_009_004
    // @brief: abnormal case: handle is deleted
    mq_config.max_num_msg = 1;
    mq_config.msg_size = 4;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config, TEST_MQ_ID2, &mq_handle), OSAL_RETURN_OK);
    EXPECT_NE(mq_handle, NULL_PTR);
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqGetConfig(mq_handle, &p_config), OSAL_RETURN_HANDLE) << "Test03_009_004 failed";
    EXPECT_EQ(p_config.max_num_msg, 0);
    EXPECT_EQ(p_config.msg_size, 0);
}
// [MqGetConfig_TC_03_009_004]

/**
 * @page MQ_TEST MQ API Test
 * @section OsalApiStateTest_MqGetConfig_TC_03_009_005 OsalApiStateTest.MqGetConfig_TC_03_009_005
 * @subsection MqGetConfig_TC_03_009_005_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection MqGetConfig_TC_03_009_005_tci Test Case ID
 * MqGetConfig_S2_A_TC_03_009_005
 * @subsection MqGetConfig_TC_03_009_005_rr Related Requirement(s)
 * \#199180
 * @subsection MqGetConfig_TC_03_009_005_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection MqGetConfig_TC_03_009_005_tcs Test Case Summary
 * This test shall checking the state of all Message queue API in Abnormal behavior in single thread
 * - Test03_008_006: checking state of R_OSAL_MqGetConfig
 * @subsection MqGetConfig_TC_03_009_005_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_MqGetConfig with valid paramter.
 * @subsection MqGetConfig_TC_03_009_005_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqGetConfig is OSAL_RETURN_STATE.
 * @subsection MqGetConfig_TC_03_009_005_testcase Test Case Example
 * @snippet test_mq.cpp MqGetConfig_TC_03_009_005
 */
// [MqGetConfig_TC_03_009_005]
TEST_F(OsalApiStateTest, MqGetConfig_TC_03_009_005)
{
    osal_mq_handle_t mq_handle = OSAL_MQ_HANDLE_INVALID;
    st_osal_mq_config mq_config;

    // Test03_009_005
    // @brief: abnormal case: state transition error of R_OSAL_MqGetConfig
    EXPECT_EQ(R_OSAL_MqGetConfig(mq_handle, &mq_config), OSAL_RETURN_STATE) << "Test03_009_005 failed";
}
// [MqGetConfig_TC_03_009_005]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReset_TC_03_010_001 MQApiTest.MqReset_TC_03_010_001
 * @subsection MqReset_TC_03_010_001_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReset_TC_03_010_001_tci Test Case ID
 * MqReset_S1_N_TC_03_010_001
 * @subsection MqReset_TC_03_010_001_rr Related Requirement(s)
 * \#199176
 * @subsection MqReset_TC_03_010_001_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection MqReset_TC_03_010_001_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqReset API in Normal behavior in single thread
 * - Test03_010_001: all valid argument, send and receive normal after reset.
 * @subsection MqReset_TC_03_010_001_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_MqSendForTimePeriod to send a message to queue. 
 * -# Call function R_OSAL_MqReset to reset the queue. 
 * -# Call function R_OSAL_MqIsEmpty to checking the queue is empty or not. 
 * -# Call function R_OSAL_MqSendForTimePeriod to send a message to queue. 
 * -# Call function R_OSAL_MqReceiveForTimePeriod to receive the message.
 * @subsection MqReset_TC_03_010_001_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK. 
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_OK. 
 * -# The return value of R_OSAL_MqIsEmpty is OSAL_RETURN_OK and the checking empty result is true. 
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK. 
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK. 
 * @subsection MqReset_TC_03_010_001_testcase Test Case Example
 * @snippet test_mq.cpp MqReset_TC_03_010_001
 */
// [MqReset_TC_03_010_001]
TEST_F(MQApiTest, MqReset_TC_03_010_001)
{
    bool IsEmpty = false;
    osal_milli_sec_t time_period_10ms = 10;
    size_t buffer_size = 4;

    // Test03_010_001
    // @brief: normal case: all valid argument, send and receive normal after reset
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReset(shared_queue), OSAL_RETURN_OK) << "Test03_010_001 failed";
    EXPECT_EQ(R_OSAL_MqIsEmpty(shared_queue, &IsEmpty), OSAL_RETURN_OK);
    EXPECT_EQ(IsEmpty, true);
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_10ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(shared_queue,time_period_10ms, &receive_msg, buffer_size), OSAL_RETURN_OK);

}
// [MqReset_TC_03_010_001]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReset_TC_03_010_002 MQApiTest.MqReset_TC_03_010_002
 * @subsection MqReset_TC_03_010_002_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReset_TC_03_010_002_tci Test Case ID
 * MqReset_S0_A_TC_03_010_002
 * @subsection MqReset_TC_03_010_002_rr Related Requirement(s)
 * \#199151, \#199165, \#199176
 * @subsection MqReset_TC_03_010_002_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqReset_TC_03_010_002_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqReset API in Abnormal behavior in single thread
 * - Test03_010_002: mq_handle parameter is NULL.
 * @subsection MqReset_TC_03_010_002_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_MqReset with mq_handle is NULL.
 * @subsection MqReset_TC_03_010_002_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_HANDLE.
 * @subsection MqReset_TC_03_010_002_testcase Test Case Example
 * @snippet test_mq.cpp MqReset_TC_03_010_002
 */
// [MqReset_TC_03_010_002]
TEST_F(MQApiTest, MqReset_TC_03_010_002)
{
    // Test03_010_002
    // @brief: abnormal case: handle is NULL
    EXPECT_EQ(R_OSAL_MqReset(OSAL_MQ_HANDLE_INVALID), OSAL_RETURN_HANDLE) << "Test03_010_002 failed";
}
// [MqReset_TC_03_010_002]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReset_TC_03_010_003 MQApiTest.MqReset_TC_03_010_003
 * @subsection MqReset_TC_03_010_003_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReset_TC_03_010_003_tci Test Case ID
 * MqReset_S0_A_TC_03_010_003
 * @subsection MqReset_TC_03_010_003_rr Related Requirement(s)
 * \#199176
 * @subsection MqReset_TC_03_010_003_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqReset_TC_03_010_003_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqReset API in Abnormal behavior in single thread
 * - Test03_010_003: with mq_handle has been deleted.
 * @subsection MqReset_TC_03_010_003_tpts Test Procedure/Test Steps
 * -# Create a new message queue by calling R_OSAL_MqCreate.
 * -# Delete created queue by calling R_OSAL_MqDelete.
 * -# Call function R_OSAL_MqReset.
 * @subsection MqReset_TC_03_010_003_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK and the return handle is not NULL_PTR.
 * -# The return value of R_OSAL_MqDelete is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_HANDLE.
 * @subsection MqReset_TC_03_010_003_testcase Test Case Example
 * @snippet test_mq.cpp MqReset_TC_03_010_003
 */
// [MqReset_TC_03_010_003]
TEST_F(MQApiTest, MqReset_TC_03_010_003)
{
    // Test03_010_003
    // @brief: abnormal case: handle is deleted
    mq_config.max_num_msg = 1;
    mq_config.msg_size = 4;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config, TEST_MQ_ID2, &mq_handle), OSAL_RETURN_OK);
    EXPECT_NE(mq_handle, NULL_PTR);
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_HANDLE) << "Test03_010_003 failed";
}
// [MqReset_TC_03_010_003]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReset_TC_03_010_004 MQApiTest.MqReset_TC_03_010_004
 * @subsection MqReset_TC_03_010_004_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReset_TC_03_010_004_tci Test Case ID
 * MqReset_S3_N_TC_03_010_004
 * @subsection MqReset_TC_03_010_004_rr Related Requirement(s)
 * \#199176, \#199181
 * @subsection MqReset_TC_03_010_004_pp Prerequisite/Precondition
 * - OSAL is initialized and the shared_queue has been created by R_OSAL_MqCreate.
 * - The synchronization queue was created.
 * @subsection MqReset_TC_03_010_004_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqReset API in Normal behavior in multi thread
 * - Test03_010_004: using multi thread with one queue.
 * @subsection MqReset_TC_03_010_004_tpts Test Procedure/Test Steps
 * -# Send 2 message to the queue the make the queue full by calling function R_OSAL_MqSendForTimePeriod 2 times.
 * -# Create 1st thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *          -# task_name is "sub_thread5" 
 *          -# func is osal_test_mq5 function with functionality:
 *              - Get the synchronization queue config by using R_OSAL_MqGetConfig
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *              - Send 2 messages 0x2222 to the queue by R_OSAL_MqSendForTimePeriod.
 *          -# userarg is send_timeperiod_arg1 with:
 *              - mq_handle is shared_queue
 *              - sync_mq is handle of synchronization queue
 *              - sync_send_msg is the sending message for synchronization purpose
 *              - needToSync is true
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1
 *      - Address of thread handle.
 * -# Create 2nd thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *          -# task_name is "sub_thread6" 
 *          -# func is osal_test_mq6 function with functionality:
 *              - Get the synchronization queue config by using R_OSAL_MqGetConfig
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *              - Send 2 messages 0x3333 to the queue by R_OSAL_MqSendForTimePeriod.
 *          -# userarg is send_timeperiod_arg2 with:
 *              - mq_handle is shared_queue
 *              - sync_mq is handle of synchronization queue
 *              - sync_send_msg is the sending message for synchronization purpose
 *              - needToSync is true
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2
 *      - Address of thread handle.
 * -# Receive 2 messages from synchronization queue by using R_OSAL_MqReceiveForTimePeriod
 * -# Create 3rd thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *          -# task_name is "sub_thread7" 
 *          -# func is osal_test_mq7 function with functionality:
 *              - Reset the queue by R_OSAL_MqReset.
 *          -# userarg is shared_queue.
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thead_id is TEST_THREAD_ID3
 *      - Address of thread handle.
 * -# Calling R_OSAL_ThreadJoin to join the 3rd thread.
 * -# Calling R_OSAL_ThreadJoin to join the 1st thread.
 * -# Calling R_OSAL_ThreadJoin to join the 2nd thread.
 * -# Call function R_OSAL_MqReceiveForTimePeriod to receive the 1st message.
 * -# Call function R_OSAL_MqReceiveForTimePeriod to receive the 2nd message.
 * -# Call function R_OSAL_MqReset.
 * @subsection MqReset_TC_03_010_004_ereb Expected Result/Behavior
 * -# The return value of 2 times calling R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of the 1st R_OSAL_ThreadCreate is OSAL_RETURN_OK.<br>
 * In function osal_test_mq5:<br>
 *      - R_OSAL_MqGetConfig return OSAL_RETURN_OK
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * -# The return value of the 2nd R_OSAL_ThreadCreate is OSAL_RETURN_OK.<br>
 * In function osal_test_mq6:<br>
 *      - R_OSAL_MqGetConfig return OSAL_RETURN_OK
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of the 3rd R_OSAL_ThreadCreate is OSAL_RETURN_OK.<br>
 * In function osal_test_mq7:<br>
 *      - R_OSAL_MqReset return OSAL_RETURN_OK
 * -# The return value of the 3rd R_OSAL_ThreadJoin is OSAL_RETURN_OK.<br>
 * "return_value3" is OSAL_RETURN_OK
 * -# The return value of the 1st R_OSAL_ThreadJoin is OSAL_RETURN_OK.<br>
 * "return_value1" is OSAL_RETURN_OK
 * -# The return value of the 2nd R_OSAL_ThreadJoin is OSAL_RETURN_OK.<br>
 * "return_value2" is OSAL_RETURN_OK
 * -# The return value of the first R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK and the received data is 0x2222 or 0x3333.
 * -# The return value of the second R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK and the received data is 0x2222 or 0x3333.
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_OK.
 * @subsection MqReset_TC_03_010_004_testcase Test Case Example
 * @snippet test_mq.cpp MqReset_TC_03_010_004
 * @snippet test_mq.cpp osal_test_mq5
 * @snippet test_mq.cpp osal_test_mq6
 * @snippet test_mq.cpp osal_test_mq7
 */
// [MqReset_TC_03_010_004]
TEST_F(MQApiTest, MqReset_TC_03_010_004)
{
    int64_t return_value1 = 0;
    int64_t return_value2 = 0;
    int64_t return_value3 = 0;
    st_osal_thread_config_t config1;
    st_osal_thread_config_t config2;
    st_osal_thread_config_t config3;
    osal_thread_handle_t thread_handle1 = OSAL_THREAD_HANDLE_INVALID;
    osal_thread_handle_t thread_handle2 = OSAL_THREAD_HANDLE_INVALID;
    osal_thread_handle_t thread_handle3 = OSAL_THREAD_HANDLE_INVALID;
    size_t buffer_size = 4;
    osal_milli_sec_t time_period_10ms = 10;
    osal_milli_sec_t time_period_100ms = 100;
    bool message_flag = false;
    st_arg_mq_send_timeperiod_t send_timeperiod_arg1;
    st_arg_mq_send_timeperiod_t send_timeperiod_arg2;

    // Test03_010_004
    // @brief: normal case: Multi thread with one queue
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_100ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_100ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);

    send_timeperiod_arg1.mq_handle = shared_queue;
    send_timeperiod_arg1.sync_mq = sync_mq;
    send_timeperiod_arg1.sync_send_msg = sync_send_msg;
    send_timeperiod_arg1.needToSync = true;
    config1.func       = osal_test_mq5;
    config1.userarg    = (void *)&send_timeperiod_arg1;
    config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config1.task_name  = (char *)"sub_thread5";
    config1.stack_size = THREAD_STACK_SIZE;

    send_timeperiod_arg2.mq_handle = shared_queue;
    send_timeperiod_arg2.sync_mq = sync_mq;
    send_timeperiod_arg2.sync_send_msg = sync_send_msg;
    send_timeperiod_arg2.needToSync = true;
    config2.func       = osal_test_mq6;
    config2.userarg    = (void *)&send_timeperiod_arg2;
    config2.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config2.task_name  = (char *)"sub_thread6";
    config2.stack_size = THREAD_STACK_SIZE;

    config3.func       = osal_test_mq7;
    config3.userarg    = (void *)&shared_queue;
    config3.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config3.task_name  = (char *)"sub_thread7";
    config3.stack_size = THREAD_STACK_SIZE;

    EXPECT_EQ(R_OSAL_ThreadCreate(&config1, TEST_THREAD_ID1, &thread_handle1),OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThreadCreate(&config2, TEST_THREAD_ID2, &thread_handle2),OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &sync_receive_msg, sync_mq_config.msg_size), OSAL_RETURN_OK);
    EXPECT_EQ(sync_receive_msg, sync_send_msg);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &sync_receive_msg, sync_mq_config.msg_size), OSAL_RETURN_OK);
    EXPECT_EQ(sync_receive_msg, sync_send_msg);
    EXPECT_EQ(R_OSAL_ThreadCreate(&config3, TEST_THREAD_ID3, &thread_handle3),OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle3, &return_value3),OSAL_RETURN_OK);
    EXPECT_EQ(return_value3, OSAL_RETURN_OK) << "Test03_010_004 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &return_value1),OSAL_RETURN_OK);
    EXPECT_EQ(return_value1, OSAL_RETURN_OK) << "Test03_010_004 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle2, &return_value2),OSAL_RETURN_OK);
    EXPECT_EQ(return_value2, OSAL_RETURN_OK) << "Test03_010_004 failed";

    // check receive date with send data by test thread
    receive_msg = 0x0;
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(shared_queue,time_period_10ms, &receive_msg, buffer_size), OSAL_RETURN_OK);
    if ((0x2222 == receive_msg) || (0x3333 == receive_msg))
    {
        message_flag = true;
    }
    EXPECT_EQ(message_flag, true);
    message_flag = false;

    // check receive date with send data by test thread
    receive_msg = 0x0;
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(shared_queue,time_period_10ms, &receive_msg, buffer_size), OSAL_RETURN_OK);
    if ((0x2222 == receive_msg) || (0x3333 == receive_msg))
    {
        message_flag = true;
    }
    EXPECT_EQ(message_flag, true);
    message_flag = false;

    EXPECT_EQ(R_OSAL_MqReset(shared_queue), OSAL_RETURN_OK) << "Test03_010_004 failed";
}
// [MqReset_TC_03_010_004]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReset_TC_03_010_005 MQApiTest.MqReset_TC_03_010_005
 * @subsection MqReset_TC_03_010_005_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReset_TC_03_010_005_tci Test Case ID
 * MqReset_S3_N_TC_03_010_005
 * @subsection MqReset_TC_03_010_005_rr Related Requirement(s)
 * \#199176, \#199181
 * @subsection MqReset_TC_03_010_005_pp Prerequisite/Precondition
 * - OSAL is initialized and the shared_queue has been created by R_OSAL_MqCreate.
 * - The synchronization queue was created.
 * @subsection MqReset_TC_03_010_005_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqReset API in Abnormal behavior in multi thread
 * - Pre-process: create 2nd queue
 * - Test03_010_005: using multi thread with multi queue.
 * - Post-process: delete 2nd queue
 * @subsection MqReset_TC_03_010_005_tpts Test Procedure/Test Steps
 * -# Create a new queue (call 2nd queue) along with the existed queue (call 1st queue) by calling function R_OSAL_MqCreate.
 * -# Make both 2 queue full by sending 2 message to the 1st queue and 1 message to 2nd queue by using R_OSAL_MqSendForTimePeriod.
 * -# Create 1st thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *          -# task_name is "sub_thread5" 
 *          -# func is osal_test_mq5 function with functionality:
 *              - Get the synchronization queue config by using R_OSAL_MqGetConfig
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *              - Send 2 messages 0x2222 to the queue by R_OSAL_MqSendForTimePeriod.
 *          -# userarg is send_timeperiod_arg1 with:
 *              - mq_handle is shared_queue
 *              - sync_mq is handle of synchronization queue
 *              - sync_send_msg is the sending message for synchronization purpose
 *              - needToSync is true
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thead_id is TEST_THREAD_ID1
 *      - Address of thread handle.
 * -# Create 2nd thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *          -# task_name is "sub_thread6" 
 *          -# func is osal_test_mq6 function with functionality:
 *              - Get the synchronization queue config by using R_OSAL_MqGetConfig
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *              - Send 2 messages 0x3333 to the queue by R_OSAL_MqSendForTimePeriod.
 *          -# userarg is send_timeperiod_arg1 with:
 *              - mq_handle is 2nd queue (mq_handle)
 *              - sync_mq is handle of synchronization queue
 *              - sync_send_msg is the sending message for synchronization purpose
 *              - needToSync is true
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thead_id is TEST_THREAD_ID2
 *      - Address of thread handle.
 * -# Receive 2 messages from synchronization queue by using R_OSAL_MqReceiveForTimePeriod
 * -# Create 3rd thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *          -# task_name is "sub_thread7" 
 *          -# func is osal_test_mq7 function with functionality:
 *              - Reset the queue by R_OSAL_MqReset.
 *          -# userarg is 1st queue (shared_queue).
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thead_id is TEST_THREAD_ID3
 *      - Address of thread handle.
 * -# Calling R_OSAL_ThreadJoin to join the 3rd thread.
 * -# Calling R_OSAL_ThreadJoin to join the 1st thread.
 * -# Calling R_OSAL_ThreadJoin to join the 2nd thread.
 * -# Calling function R_OSAL_MqReceiveForTimePeriod to receive the message from the 1st queue.
 * -# Calling R_OSAL_MqReset to reset the 2nd queue.
 * -# Calling R_OSAL_MqDelete to delete the 2nd queue.
 * @subsection MqReset_TC_03_010_005_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK and the return handle os not NULL_PTR.
 * -# The return value of 3 times calling R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of the 1st R_OSAL_ThreadCreate is OSAL_RETURN_OK.<br>
 * In function osal_test_mq5:<br>
 *      - R_OSAL_MqGetConfig return OSAL_RETURN_OK
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * -# The return value of the 2nd R_OSAL_ThreadCreate is OSAL_RETURN_OK.<br>
 * In function osal_test_mq6:<br>
 *      - R_OSAL_MqGetConfig return OSAL_RETURN_OK
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_TIME
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of the 3rd R_OSAL_ThreadCreate is OSAL_RETURN_OK.<br>
 * In function osal_test_mq7:<br>
 *      - R_OSAL_MqReset return OSAL_RETURN_OK
 * -# The return value of the 3rd R_OSAL_ThreadJoin is OSAL_RETURN_OK.<br>
 * "return_value3" is OSAL_RETURN_OK
 * -# The return value of the 1st R_OSAL_ThreadJoin is OSAL_RETURN_OK.<br>
 * "return_value1" is OSAL_RETURN_OK
 * -# The return value of the 2nd R_OSAL_ThreadJoin is OSAL_RETURN_OK.<br>
 * "return_value2" is OSAL_RETURN_TIME
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK and the receive message is 0x2222.
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqDelete is OSAL_RETURN_OK.
 * @subsection MqReset_TC_03_010_005_testcase Test Case Example
 * @snippet test_mq.cpp MqReset_TC_03_010_005
 * @snippet test_mq.cpp osal_test_mq5
 * @snippet test_mq.cpp osal_test_mq6
 * @snippet test_mq.cpp osal_test_mq7
 */
// [MqReset_TC_03_010_005]
TEST_F(MQApiTest, MqReset_TC_03_010_005)
{
    int64_t return_value1 = 0;
    int64_t return_value2 = 0;
    int64_t return_value3 = 0;
    st_osal_thread_config_t config1;
    st_osal_thread_config_t config2;
    st_osal_thread_config_t config3;
    osal_thread_handle_t thread_handle1 = OSAL_THREAD_HANDLE_INVALID;
    osal_thread_handle_t thread_handle2 = OSAL_THREAD_HANDLE_INVALID;
    osal_thread_handle_t thread_handle3 = OSAL_THREAD_HANDLE_INVALID;
    osal_milli_sec_t time_period_10ms = 10;
    osal_milli_sec_t time_period_100ms = 100;
    size_t buffer_size = 4;
    st_arg_mq_send_timeperiod_t send_timeperiod_arg1;
    st_arg_mq_send_timeperiod_t send_timeperiod_arg2;

    //pre-process: create 2nd queue
    mq_config.max_num_msg = 1;
    mq_config.msg_size = 4;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config, TEST_MQ_ID2, &mq_handle), OSAL_RETURN_OK);
    EXPECT_NE(mq_handle, NULL_PTR);

    // Test03_010_005
    // @brief: abnormal case: Multi thread with multi-queue full
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_100ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(shared_queue,time_period_100ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(mq_handle,time_period_100ms, &send_msg, sizeof(send_msg)), OSAL_RETURN_OK);

    send_timeperiod_arg1.mq_handle = shared_queue;
    send_timeperiod_arg1.sync_mq = sync_mq;
    send_timeperiod_arg1.sync_send_msg = sync_send_msg;
    send_timeperiod_arg1.needToSync = true;
    config1.func       = osal_test_mq5;
    config1.userarg    = (void *)&send_timeperiod_arg1;
    config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config1.task_name  = (char *)"sub_thread5";
    config1.stack_size = THREAD_STACK_SIZE;

    send_timeperiod_arg2.mq_handle = mq_handle;
    send_timeperiod_arg2.sync_mq = sync_mq;
    send_timeperiod_arg2.sync_send_msg = sync_send_msg;
    send_timeperiod_arg2.needToSync = true;
    config2.func       = osal_test_mq6;
    config2.userarg    = (void *)&send_timeperiod_arg2;
    config2.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config2.task_name  = (char *)"sub_thread6";
    config2.stack_size = THREAD_STACK_SIZE;

    config3.func       = osal_test_mq7;
    config3.userarg    = (void *)&shared_queue;
    config3.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config3.task_name  = (char *)"sub_thread7";
    config3.stack_size = THREAD_STACK_SIZE;

    EXPECT_EQ(R_OSAL_ThreadCreate(&config1, TEST_THREAD_ID1, &thread_handle1),OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThreadCreate(&config2, TEST_THREAD_ID2, &thread_handle2),OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &sync_receive_msg, sync_mq_config.msg_size), OSAL_RETURN_OK);
    EXPECT_EQ(sync_receive_msg, sync_send_msg);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &sync_receive_msg, sync_mq_config.msg_size), OSAL_RETURN_OK);
    EXPECT_EQ(sync_receive_msg, sync_send_msg);
    EXPECT_EQ(R_OSAL_ThreadCreate(&config3, TEST_THREAD_ID3, &thread_handle3),OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle3, &return_value3),OSAL_RETURN_OK);
    EXPECT_EQ(return_value3, OSAL_RETURN_OK) << "Test03_010_005 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &return_value1),OSAL_RETURN_OK);
    EXPECT_EQ(return_value1, OSAL_RETURN_OK) << "Test03_010_005 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle2, &return_value2),OSAL_RETURN_OK);
    EXPECT_EQ(return_value2, OSAL_RETURN_TIME) << "Test03_010_005 failed";
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(shared_queue,time_period_10ms, &receive_msg, buffer_size), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, 0x2222); //check receive data with sent data in thread 1
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK) << "Test03_010_005 failed";

    //post-process: delete 2nd queue
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK);
    mq_handle = OSAL_MQ_HANDLE_INVALID;
}
// [MqReset_TC_03_010_005]

/**
 * @page MQ_TEST MQ API Test
 * @section OsalApiStateTest_MqReset_TC_03_010_006 OsalApiStateTest.MqReset_TC_03_010_006
 * @subsection MqReset_TC_03_010_006_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection MqReset_TC_03_010_006_tci Test Case ID
 * MqReset_S2_A_TC_03_010_006
 * @subsection MqReset_TC_03_010_006_rr Related Requirement(s)
 * \#199176
 * @subsection MqReset_TC_03_010_006_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection MqReset_TC_03_010_006_tcs Test Case Summary
 * This test shall checking the state of all Message queue API in Abnormal behavior in single thread
 * - Test03_010_006: checking state of R_OSAL_MqReset
 * @subsection MqReset_TC_03_010_006_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_MqReset with valid paramter.
 * @subsection MqReset_TC_03_010_006_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_STATE.
 * @subsection MqReset_TC_03_010_006_testcase Test Case Example
 * @snippet test_mq.cpp MqReset_TC_03_010_006
 */
// [MqReset_TC_03_010_006]
TEST_F(OsalApiStateTest, MqReset_TC_03_010_006)
{
    osal_mq_handle_t mq_handle = OSAL_MQ_HANDLE_INVALID;

    // Test03_010_006
    // @brief: abnormal case: state transition error of R_OSAL_MqReset
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_STATE) << "Test03_010_006 failed";
}
// [MqReset_TC_03_010_006]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReset_TC_03_010_008 MQApiTest.MqReset_TC_03_010_008
 * @subsection MqReset_TC_03_010_008_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReset_TC_03_010_008_tci Test Case ID
 * MqReset_S3_N_TC_03_010_008
 * @subsection MqReset_TC_03_010_008_rr Related Requirement(s)
 * \#199176
 * @subsection MqReset_TC_03_010_008_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqReset_TC_03_010_008_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqReset API in Normal behavior in multi thread
 * - Test03_010_008: using R_OSAL_MqReset on the full queue to unblock a thread which is being blocked by this queue.
 * @subsection MqReset_TC_03_010_008_tpts Test Procedure/Test Steps
 * -# Create new message queue with msg_size is 4 and max_num_msg is 1 by using R_OSAL_MqCreate
 * -# Make this queue to be full by sending a "0x1234" to this queue using R_OSAL_MqSendForTimePeriod
 * -# Create new thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *          -# task_name is "task03_003_010_008"
 *          -# func is "osal_test_thread_send_timeperiod" function with functionality:
 *              - Get the synchronization queue config by using R_OSAL_MqGetConfig
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *              - Send a messages "0x4321" to the queue by R_OSAL_MqSendForTimePeriod.
 *          -# userarg is send_timeperiod_arg with:
 *              - mq_handle is the handle of created MQ
 *              - time is 5000ms
 *              - p_buffer is the address of send_data
 *              - buffer_size is the message size in config of created MQ
 *              - sync_mq is handle of synchronization queue
 *              - sync_send_msg is the sending message for synchronization purpose
 *              - needToSync is true
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thead_id is TEST_THREAD_ID1
 *      - Address of new thread handle.
 * -# Receive message from synchronization queue by using R_OSAL_MqReceiveForTimePeriod
 * -# Suspend main thread by 100ms to to make sure osal_test_thread_send_timeperiod has been blocked by R_OSAL_MqSendForTimePeriod
 * -# Reset the message queue by using R_OSAL_MqReset
 * -# Join created thread "task03_003_010_008" by using R_OSAL_ThreadJoin
 * -# Get the message queue config by using R_OSAL_MqGetConfig
 * -# Receive the message from this queue by using R_OSAL_MqReceiveForTimePeriod with the msg_size in the config above
 * -# Delete the created message queue by using R_OSAL_MqDelete
 * @subsection MqReset_TC_03_010_008_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqSendForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_ThreadCreate is OSAL_RETURN_OK.<br>
 * In worker function osal_test_thread_send_timeperiod:<br>
 *      - R_OSAL_MqGetConfig return OSAL_RETURN_OK
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_ThreadSleepForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_ThreadJoin is OSAL_RETURN_OK.<br>
 * The value of "return_value1" need to be equal with  OSAL_RETURN_OK
 * -# The return value of R_OSAL_MqGetConfig is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK.<br>
 * The value of "receive_msg" need to be equal with 0x4321
 * -# The return value of R_OSAL_MqDelete is OSAL_RETURN_OK.
 * @subsection MqReset_TC_03_010_008_testcase Test Case Example
 * @snippet test_mq.cpp MqReset_TC_03_010_008
 * @snippet test_mq.cpp osal_test_thread_send_timeperiod
 */
// [MqReset_TC_03_010_008]
TEST_F(MQApiTest, MqReset_TC_03_010_008)
{
    int64_t return_value1 = 0;
    st_osal_thread_config_t config1;
    osal_thread_handle_t thread_handle1 = OSAL_THREAD_HANDLE_INVALID;
    size_t buffer_size = 4;
    osal_milli_sec_t time_period_10ms = 10;
    osal_milli_sec_t time_period_100ms = 100;
    st_arg_mq_send_timeperiod_t send_timeperiod_arg;
    int send_data = 0;
    osal_thread_id_t thread_id;
    osal_mq_id_t mq_Id;

    // Test03_010_008
    // @brief: normal case: using R_OSAL_MqReset to reset a full queue and unblock a thread
    mq_config.max_num_msg = 1;
    mq_config.msg_size = 4;
    mq_Id = TEST_MQ_ID2;
    EXPECT_EQ(R_OSAL_MqCreate(&mq_config, mq_Id, &mq_handle), OSAL_RETURN_OK);
    EXPECT_NE(mq_handle, NULL_PTR);
    send_data = 0x1234;
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(mq_handle,time_period_100ms, &send_data, mq_config.msg_size), OSAL_RETURN_OK);

    send_data = 0x4321;
    send_timeperiod_arg.mq_handle = mq_handle;
    send_timeperiod_arg.time = 5000;
    send_timeperiod_arg.p_buffer =  &send_data;
    send_timeperiod_arg.buffer_size = mq_config.msg_size;
    send_timeperiod_arg.sync_mq = sync_mq;
    send_timeperiod_arg.sync_send_msg = sync_send_msg;
    send_timeperiod_arg.needToSync = true;

    config1.func       = osal_test_thread_send_timeperiod;
    config1.userarg    = (void *)&send_timeperiod_arg;
    config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config1.task_name  = "task03_003_010_008";
    config1.stack_size = THREAD_STACK_SIZE;
    thread_id = TEST_THREAD_ID1;

    EXPECT_EQ(R_OSAL_ThreadCreate(&config1, thread_id, &thread_handle1),OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &sync_receive_msg, sync_mq_config.msg_size), OSAL_RETURN_OK);
    EXPECT_EQ(sync_receive_msg, sync_send_msg);
    // Sleep 100ms to make sure osal_test_thread_send_timeperiod has been blocked by R_OSAL_MqSendForTimePeriod
    EXPECT_EQ(R_OSAL_ThreadSleepForTimePeriod(time_period_100ms), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReset(mq_handle), OSAL_RETURN_OK) << "Test03_010_008 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &return_value1),OSAL_RETURN_OK);
    EXPECT_EQ(return_value1, OSAL_RETURN_OK) << "Test03_010_008 failed";
    EXPECT_EQ(R_OSAL_MqGetConfig(mq_handle, &mq_config), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(mq_handle,time_period_10ms, &receive_msg, mq_config.msg_size), OSAL_RETURN_OK) << "Test03_010_008 failed";
    EXPECT_EQ(receive_msg, send_data) << "Test03_010_008 failed"; // check receive message sent to the queue after thread_handle1 is unblocked.
    EXPECT_EQ(R_OSAL_MqDelete(mq_handle), OSAL_RETURN_OK);
}
// [MqReset_TC_03_010_008]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqReset_TC_03_010_009 MQApiTest.MqReset_TC_03_010_009
 * @subsection MqReset_TC_03_010_009_tsi Test Suite ID
 * MQApiTest
 * @subsection MqReset_TC_03_010_009_tci Test Case ID
 * MqReset_S3_N_TC_03_010_009
 * @subsection MqReset_TC_03_010_009_rr Related Requirement(s)
 * \#199176
 * @subsection MqReset_TC_03_010_009_pp Prerequisite/Precondition
 * - OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * - The synchronization queue was created
 * @subsection MqReset_TC_03_010_009_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqReset API in Normal behavior in multi thread
 * - Test03_010_009: R_OSAL_MqReset does not cancel the wait when R_OSAL_MqReceiveForTimePeriod is waiting in the empty Queue.
 * @subsection MqReset_TC_03_010_009_tpts Test Procedure/Test Steps
 * -# Check the queue is empty or not by using R_OSAL_MqIsEmpty
 * -# Create new thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE10
 *          -# task_name is "task03_010_009" 
 *          -# func is "osal_test_thread_receive_timeperiod" function with functionality:
 *              - Get the synchronization queue config by using R_OSAL_MqGetConfig
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *              - Receive the message from queue by using R_OSAL_MqReceiveForTimePeriod.
 *          -# userarg is receive_timeperiod_arg with:
 *              - mq_handle is shared_queue
 *              - time is 2000ms
 *              - sync_mq is handle of synchronization queue
 *              - sync_send_msg is the sending message for synchronization purpose
 *              - needToSync is true
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thead_id is TEST_THREAD_ID1
 *      - Address of thread handle.
 * -# Receive message from synchronization queue by using R_OSAL_MqReceiveForTimePeriod
 * -# Sleep 100ms by using R_OSAL_ThreadSleepForTimePeriod to make sure osal_test_thread_receive_timeperiod has already been blocked by R_OSAL_MqReceiveForTimePeriod
 * -# Reset the "shared_queue" by using R_OSAL_MqReset
 * -# Join created thread by using R_OSAL_ThreadJoin
 * @subsection MqReset_TC_03_010_009_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqIsEmpty is OSAL_RETURN_OK.<br>
 * The value of "isEmpty" need to be equal with true
 * -# The return value of R_OSAL_ThreadCreate is OSAL_RETURN_OK.<br>
 * In worker function osal_test_thread_receive_timeperiod:<br>
 *      - R_OSAL_MqGetConfig return OSAL_RETURN_OK
 *      - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 *      - R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_TIME
 * -# The return value of R_OSAL_MqReceiveForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_ThreadSleepForTimePeriod is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqReset is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_ThreadJoin is OSAL_RETURN_OK.<br>
 * The value of "return_value1" need to be equal with OSAL_RETURN_TIME
 * @subsection MqReset_TC_03_010_009_testcase Test Case Example
 * @snippet test_mq.cpp MqReset_TC_03_010_009
 * @snippet test_mq.cpp osal_test_thread_receive_timeperiod
 */
// [MqReset_TC_03_010_009]
TEST_F(MQApiTest, MqReset_TC_03_010_009)
{
    int64_t return_value1 = 0;
    st_osal_thread_config_t config1;
    osal_thread_handle_t thread_handle1 = OSAL_THREAD_HANDLE_INVALID;
    osal_milli_sec_t time_period_100ms = 100;
    osal_thread_id_t thread_id;
    bool isEmpty = false;
    st_arg_mq_receive_timeperiod_t receive_timeperiod_arg;

    // Test03_010_009
    // @brief: normal case: R_OSAL_MqReset does not cancel the wait when R_OSAL_MqReceiveForTimePeriod is waiting in the empty Queue.
    EXPECT_EQ(R_OSAL_MqIsEmpty(shared_queue, &isEmpty), OSAL_RETURN_OK) << "Test03_010_009 failed";
    EXPECT_TRUE(isEmpty) << "Test03_010_009 failed";
    receive_timeperiod_arg.mq_handle = shared_queue;
    receive_timeperiod_arg.time = 2000;
    receive_timeperiod_arg.sync_mq = sync_mq;
    receive_timeperiod_arg.sync_send_msg = sync_send_msg;
    receive_timeperiod_arg.needToSync = true;

    config1.func       = osal_test_thread_receive_timeperiod;
    config1.userarg    = (void *)&receive_timeperiod_arg;
    config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config1.task_name  = "task03_010_009";
    config1.stack_size = THREAD_STACK_SIZE;
    thread_id = TEST_THREAD_ID1;

    EXPECT_EQ(R_OSAL_ThreadCreate(&config1, thread_id, &thread_handle1),OSAL_RETURN_OK) << "Test03_010_009 failed";
    EXPECT_NE(thread_handle1, NULL_PTR) << "Test03_010_009 failed";
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &sync_receive_msg, sync_mq_config.msg_size), OSAL_RETURN_OK);
    EXPECT_EQ(sync_receive_msg, sync_send_msg);
    EXPECT_EQ(R_OSAL_ThreadSleepForTimePeriod(time_period_100ms), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReset(shared_queue), OSAL_RETURN_OK) << "Test03_010_009 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &return_value1), OSAL_RETURN_OK) << "Test03_010_009 failed";
    EXPECT_EQ(return_value1, (int64_t)OSAL_RETURN_TIME) << "Test03_010_009 failed";
}
// [MqReset_TC_03_010_009]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqInitializeMqConfigSt_TC_03_011_001 MQApiTest.MqInitializeMqConfigSt_TC_03_011_001
 * @subsection MqInitializeMqConfigSt_TC_03_011_001_tsi Test Suite ID
 * MQApiTest
 * @subsection MqInitializeMqConfigSt_TC_03_011_001_tci Test Case ID
 * MqInitializeMqConfigSt_S1_N_TC_03_011_001
 * @subsection MqInitializeMqConfigSt_TC_03_011_001_rr Related Requirement(s)
 * \#199166, \#276547
 * @subsection MqInitializeMqConfigSt_TC_03_011_001_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqInitializeMqConfigSt_TC_03_011_001_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqInitializeMqConfigSt API in Normal behavior in single thread
 * - Test03_011_001: valid config.
 * @subsection MqInitializeMqConfigSt_TC_03_011_001_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_MqInitializeMqConfigSt with valid config paramter.
 * @subsection MqInitializeMqConfigSt_TC_03_011_001_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqInitializeMqConfigSt is OSAL_RETURN_OK.
 * @subsection MqInitializeMqConfigSt_TC_03_011_001_testcase Test Case Example
 * @snippet test_mq.cpp MqInitializeMqConfigSt_TC_03_011_001
 */
// [MqInitializeMqConfigSt_TC_03_011_001]
TEST_F(MQApiTest, MqInitializeMqConfigSt_TC_03_011_001)
{
    st_osal_mq_config_t  config;

    // Test03_011_001
    // @brief: normal case: valid config
    EXPECT_EQ(R_OSAL_MqInitializeMqConfigSt(&config), OSAL_RETURN_OK) << "Test03_011_001 failed";
    EXPECT_EQ(config.max_num_msg, 0);
    EXPECT_EQ(config.msg_size, 0);
}
// [MqInitializeMqConfigSt_TC_03_011_001]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqInitializeMqConfigSt_TC_03_011_002 MQApiTest.MqInitializeMqConfigSt_TC_03_011_002
 * @subsection MqInitializeMqConfigSt_TC_03_011_002_tsi Test Suite ID
 * MQApiTest
 * @subsection MqInitializeMqConfigSt_TC_03_011_002_tci Test Case ID
 * MqInitializeMqConfigSt_S0_A_TC_03_011_002
 * @subsection MqInitializeMqConfigSt_TC_03_011_002_rr Related Requirement(s)
 * \#199166, \#276547
 * @subsection MqInitializeMqConfigSt_TC_03_011_002_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqInitializeMqConfigSt_TC_03_011_002_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqInitializeMqConfigSt API in Abnormal behavior in single thread
 * - Test03_011_002: config is NULL.
 * @subsection MqInitializeMqConfigSt_TC_03_011_002_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_MqInitializeMqConfigSt with valid NULL paramter.
 * @subsection MqInitializeMqConfigSt_TC_03_011_002_ereb Expected Result/Behavior
 * -# The return value of R_OSAL_MqInitializeMqConfigSt is OSAL_RETURN_PAR.
 * @subsection MqInitializeMqConfigSt_TC_03_011_002_testcase Test Case Example
 * @snippet test_mq.cpp MqInitializeMqConfigSt_TC_03_011_002
 */
// [MqInitializeMqConfigSt_TC_03_011_002]
TEST_F(MQApiTest, MqInitializeMqConfigSt_TC_03_011_002)
{
    st_osal_mq_config_t * config = NULL;

    // Test03_011_002
    // @brief: abnormal case: config is NULL
    EXPECT_EQ(R_OSAL_MqInitializeMqConfigSt(config), OSAL_RETURN_PAR) << "Test03_011_002 failed";
}
// [MqInitializeMqConfigSt_TC_03_011_002]

/**
 * @page MQ_TEST MQ API Test
 * @section MQApiTest_MqInitializeMqConfigSt_TC_03_011_003 MQApiTest.MqInitializeMqConfigSt_TC_03_011_003
 * @subsection MqInitializeMqConfigSt_TC_03_011_003_tsi Test Suite ID
 * MQApiTest
 * @subsection MqInitializeMqConfigSt_TC_03_011_003_tci Test Case ID
 * MqInitializeMqConfigSt_S2_A_TC_03_011_003
 * @subsection MqInitializeMqConfigSt_TC_03_011_003_rr Related Requirement(s)
 * \#199166, \#276547
 * @subsection MqInitializeMqConfigSt_TC_03_011_003_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection MqInitializeMqConfigSt_TC_03_011_003_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqInitializeMqConfigSt API in Abnormal behavior in single thread
 * - Test03_011_003: Create new MQ using config got from R_OSAL_MqInitializeMqConfigSt
 * @subsection MqInitializeMqConfigSt_TC_03_011_003_tpts Test Procedure/Test Steps
 * -# Initialize the mq_handle with MQ_HANDLE_INITIALIZED_VALUE by using memset.<br>
 * MQ_HANDLE_INITIALIZED_VALUE must be different from OSAL_MQ_HANDLE_INVALID
 * -# Initialize Message Queue configuration object by using R_OSAL_MqInitializeMqConfigSt
 * -# Create new MQ by using R_OSAL_MqCreate with the configuration above
 * @subsection MqInitializeMqConfigSt_TC_03_011_003_ereb Expected Result/Behavior
 * -# memset a value for mq_handle successfully.
 * -# The return value of R_OSAL_MqInitializeMqConfigSt is OSAL_RETURN_OK.
 * -# The return value of R_OSAL_MqCreate is OSAL_RETURN_PAR.<br>
 * "mq_handle" is OSAL_MQ_HANDLE_INVALID.
 * @subsection MqInitializeMqConfigSt_TC_03_011_003_testcase Test Case Example
 * @snippet test_mq.cpp MqInitializeMqConfigSt_TC_03_011_003
 */
// [MqInitializeMqConfigSt_TC_03_011_003]
TEST_F(MQApiTest, MqInitializeMqConfigSt_TC_03_011_003)
{
    osal_mq_id_t mq_id = TEST_MQ_ID2;
    st_osal_mq_config_t valid_config;

    // Test03_011_003
    // @brief: abnormal case: Create new MQ using config got from R_OSAL_MqInitializeMqConfigSt
    memset(&mq_handle, MQ_HANDLE_INITIALIZED_VALUE, sizeof(mq_handle));
    EXPECT_EQ(R_OSAL_MqInitializeMqConfigSt(&valid_config), OSAL_RETURN_OK) << "Test03_011_003 failed";
    EXPECT_EQ(R_OSAL_MqCreate(&valid_config, mq_id, &mq_handle), OSAL_RETURN_PAR) << "Test03_011_003 failed";
    EXPECT_EQ(mq_handle, OSAL_MQ_HANDLE_INVALID) << "Test03_011_003 failed";
}
// [MqInitializeMqConfigSt_TC_03_011_003]


class MQSizeParameterizedTest :public PARENT_TEST_FW_PARAMETERIZED_CLASS
{
protected:
    osal_mq_handle_t queue;
    int send_msg = 0xFFFF;
    int receive_msg = 0;
    st_osal_mq_config_t config;

// [MQSizeParameterizedTest_SetUp]
    void SetUp() override
    {
        config.max_num_msg = 2;
        config.msg_size    = 4;
        EXPECT_EQ(R_OSAL_MqCreate(&config, TEST_MQ_ID1, &queue), OSAL_RETURN_OK);
        EXPECT_NE(queue, NULL_PTR);
    }
// [MQSizeParameterizedTest_SetUp]

// [MQSizeParameterizedTest_TearDown]
    void TearDown() override
    {
        EXPECT_EQ(R_OSAL_MqReset(queue), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_MqDelete(queue), OSAL_RETURN_OK);
    }
// [MQSizeParameterizedTest_TearDown]
};

// message size: {0, 1, 2, ..., 99}
#if !defined(OSAL_TEST_FRAMEWORK)
INSTANTIATE_TEST_CASE_P(MQSizeParameterizedTests, MQSizeParameterizedTest, ::testing::Range(0, 100));
#else
INSTANTIATE_TEST_CASE_P(MQSizeParameterizedTests, MQSizeParameterizedTest, (0, 100));
#endif

/**
 * @page MQ_TEST MQ API Test
 * @section MQSizeParameterizedTest_MqSendUntilTimeStamp_TC_03_003_022 MQSizeParameterizedTest.MqSendUntilTimeStamp_TC_03_003_022
 * @subsection MqSendUntilTimeStamp_TC_03_003_022_tsi Test Suite ID
 * MQSizeParameterizedTest
 * @subsection MqSendUntilTimeStamp_TC_03_003_022_tci Test Case ID
 * MqSendUntilTimeStamp_S0_A_TC_03_003_022
 * @subsection MqSendUntilTimeStamp_TC_03_003_022_rr Related Requirement(s)
 * \#199172
 * @subsection MqSendUntilTimeStamp_TC_03_003_022_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Message queue is created
 * @subsection MqSendUntilTimeStamp_TC_03_003_022_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqInitializeMqConfigSt API in Abnormal behavior in single thread
 * - Test03_003_022: Send message using R_OSAL_MqSendUntilTimeStamp with the variant of message size
 * @subsection MqSendUntilTimeStamp_TC_03_003_022_tpts Test Procedure/Test Steps
 * -# Get the configuration of created queue by using R_OSAL_MqGetConfig
 * -# Get the current timestamp by using R_OSAL_ClockTimeGetTimeStamp
 * -# Send message to the queue by using R_OSAL_MqSendUntilTimeStamp with the message size in range [0 .. 100)
 * @subsection MqSendUntilTimeStamp_TC_03_003_022_ereb Expected Result/Behavior
 * -# R_OSAL_MqGetConfig return OSAL_RETURN_OK
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK
 * -# In case message size is equal to mq config size, R_OSAL_MqSendUntilTimeStamp return OSAL_RETURN_OK.<br>
 * Otherwise, R_OSAL_MqSendUntilTimeStamp return OSAL_RETURN_PAR.
 * @subsection MqSendUntilTimeStamp_TC_03_003_022_testcase Test Case Example
 * @snippet test_mq.cpp MqSendUntilTimeStamp_TC_03_003_022
 */
// [MqSendUntilTimeStamp_TC_03_003_022]
TEST_P(MQSizeParameterizedTest, MqSendUntilTimeStamp_TC_03_003_022)
{
    st_osal_time_t  current_time;
    size_t msg_size = GetParam();

    // Test03_003_022
    // @brief: check with the variant of message size
    EXPECT_EQ(R_OSAL_MqGetConfig(queue, &config), OSAL_RETURN_OK) << "Test03_003_022 failed";
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK) << "Test03_003_022 failed";
    current_time.tv_sec += 1;
    if(msg_size != config.msg_size)
    {
        EXPECT_EQ(R_OSAL_MqSendUntilTimeStamp(queue, &current_time, &send_msg, msg_size), OSAL_RETURN_PAR) << "Test03_003_022 failed";
    }
    else
    {
        EXPECT_EQ(R_OSAL_MqSendUntilTimeStamp(queue, &current_time, &send_msg, msg_size), OSAL_RETURN_OK) << "Test03_003_022 failed";
    }
}
// [MqSendUntilTimeStamp_TC_03_003_022]

/**
 * @page MQ_TEST MQ API Test
 * @section MQSizeParameterizedTest_MqSendForTimePeriod_TC_03_004_020 MQSizeParameterizedTest.MqSendForTimePeriod_TC_03_004_020
 * @subsection MqSendForTimePeriod_TC_03_004_020_tsi Test Suite ID
 * MQSizeParameterizedTest
 * @subsection MqSendForTimePeriod_TC_03_004_020_tci Test Case ID
 * MqSendForTimePeriod_S0_A_TC_03_004_020
 * @subsection MqSendForTimePeriod_TC_03_004_020_rr Related Requirement(s)
 * \#199173
 * @subsection MqSendForTimePeriod_TC_03_004_020_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Message queue is created
 * @subsection MqSendForTimePeriod_TC_03_004_020_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqInitializeMqConfigSt API in Abnormal behavior in single thread
 * - Test03_004_020: Send message using R_OSAL_MqSendForTimePeriod with the variant of message size
 * @subsection MqSendForTimePeriod_TC_03_004_020_tpts Test Procedure/Test Steps
 * -# Get the configuration of created queue by using R_OSAL_MqGetConfig
 * -# Send message to the queue by using R_OSAL_MqSendForTimePeriod with the message size in range [0 .. 100)
 * @subsection MqSendForTimePeriod_TC_03_004_020_ereb Expected Result/Behavior
 * -# R_OSAL_MqGetConfig return OSAL_RETURN_OK
 * -# In case message size is equal to mq config size, R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK.<br>
 * Otherwise, R_OSAL_MqSendForTimePeriod return OSAL_RETURN_PAR.
 * @subsection MqSendForTimePeriod_TC_03_004_020_testcase Test Case Example
 * @snippet test_mq.cpp MqSendForTimePeriod_TC_03_004_020
 */
// [MqSendForTimePeriod_TC_03_004_020]
TEST_P(MQSizeParameterizedTest, MqSendForTimePeriod_TC_03_004_020)
{
    osal_milli_sec_t time = 10;
    size_t msg_size = GetParam();

    // Test03_004_020
    // @brief: check with the variant of message size
    EXPECT_EQ(R_OSAL_MqGetConfig(queue, &config), OSAL_RETURN_OK) << "Test03_004_020 failed";
    if(msg_size != config.msg_size)
    {
        EXPECT_EQ(R_OSAL_MqSendForTimePeriod(queue, time, &send_msg, msg_size), OSAL_RETURN_PAR) << "Test03_004_020 failed";
    }
    else
    {
        EXPECT_EQ(R_OSAL_MqSendForTimePeriod(queue, time, &send_msg, msg_size), OSAL_RETURN_OK) << "Test03_004_020 failed";
    }
}
// [MqSendForTimePeriod_TC_03_004_020]

/**
 * @page MQ_TEST MQ API Test
 * @section MQSizeParameterizedTest_MqReceiveUntilTimeStamp_TC_03_005_021 MQSizeParameterizedTest.MqReceiveUntilTimeStamp_TC_03_005_021
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_021_tsi Test Suite ID
 * MQSizeParameterizedTest
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_021_tci Test Case ID
 * MqReceiveUntilTimeStamp_S0_A_TC_03_005_021
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_021_rr Related Requirement(s)
 * \#199174
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_021_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Message queue is created
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_021_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqInitializeMqConfigSt API in Abnormal behavior in single thread
 * - Test03_005_021: Receive message using R_OSAL_MqReceiveUntilTimeStamp with the variant of message size
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_021_tpts Test Procedure/Test Steps
 * -# Get the configuration of created queue by using R_OSAL_MqGetConfig
 * -# Get the current timestamp by using R_OSAL_ClockTimeGetTimeStamp
 * -# Send message to the queue by using R_OSAL_MqSendUntilTimeStamp with the message size is mq config size
 * -# Receive message from the queue by using R_OSAL_MqReceiveUntilTimeStamp with the message size in range [0 .. 100)
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_021_ereb Expected Result/Behavior
 * -# R_OSAL_MqGetConfig return OSAL_RETURN_OK
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK
 * -# R_OSAL_MqSendUntilTimeStamp return OSAL_RETURN_OK
 * -# In case message size is equal to mq config size, R_OSAL_MqReceiveUntilTimeStamp return OSAL_RETURN_OK.<br>
 * Otherwise, R_OSAL_MqReceiveUntilTimeStamp return OSAL_RETURN_PAR.
 * @subsection MqReceiveUntilTimeStamp_TC_03_005_021_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveUntilTimeStamp_TC_03_005_021
 */
// [MqReceiveUntilTimeStamp_TC_03_005_021]
TEST_P(MQSizeParameterizedTest, MqReceiveUntilTimeStamp_TC_03_005_021)
{
    st_osal_time_t  current_time;
    size_t msg_size = GetParam();

    // Test03_005_021
    // @brief: check with the variant of message size
    EXPECT_EQ(R_OSAL_MqGetConfig(queue, &config), OSAL_RETURN_OK) << "Test03_005_021 failed";
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time), OSAL_RETURN_OK) << "Test03_005_021 failed";
    current_time.tv_sec += 1;
    EXPECT_EQ(R_OSAL_MqSendUntilTimeStamp(queue, &current_time, &send_msg, config.msg_size), OSAL_RETURN_OK) << "Test03_005_021 failed";
    if(msg_size != config.msg_size)
    {
        EXPECT_EQ(R_OSAL_MqReceiveUntilTimeStamp(queue, &current_time, &receive_msg, msg_size), OSAL_RETURN_PAR) << "Test03_005_021 failed";
    }
    else
    {
        EXPECT_EQ(R_OSAL_MqReceiveUntilTimeStamp(queue, &current_time, &receive_msg, msg_size), OSAL_RETURN_OK) << "Test03_005_021 failed";
        EXPECT_EQ(receive_msg, send_msg) << "Test03_005_021 failed";
    }
}
// [MqReceiveUntilTimeStamp_TC_03_005_021]

/**
 * @page MQ_TEST MQ API Test
 * @section MQSizeParameterizedTest_MqReceiveForTimePeriod_TC_03_006_019 MQSizeParameterizedTest.MqReceiveForTimePeriod_TC_03_006_019
 * @subsection MqReceiveForTimePeriod_TC_03_006_019_tsi Test Suite ID
 * MQSizeParameterizedTest
 * @subsection MqReceiveForTimePeriod_TC_03_006_019_tci Test Case ID
 * MqReceiveForTimePeriod_S0_A_TC_03_006_019
 * @subsection MqReceiveForTimePeriod_TC_03_006_019_rr Related Requirement(s)
 * \#199175
 * @subsection MqReceiveForTimePeriod_TC_03_006_019_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Message queue is created
 * @subsection MqReceiveForTimePeriod_TC_03_006_019_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_MqInitializeMqConfigSt API in Abnormal behavior in single thread
 * - Test03_006_019: Receive message using R_OSAL_MqReceiveForTimePeriod with the variant of message size
 * @subsection MqReceiveForTimePeriod_TC_03_006_019_tpts Test Procedure/Test Steps
 * -# Get the configuration of created queue by using R_OSAL_MqGetConfig
 * -# Send message to the queue by using R_OSAL_MqSendForTimePeriod with the message size is mq config size
 * -# Receive message from the queue by using R_OSAL_MqReceiveForTimePeriod with the message size in range [0 .. 100)
 * @subsection MqReceiveForTimePeriod_TC_03_006_019_ereb Expected Result/Behavior
 * -# R_OSAL_MqGetConfig return OSAL_RETURN_OK
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * -# In case message size is equal to mq config size, R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK.<br>
 * Otherwise, R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_PAR.
 * @subsection MqReceiveForTimePeriod_TC_03_006_019_testcase Test Case Example
 * @snippet test_mq.cpp MqReceiveForTimePeriod_TC_03_006_019
 */
// [MqReceiveForTimePeriod_TC_03_006_019]
TEST_P(MQSizeParameterizedTest, MqReceiveForTimePeriod_TC_03_006_019)
{
    osal_milli_sec_t time = 10;
    size_t msg_size = GetParam();

    // Test03_006_019
    // @brief: check with the variant of message size
    EXPECT_EQ(R_OSAL_MqGetConfig(queue, &config), OSAL_RETURN_OK) << "Test03_006_019 failed";
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(queue, time, &send_msg, config.msg_size), OSAL_RETURN_OK) << "Test03_006_019 failed";
    if(msg_size != config.msg_size)
    {
        EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(queue, time, &receive_msg, msg_size), OSAL_RETURN_PAR) << "Test03_006_019 failed";
    }
    else
    {
        EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(queue, time, &receive_msg, msg_size), OSAL_RETURN_OK) << "Test03_006_019 failed";
        EXPECT_EQ(receive_msg, send_msg) << "Test03_006_019 failed";
    }
}
// [MqReceiveForTimePeriod_TC_03_006_019]
#endif