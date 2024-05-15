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
* File Name :    test_timeclock.cpp
* Version :      3.1.1
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Time and Clock Manager API Functional Test (From the perspective of OSAL API Specification)
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           2.0.0     29.10.2021    Create New
 *           2.0.1     15.11.2021    Update Test Name
 *                                   TimeClockAndSleep -> TimeClockAndSleep_TC_12_001_007
 *           2.0.2     18.11.2021    Fix Typo
 *           3.0.0     08.03.2022    Apply new/updated device definition related to interrupt context test
 *                                   Apply updated allocate_setup_data_memory, setup_hw_register and deallocate_data_memory
 *           3.1.0     15.12.2022    Add new test cases as below:
 *                                      - ClockTimeCalculateTimeDifference_TC_04_002_014
 *                                      - ClockTimeCalculateTimeDifference_TC_04_002_015
 *                                      - ClockTimeCalculateTimeDifference_TC_04_002_016
 *                                      - ClockTimeCalculateTimeDifference_TC_04_002_017
 *                                      - ClockTimeCalculateTimeDifference_TC_04_002_018
 *                                      - ClockTimeCalculateTimeDifference_TC_04_002_019
 *           3.1.1     19.05.2023    Add process to carry tv_sec when tv_nsec exceeds 1sec against time_t structure
***********************************************************************************************************************/
#include "test_common.hpp"
#if !defined(OSAL_TEST_FRAMEWORK)
using namespace std;
#else
using namespace OsalFWHelper;
#endif

/* armclang will be warnig, when specified OsalApiStateTest to TEST_F */
/* W.A Derive a class and replace it by define */
class OsalApiStateTest_TIM : public OsalApiStateTest
{
    /* There is no need for member variables and member functions.
       We added them just for the sake of adding a test class. */
};
#define OsalApiStateTest OsalApiStateTest_TIM

/* armclang will be warnig, when specified OSALINTTESTS to TEST_F */
/* W.A Derive a class and replace it by define */
class OSALINTTESTS_TIM : public OSALINTTESTS
{
    /* There is no need for member variables and member functions.
       We added them just for the sake of adding a test class. */
};
#define OSALINTTESTS OSALINTTESTS_TIM

bool operator>(const st_osal_time_t & lhs, const st_osal_time_t & rhs)
{
    return (lhs.tv_sec * 1000 * 1000 * 1000 + lhs.tv_nsec) > (rhs.tv_sec * 1000 * 1000 * 1000 + rhs.tv_nsec);
}

bool operator<(const st_osal_time_t & lhs, const st_osal_time_t & rhs)
{
    return rhs > lhs;
}

bool operator==(const st_osal_time_t & lhs, const st_osal_time_t & rhs)
{
    return !(lhs > rhs) && !(lhs < rhs);
}

bool operator>=(const st_osal_time_t & lhs, const st_osal_time_t & rhs)
{
    return (lhs > rhs) || (lhs == rhs);
}

bool operator<=(const st_osal_time_t & lhs, const st_osal_time_t & rhs)
{
    return (lhs < rhs) || (lhs == rhs);
}

st_osal_time_t operator+(const st_osal_time_t & lhs, const st_osal_time_t & rhs)
{
    st_osal_time_t result;
    uint32_t       factor = 1000 * 1000 * 1000;
    result.tv_nsec        = (lhs.tv_nsec + rhs.tv_nsec) % factor;
    result.tv_sec         = ((lhs.tv_nsec + rhs.tv_nsec) / factor) + (lhs.tv_sec + lhs.tv_sec);
    return result;
}
/**
 * @page TimeClock_TEST Time Clock API Test
 * @test
 * @test @link OSALTESTS_TimeClockAndSleep_TC_12_001_007 @endlink
 * @test @link TimeClockApiTest_ClockTimeGetTimeStamp_TC_04_001_001 @endlink
 * @test @link TimeClockApiTest_ClockTimeGetTimeStamp_TC_04_001_002 @endlink
 * @test @link TimeClockApiTest_ClockTimeGetTimeStamp_TC_04_001_003 @endlink
 * @test @link TimeClockApiTest_ClockTimeGetTimeStamp_TC_04_001_004 @endlink
 * @test @link TimeClockApiTest_ClockTimeGetTimeStamp_TC_04_001_005 @endlink
 * @test @link OsalApiStateTest_ClockTimeGetTimeStamp_TC_04_001_006 @endlink
 * @test @link OSALINTTESTS_ClockTimeGetTimeStamp_TC_04_001_007 @endlink
 * @test @link OSALINTTESTS_ClockTimeGetTimeStamp_TC_04_001_008 @endlink
 * @test @link TimeClockApiTest_ClockTimeCalculateTimeDifference_TC_04_002_001 @endlink
 * @test @link TimeClockApiTest_ClockTimeCalculateTimeDifference_TC_04_002_002 @endlink
 * @test @link TimeClockApiTest_ClockTimeCalculateTimeDifference_TC_04_002_003 @endlink
 * @test @link TimeClockApiTest_ClockTimeCalculateTimeDifference_TC_04_002_004 @endlink
 * @test @link TimeClockApiTest_ClockTimeCalculateTimeDifference_TC_04_002_005 @endlink
 * @test @link TimeClockApiTest_ClockTimeCalculateTimeDifference_TC_04_002_006 @endlink
 * @test @link OsalApiStateTest_ClockTimeCalculateTimeDifference_TC_04_002_007 @endlink
 * @test @link OSALINTTESTS_ClockTimeCalculateTimeDifference_TC_04_002_008 @endlink
 * @test @link OSALINTTESTS_ClockTimeCalculateTimeDifference_TC_04_002_009 @endlink
 * @test @link TimeClockApiTest_ClockTimeCalculateTimeDifference_TC_04_002_010 @endlink
 * @test @link TimeClockApiTest_ClockTimeCalculateTimeDifference_TC_04_002_011 @endlink
 * @test @link TimeClockApiTest_ClockTimeCalculateTimeDifference_TC_04_002_012 @endlink
 * @test @link TimeClockApiTest_ClockTimeCalculateTimeDifference_TC_04_002_013 @endlink
 * @test @link TimeClockApiTest_ClockTimeCalculateTimeDifference_TC_04_002_014 @endlink
 * @test @link TimeClockApiTest_ClockTimeCalculateTimeDifference_TC_04_002_015 @endlink
 * @test @link TimeClockApiTest_ClockTimeCalculateTimeDifference_TC_04_002_016 @endlink
 * @test @link TimeClockApiTest_ClockTimeCalculateTimeDifference_TC_04_002_017 @endlink
 * @test @link TimeClockApiTest_ClockTimeCalculateTimeDifference_TC_04_002_018 @endlink
 * @test @link TimeClockApiTest_ClockTimeCalculateTimeDifference_TC_04_002_019 @endlink
 * @section TimeClockTEST TimeClock_TEST function
 * @subsection TimeClock_TEST_SetUp TimeClock_TEST Setup
 * Common to Test Suit of TimeClock_TEST, prepare process to be executed before executing the test. <br>
 * For example:
 * @snippet test_common.hpp OSALTESTS_SetUp
 * @subsection TimeClock_TEST_TearDown TimeClock_TEST TearDown
 * Common to Test Suit of TimeClock_TEST, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_common.hpp OSALTESTS_TearDown
 * @section TimeClockApiTest TimeClockApiTest function
 * @subsection TimeClockApiTest_SetUp TimeClockApiTest Setup
 * Common to Test Suit of TimeClockApiTest, prepare process to be executed before executing the test. <br>
 * For example:
 * @snippet test_common.hpp OSALTESTS_SetUp
 * @subsection TimeClockApiTest_TearDown TimeClockApiTest TearDown
 * Common to Test Suit of TimeClockApiTest, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_common.hpp OSALTESTS_TearDown
 * @section OSALINTTESTS_TimeClockApiTest OSALINTTESTS of TimeClockApiTest function
 * @subsection OSALINTTESTS_TimeClockApiTest_SetUp OSALINTTESTS of TimeClockApiTest Setup
 * Common to Test Suit of OSALINTTESTS TimeClockApiTest, prepare process to be executed before executing the test. <br>
 * This test suite generates interrupts. These process is preparation<br>
 * For example:
 * @snippet test_common.hpp OSALINTTESTS_SetUp
 * @subsection OSALINTTESTS_TimeClockApiTest_TearDown OSALINTTESTS of TimeClockApiTest TearDown
 * Common to Test Suit of OSALINTTESTS TimeClockApiTest, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_common.hpp OSALINTTESTS_TearDown
 * @section OsalApiStateTest_TimeClockApiTest OsalApiStateTest of TimeClockApiTest function
 * @subsection OsalApiStateTest_TimeClockApiTest_SetUp OsalApiStateTest of TimeClockApiTest Setup
 * Common to Test Suit of OsalApiStateTest TimeClockApiTest, prepare process to be executed before executing the test. <br>
 * For example:
 * @snippet test_common.hpp OsalApiStateTest_SetUp
 * @subsection OsalApiStateTest_TimeClockApiTest_TearDown OsalApiStateTest of TimeClockApiTest TearDown
 * Common to Test Suit of OsalApiStateTest TimeClockApiTest, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_common.hpp OsalApiStateTest_TearDown
 */

/**
 * @page TimeClock_TEST Time Clock API Test
 * @section OSALTESTS_TimeClockAndSleep_TC_12_001_007 OSALTESTS.TimeClockAndSleep_TC_12_001_007
 * @subsection TimeClockAndSleep_tsi Test Suite ID
 * OSALTESTS
 * @subsection TimeClockAndSleep_tci Test Case ID
 * TimeClockAndSleep_S3_N_TC_12_001_007
 * @subsection TimeClockAndSleep_rr Related Requirement(s)
 * \#199182, \#199184, \#199185, \#208002
 * @subsection TimeClockAndSleep_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection TimeClockAndSleep_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to get time stamp and the difference between two times.
 * @subsection TimeClockAndSleep_tpts Test Procedure/Test Steps
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Suspend thread with R_OSAL_ThreadSleepUntilTimeStamp (around 3ms).
 * -# Get the current time stamp again by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION. <br>
 * -# Calculate the difference between two times (at step 1 and step 3) using R_OSAL_ClockTimeCalculateTimeDifference.
 * -# Get the current time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Suspend thread 3ms with R_OSAL_ThreadSleepForTimePeriod.
 * -# Get the current time stamp again by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Calculate the difference between two times (at step 6 and 8) with R_OSAL_ClockTimeCalculateTimeDifference.
 * @subsection TimeClockAndSleep_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadSleepUntilTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK. <br>
 * The returned timestamp is equal or bigger than the previous timestamp (at step 1) plus with 3ms
 * -# R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK. <br>
 * The different time is equal or bigger than 3ms.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadSleepForTimePeriod return OSAL_RETURN_OK.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK. <br>
 * The different time is equal or bigger than 3ms.
 * @subsection TimeClockAndSleep_testcase Test Case Example
 * @snippet test_timeclock.cpp TimeClockAndSleep_TC_12_001_007
 */
