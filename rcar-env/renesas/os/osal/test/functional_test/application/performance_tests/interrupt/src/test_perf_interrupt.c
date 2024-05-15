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
* File Name :    test_perf_interrupt.c
* Version :      3.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Interrupt Manager Performance Test
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           3.0.0     04.06.2023    Newly create
***********************************************************************************************************************/
#include "common_header.h"

static osal_device_handle_t g_device_handle = OSAL_DEVICE_HANDLE_INVALID;
/* Execute the performance test for OSAL Interrupt APIs */
void exec_perf_interrupt()
{
    e_osal_return_t osal_ret;

    osal_ret = R_OSAL_IoDeviceOpen(TEST_DEV, &g_device_handle);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        TEST(R_OSAL_InterruptGetNumOfIrqChannels, TC_20_001_001);
        TEST(R_OSAL_InterruptRegisterIsr, TC_20_001_002);
        TEST(R_OSAL_InterruptUnregisterIsr, TC_20_001_003);
        TEST(R_OSAL_InterruptEnableIsr, TC_20_001_004);
        TEST(R_OSAL_InterruptDisableIsr, TC_20_001_005);
        TEST(R_OSAL_InterruptIsISRContext, TC_20_001_006);
        TEST(R_OSAL_InterruptSetIsrPriority, TC_20_001_007);
    }

    if(OSAL_DEVICE_HANDLE_INVALID != g_device_handle)
    {
        osal_ret = R_OSAL_IoDeviceClose(g_device_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
    }
}

/* Simple Interrupt callback function for testing Interrupt Manager API */
void osal_test_cb_sample(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
{
    (void)dev_handle;
    (void)irq_channel;
    (void)irq_arg;
}

/* Interrupt callback function for testing R_OSAL_InterruptRegisterIsr */
void isr_interrupt_register_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
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
        osal_ret = exec_perf_InterruptRegisterIsr(user_data->api_name);
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

/* Interrupt callback function for testing R_OSAL_InterruptUnregisterIsr */
void isr_interrupt_unregister_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
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
        osal_ret = exec_perf_InterruptUnregisterIsr(user_data->api_name);
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

/* Interrupt callback function for testing R_OSAL_InterruptEnableIsr */
void isr_interrupt_enable_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
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
        osal_ret = exec_perf_InterruptEnableIsr(user_data->api_name);
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

/* Interrupt callback function for testing R_OSAL_InterruptDisableIsr */
void isr_interrupt_disable_isr_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
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
        osal_ret = exec_perf_InterruptDisableIsr(user_data->api_name);
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

/* Interrupt callback function for testing R_OSAL_InterruptIsISRContext */
void isr_interrupt_is_ISRContext_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
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
        osal_ret = exec_perf_InterruptIsISRContext(user_data->api_name);
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

/* Measure the execution time of R_OSAL_InterruptGetNumOfIrqChannels */
e_osal_return_t exec_perf_InterruptGetNumOfIrqChannels()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    size_t num_of_channel;

    START_MEASURE
    osal_ret = R_OSAL_InterruptGetNumOfIrqChannels(g_device_handle, &num_of_channel);
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

/* Execute the performance test for R_OSAL_InterruptGetNumOfIrqChannels in normal context */
int R_OSAL_InterruptGetNumOfIrqChannels_TC_20_001_001_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_InterruptGetNumOfIrqChannels();
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

/* Execute the performance test for R_OSAL_InterruptGetNumOfIrqChannels in normal and interrupt context */
void R_OSAL_InterruptGetNumOfIrqChannels_TC_20_001_001(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_InterruptGetNumOfIrqChannels_TC_20_001_001_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_InterruptGetNumOfIrqChannels is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_InterruptRegisterIsr */
e_osal_return_t exec_perf_InterruptRegisterIsr()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    uint64_t irq_channel = 0;
    e_osal_interrupt_priority_t priority_value = OSAL_INTERRUPT_PRIORITY_TYPE10;
    void *p_irq_arg = NULL;
    int arg = 10;

    START_MEASURE
    osal_ret = R_OSAL_InterruptRegisterIsr(g_device_handle, irq_channel, priority_value, osal_test_cb_sample, (void *)&arg);
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
        osal_ret = R_OSAL_InterruptUnregisterIsr(g_device_handle, irq_channel, osal_test_cb_sample);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_InterruptRegisterIsr in normal context */
int R_OSAL_InterruptRegisterIsr_TC_20_001_002_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_InterruptRegisterIsr();
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

/* Execute the performance test for R_OSAL_InterruptRegisterIsr in interrupt context */
int R_OSAL_InterruptRegisterIsr_TC_20_001_002_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_interrupt_register_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_InterruptRegisterIsr in normal and interrupt context */
void R_OSAL_InterruptRegisterIsr_TC_20_001_002(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_InterruptRegisterIsr_TC_20_001_002_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_InterruptRegisterIsr_TC_20_001_002_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor && 0 == ret_isr, api_name, test_num);
}

/* Measure the execution time of R_OSAL_InterruptUnregisterIsr */
e_osal_return_t exec_perf_InterruptUnregisterIsr()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    uint64_t irq_channel = 0;
    e_osal_interrupt_priority_t priority_value = OSAL_INTERRUPT_PRIORITY_TYPE10;
    void *p_irq_arg = NULL;
    int arg = 10;

    osal_ret = R_OSAL_InterruptRegisterIsr(g_device_handle, irq_channel, priority_value, osal_test_cb_sample, (void *)&arg);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        START_MEASURE
        osal_ret = R_OSAL_InterruptUnregisterIsr(g_device_handle, irq_channel, osal_test_cb_sample);
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

/* Execute the performance test for R_OSAL_InterruptUnregisterIsr in normal context */
int R_OSAL_InterruptUnregisterIsr_TC_20_001_003_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_InterruptUnregisterIsr();
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

/* Execute the performance test for R_OSAL_InterruptUnregisterIsr in interrupt context */
int R_OSAL_InterruptUnregisterIsr_TC_20_001_003_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_interrupt_unregister_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_InterruptUnregisterIsr in normal and interrupt context */
void R_OSAL_InterruptUnregisterIsr_TC_20_001_003(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_InterruptUnregisterIsr_TC_20_001_003_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_InterruptUnregisterIsr_TC_20_001_003_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor && 0 == ret_isr, api_name, test_num);
}

