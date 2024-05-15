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
* File Name :    test_interrupt.cpp
* Version :      3.3.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Interrupt Manager API Functional Test (From the perspective of OSAL API Specification)
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           2.0.0     29.10.2021    Create New
 *           2.0.1     18.11.2021    Fix Typo
 *           2.1.0     15.12.2021    Add InterruptSequence_TC_12_001_023~28
 *           2.1.1     21.01.2022    Add Interrupt Mask Clear in osal_test_int_cb_TC_12_001_028
 *           2.1.2     15.02.2022    Apply new device and its setting definition
 *           3.0.0     08.03.2022    Apply new/updated device definition related to interrupt context test
 *                                   Apply updated allocate_setup_data_memory, setup_hw_register and deallocate_data_memory
 *           3.1.0     23.03.2022    Add InterruptUnregisterIsr_TC_05_003_013
 *                                   Update interrupt callback function of InterruptIsISRContext_TC_05_007_004
 *           3.2.0     04.01.2023    Add InterruptRegisterIsr_TC_05_002_012, InterruptRegisterIsr_TC_05_002_013
 *           3.3.0     01.02.2023    InterruptRegisterIsr_TC_05_002_014 and InterruptRegisterIsr_TC_05_002_015
***********************************************************************************************************************/
#include "test_common.hpp"
#if !defined(OSAL_TEST_FRAMEWORK)
using namespace std;
#else
using namespace OsalFWHelper;
#endif

/* armclang will be warnig, when specified OsalApiStateTest to TEST_F */
/* W.A Derive a class and replace it by define */
class OsalApiStateTest_INT : public OsalApiStateTest
{
    /* There is no need for member variables and member functions.
       We added them just for the sake of adding a test class. */
};
#define OsalApiStateTest OsalApiStateTest_INT

/* armclang will be warnig, when specified OSALINTTESTS to TEST_F */
/* W.A Derive a class and replace it by define */
class OSALINTTESTS_INT : public OSALINTTESTS
{
    /* There is no need for member variables and member functions.
       We added them just for the sake of adding a test class. */
};
#define OSALINTTESTS OSALINTTESTS_INT

/**
 * @page INT_TEST Interrupt API Test
 * @test
 * @test @link INTApiTest_InterruptGetNumOfIrqChannels_TC_05_001_001 @endlink
 * @test @link INTApiTest_InterruptGetNumOfIrqChannels_TC_05_001_002 @endlink
 * @test @link INTApiTest_InterruptGetNumOfIrqChannels_TC_05_001_003 @endlink
 * @test @link INTApiTest_InterruptGetNumOfIrqChannels_TC_05_001_004 @endlink
 * @test @link OsalApiStateTest_InterruptGetNumOfIrqChannels_TC_05_001_005 @endlink
 * @test @link INTApiTest_InterruptGetNumOfIrqChannels_TC_05_001_007 @endlink
 * @test @link INTApiTest_InterruptRegisterIsr_TC_05_002_001 @endlink
 * @test @link INTApiTest_InterruptRegisterIsr_TC_05_002_002 @endlink
 * @test @link INTApiTest_InterruptRegisterIsr_TC_05_002_003 @endlink
 * @test @link INTApiTest_InterruptRegisterIsr_TC_05_002_004 @endlink
 * @test @link INTApiTest_InterruptRegisterIsr_TC_05_002_005 @endlink
 * @test @link INTApiTest_InterruptRegisterIsr_TC_05_002_006 @endlink
 * @test @link INTApiTest_InterruptRegisterIsr_TC_05_002_007 @endlink
 * @test @link INTApiTest_InterruptRegisterIsr_TC_05_002_008 @endlink
 * @test @link OsalApiStateTest_InterruptRegisterIsr_TC_05_002_009 @endlink
 * @test @link OSALINTTESTS_InterruptRegisterIsr_TC_05_002_011 @endlink
 * @test @link INTApiTest_InterruptRegisterIsr_TC_05_002_012 @endlink
 * @test @link INTApiTest_InterruptRegisterIsr_TC_05_002_013 @endlink
 * @test @link INTApiTest_InterruptRegisterIsr_TC_05_002_014 @endlink
 * @test @link INTApiTest_InterruptRegisterIsr_TC_05_002_015 @endlink
 * @test @link INTApiTest_InterruptUnregisterIsr_TC_05_003_001 @endlink
 * @test @link INTApiTest_InterruptUnregisterIsr_TC_05_003_002 @endlink
 * @test @link INTApiTest_InterruptUnregisterIsr_TC_05_003_003 @endlink
 * @test @link INTApiTest_InterruptUnregisterIsr_TC_05_003_004 @endlink
 * @test @link INTApiTest_InterruptUnregisterIsr_TC_05_003_005 @endlink
 * @test @link INTApiTest_InterruptUnregisterIsr_TC_05_003_006 @endlink
 * @test @link INTApiTest_InterruptUnregisterIsr_TC_05_003_007 @endlink
 * @test @link INTApiTest_InterruptUnregisterIsr_TC_05_003_008 @endlink
 * @test @link OsalApiStateTest_InterruptUnregisterIsr_TC_05_003_009 @endlink
 * @test @link INTApiTest_InterruptUnregisterIsr_TC_05_003_011 @endlink
 * @test @link OSALINTTESTS_InterruptUnregisterIsr_TC_05_003_012 @endlink
 * @test @link INTApiTest_InterruptUnregisterIsr_TC_05_003_013 @endlink
 * @test @link INTApiTest_InterruptEnableIsr_TC_05_004_001 @endlink
 * @test @link INTApiTest_InterruptEnableIsr_TC_05_004_002 @endlink
 * @test @link INTApiTest_InterruptEnableIsr_TC_05_004_003 @endlink
 * @test @link INTApiTest_InterruptEnableIsr_TC_05_004_004 @endlink
 * @test @link INTApiTest_InterruptEnableIsr_TC_05_004_005 @endlink
 * @test @link OsalApiStateTest_InterruptEnableIsr_TC_05_004_006 @endlink
 * @test @link OSALINTTESTS_InterruptEnableIsr_TC_05_004_007 @endlink
 * @test @link INTApiTest_InterruptEnableIsr_TC_05_004_009 @endlink
 * @test @link INTApiTest_InterruptDisableIsr_TC_05_005_001 @endlink
 * @test @link INTApiTest_InterruptDisableIsr_TC_05_005_002 @endlink
 * @test @link INTApiTest_InterruptDisableIsr_TC_05_005_003 @endlink
 * @test @link INTApiTest_InterruptDisableIsr_TC_05_005_004 @endlink
 * @test @link INTApiTest_InterruptDisableIsr_TC_05_005_005 @endlink
 * @test @link OsalApiStateTest_InterruptDisableIsr_TC_05_005_006 @endlink
 * @test @link OSALINTTESTS_InterruptDisableIsr_TC_05_005_007 @endlink
 * @test @link INTApiTest_InterruptDisableIsr_TC_05_005_009 @endlink
 * @test @link INTApiTest_InterruptSetIsrPriority_TC_05_006_001 @endlink
 * @test @link INTApiTest_InterruptSetIsrPriority_TC_05_006_002 @endlink
 * @test @link INTApiTest_InterruptSetIsrPriority_TC_05_006_003 @endlink
 * @test @link INTApiTest_InterruptSetIsrPriority_TC_05_006_004 @endlink
 * @test @link INTApiTest_InterruptSetIsrPriority_TC_05_006_005 @endlink
 * @test @link INTApiTest_InterruptSetIsrPriority_TC_05_006_006 @endlink
 * @test @link INTApiTest_InterruptSetIsrPriority_TC_05_006_007 @endlink
 * @test @link INTApiTest_InterruptSetIsrPriority_TC_05_006_008 @endlink
 * @test @link OsalApiStateTest_InterruptSetIsrPriority_TC_05_006_009 @endlink
 * @test @link INTApiTest_InterruptSetIsrPriority_TC_05_006_011 @endlink
 * @test @link INTApiTest_InterruptIsISRContext_TC_05_007_001 @endlink
 * @test @link INTApiTest_InterruptIsISRContext_TC_05_007_002 @endlink
 * @test @link OsalApiStateTest_InterruptIsISRContext_TC_05_007_003 @endlink
 * @test @link OSALINTTESTS_InterruptIsISRContext_TC_05_007_004 @endlink
 * @test @link OSALINTTESTS_InterruptSequence_TC_12_001_023 @endlink
 * @test @link OSALINTTESTS_InterruptSequence_TC_12_001_024 @endlink
 * @test @link OSALINTTESTS_InterruptSequence_TC_12_001_025 @endlink
 * @test @link OSALINTTESTS_InterruptSequence_TC_12_001_026 @endlink
 * @test @link OSALINTTESTS_InterruptSequence_TC_12_001_027 @endlink
 * @test @link OSALINTTESTS_InterruptSequence_TC_12_001_028 @endlink
 * @section INTApiTest INTApiTest function
 * @subsection INTApiTest_SetUp INTApiTest Setup
 * Common to Test Suit of INTApiTest, prepare process to be executed before executing the test. <br>
 * For example:
 * @snippet test_interrupt.cpp INTApiTest_SetUp
 * @subsection INTApiTest_TearDown INTApiTest TearDown
 * @snippet test_interrupt.cpp INTApiTest_TearDown
 * Common to Test Suit of INTApiTest, prepare process to be executed after executing the test. <br>
 * For example:
 * @section OSALINTTESTS_INTApiTest OSALINTTESTS of INTApiTest function
 * @subsection OSALINTTESTS_INTApiTest_SetUp OSALINTTESTS of INTApiTest Setup
 * Common to Test Suit of OSALINTTESTS INTApiTest, prepare process to be executed before executing the test. <br>
 * This test suite generates interrupts. These process is preparation<br>
 * For example:
 * @snippet test_common.hpp OSALINTTESTS_SetUp
 * @subsection OSALINTTESTS_INTApiTest_TearDown OSALINTTESTS of INTApiTest TearDown
 * Common to Test Suit of OSALINTTESTS INTApiTest, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_common.hpp OSALINTTESTS_TearDown
 * @section OsalApiStateTest_INTApiTest OsalApiStateTest of INTApiTest function
 * @subsection OsalApiStateTest_INTApiTest_SetUp OsalApiStateTest of INTApiTest Setup
 * Common to Test Suit of OsalApiStateTest INTApiTest, prepare process to be executed before executing the test. <br>
 * For example:
 * @snippet test_common.hpp OsalApiStateTest_SetUp
 * @subsection OsalApiStateTest_INTApiTest_TearDown OsalApiStateTest of INTApiTest TearDown
 * Common to Test Suit of OsalApiStateTest INTApiTest, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_common.hpp OsalApiStateTest_TearDown
 */
class INTApiTest : public OSALTESTS
{
protected:
    static osal_device_handle_t device_handle;
    static osal_device_handle_t device_handle1;

    void SetUp() override{

    }
    void TearDown() override{

    }

// [INTApiTest_SetUp]
    static void SetUpTestSuite()
    {
        EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_INT_DEVICE_1, &device_handle), OSAL_RETURN_OK);
        EXPECT_NE(device_handle, NULL_PTR);
        EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_INT_DEVICE_2, &device_handle1), OSAL_RETURN_OK);
        EXPECT_NE(device_handle1, NULL_PTR);
    }
// [INTApiTest_SetUp]

// [INTApiTest_TearDown]
    static void TearDownTestSuite()
    {
        EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle1), OSAL_RETURN_OK);
    }
// [INTApiTest_TearDown]
};

osal_device_handle_t INTApiTest::device_handle = OSAL_DEVICE_HANDLE_INVALID;
osal_device_handle_t INTApiTest::device_handle1 = OSAL_DEVICE_HANDLE_INVALID;

// [osal_test_cb_sample]
void osal_test_cb_sample(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    (void)dev_handle;
    (void)irq_channel;
    (void)irq_arg;
}
// [osal_test_cb_sample]

// [osal_test_cb_sample1]
void osal_test_cb_sample1(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    (void)dev_handle;
    (void)irq_channel;
    (void)irq_arg;
}
// [osal_test_cb_sample1]

// [osal_test_int_cb_TC_05_002_011]
void osal_test_int_cb_TC_05_002_011(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    uint32_t reg = TEST_REG_CLEAR_REGISTER;
    uint32_t value = TEST_CLEAR_REGISTER_VALUE;
    osal_device_handle_t* p_local_dev = (osal_device_handle_t*)(local_arg->additional_arg);
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    int arg = 10;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, reg, value), OSAL_RETURN_OK);

    // Test05_002_011
    // @brief: normal case: call R_OSAL_InterruptRegisterIsr in ISR
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(*p_local_dev, TEST_IRQ_CHANNEL_DEVICE_INT, priority, osal_test_cb_sample, (void *)&arg), OSAL_RETURN_OK) << "Test05_002_011 failed";

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, 0, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_test_int_cb_TC_05_002_011]

// [osal_test_int_cb_TC_05_003_012]
void osal_test_int_cb_TC_05_003_012(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    uint32_t reg = TEST_REG_CLEAR_REGISTER;
    uint32_t value = TEST_CLEAR_REGISTER_VALUE;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, reg, value), OSAL_RETURN_OK);

    // Test05_003_012
    // @brief: normal case: call R_OSAL_InterruptUnregisterIsr itself in ISR
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(dev_handle, irq_channel, osal_test_int_cb_TC_05_003_012), OSAL_RETURN_OK) << "Test05_002_012 failed";

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, 0, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_test_int_cb_TC_05_003_012]

// [osal_test_int_cb_TC_05_004_007]
void osal_test_int_cb_TC_05_004_007(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    uint32_t reg = TEST_REG_CLEAR_REGISTER;
    uint32_t value = TEST_CLEAR_REGISTER_VALUE;
    osal_device_handle_t* p_local_dev = (osal_device_handle_t*)(local_arg->additional_arg);
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    int arg = 10;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, reg, value), OSAL_RETURN_OK);

    // Test05_004_007
    // @brief: normal case: call R_OSAL_InterruptEnableIsr for second test target device in ISR
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(*p_local_dev, TEST_IRQ_CHANNEL_DEVICE_INT, priority, osal_test_cb_sample, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(*p_local_dev, TEST_IRQ_CHANNEL_DEVICE_INT), OSAL_RETURN_OK) << "Test05_004_007 failed";

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, 0, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_test_int_cb_TC_05_004_007]

// [osal_test_int_cb_TC_05_005_007]
void osal_test_int_cb_TC_05_005_007(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    uint32_t reg = TEST_REG_CLEAR_REGISTER;
    uint32_t value = TEST_CLEAR_REGISTER_VALUE;

    // Test05_005_007
    // @brief: normal case: call R_OSAL_InterruptDisableIsr in ISR
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK) << "Test05_005_007 failed";

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, reg, value), OSAL_RETURN_OK);

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, 0, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_test_int_cb_TC_05_005_007]

// [osal_test_int_cb_TC_05_007_004]
void osal_test_int_cb_TC_05_007_004(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    bool is_isr;
    uint32_t reg = TEST_REG_CLEAR_REGISTER;
    uint32_t value = TEST_CLEAR_REGISTER_VALUE;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, reg, value), OSAL_RETURN_OK);

    // Test05_007_004
    // @brief: normal case: call R_OSAL_InterruptIsISRContext in ISR
    EXPECT_EQ(R_OSAL_InterruptIsISRContext(&is_isr), OSAL_RETURN_OK) << "Test05_007_004 failed";
    EXPECT_TRUE(is_isr) << "Test05_007_004 failed";

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, 0, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_test_int_cb_TC_05_007_004]

// [osal_int_seq_test_cb_TC_12_001]
void osal_int_seq_test_cb_TC_12_001(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    bool is_isr;
    uint32_t reg = TEST_REG_CLEAR_REGISTER;
    uint32_t value = TEST_CLEAR_REGISTER_VALUE;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, reg, value), OSAL_RETURN_OK);

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, 0, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_int_seq_test_cb_TC_12_001]

// [osal_test_int_cb_TC_12_001_027]
void osal_test_int_cb_TC_12_001_027(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    bool is_isr;
    uint32_t reg = TEST_REG_CLEAR_REGISTER;
    uint32_t value = TEST_CLEAR_REGISTER_VALUE;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, reg, value), OSAL_RETURN_OK);

    /* Unregist interrupt function */
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(dev_handle, irq_channel, osal_test_int_cb_TC_12_001_027), OSAL_RETURN_OK);

    /* Send finish interrupt */
    R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, 0, &local_arg->send_msg, sizeof(local_arg->send_msg));
}
// [osal_test_int_cb_TC_12_001_027]

// [osal_test_int_cb_TC_12_001_028]
void osal_test_int_cb_TC_12_001_028(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    bool is_isr;
    uint32_t reg = TEST_REG_CLEAR_REGISTER;
    uint32_t value = TEST_CLEAR_REGISTER_VALUE;

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, reg, value), OSAL_RETURN_OK);

    /* Not Disable Interrupt */
    R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, 0, &local_arg->send_msg, sizeof(local_arg->send_msg));
}
// [osal_test_int_cb_TC_12_001_028]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptGetNumOfIrqChannels_TC_05_001_001 INTApiTest.InterruptGetNumOfIrqChannels_TC_05_001_001
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_001_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_001_tci Test Case ID
 * InterruptGetNumOfIrqChannels_S1_N_TC_05_001_001
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_001_rr Related Requirement(s)
 * \#230924
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_001_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_001_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptGetNumOfIrqChannels on normal condition:
 * - Test05_001_001: get the number of interrupt channel of input device.
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_001_tpts Test Procedure/Test Steps
 * -# Get the number of interrupt channel corresponding with opened device handle test target device by calling to R_OSAL_InterruptGetNumOfIrqChannels
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_001_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * Value of "numOfChannel" is not equal to 0.
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_001_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptGetNumOfIrqChannels_TC_05_001_001
 */
// [InterruptGetNumOfIrqChannels_TC_05_001_001]
TEST_F(INTApiTest, InterruptGetNumOfIrqChannels_TC_05_001_001)
{
    size_t numOfChannel = 0;

    // Test05_001_001
    // @brief: normal case: get the number of interrupt channel of input device
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK) << "Test05_001_001 failed";
    EXPECT_NE(numOfChannel, 0) << "Test05_001_001 failed";
}
// [InterruptGetNumOfIrqChannels_TC_05_001_001]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptGetNumOfIrqChannels_TC_05_001_002 INTApiTest.InterruptGetNumOfIrqChannels_TC_05_001_002
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_002_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_002_tci Test Case ID
 * InterruptGetNumOfIrqChannels_S0_A_TC_05_001_002
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_002_rr Related Requirement(s)
 * \#199151, \#230924
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_002_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_002_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptGetNumOfIrqChannels on abnormal condition:
 * - Test05_001_002: get the number of interrupt channel with device handle is NULL.
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_002_tpts Test Procedure/Test Steps
 * -# Get the number of interrupt channel by calling to R_OSAL_InterruptGetNumOfIrqChannels with the argument as below:
 *      - device handle is NULL
 *      - Address of "numOfChannel"
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_002_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_HANDLE.<br>
 * Value of "numOfChannel" is equal to 0.
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_002_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptGetNumOfIrqChannels_TC_05_001_002
 */
// [InterruptGetNumOfIrqChannels_TC_05_001_002]
TEST_F(INTApiTest, InterruptGetNumOfIrqChannels_TC_05_001_002)
{
    osal_device_handle_t p_device_handle = NULL;
    size_t numOfChannel = 0;

    // Test05_001_002
    // @brief: abnormal case: device handle is NULL
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(p_device_handle, &numOfChannel), OSAL_RETURN_HANDLE) << "Test05_001_002 failed";
    EXPECT_EQ(numOfChannel, 0) << "Test05_001_002 failed";
}
// [InterruptGetNumOfIrqChannels_TC_05_001_002]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptGetNumOfIrqChannels_TC_05_001_003 INTApiTest.InterruptGetNumOfIrqChannels_TC_05_001_003
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_003_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_003_tci Test Case ID
 * InterruptGetNumOfIrqChannels_S2_A_TC_05_001_003
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_003_rr Related Requirement(s)
 * \#230924
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_003_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_003_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptGetNumOfIrqChannels on abnormal condition:
 * - Test05_001_003: get the number of interrupt channel with device handle has already been closed.
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_003_tpts Test Procedure/Test Steps
 * -# Close test target device by using R_OSAL_IoDeviceClose.
 * -# Get the number of interrupt channel by calling to R_OSAL_InterruptGetNumOfIrqChannels with the argument as below:
 *      - device handle which has already been closed at step 1.
 *      - Address of "numOfChannel"
 * -# Re-open test target device for the next test by using R_OSAL_IoDeviceOpen.
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_003_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_HANDLE.<br>
 * Value of "numOfChannel" is equal to 0.
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not equal to NULL_PTR.
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_003_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptGetNumOfIrqChannels_TC_05_001_003
 */
