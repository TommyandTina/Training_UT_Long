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
* File Name :    test_perf_io.c
* Version :      3.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL IO Manager Performance Test
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           3.0.0     06.06.2023    Newly create
***********************************************************************************************************************/
#include "common_header.h"

/* Global device handle using for testing IO manager APIs */
static osal_device_handle_t g_device_handle = OSAL_DEVICE_HANDLE_INVALID;
/* Execute the performance test for OSAL IO APIs */
void exec_perf_io()
{
    e_osal_return_t osal_ret;

    TEST(R_OSAL_IoGetNumOfDevices, TC_21_001_001);
    TEST(R_OSAL_IoGetDeviceList, TC_21_001_002);
    TEST(R_OSAL_IoDeviceOpen, TC_21_001_003);
    TEST(R_OSAL_IoDeviceClose, TC_21_001_004);
    TEST(R_OSAL_IoGetAxiBusIdFromDeviceName, TC_21_001_022);
    TEST(R_OSAL_IoGetAxiBusNameList, TC_21_001_023);
    TEST(R_OSAL_IoGetAxiBusName, TC_21_001_024);

    osal_ret = R_OSAL_IoDeviceOpen(TEST_IO_DEV, &g_device_handle);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
#if !defined(TEST_PERF_LINUX_SIL) && !defined(TEST_PERF_WINDOWS)
        TEST(R_OSAL_IoRead8, TC_21_001_005);
        TEST(R_OSAL_IoRead16, TC_21_001_006);
        TEST(R_OSAL_IoRead64, TC_21_001_008);
        TEST(R_OSAL_IoWrite8, TC_21_001_009);
        TEST(R_OSAL_IoWrite16, TC_21_001_010);
        TEST(R_OSAL_IoWrite64, TC_21_001_012);
        TEST(R_OSAL_IoBlockRead8, TC_21_001_013);
        TEST(R_OSAL_IoBlockRead16, TC_21_001_014);
        TEST(R_OSAL_IoBlockRead64, TC_21_001_016);
        TEST(R_OSAL_IoBlockWrite8, TC_21_001_017);
        TEST(R_OSAL_IoBlockWrite16, TC_21_001_018);
        TEST(R_OSAL_IoBlockWrite64, TC_21_001_020);
#endif
        TEST(R_OSAL_IoRead32, TC_21_001_007);
        TEST(R_OSAL_IoWrite32, TC_21_001_011);
        TEST(R_OSAL_IoBlockRead32, TC_21_001_015);
        TEST(R_OSAL_IoBlockWrite32, TC_21_001_019);
        TEST(R_OSAL_IoGetDeviceAxiBusId, TC_21_001_021);
        TEST(R_OSAL_IoGetNumOfAxiIdsForAxiBusName, TC_21_001_025);
        TEST(R_OSAL_IoGetAxiIdListForAxiBusName, TC_21_001_026);
        TEST(R_OSAL_IoGetNumOfAxiIdsForDeviceHdl, TC_21_001_027);
        TEST(R_OSAL_IoGetAxiIdListForDeviceHdl, TC_21_001_028);
        TEST(R_OSAL_IoGetNumOfAxiIdsForDeviceName, TC_21_001_029);
        TEST(R_OSAL_IoGetAxiIdListForDeviceName, TC_21_001_030);

        osal_ret = R_OSAL_IoDeviceClose(g_device_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
    }
}

#if !defined(TEST_PERF_LINUX_SIL) && !defined(TEST_PERF_WINDOWS)
/* Interrupt callback function for testing R_OSAL_IoRead8 */
void isr_io_read8_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
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
        osal_ret = exec_perf_IoRead8(TEST_IO_READ8_OFFSET_ISR);
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

/* Interrupt callback function for testing R_OSAL_IoRead16 */
void isr_io_read16_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
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
        osal_ret = exec_perf_IoRead16(TEST_IO_READ16_OFFSET_ISR);
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
#endif

/* Interrupt callback function for testing R_OSAL_IoRead32 */
void isr_io_read32_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
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
        osal_ret = exec_perf_IoRead32(TEST_IO_READ32_OFFSET_ISR);
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

#if !defined(TEST_PERF_LINUX_SIL) && !defined(TEST_PERF_WINDOWS)
/* Interrupt callback function for testing R_OSAL_IoRead64 */
void isr_io_read64_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
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
        osal_ret = exec_perf_IoRead64(TEST_IO_READ64_OFFSET_ISR);
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
#endif

#if !defined(TEST_PERF_LINUX_SIL) && !defined(TEST_PERF_WINDOWS)
/* Interrupt callback function for testing R_OSAL_IoWrite8 */
void isr_io_write8_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
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
        osal_ret = exec_perf_IoWrite8(TEST_IO_WRITE8_OFFSET_ISR);
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

/* Interrupt callback function for testing R_OSAL_IoWrite16 */
void isr_io_write16_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
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
        osal_ret = exec_perf_IoWrite16(TEST_IO_WRITE16_OFFSET_ISR);
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
#endif

/* Interrupt callback function for testing R_OSAL_IoWrite32 */
void isr_io_write32_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
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
        osal_ret = exec_perf_IoWrite32(TEST_IO_WRITE32_OFFSET_ISR);
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