/* Measure the execution time of R_OSAL_InterruptEnableIsr */
e_osal_return_t exec_perf_InterruptEnableIsr()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    uint64_t irq_channel = 0;
    e_osal_interrupt_priority_t priority_value = OSAL_INTERRUPT_PRIORITY_TYPE10;
    void *p_irq_arg = NULL;
    int arg = 10;

    osal_ret = R_OSAL_InterruptRegisterIsr(g_device_handle, irq_channel, priority_value, osal_test_cb_sample, (void *)&arg);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        START_MEASURE
        osal_ret = R_OSAL_InterruptEnableIsr(g_device_handle, irq_channel);
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
            osal_ret = R_OSAL_InterruptDisableIsr(g_device_handle, irq_channel);
            if(OSAL_RETURN_OK != osal_ret)
            {
                LOG_ERROR("Failure, error = %d\n", osal_ret);
            }
        }
        else
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }

        osal_ret = R_OSAL_InterruptUnregisterIsr(g_device_handle, irq_channel, osal_test_cb_sample);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_InterruptEnableIsr in normal context */
int R_OSAL_InterruptEnableIsr_TC_20_001_004_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_InterruptEnableIsr();
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

/* Execute the performance test for R_OSAL_InterruptEnableIsr in interrupt context */
int R_OSAL_InterruptEnableIsr_TC_20_001_004_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_interrupt_enable_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_InterruptEnableIsr in normal and interrupt context */
void R_OSAL_InterruptEnableIsr_TC_20_001_004(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_InterruptEnableIsr_TC_20_001_004_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_InterruptEnableIsr_TC_20_001_004_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor && 0 == ret_isr, api_name, test_num);
}

