/***********************************************************************************************************************
* Copyright [2021 - 2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    test_io_extension_api.cpp
* Version :      3.3.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL IO(AXI Bus) Manager API Functional Test (From the perspective of OSAL API Specification)
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           2.0.0     29.10.2021    Create New
 *           2.0.1     15.11.2021    Update Test Name
 *                                   getDeviceAxiBusId -> getDeviceAxiBusId_TC_12_001_014
 *                                   getAxiBusIdFromDeviceName -> getAxiBusIdFromDeviceName_TC_12_001_011
 *                                   getAxiBusName -> getAxiBusName_TC_12_001_012
 *           2.0.2     18.11.2021    Fix Typo
 *           3.0.0     15.02.2022    Apply new device and its setting definition
 *           3.0.1     24.06.2022    Update the below test cases of R_OSAL_IoGetAxiBusName, R_OSAL_IoGetAxiBusNameList:
 *                                      - IoGetAxiBusName_TC_06_022_001
 *                                      - IoGetAxiBusName_TC_06_022_002
 *                                      - IoGetAxiBusName_TC_06_022_003
 *                                      - IoGetAxiBusName_TC_06_022_004
 *                                      - IoGetAxiBusNameList_TC_06_023_001
 *           3.1.0     04.01.2023    Add test case IoGetDeviceAxiBusId_TC_06_024_005
 *           3.2.0     01.02.2023    Add new boundary test case IoGetAxiBusName_TC_06_022_006
 *           3.3.0     02.03.2023    Update Normal Case Test Cases for AXI Bus API
 *                                       - added test Sequential device
 *                                       - for example) ims_00, ims_01 
 *           3.4.0     11.09.2023   Add AXI Name definition of V4M
***********************************************************************************************************************/
#include "test_common.hpp"
#include <stddef.h>

#if !defined(OSAL_TEST_FRAMEWORK)
using namespace std;
#else
using namespace OsalFWHelper;
#endif

/* armclang will be warnig, when specified OsalApiStateTest to TEST_F */
/* W.A Derive a class and replace it by define */
class OsalApiStateTest_IOE : public OsalApiStateTest
{
    /* There is no need for member variables and member functions.
       We added them just for the sake of adding a test class. */
};
#define OsalApiStateTest OsalApiStateTest_IOE

#if !defined(SUPPORT_NEW_AXI_BUS_NAME_RULE)
#define IoExtensionApiTest DISABLED_IoExtensionApiTest
#endif

/**
 * @page AXI_BUS_TEST AXI Bus API Test
 * @test
 * @test @link OsalIoExtensionTest_getDeviceAxiBusId_TC_12_001_014 @endlink
 * @test @link OsalIoExtensionTest_getAxiBusIdFromDeviceName_TC_12_001_011 @endlink
 * @test @link OsalIoExtensionTest_getAxiBusName_TC_12_001_012 @endlink
 * @test @link IoExtensionApiTest_IoGetAxiBusIdFromDeviceName_TC_06_021_001 @endlink
 * @test @link IoExtensionApiTest_IoGetAxiBusIdFromDeviceName_TC_06_021_002 @endlink
 * @test @link IoExtensionApiTest_IoGetAxiBusIdFromDeviceName_TC_06_021_003 @endlink
 * @test @link IoExtensionApiTest_IoGetAxiBusIdFromDeviceName_TC_06_021_004 @endlink
 * @test @link OsalApiStateTest_IoGetAxiBusIdFromDeviceName_TC_06_021_005 @endlink
 * @test @link IoExtensionApiTest_IoGetAxiBusName_TC_06_022_001 @endlink
 * @test @link IoExtensionApiTest_IoGetAxiBusName_TC_06_022_002 @endlink
 * @test @link IoExtensionApiTest_IoGetAxiBusName_TC_06_022_003 @endlink
 * @test @link IoExtensionApiTest_IoGetAxiBusName_TC_06_022_004 @endlink
 * @test @link OsalApiStateTest_IoGetAxiBusName_TC_06_022_005 @endlink
 * @test @link IoExtensionApiTest_IoGetAxiBusName_TC_06_022_006 @endlink
 * @test @link IoExtensionApiTest_IoGetAxiBusNameList_TC_06_023_001 @endlink
 * @test @link IoExtensionApiTest_IoGetAxiBusNameList_TC_06_023_002 @endlink
 * @test @link IoExtensionApiTest_IoGetAxiBusNameList_TC_06_023_003 @endlink
 * @test @link IoExtensionApiTest_IoGetAxiBusNameList_TC_06_023_004 @endlink
 * @test @link IoExtensionApiTest_IoGetAxiBusNameList_TC_06_023_005 @endlink
 * @test @link OsalApiStateTest_IoGetAxiBusNameList_TC_06_023_006 @endlink
 * @test @link IoExtensionApiTest_IoGetDeviceAxiBusId_TC_06_024_001 @endlink
 * @test @link IoExtensionApiTest_IoGetDeviceAxiBusId_TC_06_024_002 @endlink
 * @test @link IoExtensionApiTest_IoGetDeviceAxiBusId_TC_06_024_003 @endlink
 * @test @link OsalApiStateTest_IoGetDeviceAxiBusId_TC_06_024_004 @endlink
 * @test @link IoExtensionApiTest_IoGetDeviceAxiBusId_TC_06_024_005 @endlink
 * @section OsalIoExtensionTest OsalIoExtensionTest function
 * @subsection OsalIoExtensionTest_SetUp OsalIoExtensionTest Setup
 * Common to Test Suit of OsalIoExtensionTest, prepare process to be executed before executing the test. <br>
 * For example:
 * @snippet test_io_extension_api.cpp OsalIoExtensionTest_SetUp
 * @subsection OsalIoExtensionTest_TearDown OsalIoExtensionTest TearDown
 * Common to Test Suit of OsalIoExtensionTest, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_io_extension_api.cpp OsalIoExtensionTest_TearDown
 * @section IoExtensionApiTest IoExtensionApiTest function
 * @subsection IoExtensionApiTest_SetUp IoExtensionApiTest Setup
 * Common to Test Suit of IoExtensionApiTest, prepare process to be executed before executing the test. <br>
 * For example:
 * @snippet test_io_extension_api.cpp IoExtensionApiTest_SetUp
 * @subsection IoExtensionApiTest_TearDown IoExtensionApiTest TearDown
 * Common to Test Suit of IoExtensionApiTest, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_io_extension_api.cpp IoExtensionApiTest_TearDown
 * @section OsalApiStateTest_IoExtensionApiTest OsalApiStateTest of IoExtensionApiTest function
 * @subsection OsalApiStateTest_IoExtensionApiTest_SetUp OsalApiStateTest of IoExtensionApiTest Setup
 * Common to Test Suit of OsalApiStateTest IoExtensionApiTest, prepare process to be executed before executing the test. <br>
 * For example:
 * @snippet test_common.hpp OsalApiStateTest_SetUp
 * @subsection OsalApiStateTest_IoExtensionApiTest_TearDown OsalApiStateTest of IoExtensionApiTest TearDown
 * Common to Test Suit of OsalApiStateTest IoExtensionApiTest, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_common.hpp OsalApiStateTest_TearDown
 */
class OsalIoExtensionTest : public PARENT_TEST_FW_CLASS
{
protected:
// [OsalIoExtensionTest_SetUp]
    void SetUp() override
    {
        /* R_OSAL_Initialize is already called before test start */
    }
// [OsalIoExtensionTest_SetUp]

// [OsalIoExtensionTest_TearDown]
    void TearDown() override
    {
        /* R_OSAL_Deinitialize is called after test end */
    }
// [OsalIoExtensionTest_TearDown]
};

/**
 * @page AXI_BUS_TEST AXI Bus API Test
 * @section OsalIoExtensionTest_getDeviceAxiBusId_TC_12_001_014 OsalIoExtensionTest.getDeviceAxiBusId_TC_12_001_014
 * @subsection getDeviceAxiBusId_tsi Test Suite ID.
 * OsalIoExtensionTest
 * @subsection getDeviceAxiBusId_tci Test Case ID
 * getDeviceAxiBusId_S3_N_TC_12_001_014
 * @subsection getDeviceAxiBusId_rr Related Requirement(s)
 * \#199199, \#199211, \#276272, \#276279
 * @subsection getDeviceAxiBusId_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection getDeviceAxiBusId_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetDeviceAxiBusId function
 * @subsection getDeviceAxiBusId_tpts Test Procedure/Test Steps
 * -# Get required buffer size for device list with TEST_IO_EXTENSION_DEVICE_TYPE_2 by using R_OSAL_IoGetDeviceList.
 * -# Allocate memory with required buffer size
 * -# Get the device list with device type is TEST_IO_EXTENSION_DEVICE_TYPE_2 by using R_OSAL_IoGetDeviceList.
 * -# Seperate the device ID in device list. <br>
 * Free allocated memory. <br>
 * Open the first device ID in device list by using R_OSAL_IoDeviceOpen.
 * -# Set "axi_id" to OSAL_AXI_BUS_ID_INVALID. <br>
 * Get the AXI Bus ID with given opened device handle by using R_OSAL_IoGetDeviceAxiBusId.
 * -# Set "axi_id2" to OSAL_AXI_BUS_ID_INVALID. <br>
 * Get the AXI Bus ID with given device ID by using R_OSAL_IoGetAxiBusIdFromDeviceName.
 * -# Set "axi_id" to OSAL_AXI_BUS_ID_INVALID. <br>
 * Get the AXI Bus ID by using R_OSAL_IoGetDeviceAxiBusId:
 *     - Passing the following parameters
 *         -# Device handle = OSAL_DEVICE_HANDLE_INVALID
 *         -# Address of "axi_id"
 * -# Get the AXI Bus ID by using R_OSAL_IoGetDeviceAxiBusId:
 *     - Passing the following parameters
 *         -# Device handle of first device ID (Already opened)
 *         -# Nullptr
 * -# Close first device ID in device list by using R_OSAL_IoDeviceClose.
 * @subsection getDeviceAxiBusId_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetDeviceList return OSAL_RETURN_OK_RQST. <br>
 * Returned "requiredBufferSize" is bigger than 0
 * -# Memory allocation succeeded (Pointer to buffer != Nullpointer)
 * -# R_OSAL_IoGetDeviceList return OSAL_RETURN_OK. <br>
 * Value of "writtenBytes" is equal to "requiredBufferSize"
 * -# Allocated memory free succeeded. <br>
 * R_OSAL_IoDeviceOpen return OSAL_RETURN_OK. <br>
 * Device handle of opened device is not invalid (OSAL_DEVICE_HANDLE_INVALID).
 * -# R_OSAL_IoGetDeviceAxiBusId return OSAL_RETURN_OK. <br>
 * Returned AXI Bus ID: "axi_id" is not invalid (OSAL_AXI_BUS_ID_INVALID).
 * -# R_OSAL_IoGetAxiBusIdFromDeviceName return OSAL_RETURN_OK. <br>
 * Returned AXI Bus ID: axi_id2 is equal to  "axi_id".
 * -# R_OSAL_IoGetDeviceAxiBusId return OSAL_RETURN_HANDLE. <br>
 * "axi_id" is equal to OSAL_AXI_BUS_ID_INVALID
 * -# R_OSAL_IoGetDeviceAxiBusId return OSAL_RETURN_PAR. <br>
 * "axi_id" is equal to OSAL_AXI_BUS_ID_INVALID
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection getDeviceAxiBusId_testcase Test Case Example
 * @snippet test_io_extension_api.cpp getDeviceAxiBusId_TC_12_001_014
 */
// [getDeviceAxiBusId_TC_12_001_014]
TEST_F(OsalIoExtensionTest, getDeviceAxiBusId_TC_12_001_014)
{
    /* check if an AXI ID can be obtained from an device */

    /* first get/open an device*/
    size_t requiredBufferSize = 0;

    ASSERT_EQ(OSAL_RETURN_OK_RQST, R_OSAL_IoGetDeviceList(TEST_IO_EXTENSION_DEVICE_TYPE_2, NULL_PTR, 0, &requiredBufferSize));
    ASSERT_GT(requiredBufferSize, 0);

    char * buffer = static_cast<char *>(calloc(requiredBufferSize, sizeof(char)));
    ASSERT_NE(buffer, NULL_PTR) << " Failed to allocate required buffer";

    size_t writtenBytes = 0;
    EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_IoGetDeviceList(TEST_IO_EXTENSION_DEVICE_TYPE_2, buffer, requiredBufferSize, &writtenBytes));
    EXPECT_EQ(writtenBytes, requiredBufferSize);

    vector<string> deviceList;
    char* token = strtok(buffer, "\n");
    while (token != NULL)
    {
        deviceList.push_back(string(token));
        token = strtok(NULL, "\n");
    }
    free(buffer); // do not need the buffer anymore
    buffer = NULL_PTR;

    /* open the first device in the list to proceed testing the AXI Bus ID functions*/
    osal_device_handle_t dev = OSAL_DEVICE_HANDLE_INVALID;
    ASSERT_EQ(OSAL_RETURN_OK, R_OSAL_IoDeviceOpen(deviceList.begin()->c_str(), &dev));
    ASSERT_NE(OSAL_DEVICE_HANDLE_INVALID, dev);

    /* now start the test for IO extension*/
    osal_axi_bus_id_t axi_id = OSAL_AXI_BUS_ID_INVALID;

    EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_IoGetDeviceAxiBusId(dev, &axi_id));
    EXPECT_NE(OSAL_AXI_BUS_ID_INVALID, axi_id);

    /*For IMP we know it is supposed to be the "ir" domain but we do not know which number is assigned and via the name
     * check is implemented in a different test. */
    osal_axi_bus_id_t axi_id2 = OSAL_AXI_BUS_ID_INVALID;
    EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_IoGetAxiBusIdFromDeviceName(deviceList.begin()->c_str(), &axi_id2));
    EXPECT_EQ(axi_id, axi_id2);

    /* negative checks */
    axi_id = OSAL_AXI_BUS_ID_INVALID;
    EXPECT_EQ(OSAL_RETURN_HANDLE, R_OSAL_IoGetDeviceAxiBusId(OSAL_DEVICE_HANDLE_INVALID, &axi_id));
    EXPECT_EQ(axi_id, OSAL_AXI_BUS_ID_INVALID); // do not expect to be modified by the function
    EXPECT_EQ(OSAL_RETURN_PAR, R_OSAL_IoGetDeviceAxiBusId(dev, NULL_PTR));
    EXPECT_EQ(axi_id, OSAL_AXI_BUS_ID_INVALID); // do not expect to be modified by the function

    ASSERT_EQ(OSAL_RETURN_OK, R_OSAL_IoDeviceClose(dev));
}
// [getDeviceAxiBusId_TC_12_001_014]