#if !defined(TEST_PERF_LINUX_SIL) && !defined(TEST_PERF_WINDOWS)
/* Interrupt callback function for testing R_OSAL_IoWrite64 */
void isr_io_write64_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
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
        osal_ret = exec_perf_IoWrite64(TEST_IO_WRITE64_OFFSET_ISR);
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
#endif

#if !defined(TEST_PERF_LINUX_SIL) && !defined(TEST_PERF_WINDOWS)
/* Interrupt callback function for testing R_OSAL_IoBlockRead8 */
void isr_io_blockread8_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
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
        osal_ret = exec_perf_IoBlockRead8(TEST_IO_BLOCKREAD8_OFFSET_ISR);
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

/* Interrupt callback function for testing R_OSAL_IoBlockRead16 */
void isr_io_blockread16_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
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
        osal_ret = exec_perf_IoBlockRead16(TEST_IO_BLOCKREAD16_OFFSET_ISR);
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
#endif

/* Interrupt callback function for testing R_OSAL_IoBlockRead32 */
void isr_io_blockread32_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
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
        osal_ret = exec_perf_IoBlockRead32(TEST_IO_BLOCKREAD32_OFFSET_ISR);
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

#if !defined(TEST_PERF_LINUX_SIL) && !defined(TEST_PERF_WINDOWS)
/* Interrupt callback function for testing R_OSAL_IoBlockRead64 */
void isr_io_blockread64_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
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
        osal_ret = exec_perf_IoBlockRead64(TEST_IO_BLOCKREAD64_OFFSET_ISR);
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
#endif

#if !defined(TEST_PERF_LINUX_SIL) && !defined(TEST_PERF_WINDOWS)
/* Interrupt callback function for testing R_OSAL_IoBlockWrite8 */
void isr_io_blockwrite8_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
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
        osal_ret = exec_perf_IoBlockWrite8(TEST_IO_BLOCKWRITE8_OFFSET_ISR);
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

/* Interrupt callback function for testing R_OSAL_IoBlockWrite16 */
void isr_io_blockwrite16_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
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
        osal_ret = exec_perf_IoBlockWrite16(TEST_IO_BLOCKWRITE16_OFFSET_ISR);
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
#endif

/* Interrupt callback function for testing R_OSAL_IoBlockWrite32 */
void isr_io_blockwrite32_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
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
        osal_ret = exec_perf_IoBlockWrite32(TEST_IO_BLOCKWRITE32_OFFSET_ISR);
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

#if !defined(TEST_PERF_LINUX_SIL) && !defined(TEST_PERF_WINDOWS)
/* Interrupt callback function for testing R_OSAL_IoBlockWrite64 */
void isr_io_blockwrite64_callback_function(osal_device_handle_t dev_handle, uint64_t irq_channel, void *irq_arg)
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
        osal_ret = exec_perf_IoBlockWrite64(TEST_IO_BLOCKWRITE64_OFFSET_ISR);
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
#endif

/* Measure the execution time of R_OSAL_IoGetNumOfDevices */
e_osal_return_t exec_perf_IoGetNumOfDevices()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    size_t numOfDevices;

    START_MEASURE
    osal_ret = R_OSAL_IoGetNumOfDevices(TEST_IO_DEV_TYPE, &numOfDevices);
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

/* Execute the performance test for R_OSAL_IoGetNumOfDevices in normal context */
int R_OSAL_IoGetNumOfDevices_TC_21_001_001_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_IoGetNumOfDevices();
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

