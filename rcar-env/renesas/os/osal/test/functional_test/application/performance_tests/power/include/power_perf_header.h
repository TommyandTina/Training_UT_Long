/***********************************************************************************************************************
* Copyright [2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    power_perf_header.h
* Version :      3.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Header of OSAL Power Manager Performance Test
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           3.0.0     04.06.2023    Create New
***********************************************************************************************************************/
#ifndef OSAL_PERF_POWER_H_
#define OSAL_PERF_POWER_H_

/* Definition */
#if defined(OSAL_PERF_TEST_S4) /* R-Car S4 */
#define TEST_FBA_DEVICE                  "fba_ca550" // device use for testing R_OSAL_PmSetPostClock API
#define TEST_PM_DEVICE                   "caiplite_00"
#elif defined(OSAL_PERF_TEST_V4H)
#define TEST_FBA_DEVICE                  "fba_ims0" // device use for testing R_OSAL_PmSetPostClock API
#define TEST_PM_DEVICE                   "ims_00"
#else /* V3Hx, V3M2 */
#define TEST_FBA_DEVICE                  "fba_ims0" // device use for testing R_OSAL_PmSetPostClock API
#define TEST_PM_DEVICE                   "imp_top_00"
#endif

#define TEST_PM_TIMEOUT                  2000U

/* Function declaration */

/* Test function name policy: {api_name}_{test_num}
 * - api_name: the target api name
 * - test_num: test number which is described in PCL and start with "TC"
 *
 * Test sub-function name policy: {api_name}_{test_num}_{context}
 * - api_name and test_num is same as above
 * - context:
 *      -# NOR: it means that this test case is to confirm the performance of target api in normal context
 *      -# ISR: it means that this test case is to confirm the performance of target api in interrupt context
 *
 * Example: test case 15_001_032 of API R_OSAL_ClockTimeGetTimeStamp:
 * --> test function name is "R_OSAL_ClockTimeGetTimeStamp_TC_15_001_032"
 * --> test sub-function name (normal context) is "R_OSAL_ClockTimeGetTimeStamp_TC_15_001_032_NOR"
 * --> test sub-function name (interrupt context) is "R_OSAL_ClockTimeGetTimeStamp_TC_15_001_032_ISR"
 */

void exec_perf_power();

e_osal_return_t exec_perf_PmSetRequiredState(bool applyImmediate);
int R_OSAL_PmSetRequiredState_TC_22_001_001_NOR(const char* func, uint64_t expected_time);
void R_OSAL_PmSetRequiredState_TC_22_001_001(uint64_t expected_time);

e_osal_return_t exec_perf_PmGetRequiredState();
int R_OSAL_PmGetRequiredState_TC_22_001_002_NOR(const char* func, uint64_t expected_time);
void R_OSAL_PmGetRequiredState_TC_22_001_002(uint64_t expected_time);

e_osal_return_t exec_perf_PmWaitForState();
int R_OSAL_PmWaitForState_TC_22_001_003_NOR(const char* func, uint64_t expected_time);
void R_OSAL_PmWaitForState_TC_22_001_003(uint64_t expected_time);

e_osal_return_t exec_perf_PmGetState();
int R_OSAL_PmGetState_TC_22_001_004_NOR(const char* func, uint64_t expected_time);
void R_OSAL_PmGetState_TC_22_001_004(uint64_t expected_time);

e_osal_return_t exec_perf_PmGetLowestIntermediatePowerState();
int R_OSAL_PmGetLowestIntermediatePowerState_TC_22_001_005_NOR(const char* func, uint64_t expected_time);
void R_OSAL_PmGetLowestIntermediatePowerState_TC_22_001_005(uint64_t expected_time);

e_osal_return_t exec_perf_PmSetPolicy(bool applyImmediate);
int R_OSAL_PmSetPolicy_TC_22_001_006_NOR(const char* func, uint64_t expected_time);
void R_OSAL_PmSetPolicy_TC_22_001_006(uint64_t expected_time);

e_osal_return_t exec_perf_PmGetPolicy();
int R_OSAL_PmGetPolicy_TC_22_001_007_NOR(const char* func, uint64_t expected_time);
void R_OSAL_PmGetPolicy_TC_22_001_007(uint64_t expected_time);

e_osal_return_t exec_perf_PmSetResetState();
int R_OSAL_PmSetResetState_TC_22_001_008_NOR(const char* func, uint64_t expected_time);
void R_OSAL_PmSetResetState_TC_22_001_008(uint64_t expected_time);

e_osal_return_t exec_perf_PmGetResetState();
int R_OSAL_PmGetResetState_TC_22_001_009_NOR(const char* func, uint64_t expected_time);
void R_OSAL_PmGetResetState_TC_22_001_009(uint64_t expected_time);

#if !defined (TEST_PERF_WINDOWS) && !defined (TEST_PERF_LINUX_SIL)
e_osal_return_t exec_perf_PmSetPostClock(bool clock_enable);
int R_OSAL_PmSetPostClock_TC_22_001_010_NOR(const char* func, uint64_t expected_time);
void R_OSAL_PmSetPostClock_TC_22_001_010(uint64_t expected_time);

int R_OSAL_PmSetPostClock_TC_22_001_012_NOR(const char* func, uint64_t expected_time);
void R_OSAL_PmSetPostClock_TC_22_001_012(uint64_t expected_time);
#endif

int R_OSAL_PmSetRequiredState_TC_22_001_011_NOR(const char* func, uint64_t expected_time);
void R_OSAL_PmSetRequiredState_TC_22_001_011(uint64_t expected_time);

int R_OSAL_PmSetPolicy_TC_22_001_013_NOR(const char* func, uint64_t expected_time);
void R_OSAL_PmSetPolicy_TC_22_001_013(uint64_t expected_time);
#endif /* End of File */
