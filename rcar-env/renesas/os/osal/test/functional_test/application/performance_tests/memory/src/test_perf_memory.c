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
* File Name :    test_perf_memory.c
* Version :      3.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Memory Manager Performance Test
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           3.0.0     05.06.2023    Newly create
***********************************************************************************************************************/
#include "common_header.h"

/* Global mmngr, buffer handle and mmngr configuration for testing */
static osal_memory_manager_handle_t g_mmngr_handle = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
static osal_memory_buffer_handle_t g_buffer_handle = OSAL_MEMORY_BUFFER_HANDLE_INVALID;
static st_osal_mmngr_config_t g_mmngr_config;

/* Execute the performance test for OSAL Memory APIs */
void exec_perf_memory()
{
    e_osal_return_t osal_ret;

    osal_ret = R_OSAL_MmngrGetOsalMaxConfig(&g_mmngr_config);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        /* Testing for APIs without using mmngr handle or buffer handle */
        TEST(R_OSAL_MmngrOpen, TC_23_001_001);
        TEST(R_OSAL_MmngrClose, TC_23_001_002);
        TEST(R_OSAL_MmngrGetOsalMaxConfig, TC_23_001_006);
        TEST(R_OSAL_MmngrInitializeMmngrConfigSt, TC_23_001_007);
        TEST(R_OSAL_MmngrRegisterMonitor, TC_23_001_008);
        TEST(R_OSAL_MmngrOpenFromRegionIdx, TC_23_001_020);
        TEST(R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx, TC_23_001_021);

        g_mmngr_config.mode = OSAL_MMNGR_ALLOC_MODE_STACK_PROT;
        osal_ret = R_OSAL_MmngrOpen(&g_mmngr_config, &g_mmngr_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
        else
        {   /* Testing for APIs using mmngr handle */
            TEST(R_OSAL_MmngrAlloc, TC_23_001_003);
            TEST(R_OSAL_MmngrDealloc, TC_23_001_004);
            TEST(R_OSAL_MmngrGetConfig, TC_23_001_005);
#if !defined(NDEBUG)
            TEST(R_OSAL_MmngrDebugMmngrPrint, TC_23_001_016);
#endif
            TEST(R_OSAL_MmngrInitializeMemoryManagerObj, TC_23_001_018);
            TEST(R_OSAL_MmngrInitializeMemoryBufferObj, TC_23_001_019);

            osal_ret = R_OSAL_MmngrAlloc(g_mmngr_handle, BUFFER_SIZE, BUFFER_ALIGNMENT, &g_buffer_handle);
            if(OSAL_RETURN_OK != osal_ret)
            {
                LOG_ERROR("Failure, error = %d\n", osal_ret);
            }
            else
            { /* Testing for APIs using buffer handle */
                TEST(R_OSAL_MmngrGetCpuPtr, TC_23_001_009);
                TEST(R_OSAL_MmngrGetConstCpuPtr, TC_23_001_010);
                TEST(R_OSAL_MmngrFlush, TC_23_001_011);
                TEST(R_OSAL_MmngrInvalidate, TC_23_001_012);
                TEST(R_OSAL_MmngrGetHwAddr, TC_23_001_013);
                TEST(R_OSAL_MmngrGetSize, TC_23_001_014);
                TEST(R_OSAL_MmngrCheckBuffer, TC_23_001_015);
    #if !defined(NDEBUG)
                TEST(R_OSAL_MmngrDebugBufferPrint, TC_23_001_017);
    #endif

                /* post-processing */
                osal_ret = R_OSAL_MmngrDealloc(g_mmngr_handle, g_buffer_handle);
                if(OSAL_RETURN_OK != osal_ret)
                {
                    LOG_ERROR("Failure, error = %d\n", osal_ret);
                }
            }

            osal_ret = R_OSAL_MmngrClose(g_mmngr_handle);
            if(OSAL_RETURN_OK != osal_ret)
            {
                LOG_ERROR("Failure, error = %d\n", osal_ret);
            }
        }
    }
}

