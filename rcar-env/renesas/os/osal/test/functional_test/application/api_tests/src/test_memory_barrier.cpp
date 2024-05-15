/***********************************************************************************************************************
* Copyright [2021-2022] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    test_memory_barrier.cpp
* Version :      3.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL ThreadSync Manager API Functional Test (From the perspective of OSAL API Specification)
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           2.0.0     29.10.2021    Create New
 *           2.0.1     18.11.2021    Fix Typo
 *           3.0.0     08.03.2022    Apply new/updated device definition related to interrupt context test
 *                                   Apply updated allocate_setup_data_memory, setup_hw_register and deallocate_data_memory
***********************************************************************************************************************/
#include "test_common.hpp"
#if !defined(OSAL_TEST_FRAMEWORK)
using namespace std;
#else
using namespace OsalFWHelper;
#endif

/* armclang will be warnig, when specified OsalApiStateTest to TEST_F */
/* W.A Derive a class and replace it by define */
class OsalApiStateTest_MB : public OsalApiStateTest
{
    /* There is no need for member variables and member functions.
       We added them just for the sake of adding a test class. */
};
#define OsalApiStateTest OsalApiStateTest_MB

/* armclang will be warnig, when specified OSALINTTESTS to TEST_F */
/* W.A Derive a class and replace it by define */
class OSALINTTESTS_MB : public OSALINTTESTS
{
    /* There is no need for member variables and member functions.
       We added them just for the sake of adding a test class. */
};
#define OSALINTTESTS OSALINTTESTS_MB

/**
 * @page ThreadSyncMemoryBarrierApiTest_TEST ThreadSync Memory Barrier API Test
 * @test
 * @test @link ThreadSyncMemoryBarrierApiTest_ThsyncMemoryBarrier_TC_02_013_001 @endlink
 * @test @link OsalApiStateTest_ThsyncMemoryBarrier_TC_02_013_002 @endlink
 * @test @link ThreadSyncMemoryBarrierApiTest_ThsyncMemoryBarrier_TC_02_013_003 @endlink
 * @test @link OSALINTTESTS_ThsyncMemoryBarrier_TC_02_013_004 @endlink
 * @section ThreadSyncMemoryBarrierApiTest ThreadSyncMemoryBarrierApiTest function
 * @subsection ThreadSyncMemoryBarrierApiTest_SetUp ThreadSyncMemoryBarrierApiTest Setup
 * Common to Test Suit of ThreadSyncMemoryBarrierApiTest, prepare process to be executed before executing the test. <br>
 * For example:
 * @snippet test_common.hpp OSALTESTS_SetUp
 * @subsection ThreadSyncMemoryBarrierApiTest_TearDown ThreadSyncMemoryBarrierApiTest TearDown
 * Common to Test Suit of ThreadSyncMemoryBarrierApiTest, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_common.hpp OSALTESTS_TearDown
 */

class ThreadSyncMemoryBarrierApiTest : public OSALTESTS
{
    /* There is no need for member variables and member functions.
       We added them just for the sake of adding a test class. */
};

// [osal_memorybarrier_isr_cb_TC_02_013_004]
void osal_memorybarrier_isr_cb_TC_02_013_004(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    osal_milli_sec_t time_0ms = 0;
    
    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, TEST_REG_CLEAR_REGISTER, TEST_CLEAR_REGISTER_VALUE), OSAL_RETURN_OK);

    EXPECT_EQ(R_OSAL_ThsyncMemoryBarrier(), OSAL_RETURN_OK) << "Test02_013_004 failed";

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, time_0ms, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_memorybarrier_isr_cb_TC_02_013_004]

/**
 * @page ThreadSyncMemoryBarrierApiTest_TEST ThreadSync Memory Barrier API Test
 * @section ThreadSyncMemoryBarrierApiTest_ThsyncMemoryBarrier_TC_02_013_001 ThreadSyncMemoryBarrierApiTest.ThsyncMemoryBarrier_TC_02_013_001
 * @subsection ThsyncMemoryBarrier_TC_02_013_001_tsi Test Suite ID
 * ThreadSyncMemoryBarrierApiTest
 * @subsection ThsyncMemoryBarrier_TC_02_013_001_tci Test Case ID
 * ThsyncMemoryBarrier_S1_N_TC_02_013_001
 * @subsection ThsyncMemoryBarrier_TC_02_013_001_rr Related Requirement(s)
 * \#325031
 * @subsection ThsyncMemoryBarrier_TC_02_013_001_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection ThsyncMemoryBarrier_TC_02_013_001_tcs Test Case Summary
 * This test shall verify the functionality of OSAL API to issue memory barrier with normal condition:
 * - Test02_013_001: issue memory barrier.
 * @subsection ThsyncMemoryBarrier_TC_02_013_001_tpts Test Procedure/Test Steps
 * -#  issue memory barrier by using R_OSAL_ThsyncMemoryBarrier.
 * @subsection ThsyncMemoryBarrier_TC_02_013_001_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMemoryBarrier return OSAL_RETURN_OK.
 * @subsection ThsyncMemoryBarrier_TC_02_013_001_testcase Test Case Example
 * @snippet test_memory_barrier.cpp ThsyncMemoryBarrier_TC_02_013_001
 */
