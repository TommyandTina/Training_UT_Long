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
* File Name :    test_pm.cpp
* Version :      3.4.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Power Manager API Functional Test (From the perspective of OSAL API Specification)
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           2.0.0     29.10.2021    Create New
 *           2.0.1     11.11.2021    Add R_OSAL_PmSetRequiredState with OSAL_PM_RESET_STATE_RELEASED for each test case
 *                                   change test target device to "imp_00" from "imp_top_00"
 *           2.0.2     18.11.2021    Fix Typo
 *           2.0.3     06.12.2021    Add Sleep Sequence for PmGetResetState Test
 *                                   Update delta_time of PmWaitForState Test
 *                                       Because these test are dependent on BSP of OS
 *           3.0.0     15.02.2022    Apply new device and its setting definition
 *                                   Temporarily disable PMApiTest and PMApiSequenceTest for S4
 *                     25.02.2022    Enable PMApiTest and PMApiSequenceTest for S4
 *                                   Update to use "rtdmac_control_00" and "rtdmac_control_01" device for PM testing on S4.
 *           3.1.0     24.05.2022    Add new test PmSoftwareReset_TC_07_012_001
 *                                   Add new test PmSoftwareReset_TC_07_012_002
 *                                   Add new test PmSoftwareReset_TC_07_012_003
 *                                   Add new test PmSoftwareReset_TC_07_012_004
 *           3.1.1     26.05.2022    Remove software reset applying using OSAL APIs
 *                                   Add read and write back the initialize register to PMApiSequenceTest SetUp/TearDown
 *           3.1.2     30.05.2022    Enable disabled test cases related to shared power domain device
 *           3.2.0     11.07.2022    Add new test cases using assigned device handle that isn't supported by PM
 *           3.3.0     21.11.2022    Add reinitialize power policy of devices in the SetUp and TearDown of PMApiTest
 *                                   Add new test for initialize leak check of power lowest state 07_010_012 to 07_010_014
 *                                   Add new test for checking set policy of device into PG 07_011_022 to 07_011_027
 *                                   Add powering down operation of "imp_01" device for test cases that set "imp_00" to PG
 *                     25.11.2022    Add new test cases to confirm the PM required state transition:
 *                                      - PmSetRequiredState_TC_07_001_012
 *                                      - PmSetRequiredState_TC_07_001_013
 *                                      - PmSetRequiredState_TC_07_001_014
 *                                      - PmSetRequiredState_TC_07_001_015
 *                                      - PmSetRequiredState_TC_07_001_016
 *                                      - PmSetRequiredState_TC_07_001_017
 *                                      - PmSetRequiredState_TC_07_001_018
 *                                      - PmSetRequiredState_TC_07_001_019
 *                                      - PmSetRequiredState_TC_07_001_020
 *           3.4.0     01.02.2023    Add new boundary test cases as below:
 *                                      - PmSetRequiredState_TC_07_001_021
 *                                      - PmWaitForState_TC_07_003_022
 *                                      - PmSetPolicy_TC_07_005_020
 *                                      - PmSetResetState_TC_07_007_009
 *                                      - PmSetPostClock_TC_07_009_016
 *                                      - PmSetPostClock_TC_07_009_017
 *                                      - PmSetPostClock_TC_07_009_018
 *                                      - PmSetPostClock_TC_07_009_019
 *                     10.02.2023       - Disabled PmSetPostClock_TC_07_009_018. Because, clock_divider value depend on OS.
 *           3.5.0     23.02.2023       - Re-Enable PmSetPostClock_TC_07_009_018.
 *           3.6.0     23.03.2023    Update expect value of PmSetPostClock_TC_07_009_018
 *           3.7.0     11.09.2023    Add Device definition of V4M
***********************************************************************************************************************/
#include "test_common.hpp"
#if !defined(OSAL_TEST_FRAMEWORK)
using namespace std;
#else
using namespace OsalFWHelper;
#endif

#if defined (TEST_SIL)

/* Simulator does not support fba_* or fbc_* devices */
#define PmSetPostClock_TC_07_009_001 DISABLED_PmSetPostClock_TC_07_009_001
#define PmSetPostClock_TC_07_009_002 DISABLED_PmSetPostClock_TC_07_009_002
#define PmSetPostClock_TC_07_009_003 DISABLED_PmSetPostClock_TC_07_009_003
#define PmSetPostClock_TC_07_009_004 DISABLED_PmSetPostClock_TC_07_009_004
#define PmSetPostClock_TC_07_009_007 DISABLED_PmSetPostClock_TC_07_009_007
#define PmSetPostClock_TC_07_009_010 DISABLED_PmSetPostClock_TC_07_009_010
#define PmSetPostClock_TC_07_009_011 DISABLED_PmSetPostClock_TC_07_009_011
#define PmSetPostClock_TC_07_009_012 DISABLED_PmSetPostClock_TC_07_009_012
#define PmSetPostClock_TC_07_009_013 DISABLED_PmSetPostClock_TC_07_009_013
#define PmSetPostClock_TC_07_009_014 DISABLED_PmSetPostClock_TC_07_009_014
#define PmSetPostClock_TC_07_009_016 DISABLED_PmSetPostClock_TC_07_009_016
#define PmSetPostClock_TC_07_009_017 DISABLED_PmSetPostClock_TC_07_009_017
#define PmSetPostClock_TC_07_009_018 DISABLED_PmSetPostClock_TC_07_009_018
#define PmSetPostClock_TC_07_009_019 DISABLED_PmSetPostClock_TC_07_009_019

/* Simulator does not support cpg to perform sw reset */
#define PmSoftwareReset_TC_07_012_001 DISABLED_PmSoftwareReset_TC_07_012_001
#define PmSoftwareReset_TC_07_012_002 DISABLED_PmSoftwareReset_TC_07_012_002
#define PmSoftwareReset_TC_07_012_003 DISABLED_PmSoftwareReset_TC_07_012_003
#define PmSoftwareReset_TC_07_012_004 DISABLED_PmSoftwareReset_TC_07_012_004

/* Simulator does not have unsupport reset/clock device on V3U/V4H/V4M */
#if defined(OSAL_SOC_V4H) || defined(OSAL_SOC_V4H2) || defined(OSAL_SOC_V3U) || defined(OSAL_SOC_V4M)
#define PmSetRequiredState_TC_07_001_011 DISABLED_PmSetRequiredState_TC_07_001_011
#define PmGetRequiredState_TC_07_002_008 DISABLED_PmGetRequiredState_TC_07_002_008
#define PmWaitForState_TC_07_003_021 DISABLED_PmWaitForState_TC_07_003_021
#define PmGetState_TC_07_004_011 DISABLED_PmGetState_TC_07_004_011
#define PmSetPolicy_TC_07_005_019 DISABLED_PmSetPolicy_TC_07_005_019
#define PmGetPolicy_TC_07_006_009 DISABLED_PmGetPolicy_TC_07_006_009
#define PmSetResetState_TC_07_007_008 DISABLED_PmSetResetState_TC_07_007_008
#define PmGetResetState_TC_07_008_007 DISABLED_PmGetResetState_TC_07_008_007
#define PmGetLowestIntermediatePowerState_TC_07_010_011 DISABLED_PmGetLowestIntermediatePowerState_TC_07_010_011
#endif
#endif

/* These tests has been disabled as a workaround. */
/* The types of timers used in PMA and Zephyr OSAL wrapper are different, */
/* and the processing time is not correctly calculated.*/
#if defined(TEST_ZEPHYR) 
#define PmWaitForState_TC_07_003_018 DISABLED_PmWaitForState_TC_07_003_018
#define PmWaitForState_TC_07_003_019 DISABLED_PmWaitForState_TC_07_003_019
#endif
/**
 * @page PM_TEST Power API Test
 * @test
 * @test @link PMApiTest_PmSetRequiredState_TC_07_001_001 @endlink
 * @test @link PMApiTest_PmSetRequiredState_TC_07_001_002 @endlink
 * @test @link PMApiTest_PmSetRequiredState_TC_07_001_003 @endlink
 * @test @link PMApiTest_PmSetRequiredState_TC_07_001_004 @endlink
 * @test @link PMApiTest_PmSetRequiredState_TC_07_001_005 @endlink
 * @test @link PMApiTest_PmSetRequiredState_TC_07_001_006 @endlink
 * @test @link PMApiTest_PmSetRequiredState_TC_07_001_007 @endlink
 * @test @link PMApiTest_PmSetRequiredState_TC_07_001_008 @endlink
 * @test @link OsalApiStateTest_PmSetRequiredState_TC_07_001_009 @endlink
 * @test @link PMApiTest_PmSetRequiredState_TC_07_001_010 @endlink
 * @test @link PMApiTest_PmSetRequiredState_TC_07_001_011 @endlink
 * @test @link PMApiTest_PmSetRequiredState_TC_07_001_012 @endlink
 * @test @link PMApiTest_PmSetRequiredState_TC_07_001_013 @endlink
 * @test @link PMApiTest_PmSetRequiredState_TC_07_001_014 @endlink
 * @test @link PMApiTest_PmSetRequiredState_TC_07_001_015 @endlink
 * @test @link PMApiTest_PmSetRequiredState_TC_07_001_016 @endlink
 * @test @link PMApiTest_PmSetRequiredState_TC_07_001_017 @endlink
 * @test @link PMApiTest_PmSetRequiredState_TC_07_001_018 @endlink
 * @test @link PMApiTest_PmSetRequiredState_TC_07_001_019 @endlink
 * @test @link PMApiTest_PmSetRequiredState_TC_07_001_020 @endlink
 * @test @link PMApiTest_PmSetRequiredState_TC_07_001_021 @endlink
 * @test @link PMApiTest_PmGetRequiredState_TC_07_002_001 @endlink
 * @test @link PMApiTest_PmGetRequiredState_TC_07_002_002 @endlink
 * @test @link PMApiTest_PmGetRequiredState_TC_07_002_003 @endlink
 * @test @link PMApiTest_PmGetRequiredState_TC_07_002_004 @endlink
 * @test @link PMApiTest_PmGetRequiredState_TC_07_002_005 @endlink
 * @test @link OsalApiStateTest_PmGetRequiredState_TC_07_002_006 @endlink
 * @test @link PMApiTest_PmGetRequiredState_TC_07_002_007 @endlink
 * @test @link PMApiTest_PmGetRequiredState_TC_07_002_008 @endlink
 * @test @link PMApiTest_PmWaitForState_TC_07_003_001 @endlink
 * @test @link PMApiTest_PmWaitForState_TC_07_003_002 @endlink
 * @test @link PMApiTest_PmWaitForState_TC_07_003_003 @endlink
 * @test @link PMApiTest_PmWaitForState_TC_07_003_004 @endlink
 * @test @link PMApiTest_PmWaitForState_TC_07_003_005 @endlink
 * @test @link PMApiTest_PmWaitForState_TC_07_003_006 @endlink
 * @test @link PMApiTest_PmWaitForState_TC_07_003_007 @endlink
 * @test @link PMApiTest_PmWaitForState_TC_07_003_008 @endlink
 * @test @link PMApiTest_PmWaitForState_TC_07_003_009 @endlink
 * @test @link PMApiTest_PmWaitForState_TC_07_003_010 @endlink
 * @test @link PMApiTest_PmWaitForState_TC_07_003_011 @endlink
 * @test @link PMApiTest_PmWaitForState_TC_07_003_012 @endlink
 * @test @link PMApiTest_PmWaitForState_TC_07_003_013 @endlink
 * @test @link PMApiTest_PmWaitForState_TC_07_003_014 @endlink
 * @test @link PMApiTest_PmWaitForState_TC_07_003_015 @endlink
 * @test @link PMApiTest_PmWaitForState_TC_07_003_016 @endlink
 * @test @link OsalApiStateTest_PmWaitForState_TC_07_003_017 @endlink
 * @test @link PMApiTest_PmWaitForState_TC_07_003_018 @endlink
 * @test @link PMApiTest_PmWaitForState_TC_07_003_019 @endlink
 * @test @link PMApiTest_PmWaitForState_TC_07_003_020 @endlink
 * @test @link PMApiTest_PmWaitForState_TC_07_003_021 @endlink
 * @test @link PMApiTest_PmWaitForState_TC_07_003_022 @endlink
 * @test @link PMApiTest_PmGetState_TC_07_004_001 @endlink
 * @test @link PMApiTest_PmGetState_TC_07_004_002 @endlink
 * @test @link PMApiTest_PmGetState_TC_07_004_003 @endlink
 * @test @link PMApiTest_PmGetState_TC_07_004_004 @endlink
 * @test @link PMApiTest_PmGetState_TC_07_004_005 @endlink
 * @test @link PMApiTest_PmGetState_TC_07_004_006 @endlink
 * @test @link PMApiTest_PmGetState_TC_07_004_007 @endlink
 * @test @link PMApiTest_PmGetState_TC_07_004_008 @endlink
 * @test @link OsalApiStateTest_PmGetState_TC_07_004_009 @endlink
 * @test @link PMApiTest_PmGetState_TC_07_004_010 @endlink
 * @test @link PMApiTest_PmGetState_TC_07_004_011 @endlink
 * @test @link PMApiTest_PmSetPolicy_TC_07_005_001 @endlink
 * @test @link PMApiTest_PmSetPolicy_TC_07_005_002 @endlink
 * @test @link PMApiTest_PmSetPolicy_TC_07_005_003 @endlink
 * @test @link PMApiTest_PmSetPolicy_TC_07_005_004 @endlink
 * @test @link PMApiTest_PmSetPolicy_TC_07_005_005 @endlink
 * @test @link PMApiTest_PmSetPolicy_TC_07_005_006 @endlink
 * @test @link PMApiTest_PmSetPolicy_TC_07_005_007 @endlink
 * @test @link PMApiTest_PmSetPolicy_TC_07_005_008 @endlink
 * @test @link PMApiTest_PmSetPolicy_TC_07_005_009 @endlink
 * @test @link PMApiTest_PmSetPolicy_TC_07_005_010 @endlink
 * @test @link PMApiTest_PmSetPolicy_TC_07_005_011 @endlink
 * @test @link PMApiTest_PmSetPolicy_TC_07_005_012 @endlink
 * @test @link PMApiTest_PmSetPolicy_TC_07_005_013 @endlink
 * @test @link PMApiTest_PmSetPolicy_TC_07_005_014 @endlink
 * @test @link PMApiTest_PmSetPolicy_TC_07_005_015 @endlink
 * @test @link PMApiTest_PmSetPolicy_TC_07_005_016 @endlink
 * @test @link OsalApiStateTest_PmSetPolicy_TC_07_005_017 @endlink
 * @test @link PMApiTest_PmSetPolicy_TC_07_005_018 @endlink
 * @test @link PMApiTest_PmSetPolicy_TC_07_005_019 @endlink
 * @test @link PMApiTest_PmSetPolicy_TC_07_005_020 @endlink
 * @test @link PMApiTest_PmGetPolicy_TC_07_006_001 @endlink
 * @test @link PMApiTest_PmGetPolicy_TC_07_006_002 @endlink
 * @test @link PMApiTest_PmGetPolicy_TC_07_006_003 @endlink
 * @test @link PMApiTest_PmGetPolicy_TC_07_006_004 @endlink
 * @test @link PMApiTest_PmGetPolicy_TC_07_006_005 @endlink
 * @test @link PMApiTest_PmGetPolicy_TC_07_006_006 @endlink
 * @test @link OsalApiStateTest_PmGetPolicy_TC_07_006_007 @endlink
 * @test @link PMApiTest_PmGetPolicy_TC_07_006_008 @endlink
 * @test @link PMApiTest_PmGetPolicy_TC_07_006_009 @endlink
 * @test @link PMApiTest_PmSetResetState_TC_07_007_001 @endlink
 * @test @link PMApiTest_PmSetResetState_TC_07_007_002 @endlink
 * @test @link PMApiTest_PmSetResetState_TC_07_007_003 @endlink
 * @test @link PMApiTest_PmSetResetState_TC_07_007_004 @endlink
 * @test @link PMApiTest_PmSetResetState_TC_07_007_005 @endlink
 * @test @link PMApiTest_PmSetResetState_TC_07_007_006 @endlink
 * @test @link OsalApiStateTest_PmSetResetState_TC_07_007_007 @endlink
 * @test @link PMApiTest_PmSetResetState_TC_07_007_008 @endlink
 * @test @link PMApiTest_PmSetResetState_TC_07_007_009 @endlink
 * @test @link PMApiTest_PmGetResetState_TC_07_008_001 @endlink
 * @test @link PMApiTest_PmGetResetState_TC_07_008_002 @endlink
 * @test @link PMApiTest_PmGetResetState_TC_07_008_003 @endlink
 * @test @link PMApiTest_PmGetResetState_TC_07_008_004 @endlink
 * @test @link PMApiTest_PmGetResetState_TC_07_008_005 @endlink
 * @test @link OsalApiStateTest_PmGetResetState_TC_07_008_006 @endlink
 * @test @link PMApiTest_PmGetResetState_TC_07_008_007 @endlink
 * @test @link PMApiTest_PmSetPostClock_TC_07_009_001 @endlink
 * @test @link PMApiTest_PmSetPostClock_TC_07_009_002 @endlink
 * @test @link PMApiTest_PmSetPostClock_TC_07_009_003 @endlink
 * @test @link PMApiTest_PmSetPostClock_TC_07_009_004 @endlink
 * @test @link PMApiTest_PmSetPostClock_TC_07_009_005 @endlink
 * @test @link PMApiTest_PmSetPostClock_TC_07_009_006 @endlink
 * @test @link PMApiTest_PmSetPostClock_TC_07_009_007 @endlink
 * @test @link PMApiTest_PmSetPostClock_TC_07_009_008 @endlink
 * @test @link PMApiTest_PmSetPostClock_TC_07_009_009 @endlink
 * @test @link PMApiTest_PmSetPostClock_TC_07_009_010 @endlink
 * @test @link PMApiTest_PmSetPostClock_TC_07_009_011 @endlink
 * @test @link PMApiTest_PmSetPostClock_TC_07_009_012 @endlink
 * @test @link PMApiTest_PmSetPostClock_TC_07_009_013 @endlink
 * @test @link PMApiTest_PmSetPostClock_TC_07_009_014 @endlink
 * @test @link OsalApiStateTest_PmSetPostClock_TC_07_009_015 @endlink
 * @test @link PMApiTest_PmSetPostClock_TC_07_009_016 @endlink
 * @test @link PMApiTest_PmSetPostClock_TC_07_009_017 @endlink
 * @test @link PMApiTest_PmSetPostClock_TC_07_009_019 @endlink
 * @test @link PMApiTest_PmGetLowestIntermediatePowerState_TC_07_010_001 @endlink
 * @test @link PMApiTest_PmGetLowestIntermediatePowerState_TC_07_010_002 @endlink
 * @test @link PMApiTest_PmGetLowestIntermediatePowerState_TC_07_010_003 @endlink
 * @test @link PMApiTest_PmGetLowestIntermediatePowerState_TC_07_010_004 @endlink
 * @test @link PMApiTest_PmGetLowestIntermediatePowerState_TC_07_010_005 @endlink
 * @test @link PMApiTest_PmGetLowestIntermediatePowerState_TC_07_010_006 @endlink
 * @test @link PMApiTest_PmGetLowestIntermediatePowerState_TC_07_010_007 @endlink
 * @test @link PMApiTest_PmGetLowestIntermediatePowerState_TC_07_010_008 @endlink
 * @test @link OsalApiStateTest_PmGetLowestIntermediatePowerState_TC_07_010_009 @endlink
 * @test @link PMApiTest_PmGetLowestIntermediatePowerState_TC_07_010_010 @endlink
 * @test @link PMApiTest_PmGetLowestIntermediatePowerState_TC_07_010_011 @endlink
 * @test @link PMApiTest_PmGetLowestIntermediatePowerState_TC_07_010_012 @endlink
 * @test @link PMApiTest_PmGetLowestIntermediatePowerState_TC_07_010_013 @endlink
 * @test @link PMApiTest_PmGetLowestIntermediatePowerState_TC_07_010_014 @endlink
 * @test @link PMApiSequenceTest_PmSequence_TC_07_011_001 @endlink
 * @test @link PMApiSequenceTest_PmSequence_TC_07_011_002 @endlink
 * @test @link PMApiSequenceTest_PmSequence_TC_07_011_003 @endlink
 * @test @link PMApiSequenceTest_PmSequence_TC_07_011_004 @endlink
 * @test @link PMApiSequenceTest_PmSequence_TC_07_011_005 @endlink
 * @test @link PMApiSequenceTest_PmSequence_TC_07_011_006 @endlink
 * @test @link PMApiSequenceTest_PmSequence_TC_07_011_007 @endlink
 * @test @link PMApiSequenceTest_PmSequence_TC_07_011_008 @endlink
 * @test @link PMApiSequenceTest_PmSequence_TC_07_011_009 @endlink
 * @test @link PMApiSequenceTest_PmSequence_TC_07_011_010 @endlink
 * @test @link PMApiSequenceTest_PmSequence_TC_07_011_011 @endlink
 * @test @link PMApiSequenceTest_PmSequence_TC_07_011_012 @endlink
 * @test @link PMApiSequenceTest_PmSequence_TC_07_011_013 @endlink
 * @test @link PMApiSequenceTest_PmSequence_TC_07_011_014 @endlink
 * @test @link PMApiSequenceTest_PmSequence_TC_07_011_015 @endlink
 * @test @link PMApiSequenceTest_PmSequence_TC_07_011_016 @endlink
 * @test @link PMApiSequenceTest_PmSequence_TC_07_011_017 @endlink
 * @test @link PMApiSequenceTest_PmSequence_TC_07_011_018 @endlink
 * @test @link PMApiSequenceTest_PmSequence_TC_07_011_019 @endlink
 * @test @link PMApiSequenceTest_PmSequence_TC_07_011_020 @endlink
 * @test @link PMApiSequenceTest_PmSequence_TC_07_011_021 @endlink
 * @test @link PMApiSequenceTest_PmSequence_TC_07_011_022 @endlink
 * @test @link PMApiSequenceTest_PmSequence_TC_07_011_023 @endlink
 * @test @link PMApiSequenceTest_PmSequence_TC_07_011_024 @endlink
 * @test @link PMApiSequenceTest_PmSequence_TC_07_011_025 @endlink
 * @test @link PMApiSequenceTest_PmSequence_TC_07_011_026 @endlink
 * @test @link PMApiSequenceTest_PmSequence_TC_07_011_027 @endlink
 * @test @link PMApiTest_PmSoftwareReset_TC_07_012_001 @endlink
 * @test @link PMApiTest_PmSoftwareReset_TC_07_012_002 @endlink
 * @test @link PMApiTest_PmSoftwareReset_TC_07_012_003 @endlink
 * @test @link PMApiTest_PmSoftwareReset_TC_07_012_004 @endlink
 * @section PMApiTest PMApiTest function
 * @subsection PMApiTest_SetUp PMApiTest Setup
 * Common to Test Suit of PMApiTest, prepare process to be executed before executing the test. <br>
 * For example:
 * @snippet test_pm.cpp PMApiTest_SetUp
 * @subsection PMApiTest_TearDown PMApiTest TearDown
 * Common to Test Suit of PMApiTest, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_pm.cpp PMApiTest_TearDown
 * @section PMApiSequenceTest PMApiSequenceTest function
 * @subsection PMApiSequenceTest_SetUp PMApiSequenceTest Setup
 * Common to Test Suit of PMApiSequenceTest, prepare process to be executed before executing the test. <br>
 * For example:
 * @snippet test_pm.cpp PMApiSequenceTest_SetUp
 * @subsection PMApiSequenceTest_TearDown PMApiSequenceTest TearDown
 * Common to Test Suit of PMApiSequenceTest, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_pm.cpp PMApiSequenceTest_TearDown
 * @section OsalApiStateTest_PMApiTest OsalApiStateTest of PMApiTest function
 * @subsection OsalApiStateTest_PMApiTest_SetUp OsalApiStateTest of PMApiTest Setup
 * Common to Test Suit of OsalApiStateTest PMApiTest, prepare process to be executed before executing the test. <br>
 * For example:
 * @snippet test_common.hpp OsalApiStateTest_SetUp
 * @subsection OsalApiStateTest_PMApiTest_TearDown OsalApiStateTest of PMApiTest TearDown
 * Common to Test Suit of OsalApiStateTest PMApiTest, prepare process to be executed after executing the test. <br>
 * For example:
 * @snippet test_common.hpp OsalApiStateTest_TearDown
 */
class PMApiTest : public OSALTESTS
{
protected:
    static osal_device_handle_t device_handle;
    static osal_device_handle_t device_handle_dependence;
    static osal_device_handle_t device_handle1;
    static osal_device_handle_t device_handle2;
    static osal_device_handle_t device_handle3;
    static osal_device_handle_t unsupport_power_dev_handle;
    static osal_device_handle_t unsupport_clock_dev_handle;
    static osal_device_handle_t unsupport_reset_dev_handle;
    static e_osal_pm_policy_t initialize_policy_handle;
    static e_osal_pm_policy_t initialize_policy_handle1;
    static e_osal_pm_policy_t initialize_policy_handle2;
    static e_osal_pm_policy_t initialize_policy_handle3;

// [PMApiTest_SetUp]
    static void SetUpTestSuite()
    {
#if defined(OSAL_SOC_V3U) || defined(OSAL_SOC_V3H1) || defined(OSAL_SOC_V3H2) || defined(OSAL_SOC_V3M2) || defined(OSAL_SOC_V4H) || defined(OSAL_SOC_V4H2) || defined(OSAL_SOC_V4M) /* V3x, V4H, V4M */
        EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_PM_DEVICE_2, &device_handle), OSAL_RETURN_OK);
        EXPECT_NE(device_handle, NULL_PTR);
        EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_PM_DEVICE_1, &device_handle1), OSAL_RETURN_OK);
        EXPECT_NE(device_handle1, NULL_PTR);
        EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_PM_DEVICE_2_DEPENDENCE, &device_handle_dependence), OSAL_RETURN_OK);
        EXPECT_NE(device_handle_dependence, NULL_PTR);
#if !defined(TEST_SIL)
        EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_FBA_DEVICE_ID, &device_handle2), OSAL_RETURN_OK);
        EXPECT_NE(device_handle2, NULL_PTR);
#endif
        /* On Linux, imp_00 cannot be power on without imp_top_00 power on. This is Linux Restriction */
        EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_PM_DEVICE_3, &device_handle3), OSAL_RETURN_OK);
        EXPECT_NE(device_handle3, NULL_PTR);
        EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle3, OSAL_PM_REQUIRED_STATE_REQUIRED, false), OSAL_RETURN_OK);
#elif defined(OSAL_SOC_S4) /* S4 */
        EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_PM_DEVICE_2, &device_handle), OSAL_RETURN_OK);
        EXPECT_NE(device_handle, NULL_PTR);
        EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_PM_DEVICE_1, &device_handle1), OSAL_RETURN_OK);
        EXPECT_NE(device_handle1, NULL_PTR);
        EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_FBA_DEVICE_ID, &device_handle2), OSAL_RETURN_OK);
        EXPECT_NE(device_handle2, NULL_PTR);
#endif

        EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_UNSUPPORT_POWER_DEVICE, &unsupport_power_dev_handle), OSAL_RETURN_OK);
        EXPECT_NE(unsupport_power_dev_handle, NULL_PTR);
#if defined(OSAL_SOC_V3H1) || defined(OSAL_SOC_V3H2) || defined(OSAL_SOC_V3M2)
        EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_UNSUPPORT_RESET_DEVICE, &unsupport_reset_dev_handle), OSAL_RETURN_OK);
        EXPECT_NE(unsupport_reset_dev_handle, NULL_PTR);
        unsupport_clock_dev_handle = unsupport_power_dev_handle; // use same device system_ram_00 for unsupport power and clock device handle
#else /* V4H, V3U, S4 */
#if !defined(TEST_SIL)
        EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_UNSUPPORT_CLOCK_DEVICE, &unsupport_clock_dev_handle), OSAL_RETURN_OK);
        EXPECT_NE(unsupport_clock_dev_handle, NULL_PTR);
        EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_UNSUPPORT_RESET_DEVICE, &unsupport_reset_dev_handle), OSAL_RETURN_OK);
        EXPECT_NE(unsupport_reset_dev_handle, NULL_PTR);
#endif
#endif
    }
// [PMApiTest_SetUp]

// [PMApiTest_TearDown]
    static void TearDownTestSuite()
    {
        bool applyImmediate = true;
#if defined(OSAL_SOC_V3U) || defined(OSAL_SOC_V3H1) || defined(OSAL_SOC_V3H2) || defined(OSAL_SOC_V3M2) || defined(OSAL_SOC_V4H) || defined(OSAL_SOC_V4H2) || defined(OSAL_SOC_V4M) /* V3x, V4H, V4M */
        /* On Linux, Linux Restriction cleanup process */
        EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle3, OSAL_PM_REQUIRED_STATE_RELEASED, false), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle1), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle_dependence), OSAL_RETURN_OK);
#if !defined(TEST_SIL)
        EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle2), OSAL_RETURN_OK);
#endif
        EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle3), OSAL_RETURN_OK);
#elif defined(OSAL_SOC_S4) /* S4 */
        EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle1), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle2), OSAL_RETURN_OK);
#endif

        EXPECT_EQ(R_OSAL_IoDeviceClose(unsupport_power_dev_handle), OSAL_RETURN_OK);
#if defined(OSAL_SOC_V3H1) || defined(OSAL_SOC_V3H2) || defined(OSAL_SOC_V3M2)
        EXPECT_EQ(R_OSAL_IoDeviceClose(unsupport_reset_dev_handle), OSAL_RETURN_OK);
#else /* V4H, V3U, S4 */
#if !defined(TEST_SIL)
        EXPECT_EQ(R_OSAL_IoDeviceClose(unsupport_clock_dev_handle), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoDeviceClose(unsupport_reset_dev_handle), OSAL_RETURN_OK);
#endif
#endif
    }
// [PMApiTest_TearDown]


    void SetUp() override{
#if defined(OSAL_SOC_V3U) || defined(OSAL_SOC_V3H1) || defined(OSAL_SOC_V3H2) || defined(OSAL_SOC_V3M2) || defined(OSAL_SOC_V4H) || defined(OSAL_SOC_V4H2) || defined(OSAL_SOC_V4M) /* V3x, V4H, V4M */
        EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle, &initialize_policy_handle), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle1, &initialize_policy_handle1), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle3, &initialize_policy_handle3), OSAL_RETURN_OK);
#elif defined(OSAL_SOC_S4) /* S4 */
        EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle, &initialize_policy_handle), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle1, &initialize_policy_handle1), OSAL_RETURN_OK);
#endif
#if defined(OSAL_SOC_V4M) && !defined(TEST_SIL) /* R-Car V4M */
        {
            osal_device_handle_t device_tmp;
            EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_PM_DEVICE_SEQUENCE_PG_OFF_1, &device_tmp), OSAL_RETURN_OK);
            EXPECT_EQ(R_OSAL_PmSetRequiredState(device_tmp, OSAL_PM_REQUIRED_STATE_RELEASED, true), OSAL_RETURN_OK);
            EXPECT_EQ(R_OSAL_PmSetPolicy(device_tmp, OSAL_PM_POLICY_PG, true), OSAL_RETURN_OK);
            EXPECT_EQ(R_OSAL_IoDeviceClose(device_tmp), OSAL_RETURN_OK);
            EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_PM_DEVICE_SEQUENCE_PG_OFF_2, &device_tmp), OSAL_RETURN_OK);
            EXPECT_EQ(R_OSAL_PmSetRequiredState(device_tmp, OSAL_PM_REQUIRED_STATE_RELEASED, true), OSAL_RETURN_OK);
            EXPECT_EQ(R_OSAL_PmSetPolicy(device_tmp, OSAL_PM_POLICY_PG, true), OSAL_RETURN_OK);
            EXPECT_EQ(R_OSAL_IoDeviceClose(device_tmp), OSAL_RETURN_OK);
        }
#endif
    }
    void TearDown() override{
        bool applyImmediate = true;
        e_osal_pm_required_state_t return_required_state = OSAL_PM_REQUIRED_STATE_INVALID;
#if defined(OSAL_SOC_V3U) || defined(OSAL_SOC_V3H1) || defined(OSAL_SOC_V3H2) || defined(OSAL_SOC_V3M2) || defined(OSAL_SOC_V4H) || defined(OSAL_SOC_V4H2) || defined(OSAL_SOC_V4M) /* V3x, V4H, V4M */
        //Set released for device_handle
        EXPECT_EQ(R_OSAL_PmGetRequiredState(device_handle, &return_required_state), OSAL_RETURN_OK);
        if(return_required_state == OSAL_PM_REQUIRED_STATE_REQUIRED)
        {
            EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED , applyImmediate), OSAL_RETURN_OK);
        }
        //Set released for device_handle_1
        EXPECT_EQ(R_OSAL_PmGetRequiredState(device_handle1, &return_required_state), OSAL_RETURN_OK);
        if(return_required_state == OSAL_PM_REQUIRED_STATE_REQUIRED)
        {
            EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED , applyImmediate), OSAL_RETURN_OK);
        }
        //Set released for device_handle_3
        EXPECT_EQ(R_OSAL_PmGetRequiredState(device_handle3, &return_required_state), OSAL_RETURN_OK);
        if(return_required_state == OSAL_PM_REQUIRED_STATE_REQUIRED)
        {
            EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle3, OSAL_PM_REQUIRED_STATE_RELEASED , applyImmediate), OSAL_RETURN_OK);
        }
        EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, initialize_policy_handle, applyImmediate), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, initialize_policy_handle1, applyImmediate), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle3, initialize_policy_handle3, applyImmediate), OSAL_RETURN_OK);
#elif defined(OSAL_SOC_S4) /* S4 */
        //Set released for device_handle_1
        EXPECT_EQ(R_OSAL_PmGetRequiredState(device_handle, &return_required_state), OSAL_RETURN_OK);
        if(return_required_state == OSAL_PM_REQUIRED_STATE_REQUIRED)
        {
            EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED , applyImmediate), OSAL_RETURN_OK);
        }
        //Set released for device_handle_1
        EXPECT_EQ(R_OSAL_PmGetRequiredState(device_handle1, &return_required_state), OSAL_RETURN_OK);
        if(return_required_state == OSAL_PM_REQUIRED_STATE_REQUIRED)
        {
            EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED , applyImmediate), OSAL_RETURN_OK);
        }
        EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, initialize_policy_handle, applyImmediate), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, initialize_policy_handle1, applyImmediate), OSAL_RETURN_OK);
#endif

    }
};

osal_device_handle_t PMApiTest::device_handle = OSAL_DEVICE_HANDLE_INVALID;
osal_device_handle_t PMApiTest::device_handle_dependence = OSAL_DEVICE_HANDLE_INVALID;
osal_device_handle_t PMApiTest::device_handle1 = OSAL_DEVICE_HANDLE_INVALID;
osal_device_handle_t PMApiTest::device_handle2 = OSAL_DEVICE_HANDLE_INVALID;
osal_device_handle_t PMApiTest::device_handle3 = OSAL_DEVICE_HANDLE_INVALID;
osal_device_handle_t PMApiTest::unsupport_power_dev_handle = OSAL_DEVICE_HANDLE_INVALID;
osal_device_handle_t PMApiTest::unsupport_clock_dev_handle = OSAL_DEVICE_HANDLE_INVALID;
osal_device_handle_t PMApiTest::unsupport_reset_dev_handle = OSAL_DEVICE_HANDLE_INVALID;
e_osal_pm_policy_t PMApiTest::initialize_policy_handle = OSAL_PM_POLICY_INVALID;
e_osal_pm_policy_t PMApiTest::initialize_policy_handle1 = OSAL_PM_POLICY_INVALID;
e_osal_pm_policy_t PMApiTest::initialize_policy_handle2 = OSAL_PM_POLICY_INVALID;
e_osal_pm_policy_t PMApiTest::initialize_policy_handle3 = OSAL_PM_POLICY_INVALID;
class PMApiSequenceTest : public OSALTESTS
{
protected:
    osal_device_handle_t device_handle1;
    osal_device_handle_t device_handle2;
    osal_device_handle_t device_handle2_dependence;
    uint32_t init_data32 = 0;
    uintptr_t offset = PM_WRITE32_REG_OFFSET;

// [PMApiSequenceTest_SetUp]
    void SetUp() override
    {
        EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_PM_DEVICE_SEQUENCE, &device_handle1), OSAL_RETURN_OK);
        EXPECT_NE(device_handle1, NULL_PTR);
        /* Read initialization value of PM_WRITE32_REG_OFFSET*/
        EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_REQUIRED, true), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoRead32(device_handle1, offset, &init_data32), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, true), OSAL_RETURN_OK);
#if !defined(OSAL_SOC_S4) /* R-Car S4 doesn't have PG device */
        EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_PM_DEVICE_SEQUENCE_PG, &device_handle2), OSAL_RETURN_OK);
        EXPECT_NE(device_handle2, nullptr);
        EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_PM_DEVICE_2_DEPENDENCE, &device_handle2_dependence), OSAL_RETURN_OK);
        EXPECT_NE(device_handle2_dependence, nullptr);
#endif
#if defined(OSAL_SOC_V4M) && !defined(TEST_SIL) /* R-Car V4M */
        {
            osal_device_handle_t device_tmp;
            EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_PM_DEVICE_SEQUENCE_PG_OFF_1, &device_tmp), OSAL_RETURN_OK);
            EXPECT_EQ(R_OSAL_PmSetRequiredState(device_tmp, OSAL_PM_REQUIRED_STATE_RELEASED, true), OSAL_RETURN_OK);
            EXPECT_EQ(R_OSAL_PmSetPolicy(device_tmp, OSAL_PM_POLICY_PG, true), OSAL_RETURN_OK);
            EXPECT_EQ(R_OSAL_IoDeviceClose(device_tmp), OSAL_RETURN_OK);
            EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_PM_DEVICE_SEQUENCE_PG_OFF_2, &device_tmp), OSAL_RETURN_OK);
            EXPECT_EQ(R_OSAL_PmSetRequiredState(device_tmp, OSAL_PM_REQUIRED_STATE_RELEASED, true), OSAL_RETURN_OK);
            EXPECT_EQ(R_OSAL_PmSetPolicy(device_tmp, OSAL_PM_POLICY_PG, true), OSAL_RETURN_OK);
            EXPECT_EQ(R_OSAL_IoDeviceClose(device_tmp), OSAL_RETURN_OK);
        }
#endif
    }
// [PMApiSequenceTest_SetUp]

// [PMApiSequenceTest_TearDown]
    void TearDown() override
    {
        /* Write back initialization value of PM_WRITE32_REG_OFFSET */
        EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_REQUIRED, true), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoWrite32(device_handle1, offset, init_data32), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, true), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle1), OSAL_RETURN_OK);
#if !defined(OSAL_SOC_S4) /* R-Car S4 doesn't have PG device */
        EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle2), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle2_dependence), OSAL_RETURN_OK);
#endif
    }
// [PMApiSequenceTest_TearDown]
};

#ifndef SKIP_SECTION_1
/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetRequiredState_TC_07_001_001 PMApiTest.PmSetRequiredState_TC_07_001_001
 * @subsection PmSetRequiredState_TC_07_001_001_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetRequiredState_TC_07_001_001_tci Test Case ID
 * PmSetRequiredState_S1_BN_TC_07_001_001
 * @subsection PmSetRequiredState_TC_07_001_001_rr Related Requirement(s)
 *  \#239132, \#239134
 * @subsection PmSetRequiredState_TC_07_001_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetRequiredState_TC_07_001_001_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetRequiredState function in normal condition:
 * - Test07_001_001: set OSAL_PM_REQUIRED_STATE_REQUIRED state of test target device with applyImmediate is true
 * @subsection PmSetRequiredState_TC_07_001_001_tpts Test Procedure/Test Steps
 * -# Set the required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Get the current required state by using R_OSAL_PmGetRequiredState
 * -# Set the required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * @subsection PmSetRequiredState_TC_07_001_001_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * Value of "req_state" is equal to OSAL_PM_REQUIRED_STATE_REQUIRED
 * @subsection PmSetRequiredState_TC_07_001_001_testcase Test Case Example
 * @snippet test_pm.cpp PmSetRequiredState_TC_07_001_001
 */
// [PmSetRequiredState_TC_07_001_001]
TEST_F(PMApiTest, PmSetRequiredState_TC_07_001_001)
{
    e_osal_pm_required_state_t req_state = OSAL_PM_REQUIRED_STATE_INVALID;
    bool applyImmediate = false;

    // @brief: normal case: set OSAL_PM_REQUIRED_STATE_REQUIRED of device with applyImmediate is true
    // Test07_001_001
    applyImmediate = true;
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK) << "Test07_001_001 failed";
    EXPECT_EQ(R_OSAL_PmGetRequiredState(device_handle, &req_state), OSAL_RETURN_OK) << "Test07_001_001 failed";
    EXPECT_EQ(req_state, OSAL_PM_REQUIRED_STATE_REQUIRED) << "Test07_001_001 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, false), OSAL_RETURN_OK);
}
// [PmSetRequiredState_TC_07_001_001]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetRequiredState_TC_07_001_002 PMApiTest.PmSetRequiredState_TC_07_001_002
 * @subsection PmSetRequiredState_TC_07_001_002_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetRequiredState_TC_07_001_002_tci Test Case ID
 * PmSetRequiredState_S1_BN_TC_07_001_002
 * @subsection PmSetRequiredState_TC_07_001_002_rr Related Requirement(s)
 *  \#239132, \#239134
 * @subsection PmSetRequiredState_TC_07_001_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetRequiredState_TC_07_001_002_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetRequiredState function in normal condition:
 * - Test07_001_002: set OSAL_PM_REQUIRED_STATE_RELEASED state of test target device with applyImmediate is true
 * @subsection PmSetRequiredState_TC_07_001_002_tpts Test Procedure/Test Steps
 * -# Set the required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Get the current required state by using R_OSAL_PmGetRequiredState
 * @subsection PmSetRequiredState_TC_07_001_002_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.<br>
 * Value of "req_state" is equal to OSAL_PM_REQUIRED_STATE_RELEASED
 * @subsection PmSetRequiredState_TC_07_001_002_testcase Test Case Example
 * @snippet test_pm.cpp PmSetRequiredState_TC_07_001_002
 */
// [PmSetRequiredState_TC_07_001_002]
TEST_F(PMApiTest, PmSetRequiredState_TC_07_001_002)
{
    e_osal_pm_required_state_t req_state = OSAL_PM_REQUIRED_STATE_INVALID;
    bool applyImmediate = false;

    // @brief: normal case: set OSAL_PM_REQUIRED_STATE_RELEASED of device with applyImmediate is true
    // Test07_001_002
    applyImmediate = true;
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_001_002 failed";
    EXPECT_EQ(R_OSAL_PmGetRequiredState(device_handle, &req_state), OSAL_RETURN_OK) << "Test07_001_002 failed";
    EXPECT_EQ(req_state, OSAL_PM_REQUIRED_STATE_RELEASED) << "Test07_001_002 failed";
}
// [PmSetRequiredState_TC_07_001_002]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetRequiredState_TC_07_001_003 PMApiTest.PmSetRequiredState_TC_07_001_003
 * @subsection PmSetRequiredState_TC_07_001_003_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetRequiredState_TC_07_001_003_tci Test Case ID
 * PmSetRequiredState_S1_BN_TC_07_001_003
 * @subsection PmSetRequiredState_TC_07_001_003_rr Related Requirement(s)
 *  \#239132, \#239134
 * @subsection PmSetRequiredState_TC_07_001_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetRequiredState_TC_07_001_003_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetRequiredState function in normal condition:
 * - Test07_001_003: set OSAL_PM_REQUIRED_STATE_REQUIRED state of test target device with applyImmediate is false
 * @subsection PmSetRequiredState_TC_07_001_003_tpts Test Procedure/Test Steps
 * -# Set the required state by using R_OSAL_PmSetRequiredState with the arguments as below:
 *      - Device handle of test target device
 *      - OSAL_PM_REQUIRED_STATE_REQUIRED
 *      - false
 * -# Get the current power state by using R_OSAL_PmWaitForState/R_OSAL_PmGetState.<br>
 * The power state need to be OSAL_PM_STATE_RESET or OSAL_PM_STATE_READY
 * -# Set the required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * @subsection PmSetRequiredState_TC_07_001_003_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_PmWaitForState/R_OSAL_PmGetState return OSAL_RETURN_OK.
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * @subsection PmSetRequiredState_TC_07_001_003_testcase Test Case Example
 * @snippet test_pm.cpp PmSetRequiredState_TC_07_001_003
 */
// [PmSetRequiredState_TC_07_001_003]
TEST_F(PMApiTest, PmSetRequiredState_TC_07_001_003)
{
    e_osal_pm_required_state_t req_state = OSAL_PM_REQUIRED_STATE_INVALID;
    bool applyImmediate = false;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_pm_state_t p_state = OSAL_PM_STATE_INVALID;
    osal_milli_sec_t wait_time_1000s = 1000;

    // @brief: normal case: set OSAL_PM_REQUIRED_STATE_REQUIRED of device with applyImmediate is false
    // Test07_001_003
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK) << "Test07_001_003 failed";
    osal_ret = R_OSAL_PmWaitForState(device_handle, OSAL_PM_STATE_RESET, wait_time_1000s);
    if (OSAL_RETURN_OK == osal_ret) /*Current reset state is RESET*/
    {
        SUCCEED();
    }
    else if (OSAL_RETURN_TIME == osal_ret) /* Current reset state is READY */
    {
        EXPECT_EQ(R_OSAL_PmGetState(device_handle, &p_state), OSAL_RETURN_OK) << "Test07_001_003 failed";
        EXPECT_EQ(p_state, OSAL_PM_STATE_READY) << "Test07_001_003 failed";
    }
    else
    {
        ADD_FAILURE();
    }
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, false), OSAL_RETURN_OK);
}
// [PmSetRequiredState_TC_07_001_003]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetRequiredState_TC_07_001_004 PMApiTest.PmSetRequiredState_TC_07_001_004
 * @subsection PmSetRequiredState_TC_07_001_004_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetRequiredState_TC_07_001_004_tci Test Case ID
 * PmSetRequiredState_S1_BN_TC_07_001_004
 * @subsection PmSetRequiredState_TC_07_001_004_rr Related Requirement(s)
 *  \#239132, \#239134
 * @subsection PmSetRequiredState_TC_07_001_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetRequiredState_TC_07_001_004_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetRequiredState function in normal condition:
 * - Test07_001_004: set OSAL_PM_REQUIRED_STATE_RELEASED state of test target device with applyImmediate is false
 * @subsection PmSetRequiredState_TC_07_001_004_tpts Test Procedure/Test Steps
 * -# Set the required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Get the current required state by using R_OSAL_PmGetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_PG  by using R_OSAL_PmSetPolicy
 * -# Set the required state by using R_OSAL_PmSetRequiredState with the arguments as below:
 *      - Device handle of test target device
 *      - OSAL_PM_REQUIRED_STATE_RELEASED
 *      - false
 * -# Wait for OSAL_PM_STATE_RESET in 10ms by using R_OSAL_PmWaitForState
 * @subsection PmSetRequiredState_TC_07_001_004_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.<br>
 * Value of "req_state" is equal to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK.7
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK.
 * @subsection PmSetRequiredState_TC_07_001_004_testcase Test Case Example
 * @snippet test_pm.cpp PmSetRequiredState_TC_07_001_004
 */
// [PmSetRequiredState_TC_07_001_004]
TEST_F(PMApiTest, PmSetRequiredState_TC_07_001_004)
{
    e_osal_pm_required_state_t req_state = OSAL_PM_REQUIRED_STATE_INVALID;
    bool applyImmediate = false;
    osal_milli_sec_t wait_time_10s = 10;

    // @brief: normal case: set OSAL_PM_REQUIRED_STATE_RELEASED of device with applyImmediate is false
    // Test07_001_004
    applyImmediate = true;
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK) << "Test07_001_004 failed";
    EXPECT_EQ(R_OSAL_PmGetRequiredState(device_handle, &req_state), OSAL_RETURN_OK) << "Test07_001_004 failed";
#if defined(OSAL_SOC_V3M2) // Clock of IMP on V3M2 share with other device. (IMP_00, IMP_01)
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle_dependence, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK) << "Test07_001_004 failed";
    EXPECT_EQ(R_OSAL_PmGetRequiredState(device_handle_dependence, &req_state), OSAL_RETURN_OK) << "Test07_001_004 failed";
#endif
    EXPECT_EQ(req_state, OSAL_PM_REQUIRED_STATE_REQUIRED) << "Test07_001_004 failed";
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, OSAL_PM_POLICY_CG, applyImmediate), OSAL_RETURN_OK) << "Test07_001_004 failed";
#if defined(OSAL_SOC_V3M2)
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle_dependence, OSAL_PM_POLICY_CG, applyImmediate), OSAL_RETURN_OK) << "Test07_001_004 failed";
#endif
    applyImmediate = false;
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_001_004 failed";
#if defined(OSAL_SOC_V3M2)
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle_dependence, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_001_004 failed";
#endif
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle, OSAL_PM_STATE_CG, wait_time_10s), OSAL_RETURN_OK) << "Test07_001Test07_001_004_004 failed";
}
// [PmSetRequiredState_TC_07_001_004]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetRequiredState_TC_07_001_005 PMApiTest.PmSetRequiredState_TC_07_001_005
 * @subsection PmSetRequiredState_TC_07_001_005_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetRequiredState_TC_07_001_005_tci Test Case ID
 * PmSetRequiredState_S0_A_TC_07_001_005
 * @subsection PmSetRequiredState_TC_07_001_005_rr Related Requirement(s)
 * \#199151, \#239134
 * @subsection PmSetRequiredState_TC_07_001_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection PmSetRequiredState_TC_07_001_005_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetRequiredState function in abnormal condition:
 * - Test07_001_005: set power required state with device handle is NULL
 * @subsection PmSetRequiredState_TC_07_001_005_tpts Test Procedure/Test Steps
 * -# Set the required state by using R_OSAL_PmSetRequiredState with the arguments as below:
 *      - Device handle is NULL
 *      - OSAL_PM_REQUIRED_STATE_REQUIRED
 *      - true
 * @subsection PmSetRequiredState_TC_07_001_005_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_HANDLE
 * @subsection PmSetRequiredState_TC_07_001_005_testcase Test Case Example
 * @snippet test_pm.cpp PmSetRequiredState_TC_07_001_005
 */
// [PmSetRequiredState_TC_07_001_005]
TEST_F(PMApiTest, PmSetRequiredState_TC_07_001_005)
{
    bool applyImmediate = true;

    // @brief: abnormal case: device handle is NULL
    // Test07_001_005
    EXPECT_EQ(R_OSAL_PmSetRequiredState(OSAL_DEVICE_HANDLE_INVALID, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_HANDLE) << "Test07_001_005 failed";
}
// [PmSetRequiredState_TC_07_001_005]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetRequiredState_TC_07_001_006 PMApiTest.PmSetRequiredState_TC_07_001_006
 * @subsection PmSetRequiredState_TC_07_001_006_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetRequiredState_TC_07_001_006_tci Test Case ID
 * PmSetRequiredState_S2_BA_TC_07_001_006
 * @subsection PmSetRequiredState_TC_07_001_006_rr Related Requirement(s)
 * \#239134
 * @subsection PmSetRequiredState_TC_07_001_006_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetRequiredState_TC_07_001_006_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetRequiredState function in abnormal condition:
 * - Test07_001_006: set power required state with device handle has already been closed
 * @subsection PmSetRequiredState_TC_07_001_006_tpts Test Procedure/Test Steps
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * -# Set the required state by using R_OSAL_PmSetRequiredState with the arguments as below:
 *      - Device handle of test target device which has already been closed at step 12
 *      - OSAL_PM_REQUIRED_STATE_REQUIRED
 *      - true
 * -# Re-open test target device for the next test cases by using R_OSAL_IoDeviceOpen
 * @subsection PmSetRequiredState_TC_07_001_006_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_HANDLE
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not equal to NULL_PTR
 * @subsection PmSetRequiredState_TC_07_001_006_testcase Test Case Example
 * @snippet test_pm.cpp PmSetRequiredState_TC_07_001_006
 */
// [PmSetRequiredState_TC_07_001_006]
TEST_F(PMApiTest, PmSetRequiredState_TC_07_001_006)
{
    bool applyImmediate = true;

    // @brief: abnormal case: device handle is closed
    // Test07_001_006
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_HANDLE) << "Test07_001_006 failed";
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_PM_DEVICE_2, &device_handle), OSAL_RETURN_OK); // re-open for the next test case
    EXPECT_NE(device_handle, NULL_PTR);
}
// [PmSetRequiredState_TC_07_001_006]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetRequiredState_TC_07_001_007 PMApiTest.PmSetRequiredState_TC_07_001_007
 * @subsection PmSetRequiredState_TC_07_001_007_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetRequiredState_TC_07_001_007_tci Test Case ID
 * PmSetRequiredState_S0_BA_TC_07_001_007
 * @subsection PmSetRequiredState_TC_07_001_007_rr Related Requirement(s)
 * \#239132, \#239134 
 * @subsection PmSetRequiredState_TC_07_001_007_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetRequiredState_TC_07_001_007_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetRequiredState function in abnormal condition:
 * - Test07_001_006: set with the required state is invalid
 * @subsection PmSetRequiredState_TC_07_001_007_tpts Test Procedure/Test Steps
 * -# Set the required state by using R_OSAL_PmSetRequiredState with the arguments as below:
 *      - Device handle of test target device
 *      - OSAL_PM_REQUIRED_STATE_INVALID
 *      - true
 * @subsection PmSetRequiredState_TC_07_001_007_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_PAR
 * @subsection PmSetRequiredState_TC_07_001_007_testcase Test Case Example
 * @snippet test_pm.cpp PmSetRequiredState_TC_07_001_007
 */
// [PmSetRequiredState_TC_07_001_007]
TEST_F(PMApiTest, PmSetRequiredState_TC_07_001_007)
{
    bool applyImmediate = true;

    // @brief: abnormal case: state in R_OSAL_PmSetRequiredState is OSAL_PM_REQUIRED_STATE_INVALID
    // Test07_001_007
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_INVALID, applyImmediate), OSAL_RETURN_PAR) << "Test07_001_007 failed";
}
// [PmSetRequiredState_TC_07_001_007]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetRequiredState_TC_07_001_008 PMApiTest.PmSetRequiredState_TC_07_001_008
 * @subsection PmSetRequiredState_TC_07_001_008_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetRequiredState_TC_07_001_008_tci Test Case ID
 * PmSetRequiredState_S0_BA_TC_07_001_008
 * @subsection PmSetRequiredState_TC_07_001_008_rr Related Requirement(s)
 * \#239132, \#239134 
 * @subsection PmSetRequiredState_TC_07_001_008_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetRequiredState_TC_07_001_008_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetRequiredState function in abnormal condition:
 * - Test07_001_008: set with the required state is OSAL_PM_REQUIRED_STATE_RELEASED + 1
 * @subsection PmSetRequiredState_TC_07_001_008_tpts Test Procedure/Test Steps
 * -# Set the required state by using R_OSAL_PmSetRequiredState with the arguments as below:
 *      - Device handle of test target device
 *      - OSAL_PM_REQUIRED_STATE_RELEASED + 1
 *      - true
 * @subsection PmSetRequiredState_TC_07_001_008_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_PAR
 * @subsection PmSetRequiredState_TC_07_001_008_testcase Test Case Example
 * @snippet test_pm.cpp PmSetRequiredState_TC_07_001_008
 */
// [PmSetRequiredState_TC_07_001_008]
TEST_F(PMApiTest, PmSetRequiredState_TC_07_001_008)
{
    bool applyImmediate = true;

    // @brief: abnormal case: state in R_OSAL_PmSetRequiredState is OSAL_PM_REQUIRED_STATE_RELEASED + 1
    // Test07_001_008
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, (e_osal_pm_required_state_t)(OSAL_PM_REQUIRED_STATE_RELEASED + 1), applyImmediate), OSAL_RETURN_PAR) << "Test07_001_008 failed";
}
// [PmSetRequiredState_TC_07_001_008]

/**
 * @page PM_TEST Power API Test
 * @section OsalApiStateTest_PmSetRequiredState_TC_07_001_009 OsalApiStateTest.PmSetRequiredState_TC_07_001_009
 * @subsection PmSetRequiredState_TC_07_001_009_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection PmSetRequiredState_TC_07_001_009_tci Test Case ID
 * PmSetRequiredState_S2_A_TC_07_001_009
 * @subsection PmSetRequiredState_TC_07_001_009_rr Related Requirement(s)
 * \#239134
 * @subsection PmSetRequiredState_TC_07_001_009_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection PmSetRequiredState_TC_07_001_009_tcs Test Case Summary
 * This test shall verify the functionality of the PM APIs in invalid state transition:
 * - Test07_001_009: Checking the state of R_OSAL_PmSetRequiredState
 * @subsection PmSetRequiredState_TC_07_001_009_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_PmSetRequiredState
 * @subsection PmSetRequiredState_TC_07_001_009_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_STATE.
 * @subsection PmSetRequiredState_TC_07_001_009_testcase Test Case Example
 * @snippet test_pm.cpp PmSetRequiredState_TC_07_001_009
 */
// [PmSetRequiredState_TC_07_001_009]
TEST_F(OsalApiStateTest, PmSetRequiredState_TC_07_001_009)
{
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;
    // Test07_001_009
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true), OSAL_RETURN_STATE) << "Test07_001_009 failed";
}
// [PmSetRequiredState_TC_07_001_009]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetRequiredState_TC_07_001_010 PMApiTest.PmSetRequiredState_TC_07_001_010
 * @subsection PmSetRequiredState_TC_07_001_010_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetRequiredState_TC_07_001_010_tci Test Case ID
 * PmSetRequiredState_S0_A_TC_07_001_010
 * @subsection PmSetRequiredState_TC_07_001_010_rr Related Requirement(s)
 * \#239132, \#239134
 * @subsection PmSetRequiredState_TC_07_001_010_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetRequiredState_TC_07_001_010_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetRequiredState function in abnormal condition:
 * - Test07_001_010: The device assigned to handle is not supported by Power Manager
 * @subsection PmSetRequiredState_TC_07_001_010_tpts Test Procedure/Test Steps
 * -# Set the required state by using R_OSAL_PmSetRequiredState with the unsupport power device handle
 * @subsection PmSetRequiredState_TC_07_001_010_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_UNSUPPORTED_OPERATION
 * @subsection PmSetRequiredState_TC_07_001_010_testcase Test Case Example
 * @snippet test_pm.cpp PmSetRequiredState_TC_07_001_010
 */
// [PmSetRequiredState_TC_07_001_010]
TEST_F(PMApiTest, PmSetRequiredState_TC_07_001_010)
{
    bool applyImmediate = true;

    // @brief: abnormal case: The device assigned to handle is not supported by Power Manager
    // Test07_001_010
    EXPECT_EQ(R_OSAL_PmSetRequiredState(unsupport_power_dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_UNSUPPORTED_OPERATION) << "Test07_001_010 failed";
}
// [PmSetRequiredState_TC_07_001_010]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetRequiredState_TC_07_001_011 PMApiTest.PmSetRequiredState_TC_07_001_011
 * @subsection PmSetRequiredState_TC_07_001_011_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetRequiredState_TC_07_001_011_tci Test Case ID
 * PmSetRequiredState_S0_A_TC_07_001_011
 * @subsection PmSetRequiredState_TC_07_001_011_rr Related Requirement(s)
 * \#239132, \#239134
 * @subsection PmSetRequiredState_TC_07_001_011_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetRequiredState_TC_07_001_011_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetRequiredState function in abnormal condition:
 * - Test07_001_011: The device assigned to handle is not supported by Power Manager
 * @subsection PmSetRequiredState_TC_07_001_011_tpts Test Procedure/Test Steps
 * -# Set the required state by using R_OSAL_PmSetRequiredState with the unsupport clock device handle
 * @subsection PmSetRequiredState_TC_07_001_011_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_UNSUPPORTED_OPERATION
 * @subsection PmSetRequiredState_TC_07_001_011_testcase Test Case Example
 * @snippet test_pm.cpp PmSetRequiredState_TC_07_001_011
 */
// [PmSetRequiredState_TC_07_001_011]
TEST_F(PMApiTest, PmSetRequiredState_TC_07_001_011)
{
    bool applyImmediate = true;

    // @brief: abnormal case: The device assigned to handle is not supported by Power Manager
    // Test07_001_011
    EXPECT_EQ(R_OSAL_PmSetRequiredState(unsupport_clock_dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_UNSUPPORTED_OPERATION) << "Test07_001_011 failed";
}
// [PmSetRequiredState_TC_07_001_011]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetRequiredState_TC_07_001_012 PMApiTest.PmSetRequiredState_TC_07_001_012
 * @subsection PmSetRequiredState_TC_07_001_012_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetRequiredState_TC_07_001_012_tci Test Case ID
 * PmSetRequiredState_S1_BN_TC_07_001_012
 * @subsection PmSetRequiredState_TC_07_001_012_rr Related Requirement(s)
 * \#239132, \#239134
 * @subsection PmSetRequiredState_TC_07_001_012_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetRequiredState_TC_07_001_012_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetRequiredState function in normal condition:
 * - Test07_001_012: Asynchronous pm required state transition to OSAL_PM_REQUIRED_STATE_REQUIRED when reset state is OSAL_PM_RESET_STATE_RELEASED
 * @subsection PmSetRequiredState_TC_07_001_012_tpts Test Procedure/Test Steps
 * -# Change reset state to OSAL_PM_RESET_STATE_RELEASED by using R_OSAL_PmSetResetState
 * -# Change required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState with applyImmediate is false
 * -# Wait for state OSAL_PM_STATE_READY by using R_OSAL_PmWaitForState
 * -# Change required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState with applyImmediate is true
 * @subsection PmSetRequiredState_TC_07_001_012_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * @subsection PmSetRequiredState_TC_07_001_012_testcase Test Case Example
 * @snippet test_pm.cpp PmSetRequiredState_TC_07_001_012
 */
// [PmSetRequiredState_TC_07_001_012]
TEST_F(PMApiTest, PmSetRequiredState_TC_07_001_012)
{
    bool applyImmediate = false;
    osal_milli_sec_t waiting_time = 1000;

    // @brief: normal case: asynchronous pm required state transition to OSAL_PM_REQUIRED_STATE_REQUIRED when reset state is OSAL_PM_RESET_STATE_RELEASED
    // Test07_001_012
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle1, OSAL_PM_RESET_STATE_RELEASED), OSAL_RETURN_OK) << "Test07_001_012 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK) << "Test07_001_012 failed";
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle1, OSAL_PM_STATE_READY, waiting_time), OSAL_RETURN_OK) << "Test07_001_012 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, true), OSAL_RETURN_OK) << "Test07_001_012 failed";
}
// [PmSetRequiredState_TC_07_001_012]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetRequiredState_TC_07_001_013 PMApiTest.PmSetRequiredState_TC_07_001_013
 * @subsection PmSetRequiredState_TC_07_001_013_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetRequiredState_TC_07_001_013_tci Test Case ID
 * PmSetRequiredState_S1_BN_TC_07_001_013
 * @subsection PmSetRequiredState_TC_07_001_013_rr Related Requirement(s)
 * \#239132, \#239134
 * @subsection PmSetRequiredState_TC_07_001_013_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetRequiredState_TC_07_001_013_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetRequiredState function in normal condition:
 * - Test07_001_013: Asynchronous pm required state transition to OSAL_PM_REQUIRED_STATE_REQUIRED when reset state is OSAL_PM_RESET_STATE_APPLIED
 * @subsection PmSetRequiredState_TC_07_001_013_tpts Test Procedure/Test Steps
 * -# Change reset state to OSAL_PM_RESET_STATE_APPLIED by using R_OSAL_PmSetResetState
 * -# Change required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState with applyImmediate is false
 * -# Wait for state OSAL_PM_STATE_RESET by using R_OSAL_PmWaitForState
 * -# Change reset state to OSAL_PM_RESET_STATE_RELEASED by using R_OSAL_PmSetResetState
 * -# Change required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState with applyImmediate is true
 * @subsection PmSetRequiredState_TC_07_001_013_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * @subsection PmSetRequiredState_TC_07_001_013_testcase Test Case Example
 * @snippet test_pm.cpp PmSetRequiredState_TC_07_001_013
 */
// [PmSetRequiredState_TC_07_001_013]
TEST_F(PMApiTest, PmSetRequiredState_TC_07_001_013)
{
    bool applyImmediate = false;
    osal_milli_sec_t waiting_time = 1000;

    // @brief: normal case: asynchronous pm required state transition to OSAL_PM_REQUIRED_STATE_REQUIRED when reset state is OSAL_PM_RESET_STATE_APPLIED
    // Test07_001_013
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle1, OSAL_PM_RESET_STATE_APPLIED), OSAL_RETURN_OK) << "Test07_001_013 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK) << "Test07_001_013 failed";
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle1, OSAL_PM_STATE_RESET, waiting_time), OSAL_RETURN_OK) << "Test07_001_013 failed";
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle1, OSAL_PM_RESET_STATE_RELEASED), OSAL_RETURN_OK) << "Test07_001_013 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, true), OSAL_RETURN_OK) << "Test07_001_013 failed";
}
// [PmSetRequiredState_TC_07_001_013]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetRequiredState_TC_07_001_014 PMApiTest.PmSetRequiredState_TC_07_001_014
 * @subsection PmSetRequiredState_TC_07_001_014_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetRequiredState_TC_07_001_014_tci Test Case ID
 * PmSetRequiredState_S1_BN_TC_07_001_014
 * @subsection PmSetRequiredState_TC_07_001_014_rr Related Requirement(s)
 * \#239132, \#239134
 * @subsection PmSetRequiredState_TC_07_001_014_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetRequiredState_TC_07_001_014_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetRequiredState function in normal condition:
 * - Test07_001_014: Asynchronous pm required state transition to OSAL_PM_REQUIRED_STATE_RELEASED when policy is OSAL_PM_POLICY_HP
 * @subsection PmSetRequiredState_TC_07_001_014_tpts Test Procedure/Test Steps
 * -# Change required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState with applyImmediate is true
 * -# Change pm policy to OSAL_PM_POLICY_HP by using R_OSAL_PmSetPolicy with applyImmediate is false
 * -# Change required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState with applyImmediate is false
 * -# Wait for state OSAL_PM_STATE_ENABLED by using R_OSAL_PmWaitForState
 * @subsection PmSetRequiredState_TC_07_001_014_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK
 * @subsection PmSetRequiredState_TC_07_001_014_testcase Test Case Example
 * @snippet test_pm.cpp PmSetRequiredState_TC_07_001_014
 */
// [PmSetRequiredState_TC_07_001_014]
TEST_F(PMApiTest, PmSetRequiredState_TC_07_001_014)
{
    bool applyImmediate = false;
    osal_milli_sec_t waiting_time = 1000;

    // @brief: normal case: asynchronous pm required state transition to OSAL_PM_REQUIRED_STATE_RELEASED when policy is HP
    // Test07_001_014
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true), OSAL_RETURN_OK) << "Test07_001_014 failed";
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, OSAL_PM_POLICY_HP, applyImmediate), OSAL_RETURN_OK) << "Test07_001_014 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_001_014 failed";
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle, OSAL_PM_STATE_ENABLED, waiting_time), OSAL_RETURN_OK) << "Test07_001_014 failed";
}
// [PmSetRequiredState_TC_07_001_014]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetRequiredState_TC_07_001_015 PMApiTest.PmSetRequiredState_TC_07_001_015
 * @subsection PmSetRequiredState_TC_07_001_015_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetRequiredState_TC_07_001_015_tci Test Case ID
 * PmSetRequiredState_S1_BN_TC_07_001_015
 * @subsection PmSetRequiredState_TC_07_001_015_rr Related Requirement(s)
 * \#239132, \#239134
 * @subsection PmSetRequiredState_TC_07_001_015_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetRequiredState_TC_07_001_015_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetRequiredState function in normal condition:
 * - Test07_001_015: Asynchronous pm required state transition to OSAL_PM_REQUIRED_STATE_RELEASED when policy is OSAL_PM_POLICY_PG
 * @subsection PmSetRequiredState_TC_07_001_015_tpts Test Procedure/Test Steps
 * -# Change required state of target device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState with applyImmediate is true
 * -# Change required state of the dependence device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState with applyImmediate is true
 * -# Change pm policy of the dependence device to OSAL_PM_POLICY_HP by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Change required state of the dependence device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState with applyImmediate is true
 * -# Change pm policy to OSAL_PM_POLICY_HP by using R_OSAL_PmSetPolicy with applyImmediate is false
 * -# Change required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState with applyImmediate is false
 * -# Wait for state PM_STATE_EXPECTED_VALUE by using R_OSAL_PmWaitForState
 * @subsection PmSetRequiredState_TC_07_001_015_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK
 * @subsection PmSetRequiredState_TC_07_001_015_testcase Test Case Example
 * @snippet test_pm.cpp PmSetRequiredState_TC_07_001_015
 */
// [PmSetRequiredState_TC_07_001_015]
TEST_F(PMApiTest, PmSetRequiredState_TC_07_001_015)
{
    bool applyImmediate = false;
    osal_milli_sec_t waiting_time = 1000;

    // @brief: normal case: asynchronous pm required state transition to OSAL_PM_REQUIRED_STATE_RELEASED when policy is PG
    // Test07_001_015
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true), OSAL_RETURN_OK) << "Test07_001_015 failed";
#if !defined(OSAL_SOC_S4) /* On QNX, imp_00's power can't be OFF without imp_01's power is OFF */
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle_dependence, OSAL_PM_REQUIRED_STATE_REQUIRED, true), OSAL_RETURN_OK) << "Test07_001_015 failed";
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle_dependence, OSAL_PM_POLICY_PG, true), OSAL_RETURN_OK) << "Test07_001_015 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle_dependence, OSAL_PM_REQUIRED_STATE_RELEASED, true), OSAL_RETURN_OK) << "Test07_001_015 failed";
#endif
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK) << "Test07_001_015 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_001_015 failed";
    /* S4 (power domain is alway ON): PM_STATE_EXPECTED_VALUE is OSAL_PM_STATE_CG
       V3x, V4H: PM_STATE_EXPECTED_VALUE is OSAL_PM_STATE_PG */
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle, PM_STATE_EXPECTED_VALUE, waiting_time), OSAL_RETURN_OK) << "Test07_001_015 failed";
}
// [PmSetRequiredState_TC_07_001_015]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetRequiredState_TC_07_001_016 PMApiTest.PmSetRequiredState_TC_07_001_016
 * @subsection PmSetRequiredState_TC_07_001_016_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetRequiredState_TC_07_001_016_tci Test Case ID
 * PmSetRequiredState_S1_BN_TC_07_001_016
 * @subsection PmSetRequiredState_TC_07_001_016_rr Related Requirement(s)
 * \#239132, \#239134
 * @subsection PmSetRequiredState_TC_07_001_016_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetRequiredState_TC_07_001_016_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetRequiredState function in normal condition:
 * - Test07_001_016: Synchronous pm required state transition to OSAL_PM_REQUIRED_STATE_REQUIRED when reset state is OSAL_PM_RESET_STATE_RELEASED
 * @subsection PmSetRequiredState_TC_07_001_016_tpts Test Procedure/Test Steps
 * -# Change reset state to OSAL_PM_RESET_STATE_RELEASED by using R_OSAL_PmSetResetState
 * -# Change required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState with applyImmediate is true
 * -# Wait for state OSAL_PM_STATE_READY by using R_OSAL_PmWaitForState
 * -# Change required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState with applyImmediate is true
 * @subsection PmSetRequiredState_TC_07_001_016_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * @subsection PmSetRequiredState_TC_07_001_016_testcase Test Case Example
 * @snippet test_pm.cpp PmSetRequiredState_TC_07_001_016
 */
// [PmSetRequiredState_TC_07_001_016]
TEST_F(PMApiTest, PmSetRequiredState_TC_07_001_016)
{
    bool applyImmediate = true;
    osal_milli_sec_t waiting_time = 1000;

    // @brief: normal case: synchronous pm required state transition to OSAL_PM_REQUIRED_STATE_REQUIRED when reset state is OSAL_PM_RESET_STATE_RELEASED
    // Test07_001_016
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle1, OSAL_PM_RESET_STATE_RELEASED), OSAL_RETURN_OK) << "Test07_001_016 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK) << "Test07_001_016 failed";
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle1, OSAL_PM_STATE_READY, waiting_time), OSAL_RETURN_OK) << "Test07_001_016 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, true), OSAL_RETURN_OK) << "Test07_001_016 failed";
}
// [PmSetRequiredState_TC_07_001_016]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetRequiredState_TC_07_001_017 PMApiTest.PmSetRequiredState_TC_07_001_017
 * @subsection PmSetRequiredState_TC_07_001_017_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetRequiredState_TC_07_001_017_tci Test Case ID
 * PmSetRequiredState_S1_BN_TC_07_001_017
 * @subsection PmSetRequiredState_TC_07_001_017_rr Related Requirement(s)
 * \#239132, \#239134
 * @subsection PmSetRequiredState_TC_07_001_017_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetRequiredState_TC_07_001_017_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetRequiredState function in normal condition:
 * - Test07_001_017: Synchronous pm required state transition to OSAL_PM_REQUIRED_STATE_REQUIRED when reset state is OSAL_PM_RESET_STATE_APPLIED
 * @subsection PmSetRequiredState_TC_07_001_017_tpts Test Procedure/Test Steps
 * -# Change reset state to OSAL_PM_RESET_STATE_APPLIED by using R_OSAL_PmSetResetState
 * -# Change required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState with applyImmediate is true
 * -# Wait for state OSAL_PM_STATE_RESET by using R_OSAL_PmWaitForState
 * -# Change reset state to OSAL_PM_RESET_STATE_RELEASED by using R_OSAL_PmSetResetState
 * -# Change required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState with applyImmediate is true
 * @subsection PmSetRequiredState_TC_07_001_017_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * @subsection PmSetRequiredState_TC_07_001_017_testcase Test Case Example
 * @snippet test_pm.cpp PmSetRequiredState_TC_07_001_017
 */
// [PmSetRequiredState_TC_07_001_017]
TEST_F(PMApiTest, PmSetRequiredState_TC_07_001_017)
{
    bool applyImmediate = true;
    osal_milli_sec_t waiting_time = 1000;

    // @brief: normal case: synchronous pm required state transition to OSAL_PM_REQUIRED_STATE_REQUIRED when reset state is OSAL_PM_RESET_STATE_APPLIED
    // Test07_001_017
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle1, OSAL_PM_RESET_STATE_APPLIED), OSAL_RETURN_OK) << "Test07_001_017 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK) << "Test07_001_017 failed";
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle1, OSAL_PM_STATE_RESET, waiting_time), OSAL_RETURN_OK) << "Test07_001_017 failed";
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle1, OSAL_PM_RESET_STATE_RELEASED), OSAL_RETURN_OK) << "Test07_001_017 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, true), OSAL_RETURN_OK) << "Test07_001_017 failed";
}
// [PmSetRequiredState_TC_07_001_017]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetRequiredState_TC_07_001_018 PMApiTest.PmSetRequiredState_TC_07_001_018
 * @subsection PmSetRequiredState_TC_07_001_018_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetRequiredState_TC_07_001_018_tci Test Case ID
 * PmSetRequiredState_S1_BN_TC_07_001_018
 * @subsection PmSetRequiredState_TC_07_001_018_rr Related Requirement(s)
 * \#239132, \#239134
 * @subsection PmSetRequiredState_TC_07_001_018_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetRequiredState_TC_07_001_018_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetRequiredState function in normal condition:
 * - Test07_001_018: Synchronous pm required state transition to OSAL_PM_REQUIRED_STATE_RELEASED when policy is OSAL_PM_POLICY_HP
 * @subsection PmSetRequiredState_TC_07_001_018_tpts Test Procedure/Test Steps
 * -# Change required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState with applyImmediate is true
 * -# Change pm policy to OSAL_PM_POLICY_HP by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Change required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState with applyImmediate is true
 * -# Wait for state OSAL_PM_STATE_ENABLED by using R_OSAL_PmWaitForState
 * @subsection PmSetRequiredState_TC_07_001_018_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK
 * @subsection PmSetRequiredState_TC_07_001_018_testcase Test Case Example
 * @snippet test_pm.cpp PmSetRequiredState_TC_07_001_018
 */
// [PmSetRequiredState_TC_07_001_018]
TEST_F(PMApiTest, PmSetRequiredState_TC_07_001_018)
{
    bool applyImmediate = true;
    osal_milli_sec_t waiting_time = 1000;

    // @brief: normal case: synchronous pm required state transition to OSAL_PM_REQUIRED_STATE_RELEASED when policy is HP
    // Test07_001_018
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true), OSAL_RETURN_OK) << "Test07_001_018 failed";
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, OSAL_PM_POLICY_HP, applyImmediate), OSAL_RETURN_OK) << "Test07_001_018 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_001_018 failed";
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle, OSAL_PM_STATE_ENABLED, waiting_time), OSAL_RETURN_OK) << "Test07_001_018 failed";
}
// [PmSetRequiredState_TC_07_001_018]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetRequiredState_TC_07_001_019 PMApiTest.PmSetRequiredState_TC_07_001_019
 * @subsection PmSetRequiredState_TC_07_001_019_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetRequiredState_TC_07_001_019_tci Test Case ID
 * PmSetRequiredState_S1_BN_TC_07_001_019
 * @subsection PmSetRequiredState_TC_07_001_019_rr Related Requirement(s)
 * \#239132, \#239134
 * @subsection PmSetRequiredState_TC_07_001_019_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetRequiredState_TC_07_001_019_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetRequiredState function in normal condition:
 * - Test07_001_019: synchronous pm required state transition to OSAL_PM_REQUIRED_STATE_RELEASED when policy is OSAL_PM_POLICY_PG
 * @subsection PmSetRequiredState_TC_07_001_019_tpts Test Procedure/Test Steps
 * -# Change required state of target device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState with applyImmediate is true
 * -# Change required state of the dependence device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState with applyImmediate is true
 * -# Change pm policy of the dependence device to OSAL_PM_POLICY_HP by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Change required state of the dependence device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState with applyImmediate is true
 * -# Change pm policy to OSAL_PM_POLICY_HP by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Change required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState with applyImmediate is true
 * -# Wait for state PM_STATE_EXPECTED_VALUE by using R_OSAL_PmWaitForState
 * @subsection PmSetRequiredState_TC_07_001_019_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK
 * @subsection PmSetRequiredState_TC_07_001_019_testcase Test Case Example
 * @snippet test_pm.cpp PmSetRequiredState_TC_07_001_019
 */
// [PmSetRequiredState_TC_07_001_019]
TEST_F(PMApiTest, PmSetRequiredState_TC_07_001_019)
{
    bool applyImmediate = true;
    osal_milli_sec_t waiting_time = 1000;

    // @brief: normal case: synchronous pm required state transition to OSAL_PM_REQUIRED_STATE_RELEASED when policy is PG
    // Test07_001_019
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true), OSAL_RETURN_OK) << "Test07_001_019 failed";
#if !defined(OSAL_SOC_S4) /* On QNX, imp_00's power can't be OFF without imp_01's power is OFF */
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle_dependence, OSAL_PM_REQUIRED_STATE_REQUIRED, true), OSAL_RETURN_OK) << "Test07_001_019 failed";
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle_dependence, OSAL_PM_POLICY_PG, true), OSAL_RETURN_OK) << "Test07_001_019 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle_dependence, OSAL_PM_REQUIRED_STATE_RELEASED, true), OSAL_RETURN_OK) << "Test07_001_019 failed";
#endif
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK) << "Test07_001_019 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_001_019 failed";
    /* S4 (power domain is alway ON): PM_STATE_EXPECTED_VALUE is OSAL_PM_STATE_CG
       V3x, V4H: PM_STATE_EXPECTED_VALUE is OSAL_PM_STATE_PG */
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle, PM_STATE_EXPECTED_VALUE, waiting_time), OSAL_RETURN_OK) << "Test07_001_019 failed";
}
// [PmSetRequiredState_TC_07_001_019]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetRequiredState_TC_07_001_020 PMApiTest.PmSetRequiredState_TC_07_001_020
 * @subsection PmSetRequiredState_TC_07_001_020_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetRequiredState_TC_07_001_020_tci Test Case ID
 * PmSetRequiredState_S1_BN_TC_07_001_020
 * @subsection PmSetRequiredState_TC_07_001_020_rr Related Requirement(s)
 * \#239132, \#239134
 * @subsection PmSetRequiredState_TC_07_001_020_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetRequiredState_TC_07_001_020_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetRequiredState function in normal condition:
 * - Test07_001_020: Synchronous pm required state transition to OSAL_PM_REQUIRED_STATE_RELEASED when policy is OSAL_PM_POLICY_CG
 * @subsection PmSetRequiredState_TC_07_001_020_tpts Test Procedure/Test Steps
 * -# Change required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState with applyImmediate is true
 * -# Change pm policy to OSAL_PM_POLICY_CG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Change required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState with applyImmediate is true
 * -# Wait for state OSAL_PM_STATE_CG by using R_OSAL_PmWaitForState
 * @subsection PmSetRequiredState_TC_07_001_020_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK
 * @subsection PmSetRequiredState_TC_07_001_020_testcase Test Case Example
 * @snippet test_pm.cpp PmSetRequiredState_TC_07_001_020
 */
// [PmSetRequiredState_TC_07_001_020]
TEST_F(PMApiTest, PmSetRequiredState_TC_07_001_020)
{
    bool applyImmediate = true;
    osal_milli_sec_t waiting_time = 1000;

    // @brief: normal case: synchronous pm required state transition to OSAL_PM_REQUIRED_STATE_RELEASED when policy is CG
    // Test07_001_020
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true), OSAL_RETURN_OK) << "Test07_001_020 failed";
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, OSAL_PM_POLICY_CG, applyImmediate), OSAL_RETURN_OK) << "Test07_001_020 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_001_020 failed";
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle, OSAL_PM_STATE_CG, waiting_time), OSAL_RETURN_OK) << "Test07_001_020 failed";
}
// [PmSetRequiredState_TC_07_001_020]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetRequiredState_TC_07_001_021 PMApiTest.PmSetRequiredState_TC_07_001_021
 * @subsection PmSetRequiredState_TC_07_001_021_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetRequiredState_TC_07_001_021_tci Test Case ID
 * PmSetRequiredState_S0_BA_TC_07_001_021
 * @subsection PmSetRequiredState_TC_07_001_021_rr Related Requirement(s)
 * \#239132, \#239134
 * @subsection PmSetRequiredState_TC_07_001_021_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetRequiredState_TC_07_001_021_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetRequiredState function in normal condition:
 * - Test07_001_021: Change the power required state to (OSAL_PM_REQUIRED_STATE_INVALID - 1)
 * @subsection PmSetRequiredState_TC_07_001_021_tpts Test Procedure/Test Steps
 * -# Change required state to (OSAL_PM_REQUIRED_STATE_INVALID - 1) by using R_OSAL_PmSetRequiredState with applyImmediate is true
 * @subsection PmSetRequiredState_TC_07_001_021_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_PAR
 * @subsection PmSetRequiredState_TC_07_001_021_testcase Test Case Example
 * @snippet test_pm.cpp PmSetRequiredState_TC_07_001_021
 */
// [PmSetRequiredState_TC_07_001_021]
TEST_F(PMApiTest, PmSetRequiredState_TC_07_001_021)
{
    bool applyImmediate = true;

    // @brief: boundary abnormal case: the required state is (OSAL_PM_REQUIRED_STATE_INVALID - 1)
    // Test07_001_021
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, (e_osal_pm_required_state_t)(OSAL_PM_REQUIRED_STATE_INVALID - 1), applyImmediate), OSAL_RETURN_PAR) << "Test07_001_021 failed";
}
// [PmSetRequiredState_TC_07_001_021]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetRequiredState_TC_07_002_001 PMApiTest.PmGetRequiredState_TC_07_002_001
 * @subsection PmGetRequiredState_TC_07_002_001_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetRequiredState_TC_07_002_001_tci Test Case ID
 * PmGetRequiredState_S1_N_TC_07_002_001
 * @subsection PmGetRequiredState_TC_07_002_001_rr Related Requirement(s)
 * \#239135
 * @subsection PmGetRequiredState_TC_07_002_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmGetRequiredState_TC_07_002_001_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetRequiredState function in normal condition:
 * - Test07_002_001: set OSAL_PM_REQUIRED_STATE_REQUIRED state of test target device
 * @subsection PmGetRequiredState_TC_07_002_001_tpts Test Procedure/Test Steps
 * -# Set the required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Get the current required state by using R_OSAL_PmGetRequiredState
 * -# Set the required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * @subsection PmGetRequiredState_TC_07_002_001_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.<br>
 * Value of "req_state" is equal to OSAL_PM_REQUIRED_STATE_REQUIRED
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * @subsection PmGetRequiredState_TC_07_002_001_testcase Test Case Example
 * @snippet test_pm.cpp PmGetRequiredState_TC_07_002_001
 */
// [PmGetRequiredState_TC_07_002_001]
TEST_F(PMApiTest, PmGetRequiredState_TC_07_002_001)
{
    e_osal_pm_required_state_t req_state = OSAL_PM_REQUIRED_STATE_INVALID;
    bool applyImmediate = false;

    // @brief: normal case: get OSAL_PM_REQUIRED_STATE_REQUIRED of device
    // Test07_002_001
    applyImmediate = true;
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK) << "Test07_002_001 failed";
    EXPECT_EQ(R_OSAL_PmGetRequiredState(device_handle, &req_state), OSAL_RETURN_OK) << "Test07_002_001 failed";
    EXPECT_EQ(req_state, OSAL_PM_REQUIRED_STATE_REQUIRED) << "Test07_002_001 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, false), OSAL_RETURN_OK);
}
// [PmGetRequiredState_TC_07_002_001]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetRequiredState_TC_07_002_002 PMApiTest.PmGetRequiredState_TC_07_002_002
 * @subsection PmGetRequiredState_TC_07_002_002_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetRequiredState_TC_07_002_002_tci Test Case ID
 * PmGetRequiredState_S1_N_TC_07_002_002
 * @subsection PmGetRequiredState_TC_07_002_002_rr Related Requirement(s)
 * \#239135
 * @subsection PmGetRequiredState_TC_07_002_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmGetRequiredState_TC_07_002_002_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetRequiredState function in normal condition:
 * - Test07_002_002: Get OSAL_PM_REQUIRED_STATE_RELEASED state of test target device
 * @subsection PmGetRequiredState_TC_07_002_002_tpts Test Procedure/Test Steps
 * -# Set the required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Get the current required state by using R_OSAL_PmGetRequiredState
 * @subsection PmGetRequiredState_TC_07_002_002_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_PmGetRequiredState return OSAL_RETURN_OK.<br>
 * Value of "req_state" is equal to OSAL_PM_REQUIRED_STATE_RELEASED
 * @subsection PmGetRequiredState_TC_07_002_002_testcase Test Case Example
 * @snippet test_pm.cpp PmGetRequiredState_TC_07_002_002
 */
// [PmGetRequiredState_TC_07_002_002]
TEST_F(PMApiTest, PmGetRequiredState_TC_07_002_002)
{
    e_osal_pm_required_state_t req_state = OSAL_PM_REQUIRED_STATE_INVALID;
    bool applyImmediate = false;

    // @brief: normal case: get OSAL_PM_REQUIRED_STATE_RELEASED of device
    // Test07_002_002
    applyImmediate = true;
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_002_002 failed";
    EXPECT_EQ(R_OSAL_PmGetRequiredState(device_handle, &req_state), OSAL_RETURN_OK) << "Test07_002_002 failed";
    EXPECT_EQ(req_state, OSAL_PM_REQUIRED_STATE_RELEASED) << "Test07_002_002 failed";
}
// [PmGetRequiredState_TC_07_002_002]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetRequiredState_TC_07_002_003 PMApiTest.PmGetRequiredState_TC_07_002_003
 * @subsection PmGetRequiredState_TC_07_002_003_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetRequiredState_TC_07_002_003_tci Test Case ID
 * PmGetRequiredState_S0_A_TC_07_002_003
 * @subsection PmGetRequiredState_TC_07_002_003_rr Related Requirement(s)
 * \#199151, \#239135
 * @subsection PmGetRequiredState_TC_07_002_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection PmGetRequiredState_TC_07_002_003_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetRequiredState function in abnormal condition:
 * - Test07_002_003: Get power required state with device handle is NULL
 * @subsection PmGetRequiredState_TC_07_002_003_tpts Test Procedure/Test Steps
 * -# Set the required state by using R_OSAL_PmGetRequiredState with the arguments as below:
 *      - Device handle is NULL
 *      - Address of "req_state"
 * @subsection PmGetRequiredState_TC_07_002_003_ereb Expected Result/Behavior
 * -# R_OSAL_PmGetRequiredState return OSAL_RETURN_HANDLE
 * @subsection PmGetRequiredState_TC_07_002_003_testcase Test Case Example
 * @snippet test_pm.cpp PmGetRequiredState_TC_07_002_003
 */
// [PmGetRequiredState_TC_07_002_003]
TEST_F(PMApiTest, PmGetRequiredState_TC_07_002_003)
{
    e_osal_pm_required_state_t req_state = OSAL_PM_REQUIRED_STATE_INVALID;

    // @brief: abnormal case: device handle is NULL
    // Test07_002_003
    EXPECT_EQ(R_OSAL_PmGetRequiredState(OSAL_DEVICE_HANDLE_INVALID, &req_state), OSAL_RETURN_HANDLE) << "Test07_002_003 failed";
}
// [PmGetRequiredState_TC_07_002_003]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetRequiredState_TC_07_002_004 PMApiTest.PmGetRequiredState_TC_07_002_004
 * @subsection PmGetRequiredState_TC_07_002_004_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetRequiredState_TC_07_002_004_tci Test Case ID
 * PmGetRequiredState_S0_A_TC_07_002_004
 * @subsection PmGetRequiredState_TC_07_002_004_rr Related Requirement(s)
 * \#239135
 * @subsection PmGetRequiredState_TC_07_002_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmGetRequiredState_TC_07_002_004_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetRequiredState function in abnormal condition:
 * - Test07_002_004: get required state with p_state is NULL
 * @subsection PmGetRequiredState_TC_07_002_004_tpts Test Procedure/Test Steps
 * -# Set the required state by using R_OSAL_PmGetRequiredState with the arguments as below:
 *      - Device handle of test target device
 *      - NULL
 * @subsection PmGetRequiredState_TC_07_002_004_ereb Expected Result/Behavior
 * -# R_OSAL_PmGetRequiredState return OSAL_RETURN_PAR
 * @subsection PmGetRequiredState_TC_07_002_004_testcase Test Case Example
 * @snippet test_pm.cpp PmGetRequiredState_TC_07_002_004
 */
// [PmGetRequiredState_TC_07_002_004]
TEST_F(PMApiTest, PmGetRequiredState_TC_07_002_004)
{
    e_osal_pm_required_state_t *p_returned_state = NULL;

    // Test07_002_004
    // @brief: abnormal case: p_state in R_OSAL_PmGetRequiredState is NULL
    EXPECT_EQ(R_OSAL_PmGetRequiredState(device_handle, p_returned_state), OSAL_RETURN_PAR) << "Test07_002_004 failed";
}
// [PmGetRequiredState_TC_07_002_004]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetRequiredState_TC_07_002_005 PMApiTest.PmGetRequiredState_TC_07_002_005
 * @subsection PmGetRequiredState_TC_07_002_005_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetRequiredState_TC_07_002_005_tci Test Case ID
 * PmGetRequiredState_S2_A_TC_07_002_005
 * @subsection PmGetRequiredState_TC_07_002_005_rr Related Requirement(s)
 * \#239135
 * @subsection PmGetRequiredState_TC_07_002_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmGetRequiredState_TC_07_002_005_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetRequiredState function in abnormal condition:
 * - Test07_002_005: set power required state with device handle has already been closed
 * @subsection PmGetRequiredState_TC_07_002_005_tpts Test Procedure/Test Steps
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * -# Set the required state by using R_OSAL_PmGetRequiredState with the arguments as below:
 *      - Device handle of test target device which has already been closed
 *      - Address of "req_state"
 * -# Re-open test target device for the next test cases by using R_OSAL_IoDeviceOpen
 * @subsection PmGetRequiredState_TC_07_002_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_PmGetRequiredState return OSAL_RETURN_HANDLE
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not equal to NULL_PTR
 * @subsection PmGetRequiredState_TC_07_002_005_testcase Test Case Example
 * @snippet test_pm.cpp PmGetRequiredState_TC_07_002_005
 */
// [PmGetRequiredState_TC_07_002_005]
TEST_F(PMApiTest, PmGetRequiredState_TC_07_002_005)
{
    e_osal_pm_required_state_t req_state = OSAL_PM_REQUIRED_STATE_INVALID;
    // @brief: abnormal case: device handle is closed
    // Test07_002_005
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmGetRequiredState(device_handle, &req_state), OSAL_RETURN_HANDLE) << "Test07_002_005 failed";
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_PM_DEVICE_2, &device_handle), OSAL_RETURN_OK); // re-open for the next test case
    EXPECT_NE(device_handle, NULL_PTR);
}
// [PmGetRequiredState_TC_07_002_005]

/**
 * @page PM_TEST Power API Test
 * @section OsalApiStateTest_PmGetRequiredState_TC_07_002_006 OsalApiStateTest.PmGetRequiredState_TC_07_002_006
 * @subsection PmGetRequiredState_TC_07_002_006_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection PmGetRequiredState_TC_07_002_006_tci Test Case ID
 * PmGetRequiredState_S2_A_TC_07_002_006
 * @subsection PmGetRequiredState_TC_07_002_006_rr Related Requirement(s)
 * \#239135
 * @subsection PmGetRequiredState_TC_07_002_006_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection PmGetRequiredState_TC_07_002_006_tcs Test Case Summary
 * This test shall verify the functionality of the PM APIs in invalid state transition:
 * - Test07_002_006: Checking the state of R_OSAL_PmGetRequiredState
 * @subsection PmGetRequiredState_TC_07_002_006_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_PmGetRequiredState
 * @subsection PmGetRequiredState_TC_07_002_006_ereb Expected Result/Behavior
 * -# R_OSAL_PmGetRequiredState return OSAL_RETURN_STATE. <br>
 * The returned require state is OSAL_PM_REQUIRED_STATE_REQUIRED
 * @subsection PmGetRequiredState_TC_07_002_006_testcase Test Case Example
 * @snippet test_pm.cpp PmGetRequiredState_TC_07_002_006
 */
// [PmGetRequiredState_TC_07_002_006]
TEST_F(OsalApiStateTest, PmGetRequiredState_TC_07_002_006)
{
    e_osal_pm_required_state_t req_state = OSAL_PM_REQUIRED_STATE_INVALID;
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;

    // Test07_002_006
    EXPECT_EQ(R_OSAL_PmGetRequiredState(device_handle, &req_state), OSAL_RETURN_STATE) << "Test07_002_006 failed";
    EXPECT_EQ(req_state, OSAL_PM_REQUIRED_STATE_INVALID) << "Test07_002_006 failed";
}
// [PmGetRequiredState_TC_07_002_006]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetRequiredState_TC_07_002_007 PMApiTest.PmGetRequiredState_TC_07_002_007
 * @subsection PmGetRequiredState_TC_07_002_007_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetRequiredState_TC_07_002_007_tci Test Case ID
 * PmGetRequiredState_S0_A_TC_07_002_007
 * @subsection PmGetRequiredState_TC_07_002_007_rr Related Requirement(s)
 * \#239135
 * @subsection PmGetRequiredState_TC_07_002_007_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmGetRequiredState_TC_07_002_007_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetRequiredState function in abnormal condition:
 * - Test07_002_007: The device assigned to handle is not supported by Power Manager
 * @subsection PmGetRequiredState_TC_07_002_007_tpts Test Procedure/Test Steps
 * -# Set the required state by using R_OSAL_PmGetRequiredState with the unsupport power device handle
 * @subsection PmGetRequiredState_TC_07_002_007_ereb Expected Result/Behavior
 * -# R_OSAL_PmGetRequiredState return OSAL_RETURN_UNSUPPORTED_OPERATION
 * @subsection PmGetRequiredState_TC_07_002_007_testcase Test Case Example
 * @snippet test_pm.cpp PmGetRequiredState_TC_07_002_007
 */
// [PmGetRequiredState_TC_07_002_007]
TEST_F(PMApiTest, PmGetRequiredState_TC_07_002_007)
{
    e_osal_pm_required_state_t req_state = OSAL_PM_REQUIRED_STATE_INVALID;
    // @brief: abnormal case: The device assigned to handle is not supported by Power Manager
    // Test07_002_007
    EXPECT_EQ(R_OSAL_PmGetRequiredState(unsupport_power_dev_handle, &req_state), OSAL_RETURN_UNSUPPORTED_OPERATION) << "Test07_002_007 failed";
}
// [PmGetRequiredState_TC_07_002_007]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetRequiredState_TC_07_002_008 PMApiTest.PmGetRequiredState_TC_07_002_008
 * @subsection PmGetRequiredState_TC_07_002_008_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetRequiredState_TC_07_002_008_tci Test Case ID
 * PmGetRequiredState_S0_A_TC_07_002_008
 * @subsection PmGetRequiredState_TC_07_002_008_rr Related Requirement(s)
 * \#239135
 * @subsection PmGetRequiredState_TC_07_002_008_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmGetRequiredState_TC_07_002_008_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetRequiredState function in abnormal condition:
 * - Test07_002_008: The device assigned to handle is not supported by Power Manager
 * @subsection PmGetRequiredState_TC_07_002_008_tpts Test Procedure/Test Steps
 * -# Set the required state by using R_OSAL_PmGetRequiredState with the unsupport clock device handle
 * @subsection PmGetRequiredState_TC_07_002_008_ereb Expected Result/Behavior
 * -# R_OSAL_PmGetRequiredState return OSAL_RETURN_UNSUPPORTED_OPERATION
 * @subsection PmGetRequiredState_TC_07_002_008_testcase Test Case Example
 * @snippet test_pm.cpp PmGetRequiredState_TC_07_002_008
 */
// [PmGetRequiredState_TC_07_002_008]
TEST_F(PMApiTest, PmGetRequiredState_TC_07_002_008)
{
    e_osal_pm_required_state_t req_state = OSAL_PM_REQUIRED_STATE_INVALID;
    // @brief: abnormal case: The device assigned to handle is not supported by Power Manager
    // Test07_002_008
    EXPECT_EQ(R_OSAL_PmGetRequiredState(unsupport_clock_dev_handle, &req_state), OSAL_RETURN_UNSUPPORTED_OPERATION) << "Test07_002_008 failed";
}
// [PmGetRequiredState_TC_07_002_008]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmWaitForState_TC_07_003_001 PMApiTest.PmWaitForState_TC_07_003_001
 * @subsection PmWaitForState_TC_07_003_001_tsi Test Suite ID
 * PMApiTest
 * @subsection PmWaitForState_TC_07_003_001_tci Test Case ID
 * PmWaitForState_S2_A_TC_07_003_001
 * @subsection PmWaitForState_TC_07_003_001_rr Related Requirement(s)
 * \#282095
 * @subsection PmWaitForState_TC_07_003_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmWaitForState_TC_07_003_001_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmWaitForState function in abnormal condition:
 * - Test07_003_001: wait for OSAL_PM_STATE_PG when power is under requiring
 * @subsection PmWaitForState_TC_07_003_001_tpts Test Procedure/Test Steps
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_PG  by using R_OSAL_PmSetPolicy
 * -# Wait for OSAL_PM_STATE_PG in 10ms by using R_OSAL_PmWaitForState
 * -# Set the required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * @subsection PmWaitForState_TC_07_003_001_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_TIME
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * @subsection PmWaitForState_TC_07_003_001_testcase Test Case Example
 * @snippet test_pm.cpp PmWaitForState_TC_07_003_001
 */
// [PmWaitForState_TC_07_003_001]
TEST_F(PMApiTest, PmWaitForState_TC_07_003_001)
{
    bool applyImmediate = true;

    // Test07_003_001
    // @brief: abnormal case: wait for OSAL_PM_STATE_PG when power is under requiring
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle, OSAL_PM_STATE_PG, PM_TIMEOUT), OSAL_RETURN_TIME) << "Test07_003_001 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, false), OSAL_RETURN_OK);
}
// [PmWaitForState_TC_07_003_001]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmWaitForState_TC_07_003_002 PMApiTest.PmWaitForState_TC_07_003_002
 * @subsection PmWaitForState_TC_07_003_002_tsi Test Suite ID
 * PMApiTest
 * @subsection PmWaitForState_TC_07_003_002_tci Test Case ID
 * PmWaitForState_S2_A_TC_07_003_002
 * @subsection PmWaitForState_TC_07_003_002_rr Related Requirement(s)
 * \#282095
 * @subsection PmWaitForState_TC_07_003_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmWaitForState_TC_07_003_002_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmWaitForState function in abnormal condition:
 * - Test07_003_002: wait for OSAL_PM_STATE_CG when power is under requiring
 * @subsection PmWaitForState_TC_07_003_002_tpts Test Procedure/Test Steps
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_CG  by using R_OSAL_PmSetPolicy
 * -# Wait for OSAL_PM_STATE_CG in 10ms by using R_OSAL_PmWaitForState
 * -# Set the required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * @subsection PmWaitForState_TC_07_003_002_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_TIME
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * @subsection PmWaitForState_TC_07_003_002_testcase Test Case Example
 * @snippet test_pm.cpp PmWaitForState_TC_07_003_002
 */
// [PmWaitForState_TC_07_003_002]
TEST_F(PMApiTest, PmWaitForState_TC_07_003_002)
{
    bool applyImmediate = true;

    // Test07_003_002
    // @brief: abnormal case: wait for OSAL_PM_STATE_CG when power is under requiring
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, OSAL_PM_POLICY_CG, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle, OSAL_PM_STATE_CG, PM_TIMEOUT), OSAL_RETURN_TIME) << "Test07_003_002 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, false), OSAL_RETURN_OK);
}
// [PmWaitForState_TC_07_003_002]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmWaitForState_TC_07_003_003 PMApiTest.PmWaitForState_TC_07_003_003
 * @subsection PmWaitForState_TC_07_003_003_tsi Test Suite ID
 * PMApiTest
 * @subsection PmWaitForState_TC_07_003_003_tci Test Case ID
 * PmWaitForState_S2_A_TC_07_003_003
 * @subsection PmWaitForState_TC_07_003_003_rr Related Requirement(s)
 * \#282095
 * @subsection PmWaitForState_TC_07_003_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmWaitForState_TC_07_003_003_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmWaitForState function in abnormal condition:
 * - Test07_003_003: wait for OSAL_PM_STATE_ENABLED when power is under requiring
 * @subsection PmWaitForState_TC_07_003_003_tpts Test Procedure/Test Steps
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_HP  by using R_OSAL_PmSetPolicy
 * -# Wait for OSAL_PM_STATE_ENABLED in 10ms by using R_OSAL_PmWaitForState
 * -# Set the required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * @subsection PmWaitForState_TC_07_003_003_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_TIME
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * @subsection PmWaitForState_TC_07_003_003_testcase Test Case Example
 * @snippet test_pm.cpp PmWaitForState_TC_07_003_003
 */
// [PmWaitForState_TC_07_003_003]
TEST_F(PMApiTest, PmWaitForState_TC_07_003_003)
{
    bool applyImmediate = true;

    // Test07_003_003
    // @brief: abnormal case: wait for OSAL_PM_STATE_ENABLED when power is under requiring
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, OSAL_PM_POLICY_HP, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle, OSAL_PM_STATE_ENABLED, PM_TIMEOUT), OSAL_RETURN_TIME) << "Test07_003_003 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, false), OSAL_RETURN_OK);
}
// [PmWaitForState_TC_07_003_003]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmWaitForState_TC_07_003_004 PMApiTest.PmWaitForState_TC_07_003_004
 * @subsection PmWaitForState_TC_07_003_004_tsi Test Suite ID
 * PMApiTest
 * @subsection PmWaitForState_TC_07_003_004_tci Test Case ID
 * PmWaitForState_S1_N_TC_07_003_004
 * @subsection PmWaitForState_TC_07_003_004_rr Related Requirement(s)
 * \#239129, \#282095
 * @subsection PmWaitForState_TC_07_003_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmWaitForState_TC_07_003_004_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmWaitForState function in normal condition:
 * - Test07_003_004: wait for OSAL_PM_STATE_RESET
 * @subsection PmWaitForState_TC_07_003_004_tpts Test Procedure/Test Steps
 * -# Set the required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set the reset state to OSAL_PM_RESET_STATE_APPLIED by using R_OSAL_PmSetResetState
 * -# Wait for OSAL_PM_STATE_RESET in 10ms by using R_OSAL_PmWaitForState
 * -# Set the required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * @subsection PmWaitForState_TC_07_003_004_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * @subsection PmWaitForState_TC_07_003_004_testcase Test Case Example
 * @snippet test_pm.cpp PmWaitForState_TC_07_003_004
 */
// [PmWaitForState_TC_07_003_004]
TEST_F(PMApiTest, PmWaitForState_TC_07_003_004)
{
    bool applyImmediate = true;

// Test07_003_004
// @brief: normal case: wait for OSAL_PM_STATE_RESET
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle, OSAL_PM_RESET_STATE_APPLIED), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle, OSAL_PM_STATE_RESET, PM_TIMEOUT), OSAL_RETURN_OK) << "Test07_003_004 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, false), OSAL_RETURN_OK);
}
// [PmWaitForState_TC_07_003_004]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmWaitForState_TC_07_003_005 PMApiTest.PmWaitForState_TC_07_003_005
 * @subsection PmWaitForState_TC_07_003_005_tsi Test Suite ID
 * PMApiTest
 * @subsection PmWaitForState_TC_07_003_005_tci Test Case ID
 * PmWaitForState_S1_BN_TC_07_003_005
 * @subsection PmWaitForState_TC_07_003_005_rr Related Requirement(s)
 * \#239129, \#282095
 * @subsection PmWaitForState_TC_07_003_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmWaitForState_TC_07_003_005_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmWaitForState function in normal condition:
 * - Test07_003_005: wait for OSAL_PM_STATE_READY
 * @subsection PmWaitForState_TC_07_003_005_tpts Test Procedure/Test Steps
 * -# Set the required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set the reset state to OSAL_PM_RESET_STATE_RELEASED by using R_OSAL_PmSetResetState
 * -# Wait for OSAL_PM_STATE_READY in 10ms by using R_OSAL_PmWaitForState
 * -# Set the required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * @subsection PmWaitForState_TC_07_003_005_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * @subsection PmWaitForState_TC_07_003_005_testcase Test Case Example
 * @snippet test_pm.cpp PmWaitForState_TC_07_003_005
 */
// [PmWaitForState_TC_07_003_005]
TEST_F(PMApiTest, PmWaitForState_TC_07_003_005)
{
    bool applyImmediate = true;

    // Test07_003_005
    // @brief: normal case: wait for OSAL_PM_STATE_READY
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle, OSAL_PM_RESET_STATE_RELEASED), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle, OSAL_PM_STATE_READY, PM_TIMEOUT), OSAL_RETURN_OK) << "Test07_003_005 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, false), OSAL_RETURN_OK);
}
// [PmWaitForState_TC_07_003_005]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmWaitForState_TC_07_003_006 PMApiTest.PmWaitForState_TC_07_003_006
 * @subsection PmWaitForState_TC_07_003_006_tsi Test Suite ID
 * PMApiTest
 * @subsection PmWaitForState_TC_07_003_006_tci Test Case ID
 * PmWaitForState_S1_BN_TC_07_003_006
 * @subsection PmWaitForState_TC_07_003_006_rr Related Requirement(s)
 * \#239129, \#282095
 * @subsection PmWaitForState_TC_07_003_006_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmWaitForState_TC_07_003_006_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmWaitForState function in normal condition:
 * - Test07_003_006: wait for OSAL_PM_STATE_PG
 * @subsection PmWaitForState_TC_07_003_006_tpts Test Procedure/Test Steps
 * -# Set the required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set the required state of the dependence device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set power policy of the dependence device to OSAL_PM_POLICY_PG  by using R_OSAL_PmSetPolicy
 * -# Set power required state of the dependence device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_PG  by using R_OSAL_PmSetPolicy
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Wait for OSAL_PM_STATE_PG in 10ms by using R_OSAL_PmWaitForState
 * @subsection PmWaitForState_TC_07_003_006_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK
 * @subsection PmWaitForState_TC_07_003_006_testcase Test Case Example
 * @snippet test_pm.cpp PmWaitForState_TC_07_003_006
 */
// [PmWaitForState_TC_07_003_006]
TEST_F(PMApiTest, PmWaitForState_TC_07_003_006)
{
    bool applyImmediate = true;

    // Test07_003_006
    // @brief: normal case: wait for OSAL_PM_STATE_PG
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);
#if !defined(OSAL_SOC_S4) /* R-Car S4 doesn't have PG device */
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle_dependence, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);
    //Set policy OSAL_PM_POLICY_PG + OSAL_PM_REQUIRED_STATE_RELEASED for dependence device
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle_dependence, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle_dependence, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK);
#endif
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle, PM_STATE_EXPECTED_VALUE, PM_TIMEOUT), OSAL_RETURN_OK) << "Test07_003_006 failed";
}
// [PmWaitForState_TC_07_003_006]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmWaitForState_TC_07_003_007 PMApiTest.PmWaitForState_TC_07_003_007
 * @subsection PmWaitForState_TC_07_003_007_tsi Test Suite ID
 * PMApiTest
 * @subsection PmWaitForState_TC_07_003_007_tci Test Case ID
 * PmWaitForState_S1_N_TC_07_003_007
 * @subsection PmWaitForState_TC_07_003_007_rr Related Requirement(s)
 * \#239129, \#282095
 * @subsection PmWaitForState_TC_07_003_007_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmWaitForState_TC_07_003_007_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmWaitForState function in normal condition:
 * - Test07_003_007: wait for OSAL_PM_STATE_CG
 * @subsection PmWaitForState_TC_07_003_007_tpts Test Procedure/Test Steps
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_CG  by using R_OSAL_PmSetPolicy
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Wait for OSAL_PM_STATE_CG in 10ms by using R_OSAL_PmWaitForState
 * @subsection PmWaitForState_TC_07_003_007_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK
 * @subsection PmWaitForState_TC_07_003_007_testcase Test Case Example
 * @snippet test_pm.cpp PmWaitForState_TC_07_003_007
 */
// [PmWaitForState_TC_07_003_007]
TEST_F(PMApiTest, PmWaitForState_TC_07_003_007)
{
    bool applyImmediate = true;

    // Test07_003_007
    // @brief: normal case: wait for OSAL_PM_STATE_CG
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, OSAL_PM_POLICY_CG, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle, OSAL_PM_STATE_CG, PM_TIMEOUT), OSAL_RETURN_OK) << "Test07_003_007 failed";
}
// [PmWaitForState_TC_07_003_007]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmWaitForState_TC_07_003_008 PMApiTest.PmWaitForState_TC_07_003_008
 * @subsection PmWaitForState_TC_07_003_008_tsi Test Suite ID
 * PMApiTest
 * @subsection PmWaitForState_TC_07_003_008_tci Test Case ID
 * PmWaitForState_S1_N_TC_07_003_008
 * @subsection PmWaitForState_TC_07_003_008_rr Related Requirement(s)
 * \#239129, \#282095
 * @subsection PmWaitForState_TC_07_003_008_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmWaitForState_TC_07_003_008_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmWaitForState function in normal condition:
 * - Test07_003_008: wait for OSAL_PM_STATE_ENABLED
 * @subsection PmWaitForState_TC_07_003_008_tpts Test Procedure/Test Steps
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_HP  by using R_OSAL_PmSetPolicy
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Wait for OSAL_PM_STATE_ENABLED in 10ms by using R_OSAL_PmWaitForState
 * @subsection PmWaitForState_TC_07_003_008_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK
 * @subsection PmWaitForState_TC_07_003_008_testcase Test Case Example
 * @snippet test_pm.cpp PmWaitForState_TC_07_003_008
 */
// [PmWaitForState_TC_07_003_008]
TEST_F(PMApiTest, PmWaitForState_TC_07_003_008)
{
    bool applyImmediate = true;

    // Test07_003_008
    // @brief: normal case: wait for OSAL_PM_STATE_ENABLED
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, OSAL_PM_POLICY_HP, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle, OSAL_PM_STATE_ENABLED, PM_TIMEOUT), OSAL_RETURN_OK) << "Test07_003_008 failed";
}
// [PmWaitForState_TC_07_003_008]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmWaitForState_TC_07_003_009 PMApiTest.PmWaitForState_TC_07_003_009
 * @subsection PmWaitForState_TC_07_003_009_tsi Test Suite ID
 * PMApiTest
 * @subsection PmWaitForState_TC_07_003_009_tci Test Case ID
 * PmWaitForState_S2_A_TC_07_003_009
 * @subsection PmWaitForState_TC_07_003_009_rr Related Requirement(s)
 * \#282095
 * @subsection PmWaitForState_TC_07_003_009_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmWaitForState_TC_07_003_009_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmWaitForState function in abnormal condition:
 * - Test07_003_009: wait for OSAL_PM_STATE_RESET when power has already been released
 * @subsection PmWaitForState_TC_07_003_009_tpts Test Procedure/Test Steps
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set the reset state to OSAL_PM_RESET_STATE_APPLIED by using R_OSAL_PmSetResetState
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Wait for OSAL_PM_STATE_RESET in 10ms by using R_OSAL_PmWaitForState
 * @subsection PmWaitForState_TC_07_003_009_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_TIME
 * @subsection PmWaitForState_TC_07_003_009_testcase Test Case Example
 * @snippet test_pm.cpp PmWaitForState_TC_07_003_009
 */
// [PmWaitForState_TC_07_003_009]
TEST_F(PMApiTest, PmWaitForState_TC_07_003_009)
{
    bool applyImmediate = true;

    // Test07_003_009
    // @brief: abnormal case: wait for OSAL_PM_STATE_RESET when power has already been released
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle, OSAL_PM_RESET_STATE_APPLIED), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle, OSAL_PM_STATE_RESET, PM_TIMEOUT), OSAL_RETURN_TIME) << "Test07_003_009 failed";
}
// [PmWaitForState_TC_07_003_009]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmWaitForState_TC_07_003_010 PMApiTest.PmWaitForState_TC_07_003_010
 * @subsection PmWaitForState_TC_07_003_010_tsi Test Suite ID
 * PMApiTest
 * @subsection PmWaitForState_TC_07_003_010_tci Test Case ID
 * PmWaitForState_S2_A_TC_07_003_010
 * @subsection PmWaitForState_TC_07_003_010_rr Related Requirement(s)
 * \#282095
 * @subsection PmWaitForState_TC_07_003_010_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmWaitForState_TC_07_003_010_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmWaitForState function in abnormal condition:
 * - Test07_003_010: wait for OSAL_PM_STATE_READY when power has already been released
 * @subsection PmWaitForState_TC_07_003_010_tpts Test Procedure/Test Steps
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set the reset state to OSAL_PM_RESET_STATE_RELEASED by using R_OSAL_PmSetResetState
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Wait for OSAL_PM_STATE_READY in 10ms by using R_OSAL_PmWaitForState
 * @subsection PmWaitForState_TC_07_003_010_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_TIME
 * @subsection PmWaitForState_TC_07_003_010_testcase Test Case Example
 * @snippet test_pm.cpp PmWaitForState_TC_07_003_010
 */
// [PmWaitForState_TC_07_003_010]
TEST_F(PMApiTest, PmWaitForState_TC_07_003_010)
{
    bool applyImmediate = true;

    // Test07_003_010
    // @brief: abnormal case: wait for OSAL_PM_STATE_READY when power has already been released
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle, OSAL_PM_RESET_STATE_RELEASED), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle, OSAL_PM_STATE_READY, PM_TIMEOUT), OSAL_RETURN_TIME) << "Test07_003_010 failed";
}
// [PmWaitForState_TC_07_003_010]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmWaitForState_TC_07_003_011 PMApiTest.PmWaitForState_TC_07_003_011
 * @subsection PmWaitForState_TC_07_003_011_tsi Test Suite ID
 * PMApiTest
 * @subsection PmWaitForState_TC_07_003_011_tci Test Case ID
 * PmWaitForState_S0_A_TC_07_003_011
 * @subsection PmWaitForState_TC_07_003_011_rr Related Requirement(s)
 * \#199151, \#282095
 * @subsection PmWaitForState_TC_07_003_011_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection PmWaitForState_TC_07_003_011_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmWaitForState function in abnormal condition:
 * - Test07_003_011: device handle is NULL
 * @subsection PmWaitForState_TC_07_003_011_tpts Test Procedure/Test Steps
 * -# Wait power state by using R_OSAL_PmWaitForState with the arguments as below:
 *      - Device handle is NULL
 *      - OSAL_PM_STATE_CG
 *      - 10ms
 * @subsection PmWaitForState_TC_07_003_011_ereb Expected Result/Behavior
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_HANDLE
 * @subsection PmWaitForState_TC_07_003_011_testcase Test Case Example
 * @snippet test_pm.cpp PmWaitForState_TC_07_003_011
 */
// [PmWaitForState_TC_07_003_011]
TEST_F(PMApiTest, PmWaitForState_TC_07_003_011)
{
    // Test07_003_011
    // @brief: abnormal case: device handle is NULL
    EXPECT_EQ(R_OSAL_PmWaitForState(OSAL_DEVICE_HANDLE_INVALID, OSAL_PM_STATE_CG, PM_TIMEOUT), OSAL_RETURN_HANDLE) << "Test07_003_011 failed";
}
// [PmWaitForState_TC_07_003_011]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmWaitForState_TC_07_003_012 PMApiTest.PmWaitForState_TC_07_003_012
 * @subsection PmWaitForState_TC_07_003_012_tsi Test Suite ID
 * PMApiTest
 * @subsection PmWaitForState_TC_07_003_012_tci Test Case ID
 * PmWaitForState_S0_A_TC_07_003_012
 * @subsection PmWaitForState_TC_07_003_012_rr Related Requirement(s)
 * \#282095
 * @subsection PmWaitForState_TC_07_003_012_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmWaitForState_TC_07_003_012_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmWaitForState function in abnormal condition:
 * - Test07_003_012: device handle is closed
 * @subsection PmWaitForState_TC_07_003_012_tpts Test Procedure/Test Steps
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * -# Wait power state by using R_OSAL_PmWaitForState with the arguments as below:
 *      - Device handle of test target device which has already been closed
 *      - OSAL_PM_STATE_CG
 *      - 10ms
 * -# Re-open test target device by using R_OSAL_IoDeviceOpen
 * @subsection PmWaitForState_TC_07_003_012_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_HANDLE
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not equal to NULL_PTR
 * @subsection PmWaitForState_TC_07_003_012_testcase Test Case Example
 * @snippet test_pm.cpp PmWaitForState_TC_07_003_012
 */
// [PmWaitForState_TC_07_003_012]
TEST_F(PMApiTest, PmWaitForState_TC_07_003_012)
{
    // Test07_003_012
    // @brief: abnormal case: device handle is closed
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle, OSAL_PM_STATE_CG, PM_TIMEOUT), OSAL_RETURN_HANDLE) << "Test07_003_012 failed";
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_PM_DEVICE_2, &device_handle), OSAL_RETURN_OK); // re-open for the next test case
    EXPECT_NE(device_handle, NULL_PTR);
}
// [PmWaitForState_TC_07_003_012]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmWaitForState_TC_07_003_013 PMApiTest.PmWaitForState_TC_07_003_013
 * @subsection PmWaitForState_TC_07_003_013_tsi Test Suite ID
 * PMApiTest
 * @subsection PmWaitForState_TC_07_003_013_tci Test Case ID
 * PmWaitForState_S0_BA_TC_07_003_013
 * @subsection PmWaitForState_TC_07_003_013_rr Related Requirement(s)
 * \#239129, \#282095
 * @subsection PmWaitForState_TC_07_003_013_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmWaitForState_TC_07_003_013_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmWaitForState function in abnormal condition:
 * - Test07_003_013: state is OSAL_PM_STATE_INVALID
 * @subsection PmWaitForState_TC_07_003_013_tpts Test Procedure/Test Steps
 * -# Wait for OSAL_PM_STATE_INVALID in 10ms by using R_OSAL_PmWaitForState
 * @subsection PmWaitForState_TC_07_003_013_ereb Expected Result/Behavior
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_PAR
 * @subsection PmWaitForState_TC_07_003_013_testcase Test Case Example
 * @snippet test_pm.cpp PmWaitForState_TC_07_003_013
 */
// [PmWaitForState_TC_07_003_013]
TEST_F(PMApiTest, PmWaitForState_TC_07_003_013)
{
    // Test07_003_013
    // @brief: abnormal case: state is OSAL_PM_STATE_INVALID
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle, OSAL_PM_STATE_INVALID, PM_TIMEOUT), OSAL_RETURN_PAR) << "Test07_003_013 failed";
}
// [PmWaitForState_TC_07_003_013]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmWaitForState_TC_07_003_014 PMApiTest.PmWaitForState_TC_07_003_014
 * @subsection PmWaitForState_TC_07_003_014_tsi Test Suite ID
 * PMApiTest
 * @subsection PmWaitForState_TC_07_003_014_tci Test Case ID
 * PmWaitForState_S0_BA_TC_07_003_014
 * @subsection PmWaitForState_TC_07_003_014_rr Related Requirement(s)
 * \#239129, \#282095
 * @subsection PmWaitForState_TC_07_003_014_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmWaitForState_TC_07_003_014_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmWaitForState function in abnormal condition:
 * - Test07_003_014: state is OSAL_PM_STATE_READY + 1
 * @subsection PmWaitForState_TC_07_003_014_tpts Test Procedure/Test Steps
 * -# Wait power state by using R_OSAL_PmWaitForState with the arguments as below:
 *      - Device handle of test target device
 *      - OSAL_PM_STATE_READY + 1
 *      - 10ms
 * @subsection PmWaitForState_TC_07_003_014_ereb Expected Result/Behavior
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_PAR
 * @subsection PmWaitForState_TC_07_003_014_testcase Test Case Example
 * @snippet test_pm.cpp PmWaitForState_TC_07_003_014
 */
// [PmWaitForState_TC_07_003_014]
TEST_F(PMApiTest, PmWaitForState_TC_07_003_014)
{
    // Test07_003_014
    // @brief: abnormal case: state is OSAL_PM_STATE_READY + 1
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle, (e_osal_pm_state_t)(OSAL_PM_STATE_READY + 1), PM_TIMEOUT), OSAL_RETURN_PAR) << "Test07_003_014 failed";
}
// [PmWaitForState_TC_07_003_014]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmWaitForState_TC_07_003_015 PMApiTest.PmWaitForState_TC_07_003_015
 * @subsection PmWaitForState_TC_07_003_015_tsi Test Suite ID
 * PMApiTest
 * @subsection PmWaitForState_TC_07_003_015_tci Test Case ID
 * PmWaitForState_S0_BA_TC_07_003_015
 * @subsection PmWaitForState_TC_07_003_015_rr Related Requirement(s)
 * \#282095
 * @subsection PmWaitForState_TC_07_003_015_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmWaitForState_TC_07_003_015_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmWaitForState function in abnormal condition:
 * - Test07_003_015: timeout is negative number
 * @subsection PmWaitForState_TC_07_003_015_tpts Test Procedure/Test Steps
 * -# Wait power state by using R_OSAL_PmWaitForState with the arguments as below:
 *      - Device handle of test target device
 *      - OSAL_PM_STATE_CG
 *      - (-1)ms
 * @subsection PmWaitForState_TC_07_003_015_ereb Expected Result/Behavior
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_PAR
 * @subsection PmWaitForState_TC_07_003_015_testcase T est Case Example
 * @snippet test_pm.cpp PmWaitForState_TC_07_003_015
 */
// [PmWaitForState_TC_07_003_015]
TEST_F(PMApiTest, PmWaitForState_TC_07_003_015)
{
    int negative_time = -1;

    // Test07_003_015
    // @brief: abnormal case: timeout is negative number
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle, OSAL_PM_STATE_CG, negative_time), OSAL_RETURN_PAR) << "Test07_003_015 failed";
}
// [PmWaitForState_TC_07_003_015]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmWaitForState_TC_07_003_016 PMApiTest.PmWaitForState_TC_07_003_016
 * @subsection PmWaitForState_TC_07_003_016_tsi Test Suite ID
 * PMApiTest
 * @subsection PmWaitForState_TC_07_003_016_tci Test Case ID
 * PmWaitForState_S0_A_TC_07_003_016
 * @subsection PmWaitForState_TC_07_003_016_rr Related Requirement(s)
 * \#239129, \#282095
 * @subsection PmWaitForState_TC_07_003_016_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmWaitForState_TC_07_003_016_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmWaitForState function in abnormal condition:
 * - Test07_003_016: current is OSAL_PM_STATE_ENABLED but wait for OSAL_PM_STATE_CG
 * @subsection PmWaitForState_TC_07_003_016_tpts Test Procedure/Test Steps
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_HP  by using R_OSAL_PmSetPolicy
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Wait for OSAL_PM_STATE_CG in 10ms by using R_OSAL_PmWaitForState
 * @subsection PmWaitForState_TC_07_003_016_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_TIME
 * @subsection PmWaitForState_TC_07_003_016_testcase Test Case Example
 * @snippet test_pm.cpp PmWaitForState_TC_07_003_016
 */
// [PmWaitForState_TC_07_003_016]
TEST_F(PMApiTest, PmWaitForState_TC_07_003_016)
{
    bool applyImmediate = true;

    // Test07_003_016
    // @brief: abnormal case: current is OSAL_PM_STATE_ENABLED but wait for OSAL_PM_STATE_CG
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, OSAL_PM_POLICY_HP, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle, OSAL_PM_STATE_CG, PM_TIMEOUT), OSAL_RETURN_TIME) << "Test07_003_016 failed";
}
// [PmWaitForState_TC_07_003_016]

/**
 * @page PM_TEST Power API Test
 * @section OsalApiStateTest_PmWaitForState_TC_07_003_017 OsalApiStateTest.PmWaitForState_TC_07_003_017
 * @subsection PmWaitForState_TC_07_003_017_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection PmWaitForState_TC_07_003_017_tci Test Case ID
 * PmWaitForState_S2_A_TC_07_003_017
 * @subsection PmWaitForState_TC_07_003_017_rr Related Requirement(s)
 * \#282095
 * @subsection PmWaitForState_TC_07_003_017_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection PmWaitForState_TC_07_003_017_tcs Test Case Summary
 * This test shall verify the functionality of the PM APIs in invalid state transition:
 * - Test07_003_017: Checking the state of R_OSAL_PmWaitForState
 * @subsection PmWaitForState_TC_07_003_017_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_PmWaitForState
 * @subsection PmWaitForState_TC_07_003_017_ereb Expected Result/Behavior
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_STATE.
 * @subsection PmWaitForState_TC_07_003_017_testcase Test Case Example
 * @snippet test_pm.cpp PmWaitForState_TC_07_003_017
 */
// [PmWaitForState_TC_07_003_017]
TEST_F(OsalApiStateTest, PmWaitForState_TC_07_003_017)
{
    osal_milli_sec_t time_period_10ms = 10;
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;

    // Test07_003_017
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle, OSAL_PM_STATE_RESET, time_period_10ms), OSAL_RETURN_STATE) << "Test07_003_017 failed";
}
// [PmWaitForState_TC_07_003_017]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmWaitForState_TC_07_003_018 PMApiTest.PmWaitForState_TC_07_003_018
 * @subsection PmWaitForState_TC_07_003_018_tsi Test Suite ID
 * PMApiTest
 * @subsection PmWaitForState_TC_07_003_018_tci Test Case ID
 * PmWaitForState_S1_N_TC_07_003_018
 * @subsection PmWaitForState_TC_07_003_018_rr Related Requirement(s)
 * \#282095
 * @subsection PmWaitForState_TC_07_003_018_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmWaitForState_TC_07_003_018_tcs Test Case Summary
 * This test shall measure the time execution of R_OSAL_PmWaitForState with normal condition:
 * - Pre-process: Change Power state to OSAL_PM_STATE_CG
 * - Test07_003_018: measure the time execution of R_OSAL_PmWaitForState with timeout is 10ms
 * @subsection PmWaitForState_TC_07_003_018_tpts Test Procedure/Test Steps
 * -# Set the required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState with applyImmediate is true
 * -# Set power policy to OSAL_PM_POLICY_CG  by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState with applyImmediate is true
 * -# Use the "for" loop in which we repeat 101 times with 4 steps as below:
 *      - Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *      - Wait for OSAL_PM_STATE_PG in 10ms by using R_OSAL_PmWaitForState
 *      - Get the current time stamp again by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *      - Get the execution time of R_OSAL_PmWaitForState by calculating the different time using R_OSAL_ClockTimeCalculateTimeDifference.<br>
 *        Execution time is stored in an array "diff_time"
 * -# Find the median of "diff_time" array.<br>
 * Compare it with 10ms plus "delta_time" (PM_WAITFORSTATE_DELTATIME + 100usec)
 * @subsection PmWaitForState_TC_07_003_018_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK.
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# The expected result of 4 steps in "for" loop are as below:
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 *      - R_OSAL_PmWaitForState return OSAL_RETURN_TIME.
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 *      - R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK.
 * -# Getting the median of "diff_time" array successfully.<br>
 * Median value need to be not exceed 10ms plus with "delta_time" (PM_WAITFORSTATE_DELTATIME + 100usec)
 * @subsection PmWaitForState_TC_07_003_018_testcase Test Case Example
 * @snippet test_pm.cpp PmWaitForState_TC_07_003_018
 * @snippet test_common.hpp findMedian
 */
// [PmWaitForState_TC_07_003_018]
TEST_F(PMApiTest, PmWaitForState_TC_07_003_018)
{
    bool applyImmediate = true;
    st_osal_time_t current_time_1st, current_time_2nd;
    osal_nano_sec_t diff_time[101];
    osal_nano_sec_t delta_time = PM_WAITFORSTATE_DELTATIME + 100000; // PM_WAITFORSTATE_DELTATIME + 100usec
    osal_nano_sec_t median = 0;
    int elements = 0;
    osal_milli_sec_t time;

    // pre-process: Change Power state to OSAL_PM_STATE_CG
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, OSAL_PM_POLICY_CG, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK);

    // Test07_003_018
    // @brief: normal case: time_period = 10
    time = 10;
    for(int i = 0; i <= 100; i++)
    {
        EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_1st), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_PmWaitForState(device_handle, OSAL_PM_STATE_PG, time), OSAL_RETURN_TIME) << "Test07_003_018 failed";
        EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_2nd), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &diff_time[i]), OSAL_RETURN_OK);
    }
    elements = sizeof(diff_time) / sizeof(diff_time[0]);
    median = (osal_nano_sec_t)findMedian(diff_time, elements);
    delta_time = PM_WAITFORSTATE_DELTATIME + 100000; // PM_WAITFORSTATE_DELTATIME + 100usec
    EXPECT_LE(median, time * MSEC_TO_NSEC + delta_time) << "Test07_003_018 failed";
}
// [PmWaitForState_TC_07_003_018]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmWaitForState_TC_07_003_019 PMApiTest.PmWaitForState_TC_07_003_019
 * @subsection PmWaitForState_TC_07_003_019_tsi Test Suite ID
 * PMApiTest
 * @subsection PmWaitForState_TC_07_003_019_tci Test Case ID
 * PmWaitForState_S1_BN_TC_07_003_019
 * @subsection PmWaitForState_TC_07_003_019_rr Related Requirement(s)
 * \#282095
 * @subsection PmWaitForState_TC_07_003_019_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmWaitForState_TC_07_003_019_tcs Test Case Summary
 * This test shall measure the time execution of R_OSAL_PmWaitForState with normal condition:
 * - Pre-process: Change Power state to OSAL_PM_STATE_CG
 * - Test07_003_019: measure the time execution of R_OSAL_PmWaitForState with timeout is 0ms
 * @subsection PmWaitForState_TC_07_003_019_tpts Test Procedure/Test Steps
 * -# Set the required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState with applyImmediate is true
 * -# Set power policy to OSAL_PM_POLICY_CG  by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState with applyImmediate is true
 * -# Use the "for" loop in which we repeat 101 times with 4 steps as below:
 *      - Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *      - Wait for OSAL_PM_STATE_PG in 0ms by using R_OSAL_PmWaitForState
 *      - Get the current time stamp again by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *      - Get the execution time of R_OSAL_PmWaitForState by calculating the different time using R_OSAL_ClockTimeCalculateTimeDifference.<br>
 *        Execution time is stored in an array "diff_time"
 * -# Find the median of "diff_time" array.<br>
 * Compare it with "delta_time" (PM_WAITFORSTATE_DELTATIME + 100usec)
 * -# Use the "for" loop in which we repeat 101 times with 4 steps as below:
 *      - Get the time stamp by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *      - Wait for OSAL_PM_STATE_CG in 0ms by using R_OSAL_PmWaitForState
 *      - Get the current time stamp again by using R_OSAL_ClockTimeGetTimeStamp with OSAL_CLOCK_TYPE_HIGH_RESOLUTION.
 *      - Get the execution time of R_OSAL_PmWaitForState by calculating the different time using R_OSAL_ClockTimeCalculateTimeDifference.<br>
 *        Execution time is stored in an array "diff_time"
 * -# Find the median of "diff_time" array.<br>
 * Compare it with "delta_time" (100usec)
 * @subsection PmWaitForState_TC_07_003_019_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK.
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK.
 * -# The expected result of 4 steps in "for" loop are as below:
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 *      - R_OSAL_PmWaitForState return OSAL_RETURN_TIME.
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 *      - R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK.
 * -# Getting the median of "diff_time" array successfully.<br>
 * Median value need to be not exceed "delta_time" (PM_WAITFORSTATE_DELTATIME + 100usec)
 * -# The expected result of 4 steps in "for" loop are as below:
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 *      - R_OSAL_PmWaitForState return OSAL_RETURN_OK.
 *      - R_OSAL_ClockTimeGetTimeStamp return OSAL_RETURN_OK.
 *      - R_OSAL_ClockTimeCalculateTimeDifference return OSAL_RETURN_OK.
 * -# Getting the median of "diff_time" array successfully.<br>
 * Median value need to be not exceed "delta_time" (100usec)
 * @subsection PmWaitForState_TC_07_003_019_testcase Test Case Example
 * @snippet test_pm.cpp PmWaitForState_TC_07_003_019
 * @snippet test_common.hpp findMedian
 */
// [PmWaitForState_TC_07_003_019]
TEST_F(PMApiTest, PmWaitForState_TC_07_003_019)
{
    bool applyImmediate = true;
    st_osal_time_t current_time_1st, current_time_2nd;
    osal_nano_sec_t diff_time[101];
    osal_nano_sec_t delta_time = PM_WAITFORSTATE_DELTATIME + 100000; // PM_WAITFORSTATE_DELTATIME + 100usec
    osal_nano_sec_t median = 0;
    int elements = 0;
    osal_milli_sec_t time;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    // pre-process: Change Power state to OSAL_PM_STATE_CG
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, OSAL_PM_POLICY_CG, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK);

    // Test07_003_019
    // @brief: normal case: time_period = 0
    time = 0;
    for(int i = 0; i <= 100; i++)
    {
        EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_1st), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_PmWaitForState(device_handle, OSAL_PM_STATE_PG, time), OSAL_RETURN_TIME) << "Test07_003_019 failed";
        EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_2nd), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &diff_time[i]), OSAL_RETURN_OK);
    }

    elements = sizeof(diff_time) / sizeof(diff_time[0]);
    median = (osal_nano_sec_t)findMedian(diff_time, elements);
    delta_time = PM_WAITFORSTATE_DELTATIME + 100000; // PM_WAITFORSTATE_DELTATIME + 100usec
    EXPECT_LE(median, delta_time) << "Test07_003_019 failed";

    for(int i = 0; i <= 100; i++)
    {
        EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_1st), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_PmWaitForState(device_handle, OSAL_PM_STATE_CG, time), OSAL_RETURN_OK) << "Test07_003_019 failed";
        EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_2nd), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &diff_time[i]), OSAL_RETURN_OK);
    }

    elements = sizeof(diff_time) / sizeof(diff_time[0]);
    median = (osal_nano_sec_t)findMedian(diff_time, elements);
    delta_time = 100000; //100usec
    EXPECT_LE(median, delta_time) << "Test07_003_019 failed";
}
// [PmWaitForState_TC_07_003_019]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmWaitForState_TC_07_003_020 PMApiTest.PmWaitForState_TC_07_003_020
 * @subsection PmWaitForState_TC_07_003_020_tsi Test Suite ID
 * PMApiTest
 * @subsection PmWaitForState_TC_07_003_020_tci Test Case ID
 * PmWaitForState_S0_A_TC_07_003_020
 * @subsection PmWaitForState_TC_07_003_020_rr Related Requirement(s)
 * \#239129, \#282095
 * @subsection PmWaitForState_TC_07_003_020_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmWaitForState_TC_07_003_020_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmWaitForState function in abnormal condition:
 * - Test07_003_020: The device assigned to handle is not supported by Power Manager
 * @subsection PmWaitForState_TC_07_003_020_tpts Test Procedure/Test Steps
 * -# Wait for OSAL_PM_STATE_ENABLED in PM_TIMEOUT by using R_OSAL_PmWaitForState with unsupport power device handle
 * @subsection PmWaitForState_TC_07_003_020_ereb Expected Result/Behavior
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_UNSUPPORTED_OPERATION
 * @subsection PmWaitForState_TC_07_003_020_testcase Test Case Example
 * @snippet test_pm.cpp PmWaitForState_TC_07_003_020
 */
// [PmWaitForState_TC_07_003_020]
TEST_F(PMApiTest, PmWaitForState_TC_07_003_020)
{
    bool applyImmediate = true;

    // Test07_003_020
    // @brief: abnormal case: The device assigned to handle is not supported by Power Manager
    EXPECT_EQ(R_OSAL_PmWaitForState(unsupport_power_dev_handle, OSAL_PM_STATE_ENABLED, PM_TIMEOUT), OSAL_RETURN_UNSUPPORTED_OPERATION) << "Test07_003_020 failed";
}
// [PmWaitForState_TC_07_003_020]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmWaitForState_TC_07_003_021 PMApiTest.PmWaitForState_TC_07_003_021
 * @subsection PmWaitForState_TC_07_003_021_tsi Test Suite ID
 * PMApiTest
 * @subsection PmWaitForState_TC_07_003_021_tci Test Case ID
 * PmWaitForState_S0_A_TC_07_003_021
 * @subsection PmWaitForState_TC_07_003_021_rr Related Requirement(s)
 * \#239129, \#282095
 * @subsection PmWaitForState_TC_07_003_021_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmWaitForState_TC_07_003_021_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmWaitForState function in abnormal condition:
 * - Test07_003_021: The device assigned to handle is not supported by Power Manager
 * @subsection PmWaitForState_TC_07_003_021_tpts Test Procedure/Test Steps
 * -# Wait for OSAL_PM_STATE_ENABLED in PM_TIMEOUT by using R_OSAL_PmWaitForState with unsupport clock device handle
 * -# Set required state of unsupport_reset_dev_handle to REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Wait for OSAL_PM_STATE_READY in PM_TIMEOUT by using R_OSAL_PmWaitForState with unsupport reset device handle
 * -# Set required state of unsupport_reset_dev_handle to RELEASED by using R_OSAL_PmSetRequiredState
 * @subsection PmWaitForState_TC_07_003_021_ereb Expected Result/Behavior
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_UNSUPPORTED_OPERATION
 * -# R_OSAL_PmSetRequiredState return PM_UNSUPPORT_PM_EXPECTED_VALUE
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_UNSUPPORTED_OPERATION
 * -# R_OSAL_PmSetRequiredState return PM_UNSUPPORT_PM_EXPECTED_VALUE
 * @subsection PmWaitForState_TC_07_003_021_testcase Test Case Example
 * @snippet test_pm.cpp PmWaitForState_TC_07_003_021
 */
// [PmWaitForState_TC_07_003_021]
TEST_F(PMApiTest, PmWaitForState_TC_07_003_021)
{
    bool applyImmediate = true;

    // Test07_003_021
    // @brief: abnormal case: The device assigned to handle is not supported by Power Manager
    EXPECT_EQ(R_OSAL_PmWaitForState(unsupport_clock_dev_handle, OSAL_PM_STATE_ENABLED, PM_TIMEOUT), OSAL_RETURN_UNSUPPORTED_OPERATION) << "Test07_003_021 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(unsupport_reset_dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true), PM_UNSUPPORT_PM_EXPECTED_VALUE) << "Test07_003_021 failed";
    EXPECT_EQ(R_OSAL_PmWaitForState(unsupport_reset_dev_handle, OSAL_PM_STATE_READY, PM_TIMEOUT), OSAL_RETURN_UNSUPPORTED_OPERATION) << "Test07_003_021 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(unsupport_reset_dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true), PM_UNSUPPORT_PM_EXPECTED_VALUE) << "Test07_003_021 failed";
}
// [PmWaitForState_TC_07_003_021]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmWaitForState_TC_07_003_022 PMApiTest.PmWaitForState_TC_07_003_022
 * @subsection PmWaitForState_TC_07_003_022_tsi Test Suite ID
 * PMApiTest
 * @subsection PmWaitForState_TC_07_003_022_tci Test Case ID
 * PmWaitForState_S0_BA_TC_07_003_022
 * @subsection PmWaitForState_TC_07_003_022_rr Related Requirement(s)
 * \#282095
 * @subsection PmWaitForState_TC_07_003_022_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmWaitForState_TC_07_003_022_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_PmWaitForState() API
 * - Test07_003_022: Using R_OSAL_PmWaitForState with state is (OSAL_PM_STATE_INVALID - 1)
 * @subsection PmWaitForState_TC_07_003_022_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_PmWaitForState with state is (OSAL_PM_STATE_INVALID - 1)
 * @subsection PmWaitForState_TC_07_003_022_ereb Expected Result/Behavior
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_PAR.
 * @subsection PmWaitForState_TC_07_003_022_testcase Test Case Example
 * @snippet test_pm.cpp PmWaitForState_TC_07_003_022
 */
// [PmWaitForState_TC_07_003_022]
TEST_F(PMApiTest, PmWaitForState_TC_07_003_022)
{
    osal_milli_sec_t time_period_10ms = 10;

    // Test07_003_022: boundary abnormal test: Using R_OSAL_PmWaitForState with state is (OSAL_PM_STATE_INVALID - 1)
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle, (e_osal_pm_state_t)(OSAL_PM_STATE_INVALID - 1), time_period_10ms), OSAL_RETURN_PAR) << "Test07_003_022 failed";
}
// [PmWaitForState_TC_07_003_022]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetState_TC_07_004_001 PMApiTest.PmGetState_TC_07_004_001
 * @subsection PmGetState_TC_07_004_001_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetState_TC_07_004_001_tci Test Case ID
 * PmGetState_S1_N_TC_07_004_001
 * @subsection PmGetState_TC_07_004_001_rr Related Requirement(s)
 * \#239137
 * @subsection PmGetState_TC_07_004_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmGetState_TC_07_004_001_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetState function in normal condition:
 * - Test07_004_001: get the state after setting power policy to OSAL_PM_POLICY_PG
 * @subsection PmGetState_TC_07_004_001_tpts Test Procedure/Test Steps
 * -# Set the power required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set the power required state of the dependence device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set the power policy of the dependence device to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy
 * -# Set the power required state of the dependence device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Set the power policy to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy
 * -# Set the power required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Get the power state by using R_OSAL_PmGetState
 * @subsection PmGetState_TC_07_004_001_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to PM_STATE_EXPECTED_VALUE
 * @subsection PmGetState_TC_07_004_001_testcase Test Case Example
 * @snippet test_pm.cpp PmGetState_TC_07_004_001
 */
// [PmGetState_TC_07_004_001]
TEST_F(PMApiTest, PmGetState_TC_07_004_001)
{
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;
    bool applyImmediate = true;

    // Test07_004_001
    // @brief: normal case: get the state after set power policy is OSAL_PM_POLICY_PG
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);
#if !defined(OSAL_SOC_S4) /* R-Car S4 doesn't have PG device */
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle_dependence, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);
    //Set policy OSAL_PM_POLICY_PG + OSAL_PM_REQUIRED_STATE_RELEASED for dependence device
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle_dependence, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle_dependence, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK);
#endif
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmGetState(device_handle, &state), OSAL_RETURN_OK) << "Test07_004_001 failed";
    EXPECT_EQ(state, PM_STATE_EXPECTED_VALUE) << "Test07_004_001 failed";
}
// [PmGetState_TC_07_004_001]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetState_TC_07_004_002 PMApiTest.PmGetState_TC_07_004_002
 * @subsection PmGetState_TC_07_004_002_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetState_TC_07_004_002_tci Test Case ID
 * PmGetState_S1_N_TC_07_004_002
 * @subsection PmGetState_TC_07_004_002_rr Related Requirement(s)
 * \#239137
 * @subsection PmGetState_TC_07_004_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmGetState_TC_07_004_002_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetState function in normal condition:
 * - Test07_004_002: get the state after setting power policy to OSAL_PM_POLICY_CG
 * @subsection PmGetState_TC_07_004_002_tpts Test Procedure/Test Steps
 * -# Set the power required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set the power policy to OSAL_PM_POLICY_CG by using R_OSAL_PmSetPolicy
 * -# Set the power required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Get the power state by using R_OSAL_PmGetState
 * @subsection PmGetState_TC_07_004_002_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_CG
 * @subsection PmGetState_TC_07_004_002_testcase Test Case Example
 * @snippet test_pm.cpp PmGetState_TC_07_004_002
 */
// [PmGetState_TC_07_004_002]
TEST_F(PMApiTest, PmGetState_TC_07_004_002)
{
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;
    bool applyImmediate = true;

    // Test07_004_002
    // @brief: normal case: get the state after set power policy is OSAL_PM_POLICY_CG
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, OSAL_PM_POLICY_CG, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmGetState(device_handle, &state), OSAL_RETURN_OK) << "Test07_004_002 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_CG) << "Test07_004_002 failed";
}
// [PmGetState_TC_07_004_002]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetState_TC_07_004_003 PMApiTest.PmGetState_TC_07_004_003
 * @subsection PmGetState_TC_07_004_003_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetState_TC_07_004_003_tci Test Case ID
 * PmGetState_S1_N_TC_07_004_003
 * @subsection PmGetState_TC_07_004_003_rr Related Requirement(s)
 * \#239137
 * @subsection PmGetState_TC_07_004_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmGetState_TC_07_004_003_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetState function in normal condition:
 * - Test07_004_003: get the state after setting power policy to OSAL_PM_POLICY_HP
 * @subsection PmGetState_TC_07_004_003_tpts Test Procedure/Test Steps
 * -# Set the power required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set the power policy to OSAL_PM_POLICY_HP by using R_OSAL_PmSetPolicy
 * -# Set the power required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Get the power state by using R_OSAL_PmGetState
 * @subsection PmGetState_TC_07_004_003_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_ENABLED
 * @subsection PmGetState_TC_07_004_003_testcase Test Case Example
 * @snippet test_pm.cpp PmGetState_TC_07_004_003
 */
// [PmGetState_TC_07_004_003]
TEST_F(PMApiTest, PmGetState_TC_07_004_003)
{
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;
    bool applyImmediate = true;

    // Test07_004_003
    // @brief: normal case: get the state after set power policy is OSAL_PM_POLICY_HP
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, OSAL_PM_POLICY_HP, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmGetState(device_handle, &state), OSAL_RETURN_OK) << "Test07_004_003 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_ENABLED) << "Test07_004_003 failed";
}
// [PmGetState_TC_07_004_003]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetState_TC_07_004_004 PMApiTest.PmGetState_TC_07_004_004
 * @subsection PmGetState_TC_07_004_004_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetState_TC_07_004_004_tci Test Case ID
 * PmGetState_S1_N_TC_07_004_004
 * @subsection PmGetState_TC_07_004_004_rr Related Requirement(s)
 * \#239137
 * @subsection PmGetState_TC_07_004_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmGetState_TC_07_004_004_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetState function in normal condition:
 * - Test07_004_004: get the state after setting reset state to OSAL_PM_RESET_STATE_APPLIED
 * @subsection PmGetState_TC_07_004_004_tpts Test Procedure/Test Steps
 * -# Set the power required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set the reset state to OSAL_PM_RESET_STATE_APPLIED by using R_OSAL_PmSetResetState
 * -# Get the power state by using R_OSAL_PmGetState
 * -# Set the required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * @subsection PmGetState_TC_07_004_004_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_RESET
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * @subsection PmGetState_TC_07_004_004_testcase Test Case Example
 * @snippet test_pm.cpp PmGetState_TC_07_004_004
 */
// [PmGetState_TC_07_004_004]
TEST_F(PMApiTest, PmGetState_TC_07_004_004)
{
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;
    bool applyImmediate = true;

    // Test07_004_004
    // @brief: normal case: get the state after set reset state is OSAL_PM_RESET_STATE_APPLIED
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle, OSAL_PM_RESET_STATE_APPLIED), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmGetState(device_handle, &state), OSAL_RETURN_OK) << "Test07_004_004 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_RESET) << "Test07_004_004 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, false), OSAL_RETURN_OK);
}
// [PmGetState_TC_07_004_004]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetState_TC_07_004_005 PMApiTest.PmGetState_TC_07_004_005
 * @subsection PmGetState_TC_07_004_005_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetState_TC_07_004_005_tci Test Case ID
 * PmGetState_S1_N_TC_07_004_005
 * @subsection PmGetState_TC_07_004_005_rr Related Requirement(s)
 * \#239137
 * @subsection PmGetState_TC_07_004_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmGetState_TC_07_004_005_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetState function in normal condition:
 * - Test07_004_005: get the state after setting reset state to OSAL_PM_RESET_STATE_RELEASED
 * @subsection PmGetState_TC_07_004_005_tpts Test Procedure/Test Steps
 * -# Set the power required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set the reset state to OSAL_PM_RESET_STATE_RELEASED by using R_OSAL_PmSetResetState
 * -# Get the power state by using R_OSAL_PmGetState
 * -# Set the required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * @subsection PmGetState_TC_07_004_005_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_READY
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * @subsection PmGetState_TC_07_004_005_testcase Test Case Example
 * @snippet test_pm.cpp PmGetState_TC_07_004_005
 */
// [PmGetState_TC_07_004_005]
TEST_F(PMApiTest, PmGetState_TC_07_004_005)
{
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;
    bool applyImmediate = true;

    // Test07_004_005
    // @brief: normal case: get the state after set reset state is OSAL_PM_RESET_STATE_RELEASED
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle, OSAL_PM_RESET_STATE_RELEASED), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmGetState(device_handle, &state), OSAL_RETURN_OK) << "Test07_004_005 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_READY) << "Test07_004_005 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, false), OSAL_RETURN_OK);
}
// [PmGetState_TC_07_004_005]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetState_TC_07_004_006 PMApiTest.PmGetState_TC_07_004_006
 * @subsection PmGetState_TC_07_004_006_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetState_TC_07_004_006_tci Test Case ID
 * PmGetState_S0_A_TC_07_004_006
 * @subsection PmGetState_TC_07_004_006_rr Related Requirement(s)
 * \#199151, \#239137
 * @subsection PmGetState_TC_07_004_006_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection PmGetState_TC_07_004_006_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetState function in abnormal condition:
 * - Test07_004_006: get power state with device handle is NULL
 * @subsection PmGetState_TC_07_004_006_tpts Test Procedure/Test Steps
 * -# Set "state" to OSAL_PM_STATE_INVALID.<br>
 * Get the power state by using R_OSAL_PmGetState with the arguments as below:
 *      - Device handle is NULL
 *      - Address of "state"
 * @subsection PmGetState_TC_07_004_006_ereb Expected Result/Behavior
 * -# R_OSAL_PmGetState return OSAL_RETURN_HANDLE.<br>
 * Value of "state" is equal to OSAL_PM_STATE_INVALID
 * @subsection PmGetState_TC_07_004_006_testcase Test Case Example
 * @snippet test_pm.cpp PmGetState_TC_07_004_006
 */
// [PmGetState_TC_07_004_006]
TEST_F(PMApiTest, PmGetState_TC_07_004_006)
{
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;

    // Test07_004_006
    // @brief: abnormal case: device handle is NULL
    EXPECT_EQ(R_OSAL_PmGetState(OSAL_DEVICE_HANDLE_INVALID, &state), OSAL_RETURN_HANDLE) << "Test07_004_006 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_INVALID) << "Test07_004_006 failed";
}
// [PmGetState_TC_07_004_006]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetState_TC_07_004_007 PMApiTest.PmGetState_TC_07_004_007
 * @subsection PmGetState_TC_07_004_007_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetState_TC_07_004_007_tci Test Case ID
 * PmGetState_S0_A_TC_07_004_007
 * @subsection PmGetState_TC_07_004_007_rr Related Requirement(s)
 * \#239137
 * @subsection PmGetState_TC_07_004_007_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmGetState_TC_07_004_007_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetState function in abnormal condition:
 * - Test07_004_007: get power state with device handle is closed
 * @subsection PmGetState_TC_07_004_007_tpts Test Procedure/Test Steps
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * -# Get the power state by using R_OSAL_PmGetState with the arguments as below:
 *      - Device handle of test target device which has already been closed
 *      - Address of "state"
 * -# Re-open test target device for the next test cases by using R_OSAL_IoDeviceOpen
 * @subsection PmGetState_TC_07_004_007_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_HANDLE
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not equal to NULL_PTR
 * @subsection PmGetState_TC_07_004_007_testcase Test Case Example
 * @snippet test_pm.cpp PmGetState_TC_07_004_007
 */
// [PmGetState_TC_07_004_007]
TEST_F(PMApiTest, PmGetState_TC_07_004_007)
{
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;

    // Test07_004_007
    // @brief: abnormal case: device handle is closed
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmGetState(device_handle, &state), OSAL_RETURN_HANDLE) << "Test07_004_007 failed";
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_PM_DEVICE_2, &device_handle), OSAL_RETURN_OK); // re-open for the next test case
    EXPECT_NE(device_handle, NULL_PTR);
}
// [PmGetState_TC_07_004_007]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetState_TC_07_004_008 PMApiTest.PmGetState_TC_07_004_008
 * @subsection PmGetState_TC_07_004_008_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetState_TC_07_004_008_tci Test Case ID
 * PmGetState_S0_A_TC_07_004_008
 * @subsection PmGetState_TC_07_004_008_rr Related Requirement(s)
 * \#239129, \#239137
 * @subsection PmGetState_TC_07_004_008_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmGetState_TC_07_004_008_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetState function in abnormal condition:
 * - Test07_004_008: get power state with p_state is NULL
 * @subsection PmGetState_TC_07_004_008_tpts Test Procedure/Test Steps
 * -# Get the power state by using R_OSAL_PmGetState with the arguments as below:
 *      - Device handle of test target device
 *      - NULL
 * @subsection PmGetState_TC_07_004_008_ereb Expected Result/Behavior
 * -# R_OSAL_PmGetState return OSAL_RETURN_PAR
 * @subsection PmGetState_TC_07_004_008_testcase Test Case Example
 * @snippet test_pm.cpp PmGetState_TC_07_004_008
 */
// [PmGetState_TC_07_004_008]
TEST_F(PMApiTest, PmGetState_TC_07_004_008)
{
    e_osal_pm_state_t *p_returned_state = NULL;

    // Test07_004_008
    // @brief: abnormal case: p_state is NULL
    EXPECT_EQ(R_OSAL_PmGetState(device_handle, p_returned_state), OSAL_RETURN_PAR) << "Test07_004_008 failed";

}
// [PmGetState_TC_07_004_008]

/**
 * @page PM_TEST Power API Test
 * @section OsalApiStateTest_PmGetState_TC_07_004_009 OsalApiStateTest.PmGetState_TC_07_004_009
 * @subsection PmGetState_TC_07_004_009_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection PmGetState_TC_07_004_009_tci Test Case ID
 * PmGetState_S2_A_TC_07_004_009
 * @subsection PmGetState_TC_07_004_009_rr Related Requirement(s)
 * \#239137
 * @subsection PmGetState_TC_07_004_009_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection PmGetState_TC_07_004_009_tcs Test Case Summary
 * This test shall verify the functionality of the PM APIs in invalid state transition:
 * - Test07_004_009: Checking the state of R_OSAL_PmGetState
 * @subsection PmGetState_TC_07_004_009_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_PmGetState
 * @subsection PmGetState_TC_07_004_009_ereb Expected Result/Behavior
 * -# R_OSAL_PmGetState return OSAL_RETURN_STATE. <br>
 * The returned state is OSAL_PM_STATE_INVALID.
 * @subsection PmGetState_TC_07_004_009_testcase Test Case Example
 * @snippet test_pm.cpp PmGetState_TC_07_004_009
 */
// [PmGetState_TC_07_004_009]
TEST_F(OsalApiStateTest, PmGetState_TC_07_004_009)
{
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;

    // Test07_004_009
    EXPECT_EQ(R_OSAL_PmGetState(device_handle, &state), OSAL_RETURN_STATE) << "Test07_004_009 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_INVALID) << "Test07_004_009 failed";
}
// [PmGetState_TC_07_004_009]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetState_TC_07_004_010 PMApiTest.PmGetState_TC_07_004_010
 * @subsection PmGetState_TC_07_004_010_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetState_TC_07_004_010_tci Test Case ID
 * PmGetState_S0_A_TC_07_004_010
 * @subsection PmGetState_TC_07_004_010_rr Related Requirement(s)
 * \#239129, \#239137
 * @subsection PmGetState_TC_07_004_010_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmGetState_TC_07_004_010_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetState function in abnormal condition:
 * - Test07_004_010: The device assigned to handle is not supported by Power Manager
 * @subsection PmGetState_TC_07_004_010_tpts Test Procedure/Test Steps
 * -# Get the power state by using R_OSAL_PmGetState with the unsupport power device handle
 * @subsection PmGetState_TC_07_004_010_ereb Expected Result/Behavior
 * -# R_OSAL_PmGetState return OSAL_RETURN_UNSUPPORTED_OPERATION
 * @subsection PmGetState_TC_07_004_010_testcase Test Case Example
 * @snippet test_pm.cpp PmGetState_TC_07_004_010
 */
// [PmGetState_TC_07_004_010]
TEST_F(PMApiTest, PmGetState_TC_07_004_010)
{
    e_osal_pm_state_t returned_state;

    // Test07_004_010
    // @brief: abnormal case: The device assigned to handle is not supported by Power Manager
    EXPECT_EQ(R_OSAL_PmGetState(unsupport_power_dev_handle, &returned_state), OSAL_RETURN_UNSUPPORTED_OPERATION) << "Test07_004_010 failed";
}
// [PmGetState_TC_07_004_010]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetState_TC_07_004_011 PMApiTest.PmGetState_TC_07_004_011
 * @subsection PmGetState_TC_07_004_011_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetState_TC_07_004_011_tci Test Case ID
 * PmGetState_S0_A_TC_07_004_011
 * @subsection PmGetState_TC_07_004_011_rr Related Requirement(s)
 * \#239129, \#239137
 * @subsection PmGetState_TC_07_004_011_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmGetState_TC_07_004_011_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetState function in abnormal condition:
 * - Test07_004_011: The device assigned to handle is not supported by Power Manager
 * @subsection PmGetState_TC_07_004_011_tpts Test Procedure/Test Steps
 * -# Get the power state by using R_OSAL_PmGetState with the unsupport clock device handle
 * -# Set required state of unsupport_reset_dev_handle to REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Get the power state by using R_OSAL_PmGetState with the unsupport reset device handle
 * -# Set required state of unsupport_reset_dev_handle to RELEASED by using R_OSAL_PmSetRequiredState
 * @subsection PmGetState_TC_07_004_011_ereb Expected Result/Behavior
 * -# R_OSAL_PmGetState return OSAL_RETURN_UNSUPPORTED_OPERATION
 * -# R_OSAL_PmSetRequiredState return PM_UNSUPPORT_PM_EXPECTED_VALUE
 * -# R_OSAL_PmGetState return OSAL_RETURN_UNSUPPORTED_OPERATION
 * -# R_OSAL_PmSetRequiredState return PM_UNSUPPORT_PM_EXPECTED_VALUE
 * @subsection PmGetState_TC_07_004_011_testcase Test Case Example
 * @snippet test_pm.cpp PmGetState_TC_07_004_011
 */
// [PmGetState_TC_07_004_011]
TEST_F(PMApiTest, PmGetState_TC_07_004_011)
{
    e_osal_pm_state_t returned_state;

    // Test07_004_011
    // @brief: abnormal case: The device assigned to handle is not supported by Power Manager
    EXPECT_EQ(R_OSAL_PmGetState(unsupport_clock_dev_handle, &returned_state), OSAL_RETURN_UNSUPPORTED_OPERATION) << "Test07_004_011 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(unsupport_reset_dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true), PM_UNSUPPORT_PM_EXPECTED_VALUE) << "Test07_004_011 failed";
    EXPECT_EQ(R_OSAL_PmGetState(unsupport_reset_dev_handle, &returned_state), OSAL_RETURN_UNSUPPORTED_OPERATION) << "Test07_004_011 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(unsupport_reset_dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true), PM_UNSUPPORT_PM_EXPECTED_VALUE) << "Test07_004_011 failed";
}
// [PmGetState_TC_07_004_011]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPolicy_TC_07_005_001 PMApiTest.PmSetPolicy_TC_07_005_001
 * @subsection PmSetPolicy_TC_07_005_001_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPolicy_TC_07_005_001_tci Test Case ID
 * PmSetPolicy_S1_BN_TC_07_005_001
 * @subsection PmSetPolicy_TC_07_005_001_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSetPolicy_TC_07_005_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetPolicy_TC_07_005_001_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPolicy function in normal condition:
 * - Test07_005_001: set policy with OSAL_PM_POLICY_PG, applyImmediate is true, in REQUIRED state
 * @subsection PmSetPolicy_TC_07_005_001_tpts Test Procedure/Test Steps
 * -# Set power required state of test target device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set power required state of the dependence device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set power policy of the dependence device to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Set power required state of the dependence device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Set power required state of test target device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Get current power policy by using R_OSAL_PmGetPolicy
 * @subsection PmSetPolicy_TC_07_005_001_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is not equal to PM_STATE_EXPECTED_VALUE
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_OK.<br>
 * Value of "policy" is equal to OSAL_PM_POLICY_PG
 * @subsection PmSetPolicy_TC_07_005_001_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPolicy_TC_07_005_001
 */
// [PmSetPolicy_TC_07_005_001]
TEST_F(PMApiTest, PmSetPolicy_TC_07_005_001)
{
    e_osal_pm_policy_t policy = OSAL_PM_POLICY_INVALID;
    bool applyImmediate = true;
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;

    // @brief: normal case: set policy with OSAL_PM_POLICY_PG, applyImmediate is true
    // Test07_005_001
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK); 
#if !defined(OSAL_SOC_S4) /* R-Car S4 doesn't have PG device */
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle_dependence, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK); 
    //Set policy OSAL_PM_POLICY_PG + OSAL_PM_REQUIRED_STATE_RELEASED for dependence device
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle_dependence, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle_dependence, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK);
#endif
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK) << "Test07_005_001 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle, &state), OSAL_RETURN_OK) << "Test07_005_001 failed";
    EXPECT_NE(state, PM_STATE_EXPECTED_VALUE) << "Test07_005_001 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_005_001 failed";
    EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle, &policy), OSAL_RETURN_OK) << "Test07_005_001 failed";
    EXPECT_EQ(policy, OSAL_PM_POLICY_PG) << "Test07_005_001 failed";
}
// [PmSetPolicy_TC_07_005_001]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPolicy_TC_07_005_002 PMApiTest.PmSetPolicy_TC_07_005_002
 * @subsection PmSetPolicy_TC_07_005_002_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPolicy_TC_07_005_002_tci Test Case ID
 * PmSetPolicy_S1_N_TC_07_005_002
 * @subsection PmSetPolicy_TC_07_005_002_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSetPolicy_TC_07_005_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetPolicy_TC_07_005_002_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPolicy function in normal condition:
 * - Test07_005_002: set policy with OSAL_PM_POLICY_CG, applyImmediate is true, in REQUIRED state
 * @subsection PmSetPolicy_TC_07_005_002_tpts Test Procedure/Test Steps
 * -# Set power required state of test target device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_CG by using R_OSAL_PmSetPolicy applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Set power required state of test target device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Get current power policy by using R_OSAL_PmGetPolicy
 * @subsection PmSetPolicy_TC_07_005_002_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is not equal to OSAL_PM_STATE_CG
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_OK.<br>
 * Value of "policy" is equal to OSAL_PM_POLICY_CG
 * @subsection PmSetPolicy_TC_07_005_002_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPolicy_TC_07_005_002
 */
// [PmSetPolicy_TC_07_005_002]
TEST_F(PMApiTest, PmSetPolicy_TC_07_005_002)
{
    e_osal_pm_policy_t policy = OSAL_PM_POLICY_INVALID;
    bool applyImmediate = true;
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;

    // @brief: normal case: set policy with OSAL_PM_POLICY_CG, applyImmediate is true
    // Test07_005_002
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, OSAL_PM_POLICY_CG, applyImmediate), OSAL_RETURN_OK) << "Test07_005_002 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle, &state), OSAL_RETURN_OK) << "Test07_005_002 failed";
    EXPECT_NE(state, OSAL_PM_STATE_CG) << "Test07_005_002 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_005_002 failed";
    EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle, &policy), OSAL_RETURN_OK) << "Test07_005_002 failed";
    EXPECT_EQ(policy, OSAL_PM_POLICY_CG) << "Test07_005_002 failed";
}
// [PmSetPolicy_TC_07_005_002]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPolicy_TC_07_005_003 PMApiTest.PmSetPolicy_TC_07_005_003
 * @subsection PmSetPolicy_TC_07_005_003_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPolicy_TC_07_005_003_tci Test Case ID
 * PmSetPolicy_S1_BN_TC_07_005_003
 * @subsection PmSetPolicy_TC_07_005_003_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSetPolicy_TC_07_005_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetPolicy_TC_07_005_003_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPolicy function in normal condition:
 * - Test07_005_003: set policy with OSAL_PM_POLICY_HP, applyImmediate is true, in REQUIRED state
 * @subsection PmSetPolicy_TC_07_005_003_tpts Test Procedure/Test Steps
 * -# Set power required state of test target device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_HP by using R_OSAL_PmSetPolicy applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Set power required state of test target device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Get current power policy by using R_OSAL_PmGetPolicy
 * @subsection PmSetPolicy_TC_07_005_003_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is not equal to OSAL_PM_STATE_ENABLED
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_OK.<br>
 * Value of "policy" is equal to OSAL_PM_POLICY_HP
 * @subsection PmSetPolicy_TC_07_005_003_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPolicy_TC_07_005_003
 */
// [PmSetPolicy_TC_07_005_003]
TEST_F(PMApiTest, PmSetPolicy_TC_07_005_003)
{
    e_osal_pm_policy_t policy = OSAL_PM_POLICY_INVALID;
    bool applyImmediate = true;
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;

    // @brief: normal case: set policy with OSAL_PM_POLICY_HP, applyImmediate is true
    // Test07_005_003
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, OSAL_PM_POLICY_HP, applyImmediate), OSAL_RETURN_OK) << "Test07_005_003 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle, &state), OSAL_RETURN_OK) << "Test07_005_003 failed";
    EXPECT_NE(state, OSAL_PM_STATE_ENABLED) << "Test07_005_003 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_005_003 failed";
    EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle, &policy), OSAL_RETURN_OK) << "Test07_005_003 failed";
    EXPECT_EQ(policy, OSAL_PM_POLICY_HP) << "Test07_005_003 failed";
}
// [PmSetPolicy_TC_07_005_003]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPolicy_TC_07_005_004 PMApiTest.PmSetPolicy_TC_07_005_004
 * @subsection PmSetPolicy_TC_07_005_004_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPolicy_TC_07_005_004_tci Test Case ID
 * PmSetPolicy_S1_BN_TC_07_005_004
 * @subsection PmSetPolicy_TC_07_005_004_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSetPolicy_TC_07_005_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmSetPolicy_TC_07_005_004_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPolicy function in normal condition:
 * - Test07_005_004: set policy with OSAL_PM_POLICY_PG, applyImmediate is false, in REQUIRED state
 * @subsection PmSetPolicy_TC_07_005_004_tpts Test Procedure/Test Steps
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyImmediate is false
 * -# Wait power state to become OSAL_PM_STATE_CG by using R_OSAL_PmWaitForState.
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Wait power state to become OSAL_PM_STATE_CG by using R_OSAL_PmWaitForState
 * @subsection PmSetPolicy_TC_07_005_004_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_TIME
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK
 * @subsection PmSetPolicy_TC_07_005_004_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPolicy_TC_07_005_004
 */
// [PmSetPolicy_TC_07_005_004]
TEST_F(PMApiTest, PmSetPolicy_TC_07_005_004)
{
    bool applyImmediate = true;
    osal_milli_sec_t time_period_1000ms = 1000;

    // Test07_005_004
    // @brief: normal case: set policy with OSAL_PM_POLICY_PG, applyImmediate is false
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);
    applyImmediate = false;
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK) << "Test07_005_004 failed";
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle1, OSAL_PM_STATE_CG, time_period_1000ms), OSAL_RETURN_TIME) << "Test07_005_004 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_005_004 failed";
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle1, OSAL_PM_STATE_CG, time_period_1000ms), OSAL_RETURN_OK) << "Test07_005_004 failed";
}
// [PmSetPolicy_TC_07_005_004]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPolicy_TC_07_005_005 PMApiTest.PmSetPolicy_TC_07_005_005
 * @subsection PmSetPolicy_TC_07_005_005_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPolicy_TC_07_005_005_tci Test Case ID
 * PmSetPolicy_S1_N_TC_07_005_005
 * @subsection PmSetPolicy_TC_07_005_005_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSetPolicy_TC_07_005_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmSetPolicy_TC_07_005_005_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPolicy function in normal condition:
 * - Test07_005_005: set policy with OSAL_PM_POLICY_CG, applyImmediate is false, in REQUIRED state
 * @subsection PmSetPolicy_TC_07_005_005_tpts Test Procedure/Test Steps
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_CG by using R_OSAL_PmSetPolicy with applyImmediate is false
 * -# Wait power state to become OSAL_PM_STATE_CG by using R_OSAL_PmWaitForState
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Wait power state to become OSAL_PM_STATE_CG by using R_OSAL_PmWaitForState
 * @subsection PmSetPolicy_TC_07_005_005_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_TIME
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK
 * @subsection PmSetPolicy_TC_07_005_005_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPolicy_TC_07_005_005
 */
// [PmSetPolicy_TC_07_005_005]
TEST_F(PMApiTest, PmSetPolicy_TC_07_005_005)
{
    bool applyImmediate = true;
    osal_milli_sec_t time_period_1000ms = 1000;

    // Test07_005_005
    // @brief: normal case: set policy with OSAL_PM_POLICY_CG, applyImmediate is false
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);
    applyImmediate = false;
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_CG, applyImmediate), OSAL_RETURN_OK) << "Test07_005_005 failed";
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle1, OSAL_PM_STATE_CG, time_period_1000ms), OSAL_RETURN_TIME) << "Test07_005_005 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_005_005 failed";
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle1, OSAL_PM_STATE_CG, time_period_1000ms), OSAL_RETURN_OK) << "Test07_005_005 failed";
}
// [PmSetPolicy_TC_07_005_005]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPolicy_TC_07_005_006 PMApiTest.PmSetPolicy_TC_07_005_006
 * @subsection PmSetPolicy_TC_07_005_006_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPolicy_TC_07_005_006_tci Test Case ID
 * PmSetPolicy_S1_BN_TC_07_005_006
 * @subsection PmSetPolicy_TC_07_005_006_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSetPolicy_TC_07_005_006_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmSetPolicy_TC_07_005_006_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPolicy function in normal condition:
 * - Test07_005_006: set policy with OSAL_PM_POLICY_HP, applyImmediate is false, in REQUIRED state
 * @subsection PmSetPolicy_TC_07_005_006_tpts Test Procedure/Test Steps
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_HP by using R_OSAL_PmSetPolicy with applyImmediate is false
 * -# Wait power state to become OSAL_PM_STATE_ENABLED by using R_OSAL_PmWaitForState
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Wait power state to become OSAL_PM_STATE_ENABLED by using R_OSAL_PmWaitForState
 * @subsection PmSetPolicy_TC_07_005_006_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_TIME
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK
 * @subsection PmSetPolicy_TC_07_005_006_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPolicy_TC_07_005_006
 */
// [PmSetPolicy_TC_07_005_006]
TEST_F(PMApiTest, PmSetPolicy_TC_07_005_006)
{
    bool applyImmediate = true;
    osal_milli_sec_t time_period_1000ms = 1000;

    // Test07_005_006
    // @brief: normal case: set policy with OSAL_PM_POLICY_HP, applyImmediate is false
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);
    applyImmediate = false;
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_HP, applyImmediate), OSAL_RETURN_OK) << "Test07_005_006 failed";
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle1, OSAL_PM_STATE_ENABLED, time_period_1000ms), OSAL_RETURN_TIME) << "Test07_005_006 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_005_006 failed";
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle1, OSAL_PM_STATE_ENABLED, time_period_1000ms), OSAL_RETURN_OK) << "Test07_005_006 failed";
}
// [PmSetPolicy_TC_07_005_006]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPolicy_TC_07_005_007 PMApiTest.PmSetPolicy_TC_07_005_007
 * @subsection PmSetPolicy_TC_07_005_007_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPolicy_TC_07_005_007_tci Test Case ID
 * PmSetPolicy_S1_BN_TC_07_005_007
 * @subsection PmSetPolicy_TC_07_005_007_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSetPolicy_TC_07_005_007_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetPolicy_TC_07_005_007_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPolicy function in normal condition:
 * - Test07_005_007: set policy with OSAL_PM_POLICY_PG, applyImmediate is true, during RELEASED
 * @subsection PmSetPolicy_TC_07_005_007_tpts Test Procedure/Test Steps
 * -# Set power required state of the dependence device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Set power policy of the dependence device to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Set power required state of test target device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power policy by using R_OSAL_PmGetPolicy
 * @subsection PmSetPolicy_TC_07_005_007_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_OK.<br>
 * Value of "policy" is equal to OSAL_PM_POLICY_PG
 * @subsection PmSetPolicy_TC_07_005_007_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPolicy_TC_07_005_007
 */
// [PmSetPolicy_TC_07_005_007]
TEST_F(PMApiTest, PmSetPolicy_TC_07_005_007)
{
    bool applyImmediate = true;
    e_osal_pm_policy_t policy = OSAL_PM_POLICY_INVALID;

    // Test07_005_007
    // @brief: normal case: set policy with OSAL_PM_POLICY_PG, applyImmediate is true, during RELEASED
#if !defined(OSAL_SOC_S4) /* R-Car S4 doesn't have PG device */
    //Set policy OSAL_PM_POLICY_PG + OSAL_PM_REQUIRED_STATE_RELEASED for dependence device
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle_dependence, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle_dependence, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK) << "Test07_005_007 failed";
#endif
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK) << "Test07_005_007 failed";
    EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle, &policy), OSAL_RETURN_OK) << "Test07_005_007 failed";
    EXPECT_EQ(policy, OSAL_PM_POLICY_PG) << "Test07_005_007 failed";
}
// [PmSetPolicy_TC_07_005_007]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPolicy_TC_07_005_008 PMApiTest.PmSetPolicy_TC_07_005_008
 * @subsection PmSetPolicy_TC_07_005_008_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPolicy_TC_07_005_008_tci Test Case ID
 * PmSetPolicy_S1_N_TC_07_005_008
 * @subsection PmSetPolicy_TC_07_005_008_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSetPolicy_TC_07_005_008_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetPolicy_TC_07_005_008_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPolicy function in normal condition:
 * - Test07_005_008: set policy with OSAL_PM_POLICY_PG, applyImmediate is true, during RELEASED
 * @subsection PmSetPolicy_TC_07_005_008_tpts Test Procedure/Test Steps
 * -# Set power required state of test target device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_CG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power policy by using R_OSAL_PmGetPolicy
 * @subsection PmSetPolicy_TC_07_005_008_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_OK.<br>
 * Value of "policy" is equal to OSAL_PM_POLICY_CG
 * @subsection PmSetPolicy_TC_07_005_008_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPolicy_TC_07_005_008
 */
// [PmSetPolicy_TC_07_005_008]
TEST_F(PMApiTest, PmSetPolicy_TC_07_005_008)
{
    bool applyImmediate = true;
    e_osal_pm_policy_t policy = OSAL_PM_POLICY_INVALID;

    // Test07_005_008
    // @brief: normal case: set policy with OSAL_PM_POLICY_CG, applyImmediate is true, during RELEASED
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, OSAL_PM_POLICY_CG, applyImmediate), OSAL_RETURN_OK) << "Test07_005_008 failed";
    EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle, &policy), OSAL_RETURN_OK) << "Test07_005_008 failed";
    EXPECT_EQ(policy, OSAL_PM_POLICY_CG) << "Test07_005_008 failed";
}
// [PmSetPolicy_TC_07_005_008]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPolicy_TC_07_005_009 PMApiTest.PmSetPolicy_TC_07_005_009
 * @subsection PmSetPolicy_TC_07_005_009_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPolicy_TC_07_005_009_tci Test Case ID
 * PmSetPolicy_S1_BN_TC_07_005_009
 * @subsection PmSetPolicy_TC_07_005_009_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSetPolicy_TC_07_005_009_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetPolicy_TC_07_005_009_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPolicy function in normal condition:
 * - Test07_005_009: set policy with OSAL_PM_POLICY_HP, applyImmediate is true, during RELEASED
 * @subsection PmSetPolicy_TC_07_005_009_tpts Test Procedure/Test Steps
 * -# Set power required state of test target device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_HP by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power policy by using R_OSAL_PmGetPolicy
 * @subsection PmSetPolicy_TC_07_005_009_ereb Expected Result/Behavior
* -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_OK.<br>
 * Value of "policy" is equal to OSAL_PM_POLICY_HP
 * @subsection PmSetPolicy_TC_07_005_009_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPolicy_TC_07_005_009
 */
// [PmSetPolicy_TC_07_005_009]
TEST_F(PMApiTest, PmSetPolicy_TC_07_005_009)
{
    bool applyImmediate = true;
    e_osal_pm_policy_t policy = OSAL_PM_POLICY_INVALID;

    // Test07_005_009
    // @brief: normal case: set policy with OSAL_PM_POLICY_HP, applyImmediate is true, during RELEASED
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, OSAL_PM_POLICY_HP, applyImmediate), OSAL_RETURN_OK) << "Test07_005_009 failed";
    EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle, &policy), OSAL_RETURN_OK) << "Test07_005_009 failed";
    EXPECT_EQ(policy, OSAL_PM_POLICY_HP) << "Test07_005_009 failed";
}
// [PmSetPolicy_TC_07_005_009]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPolicy_TC_07_005_010 PMApiTest.PmSetPolicy_TC_07_005_010
 * @subsection PmSetPolicy_TC_07_005_010_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPolicy_TC_07_005_010_tci Test Case ID
 * PmSetPolicy_S1_BN_TC_07_005_010
 * @subsection PmSetPolicy_TC_07_005_010_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSetPolicy_TC_07_005_010_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmSetPolicy_TC_07_005_010_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPolicy function in normal condition:
 * - Test07_005_010: set policy with OSAL_PM_POLICY_PG, applyImmediate is false, during RELEASED
 * @subsection PmSetPolicy_TC_07_005_010_tpts Test Procedure/Test Steps
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyImmediate is false
 * -# Wait power state to become OSAL_PM_STATE_CG by using R_OSAL_PmWaitForState
 * @subsection PmSetPolicy_TC_07_005_010_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK.
 * @subsection PmSetPolicy_TC_07_005_010_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPolicy_TC_07_005_010
 */
// [PmSetPolicy_TC_07_005_010]
TEST_F(PMApiTest, PmSetPolicy_TC_07_005_010)
{
    bool applyImmediate = true;
    osal_milli_sec_t time_period_1000ms = 1000;

    // Test07_005_010
    // @brief: normal case: set policy with OSAL_PM_POLICY_PG, applyImmediate is false, during RELEASED
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK);
    applyImmediate = false;
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK) << "Test07_005_010 failed";
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle1, OSAL_PM_STATE_CG, time_period_1000ms), OSAL_RETURN_OK) << "Test07_005_010 failed";

}
// [PmSetPolicy_TC_07_005_010]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPolicy_TC_07_005_011 PMApiTest.PmSetPolicy_TC_07_005_011
 * @subsection PmSetPolicy_TC_07_005_011_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPolicy_TC_07_005_011_tci Test Case ID
 * PmSetPolicy_S1_N_TC_07_005_011
 * @subsection PmSetPolicy_TC_07_005_011_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSetPolicy_TC_07_005_011_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmSetPolicy_TC_07_005_011_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPolicy function in normal condition:
 * - Test07_005_011: set policy with OSAL_PM_POLICY_CG, applyImmediate is false, during RELEASED
 * @subsection PmSetPolicy_TC_07_005_011_tpts Test Procedure/Test Steps
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_CG by using R_OSAL_PmSetPolicy with applyImmediate is false
 * -# Wait power state to become OSAL_PM_STATE_CG by using R_OSAL_PmWaitForState
 * @subsection PmSetPolicy_TC_07_005_011_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK.
 * @subsection PmSetPolicy_TC_07_005_011_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPolicy_TC_07_005_011
 */
// [PmSetPolicy_TC_07_005_011]
TEST_F(PMApiTest, PmSetPolicy_TC_07_005_011)
{
    bool applyImmediate = true;
    osal_milli_sec_t time_period_1000ms = 1000;

    // Test07_005_011
    // @brief: normal case: set policy with OSAL_PM_POLICY_CG, applyImmediate is false, during RELEASED
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK);
    applyImmediate = false;
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_CG, applyImmediate), OSAL_RETURN_OK) << "Test07_005_011 failed";
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle1, OSAL_PM_STATE_CG, time_period_1000ms), OSAL_RETURN_OK) << "Test07_005_011 failed";
}
// [PmSetPolicy_TC_07_005_011]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPolicy_TC_07_005_012 PMApiTest.PmSetPolicy_TC_07_005_012
 * @subsection PmSetPolicy_TC_07_005_012_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPolicy_TC_07_005_012_tci Test Case ID
 * PmSetPolicy_S1_BN_TC_07_005_012
 * @subsection PmSetPolicy_TC_07_005_012_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSetPolicy_TC_07_005_012_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmSetPolicy_TC_07_005_012_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPolicy function in normal condition:
 * - Test07_005_012: set policy with OSAL_PM_POLICY_HP, applyImmediate is false, during RELEASED
 * @subsection PmSetPolicy_TC_07_005_012_tpts Test Procedure/Test Steps
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_HP by using R_OSAL_PmSetPolicy with applyImmediate is false
 * -# Wait power state to become OSAL_PM_STATE_ENABLED by using R_OSAL_PmWaitForState
 * @subsection PmSetPolicy_TC_07_005_012_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK.
 * @subsection PmSetPolicy_TC_07_005_012_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPolicy_TC_07_005_012
 */
// [PmSetPolicy_TC_07_005_012]
TEST_F(PMApiTest, PmSetPolicy_TC_07_005_012)
{
    bool applyImmediate = true;
    osal_milli_sec_t time_period_1000ms = 1000;

    // Test07_005_012
    // @brief: normal case: set policy with OSAL_PM_POLICY_HP, applyImmediate is false, during RELEASED
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK);
    applyImmediate = false;
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_HP, applyImmediate), OSAL_RETURN_OK) << "Test07_005_012 failed";
    EXPECT_EQ(R_OSAL_PmWaitForState(device_handle1, OSAL_PM_STATE_ENABLED, time_period_1000ms), OSAL_RETURN_OK) << "Test07_005_012 failed";
}
// [PmSetPolicy_TC_07_005_012]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPolicy_TC_07_005_013 PMApiTest.PmSetPolicy_TC_07_005_013
 * @subsection PmSetPolicy_TC_07_005_013_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPolicy_TC_07_005_013_tci Test Case ID
 * PmSetPolicy_S0_A_TC_07_005_013
 * @subsection PmSetPolicy_TC_07_005_013_rr Related Requirement(s)
 * \#199151, \#239139
 * @subsection PmSetPolicy_TC_07_005_013_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection PmSetPolicy_TC_07_005_013_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPolicy function in abnormal condition:
 * - Test07_005_013: set policy with device handle is NULL
 * @subsection PmSetPolicy_TC_07_005_013_tpts Test Procedure/Test Steps
 * -# Set "policy" to OSAL_PM_POLICY_INVALID.<br>
 * Set power policy by using R_OSAL_PmSetPolicy with the arguments as below:
 *      - Device handle is NULL
 *      - OSAL_PM_POLICY_HP
 *      - true
 * @subsection PmSetPolicy_TC_07_005_013_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_HANDLE
 * @subsection PmSetPolicy_TC_07_005_013_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPolicy_TC_07_005_013
 */
// [PmSetPolicy_TC_07_005_013]
TEST_F(PMApiTest, PmSetPolicy_TC_07_005_013)
{
    bool applyImmediate = true;

    // @brief: abnormal case: device handle is NULL
    // Test07_005_013
    EXPECT_EQ(R_OSAL_PmSetPolicy(OSAL_DEVICE_HANDLE_INVALID, OSAL_PM_POLICY_HP, applyImmediate), OSAL_RETURN_HANDLE) << "Test07_005_013 failed";
}
// [PmSetPolicy_TC_07_005_013]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPolicy_TC_07_005_014 PMApiTest.PmSetPolicy_TC_07_005_014
 * @subsection PmSetPolicy_TC_07_005_014_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPolicy_TC_07_005_014_tci Test Case ID
 * PmSetPolicy_S0_A_TC_07_005_014
 * @subsection PmSetPolicy_TC_07_005_014_rr Related Requirement(s)
 * \#239139
 * @subsection PmSetPolicy_TC_07_005_014_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetPolicy_TC_07_005_014_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPolicy function in abnormal condition:
 * - Test07_005_014: set policy with device handle is NULL
 * @subsection PmSetPolicy_TC_07_005_014_tpts Test Procedure/Test Steps
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * -# Set power policy by using R_OSAL_PmSetPolicy with the arguments as below:
 *      - Device handle of test target device which has already been closed
 *      - OSAL_PM_POLICY_HP
 *      - true
 * -# Re-open test target device by using R_OSAL_IoDeviceOpen for the next test cases
 * @subsection PmSetPolicy_TC_07_005_014_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_HANDLE
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not equal to NULL_PTR
 * @subsection PmSetPolicy_TC_07_005_014_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPolicy_TC_07_005_014
 */
// [PmSetPolicy_TC_07_005_014]
TEST_F(PMApiTest, PmSetPolicy_TC_07_005_014)
{
    bool applyImmediate = true;

    // Test07_005_014
    // @brief: abnormal case: device handle is closed
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, OSAL_PM_POLICY_HP, applyImmediate), OSAL_RETURN_HANDLE) << "Test07_005_014 failed";
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_PM_DEVICE_2, &device_handle), OSAL_RETURN_OK); // re-open for the next test case
    EXPECT_NE(device_handle, NULL_PTR);
}
// [PmSetPolicy_TC_07_005_014]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPolicy_TC_07_005_015 PMApiTest.PmSetPolicy_TC_07_005_015
 * @subsection PmSetPolicy_TC_07_005_015_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPolicy_TC_07_005_015_tci Test Case ID
 * PmSetPolicy_S0_BA_TC_07_005_015
 * @subsection PmSetPolicy_TC_07_005_015_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSetPolicy_TC_07_005_015_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetPolicy_TC_07_005_015_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPolicy function in abnormal condition:
 * - Test07_005_015: set power policy with policy is OSAL_PM_POLICY_INVALID
 * @subsection PmSetPolicy_TC_07_005_015_tpts Test Procedure/Test Steps
 * -# Set power policy by using R_OSAL_PmSetPolicy with the arguments as below:
 *      - Device handle of test target device
 *      - OSAL_PM_POLICY_INVALID
 *      - true
 * @subsection PmSetPolicy_TC_07_005_015_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_PAR
 * @subsection PmSetPolicy_TC_07_005_015_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPolicy_TC_07_005_015
 */
// [PmSetPolicy_TC_07_005_015]
TEST_F(PMApiTest, PmSetPolicy_TC_07_005_015)
{
    bool applyImmediate = true;

    // @brief: abnormal case: policy in R_OSAL_PmSetPolicy is OSAL_PM_POLICY_INVALID
    // Test07_005_015
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, OSAL_PM_POLICY_INVALID, applyImmediate), OSAL_RETURN_PAR) << "Test07_005_015 failed";

}
// [PmSetPolicy_TC_07_005_015]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPolicy_TC_07_005_016 PMApiTest.PmSetPolicy_TC_07_005_016
 * @subsection PmSetPolicy_TC_07_005_016_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPolicy_TC_07_005_016_tci Test Case ID
 * PmSetPolicy_S0_BA_TC_07_005_016
 * @subsection PmSetPolicy_TC_07_005_016_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSetPolicy_TC_07_005_016_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetPolicy_TC_07_005_016_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPolicy function in abnormal condition:
 * - Test07_005_016: set power policy with policy is OSAL_PM_POLICY_HP + 1
 * @subsection PmSetPolicy_TC_07_005_016_tpts Test Procedure/Test Steps
 * -# Set power policy by using R_OSAL_PmSetPolicy with the arguments as below:
 *      - Device handle of test target device
 *      - OSAL_PM_POLICY_HP + 1
 *      - true
 * @subsection PmSetPolicy_TC_07_005_016_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_PAR
 * @subsection PmSetPolicy_TC_07_005_016_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPolicy_TC_07_005_016
 */
// [PmSetPolicy_TC_07_005_016]
TEST_F(PMApiTest, PmSetPolicy_TC_07_005_016)
{
    bool applyImmediate = true;

    // @brief: abnormal case: policy in R_OSAL_PmSetPolicy is OSAL_PM_POLICY_HP + 1
    // Test07_005_016
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, e_osal_pm_policy_t (OSAL_PM_POLICY_HP + 1), applyImmediate), OSAL_RETURN_PAR) << "Test07_005_016 failed";

}
// [PmSetPolicy_TC_07_005_016]

/**
 * @page PM_TEST Power API Test
 * @section OsalApiStateTest_PmSetPolicy_TC_07_005_017 OsalApiStateTest.PmSetPolicy_TC_07_005_017
 * @subsection PmSetPolicy_TC_07_005_017_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection PmSetPolicy_TC_07_005_017_tci Test Case ID
 * PmSetPolicy_S2_A_TC_07_005_017
 * @subsection PmSetPolicy_TC_07_005_017_rr Related Requirement(s)
 * \#239139
 * @subsection PmSetPolicy_TC_07_005_017_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection PmSetPolicy_TC_07_005_017_tcs Test Case Summary
 * This test shall verify the functionality of the PM APIs in invalid state transition:
 * - Test07_005_017: Checking the state of R_OSAL_PmSetPolicy
 * @subsection PmSetPolicy_TC_07_005_017_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_PmSetPolicy
 * @subsection PmSetPolicy_TC_07_005_017_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_STATE.
 * @subsection PmSetPolicy_TC_07_005_017_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPolicy_TC_07_005_017
 */
// [PmSetPolicy_TC_07_005_017]
TEST_F(OsalApiStateTest, PmSetPolicy_TC_07_005_017)
{
    bool applyImmediate = true;
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;

    // Test07_005_017
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_STATE) << "Test07_005_017 failed";
}
// [PmSetPolicy_TC_07_005_017]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPolicy_TC_07_005_018 PMApiTest.PmSetPolicy_TC_07_005_018
 * @subsection PmSetPolicy_TC_07_005_018_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPolicy_TC_07_005_018_tci Test Case ID
 * PmSetPolicy_S0_A_TC_07_005_018
 * @subsection PmSetPolicy_TC_07_005_018_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSetPolicy_TC_07_005_018_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetPolicy_TC_07_005_018_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPolicy function in abnormal condition:
 * - Test07_005_018: The device assigned to handle is not supported by Power Manager
 * @subsection PmSetPolicy_TC_07_005_018_tpts Test Procedure/Test Steps
 * -# Set power policy by using R_OSAL_PmSetPolicy with the unsupport power device handle
 * @subsection PmSetPolicy_TC_07_005_018_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_UNSUPPORTED_OPERATION
 * @subsection PmSetPolicy_TC_07_005_018_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPolicy_TC_07_005_018
 */
// [PmSetPolicy_TC_07_005_018]
TEST_F(PMApiTest, PmSetPolicy_TC_07_005_018)
{
    bool applyImmediate = true;

    // @brief: abnormal case: The device assigned to handle is not supported by Power Manager
    // Test07_005_018
    EXPECT_EQ(R_OSAL_PmSetPolicy(unsupport_power_dev_handle, OSAL_PM_POLICY_HP, applyImmediate), OSAL_RETURN_UNSUPPORTED_OPERATION) << "Test07_005_018 failed";
}
// [PmSetPolicy_TC_07_005_018]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPolicy_TC_07_005_019 PMApiTest.PmSetPolicy_TC_07_005_019
 * @subsection PmSetPolicy_TC_07_005_019_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPolicy_TC_07_005_019_tci Test Case ID
 * PmSetPolicy_S0_A_TC_07_005_019
 * @subsection PmSetPolicy_TC_07_005_019_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSetPolicy_TC_07_005_019_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetPolicy_TC_07_005_019_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPolicy function in abnormal condition:
 * - Test07_005_019: The device assigned to handle is not supported by Power Manager
 * @subsection PmSetPolicy_TC_07_005_019_tpts Test Procedure/Test Steps
 * -# Set power policy by using R_OSAL_PmSetPolicy with the unsupport clock device handle
 * @subsection PmSetPolicy_TC_07_005_019_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_UNSUPPORTED_OPERATION
 * @subsection PmSetPolicy_TC_07_005_019_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPolicy_TC_07_005_019
 */
// [PmSetPolicy_TC_07_005_019]
TEST_F(PMApiTest, PmSetPolicy_TC_07_005_019)
{
    bool applyImmediate = true;

    // @brief: abnormal case: The device assigned to handle is not supported by Power Manager
    // Test07_005_019
    EXPECT_EQ(R_OSAL_PmSetPolicy(unsupport_clock_dev_handle, OSAL_PM_POLICY_HP, applyImmediate), OSAL_RETURN_UNSUPPORTED_OPERATION) << "Test07_005_019 failed";
}
// [PmSetPolicy_TC_07_005_019]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPolicy_TC_07_005_020 PMApiTest.PmSetPolicy_TC_07_005_020
 * @subsection PmSetPolicy_TC_07_005_020_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPolicy_TC_07_005_020_tci Test Case ID
 * PmSetPolicy_S0_BA_TC_07_005_020
 * @subsection PmSetPolicy_TC_07_005_020_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSetPolicy_TC_07_005_020_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetPolicy_TC_07_005_020_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPolicy function in abnormal condition:
 * - Test07_005_020: Using R_OSAL_PmSetPolicy with the policy is (OSAL_PM_POLICY_INVALID - 1)
 * @subsection PmSetPolicy_TC_07_005_020_tpts Test Procedure/Test Steps
 * -# Calling R_OSAL_PmSetPolicy with the policy is (OSAL_PM_POLICY_INVALID - 1)
 * @subsection PmSetPolicy_TC_07_005_020_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_PAR
 * @subsection PmSetPolicy_TC_07_005_020_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPolicy_TC_07_005_020
 */
// [PmSetPolicy_TC_07_005_020]
TEST_F(PMApiTest, PmSetPolicy_TC_07_005_020)
{
    bool applyImmediate = true;

    // @brief: boundary abnormal case: policy in R_OSAL_PmSetPolicy is OSAL_PM_POLICY_INVALID - 1
    // Test07_005_020
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, e_osal_pm_policy_t(OSAL_PM_POLICY_INVALID - 1), applyImmediate), OSAL_RETURN_PAR) << "Test07_005_020 failed";
}
// [PmSetPolicy_TC_07_005_020]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetPolicy_TC_07_006_001 PMApiTest.PmGetPolicy_TC_07_006_001
 * @subsection PmGetPolicy_TC_07_006_001_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetPolicy_TC_07_006_001_tci Test Case ID
 * PmGetPolicy_S1_N_TC_07_006_001
 * @subsection PmGetPolicy_TC_07_006_001_rr Related Requirement(s)
 * \#239140
 * @subsection PmGetPolicy_TC_07_006_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmGetPolicy_TC_07_006_001_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetPolicy function in abnormal condition:
 * - Test07_006_001: get policy with OSAL_PM_POLICY_PG
 * @subsection PmGetPolicy_TC_07_006_001_tpts Test Procedure/Test Steps
 * -# Set power required state of test target device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set power required state of the dependence device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set power policy of the dependence device to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Set power required state of the dependence device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Set power required state of test target device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Get current power policy by using R_OSAL_PmGetPolicy
 * @subsection PmGetPolicy_TC_07_006_001_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_OK.<br>
 * Value of "policy" is equal to OSAL_PM_POLICY_PG
 * @subsection PmGetPolicy_TC_07_006_001_testcase Test Case Example
 * @snippet test_pm.cpp PmGetPolicy_TC_07_006_001
 */
// [PmGetPolicy_TC_07_006_001]
TEST_F(PMApiTest, PmGetPolicy_TC_07_006_001)
{
    e_osal_pm_policy_t policy = OSAL_PM_POLICY_INVALID;
    bool applyImmediate = true;

    // @brief: normal case: get policy with OSAL_PM_POLICY_PG
    // Test07_006_001
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK); 
#if !defined(OSAL_SOC_S4) /* R-Car S4 doesn't have PG device */
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle_dependence, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK); 
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle_dependence, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK) << "Test07_006_001 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle_dependence, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_006_001 failed";
#endif
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK) << "Test07_006_001 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_006_001 failed";
    EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle, &policy), OSAL_RETURN_OK) << "Test07_006_001 failed";
    EXPECT_EQ(policy, OSAL_PM_POLICY_PG) << "Test07_006_001 failed";
}
// [PmGetPolicy_TC_07_006_001]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetPolicy_TC_07_006_002 PMApiTest.PmGetPolicy_TC_07_006_002
 * @subsection PmGetPolicy_TC_07_006_002_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetPolicy_TC_07_006_002_tci Test Case ID
 * PmGetPolicy_S1_N_TC_07_006_002
 * @subsection PmGetPolicy_TC_07_006_002_rr Related Requirement(s)
 * \#239140
 * @subsection PmGetPolicy_TC_07_006_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmGetPolicy_TC_07_006_002_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetPolicy function in normal condition:
 * - Test07_006_002: get policy with OSAL_PM_POLICY_CG
 * @subsection PmGetPolicy_TC_07_006_002_tpts Test Procedure/Test Steps
 * -# Set power required state of test target device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_CG by using R_OSAL_PmSetPolicy applyImmediate is true
 * -# Set power required state of test target device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Get current power policy by using R_OSAL_PmGetPolicy
 * @subsection PmGetPolicy_TC_07_006_002_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_OK.<br>
 * Value of "policy" is equal to OSAL_PM_POLICY_CG
 * @subsection PmGetPolicy_TC_07_006_002_testcase Test Case Example
 * @snippet test_pm.cpp PmGetPolicy_TC_07_006_002
 */
// [PmGetPolicy_TC_07_006_002]
TEST_F(PMApiTest, PmGetPolicy_TC_07_006_002)
{
    e_osal_pm_policy_t policy = OSAL_PM_POLICY_INVALID;
    bool applyImmediate = true;

    // @brief: normal case: get policy with OSAL_PM_POLICY_CG
    // Test07_006_002
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, OSAL_PM_POLICY_CG, applyImmediate), OSAL_RETURN_OK) << "Test07_006_002 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_006_002 failed";
    EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle, &policy), OSAL_RETURN_OK) << "Test07_006_002 failed";
    EXPECT_EQ(policy, OSAL_PM_POLICY_CG) << "Test07_006_002 failed";
}
// [PmGetPolicy_TC_07_006_002]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetPolicy_TC_07_006_003 PMApiTest.PmGetPolicy_TC_07_006_003
 * @subsection PmGetPolicy_TC_07_006_003_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetPolicy_TC_07_006_003_tci Test Case ID
 * PmGetPolicy_S1_N_TC_07_006_003
 * @subsection PmGetPolicy_TC_07_006_003_rr Related Requirement(s)
 * \#239140
 * @subsection PmGetPolicy_TC_07_006_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmGetPolicy_TC_07_006_003_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetPolicy function in normal condition:
 * - Test07_006_003: get policy with OSAL_PM_POLICY_HP.
 * @subsection PmGetPolicy_TC_07_006_003_tpts Test Procedure/Test Steps
 * -# Set power required state of test target device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_HP by using R_OSAL_PmSetPolicy applyImmediate is true
 * -# Set power required state of test target device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Get current power policy by using R_OSAL_PmGetPolicy
 * @subsection PmGetPolicy_TC_07_006_003_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_OK.<br>
 * Value of "policy" is equal to OSAL_PM_POLICY_HP
 * @subsection PmGetPolicy_TC_07_006_003_testcase Test Case Example
 * @snippet test_pm.cpp PmGetPolicy_TC_07_006_003
 */
// [PmGetPolicy_TC_07_006_003]
TEST_F(PMApiTest, PmGetPolicy_TC_07_006_003)
{
    e_osal_pm_policy_t policy = OSAL_PM_POLICY_INVALID;
    bool applyImmediate = true;

    // @brief: normal case: get policy with OSAL_PM_POLICY_HP
    // Test07_006_003
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, OSAL_PM_POLICY_HP, applyImmediate), OSAL_RETURN_OK) << "Test07_006_003 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_006_003 failed";
    EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle, &policy), OSAL_RETURN_OK) << "Test07_006_003 failed";
    EXPECT_EQ(policy, OSAL_PM_POLICY_HP) << "Test07_006_003 failed";
}
// [PmGetPolicy_TC_07_006_003]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetPolicy_TC_07_006_004 PMApiTest.PmGetPolicy_TC_07_006_004
 * @subsection PmGetPolicy_TC_07_006_004_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetPolicy_TC_07_006_004_tci Test Case ID
 * PmGetPolicy_S0_A_TC_07_006_004
 * @subsection PmGetPolicy_TC_07_006_004_rr Related Requirement(s)
 * \#199151, \#239140
 * @subsection PmGetPolicy_TC_07_006_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection PmGetPolicy_TC_07_006_004_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetPolicy function in abnormal condition:
 * - Test07_006_004: get policy with device handle is NULL
 * @subsection PmGetPolicy_TC_07_006_004_tpts Test Procedure/Test Steps
 * -# Get power policy by using R_OSAL_PmGetPolicy with the arguments as below:
 *      - Device handle is NULL
 *      - Address of "policy"
 * @subsection PmGetPolicy_TC_07_006_004_ereb Expected Result/Behavior
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_HANDLE.<br>
 * Value of "policy" is equal to OSAL_PM_POLICY_INVALID
 * @subsection PmGetPolicy_TC_07_006_004_testcase Test Case Example
 * @snippet test_pm.cpp PmGetPolicy_TC_07_006_004
 */
// [PmGetPolicy_TC_07_006_004]
TEST_F(PMApiTest, PmGetPolicy_TC_07_006_004)
{
    e_osal_pm_policy_t policy = OSAL_PM_POLICY_INVALID;

    // @brief: abnormal case: device handle is NULL
    // Test07_006_004
    EXPECT_EQ(R_OSAL_PmGetPolicy(OSAL_DEVICE_HANDLE_INVALID, &policy), OSAL_RETURN_HANDLE) << "Test07_006_004 failed";
    EXPECT_EQ(policy, OSAL_PM_POLICY_INVALID) << "Test07_006_004 failed";
}
// [PmGetPolicy_TC_07_006_004]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetPolicy_TC_07_006_005 PMApiTest.PmGetPolicy_TC_07_006_005
 * @subsection PmGetPolicy_TC_07_006_005_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetPolicy_TC_07_006_005_tci Test Case ID
 * PmGetPolicy_S0_A_TC_07_006_005
 * @subsection PmGetPolicy_TC_07_006_005_rr Related Requirement(s)
 * \#239140
 * @subsection PmGetPolicy_TC_07_006_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmGetPolicy_TC_07_006_005_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetPolicy function in abnormal condition:
 * - Test07_006_005: get policy with device handle is closed
 * @subsection PmGetPolicy_TC_07_006_005_tpts Test Procedure/Test Steps
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * -# Get power policy by using R_OSAL_PmGetPolicy with the arguments as below:
 *      - Device handle of test target device which has already been closed
 *      - Address of "policy"
 * -# Re-open test target device by using R_OSAL_IoDeviceOpen for the next test cases
 * @subsection PmGetPolicy_TC_07_006_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_HANDLE
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not equal to NULL_PTR
 * @subsection PmGetPolicy_TC_07_006_005_testcase Test Case Example
 * @snippet test_pm.cpp PmGetPolicy_TC_07_006_005
 */
// [PmGetPolicy_TC_07_006_005]
TEST_F(PMApiTest, PmGetPolicy_TC_07_006_005)
{
    bool applyImmediate = true;
    e_osal_pm_policy_t policy = OSAL_PM_POLICY_INVALID;

    // Test07_006_005
    // @brief: abnormal case: device handle is closed
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle, &policy), OSAL_RETURN_HANDLE) << "Test07_006_005 failed";
    EXPECT_EQ(policy, OSAL_PM_POLICY_INVALID) << "Test07_006_005 failed";
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_PM_DEVICE_2, &device_handle), OSAL_RETURN_OK); // re-open for the next test case
    EXPECT_NE(device_handle, NULL_PTR);
}
// [PmGetPolicy_TC_07_006_005]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetPolicy_TC_07_006_006 PMApiTest.PmGetPolicy_TC_07_006_006
 * @subsection PmGetPolicy_TC_07_006_006_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetPolicy_TC_07_006_006_tci Test Case ID
 * PmGetPolicy_S0_A_TC_07_006_006
 * @subsection PmGetPolicy_TC_07_006_006_rr Related Requirement(s)
 * \#239140
 * @subsection PmGetPolicy_TC_07_006_006_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmGetPolicy_TC_07_006_006_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetPolicy function in abnormal condition:
 * - Test07_006_006: get power policy with p_policy is NULL
 * @subsection PmGetPolicy_TC_07_006_006_tpts Test Procedure/Test Steps
 * -# Get power policy by using R_OSAL_PmGetPolicy with the arguments as below:
 *      - Device handle of test target device
 *      - NULL
 * @subsection PmGetPolicy_TC_07_006_006_ereb Expected Result/Behavior
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_PAR
 * @subsection PmGetPolicy_TC_07_006_006_testcase Test Case Example
 * @snippet test_pm.cpp PmGetPolicy_TC_07_006_006
 */
// [PmGetPolicy_TC_07_006_006]
TEST_F(PMApiTest, PmGetPolicy_TC_07_006_006)
{
    e_osal_pm_policy_t * p_returned_policy = NULL;

    // Test07_006_006
    // @brief: abnormal case: p_policy in R_OSAL_PmGetPolicy is NULL
    EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle, p_returned_policy), OSAL_RETURN_PAR) << "Test07_006_006 failed";

}
// [PmGetPolicy_TC_07_006_006]

/**
 * @page PM_TEST Power API Test
 * @section OsalApiStateTest_PmGetPolicy_TC_07_006_007 OsalApiStateTest.PmGetPolicy_TC_07_006_007
 * @subsection PmGetPolicy_TC_07_006_007_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection PmGetPolicy_TC_07_006_007_tci Test Case ID
 * PmGetPolicy_S2_A_TC_07_006_007
 * @subsection PmGetPolicy_TC_07_006_007_rr Related Requirement(s)
 * \#239140
 * @subsection PmGetPolicy_TC_07_006_007_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection PmGetPolicy_TC_07_006_007_tcs Test Case Summary
 * This test shall verify the functionality of the PM APIs in invalid state transition:
 * - Test07_006_007: Checking the state of R_OSAL_PmGetPolicy
 * @subsection PmGetPolicy_TC_07_006_007_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_PmGetPolicy
 * @subsection PmGetPolicy_TC_07_006_007_ereb Expected Result/Behavior
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_STATE.<br>
 * The returned policy is OSAL_PM_POLICY_INVALID
 * @subsection PmGetPolicy_TC_07_006_007_testcase Test Case Example
 * @snippet test_pm.cpp PmGetPolicy_TC_07_006_007
 */
// [PmGetPolicy_TC_07_006_007]
TEST_F(OsalApiStateTest, PmGetPolicy_TC_07_006_007)
{
    e_osal_pm_policy_t policy = OSAL_PM_POLICY_INVALID;
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;

    // Test07_006_007
    EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle, &policy), OSAL_RETURN_STATE) << "Test07_006_007 failed";
    EXPECT_EQ(policy, OSAL_PM_POLICY_INVALID) << "Test07_006_007 failed";
}
// [PmGetPolicy_TC_07_006_007]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetPolicy_TC_07_006_008 PMApiTest.PmGetPolicy_TC_07_006_008
 * @subsection PmGetPolicy_TC_07_006_008_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetPolicy_TC_07_006_008_tci Test Case ID
 * PmGetPolicy_S0_A_TC_07_006_008
 * @subsection PmGetPolicy_TC_07_006_008_rr Related Requirement(s)
 * \#239140
 * @subsection PmGetPolicy_TC_07_006_008_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmGetPolicy_TC_07_006_008_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetPolicy function in abnormal condition:
 * - Test07_006_008: The device assigned to handle is not supported by Power Manager
 * @subsection PmGetPolicy_TC_07_006_008_tpts Test Procedure/Test Steps
 * -# Get power policy by using R_OSAL_PmGetPolicy with the unsupport power device handle
 * @subsection PmGetPolicy_TC_07_006_008_ereb Expected Result/Behavior
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_UNSUPPORTED_OPERATION
 * @subsection PmGetPolicy_TC_07_006_008_testcase Test Case Example
 * @snippet test_pm.cpp PmGetPolicy_TC_07_006_008
 */
// [PmGetPolicy_TC_07_006_008]
TEST_F(PMApiTest, PmGetPolicy_TC_07_006_008)
{
    e_osal_pm_policy_t returned_policy;

    // Test07_006_008
    // @brief: abnormal case: The device assigned to handle is not supported by Power Manager
    EXPECT_EQ(R_OSAL_PmGetPolicy(unsupport_power_dev_handle, &returned_policy), OSAL_RETURN_UNSUPPORTED_OPERATION) << "Test07_006_008 failed";
}
// [PmGetPolicy_TC_07_006_008]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetPolicy_TC_07_006_009 PMApiTest.PmGetPolicy_TC_07_006_009
 * @subsection PmGetPolicy_TC_07_006_009_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetPolicy_TC_07_006_009_tci Test Case ID
 * PmGetPolicy_S0_A_TC_07_006_009
 * @subsection PmGetPolicy_TC_07_006_009_rr Related Requirement(s)
 * \#239140
 * @subsection PmGetPolicy_TC_07_006_009_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmGetPolicy_TC_07_006_009_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetPolicy function in abnormal condition:
 * - Test07_006_009: The device assigned to handle is not supported by Power Manager
 * @subsection PmGetPolicy_TC_07_006_009_tpts Test Procedure/Test Steps
 * -# Get power policy by using R_OSAL_PmGetPolicy with the unsupport clock device handle
 * @subsection PmGetPolicy_TC_07_006_009_ereb Expected Result/Behavior
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_UNSUPPORTED_OPERATION
 * @subsection PmGetPolicy_TC_07_006_009_testcase Test Case Example
 * @snippet test_pm.cpp PmGetPolicy_TC_07_006_009
 */
// [PmGetPolicy_TC_07_006_009]
TEST_F(PMApiTest, PmGetPolicy_TC_07_006_009)
{
    e_osal_pm_policy_t returned_policy;

    // Test07_006_009
    // @brief: abnormal case: The device assigned to handle is not supported by Power Manager
    EXPECT_EQ(R_OSAL_PmGetPolicy(unsupport_clock_dev_handle, &returned_policy), OSAL_RETURN_UNSUPPORTED_OPERATION) << "Test07_006_009 failed";
}
// [PmGetPolicy_TC_07_006_009]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetResetState_TC_07_007_001 PMApiTest.PmSetResetState_TC_07_007_001
 * @subsection PmSetResetState_TC_07_007_001_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetResetState_TC_07_007_001_tci Test Case ID
 * PmSetResetState_S1_BN_TC_07_007_001
 * @subsection PmSetResetState_TC_07_007_001_rr Related Requirement(s)
 * \#239133, \#239141
 * @subsection PmSetResetState_TC_07_007_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetResetState_TC_07_007_001_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetResetState function in normal condition:
 * - Pre-process: set required state to OSAL_PM_REQUIRED_STATE_REQUIRED.
 * - Test07_007_001: set reset state with OSAL_PM_RESET_STATE_APPLIED
 * @subsection PmSetResetState_TC_07_007_001_tpts Test Procedure/Test Steps
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set reset state to OSAL_PM_RESET_STATE_APPLIED by using R_OSAL_PmSetResetState
 * -# Get reset state by using R_OSAL_PmGetResetState
 * -# Set the required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * @subsection PmSetResetState_TC_07_007_001_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetResetState return OSAL_RETURN_OK.<br>
 * Value of "reset" is equal to OSAL_PM_RESET_STATE_APPLIED
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * @subsection PmSetResetState_TC_07_007_001_testcase Test Case Example
 * @snippet test_pm.cpp PmSetResetState_TC_07_007_001
 */
// [PmSetResetState_TC_07_007_001]
TEST_F(PMApiTest, PmSetResetState_TC_07_007_001)
{
    e_osal_pm_reset_state_t reset = OSAL_PM_RESET_STATE_INVALID;
    osal_device_handle_t dev = OSAL_DEVICE_HANDLE_INVALID;
    bool applyImmediate = true;

    // pre-process: set required state to OSAL_PM_REQUIRED_STATE_REQUIRED.
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);

    // @brief: normal case: set reset state with OSAL_PM_RESET_STATE_APPLIED
    // Test07_007_001
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle, OSAL_PM_RESET_STATE_APPLIED), OSAL_RETURN_OK) << "Test07_007_001 failed";
    R_OSAL_ThreadSleepForTimePeriod(10);
    EXPECT_EQ(R_OSAL_PmGetResetState(device_handle, &reset), OSAL_RETURN_OK) << "Test07_007_001 failed";
    EXPECT_EQ(reset, OSAL_PM_RESET_STATE_APPLIED) << "Test07_007_001 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, false), OSAL_RETURN_OK);
}
// [PmSetResetState_TC_07_007_001]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetResetState_TC_07_007_002 PMApiTest.PmSetResetState_TC_07_007_002
 * @subsection PmSetResetState_TC_07_007_002_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetResetState_TC_07_007_002_tci Test Case ID
 * PmSetResetState_S1_BN_TC_07_007_002
 * @subsection PmSetResetState_TC_07_007_002_rr Related Requirement(s)
 * \#239133, \#239141
 * @subsection PmSetResetState_TC_07_007_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetResetState_TC_07_007_002_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetResetState function in normal condition:
 * - Pre-process: set required state to OSAL_PM_REQUIRED_STATE_REQUIRED.
 * - Test07_007_002: set reset state with OSAL_PM_RESET_STATE_RELEASED
 * @subsection PmSetResetState_TC_07_007_002_tpts Test Procedure/Test Steps
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set reset state to OSAL_PM_RESET_STATE_RELEASED by using R_OSAL_PmSetResetState
 * -# Get reset state by using R_OSAL_PmGetResetState
 * @subsection PmSetResetState_TC_07_007_002_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetResetState return OSAL_RETURN_OK.<br>
 * Value of "reset" is equal to OSAL_PM_RESET_STATE_RELEASED
 * @subsection PmSetResetState_TC_07_007_002_testcase Test Case Example
 * @snippet test_pm.cpp PmSetResetState_TC_07_007_002
 */
// [PmSetResetState_TC_07_007_002]
TEST_F(PMApiTest, PmSetResetState_TC_07_007_002)
{
    bool applyImmediate = true;
    e_osal_pm_reset_state_t reset = OSAL_PM_RESET_STATE_INVALID;

    // pre-process: set required state to OSAL_PM_REQUIRED_STATE_REQUIRED.
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);

    // @brief: normal case: set reset state with OSAL_PM_RESET_STATE_RELEASED
    // Test07_007_002
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle, OSAL_PM_RESET_STATE_RELEASED), OSAL_RETURN_OK) << "Test07_007_002 failed";
    R_OSAL_ThreadSleepForTimePeriod(10);
    EXPECT_EQ(R_OSAL_PmGetResetState(device_handle, &reset), OSAL_RETURN_OK) << "Test07_007_002 failed";
    EXPECT_EQ(reset, OSAL_PM_RESET_STATE_RELEASED) << "Test07_007_002 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, false), OSAL_RETURN_OK);
}
// [PmSetResetState_TC_07_007_002]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetResetState_TC_07_007_003 PMApiTest.PmSetResetState_TC_07_007_003
 * @subsection PmSetResetState_TC_07_007_003_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetResetState_TC_07_007_003_tci Test Case ID
 * PmSetResetState_S0_A_TC_07_007_003
 * @subsection PmSetResetState_TC_07_007_003_rr Related Requirement(s)
 * \#199151, \#239141
 * @subsection PmSetResetState_TC_07_007_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection PmSetResetState_TC_07_007_003_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetResetState function in abnormal condition:
 * - Test07_007_003: set reset state with device handle is NULL
 * @subsection PmSetResetState_TC_07_007_003_tpts Test Procedure/Test Steps
 * -# Set "reset" to OSAL_PM_RESET_STATE_INVALID.<br>
 * Set reset state by using R_OSAL_PmSetResetState with the arguments as below:
 *      - Device handle is NULL
 *      - OSAL_PM_RESET_STATE_RELEASED
 * @subsection PmSetResetState_TC_07_007_003_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_HANDLE
 * @subsection PmSetResetState_TC_07_007_003_testcase Test Case Example
 * @snippet test_pm.cpp PmSetResetState_TC_07_007_003
 */
// [PmSetResetState_TC_07_007_003]
TEST_F(PMApiTest, PmSetResetState_TC_07_007_003)
{
    bool applyImmediate = true;

    // @brief: abnormal case: device handle is NULL
    // Test07_007_003
    EXPECT_EQ(R_OSAL_PmSetResetState(OSAL_DEVICE_HANDLE_INVALID, OSAL_PM_RESET_STATE_RELEASED), OSAL_RETURN_HANDLE) << "Test07_007_003 failed";
}
// [PmSetResetState_TC_07_007_003]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetResetState_TC_07_007_004 PMApiTest.PmSetResetState_TC_07_007_004
 * @subsection PmSetResetState_TC_07_007_004_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetResetState_TC_07_007_004_tci Test Case ID
 * PmSetResetState_S0_A_TC_07_007_004
 * @subsection PmSetResetState_TC_07_007_004_rr Related Requirement(s)
 * \#239141
 * @subsection PmSetResetState_TC_07_007_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetResetState_TC_07_007_004_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetResetState function in abnormal condition:
 * - Pre-process: set required state to OSAL_PM_REQUIRED_STATE_REQUIRED.
 * - Test07_007_004: set reset state with device handle is closed
 * @subsection PmSetResetState_TC_07_007_004_tpts Test Procedure/Test Steps
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * -# Set reset state by using R_OSAL_PmSetResetState with the arguments as below:
 *      - Device handle of test target device which has already been closed
 *      - OSAL_PM_RESET_STATE_RELEASED
 * -# Re-open test target device by using R_OSAL_IoDeviceOpen for the next test cases
 * @subsection PmSetResetState_TC_07_007_004_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_HANDLE
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not equal to NULL_PTR
 * @subsection PmSetResetState_TC_07_007_004_testcase Test Case Example
 * @snippet test_pm.cpp PmSetResetState_TC_07_007_004
 */
// [PmSetResetState_TC_07_007_004]
TEST_F(PMApiTest, PmSetResetState_TC_07_007_004)
{

    // @brief: abnormal case: device handle is closed
    // Test07_007_004
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle, OSAL_PM_RESET_STATE_RELEASED), OSAL_RETURN_HANDLE) << "Test07_007_004 failed";
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_PM_DEVICE_2, &device_handle), OSAL_RETURN_OK); // re-open for the next test case
    EXPECT_NE(device_handle, NULL_PTR);
}
// [PmSetResetState_TC_07_007_004]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetResetState_TC_07_007_005 PMApiTest.PmSetResetState_TC_07_007_005
 * @subsection PmSetResetState_TC_07_007_005_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetResetState_TC_07_007_005_tci Test Case ID
 * PmSetResetState_S0_BA_TC_07_007_005
 * @subsection PmSetResetState_TC_07_007_005_rr Related Requirement(s)
 * \#239133, \#239141
 * @subsection PmSetResetState_TC_07_007_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetResetState_TC_07_007_005_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetResetState function in abnormal condition:
 * - Pre-process: set required state to OSAL_PM_REQUIRED_STATE_REQUIRED.
 * - Test07_007_005: set reset state is OSAL_PM_RESET_STATE_INVALID
 * @subsection PmSetResetState_TC_07_007_005_tpts Test Procedure/Test Steps
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set reset state by using R_OSAL_PmSetResetState with the arguments as below:
 *      - Device handle of test target device
 *      - OSAL_PM_RESET_STATE_INVALID
 * -# Set the required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * @subsection PmSetResetState_TC_07_007_005_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_PAR
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * @subsection PmSetResetState_TC_07_007_005_testcase Test Case Example
 * @snippet test_pm.cpp PmSetResetState_TC_07_007_005
 */
// [PmSetResetState_TC_07_007_005]
TEST_F(PMApiTest, PmSetResetState_TC_07_007_005)
{
    bool applyImmediate = true;

    // pre-process: set required state to OSAL_PM_REQUIRED_STATE_REQUIRED.
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);

    // @brief: abnormal case: state in R_OSAL_PmSetResetState is OSAL_PM_RESET_STATE_INVALID
    // Test07_007_005
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle, OSAL_PM_RESET_STATE_INVALID), OSAL_RETURN_PAR) << "Test07_007_005 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, false), OSAL_RETURN_OK);
}
// [PmSetResetState_TC_07_007_005]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetResetState_TC_07_007_006 PMApiTest.PmSetResetState_TC_07_007_006
 * @subsection PmSetResetState_TC_07_007_006_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetResetState_TC_07_007_006_tci Test Case ID
 * PmSetResetState_S0_BA_TC_07_007_006
 * @subsection PmSetResetState_TC_07_007_006_rr Related Requirement(s)
 * \#239133, \#239141
 * @subsection PmSetResetState_TC_07_007_006_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetResetState_TC_07_007_006_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetResetState function in abnormal condition:
 * - Pre-process: set required state to OSAL_PM_REQUIRED_STATE_REQUIRED.
 * - Test07_007_006: set reset state is OSAL_PM_RESET_STATE_RELEASED + 1
 * @subsection PmSetResetState_TC_07_007_006_tpts Test Procedure/Test Steps
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set reset state by using R_OSAL_PmSetResetState with the arguments as below:
 *      - Device handle of test target device
 *      - OSAL_PM_RESET_STATE_RELEASED + 1
 * -# Set the required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * @subsection PmSetResetState_TC_07_007_006_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_PAR
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * @subsection PmSetResetState_TC_07_007_006_testcase Test Case Example
 * @snippet test_pm.cpp PmSetResetState_TC_07_007_006
 */
// [PmSetResetState_TC_07_007_006]
TEST_F(PMApiTest, PmSetResetState_TC_07_007_006)
{
    bool applyImmediate = true;

    // pre-process: set required state to OSAL_PM_REQUIRED_STATE_REQUIRED.
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);

    // @brief: abnormal case: state in R_OSAL_PmSetResetState is OSAL_PM_RESET_STATE_RELEASED + 1
    // Test07_007_006
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle, (e_osal_pm_reset_state_t)(OSAL_PM_RESET_STATE_RELEASED + 1)), OSAL_RETURN_PAR) << "Test07_007_006 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, false), OSAL_RETURN_OK);
}
// [PmSetResetState_TC_07_007_006]

/**
 * @page PM_TEST Power API Test
 * @section OsalApiStateTest_PmSetResetState_TC_07_007_007 OsalApiStateTest.PmSetResetState_TC_07_007_007
 * @subsection PmSetResetState_TC_07_007_007_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection PmSetResetState_TC_07_007_007_tci Test Case ID
 * PmSetResetState_S2_A_TC_07_007_007
 * @subsection PmSetResetState_TC_07_007_007_rr Related Requirement(s)
 * \#239141
 * @subsection PmSetResetState_TC_07_007_007_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection PmSetResetState_TC_07_007_007_tcs Test Case Summary
 * This test shall verify the functionality of the PM APIs in invalid state transition:
 * - Test07_007_007: Checking the state of R_OSAL_PmSetResetState
 * @subsection PmSetResetState_TC_07_007_007_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_PmSetResetState
 * @subsection PmSetResetState_TC_07_007_007_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_STATE.
 * @subsection PmSetResetState_TC_07_007_007_testcase Test Case Example
 * @snippet test_pm.cpp PmSetResetState_TC_07_007_007
 */
// [PmSetResetState_TC_07_007_007]
TEST_F(OsalApiStateTest, PmSetResetState_TC_07_007_007)
{
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;

    // Test07_007_007
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle, OSAL_PM_RESET_STATE_APPLIED), OSAL_RETURN_STATE) << "Test07_007_007 failed";
}
// [PmSetResetState_TC_07_007_007]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetResetState_TC_07_007_008 PMApiTest.PmSetResetState_TC_07_007_008
 * @subsection PmSetResetState_TC_07_007_008_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetResetState_TC_07_007_008_tci Test Case ID
 * PmSetResetState_S0_A_TC_07_007_008
 * @subsection PmSetResetState_TC_07_007_008_rr Related Requirement(s)
 * \#239133, \#239141
 * @subsection PmSetResetState_TC_07_007_008_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetResetState_TC_07_007_008_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetResetState function in abnormal condition:
 * - Test07_007_008: The device assigned to handle is not supported by Power Manager
 * @subsection PmSetResetState_TC_07_007_008_tpts Test Procedure/Test Steps
 * -# Set reset state by using R_OSAL_PmSetResetState with the unsupport reset device handle
 * @subsection PmSetResetState_TC_07_007_008_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_UNSUPPORTED_OPERATION
 * @subsection PmSetResetState_TC_07_007_008_testcase Test Case Example
 * @snippet test_pm.cpp PmSetResetState_TC_07_007_008
 */
// [PmSetResetState_TC_07_007_008]
TEST_F(PMApiTest, PmSetResetState_TC_07_007_008)
{
    bool applyImmediate = true;

    // @brief: abnormal case: The device assigned to handle is not supported by Power Manager
    // Test07_007_008
    EXPECT_EQ(R_OSAL_PmSetResetState(unsupport_reset_dev_handle, OSAL_PM_RESET_STATE_RELEASED), OSAL_RETURN_UNSUPPORTED_OPERATION) << "Test07_007_008 failed";
}
// [PmSetResetState_TC_07_007_008]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetResetState_TC_07_007_009 PMApiTest.PmSetResetState_TC_07_007_009
 * @subsection PmSetResetState_TC_07_007_009_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetResetState_TC_07_007_009_tci Test Case ID
 * PmSetResetState_S0_BA_TC_07_007_009
 * @subsection PmSetResetState_TC_07_007_009_rr Related Requirement(s)
 * \#239141
 * @subsection PmSetResetState_TC_07_007_009_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSetResetState_TC_07_007_009_tcs Test Case Summary
 * This test shall verify the functionality of R_OSAL_PmSetResetState()
 * - Test07_007_009: Using R_OSAL_PmSetResetState with the reset state is (OSAL_PM_RESET_STATE_INVALID - 1)
 * @subsection PmSetResetState_TC_07_007_009_tpts Test Procedure/Test Steps
 * -# Call function  R_OSAL_PmSetResetState with the reset state is (OSAL_PM_RESET_STATE_INVALID - 1)
 * @subsection PmSetResetState_TC_07_007_009_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_PAR.
 * @subsection PmSetResetState_TC_07_007_009_testcase Test Case Example
 * @snippet test_pm.cpp PmSetResetState_TC_07_007_009
 */
// [PmSetResetState_TC_07_007_009]
TEST_F(PMApiTest, PmSetResetState_TC_07_007_009)
{
    // Test07_007_009: boundary abormal test: Using R_OSAL_PmSetResetState with the reset state is (OSAL_PM_RESET_STATE_INVALID - 1)
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle, (e_osal_pm_reset_state_t)(OSAL_PM_RESET_STATE_INVALID - 1)), OSAL_RETURN_PAR) << "Test07_007_009 failed";
}
// [PmSetResetState_TC_07_007_009]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetResetState_TC_07_008_001 PMApiTest.PmGetResetState_TC_07_008_001
 * @subsection PmGetResetState_TC_07_008_001_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetResetState_TC_07_008_001_tci Test Case ID
 * PmGetResetState_S1_N_TC_07_008_001
 * @subsection PmGetResetState_TC_07_008_001_rr Related Requirement(s)
 *  \#239142
 * @subsection PmGetResetState_TC_07_008_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmGetResetState_TC_07_008_001_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetResetState function in normal condition:
 * - Pre-process: set required state to OSAL_PM_REQUIRED_STATE_REQUIRED.
 * - Test07_008_001: get reset state with OSAL_PM_RESET_STATE_APPLIED
 * @subsection PmGetResetState_TC_07_008_001_tpts Test Procedure/Test Steps
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set reset state to OSAL_PM_RESET_STATE_APPLIED by using R_OSAL_PmSetResetState
 * -# Get reset state by using R_OSAL_PmGetResetState
 * -# Set the required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * @subsection PmGetResetState_TC_07_008_001_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetResetState return OSAL_RETURN_OK.<br>
 * Value of "reset" is equal to OSAL_PM_RESET_STATE_APPLIED
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * @subsection PmGetResetState_TC_07_008_001_testcase Test Case Example
 * @snippet test_pm.cpp PmGetResetState_TC_07_008_001
 */
// [PmGetResetState_TC_07_008_001]
TEST_F(PMApiTest, PmGetResetState_TC_07_008_001)
{
    e_osal_pm_reset_state_t reset = OSAL_PM_RESET_STATE_INVALID;
    bool applyImmediate = true;

    // pre-process: set required state to OSAL_PM_REQUIRED_STATE_REQUIRED.
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);

    // @brief: normal case: get reset state with OSAL_PM_RESET_STATE_APPLIED
    // Test07_008_001
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle, OSAL_PM_RESET_STATE_APPLIED), OSAL_RETURN_OK) << "Test07_008_001 failed";
    R_OSAL_ThreadSleepForTimePeriod(10);
    EXPECT_EQ(R_OSAL_PmGetResetState(device_handle, &reset), OSAL_RETURN_OK) << "Test07_008_001 failed";
    EXPECT_EQ(reset, OSAL_PM_RESET_STATE_APPLIED) << "Test07_008_001 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, false), OSAL_RETURN_OK);
}
// [PmGetResetState_TC_07_008_001]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetResetState_TC_07_008_002 PMApiTest.PmGetResetState_TC_07_008_002
 * @subsection PmGetResetState_TC_07_008_002_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetResetState_TC_07_008_002_tci Test Case ID
 * PmGetResetState_S1_N_TC_07_008_002
 * @subsection PmGetResetState_TC_07_008_002_rr Related Requirement(s)
 *  \#239142
 * @subsection PmGetResetState_TC_07_008_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmGetResetState_TC_07_008_002_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetResetState function in normal condition:
 * - Pre-process: set required state to OSAL_PM_REQUIRED_STATE_REQUIRED.
 * - Test07_008_002: get reset state with OSAL_PM_RESET_STATE_RELEASED
 * @subsection PmGetResetState_TC_07_008_002_tpts Test Procedure/Test Steps
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set reset state to OSAL_PM_RESET_STATE_RELEASED by using R_OSAL_PmSetResetState
 * -# Get reset state by using R_OSAL_PmGetResetState
 * -# Set the required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * @subsection PmGetResetState_TC_07_008_002_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetResetState return OSAL_RETURN_OK.<br>
 * Value of "reset" is equal to OSAL_PM_RESET_STATE_RELEASED
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * @subsection PmGetResetState_TC_07_008_002_testcase Test Case Example
 * @snippet test_pm.cpp PmGetResetState_TC_07_008_002
 */
// [PmGetResetState_TC_07_008_002]
TEST_F(PMApiTest, PmGetResetState_TC_07_008_002)
{
    e_osal_pm_reset_state_t reset = OSAL_PM_RESET_STATE_INVALID;
    bool applyImmediate = true;

    // pre-process: set required state to OSAL_PM_REQUIRED_STATE_REQUIRED.
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);

    // @brief: normal case: get reset state with OSAL_PM_RESET_STATE_RELEASED
    // Test07_008_002
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle, OSAL_PM_RESET_STATE_RELEASED), OSAL_RETURN_OK) << "Test07_008_002 failed";
    R_OSAL_ThreadSleepForTimePeriod(10);
    EXPECT_EQ(R_OSAL_PmGetResetState(device_handle, &reset), OSAL_RETURN_OK) << "Test07_008_002 failed";
    EXPECT_EQ(reset, OSAL_PM_RESET_STATE_RELEASED) << "Test07_008_002 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, false), OSAL_RETURN_OK);
}
// [PmGetResetState_TC_07_008_002]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetResetState_TC_07_008_003 PMApiTest.PmGetResetState_TC_07_008_003
 * @subsection PmGetResetState_TC_07_008_003_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetResetState_TC_07_008_003_tci Test Case ID
 * PmGetResetState_S0_A_TC_07_008_003
 * @subsection PmGetResetState_TC_07_008_003_rr Related Requirement(s)
 * \#199151, \#239142
 * @subsection PmGetResetState_TC_07_008_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection PmGetResetState_TC_07_008_003_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetResetState function in abnormal condition:
 * - Test07_008_003: get reset state with device handle is NULL
 * @subsection PmGetResetState_TC_07_008_003_tpts Test Procedure/Test Steps
 * -# Get reset state by using R_OSAL_PmGetResetState with the arguments as below:
 *      - Device handle is NULL
 *      - Address of "reset"
 * @subsection PmGetResetState_TC_07_008_003_ereb Expected Result/Behavior
 * -# R_OSAL_PmGetResetState return OSAL_RETURN_HANDLE.<br>
 * Value of "reset" is equal to OSAL_PM_RESET_STATE_INVALID
 * @subsection PmGetResetState_TC_07_008_003_testcase Test Case Example
 * @snippet test_pm.cpp PmGetResetState_TC_07_008_003
 */
// [PmGetResetState_TC_07_008_003]
TEST_F(PMApiTest, PmGetResetState_TC_07_008_003)
{
    e_osal_pm_reset_state_t reset = OSAL_PM_RESET_STATE_INVALID;
    bool applyImmediate = true;

    // @brief: abnormal case: device handle is NULL
    // Test07_008_003
    EXPECT_EQ(R_OSAL_PmGetResetState(OSAL_DEVICE_HANDLE_INVALID, &reset), OSAL_RETURN_HANDLE) << "Test07_008_003 failed";
    EXPECT_EQ(reset, OSAL_PM_RESET_STATE_INVALID) << "Test07_008_003 failed";
}
// [PmGetResetState_TC_07_008_003]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetResetState_TC_07_008_004 PMApiTest.PmGetResetState_TC_07_008_004
 * @subsection PmGetResetState_TC_07_008_004_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetResetState_TC_07_008_004_tci Test Case ID
 * PmGetResetState_S0_A_TC_07_008_004
 * @subsection PmGetResetState_TC_07_008_004_rr Related Requirement(s)
 * \#239142
 * @subsection PmGetResetState_TC_07_008_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmGetResetState_TC_07_008_004_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetResetState function in abnormal condition:
 * - Pre-process: set required state to OSAL_PM_REQUIRED_STATE_REQUIRED.
 * - Test07_008_004: get reset state with device handle is closed
 * @subsection PmGetResetState_TC_07_008_004_tpts Test Procedure/Test Steps
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * -# Get reset state by using R_OSAL_PmGetResetState with the arguments as below:
 *      - Device handle of test target device which has already been closed
 *      - Address of "reset"
 * -# Re-open test target device by using R_OSAL_IoDeviceOpen for the next test cases
 * @subsection PmGetResetState_TC_07_008_004_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_PmGetResetState return OSAL_RETURN_HANDLE.<br>
 * Value of "reset" is equal to OSAL_PM_RESET_STATE_INVALID
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * "device_handle" is not equal to NULL_PTR
 * @subsection PmGetResetState_TC_07_008_004_testcase Test Case Example
 * @snippet test_pm.cpp PmGetResetState_TC_07_008_004
 */
// [PmGetResetState_TC_07_008_004]
TEST_F(PMApiTest, PmGetResetState_TC_07_008_004)
{
    e_osal_pm_reset_state_t reset = OSAL_PM_RESET_STATE_INVALID;
    bool applyImmediate = true;

    // @brief: abnormal case: device handle is closed
    // Test07_008_004
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmGetResetState(device_handle, &reset), OSAL_RETURN_HANDLE) << "Test07_008_004 failed";
    EXPECT_EQ(reset, OSAL_PM_RESET_STATE_INVALID) << "Test07_008_004 failed";
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_PM_DEVICE_2, &device_handle), OSAL_RETURN_OK); // re-open for the next test case
    EXPECT_NE(device_handle, NULL_PTR);
}
// [PmGetResetState_TC_07_008_004]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetResetState_TC_07_008_005 PMApiTest.PmGetResetState_TC_07_008_005
 * @subsection PmGetResetState_TC_07_008_005_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetResetState_TC_07_008_005_tci Test Case ID
 * PmGetResetState_S0_A_TC_07_008_005
 * @subsection PmGetResetState_TC_07_008_005_rr Related Requirement(s)
 * \#239133, \#239142
 * @subsection PmGetResetState_TC_07_008_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmGetResetState_TC_07_008_005_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetResetState function in abnormal condition:
 * - Pre-process: set required state to OSAL_PM_REQUIRED_STATE_REQUIRED.
 * - Test07_008_005: get reset state with p_state in R_OSAL_PmGetResetState is NULL
 * @subsection PmGetResetState_TC_07_008_005_tpts Test Procedure/Test Steps
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Get reset state by using R_OSAL_PmGetResetState with the arguments as below:
 *      - Device handle of test target device
 *      - NULL
 * -# Set the required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * @subsection PmGetResetState_TC_07_008_005_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetResetState return OSAL_RETURN_PAR
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * @subsection PmGetResetState_TC_07_008_005_testcase Test Case Example
 * @snippet test_pm.cpp PmGetResetState_TC_07_008_005
 */
// [PmGetResetState_TC_07_008_005]
TEST_F(PMApiTest, PmGetResetState_TC_07_008_005)
{
    bool applyImmediate = true;
    e_osal_pm_reset_state_t *p_returned_reset = NULL;

    // pre-process: set required state to OSAL_PM_REQUIRED_STATE_REQUIRED.
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);

    // Test07_008_005
    // @brief: abnormal case: p_state in R_OSAL_PmGetResetState is NULL
    EXPECT_EQ(R_OSAL_PmGetResetState(device_handle, p_returned_reset), OSAL_RETURN_PAR) << "Test07_008_005 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, false), OSAL_RETURN_OK);
}
// [PmGetResetState_TC_07_008_005]

/**
 * @page PM_TEST Power API Test
 * @section OsalApiStateTest_PmGetResetState_TC_07_008_006 OsalApiStateTest.PmGetResetState_TC_07_008_006
 * @subsection PmGetResetState_TC_07_008_006_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection PmGetResetState_TC_07_008_006_tci Test Case ID
 * PmGetResetState_S2_A_TC_07_008_006
 * @subsection PmGetResetState_TC_07_008_006_rr Related Requirement(s)
 * \#239142
 * @subsection PmGetReset7State_TC_07_008_006_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection PmGetResetState_TC_07_008_006_tcs Test Case Summary
 * This test shall verify the functionality of the PM APIs in invalid state transition:
 * - Test07_008_006: Checking the state of R_OSAL_PmGetResetState
 * @subsection PmGetResetState_TC_07_008_006_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_PmGetResetState
 * @subsection PmGetResetState_TC_07_008_006_ereb Expected Result/Behavior
 * -# R_OSAL_PmGetResetState return OSAL_RETURN_STATE.<br>
 * The returned reset state is OSAL_PM_RESET_STATE_INVALID.
 * @subsection PmGetResetState_TC_07_008_006_testcase Test Case Example
 * @snippet test_pm.cpp PmGetResetState_TC_07_008_006
 */
// [PmGetResetState_TC_07_008_006]
TEST_F(OsalApiStateTest, PmGetResetState_TC_07_008_006)
{
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;
    e_osal_pm_reset_state_t reset = OSAL_PM_RESET_STATE_INVALID;

    // Test07_008_006
    EXPECT_EQ(R_OSAL_PmGetResetState(device_handle, &reset), OSAL_RETURN_STATE) << "Test07_008_006 failed";
    EXPECT_EQ(reset, OSAL_PM_RESET_STATE_INVALID) << "Test07_008_006 failed";
}
// [PmGetResetState_TC_07_008_006]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetResetState_TC_07_008_007 PMApiTest.PmGetResetState_TC_07_008_007
 * @subsection PmGetResetState_TC_07_008_007_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetResetState_TC_07_008_007_tci Test Case ID
 * PmGetResetState_S0_A_TC_07_008_007
 * @subsection PmGetResetState_TC_07_008_007_rr Related Requirement(s)
 * \#239133, \#239142
 * @subsection PmGetResetState_TC_07_008_007_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmGetResetState_TC_07_008_007_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetResetState function in abnormal condition:
 * - Test07_008_007: The device assigned to handle is not supported by Power Manager
 * @subsection PmGetResetState_TC_07_008_007_tpts Test Procedure/Test Steps
 * -# Get reset state by using R_OSAL_PmGetResetState with the unsupport reset device
 * @subsection PmGetResetState_TC_07_008_007_ereb Expected Result/Behavior
 * -# R_OSAL_PmGetResetState return OSAL_RETURN_UNSUPPORTED_OPERATION
 * @subsection PmGetResetState_TC_07_008_007_testcase Test Case Example
 * @snippet test_pm.cpp PmGetResetState_TC_07_008_007
 */
// [PmGetResetState_TC_07_008_007]
TEST_F(PMApiTest, PmGetResetState_TC_07_008_007)
{
    e_osal_pm_reset_state_t returned_reset;

    // Test07_008_007
    // @brief: abnormal case: The device assigned to handle is not supported by Power Manager
    EXPECT_EQ(R_OSAL_PmGetResetState(unsupport_reset_dev_handle, &returned_reset), OSAL_RETURN_UNSUPPORTED_OPERATION) << "Test07_008_007 failed";
}
// [PmGetResetState_TC_07_008_007]
#endif

#ifndef SKIP_SECTION_2
#if !defined(UNSUPPORT_POSTCLOCK_API)
/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPostClock_TC_07_009_001 PMApiTest.PmSetPostClock_TC_07_009_001
 * @subsection PmSetPostClock_TC_07_009_001_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPostClock_TC_07_009_001_tci Test Case ID
 * PmSetPostClock_S1_N_TC_07_009_001
 * @subsection PmSetPostClock_TC_07_009_001_rr Related Requirement(s)
 * \#287972
 * @subsection PmSetPostClock_TC_07_009_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - FBA/FBC device was opened.
 * @subsection PmSetPostClock_TC_07_009_001_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPostClock function in normal condition:
 * - Test07_009_001: enable post clock, set clock divider to TEST_CLOCK_DIVIDER (Values available on the target HW) for FBA/FBC device
 * @subsection PmSetPostClock_TC_07_009_001_tpts Test Procedure/Test Steps
 * -# Enable post-clock and set clock divider to TEST_CLOCK_DIVIDER for FBA/FBC device by using R_OSAL_PmSetPostClock
 * @subsection PmSetPostClock_TC_07_009_001_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetPostClock return OSAL_RETURN_OK
 * @subsection PmSetPostClock_TC_07_009_001_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPostClock_TC_07_009_001
 */
/* Please remove this ifdef once implemented */
// [PmSetPostClock_TC_07_009_001]
TEST_F(PMApiTest, PmSetPostClock_TC_07_009_001)
{
    bool clock_enable = true;

    // Test07_009_001
    //brief: normal case: enable post clock, set clock divider to TEST_CLOCK_DIVIDER for FBA/FBC device
    EXPECT_EQ(R_OSAL_PmSetPostClock(device_handle2, clock_enable, TEST_CLOCK_DIVIDER), OSAL_RETURN_OK) << "Test07_009_001 failed";
}
// [PmSetPostClock_TC_07_009_001]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPostClock_TC_07_009_002 PMApiTest.PmSetPostClock_TC_07_009_002
 * @subsection PmSetPostClock_TC_07_009_002_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPostClock_TC_07_009_002_tci Test Case ID
 * PmSetPostClock_S1_N_TC_07_009_002
 * @subsection PmSetPostClock_TC_07_009_002_rr Related Requirement(s)
 * \#287972
 * @subsection PmSetPostClock_TC_07_009_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - FBA/FBC device was opened.
 * @subsection PmSetPostClock_TC_07_009_002_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPostClock function in normal condition:
 * - Test07_009_002: disable post clock for FBA/FBC device
 * @subsection PmSetPostClock_TC_07_009_002_tpts Test Procedure/Test Steps
 * -# Disable post-clock of FBA/FBC device by using R_OSAL_PmSetPostClock
 * @subsection PmSetPostClock_TC_07_009_002_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetPostClock return OSAL_RETURN_OK
 * @subsection PmSetPostClock_TC_07_009_002_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPostClock_TC_07_009_002
 */
/* Please remove this ifdef once implemented */
// [PmSetPostClock_TC_07_009_002]
TEST_F(PMApiTest, PmSetPostClock_TC_07_009_002)
{
    bool clock_enable = false;

    // Test07_009_002
    //brief: normal case: disable post clock for FBA/FBC device
    EXPECT_EQ(R_OSAL_PmSetPostClock(device_handle2, clock_enable, TEST_CLOCK_DIVIDER), OSAL_RETURN_OK) << "Test07_009_002 failed";
}
// [PmSetPostClock_TC_07_009_002]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPostClock_TC_07_009_003 PMApiTest.PmSetPostClock_TC_07_009_003
 * @subsection PmSetPostClock_TC_07_009_003_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPostClock_TC_07_009_003_tci Test Case ID
 * PmSetPostClock_S0_BA_TC_07_009_003
 * @subsection PmSetPostClock_TC_07_009_003_rr Related Requirement(s)
 * \#287972
 * @subsection PmSetPostClock_TC_07_009_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - FBA/FBC device was opened.
 * @subsection PmSetPostClock_TC_07_009_003_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPostClock function in abnormal condition:
 * - Test07_009_003: enable post clock, set clock divider to 64 for FBA/FBC device
 * @subsection PmSetPostClock_TC_07_009_003_tpts Test Procedure/Test Steps
 * -# Enable post-clock and set clock divider to 64 for FBA/FBC device by using R_OSAL_PmSetPostClock
 * @subsection PmSetPostClock_TC_07_009_003_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetPostClock return OSAL_RETURN_PAR
 * @subsection PmSetPostClock_TC_07_009_003_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPostClock_TC_07_009_003
 */
// [PmSetPostClock_TC_07_009_003]
TEST_F(PMApiTest, PmSetPostClock_TC_07_009_003)
{
    bool clock_enable = true;
    uint32_t clock_divider = 64;

    // Test07_009_003
    //brief: boundary abnormal case: enable post clock, set clock divider to 64 for FBA/FBC device
    EXPECT_EQ(R_OSAL_PmSetPostClock(device_handle2, clock_enable, clock_divider), OSAL_RETURN_PAR) << "Test07_009_003 failed";
}
// [PmSetPostClock_TC_07_009_003]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPostClock_TC_07_009_004 PMApiTest.PmSetPostClock_TC_07_009_004
 * @subsection PmSetPostClock_TC_07_009_004_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPostClock_TC_07_009_004_tci Test Case ID
 * PmSetPostClock_S0_BA_TC_07_009_004
 * @subsection PmSetPostClock_TC_07_009_004_rr Related Requirement(s)
 * \#287972
 * @subsection PmSetPostClock_TC_07_009_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - FBA/FBC device was opened.
 * @subsection PmSetPostClock_TC_07_009_004_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPostClock function in abnormal condition:
 * - Test07_009_004: disable post clock, set clock divider to 64 for FBA/FBC device
 * @subsection PmSetPostClock_TC_07_009_004_tpts Test Procedure/Test Steps
 * -# Disable post-clock and set clock divider to 64 for FBA/FBC device by using R_OSAL_PmSetPostClock
 * @subsection PmSetPostClock_TC_07_009_004_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetPostClock return OSAL_RETURN_PAR
 * @subsection PmSetPostClock_TC_07_009_004_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPostClock_TC_07_009_004
 */
// [PmSetPostClock_TC_07_009_004]
TEST_F(PMApiTest, PmSetPostClock_TC_07_009_004)
{
    bool clock_enable = false;
    uint32_t clock_divider = 64;

    // Test07_009_004
    //brief: boundary abnormal case: disable post clock, set clock divider to 64 for FBA/FBC device
    EXPECT_EQ(R_OSAL_PmSetPostClock(device_handle2, clock_enable, clock_divider), OSAL_RETURN_PAR) << "Test07_009_004 failed";
}
// [PmSetPostClock_TC_07_009_004]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPostClock_TC_07_009_005 PMApiTest.PmSetPostClock_TC_07_009_005
 * @subsection PmSetPostClock_TC_07_009_005_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPostClock_TC_07_009_005_tci Test Case ID
 * PmSetPostClock_S0_A_TC_07_009_005
 * @subsection PmSetPostClock_TC_07_009_005_rr Related Requirement(s)
 * \#287972
 * @subsection PmSetPostClock_TC_07_009_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmSetPostClock_TC_07_009_005_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPostClock function in abnormal condition:
 * - Test07_009_005: enable post clock for un-support device
 * @subsection PmSetPostClock_TC_07_009_005_tpts Test Procedure/Test Steps
 * -# Enable post-clock and set clock divider for un-support device by using R_OSAL_PmSetPostClock
 * @subsection PmSetPostClock_TC_07_009_005_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetPostClock return OSAL_RETURN_UNSUPPORTED_OPERATION
 * @subsection PmSetPostClock_TC_07_009_005_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPostClock_TC_07_009_005
 */
// [PmSetPostClock_TC_07_009_005]
TEST_F(PMApiTest, PmSetPostClock_TC_07_009_005)
{
    bool clock_enable = true;

    // Test07_009_005
    //brief: abnormal case: set post clock for un-support device, clock_divider is true
    EXPECT_EQ(R_OSAL_PmSetPostClock(device_handle1, clock_enable, TEST_CLOCK_DIVIDER), OSAL_RETURN_UNSUPPORTED_OPERATION) << "Test07_009_005 failed";
}
// [PmSetPostClock_TC_07_009_005]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPostClock_TC_07_009_006 PMApiTest.PmSetPostClock_TC_07_009_006
 * @subsection PmSetPostClock_TC_07_009_006_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPostClock_TC_07_009_006_tci Test Case ID
 * PmSetPostClock_S0_A_TC_07_009_006
 * @subsection PmSetPostClock_TC_07_009_006_rr Related Requirement(s)
 * \#199151, \#287972
 * @subsection PmSetPostClock_TC_07_009_006_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection PmSetPostClock_TC_07_009_006_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPostClock function in abnormal condition:
 * - Test07_009_006: enable post clock for invalid device handle
 * @subsection PmSetPostClock_TC_07_009_006_tpts Test Procedure/Test Steps
 * -# Enable post-clock and set clock divider for invalid device handle by using R_OSAL_PmSetPostClock
 * @subsection PmSetPostClock_TC_07_009_006_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetPostClock return OSAL_RETURN_HANDLE
 * @subsection PmSetPostClock_TC_07_009_006_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPostClock_TC_07_009_006
 */
// [PmSetPostClock_TC_07_009_006]
TEST_F(PMApiTest, PmSetPostClock_TC_07_009_006)
{
    bool clock_enable = true;

    // Test07_009_006
    //brief: abnormal case: set post clock for invalid device handle, clock_divider is true
    EXPECT_EQ(R_OSAL_PmSetPostClock(OSAL_DEVICE_HANDLE_INVALID, clock_enable, TEST_CLOCK_DIVIDER), OSAL_RETURN_HANDLE) << "Test07_009_006 failed";
}
// [PmSetPostClock_TC_07_009_006]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPostClock_TC_07_009_007 PMApiTest.PmSetPostClock_TC_07_009_007
 * @subsection PmSetPostClock_TC_07_009_007_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPostClock_TC_07_009_007_tci Test Case ID
 * PmSetPostClock_S0_A_TC_07_009_007
 * @subsection PmSetPostClock_TC_07_009_007_rr Related Requirement(s)
 * \#287972
 * @subsection PmSetPostClock_TC_07_009_007_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - FBA/FBC device was opened.
 * @subsection PmSetPostClock_TC_07_009_007_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPostClock function in abnormal condition:
 * - Test07_009_007: enable post clock for closed device handle
 * @subsection PmSetPostClock_TC_07_009_007_tpts Test Procedure/Test Steps
 * -# Close FBA/FBC device by using R_OSAL_IoDeviceClose
 * -# Enable post-clock and set clock divider for closed device handle above by using R_OSAL_PmSetPostClock
 * -# Re-open FBA/FBC device by using R_OSAL_IoDeviceOpen for the next test cases
 * @subsection PmSetPostClock_TC_07_009_007_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPostClock return OSAL_RETURN_HANDLE
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK
 * @subsection PmSetPostClock_TC_07_009_007_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPostClock_TC_07_009_007
 */
// [PmSetPostClock_TC_07_009_007]
TEST_F(PMApiTest, PmSetPostClock_TC_07_009_007)
{
    bool clock_enable = true;

    // Test07_009_007
    //brief: abnormal case: set post clock for closed device handle, clock_divider is true
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle2), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetPostClock(device_handle2, clock_enable, TEST_CLOCK_DIVIDER), OSAL_RETURN_HANDLE) << "Test07_009_007 failed";
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_FBA_DEVICE_ID, &device_handle2), OSAL_RETURN_OK); // re-open for other tests
    EXPECT_NE(device_handle2, NULL_PTR);
}
// [PmSetPostClock_TC_07_009_007]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPostClock_TC_07_009_008 PMApiTest.PmSetPostClock_TC_07_009_008
 * @subsection PmSetPostClock_TC_07_009_008_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPostClock_TC_07_009_008_tci Test Case ID
 * PmSetPostClock_S0_A_TC_07_009_008
 * @subsection PmSetPostClock_TC_07_009_008_rr Related Requirement(s)
 * \#287972
 * @subsection PmSetPostClock_TC_07_009_008_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - FBA/FBC device were opened.
 * @subsection PmSetPostClock_TC_07_009_008_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPostClock function in abnormal condition:
 * - Test07_009_008: disable post clock for un-support device
 * @subsection PmSetPostClock_TC_07_009_008_tpts Test Procedure/Test Steps
 * -# Disable post-clock and set clock divider for un-support device by using R_OSAL_PmSetPostClock
 * @subsection PmSetPostClock_TC_07_009_008_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetPostClock return OSAL_RETURN_UNSUPPORTED_OPERATION
 * @subsection PmSetPostClock_TC_07_009_008_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPostClock_TC_07_009_008
 */
// [PmSetPostClock_TC_07_009_008]
TEST_F(PMApiTest, PmSetPostClock_TC_07_009_008)
{
    bool clock_enable = false;

    // Test07_009_008
    //brief: abnormal case: set post clock for un-support device, clock_divider is false
    EXPECT_EQ(R_OSAL_PmSetPostClock(device_handle1, clock_enable, TEST_CLOCK_DIVIDER), OSAL_RETURN_UNSUPPORTED_OPERATION) << "Test07_009_008 failed";
}
// [PmSetPostClock_TC_07_009_008]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPostClock_TC_07_009_009 PMApiTest.PmSetPostClock_TC_07_009_009
 * @subsection PmSetPostClock_TC_07_009_009_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPostClock_TC_07_009_009_tci Test Case ID
 * PmSetPostClock_S0_A_TC_07_009_009
 * @subsection PmSetPostClock_TC_07_009_009_rr Related Requirement(s)
 * \#199151, \#287972
 * @subsection PmSetPostClock_TC_07_009_009_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection PmSetPostClock_TC_07_009_009_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPostClock function in abnormal condition:
 * - Test07_009_009: disable post clock for invalid device handle
 * @subsection PmSetPostClock_TC_07_009_009_tpts Test Procedure/Test Steps
 * -# Disable post-clock and set clock divider for invalid device handle by using R_OSAL_PmSetPostClock
 * @subsection PmSetPostClock_TC_07_009_009_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetPostClock return OSAL_RETURN_HANDLE
 * @subsection PmSetPostClock_TC_07_009_009_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPostClock_TC_07_009_009
 */
// [PmSetPostClock_TC_07_009_009]
TEST_F(PMApiTest, PmSetPostClock_TC_07_009_009)
{
    bool clock_enable = false;

    // Test07_009_009
    //brief: abnormal case: set post clock for invalid device handle, clock_divider is false
    EXPECT_EQ(R_OSAL_PmSetPostClock(OSAL_DEVICE_HANDLE_INVALID, clock_enable, TEST_CLOCK_DIVIDER), OSAL_RETURN_HANDLE) << "Test07_009_009 failed";
}
// [PmSetPostClock_TC_07_009_009]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPostClock_TC_07_009_010 PMApiTest.PmSetPostClock_TC_07_009_010
 * @subsection PmSetPostClock_TC_07_009_010_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPostClock_TC_07_009_010_tci Test Case ID
 * PmSetPostClock_S0_A_TC_07_009_010
 * @subsection PmSetPostClock_TC_07_009_010_rr Related Requirement(s)
 * \#287972
 * @subsection PmSetPostClock_TC_07_009_010_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - FBA/FBC device was opened.
 * @subsection PmSetPostClock_TC_07_009_010_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPostClock function in abnormal condition:
 * - Test07_009_010: disable post clock for closed device handle
 * @subsection PmSetPostClock_TC_07_009_010_tpts Test Procedure/Test Steps
 * -# Close FBA/FBC device by using R_OSAL_IoDeviceClose
 * -# Disable post-clock and set clock divider for closed device handle above by using R_OSAL_PmSetPostClock
 * -# Re-open FBA/FBC device by using R_OSAL_IoDeviceOpen for the next test cases
 * @subsection PmSetPostClock_TC_07_009_010_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPostClock return OSAL_RETURN_HANDLE
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK
 * @subsection PmSetPostClock_TC_07_009_010_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPostClock_TC_07_009_010
 */
// [PmSetPostClock_TC_07_009_010]
TEST_F(PMApiTest, PmSetPostClock_TC_07_009_010)
{
    bool clock_enable = false;

    // Test07_009_010
    //brief: abnormal case: set post clock for closed device handle, clock_divider is false
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle2), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetPostClock(device_handle2, clock_enable, TEST_CLOCK_DIVIDER), OSAL_RETURN_HANDLE) << "Test07_009_010 failed";
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_FBA_DEVICE_ID, &device_handle2), OSAL_RETURN_OK); // re-open for other tests
    EXPECT_NE(device_handle2, NULL_PTR);
}
// [PmSetPostClock_TC_07_009_010]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPostClock_TC_07_009_011 PMApiTest.PmSetPostClock_TC_07_009_011
 * @subsection PmSetPostClock_TC_07_009_011_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPostClock_TC_07_009_011_tci Test Case ID
 * PmSetPostClock_S0_A_TC_07_009_011
 * @subsection PmSetPostClock_TC_07_009_011_rr Related Requirement(s)
 * \#287972
 * @subsection PmSetPostClock_TC_07_009_011_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - FBA/FBC device was opened.
 * @subsection PmSetPostClock_TC_07_009_011_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPostClock function in abnormal condition:
 * - Test07_009_011: enable post clock, set clock divider to 100 for FBA/FBC device
 * @subsection PmSetPostClock_TC_07_009_011_tpts Test Procedure/Test Steps
 * -# Enable post-clock and set clock divider to 100 for FBA/FBC device by using R_OSAL_PmSetPostClock
 * @subsection PmSetPostClock_TC_07_009_011_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetPostClock return OSAL_RETURN_PAR
 * @subsection PmSetPostClock_TC_07_009_011_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPostClock_TC_07_009_011
 */
// [PmSetPostClock_TC_07_009_011]
TEST_F(PMApiTest, PmSetPostClock_TC_07_009_011)
{
    bool clock_enable = true;
    uint32_t clock_divider = 100;

    // Test07_009_011
    //brief: abnormal case: enable post clock, set clock divider to 100 for FBA/FBC device
    EXPECT_EQ(R_OSAL_PmSetPostClock(device_handle2, clock_enable, clock_divider), OSAL_RETURN_PAR) << "Test07_009_011 failed";
}
// [PmSetPostClock_TC_07_009_011]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPostClock_TC_07_009_012 PMApiTest.PmSetPostClock_TC_07_009_012
 * @subsection PmSetPostClock_TC_07_009_012_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPostClock_TC_07_009_012_tci Test Case ID
 * PmSetPostClock_S0_A_TC_07_009_012
 * @subsection PmSetPostClock_TC_07_009_012_rr Related Requirement(s)
 * \#287972
 * @subsection PmSetPostClock_TC_07_009_012_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - FBA/FBC device was opened.
 * @subsection PmSetPostClock_TC_07_009_012_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPostClock function in abnormal condition:
 * - Test07_009_012: disable post clock, set clock divider to 100 for FBA/FBC device
 * @subsection PmSetPostClock_TC_07_009_012_tpts Test Procedure/Test Steps
 * -# Disable post-clock and set clock divider to 100 for FBA/FBC device by using R_OSAL_PmSetPostClock
 * @subsection PmSetPostClock_TC_07_009_012_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetPostClock return OSAL_RETURN_PAR
 * @subsection PmSetPostClock_TC_07_009_012_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPostClock_TC_07_009_012
 */
// [PmSetPostClock_TC_07_009_012]
TEST_F(PMApiTest, PmSetPostClock_TC_07_009_012)
{
    bool clock_enable = false;
    uint32_t clock_divider = 100;

    // Test07_009_012
    //brief: abnormal case: disable post clock, set clock divider to 100 for FBA/FBC device
    EXPECT_EQ(R_OSAL_PmSetPostClock(device_handle2, clock_enable, clock_divider), OSAL_RETURN_PAR) << "Test07_009_012 failed";
}
// [PmSetPostClock_TC_07_009_012]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPostClock_TC_07_009_013 PMApiTest.PmSetPostClock_TC_07_009_013
 * @subsection PmSetPostClock_TC_07_009_013_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPostClock_TC_07_009_013_tci Test Case ID
 * PmSetPostClock_S0_A_TC_07_009_013
 * @subsection PmSetPostClock_TC_07_009_013_rr Related Requirement(s)
 * \#287972
 * @subsection PmSetPostClock_TC_07_009_013_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - FBA/FBC device was opened.
 * @subsection PmSetPostClock_TC_07_009_013_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPostClock function in normal condition:
 * - Test07_009_013: enable enabled post clock
 * @subsection PmSetPostClock_TC_07_009_013_tpts Test Procedure/Test Steps
 * -# Enable post-clock and set clock divider to TEST_CLOCK_DIVIDER for FBA/FBC device by using R_OSAL_PmSetPostClock
 * -# Enable post-clock and set clock divider to TEST_CLOCK_DIVIDER for FBA/FBC device by using R_OSAL_PmSetPostClock
 * @subsection PmSetPostClock_TC_07_009_013_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetPostClock return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPostClock return OSAL_RETURN_OK
 * @subsection PmSetPostClock_TC_07_009_013_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPostClock_TC_07_009_013
 */
/* Please remove this ifdef once implemented */
// [PmSetPostClock_TC_07_009_013]
TEST_F(PMApiTest, PmSetPostClock_TC_07_009_013)
{
    bool clock_enable = true;

    // Test07_009_013
    //brief: normal case: enable post clock, set clock divider to 100 for FBA/FBC device 2 times
    EXPECT_EQ(R_OSAL_PmSetPostClock(device_handle2, clock_enable, TEST_CLOCK_DIVIDER), OSAL_RETURN_OK) << "Test07_009_013 failed";
    EXPECT_EQ(R_OSAL_PmSetPostClock(device_handle2, clock_enable, TEST_CLOCK_DIVIDER), OSAL_RETURN_OK) << "Test07_009_013 failed";
}
// [PmSetPostClock_TC_07_009_013]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPostClock_TC_07_009_014 PMApiTest.PmSetPostClock_TC_07_009_014
 * @subsection PmSetPostClock_TC_07_009_014_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPostClock_TC_07_009_014_tci Test Case ID
 * PmSetPostClock_S0_A_TC_07_009_014
 * @subsection PmSetPostClock_TC_07_009_014_rr Related Requirement(s)
 * \#287972
 * @subsection PmSetPostClock_TC_07_009_014_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - FBA/FBC device was opened.
 * @subsection PmSetPostClock_TC_07_009_014_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPostClock function in normal condition:
 * - Test07_009_014: disable disabled post clock
 * @subsection PmSetPostClock_TC_07_009_014_tpts Test Procedure/Test Steps
 * -# Disable post-clock for FBA/FBC device by using R_OSAL_PmSetPostClock
 * -# Disable post-clock for FBA/FBC device by using R_OSAL_PmSetPostClock
 * @subsection PmSetPostClock_TC_07_009_014_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetPostClock return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPostClock return OSAL_RETURN_OK
 * @subsection PmSetPostClock_TC_07_009_014_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPostClock_TC_07_009_014
 */
/* Please remove this ifdef once implemented */
// [PmSetPostClock_TC_07_009_014]
TEST_F(PMApiTest, PmSetPostClock_TC_07_009_014)
{
    bool clock_enable = false;

    // Test07_009_014
    //brief: normal case: disable post clock, set clock divider to 100 for FBA/FBC device 2 times
    EXPECT_EQ(R_OSAL_PmSetPostClock(device_handle2, clock_enable, TEST_CLOCK_DIVIDER), OSAL_RETURN_OK) << "Test07_009_014 failed";
    EXPECT_EQ(R_OSAL_PmSetPostClock(device_handle2, clock_enable, TEST_CLOCK_DIVIDER), OSAL_RETURN_OK) << "Test07_009_014 failed";
}
// [PmSetPostClock_TC_07_009_014]

/**
 * @page PM_TEST Power API Test
 * @section OsalApiStateTest_PmSetPostClock_TC_07_009_015 OsalApiStateTest.PmSetPostClock_TC_07_009_015
 * @subsection PmSetPostClock_TC_07_009_015_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection PmSetPostClock_TC_07_009_015_tci Test Case ID
 * PmSetPostClock_S2_A_TC_07_009_015
 * @subsection PmSetPostClock_TC_07_009_015_rr Related Requirement(s)
 * None
 * @subsection PmSetPostClock_TC_07_009_015_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection PmSetPostClock_TC_07_009_015_tcs Test Case Summary
 * This test shall verify the functionality of the PM APIs in invalid state transition:
 * - Test07_009_015: Checking the state of R_OSAL_PmSetPostClock
 * @subsection PmSetPostClock_TC_07_009_015_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_PmSetPostClock
 * @subsection PmSetPostClock_TC_07_009_015_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetPostClock return OSAL_RETURN_STATE.
 * @subsection PmSetPostClock_TC_07_009_015_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPostClock_TC_07_009_015
 */
// [PmSetPostClock_TC_07_009_015]
TEST_F(OsalApiStateTest, PmSetPostClock_TC_07_009_015)
{
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;
    bool clock_enable = true;

    // Test07_009_015
    EXPECT_EQ(R_OSAL_PmSetPostClock(device_handle, clock_enable, TEST_CLOCK_DIVIDER), OSAL_RETURN_STATE) << "Test07_009_015 failed";
}
// [PmSetPostClock_TC_07_009_015]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPostClock_TC_07_009_016 PMApiTest.PmSetPostClock_TC_07_009_016
 * @subsection PmSetPostClock_TC_07_009_016_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPostClock_TC_07_009_016_tci Test Case ID
 * PmSetPostClock_S0_BN_TC_07_009_016
 * @subsection PmSetPostClock_TC_07_009_016_rr Related Requirement(s)
 * \#287972
 * @subsection PmSetPostClock_TC_07_009_016_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - FBA/FBC device was opened.
 * @subsection PmSetPostClock_TC_07_009_016_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPostClock function:
 * - Test07_009_016: Enable post clock, set clock divider to value less than 64 for FBA/FBC device
 * @subsection PmSetPostClock_TC_07_009_016_tpts Test Procedure/Test Steps
 * -# Enable post clock, set clock divider to value less than 64 for FBA/FBC device by using R_OSAL_PmSetPostClock
 * @subsection PmSetPostClock_TC_07_009_016_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetPostClock return OSAL_RETURN_OK
 * @subsection PmSetPostClock_TC_07_009_016_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPostClock_TC_07_009_016
 */
// [PmSetPostClock_TC_07_009_016]
TEST_F(PMApiTest, PmSetPostClock_TC_07_009_016)
{
    bool clock_enable = true;
    uint32_t clock_divider = 63;

    // Test07_009_016
    //brief: boundary normal case: Enable post clock, set clock divider to value less than 64 for FBA/FBC device
    EXPECT_EQ(R_OSAL_PmSetPostClock(device_handle2, clock_enable, clock_divider), OSAL_RETURN_OK) << "Test07_009_016 failed";
}
// [PmSetPostClock_TC_07_009_016]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPostClock_TC_07_009_017 PMApiTest.PmSetPostClock_TC_07_009_017
 * @subsection PmSetPostClock_TC_07_009_017_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPostClock_TC_07_009_017_tci Test Case ID
 * PmSetPostClock_S0_BN_TC_07_009_017
 * @subsection PmSetPostClock_TC_07_009_017_rr Related Requirement(s)
 * \#287972
 * @subsection PmSetPostClock_TC_07_009_017_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - FBA/FBC device was opened.
 * @subsection PmSetPostClock_TC_07_009_017_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPostClock function:
 * - Test07_009_017: Disable post clock, set clock divider to value less than 64 for FBA/FBC device
 * @subsection PmSetPostClock_TC_07_009_017_tpts Test Procedure/Test Steps
 * -# Disable post clock, set clock divider to value less than 64 for FBA/FBC device by using R_OSAL_PmSetPostClock
 * @subsection PmSetPostClock_TC_07_009_017_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetPostClock return OSAL_RETURN_OK
 * @subsection PmSetPostClock_TC_07_009_017_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPostClock_TC_07_009_017
 */
// [PmSetPostClock_TC_07_009_017]
TEST_F(PMApiTest, PmSetPostClock_TC_07_009_017)
{
    bool clock_enable = false;
    uint32_t clock_divider = 63;

    // Test07_009_017
    //brief: boundary normal case: Disable post clock, set clock divider to value less than 64 for FBA/FBC device
    EXPECT_EQ(R_OSAL_PmSetPostClock(device_handle2, clock_enable, clock_divider), OSAL_RETURN_OK) << "Test07_009_017 failed";
}
// [PmSetPostClock_TC_07_009_017]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPostClock_TC_07_009_018 PMApiTest.PmSetPostClock_TC_07_009_018
 * @subsection PmSetPostClock_TC_07_009_018_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPostClock_TC_07_009_018_tci Test Case ID
 * PmSetPostClock_S0_BN_TC_07_009_018
 * @subsection PmSetPostClock_TC_07_009_018_rr Related Requirement(s)
 * \#287972
 * @subsection PmSetPostClock_TC_07_009_018_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - FBA/FBC device was opened.
 * @subsection PmSetPostClock_TC_07_009_018_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPostClock function:
 * - Test07_009_018: Enable post clock, set clock divider to 0 for FBA/FBC device
 * @subsection PmSetPostClock_TC_07_009_018_tpts Test Procedure/Test Steps
 * -# Enable post clock, set clock divider to 0 for FBA/FBC device by using R_OSAL_PmSetPostClock
 * @subsection PmSetPostClock_TC_07_009_018_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetPostClock return POST_CLOCK_BOUNDARY_TEST_EXPECTED_VALUE
 * @subsection PmSetPostClock_TC_07_009_018_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPostClock_TC_07_009_018
 */
// [PmSetPostClock_TC_07_009_018]
TEST_F(PMApiTest, PmSetPostClock_TC_07_009_018)
{
    bool clock_enable = true;
    uint32_t clock_divider = 0;

    // Test07_009_018
    //brief: boundary normal case: Enable post clock, set clock divider to 0 for FBA/FBC device
    EXPECT_EQ(R_OSAL_PmSetPostClock(device_handle2, clock_enable, clock_divider), POST_CLOCK_BOUNDARY_TEST_EXPECTED_VALUE) << "Test07_009_018 failed";
}
// [PmSetPostClock_TC_07_009_018]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSetPostClock_TC_07_009_019 PMApiTest.PmSetPostClock_TC_07_009_019
 * @subsection PmSetPostClock_TC_07_009_019_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSetPostClock_TC_07_009_019_tci Test Case ID
 * PmSetPostClock_S0_BN_TC_07_009_019
 * @subsection PmSetPostClock_TC_07_009_019_rr Related Requirement(s)
 * \#287972
 * @subsection PmSetPostClock_TC_07_009_019_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - FBA/FBC device was opened.
 * @subsection PmSetPostClock_TC_07_009_019_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmSetPostClock function:
 * - Test07_009_019: Disable post clock, set clock divider to 0 for FBA/FBC device
 * @subsection PmSetPostClock_TC_07_009_019_tpts Test Procedure/Test Steps
 * -# Disable post clock, set clock divider to 0 for FBA/FBC device by using R_OSAL_PmSetPostClock
 * @subsection PmSetPostClock_TC_07_009_019_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetPostClock return OSAL_RETURN_OK
 * @subsection PmSetPostClock_TC_07_009_019_testcase Test Case Example
 * @snippet test_pm.cpp PmSetPostClock_TC_07_009_019
 */
// [PmSetPostClock_TC_07_009_019]
TEST_F(PMApiTest, PmSetPostClock_TC_07_009_019)
{
    bool clock_enable = false;
    uint32_t clock_divider = 0;

    // Test07_009_019
    //brief: boundary normal case: Disable post clock, set clock divider to 0 for FBA/FBC device
    EXPECT_EQ(R_OSAL_PmSetPostClock(device_handle2, clock_enable, clock_divider), OSAL_RETURN_OK) << "Test07_009_019 failed";
}
// [PmSetPostClock_TC_07_009_019]
#endif /* UNSUPPORT_POSTCLOCK_API */

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetLowestIntermediatePowerState_TC_07_010_001 PMApiTest.PmGetLowestIntermediatePowerState_TC_07_010_001
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_001_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_001_tci Test Case ID
 * PmGetLowestIntermediatePowerState_S1_N_TC_07_010_001
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_001_rr Related Requirement(s)
 * \#239138
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_001_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetLowestIntermediatePowerState function in normal condition:
 * - Pre-process: close and re-open to reset the power state of test target device.
 * - Test07_010_001: get the lowest power state that test target device can reach at this time is OSAL_PM_STATE_ENABLED
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_001_tpts Test Procedure/Test Steps
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * -# Re-open test target device by using R_OSAL_IoDeviceOpen
 * -# Set required power state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState with applyimmediate is true.
 * -# Set power policy to OSAL_PM_POLICY_HP by using R_OSAL_PmSetPolicy with applyimmediate is true.
 * -# Set required power state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState with applyimmediate is true.
 * -# Get the lowest intermediate power state of test target device by using R_OSAL_PmGetLowestIntermediatePowerState
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_001_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * Value of "device_handle1" is not NULL
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetLowestIntermediatePowerState return OSAL_RETURN_OK.<br>
 * Value of "state" is OSAL_PM_STATE_ENABLED
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_001_testcase Test Case Example
 * @snippet test_pm.cpp PmGetLowestIntermediatePowerState_TC_07_010_001
 */
/* Please remove this ifdef once implemented */
// [PmGetLowestIntermediatePowerState_TC_07_010_001]
TEST_F(PMApiTest, PmGetLowestIntermediatePowerState_TC_07_010_001)
{
    bool applyimmediate = false;
    e_osal_pm_required_state_t required_state;
    e_osal_pm_state_t state;
    e_osal_pm_policy_t policy;

    // pre-process: close and re-open to reset device state.
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle1), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_PM_DEVICE_1, &device_handle1), OSAL_RETURN_OK);
    EXPECT_NE(device_handle1, NULL_PTR);

    // Test07_010_001
    // @brief: normal case: the lowest power state is ENABLE
    applyimmediate = true;
    required_state = OSAL_PM_REQUIRED_STATE_REQUIRED;
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, required_state, applyimmediate), OSAL_RETURN_OK);
    policy = OSAL_PM_POLICY_HP;
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, policy, applyimmediate), OSAL_RETURN_OK);
    required_state = OSAL_PM_REQUIRED_STATE_RELEASED;
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, required_state, applyimmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmGetLowestIntermediatePowerState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_010_001 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_ENABLED);
}
// [PmGetLowestIntermediatePowerState_TC_07_010_001]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetLowestIntermediatePowerState_TC_07_010_002 PMApiTest.PmGetLowestIntermediatePowerState_TC_07_010_002
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_002_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_002_tci Test Case ID
 * PmGetLowestIntermediatePowerState_S1_N_TC_07_010_002
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_002_rr Related Requirement(s)
 * \#239138
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_002_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetLowestIntermediatePowerState function in normal condition:
 * - Pre-process: close and re-open to reset the power state of test target device.
 * - Test07_010_002: get the lowest power state that test target device can reach at this time is OSAL_PM_STATE_CG in RELEASED
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_002_tpts Test Procedure/Test Steps
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * -# Re-open test target device by using R_OSAL_IoDeviceOpen
 * -# Set required power state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState with applyimmediate is true.
 * -# Set required power state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState with applyimmediate is true.
 * -# Set power policy to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyimmediate is true.
 * -# Set power policy to OSAL_PM_POLICY_CG by using R_OSAL_PmSetPolicy with applyimmediate is true.
 * -# Set power policy to OSAL_PM_POLICY_HP by using R_OSAL_PmSetPolicy with applyimmediate is true.
 * -# Get the lowest intermediate power state of test target device by using R_OSAL_PmGetLowestIntermediatePowerState
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_002_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * Value of "device_handle1" is not NULL
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetLowestIntermediatePowerState return OSAL_RETURN_OK.<br>
 * Value of "state" is OSAL_PM_STATE_CG
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_002_testcase Test Case Example
 * @snippet test_pm.cpp PmGetLowestIntermediatePowerState_TC_07_010_002
 */
/* Please remove this ifdef once implemented */
// [PmGetLowestIntermediatePowerState_TC_07_010_002]
TEST_F(PMApiTest, PmGetLowestIntermediatePowerState_TC_07_010_002)
{
    bool applyimmediate = true;
    e_osal_pm_state_t state;
    e_osal_pm_policy_t policy;
    e_osal_pm_required_state_t required_state;

    // pre-process: close and re-open to reset device state.
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle1), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_PM_DEVICE_1, &device_handle1), OSAL_RETURN_OK);
    EXPECT_NE(device_handle1, NULL_PTR);

    // Test07_010_002
    // @brief: normal case: get the lowest power state is CG in RELEASED
    required_state = OSAL_PM_REQUIRED_STATE_REQUIRED;
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, required_state, applyimmediate), OSAL_RETURN_OK);
    required_state = OSAL_PM_REQUIRED_STATE_RELEASED;
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, required_state, applyimmediate), OSAL_RETURN_OK);
    policy = OSAL_PM_POLICY_PG;
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, policy, applyimmediate), OSAL_RETURN_OK);
    policy = OSAL_PM_POLICY_CG;
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, policy, applyimmediate), OSAL_RETURN_OK);
    policy = OSAL_PM_POLICY_HP;
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, policy, applyimmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmGetLowestIntermediatePowerState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_010_002 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_CG);
}
// [PmGetLowestIntermediatePowerState_TC_07_010_002]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetLowestIntermediatePowerState_TC_07_010_003 PMApiTest.PmGetLowestIntermediatePowerState_TC_07_010_003
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_003_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_003_tci Test Case ID
 * PmGetLowestIntermediatePowerState_S1_A_TC_07_010_003
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_003_rr Related Requirement(s)
 * \#239138
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_003_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetLowestIntermediatePowerState function in normal condition:
 * - Pre-process: close and re-open to reset the power state of test target device.
 * - Test07_010_003: the lowest power state that test target device can reach at this time is OSAL_PM_STATE_CG in REQUIRED
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_003_tpts Test Procedure/Test Steps
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * -# Re-open test target device by using R_OSAL_IoDeviceOpen
 * -# Set required power state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState with applyimmediate is true.
 * -# Set required power state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState with applyimmediate is true.
 * -# Set power policy to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyimmediate is true.
 * -# Set power policy to OSAL_PM_POLICY_CG by using R_OSAL_PmSetPolicy with applyimmediate is true.
 * -# Set power policy to OSAL_PM_POLICY_HP by using R_OSAL_PmSetPolicy with applyimmediate is true.
 * -# Set required power state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState with applyimmediate is true.
 * -# Get the lowest intermediate power state of test target device by using R_OSAL_PmGetLowestIntermediatePowerState
 * -# Set required power state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_003_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * Value of "device_handle1" is not NULL
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetLowestIntermediatePowerState return OSAL_RETURN_OK.<br>
 * Value of "state" is OSAL_PM_STATE_CG
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_003_testcase Test Case Example
 * @snippet test_pm.cpp PmGetLowestIntermediatePowerState_TC_07_010_003
 */
/* Please remove this ifdef once implemented */
// [PmGetLowestIntermediatePowerState_TC_07_010_003]
TEST_F(PMApiTest, PmGetLowestIntermediatePowerState_TC_07_010_003)
{
    bool applyimmediate = true;
    e_osal_pm_required_state_t required_state;
    e_osal_pm_state_t state;
    e_osal_pm_policy_t policy;

    // pre-process: close and re-open to reset device state.
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle1), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_PM_DEVICE_1, &device_handle1), OSAL_RETURN_OK);
    EXPECT_NE(device_handle1, NULL_PTR);

    // Test07_010_003
    // @brief: normal case: get the lowest power state is CG in REQUIRED
    required_state = OSAL_PM_REQUIRED_STATE_REQUIRED;
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, required_state, applyimmediate), OSAL_RETURN_OK);
    required_state = OSAL_PM_REQUIRED_STATE_RELEASED;
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, required_state, applyimmediate), OSAL_RETURN_OK);
    policy = OSAL_PM_POLICY_PG;
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, policy, applyimmediate), OSAL_RETURN_OK);
    policy = OSAL_PM_POLICY_CG;
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, policy, applyimmediate), OSAL_RETURN_OK);
    policy = OSAL_PM_POLICY_HP;
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, policy, applyimmediate), OSAL_RETURN_OK);
    required_state = OSAL_PM_REQUIRED_STATE_REQUIRED;
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, required_state, applyimmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmGetLowestIntermediatePowerState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_010_003 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_CG);
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, false), OSAL_RETURN_OK);
}
// [PmGetLowestIntermediatePowerState_TC_07_010_003]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetLowestIntermediatePowerState_TC_07_010_004 PMApiTest.PmGetLowestIntermediatePowerState_TC_07_010_004
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_004_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_004_tci Test Case ID
 * PmGetLowestIntermediatePowerState_S1_A_TC_07_010_004
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_004_rr Related Requirement(s)
 * \#239138
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_004_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetLowestIntermediatePowerState function in normal condition:
 * - Pre-process: close and re-open to reset the power state of test target device.
 * - Test07_010_004: the lowest power state that test target device can reach at this time is OSAL_PM_STATE_PG in RELEASED
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_004_tpts Test Procedure/Test Steps
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * -# Re-open test target device by using R_OSAL_IoDeviceOpen
 * -# Set required power state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState with applyimmediate is true.
 * -# Set required power state to OSAL_PM_REQUIRED_STATE_REQUIRED for the dependence device by using R_OSAL_PmSetRequiredState with applyimmediate is true.
 * -# Set required power state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState with applyimmediate is true.
 * -# Set required power state to OSAL_PM_REQUIRED_STATE_RELEASED for the dependence device by using R_OSAL_PmSetRequiredState with applyimmediate is true.
 * -# Set power policy to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyimmediate is true.
 * -# Set power policy of the dependence device to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyimmediate is true.
 * -# Set power policy to OSAL_PM_POLICY_CG by using R_OSAL_PmSetPolicy with applyimmediate is true.
 * -# Set power policy to OSAL_PM_POLICY_HP by using R_OSAL_PmSetPolicy with applyimmediate is true.
 * -# Get the lowest intermediate power state of test target device by using R_OSAL_PmGetLowestIntermediatePowerState
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_004_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * Value of "device_handle" is not NULL
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetLowestIntermediatePowerState return OSAL_RETURN_OK.<br>
 * Value of "state" is PM_STATE_EXPECTED_VALUE
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_004_testcase Test Case Example
 * @snippet test_pm.cpp PmGetLowestIntermediatePowerState_TC_07_010_004
 */
/* Please remove this ifdef once implemented */
// [PmGetLowestIntermediatePowerState_TC_07_010_004]
TEST_F(PMApiTest, PmGetLowestIntermediatePowerState_TC_07_010_004)
{
    bool applyimmediate = true;
    e_osal_pm_required_state_t required_state;
    e_osal_pm_state_t state;
    e_osal_pm_policy_t policy;

    // pre-process: close and re-open to reset device state.
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_PM_DEVICE_2, &device_handle), OSAL_RETURN_OK);
    EXPECT_NE(device_handle, NULL_PTR);

    // Test07_010_004
    // @brief: normal case: the lowest power state is PG in RELEASED
    required_state = OSAL_PM_REQUIRED_STATE_REQUIRED;
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, required_state, applyimmediate), OSAL_RETURN_OK);
#if !defined(OSAL_SOC_S4) /* R-Car S4 doesn't have PG device */
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle_dependence, required_state, applyimmediate), OSAL_RETURN_OK);
#endif
    required_state = OSAL_PM_REQUIRED_STATE_RELEASED;
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, required_state, applyimmediate), OSAL_RETURN_OK);
#if !defined(OSAL_SOC_S4) /* R-Car S4 doesn't have PG device */
    //Set policy OSAL_PM_POLICY_PG + OSAL_PM_REQUIRED_STATE_RELEASED for dependence device
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle_dependence, required_state, applyimmediate), OSAL_RETURN_OK);
    policy = OSAL_PM_POLICY_PG;
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle_dependence, policy, applyimmediate), OSAL_RETURN_OK);
#endif
    policy = OSAL_PM_POLICY_PG;
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, policy, applyimmediate), OSAL_RETURN_OK);
    policy = OSAL_PM_POLICY_CG;
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, policy, applyimmediate), OSAL_RETURN_OK);
    policy = OSAL_PM_POLICY_HP;
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, policy, applyimmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmGetLowestIntermediatePowerState(device_handle, &state), OSAL_RETURN_OK) << "Test07_010_004 failed";
    EXPECT_EQ(state, PM_STATE_EXPECTED_VALUE);
}
// [PmGetLowestIntermediatePowerState_TC_07_010_004]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetLowestIntermediatePowerState_TC_07_010_005 PMApiTest.PmGetLowestIntermediatePowerState_TC_07_010_005
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_005_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_005_tci Test Case ID
 * PmGetLowestIntermediatePowerState_S1_A_TC_07_010_005
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_005_rr Related Requirement(s)
 * \#239138
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_005_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetLowestIntermediatePowerState function in normal condition:
 * - Pre-process: close and re-open to reset the power state of test target device.
 * - Test07_010_005: the lowest power state that test target device can reach at this time is OSAL_PM_STATE_PG in REQUIRED
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_005_tpts Test Procedure/Test Steps
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * -# Re-open test target device by using R_OSAL_IoDeviceOpen
 * -# Set required power state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState with applyimmediate is true.
 * -# Set required power state to OSAL_PM_REQUIRED_STATE_REQUIRED for the dependence device by using R_OSAL_PmSetRequiredState with applyimmediate is true.
 * -# Set required power state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState with applyimmediate is true.
 * -# Set required power state to OSAL_PM_REQUIRED_STATE_RELEASED for the dependence device by using R_OSAL_PmSetRequiredState with applyimmediate is true.
 * -# Set power policy of the dependence device to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyimmediate is true.
 * -# Set power policy to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyimmediate is true.
 * -# Set power policy to OSAL_PM_POLICY_CG by using R_OSAL_PmSetPolicy with applyimmediate is true.
 * -# Set power policy to OSAL_PM_POLICY_HP by using R_OSAL_PmSetPolicy with applyimmediate is true.
 * -# Set required power state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState with applyimmediate is true.
 * -# Get the lowest intermediate power state of test target device by using R_OSAL_PmGetLowestIntermediatePowerState
 * -# Set required power state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_005_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * Value of "device_handle" is not NULL
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetLowestIntermediatePowerState return OSAL_RETURN_OK.<br>
 * Value of "state" is PM_STATE_EXPECTED_VALUE
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_005_testcase Test Case Example
 * @snippet test_pm.cpp PmGetLowestIntermediatePowerState_TC_07_010_005
 */
/* Please remove this ifdef once implemented */
// [PmGetLowestIntermediatePowerState_TC_07_010_005]
TEST_F(PMApiTest, PmGetLowestIntermediatePowerState_TC_07_010_005)
{
    bool applyimmediate = true;
    e_osal_pm_required_state_t required_state;
    e_osal_pm_state_t state;
    e_osal_pm_policy_t policy;

    // pre-process: close and re-open to reset device state.
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_PM_DEVICE_2, &device_handle), OSAL_RETURN_OK);
    EXPECT_NE(device_handle, NULL_PTR);

    // Test07_010_005
    // @brief: normal case: the lowest power state is PG in REQUIRED
    required_state = OSAL_PM_REQUIRED_STATE_REQUIRED;
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, required_state, applyimmediate), OSAL_RETURN_OK);
#if !defined(OSAL_SOC_S4) /* R-Car S4 doesn't have PG device */
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle_dependence, required_state, applyimmediate), OSAL_RETURN_OK);
#endif
    required_state = OSAL_PM_REQUIRED_STATE_RELEASED;
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, required_state, applyimmediate), OSAL_RETURN_OK);
#if !defined(OSAL_SOC_S4) /* R-Car S4 doesn't have PG device */
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle_dependence, required_state, applyimmediate), OSAL_RETURN_OK);
#endif
    policy = OSAL_PM_POLICY_PG;
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, policy, applyimmediate), OSAL_RETURN_OK);
#if !defined(OSAL_SOC_S4) /* R-Car S4 doesn't have PG device */
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle_dependence, policy, applyimmediate), OSAL_RETURN_OK);
#endif
    policy = OSAL_PM_POLICY_CG;
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, policy, applyimmediate), OSAL_RETURN_OK);
    policy = OSAL_PM_POLICY_HP;
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, policy, applyimmediate), OSAL_RETURN_OK);
    required_state = OSAL_PM_REQUIRED_STATE_REQUIRED;
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, required_state, applyimmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmGetLowestIntermediatePowerState(device_handle, &state), OSAL_RETURN_OK) << "Test07_010_005 failed";
    EXPECT_EQ(state, PM_STATE_EXPECTED_VALUE);
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, false), OSAL_RETURN_OK);
}
// [PmGetLowestIntermediatePowerState_TC_07_010_005]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetLowestIntermediatePowerState_TC_07_010_006 PMApiTest.PmGetLowestIntermediatePowerState_TC_07_010_006
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_006_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_006_tci Test Case ID
 * PmGetLowestIntermediatePowerState_S0_A_TC_07_010_006
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_006_rr Related Requirement(s)
 * \#199151, \#239138
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_006_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_006_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetLowestIntermediatePowerState function in abnormal condition:
 * - Test07_010_006: get lowest state with invalid device handle
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_006_tpts Test Procedure/Test Steps
 * -# Get the lowest intermediate power state with OSAL_DEVICE_HANDLE_INVALID device handle by using R_OSAL_PmGetLowestIntermediatePowerState
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_006_ereb Expected Result/Behavior
 * -# R_OSAL_PmGetLowestIntermediatePowerState return OSAL_RETURN_HANDLE
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_006_testcase Test Case Example
 * @snippet test_pm.cpp PmGetLowestIntermediatePowerState_TC_07_010_006
 */
// [PmGetLowestIntermediatePowerState_TC_07_010_006]
TEST_F(PMApiTest, PmGetLowestIntermediatePowerState_TC_07_010_006)
{
    e_osal_pm_state_t state;

    // Test07_010_006
    // @brief: abnormal case: get lowest state with invalid device handle
    EXPECT_EQ(R_OSAL_PmGetLowestIntermediatePowerState(OSAL_DEVICE_HANDLE_INVALID, &state), OSAL_RETURN_HANDLE) << "Test07_010_006 failed";
}
// [PmGetLowestIntermediatePowerState_TC_07_010_006]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetLowestIntermediatePowerState_TC_07_010_007 PMApiTest.PmGetLowestIntermediatePowerState_TC_07_010_007
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_007_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_007_tci Test Case ID
 * PmGetLowestIntermediatePowerState_S0_A_TC_07_010_007
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_007_rr Related Requirement(s)
 * \#239138
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_007_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_007_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetLowestIntermediatePowerState function in abnormal condition:
 * - Test07_010_007: get lowest state with closed device handle
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_007_tpts Test Procedure/Test Steps
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * -# Get the lowest intermediate power state with closed device handle above by using R_OSAL_PmGetLowestIntermediatePowerState
 * -# Re-open test target device by using R_OSAL_IoDeviceOpen
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_007_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_PmGetLowestIntermediatePowerState return OSAL_RETURN_HANDLE
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * Value of "device_handle1" is not NULL
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_007_testcase Test Case Example
 * @snippet test_pm.cpp PmGetLowestIntermediatePowerState_TC_07_010_007
 */
// [PmGetLowestIntermediatePowerState_TC_07_010_007]
TEST_F(PMApiTest, PmGetLowestIntermediatePowerState_TC_07_010_007)
{
    e_osal_pm_state_t state;

    // Test07_010_007
    // @brief: abnormal case: get lowest state with closed device handle
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle1), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmGetLowestIntermediatePowerState(device_handle1, &state), OSAL_RETURN_HANDLE) << "Test07_010_007 failed";
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_PM_DEVICE_1, &device_handle1), OSAL_RETURN_OK); // re-open for other tests
    EXPECT_NE(device_handle1, NULL_PTR);
}
// [PmGetLowestIntermediatePowerState_TC_07_010_007]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetLowestIntermediatePowerState_TC_07_010_008 PMApiTest.PmGetLowestIntermediatePowerState_TC_07_010_008
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_008_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_008_tci Test Case ID
 * PmGetLowestIntermediatePowerState_S0_A_TC_07_010_008
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_008_rr Related Requirement(s)
 * \#239138
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_008_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_008_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetLowestIntermediatePowerState function in abnormal condition:
 * - Test07_010_008: get lowest state with p_state is NULL
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_008_tpts Test Procedure/Test Steps
 * -# Get the lowest intermediate power state by using R_OSAL_PmGetLowestIntermediatePowerState with the argument as below:
 *      - device handle of test target device
 *      - NULL
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_008_ereb Expected Result/Behavior
 * -# R_OSAL_PmGetLowestIntermediatePowerState return OSAL_RETURN_PAR
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_008_testcase Test Case Example
 * @snippet test_pm.cpp PmGetLowestIntermediatePowerState_TC_07_010_008
 */
// [PmGetLowestIntermediatePowerState_TC_07_010_008]
TEST_F(PMApiTest, PmGetLowestIntermediatePowerState_TC_07_010_008)
{
    e_osal_pm_state_t * p_returned_state = NULL;

    // Test07_010_008
    // @brief: abnormal case: get lowest state with p_state is NULL
    EXPECT_EQ(R_OSAL_PmGetLowestIntermediatePowerState(device_handle1, p_returned_state), OSAL_RETURN_PAR) << "Test07_010_008 failed";
}
// [PmGetLowestIntermediatePowerState_TC_07_010_008]

/**
 * @page PM_TEST Power API Test
 * @section OsalApiStateTest_PmGetLowestIntermediatePowerState_TC_07_010_009 OsalApiStateTest.PmGetLowestIntermediatePowerState_TC_07_010_009
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_009_tsi Test Suite ID
 * OsalApiStateTest
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_009_tci Test Case ID
 * PmGetLowestIntermediatePowerState_S2_A_TC_07_010_009
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_009_rr Related Requirement(s)
 * None
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_009_pp Prerequisite/Precondition
 * - R_OSAL_Deinitialize is already called
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_009_tcs Test Case Summary
 * This test shall verify the functionality of the PM APIs in invalid state transition:
 * - Test07_010_009: Checking the state of R_OSAL_PmGetLowestIntermediatePowerState
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_009_tpts Test Procedure/Test Steps
 * -# Call function R_OSAL_PmGetLowestIntermediatePowerState
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_009_ereb Expected Result/Behavior
 * -# R_OSAL_PmGetLowestIntermediatePowerState return OSAL_RETURN_STATE.
 * The returned state is OSAL_PM_STATE_INVALID.
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_009_testcase Test Case Example
 * @snippet test_pm.cpp PmGetLowestIntermediatePowerState_TC_07_010_009
 */
// [PmGetLowestIntermediatePowerState_TC_07_010_009]
TEST_F(OsalApiStateTest, PmGetLowestIntermediatePowerState_TC_07_010_009)
{
    osal_device_handle_t device_handle = OSAL_DEVICE_HANDLE_INVALID;
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;

    // Test07_010_009
    EXPECT_EQ(R_OSAL_PmGetLowestIntermediatePowerState(device_handle, &state), OSAL_RETURN_STATE) << "Test07_010_009 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_INVALID) << "Test07_010_009 failed";
}
// [PmGetLowestIntermediatePowerState_TC_07_010_009]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetLowestIntermediatePowerState_TC_07_010_010 PMApiTest.PmGetLowestIntermediatePowerState_TC_07_010_010
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_010_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_010_tci Test Case ID
 * PmGetLowestIntermediatePowerState_S0_A_TC_07_010_010
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_010_rr Related Requirement(s)
 * \#239138
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_010_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_010_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetLowestIntermediatePowerState function in abnormal condition:
 * - Test07_010_010: The device assigned to handle is not supported by Power Manager
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_010_tpts Test Procedure/Test Steps
 * -# Get the lowest intermediate power state by using R_OSAL_PmGetLowestIntermediatePowerState with the unsupport power device handle
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_010_ereb Expected Result/Behavior
 * -# R_OSAL_PmGetLowestIntermediatePowerState return OSAL_RETURN_UNSUPPORTED_OPERATION
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_010_testcase Test Case Example
 * @snippet test_pm.cpp PmGetLowestIntermediatePowerState_TC_07_010_010
 */
// [PmGetLowestIntermediatePowerState_TC_07_010_010]
TEST_F(PMApiTest, PmGetLowestIntermediatePowerState_TC_07_010_010)
{
    e_osal_pm_state_t returned_state;

    // Test07_010_010
    // @brief: abnormal case: The device assigned to handle is not supported by Power Manager
    EXPECT_EQ(R_OSAL_PmGetLowestIntermediatePowerState(unsupport_power_dev_handle, &returned_state), OSAL_RETURN_UNSUPPORTED_OPERATION) << "Test07_010_010 failed";
}
// [PmGetLowestIntermediatePowerState_TC_07_010_010]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetLowestIntermediatePowerState_TC_07_010_011 PMApiTest.PmGetLowestIntermediatePowerState_TC_07_010_011
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_011_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_011_tci Test Case ID
 * PmGetLowestIntermediatePowerState_S0_A_TC_07_010_011
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_011_rr Related Requirement(s)
 * \#239138
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_011_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_011_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetLowestIntermediatePowerState function in abnormal condition:
 * - Test07_010_011: The device assigned to handle is not supported by Power Manager
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_011_tpts Test Procedure/Test Steps
 * -# Get the lowest intermediate power state by using R_OSAL_PmGetLowestIntermediatePowerState with the unsupport clock device handle
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_011_ereb Expected Result/Behavior
 * -# R_OSAL_PmGetLowestIntermediatePowerState return OSAL_RETURN_UNSUPPORTED_OPERATION
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_011_testcase Test Case Example
 * @snippet test_pm.cpp PmGetLowestIntermediatePowerState_TC_07_010_011
 */
// [PmGetLowestIntermediatePowerState_TC_07_010_011]
TEST_F(PMApiTest, PmGetLowestIntermediatePowerState_TC_07_010_011)
{
    e_osal_pm_state_t returned_state;

    // Test07_010_011
    // @brief: abnormal case: The device assigned to handle is not supported by Power Manager
    EXPECT_EQ(R_OSAL_PmGetLowestIntermediatePowerState(unsupport_clock_dev_handle, &returned_state), OSAL_RETURN_UNSUPPORTED_OPERATION) << "Test07_010_011 failed";
}
// [PmGetLowestIntermediatePowerState_TC_07_010_011]

#if !defined(OSAL_SOC_S4) /* R-Car S4 doesn't have PG device */
/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetLowestIntermediatePowerState_TC_07_010_012 PMApiTest.PmGetLowestIntermediatePowerState_TC_07_010_012
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_012_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_012_tci Test Case ID
 * PmGetLowestIntermediatePowerState_S0_A_TC_07_010_012
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_012_rr Related Requirement(s)
 * \#239138
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_012_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_012_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetLowestIntermediatePowerState function in normal condition:
 * - Test07_010_012: Initialize leak check for the PG policy initialize device
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_012_tpts Test Procedure/Test Steps
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * -# Re-open test target device by using R_OSAL_IoDeviceOpen
 * -# Set required power state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState with applyimmediate is true.
 * -# Set power policy to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyimmediate is true.
 * -# Set required power state to OSAL_PM_REQUIRED_STATE_RELEASE by using R_OSAL_PmSetRequiredState with applyimmediate is true.
 * -# Set power policy to OSAL_PM_POLICY_CG by using R_OSAL_PmSetPolicy with applyimmediate is true.
 * -# Get the lowest intermediate power state of test target device by using R_OSAL_PmGetLowestIntermediatePowerState
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_012_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * Value of "device_handle" is not NULL
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetLowestIntermediatePowerState return OSAL_RETURN_OK.<br>
 * Value of "state" is OSAL_PM_STATE_PG
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_012_testcase Test Case Example
 * @snippet test_pm.cpp PmGetLowestIntermediatePowerState_TC_07_010_012
 */
// [PmGetLowestIntermediatePowerState_TC_07_010_012]
TEST_F(PMApiTest, PmGetLowestIntermediatePowerState_TC_07_010_012)
{
    bool applyimmediate = true;
    e_osal_pm_required_state_t required_state;
    e_osal_pm_state_t state;
    e_osal_pm_policy_t policy;

    // pre-process: close and re-open to reset device state.
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_PM_DEVICE_2, &device_handle), OSAL_RETURN_OK);
    EXPECT_NE(device_handle, nullptr);

    // Test07_010_012
    // @brief: normal case: Initialize leak check for the PG policy initialize device.
    required_state = OSAL_PM_REQUIRED_STATE_REQUIRED;
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, required_state, applyimmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle_dependence, required_state, applyimmediate), OSAL_RETURN_OK);

    //set PG policy then RELEASED for "imp_01"
    policy = OSAL_PM_POLICY_PG;
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle_dependence, policy, applyimmediate), OSAL_RETURN_OK);
    required_state = OSAL_PM_REQUIRED_STATE_RELEASED;
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle_dependence, required_state, applyimmediate), OSAL_RETURN_OK);

    //set PG policy then RELEASED for "imp_00"
    policy = OSAL_PM_POLICY_PG;
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, policy, applyimmediate), OSAL_RETURN_OK);
    required_state = OSAL_PM_REQUIRED_STATE_RELEASED;
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, required_state, applyimmediate), OSAL_RETURN_OK);

    policy = OSAL_PM_POLICY_CG;
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, policy, applyimmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmGetLowestIntermediatePowerState(device_handle, &state), OSAL_RETURN_OK) << "Test07_010_012 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_PG);
}
// [PmGetLowestIntermediatePowerState_TC_07_010_012]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetLowestIntermediatePowerState_TC_07_010_013 PMApiTest.PmGetLowestIntermediatePowerState_TC_07_010_013
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_013_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_013_tci Test Case ID
 * PmGetLowestIntermediatePowerState_S0_A_TC_07_010_013
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_013_rr Related Requirement(s)
 * \#239138
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_013_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_013_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetLowestIntermediatePowerState function in normal condition:
 * - Test07_010_013: Initialize leak check for the PG policy initialize device
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_013_tpts Test Procedure/Test Steps
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * -# Re-open test target device by using R_OSAL_IoDeviceOpen
 * -# Set required power state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState with applyimmediate is true.
 * -# Set required power state of the dependence device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState with applyimmediate is true.
 * -# Set power policy of the dependence device to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyimmediate is true.
 * -# Set required power state of the dependence device to OSAL_PM_REQUIRED_STATE_RELEASE by using R_OSAL_PmSetRequiredState with applyimmediate is true.
 * -# Set power policy to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyimmediate is true.
 * -# Set required power state to OSAL_PM_REQUIRED_STATE_RELEASE by using R_OSAL_PmSetRequiredState with applyimmediate is true.
 * -# Set power policy to OSAL_PM_POLICY_HP by using R_OSAL_PmSetPolicy with applyimmediate is true.
 * -# Get the lowest intermediate power state of test target device by using R_OSAL_PmGetLowestIntermediatePowerState
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_013_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * Value of "device_handle" is not NULL
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetLowestIntermediatePowerState return OSAL_RETURN_OK.<br>
 * Value of "state" is OSAL_PM_STATE_PG
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_013_testcase Test Case Example
 * @snippet test_pm.cpp PmGetLowestIntermediatePowerState_TC_07_010_013
 */
// [PmGetLowestIntermediatePowerState_TC_07_010_013]
TEST_F(PMApiTest, PmGetLowestIntermediatePowerState_TC_07_010_013)
{
    bool applyimmediate = true;
    e_osal_pm_required_state_t required_state;
    e_osal_pm_state_t state;
    e_osal_pm_policy_t policy;

    // pre-process: close and re-open to reset device state.
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_PM_DEVICE_2, &device_handle), OSAL_RETURN_OK);
    EXPECT_NE(device_handle, nullptr);

    // Test07_010_013
    // @brief: normal case: Initialize leak check for the PG policy initialize device.
    required_state = OSAL_PM_REQUIRED_STATE_REQUIRED;
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, required_state, applyimmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle_dependence, required_state, applyimmediate), OSAL_RETURN_OK);

    //set PG policy then RELEASED for "imp_01"
    policy = OSAL_PM_POLICY_PG;
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle_dependence, policy, applyimmediate), OSAL_RETURN_OK);
    required_state = OSAL_PM_REQUIRED_STATE_RELEASED;
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle_dependence, required_state, applyimmediate), OSAL_RETURN_OK);

    //set PG policy then RELEASED for "imp_00"
    policy = OSAL_PM_POLICY_PG;
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, policy, applyimmediate), OSAL_RETURN_OK);
    required_state = OSAL_PM_REQUIRED_STATE_RELEASED;
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, required_state, applyimmediate), OSAL_RETURN_OK);
    policy = OSAL_PM_POLICY_HP;
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle, policy, applyimmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmGetLowestIntermediatePowerState(device_handle, &state), OSAL_RETURN_OK) << "Test07_010_013 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_PG);
}
// [PmGetLowestIntermediatePowerState_TC_07_010_013]
#endif

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmGetLowestIntermediatePowerState_TC_07_010_014 PMApiTest.PmGetLowestIntermediatePowerState_TC_07_010_014
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_014_tsi Test Suite ID
 * PMApiTest
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_014_tci Test Case ID
 * PmGetLowestIntermediatePowerState_S0_A_TC_07_010_014
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_014_rr Related Requirement(s)
 * \#239138
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_014_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_014_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_PmGetLowestIntermediatePowerState function in normal condition:
 * - Test07_010_014: Initialize leak check for the CG policy initialize device
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_014_tpts Test Procedure/Test Steps
 * -# Close test target device by using R_OSAL_IoDeviceClose
 * -# Re-open test target device by using R_OSAL_IoDeviceOpen
 * -# Set required power state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState with applyimmediate is true.
 * -# Set power policy to OSAL_PM_POLICY_CG by using R_OSAL_PmSetPolicy with applyimmediate is true.
 * -# Set required power state to OSAL_PM_REQUIRED_STATE_RELEASE by using R_OSAL_PmSetRequiredState with applyimmediate is true.
 * -# Set power policy to OSAL_PM_POLICY_HP by using R_OSAL_PmSetPolicy with applyimmediate is true.
 * -# Get the lowest intermediate power state of test target device by using R_OSAL_PmGetLowestIntermediatePowerState
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_014_ereb Expected Result/Behavior
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK.<br>
 * Value of "device_handle" is not NULL
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetLowestIntermediatePowerState return OSAL_RETURN_OK.<br>
 * Value of "state" is OSAL_PM_STATE_CG
 * @subsection PmGetLowestIntermediatePowerState_TC_07_010_014_testcase Test Case Example
 * @snippet test_pm.cpp PmGetLowestIntermediatePowerState_TC_07_010_014
 */
// [PmGetLowestIntermediatePowerState_TC_07_010_014]
TEST_F(PMApiTest, PmGetLowestIntermediatePowerState_TC_07_010_014)
{
    bool applyimmediate = true;
    e_osal_pm_required_state_t required_state;
    e_osal_pm_state_t state;
    e_osal_pm_policy_t policy;

    // pre-process: close and re-open to reset device state.
    EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle1), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_PM_DEVICE_1, &device_handle1), OSAL_RETURN_OK);
    EXPECT_NE(device_handle1, nullptr);

    // Test07_010_014
    // @brief: normal case: Initialize leak check for the CG policy initialize device
    required_state = OSAL_PM_REQUIRED_STATE_REQUIRED;
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, required_state, applyimmediate), OSAL_RETURN_OK);
    policy = OSAL_PM_POLICY_CG;
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, policy, applyimmediate), OSAL_RETURN_OK);
    required_state = OSAL_PM_REQUIRED_STATE_RELEASED;
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, required_state, applyimmediate), OSAL_RETURN_OK);
    policy = OSAL_PM_POLICY_HP;
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, policy, applyimmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmGetLowestIntermediatePowerState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_010_014 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_CG);
}
// [PmGetLowestIntermediatePowerState_TC_07_010_014]

/**
 * @page PM_TEST Power API Test
 * @section PMApiSequenceTest_PmSequence_TC_07_011_001 PMApiSequenceTest.PmSequence_TC_07_011_001
 * @subsection PmSequence_TC_07_011_001_tsi Test Suite ID
 * PMApiSequenceTest
 * @subsection PmSequence_TC_07_011_001_tci Test Case ID
 * PmSetPolicy_S1_N_TC_07_011_001
 * @subsection PmSequence_TC_07_011_001_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSequence_TC_07_011_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmSequence_TC_07_011_001_tcs Test Case Summary
 * This test shall verify the functionality of the PM sequence call when using with R_OSAL_PmSetPolicy function in normal condition:
 * - Pre-process: Set REQUIRED state, APPLY reset
 * - Test07_011_001 set policy with OSAL_PM_POLICY_CG during REQUIRED state, apply reset
 * @subsection PmSequence_TC_07_011_001_tpts Test Procedure/Test Steps
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set reset state to OSAL_PM_RESET_STATE_APPLIED by using R_OSAL_PmSetResetState
 * -# Set power policy to OSAL_PM_POLICY_CG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Set required power state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * @subsection PmSequence_TC_07_011_001_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_RESET
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * @subsection PmSequence_TC_07_011_001_testcase Test Case Example
 * @snippet test_pm.cpp PmSequence_TC_07_011_001
 */
// [PmSequence_TC_07_011_001]
TEST_F(PMApiSequenceTest, PmSequence_TC_07_011_001)
{
    bool applyImmediate = true;
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;

    //pre-process: Set REQUIRED state, APPLY reset
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle1, OSAL_PM_RESET_STATE_APPLIED), OSAL_RETURN_OK);

    // @brief: normal case: set policy with OSAL_PM_POLICY_CG during REQUIRED state, apply reset
    // Test07_011_001
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_CG, applyImmediate), OSAL_RETURN_OK) << "Test07_011_001 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_001 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_RESET) << "Test07_011_001 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, false), OSAL_RETURN_OK);
}
// [PmSequence_TC_07_011_001]

/**
 * @page PM_TEST Power API Test
 * @section PMApiSequenceTest_PmSequence_TC_07_011_002 PMApiSequenceTest.PmSequence_TC_07_011_002
 * @subsection PmSequence_TC_07_011_002_tsi Test Suite ID
 * PMApiSequenceTest
 * @subsection PmSequence_TC_07_011_002_tci Test Case ID
 * PmSetPolicy_S1_N_TC_07_011_002
 * @subsection PmSequence_TC_07_011_002_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSequence_TC_07_011_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmSequence_TC_07_011_002_tcs Test Case Summary
 * This test shall verify the functionality of the PM sequence call when using with R_OSAL_PmSetPolicy function in normal condition:
 * - Pre-process: set REQUIRED state, RELEASE reset
 * - Test07_011_002: set policy with OSAL_PM_POLICY_HP during REQUIRED state, RELEASE reset
 * @subsection PmSequence_TC_07_011_002_tpts Test Procedure/Test Steps
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set reset state to OSAL_PM_RESET_STATE_RELEASED by using R_OSAL_PmSetResetState
 * -# Set power policy to OSAL_PM_POLICY_HP by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Get current power policy by using R_OSAL_PmGetPolicy
 * -# Write "PM_WRITE32_VALUE" to Target test device with offset "PM_WRITE32_REG_OFFSET" by using R_OSAL_IoWrite32.
 * -# Read the register of Target test device with offset "PM_WRITE32_REG_OFFSET" by using R_OSAL_IoRead32.
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * @subsection PmSequence_TC_07_011_002_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_READY
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_OK.<br>
 * Value of "policy" is equal to OSAL_PM_POLICY_HP
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead32 return OSAL_RETURN_OK.<br>
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * The written value and the read value are the same.
 * @subsection PmSequence_TC_07_011_002_testcase Test Case Example
 * @snippet test_pm.cpp PmSequence_TC_07_011_002
 */
// [PmSequence_TC_07_011_002]
TEST_F(PMApiSequenceTest, PmSequence_TC_07_011_002)
{
    e_osal_pm_policy_t policy = OSAL_PM_POLICY_INVALID;
    bool applyImmediate = true;
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;
    uintptr_t offset = PM_WRITE32_REG_OFFSET;
    uint32_t read_data32 = 0;
    uint32_t write_data32 = PM_WRITE32_VALUE;

    //pre-process: Set REQUIRED state, RELEASE reset
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle1, OSAL_PM_RESET_STATE_RELEASED), OSAL_RETURN_OK);

    // @brief: normal case: set policy with OSAL_PM_POLICY_HP during REQUIRED state, RELEASE reset
    // Test07_011_002
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_HP, applyImmediate), OSAL_RETURN_OK) << "Test07_011_002 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_002 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_READY) << "Test07_011_002 failed";
    EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle1, &policy), OSAL_RETURN_OK) << "Test07_011_002 failed";
    EXPECT_EQ(policy, OSAL_PM_POLICY_HP) << "Test07_011_002 failed";
    EXPECT_EQ(R_OSAL_IoWrite32(device_handle1, offset, write_data32), OSAL_RETURN_OK) << "Test07_011_002 failed";
    EXPECT_EQ(R_OSAL_IoRead32(device_handle1, offset, &read_data32), OSAL_RETURN_OK) << "Test07_011_002 failed";
    EXPECT_EQ(read_data32, write_data32) << "Test07_011_002 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, false), OSAL_RETURN_OK);

}
// [PmSequence_TC_07_011_002]

/**
 * @page PM_TEST Power API Test
 * @section PMApiSequenceTest_PmSequence_TC_07_011_003 PMApiSequenceTest.PmSequence_TC_07_011_003
 * @subsection PmSequence_TC_07_011_003_tsi Test Suite ID
 * PMApiSequenceTest
 * @subsection PmSequence_TC_07_011_003_tci Test Case ID
 * PmSetPolicy_S1_N_TC_07_011_003
 * @subsection PmSequence_TC_07_011_003_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSequence_TC_07_011_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmSequence_TC_07_011_003_tcs Test Case Summary
 * This test shall verify the functionality of the PM sequence call when using with R_OSAL_PmSetPolicy function in normal condition:
 * - Pre-process: set REQUIRED state, RELEASE reset
 * - Test07_011_003: set policy with OSAL_PM_POLICY_CG during REQUIRED state, RELEASE reset
 * @subsection PmSequence_TC_07_011_003_tpts Test Procedure/Test Steps
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set reset state to OSAL_PM_RESET_STATE_RELEASED by using R_OSAL_PmSetResetState
 * -# Set power policy to OSAL_PM_POLICY_CG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Get current power policy by using R_OSAL_PmGetPolicy
 * -# Write "PM_WRITE32_VALUE" to Target test device with offset "PM_WRITE32_REG_OFFSET" by using R_OSAL_IoWrite32.
 * -# Read the register of Target test device with offset "PM_WRITE32_REG_OFFSET" by using R_OSAL_IoRead32.
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * @subsection PmSequence_TC_07_011_003_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_READY
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_OK.<br>
 * Value of "policy" is equal to OSAL_PM_POLICY_CG
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead32 return OSAL_RETURN_OK.<br>
 * The written value and the read value are the same.
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * @subsection PmSequence_TC_07_011_003_testcase Test Case Example
 * @snippet test_pm.cpp PmSequence_TC_07_011_003
 */
// [PmSequence_TC_07_011_003]
TEST_F(PMApiSequenceTest, PmSequence_TC_07_011_003)
{
    e_osal_pm_policy_t policy = OSAL_PM_POLICY_INVALID;
    bool applyImmediate = true;
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;
    uintptr_t offset = PM_WRITE32_REG_OFFSET;
    uint32_t read_data32 = 0;
    uint32_t write_data32 = PM_WRITE32_VALUE;

    //pre-process: Set REQUIRED state, RELEASE reset
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle1, OSAL_PM_RESET_STATE_RELEASED), OSAL_RETURN_OK);

    // @brief: normal case: set policy with OSAL_PM_POLICY_CG during REQUIRED state, RELEASE reset
    // Test07_011_003
    EXPECT_EQ(R_OSAL_IoRead32(device_handle1, offset, &read_data32), OSAL_RETURN_OK) << "Test07_011_003 failed";
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_CG, applyImmediate), OSAL_RETURN_OK) << "Test07_011_003 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_003 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_READY) << "Test07_011_003 failed";
    EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle1, &policy), OSAL_RETURN_OK) << "Test07_011_003 failed";
    EXPECT_EQ(policy, OSAL_PM_POLICY_CG) << "Test07_011_003 failed";
    EXPECT_EQ(R_OSAL_IoWrite32(device_handle1, offset, write_data32), OSAL_RETURN_OK) << "Test07_011_003 failed";
    EXPECT_EQ(R_OSAL_IoRead32(device_handle1, offset, &read_data32), OSAL_RETURN_OK) << "Test07_011_003 failed";
    EXPECT_EQ(read_data32, write_data32) << "Test07_011_003 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, false), OSAL_RETURN_OK);

}
// [PmSequence_TC_07_011_003]

/**
 * @page PM_TEST Power API Test
 * @section PMApiSequenceTest_PmSequence_TC_07_011_004 PMApiSequenceTest.PmSequence_TC_07_011_004
 * @subsection PmSequence_TC_07_011_004_tsi Test Suite ID
 * PMApiSequenceTest
 * @subsection PmSequence_TC_07_011_004_tci Test Case ID
 * PmSetPolicy_S1_N_TC_07_011_004
 * @subsection PmSequence_TC_07_011_004_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSequence_TC_07_011_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmSequence_TC_07_011_004_tcs Test Case Summary
 * This test shall verify the functionality of the PM sequence call when using with R_OSAL_PmSetPolicy function in normal condition:
 * - Pre-process: set REQUIRED state, RELEASE reset
 * - Test07_011_004: set policy with OSAL_PM_POLICY_PG during REQUIRED state, RELEASE reset
 * @subsection PmSequence_TC_07_011_004_tpts Test Procedure/Test Steps
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set reset state to OSAL_PM_RESET_STATE_RELEASED by using R_OSAL_PmSetResetState
 * -# Set power policy to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Get current power policy by using R_OSAL_PmGetPolicy
 * -# Write "PM_WRITE32_VALUE" to Target test device with offset "PM_WRITE32_REG_OFFSET" by using R_OSAL_IoWrite32.
 * -# Read the register of Target test device with offset "PM_WRITE32_REG_OFFSET" by using R_OSAL_IoRead32.
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * @subsection PmSequence_TC_07_011_004_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_READY
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_OK.<br>
 * Value of "policy" is equal to OSAL_PM_POLICY_PG
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead32 return OSAL_RETURN_OK.<br>
 * The written value and the read value are the same.
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * @subsection PmSequence_TC_07_011_004_testcase Test Case Example
 * @snippet test_pm.cpp PmSequence_TC_07_011_004
 */
// [PmSequence_TC_07_011_004]
TEST_F(PMApiSequenceTest, PmSequence_TC_07_011_004)
{
    e_osal_pm_policy_t policy = OSAL_PM_POLICY_INVALID;
    bool applyImmediate = true;
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;
    uintptr_t offset = PM_WRITE32_REG_OFFSET;
    uint32_t read_data32 = 0;
    uint32_t write_data32 = PM_WRITE32_VALUE;

    //pre-process: Set REQUIRED state, RELEASE reset
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK);
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle1, OSAL_PM_RESET_STATE_RELEASED), OSAL_RETURN_OK);

    // @brief: normal case: set policy with OSAL_PM_POLICY_PG during REQUIRED state, RELEASE reset
    // Test07_011_004
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK) << "Test07_011_004 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_004 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_READY) << "Test07_011_004 failed";
    EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle1, &policy), OSAL_RETURN_OK) << "Test07_011_004 failed";
    EXPECT_EQ(policy, OSAL_PM_POLICY_PG) << "Test07_011_004 failed";
    EXPECT_EQ(R_OSAL_IoWrite32(device_handle1, offset, write_data32), OSAL_RETURN_OK) << "Test07_011_004 failed";
    EXPECT_EQ(R_OSAL_IoRead32(device_handle1, offset, &read_data32), OSAL_RETURN_OK) << "Test07_011_004 failed";
    EXPECT_EQ(read_data32, write_data32) << "Test07_011_004 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, false), OSAL_RETURN_OK);

}
// [PmSequence_TC_07_011_004]

/**
 * @page PM_TEST Power API Test
 * @section PMApiSequenceTest_PmSequence_TC_07_011_005 PMApiSequenceTest.PmSequence_TC_07_011_005
 * @subsection PmSequence_TC_07_011_005_tsi Test Suite ID
 * PMApiSequenceTest
 * @subsection PmSequence_TC_07_011_005_tci Test Case ID
 * PmSetPolicy_S1_N_TC_07_011_005
 * @subsection PmSequence_TC_07_011_005_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSequence_TC_07_011_005_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmSequence_TC_07_011_005_tcs Test Case Summary
 * This test shall verify the functionality of the PM sequence call when using with R_OSAL_PmSetPolicy function in normal condition:
 * - Pre-process: set RELEASED state.
 * - Test07_011_005: set policy with OSAL_PM_POLICY_HP from OSAL_PM_POLICY_HP during RELEASE state
 * @subsection PmSequence_TC_07_011_005_tpts Test Procedure/Test Steps
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_HP by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Set power policy to OSAL_PM_POLICY_HP by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Get current power policy by using R_OSAL_PmGetPolicy
 * -# Write "PM_WRITE32_VALUE" to Target test device with offset "PM_WRITE32_REG_OFFSET" by using R_OSAL_IoWrite32.
 * -# Read the register of Target test device with offset "PM_WRITE32_REG_OFFSET" by using R_OSAL_IoRead32.
 * @subsection PmSequence_TC_07_011_005_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_ENABLED.
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_ENABLED
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_OK.<br>
 * Value of "policy" is equal to OSAL_PM_POLICY_HP
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead32 return OSAL_RETURN_OK.<br>
 * The written value and the read value are the same.
 * @subsection PmSequence_TC_07_011_005_testcase Test Case Example
 * @snippet test_pm.cpp PmSequence_TC_07_011_005
 */
// [PmSequence_TC_07_011_005]
TEST_F(PMApiSequenceTest, PmSequence_TC_07_011_005)
{
    e_osal_pm_policy_t policy = OSAL_PM_POLICY_INVALID;
    bool applyImmediate = true;
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;
    uintptr_t offset = PM_WRITE32_REG_OFFSET;
    uint32_t read_data32 = 0;
    uint32_t write_data32 = PM_WRITE32_VALUE;

    //pre-process: Set RELEASED state
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK);

    // @brief: normal case: set policy from OSAL_PM_POLICY_HP to OSAL_PM_POLICY_HP during RELEASE state
    // Test07_011_005
    //current policy
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_HP, applyImmediate), OSAL_RETURN_OK) << "Test07_011_005 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_005 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_ENABLED) << "Test07_011_005 failed";

    //after policy
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_HP, applyImmediate), OSAL_RETURN_OK) << "Test07_011_005 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_005 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_ENABLED) << "Test07_011_005 failed";
    EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle1, &policy), OSAL_RETURN_OK) << "Test07_011_005 failed";
    EXPECT_EQ(policy, OSAL_PM_POLICY_HP) << "Test07_011_005 failed";
    EXPECT_EQ(R_OSAL_IoWrite32(device_handle1, offset, write_data32), OSAL_RETURN_OK) << "Test07_011_005 failed";
    EXPECT_EQ(R_OSAL_IoRead32(device_handle1, offset, &read_data32), OSAL_RETURN_OK) << "Test07_011_005 failed";
    EXPECT_EQ(read_data32, write_data32) << "Test07_011_005 failed";

}
// [PmSequence_TC_07_011_005]

/**
 * @page PM_TEST Power API Test
 * @section PMApiSequenceTest_PmSequence_TC_07_011_006 PMApiSequenceTest.PmSequence_TC_07_011_006
 * @subsection PmSequence_TC_07_011_006_tsi Test Suite ID
 * PMApiSequenceTest
 * @subsection PmSequence_TC_07_011_006_tci Test Case ID
 * PmSetPolicy_S1_N_TC_07_011_006
 * @subsection PmSequence_TC_07_011_006_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSequence_TC_07_011_006_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmSequence_TC_07_011_006_tcs Test Case Summary
 * This test shall verify the functionality of the PM sequence call when using with R_OSAL_PmSetPolicy function in normal condition:
 * - Pre-process: set RELEASED state.
 * - Test07_011_006: set policy from OSAL_PM_POLICY_HP to OSAL_PM_POLICY_CG during RELEASE state
 * @subsection PmSequence_TC_07_011_006_tpts Test Procedure/Test Steps
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_HP by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Set power policy to OSAL_PM_POLICY_CG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Get current power policy by using R_OSAL_PmGetPolicy
 * @subsection PmSequence_TC_07_011_006_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_ENABLED.
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_CG
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_OK.<br>
 * Value of "policy" is equal to OSAL_PM_POLICY_CG
 * @subsection PmSequence_TC_07_011_006_testcase Test Case Example
 * @snippet test_pm.cpp PmSequence_TC_07_011_006
 */
// [PmSequence_TC_07_011_006]
TEST_F(PMApiSequenceTest, PmSequence_TC_07_011_006)
{
    e_osal_pm_policy_t policy = OSAL_PM_POLICY_INVALID;
    bool applyImmediate = true;
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;

    //pre-process: Set RELEASED state
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK);

    // @brief: normal case: set policy from OSAL_PM_POLICY_HP to OSAL_PM_POLICY_CG during RELEASE state
    // Test07_011_006
    //current policy
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_HP, applyImmediate), OSAL_RETURN_OK) << "Test07_011_006 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_006 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_ENABLED) << "Test07_011_006 failed";

    //after policy
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_CG, applyImmediate), OSAL_RETURN_OK) << "Test07_011_006 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_006 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_CG) << "Test07_011_006 failed";
    EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle1, &policy), OSAL_RETURN_OK) << "Test07_011_006 failed";
    EXPECT_EQ(policy, OSAL_PM_POLICY_CG) << "Test07_011_006 failed";
}
// [PmSequence_TC_07_011_006]

/**
 * @page PM_TEST Power API Test
 * @section PMApiSequenceTest_PmSequence_TC_07_011_007 PMApiSequenceTest.PmSequence_TC_07_011_007
 * @subsection PmSequence_TC_07_011_007_tsi Test Suite ID
 * PMApiSequenceTest
 * @subsection PmSequence_TC_07_011_007_tci Test Case ID
 * PmSetPolicy_S1_N_TC_07_011_007
 * @subsection PmSequence_TC_07_011_007_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSequence_TC_07_011_007_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmSequence_TC_07_011_007_tcs Test Case Summary
 * This test shall verify the functionality of the PM sequence call when using with R_OSAL_PmSetPolicy function in normal condition:
 * - Pre-process: set RELEASED state.
 * - Test07_011_007: set policy from OSAL_PM_POLICY_HP to OSAL_PM_POLICY_PG during RELEASE state
 * @subsection PmSequence_TC_07_011_007_tpts Test Procedure/Test Steps
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_HP by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Set power policy to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Get current power policy by using R_OSAL_PmGetPolicy
 * @subsection PmSequence_TC_07_011_007_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_ENABLED.
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_CG
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_OK.<br>
 * Value of "policy" is equal to OSAL_PM_POLICY_PG
 * @subsection PmSequence_TC_07_011_007_testcase Test Case Example
 * @snippet test_pm.cpp PmSequence_TC_07_011_007
 */
// [PmSequence_TC_07_011_007]
TEST_F(PMApiSequenceTest, PmSequence_TC_07_011_007)
{
    e_osal_pm_policy_t policy = OSAL_PM_POLICY_INVALID;
    bool applyImmediate = true;
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;

    //pre-process: Set RELEASED state
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK);

    // @brief: normal case: set policy from OSAL_PM_POLICY_HP to OSAL_PM_POLICY_PG during RELEASE state
    // Test07_011_007
    //current policy
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_HP, applyImmediate), OSAL_RETURN_OK) << "Test07_011_007 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_007 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_ENABLED) << "Test07_011_007 failed";

    //after policy
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK) << "Test07_011_007 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_007 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_CG) << "Test07_011_007 failed";
    EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle1, &policy), OSAL_RETURN_OK) << "Test07_011_007 failed";
    EXPECT_EQ(policy, OSAL_PM_POLICY_PG) << "Test07_011_007 failed";
}
// [PmSequence_TC_07_011_007]

/**
 * @page PM_TEST Power API Test
 * @section PMApiSequenceTest_PmSequence_TC_07_011_008 PMApiSequenceTest.PmSequence_TC_07_011_008
 * @subsection PmSequence_TC_07_011_008_tsi Test Suite ID
 * PMApiSequenceTest
 * @subsection PmSequence_TC_07_011_008_tci Test Case ID
 * PmSetPolicy_S1_N_TC_07_011_008
 * @subsection PmSequence_TC_07_011_008_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSequence_TC_07_011_008_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmSequence_TC_07_011_008_tcs Test Case Summary
 * This test shall verify the functionality of the PM sequence call when using with R_OSAL_PmSetPolicy function in normal condition:
 * - Pre-process: set RELEASED state.
 * - Test07_011_008: set policy from OSAL_PM_POLICY_CG to OSAL_PM_POLICY_HP during RELEASE state
 * @subsection PmSequence_TC_07_011_008_tpts Test Procedure/Test Steps
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_CG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Set power policy to OSAL_PM_POLICY_HP by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Get current power policy by using R_OSAL_PmGetPolicy
 * -# Write "PM_WRITE32_VALUE" to Target test device with offset "PM_WRITE32_REG_OFFSET" by using R_OSAL_IoWrite32.
 * -# Read the register of Target test device with offset "PM_WRITE32_REG_OFFSET" by using R_OSAL_IoRead32.
 * @subsection PmSequence_TC_07_011_008_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_CG.
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_ENABLED
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_OK.<br>
 * Value of "policy" is equal to OSAL_PM_POLICY_HP
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead32 return OSAL_RETURN_OK.<br>
 * The written value and the read value are the same.
 * @subsection PmSequence_TC_07_011_008_testcase Test Case Example
 * @snippet test_pm.cpp PmSequence_TC_07_011_008
 */
// [PmSequence_TC_07_011_008]
TEST_F(PMApiSequenceTest, PmSequence_TC_07_011_008)
{
    e_osal_pm_policy_t policy = OSAL_PM_POLICY_INVALID;
    bool applyImmediate = true;
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;
    uintptr_t offset = PM_WRITE32_REG_OFFSET;
    uint32_t read_data32 = 0;
    uint32_t write_data32 = PM_WRITE32_VALUE;

    //pre-process: Set RELEASED state
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK);

    // @brief: normal case: set policy from OSAL_PM_POLICY_CG to OSAL_PM_POLICY_HP during RELEASE state
    // Test07_011_008
    //current policy
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_CG, applyImmediate), OSAL_RETURN_OK) << "Test07_011_008 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_008 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_CG) << "Test07_011_008 failed";

    //after policy
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_HP, applyImmediate), OSAL_RETURN_OK) << "Test07_011_008 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_008 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_ENABLED) << "Test07_011_008 failed";
    EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle1, &policy), OSAL_RETURN_OK) << "Test07_011_008 failed";
    EXPECT_EQ(policy, OSAL_PM_POLICY_HP) << "Test07_011_008 failed";
    EXPECT_EQ(R_OSAL_IoWrite32(device_handle1, offset, write_data32), OSAL_RETURN_OK) << "Test07_011_008 failed";
    EXPECT_EQ(R_OSAL_IoRead32(device_handle1, offset, &read_data32), OSAL_RETURN_OK) << "Test07_011_008 failed";
    EXPECT_EQ(read_data32, write_data32) << "Test07_011_008 failed";
}
// [PmSequence_TC_07_011_008]

/**
 * @page PM_TEST Power API Test
 * @section PMApiSequenceTest_PmSequence_TC_07_011_009 PMApiSequenceTest.PmSequence_TC_07_011_009
 * @subsection PmSequence_TC_07_011_009_tsi Test Suite ID
 * PMApiSequenceTest
 * @subsection PmSequence_TC_07_011_009_tci Test Case ID
 * PmSetPolicy_S1_N_TC_07_011_009
 * @subsection PmSequence_TC_07_011_009_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSequence_TC_07_011_009_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmSequence_TC_07_011_009_tcs Test Case Summary
 * This test shall verify the functionality of the PM sequence call when using with R_OSAL_PmSetPolicy function in normal condition:
 * - Pre-process: set RELEASED state.
 * - Test07_011_009: set policy from OSAL_PM_POLICY_CG to OSAL_PM_POLICY_CG during RELEASE state
 * @subsection PmSequence_TC_07_011_009_tpts Test Procedure/Test Steps
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_CG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Set power policy to OSAL_PM_POLICY_CG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Get current power policy by using R_OSAL_PmGetPolicy
 * @subsection PmSequence_TC_07_011_009_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_CG.
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_CG
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_OK.<br>
 * Value of "policy" is equal to OSAL_PM_POLICY_CG
 * @subsection PmSequence_TC_07_011_009_testcase Test Case Example
 * @snippet test_pm.cpp PmSequence_TC_07_011_009
 */
// [PmSequence_TC_07_011_009]
TEST_F(PMApiSequenceTest, PmSequence_TC_07_011_009)
{
    e_osal_pm_policy_t policy = OSAL_PM_POLICY_INVALID;
    bool applyImmediate = true;
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;

    //pre-process: Set RELEASED state
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK);

    // @brief: normal case: set policy from OSAL_PM_POLICY_CG to OSAL_PM_POLICY_CG during RELEASE state
    // Test07_011_009
    //current policy
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_CG, applyImmediate), OSAL_RETURN_OK) << "Test07_011_009 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_009 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_CG) << "Test07_011_009 failed";

    //after policy
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_CG, applyImmediate), OSAL_RETURN_OK) << "Test07_011_009 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_009 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_CG) << "Test07_011_009 failed";
    EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle1, &policy), OSAL_RETURN_OK) << "Test07_011_009 failed";
    EXPECT_EQ(policy, OSAL_PM_POLICY_CG) << "Test07_011_009 failed";
}
// [PmSequence_TC_07_011_009]

/**
 * @page PM_TEST Power API Test
 * @section PMApiSequenceTest_PmSequence_TC_07_011_010 PMApiSequenceTest.PmSequence_TC_07_011_010
 * @subsection PmSequence_TC_07_011_010_tsi Test Suite ID
 * PMApiSequenceTest
 * @subsection PmSequence_TC_07_011_010_tci Test Case ID
 * PmSetPolicy_S1_N_TC_07_011_010
 * @subsection PmSequence_TC_07_011_010_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSequence_TC_07_011_010_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmSequence_TC_07_011_010_tcs Test Case Summary
 * This test shall verify the functionality of the PM sequence call when using with R_OSAL_PmSetPolicy function in normal condition:
 * - Pre-process: set RELEASED state.
 * - Test07_011_010: set policy from OSAL_PM_POLICY_CG to OSAL_PM_POLICY_PG during RELEASE state
 * @subsection PmSequence_TC_07_011_010_tpts Test Procedure/Test Steps
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_CG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Set power policy to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Get current power policy by using R_OSAL_PmGetPolicy
 * @subsection PmSequence_TC_07_011_010_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_CG.
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_CG
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_OK.<br>
 * Value of "policy" is equal to OSAL_PM_POLICY_PG
 * @subsection PmSequence_TC_07_011_010_testcase Test Case Example
 * @snippet test_pm.cpp PmSequence_TC_07_011_010
 */
// [PmSequence_TC_07_011_010]
TEST_F(PMApiSequenceTest, PmSequence_TC_07_011_010)
{
    e_osal_pm_policy_t policy = OSAL_PM_POLICY_INVALID;
    bool applyImmediate = true;
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;

    //pre-process: Set RELEASED state
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK);

    // @brief: normal case: set policy from OSAL_PM_POLICY_CG to OSAL_PM_POLICY_PG during RELEASE state
    // Test07_011_010
    //current policy
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_CG, applyImmediate), OSAL_RETURN_OK) << "Test07_011_010 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_010 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_CG) << "Test07_011_010 failed";

    //after policy
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK) << "Test07_011_010 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_010 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_CG) << "Test07_011_010 failed";
    EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle1, &policy), OSAL_RETURN_OK) << "Test07_011_010 failed";
    EXPECT_EQ(policy, OSAL_PM_POLICY_PG) << "Test07_011_010 failed";
}
// [PmSequence_TC_07_011_010]

/**
 * @page PM_TEST Power API Test
 * @section PMApiSequenceTest_PmSequence_TC_07_011_011 PMApiSequenceTest.PmSequence_TC_07_011_011
 * @subsection PmSequence_TC_07_011_011_tsi Test Suite ID
 * PMApiSequenceTest
 * @subsection PmSequence_TC_07_011_011_tci Test Case ID
 * PmSetPolicy_S1_N_TC_07_011_011
 * @subsection PmSequence_TC_07_011_011_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSequence_TC_07_011_011_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmSequence_TC_07_011_011_tcs Test Case Summary
 * This test shall verify the functionality of the PM sequence call when using with R_OSAL_PmSetPolicy function in normal condition:
 * - Pre-process: set RELEASED state.
 * - Test07_011_011: set policy from OSAL_PM_POLICY_PG to OSAL_PM_POLICY_HP during RELEASE state
 * @subsection PmSequence_TC_07_011_011_tpts Test Procedure/Test Steps
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Set power policy to OSAL_PM_POLICY_HP by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Get current power policy by using R_OSAL_PmGetPolicy
 * -# Write "PM_WRITE32_VALUE" to Target test device with offset "PM_WRITE32_REG_OFFSET" by using R_OSAL_IoWrite32.
 * -# Read the register of Target test device with offset "PM_WRITE32_REG_OFFSET" by using R_OSAL_IoRead32.
 * @subsection PmSequence_TC_07_011_011_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_CG.
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_ENABLED
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_OK.<br>
 * Value of "policy" is equal to OSAL_PM_POLICY_HP
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead32 return OSAL_RETURN_OK.<br>
 * The written value and the read value are the same.
 * @subsection PmSequence_TC_07_011_011_testcase Test Case Example
 * @snippet test_pm.cpp PmSequence_TC_07_011_011
 */
// [PmSequence_TC_07_011_011]
TEST_F(PMApiSequenceTest, PmSequence_TC_07_011_011)
{
    e_osal_pm_policy_t policy = OSAL_PM_POLICY_INVALID;
    bool applyImmediate = true;
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;
    uintptr_t offset = PM_WRITE32_REG_OFFSET;
    uint32_t read_data32 = 0;
    uint32_t write_data32 = PM_WRITE32_VALUE;

    //pre-process: Set RELEASED state
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK);

    // @brief: normal case: set policy from OSAL_PM_POLICY_PG to OSAL_PM_POLICY_HP during RELEASE state
    // Test07_011_011
    //current policy
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK) << "Test07_011_011 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_011 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_CG) << "Test07_011_011 failed";

    //after policy
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_HP, applyImmediate), OSAL_RETURN_OK) << "Test07_011_011 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_011 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_ENABLED) << "Test07_011_011 failed";
    EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle1, &policy), OSAL_RETURN_OK) << "Test07_011_011 failed";
    EXPECT_EQ(policy, OSAL_PM_POLICY_HP) << "Test07_011_011 failed";
    EXPECT_EQ(R_OSAL_IoWrite32(device_handle1, offset, write_data32), OSAL_RETURN_OK) << "Test07_011_002 failed";
    EXPECT_EQ(R_OSAL_IoRead32(device_handle1, offset, &read_data32), OSAL_RETURN_OK) << "Test07_011_002 failed";
    EXPECT_EQ(read_data32, write_data32) << "Test07_011_002 failed";
}
// [PmSequence_TC_07_011_011]

/**
 * @page PM_TEST Power API Test
 * @section PMApiSequenceTest_PmSequence_TC_07_011_012 PMApiSequenceTest.PmSequence_TC_07_011_012
 * @subsection PmSequence_TC_07_011_012_tsi Test Suite ID
 * PMApiSequenceTest
 * @subsection PmSequence_TC_07_011_012_tci Test Case ID
 * PmSetPolicy_S1_N_TC_07_011_012
 * @subsection PmSequence_TC_07_011_012_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSequence_TC_07_011_012_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmSequence_TC_07_011_012_tcs Test Case Summary
 * This test shall verify the functionality of the PM sequence call when using with R_OSAL_PmSetPolicy function in normal condition:
 * - Pre-process: set RELEASED state.
 * - Test07_011_012: set policy from OSAL_PM_POLICY_PG to OSAL_PM_POLICY_CG during RELEASE state
 * @subsection PmSequence_TC_07_011_012_tpts Test Procedure/Test Steps
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Set power policy to OSAL_PM_POLICY_CG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Get current power policy by using R_OSAL_PmGetPolicy
 * @subsection PmSequence_TC_07_011_012_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_CG.
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_CG
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_OK.<br>
 * Value of "policy" is equal to OSAL_PM_POLICY_CG
 * @subsection PmSequence_TC_07_011_012_testcase Test Case Example
 * @snippet test_pm.cpp PmSequence_TC_07_011_012
 */
// [PmSequence_TC_07_011_012]
TEST_F(PMApiSequenceTest, PmSequence_TC_07_011_012)
{
    e_osal_pm_policy_t policy = OSAL_PM_POLICY_INVALID;
    bool applyImmediate = true;
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;

    //pre-process: Set RELEASED state
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK);

    // @brief: normal case: set policy from OSAL_PM_POLICY_PG to OSAL_PM_POLICY_CG during RELEASE state
    // Test07_011_012
    //current policy
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK) << "Test07_011_012 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_012 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_CG) << "Test07_011_012 failed";

    //after policy
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_CG, applyImmediate), OSAL_RETURN_OK) << "Test07_011_012 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_012 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_CG) << "Test07_011_012 failed";
    EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle1, &policy), OSAL_RETURN_OK) << "Test07_011_012 failed";
    EXPECT_EQ(policy, OSAL_PM_POLICY_CG) << "Test07_011_012 failed";
}
// [PmSequence_TC_07_011_012]

/**
 * @page PM_TEST Power API Test
 * @section PMApiSequenceTest_PmSequence_TC_07_011_013 PMApiSequenceTest.PmSequence_TC_07_011_013
 * @subsection PmSequence_TC_07_011_013_tsi Test Suite ID
 * PMApiSequenceTest
 * @subsection PmSequence_TC_07_011_013_tci Test Case ID
 * PmSetPolicy_S1_N_TC_07_011_013
 * @subsection PmSequence_TC_07_011_013_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSequence_TC_07_011_013_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmSequence_TC_07_011_013_tcs Test Case Summary
 * This test shall verify the functionality of the PM sequence call when using with R_OSAL_PmSetPolicy function in normal condition:
 * - Pre-process: set RELEASED state.
 * - Test07_011_013: set policy from OSAL_PM_POLICY_PG to OSAL_PM_POLICY_PG during RELEASE state
 * @subsection PmSequence_TC_07_011_013_tpts Test Procedure/Test Steps
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Set power policy to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Get current power policy by using R_OSAL_PmGetPolicy
 * @subsection PmSequence_TC_07_011_013_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_CG.
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_CG
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_OK.<br>
 * Value of "policy" is equal to OSAL_PM_POLICY_PG
 * @subsection PmSequence_TC_07_011_013_testcase Test Case Example
 * @snippet test_pm.cpp PmSequence_TC_07_011_013
 */
// [PmSequence_TC_07_011_013]
TEST_F(PMApiSequenceTest, PmSequence_TC_07_011_013)
{
    e_osal_pm_policy_t policy = OSAL_PM_POLICY_INVALID;
    bool applyImmediate = true;
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;

    //pre-process: Set RELEASED state
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK);

    // @brief: normal case: set policy from OSAL_PM_POLICY_PG to OSAL_PM_POLICY_PG during RELEASE state
    // Test07_011_013
    //current policy
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK) << "Test07_011_013 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_013 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_CG) << "Test07_011_013 failed";

    //after policy
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK) << "Test07_011_013 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_013 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_CG) << "Test07_011_013 failed";
    EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle1, &policy), OSAL_RETURN_OK) << "Test07_011_013 failed";
    EXPECT_EQ(policy, OSAL_PM_POLICY_PG) << "Test07_011_013 failed";
}
// [PmSequence_TC_07_011_013]

/**
 * @page PM_TEST Power API Test
 * @section PMApiSequenceTest_PmSequence_TC_07_011_014 PMApiSequenceTest.PmSequence_TC_07_011_014
 * @subsection PmSequence_TC_07_011_014_tsi Test Suite ID
 * PMApiSequenceTest
 * @subsection PmSequence_TC_07_011_014_tci Test Case ID
 * PmSetPolicy_S1_N_TC_07_011_014
 * @subsection PmSequence_TC_07_011_014_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSequence_TC_07_011_014_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmSequence_TC_07_011_014_tcs Test Case Summary
 * This test shall verify the functionality of the PM sequence call when using with R_OSAL_PmSetPolicy function in normal condition:
 * - Test07_011_014: set policy to OSAL_PM_POLICY_CG during REQUIRED, APPLY reset then change to RELEASED.
 * @subsection PmSequence_TC_07_011_014_tpts Test Procedure/Test Steps
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_CG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Set reset state to OSAL_PM_RESET_STATE_APPLIED by using R_OSAL_PmSetResetState
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Get current power policy by using R_OSAL_PmGetPolicy
 * @subsection PmSequence_TC_07_011_014_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_RESET.
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_CG
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_OK.<br>
 * Value of "policy" is equal to OSAL_PM_POLICY_CG
 * @subsection PmSequence_TC_07_011_014_testcase Test Case Example
 * @snippet test_pm.cpp PmSequence_TC_07_011_014
 */
// [PmSequence_TC_07_011_014]
TEST_F(PMApiSequenceTest, PmSequence_TC_07_011_014)
{
    e_osal_pm_policy_t policy = OSAL_PM_POLICY_INVALID;
    bool applyImmediate = true;
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;

    // @brief: normal case: set policy to OSAL_PM_POLICY_CG during REQUIRED, APPLY reset then change to RELEASED.
    // Test07_011_014
    //REQUIRED
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK) << "Test07_011_014 failed";
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_CG, applyImmediate), OSAL_RETURN_OK) << "Test07_011_014 failed";
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle1, OSAL_PM_RESET_STATE_APPLIED), OSAL_RETURN_OK) << "Test07_011_014 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_014 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_RESET) << "Test07_011_014 failed";

    //RELEASED
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_011_014 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_014 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_CG) << "Test07_011_014 failed";
    EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle1, &policy), OSAL_RETURN_OK) << "Test07_011_014 failed";
    EXPECT_EQ(policy, OSAL_PM_POLICY_CG) << "Test07_011_014 failed";
}
// [PmSequence_TC_07_011_014]

/**
 * @page PM_TEST Power API Test
 * @section PMApiSequenceTest_PmSequence_TC_07_011_015 PMApiSequenceTest.PmSequence_TC_07_011_015
 * @subsection PmSequence_TC_07_011_015_tsi Test Suite ID
 * PMApiSequenceTest
 * @subsection PmSequence_TC_07_011_015_tci Test Case ID
 * PmSetPolicy_S1_N_TC_07_011_015
 * @subsection PmSequence_TC_07_011_015_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSequence_TC_07_011_015_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmSequence_TC_07_011_015_tcs Test Case Summary
 * This test shall verify the functionality of the PM sequence call when using with R_OSAL_PmSetPolicy function in normal condition:
 * - Test07_011_015: set policy to OSAL_PM_POLICY_HP during REQUIRED, RELEASE reset then change to RELEASED
 * @subsection PmSequence_TC_07_011_015_tpts Test Procedure/Test Steps
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_HP by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Set reset state to OSAL_PM_RESET_STATE_RELEASED by using R_OSAL_PmSetResetState
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Get current power policy by using R_OSAL_PmGetPolicy
 * -# Write "PM_WRITE32_VALUE" to Target test device with offset "PM_WRITE32_REG_OFFSET" by using R_OSAL_IoWrite32.
 * -# Read the register of Target test device with offset "PM_WRITE32_REG_OFFSET" by using R_OSAL_IoRead32.
 * @subsection PmSequence_TC_07_011_015_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_READY.
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_ENABLED
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_OK.<br>
 * Value of "policy" is equal to OSAL_PM_POLICY_HP
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead32 return OSAL_RETURN_OK.<br>
 * The written value and the read value are the same.
 * @subsection PmSequence_TC_07_011_015_testcase Test Case Example
 * @snippet test_pm.cpp PmSequence_TC_07_011_015
 */
// [PmSequence_TC_07_011_015]
TEST_F(PMApiSequenceTest, PmSequence_TC_07_011_015)
{
    e_osal_pm_policy_t policy = OSAL_PM_POLICY_INVALID;
    bool applyImmediate = true;
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;
    uintptr_t offset = PM_WRITE32_REG_OFFSET;
    uint32_t read_data32 = 0;
    uint32_t write_data32 = PM_WRITE32_VALUE;

    // @brief: normal case: set policy to OSAL_PM_POLICY_HP during REQUIRED, RELEASE reset then change to RELEASED
    // Test07_011_015
    //REQUIRED
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK) << "Test07_011_015 failed";
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_HP, applyImmediate), OSAL_RETURN_OK) << "Test07_011_015 failed";
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle1, OSAL_PM_RESET_STATE_RELEASED), OSAL_RETURN_OK) << "Test07_011_015 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_015 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_READY) << "Test07_011_015 failed";

    //RELEASED
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_011_002 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_015 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_ENABLED) << "Test07_011_015 failed";
    EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle1, &policy), OSAL_RETURN_OK) << "Test07_011_015 failed";
    EXPECT_EQ(policy, OSAL_PM_POLICY_HP) << "Test07_011_015 failed";
    EXPECT_EQ(R_OSAL_IoWrite32(device_handle1, offset, write_data32), OSAL_RETURN_OK) << "Test07_011_002 failed";
    EXPECT_EQ(R_OSAL_IoRead32(device_handle1, offset, &read_data32), OSAL_RETURN_OK) << "Test07_011_002 failed";
    EXPECT_EQ(read_data32, write_data32) << "Test07_011_002 failed";
}
// [PmSequence_TC_07_011_015]

/**
 * @page PM_TEST Power API Test
 * @section PMApiSequenceTest_PmSequence_TC_07_011_016 PMApiSequenceTest.PmSequence_TC_07_011_016
 * @subsection PmSequence_TC_07_011_016_tsi Test Suite ID
 * PMApiSequenceTest
 * @subsection PmSequence_TC_07_011_016_tci Test Case ID
 * PmSetPolicy_S1_N_TC_07_011_016
 * @subsection PmSequence_TC_07_011_016_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSequence_TC_07_011_016_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmSequence_TC_07_011_016_tcs Test Case Summary
 * This test shall verify the functionality of the PM sequence call when using with R_OSAL_PmSetPolicy function in normal condition:
 * - Test07_011_016: set policy to OSAL_PM_POLICY_CG during REQUIRED, RELEASE reset then change to RELEASED
 * @subsection PmSequence_TC_07_011_016_tpts Test Procedure/Test Steps
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_HP by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Set reset state to OSAL_PM_RESET_STATE_RELEASED by using R_OSAL_PmSetResetState
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Get current power policy by using R_OSAL_PmGetPolicy
 * @subsection PmSequence_TC_07_011_016_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_READY.
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_ENABLED
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_OK.<br>
 * Value of "policy" is equal to OSAL_PM_POLICY_HP
 * @subsection PmSequence_TC_07_011_016_testcase Test Case Example
 * @snippet test_pm.cpp PmSequence_TC_07_011_016
 */
// [PmSequence_TC_07_011_016]
TEST_F(PMApiSequenceTest, PmSequence_TC_07_011_016)
{
    e_osal_pm_policy_t policy = OSAL_PM_POLICY_INVALID;
    bool applyImmediate = true;
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;

    // @brief: normal case: set policy to OSAL_PM_POLICY_CG during REQUIRED, RELEASE reset then change to RELEASED
    // Test07_011_016
    //REQUIRED
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK) << "Test07_011_016 failed";
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_CG, applyImmediate), OSAL_RETURN_OK) << "Test07_011_016 failed";
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle1, OSAL_PM_RESET_STATE_RELEASED), OSAL_RETURN_OK) << "Test07_011_016 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_016 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_READY) << "Test07_011_016 failed";

    //RELEASED
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_011_016 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_016 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_CG) << "Test07_011_016 failed";
    EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle1, &policy), OSAL_RETURN_OK) << "Test07_011_016 failed";
    EXPECT_EQ(policy, OSAL_PM_POLICY_CG) << "Test07_011_016 failed";
}
// [PmSequence_TC_07_011_016]

/**
 * @page PM_TEST Power API Test
 * @section PMApiSequenceTest_PmSequence_TC_07_011_017 PMApiSequenceTest.PmSequence_TC_07_011_017
 * @subsection PmSequence_TC_07_011_017_tsi Test Suite ID
 * PMApiSequenceTest
 * @subsection PmSequence_TC_07_011_017_tci Test Case ID
 * PmSetPolicy_S1_N_TC_07_011_017
 * @subsection PmSequence_TC_07_011_017_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSequence_TC_07_011_017_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmSequence_TC_07_011_017_tcs Test Case Summary
 * This test shall verify the functionality of the PM sequence call when using with R_OSAL_PmSetPolicy function in normal condition:
 * - Test07_011_017: set policy to OSAL_PM_POLICY_PG during REQUIRED, RELEASE reset then change to RELEASED
 * @subsection PmSequence_TC_07_011_017_tpts Test Procedure/Test Steps
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Set reset state to OSAL_PM_RESET_STATE_RELEASED by using R_OSAL_PmSetResetState
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Get current power policy by using R_OSAL_PmGetPolicy
 * @subsection PmSequence_TC_07_011_017_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_READY.
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_CG
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_OK.<br>
 * Value of "policy" is equal to OSAL_PM_POLICY_PG
 * @subsection PmSequence_TC_07_011_017_testcase Test Case Example
 * @snippet test_pm.cpp PmSequence_TC_07_011_017
 */
// [PmSequence_TC_07_011_017]
TEST_F(PMApiSequenceTest, PmSequence_TC_07_011_017)
{
    e_osal_pm_policy_t policy = OSAL_PM_POLICY_INVALID;
    bool applyImmediate = true;
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;

    // @brief: normal case: set policy to OSAL_PM_POLICY_PG during REQUIRED, RELEASE reset then change to RELEASED
    // Test07_011_017
    //REQUIRED
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK) << "Test07_011_017 failed";
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK) << "Test07_011_017 failed";
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle1, OSAL_PM_RESET_STATE_RELEASED), OSAL_RETURN_OK) << "Test07_011_017 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_017 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_READY) << "Test07_011_017 failed";

    //RELEASED
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_011_017 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_017 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_CG) << "Test07_011_017 failed";
    EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle1, &policy), OSAL_RETURN_OK) << "Test07_011_017 failed";
    EXPECT_EQ(policy, OSAL_PM_POLICY_PG) << "Test07_011_017 failed";
}
// [PmSequence_TC_07_011_017]

/**
 * @page PM_TEST Power API Test
 * @section PMApiSequenceTest_PmSequence_TC_07_011_018 PMApiSequenceTest.PmSequence_TC_07_011_018
 * @subsection PmSequence_TC_07_011_018_tsi Test Suite ID
 * PMApiSequenceTest
 * @subsection PmSequence_TC_07_011_018_tci Test Case ID
 * PmSetPolicy_S1_N_TC_07_011_018
 * @subsection PmSequence_TC_07_011_018_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSequence_TC_07_011_018_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmSequence_TC_07_011_018_tcs Test Case Summary
 * This test shall verify the functionality of the PM sequence call when using with R_OSAL_PmSetPolicy function in normal condition:
 * - Test07_011_018: set policy to OSAL_PM_POLICY_CG during RELEASED, APPLY reset then change to REQUIRED
 * @subsection PmSequence_TC_07_011_018_tpts Test Procedure/Test Steps
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_CG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Set reset state to OSAL_PM_RESET_STATE_APPLIED by using R_OSAL_PmSetResetState
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Get current power policy by using R_OSAL_PmGetPolicy
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * @subsection PmSequence_TC_07_011_018_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_CG.
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_RESET
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_OK.<br>
 * Value of "policy" is equal to OSAL_PM_POLICY_CG
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * @subsection PmSequence_TC_07_011_018_testcase Test Case Example
 * @snippet test_pm.cpp PmSequence_TC_07_011_018
 */
// [PmSequence_TC_07_011_018]
TEST_F(PMApiSequenceTest, PmSequence_TC_07_011_018)
{
    e_osal_pm_policy_t policy = OSAL_PM_POLICY_INVALID;
    bool applyImmediate = true;
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;

    // @brief: normal case: set policy to OSAL_PM_POLICY_CG during RELEASED, APPLY reset then change to REQUIRED
    // Test07_011_018
    //RELEASED
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_011_018 failed";
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_CG, applyImmediate), OSAL_RETURN_OK) << "Test07_011_018 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_018 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_CG) << "Test07_011_018 failed";
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle1, OSAL_PM_RESET_STATE_APPLIED), OSAL_RETURN_OK) << "Test07_011_018 failed";

    //REQUIRED
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK) << "Test07_011_018 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_018 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_RESET) << "Test07_011_018 failed";
    EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle1, &policy), OSAL_RETURN_OK) << "Test07_011_018 failed";
    EXPECT_EQ(policy, OSAL_PM_POLICY_CG) << "Test07_011_018 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, false), OSAL_RETURN_OK);
}
// [PmSequence_TC_07_011_018]

/**
 * @page PM_TEST Power API Test
 * @section PMApiSequenceTest_PmSequence_TC_07_011_019 PMApiSequenceTest.PmSequence_TC_07_011_019
 * @subsection PmSequence_TC_07_011_019_tsi Test Suite ID
 * PMApiSequenceTest
 * @subsection PmSequence_TC_07_011_019_tci Test Case ID
 * PmSetPolicy_S1_N_TC_07_011_019
 * @subsection PmSequence_TC_07_011_019_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSequence_TC_07_011_019_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmSequence_TC_07_011_019_tcs Test Case Summary
 * This test shall verify the functionality of the PM sequence call when using with R_OSAL_PmSetPolicy function in normal condition:
 * - Test07_011_019: set policy to OSAL_PM_POLICY_HP during RELEASED, RELEASE reset then change to REQUIRED
 * @subsection PmSequence_TC_07_011_019_tpts Test Procedure/Test Steps
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_HP by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Set reset state to OSAL_PM_RESET_STATE_RELEASED by using R_OSAL_PmSetResetState
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Get current power policy by using R_OSAL_PmGetPolicy
 * -# Write "PM_WRITE32_VALUE" to Target test device with offset "PM_WRITE32_REG_OFFSET" by using R_OSAL_IoWrite32.
 * -# Read the register of Target test device with offset "PM_WRITE32_REG_OFFSET" by using R_OSAL_IoRead32.
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * @subsection PmSequence_TC_07_011_019_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_ENABLED.
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_READY
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_OK.<br>
 * Value of "policy" is equal to OSAL_PM_POLICY_HP
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead32 return OSAL_RETURN_OK.<br>
 * The written value and the read value are the same.
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * @subsection PmSequence_TC_07_011_019_testcase Test Case Example
 * @snippet test_pm.cpp PmSequence_TC_07_011_019
 */
// [PmSequence_TC_07_011_019]
TEST_F(PMApiSequenceTest, PmSequence_TC_07_011_019)
{
    e_osal_pm_policy_t policy = OSAL_PM_POLICY_INVALID;
    bool applyImmediate = true;
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;
    uintptr_t offset = PM_WRITE32_REG_OFFSET;
    uint32_t read_data32 = 0;
    uint32_t write_data32 = PM_WRITE32_VALUE;

    // @brief: normal case: set policy to OSAL_PM_POLICY_HP during RELEASED, RELEASE reset then change to REQUIRED
    // Test07_011_019
    //RELEASED
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_011_019 failed";
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_HP, applyImmediate), OSAL_RETURN_OK) << "Test07_011_019 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_019 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_ENABLED) << "Test07_011_019 failed";
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle1, OSAL_PM_RESET_STATE_RELEASED), OSAL_RETURN_OK) << "Test07_011_019 failed";

    //REQUIRED
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK) << "Test07_011_019 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_019 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_READY) << "Test07_011_019 failed";
    EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle1, &policy), OSAL_RETURN_OK) << "Test07_011_019 failed";
    EXPECT_EQ(policy, OSAL_PM_POLICY_HP) << "Test07_011_019 failed";
    EXPECT_EQ(R_OSAL_IoWrite32(device_handle1, offset, write_data32), OSAL_RETURN_OK) << "Test07_011_002 failed";
    EXPECT_EQ(R_OSAL_IoRead32(device_handle1, offset, &read_data32), OSAL_RETURN_OK) << "Test07_011_002 failed";
    EXPECT_EQ(read_data32, write_data32) << "Test07_011_002 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, false), OSAL_RETURN_OK);
}
// [PmSequence_TC_07_011_019]

/**
 * @page PM_TEST Power API Test
 * @section PMApiSequenceTest_PmSequence_TC_07_011_020 PMApiSequenceTest.PmSequence_TC_07_011_020
 * @subsection PmSequence_TC_07_011_020_tsi Test Suite ID
 * PMApiSequenceTest
 * @subsection PmSequence_TC_07_011_020_tci Test Case ID
 * PmSetPolicy_S1_N_TC_07_011_020
 * @subsection PmSequence_TC_07_011_020_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSequence_TC_07_011_020_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmSequence_TC_07_011_020_tcs Test Case Summary
 * This test shall verify the functionality of the PM sequence call when using with R_OSAL_PmSetPolicy function in normal condition:
 * - Test07_011_020: set policy to OSAL_PM_POLICY_CG during RELEASED, RELEASE reset then change to REQUIRED
 * @subsection PmSequence_TC_07_011_020_tpts Test Procedure/Test Steps
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_CG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Set reset state to OSAL_PM_RESET_STATE_RELEASED by using R_OSAL_PmSetResetState
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Get current power policy by using R_OSAL_PmGetPolicy
 * -# Write "PM_WRITE32_VALUE" to Target test device with offset "PM_WRITE32_REG_OFFSET" by using R_OSAL_IoWrite32.
 * -# Read the register of Target test device with offset "PM_WRITE32_REG_OFFSET" by using R_OSAL_IoRead32.
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * @subsection PmSequence_TC_07_011_020_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_CG.
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_READY
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_OK.<br>
 * Value of "policy" is equal to OSAL_PM_POLICY_CG
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead32 return OSAL_RETURN_OK.<br>
 * The written value and the read value are the same.
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * @subsection PmSequence_TC_07_011_020_testcase Test Case Example
 * @snippet test_pm.cpp PmSequence_TC_07_011_020
 */
// [PmSequence_TC_07_011_020]
TEST_F(PMApiSequenceTest, PmSequence_TC_07_011_020)
{
    e_osal_pm_policy_t policy = OSAL_PM_POLICY_INVALID;
    bool applyImmediate = true;
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;
    uintptr_t offset = PM_WRITE32_REG_OFFSET;
    uint32_t read_data32 = 0;
    uint32_t write_data32 = PM_WRITE32_VALUE;

    // @brief: normal case: set policy to OSAL_PM_POLICY_CG during RELEASED, RELEASE reset then change to REQUIRED
    // Test07_011_020
    //RELEASED
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_011_020 failed";
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_CG, applyImmediate), OSAL_RETURN_OK) << "Test07_011_020 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_020 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_CG) << "Test07_011_020 failed";
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle1, OSAL_PM_RESET_STATE_RELEASED), OSAL_RETURN_OK) << "Test07_011_020 failed";

    //REQUIRED
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK) << "Test07_011_020 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_020 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_READY) << "Test07_011_020 failed";
    EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle1, &policy), OSAL_RETURN_OK) << "Test07_011_020 failed";
    EXPECT_EQ(policy, OSAL_PM_POLICY_CG) << "Test07_011_020 failed";
    EXPECT_EQ(R_OSAL_IoWrite32(device_handle1, offset, write_data32), OSAL_RETURN_OK) << "Test07_011_002 failed";
    EXPECT_EQ(R_OSAL_IoRead32(device_handle1, offset, &read_data32), OSAL_RETURN_OK) << "Test07_011_002 failed";
    EXPECT_EQ(read_data32, write_data32) << "Test07_011_002 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, false), OSAL_RETURN_OK);
}
// [PmSequence_TC_07_011_020]

/**
 * @page PM_TEST Power API Test
 * @section PMApiSequenceTest_PmSequence_TC_07_011_021 PMApiSequenceTest.PmSequence_TC_07_011_021
 * @subsection PmSequence_TC_07_011_021_tsi Test Suite ID
 * PMApiSequenceTest
 * @subsection PmSequence_TC_07_011_021_tci Test Case ID
 * PmSetPolicy_S1_N_TC_07_011_021
 * @subsection PmSequence_TC_07_011_021_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSequence_TC_07_011_021_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmSequence_TC_07_011_021_tcs Test Case Summary
 * This test shall verify the functionality of the PM sequence call when using with R_OSAL_PmSetPolicy function in normal condition:
 * - Test07_011_021: set policy to OSAL_PM_POLICY_PG during RELEASED, RELEASE reset then change to REQUIRED
 * @subsection PmSequence_TC_07_011_021_tpts Test Procedure/Test Steps
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Set reset state to OSAL_PM_RESET_STATE_RELEASED by using R_OSAL_PmSetResetState
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Get current power policy by using R_OSAL_PmGetPolicy
 * -# Write "PM_WRITE32_VALUE" to Target test device with offset "PM_WRITE32_REG_OFFSET" by using R_OSAL_IoWrite32.
 * -# Read the register of Target test device with offset "PM_WRITE32_REG_OFFSET" by using R_OSAL_IoRead32.
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * @subsection PmSequence_TC_07_011_021_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_CG.
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_READY
 * -# R_OSAL_PmGetPolicy return OSAL_RETURN_OK.<br>
 * Value of "policy" is equal to OSAL_PM_POLICY_PG
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead32 return OSAL_RETURN_OK.<br>
 * The written value and the read value are the same.
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * @subsection PmSequence_TC_07_011_021_testcase Test Case Example
 * @snippet test_pm.cpp PmSequence_TC_07_011_021
 */
// [PmSequence_TC_07_011_021]
TEST_F(PMApiSequenceTest, PmSequence_TC_07_011_021)
{
    e_osal_pm_policy_t policy = OSAL_PM_POLICY_INVALID;
    bool applyImmediate = true;
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;
    uintptr_t offset = PM_WRITE32_REG_OFFSET;
    uint32_t read_data32 = 0;
    uint32_t write_data32 = PM_WRITE32_VALUE;

    // @brief: normal case: set policy to OSAL_PM_POLICY_PG during RELEASED, RELEASE reset then change to REQUIRED
    // Test07_011_021
    //REQUIRED
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_011_021 failed";
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle1, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK) << "Test07_011_021 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_021 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_CG) << "Test07_011_021 failed";
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle1, OSAL_PM_RESET_STATE_RELEASED), OSAL_RETURN_OK) << "Test07_011_021 failed";

    //RELEASED
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK) << "Test07_011_021 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle1, &state), OSAL_RETURN_OK) << "Test07_011_021 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_READY) << "Test07_011_021 failed";
    EXPECT_EQ(R_OSAL_PmGetPolicy(device_handle1, &policy), OSAL_RETURN_OK) << "Test07_011_021 failed";
    EXPECT_EQ(policy, OSAL_PM_POLICY_PG) << "Test07_011_021 failed";
    EXPECT_EQ(R_OSAL_IoWrite32(device_handle1, offset, write_data32), OSAL_RETURN_OK) << "Test07_011_002 failed";
    EXPECT_EQ(R_OSAL_IoRead32(device_handle1, offset, &read_data32), OSAL_RETURN_OK) << "Test07_011_002 failed";
    EXPECT_EQ(read_data32, write_data32) << "Test07_011_002 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, false), OSAL_RETURN_OK);
}
// [PmSequence_TC_07_011_021]

#if !defined(OSAL_SOC_S4) /* R-Car S4 doesn't have PG device */
/**
 * @page PM_TEST Power API Test
 * @section PMApiSequenceTest_PmSequence_TC_07_011_022 PMApiSequenceTest.PmSequence_TC_07_011_022
 * @subsection PmSequence_TC_07_011_022_tsi Test Suite ID
 * PMApiSequenceTest
 * @subsection PmSequence_TC_07_011_022_tci Test Case ID
 * PmSetPolicy_S1_N_TC_07_011_022
 * @subsection PmSequence_TC_07_011_022_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSequence_TC_07_011_022_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmSequence_TC_07_011_022_tcs Test Case Summary
 * This test shall verify the functionality of the PM sequence call when using with R_OSAL_PmSetPolicy function in normal condition:
 * - Test07_011_022: set policy to OSAL_PM_POLICY_PG during REQUIRED state
 * @subsection PmSequence_TC_07_011_022_tpts Test Procedure/Test Steps
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Set reset state to OSAL_PM_RESET_STATE_RELEASED by using R_OSAL_PmSetResetState
 * -# Get current power state by using R_OSAL_PmGetState
 * @subsection PmSequence_TC_07_011_022_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_READY.
 * @subsection PmSequence_TC_07_011_022_testcase Test Case Example
 * @snippet test_pm.cpp PmSequence_TC_07_011_022
 */
// [PmSequence_TC_07_011_022]
TEST_F(PMApiSequenceTest, PmSequence_TC_07_011_022)
{
    bool applyImmediate = true;
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;

    // @brief: normal case: set policy to OSAL_PM_POLICY_PG during REQUIRED
    // Test07_011_022
    //REQUIRED
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK) << "Test07_011_022 failed";
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle2, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK) << "Test07_011_022 failed";
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle2, OSAL_PM_RESET_STATE_RELEASED), OSAL_RETURN_OK) << "Test07_011_022 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle2, &state), OSAL_RETURN_OK) << "Test07_011_022 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_READY) << "Test07_011_022 failed";
}
// [PmSequence_TC_07_011_022]

/**
 * @page PM_TEST Power API Test
 * @section PMApiSequenceTest_PmSequence_TC_07_011_023 PMApiSequenceTest.PmSequence_TC_07_011_023
 * @subsection PmSequence_TC_07_011_023_tsi Test Suite ID
 * PMApiSequenceTest
 * @subsection PmSequence_TC_07_011_023_tci Test Case ID
 * PmSetPolicy_S1_N_TC_07_011_023
 * @subsection PmSequence_TC_07_011_023_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSequence_TC_07_011_023_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmSequence_TC_07_011_023_tcs Test Case Summary
 * This test shall verify the functionality of the PM sequence call when using with R_OSAL_PmSetPolicy function in normal condition:
 * - Test07_011_023: set policy to OSAL_PM_POLICY_HP then OSAL_PM_POLICY_PG during RELEASED state
 * @subsection PmSequence_TC_07_011_023_tpts Test Procedure/Test Steps
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_HP by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Set power policy of the dependence device to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Set power required state of the dependence device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * @subsection PmSequence_TC_07_011_023_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_ENABLED.
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_PG.
 * @subsection PmSequence_TC_07_011_023_testcase Test Case Example
 * @snippet test_pm.cpp PmSequence_TC_07_011_023
 */
// [PmSequence_TC_07_011_023]
TEST_F(PMApiSequenceTest, PmSequence_TC_07_011_023)
{
    bool applyImmediate = true;
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;

    // @brief: normal case: set policy to OSAL_PM_POLICY_HP then OSAL_PM_POLICY_PG during RELEASED
    // Test07_011_023
    //RELEASED
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_011_023 failed";
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle2, OSAL_PM_POLICY_HP, applyImmediate), OSAL_RETURN_OK) << "Test07_011_023 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle2, &state), OSAL_RETURN_OK) << "Test07_011_023 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_ENABLED) << "Test07_011_023 failed";
    //Set policy OSAL_PM_POLICY_PG + OSAL_PM_REQUIRED_STATE_RELEASED for dependence device
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle2_dependence, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK) << "Test07_011_023 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle2_dependence, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_011_023 failed";
    // Set policy OSAL_PM_POLICY_PG for target device
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle2, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK) << "Test07_011_023 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle2, &state), OSAL_RETURN_OK) << "Test07_011_023 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_PG) << "Test07_011_023 failed";
}
// [PmSequence_TC_07_011_023]

/**
 * @page PM_TEST Power API Test
 * @section PMApiSequenceTest_PmSequence_TC_07_011_024 PMApiSequenceTest.PmSequence_TC_07_011_024
 * @subsection PmSequence_TC_07_011_024_tsi Test Suite ID
 * PMApiSequenceTest
 * @subsection PmSequence_TC_07_011_024_tci Test Case ID
 * PmSetPolicy_S1_N_TC_07_011_024
 * @subsection PmSequence_TC_07_011_024_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSequence_TC_07_011_024_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmSequence_TC_07_011_024_tcs Test Case Summary
 * This test shall verify the functionality of the PM sequence call when using with R_OSAL_PmSetPolicy function in normal condition:
 * - Test07_011_024: set policy to OSAL_PM_POLICY_CG then OSAL_PM_POLICY_PG during RELEASED state
 * @subsection PmSequence_TC_07_011_024_tpts Test Procedure/Test Steps
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_CG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Set power policy of the dependence device to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Set power required state of the dependence device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * @subsection PmSequence_TC_07_011_024_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_CG.
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_PG.
 * @subsection PmSequence_TC_07_011_024_testcase Test Case Example
 * @snippet test_pm.cpp PmSequence_TC_07_011_024
 */
// [PmSequence_TC_07_011_024]
TEST_F(PMApiSequenceTest, PmSequence_TC_07_011_024)
{
    bool applyImmediate = true;
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;

    // @brief: normal case: set policy to OSAL_PM_POLICY_CG then OSAL_PM_POLICY_PG during RELEASED
    // Test07_011_024
    //RELEASED
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_011_024 failed";
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle2, OSAL_PM_POLICY_CG, applyImmediate), OSAL_RETURN_OK) << "Test07_011_024 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle2, &state), OSAL_RETURN_OK) << "Test07_011_024 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_CG) << "Test07_011_024 failed";
    //Set policy OSAL_PM_POLICY_PG + OSAL_PM_REQUIRED_STATE_RELEASED for dependence device
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle2_dependence, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK) << "Test07_011_024 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle2_dependence, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_011_024 failed";
    // Set policy OSAL_PM_POLICY_PG for target device
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle2, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK) << "Test07_011_024 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle2, &state), OSAL_RETURN_OK) << "Test07_011_024 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_PG) << "Test07_011_024 failed";
}
// [PmSequence_TC_07_011_024]


/**
 * @page PM_TEST Power API Test
 * @section PMApiSequenceTest_PmSequence_TC_07_011_025 PMApiSequenceTest.PmSequence_TC_07_011_025
 * @subsection PmSequence_TC_07_011_025_tsi Test Suite ID
 * PMApiSequenceTest
 * @subsection PmSequence_TC_07_011_025_tci Test Case ID
 * PmSetPolicy_S1_N_TC_07_011_025
 * @subsection PmSequence_TC_07_011_025_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSequence_TC_07_011_025_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmSequence_TC_07_011_025_tcs Test Case Summary
 * This test shall verify the functionality of the PM sequence call when using with R_OSAL_PmSetPolicy function in normal condition:
 * - Test07_011_025: set policy to OSAL_PM_POLICY_PG during RELEASED state
 * @subsection PmSequence_TC_07_011_025_tpts Test Procedure/Test Steps
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Set power policy of the dependence device to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Set power required state of the dependence device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * @subsection PmSequence_TC_07_011_025_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_PG.
 * @subsection PmSequence_TC_07_011_025_testcase Test Case Example
 * @snippet test_pm.cpp PmSequence_TC_07_011_025
 */
// [PmSequence_TC_07_011_025]
TEST_F(PMApiSequenceTest, PmSequence_TC_07_011_025)
{
    bool applyImmediate = true;
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;

    // @brief: normal case: set policy to OSAL_PM_POLICY_PG during RELEASED
    // Test07_011_025
    //RELEASED
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_011_025 failed";
    //Set policy OSAL_PM_POLICY_PG + OSAL_PM_REQUIRED_STATE_RELEASED for dependence device
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle2_dependence, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK) << "Test07_011_025 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle2_dependence, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_011_025 failed";
    // Set policy OSAL_PM_POLICY_PG for target device
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle2, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK) << "Test07_011_025 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle2, &state), OSAL_RETURN_OK) << "Test07_011_025 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_PG) << "Test07_011_025 failed";
}
// [PmSequence_TC_07_011_025]

/**
 * @page PM_TEST Power API Test
 * @section PMApiSequenceTest_PmSequence_TC_07_011_026 PMApiSequenceTest.PmSequence_TC_07_011_026
 * @subsection PmSequence_TC_07_011_026_tsi Test Suite ID
 * PMApiSequenceTest
 * @subsection PmSequence_TC_07_011_026_tci Test Case ID
 * PmSetPolicy_S1_N_TC_07_011_026
 * @subsection PmSequence_TC_07_011_026_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSequence_TC_07_011_026_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmSequence_TC_07_011_026_tcs Test Case Summary
 * This test shall verify the functionality of the PM sequence call when using with R_OSAL_PmSetPolicy function in normal condition:
 * - Test07_011_026: set policy to OSAL_PM_POLICY_PG during REQUIRED state then change to RELEASED state
 * @subsection PmSequence_TC_07_011_026_tpts Test Procedure/Test Steps
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Set reset state to OSAL_PM_RESET_STATE_RELEASED by using R_OSAL_PmSetResetState
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Set power policy of the dependence device to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Set power required state of the dependence device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Get current power state by using R_OSAL_PmGetState
 * @subsection PmSequence_TC_07_011_026_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_READY.
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_PG.
 * @subsection PmSequence_TC_07_011_026_testcase Test Case Example
 * @snippet test_pm.cpp PmSequence_TC_07_011_026
 */
// [PmSequence_TC_07_011_026]
TEST_F(PMApiSequenceTest, PmSequence_TC_07_011_026)
{
    bool applyImmediate = true;
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;

    // @brief: normal case: set policy to OSAL_PM_POLICY_PG during REQUIRED state then change to RELEASED state
    // Test07_011_026
    //REQUIRED
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK) << "Test07_011_026 failed";
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle2, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK) << "Test07_011_026 failed";
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle2, OSAL_PM_RESET_STATE_RELEASED), OSAL_RETURN_OK) << "Test07_011_026 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle2, &state), OSAL_RETURN_OK) << "Test07_011_026 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_READY) << "Test07_011_026 failed";

    //RELEASED
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_011_026 failed";
    //Set policy OSAL_PM_POLICY_PG + OSAL_PM_REQUIRED_STATE_RELEASED for dependence device
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle2_dependence, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK) << "Test07_011_026 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle2_dependence, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_011_026 failed";

    EXPECT_EQ(R_OSAL_PmGetState(device_handle2, &state), OSAL_RETURN_OK) << "Test07_011_026 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_PG) << "Test07_011_026 failed";
}
// [PmSequence_TC_07_011_026]


/**
 * @page PM_TEST Power API Test
 * @section PMApiSequenceTest_PmSequence_TC_07_011_027 PMApiSequenceTest.PmSequence_TC_07_011_027
 * @subsection PmSequence_TC_07_011_027_tsi Test Suite ID
 * PMApiSequenceTest
 * @subsection PmSequence_TC_07_011_027_tci Test Case ID
 * PmSetPolicy_S1_N_TC_07_011_027
 * @subsection PmSequence_TC_07_011_027_rr Related Requirement(s)
 * \#239131, \#239139
 * @subsection PmSequence_TC_07_011_027_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - Target test device was opened.
 * @subsection PmSequence_TC_07_011_027_tcs Test Case Summary
 * This test shall verify the functionality of the PM sequence call when using with R_OSAL_PmSetPolicy function in normal condition:
 * - Test07_011_027: set policy to OSAL_PM_POLICY_PG during RELEASED state then change to REQUIRED state
 * @subsection PmSequence_TC_07_011_027_tpts Test Procedure/Test Steps
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Set power policy of the dependence device to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Set power required state of the dependence device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * -# Set power policy to OSAL_PM_POLICY_PG by using R_OSAL_PmSetPolicy with applyImmediate is true
 * -# Get current power state by using R_OSAL_PmGetState
 * -# Set power required state of Target test device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set reset state to OSAL_PM_RESET_STATE_RELEASED by using R_OSAL_PmSetResetState
 * -# Get current power state by using R_OSAL_PmGetState
 * @subsection PmSequence_TC_07_011_027_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetPolicy return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_PG.
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmGetState return OSAL_RETURN_OK.<br>
 * Value of "state" is equal to OSAL_PM_STATE_READY.
 * @subsection PmSequence_TC_07_011_027_testcase Test Case Example
 * @snippet test_pm.cpp PmSequence_TC_07_011_027
 */
// [PmSequence_TC_07_011_027]
TEST_F(PMApiSequenceTest, PmSequence_TC_07_011_027)
{
    bool applyImmediate = true;
    e_osal_pm_state_t state = OSAL_PM_STATE_INVALID;

    // @brief: normal case: set policy to OSAL_PM_POLICY_PG during RELEASED state then change to REQUIRED state
    // Test07_011_027
    //RELEASED
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_011_027 failed";
    //Set policy OSAL_PM_POLICY_PG + OSAL_PM_REQUIRED_STATE_RELEASED for dependence device
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle2_dependence, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK) << "Test07_011_027 failed";
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle2_dependence, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate), OSAL_RETURN_OK) << "Test07_011_027 failed";
    // Set policy OSAL_PM_POLICY_PG for target device
    EXPECT_EQ(R_OSAL_PmSetPolicy(device_handle2, OSAL_PM_POLICY_PG, applyImmediate), OSAL_RETURN_OK) << "Test07_011_027 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle2, &state), OSAL_RETURN_OK) << "Test07_011_027 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_PG) << "Test07_011_027 failed";

    //REQUIRED
    EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK) << "Test07_011_027 failed";
    EXPECT_EQ(R_OSAL_PmSetResetState(device_handle2, OSAL_PM_RESET_STATE_RELEASED), OSAL_RETURN_OK) << "Test07_011_027 failed";
    EXPECT_EQ(R_OSAL_PmGetState(device_handle2, &state), OSAL_RETURN_OK) << "Test07_011_027 failed";
    EXPECT_EQ(state, OSAL_PM_STATE_READY) << "Test07_011_027 failed";
}
// [PmSequence_TC_07_011_027]
#endif

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSoftwareReset_TC_07_012_001 PMApiTest.PmSoftwareReset_TC_07_012_001
 * @subsection PmSoftwareReset_TC_07_012_001_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSoftwareReset_TC_07_012_001_tci Test Case ID
 * PmSetResetState_S1_N_TC_07_012_001
 * @subsection PmSoftwareReset_TC_07_012_001_rr Related Requirement(s)
 * \#239133, \#239141, \#239142
 * @subsection PmSoftwareReset_TC_07_012_001_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSoftwareReset_TC_07_012_001_tcs Test Case Summary
 * This test shall verify the software reset functionality when using OSAL Power APIs in normal condition:
 * - Pre-process: set required state to OSAL_PM_REQUIRED_STATE_REQUIRED and reset state to OSAL_PM_RESET_STATE_RELEASED.
 * - Test07_012_001: confirm the functionality of software reset in read/write register with applyImmediate is true
 * @subsection PmSoftwareReset_TC_07_012_001_tpts Test Procedure/Test Steps
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set reset state to OSAL_PM_RESET_STATE_RELEASED by using R_OSAL_PmSetResetState
 * -# Wait for state OSAL_PM_STATE_READY by using R_OSAL_PmWaitForState
 * -# Read the initialized value of register with offset "PM_WRITE32_REG_OFFSET" and assign it to "init_value" by using R_OSAL_IoRead32
 * -# Write data with offset "PM_WRITE32_REG_OFFSET" by using R_OSAL_IoWrite32
 * -# Read data with offset "PM_WRITE32_REG_OFFSET" by using R_OSAL_IoRead32
 * -# Set reset state to OSAL_PM_RESET_STATE_APPLIED by using R_OSAL_PmSetResetState
 * -# Wait for state OSAL_PM_STATE_RESET by using R_OSAL_PmWaitForState
 * -# Set reset state to OSAL_PM_RESET_STATE_RELEASED by using R_OSAL_PmSetResetState
 * -# Wait for state OSAL_PM_STATE_READY by using R_OSAL_PmWaitForState
 * -# Read data with offset "PM_WRITE32_REG_OFFSET" by using R_OSAL_IoRead32
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_RELEASE by using R_OSAL_PmSetRequiredState
 * @subsection PmSoftwareReset_TC_07_012_001_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK
 * -# R_OSAL_IoRead32 return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_IoRead32 return OSAL_RETURN_OK.<br>
 * Value of "read_data32" is equal to "write_data32"
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK.
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead32 return OSAL_RETURN_OK.<br>
 * Value of "read_data32" is equal to "init_value"
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * @subsection PmSoftwareReset_TC_07_012_001_testcase Test Case Example
 * @snippet test_pm.cpp PmSoftwareReset_TC_07_012_001
 */
// [PmSoftwareReset_TC_07_012_001]
TEST_F(PMApiTest, PmSoftwareReset_TC_07_012_001)
{
        uint32_t read_data32;
        uint32_t write_data32 = PM_WRITE32_VALUE ;
        uint32_t init_value;
        uintptr_t offset = PM_WRITE32_REG_OFFSET ;
        bool applyImmediate = true;
        osal_milli_sec_t time = 1000;

        // pre-process: set required state to OSAL_PM_REQUIRED_STATE_REQUIRED
        EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK) << "pre-process failed";
        EXPECT_EQ(R_OSAL_PmSetResetState(device_handle1, OSAL_PM_RESET_STATE_RELEASED), OSAL_RETURN_OK) << "pre-process failed";
        EXPECT_EQ(R_OSAL_PmWaitForState(device_handle1, OSAL_PM_STATE_READY, time), OSAL_RETURN_OK) << "pre-process failed";

        // Test07_012_001
        // read the initialize value
        EXPECT_EQ(R_OSAL_IoRead32(device_handle1, offset, &init_value), OSAL_RETURN_OK) << "Test07_012_001 failed";

        // write/read the a valid value
        EXPECT_EQ(R_OSAL_IoWrite32(device_handle1, offset, write_data32), OSAL_RETURN_OK) << "Test07_012_001 failed";
        EXPECT_EQ(R_OSAL_IoRead32(device_handle1, offset, &read_data32), OSAL_RETURN_OK) << "Test07_012_001 failed";
        EXPECT_EQ(read_data32, write_data32) << "Test07_012_001 failed";

        // apply sw reset
        EXPECT_EQ(R_OSAL_PmSetResetState(device_handle1, OSAL_PM_RESET_STATE_APPLIED), OSAL_RETURN_OK) << "Test07_012_001 failed";
        EXPECT_EQ(R_OSAL_PmWaitForState(device_handle1, OSAL_PM_STATE_RESET, time), OSAL_RETURN_OK) << "Test07_012_001 failed";
        EXPECT_EQ(R_OSAL_PmSetResetState(device_handle1, OSAL_PM_RESET_STATE_RELEASED), OSAL_RETURN_OK) << "Test07_012_001 failed";
        EXPECT_EQ(R_OSAL_PmWaitForState(device_handle1, OSAL_PM_STATE_READY, time), OSAL_RETURN_OK) << "Test07_012_001 failed";

        // read back register after applying sw reset
        EXPECT_EQ(R_OSAL_IoRead32(device_handle1, offset, &read_data32), OSAL_RETURN_OK) << "Test07_012_001 failed";
        EXPECT_EQ(read_data32, init_value) << "Test07_012_001 failed";

        // post-process: set required state to OSAL_PM_REQUIRED_STATE_RELEASE
        EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, true), OSAL_RETURN_OK);
}
// [PmSoftwareReset_TC_07_012_001]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSoftwareReset_TC_07_012_002 PMApiTest.PmSoftwareReset_TC_07_012_002
 * @subsection PmSoftwareReset_TC_07_012_002_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSoftwareReset_TC_07_012_002_tci Test Case ID
 * PmSetResetState_S2_A_TC_07_012_002
 * @subsection PmSoftwareReset_TC_07_012_002_rr Related Requirement(s)
 * \#239133, \#239141, \#239142
 * @subsection PmSoftwareReset_TC_07_012_002_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSoftwareReset_TC_07_012_002_tcs Test Case Summary
 * This test shall verify functionality of IO Read/Write when not applying the software reset through OSAL Power APIs:
 * - Pre-process: set required state to OSAL_PM_REQUIRED_STATE_REQUIRED and reset state to OSAL_PM_RESET_STATE_RELEASED.
 * - Test07_012_002: confirm IO read/write functionality when not applying the software reset with applyImmediate is true
 * @subsection PmSoftwareReset_TC_07_012_002_tpts Test Procedure/Test Steps
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set reset state to OSAL_PM_RESET_STATE_RELEASED by using R_OSAL_PmSetResetState
 * -# Wait for state OSAL_PM_STATE_READY by using R_OSAL_PmWaitForState
 * -# Read the initialized value of register with offset "PM_WRITE32_REG_OFFSET" and assign it to "init_value" by using R_OSAL_IoRead32
 * -# Write data with offset "PM_WRITE32_REG_OFFSET" by using R_OSAL_IoWrite32
 * -# Read data with offset "PM_WRITE32_REG_OFFSET" by using R_OSAL_IoRead32
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_RELEASE by using R_OSAL_PmSetRequiredState
 * -# Close device by using R_OSAL_IoDeviceClose
 * -# Open device by using R_OSAL_IoDeviceOpen
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set reset state to OSAL_PM_RESET_STATE_RELEASED by using R_OSAL_PmSetResetState
 * -# Wait for state OSAL_PM_STATE_READY by using R_OSAL_PmWaitForState
 * -# Read data with offset "PM_WRITE32_REG_OFFSET"  by using R_OSAL_IoRead32
 * -# Set reset state to OSAL_PM_RESET_STATE_APPLIED by using R_OSAL_PmSetResetState
 * -# Wait for state OSAL_PM_STATE_RESET by using R_OSAL_PmWaitForState
 * -# Set reset state to OSAL_PM_RESET_STATE_RELEASED by using R_OSAL_PmSetResetState
 * -# Wait for state OSAL_PM_STATE_READY by using R_OSAL_PmWaitForState
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_RELEASE by using R_OSAL_PmSetRequiredState
 * @subsection PmSoftwareReset_TC_07_012_002_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK
 * -# R_OSAL_IoRead32 return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_IoRead32 return OSAL_RETURN_OK.<br>
 * Value of "read_data32" is equal to "write_data32"
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK
 * -# R_OSAL_IoRead32 return OSAL_RETURN_OK.<br>
 * Value of "read_data32" is equal to "write_data32".<br>
 * Value of "read_data32" is not equal to "init_value"
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK.
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK.
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * @subsection PmSoftwareReset_TC_07_012_002_testcase Test Case Example
 * @snippet test_pm.cpp PmSoftwareReset_TC_07_012_002
 */
// [PmSoftwareReset_TC_07_012_002]
TEST_F(PMApiTest, PmSoftwareReset_TC_07_012_002)
{
        uint32_t read_data32;
        uint32_t write_data32 = PM_WRITE32_VALUE ;
        uint32_t init_value;
        uintptr_t offset = PM_WRITE32_REG_OFFSET ;
        bool applyImmediate = true;
        osal_milli_sec_t time = 1000;

        // pre-process: set required state to OSAL_PM_REQUIRED_STATE_REQUIRED
        EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK) << "pre-process failed";
        EXPECT_EQ(R_OSAL_PmSetResetState(device_handle1, OSAL_PM_RESET_STATE_RELEASED), OSAL_RETURN_OK) << "pre-process failed";
        EXPECT_EQ(R_OSAL_PmWaitForState(device_handle1, OSAL_PM_STATE_READY, time), OSAL_RETURN_OK) << "pre-process failed";

        // Test07_012_002
        // read the initialize value
        EXPECT_EQ(R_OSAL_IoRead32(device_handle1, offset, &init_value), OSAL_RETURN_OK) << "Test07_012_002 failed";

        // write/read the a valid value
        EXPECT_EQ(R_OSAL_IoWrite32(device_handle1, offset, write_data32), OSAL_RETURN_OK) << "Test07_012_002 failed";
        EXPECT_EQ(R_OSAL_IoRead32(device_handle1, offset, &read_data32), OSAL_RETURN_OK) << "Test07_012_002 failed";
        EXPECT_EQ(read_data32, write_data32) << "Test07_012_002 failed";

        // close and re-open device
        EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, true), OSAL_RETURN_OK) << "Test07_012_002 failed";
        EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle1), OSAL_RETURN_OK) << "Test07_012_002 failed";
        EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_PM_DEVICE_1, &device_handle1), OSAL_RETURN_OK) << "Test07_012_002 failed";

        // read register after re-open
        EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK) << "Test07_012_002 failed";
        EXPECT_EQ(R_OSAL_PmSetResetState(device_handle1, OSAL_PM_RESET_STATE_RELEASED), OSAL_RETURN_OK) << "Test07_012_002 failed";
        EXPECT_EQ(R_OSAL_PmWaitForState(device_handle1, OSAL_PM_STATE_READY, time), OSAL_RETURN_OK) << "Test07_012_002 failed";
        EXPECT_EQ(R_OSAL_IoRead32(device_handle1, offset, &read_data32), OSAL_RETURN_OK) << "Test07_012_002 failed";
        EXPECT_EQ(read_data32, write_data32) << "Test07_012_002 failed";
        EXPECT_NE(read_data32, init_value) << "Test07_012_002 failed";

        // post-process: Apply sw reset and set required state to OSAL_PM_REQUIRED_STATE_RELEASE
        EXPECT_EQ(R_OSAL_PmSetResetState(device_handle1, OSAL_PM_RESET_STATE_APPLIED), OSAL_RETURN_OK) << "Test07_012_002 failed";
        EXPECT_EQ(R_OSAL_PmWaitForState(device_handle1, OSAL_PM_STATE_RESET, time), OSAL_RETURN_OK) << "Test07_012_002 failed";
        EXPECT_EQ(R_OSAL_PmSetResetState(device_handle1, OSAL_PM_RESET_STATE_RELEASED), OSAL_RETURN_OK) << "Test07_012_002 failed";
        EXPECT_EQ(R_OSAL_PmWaitForState(device_handle1, OSAL_PM_STATE_READY, time), OSAL_RETURN_OK) << "Test07_012_002 failed";

        EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, true), OSAL_RETURN_OK) << "Test07_012_002 failed";
}
// [PmSoftwareReset_TC_07_012_002]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSoftwareReset_TC_07_012_003 PMApiTest.PmSoftwareReset_TC_07_012_003
 * @subsection PmSoftwareReset_TC_07_012_003_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSoftwareReset_TC_07_012_003_tci Test Case ID
 * PmSetResetState_S1_N_TC_07_012_003
 * @subsection PmSoftwareReset_TC_07_012_003_rr Related Requirement(s)
 * \#239133, \#239141, \#239142
 * @subsection PmSoftwareReset_TC_07_012_003_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSoftwareReset_TC_07_012_003_tcs Test Case Summary
 * This test shall verify the software reset functionality when using OSAL Power APIs in normal condition:
 * - Pre-process: set required state to OSAL_PM_REQUIRED_STATE_REQUIRED and reset state to OSAL_PM_RESET_STATE_RELEASED.
 * - Test07_012_003: confirm the functionality of software reset in read/write register with applyImmediate is false
 * @subsection PmSoftwareReset_TC_07_012_003_tpts Test Procedure/Test Steps
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set reset state to OSAL_PM_RESET_STATE_RELEASED by using R_OSAL_PmSetResetState
 * -# Wait for state OSAL_PM_STATE_READY by using R_OSAL_PmWaitForState
 * -# Read the initialized value of register with offset "PM_WRITE32_REG_OFFSET" and assign it to "init_value" by using R_OSAL_IoRead32
 * -# Write data with offset "PM_WRITE32_REG_OFFSET" by using R_OSAL_IoWrite32
 * -# Read data with offset "PM_WRITE32_REG_OFFSET" by using R_OSAL_IoRead32
 * -# Set reset state to OSAL_PM_RESET_STATE_APPLIED by using R_OSAL_PmSetResetState
 * -# Wait for state OSAL_PM_STATE_RESET by using R_OSAL_PmWaitForState
 * -# Set reset state to OSAL_PM_RESET_STATE_RELEASED by using R_OSAL_PmSetResetState
 * -# Wait for state OSAL_PM_STATE_READY by using R_OSAL_PmWaitForState
 * -# Read data with offset "PM_WRITE32_REG_OFFSET" by using R_OSAL_IoRead32
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_RELEASE by using R_OSAL_PmSetRequiredState
 * @subsection PmSoftwareReset_TC_07_012_003_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK
 * -# R_OSAL_IoRead32 return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_IoRead32 return OSAL_RETURN_OK.<br>
 * Value of "read_data32" is equal to "write_data32"
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK.
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK.
 * -# R_OSAL_IoRead32 return OSAL_RETURN_OK.<br>
 * Value of "read_data32" is equal to "init_value"
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * @subsection PmSoftwareReset_TC_07_012_003 Test Case Example
 * @snippet test_pm.cpp PmSoftwareReset_TC_07_012_003
 */
// [PmSoftwareReset_TC_07_012_003]
TEST_F(PMApiTest, PmSoftwareReset_TC_07_012_003)
{
        uint32_t read_data32;
        uint32_t write_data32 = PM_WRITE32_VALUE ;
        uint32_t init_value;
        uintptr_t offset = PM_WRITE32_REG_OFFSET ;
        bool applyImmediate = false;
        osal_milli_sec_t time = 1000;

        // pre-process: set required state to OSAL_PM_REQUIRED_STATE_REQUIRED
        EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK) << "pre-process failed";
        EXPECT_EQ(R_OSAL_PmSetResetState(device_handle1, OSAL_PM_RESET_STATE_RELEASED), OSAL_RETURN_OK) << "pre-process failed";
        EXPECT_EQ(R_OSAL_PmWaitForState(device_handle1, OSAL_PM_STATE_READY, time), OSAL_RETURN_OK) << "pre-process failed";

        // Test07_012_003
        // read the initialize value
        EXPECT_EQ(R_OSAL_IoRead32(device_handle1, offset, &init_value), OSAL_RETURN_OK) << "Test07_012_003 failed";

        // write/read the a valid value
        EXPECT_EQ(R_OSAL_IoWrite32(device_handle1, offset, write_data32), OSAL_RETURN_OK) << "Test07_012_003 failed";
        EXPECT_EQ(R_OSAL_IoRead32(device_handle1, offset, &read_data32), OSAL_RETURN_OK) << "Test07_012_003 failed";
        EXPECT_EQ(read_data32, write_data32) << "Test07_012_003 failed";

        // apply sw reset
        EXPECT_EQ(R_OSAL_PmSetResetState(device_handle1, OSAL_PM_RESET_STATE_APPLIED), OSAL_RETURN_OK) << "Test07_012_003 failed";
        EXPECT_EQ(R_OSAL_PmWaitForState(device_handle1, OSAL_PM_STATE_RESET, time), OSAL_RETURN_OK) << "Test07_012_003 failed";
        EXPECT_EQ(R_OSAL_PmSetResetState(device_handle1, OSAL_PM_RESET_STATE_RELEASED), OSAL_RETURN_OK) << "Test07_012_003 failed";
        EXPECT_EQ(R_OSAL_PmWaitForState(device_handle1, OSAL_PM_STATE_READY, time), OSAL_RETURN_OK) << "Test07_012_003 failed";

        // read back register after applying sw reset
        EXPECT_EQ(R_OSAL_IoRead32(device_handle1, offset, &read_data32), OSAL_RETURN_OK) << "Test07_012_003 failed";
        EXPECT_EQ(read_data32, init_value) << "Test07_012_003 failed";

        // post-process: set required state to OSAL_PM_REQUIRED_STATE_RELEASE
        EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, true), OSAL_RETURN_OK);
}
// [PmSoftwareReset_TC_07_012_003]

/**
 * @page PM_TEST Power API Test
 * @section PMApiTest_PmSoftwareReset_TC_07_012_004 PMApiTest.PmSoftwareReset_TC_07_012_004
 * @subsection PmSoftwareReset_TC_07_012_004_tsi Test Suite ID
 * PMApiTest
 * @subsection PmSoftwareReset_TC_07_012_004_tci Test Case ID
 * PmSetResetState_S2_A_TC_07_012_004
 * @subsection PmSoftwareReset_TC_07_012_004_rr Related Requirement(s)
 * \#239133, \#239141, \#239142
 * @subsection PmSoftwareReset_TC_07_012_004_pp Prerequisite/Precondition
 * - OSAL is initialized.
 * - test target device was opened.
 * @subsection PmSoftwareReset_TC_07_012_004_tcs Test Case Summary
 * This test shall verify functionality of IO Read/Write when not applying the software reset through OSAL Power APIs:
 * - Pre-process: set required state to OSAL_PM_REQUIRED_STATE_REQUIRED and reset state to OSAL_PM_RESET_STATE_RELEASED.
 * - Test07_012_004: confirm IO read/write functionality when not applying the software reset with applyImmediate is false
 * @subsection PmSoftwareReset_TC_07_012_004_tpts Test Procedure/Test Steps
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set reset state to OSAL_PM_RESET_STATE_RELEASED by using R_OSAL_PmSetResetState
 * -# Wait for state OSAL_PM_STATE_READY by using R_OSAL_PmWaitForState
 * -# Read the initialized value of register with offset "PM_WRITE32_REG_OFFSET" and assign it to "init_value" by using R_OSAL_IoRead32
 * -# Write data with offset "PM_WRITE32_REG_OFFSET" by using R_OSAL_IoWrite32
 * -# Read data with offset "PM_WRITE32_REG_OFFSET" by using R_OSAL_IoRead32
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_RELEASE by using R_OSAL_PmSetRequiredState
 * -# Close device by using R_OSAL_IoDeviceClose
 * -# Open device by using R_OSAL_IoDeviceOpen
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * -# Set reset state to OSAL_PM_RESET_STATE_RELEASED by using R_OSAL_PmSetResetState
 * -# Wait for state OSAL_PM_STATE_READY by using R_OSAL_PmWaitForState
 * -# Read data with offset "PM_WRITE32_REG_OFFSET"  by using R_OSAL_IoRead32
 * -# Set reset state to OSAL_PM_RESET_STATE_APPLIED by using R_OSAL_PmSetResetState
 * -# Wait for state OSAL_PM_STATE_RESET by using R_OSAL_PmWaitForState
 * -# Set reset state to OSAL_PM_RESET_STATE_RELEASED by using R_OSAL_PmSetResetState
 * -# Wait for state OSAL_PM_STATE_READY by using R_OSAL_PmWaitForState
 * -# Set power required state to OSAL_PM_REQUIRED_STATE_RELEASE by using R_OSAL_PmSetRequiredState
 * @subsection PmSoftwareReset_TC_07_012_004_ereb Expected Result/Behavior
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK
 * -# R_OSAL_IoRead32 return OSAL_RETURN_OK
 * -# R_OSAL_IoWrite32 return OSAL_RETURN_OK
 * -# R_OSAL_IoRead32 return OSAL_RETURN_OK.<br>
 * Value of "read_data32" is equal to "write_data32"
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_IoDeviceClose return OSAL_RETURN_OK
 * -# R_OSAL_IoDeviceOpen return OSAL_RETURN_OK
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK
 * -# R_OSAL_IoRead32 return OSAL_RETURN_OK.<br>
 * Value of "read_data32" is equal to "write_data32".<br>
 * Value of "read_data32" is not equal to "init_value"
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK.
 * -# R_OSAL_PmSetResetState return OSAL_RETURN_OK
 * -# R_OSAL_PmWaitForState return OSAL_RETURN_OK.
 * -# R_OSAL_PmSetRequiredState return OSAL_RETURN_OK
 * @subsection PmSoftwareReset_TC_07_012_004_testcase Test Case Example
 * @snippet test_pm.cpp PmSoftwareReset_TC_07_012_004
 */
// [PmSoftwareReset_TC_07_012_004]
TEST_F(PMApiTest, PmSoftwareReset_TC_07_012_004)
{
        uint32_t read_data32;
        uint32_t write_data32 = PM_WRITE32_VALUE ;
        uint32_t init_value;
        uintptr_t offset = PM_WRITE32_REG_OFFSET ;
        bool applyImmediate = false;
        osal_milli_sec_t time = 1000;

        // pre-process: set required state to OSAL_PM_REQUIRED_STATE_REQUIRED
        EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK) << "pre-process failed";
        EXPECT_EQ(R_OSAL_PmSetResetState(device_handle1, OSAL_PM_RESET_STATE_RELEASED), OSAL_RETURN_OK) << "pre-process failed";
        EXPECT_EQ(R_OSAL_PmWaitForState(device_handle1, OSAL_PM_STATE_READY, time), OSAL_RETURN_OK) << "pre-process failed";

        // Test07_012_004
        // read the initialize value
        EXPECT_EQ(R_OSAL_IoRead32(device_handle1, offset, &init_value), OSAL_RETURN_OK) << "Test07_012_004 failed";

        // write/read the a valid value
        EXPECT_EQ(R_OSAL_IoWrite32(device_handle1, offset, write_data32), OSAL_RETURN_OK) << "Test07_012_004 failed";
        EXPECT_EQ(R_OSAL_IoRead32(device_handle1, offset, &read_data32), OSAL_RETURN_OK) << "Test07_012_004 failed";
        EXPECT_EQ(read_data32, write_data32) << "Test07_012_004 failed";

        // close and re-open device
        EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, true), OSAL_RETURN_OK) << "Test07_012_004 failed";
        EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle1), OSAL_RETURN_OK) << "Test07_012_004 failed";
        EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_PM_DEVICE_1, &device_handle1), OSAL_RETURN_OK) << "Test07_012_004 failed";

        // read register after re-open
        EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate), OSAL_RETURN_OK) << "Test07_012_004 failed";
        EXPECT_EQ(R_OSAL_PmSetResetState(device_handle1, OSAL_PM_RESET_STATE_RELEASED), OSAL_RETURN_OK) << "Test07_012_004 failed";
        EXPECT_EQ(R_OSAL_PmWaitForState(device_handle1, OSAL_PM_STATE_READY, time), OSAL_RETURN_OK) << "Test07_012_004 failed";
        EXPECT_EQ(R_OSAL_IoRead32(device_handle1, offset, &read_data32), OSAL_RETURN_OK) << "Test07_012_004 failed";
        EXPECT_EQ(read_data32, write_data32) << "Test07_012_004 failed";
        EXPECT_NE(read_data32, init_value) << "Test07_012_004 failed";

        // post-process: Apply sw reset and set required state to OSAL_PM_REQUIRED_STATE_RELEASE
        EXPECT_EQ(R_OSAL_PmSetResetState(device_handle1, OSAL_PM_RESET_STATE_APPLIED), OSAL_RETURN_OK) << "Test07_012_004 failed";
        EXPECT_EQ(R_OSAL_PmWaitForState(device_handle1, OSAL_PM_STATE_RESET, time), OSAL_RETURN_OK) << "Test07_012_004 failed";
        EXPECT_EQ(R_OSAL_PmSetResetState(device_handle1, OSAL_PM_RESET_STATE_RELEASED), OSAL_RETURN_OK) << "Test07_012_004 failed";
        EXPECT_EQ(R_OSAL_PmWaitForState(device_handle1, OSAL_PM_STATE_READY, time), OSAL_RETURN_OK) << "Test07_012_004 failed";

        EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle1, OSAL_PM_REQUIRED_STATE_RELEASED, true), OSAL_RETURN_OK) << "Test07_012_004 failed";
}
// [PmSoftwareReset_TC_07_012_004]
#endif