// [InterruptGetNumOfIrqChannels_TC_05_001_003]
TEST_F(INTApiTest, InterruptGetNumOfIrqChannels_TC_05_001_003)
{
    size_t numOfChannel = 0;

    // Test05_001_003
    // @brief: abnormal case: device handle is closed
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_HANDLE) << "Test05_001_003 failed";
    EXPECT_EQ(numOfChannel, 0) << "Test05_001_003 failed";
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_INT_DEVICE_1, &device_handle), OSAL_RETURN_OK);
    EXPECT_NE(device_handle, NULL_PTR);
}
// [InterruptGetNumOfIrqChannels_TC_05_001_003]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptGetNumOfIrqChannels_TC_05_001_004 INTApiTest.InterruptGetNumOfIrqChannels_TC_05_001_004
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_004_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_004_tci Test Case ID
 * InterruptGetNumOfIrqChannels_S0_A_TC_05_001_004
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_004_rr Related Requirement(s)
 * \#230924
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_004_pp Prerequisite/Precondition
 * -# OSAL is initialized.<br>
 * -# Device handle is opened test target device.
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_004_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptGetNumOfIrqChannels on abnormal condition:
 * - Test05_001_004: p_numOfChannel is NULL.
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_004_tpts Test Procedure/Test Steps
 * -# Get the number of interrupt channel by calling to R_OSAL_InterruptGetNumOfIrqChannels with the argument as below:
 *      - device handle of test target device.
 *      - NULL.
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_004_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_PAR.
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_004_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptGetNumOfIrqChannels_TC_05_001_004
 */
// [InterruptGetNumOfIrqChannels_TC_05_001_004]
TEST_F(INTApiTest, InterruptGetNumOfIrqChannels_TC_05_001_004)
{
    size_t* p_numOfChannel = NULL;

    // Test05_001_004
    // @brief: abnormal case: p_numOfChannel is NULL
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, p_numOfChannel), OSAL_RETURN_PAR) << "Test05_001_004 failed";
}
// [InterruptGetNumOfIrqChannels_TC_05_001_004]

/**
 * @page INT_TEST Interrupt API Test
 * @section OsalApiStateTest_InterruptGetNumOfIrqChannels_TC_05_001_005 OsalApiStateTest.InterruptGetNumOfIrqChannels_TC_05_001_005
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_005_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_005_tci Test Case ID
 * InterruptGetNumOfIrqChannels_S2_A_TC_05_001_005
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_005_rr Related Requirement(s)
 * \#230924
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_005_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_005_tcs Test Case Summary
 * This test shall verify the state transition of Clock Time APIs on abnormal condition:
 * - Test05_001_005: state transition error of R_OSAL_InterruptGetNumOfIrqChannels
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_005_tpts Test Procedure/Test Steps
 * -# Get the number of interrupt channel corresponding with opened device handle test target device by calling to R_OSAL_InterruptGetNumOfIrqChannels
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_005_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_STATE.<br>
 * Value of "numOfChannel" is equal to 0.
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_005_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptGetNumOfIrqChannels_TC_05_001_005
 */
// [InterruptGetNumOfIrqChannels_TC_05_001_005]
TEST_F(OsalApiStateTest, InterruptGetNumOfIrqChannels_TC_05_001_005)
{
    size_t numOfChannel = 0;
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;

    // Test05_001_005
    // @brief: abnormal case: state transition error of R_OSAL_InterruptGetNumOfIrqChannels
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_STATE) << "Test05_001_005 failed";
    EXPECT_EQ(numOfChannel, 0) << "Test05_001_005 failed";
}
// [InterruptGetNumOfIrqChannels_TC_05_001_005]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptGetNumOfIrqChannels_TC_05_001_007 INTApiTest.InterruptGetNumOfIrqChannels_TC_05_001_007
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_007_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_007_tci Test Case ID
 * InterruptGetNumOfIrqChannels_S1_N_TC_05_001_007
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_007_rr Related Requirement(s)
 * \#230924
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_007_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_007_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptGetNumOfIrqChannels on normal condition:
 * - Test05_001_007: R_OSAL_InterruptGetNumOfIrqChannels confirm with test target device which does not have interrupt channel.
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_007_tpts Test Procedure/Test Steps
 * -# Get the number of interrupt channel by calling to R_OSAL_InterruptGetNumOfIrqChannels with the argument as below:
 *      - device handle of test target device
 *      - Address of "numOfChannel"
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_007_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * Value of "numOfChannel" is equal to 0.
 * @subsection InterruptGetNumOfIrqChannels_TC_05_001_007_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptGetNumOfIrqChannels_TC_05_001_007
 */
// [InterruptGetNumOfIrqChannels_TC_05_001_007]
TEST_F(INTApiTest, InterruptGetNumOfIrqChannels_TC_05_001_007)
{
    size_t numOfChannel = 0;

    // Test05_001_007
    // @brief: normal case: R_OSAL_InterruptGetNumOfIrqChannels confirm with test target device which does not have interrupt channel
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle1, &numOfChannel), OSAL_RETURN_OK) << "Test05_001_007 failed";
    EXPECT_EQ(numOfChannel, 0) << "Test05_001_007 failed";
}
// [InterruptGetNumOfIrqChannels_TC_05_001_007]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptRegisterIsr_TC_05_002_001 INTApiTest.InterruptRegisterIsr_TC_05_002_001
 * @subsection InterruptRegisterIsr_TC_05_002_001_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptRegisterIsr_TC_05_002_001_tci Test Case ID
 * InterruptRegisterIsr_S1_BN_TC_05_002_001
 * @subsection InterruptRegisterIsr_TC_05_002_001_rr Related Requirement(s)
 * \#199188
 * @subsection InterruptRegisterIsr_TC_05_002_001_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptRegisterIsr_TC_05_002_001_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptRegisterIsr APIs on normal condition:
 * - Test05_002_001: register interrupt for device corresponding with valid interrupt channel.
 * - post-process: unregister interrupt channel TEST_IRQ_CHANNEL_DEVICE_VALID
 * @subsection InterruptRegisterIsr_TC_05_002_001_tpts Test Procedure/Test Steps
 * -# Get the number of interrupt channel corresponding with opened device handle test target device by calling to R_OSAL_InterruptGetNumOfIrqChannels.
 * -# Register interrupt callback "osal_test_cb_sample" for an interrupt occurs from interrupt channel TEST_IRQ_CHANNEL_DEVICE_VALID of test target device by calling to R_OSAL_InterruptRegisterIsr.
 * -# Unregister interrupt callback "osal_test_cb_sample" registered at step 2 by calling to R_OSAL_InterruptUnregisterIsr.
 * @subsection InterruptRegisterIsr_TC_05_002_001_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * Value of "numOfChannel" is bigger than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK.
 * @subsection InterruptRegisterIsr_TC_05_002_001_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptRegisterIsr_TC_05_002_001
 * @snippet test_interrupt.cpp osal_test_cb_sample
 */
// [InterruptRegisterIsr_TC_05_002_001]
TEST_F(INTApiTest, InterruptRegisterIsr_TC_05_002_001)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE10;
    int arg = 10;

    // Test05_002_001
    // @brief: normal case: register interrupt for device corresponding with interrupt channel
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, priority, osal_test_cb_sample, (void *)&arg), OSAL_RETURN_OK) << "Test05_002_001 failed";

    // post-process: Unregister interrupt
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, osal_test_cb_sample), OSAL_RETURN_OK) << "post-process failed";
}
// [InterruptRegisterIsr_TC_05_002_001]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptRegisterIsr_TC_05_002_002 INTApiTest.InterruptRegisterIsr_TC_05_002_002
 * @subsection InterruptRegisterIsr_TC_05_002_002_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptRegisterIsr_TC_05_002_002_tci Test Case ID
 * InterruptRegisterIsr_S2_A_TC_05_002_002
 * @subsection InterruptRegisterIsr_TC_05_002_002_rr Related Requirement(s)
 * \#199188
 * @subsection InterruptRegisterIsr_TC_05_002_002_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptRegisterIsr_TC_05_002_002_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptRegisterIsr APIs with abnormal condition:
 * - Test05_002_002: register interrupt for device does not have its own interrupt channel.
 * @subsection InterruptRegisterIsr_TC_05_002_002_tpts Test Procedure/Test Steps
 * -# Get the number of interrupt channel corresponding with opened device handle test target device by calling to R_OSAL_InterruptGetNumOfIrqChannels.
 * -# Register an interrupt callback by using R_OSAL_InterruptRegisterIsr with the arguments as below:
 *      - Device handle of test target device.<br>
 *        test target device is device which does not have its own interrupt channel.
 *      - Interrupt channel is 0.
 *      - Priority is OSAL_INTERRUPT_PRIORITY_TYPE10
 *      - Callback function is "osal_test_cb_sample"
 *      - Address of "arg"
 * @subsection InterruptRegisterIsr_TC_05_002_002_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * Value of "numOfChannel" is equal to 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_HANDLE.
 * @subsection InterruptRegisterIsr_TC_05_002_002_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptRegisterIsr_TC_05_002_002
 * @snippet test_interrupt.cpp osal_test_cb_sample
 */
// [InterruptRegisterIsr_TC_05_002_002]
TEST_F(INTApiTest, InterruptRegisterIsr_TC_05_002_002)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE10;
    int arg = 10;

    // Test05_002_002
    // @brief: abnormal case: register interrupt for device does not have its own interrupt channel
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle1, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_EQ(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle1, TEST_IRQ_CHANNEL_DEVICE_INVALID, priority, osal_test_cb_sample, (void *)&arg), OSAL_RETURN_HANDLE) << "Test05_002_002 failed";
}
// [InterruptRegisterIsr_TC_05_002_002]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptRegisterIsr_TC_05_002_003 INTApiTest.InterruptRegisterIsr_TC_05_002_003
 * @subsection InterruptRegisterIsr_TC_05_002_003_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptRegisterIsr_TC_05_002_003_tci Test Case ID
 * InterruptRegisterIsr_S0_A_TC_05_002_003
 * @subsection InterruptRegisterIsr_TC_05_002_003_rr Related Requirement(s)
 * \#199151, \#199188
 * @subsection InterruptRegisterIsr_TC_05_002_003_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * @subsection InterruptRegisterIsr_TC_05_002_003_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptRegisterIsr APIs with abnormal condition:
 * - Test05_002_003: register interrupt for device handle is NULL.
 * @subsection InterruptRegisterIsr_TC_05_002_003_tpts Test Procedure/Test Steps
 * -# Register an interrupt callback by using R_OSAL_InterruptRegisterIsr with the arguments as below:
 *      - Device handle is NULL
 *      - Interrupt channel TEST_IRQ_CHANNEL_DEVICE_VALID
 *      - Priority is OSAL_INTERRUPT_PRIORITY_TYPE10
 *      - Callback function is "osal_test_cb_sample"
 *      - Address of "arg"
 * @subsection InterruptRegisterIsr_TC_05_002_003_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_HANDLE.
 * @subsection InterruptRegisterIsr_TC_05_002_003_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptRegisterIsr_TC_05_002_003
 * @snippet test_interrupt.cpp osal_test_cb_sample
 */
// [InterruptRegisterIsr_TC_05_002_003]
TEST_F(INTApiTest, InterruptRegisterIsr_TC_05_002_003)
{
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE10;
    int arg = 10;
    osal_device_handle_t p_device_handle = NULL;

    // Test05_002_003
    // @brief: abnormal case: register interrupt for device handle is NULL
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(p_device_handle, TEST_IRQ_CHANNEL_DEVICE_INVALID, priority, osal_test_cb_sample, (void *)&arg), OSAL_RETURN_HANDLE) << "Test05_002_003 failed";
}
// [InterruptRegisterIsr_TC_05_002_003]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptRegisterIsr_TC_05_002_004 INTApiTest.InterruptRegisterIsr_TC_05_002_004
 * @subsection InterruptRegisterIsr_TC_05_002_004_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptRegisterIsr_TC_05_002_004_tci Test Case ID
 * InterruptRegisterIsr_S2_A_TC_05_002_004
 * @subsection InterruptRegisterIsr_TC_05_002_004_rr Related Requirement(s)
 * \#199188
 * @subsection InterruptRegisterIsr_TC_05_002_004_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptRegisterIsr_TC_05_002_004_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptRegisterIsr APIs with abnormal condition:
 * - Test05_002_004: register interrupt for device handle has already been closed.
 * @subsection InterruptRegisterIsr_TC_05_002_004_tpts Test Procedure/Test Steps
 * -# Close test target device by using R_OSAL_IoDeviceClose.
 * -# Register an interrupt callback by using R_OSAL_InterruptRegisterIsr with the arguments as below:
 *      - Device handle has already closed at step 1
 *      - Interrupt channel TEST_IRQ_CHANNEL_DEVICE_VALID
 *      - Priority is OSAL_INTERRUPT_PRIORITY_TYPE10
 *      - Callback function is "osal_test_cb_sample"
 *      - Address of "arg"
 * -# Re-open test target device for the next test cases by using R_OSAL_IoDeviceOpen.
 * @subsection InterruptRegisterIsr_TC_05_002_004_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_HANDLE.
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * @subsection InterruptRegisterIsr_TC_05_002_004_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptRegisterIsr_TC_05_002_004
 * @snippet test_interrupt.cpp osal_test_cb_sample
 */
// [InterruptRegisterIsr_TC_05_002_004]
TEST_F(INTApiTest, InterruptRegisterIsr_TC_05_002_004)
{
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE10;
    int arg = 10;

    // Test05_002_004
    // @brief: abnormal case: register interrupt for device handle is closed
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, priority, osal_test_cb_sample, (void *)&arg), OSAL_RETURN_HANDLE) << "Test05_002_004 failed";
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_INT_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "Test05_002_004 failed";
    EXPECT_NE(device_handle, NULL_PTR) << "Test05_002_004 failed";
}
// [InterruptRegisterIsr_TC_05_002_004]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptRegisterIsr_TC_05_002_005 INTApiTest.InterruptRegisterIsr_TC_05_002_005
 * @subsection InterruptRegisterIsr_TC_05_002_005_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptRegisterIsr_TC_05_002_005_tci Test Case ID
 * InterruptRegisterIsr_S0_BA_TC_05_002_005
 * @subsection InterruptRegisterIsr_TC_05_002_005_rr Related Requirement(s)
 * \#199188
 * @subsection InterruptRegisterIsr_TC_05_002_005_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptRegisterIsr_TC_05_002_005_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptRegisterIsr APIs with abnormal condition:
 * - Test05_002_005: register interrupt with interrupt channel is invalid.
 * @subsection InterruptRegisterIsr_TC_05_002_005_tpts Test Procedure/Test Steps
 * -# Get the number of interrupt channel corresponding with opened device handle test target device by calling to R_OSAL_InterruptGetNumOfIrqChannels.
 * -# Register an interrupt callback by using R_OSAL_InterruptRegisterIsr with the arguments as below:
 *      - Device handle of test target device
 *      - Interrupt channel is out-of-valid range: [0 -- "numOfChannel").<br>
 *        "numOfChannel" got at step 1.
 *      - Priority is OSAL_INTERRUPT_PRIORITY_TYPE10
 *      - Callback function is "osal_test_cb_sample"
 *      - Address of "arg"
 * @subsection InterruptRegisterIsr_TC_05_002_005_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * Value of "numOfChannel" is bigger than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_PAR.
 * @subsection InterruptRegisterIsr_TC_05_002_005_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptRegisterIsr_TC_05_002_005
 * @snippet test_interrupt.cpp osal_test_cb_sample
 */
// [InterruptRegisterIsr_TC_05_002_005]
TEST_F(INTApiTest, InterruptRegisterIsr_TC_05_002_005)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE10;
    int arg = 10;

    // Test05_002_005
    // @brief: abnormal case: register interrupt with irq_channel is invalid.
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, numOfChannel, priority, osal_test_cb_sample, (void *)&arg), OSAL_RETURN_PAR) << "Test05_002_005 failed";
}
// [InterruptRegisterIsr_TC_05_002_005]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptRegisterIsr_TC_05_002_006 INTApiTest.InterruptRegisterIsr_TC_05_002_006
 * @subsection InterruptRegisterIsr_TC_05_002_006_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptRegisterIsr_TC_05_002_006_tci Test Case ID
 * InterruptRegisterIsr_S0_A_TC_05_002_006
 * @subsection InterruptRegisterIsr_TC_05_002_006_rr Related Requirement(s)
 * \#199186, \#199188
 * @subsection InterruptRegisterIsr_TC_05_002_006_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptRegisterIsr_TC_05_002_006_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptRegisterIsr APIs with abnormal condition:
 * - Test05_002_006: register interrupt with callback function is NULL.
 * @subsection InterruptRegisterIsr_TC_05_002_006_tpts Test Procedure/Test Steps
 * -# Get the number of interrupt channel corresponding with opened device handle test target device by calling to R_OSAL_InterruptGetNumOfIrqChannels.
 * -# Register an interrupt callback by using R_OSAL_InterruptRegisterIsr with the arguments as below:
 *      - Device handle of test target device
 *      - Interrupt channel is 0.
 *      - Priority is OSAL_INTERRUPT_PRIORITY_TYPE10
 *      - Callback function is NULL
 *      - Address of "arg"
 * @subsection InterruptRegisterIsr_TC_05_002_006_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * Value of "numOfChannel" is bigger than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_PAR.
 * @subsection InterruptRegisterIsr_TC_05_002_006_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptRegisterIsr_TC_05_002_006
 */
// [InterruptRegisterIsr_TC_05_002_006]
TEST_F(INTApiTest, InterruptRegisterIsr_TC_05_002_006)
{
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE10;
    int arg = 10;
    p_osal_isr_func_t p_irqHandlerFunction = NULL;
    size_t numOfChannel = 0;

    // Test05_002_006
    // @brief: abnormal case: register interrupt with irqHandlerFunction is NULL
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK); 
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, priority, p_irqHandlerFunction, (void *)&arg), OSAL_RETURN_PAR) << "Test05_002_006 failed";
}
// [InterruptRegisterIsr_TC_05_002_006]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptRegisterIsr_TC_05_002_007 INTApiTest.InterruptRegisterIsr_TC_05_002_007
 * @subsection InterruptRegisterIsr_TC_05_002_007_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptRegisterIsr_TC_05_002_007_tci Test Case ID
 * InterruptRegisterIsr_S2_A_TC_05_002_007
 * @subsection InterruptRegisterIsr_TC_05_002_007_rr Related Requirement(s)
 * \#199188, \#199189
 * @subsection InterruptRegisterIsr_TC_05_002_007_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptRegisterIsr_TC_05_002_007_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptRegisterIsr APIs with abnormal condition:
 * - Test05_002_007: register interrupt handler which has already been enabled.
 * @subsection InterruptRegisterIsr_TC_05_002_007_tpts Test Procedure/Test Steps
 * -# Get the number of interrupt channel corresponding with opened device handle test target device by calling to R_OSAL_InterruptGetNumOfIrqChannels.
 * -# Register interrupt callback "osal_test_cb_sample" for an interrupt occurs from interrupt channel TEST_IRQ_CHANNEL_DEVICE_VALID of test target device by calling to R_OSAL_InterruptRegisterIsr.
 * -# Enable the interrupt handler which has already registered at step 2 by using R_OSAL_InterruptEnableIsr.
 * -# Register an interrupt callback by using R_OSAL_InterruptRegisterIsr with the same arguments as step 2.
 * -# Disable the interrupt handle which has already registered at step 2 by using R_OSAL_InterruptDisableIsr.
 * -# Unregister interrupt callback "osal_test_cb_sample" registered at step 2 by calling to R_OSAL_InterruptUnregisterIsr.
 * @subsection InterruptRegisterIsr_TC_05_002_007_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * Value of "numOfChannel" is bigger than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_BUSY.
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK.
 * @subsection InterruptRegisterIsr_TC_05_002_007_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptRegisterIsr_TC_05_002_007
 * @snippet test_interrupt.cpp osal_test_cb_sample
 */
