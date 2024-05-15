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
* File Name :    test_perf_message.c
* Version :      3.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Message Queue Manager Performance Test
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           3.0.0     04.06.2023    Newly create
***********************************************************************************************************************/
#include "common_header.h"

static osal_mq_handle_t g_mq_handle = OSAL_MQ_HANDLE_INVALID;
/* Execute the performance test for OSAL Message Queue APIs */
void exec_perf_mq()
{
    e_osal_return_t osal_ret;
    st_osal_mq_config_t mq_config = {16, 64};

    TEST(R_OSAL_MqCreate, TC_19_001_001);
    TEST(R_OSAL_MqDelete, TC_19_001_002);

    osal_ret = R_OSAL_MqCreate(&mq_config, TEST_MQ_ID, &g_mq_handle);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        TEST(R_OSAL_MqSendUntilTimeStamp, TC_19_001_003);
        TEST(R_OSAL_MqSendForTimePeriod, TC_19_001_004);
        TEST(R_OSAL_MqReceiveUntilTimeStamp, TC_19_001_005);
        TEST(R_OSAL_MqReceiveForTimePeriod, TC_19_001_006);
        TEST(R_OSAL_MqIsFull, TC_19_001_007);
        TEST(R_OSAL_MqIsEmpty, TC_19_001_008);
        TEST(R_OSAL_MqGetConfig, TC_19_001_009);
        TEST(R_OSAL_MqReset, TC_19_001_010);
        TEST(R_OSAL_MqInitializeMqConfigSt, TC_19_001_011);
    }

    if(OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_MqDelete(g_mq_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
    }
}

/* Interrupt callback function for testing R_OSAL_MqSendUntilTimeStamp */
void isr_mq_send_timestamp_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
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
        osal_ret = exec_perf_MqSendUntilTimeStamp();
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

/* Interrupt callback function for testing R_OSAL_MqSendForTimePeriod */
void isr_mq_send_timeperiod_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
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
        osal_ret = exec_perf_MqSendForTimePeriod();
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

/* Interrupt callback function for testing R_OSAL_MqReceiveUntilTimeStamp */
void isr_mq_receive_timestamp_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
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
        osal_ret = exec_perf_MqReceiveUntilTimeStamp();
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

/* Interrupt callback function for testing R_OSAL_MqReceiveForTimePeriod */
void isr_mq_receive_timeperiod_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
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
        osal_ret = exec_perf_MqReceiveForTimePeriod();
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

/* Interrupt callback function for testing R_OSAL_MqIsFull */
void isr_mq_isfull_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
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
        osal_ret = exec_perf_MqIsFull();
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

/* Interrupt callback function for testing R_OSAL_MqIsEmpty */
void isr_mq_isempty_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
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
        osal_ret = exec_perf_MqIsEmpty();
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

