/***********************************************************************************************************************
* Copyright [2021-2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    osal_sample_device_main.c
* Version :      3.6.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Sample application to use OSAL IO/Interrupt/Power API
***********************************************************************************************************************/
#include "osal_sample_device_common.h"

static int32_t io_main_task(void);
#if !defined(UNSUPPORT_INTERRUPT)
static osal_sample_device_callback_ret_t gs_callback_ret;
static int32_t int_main_task(void);
static int32_t setup_interrupt_hw(osal_device_handle_t* p_dev_handles, osal_memory_manager_handle_t* p_memory_handle, osal_memory_buffer_handle_t* p_buffer_handle, bool* p_is_pm_active);
static int32_t create_mq(osal_mq_handle_t* p_mq_handle);
static int32_t interrupt_check(osal_device_handle_t dev_handle, bool *int_channel_flag);
static int32_t start_interrupt(osal_device_handle_t dev_handle, p_osal_isr_func_t irqHandlerFunction, void* arg_callback, bool *interrupt_flag);
static int32_t isr_context_check(void);
static int32_t check_interrupt_proc(osal_mq_handle_t mq_handle);
static int32_t stop_interrupt(osal_device_handle_t dev_handle, uint64_t irq_channel, p_osal_isr_func_t irqHandlerFunction);
static int32_t delete_mq(osal_mq_handle_t mq_handle);
static int32_t teardown_interrupt_hw(const osal_device_handle_t* dev_handles, osal_memory_manager_handle_t memory_handle, osal_memory_buffer_handle_t buffer_handle, bool is_pm_active);
void int_cb_func(osal_device_handle_t dev_handle, uint64_t irq_channel, void * irq_arg);
static int32_t send_message(osal_mq_handle_t mq_handle);
static int32_t setup_buffer(osal_memory_manager_handle_t* p_memory_handle, osal_memory_buffer_handle_t* p_buffer_handle);
#if defined(OSAL_SOC_S4)
static int32_t setup_rtdmac_register(osal_device_handle_t* dev_handles, osal_memory_buffer_handle_t buffer_handle);
#endif
#if defined(OSAL_SOC_V4H) || defined(OSAL_SOC_V3U) || defined(OSAL_SOC_V3H1) || defined(OSAL_SOC_V3H2) || defined(OSAL_SOC_V3M2) || defined(OSAL_SOC_V4M)
static int32_t setup_imr_register(osal_device_handle_t dev_handle, osal_memory_buffer_handle_t buffer_handle);
#endif
#endif
static int32_t io_register_access_task(osal_device_handle_t dev_handle);
static int32_t io_read_write_single_block(osal_device_handle_t dev_handle);
static int32_t io_read_write_multi_blocks(osal_device_handle_t dev_handle);
static int32_t device_open(const char * device_type, size_t device_channel, osal_device_handle_t *device_handle);
static int32_t find_device_id(osal_sample_device_config_t device_config, size_t device_channel, char** device_id);
static int32_t get_device_list(const char * device_type, osal_sample_device_config_t* device_config);
#if defined(SUPPORT_AXI_BUS_NAME)
static int32_t io_axi_name_and_id_management_task(osal_device_handle_t dev_handle);
static int32_t get_bus_name_list(char** axi_name_list, size_t *numOfByte_list);
static int32_t io_axi_bus_name_list_task(char* selected_axi_bus_name);
static int32_t split_axi_bus_name(char* axi_bus_name_list, size_t numOfByte_list, size_t numOfAxiBus, char** splitted_axi_bus_name_list);
static int32_t get_axi_bus_name(osal_axi_bus_id_t axi_id, char** axi_name);
static int32_t get_axi_id_list_from_axi_name(const char* axi_name, osal_axi_bus_id_t** axi_id_list);
static int32_t get_axi_id_list_from_dev_name(const char* device_name, osal_axi_bus_id_t** axi_id_list);
static int32_t get_axi_id_list_from_dev_handle(osal_device_handle_t device_handle, osal_axi_bus_id_t** axi_id_list);
static int32_t open_allocate_memory(osal_memory_manager_handle_t* p_memHandle, osal_memory_buffer_handle_t* p_buffHandle);
static int32_t deallocate_close_memory(osal_memory_manager_handle_t memHandle, osal_memory_buffer_handle_t buffHandle);
static int32_t get_hw_address(const osal_axi_bus_id_t* axi_id_list);
static int32_t io_axi_id_from_axi_name_task(const char* axi_name);
static int32_t io_axi_id_from_dev_name_task(const char* device_name);
static int32_t io_axi_id_from_dev_handle_task(osal_device_handle_t dev_handle);
static int32_t get_hw_address_through_axi_id_task(const char* axi_name, const char* device_name, osal_device_handle_t device_handle);
#endif
#if defined(SUPPORT_POWER)
static int32_t pm_control_active(osal_device_handle_t dev_handle);
static int32_t pm_apply_software_reset(osal_device_handle_t dev_handle);
static int32_t pm_policy_control(osal_device_handle_t dev_handle, e_osal_pm_policy_t powerpolicy);
static int32_t pm_control_deactive(osal_device_handle_t dev_handle);
static int32_t pm_appendix_task(void);
static int32_t get_lowest_intermediate_state(osal_device_handle_t dev_handle, e_osal_pm_state_t *powerState);
#endif
#if defined(SUPPORT_FBA)
static int32_t set_post_clock(void);
static int32_t device_open_fba(const char * hierarchy_name, osal_device_handle_t *device_handle);
static int32_t find_fba_device_id(const char * hierarchy_name, osal_sample_device_config_t device_config, char** fba_device_id);
#endif

extern uint64_t g_osal_main_thread_cpu_affinity;

/**
 * @defgroup osal_sample_device OSAL Application: Sample For OSAL IO, Power Management, Interrupt Operation
 * @{
 */
/**
 * @brief The main function of the OSAL Sample Device application
 * - Output this Device Sample Configuration
 * - Call R_OSAL_GetVersion and output OSAL version
 * - Call R_OSAL_Initialize and call the device_task function. Then call R_OSAL_Deinitialize
 * - The device_task is divided into 4 main tasks as below:
 *      -# @link int_main_task @endlink describes the function of the Interrupt control
 *      -# @link io_main_task @endlink describes the function of the IO control
 *      -# @link pm_appendix_task @endlink describes the function of the Power control
 *      -# @link set_post_clock @endlink describes the function of set POST Clock of Power Manager API
 * 
 * @return 0 on success
 * @return -1 on failure
 */
int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv; /* PRQA S 2956 # No problem because argv is not used */

    /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
    (void)printf("================== osal_sample_device Configuration ===========\n");
    OSAL_SAMPLE_INFO("Sample Target Device is %s\n", SAMPLE_DEVICE_ID);
#if defined(SUPPORT_POWER)
    OSAL_SAMPLE_INFO("%s supports Power Manager API.\n", SAMPLE_DEVICE_ID);
#else
    OSAL_SAMPLE_INFO("%s does not support Power Manager API. pm_main_task and get_lowest_intermediate_state is not executed on %s.\n", SAMPLE_DEVICE_ID, SAMPLE_DEVICE_ID);
#endif
#if defined(SUPPORT_FBA)
    OSAL_SAMPLE_INFO("This SoC supports FIELD BIST Device. Target device is fba_%s\n", FBA_SAMPLE_DEVICE_SUFFIX_NAME);
#else
    OSAL_SAMPLE_INFO("This SoC does not support FIELD BIST Device. set_post_clock is not executed on %s.\n", SAMPLE_DEVICE_ID);
#endif
    /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
    (void)printf("================== osal_sample_device start ==================\n");
    /* local variable */
    e_osal_return_t osal_ret;
    /* PRQA S 5209 1 # This variable is used in the return value of the main function, do not modify it */
    int app_ret;
    bool success = true;
    unsigned int test_pass = 0;
    unsigned int test_fail = 0;
    unsigned int test_skip = 0;
    const st_osal_version_t *version;

    version = R_OSAL_GetVersion();
    printf("OSAL API version %u.%u.%u\n", version->api.major, version->api.minor, version->api.patch );
    printf("OSAL Wrapper version %u.%u.%u\n", version->wrapper.major, version->wrapper.minor, version->wrapper.patch );

    OSAL_SAMPLE_INFO("initialize osal\n");
    osal_ret = R_OSAL_Initialize();
    if(OSAL_RETURN_OK != osal_ret)
    {
        success = false;
        OSAL_SAMPLE_ERR("R_OSAL_Initialize was failed, osal_ret = %d\n", osal_ret);
    }
    else
    {
        if (0x0U != g_osal_main_thread_cpu_affinity)
        {
            /* Change the CPU Affinity of the main thread. */
            ThreadCtl(_NTO_TCTL_RUNMASK, (void *)g_osal_main_thread_cpu_affinity);
        }

#if !defined(UNSUPPORT_INTERRUPT)
        OSAL_SAMPLE_INFO("execute int_main_task\n");
        app_ret = int_main_task();
        if(0 != app_ret)
        {
            success = false;
            OSAL_SAMPLE_ERR("int_main_task operation was failed\n");
        }
        else
        {
            OSAL_SAMPLE_INFO("int_main_task operation is completed\n");
        }
#endif

        OSAL_SAMPLE_INFO("execute io_main_task\n");
        app_ret = io_main_task();
        if(0 != app_ret)
        {
            success = false;
            OSAL_SAMPLE_ERR("io_main_task operation was failed\n");
        }
        else
        {
            OSAL_SAMPLE_INFO("io_main_task operation is completed\n");
        }

#if defined(SUPPORT_POWER)
        OSAL_SAMPLE_INFO("execute pm_appendix_task\n");
        app_ret = pm_appendix_task();
        if(0 != app_ret)
        {
            success = false;
            OSAL_SAMPLE_ERR("pm_appendix_task operation was failed\n");
        }
        else
        {
            OSAL_SAMPLE_INFO("pm_appendix_task operation is completed\n");
        }
#endif

#if defined(SUPPORT_FBA)
        OSAL_SAMPLE_INFO("execute set_post_clock\n");
        app_ret = set_post_clock();
        if(0 != app_ret)
        {
            success = false;
            OSAL_SAMPLE_ERR("set_post_clock operation was failed\n");
        }
        else
        {
            OSAL_SAMPLE_INFO("set_post_clock operation is completed\n");
        }
#endif

        OSAL_SAMPLE_INFO("de-initialize osal\n");
        osal_ret = R_OSAL_Deinitialize();
        if(OSAL_RETURN_OK != osal_ret)
        {
            success = false;
            OSAL_SAMPLE_ERR("R_OSAL_Deinitialize was failed, osal_ret = %d\n", osal_ret);
        }
    }

    if(true == success)
    {
        OSAL_SAMPLE_INFO("osal_sample_device run successfully\n");
        test_pass++;
    }
    else
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("osal_sample_device was failed\n");
        test_fail++;
    }

    /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
    (void)printf("================== osal_sample_device finish ==================\n");
    printf("< OSAL Tests OK = %u, NG = %u, NT = %u>\n", test_pass, test_fail, test_skip);

    return app_ret;
}

/**
 * @defgroup int_main_task OSAL Interrupt APIs Usage
 * @{
 */
/**
 * @brief Interrupt main task:<br>
 * (OSAL API UM reference: Chapter 2.6.1: Register interrupt and Chapter 2.6.3: API restriction for interrupt context)
 * - Open devices and setup hardware register by using @link setup_interrupt_hw @endlink
 * - Create message queue used to inform interrupt process occurred by using @link create_mq @endlink
 * - Check the sample device to know whether it supports interrupt handling or not by using @link interrupt_check @endlink
 * - Register and enable interrupt by using @link start_interrupt @endlink
 * - Check the context of the calling thread by using @link isr_context_check @endlink
 * - Wait for interrupt process occurred and check callback function operation by using @link check_interrupt_proc @endlink
 * - Disable and unregister the interrupt by using @link stop_interrupt @endlink
 * - Delete the message queue by using @link delete_mq @endlink
 * - Close devices and deallocate memory by using @link teardown_interrupt_hw @endlink
 * 
 * @return 0 on success
 * @return -1 on failure
 */
#if !defined(UNSUPPORT_INTERRUPT)
static int32_t int_main_task(void)
{
    int32_t app_ret;
    bool int_channel_flag = false;
    bool interrupt_flag = false;
    bool is_pm_active = false;
    int32_t return_value;
    osal_sample_device_arg_callback_t arg_callback;
    osal_device_handle_t dev_handles[2] = {OSAL_DEVICE_HANDLE_INVALID, OSAL_DEVICE_HANDLE_INVALID};
    osal_memory_manager_handle_t memory_handle = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    osal_memory_buffer_handle_t buffer_handle = OSAL_MEMORY_BUFFER_HANDLE_INVALID;
    osal_mq_handle_t mq_handle = OSAL_MQ_HANDLE_INVALID;
    gs_callback_ret.error_line = 0;
    gs_callback_ret.interrupt_cnt = 0;

    /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
    (void)printf("\n================== int_main_task start ==================\n");

    OSAL_SAMPLE_INFO("Open device and setup hardware register\n");
    app_ret = setup_interrupt_hw(dev_handles, &memory_handle, &buffer_handle, &is_pm_active);
    if(0 != app_ret)
    {
        OSAL_SAMPLE_ERR("Setup interrupt hardware was failed\n");
    }

    if(0 == app_ret){
        OSAL_SAMPLE_INFO("Create message queue used to inform interrupt process occurred\n");
        app_ret = create_mq(&mq_handle);
        if(0 != app_ret)
        {
            OSAL_SAMPLE_ERR("Creating message queue was failed\n");
        }
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Start checking device has an interrupt handler or not\n");
        app_ret = interrupt_check(dev_handles[0], &int_channel_flag);
        if(0 != app_ret)
        {
            OSAL_SAMPLE_ERR("Checking the number of handlers in the device failed\n");
        }
    }

    if(true == int_channel_flag)
    {
        if(0 == app_ret)
        {
            arg_callback.mq_handle = mq_handle;

            OSAL_SAMPLE_INFO("Start to register a callback handler and enable for interrupt channel 0\n");
            /* PRQA S 0314 1 # The parameter must be (void *) and there is no side side-effect in this case. */
            app_ret = start_interrupt(dev_handles[0], int_cb_func , (void*)&arg_callback, &interrupt_flag);
            if(0 != app_ret)
            {
                OSAL_SAMPLE_ERR("Register a callback handler and enable for interrupt channel 0 was failed\n");
            }
        }
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Start checking the current context. The expected context is not a context from interrupt.\n");
        app_ret = isr_context_check();
        if(0 != app_ret)
        {
            OSAL_SAMPLE_ERR("Check the current context failed\n");
        }
    }

    if(true == interrupt_flag)
    {
        if(0 == app_ret)
        {
            OSAL_SAMPLE_INFO("Wait for interrupt process occurred and check callback function operation\n");
            app_ret = check_interrupt_proc(mq_handle);
            if(0 != app_ret)
            {
                OSAL_SAMPLE_ERR("Checking callback function operation was failed\n");
            }
        }

        OSAL_SAMPLE_INFO("Start disabling interrupt and unregister the callback handler\n");
        return_value = stop_interrupt(dev_handles[0], IRQ_CHANNEL, int_cb_func);
        if(0 != return_value)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Disable interrupt and unregister callback handler failed\n");
        }
    }

    OSAL_SAMPLE_INFO("Delete the message queue\n");
    return_value = delete_mq(mq_handle);
    if(0 != return_value)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Deleting the message queue was failed\n");
    }

    OSAL_SAMPLE_INFO("Close devices and deallocate memory\n");
    return_value = teardown_interrupt_hw(dev_handles, memory_handle, buffer_handle, is_pm_active);
    if(0 != return_value)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Closing devices and deallocating memory was failed\n");
    }

    /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
    (void)printf("================== int_main_task finish ==================\n\n");
    return app_ret;
    /* PRQA S 9104, 9107 2 # Sample Application aims to show how to use the OSAL API and the order in which it is called. Splitting functions reduces readability.
    There are no significant side effects to the behavior of the sample app code without modification. */
}

