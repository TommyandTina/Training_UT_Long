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
* File Name :    common_func.c
* Version :      3.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Common method of OSAL Performance Test
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           3.0.0     18.04.2023    Newly create
***********************************************************************************************************************/
#include "common_header.h"

/* Global parameter */
uint64_t g_diff_time;
static uint64_t* g_exec_time_list = NULL;
static size_t g_exec_time_list_size = 0;
#if defined(OSAL_PERF_TEST_S4)
uint16_t init_rdmor_data;
uint32_t init_data[5];
#else
uint32_t init_data[3];
#endif
uint32_t init_clear_data;

/* Get the exection time of target API */
uint64_t get_exec_time()
{
    return g_diff_time;
}

/* Calculate the average value of exection times */
uint64_t cal_average()
{
    uint64_t sum = 0;

    for (size_t i = 0; i < g_exec_time_list_size; i++)
    {
        sum += g_exec_time_list[i];
    }

    return (sum / g_exec_time_list_size);
}

/* Find the maximum value in exection time list */
uint64_t find_max()
{
    uint64_t max = 0;

    for (size_t i = 0; i < g_exec_time_list_size; i++)
    {
        if (max < g_exec_time_list[i])
        {
            max = g_exec_time_list[i];
        }
    }

    return max;
}

/* Find the minimum value in exection time list */
uint64_t find_min()
{
    uint64_t min = ULONG_MAX;

    for (size_t i = 0; i < g_exec_time_list_size; i++)
    {
        if (min > g_exec_time_list[i])
        {
            min = g_exec_time_list[i];
        }
    }

    return min;
}

/* Push an execution time value to global list g_exec_time_list */
void push_back(uint64_t value)
{
    
    uint64_t* tmp_list = (uint64_t*)realloc((void*)g_exec_time_list, (g_exec_time_list_size + 1U) * sizeof(uint64_t));
    if(NULL == tmp_list)
    {
        LOG_ERROR("time list expanding failed\n");
    }
    else
    {
        g_exec_time_list = tmp_list;
        g_exec_time_list[g_exec_time_list_size] = value;
        g_exec_time_list_size++;
    }
}

/* Get the average and maximum execution time and dump them to the console.
 * Do the comparison between avarage time and expected time.
 * And then clean g_exec_time_list for the next test case.
 */
int get_test_info(const char* func, uint64_t expected_time, const char* context)
{
    uint64_t average_val;
    uint64_t max_val;
    uint64_t min_val;
    int ret = 0;

    average_val = cal_average();
    max_val = find_max();
    min_val = find_min();

    LOG("[%s] %s: Maximum: %llu, Minimum: %llu, Average: %llu, Expectation: %llu\n", context, func, max_val, min_val, average_val, expected_time);

    if (expected_time < average_val)
    {
        LOG("%s has exceeded the expected execution time. \n", func);
        ret = -1;
    }

    if (NULL != g_exec_time_list)
    {
        free(g_exec_time_list);
        g_exec_time_list = NULL;
        g_exec_time_list_size = 0;
    }

    return ret;
}

/* Judge the test case OK or FAILED */
void test_judgement(bool ret, const char* func, const char* num)
{
    char tmp_str[100];

    connect_str(func, num, tmp_str);
    if (true == ret)
    {
        LOG("[       OK ] %s\n", tmp_str);
    }
    else
    {
        LOG("[  FAILED  ] %s\n", tmp_str);
    }
}

/* Connect 2 strings with the "_"
 * Example: str1 = "abc", str2 = "def" --> output = "abc_def"
 */
void connect_str(const char* str1, const char* str2, char* output)
{
    strcpy(output, str1);
    strcat(output, "_");
    strcat(output, str2);
}

/* Get a sub-string from an input string based on position, length. The behavior is the same as std::string::substr().
 * Example, pos = 1; len = 3; input_str = "abcdef" --> output_str = "bcd"
 */
