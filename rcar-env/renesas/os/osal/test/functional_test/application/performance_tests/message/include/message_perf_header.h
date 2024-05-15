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
* File Name :    message_perf_header.h
* Version :      3.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Header of OSAL Message Queue Manager Performance Test
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           3.0.0     04.06.2023    Create New
***********************************************************************************************************************/
#ifndef OSAL_PERF_MQ_H_
#define OSAL_PERF_MQ_H_

/* Definition */
#define TEST_MQ_ID                     0xf001U /* OSAL_MQ_OSAL_01 { 16, 64}*/
#define SEND_MSG_ARR                   (int32_t[]){SEND_MSG, SEND_MSG, SEND_MSG, SEND_MSG, SEND_MSG,\
                                       SEND_MSG, SEND_MSG, SEND_MSG, SEND_MSG, SEND_MSG,\
                                       SEND_MSG, SEND_MSG, SEND_MSG, SEND_MSG, SEND_MSG, SEND_MSG}
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

void exec_perf_mq();

void isr_mq_send_timestamp_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg);
void isr_mq_send_timeperiod_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg);
void isr_mq_receive_timestamp_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg);
void isr_mq_receive_timeperiod_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg);
void isr_mq_isfull_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg);
void isr_mq_isempty_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg);

e_osal_return_t exec_perf_MqCreate();
int R_OSAL_MqCreate_TC_19_001_001_NOR(const char* func, uint64_t expected_time);
void R_OSAL_MqCreate_TC_19_001_001(uint64_t expected_time);

e_osal_return_t exec_perf_MqDelete();
int R_OSAL_MqDelete_TC_19_001_002_NOR(const char* func, uint64_t expected_time);
void R_OSAL_MqDelete_TC_19_001_002(uint64_t expected_time);

e_osal_return_t exec_perf_MqSendUntilTimeStamp();
int R_OSAL_MqSendUntilTimeStamp_TC_19_001_003_NOR(const char* func, uint64_t expected_time);
int R_OSAL_MqSendUntilTimeStamp_TC_19_001_003_ISR(const char* func, uint64_t expected_time);
void R_OSAL_MqSendUntilTimeStamp_TC_19_001_003(uint64_t expected_time);

e_osal_return_t exec_perf_MqSendForTimePeriod();
int R_OSAL_MqSendForTimePeriod_TC_19_001_004_NOR(const char* func, uint64_t expected_time);
int R_OSAL_MqSendForTimePeriod_TC_19_001_004_ISR(const char* func, uint64_t expected_time);
void R_OSAL_MqSendForTimePeriod_TC_19_001_004(uint64_t expected_time);

e_osal_return_t exec_perf_MqReceiveUntilTimeStamp();
int R_OSAL_MqReceiveUntilTimeStamp_TC_19_001_005_NOR(const char* func, uint64_t expected_time);
int R_OSAL_MqReceiveUntilTimeStamp_TC_19_001_005_ISR(const char* func, uint64_t expected_time);
void R_OSAL_MqReceiveUntilTimeStamp_TC_19_001_005(uint64_t expected_time);

e_osal_return_t exec_perf_MqReceiveForTimePeriod();
int R_OSAL_MqReceiveForTimePeriod_TC_19_001_006_NOR(const char* func, uint64_t expected_time);
int R_OSAL_MqReceiveForTimePeriod_TC_19_001_006_ISR(const char* func, uint64_t expected_time);
void R_OSAL_MqReceiveForTimePeriod_TC_19_001_006(uint64_t expected_time);

e_osal_return_t exec_perf_MqIsFull();
int R_OSAL_MqIsFull_TC_19_001_007_NOR(const char* func, uint64_t expected_time);
int R_OSAL_MqIsFull_TC_19_001_007_ISR(const char* func, uint64_t expected_time);
void R_OSAL_MqIsFull_TC_19_001_007(uint64_t expected_time);

e_osal_return_t exec_perf_MqIsEmpty();
int R_OSAL_MqIsEmpty_TC_19_001_008_NOR(const char* func, uint64_t expected_time);
int R_OSAL_MqIsEmpty_TC_19_001_008_ISR(const char* func, uint64_t expected_time);
void R_OSAL_MqIsEmpty_TC_19_001_008(uint64_t expected_time);

e_osal_return_t exec_perf_MqGetConfig();
int R_OSAL_MqGetConfig_TC_19_001_009_NOR(const char* func, uint64_t expected_time);
int R_OSAL_MqGetConfig_TC_19_001_009_ISR(const char* func, uint64_t expected_time);
void R_OSAL_MqGetConfig_TC_19_001_009(uint64_t expected_time);

e_osal_return_t exec_perf_MqReset();
int R_OSAL_MqReset_TC_19_001_010_NOR(const char* func, uint64_t expected_time);
void R_OSAL_MqReset_TC_19_001_010(uint64_t expected_time);

e_osal_return_t exec_perf_MqInitializeMqConfigSt();
int R_OSAL_MqInitializeMqConfigSt_TC_19_001_011_NOR(const char* func, uint64_t expected_time);
void R_OSAL_MqInitializeMqConfigSt_TC_19_001_011(uint64_t expected_time);

#endif /* End of File */