// [TimeClockAndSleep_TC_12_001_007]
TEST_F(OSALTESTS, TimeClockAndSleep_TC_12_001_007)
{
    st_osal_time_t      time_before_sleep, time_after_sleep, sleepTill;
    e_osal_clock_type_t type;
    osal_nano_sec_t     difference_expected = 0;
    int32_t             ms                  = 3; // 3ms
    int32_t             difference          = ms * 1000 * 1000;

    type = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(type, &time_before_sleep), OSAL_RETURN_OK);

    sleepTill.tv_sec  = time_before_sleep.tv_sec;
    sleepTill.tv_nsec = time_before_sleep.tv_nsec + difference;
    if(MAX_NSEC < sleepTill.tv_nsec)
    {
        sleepTill.tv_sec += 1;
        sleepTill.tv_nsec %= (MAX_NSEC + 1);
    }

    EXPECT_EQ(R_OSAL_ThreadSleepUntilTimeStamp(&sleepTill), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(type, &time_after_sleep), OSAL_RETURN_OK);

    EXPECT_GE(time_after_sleep, sleepTill);

    EXPECT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&time_after_sleep, &time_before_sleep, &difference_expected),
              OSAL_RETURN_OK);
    EXPECT_GE(difference_expected, difference);

    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(type, &time_before_sleep), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThreadSleepForTimePeriod(ms), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(type, &time_after_sleep), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&time_after_sleep, &time_before_sleep, &difference_expected),
              OSAL_RETURN_OK);
    EXPECT_GE(difference_expected, difference);
}
// [TimeClockAndSleep_TC_12_001_007]

class TimeClockApiTest : public OSALTESTS
{
protected:
    e_osal_clock_type_t clock_type;
    st_osal_time_t absolute_time;
};

// [osal_timeclock_isr_cb_TC_04_001_007]
void osal_timeclock_isr_cb_TC_04_001_007(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    e_osal_clock_type_t clock_type;
    osal_milli_sec_t time_0ms = 0;
    st_osal_time_t absolute_time;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, TEST_REG_CLEAR_REGISTER, TEST_CLEAR_REGISTER_VALUE), OSAL_RETURN_OK);

    // Test04_001_007
    // @brief: call R_OSAL_ClockTimeGetTimeStamp in ISR with clock_type = OSAL_CLOCK_TYPE_HIGH_RESOLUTION
    memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
    clock_type  = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(clock_type, &absolute_time), OSAL_RETURN_OK) << "Test04_001_007 failed";

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, time_0ms, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_timeclock_isr_cb_TC_04_001_007]

// [osal_timeclock_isr_cb_TC_04_001_008]
void osal_timeclock_isr_cb_TC_04_001_008(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    e_osal_clock_type_t clock_type;
    osal_milli_sec_t time_0ms = 0;
    st_osal_time_t absolute_time;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, TEST_REG_CLEAR_REGISTER, TEST_CLEAR_REGISTER_VALUE), OSAL_RETURN_OK);

    // Test04_001_008
    // @brief: call R_OSAL_ClockTimeGetTimeStamp in ISR with clock_type = OSAL_CLOCK_TYPE_HIGH_PRECISION
    memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
    clock_type  = OSAL_CLOCK_TYPE_HIGH_PRECISION;
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(clock_type, &absolute_time), OSAL_RETURN_OK) << "Test04_001_008 failed";

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, time_0ms, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_timeclock_isr_cb_TC_04_001_008]

// [osal_timeclock_isr_cb_TC_04_002_008]
void osal_timeclock_isr_cb_TC_04_002_008(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    e_osal_clock_type_t clock_type;
    osal_milli_sec_t time_0ms = 0;
    st_osal_time_t p_time2;
    st_osal_time_t p_time1;
    osal_nano_sec_t p_time_difference;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, TEST_REG_CLEAR_REGISTER, TEST_CLEAR_REGISTER_VALUE), OSAL_RETURN_OK);

    // Test04_002_008
    // @brief: call R_OSAL_ClockTimeCalculateTimeDifference in ISR with time2 is greater than time1
    memset(&p_time2, 0x00, sizeof(st_osal_time_t));
    memset(&p_time1, 0x00, sizeof(st_osal_time_t));
    memset(&p_time_difference, 0x00, sizeof(osal_nano_sec_t));
    clock_type  = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(clock_type, &p_time1), OSAL_RETURN_OK);
    p_time2.tv_sec = p_time1.tv_sec + 1;
    p_time2.tv_nsec = p_time1.tv_nsec;
    ASSERT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&p_time2, &p_time1, &p_time_difference), OSAL_RETURN_OK) << "Test04_002_008 failed";
    EXPECT_EQ(p_time_difference, 1000000000) << "Test04_002_008 failed";

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, time_0ms, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_timeclock_isr_cb_TC_04_002_008]

// [osal_timeclock_isr_cb_TC_04_002_009]
void osal_timeclock_isr_cb_TC_04_002_009(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    e_osal_clock_type_t clock_type;
    osal_milli_sec_t time_0ms = 0;
    st_osal_time_t p_time2;
    st_osal_time_t p_time1;
    osal_nano_sec_t p_time_difference;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, TEST_REG_CLEAR_REGISTER, TEST_CLEAR_REGISTER_VALUE), OSAL_RETURN_OK);

    // Test04_002_009
    // @brief: call R_OSAL_ClockTimeCalculateTimeDifference in ISR with time2 =  time1
    memset(&p_time2, 0x00, sizeof(st_osal_time_t));
    memset(&p_time1, 0x00, sizeof(st_osal_time_t));
    memset(&p_time_difference, 0x00, sizeof(osal_nano_sec_t));
    clock_type  = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(clock_type, &p_time1), OSAL_RETURN_OK);
    p_time2.tv_sec = p_time1.tv_sec;
    p_time2.tv_nsec = p_time1.tv_nsec;
    ASSERT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&p_time2, &p_time1, &p_time_difference), OSAL_RETURN_OK) << "Test04_002_009 failed";
    EXPECT_EQ(p_time_difference, 0) << "Test04_002_009 failed";

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, time_0ms, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_timeclock_isr_cb_TC_04_002_009]

/**
 * @page TimeClock_TEST Time Clock API Test
 * @section TimeClockApiTest_ClockTimeGetTimeStamp_TC_04_001_001 TimeClockApiTest.ClockTimeGetTimeStamp_TC_04_001_001
 * @subsection ClockTimeGetTimeStamp_TC_04_001_001_tsi Test Suite ID
 * TimeClockApiTest
 * @subsection ClockTimeGetTimeStamp_TC_04_001_001_tci Test Case ID
 * ClockTimeGetTimeStamp_S1_BN_TC_04_001_001
 * @subsection ClockTimeGetTimeStamp_TC_04_001_001_rr Related Requirement(s)
 * \#199182, \#199184, \#208002
 * @subsection ClockTimeGetTimeStamp_TC_04_001_001_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ClockTimeGetTimeStamp_TC_04_001_001_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to acquire the system time stamp on normal condition.
 * - Test04_001_001: "clock_type" is equal to OSAL_CLOCK_TYPE_HIGH_RESOLUTION
 * @subsection ClockTimeGetTimeStamp_TC_04_001_001_tpts Test Procedure/Test Steps
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * @subsection ClockTimeGetTimeStamp_TC_04_001_001_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.<br>
 * Value of "tv_sec" is greater than or equal to 0.<br>
 * Value of "tv_nsec" is greater than or equal to 0.<br>
 * Value of "tv_nsec" is less than or equal to 999999999.
 * @subsection ClockTimeGetTimeStamp_TC_04_001_001_testcase Test Case Example
 * @snippet test_timeclock.cpp ClockTimeGetTimeStamp_TC_04_001_001
 */
// [ClockTimeGetTimeStamp_TC_04_001_001]
TEST_F(TimeClockApiTest, ClockTimeGetTimeStamp_TC_04_001_001)
{
    // Test04_001_001
    // @brief: normal case: clock_type =  OSAL_CLOCK_TYPE_HIGH_RESOLUTION
    memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
    clock_type  = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(clock_type, &absolute_time), OSAL_RETURN_OK) << "Test04_001_001 failed";
    EXPECT_GE(absolute_time.tv_sec, 0) << "Test04_001_001 failed";
    EXPECT_GE(absolute_time.tv_nsec, 0) << "Test04_001_001 failed";
    EXPECT_LE(absolute_time.tv_nsec, 999999999) << "Test04_001_001 failed";
}
// [ClockTimeGetTimeStamp_TC_04_001_001]

/**
 * @page TimeClock_TEST Time Clock API Test
 * @section TimeClockApiTest_ClockTimeGetTimeStamp_TC_04_001_002 TimeClockApiTest.ClockTimeGetTimeStamp_TC_04_001_002
 * @subsection ClockTimeGetTimeStamp_TC_04_001_002_tsi Test Suite ID
 * TimeClockApiTest
 * @subsection ClockTimeGetTimeStamp_TC_04_001_002_tci Test Case ID
 * ClockTimeGetTimeStamp_S1_BN_TC_04_001_002
 * @subsection ClockTimeGetTimeStamp_TC_04_001_002_rr Related Requirement(s)
 * \#199182, \#199184, \#208002
 * @subsection ClockTimeGetTimeStamp_TC_04_001_002_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ClockTimeGetTimeStamp_TC_04_001_002_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to acquire the system time stamp on normal condition.
 * - Test04_001_002: "clock_type" is equal to OSAL_CLOCK_TYPE_HIGH_PRECISION
 * @subsection ClockTimeGetTimeStamp_TC_04_001_002_tpts Test Procedure/Test Steps
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_PRECISION.
 * @subsection ClockTimeGetTimeStamp_TC_04_001_002_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.<br>
 * Value of "tv_sec" is greater than or equal to 0.<br>
 * Value of "tv_nsec" is greater than or equal to 0.<br>
 * Value of "tv_nsec" is less than or equal to 999999999.
 * @subsection ClockTimeGetTimeStamp_TC_04_001_002_testcase Test Case Example
 * @snippet test_timeclock.cpp ClockTimeGetTimeStamp_TC_04_001_002
 */