/* Measure the execution time of R_OSAL_MqCreate */
e_osal_return_t exec_perf_MqCreate()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    osal_mq_handle_t mq_handle = OSAL_MQ_HANDLE_INVALID;
    st_osal_mq_config_t mq_config = {16, 64};

    START_MEASURE
    osal_ret = R_OSAL_MqCreate(&mq_config, TEST_MQ_ID, &mq_handle);
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
        osal_ret = R_OSAL_MqDelete(mq_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_MqCreate in normal context */
int R_OSAL_MqCreate_TC_19_001_001_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_MqCreate();
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

/* Execute the performance test for R_OSAL_MqCreate */
void R_OSAL_MqCreate_TC_19_001_001(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_MqCreate_TC_19_001_001_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_MqCreate is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_MqDelete */
e_osal_return_t exec_perf_MqDelete()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    osal_mq_handle_t mq_handle = OSAL_MQ_HANDLE_INVALID;
    st_osal_mq_config_t mq_config = {16, 64};

    osal_ret = R_OSAL_MqCreate(&mq_config, TEST_MQ_ID, &mq_handle);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        START_MEASURE
        osal_ret = R_OSAL_MqDelete(mq_handle);
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

/* Execute the performance test for R_OSAL_MqDelete in normal context */
int R_OSAL_MqDelete_TC_19_001_002_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_MqDelete();
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

/* Execute the performance test for R_OSAL_MqDelete */
void R_OSAL_MqDelete_TC_19_001_002(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_MqDelete_TC_19_001_002_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_MqDelete is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_MqSendUntilTimeStamp */
e_osal_return_t exec_perf_MqSendUntilTimeStamp()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    st_osal_time_t abs_time;
    int32_t receive_msg[16];
    osal_milli_sec_t time_period = 0;

    osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        START_MEASURE
        osal_ret = R_OSAL_MqSendUntilTimeStamp(g_mq_handle, &abs_time, SEND_MSG_ARR, sizeof(SEND_MSG_ARR));
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

    /* Post-processing */
    osal_ret = R_OSAL_MqReceiveForTimePeriod(g_mq_handle, time_period, receive_msg, sizeof(receive_msg));
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_MqSendUntilTimeStamp in normal context */
int R_OSAL_MqSendUntilTimeStamp_TC_19_001_003_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_MqSendUntilTimeStamp();
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

/* Execute the performance test for R_OSAL_MqSendUntilTimeStamp in interrupt context */
int R_OSAL_MqSendUntilTimeStamp_TC_19_001_003_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_mq_send_timestamp_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_MqSendUntilTimeStamp */
void R_OSAL_MqSendUntilTimeStamp_TC_19_001_003(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_MqSendUntilTimeStamp_TC_19_001_003_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_MqSendUntilTimeStamp_TC_19_001_003_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor && 0 == ret_isr, api_name, test_num);
}

/* Measure the execution time of R_OSAL_MqSendForTimePeriod */
e_osal_return_t exec_perf_MqSendForTimePeriod()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    osal_milli_sec_t time_period = 0;
    int32_t receive_msg[16];

    START_MEASURE
    osal_ret = R_OSAL_MqSendForTimePeriod(g_mq_handle, time_period, SEND_MSG_ARR, sizeof(SEND_MSG_ARR));
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

    /* Post-processing */
    osal_ret = R_OSAL_MqReceiveForTimePeriod(g_mq_handle, time_period, receive_msg, sizeof(receive_msg));
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_MqSendForTimePeriod in normal context */
int R_OSAL_MqSendForTimePeriod_TC_19_001_004_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_MqSendForTimePeriod();
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

/* Execute the performance test for R_OSAL_MqSendForTimePeriod in interrupt context */
int R_OSAL_MqSendForTimePeriod_TC_19_001_004_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_mq_send_timeperiod_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_MqSendForTimePeriod */
void R_OSAL_MqSendForTimePeriod_TC_19_001_004(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_MqSendForTimePeriod_TC_19_001_004_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_MqSendForTimePeriod_TC_19_001_004_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor && 0 == ret_isr, api_name, test_num);
}

/* Measure the execution time of R_OSAL_MqReceiveUntilTimeStamp */
e_osal_return_t exec_perf_MqReceiveUntilTimeStamp()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    st_osal_time_t abs_time;
    int32_t receive_msg[16];
    osal_milli_sec_t time_period = 1000;

    osal_ret = R_OSAL_MqSendForTimePeriod(g_mq_handle, time_period, SEND_MSG_ARR, sizeof(SEND_MSG_ARR));
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
            osal_ret = R_OSAL_MqReceiveUntilTimeStamp(g_mq_handle, &abs_time, receive_msg, sizeof(receive_msg));
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

    return osal_ret;
}

/* Execute the performance test for R_OSAL_MqReceiveUntilTimeStamp in normal context */
int R_OSAL_MqReceiveUntilTimeStamp_TC_19_001_005_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_MqReceiveUntilTimeStamp();
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

/* Execute the performance test for R_OSAL_MqReceiveUntilTimeStamp in interrupt context */
int R_OSAL_MqReceiveUntilTimeStamp_TC_19_001_005_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_mq_receive_timestamp_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_MqReceiveUntilTimeStamp */
void R_OSAL_MqReceiveUntilTimeStamp_TC_19_001_005(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_MqReceiveUntilTimeStamp_TC_19_001_005_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_MqReceiveUntilTimeStamp_TC_19_001_005_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor && 0 == ret_isr, api_name, test_num);
}

/* Measure the execution time of R_OSAL_MqReceiveForTimePeriod */
e_osal_return_t exec_perf_MqReceiveForTimePeriod()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    int32_t receive_msg[16];
    osal_milli_sec_t time_period = 0;

    osal_ret = R_OSAL_MqSendForTimePeriod(g_mq_handle, time_period, SEND_MSG_ARR, sizeof(SEND_MSG_ARR));
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        START_MEASURE
        osal_ret = R_OSAL_MqReceiveForTimePeriod(g_mq_handle, time_period, receive_msg, sizeof(receive_msg));
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