/**
 * @brief This function is to setup interrupt hardware
 * - Open sample device by using @link device_open @endlink
 * - Open sub device by using @link device_open @endlink
 * - If SUPPORT_POWER is enabled, set power required state to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * - Allocate and setup buffer by using @link setup_buffer @endlink
 * - If OSAL_SOC_S4 is defined, setup hardware register of RT-DMAC by using @link setup_rtdmac_register @endlink
 * - If OSAL_SOC_V4h or OSAL_SOC_V3U or OSAL_SOC_V3H1 or OSAL_SOC_V3H2 or OSAL_SOC_V3M2 defined, setup hardware register of IMR by using @link setup_imr_register @endlink
 *
 * @param[in, out] p_dev_handles the handles of devices
 * @param[in, out] p_memory_handle the handle of memory manager
 * @param[in, out] p_buffer_handle the handle of buffer
 * @param[in, out] p_is_pm_active the information about whether device pm is active
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t setup_interrupt_hw(osal_device_handle_t* p_dev_handles,
                            osal_memory_manager_handle_t* p_memory_handle,
                            osal_memory_buffer_handle_t* p_buffer_handle,
                            bool* p_is_pm_active)
{
#if defined(SUPPORT_POWER)
    e_osal_return_t osal_ret;
#endif
    int32_t app_ret;
    osal_device_handle_t dev_handles[2] = {OSAL_DEVICE_HANDLE_INVALID, OSAL_DEVICE_HANDLE_INVALID};
    osal_memory_manager_handle_t memory_handle = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    osal_memory_buffer_handle_t buffer_handle = OSAL_MEMORY_BUFFER_HANDLE_INVALID;
    bool local_is_pm_active = false;

    /* open sample device */
    app_ret = device_open(SAMPLE_DEVICE_TYPE, 0, &dev_handles[0]);
    if(0 != app_ret)
    {
        OSAL_SAMPLE_ERR("%s opening was failed \n", SAMPLE_DEVICE_ID);
    }

    if(0 != strcmp(SAMPLE_INT_SUB_DEVICE_TYPE, ""))
    {
        /* open sub device */
        app_ret = device_open(SAMPLE_INT_SUB_DEVICE_TYPE, 0, &dev_handles[1]);
        if(0 != app_ret)
        {
            OSAL_SAMPLE_ERR("%s opening was failed \n", SAMPLE_INT_SUB_DEVICE_ID);
        }
        (void)R_OSAL_PmSetRequiredState(dev_handles[1], OSAL_PM_REQUIRED_STATE_REQUIRED, true);
    }

#if defined(SUPPORT_POWER)
    if (0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Set power required state to OSAL_PM_REQUIRED_STATE_REQUIRED\n");
        osal_ret = R_OSAL_PmSetRequiredState(dev_handles[0], OSAL_PM_REQUIRED_STATE_REQUIRED, true);
        /* SAMPLE_DEVICE_ID and SAMPLE_INT_SUB_DEVICE_ID of S4 have same clock/power/reset domain */
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Required state setting was failed, osal_ret= %d\n", osal_ret);
        }
        else
        {
            local_is_pm_active = true;
        }
    }
#endif

    if(0 == app_ret)
    {
        /* setup buffer */
        app_ret = setup_buffer(&memory_handle, &buffer_handle);
    }

    if(0 == app_ret)
    {
#if defined(OSAL_SOC_S4)
        /* setup hardware register of RT-DMAC*/
        app_ret = setup_rtdmac_register(dev_handles, buffer_handle);
#elif defined(OSAL_SOC_V4H) || defined(OSAL_SOC_V3U) || defined(OSAL_SOC_V3H1) || defined(OSAL_SOC_V3H2) || defined(OSAL_SOC_V3M2) || defined(OSAL_SOC_V4M)
        /* setup hardware register of IMR*/
        app_ret = setup_imr_register(dev_handles[0], buffer_handle);
#endif
    }

    p_dev_handles[0] = dev_handles[0];
    p_dev_handles[1] = dev_handles[1];
    *p_memory_handle = memory_handle;
    *p_buffer_handle = buffer_handle;
    *p_is_pm_active = local_is_pm_active;

    return app_ret;
}

/**
 * @brief This function is to create message queue.
 * - Get initial message queue config by using R_OSAL_MqInitializeMqConfigSt
 * - Create message queue by using R_OSAL_MqCreate
 *
 * @param[in, out] p_mq_handle the handle of message queue
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t create_mq(osal_mq_handle_t* p_mq_handle)
{

    /* local variable */
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;
    st_osal_mq_config_t mq_config;
    osal_mq_handle_t mq_handle = OSAL_MQ_HANDLE_INVALID;

    OSAL_SAMPLE_INFO("Get initial message queue config\n");
    osal_ret = R_OSAL_MqInitializeMqConfigSt(&mq_config);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Getting initial config was failed, osal_ret= %d\n", osal_ret);
    }
    else
    {
        mq_config.max_num_msg = MAX_NUM_OF_MSG;
        mq_config.msg_size = MSG_SIZE;
        OSAL_SAMPLE_INFO("Create a message queue with max_num_msg:2, msg_size:sizeof(mq_msg_t)\n");
        osal_ret = R_OSAL_MqCreate(&mq_config, (osal_mq_id_t)MQ_ID_NO1, &mq_handle);
        if (OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Message queue creation was failed, osal_ret= %d\n", osal_ret);
        }
    }

    *p_mq_handle = mq_handle;

    return app_ret;
}

/**
 * @brief This function is to check to know whether the sample device supports interrupt handling or not
 * - Get the number of interrupt channels of the sample device by using R_OSAL_InterruptGetNumOfIrqChannels
 * - If the sample device doesn't have any interrupt channel, it means that that sample device doesn't support interrupt handling.
 * 
 * @param[in] dev_handle OSAL device handle of device which is going to be checked
 * @param[in, out] int_channel_flag The address of a boolean flag. The interrupt supporting result will be stored in this parameter:
 * - true: device has interrupt handling
 * - false: device doesn't have interrupt handling
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t interrupt_check(osal_device_handle_t dev_handle, bool *int_channel_flag)
{
    int32_t app_ret = 0;
    e_osal_return_t osal_ret;
    size_t numOfChannel = 0;

    OSAL_SAMPLE_INFO("Get the number of interrupt channels\n");
    osal_ret = R_OSAL_InterruptGetNumOfIrqChannels(dev_handle, &numOfChannel);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Get number of interrupt channels failed\n");
    }
    else if(0U == numOfChannel)
    {
        OSAL_SAMPLE_INFO("Device does not have interrupt handler\n");
    }
    else
    {
        OSAL_SAMPLE_INFO("Device has interrupt handler\n");
        *int_channel_flag = true;
    }

    return app_ret;
}

/**
 * @brief This function is to register and enable an interrupt handler for a specific interrupt channel.
 * - Register an interrupt handler for interrupt channel IRQ_CHANNEL by using R_OSAL_InterruptRegisterIsr
 * - Change the interrupt priority by using R_OSAL_InterruptSetIsrPriority
 * - Enable the registered interrupt handler by using R_OSAL_InterruptEnableIsr
 * 
 * @param[in] dev_handle OSAL device handle of device which is used for setting an interrupt
 * @param[in] irqHandlerFunction Interrupt handler function. This function is used by user for handling the interrupt occurs from device
 * @param[in] arg_callback The argument of callback function
 * @param[in, out] interrupt_flag The address of a boolean flag used for determining whether the interrupt setting is completed or not:
 * - true: interrupt registering and enabling is successful
 * - false: interrupt registering or enabling is failed
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t start_interrupt(osal_device_handle_t dev_handle, p_osal_isr_func_t irqHandlerFunction, void* arg_callback, bool *interrupt_flag)
{
    int32_t app_ret = 0;
    e_osal_return_t osal_ret;

    OSAL_SAMPLE_INFO("Register a callback handler with interrupt channel (%d)\n", IRQ_CHANNEL);
    osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, IRQ_CHANNEL, OSAL_INTERRUPT_PRIORITY_TYPE10, irqHandlerFunction , arg_callback);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Function callback register failed, osal_ret = %d\n", osal_ret);
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Change the priority from OSAL_INTERRUPT_PRIORITY_TYPE10 to OSAL_INTERRUPT_PRIORITY_TYPE11\n");
        osal_ret = R_OSAL_InterruptSetIsrPriority(dev_handle, IRQ_CHANNEL, OSAL_INTERRUPT_PRIORITY_TYPE11);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Interrupt priority change failed, osal_ret = %d\n", osal_ret);
        }
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Enable registered interrupt\n");
        osal_ret = R_OSAL_InterruptEnableIsr(dev_handle, IRQ_CHANNEL);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Interrupt enable failed, osal_ret = %d\n", osal_ret);
        }
    }

    if(0 == app_ret)
    {
        *interrupt_flag = true;
    }

    return app_ret;
}

/**
 * @brief This function is to check the current context of the calling thread
 * - Get the current context by calling to R_OSAL_InterruptIsISRContext
 * - The expected context is not context from interrupt
 * 
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t isr_context_check(void)
{
    int32_t app_ret = 0;
    e_osal_return_t osal_ret;
    bool isISR = true;

    OSAL_SAMPLE_INFO("Check the current context\n");
    osal_ret = R_OSAL_InterruptIsISRContext(&isISR);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Interrupt context checking failed, osal_ret = %d\n", osal_ret);
    }

    if(0 == app_ret)
    {
        if(true == isISR)
        {
            OSAL_SAMPLE_INFO("The calling thread is under interrupt context\n");
        }
        else
        {
            OSAL_SAMPLE_INFO("The calling thread is under process context\n");
        }
    }

    return app_ret;
}

/**
 * @brief This function is to wait interrupt process occurred and check callback function operation.
 * - Receive message from queue by using R_OSAL_MqReceiveForTimePeriod
 * The expected value of "gs_callback_ret.interrupt_cnt" is 1
 * The expected value of "gs_callback_ret.osal_ret" is OSAL_RETURN_OK
 *
 * @param[in] mq_handle the handle of message queue
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t check_interrupt_proc(osal_mq_handle_t mq_handle)
{
    /* local variable */
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;
    mq_msg_t receive_buffer;

    OSAL_SAMPLE_INFO("Receive message from queue\n");
    /* PRQA S 0314 1 # p_buffer need to be a void pointer type so that there is no side-effect in this case. */
    osal_ret = R_OSAL_MqReceiveForTimePeriod(mq_handle, TIMEOUT, (void *)&receive_buffer, sizeof(receive_buffer));
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("receiving message was failed, osal_ret = %d\n", osal_ret);
    }

    if (1U < gs_callback_ret.interrupt_cnt)
    {
        OSAL_SAMPLE_INFO("Interrupt occurred more than once\n");
    }

    if(OSAL_RETURN_OK == gs_callback_ret.osal_ret)
    {
        OSAL_SAMPLE_INFO("Callback function operation was successed\n");
    }
    else
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Callback function operation was failed, osal_et = %d, int_cb_func(%d)\n", gs_callback_ret.osal_ret, gs_callback_ret.error_line);
    }
    gs_callback_ret.interrupt_cnt = 0;

    return app_ret;
}

/**
 * @brief This function is to disable and unregister an interrupt handler for a specific interrupt channel.
 * - Disable an interrupt handler by using R_OSAL_InterruptDisableIsr
 * - Unregister the registered interrupt handler by using R_OSAL_InterruptUnregisterIsr
 * 
 * @param[in] dev_handle OSAL device handle of device which interrupt handling is going to be stopped
 * @param[in] irq_channel Irq channel number which has already been selected during the registration step
 * @param[in] irqHandlerFunction Interrupt handler function which has already been chosen during the registration step
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t stop_interrupt(osal_device_handle_t dev_handle, uint64_t irq_channel, p_osal_isr_func_t irqHandlerFunction)
{
    int32_t app_ret = 0;
    e_osal_return_t osal_ret;

    OSAL_SAMPLE_INFO("Disable enabled interrupt\n");
    osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, irq_channel);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Interrupt disable failed, osal_ret = %d\n", osal_ret);
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Unregister registered interrupt\n");
        osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, irq_channel, irqHandlerFunction);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Interrupt unregister failed, osal_ret = %d\n", osal_ret);
        }
    }

    return app_ret;
}

/**
 * @brief This function is to delete message queue.
 * - Delete message queue by using R_OSAL_MqDelete
 *
 * @param[in] mq_handle the handle of message queue
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t delete_mq(osal_mq_handle_t mq_handle)
{
    int32_t app_ret = 0;
    e_osal_return_t osal_ret;

    if(OSAL_MQ_HANDLE_INVALID != mq_handle)
    {
        OSAL_SAMPLE_INFO("Delete message queue\n");
        osal_ret = R_OSAL_MqDelete(mq_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Can not delete message queue, osal_ret = %d\n", osal_ret);
        }
    }

    return app_ret;
}

/**
 * @brief This function is to close devices and deallocate memory.
 * - If SUPPORT_POWER is enabled, set power required state to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * - Deallocate memory buffer of the second buffer_handle by using R_OSAL_MmngrDealloc
 * - Deallocate memory buffer of the buffer_handle by using R_OSAL_MmngrDealloc
 * - Delete memory manager handle by using R_OSAL_MmngrClose
 * - Close sample device by using R_OSAL_IoDeviceClose
 * - Close sub device by using R_OSAL_IoDeviceClose.
 *
 * @param[in] dev_handles the handles of devices
 * @param[in] memory_handle the handle of memory manager
 * @param[in] buffer_handle the handle of buffer
 * @param[in] is_pm_active the information about whether device pm is active
 * @return 0 in success
 * @return -1 on failure
 */
