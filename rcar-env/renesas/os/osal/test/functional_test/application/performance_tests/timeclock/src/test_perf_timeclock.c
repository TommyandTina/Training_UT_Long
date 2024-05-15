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
* File Name :    test_perf_timeclock.c
* Version :      3.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Time&Clock Manager Performance Test
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           3.0.0     20.04.2023    Newly create
***********************************************************************************************************************/
#include "common_header.h"

/* Execute the performance test for OSAL Time&Clock APIs */
void exec_perf_timeclock()
{
    TEST(R_OSAL_ClockTimeGetTimeStamp, TC_18_001_001);
    TEST(R_OSAL_ClockTimeCalculateTimeDifference, TC_18_001_002);
}

/* Interrupt callback function for testing for testing R_OSAL_ClockTimeGetTimeStamp*/
void isr_timeclock_get_time_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
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
        osal_ret = exec_perf_ClockTimeGetTimeStamp(user_data->api_name);
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

/* Interrupt callback function for testing R_OSAL_ClockTimeCalculateTimeDifference */
void isr_timeclock_calculate_time_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
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
        osal_ret = exec_perf_ClockTimeCalculateTimeDifference(user_data->api_name);
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

/* Measure the execution time of R_OSAL_ClockTimeGetTimeStamp */
e_osal_return_t exec_perf_ClockTimeGetTimeStamp()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    st_osal_time_t timestamp;

    START_MEASURE
    osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &timestamp);
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

/* Execute the performance test for R_OSAL_ClockTimeGetTimeStamp in normal context */
int R_OSAL_ClockTimeGetTimeStamp_TC_18_001_001_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_ClockTimeGetTimeStamp();
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

/* Execute the performance test for R_OSAL_ClockTimeGetTimeStamp in interrupt context */
int R_OSAL_ClockTimeGetTimeStamp_TC_18_001_001_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_timeclock_get_time_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_ClockTimeGetTimeStamp in normal and interrupt context */
void R_OSAL_ClockTimeGetTimeStamp_TC_18_001_001(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_ClockTimeGetTimeStamp_TC_18_001_001_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_ClockTimeGetTimeStamp_TC_18_001_001_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement((0 == ret_isr && 0 == ret_nor), api_name, test_num);
}

/* Measure the execution time of R_OSAL_ClockTimeCalculateTimeDifference */
e_osal_return_t exec_perf_ClockTimeCalculateTimeDifference()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    st_osal_time_t timestamp1;
    st_osal_time_t timestamp2;
    osal_nano_sec_t time_difference;

    osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &timestamp1);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        timestamp2.tv_sec = timestamp1.tv_sec + 1;
        timestamp2.tv_nsec = timestamp1.tv_nsec;
        START_MEASURE
        osal_ret = R_OSAL_ClockTimeCalculateTimeDifference(&timestamp2, &timestamp1, &time_difference);
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

/* Execute the performance test for R_OSAL_ClockTimeCalculateTimeDifference in normal context */
int R_OSAL_ClockTimeCalculateTimeDifference_TC_18_001_002_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_ClockTimeCalculateTimeDifference();
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

/* Execute the performance test for R_OSAL_ClockTimeCalculateTimeDifference in interrupt context */
int R_OSAL_ClockTimeCalculateTimeDifference_TC_18_001_002_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_timeclock_calculate_time_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_ClockTimeCalculateTimeDifference in normal and interrupt context */
void R_OSAL_ClockTimeCalculateTimeDifference_TC_18_001_002(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_ClockTimeCalculateTimeDifference_TC_18_001_002_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_ClockTimeCalculateTimeDifference_TC_18_001_002_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement((0 == ret_isr && 0 == ret_nor), api_name, test_num);
}
