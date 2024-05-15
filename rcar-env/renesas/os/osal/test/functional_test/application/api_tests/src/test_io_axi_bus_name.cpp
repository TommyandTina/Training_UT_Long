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
* File Name :    test_io_axi_bus_name.cpp
* Version :      3.2.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL IO Manager API(AXI Bus Name) Functional Test (From the perspective of OSAL API Specification)
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           2.0.0     28.10.2021    Create New
 *           2.1.0     18.11.2021    Add Test Cases
 *                                       - IoGetAxiIdListForAXIBusName_TC_06_028_012
 *                                       - IoGetAxiIdListForAXIBusName_TC_06_028_013
 *                                       - IoGetAxiIdListForDeviceHdl_TC_06_029_009
 *                                       - IoGetAxiIdListForDeviceHdl_TC_06_029_010
 *                                       - IoGetAxiIdListForDeviceHdl_TC_06_029_011
 *                                       - IoGetAxiIdListForDeviceName_TC_06_030_009
 *                                       - IoGetAxiIdListForDeviceName_TC_06_030_010
 *                                       - IoGetAxiIdListForDeviceName_TC_06_030_011
 *                                   Fix Test Data by using Device information
 *                                       - https://jira-gasg.renesas.eu/confluence/display/SWDev/xOS+Device+information
 *                                   Fix Expect value
 *                                       - IoGetAxiIdListForDeviceName_TC_06_030_006
 *                                       - IoGetAxiIdListForDeviceHdl_TC_06_029_007
 *                                       - IoGetNumOfAXIIdsForDeviceName_TC_06_027_005
 *                                       - IoGetNumOfAXIIdsForDeviceHdl_TC_06_026_004
 *                                            OSAL_RETURN_UNKNOWN_NAME -> OSAL_RETURN_OK
 *                                   Add Doxygen Comment for each Test Cases
 *           2.1.1     18.11.2021    Fix Typo
 *           3.0.0     21.01.2022    Add V4H Test Data
 *                     02.02.2022    Update V4H Test Data (refer to device informantion v3.-1.0)
 *                                   Add S4 Test Data (refer to device informantion v3.-1.0)
 *                     15.02.2022    Apply new device and its setting definition
 *                     20.02.2022    Temporarily disable IoAXIBusNameApiTest on S4
 *                     20.02.2022    Enable IoAXIBusNameApiTest on S4
 *                                   Update axi_bus_name_test_data for S4
 *                     04.03.2022    Update S4 and V4H AXI Bus Name information
 *           3.0.1     21.04.2022    Update V4H AXI Bus Name information
 *           3.0.2     19.05.2022    Update AXI Bus Name test data for V3M2 SIL
 *           3.1.0     04.01.2023    Add new test cases to cover OSAL API Specification
 *                                       - IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_006
 *                                       - IoGetAxiIdListForAxiBusName_TC_06_028_014
 *                                       - IoGetAxiIdListForDeviceHdl_TC_06_029_012
 *                                       - IoGetAxiIdListForDeviceName_TC_06_030_012
 *                                   Update test code of test case
 *                                       - IoGetNumOfAxiIdsForDeviceName_TC_06_027_005
 *                                       - IoGetAxiIdListForDeviceName_TC_06_030_006
 *           3.2.0     02.03.2023    Update Normal Case Test Cases for AXI Bus API
 *                                       - added test Sequential device
 *                                       - for example) ims_00, ims_01 
 *           3.3.0     11.09.2023   Add AXI Name definition of V4M
***********************************************************************************************************************/
#include "test_common.hpp"

#if !defined(OSAL_TEST_FRAMEWORK)
using namespace std;
#else
using namespace OsalTestFrameworkMatcher;
#endif

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @test
 * @test @link IoAXIBusNameApiTest_IoGetNumOfAxiIdsForAxiBusName_TC_06_025_001 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetNumOfAxiIdsForAxiBusName_TC_06_025_002 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetNumOfAxiIdsForAxiBusName_TC_06_025_003 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetNumOfAxiIdsForAxiBusName_TC_06_025_004 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetNumOfAxiIdsForAxiBusName_TC_06_025_005 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetNumOfAxiIdsForAxiBusName_TC_06_025_006 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetNumOfAxiIdsForAxiBusName_TC_06_025_007 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetNumOfAxiIdsForAxiBusName_TC_06_025_008 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetNumOfAxiIdsForAxiBusName_TC_06_025_009 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_001 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_002 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_003 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_004 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_005 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_006 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetNumOfAxiIdsForDeviceName_TC_06_027_001 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetNumOfAxiIdsForDeviceName_TC_06_027_002 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetNumOfAxiIdsForDeviceName_TC_06_027_003 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetNumOfAxiIdsForDeviceName_TC_06_027_004 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetNumOfAxiIdsForDeviceName_TC_06_027_005 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetNumOfAxiIdsForDeviceName_TC_06_027_006 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForAxiBusName_TC_06_028_001 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForAxiBusName_TC_06_028_002 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForAxiBusName_TC_06_028_003 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForAxiBusName_TC_06_028_004 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForAxiBusName_TC_06_028_005 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForAxiBusName_TC_06_028_006 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForAxiBusName_TC_06_028_007 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForAxiBusName_TC_06_028_008 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForAxiBusName_TC_06_028_009 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForAxiBusName_TC_06_028_010 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForAxiBusName_TC_06_028_011 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForAxiBusName_TC_06_028_012 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForAxiBusName_TC_06_028_013 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForAxiBusName_TC_06_028_014 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForDeviceHdl_TC_06_029_001 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForDeviceHdl_TC_06_029_002 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForDeviceHdl_TC_06_029_003 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForDeviceHdl_TC_06_029_004 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForDeviceHdl_TC_06_029_005 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForDeviceHdl_TC_06_029_006 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForDeviceHdl_TC_06_029_007 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForDeviceHdl_TC_06_029_008 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForDeviceHdl_TC_06_029_009 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForDeviceHdl_TC_06_029_010 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForDeviceHdl_TC_06_029_011 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForDeviceHdl_TC_06_029_012 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForDeviceName_TC_06_030_001 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForDeviceName_TC_06_030_002 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForDeviceName_TC_06_030_003 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForDeviceName_TC_06_030_004 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForDeviceName_TC_06_030_005 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForDeviceName_TC_06_030_006 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForDeviceName_TC_06_030_007 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForDeviceName_TC_06_030_008 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForDeviceName_TC_06_030_009 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForDeviceName_TC_06_030_010 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForDeviceName_TC_06_030_011 @endlink
 * @test @link IoAXIBusNameApiTest_IoGetAxiIdListForDeviceName_TC_06_030_012 @endlink
 * @section IoAXIBusNameApiTest IoAXIBusNameApiTest function
 * @subsection IoAXIBusNameApiTest_SetUp IoAXIBusNameApiTest Setup
 * Common to Test Suit of IoAXIBusNameApiTest, prepare process to be executed before executing the test. <br>
 * For example:
 * @snippet test_io_axi_bus_name.cpp IoAXIBusNameApiTest_SetUp
 * @subsection IoAXIBusNameApiTest_TearDown IoAXIBusNameApiTest TearDown
 * Common to Test Suit of IoAXIBusNameApiTest, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_io_axi_bus_name.cpp IoAXIBusNameApiTest_TearDown
 */
class IoAXIBusNameApiTest : public PARENT_TEST_FW_CLASS
{
public:
    osal_device_handle_t erro_device_hndl = OSAL_DEVICE_HANDLE_INVALID;
    osal_device_handle_t test_device_hndl = OSAL_DEVICE_HANDLE_INVALID;
    osal_device_handle_t test_device_hndl_1 = OSAL_DEVICE_HANDLE_INVALID;
    osal_device_handle_t axi_id_without_device_hndl = OSAL_DEVICE_HANDLE_INVALID;

    typedef struct st_osal_axi_bus_name_test_data{
        char *test_bus_name;
        size_t axi_ids;
    } st_osal_axi_bus_name_test_data_t;

    typedef struct st_osal_device_info_axi_bus_test_data{
        char *test_device_name;
        osal_device_handle_t test_hndl;
        size_t axi_ids;
    } st_osal_device_info_axi_bus_test_data_t;

    vector<st_osal_axi_bus_name_test_data_t> axi_bus_name_test_data;
    vector<st_osal_device_info_axi_bus_test_data_t> device_info_test_data;

// [IoAXIBusNameApiTest_SetUp]
    void SetUp() override
    {
      st_osal_axi_bus_name_test_data_t axi_bus_name_test_data_array[] =
      {
#if defined (OSAL_SOC_V3U)
        {"vi0_10-11_14-15", 4},
        {"ir_16-23", 8},
        {"vc0_00-10", 0},
        {"ir_00_02-03_16-23", 0},
        {"ir_15", 0},
        {"ir_16", 1},
        {"ir_16_18_20-22", 5},
#elif defined (OSAL_SOC_V3H1)
        {"vc0_00-06", 7},
        {"ir_00-01", 2},
        {"vc0_00-10", 0},
        {"ir_00_02-03_16-23", 0},
        {"ir_15", 0},
        {"vc0_04", 1},
        {"vc0_00_02_04-06", 5},
#elif defined (OSAL_SOC_V3H2)
        {"vc0_00-06", 7},
        {"ir_00-01", 2},
        {"vc0_00-10", 0},
        {"ir_00_02-03_16-23", 0},
        {"ir_15", 0},
        {"vc0_04", 1},
        {"vc0_00_02_04-06", 5},
#elif defined (OSAL_SOC_V3M2)
#if !defined(TEST_SIL)
        {"vi0_02-04_06-07", 5},
        {"ir_00-01", 2},
        {"vi0_00-10", 0},
        {"ir_00_02-03_16-23", 0},
        {"ir_15", 0},
        {"vi0_04", 1},
        {"vi0_02-03_06-07", 4},
#else
        {"vi0_02-03", 2}, // V3M2 Simulation only support ims/imr and imp device type
        {"ir_00-01", 2},
        {"vi0_00-10", 0},
        {"ir_00_02-03_16-23", 0},
        {"ir_15", 0},
        {"vi0_02", 1},
        {"vi0_03", 1},
#endif
#elif defined (OSAL_SOC_V4H)
        {"vi0_10-11_14-15_18-19_22-23_26-27", 10}, // isp
        {"vc_00-02_04-06_08-09", 8}, // imr + ims
        {"vi0_00-10", 0}, // axi_bus_name valid value of vi0 + vi0 invalid value
        {"vi0_18-20", 0}, // axi_bus_name valid value of isp + vi0 invalid value 
        {"vc_00-03", 0}, // axi_bus_name valid value of imr + vc invalid value
        {"vi0_10", 1}, // part of axi_bus_name of isp
        {"vi0_10_14_19_23", 4}, // part of axi_bus_name of isp_0 and isp1
#elif defined (OSAL_SOC_V4H2)
        {"vi0_10-11_14-15_18-19_22-23_26-27", 10}, // isp
        {"vc_00-02_04-06_08-09", 8}, // imr + ims
        {"vi0_00-10", 0}, // axi_bus_name valid value of vi0 + vi0 invalid value
        {"vi0_18-20", 0}, // axi_bus_name valid value of isp + vi0 invalid value 
        {"vc_00-03", 0}, // axi_bus_name valid value of imr + vc invalid value
        {"vi0_10", 1}, // part of axi_bus_name of isp
        {"vi0_10_14_19_23", 4}, // part of axi_bus_name of isp_0 and isp1
#elif defined (OSAL_SOC_V4M)
        {"vi0_10_14_18_22_26", 5}, // isp
        {"vc_00-01_04-05_08-09", 6}, // imr + ims
        {"vi0_00-10", 0}, // axi_bus_name valid value of vi0 + vi0 invalid value
        {"vi0_18-20", 0}, // axi_bus_name valid value of isp + vi0 invalid value 
        {"vc_00-03", 0}, // axi_bus_name valid value of imr + vc invalid value
        {"vi0_10", 1}, // part of axi_bus_name of isp
        {"vi0_10_14_18_26", 4}, // part of axi_bus_name of isp_0 and isp1
#elif defined (OSAL_SOC_S4)
        {"rt1_00-15", 16}, // rt_dmac_control_00
        {"rt1_16-31", 16}, // rt_dmac_control_01
        {"rt1_00-15_97", 0}, // rt_dmac_control_00 + invalid value
        {"rt1_00-15_98", 0}, // rt_dmac_control_00 + invalid value
        {"rt1_00-15_99", 0}, // rt_dmac_control_00 + invalid value
        {"rt1_00-12", 13},  // part of axi_bus_name of rt_dmac_control_00
        {"rt1_00-12_16-20_25-31", 25},  // part of axi_bus_name of rt_dmac_control_00 + rt_dmac_control_01
#endif
      };

      st_osal_device_info_axi_bus_test_data_t device_info_test_data_array[] =
      {
#if defined (OSAL_SOC_V3U)
        {TEST_AXI_BUS_NAME_DEVICE_VALID, test_device_hndl, TEST_DEVICE_AXI_ID_VALID},
        {TEST_AXI_BUS_NAME_DEVICE_INVALID, axi_id_without_device_hndl, TEST_DEVICE_AXI_ID_INVALID},
        {UNKNOW_DEVICE_TYPE, erro_device_hndl, TEST_DEVICE_AXI_ID_INVALID},
        {TEST_AXI_BUS_NAME_DEVICE_VALID_1, test_device_hndl_1, TEST_DEVICE_AXI_ID_VALID_1}
#elif defined (OSAL_SOC_V3H1)
        {TEST_AXI_BUS_NAME_DEVICE_VALID, test_device_hndl, TEST_DEVICE_AXI_ID_VALID},
        {TEST_AXI_BUS_NAME_DEVICE_INVALID, axi_id_without_device_hndl, TEST_DEVICE_AXI_ID_INVALID},
        {UNKNOW_DEVICE_TYPE, erro_device_hndl, TEST_DEVICE_AXI_ID_INVALID},
        {TEST_AXI_BUS_NAME_DEVICE_VALID_1, test_device_hndl_1, TEST_DEVICE_AXI_ID_VALID_1}
#elif defined (OSAL_SOC_V3H2)
        {TEST_AXI_BUS_NAME_DEVICE_VALID, test_device_hndl, TEST_DEVICE_AXI_ID_VALID},
        {TEST_AXI_BUS_NAME_DEVICE_INVALID, axi_id_without_device_hndl, TEST_DEVICE_AXI_ID_INVALID},
        {UNKNOW_DEVICE_TYPE, erro_device_hndl, TEST_DEVICE_AXI_ID_INVALID},
        {TEST_AXI_BUS_NAME_DEVICE_VALID_1, test_device_hndl_1, TEST_DEVICE_AXI_ID_VALID_1}
#elif defined (OSAL_SOC_V3M2)
        {TEST_AXI_BUS_NAME_DEVICE_VALID, test_device_hndl, TEST_DEVICE_AXI_ID_VALID},
        {TEST_AXI_BUS_NAME_DEVICE_INVALID, axi_id_without_device_hndl, TEST_DEVICE_AXI_ID_INVALID},
        {UNKNOW_DEVICE_TYPE, erro_device_hndl, TEST_DEVICE_AXI_ID_INVALID},
        {TEST_AXI_BUS_NAME_DEVICE_VALID_1, test_device_hndl_1, TEST_DEVICE_AXI_ID_VALID_1}
#elif defined (OSAL_SOC_V4H)
        {TEST_AXI_BUS_NAME_DEVICE_VALID, test_device_hndl, TEST_DEVICE_AXI_ID_VALID},
        {TEST_AXI_BUS_NAME_DEVICE_INVALID, axi_id_without_device_hndl, TEST_DEVICE_AXI_ID_INVALID},
        {UNKNOW_DEVICE_TYPE, erro_device_hndl, TEST_DEVICE_AXI_ID_INVALID},
        {TEST_AXI_BUS_NAME_DEVICE_VALID_1, test_device_hndl_1, TEST_DEVICE_AXI_ID_VALID_1}
#elif defined (OSAL_SOC_V4H2)
        {TEST_AXI_BUS_NAME_DEVICE_VALID, test_device_hndl, TEST_DEVICE_AXI_ID_VALID},
        {TEST_AXI_BUS_NAME_DEVICE_INVALID, axi_id_without_device_hndl, TEST_DEVICE_AXI_ID_INVALID},
        {UNKNOW_DEVICE_TYPE, erro_device_hndl, TEST_DEVICE_AXI_ID_INVALID},
        {TEST_AXI_BUS_NAME_DEVICE_VALID_1, test_device_hndl_1, TEST_DEVICE_AXI_ID_VALID_1}
#elif defined (OSAL_SOC_V4M)
        {TEST_AXI_BUS_NAME_DEVICE_VALID, test_device_hndl, TEST_DEVICE_AXI_ID_VALID},
        {TEST_AXI_BUS_NAME_DEVICE_INVALID, axi_id_without_device_hndl, TEST_DEVICE_AXI_ID_INVALID},
        {UNKNOW_DEVICE_TYPE, erro_device_hndl, TEST_DEVICE_AXI_ID_INVALID},
        {TEST_AXI_BUS_NAME_DEVICE_VALID_1, test_device_hndl_1, TEST_DEVICE_AXI_ID_VALID_1}
#elif defined (OSAL_SOC_S4)
        {TEST_AXI_BUS_NAME_DEVICE_VALID, test_device_hndl, TEST_DEVICE_AXI_ID_VALID},
        {TEST_AXI_BUS_NAME_DEVICE_INVALID, axi_id_without_device_hndl, TEST_DEVICE_AXI_ID_INVALID},
        {UNKNOW_DEVICE_TYPE, erro_device_hndl, TEST_DEVICE_AXI_ID_INVALID},
        {TEST_AXI_BUS_NAME_DEVICE_VALID_1, test_device_hndl_1, TEST_DEVICE_AXI_ID_VALID_1}
#endif
      };

      for(auto i = 0; i < sizeof(axi_bus_name_test_data_array)/sizeof(axi_bus_name_test_data_array[0]); i++)
      {
        axi_bus_name_test_data.push_back(axi_bus_name_test_data_array[i]);
      }

      for(auto i = 0; i < sizeof(device_info_test_data_array)/sizeof(device_info_test_data_array[0]); i++)
      {
        device_info_test_data.push_back(device_info_test_data_array[i]);
      }

      EXPECT_EQ(R_OSAL_IoDeviceOpen(device_info_test_data[0].test_device_name, &device_info_test_data[0].test_hndl), OSAL_RETURN_OK);
      EXPECT_EQ(R_OSAL_IoDeviceOpen(device_info_test_data[1].test_device_name, &device_info_test_data[1].test_hndl), OSAL_RETURN_OK);
      EXPECT_EQ(R_OSAL_IoDeviceOpen(device_info_test_data[3].test_device_name, &device_info_test_data[3].test_hndl), OSAL_RETURN_OK);
    }
// [IoAXIBusNameApiTest_SetUp]

// [IoAXIBusNameApiTest_TearDown]
    void TearDown() override
    {
      EXPECT_EQ(R_OSAL_IoDeviceClose(device_info_test_data[0].test_hndl), OSAL_RETURN_OK);
      EXPECT_EQ(R_OSAL_IoDeviceClose(device_info_test_data[1].test_hndl), OSAL_RETURN_OK);
      EXPECT_EQ(R_OSAL_IoDeviceClose(device_info_test_data[3].test_hndl), OSAL_RETURN_OK);
    }
// [IoAXIBusNameApiTest_TearDown]
};

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetNumOfAxiIdsForAxiBusName_TC_06_025_001 IoAXIBusNameApiTest.IoGetNumOfAxiIdsForAxiBusName_TC_06_025_001
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_001_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_001_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S1_N_TC_06_025_001
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_001_rr Related Requirement(s)
 * \#326472
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_001_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetNumOfAxiIdsForAxiBusName function
 * - Test06_025_001: Get the number of AXI IDs for the given valid AIX Bus Name
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_001_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs for the given valid AIX Bus Name "axi_bus_name_test_data[0].test_bus_name" by using R_OSAL_IoGetNumOfAxiIdsForAxiBusName
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_001_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForAxiBusName return OSAL_RETURN_OK.<br>
 * The value of "num_of_axi_ids" is equal to the expected value
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_001_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetNumOfAxiIdsForAxiBusName_TC_06_025_001
 */