static int32_t teardown_interrupt_hw(   const osal_device_handle_t* dev_handles,
                                        osal_memory_manager_handle_t memory_handle,
                                        osal_memory_buffer_handle_t buffer_handle,
                                        bool is_pm_active)
{
    int32_t app_ret = 0;
    e_osal_return_t osal_ret;

    if(true == is_pm_active)
    {
#if defined(SUPPORT_POWER)
        OSAL_SAMPLE_INFO("Set power required state to OSAL_PM_REQUIRED_STATE_RELEASED\n");
        osal_ret = R_OSAL_PmSetRequiredState(dev_handles[0], OSAL_PM_REQUIRED_STATE_RELEASED, true);
        /* SAMPLE_DEVICE_ID and SAMPLE_INT_SUB_DEVICE_ID of S4 have same clock/power/reset domain */
        if (OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Required state setting was failed, osal_ret = %d\n", osal_ret);
        }
#endif
    }

    if(OSAL_MEMORY_BUFFER_HANDLE_INVALID != buffer_handle)
    {
        OSAL_SAMPLE_INFO("Deallocate memory buffer\n");
        osal_ret = R_OSAL_MmngrDealloc(memory_handle, buffer_handle);
        if (OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Can not deallocate memory buffer, osal_ret = %d\n", osal_ret);
        }
    }

    if(OSAL_MEMORY_MANAGER_HANDLE_INVALID != memory_handle)
    {
        OSAL_SAMPLE_INFO("Delete memory manager handle\n")
        osal_ret = R_OSAL_MmngrClose(memory_handle);
        if (OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Can not delete memory manager handle, osal_ret = %d\n", osal_ret);
        }
    }

    if(OSAL_DEVICE_HANDLE_INVALID != dev_handles[0])
    {
        OSAL_SAMPLE_INFO("Close device\n");
        osal_ret = R_OSAL_IoDeviceClose(dev_handles[0]);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Can not close %s, osal_ret = %d\n",SAMPLE_DEVICE_ID, osal_ret);
        }
    }

    if(OSAL_DEVICE_HANDLE_INVALID != dev_handles[1])
    {
        OSAL_SAMPLE_INFO("Close device\n");
        osal_ret = R_OSAL_IoDeviceClose(dev_handles[1]);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Can not close %s, osal_ret = %d\n",SAMPLE_INT_SUB_DEVICE_TYPE, osal_ret);
        }
    }

    return app_ret;
    /* PRQA S 9104 2 # Sample Application aims to show how to use the OSAL API and the order in which it is called. Splitting functions reduces readability.
   There are no significant side effects to the behavior of the sample app code without modification. */
}

/**
 * @brief Interrupt callback function
 * - Set device status clear register by using R_OSAL_IoWrite32
 * - Disable enabled interrupt by using R_OSAL_InterruptDisableIsr
 * - Send message by using @link send_message @endlink
 * 
 * @param[in] dev_handle the handle of device
 * @param[in] irq_channel the interrupt channel
 * @param[in] irq_arg the callback function argument which is a void pointer type
 */
/* PRQA S 3673,1505 1 # Implemented in accordance with the definition of p_osal_isr_func_t. */
void int_cb_func(osal_device_handle_t dev_handle, uint64_t irq_channel, void * irq_arg)
{
    int32_t app_ret;
    e_osal_return_t osal_ret;
    /* PRQA S 0316 1 # Required cast on system, not modifiable. */
    const osal_sample_device_arg_callback_t *local_arg = (const osal_sample_device_arg_callback_t *)irq_arg;
    gs_callback_ret.interrupt_cnt ++;

    /* Set device status clear register */
    osal_ret = R_OSAL_IoWrite32(dev_handle, SAMPLE_CLEAR_REGISTER, SAMPLE_CLEAR_REGISTER_VALUE);
    if(OSAL_RETURN_OK != osal_ret)
    {
        /* Device status clear register set was failed */
        gs_callback_ret.error_line = __LINE__;
        gs_callback_ret.osal_ret = osal_ret;
    }

    /* Disable enabled interrupt */
    osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, irq_channel);
    if(OSAL_RETURN_OK != osal_ret)
    {
        /* Interrupt disable failed, osal_ret */
        if (OSAL_RETURN_OK == gs_callback_ret.osal_ret)
        {
            gs_callback_ret.error_line = __LINE__;
            gs_callback_ret.osal_ret = osal_ret;
        }
    }

    /* Send Message */
    app_ret = send_message(local_arg->mq_handle);
    if(0 != app_ret)
    {
        /* Sending message was failed */
    }

}

/**
 * @brief Send Message buffer<br>
 * If this function called from interrupt context, error is stored in gs_callback_ret and do not output.
 * - Check the current context by using R_OSAL_InterruptIsISRContext
 * - If the current context is context from interrupt, check whether the message queue is full or not by using R_OSAL_MqIsFull
 * - Send message to queue by using R_OSAL_MqSendForTimePeriod
 *
 * @param[in, out] mq_handle the handle of message queue
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t send_message(osal_mq_handle_t mq_handle)
{
    int32_t app_ret = 0;
    e_osal_return_t osal_ret;
    mq_msg_t send_buffer = MSG_VALUE;
    bool isISR = true;
    bool isFull = true;

    /*Check the current context */
    osal_ret = R_OSAL_InterruptIsISRContext(&isISR);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        /* Interrupt context check was failed */
        if (OSAL_RETURN_OK == gs_callback_ret.osal_ret)
        {
            gs_callback_ret.error_line = __LINE__;
            gs_callback_ret.osal_ret = osal_ret;
        }
    }

    if (true == isISR)
    {
        /* Check whether the message queue is full or not */
        osal_ret = R_OSAL_MqIsFull(mq_handle, &isFull);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            /* Checking message queue was failed */
            if (OSAL_RETURN_OK == gs_callback_ret.osal_ret)
            {
                gs_callback_ret.error_line = __LINE__;
                gs_callback_ret.osal_ret = osal_ret;
            }
        }

        if(false == isFull)
        {
            /* Send message to queue */
            /* If the current context is context from interrupt, this function need without timeout (time_period = 0ms) */
            /* PRQA S 0314 1 # p_buffer need to be a void pointer type so that there is no side-effect in this case. */
            osal_ret = R_OSAL_MqSendForTimePeriod(mq_handle, 0, (void*)&send_buffer, sizeof(send_buffer));
            if(OSAL_RETURN_OK != osal_ret)
            {
                app_ret = -1;
                /* Sending message was failed */
                if (OSAL_RETURN_OK == gs_callback_ret.osal_ret)
                {
                    gs_callback_ret.error_line = __LINE__;
                    gs_callback_ret.osal_ret = osal_ret;
                }
            }
        }
        else
        {
            /*  If the current context is context from interrupt and queue is full,
                R_OSAL_MqSendForTimePeriod occur error (OSAL_RETURN_TIME) */
        }
    }
    else
    {
        /* Send message to queue */
        /* PRQA S 0314 1 # p_buffer need to be a void pointer type so that there is no side-effect in this case. */
        osal_ret = R_OSAL_MqSendForTimePeriod(mq_handle, TIMEOUT, (void*)&send_buffer, sizeof(send_buffer));
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Sending message was failed, osal_ret = %d\n", osal_ret);
        }
    }

    return app_ret;
}

/**
 * @brief This function is to set data to the buffer object.
 * - Get the maximum memory configuration by using R_OSAL_MmngrGetOsalMaxConfig
 * - Open new memory manager by using R_OSAL_MmngrOpen
 * - Allocate buffer by using R_OSAL_MmngrAlloc
 * - Get CPU pointer of the buffer by using R_OSAL_MmngrGetCpuPtr
 * - Write data to the buffer by using memcpy
 * - Get size of the buffer by using R_OSAL_MmngrGetSize
 * - Synchronizes CPU memory of the buffer to hardware memory by using R_OSAL_MmngrFlush
 *
 * @param[in, out] p_memory_handle the handle of memory manager
 * @param[in, out] p_buffer_handle the handle of buffer
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t setup_buffer(osal_memory_manager_handle_t* p_memory_handle, osal_memory_buffer_handle_t* p_buffer_handle)
{
    int32_t app_ret = 0;
    e_osal_return_t osal_ret;
    void* cpu_ptr = NULL;
    /* PRQA S 3678 1 # Do not use const to cast to non-constant. */
    uint32_t* int_cpu_ptr;
    size_t size = 0;
    st_osal_mmngr_config_t memory_max_config;
    uint32_t dummy_data1 = SAMPLE_DUMMY_BUFFER_DATA_1;
    uint32_t dummy_data2 = SAMPLE_DUMMY_BUFFER_DATA_2;
    osal_memory_manager_handle_t memory_handle = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    osal_memory_buffer_handle_t buffer_handle = OSAL_MEMORY_BUFFER_HANDLE_INVALID;

    OSAL_SAMPLE_INFO("Get the maximum memory configuration\n");
    osal_ret = R_OSAL_MmngrGetOsalMaxConfig(&memory_max_config);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Can not get the memory max configuration, osal_ret = %d\n", osal_ret);
    } else {
        OSAL_SAMPLE_INFO("Open new memory manager\n");
        osal_ret = R_OSAL_MmngrOpen(&memory_max_config, &memory_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            memory_handle = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
            OSAL_SAMPLE_ERR("Open memory handle failed , osal_ret = %d\n", osal_ret);
        } else {
            OSAL_SAMPLE_INFO("Allocate buffer\n");
            osal_ret = R_OSAL_MmngrAlloc(memory_handle, SAMPLE_BUFFER_SIZE, SAMPLE_BUFFER_ALIGNMENT, &buffer_handle);
            if(OSAL_RETURN_OK != osal_ret)
            {
                app_ret = -1;
                buffer_handle = OSAL_MEMORY_BUFFER_HANDLE_INVALID;
                OSAL_SAMPLE_ERR("Memory allocation failed, osal_ret = %d\n", osal_ret);
            }
        }
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Get CPU pointer of the buffer\n");
        osal_ret = R_OSAL_MmngrGetCpuPtr(buffer_handle, &cpu_ptr);
        if (OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Getting CPU pointer was failed, osal_ret = %d\n", osal_ret);
        }
    }
    if(0 == app_ret)
    {
        /* PRQA S 0316 1 # There is no problem because it is only use to get other address of buffer. */
        int_cpu_ptr = (uint32_t*)cpu_ptr;

        /* Write data to the beginning of the buffer */
        /* PRQA S 1496 2 # Caller of this function should guarantee size of buffer */
        /* PRQA S 0314, 0315 1 # The 1st parameter must be (void *) and there is no side-effect in this case. */
        (void)memcpy((void *)&int_cpu_ptr[SAMPLE_BUFFER_ADDR_INDEX_1], &dummy_data1, sizeof(dummy_data1));

        /* Write data to the beginning of the buffer */
        /* PRQA S 1496 2 # Caller of this function should guarantee size of buffer */
        /* PRQA S 0314, 0315 1 # The 1st parameter must be (void *) and there is no side-effect in this case. */
        (void)memcpy((void *)&int_cpu_ptr[SAMPLE_BUFFER_ADDR_INDEX_2], &dummy_data2, sizeof(dummy_data2));

        OSAL_SAMPLE_INFO("Get size of the buffer\n");
        osal_ret = R_OSAL_MmngrGetSize(buffer_handle, &size);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Getting size was failed, osal_ret = %d\n", osal_ret);
        }
    }
    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Synchronizes CPU memory of the buffer to hardware memory\n");
        osal_ret = R_OSAL_MmngrFlush(buffer_handle, 0, size);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Synchronizing CPU memory to hardware memory was failed, osal_ret = %d\n", osal_ret);
        }
    }

    *p_memory_handle = memory_handle;
    *p_buffer_handle = buffer_handle;

    return app_ret;
}

/**
 * @brief This function is to setup the RT-DMAC register.
 * - Get axi bus id of sample device by using @link get_axi_id_list_from_dev_name @endlink
 * - Get hardware address of the buffer of the buffer_handle by using R_OSAL_MmngrGetHwAddr
 * - Set DMA operating register by using R_OSAL_IoWrite16
 * - Set hardware address to DMA source address register by using R_OSAL_IoWrite32
 * - Set hardware address to DMA destination address register by using R_OSAL_IoWrite32
 * - Set DMA transfer count register by using R_OSAL_IoWrite32
 * - Set DMA channel B control register by using R_OSAL_IoWrite32
 * - Set DMA channel control register by using R_OSAL_IoWrite32
 *
 * @param[in] dev_handles the handles of devices
 * @param[in] buffer_handle the handle of buffer
 * @return 0 on success
 * @return -1 on failure
 */
#if defined(OSAL_SOC_S4)
static int32_t setup_rtdmac_register(   osal_device_handle_t* dev_handles,
                                        osal_memory_buffer_handle_t buffer_handle)
{
    int32_t app_ret;
    e_osal_return_t osal_ret;
    uintptr_t local_hw_addr = 0;
    uint32_t* local_hw_int_addr;
    uintptr_t local_src_addr;
    uintptr_t local_dest_addr;
    osal_axi_bus_id_t* axi_id_list_from_device_name = NULL;

    OSAL_SAMPLE_INFO("Get axi bus id of sample device\n");
    app_ret = get_axi_id_list_from_dev_name(SAMPLE_DEVICE_ID, &axi_id_list_from_device_name);
    if(0 != app_ret)
    {
        OSAL_SAMPLE_ERR("Getting axi bus id was failed");
    }
    else
    {
        if(NULL == axi_id_list_from_device_name)
        {
            app_ret = -1;
            OSAL_SAMPLE_INFO("Device name %s doesn't have AXI IDs\n", SAMPLE_DEVICE_ID);
        }
        else
        {
            OSAL_SAMPLE_INFO("Get hardware address of the buffer of the buffer_handle\n");
            osal_ret = R_OSAL_MmngrGetHwAddr(buffer_handle, axi_id_list_from_device_name[0], &local_hw_addr);
            if(OSAL_RETURN_OK != osal_ret)
            {
                app_ret = -1;
                OSAL_SAMPLE_ERR("Getting hardware address was failed, osal_ret = %d\n", osal_ret);
            } else
            {
                local_hw_int_addr = (uint32_t *) local_hw_addr;
                local_src_addr = (uintptr_t)&local_hw_int_addr[SAMPLE_BUFFER_ADDR_INDEX_1];
                local_dest_addr = (uintptr_t)&local_hw_int_addr[SAMPLE_BUFFER_ADDR_INDEX_2];
            }
        }
    }
    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Set DMA operating register\n");
        osal_ret = R_OSAL_IoWrite16(dev_handles[1], SAMPLE_REG_RDMOR, SAMPLE_TRANSFER_RDMOR_VALUE);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("DMA operating register set was failed, osal_ret = %d\n", osal_ret);
        }
    }
    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Set hardware address to DMA source address register\n");
        osal_ret = R_OSAL_IoWrite32(dev_handles[0], SAMPLE_REG_RDMSAR, (uint32_t)local_src_addr);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Setting hardware address was failed, osal_ret = %d\n", osal_ret);
        }
    }
    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Set hardware address to DMA destination address register\n");
        osal_ret = R_OSAL_IoWrite32(dev_handles[0], SAMPLE_REG_RDMDAR, (uint32_t)local_dest_addr);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Setting hardware address was failed, osal_ret = %d\n", osal_ret);
        }
    }
    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Set DMA transfer count register\n");
        osal_ret = R_OSAL_IoWrite32(dev_handles[0], SAMPLE_REG_RDMTCR, SAMPLE_TRANSFER_COUNT_VALUE);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("DMA transfer count register set was failed, osal_ret = %d\n", osal_ret);
        }
    }
    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Set DMA channel B control register\n");
        osal_ret = R_OSAL_IoWrite32(dev_handles[0], SAMPLE_REG_RDMCHCRB, SAMPLE_TRANSFER_STEP_VALUE);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("DMA channel B control register set was failed, osal_ret = %d\n", osal_ret);
        }
    }
    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Set DMA channel control register\n");
        osal_ret = R_OSAL_IoWrite32(dev_handles[0], SAMPLE_REG_RDMCHCR, SAMPLE_TRANSFER_RDMCHCR_VALUE);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("DMA channel control register set was failed, osal_ret = %d\n", osal_ret);
        }
    }
    if(NULL != axi_id_list_from_device_name)
    {
        /* PRQA S 5118 2 # Allocated in the sub-function called by this function */
        /* PRQA S 0314 1 # The parameter must be (void *) and there is no side-effect in this case. */
        free((void*)axi_id_list_from_device_name);
    }
    return app_ret;
    /* PRQA S 9104 2 # Sample Application aims to show how to use the OSAL API and the order in which it is called. Splitting functions reduces readability.
   There are no significant side effects to the behavior of the sample app code without modification. */
}
#endif

