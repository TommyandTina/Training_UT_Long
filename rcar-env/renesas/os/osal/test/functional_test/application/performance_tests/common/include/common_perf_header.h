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
* File Name :    common_perf_header.h
* Version :      3.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Header of OSAL Common API Performance Test
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           3.0.0     08.06.2023    Newly Create
***********************************************************************************************************************/
#ifndef OSAL_PERF_COMMON_API_H_
#define OSAL_PERF_COMMON_API_H_

/* Definition */

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

void exec_perf_common_api();

e_osal_return_t exec_perf_Initialize();
int R_OSAL_Initialize_TC_24_001_001_NOR(const char* func, uint64_t expected_time);
void R_OSAL_Initialize_TC_24_001_001(uint64_t expected_time);

e_osal_return_t exec_perf_Deinitialize();
int R_OSAL_Deinitialize_TC_24_001_002_NOR(const char* func, uint64_t expected_time);
void R_OSAL_Deinitialize_TC_24_001_002(uint64_t expected_time);

st_osal_version_t * exec_perf_GetVersion();
int R_OSAL_GetVersion_TC_24_001_003_NOR(const char* func, uint64_t expected_time);
void R_OSAL_GetVersion_TC_24_001_003(uint64_t expected_time);

#endif /* End of File */