/* Execute the performance test for R_OSAL_IoGetNumOfDevices in normal and interrupt context */
void R_OSAL_IoGetNumOfDevices_TC_21_001_001(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_IoGetNumOfDevices_TC_21_001_001_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_IoGetNumOfDevices is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_IoGetDeviceList */
e_osal_return_t exec_perf_IoGetDeviceLists()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    void *p_buffer;
    size_t size;
    size_t byte_written;

    osal_ret = R_OSAL_IoGetDeviceList(TEST_IO_DEV_TYPE, NULL, 0, &size);
    if(OSAL_RETURN_OK_RQST != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        p_buffer = (char*)malloc(size);
        START_MEASURE
        osal_ret = R_OSAL_IoGetDeviceList(TEST_IO_DEV_TYPE, p_buffer, size, &byte_written);
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

        free(p_buffer);
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_IoGetDeviceList in normal context */
int R_OSAL_IoGetDeviceList_TC_21_001_002_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_IoGetDeviceLists();
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

/* Execute the performance test for R_OSAL_IoGetDeviceList in normal and interrupt context */
void R_OSAL_IoGetDeviceList_TC_21_001_002(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_IoGetDeviceList_TC_21_001_002_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_IoGetDeviceList is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_IoDeviceOpen */
e_osal_return_t exec_perf_IoDeviceOpen()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    osal_device_handle_t local_device_handle = OSAL_DEVICE_HANDLE_INVALID;

    START_MEASURE
    osal_ret = R_OSAL_IoDeviceOpen(TEST_DEV, &local_device_handle);
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

    osal_ret = R_OSAL_IoDeviceClose(local_device_handle);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_IoDeviceOpen in normal context */
int R_OSAL_IoDeviceOpen_TC_21_001_003_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_IoDeviceOpen();
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

/* Execute the performance test for R_OSAL_IoDeviceOpen in normal and interrupt context */
void R_OSAL_IoDeviceOpen_TC_21_001_003(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_IoDeviceOpen_TC_21_001_003_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_IoDeviceOpen is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_IoDeviceClose */
e_osal_return_t exec_perf_IoDeviceClose()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    osal_device_handle_t local_device_handle = OSAL_DEVICE_HANDLE_INVALID;

    osal_ret = R_OSAL_IoDeviceOpen(TEST_DEV, &local_device_handle);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        START_MEASURE
        osal_ret = R_OSAL_IoDeviceClose(local_device_handle);
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

/* Execute the performance test for R_OSAL_IoDeviceClose in normal context */
int R_OSAL_IoDeviceClose_TC_21_001_004_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_IoDeviceClose();
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

/* Execute the performance test for R_OSAL_IoDeviceClose in normal and interrupt context */
void R_OSAL_IoDeviceClose_TC_21_001_004(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_IoDeviceClose_TC_21_001_004_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_IoDeviceClose is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

#if !defined(TEST_PERF_LINUX_SIL) && !defined(TEST_PERF_WINDOWS)
/* Measure the execution time of R_OSAL_IoRead8 */
e_osal_return_t exec_perf_IoRead8(uintptr_t offset)
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    uint8_t data;

    START_MEASURE
    osal_ret = R_OSAL_IoRead8(g_device_handle, offset, &data);
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

/* Execute the performance test for R_OSAL_IoRead8 in normal context */
int R_OSAL_IoRead8_TC_21_001_005_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_IoRead8(TEST_IO_READ8_OFFSET_NOR);
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

/* Execute the performance test for R_OSAL_IoRead8 in interrupt context */
int R_OSAL_IoRead8_TC_21_001_005_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_io_read8_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_IoRead8 in normal and interrupt context */
void R_OSAL_IoRead8_TC_21_001_005(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_IoRead8_TC_21_001_005_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_IoRead8_TC_21_001_005_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor && 0 == ret_isr, api_name, test_num);
}

/* Measure the execution time of R_OSAL_IoRead16 */
e_osal_return_t exec_perf_IoRead16(uintptr_t offset)
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    uint16_t data;

    START_MEASURE
    osal_ret = R_OSAL_IoRead16(g_device_handle, offset, &data);
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

/* Execute the performance test for R_OSAL_IoRead16 in normal context */
int R_OSAL_IoRead16_TC_21_001_006_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_IoRead16(TEST_IO_READ16_OFFSET_NOR);
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

/* Execute the performance test for R_OSAL_IoRead16 in interrupt context */
int R_OSAL_IoRead16_TC_21_001_006_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_io_read16_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_IoRead16 in normal and interrupt context */
void R_OSAL_IoRead16_TC_21_001_006(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_IoRead16_TC_21_001_006_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_IoRead16_TC_21_001_006_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor && 0 == ret_isr, api_name, test_num);
}
#endif

/* Measure the execution time of R_OSAL_IoRead32 */
e_osal_return_t exec_perf_IoRead32(uintptr_t offset)
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    uint32_t data;

    START_MEASURE
    osal_ret = R_OSAL_IoRead32(g_device_handle, offset, &data);
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

/* Execute the performance test for R_OSAL_IoRead32 in normal context */
int R_OSAL_IoRead32_TC_21_001_007_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_IoRead32(TEST_IO_READ32_OFFSET_NOR);
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

/* Execute the performance test for R_OSAL_IoRead32 in interrupt context */
int R_OSAL_IoRead32_TC_21_001_007_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_io_read32_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_IoRead32 in normal and interrupt context */
void R_OSAL_IoRead32_TC_21_001_007(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_IoRead32_TC_21_001_007_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_IoRead32_TC_21_001_007_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor && 0 == ret_isr, api_name, test_num);
}

#if !defined(TEST_PERF_LINUX_SIL) && !defined(TEST_PERF_WINDOWS)
/* Measure the execution time of R_OSAL_IoRead64 */
e_osal_return_t exec_perf_IoRead64(uintptr_t offset)
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    uint64_t data;

    START_MEASURE
    osal_ret = R_OSAL_IoRead64(g_device_handle, offset, &data);
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

/* Execute the performance test for R_OSAL_IoRead64 in normal context */
int R_OSAL_IoRead64_TC_21_001_008_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_IoRead64(TEST_IO_READ64_OFFSET_NOR);
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

/* Execute the performance test for R_OSAL_IoRead64 in interrupt context */
int R_OSAL_IoRead64_TC_21_001_008_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_io_read64_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_IoRead64 in normal and interrupt context */
void R_OSAL_IoRead64_TC_21_001_008(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_IoRead64_TC_21_001_008_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_IoRead64_TC_21_001_008_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor && 0 == ret_isr, api_name, test_num);
}
#endif

#if !defined(TEST_PERF_LINUX_SIL) && !defined(TEST_PERF_WINDOWS)
/* Measure the execution time of R_OSAL_IoWrite8 */
e_osal_return_t exec_perf_IoWrite8(uintptr_t offset)
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    uint8_t data = TEST_IO_WRITE8_VAL;

    START_MEASURE
    osal_ret = R_OSAL_IoWrite8(g_device_handle, offset, data);
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

/* Execute the performance test for R_OSAL_IoWrite8 in normal context */
int R_OSAL_IoWrite8_TC_21_001_009_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_IoWrite8(TEST_IO_WRITE8_OFFSET_NOR);
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

/* Execute the performance test for R_OSAL_IoWrite8 in interrupt context */
int R_OSAL_IoWrite8_TC_21_001_009_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_io_write8_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_IoWrite8 in normal and interrupt context */
void R_OSAL_IoWrite8_TC_21_001_009(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_IoWrite8_TC_21_001_009_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_IoWrite8_TC_21_001_009_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor && 0 == ret_isr, api_name, test_num);
}