// [IoGetNumOfAxiIdsForAxiBusName_TC_06_025_001]
TEST_F(IoAXIBusNameApiTest, IoGetNumOfAxiIdsForAxiBusName_TC_06_025_001)
{
  size_t num_of_axi_ids = 0;

  // test: valid axi_bus_name
  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForAxiBusName(axi_bus_name_test_data[0].test_bus_name, &num_of_axi_ids), OSAL_RETURN_OK) << "Test06_025_001 failed";
  EXPECT_EQ(axi_bus_name_test_data[0].axi_ids, num_of_axi_ids);
}
// [IoGetNumOfAxiIdsForAxiBusName_TC_06_025_001]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetNumOfAxiIdsForAxiBusName_TC_06_025_002 IoAXIBusNameApiTest.IoGetNumOfAxiIdsForAxiBusName_TC_06_025_002
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_002_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_002_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S1_N_TC_06_025_002
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_002_rr Related Requirement(s)
 * \#326472
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_002_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetNumOfAxiIdsForAxiBusName function
 * - Test06_025_002: Get the number of AXI IDs for the given valid AIX Bus Name
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_002_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs for the given valid AIX Bus Name "axi_bus_name_test_data[1].test_bus_name" by using R_OSAL_IoGetNumOfAxiIdsForAxiBusName<br>
 * different bus domain name to 06_025_001
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_002_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForAxiBusName return OSAL_RETURN_OK.<br>
 * The value of "num_of_axi_ids" is equal to the expected value
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_002_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetNumOfAxiIdsForAxiBusName_TC_06_025_002
 */
// [IoGetNumOfAxiIdsForAxiBusName_TC_06_025_002]
TEST_F(IoAXIBusNameApiTest, IoGetNumOfAxiIdsForAxiBusName_TC_06_025_002)
{
  size_t num_of_axi_ids = 0;

  // test: valid axi_bus_name
  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForAxiBusName(axi_bus_name_test_data[1].test_bus_name, &num_of_axi_ids), OSAL_RETURN_OK) << "Test06_025_002 failed";
  EXPECT_EQ(axi_bus_name_test_data[1].axi_ids, num_of_axi_ids);
}
// [IoGetNumOfAxiIdsForAxiBusName_TC_06_025_002]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetNumOfAxiIdsForAxiBusName_TC_06_025_003 IoAXIBusNameApiTest.IoGetNumOfAxiIdsForAxiBusName_TC_06_025_003
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_003_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_003_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_025_003
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_003_rr Related Requirement(s)
 * \#326472
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_003_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetNumOfAxiIdsForAxiBusName
 * - Test06_025_003: Get the number of AXI IDs with p_num_of_axi_ids is NULL
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_003_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs with p_num_of_axi_ids is NULL by using R_OSAL_IoGetNumOfAxiIdsForAxiBusName
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_003_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForAxiBusName return OSAL_RETURN_PAR
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_003_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetNumOfAxiIdsForAxiBusName_TC_06_025_003
 */
// [IoGetNumOfAxiIdsForAxiBusName_TC_06_025_003]
TEST_F(IoAXIBusNameApiTest, IoGetNumOfAxiIdsForAxiBusName_TC_06_025_003)
{
  size_t num_of_axi_ids = 0;

  // test: parameter is invalid
  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForAxiBusName(axi_bus_name_test_data[0].test_bus_name, NULL), OSAL_RETURN_PAR) << "Test06_025_003 failed";
}
// [IoGetNumOfAxiIdsForAxiBusName_TC_06_025_003]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetNumOfAxiIdsForAxiBusName_TC_06_025_004 IoAXIBusNameApiTest.IoGetNumOfAxiIdsForAxiBusName_TC_06_025_004
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_004_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_004_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_025_004
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_004_rr Related Requirement(s)
 * \#326472
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_004_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetNumOfAxiIdsForAxiBusName function
 * - Test06_025_004: Get the number of AXI IDs with p_num_of_axi_ids is NULL
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_004_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs with p_num_of_axi_ids is NULL by using R_OSAL_IoGetNumOfAxiIdsForAxiBusName
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_004_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForAxiBusName return OSAL_RETURN_PAR
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_004_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetNumOfAxiIdsForAxiBusName_TC_06_025_004
 */
// [IoGetNumOfAxiIdsForAxiBusName_TC_06_025_004]
TEST_F(IoAXIBusNameApiTest, IoGetNumOfAxiIdsForAxiBusName_TC_06_025_004)
{
  size_t num_of_axi_ids = 0;

  // test: parameter is invalid
  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForAxiBusName(NULL, &num_of_axi_ids), OSAL_RETURN_PAR) << "Test06_025_004 failed";
}
// [IoGetNumOfAxiIdsForAxiBusName_TC_06_025_004]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetNumOfAxiIdsForAxiBusName_TC_06_025_005 IoAXIBusNameApiTest.IoGetNumOfAxiIdsForAxiBusName_TC_06_025_005
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_005_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_005_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_025_005
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_005_rr Related Requirement(s)
 * \#326472
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_005_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetNumOfAxiIdsForAxiBusName function
 * - Test06_025_005: Get the number of AXI IDs with axi_bus_name has axi_id that is not retained
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_005_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs with axi_bus_name "axi_bus_name_test_data[3].test_bus_name" has axi_id that is not retained by using R_OSAL_IoGetNumOfAxiIdsForAxiBusName
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForAxiBusName return OSAL_RETURN_UNKNOWN_NAME
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_005_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetNumOfAxiIdsForAxiBusName_TC_06_025_005
 */
// [IoGetNumOfAxiIdsForAxiBusName_TC_06_025_005]
TEST_F(IoAXIBusNameApiTest, IoGetNumOfAxiIdsForAxiBusName_TC_06_025_005)
{
  size_t num_of_axi_ids = 0;

  // test: axi_bus_name has axi_id that is not retained
  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForAxiBusName(axi_bus_name_test_data[3].test_bus_name, &num_of_axi_ids), OSAL_RETURN_UNKNOWN_NAME) << "Test06_025_005 failed";
}
// [IoGetNumOfAxiIdsForAxiBusName_TC_06_025_005]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetNumOfAxiIdsForAxiBusName_TC_06_025_006 IoAXIBusNameApiTest.IoGetNumOfAxiIdsForAxiBusName_TC_06_025_006
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_006_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_006_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_025_006
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_006_rr Related Requirement(s)
 * \#326472
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_006_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_006_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetNumOfAxiIdsForAxiBusName function
 * - Test06_025_006: Get the number of AXI IDs with axi_bus_name has axi_id that is not retained
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_006_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs with axi_bus_name "axi_bus_name_test_data[4].test_bus_name" has axi_id that is not retained by using R_OSAL_IoGetNumOfAxiIdsForAxiBusName
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_006_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForAxiBusName return OSAL_RETURN_UNKNOWN_NAME
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_006_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetNumOfAxiIdsForAxiBusName_TC_06_025_006
 */
// [IoGetNumOfAxiIdsForAxiBusName_TC_06_025_006]
TEST_F(IoAXIBusNameApiTest, IoGetNumOfAxiIdsForAxiBusName_TC_06_025_006)
{
  size_t num_of_axi_ids = 0;

  // test: axi_bus_name has axi_id that is not retained
  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForAxiBusName(axi_bus_name_test_data[4].test_bus_name, &num_of_axi_ids), OSAL_RETURN_UNKNOWN_NAME) << "Test006_025_006 failed";
}
// [IoGetNumOfAxiIdsForAxiBusName_TC_06_025_006]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetNumOfAxiIdsForAxiBusName_TC_06_025_007 IoAXIBusNameApiTest.IoGetNumOfAxiIdsForAxiBusName_TC_06_025_007
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_007_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_007_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S2_A_TC_06_025_007
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_007_rr Related Requirement(s)
 * \#326472
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_007_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_007_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetNumOfAxiIdsForAxiBusName function
 * - Test06_025_007: Confirm state transition of R_OSAL_IoGetNumOfAxiIdsForAxiBusName
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_007_tpts Test Procedure / Test Steps
 * -# Close opened devices by using R_OSAL_IoDeviceClose
 * -# Deinit OSAL APIs by using R_OSAL_Deinitialize
 * -# Call R_OSAL_IoGetNumOfAxiIdsForAxiBusName
 * -# Init OSAL APIs by using R_OSAL_Initialize
 * -# Open again required devices by using R_OSAL_IoDeviceOpen
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_007_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_Deinitialize return OSAL_RETURN_OK
 * -# R_OSAL_IoGetNumOfAxiIdsForAxiBusName return OSAL_RETURN_STATE
 * -# R_OSAL_Initialize return OSAL_RETURN_OK
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_007_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetNumOfAxiIdsForAxiBusName_TC_06_025_007
 */
// [IoGetNumOfAxiIdsForAxiBusName_TC_06_025_007]
TEST_F(IoAXIBusNameApiTest, IoGetNumOfAxiIdsForAxiBusName_TC_06_025_007)
{
  size_t num_of_axi_ids = 0;

  // pre-process of testing
  EXPECT_EQ(R_OSAL_IoDeviceClose(device_info_test_data[0].test_hndl), OSAL_RETURN_OK);
  EXPECT_EQ(R_OSAL_IoDeviceClose(device_info_test_data[1].test_hndl), OSAL_RETURN_OK);
  EXPECT_EQ(R_OSAL_IoDeviceClose(device_info_test_data[3].test_hndl), OSAL_RETURN_OK);
  EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_OK) << "pre-process failed";

  // test: Don't execute initialize
  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForAxiBusName(axi_bus_name_test_data[0].test_bus_name, &num_of_axi_ids), OSAL_RETURN_STATE) << "Test06_025_007 failed";

  // post-process of testing
  EXPECT_EQ(R_OSAL_Initialize(), OSAL_RETURN_OK) << "post-process failed";
  EXPECT_EQ(R_OSAL_IoDeviceOpen(device_info_test_data[0].test_device_name, &device_info_test_data[0].test_hndl), OSAL_RETURN_OK);
  EXPECT_EQ(R_OSAL_IoDeviceOpen(device_info_test_data[1].test_device_name, &device_info_test_data[1].test_hndl), OSAL_RETURN_OK);
  EXPECT_EQ(R_OSAL_IoDeviceOpen(device_info_test_data[3].test_device_name, &device_info_test_data[3].test_hndl), OSAL_RETURN_OK);
}
// [IoGetNumOfAxiIdsForAxiBusName_TC_06_025_007]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetNumOfAxiIdsForAxiBusName_TC_06_025_008 IoAXIBusNameApiTest.IoGetNumOfAxiIdsForAxiBusName_TC_06_025_008
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_008_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_008_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S1_N_TC_06_025_008
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_008_rr Related Requirement(s)
 * \#326472
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_008_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_008_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetNumOfAxiIdsForAxiBusName function
 * - Test06_025_008: Get the number of AXI IDs for the given valid AIX Bus Name
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_008_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs for the given valid AIX Bus Name "axi_bus_name_test_data[5].test_bus_name" by using R_OSAL_IoGetNumOfAxiIdsForAxiBusName<br>
 * The axi_bus_name that specify contains all uTLBs for the bus domain
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_008_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForAxiBusName return OSAL_RETURN_OK.<br>
 * The value of "num_of_axi_ids" is equal to the expected value
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_008_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetNumOfAxiIdsForAxiBusName_TC_06_025_008
 */