void get_substr(size_t pos, size_t len, const char* input_str, char* output_str)
{
    if (0U == len) // from pos to the end
    {
        len = strlen(input_str) - pos;
    }

    strncpy(output_str, input_str + pos, len);
    output_str[len] = '\0';
}

/* Find and return the first position of a string in another string. The behavior is the same as std::string::find()
 * Example, hint = "cde"; str = "abcdef" --> position = 2
 */
size_t find_str(const char* hint, const char* str)
{
    char* found_str;
    size_t pos_str = 0;

    found_str = strstr(str, hint);
    if (NULL == found_str)
    {
        LOG_ERROR("Test case's name format is not correct\n");
    }
    else
    {
        pos_str = found_str - str;
    }

    return pos_str;
}

/* Get the target api name and test case number from test function.
 * Example, test function = "R_OSAL_ThreadCreate_TC_15_001_001" --> api_name = "R_OSAL_ThreadCreate", test_num = "TC_15_001_001"
 */
void extract_test_info(const char* str, char* api_name, char* test_num)
{
    size_t pos;

    pos = find_str("_TC_", str);
    get_substr(0, pos, str, api_name);
    get_substr(pos + 1, 0, str, test_num);
}

/* Open new memory manager handle */
e_osal_return_t open_mem(osal_memory_manager_handle_t* p_mmngr_handle)
{
    e_osal_return_t osal_ret;
    st_osal_mmngr_config_t config;
    osal_memory_manager_handle_t local_hndl_mmngr;

    osal_ret = R_OSAL_MmngrGetOsalMaxConfig(&config);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        osal_ret = R_OSAL_MmngrOpen(&config, &local_hndl_mmngr);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
            local_hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
        }
    }

    *p_mmngr_handle = local_hndl_mmngr;

    return osal_ret;
}

/* Prepare the work buffers which are used for generating interrupt from HW */
e_osal_return_t prepare_work_buffer(osal_memory_manager_handle_t mmngr_hndl, uint32_t *p_hw_addr, osal_memory_buffer_handle_t* p_buffer_obj)
{
    e_osal_return_t osal_ret;
    osal_memory_buffer_handle_t local_buffer_obj;
    uint32_t data;
    void *cpu_ptr[NUM_OF_BUFFERS];
    osal_axi_bus_id_t *axi_id = NULL;
    uintptr_t local_hw_addr = 0;
    st_osal_mmngr_config_t config;
    size_t num_of_max_entry = 0;
    size_t num_of_written_axi_ids = 0;

    for(size_t i = 0; i < NUM_OF_BUFFERS; i++)
    {
        osal_ret = R_OSAL_MmngrAlloc(mmngr_hndl, BUFFER_SIZE, BUFFER_ALIGNMENT, &local_buffer_obj);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
            break;
        }
        p_buffer_obj[i] = local_buffer_obj;

        /* Get HW address of buffer */
        osal_ret = R_OSAL_IoGetNumOfAxiIdsForDeviceName(DEV_GEN_ISR_1, &num_of_max_entry);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
            break;
        }

        axi_id = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);
        if(NULL == axi_id)
        {
            LOG_ERROR("Malloc failed\n");
            osal_ret = OSAL_RETURN_FAIL;
            break;
        }

        osal_ret = R_OSAL_IoGetAxiIdListForDeviceName(DEV_GEN_ISR_1, axi_id, num_of_max_entry, &num_of_written_axi_ids);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
            break;
        }

        osal_ret = R_OSAL_MmngrGetHwAddr(local_buffer_obj, axi_id[0], &local_hw_addr);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
            break;
        }
        p_hw_addr[i] = (uint32_t)local_hw_addr;

        /* Get CPU accessible pointer of the buffer */
        cpu_ptr[i] = NULL;
        osal_ret = R_OSAL_MmngrGetCpuPtr(local_buffer_obj, &cpu_ptr[i]);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
            break;
        }

        /* Write display list data to the beginning of the buffer */
        data = DUMMY_BUFFER_DATA[i];
        (void)memcpy(cpu_ptr[i], (void*)&data, sizeof(uint32_t));
        osal_ret = R_OSAL_MmngrFlush(local_buffer_obj, 0 , BUFFER_SIZE);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
            break;
        }

        free(axi_id);
        axi_id = NULL;
    }

    return osal_ret;
}