/* Measure the execution time of R_OSAL_IoWrite16 */
e_osal_return_t exec_perf_IoWrite16(uintptr_t offset)
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    uint16_t data = TEST_IO_WRITE16_VAL;

    START_MEASURE
    osal_ret = R_OSAL_IoWrite16(g_device_handle, offset, data);
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

/* Execute the performance test for R_OSAL_IoWrite16 in normal context */
int R_OSAL_IoWrite16_TC_21_001_010_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_IoWrite16(TEST_IO_WRITE16_OFFSET_NOR);
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

/* Execute the performance test for R_OSAL_IoWrite16 in interrupt context */
int R_OSAL_IoWrite16_TC_21_001_010_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_io_write16_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_IoWrite16 in normal and interrupt context */
void R_OSAL_IoWrite16_TC_21_001_010(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_IoWrite16_TC_21_001_010_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_IoWrite16_TC_21_001_010_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor && 0 == ret_isr, api_name, test_num);
}
#endif

/* Measure the execution time of R_OSAL_IoWrite32 */
e_osal_return_t exec_perf_IoWrite32(uintptr_t offset)
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    uint32_t data = TEST_IO_WRITE32_VAL;

    START_MEASURE
    osal_ret = R_OSAL_IoWrite32(g_device_handle, offset, data);
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

/* Execute the performance test for R_OSAL_IoWrite32 in normal context */
int R_OSAL_IoWrite32_TC_21_001_011_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_IoWrite32(TEST_IO_WRITE32_OFFSET_NOR);
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

/* Execute the performance test for R_OSAL_IoWrite32 in interrupt context */
int R_OSAL_IoWrite32_TC_21_001_011_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_io_write32_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_IoWrite32 in normal and interrupt context */
void R_OSAL_IoWrite32_TC_21_001_011(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_IoWrite32_TC_21_001_011_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_IoWrite32_TC_21_001_011_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor && 0 == ret_isr, api_name, test_num);
}

#if !defined(TEST_PERF_LINUX_SIL) && !defined(TEST_PERF_WINDOWS)
/* Measure the execution time of R_OSAL_IoWrite64 */
e_osal_return_t exec_perf_IoWrite64(uintptr_t offset)
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    uint64_t data = TEST_IO_WRITE64_VAL;

    START_MEASURE
    osal_ret = R_OSAL_IoWrite64(g_device_handle, offset, data);
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

/* Execute the performance test for R_OSAL_IoWrite64 in normal context */
int R_OSAL_IoWrite64_TC_21_001_012_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_IoWrite64(TEST_IO_WRITE64_OFFSET_NOR);
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

/* Execute the performance test for R_OSAL_IoWrite64 in interrupt context */
int R_OSAL_IoWrite64_TC_21_001_012_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_io_write64_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_IoWrite64 in normal and interrupt context */
void R_OSAL_IoWrite64_TC_21_001_012(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_IoWrite64_TC_21_001_012_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_IoWrite64_TC_21_001_012_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor && 0 == ret_isr, api_name, test_num);
}
#endif

#if !defined(TEST_PERF_LINUX_SIL) && !defined(TEST_PERF_WINDOWS)
/* Measure the execution time of R_OSAL_IoBlockRead8 */
e_osal_return_t exec_perf_IoBlockRead8(uintptr_t offset)
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    uint8_t data[4];
    size_t size = 4;

    START_MEASURE
    osal_ret = R_OSAL_IoBlockRead8(g_device_handle, offset, data, size);
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

/* Execute the performance test for R_OSAL_IoBlockRead8 in normal context */
int R_OSAL_IoBlockRead8_TC_21_001_013_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_IoBlockRead8(TEST_IO_BLOCKREAD8_OFFSET_NOR);
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

/* Execute the performance test for R_OSAL_IoBlockRead8 in interrupt context */
int R_OSAL_IoBlockRead8_TC_21_001_013_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_io_blockread8_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_IoBlockRead8 in normal and interrupt context */
void R_OSAL_IoBlockRead8_TC_21_001_013(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_IoBlockRead8_TC_21_001_013_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_IoBlockRead8_TC_21_001_013_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor && 0 == ret_isr, api_name, test_num);
}