/**
 * @page AXI_BUS_TEST AXI Bus API Test
 * @section OsalIoExtensionTest_getAxiBusIdFromDeviceName_TC_12_001_011 OsalIoExtensionTest.getAxiBusIdFromDeviceName_TC_12_001_011
 * @subsection getAxiBusIdFromDeviceName_tsi Test Suite ID.
 * OsalIoExtensionTest
 * @subsection getAxiBusIdFromDeviceName_tci Test Case ID
 * getAxiBusIdFromDeviceName_S3_N_TC_12_001_011
 * @subsection getAxiBusIdFromDeviceName_rr Related Requirement(s)
 * \#276274, \#276279
 * @subsection getAxiBusIdFromDeviceName_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection getAxiBusIdFromDeviceName_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiBusIdFromDeviceName function
 * @subsection getAxiBusIdFromDeviceName_tpts Test Procedure/Test Steps
 * -# Get required buffer size for device list with TEST_IO_EXTENSION_DEVICE_TYPE_2 by using R_OSAL_IoGetDeviceList.
 * -# Allocate memory with required buffer size
 * -# Get the device list with device type is TEST_IO_EXTENSION_DEVICE_TYPE_2 by using R_OSAL_IoGetDeviceList.
 * -# Seperate the device ID in device list. <br>
 * Free allocated memory. <br>
 * -# Set "axi_id" to OSAL_AXI_BUS_ID_INVALID. <br>
 * Get AXI Bus ID for the first device ID in device list by using R_OSAL_IoGetAxiBusIdFromDeviceName.
 * -# Set "axi_id" to OSAL_AXI_BUS_ID_INVALID. <br>
 * Get the AXI Bus ID by using R_OSAL_IoGetAxiBusIdFromDeviceName:
 *     - Passing the following parameters
 *         -# NULL_PTR
 *         -# Address of "axi_id"
 * -# Get the AXI Bus ID by using R_OSAL_IoGetAxiBusIdFromDeviceName:
 *     - Passing the following parameters
 *         -# The first device ID in device list
 *         -# NULL_PTR
 * -# Get the AXI Bus ID by using R_OSAL_IoGetAxiBusIdFromDeviceName:
 *     - Passing the following parameters
 *         -# NULL_PTR
 *         -# NULL_PTR
 * -# Get the AXI Bus ID by using R_OSAL_IoGetAxiBusIdFromDeviceName:
 *     - Passing the following parameters
 *         -# unsupported device ID: "renesas"
 *         -# NULL_PTR
 * @subsection getAxiBusIdFromDeviceName_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetDeviceList return OSAL_RETURN_OK_RQST. <br>
 * Returned "requiredBufferSize" is bigger than 0
 * -# Memory allocation succeeded (Pointer to buffer != Nullpointer)
 * -# R_OSAL_IoGetDeviceList return OSAL_RETURN_OK. <br>
 * Value of "writtenBytes" is equal to "requiredBufferSize"
 * -# Allocated memory free succeeded. <br>
 * -# R_OSAL_IoGetAxiBusIdFromDeviceName return OSAL_RETURN_OK. <br>
 * Returned "axi_id" is not invalid (OSAL_AXI_BUS_ID_INVALID).
 * -# R_OSAL_IoGetAxiBusIdFromDeviceName return OSAL_RETURN_PAR. <br>
 * Returned "axi_id" is equal to OSAL_AXI_BUS_ID_INVALID
 * -# R_OSAL_IoGetAxiBusIdFromDeviceName return OSAL_RETURN_PAR.
 * -# R_OSAL_IoGetAxiBusIdFromDeviceName return OSAL_RETURN_PAR.
 * -# R_OSAL_IoGetAxiBusIdFromDeviceName return OSAL_RETURN_UNKNOWN_NAME. <br>
 * Returned "axi_id" is equal to OSAL_AXI_BUS_ID_INVALID
 * @subsection getAxiBusIdFromDeviceName_testcase Test Case Example
 * @snippet test_io_extension_api.cpp getAxiBusIdFromDeviceName_TC_12_001_011
 */
// [getAxiBusIdFromDeviceName_TC_12_001_011]
TEST_F(OsalIoExtensionTest, getAxiBusIdFromDeviceName_TC_12_001_011) 
{
    /* check if an AXI ID can be obtained from an device */

    /* first get an valid device name*/
    size_t requiredBufferSize = 0;

    ASSERT_EQ(OSAL_RETURN_OK_RQST, R_OSAL_IoGetDeviceList(TEST_IO_EXTENSION_DEVICE_TYPE_2, NULL_PTR, 0, &requiredBufferSize));
    ASSERT_GT(requiredBufferSize, 0);

    char * buffer = static_cast<char *>(calloc(requiredBufferSize, sizeof(char)));
    ASSERT_NE(buffer, NULL_PTR) << " Failed to allocate required buffer";

    size_t writtenBytes = 0;
    EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_IoGetDeviceList(TEST_IO_EXTENSION_DEVICE_TYPE_2, buffer, requiredBufferSize, &writtenBytes));
    EXPECT_EQ(writtenBytes, requiredBufferSize);

    vector<string> deviceList;
    char* token = strtok(buffer, "\n");
    while (token != NULL)
    {
        deviceList.push_back(string(token));
        token = strtok(NULL, "\n");
    }
    free(buffer); // do not need the buffer anymore
    buffer = NULL_PTR;

    /* now start the test for IO extension*/
    osal_axi_bus_id_t axi_id = OSAL_AXI_BUS_ID_INVALID;
    EXPECT_EQ(OSAL_RETURN_OK, R_OSAL_IoGetAxiBusIdFromDeviceName(deviceList.begin()->c_str(), &axi_id));
    EXPECT_NE(axi_id, OSAL_AXI_BUS_ID_INVALID); // do not expect to be modified by the function

    /* negativ checks */
    axi_id = OSAL_AXI_BUS_ID_INVALID;
    EXPECT_EQ(OSAL_RETURN_PAR, R_OSAL_IoGetAxiBusIdFromDeviceName(NULL_PTR, &axi_id));
    EXPECT_EQ(axi_id, OSAL_AXI_BUS_ID_INVALID); // do not expect to be modified by the function

    EXPECT_EQ(OSAL_RETURN_PAR, R_OSAL_IoGetAxiBusIdFromDeviceName(deviceList.begin()->c_str(), NULL_PTR));
    EXPECT_EQ(OSAL_RETURN_PAR, R_OSAL_IoGetAxiBusIdFromDeviceName(NULL_PTR, NULL_PTR));

    EXPECT_EQ(OSAL_RETURN_UNKNOWN_NAME, R_OSAL_IoGetAxiBusIdFromDeviceName("renesas", &axi_id));
    EXPECT_EQ(axi_id, OSAL_AXI_BUS_ID_INVALID); // do not expect to be modified by the function
}
// [getAxiBusIdFromDeviceName_TC_12_001_011]

/**
 * @page AXI_BUS_TEST AXI Bus API Test
 * @section OsalIoExtensionTest_getAxiBusName_TC_12_001_012 OsalIoExtensionTest.getAxiBusName_TC_12_001_012
 * @subsection getAxiBusName_tsi Test Suite ID.
 * OsalIoExtensionTest
 * @subsection getAxiBusName_tci Test Case ID
 * getAxiBusName_S3_N_TC_12_001_012
 * @subsection getAxiBusName_rr Related Requirement(s)
 * \#276277
 * @subsection getAxiBusName_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection getAxiBusName_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiBusName function
 * @subsection getAxiBusName_tpts Test Procedure/Test Steps
 * -# Get required buffer size for AXI Bus name corresponding with OSAL_AXI_BUS_ID_INVALID by using R_OSAL_IoGetAxiBusName.
 * -# Allocate buffer with required buffer size.
 * -# Get AXI Bus name corresponding with OSAL_AXI_BUS_ID_INVALID by using R_OSAL_IoGetAxiBusName.
 * -# Compare the content of buffer with "invalid". <br>
 Free allocated buffer
 * -# Get required buffer size for AXI Bus name corresponding with OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY by using R_OSAL_IoGetAxiBusName.
 * -# Allocate buffer with required buffer size.
 * -# Get AXI Bus name corresponding with OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY by using R_OSAL_IoGetAxiBusName.
 * -# Compare the content of buffer with "mm(ipa)". <br>
 Free allocated buffer
 * -# Set “writtenBytes” to 0 <br>
 * Get buffer with AXIBus names from OSAL using R_OSAL_IoGetAxiBusName function
 *     - Passing the following parameters
 *         -# OSAL_AXI_BUS_ID_INVALID
 *         -# NULL_PTR
 *         -# Maximum value for an object type size_t
 *         -# Address of “writtenBytes”
 * -# Allocate buffer with required buffer size is 20 byte
 * -# Get buffer with AXIBus names from OSAL using R_OSAL_IoGetAxiBusName function
 *     - Passing the following parameters
 *         -# OSAL_AXI_BUS_ID_INVALID
 *         -# buffer
 *         -# buffer size is 0
 *         -# Address of “writtenBytes”
 * -# Get buffer with AXIBus names from OSAL using R_OSAL_IoGetAxiBusName function
 *     - Passing the following parameters
 *         -# OSAL_AXI_BUS_ID_INVALID
 *         -# buffer
 *         -# buffer size is 1
 *         -# Address of “writtenBytes”
 * -# Free allocated buffer
 * @subsection getAxiBusName_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetAxiBusName return OSAL_RETURN_OK_RQST. <br>
 * Returned requiredBufferSize is bigger than 0.
 * -# Memory allocation succeeded (Pointer to buffer != Nullpointer)
 * -# R_OSAL_IoGetAxiBusName return OSAL_RETURN_OK. <br>
 * "writtenBytes" is equal to "requiredBufferSize"
 * -# "buffer" is equal to "invalid". <br>
 * Allocated buffer free succeeded.
 * -# R_OSAL_IoGetAxiBusName return OSAL_RETURN_OK_RQST. <br>
 * Returned requiredBufferSize is bigger than 0.
 * -# Memory allocation succeeded (Pointer to buffer != Nullpointer)
 * -# R_OSAL_IoGetAxiBusName return OSAL_RETURN_OK. <br>
 * "writtenBytes" is equal to "requiredBufferSize"
 * -# "buffer" is equal to "mm(ipa)". <br>
 * Allocated buffer free succeeded.
 * -# R_OSAL_IoGetAxiBusName return OSAL_RETURN_PAR. <br>
 * "writtenBytes" is 0.
 * -# Allocated buffer free succeeded.
 * -# R_OSAL_IoGetAxiBusName return OSAL_RETURN_PAR. <br>
 * "writtenBytes" is 0.
 * -# R_OSAL_IoGetAxiBusName return OSAL_RETURN_PAR. <br>
 * "writtenBytes" is 0.
 * -# Allocated buffer free succeeded.
 * @subsection getAxiBusName_testcase Test Case Example
 * @snippet test_io_extension_api.cpp getAxiBusName_TC_12_001_012
 */
