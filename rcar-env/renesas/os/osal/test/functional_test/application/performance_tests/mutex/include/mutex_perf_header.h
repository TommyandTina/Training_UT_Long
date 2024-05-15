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
* File Name :    mutex_perf_header.h
* Version :      3.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Header of OSAL Thread Synchronization Mutex Manager Performance Test
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           3.0.0     01.06.2023    Create New
***********************************************************************************************************************/
#ifndef OSAL_PERF_MUTEX_H_
#define OSAL_PERF_MUTEX_H_

/* Definition */
#define TEST_MUTEX_ID1                      0xf000U /* OSAL_MUTEX_OSAL_01 */
#define TEST_MUTEX_ID2                      0xf001U /* OSAL_MUTEX_OSAL_02 */

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

void exec_perf_mutex();

void isr_threadsync_memory_barrier_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg);
void isr_threadsync_mutex_lock_timestamp_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg);
void isr_threadsync_mutex_lock_timeperiod_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg);
void isr_threadsync_mutex_trylock_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg);
void isr_threadsync_mutex_unlock_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg);

e_osal_return_t exec_perf_ThsyncMutexCreate();
int R_OSAL_ThsyncMutexCreate_TC_16_001_001_NOR(const char* func, uint64_t expected_time);
void R_OSAL_ThsyncMutexCreate_TC_16_001_001(uint64_t expected_time);

e_osal_return_t exec_perf_ThsyncMemoryBarrier();
int R_OSAL_ThsyncMemoryBarrier_TC_16_001_002_NOR(const char* func, uint64_t expected_time);
int R_OSAL_ThsyncMemoryBarrier_TC_16_001_002_ISR(const char* func, uint64_t expected_time);
void R_OSAL_ThsyncMemoryBarrier_TC_16_001_002(uint64_t expected_time);

e_osal_return_t exec_perf_ThsyncMutexDestroy();
int R_OSAL_ThsyncMutexDestroy_TC_16_001_003_NOR(const char* func, uint64_t expected_time);
void R_OSAL_ThsyncMutexDestroy_TC_16_001_003(uint64_t expected_time);

e_osal_return_t exec_perf_ThsyncMutexLockUntilTimeStamp();
int R_OSAL_ThsyncMutexLockUntilTimeStamp_TC_16_001_004_NOR(const char* func, uint64_t expected_time);
int R_OSAL_ThsyncMutexLockUntilTimeStamp_TC_16_001_004_ISR(const char* func, uint64_t expected_time);
void R_OSAL_ThsyncMutexLockUntilTimeStamp_TC_16_001_004(uint64_t expected_time);

e_osal_return_t exec_perf_ThsyncMutexLockForTimePeriod();
int R_OSAL_ThsyncMutexLockForTimePeriod_TC_16_001_005_NOR(const char* func, uint64_t expected_time);
int R_OSAL_ThsyncMutexLockForTimePeriod_TC_16_001_005_ISR(const char* func, uint64_t expected_time);
void R_OSAL_ThsyncMutexLockForTimePeriod_TC_16_001_005(uint64_t expected_time);

e_osal_return_t exec_perf_ThsyncMutexTryLock();
int R_OSAL_ThsyncMutexTryLock_TC_16_001_006_NOR(const char* func, uint64_t expected_time);
int R_OSAL_ThsyncMutexTryLock_TC_16_001_006_ISR(const char* func, uint64_t expected_time);
void R_OSAL_ThsyncMutexTryLock_TC_16_001_006(uint64_t expected_time);

e_osal_return_t exec_perf_ThsyncMutexUnlock();
int R_OSAL_ThsyncMutexUnlock_TC_16_001_007_NOR(const char* func, uint64_t expected_time);
int R_OSAL_ThsyncMutexUnlock_TC_16_001_007_ISR(const char* func, uint64_t expected_time);
void R_OSAL_ThsyncMutexUnlock_TC_16_001_007(uint64_t expected_time);
#endif /* End of File */