// [InterruptRegisterIsr_TC_05_002_007]
TEST_F(INTApiTest, InterruptRegisterIsr_TC_05_002_007)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE10;
    int arg = 10;

    // Test05_002_007
    // @brief: abnormal case: register interrupt channel which has already been enabled
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, priority, osal_test_cb_sample, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, priority, osal_test_cb_sample, (void *)&arg), OSAL_RETURN_BUSY) << "Test05_002_007 failed";
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, osal_test_cb_sample), OSAL_RETURN_OK);
}
// [InterruptRegisterIsr_TC_05_002_007]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptRegisterIsr_TC_05_002_008 INTApiTest.InterruptRegisterIsr_TC_05_002_008
 * @subsection InterruptRegisterIsr_TC_05_002_008_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptRegisterIsr_TC_05_002_008_tci Test Case ID
 * InterruptRegisterIsr_S4_A_TC_05_002_008
 * @subsection InterruptRegisterIsr_TC_05_002_008_rr Related Requirement(s)
 * \#199188, \#199189
 * @subsection InterruptRegisterIsr_TC_05_002_008_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptRegisterIsr_TC_05_002_008_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptRegisterIsr APIs with abnormal condition:
 * - Test05_002_008: register a callback function for interrupt channel which has already been registered.
 * @subsection InterruptRegisterIsr_TC_05_002_008_tpts Test Procedure/Test Steps
 * -# Get the number of interrupt channel corresponding with opened device handle test target device by calling to R_OSAL_InterruptGetNumOfIrqChannels.
 * -# Register interrupt callback "osal_test_cb_sample" for an interrupt occurs from interrupt channel TEST_IRQ_CHANNEL_DEVICE_VALID of test target device by calling to R_OSAL_InterruptRegisterIsr.
 * -# Register an interrupt callback by using R_OSAL_InterruptRegisterIsr with the same arguments as step 2.
 * -# Unregister interrupt callback "osal_test_cb_sample" registered at step 2 by calling to R_OSAL_InterruptUnregisterIsr.
 * @subsection InterruptRegisterIsr_TC_05_002_008_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * Value of "numOfChannel" is bigger than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_BUSY.
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK.
 * @subsection InterruptRegisterIsr_TC_05_002_008_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptRegisterIsr_TC_05_002_008
 * @snippet test_interrupt.cpp osal_test_cb_sample
 */
// [InterruptRegisterIsr_TC_05_002_008]
TEST_F(INTApiTest, InterruptRegisterIsr_TC_05_002_008)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE10;
    int arg = 10;

    // Test05_002_008
    // @brief: abnormal case: register interrupt channel which has already been registered
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, priority, osal_test_cb_sample, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, priority, osal_test_cb_sample, (void *)&arg), OSAL_RETURN_BUSY) << "Test05_002_008 failed";
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, osal_test_cb_sample), OSAL_RETURN_OK);
}
// [InterruptRegisterIsr_TC_05_002_008]

/**
 * @page INT_TEST Interrupt API Test
 * @section OsalApiStateTest_InterruptRegisterIsr_TC_05_002_009 OsalApiStateTest.InterruptRegisterIsr_TC_05_002_009
 * @subsection InterruptRegisterIsr_TC_05_002_009_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection InterruptRegisterIsr_TC_05_002_009_tci Test Case ID
 * InterruptRegisterIsr_S2_A_TC_05_002_009
 * @subsection InterruptRegisterIsr_TC_05_002_009_rr Related Requirement(s)
 * \#199188
 * @subsection InterruptRegisterIsr_TC_05_002_009_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection InterruptRegisterIsr_TC_05_002_009_tcs Test Case Summary
 * This test shall verify the state transition of Clock Time APIs on abnormal condition:
 * - Test05_002_009: state transition error of R_OSAL_InterruptRegisterIsr
 * @subsection InterruptRegisterIsr_TC_05_002_009_tpts Test Procedure/Test Steps
 * -# Register interrupt callback "osal_test_cb_sample" for an interrupt occurs from interrupt channel TEST_IRQ_CHANNEL_DEVICE_VALID of test target device by calling to R_OSAL_InterruptRegisterIsr.
 * @subsection InterruptRegisterIsr_TC_05_002_009_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_STATE.
 * @subsection InterruptRegisterIsr_TC_05_002_009_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptRegisterIsr_TC_05_002_009
 * @snippet test_interrupt.cpp osal_test_cb_sample
 */
// [InterruptRegisterIsr_TC_05_002_009]
TEST_F(OsalApiStateTest, InterruptRegisterIsr_TC_05_002_009)
{
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE11;
    int arg = 10;
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;

    // Test05_002_009
    // @brief: abnormal case: state transition error of R_OSAL_InterruptRegisterIsr
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, priority, osal_test_cb_sample, (void *)&arg), OSAL_RETURN_STATE) << "Test05_002_009 failed";
}
// [InterruptRegisterIsr_TC_05_002_009]

/**
 * @page INT_TEST Interrupt API Test
 * @section OSALINTTESTS_InterruptRegisterIsr_TC_05_002_011 OSALINTTESTS.InterruptRegisterIsr_TC_05_002_011
 * @subsection InterruptRegisterIsr_TC_05_002_011_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection InterruptRegisterIsr_TC_05_002_011_tci Test Case ID
 * InterruptRegisterIsr_S3_N_TC_05_002_011
 * @subsection InterruptRegisterIsr_TC_05_002_011_rr Related Requirement(s)
 * \#199193, \#199194, \#199195, \#230924, \#276767
 * @subsection InterruptRegisterIsr_TC_05_002_011_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection InterruptRegisterIsr_TC_05_002_011_tcs Test Case Summary
 * @subsubsection InterruptRegisterIsr_TC_05_002_011_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Open second test target device to register an second test target device interrupt in the interrupt context
 * - Setup required memory for interrupt generation
 * - Register callback for first test target device function and enable interrupt
 * - Wait for interrupt callback finish
 * @subsubsection InterruptRegisterIsr_TC_05_002_011_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Disable interrupts for second test target device enabled in the interrupt context
 * - Unregister the interrupt callback function of first test target device and second test target device registered in the interrupt context 
 * - Deallocate allocated memory
 * - Close second test target device devices
 * @subsubsection InterruptRegisterIsr_TC_05_002_011_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality of the Interrupt APIs which are called in the ISR with normal condition.
 * - Disable interrupt for first test target device in ISR and clear status register
 * - Test05_002_011: Register interrupt callback function for second test target device by using R_OSAL_InterruptRegisterIsr in ISR
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection InterruptRegisterIsr_TC_05_002_011_tpts Test Procedure/Test Steps
 * @subsubsection InterruptRegisterIsr_TC_05_002_011_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Open second test target device by using R_OSAL_IoDeviceOpen
 * -# Allocate required memory for generate interrupt from first test target device and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function for first test target device by using R_OSAL_InterruptRegisterIsr
 * -# Enable interrupt for first test target device by using R_OSAL_InterruptEnableIsr
 * -# Setup first test target device specific register to generate the interrupt by calling to setup_hw_register
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection InterruptRegisterIsr_TC_05_002_011_post_tpts Test Procedure/Test Steps of Post-process
 * -# Disable interrupt for second test target device by using R_OSAL_InterruptDisableIsr
 * -# Unregister interrupt for first test target device by using R_OSAL_InterruptUnregisterIsr
 * -# Unregister interrupt for second test target device by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * -# Close second test target device device by using R_OSAL_IoDeviceClose
 * @subsubsection InterruptRegisterIsr_TC_05_002_011_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt for first test target device immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Register the interrupt callback function for second test target device by using R_OSAL_InterruptRegisterIsr
 * -# Send Message interrupt of context finished by using R_OSAL_MqSendForTimePeriod
 * @subsection InterruptRegisterIsr_TC_05_002_011_ereb Expected Result/Behavior
 * @subsubsection InterruptRegisterIsr_TC_05_002_011_pre_ereb Expected Result/Behavior of Pre-process
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK
 * -# allocate_setup_data_memory run successfully
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * The value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK
 * -# setup_hw_register run successfully
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection InterruptRegisterIsr_TC_05_002_011_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsubsection InterruptRegisterIsr_TC_05_002_011_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * @subsection InterruptRegisterIsr_TC_05_002_011_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptRegisterIsr_TC_05_002_011
 * @snippet test_interrupt.cpp osal_test_int_cb_TC_05_002_011
 * @snippet test_interrupt.cpp osal_test_cb_sample
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [InterruptRegisterIsr_TC_05_002_011]
TEST_F(OSALINTTESTS, InterruptRegisterIsr_TC_05_002_011)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    osal_milli_sec_t time_period = 2000;
    osal_device_handle_t device_handle1 = OSAL_DEVICE_HANDLE_INVALID;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};

/* pre-process:
 * Open second test target device to register an second test target device interrupt in the interrupt context
 * Setup required memory for interrupt generation.
 * Register callback function and enable interrupt.
 * Wait for interrupt callback finish */
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_INTCTX_DEVICE_2, &device_handle1), OSAL_RETURN_OK);
    EXPECT_NE(device_handle1, NULL_PTR);
    arg.additional_arg = (void*)&device_handle1;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_test_int_cb_TC_05_002_011, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Disable and Unregister interrupt for registered interrupt of second test target device on interrupt callback function of first test target device
 * Deallocate allocated memory
 * Close second test target device*/

    EXPECT_EQ(R_OSAL_InterruptDisableIsr(device_handle1, TEST_IRQ_CHANNEL_DEVICE_INT), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_test_int_cb_TC_05_002_011), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle1, TEST_IRQ_CHANNEL_DEVICE_INT, osal_test_cb_sample), OSAL_RETURN_OK);
    deallocate_data_memory(hndl_mmngr, buffer_obj);
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle1), OSAL_RETURN_OK);
}
// [InterruptRegisterIsr_TC_05_002_011]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptRegisterIsr_TC_05_002_012 INTApiTest.InterruptRegisterIsr_TC_05_002_012
 * @subsection InterruptRegisterIsr_TC_05_002_012_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptRegisterIsr_TC_05_002_012_tci Test Case ID
 * InterruptRegisterIsr_S0_BA_TC_05_002_012
 * @subsection InterruptRegisterIsr_TC_05_002_012_rr Related Requirement(s)
 * \#199187, \#199188
 * @subsection InterruptRegisterIsr_TC_05_002_012_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * @subsection InterruptRegisterIsr_TC_05_002_012_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptRegisterIsr APIs with abnormal condition:
 * - Test05_002_012: priority is out of range.
 * @subsection InterruptRegisterIsr_TC_05_002_012_tpts Test Procedure/Test Steps
 * -# Register an interrupt callback by using R_OSAL_InterruptRegisterIsr with the arguments as below:
 *      - Device handle is valid
 *      - Interrupt channel TEST_IRQ_CHANNEL_DEVICE_VALID
 *      - Priority is OSAL_INTERRUPT_PRIORITY_HIGHEST + 1
 *      - Callback function is "osal_test_cb_sample"
 *      - Address of "arg"
 * @subsection InterruptRegisterIsr_TC_05_002_012_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_PAR.
 * @subsection InterruptRegisterIsr_TC_05_002_012_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptRegisterIsr_TC_05_002_012
 * @snippet test_interrupt.cpp osal_test_cb_sample
 */
// [InterruptRegisterIsr_TC_05_002_012]
TEST_F(INTApiTest, InterruptRegisterIsr_TC_05_002_012)
{
    e_osal_interrupt_priority_t priority = e_osal_interrupt_priority_t(OSAL_INTERRUPT_PRIORITY_HIGHEST + 1);

    // Test05_002_012
    // @brief: abnormal case: priority is out of range
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, priority, osal_test_cb_sample, NULL), OSAL_RETURN_PAR) << "Test05_002_012 failed";
}
// [InterruptRegisterIsr_TC_05_002_012]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptRegisterIsr_TC_05_002_013 INTApiTest.InterruptRegisterIsr_TC_05_002_013
 * @subsection InterruptRegisterIsr_TC_05_002_013_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptRegisterIsr_TC_05_002_013_tci Test Case ID
 * InterruptRegisterIsr_S0_BA_TC_05_002_013
 * @subsection InterruptRegisterIsr_TC_05_002_013_rr Related Requirement(s)
 * \#199187, \#199188
 * @subsection InterruptRegisterIsr_TC_05_002_013_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * @subsection InterruptRegisterIsr_TC_05_002_013_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptRegisterIsr APIs with abnormal condition:
 * - Test05_002_013: priority is out of range.
 * @subsection InterruptRegisterIsr_TC_05_002_013_tpts Test Procedure/Test Steps
 * -# Register an interrupt callback by using R_OSAL_InterruptRegisterIsr with the arguments as below:
 *      - Device handle is valid
 *      - Interrupt channel TEST_IRQ_CHANNEL_DEVICE_VALID
 *      - Priority is OSAL_INTERRUPT_PRIORITY_LOWEST - 1
 *      - Callback function is "osal_test_cb_sample"
 *      - Address of "arg"
 * @subsection InterruptRegisterIsr_TC_05_002_013_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_PAR.
 * @subsection InterruptRegisterIsr_TC_05_002_013_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptRegisterIsr_TC_05_002_013
 * @snippet test_interrupt.cpp osal_test_cb_sample
 */
// [InterruptRegisterIsr_TC_05_002_013]
TEST_F(INTApiTest, InterruptRegisterIsr_TC_05_002_013)
{
    e_osal_interrupt_priority_t priority = e_osal_interrupt_priority_t(OSAL_INTERRUPT_PRIORITY_LOWEST - 1);

    // Test05_002_013
    // @brief: abnormal case: priority is out of range
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, priority, osal_test_cb_sample, NULL), OSAL_RETURN_PAR) << "Test05_002_013 failed";
}
// [InterruptRegisterIsr_TC_05_002_013]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptRegisterIsr_TC_05_002_014 INTApiTest.InterruptRegisterIsr_TC_05_002_014
 * @subsection InterruptRegisterIsr_TC_05_002_014_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptRegisterIsr_TC_05_002_014_tci Test Case ID
 * InterruptRegisterIsr_S0_BN_TC_05_002_014
 * @subsection InterruptRegisterIsr_TC_05_002_014_rr Related Requirement(s)
 * \#199187, \#199188
 * @subsection InterruptRegisterIsr_TC_05_002_014_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * @subsection InterruptRegisterIsr_TC_05_002_014_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptRegisterIsr APIs:
 * - Test05_002_014: priority is OSAL_INTERRUPT_PRIORITY_LOWEST
 * @subsection InterruptRegisterIsr_TC_05_002_014_tpts Test Procedure/Test Steps
 * -# Register an interrupt callback by using R_OSAL_InterruptRegisterIsr with the arguments as below:
 *      - Device handle is valid
 *      - Interrupt channel TEST_IRQ_CHANNEL_DEVICE_VALID
 *      - Priority is OSAL_INTERRUPT_PRIORITY_LOWEST
 *      - Callback function is "osal_test_cb_sample"
 *      - Address of "arg"
 * @subsection InterruptRegisterIsr_TC_05_002_014_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_PAR.
 * @subsection InterruptRegisterIsr_TC_05_002_014_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptRegisterIsr_TC_05_002_014
 * @snippet test_interrupt.cpp osal_test_cb_sample
 */
// [InterruptRegisterIsr_TC_05_002_014]
TEST_F(INTApiTest, InterruptRegisterIsr_TC_05_002_014)
{
    // Test05_002_014
    // @brief: abnormal case: priority is OSAL_INTERRUPT_PRIORITY_LOWEST
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, OSAL_INTERRUPT_PRIORITY_LOWEST, osal_test_cb_sample, NULL), OSAL_RETURN_OK) << "Test05_002_014 failed";
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, osal_test_cb_sample), OSAL_RETURN_OK) << "Test05_002_014 failed";
}
// [InterruptRegisterIsr_TC_05_002_014]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptRegisterIsr_TC_05_002_015 INTApiTest.InterruptRegisterIsr_TC_05_002_015
 * @subsection InterruptRegisterIsr_TC_05_002_015_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptRegisterIsr_TC_05_002_015_tci Test Case ID
 * InterruptRegisterIsr_S0_BN_TC_05_002_015
 * @subsection InterruptRegisterIsr_TC_05_002_015_rr Related Requirement(s)
 * \#199187, \#199188
 * @subsection InterruptRegisterIsr_TC_05_002_015_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * @subsection InterruptRegisterIsr_TC_05_002_015_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptRegisterIsr APIs:
 * - Test05_002_015: priority is OSAL_INTERRUPT_PRIORITY_HIGHEST
 * @subsection InterruptRegisterIsr_TC_05_002_015_tpts Test Procedure/Test Steps
 * -# Register an interrupt callback by using R_OSAL_InterruptRegisterIsr with the arguments as below:
 *      - Device handle is valid
 *      - Interrupt channel TEST_IRQ_CHANNEL_DEVICE_VALID
 *      - Priority is OSAL_INTERRUPT_PRIORITY_HIGHEST
 *      - Callback function is "osal_test_cb_sample"
 *      - Address of "arg"
 * @subsection InterruptRegisterIsr_TC_05_002_015_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_PAR.
 * @subsection InterruptRegisterIsr_TC_05_002_015_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptRegisterIsr_TC_05_002_015
 * @snippet test_interrupt.cpp osal_test_cb_sample
 */
// [InterruptRegisterIsr_TC_05_002_015]
TEST_F(INTApiTest, InterruptRegisterIsr_TC_05_002_015)
{
    // Test05_002_015
    // @brief: abnormal case: priority is OSAL_INTERRUPT_PRIORITY_HIGHEST
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, OSAL_INTERRUPT_PRIORITY_HIGHEST, osal_test_cb_sample, NULL), OSAL_RETURN_OK) << "Test05_002_015 failed";
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, osal_test_cb_sample), OSAL_RETURN_OK) << "Test05_002_015 failed";
}
// [InterruptRegisterIsr_TC_05_002_015]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptUnregisterIsr_TC_05_003_001 INTApiTest.InterruptUnregisterIsr_TC_05_003_001
 * @subsection InterruptUnregisterIsr_TC_05_003_001_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptUnregisterIsr_TC_05_003_001_tci Test Case ID
 * InterruptUnregisterIsr_S1_BN_TC_05_003_001
 * @subsection InterruptUnregisterIsr_TC_05_003_001_rr Related Requirement(s)
 * \#199191
 * @subsection InterruptUnregisterIsr_TC_05_003_001_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptUnregisterIsr_TC_05_003_001_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptRegisterIsr/R_OSAL_InterruptUnregisterIsr APIs on normal condition:
 * - pre-process: get the number of interrupt channel and register interrupt callback function related to channel 0
 * - Test05_003_001: unregister interrupt for device corresponding with valid interrupt channel.
 * @subsection InterruptUnregisterIsr_TC_05_003_001_tpts Test Procedure/Test Steps
 * -# Get the number of interrupt channel corresponding with opened device handle test target device by calling to R_OSAL_InterruptGetNumOfIrqChannels.
 * -# Register interrupt callback "osal_test_cb_sample" for an interrupt occurs from interrupt channel TEST_IRQ_CHANNEL_DEVICE_VALID of test target device by calling to R_OSAL_InterruptRegisterIsr.
 * -# Unregister interrupt callback "osal_test_cb_sample" registered at step 2 by calling to R_OSAL_InterruptUnregisterIsr.
 * @subsection InterruptUnregisterIsr_TC_05_003_001_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * Value of "numOfChannel" is bigger than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK.
 * @subsection InterruptUnregisterIsr_TC_05_003_001_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptUnregisterIsr_TC_05_003_001
 * @snippet test_interrupt.cpp osal_test_cb_sample
 */
// [InterruptUnregisterIsr_TC_05_003_001]
TEST_F(INTApiTest, InterruptUnregisterIsr_TC_05_003_001)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE10;
    int arg = 10;

    // pre-process: Register interrupt
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_GT(numOfChannel, 0) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, priority, osal_test_cb_sample, (void *)&arg), OSAL_RETURN_OK) << "pre-process failed";

    // Test05_003_001
    // @brief: normal case: unregister interrupt for device corresponding with interrupt channel
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, osal_test_cb_sample), OSAL_RETURN_OK) << "Test05_003_001 failed";
}
// [InterruptUnregisterIsr_TC_05_003_001]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptUnregisterIsr_TC_05_003_002 INTApiTest.InterruptUnregisterIsr_TC_05_003_002
 * @subsection InterruptUnregisterIsr_TC_05_003_002_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptUnregisterIsr_TC_05_003_002_tci Test Case ID
 * InterruptUnregisterIsr_S0_A_TC_05_003_002
 * @subsection InterruptUnregisterIsr_TC_05_003_002_rr Related Requirement(s)
 * \#199151, \#199191
 * @subsection InterruptUnregisterIsr_TC_05_003_002_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * @subsection InterruptUnregisterIsr_TC_05_003_002_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptUnregisterIsr APIs with abnormal condition:
 * - Test05_003_002: unregister interrupt for device handle is NULL.
 * @subsection InterruptUnregisterIsr_TC_05_003_002_tpts Test Procedure/Test Steps
 * -# Unregister interrupt callback by using to R_OSAL_InterruptUnregisterIsr with the argument as below:
 *      - Device handle is NULL
 *      - Interrupt channel TEST_IRQ_CHANNEL_DEVICE_VALID
 *      - Callback function is "osal_test_cb_sample"
 * @subsection InterruptUnregisterIsr_TC_05_003_002_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_HANDLE.
 * @subsection InterruptUnregisterIsr_TC_05_003_002_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptUnregisterIsr_TC_05_003_002
 * @snippet test_interrupt.cpp osal_test_cb_sample
 */
