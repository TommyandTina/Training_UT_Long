/***********************************************************************************************************************
* Copyright [2022] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    test_system_ram.cpp
* Version :      3.0.1
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL IO API Functional Test using System Ram
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           3.0.0     01.03.2022    Create New
 *                     11.03.2022    Add Power Manage API
 *           3.0.1     08.06.2022    Add Target SoC (V3H2, V3M2)
***********************************************************************************************************************/
#include "test_common.hpp"
#if !defined(OSAL_TEST_FRAMEWORK)
using namespace std;
#else
using namespace OsalFWHelper;
#endif

#if defined(UNSUPPORT_SYSTEM_RAM_TEST)
#define SystemRamTest DISABLED_SystemRamTest
#define SystemRam DISABLED_SystemRam
#endif

/**
 * @page SystemRam_Test Sytem Ram testing
 * @test
 * @test @link SystemRam_IoOpenClose_TC_014_001_001 @endlink
 * @test @link SystemRamTest_IoReadWrite_TC_014_001_002 @endlink
 * @test @link SystemRamTest_IoReadWrite_TC_014_001_003 @endlink
 * @test @link SystemRamTest_IoReadWrite_TC_014_001_004 @endlink
 * @test @link SystemRamTest_IoReadWrite_TC_014_001_005 @endlink
 * @section SystemRam System Ram testing function
 * @subsection SystemRam_SetUp System Ram tesing Setup
 * Common to Test Suit of Sytem Ram test, prepare process to be executed before executing the test. <br>
 * For example:
 * @snippet test_system_ram.cpp SystemRamTest_SetUp
 * @subsection SystemRam_TearDown System Ram tesing TearDown
 * Common to Test Suit of Sytem Ram test, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_system_ram.cpp SystemRamTest_TearDown
 */

class SystemRamTest : public OSALTESTS
{
protected:
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;
    uint32_t read_data = 0;

// [SystemRamTest_SetUp]
    void SetUp() override{
        EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_SYSTEM_RAM_ID, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
        EXPECT_NE(device_handle, NULL_PTR) << "pre-process falied";
        R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, false);
    }
// [SystemRamTest_SetUp]

// [SystemRamTest_TearDown]
    void TearDown() override{
        R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, false);
        EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    }
// [SystemRamTest_TearDown]
};

/**
 * @page SystemRam_Test Sytem Ram testing
 * @section SystemRam_IoOpenClose_TC_014_001_001 SystemRam.IoOpenClose_TC_014_001_001
 * @subsection IoOpenClose_TC_014_001_001_tsi Test Suite ID
 * SystemRam
 * @subsection IoOpenClose_TC_014_001_001_tci Test Case ID
 * IoOpenClose_S1_N_TC_014_001_001
 * @subsection IoOpenClose_TC_014_001_001_rr Related Requirement(s)
 * \#199198, \#199210 \#199211, \#199212
 * @subsection IoOpenClose_TC_014_001_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoOpenClose_TC_014_001_001_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoDeviceOpen/R_OSAL_IoDeviceClose function with System Ram
 * - Test014_001_001: Open/Close System Ram device
 * @subsection IoOpenClose_TC_014_001_001_tpts Test Procedure/Test Steps
 * -# Open TEST_SYSTEM_RAM_ID by using R_OSAL_IoDeviceOpen.
 * -# Close TEST_SYSTEM_RAM_ID by using R_OSAL_IoDeviceClose.
 * @subsection IoOpenClose_TC_014_001_001_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoOpenClose_TC_014_001_001_testcase Test Case Example
 * @snippet test_system_ram.cpp IoOpenClose_TC_014_001_001
 */
// [IoOpenClose_TC_014_001_001]
TEST(SystemRam, IoOpenClose_TC_014_001_001)
{
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;

    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_SYSTEM_RAM_ID, &device_handle), OSAL_RETURN_OK) << "Test014_001_001 falied";
    EXPECT_NE(device_handle, NULL_PTR) << "Test014_001_001 falied";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "Test014_001_001 falied";
}
// [IoOpenClose_TC_014_001_001]