/* Deallocate work buffers and close memory manager handle */
e_osal_return_t release_mem(osal_memory_manager_handle_t hndl_mmngr, osal_memory_buffer_handle_t* buffer_obj)
{
    e_osal_return_t osal_ret;

    for(int i = NUM_OF_BUFFERS - 1; i >= 0; i--)
    {
        osal_ret = R_OSAL_MmngrDealloc(hndl_mmngr, buffer_obj[i]);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
            break;
        }
    }

    if(OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_MmngrClose(hndl_mmngr);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
    }

    return osal_ret;
}

/* Setup required HW register in order to generate interrupt */
e_osal_return_t setup_hw_register(osal_device_handle_t* dev, uint32_t* hw_addr)
{
    e_osal_return_t osal_ret;

#if defined(OSAL_PERF_TEST_S4)
    /* Config DMA operation register */
    osal_ret = R_OSAL_IoWrite16(dev[1], REG_RDMOR, TRANSFER_RDMOR_VALUE);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit;
    }

    /* Config transfer src address */
    osal_ret = R_OSAL_IoWrite32(dev[0], REG_RDMSAR, hw_addr[0]);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit;
    }

    /* Config transfer dst address */
    osal_ret = R_OSAL_IoWrite32(dev[0], REG_RDMDAR, hw_addr[1]);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit;
    }

    /* Config transfer count */
    osal_ret = R_OSAL_IoWrite32(dev[0], REG_RDMTCR, TRANSFER_COUNT_VALUE);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit;
    }

    /* Config transfer steps */
    osal_ret = R_OSAL_IoWrite32(dev[0], REG_RDMCHCRB, TRANSFER_STEP_VALUE);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit;
    }

    /* Config DMA channel control register */
    osal_ret = R_OSAL_IoWrite32(dev[0], REG_RDMCHCR, TRANSFER_RDMCHCR_VALUE);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
#else
    osal_ret = R_OSAL_IoWrite32(dev[0], IMR_REG_DLSAR, hw_addr[0]);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit;
    }

    osal_ret = R_OSAL_IoWrite32(dev[0], IMR_REG_IMR, IMR_IMR_VALUE);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit;
    }

    osal_ret = R_OSAL_IoWrite32(dev[0], IMR_REG_CR, IMR_CR_VALUE);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
#endif

    exit:
    return osal_ret;
}

/* Open new device handles which are used for generating interrupt */
e_osal_return_t open_devices(osal_device_handle_t* dev)
{
    e_osal_return_t osal_ret;

#if defined(OSAL_PERF_TEST_S4)
    osal_ret = R_OSAL_IoDeviceOpen(DEV_GEN_ISR_2, &dev[1]);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit;
    }

    osal_ret = R_OSAL_IoDeviceOpen(DEV_GEN_ISR_1, &dev[0]);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit;
    }

    osal_ret = R_OSAL_PmSetRequiredState(dev[1], OSAL_PM_REQUIRED_STATE_REQUIRED, true);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
#else
    osal_ret = R_OSAL_IoDeviceOpen(DEV_GEN_ISR_1, &dev[0]);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit;
    }

    osal_ret = R_OSAL_PmSetRequiredState(dev[0], OSAL_PM_REQUIRED_STATE_REQUIRED, true);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
#endif

    exit:
    return osal_ret;
}

/* Close device handles which are used for generating interrupt */
e_osal_return_t release_devices(osal_device_handle_t* dev)
{
    e_osal_return_t osal_ret;

    /* Close test target device */
#if defined(OSAL_PERF_TEST_S4)
    osal_ret = R_OSAL_PmSetRequiredState(dev[1], OSAL_PM_REQUIRED_STATE_RELEASED, true);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }

    osal_ret = R_OSAL_IoDeviceClose(dev[0]);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }

    osal_ret = R_OSAL_IoDeviceClose(dev[1]);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