/* Execute the performance test for R_OSAL_MqReceiveForTimePeriod in normal context */
int R_OSAL_MqReceiveForTimePeriod_TC_19_001_006_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_MqReceiveForTimePeriod();
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

/* Execute the performance test for R_OSAL_MqReceiveForTimePeriod in interrupt context */
int R_OSAL_MqReceiveForTimePeriod_TC_19_001_006_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_mq_receive_timeperiod_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_MqReceiveForTimePeriod */
void R_OSAL_MqReceiveForTimePeriod_TC_19_001_006(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_MqReceiveForTimePeriod_TC_19_001_006_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_MqReceiveForTimePeriod_TC_19_001_006_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor && 0 == ret_isr, api_name, test_num);
}

/* Measure the execution time of R_OSAL_MqIsFull */
e_osal_return_t exec_perf_MqIsFull()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    bool is_full = false;

    START_MEASURE
    osal_ret = R_OSAL_MqIsFull(g_mq_handle, &is_full);
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

/* Execute the performance test for R_OSAL_MqIsFull in normal context */
int R_OSAL_MqIsFull_TC_19_001_007_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_MqIsFull();
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

/* Execute the performance test for R_OSAL_MqIsFull in interrupt context */
int R_OSAL_MqIsFull_TC_19_001_007_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_mq_isfull_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_MqIsFull */
void R_OSAL_MqIsFull_TC_19_001_007(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_MqIsFull_TC_19_001_007_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_MqIsFull_TC_19_001_007_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor && 0 == ret_isr, api_name, test_num);
}

/* Measure the execution time of R_OSAL_MqIsEmpty */
e_osal_return_t exec_perf_MqIsEmpty()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    bool is_empty = false;

    START_MEASURE
    osal_ret = R_OSAL_MqIsEmpty(g_mq_handle, &is_empty);
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

/* Execute the performance test for R_OSAL_MqIsEmpty in normal context */
int R_OSAL_MqIsEmpty_TC_19_001_008_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_MqIsEmpty();
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

/* Execute the performance test for R_OSAL_MqIsEmpty in interrupt context */
int R_OSAL_MqIsEmpty_TC_19_001_008_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_mq_isempty_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_MqIsEmpty */
void R_OSAL_MqIsEmpty_TC_19_001_008(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_MqIsEmpty_TC_19_001_008_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_MqIsEmpty_TC_19_001_008_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor && 0 == ret_isr, api_name, test_num);
}

/* Measure the execution time of R_OSAL_MqGetConfig */
e_osal_return_t exec_perf_MqGetConfig()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    st_osal_mq_config_t config;

    START_MEASURE
    osal_ret = R_OSAL_MqGetConfig(g_mq_handle, &config);
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

/* Execute the performance test for R_OSAL_MqGetConfig in normal context */
int R_OSAL_MqGetConfig_TC_19_001_009_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_MqGetConfig();
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

/* Execute the performance test for R_OSAL_MqGetConfig */
void R_OSAL_MqGetConfig_TC_19_001_009(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_MqGetConfig_TC_19_001_009_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_MqGetConfig is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_MqReset */
e_osal_return_t exec_perf_MqReset()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    osal_milli_sec_t time_period = 0;

    osal_ret = R_OSAL_MqSendForTimePeriod(g_mq_handle, time_period, SEND_MSG_ARR, sizeof(SEND_MSG_ARR));
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        START_MEASURE
        osal_ret = R_OSAL_MqReset(g_mq_handle);
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

/* Execute the performance test for R_OSAL_MqReset in normal context */
int R_OSAL_MqReset_TC_19_001_010_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_MqReset();
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

/* Execute the performance test for R_OSAL_MqReset */
void R_OSAL_MqReset_TC_19_001_010(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_MqReset_TC_19_001_010_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_MqReset is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_MqInitializeMqConfigSt */
e_osal_return_t exec_perf_MqInitializeMqConfigSt()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    st_osal_mq_config_t config;

    START_MEASURE
    osal_ret = R_OSAL_MqInitializeMqConfigSt(&config);
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

/* Execute the performance test for R_OSAL_MqInitializeMqConfigSt in normal context */
int R_OSAL_MqInitializeMqConfigSt_TC_19_001_011_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_MqInitializeMqConfigSt();
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

/* Execute the performance test for R_OSAL_MqInitializeMqConfigSt */
void R_OSAL_MqInitializeMqConfigSt_TC_19_001_011(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_MqInitializeMqConfigSt_TC_19_001_011_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_MqInitializeMqConfigSt is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}
