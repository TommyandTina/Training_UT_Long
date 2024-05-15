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
* File Name :    common_header.h
* Version :      3.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Common Header of OSAL Performance Test
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           3.0.0     18.04.2023    Newly create
***********************************************************************************************************************/
#ifndef OSAL_PERF_COMMON_H_
#define OSAL_PERF_COMMON_H_

/* Header include */
#include "rcar-xos/osal/r_osal.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

#if defined(OSAL_PERF_TEST_S4)
#include "expected_perf_s4.h"
#elif defined(OSAL_PERF_TEST_V4H)
#include "expected_perf_v4h.h"
#elif defined(OSAL_PERF_TEST_V3H)
#include "expected_perf_v3h.h"
#elif defined(OSAL_PERF_TEST_V3M2)
#include "expected_perf_v3m.h"
#else
#error "Un-supported SoC"
#endif

#if defined(TEST_PERF_THREAD)
#include "thread_perf_header.h"
#endif

#if defined(TEST_PERF_TIMECLOCK)
#include "timeclock_perf_header.h"
#endif

#if defined(TEST_PERF_INTERRUPT)
#include "interrupt_perf_header.h"
#endif

#if defined(TEST_PERF_IO)
#include "io_perf_header.h"
#endif

#if defined(TEST_PERF_COND)
#include "cond_perf_header.h"
#endif

#if defined(TEST_PERF_MUTEX)
#include "mutex_perf_header.h"
#endif

#if defined(TEST_PERF_MQ)
#include "message_perf_header.h"
#endif

#if defined(TEST_PERF_COMMON_API)
#include "common_perf_header.h"
#endif

#if defined(TEST_PERF_POWER)
#include "power_perf_header.h"
#endif

#if defined(TEST_PERF_MEMORY)
#include "memory_perf_header.h"
#include "rcar-xos/osal/r_osal_memory_impl.h"
#endif

/* Definition */
#define LOG(...) {printf(__VA_ARGS__);}
#define LOG_ERROR(...) {printf("error: %s(%d):", __func__, __LINE__); printf(__VA_ARGS__);}
#if defined(NDEBUG)
#define LOG_DEBUG(...)
#else
#define LOG_DEBUG(...) {printf(__VA_ARGS__);}
#endif

#define START_MEASURE get_time_begin();
#define STOP_MEASURE get_time_end(); get_time_diff();

#define SEC_TO_NSEC             1000000000ULL
#define MSEC_TO_NSEC            1000000U
#define NUM_OF_MEASURE_TIME     10U

/* Concatenate strings */
#define STR_CAT(A,B) STR_CAT_IMPL(A,B)
#define STR_CAT_IMPL(A,B) A##B

/* Get the expected execution time for each API */
#define GET_EXPECTED_VALUE(...) STR_CAT(EXPECT_, __VA_ARGS__)

/* Macro for parsing the expected execution time and pass it to the test function */
#define TEST(api, test_num) STR_CAT(STR_CAT(api, _), test_num)(GET_EXPECTED_VALUE(api))

