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
* File Name :    main.c
* Version :      3.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Main of OSAL Performance Test
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           3.0.0     18.04.2023    Newly create
***********************************************************************************************************************/
#include "common_header.h"

int main(int argc, char** argv)
{
    int ret = 0;
    e_osal_return_t osal_ret;

    get_time_initialize();

    /* Execute performance test for each manager */
    /* Performance test of Common API */
#if defined(TEST_PERF_COMMON_API)
    LOG("===== Start measuring the execution time of Common API =====\n");
    exec_perf_common_api();
#endif

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        LOG("R_OSAL_Initialize failed, error = %d\n", osal_ret);
        ret = -1;
    }

    /* Performance test of Thread manager */
#if defined(TEST_PERF_THREAD)
    LOG("===== Start measuring the execution time of Thread Manager =====\n");
    exec_perf_thread();
#endif

    /* Performance test of Time&Clock manager */
#if defined(TEST_PERF_TIMECLOCK)
    LOG("===== Start measuring the execution time of Time&Clock Manager =====\n");
    exec_perf_timeclock();
#endif

    /* Performance test of Interrupt manager */
#if defined(TEST_PERF_INTERRUPT)
    LOG("===== Start measuring the execution time of Interrupt Manager =====\n");
    exec_perf_interrupt();
#endif

    /* Performance test of IO manager */
#if defined(TEST_PERF_IO)
    LOG("===== Start measuring the execution time of Time&Clock Manager =====\n");
    exec_perf_io();
#endif

    /* Performance test of Thread Synchronization Condition Variable manager */
#if defined(TEST_PERF_COND)
    LOG("===== Start measuring the execution time of Thread Synchronization Condition Variable Manager =====\n");
    exec_perf_cond();
#endif

    /* Performance test of Thread Synchronization Mutex manager */
#if defined(TEST_PERF_MUTEX)
    LOG("===== Start measuring the execution time of Thread Synchronization Mutex Manager =====\n");
    exec_perf_mutex();
#endif

    /* Performance test of Message Queue manager */
#if defined(TEST_PERF_MQ)
    LOG("===== Start measuring the execution time of Time&Clock Manager =====\n");
    exec_perf_mq();
#endif

    /* Performance test of Power manager */
#if defined(TEST_PERF_POWER)
    LOG("===== Start measuring the execution time of Power Manager =====\n");
    exec_perf_power();
#endif

    /* Performance test of Memory manager */
#if defined(TEST_PERF_MEMORY)
    LOG("===== Start measuring the execution time of Memory Manager =====\n");
    exec_perf_memory();
#endif

    osal_ret = R_OSAL_Deinitialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        LOG("R_OSAL_Deinitialize failed, error = %d\n", osal_ret);
        ret = -1;
    }

#if defined(TEST_PERF_ZEPHYR)
    LOG("================== osal_functional_test_performance finish ==================\n");
#endif

    return ret;
}