/**
 * @brief This function is to setup the IMR register.
 * - Get axi bus id of sample device by using @link get_axi_id_list_from_dev_name @endlink
 * - Get hardware address of the buffer by using R_OSAL_MmngrGetHwAddr
 * - Set hardware address to display list start register by using R_OSAL_IoWrite32
 * - Clear device interrupt mask register by using R_OSAL_IoWrite32
 * - Set device control register by using R_OSAL_IoWrite32
 *
 * @param[in] dev_handle the handle of device
 * @param[in] buffer_handle the handle of buffer
 * @return 0 on success
 * @return -1 on failure
 */
#if defined(OSAL_SOC_V4H) || defined(OSAL_SOC_V3U) || defined(OSAL_SOC_V3H1) || defined(OSAL_SOC_V3H2) || defined(OSAL_SOC_V3M2) || defined(OSAL_SOC_V4M)
static int32_t setup_imr_register(osal_device_handle_t dev_handle, osal_memory_buffer_handle_t buffer_handle)
{
    int32_t app_ret;
    e_osal_return_t osal_ret;
    uintptr_t local_hw_addr = 0;
    osal_axi_bus_id_t* axi_id_list_from_device_name = NULL;

    OSAL_SAMPLE_INFO("Get axi bus id of sample device\n");
    app_ret = get_axi_id_list_from_dev_name(SAMPLE_DEVICE_ID, &axi_id_list_from_device_name);
    if(0 != app_ret)
    {
        OSAL_SAMPLE_ERR("Getting axi bus id was failed");
    }
    else
    {
        if(NULL == axi_id_list_from_device_name)
        {
            app_ret = -1;
            OSAL_SAMPLE_INFO("Device name %s doesn't have AXI IDs\n", SAMPLE_DEVICE_ID);
        }
        else
        {
            OSAL_SAMPLE_INFO("Get hardware address of the buffer\n");
            osal_ret = R_OSAL_MmngrGetHwAddr(buffer_handle, axi_id_list_from_device_name[0], &local_hw_addr);
            if(OSAL_RETURN_OK != osal_ret)
            {
                app_ret = -1;
                OSAL_SAMPLE_ERR("Getting hardware address was failed, osal_ret = %d\n", osal_ret);
            }
        }
    }
    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Set hardware address to display list start register\n");
        osal_ret = R_OSAL_IoWrite32(dev_handle, SAMPLE_IMR_DLSAR, (uint32_t)local_hw_addr);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Setting hardware address was failed, osal_ret = %d\n", osal_ret);
        }
    }
    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Clear device interrupt mask register\n");
        osal_ret = R_OSAL_IoWrite32(dev_handle, SAMPLE_IMR_IMR, SAMPLE_IMR_IMR_VALUE);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Device interrupt mask register clear was failed, osal_ret = %d\n", osal_ret);
        }
    }
    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Set device control register\n");
        osal_ret = R_OSAL_IoWrite32(dev_handle, SAMPLE_IMR_CR, SAMPLE_IMR_CR_VALUE);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Device control register set was failed, osal_ret = %d\n", osal_ret);
        }
    }
    if(NULL != axi_id_list_from_device_name)
    {
        /* PRQA S 5118 2 # Allocated in the sub-function called by this function */
        /* PRQA S 0314 1 # The parameter must be (void *) and there is no side-effect in this case. */
        free((void*)axi_id_list_from_device_name);
    }
    return app_ret;
    /* PRQA S 9104 2 # Sample Application aims to show how to use the OSAL API and the order in which it is called. Splitting functions reduces readability.
   There are no significant side effects to the behavior of the sample app code without modification. */
}
#endif  /* end of OSAL_SOC_V4H || OSAL_SOC_V3U || OSAL_SOC_V3H1 || OSAL_SOC_V3H2 || OSAL_SOC_V3M2 || OSAL_SOC_V4M  */
#endif  /* end of !UNSUPPORT_INTERRUPT */
/**@} int_main_task*/

/**
 * @defgroup io_main_task OSAL IO APIs Usage
 * @{
 */
/**
 * @brief IO main task:<br>
 * (OSAL API UM reference: Chapter 2.7.1: Device open, Chapter 2.7.2: register access, Chapter 2.7.3: AXI bus name and id management)<br>
 * OSAL Power Manager API used in this function is enabled when SUPPORT_POWER is enabled.
 * - Open sample device by using @link device_open @endlink
 * - Do the IO register access by using @link io_register_access_task @endlink
 * - Execute the IO AXI bus name and AXI bus ID management task by calling @link io_axi_name_and_id_management_task @endlink
 * - Close the sample device by using R_OSAL_IoDeviceClose
 * 
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t io_main_task(void)
{
    e_osal_return_t osal_ret;
    int32_t app_ret;
    bool io_tasks_success = true;
    bool device_open_flag = false;
    osal_device_handle_t dev_handle = OSAL_DEVICE_HANDLE_INVALID;

#if defined(OSAL_SOC_S4)
    osal_device_handle_t dev_handle_sub;	
    (void)device_open(SAMPLE_DEVICE_TYPE_SUB, 0, &dev_handle_sub);
    (void)R_OSAL_PmSetRequiredState(dev_handle_sub, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
    (void)R_OSAL_IoDeviceClose(dev_handle_sub);
#endif

    /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
    (void)printf("\n================== io_main_task start ==================\n");
    app_ret = device_open(SAMPLE_DEVICE_TYPE, 0, &dev_handle);
    if(0 != app_ret)
    {
        io_tasks_success = false;
        OSAL_SAMPLE_ERR("%s opening was failed \n", SAMPLE_DEVICE_ID);
    }
    else
    {
        device_open_flag = true;
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Start executing tasks related to register accession\n");
        app_ret = io_register_access_task(dev_handle);
        if(0 != app_ret)
        {
            io_tasks_success = false;
            OSAL_SAMPLE_ERR("Register accession was failed\n");
        }
    }

#if defined(SUPPORT_AXI_BUS_NAME)
    OSAL_SAMPLE_INFO("Start executing tasks related to Axi bus name\n");
    app_ret = io_axi_name_and_id_management_task(dev_handle);
    if(0 != app_ret)
    {
        io_tasks_success = false;
        OSAL_SAMPLE_ERR("Axi bus name tasks was failed\n");
    }
#endif

    if(false == io_tasks_success)
    {
        app_ret = -1;
    }

    if(true == device_open_flag)
    {
        OSAL_SAMPLE_INFO("close device\n");
        osal_ret = R_OSAL_IoDeviceClose(dev_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Device closing failed, osal_ret = %d\n", osal_ret);
        }
    }

    /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
    (void)printf("================== io_main_task finish ==================\n\n");
    return app_ret;
}

/**
 * @defgroup io_device_open OSAL IO Device Opening APIs Usage
 * @{
 */
/**
 * @brief This function uses to find the sample device ID
 * - Split the input device list passed through the device config structure
 * - Select the sample device ID from the splitted device list based on the input device channel
 *
 * @param[in] device_config Information of all available devices corresponding with an input device type in current system
 * @param[in] device_channel The device channel number:
 * - If device only has a single channel. Example: system_ram_00, please input channel number "0" to this parameter
 * - If device has multi channels (Example: ims_00, ims_01, ...), please input the expected channel number "0" or "1", ... to this parameter
 * @param[in, out] device_id Found device's name corresponding with device_channel and existing in device_config will be stored in this parameter
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t find_device_id(osal_sample_device_config_t device_config, size_t device_channel, char** device_id)
{
    int32_t app_ret =0;
    size_t j = 0;
    char* splitted_devicelist[device_config.num_of_devices];

    splitted_devicelist[0] = &device_config.device_list[0];
    for (size_t i = 0; i < device_config.num_of_bytes; i++)
    {
        if(device_config.device_list[i] == '\n')
        {
            j++;
            device_config.device_list[i] = '\0';
            splitted_devicelist[j] = &device_config.device_list[i+1U];
        }
    }

    if(j > device_config.num_of_devices)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Device parsing error \n");
    }

    if(0 == app_ret)
    {
        *device_id = splitted_devicelist[device_channel];
    }
    else
    {
        *device_id = NULL;
    }

    return app_ret;
}

/**
 * @brief This function uses to get the list of available devices in the current system
 * - Get the number of available devices in the current system by using R_OSAL_IoGetNumOfDevices
 * - Get the required size of the buffer which is used to store the device list by calling to R_OSAL_IoGetDeviceList
 * - Allocate the buffer with the required size above
 * - Get the device list by using R_OSAL_IoGetDeviceList
 *
 * @param[in] device_type The OSAL device type. This function will get all available devices corresponding with this device type.
 * @param[in, out] device_config An pointer which is used for storing the information of all devices related to device_type
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t get_device_list(const char * device_type, osal_sample_device_config_t* device_config)
{
    int32_t app_ret = 0;
    char* local_device_list;
    e_osal_return_t osal_ret;
    size_t requiredSize = 0;

    OSAL_SAMPLE_INFO("Get the number of devices\n");
    osal_ret = R_OSAL_IoGetNumOfDevices(device_type, &device_config->num_of_devices);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Get the number of devices failed, device_type = %s osal_ret = %d\n", device_type, osal_ret);
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Get required size of device list\n");
        osal_ret = R_OSAL_IoGetDeviceList(device_type, NULL, 0, &requiredSize);
        if(OSAL_RETURN_OK_RQST != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Get the required size of the device list buffer failed, device_type = %s osal_ret = %d\n", device_type,osal_ret);
        }
    }

    OSAL_SAMPLE_INFO("Allocate a buffer to store device list\n");
    /* PRQA S 5118 1 # There is no memory leak here. free was called in the calling function. */
    local_device_list = (char*)malloc(requiredSize);
    if(NULL == local_device_list)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Allocate a buffer to store device list failed\n");
    }

    OSAL_SAMPLE_INFO("Get the list of device identifiers\n");
    osal_ret = R_OSAL_IoGetDeviceList(device_type, local_device_list, requiredSize, &device_config->num_of_bytes);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Get device list buffer failed, device_type = %s osal_ret = %d\n", device_type, osal_ret);
    }

    if(0 == app_ret)
    {
        /* PRQA S 2916 1 # The address stored in local_device_list is freed in the calling function. */
       device_config->device_list = local_device_list;
    }
    else
    {
        device_config->device_list = NULL;
    }

    return app_ret;
}

/**
 * @brief Device opening function
 * - Get the device list by calling @link get_device_list @endlink
 * - Find the sample device from the device list by using @link find_device_id @endlink
 * - Open the found sample device by using R_OSAL_IoDeviceOpen
 *
 * @param[in] device_type OSAL device type of device which is going to be opened
 * @param[in] device_channel The device channel number:
 * - If device only has a single channel. Example: system_ram_00, please input channel number "0" to this parameter
 * - If device has multi channels (Example: ims_00, ims_01, ...), please input the expected channel number "0" or "1", ... to this parameter
 * @param[in, out] device_handle OSAL device handle of opened device will be stored in this parameter
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t device_open(const char * device_type, size_t device_channel, osal_device_handle_t *device_handle)
{
    int32_t app_ret;
    e_osal_return_t osal_ret;
    char* device_id = NULL;
    osal_sample_device_config_t device_config;

    OSAL_SAMPLE_INFO("Get device list\n");
    app_ret = get_device_list(device_type, &device_config);
    if(0 != app_ret)
    {
        OSAL_SAMPLE_ERR("Get device list failed\n");
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Find the target device ID from the device list\n");
        app_ret = find_device_id(device_config, device_channel, &device_id);
        if(0 != app_ret)
        {
            OSAL_SAMPLE_ERR("Split device list failed\n");
        }
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("open device: %s\n", device_id);
        osal_ret = R_OSAL_IoDeviceOpen(device_id, device_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Device opening was failed, osal_ret = %d\n", osal_ret);
        }
        else
        {
            OSAL_SAMPLE_INFO("open device %s successfully\n", device_id);
        }
    }

    if(0 != app_ret)
    {
        *device_handle = OSAL_DEVICE_HANDLE_INVALID;
    }

    if(NULL != device_config.device_list)
    {
        /* PRQA S 5118 2 # Allocated in the sub-function called by this function */
        /* PRQA S 0314 1 # The 1st parameter must be (void *) and there is no side-effect in this case. */
        (void)free((void*)device_config.device_list);
    }

    return app_ret;
}
/**@} io_device_open*/

/**
 * @defgroup io_register_access OSAL IO register access APIs usage
 * @{
 */