/* Simple handler for testing R_OSAL_MmngrRegisterMonitor*/
void register_monitor_handler(void* const user_arg, e_osal_mmngr_event_t event,
    e_osal_return_t error, osal_memory_buffer_handle_t buffer_hndl)
{
    (void)user_arg;
    (void)error;
    (void)buffer_hndl;
    LOG_DEBUG("Register Monitor callback function");
}

/* Measure the execution time of R_OSAL_MmngrOpen */
e_osal_return_t exec_perf_MmngrOpen()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    osal_memory_manager_handle_t local_mmngr_handle;

    START_MEASURE
    osal_ret = R_OSAL_MmngrOpen(&g_mmngr_config, &local_mmngr_handle);
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
        osal_ret = R_OSAL_MmngrClose(local_mmngr_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_MmngrOpen */
int R_OSAL_MmngrOpen_TC_23_001_001_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_MmngrOpen();
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

/* Execute the performance test for R_OSAL_MmngrOpen */
void R_OSAL_MmngrOpen_TC_23_001_001(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_MmngrOpen_TC_23_001_001_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_MmngrOpen is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_MmngrClose */
e_osal_return_t exec_perf_MmngrClose()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    osal_memory_manager_handle_t local_mmngr_handle;

    osal_ret = R_OSAL_MmngrOpen(&g_mmngr_config, &local_mmngr_handle);
    if(OSAL_RETURN_OK != osal_ret)
    {

        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        START_MEASURE
        osal_ret = R_OSAL_MmngrClose(local_mmngr_handle);
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

/* Execute the performance test for R_OSAL_MmngrClose */
int R_OSAL_MmngrClose_TC_23_001_002_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_MmngrClose();
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

/* Execute the performance test for R_OSAL_MmngrClose */
void R_OSAL_MmngrClose_TC_23_001_002(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_MmngrClose_TC_23_001_002_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_MmngrClose is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_MmngrAlloc */
e_osal_return_t exec_perf_MmngrAlloc()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    osal_memory_buffer_handle_t local_buffer_handle;

    START_MEASURE
    osal_ret = R_OSAL_MmngrAlloc(g_mmngr_handle, BUFFER_SIZE, BUFFER_ALIGNMENT, &local_buffer_handle);
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
        osal_ret = R_OSAL_MmngrDealloc(g_mmngr_handle, local_buffer_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_MmngrAlloc */
int R_OSAL_MmngrAlloc_TC_23_001_003_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_MmngrAlloc();
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

/* Execute the performance test for R_OSAL_MmngrAlloc */
void R_OSAL_MmngrAlloc_TC_23_001_003(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_MmngrAlloc_TC_23_001_003_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_MmngrAlloc is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_MmngrDealloc */
e_osal_return_t exec_perf_MmngrDealloc()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    osal_memory_buffer_handle_t local_buffer_handle;

    osal_ret = R_OSAL_MmngrAlloc(g_mmngr_handle, BUFFER_SIZE, BUFFER_ALIGNMENT, &local_buffer_handle);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        START_MEASURE
        osal_ret = R_OSAL_MmngrDealloc(g_mmngr_handle, local_buffer_handle);
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

/* Execute the performance test for R_OSAL_MmngrDealloc */
int R_OSAL_MmngrDealloc_TC_23_001_004_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_MmngrDealloc();
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

/* Execute the performance test for R_OSAL_MmngrDealloc */
void R_OSAL_MmngrDealloc_TC_23_001_004(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_MmngrDealloc_TC_23_001_004_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_MmngrDealloc is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_MmngrGetConfig */
e_osal_return_t exec_perf_MmngrGetConfig()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    st_osal_mmngr_config_t config;

    START_MEASURE
    osal_ret = R_OSAL_MmngrGetConfig(g_mmngr_handle, &config);
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

/* Execute the performance test for R_OSAL_MmngrGetConfig */
int R_OSAL_MmngrGetConfig_TC_23_001_005_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_MmngrGetConfig();
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

/* Execute the performance test for R_OSAL_MmngrGetConfig */
void R_OSAL_MmngrGetConfig_TC_23_001_005(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_MmngrGetConfig_TC_23_001_005_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_MmngrGetConfig is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_MmngrGetOsalMaxConfig */
e_osal_return_t exec_perf_MmngrGetOsalMaxConfig()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    st_osal_mmngr_config_t config;

    START_MEASURE
    osal_ret = R_OSAL_MmngrGetOsalMaxConfig(&config);
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

/* Execute the performance test for R_OSAL_MmngrGetOsalMaxConfig */
int R_OSAL_MmngrGetOsalMaxConfig_TC_23_001_006_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_MmngrGetOsalMaxConfig();
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

/* Execute the performance test for R_OSAL_MmngrGetOsalMaxConfig */
void R_OSAL_MmngrGetOsalMaxConfig_TC_23_001_006(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_MmngrGetOsalMaxConfig_TC_23_001_006_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_MmngrGetOsalMaxConfig is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_MmngrInitializeMmngrConfigSt */
e_osal_return_t exec_perf_MmngrInitializeMmngrConfigSt()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    st_osal_mmngr_config_t config;

    START_MEASURE
    osal_ret = R_OSAL_MmngrInitializeMmngrConfigSt(&config);
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

/* Execute the performance test for R_OSAL_MmngrInitializeMmngrConfigSt */
int R_OSAL_MmngrInitializeMmngrConfigSt_TC_23_001_007_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_MmngrInitializeMmngrConfigSt();
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

/* Execute the performance test for R_OSAL_MmngrInitializeMmngrConfigSt */
void R_OSAL_MmngrInitializeMmngrConfigSt_TC_23_001_007(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_MmngrInitializeMmngrConfigSt_TC_23_001_007_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_MmngrInitializeMmngrConfigSt is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_MmngrRegisterMonitor */
e_osal_return_t exec_perf_MmngrRegisterMonitor()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    osal_memory_manager_handle_t local_mmngr_handle;
    int user_arg = 55;

    osal_ret = R_OSAL_MmngrOpen(&g_mmngr_config, &local_mmngr_handle);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        START_MEASURE
        osal_ret = R_OSAL_MmngrRegisterMonitor(local_mmngr_handle, OSAL_MMNGR_EVENT_ALLOCATE, register_monitor_handler, &user_arg);
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

        osal_ret = R_OSAL_MmngrClose(local_mmngr_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_MmngrRegisterMonitor */
int R_OSAL_MmngrRegisterMonitor_TC_23_001_008_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_MmngrRegisterMonitor();
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

/* Execute the performance test for R_OSAL_MmngrRegisterMonitor */
void R_OSAL_MmngrRegisterMonitor_TC_23_001_008(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_MmngrRegisterMonitor_TC_23_001_008_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_MmngrRegisterMonitor is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_MmngrGetCpuPtr */
e_osal_return_t exec_perf_MmngrGetCpuPtr()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    void* cpu_ptr;

    START_MEASURE
    osal_ret = R_OSAL_MmngrGetCpuPtr(g_buffer_handle, &cpu_ptr);
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

/* Execute the performance test for R_OSAL_MmngrGetCpuPtr */
int R_OSAL_MmngrGetCpuPtr_TC_23_001_009_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_MmngrGetCpuPtr();
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

/* Execute the performance test for R_OSAL_MmngrGetCpuPtr */
void R_OSAL_MmngrGetCpuPtr_TC_23_001_009(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_MmngrGetCpuPtr_TC_23_001_009_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_MmngrGetCpuPtr is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_MmngrGetConstCpuPtr */
e_osal_return_t exec_perf_MmngrGetConstCpuPtr()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    const void* const_cpu_ptr;

    START_MEASURE
    osal_ret = R_OSAL_MmngrGetConstCpuPtr(g_buffer_handle, &const_cpu_ptr);
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

/* Execute the performance test for R_OSAL_MmngrGetConstCpuPtr */
int R_OSAL_MmngrGetConstCpuPtr_TC_23_001_010_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_MmngrGetConstCpuPtr();
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

/* Execute the performance test for R_OSAL_MmngrGetConstCpuPtr */
void R_OSAL_MmngrGetConstCpuPtr_TC_23_001_010(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_MmngrGetConstCpuPtr_TC_23_001_010_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_MmngrGetConstCpuPtr is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_MmngrFlush */
e_osal_return_t exec_perf_MmngrFlush()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    size_t offset = 64;

    START_MEASURE
    osal_ret = R_OSAL_MmngrFlush(g_buffer_handle, offset, BUFFER_SIZE);
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

/* Execute the performance test for R_OSAL_MmngrFlush */
int R_OSAL_MmngrFlush_TC_23_001_011_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_MmngrFlush();
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

/* Execute the performance test for R_OSAL_MmngrFlush */
void R_OSAL_MmngrFlush_TC_23_001_011(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_MmngrFlush_TC_23_001_011_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_MmngrFlush is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_MmngrInvalidate */
e_osal_return_t exec_perf_MmngrInvalidate()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    size_t offset = 64;

    START_MEASURE
    osal_ret = R_OSAL_MmngrInvalidate(g_buffer_handle, offset, BUFFER_SIZE);
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

/* Execute the performance test for R_OSAL_MmngrInvalidate */
int R_OSAL_MmngrInvalidate_TC_23_001_012_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_MmngrInvalidate();
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

/* Execute the performance test for R_OSAL_MmngrInvalidate */
void R_OSAL_MmngrInvalidate_TC_23_001_012(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_MmngrInvalidate_TC_23_001_012_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_MmngrInvalidate is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_MmngrGetHwAddr */
e_osal_return_t exec_perf_MmngrGetHwAddr()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    osal_axi_bus_id_t axi_id = OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY;
    uintptr_t hw_addr;

    START_MEASURE
    osal_ret = R_OSAL_MmngrGetHwAddr(g_buffer_handle, axi_id, &hw_addr);
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

/* Execute the performance test for R_OSAL_MmngrGetHwAddr */
int R_OSAL_MmngrGetHwAddr_TC_23_001_013_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_MmngrGetHwAddr();
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

/* Execute the performance test for R_OSAL_MmngrGetHwAddr */
void R_OSAL_MmngrGetHwAddr_TC_23_001_013(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_MmngrGetHwAddr_TC_23_001_013_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_MmngrGetHwAddr is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_MmngrGetSize */
e_osal_return_t exec_perf_MmngrGetSize()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    size_t get_size;

    START_MEASURE
    osal_ret = R_OSAL_MmngrGetSize(g_buffer_handle, &get_size);
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

/* Execute the performance test for R_OSAL_MmngrGetSize */
int R_OSAL_MmngrGetSize_TC_23_001_014_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_MmngrGetSize();
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

/* Execute the performance test for R_OSAL_MmngrGetSize */
void R_OSAL_MmngrGetSize_TC_23_001_014(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_MmngrGetSize_TC_23_001_014_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_MmngrGetSize is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_MmngrCheckBuffer */
e_osal_return_t exec_perf_MmngrCheckBuffer()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;

    START_MEASURE
    osal_ret = R_OSAL_MmngrCheckBuffer(g_buffer_handle);
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

/* Execute the performance test for R_OSAL_MmngrCheckBuffer */
int R_OSAL_MmngrCheckBuffer_TC_23_001_015_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_MmngrCheckBuffer();
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

/* Execute the performance test for R_OSAL_MmngrCheckBuffer */
void R_OSAL_MmngrCheckBuffer_TC_23_001_015(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_MmngrCheckBuffer_TC_23_001_015_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_MmngrCheckBuffer is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

#if !defined(NDEBUG)
/* Measure the execution time of R_OSAL_MmngrDebugMmngrPrint */
e_osal_return_t exec_perf_MmngrDebugMmngrPrint()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;

    START_MEASURE
    osal_ret = R_OSAL_MmngrDebugMmngrPrint(g_mmngr_handle, stdout);
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

/* Execute the performance test for R_OSAL_MmngrDebugMmngrPrint */
int R_OSAL_MmngrDebugMmngrPrint_TC_23_001_016_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_MmngrDebugMmngrPrint();
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

/* Execute the performance test for R_OSAL_MmngrDebugMmngrPrint */
void R_OSAL_MmngrDebugMmngrPrint_TC_23_001_016(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_MmngrDebugMmngrPrint_TC_23_001_016_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_MmngrDebugMmngrPrint is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_MmngrDebugBufferPrint */
e_osal_return_t exec_perf_MmngrDebugBufferPrint()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;

    START_MEASURE
    osal_ret = R_OSAL_MmngrDebugBufferPrint(g_buffer_handle, stdout);
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

/* Execute the performance test for R_OSAL_MmngrDebugBufferPrint */
int R_OSAL_MmngrDebugBufferPrint_TC_23_001_017_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_MmngrDebugBufferPrint();
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

/* Execute the performance test for R_OSAL_MmngrDebugBufferPrint */
void R_OSAL_MmngrDebugBufferPrint_TC_23_001_017(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_MmngrDebugBufferPrint_TC_23_001_017_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_MmngrDebugBufferPrint is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}
#endif

/* Measure the execution time of R_OSAL_MmngrInitializeMemoryManagerObj */
e_osal_return_t exec_perf_MmngrInitializeMemoryManagerObj()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    osal_memory_manager_t mmngr_obj;

    START_MEASURE
    osal_ret = R_OSAL_MmngrInitializeMemoryManagerObj(&mmngr_obj);
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

/* Execute the performance test for R_OSAL_MmngrInitializeMemoryManagerObj */
int R_OSAL_MmngrInitializeMemoryManagerObj_TC_23_001_018_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_MmngrInitializeMemoryManagerObj();
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

/* Execute the performance test for R_OSAL_MmngrInitializeMemoryManagerObj */
void R_OSAL_MmngrInitializeMemoryManagerObj_TC_23_001_018(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_MmngrInitializeMemoryManagerObj_TC_23_001_018_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_MmngrInitializeMemoryManagerObj is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_MmngrInitializeMemoryBufferObj */
e_osal_return_t exec_perf_MmngrInitializeMemoryBufferObj()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    osal_memory_buffer_t buffer_obj;

    START_MEASURE
    osal_ret = R_OSAL_MmngrInitializeMemoryBufferObj(&buffer_obj);
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

/* Execute the performance test for R_OSAL_MmngrInitializeMemoryBufferObj */
int R_OSAL_MmngrInitializeMemoryBufferObj_TC_23_001_019_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_MmngrInitializeMemoryBufferObj();
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

/* Execute the performance test for R_OSAL_MmngrInitializeMemoryBufferObj */
void R_OSAL_MmngrInitializeMemoryBufferObj_TC_23_001_019(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_MmngrInitializeMemoryBufferObj_TC_23_001_019_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_MmngrInitializeMemoryBufferObj is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_MmngrOpenFromRegionIdx */
e_osal_return_t exec_perf_MmngrOpenFromRegionIdx()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    st_osal_mmngr_config_t config;
    osal_memory_region_idx_t region_idx = 0;
    osal_memory_manager_handle_t local_mmngr_handle;

    osal_ret = R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx(region_idx, &config);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        START_MEASURE
        osal_ret = R_OSAL_MmngrOpenFromRegionIdx(&config, region_idx, &local_mmngr_handle);
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
            osal_ret = R_OSAL_MmngrClose(local_mmngr_handle);
            if(OSAL_RETURN_OK != osal_ret)
            {
                LOG_ERROR("Failure, error = %d\n", osal_ret);
            }
        }
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_MmngrOpenFromRegionIdx */
int R_OSAL_MmngrOpenFromRegionIdx_TC_23_001_020_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_MmngrOpenFromRegionIdx();
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

/* Execute the performance test for R_OSAL_MmngrOpenFromRegionIdx */
void R_OSAL_MmngrOpenFromRegionIdx_TC_23_001_020(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_MmngrOpenFromRegionIdx_TC_23_001_020_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_MmngrOpenFromRegionIdx is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx */
e_osal_return_t exec_perf_MmngrGetOsalMaxConfigFromRegionIdx()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    st_osal_mmngr_config_t config;
    osal_memory_region_idx_t region_idx = 0;

    START_MEASURE
    osal_ret = R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx(region_idx, &config);
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

/* Execute the performance test for R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx */
int R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx_TC_23_001_021_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_MmngrGetOsalMaxConfigFromRegionIdx();
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

/* Execute the performance test for R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx */
void R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx_TC_23_001_021(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx_TC_23_001_021_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}
