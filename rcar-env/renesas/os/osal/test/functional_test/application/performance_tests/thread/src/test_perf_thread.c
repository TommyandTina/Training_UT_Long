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
* File Name :    test_perf_thread.c
* Version :      3.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Thread Manager Performance Test
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           3.0.0     18.04.2023    Newly create
***********************************************************************************************************************/
#include "common_header.h"

/* Execute the performance test for OSAL Thread APIs */
void exec_perf_thread()
{
    TEST(R_OSAL_ThreadCreate, TC_15_001_001);
    TEST(R_OSAL_ThreadSleepUntilTimeStamp, TC_15_001_002);
    TEST(R_OSAL_ThreadSleepForTimePeriod, TC_15_001_003);
    TEST(R_OSAL_ThreadSelf, TC_15_001_004);
    TEST( R_OSAL_ThreadEqual, TC_15_001_005);
    TEST(R_OSAL_ThreadJoin, TC_15_001_006);
    TEST(R_OSAL_ThreadInitializeThreadConfigSt, TC_15_001_007);
}

/* Simple worker function */
int64_t worker_function(void * arg)
{
    (void)arg;
    return (int64_t) 0;
}

/* Simple worker function */
int64_t worker_function_thread_self(void * arg)
{
    (void)arg;
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    osal_thread_handle_t thread_handle = OSAL_THREAD_HANDLE_INVALID;

    START_MEASURE
    osal_ret = R_OSAL_ThreadSelf(&thread_handle);
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

    return (int64_t) osal_ret;
}

/* Measure the execution time of R_OSAL_ThreadCreate */
e_osal_return_t exec_perf_ThreadCreate()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    st_osal_thread_config_t config;
    osal_thread_handle_t thread_handle = OSAL_THREAD_HANDLE_INVALID;
    int64_t thread_ret;

    config.priority = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = (char *)"threadcreate";
    config.func = worker_function;
    config.userarg = NULL;
    config.stack_size = THREAD_STACK_SIZE;

    START_MEASURE
    osal_ret = R_OSAL_ThreadCreate(&config, THREAD_ID1, &thread_handle);
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
        osal_ret = R_OSAL_ThreadJoin(thread_handle, &thread_ret);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_ThreadCreate in normal context */
int R_OSAL_ThreadCreate_TC_15_001_001_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_ThreadCreate();
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

