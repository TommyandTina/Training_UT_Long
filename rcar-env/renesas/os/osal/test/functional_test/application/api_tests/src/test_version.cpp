/***********************************************************************************************************************
* Copyright [2020-2022] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    test_version.cpp
* Version :      2.0.2
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Version API Functional Test
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           2.0.0     29.10.2021    Create New
 *           2.0.1     18.11.2021    Fix Typo
 *           2.0.2     03.01.2022    Update test name OsalVersion.versionCheck_TC_11_001_001
***********************************************************************************************************************/
#include "rcar-xos/osal/r_osal.h"
#if !defined(OSAL_TEST_FRAMEWORK)
#include "gmock/gmock.h"
#include "gtest/gtest.h"
using namespace std;
#else
#include "osal_test_framework.hpp"
using namespace OsalFWHelper;
#endif

/**
 * @page Version Version API Test
 * @test
 * @test @link OsalVersion_versionCheck_TC_11_001_001 @endlink
 */

/**
 * @page Version Version API Test
 * @section OsalVersion_versionCheck_TC_11_001_001 OsalVersion.versionCheck_TC_11_001_001
 * @subsection versionCheck_TC_11_001_001_tsi Test Suite ID
 * OsalVersion
 * @subsection versionCheck_TC_11_001_001_tci Test Case ID
 * versionCheck_S1_N_TC_11_001_001
 * @subsection versionCheck_TC_11_001_001_rr Related Requirement(s)
 * \#288821
 * @subsection versionCheck_TC_11_001_001_pp Prerequisite/Precondition
 * OSAL is initialized.
 * @subsection versionCheck_TC_11_001_001_tcs Test Case Summary
 * This test shall verify the functionality of the R_OSAL_GetVersion function
 * @subsection versionCheck_TC_11_001_001_tpts Test Procedure/Test Steps
 * -# Get version values with majorVersion from OSAL using R_OSAL_GetVersion function.
 * -# Get version values with minorVersion from OSAL using R_OSAL_GetVersion function.
 * -# Get version values with patchVersion from OSAL using R_OSAL_GetVersion function.
 * @subsection versionCheck_TC_11_001_001_ereb Expected Result/Behavior
 * -# R_OSAL_GetVersion return OSAL_VERSION_MAJOR. <br>
 * Value of OSAL_VERSION_MAJOR is equal to "majorVersion" of current OSAL API version.
 * -# R_OSAL_GetVersion return OSAL_VERSION_MINOR. <br>
 * Value of OSAL_VERSION_MAJOR is equal to "minorVersion" of current OSAL API version.
 * -# R_OSAL_GetVersion return OSAL_VERSION_PATCH. <br>
 * Value of OSAL_VERSION_MAJOR is equal to "patchVersion" of current OSAL API version.
 * @subsection versionCheck_TC_11_001_001_testcase Test Case Example
 * @snippet test_version.cpp versionCheck_TC_11_001_001
 */
// [versionCheck_TC_11_001_001]
TEST(OsalVersion, versionCheck_TC_11_001_001)
{
    EXPECT_EQ(OSAL_VERSION_MAJOR, R_OSAL_GetVersion()->api.major);
    EXPECT_EQ(OSAL_VERSION_MINOR, R_OSAL_GetVersion()->api.minor);
    EXPECT_EQ(OSAL_VERSION_PATCH, R_OSAL_GetVersion()->api.patch);
    /* we cannot generically check OSAL Wrapper version here */
}
// [versionCheck_TC_11_001_001]