// [ClockTimeGetTimeStamp_TC_04_001_002]
TEST_F(TimeClockApiTest, ClockTimeGetTimeStamp_TC_04_001_002)
{
    // Test04_001_002
    // @brief: normal case: clock_type =  OSAL_CLOCK_TYPE_HIGH_PRECISION
    memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
    clock_type  = OSAL_CLOCK_TYPE_HIGH_PRECISION;
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(clock_type, &absolute_time), OSAL_RETURN_OK) << "Test04_001_002 failed";
    EXPECT_GE(absolute_time.tv_sec, 0) << "Test04_001_002 failed";
    EXPECT_GE(absolute_time.tv_nsec, 0) << "Test04_001_002 failed";
    EXPECT_LE(absolute_time.tv_nsec, 999999999) << "Test04_001_002 failed";
}
// [ClockTimeGetTimeStamp_TC_04_001_002]

/**
 * @page TimeClock_TEST Time Clock API Test
 * @section TimeClockApiTest_ClockTimeGetTimeStamp_TC_04_001_003 TimeClockApiTest.ClockTimeGetTimeStamp_TC_04_001_003
 * @subsection ClockTimeGetTimeStamp_TC_04_001_003_tsi Test Suite ID
 * TimeClockApiTest
 * @subsection ClockTimeGetTimeStamp_TC_04_001_003_tci Test Case ID
 * ClockTimeGetTimeStamp_S0_BA_TC_04_001_003
 * @subsection ClockTimeGetTimeStamp_TC_04_001_003_rr Related Requirement(s)
 * \#199182, \#199184
 * @subsection ClockTimeGetTimeStamp_TC_04_001_003_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ClockTimeGetTimeStamp_TC_04_001_003_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to acquire the system time stamp on abnormal condition.
 * - Test04_001_003: "clock_type" is equal to OSAL_CLOCK_TYPE_HIGH_RESOLUTION - 1
 * @subsection ClockTimeGetTimeStamp_TC_04_001_003_tpts Test Procedure/Test Steps
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION - 1.
 * @subsection ClockTimeGetTimeStamp_TC_04_001_003_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_PAR.
 * @subsection ClockTimeGetTimeStamp_TC_04_001_003_testcase Test Case Example
 * @snippet test_timeclock.cpp ClockTimeGetTimeStamp_TC_04_001_003
 */
// [ClockTimeGetTimeStamp_TC_04_001_003]
TEST_F(TimeClockApiTest, ClockTimeGetTimeStamp_TC_04_001_003)
{
    // Test04_001_003
    // @brief: abnormal case: clock_type =  OSAL_CLOCK_TYPE_HIGH_RESOLUTION - 1
    memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
    clock_type  = (e_osal_clock_type_t)(OSAL_CLOCK_TYPE_HIGH_RESOLUTION -1);
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(clock_type, &absolute_time), OSAL_RETURN_PAR) << "Test04_001_003 failed";
}
// [ClockTimeGetTimeStamp_TC_04_001_003]

/**
 * @page TimeClock_TEST Time Clock API Test
 * @section TimeClockApiTest_ClockTimeGetTimeStamp_TC_04_001_004 TimeClockApiTest.ClockTimeGetTimeStamp_TC_04_001_004
 * @subsection ClockTimeGetTimeStamp_TC_04_001_004_tsi Test Suite ID
 * TimeClockApiTest
 * @subsection ClockTimeGetTimeStamp_TC_04_001_004_tci Test Case ID
 * ClockTimeGetTimeStamp_S0_BA_TC_04_001_004
 * @subsection ClockTimeGetTimeStamp_TC_04_001_004_rr Related Requirement(s)
 * \#199182, \#199184
 * @subsection ClockTimeGetTimeStamp_TC_04_001_004_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ClockTimeGetTimeStamp_TC_04_001_004_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to acquire the system time stamp on abnormal condition.
 * - Test04_001_004: "clock_type" is equal to OSAL_CLOCK_TYPE_HIGH_PRECISION + 1
 * @subsection ClockTimeGetTimeStamp_TC_04_001_004_tpts Test Procedure/Test Steps
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION + 1.
 * @subsection ClockTimeGetTimeStamp_TC_04_001_004_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_PAR.
 * @subsection ClockTimeGetTimeStamp_TC_04_001_004_testcase Test Case Example
 * @snippet test_timeclock.cpp ClockTimeGetTimeStamp_TC_04_001_004
 */
// [ClockTimeGetTimeStamp_TC_04_001_004]
TEST_F(TimeClockApiTest, ClockTimeGetTimeStamp_TC_04_001_004)
{
    // Test04_001_004
    // @brief: abnormal case: clock_type =  OSAL_CLOCK_TYPE_HIGH_PRECISION + 1
    memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
    clock_type  = (e_osal_clock_type_t)(OSAL_CLOCK_TYPE_HIGH_PRECISION +1);
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(clock_type, &absolute_time), OSAL_RETURN_PAR) << "Test04_001_004 failed";
}
// [ClockTimeGetTimeStamp_TC_04_001_004]

/**
 * @page TimeClock_TEST Time Clock API Test
 * @section TimeClockApiTest_ClockTimeGetTimeStamp_TC_04_001_005 TimeClockApiTest.ClockTimeGetTimeStamp_TC_04_001_005
 * @subsection ClockTimeGetTimeStamp_TC_04_001_005_tsi Test Suite ID
 * TimeClockApiTest
 * @subsection ClockTimeGetTimeStamp_TC_04_001_005_tci Test Case ID
 * ClockTimeGetTimeStamp_S0_A_TC_04_001_005
 * @subsection ClockTimeGetTimeStamp_TC_04_001_005_rr Related Requirement(s)
 * \#199182, \#199184
 * @subsection ClockTimeGetTimeStamp_TC_04_001_005_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ClockTimeGetTimeStamp_TC_04_001_005_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to acquire the system time stamp on abnormal condition.
 * - Test04_001_005: "p_absolute_time" is NULL
 * @subsection ClockTimeGetTimeStamp_TC_04_001_005_tpts Test Procedure/Test Steps
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with the argument as below:
 *      - "clock_type" is equal to OSAL_CLOCK_TYPE_HIGH_RESOLUTION
 *      - "p_time_stamp" is NULL
 * @subsection ClockTimeGetTimeStamp_TC_04_001_005_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_PAR.
 * @subsection ClockTimeGetTimeStamp_TC_04_001_005_testcase Test Case Example
 * @snippet test_timeclock.cpp ClockTimeGetTimeStamp_TC_04_001_005
 */
// [ClockTimeGetTimeStamp_TC_04_001_005]
TEST_F(TimeClockApiTest, ClockTimeGetTimeStamp_TC_04_001_005)
{
    // Test04_001_005
    // @brief: abnormal case: p_absolute_time =  NULL
    st_osal_time_t *p_time_stamp = NULL;
    clock_type  = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(clock_type, p_time_stamp), OSAL_RETURN_PAR) << "Test04_001_005 failed";
}
// [ClockTimeGetTimeStamp_TC_04_001_005]

/**
 * @page TimeClock_TEST Time Clock API Test
 * @section OsalApiStateTest_ClockTimeGetTimeStamp_TC_04_001_006 OsalApiStateTest.ClockTimeGetTimeStamp_TC_04_001_006
 * @subsection ClockTimeGetTimeStamp_TC_04_001_006_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection ClockTimeGetTimeStamp_TC_04_001_006_tci Test Case ID
 * ClockTimeGetTimeStamp_S2_A_TC_04_001_006
 * @subsection ClockTimeGetTimeStamp_TC_04_001_006_rr Related Requirement(s)
 * \#199182, \#199184
 * @subsection ClockTimeGetTimeStamp_TC_04_001_006_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection ClockTimeGetTimeStamp_TC_04_001_006_tcs Test Case Summary
 * This test shall verify the state transition of Clock Time APIs:
 * - Test04_001_006: state transition error of R_OSAL_ClockTimeGetTimeStamp
 * @subsection ClockTimeGetTimeStamp_TC_04_001_006_tpts Test Procedure/Test Steps
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * @subsection ClockTimeGetTimeStamp_TC_04_001_006_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_STATE.
 * @subsection ClockTimeGetTimeStamp_TC_04_001_006_testcase Test Case Example
 * @snippet test_timeclock.cpp ClockTimeGetTimeStamp_TC_04_001_006
 */
// [ClockTimeGetTimeStamp_TC_04_001_006]
TEST_F(OsalApiStateTest, ClockTimeGetTimeStamp_TC_04_001_006)
{
    e_osal_clock_type_t clock_type;
    st_osal_time_t absolute_time;
    
    // Test04_001_006
    // @brief: abnormal case: state transition error
    memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
    clock_type  = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(clock_type, &absolute_time), OSAL_RETURN_STATE) << "Test04_001_006 failed";
}
// [ClockTimeGetTimeStamp_TC_04_001_006]