// [getAxiBusName_TC_12_001_012]
TEST_F(OsalIoExtensionTest, getAxiBusName_TC_12_001_012)
{
    size_t requiredBufferSize = 0;

    /* check for predefined AXI Bus name INVALID*/
    ASSERT_EQ(OSAL_RETURN_OK_RQST, R_OSAL_IoGetAxiBusName(OSAL_AXI_BUS_ID_INVALID, NULL_PTR, 0, &requiredBufferSize));
    ASSERT_GT(requiredBufferSize, 0);

    char * buffer = static_cast<char *>(calloc(requiredBufferSize, sizeof(char)));
    ASSERT_NE(buffer, NULL_PTR) << " Failed to allocate required buffer";

    size_t writtenBytes = 0;
    EXPECT_EQ(OSAL_RETURN_OK,
              R_OSAL_IoGetAxiBusName(OSAL_AXI_BUS_ID_INVALID, buffer, requiredBufferSize, &writtenBytes));
    EXPECT_EQ(writtenBytes, requiredBufferSize);

    EXPECT_STREQ(buffer, "invalid");
    free(buffer); // do not need the buffer anymore
    buffer = NULL_PTR;

    /* check for predefined AXI Bus name MAIN_MEMORY*/
    ASSERT_EQ(OSAL_RETURN_OK_RQST, R_OSAL_IoGetAxiBusName(OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY, NULL_PTR, 0, &requiredBufferSize));
    ASSERT_GT(requiredBufferSize, 0);

    buffer = static_cast<char *>(calloc(requiredBufferSize, sizeof(char)));
    ASSERT_NE(buffer, NULL_PTR) << " Failed to allocate required buffer";

    writtenBytes = 0;
    EXPECT_EQ(OSAL_RETURN_OK,
              R_OSAL_IoGetAxiBusName(OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY, buffer, requiredBufferSize, &writtenBytes));
    EXPECT_EQ(writtenBytes, requiredBufferSize);

    EXPECT_STREQ(buffer, "mm(ipa)");
    free(buffer); // do not need the buffer anymore
    buffer = NULL_PTR;

    /* negativ checks */
    requiredBufferSize = 0xaffe;
    writtenBytes       = 0;
    EXPECT_EQ(OSAL_RETURN_PAR, R_OSAL_IoGetAxiBusName(OSAL_AXI_BUS_ID_INVALID, NULL_PTR,
                                                       SIZE_MAX, &writtenBytes));
    EXPECT_EQ(writtenBytes, 0);
    buffer = static_cast<char *>(calloc(20, sizeof(char)));
    EXPECT_EQ(OSAL_RETURN_PAR, R_OSAL_IoGetAxiBusName(OSAL_AXI_BUS_ID_INVALID, buffer, 0, &writtenBytes));
    EXPECT_EQ(writtenBytes, 0);
    EXPECT_EQ(OSAL_RETURN_PAR, R_OSAL_IoGetAxiBusName(OSAL_AXI_BUS_ID_INVALID, buffer, 1, &writtenBytes));
    EXPECT_EQ(writtenBytes, 0);
    free(buffer); // do not need the buffer anymore
    buffer = NULL_PTR;
    EXPECT_EQ(writtenBytes, 0);
}
// [getAxiBusName_TC_12_001_012]

class IoExtensionApiTest : public PARENT_TEST_FW_CLASS
{
protected:
    /* RVC implementation base on Linux OSAL IT*/
    typedef struct st_osal_test_axi_data{
        char* device_id;
        char* axi_name;
        size_t num_axi_id;
    } st_osal_test_axi_data_t;

    st_osal_test_axi_data_t gs_osal_test_axi_data[2] =
    {
#if defined(OSAL_SOC_V3M2)
        {TEST_IO_EXTENSION_DEVICE_1, TEST_AXI_BUS_NAME_1, TEST_NUM_AXI_ID_1},
        {TEST_IO_EXTENSION_DEVICE_2, TEST_AXI_BUS_NAME_2, TEST_NUM_AXI_ID_2}
#elif defined(OSAL_SOC_V3H1) || defined(OSAL_SOC_V3H2)
        {TEST_IO_EXTENSION_DEVICE_1, TEST_AXI_BUS_NAME_1, TEST_NUM_AXI_ID_1},
        {TEST_IO_EXTENSION_DEVICE_2, TEST_AXI_BUS_NAME_2, TEST_NUM_AXI_ID_2}
#elif defined(OSAL_SOC_V4H)
        {TEST_IO_EXTENSION_DEVICE_1, TEST_AXI_BUS_NAME_1, TEST_NUM_AXI_ID_1},
        {TEST_IO_EXTENSION_DEVICE_2, TEST_AXI_BUS_NAME_2, TEST_NUM_AXI_ID_2}
#elif defined(OSAL_SOC_V4H2)
        {TEST_IO_EXTENSION_DEVICE_1, TEST_AXI_BUS_NAME_1, TEST_NUM_AXI_ID_1},
        {TEST_IO_EXTENSION_DEVICE_2, TEST_AXI_BUS_NAME_2, TEST_NUM_AXI_ID_2}
#elif defined(OSAL_SOC_V4M)
        {TEST_IO_EXTENSION_DEVICE_1, TEST_AXI_BUS_NAME_1, TEST_NUM_AXI_ID_1},
        {TEST_IO_EXTENSION_DEVICE_2, TEST_AXI_BUS_NAME_2, TEST_NUM_AXI_ID_2}
#elif defined(OSAL_SOC_S4)
        {TEST_IO_EXTENSION_DEVICE_1, TEST_AXI_BUS_NAME_1, TEST_NUM_AXI_ID_1},
        {TEST_IO_EXTENSION_DEVICE_2, TEST_AXI_BUS_NAME_2, TEST_NUM_AXI_ID_2}
#elif defined(OSAL_SOC_V3U)
        {TEST_IO_EXTENSION_DEVICE_1, TEST_AXI_BUS_NAME_1, TEST_NUM_AXI_ID_1},
        {TEST_IO_EXTENSION_DEVICE_2, TEST_AXI_BUS_NAME_2, TEST_NUM_AXI_ID_2}
#endif
    };

    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;
    static vector<string> device_list;

// [IoExtensionApiTest_SetUp]
    static void SetUpTestSuite()
    {
        size_t size;
        size_t byte_written;
        char *buffer;

        char* p_buffer = NULL;
        size_t buffer_size = 0;
        EXPECT_EQ(R_OSAL_IoGetDeviceList(TEST_IO_EXTENSION_DEVICE_TYPE_1, p_buffer, buffer_size, &size), OSAL_RETURN_OK_RQST);
        buffer = (char*)malloc(size);
        EXPECT_NE(buffer, NULL_PTR);
        EXPECT_EQ(R_OSAL_IoGetDeviceList(TEST_IO_EXTENSION_DEVICE_TYPE_1, buffer, size, &byte_written), OSAL_RETURN_OK);
        EXPECT_EQ(size, byte_written);
        char* token = strtok(buffer, "\n");
        while (token != NULL)
        {
            device_list.push_back(string(token));
            token = strtok(NULL, "\n");
        }
        free(buffer);
    }
// [IoExtensionApiTest_SetUp]

// [IoExtensionApiTest_TearDown]
    static void TearDownTestSuite()
    {
        device_list.clear();
        device_list.shrink_to_fit();
        EXPECT_EQ(device_list.size(), 0);
        EXPECT_EQ(device_list.capacity(), 0);
    }
// [IoExtensionApiTest_TearDown]

// [GetAxiIdFromAxiIdListOfHandle]
    void GetAxiIdFromAxiIdListOfHandle(osal_axi_bus_id_t &axi_id, osal_device_handle_t device_handle)
    {
        osal_axi_bus_id_t *axi_id_list = NULL_PTR;
        size_t num_of_max_entry = 0;
        size_t num_of_written_axi_ids = 0;

        EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForDeviceHdl(device_handle, &num_of_max_entry), OSAL_RETURN_OK);
        EXPECT_EQ(num_of_max_entry, gs_osal_test_axi_data[0].num_axi_id);
        axi_id_list = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);
        EXPECT_NE(axi_id_list, NULL_PTR);
        EXPECT_EQ(R_OSAL_IoGetAxiIdListForDeviceHdl(device_handle, axi_id_list, num_of_max_entry, &num_of_written_axi_ids), OSAL_RETURN_OK);
        EXPECT_GT(num_of_written_axi_ids, 0) << "Device does not support AXI bus";
        if(1 < num_of_written_axi_ids)
        {
            srand((unsigned int)time(NULL_PTR)); // make rand() generate a different value at different execution times
            axi_id = axi_id_list[rand() % num_of_written_axi_ids]; // get random value in range [0 .. (num_of_written_axi_ids - 1)]
            EXPECT_TRUE((0 <= axi_id) && (num_of_written_axi_ids > axi_id));
        }
        else
        {
            axi_id = axi_id_list[0];
        }
        free(axi_id_list);
    }
// [GetAxiIdFromAxiIdListOfHandle]
};
vector<string> IoExtensionApiTest::device_list;

/* RVC implementation base on Linux OSAL IT*/
/**
 * @page AXI_BUS_TEST AXI Bus API Test
 * @section IoExtensionApiTest_IoGetAxiBusIdFromDeviceName_TC_06_021_001 IoExtensionApiTest.IoGetAxiBusIdFromDeviceName_TC_06_021_001
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_001_tsi Test Suite ID.
 * IoExtensionApiTest
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_001_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S1_N_TC_06_021_001
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_001_rr Related Requirement(s)
 * \#276274
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Device list of TEST_IO_EXTENSION_DEVICE_TYPE_1 is contained into "device_list"
 * - Axi bus name of corresponding test target devices are stored into gs_osal_test_axi_data
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_001_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiBusIdFromDeviceName function with normal condition:
 * - Test06_021_001: get axi bus id corresponding with device id (test target device).
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_001_tpts Test Procedure/Test Steps
 * -# Get the axi bus id of test target device by using R_OSAL_IoGetAxiBusIdFromDeviceName
 * -# Get the required size for axi bus name of "axi_id" got from step 1 by using R_OSAL_IoGetAxiBusName
 * -# Allocate buffer with the required size
 * -# Get the axi bus name of axi bus id got from step 1 by using R_OSAL_IoGetAxiBusName
 * -# Free allocated buffer
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_001_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetAxiBusIdFromDeviceName return OSAL_RETURN_OK.<br>
 * Value of "axi_id" is not equal to OSAL_AXI_BUS_ID_INVALID
 * -# R_OSAL_IoGetAxiBusName return OSAL_RETURN_OK_RQST.<br>
 * Value of "size" is not equal to 0
 * -# Memory allocation succeeded (Pointer to buffer != Nullpointer)
 * -# R_OSAL_IoGetAxiBusName return OSAL_RETURN_OK.<br>
 * Value of "byte_written" is equal to "size".<br>
 * "axi_name" is equal to "axi_name" of "gs_osal_test_axi_data"
 * -# Memory freeing succeeded.
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_001_testcase Test Case Example
 * @snippet test_io_extension_api.cpp IoGetAxiBusIdFromDeviceName_TC_06_021_001
 */
// [IoGetAxiBusIdFromDeviceName_TC_06_021_001]
TEST_F(IoExtensionApiTest, IoGetAxiBusIdFromDeviceName_TC_06_021_001)
{
    osal_axi_bus_id_t axi_id = OSAL_AXI_BUS_ID_INVALID;
    size_t size = 0;
    size_t byte_written = 0;
    size_t target_num = 0;
    char *axi_name;

    // Test06_021_001
    // @brief: normal case: get axi bus id corresponding with device id.
    for(target_num = 0; target_num < sizeof(gs_osal_test_axi_data)/sizeof(gs_osal_test_axi_data[0]); target_num++)
    {
        EXPECT_EQ(R_OSAL_IoGetAxiBusIdFromDeviceName(device_list.at(target_num).c_str(), &axi_id), OSAL_RETURN_OK) << "Test06_021_001 failed";
        EXPECT_NE(axi_id, OSAL_AXI_BUS_ID_INVALID) << "Test06_021_001 failed";
        EXPECT_NE(axi_id, OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY) << "Test06_021_001 failed";
        char* p_buffer = NULL;
        size_t b_size = 0;
        EXPECT_EQ(R_OSAL_IoGetAxiBusName(axi_id, p_buffer, b_size, &size), OSAL_RETURN_OK_RQST) << "Test06_021_001 failed";
        EXPECT_NE(size, 0);
        axi_name = (char*)malloc(size);
        EXPECT_NE(axi_name, NULL_PTR);
        EXPECT_EQ(R_OSAL_IoGetAxiBusName(axi_id, axi_name, size, &byte_written), OSAL_RETURN_OK) << "Test06_021_001 failed";
        EXPECT_EQ(size, byte_written);
        for (size_t it = 0; it < sizeof(gs_osal_test_axi_data)/sizeof(gs_osal_test_axi_data[0]); it++)
        {
            if(!strcmp(gs_osal_test_axi_data[it].device_id, device_list.at(target_num).c_str()))
            EXPECT_FALSE(strcmp(gs_osal_test_axi_data[it].axi_name, axi_name)) << "Test06_021_001 failed";
        }
        free(axi_name);
    }
}
// [IoGetAxiBusIdFromDeviceName_TC_06_021_001]

/**
 * @page AXI_BUS_TEST AXI Bus API Test
 * @section IoExtensionApiTest_IoGetAxiBusIdFromDeviceName_TC_06_021_002 IoExtensionApiTest.IoGetAxiBusIdFromDeviceName_TC_06_021_002
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_002_tsi Test Suite ID.
 * IoExtensionApiTest
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_002_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_021_002
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_002_rr Related Requirement(s)
 * \#276274
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_002_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiBusIdFromDeviceName function with abnormal condition:
 * - Test06_021_002: get axi bus id with device_name is NULL by using R_OSAL_IoGetAxiBusIdFromDeviceName
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_002_tpts Test Procedure/Test Steps
 * -# Get the axi bus ID by using R_OSAL_IoGetAxiBusIdFromDeviceName with the arguments as below:
 *      - Device ID is NULL
 *      - Address of "axi_id"
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_002_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetAxiBusIdFromDeviceName return OSAL_RETURN_PAR.<br>
 * Value of "axi_id" is equal to OSAL_AXI_BUS_ID_INVALID
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_002_testcase Test Case Example
 * @snippet test_io_extension_api.cpp IoGetAxiBusIdFromDeviceName_TC_06_021_002
 */
// [IoGetAxiBusIdFromDeviceName_TC_06_021_002]
TEST_F(IoExtensionApiTest, IoGetAxiBusIdFromDeviceName_TC_06_021_002)
{
    osal_axi_bus_id_t axi_id = OSAL_AXI_BUS_ID_INVALID;
    char *device_name = NULL;

    // Test06_021_002
    // @brief: abnormal case: device_name is NULL
    EXPECT_EQ(R_OSAL_IoGetAxiBusIdFromDeviceName(device_name, &axi_id), OSAL_RETURN_PAR) << "Test06_021_002 failed";
    EXPECT_EQ(axi_id, OSAL_AXI_BUS_ID_INVALID) << "Test06_021_002 failed";
}
// [IoGetAxiBusIdFromDeviceName_TC_06_021_002]