/**
 * @brief This function is to do the IO register accessing (read/write)
 * - Active the power and clock domain of the sample device by using @link pm_control_active @endlink
 * - Access (read/write) the device register with a single block (32-bit)
 * - Access (read/write) the device register with multi-block (SAMPLE_BLOCK_SIZE byte)
 * - Deactivate the power and clock domain of the sample device by using @link pm_control_deactive @endlink
 *
 * @param[in] dev_handle OSAL device handle of device which this function does the IO register accessing to
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t io_register_access_task(osal_device_handle_t dev_handle)
{
    int32_t app_ret;
    bool is_pm_active = false;
    bool register_access_success = true;

#if defined(SUPPORT_POWER)
    OSAL_SAMPLE_INFO("Activate the device pm\n");
    app_ret = pm_control_active(dev_handle);
    if(0 != app_ret)
    {
        register_access_success = false;
        OSAL_SAMPLE_ERR("Device power activation failed\n");
    }
    else
    {
        is_pm_active = true;
    }
#else
    is_pm_active = true;
#endif

    if(true == is_pm_active)
    {
        OSAL_SAMPLE_INFO("Start checking read write operation for single block \n");
        app_ret = io_read_write_single_block(dev_handle);
        if(0 != app_ret)
        {
            register_access_success = false;
            OSAL_SAMPLE_ERR("Write and read for the single block was failed\n");
        }
    }

    if(true == is_pm_active)
    {
        OSAL_SAMPLE_INFO("Start checking read write operation for multi blocks \n");
        app_ret = io_read_write_multi_blocks(dev_handle);
        if(0 != app_ret)
        {
            register_access_success = false;
            OSAL_SAMPLE_ERR("Write and read operation for multi blocks failed\n");
        }
    }

#if defined(SUPPORT_POWER)
    if(true == is_pm_active)
    {
        OSAL_SAMPLE_INFO("Deactivate the device pm\n");
        app_ret = pm_control_deactive(dev_handle);
        if(0 != app_ret)
        {
            register_access_success = false;
            OSAL_SAMPLE_ERR("Device power deactivation failed\n");
        }
    }
#endif

    if(false == register_access_success)
    {
        app_ret = -1;
    }

    return app_ret;
}

/**
 * @brief This function is to access the device register with a single block (32-bit)
 * - Read/write single block data with register offset SAMPLE_SINGLE_BLOCK_OFFSET by using R_OSAL_IoRead32/R_OSAL_IoWrite32 with steps as below:
 *      -# Write data to register by using R_OSAL_IoWrite32
 *      -# Read back data from the register by using R_OSAL_IoRead32
 *      -# the read-back data must be equal to the written data.
 * - Apply the software reset
 *
 * @param[in] dev_handle OSAL device handle of device which this function does the single-block IO register accessing to
 * @return 0 on success
 * @return -1 on failure
 * @note Please select the Read/Write API that matches the characteristics of the platform architecture and OS.
 */
static int32_t io_read_write_single_block(osal_device_handle_t dev_handle)
{
    int32_t app_ret = 0;
    uint32_t read_data32 = 0;
    e_osal_return_t osal_ret;
#if defined(SUPPORT_POWER)
    bool write_success = false;
#endif

    OSAL_SAMPLE_INFO("Write data is 0x%08x to %s with offset is 0x%08x by using R_OSAL_IoWrite32\n", SAMPLE_WRITE_DATA32, SAMPLE_DEVICE_ID, SAMPLE_SINGLE_BLOCK_OFFSET);
    osal_ret = R_OSAL_IoWrite32(dev_handle, SAMPLE_SINGLE_BLOCK_OFFSET, SAMPLE_WRITE_DATA32);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("R_OSAL_IoWrite32 failed, osal_ret = %d\n", osal_ret);
    }
    else
    {
#if defined(SUPPORT_POWER)
        write_success = true;
#endif
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Read data from %s with offset is 0x%08x by using R_OSAL_IoRead32. The read data will be 0x%08x\n", SAMPLE_DEVICE_ID, SAMPLE_SINGLE_BLOCK_OFFSET, SAMPLE_WRITE_DATA32);
        osal_ret = R_OSAL_IoRead32(dev_handle, SAMPLE_SINGLE_BLOCK_OFFSET, &read_data32);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("R_OSAL_IoRead32 failed, osal_ret = %d\n", osal_ret);
        }
    }

    if(0 == app_ret)
    {
        if(read_data32 != SAMPLE_WRITE_DATA32)
        {
            OSAL_SAMPLE_ERR("Read data = 0x%08x is not equal to write data = 0x%08x\n", read_data32, SAMPLE_WRITE_DATA32);
        }
    }

#if defined(SUPPORT_POWER)
    if(true == write_success)
    {
        OSAL_SAMPLE_INFO("Start to apply software reset\n");
        app_ret = pm_apply_software_reset(dev_handle);
        if(0 != app_ret)
        {
            OSAL_SAMPLE_ERR("Apply software reset failed\n");
        }
    }
#endif

    return app_ret;
}

/**
 * @brief This function is to access the device register with multi-block (SAMPLE_BLOCK_SIZE byte)
 * - Read/write multi-block data with register offset SAMPLE_MULTI_BLOCK_OFFSET by using R_OSAL_IoBlockRead32/R_OSAL_IoBlockWrite32 with steps as below:
 *      -# Write data to register by using R_OSAL_IoBlockWrite32
 *      -# Read back data from the register by using R_OSAL_IoBlockRead32
 *      -# the read-back data must be equal to the written data.
 * - Apply the software reset
 *
 * @param[in] dev_handle OSAL device handle of device which this function does the multi-block IO register accessing to
 * @return 0 on success
 * @return -1 on failure
 * @note Please select the Read/Write API that matches the characteristics of the platform architecture and OS.
 */
static int32_t io_read_write_multi_blocks(osal_device_handle_t dev_handle)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;
    size_t count;
    uint32_t read_block_data32[4];
#if defined(SUPPORT_POWER)
    bool write_success = false;
#endif

    OSAL_SAMPLE_INFO("Write %d-byte data {0x%08x, 0x%08x, 0x%08x, 0x%08x} to %s with offset is 0x%08x by using R_OSAL_IoBlockWrite32\n",
                                                                    SAMPLE_BLOCK_SIZE, SAMPLE_BLOCK_WRITE_DATA32[0], SAMPLE_BLOCK_WRITE_DATA32[1], SAMPLE_BLOCK_WRITE_DATA32[2],
                                                                                        SAMPLE_BLOCK_WRITE_DATA32[3], SAMPLE_DEVICE_ID, SAMPLE_MULTI_BLOCK_OFFSET);
    osal_ret = R_OSAL_IoBlockWrite32(dev_handle, SAMPLE_MULTI_BLOCK_OFFSET, SAMPLE_BLOCK_WRITE_DATA32, SAMPLE_BLOCK_SIZE);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("R_OSAL_IoBlockWrite32 failed, osal_ret = %d\n", osal_ret);
    }
    else
    {
#if defined(SUPPORT_POWER)
        write_success = true;
#endif
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Read %d-byte data from %s with offset is 0x%08x by using R_OSAL_IoBlockRead32\n", SAMPLE_BLOCK_SIZE, SAMPLE_DEVICE_ID, SAMPLE_MULTI_BLOCK_OFFSET);
        OSAL_SAMPLE_INFO("The read data will be {0x%08x, 0x%08x, 0x%08x, 0x%08x}\n", SAMPLE_BLOCK_WRITE_DATA32[0], SAMPLE_BLOCK_WRITE_DATA32[1], SAMPLE_BLOCK_WRITE_DATA32[2], SAMPLE_BLOCK_WRITE_DATA32[3]);
        osal_ret = R_OSAL_IoBlockRead32(dev_handle, SAMPLE_MULTI_BLOCK_OFFSET, read_block_data32, SAMPLE_BLOCK_SIZE);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("R_OSAL_IoBlockRead32 failed, osal_ret = %d\n", osal_ret);
        }
    }

    if(0 == app_ret)
    {
        for(count = 0; count < ((size_t)SAMPLE_BLOCK_SIZE/sizeof(SAMPLE_BLOCK_WRITE_DATA32[0])); count++)
            {
                if(SAMPLE_BLOCK_WRITE_DATA32[count] != read_block_data32[count])
                {
                    OSAL_SAMPLE_ERR("Read data = 0x%08x is not equal to write data = 0x%08x\n", read_block_data32[count], SAMPLE_BLOCK_WRITE_DATA32[count]);
                    break;
                }
            }
    }

#if defined(SUPPORT_POWER)
    if(true == write_success)
    {
        OSAL_SAMPLE_INFO("Start to apply software reset\n");
        app_ret = pm_apply_software_reset(dev_handle);
        if(0 != app_ret)
        {
            OSAL_SAMPLE_ERR("Apply software reset failed\n");
        }
    }
#endif

    return app_ret;
}

/**@} io_register_access*/

/**
 * @defgroup io_axi_bus_name OSAL IO AXI Bus name usage
 * @{
 */
/**
 * @brief This function is to describe the function of AXI bus name and AXi bus ID management
 * - Get the list of available AXI bus names in the current system by calling @link io_axi_bus_name_list_task @endlink
 * - Get the hw/peripheral/axi bus domain-specific address by using @link get_hw_address_through_axi_id_task @endlink
 *
 * @param[in] dev_handle OSAL device handle. This function get the AXI bus ID list based on this device.
 * @return 0 on success
 * @return -1 on failure
 */
#if defined(SUPPORT_AXI_BUS_NAME)
static int32_t io_axi_name_and_id_management_task(osal_device_handle_t dev_handle)
{
    int32_t app_ret;
    char target_axi_bus_name[100];

    OSAL_SAMPLE_INFO("Start getting the axi bus name list \n");
    app_ret = io_axi_bus_name_list_task(&target_axi_bus_name[0]);
    if(0 != app_ret)
    {
        OSAL_SAMPLE_ERR("Getting the axi bus name list failed\n");
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Start getting hw/peripheral/axi bus domain-specific address through axi id: \n");
        /* PRQA S 2822 2 # There is no problem because the pointer and size are checked
         by confirming the value of app_ret */
        app_ret = get_hw_address_through_axi_id_task(target_axi_bus_name, SAMPLE_DEVICE_ID, dev_handle);
        if(0 != app_ret)
        {
            OSAL_SAMPLE_ERR("Get hw/peripheral/axi bus domain-specific address through axi id was failed\n");
        }
    }

    return app_ret;
}

/**
 * @brief This function is to get and show the list of available AXI bus names in the current system
 * - Get the list of available AXI bus names in the current system by calling @link get_bus_name_list @endlink
 * - Split and show all elements in the AXI bus name list by using @link split_axi_bus_name @endlink
 * - Select an AXI bus name from the list. This is an input of @link get_hw_address_through_axi_id_task @endlink
 *
 * @param[in, out] selected_axi_bus_name This parameter is to store the selected AXI bus name from the list of all available AXI bus name.<br>
 * In this sample, we choose the 3rd AXI bus name in the list because 1st and 2nd AXI bus name are "invalid" and "mm(ipa)".
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t io_axi_bus_name_list_task(char* selected_axi_bus_name)
{
    int32_t app_ret;
    size_t numOfByte_list = 0;
    char* axi_bus_name_list = NULL;
    char** splitted_axi_bus_name_list;
    size_t num_of_axi_bus_name = 0;

    OSAL_SAMPLE_INFO("Get the AXI bus name list\n");
    app_ret = get_bus_name_list(&axi_bus_name_list, &numOfByte_list);
    if(0 != app_ret)
    {
        OSAL_SAMPLE_ERR("Get the AXI bus name list failed");
    }

    if(0 == app_ret)
    {
        for(size_t i = 0; i < numOfByte_list; i++)
        {
            /* PRQA S 2822 2 # There is no problem because the pointer and size are checked
             by confirming the value of app_ret */
            if('\0' == axi_bus_name_list[i])
            {
                num_of_axi_bus_name++;
            }
        }
    }

    /* PRQA S 5118,2707 1 # There is no memory leak here. free was called in the calling function. */
    splitted_axi_bus_name_list = (char **)malloc(sizeof(char*)*num_of_axi_bus_name);
    if(NULL != splitted_axi_bus_name_list)
    {
        OSAL_SAMPLE_INFO("Start splitting the AXI bus name list and show the axi bus list content\n");
        app_ret = split_axi_bus_name(axi_bus_name_list, numOfByte_list, num_of_axi_bus_name, splitted_axi_bus_name_list);
    }

    if(0 == app_ret)
    {
        /* PRQA S 2822 2 # There is no problem because the pointer and size are checked
         by confirming the value of app_ret */
        (void)strcpy(selected_axi_bus_name, splitted_axi_bus_name_list[2]);
    }

    if(NULL != splitted_axi_bus_name_list)
    {
        /* PRQA S 5118 2 # Allocated in this function */
        /* PRQA S 0314 1 # The 1st parameter must be (void *) and there is no side-effect in this case. */
        (void)free((void*)splitted_axi_bus_name_list);
    }

    if(NULL != axi_bus_name_list)
    {
        /* PRQA S 5118 2 # Allocated in the sub-function called by this function */
        /* PRQA S 0314 1 # The 1st parameter must be (void *) and there is no side-effect in this case. */
        (void)free((void*)axi_bus_name_list);
    }

    return app_ret;
}

/**
 * @brief This function is to get the list of available AXI bus names in the current system
 * - Get the required size of the buffer which is used to store the AXI bus name list by calling R_OSAL_IoGetAxiBusNameList
 * - Allocate a buffer with the required size above
 * - Get the list of AXI bus names by calling to R_OSAL_IoGetAxiBusNameList
 *
 * @param[in, out] axi_name_list This parameter is to store the list of all available AXI bus name
 * @param[in, out] numOfByte_list This parameter is to store the number of bytes (to be) written to the AXI bus name list's buffer
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t get_bus_name_list(char** axi_name_list, size_t *numOfByte_list)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;
    char* local_axi_name_list;
    size_t local_numofbyte_list = 0;
    size_t required_size_list = 0;

    OSAL_SAMPLE_INFO("Get the required size AXI bus name list by using R_OSAL_IoGetAxiBusNameList\n");
    osal_ret = R_OSAL_IoGetAxiBusNameList(NULL, 0, &required_size_list);
    if(OSAL_RETURN_OK_RQST != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Getting the required size of AXI bus name list failed, osal_ret = %d\n", osal_ret);
    }
    else
    {
        /* PRQA S 5209 1 # This casting to fix compiler-dependent. There is no side-effect in this case */
        OSAL_SAMPLE_INFO("The required size AXI bus name list is %lu\n", (unsigned long)required_size_list);
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Allocate a buffer for the AXI name list\n");
        /* PRQA S 5118,2707 1 # There is no memory leak here. free was called in the calling function. */
        local_axi_name_list = (char *)malloc(required_size_list);
        if(NULL == local_axi_name_list)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("AXI bus name list allocation failed\n");
        }
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Get the string list of all axi bus names by using R_OSAL_IoGetAxiBusNameList\n");
        osal_ret = R_OSAL_IoGetAxiBusNameList(local_axi_name_list, required_size_list, &local_numofbyte_list);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Get AXI bus name list failed, osal_ret = %d\n", osal_ret);
        }

        if(required_size_list != local_numofbyte_list)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("R_OSAL_IoGetAxiBusNameList operation failed \n");
        }
    }

    if(0 == app_ret)
    {
        /* PRQA S 2916 1 # The address stored in axi_name_list is freed in the calling function. */
        *axi_name_list = local_axi_name_list;
        *numOfByte_list = local_numofbyte_list;
    }
    else
    {
        *axi_name_list = NULL;
        *numOfByte_list = 0;
    }

    return app_ret;
}

