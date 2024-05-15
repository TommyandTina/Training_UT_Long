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
* File Name :    test_perf_power.c
* Version :      3.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Power Manager Performance Test
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           3.0.0     04.06.2023    Newly create
***********************************************************************************************************************/
#include "common_header.h"

static osal_device_handle_t g_device_handle = OSAL_DEVICE_HANDLE_INVALID;
static osal_device_handle_t g_fba_dev_handle = OSAL_DEVICE_HANDLE_INVALID;
/* Execute the performance test for OSAL Power APIs */
void exec_perf_power()
{
    e_osal_return_t osal_ret;

    osal_ret = R_OSAL_IoDeviceOpen(TEST_PM_DEVICE, &g_device_handle);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        TEST(R_OSAL_PmSetRequiredState, TC_22_001_001);
        TEST(R_OSAL_PmSetRequiredState, TC_22_001_011);
        TEST(R_OSAL_PmGetRequiredState, TC_22_001_002);
        TEST(R_OSAL_PmWaitForState, TC_22_001_003);
        TEST(R_OSAL_PmGetState, TC_22_001_004);
        TEST(R_OSAL_PmGetLowestIntermediatePowerState, TC_22_001_005);
        TEST(R_OSAL_PmSetPolicy, TC_22_001_006);
        TEST(R_OSAL_PmSetPolicy, TC_22_001_013);
        TEST(R_OSAL_PmGetPolicy, TC_22_001_007);
        TEST(R_OSAL_PmSetResetState, TC_22_001_008);
        TEST(R_OSAL_PmGetResetState, TC_22_001_009);

        osal_ret = R_OSAL_IoDeviceClose(g_device_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
    }

#if !defined (TEST_PERF_WINDOWS) && !defined (TEST_PERF_LINUX_SIL)
    osal_ret = R_OSAL_IoDeviceOpen(TEST_FBA_DEVICE, &g_fba_dev_handle);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        TEST(R_OSAL_PmSetPostClock, TC_22_001_010);
        TEST(R_OSAL_PmSetPostClock, TC_22_001_012);
    }

    if(OSAL_DEVICE_HANDLE_INVALID != g_fba_dev_handle)
    {
        osal_ret = R_OSAL_IoDeviceClose(g_fba_dev_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
    }
#endif
}