/**
 * @page AXI_BUS_TEST AXI Bus API Test
 * @section IoExtensionApiTest_IoGetAxiBusIdFromDeviceName_TC_06_021_003 IoExtensionApiTest.IoGetAxiBusIdFromDeviceName_TC_06_021_003
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_003_tsi Test Suite ID.
 * IoExtensionApiTest
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_003_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_021_003
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_003_rr Related Requirement(s)
 * \#276274, \#276279
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Device list of TEST_IO_EXTENSION_DEVICE_TYPE_1 is contained into "device_list"
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_003_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiBusIdFromDeviceName function with abnormal condition:
 * - Test06_021_003: axi_id is NULL
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_003_tpts Test Procedure/Test Steps
 * -# Get the axi bus ID by using R_OSAL_IoGetAxiBusIdFromDeviceName with the arguments as below:
 *      - Device ID is test target device
 *      - NULL
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_003_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetAxiBusIdFromDeviceName return OSAL_RETURN_PAR
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_003_testcase Test Case Example
 * @snippet test_io_extension_api.cpp IoGetAxiBusIdFromDeviceName_TC_06_021_003
 */
// [IoGetAxiBusIdFromDeviceName_TC_06_021_003]
TEST_F(IoExtensionApiTest, IoGetAxiBusIdFromDeviceName_TC_06_021_003)
{
    osal_axi_bus_id_t *p_axi_id = NULL;

    // Test06_021_003
    // @brief: abnormal case: axi_id is NULL
    EXPECT_EQ(R_OSAL_IoGetAxiBusIdFromDeviceName(device_list.at(0).c_str(), p_axi_id), OSAL_RETURN_PAR) << "Test06_021_003 failed";
}
// [IoGetAxiBusIdFromDeviceName_TC_06_021_003]

/**
 * @page AXI_BUS_TEST AXI Bus API Test
 * @section IoExtensionApiTest_IoGetAxiBusIdFromDeviceName_TC_06_021_004 IoExtensionApiTest.IoGetAxiBusIdFromDeviceName_TC_06_021_004
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_004_tsi Test Suite ID.
 * IoExtensionApiTest
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_004_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_021_004
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_004_rr Related Requirement(s)
 * \#276274
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_004_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiBusIdFromDeviceName function with abnormal condition:
 * - Test06_021_004: get axi bus id with input device id is not supported
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_004_tpts Test Procedure/Test Steps
 * -# Get the axi bus ID by using R_OSAL_IoGetAxiBusIdFromDeviceName with the arguments as below:
 *      - Device ID is UNKNOW_DEVICE_TYPE
 *      - Address of "axi_id"
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_004_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetAxiBusIdFromDeviceName return OSAL_RETURN_UNKNOWN_NAME.<br>
 * Value of "axi_id" is equal to OSAL_AXI_BUS_ID_INVALID
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_004_testcase Test Case Example
 * @snippet test_io_extension_api.cpp IoGetAxiBusIdFromDeviceName_TC_06_021_004
 */
// [IoGetAxiBusIdFromDeviceName_TC_06_021_004]
TEST_F(IoExtensionApiTest, IoGetAxiBusIdFromDeviceName_TC_06_021_004)
{
    osal_axi_bus_id_t axi_id = OSAL_AXI_BUS_ID_INVALID;

    // Test06_021_004
    // @brief: abnormal case: input device id is not supported
    EXPECT_EQ(R_OSAL_IoGetAxiBusIdFromDeviceName(UNKNOW_DEVICE_TYPE, &axi_id), OSAL_RETURN_UNKNOWN_NAME) << "Test06_021_004 failed";
    EXPECT_EQ(axi_id, OSAL_AXI_BUS_ID_INVALID) << "Test06_021_004 failed";
}
// [IoGetAxiBusIdFromDeviceName_TC_06_021_004]

/**
 * @page AXI_BUS_TEST AXI Bus API Test
 * @section OsalApiStateTest_IoGetAxiBusIdFromDeviceName_TC_06_021_005 OsalApiStateTest.IoGetAxiBusIdFromDeviceName_TC_06_021_005
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_005_tsi Test Suite ID.
 * OsalApiStateTest
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_005_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S2_A_TC_06_021_005
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_005_rr Related Requirement(s)
 * \#276274
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_005_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_005_tcs Test Case Summary
 * This test shall verify the state transition of IO Extension APIs with abnormal condition:
 * - Test06_021_005: state transition error with R_OSAL_IoGetAxiBusIdFromDeviceName.
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_005_tpts Test Procedure/Test Steps
 * -# Get the axi bus id of test target device by using R_OSAL_IoGetAxiBusIdFromDeviceName.
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetAxiBusIdFromDeviceName return OSAL_RETURN_STATE.<br>
 * Value of "axi_id" is equal to OSAL_AXI_BUS_ID_INVALID
 * @subsection IoGetAxiBusIdFromDeviceName_TC_06_021_005_testcase Test Case Example
 * @snippet test_io_extension_api.cpp IoGetAxiBusIdFromDeviceName_TC_06_021_005
 */
// [IoGetAxiBusIdFromDeviceName_TC_06_021_005]
TEST_F(OsalApiStateTest, IoGetAxiBusIdFromDeviceName_TC_06_021_005)
{
    osal_axi_bus_id_t axi_id = OSAL_AXI_BUS_ID_INVALID;

    // Test06_021_005
    // @brief: abnormal case: state transition error with R_OSAL_IoGetAxiBusIdFromDeviceName
    EXPECT_EQ(R_OSAL_IoGetAxiBusIdFromDeviceName(TEST_PM_DEVICE_1, &axi_id), OSAL_RETURN_STATE) << "Test06_021_005 failed";
    EXPECT_EQ(axi_id, OSAL_AXI_BUS_ID_INVALID) << "Test06_021_005 failed";
}
// [IoGetAxiBusIdFromDeviceName_TC_06_021_005]

/**
 * @page AXI_BUS_TEST AXI Bus API Test
 * @section IoExtensionApiTest_IoGetAxiBusName_TC_06_022_001 IoExtensionApiTest.IoGetAxiBusName_TC_06_022_001
 * @subsection IoGetAxiBusName_TC_06_022_001_tsi Test Suite ID.
 * IoExtensionApiTest
 * @subsection IoGetAxiBusName_TC_06_022_001_tci Test Case ID
 * IoGetAxiBusName_S1_N_TC_06_022_001
 * @subsection IoGetAxiBusName_TC_06_022_001_rr Related Requirement(s)
 * \#276277
 * @subsection IoGetAxiBusName_TC_06_022_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Device list of TEST_IO_EXTENSION_DEVICE_TYPE_1 is contained into "device_list"
 * @subsection IoGetAxiBusName_TC_06_022_001_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiBusName function with normal condition:
 * - pre-process: Do with some steps as below:<br>
 *      -# Open test target device
 *      -# Get one axi bus ID from its axi bus ID list by using GetAxiIdFromAxiIdListOfHandle.
 *      -# Get the axi bus name list by using R_OSAL_IoGetAxiBusNameList
 * - Test06_022_001: get axi bus name corresponding with axi bus id.
 * - post-process: close test target device and free un-used allocated memory
 * @subsection IoGetAxiBusName_TC_06_022_001_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Get the axi bus ID of test target device by using GetAxiIdFromAxiIdListOfHandle
 * -# Get the required size for axi bus name of the given axi bus ID by using R_OSAL_IoGetAxiBusName
 * -# Allocate buffer with the required size.
 * -# memset all elements in this buffer by '\0'
 * -# Get the axi bus name of the given axi bus id by using R_OSAL_IoGetAxiBusName
 * -# Find the '\0' in the output AXI bus name buffer
 * -# Free allocated buffer
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoGetAxiBusName_TC_06_022_001_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not equal to NULL_PTR
 * -# Value of "axi_id" is not equal to OSAL_AXI_BUS_ID_INVALID
 * -# R_OSAL_IoGetAxiBusName return OSAL_RETURN_OK_RQST.<br>
 * Value of "busname_size" is equal to size of "axi_name" of "gs_osal_test_axi_data".
 * -# Memory allocation succeeded (Pointer to buffer != Nullpointer)
 * -# Memset run successfully
 * -# R_OSAL_IoGetAxiBusName return OSAL_RETURN_OK.<br>
 * Value of "busname_byte_written" is equal to "busname_size".
 * -# There is no '\0' in the output AXI name buffer
 * -# Memory freeing succeeded
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoGetAxiBusName_TC_06_022_001_testcase Test Case Example
 * @snippet test_io_extension_api.cpp IoGetAxiBusName_TC_06_022_001
 * @snippet test_io_extension_api.cpp GetAxiIdFromAxiIdListOfHandle
 */
