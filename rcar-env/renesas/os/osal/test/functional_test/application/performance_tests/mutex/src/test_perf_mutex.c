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
* File Name :    test_perf_mutex.c
* Version :      3.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Thread Synchronization Mutex Manager Performance Test
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           3.0.0     01.06.2023    Newly create
***********************************************************************************************************************/
#include "common_header.h"

/* Global mutex handle for performance testing */
static osal_mutex_handle_t g_mutex_handle = OSAL_MUTEX_HANDLE_INVALID;

/* Execute the performance test for OSAL Thread APIs */
void exec_perf_mutex()
{
    e_osal_return_t osal_ret;
    osal_mutex_id_t mutex_id;

    /* pre-processing */
    mutex_id = TEST_MUTEX_ID1;
    osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &g_mutex_handle);
    if(OSAL_RETURN_OK == osal_ret)
    {
        TEST(R_OSAL_ThsyncMutexCreate, TC_16_001_001);
        TEST(R_OSAL_ThsyncMemoryBarrier, TC_16_001_002);
        TEST(R_OSAL_ThsyncMutexDestroy, TC_16_001_003);
        TEST(R_OSAL_ThsyncMutexLockUntilTimeStamp, TC_16_001_004);
        TEST(R_OSAL_ThsyncMutexLockForTimePeriod, TC_16_001_005);
        TEST(R_OSAL_ThsyncMutexTryLock, TC_16_001_006);
        TEST(R_OSAL_ThsyncMutexUnlock, TC_16_001_007);
    }
    else
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }

    /* post processing */
    if (OSAL_MUTEX_HANDLE_INVALID != g_mutex_handle)
    {
        osal_ret = R_OSAL_ThsyncMutexDestroy(g_mutex_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
    }
}

/* Interrupt callback function for testing R_OSAL_ThsyncMemoryBarrier */
void isr_threadsync_memory_barrier_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
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
        osal_ret = exec_perf_ThsyncMemoryBarrier();
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

/* Interrupt callback function for testing R_OSAL_ThsyncMutexLockUntilTimeStamp */
void isr_threadsync_mutex_lock_timestamp_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
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
        osal_ret = exec_perf_ThsyncMutexLockUntilTimeStamp();
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

/* Interrupt callback function for testing R_OSAL_ThsyncMutexLockForTimePeriod */
void isr_threadsync_mutex_lock_timeperiod_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
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
        osal_ret = exec_perf_ThsyncMutexLockForTimePeriod();
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

/* Interrupt callback function for testing R_OSAL_ThsyncMutexTryLock */
void isr_threadsync_mutex_trylock_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
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
        osal_ret = exec_perf_ThsyncMutexTryLock();
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

/* Interrupt callback function for testing R_OSAL_ThsyncMutexUnlock */
void isr_threadsync_mutex_unlock_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
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
        osal_ret = exec_perf_ThsyncMutexUnlock();
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