/**
 * @page TimeClock_TEST Time Clock API Test
 * @section OSALINTTESTS_ClockTimeGetTimeStamp_TC_04_001_007 OSALINTTESTS.ClockTimeGetTimeStamp_TC_04_001_007
 * @subsection ClockTimeGetTimeStamp_TC_04_001_007_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection ClockTimeGetTimeStamp_TC_04_001_007_tci Test Case ID
 * ClockTimeGetTimeStamp_S1_N_TC_04_001_007
 * @subsection ClockTimeGetTimeStamp_TC_04_001_007_rr Related Requirement(s)
 * \#199182, \#199184, \#208002
 * @subsection ClockTimeGetTimeStamp_TC_04_001_007_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection ClockTimeGetTimeStamp_TC_04_001_007_tcs Test Case Summary
 * @subsubsection ClockTimeGetTimeStamp_TC_04_001_007_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Setup required memory for interrupt generation
 * - Register callack function and enable interrupt
 * - Wait for interrupt callback finish
 * @subsubsection ClockTimeGetTimeStamp_TC_04_001_007_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * @subsubsection ClockTimeGetTimeStamp_TC_04_001_007_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality of OSAL API in interrupt context with normal condition.
 * - Disable and clear interrupt status
 * - Test04_001_007: Get time-stamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION clock in ISR
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection ClockTimeGetTimeStamp_TC_04_001_007_tpts Test Procedure/Test Steps
 * @subsubsection ClockTimeGetTimeStamp_TC_04_001_007_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Allocate required memory for generate interrupt from hardware and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function by using R_OSAL_InterruptRegisterIsr
 * -# Enable interrupt by using R_OSAL_InterruptEnableIsr
 * -# Setup hardware specific register to generate the interrupt by calling to setup_hw_register
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection ClockTimeGetTimeStamp_TC_04_001_007_post_tpts Test Procedure/Test Steps of Post-process
 * -# Unregister interrupt by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * @subsubsection ClockTimeGetTimeStamp_TC_04_001_007_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Send Message interrupt of context finished by using R_OSAL_MqSendForTimePeriod
 * @subsection ClockTimeGetTimeStamp_TC_04_001_007_ereb Expected Result/Behavior
 * @subsubsection ClockTimeGetTimeStamp_TC_04_001_007_pre_ereb Expected Result/Behavior of Pre-process
 * -# Allocate required memory successfully without any errors.
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * Value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK.
 * -# setup_hw_register run successfully.
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection ClockTimeGetTimeStamp_TC_04_001_007_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * @subsubsection ClockTimeGetTimeStamp_TC_04_001_007_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK.
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * @subsection ClockTimeGetTimeStamp_TC_04_001_007_testcase Test Case Example
 * @snippet test_timeclock.cpp ClockTimeGetTimeStamp_TC_04_001_007
 * @snippet test_timeclock.cpp osal_timeclock_isr_cb_TC_04_001_007
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [ClockTimeGetTimeStamp_TC_04_001_007]
TEST_F(OSALINTTESTS, ClockTimeGetTimeStamp_TC_04_001_007)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    osal_milli_sec_t time_period = 2000;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};

/* pre-process:
 * Setup required memory for interrupt generation.
 * Register callack function and enable interrupt.
 * Wait for interrupt callback finish */
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;

    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_timeclock_isr_cb_TC_04_001_007, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory*/
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_timeclock_isr_cb_TC_04_001_007), OSAL_RETURN_OK);
    deallocate_data_memory(hndl_mmngr, buffer_obj);
}
// [ClockTimeGetTimeStamp_TC_04_001_007]

/**
 * @page TimeClock_TEST Time Clock API Test
 * @section OSALINTTESTS_ClockTimeGetTimeStamp_TC_04_001_008 OSALINTTESTS.ClockTimeGetTimeStamp_TC_04_001_008
 * @subsection ClockTimeGetTimeStamp_TC_04_001_008_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection ClockTimeGetTimeStamp_TC_04_001_008_tci Test Case ID
 * ClockTimeGetTimeStamp_S1_N_TC_04_001_008
 * @subsection ClockTimeGetTimeStamp_TC_04_001_008_rr Related Requirement(s)
 * \#199182, \#199184, \#208002
 * @subsection ClockTimeGetTimeStamp_TC_04_001_008_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection ClockTimeGetTimeStamp_TC_04_001_008_tcs Test Case Summary
 * @subsubsection ClockTimeGetTimeStamp_TC_04_001_008_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Setup required memory for interrupt generation
 * - Register callack function and enable interrupt
 * - Wait for interrupt callback finish
 * @subsubsection ClockTimeGetTimeStamp_TC_04_001_008_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * @subsubsection ClockTimeGetTimeStamp_TC_04_001_008_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality of OSAL API in interrupt context with normal condition.
 * - Step 1 - 2: Disable and clear interrupt status
 * - Step 4: Test04_001_008: Get time-stamp with OSAL_CLOCK_TYPE_HIGH_PRECISION clock in ISR
 * - Step 9: Send a message of the end of the interrupt thread to the message queue
 * @subsection ClockTimeGetTimeStamp_TC_04_001_008_tpts Test Procedure/Test Steps
 * @subsubsection ClockTimeGetTimeStamp_TC_04_001_008_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Allocate required memory for generate interrupt from hardware and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function by using R_OSAL_InterruptRegisterIsr
 * -# Enable interrupt by using R_OSAL_InterruptEnableIsr
 * -# Setup hardware specific register to generate the interrupt by calling to setup_hw_register
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection ClockTimeGetTimeStamp_TC_04_001_008_post_tpts Test Procedure/Test Steps of Post-process
 * -# Unregister interrupt by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * @subsubsection ClockTimeGetTimeStamp_TC_04_001_008_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_PRECISION.
 * -# Send Message interrupt of context finished by using R_OSAL_MqSendForTimePeriod
 * @subsection ClockTimeGetTimeStamp_TC_04_001_008_ereb Expected Result/Behavior
 * @subsubsection ClockTimeGetTimeStamp_TC_04_001_008_pre_ereb Expected Result/Behavior of Pre-process
 * -# Allocate required memory successfully without any errors.
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * Value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK.
 * -# setup_hw_register run successfully.
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection ClockTimeGetTimeStamp_TC_04_001_008_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * @subsubsection ClockTimeGetTimeStamp_TC_04_001_008_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK.
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * @subsection ClockTimeGetTimeStamp_TC_04_001_008_testcase Test Case Example
 * @snippet test_timeclock.cpp ClockTimeGetTimeStamp_TC_04_001_008
 * @snippet test_timeclock.cpp osal_timeclock_isr_cb_TC_04_001_008
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [ClockTimeGetTimeStamp_TC_04_001_008]
TEST_F(OSALINTTESTS, ClockTimeGetTimeStamp_TC_04_001_008)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    osal_milli_sec_t time_period = 2000;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};

/* pre-process:
 * Setup required memory for interrupt generation.
 * Register callack function and enable interrupt.
 * Wait for interrupt callback finish */
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;

    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_timeclock_isr_cb_TC_04_001_008, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory*/
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_timeclock_isr_cb_TC_04_001_008), OSAL_RETURN_OK);
    deallocate_data_memory(hndl_mmngr, buffer_obj);
}
// [ClockTimeGetTimeStamp_TC_04_001_008]

/**
 * @page TimeClock_TEST Time Clock API Test
 * @section TimeClockApiTest_ClockTimeCalculateTimeDifference_TC_04_002_001 TimeClockApiTest.ClockTimeCalculateTimeDifference_TC_04_002_001
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_001_tsi Test Suite ID
 * TimeClockApiTest
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_001_tci Test Case ID
 * ClockTimeCalculateTimeDifference_S1_N_TC_04_002_001
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_001_rr Related Requirement(s)
 * \#199185, \#208002
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_001_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_001_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to acquire the difference between two times on normal condition.
 * - Test04_002_001: "p_time2.tv_sec" is greater than "p_time1.tv_sec". "p_time2.tv_nsec" is equal to "p_time1.tv_nsec"
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_001_tpts Test Procedure/Test Steps
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Calculate the difference between two times using R_OSAL_ClockTimeCalculateTimeDifference with the argument as below:
 *      - "p_time2" is not NULL
 *      - "p_time1" is not NULL and less than "p_time2"
 *      - "p_time_difference" is not NULL
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_001_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK.<br>
 * Value of "p_time_difference" is equal to 1000000000.
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_001_testcase Test Case Example
 * @snippet test_timeclock.cpp ClockTimeCalculateTimeDifference_TC_04_002_001
 */
// [ClockTimeCalculateTimeDifference_TC_04_002_001]
TEST_F(TimeClockApiTest, ClockTimeCalculateTimeDifference_TC_04_002_001)
{
    st_osal_time_t p_time2;
    st_osal_time_t p_time1;
    osal_nano_sec_t p_time_difference;

    // Test04_002_001
    // @brief: normal case: time2 greater than time1
    memset(&p_time2, 0x00, sizeof(st_osal_time_t));
    memset(&p_time1, 0x00, sizeof(st_osal_time_t));
    memset(&p_time_difference, 0x00, sizeof(osal_nano_sec_t));
    clock_type  = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(clock_type, &p_time1), OSAL_RETURN_OK);
    p_time2.tv_sec = p_time1.tv_sec + 1;
    p_time2.tv_nsec = p_time1.tv_nsec;
    ASSERT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&p_time2, &p_time1, &p_time_difference), OSAL_RETURN_OK) << "Test04_002_001 failed";
    EXPECT_EQ(p_time_difference, 1000000000) << "Test04_002_001 failed";
}
// [ClockTimeCalculateTimeDifference_TC_04_002_001]

/**
 * @page TimeClock_TEST Time Clock API Test
 * @section TimeClockApiTest_ClockTimeCalculateTimeDifference_TC_04_002_002 TimeClockApiTest.ClockTimeCalculateTimeDifference_TC_04_002_002
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_002_tsi Test Suite ID
 * TimeClockApiTest
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_002_tci Test Case ID
 * ClockTimeCalculateTimeDifference_S1_N_TC_04_002_002
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_002_rr Related Requirement(s)
 * \#199185, \#208002
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_002_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_002_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to acquire the difference between two times on normal condition.
 * - Test04_002_002: "p_time2.tv_sec" is less than "p_time1.tv_sec". "p_time2.tv_nsec" is equal to "p_time1.tv_nsec"
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_002_tpts Test Procedure/Test Steps
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Calculate the difference between two times using R_OSAL_ClockTimeCalculateTimeDifference with the argument as below:
 *      - "p_time2" is not NULL
 *      - "p_time1" is not NULL and bigger than "p_time2"
 *      - "p_time_difference" is not NULL
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_002_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK.<br>
 * Value of "p_time_difference" is equal to -1000000000.
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_002_testcase Test Case Example
 * @snippet test_timeclock.cpp ClockTimeCalculateTimeDifference_TC_04_002_002
 */
// [ClockTimeCalculateTimeDifference_TC_04_002_002]
TEST_F(TimeClockApiTest, ClockTimeCalculateTimeDifference_TC_04_002_002)
{
    st_osal_time_t p_time2;
    st_osal_time_t p_time1;
    osal_nano_sec_t p_time_difference;

    // Test04_002_002
    // @brief: normal case: time1 greater than time2
    memset(&p_time2, 0x00, sizeof(st_osal_time_t));
    memset(&p_time1, 0x00, sizeof(st_osal_time_t));
    memset(&p_time_difference, 0x00, sizeof(osal_nano_sec_t));
    clock_type  = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(clock_type, &p_time2), OSAL_RETURN_OK);
    p_time1.tv_sec = p_time2.tv_sec + 1;
    p_time1.tv_nsec = p_time2.tv_nsec;
    ASSERT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&p_time2, &p_time1, &p_time_difference), OSAL_RETURN_OK) << "Test04_002_002 failed";
    EXPECT_EQ(p_time_difference, (int64_t)-1000000000) << "Test04_002_002 failed";
}
// [ClockTimeCalculateTimeDifference_TC_04_002_002]