// [InterruptUnregisterIsr_TC_05_003_002]
TEST_F(INTApiTest, InterruptUnregisterIsr_TC_05_003_002)
{
    osal_device_handle_t p_device_handle = NULL;

    // Test05_003_002
    // @brief: abnormal case: unregister interrupt for device handle is NULL
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(p_device_handle, TEST_IRQ_CHANNEL_DEVICE_INVALID, osal_test_cb_sample), OSAL_RETURN_HANDLE) << "Test05_003_002 failed";
}
// [InterruptUnregisterIsr_TC_05_003_002]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptUnregisterIsr_TC_05_003_003 INTApiTest.InterruptUnregisterIsr_TC_05_003_003
 * @subsection InterruptUnregisterIsr_TC_05_003_003_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptUnregisterIsr_TC_05_003_003_tci Test Case ID
 * InterruptUnregisterIsr_S1_A_TC_05_003_003
 * @subsection InterruptUnregisterIsr_TC_05_003_003_rr Related Requirement(s)
 * \#199191
 * @subsection InterruptUnregisterIsr_TC_05_003_003_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptUnregisterIsr_TC_05_003_003_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptUnregisterIsr APIs with abnormal condition:
 * - Test05_003_003: unregister interrupt for device handle has already been closed.
 * @subsection InterruptUnregisterIsr_TC_05_003_003_tpts Test Procedure/Test Steps
 * -# Close test target device by using R_OSAL_IoDeviceClose.
 * -# Unregister interrupt callback by using to R_OSAL_InterruptUnregisterIsr with the argument as below:
 *      - Device handle has already closed at step 1
 *      - Interrupt channel TEST_IRQ_CHANNEL_DEVICE_VALID
 *      - Callback function is "osal_test_cb_sample"
 * -# Re-open test target device for the next test cases by using R_OSAL_IoDeviceOpen.
 * @subsection InterruptUnregisterIsr_TC_05_003_003_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_HANDLE.
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * @subsection InterruptUnregisterIsr_TC_05_003_003_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptUnregisterIsr_TC_05_003_003
 * @snippet test_interrupt.cpp osal_test_cb_sample
 */
// [InterruptUnregisterIsr_TC_05_003_003]
TEST_F(INTApiTest, InterruptUnregisterIsr_TC_05_003_003)
{
    // Test05_003_003
    // @brief: abnormal case: unregister interrupt for device handle is closed
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, osal_test_cb_sample), OSAL_RETURN_HANDLE) << "Test05_003_003 failed";
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_INT_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "Test05_003_003 failed";
    EXPECT_NE(device_handle, NULL_PTR) << "Test05_003_003 failed";
}
// [InterruptUnregisterIsr_TC_05_003_003]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptUnregisterIsr_TC_05_003_004 INTApiTest.InterruptUnregisterIsr_TC_05_003_004
 * @subsection InterruptUnregisterIsr_TC_05_003_004_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptUnregisterIsr_TC_05_003_004_tci Test Case ID
 * InterruptUnregisterIsr_S0_BA_TC_05_003_004
 * @subsection InterruptUnregisterIsr_TC_05_003_004_rr Related Requirement(s)
 * \#199191
 * @subsection InterruptUnregisterIsr_TC_05_003_004_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptUnregisterIsr_TC_05_003_004_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptUnregisterIsr APIs with abnormal condition:
 * - Test05_003_004: unregister interrupt with interrupt channel is invalid.
 * @subsection InterruptUnregisterIsr_TC_05_003_004_tpts Test Procedure/Test Steps
 * -# Get the number of interrupt channel corresponding with opened device handle test target device by calling to R_OSAL_InterruptGetNumOfIrqChannels.
 * -# Unregister interrupt callback by using to R_OSAL_InterruptUnregisterIsr with the argument as below:
 *      - Device handle of test target device
 *      - Interrupt channel is out-of-valid range: [0 -- "numOfChannel").<br>
 *        "numOfChannel" got at step 1.
 *      - Callback function is "osal_test_cb_sample"
 * @subsection InterruptUnregisterIsr_TC_05_003_004_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * Value of "numOfChannel" is bigger than 0.
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_PAR.
 * @subsection InterruptUnregisterIsr_TC_05_003_004_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptUnregisterIsr_TC_05_003_004
 * @snippet test_interrupt.cpp osal_test_cb_sample
 */
// [InterruptUnregisterIsr_TC_05_003_004]
TEST_F(INTApiTest, InterruptUnregisterIsr_TC_05_003_004)
{
    size_t numOfChannel = 0;

    // Test05_003_004
    // @brief: abnormal case: unregister interrupt with irq_channel is invalid.
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, numOfChannel, osal_test_cb_sample), OSAL_RETURN_PAR) << "Test05_003_004 failed";
}
// [InterruptUnregisterIsr_TC_05_003_004]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptUnregisterIsr_TC_05_003_005 INTApiTest.InterruptUnregisterIsr_TC_05_003_005
 * @subsection InterruptUnregisterIsr_TC_05_003_005_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptUnregisterIsr_TC_05_003_005_tci Test Case ID
 * InterruptUnregisterIsr_S4_A_TC_05_003_005
 * @subsection InterruptUnregisterIsr_TC_05_003_005_rr Related Requirement(s)
 * \#199186, \#199191
 * @subsection InterruptUnregisterIsr_TC_05_003_005_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptUnregisterIsr_TC_05_003_005_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptUnregisterIsr APIs with abnormal condition:
 * - Test05_003_005: register/unregister interrupt with callback function is NULL.
 * @subsection InterruptUnregisterIsr_TC_05_003_005_tpts Test Procedure/Test Steps
 * -# Unregister interrupt callback by using to R_OSAL_InterruptUnregisterIsr with the argument as below:
 *      - Device handle of test target device
 *      - Interrupt channel is 0.
 *      - Callback function is NULL
 * @subsection InterruptUnregisterIsr_TC_05_003_005_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_PAR.
 * @subsection InterruptUnregisterIsr_TC_05_003_005_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptUnregisterIsr_TC_05_003_005
 */
// [InterruptUnregisterIsr_TC_05_003_005]
TEST_F(INTApiTest, InterruptUnregisterIsr_TC_05_003_005)
{
    p_osal_isr_func_t p_irqHandlerFunction = NULL;

    // Test05_003_005
    // @brief: abnormal case: unregister interrupt with irqHandlerFunction is NULL
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, p_irqHandlerFunction), OSAL_RETURN_PAR) << "Test05_003_005 failed";
}
// [InterruptUnregisterIsr_TC_05_003_005]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptUnregisterIsr_TC_05_003_006 INTApiTest.InterruptUnregisterIsr_TC_05_003_006
 * @subsection InterruptUnregisterIsr_TC_05_003_006_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptUnregisterIsr_TC_05_003_006_tci Test Case ID
 * InterruptUnregisterIsr_S2_A_TC_05_003_006
 * @subsection InterruptUnregisterIsr_TC_05_003_006_rr Related Requirement(s)
 * \#199191
 * @subsection InterruptUnregisterIsr_TC_05_003_006_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptUnregisterIsr_TC_05_003_006_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptUnregisterIsr APIs with abnormal condition:
 * - Test05_003_006: unregister the interrupt handler which has not been registered.
 * @subsection InterruptUnregisterIsr_TC_05_003_006_tpts Test Procedure/Test Steps
 * -# Unregister interrupt callback "osal_test_cb_sample" by calling to R_OSAL_InterruptUnregisterIsr with interrupt channel which has not been registered.
 * @subsection InterruptUnregisterIsr_TC_05_003_006_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_PAR.
 * @subsection InterruptUnregisterIsr_TC_05_003_006_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptUnregisterIsr_TC_05_003_006
 * @snippet test_interrupt.cpp osal_test_cb_sample
 */
// [InterruptUnregisterIsr_TC_05_003_006]
TEST_F(INTApiTest, InterruptUnregisterIsr_TC_05_003_006)
{
    // Test05_003_006
    // @brief: abnormal case: unregister with interrupt channel which has not been registered
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, osal_test_cb_sample), OSAL_RETURN_PAR) << "Test05_003_006 failed";
}
// [InterruptUnregisterIsr_TC_05_003_006]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptUnregisterIsr_TC_05_003_007 INTApiTest.InterruptUnregisterIsr_TC_05_003_007
 * @subsection InterruptUnregisterIsr_TC_05_003_007_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptUnregisterIsr_TC_05_003_007_tci Test Case ID
 * InterruptUnregisterIsr_S2_A_TC_05_003_007
 * @subsection InterruptUnregisterIsr_TC_05_003_007_rr Related Requirement(s)
 * \#199191, \#199192
 * @subsection InterruptUnregisterIsr_TC_05_003_007_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptUnregisterIsr_TC_05_003_007_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptUnregisterIsr APIs with abnormal condition:
 * - Test05_003_007: unregister with the irqHandlerFunction which has not been registered
 * - post-process: unregister interrupt channel TEST_IRQ_CHANNEL_DEVICE_VALID
 * @subsection InterruptUnregisterIsr_TC_05_003_007_tpts Test Procedure/Test Steps
 * -# Get the number of interrupt channel corresponding with opened device handle test target device by calling to R_OSAL_InterruptGetNumOfIrqChannels.
 * -# Register interrupt callback "osal_test_cb_sample" for an interrupt occurs from interrupt channel TEST_IRQ_CHANNEL_DEVICE_VALID of test target device by calling to R_OSAL_InterruptRegisterIsr.
 * -# Unregister interrupt callback by using to R_OSAL_InterruptUnregisterIsr with the argument as below:
 *      - Device handle of test target device
 *      - Interrupt channel is 0.
 *      - Callback function is "osal_test_cb_sample1"
 * -# Unregister interrupt callback "osal_test_cb_sample" registered at step 2 by calling to R_OSAL_InterruptUnregisterIsr.
 * @subsection InterruptUnregisterIsr_TC_05_003_007_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * Value of "numOfChannel" is bigger than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_PAR.
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK.
 * @subsection InterruptUnregisterIsr_TC_05_003_007_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptUnregisterIsr_TC_05_003_007
 * @snippet test_interrupt.cpp osal_test_cb_sample
 * @snippet test_interrupt.cpp osal_test_cb_sample1
 */
// [InterruptUnregisterIsr_TC_05_003_007]
TEST_F(INTApiTest, InterruptUnregisterIsr_TC_05_003_007)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE10;
    int arg = 10;

    // Test05_003_007
    // @brief: abnormal case: unregister with the irqHandlerFunction which has not been registered
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, priority, osal_test_cb_sample, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, osal_test_cb_sample1), OSAL_RETURN_PAR) << "Test05_003_007 failed";
    
    // post-process: Unregister interrupt
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, osal_test_cb_sample), OSAL_RETURN_OK);
}
// [InterruptUnregisterIsr_TC_05_003_007]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptUnregisterIsr_TC_05_003_008 INTApiTest.InterruptUnregisterIsr_TC_05_003_008
 * @subsection InterruptUnregisterIsr_TC_05_003_008_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptUnregisterIsr_TC_05_003_008_tci Test Case ID
 * InterruptUnregisterIsr_S2_A_TC_05_003_008
 * @subsection InterruptUnregisterIsr_TC_05_003_008_rr Related Requirement(s)
 * \#199190, \#199191
 * @subsection InterruptUnregisterIsr_TC_05_003_008_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptUnregisterIsr_TC_05_003_008_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptUnregisterIsr APIs with abnormal condition:
 * - pre-process: get the number of interrupt channel and register interrupt callback function related to channel 0
 * - Test05_003_008: unregister he interrupt handler which has already been enabled.
 * @subsection InterruptUnregisterIsr_TC_05_003_008_tpts Test Procedure/Test Steps
 * -# Get the number of interrupt channel corresponding with opened device handle test target device by calling to R_OSAL_InterruptGetNumOfIrqChannels.
 * -# Register interrupt callback "osal_test_cb_sample" for an interrupt occurs from interrupt channel TEST_IRQ_CHANNEL_DEVICE_VALID of test target device by calling to R_OSAL_InterruptRegisterIsr.
 * -# Enable the interrupt handler which has already registered at step 2 by using R_OSAL_InterruptEnableIsr.
 * -# Unregister interrupt callback "osal_test_cb_sample" enabled at step 3 by calling to R_OSAL_InterruptUnregisterIsr.
 * -# Disable the interrupt handle which has already enabled at step 3 by using R_OSAL_InterruptDisableIsr.
 * -# Unregister again interrupt callback "osal_test_cb_sample" enabled at step 3 by calling to R_OSAL_InterruptUnregisterIsr.
 * @subsection InterruptUnregisterIsr_TC_05_003_008_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * Value of "numOfChannel" is bigger than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_BUSY.
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK.
 * @subsection InterruptUnregisterIsr_TC_05_003_008_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptUnregisterIsr_TC_05_003_008
 * @snippet test_interrupt.cpp osal_test_cb_sample
 */
// [InterruptUnregisterIsr_TC_05_003_008]
TEST_F(INTApiTest, InterruptUnregisterIsr_TC_05_003_008)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE10;
    int arg = 10;

    // pre-process: Register interrupt
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, priority, osal_test_cb_sample, (void *)&arg), OSAL_RETURN_OK);

    // Test05_003_008
    // @brief: abnormal case: unregister with the interrupt channel which has already been enabled
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, osal_test_cb_sample), OSAL_RETURN_BUSY) << "Test05_003_008 failed";
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, osal_test_cb_sample), OSAL_RETURN_OK);
}
// [InterruptUnregisterIsr_TC_05_003_008]

/**
 * @page INT_TEST Interrupt API Test
 * @section OsalApiStateTest_InterruptUnregisterIsr_TC_05_003_009 OsalApiStateTest.InterruptUnregisterIsr_TC_05_003_009
 * @subsection InterruptUnregisterIsr_TC_05_003_009_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection InterruptUnregisterIsr_TC_05_003_009_tci Test Case ID
 * InterruptUnregisterIsr_S2_A_TC_05_003_009
 * @subsection InterruptUnregisterIsr_TC_05_003_009_rr Related Requirement(s)
 * \#199191
 * @subsection InterruptUnregisterIsr_TC_05_003_009_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection InterruptUnregisterIsr_TC_05_003_009_tcs Test Case Summary
 * This test shall verify the state transition of Clock Time APIs on abnormal condition:
 * - Test05_003_009: state transition error of R_OSAL_InterruptUnregisterIsr
 * @subsection InterruptUnregisterIsr_TC_05_003_009_tpts Test Procedure/Test Steps
 * -# Unregister interrupt callback "osal_test_cb_sample" by calling to R_OSAL_InterruptUnregisterIsr.
 * @subsection InterruptUnregisterIsr_TC_05_003_009_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_STATE.
 * @subsection InterruptUnregisterIsr_TC_05_003_009_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptUnregisterIsr_TC_05_003_009
 * @snippet test_interrupt.cpp osal_test_cb_sample
 */
// [InterruptUnregisterIsr_TC_05_003_009]
TEST_F(OsalApiStateTest, InterruptUnregisterIsr_TC_05_003_009)
{
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;

    // Test05_003_009
    // @brief: abnormal case: state transition error of R_OSAL_InterruptUnregisterIsr
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, osal_test_cb_sample), OSAL_RETURN_STATE) << "Test05_003_009 failed";
}
// [InterruptUnregisterIsr_TC_05_003_009]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptUnregisterIsr_TC_05_003_011 INTApiTest.InterruptUnregisterIsr_TC_05_003_011
 * @subsection InterruptUnregisterIsr_TC_05_003_011_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptUnregisterIsr_TC_05_003_011_tci Test Case ID
 * InterruptUnregisterIsr_S4_A_TC_05_003_011
 * @subsection InterruptUnregisterIsr_TC_05_003_011_rr Related Requirement(s)
 * \#199191
 * @subsection InterruptUnregisterIsr_TC_05_003_011_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptUnregisterIsr_TC_05_003_011_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptUnregisterIsr APIs with abnormal condition:
 * - Test05_003_011: unregister interrupt for device does not have its own interrupt channel.
 * @subsection InterruptUnregisterIsr_TC_05_003_011_tpts Test Procedure/Test Steps
 * -# Get the number of interrupt channel corresponding with opened device handle test target device by calling to R_OSAL_InterruptGetNumOfIrqChannels.
 * -# Unregister interrupt callback by using to R_OSAL_InterruptUnregisterIsr with the argument as below:
 *      - Device handle of test target device.<br>
 *        test target device is device which does not have its own interrupt channel.
 *      - Interrupt channel is 0.
 *      - Callback function is "osal_test_cb_sample"
 * @subsection InterruptUnregisterIsr_TC_05_003_011_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * Value of "numOfChannel" is equal to 0.
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_HANDLE.
 * @subsection InterruptUnregisterIsr_TC_05_003_011_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptUnregisterIsr_TC_05_003_011
 * @snippet test_interrupt.cpp osal_test_cb_sample
 */
// [InterruptUnregisterIsr_TC_05_003_011]
TEST_F(INTApiTest, InterruptUnregisterIsr_TC_05_003_011)
{
    size_t numOfChannel = 0;

    // Test05_003_011
    // @brief: abnormal case: unregister interrupt for device does not have its own interrupt channel
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle1, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_EQ(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle1, TEST_IRQ_CHANNEL_DEVICE_INVALID, osal_test_cb_sample), OSAL_RETURN_HANDLE) << "Test05_003_011 failed";
}
// [InterruptUnregisterIsr_TC_05_003_011]

/**
 * @page INT_TEST Interrupt API Test
 * @section OSALINTTESTS_InterruptUnregisterIsr_TC_05_003_012 OSALINTTESTS.InterruptUnregisterIsr_TC_05_003_012
 * @subsection InterruptUnregisterIsr_TC_05_003_012_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection InterruptUnregisterIsr_TC_05_003_012_tci Test Case ID
 * InterruptUnregisterIsr_S3_N_TC_05_003_012
 * @subsection InterruptUnregisterIsr_TC_05_003_012_rr Related Requirement(s)
 * \#199193, \#199194, \#199195, \#230924, \#276767
 * @subsection InterruptUnregisterIsr_TC_05_003_012_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection InterruptUnregisterIsr_TC_05_003_012_tcs Test Case Summary
 * @subsubsection InterruptUnregisterIsr_TC_05_003_012_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Setup required memory for interrupt generation
 * - Register callback function and enable interrupt
 * - Wait for interrupt callback function finish
 * @subsubsection InterruptUnregisterIsr_TC_05_003_012_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Deallocate allocated memory
 * @subsubsection InterruptUnregisterIsr_TC_05_003_012_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality of the Interrupt APIs which are called in the ISR with normal condition.
 * - Disable interrupt for test target device in ISR and clear status register
 * - Unregister itself interrupt callback function for test target device by using R_OSAL_InterruptUnregisterIsr in ISR
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection InterruptUnregisterIsr_TC_05_003_012_tpts Test Procedure/Test Steps
 * @subsubsection InterruptUnregisterIsr_TC_05_003_012_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Allocate required memory for generate interrupt from test target device and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function by using R_OSAL_InterruptRegisterIsr
 * -# Enable interrupt by using R_OSAL_InterruptEnableIsr
 * -# Setup test target device specific register to generate the interrupt by calling to setup_hw_register
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection InterruptUnregisterIsr_TC_05_003_012_post_tpts Test Procedure/Test Steps of Post-process
 * -# Deallocate required memory by calling to deallocate_data_memory
 * @subsubsection InterruptUnregisterIsr_TC_05_003_012_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt for test target device immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Unregister the interrupt callback function for test target device by using R_OSAL_InterruptUnregisterIsr
 * -# Send Message interrupt of context finished by using R_OSAL_MqSendForTimePeriod
 * @subsection InterruptUnregisterIsr_TC_05_003_012_ereb Expected Result/Behavior
 * @subsubsection InterruptUnregisterIsr_TC_05_003_012_pre_ereb Expected Result/Behavior of Pre-process
 * -# allocate_setup_data_memory run successfully
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * The value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK
 * -# setup_hw_register run successfully
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection InterruptUnregisterIsr_TC_05_003_012_post_ereb Expected Result/Behavior of Post-process
 * -# deallocate_data_memory run successfully.
 * @subsubsection InterruptUnregisterIsr_TC_05_003_012_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * @subsection InterruptUnregisterIsr_TC_05_003_012_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptUnregisterIsr_TC_05_003_012
 * @snippet test_interrupt.cpp osal_test_int_cb_TC_05_003_012
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [InterruptUnregisterIsr_TC_05_003_012]
TEST_F(OSALINTTESTS, InterruptUnregisterIsr_TC_05_003_012)
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
 * Register callback function and enable interrupt.
 * Wait for interrupt callback finish */
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_test_int_cb_TC_05_003_012, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Deallocate allocated memory*/

    deallocate_data_memory(hndl_mmngr, buffer_obj);
}
// [InterruptUnregisterIsr_TC_05_003_012]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptUnregisterIsr_TC_05_003_013 INTApiTest.InterruptUnregisterIsr_TC_05_003_013
 * @subsection InterruptUnregisterIsr_TC_05_003_013_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptUnregisterIsr_TC_05_003_013_tci Test Case ID
 * InterruptUnregisterIsr_S2_A_TC_05_003_013
 * @subsection InterruptUnregisterIsr_TC_05_003_013_rr Related Requirement(s)
 * \#199191
 * @subsection InterruptUnregisterIsr_TC_05_003_013_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptUnregisterIsr_TC_05_003_013_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptUnregisterIsr APIs with abnormal condition:
 * - pre-process: get the number of interrupt channel and register interrupt callback function related to channel 0
 * - Test05_003_013: unregister the interrupt handler which has already been unregistered.
 * @subsection InterruptUnregisterIsr_TC_05_003_013_tpts Test Procedure/Test Steps
 * -# Get the number of interrupt channel corresponding with opened device handle test target device by calling to R_OSAL_InterruptGetNumOfIrqChannels.
 * -# Register interrupt callback "osal_test_cb_sample" for an interrupt occurs from interrupt channel TEST_IRQ_CHANNEL_DEVICE_VALID of test target device by calling to R_OSAL_InterruptRegisterIsr.
 * -# Unregister interrupt callback "osal_test_cb_sample" registered at step 2 by calling to R_OSAL_InterruptUnregisterIsr.
 * -# Unregister interrupt callback "osal_test_cb_sample" again by calling to R_OSAL_InterruptUnregisterIsr with interrupt channel which has already been unregistered.
 * @subsection InterruptUnregisterIsr_TC_05_003_013_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * Value of "numOfChannel" is bigger than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_PAR.
 * @subsection InterruptUnregisterIsr_TC_05_003_013_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptUnregisterIsr_TC_05_003_013
 * @snippet test_interrupt.cpp osal_test_cb_sample
 */
