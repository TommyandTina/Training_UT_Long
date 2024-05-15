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
* File Name :    io_perf_header.h
* Version :      3.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Header of OSAL IO Manager Performance Test
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           3.0.0     06.06.2023    Create New
***********************************************************************************************************************/
#ifndef OSAL_PERF_IO_H_
#define OSAL_PERF_IO_H_

#if defined(OSAL_PERF_TEST_S4)
#include "io_perf_device_definition_s4.h"
#elif defined(OSAL_PERF_TEST_V4H)
#include "io_perf_device_definition_v4h.h"
#elif defined(OSAL_PERF_TEST_V3H)
#include "io_perf_device_definition_v3h.h"
#elif defined(OSAL_PERF_TEST_V3M2)
#include "io_perf_device_definition_v3m.h"
#else
#error "Un-supported SoC"
#endif

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

void exec_perf_io();

void isr_io_read8_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg);
void isr_io_read16_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg);
void isr_io_read32_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg);
void isr_io_read64_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg);
void isr_io_write8_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg);
void isr_io_write16_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg);
void isr_io_write32_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg);
void isr_io_write64_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg);
void isr_io_blockread8_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg);
void isr_io_blockread16_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg);
void isr_io_blockread32_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg);
void isr_io_blockread64_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg);
void isr_io_blockwrite8_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg);
void isr_io_blockwrite16_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg);
void isr_io_blockwrite32_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg);
void isr_io_blockwrite64_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg);

e_osal_return_t exec_perf_IoGetNumOfDevice();
int R_OSAL_IoGetNumOfDevices_TC_21_001_001_NOR(const char* func, uint64_t expected_time);
void R_OSAL_IoGetNumOfDevices_TC_21_001_001(uint64_t expected_time);

e_osal_return_t exec_perf_IoGetDeviceLists();
int R_OSAL_IoGetDeviceList_TC_21_001_002_NOR(const char* func, uint64_t expected_time);
void R_OSAL_IoGetDeviceList_TC_21_001_002(uint64_t);

e_osal_return_t exec_perf_IoDeviceOpen();
int R_OSAL_IoDeviceOpen_TC_21_001_003_NOR(const char* func, uint64_t expected_time);
void R_OSAL_IoDeviceOpen_TC_21_001_003(uint64_t expected_time);

e_osal_return_t exec_perf_IoDeviceClose();
int R_OSAL_IoDeviceClose_TC_21_001_004_NOR(const char* func, uint64_t expected_time);
void R_OSAL_IoDeviceClose_TC_21_001_004(uint64_t expected_time);

#if !defined(TEST_PERF_LINUX_SIL) && !defined(TEST_PERF_WINDOWS)
e_osal_return_t exec_perf_IoRead8(uintptr_t offset);
int R_OSAL_IoRead8_TC_21_001_005_NOR(const char* func, uint64_t expected_time);
int R_OSAL_IoRead8_TC_21_001_005_ISR(const char* func, uint64_t expected_time);
void R_OSAL_IoRead8_TC_21_001_005(uint64_t);

e_osal_return_t exec_perf_IoRead16(uintptr_t offset);
int R_OSAL_IoRead16_TC_21_001_006_NOR(const char* func, uint64_t expected_time);
int R_OSAL_IoRead16_TC_21_001_006_ISR(const char* func, uint64_t expected_time);
void R_OSAL_IoRead16_TC_21_001_006(uint64_t);
#endif

e_osal_return_t exec_perf_IoRead32(uintptr_t offset);
int R_OSAL_IoRead32_TC_21_001_007_NOR(const char* func, uint64_t expected_time);
int R_OSAL_IoRead32_TC_21_001_007_ISR(const char* func, uint64_t expected_time);
void R_OSAL_IoRead32_TC_21_001_007(uint64_t);

#if !defined(TEST_PERF_LINUX_SIL) && !defined(TEST_PERF_WINDOWS)
e_osal_return_t exec_perf_IoRead64(uintptr_t offset);
int R_OSAL_IoRead64_TC_21_001_008_NOR(const char* func, uint64_t expected_time);
int R_OSAL_IoRead64_TC_21_001_008_ISR(const char* func, uint64_t expected_time);
void R_OSAL_IoRead64_TC_21_001_008(uint64_t);