/**
 * @page SystemRam_Test Sytem Ram testing
 * @section SystemRamTest_IoReadWrite_TC_014_001_002 SystemRamTest.IoReadWrite_TC_014_001_002
 * @subsection IoReadWrite_TC_014_001_002_tsi Test Suite ID
 * SystemRamTest
 * @subsection IoReadWrite_TC_014_001_002_tci Test Case ID
 * IoOpenClose_S1_N_TC_014_001_002
 * @subsection IoReadWrite_TC_014_001_002_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203, \#199211
 * @subsection IoReadWrite_TC_014_001_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - TEST_SYSTEM_RAM_ID has already been opened
 * @subsection IoReadWrite_TC_014_001_002_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoRead32/R_OSAL_IoWrite32 function with System Ram
 * - Test014_001_002: Read/write data from/to System Ram device with the valid offset
 * @subsection IoReadWrite_TC_014_001_002_tpts Test Procedure/Test Steps
 * -# R_OSAL_IoWrite32 the value TEST_SYSTEM_RAM_VALID_VALUE to TEST_SYSTEM_RAM_ID with offset is TEST_SYSTEM_RAM_OFFSET_NC by using R_OSAL_IoWrite32.
 * -# Read-back the value from TEST_SYSTEM_RAM_ID with offset is TEST_SYSTEM_RAM_OFFSET_NC by using R_OSAL_IoRead32.
 * -# Clean data of TEST_SYSTEM_RAM_ID at offset TEST_SYSTEM_RAM_OFFSET_NC by using R_OSAL_IoWrite32
 * @subsection IoReadWrite_TC_014_001_002_ereb Expected Result/Behavior
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead32 return OSAL_RETURN_OK.<br>
 * Value of "read_data" is equal to TEST_SYSTEM_RAM_VALID_VALUE.
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK.
 * @subsection IoReadWrite_TC_014_001_002_testcase Test Case Example
 * @snippet test_system_ram.cpp IoReadWrite_TC_014_001_002
 */
// [IoReadWrite_TC_014_001_002]
TEST_F(SystemRamTest, IoReadWrite_TC_014_001_002)
{
    EXPECT_NE(device_handle, NULL_PTR) << "Test014_001_002 falied";
    EXPECT_EQ(R_OSAL_IoWrite32(device_handle, TEST_SYSTEM_RAM_OFFSET_NC, TEST_SYSTEM_RAM_VALID_VALUE), OSAL_RETURN_OK) << "Test014_001_002 falied";
    EXPECT_EQ(R_OSAL_IoRead32(device_handle, TEST_SYSTEM_RAM_OFFSET_NC, &read_data), OSAL_RETURN_OK) << "Test014_001_002 failed";
    EXPECT_EQ(read_data, TEST_SYSTEM_RAM_VALID_VALUE) << "Test014_001_002 falied";
    // clean data
    EXPECT_EQ(R_OSAL_IoWrite32(device_handle, TEST_SYSTEM_RAM_OFFSET_NC, TEST_SYSTEM_RAM_CLEAN_VALUE), OSAL_RETURN_OK) << "Test014_001_002 failed";
}
// [IoReadWrite_TC_014_001_002]

/**
 * @page SystemRam_Test Sytem Ram testing
 * @section SystemRamTest_IoReadWrite_TC_014_001_003 SystemRamTest.IoReadWrite_TC_014_001_003
 * @subsection IoReadWrite_TC_014_001_003_tsi Test Suite ID
 * SystemRamTest
 * @subsection IoReadWrite_TC_014_001_003_tci Test Case ID
 * IoOpenClose_S1_BN_TC_014_001_003
 * @subsection IoReadWrite_TC_014_001_003_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203, \#199211
 * @subsection IoReadWrite_TC_014_001_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - TEST_SYSTEM_RAM_ID has already been opened
 * @subsection IoReadWrite_TC_014_001_003_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoRead32/R_OSAL_IoWrite32 function with System Ram
 * - Test014_001_003: Read/write data from/to System Ram device with the valid offset at boundary
 * @subsection IoReadWrite_TC_014_001_003_tpts Test Procedure/Test Steps
 * -# R_OSAL_IoWrite32 the value TEST_SYSTEM_RAM_VALID_VALUE to TEST_SYSTEM_RAM_ID with offset is TEST_SYSTEM_RAM_OFFSET_BN by using R_OSAL_IoWrite32.
 * -# Read-back the value from TEST_SYSTEM_RAM_ID with offset is TEST_SYSTEM_RAM_OFFSET_BN by using R_OSAL_IoRead32.
 * -# Clean data of TEST_SYSTEM_RAM_ID at offset TEST_SYSTEM_RAM_OFFSET_BN by using R_OSAL_IoWrite32
 * @subsection IoReadWrite_TC_014_001_003_ereb Expected Result/Behavior
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead32 return OSAL_RETURN_OK.<br>
 * Value of "read_data" is equal to TEST_SYSTEM_RAM_VALID_VALUE.
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK.
 * @subsection IoReadWrite_TC_014_001_003_testcase Test Case Example
 * @snippet test_system_ram.cpp IoReadWrite_TC_014_001_003
 */
// [IoReadWrite_TC_014_001_003]
TEST_F(SystemRamTest, IoReadWrite_TC_014_001_003)
{
    EXPECT_EQ(R_OSAL_IoWrite32(device_handle, TEST_SYSTEM_RAM_OFFSET_BN, TEST_SYSTEM_RAM_VALID_VALUE), OSAL_RETURN_OK) << "Test014_001_003 falied";
    EXPECT_EQ(R_OSAL_IoRead32(device_handle, TEST_SYSTEM_RAM_OFFSET_BN, &read_data), OSAL_RETURN_OK) << "Test014_001_003 failed";
    EXPECT_EQ(read_data, TEST_SYSTEM_RAM_VALID_VALUE) << "Test014_001_003 falied";
    // clean data
    EXPECT_EQ(R_OSAL_IoWrite32(device_handle, TEST_SYSTEM_RAM_OFFSET_BN, TEST_SYSTEM_RAM_CLEAN_VALUE), OSAL_RETURN_OK) << "Test014_001_003 failed";
}
// [IoReadWrite_TC_014_001_003]