// [InterruptUnregisterIsr_TC_05_003_013]
TEST_F(INTApiTest, InterruptUnregisterIsr_TC_05_003_013)
{
    size_t numOfChannel = 0;
    int arg = 10;

    // pre-process: Register interrupt
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_GT(numOfChannel, 0) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, OSAL_INTERRUPT_PRIORITY_TYPE10, osal_test_cb_sample, (void *)&arg), OSAL_RETURN_OK) << "pre-process failed";

    // Test05_003_013
    // @brief: abnormal case: unregister with interrupt channel which has already been unregistered
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, osal_test_cb_sample), OSAL_RETURN_OK) << "Test05_003_013 failed";
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, osal_test_cb_sample), OSAL_RETURN_PAR) << "Test05_003_013 failed";
}
// [InterruptUnregisterIsr_TC_05_003_013]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptEnableIsr_TC_05_004_001 INTApiTest.InterruptEnableIsr_TC_05_004_001
 * @subsection InterruptEnableIsr_TC_05_004_001_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptEnableIsr_TC_05_004_001_tci Test Case ID
 * InterruptEnableIsr_S1_BN_TC_05_004_001
 * @subsection InterruptEnableIsr_TC_05_004_001_rr Related Requirement(s)
 * \#199193
 * @subsection InterruptEnableIsr_TC_05_004_001_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptEnableIsr_TC_05_004_001_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptEnableIsr APIs with normal condition:
 * - pre-process: get the number of interrupt channel and register interrupt callback function related to channel 0
 * - Test05_004_001: enable interrupt for device corresponding with registered interrupt handler.
 * - post-process: disable and unregister interrupt channel TEST_IRQ_CHANNEL_DEVICE_VALID
 * @subsection InterruptEnableIsr_TC_05_004_001_tpts Test Procedure/Test Steps
 * -# Get the number of interrupt channel corresponding with opened device handle test target device by calling to R_OSAL_InterruptGetNumOfIrqChannels.
 * -# Register interrupt callback "osal_test_cb_sample" for an interrupt occurs from interrupt channel TEST_IRQ_CHANNEL_DEVICE_VALID of test target device by calling to R_OSAL_InterruptRegisterIsr.
 * -# Enable interrupt handler which has already been registered at step 2 by using R_OSAL_InterruptEnableIsr.
 * -# Disable interrupt handler which has already been enabled at step 3 by using R_OSAL_InterruptDisableIsr.
 * -# Unregister interrupt callback "osal_test_cb_sample" registered at step 2 by calling to R_OSAL_InterruptUnregisterIsr.
 * @subsection InterruptEnableIsr_TC_05_004_001_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * Value of "numOfChannel" is bigger than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK.
 * @subsection InterruptEnableIsr_TC_05_004_001_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptEnableIsr_TC_05_004_001
 * @snippet test_interrupt.cpp osal_test_cb_sample
 */
// [InterruptEnableIsr_TC_05_004_001]
TEST_F(INTApiTest, InterruptEnableIsr_TC_05_004_001)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE10;
    int arg = 10;

    // pre-process: Register interrupt
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_GT(numOfChannel, 0) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, priority, osal_test_cb_sample, (void *)&arg), OSAL_RETURN_OK) << "pre-process failed";

    // Test05_004_001
    // @brief: normal case: enable interrupt for device corresponding with interrupt channel
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID), OSAL_RETURN_OK) << "Test05_004_001 failed";

    // post-process: Unregister interrupt
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, osal_test_cb_sample), OSAL_RETURN_OK) << "post-process failed";
}
// [InterruptEnableIsr_TC_05_004_001]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptEnableIsr_TC_05_004_002 INTApiTest.InterruptEnableIsr_TC_05_004_002
 * @subsection InterruptEnableIsr_TC_05_004_002_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptEnableIsr_TC_05_004_002_tci Test Case ID
 * InterruptEnableIsr_S0_A_TC_05_004_002
 * @subsection InterruptEnableIsr_TC_05_004_002_rr Related Requirement(s)
 * \#199151, \#199193
 * @subsection InterruptEnableIsr_TC_05_004_002_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * @subsection InterruptEnableIsr_TC_05_004_002_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptEnableIsr APIs with abnormal condition:
 * - Test05_004_002: enable interrupt with device handle is NULL.
 * @subsection InterruptEnableIsr_TC_05_004_002_tpts Test Procedure/Test Steps
 * -# Enable interrupt handler by using R_OSAL_InterruptEnableIsr with the arguments as below:
 *      - Device handle is NULL
 *      - Interrupt channel is 0
 * @subsection InterruptEnableIsr_TC_05_004_002_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_HANDLE
 * @subsection InterruptEnableIsr_TC_05_004_002_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptEnableIsr_TC_05_004_002
 */
// [InterruptEnableIsr_TC_05_004_002]
TEST_F(INTApiTest, InterruptEnableIsr_TC_05_004_002)
{
    osal_device_handle_t p_device_handle = NULL;

    // Test05_004_002
    // @brief: abnormal case: enable interrupt for device handle is NULL
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(p_device_handle, TEST_IRQ_CHANNEL_DEVICE_INVALID), OSAL_RETURN_HANDLE) << "Test05_004_002 failed";
}
// [InterruptEnableIsr_TC_05_004_002]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptEnableIsr_TC_05_004_003 INTApiTest.InterruptEnableIsr_TC_05_004_003
 * @subsection InterruptEnableIsr_TC_05_004_003_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptEnableIsr_TC_05_004_003_tci Test Case ID
 * InterruptEnableIsr_S2_A_TC_05_004_003
 * @subsection InterruptEnableIsr_TC_05_004_003_rr Related Requirement(s)
 * \#199193
 * @subsection InterruptEnableIsr_TC_05_004_003_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptEnableIsr_TC_05_004_003_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptEnableIsr APIs with abnormal condition:
 * - Test05_004_003: enable interrupt with device handle has already been closed.
 * @subsection InterruptEnableIsr_TC_05_004_003_tpts Test Procedure/Test Steps
 * -# Close test target device by using R_OSAL_IoDeviceClose.
 * -# Enable interrupt handler by using R_OSAL_InterruptEnableIsr with the arguments as below:
 *      - Device handle has already been closed at step 1.
 *      - Interrupt channel is 0
 * -# Re-open test target device for the next test cases by using R_OSAL_IoDeviceOpen.
 * @subsection InterruptEnableIsr_TC_05_004_003_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_HANDLE.
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * @subsection InterruptEnableIsr_TC_05_004_003_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptEnableIsr_TC_05_004_003
 */
// [InterruptEnableIsr_TC_05_004_003]
TEST_F(INTApiTest, InterruptEnableIsr_TC_05_004_003)
{
    // Test05_004_003
    // @brief: abnormal case: enable interrupt for device handle is closed
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID), OSAL_RETURN_HANDLE) << "Test05_004_003 failed";
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_INT_DEVICE_1, &device_handle), OSAL_RETURN_OK);
    EXPECT_NE(device_handle, NULL_PTR);
}
// [InterruptEnableIsr_TC_05_004_003]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptEnableIsr_TC_05_004_004 INTApiTest.InterruptEnableIsr_TC_05_004_004
 * @subsection InterruptEnableIsr_TC_05_004_004_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptEnableIsr_TC_05_004_004_tci Test Case ID
 * InterruptEnableIsr_S0_BA_TC_05_004_004
 * @subsection InterruptEnableIsr_TC_05_004_004_rr Related Requirement(s)
 * \#199193
 * @subsection InterruptEnableIsr_TC_05_004_004_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptEnableIsr_TC_05_004_004_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptEnableIsr APIs with abnormal condition:
 * - Test05_004_004: enable interrupt with interrupt channel is invalid.
 * @subsection InterruptEnableIsr_TC_05_004_004_tpts Test Procedure/Test Steps
 * -# Get the number of interrupt channel corresponding with opened device handle test target device by calling to R_OSAL_InterruptGetNumOfIrqChannels.
 * -# Enable interrupt handler by using R_OSAL_InterruptEnableIsr with the arguments as below:
 *      - Device handle does not have a interrupt handler registered by R_OSAL_InterruptRegisterIsr.
 *      - Interrupt channel is out-of-valid range: [0 -- "numOfChannel").<br>
 *        "numOfChannel" got at step 1.
 * @subsection InterruptEnableIsr_TC_05_004_004_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * Value of "numOfChannel" is bigger than 0.
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_PAR.
 * @subsection InterruptEnableIsr_TC_05_004_004_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptEnableIsr_TC_05_004_004
 */
// [InterruptEnableIsr_TC_05_004_004]
TEST_F(INTApiTest, InterruptEnableIsr_TC_05_004_004)
{
    size_t numOfChannel = 0;

    // Test05_004_004
    // @brief: abnormal case: enable interrupt with irq_channel is invalid
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, numOfChannel), OSAL_RETURN_PAR) << "Test05_004_004 failed";
}
// [InterruptEnableIsr_TC_05_004_004]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptEnableIsr_TC_05_004_005 INTApiTest.InterruptEnableIsr_TC_05_004_005
 * @subsection InterruptEnableIsr_TC_05_004_005_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptEnableIsr_TC_05_004_005_tci Test Case ID
 * InterruptEnableIs_S2_A_TC_05_004_005
 * @subsection InterruptEnableIsr_TC_05_004_005_rr Related Requirement(s)
 * \#199193
 * @subsection InterruptEnableIsr_TC_05_004_005_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptEnableIsr_TC_05_004_005_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptEnableIsr APIs with abnormal condition:
 * - Test05_004_005: enable interrupt with interrupt handler is not registered.
 * @subsection InterruptEnableIsr_TC_05_004_005_tpts Test Procedure/Test Steps
 * -# Enable interrupt handler by using R_OSAL_InterruptEnableIsr with the arguments as below:
 *      - Device handle does not have a interrupt handler registered by R_OSAL_InterruptRegisterIsr.
 *      - Interrupt channel is 0
 * @subsection InterruptEnableIsr_TC_05_004_005_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_FAIL.
 * @subsection InterruptEnableIsr_TC_05_004_005_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptEnableIsr_TC_05_004_005
 */
// [InterruptEnableIsr_TC_05_004_005]
TEST_F(INTApiTest, InterruptEnableIsr_TC_05_004_005)
{
    // Test05_004_005
    // @brief: abnormal case: enable interrupt with irq_channel has not been registered
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID), OSAL_RETURN_FAIL) << "Test05_004_005 failed";
}
// [InterruptEnableIsr_TC_05_004_005]

/**
 * @page INT_TEST Interrupt API Test
 * @section OsalApiStateTest_InterruptEnableIsr_TC_05_004_006 OsalApiStateTest.InterruptEnableIsr_TC_05_004_006
 * @subsection InterruptEnableIsr_TC_05_004_006_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection InterruptEnableIsr_TC_05_004_006_tci Test Case ID
 * InterruptEnableIsr_S2_A_TC_05_004_006
 * @subsection InterruptEnableIsr_TC_05_004_006_rr Related Requirement(s)
 * \#199193
 * @subsection InterruptEnableIsr_TC_05_004_006_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection InterruptEnableIsr_TC_05_004_006_tcs Test Case Summary
 * This test shall verify the state transition of Clock Time APIs on abnormal condition:
 * - Test05_004_006: state transition error of R_OSAL_InterruptEnableIsr
 * @subsection InterruptEnableIsr_TC_05_004_006_tpts Test Procedure/Test Steps
 * -# Enable interrupt handler by using R_OSAL_InterruptEnableIsr.
 * @subsection InterruptEnableIsr_TC_05_004_006_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_STATE.
 * @subsection InterruptEnableIsr_TC_05_004_006_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptEnableIsr_TC_05_004_006
 */
// [InterruptEnableIsr_TC_05_004_006]
TEST_F(OsalApiStateTest, InterruptEnableIsr_TC_05_004_006)
{
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;

    // Test05_004_006
    // @brief: abnormal case: state transition error of R_OSAL_InterruptEnableIsr
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID), OSAL_RETURN_STATE) << "Test05_004_006 failed";
}
// [InterruptEnableIsr_TC_05_004_006]

/**
 * @page INT_TEST Interrupt API Test
 * @section OSALINTTESTS_InterruptEnableIsr_TC_05_004_007 OSALINTTESTS.InterruptEnableIsr_TC_05_004_007
 * @subsection InterruptEnableIsr_TC_05_004_007_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection InterruptEnableIsr_TC_05_004_007_tci Test Case ID
 * InterruptEnableIsr_S3_N_TC_05_004_007
 * @subsection InterruptEnableIsr_TC_05_004_007_rr Related Requirement(s)
 * \#199193, \#199194, \#199195, \#230924, \#276767
 * @subsection InterruptEnableIsr_TC_05_004_007_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection InterruptEnableIsr_TC_05_004_007_tcs Test Case Summary
 * @subsubsection InterruptEnableIsr_TC_05_004_007_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Open second test target device to register an second test target device interrupt in the interrupt context
 * - Setup required memory for interrupt generation
 * - Register callback function for first test target device and enable interrupt
 * - Wait for interrupt callback finish
 * @subsubsection InterruptEnableIsr_TC_05_004_007_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Disable interrupts for second test target device enabled in the interrupt context
 * - Unregister the interrupt callback function of first test target device and second test target device registered in the interrupt context 
 * - Deallocate allocated memory
 * - Close second test target device devices
 * @subsubsection InterruptEnableIsr_TC_05_004_007_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality of the Interrupt APIs which are called in the ISR with normal condition.
 * - Disable interrupt for first test target device in ISR and clear status register
 * - Test05_004_007: Enable interrupt for second test target device by using R_OSAL_InterruptEnableIsr in ISR
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection InterruptEnableIsr_TC_05_004_007_tpts Test Procedure/Test Steps
 * @subsubsection InterruptEnableIsr_TC_05_004_007_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Open second test target device by using R_OSAL_IoDeviceOpen
 * -# Allocate required memory for generate interrupt from first test target device and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function for first test target device by using R_OSAL_InterruptRegisterIsr
 * -# Enable interrupt for first test target device by using R_OSAL_InterruptEnableIsr
 * -# Setup first test target device specific register to generate the interrupt by calling to setup_hw_register
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection InterruptEnableIsr_TC_05_004_007_post_tpts Test Procedure/Test Steps of Post-process
 * -# Disable interrupt for second test target device by using R_OSAL_InterruptDisableIsr
 * -# Unregister interrupt for first test target device by using R_OSAL_InterruptUnregisterIsr
 * -# Unregister interrupt for second test target device by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * -# Close second test target device device by using R_OSAL_IoDeviceClose
 * @subsubsection InterruptEnableIsr_TC_05_004_007_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt for first test target device immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Register the interrupt callback function for second test target device by using R_OSAL_InterruptRegisterIsr
 * -# Enable interrupt for second test target device by using R_OSAL_InterruptEnableIsr
 * -# Send Message interrupt of context finished by using R_OSAL_MqSendForTimePeriod
 * @subsection InterruptEnableIsr_TC_05_004_007_ereb Expected Result/Behavior
 * @subsubsection InterruptEnableIsr_TC_05_004_007_pre_ereb Expected Result/Behavior of Pre-process
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK
 * -# allocate_setup_data_memory run successfully
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * The value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK
 * -# setup_hw_register run successfully
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection InterruptEnableIsr_TC_05_004_007_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsubsection InterruptEnableIsr_TC_05_004_007_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * @subsection InterruptEnableIsr_TC_05_004_007_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptEnableIsr_TC_05_004_007
 * @snippet test_interrupt.cpp osal_test_int_cb_TC_05_004_007
 * @snippet test_interrupt.cpp osal_test_cb_sample
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [InterruptEnableIsr_TC_05_004_007]
TEST_F(OSALINTTESTS, InterruptEnableIsr_TC_05_004_007)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    osal_milli_sec_t time_period = 2000;
    osal_device_handle_t device_handle1 = OSAL_DEVICE_HANDLE_INVALID;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};

/* pre-process:
 * Open second test target device to register an second test target device interrupt in the interrupt context
 * Setup required memory for interrupt generation.
 * Register callback function and enable interrupt.
 * Wait for interrupt callback finish */
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_INTCTX_DEVICE_2, &device_handle1), OSAL_RETURN_OK);
    EXPECT_NE(device_handle1, NULL_PTR);
    arg.additional_arg = (void*)&device_handle1;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_test_int_cb_TC_05_004_007, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Disable and Unregister interrupt for registered interrupt of second test target device on interrupt callback function of first test target device
 * Deallocate allocated memory
 * Close second test target device*/

    EXPECT_EQ(R_OSAL_InterruptDisableIsr(device_handle1, TEST_IRQ_CHANNEL_DEVICE_INT), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_test_int_cb_TC_05_004_007), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle1, TEST_IRQ_CHANNEL_DEVICE_INT, osal_test_cb_sample), OSAL_RETURN_OK);
    deallocate_data_memory(hndl_mmngr, buffer_obj);
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle1), OSAL_RETURN_OK);
}
// [InterruptEnableIsr_TC_05_004_007]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptEnableIsr_TC_05_004_009 INTApiTest.InterruptEnableIsr_TC_05_004_009
 * @subsection InterruptEnableIsr_TC_05_004_009_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptEnableIsr_TC_05_004_009_tci Test Case ID
 * InterruptEnableIsr_S4_A_TC_05_004_009
 * @subsection InterruptEnableIsr_TC_05_004_009_rr Related Requirement(s)
 * \#199193
 * @subsection InterruptEnableIsr_TC_05_004_009_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptEnableIsr_TC_05_004_009_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptEnableIsr APIs with abnormal condition:
 * - Test05_004_009: enable interrupt with test target device which does not have interrupt channel.
 * @subsection InterruptEnableIsr_TC_05_004_009_tpts Test Procedure/Test Steps
 * -# Enable interrupt handler by using R_OSAL_InterruptEnableIsr with the arguments as below:
 *      - Device handle of test target device
 *      - Interrupt channel is 0
 * @subsection InterruptEnableIsr_TC_05_004_009_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_HANDLE.
 * @subsection InterruptEnableIsr_TC_05_004_009_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptEnableIsr_TC_05_004_009
 */
// [InterruptEnableIsr_TC_05_004_009]
TEST_F(INTApiTest, InterruptEnableIsr_TC_05_004_009)
{
    // Test05_004_009
    // @brief: abnormal case: enable interrupt with test target device which does not have interrupt channel
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle1, TEST_IRQ_CHANNEL_DEVICE_INVALID), OSAL_RETURN_HANDLE) << "Test05_004_009 failed";
}
// [InterruptEnableIsr_TC_05_004_009]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptDisableIsr_TC_05_005_001 INTApiTest.InterruptDisableIsr_TC_05_005_001
 * @subsection InterruptDisableIsr_TC_05_005_001_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptDisableIsr_TC_05_005_001_tci Test Case ID
 * InterruptDisableIsr_S1_BN_TC_05_005_001
 * @subsection InterruptDisableIsr_TC_05_005_001_rr Related Requirement(s)
 * \#199194
 * @subsection InterruptDisableIsr_TC_05_005_001_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptDisableIsr_TC_05_005_001_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptDisableIsr APIs with normal condition:
 * - pre-process: get the number of interrupt channel, register and enable interrupt callback function related to channel 0
 * - Test05_005_001: disable interrupt for device corresponding with registered interrupt handler.
 * - post-process: unregister interrupt channel TEST_IRQ_CHANNEL_DEVICE_VALID
 * @subsection InterruptDisableIsr_TC_05_005_001_tpts Test Procedure/Test Steps
 * -# Get the number of interrupt channel corresponding with opened device handle test target device by calling to R_OSAL_InterruptGetNumOfIrqChannels.
 * -# Register interrupt callback "osal_test_cb_sample" for an interrupt occurs from interrupt channel TEST_IRQ_CHANNEL_DEVICE_VALID of test target device by calling to R_OSAL_InterruptRegisterIsr.
 * -# Enable interrupt handler which has already been registered at step 2 by using R_OSAL_InterruptEnableIsr.
 * -# Disable interrupt handler which has already been enabled at step 3 by using R_OSAL_InterruptDisableIsr.
 * -# Unregister interrupt callback "osal_test_cb_sample" registered at step 2 by calling to R_OSAL_InterruptUnregisterIsr.
 * @subsection InterruptDisableIsr_TC_05_005_001_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * Value of "numOfChannel" is bigger than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK.
 * @subsection InterruptDisableIsr_TC_05_005_001_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptDisableIsr_TC_05_005_001
 * @snippet test_interrupt.cpp osal_test_cb_sample
 */
