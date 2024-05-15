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
* File Name :    timeclock_perf_header.h
* Version :      3.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Header of OSAL Time&Clock Manager Performance Test
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           3.0.0     20.04.2023    Newly Create
***********************************************************************************************************************/
#ifndef OSAL_PERF_TIMECLOCK_H_
#define OSAL_PERF_TIMECLOCK_H_

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
 * Example: test case 18_001_001 of API R_OSAL_ClockTimeGetTimeStamp:
 * --> test function name is "R_OSAL_ClockTimeGetTimeStamp_TC_18_001_001"
 * --> test sub-function name (normal context) is "R_OSAL_ClockTimeGetTimeStamp_TC_18_001_001_NOR"
 * --> test sub-function name (interrupt context) is "R_OSAL_ClockTimeGetTimeStamp_TC_18_001_001_ISR"
 */

void exec_perf_timeclock();
void isr_timeclock_get_time_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg);
void isr_timeclock_calculate_time_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg);

e_osal_return_t exec_perf_ClockTimeGetTimeStamp();
int R_OSAL_ClockTimeGetTimeStamp_TC_18_001_001_NOR(const char* func, uint64_t expected_time);
int R_OSAL_ClockTimeGetTimeStamp_TC_18_001_001_ISR(const char* func, uint64_t expected_time);
void R_OSAL_ClockTimeGetTimeStamp_TC_18_001_001(uint64_t expected_time);

e_osal_return_t exec_perf_ClockTimeCalculateTimeDifference();
int R_OSAL_ClockTimeCalculateTimeDifference_TC_18_001_002_NOR(const char* func, uint64_t expected_time);
int R_OSAL_ClockTimeCalculateTimeDifference_TC_18_001_002_ISR(const char* func, uint64_t expected_time);
void R_OSAL_ClockTimeCalculateTimeDifference_TC_18_001_002(uint64_t expected_time);

#endif /* End of File */