/**
 * @page SystemRam_Test Sytem Ram testing
 * @section SystemRamTest_IoReadWrite_TC_014_001_004 SystemRamTest.IoReadWrite_TC_014_001_004
 * @subsection IoReadWrite_TC_014_001_004_tsi Test Suite ID
 * SystemRamTest
 * @subsection IoReadWrite_TC_014_001_004_tci Test Case ID
 * IoOpenClose_S0_BA_TC_014_001_004
 * @subsection IoReadWrite_TC_014_001_004_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203, \#199211
 * @subsection IoReadWrite_TC_014_001_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - TEST_SYSTEM_RAM_ID has already been opened
 * @subsection IoReadWrite_TC_014_001_004_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoRead32/R_OSAL_IoWrite32 function with System Ram
 * - Test014_001_004: Read/write data from/to System Ram device with the invalid offset at boundary
 * @subsection IoReadWrite_TC_014_001_004_tpts Test Procedure/Test Steps
 * -# Read the value from TEST_SYSTEM_RAM_ID with offset is TEST_SYSTEM_RAM_OFFSET_BA by using R_OSAL_IoRead32.
 * -# R_OSAL_IoWrite32 the value TEST_SYSTEM_RAM_VALID_VALUE to TEST_SYSTEM_RAM_ID with offset is TEST_SYSTEM_RAM_OFFSET_BA by using R_OSAL_IoWrite32.
 * @subsection IoReadWrite_TC_014_001_004_ereb Expected Result/Behavior
 * -# R_OSAL_IoRead32 return OSAL_RETURN_PAR.
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_PAR.
 * @subsection IoReadWrite_TC_014_001_004_testcase Test Case Example
 * @snippet test_system_ram.cpp IoReadWrite_TC_014_001_004
 */
// [IoReadWrite_TC_014_001_004]
TEST_F(SystemRamTest, IoReadWrite_TC_014_001_004)
{
    EXPECT_EQ(R_OSAL_IoRead32(device_handle, TEST_SYSTEM_RAM_OFFSET_BA, &read_data), OSAL_RETURN_PAR) << "Test014_001_004 failed";
    EXPECT_EQ(R_OSAL_IoWrite32(device_handle, TEST_SYSTEM_RAM_OFFSET_BA, TEST_SYSTEM_RAM_VALID_VALUE), OSAL_RETURN_PAR) << "Test014_001_004 falied";
}
// [IoReadWrite_TC_014_001_004]

/**
 * @page SystemRam_Test Sytem Ram testing
 * @section SystemRamTest_IoReadWrite_TC_014_001_005 SystemRamTest.IoReadWrite_TC_014_001_005
 * @subsection IoReadWrite_TC_014_001_005_tsi Test Suite ID
 * SystemRamTest
 * @subsection IoReadWrite_TC_014_001_005_tci Test Case ID
 * IoOpenClose_S0_BA_TC_014_001_004
 * @subsection IoReadWrite_TC_014_001_005_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203, \#199211
 * @subsection IoReadWrite_TC_014_001_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - TEST_SYSTEM_RAM_ID has already been opened
 * @subsection IoReadWrite_TC_014_001_005_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoRead32/R_OSAL_IoWrite32 function with System Ram
 * - Test014_001_005: Read/write data from/to System Ram device with the invalid offset
 * @subsection IoReadWrite_TC_014_001_005_tpts Test Procedure/Test Steps
 * -# Read the value from TEST_SYSTEM_RAM_ID with offset is TEST_SYSTEM_RAM_OFFSET_AC by using R_OSAL_IoRead32.
 * -# R_OSAL_IoWrite32 the value TEST_SYSTEM_RAM_VALID_VALUE to TEST_SYSTEM_RAM_ID with offset is TEST_SYSTEM_RAM_OFFSET_AC by using R_OSAL_IoWrite32.
 * @subsection IoReadWrite_TC_014_001_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoRead32 return OSAL_RETURN_PAR.
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_PAR.
 * @subsection IoReadWrite_TC_014_001_005_testcase Test Case Example
 * @snippet test_system_ram.cpp IoReadWrite_TC_014_001_005
 */
// [IoReadWrite_TC_014_001_005]
TEST_F(SystemRamTest, IoReadWrite_TC_014_001_005)
{
    EXPECT_EQ(R_OSAL_IoRead32(device_handle, TEST_SYSTEM_RAM_OFFSET_AC, &read_data), OSAL_RETURN_PAR) << "Test014_001_005 failed";
    EXPECT_EQ(R_OSAL_IoWrite32(device_handle, TEST_SYSTEM_RAM_OFFSET_AC, TEST_SYSTEM_RAM_VALID_VALUE), OSAL_RETURN_PAR) << "Test014_001_005 falied";
}
// [IoReadWrite_TC_014_001_005]