// [InterruptDisableIsr_TC_05_005_001]
TEST_F(INTApiTest, InterruptDisableIsr_TC_05_005_001)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE10;
    int arg = 10;

    // pre-process: Register interrupt
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_GT(numOfChannel, 0) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, priority, osal_test_cb_sample, (void *)&arg), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID), OSAL_RETURN_OK) << "pre-process failed";

    // Test05_005_001
    // @brief: normal case: disable interrupt for device corresponding with interrupt channel
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID), OSAL_RETURN_OK) << "Test05_005_001 failed";

    // post-process: Unregister interrupt
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, osal_test_cb_sample), OSAL_RETURN_OK) << "post-process failed";
}
// [InterruptDisableIsr_TC_05_005_001]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptDisableIsr_TC_05_005_002 INTApiTest.InterruptDisableIsr_TC_05_005_002
 * @subsection InterruptDisableIsr_TC_05_005_002_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptDisableIsr_TC_05_005_002_tci Test Case ID
 * InterruptDisableIsr_S0_A_TC_05_005_002
 * @subsection InterruptDisableIsr_TC_05_005_002_rr Related Requirement(s)
 * \#199151, \#199194
 * @subsection InterruptDisableIsr_TC_05_005_002_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * @subsection InterruptDisableIsr_TC_05_005_002_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptDisableIsr APIs with abnormal condition:
 * - Test05_005_002: disable interrupt with device handle is NULL.
 * @subsection InterruptDisableIsr_TC_05_005_002_tpts Test Procedure/Test Steps
 * -# Disable interrupt handler by using R_OSAL_InterruptDisableIsr with the arguments as below:
 *      - Device handle is NULL
 *      - Interrupt channel is 0
 * @subsection InterruptDisableIsr_TC_05_005_002_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_HANDLE.
 * @subsection InterruptDisableIsr_TC_05_005_002_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptDisableIsr_TC_05_005_002
 */
// [InterruptDisableIsr_TC_05_005_002]
TEST_F(INTApiTest, InterruptDisableIsr_TC_05_005_002)
{
    osal_device_handle_t p_device_handle = NULL;

    // Test05_005_002
    // @brief: abnormal case: disable interrupt for device handle is NULL
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(p_device_handle, TEST_IRQ_CHANNEL_DEVICE_INVALID), OSAL_RETURN_HANDLE) << "Test05_005_002 failed";
}
// [InterruptDisableIsr_TC_05_005_002]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptDisableIsr_TC_05_005_003 INTApiTest.InterruptDisableIsr_TC_05_005_003
 * @subsection InterruptDisableIsr_TC_05_005_003_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptDisableIsr_TC_05_005_003_tci Test Case ID
 * InterruptDisableIsr_S1_A_TC_05_005_003
 * @subsection InterruptDisableIsr_TC_05_005_003_rr Related Requirement(s)
 * \#199194
 * @subsection InterruptDisableIsr_TC_05_005_003_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptDisableIsr_TC_05_005_003_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptDisableIsr APIs with abnormal condition:
 * - Test05_005_003: disable interrupt with device handle has already been closed.
 * @subsection InterruptDisableIsr_TC_05_005_003_tpts Test Procedure/Test Steps
 * -# Close test target device by using R_OSAL_IoDeviceClose.
 * -# Disable interrupt handler by using R_OSAL_InterruptDisableIsr with the arguments as below:
 *      - Device handle has already been closed at step 1.
 *      - Interrupt channel is 0
 * -# Re-open test target device for the next test cases by using R_OSAL_IoDeviceOpen.
 * @subsection InterruptDisableIsr_TC_05_005_003_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_HANDLE.
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * @subsection InterruptDisableIsr_TC_05_005_003_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptDisableIsr_TC_05_005_003
 */
// [InterruptDisableIsr_TC_05_005_003]
TEST_F(INTApiTest, InterruptDisableIsr_TC_05_005_003)
{
    // Test05_005_003
    // @brief: abnormal case: disable interrupt for device handle is closed
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID), OSAL_RETURN_HANDLE) << "Test05_005_003 failed";
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_INT_DEVICE_1, &device_handle), OSAL_RETURN_OK);
    EXPECT_NE(device_handle, NULL_PTR);
}
// [InterruptDisableIsr_TC_05_005_003]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptDisableIsr_TC_05_005_004 INTApiTest.InterruptDisableIsr_TC_05_005_004
 * @subsection InterruptDisableIsr_TC_05_005_004_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptDisableIsr_TC_05_005_004_tci Test Case ID
 * InterruptDisableIsr_S0_BA_TC_05_005_004
 * @subsection InterruptDisableIsr_TC_05_005_004_rr Related Requirement(s)
 * \#199194
 * @subsection InterruptDisableIsr_TC_05_005_004_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptDisableIsr_TC_05_005_004_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptDisableIsr APIs with abnormal condition:
 * - Test05_005_004: disable interrupt with interrupt channel is invalid.
 * @subsection InterruptDisableIsr_TC_05_005_004_tpts Test Procedure/Test Steps
 * -# Get the number of interrupt channel corresponding with opened device handle test target device by calling to R_OSAL_InterruptGetNumOfIrqChannels.
 * -# Disable interrupt handler by using R_OSAL_InterruptDisableIsr with the arguments as below:
 *      - Device handle does not have a interrupt handler registered by R_OSAL_InterruptRegisterIsr.
 *      - Interrupt channel is out-of-valid range: [0 -- "numOfChannel").<br>
 *        "numOfChannel" got at step 1.
 * @subsection InterruptDisableIsr_TC_05_005_004_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * Value of "numOfChannel" is bigger than 0.
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_PAR.
 * @subsection InterruptDisableIsr_TC_05_005_004_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptDisableIsr_TC_05_005_004
 */
// [InterruptDisableIsr_TC_05_005_004]
TEST_F(INTApiTest, InterruptDisableIsr_TC_05_005_004)
{
    size_t numOfChannel = 0;

    // Test05_005_004
    // @brief: abnormal case: disable interrupt with irq_channel is invalid
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(device_handle, numOfChannel), OSAL_RETURN_PAR) << "Test05_005_004 failed";
}
// [InterruptDisableIsr_TC_05_005_004]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptDisableIsr_TC_05_005_005 INTApiTest.InterruptDisableIsr_TC_05_005_005
 * @subsection InterruptDisableIsr_TC_05_005_005_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptDisableIsr_TC_05_005_005_tci Test Case ID
 * InterruptDisableIsr_S2_A_TC_05_005_005
 * @subsection InterruptDisableIsr_TC_05_005_005_rr Related Requirement(s)
 * \#199194
 * @subsection InterruptDisableIsr_TC_05_005_005_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptDisableIsr_TC_05_005_005_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptDisableIsr APIs with abnormal condition:
 * - Test05_005_005: disable interrupt with interrupt handler is not registered.
 * @subsection InterruptDisableIsr_TC_05_005_005_tpts Test Procedure/Test Steps
 * -# Disable interrupt handler by using R_OSAL_InterruptDisableIsr with the arguments as below:
 *      - Device handle does not have a interrupt handler registered by R_OSAL_InterruptRegisterIsr.
 *      - Interrupt channel is 0
 * @subsection InterruptDisableIsr_TC_05_005_005_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_FAIL.
 * @subsection InterruptDisableIsr_TC_05_005_005_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptDisableIsr_TC_05_005_005
 */
// [InterruptDisableIsr_TC_05_005_005]
TEST_F(INTApiTest, InterruptDisableIsr_TC_05_005_005)
{
    // Test05_005_005
    // @brief: abnormal case: disable interrupt with irq_channel has not been registered
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID), OSAL_RETURN_FAIL) << "Test05_005_005 failed";
}
// [InterruptDisableIsr_TC_05_005_005]

/**
 * @page INT_TEST Interrupt API Test
 * @section OsalApiStateTest_InterruptDisableIsr_TC_05_005_006 OsalApiStateTest.InterruptDisableIsr_TC_05_005_006
 * @subsection InterruptDisableIsr_TC_05_005_006_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection InterruptDisableIsr_TC_05_005_006_tci Test Case ID
 * InterruptDisableIsr_S2_A_TC_05_005_006
 * @subsection InterruptDisableIsr_TC_05_005_006_rr Related Requirement(s)
 * \#199194
 * @subsection InterruptDisableIsr_TC_05_005_006_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection InterruptDisableIsr_TC_05_005_006_tcs Test Case Summary
 * This test shall verify the state transition of Clock Time APIs on abnormal condition:
 * - Test05_005_006: state transition error of R_OSAL_InterruptDisableIsr
 * @subsection InterruptDisableIsr_TC_05_005_006_tpts Test Procedure/Test Steps
 * -# Disable interrupt handler by using R_OSAL_InterruptDisableIsr.
 * @subsection InterruptDisableIsr_TC_05_005_006_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_STATE.
 * @subsection InterruptDisableIsr_TC_05_005_006_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptDisableIsr_TC_05_005_006
 */
// [InterruptDisableIsr_TC_05_005_006]
TEST_F(OsalApiStateTest, InterruptDisableIsr_TC_05_005_006)
{
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;

    // Test05_005_006
    // @brief: abnormal case: state transition error of R_OSAL_InterruptDisableIsr
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID), OSAL_RETURN_STATE) << "Test05_005_006 failed";
}
// [InterruptDisableIsr_TC_05_005_006]

/**
 * @page INT_TEST Interrupt API Test
 * @section OSALINTTESTS_InterruptDisableIsr_TC_05_005_007 OSALINTTESTS.InterruptDisableIsr_TC_05_005_007
 * @subsection InterruptDisableIsr_TC_05_005_007_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection InterruptDisableIsr_TC_05_005_007_tci Test Case ID
 * InterruptDisableIsr_S3_N_TC_05_005_007
 * @subsection InterruptDisableIsr_TC_05_005_007_rr Related Requirement(s)
 * \#199193, \#199194, \#199195, \#230924, \#276767
 * @subsection InterruptDisableIsr_TC_05_005_007_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection InterruptDisableIsr_TC_05_005_007_tcs Test Case Summary
 * @subsubsection InterruptDisableIsr_TC_05_005_007_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Setup required memory for interrupt generation
 * - Register callback function and enable interrupt
 * - Wait for interrupt callback finish
 * @subsubsection InterruptDisableIsr_TC_05_005_007_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister the interrupt callback function of test target device registered in the interrupt context 
 * - Deallocate allocated memory
 * @subsubsection InterruptDisableIsr_TC_05_005_007_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality of the Interrupt APIs which are called in the ISR with normal condition.
 * - Test05_005_007: Disable interrupt for test target device in ISR and clear status register
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection InterruptDisableIsr_TC_05_005_007_tpts Test Procedure/Test Steps
 * @subsubsection InterruptDisableIsr_TC_05_005_007_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Allocate required memory for generate interrupt from test target device and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function by using R_OSAL_InterruptRegisterIsr
 * -# Enable interrupt by using R_OSAL_InterruptEnableIsr
 * -# Setup test target device specific register to generate the interrupt by calling to setup_hw_register
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection InterruptDisableIsr_TC_05_005_007_post_tpts Test Procedure/Test Steps of Post-process
 * -# Unregister interrupt by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * @subsubsection InterruptDisableIsr_TC_05_005_007_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt for test target device immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Send Message interrupt of context finished by using R_OSAL_MqSendForTimePeriod
 * @subsection InterruptDisableIsr_TC_05_005_007_ereb Expected Result/Behavior
 * @subsubsection InterruptDisableIsr_TC_05_005_007_pre_ereb Expected Result/Behavior of Pre-process
 * -# allocate_setup_data_memory run successfully
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * The value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK
 * -# setup_hw_register run successfully
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection InterruptDisableIsr_TC_05_005_007_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * @subsubsection InterruptDisableIsr_TC_05_005_007_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * @subsection InterruptDisableIsr_TC_05_005_007_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptDisableIsr_TC_05_005_007
 * @snippet test_interrupt.cpp osal_test_int_cb_TC_05_005_007
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [InterruptDisableIsr_TC_05_005_007]
TEST_F(OSALINTTESTS, InterruptDisableIsr_TC_05_005_007)
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
 * Register callback function and enable interrupt.
 * Wait for interrupt callback finish */
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_test_int_cb_TC_05_005_007, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt for registered interrupt of test target device
 * Deallocate allocated memory */

    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_test_int_cb_TC_05_005_007), OSAL_RETURN_OK);
    deallocate_data_memory(hndl_mmngr, buffer_obj);
}
// [InterruptDisableIsr_TC_05_005_007]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptDisableIsr_TC_05_005_009 INTApiTest.InterruptDisableIsr_TC_05_005_009
 * @subsection InterruptDisableIsr_TC_05_005_009_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptDisableIsr_TC_05_005_009_tci Test Case ID
 * InterruptDisableIsr_S4_A_TC_05_005_009
 * @subsection InterruptDisableIsr_TC_05_005_009_rr Related Requirement(s)
 * \#199194
 * @subsection InterruptDisableIsr_TC_05_005_009_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptDisableIsr_TC_05_005_009_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptDisableIsr APIs with abnormal condition:
 * - Test05_005_009: disable interrupt with test target device which does not have interrupt channel.
 * @subsection InterruptDisableIsr_TC_05_005_009_tpts Test Procedure/Test Steps
 * -# Disable interrupt handler by using R_OSAL_InterruptDisableIsr with the arguments as below:
 *      - Device handle of test target device
 *      - Interrupt channel is 0
 * @subsection InterruptDisableIsr_TC_05_005_009_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_HANDLE.
 * @subsection InterruptDisableIsr_TC_05_005_009_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptDisableIsr_TC_05_005_009
 */
// [InterruptDisableIsr_TC_05_005_009]
TEST_F(INTApiTest, InterruptDisableIsr_TC_05_005_009)
{
    // Test05_005_009
    // @brief: abnormal case: disable interrupt with test target device which does not have interrupt channel
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(device_handle1, TEST_IRQ_CHANNEL_DEVICE_INVALID), OSAL_RETURN_HANDLE) << "Test05_005_009 failed";
}
// [InterruptDisableIsr_TC_05_005_009]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptSetIsrPriority_TC_05_006_001 INTApiTest.InterruptSetIsrPriority_TC_05_006_001
 * @subsection InterruptSetIsrPriority_TC_05_006_001_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptSetIsrPriority_TC_05_006_001_tci Test Case ID
 * InterruptSetIsrPriority_S1_N_TC_05_006_001
 * @subsection InterruptSetIsrPriority_TC_05_006_001_rr Related Requirement(s)
 * \#199196
 * @subsection InterruptSetIsrPriority_TC_05_006_001_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptSetIsrPriority_TC_05_006_001_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptSetIsrPriority:
 * - pre-process: get the number of interrupt channel, register and enable interrupt callback function related to channel 0
 * - Test05_006_001: change interrupt priority to OSAL_INTERRUPT_PRIORITY_TYPE11.
 * - post-process: disable and unregister interrupt channel TEST_IRQ_CHANNEL_DEVICE_VALID
 * @subsection InterruptSetIsrPriority_TC_05_006_001_tpts Test Procedure/Test Steps
 * -# Get the number of interrupt channel corresponding with opened device handle test target device by calling to R_OSAL_InterruptGetNumOfIrqChannels.
 * -# Register interrupt handler for test target device with priority is OSAL_INTERRUPT_PRIORITY_TYPE10 by calling to R_OSAL_InterruptRegisterIsr.
 * -# Enable interrupt handler which has already been registered at step 2 by using R_OSAL_InterruptEnableIsr.
 * -# Change interrupt priority to OSAL_INTERRUPT_PRIORITY_TYPE11 by using R_OSAL_InterruptSetIsrPriority.
 * -# Disable interrupt handler which has already been enabled at step 3 by using R_OSAL_InterruptDisableIsr.
 * -# Unregister interrupt callback "osal_test_cb_sample" registered at step 2 by calling to R_OSAL_InterruptUnregisterIsr.
 * @subsection InterruptSetIsrPriority_TC_05_006_001_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * Value of "numOfChannel" is bigger than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptSetIsrPriority return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK.
 * @subsection InterruptSetIsrPriority_TC_05_006_001_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptSetIsrPriority_TC_05_006_001
 * @snippet test_interrupt.cpp osal_test_cb_sample
 */
// [InterruptSetIsrPriority_TC_05_006_001]
TEST_F(INTApiTest, InterruptSetIsrPriority_TC_05_006_001)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE10;
    int arg = 10;

    // pre-process: Register and Enable interrupt handle for test target device
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_GT(numOfChannel, 0) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, priority, osal_test_cb_sample, (void *)&arg), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID), OSAL_RETURN_OK) << "pre-process failed";

    // Test05_006_001
    // @brief: normal case: change interrupt priority to OSAL_INTERRUPT_PRIORITY_TYPE11
    EXPECT_EQ(R_OSAL_InterruptSetIsrPriority(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, OSAL_INTERRUPT_PRIORITY_TYPE11), OSAL_RETURN_OK) << "Test05_006_001 failed";

    // post-process: Unable and Unregister interrupt
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, osal_test_cb_sample), OSAL_RETURN_OK) << "post-process failed";
}
// [InterruptSetIsrPriority_TC_05_006_001]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptSetIsrPriority_TC_05_006_002 INTApiTest.InterruptSetIsrPriority_TC_05_006_002
 * @subsection InterruptSetIsrPriority_TC_05_006_002_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptSetIsrPriority_TC_05_006_002_tci Test Case ID
 * InterruptSetIsrPriority_S1_BN_TC_05_006_002
 * @subsection InterruptSetIsrPriority_TC_05_006_002_rr Related Requirement(s)
 * \#199196, \#294296
 * @subsection InterruptSetIsrPriority_TC_05_006_002_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptSetIsrPriority_TC_05_006_002_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptSetIsrPriority:
 * - pre-process: get the number of interrupt channel, register and enable interrupt callback function related to channel 0
 * - Test05_006_002: change interrupt priority to OSAL_INTERRUPT_PRIORITY_LOWEST.
 * - post-process: disable and unregister interrupt channel TEST_IRQ_CHANNEL_DEVICE_VALID
 * @subsection InterruptSetIsrPriority_TC_05_006_002_tpts Test Procedure/Test Steps
 * -# Get the number of interrupt channel corresponding with opened device handle test target device by calling to R_OSAL_InterruptGetNumOfIrqChannels.
 * -# Register interrupt handler for test target device with priority is OSAL_INTERRUPT_PRIORITY_TYPE10 by calling to R_OSAL_InterruptRegisterIsr.
 * -# Enable interrupt handler which has already been registered at step 2 by using R_OSAL_InterruptEnableIsr.
 * -# Change interrupt priority to OSAL_INTERRUPT_PRIORITY_LOWEST by using R_OSAL_InterruptSetIsrPriority.
 * -# Disable interrupt handler which has already been enabled at step 3 by using R_OSAL_InterruptDisableIsr.
 * -# Unregister interrupt callback "osal_test_cb_sample" registered at step 2 by calling to R_OSAL_InterruptUnregisterIsr.
 * @subsection InterruptSetIsrPriority_TC_05_006_002_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * Value of "numOfChannel" is bigger than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptSetIsrPriority return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK.
 * @subsection InterruptSetIsrPriority_TC_05_006_002_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptSetIsrPriority_TC_05_006_002
 * @snippet test_interrupt.cpp osal_test_cb_sample
 */