/**
 * @brief This function is to split and show the AXI bus name list
 * - Split the AXi bus name list
 * - Show all elements of the AXI bus name list
 *
 * @param[in] axi_bus_name_list AXI bus name list which is going to be split
 * @param[in] numOfByte_list Amount of bytes (to be) written to the buffer AXI bus name list
 * @param[in] numOfAxiBus Number of AXI bus name in the list
 * @param[in, out] splitted_axi_bus_name_list This parameter is an array which each element contains an AXI bus name split from the AXI bus name list
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t split_axi_bus_name(char* axi_bus_name_list, size_t numOfByte_list, size_t numOfAxiBus, char** splitted_axi_bus_name_list)
{
    int32_t app_ret = 0;
    size_t j = 0;

    splitted_axi_bus_name_list[0] = &axi_bus_name_list[0];
    for (size_t i = 0; i < (numOfByte_list - 1U); i++)
    {
        if(axi_bus_name_list[i] == '\0')
        {
            j++;
            splitted_axi_bus_name_list[j] = &axi_bus_name_list[i+1U];
        }
    }

    if(j > numOfAxiBus)
    {
        app_ret = -1;
        OSAL_SAMPLE_INFO("Splitting axi bus name list failed\n");
    }

    OSAL_SAMPLE_INFO("Show the content of each element of AXI bus name list:\n");

    /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
    (void)printf("|%-45s|\n", "AXI Bus Name in the list");
    for(size_t i = 0; i < numOfAxiBus; i++)
    {
        /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
        (void)printf("|%-45s|\n", splitted_axi_bus_name_list[i]);
    }

    return app_ret;
}

/**
 * @brief This function is to get the AXI bus name from the given AXI bus ID
 * - Get the required size of the buffer which is used to store the AXI bus name by using R_OSAL_IoGetAxiBusName
 * - Allocate a buffer with the required size above
 * - Get the AXI bus name by using R_OSAL_IoGetAxiBusName
 *
 * @param[in] axi_id AXI bus ID which. This function will get the AXI bus name corresponding with this AXI bus ID
 * @param[in, out] axi_name This parameter is to store the output AXI bus name
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t get_axi_bus_name(osal_axi_bus_id_t axi_id, char** axi_name)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;
    size_t numOfByte_name = 0;
    size_t required_size = 0;
    char* local_axi_name;

    osal_ret = R_OSAL_IoGetAxiBusName(axi_id, NULL, 0, &required_size);
    if(OSAL_RETURN_OK_RQST != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Getting the required size failed, osal_ret = %d\n", osal_ret);
    }
    else if(0U == required_size)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Operation failed\n");
    }
    else
    {
        /* R_OSAL_IoGetAxiBusName work normally */
    }

    if(0 == app_ret)
    {
        /* PRQA S 5118,2707 1 # There is no memory leak here. free was called in the calling function. */
        local_axi_name = (char*)malloc(required_size);
        if(NULL == local_axi_name)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Allocate a buffer for the AXI name failed\n");
        }
    }

    if(0 == app_ret)
    {
        osal_ret = R_OSAL_IoGetAxiBusName(axi_id, local_axi_name, required_size, &numOfByte_name);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Get AXI bus name failed, osal_ret = %d\n", osal_ret);
        }

        if(numOfByte_name != required_size)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Operation failed\n");
        }

    }

    if(0 == app_ret)
    {
        /* PRQA S 2916 1 # The address stored in axi_name is freed in the calling function. */
        *axi_name = local_axi_name;
    }
    else
    {
        *axi_name = NULL;
    }

    return app_ret;
}

/**
 * @brief This function is to get and show the hw/peripheral/axi bus domain-specific address with the given sample device handle, device name, AXI bus name
 * - Get and show the hw/peripheral/axi bus domain-specific address with the given AXI bus name by using @link io_axi_id_from_axi_name_task @endlink
 * - Get and show the hw/peripheral/axi bus domain-specific address with the given sample device name by using @link io_axi_id_from_dev_name_task @endlink
 * - Get and show the hw/peripheral/axi bus domain-specific address with the given sample device handle by using @link io_axi_id_from_dev_handle_task @endlink
 *
 * @param[in] axi_name AXI bus name which is used for getting the AXI bus ID list
 * @param[in] device_name Device name which is used for getting the AXI bus ID list
 * @param[in] device_handle OSAL device handle which is used for getting the AXI bus ID list
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t get_hw_address_through_axi_id_task(const char* axi_name, const char* device_name, osal_device_handle_t device_handle)
{
    int32_t app_ret;
    bool success = true;

    OSAL_SAMPLE_INFO("Start getting axi id list from axi name, device name, device handle\n");
    app_ret = io_axi_id_from_axi_name_task(axi_name);
    if(0 != app_ret)
    {
        success = false;
        OSAL_SAMPLE_ERR("Can't get axi id list from axi name \n");
    }

    app_ret = io_axi_id_from_dev_name_task(device_name);
    if(0 != app_ret)
    {
        success = false;
        OSAL_SAMPLE_ERR("Can't get axi id list from device name \n");
    }

    app_ret = io_axi_id_from_dev_handle_task(device_handle);
    if(0 != app_ret)
    {
        success = false;
        OSAL_SAMPLE_ERR("Can't get axi id list from device handle \n");
    }

    if(false == success)
    {
        app_ret = -1;
    }

    return app_ret;
}

/**
 * @brief This function is to get and show the hw/peripheral/axi bus domain-specific address with the given AXI bus name
 * - Get the AXI bus ID list with the given AXI bus name by using @link get_axi_id_list_from_axi_name @endlink
 * - Get and show the hw/peripheral/axi bus domain-specific address with an AXI bus ID from the list above by using @link get_hw_address @endlink
 *
 * @param[in] axi_name The AXI bus name which is used for getting the AXI bus ID list
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t io_axi_id_from_axi_name_task(const char* axi_name)
{
    int32_t app_ret;
    osal_axi_bus_id_t* axi_id_list_from_axi_name = NULL;

    OSAL_SAMPLE_INFO("Get the list of AXI IDs with the given axi name (%s) \n", axi_name);
    app_ret = get_axi_id_list_from_axi_name(axi_name, &axi_id_list_from_axi_name);
    if(0 != app_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Get axi id list from axi name failed\n");
    }

    if(0 == app_ret)
    {
        app_ret = get_hw_address(axi_id_list_from_axi_name);
        if(0 != app_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Get the list of AXI IDs with the given axi name: %s failed\n", axi_name);
        }
    }

    if(NULL != axi_id_list_from_axi_name)
    {
        /* PRQA S 5118 2 # Allocated in the sub-function called by this function */
        /* PRQA S 0314 1 # The 1st parameter must be (void *) and there is no side-effect in this case. */
        (void)free((void*)axi_id_list_from_axi_name);
    }

    return app_ret;
}

/**
 * @brief This function is to get and show the hw/peripheral/axi bus domain-specific address with the given sample device name
 * - Get the AXI bus ID list with the given sample device name by using @link get_axi_id_list_from_dev_name @endlink
 * - Get and show the hw/peripheral/axi bus domain-specific address with an AXI bus ID from the list above by using @link get_hw_address @endlink
 *
 * @param[in] device_name Device name which is used for getting the AXI bus ID list
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t io_axi_id_from_dev_name_task(const char* device_name)
{
    int32_t app_ret;
    osal_axi_bus_id_t* axi_id_list_from_device_name = NULL;

    OSAL_SAMPLE_INFO("Get the list of AXI IDs with the given device name %s\n", device_name);
    app_ret = get_axi_id_list_from_dev_name(device_name, &axi_id_list_from_device_name);
    if(0 != app_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Get the list of AXI IDs with the given device %s name failed\n", device_name);
    }

    if(0 == app_ret)
    {
        app_ret = get_hw_address(axi_id_list_from_device_name);
        if(0 != app_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Get hw/peripheral/axi bus domain-specific address of axi id list from device name failed\n");
        }
    }

    if(NULL != axi_id_list_from_device_name)
    {
        /* PRQA S 5118 2 # Allocated in the sub-function called by this function */
        /* PRQA S 0314 1 # The 1st parameter must be (void *) and there is no side-effect in this case. */
        (void)free((void*)axi_id_list_from_device_name);
    }

    return app_ret;
}

/**
 * @brief This function is to get and show the hw/peripheral/axi bus domain-specific address with the given sample device handle
 * - Get the AXI bus ID list with the given sample device handle by using @link get_axi_id_list_from_dev_handle @endlink
 * - Get and show the hw/peripheral/axi bus domain-specific address with an AXI bus ID from the list above by using @link get_hw_address @endlink
 *
 * @param[in] dev_handle OSAL device handle which is used for getting the AXI bus ID list
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t io_axi_id_from_dev_handle_task(osal_device_handle_t dev_handle)
{
    int32_t app_ret;
    osal_axi_bus_id_t* axi_id_list_from_device_handle = NULL;

    OSAL_SAMPLE_INFO("Get the list of AXI IDs with the given device handle of device name %s\n", SAMPLE_DEVICE_ID);
    app_ret = get_axi_id_list_from_dev_handle(dev_handle, &axi_id_list_from_device_handle);
    if(0 != app_ret)
    {
        OSAL_SAMPLE_ERR("Get the list of AXI IDs with the given device handle related to device %s name failed\n", SAMPLE_DEVICE_ID);
    }

    if(0 == app_ret)
    {
        app_ret = get_hw_address(axi_id_list_from_device_handle);
        if(0 != app_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Get hw/peripheral/axi bus domain-specific address of axi id list from device handle failed\n");
        }
    }

    if(NULL != axi_id_list_from_device_handle)
    {
        /* PRQA S 5118 2 # Allocated in the sub-function called by this function */
        /* PRQA S 0314 1 # The 1st parameter must be (void *) and there is no side-effect in this case. */
        (void)free((void*)axi_id_list_from_device_handle);
    }

    return app_ret;
}

/**
 * @brief This function is to get and show the list of AXI bus IDs with the given AXi bus name
 * - Get the number of AXI bus IDs related to the given AXI bus name by using R_OSAL_IoGetNumOfAxiIdsForAxiBusName
 * - Allocate a buffer with the size is the number of AXI bus ID above
 * - Get the AXi bus ID list by using R_OSAL_IoGetAxiIdListForAxiBusName
 * - Get and show the AXI bus name which is related to each element in the AXI bus ID list by using @link get_axi_bus_name @endlink
 *
 * @param[in] axi_name The AXI bus name which is used for getting the AXI bus ID list
 * @param[in, out] axi_id_list This parameter is to store the AXI bus ID list corresponding with axi_name
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t get_axi_id_list_from_axi_name(const char* axi_name, osal_axi_bus_id_t** axi_id_list)
{
    int32_t app_ret = 0;
    size_t num_axi_id = 0;
    char* axi_names = NULL;
    e_osal_return_t osal_ret;
    osal_axi_bus_id_t* local_axi_id_list;

    OSAL_SAMPLE_INFO("Get the number of AXI IDs with the given AXI bus name (%s) by using R_OSAL_IoGetNumOfAxiIdsForAxiBusName\n", axi_name);
    osal_ret = R_OSAL_IoGetNumOfAxiIdsForAxiBusName(axi_name, &num_axi_id);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Get the number of AXI IDs failed, osal_ret = %d\n", osal_ret);
    }
    else
    {
        /* PRQA S 5209 1 # This casting to fix compiler-dependent. There is no side-effect in this case */
        OSAL_SAMPLE_INFO("The number of AXI IDs with the given AXI bus name (%s) is (%lu)\n", axi_name, (unsigned long)num_axi_id);
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Allocate a buffer for the AXI ID list\n");
        /* PRQA S 5118,2707 1 # There is no memory leak here. free was called in the calling function. */
        local_axi_id_list = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_axi_id);
        if(NULL == local_axi_id_list)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Memory allocation for the list of AXI IDs failed\n");
        }
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Get the list of AXI IDs with the given AXI bus name (%s) by using R_OSAL_IoGetAxiIdListForAxiBusName\n", axi_name);
        osal_ret = R_OSAL_IoGetAxiIdListForAxiBusName(axi_name, local_axi_id_list, num_axi_id, &num_axi_id);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Get the list of AXI IDs failed, osal_ret = %d\n", osal_ret);
        }
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Get the AXI bus name of all AXI IDs from the list above\n");
        /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
        (void)printf("|%-7s|%-12s|\n", "AXI IDs", "AXI Bus Name");
        for(size_t i = 0; i < num_axi_id; i++)
        {
            app_ret = get_axi_bus_name(local_axi_id_list[i], &axi_names);
            if(0 != app_ret)
            {
                OSAL_SAMPLE_ERR("Get the AXI bus name failed\n");
            }
            else
            {
                /* PRQA S 5209 2 # This casting to fix compiler-dependent. There is no side-effect in this case */
                /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
                (void)printf("|%-7lu|%-12s|\n", (unsigned long)local_axi_id_list[i], axi_names);
                if(NULL != axi_names)
                {
                    /* PRQA S 5118 2 # Allocated in the sub-function called by this function */
                    /* PRQA S 0314 1 # The 1st parameter must be (void *) and there is no side-effect in this case. */
                    (void)free((void*)axi_names);
                    axi_names = NULL;
                }
            }
        }
    }

    if(0 == app_ret)
    {
        *axi_id_list = local_axi_id_list;
    }
    else
    {
        *axi_id_list = NULL;
    }

    return app_ret;
}