// [IoGetNumOfAxiIdsForAxiBusName_TC_06_025_008]
TEST_F(IoAXIBusNameApiTest, IoGetNumOfAxiIdsForAxiBusName_TC_06_025_008)
{
  size_t num_of_axi_ids = 0;

  // test: valid axi_bus_name
  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForAxiBusName(axi_bus_name_test_data[5].test_bus_name, &num_of_axi_ids), OSAL_RETURN_OK) << "Test06_025_008 failed";
  EXPECT_EQ(axi_bus_name_test_data[5].axi_ids, num_of_axi_ids);
}
// [IoGetNumOfAxiIdsForAxiBusName_TC_06_025_008]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetNumOfAxiIdsForAxiBusName_TC_06_025_009 IoAXIBusNameApiTest.IoGetNumOfAxiIdsForAxiBusName_TC_06_025_009
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_009_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_009_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S1_N_TC_06_025_009
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_009_rr Related Requirement(s)
 * \#326472
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_009_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_009_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetNumOfAxiIdsForAxiBusName function
 * - Test06_025_009: Get the number of AXI IDs for the given valid AIX Bus Name
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_009_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs for the given valid AIX Bus Name "axi_bus_name_test_data[6].test_bus_name" by using R_OSAL_IoGetNumOfAxiIdsForAxiBusName<br>
 * The axi_bus_name that specify contains partial uTLBs for the bus domain
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_009_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForAxiBusName return OSAL_RETURN_OK.<br>
 * The value of "num_of_axi_ids" is equal to the expected value
 * @subsection IoGetNumOfAxiIdsForAxiBusName_TC_06_025_009_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetNumOfAxiIdsForAxiBusName_TC_06_025_009
 */
// [IoGetNumOfAxiIdsForAxiBusName_TC_06_025_009]
TEST_F(IoAXIBusNameApiTest, IoGetNumOfAxiIdsForAxiBusName_TC_06_025_009)
{
  size_t num_of_axi_ids = 0;

  // test: valid axi_bus_name
  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForAxiBusName(axi_bus_name_test_data[6].test_bus_name, &num_of_axi_ids), OSAL_RETURN_OK) << "Test06_025_009 failed";
  EXPECT_EQ(axi_bus_name_test_data[6].axi_ids, num_of_axi_ids);
}
// [IoGetNumOfAxiIdsForAxiBusName_TC_06_025_009]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_001 IoAXIBusNameApiTest.IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_001
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_001_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_001_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S1_N_TC_06_026_001
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_001_rr Related Requirement(s)
 * \#329827
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Device handle was opened
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_001_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetNumOfAxiIdsForDeviceHdl function
 * - Test06_026_001: Get the number of AXI IDs for the given valid device handle
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_001_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs for the given valid device handle "device_info_test_data[0].test_hndl" by using R_OSAL_IoGetNumOfAxiIdsForDeviceHdl
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_001_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForDeviceHdl return OSAL_RETURN_OK.<br>
 * The value of "num_of_axi_ids" is equal to the expected value
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_001_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_001
 */
// [IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_001]
TEST_F(IoAXIBusNameApiTest, IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_001)
{
  size_t num_of_axi_ids = 0;

  // test: valid device handle
  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForDeviceHdl(device_info_test_data[0].test_hndl, &num_of_axi_ids), OSAL_RETURN_OK) << "Test06_026_001 failed";
  EXPECT_EQ(device_info_test_data[0].axi_ids, num_of_axi_ids);
}
// [IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_001]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_002 IoAXIBusNameApiTest.IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_002
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_002_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_002_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_026_002
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_002_rr Related Requirement(s)
 * \#329827
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Device handle was opened
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_002_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetNumOfAxiIdsForDeviceHdl function
 * - Test06_026_002: Get the number of AXI IDs with p_num_of_axi_ids is NULL
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_002_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs with p_num_of_axi_ids is NULL by using R_OSAL_IoGetNumOfAxiIdsForDeviceHdl
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_002_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForDeviceHdl return OSAL_RETURN_PAR
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_002_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_002
 */
// [IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_002]
TEST_F(IoAXIBusNameApiTest, IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_002)
{
  // test: invalid parmeter
  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForDeviceHdl(device_info_test_data[0].test_hndl, NULL), OSAL_RETURN_PAR) << "Test06_026_002 failed";
}
// [IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_002]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_003 IoAXIBusNameApiTest.IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_003
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_003_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_003_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_026_003
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_003_rr Related Requirement(s)
 * \#329827
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_003_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetNumOfAxiIdsForDeviceHdl function
 * - Test06_026_003: Get the number of AXI IDs with invalid device handle
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_003_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs with invalid device handle by using R_OSAL_IoGetNumOfAxiIdsForDeviceHdl
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_003_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForDeviceHdl return OSAL_RETURN_HANDLE
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_003_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_003
 */
// [IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_003]
TEST_F(IoAXIBusNameApiTest, IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_003)
{
  size_t num_of_axi_ids = 0;

  // test: invalid device handle
  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForDeviceHdl(device_info_test_data[2].test_hndl, &num_of_axi_ids), OSAL_RETURN_HANDLE) << "Test06_026_003 failed";
}
// [IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_003]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_004 IoAXIBusNameApiTest.IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_004
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_004_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_004_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S1_N_TC_06_026_004
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_004_rr Related Requirement(s)
 * \#329827
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Device handle was opened
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_004_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetNumOfAxiIdsForDeviceHdl function
 * - Test06_026_004: Get the number of AXI IDs with valid device handle doesn'thave axi_id
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_004_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs with valid device handle doesn'thave axi_id by using R_OSAL_IoGetNumOfAxiIdsForDeviceHdl
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_004_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForDeviceHdl return OSAL_RETURN_OK
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_004_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_004
 */
// [IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_004]
TEST_F(IoAXIBusNameApiTest, IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_004)
{
  size_t num_of_axi_ids = 0;

  // test: valid device handle doesn't have axi_id
  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForDeviceHdl(device_info_test_data[1].test_hndl, &num_of_axi_ids), OSAL_RETURN_OK) << "Test06_026_004 failed";
}
// [IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_004]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_005 IoAXIBusNameApiTest.IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_005
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_005_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_005_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S2_A_TC_06_026_005
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_005_rr Related Requirement(s)
 * \#329827
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Device handle was opened
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_005_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetNumOfAxiIdsForDeviceHdl function
 * - Test06_026_005: Confirm state transition of R_OSAL_IoGetNumOfAxiIdsForDeviceHdl
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_005_tpts Test Procedure / Test Steps
 * -# Close opened devices by using R_OSAL_IoDeviceClose
 * -# Deinit OSAL APIs by using R_OSAL_Deinitialize
 * -# Call R_OSAL_IoGetNumOfAxiIdsForDeviceHdl
 * -# Init OSAL APIs by using R_OSAL_Initialize
 * -# Open required devices again by using R_OSAL_IoDeviceOpen
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_Deinitialize return OSAL_RETURN_OK
 * -# R_OSAL_IoGetNumOfAxiIdsForDeviceHdl return OSAL_RETURN_STATE
 * -# R_OSAL_Initialize return OSAL_RETURN_OK
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_005_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_005
 */
// [IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_005]
TEST_F(IoAXIBusNameApiTest, IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_005)
{
  size_t num_of_axi_ids = 0;

  // pre-process of testing
  EXPECT_EQ(R_OSAL_IoDeviceClose(device_info_test_data[0].test_hndl), OSAL_RETURN_OK);
  EXPECT_EQ(R_OSAL_IoDeviceClose(device_info_test_data[1].test_hndl), OSAL_RETURN_OK);
  EXPECT_EQ(R_OSAL_IoDeviceClose(device_info_test_data[3].test_hndl), OSAL_RETURN_OK);
  EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_OK) << "pre-process failed";
  
  // test: Don't execute initialize
  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForDeviceHdl(device_info_test_data[0].test_hndl, &num_of_axi_ids), OSAL_RETURN_STATE) << "Test06_026_005 failed";

  // post-process of testing
  EXPECT_EQ(R_OSAL_Initialize(), OSAL_RETURN_OK) << "post-process failed";
  EXPECT_EQ(R_OSAL_IoDeviceOpen(device_info_test_data[0].test_device_name, &device_info_test_data[0].test_hndl), OSAL_RETURN_OK);
  EXPECT_EQ(R_OSAL_IoDeviceOpen(device_info_test_data[1].test_device_name, &device_info_test_data[1].test_hndl), OSAL_RETURN_OK);
  EXPECT_EQ(R_OSAL_IoDeviceOpen(device_info_test_data[3].test_device_name, &device_info_test_data[3].test_hndl), OSAL_RETURN_OK);
}
// [IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_005]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_006 IoAXIBusNameApiTest.IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_006
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_006_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_006_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S2_A_TC_06_026_006
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_006_rr Related Requirement(s)
 * \#329827
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_006_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_006_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetNumOfAxiIdsForDeviceHdl function
 * - Test06_026_006: Get the number of AXI IDs with the device handle was already closed
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_006_tpts Test Procedure / Test Steps
 * -# Close device handle by using R_OSAL_IoDeviceClose
 * -# Get the number of AXI IDs with the device handle was already closed by using R_OSAL_IoGetNumOfAxiIdsForDeviceHdl
 * -# Re-open device handle for the next test case by using R_OSAL_IoDeviceOpen
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_006_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_IoGetNumOfAxiIdsForDeviceHdl return OSAL_RETURN_HANDLE
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK
 * @subsection IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_006_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_006
 */
// [IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_006]
TEST_F(IoAXIBusNameApiTest, IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_006)
{
  size_t num_of_axi_ids = 0;

  // pre-process of testing
  EXPECT_EQ(R_OSAL_IoDeviceClose(device_info_test_data[0].test_hndl), OSAL_RETURN_OK);
  // test: device handle was already closed by R_OSAL_IoDeviceClose
  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForDeviceHdl(device_info_test_data[0].test_hndl, &num_of_axi_ids), OSAL_RETURN_HANDLE) << "Test06_026_006 failed";
  // post-process of testing
  EXPECT_EQ(R_OSAL_IoDeviceOpen(device_info_test_data[0].test_device_name, &device_info_test_data[0].test_hndl), OSAL_RETURN_OK);
}
// [IoGetNumOfAxiIdsForDeviceHdl_TC_06_026_006]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetNumOfAxiIdsForDeviceName_TC_06_027_001 IoAXIBusNameApiTest.IoGetNumOfAxiIdsForDeviceName_TC_06_027_001
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_001_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_001_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S1_N_TC_06_027_001
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_001_rr Related Requirement(s)
 * \#329829
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_001_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetNumOfAxiIdsForDeviceName function
 * - Test06_027_001: Get the number of AXI IDs for the given valid device name
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_001_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs for the given valid device name "device_info_test_data[0].test_device_name" by using R_OSAL_IoGetNumOfAxiIdsForDeviceName
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_001_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForDeviceName return OSAL_RETURN_OK.<br>
 * The value of "num_of_axi_ids" is equal to the expected value
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_001_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetNumOfAxiIdsForDeviceName_TC_06_027_001
 */
// [IoGetNumOfAxiIdsForDeviceName_TC_06_027_001]
TEST_F(IoAXIBusNameApiTest, IoGetNumOfAxiIdsForDeviceName_TC_06_027_001)
{
  size_t num_of_axi_ids = 0;

  // test: valid device name
  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForDeviceName(device_info_test_data[0].test_device_name, &num_of_axi_ids), OSAL_RETURN_OK) << "Test06_027_001 failed";
  EXPECT_EQ(device_info_test_data[0].axi_ids, num_of_axi_ids);
}
// [IoGetNumOfAxiIdsForDeviceName_TC_06_027_001]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetNumOfAxiIdsForDeviceName_TC_06_027_002 IoAXIBusNameApiTest.IoGetNumOfAxiIdsForDeviceName_TC_06_027_002
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_002_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_002_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_027_002
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_002_rr Related Requirement(s)
 * \#329829
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_002_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetNumOfAxiIdsForDeviceName function
 * - Test06_027_002: Get the number of AXI IDs with p_num_of_axi_ids is NULL
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_002_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs with p_num_of_axi_ids is NULL by using R_OSAL_IoGetNumOfAxiIdsForDeviceName
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_002_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForDeviceName return OSAL_RETURN_PAR
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_002_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetNumOfAxiIdsForDeviceName_TC_06_027_002
 */
// [IoGetNumOfAxiIdsForDeviceName_TC_06_027_002]
TEST_F(IoAXIBusNameApiTest, IoGetNumOfAxiIdsForDeviceName_TC_06_027_002)
{
  // test: invalid prameter
  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForDeviceName(axi_bus_name_test_data[0].test_bus_name, NULL), OSAL_RETURN_PAR) << "Test06_027_002 failed";
}
// [IoGetNumOfAxiIdsForDeviceName_TC_06_027_002]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetNumOfAxiIdsForDeviceName_TC_06_027_003 IoAXIBusNameApiTest.IoGetNumOfAxiIdsForDeviceName_TC_06_027_003
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_003_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_003_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_027_003
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_003_rr Related Requirement(s)
 * \#329829
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_003_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetNumOfAxiIdsForDeviceName function
 * - Test06_027_003: Get the number of AXI IDs with p_device_name is NULL
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_003_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs with p_device_name is NULL by using R_OSAL_IoGetNumOfAxiIdsForDeviceName
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_003_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForDeviceName return OSAL_RETURN_PAR
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_003_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetNumOfAxiIdsForDeviceName_TC_06_027_003
 */
// [IoGetNumOfAxiIdsForDeviceName_TC_06_027_003]
TEST_F(IoAXIBusNameApiTest, IoGetNumOfAxiIdsForDeviceName_TC_06_027_003)
{
  size_t num_of_axi_ids = 0;

  // test: invalid prameter
  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForDeviceName(NULL,  &num_of_axi_ids), OSAL_RETURN_PAR) << "Test06_027_003 failed";
}
// [IoGetNumOfAxiIdsForDeviceName_TC_06_027_003]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetNumOfAxiIdsForDeviceName_TC_06_027_004 IoAXIBusNameApiTest.IoGetNumOfAxiIdsForDeviceName_TC_06_027_004
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_004_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_004_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_027_004
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_004_rr Related Requirement(s)
 * \#329829
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_004_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetNumOfAxiIdsForDeviceName function
 * - Test06_027_004: Get the number of AXI IDs with unknow device name
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_004_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs with unknow device name by using R_OSAL_IoGetNumOfAxiIdsForDeviceName
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_004_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForDeviceName return OSAL_RETURN_UNKNOWN_NAME
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_004_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetNumOfAxiIdsForDeviceName_TC_06_027_004
 */
// [IoGetNumOfAxiIdsForDeviceName_TC_06_027_004]
TEST_F(IoAXIBusNameApiTest, IoGetNumOfAxiIdsForDeviceName_TC_06_027_004)
{
  size_t num_of_axi_ids = 0;

  // test: Don't include device name
  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForDeviceName(device_info_test_data[2].test_device_name, &num_of_axi_ids), OSAL_RETURN_UNKNOWN_NAME) << "Test06_027_004 failed";
}
// [IoGetNumOfAxiIdsForDeviceName_TC_06_027_004]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetNumOfAxiIdsForDeviceName_TC_06_027_005 IoAXIBusNameApiTest.IoGetNumOfAxiIdsForDeviceName_TC_06_027_005
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_005_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_005_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S1_N_TC_06_027_005
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_005_rr Related Requirement(s)
 * \#329829
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_005_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetNumOfAxiIdsForDeviceName function
 * - Test06_027_005: Get the number of AXI IDs with valid device name, but device doesn'thave axi_id
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_005_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs with valid device name, but device doesn'thave axi_id by using R_OSAL_IoGetNumOfAxiIdsForDeviceName
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForDeviceName return OSAL_RETURN_OK
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_005_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetNumOfAxiIdsForDeviceName_TC_06_027_005
 */