/* Execute the performance test for R_OSAL_ThreadCreate in normal and interrupt context */
void R_OSAL_ThreadCreate_TC_15_001_001(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_ThreadCreate_TC_15_001_001_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_ThreadCreate is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_ThreadSleepUntilTimeStamp */
e_osal_return_t exec_perf_ThreadSleepUntilTimeStamp()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    st_osal_time_t abs_time;

    osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        START_MEASURE
        osal_ret = R_OSAL_ThreadSleepUntilTimeStamp(&abs_time);
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

/* Execute the performance test for R_OSAL_ThreadSleepUntilTimeStamp in normal context */
int R_OSAL_ThreadSleepUntilTimeStamp_TC_15_001_002_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_ThreadSleepUntilTimeStamp();
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

/* Execute the performance test for R_OSAL_ThreadSleepUntilTimeStamp in normal and interrupt context */
void R_OSAL_ThreadSleepUntilTimeStamp_TC_15_001_002(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_ThreadSleepUntilTimeStamp_TC_15_001_002_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_ThreadSleepUntilTimeStamp is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_ThreadSleepForTimePeriod */
e_osal_return_t exec_perf_ThreadSleepForTimePeriod()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    osal_milli_sec_t time_period = 0;

    START_MEASURE
    osal_ret = R_OSAL_ThreadSleepForTimePeriod(time_period);
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

/* Execute the performance test for R_OSAL_ThreadSleepForTimePeriod in normal context */
int R_OSAL_ThreadSleepForTimePeriod_TC_15_001_003_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_ThreadSleepForTimePeriod();
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

/* Execute the performance test for R_OSAL_ThreadSleepForTimePeriod in normal and interrupt context */
void R_OSAL_ThreadSleepForTimePeriod_TC_15_001_003(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_ThreadSleepForTimePeriod_TC_15_001_003_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_ThreadSleepForTimePeriod is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_ThreadSelf */
e_osal_return_t exec_perf_ThreadSelf()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    st_osal_thread_config_t config;
    osal_thread_handle_t local_handle = OSAL_THREAD_HANDLE_INVALID;
    int64_t thread_ret;

    config.priority = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = (char *)"threadself";
    config.func = worker_function_thread_self;
    config.userarg = NULL;
    config.stack_size = THREAD_STACK_SIZE;

    osal_ret = R_OSAL_ThreadCreate(&config, THREAD_ID1, &local_handle);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        osal_ret = R_OSAL_ThreadJoin(local_handle, &thread_ret);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_ThreadSelf in normal context */
int R_OSAL_ThreadSelf_TC_15_001_004_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_ThreadSelf();
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

/* Execute the performance test for R_OSAL_ThreadSelf in normal and interrupt context */
void R_OSAL_ThreadSelf_TC_15_001_004(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_ThreadSelf_TC_15_001_004_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_ThreadSelf is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_ThreadEqual */
e_osal_return_t exec_perf_ThreadEqual()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    st_osal_thread_config_t config;
    osal_thread_handle_t thread_handle1 = OSAL_THREAD_HANDLE_INVALID;
    int64_t thread_ret1;
    osal_thread_handle_t thread_handle2 = OSAL_THREAD_HANDLE_INVALID;
    int64_t thread_ret2;
    bool equal_result = false;

    config.priority = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = (char *)"thread_equal1";
    config.func = worker_function;
    config.userarg = NULL;
    config.stack_size = THREAD_STACK_SIZE;

    osal_ret = R_OSAL_ThreadCreate(&config, THREAD_ID1, &thread_handle1);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        config.task_name = (char *)"thread_equal2";
        osal_ret = R_OSAL_ThreadCreate(&config, THREAD_ID2, &thread_handle2);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
        else
        {
            START_MEASURE
            osal_ret = R_OSAL_ThreadEqual(thread_handle1, thread_handle2, &equal_result);
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
    }

    /* post-processing */
    osal_ret = R_OSAL_ThreadJoin(thread_handle1, &thread_ret1);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }

    osal_ret = R_OSAL_ThreadJoin(thread_handle2, &thread_ret2);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_ThreadEqual in normal context */
int R_OSAL_ThreadEqual_TC_15_001_005_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_ThreadEqual();
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

/* Execute the performance test for R_OSAL_ThreadEqual in normal and interrupt context */
void R_OSAL_ThreadEqual_TC_15_001_005(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_ThreadEqual_TC_15_001_005_NOR(api_name, expected_time);

    /* ISR context */
    /* Thread Join is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_ThreadJoin */
e_osal_return_t exec_perf_ThreadJoin()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    st_osal_thread_config_t config;
    osal_thread_handle_t thread_handle = OSAL_THREAD_HANDLE_INVALID;
    int64_t thread_ret;

    config.priority = OSAL_THREAD_PRIORITY_TYPE1;
    config.task_name = (char *)"threadJoin";
    config.func = worker_function;
    config.userarg = NULL;
    config.stack_size = THREAD_STACK_SIZE;

    osal_ret = R_OSAL_ThreadCreate(&config, THREAD_ID1, &thread_handle);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }

    if(OSAL_RETURN_OK == osal_ret)
    {
        START_MEASURE
        osal_ret = R_OSAL_ThreadJoin(thread_handle, &thread_ret);
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

/* Execute the performance test for R_OSAL_ThreadJoin in normal context */
int R_OSAL_ThreadJoin_TC_15_001_006_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_ThreadJoin();
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

/* Execute the performance test for R_OSAL_ThreadJoin in normal and interrupt context */
void R_OSAL_ThreadJoin_TC_15_001_006(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_ThreadJoin_TC_15_001_006_NOR(api_name, expected_time);

    /* ISR context */
    /* Thread Join is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_ThreadInitializeThreadConfigSt */
e_osal_return_t exec_perf_ThreadInitializeThreadConfigSt()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    st_osal_thread_config_t local_config;

    START_MEASURE
    osal_ret = R_OSAL_ThreadInitializeThreadConfigSt(&local_config);
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

/* Execute the performance test for R_OSAL_ThreadInitializeThreadConfigSt in normal context */
int R_OSAL_ThreadInitializeThreadConfigSt_TC_15_001_007_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_ThreadInitializeThreadConfigSt();
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

/* Execute the performance test for R_OSAL_ThreadInitializeThreadConfigSt in normal and interrupt context */
void R_OSAL_ThreadInitializeThreadConfigSt_TC_15_001_007(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_ThreadInitializeThreadConfigSt_TC_15_001_007_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_ThreadInitializeThreadConfigSt is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}