/* Measure the execution time of R_OSAL_ThsyncMutexCreate */
e_osal_return_t exec_perf_ThsyncMutexCreate()
{
    e_osal_return_t osal_ret;
    osal_mutex_id_t mutex_id;
    osal_mutex_handle_t mutex_handle = OSAL_MUTEX_HANDLE_INVALID;
    uint64_t exec_time;

    mutex_id = TEST_MUTEX_ID2;
    START_MEASURE
    osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle);
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
        osal_ret = R_OSAL_ThsyncMutexDestroy(mutex_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_ThsyncMutexCreate in normal context */
int R_OSAL_ThsyncMutexCreate_TC_16_001_001_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_ThsyncMutexCreate();
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

/* Execute the performance test for R_OSAL_ThsyncMutexCreate in normal and interrupt context */
void R_OSAL_ThsyncMutexCreate_TC_16_001_001(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_ThsyncMutexCreate_TC_16_001_001_NOR(api_name, expected_time);

    /* ISR context */
    /* ThreadSync Mutex create is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_ThsyncMemoryBarrier */
e_osal_return_t exec_perf_ThsyncMemoryBarrier()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;

    START_MEASURE
    osal_ret = R_OSAL_ThsyncMemoryBarrier();
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

/* Execute the performance test for R_OSAL_ThsyncMemoryBarrier in normal context */
int R_OSAL_ThsyncMemoryBarrier_TC_16_001_002_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_ThsyncMemoryBarrier();
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

/* Execute the performance test for R_OSAL_ThsyncMemoryBarrier in interrupt context */
int R_OSAL_ThsyncMemoryBarrier_TC_16_001_002_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_threadsync_memory_barrier_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_ThsyncMemoryBarrier in normal and interrupt context */
void R_OSAL_ThsyncMemoryBarrier_TC_16_001_002(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_ThsyncMemoryBarrier_TC_16_001_002_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_ThsyncMemoryBarrier_TC_16_001_002_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor && 0 == ret_isr, api_name, test_num);
}

/* Measure the execution time of R_OSAL_ThsyncMutexDestroy */
e_osal_return_t exec_perf_ThsyncMutexDestroy()
{
    e_osal_return_t osal_ret;
    osal_mutex_id_t mutex_id;
    osal_mutex_handle_t mutex_handle = OSAL_MUTEX_HANDLE_INVALID;
    uint64_t exec_time;

    mutex_id = TEST_MUTEX_ID2;
    osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }

    if(OSAL_RETURN_OK == osal_ret)
    {
        START_MEASURE
        osal_ret = R_OSAL_ThsyncMutexDestroy(mutex_handle);
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

/* Execute the performance test for R_OSAL_ThsyncMutexDestroy in normal context */
int R_OSAL_ThsyncMutexDestroy_TC_16_001_003_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_ThsyncMutexDestroy();
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

/* Execute the performance test for R_OSAL_ThsyncMutexDestroy in normal and interrupt context */
void R_OSAL_ThsyncMutexDestroy_TC_16_001_003(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_ThsyncMutexDestroy_TC_16_001_003_NOR(api_name, expected_time);

    /* ISR context */
    /* ThreadSync Mutex Destroy is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_ThsyncMutexLockUntilTimeStamp */
e_osal_return_t exec_perf_ThsyncMutexLockUntilTimeStamp()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    st_osal_time_t abs_time;
    bool is_isr = false;

    osal_ret = R_OSAL_InterruptIsISRContext(&is_isr);
    if (OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }

    osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time);
    if(OSAL_RETURN_OK == osal_ret)
    {
        if (false == is_isr)
        {
            /* Add timeout incase normal context*/
            abs_time.tv_sec = abs_time.tv_sec + 1;
        }
        START_MEASURE
        osal_ret = R_OSAL_ThsyncMutexLockUntilTimeStamp(g_mutex_handle, &abs_time);
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

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_ThsyncMutexUnlock(g_mutex_handle);
            if (OSAL_RETURN_OK != osal_ret)
            {
                LOG_ERROR("Failure, error = %d\n", osal_ret);
            }
        }
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_ThsyncMutexLockUntilTimeStamp in normal context */
int R_OSAL_ThsyncMutexLockUntilTimeStamp_TC_16_001_004_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_ThsyncMutexLockUntilTimeStamp();
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

/* Execute the performance test for R_OSAL_ThsyncMutexLockUntilTimeStamp in interrupt context */
int R_OSAL_ThsyncMutexLockUntilTimeStamp_TC_16_001_004_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_threadsync_mutex_lock_timestamp_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_ThsyncMutexLockUntilTimeStamp in normal and interrupt context */
void R_OSAL_ThsyncMutexLockUntilTimeStamp_TC_16_001_004(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_ThsyncMutexLockUntilTimeStamp_TC_16_001_004_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_ThsyncMutexLockUntilTimeStamp_TC_16_001_004_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor && 0 == ret_isr, api_name, test_num);
}

/* Measure the execution time of R_OSAL_ThsyncMutexLockForTimePeriod */
e_osal_return_t exec_perf_ThsyncMutexLockForTimePeriod()
{
    e_osal_return_t osal_ret;

    uint64_t exec_time;
    osal_milli_sec_t time_period = 0;
    bool is_isr = false;

    osal_ret = R_OSAL_InterruptIsISRContext(&is_isr);
    if (OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        if (false == is_isr)
        {
            /* Add timeout incase normal context*/
            time_period = 10;
        }
        START_MEASURE
        osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(g_mutex_handle, time_period);
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

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_ThsyncMutexUnlock(g_mutex_handle);
            if (OSAL_RETURN_OK != osal_ret)
            {
                LOG_ERROR("Failure, error = %d\n", osal_ret);
            }
        }
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_ThsyncMutexLockForTimePeriod in normal context */
int R_OSAL_ThsyncMutexLockForTimePeriod_TC_16_001_005_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_ThsyncMutexLockForTimePeriod();
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

/* Execute the performance test for R_OSAL_ThsyncMutexLockForTimePeriod in interrupt context */
int R_OSAL_ThsyncMutexLockForTimePeriod_TC_16_001_005_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_threadsync_mutex_lock_timeperiod_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_ThsyncMutexLockForTimePeriod in normal and interrupt context */
void R_OSAL_ThsyncMutexLockForTimePeriod_TC_16_001_005(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_ThsyncMutexLockForTimePeriod_TC_16_001_005_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_ThsyncMutexLockForTimePeriod_TC_16_001_005_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor && 0 == ret_isr, api_name, test_num);
}

/* Measure the execution time of R_OSAL_ThsyncMutexTryLock */
e_osal_return_t exec_perf_ThsyncMutexTryLock()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;

    START_MEASURE
    osal_ret = R_OSAL_ThsyncMutexTryLock(g_mutex_handle);
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

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_ThsyncMutexUnlock(g_mutex_handle);
        if (OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_ThsyncMutexTrylock in normal context */
int R_OSAL_ThsyncMutexTryLock_TC_16_001_006_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_ThsyncMutexTryLock();
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

/* Execute the performance test for R_OSAL_ThsyncMutexTryLock in interrupt context */
int R_OSAL_ThsyncMutexTryLock_TC_16_001_006_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_threadsync_mutex_trylock_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_ThsyncMutexTryLock in normal and interrupt context */
void R_OSAL_ThsyncMutexTryLock_TC_16_001_006(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_ThsyncMutexTryLock_TC_16_001_006_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_ThsyncMutexTryLock_TC_16_001_006_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor && 0 == ret_isr, api_name, test_num);
}

/* Measure the execution time of R_OSAL_ThsyncMutexUnlock */
e_osal_return_t exec_perf_ThsyncMutexUnlock()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;

    osal_ret = R_OSAL_ThsyncMutexTryLock(g_mutex_handle);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        START_MEASURE
        osal_ret = R_OSAL_ThsyncMutexUnlock(g_mutex_handle);
        STOP_MEASURE
        if (OSAL_RETURN_OK == osal_ret)
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

/* Execute the performance test for R_OSAL_ThsyncMutexUnlock in normal context */
int R_OSAL_ThsyncMutexUnlock_TC_16_001_007_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_ThsyncMutexUnlock();
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

/* Execute the performance test for R_OSAL_ThsyncMutexUnLock in interrupt context */
int R_OSAL_ThsyncMutexUnlock_TC_16_001_007_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_threadsync_mutex_unlock_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_ThsyncMutexUnLock in normal and interrupt context */
void R_OSAL_ThsyncMutexUnlock_TC_16_001_007(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_ThsyncMutexUnlock_TC_16_001_007_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_ThsyncMutexUnlock_TC_16_001_007_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor && 0 == ret_isr, api_name, test_num);
}