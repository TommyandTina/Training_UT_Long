/***********************************************************************************************************************
* Copyright [2021-2024] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    test_io.cpp
* Version :      3.7.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL IO Manager API Functional Test (From the perspective of OSAL API Specification)
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           2.0.0     29.10.2021    Create New
 *           2.0.1     08.11.2021    Add Support Device Definition
 *           2.0.2     11.11.2021    Add set_clock_off each tests
 *           2.0.3     15.11.2021    Update Test Name
 *                                   openClose -> openClose_TC_12_001_009
 *           2.0.4     18.11.2021    Fix Typo
 *           3.0.0     02.02.2022    Add V4H/S4 device definition
 *                                   Change definition name SUPPORT -> UNSUPPORT and "#if defined" -> "#if !defined"
 *                     15.02.2022    Apply new device and its setting definition
 *                     28.02.2022    Change to use device ID definition directly instead of getting from "device_list"
 *                     04.03.2022    Update S4 and V4H Device information
 *                     08.03.2022    Apply new/updated device definition related to interrupt context test
 *                                   Apply updated allocate_setup_data_memory, setup_hw_register and deallocate_data_memory
 *           3.1.0     23.03.2022    Add tests of IoBlockRead8/16/32/64 and IoBlockWrite8/16/32/64 with input size is 0.
 *                                      - IoBlockRead8_TC_06_009_010, IoBlockWrite8_TC_06_017_010
 *                                      - IoBlockRead16_TC_06_010_010, IoBlockWrite16_TC_06_018_010
 *                                      - IoBlockRead32_TC_06_011_010, IoBlockWrite32_TC_06_019_010
 *                                      - IoBlockRead64_TC_06_012_010, IoBlockWrite64_TC_06_020_010
 *                                   Update tests will be disabled in unsupported 8/16/32/64 bit platform
 *           3.1.1     04.04.2022    Update Synchronization method for IoDeviceClose_TC_06_004_006
 *           3.1.2     20.04.2022    Update V4H support device (WWDT/ICUMX_CMD/VDSP00/VIP_WRAP)
 *           3.1.3     26.05.2022    Add read/write-back initialized value to IO OSALINTTESTS to reset target register
 *                                   Update sequence to read/write-back init value for all tests that use software reset
 *                                   Remove clean_register function
 *                                   Add read/write-back initialized value for to reset target register to the original
 *           3.1.4     07.06.2022    Update set_clock_off and set_clock_on function
 *                     08.06.2022    Update Test Target Device of IoDeviceClose_TC_06_004_006
 *           3.2.4     08.26.2022    Add IO Tests about one handle access from Multi threads
 *                                      - IoDeviceOpen_TC_06_003_008
 *                                      - IoWrite8_TC_06_013_008, IoWrite16_TC_06_014_008
 *                                      - IoWrite32_TC_06_015_008, IoWrite64_TC_06_016_008
 *                                      - IoBlockWrite8_TC_06_017_011, IoBlockWrite16_TC_06_018_011
 *                                      - IoBlockWrite32_TC_06_019_011, IoBlockWrite64_TC_06_020_011
 *           3.3.0     04.01.2023   Add new IO Tests to cover OSAL API Specification
 *                                      - IoGetNumOfDevices_TC_06_001_006, IoGetDeviceList_TC_06_002_011
 *                                  Update test code & doxygen of test cases
 *                                      - IoDeviceOpen_TC_06_003_002, IoDeviceOpen_TC_06_003_003
 *                                      - IoDeviceOpen_TC_06_003_007
 *           3.4.0     20.01.2023   Update Device information for ICCOM
 *           3.5.0     20.01.2023   Update Device information for imp_spmc(V4H2) about each OS.
 *           3.6.0     01.02.2023   Add new test cases as below:
 *                                      - IoGetNumOfDevices_TC_06_001_007
 *                                      - IoGetDeviceList_TC_06_002_012
 *           3.6.1     08.02.2023   Append terminating character to string
 *                                      - IoGetNumOfDevices_TC_06_001_006, IoGetNumOfDevices_TC_06_001_007
 *                                      - IoGetDeviceList_TC_06_002_011, IoGetDeviceList_TC_06_002_012
 *           3.6.2     09.02.2023   Add Device information for ECM
 *           3.7.0     11.09.2023   Add Device definition of V4M
***********************************************************************************************************************/
#include "test_common.hpp"

#if !defined(OSAL_TEST_FRAMEWORK)
using namespace std;
#else
using namespace OsalFWHelper;
#endif

/* armclang will be warnig, when specified OsalApiStateTest to TEST_F */
/* W.A Derive a class and replace it by define */
class OsalApiStateTest_IO : public OsalApiStateTest
{
    /* There is no need for member variables and member functions.
       We added them just for the sake of adding a test class. */
};
#define OsalApiStateTest OsalApiStateTest_IO


/* armclang will be warnig, when specified OSALINTTESTS to TEST_F */
/* W.A Derive a class and replace it by define */
class OSALINTTESTS_IO : public OSALINTTESTS
{
    /* There is no need for member variables and member functions.
       We added them just for the sake of adding a test class. */
};
#define OSALINTTESTS OSALINTTESTS_IO

#if defined (TEST_SIL)
/* Simulator only supports 32bit bus */
#undef SUPPORT_64BIT_BUS

#define IoRead8_TC_06_005_001 DISABLED_IoRead8_TC_06_005_001
#define IoRead8_TC_06_005_008 DISABLED_IoRead8_TC_06_005_008
#define IoWrite8_TC_06_013_001 DISABLED_IoWrite8_TC_06_013_001
#define IoWrite8_TC_06_013_007 DISABLED_IoWrite8_TC_06_013_007
#define IoWrite8_TC_06_013_008 DISABLED_IoWrite8_TC_06_013_008
#define IoRead16_TC_06_006_001 DISABLED_IoRead16_TC_06_006_001
#define IoRead16_TC_06_006_008 DISABLED_IoRead16_TC_06_006_008
#define IoWrite16_TC_06_014_001 DISABLED_IoWrite16_TC_06_014_001
#define IoWrite16_TC_06_014_007 DISABLED_IoWrite16_TC_06_014_007
#define IoWrite16_TC_06_014_008 DISABLED_IoWrite16_TC_06_014_008
#define IoBlockRead16_TC_06_010_002 DISABLED_IoBlockRead16_TC_06_010_002
#define IoBlockWrite16_TC_06_018_002 DISABLED_IoBlockWrite16_TC_06_018_002
#define IoRead64_TC_06_008_001 DISABLED_IoRead64_TC_06_008_001
#define IoRead64_TC_06_008_008 DISABLED_IoRead64_TC_06_008_008
#define IoWrite64_TC_06_016_001 DISABLED_IoWrite64_TC_06_016_001
#define IoWrite64_TC_06_016_007 DISABLED_IoWrite64_TC_06_016_007
#define IoWrite64_TC_06_016_008 DISABLED_IoWrite64_TC_06_016_008
#define IoBlockWrite8_TC_06_017_011 DISABLED_IoBlockWrite8_TC_06_017_011
#define IoBlockWrite16_TC_06_018_011 DISABLED_IoBlockWrite16_TC_06_018_011
#define IoBlockRead64_TC_06_012_002 DISABLED_IoBlockRead64_TC_06_012_002
#define IoBlockWrite64_TC_06_020_002 DISABLED_IoBlockWrite64_TC_06_020_002
#define IoBlockWrite64_TC_06_020_011 DISABLED_IoBlockWrite64_TC_06_020_011
#endif

#if !defined (SUPPORT_8BIT_BUS)
#define IoBlockRead8_TC_06_009_001 DISABLED_IoBlockRead8_TC_06_009_001
#define IoBlockRead8_TC_06_009_009 DISABLED_IoBlockRead8_TC_06_009_009
#define IoBlockRead8_TC_06_009_010 DISABLED_IoBlockRead8_TC_06_009_010
#define IoBlockWrite8_TC_06_017_001 DISABLED_IoBlockWrite8_TC_06_017_001
#define IoBlockWrite8_TC_06_017_009 DISABLED_IoBlockWrite8_TC_06_017_009
#define IoBlockWrite8_TC_06_017_010 DISABLED_IoBlockWrite8_TC_06_017_010
#define IoBlockWrite8_TC_06_017_011 DISABLED_IoBlockWrite8_TC_06_017_011
#endif

#if !defined (SUPPORT_16BIT_BUS)
#define IoBlockRead16_TC_06_010_001 DISABLED_IoBlockRead16_TC_06_010_001
#define IoBlockRead16_TC_06_010_009 DISABLED_IoBlockRead16_TC_06_010_009
#define IoBlockRead16_TC_06_010_010 DISABLED_IoBlockRead16_TC_06_010_010
#define IoBlockWrite16_TC_06_018_001 DISABLED_IoBlockWrite16_TC_06_018_001
#define IoBlockWrite16_TC_06_018_009 DISABLED_IoBlockWrite16_TC_06_018_009
#define IoBlockWrite16_TC_06_018_010 DISABLED_IoBlockWrite16_TC_06_018_010
#define IoBlockWrite16_TC_06_018_011 DISABLED_IoBlockWrite16_TC_06_018_011
#endif

#if !defined (SUPPORT_32BIT_BUS)
#define IoBlockRead32_TC_06_011_001 DISABLED_IoBlockRead32_TC_06_011_001
#define IoBlockRead32_TC_06_011_009 DISABLED_IoBlockRead32_TC_06_011_009
#define IoBlockRead32_TC_06_011_010 DISABLED_IoBlockRead32_TC_06_011_010
#define IoBlockWrite32_TC_06_019_001 DISABLED_IoBlockWrite32_TC_06_019_001
#define IoBlockWrite32_TC_06_019_009 DISABLED_IoBlockWrite32_TC_06_019_009
#define IoBlockWrite32_TC_06_019_010 DISABLED_IoBlockWrite32_TC_06_019_010
#define IoBlockWrite32_TC_06_019_011 DISABLED_IoBlockWrite32_TC_06_019_011
#endif

#if !defined (SUPPORT_64BIT_BUS)
#define IoBlockRead64_TC_06_012_001 DISABLED_IoBlockRead64_TC_06_012_001
#define IoBlockRead64_TC_06_012_009 DISABLED_IoBlockRead64_TC_06_012_009
#define IoBlockRead64_TC_06_012_010 DISABLED_IoBlockRead64_TC_06_012_010
#define IoBlockWrite64_TC_06_020_001 DISABLED_IoBlockWrite64_TC_06_020_001
#define IoBlockWrite64_TC_06_020_009 DISABLED_IoBlockWrite64_TC_06_020_009
#define IoBlockWrite64_TC_06_020_010 DISABLED_IoBlockWrite64_TC_06_020_010
#define IoBlockWrite64_TC_06_020_011 DISABLED_IoBlockWrite64_TC_06_020_011
#endif

/**
 * @page IO_TEST IO API Test
 * @test
 * @test @link IOTEST_openClose_TC_12_001_009 @endlink
 * @test @link IOApiTest_IoGetNumOfDevices_TC_06_001_001 @endlink
 * @test @link IOApiTest_IoGetNumOfDevices_TC_06_001_002 @endlink
 * @test @link IOApiTest_IoGetNumOfDevices_TC_06_001_003 @endlink
 * @test @link IOApiTest_IoGetNumOfDevices_TC_06_001_004 @endlink
 * @test @link OsalApiStateTest_IoGetNumOfDevices_TC_06_001_005 @endlink
 * @test @link IOApiTest_IoGetNumOfDevices_TC_06_001_006 @endlink
 * @test @link IOApiTest_IoGetNumOfDevices_TC_06_001_007 @endlink
 * @test @link IOApiTest_IoGetDeviceList_TC_06_002_001 @endlink
 * @test @link IOApiTest_IoGetDeviceList_TC_06_002_002 @endlink
 * @test @link IOApiTest_IoGetDeviceList_TC_06_002_003 @endlink
 * @test @link IOApiTest_IoGetDeviceList_TC_06_002_004 @endlink
 * @test @link IOApiTest_IoGetDeviceList_TC_06_002_005 @endlink
 * @test @link IOApiTest_IoGetDeviceList_TC_06_002_006 @endlink
 * @test @link IOApiTest_IoGetDeviceList_TC_06_002_007 @endlink
 * @test @link IOApiTest_IoGetDeviceList_TC_06_002_008 @endlink
 * @test @link IOApiTest_IoGetDeviceList_TC_06_002_009 @endlink
 * @test @link OsalApiStateTest_IoGetDeviceList_TC_06_002_010 @endlink
 * @test @link IOApiTest_IoGetDeviceList_TC_06_002_011 @endlink
 * @test @link IOApiTest_IoGetDeviceList_TC_06_002_012 @endlink
 * @test @link IOApiTest_IoDeviceOpen_TC_06_003_001 @endlink
 * @test @link IOApiTest_IoDeviceOpen_TC_06_003_002 @endlink
 * @test @link IOApiTest_IoDeviceOpen_TC_06_003_003 @endlink
 * @test @link IOApiTest_IoDeviceOpen_TC_06_003_004 @endlink
 * @test @link IOApiTest_IoDeviceOpen_TC_06_003_005 @endlink
 * @test @link IOApiTest_IoDeviceOpen_TC_06_003_006 @endlink
 * @test @link OsalApiStateTest_IoDeviceOpen_TC_06_003_007 @endlink
 * @test @link IOApiTest_IoDeviceOpen_TC_06_003_008 @endlink
 * @test @link IOApiTest_IoDeviceClose_TC_06_004_001 @endlink
 * @test @link IOApiTest_IoDeviceClose_TC_06_004_002 @endlink
 * @test @link IOApiTest_IoDeviceClose_TC_06_004_003 @endlink
 * @test @link IOApiTest_IoDeviceClose_TC_06_004_004 @endlink
 * @test @link OsalApiStateTest_IoDeviceClose_TC_06_004_005 @endlink
 * @test @link IOApiTest_IoDeviceClose_TC_06_004_006 @endlink
 * @test @link IOApiTest_IoRead8_TC_06_005_001 @endlink
 * @test @link IOApiTest_IoRead8_TC_06_005_003 @endlink
 * @test @link IOApiTest_IoRead8_TC_06_005_004 @endlink
 * @test @link IOApiTest_IoRead8_TC_06_005_005 @endlink
 * @test @link IOApiTest_IoRead8_TC_06_005_006 @endlink
 * @test @link OsalApiStateTest_IoRead8_TC_06_005_007 @endlink
 * @test @link OSALINTTESTS_IoRead8_TC_06_005_008 @endlink
 * @test @link IOApiTest_IoRead16_TC_06_006_001 @endlink
 * @test @link IOApiTest_IoRead16_TC_06_006_002 @endlink
 * @test @link IOApiTest_IoRead16_TC_06_006_003 @endlink
 * @test @link IOApiTest_IoRead16_TC_06_006_004 @endlink
 * @test @link IOApiTest_IoRead16_TC_06_006_005 @endlink
 * @test @link IOApiTest_IoRead16_TC_06_006_006 @endlink
 * @test @link OsalApiStateTest_IoRead16_TC_06_006_007 @endlink
 * @test @link OSALINTTESTS_IoRead16_TC_06_006_008 @endlink
 * @test @link IOApiTest_IoRead32_TC_06_007_001 @endlink
 * @test @link IOApiTest_IoRead32_TC_06_007_002 @endlink
 * @test @link IOApiTest_IoRead32_TC_06_007_003 @endlink
 * @test @link IOApiTest_IoRead32_TC_06_007_004 @endlink
 * @test @link IOApiTest_IoRead32_TC_06_007_005 @endlink
 * @test @link IOApiTest_IoRead32_TC_06_007_006 @endlink
 * @test @link OsalApiStateTest_IoRead32_TC_06_007_007 @endlink
 * @test @link OSALINTTESTS_IoRead32_TC_06_007_008 @endlink
 * @test @link IOApiTest_IoRead64_TC_06_008_001 @endlink
 * @test @link IOApiTest_IoRead64_TC_06_008_002 @endlink
 * @test @link IOApiTest_IoRead64_TC_06_008_003 @endlink
 * @test @link IOApiTest_IoRead64_TC_06_008_004 @endlink
 * @test @link IOApiTest_IoRead64_TC_06_008_005 @endlink
 * @test @link IOApiTest_IoRead64_TC_06_008_006 @endlink
 * @test @link OsalApiStateTest_IoRead64_TC_06_008_007 @endlink
 * @test @link OSALINTTESTS_IoRead64_TC_06_008_008 @endlink
 * @test @link IOApiTest_IoWrite8_TC_06_013_001 @endlink
 * @test @link IOApiTest_IoWrite8_TC_06_013_003 @endlink
 * @test @link IOApiTest_IoWrite8_TC_06_013_004 @endlink
 * @test @link IOApiTest_IoWrite8_TC_06_013_005 @endlink
 * @test @link OsalApiStateTest_IoWrite8_TC_06_013_006 @endlink
 * @test @link OSALINTTESTS_IoWrite8_TC_06_013_007 @endlink
 * @test @link IOApiTest_IoWrite8_TC_06_013_008 @endlink
 * @test @link IOApiTest_IoWrite16_TC_06_014_001 @endlink
 * @test @link IOApiTest_IoWrite16_TC_06_014_002 @endlink
 * @test @link IOApiTest_IoWrite16_TC_06_014_003 @endlink
 * @test @link IOApiTest_IoWrite16_TC_06_014_004 @endlink
 * @test @link IOApiTest_IoWrite16_TC_06_014_005 @endlink
 * @test @link OsalApiStateTest_IoWrite16_TC_06_014_006 @endlink
 * @test @link OSALINTTESTS_IoWrite16_TC_06_014_007 @endlink
 * @test @link IOApiTest_IoWrite16_TC_06_014_008 @endlink
 * @test @link IOApiTest_IoWrite32_TC_06_015_001 @endlink
 * @test @link IOApiTest_IoWrite32_TC_06_015_002 @endlink
 * @test @link IOApiTest_IoWrite32_TC_06_015_003 @endlink
 * @test @link IOApiTest_IoWrite32_TC_06_015_004 @endlink
 * @test @link IOApiTest_IoWrite32_TC_06_015_005 @endlink
 * @test @link OsalApiStateTest_IoWrite32_TC_06_015_006 @endlink
 * @test @link OSALINTTESTS_IoWrite32_TC_06_015_007 @endlink
 * @test @link IOApiTest_IoWrite32_TC_06_015_008 @endlink
 * @test @link IOApiTest_IoWrite64_TC_06_016_001 @endlink
 * @test @link IOApiTest_IoWrite64_TC_06_016_002 @endlink
 * @test @link IOApiTest_IoWrite64_TC_06_016_003 @endlink
 * @test @link IOApiTest_IoWrite64_TC_06_016_004 @endlink
 * @test @link IOApiTest_IoWrite64_TC_06_016_005 @endlink
 * @test @link OsalApiStateTest_IoWrite64_TC_06_016_006 @endlink
 * @test @link OSALINTTESTS_IoWrite64_TC_06_016_007 @endlink
 * @test @link IOApiTest_IoWrite64_TC_06_016_008 @endlink
 * @test @link IOApiTest_IoBlockRead8_TC_06_009_001 @endlink
 * @test @link IOApiTest_IoBlockRead8_TC_06_009_003 @endlink
 * @test @link IOApiTest_IoBlockRead8_TC_06_009_004 @endlink
 * @test @link IOApiTest_IoBlockRead8_TC_06_009_005 @endlink
 * @test @link IOApiTest_IoBlockRead8_TC_06_009_006 @endlink
 * @test @link IOApiTest_IoBlockRead8_TC_06_009_007 @endlink
 * @test @link OsalApiStateTest_IoBlockRead8_TC_06_009_008 @endlink
 * @test @link OSALINTTESTS_IoBlockRead8_TC_06_009_009 @endlink
 * @test @link IOApiTest_IoBlockRead8_TC_06_009_010 @endlink
 * @test @link IOApiTest_IoBlockRead16_TC_06_010_001 @endlink
 * @test @link IOApiTest_IoBlockRead16_TC_06_010_002 @endlink
 * @test @link IOApiTest_IoBlockRead16_TC_06_010_003 @endlink
 * @test @link IOApiTest_IoBlockRead16_TC_06_010_004 @endlink
 * @test @link IOApiTest_IoBlockRead16_TC_06_010_005 @endlink
 * @test @link IOApiTest_IoBlockRead16_TC_06_010_006 @endlink
 * @test @link IOApiTest_IoBlockRead16_TC_06_010_007 @endlink
 * @test @link OsalApiStateTest_IoBlockRead16_TC_06_010_008 @endlink
 * @test @link OSALINTTESTS_IoBlockRead16_TC_06_010_009 @endlink
 * @test @link IOApiTest_IoBlockRead16_TC_06_010_010 @endlink
 * @test @link IOApiTest_IoBlockRead32_TC_06_011_001 @endlink
 * @test @link IOApiTest_IoBlockRead32_TC_06_011_002 @endlink
 * @test @link IOApiTest_IoBlockRead32_TC_06_011_003 @endlink
 * @test @link IOApiTest_IoBlockRead32_TC_06_011_004 @endlink
 * @test @link IOApiTest_IoBlockRead32_TC_06_011_005 @endlink
 * @test @link IOApiTest_IoBlockRead32_TC_06_011_006 @endlink
 * @test @link IOApiTest_IoBlockRead32_TC_06_011_007 @endlink
 * @test @link OsalApiStateTest_IoBlockRead32_TC_06_011_008 @endlink
 * @test @link OSALINTTESTS_IoBlockRead32_TC_06_011_009 @endlink
 * @test @link IOApiTest_IoBlockRead32_TC_06_011_010 @endlink
 * @test @link IOApiTest_IoBlockRead64_TC_06_012_001 @endlink
 * @test @link IOApiTest_IoBlockRead64_TC_06_012_002 @endlink
 * @test @link IOApiTest_IoBlockRead64_TC_06_012_003 @endlink
 * @test @link IOApiTest_IoBlockRead64_TC_06_012_004 @endlink
 * @test @link IOApiTest_IoBlockRead64_TC_06_012_005 @endlink
 * @test @link IOApiTest_IoBlockRead64_TC_06_012_006 @endlink
 * @test @link IOApiTest_IoBlockRead64_TC_06_012_007 @endlink
 * @test @link OsalApiStateTest_IoBlockRead64_TC_06_012_008 @endlink
 * @test @link OSALINTTESTS_IoBlockRead64_TC_06_012_009 @endlink
 * @test @link IOApiTest_IoBlockRead64_TC_06_012_010 @endlink
 * @test @link IOApiTest_IoBlockWrite8_TC_06_017_001 @endlink
 * @test @link IOApiTest_IoBlockWrite8_TC_06_017_003 @endlink
 * @test @link IOApiTest_IoBlockWrite8_TC_06_017_004 @endlink
 * @test @link IOApiTest_IoBlockWrite8_TC_06_017_005 @endlink
 * @test @link IOApiTest_IoBlockWrite8_TC_06_017_006 @endlink
 * @test @link IOApiTest_IoBlockWrite8_TC_06_017_007 @endlink
 * @test @link OsalApiStateTest_IoBlockWrite8_TC_06_017_008 @endlink
 * @test @link OSALINTTESTS_IoBlockWrite8_TC_06_017_009 @endlink
 * @test @link IOApiTest_IoBlockWrite8_TC_06_017_010 @endlink
 * @test @link IOApiTest_IoBlockWrite8_TC_06_017_011 @endlink
 * @test @link IOApiTest_IoBlockWrite16_TC_06_018_001 @endlink
 * @test @link IOApiTest_IoBlockWrite16_TC_06_018_002 @endlink
 * @test @link IOApiTest_IoBlockWrite16_TC_06_018_003 @endlink
 * @test @link IOApiTest_IoBlockWrite16_TC_06_018_004 @endlink
 * @test @link IOApiTest_IoBlockWrite16_TC_06_018_005 @endlink
 * @test @link IOApiTest_IoBlockWrite16_TC_06_018_006 @endlink
 * @test @link IOApiTest_IoBlockWrite16_TC_06_018_007 @endlink
 * @test @link OsalApiStateTest_IoBlockWrite16_TC_06_018_008 @endlink
 * @test @link OSALINTTESTS_IoBlockWrite16_TC_06_018_009 @endlink
 * @test @link IOApiTest_IoBlockWrite16_TC_06_018_010 @endlink
 * @test @link IOApiTest_IoBlockWrite16_TC_06_018_011 @endlink
 * @test @link IOApiTest_IoBlockWrite32_TC_06_019_001 @endlink
 * @test @link IOApiTest_IoBlockWrite32_TC_06_019_002 @endlink
 * @test @link IOApiTest_IoBlockWrite32_TC_06_019_003 @endlink
 * @test @link IOApiTest_IoBlockWrite32_TC_06_019_004 @endlink
 * @test @link IOApiTest_IoBlockWrite32_TC_06_019_005 @endlink
 * @test @link IOApiTest_IoBlockWrite32_TC_06_019_006 @endlink
 * @test @link IOApiTest_IoBlockWrite32_TC_06_019_007 @endlink
 * @test @link OsalApiStateTest_IoBlockWrite32_TC_06_019_008 @endlink
 * @test @link OSALINTTESTS_IoBlockWrite32_TC_06_019_009 @endlink
 * @test @link IOApiTest_IoBlockWrite32_TC_06_019_010 @endlink
 * @test @link IOApiTest_IoBlockWrite32_TC_06_019_011 @endlink
 * @test @link IOApiTest_IoBlockWrite64_TC_06_020_001 @endlink
 * @test @link IOApiTest_IoBlockWrite64_TC_06_020_002 @endlink
 * @test @link IOApiTest_IoBlockWrite64_TC_06_020_003 @endlink
 * @test @link IOApiTest_IoBlockWrite64_TC_06_020_004 @endlink
 * @test @link IOApiTest_IoBlockWrite64_TC_06_020_005 @endlink
 * @test @link IOApiTest_IoBlockWrite64_TC_06_020_006 @endlink
 * @test @link IOApiTest_IoBlockWrite64_TC_06_020_007 @endlink
 * @test @link OsalApiStateTest_IoBlockWrite64_TC_06_020_008 @endlink
 * @test @link OSALINTTESTS_IoBlockWrite64_TC_06_020_009 @endlink
 * @test @link IOApiTest_IoBlockWrite64_TC_06_020_010 @endlink
 * @test @link IOApiTest_IoBlockWrite64_TC_06_020_011 @endlink
 * @section IOTEST IOTEST function
 * @subsection IOTEST_SetUp IOTEST Setup
 * Common to Test Suit of IOTEST, prepare process to be executed before executing the test. <br>
 * For example:
 * @snippet test_common.hpp OSALTESTS_SetUp
 * @subsection IOTEST_TearDown IOTEST TearDown
 * Common to Test Suit of IOTEST, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_common.hpp OSALTESTS_TearDown
 * @section IOApiTest IOApiTest function
 * @subsection IOApiTest_SetUp IOApiTest Setup
 * Common to Test Suit of IOApiTest, prepare process to be executed before executing the test. <br>
 * For example:
 * @snippet test_io.cpp IOApiTest_SetUp
 * @snippet test_io.cpp IOApiTest_SetUpTestSuite
 * @subsection IOApiTest_TearDown IOApiTest TearDown
 * Common to Test Suit of IOApiTest, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_io.cpp IOApiTest_TearDown
 * @snippet test_io.cpp IOApiTest_TearDownTestSuite
 * @section OSALINTTESTS_IOApiTest OSALINTTESTS of IOApiTest function
 * @subsection OSALINTTESTS_IOApiTest_SetUp OSALINTTESTS of IOApiTest Setup
 * Common to Test Suit of OSALINTTESTS IOApiTest, prepare process to be executed before executing the test. <br>
 * This test suite generates interrupts. These process is preparation<br>
 * For example:
 * @snippet test_common.hpp OSALINTTESTS_SetUp
 * @subsection OSALINTTESTS_IOApiTest_TearDown OSALINTTESTS of IOApiTest TearDown
 * Common to Test Suit of OSALINTTESTS IOApiTest, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_common.hpp OSALINTTESTS_TearDown
 * @section OsalApiStateTest_IOApiTest OsalApiStateTest of IOApiTest function
 * @subsection OsalApiStateTest_IOApiTest_SetUp OsalApiStateTest of IOApiTest Setup
 * Common to Test Suit of OsalApiStateTest IOApiTest, prepare process to be executed before executing the test. <br>
 * For example:
 * @snippet test_common.hpp OsalApiStateTest_SetUp
 * @subsection OsalApiStateTest_IOApiTest_TearDown OsalApiStateTest of IOApiTest TearDown
 * Common to Test Suit of OsalApiStateTest IOApiTest, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_common.hpp OsalApiStateTest_TearDown
 */
class IOTEST : public OSALTESTS
{
    /* Do nothing */
};

/**
 * @page IO_TEST IO API Test
 * @section IOTEST_openClose_TC_12_001_009 IOTEST.openClose_TC_12_001_009
 * @subsection openClose_tsi Test Suite ID
 * IOTEST
 * @subsection openClose_tci Test Case ID
 * openClose_S3_N_TC_12_001_009
 * @subsection openClose_rr Related Requirement(s)
 * \#199198, \#199209, \#199210, \#199211, \#199212, \#276768
 * @subsection openClose_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection openClose_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoDeviceOpen and R_OSAL_IoDeviceClose function
 * @subsection openClose_tpts Test Procedure/Test Steps
 * -# Get the list of available devices
 * -# Declare new a device handle for each device. <br>
 * Open all device in this device list by using R_OSAL_IoDeviceOpen <br>
 * Store the device handle of opened devices into a handle list.
 * -# Close all opended devices through stored device handle by using R_OSAL_IoDeviceClose. <br>
 * Clear device handle list.
 * -# Open each device in the device list twice times. <br>
 * Close each opended device twice times.
 * -# Declare one device handle for all available devices. <br>
 * Open all device in this device list by using R_OSAL_IoDeviceOpen <br>
 * Store the device handle of opened devices into a handle list.
 * -# Close all opended devices through stored device handle by using R_OSAL_IoDeviceClose. <br>
 * Clear device handle list.
 * -# Get the biggest length of device ID in device list.
 * -# Allocate a buffer with size is biggest device ID's length plus one (includes terminated character "\0").
 * -# Copy each device ID in device list to allocated buffer. <br>
 * Open each device ID by using R_OSAL_IoDeviceOpen. <br>
 * Store the device handle of opened devices into a handle list.
 * -# Close all opended devices through stored device handle by using R_OSAL_IoDeviceClose. <br>
 * Clear device handle list.
 * -# Free allocated buffer.
 * @subsection openClose_ereb Expected Result/Behavior
 * -# Return a list of available devices: "devices".
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK for each device. <br>
 * All device handles of opened device are stored into "osal_devs".
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK for each device.
 * -# The first R_OSAL_IoDeviceOpen return OSAL_RETURN_OK. <br>
 * The second R_OSAL_IoDeviceOpen return OSAL_RETURN_BUSY. <br>
 * The first R_OSAL_IoDeviceClose return OSAL_RETURN_OK. <br>
 * The second R_OSAL_IoDeviceClose return OSAL_RETURN_HANDLE.
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK for each device. <br>
 * All device handles of opened device are stored into "osal_devs".
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK for each device in "osal_devs"
 * -# Return a biggest length of available device IDs.
 * -# Buffer allocation succeeded (Pointer to buffer != Nullpointer)
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK for each device. <br>
 * All device handles of opened device are stored into "osal_devs".
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK for each device.
 * -# Buffer free succeeded.
 * @subsection openClose_testcase Test Case Example
 * @snippet test_io.cpp openClose_TC_12_001_009
 * @snippet test_common.hpp available_devices
 */
// [openClose_TC_12_001_009]
TEST_F(IOTEST, openClose_TC_12_001_009)
{
    const vector<string> devices = available_devices();
    vector<osal_device_handle_t> osal_devs;

    //positive: open all devices
    for (string dev : devices)
    {
        osal_device_handle_t osal_dev;
        EXPECT_EQ(R_OSAL_IoDeviceOpen(dev.c_str(), &osal_dev), OSAL_RETURN_OK);
        osal_devs.push_back(osal_dev);
    }

    //positive: close all devices
    for (osal_device_handle_t osal_dev : osal_devs)
    {
        EXPECT_EQ(R_OSAL_IoDeviceClose(osal_dev), OSAL_RETURN_OK);
    }
    osal_devs.clear();

    //negative: open twice, close twice
    for (string dev : devices)
    {
        osal_device_handle_t osal_dev1, osal_dev2;
        EXPECT_EQ(R_OSAL_IoDeviceOpen(dev.c_str(), &osal_dev1), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoDeviceOpen(dev.c_str(), &osal_dev2), OSAL_RETURN_BUSY);

        EXPECT_EQ(R_OSAL_IoDeviceClose(osal_dev1), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoDeviceClose(osal_dev1), OSAL_RETURN_HANDLE);
    }

    //reuse same handle
    {
        osal_device_handle_t osal_dev;
        for (string dev : devices)
        {
            EXPECT_EQ(R_OSAL_IoDeviceOpen(dev.c_str(), &osal_dev), OSAL_RETURN_OK);
            osal_devs.push_back(osal_dev);
        }
    }
    for (osal_device_handle_t osal_dev : osal_devs)
    {
        EXPECT_EQ(R_OSAL_IoDeviceClose(osal_dev), OSAL_RETURN_OK);
    }
    osal_devs.clear();

    
    //reuse id_ptr
    {
        size_t max = 0;
        for (string device : devices)
        {
            if (strlen(device.c_str()) > max)
            {
                max = strlen(device.c_str());
            }
        }
        max++;
        char* name_buf = (char*)malloc(max);
        ASSERT_NE(name_buf, NULL_PTR);

        for (string device : devices)
        {
            osal_device_handle_t osal_dev;
            strcpy(name_buf, device.c_str());
            EXPECT_EQ(R_OSAL_IoDeviceOpen(name_buf, &osal_dev), OSAL_RETURN_OK);
            osal_devs.push_back(osal_dev);
        }

        for (osal_device_handle_t osal_dev : osal_devs)
        {
            EXPECT_EQ(R_OSAL_IoDeviceClose(osal_dev), OSAL_RETURN_OK);
        }
        osal_devs.clear();
        free(name_buf);
    }
    
}
// [openClose_TC_12_001_009]

class IOApiTest : public OSALTESTS
{
protected:
    osal_mq_handle_t sync_mq;
    st_osal_mq_config mq_config = {10, 4};
    int send_msg = 0xFFFF;
    int receive_msg = 0;

// [IOApiTest_SetUp]
    void SetUp() override{
        EXPECT_EQ(R_OSAL_MqCreate(&mq_config, SYNC_MQ_ID, &sync_mq), OSAL_RETURN_OK);
        EXPECT_NE(sync_mq, OSAL_MQ_HANDLE_INVALID);
#if defined(OSAL_SOC_S4)
#if !defined(OSAL_TEST_FRAMEWORK)
        if(strcmp(testing::UnitTest::GetInstance()->current_test_info()->name(), "IoDeviceOpen_TC_06_003_001") &&
            strcmp(testing::UnitTest::GetInstance()->current_test_info()->name(), "IoDeviceClose_TC_06_004_001"))
#else
        if(strcmp(TestCaseInfo<OsalTestFrameWork>::get().current_test_name(), "IoDeviceOpen_TC_06_003_001") &&
            strcmp(TestCaseInfo<OsalTestFrameWork>::get().current_test_name(), "IoDeviceClose_TC_06_004_001"))
#endif
        {
            // device used to turn ON/OFF clock and power used in S4 testing
            EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_2, &device_handle2), OSAL_RETURN_OK);
            EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_REQUIRED, true), OSAL_RETURN_OK);
        }
#endif
    }
// [IOApiTest_SetUp]

// [IOApiTest_TearDown]
    void TearDown() override{
        EXPECT_EQ(R_OSAL_MqReset(sync_mq), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_MqDelete(sync_mq), OSAL_RETURN_OK);
#if defined(OSAL_SOC_S4)
#if !defined(OSAL_TEST_FRAMEWORK)
        if(strcmp(testing::UnitTest::GetInstance()->current_test_info()->name(), "IoDeviceOpen_TC_06_003_001") &&
            strcmp(testing::UnitTest::GetInstance()->current_test_info()->name(), "IoDeviceClose_TC_06_004_001"))
#else
        if(strcmp(TestCaseInfo<OsalTestFrameWork>::get().current_test_name(), "IoDeviceOpen_TC_06_003_001") &&
            strcmp(TestCaseInfo<OsalTestFrameWork>::get().current_test_name(), "IoDeviceClose_TC_06_004_001"))
#endif
        {
            EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_RELEASED, true), OSAL_RETURN_OK);
            // device used to turn ON/OFF clock and power used in S4 testing
            EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle2), OSAL_RETURN_OK);
        }
#endif
    }
// [IOApiTest_TearDown]

    /* RVC implementation base on Linux OSAL IT*/
    typedef struct st_osal_test_info{
        char* device_type;
        size_t numOfDevice;
        char* buffer;
    } st_osal_test_info_t;

    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;
    osal_device_handle_t device_handle1 = OSAL_DEVICE_HANDLE_INVALID;
    osal_device_handle_t device_handle2 = OSAL_DEVICE_HANDLE_INVALID;
    static vector<string> device_list;
    static vector<st_osal_test_info_t> devices;

// [IOApiTest_SetUpTestSuite]
    static void SetUpTestSuite()
    {
        size_t size;
        size_t byte_written;
        char *buffer;
        st_osal_test_info_t devices_info[] =
        {
#if defined (OSAL_SOC_V3U)
        // Correct the description when the SoC device name is updated
            {"ims",                 2,             "ims_00\nims_01\0"},
            {"imr",                 4,             "imr_00\nimr_01\nimr_02\nimr_03\0"},
            {"imp_top",             1,             "imp_top_00\0"},
            {"imp",                 4,             "imp_00\nimp_01\nimp_02\nimp_03\0"},
            {"imp_ocv",             8,             "imp_ocv_00\nimp_ocv_01\nimp_ocv_02\nimp_ocv_03\nimp_ocv_04\nimp_ocv_05\nimp_ocv_06\nimp_ocv_07\0"},
            {"imp_dma",             2,             "imp_dma_00\nimp_dma_01\0"},
            {"imp_psc",             2,             "imp_psc_00\nimp_psc_01\0"},
            {"imp_cnn",             3,             "imp_cnn_00\nimp_cnn_01\nimp_cnn_02\0"},
            {"imp_spmi",            2,             "imp_spmi_00\nimp_spmi_01\0"},
            {"imp_spmc",            3,             "imp_spmc_00\nimp_spmc_01\nimp_spmc_02\0"},
            {"imp_reg_spmi",        2,             "imp_reg_spmi_00\nimp_reg_spmi_01\0"},
            {"imp_reg_spmc",        3,             "imp_reg_spmc_00\nimp_reg_spmc_01\nimp_reg_spmc_02\0"},
#if !defined(UNSUPPORT_IMP_CHILD_DEVICE)
            {"imp_dta",             1,             "imp_dta_00\0"},
            {"imp_radsp",           2,             "imp_radsp_00\nimp_radsp_01\0"},
#endif /* UNSUPPORT_IMP_CHILD_DEVICE */
#if !defined(UNSUPPORT_IMP_CHILD_DEVICE_DMAC)
            {"imp_radsp_dmac",      2,             "imp_radsp_dmac_00\nimp_radsp_dmac_01\0"},
#endif /* UNSUPPORT_IMP_CHILD_DEVICE_DMAC */
            {"imp_ldmac_master",    1,             "imp_ldmac_master_00\0"},
            {"imp_ldmac_checker",   1,             "imp_ldmac_checker_00\0"},
            {"dof",                 2,             "dof_00\ndof_01\0"},
            {"stv",                 2,             "stv_00\nstv_01\0"},
            {"acf",                 4,             "acf_00\nacf_01\nacf_02\nacf_03\0"},
#if !defined(UNSUPPORT_ICCOM)
            {"iccom",               1,             "iccom_00\0"},
#endif /* UNSUPPORT_ICCOM */
#if !defined(UNSUPPORT_IVCP1E)
            {"ivcp1e",              1,             "ivcp1e_00\0"},
#endif /* UNSUPPORT_IVCP1E */
#if !defined(UNSUPPORT_VCP4)
            {"vcp4",                1,             "vcp4_00\0"},
#endif /* UNSUPPORT_VCP4 */
#if !defined(UNSUPPORT_FCPC)
            {"fcpc",                1,             "fcpc_00\0"},
#endif /* UNSUPPORT_FCPC */
            {"cisp",                4,             "cisp_00\ncisp_01\ncisp_02\ncisp_03\0"},
            {"tisp",                4,             "tisp_00\ntisp_01\ntisp_02\ntisp_03\0"},
#if !defined(UNSUPPORT_VSPX)
            {"vspx",                4,             "vspx_00\nvspx_01\nvspx_02\nvspx_03\0"},
#endif /* UNSUPPORT_VSPX */
#if !defined(UNSUPPORT_FBC_DEVICE)
            {"fbc",                 1,             "fbc_00\0"},
#endif /* UNSUPPORT_FBC_DEVICE */
#if !defined(UNSUPPORT_FBA)
            {"fba",                 68,            "fba_ims0\nfba_ims1\nfba_imr0\nfba_imr1\nfba_imr2\nfba_imr3\nfba_imp0\nfba_imp1\nfba_imp2\nfba_imp3\nfba_imp_ocv1\nfba_imp_ocv2\nfba_imp_ocv0\nfba_imp_ocv3\nfba_imp_ocv4\nfba_imp_ocv5\nfba_imp_ocv6\nfba_imp_ocv7\nfba_imp_dp0\nfba_imp_dp1\nfba_imp_cnn0_main\nfba_imp_cnn0_sub0\nfba_imp_cnn0_sub1\nfba_imp_cnn0_sub2\nfba_imp_cnn0_sub3\nfba_imp_cnn1_main\nfba_imp_cnn1_sub0\nfba_imp_cnn1_sub1\nfba_imp_cnn1_sub2\nfba_imp_cnn1_sub3\nfba_imp_cnn2_main\nfba_imp_cnn2_sub0\nfba_imp_cnn2_sub1\nfba_imp_cnn2_sub2\nfba_imp_cnn2_sub3\nfba_imp_dsp0\nfba_imp_dsp1\nfba_imp_cnram0\nfba_imp_cnram1\nfba_imp_cnram2\nfba_dof0\nfba_dof1\nfba_stv0\nfba_stv1\nfba_acf0\nfba_acf1\nfba_smps0\nfba_smps1\nfba_smes0\nfba_smes1\nfba_smpo0\nfba_smpo1\nfba_isp0\nfba_isp1\nfba_isp2\nfba_isp3\nfba_ca76_0_cl0\nfba_ca76_0_cl1\nfba_ca76_1_cl0\nfba_ca76_1_cl1\nfba_ca76_0_cl0_cpu0\nfba_ca76_0_cl0_cpu1\nfba_ca76_0_cl1_cpu0\nfba_ca76_0_cl1_cpu1\nfba_ca76_1_cl0_cpu0\nfba_ca76_1_cl0_cpu1\nfba_ca76_1_cl1_cpu0\nfba_ca76_1_cl1_cpu1\0"},
#endif /* UNSUPPORT_FBA */
#if !defined(UNSUPPORT_RFSO)
            {"rfso",                11,            "rfso_00\nrfso_01\nrfso_02\nrfso_03\nrfso_04\nrfso_05\nrfso_06\nrfso_07\nrfso_08\nrfso_09\nrfso_10\0"},
#endif /* UNSUPPORT_RFSO */
#if !defined(UNSUPPORT_IPMMU)
            {"ipmmu_mm",            1,             "ipmmu_mm_00\0"},
            {"ipmmu_cache",         11,            "ipmmu_cache_vi0\nipmmu_cache_vi1\nipmmu_cache_vc0\nipmmu_cache_ir\nipmmu_cache_rt0\nipmmu_cache_rt1\nipmmu_cache_ds0\nipmmu_cache_ds1\nipmmu_cache_vip0\nipmmu_cache_vip1\nipmmu_cache_3dg\0"},
#endif
#if !defined(UNSUPPORT_FCPR)
            {"fcprr",               23,            "fcprr_dof_00\nfcprr_dof_01\nfcprr_stv_00\nfcprr_stv_01\nfcprr_acf_00\nfcprr_acf_01\nfcprr_acf_02\nfcprr_acf_03\nfcprr_imp_00\nfcprr_imp_01\nfcprr_ims_00\nfcprr_ims_01\nfcprr_imr_00\nfcprr_imr_01\nfcprr_imr_02\nfcprr_imr_03\nfcprr_vspd_00\nfcprr_vspd_01\nfcprr_vspx_00\nfcprr_vspx_01\nfcprr_vspx_02\nfcprr_vspx_03\nfcprr_rtdmac_00_03\0"},
            {"fcprw",               8,             "fcprw_dof_00\nfcprw_dof_01\nfcprw_imp_00\nfcprw_imp_01\nfcprw_imr_00_01\nfcprw_imr_02_03\nfcprw_vin_00_29\nfcprw_isp_00_03\0"},
            {"fcpra",               7,             "fcpra_vip0\nfcpra_vip1\nfcpra_vip2\nfcpra_imp\nfcpra_vc\nfcpra_vio\nfcpra_rtdmac\0"}
#endif /* UNSUPPORT_FCPR */
#elif defined(OSAL_SOC_V3H2)
            {"ims",                 3,             "ims_00\nims_01\nims_02\0"},
            {"imr",                 2,             "imr_00\nimr_01\0"},
            {"imp_top",             1,             "imp_top_00\0"},
            {"imp",                 5,             "imp_00\nimp_01\nimp_02\nimp_03\nimp_04\0"},
            {"imp_slim",            1,             "imp_slim_00\0"},
            {"imp_ocv",             5,             "imp_ocv_00\nimp_ocv_01\nimp_ocv_02\nimp_ocv_03\nimp_ocv_04\0"},
            {"imp_dma",             2,             "imp_dma_00\nimp_dma_01\0"},
            {"imp_psc",             1,             "imp_psc_00\0"},
            {"imp_cnn",             1,             "imp_cnn_00\0"},
            {"imp_spmi",            1,             "imp_spmi_00\0"},
            {"imp_dma_slim",        1,             "imp_dma_slim_00\0"},
            {"dof",                 1,             "dof_00\0"},
            {"stv",                 1,             "stv_00\0"},
            {"acf",                 5,             "acf_00\nacf_01\nacf_02\nacf_03\nacf_04\0"},
#if !defined(UNSUPPORT_ICCOM)
            {"iccom_reg",           1,             "iccom_reg_00\0"},
            {"iccom",               8,             "iccom_00\niccom_01\niccom_02\niccom_03\niccom_04\niccom_05\niccom_06\niccom_07\0"},
#endif /* UNSUPPORT_ICCOM */
#if !defined(UNSUPPORT_IVCP1E)
            {"ivcp1e",              1,             "ivcp1e_00\0"},
#endif /* UNSUPPORT_IVCP1E */
            {"cisp",                2,             "cisp_00\ncisp_01\0"},
            {"tisp",                2,             "tisp_00\ntisp_01\0"},
#if !defined(UNSUPPORT_FBC_DEVICE)
            {"fbc",                 1,             "fbc_00\0"},
#endif /* UNSUPPORT_FBC_DEVICE */
#if !defined(UNSUPPORT_FBA)
            {"fba",                 28,            "fba_imr0\nfba_imr1\nfba_ims0\nfba_ims1\nfba_ims2\nfba_imp0\nfba_imp1\nfba_imp2\nfba_imp3\nfba_imp4\nfba_imp_ocv0\nfba_imp_ocv1\nfba_imp_ocv2\nfba_imp_ocv3\nfba_imp_ocv4\nfba_imp_dp0\nfba_imp_dp1\nfba_imp_cnn0\nfba_imp_slim0\nfba_dof0\nfba_stv0\nfba_acf0\nfba_acf1\nfba_ca53d\nfba_ca530\nfba_ca531\nfba_ca532\nfba_ca533\0"},
#endif /* UNSUPPORT_FBA */
#if !defined(UNSUPPORT_RFSO)
            {"rfso",                11,            "rfso_00\nrfso_01\nrfso_02\nrfso_03\nrfso_04\nrfso_05\nrfso_06\nrfso_07\nrfso_08\nrfso_09\nrfso_10\0"},
#endif /* UNSUPPORT_RFSO */
#if !defined(UNSUPPORT_IPMMU)
            {"ipmmu_mm",            1,             "ipmmu_mm_00\0"},
            {"ipmmu_cache",         7,             "ipmmu_cache_vi0\nipmmu_cache_vc0\nipmmu_cache_ir\nipmmu_cache_rt0\nipmmu_cache_ds1\nipmmu_cache_vip0\nipmmu_cache_vip1\0"},
#endif /* UNSUPPORT_IPMMU */
#if !defined(UNSUPPORT_FCPR)
            {"fcprr",               18,            "fcprr_stv_00\nfcprr_dof_00\nfcprr_acf_00\nfcprr_acf_01\nfcprr_acf_02\nfcprr_acf_03\nfcprr_acf_04\nfcprr_imp_00\nfcprr_imp_01\nfcprr_ims_00\nfcprr_ims_01\nfcprr_ims_02\nfcprr_imr_00\nfcprr_imr_01\nfcprr_vspd_00\nfcprr_ivcp1e_00\nfcprr_pcie_00\nfcprr_ca53_00\0"},
            {"fcprw",               7,             "fcprw_dof_00\nfcprw_imp_00\nfcprw_imp_01\nfcprw_imr_00\nfcprw_imr_01\nfcprw_isp_00\nfcprw_isp_01\0"},
            {"fcpra",               6,             "fcpra_vip0\nfcpra_vip1\nfcpra_imp\nfcpra_imr\nfcpra_vio\nfcpra_ca53\0"}
#endif /* UNSUPPORT_FCPR */
#elif defined(OSAL_SOC_V3H1)
            {"ims",                 4,             "ims_00\nims_01\nims_02\nims_03\0"},
            {"imr",                 2,             "imr_00\nimr_01\0"},
            {"imp_top",             1,             "imp_top_00\0"},
            {"imp",                 5,             "imp_00\nimp_01\nimp_02\nimp_03\nimp_04\0"},
            {"imp_slim",            1,             "imp_slim_00\0"},
            {"imp_ocv",             5,             "imp_ocv_00\nimp_ocv_01\nimp_ocv_02\nimp_ocv_03\nimp_ocv_04\0"},
            {"imp_dma",             2,             "imp_dma_00\nimp_dma_01\0"},
            {"imp_psc",             2,             "imp_psc_00\nimp_psc_01\0"},
            {"imp_cnn",             1,             "imp_cnn_00\0"},
            {"imp_spmi",            1,             "imp_spmi_00\0"},
#if !defined(UNSUPPORT_VIP)
            {"dof",                 1,             "dof_00\0"},
            {"stv",                 1,             "stv_00\0"},
            {"acf",                 5,             "acf_00\nacf_01\nacf_02\nacf_03\nacf_04\0"},
#endif /* UNSUPPORT_VIP */
#if !defined(UNSUPPORT_ICCOM)
            {"iccom_reg",           1,             "iccom_reg_00\0"},
            {"iccom",               8,             "iccom_00\niccom_01\niccom_02\niccom_03\niccom_04\niccom_05\niccom_06\niccom_07\0"},
#endif /* UNSUPPORT_ICCOM */
#if !defined(UNSUPPORT_IVCP1E)
            {"ivcp1e",              1,             "ivcp1e_00\0"},
#endif /* UNSUPPORT_IVCP1E */
#if !defined(UNSUPPORT_ISP)
            {"cisp",                2,             "cisp_00\ncisp_01\0"},
            {"tisp",                2,             "tisp_00\ntisp_01\0"},
#endif /* UNSUPPORT_ISP */
#if !defined(UNSUPPORT_FBC_DEVICE)
            {"fbc",                 1,             "fbc_00\0"},
#endif /* UNSUPPORT_FBC_DEVICE */
#if !defined(UNSUPPORT_FBA)
            {"fba",                 29,            "fba_imr0\nfba_imr1\nfba_ims0\nfba_ims1\nfba_ims2\nfba_ims3\nfba_imp0\nfba_imp1\nfba_imp2\nfba_imp3\nfba_imp4\nfba_imp_ocv0\nfba_imp_ocv1\nfba_imp_ocv2\nfba_imp_ocv3\nfba_imp_ocv4\nfba_imp_dp0\nfba_imp_dp1\nfba_imp_cnn0\nfba_imp_slim0\nfba_dof0\nfba_stv0\nfba_acf0\nfba_acf1\nfba_ca53d\nfba_ca530\nfba_ca531\nfba_ca532\nfba_ca533\0"},
#endif /* UNSUPPORT_FBA */
#if !defined(UNSUPPORT_RFSO)
            {"rfso",                11,            "rfso_00\nrfso_01\nrfso_02\nrfso_03\nrfso_04\nrfso_05\nrfso_06\nrfso_07\nrfso_08\nrfso_09\nrfso_10\0"},
#endif /* UNSUPPORT_RFSO */
#if !defined(UNSUPPORT_IPMMU)
            {"ipmmu_mm",            1,             "ipmmu_mm_00\0"},
            {"ipmmu_cache",         7,             "ipmmu_cache_vi0\nipmmu_cache_vc0\nipmmu_cache_ir\nipmmu_cache_rt0\nipmmu_cache_ds1\nipmmu_cache_vip0\nipmmu_cache_vip1\0"},
#endif
#if !defined(UNSUPPORT_FCPR)
            {"fcprr",               19,            "fcprr_stv_00\nfcprr_dof_00\nfcprr_acf_00\nfcprr_acf_01\nfcprr_acf_02\nfcprr_acf_03\nfcprr_acf_04\nfcprr_imp_00\nfcprr_imp_01\nfcprr_ims_00\nfcprr_ims_01\nfcprr_ims_02\nfcprr_imr_00\nfcprr_imr_01\nfcprr_vspd_00\nfcprr_ivcp1e_00\nfcprr_pcie_00\nfcprr_rtdmac_00\nfcprr_ca53_00\0"},
            {"fcprw",               9,             "fcprw_dof_00\nfcprw_imp_00\nfcprw_imp_01\nfcprw_imr_00\nfcprw_imr_01\nfcprw_vin_00_03\nfcprw_vin_04_07\nfcprw_isp_00\nfcprw_isp_01\0"},
            {"fcpra",               7,             "fcpra_vip0\nfcpra_vip1\nfcpra_imp\nfcpra_imr\nfcpra_vio\nfcpra_rtdmac\nfcpra_ca53\0"}
#endif /* UNSUPPORT_FCPR */
#elif defined(OSAL_SOC_V3M2) /* V3M2 */
            {"ims",                 4,             "ims_00\nims_01\nims_02\nims_03\0"},
            {"imp_top",             1,             "imp_top_00\0"},
            {"imp",                 4,             "imp_00\nimp_01\nimp_02\nimp_03\0"},
            {"imp_ocv",             2,             "imp_ocv_00\nimp_ocv_01\0"},
            {"imp_dma",             1,             "imp_dma_00\0"},
            {"imp_psc",             1,             "imp_psc_00\0"},
            {"imp_cnn",             1,             "imp_cnn_00\0"},
            {"imp_spmi",            1,             "imp_spmi_00\0"},
#if !defined(UNSUPPORT_ICCOM)
            {"iccom_reg",           1,             "iccom_reg_00\0"},
            {"iccom",               8,             "iccom_00\niccom_01\niccom_02\niccom_03\niccom_04\niccom_05\niccom_06\niccom_07\0"},
#endif /* UNSUPPORT_ICCOM */
#if !defined(UNSUPPORT_IVCP1E)
            {"ivcp1e",              1,             "ivcp1e_00\0"},
#endif /* UNSUPPORT_IVCP1E */
#if !defined(UNSUPPORT_ISP)
            {"cisp",                1,             "cisp_00\0"},
            {"tisp",                1,             "tisp_00\0"},
#endif /* UNSUPPORT_ISP */
#if !defined(UNSUPPORT_FBC_DEVICE)
            {"fbc",                 1,             "fbc_00\0"},
#endif /* UNSUPPORT_FBC_DEVICE */
#if !defined(UNSUPPORT_FBA)
            {"fba",                 11,            "fba_ims0\nfba_ims1\nfba_imp0\nfba_imp1\nfba_imp_ocv0\nfba_imp_ocv1\nfba_imp_dp0\nfba_imp_cnn0\nfba_ca53d\nfba_ca530\nfba_ca531\0"},
#endif /* UNSUPPORT_FBA */
#if !defined(UNSUPPORT_RFSO)
            {"rfso",                11,            "rfso_00\nrfso_01\nrfso_02\nrfso_03\nrfso_04\nrfso_05\nrfso_06\nrfso_07\nrfso_08\nrfso_09\nrfso_10\0"},
#endif /* UNSUPPORT_RFSO */
#if !defined(UNSUPPORT_IPMMU)
            {"ipmmu_mm",            1,             "ipmmu_mm_00\0"},
            {"ipmmu_cache",         4,             "ipmmu_cache_vi0\nipmmu_cache_ir\nipmmu_cache_rt0\nipmmu_cache_ds1\0"}
#endif /* UNSUPPORT_IPMMU */
#elif defined(OSAL_SOC_V4H)
            {"ims",                 2,             "ims_00\nims_01\0"},
            {"imr",                 3,             "imr_00\nimr_01\nimr_02\0"},
            {"imp_top",             1,             "imp_top_00\0"},
            {"imp",                 4,             "imp_00\nimp_01\nimp_02\nimp_03\0"},
            {"imp_ocv",             4,             "imp_ocv_00\nimp_ocv_01\nimp_ocv_02\nimp_ocv_03\0"},
            {"imp_dma",             2,             "imp_dma_00\nimp_dma_01\0"},
            {"imp_psc",             1,             "imp_psc_00\0"},
            {"imp_dta",             1,             "imp_dta_00\0"},
            {"imp_dma_slim",        2,             "imp_dma_slim_00\nimp_dma_slim_01\0"},
            {"imp_cnn",             1,             "imp_cnn_00\0"},
            {"imp_spmi",            1,             "imp_spmi_00\0"},
#if defined (TEST_SIL)
            {"imp_spmc",            1,             "imp_spmc_00\0"},
#else
            {"imp_spmc",            2,             "imp_spmc_00\nimp_spmc_01\0"},
#endif
            {"imp_reg_spmi",        1,             "imp_reg_spmi_00\0"},
            {"imp_reg_spmc",        1,             "imp_reg_spmc_00\0"},
#if !defined(UNSUPPORT_VDSP)
            {"vdsp00",              8,             "vdsp00_00\nvdsp00_01\nvdsp00_02\nvdsp00_03\nvdsp00_04\nvdsp00_05\nvdsp00_06\nvdsp00_07\0"},
            {"vdsp01",              8,             "vdsp01_00\nvdsp01_01\nvdsp01_02\nvdsp01_03\nvdsp01_04\nvdsp01_05\nvdsp01_06\nvdsp01_07\0"},
            {"vdsp02",              8,             "vdsp02_00\nvdsp02_01\nvdsp02_02\nvdsp02_03\nvdsp02_04\nvdsp02_05\nvdsp02_06\nvdsp02_07\0"},
            {"vdsp03",              8,             "vdsp03_00\nvdsp03_01\nvdsp03_02\nvdsp03_03\nvdsp03_04\nvdsp03_05\nvdsp03_06\nvdsp03_07\0"},
#endif /* UNSUPPORT_VDSP */
#if !defined(UNSUPPORT_VIP)
            {"dof",                 1,             "dof_00\0"},
            {"smd_ps",              1,             "smd_ps_00\0"},
            {"smd_post",            1,             "smd_post_00\0"},
            {"dof_fcprm",           1,             "dof_fcprm_00\0"},
#endif /* UNSUPPORT_VIP */
#if !defined(UNSUPPORT_VIP_WRAPPER)
            {"vip_wrap",            1,             "vip_wrap_00\0"},
#endif /* UNSUPPORT_VIP_WRAPPER */
#if !defined(UNSUPPORT_GFX)
            {"gsx",                 1,             "gsx_00\0"},
#endif /* UNSUPPORT_GFX */
#if !defined(UNSUPPORT_ICCOM)
#if defined(TEST_EMCOS)
            {"iccom",               1,             "iccom_00\0"},
#else
            {"iccom_reg",           1,             "iccom_reg_00\0"},
#if defined(TEST_LINUX) || defined(TEST_QNX) /* for CA */
            {"iccom",              12,             "iccom_00\niccom_01\niccom_02\niccom_03\niccom_04\niccom_05\niccom_06\niccom_07\niccom_08\niccom_09\niccom_10\niccom_11\0"},
#else  /* for CR */
            {"iccom",              4,             "iccom_00\niccom_01\niccom_02\niccom_03\0"},
#endif /* CA or CR */
#endif /* TEST_EMCOS*/
#endif /* UNSUPPORT_ICCOM */
#if !defined(UNSUPPORT_IVCP1E)
            {"ivcp1e",              1,             "ivcp1e_00\0"},
#endif /* UNSUPPORT_IVCP1E */
#if !defined(UNSUPPORT_VCP4)
            {"vcp4",                1,             "vcp4_00\0"},
#endif /* UNSUPPORT_VCP4 */
#if !defined(UNSUPPORT_FCPC)
            {"fcpc",                1,             "fcpc_00\0"},
#endif /* UNSUPPORT_FCPC */
#if !defined(UNSUPPORT_ISP)
            {"cisp",                2,             "cisp_00\ncisp_01\0"},
            {"tisp",                2,             "tisp_00\ntisp_01\0"},
#endif /* UNSUPPORT_VSPX */
#if !defined(UNSUPPORT_VSPX)
            {"vspx",                2,             "vspx_00\nvspx_01\0"},
#endif /* UNSUPPORT_VSPX */
#if !defined(UNSUPPORT_FBC_DEVICE)
            {"fbc",                 1,             "fbc_00\0"},
#endif /* UNSUPPORT_FBC_DEVICE */
#if !defined(UNSUPPORT_FBA)
#if defined(TEST_LINUX) || defined(TEST_QNX) /* for CA */
            {"fba",                 35,            "fba_ims0\nfba_ims1\nfba_imr0\nfba_imr1\nfba_imr2\nfba_imp0\nfba_imp1\nfba_imp2\nfba_imp3\nfba_imp_ocv0\nfba_imp_ocv1\nfba_imp_ocv2\nfba_imp_ocv3\nfba_imp_dp0\nfba_imp_dp1\nfba_imp_cnn0\nfba_imp_dsp0\nfba_imp_dsp1\nfba_imp_dsp2\nfba_imp_dsp3\nfba_imp_slim0\nfba_imp_slim1\nfba_imp_cnram0\nfba_umfl0\nfba_smps0\nfba_smpo0\nfba_pap0\nfba_isp0\nfba_isp1\nfba_ca76d0\nfba_ca76d1\nfba_ca760\nfba_ca761\nfba_ca762\nfba_ca763\0"},
#else  /* for CR */
            {"fba",                 29,            "fba_ims0\nfba_ims1\nfba_imr0\nfba_imr1\nfba_imr2\nfba_imp0\nfba_imp1\nfba_imp2\nfba_imp3\nfba_imp_ocv0\nfba_imp_ocv1\nfba_imp_ocv2\nfba_imp_ocv3\nfba_imp_dp0\nfba_imp_dp1\nfba_imp_cnn0\nfba_imp_dsp0\nfba_imp_dsp1\nfba_imp_dsp2\nfba_imp_dsp3\nfba_imp_cnram0\nfba_imp_slim0\nfba_imp_slim1\nfba_umfl0\nfba_smps0\nfba_smpo0\nfba_pap0\nfba_isp0\nfba_isp1\0"},
#endif /* CA or CR */
#endif /* UNSUPPORT_FBA */
#if !defined(UNSUPPORT_RFSO)
            {"rfso",                11,            "rfso_00\nrfso_01\nrfso_02\nrfso_03\nrfso_04\nrfso_05\nrfso_06\nrfso_07\nrfso_08\nrfso_09\nrfso_10\0"},
#endif /* UNSUPPORT_RFSO */
#if !defined(UNSUPPORT_IPMMU)
            {"ipmmu_mm",            1,             "ipmmu_mm_00\0"},
            {"ipmmu_cache",         11,            "ipmmu_cache_vi0\nipmmu_cache_vi1\nipmmu_cache_vc\nipmmu_cache_ir\nipmmu_cache_rt0\nipmmu_cache_rt1\nipmmu_cache_ds0\nipmmu_cache_hsc\nipmmu_cache_vip0\nipmmu_cache_vip1\nipmmu_cache_3dg\0"},
#endif /* UNSUPPORT_IPMMU */
#if !defined(UNSUPPORT_FCPR)
            {"fcprr",               23,            "fcprr_dof_00\nfcprr_dof_01\nfcprr_stv_00\nfcprr_stv_01\nfcprr_acf_00\nfcprr_acf_01\nfcprr_acf_02\nfcprr_acf_03\nfcprr_imp_00\nfcprr_imp_01\nfcprr_ims_00\nfcprr_ims_01\nfcprr_imr_00\nfcprr_imr_01\nfcprr_imr_02\nfcprr_imr_03\nfcprr_vspd_00\nfcprr_vspd_01\nfcprr_vspx_00\nfcprr_vspx_01\nfcprr_vspx_02\nfcprr_vspx_03\nfcprr_rtdmac_00_03\0"},
            {"fcprw",               8,             "fcprw_dof_00\nfcprw_dof_01\nfcprw_imp_00\nfcprw_imp_01\nfcprw_imr_00_01\nfcprw_imr_02_03\nfcprw_vin_00_29\nfcprw_isp_00_03\0"},
            {"fcpra",               7,             "fcpra_vip0\nfcpra_vip1\nfcpra_vip2\nfcpra_imp\nfcpra_vc\nfcpra_vio\nfcpra_rtdmac\0"},
#endif /* UNSUPPORT_FCPR */
#if !defined(UNSUPPORT_CAIPLITE)
            {"caiplite",            8,             "caiplite_00\ncaiplite_01\ncaiplite_02\ncaiplite_03\ncaiplite_04\ncaiplite_05\ncaiplite_06\ncaiplite_07\0"},
#endif /* UNSUPPORT_CAIPLITE */
#if !defined(UNSUPPORT_WCRC_CAIPLITE)
            {"wcrc_caiplite",       4,             "wcrc_caiplite_00\nwcrc_caiplite_01\nwcrc_caiplite_02\nwcrc_caiplite_03\0"},
#endif /* UNSUPPORT_WCRC_CAIPLITE */
#if !defined(UNSUPPORT_CAIPLITE_LITE)
            {"caiplite_wrapper",    1,             "caiplite_wrapper_00\0"},
#endif /* UNSUPPORT_CAIPLITE_LITE */
#if !defined(UNSUPPORT_VIDEO_INPUT)
            {"vin",                 16,             "vin_00\nvin_01\nvin_02\nvin_03\nvin_04\nvin_05\nvin_06\nvin_07\nvin_10\nvin_11\nvin_12\nvin_13\nvin_14\nvin_15\nvin_16\nvin_17\0"},
#endif /* UNSUPPORT_VIDEO_INPUT */
#if !defined(UNSUPPORT_VIDEO_OUTPUT_VSPD)
            {"vsp",                 2,              "vsp_00\nvsp_01\0"},
#endif /* UNSUPPORT_VIDEO_OUTPUT_VSPD */
#if !defined(UNSUPPORT_VIDEO_OUTPUT_CSI2)
            {"csi",                 2,              "csi_00\ncsi_01\0"},
#endif /* UNSUPPORT_VIDEO_OUTPUT_CSI2 */
#if !defined(UNSUPPORT_VIDEO_OUTPUT)
            {"doc",                 1,             "doc_00\0"},
#endif /* UNSUPPORT_VIDEO_INPUT */
#if !defined(UNSUPPORT_VIDEO_OUTPUT_DSI_ENCODER)
            {"dsi",                 2,             "dsi_00\ndsi_01\0"},
#endif /* UNSUPPORT_VIDEO_OUTPUT_DSI_ENCODER */
#if !defined(UNSUPPORT_VIDEO_OUTPUT_DISPLAY)
            {"du",                  1,             "du_00\0"},
#endif /* UNSUPPORT_VIDEO_OUTPUT_DISPLAY */
#if !defined(UNSUPPORT_RTDMAC_CONTROL)
            {"rtdmac_control0",     16,            "rtdmac_control0_00\nrtdmac_control0_01\nrtdmac_control0_02\nrtdmac_control0_03\nrtdmac_control0_04\nrtdmac_control0_04\nrtdmac_control0_06\nrtdmac_control0_07\nrtdmac_control0_08\nrtdmac_control0_09\nrtdmac_control0_10\nrtdmac_control0_11\nrtdmac_control0_12\nrtdmac_control0_13\nrtdmac_control0_14\nrtdmac_control0_15\0"},
            {"rtdmac_control1",     16,            "rtdmac_control1_00\nrtdmac_control1_01\nrtdmac_control1_02\nrtdmac_control1_03\nrtdmac_control1_04\nrtdmac_control1_04\nrtdmac_control1_06\nrtdmac_control1_07\nrtdmac_control1_08\nrtdmac_control1_09\nrtdmac_control1_10\nrtdmac_control1_11\nrtdmac_control1_12\nrtdmac_control1_13\nrtdmac_control1_14\nrtdmac_control1_15\0"},
            {"rtdmac_control2",     16,            "rtdmac_control2_00\nrtdmac_control2_01\nrtdmac_control2_02\nrtdmac_control2_03\nrtdmac_control2_04\nrtdmac_control2_04\nrtdmac_control2_06\nrtdmac_control2_07\nrtdmac_control2_08\nrtdmac_control2_09\nrtdmac_control2_10\nrtdmac_control2_11\nrtdmac_control2_12\nrtdmac_control2_13\nrtdmac_control2_14\nrtdmac_control2_15\0"},
            {"rtdmac_control3",     16,            "rtdmac_control3_00\nrtdmac_control3_01\nrtdmac_control3_02\nrtdmac_control3_03\nrtdmac_control3_04\nrtdmac_control3_04\nrtdmac_control3_06\nrtdmac_control3_07\nrtdmac_control3_08\nrtdmac_control3_09\nrtdmac_control3_10\nrtdmac_control3_11\nrtdmac_control3_12\nrtdmac_control3_13\nrtdmac_control3_14\nrtdmac_control3_15\0"},
#endif /* UNSUPPORT_RTDMAC_CONTROL */
#if !defined(UNSUPPORT_WWDT)
            {"wwdt",                10,            "wwdt_00\nwwdt_01\nwwdt_02\nwwdt_03\nwwdt_04\nwwdt_05\nwwdt_06\nwwdt_07\nwwdt_08\nwwdt_09\0"},
#endif /* UNSUPPORT_WWDT */
#if !defined(UNSUPPORT_VIDEO_OUTPUT_DSC)
            {"dsc",                 1,             "dsc_00\0"},
#endif /* UNSUPPORT_VIDEO_OUTPUT_DSC */
#if !defined(UNSUPPORT_RTDMAC_DESCRIPTOR)
            {"rtdmac_descriptor0",  1,             "rtdmac_descriptor0_00\0"},
            {"rtdmac_descriptor1",  1,             "rtdmac_descriptor1_00\0"},
            {"rtdmac_descriptor2",  1,             "rtdmac_descriptor2_00\0"},
            {"rtdmac_descriptor3",  1,             "rtdmac_descriptor3_00\0"},
#endif /* UNSUPPORT_RTDMAC_DESCRIPTOR */
#if !defined(UNSUPPORT_PAP)
            {"hsm",                 1,             "hsm_00\0"},
            {"dcm",                 1,             "dcm_00\0"},
            {"cbf",                 1,             "cbf_00\0"},
            {"sbf",                 1,             "sbf_00\0"},
            {"osm",                 1,             "osm_00\0"},
            {"ism",                 2,             "ism_00\nism_01\0"},
            {"sdmac",               1,             "sdmac_00\0"},
            {"fbs",                 1,             "fbs_00\0"},
            {"apb",                 1,             "apb_00\0"},
            {"dul",                 1,             "dul_00\0"},
            {"ecm",                 1,             "ecm_00\0"},
#endif /* UNSUPPORT_PAP */
#if !defined(UNSUPPORT_SYSTEM_RAM)
            {"system_ram",          1,             "system_ram_00\0"},
#endif /* UNSUPPORT_SYSTEM_RAM */
#if !defined(UNSUPPORT_ICUMX_CMD)
            {"icumx_cmd",           1,             "icumx_cmd_00\0"},
#endif /* UNSUPPORT_ICUMX_CMD */
#elif defined(OSAL_SOC_V4H2)
            {"ims",                 2,             "ims_00\nims_01\0"},
            {"imr",                 3,             "imr_00\nimr_01\nimr_02\0"},
            {"imp_top",             1,             "imp_top_00\0"},
            {"imp",                 4,             "imp_00\nimp_01\nimp_02\nimp_03\0"},
            {"imp_ocv",             4,             "imp_ocv_00\nimp_ocv_01\nimp_ocv_02\nimp_ocv_03\0"},
            {"imp_dma",             2,             "imp_dma_00\nimp_dma_01\0"},
            {"imp_psc",             1,             "imp_psc_00\0"},
            {"imp_dta",             1,             "imp_dta_00\0"},
            {"imp_dma_slim",        2,             "imp_dma_slim_00\nimp_dma_slim_01\0"},
            {"imp_cnn",             1,             "imp_cnn_00\0"},
            {"imp_spmi",            1,             "imp_spmi_00\0"},
            {"imp_spmc",            2,             "imp_spmc_00\nimp_spmc_01\0"},
            {"imp_reg_spmi",        1,             "imp_reg_spmi_00\0"},
            {"imp_reg_spmc",        1,             "imp_reg_spmc_00\0"},
#if !defined(UNSUPPORT_VDSP)
            {"vdsp00",              8,             "vdsp00_00\nvdsp00_01\nvdsp00_02\nvdsp00_03\nvdsp00_04\nvdsp00_05\nvdsp00_06\nvdsp00_07\0"},
            {"vdsp01",              8,             "vdsp01_00\nvdsp01_01\nvdsp01_02\nvdsp01_03\nvdsp01_04\nvdsp01_05\nvdsp01_06\nvdsp01_07\0"},
            {"vdsp02",              8,             "vdsp02_00\nvdsp02_01\nvdsp02_02\nvdsp02_03\nvdsp02_04\nvdsp02_05\nvdsp02_06\nvdsp02_07\0"},
            {"vdsp03",              8,             "vdsp03_00\nvdsp03_01\nvdsp03_02\nvdsp03_03\nvdsp03_04\nvdsp03_05\nvdsp03_06\nvdsp03_07\0"},
#endif /* UNSUPPORT_VDSP */
#if !defined(UNSUPPORT_VIP)
            {"dof",                 1,             "dof_00\0"},
            {"smd_ps",              1,             "smd_ps_00\0"},
            {"smd_post",            1,             "smd_post_00\0"},
            {"dof_fcprm",           1,             "dof_fcprm_00\0"},
#endif /* UNSUPPORT_VIP */
#if !defined(UNSUPPORT_VIP_WRAPPER)
            {"vip_wrap",            1,             "vip_wrap_00\0"},
#endif /* UNSUPPORT_VIP_WRAPPER */
#if !defined(UNSUPPORT_GFX)
            {"gsx",                 1,             "gsx_00\0"},
#endif /* UNSUPPORT_GFX */
#if !defined(UNSUPPORT_ICCOM)
            {"iccom_reg",           1,             "iccom_reg_00\0"},
            {"iccom",              12,             "iccom_00\niccom_01\niccom_02\niccom_03\niccom_04\niccom_05\niccom_06\niccom_07\niccom_08\niccom_09\niccom_10\niccom_11\0"},
#endif /* UNSUPPORT_ICCOM */
#if !defined(UNSUPPORT_IVCP1E)
            {"ivcp1e",              1,             "ivcp1e_00\0"},
#endif /* UNSUPPORT_IVCP1E */
#if !defined(UNSUPPORT_VCP4)
            {"vcp4",                1,             "vcp4_00\0"},
#endif /* UNSUPPORT_VCP4 */
#if !defined(UNSUPPORT_FCPC)
            {"fcpc",                1,             "fcpc_00\0"},
#endif /* UNSUPPORT_FCPC */
#if !defined(UNSUPPORT_ISP)
            {"cisp",                2,             "cisp_00\ncisp_01\0"},
            {"tisp",                2,             "tisp_00\ntisp_01\0"},
#endif /* UNSUPPORT_VSPX */
#if !defined(UNSUPPORT_VSPX)
            {"vspx",                2,             "vspx_00\nvspx_01\0"},
#endif /* UNSUPPORT_VSPX */
#if !defined(UNSUPPORT_FBC_DEVICE)
            {"fbc",                 1,             "fbc_00\0"},
#endif /* UNSUPPORT_FBC_DEVICE */
#if !defined(UNSUPPORT_FBA)
            {"fba",                 35,            "fba_ims0\nfba_ims1\nfba_imr0\nfba_imr1\nfba_imr2\nfba_imp0\nfba_imp1\nfba_imp2\nfba_imp3\nfba_imp_ocv0\nfba_imp_ocv1\nfba_imp_ocv2\nfba_imp_ocv3\nfba_imp_dp0\nfba_imp_dp1\nfba_imp_cnn0\nfba_imp_dsp0\nfba_imp_dsp1\nfba_imp_dsp2\nfba_imp_dsp3\nfba_imp_slim0\nfba_imp_slim1\nfba_imp_cnram0\nfba_umfl0\nfba_smps0\nfba_smpo0\nfba_pap0\nfba_isp0\nfba_isp1\nfba_ca76d0\nfba_ca76d1\nfba_ca760\nfba_ca761\nfba_ca762\nfba_ca763\0"},
#endif /* UNSUPPORT_FBA */
#if !defined(UNSUPPORT_RFSO)
            {"rfso",                11,            "rfso_00\nrfso_01\nrfso_02\nrfso_03\nrfso_04\nrfso_05\nrfso_06\nrfso_07\nrfso_08\nrfso_09\nrfso_10\0"},
#endif /* UNSUPPORT_RFSO */
#if !defined(UNSUPPORT_IPMMU)
            {"ipmmu_mm",            1,             "ipmmu_mm_00\0"},
            {"ipmmu_cache",         11,            "ipmmu_cache_vi0\nipmmu_cache_vi1\nipmmu_cache_vc\nipmmu_cache_ir\nipmmu_cache_rt0\nipmmu_cache_rt1\nipmmu_cache_ds0\nipmmu_cache_hsc\nipmmu_cache_vip0\nipmmu_cache_vip1\nipmmu_cache_3dg\0"},
#endif /* UNSUPPORT_IPMMU */
#if !defined(UNSUPPORT_FCPR)
            {"fcprr",               23,            "fcprr_dof_00\nfcprr_dof_01\nfcprr_stv_00\nfcprr_stv_01\nfcprr_acf_00\nfcprr_acf_01\nfcprr_acf_02\nfcprr_acf_03\nfcprr_imp_00\nfcprr_imp_01\nfcprr_ims_00\nfcprr_ims_01\nfcprr_imr_00\nfcprr_imr_01\nfcprr_imr_02\nfcprr_imr_03\nfcprr_vspd_00\nfcprr_vspd_01\nfcprr_vspx_00\nfcprr_vspx_01\nfcprr_vspx_02\nfcprr_vspx_03\nfcprr_rtdmac_00_03\0"},
            {"fcprw",               8,             "fcprw_dof_00\nfcprw_dof_01\nfcprw_imp_00\nfcprw_imp_01\nfcprw_imr_00_01\nfcprw_imr_02_03\nfcprw_vin_00_29\nfcprw_isp_00_03\0"},
            {"fcpra",               7,             "fcpra_vip0\nfcpra_vip1\nfcpra_vip2\nfcpra_imp\nfcpra_vc\nfcpra_vio\nfcpra_rtdmac\0"},
#endif /* UNSUPPORT_FCPR */
#if !defined(UNSUPPORT_CAIPLITE)
            {"caiplite",            8,             "caiplite_00\ncaiplite_01\ncaiplite_02\ncaiplite_03\ncaiplite_04\ncaiplite_05\ncaiplite_06\ncaiplite_07\0"},
#endif /* UNSUPPORT_CAIPLITE */
#if !defined(UNSUPPORT_WCRC_CAIPLITE)
            {"wcrc_caiplite",       4,             "wcrc_caiplite_00\nwcrc_caiplite_01\nwcrc_caiplite_02\nwcrc_caiplite_03\0"},
#endif /* UNSUPPORT_WCRC_CAIPLITE */
#if !defined(UNSUPPORT_CAIPLITE_LITE)
            {"caiplite_wrapper",    1,             "caiplite_wrapper_00\0"},
#endif /* UNSUPPORT_CAIPLITE_LITE */
#if !defined(UNSUPPORT_VIDEO_INPUT)
            {"vin",                 16,             "vin_00\nvin_01\nvin_02\nvin_03\nvin_04\nvin_05\nvin_06\nvin_07\nvin_10\nvin_11\nvin_12\nvin_13\nvin_14\nvin_15\nvin_16\nvin_17\0"},
#endif /* UNSUPPORT_VIDEO_INPUT */
#if !defined(UNSUPPORT_VIDEO_OUTPUT_VSPD)
            {"vsp",                 2,              "vsp_00\nvsp_01\0"},
#endif /* UNSUPPORT_VIDEO_OUTPUT_VSPD */
#if !defined(UNSUPPORT_VIDEO_OUTPUT_CSI2)
            {"csi",                 2,              "csi_00\ncsi_01\0"},
#endif /* UNSUPPORT_VIDEO_OUTPUT_CSI2 */
#if !defined(UNSUPPORT_VIDEO_OUTPUT)
            {"doc",                 1,             "doc_00\0"},
#endif /* UNSUPPORT_VIDEO_INPUT */
#if !defined(UNSUPPORT_VIDEO_OUTPUT_DSI_ENCODER)
            {"dsi",                 2,             "dsi_00\ndsi_01\0"},
#endif /* UNSUPPORT_VIDEO_OUTPUT_DSI_ENCODER */
#if !defined(UNSUPPORT_VIDEO_OUTPUT_DISPLAY)
            {"du",                  1,             "du_00\0"},
#endif /* UNSUPPORT_VIDEO_OUTPUT_DISPLAY */
#if !defined(UNSUPPORT_RTDMAC_CONTROL)
            {"rtdmac_control0",     16,            "rtdmac_control0_00\nrtdmac_control0_01\nrtdmac_control0_02\nrtdmac_control0_03\nrtdmac_control0_04\nrtdmac_control0_04\nrtdmac_control0_06\nrtdmac_control0_07\nrtdmac_control0_08\nrtdmac_control0_09\nrtdmac_control0_10\nrtdmac_control0_11\nrtdmac_control0_12\nrtdmac_control0_13\nrtdmac_control0_14\nrtdmac_control0_15\0"},
            {"rtdmac_control1",     16,            "rtdmac_control1_00\nrtdmac_control1_01\nrtdmac_control1_02\nrtdmac_control1_03\nrtdmac_control1_04\nrtdmac_control1_04\nrtdmac_control1_06\nrtdmac_control1_07\nrtdmac_control1_08\nrtdmac_control1_09\nrtdmac_control1_10\nrtdmac_control1_11\nrtdmac_control1_12\nrtdmac_control1_13\nrtdmac_control1_14\nrtdmac_control1_15\0"},
            {"rtdmac_control2",     16,            "rtdmac_control2_00\nrtdmac_control2_01\nrtdmac_control2_02\nrtdmac_control2_03\nrtdmac_control2_04\nrtdmac_control2_04\nrtdmac_control2_06\nrtdmac_control2_07\nrtdmac_control2_08\nrtdmac_control2_09\nrtdmac_control2_10\nrtdmac_control2_11\nrtdmac_control2_12\nrtdmac_control2_13\nrtdmac_control2_14\nrtdmac_control2_15\0"},
            {"rtdmac_control3",     16,            "rtdmac_control3_00\nrtdmac_control3_01\nrtdmac_control3_02\nrtdmac_control3_03\nrtdmac_control3_04\nrtdmac_control3_04\nrtdmac_control3_06\nrtdmac_control3_07\nrtdmac_control3_08\nrtdmac_control3_09\nrtdmac_control3_10\nrtdmac_control3_11\nrtdmac_control3_12\nrtdmac_control3_13\nrtdmac_control3_14\nrtdmac_control3_15\0"},
#endif /* UNSUPPORT_RTDMAC_CONTROL */
#if !defined(UNSUPPORT_WWDT)
            {"wwdt",                10,            "wwdt_00\nwwdt_01\nwwdt_02\nwwdt_03\nwwdt_04\nwwdt_05\nwwdt_06\nwwdt_07\nwwdt_08\nwwdt_09\0"},
#endif /* UNSUPPORT_WWDT */
#if !defined(UNSUPPORT_VIDEO_OUTPUT_DSC)
            {"dsc",                 1,             "dsc_00\0"},
#endif /* UNSUPPORT_VIDEO_OUTPUT_DSC */
#if !defined(UNSUPPORT_RTDMAC_DESCRIPTOR)
            {"rtdmac_descriptor0",  1,             "rtdmac_descriptor0_00\0"},
            {"rtdmac_descriptor1",  1,             "rtdmac_descriptor1_00\0"},
            {"rtdmac_descriptor2",  1,             "rtdmac_descriptor2_00\0"},
            {"rtdmac_descriptor3",  1,             "rtdmac_descriptor3_00\0"},
#endif /* UNSUPPORT_RTDMAC_DESCRIPTOR */
#if !defined(UNSUPPORT_PAP)
            {"hsm",                 1,             "hsm_00\0"},
            {"dcm",                 1,             "dcm_00\0"},
            {"cbf",                 1,             "cbf_00\0"},
            {"sbf",                 1,             "sbf_00\0"},
            {"osm",                 1,             "osm_00\0"},
            {"ism",                 2,             "ism_00\nism_01\0"},
            {"sdmac",               1,             "sdmac_00\0"},
            {"fbs",                 1,             "fbs_00\0"},
            {"apb",                 1,             "apb_00\0"},
            {"dul",                 1,             "dul_00\0"},
            {"ecm",                 1,             "ecm_00\0"},
#endif /* UNSUPPORT_PAP */
#if !defined(UNSUPPORT_SYSTEM_RAM)
            {"system_ram",          1,             "system_ram_00\0"},
#endif /* UNSUPPORT_SYSTEM_RAM */
#if !defined(UNSUPPORT_ICUMX_CMD)
            {"icumx_cmd",           1,             "icumx_cmd_00\0"},
#endif /* UNSUPPORT_ICUMX_CMD */
#elif defined(OSAL_SOC_V4M)
            {"ims",                 2,             "ims_00\nims_01\0"},
            {"imr",                 2,             "imr_00\nimr_01\0"},
            {"imp_top",             1,             "imp_top_00\0"},
            {"imp",                 2,             "imp_00\nimp_01\0"},
            {"imp_ocv",             4,             "imp_ocv_00\nimp_ocv_01\nimp_ocv_02\nimp_ocv_03\0"},
            {"imp_dma",             2,             "imp_dma_00\nimp_dma_01\0"},
            {"imp_psc",             1,             "imp_psc_00\0"},
            {"imp_dta",             1,             "imp_dta_00\0"},
            {"imp_dma_slim",        2,             "imp_dma_slim_00\nimp_dma_slim_01\0"},
            {"imp_cnn",             1,             "imp_cnn_00\0"},
            {"imp_spmi",            1,             "imp_spmi_00\0"},
            {"imp_spmc",            2,             "imp_spmc_00\nimp_spmc_01\0"},
            {"imp_reg_spmi",        1,             "imp_reg_spmi_00\0"},
            {"imp_reg_spmc",        1,             "imp_reg_spmc_00\0"},
#if !defined(UNSUPPORT_VDSP)
            {"vdsp00",              9,             "vdsp00_00\nvdsp00_01\nvdsp00_02\nvdsp00_03\nvdsp00_04\nvdsp00_05\nvdsp00_06\nvdsp00_07\nvdsp00_08\0"},
            {"vdsp01",              9,             "vdsp01_00\nvdsp01_01\nvdsp01_02\nvdsp01_03\nvdsp01_04\nvdsp01_05\nvdsp01_06\nvdsp01_07\nvdsp01_08\0"},
#endif /* UNSUPPORT_VDSP */
#if !defined(UNSUPPORT_VIP)
            {"dof",                 1,             "dof_00\0"},
            {"smd_ps",              1,             "smd_ps_00\0"},
            {"smd_post",            1,             "smd_post_00\0"},
            {"dof_fcprm",           1,             "dof_fcprm_00\0"},
#endif /* UNSUPPORT_VIP */
#if !defined(UNSUPPORT_VIP_WRAPPER)
            {"vip_wrap",            1,             "vip_wrap_00\0"},
#endif /* UNSUPPORT_VIP_WRAPPER */
#if !defined(UNSUPPORT_GFX)
            {"gsx",                 1,             "gsx_00\0"},
#endif /* UNSUPPORT_GFX */
#if !defined(UNSUPPORT_ICCOM)
            {"iccom_reg",           1,             "iccom_reg_00\0"},
            {"iccom",              12,             "iccom_00\niccom_01\niccom_02\niccom_03\niccom_04\niccom_05\niccom_06\niccom_07\niccom_08\niccom_09\niccom_10\niccom_11\0"},
#endif /* UNSUPPORT_ICCOM */
#if !defined(UNSUPPORT_IVCP1E)
            {"ivcp1e",              1,             "ivcp1e_00\0"},
#endif /* UNSUPPORT_IVCP1E */
#if !defined(UNSUPPORT_FCPC)
            {"fcpc",                1,             "fcpc_00\0"},
#endif /* UNSUPPORT_FCPC */
#if !defined(UNSUPPORT_ISP)
            {"cisp",                1,             "cisp_00\0"},
            {"tisp",                2,             "tisp_00\ntisp_01\0"},
#endif /* UNSUPPORT_VSPX */
#if !defined(UNSUPPORT_VSPX)
            {"vspx",                1,             "vspx_00\0"},
#endif /* UNSUPPORT_VSPX */
#if !defined(UNSUPPORT_FCPVX)
            {"fcpvx",                1,             "fcpvx_00\0"},
#endif /* UNSUPPORT_FCPVX */
#if !defined(UNSUPPORT_GICD)
            {"gicd",                1,             "gicd_00\0"},
#endif /* UNSUPPORT_GICD */
#if !defined(UNSUPPORT_FBC_DEVICE)
            {"fbc",                 1,             "fbc_00\0"},
#endif /* UNSUPPORT_FBC_DEVICE */
#if !defined(UNSUPPORT_FBA)
            {"fba",                 26,            "fba_ims0\nfba_ims1\nfba_imr0\nfba_imr1\nfba_imp0\nfba_imp1\nfba_imp_ocv0\nfba_imp_ocv1\nfba_imp_ocv2\nfba_imp_ocv3\nfba_imp_dp0\nfba_imp_dp1\nfba_imp_cnn0\nfba_imp_dsp0\nfba_imp_dsp1\nfba_imp_cnram0\nfba_umfl0\nfba_smps0\nfba_smpo0\nfba_pap0\nfba_isp0\nfba_ca76d0\nfba_ca760\nfba_ca761\nfba_ca762\nfba_ca763\0"},
#endif /* UNSUPPORT_FBA */
#if !defined(UNSUPPORT_RFSO)
            {"rfso",                11,            "rfso_00\nrfso_01\nrfso_02\nrfso_03\nrfso_04\nrfso_05\nrfso_06\nrfso_07\nrfso_08\nrfso_09\nrfso_10\0"},
#endif /* UNSUPPORT_RFSO */
#if !defined(UNSUPPORT_IPMMU)
            {"ipmmu_mm",            1,             "ipmmu_mm_00\0"},
            {"ipmmu_cache",         11,            "ipmmu_cache_vi0\nipmmu_cache_vi1\nipmmu_cache_vc\nipmmu_cache_ir\nipmmu_cache_rt0\nipmmu_cache_rt1\nipmmu_cache_ds0\nipmmu_cache_hsc\nipmmu_cache_vip0\nipmmu_cache_vip1\nipmmu_cache_3dg\0"},
#endif /* UNSUPPORT_IPMMU */
#if !defined(UNSUPPORT_FCPR)
            {"fcprr",               23,            "fcprr_dof_00\nfcprr_dof_01\nfcprr_stv_00\nfcprr_stv_01\nfcprr_acf_00\nfcprr_acf_01\nfcprr_acf_02\nfcprr_acf_03\nfcprr_imp_00\nfcprr_imp_01\nfcprr_ims_00\nfcprr_ims_01\nfcprr_imr_00\nfcprr_imr_01\nfcprr_imr_02\nfcprr_imr_03\nfcprr_vspd_00\nfcprr_vspd_01\nfcprr_vspx_00\nfcprr_vspx_01\nfcprr_vspx_02\nfcprr_vspx_03\nfcprr_rtdmac_00_03\0"},
            {"fcprw",               8,             "fcprw_dof_00\nfcprw_dof_01\nfcprw_imp_00\nfcprw_imp_01\nfcprw_imr_00_01\nfcprw_imr_02_03\nfcprw_vin_00_29\nfcprw_isp_00_03\0"},
            {"fcpra",               7,             "fcpra_vip0\nfcpra_vip1\nfcpra_vip2\nfcpra_imp\nfcpra_vc\nfcpra_vio\nfcpra_rtdmac\0"},
#endif /* UNSUPPORT_FCPR */
#if !defined(UNSUPPORT_CAIPLITE)
            {"caiplite",            8,             "caiplite_00\ncaiplite_01\ncaiplite_02\ncaiplite_03\ncaiplite_04\ncaiplite_05\ncaiplite_06\ncaiplite_07\0"},
#endif /* UNSUPPORT_CAIPLITE */
#if !defined(UNSUPPORT_WCRC_CAIPLITE)
            {"wcrc_caiplite",       4,             "wcrc_caiplite_00\nwcrc_caiplite_01\nwcrc_caiplite_02\nwcrc_caiplite_03\0"},
#endif /* UNSUPPORT_WCRC_CAIPLITE */
#if !defined(UNSUPPORT_CAIPLITE_LITE)
            {"caiplite_wrapper",    1,             "caiplite_wrapper_00\0"},
#endif /* UNSUPPORT_CAIPLITE_LITE */
#if !defined(UNSUPPORT_VIDEO_INPUT)
            {"vin",                 16,             "vin_00\nvin_01\nvin_02\nvin_03\nvin_04\nvin_05\nvin_06\nvin_07\nvin_10\nvin_11\nvin_12\nvin_13\nvin_14\nvin_15\nvin_16\nvin_17\0"},
#endif /* UNSUPPORT_VIDEO_INPUT */
#if !defined(UNSUPPORT_VIDEO_OUTPUT_VSPD)
            {"vsp",                 1,              "vsp_00\0"},
#endif /* UNSUPPORT_VIDEO_OUTPUT_VSPD */
#if !defined(UNSUPPORT_VIDEO_OUTPUT_CSI2)
            {"csi",                 2,              "csi_00\ncsi_01\0"},
#endif /* UNSUPPORT_VIDEO_OUTPUT_CSI2 */
#if !defined(UNSUPPORT_VIDEO_OUTPUT)
            {"doc",                 1,             "doc_00\0"},
#endif /* UNSUPPORT_VIDEO_INPUT */
#if !defined(UNSUPPORT_VIDEO_OUTPUT_DSI_ENCODER)
            {"dsi",                 1,             "dsi_00\0"},
#endif /* UNSUPPORT_VIDEO_OUTPUT_DSI_ENCODER */
#if !defined(UNSUPPORT_VIDEO_OUTPUT_DISPLAY)
            {"du",                  1,             "du_00\0"},
#endif /* UNSUPPORT_VIDEO_OUTPUT_DISPLAY */
#if !defined(UNSUPPORT_RTDMAC_CONTROL)
            {"rtdmac_control0",     16,            "rtdmac_control0_00\nrtdmac_control0_01\nrtdmac_control0_02\nrtdmac_control0_03\nrtdmac_control0_04\nrtdmac_control0_04\nrtdmac_control0_06\nrtdmac_control0_07\nrtdmac_control0_08\nrtdmac_control0_09\nrtdmac_control0_10\nrtdmac_control0_11\nrtdmac_control0_12\nrtdmac_control0_13\nrtdmac_control0_14\nrtdmac_control0_15\0"},
            {"rtdmac_control1",     16,            "rtdmac_control1_00\nrtdmac_control1_01\nrtdmac_control1_02\nrtdmac_control1_03\nrtdmac_control1_04\nrtdmac_control1_04\nrtdmac_control1_06\nrtdmac_control1_07\nrtdmac_control1_08\nrtdmac_control1_09\nrtdmac_control1_10\nrtdmac_control1_11\nrtdmac_control1_12\nrtdmac_control1_13\nrtdmac_control1_14\nrtdmac_control1_15\0"},
#endif /* UNSUPPORT_RTDMAC_CONTROL */
#if !defined(UNSUPPORT_WWDT)
            {"wwdt",                10,            "wwdt_00\nwwdt_01\nwwdt_02\nwwdt_03\nwwdt_04\nwwdt_05\nwwdt_06\nwwdt_07\nwwdt_08\nwwdt_09\0"},
#endif /* UNSUPPORT_WWDT */
#if !defined(UNSUPPORT_VIDEO_OUTPUT_DSC)
            {"dsc",                 1,             "dsc_00\0"},
#endif /* UNSUPPORT_VIDEO_OUTPUT_DSC */
#if !defined(UNSUPPORT_RTDMAC_DESCRIPTOR)
            {"rtdmac_descriptor0",  1,             "rtdmac_descriptor0_00\0"},
            {"rtdmac_descriptor1",  1,             "rtdmac_descriptor1_00\0"},
#endif /* UNSUPPORT_RTDMAC_DESCRIPTOR */
#if !defined(UNSUPPORT_PAP)
            {"hsm",                 1,             "hsm_00\0"},
            {"dcm",                 1,             "dcm_00\0"},
            {"cbf",                 1,             "cbf_00\0"},
            {"sbf",                 1,             "sbf_00\0"},
            {"osm",                 1,             "osm_00\0"},
            {"ism",                 2,             "ism_00\nism_01\0"},
            {"sdmac",               1,             "sdmac_00\0"},
            {"fbs",                 1,             "fbs_00\0"},
            {"apb",                 1,             "apb_00\0"},
            {"dul",                 1,             "dul_00\0"},
            {"ecm",                 1,             "ecm_00\0"},
#endif /* UNSUPPORT_PAP */
#if !defined(UNSUPPORT_SYSTEM_RAM)
            {"system_ram",          1,             "system_ram_00\0"},
#endif /* UNSUPPORT_SYSTEM_RAM */
#if !defined(UNSUPPORT_ICUMX_CMD)
            {"icumx_cmd",           1,             "icumx_cmd_00\0"},
#endif /* UNSUPPORT_ICUMX_CMD */
#if !defined(UNSUPPORT_QOS)
            {"qos",           1,             "qos_00\0"},
#endif /* UNSUPPORT_QOS */
#if !defined(UNSUPPORT_RCARBUSMONI)
            {"rcarbusmoni",           3,             "rcarbusmoni_00\nrcarbusmoni_01\nrcarbusmoni_02\0"},
#endif /* UNSUPPORT_RCARBUSMONI */
#elif defined(OSAL_SOC_S4)
#if !defined(UNSUPPORT_CAIPLITE)
            {"caiplite",            8,             "caiplite_00\ncaiplite_01\ncaiplite_02\ncaiplite_03\ncaiplite_04\ncaiplite_05\ncaiplite_06\ncaiplite_07\0"},
#endif /* UNSUPPORT_CAIPLITE */
#if !defined(UNSUPPORT_WCRC_CAIPLITE)
            {"wcrc_caiplite",       4,             "wcrc_caiplite_00\nwcrc_caiplite_01\nwcrc_caiplite_02\nwcrc_caiplite_03\0"},
#endif /* UNSUPPORT_WCRC_CAIPLITE */
#if !defined(UNSUPPORT_RTDMAC_CONTROL)
            {"rtdmac_control0",     16,            "rtdmac_control0_00\nrtdmac_control0_01\nrtdmac_control0_02\nrtdmac_control0_03\nrtdmac_control0_04\nrtdmac_control0_04\nrtdmac_control0_06\nrtdmac_control0_07\nrtdmac_control0_08\nrtdmac_control0_09\nrtdmac_control0_10\nrtdmac_control0_11\nrtdmac_control0_12\nrtdmac_control0_13\nrtdmac_control0_14\nrtdmac_control0_15\0"},
            {"rtdmac_control1",     16,            "rtdmac_control1_00\nrtdmac_control1_01\nrtdmac_control1_02\nrtdmac_control1_03\nrtdmac_control1_04\nrtdmac_control1_04\nrtdmac_control1_06\nrtdmac_control1_07\nrtdmac_control1_08\nrtdmac_control1_09\nrtdmac_control1_10\nrtdmac_control1_11\nrtdmac_control1_12\nrtdmac_control1_13\nrtdmac_control1_14\nrtdmac_control1_15\0"},
            {"rtdmac_control2",     16,            "rtdmac_control2_00\nrtdmac_control2_01\nrtdmac_control2_02\nrtdmac_control2_03\nrtdmac_control2_04\nrtdmac_control2_04\nrtdmac_control2_06\nrtdmac_control2_07\nrtdmac_control2_08\nrtdmac_control2_09\nrtdmac_control2_10\nrtdmac_control2_11\nrtdmac_control2_12\nrtdmac_control2_13\nrtdmac_control2_14\nrtdmac_control2_15\0"},
            {"rtdmac_control3",     16,            "rtdmac_control3_00\nrtdmac_control3_01\nrtdmac_control3_02\nrtdmac_control3_03\nrtdmac_control3_04\nrtdmac_control3_04\nrtdmac_control3_06\nrtdmac_control3_07\nrtdmac_control3_08\nrtdmac_control3_09\nrtdmac_control3_10\nrtdmac_control3_11\nrtdmac_control3_12\nrtdmac_control3_13\nrtdmac_control3_14\nrtdmac_control3_15\0"},
#endif /* UNSUPPORT_RTDMAC_CONTROL */
#if !defined(UNSUPPORT_RTDMAC_DESCRIPTOR)
            {"rtdmac_descriptor0",  1,             "rtdmac_descriptor0_00\0"},
            {"rtdmac_descriptor1",  1,             "rtdmac_descriptor1_00\0"},
            {"rtdmac_descriptor2",  1,             "rtdmac_descriptor2_00\0"},
            {"rtdmac_descriptor3",  1,             "rtdmac_descriptor3_00\0"},
#endif /* UNSUPPORT_RTDMAC_DESCRIPTOR */
#if !defined(UNSUPPORT_SIPS)
            {"ships",               3,             "ships_00\nships_01\nships_02\0"},
#endif /* UNSUPPORT_SIPS */
#if !defined(UNSUPPORT_ICCOM)
            {"iccom_reg",           1,             "iccom_reg_00\0"},
            {"iccom",              24,             "iccom_00\niccom_01\niccom_02\niccom_03\niccom_04\niccom_05\niccom_06\niccom_07\niccom_08\niccom_09\niccom_10\niccom_11\niccom_12\niccom_13\niccom_14\niccom_15\niccom_16\niccom_17\niccom_18\niccom_19\niccom_20\niccom_21\niccom_22\niccom_23\0"},
#endif /* UNSUPPORT_ICCOM */
#if !defined(UNSUPPORT_SYSTEM_RAM)
            {"system_ram",          1,             "system_ram_00\0"},
#endif /* UNSUPPORT_SYSTEM_RAM */
#if !defined(UNSUPPORT_ICUMX_CMD)
            {"icumx_cmd",           1,             "icumx_cmd_00\0"},
#endif /* UNSUPPORT_ICUMX_CMD */
#endif
        };

        for(auto i = 0; i < sizeof(devices_info)/sizeof(devices_info[0]); i++)
        {
            devices.push_back(devices_info[i]);
        }

        for (st_osal_test_info_t dev : devices)
        {
            EXPECT_EQ(R_OSAL_IoGetDeviceList(dev.device_type, NULL, 0, &size), OSAL_RETURN_OK_RQST) << "failed. Device type: " << dev.device_type;
            buffer = (char*)malloc(size);
            EXPECT_NE(buffer, NULL_PTR);
            EXPECT_EQ(R_OSAL_IoGetDeviceList(dev.device_type, buffer, size, &byte_written), OSAL_RETURN_OK) << "failed. Device type: " << dev.device_type;
            EXPECT_EQ(size, byte_written);
            char* token = strtok(buffer, "\n");
            while (token != NULL)
            {
                device_list.push_back(string(token));
                token = strtok(NULL, "\n");
            }
            free(buffer);
        }
    }
// [IOApiTest_SetUpTestSuite]

// [IOApiTest_TearDownTestSuite]
    static void TearDownTestSuite()
    {
        device_list.clear();
        device_list.shrink_to_fit();
        EXPECT_EQ(device_list.size(), 0);
        EXPECT_EQ(device_list.capacity(), 0);
    }
// [IOApiTest_TearDownTestSuite]

// [set_clock_on]
    e_osal_return_t set_clock_on(osal_device_handle_t handle)
    {
        e_osal_return_t osal_ret = OSAL_RETURN_OK;
#if !defined(OSAL_SOC_S4)
        e_osal_pm_required_state_t req_state;

        if (NULL == handle)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else
        {
            osal_ret = R_OSAL_PmGetRequiredState(handle, &req_state);
            EXPECT_EQ(osal_ret, OSAL_RETURN_OK);

            if (req_state != OSAL_PM_REQUIRED_STATE_REQUIRED)
            {
                osal_ret = R_OSAL_PmSetRequiredState(handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
                EXPECT_EQ(osal_ret, OSAL_RETURN_OK);
            }
        }
#else
        /* Do nothing. Clock and power have already been turned ON in SetUpTestSuite*/
        (void)handle;
#endif
        return osal_ret;
    }
// [set_clock_on]

// [set_clock_off]
    e_osal_return_t set_clock_off(osal_device_handle_t handle)
    {
        e_osal_return_t osal_ret = OSAL_RETURN_OK;
#if !defined(OSAL_SOC_S4)
        e_osal_pm_required_state_t req_state;

        if (NULL == handle)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else
        {
            osal_ret = R_OSAL_PmGetRequiredState(handle, &req_state);
            EXPECT_EQ(osal_ret, OSAL_RETURN_OK);

            if (req_state == OSAL_PM_REQUIRED_STATE_REQUIRED)
            {
                osal_ret = R_OSAL_PmSetRequiredState(handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
                EXPECT_EQ(osal_ret, OSAL_RETURN_OK);
            }
        }
#else
        /* Do nothing. Clock and power will be turned OFF in TearDownTestSuite */
        (void)handle;
#endif
        return osal_ret;
    }
// [set_clock_off]
};

struct io_test_multiple_thread_worker_arg
{
    io_test_multiple_thread_worker_arg(osal_device_handle_t * device_hndl, osal_thread_id_t tid)
    : device_handle(device_hndl), uid(tid)
    {
    }
    osal_device_handle_t *device_handle;
    osal_thread_id_t uid;
    osal_mq_handle_t mq;
    int send_msg;
    uint8_t init_data8;
    uint16_t init_data16;
    uint32_t init_data32;
    uint64_t init_data64;
    uint8_t init_block_data8[16];
    uint16_t init_block_data16[8];
    uint32_t init_block_data32[4];
    uint64_t init_block_data64[2];
};

// [io_open_close_device_child_thread]
int64_t io_open_close_device_child_thread(void * arg)
{
    io_test_multiple_thread_worker_arg *testStruct = (io_test_multiple_thread_worker_arg *)arg;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_mq_config local_mq_config;

    if(testStruct->uid == TEST_THREAD_ID2)
    {
        EXPECT_EQ(R_OSAL_MqGetConfig(testStruct->mq, &local_mq_config), OSAL_RETURN_OK) << "Test06_003_008 failed";
        EXPECT_EQ(R_OSAL_IoDeviceClose(*(testStruct->device_handle)), OSAL_RETURN_HANDLE) << "Test06_003_008 failed";
        EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, testStruct->device_handle), OSAL_RETURN_OK) << "Test06_003_008 failed";
        EXPECT_NE(*(testStruct->device_handle), NULL_PTR) << "Test06_003_008 failed";
        EXPECT_EQ(R_OSAL_MqSendForTimePeriod(testStruct->mq, SYNC_MQ_TIME, &testStruct->send_msg, local_mq_config.msg_size), OSAL_RETURN_OK) << "Test06_003_008 failed";
    }
    else if(testStruct->uid == TEST_THREAD_ID3)
    {
        EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, testStruct->device_handle), OSAL_RETURN_BUSY) << "Test06_003_008 failed";
        EXPECT_NE(*(testStruct->device_handle), NULL_PTR) << "Test06_003_008 failed";
        EXPECT_EQ(R_OSAL_IoDeviceClose(*(testStruct->device_handle)), OSAL_RETURN_OK) << "Test06_003_008 failed";
        *(testStruct->device_handle) = OSAL_DEVICE_HANDLE_INVALID;
    }
    else
    {
    }
    return (int64_t) osal_ret;
}
// [io_open_close_device_child_thread]

// [io_read_write_8_device_child_thread]
int64_t io_read_write_8_device_child_thread(void * arg)
{
    io_test_multiple_thread_worker_arg *testStruct = (io_test_multiple_thread_worker_arg *)arg;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint8_t read_data8;
    uint8_t write_data8 = TEST_WRITE8_VAL_NC;
    uintptr_t offset = TEST_WRITE8_REG_OFFSET_NC;
    st_osal_mq_config local_mq_config;

    if(testStruct->uid == TEST_THREAD_ID2)
    {
        EXPECT_EQ(R_OSAL_MqGetConfig(testStruct->mq, &local_mq_config), OSAL_RETURN_OK) << "Test06_013_008 failed";
        EXPECT_EQ(R_OSAL_IoRead8(*(testStruct->device_handle), offset, &testStruct->init_data8), OSAL_RETURN_OK) << "Test06_013_008 failed";
        EXPECT_EQ(R_OSAL_IoWrite8(*(testStruct->device_handle), offset, write_data8), OSAL_RETURN_OK) << "Test06_013_008 failed";
        EXPECT_EQ(R_OSAL_MqSendForTimePeriod(testStruct->mq, SYNC_MQ_TIME, &testStruct->send_msg, local_mq_config.msg_size), OSAL_RETURN_OK) << "Test06_013_008 failed";
    }
    else if(testStruct->uid == TEST_THREAD_ID3)
    {
        EXPECT_EQ(R_OSAL_IoRead8(*(testStruct->device_handle), offset, &read_data8), OSAL_RETURN_OK) << "Test06_013_008 failed";
        EXPECT_EQ(read_data8, write_data8) << "Test06_013_008 failed";
        EXPECT_EQ(R_OSAL_IoWrite8(*(testStruct->device_handle), offset, testStruct->init_data8), OSAL_RETURN_OK) << "Test06_013_008 failed";
    }
    else
    {
    }
    return (int64_t) osal_ret;
}
// [io_read_write_8_device_child_thread]

// [io_read_write_16_device_child_thread]
int64_t io_read_write_16_device_child_thread(void * arg)
{
    io_test_multiple_thread_worker_arg *testStruct = (io_test_multiple_thread_worker_arg *)arg;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint16_t read_data16;
    uint16_t write_data16 = TEST_WRITE16_VAL_NC;
    uintptr_t offset = TEST_WRITE16_REG_OFFSET_NC;
    st_osal_mq_config local_mq_config;

    if(testStruct->uid == TEST_THREAD_ID2)
    {
        EXPECT_EQ(R_OSAL_MqGetConfig(testStruct->mq, &local_mq_config), OSAL_RETURN_OK) << "Test06_014_008 failed";
        EXPECT_EQ(R_OSAL_IoRead16(*(testStruct->device_handle), offset, &testStruct->init_data16), OSAL_RETURN_OK) << "Test06_014_008 failed";
        EXPECT_EQ(R_OSAL_IoWrite16(*(testStruct->device_handle), offset, write_data16), OSAL_RETURN_OK) << "Test06_014_008 failed";
        EXPECT_EQ(R_OSAL_MqSendForTimePeriod(testStruct->mq, SYNC_MQ_TIME, &testStruct->send_msg, local_mq_config.msg_size), OSAL_RETURN_OK) << "Test06_014_008 failed";
    }
    else if(testStruct->uid == TEST_THREAD_ID3)
    {
        EXPECT_EQ(R_OSAL_IoRead16(*(testStruct->device_handle), offset, &read_data16), OSAL_RETURN_OK) << "Test06_014_008 failed";
        EXPECT_EQ(read_data16, write_data16) << "Test06_014_008 failed";
        EXPECT_EQ(R_OSAL_IoWrite16(*(testStruct->device_handle), offset, testStruct->init_data16), OSAL_RETURN_OK) << "Test06_014_008 failed";
    }
    else
    {
    }
    return (int64_t) osal_ret;
}
// [io_read_write_16_device_child_thread]

// [io_read_write_32_device_child_thread]
int64_t io_read_write_32_device_child_thread(void * arg)
{
    io_test_multiple_thread_worker_arg *testStruct = (io_test_multiple_thread_worker_arg *)arg;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint32_t read_data32;
    uint32_t write_data32 = TEST_WRITE32_VAL_NC;
    uintptr_t offset = TEST_WRITE32_REG_OFFSET_NC;
    st_osal_mq_config local_mq_config;

    if(testStruct->uid == TEST_THREAD_ID2)
    {
        EXPECT_EQ(R_OSAL_MqGetConfig(testStruct->mq, &local_mq_config), OSAL_RETURN_OK) << "Test06_015_008 failed";
        EXPECT_EQ(R_OSAL_IoRead32(*(testStruct->device_handle), offset, &testStruct->init_data32), OSAL_RETURN_OK) << "Test06_015_008 failed";
        EXPECT_EQ(R_OSAL_IoWrite32(*(testStruct->device_handle), offset, write_data32), OSAL_RETURN_OK) << "Test06_015_008 failed";
        EXPECT_EQ(R_OSAL_MqSendForTimePeriod(testStruct->mq, SYNC_MQ_TIME, &testStruct->send_msg, local_mq_config.msg_size), OSAL_RETURN_OK) << "Test06_015_008 failed";
    }
    else if(testStruct->uid == TEST_THREAD_ID3)
    {
        EXPECT_EQ(R_OSAL_IoRead32(*(testStruct->device_handle), offset, &read_data32), OSAL_RETURN_OK) << "Test06_015_008 failed";
        EXPECT_EQ(read_data32, write_data32) << "Test06_015_008 failed";
        EXPECT_EQ(R_OSAL_IoWrite32(*(testStruct->device_handle), offset, testStruct->init_data32), OSAL_RETURN_OK) << "Test06_015_008 failed";
    }
    else
    {
    }
    return (int64_t) osal_ret;
}
// [io_read_write_32_device_child_thread]

// [io_read_write_64_device_child_thread]
int64_t io_read_write_64_device_child_thread(void * arg)
{
    io_test_multiple_thread_worker_arg *testStruct = (io_test_multiple_thread_worker_arg *)arg;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint64_t read_data64;
    uint64_t write_data64 = TEST_WRITE64_VAL_NC;
    uintptr_t offset = TEST_WRITE64_REG_OFFSET_NC;
    st_osal_mq_config local_mq_config;

    if(testStruct->uid == TEST_THREAD_ID2)
    {
        EXPECT_EQ(R_OSAL_MqGetConfig(testStruct->mq, &local_mq_config), OSAL_RETURN_OK) << "Test06_016_008 failed";
        EXPECT_EQ(R_OSAL_IoRead64(*(testStruct->device_handle), offset, &testStruct->init_data64), OSAL_RETURN_OK) << "Test06_016_008 failed";
        EXPECT_EQ(R_OSAL_IoWrite64(*(testStruct->device_handle), offset, write_data64), OSAL_RETURN_OK) << "Test06_016_008 failed";
        EXPECT_EQ(R_OSAL_MqSendForTimePeriod(testStruct->mq, SYNC_MQ_TIME, &testStruct->send_msg, local_mq_config.msg_size), OSAL_RETURN_OK) << "Test06_016_008 failed";
    }
    else if(testStruct->uid == TEST_THREAD_ID3)
    {
        EXPECT_EQ(R_OSAL_IoRead64(*(testStruct->device_handle), offset, &read_data64), OSAL_RETURN_OK) << "Test06_016_008 failed";
        EXPECT_EQ(read_data64, write_data64) << "Test06_016_008 failed";
        EXPECT_EQ(R_OSAL_IoWrite64(*(testStruct->device_handle), offset, testStruct->init_data64), OSAL_RETURN_OK) << "Test06_016_008 failed";
    }
    else
    {
    }
    return (int64_t) osal_ret;
}
// [io_read_write_64_device_child_thread]

// [io_block_read_write_8_device_child_thread]
int64_t io_block_read_write_8_device_child_thread(void * arg)
{
    io_test_multiple_thread_worker_arg *testStruct = (io_test_multiple_thread_worker_arg *)arg;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint8_t read_data8[16];
    uint8_t register_data8[16] = {TEST_WRITEBLOCK8_VAL_NC[0], TEST_WRITEBLOCK8_VAL_NC[1],
                                  TEST_WRITEBLOCK8_VAL_NC[2], TEST_WRITEBLOCK8_VAL_NC[3],
                                  TEST_WRITEBLOCK8_VAL_NC[4], TEST_WRITEBLOCK8_VAL_NC[5],
                                  TEST_WRITEBLOCK8_VAL_NC[6], TEST_WRITEBLOCK8_VAL_NC[7],
                                  TEST_WRITEBLOCK8_VAL_NC[8], TEST_WRITEBLOCK8_VAL_NC[9],
                                  TEST_WRITEBLOCK8_VAL_NC[10], TEST_WRITEBLOCK8_VAL_NC[11],
                                  TEST_WRITEBLOCK8_VAL_NC[12], TEST_WRITEBLOCK8_VAL_NC[13],
                                  TEST_WRITEBLOCK8_VAL_NC[14], TEST_WRITEBLOCK8_VAL_NC[15]};
    uintptr_t offset = TEST_WRITEBLOCK8_REG_OFFSET_NC;
    size_t size = 16;
    st_osal_mq_config local_mq_config;

    if(testStruct->uid == TEST_THREAD_ID2)
    {
        EXPECT_EQ(R_OSAL_MqGetConfig(testStruct->mq, &local_mq_config), OSAL_RETURN_OK) << "Test06_017_011 failed";
        EXPECT_EQ(R_OSAL_IoBlockRead8(*(testStruct->device_handle), offset, testStruct->init_block_data8, size), OSAL_RETURN_OK) << "Test06_017_011 failed";
        EXPECT_EQ(R_OSAL_IoBlockWrite8(*(testStruct->device_handle), offset, register_data8, size), OSAL_RETURN_OK) << "Test06_017_011 failed";
        EXPECT_EQ(R_OSAL_MqSendForTimePeriod(testStruct->mq, SYNC_MQ_TIME, &testStruct->send_msg, local_mq_config.msg_size), OSAL_RETURN_OK) << "Test06_017_011 failed";
    }
    else if(testStruct->uid == TEST_THREAD_ID3)
    {
        EXPECT_EQ(R_OSAL_IoBlockRead8(*(testStruct->device_handle), offset, read_data8, size), OSAL_RETURN_OK) << "Test06_017_011 failed";
        EXPECT_TRUE(equal(begin(register_data8), end(register_data8), begin(read_data8))) << "Test06_017_011 failed";
        EXPECT_EQ(R_OSAL_IoBlockWrite8(*(testStruct->device_handle), offset, testStruct->init_block_data8, size), OSAL_RETURN_OK) << "Test06_017_011 failed";
    }
    else
    {
    }
    return (int64_t) osal_ret;
}
// [io_block_read_write_8_device_child_thread]

// [io_block_read_write_16_device_child_thread]
int64_t io_block_read_write_16_device_child_thread(void * arg)
{
    io_test_multiple_thread_worker_arg *testStruct = (io_test_multiple_thread_worker_arg *)arg;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint16_t read_data16[8];
    uint16_t register_data16[8] = {TEST_WRITEBLOCK16_VAL_NC[0], TEST_WRITEBLOCK16_VAL_NC[1],
                                   TEST_WRITEBLOCK16_VAL_NC[2], TEST_WRITEBLOCK16_VAL_NC[3],
                                   TEST_WRITEBLOCK16_VAL_NC[4], TEST_WRITEBLOCK16_VAL_NC[5],
                                   TEST_WRITEBLOCK16_VAL_NC[6], TEST_WRITEBLOCK16_VAL_NC[7]};
    uintptr_t offset = TEST_WRITEBLOCK16_REG_OFFSET_NC;
    size_t size = 16;
    st_osal_mq_config local_mq_config;

    if(testStruct->uid == TEST_THREAD_ID2)
    {
        EXPECT_EQ(R_OSAL_MqGetConfig(testStruct->mq, &local_mq_config), OSAL_RETURN_OK) << "Test06_018_011 failed";
        EXPECT_EQ(R_OSAL_IoBlockRead16(*(testStruct->device_handle), offset, testStruct->init_block_data16, size), OSAL_RETURN_OK) << "Test06_018_011 failed";
        EXPECT_EQ(R_OSAL_IoBlockWrite16(*(testStruct->device_handle), offset, register_data16, size), OSAL_RETURN_OK) << "Test06_018_011 failed";
        EXPECT_EQ(R_OSAL_MqSendForTimePeriod(testStruct->mq, SYNC_MQ_TIME, &testStruct->send_msg, local_mq_config.msg_size), OSAL_RETURN_OK) << "Test06_018_011 failed";
    }
    else if(testStruct->uid == TEST_THREAD_ID3)
    {
        EXPECT_EQ(R_OSAL_IoBlockRead16(*(testStruct->device_handle), offset, read_data16, size), OSAL_RETURN_OK) << "Test06_018_011 failed";
        EXPECT_TRUE(equal(begin(register_data16), end(register_data16), begin(read_data16))) << "Test06_018_011 failed";
        EXPECT_EQ(R_OSAL_IoBlockWrite16(*(testStruct->device_handle), offset, testStruct->init_block_data16, size), OSAL_RETURN_OK) << "Test06_018_011 failed";
    }
    else
    {
    }
    return (int64_t) osal_ret;
}
// [io_block_read_write_16_device_child_thread]

// [io_block_read_write_32_device_child_thread]
int64_t io_block_read_write_32_device_child_thread(void * arg)
{
    io_test_multiple_thread_worker_arg *testStruct = (io_test_multiple_thread_worker_arg *)arg;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint32_t read_data32[4];
    uint32_t register_data32[4] = {TEST_WRITEBLOCK32_VAL_NC[0], TEST_WRITEBLOCK32_VAL_NC[1], TEST_WRITEBLOCK32_VAL_NC[2], TEST_WRITEBLOCK32_VAL_NC[3]};
    uintptr_t offset = TEST_WRITEBLOCK32_REG_OFFSET_NC;
    size_t size = 16;
    st_osal_mq_config local_mq_config;

    if(testStruct->uid == TEST_THREAD_ID2)
    {
        EXPECT_EQ(R_OSAL_MqGetConfig(testStruct->mq, &local_mq_config), OSAL_RETURN_OK) << "Test06_019_011 failed";
        EXPECT_EQ(R_OSAL_IoBlockRead32(*(testStruct->device_handle), offset, testStruct->init_block_data32, size), OSAL_RETURN_OK) << "Test06_019_011 failed";
        EXPECT_EQ(R_OSAL_IoBlockWrite32(*(testStruct->device_handle), offset, register_data32, size), OSAL_RETURN_OK) << "Test06_019_011 failed";
        EXPECT_EQ(R_OSAL_MqSendForTimePeriod(testStruct->mq, SYNC_MQ_TIME, &testStruct->send_msg, local_mq_config.msg_size), OSAL_RETURN_OK) << "Test06_019_011 failed";
    }
    else if(testStruct->uid == TEST_THREAD_ID3)
    {
        EXPECT_EQ(R_OSAL_IoBlockRead32(*(testStruct->device_handle), offset, read_data32, size), OSAL_RETURN_OK) << "Test06_019_011 failed";
        EXPECT_TRUE(equal(begin(register_data32), end(register_data32), begin(read_data32))) << "Test06_019_011 failed";
        EXPECT_EQ(R_OSAL_IoBlockWrite32(*(testStruct->device_handle), offset, testStruct->init_block_data32, size), OSAL_RETURN_OK) << "Test06_019_011 failed";
    }
    else
    {
    }
    return (int64_t) osal_ret;
}
// [io_block_read_write_32_device_child_thread]

// [io_block_read_write_64_device_child_thread]
int64_t io_block_read_write_64_device_child_thread(void * arg)
{
    io_test_multiple_thread_worker_arg *testStruct = (io_test_multiple_thread_worker_arg *)arg;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint64_t read_data64[2];
    uint64_t register_data64[2] = {TEST_WRITEBLOCK64_VAL_NC[0], TEST_WRITEBLOCK64_VAL_NC[1]};
    uintptr_t offset = TEST_WRITEBLOCK64_REG_OFFSET_NC;
    size_t size = 16;
    st_osal_mq_config local_mq_config;

    if(testStruct->uid == TEST_THREAD_ID2)
    {
        EXPECT_EQ(R_OSAL_MqGetConfig(testStruct->mq, &local_mq_config), OSAL_RETURN_OK) << "Test06_020_011 failed";
        EXPECT_EQ(R_OSAL_IoBlockRead64(*(testStruct->device_handle), offset, testStruct->init_block_data64, size), OSAL_RETURN_OK) << "Test06_020_011 failed";
        EXPECT_EQ(R_OSAL_IoBlockWrite64(*(testStruct->device_handle), offset, register_data64, size), OSAL_RETURN_OK) << "Test06_020_011 failed";
        EXPECT_EQ(R_OSAL_MqSendForTimePeriod(testStruct->mq, SYNC_MQ_TIME, &testStruct->send_msg, local_mq_config.msg_size), OSAL_RETURN_OK) << "Test06_020_011 failed";
    }
    else if(testStruct->uid == TEST_THREAD_ID3)
    {
        EXPECT_EQ(R_OSAL_IoBlockRead64(*(testStruct->device_handle), offset, read_data64, size), OSAL_RETURN_OK) << "Test06_020_011 failed";
        EXPECT_TRUE(equal(begin(register_data64), end(register_data64), begin(read_data64))) << "Test06_020_011 failed";
        EXPECT_EQ(R_OSAL_IoBlockWrite64(*(testStruct->device_handle), offset, testStruct->init_block_data64, size), OSAL_RETURN_OK) << "Test06_020_011 failed";
    }
    else
    {
    }
    return (int64_t) osal_ret;
}
// [io_block_read_write_64_device_child_thread]

vector<string> IOApiTest::device_list;
vector<IOApiTest::st_osal_test_info_t> IOApiTest::devices;

// [osal_io_test_cb_sample]
void osal_io_test_cb_sample(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    (void)dev_handle;
    (void)irq_channel;
    (void)irq_arg;
}
// [osal_io_test_cb_sample]

typedef struct st_io_arg
{
    size_t numOfDevice;
    char* deviceList;
    size_t byteWritten;
} st_io_arg_t;

struct device_close_abnormal_worker_arg
{
    device_close_abnormal_worker_arg(osal_mutex_handle_t * mtx_handle, osal_device_handle_t * dev_handle) : mutex_handle(mtx_handle), device_handle(dev_handle)
    {
    }
    osal_mutex_handle_t * mutex_handle;
    osal_device_handle_t * device_handle;
    osal_mq_handle_t mq;
    int send_msg;
};

// [osal_io_isr_test_TC_06_005_008]
void osal_io_isr_test_TC_06_005_008(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    uint8_t read_data8;
    uint8_t expected_data = TEST_READ8_VAL_INT;
    uintptr_t offset = TEST_REG_CLEAR_REGISTER;
    uint32_t write_data = TEST_CLEAR_REGISTER_VALUE;
    osal_milli_sec_t time_0ms = 0;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, offset, write_data), OSAL_RETURN_OK);

    // Test06_005_008
    // @brief: normal case: read8 processing in ISR
    offset = TEST_READ8_REG_OFFSET_INT;
    EXPECT_EQ(R_OSAL_IoRead8(dev_handle, offset, &read_data8), OSAL_RETURN_OK) << "Test06_005_008 failed";
    EXPECT_EQ(read_data8, expected_data) << "Test06_005_008 failed";

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, time_0ms, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_io_isr_test_TC_06_005_008]

// [osal_io_isr_test_TC_06_006_008]
void osal_io_isr_test_TC_06_006_008(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    uint16_t read_data16;
    uint16_t expected_data = TEST_READ16_VAL_INT;
    uintptr_t offset = TEST_REG_CLEAR_REGISTER;
    uint32_t write_data = TEST_CLEAR_REGISTER_VALUE;
    osal_milli_sec_t time_0ms = 0;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, offset, write_data), OSAL_RETURN_OK);

    // Test06_006_008
    // @brief: normal case: read16 processing in ISR
    offset = TEST_READ16_REG_OFFSET_INT;
    EXPECT_EQ(R_OSAL_IoRead16(dev_handle, offset, &read_data16), OSAL_RETURN_OK) << "Test06_006_008 failed";
    EXPECT_EQ(read_data16, expected_data) << "Test06_006_008 failed";

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, time_0ms, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_io_isr_test_TC_06_006_008]

// [osal_io_isr_test_TC_06_007_008]
void osal_io_isr_test_TC_06_007_008(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    uint32_t read_data32;
    uint32_t expected_data = TEST_READ32_VAL_INT;
    uintptr_t offset = TEST_REG_CLEAR_REGISTER;
    uint32_t write_data = TEST_CLEAR_REGISTER_VALUE;
    osal_milli_sec_t time_0ms = 0;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, offset, write_data), OSAL_RETURN_OK);

    // Test06_007_008
    // @brief: normal case: read32 processing in ISR
    offset = TEST_READ32_REG_OFFSET_INT; // Status Register
    EXPECT_EQ(R_OSAL_IoRead32(dev_handle, offset, &read_data32), OSAL_RETURN_OK) << "Test06_007_008 failed";
    EXPECT_EQ(read_data32, expected_data)  << "Test06_007_008 failed";

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, time_0ms, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_io_isr_test_TC_06_007_008]

// [osal_io_isr_test_TC_06_008_008]
void osal_io_isr_test_TC_06_008_008(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    uint64_t read_data64;
    uint64_t expected_data = TEST_READ64_VAL_INT;
    uintptr_t offset = TEST_REG_CLEAR_REGISTER;
    uint32_t write_data = TEST_CLEAR_REGISTER_VALUE;
    osal_milli_sec_t time_0ms = 0;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, offset, write_data), OSAL_RETURN_OK);

    // Test06_008_008
    // @brief: normal case: read64 processing in ISR
    offset = TEST_READ64_REG_OFFSET_INT;
    EXPECT_EQ(R_OSAL_IoRead64(dev_handle, offset, &read_data64), OSAL_RETURN_OK) << "Test06_008_008 failed";
    EXPECT_EQ(read_data64, expected_data) << "Test06_008_008 failed";

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, time_0ms, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_io_isr_test_TC_06_008_008]

// [osal_io_isr_test_TC_06_009_009]
void osal_io_isr_test_TC_06_009_009(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    size_t size = 4;
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    uint8_t readblock_data8[4];
    uint8_t register_data8[4] = {TEST_READBLOCK8_VAL_INT[0], TEST_READBLOCK8_VAL_INT[1], TEST_READBLOCK8_VAL_INT[2], TEST_READBLOCK8_VAL_INT[3]};
    uintptr_t offset = TEST_REG_CLEAR_REGISTER;
    uint32_t write_data = TEST_CLEAR_REGISTER_VALUE;
    osal_milli_sec_t time_0ms = 0;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, offset, write_data), OSAL_RETURN_OK);

    // Test06_009_009
    // @brief: normal case: blockread8 processing in ISR
    offset = TEST_READBLOCK8_REG_OFFSET_INT;
    EXPECT_EQ(R_OSAL_IoBlockRead8(dev_handle, offset, readblock_data8, size), OSAL_RETURN_OK) << "Test06_009_009 failed";
    EXPECT_TRUE(equal(begin(register_data8), end(register_data8), begin(readblock_data8))) << "Test06_009_009 failed";

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, time_0ms, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_io_isr_test_TC_06_009_009]

// [osal_io_isr_test_TC_06_010_009]
void osal_io_isr_test_TC_06_010_009(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    size_t size = 4;
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    uint16_t readblock_data16[2];
    uint16_t register_data16[2] = {TEST_READBLOCK16_VAL_INT[0], TEST_READBLOCK16_VAL_INT[1]};
    uintptr_t offset = TEST_REG_CLEAR_REGISTER;
    uint32_t write_data = TEST_CLEAR_REGISTER_VALUE;
    osal_milli_sec_t time_0ms = 0;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, offset, write_data), OSAL_RETURN_OK);

    // Test06_010_009
    // @brief: normal case: blockread16 processing in ISR
    offset = TEST_READBLOCK16_REG_OFFSET_INT;
    EXPECT_EQ(R_OSAL_IoBlockRead16(dev_handle, offset, readblock_data16, size), OSAL_RETURN_OK) << "Test06_010_009 failed";
    EXPECT_TRUE(equal(begin(register_data16), end(register_data16), begin(readblock_data16))) << "Test06_010_009 failed";

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, time_0ms, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_io_isr_test_TC_06_010_009]

// [osal_io_isr_test_TC_06_011_009]
void osal_io_isr_test_TC_06_011_009(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    size_t size = 8;
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    uint32_t readblock_data32[2];
    uint32_t register_data32[2] = {TEST_READBLOCK32_VAL_INT[0], TEST_READBLOCK32_VAL_INT[1]};
    uintptr_t offset = TEST_REG_CLEAR_REGISTER;
    uint32_t write_data = TEST_CLEAR_REGISTER_VALUE;
    osal_milli_sec_t time_0ms = 0;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, offset, write_data), OSAL_RETURN_OK);

    // Test06_011_009
    // @brief: normal case: blockread32 processing in ISR
    offset = TEST_READBLOCK32_REG_OFFSET_INT;
    EXPECT_EQ(R_OSAL_IoBlockRead32(dev_handle, offset, readblock_data32, size), OSAL_RETURN_OK) << "Test06_011_009 failed";
    readblock_data32[0] = readblock_data32[0] & 0xFFFFFFFF;
    readblock_data32[1] = readblock_data32[1] & 0x00000000;
    EXPECT_TRUE(equal(begin(register_data32), end(register_data32), begin(readblock_data32))) << "Test06_011_009 failed";

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, time_0ms, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_io_isr_test_TC_06_011_009]

// [osal_io_isr_test_TC_06_012_009]
void osal_io_isr_test_TC_06_012_009(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    size_t size = 16;
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    uint64_t readblock_data64[2];
    uint64_t register_data64[2] = {TEST_READBLOCK64_VAL_INT[0], TEST_READBLOCK64_VAL_INT[1]};
    uintptr_t offset = TEST_REG_CLEAR_REGISTER;
    uint32_t write_data = TEST_CLEAR_REGISTER_VALUE;
    osal_milli_sec_t time_0ms = 0;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, offset, write_data), OSAL_RETURN_OK);

    // Test06_012_009
    // @brief: normal case: blockread64 processing in ISR
    offset = TEST_READBLOCK64_REG_OFFSET_INT;
    EXPECT_EQ(R_OSAL_IoBlockRead64(dev_handle, offset, readblock_data64, size), OSAL_RETURN_OK) << "Test06_012_009 failed";
    EXPECT_TRUE(equal(begin(register_data64), end(register_data64), begin(readblock_data64))) << "Test06_012_009 failed";

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, time_0ms, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_io_isr_test_TC_06_012_009]

// [osal_io_isr_test_TC_06_013_007]
void osal_io_isr_test_TC_06_013_007(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    uint8_t init_data8;
    uint8_t read_data8;
    uint8_t write_data8 = TEST_WRITE8_VAL_INT;
    uintptr_t offset = TEST_REG_CLEAR_REGISTER;
    uint32_t write_data = TEST_CLEAR_REGISTER_VALUE;
    osal_milli_sec_t time_0ms = 0;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, offset, write_data), OSAL_RETURN_OK);

    // Test06_013_007
    // @brief: normal case: write8 processing in ISR
    offset = TEST_WRITE8_REG_OFFSET_INT;

    /* Read initialization value */
    EXPECT_EQ(R_OSAL_IoRead8(dev_handle, offset, &init_data8), OSAL_RETURN_OK) << "Test06_013_007 failed";

    EXPECT_EQ(R_OSAL_IoWrite8(dev_handle, offset, write_data8), OSAL_RETURN_OK) << "Test06_013_007 failed";
    EXPECT_EQ(R_OSAL_IoRead8(dev_handle, offset, &read_data8), OSAL_RETURN_OK)  << "Test06_013_007 failed";
    EXPECT_EQ(read_data8, write_data8) << "Test06_013_007 failed";

    /* Write back initialization value */
    EXPECT_EQ(R_OSAL_IoWrite8(dev_handle, offset, init_data8), OSAL_RETURN_OK) << "Test06_013_007 failed";

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, time_0ms, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_io_isr_test_TC_06_013_007]

// [osal_io_isr_test_TC_06_014_007]
void osal_io_isr_test_TC_06_014_007(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    uint16_t init_data16;
    uint16_t read_data16;
    uint16_t write_data16 = TEST_WRITE16_VAL_INT;
    uintptr_t offset = TEST_REG_CLEAR_REGISTER;
    uint32_t write_data = TEST_CLEAR_REGISTER_VALUE;
    osal_milli_sec_t time_0ms = 0;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, offset, write_data), OSAL_RETURN_OK);

    // Test06_014_007
    // @brief: normal case: write16 processing in ISR
    offset = TEST_WRITE16_REG_OFFSET_INT;

    /* Read initialization value */
    EXPECT_EQ(R_OSAL_IoRead16(dev_handle, offset, &init_data16), OSAL_RETURN_OK) << "Test06_014_007 failed";

    EXPECT_EQ(R_OSAL_IoWrite16(dev_handle, offset, write_data16), OSAL_RETURN_OK) << "Test06_014_007 failed";
    EXPECT_EQ(R_OSAL_IoRead16(dev_handle, offset, &read_data16), OSAL_RETURN_OK) << "Test06_014_007 failed";
    EXPECT_EQ(read_data16, write_data16) << "Test06_014_007 failed";

    /* Write back initialization value */
    EXPECT_EQ(R_OSAL_IoWrite16(dev_handle, offset, init_data16), OSAL_RETURN_OK) << "Test06_014_007 failed";

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, time_0ms, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_io_isr_test_TC_06_014_007]

// [osal_io_isr_test_TC_06_015_007]
void osal_io_isr_test_TC_06_015_007(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    uint32_t init_data32;
    uint32_t read_data32;
    uint32_t write_data32 = TEST_WRITE32_VAL_INT;
    uintptr_t offset = TEST_REG_CLEAR_REGISTER;
    uint32_t write_data = TEST_CLEAR_REGISTER_VALUE;
    osal_milli_sec_t time_0ms = 0;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, offset, write_data), OSAL_RETURN_OK);

    // Test06_015_007
    // @brief: normal case: write32 processing in ISR
    offset = TEST_WRITE32_REG_OFFSET_INT;

    /* Read initialization value */
    EXPECT_EQ(R_OSAL_IoRead32(dev_handle, offset, &init_data32), OSAL_RETURN_OK) << "Test06_015_007 failed";

    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, offset, write_data32), OSAL_RETURN_OK) << "Test06_015_007 failed";
    EXPECT_EQ(R_OSAL_IoRead32(dev_handle, offset, &read_data32), OSAL_RETURN_OK) << "Test06_015_007 failed";
    EXPECT_EQ(read_data32, write_data32)  << "Test06_015_007 failed";

    /* Write back initialization value */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, offset, init_data32), OSAL_RETURN_OK) << "Test06_015_007 failed";

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, time_0ms, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_io_isr_test_TC_06_015_007]

// [osal_io_isr_test_TC_06_016_007]
void osal_io_isr_test_TC_06_016_007(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    uint64_t init_data64;
    uint64_t read_data64;
    uint64_t write_data64 = TEST_WRITE64_VAL_INT;
    uintptr_t offset = TEST_REG_CLEAR_REGISTER;
    uint32_t write_data = TEST_CLEAR_REGISTER_VALUE;
    osal_milli_sec_t time_0ms = 0;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, offset, write_data), OSAL_RETURN_OK);

    // Test06_016_007
    // @brief: normal case: write64 processing in ISR
    offset = TEST_WRITE64_REG_OFFSET_INT;

    /* Read initialization value */
    EXPECT_EQ(R_OSAL_IoRead64(dev_handle, offset, &init_data64), OSAL_RETURN_OK) << "Test06_016_007 failed";

    EXPECT_EQ(R_OSAL_IoWrite64(dev_handle, offset, write_data64), OSAL_RETURN_OK) << "Test06_016_007 failed";
    EXPECT_EQ(R_OSAL_IoRead64(dev_handle, offset, &read_data64), OSAL_RETURN_OK) << "Test06_016_007 failed";
    EXPECT_EQ(read_data64, write_data64) << "Test06_016_007 failed";

    /* Write back initialization value */
    EXPECT_EQ(R_OSAL_IoWrite64(dev_handle, offset, init_data64), OSAL_RETURN_OK) << "Test06_016_007 failed";

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, time_0ms, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_io_isr_test_TC_06_016_007]

// [osal_io_isr_test_TC_06_017_009]
void osal_io_isr_test_TC_06_017_009(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    size_t size = 16;
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    uint8_t init_data8[16];
    uint8_t readblock_data8[16];
    uint8_t registerblock_data8[16] = {TEST_WRITEBLOCK8_VAL_INT[0], TEST_WRITEBLOCK8_VAL_INT[1],
                                       TEST_WRITEBLOCK8_VAL_INT[2], TEST_WRITEBLOCK8_VAL_INT[3],
                                       TEST_WRITEBLOCK8_VAL_INT[4], TEST_WRITEBLOCK8_VAL_INT[5],
                                       TEST_WRITEBLOCK8_VAL_INT[6], TEST_WRITEBLOCK8_VAL_INT[7],
                                       TEST_WRITEBLOCK8_VAL_INT[8], TEST_WRITEBLOCK8_VAL_INT[9],
                                       TEST_WRITEBLOCK8_VAL_INT[10], TEST_WRITEBLOCK8_VAL_INT[11],
                                       TEST_WRITEBLOCK8_VAL_INT[12], TEST_WRITEBLOCK8_VAL_INT[13],
                                       TEST_WRITEBLOCK8_VAL_INT[14], TEST_WRITEBLOCK8_VAL_INT[15]};
    uintptr_t offset = TEST_REG_CLEAR_REGISTER;
    uint32_t write_data = TEST_CLEAR_REGISTER_VALUE;
    osal_milli_sec_t time_0ms = 0;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, offset, write_data), OSAL_RETURN_OK);

    // Test06_017_009
    // @brief: normal case: blockwrite8 processing in ISR
    offset = TEST_WRITEBLOCK8_REG_OFFSET_INT;

    /* Read initialization value */
    EXPECT_EQ(R_OSAL_IoBlockRead8(dev_handle, offset, init_data8, size), OSAL_RETURN_OK) << "Test06_017_009 failed";

    EXPECT_EQ(R_OSAL_IoBlockWrite8(dev_handle, offset, registerblock_data8, size), OSAL_RETURN_OK) << "Test06_017_009 failed";
    EXPECT_EQ(R_OSAL_IoBlockRead8(dev_handle, offset, readblock_data8, size), OSAL_RETURN_OK) << "Test06_017_009 failed";
    EXPECT_TRUE(equal(begin(registerblock_data8), end(registerblock_data8), begin(readblock_data8))) << "Test06_017_009 failed";

    /* Write back initialization value */
    EXPECT_EQ(R_OSAL_IoBlockWrite8(dev_handle, offset, init_data8, size), OSAL_RETURN_OK) << "Test06_017_009 failed";

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, time_0ms, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_io_isr_test_TC_06_017_009]

// [osal_io_isr_test_TC_06_018_009]
void osal_io_isr_test_TC_06_018_009(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    size_t size = 16;
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    uint16_t init_data16[8];
    uint16_t readblock_data16[8];
    uint16_t registerblock_data16[8] = {TEST_WRITEBLOCK16_VAL_INT[0], TEST_WRITEBLOCK16_VAL_INT[1],
                                        TEST_WRITEBLOCK16_VAL_INT[2], TEST_WRITEBLOCK16_VAL_INT[3],
                                        TEST_WRITEBLOCK16_VAL_INT[4], TEST_WRITEBLOCK16_VAL_INT[5],
                                        TEST_WRITEBLOCK16_VAL_INT[6], TEST_WRITEBLOCK16_VAL_INT[7]};
    uintptr_t offset = TEST_REG_CLEAR_REGISTER;
    uint32_t write_data = TEST_CLEAR_REGISTER_VALUE;
    osal_milli_sec_t time_0ms = 0;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, offset, write_data), OSAL_RETURN_OK);

    // Test06_018_009
    // @brief: normal case: blockwrite16 processing in ISR
    offset = TEST_WRITEBLOCK16_REG_OFFSET_INT;

    /* Read initialization value */
    EXPECT_EQ(R_OSAL_IoBlockRead16(dev_handle, offset, init_data16, size), OSAL_RETURN_OK) << "Test06_018_009 failed";

    EXPECT_EQ(R_OSAL_IoBlockWrite16(dev_handle, offset, registerblock_data16, size), OSAL_RETURN_OK) << "Test06_018_009 failed";
    EXPECT_EQ(R_OSAL_IoBlockRead16(dev_handle, offset, readblock_data16, size), OSAL_RETURN_OK) << "Test06_018_009 failed";
    EXPECT_TRUE(equal(begin(registerblock_data16), end(registerblock_data16), begin(readblock_data16))) << "Test06_018_009 failed";

    /* Write back initialization value */
    EXPECT_EQ(R_OSAL_IoBlockWrite16(dev_handle, offset, init_data16, size), OSAL_RETURN_OK) << "Test06_018_009 failed";

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, time_0ms, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_io_isr_test_TC_06_018_009]

// [osal_io_isr_test_TC_06_019_009]
void osal_io_isr_test_TC_06_019_009(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    size_t size = 16;
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    uint32_t init_data32[4];
    uint32_t readblock_data32[4];
    uint32_t registerblock_data32[4] = {TEST_WRITEBLOCK32_VAL_INT[0], TEST_WRITEBLOCK32_VAL_INT[1], TEST_WRITEBLOCK32_VAL_INT[2], TEST_WRITEBLOCK32_VAL_INT[3]};
    uintptr_t offset = TEST_REG_CLEAR_REGISTER;
    uint32_t write_data = TEST_CLEAR_REGISTER_VALUE;
    osal_milli_sec_t time_0ms = 0;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, offset, write_data), OSAL_RETURN_OK);

    // Test06_019_009
    // @brief: normal case: blockwrite32 processing in ISR
    offset = TEST_WRITEBLOCK32_REG_OFFSET_INT;

    /* Read initialization value */
    EXPECT_EQ(R_OSAL_IoBlockRead32(dev_handle, offset, init_data32, size), OSAL_RETURN_OK) << "Test06_019_009 failed";

    EXPECT_EQ(R_OSAL_IoBlockWrite32(dev_handle, offset, registerblock_data32, size), OSAL_RETURN_OK) << "Test06_019_009 failed";
    EXPECT_EQ(R_OSAL_IoBlockRead32(dev_handle, offset, readblock_data32, size), OSAL_RETURN_OK) << "Test06_019_009 failed";
    EXPECT_TRUE(equal(begin(registerblock_data32), end(registerblock_data32), begin(readblock_data32))) << "Test06_019_009 failed";

    /* Write back initialization value */
    EXPECT_EQ(R_OSAL_IoBlockWrite32(dev_handle, offset, init_data32, size), OSAL_RETURN_OK) << "Test06_019_009 failed";

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, time_0ms, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_io_isr_test_TC_06_019_009]

// [osal_io_isr_test_TC_06_020_009]
void osal_io_isr_test_TC_06_020_009(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    size_t size = 16;
    OSALINTTESTS::st_callback_arg_t* local_arg = (OSALINTTESTS::st_callback_arg_t*)irq_arg;
    uint64_t init_data64[2];
    uint64_t readblock_data64[2];
    uint64_t registerblock_data64[2] = {TEST_WRITEBLOCK64_VAL_INT[0], TEST_WRITEBLOCK64_VAL_INT[1]};
    uintptr_t offset = TEST_REG_CLEAR_REGISTER;
    uint32_t write_data = TEST_CLEAR_REGISTER_VALUE;
    osal_milli_sec_t time_0ms = 0;

    /* Disable interrupt */
    EXPECT_EQ(R_OSAL_InterruptDisableIsr(dev_handle, irq_channel), OSAL_RETURN_OK);

    /* Clear the status register */
    EXPECT_EQ(R_OSAL_IoWrite32(dev_handle, offset, write_data), OSAL_RETURN_OK);

    // Test06_020_009
    // @brief: normal case: blockwrite64 processing in ISR
    offset = TEST_WRITEBLOCK64_REG_OFFSET_INT;

    /* Read initialization value */
    EXPECT_EQ(R_OSAL_IoBlockRead64(dev_handle, offset, init_data64, size), OSAL_RETURN_OK) << "Test06_020_009 failed";

    EXPECT_EQ(R_OSAL_IoBlockWrite64(dev_handle, offset, registerblock_data64, size), OSAL_RETURN_OK) << "Test06_020_009 failed";
    EXPECT_EQ(R_OSAL_IoBlockRead64(dev_handle, offset, readblock_data64, size), OSAL_RETURN_OK) << "Test06_020_009 failed";
    EXPECT_TRUE(equal(begin(registerblock_data64), end(registerblock_data64), begin(readblock_data64))) << "Test06_020_009 failed";

    /* Write back initialization value */
    EXPECT_EQ(R_OSAL_IoBlockWrite64(dev_handle, offset, init_data64, size), OSAL_RETURN_OK) << "Test06_020_009 failed";

    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(local_arg->mq_handle, time_0ms, &local_arg->send_msg, sizeof(local_arg->send_msg)), OSAL_RETURN_OK);
}
// [osal_io_isr_test_TC_06_020_009]

// [device_wait_pm_state_worker_function]
int64_t device_wait_pm_state_worker_function(void * arg)
{
    device_close_abnormal_worker_arg *testStruct = (device_close_abnormal_worker_arg *)arg;
    osal_milli_sec_t pm_timeout = 2000;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    bool applyImmediate;
    st_osal_mq_config local_mq_config;

    EXPECT_EQ(R_OSAL_MqGetConfig(testStruct->mq, &local_mq_config), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_ThsyncMutexTryLock(*(testStruct->mutex_handle)), OSAL_RETURN_OK) << "Test06_004_006 failed";
    EXPECT_EQ(R_OSAL_MqSendForTimePeriod(testStruct->mq, SYNC_MQ_TIME, &testStruct->send_msg, local_mq_config.msg_size), OSAL_RETURN_OK);
    applyImmediate = true;
    EXPECT_EQ(R_OSAL_PmSetRequiredState(*(testStruct->device_handle), OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK) << "Test06_004_006 failed";
    EXPECT_EQ(R_OSAL_PmSetPolicy(*(testStruct->device_handle), OSAL_PM_POLICY_HP, applyImmediate), OSAL_RETURN_OK) << "Test06_004_006 failed";
    applyImmediate = false;
    EXPECT_EQ(R_OSAL_PmSetRequiredState(*(testStruct->device_handle), OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test06_004_006 failed";
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(*(testStruct->mutex_handle)), OSAL_RETURN_OK) << "Test06_004_006 failed";
    osal_ret = R_OSAL_PmWaitForState(*(testStruct->device_handle), OSAL_PM_STATE_PG, pm_timeout);
    EXPECT_EQ(osal_ret, OSAL_RETURN_TIME) << "Test06_004_006 failed";
    return (int64_t)osal_ret;
}
// [device_wait_pm_state_worker_function]

// [device_close_woker_function]
int64_t device_close_woker_function(void * arg)
{
    device_close_abnormal_worker_arg *testStruct = (device_close_abnormal_worker_arg *)arg;
    osal_milli_sec_t mutex_timeout = 2000;
    osal_milli_sec_t sleep_time = 1;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    EXPECT_EQ(R_OSAL_ThsyncMutexLockForTimePeriod(*(testStruct->mutex_handle), mutex_timeout), OSAL_RETURN_OK) << "Test06_004_006 failed";
    osal_ret = R_OSAL_IoDeviceClose(*(testStruct->device_handle));
    EXPECT_EQ(R_OSAL_ThsyncMutexUnlock(*(testStruct->mutex_handle)), OSAL_RETURN_OK) << "Test06_004_006 failed";
    EXPECT_EQ(osal_ret, OSAL_RETURN_BUSY) << "Test06_004_006 failed";
    return (int64_t)osal_ret;
}
// [device_close_woker_function]

#ifndef SKIP_SECTION_1
/* RVC implementation base on Linux OSAL IT*/
/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoGetNumOfDevices_TC_06_001_001 IOApiTest.IoGetNumOfDevices_TC_06_001_001
 * @subsection IoGetNumOfDevices_TC_06_001_001_tsi Test Suite ID
 * IOApiTest
 * @subsection IoGetNumOfDevices_TC_06_001_001_tci Test Case ID
 * IoGetNumOfDevices_S1_N_TC_06_001_001
 * @subsection IoGetNumOfDevices_TC_06_001_001_rr Related Requirement(s)
 * \#199208, \#199211
 * @subsection IoGetNumOfDevices_TC_06_001_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - "devices" contains device type, number of device and device list buffer of available devices which will be used for testing.
 * @subsection IoGetNumOfDevices_TC_06_001_001_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetNumOfDevicesNormal function in normal condition:
 * - Test06_001_001: get number of device corresponding with some device type.
 * @subsection IoGetNumOfDevices_TC_06_001_001_tpts Test Procedure/Test Steps
 * -# Get the number of device corresponding with each "device_type" of "devices" by using R_OSAL_IoGetNumOfDevices.
 * @subsection IoGetNumOfDevices_TC_06_001_001_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfDevices return OSAL_RETURN_OK.<br>
 * Value of "numOfDevice" is equal to "numOfDevice" of "devices".
 * @subsection IoGetNumOfDevices_TC_06_001_001_testcase Test Case Example
 * @snippet test_io.cpp IoGetNumOfDevices_TC_06_001_001
 */
// [IoGetNumOfDevices_TC_06_001_001]
TEST_F(IOApiTest, IoGetNumOfDevices_TC_06_001_001)
{
    size_t numOfDevice;

// Test06_001_001
// @brief: normal case: get number of device corresponding with some device type.
    for (st_osal_test_info_t dev : IOApiTest::devices)
    {
        ASSERT_EQ(R_OSAL_IoGetNumOfDevices(dev.device_type, &numOfDevice), OSAL_RETURN_OK) << "Test06_001_001 failed. Device type: " << dev.device_type;
        EXPECT_EQ(numOfDevice, dev.numOfDevice) << "Test06_001_001 failed. Device type: " << dev.device_type;
    }
}
// [IoGetNumOfDevices_TC_06_001_001]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoGetNumOfDevices_TC_06_001_002 IOApiTest.IoGetNumOfDevices_TC_06_001_002
 * @subsection IoGetNumOfDevices_TC_06_001_002_tsi Test Suite ID
 * IOApiTest
 * @subsection IoGetNumOfDevices_TC_06_001_002_tci Test Case ID
 * IoGetNumOfDevices_S0_A_TC_06_001_002
 * @subsection IoGetNumOfDevices_TC_06_001_002_rr Related Requirement(s)
 * \#199208
 * @subsection IoGetNumOfDevices_TC_06_001_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetNumOfDevices_TC_06_001_002_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetNumOfDevices function in abnormal condition:
 * - Test06_001_002: get number of device with device type is NULL
 * @subsection IoGetNumOfDevices_TC_06_001_002_tpts Test Procedure/Test Steps
 * -# Get the number of device by using R_OSAL_IoGetNumOfDevices with the arguments as below:
 *      - Device type is NULL
 *      - Address of "numOfDevice"
 * @subsection IoGetNumOfDevices_TC_06_001_002_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfDevices return OSAl_RETURN_PAR.
 * @subsection IoGetNumOfDevices_TC_06_001_002_testcase Test Case Example
 * @snippet test_io.cpp IoGetNumOfDevices_TC_06_001_002
 */
// [IoGetNumOfDevices_TC_06_001_002]
TEST_F(IOApiTest, IoGetNumOfDevices_TC_06_001_002)
{
    size_t numOfDevice;
    char* p_deviceType = NULL;

// Test06_001_002
// @brief: abnormal case: deviceType is NULL
    ASSERT_EQ(R_OSAL_IoGetNumOfDevices(p_deviceType, &numOfDevice), OSAL_RETURN_PAR) << "Test06_001_002 failed";
}
// [IoGetNumOfDevices_TC_06_001_002]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoGetNumOfDevices_TC_06_001_003 IOApiTest.IoGetNumOfDevices_TC_06_001_003
 * @subsection IoGetNumOfDevices_TC_06_001_003_tsi Test Suite ID
 * IOApiTest
 * @subsection IoGetNumOfDevices_TC_06_001_003_tci Test Case ID
 * IoGetNumOfDevices_S0_A_TC_06_001_003
 * @subsection IoGetNumOfDevices_TC_06_001_003_rr Related Requirement(s)
 * \#199208
 * @subsection IoGetNumOfDevices_TC_06_001_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetNumOfDevices_TC_06_001_003_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetNumOfDevicesNormal function in normal condition:
 * - Test06_001_003: get number of device with device type is not supported
 * @subsection IoGetNumOfDevices_TC_06_001_003_tpts Test Procedure/Test Steps
 * -# Get the number of device by using R_OSAL_IoGetNumOfDevices with the arguments as below:
 *      - Device type is UNKNOW_DEVICE_TYPE
 *      - Address of "numOfDevice"
 * @subsection IoGetNumOfDevices_TC_06_001_003_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfDevices return OSAl_RETURN_OK.
 * @subsection IoGetNumOfDevices_TC_06_001_003_testcase Test Case Example
 * @snippet test_io.cpp IoGetNumOfDevices_TC_06_001_003
 */
// [IoGetNumOfDevices_TC_06_001_003]
TEST_F(IOApiTest, IoGetNumOfDevices_TC_06_001_003)
{
    size_t numOfDevice = 0;

// Test06_001_003
// @brief: normal case: deviceType is not supported
    ASSERT_EQ(R_OSAL_IoGetNumOfDevices(UNKNOW_DEVICE_TYPE, &numOfDevice), OSAL_RETURN_OK) << "Test06_001_003 failed";
    ASSERT_EQ(numOfDevice, 0) << "Test06_001_003 failed";
}
// [IoGetNumOfDevices_TC_06_001_003]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoGetNumOfDevices_TC_06_001_004 IOApiTest.IoGetNumOfDevices_TC_06_001_004
 * @subsection IoGetNumOfDevices_TC_06_001_004_tsi Test Suite ID
 * IOApiTest
 * @subsection IoGetNumOfDevices_TC_06_001_004_tci Test Case ID
 * IoGetNumOfDevices_S0_A_TC_06_001_004
 * @subsection IoGetNumOfDevices_TC_06_001_004_rr Related Requirement(s)
 * \#199208
 * @subsection IoGetNumOfDevices_TC_06_001_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetNumOfDevices_TC_06_001_004_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetNumOfDevices function in abnormal condition:
 * - Test06_001_004: p_numOfDevices is NULL
 * @subsection IoGetNumOfDevices_TC_06_001_004_tpts Test Procedure/Test Steps
 * -# Get the number of device by using R_OSAL_IoGetNumOfDevices with the arguments as below:
 *      - Device type is "ims"
 *      - NULL
 * @subsection IoGetNumOfDevices_TC_06_001_004_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfDevices return OSAl_RETURN_PAR.
 * @subsection IoGetNumOfDevices_TC_06_001_004_testcase Test Case Example
 * @snippet test_io.cpp IoGetNumOfDevices_TC_06_001_004
 */
// [IoGetNumOfDevices_TC_06_001_004]
TEST_F(IOApiTest, IoGetNumOfDevices_TC_06_001_004)
{
    size_t* p_numOfDevices = NULL;

// Test06_001_004
// @brief: abnormal case: p_numOfDevices is NULL
    ASSERT_EQ(R_OSAL_IoGetNumOfDevices(TEST_IO_DEVICE_TYPE_1, p_numOfDevices), OSAL_RETURN_PAR) << "Test06_001_004 failed";
}
// [IoGetNumOfDevices_TC_06_001_004]

/**
 * @page IO_TEST IO API Test
 * @section OsalApiStateTest_IoGetNumOfDevices_TC_06_001_005 OsalApiStateTest.IoGetNumOfDevices_TC_06_001_005
 * @subsection IoGetNumOfDevices_TC_06_001_005_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection IoGetNumOfDevices_TC_06_001_005_tci Test Case ID
 * IoGetNumOfDevices_S2_A_TC_06_001_005
 * @subsection IoGetNumOfDevices_TC_06_001_005_rr Related Requirement(s)
 * \#199208
 * @subsection IoGetNumOfDevices_TC_06_001_005_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize has already been called
 * @subsection IoGetNumOfDevices_TC_06_001_005_tcs Test Case Summary
 * This test shall verify the state transition of the R_OSAL_IoGetNumOfDevices function:
 * - Test06_001_005: confirm state transition error with R_OSAL_IoGetNumOfDevices
 * @subsection IoGetNumOfDevices_TC_06_001_005_tpts Test Procedure/Test Steps
 * -# Confirm the state transition of R_OSAL_IoGetNumOfDevices
 * @subsection IoGetNumOfDevices_TC_06_001_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfDevices return OSAL_RETURN_STATE
 * @subsection IoGetNumOfDevices_TC_06_001_005_testcase Test Case Example
 * @snippet test_io.cpp IoGetNumOfDevices_TC_06_001_005
 */
// [IoGetNumOfDevices_TC_06_001_005]
TEST_F(OsalApiStateTest, IoGetNumOfDevices_TC_06_001_005)
{
   size_t numOfDevice;

// Test06_001_005
// @brief: abnormal case: state transition error when get number of devices
    EXPECT_EQ(R_OSAL_IoGetNumOfDevices(TEST_IO_DEVICE_TYPE_1, &numOfDevice), OSAL_RETURN_STATE) << "Test06_001_005 failed";
}
// [IoGetNumOfDevices_TC_06_001_005]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoGetNumOfDevices_TC_06_001_006 IOApiTest.IoGetNumOfDevices_TC_06_001_006
 * @subsection IoGetNumOfDevices_TC_06_001_006_tsi Test Suite ID
 * IOApiTest
 * @subsection IoGetNumOfDevices_TC_06_001_006_tci Test Case ID
 * IoGetNumOfDevices_S0_BA_TC_06_001_006
 * @subsection IoGetNumOfDevices_TC_06_001_006_rr Related Requirement(s)
 * \#199208
 * @subsection IoGetNumOfDevices_TC_06_001_006_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetNumOfDevices_TC_06_001_006_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetNumOfDevices function in abnormal condition:
 * - Test06_001_006: get number of device with length of testing device type is more than OSAL_DEVICE_TYPE_NAME_MAX_LENGTH.
 * @subsection IoGetNumOfDevices_TC_06_001_006_tpts Test Procedure/Test Steps
 * -# Get the number of device by using R_OSAL_IoGetNumOfDevices with the arguments as below:
 *      - Length of testing device type is more than OSAL_DEVICE_TYPE_NAME_MAX_LENGTH.
 *      - Address of "numOfDevice"
 * @subsection IoGetNumOfDevices_TC_06_001_006_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfDevices return OSAL_RETURN_PAR.
 * @subsection IoGetNumOfDevices_TC_06_001_006_testcase Test Case Example
 * @snippet test_io.cpp IoGetNumOfDevices_TC_06_001_006
 */
// [IoGetNumOfDevices_TC_06_001_006]
TEST_F(IOApiTest, IoGetNumOfDevices_TC_06_001_006)
{
    size_t numOfDevice = 0;
    char p_deviceType[OSAL_DEVICE_TYPE_NAME_LENGTH_OUT_RANGE + 1];
    int i;

// Test06_001_006
// @brief: abnormal case: deviceType is more than OSAL_DEVICE_TYPE_NAME_MAX_LENGTH.
    for (i = 0; i < OSAL_DEVICE_TYPE_NAME_LENGTH_OUT_RANGE; i++)
    {
        p_deviceType[i] = 'a';
    }
    p_deviceType[OSAL_DEVICE_TYPE_NAME_LENGTH_OUT_RANGE] = '\0';
    EXPECT_EQ(R_OSAL_IoGetNumOfDevices(p_deviceType, &numOfDevice), OSAL_RETURN_PAR) << "Test06_001_006 failed";
    EXPECT_EQ(numOfDevice, 0U);
}
// [IoGetNumOfDevices_TC_06_001_006]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoGetNumOfDevices_TC_06_001_007 IOApiTest.IoGetNumOfDevices_TC_06_001_007
 * @subsection IoGetNumOfDevices_TC_06_001_007_tsi Test Suite ID
 * IOApiTest
 * @subsection IoGetNumOfDevices_TC_06_001_007_tci Test Case ID
 * IoGetNumOfDevices_S0_BN_TC_06_001_007
 * @subsection IoGetNumOfDevices_TC_06_001_007_rr Related Requirement(s)
 * \#199208
 * @subsection IoGetNumOfDevices_TC_06_001_007_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetNumOfDevices_TC_06_001_007_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetNumOfDevices function in abnormal condition:
 * - Test06_001_007: get number of device with length of testing device type is equal to OSAL_DEVICE_TYPE_NAME_MAX_LENGTH.
 * @subsection IoGetNumOfDevices_TC_06_001_007_tpts Test Procedure/Test Steps
 * -# Get the number of device by using R_OSAL_IoGetNumOfDevices with the arguments as below:
 *      - Length of testing device type is equal to OSAL_DEVICE_TYPE_NAME_MAX_LENGTH.
 *      - Address of "numOfDevice"
 * @subsection IoGetNumOfDevices_TC_06_001_007_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetNumOfDevices return OSAL_RETURN_PAR.
 * @subsection IoGetNumOfDevices_TC_06_001_007_testcase Test Case Example
 * @snippet test_io.cpp IoGetNumOfDevices_TC_06_001_007
 */
// [IoGetNumOfDevices_TC_06_001_007]
TEST_F(IOApiTest, IoGetNumOfDevices_TC_06_001_007)
{
    size_t numOfDevice = 10;
    char p_deviceType[OSAL_DEVICE_TYPE_NAME_MAX_LENGTH + 1];
    int i;

// Test06_001_007
// @brief: boundary normal case: Length of device type is OSAL_DEVICE_TYPE_NAME_MAX_LENGTH.
    for (i = 0; i < OSAL_DEVICE_TYPE_NAME_MAX_LENGTH; i++)
    {
        p_deviceType[i] = 'a';
    }
    p_deviceType[OSAL_DEVICE_TYPE_NAME_MAX_LENGTH] = '\0';
    EXPECT_EQ(R_OSAL_IoGetNumOfDevices(p_deviceType, &numOfDevice), OSAL_RETURN_OK) << "Test06_001_007 failed";
    EXPECT_EQ(numOfDevice, 0U);
}
// [IoGetNumOfDevices_TC_06_001_007]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoGetDeviceList_TC_06_002_001 IOApiTest.IoGetDeviceList_TC_06_002_001
 * @subsection IoGetDeviceList_TC_06_002_001_tsi Test Suite ID
 * IOApiTest
 * @subsection IoGetDeviceList_TC_06_002_001_tci Test Case ID
 * IoGetDeviceList_S1_N_TC_06_002_001
 * @subsection IoGetDeviceList_TC_06_002_001_rr Related Requirement(s)
 * \#199197, \#199211, \#276768
 * @subsection IoGetDeviceList_TC_06_002_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - "devices" contains device type, number of device and device list buffer of available devices which will be used for testing.
 * @subsection IoGetDeviceList_TC_06_002_001_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetDeviceList function in normal condition:
 * - Test06_002_001: get device list corresponding with some device type.
 * @subsection IoGetDeviceList_TC_06_002_001_tpts Test Procedure/Test Steps
 * -# Request the required buffer size for device list of each "device_type" in "devices" by using R_OSAL_IoGetDeviceList
 * -# Allocate memory with required buffer size
 * -# Get the device list of each "device_type" in "devices" by using R_OSAL_IoGetDeviceList.
 * -# Free allocated memory
 * @subsection IoGetDeviceList_TC_06_002_001_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetDeviceList return OSAL_RETURN_OK_RQST.<>
 * Value of "size" is equal to length of the "buffer" in "devices" plus 1.
 * -# Memory allocation succeeded (Pointer to buffer != Nullpointer)
 * -# R_OSAL_IoGetDeviceList return OSAL_RETURN_OK.<br>
 * Value of "byte_written" is equal to "size".<br>
 * Content of "buffer" is equal to "buffer" in "devices"
 * -# Memory freeing succeeded
 * @subsection IoGetDeviceList_TC_06_002_001_testcase Test Case Example
 * @snippet test_io.cpp IoGetDeviceList_TC_06_002_001
 */
// [IoGetDeviceList_TC_06_002_001]
TEST_F(IOApiTest, IoGetDeviceList_TC_06_002_001)
{
    size_t size = 0;
    size_t byte_written = 0;
    char *buffer;
    char* p_buffer = NULL;

// Test06_002_001
// @brief: normal case: get device list corresponding with some device type.
    for (st_osal_test_info_t dev : IOApiTest::devices)
    {
        EXPECT_EQ(R_OSAL_IoGetDeviceList(dev.device_type, p_buffer, size, &size), OSAL_RETURN_OK_RQST) << "Test06_002_001 failed";
        EXPECT_EQ(size, strlen(dev.buffer) + 1) << "Test06_002_001 failed. Device type: " << dev.device_type;
        buffer = (char*)malloc(size);
        EXPECT_NE(buffer, NULL_PTR);
        EXPECT_EQ(R_OSAL_IoGetDeviceList(dev.device_type, buffer, size, &byte_written), OSAL_RETURN_OK) << "Test06_002_001 failed";
        EXPECT_EQ(size, byte_written) << "Test06_002_001 failed";

        vector<string> dev_list;
        char* tmp_buffer = (char*)malloc(strlen(dev.buffer) + 1);
        strcpy(tmp_buffer, dev.buffer); // strtok modifies the buffer so need a tmp buffer here.
        char* token = strtok(tmp_buffer, "\n");
        while (token != NULL)
        {
            dev_list.push_back(string(token));
            token = strtok(NULL, "\n");
        }
        free(tmp_buffer);

        vector<string> buffer_list;
        token = strtok(buffer, "\n");
        while (token != NULL)
        {
            buffer_list.push_back(string(token));
            token = strtok(NULL, "\n");
        }

        for (string dev_id : dev_list)
        {
            bool find_result = false;
            for(auto itr = buffer_list.begin(); itr != buffer_list.end(); itr++)
            {
                if(!strcmp(dev_id.c_str(), (*itr).c_str()))
                {
                    find_result = true;
                    break;
                }
            }
            EXPECT_TRUE(find_result) << "Test06_002_001 failed. Device type: " << dev.device_type << " Device ID: " << dev_id.c_str() << "\n";
        }

        free(buffer);
        size = 0;
    }
}
// [IoGetDeviceList_TC_06_002_001]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoGetDeviceList_TC_06_002_002 IOApiTest.IoGetDeviceList_TC_06_002_002
 * @subsection IoGetDeviceList_TC_06_002_002_tsi Test Suite ID
 * IOApiTest
 * @subsection IoGetDeviceList_TC_06_002_002_tci Test Case ID
 * IoGetDeviceList_S0_A_TC_06_002_002
 * @subsection IoGetDeviceList_TC_06_002_002_rr Related Requirement(s)
 * \#276768
 * @subsection IoGetDeviceList_TC_06_002_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetDeviceList_TC_06_002_002_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetDeviceList function in abnormal condition:
 * - Test06_002_002: get device list with device type is NULL
 * @subsection IoGetDeviceList_TC_06_002_002_tpts Test Procedure/Test Steps
 * -# Allocate memory with size is 100 byte
 * -# Get the device list by using R_OSAL_IoGetDeviceList with the arguments as below:
 *      - Device type is NULL
 *      - "buffer" point to memory buffer which has already been allocated.
 *      - size of "buffer"
 *      - Address of "byte_written"
 * -# Free allocated memory
 * @subsection IoGetDeviceList_TC_06_002_002_ereb Expected Result/Behavior
 * -# Memory allocation succeeded (Pointer to buffer != Nullpointer)
 * -# R_OSAL_IoGetDeviceList return OSAL_RETURN_PAR.<br>
 * Value of "byte_written" is equal to 0.
 * -# Allocated memory free successfully
 * @subsection IoGetDeviceList_TC_06_002_002_testcase Test Case Example
 * @snippet test_io.cpp IoGetDeviceList_TC_06_002_002
 */
// [IoGetDeviceList_TC_06_002_002]
TEST_F(IOApiTest, IoGetDeviceList_TC_06_002_002)
{
    size_t byte_written = 0;
    char *buffer;
    char* p_deviceType = NULL;

// Test06_002_002
// @brief: abnormal case: deviceType is NULL
    buffer = (char*)malloc(TEST_DEVICE_LIST_BUFFER_SIZE);
    EXPECT_NE(buffer, NULL_PTR);
    EXPECT_EQ(R_OSAL_IoGetDeviceList(p_deviceType, buffer, sizeof(buffer), &byte_written), OSAL_RETURN_PAR) << "Test06_002_002 failed";
    EXPECT_EQ(byte_written, 0) << "Test06_002_002 failed";
    free(buffer);
}
// [IoGetDeviceList_TC_06_002_002]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoGetDeviceList_TC_06_002_003 IOApiTest.IoGetDeviceList_TC_06_002_003
 * @subsection IoGetDeviceList_TC_06_002_003_tsi Test Suite ID
 * IOApiTest
 * @subsection IoGetDeviceList_TC_06_002_003_tci Test Case ID
 * IoGetDeviceList_S0_A_TC_06_002_003
 * @subsection IoGetDeviceList_TC_06_002_003_rr Related Requirement(s)
 * \#276768
 * @subsection IoGetDeviceList_TC_06_002_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetDeviceList_TC_06_002_003_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetDeviceList function in abnormal condition:
 * - Test06_002_003: get device list with device type is not supported
 * @subsection IoGetDeviceList_TC_06_002_003_tpts Test Procedure/Test Steps
 * -# Allocate memory with size is 100 byte
 * -# Get the device list by using R_OSAL_IoGetDeviceList with the arguments the device type is not supported.
 * -# Free allocated memory
 * @subsection IoGetDeviceList_TC_06_002_003_ereb Expected Result/Behavior
 * -# Memory allocation succeeded (Pointer to buffer != Nullpointer)
 * -# R_OSAL_IoGetDeviceList return OSAL_RETURN_UNKNOWN_NAME.<br>
 * Value of "byte_written" is equal to 0.
 * -# Allocated memory free successfully
 * @subsection IoGetDeviceList_TC_06_002_003_testcase Test Case Example
 * @snippet test_io.cpp IoGetDeviceList_TC_06_002_003
 */
// [IoGetDeviceList_TC_06_002_003]
TEST_F(IOApiTest, IoGetDeviceList_TC_06_002_003)
{
    size_t byte_written = 0;
    char *buffer;

// Test06_002_003
// @brief: abnormal case: deviceType is not supported
    buffer = (char*)malloc(TEST_DEVICE_LIST_BUFFER_SIZE);
    EXPECT_NE(buffer, NULL_PTR);
    EXPECT_EQ(R_OSAL_IoGetDeviceList(UNKNOW_DEVICE_TYPE, buffer, sizeof(buffer), &byte_written), OSAL_RETURN_UNKNOWN_NAME) << "Test06_002_003 failed";
    EXPECT_EQ(byte_written, 0) << "Test06_002_003 failed";
    free(buffer);
}
// [IoGetDeviceList_TC_06_002_003]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoGetDeviceList_TC_06_002_004 IOApiTest.IoGetDeviceList_TC_06_002_004
 * @subsection IoGetDeviceList_TC_06_002_004_tsi Test Suite ID
 * IOApiTest
 * @subsection IoGetDeviceList_TC_06_002_004_tci Test Case ID
 * IoGetDeviceList_S0_A_TC_06_002_004
 * @subsection IoGetDeviceList_TC_06_002_004_rr Related Requirement(s)
 * \#276768
 * @subsection IoGetDeviceList_TC_06_002_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetDeviceList_TC_06_002_004_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetDeviceList function in abnormal condition:
 * - Test06_002_004: get device list with p_buffer is NULL, buffer_size is not 0.
 * @subsection IoGetDeviceList_TC_06_002_004_tpts Test Procedure/Test Steps
 * -# Allocate memory with size is 100 byte
 * -# Get the device list by using R_OSAL_IoGetDeviceList with the arguments as below:
 *      - Device type is "ims"
 *      - NULL
 *      - size of "buffer"
 *      - Address of "byte_written"
 * -# Free allocated memory
 * @subsection IoGetDeviceList_TC_06_002_004_ereb Expected Result/Behavior
 * -# Memory allocation succeeded (Pointer to buffer != Nullpointer)
 * -# R_OSAL_IoGetDeviceList return OSAL_RETURN_PAR.<br>
 * Value of "byte_written" is equal to 0.
 * -# Allocated memory free successfully
 * @subsection IoGetDeviceList_TC_06_002_004_testcase Test Case Example
 * @snippet test_io.cpp IoGetDeviceList_TC_06_002_004
 */
// [IoGetDeviceList_TC_06_002_004]
TEST_F(IOApiTest, IoGetDeviceList_TC_06_002_004)
{
    size_t byte_written = 0;
    char *buffer;
    char* p_buffer = NULL;

// Test06_002_004
// @brief: abnormal case: p_buffer is NULL, buffer_size is not 0
    buffer = (char*)malloc(TEST_DEVICE_LIST_BUFFER_SIZE);
    EXPECT_NE(buffer, NULL_PTR);
    EXPECT_EQ(R_OSAL_IoGetDeviceList(TEST_IO_DEVICE_TYPE_1, p_buffer, sizeof(buffer), &byte_written), OSAL_RETURN_PAR) << "Test06_002_004 failed";
    EXPECT_EQ(byte_written, 0) << "Test06_002_004 failed";
    free(buffer);
}
// [IoGetDeviceList_TC_06_002_004]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoGetDeviceList_TC_06_002_005 IOApiTest.IoGetDeviceList_TC_06_002_005
 * @subsection IoGetDeviceList_TC_06_002_005_tsi Test Suite ID
 * IOApiTest
 * @subsection IoGetDeviceList_TC_06_002_005_tci Test Case ID
 * IoGetDeviceList_S0_A_TC_06_002_005
 * @subsection IoGetDeviceList_TC_06_002_005_rr Related Requirement(s)
 * \#276768
 * @subsection IoGetDeviceList_TC_06_002_005l_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - "devices" contains device type, number of device and device list buffer of available devices which will be used for testing.
 * @subsection IoGetDeviceList_TC_06_002_005_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetDeviceList function in normal condition:
 * - Test06_002_005: get device list with p_buffer is NULL, buffer_size is 0
 * @subsection IoGetDeviceList_TC_06_002_005_tpts Test Procedure/Test Steps
 * -# Get the device list by using R_OSAL_IoGetDeviceList with the arguments as below:
 *      - Device type is "ims"
 *      - NULL
 *      - Buffer size is 0
 *      - Address of "byte_written"
 * @subsection IoGetDeviceList_TC_06_002_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetDeviceList return OSAL_RETURN_OK_RQST.<br>
 * Value of "byte_written" is equal to "buffer" length of first element ("ims") in "devices" plus 1.
 * @subsection IoGetDeviceList_TC_06_002_005_testcase Test Case Example
 * @snippet test_io.cpp IoGetDeviceList_TC_06_002_005
 */
// [IoGetDeviceList_TC_06_002_005]
TEST_F(IOApiTest, IoGetDeviceList_TC_06_002_005)
{
    size_t size = 0;
    size_t byte_written = 0;
    char* p_buffer = NULL;

// Test06_002_005
// @brief: normal case: p_buffer is NULL, buffer_size is 0
    EXPECT_EQ(R_OSAL_IoGetDeviceList(TEST_IO_DEVICE_TYPE_1, p_buffer, size, &byte_written), OSAL_RETURN_OK_RQST) << "Test06_002_005 failed";
    EXPECT_EQ(byte_written, TEST_IO_DEVICE_LIST_SIZE) << "Test06_002_005 failed";
}
// [IoGetDeviceList_TC_06_002_005]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoGetDeviceList_TC_06_002_006 IOApiTest.IoGetDeviceList_TC_06_002_006
 * @subsection IoGetDeviceList_TC_06_002_006_tsi Test Suite ID
 * IOApiTest
 * @subsection IoGetDeviceList_TC_06_002_006_tci Test Case ID
 * IoGetDeviceList_S0_A_TC_06_002_006
 * @subsection IoGetDeviceList_TC_06_002_006_rr Related Requirement(s)
 * \#276768
 * @subsection IoGetDeviceList_TC_06_002_006_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetDeviceList_TC_06_002_006_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetDeviceList function in abnormal condition:
 * - Test06_002_006: get device list with p_buffer is not NULL, buffer_size is 0
 * @subsection IoGetDeviceList_TC_06_002_006_tpts Test Procedure/Test Steps
 * -# Allocate memory with size is 100 byte
 * -# Get the device list by using R_OSAL_IoGetDeviceList with the arguments as below:
 *      - Device type is "ims"
 *      - "buffer" point to memory buffer which has already been allocated.
 *      - Buffer size is 0
 *      - Address of "byte_written"
 * -# Free allocated memory pointed by "buffer"
 * @subsection IoGetDeviceList_TC_06_002_006_ereb Expected Result/Behavior
 * -# Memory allocation succeeded (Pointer to buffer != Nullpointer)
 * -# R_OSAL_IoGetDeviceList return OSAL_RETURN_PAR.<br>
 * Value of "byte_written" is equal to 0.
 * -# Memory freeing succeeded
 * @subsection IoGetDeviceList_TC_06_002_006_testcase Test Case Example
 * @snippet test_io.cpp IoGetDeviceList_TC_06_002_006
 */
// [IoGetDeviceList_TC_06_002_006]
TEST_F(IOApiTest, IoGetDeviceList_TC_06_002_006)
{
    size_t byte_written = 0;
    char *buffer;

// Test06_002_006
// @brief: abnormal case: p_buffer is not NULL, buffer_size is 0
    buffer = (char*)malloc(TEST_DEVICE_LIST_BUFFER_SIZE);
    EXPECT_NE(buffer, NULL_PTR);
    EXPECT_EQ(R_OSAL_IoGetDeviceList(TEST_IO_DEVICE_TYPE_1, buffer, byte_written, &byte_written), OSAL_RETURN_PAR) << "Test06_002_006 failed";
    EXPECT_EQ(byte_written, 0) << "Test06_002_006 failed";
    free(buffer);
}
// [IoGetDeviceList_TC_06_002_006]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoGetDeviceList_TC_06_002_007 IOApiTest.IoGetDeviceList_TC_06_002_007
 * @subsection IoGetDeviceList_TC_06_002_007_tsi Test Suite ID
 * IOApiTest
 * @subsection IoGetDeviceList_TC_06_002_007_tci Test Case ID
 * IoGetDeviceList_S0_A_TC_06_002_007
 * @subsection IoGetDeviceList_TC_06_002_007_rr Related Requirement(s)
 * \#276768
 * @subsection IoGetDeviceList_TC_06_002_007_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetDeviceList_TC_06_002_007_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetDeviceList function in abnormal condition:
 * - Test06_002_007: get device list with buffer_size is less than the required size.
 * @subsection IoGetDeviceList_TC_06_002_007_tpts Test Procedure/Test Steps
 * -# Request the required buffer size for device list of "ims" by using R_OSAL_IoGetDeviceList
 * -# Allocate memory with size is the required buffer size
 * -# Get the device list by using R_OSAL_IoGetDeviceList with the arguments as below:
 *      - Device type is "ims"
 *      - "buffer" point to memory buffer which has already been allocated.
 *      - Value of "size" minus 1
 *      - Address of "byte_written"
 * -# Free allocated memory pointed by "buffer"
 * @subsection IoGetDeviceList_TC_06_002_007_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetDeviceList return OSAL_RETURN_OK_RQST.<br>
 * Value of "size" is not equal to 0.
 * -# Memory allocation succeeded (Pointer to buffer != Nullpointer)
 * -# R_OSAL_IoGetDeviceList return OSAL_RETURN_PAR.<br>
 * Value of "byte_written" is equal to 0.
 * -# Memory freeing succeeded
 * @subsection IoGetDeviceList_TC_06_002_007_testcase Test Case Example
 * @snippet test_io.cpp IoGetDeviceList_TC_06_002_007
 */
// [IoGetDeviceList_TC_06_002_007]
TEST_F(IOApiTest, IoGetDeviceList_TC_06_002_007)
{
    size_t size = 0;
    size_t byte_written = 0;
    char *buffer;
    char* p_buffer = NULL;

// Test06_002_007
// @brief: abnormal case: buffer_size is less than the needed size
    EXPECT_EQ(R_OSAL_IoGetDeviceList(TEST_IO_DEVICE_TYPE_1, p_buffer, size, &size), OSAL_RETURN_OK_RQST) << "Test06_002_007 failed";
    EXPECT_NE(size, 0) << "Test06_002_007 failed";
    buffer = (char*)malloc(size);
    EXPECT_NE(buffer, NULL_PTR);
    EXPECT_EQ(R_OSAL_IoGetDeviceList(TEST_IO_DEVICE_TYPE_1, buffer, size - 1, &byte_written), OSAL_RETURN_PAR) << "Test06_002_007 failed";
    EXPECT_EQ(byte_written, 0) << "Test06_002_007 failed";
    free(buffer);
}
// [IoGetDeviceList_TC_06_002_007]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoGetDeviceList_TC_06_002_008 IOApiTest.IoGetDeviceList_TC_06_002_008
 * @subsection IoGetDeviceList_TC_06_002_008_tsi Test Suite ID
 * IOApiTest
 * @subsection IoGetDeviceList_TC_06_002_008_tci Test Case ID
 * IoGetDeviceList_S1_A_TC_06_002_008
 * @subsection IoGetDeviceList_TC_06_002_008_rr Related Requirement(s)
 * \#199211, \#276768
 * @subsection IoGetDeviceList_TC_06_002_008_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetDeviceList_TC_06_002_008_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetDeviceList function in normal condition:
 * - Test06_002_008: get device list with buffer_size is bigger than the required size
 * @subsection IoGetDeviceList_TC_06_002_008_tpts Test Procedure/Test Steps
 * -# Request the required buffer size for device list of "ims" by using R_OSAL_IoGetDeviceList
 * -# Allocate memory with required buffer size plus 1
 * -# Get the device list by using R_OSAL_IoGetDeviceList with the arguments as below:
 *      - Device type is "ims"
 *      - "buffer" point to memory buffer which has already been allocated.
 *      - Value of "size" plus 1
 *      - Address of "byte_written"
 * -# Free allocated memory
 * @subsection IoGetDeviceList_TC_06_002_008_ereb Expected Result/Behavior
 * -# Memory allocation succeeded (Pointer to buffer != Nullpointer)
 * -# R_OSAL_IoGetDeviceList return OSAL_RETURN_OK_RQST.<br>
 * Value of "size" is not equal to 0.
 * -# R_OSAL_IoGetDeviceList return OSAL_RETURN_OK.<br>
 * Value of "byte_written" is not equal to 0.
 * -# Memory freeing succeeded
 * @subsection IoGetDeviceList_TC_06_002_008_testcase Test Case Example
 * @snippet test_io.cpp IoGetDeviceList_TC_06_002_008
 */
// [IoGetDeviceList_TC_06_002_008]
TEST_F(IOApiTest, IoGetDeviceList_TC_06_002_008)
{
    size_t size = 0;
    size_t byte_written = 0;
    char *buffer;
    char* p_buffer = NULL;

// Test06_002_008
// @brief: normal case: buffer_size is bigger than the needed size
    EXPECT_EQ(R_OSAL_IoGetDeviceList(TEST_IO_DEVICE_TYPE_1, p_buffer, size, &size), OSAL_RETURN_OK_RQST) << "Test06_002_008 failed";
    EXPECT_NE(size, 0) << "Test06_002_008 failed";
    buffer = (char*)malloc(size + 1);
    EXPECT_EQ(R_OSAL_IoGetDeviceList(TEST_IO_DEVICE_TYPE_1, buffer, size + 1, &byte_written), OSAL_RETURN_OK) << "Test06_002_008 failed";
    EXPECT_NE(byte_written, 0) << "Test06_002_008 failed";
    free(buffer);
}
// [IoGetDeviceList_TC_06_002_008]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoGetDeviceList_TC_06_002_009 IOApiTest.IoGetDeviceList_TC_06_002_009
 * @subsection IoGetDeviceList_TC_06_002_009_tsi Test Suite ID
 * IOApiTest
 * @subsection IoGetDeviceList_TC_06_002_009_tci Test Case ID
 * IoGetDeviceList_S0_A_TC_06_002_009
 * @subsection IoGetDeviceList_TC_06_002_009_rr Related Requirement(s)
 * \#276768
 * @subsection IoGetDeviceList_TC_06_002_009_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetDeviceList_TC_06_002_009_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetDeviceList function in abnormal condition:
 * - Test06_002_009: get device list with p_num_of_byte is NULL
 * @subsection IoGetDeviceList_TC_06_002_009_tpts Test Procedure/Test Steps
 * -# Allocate memory with size is 100 byte
 * -# Get the device list by using R_OSAL_IoGetDeviceList with the arguments as below:
 *      - Device type is "ims"
 *      - "buffer" point to memory buffer which has already been allocated.
 *      - Value of "size"
 *      - NULL
 * -# Free allocated memory pointed by "buffer"
 * @subsection IoGetDeviceList_TC_06_002_009_ereb Expected Result/Behavior
 * -# Memory allocation succeeded (Pointer to buffer != Nullpointer)
 * -# R_OSAL_IoGetDeviceList return OSAL_RETURN_PAR.
 * -# Memory freeing succeeded
 * @subsection IoGetDeviceList_TC_06_002_009_testcase Test Case Example
 * @snippet test_io.cpp IoGetDeviceList_TC_06_002_009
 */
// [IoGetDeviceList_TC_06_002_009]
TEST_F(IOApiTest, IoGetDeviceList_TC_06_002_009)
{
    char *buffer;
    size_t* p_num_of_byte = NULL;

// Test06_002_009
// @brief: abnormal case: p_num_of_byte is NULL
    buffer = (char*)malloc(TEST_DEVICE_LIST_BUFFER_SIZE);
    EXPECT_NE(buffer, NULL_PTR);
    EXPECT_EQ(R_OSAL_IoGetDeviceList(TEST_IO_DEVICE_TYPE_1, buffer, sizeof(buffer), p_num_of_byte), OSAL_RETURN_PAR) << "Test06_002_009 failed";
    free(buffer);
}
// [IoGetDeviceList_TC_06_002_009]

/**
 * @page IO_TEST IO API Test
 * @section OsalApiStateTest_IoGetDeviceList_TC_06_002_010 OsalApiStateTest.IoGetDeviceList_TC_06_002_010
 * @subsection IoGetDeviceList_TC_06_002_010_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection IoGetDeviceList_TC_06_002_010_tci Test Case ID
 * IoGetDeviceList_S2_A_TC_06_002_010
 * @subsection IoGetDeviceList_TC_06_002_010_rr Related Requirement(s)
 * \#276768
 * @subsection IoGetDeviceList_TC_06_002_010_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize has already been called
 * @subsection IoGetDeviceList_TC_06_002_010_tcs Test Case Summary
 * This test shall verify the state transition of the R_OSAL_IoGetDeviceList function:
 * - Test06_002_010: confirm state transition error with R_OSAL_IoGetDeviceList
 * @subsection IoGetDeviceList_TC_06_002_010_tpts Test Procedure/Test Steps
 * -# Confirm the state transition of R_OSAL_IoGetDeviceList
 * @subsection IoGetDeviceList_TC_06_002_010_ereb Expected Result/Behavior
 * -# R_OSAL_IoGetDeviceList return OSAL_RETURN_STATE.<br>
 * Value of "byte_written" is equal to 0.
 * @subsection IoGetDeviceList_TC_06_002_010_testcase Test Case Example
 * @snippet test_io.cpp IoGetDeviceList_TC_06_002_010
 */
// [IoGetDeviceList_TC_06_002_010]
TEST_F(OsalApiStateTest, IoGetDeviceList_TC_06_002_010)
{
    size_t byte_written;
    char *buffer = NULL;

// Test06_002_010
// @brief: abnormal case: state transition error when get device list
    byte_written = 0;
    EXPECT_EQ(R_OSAL_IoGetDeviceList(TEST_IO_DEVICE_TYPE_1, buffer, byte_written, &byte_written), OSAL_RETURN_STATE) << "Test06_002_010 failed";
    EXPECT_EQ(byte_written, 0) << "Test06_002_010 failed";
}
// [IoGetDeviceList_TC_06_002_010]


/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoGetDeviceList_TC_06_002_011 IOApiTest.IoGetDeviceList_TC_06_002_011
 * @subsection IoGetDeviceList_TC_06_002_011_tsi Test Suite ID
 * IOApiTest
 * @subsection IoGetDeviceList_TC_06_002_011_tci Test Case ID
 * IoGetDeviceList_S0_BA_TC_06_002_011
 * @subsection IoGetDeviceList_TC_06_002_011_rr Related Requirement(s)
 * \#276768
 * @subsection IoGetDeviceList_TC_06_002_011_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetDeviceList_TC_06_002_011_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetDeviceList function in abnormal condition:
 * - Test06_002_011: get device list with length of testing device type is more than OSAL_DEVICE_TYPE_NAME_MAX_LENGTH
 * @subsection IoGetDeviceList_TC_06_002_011_tpts Test Procedure/Test Steps
 * -# Allocate memory with size is TEST_DEVICE_LIST_BUFFER_SIZE byte
 * -# Get the device list by using R_OSAL_IoGetDeviceList with the arguments as below:
 *      - Length of testing device type is more than OSAL_DEVICE_TYPE_NAME_MAX_LENGTH
 *      - "buffer" point to memory buffer which has already been allocated.
 *      - size of "buffer"
 *      - Address of "byte_written"
 * -# Free allocated memory
 * @subsection IoGetDeviceList_TC_06_002_011_ereb Expected Result/Behavior
 * -# Memory allocation succeeded (Pointer to buffer != Nullpointer)
 * -# R_OSAL_IoGetDeviceList return OSAL_RETURN_PAR.<br>
 * Value of "byte_written" is equal to 0.
 * -# Allocated memory free successfully
 * @subsection IoGetDeviceList_TC_06_002_011_testcase Test Case Example
 * @snippet test_io.cpp IoGetDeviceList_TC_06_002_011
 */
// [IoGetDeviceList_TC_06_002_011]
TEST_F(IOApiTest, IoGetDeviceList_TC_06_002_011)
{
    size_t byte_written = 0;
    char *buffer;
    char p_deviceType[OSAL_DEVICE_TYPE_NAME_LENGTH_OUT_RANGE + 1];
    int i;

// Test06_002_011
// @brief: abnormal case: device_type_name is more than OSAL_DEVICE_TYPE_NAME_MAX_LENGTH.
    for (i = 0; i < OSAL_DEVICE_TYPE_NAME_LENGTH_OUT_RANGE; i++)
    {
        p_deviceType[i] = 'a';
    }
    p_deviceType[OSAL_DEVICE_TYPE_NAME_LENGTH_OUT_RANGE] = '\0';
    buffer = (char*)malloc(TEST_DEVICE_LIST_BUFFER_SIZE);
    EXPECT_NE(buffer, (char*)NULL);
    EXPECT_EQ(R_OSAL_IoGetDeviceList(p_deviceType, buffer, sizeof(buffer), &byte_written), OSAL_RETURN_PAR) << "Test06_002_011 failed";
    EXPECT_EQ(byte_written, 0U) << "Test06_002_011 failed";
    free(buffer);
}
// [IoGetDeviceList_TC_06_002_011]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoGetDeviceList_TC_06_002_012 IOApiTest.IoGetDeviceList_TC_06_002_012
 * @subsection IoGetDeviceList_TC_06_002_012_tsi Test Suite ID
 * IOApiTest
 * @subsection IoGetDeviceList_TC_06_002_012_tci Test Case ID
 * IoGetDeviceList_S0_BN_TC_06_002_012
 * @subsection IoGetDeviceList_TC_06_002_012_rr Related Requirement(s)
 * \#276768
 * @subsection IoGetDeviceList_TC_06_002_012_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoGetDeviceList_TC_06_002_012_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoGetDeviceList function in abnormal condition:
 * - Test06_002_012: get device list with length of testing device type is equal to OSAL_DEVICE_TYPE_NAME_MAX_LENGTH
 * @subsection IoGetDeviceList_TC_06_002_012_tpts Test Procedure/Test Steps
 * -# Allocate memory with size is TEST_DEVICE_LIST_BUFFER_SIZE byte
 * -# Get the device list by using R_OSAL_IoGetDeviceList with the arguments as below:
 *      - Length of testing device type is equal to OSAL_DEVICE_TYPE_NAME_MAX_LENGTH
 *      - "buffer" point to memory buffer which has already been allocated.
 *      - size of "buffer"
 *      - Address of "byte_written"
 * -# Free allocated memory
 * @subsection IoGetDeviceList_TC_06_002_012_ereb Expected Result/Behavior
 * -# Memory allocation succeeded (Pointer to buffer != Nullpointer)
 * -# R_OSAL_IoGetDeviceList return OSAL_RETURN_UNKNOWN_NAME.<br>
 * Value of "byte_written" is equal to 0.
 * -# Allocated memory free successfully
 * @subsection IoGetDeviceList_TC_06_002_012_testcase Test Case Example
 * @snippet test_io.cpp IoGetDeviceList_TC_06_002_012
 */
// [IoGetDeviceList_TC_06_002_012]
TEST_F(IOApiTest, IoGetDeviceList_TC_06_002_012)
{
    size_t byte_written = 0;
    char *buffer;
    char p_deviceType[OSAL_DEVICE_TYPE_NAME_MAX_LENGTH + 1];
    int i;

// Test06_002_012
// @brief: boundary normal case: device_type_name is equal to OSAL_DEVICE_TYPE_NAME_MAX_LENGTH.
    for (i = 0; i < OSAL_DEVICE_TYPE_NAME_MAX_LENGTH; i++)
    {
        p_deviceType[i] = 'a';
    }
    p_deviceType[OSAL_DEVICE_TYPE_NAME_MAX_LENGTH] = '\0';
    buffer = (char*)malloc(TEST_DEVICE_LIST_BUFFER_SIZE);
    EXPECT_NE(buffer, (char*)NULL);
    EXPECT_EQ(R_OSAL_IoGetDeviceList(p_deviceType, buffer, sizeof(buffer), &byte_written), OSAL_RETURN_UNKNOWN_NAME) << "Test06_002_012 failed";
    EXPECT_EQ(byte_written, 0U) << "Test06_002_012 failed";
    free(buffer);
}
// [IoGetDeviceList_TC_06_002_012]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoDeviceOpen_TC_06_003_001 IOApiTest.IoDeviceOpen_TC_06_003_001
 * @subsection IoDeviceOpen_TC_06_003_001_tsi Test Suite ID
 * IOApiTest
 * @subsection IoDeviceOpen_TC_06_003_001_tci Test Case ID
 * IoDeviceOpen_S1_N_TC_06_003_001
 * @subsection IoDeviceOpen_TC_06_003_001_rr Related Requirement(s)
 * \#199198, \#199210
 * @subsection IoDeviceOpen_TC_06_003_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - "device_list" contain the device list of each "device_type" in "devices".
 * @subsection IoDeviceOpen_TC_06_003_001_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoDeviceOpen function in normal condition:
 * - Test06_003_001: open all device in "devices_list".
 * @subsection IoDeviceOpen_TC_06_003_001_tpts Test Procedure/Test Steps
 * -# Open all available devices in "device_list" by using R_OSAL_IoDeviceOpen.
 * -# Close all opened device at step 1 for the next test cases by using R_OSAL_IoDeviceClose.
 * @subsection IoDeviceOpen_TC_06_003_001_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoDeviceOpen_TC_06_003_001_testcase Test Case Example
 * @snippet test_io.cpp IoDeviceOpen_TC_06_003_001
 */
// [IoDeviceOpen_TC_06_003_001]
TEST_F(IOApiTest, IoDeviceOpen_TC_06_003_001)
{
// Test06_003_001
// @brief: normal case: open all device type in "devices".
    for (auto it = device_list.begin(); it != device_list.end(); it++)
    {
        EXPECT_EQ(R_OSAL_IoDeviceOpen((*it).c_str(), &device_handle), OSAL_RETURN_OK) << "Test06_003_001 failed";
        EXPECT_NE(device_handle, NULL_PTR) << "Test06_003_001 failed";
        EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "Test06_003_001 failed";
        device_handle = OSAL_DEVICE_HANDLE_INVALID;
    }
}
// [IoDeviceOpen_TC_06_003_001]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoDeviceOpen_TC_06_003_002 IOApiTest.IoDeviceOpen_TC_06_003_002
 * @subsection IoDeviceOpen_TC_06_003_002_tsi Test Suite ID
 * IOApiTest
 * @subsection IoDeviceOpen_TC_06_003_002_tci Test Case ID
 * IoDeviceOpen_S0_A_TC_06_003_002
 * @subsection IoDeviceOpen_TC_06_003_002_rr Related Requirement(s)
 * \#199210
 * @subsection IoDeviceOpen_TC_06_003_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoDeviceOpen_TC_06_003_002_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoDeviceOpen function in abnormal condition:
 * - Test06_003_002: device id is NULL
 * @subsection IoDeviceOpen_TC_06_003_002_tpts Test Procedure/Test Steps
 * -# Initialize the device_handle with DEVICE_HANDLE_INITIALIZED_VALUE by using memset.<br>
 * DEVICE_HANDLE_INITIALIZED_VALUE must be different from OSAL_DEVICE_HANDLE_INVALID
 * -# Open a device by using R_OSAL_IoDeviceOpen with the arguments as below:
 *      - Device ID is NULL
 *      - Address of "device_handle"
 * @subsection IoDeviceOpen_TC_06_003_002_ereb Expected Result/Behavior
 * -# memset a value for device_handle successfully
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_PAR.
 * -# device_handle is OSAL_DEVICE_HANDLE_INVALID.
 * @subsection IoDeviceOpen_TC_06_003_002_testcase Test Case Example
 * @snippet test_io.cpp IoDeviceOpen_TC_06_003_002
 */
// [IoDeviceOpen_TC_06_003_002]
TEST_F(IOApiTest, IoDeviceOpen_TC_06_003_002)
{
    char* dev_id = NULL;

// Test06_003_002
// @brief: abnormal case: device id is NULL
    memset(&device_handle, DEVICE_HANDLE_INITIALIZED_VALUE, sizeof(device_handle));
    EXPECT_EQ(R_OSAL_IoDeviceOpen(dev_id, &device_handle), OSAL_RETURN_PAR) << "Test06_003_002 failed";
    EXPECT_EQ(device_handle, OSAL_DEVICE_HANDLE_INVALID) << "Test06_003_002 failed";
}
// [IoDeviceOpen_TC_06_003_002]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoDeviceOpen_TC_06_003_003 IOApiTest.IoDeviceOpen_TC_06_003_003
 * @subsection IoDeviceOpen_TC_06_003_003_tsi Test Suite ID
 * IOApiTest
 * @subsection IoDeviceOpen_TC_06_003_003_tci Test Case ID
 * IoDeviceOpen_S0_A_TC_06_003_003
 * @subsection IoDeviceOpen_TC_06_003_003_rr Related Requirement(s)
 * \#199210
 * @subsection IoDeviceOpen_TC_06_003_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoDeviceOpen_TC_06_003_003_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoDeviceOpen function in abnormal condition:
 * - Test06_003_003: device id is not supported
 * @subsection IoDeviceOpen_TC_06_003_003_tpts Test Procedure/Test Steps
 * -# Initialize the device_handle with DEVICE_HANDLE_INITIALIZED_VALUE by using memset.<br>
 * DEVICE_HANDLE_INITIALIZED_VALUE must be different from OSAL_DEVICE_HANDLE_INVALID
 * -# Open a device by using R_OSAL_IoDeviceOpen with the arguments as below:
 *      - Device ID is UNKNOW_DEVICE_TYPE
 *      - Address of "device_handle"
 * @subsection IoDeviceOpen_TC_06_003_003_ereb Expected Result/Behavior
 * -# memset a value for device_handle successfully
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_ID.
 * -# device_handle is OSAL_DEVICE_HANDLE_INVALID.
 * @subsection IoDeviceOpen_TC_06_003_003_testcase Test Case Example
 * @snippet test_io.cpp IoDeviceOpen_TC_06_003_003
 */
// [IoDeviceOpen_TC_06_003_003]
TEST_F(IOApiTest, IoDeviceOpen_TC_06_003_003)
{
// Test06_003_003
// @brief: abnormal case: device id is not supported
    memset(&device_handle, DEVICE_HANDLE_INITIALIZED_VALUE, sizeof(device_handle));
    EXPECT_EQ(R_OSAL_IoDeviceOpen(UNKNOW_DEVICE_TYPE, &device_handle), OSAL_RETURN_ID) << "Test06_003_003 failed";
    EXPECT_EQ(device_handle, OSAL_DEVICE_HANDLE_INVALID) << "Test06_003_003 failed";
}
// [IoDeviceOpen_TC_06_003_003]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoDeviceOpen_TC_06_003_004 IOApiTest.IoDeviceOpen_TC_06_003_004
 * @subsection IoDeviceOpen_TC_06_003_004_tsi Test Suite ID
 * IOApiTest
 * @subsection IoDeviceOpen_TC_06_003_004_tci Test Case ID
 * IoDeviceOpen_S0_A_TC_06_003_004
 * @subsection IoDeviceOpen_TC_06_003_004_rr Related Requirement(s)
 * \#199199, \#199210
 * @subsection IoDeviceOpen_TC_06_003_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoDeviceOpen_TC_06_003_004_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoDeviceOpen function in abnormal condition:
 * - Test06_003_004: p_handle is NULL
 * @subsection IoDeviceOpen_TC_06_003_004_tpts Test Procedure/Test Steps
 * -# Open a device by using R_OSAL_IoDeviceOpen with the arguments as below:
 *      - Device ID is test target device
 *      - NULL
 * @subsection IoDeviceOpen_TC_06_003_004_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_PAR.
 * @subsection IoDeviceOpen_TC_06_003_004_testcase Test Case Example
 * @snippet test_io.cpp IoDeviceOpen_TC_06_003_004
 */
// [IoDeviceOpen_TC_06_003_004]
TEST_F(IOApiTest, IoDeviceOpen_TC_06_003_004)
{
    osal_device_handle_t* p_handle = NULL;

// Test06_003_004
// @brief: abnormal case: p_handle is NULL
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, p_handle), OSAL_RETURN_PAR) << "Test06_003_004 failed";
}
// [IoDeviceOpen_TC_06_003_004]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoDeviceOpen_TC_06_003_005 IOApiTest.IoDeviceOpen_TC_06_003_005
 * @subsection IoDeviceOpen_TC_06_003_005_tsi Test Suite ID
 * IOApiTest
 * @subsection IoDeviceOpen_TC_06_003_005_tci Test Case ID
 * IoDeviceOpen_S1_N_TC_06_003_005
 * @subsection IoDeviceOpen_TC_06_003_005_rr Related Requirement(s)
 * \#199210
 * @subsection IoDeviceOpen_TC_06_003_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoDeviceOpen_TC_06_003_005_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoDeviceOpen function in normal condition:
 * - Test06_003_005: re-open after close a device
 * - post-process: close opened device test target device
 * @subsection IoDeviceOpen_TC_06_003_005_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * -# Re-open test target device by using R_OSAL_IoDeviceOpen
 * -# Close test target device for the next test cases by using R_OSAL_IoDeviceClose
 * @subsection IoDeviceOpen_TC_06_003_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 *  "device_handle" is not NULL_PTR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoDeviceOpen_TC_06_003_005_testcase Test Case Example
 * @snippet test_io.cpp IoDeviceOpen_TC_06_003_005
 */
// [IoDeviceOpen_TC_06_003_005]
TEST_F(IOApiTest, IoDeviceOpen_TC_06_003_005)
{
// Test06_003_005
// @brief: normal case: re-open after close
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "Test06_003_005 failed";
    EXPECT_NE(device_handle, NULL_PTR) << "Test06_003_005 failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK);
    ASSERT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "Test06_003_005 failed";
    EXPECT_NE(device_handle, NULL_PTR) << "Test06_003_005 failed";
// post-process: close opened device
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoDeviceOpen_TC_06_003_005]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoDeviceOpen_TC_06_003_006 IOApiTest.IoDeviceOpen_TC_06_003_006
 * @subsection IoDeviceOpen_TC_06_003_006_tsi Test Suite ID
 * IOApiTest
 * @subsection IoDeviceOpen_TC_06_003_006_tci Test Case ID
 * IoDeviceOpen_S2_A_TC_06_003_006
 * @subsection IoDeviceOpen_TC_06_003_006_rr Related Requirement(s)
 * \#199210
 * @subsection IoDeviceOpen_TC_06_003_006_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoDeviceOpen_TC_06_003_006_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoDeviceOpen function in abnormal condition:
 * - Test06_003_006: open the opened device
 * - post-process: close opended device test target device
 * @subsection IoDeviceOpen_TC_06_003_006_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Continue to open test target device by using R_OSAL_IoDeviceOpen
 * -# Close test target device for the next test cases by using R_OSAL_IoDeviceClose
 * @subsection IoDeviceOpen_TC_06_003_006_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 *  "device_handle" is not NULL_PTR.
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_BUSY.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoDeviceOpen_TC_06_003_006_testcase Test Case Example
 * @snippet test_io.cpp IoDeviceOpen_TC_06_003_006
 */
// [IoDeviceOpen_TC_06_003_006]
TEST_F(IOApiTest, IoDeviceOpen_TC_06_003_006)
{
// Test06_003_006
// @brief: abnormal case: open on the opened device
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "Test06_003_006 failed";
    EXPECT_NE(device_handle, NULL_PTR) << "Test06_003_006 failed";
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle1), OSAL_RETURN_BUSY) << "Test06_003_006 failed";
// post-process: close opended device
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
    device_handle1 = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoDeviceOpen_TC_06_003_006]

/**
 * @page IO_TEST IO API Test
 * @section OsalApiStateTest_IoDeviceOpen_TC_06_003_007 OsalApiStateTest.IoDeviceOpen_TC_06_003_007
 * @subsection IoDeviceOpen_TC_06_003_007_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection IoDeviceOpen_TC_06_003_007_tci Test Case ID
 * IoDeviceOpen_S2_A_TC_06_003_007
 * @subsection IoDeviceOpen_TC_06_003_007_rr Related Requirement(s)
 * \#199210
 * @subsection IoDeviceOpen_TC_06_003_007_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize has already been called
 * @subsection IoDeviceOpen_TC_06_003_007_tcs Test Case Summary
 * This test shall verify the state transition of the R_OSAL_IoDeviceOpen function:
 * - Test06_003_007: confirm state transition error with R_OSAL_IoDeviceOpen
 * @subsection IoDeviceOpen_TC_06_003_007_tpts Test Procedure/Test Steps
 * -# Initialize the device_handle with DEVICE_HANDLE_INITIALIZED_VALUE by using memset.<br>
 * DEVICE_HANDLE_INITIALIZED_VALUE must be different from OSAL_DEVICE_HANDLE_INVALID
 * -# Confirm the state transition of R_OSAL_IoDeviceOpen
 * @subsection IoDeviceOpen_TC_06_003_007_ereb Expected Result/Behavior
 * -# memset a value for device_handle successfully
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_STATE
 * @subsection IoDeviceOpen_TC_06_003_007_testcase Test Case Example
 * @snippet test_io.cpp IoDeviceOpen_TC_06_003_007
 */
// [IoDeviceOpen_TC_06_003_007]
TEST_F(OsalApiStateTest, IoDeviceOpen_TC_06_003_007)
{
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;

// Test06_003_007
// @brief: abnormal case: state transition error when open a device
    memset(&device_handle, DEVICE_HANDLE_INITIALIZED_VALUE, sizeof(device_handle));
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_STATE) << "Test06_003_007 failed";
    EXPECT_EQ(device_handle, OSAL_DEVICE_HANDLE_INVALID) << "Test06_003_007 failed";
}
// [IoDeviceOpen_TC_06_003_007]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoDeviceOpen_TC_06_003_008 IOApiTest.IoDeviceOpen_TC_06_003_008
 * @subsection IoDeviceOpen_TC_06_003_008_tsi Test Suite ID
 * IOApiTest
 * @subsection IoDeviceOpen_TC_06_003_008_tci Test Case ID
 * IoDeviceOpen_S1_N_TC_06_003_008
 * @subsection IoDeviceOpen_TC_06_003_008_rr Related Requirement(s)
 * \#199198, \#199210, \#199211, \#199212
 * @subsection IoDeviceOpen_TC_06_003_008_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - The synchronization queue was created
 * @subsection IoDeviceOpen_TC_06_003_008_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoDeviceOpen and R_OSAL_IoDeviceClose function with the sample device in multi-threads:
 * - Test06_003_008: Open/Close same device in multi-threads
 * @subsection IoDeviceOpen_TC_06_003_008_tpts Test Procedure/Test Steps
 * -# Create first thread using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE3
 *          -# task_name is "Open/Close same device in multi-threads"
 *          -# func is "io_open_close_device_child_thread" function with functionality:
 *              - Get config of synchronization queue by using R_OSAL_MqGetConfig
 *              - Close device when it haven't been openned yet by using R_OSAL_IoDeviceClose
 *              - Open device TEST_IO_DEVICE_1 by using R_OSAL_IoDeviceOpen
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *          -# userarg includes device handle and thread id
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2
 *      - Address of first thread handle.
 * -# Receive message from synchronization queue by using R_OSAL_MqReceiveForTimePeriod
 * -# Create second thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE3
 *          -# task_name is "Open/Close same device in multi-threads"
 *          -# func is "io_open_close_device_child_thread" function with functionality:
 *              - Open device when it have already openned by using R_OSAL_IoDeviceOpen
 *              - Close test target device by using R_OSAL_IoDeviceClose.
 *          -# userarg includes device handle and thread id
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID3
 *      - Address of second thread handle.
 * -# Join first thread by using R_OSAL_ThreadJoin.
 * -# Join second thread by using R_OSAL_ThreadJoin.
 * @subsection IoDeviceOpen_TC_06_003_008_ereb Expected Result/Behavior
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with first thread.<br>
 * "thread_handle1" is not NULL_PTR.<br>
 * In io_open_close_device_child_thread function:
 *  - R_OSAL_MqGetConfig return OSAL_RETURN_OK.
 *  - R_OSAL_IoDeviceClose return OSAL_RETURN_HANDLE.
 *  - R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 *  - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK.
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK.<br>
 * The value of "receive_msg" need to be equal to "send_msg"
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with second thread.<br>
 * "thread_handle2" is not NULL_PTR.<br>
 * In io_open_close_device_child_thread function:
 *  - R_OSAL_IoDeviceOpen return OSAL_RETURN_BUSY.
 *  - R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 *  - Set device handle to OSAL_DEVICE_HANDLE_INVALID.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with first thread.<br>
 * Value of "result" is equal to OSAL_RETURN_OK.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with second thread.<br>
 * Value of "result" is equal to OSAL_RETURN_OK.
 * @subsection IoDeviceOpen_TC_06_003_008_testcase Test Case Example
 * @snippet test_io.cpp IoDeviceOpen_TC_06_003_008
 * @snippet test_io.cpp io_open_close_device_child_thread
 */
// [IoDeviceOpen_TC_06_003_008]
TEST_F(IOApiTest, IoDeviceOpen_TC_06_003_008)
{

    st_osal_thread_config_t config_thread;
    int64_t                 result          = 0;
    osal_thread_handle_t    thread_handle1  = OSAL_THREAD_HANDLE_INVALID;
    osal_thread_handle_t    thread_handle2  = OSAL_THREAD_HANDLE_INVALID;

    // Test06_003_008
    // @brief: normal case: Open/Close same device in multi-threads.

    // Thread 1: Open device
    io_test_multiple_thread_worker_arg user_arg1(&device_handle, TEST_THREAD_ID2);
    user_arg1.send_msg = send_msg;
    user_arg1.mq = sync_mq;
    config_thread.userarg       = &user_arg1;
    config_thread.priority      = OSAL_THREAD_PRIORITY_TYPE3;
    config_thread.task_name     = (char *)"Open/Close same device in multi-threads";
    config_thread.func          = io_open_close_device_child_thread;
    config_thread.stack_size    = THREAD_STACK_SIZE;
    EXPECT_EQ(R_OSAL_ThreadCreate(&config_thread, TEST_THREAD_ID2, &thread_handle1), OSAL_RETURN_OK) << "Test06_003_008 failed";
    EXPECT_NE(thread_handle1, NULL_PTR) << "Test06_003_008 failed";
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &receive_msg, mq_config.msg_size), OSAL_RETURN_OK) << "Test06_003_008 failed";
    EXPECT_EQ(receive_msg, send_msg) << "Test06_003_008 failed";

    // Thread 2: Close device
    user_arg1.uid              = TEST_THREAD_ID3;
    EXPECT_EQ(R_OSAL_ThreadCreate(&config_thread, TEST_THREAD_ID3, &thread_handle2), OSAL_RETURN_OK) << "Test06_003_008 failed";
    EXPECT_NE(thread_handle2, NULL_PTR) << "Test06_003_008 failed";

    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &result),OSAL_RETURN_OK) << "Test06_003_008 failed";
    EXPECT_EQ(result, (int64_t)OSAL_RETURN_OK) << "Test06_003_008 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle2, &result),OSAL_RETURN_OK) << "Test06_003_008 failed";
    EXPECT_EQ(result, (int64_t)OSAL_RETURN_OK) << "Test06_003_008 failed";
}
// [IoDeviceOpen_TC_06_003_008]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoDeviceClose_TC_06_004_001 IOApiTest.IoDeviceClose_TC_06_004_001
 * @subsection IoDeviceClose_TC_06_004_001_tsi Test Suite ID
 * IOApiTest
 * @subsection IoDeviceClose_TC_06_004_001_tci Test Case ID
 * IoDeviceClose_S1_N_TC_06_004_001
 * @subsection IoDeviceClose_TC_06_004_001_rr Related Requirement(s)
 * \#199198, \#199211, \#199212
 * @subsection IoDeviceClose_TC_06_004_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - "device_list" contain the device list of each "device_type" in "devices".
 * @subsection IoDeviceClose_TC_06_004_001_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoDeviceClose function in normal condition:
 * - pre-process: open all target testing device
 * - Test06_004_001: close all device type in "devices".
 * @subsection IoDeviceClose_TC_06_004_001_tpts Test Procedure/Test Steps
 * -# Open all available devices in "device_list" by using R_OSAL_IoDeviceOpen.
 * -# Close all opened device at step 1 for the next test cases by using R_OSAL_IoDeviceClose.
 * @subsection IoDeviceClose_TC_06_004_001_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoDeviceClose_TC_06_004_001_testcase Test Case Example
 * @snippet test_io.cpp IoDeviceClose_TC_06_004_001
 */
// [IoDeviceClose_TC_06_004_001]
TEST_F(IOApiTest, IoDeviceClose_TC_06_004_001)
{
    for (auto it = device_list.begin(); it != device_list.end(); it++)
    {
        // pre-process: open all target testing device
        EXPECT_EQ(R_OSAL_IoDeviceOpen((*it).c_str(), &device_handle), OSAL_RETURN_OK) << "pre-process failed";
        EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";

        // Test06_004_001
        // @brief: normal case: close all device type in "devices".
        EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "Test06_004_001 failed";
        device_handle = OSAL_DEVICE_HANDLE_INVALID;
    }
}
// [IoDeviceClose_TC_06_004_001]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoDeviceClose_TC_06_004_002 IOApiTest.IoDeviceClose_TC_06_004_002
 * @subsection IoDeviceClose_TC_06_004_002_tsi Test Suite ID
 * IOApiTest
 * @subsection IoDeviceClose_TC_06_004_002_tci Test Case ID
 * IoDeviceClose_S0_A_TC_06_004_002
 * @subsection IoDeviceClose_TC_06_004_002_rr Related Requirement(s)
 * \#199199, \#199212
 * @subsection IoDeviceClose_TC_06_004_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoDeviceClose_TC_06_004_002_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoDeviceClose function in abnormal condition:
 * - Test06_004_002: device handle is NULL.
 * @subsection IoDeviceClose_TC_06_004_002_tpts Test Procedure/Test Steps
 * -# Close device by using R_OSAL_IoDeviceClose with the argumnent as below:
 *      - Device handle is NULL
 * @subsection IoDeviceClose_TC_06_004_002_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_HANDLE.
 * @subsection IoDeviceClose_TC_06_004_002_testcase Test Case Example
 * @snippet test_io.cpp IoDeviceClose_TC_06_004_002
 */
// [IoDeviceClose_TC_06_004_002]
TEST_F(IOApiTest, IoDeviceClose_TC_06_004_002)
{
// Test06_004_002
// @brief: abnormal case: device handle is NULL.
    EXPECT_EQ(R_OSAL_IoDeviceClose(OSAL_DEVICE_HANDLE_INVALID), OSAL_RETURN_HANDLE) << "Test06_004_002 failed";
}
// [IoDeviceClose_TC_06_004_002]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoDeviceClose_TC_06_004_003 IOApiTest.IoDeviceClose_TC_06_004_003
 * @subsection IoDeviceClose_TC_06_004_003_tsi Test Suite ID
 * IOApiTest
 * @subsection IoDeviceClose_TC_06_004_003_tci Test Case ID
 * IoDeviceClose_S2_A_TC_06_004_003
 * @subsection IoDeviceClose_TC_06_004_003_rr Related Requirement(s)
 * \#199212
 * @subsection IoDeviceClose_TC_06_004_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoDeviceClose_TC_06_004_003_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoDeviceClose function in abnormal condition:
 * - pre-process: open device test target device
 * - Test06_004_003: close on closed device handle.
 * @subsection IoDeviceClose_TC_06_004_003_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen.
 * -# Close test target device by using R_OSAL_IoDeviceClose.
 * -# Continue to close test target device by using R_OSAL_IoDeviceClose.
 * @subsection IoDeviceClose_TC_06_004_003_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_HANDLE.
 * @subsection IoDeviceClose_TC_06_004_003_testcase Test Case Example
 * @snippet test_io.cpp IoDeviceClose_TC_06_004_003
 */
// [IoDeviceClose_TC_06_004_003]
TEST_F(IOApiTest, IoDeviceClose_TC_06_004_003)
{
// pre-process: open test target device
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";

// Test06_004_003
// @brief: abnormal case: close on closed device handle.
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_HANDLE) << "Test06_004_003 failed";
}
// [IoDeviceClose_TC_06_004_003]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoDeviceClose_TC_06_004_004 IOApiTest.IoDeviceClose_TC_06_004_004
 * @subsection IoDeviceClose_TC_06_004_004_tsi Test Suite ID
 * IOApiTest
 * @subsection IoDeviceClose_TC_06_004_004_tci Test Case ID
 * IoDeviceClose_S2_A_TC_06_004_004
 * @subsection IoDeviceClose_TC_06_004_004_rr Related Requirement(s)
 * \#199212
 * @subsection IoDeviceClose_TC_06_004_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - "device_list" contain the device list of each "device_type" in "devices".
 * @subsection IoDeviceClose_TC_06_004_004_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoDeviceClose function in abnormal condition:
 * - pre-process: open test target device
 * - Test06_004_004: close deivce when interrupt has registered.
 * - post-process: close opened test target device
 * @subsection IoDeviceClose_TC_06_004_004_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen.
 * -# Get the number of interrupt channel corresponding with opened device handle (test target device) by calling to R_OSAL_InterruptGetNumOfIrqChannels.
 * -# Register interrupt callback "osal_io_test_cb_sample" for an interrupt occurs from interrupt channel 0 of test target device by calling to R_OSAL_InterruptRegisterIsr.
 * -# Close test target device by using R_OSAL_IoDeviceClose.
 * -# Unregister interrupt callback "osal_io_test_cb_sample" registered at step 2 by calling to R_OSAL_InterruptUnregisterIsr.
 * -# Close test target device by using R_OSAL_IoDeviceClose again.
 * @subsection IoDeviceClose_TC_06_004_004_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * Value of "numOfChannel" is bigger than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_BUSY.
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoDeviceClose_TC_06_004_004_testcase Test Case Example
 * @snippet test_io.cpp IoDeviceClose_TC_06_004_004
 * @snippet test_io.cpp osal_io_test_cb_sample
 */
// [IoDeviceClose_TC_06_004_004]
TEST_F(IOApiTest, IoDeviceClose_TC_06_004_004)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE10;
    int arg = 10;
    uint64_t irq_channel = 0;

// pre-process: open test target device
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";

// Test06_004_004
// @brief: abnormal case: close deivce when interrupt has registered.
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK);
    EXPECT_GT(numOfChannel, 0);
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, irq_channel, priority, osal_io_test_cb_sample, (void *)&arg), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_BUSY) << "Test06_004_004 failed";
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, irq_channel, osal_io_test_cb_sample), OSAL_RETURN_OK);

// post-process: Close opened device
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
}
// [IoDeviceClose_TC_06_004_004]

/**
 * @page IO_TEST IO API Test
 * @section OsalApiStateTest_IoDeviceClose_TC_06_004_005 OsalApiStateTest.IoDeviceClose_TC_06_004_005
 * @subsection IoDeviceClose_TC_06_004_005_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection IoDeviceClose_TC_06_004_005_tci Test Case ID
 * IoDeviceClose_S2_A_TC_06_004_005
 * @subsection IoDeviceClose_TC_06_004_005_rr Related Requirement(s)
 * \#199212
 * @subsection IoDeviceClose_TC_06_004_005_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize has already been called
 * @subsection IoDeviceClose_TC_06_004_005_tcs Test Case Summary
 * This test shall verify the state transition of the R_OSAL_IoDeviceClose function:
 * - Test06_004_005: confirm state transition error with R_OSAL_IoDeviceClose
 * @subsection IoDeviceClose_TC_06_004_005_tpts Test Procedure/Test Steps
 * -# Confirm the state transition of R_OSAL_IoDeviceClose
 * @subsection IoDeviceClose_TC_06_004_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_STATE
 * @subsection IoDeviceClose_TC_06_004_005_testcase Test Case Example
 * @snippet test_io.cpp IoDeviceClose_TC_06_004_005
 */
// [IoDeviceClose_TC_06_004_005]
TEST_F(OsalApiStateTest, IoDeviceClose_TC_06_004_005)
{
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;

// Test06_004_005
// @brief: abnormal case: state transition error when close a device
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_STATE) << "Test06_004_005 failed";
}
// [IoDeviceClose_TC_06_004_005]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoDeviceClose_TC_06_004_006 IOApiTest.IoDeviceClose_TC_06_004_006
 * @subsection IoDeviceClose_TC_06_004_006_tsi Test Suite ID
 * IOApiTest
 * @subsection IoDeviceClose_TC_06_004_006_tci Test Case ID
 * IoDeviceClose_S2_A_TC_06_004_006
 * @subsection IoDeviceClose_TC_06_004_006_rr Related Requirement(s)
 * \#199212
 * @subsection IoDeviceClose_TC_06_004_006_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - The synchronization queue was created
 * @subsection IoDeviceClose_TC_06_004_006_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoDeviceClose function in abnormal condition:
 * - pre-process: open device test target device and create a mutex
 * - Test06_004_006: close the device while it is handled in power management.
 * - post-process: destroy mutex and close test target device
 * @subsection IoDeviceClose_TC_06_004_006_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen.
 * -# Create a new mutex by using R_OSAL_ThsyncMutexCreate with the argument as below:
 *      - mutex_id is TEST_MUTEX_ID1
 *      - Address of "mtx_handle"
 * -# Create first thread using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE2
 *          -# task_name is "Simple testing thread"
 *          -# func is "device_wait_pm_state_worker_function" function with functionality:
 *              - Get config of synchronization queue by using R_OSAL_MqGetConfig
 *              - Lock mutex by using R_OSAL_ThsyncMutexTryLock.
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *              - Set power required state of test target device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState with applyImmediate is true
 *              - Set power policy to OSAL_PM_POLICY_HP by using R_OSAL_PmSetPolicy with applyImmediate is true
 *              - Set power required state of test target device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState with applyImmediate is false
 *              - Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 *              - Wait power state to become OSAL_PM_STATE_PG by using R_OSAL_PmWaitForState with timeout is 5sec
 *          -# userarg includes device handle and mutex handle
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID1
 *      - Address of first thread handle.
 * -# Receive message from synchronization queue by using R_OSAL_MqReceiveForTimePeriod
 * -# Create second thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE1
 *          -# task_name is "Simple testing thread"
 *          -# func is "device_close_woker_function" function with functionality:
 *              - Lock mutex by using R_OSAL_ThsyncMutexLockForTimePeriod with timeout is 2sec
 *              - Close test target device by using R_OSAL_IoDeviceClose.
 *              - Unlock mutex by using R_OSAL_ThsyncMutexUnlock.
 *          -# userarg includes device handle and mutex handle
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2
 *      - Address of second thread handle.
 * -# Join first thread by using R_OSAL_ThreadJoin.
 * -# Join second thread by using R_OSAL_ThreadJoin.
 * -# Destroy the mutex by using R_OSAL_ThsyncMutexDestroy.
 * -# Close test target device by using R_OSAL_IoDeviceClose.
 * @subsection IoDeviceClose_TC_06_004_006_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# R_OSAL_ThsyncMutexCreate return OSAL_RETURN_OK.<br>
 * Value of "mtx_handle" is not NULL.
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with first thread.<br>
 * "thread_handle" is not NULL_PTR.<br>
 * In device_wait_pm_state_worker_function function:<br>
 *  - R_OSAL_MqGetConfig return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncMutexTryLock return OSAL_RETURN_OK.
 *  - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 *  - R_OSAL_PmSetPolicy return OSAL_RETURN_OK.
 *  - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 *  - R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK
 *  - R_OSAL_PmWaitForState return OSAL_RETURN_TIME.
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK.<br>
 * The value of "receive_msg" need to be equal to "send_msg"
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with second thread.<br>
 * "thread_handle1" is not NULL_PTR.<br>
 * In device_close_woker_function function:<br>
 *  - R_OSAL_ThsyncMutexLockForTimePeriod return OSAL_RETURN_OK.
 *  - R_OSAL_IoDeviceClose return OSAL_RETURN_BUSY.
 *  - R_OSAL_ThsyncMutexUnlock return OSAL_RETURN_OK.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with first thread.<br>
 * Value of "result" is equal to OSAL_RETURN_TIME.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with second thread.<br>
 * Value of "result" is equal to OSAL_RETURN_BUSY.
 * -# R_OSAL_ThsyncMutexDestroy return OSAL_RETURN_OK.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoDeviceClose_TC_06_004_006_testcase Test Case Example
 * @snippet test_io.cpp IoDeviceClose_TC_06_004_006
 * @snippet test_io.cpp device_wait_pm_state_worker_function
 * @snippet test_io.cpp device_close_woker_function
 */
// [IoDeviceClose_TC_06_004_006]
TEST_F(IOApiTest, IoDeviceClose_TC_06_004_006)
{
    osal_mutex_handle_t mtx_handle = OSAL_MUTEX_HANDLE_INVALID;
    osal_thread_handle_t thread_handle = OSAL_THREAD_HANDLE_INVALID;
    osal_thread_handle_t thread_handle1 = OSAL_THREAD_HANDLE_INVALID;
    osal_mutex_id_t mutex_id;
    osal_thread_id_t uid;
    st_osal_thread_config_t config;
    int64_t result;

// pre-process: open test target device and create a mutex
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_BUSY_CHECK_DEVICE, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    mutex_id = TEST_MUTEX_ID1;
    EXPECT_EQ(R_OSAL_ThsyncMutexCreate(mutex_id, &mtx_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(mtx_handle, NULL_PTR) << "pre-process failed";

// Test06_004_006
// @brief: abnormal case: close device with PM active
    device_close_abnormal_worker_arg user_arg(&mtx_handle, &device_handle);
    user_arg.send_msg = send_msg;
    user_arg.mq = sync_mq;
    uid              = TEST_THREAD_ID1;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE2;
    config.task_name = (char *)"Simple testing thread";
    config.func      = device_wait_pm_state_worker_function;
    config.userarg   = &user_arg;
    config.stack_size = THREAD_STACK_SIZE;
    EXPECT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle), OSAL_RETURN_OK) << "Test06_004_006 failed";
    EXPECT_NE(thread_handle, NULL_PTR);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &receive_msg, mq_config.msg_size), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

    uid              = TEST_THREAD_ID2;
    config.priority  = OSAL_THREAD_PRIORITY_TYPE1;
    config.func      = device_close_woker_function;
    EXPECT_EQ(R_OSAL_ThreadCreate(&config, uid, &thread_handle1), OSAL_RETURN_OK) << "Test06_004_006 failed";
    EXPECT_NE(thread_handle1, NULL_PTR);
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle, &result), OSAL_RETURN_OK) << "Test06_004_006 failed";
    EXPECT_EQ(result, (int64_t)OSAL_RETURN_TIME) << "Test06_004_006 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &result), OSAL_RETURN_OK) << "Test06_004_006 failed";
    EXPECT_EQ(result, (int64_t)OSAL_RETURN_BUSY) << "Test06_004_006 failed";
    thread_handle = OSAL_THREAD_HANDLE_INVALID;
    thread_handle1 = OSAL_THREAD_HANDLE_INVALID;

// post-process: Destroy mutex and close opened device
    EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
}
// [IoDeviceClose_TC_06_004_006]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoRead8_TC_06_005_001 IOApiTest.IoRead8_TC_06_005_001
 * @subsection IoRead8_TC_06_005_001_tsi Test Suite ID
 * IOApiTest
 * @subsection IoRead8_TC_06_005_001_tci Test Case ID
 * IoRead_S1_N_TC_06_005_001
 * @subsection IoRead8_TC_06_005_001_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203, \#199211
 * @subsection IoRead8_TC_06_005_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - "device_list" contain the device list of each "device_type" in "devices".
 * @subsection IoRead8_TC_06_005_001_tcs Test Case Summary
 * This test shall verify the functionality of the IoRead APIs of OSAL on normal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_005_001: use R_OSAL_IoRead8 to read data from register of test target device with offset "TEST_READ8_REG_OFFSET_NC".
 * - post-process: close test target device
 * @subsection IoRead8_TC_06_005_001_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data from test target device with offset "TEST_READ8_REG_OFFSET_NC" by using R_OSAL_IoRead8.
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoRead8_TC_06_005_001_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead8 return OSAL_RETURN_OK.<br>
 * Read data is the same as the expected data.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoRead8_TC_06_005_001_testcase Test Case Example
 * @snippet test_io.cpp IoRead8_TC_06_005_001
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoRead8_TC_06_005_001]
TEST_F(IOApiTest, IoRead8_TC_06_005_001)
{
    uint8_t read_data8;
    uintptr_t offset = TEST_READ8_REG_OFFSET_NC;
    uint8_t expected_data = TEST_READ8_VAL_NC;

    for (auto it = device_list.begin(); it != device_list.end(); it++)
    {
        if (!strcmp((*it).c_str(), TEST_IO_DEVICE_1))
        {
            // pre-proces: open test target device, turn ON its power and clock
            EXPECT_EQ(R_OSAL_IoDeviceOpen((*it).c_str(), &device_handle), OSAL_RETURN_OK) << "pre-process failed";
            EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
            EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

            // Test06_005_001
            // @brief: normal case: read register value of test target device with aligned offset.
            EXPECT_EQ(R_OSAL_IoRead8(device_handle, offset, &read_data8), OSAL_RETURN_OK) << "Test06_005_001 failed";
            EXPECT_EQ(read_data8, expected_data) << "Test06_005_001 failed";

            // post-process: close test target device
            EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
            EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
            device_handle = OSAL_DEVICE_HANDLE_INVALID;
        }
    }
}
// [IoRead8_TC_06_005_001]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoRead8_TC_06_005_003 IOApiTest.IoRead8_TC_06_005_003
 * @subsection IoRead8_TC_06_005_003_tsi Test Suite ID
 * IOApiTest
 * @subsection IoRead8_TC_06_005_003_tci Test Case ID
 * IoRead_S0_A_TC_06_005_003
 * @subsection IoRead8_TC_06_005_003_rr Related Requirement(s)
 * \#199151, \#199199, \#199200, \#199201, \#199202, \#199203
 * @subsection IoRead8_TC_06_005_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoRead8_TC_06_005_003_tcs Test Case Summary
 * This test shall verify the functionality of the IoRead APIs of OSAL on abnormal condition:
 * - Test06_005_003: use R_OSAL_IoRead8 to read register value with device handle is NULL
 * @subsection IoRead8_TC_06_005_003_tpts Test Procedure/Test Steps
 * -# Read data with offset "TEST_READ8_REG_OFFSET_AC" by using R_OSAL_IoRead8.
 * @subsection IoRead8_TC_06_005_003_ereb Expected Result/Behavior
 * -# R_OSAL_IoRead8 return OSAL_RETURN_HANDLE
 * @subsection IoRead8_TC_06_005_003_testcase Test Case Example
 * @snippet test_io.cpp IoRead8_TC_06_005_003
 */
// [IoRead8_TC_06_005_003]
TEST_F(IOApiTest, IoRead8_TC_06_005_003)
{
    uintptr_t offset;
    uint8_t read_data8;

    // @brief: abnormal case: read register value with device handle is NULL
    // Test06_005_003
    offset = TEST_READ8_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoRead8(OSAL_DEVICE_HANDLE_INVALID, offset, &read_data8), OSAL_RETURN_HANDLE) << "Test06_005_003 failed";
}
// [IoRead8_TC_06_005_003]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoRead8_TC_06_005_004 IOApiTest.IoRead8_TC_06_005_004
 * @subsection IoRead8_TC_06_005_004_tsi Test Suite ID
 * IOApiTest
 * @subsection IoRead8_TC_06_005_004_tci Test Case ID
 * IoRead_S0_BA_TC_06_005_004
 * @subsection IoRead8_TC_06_005_004_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203
 * @subsection IoRead8_TC_06_005_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoRead8_TC_06_005_004_tcs Test Case Summary
 * This test shall verify the functionality of the IoRead APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_005_004: use R_OSAL_IoRead8 read register value of test target device with offset which is out-of-register-range
 * - post-process: close test target device
 * @subsection IoRead8_TC_06_005_004_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data with offset "TEST_OFFSET_OUT_RANGE" by using R_OSAL_IoRead8.
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoRead8_TC_06_005_004_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead8 return OSAL_RETURN_PAR
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoRead8_TC_06_005_004_testcase Test Case Example
 * @snippet test_io.cpp IoRead8_TC_06_005_004
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoRead8_TC_06_005_004]
TEST_F(IOApiTest, IoRead8_TC_06_005_004)
{
    uintptr_t offset;
    uint8_t read_data8;

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: read register value with offset which is out-of-register-range
    // Test06_005_004
    offset = TEST_OFFSET_OUT_RANGE;
    EXPECT_EQ(R_OSAL_IoRead8(device_handle, offset, &read_data8), OSAL_RETURN_PAR) << "Test06_005_004 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoRead8_TC_06_005_004]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoRead8_TC_06_005_005 IOApiTest.IoRead8_TC_06_005_005
 * @subsection IoRead8_TC_06_005_005_tsi Test Suite ID
 * IOApiTest
 * @subsection IoRead8_TC_06_005_005_tci Test Case ID
 * IoRead_S0_A_TC_06_005_005
 * @subsection IoRead8_TC_06_005_005_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203
 * @subsection IoRead8_TC_06_005_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoRead8_TC_06_005_005_tcs Test Case Summary
 * This test shall verify the functionality of the IoRead APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_005_005: use R_OSAL_IoRead8 read register value of test target device with p_data is NULL
 * - post-process: close test target device
 * @subsection IoRead8_TC_06_005_005_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data with offset "TEST_READ8_REG_OFFSET_AC" by using R_OSAL_IoRead8.
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoRead8_TC_06_005_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead8 return OSAL_RETURN_PAR
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoRead8_TC_06_005_005_testcase Test Case Example
 * @snippet test_io.cpp IoRead8_TC_06_005_005
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoRead8_TC_06_005_005]
TEST_F(IOApiTest, IoRead8_TC_06_005_005)
{
    uintptr_t offset;
    void* p_data = NULL;

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: read register value with p_data is NULL
    // Test06_005_005
    offset = TEST_READ8_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoRead8(device_handle, offset, (uint8_t*)p_data), OSAL_RETURN_PAR) << "Test06_005_005 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoRead8_TC_06_005_005]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoRead8_TC_06_005_006 IOApiTest.IoRead8_TC_06_005_006
 * @subsection IoRead8_TC_06_005_006_tsi Test Suite ID
 * IOApiTest
 * @subsection IoRead8_TC_06_005_006_tci Test Case ID
 * IoRead_S2_A_TC_06_005_006
 * @subsection IoRead8_TC_06_005_006_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203
 * @subsection IoRead8_TC_06_005_006_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoRead8_TC_06_005_006_tcs Test Case Summary
 * This test shall verify the functionality of the IoRead APIs of OSAL on abnormal condition:
 * - Test06_005_006: use R_OSAL_IoRead8 read register value with closed device handle
 * @subsection IoRead8_TC_06_005_006_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * -# Read data with offset "TEST_READ8_REG_OFFSET_AC" by using R_OSAL_IoRead8.
 * @subsection IoRead8_TC_06_005_006_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_IoRead8 return OSAL_RETURN_HANDLE
 * @subsection IoRead8_TC_06_005_006_testcase Test Case Example
 * @snippet test_io.cpp IoRead8_TC_06_005_006
 */
// [IoRead8_TC_06_005_006]
TEST_F(IOApiTest, IoRead8_TC_06_005_006)
{
    uintptr_t offset;
    uint8_t read_data8;

    // @brief: abnormal case: read register value with closed device handle
    // Test06_005_006
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "Test06_005_006 failed";
    EXPECT_NE(device_handle, NULL_PTR) << "Test06_005_006 failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "Test06_005_006 failed";

    offset = TEST_READ8_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoRead8(device_handle, offset, &read_data8), OSAL_RETURN_HANDLE) << "Test06_005_006 failed";
}
// [IoRead8_TC_06_005_006]

/**
 * @page IO_TEST IO API Test
 * @section OsalApiStateTest_IoRead8_TC_06_005_007 OsalApiStateTest.IoRead8_TC_06_005_007
 * @subsection IoRead8_TC_06_005_007_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection IoRead8_TC_06_005_007_tci Test Case ID
 * IoRead_S2_A_TC_06_005_007
 * @subsection IoRead8_TC_06_005_007_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203
 * @subsection IoRead8_TC_06_005_007_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize has already been called
 * @subsection IoRead8_TC_06_005_007_tcs Test Case Summary
 * This test shall verify the state transition of the R_OSAL_IoRead8 function:
 * - Test06_005_007: confirm state transition error with R_OSAL_IoRead8
 * @subsection IoRead8_TC_06_005_007_tpts Test Procedure/Test Steps
 * -# Confirm the state transition of R_OSAL_IoRead8
 * @subsection IoRead8_TC_06_005_007_ereb Expected Result/Behavior
 * -# R_OSAL_IoRead8 return OSAL_RETURN_STATE
 * @subsection IoRead8_TC_06_005_007_testcase Test Case Example
 * @snippet test_io.cpp IoRead8_TC_06_005_007
 */
// [IoRead8_TC_06_005_007]
TEST_F(OsalApiStateTest, IoRead8_TC_06_005_007)
{
    uint8_t read_data8;
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;
    uintptr_t offset = TEST_READ8_REG_OFFSET_AC;

    // Test06_005_007
    // @brief: abnormal case: state transition error with IO Read
    EXPECT_EQ(R_OSAL_IoRead8(device_handle, offset, &read_data8), OSAL_RETURN_STATE) << "Test06_005_007 failed";
}
// [IoRead8_TC_06_005_007]

/**
 * @page IO_TEST IO API Test
 * @section OSALINTTESTS_IoRead8_TC_06_005_008 OSALINTTESTS.IoRead8_TC_06_005_008
 * @subsection IoRead8_TC_06_005_008_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection IoRead8_TC_06_005_008_tci Test Case ID
 * IoRead_S1_N_TC_06_005_008
 * @subsection IoRead8_TC_06_005_008_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203, \#199211
 * @subsection IoRead8_TC_06_005_008_pp Prerequisite/Precondition
 * -# OSAL is initialized.<br>
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection IoRead8_TC_06_005_008_tcs Test Case Summary
 * @subsubsection IoRead8_TC_06_005_008_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Setup required memory for interrupt generation.
 * - Register callack function and enable interrupt.
 * - Wait for the interrupt callback function finish.
 * @subsubsection IoRead8_TC_06_005_008_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * @subsubsection IoRead8_TC_06_005_008_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality of the IO APIs which are called in the ISR
 * - Disable and clear interrupt status
 * - Test06_005_008: Read data from test target device register by using R_OSAL_IoRead with 8 bit in ISR
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection IoRead8_TC_06_005_008_tpts Test Procedure/Test Steps
 * @subsubsection IoRead8_TC_06_005_008_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Allocate required memory for generate interrupt from test target device and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function by using R_OSAL_InterruptRegisterIsr.<br>
 * -# Enable interrupt by using R_OSAL_InterruptEnableIsr
 * -# Setup test target device specific register to generate the interrupt by calling to setup_hw_register
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection IoRead8_TC_06_005_008_post_tpts Test Procedure/Test Steps of Post-process
 * -# Unregister interrupt by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * @subsubsection IoRead8_TC_06_005_008_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Read data from test target device with offset "TEST_READ8_REG_OFFSET_INT" by using R_OSAL_IoRead8.
 * -# Send Message interrupt of context finished by using R_OSAL_MqSendForTimePeriod
 * @subsection IoRead8_TC_06_005_008_ereb Expected Result/Behavior
 * @subsubsection IoRead8_TC_06_005_008_pre_ereb Expected Result/Behavior of Pre-process
 * -# allocate_setup_data_memory run successfully
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * The value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK
 * -# setup_hw_register run successfully
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection IoRead8_TC_06_005_008_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * @subsubsection IoRead8_TC_06_005_008_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_IoRead8 return OSAL_RETURN_OK.<br>
 * Read data is the same as the expected data.
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * @subsection IoRead8_TC_06_005_008_testcase Test Case Example
 * @snippet test_io.cpp IoRead8_TC_06_005_008
 * @snippet test_io.cpp osal_io_isr_test_TC_06_005_008
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [IoRead8_TC_06_005_008]
TEST_F(OSALINTTESTS, IoRead8_TC_06_005_008)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    st_io_arg_t io_arg;
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
 * Wait for the interrupt callback function finish. */
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_GT(numOfChannel, 0) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_io_isr_test_TC_06_005_008, (void *)&arg), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK) << "pre-process failed";
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory*/
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_io_isr_test_TC_06_005_008), OSAL_RETURN_OK) << "post-process failed";
    deallocate_data_memory(hndl_mmngr, buffer_obj);
}
// [IoRead8_TC_06_005_008]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoRead16_TC_06_006_001 IOApiTest.IoRead16_TC_06_006_001
 * @subsection IoRead16_TC_06_006_001_tsi Test Suite ID
 * IOApiTest
 * @subsection IoRead16_TC_06_006_001_tci Test Case ID
 * IoRead_S1_N_TC_06_006_001
 * @subsection IoRead16_TC_06_006_001_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203, \#199211
 * @subsection IoRead16_TC_06_006_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - "device_list" contain the device list of each "device_type" in "devices".
 * @subsection IoRead16_TC_06_006_001_tcs Test Case Summary
 * This test shall verify the functionality of the IoRead APIs of OSAL on normal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_006_001: use R_OSAL_IoRead16 to read data from register of test target device with offset "TEST_READ16_REG_OFFSET_NC".
 * - post-process: close test target device
 * @subsection IoRead16_TC_06_006_001_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data from test target device with offset "TEST_READ16_REG_OFFSET_NC" by using R_OSAL_IoRead16.
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoRead16_TC_06_006_001_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead16 return OSAL_RETURN_OK.<br>
 * Read data is the same as the expected data.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoRead16_TC_06_006_001_testcase Test Case Example
 * @snippet test_io.cpp IoRead16_TC_06_006_001
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoRead16_TC_06_006_001]
TEST_F(IOApiTest, IoRead16_TC_06_006_001)
{
    uint16_t read_data16;
    uintptr_t offset = TEST_READ16_REG_OFFSET_NC;
    uint16_t expected_data = TEST_READ16_VAL_NC;

    for (auto it = device_list.begin(); it != device_list.end(); it++)
    {
        if (!strcmp((*it).c_str(), TEST_IO_DEVICE_1))
        {
            // pre-proces: open test target device, turn ON its power and clock
            EXPECT_EQ(R_OSAL_IoDeviceOpen((*it).c_str(), &device_handle), OSAL_RETURN_OK) << "pre-process failed";
            EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
            EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

            // Test06_006_001
            // @brief: normal case: read register value of test target device with aligned offset.
            EXPECT_EQ(R_OSAL_IoRead16(device_handle, offset, &read_data16), OSAL_RETURN_OK) << "Test06_006_001 failed";
            EXPECT_EQ(read_data16, expected_data) << "Test06_006_001 failed";

            // post-process: close test target device
            EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
            EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
            device_handle = OSAL_DEVICE_HANDLE_INVALID;
        }
    }
}
// [IoRead16_TC_06_006_001]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoRead16_TC_06_006_002 IOApiTest.IoRead16_TC_06_006_002
 * @subsection IoRead16_TC_06_006_002_tsi Test Suite ID
 * IOApiTest
 * @subsection IoRead16_TC_06_006_002_tci Test Case ID
 * IoRead_S0_A_TC_06_006_002
 * @subsection IoRead16_TC_06_006_002_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203
 * @subsection IoRead16_TC_06_006_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoRead16_TC_06_006_002_tcs Test Case Summary
 * This test shall verify the functionality of the IoRead APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_006_002: use R_OSAL_IoRead16 to read register value from test target device with un-aligned offset.
 * - post-process: close test target device
 * @subsection IoRead16_TC_06_006_002_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data from test target device with offset "TEST_READ16_OFFSET_UNALIGNED" by using R_OSAL_IoRead16.
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoRead16_TC_06_006_002_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead16 return OSAL_RETURN_PAR
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoRead16_TC_06_006_002_testcase Test Case Example
 * @snippet test_io.cpp IoRead16_TC_06_006_002
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoRead16_TC_06_006_002]
TEST_F(IOApiTest, IoRead16_TC_06_006_002)
{
    uintptr_t offset;
    uint16_t read_data16;

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: read register value of test target device with un-aligned offset.
    // Test06_006_002
    offset = TEST_READ16_OFFSET_UNALIGNED;
    EXPECT_EQ(R_OSAL_IoRead16(device_handle, offset, &read_data16), OSAL_RETURN_PAR) << "Test06_006_002 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoRead16_TC_06_006_002]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoRead16_TC_06_006_003 IOApiTest.IoRead16_TC_06_006_003
 * @subsection IoRead16_TC_06_006_003_tsi Test Suite ID
 * IOApiTest
 * @subsection IoRead16_TC_06_006_003_tci Test Case ID
 * IoRead_S0_A_TC_06_006_003
 * @subsection IoRead16_TC_06_006_003_rr Related Requirement(s)
 * \#199151, \#199199, \#199200, \#199201, \#199202, \#199203
 * @subsection IoRead16_TC_06_006_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoRead16_TC_06_006_003_tcs Test Case Summary
 * This test shall verify the functionality of the IoRead APIs of OSAL on abnormal condition:
 * - Test06_006_003: use R_OSAL_IoRead16 to read register value with device handle is NULL
 * @subsection IoRead16_TC_06_006_003_tpts Test Procedure/Test Steps
 * -# Read data with offset "TEST_READ16_REG_OFFSET_AC" by using R_OSAL_IoRead16.
 * @subsection IoRead16_TC_06_006_003_ereb Expected Result/Behavior
 * -# R_OSAL_IoRead16 return OSAL_RETURN_HANDLE
 * @subsection IoRead16_TC_06_006_003_testcase Test Case Example
 * @snippet test_io.cpp IoRead16_TC_06_006_003
 */
// [IoRead16_TC_06_006_003]
TEST_F(IOApiTest, IoRead16_TC_06_006_003)
{
    uintptr_t offset;
    uint16_t read_data16;

    // @brief: abnormal case: read register value with device handle is NULL
    // Test06_006_003
    offset = TEST_READ16_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoRead16(OSAL_DEVICE_HANDLE_INVALID, offset, &read_data16), OSAL_RETURN_HANDLE) << "Test06_006_003 failed";
}
// [IoRead16_TC_06_006_003]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoRead16_TC_06_006_004 IOApiTest.IoRead16_TC_06_006_004
 * @subsection IoRead16_TC_06_006_004_tsi Test Suite ID
 * IOApiTest
 * @subsection IoRead16_TC_06_006_004_tci Test Case ID
 * IoRead_S0_BA_TC_06_006_004
 * @subsection IoRead16_TC_06_006_004_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203
 * @subsection IoRead16_TC_06_006_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoRead16_TC_06_006_004_tcs Test Case Summary
 * This test shall verify the functionality of the IoRead APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_006_004: use R_OSAL_IoRead16 read register value of test target device with offset which is out-of-register-range
 * - post-process: close test target device
 * @subsection IoRead16_TC_06_006_004_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data with offset "TEST_OFFSET_OUT_RANGE" by using R_OSAL_IoRead16.
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoRead16_TC_06_006_004_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead16 return OSAL_RETURN_PAR
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoRead16_TC_06_006_004_testcase Test Case Example
 * @snippet test_io.cpp IoRead16_TC_06_006_004
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoRead16_TC_06_006_004]
TEST_F(IOApiTest, IoRead16_TC_06_006_004)
{
    uintptr_t offset;
    uint16_t read_data16;

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: read register value with offset which is out-of-register-range
    // Test06_006_004
    offset = TEST_OFFSET_OUT_RANGE;
    EXPECT_EQ(R_OSAL_IoRead16(device_handle, offset, &read_data16), OSAL_RETURN_PAR) << "Test06_006_004 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoRead16_TC_06_006_004]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoRead16_TC_06_006_005 IOApiTest.IoRead16_TC_06_006_005
 * @subsection IoRead16_TC_06_006_005_tsi Test Suite ID
 * IOApiTest
 * @subsection IoRead16_TC_06_006_005_tci Test Case ID
 * IoRead_S0_A_TC_06_006_005
 * @subsection IoRead16_TC_06_006_005_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203
 * @subsection IoRead16_TC_06_006_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoRead16_TC_06_006_005_tcs Test Case Summary
 * This test shall verify the functionality of the IoRead APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_006_005: use R_OSAL_IoRead16 read register value of test target device with p_data is NULL
 * - post-process: close test target device
 * @subsection IoRead16_TC_06_006_005_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data with offset "TEST_READ16_REG_OFFSET_AC" by using R_OSAL_IoRead16.
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoRead16_TC_06_006_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead16 return OSAL_RETURN_PAR
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoRead16_TC_06_006_005_testcase Test Case Example
 * @snippet test_io.cpp IoRead16_TC_06_006_005
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoRead16_TC_06_006_005]
TEST_F(IOApiTest, IoRead16_TC_06_006_005)
{
    uintptr_t offset;
    void* p_data = NULL;

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: read register value with p_data is NULL
    // Test06_006_005
    offset = TEST_READ16_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoRead16(device_handle, offset, (uint16_t*)p_data), OSAL_RETURN_PAR) << "Test06_006_005 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoRead16_TC_06_006_005]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoRead16_TC_06_006_006 IOApiTest.IoRead16_TC_06_006_006
 * @subsection IoRead16_TC_06_006_006_tsi Test Suite ID
 * IOApiTest
 * @subsection IoRead16_TC_06_006_006_tci Test Case ID
 * IoRead_S2_A_TC_06_006_006
 * @subsection IoRead16_TC_06_006_006_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203
 * @subsection IoRead16_TC_06_006_006_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoRead16_TC_06_006_006_tcs Test Case Summary
 * This test shall verify the functionality of the IoRead APIs of OSAL on abnormal condition:
 * - Test06_006_006: use R_OSAL_IoRead16 read register value with closed device handle
 * @subsection IoRead16_TC_06_006_006_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * -# Read data with offset "TEST_READ16_REG_OFFSET_AC" by using R_OSAL_IoRead16.
 * @subsection IoRead16_TC_06_006_006_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_IoRead16 return OSAL_RETURN_HANDLE
 * @subsection IoRead16_TC_06_006_006_testcase Test Case Example
 * @snippet test_io.cpp IoRead16_TC_06_006_006
 */
// [IoRead16_TC_06_006_006]
TEST_F(IOApiTest, IoRead16_TC_06_006_006)
{
    uintptr_t offset;
    uint16_t read_data16;

    // @brief: abnormal case: read register value with closed device handle
    // Test06_006_006
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "Test06_006_006 failed";
    EXPECT_NE(device_handle, NULL_PTR) << "Test06_006_006 failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "Test06_006_006 failed";

    offset = TEST_READ16_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoRead16(device_handle, offset, &read_data16), OSAL_RETURN_HANDLE) << "Test06_006_006 failed";
}
// [IoRead16_TC_06_006_006]

/**
 * @page IO_TEST IO API Test
 * @section OsalApiStateTest_IoRead16_TC_06_006_007 OsalApiStateTest.IoRead16_TC_06_006_007
 * @subsection IoRead16_TC_06_006_007_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection IoRead16_TC_06_006_007_tci Test Case ID
 * IoRead_S2_A_TC_06_006_007
 * @subsection IoRead16_TC_06_006_007_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203
 * @subsection IoRead16_TC_06_006_007_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize has already been called
 * @subsection IoRead16_TC_06_006_007_tcs Test Case Summary
 * This test shall verify the state transition of the R_OSAL_IoRead16 function:
 * - Test06_006_007: confirm state transition error with R_OSAL_IoRead16
 * @subsection IoRead16_TC_06_006_007_tpts Test Procedure/Test Steps
 * -# Confirm the state transition of R_OSAL_IoRead16
 * @subsection IoRead16_TC_06_006_007_ereb Expected Result/Behavior
 * -# R_OSAL_IoRead16 return OSAL_RETURN_STATE
 * @subsection IoRead16_TC_06_006_007_testcase Test Case Example
 * @snippet test_io.cpp IoRead16_TC_06_006_007
 */
// [IoRead16_TC_06_006_007]
TEST_F(OsalApiStateTest, IoRead16_TC_06_006_007)
{
    uint16_t read_data16;
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;
    uintptr_t offset = TEST_READ16_REG_OFFSET_AC;

    // Test06_006_007
    // @brief: abnormal case: state transition error with IO Read
    EXPECT_EQ(R_OSAL_IoRead16(device_handle, offset, &read_data16), OSAL_RETURN_STATE) << "Test06_006_007 failed";
}
// [IoRead16_TC_06_006_007]

/**
 * @page IO_TEST IO API Test
 * @section OSALINTTESTS_IoRead16_TC_06_006_008 OSALINTTESTS.IoRead16_TC_06_006_008
 * @subsection IoRead16_TC_06_006_008_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection IoRead16_TC_06_006_008_tci Test Case ID
 * IoRead_S1_N_TC_06_006_008
 * @subsection IoRead16_TC_06_006_008_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203, \#199211
 * @subsection IoRead16_TC_06_006_008_pp Prerequisite/Precondition
 * -# OSAL is initialized.<br>
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection IoRead16_TC_06_006_008_tcs Test Case Summary
 * @subsubsection IoRead16_TC_06_006_008_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Setup required memory for interrupt generation.
 * - Register callack function and enable interrupt.
 * - Wait for the interrupt callback function finish.
 * @subsubsection IoRead16_TC_06_006_008_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * @subsubsection IoRead16_TC_06_006_008_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality of the IO APIs which are called in the ISR
 * - Disable and clear interrupt status
 * - Test06_006_008: use R_OSAL_IoRead with 16 bit
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection IoRead16_TC_06_006_008_tpts Test Procedure/Test Steps
 * @subsubsection IoRead16_TC_06_006_008_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Allocate required memory for generate interrupt from test target device and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function by using R_OSAL_InterruptRegisterIsr.<br>
 * -# Enable interrupt by using R_OSAL_InterruptEnableIsr
 * -# Setup test target device specific register to generate the interrupt by calling to setup_hw_register
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection IoRead16_TC_06_006_008_post_tpts Test Procedure/Test Steps of Post-process
 * -# Unregister interrupt by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * @subsubsection IoRead16_TC_06_006_008_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Read data from test target device with offset "TEST_READ16_REG_OFFSET_INT" by using R_OSAL_IoRead16.
 * -# Send Message interrupt of context finished by using R_OSAL_MqSendForTimePeriod
 * @subsection IoRead16_TC_06_006_008_ereb Expected Result/Behavior
 * @subsubsection IoRead16_TC_06_006_008_pre_ereb Expected Result/Behavior of Pre-process
 * -# allocate_setup_data_memory run successfully
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * The value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK
 * -# setup_hw_register run successfully
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection IoRead16_TC_06_006_008_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * @subsubsection IoRead16_TC_06_006_008_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_IoRead16 return OSAL_RETURN_OK.<br>
 * Read data is the same as the expected data.
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * @subsection IoRead16_TC_06_006_008_testcase Test Case Example
 * @snippet test_io.cpp IoRead16_TC_06_006_008
 * @snippet test_io.cpp osal_io_isr_test_TC_06_006_008
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [IoRead16_TC_06_006_008]
TEST_F(OSALINTTESTS, IoRead16_TC_06_006_008)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    st_io_arg_t io_arg;
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
 * Wait for the interrupt callback function finish. */
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_GT(numOfChannel, 0) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_io_isr_test_TC_06_006_008, (void *)&arg), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK) << "pre-process failed";
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory*/
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_io_isr_test_TC_06_006_008), OSAL_RETURN_OK) << "post-process failed";
    deallocate_data_memory(hndl_mmngr, buffer_obj);
}
// [IoRead16_TC_06_006_008]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoRead32_TC_06_007_001 IOApiTest.IoRead32_TC_06_007_001
 * @subsection IoRead32_TC_06_007_001_tsi Test Suite ID
 * IOApiTest
 * @subsection IoRead32_TC_06_007_001_tci Test Case ID
 * IoRead_S1_N_TC_06_007_001
 * @subsection IoRead32_TC_06_007_001_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203, \#199211
 * @subsection IoRead32_TC_06_007_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - "device_list" contain the device list of each "device_type" in "devices".
 * @subsection IoRead32_TC_06_007_001_tcs Test Case Summary
 * This test shall verify the functionality of the IoRead APIs of OSAL on normal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_007_001: use R_OSAL_IoRead32 to read data from register of test target device with offset "TEST_READ32_REG_OFFSET_NC".
 * - post-process: close test target device
 * @subsection IoRead32_TC_06_007_001_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data from test target device with offset "TEST_READ32_REG_OFFSET_NC" by using R_OSAL_IoRead32.
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoRead32_TC_06_007_001_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead32 return OSAL_RETURN_OK.<br>
 * Read data is the same as the expected data.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoRead32_TC_06_007_001_testcase Test Case Example
 * @snippet test_io.cpp IoRead32_TC_06_007_001
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoRead32_TC_06_007_001]
TEST_F(IOApiTest, IoRead32_TC_06_007_001)
{
    uint32_t read_data32;
    uintptr_t offset = TEST_READ32_REG_OFFSET_NC;
    uint32_t expected_data = TEST_READ32_VAL_NC;

    for (auto it = device_list.begin(); it != device_list.end(); it++)
    {
        if (!strcmp((*it).c_str(), TEST_IO_DEVICE_1))
        {
            // pre-proces: open test target device, turn ON its power and clock
            EXPECT_EQ(R_OSAL_IoDeviceOpen((*it).c_str(), &device_handle), OSAL_RETURN_OK) << "pre-process failed";
            EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
            EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

            // Test06_007_001
            // @brief: normal case: read register value of test target device with aligned offset.
            EXPECT_EQ(R_OSAL_IoRead32(device_handle, offset, &read_data32), OSAL_RETURN_OK) << "Test06_007_001 failed";
            EXPECT_EQ(read_data32, expected_data) << "Test06_007_001 failed";

            // post-process: close test target device
            EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
            EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
            device_handle = OSAL_DEVICE_HANDLE_INVALID;
        }
    }
}
// [IoRead32_TC_06_007_001]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoRead32_TC_06_007_002 IOApiTest.IoRead32_TC_06_007_002
 * @subsection IoRead32_TC_06_007_002_tsi Test Suite ID
 * IOApiTest
 * @subsection IoRead32_TC_06_007_002_tci Test Case ID
 * IoRead_S0_A_TC_06_007_002
 * @subsection IoRead32_TC_06_007_002_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203
 * @subsection IoRead32_TC_06_007_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoRead32_TC_06_007_002_tcs Test Case Summary
 * This test shall verify the functionality of the IoRead APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_007_002: use R_OSAL_IoRead32 to read register value from test target device with un-aligned offset.
 * - post-process: close test target device
 * @subsection IoRead32_TC_06_007_002_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data from test target device with offset "TEST_READ32_OFFSET_UNALIGNED" by using R_OSAL_IoRead32.
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoRead32_TC_06_007_002_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead32 return OSAL_RETURN_PAR
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoRead32_TC_06_007_002_testcase Test Case Example
 * @snippet test_io.cpp IoRead32_TC_06_007_002
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoRead32_TC_06_007_002]
TEST_F(IOApiTest, IoRead32_TC_06_007_002)
{
    uintptr_t offset;
    uint32_t read_data32;

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: read register value of test target device with un-aligned offset.
    // Test06_007_002
    offset = TEST_READ32_OFFSET_UNALIGNED;
    EXPECT_EQ(R_OSAL_IoRead32(device_handle, offset, &read_data32), OSAL_RETURN_PAR) << "Test06_007_002 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoRead32_TC_06_007_002]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoRead32_TC_06_007_003 IOApiTest.IoRead32_TC_06_007_003
 * @subsection IoRead32_TC_06_007_003_tsi Test Suite ID
 * IOApiTest
 * @subsection IoRead32_TC_06_007_003_tci Test Case ID
 * IoRead_S0_A_TC_06_007_003
 * @subsection IoRead32_TC_06_007_003_rr Related Requirement(s)
 * \#199151, \#199199, \#199200, \#199201, \#199202, \#199203
 * @subsection IoRead32_TC_06_007_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoRead32_TC_06_007_003_tcs Test Case Summary
 * This test shall verify the functionality of the IoRead APIs of OSAL on abnormal condition:
 * - Test06_007_003: use R_OSAL_IoRead32 to read register value with device handle is NULL
 * @subsection IoRead32_TC_06_007_003_tpts Test Procedure/Test Steps
 * -# Read data with offset "TEST_READ32_REG_OFFSET_AC" by using R_OSAL_IoRead32.
 * @subsection IoRead32_TC_06_007_003_ereb Expected Result/Behavior
 * -# R_OSAL_IoRead32 return OSAL_RETURN_HANDLE
 * @subsection IoRead32_TC_06_007_003_testcase Test Case Example
 * @snippet test_io.cpp IoRead32_TC_06_007_003
 */
// [IoRead32_TC_06_007_003]
TEST_F(IOApiTest, IoRead32_TC_06_007_003)
{
    uintptr_t offset;
    uint32_t read_data32;

    // @brief: abnormal case: read register value with device handle is NULL
    // Test06_007_003
    offset = TEST_READ32_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoRead32(OSAL_DEVICE_HANDLE_INVALID, offset, &read_data32), OSAL_RETURN_HANDLE) << "Test06_007_003 failed";
}
// [IoRead32_TC_06_007_003]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoRead32_TC_06_007_004 IOApiTest.IoRead32_TC_06_007_004
 * @subsection IoRead32_TC_06_007_004_tsi Test Suite ID
 * IOApiTest
 * @subsection IoRead32_TC_06_007_004_tci Test Case ID
 * IoRead_S0_BA_TC_06_007_004
 * @subsection IoRead32_TC_06_007_004_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203
 * @subsection IoRead32_TC_06_007_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoRead32_TC_06_007_004_tcs Test Case Summary
 * This test shall verify the functionality of the IoRead APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_007_004: use R_OSAL_IoRead32 read register value of test target device with offset which is out-of-register-range
 * - post-process: close test target device
 * @subsection IoRead32_TC_06_007_004_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data with offset "TEST_OFFSET_OUT_RANGE" by using R_OSAL_IoRead32.
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoRead32_TC_06_007_004_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead32 return OSAL_RETURN_PAR
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoRead32_TC_06_007_004_testcase Test Case Example
 * @snippet test_io.cpp IoRead32_TC_06_007_004
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoRead32_TC_06_007_004]
TEST_F(IOApiTest, IoRead32_TC_06_007_004)
{
    uintptr_t offset;
    uint32_t read_data32;

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: read register value with offset which is out-of-register-range
    // Test06_007_004
    offset = TEST_OFFSET_OUT_RANGE;
    EXPECT_EQ(R_OSAL_IoRead32(device_handle, offset, &read_data32), OSAL_RETURN_PAR) << "Test06_007_004 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoRead32_TC_06_007_004]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoRead32_TC_06_007_005 IOApiTest.IoRead32_TC_06_007_005
 * @subsection IoRead32_TC_06_007_005_tsi Test Suite ID
 * IOApiTest
 * @subsection IoRead32_TC_06_007_005_tci Test Case ID
 * IoRead_S0_A_TC_06_007_005
 * @subsection IoRead32_TC_06_007_005_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203
 * @subsection IoRead32_TC_06_007_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoRead32_TC_06_007_005_tcs Test Case Summary
 * This test shall verify the functionality of the IoRead APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_007_005: use R_OSAL_IoRead32 read register value of test target device with p_data is NULL
 * - post-process: close test target device
 * @subsection IoRead32_TC_06_007_005_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data with offset "TEST_READ32_REG_OFFSET_AC" by using R_OSAL_IoRead32.
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoRead32_TC_06_007_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead32 return OSAL_RETURN_PAR
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoRead32_TC_06_007_005_testcase Test Case Example
 * @snippet test_io.cpp IoRead32_TC_06_007_005
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoRead32_TC_06_007_005]
TEST_F(IOApiTest, IoRead32_TC_06_007_005)
{
    uintptr_t offset;
    void* p_data = NULL;

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: read register value with p_data is NULL
    // Test06_007_005
    offset = TEST_READ32_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoRead32(device_handle, offset, (uint32_t*)p_data), OSAL_RETURN_PAR) << "Test06_007_005 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoRead32_TC_06_007_005]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoRead32_TC_06_007_006 IOApiTest.IoRead32_TC_06_007_006
 * @subsection IoRead32_TC_06_007_006_tsi Test Suite ID
 * IOApiTest
 * @subsection IoRead32_TC_06_007_006_tci Test Case ID
 * IoRead_S2_A_TC_06_007_006
 * @subsection IoRead32_TC_06_007_006_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203
 * @subsection IoRead32_TC_06_007_006_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoRead32_TC_06_007_006_tcs Test Case Summary
 * This test shall verify the functionality of the IoRead APIs of OSAL on abnormal condition:
 * - Test06_007_006: use R_OSAL_IoRead32 read register value with closed device handle
 * @subsection IoRead32_TC_06_007_006_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * -# Read data with offset "TEST_READ32_REG_OFFSET_AC" by using R_OSAL_IoRead32.
 * @subsection IoRead32_TC_06_007_006_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_IoRead32 return OSAL_RETURN_HANDLE
 * @subsection IoRead32_TC_06_007_006_testcase Test Case Example
 * @snippet test_io.cpp IoRead32_TC_06_007_006
 */
// [IoRead32_TC_06_007_006]
TEST_F(IOApiTest, IoRead32_TC_06_007_006)
{
    uintptr_t offset;
    uint32_t read_data32;

    // @brief: abnormal case: read register value with closed device handle
    // Test06_007_006
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "Test06_007_006 failed";
    EXPECT_NE(device_handle, NULL_PTR) << "Test06_007_006 failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "Test06_007_006 failed";

    offset = TEST_READ32_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoRead32(device_handle, offset, &read_data32), OSAL_RETURN_HANDLE) << "Test06_007_006 failed";
}
// [IoRead32_TC_06_007_006]

/**
 * @page IO_TEST IO API Test
 * @section OsalApiStateTest_IoRead32_TC_06_007_007 OsalApiStateTest.IoRead32_TC_06_007_007
 * @subsection IoRead32_TC_06_007_007_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection IoRead32_TC_06_007_007_tci Test Case ID
 * IoRead_S2_A_TC_06_007_007
 * @subsection IoRead32_TC_06_007_007_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203
 * @subsection IoRead32_TC_06_007_007_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize has already been called
 * @subsection IoRead32_TC_06_007_007_tcs Test Case Summary
 * This test shall verify the state transition of the R_OSAL_IoRead32 function:
 * - Test06_007_007: confirm state transition error with R_OSAL_IoRead32
 * @subsection IoRead32_TC_06_007_007_tpts Test Procedure/Test Steps
 * -# Confirm the state transition of R_OSAL_IoRead32
 * @subsection IoRead32_TC_06_007_007_ereb Expected Result/Behavior
 * -# R_OSAL_IoRead32 return OSAL_RETURN_STATE
 * @subsection IoRead32_TC_06_007_007_testcase Test Case Example
 * @snippet test_io.cpp IoRead32_TC_06_007_007
 */
// [IoRead32_TC_06_007_007]
TEST_F(OsalApiStateTest, IoRead32_TC_06_007_007)
{
    uint32_t read_data32;
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;
    uintptr_t offset = TEST_READ32_REG_OFFSET_AC;

    // Test06_007_007
    // @brief: abnormal case: state transition error with IO Read
    EXPECT_EQ(R_OSAL_IoRead32(device_handle, offset, &read_data32), OSAL_RETURN_STATE) << "Test06_007_007 failed";
}
// [IoRead32_TC_06_007_007]

/**
 * @page IO_TEST IO API Test
 * @section OSALINTTESTS_IoRead32_TC_06_007_008 OSALINTTESTS.IoRead32_TC_06_007_008
 * @subsection IoRead32_TC_06_007_008_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection IoRead32_TC_06_007_008_tci Test Case ID
 * IoRead_S1_N_TC_06_007_008
 * @subsection IoRead32_TC_06_007_008_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203, \#199211
 * @subsection IoRead32_TC_06_007_008_pp Prerequisite/Precondition
 * -# OSAL is initialized.<br>
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection IoRead32_TC_06_007_008_tcs Test Case Summary
 * @subsubsection IoRead32_TC_06_007_008_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Setup required memory for interrupt generation.
 * - Register callack function and enable interrupt.
 * - Wait for the interrupt callback function finish.
 * @subsubsection IoRead32_TC_06_007_008_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * @subsubsection IoRead32_TC_06_007_008_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality of the IO APIs which are called in the ISR
 * - Disable and clear interrupt status
 * - Test06_007_008 : Read data using R_OSAL_IoRead32
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection IoRead32_TC_06_007_008_tpts Test Procedure/Test Steps
 * @subsubsection IoRead32_TC_06_007_008_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Allocate required memory for generate interrupt from test target device and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function by using R_OSAL_InterruptRegisterIsr.<br>
 * -# Enable interrupt by using R_OSAL_InterruptEnableIsr
 * -# Setup test target device specific register to generate the interrupt by calling to setup_hw_register
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection IoRead32_TC_06_007_008_post_tpts Test Procedure/Test Steps of Post-process
 * -# Unregister interrupt by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * @subsubsection IoRead32_TC_06_007_008_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Read data from test target device with offset "TEST_READ32_REG_OFFSET_INT" by using R_OSAL_IoRead32.
 * -# Send Message interrupt of context finished by using R_OSAL_MqSendForTimePeriod
 * @subsection IoRead32_TC_06_007_008_ereb Expected Result/Behavior
 * @subsubsection IoRead32_TC_06_007_008_pre_ereb Expected Result/Behavior of Pre-process
 * -# allocate_setup_data_memory run successfully
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * The value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK
 * -# setup_hw_register run successfully
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection IoRead32_TC_06_007_008_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * @subsubsection IoRead32_TC_06_007_008_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_IoRead32 return OSAL_RETURN_OK.<br>
 * Read data is the same as the expected data.
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * @subsection IoRead32_TC_06_007_008_testcase Test Case Example
 * @snippet test_io.cpp IoRead32_TC_06_007_008
 * @snippet test_io.cpp osal_io_isr_test_TC_06_007_008
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [IoRead32_TC_06_007_008]
TEST_F(OSALINTTESTS, IoRead32_TC_06_007_008)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    st_io_arg_t io_arg;
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
 * Wait for the interrupt callback function finish. */
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_GT(numOfChannel, 0) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_io_isr_test_TC_06_007_008, (void *)&arg), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK) << "pre-process failed";
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory*/
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_io_isr_test_TC_06_007_008), OSAL_RETURN_OK) << "post-process failed";
    deallocate_data_memory(hndl_mmngr, buffer_obj);
}
// [IoRead32_TC_06_007_008]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoRead64_TC_06_008_001 IOApiTest.IoRead64_TC_06_008_001
 * @subsection IoRead64_TC_06_008_001_tsi Test Suite ID
 * IOApiTest
 * @subsection IoRead64_TC_06_008_001_tci Test Case ID
 * IoRead_S1_N_TC_06_008_001
 * @subsection IoRead64_TC_06_008_001_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203, \#199211
 * @subsection IoRead64_TC_06_008_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - "device_list" contain the device list of each "device_type" in "devices".
 * @subsection IoRead64_TC_06_008_001_tcs Test Case Summary
 * This test shall verify the functionality of the IoRead APIs of OSAL on normal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_008_001: use R_OSAL_IoRead64 to read data from register of test target device with offset "TEST_READ64_REG_OFFSET_NC".
 * - post-process: close test target device
 * @subsection IoRead64_TC_06_008_001_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data from test target device with offset "TEST_READ64_REG_OFFSET_NC" by using R_OSAL_IoRead64.
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoRead64_TC_06_008_001_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead64 return OSAL_RETURN_OK.<br>
 * Read data is the same as the expected data.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoRead64_TC_06_008_001_testcase Test Case Example
 * @snippet test_io.cpp IoRead64_TC_06_008_001
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoRead64_TC_06_008_001]
TEST_F(IOApiTest, IoRead64_TC_06_008_001)
{
    uint64_t read_data64;
    uintptr_t offset = TEST_READ64_REG_OFFSET_NC;
    uint64_t expected_data = TEST_READ64_VAL_NC;

    for (auto it = device_list.begin(); it != device_list.end(); it++)
    {
        if (!strcmp((*it).c_str(), TEST_IO_DEVICE_1))
        {
            // pre-proces: open test target device, turn ON its power and clock
            EXPECT_EQ(R_OSAL_IoDeviceOpen((*it).c_str(), &device_handle), OSAL_RETURN_OK) << "pre-process failed";
            EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
            EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

            // Test06_008_001
            // @brief: normal case: read register value of test target device with aligned offset.
            EXPECT_EQ(R_OSAL_IoRead64(device_handle, offset, &read_data64), OSAL_RETURN_OK) << "Test06_008_001 failed";
            read_data64 = read_data64 & 0x00000000FFFFFFFF;
            EXPECT_EQ(read_data64, expected_data) << "Test06_008_001 failed";

            // post-process: close test target device
            EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
            EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
            device_handle = OSAL_DEVICE_HANDLE_INVALID;
        }
    }
}
// [IoRead64_TC_06_008_001]
/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoRead64_TC_06_008_002 IOApiTest.IoRead64_TC_06_008_002
 * @subsection IoRead64_TC_06_008_002_tsi Test Suite ID
 * IOApiTest
 * @subsection IoRead64_TC_06_008_002_tci Test Case ID
 * IoRead_S0_A_TC_06_008_002
 * @subsection IoRead64_TC_06_008_002_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203
 * @subsection IoRead64_TC_06_008_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoRead64_TC_06_008_002_tcs Test Case Summary
 * This test shall verify the functionality of the IoRead APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_008_002: use R_OSAL_IoRead64 to read register value from test target device with un-aligned offset.
 * - post-process: close test target device
 * @subsection IoRead64_TC_06_008_002_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data from test target device with offset "TEST_READ64_OFFSET_UNALIGNED" by using R_OSAL_IoRead64.
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoRead64_TC_06_008_002_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead64 return OSAL_RETURN_PAR
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoRead64_TC_06_008_002_testcase Test Case Example
 * @snippet test_io.cpp IoRead64_TC_06_008_002
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoRead64_TC_06_008_002]
TEST_F(IOApiTest, IoRead64_TC_06_008_002)
{
    uintptr_t offset;
    uint64_t read_data64;

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: read register value of test target device with un-aligned offset.
    // Test06_008_002
    offset = TEST_READ64_OFFSET_UNALIGNED;
    EXPECT_EQ(R_OSAL_IoRead64(device_handle, offset, &read_data64), OSAL_RETURN_PAR) << "Test06_008_002 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoRead64_TC_06_008_002]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoRead64_TC_06_008_003 IOApiTest.IoRead64_TC_06_008_003
 * @subsection IoRead64_TC_06_008_003_tsi Test Suite ID
 * IOApiTest
 * @subsection IoRead64_TC_06_008_003_tci Test Case ID
 * IoRead_S0_A_TC_06_008_003
 * @subsection IoRead64_TC_06_008_003_rr Related Requirement(s)
 * \#199151, \#199199, \#199200, \#199201, \#199202, \#199203
 * @subsection IoRead64_TC_06_008_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoRead64_TC_06_008_003_tcs Test Case Summary
 * This test shall verify the functionality of the IoRead APIs of OSAL on abnormal condition:
 * - Test06_008_003: use R_OSAL_IoRead64 to read register value with device handle is NULL
 * @subsection IoRead64_TC_06_008_003_tpts Test Procedure/Test Steps
 * -# Read data with offset "TEST_READ64_REG_OFFSET_AC" by using R_OSAL_IoRead64.
 * @subsection IoRead64_TC_06_008_003_ereb Expected Result/Behavior
 * -# R_OSAL_IoRead64 return OSAL_RETURN_HANDLE
 * @subsection IoRead64_TC_06_008_003_testcase Test Case Example
 * @snippet test_io.cpp IoRead64_TC_06_008_003
 */
// [IoRead64_TC_06_008_003]
TEST_F(IOApiTest, IoRead64_TC_06_008_003)
{
    uintptr_t offset;
    uint64_t read_data64;

    // @brief: abnormal case: read register value with device handle is NULL
    // Test06_008_003
    offset = TEST_READ64_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoRead64(OSAL_DEVICE_HANDLE_INVALID, offset, &read_data64), OSAL_RETURN_HANDLE) << "Test06_008_003 failed";
}
// [IoRead64_TC_06_008_003]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoRead64_TC_06_008_004 IOApiTest.IoRead64_TC_06_008_004
 * @subsection IoRead64_TC_06_008_004_tsi Test Suite ID
 * IOApiTest
 * @subsection IoRead64_TC_06_008_004_tci Test Case ID
 * IoRead_S0_BA_TC_06_008_004
 * @subsection IoRead64_TC_06_008_004_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203
 * @subsection IoRead64_TC_06_008_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoRead64_TC_06_008_004_tcs Test Case Summary
 * This test shall verify the functionality of the IoRead APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_008_004: use R_OSAL_IoRead64 read register value of test target device with offset which is out-of-register-range
 * - post-process: close test target device
 * @subsection IoRead64_TC_06_008_004_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data with offset "TEST_OFFSET_OUT_RANGE" by using R_OSAL_IoRead64.
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoRead64_TC_06_008_004_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead64 return OSAL_RETURN_PAR
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoRead64_TC_06_008_004_testcase Test Case Example
 * @snippet test_io.cpp IoRead64_TC_06_008_004
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoRead64_TC_06_008_004]
TEST_F(IOApiTest, IoRead64_TC_06_008_004)
{
    uintptr_t offset;
    uint64_t read_data64;

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: read register value with offset which is out-of-register-range
    // Test06_008_004
    offset = TEST_OFFSET_OUT_RANGE;
    EXPECT_EQ(R_OSAL_IoRead64(device_handle, offset, &read_data64), OSAL_RETURN_PAR) << "Test06_008_004 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoRead64_TC_06_008_004]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoRead64_TC_06_008_005 IOApiTest.IoRead64_TC_06_008_005
 * @subsection IoRead64_TC_06_008_005_tsi Test Suite ID
 * IOApiTest
 * @subsection IoRead64_TC_06_008_005_tci Test Case ID
 * IoRead_S0_A_TC_06_008_005
 * @subsection IoRead64_TC_06_008_005_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203
 * @subsection IoRead64_TC_06_008_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoRead64_TC_06_008_005_tcs Test Case Summary
 * This test shall verify the functionality of the IoRead APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_008_005: use R_OSAL_IoRead64 read register value of test target device with p_data is NULL
 * - post-process: close test target device
 * @subsection IoRead64_TC_06_008_005_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data with offset "TEST_READ64_REG_OFFSET_AC" by using R_OSAL_IoRead64.
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoRead64_TC_06_008_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead64 return OSAL_RETURN_PAR
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoRead64_TC_06_008_005_testcase Test Case Example
 * @snippet test_io.cpp IoRead64_TC_06_008_005
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoRead64_TC_06_008_005]
TEST_F(IOApiTest, IoRead64_TC_06_008_005)
{
    uintptr_t offset;
    void* p_data = NULL;

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: read register value with p_data is NULL
    // Test06_008_005
    offset = TEST_READ64_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoRead64(device_handle, offset, (uint64_t*)p_data), OSAL_RETURN_PAR) << "Test06_008_005 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoRead64_TC_06_008_005]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoRead64_TC_06_008_006 IOApiTest.IoRead64_TC_06_008_006
 * @subsection IoRead64_TC_06_008_006_tsi Test Suite ID
 * IOApiTest
 * @subsection IoRead64_TC_06_008_006_tci Test Case ID
 * IoRead_S2_A_TC_06_008_006
 * @subsection IoRead64_TC_06_008_006_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203
 * @subsection IoRead64_TC_06_008_006_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoRead64_TC_06_008_006_tcs Test Case Summary
 * This test shall verify the functionality of the IoRead APIs of OSAL on abnormal condition:
 * - Test06_008_006: use R_OSAL_IoRead64 read register value with closed device handle
 * @subsection IoRead64_TC_06_008_006_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * -# Read data with offset "TEST_READ64_REG_OFFSET_AC" by using R_OSAL_IoRead64.
 * @subsection IoRead64_TC_06_008_006_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_IoRead64 return OSAL_RETURN_HANDLE
 * @subsection IoRead64_TC_06_008_006_testcase Test Case Example
 * @snippet test_io.cpp IoRead64_TC_06_008_006
 */
// [IoRead64_TC_06_008_006]
TEST_F(IOApiTest, IoRead64_TC_06_008_006)
{
    uintptr_t offset;
    uint64_t read_data64;

    // @brief: abnormal case: read register value with closed device handle
    // Test06_008_006
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "Test06_008_006 failed";
    EXPECT_NE(device_handle, NULL_PTR) << "Test06_008_006 failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "Test06_008_006 failed";

    offset = TEST_READ64_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoRead64(device_handle, offset, &read_data64), OSAL_RETURN_HANDLE) << "Test06_008_006 failed";
}
// [IoRead64_TC_06_008_006]

/**
 * @page IO_TEST IO API Test
 * @section OsalApiStateTest_IoRead64_TC_06_008_007 OsalApiStateTest.IoRead64_TC_06_008_007
 * @subsection IoRead64_TC_06_008_007_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection IoRead64_TC_06_008_007_tci Test Case ID
 * IoRead_S2_A_TC_06_008_007
 * @subsection IoRead64_TC_06_008_007_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203
 * @subsection IoRead64_TC_06_008_007_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize has already been called
 * @subsection IoRead64_TC_06_008_007_tcs Test Case Summary
 * This test shall verify the state transition of the R_OSAL_IoRead64 function:
 * - Test06_008_007: confirm state transition error with R_OSAL_IoRead64
 * @subsection IoRead64_TC_06_008_007_tpts Test Procedure/Test Steps
 * -# Confirm the state transition of R_OSAL_IoRead64
 * @subsection IoRead64_TC_06_008_007_ereb Expected Result/Behavior
 * -# R_OSAL_IoRead64 return OSAL_RETURN_STATE
 * @subsection IoRead64_TC_06_008_007_testcase Test Case Example
 * @snippet test_io.cpp IoRead64_TC_06_008_007
 */
// [IoRead64_TC_06_008_007]
TEST_F(OsalApiStateTest, IoRead64_TC_06_008_007)
{
    uint64_t read_data64;
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;
    uintptr_t offset = TEST_READ64_REG_OFFSET_AC;

    // Test06_008_007
    // @brief: abnormal case: state transition error with IO Read
    EXPECT_EQ(R_OSAL_IoRead64(device_handle, offset, &read_data64), OSAL_RETURN_STATE) << "Test06_008_007 failed";
}
// [IoRead64_TC_06_008_007]

/**
 * @page IO_TEST IO API Test
 * @section OSALINTTESTS_IoRead64_TC_06_008_008 OSALINTTESTS.IoRead64_TC_06_008_008
 * @subsection IoRead64_TC_06_008_008_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection IoRead64_TC_06_008_008_tci Test Case ID
 * IoRead_S1_N_TC_06_008_008
 * @subsection IoRead64_TC_06_008_008_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203, \#199211
 * @subsection IoRead64_TC_06_008_008_pp Prerequisite/Precondition
 * -# OSAL is initialized.<br>
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection IoRead64_TC_06_008_008_tcs Test Case Summary
 * @subsubsection IoRead64_TC_06_008_008_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Setup required memory for interrupt generation.
 * - Register callack function and enable interrupt.
 * - Wait for the interrupt callback function finish.
 * @subsubsection IoRead64_TC_06_008_008_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * @subsubsection IoRead64_TC_06_008_008_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality of the IO APIs which are called in the ISR
 * - Disable and clear interrupt status
 * - Test06_008_008: use R_OSAL_IoRead with 64 bit
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection IoRead64_TC_06_008_008_tpts Test Procedure/Test Steps
 * @subsubsection IoRead64_TC_06_008_008_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Allocate required memory for generate interrupt from test target device and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function by using R_OSAL_InterruptRegisterIsr.<br>
 * -# Enable interrupt by using R_OSAL_InterruptEnableIsr
 * -# Setup test target device specific register to generate the interrupt by calling to setup_hw_register
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection IoRead64_TC_06_008_008_post_tpts Test Procedure/Test Steps of Post-process
 * -# Unregister interrupt by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * @subsubsection IoRead64_TC_06_008_008_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Read data from test target device with offset "TEST_READ64_REG_OFFSET_INT" by using R_OSAL_IoRead64.
 * -# Send Message interrupt of context finished by using R_OSAL_MqSendForTimePeriod
 * @subsection IoRead64_TC_06_008_008_ereb Expected Result/Behavior
 * @subsubsection IoRead64_TC_06_008_008_pre_ereb Expected Result/Behavior of Pre-process
 * -# allocate_setup_data_memory run successfully
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * The value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK
 * -# setup_hw_register run successfully
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection IoRead64_TC_06_008_008_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * @subsubsection IoRead64_TC_06_008_008_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_IoRead64 return OSAL_RETURN_OK.<br>
 * Read data is the same as the expected data.
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * @subsection IoRead64_TC_06_008_008_testcase Test Case Example
 * @snippet test_io.cpp IoRead64_TC_06_008_008
 * @snippet test_io.cpp osal_io_isr_test_TC_06_008_008
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [IoRead64_TC_06_008_008]
TEST_F(OSALINTTESTS, IoRead64_TC_06_008_008)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    st_io_arg_t io_arg;
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
 * Wait for the interrupt callback function finish. */
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_GT(numOfChannel, 0) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_io_isr_test_TC_06_008_008, (void *)&arg), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK) << "pre-process failed";
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory*/
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_io_isr_test_TC_06_008_008), OSAL_RETURN_OK) << "post-process failed";
    deallocate_data_memory(hndl_mmngr, buffer_obj);
}
// [IoRead64_TC_06_008_008]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoWrite8_TC_06_013_001 IOApiTest.IoWrite8_TC_06_013_001
 * @subsection IoWrite8_TC_06_013_001_tsi Test Suite ID
 * IOApiTest
 * @subsection IoWrite8_TC_06_013_001_tci Test Case ID
 * IoWrite_S1_N_TC_06_013_001
 * @subsection IoWrite8_TC_06_013_001_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203, \#199211
 * @subsection IoWrite8_TC_06_013_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - "device_list" contain the device list of each "device_type" in "devices".
 * @subsection IoWrite8_TC_06_013_001_tcs Test Case Summary
 * This test shall verify the functionality of the IoWrite APIs of OSAL on normal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_013_001: use R_OSAL_IoWrite8 to write data to register of test target device with offset "TEST_WRITE8_REG_OFFSET_NC".
 * - post-process: write back initialization value, turn OFF clock and close test target device
 * @subsection IoWrite8_TC_06_013_001_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read initialization data from test target device with offset "TEST_WRITE8_REG_OFFSET_NC" by using R_OSAL_IoRead8.
 * -# Write data to test target device with offset "TEST_WRITE8_REG_OFFSET_NC" by using R_OSAL_IoWrite8.
 * -# Read data from test target device with offset "TEST_WRITE8_REG_OFFSET_NC" by using R_OSAL_IoRead8.
 * -# Write back initialization data to test target device with offset "TEST_WRITE8_REG_OFFSET_NC" by using R_OSAL_IoWrite8.
 * -# Set the clock of test target device status to "OFF":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoWrite8_TC_06_013_001_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead8 return OSAL_RETURN_OK.
 * -# R_OSAL_IoWrite8 return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead8 return OSAL_RETURN_OK.<br>
 * Read data is the same as the write data.
 * -# R_OSAL_IoWrite8 return OSAL_RETURN_OK.
 * -# Set clock to "OFF" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoWrite8_TC_06_013_001_testcase Test Case Example
 * @snippet test_io.cpp IoWrite8_TC_06_013_001
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoWrite8_TC_06_013_001]
TEST_F(IOApiTest, IoWrite8_TC_06_013_001)
{
    uint8_t init_data8;
    uint8_t read_data8;
    uint8_t write_data8 = TEST_WRITE8_VAL_NC;
    uintptr_t offset = TEST_WRITE8_REG_OFFSET_NC;

    for (auto it = device_list.begin(); it != device_list.end(); it++)
    {
        if (!strcmp((*it).c_str(), TEST_IO_DEVICE_1))
        {
            // pre-proces: open test target device, turn ON its power and clock
            EXPECT_EQ(R_OSAL_IoDeviceOpen((*it).c_str(), &device_handle), OSAL_RETURN_OK) << "pre-process failed";
            EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
            EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";
            EXPECT_EQ(R_OSAL_IoRead8(device_handle, offset, &init_data8), OSAL_RETURN_OK) << "pre-process failed";

            // Test06_013_001
            // @brief: normal case: write data to register of test target device with aligned offset.
            EXPECT_EQ(R_OSAL_IoWrite8(device_handle, offset, write_data8), OSAL_RETURN_OK) << "Test06_013_001 failed";
            EXPECT_EQ(R_OSAL_IoRead8(device_handle, offset, &read_data8), OSAL_RETURN_OK) << "Test06_013_001 failed";
            EXPECT_EQ(read_data8, write_data8) << "Test06_013_001 failed";

            // post-process: write back initialization value, turn OFF clock and close test target device
            EXPECT_EQ(R_OSAL_IoWrite8(device_handle, offset, init_data8), OSAL_RETURN_OK) << "post-process failed";
            EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
            EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
            device_handle = OSAL_DEVICE_HANDLE_INVALID;
        }
    }
}
// [IoWrite8_TC_06_013_001]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoWrite8_TC_06_013_003 IOApiTest.IoWrite8_TC_06_013_003
 * @subsection IoWrite8_TC_06_013_003_tsi Test Suite ID
 * IOApiTest
 * @subsection IoWrite8_TC_06_013_003_tci Test Case ID
 * IoWrite_S0_A_TC_06_013_003
 * @subsection IoWrite8_TC_06_013_003_rr Related Requirement(s)
 * \#199151, \#199199, \#199200, \#199201, \#199202, \#199203
 * @subsection IoWrite8_TC_06_013_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoWrite8_TC_06_013_003_tcs Test Case Summary
 * This test shall verify the functionality of the Write APIs of OSAL on abnormal condition:
 * - Test06_013_003: use R_OSAL_IoWrite8 to write register value with device handle is NULL
 * @subsection IoWrite8_TC_06_013_003_tpts Test Procedure/Test Steps
 * -# Write data with offset "TEST_WRITE8_REG_OFFSET_AC" by using R_OSAL_IoWrite8.
 * @subsection IoWrite8_TC_06_013_003_ereb Expected Result/Behavior
 * -# R_OSAL_IoWrite8 return OSAL_RETURN_HANDLE
 * @subsection IoWrite8_TC_06_013_003_testcase Test Case Example
 * @snippet test_io.cpp IoWrite8_TC_06_013_003
 */
// [IoWrite8_TC_06_013_003]
TEST_F(IOApiTest, IoWrite8_TC_06_013_003)
{
    uintptr_t offset;
    uint8_t write_data8 = TEST_WRITE8_VAL_AC;

    // @brief: abnormal case: write register value with device handle is NULL
    // Test06_013_003
    offset = TEST_WRITE8_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoWrite8(OSAL_DEVICE_HANDLE_INVALID, offset, write_data8), OSAL_RETURN_HANDLE) << "Test06_013_003 failed";
}
// [IoWrite8_TC_06_013_003]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoWrite8_TC_06_013_004 IOApiTest.IoWrite8_TC_06_013_004
 * @subsection IoWrite8_TC_06_013_004_tsi Test Suite ID
 * IOApiTest
 * @subsection IoWrite8_TC_06_013_004_tci Test Case ID
 * IoWrite_S0_BA_TC_06_013_004
 * @subsection IoWrite8_TC_06_013_004_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203
 * @subsection IoWrite8_TC_06_013_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoWrite8_TC_06_013_004_tcs Test Case Summary
 * This test shall verify the functionality of the IoWrite APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_013_004: use R_OSAL_IoWrite8 write data to register of test target device with offset which is out-of-register-range
 * - post-process: close test target device
 * @subsection IoWrite8_TC_06_013_004_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Write data with offset "TEST_OFFSET_OUT_RANGE" by using R_OSAL_IoWrite8.
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoWrite8_TC_06_013_004_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoWrite8 return OSAL_RETURN_PAR
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoWrite8_TC_06_013_004_testcase Test Case Example
 * @snippet test_io.cpp IoWrite8_TC_06_013_004
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoWrite8_TC_06_013_004]
TEST_F(IOApiTest, IoWrite8_TC_06_013_004)
{
    uintptr_t offset;
    uint8_t write_data8 = TEST_WRITE8_VAL_AC;

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: write register value with offset which is out-of-register-range
    // Test06_013_004
    offset = TEST_OFFSET_OUT_RANGE;
    EXPECT_EQ(R_OSAL_IoWrite8(device_handle, offset, write_data8), OSAL_RETURN_PAR) << "Test06_013_004 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoWrite8_TC_06_013_004]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoWrite8_TC_06_013_005 IOApiTest.IoWrite8_TC_06_013_005
 * @subsection IoWrite8_TC_06_013_005_tsi Test Suite ID
 * IOApiTest
 * @subsection IoWrite8_TC_06_013_005_tci Test Case ID
 * IoWrite_S2_A_TC_06_013_005
 * @subsection IoWrite8_TC_06_013_005_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203
 * @subsection IoWrite8_TC_06_013_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoWrite8_TC_06_013_005_tcs Test Case Summary
 * This test shall verify the functionality of the IoWrite APIs of OSAL on abnormal condition:
 * - Test06_013_005: use R_OSAL_IoWrite8 write data to register value of closed device handle
 * @subsection IoWrite8_TC_06_013_005_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * -# Write data with offset "TEST_WRITE8_REG_OFFSET_AC" by using R_OSAL_IoWrite8.
 * @subsection IoWrite8_TC_06_013_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite8 return OSAL_RETURN_HANDLE
 * @subsection IoWrite8_TC_06_013_005_testcase Test Case Example
 * @snippet test_io.cpp IoWrite8_TC_06_013_005
 */
// [IoWrite8_TC_06_013_005]
TEST_F(IOApiTest, IoWrite8_TC_06_013_005)
{
    uintptr_t offset;
    uint8_t write_data8 = TEST_WRITE8_VAL_AC;

    // @brief: abnormal case: write register value with closed device handle
    // Test06_013_005
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "Test06_013_005 failed";
    EXPECT_NE(device_handle, NULL_PTR) << "Test06_013_005 failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "Test06_013_005 failed";

    offset = TEST_WRITE8_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoWrite8(device_handle, offset, write_data8), OSAL_RETURN_HANDLE) << "Test06_013_005 failed";
}
// [IoWrite8_TC_06_013_005]

/**
 * @page IO_TEST IO API Test
 * @section OsalApiStateTest_IoWrite8_TC_06_013_006 OsalApiStateTest.IoWrite8_TC_06_013_006
 * @subsection IoWrite8_TC_06_013_006_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection IoWrite8_TC_06_013_006_tci Test Case ID
 * IoWrite_S2_A_TC_06_013_006
 * @subsection IoWrite8_TC_06_013_006_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203
 * @subsection IoWrite8_TC_06_013_006_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize has already been called
 * @subsection IoWrite8_TC_06_013_006_tcs Test Case Summary
 * This test shall verify the state transition of the R_OSAL_IoWrite8 function:
 * - Test06_013_006: confirm state transition error with R_OSAL_IoWrite8
 * @subsection IoWrite8_TC_06_013_006_tpts Test Procedure/Test Steps
 * -# Confirm the state transition of R_OSAL_IoWrite8
 * @subsection IoWrite8_TC_06_013_006_ereb Expected Result/Behavior
 * -# R_OSAL_IoWrite8return OSAL_RETURN_STATE
 * @subsection IoWrite8_TC_06_013_006_testcase Test Case Example
 * @snippet test_io.cpp IoWrite8_TC_06_013_006
 */
// [IoWrite8_TC_06_013_006]
TEST_F(OsalApiStateTest, IoWrite8_TC_06_013_006)
{
    uint8_t write_data8 = TEST_WRITE8_VAL_AC;
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;
    uintptr_t offset = TEST_WRITE8_REG_OFFSET_AC;

    // Test06_013_006
    // @brief: abnormal case: state transition error with IO Write
    EXPECT_EQ(R_OSAL_IoWrite8(device_handle, offset, write_data8), OSAL_RETURN_STATE) << "Test06_013_006 failed";
}
// [IoWrite8_TC_06_013_006]

/**
 * @page IO_TEST IO API Test
 * @section OSALINTTESTS_IoWrite8_TC_06_013_007 OSALINTTESTS.IoWrite8_TC_06_013_007
 * @subsection IoWrite8_TC_06_013_007_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection IoWrite8_TC_06_013_007_tci Test Case ID
 * IoWrite_S1_N_TC_06_013_007
 * @subsection IoWrite8_TC_06_013_007_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203, \#199211
 * @subsection IoWrite8_TC_06_013_007_pp Prerequisite/Precondition
 * -# OSAL is initialized.<br>
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection IoWrite8_TC_06_013_007_tcs Test Case Summary
 * @subsubsection IoWrite8_TC_06_013_007_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Setup required memory for interrupt generation.
 * - Register callack function and enable interrupt.
 * - Wait for the interrupt callback function finish.
 * @subsubsection IoWrite8_TC_06_013_007_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * @subsubsection IoWrite8_TC_06_013_007_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality of the IO APIs which are called in the ISR
 * - Disable and clear interrupt status
 * - Test06_013_007: Write data to test target device register by using R_OSAL_IoWrite with 8 bit in ISR
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection IoWrite8_TC_06_013_007_tpts Test Procedure/Test Steps
 * @subsubsection IoWrite8_TC_06_013_007_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Allocate required memory for generate interrupt from test target device and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function by using R_OSAL_InterruptRegisterIsr.<br>
 * -# Enable interrupt by using R_OSAL_InterruptEnableIsr
 * -# Setup test target device specific register to generate the interrupt by calling to setup_hw_register
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection IoWrite8_TC_06_013_007_post_tpts Test Procedure/Test Steps of Post-process
 * -# Unregister interrupt by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * @subsubsection IoWrite8_TC_06_013_007_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Read initialization data from test target device with offset "TEST_WRITE8_REG_OFFSET_INT" by using R_OSAL_IoRead8.
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Write data to test target device with offset "TEST_WRITE8_REG_OFFSET_INT" by using R_OSAL_IoWrite8.
 * -# Read data from test target device with offset "TEST_WRITE8_REG_OFFSET_INT" by using R_OSAL_IoRead8.
 * -# Write back initialization data to test target device with offset "TEST_WRITE8_REG_OFFSET_INT" by using R_OSAL_IoWrite8.
 * -# Send Message interrupt of context finished by using R_OSAL_MqSendForTimePeriod
 * @subsection IoWrite8_TC_06_013_007_ereb Expected Result/Behavior
 * @subsubsection IoWrite8_TC_06_013_007_pre_ereb Expected Result/Behavior of Pre-process
 * -# allocate_setup_data_memory run successfully
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * The value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK
 * -# setup_hw_register run successfully
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection IoWrite8_TC_06_013_007_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * @subsubsection IoWrite8_TC_06_013_007_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK
 * -# R_OSAL_IoRead8 return OSAL_RETURN_OK.
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite8 return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead8 return OSAL_RETURN_OK.<br>
 * Read data is the same as the write data.
 * -# R_OSAL_IoWrite8 return OSAL_RETURN_OK.
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * @subsection IoWrite8_TC_06_013_007_testcase Test Case Example
 * @snippet test_io.cpp IoWrite8_TC_06_013_007
 * @snippet test_io.cpp osal_io_isr_test_TC_06_013_007
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [IoWrite8_TC_06_013_007]
TEST_F(OSALINTTESTS, IoWrite8_TC_06_013_007)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    st_io_arg_t io_arg;
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
 * Wait for the interrupt callback function finish. */
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_GT(numOfChannel, 0) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_io_isr_test_TC_06_013_007, (void *)&arg), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK) << "pre-process failed";
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory*/
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_io_isr_test_TC_06_013_007), OSAL_RETURN_OK) << "post-process failed";
    deallocate_data_memory(hndl_mmngr, buffer_obj);
}
// [IoWrite8_TC_06_013_007]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoWrite8_TC_06_013_008 IOApiTest.IoWrite8_TC_06_013_008
 * @subsection IoWrite8_TC_06_013_008_tsi Test Suite ID
 * IOApiTest
 * @subsection IoWrite8_TC_06_013_008_tci Test Case ID
 * IoWrite8_S1_N_TC_06_013_008
 * @subsection IoWrite8_TC_06_013_008_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203, \#199211
 * @subsection IoWrite8_TC_06_013_008_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - The synchronization queue was created
 * @subsection IoWrite8_TC_06_013_008_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoWrite8/R_OSAL_IoRead8 function with the same device handle in multi-threads:
 * - Test06_013_008: Create 2 threads to write and read to register of the same test target device.
 * @subsection IoWrite8_TC_06_013_008_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Turn ON power and clock of the target device by calling to set_clock_on
 * -# Create first thread using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE3
 *          -# task_name is "Read/write 8 bit value from/to device register in multi-threads"
 *          -# func is "io_read_write_8_device_child_thread" function with functionality:
 *              - Get config of synchronization queue by using R_OSAL_MqGetConfig
 *              - Read initialization data from test target device with offset "TEST_WRITE8_REG_OFFSET_NC" by using R_OSAL_IoRead8.
 *              - Write data is "TEST_WRITE8_VAL_NC" to test target device with offset "TEST_WRITE8_REG_OFFSET_NC" by using R_OSAL_IoWrite8.
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *          -# userarg includes device handle and thread id
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2
 *      - Address of first thread handle.
 * -# Receive message from synchronization queue by using R_OSAL_MqReceiveForTimePeriod
 * -# Create second thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE3
 *          -# task_name is "Read/write 8 bit value from/to device register in multi-threads"
 *          -# func is "io_read_write_8_device_child_thread" function with functionality:
 *              - Read data from test target device with offset "TEST_WRITE8_REG_OFFSET_NC" by using R_OSAL_IoRead8.
 *              - Write back initialization data to test target device with offset "TEST_WRITE8_REG_OFFSET_NC" by using R_OSAL_IoWrite8.
 *          -# userarg includes device handle and thread id
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID3
 *      - Address of second thread handle.
 * -# Turn OFF power and clock domain of target device by calling to set_clock_off
 * -# Join first thread by using R_OSAL_ThreadJoin.
 * -# Join second thread by using R_OSAL_ThreadJoin.
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoWrite8_TC_06_013_008_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with first thread.<br>
 * "thread_handle1" is not NULL_PTR.<br>
 * In io_read_write_8_device_child_thread function:
 *  - R_OSAL_MqGetConfig return OSAL_RETURN_OK.
 *  - R_OSAL_IoRead8 return OSAL_RETURN_OK.
 *  - R_OSAL_IoWrite8 return OSAL_RETURN_OK.
 *  - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK.
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK.<br>
 * The value of "receive_msg" need to be equal to "send_msg"
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with second thread.<br>
 * "thread_handle2" is not NULL_PTR.<br>
 * In io_read_write_8_device_child_thread function:
 *  - R_OSAL_IoRead8 return OSAL_RETURN_OK.<br>
 * "read_data8" is equal to "write_data8".
 *  - R_OSAL_IoWrite8 return OSAL_RETURN_OK.
 * -# Set power and clock to "OFF" successfully
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with first thread.<br>
 * Value of "result" is equal to OSAL_RETURN_OK.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with second thread.<br>
 * Value of "result" is equal to OSAL_RETURN_OK.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoWrite8_TC_06_013_008_testcase Test Case Example
 * @snippet test_io.cpp IoWrite8_TC_06_013_008
 * @snippet test_io.cpp io_read_write_8_device_child_thread
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoWrite8_TC_06_013_008]
TEST_F(IOApiTest, IoWrite8_TC_06_013_008)
{

    st_osal_thread_config_t config_thread;
    int64_t                 result          = 0;
    osal_thread_handle_t    thread_handle1  = OSAL_THREAD_HANDLE_INVALID;
    osal_thread_handle_t    thread_handle2  = OSAL_THREAD_HANDLE_INVALID;

    // Test06_013_008
    // @brief: normal case: Read write register 8-bit in multi-threads.
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";

    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "Test06_013_008 failed";

    io_test_multiple_thread_worker_arg user_arg1(&device_handle, TEST_THREAD_ID2);
    user_arg1.send_msg = send_msg;
    user_arg1.mq = sync_mq;
    config_thread.userarg       = &user_arg1;
    config_thread.priority      = OSAL_THREAD_PRIORITY_TYPE3;
    config_thread.task_name     = (char *)"Read/write 8 bit value from/to device register in multi-threads";
    config_thread.func          = io_read_write_8_device_child_thread;
    config_thread.stack_size    = THREAD_STACK_SIZE;
    EXPECT_EQ(R_OSAL_ThreadCreate(&config_thread, TEST_THREAD_ID2, &thread_handle1), OSAL_RETURN_OK) << "Test06_013_008 failed";
    EXPECT_NE(thread_handle1, NULL_PTR) << "Test06_013_008 failed";
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &receive_msg, mq_config.msg_size), OSAL_RETURN_OK) << "Test06_013_008 failed";
    EXPECT_EQ(receive_msg, send_msg) << "Test06_013_008 failed";

    user_arg1.uid              = TEST_THREAD_ID3;
    EXPECT_EQ(R_OSAL_ThreadCreate(&config_thread, TEST_THREAD_ID3, &thread_handle2), OSAL_RETURN_OK) << "Test06_013_008 failed";
    EXPECT_NE(thread_handle2, NULL_PTR) << "Test06_013_008 failed";

    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "Test06_013_008 failed";

    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &result),OSAL_RETURN_OK) << "Test06_013_008 failed";
    EXPECT_EQ(result, (int64_t)OSAL_RETURN_OK) << "Test06_013_008 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle2, &result),OSAL_RETURN_OK) << "Test06_013_008 failed";
    EXPECT_EQ(result, (int64_t)OSAL_RETURN_OK) << "Test06_013_008 failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoWrite8_TC_06_013_008]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoWrite16_TC_06_014_001 IOApiTest.IoWrite16_TC_06_014_001
 * @subsection IoWrite16_TC_06_014_001_tsi Test Suite ID
 * IOApiTest
 * @subsection IoWrite16_TC_06_014_001_tci Test Case ID
 * IoWrite_S1_N_TC_06_014_001
 * @subsection IoWrite16_TC_06_014_001_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203, \#199211
 * @subsection IoWrite16_TC_06_014_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - "device_list" contain the device list of each "device_type" in "devices".
 * @subsection IoWrite16_TC_06_014_001_tcs Test Case Summary
 * This test shall verify the functionality of the IoWrite APIs of OSAL on normal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_014_001: use R_OSAL_IoWrite16 to write data to register of test target device with offset "TEST_WRITE16_REG_OFFSET_NC".
 * - post-process: write back initialization value, turn OFF clock and close test target device
 * @subsection IoWrite16_TC_06_014_001_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read initialization data from test target device with offset "TEST_WRITE16_REG_OFFSET_NC" by using R_OSAL_IoRead16.
 * -# Write data to test target device with offset "TEST_WRITE16_REG_OFFSET_NC" by using R_OSAL_IoWrite16.
 * -# Read data from test target device with offset "TEST_WRITE16_REG_OFFSET_NC" by using R_OSAL_IoRead16.
 * -# Write back initialization data to test target device with offset "TEST_WRITE16_REG_OFFSET_NC" by using R_OSAL_IoWrite16.
 * -# Set the clock of test target device status to "OFF":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoWrite16_TC_06_014_001_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead16 return OSAL_RETURN_OK.
 * -# R_OSAL_IoWrite16 return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead16 return OSAL_RETURN_OK.<br>
 * Read data is the same as the write data.
 * -# R_OSAL_IoWrite16 return OSAL_RETURN_OK.
 * -# Set clock to "OFF" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoWrite16_TC_06_014_001_testcase Test Case Example
 * @snippet test_io.cpp IoWrite16_TC_06_014_001
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoWrite16_TC_06_014_001]
TEST_F(IOApiTest, IoWrite16_TC_06_014_001)
{
    uint16_t init_data16;
    uint16_t read_data16;
    uint16_t write_data16 = TEST_WRITE16_VAL_NC;
    uintptr_t offset = TEST_WRITE16_REG_OFFSET_NC;

    for (auto it = device_list.begin(); it != device_list.end(); it++)
    {
        if (!strcmp((*it).c_str(), TEST_IO_DEVICE_1))
        {
            // pre-proces: open test target device, turn ON its power and clock
            EXPECT_EQ(R_OSAL_IoDeviceOpen((*it).c_str(), &device_handle), OSAL_RETURN_OK) << "pre-process failed";
            EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
            EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";
            EXPECT_EQ(R_OSAL_IoRead16(device_handle, offset, &init_data16), OSAL_RETURN_OK) << "pre-process failed";

            // Test06_014_001
            // @brief: normal case: write data to register of test target device with aligned offset.
            EXPECT_EQ(R_OSAL_IoWrite16(device_handle, offset, write_data16), OSAL_RETURN_OK) << "Test06_014_001 failed";
            EXPECT_EQ(R_OSAL_IoRead16(device_handle, offset, &read_data16), OSAL_RETURN_OK) << "Test06_014_001 failed";
            EXPECT_EQ(read_data16, write_data16) << "Test06_014_001 failed";

            // post-process: write back initialization value, turn OFF clock and close test target device
            EXPECT_EQ(R_OSAL_IoWrite16(device_handle, offset, init_data16), OSAL_RETURN_OK) << "post-process failed";
            EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
            EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
            device_handle = OSAL_DEVICE_HANDLE_INVALID;
        }
    }
}
// [IoWrite16_TC_06_014_001]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoWrite16_TC_06_014_002 IOApiTest.IoWrite16_TC_06_014_002
 * @subsection IoWrite16_TC_06_014_002_tsi Test Suite ID
 * IOApiTest
 * @subsection IoWrite16_TC_06_014_002_tci Test Case ID
 * IoWrite_S0_A_TC_06_014_002
 * @subsection IoWrite16_TC_06_014_002_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203
 * @subsection IoWrite16_TC_06_014_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoWrite16_TC_06_014_002_tcs Test Case Summary
 * This test shall verify the functionality of the IoWrite APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_014_002: use R_OSAL_IoWrite16 to write data to register of test target device with un-aligned offset.
 * - post-process: close test target device
 * @subsection IoWrite16_TC_06_014_002_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Write data to test target device with offset "TEST_WRITE16_OFFSET_UNALIGNED" by using R_OSAL_IoWrite16.
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoWrite16_TC_06_014_002_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoWrite16 return OSAL_RETURN_PAR
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoWrite16_TC_06_014_002_testcase Test Case Example
 * @snippet test_io.cpp IoWrite16_TC_06_014_002
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoWrite16_TC_06_014_002]
TEST_F(IOApiTest, IoWrite16_TC_06_014_002)
{
    uintptr_t offset;
    uint16_t write_data16 = TEST_WRITE16_VAL_AC;

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: write register value of test target device with un-aligned offset.
    // Test06_014_002
    offset = TEST_WRITE16_OFFSET_UNALIGNED;
    EXPECT_EQ(R_OSAL_IoWrite16(device_handle, offset, write_data16), OSAL_RETURN_PAR) << "Test06_014_002 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoWrite16_TC_06_014_002]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoWrite16_TC_06_014_003 IOApiTest.IoWrite16_TC_06_014_003
 * @subsection IoWrite16_TC_06_014_003_tsi Test Suite ID
 * IOApiTest
 * @subsection IoWrite16_TC_06_014_003_tci Test Case ID
 * IoWrite_S0_A_TC_06_014_003
 * @subsection IoWrite16_TC_06_014_003_rr Related Requirement(s)
 * \#199151, \#199199, \#199200, \#199201, \#199202, \#199203
 * @subsection IoWrite16_TC_06_014_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoWrite16_TC_06_014_003_tcs Test Case Summary
 * This test shall verify the functionality of the Write APIs of OSAL on abnormal condition:
 * - Test06_014_003: use R_OSAL_IoWrite16 to write register value with device handle is NULL
 * @subsection IoWrite16_TC_06_014_003_tpts Test Procedure/Test Steps
 * -# Write data with offset "TEST_WRITE8_REG_OFFSET_AC" by using R_OSAL_IoWrite16.
 * @subsection IoWrite16_TC_06_014_003_ereb Expected Result/Behavior
 * -# R_OSAL_IoWrite16 return OSAL_RETURN_HANDLE
 * @subsection IoWrite16_TC_06_014_003_testcase Test Case Example
 * @snippet test_io.cpp IoWrite16_TC_06_014_003
 */
// [IoWrite16_TC_06_014_003]
TEST_F(IOApiTest, IoWrite16_TC_06_014_003)
{
    uintptr_t offset;
    uint16_t write_data16 = TEST_WRITE16_VAL_AC;

    // @brief: abnormal case: write register value with device handle is NULL
    // Test06_014_003
    offset = TEST_WRITE8_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoWrite16(OSAL_DEVICE_HANDLE_INVALID, offset, write_data16), OSAL_RETURN_HANDLE) << "Test06_014_003 failed";
}
// [IoWrite16_TC_06_014_003]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoWrite16_TC_06_014_004 IOApiTest.IoWrite16_TC_06_014_004
 * @subsection IoWrite16_TC_06_014_004_tsi Test Suite ID
 * IOApiTest
 * @subsection IoWrite16_TC_06_014_004_tci Test Case ID
 * IoWrite_S0_BA_TC_06_014_004
 * @subsection IoWrite16_TC_06_014_004_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203
 * @subsection IoWrite16_TC_06_014_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoWrite16_TC_06_014_004_tcs Test Case Summary
 * This test shall verify the functionality of the IoWrite APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_014_004: use R_OSAL_IoWrite16 write data to register of test target device with offset which is out-of-register-range
 * - post-process: close test target device
 * @subsection IoWrite16_TC_06_014_004_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Write data with offset "TEST_OFFSET_OUT_RANGE" by using R_OSAL_IoWrite16.
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoWrite16_TC_06_014_004_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoWrite16 return OSAL_RETURN_PAR
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoWrite16_TC_06_014_004_testcase Test Case Example
 * @snippet test_io.cpp IoWrite16_TC_06_014_004
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoWrite16_TC_06_014_004]
TEST_F(IOApiTest, IoWrite16_TC_06_014_004)
{
    uintptr_t offset;
    uint16_t write_data16 = TEST_WRITE16_VAL_AC;

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: write register value with offset which is out-of-register-range
    // Test06_014_004
    offset = TEST_OFFSET_OUT_RANGE;
    EXPECT_EQ(R_OSAL_IoWrite16(device_handle, offset, write_data16), OSAL_RETURN_PAR) << "Test06_014_004 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoWrite16_TC_06_014_004]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoWrite16_TC_06_014_005 IOApiTest.IoWrite16_TC_06_014_005
 * @subsection IoWrite16_TC_06_014_005_tsi Test Suite ID
 * IOApiTest
 * @subsection IoWrite16_TC_06_014_005_tci Test Case ID
 * IoWrite_S2_A_TC_06_014_005
 * @subsection IoWrite16_TC_06_014_005_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203
 * @subsection IoWrite16_TC_06_014_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoWrite16_TC_06_014_005_tcs Test Case Summary
 * This test shall verify the functionality of the IoWrite APIs of OSAL on abnormal condition:
 * - Test06_014_005: use R_OSAL_IoWrite16 write data to register value of closed device handle
 * @subsection IoWrite16_TC_06_014_005_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * -# Write data with offset "TEST_WRITE8_REG_OFFSET_AC" by using R_OSAL_IoWrite16.
 * @subsection IoWrite16_TC_06_014_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite16 return OSAL_RETURN_HANDLE
 * @subsection IoWrite16_TC_06_014_005_testcase Test Case Example
 * @snippet test_io.cpp IoWrite16_TC_06_014_005
 */
// [IoWrite16_TC_06_014_005]
TEST_F(IOApiTest, IoWrite16_TC_06_014_005)
{
    uintptr_t offset;
    uint16_t write_data16 = TEST_WRITE16_VAL_AC;

    // @brief: abnormal case: write register value with closed device handle
    // Test06_014_005
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "Test06_014_005 failed";
    EXPECT_NE(device_handle, NULL_PTR) << "Test06_014_005 failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "Test06_014_005 failed";

    offset = TEST_WRITE8_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoWrite16(device_handle, offset, write_data16), OSAL_RETURN_HANDLE) << "Test06_014_005 failed";
}
// [IoWrite16_TC_06_014_005]

/**
 * @page IO_TEST IO API Test
 * @section OsalApiStateTest_IoWrite16_TC_06_014_006 OsalApiStateTest.IoWrite16_TC_06_014_006
 * @subsection IoWrite16_TC_06_014_006_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection IoWrite16_TC_06_014_006_tci Test Case ID
 * IoWrite_S2_A_TC_06_014_006
 * @subsection IoWrite16_TC_06_014_006_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203
 * @subsection IoWrite16_TC_06_014_006_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize has already been called
 * @subsection IoWrite16_TC_06_014_006_tcs Test Case Summary
 * This test shall verify the state transition of the R_OSAL_IoWrite16 function:
 * - Test06_014_006: confirm state transition error with R_OSAL_IoWrite16
 * @subsection IoWrite16_TC_06_014_006_tpts Test Procedure/Test Steps
 * -# Confirm the state transition of R_OSAL_IoWrite16
 * @subsection IoWrite16_TC_06_014_006_ereb Expected Result/Behavior
 * -# R_OSAL_IoWrite16return OSAL_RETURN_STATE
 * @subsection IoWrite16_TC_06_014_006_testcase Test Case Example
 * @snippet test_io.cpp IoWrite16_TC_06_014_006
 */
// [IoWrite16_TC_06_014_006]
TEST_F(OsalApiStateTest, IoWrite16_TC_06_014_006)
{
    uint16_t write_data16 = TEST_WRITE16_VAL_AC;
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;
    uintptr_t offset = TEST_WRITE8_REG_OFFSET_AC;

    // Test06_014_006
    // @brief: abnormal case: state transition error with IO Write
    EXPECT_EQ(R_OSAL_IoWrite16(device_handle, offset, write_data16), OSAL_RETURN_STATE) << "Test06_014_006 failed";
}
// [IoWrite16_TC_06_014_006]

/**
 * @page IO_TEST IO API Test
 * @section OSALINTTESTS_IoWrite16_TC_06_014_007 OSALINTTESTS.IoWrite16_TC_06_014_007
 * @subsection IoWrite16_TC_06_014_007_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection IoWrite16_TC_06_014_007_tci Test Case ID
 * IoWrite_S1_N_TC_06_014_007
 * @subsection IoWrite16_TC_06_014_007_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203, \#199211
 * @subsection IoWrite16_TC_06_014_007_pp Prerequisite/Precondition
 * -# OSAL is initialized.<br>
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection IoWrite16_TC_06_014_007_tcs Test Case Summary
 * @subsubsection IoWrite16_TC_06_014_007_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Setup required memory for interrupt generation.
 * - Register callack function and enable interrupt.
 * - Wait for the interrupt callback function finish.
 * @subsubsection IoWrite16_TC_06_014_007_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * @subsubsection IoWrite16_TC_06_014_007_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality of the IO APIs which are called in the ISR
 * - Disable and clear interrupt status
 * - Test06_014_007: use R_OSAL_IoWrite with 16 bit
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection IoWrite16_TC_06_014_007_tpts Test Procedure/Test Steps
 * @subsubsection IoWrite16_TC_06_014_007_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Allocate required memory for generate interrupt from test target device and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function by using R_OSAL_InterruptRegisterIsr.<br>
 * -# Enable interrupt by using R_OSAL_InterruptEnableIsr
 * -# Setup test target device specific register to generate the interrupt by calling to setup_hw_register
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection IoWrite16_TC_06_014_007_post_tpts Test Procedure/Test Steps of Post-process
 * -# Unregister interrupt by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * @subsubsection IoWrite16_TC_06_014_007_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Read initialization data from test target device with offset "TEST_WRITE16_REG_OFFSET_INT" by using R_OSAL_IoRead16.
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Write data to test target device with offset "TEST_WRITE16_REG_OFFSET_INT" by using R_OSAL_IoWrite16.
 * -# Read data from test target device with offset "TEST_WRITE16_REG_OFFSET_INT" by using R_OSAL_IoRead16.
 * -# Write back initialization data to test target device with offset "TEST_WRITE16_REG_OFFSET_INT" by using R_OSAL_IoWrite16.
 * -# Send Message interrupt of context finished by using R_OSAL_MqSendForTimePeriod
 * @subsection IoWrite16_TC_06_014_007_ereb Expected Result/Behavior
 * @subsubsection IoWrite16_TC_06_014_007_pre_ereb Expected Result/Behavior of Pre-process
 * -# allocate_setup_data_memory run successfully
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * The value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK
 * -# setup_hw_register run successfully
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection IoWrite16_TC_06_014_007_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * @subsubsection IoWrite16_TC_06_014_007_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK
 * -# R_OSAL_IoRead16 return OSAL_RETURN_OK.
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite16 return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead16 return OSAL_RETURN_OK.<br>
 * Read data is the same as the write data.
 * -# R_OSAL_IoWrite16 return OSAL_RETURN_OK.
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * @subsection IoWrite16_TC_06_014_007_testcase Test Case Example
 * @snippet test_io.cpp IoWrite16_TC_06_014_007
 * @snippet test_io.cpp osal_io_isr_test_TC_06_014_007
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [IoWrite16_TC_06_014_007]
TEST_F(OSALINTTESTS, IoWrite16_TC_06_014_007)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    st_io_arg_t io_arg;
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
 * Wait for the interrupt callback function finish. */
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_GT(numOfChannel, 0) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_io_isr_test_TC_06_014_007, (void *)&arg), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK) << "pre-process failed";
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory*/
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_io_isr_test_TC_06_014_007), OSAL_RETURN_OK) << "post-process failed";
    deallocate_data_memory(hndl_mmngr, buffer_obj);
}
// [IoWrite16_TC_06_014_007]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoWrite16_TC_06_014_008 IOApiTest.IoWrite16_TC_06_014_008
 * @subsection IoWrite16_TC_06_014_008_tsi Test Suite ID
 * IOApiTest
 * @subsection IoWrite16_TC_06_014_008_tci Test Case ID
 * IoWrite16_S1_N_TC_06_014_008
 * @subsection IoWrite16_TC_06_014_008_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203, \#19921
 * @subsection IoWrite16_TC_06_014_008_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - The synchronization queue was created
 * @subsection IoWrite16_TC_06_014_008_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoWrite16/R_OSAL_IoRead16 function with the same device handle in multi-threads:
 * - Test06_014_008: Create 2 threads to write and read to register of the same test target device.
 * @subsection IoWrite16_TC_06_014_008_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Turn ON power and clock of the target device by calling to set_clock_on
 * -# Create first thread using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE3
 *          -# task_name is "Read/write 16 bit value from/to device register in multi-threads"
 *          -# func is "io_read_write_16_device_child_thread" function with functionality:
 *              - Get config of synchronization queue by using R_OSAL_MqGetConfig
 *              - Read initialization data from test target device with offset "TEST_WRITE16_REG_OFFSET_NC" by using R_OSAL_IoRead16.
 *              - Write data is "TEST_WRITE16_VAL_NC" to test target device with offset "TEST_WRITE16_REG_OFFSET_NC" by using R_OSAL_IoWrite16.
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *          -# userarg includes device handle and thread id
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2
 *      - Address of first thread handle.
 * -# Receive message from synchronization queue by using R_OSAL_MqReceiveForTimePeriod
 * -# Create second thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE3
 *          -# task_name is "Read/write 16 bit value from/to device register in multi-threads"
 *          -# func is "io_read_write_16_device_child_thread" function with functionality:
 *              - Read data from test target device with offset "TEST_WRITE16_REG_OFFSET_NC" by using R_OSAL_IoRead16.
 *              - Write back initialization data to test target device with offset "TEST_WRITE16_REG_OFFSET_NC" by using R_OSAL_IoWrite16.
 *          -# userarg includes device handle and thread id
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID3
 *      - Address of second thread handle.
 * -# Turn OFF power and clock domain of target device by calling to set_clock_off
 * -# Join first thread by using R_OSAL_ThreadJoin.
 * -# Join second thread by using R_OSAL_ThreadJoin.
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoWrite16_TC_06_014_008_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with first thread.<br>
 * "thread_handle1" is not NULL_PTR.<br>
 * In io_read_write_16_device_child_thread function:
 *  - R_OSAL_MqGetConfig return OSAL_RETURN_OK.
 *  - R_OSAL_IoRead16 return OSAL_RETURN_OK.
 *  - R_OSAL_IoWrite16 return OSAL_RETURN_OK.
 *  - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK.
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK.<br>
 * The value of "receive_msg" need to be equal to "send_msg"
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with second thread.<br>
 * "thread_handle2" is not NULL_PTR.<br>
 * In io_read_write_16_device_child_thread function:
 *  - R_OSAL_IoRead16 return OSAL_RETURN_OK.<br>
 * "read_data16" is equal to "write_data16".
 *  - R_OSAL_IoWrite16 return OSAL_RETURN_OK.
 * -# Set power and clock to "OFF" successfully
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with first thread.<br>
 * Value of "result" is equal to OSAL_RETURN_OK.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with second thread.<br>
 * Value of "result" is equal to OSAL_RETURN_OK.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoWrite16_TC_06_014_008_testcase Test Case Example
 * @snippet test_io.cpp IoWrite16_TC_06_014_008
 * @snippet test_io.cpp io_read_write_16_device_child_thread
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoWrite16_TC_06_014_008]
TEST_F(IOApiTest, IoWrite16_TC_06_014_008)
{

    st_osal_thread_config_t config_thread;
    int64_t                 result          = 0;
    osal_thread_handle_t    thread_handle1  = OSAL_THREAD_HANDLE_INVALID;
    osal_thread_handle_t    thread_handle2  = OSAL_THREAD_HANDLE_INVALID;

    // Test06_014_008
    // @brief: normal case: Read write register 16-bit in multi-threads.
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";

    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "Test06_014_008 failed";

    io_test_multiple_thread_worker_arg user_arg1(&device_handle, TEST_THREAD_ID2);
    user_arg1.send_msg = send_msg;
    user_arg1.mq = sync_mq;
    config_thread.userarg       = &user_arg1;
    config_thread.priority      = OSAL_THREAD_PRIORITY_TYPE3;
    config_thread.task_name     = (char *)"Read/write 16 bit value from/to device register in multi-threads";
    config_thread.func          = io_read_write_16_device_child_thread;
    config_thread.stack_size    = THREAD_STACK_SIZE;
    EXPECT_EQ(R_OSAL_ThreadCreate(&config_thread, TEST_THREAD_ID2, &thread_handle1), OSAL_RETURN_OK) << "Test06_014_008 failed";
    EXPECT_NE(thread_handle1, NULL_PTR) << "Test06_014_008 failed";
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &receive_msg, mq_config.msg_size), OSAL_RETURN_OK) << "Test06_014_008 failed";
    EXPECT_EQ(receive_msg, send_msg) << "Test06_014_008 failed";

    user_arg1.uid              = TEST_THREAD_ID3;
    EXPECT_EQ(R_OSAL_ThreadCreate(&config_thread, TEST_THREAD_ID3, &thread_handle2), OSAL_RETURN_OK) << "Test06_014_008 failed";
    EXPECT_NE(thread_handle2, NULL_PTR) << "Test06_014_008 failed";

    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "Test06_014_008 failed";

    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &result),OSAL_RETURN_OK) << "Test06_014_008 failed";
    EXPECT_EQ(result, (int64_t)OSAL_RETURN_OK) << "Test06_014_008 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle2, &result),OSAL_RETURN_OK) << "Test06_014_008 failed";
    EXPECT_EQ(result, (int64_t)OSAL_RETURN_OK) << "Test06_014_008 failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoWrite16_TC_06_014_008]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoWrite32_TC_06_015_001 IOApiTest.IoWrite32_TC_06_015_001
 * @subsection IoWrite32_TC_06_015_001_tsi Test Suite ID
 * IOApiTest
 * @subsection IoWrite32_TC_06_015_001_tci Test Case ID
 * IoWrite_S1_N_TC_06_015_001
 * @subsection IoWrite32_TC_06_015_001_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203, \#199211
 * @subsection IoWrite32_TC_06_015_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - "device_list" contain the device list of each "device_type" in "devices".
 * @subsection IoWrite32_TC_06_015_001_tcs Test Case Summary
 * This test shall verify the functionality of the IoWrite APIs of OSAL on normal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_015_001: use R_OSAL_IoWrite32 to write data to register of test target device with offset "TEST_WRITE32_REG_OFFSET_NC".
 * - post-process: write back initialization value, turn OFF clock and close test target device
 * @subsection IoWrite32_TC_06_015_001_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read initialization data from test target device with offset "TEST_WRITE32_REG_OFFSET_NC" by using R_OSAL_IoRead32.
 * -# Write data to test target device with offset "TEST_WRITE32_REG_OFFSET_NC" by using R_OSAL_IoWrite32.
 * -# Read data from test target device with offset "TEST_WRITE32_REG_OFFSET_NC" by using R_OSAL_IoRead32.
 * -# Write back initialization data to test target device with offset "TEST_WRITE32_REG_OFFSET_NC" by using R_OSAL_IoWrite32.
 * -# Set the clock of test target device status to "OFF":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoWrite32_TC_06_015_001_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead32 return OSAL_RETURN_OK.
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead32 return OSAL_RETURN_OK.<br>
 * Read data is the same as the write data.
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK.
 * -# Set clock to "OFF" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoWrite32_TC_06_015_001_testcase Test Case Example
 * @snippet test_io.cpp IoWrite32_TC_06_015_001
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoWrite32_TC_06_015_001]
TEST_F(IOApiTest, IoWrite32_TC_06_015_001)
{
    uint32_t init_data32;
    uint32_t read_data32;
    uint32_t write_data32 = TEST_WRITE32_VAL_NC;
    uintptr_t offset = TEST_WRITE32_REG_OFFSET_NC;

    for (auto it = device_list.begin(); it != device_list.end(); it++)
    {
        if (!strcmp((*it).c_str(), TEST_IO_DEVICE_1))
        {
            // pre-proces: open test target device, turn ON its power and clock
            EXPECT_EQ(R_OSAL_IoDeviceOpen((*it).c_str(), &device_handle), OSAL_RETURN_OK) << "pre-process failed";
            EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
            EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";
            EXPECT_EQ(R_OSAL_IoRead32(device_handle, offset, &init_data32), OSAL_RETURN_OK) << "pre-process failed";

            // Test06_015_001
            // @brief: normal case: write data to register of test target device with aligned offset.
            EXPECT_EQ(R_OSAL_IoWrite32(device_handle, offset, write_data32), OSAL_RETURN_OK) << "Test06_015_001 failed";
            EXPECT_EQ(R_OSAL_IoRead32(device_handle, offset, &read_data32), OSAL_RETURN_OK) << "Test06_015_001 failed";
            EXPECT_EQ(read_data32, write_data32) << "Test06_015_001 failed";

            // post-process: write back initialization value, turn OFF clock and close test target device
            EXPECT_EQ(R_OSAL_IoWrite32(device_handle, offset, init_data32), OSAL_RETURN_OK) << "post-process failed";
            EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
            EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
            device_handle = OSAL_DEVICE_HANDLE_INVALID;
        }
    }
}
// [IoWrite32_TC_06_015_001]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoWrite32_TC_06_015_002 IOApiTest.IoWrite32_TC_06_015_002
 * @subsection IoWrite32_TC_06_015_002_tsi Test Suite ID
 * IOApiTest
 * @subsection IoWrite32_TC_06_015_002_tci Test Case ID
 * IoWrite_S0_A_TC_06_015_002
 * @subsection IoWrite32_TC_06_015_002_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203
 * @subsection IoWrite32_TC_06_015_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoWrite32_TC_06_015_002_tcs Test Case Summary
 * This test shall verify the functionality of the IoWrite APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_015_002: use R_OSAL_IoWrite32 to write data to register of test target device with un-aligned offset.
 * - post-process: close test target device
 * @subsection IoWrite32_TC_06_015_002_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Write data to test target device with offset "TEST_WRITE32_OFFSET_UNALIGNED" by using R_OSAL_IoWrite32.
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoWrite32_TC_06_015_002_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_PAR
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoWrite32_TC_06_015_002_testcase Test Case Example
 * @snippet test_io.cpp IoWrite32_TC_06_015_002
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoWrite32_TC_06_015_002]
TEST_F(IOApiTest, IoWrite32_TC_06_015_002)
{
    uintptr_t offset;
    uint32_t write_data32 = TEST_WRITE32_VAL_AC;

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: write register value of test target device with un-aligned offset.
    // Test06_015_002
    offset = TEST_WRITE32_OFFSET_UNALIGNED;
    EXPECT_EQ(R_OSAL_IoWrite32(device_handle, offset, write_data32), OSAL_RETURN_PAR) << "Test06_015_002 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoWrite32_TC_06_015_002]
/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoWrite32_TC_06_015_003 IOApiTest.IoWrite32_TC_06_015_003
 * @subsection IoWrite32_TC_06_015_003_tsi Test Suite ID
 * IOApiTest
 * @subsection IoWrite32_TC_06_015_003_tci Test Case ID
 * IoWrite_S0_A_TC_06_015_003
 * @subsection IoWrite32_TC_06_015_003_rr Related Requirement(s)
 * \#199151, \#199199, \#199200, \#199201, \#199202, \#199203
 * @subsection IoWrite32_TC_06_015_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoWrite32_TC_06_015_003_tcs Test Case Summary
 * This test shall verify the functionality of the Write APIs of OSAL on abnormal condition:
 * - Test06_015_003: use R_OSAL_IoWrite32 to write register value with device handle is NULL
 * @subsection IoWrite32_TC_06_015_003_tpts Test Procedure/Test Steps
 * -# Write data with offset "TEST_WRITE32_REG_OFFSET_AC" by using R_OSAL_IoWrite32.
 * @subsection IoWrite32_TC_06_015_003_ereb Expected Result/Behavior
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_HANDLE
 * @subsection IoWrite32_TC_06_015_003_testcase Test Case Example
 * @snippet test_io.cpp IoWrite32_TC_06_015_003
 */
// [IoWrite32_TC_06_015_003]
TEST_F(IOApiTest, IoWrite32_TC_06_015_003)
{
    uintptr_t offset;
    uint32_t write_data32 = TEST_WRITE32_VAL_AC;

    // @brief: abnormal case: write register value with device handle is NULL
    // Test06_015_003
    offset = TEST_WRITE32_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoWrite32(OSAL_DEVICE_HANDLE_INVALID, offset, write_data32), OSAL_RETURN_HANDLE) << "Test06_015_003 failed";
}
// [IoWrite32_TC_06_015_003]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoWrite32_TC_06_015_004 IOApiTest.IoWrite32_TC_06_015_004
 * @subsection IoWrite32_TC_06_015_004_tsi Test Suite ID
 * IOApiTest
 * @subsection IoWrite32_TC_06_015_004_tci Test Case ID
 * IoWrite_S0_BA_TC_06_015_004
 * @subsection IoWrite32_TC_06_015_004_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203
 * @subsection IoWrite32_TC_06_015_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoWrite32_TC_06_015_004_tcs Test Case Summary
 * This test shall verify the functionality of the IoWrite APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_015_004: use R_OSAL_IoWrite32 write data to register of test target device with offset which is out-of-register-range
 * - post-process: close test target device
 * @subsection IoWrite32_TC_06_015_004_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Write data with offset "TEST_OFFSET_OUT_RANGE" by using R_OSAL_IoWrite32.
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoWrite32_TC_06_015_004_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_PAR
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoWrite32_TC_06_015_004_testcase Test Case Example
 * @snippet test_io.cpp IoWrite32_TC_06_015_004
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoWrite32_TC_06_015_004]
TEST_F(IOApiTest, IoWrite32_TC_06_015_004)
{
    uintptr_t offset;
    uint32_t write_data32 = TEST_WRITE32_VAL_AC;

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: write register value with offset which is out-of-register-range
    // Test06_015_004
    offset = TEST_OFFSET_OUT_RANGE;
    EXPECT_EQ(R_OSAL_IoWrite32(device_handle, offset, write_data32), OSAL_RETURN_PAR) << "Test06_015_004 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoWrite32_TC_06_015_004]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoWrite32_TC_06_015_005 IOApiTest.IoWrite32_TC_06_015_005
 * @subsection IoWrite32_TC_06_015_005_tsi Test Suite ID
 * IOApiTest
 * @subsection IoWrite32_TC_06_015_005_tci Test Case ID
 * IoWrite_S2_A_TC_06_015_005
 * @subsection IoWrite32_TC_06_015_005_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203
 * @subsection IoWrite32_TC_06_015_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoWrite32_TC_06_015_005_tcs Test Case Summary
 * This test shall verify the functionality of the IoWrite APIs of OSAL on abnormal condition:
 * - Test06_015_005: use R_OSAL_IoWrite32 write data to register value of closed device handle
 * @subsection IoWrite32_TC_06_015_005_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * -# Write data with offset "TEST_WRITE32_REG_OFFSET_AC" by using R_OSAL_IoWrite32.
 * @subsection IoWrite32_TC_06_015_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_HANDLE
 * @subsection IoWrite32_TC_06_015_005_testcase Test Case Example
 * @snippet test_io.cpp IoWrite32_TC_06_015_005
 */
// [IoWrite32_TC_06_015_005]
TEST_F(IOApiTest, IoWrite32_TC_06_015_005)
{
    uintptr_t offset;
    uint32_t write_data32 = TEST_WRITE32_VAL_AC;

    // @brief: abnormal case: write register value with closed device handle
    // Test06_015_005
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "Test06_015_005 failed";
    EXPECT_NE(device_handle, NULL_PTR) << "Test06_015_005 failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "Test06_015_005 failed";

    offset = TEST_WRITE32_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoWrite32(device_handle, offset, write_data32), OSAL_RETURN_HANDLE) << "Test06_015_005 failed";
}
// [IoWrite32_TC_06_015_005]

/**
 * @page IO_TEST IO API Test
 * @section OsalApiStateTest_IoWrite32_TC_06_015_006 OsalApiStateTest.IoWrite32_TC_06_015_006
 * @subsection IoWrite32_TC_06_015_006_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection IoWrite32_TC_06_015_006_tci Test Case ID
 * IoWrite_S2_A_TC_06_015_006
 * @subsection IoWrite32_TC_06_015_006_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203
 * @subsection IoWrite32_TC_06_015_006_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize has already been called
 * @subsection IoWrite32_TC_06_015_006_tcs Test Case Summary
 * This test shall verify the state transition of the R_OSAL_IoWrite32 function:
 * - Test06_015_006: confirm state transition error with R_OSAL_IoWrite32
 * @subsection IoWrite32_TC_06_015_006_tpts Test Procedure/Test Steps
 * -# Confirm the state transition of R_OSAL_IoWrite32
 * @subsection IoWrite32_TC_06_015_006_ereb Expected Result/Behavior
 * -# R_OSAL_IoWrite32return OSAL_RETURN_STATE
 * @subsection IoWrite32_TC_06_015_006_testcase Test Case Example
 * @snippet test_io.cpp IoWrite32_TC_06_015_006
 */
// [IoWrite32_TC_06_015_006]
TEST_F(OsalApiStateTest, IoWrite32_TC_06_015_006)
{
    uint32_t write_data32 = TEST_WRITE32_VAL_AC;
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;
    uintptr_t offset = TEST_WRITE32_REG_OFFSET_AC;

    // Test06_015_006
    // @brief: abnormal case: state transition error with IO Write
    EXPECT_EQ(R_OSAL_IoWrite32(device_handle, offset, write_data32), OSAL_RETURN_STATE) << "Test06_015_006 failed";
}
// [IoWrite32_TC_06_015_006]

/**
 * @page IO_TEST IO API Test
 * @section OSALINTTESTS_IoWrite32_TC_06_015_007 OSALINTTESTS.IoWrite32_TC_06_015_007
 * @subsection IoWrite32_TC_06_015_007_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection IoWrite32_TC_06_015_007_tci Test Case ID
 * IoWrite_S1_N_TC_06_015_007
 * @subsection IoWrite32_TC_06_015_007_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203, \#199211
 * @subsection IoWrite32_TC_06_015_007_pp Prerequisite/Precondition
 * -# OSAL is initialized.<br>
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection IoWrite32_TC_06_015_007_tcs Test Case Summary
 * @subsubsection IoWrite32_TC_06_015_007_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Setup required memory for interrupt generation.
 * - Register callack function and enable interrupt.
 * - Wait for the interrupt callback function finish.
 * @subsubsection IoWrite32_TC_06_015_007_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * @subsubsection IoWrite32_TC_06_015_007_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality of the IO APIs which are called in the ISR
 * - Disable and clear interrupt status
 * - Test06_015_007: use R_OSAL_IoWrite with 32 bit
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection IoWrite32_TC_06_015_007_tpts Test Procedure/Test Steps
 * @subsubsection IoWrite32_TC_06_015_007_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Allocate required memory for generate interrupt from test target device and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function by using R_OSAL_InterruptRegisterIsr.<br>
 * -# Enable interrupt by using R_OSAL_InterruptEnableIsr
 * -# Setup test target device specific register to generate the interrupt by calling to setup_hw_register
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection IoWrite32_TC_06_015_007_post_tpts Test Procedure/Test Steps of Post-process
 * -# Unregister interrupt by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * @subsubsection IoWrite32_TC_06_015_007_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Read initialization data from test target device with offset "TEST_WRITE32_REG_OFFSET_INT" by using R_OSAL_IoRead32.
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Write data to test target device with offset "TEST_WRITE32_REG_OFFSET_INT" by using R_OSAL_IoWrite32.
 * -# Read data from test target device with offset "TEST_WRITE32_REG_OFFSET_INT" by using R_OSAL_IoRead32.
 * -# Write back initialization data to test target device with offset "TEST_WRITE32_REG_OFFSET_INT" by using R_OSAL_IoWrite32.
 * -# Send Message interrupt of context finished by using R_OSAL_MqSendForTimePeriod
 * @subsection IoWrite32_TC_06_015_007_ereb Expected Result/Behavior
 * @subsubsection IoWrite32_TC_06_015_007_pre_ereb Expected Result/Behavior of Pre-process
 * -# allocate_setup_data_memory run successfully
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * The value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK
 * -# setup_hw_register run successfully
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection IoWrite32_TC_06_015_007_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * @subsubsection IoWrite32_TC_06_015_007_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK
 * -# R_OSAL_IoRead32 return OSAL_RETURN_OK.
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead32 return OSAL_RETURN_OK.<br>
 * Read data is the same as the write data.
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK.
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * @subsection IoWrite32_TC_06_015_007_testcase Test Case Example
 * @snippet test_io.cpp IoWrite32_TC_06_015_007
 * @snippet test_io.cpp osal_io_isr_test_TC_06_015_007
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [IoWrite32_TC_06_015_007]
TEST_F(OSALINTTESTS, IoWrite32_TC_06_015_007)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    st_io_arg_t io_arg;
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
 * Wait for the interrupt callback function finish. */
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_GT(numOfChannel, 0) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_io_isr_test_TC_06_015_007, (void *)&arg), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK) << "pre-process failed";
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory*/
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_io_isr_test_TC_06_015_007), OSAL_RETURN_OK) << "post-process failed";
    deallocate_data_memory(hndl_mmngr, buffer_obj);
}
// [IoWrite32_TC_06_015_007]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoWrite32_TC_06_015_008 IOApiTest.IoWrite32_TC_06_015_008
 * @subsection IoWrite32_TC_06_015_008_tsi Test Suite ID
 * IOApiTest
 * @subsection IoWrite32_TC_06_015_008_tci Test Case ID
 * IoWrite32_S1_N_TC_06_015_008
 * @subsection IoWrite32_TC_06_015_008_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203, \#199211
 * @subsection IoWrite32_TC_06_015_008_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - The synchronization queue was created
 * @subsection IoWrite32_TC_06_015_008_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoWrite32/R_OSAL_IoRead32 function with the same device handle in multi-threads:
 * - Test06_015_008: Create 2 threads to write and read to register of the same test target device.
 * @subsection IoWrite32_TC_06_015_008_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Turn ON power and clock of the target device by calling to set_clock_on
 * -# Create first thread using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE3
 *          -# task_name is "Read/write 32 bit value from/to device register in multi-threads"
 *          -# func is "io_read_write_32_device_child_thread" function with functionality:
 *              - Get config of synchronization queue by using R_OSAL_MqGetConfig
 *              - Read initialization data from test target device with offset "TEST_WRITE32_REG_OFFSET_NC" by using R_OSAL_IoRead32.
 *              - Write data is "TEST_WRITE32_VAL_NC" to test target device with offset "TEST_WRITE32_REG_OFFSET_NC" by using R_OSAL_IoWrite32.
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *          -# userarg includes device handle and thread id
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2
 *      - Address of first thread handle.
 * -# Receive message from synchronization queue by using R_OSAL_MqReceiveForTimePeriod
 * -# Create second thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE3
 *          -# task_name is "Read/write 32 bit value from/to device register in multi-threads"
 *          -# func is "io_read_write_32_device_child_thread" function with functionality:
 *              - Read data from test target device with offset "TEST_WRITE32_REG_OFFSET_NC" by using R_OSAL_IoRead32.
 *              - Write back initialization data to test target device with offset "TEST_WRITE32_REG_OFFSET_NC" by using R_OSAL_IoWrite32.
 *          -# userarg includes device handle and thread id
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID3
 *      - Address of second thread handle.
 * -# Turn OFF power and clock domain of target device by calling to set_clock_off
 * -# Join first thread by using R_OSAL_ThreadJoin.
 * -# Join second thread by using R_OSAL_ThreadJoin.
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoWrite32_TC_06_015_008_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with first thread.<br>
 * "thread_handle1" is not NULL_PTR.<br>
 * In io_read_write_32_device_child_thread function:
 *  - R_OSAL_MqGetConfig return OSAL_RETURN_OK.
 *  - R_OSAL_IoRead32 return OSAL_RETURN_OK.
 *  - R_OSAL_IoWrite32 return OSAL_RETURN_OK.
 *  - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK.
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK.<br>
 * The value of "receive_msg" need to be equal to "send_msg"
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with second thread.<br>
 * "thread_handle2" is not NULL_PTR.<br>
 * In io_read_write_32_device_child_thread function:
 *  - R_OSAL_IoRead32 return OSAL_RETURN_OK.<br>
 * "read_data32" is equal to "write_data32".
 *  - R_OSAL_IoWrite32 return OSAL_RETURN_OK.
 * -# Set power and clock to "OFF" successfully
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with first thread.<br>
 * Value of "result" is equal to OSAL_RETURN_OK.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with second thread.<br>
 * Value of "result" is equal to OSAL_RETURN_OK.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoWrite32_TC_06_015_008_testcase Test Case Example
 * @snippet test_io.cpp IoWrite32_TC_06_015_008
 * @snippet test_io.cpp io_read_write_32_device_child_thread
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoWrite32_TC_06_015_008]
TEST_F(IOApiTest, IoWrite32_TC_06_015_008)
{

    st_osal_thread_config_t config_thread;
    int64_t                 result          = 0;
    osal_thread_handle_t    thread_handle1  = OSAL_THREAD_HANDLE_INVALID;
    osal_thread_handle_t    thread_handle2  = OSAL_THREAD_HANDLE_INVALID;

    // Test06_015_008
    // @brief: normal case: Read write register 32-bit in multi-threads.
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";

    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "Test06_015_008 failed";

    io_test_multiple_thread_worker_arg user_arg1(&device_handle, TEST_THREAD_ID2);
    user_arg1.send_msg = send_msg;
    user_arg1.mq = sync_mq;
    config_thread.userarg       = &user_arg1;
    config_thread.priority      = OSAL_THREAD_PRIORITY_TYPE3;
    config_thread.task_name     = (char *)"Read/write 32 bit value from/to device register in multi-threads";
    config_thread.func          = io_read_write_32_device_child_thread;
    config_thread.stack_size    = THREAD_STACK_SIZE;
    EXPECT_EQ(R_OSAL_ThreadCreate(&config_thread, TEST_THREAD_ID2, &thread_handle1), OSAL_RETURN_OK) << "Test06_015_008 failed";
    EXPECT_NE(thread_handle1, NULL_PTR) << "Test06_015_008 failed";
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &receive_msg, mq_config.msg_size), OSAL_RETURN_OK) << "Test06_015_008 failed";
    EXPECT_EQ(receive_msg, send_msg) << "Test06_015_008 failed";

    user_arg1.uid              = TEST_THREAD_ID3;
    EXPECT_EQ(R_OSAL_ThreadCreate(&config_thread, TEST_THREAD_ID3, &thread_handle2), OSAL_RETURN_OK) << "Test06_015_008 failed";
    EXPECT_NE(thread_handle2, NULL_PTR) << "Test06_015_008 failed";

    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "Test06_015_008 failed";

    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &result),OSAL_RETURN_OK) << "Test06_015_008 failed";
    EXPECT_EQ(result, (int64_t)OSAL_RETURN_OK) << "Test06_015_008 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle2, &result),OSAL_RETURN_OK) << "Test06_015_008 failed";
    EXPECT_EQ(result, (int64_t)OSAL_RETURN_OK) << "Test06_015_008 failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoWrite32_TC_06_015_008]
#endif

#ifndef SKIP_SECTION_2
/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoWrite64_TC_06_016_001 IOApiTest.IoWrite64_TC_06_016_001
 * @subsection IoWrite64_TC_06_016_001_tsi Test Suite ID
 * IOApiTest
 * @subsection IoWrite64_TC_06_016_001_tci Test Case ID
 * IoWrite_S1_N_TC_06_016_001
 * @subsection IoWrite64_TC_06_016_001_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203, \#199211
 * @subsection IoWrite64_TC_06_016_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - "device_list" contain the device list of each "device_type" in "devices".
 * @subsection IoWrite64_TC_06_016_001_tcs Test Case Summary
 * This test shall verify the functionality of the IoWrite APIs of OSAL on normal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_016_001: use R_OSAL_IoWrite64 to write data to register of test target device with offset "TEST_WRITE64_REG_OFFSET_NC".
 * - post-process: write back initialization value, turn OFF clock and close test target device
 * @subsection IoWrite64_TC_06_016_001_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read initialization data from test target device with offset "TEST_WRITE64_REG_OFFSET_NC" by using R_OSAL_IoRead64.
 * -# Write data to test target device with offset "TEST_WRITE64_REG_OFFSET_NC" by using R_OSAL_IoWrite64.
 * -# Read data from test target device with offset "TEST_WRITE64_REG_OFFSET_NC" by using R_OSAL_IoRead64.
 * -# Write back initialization data to test target device with offset "TEST_WRITE64_REG_OFFSET_NC" by using R_OSAL_IoWrite64.
 * -# Set the clock of test target device status to "OFF":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoWrite64_TC_06_016_001_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead64 return OSAL_RETURN_OK.
 * -# R_OSAL_IoWrite64 return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead64 return OSAL_RETURN_OK.<br>
 * Read data is the same as the write data.
 * -# R_OSAL_IoWrite64 return OSAL_RETURN_OK.
 * -# Set clock to "OFF" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoWrite64_TC_06_016_001_testcase Test Case Example
 * @snippet test_io.cpp IoWrite64_TC_06_016_001
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoWrite64_TC_06_016_001]
TEST_F(IOApiTest, IoWrite64_TC_06_016_001)
{
    uint64_t init_data64;
    uint64_t read_data64;
    uint64_t write_data64 = TEST_WRITE64_VAL_NC;
    uintptr_t offset = TEST_WRITE64_REG_OFFSET_NC;

    for (auto it = device_list.begin(); it != device_list.end(); it++)
    {
        if (!strcmp((*it).c_str(), TEST_IO_DEVICE_1))
        {
            // pre-proces: open test target device, turn ON its power and clock
            EXPECT_EQ(R_OSAL_IoDeviceOpen((*it).c_str(), &device_handle), OSAL_RETURN_OK) << "pre-process failed";
            EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
            EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";
            EXPECT_EQ(R_OSAL_IoRead64(device_handle, offset, &init_data64), OSAL_RETURN_OK) << "pre-process failed";

            // Test06_016_001
            // @brief: normal case: write data to register of test target device with aligned offset.
            EXPECT_EQ(R_OSAL_IoWrite64(device_handle, offset, write_data64), OSAL_RETURN_OK) << "Test06_016_001 failed";
            EXPECT_EQ(R_OSAL_IoRead64(device_handle, offset, &read_data64), OSAL_RETURN_OK) << "Test06_016_001 failed";
            EXPECT_EQ(read_data64, write_data64) << "Test06_016_001 failed";

            // post-process: write back initialization value, turn OFF clock and close test target device
            EXPECT_EQ(R_OSAL_IoWrite64(device_handle, offset, init_data64), OSAL_RETURN_OK) << "post-process failed";
            EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
            EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
            device_handle = OSAL_DEVICE_HANDLE_INVALID;
        }
    }
}
// [IoWrite64_TC_06_016_001]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoWrite64_TC_06_016_002 IOApiTest.IoWrite64_TC_06_016_002
 * @subsection IoWrite64_TC_06_016_002_tsi Test Suite ID
 * IOApiTest
 * @subsection IoWrite64_TC_06_016_002_tci Test Case ID
 * IoWrite_S0_A_TC_06_016_002
 * @subsection IoWrite64_TC_06_016_002_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203
 * @subsection IoWrite64_TC_06_016_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoWrite64_TC_06_016_002_tcs Test Case Summary
 * This test shall verify the functionality of the IoWrite APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_016_002: use R_OSAL_IoWrite64 to write data to register of test target device with un-aligned offset.
 * - post-process: close test target device
 * @subsection IoWrite64_TC_06_016_002_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Write data to test target device with offset "TEST_WRITE64_OFFSET_UNALIGNED" by using R_OSAL_IoWrite64.
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoWrite64_TC_06_016_002_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoWrite64 return OSAL_RETURN_PAR
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoWrite64_TC_06_016_002_testcase Test Case Example
 * @snippet test_io.cpp IoWrite64_TC_06_016_002
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoWrite64_TC_06_016_002]
TEST_F(IOApiTest, IoWrite64_TC_06_016_002)
{
    uintptr_t offset;
    uint64_t write_data64 = TEST_WRITE64_VAL_AC;

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: write register value of test target device with un-aligned offset.
    // Test06_016_002
    offset = TEST_WRITE64_OFFSET_UNALIGNED;
    EXPECT_EQ(R_OSAL_IoWrite64(device_handle, offset, write_data64), OSAL_RETURN_PAR) << "Test06_016_002 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoWrite64_TC_06_016_002]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoWrite64_TC_06_016_003 IOApiTest.IoWrite64_TC_06_016_003
 * @subsection IoWrite64_TC_06_016_003_tsi Test Suite ID
 * IOApiTest
 * @subsection IoWrite64_TC_06_016_003_tci Test Case ID
 * IoWrite_S0_A_TC_06_016_003
 * @subsection IoWrite64_TC_06_016_003_rr Related Requirement(s)
 * \#199151, \#199199, \#199200, \#199201, \#199202, \#199203
 * @subsection IoWrite64_TC_06_016_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoWrite64_TC_06_016_003_tcs Test Case Summary
 * This test shall verify the functionality of the Write APIs of OSAL on abnormal condition:
 * - Test06_016_003: use R_OSAL_IoWrite64 to write register value with device handle is NULL
 * @subsection IoWrite64_TC_06_016_003_tpts Test Procedure/Test Steps
 * -# Write data with offset "TEST_WRITE64_REG_OFFSET_AC" by using R_OSAL_IoWrite64.
 * @subsection IoWrite64_TC_06_016_003_ereb Expected Result/Behavior
 * -# R_OSAL_IoWrite64 return OSAL_RETURN_HANDLE
 * @subsection IoWrite64_TC_06_016_003_testcase Test Case Example
 * @snippet test_io.cpp IoWrite64_TC_06_016_003
 */
// [IoWrite64_TC_06_016_003]
TEST_F(IOApiTest, IoWrite64_TC_06_016_003)
{
    uintptr_t offset;
    uint64_t write_data64 = TEST_WRITE64_VAL_AC;

    // @brief: abnormal case: write register value with device handle is NULL
    // Test06_016_003
    offset = TEST_WRITE64_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoWrite64(OSAL_DEVICE_HANDLE_INVALID, offset, write_data64), OSAL_RETURN_HANDLE) << "Test06_016_003 failed";
}
// [IoWrite64_TC_06_016_003]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoWrite64_TC_06_016_004 IOApiTest.IoWrite64_TC_06_016_004
 * @subsection IoWrite64_TC_06_016_004_tsi Test Suite ID
 * IOApiTest
 * @subsection IoWrite64_TC_06_016_004_tci Test Case ID
 * IoWrite_S0_BA_TC_06_016_004
 * @subsection IoWrite64_TC_06_016_004_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203
 * @subsection IoWrite64_TC_06_016_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoWrite64_TC_06_016_004_tcs Test Case Summary
 * This test shall verify the functionality of the IoWrite APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_016_004: use R_OSAL_IoWrite64 write data to register of test target device with offset which is out-of-register-range
 * - post-process: close test target device
 * @subsection IoWrite64_TC_06_016_004_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Write data with offset "TEST_OFFSET_OUT_RANGE" by using R_OSAL_IoWrite64.
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoWrite64_TC_06_016_004_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoWrite64 return OSAL_RETURN_PAR
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoWrite64_TC_06_016_004_testcase Test Case Example
 * @snippet test_io.cpp IoWrite64_TC_06_016_004
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoWrite64_TC_06_016_004]
TEST_F(IOApiTest, IoWrite64_TC_06_016_004)
{
    uintptr_t offset;
    uint64_t write_data64 = TEST_WRITE64_VAL_AC;

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: write register value with offset which is out-of-register-range
    // Test06_016_004
    offset = TEST_OFFSET_OUT_RANGE;
    EXPECT_EQ(R_OSAL_IoWrite64(device_handle, offset, write_data64), OSAL_RETURN_PAR) << "Test06_016_004 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoWrite64_TC_06_016_004]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoWrite64_TC_06_016_005 IOApiTest.IoWrite64_TC_06_016_005
 * @subsection IoWrite64_TC_06_016_005_tsi Test Suite ID
 * IOApiTest
 * @subsection IoWrite64_TC_06_016_005_tci Test Case ID
 * IoWrite_S2_A_TC_06_016_005
 * @subsection IoWrite64_TC_06_016_005_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203
 * @subsection IoWrite64_TC_06_016_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoWrite64_TC_06_016_005_tcs Test Case Summary
 * This test shall verify the functionality of the IoWrite APIs of OSAL on abnormal condition:
 * - Test06_016_005: use R_OSAL_IoWrite64 write data to register value of closed device handle
 * @subsection IoWrite64_TC_06_016_005_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * -# Write data with offset "TEST_WRITE64_REG_OFFSET_AC" by using R_OSAL_IoWrite64.
 * @subsection IoWrite64_TC_06_016_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite64 return OSAL_RETURN_HANDLE
 * @subsection IoWrite64_TC_06_016_005_testcase Test Case Example
 * @snippet test_io.cpp IoWrite64_TC_06_016_005
 */
// [IoWrite64_TC_06_016_005]
TEST_F(IOApiTest, IoWrite64_TC_06_016_005)
{
    uintptr_t offset;
    uint64_t write_data64 = TEST_WRITE64_VAL_AC;

    // @brief: abnormal case: write register value with closed device handle
    // Test06_016_005
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "Test06_016_005 failed";
    EXPECT_NE(device_handle, NULL_PTR) << "Test06_016_005 failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "Test06_016_005 failed";

    offset = TEST_WRITE64_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoWrite64(device_handle, offset, write_data64), OSAL_RETURN_HANDLE) << "Test06_016_005 failed";
}
// [IoWrite64_TC_06_016_005]

/**
 * @page IO_TEST IO API Test
 * @section OsalApiStateTest_IoWrite64_TC_06_016_006 OsalApiStateTest.IoWrite64_TC_06_016_006
 * @subsection IoWrite64_TC_06_016_006_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection IoWrite64_TC_06_016_006_tci Test Case ID
 * IoWrite_S2_A_TC_06_016_006
 * @subsection IoWrite64_TC_06_016_006_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203
 * @subsection IoWrite64_TC_06_016_006_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize has already been called
 * @subsection IoWrite64_TC_06_016_006_tcs Test Case Summary
 * This test shall verify the state transition of the R_OSAL_IoWrite64 function:
 * - Test06_016_006: confirm state transition error with R_OSAL_IoWrite64
 * @subsection IoWrite64_TC_06_016_006_tpts Test Procedure/Test Steps
 * -# Confirm the state transition of R_OSAL_IoWrite64
 * @subsection IoWrite64_TC_06_016_006_ereb Expected Result/Behavior
 * -# R_OSAL_IoWrite64return OSAL_RETURN_STATE
 * @subsection IoWrite64_TC_06_016_006_testcase Test Case Example
 * @snippet test_io.cpp IoWrite64_TC_06_016_006
 */
// [IoWrite64_TC_06_016_006]
TEST_F(OsalApiStateTest, IoWrite64_TC_06_016_006)
{
    uint64_t write_data64 = TEST_WRITE64_VAL_AC;
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;
    uintptr_t offset = TEST_WRITE64_REG_OFFSET_AC;

    // Test06_016_006
    // @brief: abnormal case: state transition error with IO Write
    EXPECT_EQ(R_OSAL_IoWrite64(device_handle, offset, write_data64), OSAL_RETURN_STATE) << "Test06_016_006 failed";
}
// [IoWrite64_TC_06_016_006]

/**
 * @page IO_TEST IO API Test
 * @section OSALINTTESTS_IoWrite64_TC_06_016_007 OSALINTTESTS.IoWrite64_TC_06_016_007
 * @subsection IoWrite64_TC_06_016_007_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection IoWrite64_TC_06_016_007_tci Test Case ID
 * IoWrite_S1_N_TC_06_016_007
 * @subsection IoWrite64_TC_06_016_007_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203, \#199211
 * @subsection IoWrite64_TC_06_016_007_pp Prerequisite/Precondition
 * -# OSAL is initialized.<br>
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection IoWrite64_TC_06_016_007_tcs Test Case Summary
 * @subsubsection IoWrite64_TC_06_016_007_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Setup required memory for interrupt generation.
 * - Register callack function and enable interrupt.
 * - Wait for the interrupt callback function finish.
 * @subsubsection IoWrite64_TC_06_016_007_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * @subsubsection IoWrite64_TC_06_016_007_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality of the IO APIs which are called in the ISR
 * - Disable and clear interrupt status
 * - Test06_016_007: use R_OSAL_IoWrite with 64 bit
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection IoWrite64_TC_06_016_007_tpts Test Procedure/Test Steps
 * @subsubsection IoWrite64_TC_06_016_007_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Allocate required memory for generate interrupt from test target device and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function by using R_OSAL_InterruptRegisterIsr.<br>
 * -# Enable interrupt by using R_OSAL_InterruptEnableIsr
 * -# Setup test target device specific register to generate the interrupt by calling to setup_hw_register
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection IoWrite64_TC_06_016_007_post_tpts Test Procedure/Test Steps of Post-process
 * -# Unregister interrupt by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * @subsubsection IoWrite64_TC_06_016_007_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Read initialization data from test target device with offset "TEST_WRITE64_REG_OFFSET_INT" by using R_OSAL_IoRead64.
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Write data to test target device with offset "TEST_WRITE64_REG_OFFSET_INT" by using R_OSAL_IoWrite64.
 * -# Read data from test target device with offset "TEST_WRITE64_REG_OFFSET_INT" by using R_OSAL_IoRead64.
 * -# Write back initialization data to test target device with offset "TEST_WRITE64_REG_OFFSET_INT" by using R_OSAL_IoWrite64.
 * -# Send Message interrupt of context finished by using R_OSAL_MqSendForTimePeriod
 * @subsection IoWrite64_TC_06_016_007_ereb Expected Result/Behavior
 * @subsubsection IoWrite64_TC_06_016_007_pre_ereb Expected Result/Behavior of Pre-process
 * -# allocate_setup_data_memory run successfully
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * The value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK
 * -# setup_hw_register run successfully
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection IoWrite64_TC_06_016_007_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * @subsubsection IoWrite64_TC_06_016_007_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK
 * -# R_OSAL_IoRead64 return OSAL_RETURN_OK.
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite64 return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead64 return OSAL_RETURN_OK.<br>
 * Read data is the same as the write data.
 * -# R_OSAL_IoWrite64 return OSAL_RETURN_OK.
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * @subsection IoWrite64_TC_06_016_007_testcase Test Case Example
 * @snippet test_io.cpp IoWrite64_TC_06_016_007
 * @snippet test_io.cpp osal_io_isr_test_TC_06_016_007
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [IoWrite64_TC_06_016_007]
TEST_F(OSALINTTESTS, IoWrite64_TC_06_016_007)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    st_io_arg_t io_arg;
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
 * Wait for the interrupt callback function finish. */
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_GT(numOfChannel, 0) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_io_isr_test_TC_06_016_007, (void *)&arg), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK) << "pre-process failed";
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory*/
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_io_isr_test_TC_06_016_007), OSAL_RETURN_OK) << "post-process failed";
    deallocate_data_memory(hndl_mmngr, buffer_obj);
}
// [IoWrite64_TC_06_016_007]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoWrite64_TC_06_016_008 IOApiTest.IoWrite64_TC_06_016_008
 * @subsection IoWrite64_TC_06_016_008_tsi Test Suite ID
 * IOApiTest
 * @subsection IoWrite64_TC_06_016_008_tci Test Case ID
 * IoWrite64_S1_N_TC_06_016_008
 * @subsection IoWrite64_TC_06_016_008_rr Related Requirement(s)
 * \#199200, \#199201, \#199202, \#199203, \#199211
 * @subsection IoWrite64_TC_06_016_008_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - The synchronization queue was created
 * @subsection IoWrite64_TC_06_016_008_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoWrite64/R_OSAL_IoRead64 function with the same device handle in multi-threads:
 * - Test06_016_008: Create 2 threads to write and read to register of the same test target device.
 * @subsection IoWrite64_TC_06_016_008_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Turn ON power and clock of the target device by calling to set_clock_on
 * -# Create first thread using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE3
 *          -# task_name is "Read/write 64 bit value from/to device register in multi-threads"
 *          -# func is "io_read_write_64_device_child_thread" function with functionality:
 *              - Get config of synchronization queue by using R_OSAL_MqGetConfig
 *              - Read initialization data from test target device with offset "TEST_WRITE64_REG_OFFSET_NC" by using R_OSAL_IoRead64.
 *              - Write data is "TEST_WRITE64_VAL_NC" to test target device with offset "TEST_WRITE64_REG_OFFSET_NC" by using R_OSAL_IoWrite64.
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *          -# userarg includes device handle and thread id
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2
 *      - Address of first thread handle.
 * -# Receive message from synchronization queue by using R_OSAL_MqReceiveForTimePeriod
 * -# Create second thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE3
 *          -# task_name is "Read/write 64 bit value from/to device register in multi-threads"
 *          -# func is "io_read_write_64_device_child_thread" function with functionality:
 *              - Read data from test target device with offset "TEST_WRITE64_REG_OFFSET_NC" by using R_OSAL_IoRead64.
 *              - Write back initialization data to test target device with offset "TEST_WRITE64_REG_OFFSET_NC" by using R_OSAL_IoWrite64.
 *          -# userarg includes device handle and thread id
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID3
 *      - Address of second thread handle.
 * -# Turn OFF power and clock domain of target device by calling to set_clock_off
 * -# Join first thread by using R_OSAL_ThreadJoin.
 * -# Join second thread by using R_OSAL_ThreadJoin.
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoWrite64_TC_06_016_008_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with first thread.<br>
 * "thread_handle1" is not NULL_PTR.<br>
 * In io_read_write_64_device_child_thread function:
 *  - R_OSAL_MqGetConfig return OSAL_RETURN_OK.
 *  - R_OSAL_IoRead64 return OSAL_RETURN_OK.
 *  - R_OSAL_IoWrite64 return OSAL_RETURN_OK.
 *  - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK.
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK.<br>
 * The value of "receive_msg" need to be equal to "send_msg"
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with second thread.<br>
 * "thread_handle2" is not NULL_PTR.<br>
 * In io_read_write_64_device_child_thread function:
 *  - R_OSAL_IoRead64 return OSAL_RETURN_OK.<br>
 * "read_data64" is equal to "write_data64".
 *  - R_OSAL_IoWrite64 return OSAL_RETURN_OK.
 * -# Set power and clock to "OFF" successfully
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with first thread.<br>
 * Value of "result" is equal to OSAL_RETURN_OK.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with second thread.<br>
 * Value of "result" is equal to OSAL_RETURN_OK.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoWrite64_TC_06_016_008_testcase Test Case Example
 * @snippet test_io.cpp IoWrite64_TC_06_016_008
 * @snippet test_io.cpp io_read_write_64_device_child_thread
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoWrite64_TC_06_016_008]
TEST_F(IOApiTest, IoWrite64_TC_06_016_008)
{

    st_osal_thread_config_t config_thread;
    int64_t                 result          = 0;
    osal_thread_handle_t    thread_handle1  = OSAL_THREAD_HANDLE_INVALID;
    osal_thread_handle_t    thread_handle2  = OSAL_THREAD_HANDLE_INVALID;

    // Test06_016_008
    // @brief: normal case: Read write register 64-bit in multi-threads.
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";

    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "Test06_016_008 failed";

    io_test_multiple_thread_worker_arg user_arg1(&device_handle, TEST_THREAD_ID2);
    user_arg1.send_msg = send_msg;
    user_arg1.mq = sync_mq;
    config_thread.userarg       = &user_arg1;
    config_thread.priority      = OSAL_THREAD_PRIORITY_TYPE3;
    config_thread.task_name     = (char *)"Read/write 64 bit value from/to device register in multi-threads";
    config_thread.func          = io_read_write_64_device_child_thread;
    config_thread.stack_size    = THREAD_STACK_SIZE;
    EXPECT_EQ(R_OSAL_ThreadCreate(&config_thread, TEST_THREAD_ID2, &thread_handle1), OSAL_RETURN_OK) << "Test06_016_008 failed";
    EXPECT_NE(thread_handle1, NULL_PTR) << "Test06_016_008 failed";
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &receive_msg, mq_config.msg_size), OSAL_RETURN_OK) << "Test06_016_008 failed";
    EXPECT_EQ(receive_msg, send_msg) << "Test06_016_008 failed";

    user_arg1.uid              = TEST_THREAD_ID3;
    EXPECT_EQ(R_OSAL_ThreadCreate(&config_thread, TEST_THREAD_ID3, &thread_handle2), OSAL_RETURN_OK) << "Test06_016_008 failed";
    EXPECT_NE(thread_handle2, NULL_PTR) << "Test06_016_008 failed";

    EXPECT_EQ(R_OSAL_ThreadSleepForTimePeriod(100), OSAL_RETURN_OK);
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "Test06_016_008 failed";

    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &result),OSAL_RETURN_OK) << "Test06_016_008 failed";
    EXPECT_EQ(result, (int64_t)OSAL_RETURN_OK) << "Test06_016_008 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle2, &result),OSAL_RETURN_OK) << "Test06_016_008 failed";
    EXPECT_EQ(result, (int64_t)OSAL_RETURN_OK) << "Test06_016_008 failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoWrite64_TC_06_016_008]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockRead8_TC_06_009_001 IOApiTest.IoBlockRead8_TC_06_009_001
 * @subsection IoBlockRead8_TC_06_009_001_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockRead8_TC_06_009_001_tci Test Case ID
 * IoBlockRead_S1_N_TC_06_009_001
 * @subsection IoBlockRead8_TC_06_009_001_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207, \#199211
 * @subsection IoBlockRead8_TC_06_009_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - "device_list" contain the device list of each "device_type" in "devices".
 * @subsection IoBlockRead8_TC_06_009_001_tcs Test Case Summary
 * Run this test when 8bit Bus is supported.<br>
 * This test shall verify the functionality of the IoBlockRead APIs of OSAL on normal condition:
 * - pre-process: open test target device, turn ON its power and clock
 * - Test06_009_001: use R_OSAL_IoBlockRead8 to read data from register of test target device with offset "TEST_READBLOCK8_REG_OFFSET_NC".
 * - post-process: close test target device
 * @subsection IoBlockRead8_TC_06_009_001_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data with offset "TEST_READBLOCK8_REG_OFFSET_NC" by using R_OSAL_IoBlockRead8
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockRead8_TC_06_009_001_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockRead8 return OSAL_RETURN_OK.<br>
 * The read data is the same as expected register data.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoBlockRead8_TC_06_009_001_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead8_TC_06_009_001
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockRead8_TC_06_009_001]
TEST_F(IOApiTest, IoBlockRead8_TC_06_009_001)
{
    uintptr_t offset;
    size_t size = 4;
    uint8_t read_data8[4];
    uint8_t register_data8[4] = {TEST_READBLOCK8_VAL_NC[0], TEST_READBLOCK8_VAL_NC[1], TEST_READBLOCK8_VAL_NC[2], TEST_READBLOCK8_VAL_NC[3]};

    for (auto it = device_list.begin(); it != device_list.end(); it++)
    {
        if (!strcmp((*it).c_str(), TEST_IO_DEVICE_1))
        {
            // pre-process: open test target device, turn ON its power and clock
            EXPECT_EQ(R_OSAL_IoDeviceOpen((*it).c_str(), &device_handle), OSAL_RETURN_OK) << "pre-process failed";
            EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
            EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

            // @brief: normal case: read register value of test target device with aligned offset.
            // Test06_009_001
            offset = TEST_READBLOCK8_REG_OFFSET_NC;
            EXPECT_EQ(R_OSAL_IoBlockRead8(device_handle, offset, read_data8, size), OSAL_RETURN_OK) << "Test06_009_001 failed";
            EXPECT_TRUE(equal(begin(register_data8), end(register_data8), begin(read_data8))) << "Test06_009_001 failed";

            // post-process: close test target device
            EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
            EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
            device_handle = OSAL_DEVICE_HANDLE_INVALID;
        }
    }
}
// [IoBlockRead8_TC_06_009_001]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockRead8_TC_06_009_003 IOApiTest.IoBlockRead8_TC_06_009_003
 * @subsection IoBlockRead8_TC_06_009_003_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockRead8_TC_06_009_003_tci Test Case ID
 * IoBlockRead_S0_A_TC_06_009_003
 * @subsection IoBlockRead8_TC_06_009_003_rr Related Requirement(s)
 * \#199151, \#199199, \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockRead8_TC_06_009_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockRead8_TC_06_009_003_tcs Test Case Summary
 * This test shall verify the functionality of the IoRead APIs of OSAL on abnormal condition:
 * - Test06_009_003: use R_OSAL_IoBlockRead8 to read register value with device handle is NULL
 * @subsection IoBlockRead8_TC_06_009_003_tpts Test Procedure/Test Steps
 * -# Read data with offset "TEST_READBLOCK8_REG_OFFSET_AC" by using R_OSAL_IoBlockRead8.
 * @subsection IoBlockRead8_TC_06_009_003_ereb Expected Result/Behavior
 * -# R_OSAL_IoBlockRead8 return OSAL_RETURN_HANDLE
 * @subsection IoBlockRead8_TC_06_009_003_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead8_TC_06_009_003
 */
// [IoBlockRead8_TC_06_009_003]
TEST_F(IOApiTest, IoBlockRead8_TC_06_009_003)
{
    uintptr_t offset;
    uint8_t read_data8[16];
    size_t size = 16;

    // @brief: abnormal case: read register value with device handle is NULL
    // Test06_009_003
    offset = TEST_READBLOCK8_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoBlockRead8(OSAL_DEVICE_HANDLE_INVALID, offset, read_data8, size), OSAL_RETURN_HANDLE) << "Test06_009_003 failed";
}
// [IoBlockRead8_TC_06_009_003]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockRead8_TC_06_009_004 IOApiTest.IoBlockRead8_TC_06_009_004
 * @subsection IoBlockRead8_TC_06_009_004_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockRead8_TC_06_009_004_tci Test Case ID
 * IoBlockRead_S0_BA_TC_06_009_004
 * @subsection IoBlockRead8_TC_06_009_004_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockRead8_TC_06_009_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockRead8_TC_06_009_004_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockRead APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_009_004: use R_OSAL_IoBlockRead8 to read register value of test target device with offset which is out-of-register-range.
 * - post-process: close test target device
 * @subsection IoBlockRead8_TC_06_009_004_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data with offset "TEST_OFFSET_OUT_RANGE" by using R_OSAL_IoBlockRead8
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockRead8_TC_06_009_004_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockRead8 return OSAL_RETURN_PAR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoBlockRead8_TC_06_009_004_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead8_TC_06_009_004
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockRead8_TC_06_009_004]
TEST_F(IOApiTest, IoBlockRead8_TC_06_009_004)
{
    uintptr_t offset;
    size_t size = 16;
    uint8_t read_data8[16];

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: read register value of test target device with offset which is out-of-register-range.
    // Test06_009_004
    offset = TEST_OFFSET_OUT_RANGE;
    EXPECT_EQ(R_OSAL_IoBlockRead8(device_handle, offset, read_data8, size), OSAL_RETURN_PAR) << "Test06_009_004 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockRead8_TC_06_009_004]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockRead8_TC_06_009_005 IOApiTest.IoBlockRead8_TC_06_009_005
 * @subsection IoBlockRead8_TC_06_009_005_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockRead8_TC_06_009_005_tci Test Case ID
 * IoBlockRead_S0_A_TC_06_009_005
 * @subsection IoBlockRead8_TC_06_009_005_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockRead8_TC_06_009_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockRead8_TC_06_009_005_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockRead APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_009_005: use R_OSAL_IoBlockRead8 to read register value of test target device with p_data is NULL
 * - post-process: close test target device
 * @subsection IoBlockRead8_TC_06_009_005_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data with offset "TEST_READBLOCK8_REG_OFFSET_AC" by using R_OSAL_IoBlockRead8
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockRead8_TC_06_009_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockRead8 return OSAL_RETURN_PAR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoBlockRead8_TC_06_009_005_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead8_TC_06_009_005
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockRead8_TC_06_009_005]
TEST_F(IOApiTest, IoBlockRead8_TC_06_009_005)
{
    uintptr_t offset;
    size_t size = 16;
    void* p_data = NULL;

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: read register value of test target device with p_data is NULL
    // Test06_009_005
    offset = TEST_READBLOCK8_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoBlockRead8(device_handle, offset, (uint8_t*)p_data, size), OSAL_RETURN_PAR) << "Test06_009_005 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockRead8_TC_06_009_005]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockRead8_TC_06_009_006 IOApiTest.IoBlockRead8_TC_06_009_006
 * @subsection IoBlockRead8_TC_06_009_006_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockRead8_TC_06_009_006_tci Test Case ID
 * IoBlockRead_S0_BA_TC_06_009_006
 * @subsection IoBlockRead8_TC_06_009_006_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockRead8_TC_06_009_006_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockRead8_TC_06_009_006_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockRead APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_009_006: use R_OSAL_IoBlockRead8 to read register value of test target device with size is out-of-range of register
 * - post-process: close test target device
 * @subsection IoBlockRead8_TC_06_009_006_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data with offset "TEST_READBLOCK8_REG_OFFSET_AC" by using R_OSAL_IoBlockRead8
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockRead8_TC_06_009_006_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockRead8 return OSAL_RETURN_PAR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoBlockRead8_TC_06_009_006_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead8_TC_06_009_006
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockRead8_TC_06_009_006]
TEST_F(IOApiTest, IoBlockRead8_TC_06_009_006)
{
    uintptr_t offset;
    size_t size = TEST_IO_SIZE_OUT_RANGE;
    uint8_t read_data8[16];

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: read register value of test target device with size is out-of-range of register
    // Test06_009_006
    offset = TEST_READBLOCK8_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoBlockRead8(device_handle, offset, read_data8, size), OSAL_RETURN_PAR) << "Test06_009_006 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockRead8_TC_06_009_006]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockRead8_TC_06_009_007 IOApiTest.IoBlockRead8_TC_06_009_007
 * @subsection IoBlockRead8_TC_06_009_007_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockRead8_TC_06_009_007_tci Test Case ID
 * IoBlockRead_S2_A_TC_06_009_007
 * @subsection IoBlockRead8_TC_06_009_007_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockRead8_TC_06_009_007_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockRead8_TC_06_009_007_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockRead APIs of OSAL on abnormal condition:
 * - Test06_009_007: use R_OSAL_IoBlockRead8 read register value with closed device handle
 * @subsection IoBlockRead8_TC_06_009_007_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * -# Read data with offset "TEST_READBLOCK8_REG_OFFSET_AC" by using R_OSAL_IoBlockRead8.
 * @subsection IoBlockRead8_TC_06_009_007_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_IoBlockRead8 return OSAL_RETURN_HANDLE
 * @subsection IoBlockRead8_TC_06_009_007_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead8_TC_06_009_007
 */
// [IoBlockRead8_TC_06_009_007]
TEST_F(IOApiTest, IoBlockRead8_TC_06_009_007)
{
    uintptr_t offset;
    size_t size = 16;
    uint8_t read_data8[16];

    // @brief: abnormal case: read register value with closed device handle
    // Test06_009_007
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "Test06_009_007 failed";
    EXPECT_NE(device_handle, NULL_PTR) << "Test06_009_007 failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "Test06_009_007 failed";

    offset = TEST_READBLOCK8_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoBlockRead8(device_handle, offset, read_data8, size), OSAL_RETURN_HANDLE) << "Test06_009_007 failed";
}
// [IoBlockRead8_TC_06_009_007]

/**
 * @page IO_TEST IO API Test
 * @section OsalApiStateTest_IoBlockRead8_TC_06_009_008 OsalApiStateTest.IoBlockRead8_TC_06_009_008
 * @subsection IoBlockRead8_TC_06_009_008_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection IoBlockRead8_TC_06_009_008_tci Test Case ID
 * IoBlockRead_S2_A_TC_06_009_008
 * @subsection IoBlockRead8_TC_06_009_008_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockRead8_TC_06_009_008_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize has already been called
 * @subsection IoBlockRead8_TC_06_009_008_tcs Test Case Summary
 * This test shall verify the state transition of the R_OSAL_IoBlockRead8 function:
 * - Test06_009_008: confirm state transition error with R_OSAL_IoBlockRead8
 * @subsection IoBlockRead8_TC_06_009_008_tpts Test Procedure/Test Steps
 * -# Confirm the state transition of R_OSAL_IoBlockRead8
 * @subsection IoBlockRead8_TC_06_009_008_ereb Expected Result/Behavior
 * -# R_OSAL_IoBlockRead8return OSAL_RETURN_STATE
 * @subsection IoBlockRead8_TC_06_009_008_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead8_TC_06_009_008
 */
// [IoBlockRead8_TC_06_009_008]
TEST_F(OsalApiStateTest, IoBlockRead8_TC_06_009_008)
{
    uint8_t b_read_data8[16];
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;
    uintptr_t offset = TEST_READBLOCK8_REG_OFFSET_AC;
    size_t size = 16;

    // Test06_009_008
    // @brief: abnormal case: state transition error with IO BlockRead
    EXPECT_EQ(R_OSAL_IoBlockRead8(device_handle, offset, b_read_data8, size), OSAL_RETURN_STATE) << "Test06_009_008 failed";
}
// [IoBlockRead8_TC_06_009_008]

/**
 * @page IO_TEST IO API Test
 * @section OSALINTTESTS_IoBlockRead8_TC_06_009_009 OSALINTTESTS.IoBlockRead8_TC_06_009_009
 * @subsection IoBlockRead8_TC_06_009_009_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection IoBlockRead8_TC_06_009_009_tci Test Case ID
 * IoBlockRead_S1_N_TC_06_009_009
 * @subsection IoBlockRead8_TC_06_009_009_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207, \#199211
 * @subsection IoBlockRead8_TC_06_009_009_pp Prerequisite/Precondition
 * -# OSAL is initialized.<br>
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection IoBlockRead8_TC_06_009_009_tcs Test Case Summary
 * Run this test when 8bit Bus is supported.
 * @subsubsection IoBlockRead8_TC_06_009_009_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Setup required memory for interrupt generation.
 * - Register callack function and enable interrupt.
 * - Wait for the interrupt callback function finish.
 * @subsubsection IoBlockRead8_TC_06_009_009_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * @subsubsection IoBlockRead8_TC_06_009_009_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality of the IO APIs which are called in the ISR
 * - Disable and clear interrupt status
 * - Test06_009_009 : Read data from test target device register by using R_OSAL_IoBlockRead with 8 bit in ISR
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection IoBlockRead8_TC_06_009_009_tpts Test Procedure/Test Steps
 * @subsubsection IoBlockRead8_TC_06_009_009_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Allocate required memory for generate interrupt from test target device and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function by using R_OSAL_InterruptRegisterIsr.<br>
 * -# Enable interrupt by using R_OSAL_InterruptEnableIsr
 * -# Setup test target device specific register to generate the interrupt by calling to setup_hw_register
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection IoBlockRead8_TC_06_009_009_post_tpts Test Procedure/Test Steps of Post-process
 * -# Unregister interrupt by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * @subsubsection IoBlockRead8_TC_06_009_009_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Read data with offset "TEST_READBLOCK8_REG_OFFSET_INT" by using R_OSAL_IoBlockRead8
 * -# Send Message interrupt of context finished by using R_OSAL_MqSendForTimePeriod
 * @subsection IoBlockRead8_TC_06_009_009_ereb Expected Result/Behavior
 * @subsubsection IoBlockRead8_TC_06_009_009_pre_ereb Expected Result/Behavior of Pre-process
 * -# allocate_setup_data_memory run successfully
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * The value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK
 * -# setup_hw_register run successfully
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection IoBlockRead8_TC_06_009_009_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * @subsubsection IoBlockRead8_TC_06_009_009_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_IoBlockRead8 return OSAL_RETURN_OK.<br>
 * The read data is the same as expected register data.
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * @subsection IoBlockRead8_TC_06_009_009_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead8_TC_06_009_009
 * @snippet test_io.cpp osal_io_isr_test_TC_06_009_009
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [IoBlockRead8_TC_06_009_009]
TEST_F(OSALINTTESTS, IoBlockRead8_TC_06_009_009)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    st_io_arg_t io_arg;
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
 * Wait for the interrupt callback function finish. */
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_GT(numOfChannel, 0) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_io_isr_test_TC_06_009_009, (void *)&arg), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK) << "pre-process failed";
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory*/
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_io_isr_test_TC_06_009_009), OSAL_RETURN_OK) << "post-process failed";
    deallocate_data_memory(hndl_mmngr, buffer_obj);
}
// [IoBlockRead8_TC_06_009_009]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockRead8_TC_06_009_010 IOApiTest.IoBlockRead8_TC_06_009_010
 * @subsection IoBlockRead8_TC_06_009_010_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockRead8_TC_06_009_010_tci Test Case ID
 * IoBlockRead_S1_N_TC_06_009_010
 * @subsection IoBlockRead8_TC_06_009_010_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207, \#199211
 * @subsection IoBlockRead8_TC_06_009_010_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockRead8_TC_06_009_010_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockRead APIs of OSAL on normal condition:
 * - pre-process: open test target device, turn ON its power and clock
 * - Test06_009_010: use R_OSAL_IoBlockRead8 to read data from register of test target device with offset "TEST_READBLOCK8_REG_OFFSET_NC" and size is 0.
 * - post-process: turn OFF clock and close test target device
 * @subsection IoBlockRead8_TC_06_009_010_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data with offset "TEST_READBLOCK8_REG_OFFSET_NC" and size is 0 by using R_OSAL_IoBlockRead8
 * -# Set the clock of test target device status to "OFF":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockRead8_TC_06_009_010_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockRead8 return OSAL_RETURN_OK.<br>
 * The value of "read_data8" is equal to 0.
 * -# Set clock to "OFF" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoBlockRead8_TC_06_009_010_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead8_TC_06_009_010
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockRead8_TC_06_009_010]
TEST_F(IOApiTest, IoBlockRead8_TC_06_009_010)
{
    uint8_t read_data8 = 0U;

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: normal case: read register value of test target device with size is 0.
    // Test06_009_010
    EXPECT_EQ(R_OSAL_IoBlockRead8(device_handle, TEST_READBLOCK8_REG_OFFSET_NC, &read_data8, TEST_IO_SIZE_ZERO), OSAL_RETURN_OK) << "Test06_009_010 failed";
    EXPECT_EQ(read_data8, 0) << "Test06_009_010 failed";

    // post-process: turn OFF clock and close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockRead8_TC_06_009_010]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockRead16_TC_06_010_001 IOApiTest.IoBlockRead16_TC_06_010_001
 * @subsection IoBlockRead16_TC_06_010_001_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockRead16_TC_06_010_001_tci Test Case ID
 * IoBlockRead_S1_N_TC_06_010_001
 * @subsection IoBlockRead16_TC_06_010_001_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207, \#199211
 * @subsection IoBlockRead16_TC_06_010_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - "device_list" contain the device list of each "device_type" in "devices".
 * @subsection IoBlockRead16_TC_06_010_001_tcs Test Case Summary
 * Run this test when 16bit Bus is supported.<br>
 * This test shall verify the functionality of the IoBlockRead APIs of OSAL on normal condition:
 * - pre-process: open test target device, turn ON its power and clock
 * - Test06_010_001: use R_OSAL_IoBlockRead16 to read data from register of test target device with offset "TEST_READBLOCK16_REG_OFFSET_NC".
 * - post-process: close test target device
 * @subsection IoBlockRead16_TC_06_010_001_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data with offset "TEST_READBLOCK16_REG_OFFSET_NC" by using R_OSAL_IoBlockRead16
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockRead16_TC_06_010_001_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockRead16 return OSAL_RETURN_OK.<br>
 * The read data is the same as expected register data.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoBlockRead16_TC_06_010_001_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead16_TC_06_010_001
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockRead16_TC_06_010_001]
TEST_F(IOApiTest, IoBlockRead16_TC_06_010_001)
{
    uintptr_t offset;
    size_t size = 4;
    uint16_t read_data16[2];
    uint16_t register_data16[2] = {TEST_READBLOCK16_VAL_NC[0], TEST_READBLOCK16_VAL_NC[1]};

    for (auto it = device_list.begin(); it != device_list.end(); it++)
    {
        if (!strcmp((*it).c_str(), TEST_IO_DEVICE_1))
        {
            // pre-process: open test target device, turn ON its power and clock
            EXPECT_EQ(R_OSAL_IoDeviceOpen((*it).c_str(), &device_handle), OSAL_RETURN_OK) << "pre-process failed";
            EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
            EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

            // @brief: normal case: read register value of test target device with aligned offset.
            // Test06_010_001
            offset = TEST_READBLOCK16_REG_OFFSET_NC;
            EXPECT_EQ(R_OSAL_IoBlockRead16(device_handle, offset, read_data16, size), OSAL_RETURN_OK) << "Test06_010_001 failed";
            EXPECT_TRUE(equal(begin(register_data16), end(register_data16), begin(read_data16))) << "Test06_010_001 failed";

            // post-process: close test target device
            EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
            EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
            device_handle = OSAL_DEVICE_HANDLE_INVALID;
        }
    }
}
// [IoBlockRead16_TC_06_010_001]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockRead16_TC_06_010_002 IOApiTest.IoBlockRead16_TC_06_010_002
 * @subsection IoBlockRead16_TC_06_010_002_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockRead16_TC_06_010_002_tci Test Case ID
 * IoBlockRead_S0_A_TC_06_010_002
 * @subsection IoBlockRead16_TC_06_010_002_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockRead16_TC_06_010_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockRead16_TC_06_010_002_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockRead APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_010_002: use R_OSAL_IoBlockRead16 to read register value of test target device with un-aligned offset.
 * - post-process: close test target device
 * @subsection IoBlockRead16_TC_06_010_002_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data with offset "TEST_READBLOCK16_OFFSET_UNALIGNED" by using R_OSAL_IoBlockRead16
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockRead16_TC_06_010_002_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockRead16 return OSAL_RETURN_PAR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoBlockRead16_TC_06_010_002_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead16_TC_06_010_002
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockRead16_TC_06_010_002]
TEST_F(IOApiTest, IoBlockRead16_TC_06_010_002)
{
    uintptr_t offset;
    size_t size = 16;
    uint16_t read_data16[8];

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: read register value of test target device with un-aligned offset.
    // Test06_010_002
    offset = TEST_READBLOCK16_OFFSET_UNALIGNED;
    EXPECT_EQ(R_OSAL_IoBlockRead16(device_handle, offset, read_data16, size), OSAL_RETURN_PAR) << "Test06_010_002 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockRead16_TC_06_010_002]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockRead16_TC_06_010_003 IOApiTest.IoBlockRead16_TC_06_010_003
 * @subsection IoBlockRead16_TC_06_010_003_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockRead16_TC_06_010_003_tci Test Case ID
 * IoBlockRead_S0_A_TC_06_010_003
 * @subsection IoBlockRead16_TC_06_010_003_rr Related Requirement(s)
 * \#199151, \#199199, \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockRead16_TC_06_010_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockRead16_TC_06_010_003_tcs Test Case Summary
 * This test shall verify the functionality of the IoRead APIs of OSAL on abnormal condition:
 * - Test06_010_003: use R_OSAL_IoBlockRead16 to read register value with device handle is NULL
 * @subsection IoBlockRead16_TC_06_010_003_tpts Test Procedure/Test Steps
 * -# Read data with offset "TEST_READBLOCK16_REG_OFFSET_AC" by using R_OSAL_IoBlockRead16.
 * @subsection IoBlockRead16_TC_06_010_003_ereb Expected Result/Behavior
 * -# R_OSAL_IoBlockRead16 return OSAL_RETURN_HANDLE
 * @subsection IoBlockRead16_TC_06_010_003_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead16_TC_06_010_003
 */
// [IoBlockRead16_TC_06_010_003]
TEST_F(IOApiTest, IoBlockRead16_TC_06_010_003)
{
    uintptr_t offset;
    uint16_t read_data16[8];
    size_t size = 16;

    // @brief: abnormal case: read register value with device handle is NULL
    // Test06_010_003
    offset = TEST_READBLOCK16_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoBlockRead16(OSAL_DEVICE_HANDLE_INVALID, offset, read_data16, size), OSAL_RETURN_HANDLE) << "Test06_010_003 failed";
}
// [IoBlockRead16_TC_06_010_003]


/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockRead16_TC_06_010_004 IOApiTest.IoBlockRead16_TC_06_010_004
 * @subsection IoBlockRead16_TC_06_010_004_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockRead16_TC_06_010_004_tci Test Case ID
 * IoBlockRead_S0_BA_TC_06_010_004
 * @subsection IoBlockRead16_TC_06_010_004_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockRead16_TC_06_010_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockRead16_TC_06_010_004_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockRead APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_010_004: use R_OSAL_IoBlockRead16 to read register value of test target device with offset which is out-of-register-range.
 * - post-process: close test target device
 * @subsection IoBlockRead16_TC_06_010_004_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data with offset "TEST_OFFSET_OUT_RANGE" by using R_OSAL_IoBlockRead16
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockRead16_TC_06_010_004_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockRead16 return OSAL_RETURN_PAR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoBlockRead16_TC_06_010_004_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead16_TC_06_010_004
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockRead16_TC_06_010_004]
TEST_F(IOApiTest, IoBlockRead16_TC_06_010_004)
{
    uintptr_t offset;
    size_t size = 16;
    uint16_t read_data16[8];

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: read register value of test target device with offset which is out-of-register-range.
    // Test06_010_004
    offset = TEST_OFFSET_OUT_RANGE;
    EXPECT_EQ(R_OSAL_IoBlockRead16(device_handle, offset, read_data16, size), OSAL_RETURN_PAR) << "Test06_010_004 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockRead16_TC_06_010_004]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockRead16_TC_06_010_005 IOApiTest.IoBlockRead16_TC_06_010_005
 * @subsection IoBlockRead16_TC_06_010_005_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockRead16_TC_06_010_005_tci Test Case ID
 * IoBlockRead_S0_A_TC_06_010_005
 * @subsection IoBlockRead16_TC_06_010_005_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockRead16_TC_06_010_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockRead16_TC_06_010_005_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockRead APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_010_005: use R_OSAL_IoBlockRead16 to read register value of test target device with p_data is NULL
 * - post-process: close test target device
 * @subsection IoBlockRead16_TC_06_010_005_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data with offset "TEST_READBLOCK16_REG_OFFSET_AC" by using R_OSAL_IoBlockRead16
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockRead16_TC_06_010_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockRead16 return OSAL_RETURN_PAR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoBlockRead16_TC_06_010_005_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead16_TC_06_010_005
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockRead16_TC_06_010_005]
TEST_F(IOApiTest, IoBlockRead16_TC_06_010_005)
{
    uintptr_t offset;
    size_t size = 16;
    void* p_data = NULL;

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: read register value of test target device with p_data is NULL
    // Test06_010_005
    offset = TEST_READBLOCK16_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoBlockRead16(device_handle, offset, (uint16_t*)p_data, size), OSAL_RETURN_PAR) << "Test06_010_005 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockRead16_TC_06_010_005]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockRead16_TC_06_010_006 IOApiTest.IoBlockRead16_TC_06_010_006
 * @subsection IoBlockRead16_TC_06_010_006_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockRead16_TC_06_010_006_tci Test Case ID
 * IoBlockRead_S0_BA_TC_06_010_006
 * @subsection IoBlockRead16_TC_06_010_006_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockRead16_TC_06_010_006_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockRead16_TC_06_010_006_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockRead APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_010_006: use R_OSAL_IoBlockRead16 to read register value of test target device with size is out-of-range of register
 * - post-process: close test target device
 * @subsection IoBlockRead16_TC_06_010_006_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data with offset "TEST_READBLOCK16_REG_OFFSET_AC" by using R_OSAL_IoBlockRead16
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockRead16_TC_06_010_006_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockRead16 return OSAL_RETURN_PAR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoBlockRead16_TC_06_010_006_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead16_TC_06_010_006
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockRead16_TC_06_010_006]
TEST_F(IOApiTest, IoBlockRead16_TC_06_010_006)
{
    uintptr_t offset;
    size_t size = TEST_IO_SIZE_OUT_RANGE;
    uint16_t read_data16[8];

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: read register value of test target device with size is out-of-range of register
    // Test06_010_006
    offset = TEST_READBLOCK16_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoBlockRead16(device_handle, offset, read_data16, size), OSAL_RETURN_PAR) << "Test06_010_006 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockRead16_TC_06_010_006]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockRead16_TC_06_010_007 IOApiTest.IoBlockRead16_TC_06_010_007
 * @subsection IoBlockRead16_TC_06_010_007_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockRead16_TC_06_010_007_tci Test Case ID
 * IoBlockRead_S2_A_TC_06_010_007
 * @subsection IoBlockRead16_TC_06_010_007_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockRead16_TC_06_010_007_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockRead16_TC_06_010_007_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockRead APIs of OSAL on abnormal condition:
 * - Test06_010_007: use R_OSAL_IoBlockRead16 read register value with closed device handle
 * @subsection IoBlockRead16_TC_06_010_007_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * -# Read data with offset "TEST_READBLOCK16_REG_OFFSET_AC" by using R_OSAL_IoBlockRead16.
 * @subsection IoBlockRead16_TC_06_010_007_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_IoBlockRead16 return OSAL_RETURN_HANDLE
 * @subsection IoBlockRead16_TC_06_010_007_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead16_TC_06_010_007
 */
// [IoBlockRead16_TC_06_010_007]
TEST_F(IOApiTest, IoBlockRead16_TC_06_010_007)
{
    uintptr_t offset;
    size_t size = 16;
    uint16_t read_data16[8];

    // @brief: abnormal case: read register value with closed device handle
    // Test06_010_007
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "Test06_010_007 failed";
    EXPECT_NE(device_handle, NULL_PTR) << "Test06_010_007 failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "Test06_010_007 failed";

    offset = TEST_READBLOCK16_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoBlockRead16(device_handle, offset, read_data16, size), OSAL_RETURN_HANDLE) << "Test06_010_007 failed";
}
// [IoBlockRead16_TC_06_010_007]

/**
 * @page IO_TEST IO API Test
 * @section OsalApiStateTest_IoBlockRead16_TC_06_010_008 OsalApiStateTest.IoBlockRead16_TC_06_010_008
 * @subsection IoBlockRead16_TC_06_010_008_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection IoBlockRead16_TC_06_010_008_tci Test Case ID
 * IoBlockRead_S2_A_TC_06_010_008
 * @subsection IoBlockRead16_TC_06_010_008_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockRead16_TC_06_010_008_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize has already been called
 * @subsection IoBlockRead16_TC_06_010_008_tcs Test Case Summary
 * This test shall verify the state transition of the R_OSAL_IoBlockRead16 function:
 * - Test06_010_008: confirm state transition error with R_OSAL_IoBlockRead16
 * @subsection IoBlockRead16_TC_06_010_008_tpts Test Procedure/Test Steps
 * -# Confirm the state transition of R_OSAL_IoBlockRead16
 * @subsection IoBlockRead16_TC_06_010_008_ereb Expected Result/Behavior
 * -# R_OSAL_IoBlockRead16return OSAL_RETURN_STATE
 * @subsection IoBlockRead16_TC_06_010_008_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead16_TC_06_010_008
 */
// [IoBlockRead16_TC_06_010_008]
TEST_F(OsalApiStateTest, IoBlockRead16_TC_06_010_008)
{
    uint16_t b_read_data16[8];
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;
    uintptr_t offset = TEST_READBLOCK16_REG_OFFSET_AC;
    size_t size = 16;

    // Test06_010_008
    // @brief: abnormal case: state transition error with IO BlockRead
    EXPECT_EQ(R_OSAL_IoBlockRead16(device_handle, offset, b_read_data16, size), OSAL_RETURN_STATE) << "Test06_010_008 failed";
}
// [IoBlockRead16_TC_06_010_008]

/**
 * @page IO_TEST IO API Test
 * @section OSALINTTESTS_IoBlockRead16_TC_06_010_009 OSALINTTESTS.IoBlockRead16_TC_06_010_009
 * @subsection IoBlockRead16_TC_06_010_009_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection IoBlockRead16_TC_06_010_009_tci Test Case ID
 * IoBlockRead_S1_N_TC_06_010_009
 * @subsection IoBlockRead16_TC_06_010_009_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207, \#199211
 * @subsection IoBlockRead16_TC_06_010_009_pp Prerequisite/Precondition
 * -# OSAL is initialized.<br>
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection IoBlockRead16_TC_06_010_009_tcs Test Case Summary
 * Run this test when 16bit Bus is supported.
 * @subsubsection IoBlockRead16_TC_06_010_009_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Setup required memory for interrupt generation.
 * - Register callack function and enable interrupt.
 * - Wait for the interrupt callback function finish.
 * @subsubsection IoBlockRead16_TC_06_010_009_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * @subsubsection IoBlockRead16_TC_06_010_009_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality of the IO APIs which are called in the ISR
 * - Disable and clear interrupt status
 * - Test06_010_009: use R_OSAL_IoBlockRead with 16 bit
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection IoBlockRead16_TC_06_010_009_tpts Test Procedure/Test Steps
 * @subsubsection IoBlockRead16_TC_06_010_009_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Allocate required memory for generate interrupt from test target device and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function by using R_OSAL_InterruptRegisterIsr.<br>
 * -# Enable interrupt by using R_OSAL_InterruptEnableIsr
 * -# Setup test target device specific register to generate the interrupt by calling to setup_hw_register
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection IoBlockRead16_TC_06_010_009_post_tpts Test Procedure/Test Steps of Post-process
 * -# Unregister interrupt by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * @subsubsection IoBlockRead16_TC_06_010_009_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Read data with offset "TEST_READBLOCK16_REG_OFFSET_INT" by using R_OSAL_IoBlockRead16
 * -# Send Message interrupt of context finished by using R_OSAL_MqSendForTimePeriod
 * @subsection IoBlockRead16_TC_06_010_009_ereb Expected Result/Behavior
 * @subsubsection IoBlockRead16_TC_06_010_009_pre_ereb Expected Result/Behavior of Pre-process
 * -# allocate_setup_data_memory run successfully
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * The value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK
 * -# setup_hw_register run successfully
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection IoBlockRead16_TC_06_010_009_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * @subsubsection IoBlockRead16_TC_06_010_009_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_IoBlockRead16 return OSAL_RETURN_OK.<br>
 * The read data is the same as expected register data.
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * @subsection IoBlockRead16_TC_06_010_009_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead16_TC_06_010_009
 * @snippet test_io.cpp osal_io_isr_test_TC_06_010_009
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [IoBlockRead16_TC_06_010_009]
TEST_F(OSALINTTESTS, IoBlockRead16_TC_06_010_009)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    st_io_arg_t io_arg;
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
 * Wait for the interrupt callback function finish. */
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_GT(numOfChannel, 0) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_io_isr_test_TC_06_010_009, (void *)&arg), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK) << "pre-process failed";
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory*/
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_io_isr_test_TC_06_010_009), OSAL_RETURN_OK) << "post-process failed";
    deallocate_data_memory(hndl_mmngr, buffer_obj);
}
// [IoBlockRead16_TC_06_010_009]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockRead16_TC_06_010_010 IOApiTest.IoBlockRead16_TC_06_010_010
 * @subsection IoBlockRead16_TC_06_010_010_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockRead16_TC_06_010_010_tci Test Case ID
 * IoBlockRead_S1_N_TC_06_010_010
 * @subsection IoBlockRead16_TC_06_010_010_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207, \#199211
 * @subsection IoBlockRead16_TC_06_010_010_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockRead16_TC_06_010_010_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockRead APIs of OSAL on normal condition:
 * - pre-process: open test target device, turn ON its power and clock
 * - Test06_010_010: use R_OSAL_IoBlockRead16 to read data from register of test target device with offset "TEST_READBLOCK16_REG_OFFSET_NC" and size is 0.
 * - post-process: turn OFF clock and close test target device
 * @subsection IoBlockRead16_TC_06_010_010_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data with offset "TEST_READBLOCK16_REG_OFFSET_NC" and size is 0 by using R_OSAL_IoBlockRead16
 * -# Set the clock of test target device status to "OFF":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockRead16_TC_06_010_010_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockRead16 return OSAL_RETURN_OK.<br>
 * The value of "read_data16" is equal to 0.
 * -# Set clock to "OFF" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoBlockRead16_TC_06_010_010_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead16_TC_06_010_010
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockRead16_TC_06_010_010]
TEST_F(IOApiTest, IoBlockRead16_TC_06_010_010)
{
    uint16_t read_data16 = 0U;

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: normal case: read register value of test target device with size is 0.
    // Test06_010_010
    EXPECT_EQ(R_OSAL_IoBlockRead16(device_handle, TEST_READBLOCK16_REG_OFFSET_NC, &read_data16, TEST_IO_SIZE_ZERO), OSAL_RETURN_OK) << "Test06_010_010 failed";
    EXPECT_EQ(read_data16, 0) << "Test06_010_010 failed";

    // post-process: turn OFF clock and close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockRead16_TC_06_010_010]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockRead32_TC_06_011_001 IOApiTest.IoBlockRead32_TC_06_011_001
 * @subsection IoBlockRead32_TC_06_011_001_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockRead32_TC_06_011_001_tci Test Case ID
 * IoBlockRead_S1_N_TC_06_011_001
 * @subsection IoBlockRead32_TC_06_011_001_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207, \#199211
 * @subsection IoBlockRead32_TC_06_011_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - "device_list" contain the device list of each "device_type" in "devices".
 * @subsection IoBlockRead32_TC_06_011_001_tcs Test Case Summary
 * Run this test when 32bit Bus is supported.<br>
 * This test shall verify the functionality of the IoBlockRead APIs of OSAL on normal condition:
 * - pre-process: open test target device, turn ON its power and clock
 * - Test06_011_001: use R_OSAL_IoBlockRead32 to read data from register of test target device with offset "TEST_READBLOCK32_REG_OFFSET_NC".
 * - post-process: close test target device
 * @subsection IoBlockRead32_TC_06_011_001_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data with offset "TEST_READBLOCK32_REG_OFFSET_NC" by using R_OSAL_IoBlockRead32
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockRead32_TC_06_011_001_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockRead32 return OSAL_RETURN_OK.<br>
 * The read data is the same as expected register data.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoBlockRead32_TC_06_011_001_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead32_TC_06_011_001
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockRead32_TC_06_011_001]
TEST_F(IOApiTest, IoBlockRead32_TC_06_011_001)
{
    uintptr_t offset;
    size_t size = 8;
    uint32_t read_data32[2];
    uint32_t register_data32[2] = {TEST_READBLOCK32_VAL_NC[0], TEST_READBLOCK32_VAL_NC[1]};

    for (auto it = device_list.begin(); it != device_list.end(); it++)
    {
        if (!strcmp((*it).c_str(), TEST_IO_DEVICE_1))
        {
            // pre-process: open test target device, turn ON its power and clock
            EXPECT_EQ(R_OSAL_IoDeviceOpen((*it).c_str(), &device_handle), OSAL_RETURN_OK) << "pre-process failed";
            EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
            EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

            // @brief: normal case: read register value of test target device with aligned offset.
            // Test06_011_001
            offset = TEST_READBLOCK32_REG_OFFSET_NC;
            EXPECT_EQ(R_OSAL_IoBlockRead32(device_handle, offset, read_data32, size), OSAL_RETURN_OK) << "Test06_011_001 failed";
            read_data32[0] = read_data32[0] & 0xFFFFFFFF;
            read_data32[1] = read_data32[1] & 0x00000000;
            EXPECT_TRUE(equal(begin(register_data32), end(register_data32), begin(read_data32))) << "Test06_011_001 failed";

            // post-process: close test target device
            EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
            EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
            device_handle = OSAL_DEVICE_HANDLE_INVALID;
        }
    }
}
// [IoBlockRead32_TC_06_011_001]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockRead32_TC_06_011_002 IOApiTest.IoBlockRead32_TC_06_011_002
 * @subsection IoBlockRead32_TC_06_011_002_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockRead32_TC_06_011_002_tci Test Case ID
 * IoBlockRead_S0_A_TC_06_011_002
 * @subsection IoBlockRead32_TC_06_011_002_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockRead32_TC_06_011_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockRead32_TC_06_011_002_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockRead APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_011_002: use R_OSAL_IoBlockRead32 to read register value of test target device with un-aligned offset.
 * - post-process: close test target device
 * @subsection IoBlockRead32_TC_06_011_002_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data with offset "TEST_READBLOCK32_OFFSET_UNALIGNED" by using R_OSAL_IoBlockRead32
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockRead32_TC_06_011_002_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockRead32 return OSAL_RETURN_PAR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoBlockRead32_TC_06_011_002_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead32_TC_06_011_002
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockRead32_TC_06_011_002]
TEST_F(IOApiTest, IoBlockRead32_TC_06_011_002)
{
    uintptr_t offset;
    size_t size = 16;
    uint32_t read_data32[4];

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: read register value of test target device with un-aligned offset.
    // Test06_011_002
    offset = TEST_READBLOCK32_OFFSET_UNALIGNED;
    EXPECT_EQ(R_OSAL_IoBlockRead32(device_handle, offset, read_data32, size), OSAL_RETURN_PAR) << "Test06_011_002 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockRead32_TC_06_011_002]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockRead32_TC_06_011_003 IOApiTest.IoBlockRead32_TC_06_011_003
 * @subsection IoBlockRead32_TC_06_011_003_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockRead32_TC_06_011_003_tci Test Case ID
 * IoBlockRead_S0_A_TC_06_011_003
 * @subsection IoBlockRead32_TC_06_011_003_rr Related Requirement(s)
 * \#199151, \#199199, \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockRead32_TC_06_011_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockRead32_TC_06_011_003_tcs Test Case Summary
 * This test shall verify the functionality of the IoRead APIs of OSAL on abnormal condition:
 * - Test06_011_003: use R_OSAL_IoBlockRead32 to read register value with device handle is NULL
 * @subsection IoBlockRead32_TC_06_011_003_tpts Test Procedure/Test Steps
 * -# Read data with offset "TEST_READBLOCK32_REG_OFFSET_AC" by using R_OSAL_IoBlockRead32.
 * @subsection IoBlockRead32_TC_06_011_003_ereb Expected Result/Behavior
 * -# R_OSAL_IoBlockRead32 return OSAL_RETURN_HANDLE
 * @subsection IoBlockRead32_TC_06_011_003_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead32_TC_06_011_003
 */
// [IoBlockRead32_TC_06_011_003]
TEST_F(IOApiTest, IoBlockRead32_TC_06_011_003)
{
    uintptr_t offset;
    uint32_t read_data32[4];
    size_t size = 16;

    // @brief: abnormal case: read register value with device handle is NULL
    // Test06_011_003
    offset = TEST_READBLOCK32_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoBlockRead32(OSAL_DEVICE_HANDLE_INVALID, offset, read_data32, size), OSAL_RETURN_HANDLE) << "Test06_011_003 failed";
}
// [IoBlockRead32_TC_06_011_003]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockRead32_TC_06_011_004 IOApiTest.IoBlockRead32_TC_06_011_004
 * @subsection IoBlockRead32_TC_06_011_004_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockRead32_TC_06_011_004_tci Test Case ID
 * IoBlockRead_S0_BA_TC_06_011_004
 * @subsection IoBlockRead32_TC_06_011_004_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockRead32_TC_06_011_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockRead32_TC_06_011_004_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockRead APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_011_004: use R_OSAL_IoBlockRead32 to read register value of test target device with offset which is out-of-register-range.
 * - post-process: close test target device
 * @subsection IoBlockRead32_TC_06_011_004_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data with offset "TEST_OFFSET_OUT_RANGE" by using R_OSAL_IoBlockRead32
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockRead32_TC_06_011_004_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockRead32 return OSAL_RETURN_PAR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoBlockRead32_TC_06_011_004_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead32_TC_06_011_004
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockRead32_TC_06_011_004]
TEST_F(IOApiTest, IoBlockRead32_TC_06_011_004)
{
    uintptr_t offset;
    size_t size = 16;
    uint32_t read_data32[4];

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: read register value of test target device with offset which is out-of-register-range.
    // Test06_011_004
    offset = TEST_OFFSET_OUT_RANGE;
    EXPECT_EQ(R_OSAL_IoBlockRead32(device_handle, offset, read_data32, size), OSAL_RETURN_PAR) << "Test06_011_004 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockRead32_TC_06_011_004]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockRead32_TC_06_011_005 IOApiTest.IoBlockRead32_TC_06_011_005
 * @subsection IoBlockRead32_TC_06_011_005_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockRead32_TC_06_011_005_tci Test Case ID
 * IoBlockRead_S0_A_TC_06_011_005
 * @subsection IoBlockRead32_TC_06_011_005_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockRead32_TC_06_011_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockRead32_TC_06_011_005_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockRead APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_011_005: use R_OSAL_IoBlockRead32 to read register value of test target device with p_data is NULL
 * - post-process: close test target device
 * @subsection IoBlockRead32_TC_06_011_005_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data with offset "TEST_READBLOCK32_REG_OFFSET_AC" by using R_OSAL_IoBlockRead32
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockRead32_TC_06_011_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockRead32 return OSAL_RETURN_PAR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoBlockRead32_TC_06_011_005_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead32_TC_06_011_005
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockRead32_TC_06_011_005]
TEST_F(IOApiTest, IoBlockRead32_TC_06_011_005)
{
    uintptr_t offset;
    size_t size = 16;
    void* p_data = NULL;

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: read register value of test target device with p_data is NULL
    // Test06_011_005
    offset = TEST_READBLOCK32_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoBlockRead32(device_handle, offset, (uint32_t*)p_data, size), OSAL_RETURN_PAR) << "Test06_011_005 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockRead32_TC_06_011_005]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockRead32_TC_06_011_006 IOApiTest.IoBlockRead32_TC_06_011_006
 * @subsection IoBlockRead32_TC_06_011_006_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockRead32_TC_06_011_006_tci Test Case ID
 * IoBlockRead_S0_BA_TC_06_011_006
 * @subsection IoBlockRead32_TC_06_011_006_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockRead32_TC_06_011_006_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockRead32_TC_06_011_006_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockRead APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_011_006: use R_OSAL_IoBlockRead32 to read register value of test target device with size is out-of-range of register
 * - post-process: close test target device
 * @subsection IoBlockRead32_TC_06_011_006_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data with offset "TEST_READBLOCK32_REG_OFFSET_AC" by using R_OSAL_IoBlockRead32
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockRead32_TC_06_011_006_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockRead32 return OSAL_RETURN_PAR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoBlockRead32_TC_06_011_006_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead32_TC_06_011_006
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockRead32_TC_06_011_006]
TEST_F(IOApiTest, IoBlockRead32_TC_06_011_006)
{
    uintptr_t offset;
    size_t size = TEST_IO_SIZE_OUT_RANGE;
    uint32_t read_data32[4];

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: read register value of test target device with size is out-of-range of register
    // Test06_011_006
    offset = TEST_READBLOCK32_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoBlockRead32(device_handle, offset, read_data32, size), OSAL_RETURN_PAR) << "Test06_011_006 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockRead32_TC_06_011_006]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockRead32_TC_06_011_007 IOApiTest.IoBlockRead32_TC_06_011_007
 * @subsection IoBlockRead32_TC_06_011_007_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockRead32_TC_06_011_007_tci Test Case ID
 * IoBlockRead_S2_A_TC_06_011_007
 * @subsection IoBlockRead32_TC_06_011_007_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockRead32_TC_06_011_007_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockRead32_TC_06_011_007_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockRead APIs of OSAL on abnormal condition:
 * - Test06_011_007: use R_OSAL_IoBlockRead32 read register value with closed device handle
 * @subsection IoBlockRead32_TC_06_011_007_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * -# Read data with offset "TEST_READBLOCK32_REG_OFFSET_AC" by using R_OSAL_IoBlockRead32.
 * @subsection IoBlockRead32_TC_06_011_007_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_IoBlockRead32 return OSAL_RETURN_HANDLE
 * @subsection IoBlockRead32_TC_06_011_007_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead32_TC_06_011_007
 */
// [IoBlockRead32_TC_06_011_007]
TEST_F(IOApiTest, IoBlockRead32_TC_06_011_007)
{
    uintptr_t offset;
    size_t size = 16;
    uint32_t read_data32[4];

    // @brief: abnormal case: read register value with closed device handle
    // Test06_011_007
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "Test06_011_007 failed";
    EXPECT_NE(device_handle, NULL_PTR) << "Test06_011_007 failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "Test06_011_007 failed";

    offset = TEST_READBLOCK32_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoBlockRead32(device_handle, offset, read_data32, size), OSAL_RETURN_HANDLE) << "Test06_011_007 failed";
}
// [IoBlockRead32_TC_06_011_007]

/**
 * @page IO_TEST IO API Test
 * @section OsalApiStateTest_IoBlockRead32_TC_06_011_008 OsalApiStateTest.IoBlockRead32_TC_06_011_008
 * @subsection IoBlockRead32_TC_06_011_008_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection IoBlockRead32_TC_06_011_008_tci Test Case ID
 * IoBlockRead_S2_A_TC_06_011_008
 * @subsection IoBlockRead32_TC_06_011_008_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockRead32_TC_06_011_008_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize has already been called
 * @subsection IoBlockRead32_TC_06_011_008_tcs Test Case Summary
 * This test shall verify the state transition of the R_OSAL_IoBlockRead32 function:
 * - Test06_011_008: confirm state transition error with R_OSAL_IoBlockRead32
 * @subsection IoBlockRead32_TC_06_011_008_tpts Test Procedure/Test Steps
 * -# Confirm the state transition of R_OSAL_IoBlockRead32
 * @subsection IoBlockRead32_TC_06_011_008_ereb Expected Result/Behavior
 * -# R_OSAL_IoBlockRead32return OSAL_RETURN_STATE
 * @subsection IoBlockRead32_TC_06_011_008_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead32_TC_06_011_008
 */
// [IoBlockRead32_TC_06_011_008]
TEST_F(OsalApiStateTest, IoBlockRead32_TC_06_011_008)
{
    uint32_t b_read_data32[4];
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;
    uintptr_t offset = TEST_READBLOCK32_REG_OFFSET_AC;
    size_t size = 16;

    // Test06_011_008
    // @brief: abnormal case: state transition error with IO BlockRead
    EXPECT_EQ(R_OSAL_IoBlockRead32(device_handle, offset, b_read_data32, size), OSAL_RETURN_STATE) << "Test06_011_008 failed";
}
// [IoBlockRead32_TC_06_011_008]

/**
 * @page IO_TEST IO API Test
 * @section OSALINTTESTS_IoBlockRead32_TC_06_011_009 OSALINTTESTS.IoBlockRead32_TC_06_011_009
 * @subsection IoBlockRead32_TC_06_011_009_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection IoBlockRead32_TC_06_011_009_tci Test Case ID
 * IoBlockRead_S1_N_TC_06_011_009
 * @subsection IoBlockRead32_TC_06_011_009_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207, \#199211
 * @subsection IoBlockRead32_TC_06_011_009_pp Prerequisite/Precondition
 * -# OSAL is initialized.<br>
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection IoBlockRead32_TC_06_011_009_tcs Test Case Summary
 * @subsubsection IoBlockRead32_TC_06_011_009_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Setup required memory for interrupt generation.
 * - Register callack function and enable interrupt.
 * - Wait for the interrupt callback function finish.
 * @subsubsection IoBlockRead32_TC_06_011_009_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * @subsubsection IoBlockRead32_TC_06_011_009_cbf_tcs Test Case Summary in Interrupt Callback Function
 * Run this test when 32bit Bus is supported<br>
 * This test shall verify the functionality of the IO APIs which are called in the ISR
 * - Disable and clear interrupt status
 * - Test06_011_009 : use R_OSAL_IoBlockRead with 32 bit
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection IoBlockRead32_TC_06_011_009_tpts Test Procedure/Test Steps
 * @subsubsection IoBlockRead32_TC_06_011_009_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Allocate required memory for generate interrupt from test target device and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function by using R_OSAL_InterruptRegisterIsr.<br>
 * -# Enable interrupt by using R_OSAL_InterruptEnableIsr
 * -# Setup test target device specific register to generate the interrupt by calling to setup_hw_register
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection IoBlockRead32_TC_06_011_009_post_tpts Test Procedure/Test Steps of Post-process
 * -# Unregister interrupt by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * @subsubsection IoBlockRead32_TC_06_011_009_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Read data with offset "TEST_READBLOCK32_REG_OFFSET_INT" by using R_OSAL_IoBlockRead32
 * -# Send Message interrupt of context finished by using R_OSAL_MqSendForTimePeriod
 * @subsection IoBlockRead32_TC_06_011_009_ereb Expected Result/Behavior
 * @subsubsection IoBlockRead32_TC_06_011_009_pre_ereb Expected Result/Behavior of Pre-process
 * -# allocate_setup_data_memory run successfully
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * The value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK
 * -# setup_hw_register run successfully
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection IoBlockRead32_TC_06_011_009_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * @subsubsection IoBlockRead32_TC_06_011_009_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_IoBlockRead32 return OSAL_RETURN_OK.<br>
 * The read data is the same as expected register data.
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * @subsection IoBlockRead32_TC_06_011_009_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead32_TC_06_011_009
 * @snippet test_io.cpp osal_io_isr_test_TC_06_011_009
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [IoBlockRead32_TC_06_011_009]
TEST_F(OSALINTTESTS, IoBlockRead32_TC_06_011_009)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    st_io_arg_t io_arg;
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
 * Wait for the interrupt callback function finish. */
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_GT(numOfChannel, 0) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_io_isr_test_TC_06_011_009, (void *)&arg), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK) << "pre-process failed";
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory*/
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_io_isr_test_TC_06_011_009), OSAL_RETURN_OK) << "post-process failed";
    deallocate_data_memory(hndl_mmngr, buffer_obj);
}
// [IoBlockRead32_TC_06_011_009]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockRead32_TC_06_011_010 IOApiTest.IoBlockRead32_TC_06_011_010
 * @subsection IoBlockRead32_TC_06_011_010_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockRead32_TC_06_011_010_tci Test Case ID
 * IoBlockRead_S1_N_TC_06_011_010
 * @subsection IoBlockRead32_TC_06_011_010_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207, \#199211
 * @subsection IoBlockRead32_TC_06_011_010_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockRead32_TC_06_011_010_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockRead APIs of OSAL on normal condition:
 * - pre-process: open test target device, turn ON its power and clock
 * - Test06_011_010: use R_OSAL_IoBlockRead32 to read data from register of test target device with offset "TEST_READBLOCK32_REG_OFFSET_NC" and size is 0.
 * - post-process: turn OFF clock and close test target device
 * @subsection IoBlockRead32_TC_06_011_010_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data with offset "TEST_READBLOCK32_REG_OFFSET_NC" and size is 0 by using R_OSAL_IoBlockRead32
 * -# Set the clock of test target device status to "OFF":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockRead32_TC_06_011_010_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockRead32 return OSAL_RETURN_OK.<br>
 * The value of "read_data32" is equal to 0.
 * -# Set clock to "OFF" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoBlockRead32_TC_06_011_010_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead32_TC_06_011_010
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockRead32_TC_06_011_010]
TEST_F(IOApiTest, IoBlockRead32_TC_06_011_010)
{
    uint32_t read_data32 = 0U;

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: normal case: read register value of test target device with size is 0.
    // Test06_011_010
    EXPECT_EQ(R_OSAL_IoBlockRead32(device_handle, TEST_READBLOCK32_REG_OFFSET_NC, &read_data32, TEST_IO_SIZE_ZERO), OSAL_RETURN_OK) << "Test06_011_010 failed";
    EXPECT_EQ(read_data32, 0) << "Test06_011_010 failed";

    // post-process: turn OFF clock and close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockRead32_TC_06_011_010]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockRead64_TC_06_012_001 IOApiTest.IoBlockRead64_TC_06_012_001
 * @subsection IoBlockRead64_TC_06_012_001_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockRead64_TC_06_012_001_tci Test Case ID
 * IoBlockRead_S1_N_TC_06_012_001
 * @subsection IoBlockRead64_TC_06_012_001_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207, \#199211
 * @subsection IoBlockRead64_TC_06_012_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - "device_list" contain the device list of each "device_type" in "devices".
 * @subsection IoBlockRead64_TC_06_012_001_tcs Test Case Summary
 * Run this test when 64bit Bus is supported.<br>
 * This test shall verify the functionality of the IoBlockRead APIs of OSAL on normal condition:
 * - pre-process: open test target device, turn ON its power and clock
 * - Test06_012_001: use R_OSAL_IoBlockRead64 to read data from register of test target device with offset "TEST_READBLOCK64_REG_OFFSET_NC".
 * - post-process: close test target device
 * @subsection IoBlockRead64_TC_06_012_001_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data with offset "TEST_READBLOCK64_REG_OFFSET_NC" by using R_OSAL_IoBlockRead64
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockRead64_TC_06_012_001_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockRead64 return OSAL_RETURN_OK.<br>
 * The read data is the same as expected register data.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoBlockRead64_TC_06_012_001_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead64_TC_06_012_001
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockRead64_TC_06_012_001]
TEST_F(IOApiTest, IoBlockRead64_TC_06_012_001)
{
    uintptr_t offset;
    size_t size = 8;
    uint64_t read_data64[1];
    uint64_t register_data64[1] = {TEST_READBLOCK64_VAL_NC[0]};

    for (auto it = device_list.begin(); it != device_list.end(); it++)
    {
        if (!strcmp((*it).c_str(), TEST_IO_DEVICE_1))
        {
            // pre-process: open test target device, turn ON its power and clock
            EXPECT_EQ(R_OSAL_IoDeviceOpen((*it).c_str(), &device_handle), OSAL_RETURN_OK) << "pre-process failed";
            EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
            EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

            // @brief: normal case: read register value of test target device with aligned offset.
            // Test06_012_001
            offset = TEST_READBLOCK64_REG_OFFSET_NC;
            EXPECT_EQ(R_OSAL_IoBlockRead64(device_handle, offset, read_data64, size), OSAL_RETURN_OK) << "Test06_012_001 failed";
            read_data64[0] = read_data64[0] & 0x00000000FFFFFFFF;
            EXPECT_TRUE(equal(begin(register_data64), end(register_data64), begin(read_data64))) << "Test06_012_001 failed";

            // post-process: close test target device
            EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
            EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
            device_handle = OSAL_DEVICE_HANDLE_INVALID;
        }
    }
}
// [IoBlockRead64_TC_06_012_001]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockRead64_TC_06_012_002 IOApiTest.IoBlockRead64_TC_06_012_002
 * @subsection IoBlockRead64_TC_06_012_002_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockRead64_TC_06_012_002_tci Test Case ID
 * IoBlockRead_S0_A_TC_06_012_002
 * @subsection IoBlockRead64_TC_06_012_002_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockRead64_TC_06_012_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockRead64_TC_06_012_002_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockRead APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_012_002: use R_OSAL_IoBlockRead64 to read register value of test target device with un-aligned offset.
 * - post-process: close test target device
 * @subsection IoBlockRead64_TC_06_012_002_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data with offset "TEST_READBLOCK64_OFFSET_UNALIGNED" by using R_OSAL_IoBlockRead64
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockRead64_TC_06_012_002_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockRead64 return OSAL_RETURN_PAR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoBlockRead64_TC_06_012_002_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead64_TC_06_012_002
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockRead64_TC_06_012_002]
TEST_F(IOApiTest, IoBlockRead64_TC_06_012_002)
{
    uintptr_t offset;
    size_t size = 16;
    uint64_t read_data64[2];

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: read register value of test target device with un-aligned offset.
    // Test06_012_002
    offset = TEST_READBLOCK64_OFFSET_UNALIGNED;
    EXPECT_EQ(R_OSAL_IoBlockRead64(device_handle, offset, read_data64, size), OSAL_RETURN_PAR) << "Test06_012_002 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockRead64_TC_06_012_002]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockRead64_TC_06_012_003 IOApiTest.IoBlockRead64_TC_06_012_003
 * @subsection IoBlockRead64_TC_06_012_003_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockRead64_TC_06_012_003_tci Test Case ID
 * IoBlockRead_S0_A_TC_06_012_003
 * @subsection IoBlockRead64_TC_06_012_003_rr Related Requirement(s)
 * \#199151, \#199199, \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockRead64_TC_06_012_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockRead64_TC_06_012_003_tcs Test Case Summary
 * This test shall verify the functionality of the IoRead APIs of OSAL on abnormal condition:
 * - Test06_012_003: use R_OSAL_IoBlockRead64 to read register value with device handle is NULL
 * @subsection IoBlockRead64_TC_06_012_003_tpts Test Procedure/Test Steps
 * -# Read data with offset "TEST_READBLOCK64_REG_OFFSET_AC" by using R_OSAL_IoBlockRead64.
 * @subsection IoBlockRead64_TC_06_012_003_ereb Expected Result/Behavior
 * -# R_OSAL_IoBlockRead64 return OSAL_RETURN_HANDLE
 * @subsection IoBlockRead64_TC_06_012_003_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead64_TC_06_012_003
 */
// [IoBlockRead64_TC_06_012_003]
TEST_F(IOApiTest, IoBlockRead64_TC_06_012_003)
{
    uintptr_t offset;
    uint64_t read_data64[2];
    size_t size = 16;

    // @brief: abnormal case: read register value with device handle is NULL
    // Test06_012_003
    offset = TEST_READBLOCK64_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoBlockRead64(OSAL_DEVICE_HANDLE_INVALID, offset, read_data64, size), OSAL_RETURN_HANDLE) << "Test06_012_003 failed";
}
// [IoBlockRead64_TC_06_012_003]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockRead64_TC_06_012_004 IOApiTest.IoBlockRead64_TC_06_012_004
 * @subsection IoBlockRead64_TC_06_012_004_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockRead64_TC_06_012_004_tci Test Case ID
 * IoBlockRead_S0_BA_TC_06_012_004
 * @subsection IoBlockRead64_TC_06_012_004_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockRead64_TC_06_012_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockRead64_TC_06_012_004_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockRead APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_012_004: use R_OSAL_IoBlockRead64 to read register value of test target device with offset which is out-of-register-range.
 * - post-process: close test target device
 * @subsection IoBlockRead64_TC_06_012_004_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data with offset "TEST_OFFSET_OUT_RANGE" by using R_OSAL_IoBlockRead64
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockRead64_TC_06_012_004_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockRead64 return OSAL_RETURN_PAR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoBlockRead64_TC_06_012_004_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead64_TC_06_012_004
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockRead64_TC_06_012_004]
TEST_F(IOApiTest, IoBlockRead64_TC_06_012_004)
{
    uintptr_t offset;
    size_t size = 16;
    uint64_t read_data64[4];

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: read register value of test target device with offset which is out-of-register-range.
    // Test06_012_004
    offset = TEST_OFFSET_OUT_RANGE;
    EXPECT_EQ(R_OSAL_IoBlockRead64(device_handle, offset, read_data64, size), OSAL_RETURN_PAR) << "Test06_012_004 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockRead64_TC_06_012_004]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockRead64_TC_06_012_005 IOApiTest.IoBlockRead64_TC_06_012_005
 * @subsection IoBlockRead64_TC_06_012_005_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockRead64_TC_06_012_005_tci Test Case ID
 * IoBlockRead_S0_A_TC_06_012_005
 * @subsection IoBlockRead64_TC_06_012_005_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockRead64_TC_06_012_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockRead64_TC_06_012_005_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockRead APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_012_005: use R_OSAL_IoBlockRead64 to read register value of test target device with p_data is NULL
 * - post-process: close test target device
 * @subsection IoBlockRead64_TC_06_012_005_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data with offset "TEST_READBLOCK64_REG_OFFSET_AC" by using R_OSAL_IoBlockRead64
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockRead64_TC_06_012_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockRead64 return OSAL_RETURN_PAR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoBlockRead64_TC_06_012_005_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead64_TC_06_012_005
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockRead64_TC_06_012_005]
TEST_F(IOApiTest, IoBlockRead64_TC_06_012_005)
{
    uintptr_t offset;
    size_t size = 16;
    void* p_data = NULL;

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: read register value of test target device with p_data is NULL
    // Test06_012_005
    offset = TEST_READBLOCK64_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoBlockRead64(device_handle, offset, (uint64_t*)p_data, size), OSAL_RETURN_PAR) << "Test06_012_005 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockRead64_TC_06_012_005]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockRead64_TC_06_012_006 IOApiTest.IoBlockRead64_TC_06_012_006
 * @subsection IoBlockRead64_TC_06_012_006_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockRead64_TC_06_012_006_tci Test Case ID
 * IoBlockRead_S0_BA_TC_06_012_006
 * @subsection IoBlockRead64_TC_06_012_006_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockRead64_TC_06_012_006_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockRead64_TC_06_012_006_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockRead APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_012_006: use R_OSAL_IoBlockRead64 to read register value of test target device with size is out-of-range of register
 * - post-process: close test target device
 * @subsection IoBlockRead64_TC_06_012_006_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data with offset "TEST_READBLOCK64_REG_OFFSET_AC" by using R_OSAL_IoBlockRead64
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockRead64_TC_06_012_006_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockRead64 return OSAL_RETURN_PAR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoBlockRead64_TC_06_012_006_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead64_TC_06_012_006
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockRead64_TC_06_012_006]
TEST_F(IOApiTest, IoBlockRead64_TC_06_012_006)
{
    uintptr_t offset;
    size_t size = TEST_IO_SIZE_OUT_RANGE;
    uint64_t read_data64[2];

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: read register value of test target device with size is out-of-range of register
    // Test06_012_006
    offset = TEST_READBLOCK64_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoBlockRead64(device_handle, offset, read_data64, size), OSAL_RETURN_PAR) << "Test06_012_006 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockRead64_TC_06_012_006]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockRead64_TC_06_012_007 IOApiTest.IoBlockRead64_TC_06_012_007
 * @subsection IoBlockRead64_TC_06_012_007_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockRead64_TC_06_012_007_tci Test Case ID
 * IoBlockRead_S2_A_TC_06_012_007
 * @subsection IoBlockRead64_TC_06_012_007_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockRead64_TC_06_012_007_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockRead64_TC_06_012_007_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockRead APIs of OSAL on abnormal condition:
 * - Test06_012_007: use R_OSAL_IoBlockRead64 read register value with closed device handle
 * @subsection IoBlockRead64_TC_06_012_007_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * -# Read data with offset "TEST_READBLOCK64_REG_OFFSET_AC" by using R_OSAL_IoBlockRead64.
 * @subsection IoBlockRead64_TC_06_012_007_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_IoBlockRead64 return OSAL_RETURN_HANDLE
 * @subsection IoBlockRead64_TC_06_012_007_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead64_TC_06_012_007
 */
// [IoBlockRead64_TC_06_012_007]
TEST_F(IOApiTest, IoBlockRead64_TC_06_012_007)
{
    uintptr_t offset;
    size_t size = 16;
    uint64_t read_data64[2];

    // @brief: abnormal case: read register value with closed device handle
    // Test06_012_007
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "Test06_012_007 failed";
    EXPECT_NE(device_handle, NULL_PTR) << "Test06_012_007 failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "Test06_012_007 failed";

    offset = TEST_READBLOCK64_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoBlockRead64(device_handle, offset, read_data64, size), OSAL_RETURN_HANDLE) << "Test06_012_007 failed";
}
// [IoBlockRead64_TC_06_012_007]

/**
 * @page IO_TEST IO API Test
 * @section OsalApiStateTest_IoBlockRead64_TC_06_012_008 OsalApiStateTest.IoBlockRead64_TC_06_012_008
 * @subsection IoBlockRead64_TC_06_012_008_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection IoBlockRead64_TC_06_012_008_tci Test Case ID
 * IoBlockRead_S2_A_TC_06_012_008
 * @subsection IoBlockRead64_TC_06_012_008_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockRead64_TC_06_012_008_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize has already been called
 * @subsection IoBlockRead64_TC_06_012_008_tcs Test Case Summary
 * This test shall verify the state transition of the R_OSAL_IoBlockRead64 function:
 * - Test06_012_008: confirm state transition error with R_OSAL_IoBlockRead64
 * @subsection IoBlockRead64_TC_06_012_008_tpts Test Procedure/Test Steps
 * -# Confirm the state transition of R_OSAL_IoBlockRead64
 * @subsection IoBlockRead64_TC_06_012_008_ereb Expected Result/Behavior
 * -# R_OSAL_IoBlockRead64 return OSAL_RETURN_STATE
 * @subsection IoBlockRead64_TC_06_012_008_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead64_TC_06_012_008
 */
// [IoBlockRead64_TC_06_012_008]
TEST_F(OsalApiStateTest, IoBlockRead64_TC_06_012_008)
{
    uint64_t b_read_data64[2];
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;
    uintptr_t offset = TEST_READBLOCK64_REG_OFFSET_AC;
    size_t size = 16;

    // Test06_012_008
    // @brief: abnormal case: state transition error with IO BlockRead
    EXPECT_EQ(R_OSAL_IoBlockRead64(device_handle, offset, b_read_data64, size), OSAL_RETURN_STATE) << "Test06_012_008 failed";
}
// [IoBlockRead64_TC_06_012_008]

/**
 * @page IO_TEST IO API Test
 * @section OSALINTTESTS_IoBlockRead64_TC_06_012_009 OSALINTTESTS.IoBlockRead64_TC_06_012_009
 * @subsection IoBlockRead64_TC_06_012_009_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection IoBlockRead64_TC_06_012_009_tci Test Case ID
 * IoBlockRead_S1_N_TC_06_012_009
 * @subsection IoBlockRead64_TC_06_012_009_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207, \#199211
 * @subsection IoBlockRead64_TC_06_012_009_pp Prerequisite/Precondition
 * -# OSAL is initialized.<br>
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection IoBlockRead64_TC_06_012_009_tcs Test Case Summary
 * Run this test when 64bit Bus is supported.
 * @subsubsection IoBlockRead64_TC_06_012_009_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Setup required memory for interrupt generation.
 * - Register callack function and enable interrupt.
 * - Wait for the interrupt callback function finish.
 * @subsubsection IoBlockRead64_TC_06_012_009_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * @subsubsection IoBlockRead64_TC_06_012_009_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality of the IO APIs which are called in the ISR
 * - Disable and clear interrupt status
 * - Test06_012_009 : use R_OSAL_IoBlockRead with 64 bit
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection IoBlockRead64_TC_06_012_009_tpts Test Procedure/Test Steps
 * @subsubsection IoBlockRead64_TC_06_012_009_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Allocate required memory for generate interrupt from test target device and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function by using R_OSAL_InterruptRegisterIsr.<br>
 * -# Enable interrupt by using R_OSAL_InterruptEnableIsr
 * -# Setup test target device specific register to generate the interrupt by calling to setup_hw_register
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection IoBlockRead64_TC_06_012_009_post_tpts Test Procedure/Test Steps of Post-process
 * -# Unregister interrupt by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * @subsubsection IoBlockRead64_TC_06_012_009_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Read data with offset "TEST_READBLOCK64_REG_OFFSET_INT" by using R_OSAL_IoBlockRead64
 * -# Send Message interrupt of context finished by using R_OSAL_MqSendForTimePeriod
 * @subsection IoBlockRead64_TC_06_012_009_ereb Expected Result/Behavior
 * @subsubsection IoBlockRead64_TC_06_012_009_pre_ereb Expected Result/Behavior of Pre-process
 * -# allocate_setup_data_memory run successfully
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * The value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK
 * -# setup_hw_register run successfully
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection IoBlockRead64_TC_06_012_009_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * @subsubsection IoBlockRead64_TC_06_012_009_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_IoBlockRead64 return OSAL_RETURN_OK.<br>
 * The read data is the same as expected register data.
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * @subsection IoBlockRead64_TC_06_012_009_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead64_TC_06_012_009
 * @snippet test_io.cpp osal_io_isr_test_TC_06_012_009
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [IoBlockRead64_TC_06_012_009]
TEST_F(OSALINTTESTS, IoBlockRead64_TC_06_012_009)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    st_io_arg_t io_arg;
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
 * Wait for the interrupt callback function finish. */
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_GT(numOfChannel, 0) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_io_isr_test_TC_06_012_009, (void *)&arg), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK) << "pre-process failed";
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory*/
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_io_isr_test_TC_06_012_009), OSAL_RETURN_OK) << "post-process failed";
    deallocate_data_memory(hndl_mmngr, buffer_obj);
}
// [IoBlockRead64_TC_06_012_009]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockRead64_TC_06_012_010 IOApiTest.IoBlockRead64_TC_06_012_010
 * @subsection IoBlockRead64_TC_06_012_010_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockRead64_TC_06_012_010_tci Test Case ID
 * IoBlockRead_S1_N_TC_06_012_010
 * @subsection IoBlockRead64_TC_06_012_010_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207, \#199211
 * @subsection IoBlockRead64_TC_06_012_010_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockRead64_TC_06_012_010_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockRead APIs of OSAL on normal condition:
 * - pre-process: open test target device, turn ON its power and clock
 * - Test06_012_010: use R_OSAL_IoBlockRead64 to read data from register of test target device with offset "TEST_READBLOCK64_REG_OFFSET_NC" and size is 0.
 * - post-process: turn OFF clock and close test target device
 * @subsection IoBlockRead64_TC_06_012_010_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data with offset "TEST_READBLOCK64_REG_OFFSET_NC" and size is 0 by using R_OSAL_IoBlockRead64
 * -# Set the clock of test target device status to "OFF":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockRead64_TC_06_012_010_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockRead64 return OSAL_RETURN_OK.<br>
 * The value of "read_data64" is equal to 0.
 * -# Set clock to "OFF" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoBlockRead64_TC_06_012_010_testcase Test Case Example
 * @snippet test_io.cpp IoBlockRead64_TC_06_012_010
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockRead64_TC_06_012_010]
TEST_F(IOApiTest, IoBlockRead64_TC_06_012_010)
{
    uint64_t read_data64 = 0U;

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: normal case: read register value of test target device with size is 0.
    // Test06_012_010
    EXPECT_EQ(R_OSAL_IoBlockRead64(device_handle, TEST_READBLOCK64_REG_OFFSET_NC, &read_data64, TEST_IO_SIZE_ZERO), OSAL_RETURN_OK) << "Test06_012_010 failed";
    EXPECT_EQ(read_data64, 0) << "Test06_012_010 failed";

    // post-process: turn OFF clock and close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockRead64_TC_06_012_010]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite8_TC_06_017_001 IOApiTest.IoBlockWrite8_TC_06_017_001
 * @subsection IoBlockWrite8_TC_06_017_001_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite8_TC_06_017_001_tci Test Case ID
 * IoBlockWrite_S1_N_TC_06_017_001
 * @subsection IoBlockWrite8_TC_06_017_001_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207, \#199211
 * @subsection IoBlockWrite8_TC_06_017_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - "device_list" contain the device list of each "device_type" in "devices".
 * @subsection IoBlockWrite8_TC_06_017_001_tcs Test Case Summary
 * Run this test when 8bit Bus is supported.<br>
 * This test shall verify the functionality of the IoBlockWrite APIs of OSAL on normal condition:
 * - pre-process: open test target device, turn ON its power and clock
 * - Test06_017_001: use R_OSAL_IoBlockWrite8 to write data to register of test target device with offset "TEST_WRITEBLOCK8_REG_OFFSET_NC".
 * - post-process: write back initialization value, turn OFF clock and close test target device
 * @subsection IoBlockWrite8_TC_06_017_001_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read initialization data from test target device with offset "TEST_WRITEBLOCK8_REG_OFFSET_NC" by using R_OSAL_IoBlockRead8.
 * -# Write "register_data8" to test target device with offset "TEST_WRITEBLOCK8_REG_OFFSET_NC" and size is 16 by using R_OSAL_IoBlockWrite8
 * -# Read-back data from test target device with offset "TEST_WRITEBLOCK8_REG_OFFSET_NC" and size is 16 by using R_OSAL_IoBlockRead8.
 * -# Write back initialization data to test target device with offset "TEST_WRITEBLOCK8_REG_OFFSET_NC" by using R_OSAL_IoBlockWrite8.
 * -# Set the clock of test target device status to "OFF":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockWrite8_TC_06_017_001_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockRead8 return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockWrite8 return OSAL_RETURN_OK
 * -# R_OSAL_IoBlockRead8 return OSAL_RETURN_OK.<br>
 * The read-back data is the same as write data.
 * -# R_OSAL_IoBlockWrite8 return OSAL_RETURN_OK.
 * -# Set clock to "OFF" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoBlockWrite8_TC_06_017_001_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite8_TC_06_017_001
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockWrite8_TC_06_017_001]
TEST_F(IOApiTest, IoBlockWrite8_TC_06_017_001)
{
    uint8_t init_data8[16];
    uintptr_t offset = TEST_WRITEBLOCK8_REG_OFFSET_NC;
    size_t size = 16;
    uint8_t read_data8[16];
    uint8_t register_data8[16] = {TEST_WRITEBLOCK8_VAL_NC[0], TEST_WRITEBLOCK8_VAL_NC[1],
                                  TEST_WRITEBLOCK8_VAL_NC[2], TEST_WRITEBLOCK8_VAL_NC[3],
                                  TEST_WRITEBLOCK8_VAL_NC[4], TEST_WRITEBLOCK8_VAL_NC[5],
                                  TEST_WRITEBLOCK8_VAL_NC[6], TEST_WRITEBLOCK8_VAL_NC[7],
                                  TEST_WRITEBLOCK8_VAL_NC[8], TEST_WRITEBLOCK8_VAL_NC[9],
                                  TEST_WRITEBLOCK8_VAL_NC[10], TEST_WRITEBLOCK8_VAL_NC[11],
                                  TEST_WRITEBLOCK8_VAL_NC[12], TEST_WRITEBLOCK8_VAL_NC[13],
                                  TEST_WRITEBLOCK8_VAL_NC[14], TEST_WRITEBLOCK8_VAL_NC[15]};

    for (auto it = device_list.begin(); it != device_list.end(); it++)
    {
        if (!strcmp((*it).c_str(), TEST_IO_DEVICE_1))
        {
            // pre-process: open test target device, turn ON its power and clock
            EXPECT_EQ(R_OSAL_IoDeviceOpen((*it).c_str(), &device_handle), OSAL_RETURN_OK) << "pre-process failed";
            EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
            EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";
            EXPECT_EQ(R_OSAL_IoBlockRead8(device_handle, offset, init_data8, size), OSAL_RETURN_OK) << "pre-process failed";

            // @brief: normal case: write register value of test target device with aligned offset.
            // Test06_017_001
            EXPECT_EQ(R_OSAL_IoBlockWrite8(device_handle, offset, register_data8, size), OSAL_RETURN_OK) << "Test06_017_001 failed";
            EXPECT_EQ(R_OSAL_IoBlockRead8(device_handle, offset, read_data8, size), OSAL_RETURN_OK) << "Test06_017_001 failed";
            EXPECT_TRUE(equal(begin(register_data8), end(register_data8), begin(read_data8))) << "Test06_017_001 failed";

            // post-process: write back initialization value, turn OFF clock and close test target device
            EXPECT_EQ(R_OSAL_IoBlockWrite8(device_handle, offset, init_data8, size), OSAL_RETURN_OK) << "post-process failed";
            EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
            EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
            device_handle = OSAL_DEVICE_HANDLE_INVALID;
        }
    }
}
// [IoBlockWrite8_TC_06_017_001]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite8_TC_06_017_003 IOApiTest.IoBlockWrite8_TC_06_017_003
 * @subsection IoBlockWrite8_TC_06_017_003_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite8_TC_06_017_003_tci Test Case ID
 * IoBlockWrite_S0_A_TC_06_017_003
 * @subsection IoBlockWrite8_TC_06_017_003_rr Related Requirement(s)
 * \#199151, \#199199, \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockWrite8_TC_06_017_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockWrite8_TC_06_017_003_tcs Test Case Summary
 * This test shall verify the functionality of the IoWrite APIs of OSAL on abnormal condition:
 * - Test06_017_003: use R_OSAL_IoBlockWrite8 to write register value with device handle is NULL
 * @subsection IoBlockWrite8_TC_06_017_003_tpts Test Procedure/Test Steps
 * -# Write data with offset "TEST_WRITEBLOCK8_REG_OFFSET_AC" by using R_OSAL_IoBlockWrite8.
 * @subsection IoBlockWrite8_TC_06_017_003_ereb Expected Result/Behavior
 * -# R_OSAL_IoBlockWrite8 return OSAL_RETURN_HANDLE
 * @subsection IoBlockWrite8_TC_06_017_003_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite8_TC_06_017_003
 */
// [IoBlockWrite8_TC_06_017_003]
TEST_F(IOApiTest, IoBlockWrite8_TC_06_017_003)
{
    uintptr_t offset;
    size_t size = 16;
    uint8_t register_data8[16] = {TEST_WRITEBLOCK8_VAL_AC[0], TEST_WRITEBLOCK8_VAL_AC[1],
                                  TEST_WRITEBLOCK8_VAL_AC[2], TEST_WRITEBLOCK8_VAL_AC[3],
                                  TEST_WRITEBLOCK8_VAL_AC[4], TEST_WRITEBLOCK8_VAL_AC[5], 
                                  TEST_WRITEBLOCK8_VAL_AC[6], TEST_WRITEBLOCK8_VAL_AC[7], 
                                  TEST_WRITEBLOCK8_VAL_AC[8], TEST_WRITEBLOCK8_VAL_AC[9], 
                                  TEST_WRITEBLOCK8_VAL_AC[10], TEST_WRITEBLOCK8_VAL_AC[11], 
                                  TEST_WRITEBLOCK8_VAL_AC[12], TEST_WRITEBLOCK8_VAL_AC[13], 
                                  TEST_WRITEBLOCK8_VAL_AC[14], TEST_WRITEBLOCK8_VAL_AC[15]};

    // @brief: abnormal case: write register value with device handle is NULL
    // Test06_017_003
    offset = TEST_WRITEBLOCK8_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoBlockWrite8(OSAL_DEVICE_HANDLE_INVALID, offset, register_data8, size), OSAL_RETURN_HANDLE) << "Test06_017_003 failed";
}
// [IoBlockWrite8_TC_06_017_003]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite8_TC_06_017_004 IOApiTest.IoBlockWrite8_TC_06_017_004
 * @subsection IoBlockWrite8_TC_06_017_004_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite8_TC_06_017_004_tci Test Case ID
 * IoBlockWrite_S0_BA_TC_06_017_004
 * @subsection IoBlockWrite8_TC_06_017_004_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockWrite8_TC_06_017_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockWrite8_TC_06_017_004_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockWrite APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_017_004: use R_OSAL_IoBlockWrite8 to write data to register of test target device with offset which is out-of-register-range.
 * - post-process: close test target device
 * @subsection IoBlockWrite8_TC_06_017_004_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Write data with offset "TEST_OFFSET_OUT_RANGE" by using R_OSAL_IoBlockWrite8
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockWrite8_TC_06_017_004_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockWrite8 return OSAL_RETURN_PAR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoBlockWrite8_TC_06_017_004_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite8_TC_06_017_004
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockWrite8_TC_06_017_004]
TEST_F(IOApiTest, IoBlockWrite8_TC_06_017_004)
{
    uintptr_t offset;
    size_t size = 16;
    uint8_t register_data8[16] = {TEST_WRITEBLOCK8_VAL_AC[0], TEST_WRITEBLOCK8_VAL_AC[1],
                                  TEST_WRITEBLOCK8_VAL_AC[2], TEST_WRITEBLOCK8_VAL_AC[3],
                                  TEST_WRITEBLOCK8_VAL_AC[4], TEST_WRITEBLOCK8_VAL_AC[5], 
                                  TEST_WRITEBLOCK8_VAL_AC[6], TEST_WRITEBLOCK8_VAL_AC[7], 
                                  TEST_WRITEBLOCK8_VAL_AC[8], TEST_WRITEBLOCK8_VAL_AC[9], 
                                  TEST_WRITEBLOCK8_VAL_AC[10], TEST_WRITEBLOCK8_VAL_AC[11], 
                                  TEST_WRITEBLOCK8_VAL_AC[12], TEST_WRITEBLOCK8_VAL_AC[13], 
                                  TEST_WRITEBLOCK8_VAL_AC[14], TEST_WRITEBLOCK8_VAL_AC[15]};

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: write register value of test target device with offset which is out-of-register-range.
    // Test06_017_004
    offset = TEST_OFFSET_OUT_RANGE;
    EXPECT_EQ(R_OSAL_IoBlockWrite8(device_handle, offset, register_data8, size), OSAL_RETURN_PAR) << "Test06_017_004 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockWrite8_TC_06_017_004]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite8_TC_06_017_005 IOApiTest.IoBlockWrite8_TC_06_017_005
 * @subsection IoBlockWrite8_TC_06_017_005_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite8_TC_06_017_005_tci Test Case ID
 * IoBlockWrite_S0_A_TC_06_017_005
 * @subsection IoBlockWrite8_TC_06_017_005_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockWrite8_TC_06_017_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockWrite8_TC_06_017_005_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockWrite APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_017_005: use R_OSAL_IoBlockWrite64 to write register value of test target device with p_data is NULL
 * - post-process: close test target device
 * @subsection IoBlockWrite8_TC_06_017_005_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Write data with offset "TEST_WRITEBLOCK8_REG_OFFSET_AC" by using R_OSAL_IoBlockWrite64
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockWrite8_TC_06_017_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockWrite64 return OSAL_RETURN_PAR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoBlockWrite8_TC_06_017_005_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite8_TC_06_017_005
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockWrite8_TC_06_017_005]
TEST_F(IOApiTest, IoBlockWrite8_TC_06_017_005)
{
    uintptr_t offset;
    size_t size = 16;
    void* p_data = NULL;

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: write register value of test target device with p_data is NULL
    // Test06_017_005
    offset = TEST_WRITEBLOCK8_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoBlockWrite8(device_handle, offset, (uint8_t*)p_data, size), OSAL_RETURN_PAR) << "Test06_017_005 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockWrite8_TC_06_017_005]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite8_TC_06_017_006 IOApiTest.IoBlockWrite8_TC_06_017_006
 * @subsection IoBlockWrite8_TC_06_017_006_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite8_TC_06_017_006_tci Test Case ID
 * IoBlockWrite_S0_BA_TC_06_017_006
 * @subsection IoBlockWrite8_TC_06_017_006_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockWrite8_TC_06_017_006_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockWrite8_TC_06_017_006_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockWrite APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_017_006: use R_OSAL_IoBlockWrite8 to write data to register of test target device with size is out-of-range of register
 * - post-process: close test target device
 * @subsection IoBlockWrite8_TC_06_017_006_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Write data with offset "TEST_WRITEBLOCK8_REG_OFFSET_AC" by using R_OSAL_IoBlockWrite8
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockWrite8_TC_06_017_006_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockWrite8 return OSAL_RETURN_PAR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoBlockWrite8_TC_06_017_006_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite8_TC_06_017_006
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockWrite8_TC_06_017_006]
TEST_F(IOApiTest, IoBlockWrite8_TC_06_017_006)
{
    uintptr_t offset;
    size_t size = TEST_IO_SIZE_OUT_RANGE;
    uint8_t register_data8[16] = {TEST_WRITEBLOCK8_VAL_AC[0], TEST_WRITEBLOCK8_VAL_AC[1],
                                  TEST_WRITEBLOCK8_VAL_AC[2], TEST_WRITEBLOCK8_VAL_AC[3],
                                  TEST_WRITEBLOCK8_VAL_AC[4], TEST_WRITEBLOCK8_VAL_AC[5], 
                                  TEST_WRITEBLOCK8_VAL_AC[6], TEST_WRITEBLOCK8_VAL_AC[7], 
                                  TEST_WRITEBLOCK8_VAL_AC[8], TEST_WRITEBLOCK8_VAL_AC[9], 
                                  TEST_WRITEBLOCK8_VAL_AC[10], TEST_WRITEBLOCK8_VAL_AC[11], 
                                  TEST_WRITEBLOCK8_VAL_AC[12], TEST_WRITEBLOCK8_VAL_AC[13], 
                                  TEST_WRITEBLOCK8_VAL_AC[14], TEST_WRITEBLOCK8_VAL_AC[15]};

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: write register value of test target device with size is out-of-range of register
    // Test06_017_006
    offset = TEST_WRITEBLOCK8_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoBlockWrite8(device_handle, offset, register_data8, size), OSAL_RETURN_PAR) << "Test06_017_006 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockWrite8_TC_06_017_006]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite8_TC_06_017_007 IOApiTest.IoBlockWrite8_TC_06_017_007
 * @subsection IoBlockWrite8_TC_06_017_007_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite8_TC_06_017_007_tci Test Case ID
 * IoBlockWrite_S2_A_TC_06_017_007
 * @subsection IoBlockWrite8_TC_06_017_007_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockWrite8_TC_06_017_007_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockWrite8_TC_06_017_007_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockWrite APIs of OSAL on abnormal condition:
 * - Test06_017_007: use R_OSAL_IoBlockWrite8 write register value with closed device handle
 * @subsection IoBlockWrite8_TC_06_017_007_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * -# Write data with offset "TEST_WRITEBLOCK8_REG_OFFSET_AC" by using R_OSAL_IoBlockWrite8.
 * @subsection IoBlockWrite8_TC_06_017_007_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_IoBlockWrite8 return OSAL_RETURN_HANDLE
 * @subsection IoBlockWrite8_TC_06_017_007_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite8_TC_06_017_007
 */
// [IoBlockWrite8_TC_06_017_007]
TEST_F(IOApiTest, IoBlockWrite8_TC_06_017_007)
{
    uintptr_t offset;
    size_t size = 16;
    uint8_t register_data8[16] = {TEST_WRITEBLOCK8_VAL_AC[0], TEST_WRITEBLOCK8_VAL_AC[1],
                                  TEST_WRITEBLOCK8_VAL_AC[2], TEST_WRITEBLOCK8_VAL_AC[3],
                                  TEST_WRITEBLOCK8_VAL_AC[4], TEST_WRITEBLOCK8_VAL_AC[5], 
                                  TEST_WRITEBLOCK8_VAL_AC[6], TEST_WRITEBLOCK8_VAL_AC[7], 
                                  TEST_WRITEBLOCK8_VAL_AC[8], TEST_WRITEBLOCK8_VAL_AC[9], 
                                  TEST_WRITEBLOCK8_VAL_AC[10], TEST_WRITEBLOCK8_VAL_AC[11], 
                                  TEST_WRITEBLOCK8_VAL_AC[12], TEST_WRITEBLOCK8_VAL_AC[13], 
                                  TEST_WRITEBLOCK8_VAL_AC[14], TEST_WRITEBLOCK8_VAL_AC[15]};

    // @brief: abnormal case: Write register value with closed device handle
    // Test06_017_007
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "Test06_017_007 failed";
    EXPECT_NE(device_handle, NULL_PTR) << "Test06_017_007 failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "Test06_017_007 failed";

    offset = TEST_WRITEBLOCK8_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoBlockWrite8(device_handle, offset, register_data8, size), OSAL_RETURN_HANDLE) << "Test06_017_007 failed";
}
// [IoBlockWrite8_TC_06_017_007]

/**
 * @page IO_TEST IO API Test
 * @section OsalApiStateTest_IoBlockWrite8_TC_06_017_008 OsalApiStateTest.IoBlockWrite8_TC_06_017_008
 * @subsection IoBlockWrite8_TC_06_017_008_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection IoBlockWrite8_TC_06_017_008_tci Test Case ID
 * IoBlockWrite_S2_A_TC_06_017_008
 * @subsection IoBlockWrite8_TC_06_017_008_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockWrite8_TC_06_017_008_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize has already been called
 * @subsection IoBlockWrite8_TC_06_017_008_tcs Test Case Summary
 * This test shall verify the state transition of the R_OSAL_IoBlockWrite8 function:
 * - Test06_017_008: confirm state transition error with R_OSAL_IoBlockWrite8
 * @subsection IoBlockWrite8_TC_06_017_008_tpts Test Procedure/Test Steps
 * -# Confirm the state transition of R_OSAL_IoBlockWrite8
 * @subsection IoBlockWrite8_TC_06_017_008_ereb Expected Result/Behavior
 * -# R_OSAL_IoBlockWrite8return OSAL_RETURN_STATE
 * @subsection IoBlockWrite8_TC_06_017_008_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite8_TC_06_017_008
 */
// [IoBlockWrite8_TC_06_017_008]
TEST_F(OsalApiStateTest, IoBlockWrite8_TC_06_017_008)
{
    uint8_t register_data8[16] = {TEST_WRITEBLOCK8_VAL_AC[0], TEST_WRITEBLOCK8_VAL_AC[1],
                                  TEST_WRITEBLOCK8_VAL_AC[2], TEST_WRITEBLOCK8_VAL_AC[3],
                                  TEST_WRITEBLOCK8_VAL_AC[4], TEST_WRITEBLOCK8_VAL_AC[5], 
                                  TEST_WRITEBLOCK8_VAL_AC[6], TEST_WRITEBLOCK8_VAL_AC[7], 
                                  TEST_WRITEBLOCK8_VAL_AC[8], TEST_WRITEBLOCK8_VAL_AC[9], 
                                  TEST_WRITEBLOCK8_VAL_AC[10], TEST_WRITEBLOCK8_VAL_AC[11], 
                                  TEST_WRITEBLOCK8_VAL_AC[12], TEST_WRITEBLOCK8_VAL_AC[13], 
                                  TEST_WRITEBLOCK8_VAL_AC[14], TEST_WRITEBLOCK8_VAL_AC[15]};

    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;
    uintptr_t offset = TEST_WRITEBLOCK8_REG_OFFSET_AC;
    size_t size = 16;

    //Test06_017_008
    //@brief: abnormal case: state transition error with IO BlockWrite
    EXPECT_EQ(R_OSAL_IoBlockWrite8(device_handle, offset, register_data8, size), OSAL_RETURN_STATE) << "Test06_017_008 failed";
}
// [IoBlockWrite8_TC_06_017_008]

/**
 * @page IO_TEST IO API Test
 * @section OSALINTTESTS_IoBlockWrite8_TC_06_017_009 OSALINTTESTS.IoBlockWrite8_TC_06_017_009
 * @subsection IoBlockWrite8_TC_06_017_009_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection IoBlockWrite8_TC_06_017_009_tci Test Case ID
 * IoBlockWrite_S1_N_TC_06_017_009
 * @subsection IoBlockWrite8_TC_06_017_009_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207, \#199211
 * @subsection IoBlockWrite8_TC_06_017_009_pp Prerequisite/Precondition
 * -# OSAL is initialized.<br>
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection IoBlockWrite8_TC_06_017_009_tcs Test Case Summary
 * Run this test when 8bit Bus is supported.
 * @subsubsection IoBlockWrite8_TC_06_017_009_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Setup required memory for interrupt generation.
 * - Register callack function and enable interrupt.
 * - Wait for the interrupt callback function finish.
 * @subsubsection IoBlockWrite8_TC_06_017_009_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * @subsubsection IoBlockWrite8_TC_06_017_009_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality of the IO APIs which are called in the ISR
 * - Disable and clear interrupt status
 * - Test06_017_009: Write data totest target device register by using R_OSAL_IoBlockWrite with 8 bit in ISR
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection IoBlockWrite8_TC_06_017_009_tpts Test Procedure/Test Steps
 * @subsubsection IoBlockWrite8_TC_06_017_009_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Allocate required memory for generate interrupt from test target device and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function by using R_OSAL_InterruptRegisterIsr.<br>
 * -# Enable interrupt by using R_OSAL_InterruptEnableIsr
 * -# Setup test target device specific register to generate the interrupt by calling to setup_hw_register
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection IoBlockWrite8_TC_06_017_009_post_tpts Test Procedure/Test Steps of Post-process
 * -# Unregister interrupt by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * @subsubsection IoBlockWrite8_TC_06_017_009_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Read initialization data from test target device with offset "TEST_WRITEBLOCK8_REG_OFFSET_INT" by using R_OSAL_IoBlockRead8.
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Write "registerblock_data8" to test target device with offset "TEST_WRITEBLOCK8_REG_OFFSET_INT" and size is 16 by using R_OSAL_IoBlockWrite8
 * -# Read-back data from test target device with offset "TEST_WRITEBLOCK8_REG_OFFSET_INT" and size is 16 by using R_OSAL_IoBlockRead8.
 * -# Write back initialization data to test target device with offset "TEST_WRITEBLOCK8_REG_OFFSET_INT" by using R_OSAL_IoBlockWrite8.
 * -# Send Message interrupt of context finished by using R_OSAL_MqSendForTimePeriod
 * @subsection IoBlockWrite8_TC_06_017_009_ereb Expected Result/Behavior
 * @subsubsection IoBlockWrite8_TC_06_017_009_pre_ereb Expected Result/Behavior of Pre-process
 * -# allocate_setup_data_memory run successfully
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * The value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK
 * -# setup_hw_register run successfully
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection IoBlockWrite8_TC_06_017_009_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * @subsubsection IoBlockWrite8_TC_06_017_009_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK
 * -# R_OSAL_IoBlockRead8 return OSAL_RETURN_OK.
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_IoBlockWrite8 return OSAL_RETURN_OK
 * -# R_OSAL_IoBlockRead8 return OSAL_RETURN_OK.<br>
 * The read-back data is the same as write data.
 * -# R_OSAL_IoBlockWrite8 return OSAL_RETURN_OK.
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * @subsection IoBlockWrite8_TC_06_017_009_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite8_TC_06_017_009
 * @snippet test_io.cpp osal_io_isr_test_TC_06_017_009
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [IoBlockWrite8_TC_06_017_009]
TEST_F(OSALINTTESTS, IoBlockWrite8_TC_06_017_009)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    st_io_arg_t io_arg;
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
 * Wait for the interrupt callback function finish. */
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_GT(numOfChannel, 0) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_io_isr_test_TC_06_017_009, (void *)&arg), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK) << "pre-process failed";
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory*/
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_io_isr_test_TC_06_017_009), OSAL_RETURN_OK) << "post-process failed";
    deallocate_data_memory(hndl_mmngr, buffer_obj);
}
// [IoBlockWrite8_TC_06_017_009]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite8_TC_06_017_010 IOApiTest.IoBlockWrite8_TC_06_017_010
 * @subsection IoBlockWrite8_TC_06_017_010_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite8_TC_06_017_010_tci Test Case ID
 * IoBlockWrite_S1_N_TC_06_017_010
 * @subsection IoBlockWrite8_TC_06_017_010_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207, \#199211
 * @subsection IoBlockWrite8_TC_06_017_010_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockWrite8_TC_06_017_010_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockWrite APIs of OSAL on normal condition:
 * - pre-process: open test target device, turn ON its power and clock
 * - Test06_017_010: use R_OSAL_IoBlockWrite8 to write data to register of test target device with offset "TEST_WRITEBLOCK8_REG_OFFSET_NC" and size is 0.
 * - post-process: turn OFF clock and close test target device
 * @subsection IoBlockWrite8_TC_06_017_010_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data from test target device with offset "TEST_WRITEBLOCK8_REG_OFFSET_NC" and size is 16 by using R_OSAL_IoBlockRead8.
 * -# Write "register_data8" to test target device with offset "TEST_WRITEBLOCK8_REG_OFFSET_NC" and size is 0 by using R_OSAL_IoBlockWrite8
 * -# Read-back data from test target device with offset "TEST_WRITEBLOCK8_REG_OFFSET_NC" and size is 16 by using R_OSAL_IoBlockRead8.
 * -# Set the clock of test target device status to "OFF":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockWrite8_TC_06_017_010_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockRead8 return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockWrite8 return OSAL_RETURN_OK
 * -# R_OSAL_IoBlockRead8 return OSAL_RETURN_OK.<br>
 * The value of "read_data_before" is equal to "read_data_after".
 * -# Set clock to "OFF" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoBlockWrite8_TC_06_017_010_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite8_TC_06_017_010
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockWrite8_TC_06_017_010]
TEST_F(IOApiTest, IoBlockWrite8_TC_06_017_010)
{
    uint8_t read_data_before[16];
    uint8_t read_data_after[16];
    uint8_t register_data8[16] = {TEST_WRITEBLOCK8_VAL_NC[0], TEST_WRITEBLOCK8_VAL_NC[1],
                                  TEST_WRITEBLOCK8_VAL_NC[2], TEST_WRITEBLOCK8_VAL_NC[3],
                                  TEST_WRITEBLOCK8_VAL_NC[4], TEST_WRITEBLOCK8_VAL_NC[5],
                                  TEST_WRITEBLOCK8_VAL_NC[6], TEST_WRITEBLOCK8_VAL_NC[7],
                                  TEST_WRITEBLOCK8_VAL_NC[8], TEST_WRITEBLOCK8_VAL_NC[9],
                                  TEST_WRITEBLOCK8_VAL_NC[10], TEST_WRITEBLOCK8_VAL_NC[11],
                                  TEST_WRITEBLOCK8_VAL_NC[12], TEST_WRITEBLOCK8_VAL_NC[13],
                                  TEST_WRITEBLOCK8_VAL_NC[14], TEST_WRITEBLOCK8_VAL_NC[15]};

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: normal case: write register value of test target device with size is 0.
    // Test06_017_010
    EXPECT_EQ(R_OSAL_IoBlockRead8(device_handle, TEST_WRITEBLOCK8_REG_OFFSET_NC, read_data_before, TEST_IO_ACCESS_SIZE), OSAL_RETURN_OK) << "Test06_017_010 failed";
    EXPECT_EQ(R_OSAL_IoBlockWrite8(device_handle, TEST_WRITEBLOCK8_REG_OFFSET_NC, register_data8, TEST_IO_SIZE_ZERO), OSAL_RETURN_OK) << "Test06_017_010 failed";
    EXPECT_EQ(R_OSAL_IoBlockRead8(device_handle, TEST_WRITEBLOCK8_REG_OFFSET_NC, read_data_after, TEST_IO_ACCESS_SIZE), OSAL_RETURN_OK) << "Test06_017_010 failed";
    EXPECT_TRUE(equal(begin(read_data_before), end(read_data_before), begin(read_data_after))) << "Test06_017_010 failed";

    // post-process: turn OFF clock and close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockWrite8_TC_06_017_010]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite8_TC_06_017_011 IOApiTest.IoBlockWrite8_TC_06_017_011
 * @subsection IoBlockWrite8_TC_06_017_011_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite8_TC_06_017_011_tci Test Case ID
 * IoBlockWrite8_S1_N_TC_06_017_011
 * @subsection IoBlockWrite8_TC_06_017_011_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207, \#199211
 * @subsection IoBlockWrite8_TC_06_017_011_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - The synchronization queue was created
 * @subsection IoBlockWrite8_TC_06_017_011_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoBlockWrite8/R_OSAL_IoBlockRead8 function with the same device handle in multi-threads:
 * - Test06_017_011: Create 2 threads to write and read to register of the same test target device.
 * @subsection IoBlockWrite8_TC_06_017_011_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Turn ON power and clock of the target device by calling to set_clock_on
 * -# Create first thread using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE3
 *          -# task_name is "Block Read/write 8 bit value from/to device register in multi-threads"
 *          -# func is "io_block_read_write_8_device_child_thread" function with functionality:
 *              - Get config of synchronization queue by using R_OSAL_MqGetConfig
 *              - Read initialization data from test target device with offset "TEST_WRITEBLOCK8_REG_OFFSET_NC" by using R_OSAL_IoBlockRead8.
 *              - Write data is "register_data8" to test target device with offset "TEST_WRITEBLOCK8_REG_OFFSET_NC" by using R_OSAL_IoBlockWrite8.
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *          -# userarg includes device handle and thread id
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2
 *      - Address of first thread handle.
 * -# Receive message from synchronization queue by using R_OSAL_MqReceiveForTimePeriod
 * -# Create second thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE3
 *          -# task_name is "Block Read/write 8 bit value from/to device register in multi-threads"
 *          -# func is "io_block_read_write_8_device_child_thread" function with functionality:
 *              - Read data from test target device with offset "TEST_WRITEBLOCK8_REG_OFFSET_NC" by using R_OSAL_IoBlockRead8.
 *              - Write back initialization data to test target device with offset "TEST_WRITEBLOCK8_REG_OFFSET_NC" by using R_OSAL_IoBlockWrite8.
 *          -# userarg includes device handle and thread id
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID3
 *      - Address of second thread handle.
 * -# Turn OFF power and clock domain of target device by calling to set_clock_off
 * -# Join first thread by using R_OSAL_ThreadJoin.
 * -# Join second thread by using R_OSAL_ThreadJoin.
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockWrite8_TC_06_017_011_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with first thread.<br>
 * "thread_handle1" is not NULL_PTR.<br>
 * In io_block_read_write_8_device_child_thread function:
 *  - R_OSAL_MqGetConfig return OSAL_RETURN_OK.
 *  - R_OSAL_IoBlockRead8 return OSAL_RETURN_OK.
 *  - R_OSAL_IoBlockWrite8 return OSAL_RETURN_OK.
 *  - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK.
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK.<br>
 * The value of "receive_msg" need to be equal to "send_msg"
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with second thread.<br>
 * "thread_handle2" is not NULL_PTR.<br>
 * In io_block_read_write_8_device_child_thread function:
 *  - R_OSAL_IoBlockRead8 return OSAL_RETURN_OK.<br>
 * "read_data8" is equal to "register_data8".
 *  - R_OSAL_IoBlockWrite8 return OSAL_RETURN_OK.
 * -# Set power and clock to "OFF" successfully
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with first thread.<br>
 * Value of "result" is equal to OSAL_RETURN_OK.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with second thread.<br>
 * Value of "result" is equal to OSAL_RETURN_OK.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoBlockWrite8_TC_06_017_011_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite8_TC_06_017_011
 * @snippet test_io.cpp io_block_read_write_8_device_child_thread
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockWrite8_TC_06_017_011]
TEST_F(IOApiTest, IoBlockWrite8_TC_06_017_011)
{

    st_osal_thread_config_t config_thread;
    int64_t                 result          = 0;
    osal_thread_handle_t    thread_handle1  = OSAL_THREAD_HANDLE_INVALID;
    osal_thread_handle_t    thread_handle2  = OSAL_THREAD_HANDLE_INVALID;

    // Test06_017_011
    // @brief: normal case: Block read write register 8-bit in multi-threads.
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";

    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "Test06_017_011 failed";

    io_test_multiple_thread_worker_arg user_arg1(&device_handle, TEST_THREAD_ID2);
    user_arg1.send_msg = send_msg;
    user_arg1.mq = sync_mq;
    config_thread.userarg       = &user_arg1;
    config_thread.priority      = OSAL_THREAD_PRIORITY_TYPE3;
    config_thread.task_name     = (char *)"Block Read/write 8 bit value from/to device register in multi-threads";
    config_thread.func          = io_block_read_write_8_device_child_thread;
    config_thread.stack_size    = THREAD_STACK_SIZE;
    EXPECT_EQ(R_OSAL_ThreadCreate(&config_thread, TEST_THREAD_ID2, &thread_handle1), OSAL_RETURN_OK) << "Test06_017_011 failed";
    EXPECT_NE(thread_handle1, NULL_PTR) << "Test06_017_011 failed";
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &receive_msg, mq_config.msg_size), OSAL_RETURN_OK) << "Test06_017_011 failed";
    EXPECT_EQ(receive_msg, send_msg) << "Test06_017_011 failed";

    user_arg1.uid              = TEST_THREAD_ID3;
    EXPECT_EQ(R_OSAL_ThreadCreate(&config_thread, TEST_THREAD_ID3, &thread_handle2), OSAL_RETURN_OK) << "Test06_017_011 failed";
    EXPECT_NE(thread_handle2, NULL_PTR) << "Test06_017_011 failed";

    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "Test06_017_011 failed";

    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &result),OSAL_RETURN_OK) << "Test06_017_011 failed";
    EXPECT_EQ(result, (int64_t)OSAL_RETURN_OK) << "Test06_017_011 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle2, &result),OSAL_RETURN_OK) << "Test06_017_011 failed";
    EXPECT_EQ(result, (int64_t)OSAL_RETURN_OK) << "Test06_017_011 failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockWrite8_TC_06_017_011]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite16_TC_06_018_001 IOApiTest.IoBlockWrite16_TC_06_018_001
 * @subsection IoBlockWrite16_TC_06_018_001_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite16_TC_06_018_001_tci Test Case ID
 * IoBlockWrite_S1_N_TC_06_018_001
 * @subsection IoBlockWrite16_TC_06_018_001_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207, \#199211
 * @subsection IoBlockWrite16_TC_06_018_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - "device_list" contain the device list of each "device_type" in "devices".
 * @subsection IoBlockWrite16_TC_06_018_001_tcs Test Case Summary
 * Run this test when 16bit Bus is supported.<br>
 * This test shall verify the functionality of the IoBlockWrite APIs of OSAL on normal condition:
 * - pre-process: open test target device, turn ON its power and clock
 * - Test06_018_001: use R_OSAL_IoBlockWrite16 to write data to register of test target device with offset "TEST_WRITEBLOCK16_REG_OFFSET_NC".
 * - post-process: write back initialization value, turn OFF clock and close test target device
 * @subsection IoBlockWrite16_TC_06_018_001_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read initialization data from test target device with offset "TEST_WRITEBLOCK16_REG_OFFSET_NC" by using R_OSAL_IoBlockRead16.
 * -# Write "register_data16" to test target device with offset "TEST_WRITEBLOCK16_REG_OFFSET_NC" and size is 16 by using R_OSAL_IoBlockWrite16
 * -# Read-back data from test target device with offset "TEST_WRITEBLOCK16_REG_OFFSET_NC" and size is 16 by using R_OSAL_IoBlockRead16.
 * -# Write back initialization data to test target device with offset "TEST_WRITEBLOCK16_REG_OFFSET_NC" by using R_OSAL_IoBlockWrite16.
 * -# Set the clock of test target device status to "OFF":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockWrite16_TC_06_018_001_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockRead16 return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockWrite16 return OSAL_RETURN_OK
 * -# R_OSAL_IoBlockRead16 return OSAL_RETURN_OK.<br>
 * The read-back data is the same as write data.
 * -# R_OSAL_IoBlockWrite16 return OSAL_RETURN_OK.
 * -# Set clock to "OFF" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoBlockWrite16_TC_06_018_001_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite16_TC_06_018_001
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockWrite16_TC_06_018_001]
TEST_F(IOApiTest, IoBlockWrite16_TC_06_018_001)
{
    uint16_t init_data16[8];
    uintptr_t offset = TEST_WRITEBLOCK16_REG_OFFSET_NC;
    size_t size = 16;
    uint16_t read_data16[8];
    uint16_t register_data16[8] = {TEST_WRITEBLOCK16_VAL_NC[0], TEST_WRITEBLOCK16_VAL_NC[1],
                                   TEST_WRITEBLOCK16_VAL_NC[2], TEST_WRITEBLOCK16_VAL_NC[3],
                                   TEST_WRITEBLOCK16_VAL_NC[4], TEST_WRITEBLOCK16_VAL_NC[5],
                                   TEST_WRITEBLOCK16_VAL_NC[6], TEST_WRITEBLOCK16_VAL_NC[7]};

    for (auto it = device_list.begin(); it != device_list.end(); it++)
    {
        if (!strcmp((*it).c_str(), TEST_IO_DEVICE_1))
        {
            // pre-process: open test target device, turn ON its power and clock
            EXPECT_EQ(R_OSAL_IoDeviceOpen((*it).c_str(), &device_handle), OSAL_RETURN_OK) << "pre-process failed";
            EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
            EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";
            EXPECT_EQ(R_OSAL_IoBlockRead16(device_handle, offset, init_data16, size), OSAL_RETURN_OK) << "pre-process failed";

            // @brief: normal case: write register value of test target device with aligned offset.
            // Test06_018_001
            EXPECT_EQ(R_OSAL_IoBlockWrite16(device_handle, offset, register_data16, size), OSAL_RETURN_OK) << "Test06_018_001 failed";
            EXPECT_EQ(R_OSAL_IoBlockRead16(device_handle, offset, read_data16, size), OSAL_RETURN_OK) << "Test06_018_001 failed";
            EXPECT_TRUE(equal(begin(register_data16), end(register_data16), begin(read_data16))) << "Test06_018_001 failed";

            // post-process: write back initialization value, turn OFF clock and close test target device
            EXPECT_EQ(R_OSAL_IoBlockWrite16(device_handle, offset, init_data16, size), OSAL_RETURN_OK) << "post-process failed";
            EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
            EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
            device_handle = OSAL_DEVICE_HANDLE_INVALID;
        }
    }
}
// [IoBlockWrite16_TC_06_018_001]
/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite16_TC_06_018_002 IOApiTest.IoBlockWrite16_TC_06_018_002
 * @subsection IoBlockWrite16_TC_06_018_002_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite16_TC_06_018_002_tci Test Case ID
 * IoBlockWrite_S0_A_TC_06_018_002
 * @subsection IoBlockWrite16_TC_06_018_002_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockWrite16_TC_06_018_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockWrite16_TC_06_018_002_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockRead APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_018_002: use R_OSAL_IoBlockWrite16 to write data to register of test target device with un-aligned offset.
 * - post-process: close test target device
 * @subsection IoBlockWrite16_TC_06_018_002_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Write data with offset "TEST_WRITEBLOCK16_OFFSET_UNALIGNED" by using R_OSAL_IoBlockWrite16
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockWrite16_TC_06_018_002_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockWrite16 return OSAL_RETURN_PAR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoBlockWrite16_TC_06_018_002_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite16_TC_06_018_002
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockWrite16_TC_06_018_002]
TEST_F(IOApiTest, IoBlockWrite16_TC_06_018_002)
{
    uintptr_t offset;
    size_t size = 16;
    uint16_t register_data16[8] = {TEST_WRITEBLOCK16_VAL_AC[0], TEST_WRITEBLOCK16_VAL_AC[1],
                                   TEST_WRITEBLOCK16_VAL_AC[2], TEST_WRITEBLOCK16_VAL_AC[3],
                                   TEST_WRITEBLOCK16_VAL_AC[4], TEST_WRITEBLOCK16_VAL_AC[5],
                                   TEST_WRITEBLOCK16_VAL_AC[6], TEST_WRITEBLOCK16_VAL_AC[7]};

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: write register value of test target device with un-aligned offset.
    // Test06_018_002
    offset = TEST_WRITEBLOCK16_OFFSET_UNALIGNED;
    EXPECT_EQ(R_OSAL_IoBlockWrite16(device_handle, offset, register_data16, size), OSAL_RETURN_PAR) << "Test06_018_002 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockWrite16_TC_06_018_002]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite16_TC_06_018_003 IOApiTest.IoBlockWrite16_TC_06_018_003
 * @subsection IoBlockWrite16_TC_06_018_003_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite16_TC_06_018_003_tci Test Case ID
 * IoBlockWrite_S0_A_TC_06_018_003
 * @subsection IoBlockWrite16_TC_06_018_003_rr Related Requirement(s)
 * \#199151, \#199199, \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockWrite16_TC_06_018_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockWrite16_TC_06_018_003_tcs Test Case Summary
 * This test shall verify the functionality of the IoWrite APIs of OSAL on abnormal condition:
 * - Test06_018_003: use R_OSAL_IoBlockWrite16 to write register value with device handle is NULL
 * @subsection IoBlockWrite16_TC_06_018_003_tpts Test Procedure/Test Steps
 * -# Write data with offset "TEST_WRITEBLOCK16_REG_OFFSET_AC" by using R_OSAL_IoBlockWrite16.
 * @subsection IoBlockWrite16_TC_06_018_003_ereb Expected Result/Behavior
 * -# R_OSAL_IoBlockWrite16 return OSAL_RETURN_HANDLE
 * @subsection IoBlockWrite16_TC_06_018_003_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite16_TC_06_018_003
 */
// [IoBlockWrite16_TC_06_018_003]
TEST_F(IOApiTest, IoBlockWrite16_TC_06_018_003)
{
    uintptr_t offset;
    size_t size = 16;
    uint16_t register_data16[8] = {TEST_WRITEBLOCK16_VAL_AC[0], TEST_WRITEBLOCK16_VAL_AC[1],
                                   TEST_WRITEBLOCK16_VAL_AC[2], TEST_WRITEBLOCK16_VAL_AC[3],
                                   TEST_WRITEBLOCK16_VAL_AC[4], TEST_WRITEBLOCK16_VAL_AC[5],
                                   TEST_WRITEBLOCK16_VAL_AC[6], TEST_WRITEBLOCK16_VAL_AC[7]};

    // @brief: abnormal case: write register value with device handle is NULL
    // Test06_018_003
    offset = TEST_WRITEBLOCK16_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoBlockWrite16(OSAL_DEVICE_HANDLE_INVALID, offset, register_data16, size), OSAL_RETURN_HANDLE) << "Test06_018_003 failed";
}
// [IoBlockWrite16_TC_06_018_003]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite16_TC_06_018_004 IOApiTest.IoBlockWrite16_TC_06_018_004
 * @subsection IoBlockWrite16_TC_06_018_004_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite16_TC_06_018_004_tci Test Case ID
 * IoBlockWrite_S0_BA_TC_06_018_004
 * @subsection IoBlockWrite16_TC_06_018_004_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockWrite16_TC_06_018_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockWrite16_TC_06_018_004_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockWrite APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_018_004: use R_OSAL_IoBlockWrite16 to write data to register of test target device with offset which is out-of-register-range.
 * - post-process: close test target device
 * @subsection IoBlockWrite16_TC_06_018_004_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Write data with offset "TEST_OFFSET_OUT_RANGE" by using R_OSAL_IoBlockWrite16
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockWrite16_TC_06_018_004_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockWrite16 return OSAL_RETURN_PAR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoBlockWrite16_TC_06_018_004_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite16_TC_06_018_004
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockWrite16_TC_06_018_004]
TEST_F(IOApiTest, IoBlockWrite16_TC_06_018_004)
{
    uintptr_t offset;
    size_t size = 16;
    uint16_t register_data16[8] = {TEST_WRITEBLOCK16_VAL_AC[0], TEST_WRITEBLOCK16_VAL_AC[1],
                                   TEST_WRITEBLOCK16_VAL_AC[2], TEST_WRITEBLOCK16_VAL_AC[3],
                                   TEST_WRITEBLOCK16_VAL_AC[4], TEST_WRITEBLOCK16_VAL_AC[5],
                                   TEST_WRITEBLOCK16_VAL_AC[6], TEST_WRITEBLOCK16_VAL_AC[7]};

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: write register value of test target device with offset which is out-of-register-range.
    // Test06_018_004
    offset = TEST_OFFSET_OUT_RANGE;
    EXPECT_EQ(R_OSAL_IoBlockWrite16(device_handle, offset, register_data16, size), OSAL_RETURN_PAR) << "Test06_018_004 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockWrite16_TC_06_018_004]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite16_TC_06_018_005 IOApiTest.IoBlockWrite16_TC_06_018_005
 * @subsection IoBlockWrite16_TC_06_018_005_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite16_TC_06_018_005_tci Test Case ID
 * IoBlockWrite_S0_A_TC_06_018_005
 * @subsection IoBlockWrite16_TC_06_018_005_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockWrite16_TC_06_018_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockWrite16_TC_06_018_005_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockWrite APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_018_005: use R_OSAL_IoBlockWrite64 to write register value of test target device with p_data is NULL
 * - post-process: close test target device
 * @subsection IoBlockWrite16_TC_06_018_005_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Write data with offset "TEST_WRITEBLOCK16_REG_OFFSET_AC" by using R_OSAL_IoBlockWrite64
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockWrite16_TC_06_018_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockWrite64 return OSAL_RETURN_PAR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoBlockWrite16_TC_06_018_005_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite16_TC_06_018_005
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockWrite16_TC_06_018_005]
TEST_F(IOApiTest, IoBlockWrite16_TC_06_018_005)
{
    uintptr_t offset;
    size_t size = 16;
    void* p_data = NULL;

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: write register value of test target device with p_data is NULL
    // Test06_018_005
    offset = TEST_WRITEBLOCK16_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoBlockWrite16(device_handle, offset, (uint16_t*)p_data, size), OSAL_RETURN_PAR) << "Test06_018_005 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockWrite16_TC_06_018_005]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite16_TC_06_018_006 IOApiTest.IoBlockWrite16_TC_06_018_006
 * @subsection IoBlockWrite16_TC_06_018_006_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite16_TC_06_018_006_tci Test Case ID
 * IoBlockWrite_S0_BA_TC_06_018_006
 * @subsection IoBlockWrite16_TC_06_018_006_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockWrite16_TC_06_018_006_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockWrite16_TC_06_018_006_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockWrite APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_018_006: use R_OSAL_IoBlockWrite16 to write data to register of test target device with size is out-of-range of register
 * - post-process: close test target device
 * @subsection IoBlockWrite16_TC_06_018_006_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Write data with offset "TEST_WRITEBLOCK16_REG_OFFSET_AC" by using R_OSAL_IoBlockWrite16
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockWrite16_TC_06_018_006_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockWrite16 return OSAL_RETURN_PAR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoBlockWrite16_TC_06_018_006_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite16_TC_06_018_006
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockWrite16_TC_06_018_006]
TEST_F(IOApiTest, IoBlockWrite16_TC_06_018_006)
{
    uintptr_t offset;
    size_t size = TEST_IO_SIZE_OUT_RANGE;
    uint16_t register_data16[8] = {TEST_WRITEBLOCK16_VAL_AC[0], TEST_WRITEBLOCK16_VAL_AC[1],
                                   TEST_WRITEBLOCK16_VAL_AC[2], TEST_WRITEBLOCK16_VAL_AC[3],
                                   TEST_WRITEBLOCK16_VAL_AC[4], TEST_WRITEBLOCK16_VAL_AC[5],
                                   TEST_WRITEBLOCK16_VAL_AC[6], TEST_WRITEBLOCK16_VAL_AC[7]};

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: write register value of test target device with size is out-of-range of register
    // Test06_018_006
    offset = TEST_WRITEBLOCK16_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoBlockWrite16(device_handle, offset, register_data16, size), OSAL_RETURN_PAR) << "Test06_018_006 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockWrite16_TC_06_018_006]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite16_TC_06_018_007 IOApiTest.IoBlockWrite16_TC_06_018_007
 * @subsection IoBlockWrite16_TC_06_018_007_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite16_TC_06_018_007_tci Test Case ID
 * IoBlockWrite_S2_A_TC_06_018_007
 * @subsection IoBlockWrite16_TC_06_018_007_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockWrite16_TC_06_018_007_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockWrite16_TC_06_018_007_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockWrite APIs of OSAL on abnormal condition:
 * - Test06_018_007: use R_OSAL_IoBlockWrite16 write register value with closed device handle
 * @subsection IoBlockWrite16_TC_06_018_007_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * -# Write data with offset "TEST_WRITEBLOCK16_REG_OFFSET_AC" by using R_OSAL_IoBlockWrite16.
 * @subsection IoBlockWrite16_TC_06_018_007_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_IoBlockWrite16 return OSAL_RETURN_HANDLE
 * @subsection IoBlockWrite16_TC_06_018_007_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite16_TC_06_018_007
 */
// [IoBlockWrite16_TC_06_018_007]
TEST_F(IOApiTest, IoBlockWrite16_TC_06_018_007)
{
    uintptr_t offset;
    size_t size = 16;
    uint16_t register_data16[8] = {TEST_WRITEBLOCK16_VAL_AC[0], TEST_WRITEBLOCK16_VAL_AC[1],
                                   TEST_WRITEBLOCK16_VAL_AC[2], TEST_WRITEBLOCK16_VAL_AC[3],
                                   TEST_WRITEBLOCK16_VAL_AC[4], TEST_WRITEBLOCK16_VAL_AC[5],
                                   TEST_WRITEBLOCK16_VAL_AC[6], TEST_WRITEBLOCK16_VAL_AC[7]};

    // @brief: abnormal case: Write register value with closed device handle
    // Test06_018_007
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "Test06_018_007 failed";
    EXPECT_NE(device_handle, NULL_PTR) << "Test06_018_007 failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "Test06_018_007 failed";

    offset = TEST_WRITEBLOCK16_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoBlockWrite16(device_handle, offset, register_data16, size), OSAL_RETURN_HANDLE) << "Test06_018_007 failed";
}
// [IoBlockWrite16_TC_06_018_007]

/**
 * @page IO_TEST IO API Test
 * @section OsalApiStateTest_IoBlockWrite16_TC_06_018_008 OsalApiStateTest.IoBlockWrite16_TC_06_018_008
 * @subsection IoBlockWrite16_TC_06_018_008_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection IoBlockWrite16_TC_06_018_008_tci Test Case ID
 * IoBlockWrite_S2_A_TC_06_018_008
 * @subsection IoBlockWrite16_TC_06_018_008_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockWrite16_TC_06_018_008_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize has already been called
 * @subsection IoBlockWrite16_TC_06_018_008_tcs Test Case Summary
 * This test shall verify the state transition of the R_OSAL_IoBlockWrite16 function:
 * - Test06_018_008: confirm state transition error with R_OSAL_IoBlockWrite16
 * @subsection IoBlockWrite16_TC_06_018_008_tpts Test Procedure/Test Steps
 * -# Confirm the state transition of R_OSAL_IoBlockWrite16
 * @subsection IoBlockWrite16_TC_06_018_008_ereb Expected Result/Behavior
 * -# R_OSAL_IoBlockWrite16return OSAL_RETURN_STATE
 * @subsection IoBlockWrite16_TC_06_018_008_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite16_TC_06_018_008
 */
// [IoBlockWrite16_TC_06_018_008]
TEST_F(OsalApiStateTest, IoBlockWrite16_TC_06_018_008)
{
    uint16_t register_data16[8] = {TEST_WRITEBLOCK16_VAL_AC[0], TEST_WRITEBLOCK16_VAL_AC[1],
                                   TEST_WRITEBLOCK16_VAL_AC[2], TEST_WRITEBLOCK16_VAL_AC[3],
                                   TEST_WRITEBLOCK16_VAL_AC[4], TEST_WRITEBLOCK16_VAL_AC[5],
                                   TEST_WRITEBLOCK16_VAL_AC[6], TEST_WRITEBLOCK16_VAL_AC[7]};
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;
    uintptr_t offset = TEST_WRITEBLOCK16_REG_OFFSET_AC;
    size_t size = 16;

    // Test06_018_008
    // @brief: abnormal case: state transition error with IO BlockWrite
    EXPECT_EQ(R_OSAL_IoBlockWrite16(device_handle, offset, register_data16, size), OSAL_RETURN_STATE) << "Test06_018_008 failed";
}
// [IoBlockWrite16_TC_06_018_008]

/**
 * @page IO_TEST IO API Test
 * @section OSALINTTESTS_IoBlockWrite16_TC_06_018_009 OSALINTTESTS.IoBlockWrite16_TC_06_018_009
 * @subsection IoBlockWrite16_TC_06_018_009_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection IoBlockWrite16_TC_06_018_009_tci Test Case ID
 * IoBlockWrite_S1_N_TC_06_018_009
 * @subsection IoBlockWrite16_TC_06_018_009_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207, \#199211
 * @subsection IoBlockWrite16_TC_06_018_009_pp Prerequisite/Precondition
 * -# OSAL is initialized.<br>
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection IoBlockWrite16_TC_06_018_009_tcs Test Case Summary
 * Run this test when 16bit Bus is supported.
 * @subsubsection IoBlockWrite16_TC_06_018_009_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Setup required memory for interrupt generation.
 * - Register callack function and enable interrupt.
 * - Wait for the interrupt callback function finish.
 * @subsubsection IoBlockWrite16_TC_06_018_009_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * @subsubsection IoBlockWrite16_TC_06_018_009_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality of the IO APIs which are called in the ISR
 * - Disable and clear interrupt status
 * - Test06_018_009: use R_OSAL_IoBlockWrite with 16 bit
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection IoBlockWrite16_TC_06_018_009_tpts Test Procedure/Test Steps
 * @subsubsection IoBlockWrite16_TC_06_018_009_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Allocate required memory for generate interrupt from test target device and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function by using R_OSAL_InterruptRegisterIsr.<br>
 * -# Enable interrupt by using R_OSAL_InterruptEnableIsr
 * -# Setup test target device specific register to generate the interrupt by calling to setup_hw_register
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection IoBlockWrite16_TC_06_018_009_post_tpts Test Procedure/Test Steps of Post-process
 * -# Unregister interrupt by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * @subsubsection IoBlockWrite16_TC_06_018_009_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Read initialization data from test target device with offset "TEST_WRITEBLOCK16_REG_OFFSET_INT" by using R_OSAL_IoBlockRead16.
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Write "registerblock_data16" to test target device with offset "TEST_WRITEBLOCK16_REG_OFFSET_INT" and size is 16 by using R_OSAL_IoBlockWrite16
 * -# Read-back data from test target device with offset "TEST_WRITEBLOCK16_REG_OFFSET_INT" and size is 16 by using R_OSAL_IoBlockRead16.
 * -# Write back initialization data to test target device with offset "TEST_WRITEBLOCK16_REG_OFFSET_INT" by using R_OSAL_IoBlockWrite16.
 * -# Send Message interrupt of context finished by using R_OSAL_MqSendForTimePeriod
 * @subsection IoBlockWrite16_TC_06_018_009_ereb Expected Result/Behavior
 * @subsubsection IoBlockWrite16_TC_06_018_009_pre_ereb Expected Result/Behavior of Pre-process
 * -# allocate_setup_data_memory run successfully
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * The value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK
 * -# setup_hw_register run successfully
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection IoBlockWrite16_TC_06_018_009_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * @subsubsection IoBlockWrite16_TC_06_018_009_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK
 * -# R_OSAL_IoBlockRead16 return OSAL_RETURN_OK.
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_IoBlockWrite16 return OSAL_RETURN_OK
 * -# R_OSAL_IoBlockRead16 return OSAL_RETURN_OK.<br>
 * The read-back data is the same as write data.
 * -# R_OSAL_IoBlockWrite16 return OSAL_RETURN_OK.
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * @subsection IoBlockWrite16_TC_06_018_009_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite16_TC_06_018_009
 * @snippet test_io.cpp osal_io_isr_test_TC_06_018_009
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [IoBlockWrite16_TC_06_018_009]
TEST_F(OSALINTTESTS, IoBlockWrite16_TC_06_018_009)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    st_io_arg_t io_arg;
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
 * Wait for the interrupt callback function finish. */
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_GT(numOfChannel, 0) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_io_isr_test_TC_06_018_009, (void *)&arg), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK) << "pre-process failed";
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory*/
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_io_isr_test_TC_06_018_009), OSAL_RETURN_OK) << "post-process failed";
    deallocate_data_memory(hndl_mmngr, buffer_obj);
}
// [IoBlockWrite16_TC_06_018_009]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite16_TC_06_018_010 IOApiTest.IoBlockWrite16_TC_06_018_010
 * @subsection IoBlockWrite16_TC_06_018_010_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite16_TC_06_018_010_tci Test Case ID
 * IoBlockWrite_S1_N_TC_06_018_010
 * @subsection IoBlockWrite16_TC_06_018_010_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207, \#199211
 * @subsection IoBlockWrite16_TC_06_018_010_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockWrite16_TC_06_018_010_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockWrite APIs of OSAL on normal condition:
 * - pre-process: open test target device, turn ON its power and clock
 * - Test06_018_010: use R_OSAL_IoBlockWrite16 to write data to register of test target device with offset "TEST_WRITEBLOCK16_REG_OFFSET_NC" and size is 0.
 * - post-process: turn OFF clock and close test target device
 * @subsection IoBlockWrite16_TC_06_018_010_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data from test target device with offset "TEST_WRITEBLOCK16_REG_OFFSET_NC" and size is 16 by using R_OSAL_IoBlockRead16.
 * -# Write "register_data16" to test target device with offset "TEST_WRITEBLOCK16_REG_OFFSET_NC" and size is 0 by using R_OSAL_IoBlockWrite16
 * -# Read-back data from test target device with offset "TEST_WRITEBLOCK16_REG_OFFSET_NC" and size is 16 by using R_OSAL_IoBlockRead16.
 * -# Set the clock of test target device status to "OFF":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockWrite16_TC_06_018_010_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockRead16 return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockWrite16 return OSAL_RETURN_OK
 * -# R_OSAL_IoBlockRead16 return OSAL_RETURN_OK.<br>
 * The value of "read_data_before" is equal to "read_data_after".
 * -# Set clock to "OFF" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoBlockWrite16_TC_06_018_010_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite16_TC_06_018_010
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockWrite16_TC_06_018_010]
TEST_F(IOApiTest, IoBlockWrite16_TC_06_018_010)
{
    uint16_t read_data_before[8];
    uint16_t read_data_after[8];
    uint16_t register_data16[8] = {TEST_WRITEBLOCK16_VAL_NC[0], TEST_WRITEBLOCK16_VAL_NC[1],
                                   TEST_WRITEBLOCK16_VAL_NC[2], TEST_WRITEBLOCK16_VAL_NC[3],
                                   TEST_WRITEBLOCK16_VAL_NC[4], TEST_WRITEBLOCK16_VAL_NC[5],
                                   TEST_WRITEBLOCK16_VAL_NC[6], TEST_WRITEBLOCK16_VAL_NC[7]};

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: normal case: write register value of test target device with size is 0.
    // Test06_018_010
    EXPECT_EQ(R_OSAL_IoBlockRead16(device_handle, TEST_WRITEBLOCK16_REG_OFFSET_NC, read_data_before, TEST_IO_ACCESS_SIZE), OSAL_RETURN_OK) << "Test06_018_010 failed";
    EXPECT_EQ(R_OSAL_IoBlockWrite16(device_handle, TEST_WRITEBLOCK16_REG_OFFSET_NC, register_data16, TEST_IO_SIZE_ZERO), OSAL_RETURN_OK) << "Test06_018_010 failed";
    EXPECT_EQ(R_OSAL_IoBlockRead16(device_handle, TEST_WRITEBLOCK16_REG_OFFSET_NC, read_data_after, TEST_IO_ACCESS_SIZE), OSAL_RETURN_OK) << "Test06_018_010 failed";
    EXPECT_TRUE(equal(begin(read_data_before), end(read_data_before), begin(read_data_after))) << "Test06_018_010 failed";

    // post-process: turn OFF clock and close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockWrite16_TC_06_018_010]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite16_TC_06_018_011 IOApiTest.IoBlockWrite16_TC_06_018_011
 * @subsection IoBlockWrite16_TC_06_018_011_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite16_TC_06_018_011_tci Test Case ID
 * IoBlockWrite16_S1_N_TC_06_018_011
 * @subsection IoBlockWrite16_TC_06_018_011_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207, \#199211
 * @subsection IoBlockWrite16_TC_06_018_011_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - The synchronization queue was created
 * @subsection IoBlockWrite16_TC_06_018_011_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoBlockWrite16/R_OSAL_IoBlockRead16 function with the same device handle in multi-threads:
 * - Test06_018_011: Create 2 threads to write and read to register of the same test target device.
 * @subsection IoBlockWrite16_TC_06_018_011_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Turn ON power and clock of the target device by calling to set_clock_on
 * -# Create first thread using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE3
 *          -# task_name is "Block Read/write 16 bit value from/to device register in multi-threads"
 *          -# func is "io_block_read_write_16_device_child_thread" function with functionality:
 *              - Get config of synchronization queue by using R_OSAL_MqGetConfig
 *              - Read initialization data from test target device with offset "TEST_WRITEBLOCK16_REG_OFFSET_NC" by using R_OSAL_IoBlockRead16.
 *              - Write data is "register_data16" to test target device with offset "TEST_WRITEBLOCK16_REG_OFFSET_NC" by using R_OSAL_IoBlockWrite16.
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *          -# userarg includes device handle and thread id
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2
 *      - Address of first thread handle.
 * -# Receive message from synchronization queue by using R_OSAL_MqReceiveForTimePeriod
 * -# Create second thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE3
 *          -# task_name is "Block Read/write 16 bit value from/to device register in multi-threads"
 *          -# func is "io_block_read_write_16_device_child_thread" function with functionality:
 *              - Read data from test target device with offset "TEST_WRITEBLOCK16_REG_OFFSET_NC" by using R_OSAL_IoBlockRead16.
 *              - Write back initialization data to test target device with offset "TEST_WRITEBLOCK16_REG_OFFSET_NC" by using R_OSAL_IoBlockWrite16.
 *          -# userarg includes device handle and thread id
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID3
 *      - Address of second thread handle.
 * -# Turn OFF power and clock domain of target device by calling to set_clock_off
 * -# Join first thread by using R_OSAL_ThreadJoin.
 * -# Join second thread by using R_OSAL_ThreadJoin.
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockWrite16_TC_06_018_011_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with first thread.<br>
 * "thread_handle1" is not NULL_PTR.<br>
 * In io_block_read_write_16_device_child_thread function:
 *  - R_OSAL_MqGetConfig return OSAL_RETURN_OK.
 *  - R_OSAL_IoBlockRead16 return OSAL_RETURN_OK.
 *  - R_OSAL_IoBlockWrite16 return OSAL_RETURN_OK.
 *  - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK.
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK.<br>
 * The value of "receive_msg" need to be equal to "send_msg"
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with second thread.<br>
 * "thread_handle2" is not NULL_PTR.<br>
 * In io_block_read_write_16_device_child_thread function:
 *  - R_OSAL_IoBlockRead16 return OSAL_RETURN_OK.<br>
 * "read_data16" is equal to "register_data16".
 *  - R_OSAL_IoBlockWrite16 return OSAL_RETURN_OK.
 * -# Set power and clock to "OFF" successfully
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with first thread.<br>
 * Value of "result" is equal to OSAL_RETURN_OK.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with second thread.<br>
 * Value of "result" is equal to OSAL_RETURN_OK.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoBlockWrite16_TC_06_018_011_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite16_TC_06_018_011
 * @snippet test_io.cpp io_block_read_write_16_device_child_thread
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockWrite16_TC_06_018_011]
TEST_F(IOApiTest, IoBlockWrite16_TC_06_018_011)
{

    st_osal_thread_config_t config_thread;
    int64_t                 result          = 0;
    osal_thread_handle_t    thread_handle1  = OSAL_THREAD_HANDLE_INVALID;
    osal_thread_handle_t    thread_handle2  = OSAL_THREAD_HANDLE_INVALID;

    // Test06_018_011
    // @brief: normal case: Block read write register 16-bit in multi-threads.
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";

    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "Test06_018_011 failed";

    io_test_multiple_thread_worker_arg user_arg1(&device_handle, TEST_THREAD_ID2);
    user_arg1.send_msg = send_msg;
    user_arg1.mq = sync_mq;
    config_thread.userarg       = &user_arg1;
    config_thread.priority      = OSAL_THREAD_PRIORITY_TYPE3;
    config_thread.task_name     = (char *)"Block Read/write 16 bit value from/to device register in multi-threads";
    config_thread.func          = io_block_read_write_16_device_child_thread;
    config_thread.stack_size    = THREAD_STACK_SIZE;
    EXPECT_EQ(R_OSAL_ThreadCreate(&config_thread, TEST_THREAD_ID2, &thread_handle1), OSAL_RETURN_OK) << "Test06_018_011 failed";
    EXPECT_NE(thread_handle1, NULL_PTR) << "Test06_018_011 failed";
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &receive_msg, mq_config.msg_size), OSAL_RETURN_OK) << "Test06_018_011 failed";
    EXPECT_EQ(receive_msg, send_msg) << "Test06_018_011 failed";

    user_arg1.uid              = TEST_THREAD_ID3;
    EXPECT_EQ(R_OSAL_ThreadCreate(&config_thread, TEST_THREAD_ID3, &thread_handle2), OSAL_RETURN_OK) << "Test06_018_011 failed";
    EXPECT_NE(thread_handle2, NULL_PTR) << "Test06_018_011 failed";

    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "Test06_018_011 failed";

    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &result),OSAL_RETURN_OK) << "Test06_018_011 failed";
    EXPECT_EQ(result, (int64_t)OSAL_RETURN_OK) << "Test06_018_011 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle2, &result),OSAL_RETURN_OK) << "Test06_018_011 failed";
    EXPECT_EQ(result, (int64_t)OSAL_RETURN_OK) << "Test06_018_011 failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockWrite16_TC_06_018_011]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite32_TC_06_019_001 IOApiTest.IoBlockWrite32_TC_06_019_001
 * @subsection IoBlockWrite32_TC_06_019_001_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite32_TC_06_019_001_tci Test Case ID
 * IoBlockWrite_S1_N_TC_06_019_001
 * @subsection IoBlockWrite32_TC_06_019_001_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207, \#199211
 * @subsection IoBlockWrite32_TC_06_019_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - "device_list" contain the device list of each "device_type" in "devices".
 * @subsection IoBlockWrite32_TC_06_019_001_tcs Test Case Summary
 * Run this test when 32bit Bus is supported<br>
 * This test shall verify the functionality of the IoBlockWrite APIs of OSAL on normal condition:
 * - pre-process: open test target device, turn ON its power and clock
 * - Test06_019_001: use R_OSAL_IoBlockWrite32 to write data to register of test target device with offset "TEST_WRITEBLOCK32_REG_OFFSET_NC".
 * - post-process: write back initialization value, turn OFF clock and close test target device
 * @subsection IoBlockWrite32_TC_06_019_001_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read initialization data from test target device with offset "TEST_WRITEBLOCK32_REG_OFFSET_NC" by using R_OSAL_IoBlockRead32.
 * -# Write "register_data32" to test target device with offset "TEST_WRITEBLOCK32_REG_OFFSET_NC" and size is 16 by using R_OSAL_IoBlockWrite32
 * -# Read-back data from test target device with offset "TEST_WRITEBLOCK32_REG_OFFSET_NC" and size is 16 by using R_OSAL_IoBlockRead32.
 * -# Write back initialization data to test target device with offset "TEST_WRITEBLOCK32_REG_OFFSET_NC" by using R_OSAL_IoBlockWrite32.
 * -# Set the clock of test target device status to "OFF":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockWrite32_TC_06_019_001_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockRead32 return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_IoBlockRead32 return OSAL_RETURN_OK.<br>
 * The read-back data is the same as write data.
 * -# R_OSAL_IoBlockWrite32 return OSAL_RETURN_OK.
 * -# Set clock to "OFF" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoBlockWrite32_TC_06_019_001_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite32_TC_06_019_001
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockWrite32_TC_06_019_001]
TEST_F(IOApiTest, IoBlockWrite32_TC_06_019_001)
{
    uint32_t init_data32[4];
    uintptr_t offset = TEST_WRITEBLOCK32_REG_OFFSET_NC;
    size_t size = 16;
    uint32_t read_data32[4];
    uint32_t register_data32[4] = {TEST_WRITEBLOCK32_VAL_NC[0], TEST_WRITEBLOCK32_VAL_NC[1], TEST_WRITEBLOCK32_VAL_NC[2], TEST_WRITEBLOCK32_VAL_NC[3]};

    for (auto it = device_list.begin(); it != device_list.end(); it++)
    {
        if (!strcmp((*it).c_str(), TEST_IO_DEVICE_1))
        {
            // pre-process: open test target device, turn ON its power and clock
            EXPECT_EQ(R_OSAL_IoDeviceOpen((*it).c_str(), &device_handle), OSAL_RETURN_OK) << "pre-process failed";
            EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
            EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";
            EXPECT_EQ(R_OSAL_IoBlockRead32(device_handle, offset, init_data32, size), OSAL_RETURN_OK) << "pre-process failed";

            // @brief: normal case: write register value of test target device with aligned offset.
            // Test06_019_001
            EXPECT_EQ(R_OSAL_IoBlockWrite32(device_handle, offset, register_data32, size), OSAL_RETURN_OK) << "Test06_019_001 failed";
            EXPECT_EQ(R_OSAL_IoBlockRead32(device_handle, offset, read_data32, size), OSAL_RETURN_OK) << "Test06_019_001 failed";
            EXPECT_TRUE(equal(begin(register_data32), end(register_data32), begin(read_data32))) << "Test06_019_001 failed";

            // post-process: write back initialization value, turn OFF clock and close test target device
            EXPECT_EQ(R_OSAL_IoBlockWrite32(device_handle, offset, init_data32, size), OSAL_RETURN_OK) << "post-process failed";
            EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
            EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
            device_handle = OSAL_DEVICE_HANDLE_INVALID;
        }
    }
}
// [IoBlockWrite32_TC_06_019_001]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite32_TC_06_019_002 IOApiTest.IoBlockWrite32_TC_06_019_002
 * @subsection IoBlockWrite32_TC_06_019_002_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite32_TC_06_019_002_tci Test Case ID
 * IoBlockWrite_S0_A_TC_06_019_002
 * @subsection IoBlockWrite32_TC_06_019_002_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockWrite32_TC_06_019_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockWrite32_TC_06_019_002_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockRead APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_019_002: use R_OSAL_IoBlockWrite32 to write data to register of test target device with un-aligned offset.
 * - post-process: turn OFF clock and close test target device
 * @subsection IoBlockWrite32_TC_06_019_002_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Write data with offset "TEST_WRITEBLOCK32_OFFSET_UNALIGNED" by using R_OSAL_IoBlockWrite32
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockWrite32_TC_06_019_002_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockWrite32 return OSAL_RETURN_PAR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoBlockWrite32_TC_06_019_002_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite32_TC_06_019_002
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockWrite32_TC_06_019_002]
TEST_F(IOApiTest, IoBlockWrite32_TC_06_019_002)
{
    uintptr_t offset;
    size_t size = 16;
    uint32_t register_data32[4] = {TEST_WRITEBLOCK32_VAL_AC[0], TEST_WRITEBLOCK32_VAL_AC[1], TEST_WRITEBLOCK32_VAL_AC[2], TEST_WRITEBLOCK32_VAL_AC[3]};

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: write register value of test target device with un-aligned offset.
    // Test06_019_002
    offset = TEST_WRITEBLOCK32_OFFSET_UNALIGNED;
    EXPECT_EQ(R_OSAL_IoBlockWrite32(device_handle, offset, register_data32, size), OSAL_RETURN_PAR) << "Test06_019_002 failed";

    // post-process: turn OFF clock and close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockWrite32_TC_06_019_002]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite32_TC_06_019_003 IOApiTest.IoBlockWrite32_TC_06_019_003
 * @subsection IoBlockWrite32_TC_06_019_003_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite32_TC_06_019_003_tci Test Case ID
 * IoBlockWrite_S0_A_TC_06_019_003
 * @subsection IoBlockWrite32_TC_06_019_003_rr Related Requirement(s)
 * \#199151, \#199199, \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockWrite32_TC_06_019_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockWrite32_TC_06_019_003_tcs Test Case Summary
 * This test shall verify the functionality of the IoWrite APIs of OSAL on abnormal condition:
 * - Test06_019_003: use R_OSAL_IoBlockWrite32 to write register value with device handle is NULL
 * @subsection IoBlockWrite32_TC_06_019_003_tpts Test Procedure/Test Steps
 * -# Write data with offset "TEST_WRITEBLOCK32_REG_OFFSET_AC" by using R_OSAL_IoBlockWrite32.
 * @subsection IoBlockWrite32_TC_06_019_003_ereb Expected Result/Behavior
 * -# R_OSAL_IoBlockWrite32 return OSAL_RETURN_HANDLE
 * @subsection IoBlockWrite32_TC_06_019_003_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite32_TC_06_019_003
 */
// [IoBlockWrite32_TC_06_019_003]
TEST_F(IOApiTest, IoBlockWrite32_TC_06_019_003)
{
    uintptr_t offset;
    size_t size = 16;
    uint32_t register_data32[4] = {TEST_WRITEBLOCK32_VAL_AC[0], TEST_WRITEBLOCK32_VAL_AC[1], TEST_WRITEBLOCK32_VAL_AC[2], TEST_WRITEBLOCK32_VAL_AC[3]};

    // @brief: abnormal case: write register value with device handle is NULL
    // Test06_019_003
    offset = TEST_WRITEBLOCK32_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoBlockWrite32(OSAL_DEVICE_HANDLE_INVALID, offset, register_data32, size), OSAL_RETURN_HANDLE) << "Test06_019_003 failed";
}
// [IoBlockWrite32_TC_06_019_003]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite32_TC_06_019_004 IOApiTest.IoBlockWrite32_TC_06_019_004
 * @subsection IoBlockWrite32_TC_06_019_004_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite32_TC_06_019_004_tci Test Case ID
 * IoBlockWrite_S0_BA_TC_06_019_004
 * @subsection IoBlockWrite32_TC_06_019_004_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockWrite32_TC_06_019_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockWrite32_TC_06_019_004_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockWrite APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_019_004: use R_OSAL_IoBlockWrite32 to write data to register of test target device with offset which is out-of-register-range.
 * - post-process: close test target device
 * @subsection IoBlockWrite32_TC_06_019_004_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Write data with offset "TEST_OFFSET_OUT_RANGE" by using R_OSAL_IoBlockWrite32
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockWrite32_TC_06_019_004_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockWrite32 return OSAL_RETURN_PAR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoBlockWrite32_TC_06_019_004_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite32_TC_06_019_004
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockWrite32_TC_06_019_004]
TEST_F(IOApiTest, IoBlockWrite32_TC_06_019_004)
{
    uintptr_t offset;
    size_t size = 16;
    uint32_t register_data32[4] = {TEST_WRITEBLOCK32_VAL_AC[0], TEST_WRITEBLOCK32_VAL_AC[1], TEST_WRITEBLOCK32_VAL_AC[2], TEST_WRITEBLOCK32_VAL_AC[3]};

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: write register value of test target device with offset which is out-of-register-range.
    // Test06_019_004
    offset = TEST_OFFSET_OUT_RANGE;
    EXPECT_EQ(R_OSAL_IoBlockWrite32(device_handle, offset, register_data32, size), OSAL_RETURN_PAR) << "Test06_019_004 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockWrite32_TC_06_019_004]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite32_TC_06_019_005 IOApiTest.IoBlockWrite32_TC_06_019_005
 * @subsection IoBlockWrite32_TC_06_019_005_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite32_TC_06_019_005_tci Test Case ID
 * IoBlockWrite_S0_A_TC_06_019_005
 * @subsection IoBlockWrite32_TC_06_019_005_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockWrite32_TC_06_019_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockWrite32_TC_06_019_005_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockWrite APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_019_005: use R_OSAL_IoBlockWrite64 to write register value of test target device with p_data is NULL
 * - post-process: close test target device
 * @subsection IoBlockWrite32_TC_06_019_005_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Write data with offset "TEST_WRITEBLOCK32_REG_OFFSET_AC" by using R_OSAL_IoBlockWrite64
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockWrite32_TC_06_019_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockWrite64 return OSAL_RETURN_PAR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoBlockWrite32_TC_06_019_005_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite32_TC_06_019_005
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockWrite32_TC_06_019_005]
TEST_F(IOApiTest, IoBlockWrite32_TC_06_019_005)
{
    uintptr_t offset;
    size_t size = 16;
    void* p_data = NULL;

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: write register value of test target device with p_data is NULL
    // Test06_019_005
    offset = TEST_WRITEBLOCK32_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoBlockWrite32(device_handle, offset, (uint32_t*)p_data, size), OSAL_RETURN_PAR) << "Test06_019_005 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockWrite32_TC_06_019_005]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite32_TC_06_019_006 IOApiTest.IoBlockWrite32_TC_06_019_006
 * @subsection IoBlockWrite32_TC_06_019_006_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite32_TC_06_019_006_tci Test Case ID
 * IoBlockWrite_S0_BA_TC_06_019_006
 * @subsection IoBlockWrite32_TC_06_019_006_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockWrite32_TC_06_019_006_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockWrite32_TC_06_019_006_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockWrite APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_019_006: use R_OSAL_IoBlockWrite32 to write data to register of test target device with size is out-of-range of register
 * - post-process: close test target device
 * @subsection IoBlockWrite32_TC_06_019_006_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Write data with offset "TEST_WRITEBLOCK32_REG_OFFSET_AC" by using R_OSAL_IoBlockWrite32
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockWrite32_TC_06_019_006_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockWrite32 return OSAL_RETURN_PAR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoBlockWrite32_TC_06_019_006_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite32_TC_06_019_006
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockWrite32_TC_06_019_006]
TEST_F(IOApiTest, IoBlockWrite32_TC_06_019_006)
{
    uintptr_t offset;
    size_t size = TEST_IO_SIZE_OUT_RANGE;
    uint32_t register_data32[4] = {TEST_WRITEBLOCK32_VAL_AC[0], TEST_WRITEBLOCK32_VAL_AC[1], TEST_WRITEBLOCK32_VAL_AC[2], TEST_WRITEBLOCK32_VAL_AC[3]};

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: write register value of test target device with size is out-of-range of register
    // Test06_019_006
    offset = TEST_WRITEBLOCK32_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoBlockWrite32(device_handle, offset, register_data32, size), OSAL_RETURN_PAR) << "Test06_019_006 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockWrite32_TC_06_019_006]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite32_TC_06_019_007 IOApiTest.IoBlockWrite32_TC_06_019_007
 * @subsection IoBlockWrite32_TC_06_019_007_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite32_TC_06_019_007_tci Test Case ID
 * IoBlockWrite_S2_A_TC_06_019_007
 * @subsection IoBlockWrite32_TC_06_019_007_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockWrite32_TC_06_019_007_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockWrite32_TC_06_019_007_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockWrite APIs of OSAL on abnormal condition:
 * - Test06_019_007: use R_OSAL_IoBlockWrite32 write register value with closed device handle
 * @subsection IoBlockWrite32_TC_06_019_007_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * -# Write data with offset "TEST_WRITEBLOCK32_REG_OFFSET_AC" by using R_OSAL_IoBlockWrite32.
 * @subsection IoBlockWrite32_TC_06_019_007_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_IoBlockWrite32 return OSAL_RETURN_HANDLE
 * @subsection IoBlockWrite32_TC_06_019_007_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite32_TC_06_019_007
 */
// [IoBlockWrite32_TC_06_019_007]
TEST_F(IOApiTest, IoBlockWrite32_TC_06_019_007)
{
    uintptr_t offset;
    size_t size = 16;
    uint32_t register_data32[4] = {TEST_WRITEBLOCK32_VAL_AC[0], TEST_WRITEBLOCK32_VAL_AC[1], TEST_WRITEBLOCK32_VAL_AC[2], TEST_WRITEBLOCK32_VAL_AC[3]};

    // @brief: abnormal case: Write register value with closed device handle
    // Test06_019_007
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "Test06_019_007 failed";
    EXPECT_NE(device_handle, NULL_PTR) << "Test06_019_007 failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "Test06_019_007 failed";

    offset = TEST_WRITEBLOCK32_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoBlockWrite32(device_handle, offset, register_data32, size), OSAL_RETURN_HANDLE) << "Test06_019_007 failed";
}
// [IoBlockWrite32_TC_06_019_007]

/**
 * @page IO_TEST IO API Test
 * @section OsalApiStateTest_IoBlockWrite32_TC_06_019_008 OsalApiStateTest.IoBlockWrite32_TC_06_019_008
 * @subsection IoBlockWrite32_TC_06_019_008_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection IoBlockWrite32_TC_06_019_008_tci Test Case ID
 * IoBlockWrite_S2_A_TC_06_019_008
 * @subsection IoBlockWrite32_TC_06_019_008_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockWrite32_TC_06_019_008_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize has already been called
 * @subsection IoBlockWrite32_TC_06_019_008_tcs Test Case Summary
 * This test shall verify the state transition of the R_OSAL_IoBlockWrite32 function:
 * - Test06_019_008: confirm state transition error with R_OSAL_IoBlockWrite32
 * @subsection IoBlockWrite32_TC_06_019_008_tpts Test Procedure/Test Steps
 * -# Confirm the state transition of R_OSAL_IoBlockWrite32
 * @subsection IoBlockWrite32_TC_06_019_008_ereb Expected Result/Behavior
 * -# R_OSAL_IoBlockWrite32return OSAL_RETURN_STATE
 * @subsection IoBlockWrite32_TC_06_019_008_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite32_TC_06_019_008
 */
// [IoBlockWrite32_TC_06_019_008]
TEST_F(OsalApiStateTest, IoBlockWrite32_TC_06_019_008)
{
    uint32_t b_register_data32[4] = {TEST_WRITEBLOCK32_VAL_AC[0], TEST_WRITEBLOCK32_VAL_AC[1], TEST_WRITEBLOCK32_VAL_AC[2], TEST_WRITEBLOCK32_VAL_AC[3]};
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;
    uintptr_t offset = TEST_WRITEBLOCK32_REG_OFFSET_AC;
    size_t size = 16;

    // Test06_019_008
    // @brief: abnormal case: state transition error with IO BlockWrite
    EXPECT_EQ(R_OSAL_IoBlockWrite32(device_handle, offset, b_register_data32, size), OSAL_RETURN_STATE) << "Test06_019_008 failed";
}
// [IoBlockWrite32_TC_06_019_008]

/**
 * @page IO_TEST IO API Test
 * @section OSALINTTESTS_IoBlockWrite32_TC_06_019_009 OSALINTTESTS.IoBlockWrite32_TC_06_019_009
 * @subsection IoBlockWrite32_TC_06_019_009_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection IoBlockWrite32_TC_06_019_009_tci Test Case ID
 * IoBlockWrite_S1_N_TC_06_019_009
 * @subsection IoBlockWrite32_TC_06_019_009_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207, \#199211
 * @subsection IoBlockWrite32_TC_06_019_009_pp Prerequisite/Precondition
 * -# OSAL is initialized.<br>
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection IoBlockWrite32_TC_06_019_009_tcs Test Case Summary
 * Run this test when 32bit Bus is supported.<br>
 * @subsubsection IoBlockWrite32_TC_06_019_009_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Setup required memory for interrupt generation.
 * - Register callack function and enable interrupt.
 * - Wait for the interrupt callback function finish.
 * @subsubsection IoBlockWrite32_TC_06_019_009_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * @subsubsection IoBlockWrite32_TC_06_019_009_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality of the IO APIs which are called in the ISR
 * - Disable and clear interrupt status
 * - Test06_019_009: use R_OSAL_IoBlockWrite with 32 bit
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection IoBlockWrite32_TC_06_019_009_tpts Test Procedure/Test Steps
 * @subsubsection IoBlockWrite32_TC_06_019_009_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Allocate required memory for generate interrupt from test target device and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function by using R_OSAL_InterruptRegisterIsr.<br>
 * -# Enable interrupt by using R_OSAL_InterruptEnableIsr
 * -# Setup test target device specific register to generate the interrupt by calling to setup_hw_register
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection IoBlockWrite32_TC_06_019_009_post_tpts Test Procedure/Test Steps of Post-process
 * -# Unregister interrupt by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * @subsubsection IoBlockWrite32_TC_06_019_009_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Read initialization data from test target device with offset "TEST_WRITEBLOCK32_REG_OFFSET_INT" by using R_OSAL_IoBlockRead32.
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Write "registerblock_data32" to test target device with offset "TEST_WRITEBLOCK32_REG_OFFSET_INT" and size is 16 by using R_OSAL_IoBlockWrite32
 * -# Read-back data from test target device with offset "TEST_WRITEBLOCK32_REG_OFFSET_INT" and size is 16 by using R_OSAL_IoBlockRead32.
 * -# Write back initialization data to test target device with offset "TEST_WRITEBLOCK32_REG_OFFSET_INT" by using R_OSAL_IoBlockWrite32.
 * -# Send Message interrupt of context finished by using R_OSAL_MqSendForTimePeriod
 * @subsection IoBlockWrite32_TC_06_019_009_ereb Expected Result/Behavior
 * @subsubsection IoBlockWrite32_TC_06_019_009_pre_ereb Expected Result/Behavior of Pre-process
 * -# allocate_setup_data_memory run successfully
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * The value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK
 * -# setup_hw_register run successfully
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection IoBlockWrite32_TC_06_019_009_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * @subsubsection IoBlockWrite32_TC_06_019_009_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK
 * -# R_OSAL_IoBlockRead32 return OSAL_RETURN_OK.
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_IoBlockWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_IoBlockRead32 return OSAL_RETURN_OK.<br>
 * The read-back data is the same as write data.
 * -# R_OSAL_IoBlockWrite32 return OSAL_RETURN_OK.
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * @subsection IoBlockWrite32_TC_06_019_009_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite32_TC_06_019_009
 * @snippet test_io.cpp osal_io_isr_test_TC_06_019_009
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [IoBlockWrite32_TC_06_019_009]
TEST_F(OSALINTTESTS, IoBlockWrite32_TC_06_019_009)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    st_io_arg_t io_arg;
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
 * Wait for the interrupt callback function finish. */
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_GT(numOfChannel, 0) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_io_isr_test_TC_06_019_009, (void *)&arg), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK) << "pre-process failed";
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory*/
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_io_isr_test_TC_06_019_009), OSAL_RETURN_OK) << "post-process failed";
    deallocate_data_memory(hndl_mmngr, buffer_obj);
}
// [IoBlockWrite32_TC_06_019_009]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite32_TC_06_019_010 IOApiTest.IoBlockWrite32_TC_06_019_010
 * @subsection IoBlockWrite32_TC_06_019_010_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite32_TC_06_019_010_tci Test Case ID
 * IoBlockWrite_S1_N_TC_06_019_010
 * @subsection IoBlockWrite32_TC_06_019_010_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207, \#199211
 * @subsection IoBlockWrite32_TC_06_019_010_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockWrite32_TC_06_019_010_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockWrite APIs of OSAL on normal condition:
 * - pre-process: open test target device, turn ON its power and clock
 * - Test06_019_010: use R_OSAL_IoBlockWrite32 to write data to register of test target device with offset "TEST_WRITEBLOCK32_REG_OFFSET_NC" and size is 0.
 * - post-process: turn OFF clock and close test target device
 * @subsection IoBlockWrite32_TC_06_019_010_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data from test target device with offset "TEST_WRITEBLOCK32_REG_OFFSET_NC" and size is 16 by using R_OSAL_IoBlockRead32.
 * -# Write "register_data32" to test target device with offset "TEST_WRITEBLOCK32_REG_OFFSET_NC" and size is 0 by using R_OSAL_IoBlockWrite32
 * -# Read-back data from test target device with offset "TEST_WRITEBLOCK32_REG_OFFSET_NC" and size is 16 by using R_OSAL_IoBlockRead32.
 * -# Set the clock of test target device status to "OFF":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockWrite32_TC_06_019_010_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockRead32 return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_IoBlockRead32 return OSAL_RETURN_OK.<br>
 * The value of "read_data_before" is equal to "read_data_after".
 * -# Set clock to "OFF" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoBlockWrite32_TC_06_019_010_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite32_TC_06_019_010
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockWrite32_TC_06_019_010]
TEST_F(IOApiTest, IoBlockWrite32_TC_06_019_010)
{
    uint32_t read_data_before[4];
    uint32_t read_data_after[4];
    uint32_t register_data32[4] = {TEST_WRITEBLOCK32_VAL_NC[0], TEST_WRITEBLOCK32_VAL_NC[1], TEST_WRITEBLOCK32_VAL_NC[2], TEST_WRITEBLOCK32_VAL_NC[3]};

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: normal case: write register value of test target device with size is 0.
    // Test06_019_010
    EXPECT_EQ(R_OSAL_IoBlockRead32(device_handle, TEST_WRITEBLOCK32_REG_OFFSET_NC, read_data_before, TEST_IO_ACCESS_SIZE), OSAL_RETURN_OK) << "Test06_019_010 failed";
    EXPECT_EQ(R_OSAL_IoBlockWrite32(device_handle, TEST_WRITEBLOCK32_REG_OFFSET_NC, register_data32, TEST_IO_SIZE_ZERO), OSAL_RETURN_OK) << "Test06_019_010 failed";
    EXPECT_EQ(R_OSAL_IoBlockRead32(device_handle, TEST_WRITEBLOCK32_REG_OFFSET_NC, read_data_after, TEST_IO_ACCESS_SIZE), OSAL_RETURN_OK) << "Test06_019_010 failed";
    EXPECT_TRUE(equal(begin(read_data_before), end(read_data_before), begin(read_data_after))) << "Test06_019_010 failed";

    // post-process: turn OFF clock and close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockWrite32_TC_06_019_010]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite32_TC_06_019_011 IOApiTest.IoBlockWrite32_TC_06_019_011
 * @subsection IoBlockWrite32_TC_06_019_011_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite32_TC_06_019_011_tci Test Case ID
 * IoBlockWrite32_S1_N_TC_06_019_011
 * @subsection IoBlockWrite32_TC_06_019_011_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207, \#199211
 * @subsection IoBlockWrite32_TC_06_019_011_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - The synchronization queue was created
 * @subsection IoBlockWrite32_TC_06_019_011_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoBlockWrite32/R_OSAL_IoBlockRead32 function with the same device handle in multi-threads:
 * - Test06_019_011: Create 2 threads to write and read to register of the same test target device.
 * @subsection IoBlockWrite32_TC_06_019_011_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Turn ON power and clock of the target device by calling to set_clock_on
 * -# Create first thread using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE3
 *          -# task_name is "Block Read/write 32 bit value from/to device register in multi-threads"
 *          -# func is "io_block_read_write_32_device_child_thread" function with functionality:
 *              - Get config of synchronization queue by using R_OSAL_MqGetConfig
 *              - Read initialization data from test target device with offset "TEST_WRITEBLOCK32_REG_OFFSET_NC" by using R_OSAL_IoBlockRead32.
 *              - Write data is "register_data32" to test target device with offset "TEST_WRITEBLOCK32_REG_OFFSET_NC" by using R_OSAL_IoBlockWrite32.
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *          -# userarg includes device handle and thread id
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2
 *      - Address of first thread handle.
 * -# Receive message from synchronization queue by using R_OSAL_MqReceiveForTimePeriod
 * -# Create second thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE3
 *          -# task_name is "Block Read/write 32 bit value from/to device register in multi-threads"
 *          -# func is "io_block_read_write_32_device_child_thread" function with functionality:
 *              - Read data from test target device with offset "TEST_WRITEBLOCK32_REG_OFFSET_NC" by using R_OSAL_IoBlockRead32.
 *              - Write back initialization data to test target device with offset "TEST_WRITEBLOCK32_REG_OFFSET_NC" by using R_OSAL_IoBlockWrite32.
 *          -# userarg includes device handle and thread id
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID3
 *      - Address of second thread handle.
 * -# Turn OFF power and clock domain of target device by calling to set_clock_off
 * -# Join first thread by using R_OSAL_ThreadJoin.
 * -# Join second thread by using R_OSAL_ThreadJoin.
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockWrite32_TC_06_019_011_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with first thread.<br>
 * "thread_handle1" is not NULL_PTR.<br>
 * In io_block_read_write_32_device_child_thread function:
 *  - R_OSAL_MqGetConfig return OSAL_RETURN_OK.
 *  - R_OSAL_IoBlockRead32 return OSAL_RETURN_OK.
 *  - R_OSAL_IoBlockWrite32 return OSAL_RETURN_OK.
 *  - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK.
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK.<br>
 * The value of "receive_msg" need to be equal to "send_msg"
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with second thread.<br>
 * "thread_handle2" is not NULL_PTR.<br>
 * In io_block_read_write_32_device_child_thread function:
 *  - R_OSAL_IoBlockRead32 return OSAL_RETURN_OK.<br>
 * "read_data32" is equal to "register_data32".
 *  - R_OSAL_IoBlockWrite32 return OSAL_RETURN_OK.
 * -# Set power and clock to "OFF" successfully
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with first thread.<br>
 * Value of "result" is equal to OSAL_RETURN_OK.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with second thread.<br>
 * Value of "result" is equal to OSAL_RETURN_OK.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoBlockWrite32_TC_06_019_011_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite32_TC_06_019_011
 * @snippet test_io.cpp io_block_read_write_32_device_child_thread
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockWrite32_TC_06_019_011]
TEST_F(IOApiTest, IoBlockWrite32_TC_06_019_011)
{

    st_osal_thread_config_t config_thread;
    int64_t                 result          = 0;
    osal_thread_handle_t    thread_handle1  = OSAL_THREAD_HANDLE_INVALID;
    osal_thread_handle_t    thread_handle2  = OSAL_THREAD_HANDLE_INVALID;

    // Test06_019_011
    // @brief: normal case: Block read write register 32-bit in multi-threads.
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";

    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "Test06_019_011 failed";

    io_test_multiple_thread_worker_arg user_arg1(&device_handle, TEST_THREAD_ID2);
    user_arg1.send_msg = send_msg;
    user_arg1.mq = sync_mq;
    config_thread.userarg       = &user_arg1;
    config_thread.priority      = OSAL_THREAD_PRIORITY_TYPE3;
    config_thread.task_name     = (char *)"Block Read/write 32 bit value from/to device register in multi-threads";
    config_thread.func          = io_block_read_write_32_device_child_thread;
    config_thread.stack_size    = THREAD_STACK_SIZE;
    EXPECT_EQ(R_OSAL_ThreadCreate(&config_thread, TEST_THREAD_ID2, &thread_handle1), OSAL_RETURN_OK) << "Test06_019_011 failed";
    EXPECT_NE(thread_handle1, NULL_PTR) << "Test06_019_011 failed";
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &receive_msg, mq_config.msg_size), OSAL_RETURN_OK) << "Test06_019_011 failed";
    EXPECT_EQ(receive_msg, send_msg) << "Test06_019_011 failed";

    user_arg1.uid              = TEST_THREAD_ID3;
    EXPECT_EQ(R_OSAL_ThreadCreate(&config_thread, TEST_THREAD_ID3, &thread_handle2), OSAL_RETURN_OK) << "Test06_019_011 failed";
    EXPECT_NE(thread_handle2, NULL_PTR) << "Test06_019_011 failed";

    EXPECT_EQ(R_OSAL_ThreadSleepForTimePeriod(100), OSAL_RETURN_OK);
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "Test06_019_011 failed";

    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &result),OSAL_RETURN_OK) << "Test06_019_011 failed";
    EXPECT_EQ(result, (int64_t)OSAL_RETURN_OK) << "Test06_019_011 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle2, &result),OSAL_RETURN_OK) << "Test06_019_011 failed";
    EXPECT_EQ(result, (int64_t)OSAL_RETURN_OK) << "Test06_019_011 failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockWrite32_TC_06_019_011]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite64_TC_06_020_001 IOApiTest.IoBlockWrite64_TC_06_020_001
 * @subsection IoBlockWrite64_TC_06_020_001_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite64_TC_06_020_001_tci Test Case ID
 * IoBlockWrite_S1_N_TC_06_020_001
 * @subsection IoBlockWrite64_TC_06_020_001_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207, \#199211
 * @subsection IoBlockWrite64_TC_06_020_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - "device_list" contain the device list of each "device_type" in "devices".
 * @subsection IoBlockWrite64_TC_06_020_001_tcs Test Case Summary
 * Run this test when 64bit Bus is supported.<br>
 * This test shall verify the functionality of the IoBlockWrite APIs of OSAL on normal condition:
 * - pre-process: open test target device, turn ON its power and clock
 * - Test06_020_001: use R_OSAL_IoBlockWrite64 to write data to register of test target device with offset "TEST_WRITEBLOCK64_REG_OFFSET_NC".
 * - post-process: write back initialization value, turn OFF clock and close test target device
 * @subsection IoBlockWrite64_TC_06_020_001_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read initialization data from test target device with offset "TEST_WRITEBLOCK64_REG_OFFSET_NC" by using R_OSAL_IoBlockRead64.
 * -# Write "register_data64" to test target device with offset "TEST_WRITEBLOCK64_REG_OFFSET_NC" and size is 16 by using R_OSAL_IoBlockWrite64
 * -# Read-back data from test target device with offset "TEST_WRITEBLOCK64_REG_OFFSET_NC" and size is 16 by using R_OSAL_IoBlockRead64.
 * -# Write back initialization data to test target device with offset "TEST_WRITEBLOCK64_REG_OFFSET_NC" by using R_OSAL_IoBlockWrite64.
 * -# Set the clock of test target device status to "OFF":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockWrite64_TC_06_020_001_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockRead64 return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockWrite64 return OSAL_RETURN_OK
 * -# R_OSAL_IoBlockRead64 return OSAL_RETURN_OK.<br>
 * The read-back data is the same as write data.
 * -# R_OSAL_IoBlockWrite64 return OSAL_RETURN_OK.
 * -# Set clock to "OFF" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoBlockWrite64_TC_06_020_001_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite64_TC_06_020_001
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockWrite64_TC_06_020_001]
TEST_F(IOApiTest, IoBlockWrite64_TC_06_020_001)
{
    uint64_t init_data64[2];
    uintptr_t offset = TEST_WRITEBLOCK64_REG_OFFSET_NC;
    size_t size = 16;
    uint64_t read_data64[2];
    uint64_t register_data64[2] = {TEST_WRITEBLOCK64_VAL_NC[0], TEST_WRITEBLOCK64_VAL_NC[1]};

    for (auto it = device_list.begin(); it != device_list.end(); it++)
    {
        if (!strcmp((*it).c_str(), TEST_IO_DEVICE_1))
        {
            // pre-process: open test target device, turn ON its power and clock
            EXPECT_EQ(R_OSAL_IoDeviceOpen((*it).c_str(), &device_handle), OSAL_RETURN_OK) << "pre-process failed";
            EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
            EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";
            EXPECT_EQ(R_OSAL_IoBlockRead64(device_handle, offset, init_data64, size), OSAL_RETURN_OK) << "pre-process failed";

            // @brief: normal case: write register value of test target device with aligned offset.
            // Test06_020_001
            EXPECT_EQ(R_OSAL_IoBlockWrite64(device_handle, offset, register_data64, size), OSAL_RETURN_OK) << "Test06_020_001 failed";
            EXPECT_EQ(R_OSAL_IoBlockRead64(device_handle, offset, read_data64, size), OSAL_RETURN_OK) << "Test06_020_001 failed";
            EXPECT_TRUE(equal(begin(register_data64), end(register_data64), begin(read_data64))) << "Test06_020_001 failed";

            // post-process: write back initialization value, turn OFF clock and close test target device
            EXPECT_EQ(R_OSAL_IoBlockWrite64(device_handle, offset, init_data64, size), OSAL_RETURN_OK) << "post-process failed";
            EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
            EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
            device_handle = OSAL_DEVICE_HANDLE_INVALID;
        }
    }
}
// [IoBlockWrite64_TC_06_020_001]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite64_TC_06_020_002 IOApiTest.IoBlockWrite64_TC_06_020_002
 * @subsection IoBlockWrite64_TC_06_020_002_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite64_TC_06_020_002_tci Test Case ID
 * IoBlockWrite_S0_A_TC_06_020_002
 * @subsection IoBlockWrite64_TC_06_020_002_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockWrite64_TC_06_020_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockWrite64_TC_06_020_002_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockRead APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_020_002: use R_OSAL_IoBlockWrite64 to write data to register of test target device with un-aligned offset.
 * - post-process: close test target device
 * @subsection IoBlockWrite64_TC_06_020_002_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Write data with offset "TEST_WRITEBLOCK64_OFFSET_UNALIGNED" by using R_OSAL_IoBlockWrite64
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockWrite64_TC_06_020_002_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockWrite64 return OSAL_RETURN_PAR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoBlockWrite64_TC_06_020_002_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite64_TC_06_020_002
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockWrite64_TC_06_020_002]
TEST_F(IOApiTest, IoBlockWrite64_TC_06_020_002)
{
    uintptr_t offset;
    size_t size = 16;
    uint64_t register_data64[2] = {TEST_WRITEBLOCK64_VAL_AC[0], TEST_WRITEBLOCK64_VAL_AC[1]};

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: write register value of test target device with un-aligned offset.
    // Test06_020_002
    offset = TEST_WRITEBLOCK64_OFFSET_UNALIGNED;
    EXPECT_EQ(R_OSAL_IoBlockWrite64(device_handle, offset, register_data64, size), OSAL_RETURN_PAR) << "Test06_020_002 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockWrite64_TC_06_020_002]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite64_TC_06_020_003 IOApiTest.IoBlockWrite64_TC_06_020_003
 * @subsection IoBlockWrite64_TC_06_020_003_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite64_TC_06_020_003_tci Test Case ID
 * IoBlockWrite_S0_A_TC_06_020_003
 * @subsection IoBlockWrite64_TC_06_020_003_rr Related Requirement(s)
 * \#199151, \#199199, \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockWrite64_TC_06_020_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockWrite64_TC_06_020_003_tcs Test Case Summary
 * This test shall verify the functionality of the IoWrite APIs of OSAL on abnormal condition:
 * - Test06_020_003: use R_OSAL_IoBlockWrite64 to write register value with device handle is NULL
 * @subsection IoBlockWrite64_TC_06_020_003_tpts Test Procedure/Test Steps
 * -# Write data with offset "TEST_WRITEBLOCK64_REG_OFFSET_AC" by using R_OSAL_IoBlockWrite64.
 * @subsection IoBlockWrite64_TC_06_020_003_ereb Expected Result/Behavior
 * -# R_OSAL_IoBlockWrite64 return OSAL_RETURN_HANDLE
 * @subsection IoBlockWrite64_TC_06_020_003_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite64_TC_06_020_003
 */
// [IoBlockWrite64_TC_06_020_003]
TEST_F(IOApiTest, IoBlockWrite64_TC_06_020_003)
{
    uintptr_t offset;
    size_t size = 16;
    uint64_t register_data64[2] = {TEST_WRITEBLOCK64_VAL_AC[0], TEST_WRITEBLOCK64_VAL_AC[1]};

    // @brief: abnormal case: write register value with device handle is NULL
    // Test06_020_003
    offset = TEST_WRITEBLOCK64_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoBlockWrite64(OSAL_DEVICE_HANDLE_INVALID, offset, register_data64, size), OSAL_RETURN_HANDLE) << "Test06_020_003 failed";
}
// [IoBlockWrite64_TC_06_020_003]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite64_TC_06_020_004 IOApiTest.IoBlockWrite64_TC_06_020_004
 * @subsection IoBlockWrite64_TC_06_020_004_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite64_TC_06_020_004_tci Test Case ID
 * IoBlockWrite_S0_BA_TC_06_020_004
 * @subsection IoBlockWrite64_TC_06_020_004_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockWrite64_TC_06_020_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockWrite64_TC_06_020_004_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockWrite APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_020_004: use R_OSAL_IoBlockWrite64 to write data to register of test target device with offset which is out-of-register-range.
 * - post-process: close test target device
 * @subsection IoBlockWrite64_TC_06_020_004_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Write data with offset "TEST_OFFSET_OUT_RANGE" by using R_OSAL_IoBlockWrite64
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockWrite64_TC_06_020_004_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockWrite64 return OSAL_RETURN_PAR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoBlockWrite64_TC_06_020_004_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite64_TC_06_020_004
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockWrite64_TC_06_020_004]
TEST_F(IOApiTest, IoBlockWrite64_TC_06_020_004)
{
    uintptr_t offset;
    size_t size = 16;
    uint64_t register_data64[2] = {TEST_WRITEBLOCK64_VAL_AC[0], TEST_WRITEBLOCK64_VAL_AC[1]};

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: write register value of test target device with offset which is out-of-register-range.
    // Test06_020_004
    offset = TEST_OFFSET_OUT_RANGE;
    EXPECT_EQ(R_OSAL_IoBlockWrite64(device_handle, offset, register_data64, size), OSAL_RETURN_PAR) << "Test06_020_004 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockWrite64_TC_06_020_004]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite64_TC_06_020_005 IOApiTest.IoBlockWrite64_TC_06_020_005
 * @subsection IoBlockWrite64_TC_06_020_005_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite64_TC_06_020_005_tci Test Case ID
 * IoBlockWrite_S0_A_TC_06_020_005
 * @subsection IoBlockWrite64_TC_06_020_005_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockWrite64_TC_06_020_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockWrite64_TC_06_020_005_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockWrite APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_020_005: use R_OSAL_IoBlockWrite64 to write register value of test target device with p_data is NULL
 * - post-process: close test target device
 * @subsection IoBlockWrite64_TC_06_020_005_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Write data with offset "TEST_WRITEBLOCK64_REG_OFFSET_AC" by using R_OSAL_IoBlockWrite64
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockWrite64_TC_06_020_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockWrite64 return OSAL_RETURN_PAR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoBlockWrite64_TC_06_020_005_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite64_TC_06_020_005
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockWrite64_TC_06_020_005]
TEST_F(IOApiTest, IoBlockWrite64_TC_06_020_005)
{
    uintptr_t offset;
    size_t size = 16;
    void* p_data = NULL;

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: write register value of test target device with p_data is NULL
    // Test06_020_005
    offset = TEST_WRITEBLOCK64_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoBlockWrite64(device_handle, offset, (uint64_t*)p_data, size), OSAL_RETURN_PAR) << "Test06_020_005 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockWrite64_TC_06_020_005]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite64_TC_06_020_006 IOApiTest.IoBlockWrite64_TC_06_020_006
 * @subsection IoBlockWrite64_TC_06_020_006_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite64_TC_06_020_006_tci Test Case ID
 * IoBlockWrite_S0_BA_TC_06_020_006
 * @subsection IoBlockWrite64_TC_06_020_006_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockWrite64_TC_06_020_006_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockWrite64_TC_06_020_006_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockWrite APIs of OSAL on abnormal condition:
 * - pre-process: open test target device, turn ON power and clock of test target device
 * - Test06_020_006: use R_OSAL_IoBlockWrite64 to write data to register of test target device with size is out-of-range of register
 * - post-process: close test target device
 * @subsection IoBlockWrite64_TC_06_020_006_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Write data with offset "TEST_WRITEBLOCK64_REG_OFFSET_AC" by using R_OSAL_IoBlockWrite64
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockWrite64_TC_06_020_006_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockWrite64 return OSAL_RETURN_PAR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK.
 * @subsection IoBlockWrite64_TC_06_020_006_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite64_TC_06_020_006
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockWrite64_TC_06_020_006]
TEST_F(IOApiTest, IoBlockWrite64_TC_06_020_006)
{
    uintptr_t offset;
    size_t size = TEST_IO_SIZE_OUT_RANGE;
    uint64_t register_data64[2] = {TEST_WRITEBLOCK64_VAL_AC[0], TEST_WRITEBLOCK64_VAL_AC[1]};

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: write register value of test target device with size is out-of-range of register
    // Test06_020_006
    offset = TEST_WRITEBLOCK64_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoBlockWrite64(device_handle, offset, register_data64, size), OSAL_RETURN_PAR) << "Test06_020_006 failed";

    // post-process: close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockWrite64_TC_06_020_006]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite64_TC_06_020_007 IOApiTest.IoBlockWrite64_TC_06_020_007
 * @subsection IoBlockWrite64_TC_06_020_007_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite64_TC_06_020_007_tci Test Case ID
 * IoBlockWrite_S2_A_TC_06_020_007
 * @subsection IoBlockWrite64_TC_06_020_007_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockWrite64_TC_06_020_007_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockWrite64_TC_06_020_007_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockWrite APIs of OSAL on abnormal condition:
 * - Test06_020_007: use R_OSAL_IoBlockWrite64 write register value with closed device handle
 * @subsection IoBlockWrite64_TC_06_020_007_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * -# Write data with offset "TEST_WRITEBLOCK64_REG_OFFSET_AC" by using R_OSAL_IoBlockWrite64.
 * @subsection IoBlockWrite64_TC_06_020_007_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_IoBlockWrite64 return OSAL_RETURN_HANDLE
 * @subsection IoBlockWrite64_TC_06_020_007_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite64_TC_06_020_007
 */
// [IoBlockWrite64_TC_06_020_007]
TEST_F(IOApiTest, IoBlockWrite64_TC_06_020_007)
{
    uintptr_t offset;
    size_t size = 16;
    uint64_t register_data64[2] = {TEST_WRITEBLOCK64_VAL_AC[0], TEST_WRITEBLOCK64_VAL_AC[1]};

    // @brief: abnormal case: Write register value with closed device handle
    // Test06_020_007
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "Test06_020_007 failed";
    EXPECT_NE(device_handle, NULL_PTR) << "Test06_020_007 failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "Test06_020_007 failed";

    offset = TEST_WRITEBLOCK64_REG_OFFSET_AC;
    EXPECT_EQ(R_OSAL_IoBlockWrite64(device_handle, offset, register_data64, size), OSAL_RETURN_HANDLE) << "Test06_020_007 failed";
}
// [IoBlockWrite64_TC_06_020_007]

/**
 * @page IO_TEST IO API Test
 * @section OsalApiStateTest_IoBlockWrite64_TC_06_020_008 OsalApiStateTest.IoBlockWrite64_TC_06_020_008
 * @subsection IoBlockWrite64_TC_06_020_008_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection IoBlockWrite64_TC_06_020_008_tci Test Case ID
 * IoBlockWrite_S2_A_TC_06_020_008
 * @subsection IoBlockWrite64_TC_06_020_008_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207
 * @subsection IoBlockWrite64_TC_06_020_008_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize has already been called
 * @subsection IoBlockWrite64_TC_06_020_008_tcs Test Case Summary
 * This test shall verify the state transition of the R_OSAL_IoBlockWrite64 function:
 * - Test06_020_008: confirm state transition error with R_OSAL_IoBlockWrite64
 * @subsection IoBlockWrite64_TC_06_020_008_tpts Test Procedure/Test Steps
 * -# Confirm the state transition of R_OSAL_IoBlockWrite64
 * @subsection IoBlockWrite64_TC_06_020_008_ereb Expected Result/Behavior
 * -# R_OSAL_IoBlockWrite64return OSAL_RETURN_STATE
 * @subsection IoBlockWrite64_TC_06_020_008_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite64_TC_06_020_008
 */
// [IoBlockWrite64_TC_06_020_008]
TEST_F(OsalApiStateTest, IoBlockWrite64_TC_06_020_008)
{
    uint64_t b_register_data64[2] = {TEST_WRITEBLOCK64_VAL_AC[0], TEST_WRITEBLOCK64_VAL_AC[1]};
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;
    uintptr_t offset = TEST_WRITEBLOCK64_REG_OFFSET_AC;
    size_t size = 16;

    // Test06_020_008
    // @brief: abnormal case: state transition error with IO BlockWrite
    EXPECT_EQ(R_OSAL_IoBlockWrite64(device_handle, offset, b_register_data64, size), OSAL_RETURN_STATE) << "Test06_020_008 failed";
}
// [IoBlockWrite64_TC_06_020_008]

/**
 * @page IO_TEST IO API Test
 * @section OSALINTTESTS_IoBlockWrite64_TC_06_020_009 OSALINTTESTS.IoBlockWrite64_TC_06_020_009
 * @subsection IoBlockWrite64_TC_06_020_009_tsi Test Suite ID
 * OSALINTTESTS
 * @subsection IoBlockWrite64_TC_06_020_009_tci Test Case ID
 * IoBlockWrite_S1_N_TC_06_020_009
 * @subsection IoBlockWrite64_TC_06_020_009_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207, \#199211
 * @subsection IoBlockWrite64_TC_06_020_009_pp Prerequisite/Precondition
 * -# OSAL is initialized.<br>
 * -# The handle of device used to generate HW interrupt is opened
 * -# Changed required power state to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# Created an OSAL MessageQueue handle which ID is INT_SYNC_MQ_ID
 * @subsection IoBlockWrite64_TC_06_020_009_tcs Test Case Summary
 * Run this test when 64bit Bus is supported.
 * @subsubsection IoBlockWrite64_TC_06_020_009_pre_tcs Test Pre-process Summary
 * This implementation shall setup the required configuration to generate an interrupt from hardware.
 * - Setup required memory for interrupt generation.
 * - Register callack function and enable interrupt.
 * - Wait for the interrupt callback function finish.
 * @subsubsection IoBlockWrite64_TC_06_020_009_post_tcs Test Post-process Summary
 * This implementation shall clean up the required configuration of "Test pre-process"
 * - Unregister interrupt callback function
 * - Deallocate allocated memory
 * @subsubsection IoBlockWrite64_TC_06_020_009_cbf_tcs Test Case Summary in Interrupt Callback Function
 * This test shall verify the functionality of the IO APIs which are called in the ISR
 * - Disable and clear interrupt status
 * - Test06_020_009: use R_OSAL_IoBlockWrite with 64 bit
 * - Send a message of the end of the interrupt thread to the message queue
 * @subsection IoBlockWrite64_TC_06_020_009_tpts Test Procedure/Test Steps
 * @subsubsection IoBlockWrite64_TC_06_020_009_pre_tpts Test Procedure/Test Steps of Pre-process
 * -# Allocate required memory for generate interrupt from test target device and get hw address of allocated memory by calling to allocate_setup_data_memory
 * -# Get the number of interrupt channel by using R_OSAL_InterruptGetNumOfIrqChannels
 * -# Register interrupt callback function by using R_OSAL_InterruptRegisterIsr.<br>
 * -# Enable interrupt by using R_OSAL_InterruptEnableIsr
 * -# Setup test target device specific register to generate the interrupt by calling to setup_hw_register
 * -# Receive message by using R_OSAL_MqReceiveForTimePeriod
 * @subsubsection IoBlockWrite64_TC_06_020_009_post_tpts Test Procedure/Test Steps of Post-process
 * -# Unregister interrupt by using R_OSAL_InterruptUnregisterIsr
 * -# Deallocate required memory by calling to deallocate_data_memory
 * @subsubsection IoBlockWrite64_TC_06_020_009_cbf_tpts Test Procedure/Test Steps in Interrupt Callback Function
 * -# Disable interrupt immediately to prevent the unexpected interrupt generation by using R_OSAL_InterruptDisableIsr
 * -# Read initialization data from test target device with offset "TEST_WRITEBLOCK64_REG_OFFSET_INT" by using R_OSAL_IoBlockRead64.
 * -# Clear the status register by using R_OSAL_IoWrite32 with offset is TEST_REG_CLEAR_REGISTER and written data is TEST_CLEAR_REGISTER_VALUE
 * -# Write "registerblock_data64" to test target device with offset "TEST_WRITEBLOCK64_REG_OFFSET_INT" and size is 16 by using R_OSAL_IoBlockWrite64
 * -# Read-back data from test target device with offset "TEST_WRITEBLOCK64_REG_OFFSET_INT" and size is 16 by using R_OSAL_IoBlockRead64.
 * -# Write back initialization data to test target device with offset "TEST_WRITEBLOCK64_REG_OFFSET_INT" by using R_OSAL_IoBlockWrite64.
 * -# Send Message interrupt of context finished by using R_OSAL_MqSendForTimePeriod
 * @subsection IoBlockWrite64_TC_06_020_009_ereb Expected Result/Behavior
 * @subsubsection IoBlockWrite64_TC_06_020_009_pre_ereb Expected Result/Behavior of Pre-process
 * -# allocate_setup_data_memory run successfully
 * -# R_OSAL_InterruptGetNumOfIrqChannels return OSAL_RETURN_OK.<br>
 * The value of "numOfChannel" is greater than 0.
 * -# R_OSAL_InterruptRegisterIsr return OSAL_RETURN_OK
 * -# R_OSAL_InterruptEnableIsr return OSAL_RETURN_OK
 * -# setup_hw_register run successfully
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK and receive send_message by interrupt context
 * @subsubsection IoBlockWrite64_TC_06_020_009_post_ereb Expected Result/Behavior of Post-process
 * -# R_OSAL_InterruptUnregisterIsr return OSAL_RETURN_OK
 * -# deallocate_data_memory run successfully.
 * @subsubsection IoBlockWrite64_TC_06_020_009_cbf_ereb Expected Result/Behavior in Interrupt Callback Function
 * -# R_OSAL_InterruptDisableIsr return OSAL_RETURN_OK
 * -# R_OSAL_IoBlockRead64 return OSAL_RETURN_OK.
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_IoBlockWrite64 return OSAL_RETURN_OK
 * -# R_OSAL_IoBlockRead64 return OSAL_RETURN_OK.<br>
 * The read-back data is the same as write data.
 * -# R_OSAL_IoBlockWrite64 return OSAL_RETURN_OK.
 * -# R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK
 * @subsection IoBlockWrite64_TC_06_020_009_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite64_TC_06_020_009
 * @snippet test_io.cpp osal_io_isr_test_TC_06_020_009
 * @snippet test_common.hpp allocate_setup_data_memory
 * @snippet test_common.hpp setup_hw_register
 * @snippet test_common.hpp deallocate_data_memory
 */
// [IoBlockWrite64_TC_06_020_009]
TEST_F(OSALINTTESTS, IoBlockWrite64_TC_06_020_009)
{
    size_t numOfChannel = 0;
    e_osal_interrupt_priority_t priority = OSAL_INTERRUPT_PRIORITY_TYPE0;
    st_callback_arg_t arg;
    st_io_arg_t io_arg;
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
 * Wait for the interrupt callback function finish. */
    arg.mq_handle = isr_mq;
    arg.send_msg = send_msg;
    allocate_setup_data_memory(&hndl_mmngr, hw_addr, buffer_obj);
    EXPECT_EQ(R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannel), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_GT(numOfChannel, 0) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptRegisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, priority, osal_io_isr_test_TC_06_020_009, (void *)&arg), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_EQ(R_OSAL_InterruptEnableIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT), OSAL_RETURN_OK) << "pre-process failed";
    setup_hw_register(dev_arr, hw_addr);
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(isr_mq, time_period, &receive_msg, sizeof(send_msg)), OSAL_RETURN_OK);
    EXPECT_EQ(receive_msg, send_msg);

/* post-process:
 * Unregister interrupt
 * Deallocate allocated memory*/
    EXPECT_EQ(R_OSAL_InterruptUnregisterIsr(device_handle, TEST_IRQ_CHANNEL_DEVICE_GENE_INT, osal_io_isr_test_TC_06_020_009), OSAL_RETURN_OK) << "post-process failed";
    deallocate_data_memory(hndl_mmngr, buffer_obj);
}
// [IoBlockWrite64_TC_06_020_009]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite64_TC_06_020_010 IOApiTest.IoBlockWrite64_TC_06_020_010
 * @subsection IoBlockWrite64_TC_06_020_010_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite64_TC_06_020_010_tci Test Case ID
 * IoBlockWrite_S1_N_TC_06_020_010
 * @subsection IoBlockWrite64_TC_06_020_010_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207, \#199211
 * @subsection IoBlockWrite64_TC_06_020_010_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection IoBlockWrite64_TC_06_020_010_tcs Test Case Summary
 * This test shall verify the functionality of the IoBlockWrite APIs of OSAL on normal condition:
 * - pre-process: open test target device, turn ON its power and clock
 * - Test06_020_010: use R_OSAL_IoBlockWrite64 to write data to register of test target device with offset "TEST_WRITEBLOCK64_REG_OFFSET_NC" and size is 0.
 * - post-process: turn OFF clock and close test target device
 * @subsection IoBlockWrite64_TC_06_020_010_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Set power and clock of test target device status to "ON":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Read data from test target device with offset "TEST_WRITEBLOCK64_REG_OFFSET_NC" and size is 16 by using R_OSAL_IoBlockRead64.
 * -# Write "register_data64" to test target device with offset "TEST_WRITEBLOCK64_REG_OFFSET_NC" and size is 0 by using R_OSAL_IoBlockWrite64
 * -# Read-back data from test target device with offset "TEST_WRITEBLOCK64_REG_OFFSET_NC" and size is 16 by using R_OSAL_IoBlockRead64.
 * -# Set the clock of test target device status to "OFF":
 *      - Get the current required state by using R_OSAL_PmGetRequiredState
 *      - Set required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockWrite64_TC_06_020_010_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockRead64 return OSAL_RETURN_OK.
 * -# R_OSAL_IoBlockWrite64 return OSAL_RETURN_OK
 * -# R_OSAL_IoBlockRead64 return OSAL_RETURN_OK.<br>
 * The value of "read_data_before" is equal to "read_data_after".
 * -# Set clock to "OFF" successfully
 *      - R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 *      - R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoBlockWrite64_TC_06_020_010_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite64_TC_06_020_010
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockWrite64_TC_06_020_010]
TEST_F(IOApiTest, IoBlockWrite64_TC_06_020_010)
{
    uint64_t read_data_before[2];
    uint64_t read_data_after[2];
    uint64_t register_data64[2] = {TEST_WRITEBLOCK64_VAL_NC[0], TEST_WRITEBLOCK64_VAL_NC[1]};

    // pre-process: open test target device, turn ON its power and clock
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";
    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "pre-process failed";

    // @brief: abnormal case: write register value of test target device with size is 0.
    // Test06_020_010
    EXPECT_EQ(R_OSAL_IoBlockRead64(device_handle, TEST_WRITEBLOCK64_REG_OFFSET_NC, read_data_before, TEST_IO_ACCESS_SIZE), OSAL_RETURN_OK) << "Test06_020_010 failed";
    EXPECT_EQ(R_OSAL_IoBlockWrite64(device_handle, TEST_WRITEBLOCK64_REG_OFFSET_NC, register_data64, TEST_IO_SIZE_ZERO), OSAL_RETURN_OK) << "Test06_020_010 failed";
    EXPECT_EQ(R_OSAL_IoBlockRead64(device_handle, TEST_WRITEBLOCK64_REG_OFFSET_NC, read_data_after, TEST_IO_ACCESS_SIZE), OSAL_RETURN_OK) << "Test06_020_010 failed";
    EXPECT_TRUE(equal(begin(read_data_before), end(read_data_before), begin(read_data_after))) << "Test06_020_010 failed";

    // post-process: turn OFF clock and close test target device
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "post-process failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockWrite64_TC_06_020_010]

/**
 * @page IO_TEST IO API Test
 * @section IOApiTest_IoBlockWrite64_TC_06_020_011 IOApiTest.IoBlockWrite64_TC_06_020_011
 * @subsection IoBlockWrite64_TC_06_020_011_tsi Test Suite ID
 * IOApiTest
 * @subsection IoBlockWrite64_TC_06_020_011_tci Test Case ID
 * IoBlockWrite64_S1_N_TC_06_020_011
 * @subsection IoBlockWrite64_TC_06_020_011_rr Related Requirement(s)
 * \#199204, \#199205, \#199206, \#199207, \#199211
 * @subsection IoBlockWrite64_TC_06_020_011_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - The synchronization queue was created
 * @subsection IoBlockWrite64_TC_06_020_011_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_IoBlockWrite64/R_OSAL_IoBlockRead64 function with the same device handle in multi-threads:
 * - Test06_020_011: Create 2 threads to write and read to register of the same test target device.
 * @subsection IoBlockWrite64_TC_06_020_011_tpts Test Procedure/Test Steps
 * -# Open test target device by using R_OSAL_IoDeviceOpen
 * -# Turn ON power and clock of the target device by calling to set_clock_on
 * -# Create first thread using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE3
 *          -# task_name is "Block Read/write 64 bit value from/to device register in multi-threads"
 *          -# func is "io_block_read_write_64_device_child_thread" function with functionality:
 *              - Get config of synchronization queue by using R_OSAL_MqGetConfig
 *              - Read initialization data from test target device with offset "TEST_WRITEBLOCK64_REG_OFFSET_NC" by using R_OSAL_IoBlockRead64.
 *              - Write data is "register_data64" to test target device with offset "TEST_WRITEBLOCK64_REG_OFFSET_NC" by using R_OSAL_IoBlockWrite64.
 *              - Send a message to synchronization queue by using R_OSAL_MqSendForTimePeriod
 *          -# userarg includes device handle and thread id
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID2
 *      - Address of first thread handle.
 * -# Receive message from synchronization queue by using R_OSAL_MqReceiveForTimePeriod
 * -# Create second thread by using R_OSAL_ThreadCreate with the argument as below:
 *      - p_config: must be configured with following specified values
 *          -# priority is OSAL_THREAD_PRIORITY_TYPE3
 *          -# task_name is "Block Read/write 64 bit value from/to device register in multi-threads"
 *          -# func is "io_block_read_write_64_device_child_thread" function with functionality:
 *              - Read data from test target device with offset "TEST_WRITEBLOCK64_REG_OFFSET_NC" by using R_OSAL_IoBlockRead64.
 *              - Write back initialization data to test target device with offset "TEST_WRITEBLOCK64_REG_OFFSET_NC" by using R_OSAL_IoBlockWrite64.
 *          -# userarg includes device handle and thread id
 *          -# stack_size is THREAD_STACK_SIZE
 *      - thread_id is TEST_THREAD_ID3
 *      - Address of second thread handle.
 * -# Turn OFF power and clock domain of target device by calling to set_clock_off
 * -# Join first thread by using R_OSAL_ThreadJoin.
 * -# Join second thread by using R_OSAL_ThreadJoin.
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * @subsection IoBlockWrite64_TC_06_020_011_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not NULL_PTR.
 * -# Set power and clock to "ON" successfully
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with first thread.<br>
 * "thread_handle1" is not NULL_PTR.<br>
 * In io_block_read_write_64_device_child_thread function:
 *  - R_OSAL_MqGetConfig return OSAL_RETURN_OK.
 *  - R_OSAL_IoBlockRead64 return OSAL_RETURN_OK.
 *  - R_OSAL_IoBlockWrite64 return OSAL_RETURN_OK.
 *  - R_OSAL_MqSendForTimePeriod return OSAL_RETURN_OK.
 * -# R_OSAL_MqReceiveForTimePeriod return OSAL_RETURN_OK.<br>
 * The value of "receive_msg" need to be equal to "send_msg"
 * -# R_OSAL_ThreadCreate return OSAL_RETURN_OK with second thread.<br>
 * "thread_handle2" is not NULL_PTR.<br>
 * In io_block_read_write_64_device_child_thread function:
 *  - R_OSAL_IoBlockRead64 return OSAL_RETURN_OK.<br>
 * "read_data64" is equal to "register_data64".
 *  - R_OSAL_IoBlockWrite64 return OSAL_RETURN_OK.
 * -# Set power and clock to "OFF" successfully
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with first thread.<br>
 * Value of "result" is equal to OSAL_RETURN_OK.
 * -# R_OSAL_ThreadJoin return OSAL_RETURN_OK with second thread.<br>
 * Value of "result" is equal to OSAL_RETURN_OK.
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * @subsection IoBlockWrite64_TC_06_020_011_testcase Test Case Example
 * @snippet test_io.cpp IoBlockWrite64_TC_06_020_011
 * @snippet test_io.cpp io_block_read_write_64_device_child_thread
 * @snippet test_io.cpp set_clock_on
 * @snippet test_io.cpp set_clock_off
 */
// [IoBlockWrite64_TC_06_020_011]
TEST_F(IOApiTest, IoBlockWrite64_TC_06_020_011)
{

    st_osal_thread_config_t config_thread;
    int64_t                 result          = 0;
    osal_thread_handle_t    thread_handle1  = OSAL_THREAD_HANDLE_INVALID;
    osal_thread_handle_t    thread_handle2  = OSAL_THREAD_HANDLE_INVALID;

    // Test06_020_011
    // @brief: normal case: Block read write register 64-bit in multi-threads.
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_IO_DEVICE_1, &device_handle), OSAL_RETURN_OK) << "pre-process failed";
    EXPECT_NE(device_handle, NULL_PTR) << "pre-process failed";

    EXPECT_EQ(set_clock_on(device_handle), OSAL_RETURN_OK) << "Test06_020_011 failed";

    io_test_multiple_thread_worker_arg user_arg1(&device_handle, TEST_THREAD_ID2);
    user_arg1.send_msg = send_msg;
    user_arg1.mq = sync_mq;
    config_thread.userarg       = &user_arg1;
    config_thread.priority      = OSAL_THREAD_PRIORITY_TYPE3;
    config_thread.task_name     = (char *)"Block Read/write 64 bit value from/to device register in multi-threads";
    config_thread.func          = io_block_read_write_64_device_child_thread;
    config_thread.stack_size    = THREAD_STACK_SIZE;
    EXPECT_EQ(R_OSAL_ThreadCreate(&config_thread, TEST_THREAD_ID2, &thread_handle1), OSAL_RETURN_OK) << "Test06_020_011 failed";
    EXPECT_NE(thread_handle1, NULL_PTR) << "Test06_020_011 failed";
    EXPECT_EQ(R_OSAL_MqReceiveForTimePeriod(sync_mq, SYNC_MQ_TIME, &receive_msg, mq_config.msg_size), OSAL_RETURN_OK) << "Test06_020_011 failed";
    EXPECT_EQ(receive_msg, send_msg) << "Test06_020_011 failed";

    user_arg1.uid              = TEST_THREAD_ID3;
    EXPECT_EQ(R_OSAL_ThreadCreate(&config_thread, TEST_THREAD_ID3, &thread_handle2), OSAL_RETURN_OK) << "Test06_020_011 failed";
    EXPECT_NE(thread_handle2, NULL_PTR) << "Test06_020_011 failed";

    EXPECT_EQ(R_OSAL_ThreadSleepForTimePeriod(100), OSAL_RETURN_OK);
    EXPECT_EQ(set_clock_off(device_handle), OSAL_RETURN_OK) << "Test06_020_011 failed";

    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle1, &result),OSAL_RETURN_OK) << "Test06_020_011 failed";
    EXPECT_EQ(result, (int64_t)OSAL_RETURN_OK) << "Test06_020_011 failed";
    EXPECT_EQ(R_OSAL_ThreadJoin(thread_handle2, &result),OSAL_RETURN_OK) << "Test06_020_011 failed";
    EXPECT_EQ(result, (int64_t)OSAL_RETURN_OK) << "Test06_020_011 failed";
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK) << "post-process failed";
    device_handle = OSAL_DEVICE_HANDLE_INVALID;
}
// [IoBlockWrite64_TC_06_020_011]
#endif