// [IoGetNumOfAxiIdsForDeviceName_TC_06_027_005]
TEST_F(IoAXIBusNameApiTest, IoGetNumOfAxiIdsForDeviceName_TC_06_027_005)
{
  size_t num_of_axi_ids = 1;

  // test: valid device name, but device doesn'thave axi_id
  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForDeviceName(device_info_test_data[1].test_device_name, &num_of_axi_ids), OSAL_RETURN_OK) << "Test06_027_005 failed";
  EXPECT_EQ(num_of_axi_ids, 0U);
}
// [IoGetNumOfAxiIdsForDeviceName_TC_06_027_005]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetNumOfAxiIdsForDeviceName_TC_06_027_006 IoAXIBusNameApiTest.IoGetNumOfAxiIdsForDeviceName_TC_06_027_006
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_006_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_006_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S2_A_TC_06_027_006
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_006_rr Related Requirement(s)
 * \#329829
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_006_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Device handle was opened
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_006_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetNumOfAxiIdsForDeviceName function
 * - Test06_027_006: Confirm state transition of R_OSAL_IoGetNumOfAxiIdsForDeviceName
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_006_tpts Test Procedure / Test Steps
 * -# Close opened devices by using R_OSAL_IoDeviceClose
 * -# Deinit OSAL APIs by using R_OSAL_Deinitialize
 * -# Call R_OSAL_IoGetNumOfAxiIdsForDeviceName
 * -# Init OSAL APIs by using R_OSAL_Initialize
 * -# Open required devices again by using R_OSAL_IoDeviceOpen
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_006_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_Deinitialize return OSAL_RETURN_OK
 * -# R_OSAL_IoGetNumOfAxiIdsForDeviceName return OSAL_RETURN_STATE
 * -# R_OSAL_Initialize return OSAL_RETURN_OK
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK
 * @subsection IoGetNumOfAxiIdsForDeviceName_TC_06_027_006_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetNumOfAxiIdsForDeviceName_TC_06_027_006
 */
// [IoGetNumOfAxiIdsForDeviceName_TC_06_027_006]
TEST_F(IoAXIBusNameApiTest, IoGetNumOfAxiIdsForDeviceName_TC_06_027_006)
{
  size_t num_of_axi_ids = 0;

  // pre-process of testing
  EXPECT_EQ(R_OSAL_IoDeviceClose(device_info_test_data[0].test_hndl), OSAL_RETURN_OK);
  EXPECT_EQ(R_OSAL_IoDeviceClose(device_info_test_data[1].test_hndl), OSAL_RETURN_OK);
  EXPECT_EQ(R_OSAL_IoDeviceClose(device_info_test_data[3].test_hndl), OSAL_RETURN_OK);
  EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_OK) << "pre-process failed";

  // test: Don't execute initialize
  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForDeviceName(device_info_test_data[0].test_device_name, &num_of_axi_ids), OSAL_RETURN_STATE) << "Test06_027_006 failed";

  // post-process of testing
  EXPECT_EQ(R_OSAL_Initialize(), OSAL_RETURN_OK) << "post-process failed";
  EXPECT_EQ(R_OSAL_IoDeviceOpen(device_info_test_data[0].test_device_name, &device_info_test_data[0].test_hndl), OSAL_RETURN_OK);
  EXPECT_EQ(R_OSAL_IoDeviceOpen(device_info_test_data[1].test_device_name, &device_info_test_data[1].test_hndl), OSAL_RETURN_OK);
  EXPECT_EQ(R_OSAL_IoDeviceOpen(device_info_test_data[3].test_device_name, &device_info_test_data[3].test_hndl), OSAL_RETURN_OK);

}
// [IoGetNumOfAxiIdsForDeviceName_TC_06_027_006]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForAxiBusName_TC_06_028_001 IoAXIBusNameApiTest.IoGetAxiIdListForAxiBusName_TC_06_028_001
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_001_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_001_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S1_N_TC_06_028_001
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_001_rr Related Requirement(s)
 * \#322528 \#326328
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_001_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForAxiBusName function
 * - Test06_028_001: Get the list of AXI IDs for the given valid AXI Bus name
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_001_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs for the given AXI Bus name "axi_bus_name_test_data[0].test_bus_name" by using R_OSAL_IoGetNumOfAxiIdsForAxiBusName
 * -# Allocate buffer which size is equal to "sizeof(osal_axi_bus_id_t) * num_of_max_entry"
 * -# Get the list of AXI Bus IDs related to AXI Bus name "axi_bus_name_test_data[0].test_bus_name" by using R_OSAL_IoGetAxiIdListForAxiBusName
 * -# Free allocated buffer above
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_001_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForAxiBusName return OSAL_RETURN_OK.<br>
 * The value of "num_of_max_entry" is equal to the expected value
 * -# The allocation is successful
 * -# R_OSAL_IoGetAxiIdListForAxiBusName return OSAL_RETURN_OK.<br>
 * The value of "num_of_written_axi_ids" is equal to expected value
 * -# Allocated buffer is freed successfully
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_001_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForAxiBusName_TC_06_028_001
 */
// [IoGetAxiIdListForAxiBusName_TC_06_028_001]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForAxiBusName_TC_06_028_001)
{
  size_t num_of_axi_ids = 0;
  osal_axi_bus_id_t *buffer;
  size_t num_of_max_entry = 0;
  size_t num_of_written_axi_ids = 0;

  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForAxiBusName(axi_bus_name_test_data[0].test_bus_name, &num_of_max_entry), OSAL_RETURN_OK);
  EXPECT_EQ(axi_bus_name_test_data[0].axi_ids, num_of_max_entry);
  buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);

  // test: valid parameters
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForAxiBusName(axi_bus_name_test_data[0].test_bus_name, buffer, num_of_max_entry, &num_of_written_axi_ids), OSAL_RETURN_OK) << "Test06_028_001 failed";
  EXPECT_EQ(axi_bus_name_test_data[0].axi_ids, num_of_written_axi_ids);

  free(buffer);
}
// [IoGetAxiIdListForAxiBusName_TC_06_028_001]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForAxiBusName_TC_06_028_002 IoAXIBusNameApiTest.IoGetAxiIdListForAxiBusName_TC_06_028_002
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_002_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_002_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S1_N_TC_06_028_001
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_002_rr Related Requirement(s)
 * \#322528 \#326328
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_002_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForAxiBusName function
 * - Test06_028_002: Get the list of AXI IDs for the given valid AXI Bus name
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_002_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs for the given AXI Bus name "axi_bus_name_test_data[1].test_bus_name" by using R_OSAL_IoGetNumOfAxiIdsForAxiBusName<br>
 * different bus domain name to 06_028_001
 * -# Allocate buffer which size is equal to "sizeof(osal_axi_bus_id_t) * num_of_max_entry"
 * -# Get the list of AXI Bus IDs related to AXI Bus name "axi_bus_name_test_data[1].test_bus_name" by using R_OSAL_IoGetAxiIdListForAxiBusName
 * -# Free allocated buffer above
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_002_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForAxiBusName return OSAL_RETURN_OK.<br>
 * The value of "num_of_max_entry" is equal to the expected value
 * -# The allocation is successful
 * -# R_OSAL_IoGetAxiIdListForAxiBusName return OSAL_RETURN_OK.<br>
 * The value of "num_of_written_axi_ids" is equal to expected value
 * -# Allocated buffer is freeed successfully
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_002_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForAxiBusName_TC_06_028_002
 */
// [IoGetAxiIdListForAxiBusName_TC_06_028_002]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForAxiBusName_TC_06_028_002)
{
  size_t num_of_axi_ids = 0;
  osal_axi_bus_id_t *buffer;
  size_t num_of_max_entry = 0;
  size_t num_of_written_axi_ids = 0;

  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForAxiBusName(axi_bus_name_test_data[1].test_bus_name, &num_of_max_entry), OSAL_RETURN_OK);
  EXPECT_EQ(axi_bus_name_test_data[1].axi_ids, num_of_max_entry);

  buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);

  // test: valid parameters
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForAxiBusName(axi_bus_name_test_data[1].test_bus_name, buffer, num_of_max_entry, &num_of_written_axi_ids), OSAL_RETURN_OK) << "Test06_028_002 failed";
  EXPECT_EQ(axi_bus_name_test_data[1].axi_ids, num_of_written_axi_ids);

  free(buffer);
}
// [IoGetAxiIdListForAxiBusName_TC_06_028_002]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForAxiBusName_TC_06_028_003 IoAXIBusNameApiTest.IoGetAxiIdListForAxiBusName_TC_06_028_003
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_003_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_003_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_028_003
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_003_rr Related Requirement(s)
 * \#322528 \#326328
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_003_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForAxiBusName function
 * - Test06_028_003: Get the list of AXI IDs with p_axi_bus_name is NULL
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_003_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs for the given AXI Bus name "axi_bus_name_test_data[0].test_bus_name" by using R_OSAL_IoGetNumOfAxiIdsForAxiBusName
 * -# Allocate buffer which size is equal to "sizeof(osal_axi_bus_id_t) * num_of_max_entry"
 * -# Get the list of AXI Bus IDs with p_axi_bus_name is NULL by using R_OSAL_IoGetAxiIdListForAxiBusName
 * -# Free allocated buffer above
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_003_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForAxiBusName return OSAL_RETURN_OK.<br>
 * The value of "num_of_max_entry" is equal to the expected value
 * -# The allocation is successful
 * -# R_OSAL_IoGetAxiIdListForAxiBusName return OSAL_RETURN_PAR.
 * -# Allocated buffer is freeed successfully
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_003_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForAxiBusName_TC_06_028_003
 */
// [IoGetAxiIdListForAxiBusName_TC_06_028_003]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForAxiBusName_TC_06_028_003)
{
  size_t num_of_axi_ids = 0;
  osal_axi_bus_id_t *buffer;
  size_t num_of_max_entry = 0;
  size_t num_of_written_axi_ids = 0;

  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForAxiBusName(axi_bus_name_test_data[1].test_bus_name, &num_of_max_entry), OSAL_RETURN_OK);
  EXPECT_EQ(axi_bus_name_test_data[1].axi_ids, num_of_max_entry);
  buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);

  // test: invalid parameter
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForAxiBusName(NULL, buffer, num_of_max_entry, &num_of_written_axi_ids), OSAL_RETURN_PAR) << "Test06_028_003 failed";

  free(buffer);
}
// [IoGetAxiIdListForAxiBusName_TC_06_028_003]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForAxiBusName_TC_06_028_004 IoAXIBusNameApiTest.IoGetAxiIdListForAxiBusName_TC_06_028_004
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_004_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_004_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_028_004
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_004_rr Related Requirement(s)
 * \#322528 \#326328
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_004_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForAxiBusName function
 * - Test06_028_004: Get the list of AXI IDs with buffer size is less than the required buffer size
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_004_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs for the given AXI Bus name "axi_bus_name_test_data[0].test_bus_name" by using R_OSAL_IoGetNumOfAxiIdsForAxiBusName
 * -# Allocate buffer which size is equal to "sizeof(osal_axi_bus_id_t) * num_of_max_entry"
 * -# Get the list of AXI Bus IDs by using R_OSAL_IoGetAxiIdListForAxiBusName with buffer size is "num_of_max_entry - 1"
 * -# Free allocated buffer above
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_004_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForAxiBusName return OSAL_RETURN_OK.<br>
 * The value of "num_of_max_entry" is equal to the expected value
 * -# The allocation is successful
 * -# R_OSAL_IoGetAxiIdListForAxiBusName return OSAL_RETURN_PAR.
 * -# Allocated buffer is freeed successfully
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_004_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForAxiBusName_TC_06_028_004
 */
// [IoGetAxiIdListForAxiBusName_TC_06_028_004]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForAxiBusName_TC_06_028_004)
{
  size_t num_of_axi_ids = 0;
  osal_axi_bus_id_t *buffer;
  size_t num_of_max_entry = 0;
  size_t num_of_written_axi_ids = 0;

  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForAxiBusName(axi_bus_name_test_data[0].test_bus_name, &num_of_max_entry), OSAL_RETURN_OK);
  EXPECT_EQ(axi_bus_name_test_data[0].axi_ids, num_of_max_entry);
  buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);

  // test: invalid parameter
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForAxiBusName(axi_bus_name_test_data[0].test_bus_name, buffer, num_of_max_entry - 1, &num_of_written_axi_ids), OSAL_RETURN_PAR) << "Test006_028_004 failed";

  free(buffer);
}
// [IoGetAxiIdListForAxiBusName_TC_06_028_004]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForAxiBusName_TC_06_028_005 IoAXIBusNameApiTest.IoGetAxiIdListForAxiBusName_TC_06_028_005
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_005_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_005_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_028_005
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_005_rr Related Requirement(s)
 * \#322528 \#326328
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_005_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForAxiBusName function
 * - Test06_028_005: Get the list of AXI IDs with buffer size is 0
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_005_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs for the given AXI Bus name "axi_bus_name_test_data[0].test_bus_name" by using R_OSAL_IoGetNumOfAxiIdsForAxiBusName
 * -# Allocate buffer which size is equal to "sizeof(osal_axi_bus_id_t) * num_of_max_entry"
 * -# Get the list of AXI Bus IDs by using R_OSAL_IoGetAxiIdListForAxiBusName with buffer size is 0
 * -# Free allocated buffer above
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForAxiBusName return OSAL_RETURN_OK.<br>
 * The value of "num_of_max_entry" is equal to the expected value
 * -# The allocation is successful
 * -# R_OSAL_IoGetAxiIdListForAxiBusName return OSAL_RETURN_PAR.
 * -# Allocated buffer is freeed successfully
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_005_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForAxiBusName_TC_06_028_005
 */
// [IoGetAxiIdListForAxiBusName_TC_06_028_005]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForAxiBusName_TC_06_028_005)
{
  size_t num_of_axi_ids = 0;
  osal_axi_bus_id_t *buffer;
  size_t num_of_max_entry = 0;
  size_t num_of_written_axi_ids = 0;

  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForAxiBusName(axi_bus_name_test_data[0].test_bus_name, &num_of_max_entry), OSAL_RETURN_OK);
  EXPECT_EQ(axi_bus_name_test_data[0].axi_ids, num_of_max_entry);
  buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);

  // test: invalid parameter
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForAxiBusName(axi_bus_name_test_data[0].test_bus_name, buffer, 0, &num_of_written_axi_ids), OSAL_RETURN_PAR) << "Test06_028_005 failed";

  free(buffer);
}
// [IoGetAxiIdListForAxiBusName_TC_06_028_005]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForAxiBusName_TC_06_028_006 IoAXIBusNameApiTest.IoGetAxiIdListForAxiBusName_TC_06_028_006
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_006_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_006_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_028_006
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_006_rr Related Requirement(s)
 * \#322528 \#326328
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_006_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_006_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForAxiBusName function
 * - Test06_028_006: Get the list of AXI IDs with p_num_of_written_axi_ids is NULL
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_006_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs for the given AXI Bus name "axi_bus_name_test_data[0].test_bus_name" by using R_OSAL_IoGetNumOfAxiIdsForAxiBusName
 * -# Allocate buffer which size is equal to "sizeof(osal_axi_bus_id_t) * num_of_max_entry"
 * -# Get the list of AXI Bus IDs by using R_OSAL_IoGetAxiIdListForAxiBusName with p_num_of_written_axi_ids is NULL
 * -# Free allocated buffer above
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_006_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForAxiBusName return OSAL_RETURN_OK.<br>
 * The value of "num_of_max_entry" is equal to the expected value
 * -# The allocation is successful
 * -# R_OSAL_IoGetAxiIdListForAxiBusName return OSAL_RETURN_PAR.
 * -# Allocated buffer is freeed successfully
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_006_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForAxiBusName_TC_06_028_006
 */
// [IoGetAxiIdListForAxiBusName_TC_06_028_006]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForAxiBusName_TC_06_028_006)
{
  size_t num_of_axi_ids = 0;
  osal_axi_bus_id_t *buffer;
  size_t num_of_max_entry = 0;
  size_t num_of_written_axi_ids = 0;

  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForAxiBusName(axi_bus_name_test_data[0].test_bus_name, &num_of_max_entry), OSAL_RETURN_OK);
  EXPECT_EQ(axi_bus_name_test_data[0].axi_ids, num_of_max_entry);
  buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);

  // test: invalid parameter
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForAxiBusName(axi_bus_name_test_data[0].test_bus_name, buffer, num_of_max_entry, NULL), OSAL_RETURN_PAR) << "Test06_028_006 failed";

  free(buffer);
}
// [IoGetAxiIdListForAxiBusName_TC_06_028_006]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForAxiBusName_TC_06_028_007 IoAXIBusNameApiTest.IoGetAxiIdListForAxiBusName_TC_06_028_007
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_007_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_007_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_028_007
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_007_rr Related Requirement(s)
 * \#322528 \#326328
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_007_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_007_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForAxiBusName function
 * - Test06_028_007: Get the list of AXI IDs for the valid axi_bus_name, but doesn'tregister aix_id
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_007_tpts Test Procedure / Test Steps
 * -# Allocate buffer which size is equal to "sizeof(osal_axi_bus_id_t) * 11"
 * -# Get the list of AXI Bus IDs for AXI Bus name "axi_bus_name_test_data[3].test_bus_name" by using R_OSAL_IoGetAxiIdListForAxiBusName
 * -# Free allocated buffer above
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_007_ereb Expected Result/Behavior
 * -# The allocation is successful
 * -# R_OSAL_IoGetAxiIdListForAxiBusName return OSAL_RETURN_UNKNOWN_NAME.
 * -# Allocated buffer is freeed successfully
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_007_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForAxiBusName_TC_06_028_007
 */