/* Measure the execution time of R_OSAL_PmSetRequiredState */
e_osal_return_t exec_perf_PmSetRequiredState(bool applyImmediate)
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    e_osal_pm_state_t pm_state;
    e_osal_pm_reset_state_t reset_state;

    START_MEASURE
    osal_ret = R_OSAL_PmSetRequiredState(g_device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate);
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

    /* post processing */
    if (false == applyImmediate && OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_PmGetResetState(g_device_handle, &reset_state);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
        else
        {
            switch (reset_state)
            {
            case OSAL_PM_RESET_STATE_APPLIED:
                pm_state = OSAL_PM_STATE_RESET;
                break;
            case OSAL_PM_RESET_STATE_RELEASED:
                pm_state = OSAL_PM_STATE_READY;
                break;
            default:
                LOG_ERROR("Invalid state: %d\n", reset_state);
                break;
            }
        }

        if(OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_PmWaitForState(g_device_handle, pm_state, TEST_PM_TIMEOUT);
            if (OSAL_RETURN_OK != osal_ret)
            {
                LOG_ERROR("Failure, error = %d\n", osal_ret);
            }
        }
    }

    if(OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_PmSetRequiredState(g_device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_PmSetRequiredState incase applyImmediate = true in normal context */
int R_OSAL_PmSetRequiredState_TC_22_001_001_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;
    bool applyImmediate = true;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_PmSetRequiredState(applyImmediate);
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

/* Execute the performance test for R_OSAL_PmSetRequiredState in normal and interrupt context */
void R_OSAL_PmSetRequiredState_TC_22_001_001(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_PmSetRequiredState_TC_22_001_001_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_PmSetRequiredState is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_PmGetRequiredState */
e_osal_return_t exec_perf_PmGetRequiredState()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    e_osal_pm_required_state_t required_state;

    START_MEASURE
    osal_ret = R_OSAL_PmGetRequiredState(g_device_handle, &required_state);
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

/* Execute the performance test for R_OSAL_PmGetRequiredState in normal context */
int R_OSAL_PmGetRequiredState_TC_22_001_002_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_PmGetRequiredState();
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

/* Execute the performance test for R_OSAL_PmGetRequiredState in normal and interrupt context */
void R_OSAL_PmGetRequiredState_TC_22_001_002(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_PmGetRequiredState_TC_22_001_002_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_PmGetRequiredState is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_PmWaitForState */
e_osal_return_t exec_perf_PmWaitForState()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    bool applyImmediate = true;
    osal_milli_sec_t time_period = 0;

    osal_ret = R_OSAL_PmSetRequiredState(g_device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        osal_ret = R_OSAL_PmSetPolicy(g_device_handle, OSAL_PM_POLICY_HP, applyImmediate);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
        else
        {
             START_MEASURE
            osal_ret = R_OSAL_PmWaitForState(g_device_handle, OSAL_PM_STATE_ENABLED, time_period);
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

/* Execute the performance test for R_OSAL_PmWaitForState in normal context */
int R_OSAL_PmWaitForState_TC_22_001_003_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_PmWaitForState();
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

/* Execute the performance test for R_OSAL_PmWaitForState in normal and interrupt context */
void R_OSAL_PmWaitForState_TC_22_001_003(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_PmWaitForState_TC_22_001_003_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_PmWaitForState is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_PmGetState */
e_osal_return_t exec_perf_PmGetState()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    e_osal_pm_state_t pm_state;

    START_MEASURE
    osal_ret = R_OSAL_PmGetState(g_device_handle, &pm_state);
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

/* Execute the performance test for R_OSAL_PmGetState in normal context */
int R_OSAL_PmGetState_TC_22_001_004_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_PmGetState();
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

/* Execute the performance test for R_OSAL_PmGetState in normal and interrupt context */
void R_OSAL_PmGetState_TC_22_001_004(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_PmGetState_TC_22_001_004_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_PmGetState is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_PmGetLowestIntermediatePowerState */
e_osal_return_t exec_perf_PmGetLowestIntermediatePowerState()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    e_osal_pm_state_t pm_state;

    START_MEASURE
    osal_ret = R_OSAL_PmGetLowestIntermediatePowerState(g_device_handle, &pm_state);
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

/* Execute the performance test for R_OSAL_PmGetLowestIntermediatePowerState in normal context */
int R_OSAL_PmGetLowestIntermediatePowerState_TC_22_001_005_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_PmGetLowestIntermediatePowerState();
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

/* Execute the performance test for R_OSAL_PmGetLowestIntermediatePowerState in normal and interrupt context */
void R_OSAL_PmGetLowestIntermediatePowerState_TC_22_001_005(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_PmGetLowestIntermediatePowerState_TC_22_001_005_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_PmGetLowestIntermediatePowerState is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_PmSetPolicy */
e_osal_return_t exec_perf_PmSetPolicy(bool applyImmediate)
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;

    osal_ret = R_OSAL_PmSetRequiredState(g_device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        osal_ret = R_OSAL_PmSetPolicy(g_device_handle, OSAL_PM_POLICY_CG, true);
        if (OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
    }

    START_MEASURE
    osal_ret = R_OSAL_PmSetPolicy(g_device_handle, OSAL_PM_POLICY_HP, applyImmediate);
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

    /* Post-process */
    if (false == applyImmediate && OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_PmWaitForState(g_device_handle, OSAL_PM_STATE_ENABLED, TEST_PM_TIMEOUT);
        if (OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_PmSetPolicy with applyImmediate = true in normal context */
int R_OSAL_PmSetPolicy_TC_22_001_006_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;
    bool applyImmediate = true;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_PmSetPolicy(applyImmediate);
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

/* Execute the performance test for R_OSAL_PmSetPolicy in normal and interrupt context */
void R_OSAL_PmSetPolicy_TC_22_001_006(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_PmSetPolicy_TC_22_001_006_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_PmSetPolicy is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_PmGetPolicy */
e_osal_return_t exec_perf_PmGetPolicy()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    e_osal_pm_policy_t pm_policy;

    START_MEASURE
    osal_ret = R_OSAL_PmGetPolicy(g_device_handle, &pm_policy);
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

/* Execute the performance test for R_OSAL_PmGetPolicy in normal context */
int R_OSAL_PmGetPolicy_TC_22_001_007_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_PmGetPolicy();
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

/* Execute the performance test for R_OSAL_PmGetPolicy in normal and interrupt context */
void R_OSAL_PmGetPolicy_TC_22_001_007(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_PmGetPolicy_TC_22_001_007_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_PmGetPolicy is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_PmSetResetState */
e_osal_return_t exec_perf_PmSetResetState()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    bool applyImmediate = true;

    osal_ret = R_OSAL_PmSetRequiredState(g_device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, applyImmediate);
    if(OSAL_RETURN_OK != osal_ret)
    {
        LOG_ERROR("Failure, error = %d\n", osal_ret);
    }
    else
    {
        START_MEASURE
        osal_ret = R_OSAL_PmSetResetState(g_device_handle, OSAL_PM_RESET_STATE_APPLIED);
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

    /* post processing */
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_PmSetResetState(g_device_handle, OSAL_PM_RESET_STATE_RELEASED);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_PmWaitForState(g_device_handle, OSAL_PM_STATE_READY, TEST_PM_TIMEOUT);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_PmSetRequiredState(g_device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, applyImmediate);
        if(OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_PmSetResetState in normal context */
int R_OSAL_PmSetResetState_TC_22_001_008_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_PmSetResetState();
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

/* Execute the performance test for R_OSAL_PmSetResetState in normal and interrupt context */
void R_OSAL_PmSetResetState_TC_22_001_008(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_PmSetResetState_TC_22_001_008_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_PmSetResetState is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Measure the execution time of R_OSAL_PmGetResetState */
e_osal_return_t exec_perf_PmGetResetState()
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    bool applyImmediate = true;
    e_osal_pm_reset_state_t reset_state;

    START_MEASURE
    osal_ret = R_OSAL_PmGetResetState(g_device_handle, &reset_state);
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

/* Execute the performance test for R_OSAL_PmGetResetState in normal context */
int R_OSAL_PmGetResetState_TC_22_001_009_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_PmGetResetState();
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

/* Execute the performance test for R_OSAL_PmGetResetState in normal and interrupt context */
void R_OSAL_PmGetResetState_TC_22_001_009(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_PmGetResetState_TC_22_001_009_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_PmGetResetState is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

#if !defined (TEST_PERF_WINDOWS) && !defined (TEST_PERF_LINUX_SIL)
/* Measure the execution time of R_OSAL_PmSetPostClock */
e_osal_return_t exec_perf_PmSetPostClock(bool clock_enable)
{
    e_osal_return_t osal_ret;
    uint64_t exec_time;
    uint32_t clock_divider = 0xB;

    START_MEASURE
    osal_ret = R_OSAL_PmSetPostClock(g_fba_dev_handle, clock_enable, clock_divider);
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

    /* post-process */
    switch (clock_enable)
    {
    case true:
        osal_ret = R_OSAL_PmSetPostClock(g_fba_dev_handle, false, clock_divider);
        if (OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
        break;
    default: /* false */
        osal_ret = R_OSAL_PmSetPostClock(g_fba_dev_handle, true, clock_divider);
        if (OSAL_RETURN_OK != osal_ret)
        {
            LOG_ERROR("Failure, error = %d\n", osal_ret);
        }
        break;
    }

    return osal_ret;
}

/* Execute the performance test for R_OSAL_PmSetPostClock incase clock_enable = true in normal context */
int R_OSAL_PmSetPostClock_TC_22_001_010_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;
    bool clock_enable = true;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_PmSetPostClock(clock_enable);
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

/* Execute the performance test for R_OSAL_PmSetPostClock incase clock_enable = true in normal and interrupt context */
void R_OSAL_PmSetPostClock_TC_22_001_010(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_PmSetPostClock_TC_22_001_010_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_PmSetPostClock is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Execute the performance test for R_OSAL_PmSetPostClock incase clock_enable = false in normal context */
int R_OSAL_PmSetPostClock_TC_22_001_012_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;
    bool clock_enable = false;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_PmSetPostClock(clock_enable);
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

/* Execute the performance test for R_OSAL_PmSetPostClock incase clock_enable = false in normal and interrupt context */
void R_OSAL_PmSetPostClock_TC_22_001_012(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_PmSetPostClock_TC_22_001_012_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_PmSetPostClock is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}
#endif

/* Execute the performance test for R_OSAL_PmSetRequiredState incase applyImmediate = false in normal context */
int R_OSAL_PmSetRequiredState_TC_22_001_011_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;
    bool applyImmediate = false;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_PmSetRequiredState(applyImmediate);
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

/* Execute the performance test for R_OSAL_PmSetRequiredState with applyImmediate = false in normal and interrupt context */
void R_OSAL_PmSetRequiredState_TC_22_001_011(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_PmSetRequiredState_TC_22_001_011_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_PmSetRequiredState is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}

/* Execute the performance test for R_OSAL_PmSetPolicy with applyImmediate = false in normal context */
int R_OSAL_PmSetPolicy_TC_22_001_013_NOR(const char* func, uint64_t expected_time)
{
    e_osal_return_t osal_ret;
    size_t loop;
    int ret = 0;
    bool applyImmediate = false;

    /* Normal context */
    for(loop = 0; loop < NUM_OF_MEASURE_TIME; loop++)
    {
        osal_ret = exec_perf_PmSetPolicy(applyImmediate);
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

/* Execute the performance test for R_OSAL_PmSetPolicy with applyImmediate = false in normal and interrupt context */
void R_OSAL_PmSetPolicy_TC_22_001_013(uint64_t expected_time)
{
    int ret_nor;
    char api_name[100];
    char test_num[100];

    extract_test_info(__func__, api_name, test_num);
    LOG("\n===== Start measuring the execution time of %s =====\n", api_name);
    LOG("[ RUN      ] %s\n", __func__);

    /* Normal context */
    ret_nor = R_OSAL_PmSetPolicy_TC_22_001_013_NOR(api_name, expected_time);

    /* ISR context */
    /* R_OSAL_PmSetPolicy is not allowed to call in ISR */

    /* show test result: pass/failed */
    test_judgement(0 == ret_nor, api_name, test_num);
}