/* Measure the execution time of R_OSAL_InterruptDisableIsr */
e_osal_return_t exec_perf_InterruptDisableIsr()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    uint64_t irq_channel = 0;
    e_osal_interrupt_priority_t priority_value = OSAL_INTERRUPT_PRIORITY_TYPE10;
    void *p_irq_arg = NULL;
    int arg = 10;

    osal_ret = R_OSAL_InterruptRegisterIsr(g_device_handle, irq_channel, priority_value, osal_test_cb_sample, (void *)&arg);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        osal_ret = R_OSAL_InterruptEnableIsr(g_device_handle, irq_channel);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
        else
        {
            START_MEASURE
            osal_ret = R_OSAL_InterruptDisableIsr(g_device_handle, irq_channel);
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

        osal_ret = R_OSAL_InterruptUnregisterIsr(g_device_handle, irq_channel, osal_test_cb_sample);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_InterruptDisableIsr in normal context */
int R_OSAL_InterruptDisableIsr_TC_20_001_005_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_InterruptDisableIsr();
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

/* Execute the performance test for R_OSAL_InterruptDisableIsr in interrupt context */
int R_OSAL_InterruptDisableIsr_TC_20_001_005_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_interrupt_disable_isr_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_InterruptDisableIsr in normal and interrupt context */
void R_OSAL_InterruptDisableIsr_TC_20_001_005(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_InterruptDisableIsr_TC_20_001_005_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_InterruptDisableIsr_TC_20_001_005_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor && 0 == ret_isr, api_name, test_num);
}

/* Measure the execution time of R_OSAL_InterruptIsISRContext */
e_osal_return_t exec_perf_InterruptIsISRContext()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    bool is_isr = false;

    START_MEASURE
    osal_ret = R_OSAL_InterruptIsISRContext(&is_isr);
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

/* Execute the performance test for R_OSAL_InterruptIsISRContext in normal context */
int R_OSAL_InterruptIsISRContext_TC_20_001_006_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_InterruptIsISRContext();
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

/* Execute the performance test for R_OSAL_InterruptIsISRContext in interrupt context */
int R_OSAL_InterruptIsISRContext_TC_20_001_006_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_interrupt_is_ISRContext_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_InterruptIsISRContext in normal and interrupt context */
void R_OSAL_InterruptIsISRContext_TC_20_001_006(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_InterruptIsISRContext_TC_20_001_006_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_InterruptIsISRContext_TC_20_001_006_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor && 0 == ret_isr, api_name, test_num);
}

/* Measure the execution time of R_OSAL_InterruptSetIsrPriority */
e_osal_return_t exec_perf_InterruptSetIsrPriority()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    uint64_t irq_channel = 0;
    e_osal_interrupt_priority_t priority_value = OSAL_INTERRUPT_PRIORITY_TYPE10;
    void *p_irq_arg = NULL;
    int arg = 10;

    osal_ret = R_OSAL_InterruptRegisterIsr(g_device_handle, irq_channel, priority_value, osal_test_cb_sample, (void *)&arg);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        osal_ret = R_OSAL_InterruptEnableIsr(g_device_handle, irq_channel);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
        else
        {
            START_MEASURE
            osal_ret = R_OSAL_InterruptSetIsrPriority(g_device_handle, irq_channel, OSAL_INTERRUPT_PRIORITY_TYPE11);
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
            osal_ret = R_OSAL_InterruptDisableIsr(g_device_handle, irq_channel);
            if(OSAL_RETURN_OK != osal_ret)
            {
                LOG_ERROR("Failure, error = %d\n", osal_ret);
            }
        }

        osal_ret = R_OSAL_InterruptUnregisterIsr(g_device_handle, irq_channel, osal_test_cb_sample);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_InterruptSetIsrPriority in normal context */
int R_OSAL_InterruptSetIsrPriority_TC_20_001_007_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_InterruptSetIsrPriority();
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

/* Execute the performance test for R_OSAL_InterruptSetIsrPriority in normal and interrupt context */
void R_OSAL_InterruptSetIsrPriority_TC_20_001_007(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_InterruptSetIsrPriority_TC_20_001_007_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_InterruptSetIsrPriority is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}