// [ThsyncMemoryBarrier_TC_02_013_001]
TEST_F(ThreadSyncMemoryBarrierApiTest, ThsyncMemoryBarrier_TC_02_013_001)
{
// Test02_013_001
// @brief: normal case: issue memory barrier.
    ASSERT_EQ(R_OSAL_ThsyncMemoryBarrier(), OSAL_RETURN_OK) << "Test02_013_001 failed";
}
// [ThsyncMemoryBarrier_TC_02_013_001]

/**
 * @page ThreadSyncMemoryBarrierApiTest_TEST ThreadSync Memory Barrier API Test
 * @section OsalApiStateTest_ThsyncMemoryBarrier_TC_02_013_002 OsalApiStateTest.ThsyncMemoryBarrier_TC_02_013_002
 * @subsection ThsyncMemoryBarrier_TC_02_013_002_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection ThsyncMemoryBarrier_TC_02_013_002_tci Test Case ID
 * ThsyncMemoryBarrier_S2_A_TC_02_013_002
 * @subsection ThsyncMemoryBarrier_TC_02_013_002_rr Related Requirement(s)
 * \#325031
 * @subsection ThsyncMemoryBarrier_TC_02_013_002_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection ThsyncMemoryBarrier_TC_02_013_002_tcs Test Case Summary
 * This test shall verify the state transition of ThreadSync Memory Barrier API APIs with abnormal condition:
 * - Test02_013_002: state transition error of R_OSAL_ThsyncMemoryBarrier
 * @subsection ThsyncMemoryBarrier_TC_02_013_002_tpts Test Procedure/Test Steps
 * -# issue memory barrier by using R_OSAL_ThsyncMemoryBarrier
 * @subsection ThsyncMemoryBarrier_TC_02_013_002_ereb Expected Result/Behavior
 * -# R_OSAL_ThsyncMemoryBarrier return OSAL_RETURN_STATE.
 * @subsection ThsyncMemoryBarrier_TC_02_013_002_testcase Test Case Example
 * @snippet test_memory_barrier.cpp ThsyncMemoryBarrier_TC_02_013_002
 */
// [ThsyncMemoryBarrier_TC_02_013_002]
TEST_F(OsalApiStateTest, ThsyncMemoryBarrier_TC_02_013_002)
{
    // Test02_013_002
    // @brief: abnormal case: R_OSAL_ThsyncMemoryBarrier state transition error
    EXPECT_EQ(R_OSAL_ThsyncMemoryBarrier(), OSAL_RETURN_STATE) << "Test02_013_002 failed";
}
// [ThsyncMemoryBarrier_TC_02_013_002]

/**
 * @page ThreadSyncMemoryBarrierApiTest_TEST ThreadSync Memory Barrier API Test
 * @section ThreadSyncMemoryBarrierApiTest_ThsyncMemoryBarrier_TC_02_013_003 ThreadSyncMemoryBarrierApiTest.ThsyncMemoryBarrier_TC_02_013_003
 * @subsection ThsyncMemoryBarrier_TC_02_013_003_tsi Test Suite ID
 * ThreadSyncMemoryBarrierApiTest
 * @subsection ThsyncMemoryBarrier_TC_02_013_003_tci Test Case ID
 * ThsyncMemoryBarrier_S1_N_TC_02_013_003
 * @subsection ThsyncMemoryBarrier_TC_02_013_003_rr Related Requirement(s)
 * \#325031
 * @subsection ThsyncMemoryBarrier_TC_02_013_003_pp Prerequisite/Precondition
 * OSAL is initialized and the queue has been created by R_OSAL_MqCreate.
 * @subsection ThsyncMemoryBarrier_TC_02_013_003_tcs Test Case Summary
 * This test shall measure the time execution of R_OSAL_ThsyncMemoryBarrier with normal condition:
 * - Test02_013_003: measure the time execution of R_OSAL_ThsyncMemoryBarrier
 * @subsection ThsyncMemoryBarrier_TC_02_013_003_tpts Test Procedure/Test Steps
 * -# Use the "for" loop in which we repeat 101 times with 4 steps as below:
 *      - Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *      - Issue memory barrier by using R_OSAL_ThsyncMemoryBarrier
 *      - Get the current time stamp again by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *      - Get the execution time of R_OSAL_MqSendForTimePeriod by calculating the different time using R_OSAL_ClockTimeCalculateTimeDifference.<br>
 *        Execution time is stored in an array "diff_time"
 * -# Find the median of "diff_time" array.<br>
 * Compare it with "delta_time" (typically 1msec)
 * @subsection ThsyncMemoryBarrier_TC_02_013_003_ereb Expected Result/Behavior
 * -# The expected result of 4 steps in "for" loop are as below:
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 *      - R_OSAL_ThsyncMemoryBarrier return OSAL_RETURN_OK.
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 *      - R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK.
 * -# Getting the median of "diff_time" array successfully.<br>
 * Median value need to be not exceed "delta_time"
 * @subsection ThsyncMemoryBarrier_TC_02_013_003_testcase Test Case Example
 * @snippet test_memory_barrier.cpp ThsyncMemoryBarrier_TC_02_013_003
 * @snippet test_common.hpp findMedian
 */