/**
 * @page TimeClock_TEST Time Clock API Test
 * @section TimeClockApiTest_ClockTimeCalculateTimeDifference_TC_04_002_003 TimeClockApiTest.ClockTimeCalculateTimeDifference_TC_04_002_003
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_003_tsi Test Suite ID
 * TimeClockApiTest
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_003_tci Test Case ID
 * ClockTimeCalculateTimeDifference_S0_A_TC_04_002_003
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_003_rr Related Requirement(s)
 * \#199185
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_003_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_003_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to acquire the difference between two times on abnormal condition.
 * - Test04_002_003: "p_time2" is NULL
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_003_tpts Test Procedure/Test Steps
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Calculate the difference between two times using R_OSAL_ClockTimeCalculateTimeDifference with the argument as below:
 *      - "p_time2" is NULL
 *      - "p_time1" is not NULL
 *      - "p_time_difference" is not NULL
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_003_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_PAR.
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_003_testcase Test Case Example
 * @snippet test_timeclock.cpp ClockTimeCalculateTimeDifference_TC_04_002_003
 */
// [ClockTimeCalculateTimeDifference_TC_04_002_003]
TEST_F(TimeClockApiTest, ClockTimeCalculateTimeDifference_TC_04_002_003)
{
    st_osal_time_t time1;
    osal_nano_sec_t time_difference;
    st_osal_time_t *p_time2;

    // Test04_002_003
    // @brief: abnormal case: time2 is NULL
    memset(&time1, 0x00, sizeof(st_osal_time_t));
    memset(&time_difference, 0x00, sizeof(osal_nano_sec_t));
    clock_type  = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(clock_type, &time1), OSAL_RETURN_OK);
    p_time2 = NULL;
    ASSERT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(p_time2, &time1, &time_difference), OSAL_RETURN_PAR) << "Test04_002_003 failed";
}
// [ClockTimeCalculateTimeDifference_TC_04_002_003]

/**
 * @page TimeClock_TEST Time Clock API Test
 * @section TimeClockApiTest_ClockTimeCalculateTimeDifference_TC_04_002_004 TimeClockApiTest.ClockTimeCalculateTimeDifference_TC_04_002_004
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_004_tsi Test Suite ID
 * TimeClockApiTest
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_004_tci Test Case ID
 * ClockTimeCalculateTimeDifference_S0_A_TC_04_002_004
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_004_rr Related Requirement(s)
 * \#199185
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_004_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_004_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to acquire the difference between two times on abnormal condition.
 * - Test04_002_004: "p_time1" is NULL
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_004_tpts Test Procedure/Test Steps
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Calculate the difference between two times using R_OSAL_ClockTimeCalculateTimeDifference with the argument as below:
 *      - "p_time2" is not NULL
 *      - "p_time1" is NULL
 *      - "p_time_difference" is not NULL
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_004_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_PAR.
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_004_testcase Test Case Example
 * @snippet test_timeclock.cpp ClockTimeCalculateTimeDifference_TC_04_002_004
 */
// [ClockTimeCalculateTimeDifference_TC_04_002_004]
TEST_F(TimeClockApiTest, ClockTimeCalculateTimeDifference_TC_04_002_004)
{
    st_osal_time_t time2;
    st_osal_time_t time1;
    osal_nano_sec_t time_difference;
    st_osal_time_t *p_time1;

    // Test04_002_004
    // @brief: abnormal case: time1 is NULL
    memset(&time2, 0x00, sizeof(st_osal_time_t));
    memset(&time1, 0x00, sizeof(st_osal_time_t));
    memset(&time_difference, 0x00, sizeof(osal_nano_sec_t));
    clock_type  = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(clock_type, &time1), OSAL_RETURN_OK);
    time2.tv_sec = time1.tv_sec + 1;
    time2.tv_nsec = time1.tv_nsec;
    p_time1 = NULL;
    ASSERT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&time2, p_time1, &time_difference), OSAL_RETURN_PAR) << "Test04_002_004 failed";
}
// [ClockTimeCalculateTimeDifference_TC_04_002_004]

/**
 * @page TimeClock_TEST Time Clock API Test
 * @section TimeClockApiTest_ClockTimeCalculateTimeDifference_TC_04_002_005 TimeClockApiTest.ClockTimeCalculateTimeDifference_TC_04_002_005
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_005_tsi Test Suite ID
 * TimeClockApiTest
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_005_tci Test Case ID
 * ClockTimeCalculateTimeDifference_S1_N_TC_04_002_005
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_005_rr Related Requirement(s)
 * \#199185, \#208002
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_005_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_005_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to acquire the difference between two times on normal condition.
 * - Test04_002_005: "p_time2.tv_sec" is equal to "p_time1.tv_sec". "p_time2.tv_nsec" is equal to "p_time1.tv_nsec"
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_005_tpts Test Procedure/Test Steps
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Calculate the difference between two times using R_OSAL_ClockTimeCalculateTimeDifference with the argument as below:
 *      - "p_time2" is not NULL
 *      - "p_time1" is not NULL and equal to "p_time2"
 *      - "p_time_difference" is not NULL
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_005_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK.<br>
 * Value of "p_time_difference" is equal to 0.
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_005_testcase Test Case Example
 * @snippet test_timeclock.cpp ClockTimeCalculateTimeDifference_TC_04_002_005
 */
// [ClockTimeCalculateTimeDifference_TC_04_002_005]
TEST_F(TimeClockApiTest, ClockTimeCalculateTimeDifference_TC_04_002_005)
{
    st_osal_time_t p_time2;
    st_osal_time_t p_time1;
    osal_nano_sec_t p_time_difference;

    // Test04_002_005
    // @brief: normal case: time2 =  time1
    memset(&p_time2, 0x00, sizeof(st_osal_time_t));
    memset(&p_time1, 0x00, sizeof(st_osal_time_t));
    memset(&p_time_difference, 0x00, sizeof(osal_nano_sec_t));
    clock_type  = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(clock_type, &p_time1), OSAL_RETURN_OK);
    p_time2.tv_sec = p_time1.tv_sec;
    p_time2.tv_nsec = p_time1.tv_nsec;
    ASSERT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&p_time2, &p_time1, &p_time_difference), OSAL_RETURN_OK) << "Test04_002_005 failed";
    EXPECT_EQ(p_time_difference, 0) << "Test04_002_005 failed";
}
// [ClockTimeCalculateTimeDifference_TC_04_002_005]

/**
 * @page TimeClock_TEST Time Clock API Test
 * @section TimeClockApiTest_ClockTimeCalculateTimeDifference_TC_04_002_006 TimeClockApiTest.ClockTimeCalculateTimeDifference_TC_04_002_006
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_006_tsi Test Suite ID
 * TimeClockApiTest
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_006_tci Test Case ID
 * ClockTimeCalculateTimeDifference_S0_A_TC_04_002_006
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_006_rr Related Requirement(s)
 * \#199183, \#199185
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_006_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_006_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to acquire the difference between two times on abnormal condition.
 * - Test04_002_006: "p_time_difference" is NULL
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_006_tpts Test Procedure/Test Steps
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Calculate the difference between two times using R_OSAL_ClockTimeCalculateTimeDifference with the argument as below:
 *      - "p_time2" is not NULL
 *      - "p_time1" is not NULL
 *      - "p_time_difference" is NULL
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_006_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_PAR.
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_006_testcase Test Case Example
 * @snippet test_timeclock.cpp ClockTimeCalculateTimeDifference_TC_04_002_006
 */
// [ClockTimeCalculateTimeDifference_TC_04_002_006]
TEST_F(TimeClockApiTest, ClockTimeCalculateTimeDifference_TC_04_002_006)
{
    st_osal_time_t time2;
    st_osal_time_t time1;
    osal_nano_sec_t *p_time_difference;

    // Test04_002_006
    // @brief: abnormal case: p_time_difference is NULL
    memset(&time2, 0x00, sizeof(st_osal_time_t));
    memset(&time1, 0x00, sizeof(st_osal_time_t));
    clock_type  = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(clock_type, &time1), OSAL_RETURN_OK);
    time2.tv_sec = time1.tv_sec + 1;
    time2.tv_nsec = time1.tv_nsec;
    p_time_difference = NULL;
    ASSERT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&time2, &time1, p_time_difference), OSAL_RETURN_PAR) << "Test04_002_006 failed";
}
// [ClockTimeCalculateTimeDifference_TC_04_002_006]

/**
 * @page TimeClock_TEST Time Clock API Test
 * @section OsalApiStateTest_ClockTimeCalculateTimeDifference_TC_04_002_007 OsalApiStateTest.ClockTimeCalculateTimeDifference_TC_04_002_007
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_007_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_007_tci Test Case ID
 * ClockTimeCalculateTimeDifference_S2_A_TC_04_002_007
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_007_rr Related Requirement(s)
 * \#199185
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_007_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_007_tcs Test Case Summary
 * This test shall verify the state transition of Clock Time APIs:
 * - Test04_002_007: state transition error of R_OSAL_ClockTimeCalculateTimeDifference
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_007_tpts Test Procedure/Test Steps
 * -# Calculate the difference between two times using R_OSAL_ClockTimeCalculateTimeDifference
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_007_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_STATE.
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_007_testcase Test Case Example
 * @snippet test_timeclock.cpp ClockTimeCalculateTimeDifference_TC_04_002_007
 */
// [ClockTimeCalculateTimeDifference_TC_04_002_007]
TEST_F(OsalApiStateTest, ClockTimeCalculateTimeDifference_TC_04_002_007)
{
    st_osal_time_t p_time2;
    st_osal_time_t p_time1;
    osal_nano_sec_t p_time_difference;

    // Test04_002_007
    // @brief: abnormal case: state transition error
    memset(&p_time2, 0x00, sizeof(st_osal_time_t));
    memset(&p_time1, 0x00, sizeof(st_osal_time_t));
    memset(&p_time_difference, 0x00, sizeof(osal_nano_sec_t));
    ASSERT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&p_time2, &p_time1, &p_time_difference), OSAL_RETURN_STATE) << "Test04_002_007 failed";
}
// [ClockTimeCalculateTimeDifference_TC_04_002_007]

