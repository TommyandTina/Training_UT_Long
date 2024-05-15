#include "r_pma_functionl_test_fw.h"

/**********************************************************************************
  Private global variable
**********************************************************************************/
st_pma_handle_t g_pma_handle;

/**********************************************************************************
  Function: exchange_ret_code_to_str
    - description:
       Exchange PMA return code to string
**********************************************************************************/
void exchange_ret_code_to_str(e_pma_return_t pma_ret)
{
    switch (pma_ret)
    {
        case R_PMA_RESULT_OK:
            TEST_LOG("pma_ret(%d) = R_PMA_RESULT_OK\n", pma_ret);
            break;
        case R_PMA_ERR_RESULT_NG:
            TEST_LOG("pma_ret(%d) = R_PMA_ERR_RESULT_NG\n", pma_ret);
            break;
        case R_PMA_ERR_ALREADY_IN_USE:
            TEST_LOG("pma_ret(%d) = R_PMA_ERR_ALREADY_IN_USE\n", pma_ret);
            break;
        case R_PMA_ERR_INVALID_HANDLE:
            TEST_LOG("pma_ret(%d) = R_PMA_ERR_INVALID_HANDLE\n", pma_ret);
            break;
        case R_PMA_ERR_NO_VACANT_HANDLE:
            TEST_LOG("pma_ret(%d) = R_PMA_ERR_NO_VACANT_HANDLE\n", pma_ret);
            break;
        case R_PMA_ERR_INVALID_ARGUMENT:
            TEST_LOG("pma_ret(%d) = R_PMA_ERR_INVALID_ARGUMENT\n", pma_ret);
            break;
        case R_PMA_ERR_DEVICE:
            TEST_LOG("pma_ret(%d) = R_PMA_ERR_DEVICE\n", pma_ret);
            break;
        case R_PMA_ERR_TIMEOUT:
            TEST_LOG("pma_ret(%d) = R_PMA_ERR_TIMEOUT\n", pma_ret);
            break;
        case R_PMA_ERR_IO:
            TEST_LOG("pma_ret(%d) = R_PMA_ERR_IO\n", pma_ret);
            break;
        case R_PMA_ERR_DEPENDENCY:
            TEST_LOG("pma_ret(%d) = R_PMA_ERR_DEPENDENCY\n", pma_ret);
            break;
        default:
            TEST_LOG("pma_ret(%d) = Unknown Error\n", pma_ret);
            break;
    }
}

/**********************************************************************************
  Function: setup_test
    - description:
       Setup for test
**********************************************************************************/
int setup_test(void)
{
    e_osal_return_t osal_ret;
    e_pma_return_t  pma_ret;
    st_pma_config_t pma_config;

    /* Configration for PMA task */
    pma_config.task_priority           = 10;
    pma_config.timeout_ms              = 1000;
    pma_config.polling_max_num         = 10;
    pma_config.loop_num_for_rclk_cycle = 10;

    TEST_LOG("Start\n");

    /* Initialize OSAL clock timer for WaitForState */
    osal_ret = R_OSAL_ClockTimeInitialize();
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_ThreadInitialize();
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_ThsyncDeinitialize();
            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = R_OSAL_MqInitialize();
            }
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Initialize PMA */
        pma_ret = R_PMA_Init(&g_pma_handle, &pma_config);
        if (R_PMA_RESULT_OK != pma_ret)
        {
            exchange_ret_code_to_str(pma_ret);
            TEST_LOG("R_PMA_Init FAILED\n");
            return TEST_NG;
        }

        TEST_LOG("Success\n");
        return TEST_OK;
    }
    else
    {
        TEST_LOG("OSAL Initialize FAILED\n");
        return TEST_NG;
    }
}

/**********************************************************************************
  Function: cleanup_test
    - description:
       Clean up for test
**********************************************************************************/
int cleanup_test(void)
{
    e_pma_return_t  pma_ret;

    TEST_LOG("Start\n");

    /* Initialize PMA */
    pma_ret = R_PMA_Quit(g_pma_handle);
    if (R_PMA_RESULT_OK != pma_ret)
    {
        exchange_ret_code_to_str(pma_ret);
        TEST_LOG(" R_PMA_Quit FAILED\n");
        return TEST_NG;
    }

    TEST_LOG("Success\n");
    return TEST_OK;
}
