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
* File Name :    memory_perf_header.h
* Version :      3.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Header of OSAL Memory Manager Performance Test
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           3.0.0     05.06.2023    Create New
***********************************************************************************************************************/
#ifndef OSAL_PERF_MEMORY_H_
#define OSAL_PERF_MEMORY_H_

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

void exec_perf_memory();

e_osal_return_t exec_perf_MmngrOpen();
int R_OSAL_MmngrOpen_TC_23_001_001_NOR(const char* func, uint64_t expected_time);
void R_OSAL_MmngrOpen_TC_23_001_001(uint64_t expected_time);

e_osal_return_t exec_perf_MmngrClose();
int R_OSAL_MmngrClose_TC_23_001_002_NOR(const char* func, uint64_t expected_time);
void R_OSAL_MmngrClose_TC_23_001_002(uint64_t expected_time);

e_osal_return_t exec_perf_MmngrAlloc();
int R_OSAL_MmngrAlloc_TC_23_001_003_NOR(const char* func, uint64_t expected_time);
void R_OSAL_MmngrAlloc_TC_23_001_003(uint64_t expected_time);

e_osal_return_t exec_perf_MmngrDealloc();
int R_OSAL_MmngrDealloc_TC_23_001_004_NOR(const char* func, uint64_t expected_time);
void R_OSAL_MmngrDealloc_TC_23_001_004(uint64_t expected_time);

e_osal_return_t exec_perf_MmngrGetConfig();
int R_OSAL_MmngrGetConfig_TC_23_001_005_NOR(const char* func, uint64_t expected_time);
void R_OSAL_MmngrGetConfig_TC_23_001_005(uint64_t expected_time);

e_osal_return_t exec_perf_MmngrGetOsalMaxConfig();
int R_OSAL_MmngrGetOsalMaxConfig_TC_23_001_006_NOR(const char* func, uint64_t expected_time);
void R_OSAL_MmngrGetOsalMaxConfig_TC_23_001_006(uint64_t expected_time);

e_osal_return_t exec_perf_MmngrInitializeMmngrConfigSt();
int R_OSAL_MmngrInitializeMmngrConfigSt_TC_23_001_007_NOR(const char* func, uint64_t expected_time);
void R_OSAL_MmngrInitializeMmngrConfigSt_TC_23_001_007(uint64_t expected_time);

e_osal_return_t exec_perf_MmngrRegisterMonitor();
int R_OSAL_MmngrRegisterMonitor_TC_23_001_008_NOR(const char* func, uint64_t expected_time);
void R_OSAL_MmngrRegisterMonitor_TC_23_001_008(uint64_t expected_time);

e_osal_return_t exec_perf_MmngrGetCpuPtr();
int R_OSAL_MmngrGetCpuPtr_TC_23_001_009_NOR(const char* func, uint64_t expected_time);
void R_OSAL_MmngrGetCpuPtr_TC_23_001_009(uint64_t expected_time);

e_osal_return_t exec_perf_MmngrGetConstCpuPtr();
int R_OSAL_MmngrGetConstCpuPtr_TC_23_001_010_NOR(const char* func, uint64_t expected_time);
void R_OSAL_MmngrGetConstCpuPtr_TC_23_001_010(uint64_t expected_time);

e_osal_return_t exec_perf_MmngrFlush();
int R_OSAL_MmngrFlush_TC_23_001_011_NOR(const char* func, uint64_t expected_time);
void R_OSAL_MmngrFlush_TC_23_001_011(uint64_t expected_time);

e_osal_return_t exec_perf_MmngrInvalidate();
int R_OSAL_MmngrInvalidate_TC_23_001_012_NOR(const char* func, uint64_t expected_time);
void R_OSAL_MmngrInvalidate_TC_23_001_012(uint64_t expected_time);

e_osal_return_t exec_perf_MmngrGetHwAddr();
int R_OSAL_MmngrGetHwAddr_TC_23_001_013_NOR(const char* func, uint64_t expected_time);
void R_OSAL_MmngrGetHwAddr_TC_23_001_013(uint64_t expected_time);

e_osal_return_t exec_perf_MmngrGetSize();
int R_OSAL_MmngrGetSize_TC_23_001_014_NOR(const char* func, uint64_t expected_time);
void R_OSAL_MmngrGetSize_TC_23_001_014(uint64_t expected_time);

e_osal_return_t exec_perf_MmngrCheckBuffer();
int R_OSAL_MmngrCheckBuffer_TC_23_001_015_NOR(const char* func, uint64_t expected_time);
void R_OSAL_MmngrCheckBuffer_TC_23_001_015(uint64_t expected_time);

#if !defined(NDEBUG)
e_osal_return_t exec_perf_MmngrDebugMmngrPrint();
int R_OSAL_MmngrDebugMmngrPrint_TC_23_001_016_NOR(const char* func, uint64_t expected_time);
void R_OSAL_MmngrDebugMmngrPrint_TC_23_001_016(uint64_t expected_time);

e_osal_return_t exec_perf_MmngrDebugBufferPrint();
int R_OSAL_MmngrDebugBufferPrint_TC_23_001_017_NOR(const char* func, uint64_t expected_time);
void R_OSAL_MmngrDebugBufferPrint_TC_23_001_017(uint64_t expected_time);
#endif

e_osal_return_t exec_perf_MmngrInitializeMemoryManagerObj();
int R_OSAL_MmngrInitializeMemoryManagerObj_TC_23_001_018_NOR(const char* func, uint64_t expected_time);
void R_OSAL_MmngrInitializeMemoryManagerObj_TC_23_001_018(uint64_t expected_time);

e_osal_return_t exec_perf_MmngrInitializeMemoryBufferObj();
int R_OSAL_MmngrInitializeMemoryBufferObj_TC_23_001_019_NOR(const char* func, uint64_t expected_time);
void R_OSAL_MmngrInitializeMemoryBufferObj_TC_23_001_019(uint64_t expected_time);

e_osal_return_t exec_perf_MmngrOpenFromRegionIdx();
int R_OSAL_MmngrOpenFromRegionIdx_TC_23_001_020_NOR(const char* func, uint64_t expected_time);
void R_OSAL_MmngrOpenFromRegionIdx_TC_23_001_020(uint64_t expected_time);

e_osal_return_t exec_perf_MmngrGetOsalMaxConfigFromRegionIdx();
int R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx_TC_23_001_021_NOR(const char* func, uint64_t expected_time);
void R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx_TC_23_001_021(uint64_t expected_time);

#endif /* End of File */