#else
    osal_ret = R_OSAL_PmSetRequiredState(dev[0], OSAL_PM_REQUIRED_STATE_RELEASED, true);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }

    osal_ret = R_OSAL_IoDeviceClose(dev[0]);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
#endif

    return osal_ret;
}

/* Store the initialized value of register devices which are used for generating interrupt */
e_osal_return_t store_reg_init_value(osal_device_handle_t* dev)
{
    e_osal_return_t osal_ret;

#if defined(OSAL_PERF_TEST_S4)
    osal_ret = R_OSAL_IoRead32(dev[0], REG_CLEAR_REGISTER, &init_clear_data);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit;
    }

    osal_ret = R_OSAL_IoRead16(dev[1], REG_RDMOR, &init_rdmor_data);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit;
    }

    osal_ret = R_OSAL_IoRead32(dev[0], REG_RDMSAR, &init_data[0]);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit;
    }

    osal_ret = R_OSAL_IoRead32(dev[0], REG_RDMDAR, &init_data[1]);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit;
    }

    osal_ret = R_OSAL_IoRead32(dev[0], REG_RDMTCR, &init_data[2]);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit;
    }

    osal_ret = R_OSAL_IoRead32(dev[0], REG_RDMCHCRB, &init_data[3]);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit;
    }

    osal_ret = R_OSAL_IoRead32(dev[0], REG_RDMCHCR, &init_data[4]);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
#else
    osal_ret = R_OSAL_IoRead32(dev[0], REG_CLEAR_REGISTER, &init_clear_data);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit;
    }

    osal_ret = R_OSAL_IoRead32(dev[0], IMR_REG_DLSAR, &init_data[0]);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit;
    }

    osal_ret = R_OSAL_IoRead32(dev[0], IMR_REG_IMR, &init_data[1]);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit;
    }

    osal_ret = R_OSAL_IoRead32(dev[0], IMR_REG_CR, &init_data[2]);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
#endif

    exit:
    return osal_ret;
}

/* Re-store the initialized value of register devices which are used for generating interrupt */
e_osal_return_t restore_reg_init_value(osal_device_handle_t* dev)
{
    e_osal_return_t osal_ret;

#if defined(OSAL_PERF_TEST_S4)
    osal_ret = R_OSAL_IoWrite32(dev[0], REG_CLEAR_REGISTER, init_clear_data);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit;
    }

    osal_ret = R_OSAL_IoWrite16(dev[1], REG_RDMOR, init_rdmor_data);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit;
    }

    osal_ret = R_OSAL_IoWrite32(dev[0], REG_RDMSAR, init_data[0]);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit;
    }

    osal_ret = R_OSAL_IoWrite32(dev[0], REG_RDMDAR, init_data[1]);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit;
    }

    osal_ret = R_OSAL_IoWrite32(dev[0], REG_RDMTCR, init_data[2]);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit;
    }

    osal_ret = R_OSAL_IoWrite32(dev[0], REG_RDMCHCRB, init_data[3]);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit;
    }

    osal_ret = R_OSAL_IoWrite32(dev[0], REG_RDMCHCR, init_data[4]);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
#else
    osal_ret = R_OSAL_IoWrite32(dev[0], REG_CLEAR_REGISTER, init_clear_data);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit;
    }

    osal_ret = R_OSAL_IoWrite32(dev[0], IMR_REG_DLSAR, init_data[0]);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit;
    }

    osal_ret = R_OSAL_IoWrite32(dev[0], IMR_REG_IMR, init_data[1]);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit;
    }

    osal_ret = R_OSAL_IoWrite32(dev[0], IMR_REG_CR, init_data[2]);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
#endif

    exit:
    return osal_ret;
}

/* Create new MQ which is used for synchoronize between main thread and the callback function */
e_osal_return_t create_sync_method(osal_mq_handle_t* p_mq_hndl)
{
    e_osal_return_t osal_ret;
    st_osal_mq_config_t mq_config = {2, 4};

    osal_ret = R_OSAL_MqCreate(&mq_config, SYNC_MQ_ID, p_mq_hndl);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }

    return osal_ret;
}