/* Measure the execution time of R_OSAL_IoBlockRead16 */
e_osal_return_t exec_perf_IoBlockRead16(uintptr_t offset)
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    uint16_t data[2];
    size_t size = 4;

    START_MEASURE
    osal_ret = R_OSAL_IoBlockRead16(g_device_handle, offset, data, size);
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

/* Execute the performance test for R_OSAL_IoBlockRead16 in normal context */
int R_OSAL_IoBlockRead16_TC_21_001_014_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_IoBlockRead16(TEST_IO_BLOCKREAD16_OFFSET_NOR);
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

/* Execute the performance test for R_OSAL_IoBlockRead16 in interrupt context */
int R_OSAL_IoBlockRead16_TC_21_001_014_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_io_blockread16_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_IoBlockRead16 in normal and interrupt context */
void R_OSAL_IoBlockRead16_TC_21_001_014(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_IoBlockRead16_TC_21_001_014_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_IoBlockRead16_TC_21_001_014_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor && 0 == ret_isr, api_name, test_num);
}
#endif

/* Measure the execution time of R_OSAL_IoBlockRead32 */
e_osal_return_t exec_perf_IoBlockRead32(uintptr_t offset)
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    uint32_t data[2];
    size_t size = 8;

    START_MEASURE
    osal_ret = R_OSAL_IoBlockRead32(g_device_handle, offset, data, size);
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

/* Execute the performance test for R_OSAL_IoBlockRead32 in normal context */
int R_OSAL_IoBlockRead32_TC_21_001_015_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_IoBlockRead32(TEST_IO_BLOCKREAD32_OFFSET_NOR);
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

/* Execute the performance test for R_OSAL_IoBlockRead32 in interrupt context */
int R_OSAL_IoBlockRead32_TC_21_001_015_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_io_blockread32_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_IoBlockRead32 in normal and interrupt context */
void R_OSAL_IoBlockRead32_TC_21_001_015(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_IoBlockRead32_TC_21_001_015_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_IoBlockRead32_TC_21_001_015_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor && 0 == ret_isr, api_name, test_num);
}

#if !defined(TEST_PERF_LINUX_SIL) && !defined(TEST_PERF_WINDOWS)
/* Measure the execution time of R_OSAL_IoBlockRead64 */
e_osal_return_t exec_perf_IoBlockRead64(uintptr_t offset)
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    size_t size = 8;
    uint64_t data[1];

    START_MEASURE
    osal_ret = R_OSAL_IoBlockRead64(g_device_handle, offset, data, size);
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

/* Execute the performance test for R_OSAL_IoBlockRead64 in normal context */
int R_OSAL_IoBlockRead64_TC_21_001_016_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_IoBlockRead64(TEST_IO_BLOCKREAD64_OFFSET_NOR);
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

/* Execute the performance test for R_OSAL_IoBlockRead64 in interrupt context */
int R_OSAL_IoBlockRead64_TC_21_001_016_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_io_blockread64_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_IoBlockRead64 in normal and interrupt context */
void R_OSAL_IoBlockRead64_TC_21_001_016(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_IoBlockRead64_TC_21_001_016_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_IoBlockRead64_TC_21_001_016_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor && 0 == ret_isr, api_name, test_num);
}
#endif

#if !defined(TEST_PERF_LINUX_SIL) && !defined(TEST_PERF_WINDOWS)
/* Measure the execution time of R_OSAL_IoBlockWrite8 */
e_osal_return_t exec_perf_IoBlockWrite8(uintptr_t offset)
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    uint8_t data[16] = {TEST_IO_WRITEBLOCK8_VAL[0], TEST_IO_WRITEBLOCK8_VAL[1],
                                  TEST_IO_WRITEBLOCK8_VAL[2], TEST_IO_WRITEBLOCK8_VAL[3],
                                  TEST_IO_WRITEBLOCK8_VAL[4], TEST_IO_WRITEBLOCK8_VAL[5],
                                  TEST_IO_WRITEBLOCK8_VAL[6], TEST_IO_WRITEBLOCK8_VAL[7],
                                  TEST_IO_WRITEBLOCK8_VAL[8], TEST_IO_WRITEBLOCK8_VAL[9],
                                  TEST_IO_WRITEBLOCK8_VAL[10], TEST_IO_WRITEBLOCK8_VAL[11],
                                  TEST_IO_WRITEBLOCK8_VAL[12], TEST_IO_WRITEBLOCK8_VAL[13],
                                  TEST_IO_WRITEBLOCK8_VAL[14], TEST_IO_WRITEBLOCK8_VAL[15]};
    size_t size = 16;

    START_MEASURE
    osal_ret = R_OSAL_IoBlockWrite8(g_device_handle, offset, data, size);
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

/* Execute the performance test for R_OSAL_IoBlockWrite8 in normal context */
int R_OSAL_IoBlockWrite8_TC_21_001_017_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_IoBlockWrite8(TEST_IO_BLOCKWRITE8_OFFSET_NOR);
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