e_osal_return_t exec_perf_IoWrite8(uintptr_t offset);
int R_OSAL_IoWrite8_TC_21_001_009_NOR(const char* func, uint64_t expected_time);
int R_OSAL_IoWrite8_TC_21_001_009_ISR(const char* func, uint64_t expected_time);
void R_OSAL_IoWrite8_TC_21_001_009(uint64_t);

e_osal_return_t exec_perf_IoWrite16(uintptr_t offset);
int R_OSAL_IoWrite16_TC_21_001_010_NOR(const char* func, uint64_t expected_time);
int R_OSAL_IoWrite16_TC_21_001_010_ISR(const char* func, uint64_t expected_time);
void R_OSAL_IoWrite16_TC_21_001_010(uint64_t);
#endif

e_osal_return_t exec_perf_IoWrite32(uintptr_t offset);
int R_OSAL_IoWrite32_TC_21_001_011_NOR(const char* func, uint64_t expected_time);
int R_OSAL_IoWrite32_TC_21_001_011_ISR(const char* func, uint64_t expected_time);
void R_OSAL_IoWrite32_TC_21_001_011(uint64_t);

#if !defined(TEST_PERF_LINUX_SIL) && !defined(TEST_PERF_WINDOWS)
e_osal_return_t exec_perf_IoWrite64(uintptr_t offset);
int R_OSAL_IoWrite64_TC_21_001_012_NOR(const char* func, uint64_t expected_time);
int R_OSAL_IoWrite64_TC_21_001_012_ISR(const char* func, uint64_t expected_time);
void R_OSAL_IoWrite64_TC_21_001_012(uint64_t);

e_osal_return_t exec_perf_IoBlockRead8(uintptr_t offset);
int R_OSAL_IoBlockRead8_TC_21_001_013_NOR(const char* func, uint64_t expected_time);
int R_OSAL_IoBlockRead8_TC_21_001_013_ISR(const char* func, uint64_t expected_time);
void R_OSAL_IoBlockRead8_TC_21_001_013(uint64_t);

e_osal_return_t exec_perf_IoBlockRead16(uintptr_t offset);
int R_OSAL_IoBlockRead16_TC_21_001_014_NOR(const char* func, uint64_t expected_time);
int R_OSAL_IoBlockRead16_TC_21_001_014_ISR(const char* func, uint64_t expected_time);
void R_OSAL_IoBlockRead16_TC_21_001_014(uint64_t);
#endif

e_osal_return_t exec_perf_IoBlockRead32(uintptr_t offset);
int R_OSAL_IoBlockRead32_TC_21_001_015_NOR(const char* func, uint64_t expected_time);
int R_OSAL_IoBlockRead32_TC_21_001_015_ISR(const char* func, uint64_t expected_time);
void R_OSAL_IoBlockRead32_TC_21_001_015(uint64_t);

#if !defined(TEST_PERF_LINUX_SIL) && !defined(TEST_PERF_WINDOWS)
e_osal_return_t exec_perf_IoBlockRead64(uintptr_t offset);
int R_OSAL_IoBlockRead64_TC_21_001_016_NOR(const char* func, uint64_t expected_time);
int R_OSAL_IoBlockRead64_TC_21_001_016_ISR(const char* func, uint64_t expected_time);
void R_OSAL_IoBlockRead64_TC_21_001_016(uint64_t);

e_osal_return_t exec_perf_IoBlockWrite8(uintptr_t offset);
int R_OSAL_IoBlockWrite8_TC_21_001_017_NOR(const char* func, uint64_t expected_time);
int R_OSAL_IoBlockWrite8_TC_21_001_017_ISR(const char* func, uint64_t expected_time);
void R_OSAL_IoBlockWrite8_TC_21_001_017(uint64_t);

e_osal_return_t exec_perf_IoBlockWrite16(uintptr_t offset);
int R_OSAL_IoBlockWrite16_TC_21_001_018_NOR(const char* func, uint64_t expected_time);
int R_OSAL_IoBlockWrite16_TC_21_001_018_ISR(const char* func, uint64_t expected_time);
void R_OSAL_IoBlockWrite16_TC_21_001_018(uint64_t);
#endif

