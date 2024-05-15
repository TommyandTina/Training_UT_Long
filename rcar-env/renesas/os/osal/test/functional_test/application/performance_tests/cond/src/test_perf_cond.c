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
* File Name :    test_perf_cond.c
* Version :      3.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Thread Synchronization Condition Variable Manager Performance Test
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           3.0.0     02.06.2023    Newly create
***********************************************************************************************************************/
#include "common_header.h"

/* Global mutex, condition variable for performance testing */
static osal_mutex_handle_t g_mutex_handle = OSAL_MUTEX_HANDLE_INVALID;
static osal_cond_handle_t g_cond_handle = OSAL_COND_HANDLE_INVALID;

/* Execute the performance test for OSAL Thread Synchronization Condition Variable APIs */
void exec_perf_cond()
{
    e_osal_return_t osal_ret;
    osal_mutex_id_t mutex_id;
    osal_cond_id_t cond_id;

    TEST(R_OSAL_ThsyncCondCreate, TC_17_001_001);
    TEST(R_OSAL_ThsyncCondDestroy, TC_17_001_002);

    mutex_id = TEST_MUTEX_ID;
    osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &g_mutex_handle);
    if(OSAL_RETURN_OK == osal_ret)
    {
        cond_id = TEST_COND_ID;
        osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &g_cond_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
    }

    if(OSAL_RETURN_OK == osal_ret)
    {
        TEST(R_OSAL_ThsyncCondWaitUntilTimeStamp, TC_17_001_003);
        TEST(R_OSAL_ThsyncCondWaitForTimePeriod, TC_17_001_004);
        TEST(R_OSAL_ThsyncCondBroadcast, TC_17_001_005);
        TEST(R_OSAL_ThsyncCondSignal, TC_17_001_006);
    }
    else
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }

    /* post processing */
    if(OSAL_MUTEX_HANDLE_INVALID != g_mutex_handle)
    {
        osal_ret = R_OSAL_ThsyncMutexDestroy(g_mutex_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
    }

    if(OSAL_COND_HANDLE_INVALID != g_cond_handle)
    {
        osal_ret = R_OSAL_ThsyncCondDestroy(g_cond_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
    }
}

/* Interrupt callback function for testing R_OSAL_ThsyncCondBroadcast */
void isr_threadsync_cond_broadcast_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    e_osal_return_t osal_ret;
    isr_user_data_t* user_data = (isr_user_data_t*)irq_arg;
    int32_t send_msg = SEND_MSG;

    /* Disable interrupt */
    osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, irq_channel);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }

    /* Clear the status register */
    osal_ret = R_OSAL_IoWrite32(dev_handle, REG_CLEAR_REGISTER, CLEAR_REGISTER_VALUE);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }

    for(size_t loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_ThsyncCondBroadcast();
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
            break;
        }
    }

    osal_ret = R_OSAL_MqSendForTimePeriod(user_data->sync_mq, 0, &send_msg, sizeof(send_msg));
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
}

/* Interrupt callback function for testing R_OSAL_ThsyncCondSignal */
void isr_threadsync_cond_signal_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    e_osal_return_t osal_ret;
    isr_user_data_t* user_data = (isr_user_data_t*)irq_arg;
    int32_t send_msg = SEND_MSG;

    /* Disable interrupt */
    osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, irq_channel);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }

    /* Clear the status register */
    osal_ret = R_OSAL_IoWrite32(dev_handle, REG_CLEAR_REGISTER, CLEAR_REGISTER_VALUE);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }

    for(size_t loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_ThsyncCondSignal();
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
            break;
        }
    }

    osal_ret = R_OSAL_MqSendForTimePeriod(user_data->sync_mq, 0, &send_msg, sizeof(send_msg));
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
}