/* Execute the performance test for R_OSAL_IoBlockWrite8 in interrupt context */
int R_OSAL_IoBlockWrite8_TC_21_001_017_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_io_blockwrite8_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_IoBlockWrite8 in normal and interrupt context */
void R_OSAL_IoBlockWrite8_TC_21_001_017(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_IoBlockWrite8_TC_21_001_017_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_IoBlockWrite8_TC_21_001_017_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor && 0 == ret_isr, api_name, test_num);
}

/* Measure the execution time of R_OSAL_IoBlockWrite16 */
e_osal_return_t exec_perf_IoBlockWrite16(uintptr_t offset)
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    uint16_t data[8] = {TEST_IO_WRITEBLOCK16_VAL[0], TEST_IO_WRITEBLOCK16_VAL[1],
                                   TEST_IO_WRITEBLOCK16_VAL[2], TEST_IO_WRITEBLOCK16_VAL[3],
                                   TEST_IO_WRITEBLOCK16_VAL[4], TEST_IO_WRITEBLOCK16_VAL[5],
                                   TEST_IO_WRITEBLOCK16_VAL[6], TEST_IO_WRITEBLOCK16_VAL[7]};
    size_t size = 8;

    START_MEASURE
    osal_ret = R_OSAL_IoBlockWrite16(g_device_handle, offset, data, size);
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

/* Execute the performance test for R_OSAL_IoBlockWrite16 in normal context */
int R_OSAL_IoBlockWrite16_TC_21_001_018_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_IoBlockWrite16(TEST_IO_BLOCKWRITE16_OFFSET_NOR);
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

/* Execute the performance test for R_OSAL_IoBlockWrite16 in interrupt context */
int R_OSAL_IoBlockWrite16_TC_21_001_018_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_io_blockwrite16_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_IoBlockWrite16 in normal and interrupt context */
void R_OSAL_IoBlockWrite16_TC_21_001_018(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_IoBlockWrite16_TC_21_001_018_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_IoBlockWrite16_TC_21_001_018_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor && 0 == ret_isr, api_name, test_num);
}
#endif

/* Measure the execution time of R_OSAL_IoBlockWrite32 */
e_osal_return_t exec_perf_IoBlockWrite32(uintptr_t offset)
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    uint32_t data[4] = {TEST_IO_WRITEBLOCK32_VAL[0], TEST_IO_WRITEBLOCK32_VAL[1],
                                    TEST_IO_WRITEBLOCK32_VAL[2], TEST_IO_WRITEBLOCK32_VAL[3]};
    size_t size = 4;

    START_MEASURE
    osal_ret = R_OSAL_IoBlockWrite32(g_device_handle, offset, data, size);
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

/* Execute the performance test for R_OSAL_IoBlockWrite32 in normal context */
int R_OSAL_IoBlockWrite32_TC_21_001_019_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_IoBlockWrite32(TEST_IO_BLOCKWRITE32_OFFSET_NOR);
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

/* Execute the performance test for R_OSAL_IoBlockWrite32 in interrupt context */
int R_OSAL_IoBlockWrite32_TC_21_001_019_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_io_blockwrite32_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_IoBlockWrite32 in normal and interrupt context */
void R_OSAL_IoBlockWrite32_TC_21_001_019(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_IoBlockWrite32_TC_21_001_019_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_IoBlockWrite32_TC_21_001_019_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor && 0 == ret_isr, api_name, test_num);
}

#if !defined(TEST_PERF_LINUX_SIL) && !defined(TEST_PERF_WINDOWS)
/* Measure the execution time of R_OSAL_IoBlockWrite64 */
e_osal_return_t exec_perf_IoBlockWrite64(uintptr_t offset)
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    uint64_t data[2] = {TEST_IO_WRITEBLOCK64_VAL[0], TEST_IO_WRITEBLOCK64_VAL[1]};
    size_t size = 16;

    START_MEASURE
    osal_ret = R_OSAL_IoBlockWrite64(g_device_handle, offset, data, size);
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

/* Execute the performance test for R_OSAL_IoBlockWrite64 in normal context */
int R_OSAL_IoBlockWrite64_TC_21_001_020_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_IoBlockWrite64(TEST_IO_BLOCKWRITE64_OFFSET_NOR);
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

/* Execute the performance test for R_OSAL_IoBlockWrite64 in interrupt context */
int R_OSAL_IoBlockWrite64_TC_21_001_020_ISR(const char* func, uint64_t expected_time)
{
    int ret;

    /* ISR context */
    ret = generate_interrupt(isr_io_blockwrite64_callback_function, func);
    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Interrupt Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_IoBlockWrite64 in normal and interrupt context */
void R_OSAL_IoBlockWrite64_TC_21_001_020(uint64_t expected_time)
{
    int ret_nor;
    int ret_isr;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_IoBlockWrite64_TC_21_001_020_NOR(api_name, expected_time);

    /* ISR context */
    ret_isr = R_OSAL_IoBlockWrite64_TC_21_001_020_ISR(api_name, expected_time);

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor && 0 == ret_isr, api_name, test_num);
}
#endif

/* Measure the execution time of R_OSAL_IoGetDeviceAxiBusId */
e_osal_return_t exec_perf_IoGetDeviceAxiBusId()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    osal_axi_bus_id_t axi_id;

    START_MEASURE
    osal_ret = R_OSAL_IoGetDeviceAxiBusId(g_device_handle, &axi_id);
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