/* Destroy MQ which is used for synchoronize between main thread and the callback function */
e_osal_return_t destroy_sync_method(osal_mq_handle_t mq_hndl)
{
    e_osal_return_t osal_ret;

    osal_ret = R_OSAL_MqDelete(mq_hndl);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }

    return osal_ret;
}

/* The sequence call to handle (generate/clean) the interrupt from HW */
int generate_interrupt(p_osal_isr_func_t user_callback, const char* api_name)
{
    e_osal_return_t osal_ret;
    int ret = 0;
    osal_memory_manager_handle_t mmngr_hndl;
    osal_memory_buffer_handle_t buffer_obj[NUM_OF_BUFFERS];
    uint32_t hw_addr[NUM_OF_BUFFERS];
    osal_device_handle_t dev_arr[2];
    osal_mq_handle_t sync_mq;
    int32_t receive_msg = 0;
    isr_user_data_t user_arg;

    osal_ret = open_mem(&mmngr_hndl);
    if(OSAL_RETURN_OK != osal_ret)
    {
        ret = -1;
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit_5;
    }

    osal_ret = open_devices(dev_arr);
    if(OSAL_RETURN_OK != osal_ret)
    {
        ret = -1;
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit_4;
    }

    osal_ret = store_reg_init_value(dev_arr);
    if(OSAL_RETURN_OK != osal_ret)
    {
        ret = -1;
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit_3;
    }

    osal_ret = create_sync_method(&sync_mq);
    if(OSAL_RETURN_OK != osal_ret)
    {
        ret = -1;
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit_2;
    }

    osal_ret = prepare_work_buffer(mmngr_hndl, hw_addr, buffer_obj);
    if(OSAL_RETURN_OK != osal_ret)
    {
        ret = -1;
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit_1;
    }

    user_arg.sync_mq = sync_mq;
    strcpy(user_arg.api_name, api_name);
    osal_ret = R_OSAL_InterruptRegisterIsr(dev_arr[0], 0, OSAL_INTERRUPT_PRIORITY_TYPE0, user_callback, (void *)&user_arg);
    if(OSAL_RETURN_OK != osal_ret)
    {
        ret = -1;
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit_1;
    }

    osal_ret = R_OSAL_InterruptEnableIsr(dev_arr[0], 0);
    if(OSAL_RETURN_OK != osal_ret)
    {
        ret = -1;
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit_1;
    }

    osal_ret = setup_hw_register(dev_arr, hw_addr);
    if(OSAL_RETURN_OK != osal_ret)
    {
        ret = -1;
        LOG_ERROR("Failure, error = %d\n", osal_ret);
        goto exit_1;
    }

    osal_ret = R_OSAL_MqReceiveForTimePeriod(sync_mq, MQ_TIMEOUT, &receive_msg, sizeof(SEND_MSG));
    if(OSAL_RETURN_OK != osal_ret)
    {
        ret = -1;
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }

    osal_ret = R_OSAL_InterruptUnregisterIsr(dev_arr[0], 0, user_callback);
    if(OSAL_RETURN_OK != osal_ret)
    {
        ret = -1;
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }

    exit_1:
    osal_ret = destroy_sync_method(sync_mq);
    if(OSAL_RETURN_OK != osal_ret)
    {
        ret = -1;
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }

    exit_2:
    osal_ret = restore_reg_init_value(dev_arr);
    if(OSAL_RETURN_OK != osal_ret)
    {
        ret = -1;
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }

    exit_3:
    osal_ret = release_devices(dev_arr);
    if(OSAL_RETURN_OK != osal_ret)
    {
        ret = -1;
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }

    exit_4:
    osal_ret = release_mem(mmngr_hndl, buffer_obj);
    if(OSAL_RETURN_OK != osal_ret)
    {
        ret = -1;
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }

    exit_5:
    return ret;
}