/**
 * @page TimeClock_TEST Time Clock API Test
 * @section OSALINTTESTS_ClockTimeCalculateTimeDifference_TC_04_002_008 OSALINTTESTS.ClockTimeCalculateTimeDifference_TC_04_002_008
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_008_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_008_tci Test Case ID
 * ClockTimeCalculateTimeDifference_S1_N_TC_04_002_008
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_008_rr Related Requirement(s)
 * \#199185, \#208002
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_008_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_008_tcs Test Case Summary
 * @subsubsection ClockTimeCalculateTimeDifference_TC_04_002_008_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Setup required memory for interrupt generation
 * - Register callack function and enable interrupt
 * - Wait for interrupt callback finish
 * @subsubsection ClockTimeCalculateTimeDifference_TC_04_002_008_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * @subsubsection ClockTimeCalculateTimeDifference_TC_04_002_008_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality of OSAL API in interrupt context with normal condition.
 * - Disable and clear interrupt status
 * - Test04_002_008: Calculate the different time between "p_time2" and "p_time1" in ISR (p_time2 bigger than p_time1)
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_008_tpts Test Procedure/Test Steps
 * @subsubsection ClockTimeCalculateTimeDifference_TC_04_002_008_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Allocate required memory for generate interrupt from hardware and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function by using R_OSAL_InterruptRegisterIsr
 * -# Enable interrupt by using R_OSAL_InterruptEnableIsr
 * -# Setup hardware specific register to generate the interrupt by calling to setup_hw_register
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection ClockTimeCalculateTimeDifference_TC_04_002_008_post_tpts Test Procedure/Test Steps of Post-process
 * -# Unregister interrupt by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * @subsubsection ClockTimeCalculateTimeDifference_TC_04_002_008_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Calculate the difference between two times using R_OSAL_ClockTimeCalculateTimeDifference with the argument as below:
 *      - "p_time2" is not NULL
 *      - "p_time1" is not NULL is less than time2
 *      - "p_time_difference" is not NULL
 * -# Send Message interrupt of context finished by using R_OSAL_MqSendForTimePeriod
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_008_ereb Expected Result/Behavior
 * @subsubsection ClockTimeCalculateTimeDifference_TC_04_002_008_pre_ereb Expected Result/Behavior of Pre-process
 * -# Allocate required memory successfully without any errors.
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * Value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK.
 * -# setup_hw_register run successfully.
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection ClockTimeCalculateTimeDifference_TC_04_002_008_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * @subsubsection ClockTimeCalculateTimeDifference_TC_04_002_008_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK.
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK.<br>
 * The value of "p_time_difference" is equal to 1000000000ns.
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_008_testcase Test Case Example
 * @snippet test_timeclock.cpp ClockTimeCalculateTimeDifference_TC_04_002_008
 * @snippet test_timeclock.cpp osal_timeclock_isr_cb_TC_04_002_008
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [ClockTimeCalculateTimeDifference_TC_04_002_008]
TEST_F(OSALINTTESTS, ClockTimeCalculateTimeDifference_TC_04_002_008)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    osal_milli_sec_t time_period = 2000;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};

/* pre-process:
 * Setup required memory for interrupt generation.
 * Register callack function and enable interrupt.
 * Wait for interrupt callback finish */
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;

    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_timeclock_isr_cb_TC_04_002_008, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory*/
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_timeclock_isr_cb_TC_04_002_008), OSAL_RETURN_OK);
    deallocate_data_memory(hndl_mmngr, buffer_obj);
}
// [ClockTimeCalculateTimeDifference_TC_04_002_008]

/**
 * @page TimeClock_TEST Time Clock API Test
 * @section OSALINTTESTS_ClockTimeCalculateTimeDifference_TC_04_002_009 OSALINTTESTS.ClockTimeCalculateTimeDifference_TC_04_002_009
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_009_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_009_tci Test Case ID
 * ClockTimeCalculateTimeDifference_S1_N_TC_04_002_009
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_009_rr Related Requirement(s)
 * \#199185, \#208002
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_009_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_009_tcs Test Case Summary
 * @subsubsection ClockTimeCalculateTimeDifference_TC_04_002_009_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Setup required memory for interrupt generation
 * - Register callack function and enable interrupt
 * - Wait for interrupt callback finish
 * @subsubsection ClockTimeCalculateTimeDifference_TC_04_002_009_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * @subsubsection ClockTimeCalculateTimeDifference_TC_04_002_009_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality of OSAL API in interrupt context with normal condition.
 * - Disable and clear interrupt status
 * - Test04_002_009: Calculate the different time between "p_time2" and "p_time1" in ISR (p_time2 equal to p_time1)
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_009_tpts Test Procedure/Test Steps
 * @subsubsection ClockTimeCalculateTimeDifference_TC_04_002_009_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Allocate required memory for generate interrupt from hardware and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function by using R_OSAL_InterruptRegisterIsr
 * -# Enable interrupt by using R_OSAL_InterruptEnableIsr
 * -# Setup hardware specific register to generate the interrupt by calling to setup_hw_register
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection ClockTimeCalculateTimeDifference_TC_04_002_009_post_tpts Test Procedure/Test Steps of Post-process
 * -# Unregister interrupt by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * @subsubsection ClockTimeCalculateTimeDifference_TC_04_002_009_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Calculate the difference between two times using R_OSAL_ClockTimeCalculateTimeDifference with the argument as below:
 *      - "p_time2" is not NULL
 *      - "p_time1" is not NULL and equal to "p_time2"
 *      - "p_time_difference" is not NULL
 * -# Send Message interrupt of context finished by using R_OSAL_MqSendForTimePeriod
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_009_ereb Expected Result/Behavior
 * @subsubsection ClockTimeCalculateTimeDifference_TC_04_002_009_pre_ereb Expected Result/Behavior of Pre-process
 * -# Allocate required memory successfully without any errors.
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * Value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK.
 * -# setup_hw_register run successfully.
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection ClockTimeCalculateTimeDifference_TC_04_002_009_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * @subsubsection ClockTimeCalculateTimeDifference_TC_04_002_009_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK.
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK.
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK.<br>
 * The value of "p_time_difference" is equal to 0.
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_009_testcase Test Case Example
 * @snippet test_timeclock.cpp ClockTimeCalculateTimeDifference_TC_04_002_009
 * @snippet test_timeclock.cpp osal_timeclock_isr_cb_TC_04_002_009
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [ClockTimeCalculateTimeDifference_TC_04_002_009]
TEST_F(OSALINTTESTS, ClockTimeCalculateTimeDifference_TC_04_002_009)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    osal_milli_sec_t time_period = 2000;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};

/* pre-process:
 * Setup required memory for interrupt generation.
 * Register callack function and enable interrupt.
 * Wait for interrupt callback finish */
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;

    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_timeclock_isr_cb_TC_04_002_009, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory*/
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_timeclock_isr_cb_TC_04_002_009), OSAL_RETURN_OK);
    deallocate_data_memory(hndl_mmngr, buffer_obj);
}
// [ClockTimeCalculateTimeDifference_TC_04_002_009]

/**
 * @page TimeClock_TEST Time Clock API Test
 * @section TimeClockApiTest_ClockTimeCalculateTimeDifference_TC_04_002_010 TimeClockApiTest.ClockTimeCalculateTimeDifference_TC_04_002_010
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_010_tsi Test Suite ID
 * TimeClockApiTest
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_010_tci Test Case ID
 * ClockTimeCalculateTimeDifference_S0_A_TC_04_002_010
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_010_rr Related Requirement(s)
 * \#199185, \#208002
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_010_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_010_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to acquire the difference between two times on abnormal condition.
 * - Test04_002_010: checking with "tv_nsec" of time1 is bigger than 999999999
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_010_tpts Test Procedure/Test Steps
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Calculate the difference between two times using R_OSAL_ClockTimeCalculateTimeDifference with the argument as below:
 *      - "p_time2" is not NULL
 *      - "p_time1" is not NULL and "tv_nsec" is bigger than 999999999
 *      - "p_time_difference" is not NULL
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_010_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_PAR.
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_010_testcase Test Case Example
 * @snippet test_timeclock.cpp ClockTimeCalculateTimeDifference_TC_04_002_010
 */
// [ClockTimeCalculateTimeDifference_TC_04_002_010]
TEST_F(TimeClockApiTest, ClockTimeCalculateTimeDifference_TC_04_002_010)
{
    st_osal_time_t time2;
    st_osal_time_t time1;
    osal_nano_sec_t time_difference;

    // Test04_002_010
    // @brief: abnormal case: tv_nsec of time1 is bigger than 999999999
    memset(&time2, 0x00, sizeof(st_osal_time_t));
    memset(&time1, 0x00, sizeof(st_osal_time_t));
    memset(&time_difference, 0x00, sizeof(osal_nano_sec_t));
    clock_type  = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(clock_type, &time2), OSAL_RETURN_OK) << "Test04_002_010 failed";
    time1.tv_sec = time2.tv_sec + 1;
    time1.tv_nsec = MAX_NSEC + 1;
    EXPECT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&time2, &time1, &time_difference), OSAL_RETURN_PAR) << "Test04_002_010 failed";
}
// [ClockTimeCalculateTimeDifference_TC_04_002_010]

/**
 * @page TimeClock_TEST Time Clock API Test
 * @section TimeClockApiTest_ClockTimeCalculateTimeDifference_TC_04_002_011 TimeClockApiTest.ClockTimeCalculateTimeDifference_TC_04_002_011
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_011_tsi Test Suite ID
 * TimeClockApiTest
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_011_tci Test Case ID
 * ClockTimeCalculateTimeDifference_S0_A_TC_04_002_011
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_011_rr Related Requirement(s)
 * \#199185, \#208002
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_011_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_011_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to acquire the difference between two times on abnormal condition.
 * - Test04_002_011: checking with "tv_nsec" of time2 is bigger than 999999999
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_011_tpts Test Procedure/Test Steps
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Calculate the difference between two times using R_OSAL_ClockTimeCalculateTimeDifference with the argument as below:
 *      - "p_time2" is not NULL and "tv_nsec" is bigger than 999999999
 *      - "p_time1" is not NULL
 *      - "p_time_difference" is not NULL
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_011_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_PAR.
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_011_testcase Test Case Example
 * @snippet test_timeclock.cpp ClockTimeCalculateTimeDifference_TC_04_002_011
 */
// [ClockTimeCalculateTimeDifference_TC_04_002_011]
TEST_F(TimeClockApiTest, ClockTimeCalculateTimeDifference_TC_04_002_011)
{
    st_osal_time_t time2;
    st_osal_time_t time1;
    osal_nano_sec_t time_difference;

    // Test04_002_011
    // @brief: abnormal case: tv_nsec of time2 is bigger than 999999999
    memset(&time2, 0x00, sizeof(st_osal_time_t));
    memset(&time1, 0x00, sizeof(st_osal_time_t));
    memset(&time_difference, 0x00, sizeof(osal_nano_sec_t));
    clock_type  = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(clock_type, &time1), OSAL_RETURN_OK) << "Test04_002_011 failed";
    time2.tv_sec = time1.tv_sec + 1;
    time2.tv_nsec = MAX_NSEC + 1;
    EXPECT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&time2, &time1, &time_difference), OSAL_RETURN_PAR) << "Test04_002_011 failed";
}
// [ClockTimeCalculateTimeDifference_TC_04_002_011]