/* Execute the performance test for R_OSAL_IoGetDeviceAxiBusId in normal context */
int R_OSAL_IoGetDeviceAxiBusId_TC_21_001_021_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_IoGetDeviceAxiBusId();
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

/* Execute the performance test for R_OSAL_IoGetDeviceAxiBusId in normal and interrupt context */
void R_OSAL_IoGetDeviceAxiBusId_TC_21_001_021(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_IoGetDeviceAxiBusId_TC_21_001_021_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_IoGetDeviceAxiBusId is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_IoGetAxiBusIdFromDeviceName */
e_osal_return_t exec_perf_IoGetAxiBusIdFromDeviceName()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    osal_axi_bus_id_t axi_id;

    START_MEASURE
    osal_ret = R_OSAL_IoGetAxiBusIdFromDeviceName(TEST_IO_DEV, &axi_id);
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

/* Execute the performance test for R_OSAL_IoGetAxiBusIdFromDeviceName in normal context */
int R_OSAL_IoGetAxiBusIdFromDeviceName_TC_21_001_022_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_IoGetAxiBusIdFromDeviceName();
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

/* Execute the performance test for R_OSAL_IoGetAxiBusIdFromDeviceName in normal and interrupt context */
void R_OSAL_IoGetAxiBusIdFromDeviceName_TC_21_001_022(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_IoGetAxiBusIdFromDeviceName_TC_21_001_022_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_IoGetAxiBusIdFromDeviceName is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_IoGetAxiBusNameList */
e_osal_return_t exec_perf_IoGetAxiBusNameList()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    char *p_buffer = NULL;
    size_t busname_list_size;
    size_t busname_list_byte_written;
    char *axi_name_list = NULL;

    osal_ret = R_OSAL_IoGetAxiBusNameList(p_buffer, 0, &busname_list_size);
    if(OSAL_RETURN_OK_RQST != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        axi_name_list = (char*)malloc(busname_list_size);
        START_MEASURE
        osal_ret = R_OSAL_IoGetAxiBusNameList(axi_name_list, busname_list_size, &busname_list_byte_written);
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

        free(axi_name_list);
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_IoGetAxiBusNameList in normal context */
int R_OSAL_IoGetAxiBusNameList_TC_21_001_023_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_IoGetAxiBusNameList();
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

/* Execute the performance test for R_OSAL_IoGetAxiBusNameList in normal and interrupt context */
void R_OSAL_IoGetAxiBusNameList_TC_21_001_023(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_IoGetAxiBusNameList_TC_21_001_023_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_IoGetAxiBusNameList is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_IoGetAxiBusName */
e_osal_return_t exec_perf_IoGetAxiBusName()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    size_t requiredBufferSize = 0;
    size_t writtenBytes = 0;
    char *p_buffer;

    osal_ret = R_OSAL_IoGetAxiBusName(OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY, NULL, 0, &requiredBufferSize);
    if(OSAL_RETURN_OK_RQST != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        p_buffer = (char*)malloc(requiredBufferSize);
        START_MEASURE
        osal_ret = R_OSAL_IoGetAxiBusName(OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY, p_buffer, requiredBufferSize, &writtenBytes);
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

        free(p_buffer);
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_IoGetAxiBusName in normal context */
int R_OSAL_IoGetAxiBusName_TC_21_001_024_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_IoGetAxiBusName();
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

/* Execute the performance test for R_OSAL_IoGetAxiBusName in normal and interrupt context */
void R_OSAL_IoGetAxiBusName_TC_21_001_024(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_IoGetAxiBusName_TC_21_001_024_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_IoGetAxiBusName is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_IoGetNumOfAxiIdsForAxiBusName */
e_osal_return_t exec_perf_IoGetNumOfAxiIdsForAxiBusName()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    size_t num_of_axi_ids;

    START_MEASURE
    osal_ret = R_OSAL_IoGetNumOfAxiIdsForAxiBusName(TEST_AXI_BUS_NAME, &num_of_axi_ids);
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

/* Execute the performance test for R_OSAL_IoGetNumOfAxiIdsForAxiBusName in normal context */
int R_OSAL_IoGetNumOfAxiIdsForAxiBusName_TC_21_001_025_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_IoGetNumOfAxiIdsForAxiBusName();
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

/* Execute the performance test for R_OSAL_IoGetNumOfAxiIdsForAxiBusName in normal and interrupt context */
void R_OSAL_IoGetNumOfAxiIdsForAxiBusName_TC_21_001_025(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_IoGetNumOfAxiIdsForAxiBusName_TC_21_001_025_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_IoGetNumOfAxiIdsForAxiBusName is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_IoGetAxiIdListForAxiBusName */
e_osal_return_t exec_perf_IoGetAxiIdListForAxiBusName()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    osal_axi_bus_id_t *buffer;
    size_t num_of_written_axi_ids = 0;

    buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * TEST_NUM_OF_AXI_ID);
    START_MEASURE
    osal_ret = R_OSAL_IoGetAxiIdListForAxiBusName(TEST_AXI_BUS_NAME, buffer, TEST_NUM_OF_AXI_ID, &num_of_written_axi_ids);
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

    free(buffer);

    return osal_ret;
}

/* Execute the performance test for R_OSAL_IoGetAxiIdListForAxiBusName in normal context */
int R_OSAL_IoGetAxiIdListForAxiBusName_TC_21_001_026_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_IoGetAxiIdListForAxiBusName();
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

/* Execute the performance test for R_OSAL_IoGetAxiIdListForAxiBusName in normal and interrupt context */
void R_OSAL_IoGetAxiIdListForAxiBusName_TC_21_001_026(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_IoGetAxiIdListForAxiBusName_TC_21_001_026_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_IoGetAxiIdListForAxiBusName is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_IoGetNumOfAxiIdsForDeviceHdl */
e_osal_return_t exec_perf_IoGetNumOfAxiIdsForDeviceHdl()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    size_t num_of_axi_ids;

    START_MEASURE
    osal_ret = R_OSAL_IoGetNumOfAxiIdsForDeviceHdl(g_device_handle, &num_of_axi_ids);
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

/* Execute the performance test for R_OSAL_IoGetNumOfAxiIdsForDeviceHdl in normal context */
int R_OSAL_IoGetNumOfAxiIdsForDeviceHdl_TC_21_001_027_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_IoGetNumOfAxiIdsForDeviceHdl();
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

/* Execute the performance test for R_OSAL_IoGetNumOfAxiIdsForDeviceHdl in normal and interrupt context */
void R_OSAL_IoGetNumOfAxiIdsForDeviceHdl_TC_21_001_027(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_IoGetNumOfAxiIdsForDeviceHdl_TC_21_001_027_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_IoGetNumOfAxiIdsForDeviceHdl is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_IoGetAxiIdListForDeviceHdl */
e_osal_return_t exec_perf_IoGetAxiIdListForDeviceHdl()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    osal_axi_bus_id_t *buffer;
    size_t num_of_written_axi_ids;
    size_t num_of_axi_ids;

    osal_ret = R_OSAL_IoGetNumOfAxiIdsForDeviceHdl(g_device_handle, &num_of_axi_ids);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_axi_ids);
        START_MEASURE
        osal_ret = R_OSAL_IoGetAxiIdListForDeviceHdl(g_device_handle, buffer, num_of_axi_ids, &num_of_written_axi_ids);
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

        free(buffer);
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_IoGetAxiIdListForDeviceHdl in normal context */
int R_OSAL_IoGetAxiIdListForDeviceHdl_TC_21_001_028_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_IoGetAxiIdListForDeviceHdl();
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

/* Execute the performance test for R_OSAL_IoGetAxiIdListForDeviceHdl in normal and interrupt context */
void R_OSAL_IoGetAxiIdListForDeviceHdl_TC_21_001_028(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_IoGetAxiIdListForDeviceHdl_TC_21_001_028_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_IoGetAxiIdListForDeviceHdl is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_IoGetNumOfAxiIdsForDeviceName */
e_osal_return_t exec_perf_IoGetNumOfAxiIdsForDeviceName()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    size_t num_of_axi_ids;

    START_MEASURE
    osal_ret = R_OSAL_IoGetNumOfAxiIdsForDeviceName(TEST_IO_DEV, &num_of_axi_ids);
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

/* Execute the performance test for R_OSAL_IoGetNumOfAxiIdsForDeviceName in normal context */
int R_OSAL_IoGetNumOfAxiIdsForDeviceName_TC_21_001_029_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_IoGetNumOfAxiIdsForDeviceName();
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

/* Execute the performance test for R_OSAL_IoGetNumOfAxiIdsForDeviceName in normal and interrupt context */
void R_OSAL_IoGetNumOfAxiIdsForDeviceName_TC_21_001_029(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_IoGetNumOfAxiIdsForDeviceName_TC_21_001_029_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_IoGetNumOfAxiIdsForDeviceName is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_IoGetAxiIdListForDeviceName */
e_osal_return_t exec_perf_IoGetAxiIdListForDeviceName()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    osal_axi_bus_id_t *buffer;
    size_t num_of_written_axi_ids;
    size_t num_of_axi_ids;

    osal_ret = R_OSAL_IoGetNumOfAxiIdsForDeviceName(TEST_IO_DEV, &num_of_axi_ids);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        buffer = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_axi_ids);
        START_MEASURE
        osal_ret = R_OSAL_IoGetAxiIdListForDeviceName(TEST_IO_DEV, buffer, num_of_axi_ids, &num_of_written_axi_ids);
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

        free(buffer);
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_IoGetAxiIdListForDeviceName in normal context */
int R_OSAL_IoGetAxiIdListForDeviceName_TC_21_001_030_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_IoGetAxiIdListForDeviceName();
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

/* Execute the performance test for R_OSAL_IoGetAxiIdListForDeviceName in normal and interrupt context */
void R_OSAL_IoGetAxiIdListForDeviceName_TC_21_001_030(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_IoGetAxiIdListForDeviceName_TC_21_001_030_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_IoGetAxiIdListForDeviceName is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}