/* Measure the execution time of R_OSAL_ThsyncCondCreate */
e_osal_return_t exec_perf_ThsyncCondCreate()
{
    e_osal_return_t osal_ret;
    osal_cond_id_t cond_id;
    osal_cond_handle_t cond_handle = OSAL_COND_HANDLE_INVALID;
    uint64_t exec_time;

    cond_id = TEST_COND_ID;
    START_MEASURE
    osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
    STOP_MEASURE

    if(OSAL_RETURN_OK == osal_ret)
    {
        exec_time = get_exec_time();
        push_back(exec_time);
        LOG_DEBUG("Execution time = %llu (ns)\n", exec_time);
    }
    else
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }

    if(OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_ThsyncCondCreate in normal context */
int R_OSAL_ThsyncCondCreate_TC_17_001_001_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_ThsyncCondCreate();
        if(OSAL_RETURN_OK != osal_ret)
        {
            ret = -1;
            LOG_ERROR("%s's execution time measurement failed, error = %d\n", func, osal_ret);
            break;
        }
    }

    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Normal Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_ThsyncCondCreate */
void R_OSAL_ThsyncCondCreate_TC_17_001_001(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_ThsyncCondCreate_TC_17_001_001_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_ThsyncCondCreate is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_ThsyncCondDestroy */
e_osal_return_t exec_perf_ThsyncCondDestroy()
{
    e_osal_return_t osal_ret;
    osal_cond_id_t cond_id;
    osal_cond_handle_t cond_handle = OSAL_COND_HANDLE_INVALID;
    uint64_t exec_time;

    cond_id = TEST_COND_ID;
    osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }

    if(OSAL_RETURN_OK == osal_ret)
    {
        START_MEASURE
        osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
        STOP_MEASURE
        if(OSAL_RETURN_OK == osal_ret)
        {
            exec_time = get_exec_time();
            push_back(exec_time);
            LOG_DEBUG("Execution time = %llu (ns)\n", exec_time);
        }
        else
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_ThsyncCondDestroy in normal context */
int R_OSAL_ThsyncCondDestroy_TC_17_001_002_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_ThsyncCondDestroy();
        if(OSAL_RETURN_OK != osal_ret)
        {
            ret = -1;
            LOG_ERROR("%s's execution time measurement failed, error = %d\n", func, osal_ret);
            break;
        }
    }

    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Normal Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_ThsyncCondDestroy */
void R_OSAL_ThsyncCondDestroy_TC_17_001_002(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_ThsyncCondDestroy_TC_17_001_002_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_ThsyncCondDestroy is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_ThsyncCondWaitUntilTimeStamp */
e_osal_return_t exec_perf_ThsyncCondWaitUntilTimeStamp()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    st_osal_time_t abs_time;

    osal_ret = R_OSAL_ThsyncMutexTryLock(g_mutex_handle);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
        else
        {
            START_MEASURE
            osal_ret = R_OSAL_ThsyncCondWaitUntilTimeStamp(g_cond_handle, g_mutex_handle, &abs_time);
            STOP_MEASURE
            if(OSAL_RETURN_TIME == osal_ret)
            {
                exec_time = get_exec_time();
                push_back(exec_time);
                LOG_DEBUG("Execution time = %llu (ns)\n", exec_time);
            }
            else
            {
                LOG_ERROR("Failure, error = %d\n", osal_ret);
            }
        }

        osal_ret = R_OSAL_ThsyncMutexUnlock(g_mutex_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_ThsyncCondWaitUntilTimeStamp in normal context */
int R_OSAL_ThsyncCondWaitUntilTimeStamp_TC_17_001_003_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_ThsyncCondWaitUntilTimeStamp();
        if((OSAL_RETURN_OK != osal_ret) && (osal_ret != OSAL_RETURN_TIME))
        {
            ret = -1;
            LOG_ERROR("%s's execution time measurement failed, error = %d\n", func, osal_ret);
            break;
        }
    }

    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Normal Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_ThsyncCondWaitUntilTimeStamp */
void R_OSAL_ThsyncCondWaitUntilTimeStamp_TC_17_001_003(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_ThsyncCondWaitUntilTimeStamp_TC_17_001_003_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_ThsyncCondWaitUntilTimeStamp is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_ThsyncCondWaitForTimePeriod */
e_osal_return_t exec_perf_ThsyncCondWaitForTimePeriod()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    osal_milli_sec_t time_period = 0;

    osal_ret = R_OSAL_ThsyncMutexTryLock(g_mutex_handle);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        START_MEASURE
        osal_ret = R_OSAL_ThsyncCondWaitForTimePeriod(g_cond_handle, g_mutex_handle, time_period);
        STOP_MEASURE
        if(OSAL_RETURN_TIME == osal_ret)
        {
            exec_time = get_exec_time();
            push_back(exec_time);
            LOG_DEBUG("Execution time = %llu (ns)\n", exec_time);
        }
        else
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }

        osal_ret = R_OSAL_ThsyncMutexUnlock(g_mutex_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_ThsyncCondWaitForTimePeriod in normal context */
int R_OSAL_ThsyncCondWaitForTimePeriod_TC_17_001_004_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_ThsyncCondWaitForTimePeriod();
        if((OSAL_RETURN_OK != osal_ret) && (osal_ret != OSAL_RETURN_TIME))
        {
            ret = -1;
            LOG_ERROR("%s's execution time measurement failed, error = %d\n", func, osal_ret);
            break;
        }
    }

    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Normal Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_ThsyncCondWaitForTimePeriod */
void R_OSAL_ThsyncCondWaitForTimePeriod_TC_17_001_004(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_ThsyncCondWaitForTimePeriod_TC_17_001_004_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_ThsyncCondWaitForTimePeriod is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_ThsyncCondBroadcast */
e_osal_return_t exec_perf_ThsyncCondBroadcast()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;

    START_MEASURE
    osal_ret = R_OSAL_ThsyncCondBroadcast(g_cond_handle);
    STOP_MEASURE
    if(OSAL_RETURN_OK == osal_ret)
    {
        exec_time = get_exec_time();
        push_back(exec_time);
        LOG_DEBUG("Execution time = %llu (ns)\n", exec_time);
    }
    else
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_ThsyncCondBroadcast in normal context */
int R_OSAL_ThsyncCondBroadcast_TC_17_001_005_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_ThsyncCondBroadcast();
        if(OSAL_RETURN_OK != osal_ret)
        {
            ret = -1;
            LOG_ERROR("%s's execution time measurement failed, error = %d\n", func, osal_ret);
            break;
        }
    }

    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Normal Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_ThsyncCondBroadcast in interrupt context */
int R_OSAL_ThsyncCondBroadcast_TC_17_001_005_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_threadsync_cond_broadcast_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_ThsyncCondBroadcast */
void R_OSAL_ThsyncCondBroadcast_TC_17_001_005(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_ThsyncCondBroadcast_TC_17_001_005_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_ThsyncCondBroadcast_TC_17_001_005_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor && 0 == ret_isr, api_name, test_num);
}

/* Measure the execution time of R_OSAL_ThsyncCondSignal */
e_osal_return_t exec_perf_ThsyncCondSignal()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;

    START_MEASURE
    osal_ret = R_OSAL_ThsyncCondSignal(g_cond_handle);
    STOP_MEASURE
    if(OSAL_RETURN_OK == osal_ret)
    {
        exec_time = get_exec_time();
        push_back(exec_time);
        LOG_DEBUG("Execution time = %llu (ns)\n", exec_time);
    }
    else
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_ThsyncCondSignal in normal context */
int R_OSAL_ThsyncCondSignal_TC_17_001_006_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_ThsyncCondSignal();
        if(OSAL_RETURN_OK != osal_ret)
        {
            ret = -1;
            LOG_ERROR("%s's execution time measurement failed, error = %d\n", func, osal_ret);
            break;
        }
    }

    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Normal Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_ThsyncCondSignal in interrupt context */
int R_OSAL_ThsyncCondSignal_TC_17_001_006_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_threadsync_cond_signal_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_ThsyncCondSignal */
void R_OSAL_ThsyncCondSignal_TC_17_001_006(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_ThsyncCondSignal_TC_17_001_006_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_ThsyncCondSignal_TC_17_001_006_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor && 0 == ret_isr, api_name, test_num);
}