// [IoGetAxiIdListForAxiBusName_TC_06_028_007]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForAxiBusName_TC_06_028_007)
{
  size_t num_of_axi_ids = 0;
  osal_axi_bus_id_t *buffer;
  size_t num_of_max_entry = 11;
  size_t num_of_written_axi_ids = 0;

  buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);

  // axi_bus_name is valid, but doesn'tregister aix_id
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForAxiBusName(axi_bus_name_test_data[3].test_bus_name, buffer, num_of_max_entry, &num_of_written_axi_ids), OSAL_RETURN_UNKNOWN_NAME) << "Test06_028_007 failed";

  free(buffer);
}
// [IoGetAxiIdListForAxiBusName_TC_06_028_007]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForAxiBusName_TC_06_028_008 IoAXIBusNameApiTest.IoGetAxiIdListForAxiBusName_TC_06_028_008
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_008_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_008_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_028_008
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_008_rr Related Requirement(s)
 * \#322528 \#326328
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_008_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_008_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForAxiBusName function
 * - Test06_028_008: Get the list of AXI IDs for the valid axi_bus_name, but doesn'tregister aix_id
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_008_tpts Test Procedure / Test Steps
 * -# Allocate buffer which size is equal to "sizeof(osal_axi_bus_id_t) * 11"
 * -# Get the list of AXI Bus IDs for AXI Bus name "axi_bus_name_test_data[4].test_bus_name" by using R_OSAL_IoGetAxiIdListForAxiBusName
 * -# Free allocated buffer above
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_008_ereb Expected Result/Behavior
 * -# The allocation is successful
 * -# R_OSAL_IoGetAxiIdListForAxiBusName return OSAL_RETURN_UNKNOWN_NAME.
 * -# Allocated buffer is freeed successfully
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_008_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForAxiBusName_TC_06_028_008
 */
// [IoGetAxiIdListForAxiBusName_TC_06_028_008]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForAxiBusName_TC_06_028_008)
{
  size_t num_of_axi_ids = 0;
  osal_axi_bus_id_t *buffer;
  size_t num_of_max_entry = 11;
  size_t num_of_written_axi_ids = 0;

  buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);

  // axi_bus_name is valid, but doesn'tregister aix_id
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForAxiBusName(axi_bus_name_test_data[4].test_bus_name, buffer, num_of_max_entry, &num_of_written_axi_ids), OSAL_RETURN_UNKNOWN_NAME) << "Test06_028_008 failed";

  free(buffer);
}
// [IoGetAxiIdListForAxiBusName_TC_06_028_008]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForAxiBusName_TC_06_028_009 IoAXIBusNameApiTest.IoGetAxiIdListForAxiBusName_TC_06_028_009
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_009_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_009_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S2_A_TC_06_028_009
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_009_rr Related Requirement(s)
 * \#322528 \#326328
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_009_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_009_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForAxiBusName function
 * - Test06_028_009: Confirm state transition of R_OSAL_IoGetAxiIdListForAxiBusName
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_009_tpts Test Procedure / Test Steps
 * -# Allocate buffer which size is equal to "sizeof(osal_axi_bus_id_t) * 11"
 * -# Close opened devices by using R_OSAL_IoDeviceClose
 * -# Deinit OSAL APIs by using R_OSAL_Deinitialize
 * -# Call R_OSAL_IoGetAxiIdListForAxiBusName
 * -# Init OSAL APIs by using R_OSAL_Initialize
 * -# Open again required devices by using R_OSAL_IoDeviceOpen
 * -# Free allocated buffer above
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_009_ereb Expected Result/Behavior
 * -# The allocation is successful
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_Deinitialize return OSAL_RETURN_OK
 * -# R_OSAL_IoGetAxiIdListForAxiBusName return OSAL_RETURN_STATE
 * -# R_OSAL_Initialize return OSAL_RETURN_OK
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK
 * -# Allocated buffer is freeed successfully
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_009_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForAxiBusName_TC_06_028_009
 */
// [IoGetAxiIdListForAxiBusName_TC_06_028_009]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForAxiBusName_TC_06_028_009)
{
  size_t num_of_axi_ids = 0;
  osal_axi_bus_id_t *buffer;
  size_t num_of_max_entry = 11;
  size_t num_of_written_axi_ids = 0;

  buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);

  // pre-process of testing
  EXPECT_EQ(R_OSAL_IoDeviceClose(device_info_test_data[0].test_hndl), OSAL_RETURN_OK);
  EXPECT_EQ(R_OSAL_IoDeviceClose(device_info_test_data[1].test_hndl), OSAL_RETURN_OK);
  EXPECT_EQ(R_OSAL_IoDeviceClose(device_info_test_data[3].test_hndl), OSAL_RETURN_OK);
  EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_OK) << "pre-process failed";

  // test: Don't execute initialize
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForAxiBusName(axi_bus_name_test_data[0].test_bus_name, buffer, num_of_max_entry, &num_of_written_axi_ids), OSAL_RETURN_STATE) << "Test06_028_009 failed";

  // post-process of testing
  EXPECT_EQ(R_OSAL_Initialize(), OSAL_RETURN_OK) << "post-process failed";
  EXPECT_EQ(R_OSAL_IoDeviceOpen(device_info_test_data[0].test_device_name, &device_info_test_data[0].test_hndl), OSAL_RETURN_OK);
  EXPECT_EQ(R_OSAL_IoDeviceOpen(device_info_test_data[1].test_device_name, &device_info_test_data[1].test_hndl), OSAL_RETURN_OK);
  EXPECT_EQ(R_OSAL_IoDeviceOpen(device_info_test_data[3].test_device_name, &device_info_test_data[3].test_hndl), OSAL_RETURN_OK);

  free(buffer);
}
// [IoGetAxiIdListForAxiBusName_TC_06_028_009]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForAxiBusName_TC_06_028_010 IoAXIBusNameApiTest.IoGetAxiIdListForAxiBusName_TC_06_028_010
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_010_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_010_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S1_N_TC_06_028_010
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_010_rr Related Requirement(s)
 * \#322528 \#326328
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_010_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_010_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForAxiBusName function
 * - Test06_028_010: Get the list of AXI IDs for the given valid AXI Bus name
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_010_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs for the given AXI Bus name "axi_bus_name_test_data[5].test_bus_name" by using R_OSAL_IoGetNumOfAxiIdsForAxiBusName
 * -# Allocate buffer which size is equal to "sizeof(osal_axi_bus_id_t) * num_of_max_entry"
 * -# Get the list of AXI Bus IDs related to AXI Bus name "axi_bus_name_test_data[5].test_bus_name" by using R_OSAL_IoGetAxiIdListForAxiBusName
 * -# Free allocated buffer above
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_010_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForAxiBusName return OSAL_RETURN_OK.<br>
 * The value of "num_of_max_entry" is equal to the expected value
 * -# The allocation is successful
 * -# R_OSAL_IoGetAxiIdListForAxiBusName return OSAL_RETURN_OK.<br>
 * The value of "num_of_written_axi_ids" is equal to expected value
 * -# Allocated buffer is freeed successfully
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_010_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForAxiBusName_TC_06_028_010
 */
// [IoGetAxiIdListForAxiBusName_TC_06_028_010]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForAxiBusName_TC_06_028_010)
{
  size_t num_of_axi_ids = 0;
  osal_axi_bus_id_t *buffer;
  size_t num_of_max_entry = 0;
  size_t num_of_written_axi_ids = 0;

  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForAxiBusName(axi_bus_name_test_data[5].test_bus_name, &num_of_max_entry), OSAL_RETURN_OK);
  EXPECT_EQ(axi_bus_name_test_data[5].axi_ids, num_of_max_entry);
  buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);

  // test: valid parameters
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForAxiBusName(axi_bus_name_test_data[5].test_bus_name, buffer, num_of_max_entry, &num_of_written_axi_ids), OSAL_RETURN_OK) << "Test06_028_010 failed";
  EXPECT_EQ(axi_bus_name_test_data[5].axi_ids, num_of_written_axi_ids);

  free(buffer);
}
// [IoGetAxiIdListForAxiBusName_TC_06_028_010]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForAxiBusName_TC_06_028_011 IoAXIBusNameApiTest.IoGetAxiIdListForAxiBusName_TC_06_028_011
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_011_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_011_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S1_N_TC_06_028_011
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_011_rr Related Requirement(s)
 * \#322528 \#326328
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_011_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_011_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForAxiBusName function
 * - Test06_028_011: Get the list of AXI IDs for the given valid AXI Bus name
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_011_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs for the given AXI Bus name "axi_bus_name_test_data[6].test_bus_name" by using R_OSAL_IoGetNumOfAxiIdsForAxiBusName
 * -# Allocate buffer which size is equal to "sizeof(osal_axi_bus_id_t) * num_of_max_entry"
 * -# Get the list of AXI Bus IDs related to AXI Bus name "axi_bus_name_test_data[6].test_bus_name" by using R_OSAL_IoGetAxiIdListForAxiBusName
 * -# Free allocated buffer above
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_011_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForAxiBusName return OSAL_RETURN_OK.<br>
 * The value of "num_of_max_entry" is equal to the expected value
 * -# The allocation is successful
 * -# R_OSAL_IoGetAxiIdListForAxiBusName return OSAL_RETURN_OK.<br>
 * The value of "num_of_written_axi_ids" is equal to expected value
 * -# Allocated buffer is freeed successfully
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_011_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForAxiBusName_TC_06_028_011
 */
// [IoGetAxiIdListForAxiBusName_TC_06_028_011]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForAxiBusName_TC_06_028_011)
{
  size_t num_of_axi_ids = 0;
  osal_axi_bus_id_t *buffer;
  size_t num_of_max_entry = 0;
  size_t num_of_written_axi_ids = 0;

  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForAxiBusName(axi_bus_name_test_data[6].test_bus_name, &num_of_max_entry), OSAL_RETURN_OK);
  EXPECT_EQ(axi_bus_name_test_data[6].axi_ids, num_of_max_entry);
  buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);

  // test: valid parameters
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForAxiBusName(axi_bus_name_test_data[6].test_bus_name, buffer, num_of_max_entry, &num_of_written_axi_ids), OSAL_RETURN_OK) << "Test06_028_011 failed";
  EXPECT_EQ(axi_bus_name_test_data[6].axi_ids, num_of_written_axi_ids);

  free(buffer);
}
// [IoGetAxiIdListForAxiBusName_TC_06_028_011]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForDeviceHdl_TC_06_029_001 IoAXIBusNameApiTest.IoGetAxiIdListForDeviceHdl_TC_06_029_001
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_001_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_001_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S1_N_TC_06_029_001
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_001_rr Related Requirement(s)
 * \#329828
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Device handle was opened
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_001_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForDeviceHdl function
 * - Test06_029_001: Get the list of AXI IDs for the given valid device handle
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_001_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs for the given device handle "device_info_test_data[0].test_hndl" by using R_OSAL_IoGetNumOfAxiIdsForDeviceHdl
 * -# Allocate buffer which size is equal to "sizeof(osal_axi_bus_id_t) * num_of_max_entry"
 * -# Get the list of AXI Bus IDs related to device handle "device_info_test_data[0].test_hndl" by using R_OSAL_IoGetAxiIdListForDeviceHdl
 * -# Free allocated buffer above
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_001_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForDeviceHdl return OSAL_RETURN_OK.<br>
 * The value of "num_of_max_entry" is equal to the expected value
 * -# The allocation is successful
 * -# R_OSAL_IoGetAxiIdListForDeviceHdl return OSAL_RETURN_OK.<br>
 * The value of "num_of_written_axi_ids" is equal to expected value
 * -# Allocated buffer is freeed successfully
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_001_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForDeviceHdl_TC_06_029_001
 */
// [IoGetAxiIdListForDeviceHdl_TC_06_029_001]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForDeviceHdl_TC_06_029_001)
{
  size_t num_of_axi_ids = 0;
  osal_axi_bus_id_t *buffer;
  size_t num_of_max_entry = 0;
  size_t num_of_written_axi_ids = 0;

  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForDeviceHdl(device_info_test_data[0].test_hndl, &num_of_max_entry), OSAL_RETURN_OK);
  EXPECT_EQ(device_info_test_data[0].axi_ids, num_of_max_entry);
  buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);

  // test: valid parameters
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForDeviceHdl(device_info_test_data[0].test_hndl, buffer, num_of_max_entry, &num_of_written_axi_ids), OSAL_RETURN_OK) << "Test06_029_001 failed";
  EXPECT_EQ(device_info_test_data[0].axi_ids, num_of_written_axi_ids);

  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForDeviceHdl(device_info_test_data[3].test_hndl, &num_of_max_entry), OSAL_RETURN_OK);
  EXPECT_EQ(device_info_test_data[3].axi_ids, num_of_max_entry);
  buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);

  // test: valid parameters
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForDeviceHdl(device_info_test_data[3].test_hndl, buffer, num_of_max_entry, &num_of_written_axi_ids), OSAL_RETURN_OK) << "Test06_029_001 failed";
  EXPECT_EQ(device_info_test_data[3].axi_ids, num_of_written_axi_ids);

  free(buffer);
}
// [IoGetAxiIdListForDeviceHdl_TC_06_029_001]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForDeviceHdl_TC_06_029_002 IoAXIBusNameApiTest.IoGetAxiIdListForDeviceHdl_TC_06_029_002
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_002_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_002_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_029_002
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_002_rr Related Requirement(s)
 * \#329828
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Device handle was opened
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_002_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForDeviceHdl function
 * - Test06_029_002: Get the list of AXI IDs with p_array is NULL
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_002_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs for the given device handle "device_info_test_data[0].test_hndl" by using R_OSAL_IoGetNumOfAxiIdsForDeviceHdl
 * -# Allocate buffer which size is equal to "sizeof(osal_axi_bus_id_t) * num_of_max_entry"
 * -# Get the list of AXI Bus IDs by using R_OSAL_IoGetAxiIdListForDeviceHdl with p_array is NULL
 * -# Free allocated buffer above
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_002_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForDeviceHdl return OSAL_RETURN_OK.<br>
 * The value of "num_of_max_entry" is equal to the expected value
 * -# The allocation is successful
 * -# R_OSAL_IoGetAxiIdListForDeviceHdl return OSAL_RETURN_PAR
 * -# Allocated buffer is freeed successfully
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_002_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForDeviceHdl_TC_06_029_002
 */