/**
 * @page TimeClock_TEST Time Clock API Test
 * @section TimeClockApiTest_ClockTimeCalculateTimeDifference_TC_04_002_012 TimeClockApiTest.ClockTimeCalculateTimeDifference_TC_04_002_012
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_012_tsi Test Suite ID
 * TimeClockApiTest
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_012_tci Test Case ID
 * ClockTimeCalculateTimeDifference_S0_A_TC_04_002_012
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_012_rr Related Requirement(s)
 * \#199185, \#208002
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_012_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_012_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to acquire the difference between two times on abnormal condition.
 * - Test04_002_012: checking with "tv_sec" of time2 is negative number
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_012_tpts Test Procedure/Test Steps
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Calculate the difference between two times using R_OSAL_ClockTimeCalculateTimeDifference with the argument as below:
 *      - "p_time2" is not NULL and "tv_sec" is -1
 *      - "p_time1" is not NULL
 *      - "p_time_difference" is not NULL
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_012_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_PAR.
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_012_testcase Test Case Example
 * @snippet test_timeclock.cpp ClockTimeCalculateTimeDifference_TC_04_002_012
 */
// [ClockTimeCalculateTimeDifference_TC_04_002_012]
TEST_F(TimeClockApiTest, ClockTimeCalculateTimeDifference_TC_04_002_012)
{
    st_osal_time_t time2;
    st_osal_time_t time1;
    osal_nano_sec_t time_difference;

    // Test04_002_012
    // @brief: abnormal case: tv_sec of time2 is negative number
    memset(&time2, 0x00, sizeof(st_osal_time_t));
    memset(&time1, 0x00, sizeof(st_osal_time_t));
    memset(&time_difference, 0x00, sizeof(osal_nano_sec_t));
    clock_type  = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(clock_type, &time1), OSAL_RETURN_OK) << "Test04_002_012 failed";
    time2.tv_sec = -1;
    time2.tv_nsec = time1.tv_nsec;
    EXPECT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&time2, &time1, &time_difference), OSAL_RETURN_PAR) << "Test04_002_012 failed";
}
// [ClockTimeCalculateTimeDifference_TC_04_002_012]

/**
 * @page TimeClock_TEST Time Clock API Test
 * @section TimeClockApiTest_ClockTimeCalculateTimeDifference_TC_04_002_013 TimeClockApiTest.ClockTimeCalculateTimeDifference_TC_04_002_013
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_013_tsi Test Suite ID
 * TimeClockApiTest
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_013_tci Test Case ID
 * ClockTimeCalculateTimeDifference_S0_A_TC_04_002_013
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_013_rr Related Requirement(s)
 * \#199185, \#208002
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_013_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_013_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to acquire the difference between two times on abnormal condition.
 * - Test04_002_013: checking with "tv_sec" of time1 is negative number
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_013_tpts Test Procedure/Test Steps
 * -# Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Calculate the difference between two times using R_OSAL_ClockTimeCalculateTimeDifference with the argument as below:
 *      - "p_time2" is not NULL
 *      - "p_time1" is not NULL and "tv_sec" is -1
 *      - "p_time_difference" is not NULL
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_013_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_PAR.
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_013_testcase Test Case Example
 * @snippet test_timeclock.cpp ClockTimeCalculateTimeDifference_TC_04_002_013
 */
// [ClockTimeCalculateTimeDifference_TC_04_002_013]
TEST_F(TimeClockApiTest, ClockTimeCalculateTimeDifference_TC_04_002_013)
{
    st_osal_time_t time2;
    st_osal_time_t time1;
    osal_nano_sec_t time_difference;

    // Test04_002_013
    // @brief: abnormal case: tv_sec of time1 is negative number
    memset(&time2, 0x00, sizeof(st_osal_time_t));
    memset(&time1, 0x00, sizeof(st_osal_time_t));
    memset(&time_difference, 0x00, sizeof(osal_nano_sec_t));
    clock_type  = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;
    EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(clock_type, &time2), OSAL_RETURN_OK)  << "Test04_002_013 failed";
    time1.tv_sec = -1;
    time1.tv_nsec = time2.tv_nsec;
    EXPECT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&time2, &time1, &time_difference), OSAL_RETURN_PAR) << "Test04_002_013 failed";
}
// [ClockTimeCalculateTimeDifference_TC_04_002_013]

/**
 * @page TimeClock_TEST Time Clock API Test
 * @section TimeClockApiTest_ClockTimeCalculateTimeDifference_TC_04_002_014 TimeClockApiTest.ClockTimeCalculateTimeDifference_TC_04_002_014
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_014_tsi Test Suite ID
 * TimeClockApiTest
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_014_tci Test Case ID
 * ClockTimeCalculateTimeDifference_S1_N_TC_04_002_014
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_014_rr Related Requirement(s)
 * \#199185, \#208002
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_014_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_014_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to acquire the difference between two times on normal condition.
 * - Test04_002_014: "time_after.tv_sec" is less than "time_before.tv_sec". "time_after.tv_nsec" is less than "time_before.tv_nsec"
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_014_tpts Test Procedure/Test Steps
 * -# Get the time stamp "current_time" by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Calculate the difference between two times using R_OSAL_ClockTimeCalculateTimeDifference with the argument as below:
 *      - "time_after" is defined as below:
 *          -# time_after.tv_sec = current_time.tv_sec
 *          -# time_after.tv_nsec = current_time.tv_nsec
 *      - "time_before" is defined as below:
 *          -# time_before.tv_sec = current_time.tv_sec + 1
 *          -# time_before.tv_nsec = current_time.tv_nsec + 1000000
 *      - "time_difference" is not NULL
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_014_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK.<br>
 * Value of "time_difference" is equal to "-1001000000".
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_014_testcase Test Case Example
 * @snippet test_timeclock.cpp ClockTimeCalculateTimeDifference_TC_04_002_014
 */
// [ClockTimeCalculateTimeDifference_TC_04_002_014]
TEST_F(TimeClockApiTest, ClockTimeCalculateTimeDifference_TC_04_002_014)
{
    st_osal_time_t time_before;
    st_osal_time_t time_after;
    st_osal_time_t current_time;
    osal_nano_sec_t time_difference;

    // Test04_002_014
    // @brief: normal case: "time_after" is less than "time_before"
    clock_type  = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(clock_type, &current_time), OSAL_RETURN_OK);
    time_before.tv_sec = current_time.tv_sec + 1;
    time_before.tv_nsec = current_time.tv_nsec + 1000000;
    if(MAX_NSEC < time_before.tv_nsec)
    {
        time_before.tv_sec += 1;
        time_before.tv_nsec %= (MAX_NSEC + 1);
    }
    time_after.tv_sec = current_time.tv_sec;
    time_after.tv_nsec = current_time.tv_nsec;
    ASSERT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&time_after, &time_before, &time_difference), OSAL_RETURN_OK) << "Test04_002_014 failed";
    EXPECT_EQ(time_difference, -1001000000) << "Test04_002_014 failed";
}
// [ClockTimeCalculateTimeDifference_TC_04_002_014]

/**
 * @page TimeClock_TEST Time Clock API Test
 * @section TimeClockApiTest_ClockTimeCalculateTimeDifference_TC_04_002_015 TimeClockApiTest.ClockTimeCalculateTimeDifference_TC_04_002_015
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_015_tsi Test Suite ID
 * TimeClockApiTest
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_015_tci Test Case ID
 * ClockTimeCalculateTimeDifference_S1_N_TC_04_002_015
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_015_rr Related Requirement(s)
 * \#199185, \#208002
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_015_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_015_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to acquire the difference between two times on normal condition.
 * - Test04_002_015: "time_after.tv_sec" is less than "time_before.tv_sec". "time_after.tv_nsec" is greater than "time_before.tv_nsec"
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_015_tpts Test Procedure/Test Steps
 * -# Get the time stamp "current_time" by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Calculate the difference between two times using R_OSAL_ClockTimeCalculateTimeDifference with the argument as below:
 *      - "time_after" is defined as below:
 *          -# time_after.tv_sec = current_time.tv_sec
 *          -# time_after.tv_nsec = current_time.tv_nsec + 1000000
 *      - "time_before" is defined as below:
 *          -# time_before.tv_sec = current_time.tv_sec + 1
 *          -# time_before.tv_nsec = current_time.tv_nsec
 *      - "time_difference" is not NULL
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_015_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK.<br>
 * Value of "time_difference" is equal to "-999000000".
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_015_testcase Test Case Example
 * @snippet test_timeclock.cpp ClockTimeCalculateTimeDifference_TC_04_002_015
 */
// [ClockTimeCalculateTimeDifference_TC_04_002_015]
TEST_F(TimeClockApiTest, ClockTimeCalculateTimeDifference_TC_04_002_015)
{
    st_osal_time_t time_before;
    st_osal_time_t time_after;
    st_osal_time_t current_time;
    osal_nano_sec_t time_difference;

    // Test04_002_015
    // @brief: normal case: "time_after" is less than "time_before"
    clock_type  = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(clock_type, &current_time), OSAL_RETURN_OK);
    time_before.tv_sec = current_time.tv_sec + 1;
    time_before.tv_nsec = current_time.tv_nsec;
    time_after.tv_sec = current_time.tv_sec;
    time_after.tv_nsec = current_time.tv_nsec + 1000000;
    if(MAX_NSEC < time_after.tv_nsec)
    {
        time_after.tv_sec += 1;
        time_after.tv_nsec %= (MAX_NSEC + 1);
    }
    ASSERT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&time_after, &time_before, &time_difference), OSAL_RETURN_OK) << "Test04_002_015 failed";
    EXPECT_EQ(time_difference, -999000000) << "Test04_002_015 failed";
}
// [ClockTimeCalculateTimeDifference_TC_04_002_015]