#if defined(OSAL_PERF_TEST_S4)
#define DEV_GEN_ISR_1           "rtdmac_control0_00"
#define DEV_GEN_ISR_2           "rtdmac_descriptor0_00"
#define TEST_DEV                "rtdmac_control1_00"
/* Interrupt setting */
#define NUM_OF_BUFFERS          2U // the number of buffers need to be allocate for HW interrupt generation
#define BUFFER_SIZE             64U // the buffer allocation size
#define BUFFER_ALIGNMENT        64U // the buffer allocation aligment
#define DUMMY_BUFFER_DATA       (uint32_t[]){2908, 0} // Dummy data that is the initialized value of allocated buffers
#define REG_RDMSAR              0x0000 // source address
#define REG_RDMDAR              0x0004 // destination address
#define REG_RDMTCR              0x0008 // transfer count
#define REG_RDMCHCRB            0x001C // DMA Channel Control Register B
#define REG_RDMCHCR             0x000C // DMA channel control register for RTDMAC0
#define REG_RDMOR               0x0060 // DMA operation register for RT-DMAC0
#define REG_CLEAR_REGISTER      0x0100 // Clear Register
#define TRANSFER_COUNT_VALUE    0x00000001U // 1 transfer
#define TRANSFER_STEP_VALUE     0x00000000U // 1 descriptor
#define TRANSFER_RDMCHCR_VALUE  0x10040415U // setting for DMA channel control register
#define TRANSFER_RDMOR_VALUE    0x0301U // setting for DMA operation register
#define CLEAR_REGISTER_VALUE    0x00000001U // write value of Clear Register
#else
#define DEV_GEN_ISR_1           "ims_00"
#define TEST_DEV                "imp_top_00"
/* Interrupt setting */
#define NUM_OF_BUFFERS          1U // the number of buffers need to be allocate for HW interrupt generation
#define BUFFER_SIZE             64U // the buffer allocation size
#define BUFFER_ALIGNMENT        64U // the buffer allocation aligment
/* Display list data */
#define DUMMY_BUFFER_DATA       (uint32_t[]){0x8f000000U} // Dummy data that is the initialized value of allocated buffers
/* IMR register to generate HW interrupt */
#define IMR_REG_DLSAR           0x0030 // DL Start Address Register
#define IMR_REG_IMR             0x0018 // Interrupt Mask Register
#define IMR_REG_CR              0x0008 // Control Register
#define REG_CLEAR_REGISTER      0x0010 // Status Clear Register
/* IMR register value */
#define IMR_IMR_VALUE           0x00000000U // write value of Interrupt Mask Register
#define IMR_CR_VALUE            0x00000001U // write value of Control Register
#define CLEAR_REGISTER_VALUE    0x00000007U // write value of Status Clear Register
#endif
#define SEND_MSG                0x2908U
#define SYNC_MQ_ID              0xf000U
#define MQ_TIMEOUT              3000U

#if defined(TEST_PERF_ZEPHYR)
#define CPU_CLOCK               (1400000000ULL) // CR52 CPU frequency
#define PMCR_C                  (1 << 2)
#define PMCR_E                  (1 << 0)
#define PMCNTENSET_C            (1 << 31)
#endif

/* Function declaration */
uint64_t get_exec_time();
uint64_t cal_average();
uint64_t find_max();
uint64_t find_min();
int get_test_info(const char* func, uint64_t expected_time, const char* context);
void test_judgement(bool ret, const char* func, const char* num);
void connect_str(const char* str1, const char* str2, char* output);
void push_back(uint64_t value);
void get_substr(size_t pos, size_t len, const char* input_str, char* output_str);
size_t find_str(const char* hint, const char* str);
void extract_test_info(const char* str, char* api_name, char* test_num);
e_osal_return_t open_mem(osal_memory_manager_handle_t* p_mmngr_handle);
e_osal_return_t prepare_work_buffer(osal_memory_manager_handle_t mmngr_hndl, uint32_t *p_hw_addr, osal_memory_buffer_handle_t* p_buffer_obj);
e_osal_return_t release_mem(osal_memory_manager_handle_t hndl_mmngr, osal_memory_buffer_handle_t* buffer_obj);
e_osal_return_t release_mem(osal_memory_manager_handle_t hndl_mmngr, osal_memory_buffer_handle_t* buffer_obj);
e_osal_return_t open_devices(osal_device_handle_t* dev);
e_osal_return_t release_devices(osal_device_handle_t* dev);
e_osal_return_t store_reg_init_value(osal_device_handle_t* dev);
e_osal_return_t restore_reg_init_value(osal_device_handle_t* dev);
e_osal_return_t create_sync_method(osal_mq_handle_t* p_mq_hndl);
e_osal_return_t destroy_sync_method(osal_mq_handle_t mq_hndl);
int generate_interrupt(p_osal_isr_func_t user_callback, const char* api_name);
void get_time_begin();
void get_time_end();
void get_time_diff();
void get_time_initialize();

/* struct/enum definition */
typedef struct isr_user_data
{
    osal_mq_handle_t sync_mq;
    char api_name[100];
} isr_user_data_t;

/* parameter declaration */
extern uint64_t g_diff_time;

#endif /* End of File */