// [IoGetAxiIdListForDeviceHdl_TC_06_029_002]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForDeviceHdl_TC_06_029_002)
{
  size_t num_of_axi_ids = 0;
  osal_axi_bus_id_t *buffer;
  size_t num_of_max_entry = 0;
  size_t num_of_written_axi_ids = 0;

  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForDeviceHdl(device_info_test_data[0].test_hndl, &num_of_max_entry), OSAL_RETURN_OK);
  EXPECT_EQ(device_info_test_data[0].axi_ids, num_of_max_entry);
  buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);

  // test: invalid parameter
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForDeviceHdl(device_info_test_data[0].test_hndl, NULL, num_of_max_entry, &num_of_written_axi_ids), OSAL_RETURN_PAR) << "Test06_029_002 failed";

  free(buffer);
}
// [IoGetAxiIdListForDeviceHdl_TC_06_029_002]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForDeviceHdl_TC_06_029_003 IoAXIBusNameApiTest.IoGetAxiIdListForDeviceHdl_TC_06_029_003
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_003_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_003_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_029_003
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_003_rr Related Requirement(s)
 * \#329828
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Device handle was opened
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_003_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForDeviceHdl function
 * - Test06_029_003: Get the list of AXI IDs with buffer size is less than buffer required size
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_003_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs for the given device handle "device_info_test_data[0].test_hndl" by using R_OSAL_IoGetNumOfAxiIdsForDeviceHdl
 * -# Allocate buffer which size is equal to "sizeof(osal_axi_bus_id_t) * num_of_max_entry"
 * -# Get the list of AXI Bus IDs by using R_OSAL_IoGetAxiIdListForDeviceHdl with buffer size is equal to "num_of_max_entry - 1"
 * -# Free allocated buffer above
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_003_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForDeviceHdl return OSAL_RETURN_OK.<br>
 * The value of "num_of_max_entry" is equal to the expected value
 * -# The allocation is successful
 * -# R_OSAL_IoGetAxiIdListForDeviceHdl return OSAL_RETURN_PAR
 * -# Allocated buffer is freeed successfully
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_003_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForDeviceHdl_TC_06_029_003
 */
// [IoGetAxiIdListForDeviceHdl_TC_06_029_003]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForDeviceHdl_TC_06_029_003)
{
  size_t num_of_axi_ids = 0;
  osal_axi_bus_id_t *buffer;
  size_t num_of_max_entry = 0;
  size_t num_of_written_axi_ids = 0;

  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForDeviceHdl(device_info_test_data[0].test_hndl, &num_of_max_entry), OSAL_RETURN_OK);
  EXPECT_EQ(device_info_test_data[0].axi_ids, num_of_max_entry);
  buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);

  // test: invalid parameter
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForDeviceHdl(device_info_test_data[0].test_hndl, buffer, num_of_max_entry - 1, &num_of_written_axi_ids), OSAL_RETURN_PAR) << "Test06_029_003 failed";

  free(buffer);
}
// [IoGetAxiIdListForDeviceHdl_TC_06_029_003]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForDeviceHdl_TC_06_029_004 IoAXIBusNameApiTest.IoGetAxiIdListForDeviceHdl_TC_06_029_004
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_004_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_004_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_029_004
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_004_rr Related Requirement(s)
 * \#329828
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Device handle was opened
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_004_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForDeviceHdl function
 * - Test06_029_004: Get the list of AXI IDs with buffer size is 0
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_004_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs for the given device handle "device_info_test_data[0].test_hndl" by using R_OSAL_IoGetNumOfAxiIdsForDeviceHdl
 * -# Allocate buffer which size is equal to "sizeof(osal_axi_bus_id_t) * num_of_max_entry"
 * -# Get the list of AXI Bus IDs by using R_OSAL_IoGetAxiIdListForDeviceHdl with buffer size is 0
 * -# Free allocated buffer above
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_004_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForDeviceHdl return OSAL_RETURN_OK.<br>
 * The value of "num_of_max_entry" is equal to the expected value
 * -# The allocation is successful
 * -# R_OSAL_IoGetAxiIdListForDeviceHdl return OSAL_RETURN_PAR
 * -# Allocated buffer is freeed successfully
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_004_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForDeviceHdl_TC_06_029_004
 */
// [IoGetAxiIdListForDeviceHdl_TC_06_029_004]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForDeviceHdl_TC_06_029_004)
{
  size_t num_of_axi_ids = 0;
  osal_axi_bus_id_t *buffer;
  size_t num_of_max_entry = 0;
  size_t num_of_written_axi_ids = 0;

  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForDeviceHdl(device_info_test_data[0].test_hndl, &num_of_max_entry), OSAL_RETURN_OK);
  EXPECT_EQ(device_info_test_data[0].axi_ids, num_of_max_entry);
  buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);

  // test: invalid parameter
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForDeviceHdl(device_info_test_data[0].test_hndl, buffer, 0, &num_of_written_axi_ids), OSAL_RETURN_PAR) << "Test06_029_004 failed";

  free(buffer);
}
// [IoGetAxiIdListForDeviceHdl_TC_06_029_004]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForDeviceHdl_TC_06_029_005 IoAXIBusNameApiTest.IoGetAxiIdListForDeviceHdl_TC_06_029_005
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_005_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_005_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_029_005
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_005_rr Related Requirement(s)
 * \#329828
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Device handle was opened
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_005_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForDeviceHdl function
 * - Test06_029_005: Get the list of AXI IDs with p_num_of_written_axi_ids is NULL
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_005_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs for the given device handle "device_info_test_data[0].test_hndl" by using R_OSAL_IoGetNumOfAxiIdsForDeviceHdl
 * -# Allocate buffer which size is equal to "sizeof(osal_axi_bus_id_t) * num_of_max_entry"
 * -# Get the list of AXI Bus IDs by using R_OSAL_IoGetAxiIdListForDeviceHdl with p_num_of_written_axi_ids is NULL
 * -# Free allocated buffer above
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForDeviceHdl return OSAL_RETURN_OK.<br>
 * The value of "num_of_max_entry" is equal to the expected value
 * -# The allocation is successful
 * -# R_OSAL_IoGetAxiIdListForDeviceHdl return OSAL_RETURN_PAR
 * -# Allocated buffer is freeed successfully
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_005_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForDeviceHdl_TC_06_029_005
 */
// [IoGetAxiIdListForDeviceHdl_TC_06_029_005]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForDeviceHdl_TC_06_029_005)
{
  size_t num_of_axi_ids = 0;
  osal_axi_bus_id_t *buffer;
  size_t num_of_max_entry = 0;
  size_t num_of_written_axi_ids = 0;

  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForDeviceHdl(device_info_test_data[0].test_hndl, &num_of_max_entry), OSAL_RETURN_OK);
  EXPECT_EQ(device_info_test_data[0].axi_ids, num_of_max_entry);
  buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);

  // test: invalid parameter
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForDeviceHdl(device_info_test_data[0].test_hndl, buffer, num_of_max_entry, NULL), OSAL_RETURN_PAR) << "Test06_029_005 failed";

  free(buffer);
}
// [IoGetAxiIdListForDeviceHdl_TC_06_029_005]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForDeviceHdl_TC_06_029_006 IoAXIBusNameApiTest.IoGetAxiIdListForDeviceHdl_TC_06_029_006
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_006_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_006_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_029_006
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_006_rr Related Requirement(s)
 * \#329828
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_006_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_006_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForDeviceHdl function
 * - Test06_029_006: Get the list of AXI IDs with unknow device name
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_006_tpts Test Procedure / Test Steps
 * -# Allocate buffer which size is equal to "sizeof(osal_axi_bus_id_t) * 11"
 * -# Get the list of AXI Bus IDs by using R_OSAL_IoGetAxiIdListForDeviceHdl with unknow device name
 * -# Free allocated buffer above
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_006_ereb Expected Result/Behavior
 * -# The allocation is successful
 * -# R_OSAL_IoGetAxiIdListForDeviceHdl return OSAL_RETURN_HANDLE
 * -# Allocated buffer is freeed successfully
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_006_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForDeviceHdl_TC_06_029_006
 */
// [IoGetAxiIdListForDeviceHdl_TC_06_029_006]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForDeviceHdl_TC_06_029_006)
{
  size_t num_of_axi_ids = 0;
  osal_axi_bus_id_t *buffer;
  size_t num_of_max_entry = 11;
  size_t num_of_written_axi_ids = 0;

  buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);

  // test: invalid device handle
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForDeviceHdl(device_info_test_data[2].test_hndl, buffer, num_of_max_entry, &num_of_written_axi_ids), OSAL_RETURN_HANDLE) << "Test06_029_006 failed";

  free(buffer);
}
// [IoGetAxiIdListForDeviceHdl_TC_06_029_006]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForDeviceHdl_TC_06_029_007 IoAXIBusNameApiTest.IoGetAxiIdListForDeviceHdl_TC_06_029_007
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_007_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_007_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S1_N_TC_06_029_007
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_007_rr Related Requirement(s)
 * \#329828
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_007_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Device handle was opened
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_007_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForDeviceHdl function
 * - Test06_029_007: Get the list of AXI IDs with valid handle, but device handle doesn'thave axi_id
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_007_tpts Test Procedure / Test Steps
 * -# Allocate buffer which size is equal to "sizeof(osal_axi_bus_id_t) * 11"
 * -# Get the list of AXI Bus IDs by using R_OSAL_IoGetAxiIdListForDeviceHdl with valid handle, but device handle doesn'thave axi_id
 * -# Free allocated buffer above
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_007_ereb Expected Result/Behavior
 * -# The allocation is successful
 * -# R_OSAL_IoGetAxiIdListForDeviceHdl return OSAL_RETURN_OK
 * -# Allocated buffer is freeed successfully
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_007_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForDeviceHdl_TC_06_029_007
 */
// [IoGetAxiIdListForDeviceHdl_TC_06_029_007]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForDeviceHdl_TC_06_029_007)
{
  size_t num_of_axi_ids = 0;
  osal_axi_bus_id_t *buffer;
  size_t num_of_max_entry = 11;
  size_t num_of_written_axi_ids = 0;

  buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);

  // test: valid handle, but device handle doesn'thave axi_id
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForDeviceHdl(device_info_test_data[1].test_hndl, buffer, num_of_max_entry, &num_of_written_axi_ids), OSAL_RETURN_OK) << "Test06_029_007 failed";

  free(buffer);
}
// [IoGetAxiIdListForDeviceHdl_TC_06_029_007]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForDeviceHdl_TC_06_029_008 IoAXIBusNameApiTest.IoGetAxiIdListForDeviceHdl_TC_06_029_008
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_008_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_008_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S2_A_TC_06_029_008
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_008_rr Related Requirement(s)
 * \#329828
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_008_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_008_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForDeviceHdl function
 * - Test06_029_008: Confirm state transition of R_OSAL_IoGetAxiIdListForDeviceHdl
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_008_tpts Test Procedure / Test Steps
 * -# Allocate buffer which size is equal to "sizeof(osal_axi_bus_id_t) * 11"
 * -# Close opened devices by using R_OSAL_IoDeviceClose
 * -# Deinit OSAL APIs by using R_OSAL_Deinitialize
 * -# Call R_OSAL_IoGetAxiIdListForDeviceHdl
 * -# Init OSAL APIs by using R_OSAL_Initialize
 * -# Open again required devices by using R_OSAL_IoDeviceOpen
 * -# Free allocated buffer above
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_008_ereb Expected Result/Behavior
 * -# The allocation is successful
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_Deinitialize return OSAL_RETURN_OK
 * -# R_OSAL_IoGetAxiIdListForDeviceHdl return OSAL_RETURN_STATE
 * -# R_OSAL_Initialize return OSAL_RETURN_OK
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK
 * -# Allocated buffer is freeed successfully
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_008_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForDeviceHdl_TC_06_029_008
 */
// [IoGetAxiIdListForDeviceHdl_TC_06_029_008]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForDeviceHdl_TC_06_029_008)
{
  size_t num_of_axi_ids = 0;
  osal_axi_bus_id_t *buffer;
  size_t num_of_max_entry = 11;
  size_t num_of_written_axi_ids = 0;

  buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);

  // pre-process of testing
  EXPECT_EQ(R_OSAL_IoDeviceClose(device_info_test_data[0].test_hndl), OSAL_RETURN_OK);
  EXPECT_EQ(R_OSAL_IoDeviceClose(device_info_test_data[1].test_hndl), OSAL_RETURN_OK);
  EXPECT_EQ(R_OSAL_IoDeviceClose(device_info_test_data[3].test_hndl), OSAL_RETURN_OK);
  EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_OK) << "pre-process failed";

  // test: Don't execute initialize
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForDeviceHdl(device_info_test_data[0].test_hndl, buffer, num_of_max_entry, &num_of_written_axi_ids), OSAL_RETURN_STATE) << "Test06_029_008 failed";

  // post-process of testing
  EXPECT_EQ(R_OSAL_Initialize(), OSAL_RETURN_OK) << "post-process failed";
  EXPECT_EQ(R_OSAL_IoDeviceOpen(device_info_test_data[0].test_device_name, &device_info_test_data[0].test_hndl), OSAL_RETURN_OK);
  EXPECT_EQ(R_OSAL_IoDeviceOpen(device_info_test_data[1].test_device_name, &device_info_test_data[1].test_hndl), OSAL_RETURN_OK);
  EXPECT_EQ(R_OSAL_IoDeviceOpen(device_info_test_data[3].test_device_name, &device_info_test_data[3].test_hndl), OSAL_RETURN_OK);

  free(buffer);
}
// [IoGetAxiIdListForDeviceHdl_TC_06_029_008]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForDeviceName_TC_06_030_001 IoAXIBusNameApiTest.IoGetAxiIdListForDeviceName_TC_06_030_001
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_001_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_001_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S1_N_TC_06_030_001
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_001_rr Related Requirement(s)
 * \#329830
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_001_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForDeviceName function
 * - Test06_030_001: Get the list of AXI IDs for the given valid device name
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_001_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs for the given device name "device_info_test_data[0].test_device_name" by using R_OSAL_IoGetNumOfAxiIdsForDeviceName
 * -# Allocate buffer which size is equal to "sizeof(osal_axi_bus_id_t) * num_of_max_entry"
 * -# Get the list of AXI Bus IDs related to device name "device_info_test_data[0].test_device_name" by using R_OSAL_IoGetAxiIdListForDeviceName
 * -# Free allocated buffer above
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_001_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForDeviceName return OSAL_RETURN_OK.<br>
 * The value of "num_of_max_entry" is equal to the expected value
 * -# The allocation is successful
 * -# R_OSAL_IoGetAxiIdListForDeviceName return OSAL_RETURN_OK.<br>
 * The value of "num_of_written_axi_ids" is equal to expected value
 * -# Allocated buffer is freeed successfully
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_001_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForDeviceName_TC_06_030_001
 */
// [IoGetAxiIdListForDeviceName_TC_06_030_001]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForDeviceName_TC_06_030_001)
{
  size_t num_of_axi_ids = 0;
  osal_axi_bus_id_t *buffer;
  size_t num_of_max_entry = 0;
  size_t num_of_written_axi_ids = 0;

  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForDeviceName(device_info_test_data[0].test_device_name, &num_of_max_entry), OSAL_RETURN_OK);
  EXPECT_EQ(device_info_test_data[0].axi_ids, num_of_max_entry);
  buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);

  // test: valid parameters
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForDeviceName(device_info_test_data[0].test_device_name, buffer, num_of_max_entry, &num_of_written_axi_ids), OSAL_RETURN_OK) << "Test06_030_001 failed";
  EXPECT_EQ(device_info_test_data[0].axi_ids, num_of_written_axi_ids);

  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForDeviceName(device_info_test_data[3].test_device_name, &num_of_max_entry), OSAL_RETURN_OK);
  EXPECT_EQ(device_info_test_data[3].axi_ids, num_of_max_entry);
  buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);

  // test: valid parameters
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForDeviceName(device_info_test_data[3].test_device_name, buffer, num_of_max_entry, &num_of_written_axi_ids), OSAL_RETURN_OK) << "Test06_030_001 failed";
  EXPECT_EQ(device_info_test_data[3].axi_ids, num_of_written_axi_ids);

  free(buffer);
}
// [IoGetAxiIdListForDeviceName_TC_06_030_001]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForDeviceName_TC_06_030_002 IoAXIBusNameApiTest.IoGetAxiIdListForDeviceName_TC_06_030_002
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_002_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_002_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_030_002
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_002_rr Related Requirement(s)
 * \#329830
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_002_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForDeviceName function
 * - Test06_030_002: Get the list of AXI IDs with p_array is NULL
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_002_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs for the given device name "device_info_test_data[0].test_device_name" by using R_OSAL_IoGetNumOfAxiIdsForDeviceName
 * -# Allocate buffer which size is equal to "sizeof(osal_axi_bus_id_t) * num_of_max_entry"
 * -# Get the list of AXI Bus IDs by using R_OSAL_IoGetAxiIdListForDeviceName with p_array is NULL
 * -# Free allocated buffer above
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_002_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForDeviceName return OSAL_RETURN_OK.<br>
 * The value of "num_of_max_entry" is equal to the expected value
 * -# The allocation is successful
 * -# R_OSAL_IoGetAxiIdListForDeviceName return OSAL_RETURN_PAR
 * -# Allocated buffer is freeed successfully
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_002_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForDeviceName_TC_06_030_002
 */