// [ThsyncMemoryBarrier_TC_02_013_003]
TEST_F(ThreadSyncMemoryBarrierApiTest, ThsyncMemoryBarrier_TC_02_013_003)
{
    st_osal_time_t current_time_1st, current_time_2nd;
    osal_nano_sec_t diff_time[101];
    osal_nano_sec_t delta_time = 1000000; // 1ms
    osal_nano_sec_t median = 0;
    int elements = 0;

    // Test02_013_003
    // @brief: normal case: measure execution time of R_OSAL_ThsyncMemoryBarrier
    for(int i = 0; i <= 100; i++)
    {
        EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_1st), OSAL_RETURN_OK);
        ASSERT_EQ(R_OSAL_ThsyncMemoryBarrier(), OSAL_RETURN_OK) << "Test02_013_003 failed";
        EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_2nd), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &diff_time[i]), OSAL_RETURN_OK);
    }
    elements = sizeof(diff_time) / sizeof(diff_time[0]);
    median = (osal_nano_sec_t)findMedian(diff_time, elements);
    delta_time = 1000000; //1msec
    EXPECT_LE(median, delta_time) << "Test02_013_003 failed";
}
// [ThsyncMemoryBarrier_TC_02_013_003]

/**
 * @section OSALINTTESTS_ThsyncMemoryBarrier_TC_02_013_004 OSALINTTESTS.ThsyncMemoryBarrier_TC_02_013_004
 * @subsection ThsyncMemoryBarrier_TC_02_013_004_tsi Test Suite ID
 * ThreadSyncMemoryBarrierApiTest
 * @subsection ThsyncMemoryBarrier_TC_02_013_004_tci Test Case ID
 * ThsyncMemoryBarrier_S1_N_TC_02_013_004
 * @subsection ThsyncMemoryBarrier_TC_02_013_004_rr Related Requirement(s)
 * \#325031
 * @page ThreadSyncMemoryBarrierApiTest_TEST ThreadSync Memory Barrier API Test
 * -# OSAL is initialized.
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection ThsyncMemoryBarrier_TC_02_013_004_tcs Test Case Summary
 * @subsubsection ThsyncMemoryBarrier_TC_02_013_004_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Setup required memory for interrupt generation
 * - Register callack function and enable interrupt
 * - Wait for interrupt callback finish
 * @subsubsection ThsyncMemoryBarrier_TC_02_013_004_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * @subsubsection ThsyncMemoryBarrier_TC_02_013_004_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality of OSAL API in interrupt context with normal condition.
 * - Disable and clear interrupt status
 * - Test02_013_004: Call R_OSAL_ThsyncMemoryBarrier in ISR
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection ThsyncMemoryBarrier_TC_02_013_004_tpts Test Procedure/Test Steps
 * @subsubsection ThsyncMemoryBarrier_TC_02_013_004_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Allocate required memory for generate interrupt from hardware and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function by using R_OSAL_InterruptRegisterIsr
 * -# Enable interrupt by using R_OSAL_InterruptEnableIsr
 * -# Setup hardware specific register to generate the interrupt by calling to setup_hw_register
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection ThsyncMemoryBarrier_TC_02_013_004_post_tpts Test Procedure/Test Steps of Post-process
 * -# Unregister interrupt by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * @subsubsection ThsyncMemoryBarrier_TC_02_013_004_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Call R_OSAL_ThsyncMemoryBarrier
 * -# Send Message interrupt of context finished by using R_OSAL_MqSendForTimePeriod
 * @subsection ThsyncMemoryBarrier_TC_02_013_004_ereb Expected Result/Behavior
 * @subsubsection ThsyncMemoryBarrier_TC_02_013_004_pre_ereb Expected Result/Behavior of Pre-process
 * -# Allocate required memory successfully without any errors.
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * Value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK.
 * -# setup_hw_register run successfully.
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection ThsyncMemoryBarrier_TC_02_013_004_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * @subsubsection ThsyncMemoryBarrier_TC_02_013_004_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK.
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK.
 * -# R_OSAL_ThsyncMemoryBarrier return OSAL_RETURN_OK.
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * @subsection ThsyncMemoryBarrier_TC_02_013_004_testcase Test Case Example
 * @snippet test_memory_barrier.cpp ThsyncMemoryBarrier_TC_02_013_004
 * @snippet test_memory_barrier.cpp osal_memorybarrier_isr_cb_TC_02_013_004
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [ThsyncMemoryBarrier_TC_02_013_004]
TEST_F(OSALINTTESTS, ThsyncMemoryBarrier_TC_02_013_004)
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
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_memorybarrier_isr_cb_TC_02_013_004, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory*/
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_memorybarrier_isr_cb_TC_02_013_004), OSAL_RETURN_OK);
    deallocate_data_memory(hndl_mmngr, buffer_obj);
}
// [ThsyncMemoryBarrier_TC_02_013_004]