// [InterruptSetIsrPriority_TC_05_006_002]
TEST_F(INTApiTest, InterruptSetIsrPriority_TC_05_006_002)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE10;
    int arg = 10;

    // pre-process: Register and Enable interrupt handle for test target device
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_GT(numOfChannel, 0) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, priority, osal_test_cb_sample, (void *)&arg), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID), OSAL_RETURN_OK) << "pre-process failed";

    // Test05_006_002
    // @brief: normal case: change interrupt priority to OSAL_INTERRUPT_PRIORITY_LOWEST
    EXPECT_EQ(R_OSAL_InterruptSetIsrPriority(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, OSAL_INTERRUPT_PRIORITY_LOWEST), OSAL_RETURN_OK) << "Test05_006_002 failed";

    // post-process: Unable and Unregister interrupt
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, osal_test_cb_sample), OSAL_RETURN_OK) << "post-process failed";
}
// [InterruptSetIsrPriority_TC_05_006_002]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptSetIsrPriority_TC_05_006_003 INTApiTest.InterruptSetIsrPriority_TC_05_006_003
 * @subsection InterruptSetIsrPriority_TC_05_006_003_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptSetIsrPriority_TC_05_006_003_tci Test Case ID
 * InterruptSetIsrPriority_S1_BN_TC_05_006_003
 * @subsection InterruptSetIsrPriority_TC_05_006_003_rr Related Requirement(s)
 * \#199196, \#294296
 * @subsection InterruptSetIsrPriority_TC_05_006_003_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptSetIsrPriority_TC_05_006_003_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptSetIsrPriority:
 * - pre-process: get the number of interrupt channel, register and enable interrupt callback function related to channel 0
 * - Test05_006_003: change interrupt priority to OSAL_INTERRUPT_PRIORITY_HIGHEST.
 * - post-process: disable and unregister interrupt channel TEST_IRQ_CHANNEL_DEVICE_VALID
 * @subsection InterruptSetIsrPriority_TC_05_006_003_tpts Test Procedure/Test Steps
 * -# Get the number of interrupt channel corresponding with opened device handle test target device by calling to R_OSAL_InterruptGetNumOfIrqChannels.
 * -# Register interrupt handler for test target device with priority is OSAL_INTERRUPT_PRIORITY_TYPE10 by calling to R_OSAL_InterruptRegisterIsr.
 * -# Enable interrupt handler which has already been registered at step 2 by using R_OSAL_InterruptEnableIsr.
 * -# Change interrupt priority to OSAL_INTERRUPT_PRIORITY_HIGHEST by using R_OSAL_InterruptSetIsrPriority.
 * -# Disable interrupt handler which has already been enabled at step 3 by using R_OSAL_InterruptDisableIsr.
 * -# Unregister interrupt callback "osal_test_cb_sample" registered at step 2 by calling to R_OSAL_InterruptUnregisterIsr.
 * @subsection InterruptSetIsrPriority_TC_05_006_003_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * Value of "numOfChannel" is bigger than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptSetIsrPriority return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK.
 * @subsection InterruptSetIsrPriority_TC_05_006_003_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptSetIsrPriority_TC_05_006_003
 * @snippet test_interrupt.cpp osal_test_cb_sample
 */
// [InterruptSetIsrPriority_TC_05_006_003]
TEST_F(INTApiTest, InterruptSetIsrPriority_TC_05_006_003)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE10;
    int arg = 10;

    // pre-process: Register and Enable interrupt handle for test target device
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_GT(numOfChannel, 0) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, priority, osal_test_cb_sample, (void *)&arg), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID), OSAL_RETURN_OK) << "pre-process failed";

    // Test05_006_003
    // @brief: normal case: change interrupt priority to OSAL_INTERRUPT_PRIORITY_HIGHEST
    EXPECT_EQ(R_OSAL_InterruptSetIsrPriority(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, OSAL_INTERRUPT_PRIORITY_HIGHEST), OSAL_RETURN_OK) << "Test05_006_003 failed";

    // post-process: Unable and Unregister interrupt
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, osal_test_cb_sample), OSAL_RETURN_OK) << "post-process failed";
}
// [InterruptSetIsrPriority_TC_05_006_003]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptSetIsrPriority_TC_05_006_004 INTApiTest.InterruptSetIsrPriority_TC_05_006_004
 * @subsection InterruptSetIsrPriority_TC_05_006_004_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptSetIsrPriority_TC_05_006_004_tci Test Case ID
 * InterruptSetIsrPriority_S0_A_TC_05_006_004
 * @subsection InterruptSetIsrPriority_TC_05_006_004_rr Related Requirement(s)
 * \#199151, \#199196
 * @subsection InterruptSetIsrPriority_TC_05_006_004_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * @subsection InterruptSetIsrPriority_TC_05_006_004_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptSetIsrPriority with normal condition:
 * - Test05_006_004: change the priority with device handle is NULL
 * @subsection InterruptSetIsrPriority_TC_05_006_004_tpts Test Procedure/Test Steps
 * -# Change interrupt priority by using R_OSAL_InterruptSetIsrPriority with the arguments as below:
 *      - Device handle is NULL
 *      - Interrupt channel is 0
 *      - Interrupt priority is OSAL_INTERRUPT_PRIORITY_TYPE11
 * @subsection InterruptSetIsrPriority_TC_05_006_004_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptSetIsrPriority return OSAL_RETURN_HANDLE.
 * @subsection InterruptSetIsrPriority_TC_05_006_004_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptSetIsrPriority_TC_05_006_004
 */
// [InterruptSetIsrPriority_TC_05_006_004]
TEST_F(INTApiTest, InterruptSetIsrPriority_TC_05_006_004)
{
    osal_device_handle_t p_device_handle = NULL;

    // Test05_006_004
    // @brief: normal case: device handle is NULL
    EXPECT_EQ(R_OSAL_InterruptSetIsrPriority(p_device_handle, TEST_IRQ_CHANNEL_DEVICE_INVALID, OSAL_INTERRUPT_PRIORITY_TYPE11), OSAL_RETURN_HANDLE) << "Test05_006_004 failed";
}
// [InterruptSetIsrPriority_TC_05_006_004]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptSetIsrPriority_TC_05_006_005 INTApiTest.InterruptSetIsrPriority_TC_05_006_005
 * @subsection InterruptSetIsrPriority_TC_05_006_005_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptSetIsrPriority_TC_05_006_005_tci Test Case ID
 * InterruptSetIsrPriority_S1_A_TC_05_006_005
 * @subsection InterruptSetIsrPriority_TC_05_006_005_rr Related Requirement(s)
 * \#199196
 * @subsection InterruptSetIsrPriority_TC_05_006_005_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptSetIsrPriority_TC_05_006_005_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptSetIsrPriority with normal condition:
 * - Test05_006_005: change the priority with device handle has already been closed
 * @subsection InterruptSetIsrPriority_TC_05_006_005_tpts Test Procedure/Test Steps
 * -# Close test target device by using R_OSAL_IoDeviceClose.
 * -# Change interrupt priority by using R_OSAL_InterruptSetIsrPriority with the arguments as below:
 *      - Device handle of test target device which has already been closed at step 1.
 *      - Interrupt channel is 0
 *      - Interrupt priority is OSAL_INTERRUPT_PRIORITY_TYPE11
 * -# Re-open test target device for the next test cases by using R_OSAL_IoDeviceOpen
 * @subsection InterruptSetIsrPriority_TC_05_006_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptSetIsrPriority return OSAL_RETURN_HANDLE.
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * @subsection InterruptSetIsrPriority_TC_05_006_005_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptSetIsrPriority_TC_05_006_005
 */
// [InterruptSetIsrPriority_TC_05_006_005]
TEST_F(INTApiTest, InterruptSetIsrPriority_TC_05_006_005)
{
    // Test05_006_005
    // @brief: abnormal case: device handle is closed
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptSetIsrPriority(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, OSAL_INTERRUPT_PRIORITY_TYPE11), OSAL_RETURN_HANDLE) << "Test05_006_005 failed";
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_INT_DEVICE_1, &device_handle), OSAL_RETURN_OK);
    EXPECT_NE(device_handle, NULL_PTR);
}
// [InterruptSetIsrPriority_TC_05_006_005]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptSetIsrPriority_TC_05_006_006 INTApiTest.InterruptSetIsrPriority_TC_05_006_006
 * @subsection InterruptSetIsrPriority_TC_05_006_006_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptSetIsrPriority_TC_05_006_006_tci Test Case ID
 * InterruptSetIsrPriority_S0_A_TC_05_006_006
 * @subsection InterruptSetIsrPriority_TC_05_006_006_rr Related Requirement(s)
 * \#199196
 * @subsection InterruptSetIsrPriority_TC_05_006_006_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptSetIsrPriority_TC_05_006_006_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptSetIsrPriority with normal condition:
 * - Test05_006_006: change the priority with invalid interrupt channel
 * @subsection InterruptSetIsrPriority_TC_05_006_006_tpts Test Procedure/Test Steps
 * -# Get the number of interrupt channel corresponding with opened device handle test target device by calling to R_OSAL_InterruptGetNumOfIrqChannels.
 * -# Change interrupt priority by using R_OSAL_InterruptSetIsrPriority with the arguments as below:
 *      - Device handle of test target device
 *      - Interrupt channel is out-of-valid range: [0 -- "numOfChannel").<br>
 *        "numOfChannel" got at step 1.
 *      - Interrupt priority is OSAL_INTERRUPT_PRIORITY_TYPE11
 * @subsection InterruptSetIsrPriority_TC_05_006_006_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * Value of "numOfChannel" is bigger than 0.
 * -# R_OSAL_InterruptSetIsrPriority return OSAL_RETURN_PAR.
 * @subsection InterruptSetIsrPriority_TC_05_006_006_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptSetIsrPriority_TC_05_006_006
 */
// [InterruptSetIsrPriority_TC_05_006_006]
TEST_F(INTApiTest, InterruptSetIsrPriority_TC_05_006_006)
{
    size_t numOfChannel = 0;

    // Test05_006_006
    // @brief: abnormal case: irq_channel is invalid
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptSetIsrPriority(device_handle, numOfChannel, OSAL_INTERRUPT_PRIORITY_TYPE11), OSAL_RETURN_PAR) << "Test05_006_006 failed";
}
// [InterruptSetIsrPriority_TC_05_006_006]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptSetIsrPriority_TC_05_006_007 INTApiTest.InterruptSetIsrPriority_TC_05_006_007
 * @subsection InterruptSetIsrPriority_TC_05_006_007_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptSetIsrPriority_TC_05_006_007_tci Test Case ID
 * InterruptSetIsrPriority_S0_BA_TC_05_006_007
 * @subsection InterruptSetIsrPriority_TC_05_006_007_rr Related Requirement(s)
 * \#199196, \#294296
 * @subsection InterruptSetIsrPriority_TC_05_006_007_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptSetIsrPriority_TC_05_006_007_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptSetIsrPriority with normal condition:
 * - Test05_006_007: check with interrupt priority is OSAL_INTERRUPT_PRIORITY_LOWEST - 1
 * @subsection InterruptSetIsrPriority_TC_05_006_007_tpts Test Procedure/Test Steps
 * -# Get the number of interrupt channel corresponding with opened device handle test target device by calling to R_OSAL_InterruptGetNumOfIrqChannels.
 * -# Register interrupt handler for test target device with priority is OSAL_INTERRUPT_PRIORITY_TYPE10 by calling to R_OSAL_InterruptRegisterIsr.
 * -# Change interrupt priority by using R_OSAL_InterruptSetIsrPriority with the arguments as below:
 *      - Device handle of test target device
 *      - Interrupt channel is 0
 *      - Interrupt priority is OSAL_INTERRUPT_PRIORITY_LOWEST - 1
 * -# Unregister interrupt callback "osal_test_cb_sample" registered at step 2 by calling to R_OSAL_InterruptUnregisterIsr.
 * @subsection InterruptSetIsrPriority_TC_05_006_007_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * Value of "numOfChannel" is bigger than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptSetIsrPriority return OSAL_RETURN_PAR.
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK.
 * @subsection InterruptSetIsrPriority_TC_05_006_007_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptSetIsrPriority_TC_05_006_007
 * @snippet test_interrupt.cpp osal_test_cb_sample
 */
// [InterruptSetIsrPriority_TC_05_006_007]
TEST_F(INTApiTest, InterruptSetIsrPriority_TC_05_006_007)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE10;
    int arg = 10;

    // Test05_006_007
    // @brief: abnormal case: priority is invalid
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, priority, osal_test_cb_sample, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptSetIsrPriority(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, (e_osal_interrupt_priority_t)(OSAL_INTERRUPT_PRIORITY_LOWEST - 1)), OSAL_RETURN_PAR) << "Test05_006_007 failed";
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, osal_test_cb_sample), OSAL_RETURN_OK);
}
// [InterruptSetIsrPriority_TC_05_006_007]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptSetIsrPriority_TC_05_006_008 INTApiTest.InterruptSetIsrPriority_TC_05_006_008
 * @subsection InterruptSetIsrPriority_TC_05_006_008_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptSetIsrPriority_TC_05_006_008_tci Test Case ID
 * InterruptSetIsrPriority_S0_BA_TC_05_006_008
 * @subsection InterruptSetIsrPriority_TC_05_006_008_rr Related Requirement(s)
 * \#199196, \#294296
 * @subsection InterruptSetIsrPriority_TC_05_006_008_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptSetIsrPriority_TC_05_006_008_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptSetIsrPriority with normal condition:
 * - Test05_006_008: check with interrupt priority is OSAL_INTERRUPT_PRIORITY_HIGHEST + 1
 * @subsection InterruptSetIsrPriority_TC_05_006_008_tpts Test Procedure/Test Steps
 * -# Get the number of interrupt channel corresponding with opened device handle test target device by calling to R_OSAL_InterruptGetNumOfIrqChannels.
 * -# Register interrupt handler for test target device with priority is OSAL_INTERRUPT_PRIORITY_TYPE10 by calling to R_OSAL_InterruptRegisterIsr.
 * -# Change interrupt priority by using R_OSAL_InterruptSetIsrPriority with the arguments as below:
 *      - Device handle of test target device
 *      - Interrupt channel is 0
 *      - Interrupt priority is OSAL_INTERRUPT_PRIORITY_HIGHEST + 1
 * -# Unregister interrupt callback "osal_test_cb_sample" registered at step 2 by calling to R_OSAL_InterruptUnregisterIsr.
 * @subsection InterruptSetIsrPriority_TC_05_006_008_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * Value of "numOfChannel" is bigger than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK.
 * -# R_OSAL_InterruptSetIsrPriority return OSAL_RETURN_PAR.
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK.
 * @subsection InterruptSetIsrPriority_TC_05_006_008_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptSetIsrPriority_TC_05_006_008
 * @snippet test_interrupt.cpp osal_test_cb_sample
 */
// [InterruptSetIsrPriority_TC_05_006_008]
TEST_F(INTApiTest, InterruptSetIsrPriority_TC_05_006_008)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE10;
    int arg = 10;

    // Test05_006_008
    // @brief: abnormal case: priority is invalid
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, priority, osal_test_cb_sample, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptSetIsrPriority(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, (e_osal_interrupt_priority_t)(OSAL_INTERRUPT_PRIORITY_HIGHEST + 1)), OSAL_RETURN_PAR) << "Test05_006_008 failed";
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, osal_test_cb_sample), OSAL_RETURN_OK);
}
// [InterruptSetIsrPriority_TC_05_006_008]

/**
 * @page INT_TEST Interrupt API Test
 * @section OsalApiStateTest_InterruptSetIsrPriority_TC_05_006_009 OsalApiStateTest.InterruptSetIsrPriority_TC_05_006_009
 * @subsection InterruptSetIsrPriority_TC_05_006_009_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection InterruptSetIsrPriority_TC_05_006_009_tci Test Case ID
 * InterruptSetIsrPriority_S1_A_TC_05_006_009
 * @subsection InterruptSetIsrPriority_TC_05_006_009_rr Related Requirement(s)
 * \#199196
 * @subsection InterruptSetIsrPriority_TC_05_006_009_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection InterruptSetIsrPriority_TC_05_006_009_tcs Test Case Summary
 * This test shall verify the state transition of Clock Time APIs on abnormal condition:
 * - Test05_006_009: state transition error of R_OSAL_InterruptSetIsrPriority
 * @subsection InterruptSetIsrPriority_TC_05_006_009_tpts Test Procedure/Test Steps
 * -# Change interrupt priority by using R_OSAL_InterruptSetIsrPriority.
 * @subsection InterruptSetIsrPriority_TC_05_006_009_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptSetIsrPriority return OSAL_RETURN_STATE.
 * @subsection InterruptSetIsrPriority_TC_05_006_009_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptSetIsrPriority_TC_05_006_009
 */
// [InterruptSetIsrPriority_TC_05_006_009]
TEST_F(OsalApiStateTest, InterruptSetIsrPriority_TC_05_006_009)
{
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;

    // Test05_006_009
    // @brief: abnormal case: state transition error of R_OSAL_InterruptSetIsrPriority
    EXPECT_EQ(R_OSAL_InterruptSetIsrPriority(device_handle, TEST_IRQ_CHANNEL_DEVICE_VALID, OSAL_INTERRUPT_PRIORITY_LOWEST), OSAL_RETURN_STATE) << "Test05_006_009 failed";
}
// [InterruptSetIsrPriority_TC_05_006_009]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptSetIsrPriority_TC_05_006_011 INTApiTest.InterruptSetIsrPriority_TC_05_006_011
 * @subsection InterruptSetIsrPriority_TC_05_006_011_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptSetIsrPriority_TC_05_006_011_tci Test Case ID
 * InterruptSetIsrPriority_S4_A_TC_05_006_011
 * @subsection InterruptSetIsrPriority_TC_05_006_011_rr Related Requirement(s)
 * \#199196
 * @subsection InterruptSetIsrPriority_TC_05_006_011_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# Device handle is opened test target device.
 * @subsection InterruptSetIsrPriority_TC_05_006_011_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptSetIsrPriority with normal condition:
 * - Test05_006_011: change the priority with test target device which does not have interrupt channel
 * @subsection InterruptSetIsrPriority_TC_05_006_011_tpts Test Procedure/Test Steps
 * -# Change interrupt priority by using R_OSAL_InterruptSetIsrPriority with the arguments as below:
 *      - Device handle of test target device
 *      - Interrupt channel is 0
 *      - Interrupt priority is OSAL_INTERRUPT_PRIORITY_TYPE13
 * @subsection InterruptSetIsrPriority_TC_05_006_011_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptSetIsrPriority return OSAL_RETURN_HANDLE.
 * @subsection InterruptSetIsrPriority_TC_05_006_011_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptSetIsrPriority_TC_05_006_011
 */
// [InterruptSetIsrPriority_TC_05_006_011]
TEST_F(INTApiTest, InterruptSetIsrPriority_TC_05_006_011)
{
    // Test05_006_011
    // @brief: abnormal case: confirm with test target device which does not have interrupt channel
    EXPECT_EQ(R_OSAL_InterruptSetIsrPriority(device_handle1, TEST_IRQ_CHANNEL_DEVICE_INVALID, (e_osal_interrupt_priority_t)(OSAL_INTERRUPT_PRIORITY_TYPE13)), OSAL_RETURN_HANDLE) << "Test05_006_011 failed";
}
// [InterruptSetIsrPriority_TC_05_006_011]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptIsISRContext_TC_05_007_001 INTApiTest.InterruptIsISRContext_TC_05_007_001
 * @subsection InterruptIsISRContext_TC_05_007_001_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptIsISRContext_TC_05_007_001_tci Test Case ID
 * InterruptIsISRContext_S1_N_TC_05_007_001
 * @subsection InterruptIsISRContext_TC_05_007_001_rr Related Requirement(s)
 * \#199195
 * @subsection InterruptIsISRContext_TC_05_007_001_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * @subsection InterruptIsISRContext_TC_05_007_001_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptIsISRContext with normal condition:
 * - Test05_007_001: R_OSAL_InterruptIsISRContext was called by the function is not in the interrupt context.
 * @subsection InterruptIsISRContext_TC_05_007_001_tpts Test Procedure/Test Steps
 * -# Confirm the current context by using R_OSAL_InterruptIsISRContext
 * @subsection InterruptIsISRContext_TC_05_007_001_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptIsISRContext return OSAL_RETURN_OK.<br>
 * "is_isr" is false.
 * @subsection InterruptIsISRContext_TC_05_007_001_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptIsISRContext_TC_05_007_001
 */
// [InterruptIsISRContext_TC_05_007_001]
TEST_F(INTApiTest, InterruptIsISRContext_TC_05_007_001)
{
    bool is_isr;

    // Test05_007_001
    // @brief: normal case: not in the interrupt context.
    EXPECT_EQ(R_OSAL_InterruptIsISRContext(&is_isr), OSAL_RETURN_OK) << "Test05_007_001 failed";
    EXPECT_FALSE(is_isr) << "Test05_007_001 failed";
}
// [InterruptIsISRContext_TC_05_007_001]