// [IoGetAxiBusName_TC_06_022_001]
TEST_F(IoExtensionApiTest, IoGetAxiBusName_TC_06_022_001)
{
    // Parameter use for get axi bus name by using R_OSAL_IoGetAxiBusName
    osal_axi_bus_id_t axi_id = OSAL_AXI_BUS_ID_INVALID;
    size_t busname_size = 0;
    size_t busname_byte_written = 0;
    char *axi_name = NULL;
    char *busname_p_buffer = NULL;

    // pre-process: open test target device, get one axi bus ID from its axi bus ID list by using R_OSAL_IoGetNumOfAxiIdsForDeviceHdl & R_OSAL_IoGetAxiIdListForDeviceHdl. Get axi bus name by using R_OSAL_IoGetAxiBusNameList.
    EXPECT_EQ(R_OSAL_IoDeviceOpen(device_list.at(0).c_str(), &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    GetAxiIdFromAxiIdListOfHandle(axi_id, device_handle);
    EXPECT_NE(axi_id, OSAL_AXI_BUS_ID_INVALID) << "pre-process failed";

    // Test06_022_001
    // @brief: normal case: get axi bus name corresponding with axi bus id.
    EXPECT_EQ(R_OSAL_IoGetAxiBusName(axi_id, busname_p_buffer, busname_size, &busname_size), OSAL_RETURN_OK_RQST) << "Test06_022_001 failed";
    EXPECT_EQ(busname_size, strlen(gs_osal_test_axi_data[0].axi_name) + 1) << "Test06_022_001 failed";
    axi_name = (char*)malloc(busname_size);
    EXPECT_NE(axi_name, NULL_PTR) << "Test06_022_001 failed";
    (void)memset(axi_name, '\n', busname_size);
    EXPECT_EQ(R_OSAL_IoGetAxiBusName(axi_id, axi_name, busname_size, &busname_byte_written), OSAL_RETURN_OK) << "Test06_022_001 failed";
    EXPECT_EQ(busname_size, busname_byte_written) << "Test06_022_001 failed";
    for(size_t i = 0; i < busname_byte_written; i++)
    {
        EXPECT_TRUE(axi_name[i] != '\n');
    }

    // post-process: free allocated buffer and close test target device
    free(axi_name);
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
}
// [IoGetAxiBusName_TC_06_022_001]

/**
 * @page AXI_BUS_TEST AXI Bus API Test
 * @section IoExtensionApiTest_IoGetAxiBusName_TC_06_022_002 IoExtensionApiTest.IoGetAxiBusName_TC_06_022_002
 * @subsection IoGetAxiBusName_TC_06_022_002_tsi Test Suite ID.
 * IoExtensionApiTest
 * @subsection IoGetAxiBusName_TC_06_022_002_tci Test Case ID
 * IoGetAxiBusNameAbnormal_S0_A_TC_06_022_002
 * @subsection IoGetAxiBusName_TC_06_022_002_rr Related Requirement(s)
 * \#276277
 * @subsection IoGetAxiBusName_TC_06_022_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Device list of TEST_IO_EXTENSION_DEVICE_TYPE_1 is contained into "device_list"
 * @subsection IoGetAxiBusName_TC_06_022_002_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiBusName function with abnormal condition:
 * - pre-process: Do with some steps as below:<br>
 *      -# Open test target device
 *      -# Get one AXI bus ID from its axi bus ID list by using GetAxiIdFromAxiIdListOfHandle.
 *      -# Get the required size of AXI bus name buffer by using R_OSAL_IoGetAxiBusName
 * - Test06_022_002: get axi bus name with p_num_of_byte is NULL.
 * - post-process: free allocated buffer and close test target device.
 * @subsection IoGetAxiBusName_TC_06_022_002_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Get one axi ID of device handle by using GetAxiIdFromAxiIdListOfHandle
 * -# Get the required size for axi bus name of axi bus ID by using R_OSAL_IoGetAxiBusName
 * -# Allocate buffer with the required size.
 * -# Get the axi bus name by using R_OSAL_IoGetAxiBusName with the arguments as below:
 *      - Given "axi_id"
 *      - "axi_name" used to store axi bus name
 *      - "size" is the required size
 *      - NULL
 * -# Free allocated buffer
 * -# Close test target device by using R_OSAL_IoDeviceClose.
 * @subsection IoGetAxiBusName_TC_06_022_002_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not equal to NULL_PTR
 * -# Value of "axi_id" is not equal to OSAL_AXI_BUS_ID_INVALID
 * -# R_OSAL_IoGetAxiBusName return OSAL_RETURN_OK_RQST.<br>
 * Value of "size" is equal to size of "axi_name" of "gs_osal_test_axi_data".
 * -# Memory allocation succeeded (Pointer to buffer != Nullpointer)
 * -# R_OSAL_IoGetAxiBusName return OSAL_RETURN_PAR.
 * -# Memory freeing succeeded
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoGetAxiBusName_TC_06_022_002_testcase Test Case Example
 * @snippet test_io_extension_api.cpp IoGetAxiBusName_TC_06_022_002
 * @snippet test_io_extension_api.cpp GetAxiIdFromAxiIdListOfHandle
 */
// [IoGetAxiBusName_TC_06_022_002]
TEST_F(IoExtensionApiTest, IoGetAxiBusName_TC_06_022_002)
{
    osal_axi_bus_id_t axi_id = OSAL_AXI_BUS_ID_INVALID;
    size_t size = 0;
    char *axi_name = NULL;
    size_t *p_num_of_byte = NULL;
    size_t buffer_size = 0;
    char *p_buffer = NULL;

    // pre-process: get axi ID from axi ID list and get the required size for axi bus name
    EXPECT_EQ(R_OSAL_IoDeviceOpen(device_list.at(0).c_str(), &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    GetAxiIdFromAxiIdListOfHandle(axi_id, device_handle);
    EXPECT_NE(axi_id, OSAL_AXI_BUS_ID_INVALID) << "pre-process failed";
    EXPECT_EQ(R_OSAL_IoGetAxiBusName(axi_id, p_buffer, buffer_size, &size), OSAL_RETURN_OK_RQST) << "pre-process failed";
    EXPECT_EQ(size, strlen(gs_osal_test_axi_data[0].axi_name) + 1) << "pre-process failed";
    axi_name = (char*)malloc(size);
    EXPECT_NE(axi_name, NULL_PTR) << "pre-process failed";

    // Test06_022_002
    // @brief: abnormal case: p_num_of_byte is NULL.
    EXPECT_EQ(R_OSAL_IoGetAxiBusName(axi_id, axi_name, size, p_num_of_byte), OSAL_RETURN_PAR) << "Test06_022_002 failed";

    // post-process: free allocated buffer and close device
    free(axi_name);
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
}
// [IoGetAxiBusName_TC_06_022_002]

/**
 * @page AXI_BUS_TEST AXI Bus API Test
 * @section IoExtensionApiTest_IoGetAxiBusName_TC_06_022_003 IoExtensionApiTest.IoGetAxiBusName_TC_06_022_003
 * @subsection IoGetAxiBusName_TC_06_022_003_tsi Test Suite ID.
 * IoExtensionApiTest
 * @subsection IoGetAxiBusName_TC_06_022_003_tci Test Case ID
 * IoGetAxiBusName_S0_A_TC_06_022_003
 * @subsection IoGetAxiBusName_TC_06_022_003_rr Related Requirement(s)
 * \#276277
 * @subsection IoGetAxiBusName_TC_06_022_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Device list of TEST_IO_EXTENSION_DEVICE_TYPE_1 is contained into "device_list"
 * @subsection IoGetAxiBusName_TC_06_022_003_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiBusName function with abnormal condition:
 * - pre-process: Do with some steps as below:<br>
 *      -# Open test target device
 *      -# Get one AXI bus ID from its axi bus ID list by using GetAxiIdFromAxiIdListOfHandle.
 *      -# Get the required size of AXI bus name buffer by using R_OSAL_IoGetAxiBusName
 * - Test06_022_003: get axi bus name with p_buffer is NULL.
 * - post-process: close test target device.
 * @subsection IoGetAxiBusName_TC_06_022_003_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Get one axi ID of device handle by using GetAxiIdFromAxiIdListOfHandle
 * -# Get the required size for axi bus name of the given axi bus ID by using R_OSAL_IoGetAxiBusName
 * -# Get the axi bus name by using R_OSAL_IoGetAxiBusName with the arguments as below:
 *      - the given "axi_id"
 *      - NULL
 *      - "size" is the required size
 *      - Address of "byte_written"
 * -# Close test target device by using R_OSAL_IoDeviceClose.
 * @subsection IoGetAxiBusName_TC_06_022_003_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not equal to NULL_PTR
 * -# Value of "axi_id" is not equal to OSAL_AXI_BUS_ID_INVALID
 * -# R_OSAL_IoGetAxiBusName return OSAL_RETURN_OK_RQST.<br>
 * Value of "size" is equal to size of "axi_name" of "gs_osal_test_axi_data".
 * -# R_OSAL_IoGetAxiBusName return OSAL_RETURN_PAR.<br>
 * Value of "byte_written" is equal to 0
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoGetAxiBusName_TC_06_022_003_testcase Test Case Example
 * @snippet test_io_extension_api.cpp IoGetAxiBusName_TC_06_022_003
 * @snippet test_io_extension_api.cpp GetAxiIdFromAxiIdListOfHandle
 */
// [IoGetAxiBusName_TC_06_022_003]
TEST_F(IoExtensionApiTest, IoGetAxiBusName_TC_06_022_003)
{
    osal_axi_bus_id_t axi_id = OSAL_AXI_BUS_ID_INVALID;
    size_t size = 0;
    size_t byte_written = 0;
    char *p_buffer = NULL;
    size_t buffer_size = 0;

    // pre-process: get axi ID from axi ID list and get the required size for axi bus name
    EXPECT_EQ(R_OSAL_IoDeviceOpen(device_list.at(0).c_str(), &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    GetAxiIdFromAxiIdListOfHandle(axi_id, device_handle);
    EXPECT_NE(axi_id, OSAL_AXI_BUS_ID_INVALID) << "pre-process failed";
    EXPECT_EQ(R_OSAL_IoGetAxiBusName(axi_id, p_buffer, buffer_size, &size), OSAL_RETURN_OK_RQST) << "pre-process failed";
    EXPECT_EQ(size, strlen(gs_osal_test_axi_data[0].axi_name) + 1) << "pre-process failed";

    // Test06_022_003
    // @brief: abnormal case: p_buffer is NULL.
    EXPECT_EQ(R_OSAL_IoGetAxiBusName(axi_id, p_buffer, size, &byte_written), OSAL_RETURN_PAR) << "Test06_022_003 failed";
    EXPECT_EQ(byte_written, 0) << "Test06_022_003 failed";

    // post-process: close device
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
}
// [IoGetAxiBusName_TC_06_022_003]

/**
 * @page AXI_BUS_TEST AXI Bus API Test
 * @section IoExtensionApiTest_IoGetAxiBusName_TC_06_022_004 IoExtensionApiTest.IoGetAxiBusName_TC_06_022_004
 * @subsection IoGetAxiBusName_TC_06_022_004_tsi Test Suite ID.
 * IoExtensionApiTest
 * @subsection IoGetAxiBusName_TC_06_022_004_tci Test Case ID
 * IoGetAxiBusName_S0_A_TC_06_022_004
 * @subsection IoGetAxiBusName_TC_06_022_004_rr Related Requirement(s)
 * \#276277
 * @subsection IoGetAxiBusName_TC_06_022_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Device list of TEST_IO_EXTENSION_DEVICE_TYPE_1 is contained into "device_list"
 * @subsection IoGetAxiBusName_TC_06_022_004_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiBusName function with abnormal condition:
 * - pre-process: Do with some steps as below:<br>
 *      -# Open test target device
 *      -# Get one AXI bus ID from its axi bus ID list by using GetAxiIdFromAxiIdListOfHandle.
 * - Test06_022_004: get axi bus name with p_buffer is NULL and buffer_size is equal to 0.
 * - post-process: Close test target device.
 * @subsection IoGetAxiBusName_TC_06_022_004_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Get one axi ID of device handle by using GetAxiIdFromAxiIdListOfHandle
 * -# Get the required size for axi bus name of axi bus ID got from step 2 by using R_OSAL_IoGetAxiBusName
 * -# Allocate required buffer
 * -# Memset the allocated buffer with '\0'
 * -# Get the AXI bus name by using R_OSAL_IoGetAxiBusName
 * -# Find '\0' in the output AXI bus name 
 * -# Free allocated buffer
 * -# Close test target device by using R_OSAL_IoDeviceClose.
 * @subsection IoGetAxiBusName_TC_06_022_004_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not equal to NULL_PTR
 * -# Value of "axi_id" is not equal to OSAL_AXI_BUS_ID_INVALID
 * -# R_OSAL_IoGetAxiBusName return OSAL_RETURN_OK_RQST.<br>
 * Value of "size" is equal to size of "axi_name" of "gs_osal_test_axi_data".
 * -# Allocation success (Pointer to buffer != Nullpointer)
 * -# Memset run successfully
 * -# R_OSAL_IoGetAxiBusName return OSAL_RETURN_OK
 * -# No '\0' will be found in the AXI bus name
 * -# Free buffer successfully
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoGetAxiBusName_TC_06_022_004_testcase Test Case Example
 * @snippet test_io_extension_api.cpp IoGetAxiBusName_TC_06_022_004
 * @snippet test_io_extension_api.cpp GetAxiIdFromAxiIdListOfHandle
 */
// [IoGetAxiBusName_TC_06_022_004]
TEST_F(IoExtensionApiTest, IoGetAxiBusName_TC_06_022_004)
{
    osal_axi_bus_id_t axi_id = OSAL_AXI_BUS_ID_INVALID;
    size_t size = 0;
    size_t buffer_size = 0;
    char *p_buffer = NULL;
    char* axi_name = NULL;
    size_t byte_written = 0;

    // pre-process: get axi ID from axi ID list
    EXPECT_EQ(R_OSAL_IoDeviceOpen(device_list.at(0).c_str(), &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    GetAxiIdFromAxiIdListOfHandle(axi_id, device_handle);
    EXPECT_NE(axi_id, OSAL_AXI_BUS_ID_INVALID) << "pre-process failed";

    // Test06_022_004
    // @brief: abnormal case: get axi bus name with p_buffer is NULL and buffer_size is equal to 0.
    EXPECT_EQ(R_OSAL_IoGetAxiBusName(axi_id, p_buffer, buffer_size, &size), OSAL_RETURN_OK_RQST) << "Test06_022_004 failed";
    EXPECT_EQ(size, strlen(gs_osal_test_axi_data[0].axi_name) + 1) << "Test06_022_004 failed";
    axi_name = (char*)malloc(size);
    EXPECT_NE(axi_name, NULL_PTR) << "Test06_022_004 failed";
    (void)memset(axi_name, '\0', size);
    EXPECT_EQ(R_OSAL_IoGetAxiBusName(axi_id, axi_name, size, &byte_written), OSAL_RETURN_OK) << "Test06_022_004 failed";
    EXPECT_EQ(size, byte_written) << "Test06_022_004 failed";
    for(size_t i = 0; i < (byte_written - 1); i++)
    {
        EXPECT_TRUE(axi_name[i] != '\0');
    }

    // post-process: close device
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    free(axi_name);
}
// [IoGetAxiBusName_TC_06_022_004]

/**
 * @page AXI_BUS_TEST AXI Bus API Test
 * @section OsalApiStateTest_IoGetAxiBusName_TC_06_022_005 OsalApiStateTest.IoGetAxiBusName_TC_06_022_005
 * @subsection IoGetAxiBusName_TC_06_022_005_tsi Test Suite ID.
 * OsalApiStateTest
 * @subsection IoGetAxiBusName_TC_06_022_005_tci Test Case ID
 * IoGetAxiBusName_S2_A_TC_06_022_005
 * @subsection IoGetAxiBusName_TC_06_022_005_rr Related Requirement(s)
 * \#276277
 * @subsection IoGetAxiBusName_TC_06_022_005_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection IoGetAxiBusName_TC_06_022_005_tcs Test Case Summary
 * This test shall verify the state transition of IO Extension APIs with abnormal condition:
 * - Test06_022_005: state transition error with R_OSAL_IoGetAxiBusName.
 * @subsection IoGetAxiBusName_TC_06_022_005_tpts Test Procedure/Test Steps
 * -# Get the axi bus name by using R_OSAL_IoGetAxiBusName.
 * @subsection IoGetAxiBusName_TC_06_022_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetAxiBusName return OSAL_RETURN_STATE.
 * @subsection IoGetAxiBusName_TC_06_022_005_testcase Test Case Example
 * @snippet test_io_extension_api.cpp IoGetAxiBusName_TC_06_022_005
 */
// [IoGetAxiBusName_TC_06_022_005]
TEST_F(OsalApiStateTest, IoGetAxiBusName_TC_06_022_005)
{
    osal_axi_bus_id_t axi_id = OSAL_AXI_BUS_ID_INVALID;
    size_t size = 0;
    size_t byte_written = 0;
    char *axi_name = NULL;

    // Test06_022_005
    // @brief: abnormal case: state transition error with R_OSAL_IoGetAxiBusName
    EXPECT_EQ(R_OSAL_IoGetAxiBusName(axi_id, axi_name, size, &byte_written), OSAL_RETURN_STATE) << "Test06_022_005 failed";
}
// [IoGetAxiBusName_TC_06_022_005]

/**
 * @page AXI_BUS_TEST AXI Bus API Test
 * @section IoExtensionApiTest_IoGetAxiBusName_TC_06_022_006 IoExtensionApiTest.IoGetAxiBusName_TC_06_022_006
 * @subsection IoGetAxiBusName_TC_06_022_006_tsi Test Suite ID.
 * OsalApiStateTest
 * @subsection IoGetAxiBusName_TC_06_022_006_tci Test Case ID
 * IoGetAxiBusName_S0_BA_TC_06_022_006
 * @subsection IoGetAxiBusName_TC_06_022_006_rr Related Requirement(s)
 * \#276277
 * @subsection IoGetAxiBusName_TC_06_022_006_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection IoGetAxiBusName_TC_06_022_006_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiBusName function:
 * - Test06_022_006: Using R_OSAL_IoGetAxiBusName with the buffer_size is less than the size of actual buffer
 * @subsection IoGetAxiBusName_TC_06_022_006_tpts Test Procedure/Test Steps
 * -# Get the axi bus name by using R_OSAL_IoGetAxiBusName with p_buffer is NULL and buffer_size is 0.
 * -# Allocate the buffer with the required size by using malloc
 * -# Calling R_OSAL_IoGetAxiBusName with the buffer_size is less than the size of actual buffer
 * -# Free the allocated buffer
 * @subsection IoGetAxiBusName_TC_06_022_006_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetAxiBusName return OSAL_RETURN_OK_RQST
 * -# The allocation is successful
 * -# R_OSAL_IoGetAxiBusName return OSAL_RETURN_PAR
 * -# Allocated buffer is freed successfully
 * @subsection IoGetAxiBusName_TC_06_022_006_testcase Test Case Example
 * @snippet test_io_extension_api.cpp IoGetAxiBusName_TC_06_022_006
 */
// [IoGetAxiBusName_TC_06_022_006]
TEST_F(IoExtensionApiTest, IoGetAxiBusName_TC_06_022_006)
{
    osal_axi_bus_id_t axi_id = OSAL_AXI_BUS_ID_INVALID;
    size_t size = 0;
    size_t byte_written = 0;
    char *p_buffer = NULL;
    size_t buffer_size = 0;

    // Test06_022_006
    // @brief: boundary abnormal case: the buffer_size is less than the size of actual buffer
    EXPECT_EQ(R_OSAL_IoGetAxiBusName(axi_id, p_buffer, buffer_size, &size), OSAL_RETURN_OK_RQST) << "Test06_022_006 failed";
    p_buffer = (char*)malloc(size);
    EXPECT_NE(p_buffer, NULL_PTR) << "Test06_022_006 failed";
    EXPECT_EQ(R_OSAL_IoGetAxiBusName(axi_id, p_buffer, size - 1, &byte_written), OSAL_RETURN_PAR) << "Test06_022_006 failed";
    free(p_buffer);
}
// [IoGetAxiBusName_TC_06_022_006]

/**
 * @page AXI_BUS_TEST AXI Bus API Test
 * @section IoExtensionApiTest_IoGetAxiBusNameList_TC_06_023_001 IoExtensionApiTest.IoGetAxiBusNameList_TC_06_023_001
 * @subsection IoGetAxiBusNameList_TC_06_023_001_tsi Test Suite ID.
 * IoExtensionApiTest
 * @subsection IoGetAxiBusNameList_TC_06_023_001_tci Test Case ID
 * IoGetAxiBusNameList_S1_N_TC_06_023_001
 * @subsection IoGetAxiBusNameList_TC_06_023_001_rr Related Requirement(s)
 * \#276276
 * @subsection IoGetAxiBusNameList_TC_06_023_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Device list of TEST_IO_EXTENSION_DEVICE_TYPE_1 is contained into "device_list"
 * @subsection IoGetAxiBusNameList_TC_06_023_001_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiBusNameList function with normal condition:
 * - Test06_023_001: get list of available axi bus name
 * @subsection IoGetAxiBusNameList_TC_06_023_001_tpts Test Procedure/Test Steps
 * -# Get the required size of axi bus name list by using R_OSAL_IoGetAxiBusNameList
 * -# Allocate buffer with the required size.
 * -# Get the axi bus name list by using R_OSAL_IoGetAxiBusNameList
 * -# Split axi bus name from "axi_name_list" and store into "buffer"
 * -# Get the number of axi ID with the given axi bus name by using R_OSAL_IoGetNumOfAxiIdsForAxiBusName
 * -# Get the axi bus ID list with the given axi bus name by using R_OSAL_IoGetAxiIdListForAxiBusName
 * -# Get the AXI bus name of all AXI IDs in the list above by using R_OSAL_IoGetAxiBusName with step as below:
 *      - Get the required size of the buffer used to store AXI bus name by using R_OSAL_IoGetAxiBusName
 *      - Allocate buffer with the required size.
 *      - memset all elements in this buffer by '\0'
 *      - Get the axi bus name of the givent axi bus id by using R_OSAL_IoGetAxiBusName
 *      - Find the '\0' in the output AXI bus name buffer
 * -# Free allocated buffer.
 * @subsection IoGetAxiBusNameList_TC_06_023_001_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetAxiBusNameList return OSAL_RETURN_OK_RQST.<br>
 * Value of "size" is not equal to 0.
 * -# Memory allocation succeeded (Pointer to buffer != Nullpointer)
 * -# R_OSAL_IoGetAxiBusNameList return OSAL_RETURN_OK.<br>
 * Value of "byte_written" is equal to "size".<br>
 * -# "buffer" is generated successfully.<br>
 * Value of "buffer" at index OSAL_AXI_BUS_ID_INVALID is "invalid".<br>
 * Value of "buffer" at index OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY is "mm(ipa)".
 * -# R_OSAL_IoGetNumOfAxiIdsForAxiBusName return OSAL_RETURN_OK.<br>
 * Value of "num_of_max_entry" is bigger than 0
 * -# R_OSAL_IoGetAxiIdListForAxiBusName return OSAL_RETURN_OK.
 * -# Getting the AXI bus name of all AXI IDs has the below expected result:
 *      - R_OSAL_IoGetAxiBusName return OSAL_RETURN_OK_RQST.<br>
 *      Value of "busname_size" is bigger than 0.
 *      - Memory allocation succeeded (Pointer to buffer != Nullpointer)
 *      - memset run successfully
 *      - R_OSAL_IoGetAxiBusName return OSAL_RETURN_OK.<br>
 *      Value of "byte_written" is equal to "size".
 *      - There is no '\0' in the output AXI bus name buffer
 * -# Memory freeing succeeded.
 * @subsection IoGetAxiBusNameList_TC_06_023_001_testcase Test Case Example
 * @snippet test_io_extension_api.cpp IoGetAxiBusNameList_TC_06_023_001
 */
// [IoGetAxiBusNameList_TC_06_023_001]
TEST_F(IoExtensionApiTest, IoGetAxiBusNameList_TC_06_023_001)
{
    osal_axi_bus_id_t axi_id = OSAL_AXI_BUS_ID_INVALID;

    // Parameter use for R_OSAL_IoGetAxiBusName
    size_t busname_size = 0;
    size_t busname_byte_written = 0;
    char *busname_p_buffer = NULL;

    // Parameter use for R_OSAL_IoGetAxiBusNameList
    char *axi_name_list = NULL;
    size_t busname_list_size = 0;
    size_t busname_list_byte_written = 0;
    char *busname_list_p_buffer = NULL;

    // Parameter use for get axi_id
    osal_axi_bus_id_t *axi_id_list = NULL_PTR;
    size_t num_of_max_entry = 0;
    size_t num_of_written_axi_ids = 0;

    size_t count = 0;
    size_t j = 0;

    // Test06_023_001
    // @brief: normal case: get axi bus name list
    // get the required size for axi bus name list
    EXPECT_EQ(R_OSAL_IoGetAxiBusNameList(busname_list_p_buffer, busname_list_size, &busname_list_size), OSAL_RETURN_OK_RQST) << "Test06_023_001 failed";
    EXPECT_NE(busname_list_size, 0) << "Test06_023_001 failed";
    axi_name_list = (char*)malloc(busname_list_size);
    EXPECT_NE(axi_name_list, NULL_PTR) << "Test06_023_001 failed";

    // get axi bus name list
    EXPECT_EQ(R_OSAL_IoGetAxiBusNameList(axi_name_list, busname_list_size, &busname_list_byte_written), OSAL_RETURN_OK) << "Test06_023_001 failed";
    EXPECT_EQ(busname_list_byte_written, busname_list_size) << "Test06_023_001 failed";
    for(size_t i = 0; i < busname_list_size; i++)
    {
        if(axi_name_list[i] == '\0')
        {
            count++;
        }
    }
    if(count <= 2) // only has "invalid" and "mm(ipa)"
    {
        ADD_FAILURE();
    }
    else
    {
        char* buffer[count];
        buffer[0] = &axi_name_list[0];
        for (size_t i = 0; i < (busname_list_size - 1); i++)
        {
            if (axi_name_list[i] == '\0')
            {
                j++;
                buffer[j] = &axi_name_list[i+1U];
            }
        }

        for (size_t i = 0; i < count; i++) /* Dump log */
        {
            printf("buffer[ %d ] = %s\n", i, buffer[i]);
        }

        EXPECT_TRUE(!strcmp(buffer[OSAL_AXI_BUS_ID_INVALID], "invalid")) << "Test06_023_001 failed";
        EXPECT_TRUE(!strcmp(buffer[OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY], "mm(ipa)")) << "Test06_023_001 failed";

        // Get axi ID list from axi bus name & get the first axi ID from this list
        EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForAxiBusName(buffer[2], &num_of_max_entry), OSAL_RETURN_OK) << "Test06_023_001 failed";
        EXPECT_GT(num_of_max_entry, 0) << "Test06_023_001 failed";
        axi_id_list = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);
        EXPECT_EQ(R_OSAL_IoGetAxiIdListForAxiBusName(buffer[2], axi_id_list, num_of_max_entry, &num_of_written_axi_ids), OSAL_RETURN_OK) << "Test06_023_001 failed";

        for (auto i=0 ; i < num_of_written_axi_ids ; i++)
        {
            axi_id = axi_id_list[i];
            char *axi_name;
            busname_size = 0;
            busname_p_buffer = NULL;
            busname_byte_written = 0;

            EXPECT_NE(axi_id, OSAL_AXI_BUS_ID_INVALID) << "Test06_023_001 failed";
            // Get axi bus name corresponding with axi id
            EXPECT_EQ(R_OSAL_IoGetAxiBusName(axi_id, busname_p_buffer, busname_size, &busname_size), OSAL_RETURN_OK_RQST) << "Test06_023_001 failed";
            EXPECT_GT(busname_size, 0) << "Test06_023_001 failed";
            axi_name = (char*)malloc(busname_size);
            EXPECT_NE(axi_name, NULL_PTR) << "Test06_023_001 failed";
            (void)memset(axi_name, '\0', busname_size);
            EXPECT_EQ(R_OSAL_IoGetAxiBusName(axi_id, axi_name, busname_size, &busname_byte_written), OSAL_RETURN_OK) << "Test06_023_001 failed";
            EXPECT_EQ(busname_size, busname_byte_written) << "Test06_023_001 failed";
            for(size_t j = 0; j < (busname_byte_written - 1); j++) // the last element of axi name must be '\0' as the requirement.
            {
                EXPECT_TRUE(axi_name[j] != '\0');
            }
            free(axi_name);
        }
        free(axi_id_list);
    }
    // post-process: free allocated buffer
    free(axi_name_list);
}
// [IoGetAxiBusNameList_TC_06_023_001]

/**
 * @page AXI_BUS_TEST AXI Bus API Test
 * @section IoExtensionApiTest_IoGetAxiBusNameList_TC_06_023_002 IoExtensionApiTest.IoGetAxiBusNameList_TC_06_023_002
 * @subsection IoGetAxiBusNameList_TC_06_023_002_tsi Test Suite ID.
 * IoExtensionApiTest
 * @subsection IoGetAxiBusNameList_TC_06_023_002_tci Test Case ID
 * IoGetAxiBusNameList_S0_A_TC_06_023_002
 * @subsection IoGetAxiBusNameList_TC_06_023_002_rr Related Requirement(s)
 * \#276276
 * @subsection IoGetAxiBusNameList_TC_06_023_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetAxiBusNameList_TC_06_023_002_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiBusNameList function with abnormal condition:
 * - Test06_023_002: get axi bus name list with p_num_of_byte is NULL
 * @subsection IoGetAxiBusNameList_TC_06_023_002_tpts Test Procedure/Test Steps
 * -# Get the required size of axi bus name list by using R_OSAL_IoGetAxiBusNameList
 * -# Allocate buffer with the required size.
 * -# Get the axi bus name list by using R_OSAL_IoGetAxiBusNameList with the arguments as below:
 *      - "axi_name_list" to store the axi bus name list
 *      - "size" is the required size got from step 1
 *      - NULL
 * -# Free allocated buffer
 * @subsection IoGetAxiBusNameList_TC_06_023_002_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetAxiBusNameList return OSAL_RETURN_OK_RQST.<br>
 * Value of "size" is not equal to 0.
 * -# Memory allocation succeeded (Pointer to buffer != Nullpointer)
 * -# R_OSAL_IoGetAxiBusNameList return OSAL_RETURN_PAR
 * -# Memory freeing succeeded
 * @subsection IoGetAxiBusNameList_TC_06_023_002_testcase Test Case Example
 * @snippet test_io_extension_api.cpp IoGetAxiBusNameList_TC_06_023_002
 */
// [IoGetAxiBusNameList_TC_06_023_002]
TEST_F(IoExtensionApiTest, IoGetAxiBusNameList_TC_06_023_002)
{
    size_t size = 0;
    char *axi_name_list;
    char *p_buffer = NULL;
    size_t *p_num_of_byte = NULL;

    // Test06_023_002
    // @brief: abnormal case: p_num_of_byte is NULL
    EXPECT_EQ(R_OSAL_IoGetAxiBusNameList(p_buffer, size, &size), OSAL_RETURN_OK_RQST) << "Test06_023_002 failed";
    EXPECT_NE(size, 0) << "Test06_023_002 failed";
    axi_name_list = (char*)malloc(size);
    EXPECT_NE(axi_name_list, NULL_PTR) << "Test06_023_002 failed";
    EXPECT_EQ(R_OSAL_IoGetAxiBusNameList(axi_name_list, size, p_num_of_byte), OSAL_RETURN_PAR) << "Test06_023_002 failed";
    free(axi_name_list);
}
// [IoGetAxiBusNameList_TC_06_023_002]

/**
 * @page AXI_BUS_TEST AXI Bus API Test
 * @section IoExtensionApiTest_IoGetAxiBusNameList_TC_06_023_003 IoExtensionApiTest.IoGetAxiBusNameList_TC_06_023_003
 * @subsection IoGetAxiBusNameList_TC_06_023_003_tsi Test Suite ID.
 * IoExtensionApiTest
 * @subsection IoGetAxiBusNameList_TC_06_023_003_tci Test Case ID
 * IoGetAxiBusNameList_S0_A_TC_06_023_003
 * @subsection IoGetAxiBusNameList_TC_06_023_003_rr Related Requirement(s)
 * \#276276
 * @subsection IoGetAxiBusNameList_TC_06_023_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetAxiBusNameList_TC_06_023_003_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiBusNameList function with abnormal condition:
 * - pre-process: get the required size of axi bus name list
 * - Test06_023_003: get axi bus name list with p_buffer is NULL
 * @subsection IoGetAxiBusNameList_TC_06_023_003_tpts Test Procedure/Test Steps
 * -# Get the required size of axi bus name list by using R_OSAL_IoGetAxiBusNameList
 * -# Get the axi bus name list by using R_OSAL_IoGetAxiBusNameList with the arguments as below:
 *      - NULL
 *      - "size" is the required size got from step 1
 *      - Address of "byte_written"
 * @subsection IoGetAxiBusNameList_TC_06_023_003_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetAxiBusNameList return OSAL_RETURN_OK_RQST.<br>
 * Value of "size" is not equal to 0.
 * -# R_OSAL_IoGetAxiBusNameList return OSAL_RETURN_PAR.<br>
 * Value of "byte_written" is equal to 0
 * @subsection IoGetAxiBusNameList_TC_06_023_003_testcase Test Case Example
 * @snippet test_io_extension_api.cpp IoGetAxiBusNameList_TC_06_023_003
 */
// [IoGetAxiBusNameList_TC_06_023_003]
TEST_F(IoExtensionApiTest, IoGetAxiBusNameList_TC_06_023_003)
{
    size_t size = 0;
    size_t byte_written = 0;
    char *p_buffer = NULL;

    // pre-process: get the required size of axi bus name list
    EXPECT_EQ(R_OSAL_IoGetAxiBusNameList(p_buffer, size, &size), OSAL_RETURN_OK_RQST) << "pre-process failed";
    EXPECT_NE(size, 0) << "pre-process failed";

    // Test06_023_003
    // @brief: abnormal case: p_buffer is NULL
    EXPECT_EQ(R_OSAL_IoGetAxiBusNameList(p_buffer, size, &byte_written), OSAL_RETURN_PAR) << "Test06_023_003 failed";
    EXPECT_EQ(byte_written, 0) << "Test06_023_003 failed";
}
// [IoGetAxiBusNameList_TC_06_023_003]

/**
 * @page AXI_BUS_TEST AXI Bus API Test
 * @section IoExtensionApiTest_IoGetAxiBusNameList_TC_06_023_004 IoExtensionApiTest.IoGetAxiBusNameList_TC_06_023_004
 * @subsection IoGetAxiBusNameList_TC_06_023_004_tsi Test Suite ID.
 * IoExtensionApiTest
 * @subsection IoGetAxiBusNameList_TC_06_023_004_tci Test Case ID
 * IoGetAxiBusNameList_S0_A_TC_06_023_004
 * @subsection IoGetAxiBusNameList_TC_06_023_004_rr Related Requirement(s)
 * \#276276
 * @subsection IoGetAxiBusNameList_TC_06_023_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetAxiBusNameList_TC_06_023_004_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiBusNameList function with normal condition:
 * - Test06_023_004: Get the axi bus name list with p_buffer is NULL and buffer_size is equal to 0.
 * @subsection IoGetAxiBusNameList_TC_06_023_004_tpts Test Procedure/Test Steps
 * -# Get the required size of axi bus name list by using R_OSAL_IoGetAxiBusNameList with p_buffer is NULL and buffer_size is equal to 0.
 * -# Allocate required buffer
 * -# Memset the allocated buffer with '\0'
 * -# Get the AXI bus name list by using R_OSAL_IoGetAxiBusNameList
 * -# Find '\0' in the output AXI bus name list 
 * -# Free allocated buffer
 * @subsection IoGetAxiBusNameList_TC_06_023_004_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetAxiBusNameList return OSAL_RETURN_OK_RQST.<br>
 * Value of "size" is not equal to 0.
 * -# Allocation success (Pointer to buffer != Nullpointer)
 * -# Memset run successfully
 * -# R_OSAL_IoGetAxiBusNameList return OSAL_RETURN_OK
 * -# No '\0' will be found in the AXI bus name list
 * -# Free buffer successfully
 * @subsection IoGetAxiBusNameList_TC_06_023_004_testcase Test Case Example
 * @snippet test_io_extension_api.cpp IoGetAxiBusNameList_TC_06_023_004
 */
// [IoGetAxiBusNameList_TC_06_023_004]
TEST_F(IoExtensionApiTest, IoGetAxiBusNameList_TC_06_023_004)
{
    size_t size = 0;
    char *p_buffer = NULL;
    char* name_list_buffer = NULL_PTR;
    size_t byte_written = 0;

    // Test06_023_004
    // @brief: normal case: Get the axi bus name list with p_buffer is NULL and buffer_size is equal to 0.
    EXPECT_EQ(R_OSAL_IoGetAxiBusNameList(p_buffer, size, &size), OSAL_RETURN_OK_RQST) << "Test06_023_004 failed";
    EXPECT_NE(size, 0) << "Test06_023_004 failed";
    name_list_buffer = (char*)malloc(sizeof(char) * size);
    EXPECT_NE(name_list_buffer, NULL_PTR) << "Test06_023_004 failed";
    (void)memset(name_list_buffer, '\n', sizeof(name_list_buffer));
    EXPECT_EQ(R_OSAL_IoGetAxiBusNameList(name_list_buffer, size, &byte_written), OSAL_RETURN_OK) << "Test06_023_004 failed";
    EXPECT_EQ(byte_written, size) << "Test06_023_004 failed";
    for (size_t i = 0; i < byte_written; i++)
    {
        EXPECT_TRUE(name_list_buffer[i] != '\n');
    }

    free(name_list_buffer);
}
// [IoGetAxiBusNameList_TC_06_023_004]

/**
 * @page AXI_BUS_TEST AXI Bus API Test
 * @section IoExtensionApiTest_IoGetAxiBusNameList_TC_06_023_005 IoExtensionApiTest.IoGetAxiBusNameList_TC_06_023_005
 * @subsection IoGetAxiBusNameList_TC_06_023_005_tsi Test Suite ID.
 * IoExtensionApiTest
 * @subsection IoGetAxiBusNameList_TC_06_023_005_tci Test Case ID
 * IoGetAxiBusNameList_S0_A_TC_06_023_005
 * @subsection IoGetAxiBusNameList_TC_06_023_005_rr Related Requirement(s)
 * \#276276
 * @subsection IoGetAxiBusNameList_TC_06_023_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetAxiBusNameList_TC_06_023_005_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiBusNameList function with abnormal condition:
 * - pre-process: get the required size of axi bus name list
 * - Test06_023_003: get axi bus name list with p_buffer is NULL
 * - post-process: free allocated buffer
 * @subsection IoGetAxiBusNameList_TC_06_023_005_tpts Test Procedure/Test Steps
 * -# Get the required size of axi bus name list by using R_OSAL_IoGetAxiBusNameList
 * -# Allocate buffer with the required size.
 * -# Get the axi bus name list by using R_OSAL_IoGetAxiBusNameList with the arguments as below:
 *      - "axi_name_list" to store the axi bus name list
 *      - "size" minus 1 while "size" is the required size got from step 1
 *      - Address of "byte_written"
 * -# Free allocated buffer
 * @subsection IoGetAxiBusNameList_TC_06_023_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetAxiBusNameList return OSAL_RETURN_OK_RQST.<br>
 * Value of "size" is not equal to 0.
 * -# Memory allocation succeeded (Pointer to buffer != Nullpointer)
 * -# R_OSAL_IoGetAxiBusNameList return OSAL_RETURN_PAR.<br>
 * Value of "byte_written" is equal to 0
 * -# Memory freeing succeeded
 * @subsection IoGetAxiBusNameList_TC_06_023_005_testcase Test Case Example
 * @snippet test_io_extension_api.cpp IoGetAxiBusNameList_TC_06_023_005
 */
// [IoGetAxiBusNameList_TC_06_023_005]
TEST_F(IoExtensionApiTest, IoGetAxiBusNameList_TC_06_023_005)
{
    size_t size = 0;
    size_t byte_written = 0;
    char *axi_name_list;
    char *p_buffer = NULL;

    // pre-process: get the required size of axi bus name list
    EXPECT_EQ(R_OSAL_IoGetAxiBusNameList(p_buffer, size, &size), OSAL_RETURN_OK_RQST) << "pre-process failed";
    EXPECT_NE(size, 0) << "pre-process failed";
    axi_name_list = (char*)malloc(size);
    EXPECT_NE(axi_name_list, NULL_PTR) << "pre-process failed";

    // Test06_023_005
    // @brief: abnormal case: buffer_size is less than the required size
    EXPECT_EQ(R_OSAL_IoGetAxiBusNameList(axi_name_list, size - 1, &byte_written), OSAL_RETURN_PAR) << "Test06_023_005 failed";
    EXPECT_EQ(byte_written, 0);

    // post-process: free allocated buffer
    free(axi_name_list);
}
// [IoGetAxiBusNameList_TC_06_023_005]

/**
 * @page AXI_BUS_TEST AXI Bus API Test
 * @section OsalApiStateTest_IoGetAxiBusNameList_TC_06_023_006 OsalApiStateTest.IoGetAxiBusNameList_TC_06_023_006
 * @subsection IoGetAxiBusNameList_TC_06_023_006_tsi Test Suite ID.
 * OsalApiStateTest
 * @subsection IoGetAxiBusNameList_TC_06_023_006_tci Test Case ID
 * IoGetAxiBusNameList_S2_A_TC_06_023_006
 * @subsection IoGetAxiBusNameList_TC_06_023_006_rr Related Requirement(s)
 * \#276276
 * @subsection IoGetAxiBusNameList_TC_06_023_006_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection IoGetAxiBusNameList_TC_06_023_006_tcs Test Case Summary
 * This test shall verify the state transition of IO Extension APIs with abnormal condition:
 * - Test06_023_006: state transition error with R_OSAL_IoGetAxiBusNameList.
 * @subsection IoGetAxiBusNameList_TC_06_023_006_tpts Test Procedure/Test Steps
 * -# Get the required size of axi bus name list by using R_OSAL_IoGetAxiBusNameList.
 * @subsection IoGetAxiBusNameList_TC_06_023_006_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetAxiBusNameList return OSAL_RETURN_STATE.<br>
 * Value of "byte_written" is equal to 0
 * @subsection IoGetAxiBusNameList_TC_06_023_006_testcase Test Case Example
 * @snippet test_io_extension_api.cpp IoGetAxiBusNameList_TC_06_023_006
 */
// [IoGetAxiBusNameList_TC_06_023_006]
TEST_F(OsalApiStateTest, IoGetAxiBusNameList_TC_06_023_006)
{
    size_t size = 0;
    size_t byte_written = 0;
    char *axi_name_list = NULL;

// Test06_023_006
// @brief: abnormal case: state transition error with R_OSAL_IoGetAxiBusNameList
    EXPECT_EQ(R_OSAL_IoGetAxiBusNameList(axi_name_list, size, &byte_written), OSAL_RETURN_STATE) << "Test06_023_006 failed";
    EXPECT_EQ(byte_written, 0) << "Test06_023_006 failed";
}
// [IoGetAxiBusNameList_TC_06_023_006]

/**
 * @page AXI_BUS_TEST AXI Bus API Test
 * @section IoExtensionApiTest_IoGetDeviceAxiBusId_TC_06_024_001 IoExtensionApiTest.IoGetDeviceAxiBusId_TC_06_024_001
 * @subsection IoGetDeviceAxiBusId_TC_06_024_001_tsi Test Suite ID.
 * IoExtensionApiTest
 * @subsection IoGetDeviceAxiBusId_TC_06_024_001_tci Test Case ID
 * IoGetDeviceAxiBusId_S1_N_TC_06_024_001
 * @subsection IoGetDeviceAxiBusId_TC_06_024_001_rr Related Requirement(s)
 * \#199211, \#276272
 * @subsection IoGetDeviceAxiBusId_TC_06_024_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Device list of TEST_IO_EXTENSION_DEVICE_TYPE_1 is contained into "device_list"
 * - Axi bus name of corresponding test target devices are stored into gs_osal_test_axi_data
 * @subsection IoGetDeviceAxiBusId_TC_06_024_001_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetDeviceAxiBusId function with normal condition:
 * - pre-process: open test target device
 * - Test06_024_001: get axi bus id from device handle of test target device
 * - post-process: close test target device
 * @subsection IoGetDeviceAxiBusId_TC_06_024_001_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Get the axi bus id by using R_OSAL_IoGetDeviceAxiBusId
 * -# Get the required size of axi bus name of axi bus id got from step 2 by using R_OSAL_IoGetAxiBusName
 * -# Allocate buffer with the required size.
 * -# Get the axi bus name base on axi bus id got from step 2 by using R_OSAL_IoGetAxiBusName
 * -# Free allocated memory
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoGetDeviceAxiBusId_TC_06_024_001_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not equal to NULL_PTR
 * -# R_OSAL_IoGetDeviceAxiBusId return OSAL_RETURN_OK.<br>
 * Value of "axi_id" is not equal to OSAL_AXI_BUS_ID_INVALID
 * -# R_OSAL_IoGetAxiBusName return OSAL_RETURN_OK_RQST.<br>
 * Value of "size" is not equal to 0
 * -# Memory allocation succeeded (Pointer to buffer != Nullpointer)
 * -# R_OSAL_IoGetAxiBusName return OSAL_RETURN_OK.<br>
 * "byte_written" is equal to "size".<br>
 * "axi_name" is equal to "axi_name" of "gs_osal_test_axi_data"
 * -# Memory freeing succeeded.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoGetDeviceAxiBusId_TC_06_024_001_testcase Test Case Example
 * @snippet test_io_extension_api.cpp IoGetDeviceAxiBusId_TC_06_024_001
 */
// [IoGetDeviceAxiBusId_TC_06_024_001]
TEST_F(IoExtensionApiTest, IoGetDeviceAxiBusId_TC_06_024_001)
{
    osal_axi_bus_id_t axi_id = OSAL_AXI_BUS_ID_INVALID;
    size_t size = 0;
    size_t byte_written = 0;
    size_t test_target = 0;
    char *axi_name;

    for(test_target = 0; test_target < sizeof(gs_osal_test_axi_data)/sizeof(gs_osal_test_axi_data[0]); test_target++)
    {
        // pre-process: open test target device
        EXPECT_EQ(R_OSAL_IoDeviceOpen(device_list.at(test_target).c_str(), &device_handle), OSAL_RETURN_OK) << "pre-process failed";
        EXPECT_NE(device_handle, NULL_PTR);

        // Test06_024_001
        // @brief: normal case: get axi bus id from device handle
        EXPECT_EQ(R_OSAL_IoGetDeviceAxiBusId(device_handle, &axi_id), OSAL_RETURN_OK) << "Test06_024_001 failed";
        EXPECT_NE(axi_id, OSAL_AXI_BUS_ID_INVALID) << "Test06_024_001 failed";
        EXPECT_NE(axi_id, OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY) << "Test06_024_001 failed";
        size_t buffer_size = 0;
        char *p_buffer = NULL;
        EXPECT_EQ(R_OSAL_IoGetAxiBusName(axi_id, p_buffer, buffer_size, &size), OSAL_RETURN_OK_RQST) << "Test06_024_001 failed";
        EXPECT_NE(size, 0) << "Test06_024_001 failed";
        axi_name = (char*)malloc(size);
        EXPECT_NE(axi_name, NULL_PTR) << "Test06_024_001 failed";
        EXPECT_EQ(R_OSAL_IoGetAxiBusName(axi_id, axi_name, size, &byte_written), OSAL_RETURN_OK) << "Test06_024_001 failed";
        EXPECT_EQ(size, byte_written) << "Test06_024_001 failed";
        for (size_t it = 0; it < sizeof(gs_osal_test_axi_data)/sizeof(gs_osal_test_axi_data[0]); it++)
        {
            if(!strcmp(gs_osal_test_axi_data[it].device_id, device_list.at(test_target).c_str()))
            {
                EXPECT_FALSE(strcmp(gs_osal_test_axi_data[it].axi_name, axi_name)) << "Test06_024_001 failed";
            }
        }
        free(axi_name);

        // post-process: Close test target device
        EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    }
}
// [IoGetDeviceAxiBusId_TC_06_024_001]

/**
 * @page AXI_BUS_TEST AXI Bus API Test
 * @section IoExtensionApiTest_IoGetDeviceAxiBusId_TC_06_024_002 IoExtensionApiTest.IoGetDeviceAxiBusId_TC_06_024_002
 * @subsection IoGetDeviceAxiBusId_TC_06_024_002_tsi Test Suite ID.
 * IoExtensionApiTest
 * @subsection IoGetDeviceAxiBusId_TC_06_024_002_tci Test Case ID
 * IoGetDeviceAxiBusId_S0_A_TC_06_024_002
 * @subsection IoGetDeviceAxiBusId_TC_06_024_002_rr Related Requirement(s)
 * \#276272, \#276279
 * @subsection IoGetDeviceAxiBusId_TC_06_024_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Device list of TEST_IO_EXTENSION_DEVICE_TYPE_1 is contained into "device_list"
 * @subsection IoGetDeviceAxiBusId_TC_06_024_002_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetDeviceAxiBusId function with abnormal condition:
 * - pre-process: open device test target device.
 * - Test06_024_002: get axi bus id with axi_id is NULL
 * - pre-process: close device test target device.
 * @subsection IoGetDeviceAxiBusId_TC_06_024_002_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Get the axi bus id by using R_OSAL_IoGetDeviceAxiBusId with the arguments as below:
 *      - Device handle of test target device
 *      - NULL
 * -# Close test target device for the next test cases by using R_OSAL_IoDeviceClose
 * @subsection IoGetDeviceAxiBusId_TC_06_024_002_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not equal to NULL_PTR
 * -# R_OSAL_IoGetDeviceAxiBusId return OSAL_RETURN_PAR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoGetDeviceAxiBusId_TC_06_024_002_testcase Test Case Example
 * @snippet test_io_extension_api.cpp IoGetDeviceAxiBusId_TC_06_024_002
 */
// [IoGetDeviceAxiBusId_TC_06_024_002]
TEST_F(IoExtensionApiTest, IoGetDeviceAxiBusId_TC_06_024_002)
{
    osal_axi_bus_id_t *p_axi_id = NULL;

// pre-process: open test target device
    EXPECT_EQ(R_OSAL_IoDeviceOpen(device_list.at(0).c_str(), &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";

// Test06_024_002
// @brief: abnormal case: axi_id is NULL
    EXPECT_EQ(R_OSAL_IoGetDeviceAxiBusId(device_handle, p_axi_id), OSAL_RETURN_PAR) << "Test06_024_002 failed";

// pre-process: close test target device
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
}
// [IoGetDeviceAxiBusId_TC_06_024_002]

/**
 * @page AXI_BUS_TEST AXI Bus API Test
 * @section IoExtensionApiTest_IoGetDeviceAxiBusId_TC_06_024_003 IoExtensionApiTest.IoGetDeviceAxiBusId_TC_06_024_003
 * @subsection IoGetDeviceAxiBusId_TC_06_024_003_tsi Test Suite ID.
 * IoExtensionApiTest
 * @subsection IoGetDeviceAxiBusId_TC_06_024_003_tci Test Case ID
 * IoGetDeviceAxiBusId_S0_A_TC_06_024_003
 * @subsection IoGetDeviceAxiBusId_TC_06_024_003_rr Related Requirement(s)
 * \#199199, \#276272
 * @subsection IoGetDeviceAxiBusId_TC_06_024_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetDeviceAxiBusId_TC_06_024_003_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetDeviceAxiBusId function with abnormal condition:
 * - Test06_024_003: get axi bus id with device handle is NULL
 * @subsection IoGetDeviceAxiBusId_TC_06_024_003_tpts Test Procedure/Test Steps
 * -# Get the axi bus id by using R_OSAL_IoGetDeviceAxiBusId with the arguments as below:
 *      - NULL
 *      - Address of "axi_id"
 * @subsection IoGetDeviceAxiBusId_TC_06_024_003_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetDeviceAxiBusId return OSAL_RETURN_HANDLE.<br>
 * "axi_id" is equal to OSAL_AXI_BUS_ID_INVALID.
 * @subsection IoGetDeviceAxiBusId_TC_06_024_003_testcase Test Case Example
 * @snippet test_io_extension_api.cpp IoGetDeviceAxiBusId_TC_06_024_003
 */
// [IoGetDeviceAxiBusId_TC_06_024_003]
TEST_F(IoExtensionApiTest, IoGetDeviceAxiBusId_TC_06_024_003)
{
    osal_axi_bus_id_t axi_id = OSAL_AXI_BUS_ID_INVALID;

// Test06_024_003
// @brief: abnormal case: device handle is NULL
    EXPECT_EQ(R_OSAL_IoGetDeviceAxiBusId(OSAL_DEVICE_HANDLE_INVALID, &axi_id), OSAL_RETURN_HANDLE) << "Test06_024_003 failed";
    EXPECT_EQ(axi_id, OSAL_AXI_BUS_ID_INVALID) << "Test06_024_003 failed";
}
// [IoGetDeviceAxiBusId_TC_06_024_003]

/**
 * @page AXI_BUS_TEST AXI Bus API Test
 * @section OsalApiStateTest_IoGetDeviceAxiBusId_TC_06_024_004 OsalApiStateTest.IoGetDeviceAxiBusId_TC_06_024_004
 * @subsection IoGetDeviceAxiBusId_TC_06_024_004_tsi Test Suite ID.
 * OsalApiStateTest
 * @subsection IoGetDeviceAxiBusId_TC_06_024_004_tci Test Case ID
 * IoGetDeviceAxiBusId_S2_A_TC_06_024_004
 * @subsection IoGetDeviceAxiBusId_TC_06_024_004_rr Related Requirement(s)
 * \#276272
 * @subsection IoGetDeviceAxiBusId_TC_06_024_004_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection IoGetDeviceAxiBusId_TC_06_024_004_tcs Test Case Summary
 * This test shall verify the state transition of IO Extension APIs with abnormal condition:
 * - Test06_024_004: state transition error with R_OSAL_IoGetDeviceAxiBusId.
 * @subsection IoGetDeviceAxiBusId_TC_06_024_004_tpts Test Procedure/Test Steps
 * -# Get the axi bus id by using R_OSAL_IoGetDeviceAxiBusId.
 * @subsection IoGetDeviceAxiBusId_TC_06_024_004_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetDeviceAxiBusId return OSAL_RETURN_STATE.<br>
 * "axi_id" is equal to OSAL_AXI_BUS_ID_INVALID.
 * @subsection IoGetDeviceAxiBusId_TC_06_024_004_testcase Test Case Example
 * @snippet test_io_extension_api.cpp IoGetDeviceAxiBusId_TC_06_024_004
 */
// [IoGetDeviceAxiBusId_TC_06_024_004]
TEST_F(OsalApiStateTest, IoGetDeviceAxiBusId_TC_06_024_004)
{
    osal_axi_bus_id_t axi_id = OSAL_AXI_BUS_ID_INVALID;
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;

    // Test06_024_004
    // @brief: abnormal case: state transition error with R_OSAL_IoGetDeviceAxiBusId
    EXPECT_EQ(R_OSAL_IoGetDeviceAxiBusId(device_handle, &axi_id), OSAL_RETURN_STATE) << "Test06_024_004 failed";
    EXPECT_EQ(axi_id, OSAL_AXI_BUS_ID_INVALID) << "Test06_024_004 failed";
}
// [IoGetDeviceAxiBusId_TC_06_024_004]

/**
 * @page AXI_BUS_TEST AXI Bus API Test
 * @section IoExtensionApiTest_IoGetDeviceAxiBusId_TC_06_024_005 IoExtensionApiTest.IoGetDeviceAxiBusId_TC_06_024_005
 * @subsection IoGetDeviceAxiBusId_TC_06_024_005_tsi Test Suite ID.
 * IoExtensionApiTest
 * @subsection IoGetDeviceAxiBusId_TC_06_024_005_tci Test Case ID
 * IoGetDeviceAxiBusId_S0_A_TC_06_024_005
 * @subsection IoGetDeviceAxiBusId_TC_06_024_005_rr Related Requirement(s)
 * \#199211, \#276272
 * @subsection IoGetDeviceAxiBusId_TC_06_024_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetDeviceAxiBusId_TC_06_024_005_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetDeviceAxiBusId function with abnormal condition:
 * - Test06_024_005: get axi bus id from device handle was already closed by R_OSAL_IoDeviceClose
 * @subsection IoGetDeviceAxiBusId_TC_06_024_005_tpts Test Procedure/Test Steps
 * -# Get the axi bus id by using R_OSAL_IoGetDeviceAxiBusId with device handle was already closed by R_OSAL_IoDeviceClose
 * @subsection IoGetDeviceAxiBusId_TC_06_024_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetDeviceAxiBusId return OSAL_RETURN_HANDLE.
 * @subsection IoGetDeviceAxiBusId_TC_06_024_005_testcase Test Case Example
 * @snippet test_io_extension_api.cpp IoGetDeviceAxiBusId_TC_06_024_005
 */
// [IoGetDeviceAxiBusId_TC_06_024_005]
TEST_F(IoExtensionApiTest, IoGetDeviceAxiBusId_TC_06_024_005)
{
    osal_axi_bus_id_t axi_id = OSAL_AXI_BUS_ID_INVALID;

// Test06_024_005
// @brief: abnormal case: device was already closed by R_OSAL_IoDeviceClose
    EXPECT_EQ(R_OSAL_IoDeviceOpen(device_list.at(0).c_str(), &device_handle), OSAL_RETURN_OK) << "Test06_024_005 failed";
    EXPECT_NE(device_handle, OSAL_DEVICE_HANDLE_INVALID) << "Test06_024_005 failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "Test06_024_005 failed";
    EXPECT_EQ(R_OSAL_IoGetDeviceAxiBusId(device_handle, &axi_id), OSAL_RETURN_HANDLE) << "Test06_024_005 failed";
}
// [IoGetDeviceAxiBusId_TC_06_024_005]