/**
 * @brief This function is to get and show the list of AXI bus IDs with the given sample device name
 * - Get the number of AXI bus IDs related to the given sample device name by using R_OSAL_IoGetNumOfAxiIdsForDeviceName
 * - Allocate a buffer with the size is the number of AXI bus ID above
 * - Get the AXi bus ID list by using R_OSAL_IoGetAxiIdListForDeviceName
 * - Get and show the AXI bus name which is related to each element in the AXI bus ID list by using @link get_axi_bus_name @endlink
 *
 * @param[in] device_name Device name which is used for getting the AXI bus ID list
 * @param[in, out] axi_id_list This parameter is to store the AXI bus ID list corresponding with device_name
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t get_axi_id_list_from_dev_name(const char* device_name, osal_axi_bus_id_t** axi_id_list)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;
    size_t num_axi_id = 0;
    osal_axi_bus_id_t* local_axi_id_list = NULL;
    char* axi_name = NULL;
    bool unsupport_axi_bus_device = false;

    OSAL_SAMPLE_INFO("Get the number of AXI IDs with the given device name (%s) by using R_OSAL_IoGetNumOfAxiIdsForDeviceName\n", device_name);
    osal_ret = R_OSAL_IoGetNumOfAxiIdsForDeviceName(device_name, &num_axi_id);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Get the number of AXI IDs with the given device name (%s) failed, osal_ret = %d\n", device_name, osal_ret);
    }
    else if(0U == num_axi_id)
    {
        app_ret = -1;
        unsupport_axi_bus_device = true;
        OSAL_SAMPLE_INFO("Device name (%s) doesn't have AXI IDs\n", device_name);
    }
    else
    {
        /* PRQA S 5209 1 # This casting to fix compiler-dependent. There is no side-effect in this case */
        OSAL_SAMPLE_INFO("The number of AXI IDs with the given device name (%s) is (%lu)\n", device_name, (unsigned long)num_axi_id);
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Allocate buffer for the AXI ID list\n");
        /* PRQA S 5118,2707 1 # There is no memory leak here. free was called in the calling function. */
        local_axi_id_list = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_axi_id);
        if(NULL == local_axi_id_list)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Memory allocation for the list of AXI IDs failed\n");
        }
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Get the list of AXI IDs with the given device name (%s) use API R_OSAL_IoGetAxiIdListForDeviceName\n", device_name);
        osal_ret = R_OSAL_IoGetAxiIdListForDeviceName(device_name, local_axi_id_list, num_axi_id, &num_axi_id);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Get the list of AXI IDs failed, osal_ret = %d\n", osal_ret);
        }
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Get the AXI bus name of all AXI IDs from the list above\n");
        /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
        (void)printf("|%-7s|%-12s|\n", "AXI IDs", "AXI Bus Name");
        for(size_t i = 0; i < num_axi_id; i++)
        {
            app_ret = get_axi_bus_name(local_axi_id_list[i], &axi_name);
            if(0 != app_ret)
            {
                OSAL_SAMPLE_ERR("Get the AXI bus name failed\n");
            }
            else
            {
                /* PRQA S 5209 2 # This casting to fix compiler-dependent. There is no side-effect in this case */
                /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
                (void)printf("|%-7lu|%-12s|\n", (unsigned long)local_axi_id_list[i], axi_name);
                if(NULL != axi_name)
                {
                    /* PRQA S 5118 2 # Allocated in the sub-function called by this function */
                    /* PRQA S 0314 1 # The 1st parameter must be (void *) and there is no side-effect in this case. */
                    (void)free((void*)axi_name);
                    axi_name = NULL;
                }
            }
        }
    }

    if(true == unsupport_axi_bus_device)
    {
        app_ret = 0;
    }

    if(0 == app_ret)
    {
        *axi_id_list = local_axi_id_list;
    }
    else
    {
        *axi_id_list = NULL;
    }

    return app_ret;
    /* PRQA S 9104 2 # Sample Application aims to show how to use the OSAL API and the order in which it is called. Splitting functions reduces readability.
    There are no significant side effects to the behavior of the sample app code without modification. */
}

/**
 * @brief This function is to get and show the list of AXI bus IDs with the given sample device handle
 * - Get the number of AXI bus IDs related to the given sample device handle by using R_OSAL_IoGetNumOfAxiIdsForDeviceHdl
 * - Allocate a buffer with the size is the number of AXI bus ID above
 * - Get the AXi bus ID list by using R_OSAL_IoGetAxiIdListForDeviceHdl
 * - Get and show the AXI bus name which is related to each element in the AXI bus ID list by using @link get_axi_bus_name @endlink
 *
 * @param[in] device_handle OSAL device handle which is used for getting the AXI bus ID list
 * @param[in, out] axi_id_list This parameter is to store the AXI bus ID list corresponding with device_handle
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t get_axi_id_list_from_dev_handle(osal_device_handle_t device_handle, osal_axi_bus_id_t** axi_id_list)
{
    int32_t app_ret = 0;
    size_t num_axi_id = 0;
    char* axi_name = NULL;
    e_osal_return_t osal_ret;
    osal_axi_bus_id_t* local_axi_id_list = NULL;
    bool unsupport_axi_bus_device = false;

    OSAL_SAMPLE_INFO("Get the number of AXI IDs with the given device handle related to device name (%s) by using R_OSAL_IoGetNumOfAxiIdsForDeviceHdl\n", SAMPLE_DEVICE_ID);
    osal_ret = R_OSAL_IoGetNumOfAxiIdsForDeviceHdl(device_handle, &num_axi_id);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Get the number of AXI IDs failed, osal_ret = %d\n", osal_ret);
    }
    else if(0U == num_axi_id)
    {
        app_ret = -1;
        unsupport_axi_bus_device = true;
        OSAL_SAMPLE_INFO("Device handle doesn't have AXI IDs\n");
    }
    else
    {
        /* PRQA S 5209 1 # This casting to fix compiler-dependent. There is no side-effect in this case */
        OSAL_SAMPLE_INFO("The number of AXI IDs with the given device handle of device name (%s) is (%lu)\n", SAMPLE_DEVICE_ID, (unsigned long)num_axi_id);
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Allocate buffer for the AXI ID list\n");
        /* PRQA S 5118,2707 1 # There is no memory leak here. free was called in the calling function. */
        local_axi_id_list = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_axi_id);
        if(NULL == local_axi_id_list)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Memory allocation for the list of AXI IDs failed\n");
        }
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Get the list of AXI IDs with the given device handle of device name (%s) use API R_OSAL_IoGetAxiIdListForDeviceHdl\n", SAMPLE_DEVICE_ID);
        osal_ret = R_OSAL_IoGetAxiIdListForDeviceHdl(device_handle, local_axi_id_list, num_axi_id, &num_axi_id);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Get Axi Id list failed, osal_ret = %d\n", osal_ret);
        }
    }


    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Get the AXI bus name of all AXI IDs from the list above\n");
        /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
        (void)printf("|%-7s|%-12s|\n", "AXI IDs", "AXI Bus Name");
        for(size_t i = 0; i < num_axi_id; i++)
        {
            app_ret = get_axi_bus_name(local_axi_id_list[i], &axi_name);
            if(0 != app_ret)
            {
                OSAL_SAMPLE_ERR("Get the AXI bus name failed\n");
            }
            else
            {
                /* PRQA S 5209 2 # This casting to fix compiler-dependent. There is no side-effect in this case */
                /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
                (void)printf("|%-7lu|%-12s|\n", (unsigned long)local_axi_id_list[i], axi_name);
                if(NULL != axi_name)
                {
                    /* PRQA S 5118 2 # Allocated in the sub-function called by this function */
                    /* PRQA S 0314 1 # The 1st parameter must be (void *) and there is no side-effect in this case. */
                    (void)free((void*)axi_name);
                    axi_name = NULL;
                }
            }
        }
    }

    if(true == unsupport_axi_bus_device)
    {
        app_ret = 0;
    }

    if(0 == app_ret)
    {
        *axi_id_list = local_axi_id_list;
    }
    else
    {
        *axi_id_list = NULL;
    }

    return app_ret;
    /* PRQA S 9104 2 # Sample Application aims to show how to use the OSAL API and the order in which it is called. Splitting functions reduces readability.
    There are no significant side effects to the behavior of the sample app code without modification. */
}

/**
 * @brief This function is to get the hw/peripheral/axi bus domain-specific address
 * - Open a memory handle and allocate memory buffer by using @link open_allocate_memory @endlink
 * - Get the hw/peripheral/axi bus domain-specific address R_OSAL_MmngrGetHwAddr
 * - Deallocate the allocated buffer and close the memory handle by using @link deallocate_close_memory @endlink
 *
 * @param[in] axi_id_list The AXI bus ID list got from AXI bus name, device name or OSAL device handle
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t get_hw_address(const osal_axi_bus_id_t* axi_id_list)
{
    e_osal_return_t osal_ret;
    int32_t app_ret;
    char* axi_name = NULL;
    uintptr_t hwAddress = 0;
    osal_memory_manager_handle_t memHandle = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    osal_memory_buffer_handle_t buffHandle = OSAL_MEMORY_BUFFER_HANDLE_INVALID;

    OSAL_SAMPLE_INFO("Open and allocate a memory buffer\n");
    app_ret = open_allocate_memory(&memHandle, &buffHandle);
    if(0 != app_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Open and allocate memory buffer failed\n");
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Get hw/peripheral/axi bus domain specific address of allocated buffer by using R_OSAL_MmngrGetHwAddr\n");
        osal_ret = R_OSAL_MmngrGetHwAddr(buffHandle, axi_id_list[0], &hwAddress);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Can not get the axi bus domain specific address, osal_ret = %d\n", osal_ret);
        }
        else
        {
            app_ret = get_axi_bus_name(axi_id_list[0], &axi_name);
            if(0 != app_ret)
            {
                OSAL_SAMPLE_ERR("Get the AXI bus name failed\n");
            }
            else
            {
                OSAL_SAMPLE_INFO("Display information of AIX ID, AXI bus name, and the hw/peripheral/axi bus domain specific address\n");
                /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
                (void)printf("|%-6s|%-12s|%-18s|%-18s|\n", "AXI ID", "AXI Bus Name", "Buffer handle addr", "HW address");
                /* PRQA S 5124,0306 2 # It is used for the request to output the execution result of the sample. */
                /* PRQA S 5209 1 # This casting to fix compiler-dependent. There is no side-effect in this case */
                (void)printf("|%-6lu|%-12s|0x%016x|0x%016x|\n", (unsigned long)axi_id_list[0], axi_name, (unsigned int)((uintptr_t)&buffHandle), (unsigned int)hwAddress);
                if(NULL != axi_name)
                {
                    /* PRQA S 5118 2 # Allocated in the sub-function called by this function */
                    /* PRQA S 0314 1 # The 1st parameter must be (void *) and there is no side-effect in this case. */
                    (void)free((void*)axi_name);
                }
            }
        }
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Deallocate and close memory\n");
        app_ret = deallocate_close_memory(memHandle, buffHandle);
        if(0 != app_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Deallocate and close memory failed\n");
        }
    }

    return app_ret;
}

/**
 * @brief Open a new memory manager and allocate a new memory buffer
 * - Open a memory handle by using R_OSAL_MmngrOpen with max configuration
 * - Allocate buffers whose size and alignment are 64-byte
 *
 * @param[in] p_memHandle The address of the opened memory manager handle
 * @param[in, out] p_buffHandle The address the of allocated memory buffer handle
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t open_allocate_memory(osal_memory_manager_handle_t* p_memHandle, osal_memory_buffer_handle_t* p_buffHandle)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;
    size_t bufferSize = 64;
    size_t alignment = 64;
    st_osal_mmngr_config_t memMaxConfig;
    osal_memory_manager_handle_t local_memHandle = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    osal_memory_buffer_handle_t local_buffHandle = OSAL_MEMORY_BUFFER_HANDLE_INVALID;

    OSAL_SAMPLE_INFO("Get the maximum memory configuration\n");
    osal_ret = R_OSAL_MmngrGetOsalMaxConfig(&memMaxConfig);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Can not get the memory max configuration, osal_ret = %d\n", osal_ret);
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Open new memory manager\n");
        osal_ret = R_OSAL_MmngrOpen(&memMaxConfig, &local_memHandle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Open memory handle failed , osal_ret = %d\n", osal_ret);
        }
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Allocate buffer\n");
        osal_ret = R_OSAL_MmngrAlloc(local_memHandle, bufferSize, alignment, &local_buffHandle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Memory allocation failed, osal_ret = %d\n", osal_ret);
        }
    }

    if(0 == app_ret)
    {
        *p_memHandle = local_memHandle;
        *p_buffHandle = local_buffHandle;
    }
    else
    {
        *p_memHandle = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
        *p_buffHandle = OSAL_MEMORY_BUFFER_HANDLE_INVALID;
    }

    return app_ret;
}

/**
 * @brief Deallocate the memory buffer and close the memory manager
 * - Deallocate the allocated buffer by using R_OSAL_MmngrDealloc
 * - Close memory handle by using R_OSAL_MmngrClose
 *
 * @param[in] memHandle Memory buffer handle which is going to be closed
 * @param[in, out] buffHandle Memory manager handle which is going to be deallocated
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t deallocate_close_memory(osal_memory_manager_handle_t memHandle, osal_memory_buffer_handle_t buffHandle)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;

    OSAL_SAMPLE_INFO("Deallocate the allocated buffer\n");
    osal_ret = R_OSAL_MmngrDealloc(memHandle, buffHandle);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_INFO("Memory de-allocation failed, osal_ret = %d\n", osal_ret);
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Close memory handle\n");
        osal_ret = R_OSAL_MmngrClose(memHandle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Can not close memory manager, osal_ret = %d\n", osal_ret);
        }
    }

    return app_ret;
}
#endif
/**@} io_axi_bus_name*/
/**@} io_main_task*/

/**
 * @defgroup pm_appendix_task OSAL Power APIs Usage
 * @{
 */
/**
 * @brief Power management appendix task:<br>
 * (OSAL API UM reference: Chapter 2.8.4: Power State timing chart)<br>
 * This function is enabled when SUPPORT_POWER is enabled.
 * - Open sample device by using @link device_open @endlink
 * - Get the lowest intermediate state by using @link get_lowest_intermediate_state @endlink
 * - Close sample device by using R_OSAL_IoDeviceClose
 * 
 * @return 0 on success
 * @return -1 on failure
 */
#if defined(SUPPORT_POWER)
static int32_t pm_appendix_task(void)
{
    e_osal_return_t osal_ret;
    int32_t app_ret;
    osal_device_handle_t dev_handle = OSAL_DEVICE_HANDLE_INVALID;
    e_osal_pm_state_t powerState = OSAL_PM_STATE_INVALID;
    bool device_open_flag = false;

    /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
    (void)printf("\n================== pm_appendix_task start ==================\n");
    app_ret = device_open(SAMPLE_DEVICE_TYPE, 0, &dev_handle);
    if(0 != app_ret)
    {
        OSAL_SAMPLE_ERR("%s opening was failed \n", SAMPLE_DEVICE_ID);
    }
    else
    {
        device_open_flag = true;
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Start getting lowest intermediate state\n");
        app_ret = get_lowest_intermediate_state(dev_handle, &powerState);
        if(SAMPLE_PM_LOWEST_STATE != powerState)
        {
            OSAL_SAMPLE_ERR("lowest intermediate state is not %d, powerState = %d\n", SAMPLE_PM_LOWEST_STATE, powerState);
        }
    }

    if(true == device_open_flag)
    {
        OSAL_SAMPLE_INFO("close device\n");
        osal_ret = R_OSAL_IoDeviceClose(dev_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Device closing failed, osal_ret = %d\n", osal_ret);
        }
    }

    /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
    (void)printf("================== pm_appendix_task finish ==================\n\n");
    return app_ret;
}