// [IoGetAxiIdListForDeviceName_TC_06_030_002]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForDeviceName_TC_06_030_002)
{
  size_t num_of_axi_ids = 0;
  osal_axi_bus_id_t *buffer;
  size_t num_of_max_entry = 0;
  size_t num_of_written_axi_ids = 0;

  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForDeviceName(device_info_test_data[0].test_device_name, &num_of_max_entry), OSAL_RETURN_OK);
  EXPECT_EQ(device_info_test_data[0].axi_ids, num_of_max_entry);
  buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);

  // test: invalid parameter
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForDeviceName(device_info_test_data[0].test_device_name, NULL, num_of_max_entry, &num_of_written_axi_ids), OSAL_RETURN_PAR) << "Test06_030_002 failed";

  free(buffer);
}
// [IoGetAxiIdListForDeviceName_TC_06_030_002]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForDeviceName_TC_06_030_003 IoAXIBusNameApiTest.IoGetAxiIdListForDeviceName_TC_06_030_003
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_003_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_003_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_030_003
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_003_rr Related Requirement(s)
 * \#329830
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_003_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForDeviceName function
 * - Test06_030_003: Get the list of AXI IDs with p_num_of_written_axi_ids is NULL
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_003_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs for the given device name "device_info_test_data[0].test_device_name" by using R_OSAL_IoGetNumOfAxiIdsForDeviceName
 * -# Allocate buffer which size is equal to "sizeof(osal_axi_bus_id_t) * num_of_max_entry"
 * -# Get the list of AXI Bus IDs by using R_OSAL_IoGetAxiIdListForDeviceName with p_num_of_written_axi_ids is NULL
 * -# Free allocated buffer above
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_003_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForDeviceName return OSAL_RETURN_OK.<br>
 * The value of "num_of_max_entry" is equal to the expected value
 * -# The allocation is successful
 * -# R_OSAL_IoGetAxiIdListForDeviceName return OSAL_RETURN_PAR
 * -# Allocated buffer is freeed successfully
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_003_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForDeviceName_TC_06_030_003
 */
// [IoGetAxiIdListForDeviceName_TC_06_030_003]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForDeviceName_TC_06_030_003)
{
  size_t num_of_axi_ids = 0;
  osal_axi_bus_id_t *buffer;
  size_t num_of_max_entry = 0;
  size_t num_of_written_axi_ids = 0;

  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForDeviceName(device_info_test_data[0].test_device_name, &num_of_max_entry), OSAL_RETURN_OK);
  EXPECT_EQ(device_info_test_data[0].axi_ids, num_of_max_entry);
  buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);

  // test: invalid parameter
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForDeviceName(device_info_test_data[0].test_device_name, buffer, num_of_max_entry, NULL), OSAL_RETURN_PAR) << "Test06_030_003 failed";

  free(buffer);
}
// [IoGetAxiIdListForDeviceName_TC_06_030_003]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForDeviceName_TC_06_030_004 IoAXIBusNameApiTest.IoGetAxiIdListForDeviceName_TC_06_030_004
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_004_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_004_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_030_004
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_004_rr Related Requirement(s)
 * \#329830
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_004_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForDeviceName function
 * - Test06_030_004: Get the list of AXI IDs with buffer size is less than required buffer size
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_004_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs for the given device name "device_info_test_data[0].test_device_name" by using R_OSAL_IoGetNumOfAxiIdsForDeviceName
 * -# Allocate buffer which size is equal to "sizeof(osal_axi_bus_id_t) * num_of_max_entry"
 * -# Get the list of AXI Bus IDs by using R_OSAL_IoGetAxiIdListForDeviceName with buffer size is equal to "num_of_max_entry - 1"
 * -# Free allocated buffer above
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_004_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForDeviceName return OSAL_RETURN_OK.<br>
 * The value of "num_of_max_entry" is equal to the expected value
 * -# The allocation is successful
 * -# R_OSAL_IoGetAxiIdListForDeviceName return OSAL_RETURN_PAR
 * -# Allocated buffer is freeed successfully
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_004_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForDeviceName_TC_06_030_004
 */
// [IoGetAxiIdListForDeviceName_TC_06_030_004]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForDeviceName_TC_06_030_004)
{
  size_t num_of_axi_ids = 0;
  osal_axi_bus_id_t *buffer;
  size_t num_of_max_entry = 0;
  size_t num_of_written_axi_ids = 0;

  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForDeviceName(device_info_test_data[0].test_device_name, &num_of_max_entry), OSAL_RETURN_OK);
  EXPECT_EQ(device_info_test_data[0].axi_ids, num_of_max_entry);
  buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);

  // test: invalid parameter
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForDeviceName(device_info_test_data[0].test_device_name, buffer, num_of_max_entry - 1, &num_of_written_axi_ids), OSAL_RETURN_PAR) << "Test06_030_004 failed";

  free(buffer);
}
// [IoGetAxiIdListForDeviceName_TC_06_030_004]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForDeviceName_TC_06_030_005 IoAXIBusNameApiTest.IoGetAxiIdListForDeviceName_TC_06_030_005
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_005_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_005_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_030_005
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_005_rr Related Requirement(s)
 * \#329830
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_005_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForDeviceName function
 * - Test06_030_005: Get the list of AXI IDs with buffer size is 0
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_005_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs for the given device name "device_info_test_data[0].test_device_name" by using R_OSAL_IoGetNumOfAxiIdsForDeviceName
 * -# Allocate buffer which size is equal to "sizeof(osal_axi_bus_id_t) * num_of_max_entry"
 * -# Get the list of AXI Bus IDs by using R_OSAL_IoGetAxiIdListForDeviceName with buffer size is 0
 * -# Free allocated buffer above
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForDeviceName return OSAL_RETURN_OK.<br>
 * The value of "num_of_max_entry" is equal to the expected value
 * -# The allocation is successful
 * -# R_OSAL_IoGetAxiIdListForDeviceName return OSAL_RETURN_PAR
 * -# Allocated buffer is freeed successfully
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_005_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForDeviceName_TC_06_030_005
 */
// [IoGetAxiIdListForDeviceName_TC_06_030_005]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForDeviceName_TC_06_030_005)
{
  size_t num_of_axi_ids = 0;
  osal_axi_bus_id_t *buffer;
  size_t num_of_max_entry = 0;
  size_t num_of_written_axi_ids = 0;

  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForDeviceName(device_info_test_data[0].test_device_name, &num_of_max_entry), OSAL_RETURN_OK);
  EXPECT_EQ(device_info_test_data[0].axi_ids, num_of_max_entry);
  buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);

  // test: invalid parameter
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForDeviceName(device_info_test_data[0].test_device_name, buffer, 0, &num_of_written_axi_ids), OSAL_RETURN_PAR) << "Test06_030_005 failed";

  free(buffer);
}
// [IoGetAxiIdListForDeviceName_TC_06_030_005]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForDeviceName_TC_06_030_006 IoAXIBusNameApiTest.IoGetAxiIdListForDeviceName_TC_06_030_006
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_006_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_006_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S1_N_TC_06_030_006
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_006_rr Related Requirement(s)
 * \#329830
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_006_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_006_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForDeviceName function
 * - Test06_030_006: Get the list of AXI IDs with valid device name, but device doesn'thave axi_id
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_006_tpts Test Procedure / Test Steps
 * -# Allocate buffer which size is equal to "sizeof(osal_axi_bus_id_t) * 11"
 * -# Get the list of AXI Bus IDs by using R_OSAL_IoGetAxiIdListForDeviceName with valid device name, but device doesn'thave axi_id
 * -# Free allocated buffer above
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_006_ereb Expected Result/Behavior
 * -# The allocation is successful
 * -# R_OSAL_IoGetAxiIdListForDeviceName return OSAL_RETURN_OK
 * -# Allocated buffer is freeed successfully
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_006_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForDeviceName_TC_06_030_006
 */
// [IoGetAxiIdListForDeviceName_TC_06_030_006]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForDeviceName_TC_06_030_006)
{
  size_t num_of_axi_ids = 0;
  osal_axi_bus_id_t *buffer;
  size_t num_of_max_entry = 11;
  size_t num_of_written_axi_ids = 1;

  buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);

  // test: valid devie name, but device doesn'thave axi_id
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForDeviceName(device_info_test_data[1].test_device_name, buffer, num_of_max_entry, &num_of_written_axi_ids), OSAL_RETURN_OK) << "Test06_030_006 failed";
  EXPECT_EQ(num_of_written_axi_ids, 0U);

  free(buffer);
}
// [IoGetAxiIdListForDeviceName_TC_06_030_006]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForDeviceName_TC_06_030_007 IoAXIBusNameApiTest.IoGetAxiIdListForDeviceName_TC_06_030_007
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_007_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_007_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_030_007
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_007_rr Related Requirement(s)
 * \#329830
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_007_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_007_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForDeviceName function
 * - Test06_030_007: Get the list of AXI IDs with unknow device name
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_007_tpts Test Procedure / Test Steps
 * -# Allocate buffer which size is equal to "sizeof(osal_axi_bus_id_t) * 11"
 * -# Get the list of AXI Bus IDs by using R_OSAL_IoGetAxiIdListForDeviceName with unknow device name
 * -# Free allocated buffer above
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_007_ereb Expected Result/Behavior
 * -# The allocation is successful
 * -# R_OSAL_IoGetAxiIdListForDeviceName return OSAL_RETURN_UNKNOWN_NAME
 * -# Allocated buffer is freeed successfully
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_007_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForDeviceName_TC_06_030_007
 */
// [IoGetAxiIdListForDeviceName_TC_06_030_007]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForDeviceName_TC_06_030_007)
{
  size_t num_of_axi_ids = 0;
  osal_axi_bus_id_t *buffer;
  size_t num_of_max_entry = 11;
  size_t num_of_written_axi_ids = 0;

  buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);

  // test: invalid devie name
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForDeviceName(device_info_test_data[2].test_device_name, buffer, num_of_max_entry, &num_of_written_axi_ids), OSAL_RETURN_UNKNOWN_NAME) << "Test06_030_007 failed";

  free(buffer);
}
// [IoGetAxiIdListForDeviceName_TC_06_030_007]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForDeviceName_TC_06_030_008 IoAXIBusNameApiTest.IoGetAxiIdListForDeviceName_TC_06_030_008
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_008_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_008_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S2_A_TC_06_030_008
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_008_rr Related Requirement(s)
 * \#329830
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_008_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Device handle was opened
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_008_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForDeviceName function
 * - Test06_030_008: Confirm state transition of R_OSAL_IoGetAxiIdListForDeviceName
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_008_tpts Test Procedure / Test Steps
 * -# Allocate buffer which size is equal to "sizeof(osal_axi_bus_id_t) * 11"
 * -# Close opened devices by using R_OSAL_IoDeviceClose
 * -# Deinit OSAL APIs by using R_OSAL_Deinitialize
 * -# Call R_OSAL_IoGetAxiIdListForDeviceName
 * -# Init OSAL APIs by using R_OSAL_Initialize
 * -# Open required devices again by using R_OSAL_IoDeviceOpen
 * -# Free allocated buffer above
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_008_ereb Expected Result/Behavior
 * -# The allocation is successful
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_Deinitialize return OSAL_RETURN_OK
 * -# R_OSAL_IoGetAxiIdListForDeviceName return OSAL_RETURN_STATE
 * -# R_OSAL_Initialize return OSAL_RETURN_OK
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK
 * -# Allocated buffer is freeed successfully
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_008_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForDeviceName_TC_06_030_008
 */
// [IoGetAxiIdListForDeviceName_TC_06_030_008]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForDeviceName_TC_06_030_008)
{
  size_t num_of_axi_ids = 0;
  osal_axi_bus_id_t *buffer;
  size_t num_of_max_entry = 11;
  size_t num_of_written_axi_ids = 0;

  buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);

  // pre-process of testing
  EXPECT_EQ(R_OSAL_IoDeviceClose(device_info_test_data[0].test_hndl), OSAL_RETURN_OK);
  EXPECT_EQ(R_OSAL_IoDeviceClose(device_info_test_data[1].test_hndl), OSAL_RETURN_OK);
  EXPECT_EQ(R_OSAL_IoDeviceClose(device_info_test_data[3].test_hndl), OSAL_RETURN_OK);
  EXPECT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_OK) << "pre-process failed";

  // test: Don't execute initialize
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForDeviceName(device_info_test_data[1].test_device_name, buffer, num_of_max_entry, &num_of_written_axi_ids), OSAL_RETURN_STATE) << "Test06_030_008 failed";

  // post-process of testing
  EXPECT_EQ(R_OSAL_Initialize(), OSAL_RETURN_OK) << "post-process failed";
  EXPECT_EQ(R_OSAL_IoDeviceOpen(device_info_test_data[0].test_device_name, &device_info_test_data[0].test_hndl), OSAL_RETURN_OK);
  EXPECT_EQ(R_OSAL_IoDeviceOpen(device_info_test_data[1].test_device_name, &device_info_test_data[1].test_hndl), OSAL_RETURN_OK);
  EXPECT_EQ(R_OSAL_IoDeviceOpen(device_info_test_data[3].test_device_name, &device_info_test_data[3].test_hndl), OSAL_RETURN_OK);

  free(buffer);
}
// [IoGetAxiIdListForDeviceName_TC_06_030_008]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForAxiBusName_TC_06_028_012 IoAXIBusNameApiTest.IoGetAxiIdListForAxiBusName_TC_06_028_012
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_012_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_012_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_028_012
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_012_rr Related Requirement(s)
 * \#322528 \#326328
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_012_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_012_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForAxiBusName function
 * - Test06_028_012: Get the required size of AXI IDs list
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_012_tpts Test Procedure / Test Steps
 * -# Get the required size of AXI IDs list by using R_OSAL_IoGetAxiIdListForAxiBusName with p_array is NULL and num_of_max_entry is 0<br>
 * The axi_bus_name that specify contains all uTLBs for the bus domain
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_012_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetAxiIdListForAxiBusName return OSAL_RETURN_OK_RQST.<br>
 * Value of "num_of_written_axi_ids" is equal to the expected value
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_012_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForAxiBusName_TC_06_028_012
 */
// [IoGetAxiIdListForAxiBusName_TC_06_028_012]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForAxiBusName_TC_06_028_012)
{
  size_t num_of_written_axi_ids = 0;

  // test: valid parameters
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForAxiBusName(axi_bus_name_test_data[0].test_bus_name, NULL, 0, &num_of_written_axi_ids), OSAL_RETURN_OK_RQST) << "Test06_028_012 failed";
  EXPECT_EQ(axi_bus_name_test_data[0].axi_ids, num_of_written_axi_ids) << "Test06_028_012 failed";
}
// [IoGetAxiIdListForAxiBusName_TC_06_028_012]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForAxiBusName_TC_06_028_013 IoAXIBusNameApiTest.IoGetAxiIdListForAxiBusName_TC_06_028_013
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_013_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_013_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_028_013
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_013_rr Related Requirement(s)
 * \#322528 \#326328
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_013_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_013_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForAxiBusName function
 * - Test06_028_013: Get the required size of AXI IDs list
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_013_tpts Test Procedure / Test Steps
 * -# Get the required size of AXI IDs list by using R_OSAL_IoGetAxiIdListForAxiBusName with p_array is NULL and num_of_max_entry is 0<br>
 * The axi_bus_name that specify contains partial uTLBs for the bus domain
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_013_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetAxiIdListForAxiBusName return OSAL_RETURN_OK_RQST.<br>
 * Value of "num_of_written_axi_ids" is equal to the expected value
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_013_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForAxiBusName_TC_06_028_013
 */