e_osal_return_t exec_perf_IoBlockWrite32(uintptr_t offset);
int R_OSAL_IoBlockWrite32_TC_21_001_019_NOR(const char* func, uint64_t expected_time);
int R_OSAL_IoBlockWrite32_TC_21_001_019_ISR(const char* func, uint64_t expected_time);
void R_OSAL_IoBlockWrite32_TC_21_001_019(uint64_t);

#if !defined(TEST_PERF_LINUX_SIL) && !defined(TEST_PERF_WINDOWS)
e_osal_return_t exec_perf_IoBlockWrite64(uintptr_t offset);
int R_OSAL_IoBlockWrite64_TC_21_001_020_NOR(const char* func, uint64_t expected_time);
int R_OSAL_IoBlockWrite64_TC_21_001_020_ISR(const char* func, uint64_t expected_time);
void R_OSAL_IoBlockWrite64_TC_21_001_020(uint64_t);
#endif

e_osal_return_t exec_perf_IoGetDeviceAxiBusId();
int R_OSAL_IoGetDeviceAxiBusId_TC_21_001_021_NOR(const char* func, uint64_t expected_time);
void R_OSAL_IoGetDeviceAxiBusId_TC_21_001_021(uint64_t);

e_osal_return_t exec_perf_IoGetAxiBusIdFromDeviceName();
int R_OSAL_IoGetAxiBusIdFromDeviceName_TC_21_001_022_NOR(const char* func, uint64_t expected_time);
void R_OSAL_IoGetAxiBusIdFromDeviceName_TC_21_001_022(uint64_t);

e_osal_return_t exec_perf_IoGetAxiBusNameList();
int R_OSAL_IoGetAxiBusNameList_TC_21_001_023_NOR(const char* func, uint64_t expected_time);
void R_OSAL_IoGetAxiBusNameList_TC_21_001_023(uint64_t);

e_osal_return_t exec_perf_IoGetAxiBusName();
int R_OSAL_IoGetAxiBusName_TC_21_001_024_NOR(const char* func, uint64_t expected_time);
void R_OSAL_IoGetAxiBusName_TC_21_001_024(uint64_t);

e_osal_return_t exec_perf_IoGetNumOfAxiIdsForAxiBusName();
int R_OSAL_IoGetNumOfAxiIdsForAxiBusName_TC_21_001_025_NOR(const char* func, uint64_t expected_time);
void R_OSAL_IoGetNumOfAxiIdsForAxiBusName_TC_21_001_025(uint64_t);

e_osal_return_t exec_perf_IoGetAxiIdListForAxiBusName();
int R_OSAL_IoGetAxiIdListForAxiBusName_TC_21_001_026_NOR(const char* func, uint64_t expected_time);
void R_OSAL_IoGetAxiIdListForAxiBusName_TC_21_001_026(uint64_t);

e_osal_return_t exec_perf_IoGetNumOfAxiIdsForDeviceHdl();
int R_OSAL_IoGetNumOfAxiIdsForDeviceHdl_TC_21_001_027_NOR(const char* func, uint64_t expected_time);
void R_OSAL_IoGetNumOfAxiIdsForDeviceHdl_TC_21_001_027(uint64_t);

e_osal_return_t exec_perf_IoGetAxiIdListForDeviceHdl();
int R_OSAL_IoGetAxiIdListForDeviceHdl_TC_21_001_028_NOR(const char* func, uint64_t expected_time);
void R_OSAL_IoGetAxiIdListForDeviceHdl_TC_21_001_028(uint64_t);

e_osal_return_t exec_perf_IoGetNumOfAxiIdsForDeviceName();
int R_OSAL_IoGetNumOfAxiIdsForDeviceName_TC_21_001_029_NOR(const char* func, uint64_t expected_time);
void R_OSAL_IoGetNumOfAxiIdsForDeviceName_TC_21_001_029(uint64_t);

e_osal_return_t exec_perf_IoGetAxiIdListForDeviceName();
int R_OSAL_IoGetAxiIdListForDeviceName_TC_21_001_030_NOR(const char* func, uint64_t expected_time);
void R_OSAL_IoGetAxiIdListForDeviceName_TC_21_001_030(uint64_t);

#endif /* End of File */