/**
 * @brief This function is to get the pm lowest intermediate state of the sample device
 * - Change the pm required state of the sample device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState
 * - Change the pm policy to OSAL_PM_POLICY_CG by using R_OSAL_PmSetPolicy
 * - Change the pm required state of the sample device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState
 * - Change the pm policy to OSAL_PM_POLICY_HP by using R_OSAL_PmSetPolicy
 * - Get the pm lowest intermediate state of the sample device by using R_OSAL_PmGetLowestIntermediatePowerState
 *
 * @param[in] dev_handle OSAl device handle which is used for performing the sequence of PM lowest intermediate state processing
 * @param[in, out] powerState This parameter is to store the output PM lowest intermediate state
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t get_lowest_intermediate_state(osal_device_handle_t dev_handle, e_osal_pm_state_t *powerState)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;
    e_osal_pm_state_t local_powerState = OSAL_PM_STATE_INVALID;

    OSAL_SAMPLE_INFO("Set the power required status to OSAL_PM_REQUIRED_STATE_REQUIRED\n");
    osal_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Set the power required status failed, osal_ret = %d\n", osal_ret);
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Set power policy to OSAL_PM_POLICY_CG\n");
        osal_ret = R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_CG, true);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Set power policy to OSAL_PM_POLICY_CG failed, osal_ret = %d\n", osal_ret);
        }
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Set the power required status to OSAL_PM_REQUIRED_STATE_RELEASED\n");
        osal_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Set the power required status failed, osal_ret = %d\n", osal_ret);
        }
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Set power policy to OSAL_PM_POLICY_HP\n");
        osal_ret = R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_HP, true);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Set power policy to OSAL_PM_POLICY_HP failed, osal_ret = %d\n", osal_ret);
        }
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Get lowest intermediate state\n");
        osal_ret = R_OSAL_PmGetLowestIntermediatePowerState(dev_handle, &local_powerState);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Get lowest intermediate state failed, osal_ret = %d\n", osal_ret);
        }
    }

    if(0 == app_ret)
    {
        *powerState = local_powerState;
    }
    else
    {
        *powerState = OSAL_PM_STATE_INVALID;
    }

    return app_ret;
    /* PRQA S 9104 2 # Sample Application aims to show how to use the OSAL API and the order in which it is called. Splitting functions reduces readability.
    There are no significant side effects to the behavior of the sample app code without modification. */
}

/**
 * @brief This function is to activate the power and clock domain of the sample device<br>
 * (OSAL API UM reference: Chapter 2.8.1: Power control, Chapter 2.8.3: Power State Control, Chapter 2.8.4: Power State timing chart)<br>
 * This function is enabled when SUPPORT_POWER is enabled.
 * - Change the power required state of the sample device to OSAL_PM_REQUIRED_STATE_REQUIRED by using R_OSAL_PmSetRequiredState.
 * - Call to R_OSAL_PmWaitForState to make sure that the state of power and clock are changed to the ON state.
 * - Get back the pm required state by using R_OSAL_PmGetRequiredState
 *
 * @param[in] dev_handle OSAL device handle. This function actives the PM of this device handle
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t pm_control_active(osal_device_handle_t dev_handle)
{
    int32_t app_ret = 0;
    e_osal_return_t osal_ret;
    e_osal_pm_required_state_t requiredState = OSAL_PM_REQUIRED_STATE_INVALID;

    OSAL_SAMPLE_INFO("Set power required state to OSAL_PM_REQUIRED_STATE_REQUIRED\n");
    osal_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, false);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Set required state failed, osal_ret = %d\n", osal_ret);
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Get the current power status. Expect it's OSAL_PM_STATE_READY\n");
        osal_ret = R_OSAL_PmWaitForState(dev_handle, OSAL_PM_STATE_READY, TIMEOUT);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Get power status failed, osal_ret = %d\n", osal_ret);
        }
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Get the current power required status. Expect it's OSAL_PM_REQUIRED_STATE_REQUIRED\n");
        osal_ret = R_OSAL_PmGetRequiredState(dev_handle, &requiredState);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Get the power required status failed, osal_ret = %d\n", osal_ret);
        }
        else if(OSAL_PM_REQUIRED_STATE_REQUIRED != requiredState)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Set required state to OSAL_PM_REQUIRED_STATE_REQUIRED failed, osal_ret = %d\n", osal_ret);
        }
        else
        {
            /*R_OSAL_PmGetRequiredState work normally */
        }
    }

    return app_ret;
}

/**
 * @brief This function is to deactivate the power and clock domain of the sample device<br>
 * (OSAL API UM reference: Chapter 2.8.1: Power control, Chapter 2.8.3: Power State Control, Chapter 2.8.4: Power State timing chart)<br>
 * This function is enabled when SUPPORT_POWER is enabled.
 * - Change the pm policy of the sample device by calling @link pm_policy_control @endlink
 * - Change the power required state of the sample device to OSAL_PM_REQUIRED_STATE_RELEASED by using R_OSAL_PmSetRequiredState.
 * - Get back the pm required state by using R_OSAL_PmGetRequiredState
 *
 * @param[in] dev_handle OSAL device handle. This function deactivates the PM of this device handle
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t pm_control_deactive(osal_device_handle_t dev_handle)
{
    e_osal_return_t osal_ret;
    int32_t app_ret;
    e_osal_pm_required_state_t requiredState = OSAL_PM_REQUIRED_STATE_INVALID;

    app_ret = pm_policy_control(dev_handle, SAMPLE_PM_POLICY);
    if(0 != app_ret)
    {
        OSAL_SAMPLE_ERR("policy control was failed, powerPolicy = OSAL_PM_POLICY_INVALID\n");
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Set power required state to OSAL_PM_REQUIRED_STATE_RELEASED\n");
        osal_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Set the power required status failed, osal_ret = %d\n", osal_ret);
        }
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Get the current power required status. Expect it's OSAL_PM_REQUIRED_STATE_RELEASED\n");
        osal_ret = R_OSAL_PmGetRequiredState(dev_handle, &requiredState);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Get the power required status failed, osal_ret = %d\n", osal_ret);
        }
        else if(OSAL_PM_REQUIRED_STATE_RELEASED != requiredState)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Set required state to OSAL_PM_REQUIRED_STATE_RELEASED failed, osal_ret = %d\n", osal_ret);
        }
        else
        {
            /* R_OSAL_PmGetRequiredState work normally */
        }
    }

    return app_ret;
}

/**
 * @brief This function is to change the pm policy of the sample device<br>
 * This function is enabled when SUPPORT_POWER is enabled.
 * - Change the pm policy of the sample device by calling R_OSAL_PmSetPolicy
 * - Get back the pm policy by using R_OSAL_PmGetPolicy
 *
 * @param[in] dev_handle OSAL device handle. This function uses this handle to perform the control on its PM policy
 * @param[in] powerpolicy Input the expected PM policy to this parameter
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t pm_policy_control(osal_device_handle_t dev_handle, e_osal_pm_policy_t powerpolicy)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;
    e_osal_pm_policy_t local_powerpolicy = OSAL_PM_POLICY_INVALID;

    OSAL_SAMPLE_INFO("Set current power policy to %d\n", powerpolicy);
    osal_ret = R_OSAL_PmSetPolicy(dev_handle, powerpolicy, true);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Set power policy failed, osal_ret = %d\n", osal_ret);
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Get current power policy. Expect it's %d\n",powerpolicy);
        osal_ret = R_OSAL_PmGetPolicy(dev_handle, &local_powerpolicy);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Get power policy failed, osal_ret = %d\n", osal_ret);
        }
        else if(powerpolicy != local_powerpolicy)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Actual power policy (%d) is not the sam as expected powerPolicy (%d)\n", local_powerpolicy, powerpolicy);
        }
        else
        {
            /* R_OSAL_PmGetPolicy work normally */
        }
    }

    return app_ret;
}

/**
 * @brief This function is to apply the software reset on the sample device<br>
 * This function is enabled when SUPPORT_POWER is enabled.
 * - Change the pm reset state to OSAL_PM_RESET_STATE_APPLIED by calling to R_OSAL_PmSetResetState
 * - Call to R_OSAL_PmWaitForState to make sure the reset state of the device is changed already.
 * - Change the pm reset state to OSAL_PM_RESET_STATE_RELEASED by calling to R_OSAL_PmSetResetState
 * - Call to R_OSAL_PmWaitForState again to make sure the reset state of the device is released.
 *
 * @param[in] dev_handle OSAL device handle. This function used this device to perform PM software reset
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t pm_apply_software_reset(osal_device_handle_t dev_handle)
{
    int32_t app_ret = 0;
    e_osal_return_t osal_ret;

    OSAL_SAMPLE_INFO("Set the power reset status to OSAL_PM_RESET_STATE_APPLIED\n");
    osal_ret = R_OSAL_PmSetResetState(dev_handle, OSAL_PM_RESET_STATE_APPLIED);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Set the power reset status to OSAL_PM_RESET_STATE_APPLIED failed, osal_ret = %d\n", osal_ret);
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Get the current power status. Expect it's OSAL_PM_STATE_RESET\n");
        osal_ret = R_OSAL_PmWaitForState(dev_handle, OSAL_PM_STATE_RESET, TIMEOUT);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Get power state failed, osal_ret = %d\n", osal_ret);
        }
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Set the power reset status to OSAL_PM_RESET_STATE_RELEASED\n");
        osal_ret = R_OSAL_PmSetResetState(dev_handle, OSAL_PM_RESET_STATE_RELEASED);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Release reset failed, osal_ret = %d\n", osal_ret);
        }
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Get the current power status. Expect it's OSAL_PM_STATE_READY\n");
        osal_ret = R_OSAL_PmWaitForState(dev_handle, OSAL_PM_STATE_READY, TIMEOUT);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Get power state failed, osal_ret = %d\n", osal_ret);
        }
    }

    return app_ret;
}
#endif
/**@} pm_appendix_task*/

/**
 * @defgroup set_post_clock OSAL Set POST Clock API Usage
 * @{
 */
/**
 * @brief This is a sample application to set POST Clock of Power Manager API when Field Bist Controller(FBC) and Field Bist Activator(FBA) are supported.:<br>
 * (OSAL API UM reference: Chapter 2.8.2: post clock)<br>
 * This function is enabled when SUPPORT_FBA is enabled
 * - Open fba/fbc device by using @link device_open_fba @endlink
 * - Set fba/fbc device's Post clock setting by using R_OSAL_PmSetPostClock
 *     - clock enable is true
 *     - clock_divider = SAMPLE_CLOCK_DIVIDER
 * - Close fba/fbc device by using R_OSAL_IoDeviceClose
 *
 * @return 0 on success
 * @return -1 on failure
 */
#if defined(SUPPORT_FBA)
static int32_t set_post_clock(void)
{
    osal_device_handle_t fba_dev_handle = OSAL_DEVICE_HANDLE_INVALID;
    e_osal_return_t osal_ret;
    bool device_open_flag = false;
    bool clock_enable = true;
    int32_t app_ret;

    /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
    (void)printf("================== set_post_clock start ==================\n\n");
    app_ret = device_open_fba(FBA_SAMPLE_DEVICE_SUFFIX_NAME, &fba_dev_handle);
    if(0 != app_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("fba_%s opening was failed \n", FBA_SAMPLE_DEVICE_SUFFIX_NAME);
    }
    else
    {
        device_open_flag = true;
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Set clock setting of fba_%s, clock_enable=%d, clock_divider=%d\n", FBA_SAMPLE_DEVICE_SUFFIX_NAME, clock_enable, SAMPLE_CLOCK_DIVIDER);
        osal_ret = R_OSAL_PmSetPostClock(fba_dev_handle, clock_enable, SAMPLE_CLOCK_DIVIDER);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Set clock setting of fba_%s failed, osal_ret = %d\n", FBA_SAMPLE_DEVICE_SUFFIX_NAME, osal_ret);
        }
    }

    if(true == device_open_flag)
    {
        OSAL_SAMPLE_INFO("close device\n");
        osal_ret = R_OSAL_IoDeviceClose(fba_dev_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Can not close fba_%s, osal_ret = %d\n", FBA_SAMPLE_DEVICE_SUFFIX_NAME, osal_ret);
        }
    }

    /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
    (void)printf("================== set_post_clock finish ==================\n\n");
    return app_ret;
}

/**
 * @brief Device opening function for fba<br>
 * This function is enabled when SUPPORT_FBA is enabled.
 * - Get the list of available fba devices in the current system by using @link get_device_list @endlink
 * - Find the fba sample device ID from the list above
 * - Open fba device which is the specified hierarchy_name of the device ID array above by using R_OSAL_IoDeviceOpen
 *
 * @param[in] hierarchy_name Hierarchy name of fba device. This function uses this name to find full name and open it.
 * @param[in, out] device_handle This parameter is to store the opened fba device handle
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t device_open_fba(const char * hierarchy_name, osal_device_handle_t *device_handle)
{
    int32_t app_ret;
    e_osal_return_t osal_ret;
    osal_sample_device_config_t device_config;
    char* fba_device_id = NULL;

    OSAL_SAMPLE_INFO("Get device list\n");
    app_ret = get_device_list("fba", &device_config);
    if(0 != app_ret)
    {
        OSAL_SAMPLE_ERR("Get device list failed\n");
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Start finding hierarchy name\n");
        app_ret = find_fba_device_id(hierarchy_name, device_config, &fba_device_id);
        if(0 != app_ret)
        {
            OSAL_SAMPLE_ERR("Find hierarchy name failed\n");
        }
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("open device: %s\n", fba_device_id);
        /* PRQA S 2822 2 # There is no problem because the pointer and size are checked
         by confirming the value of app_ret */
        osal_ret = R_OSAL_IoDeviceOpen(fba_device_id, device_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Device opening was failed, osal_ret = %d\n", osal_ret);
        }
    }

    if(0 != app_ret)
    {
        *device_handle = OSAL_DEVICE_HANDLE_INVALID;
    }

    if(NULL != device_config.device_list)
    {
        /* PRQA S 5118 2 # Allocated in the sub-function called by this function */
        /* PRQA S 0314 1 # The 1st parameter must be (void *) and there is no side-effect in this case. */
        (void)free((void*)device_config.device_list);
    }

    return app_ret;
}

/**
 * @brief This function uses to find the fba sample device ID<br>
 * This function is enabled when SUPPORT_FBA is enabled.
 * - Split the input fba device list passed through the device config structure
 * - Select the fba sample device ID from the splitted device list based on the input hierarchy_name
 *
 * @param[in] hierarchy_name Hierarchy name of fba device
 * @param[in] device_config The information of found fba device corresponding with hierarchy_name
 * @param[in, out] fba_device_id This parameter is to store the full name of fba device corresponding with hierarchy_name
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t find_fba_device_id(const char * hierarchy_name, osal_sample_device_config_t device_config, char** fba_device_id)
{
    int32_t app_ret = 0;
    char find_str[64] = "fba_";
    size_t j = 0;
    char* splitted_devicelist[device_config.num_of_devices];

    splitted_devicelist[0] = &device_config.device_list[0];
    for (size_t i = 0; i < device_config.num_of_bytes; i++)
    {
        if(device_config.device_list[i] == '\n')
        {
            j++;
            device_config.device_list[i] = '\0';
            splitted_devicelist[j] = &device_config.device_list[i+1U];
        }
    }

    if(j > device_config.num_of_devices)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Device parsing error \n");
    }

    (void)strncat(find_str, hierarchy_name, sizeof(find_str) - strlen(find_str) - 1U);
    for (size_t i = 0; i < device_config.num_of_devices; i++)
    {
        if(0 == strncmp(splitted_devicelist[i], find_str, strlen(find_str)))
        {
            *fba_device_id = splitted_devicelist[i];
            break;
        }
    }

    return app_ret;
}
#endif
/**@} set_post_clock*/
/**@} sample device*/