/**
 * @page TimeClock_TEST Time Clock API Test
 * @section TimeClockApiTest_ClockTimeCalculateTimeDifference_TC_04_002_016 TimeClockApiTest.ClockTimeCalculateTimeDifference_TC_04_002_016
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_016_tsi Test Suite ID
 * TimeClockApiTest
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_016_tci Test Case ID
 * ClockTimeCalculateTimeDifference_S1_N_TC_04_002_016
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_016_rr Related Requirement(s)
 * \#199185, \#208002
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_016_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_016_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to acquire the difference between two times on normal condition.
 * - Test04_002_016: "time_after.tv_sec" is greater than "time_before.tv_sec". "time_after.tv_nsec" is greater than "time_before.tv_nsec"
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_016_tpts Test Procedure/Test Steps
 * -# Get the time stamp "current_time" by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Calculate the difference between two times using R_OSAL_ClockTimeCalculateTimeDifference with the argument as below:
 *      - "time_after" is defined as below:
 *          -# time_after.tv_sec = current_time.tv_sec + 1
 *          -# time_after.tv_nsec = current_time.tv_nsec + 1000000
 *      - "time_before" is defined as below:
 *          -# time_before.tv_sec = current_time.tv_sec
 *          -# time_before.tv_nsec = current_time.tv_nsec
 *      - "time_difference" is not NULL
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_016_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK.<br>
 * Value of "time_difference" is equal to "1001000000".
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_016_testcase Test Case Example
 * @snippet test_timeclock.cpp ClockTimeCalculateTimeDifference_TC_04_002_016
 */
// [ClockTimeCalculateTimeDifference_TC_04_002_016]
TEST_F(TimeClockApiTest, ClockTimeCalculateTimeDifference_TC_04_002_016)
{
    st_osal_time_t time_before;
    st_osal_time_t time_after;
    st_osal_time_t current_time;
    osal_nano_sec_t time_difference;

    // Test04_002_016
    // @brief: normal case: time_after greater than time_before
    clock_type  = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(clock_type, &current_time), OSAL_RETURN_OK);
    time_before.tv_sec = current_time.tv_sec;
    time_before.tv_nsec = current_time.tv_nsec;
    time_after.tv_sec = current_time.tv_sec + 1;
    time_after.tv_nsec = current_time.tv_nsec + 1000000;
    if(MAX_NSEC < time_after.tv_nsec)
    {
        time_after.tv_sec += 1;
        time_after.tv_nsec %= (MAX_NSEC + 1);
    }
    ASSERT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&time_after, &time_before, &time_difference), OSAL_RETURN_OK) << "Test04_002_016 failed";
    EXPECT_EQ(time_difference, 1001000000) << "Test04_002_016 failed";
}
// [ClockTimeCalculateTimeDifference_TC_04_002_016]

/**
 * @page TimeClock_TEST Time Clock API Test
 * @section TimeClockApiTest_ClockTimeCalculateTimeDifference_TC_04_002_017 TimeClockApiTest.ClockTimeCalculateTimeDifference_TC_04_002_017
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_017_tsi Test Suite ID
 * TimeClockApiTest
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_017_tci Test Case ID
 * ClockTimeCalculateTimeDifference_S1_N_TC_04_002_017
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_017_rr Related Requirement(s)
 * \#199185, \#208002
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_017_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_017_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to acquire the difference between two times on normal condition.
 * - Test04_002_017: "time_after.tv_sec" is greater than "time_before.tv_sec". "time_after.tv_nsec" is less than "time_before.tv_nsec"
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_017_tpts Test Procedure/Test Steps
 * -# Get the time stamp "current_time" by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Calculate the difference between two times using R_OSAL_ClockTimeCalculateTimeDifference with the argument as below:
 *      - "time_after" is defined as below:
 *          -# time_after.tv_sec = current_time.tv_sec + 1
 *          -# time_after.tv_nsec = current_time.tv_nsec
 *      - "time_before" is defined as below:
 *          -# time_before.tv_sec = current_time.tv_sec
 *          -# time_before.tv_nsec = current_time.tv_nsec + 1000000
 *      - "time_difference" is not NULL
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_017_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK.<br>
 * Value of "time_difference" is equal to "999000000".
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_017_testcase Test Case Example
 * @snippet test_timeclock.cpp ClockTimeCalculateTimeDifference_TC_04_002_017
 */
// [ClockTimeCalculateTimeDifference_TC_04_002_017]
TEST_F(TimeClockApiTest, ClockTimeCalculateTimeDifference_TC_04_002_017)
{
    st_osal_time_t time_before;
    st_osal_time_t time_after;
    st_osal_time_t current_time;
    osal_nano_sec_t time_difference;

    // Test04_002_017
    // @brief: normal case: time_after greater than time_before
    clock_type  = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(clock_type, &current_time), OSAL_RETURN_OK);
    time_before.tv_sec = current_time.tv_sec;
    time_before.tv_nsec = current_time.tv_nsec + 1000000;
    if(MAX_NSEC < time_before.tv_nsec)
    {
        time_before.tv_sec += 1;
        time_before.tv_nsec %= (MAX_NSEC + 1);
    }
    time_after.tv_sec = current_time.tv_sec + 1;
    time_after.tv_nsec = current_time.tv_nsec;
    ASSERT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&time_after, &time_before, &time_difference), OSAL_RETURN_OK) << "Test04_002_017 failed";
    EXPECT_EQ(time_difference, 999000000) << "Test04_002_017 failed";
}
// [ClockTimeCalculateTimeDifference_TC_04_002_017]

/**
 * @page TimeClock_TEST Time Clock API Test
 * @section TimeClockApiTest_ClockTimeCalculateTimeDifference_TC_04_002_018 TimeClockApiTest.ClockTimeCalculateTimeDifference_TC_04_002_018
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_018_tsi Test Suite ID
 * TimeClockApiTest
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_018_tci Test Case ID
 * ClockTimeCalculateTimeDifference_S1_N_TC_04_002_018
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_018_rr Related Requirement(s)
 * \#199185, \#208002
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_018_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_018_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to acquire the difference between two times on normal condition.
 * - Test04_002_018: "time_after.tv_sec" is equal to "time_before.tv_sec". "time_after.tv_nsec" is less than "time_before.tv_nsec"
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_018_tpts Test Procedure/Test Steps
 * -# Get the time stamp "current_time" by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Calculate the difference between two times using R_OSAL_ClockTimeCalculateTimeDifference with the argument as below:
 *      - "time_after" is defined as below:
 *          -# time_after.tv_sec = current_time.tv_sec
 *          -# time_after.tv_nsec = current_time.tv_nsec
 *      - "time_before" is defined as below:
 *          -# time_before.tv_sec = current_time.tv_sec
 *          -# time_before.tv_nsec = current_time.tv_nsec + 1000000
 *      - "time_difference" is not NULL
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_018_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK.<br>
 * Value of "time_difference" is equal to "-1000000".
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_018_testcase Test Case Example
 * @snippet test_timeclock.cpp ClockTimeCalculateTimeDifference_TC_04_002_018
 */
// [ClockTimeCalculateTimeDifference_TC_04_002_018]
TEST_F(TimeClockApiTest, ClockTimeCalculateTimeDifference_TC_04_002_018)
{
    st_osal_time_t time_before;
    st_osal_time_t time_after;
    st_osal_time_t current_time;
    osal_nano_sec_t time_difference;

    // Test04_002_018
    // @brief: normal case: "time_after" is less than "time_before"
    clock_type  = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(clock_type, &current_time), OSAL_RETURN_OK);
    time_before.tv_sec = current_time.tv_sec;
    time_before.tv_nsec = current_time.tv_nsec + 1000000;
    if(MAX_NSEC < time_before.tv_nsec)
    {
        time_before.tv_sec += 1;
        time_before.tv_nsec %= (MAX_NSEC + 1);
    }
    time_after.tv_sec = current_time.tv_sec;
    time_after.tv_nsec = current_time.tv_nsec;
    ASSERT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&time_after, &time_before, &time_difference), OSAL_RETURN_OK) << "Test04_002_018 failed";
    EXPECT_EQ(time_difference, -1000000) << "Test04_002_018 failed";
}
// [ClockTimeCalculateTimeDifference_TC_04_002_018]

/**
 * @page TimeClock_TEST Time Clock API Test
 * @section TimeClockApiTest_ClockTimeCalculateTimeDifference_TC_04_002_019 TimeClockApiTest.ClockTimeCalculateTimeDifference_TC_04_002_019
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_019_tsi Test Suite ID
 * TimeClockApiTest
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_019_tci Test Case ID
 * ClockTimeCalculateTimeDifference_S1_N_TC_04_002_019
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_019_rr Related Requirement(s)
 * \#199185, \#208002
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_019_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_019_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to acquire the difference between two times on normal condition.
 * - Test04_002_019: "time_after.tv_sec" is equal to "time_before.tv_sec". "time_after.tv_nsec" is greater than "time_before.tv_nsec"
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_019_tpts Test Procedure/Test Steps
 * -# Get the time stamp "current_time" by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 * -# Calculate the difference between two times using R_OSAL_ClockTimeCalculateTimeDifference with the argument as below:
 *      - "time_after" is defined as below:
 *          -# time_after.tv_sec = current_time.tv_sec
 *          -# time_after.tv_nsec = current_time.tv_nsec + 1000000
 *      - "time_before" is defined as below:
 *          -# time_before.tv_sec = current_time.tv_sec
 *          -# time_before.tv_nsec = current_time.tv_nsec
 *      - "time_difference" is not NULL
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_019_ereb Expected Result/Behavior
 * -# R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 * -# R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK.<br>
 * Value of "time_difference" is equal to "1000000".
 * @subsection ClockTimeCalculateTimeDifference_TC_04_002_019_testcase Test Case Example
 * @snippet test_timeclock.cpp ClockTimeCalculateTimeDifference_TC_04_002_019
 */
// [ClockTimeCalculateTimeDifference_TC_04_002_019]
TEST_F(TimeClockApiTest, ClockTimeCalculateTimeDifference_TC_04_002_019)
{
    st_osal_time_t time_before;
    st_osal_time_t time_after;
    st_osal_time_t current_time;
    osal_nano_sec_t time_difference;

    // Test04_002_019
    // @brief: normal case: time_after greater than time_before
    clock_type  = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;
    ASSERT_EQ(R_OSAL_ClockTimeGetTimeStamp(clock_type, &current_time), OSAL_RETURN_OK);
    time_before.tv_sec = current_time.tv_sec;
    time_before.tv_nsec = current_time.tv_nsec;
    time_after.tv_sec = current_time.tv_sec;
    time_after.tv_nsec = current_time.tv_nsec + 1000000;
    if(MAX_NSEC < time_after.tv_nsec)
    {
        time_after.tv_sec += 1;
        time_after.tv_nsec %= (MAX_NSEC + 1);
    }

    ASSERT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&time_after, &time_before, &time_difference), OSAL_RETURN_OK) << "Test04_002_019 failed";
    EXPECT_EQ(time_difference, 1000000) << "Test04_002_019 failed";
}
// [ClockTimeCalculateTimeDifference_TC_04_002_019]
