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
* File Name :    test_perf_common.c
* Version :      3.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Common API Performance Test
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           3.0.0     08.06.2023    Newly create
***********************************************************************************************************************/
#include "common_header.h"

/* Execute the performance test for OSAL Common APIs */
void exec_perf_common_api()
{
    TEST(R_OSAL_Initialize, TC_24_001_001);
    TEST(R_OSAL_Deinitialize, TC_24_001_002);
    TEST(R_OSAL_GetVersion, TC_24_001_003);
}

/* Measure the execution time of R_OSAL_Initialize */
e_osal_return_t exec_perf_Initialize()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;

    START_MEASURE
    osal_ret = R_OSAL_Initialize();
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
        osal_ret = R_OSAL_Deinitialize();
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_Initialize in normal context */
int R_OSAL_Initialize_TC_24_001_001_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_Initialize();
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

/* Execute the performance test for R_OSAL_Initialize */
void R_OSAL_Initialize_TC_24_001_001(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_Initialize_TC_24_001_001_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_Initialize is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_Deinitialize */
e_osal_return_t exec_perf_Deinitialize()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;

    osal_ret = R_OSAL_Initialize();
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        START_MEASURE
        osal_ret = R_OSAL_Deinitialize();
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

/* Execute the performance test for R_OSAL_Deinitialize in normal context */
int R_OSAL_Deinitialize_TC_24_001_002_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_Deinitialize();
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

/* Execute the performance test for R_OSAL_Deinitialize */
void R_OSAL_Deinitialize_TC_24_001_002(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_Deinitialize_TC_24_001_002_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_Deinitialize is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_GetVersion */
st_osal_version_t * exec_perf_GetVersion()
{
    st_osal_version_t * osal_version;
    uint64_t exec_time;

    START_MEASURE
    osal_version = R_OSAL_GetVersion();
    STOP_MEASURE
    exec_time = get_exec_time();
    push_back(exec_time);
    LOG_DEBUG("Execution time = %llu (ns)\n", exec_time);

    return osal_version;
}

/* Execute the performance test for R_OSAL_GetVersion in normal context */
int R_OSAL_GetVersion_TC_24_001_003_NOR(const char* func, uint64_t expected_time)
{
    st_osal_version_t * osal_version;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_version = exec_perf_GetVersion();
    }

    if (0 == ret)
    {
        ret = get_test_info(func, expected_time, "Normal Context");
    }

    return ret;
}

/* Execute the performance test for R_OSAL_GetVersion */
void R_OSAL_GetVersion_TC_24_001_003(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_GetVersion_TC_24_001_003_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_GetVersion is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}