/**
 * @page INT_TEST Interrupt API Test
 * @section INTApiTest_InterruptIsISRContext_TC_05_007_002 INTApiTest.InterruptIsISRContext_TC_05_007_002
 * @subsection InterruptIsISRContext_TC_05_007_002_tsi Test Suite ID
 * INTApiTest
 * @subsection InterruptIsISRContext_TC_05_007_002_tci Test Case ID
 * InterruptIsISRContext_S0_A_TC_05_007_002
 * @subsection InterruptIsISRContext_TC_05_007_002_rr Related Requirement(s)
 * \#199195
 * @subsection InterruptIsISRContext_TC_05_007_002_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * @subsection InterruptIsISRContext_TC_05_007_002_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_InterruptIsISRContext with abnormal condition:
 * - Test05_007_002: p_is_isr is NULL.
 * @subsection InterruptIsISRContext_TC_05_007_002_tpts Test Procedure/Test Steps
 * -# Confirm the current context by using R_OSAL_InterruptIsISRContext with the argument as below:
 *      - NULL
 * @subsection InterruptIsISRContext_TC_05_007_002_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptIsISRContext return OSAL_RETURN_PAR.
 * @subsection InterruptIsISRContext_TC_05_007_002_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptIsISRContext_TC_05_007_002
 */
// [InterruptIsISRContext_TC_05_007_002]
TEST_F(INTApiTest, InterruptIsISRContext_TC_05_007_002)
{
    bool *p_is_isr = NULL;

// Test05_007_002
// @brief: abnormal case: p_is_isr is NULL
    EXPECT_EQ(R_OSAL_InterruptIsISRContext(p_is_isr), OSAL_RETURN_PAR) << "Test05_007_002 failed";
}
// [InterruptIsISRContext_TC_05_007_002]

/**
 * @page INT_TEST Interrupt API Test
 * @section OsalApiStateTest_InterruptIsISRContext_TC_05_007_003 OsalApiStateTest.InterruptIsISRContext_TC_05_007_003
 * @subsection InterruptIsISRContext_TC_05_007_003_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection InterruptIsISRContext_TC_05_007_003_tci Test Case ID
 * InterruptIsISRContext_S1_A_TC_05_007_003
 * @subsection InterruptIsISRContext_TC_05_007_003_rr Related Requirement(s)
 * \#199195
 * @subsection InterruptIsISRContext_TC_05_007_003_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection InterruptIsISRContext_TC_05_007_003_tcs Test Case Summary
 * This test shall verify the state transition of Clock Time APIs on abnormal condition:
 * - Test05_007_003: state transition error of R_OSAL_InterruptIsISRContext
 * @subsection InterruptIsISRContext_TC_05_007_003_tpts Test Procedure/Test Steps
 * -# Confirm the current context by using R_OSAL_InterruptIsISRContext
 * @subsection InterruptIsISRContext_TC_05_007_003_ereb Expected Result/Behavior
 * -# R_OSAL_InterruptIsISRContext return OSAL_RETURN_STATE.
 * @subsection InterruptIsISRContext_TC_05_007_003_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptIsISRContext_TC_05_007_003
 */
// [InterruptIsISRContext_TC_05_007_003]
TEST_F(OsalApiStateTest, InterruptIsISRContext_TC_05_007_003)
{
    bool is_isr = false;

    // Test05_007_003
    // @brief: abnormal case: state transition error of R_OSAL_InterruptIsISRContext
    EXPECT_EQ(R_OSAL_InterruptIsISRContext(&is_isr), OSAL_RETURN_STATE) << "Test05_007_003 failed";
}
// [InterruptIsISRContext_TC_05_007_003]

/**
 * @page INT_TEST Interrupt API Test
 * @section OSALINTTESTS_InterruptIsISRContext_TC_05_007_004 OSALINTTESTS.InterruptIsISRContext_TC_05_007_004
 * @subsection InterruptIsISRContext_TC_05_007_004_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection InterruptIsISRContext_TC_05_007_004_tci Test Case ID
 * InterruptIsISRContext_S2_N_TC_05_007_004
 * @subsection InterruptIsISRContext_TC_05_007_004_rr Related Requirement(s)
 * \#199193, \#199194, \#199195, \#230924, \#276767
 * @subsection InterruptIsISRContext_TC_05_007_004_pp Prerequisite/Precondition
 * -# OSAL is initialized.
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection InterruptIsISRContext_TC_05_007_004_tcs Test Case Summary
 * @subsubsection InterruptIsISRContext_TC_05_007_004_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Setup required memory for interrupt generation
 * - Register callback function and enable interrupt
 * - Wait for interrupt callback finish
 * @subsubsection InterruptIsISRContext_TC_05_007_004_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister the interrupt callback function of test target device
 * - Deallocate allocated memory
 * @subsubsection InterruptIsISRContext_TC_05_007_004_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality of the Interrupt APIs which are called in the ISR with normal condition.
 * - Disable interrupt for test target device in ISR and clear status register
 * - Test05_007_004: Check the current context by using R_OSAL_InterruptIsISRContext in ISR
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection InterruptIsISRContext_TC_05_007_004_tpts Test Procedure/Test Steps
 * @subsubsection InterruptIsISRContext_TC_05_007_004_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Allocate required memory for generate interrupt from test target device and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function by using R_OSAL_InterruptRegisterIsr
 * -# Enable interrupt by using R_OSAL_InterruptEnableIsr
 * -# Setup test target device specific register to generate the interrupt by calling to setup_hw_register
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection InterruptIsISRContext_TC_05_007_004_post_tpts Test Procedure/Test Steps of Post-process
 * -# Unregister interrupt by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * @subsubsection InterruptIsISRContext_TC_05_007_004_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt for test target device immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Check the current context by using R_OSAL_InterruptIsISRContext
 * -# Send Message interrupt of context finished by using R_OSAL_MqSendForTimePeriod
 * @subsection InterruptIsISRContext_TC_05_007_004_ereb Expected Result/Behavior
 * @subsubsection InterruptIsISRContext_TC_05_007_004_pre_ereb Expected Result/Behavior of Pre-process
 * -# allocate_setup_data_memory run successfully
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * The value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK
 * -# setup_hw_register run successfully
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection InterruptIsISRContext_TC_05_007_004_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * @subsubsection InterruptIsISRContext_TC_05_007_004_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_InterruptIsISRContext return OSAL_RETURN_OK
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * @subsection InterruptIsISRContext_TC_05_007_004_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptIsISRContext_TC_05_007_004
 * @snippet test_interrupt.cpp osal_test_int_cb_TC_05_007_004
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [InterruptIsISRContext_TC_05_007_004]
TEST_F(OSALINTTESTS, InterruptIsISRContext_TC_05_007_004)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    osal_milli_sec_t time_period = 2000;
    osal_device_handle_t device_handle1 = OSAL_DEVICE_HANDLE_INVALID;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};

/* pre-process:
 * Setup required memory for interrupt generation.
 * Register callback function and enable interrupt.
 * Wait for interrupt callback finish */
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_test_int_cb_TC_05_007_004, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt for registered interrupt of test target device
 * Deallocate allocated memory */

    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_test_int_cb_TC_05_007_004), OSAL_RETURN_OK);
    deallocate_data_memory(hndl_mmngr, buffer_obj);
}
// [InterruptIsISRContext_TC_05_007_004]

/**
 * @page INT_TEST Interrupt API Test
 * @section OSALINTTESTS_InterruptSequence_TC_12_001_023 OSALINTTESTS.InterruptSequence_TC_12_001_023
 * @subsection InterruptSequence_TC_12_001_023_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection InterruptSequence_TC_12_001_023_tci Test Case ID
 * InterruptIsISRContext_S1_A_TC_05_007_003
 * @subsection InterruptSequence_TC_12_001_023_rr Related Requirement(s)
 * \#199193, \#199194
 * @subsection InterruptSequence_TC_12_001_023_pp Prerequisite/Precondition
 * - R_OSAL_Initialize is already called
 * @subsection InterruptSequence_TC_12_001_023_tcs Test Case Summary
 * This test confirmes behavior if number of calls to EnableIsr API is not same to number of calls to DisableIsr API
 * @subsection InterruptSequence_TC_12_001_023_tpts Test Procedure/Test Steps
 * - In Normal Context, Call InterruptEnableIsr 2 times
 * - In Normal Context, Call InterruptDisableIsr 1time
 * - Access to Register of Generate interrupt on target device
 * @subsection InterruptSequence_TC_12_001_023_ereb Expected Result/Behavior
 * Don't Genrate interrupt on target device
 * @subsection InterruptSequence_TC_12_001_023_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptSequence_TC_12_001_023
 */
// [InterruptSequence_TC_12_001_023]
TEST_F(OSALINTTESTS, InterruptSequence_TC_12_001_023)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    osal_milli_sec_t time_period = 2000;
    osal_device_handle_t device_handle1 = OSAL_DEVICE_HANDLE_INVALID;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};

/* pre-process:
 * Setup required memory for interrupt generation.
 * Register callback function and enable interrupt.
 * Wait for interrupt callback finish */
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_int_seq_test_cb_TC_12_001, (void *)&arg), OSAL_RETURN_OK);

    /* Enable -> Enable -> Disable */
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);

    /* Expect: don't generate interrput */
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_TIME);

/* post-process:
 * Unregister interrupt for registered interrupt of test target device
 * Deallocate allocated memory */
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_int_seq_test_cb_TC_12_001), OSAL_RETURN_OK);

    deallocate_data_memory(hndl_mmngr, buffer_obj);
}
// [InterruptSequence_TC_12_001_023]

/**
 * @page INT_TEST Interrupt API Test
 * @section OSALINTTESTS_InterruptSequence_TC_12_001_024 OSALINTTESTS.InterruptSequence_TC_12_001_024
 * @subsection InterruptSequence_TC_12_001_02_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection InterruptSequence_TC_12_001_02_tci Test Case ID
 * InterruptIsISRContext_S1_A_TC_05_007_003
 * @subsection InterruptSequence_TC_12_001_02_rr Related Requirement(s)
 * \#199194
 * @subsection InterruptSequence_TC_12_001_02_pp Prerequisite/Precondition
 * - R_OSAL_Initialize is already called
 * @subsection InterruptSequence_TC_12_001_02_tcs Test Case Summary
 * This test confirmes behavior if number of calls to EnableIsr API is not same to number of calls to DisableIsr API
 * @subsection InterruptSequence_TC_12_001_02_tpts Test Procedure/Test Steps
 * - In Normal Context, Call InterruptEnableIsr 2 times
 * - In Normal Context, Call InterruptDisableIsr 1time
 * - Access to Register of Generate interrupt on target device
 * @subsection InterruptSequence_TC_12_001_02_ereb Expected Result/Behavior
 * Genrate interrupt on target device is success
 * @subsection InterruptSequence_TC_12_001_02_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptSequence_TC_12_001_024
 */
// [InterruptSequence_TC_12_001_024]
TEST_F(OSALINTTESTS, InterruptSequence_TC_12_001_024)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    osal_milli_sec_t time_period = 2000;
    osal_device_handle_t device_handle1 = OSAL_DEVICE_HANDLE_INVALID;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};

/* pre-process:
 * Setup required memory for interrupt generation.
 * Register callback function and enable interrupt.
 * Wait for interrupt callback finish */
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_int_seq_test_cb_TC_12_001, (void *)&arg), OSAL_RETURN_OK);

    /* Disable -> Disable -> Enable */
    /* Expect: generate interrput */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);

    /* Expect: generate interrput */
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt for registered interrupt of test target device
 * Deallocate allocated memory */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_int_seq_test_cb_TC_12_001), OSAL_RETURN_OK);

    deallocate_data_memory(hndl_mmngr, buffer_obj);
}
// [InterruptSequence_TC_12_001_024]

/**
 * @page INT_TEST Interrupt API Test
 * @section OSALINTTESTS_InterruptSequence_TC_12_001_025 OSALINTTESTS.InterruptSequence_TC_12_001_025
 * @subsection InterruptSequence_TC_12_001_025_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection InterruptSequence_TC_12_001_025_tci Test Case ID
 * InterruptIsISRContext_S1_A_TC_05_007_003
 * @subsection InterruptSequence_TC_12_001_025_rr Related Requirement(s)
 * \#199194
 * @subsection InterruptSequence_TC_12_001_025_pp Prerequisite/Precondition
 * - R_OSAL_Initialize is already called
 * @subsection InterruptSequence_TC_12_001_025_tcs Test Case Summary
 * This test confirmes behavior if call DisableIsr API in Normal context after call DisableIsr API in interrupt context
 * @subsection InterruptSequence_TC_12_001_025_tpts Test Procedure/Test Steps
 * - In Normal Context, Call InterruptEnableIsr 1 time
 * - Access to Register of Generate interrupt on target device (1st)
 * - In Interrupt Context, Call InterruptDisableIsr 1time
 * - After finish interrupt thread, Call InterruptDisableIsr 1 time
 * - In Normal Context, Call InterruptEnableIsr 1 time
 * - Access to Register of Generate interrupt on target device (2nd)
 * @subsection InterruptSequence_TC_12_001_025_ereb Expected Result/Behavior
 * 2nd Genrate interrupt on target device is success
 * @subsection InterruptSequence_TC_12_001_025_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptSequence_TC_12_001_025
 */
// [InterruptSequence_TC_12_001_025]
TEST_F(OSALINTTESTS, InterruptSequence_TC_12_001_025)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    osal_milli_sec_t time_period = 2000;
    osal_device_handle_t device_handle1 = OSAL_DEVICE_HANDLE_INVALID;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};

/* pre-process:
 * Setup required memory for interrupt generation.
 * Register callback function and enable interrupt.
 * Wait for interrupt callback finish */
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_int_seq_test_cb_TC_12_001, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

    /* Mismatch the number of times R_OSAL_InterruptDisableIsr and R_OSAL_InterruptEnable */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_int_seq_test_cb_TC_12_001), OSAL_RETURN_OK);

    /* Generate an interrupt again */
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_int_seq_test_cb_TC_12_001, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt for registered interrupt of test target device
 * Deallocate allocated memory */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_int_seq_test_cb_TC_12_001), OSAL_RETURN_OK);

    deallocate_data_memory(hndl_mmngr, buffer_obj);
}
// [InterruptSequence_TC_12_001_025]

/**
 * @page INT_TEST Interrupt API Test
 * @section OSALINTTESTS_InterruptSequence_TC_12_001_026 OSALINTTESTS.InterruptSequence_TC_12_001_026
 * @subsection InterruptSequence_TC_12_001_026_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection InterruptSequence_TC_12_001_026_tci Test Case ID
 * InterruptIsISRContext_S1_A_TC_05_007_003
 * @subsection InterruptSequence_TC_12_001_026_rr Related Requirement(s)
 * \#199191, \#199194
 * @subsection InterruptSequence_TC_12_001_026_pp Prerequisite/Precondition
 * - R_OSAL_Initialize is already called
 * @subsection InterruptSequence_TC_12_001_026_tcs Test Case Summary
 * This test confirmes behavior if call UnregisterIsr API in Normal context after RegisterIsr API and EnableIsr API
 * @subsection InterruptSequence_TC_12_001_026_tpts Test Procedure/Test Steps
 * - In Normal Context, Call InterruptEnableIsr
 * - In Normal Context, Call InterruptDisableIsr and InteruptUnregisterIsr
 * - Access to Register of Generate interrupt on target device
 * @subsection InterruptSequence_TC_12_001_026_ereb Expected Result/Behavior
 * Don't Genrate interrupt on target device
 * @subsection InterruptSequence_TC_12_001_026_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptSequence_TC_12_001_026
 */
// [InterruptSequence_TC_12_001_026]
TEST_F(OSALINTTESTS, InterruptSequence_TC_12_001_026)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    osal_milli_sec_t time_period = 2000;
    osal_device_handle_t device_handle1 = OSAL_DEVICE_HANDLE_INVALID;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};

/* pre-process:
 * Setup required memory for interrupt generation.
 * Register callback function and enable interrupt.
 * Wait for interrupt callback finish */
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_int_seq_test_cb_TC_12_001, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);

    /* unregiter interrupt function before generate interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_int_seq_test_cb_TC_12_001), OSAL_RETURN_OK);

    /* Expect: Don't generate interrupt */
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_TIME);

    deallocate_data_memory(hndl_mmngr, buffer_obj);
}
// [InterruptSequence_TC_12_001_026]

/**
 * @page INT_TEST Interrupt API Test
 * @section OSALINTTESTS_InterruptSequence_TC_12_001_027 OSALINTTESTS.InterruptSequence_TC_12_001_027
 * @subsection InterruptSequence_TC_12_001_027_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection InterruptSequence_TC_12_001_027_tci Test Case ID
 * InterruptIsISRContext_S1_A_TC_05_007_003
 * @subsection InterruptSequence_TC_12_001_027_rr Related Requirement(s)
 * \#199191, \#199194
 * @subsection InterruptSequence_TC_12_001_027_pp Prerequisite/Precondition
 * - R_OSAL_Initialize is already called
 * @subsection InterruptSequence_TC_12_001_027_tcs Test Case Summary
 * This test confirmes behavior if call UnregisterIsr API in Interrupt context
 * @subsection InterruptSequence_TC_12_001_027_tpts Test Procedure/Test Steps
 * - In Normal Context, Call InterruptRegisterIsr
 * - In Normal Context, Call InterruptEnableIsr
 * - Access to Register of Generate interrupt on target device (1st)
 * - In Interrupt Context, Call InterruptDisableIsr and InterruptUnregisterIsr
 * - Access to Register of Generate interrupt on target device (2nd)
 * @subsection InterruptSequence_TC_12_001_027_ereb Expected Result/Behavior
 * Don't 2nd Genrate interrupt on target device
 * @subsection InterruptSequence_TC_12_001_027_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptSequence_TC_12_001_027
 */
// [InterruptSequence_TC_12_001_027]
TEST_F(OSALINTTESTS, InterruptSequence_TC_12_001_027)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    osal_milli_sec_t time_period = 2000;
    osal_device_handle_t device_handle1 = OSAL_DEVICE_HANDLE_INVALID;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};

/* pre-process:
 * Setup required memory for interrupt generation.
 * Register callback function and enable interrupt.
 * Wait for interrupt callback finish */
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_test_int_cb_TC_12_001_027, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);

    /* unregiter interrupt in interrupt function */
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

    /* Expect: Don't generate interrupt */
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_TIME);

    deallocate_data_memory(hndl_mmngr, buffer_obj);
}
// [InterruptSequence_TC_12_001_027]

/**
 * @page INT_TEST Interrupt API Test
 * @section OSALINTTESTS_InterruptSequence_TC_12_001_028 OSALINTTESTS.InterruptSequence_TC_12_001_028
 * @subsection InterruptSequence_TC_12_001_028_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection InterruptSequence_TC_12_001_028_tci Test Case ID
 * InterruptIsISRContext_S1_A_TC_05_007_003
 * @subsection InterruptSequence_TC_12_001_028_rr Related Requirement(s)
 * \#199188, \#199191, \#199193, \#199194
 * @subsection InterruptSequence_TC_12_001_028_pp Prerequisite/Precondition
 * - R_OSAL_Initialize is already called
 * @subsection InterruptSequence_TC_12_001_028_tcs Test Case Summary
 * This test confirmes behavior that Interrupt is stopped by Disable API of Normal Context
 * @subsection InterruptSequence_TC_12_001_028_tpts Test Procedure/Test Steps
 * - In Normal Context, Call InterruptRegisterIsr
 * - In Normal Context, Call InterruptEnableIsr
 * - Access to Register of Generate interrupt on target device
 * - In Normal Context, Call InterruptDisableIsr
 * @subsection InterruptSequence_TC_12_001_028_ereb Expected Result/Behavior
 * All target APIs return OSAL_RETURN_OK and stop interupt.
 * @subsection InterruptSequence_TC_12_001_028_testcase Test Case Example
 * @snippet test_interrupt.cpp InterruptSequence_TC_12_001_028
 */
// [InterruptSequence_TC_12_001_028]
TEST_F(OSALINTTESTS, InterruptSequence_TC_12_001_028)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    osal_memory_buffer_handle_t buffer_obj[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t hw_addr[TEST_NUM_OF_BUFFERS];
    osal_milli_sec_t time_period = 2000;
    osal_device_handle_t device_handle1 = OSAL_DEVICE_HANDLE_INVALID;
    int32_t receive_msg = 0;
    int32_t send_msg = 0x1;
    osal_device_handle_t dev_arr[2] = {device_handle, device_handle2};

/* pre-process:
 * Setup required memory for interrupt generation.
 * Register callback function and enable interrupt.
 * Wait for interrupt callback finish */
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_test_int_cb_TC_12_001_028, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);

    /* Don't call disable api */
    /* Generate interrupt */
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt for registered interrupt of test target device
 * Deallocate allocated memory */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_test_int_cb_TC_12_001_028), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_MqReset(isr_mq), OSAL_RETURN_OK);

    deallocate_data_memory(hndl_mmngr, buffer_obj);
}
// [InterruptSequence_TC_12_001_028]