// [IoGetAxiIdListForAxiBusName_TC_06_028_013]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForAxiBusName_TC_06_028_013)
{
  size_t num_of_written_axi_ids = 0;

  // test: valid parameters (OSAL_RETURN_OK_RQST)
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForAxiBusName(axi_bus_name_test_data[6].test_bus_name, NULL, 0, &num_of_written_axi_ids), OSAL_RETURN_OK_RQST) << "Test06_028_013 failed";
  EXPECT_EQ(axi_bus_name_test_data[6].axi_ids, num_of_written_axi_ids) << "Test06_028_013 failed";
}
// [IoGetAxiIdListForAxiBusName_TC_06_028_013]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForAxiBusName_TC_06_028_014 IoAXIBusNameApiTest.IoGetAxiIdListForAxiBusName_TC_06_028_014
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_014_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_014_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_028_014
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_014_rr Related Requirement(s)
 * \#349694 \#326328
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_014_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_014_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForAxiBusName function
 * - Test06_028_014: Get the list of AXI IDs with buffer is NULL and array_size is not 0
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_014_tpts Test Procedure / Test Steps
 * -# Get the list of AXI Bus IDs by using R_OSAL_IoGetAxiIdListForAxiBusName with buffer is NULL and array_size is not 0
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_014_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetAxiIdListForAxiBusName return OSAL_RETURN_PAR.
 * @subsection IoGetAxiIdListForAxiBusName_TC_06_028_014_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForAxiBusName_TC_06_028_014
 */
// [IoGetAxiIdListForAxiBusName_TC_06_028_014]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForAxiBusName_TC_06_028_014)
{
  size_t num_of_written_axi_ids = 0;

  // abnormal case: p_array is NULL and array_size is not 0
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForAxiBusName(axi_bus_name_test_data[0].test_bus_name, NULL, ARRAY_SIZE_IO_TEST, &num_of_written_axi_ids), OSAL_RETURN_PAR) << "Test06_028_014 failed";
}
// [IoGetAxiIdListForAxiBusName_TC_06_028_014]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForDeviceHdl_TC_06_029_009 IoAXIBusNameApiTest.IoGetAxiIdListForDeviceHdl_TC_06_029_009
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_009_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_009_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_029_009
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_009_rr Related Requirement(s)
 * \#329828
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_009_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Device handle was opened
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_009_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForDeviceHdl function
 * - Test06_029_009: Get the required size of AXI IDs list
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_009_tpts Test Procedure / Test Steps
 * -# Get the required size of AXI IDs list by using R_OSAL_IoGetAxiIdListForDeviceHdl with p_array is NULL and num_of_max_entry is 0<br>
 * Input device has axi ids
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_009_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetAxiIdListForDeviceHdl return OSAL_RETURN_OK_RQST.<br>
 * Value of "num_of_written_axi_ids" is equal to the expected value
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_009_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForDeviceHdl_TC_06_029_009
 */
// [IoGetAxiIdListForDeviceHdl_TC_06_029_009]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForDeviceHdl_TC_06_029_009)
{
  size_t num_of_written_axi_ids = 0;

  // test: valid parameters (OSAL_RETURN_OK_RQST)
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForDeviceHdl(device_info_test_data[0].test_hndl, NULL, 0, &num_of_written_axi_ids), OSAL_RETURN_OK_RQST) << "Test06_029_009 failed";
  EXPECT_EQ(device_info_test_data[0].axi_ids, num_of_written_axi_ids) << "Test06_029_009 failed";
}
// [IoGetAxiIdListForDeviceHdl_TC_06_029_009]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForDeviceHdl_TC_06_029_010 IoAXIBusNameApiTest.IoGetAxiIdListForDeviceHdl_TC_06_029_010
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_010_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_010_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_029_010
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_010_rr Related Requirement(s)
 * \#329828
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_010_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Device handle was opened
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_010_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForDeviceHdl function
 * - Test06_029_010: Get the required size of AXI IDs list
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_010_tpts Test Procedure / Test Steps
 * -# Get the required size of AXI IDs list by using R_OSAL_IoGetAxiIdListForDeviceHdl with p_array is NULL and num_of_max_entry is 0<br>
 * input device doesn'thave axi ids
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_010_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetAxiIdListForDeviceHdl return OSAL_RETURN_OK_RQST.<br>
 * Value of "num_of_written_axi_ids" is equal to the expected value
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_010_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForDeviceHdl_TC_06_029_010
 */
// [IoGetAxiIdListForDeviceHdl_TC_06_029_010]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForDeviceHdl_TC_06_029_010)
{
  size_t num_of_written_axi_ids = 0;

  // test: valid parameters (OSAL_RETURN_OK_RQST)
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForDeviceHdl(device_info_test_data[1].test_hndl, NULL, 0, &num_of_written_axi_ids), OSAL_RETURN_OK_RQST) << "Test06_029_010 failed";
  EXPECT_EQ(device_info_test_data[1].axi_ids, num_of_written_axi_ids) << "Test06_029_010 failed";
}
// [IoGetAxiIdListForDeviceHdl_TC_06_029_010]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForDeviceHdl_TC_06_029_011 IoAXIBusNameApiTest.IoGetAxiIdListForDeviceHdl_TC_06_029_011
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_011_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_011_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_029_011
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_011_rr Related Requirement(s)
 * \#329828
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_011_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_011_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForDeviceHdl function
 * - Test06_029_011: Get the required size of AXI IDs list with invalid device handle
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_011_tpts Test Procedure / Test Steps
 * -# Get the required size of AXI IDs list by using R_OSAL_IoGetAxiIdListForDeviceHdl with p_array is NULL, num_of_max_entry is 0 and invalid device handle
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_011_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetAxiIdListForDeviceHdl return OSAL_RETURN_HANDLE.<br>
 * Value of "num_of_written_axi_ids" is equal to the expected value
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_011_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForDeviceHdl_TC_06_029_011
 */
// [IoGetAxiIdListForDeviceHdl_TC_06_029_011]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForDeviceHdl_TC_06_029_011)
{
  size_t num_of_written_axi_ids = 0;

  // test: invalid device handle (parameters are OSAL_RETURN_OK_RQST)
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForDeviceHdl(device_info_test_data[2].test_hndl, NULL, 0, &num_of_written_axi_ids), OSAL_RETURN_HANDLE) << "Test06_029_011 failed";
  EXPECT_EQ(device_info_test_data[2].axi_ids, num_of_written_axi_ids) << "Test06_029_011 failed";
}
// [IoGetAxiIdListForDeviceHdl_TC_06_029_011]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForDeviceHdl_TC_06_029_012 IoAXIBusNameApiTest.IoGetAxiIdListForDeviceHdl_TC_06_029_012
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_012_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_012_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S2_A_TC_06_029_012
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_012_rr Related Requirement(s)
 * \#329828
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_012_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Device handle was opened
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_012_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForDeviceHdl function
 * - Test06_029_012: Get the list of Axi ID with the device was already closed by R_OSAL_IoDeviceClose().
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_012_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs for the given device handle "device_info_test_data[0].test_hndl" by using R_OSAL_IoGetNumOfAxiIdsForDeviceHdl
 * -# Allocate buffer which size is equal to "sizeof(osal_axi_bus_id_t) * num_of_max_entry"
 * -# Close the device by using R_OSAL_IoDeviceClose
 * -# Get the list of AXI Bus IDs related to device handle "device_info_test_data[0].test_hndl" by using R_OSAL_IoGetAxiIdListForDeviceHdl
 * -# Free allocated buffer above
 * -# Re-open the device by using R_OSAL_IoDeviceOpen for next test case.
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_012_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForDeviceHdl return OSAL_RETURN_OK.<br>
 * The value of "num_of_max_entry" is equal to the expected value
 * -# The allocation is successful
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * -# R_OSAL_IoGetAxiIdListForDeviceHdl return OSAL_RETURN_HANDLE.
 * -# Allocated buffer is freeed successfully.
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.
 * @subsection IoGetAxiIdListForDeviceHdl_TC_06_029_012_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForDeviceHdl_TC_06_029_012
 */
// [IoGetAxiIdListForDeviceHdl_TC_06_029_012]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForDeviceHdl_TC_06_029_012)
{
  osal_axi_bus_id_t *buffer;
  size_t num_of_max_entry = 0;
  size_t num_of_written_axi_ids = 0;

  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForDeviceHdl(device_info_test_data[0].test_hndl, &num_of_max_entry), OSAL_RETURN_OK);
  EXPECT_EQ(device_info_test_data[0].axi_ids, num_of_max_entry);
  buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);

  // test:  Device was already closed by R_OSAL_IoDeviceClose().
  EXPECT_EQ(R_OSAL_IoDeviceClose(device_info_test_data[0].test_hndl), OSAL_RETURN_OK);
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForDeviceHdl(device_info_test_data[0].test_hndl, buffer, num_of_max_entry, &num_of_written_axi_ids), OSAL_RETURN_HANDLE) << "Test06_029_012 failed";

  free(buffer);
  // Re-open device for next test case.
  EXPECT_EQ(R_OSAL_IoDeviceOpen(device_info_test_data[0].test_device_name, &device_info_test_data[0].test_hndl), OSAL_RETURN_OK);
}
// [IoGetAxiIdListForDeviceHdl_TC_06_029_012]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForDeviceName_TC_06_030_009 IoAXIBusNameApiTest.IoGetAxiIdListForDeviceName_TC_06_030_009
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_009_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_009_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_030_009
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_009_rr Related Requirement(s)
 * \#329830
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_009_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_009_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForDeviceName function
 * - Test06_030_009: Get the required size of AXI IDs list
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_009_tpts Test Procedure / Test Steps
 * -# Get the required size of AXI IDs list by using R_OSAL_IoGetAxiIdListForDeviceName with p_array is NULL and num_of_max_entry is 0<br>
 * input device have axi ids
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_009_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetAxiIdListForDeviceName return OSAL_RETURN_OK_RQST.<br>
 * Value of "num_of_written_axi_ids" is equal to the expected value
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_009_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForDeviceName_TC_06_030_009
 */
// [IoGetAxiIdListForDeviceName_TC_06_030_009]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForDeviceName_TC_06_030_009)
{
  size_t num_of_written_axi_ids = 0;

  // test: valid parameters (OSAL_RETURN_OK_RQST)
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForDeviceName(device_info_test_data[0].test_device_name, NULL, 0, &num_of_written_axi_ids), OSAL_RETURN_OK_RQST) << "Test06_030_009 failed";
  EXPECT_EQ(device_info_test_data[0].axi_ids, num_of_written_axi_ids) << "Test06_030_009 failed";
}
// [IoGetAxiIdListForDeviceName_TC_06_030_009]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForDeviceName_TC_06_030_010 IoAXIBusNameApiTest.IoGetAxiIdListForDeviceName_TC_06_030_010
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_010_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_010_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_030_010
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_010_rr Related Requirement(s)
 * \#329830
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_010_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_010_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForDeviceName function
 * - Test06_030_010: Get the required size of AXI IDs list
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_010_tpts Test Procedure / Test Steps
 * -# Get the required size of AXI IDs list by using R_OSAL_IoGetAxiIdListForDeviceName with p_array is NULL and num_of_max_entry is 0<br>
 * input device doesn'thave axi ids
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_010_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetAxiIdListForDeviceName return OSAL_RETURN_OK_RQST.<br>
 * Value of "num_of_written_axi_ids" is equal to the expected value
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_010_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForDeviceName_TC_06_030_010
 */
// [IoGetAxiIdListForDeviceName_TC_06_030_010]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForDeviceName_TC_06_030_010)
{
  size_t num_of_written_axi_ids = 0;

  // test: valid parameters (OSAL_RETURN_OK_RQST)
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForDeviceName(device_info_test_data[1].test_device_name, NULL, 0, &num_of_written_axi_ids), OSAL_RETURN_OK_RQST) << "Test06_030_010 failed";
  EXPECT_EQ(device_info_test_data[1].axi_ids, num_of_written_axi_ids) << "Test06_030_010 failed";
}
// [IoGetAxiIdListForDeviceName_TC_06_030_010]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForDeviceName_TC_06_030_011 IoAXIBusNameApiTest.IoGetAxiIdListForDeviceName_TC_06_030_011
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_011_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_011_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_030_011
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_011_rr Related Requirement(s)
 * \#329830
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_011_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_011_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForDeviceName function
 * - Test06_030_011: Get the required size of AXI IDs list with invalid device name
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_011_tpts Test Procedure / Test Steps
 * -# Get the required size of AXI IDs list by using R_OSAL_IoGetAxiIdListForDeviceName with p_array is NULL, num_of_max_entry is 0 and invalid device name
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_011_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetAxiIdListForDeviceName return OSAL_RETURN_UNKNOWN_NAME.<br>
 * Value of "num_of_written_axi_ids" is equal to the expected value
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_011_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForDeviceName_TC_06_030_011
 */
// [IoGetAxiIdListForDeviceName_TC_06_030_011]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForDeviceName_TC_06_030_011)
{
  size_t num_of_written_axi_ids = 0;

  // test: invalid device name (parameters are OSAL_RETURN_OK_RQST)
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForDeviceName(device_info_test_data[2].test_device_name, NULL, 0, &num_of_written_axi_ids), OSAL_RETURN_UNKNOWN_NAME) << "Test06_030_011 failed";
  EXPECT_EQ(device_info_test_data[2].axi_ids, num_of_written_axi_ids) << "Test06_030_011 failed";
}
// [IoGetAxiIdListForDeviceName_TC_06_030_011]

/**
 * @page AXI_BUS_NAME_TEST AXI Bus Name API Test
 * @section IoAXIBusNameApiTest_IoGetAxiIdListForDeviceName_TC_06_030_012 IoAXIBusNameApiTest.IoGetAxiIdListForDeviceName_TC_06_030_012
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_012_tsi Test Suite ID.
 * IoAXIBusNameApiTest
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_012_tci Test Case ID
 * IoGetAxiBusIdFromDeviceName_S0_A_TC_06_030_012
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_012_rr Related Requirement(s)
 * \#329830
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_012_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_012_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetAxiIdListForDeviceName function
 * - Test06_030_012: Get the list of AXI IDs with device name is NULL
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_012_tpts Test Procedure / Test Steps
 * -# Get the number of AXI IDs for the given device name "device_info_test_data[0].test_device_name" by using R_OSAL_IoGetNumOfAxiIdsForDeviceName
 * -# Allocate buffer which size is equal to "sizeof(osal_axi_bus_id_t) * num_of_max_entry"
 * -# Get the list of AXI Bus IDs by using R_OSAL_IoGetAxiIdListForDeviceName with device name is NULL
 * -# Free allocated buffer above
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_012_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfAxiIdsForDeviceName return OSAL_RETURN_OK.<br>
 * The value of "num_of_max_entry" is equal to the expected value
 * -# The allocation is successful
 * -# R_OSAL_IoGetAxiIdListForDeviceName return OSAL_RETURN_PAR
 * -# Allocated buffer is freeed successfully
 * @subsection IoGetAxiIdListForDeviceName_TC_06_030_012_testcase Test Case Example
 * @snippet test_io_axi_bus_name.cpp IoGetAxiIdListForDeviceName_TC_06_030_012
 */
// [IoGetAxiIdListForDeviceName_TC_06_030_012]
TEST_F(IoAXIBusNameApiTest, IoGetAxiIdListForDeviceName_TC_06_030_012)
{
  osal_axi_bus_id_t *buffer;
  size_t num_of_max_entry = 0;
  size_t num_of_written_axi_ids = 0;

  EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForDeviceName(device_info_test_data[0].test_device_name, &num_of_max_entry), OSAL_RETURN_OK);
  EXPECT_EQ(device_info_test_data[0].axi_ids, num_of_max_entry);
  buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);

  // abnormal case: device name is NULL
  EXPECT_EQ(R_OSAL_IoGetAxiIdListForDeviceName(NULL, buffer, num_of_max_entry, &num_of_written_axi_ids), OSAL_RETURN_PAR) << "Test06_030_001 failed";

  free(buffer);
}
// [IoGetAxiIdListForDeviceName_TC_06_030_012]
