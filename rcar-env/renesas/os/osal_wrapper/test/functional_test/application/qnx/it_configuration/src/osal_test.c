
/* include */
#include "osal_test.h"
#include "test_load_data.h"

int osal_test(int parent_number, int child_number)
{
    printf("\n\n");

    /* switch test case at manager number */
    switch (parent_number)
    {
        case OSAL_TEST_CONFIG:
        case OSAL_TEST_ALL:
            osal_test_configuration_menu(OSAL_TEST_ALL_EXEC);
        break;
        case OSAL_TEST_EXIT:
            printf("Quit.\n\n");
            return OSAL_TEST_OK;
        default:
            printf("That menu is not available.\n\n");
            break;
    }

    return OSAL_TEST_OK;
}

/* Configuration table check */
static void osal_test_config_check(void);

static void osal_test_result_output(int parent, int api_id, int result, char *param) 
{
    if (param == NULL)
    {
        if (result == OSAL_TEST_OK)
        {
            printf("test case %02d_%03d result OK.\n\n", parent, api_id);
        }
        else
        {
            printf("test case %02d_%03d result NG.\n\n", parent, api_id);
        }
    }
    else
    {
        if (result == OSAL_TEST_OK)
        {
            printf("test case %02d_%03d result OK. %s\n\n", parent, api_id, param);
            test_pass++;
        } 
        else 
        {
            printf("test case %02d_%03d result NG. %s\n\n", parent, api_id, param);
            test_fail++;
        }
    }
}

static char *osal_ret_to_str(e_osal_return_t ret_val)
{
    static char *str;
    
    switch(ret_val)
    {
        case OSAL_RETURN_OK:
            str = "OSAL_RETURN_OK";
            break;

        case OSAL_RETURN_PAR:
            str = "OSAL_RETURN_PAR";
            break;

        case OSAL_RETURN_MEM:
            str = "OSAL_RETURN_MEM";
            break;

        case OSAL_RETURN_DEV:
            str = "OSAL_RETURN_DEV";
            break;

        case OSAL_RETURN_TIME:
            str = "OSAL_RETURN_TIME";
            break;

        case OSAL_RETURN_CONF:
            str = "OSAL_RETURN_CONF";
            break;

        case OSAL_RETURN_FAIL:
            str = "OSAL_RETURN_FAIL";
            break;

        case OSAL_RETURN_IO:
            str = "OSAL_RETURN_IO";
            break;

        case OSAL_RETURN_BUSY:
            str = "OSAL_RETURN_BUSY";
            break;

        case OSAL_RETURN_ID:
            str = "OSAL_RETURN_ID";
            break;

        case OSAL_RETURN_HANDLE:
            str = "OSAL_RETURN_HANDLE";
            break;

        case OSAL_RETURN_STATE:
            str = "OSAL_RETURN_STATE";
            break;

        case OSAL_RETURN_PROHIBITED:
            str = "OSAL_RETURN_PROHIBITED";
            break;

        case OSAL_RETURN_UNSUPPORTED_OPERATION:
            str = "OSAL_RETURN_UNSUPPORTED_OPERATION";
            break;

        case OSAL_RETURN_HANDLE_NOT_SUPPORTED:
            str = "OSAL_RETURN_HANDLE_NOT_SUPPORTED";
            break;

        case OSAL_RETURN_UNKNOWN_NAME:
            str = "OSAL_RETURN_UNKNOWN_NAME";
            break;

        case OSAL_RETURN_CONF_UNSUPPORTED:
            str = "OSAL_RETURN_CONF_UNSUPPORTED";
            break;

        case OSAL_RETURN_OK_RQST:
            str = "OSAL_RETURN_OK_RQST";
            break;

        default:
            str = "UNKNOW ERROR";
            break;

    }

    return str;
}

void osal_test_configuration_display_menu(void)
{
    printf("*** Configuration table check ***\n");

    printf("***  10_001-008: Thread Configuration Check                     ***\n");
    printf("***  10_009-011: Mutex Configuration Check                      ***\n");
    printf("***  10_012-014: Condition Configuration Check                  ***\n");
    printf("***  10_015-019: Message Queue Configuration Check              ***\n");
    printf("***  10_020-034: Interrupt Thread Configuration Check           ***\n");
    printf("***  10_035-068: Memory Configuration Check                     ***\n");
    printf("***  10_069-097: Thread Setting Check                           ***\n");
    printf("***  10_098-099: Memory Configuration Check2                    ***\n");

    printf("***  10_100: configration table module all exec                 ***\n\n");
}

void osal_test_configuration_menu(int child)
{
    int test_number = child;
    unsigned int test_count  = 1;
    unsigned int loop_count  = 0;

    /* ALL check */
    /* when select all */
    if (child == OSAL_TEST_ALL_EXEC)
    {
        test_number = OSAL_TEST_CONFIG_CHECK;
        test_count  = OSAL_TEST_CONFIG_MAX;
    }

    for (loop_count = 0; loop_count < test_count; loop_count++)
    {
        switch (test_number)
        {
            case OSAL_TEST_CONFIG_CHECK:
                osal_test_config_check();
                break;

            default:
                printf("That menu is not available.\n\n");
                break;
        }
        test_number++;
    }
}

/* test thread */
int64_t osal_test_thread_sample(void *user_arg)
{
    (void)user_arg;

    /* wait until the main task calls R_OSAL_ThreadJoin() */
    R_OSAL_ThreadSleepForTimePeriod(2500);

    return 0;
}

static void osal_test_cb(osal_device_handle_t device_handle, uint64_t irq_channel, void *irq_arg)
{
    return;
}

extern int32_t g_config_check;
/* ------------------------ 10_001-008 [Thread Configuration Check] ------------------------ */
#if defined (CHECK_THREAD)
extern int32_t             g_expected_initialize;
extern int32_t             g_expected_thread_create;
extern osal_thread_id_t    g_thread_id;
static void osal_test_config_check(void)
{
    int32_t                    test_ret                              = OSAL_TEST_OK;
    char                       opt_info[OSAL_TEST_OPT_INFO_BUFFER];
    e_osal_return_t            osal_ret;
    st_osal_thread_config_t    config;
    int64_t                    arg                                   = 10;
    osal_thread_handle_t       thread_handle;
    int64_t                    *p_return_value;

    config.func       =      osal_test_thread_sample;
    config.userarg    =      (void *)&arg;
    config.priority   =      OSAL_THREAD_PRIORITY_TYPE10;
    config.task_name  =      "task_01_001_001";
    config.stack_size =      0x1000;

    /* R_OSAL_Initialize */
    osal_ret = R_OSAL_Initialize();
    if (g_expected_initialize != osal_ret)
    {
        test_ret = OSAL_TEST_NG;
    }
    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_initialize), osal_ret_to_str(osal_ret));
    printf("Check R_OSAL_Initialize result\n");
    osal_test_result_output(OSAL_TEST_CONFIG,
                            g_config_check,
                            test_ret, opt_info);

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (OSAL_TEST_OK == test_ret)
        {
            /* R_OSAL_ThreadCreate */
            osal_ret = R_OSAL_ThreadCreate(&config, g_thread_id, &thread_handle);
            if (g_expected_thread_create != osal_ret)
            {
                test_ret = OSAL_TEST_NG;
            }
            OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_thread_create), osal_ret_to_str(osal_ret));
            printf("Check R_OSAL_ThreadCreate result\n");
            osal_test_result_output(OSAL_TEST_CONFIG,
                                    g_config_check,
                                    test_ret, opt_info);

            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = R_OSAL_ThreadJoin(thread_handle, &p_return_value);
                if (osal_ret != OSAL_RETURN_OK)
                {
                    OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.", OSAL_TEST_CONFIG, 0);
                }
            }
        }
        osal_ret = R_OSAL_Deinitialize();
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", OSAL_TEST_CONFIG, 0);
        }
    }

    return;
}
/* ---------------------------------------------------------------------------------------- */

/* ------------------------ 10_009-011 [Mutex Configuration Check] ------------------------ */
#elif (CHECK_MUTEX)
extern int32_t         g_expected_initialize;
extern int32_t         g_expected_mutex_create;
extern osal_mutex_id_t g_mutex_id;
static void osal_test_config_check(void)
{
    int32_t                test_ret                              = OSAL_TEST_OK;
    char                   opt_info[OSAL_TEST_OPT_INFO_BUFFER];
    e_osal_return_t        osal_ret;
    osal_mutex_handle_t    handle;

    /* R_OSAL_Initialize */
    osal_ret = R_OSAL_Initialize();
    if (g_expected_initialize != osal_ret)
    {
        test_ret = OSAL_TEST_NG;
    }
    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_initialize), osal_ret_to_str(osal_ret));
    printf("Check R_OSAL_Initialize result\n");
    osal_test_result_output(OSAL_TEST_CONFIG,
                            g_config_check,
                            test_ret, opt_info);

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (OSAL_TEST_OK == test_ret)
        {
            /* R_OSAL_ThsyncMutexCreate */
            osal_ret = R_OSAL_ThsyncMutexCreate(g_mutex_id, &handle);
            if (g_expected_mutex_create != osal_ret)
            {
                test_ret = OSAL_TEST_NG;
            }
            OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_mutex_create), osal_ret_to_str(osal_ret));
            printf("Check R_OSAL_ThsyncMutexCreate result\n");
            osal_test_result_output(OSAL_TEST_CONFIG,
                                    g_config_check,
                                    test_ret, opt_info);
            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = R_OSAL_ThsyncMutexDestroy(handle);
                if (OSAL_RETURN_OK != osal_ret)
                {
                    OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.", OSAL_TEST_CONFIG, 0);
                }
            }
        }
        osal_ret = R_OSAL_Deinitialize();
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", OSAL_TEST_CONFIG, 0);
        }
    }

    return;
}
/* ---------------------------------------------------------------------------------------- */

/* ---------------------- 10_012-014 [Condition Configuration Check] ---------------------- */
#elif (CHECK_COND)
extern int32_t         g_expected_initialize;
extern int32_t         g_expected_cond_create;
extern osal_cond_id_t  g_cond_id;
static void osal_test_config_check(void)
{
    int32_t                test_ret                              = OSAL_TEST_OK;
    char                   opt_info[OSAL_TEST_OPT_INFO_BUFFER];
    e_osal_return_t        osal_ret;
    osal_cond_handle_t     handle;

    /* R_OSAL_Initialize */
    osal_ret = R_OSAL_Initialize();
    if (g_expected_initialize != osal_ret)
    {
        test_ret = OSAL_TEST_NG;
    }
    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_initialize), osal_ret_to_str(osal_ret));
    printf("Check R_OSAL_Initialize result\n");
    osal_test_result_output(OSAL_TEST_CONFIG,
                            g_config_check,
                            test_ret, opt_info);

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (OSAL_TEST_OK == test_ret)
        {
            /* R_OSAL_ThsyncCondCreate */
            osal_ret = R_OSAL_ThsyncCondCreate(g_cond_id, &handle);
            if (g_expected_cond_create != osal_ret)
            {
                test_ret = OSAL_TEST_NG;
            }
            OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_cond_create), osal_ret_to_str(osal_ret));
            printf("Check R_OSAL_ThsyncCondCreate result\n");
            osal_test_result_output(OSAL_TEST_CONFIG,
                                    g_config_check,
                                    test_ret, opt_info);
            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = R_OSAL_ThsyncCondDestroy(handle);
                if (OSAL_RETURN_OK != osal_ret)
                {
                    OSAL_TEST_ERR("R_OSAL_ThsyncCondDestroy() error.", OSAL_TEST_CONFIG, 0);
                }
            }
        }
        osal_ret = R_OSAL_Deinitialize();
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", OSAL_TEST_CONFIG, 0);
        }
    }

    return;
}
/* ---------------------------------------------------------------------------------------- */

/* -------------------- 10_015-019 [Message Queue Configuration Check] -------------------- */
#elif (CHECK_MQ)
extern int32_t         g_expected_initialize;
extern int32_t         g_expected_mq_create;
extern osal_cond_id_t  g_mq_id;
static void osal_test_config_check(void)
{
    int32_t                test_ret                              = OSAL_TEST_OK;
    char                   opt_info[OSAL_TEST_OPT_INFO_BUFFER];
    e_osal_return_t        osal_ret;
    st_osal_mq_config_t    config;
    osal_mq_handle_t       handle;

    config.max_num_msg = 1U;
    config.msg_size    = 4U;

    /* R_OSAL_Initialize */
    osal_ret = R_OSAL_Initialize();
    if (g_expected_initialize != osal_ret)
    {
        test_ret = OSAL_TEST_NG;
    }
    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_initialize), osal_ret_to_str(osal_ret));
    printf("Check R_OSAL_Initialize result\n");
    osal_test_result_output(OSAL_TEST_CONFIG,
                            g_config_check,
                            test_ret, opt_info);

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (OSAL_TEST_OK == test_ret)
        {
            /* R_OSAL_MqCreate */
            osal_ret = R_OSAL_MqCreate(&config, g_mq_id, &handle);
            if (g_expected_mq_create != osal_ret)
            {
                test_ret = OSAL_TEST_NG;
            }
            OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_mq_create), osal_ret_to_str(osal_ret));
            printf("Check R_OSAL_MqCreate result\n");
            osal_test_result_output(OSAL_TEST_CONFIG,
                                    g_config_check,
                                    test_ret, opt_info);
            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = R_OSAL_MqDelete(handle);
                if (OSAL_RETURN_OK != osal_ret)
                {
                    OSAL_TEST_ERR("R_OSAL_MqDelete() error.", OSAL_TEST_CONFIG, 0);
                }
            }
        }
        osal_ret = R_OSAL_Deinitialize();
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", OSAL_TEST_CONFIG, 0);
        }
    }

    return;
}
/* ---------------------------------------------------------------------------------------- */

/* ------------------ 10_020-034 [Interrupt Thread Configuration Check] ------------------- */
#elif (CHECK_INTERRUPT)
extern int32_t  g_expected_initialize;
extern char     *g_device_id;
extern uint64_t g_irq_channel;
extern int32_t  g_expected_numOfChannels;
extern int32_t  g_expected_register;
extern int32_t  g_expected_deinitialize;
static void osal_test_config_check(void)
{
    int32_t              test_ret      = OSAL_TEST_OK;
    char                 opt_info[OSAL_TEST_OPT_INFO_BUFFER];
    e_osal_return_t      osal_ret;
    osal_device_handle_t device_handle;
    size_t               numOfChannels = 0;
    int                  arg           = 10;

    /* R_OSAL_Initialize */
    osal_ret = R_OSAL_Initialize();
    if (g_expected_initialize != osal_ret)
    {
        test_ret = OSAL_TEST_NG;
    }
    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_initialize), osal_ret_to_str(osal_ret));
    printf("Check R_OSAL_Initialize result\n");
    osal_test_result_output(OSAL_TEST_CONFIG,
                            g_config_check,
                            test_ret, opt_info);
#if defined (CHECK_REGISTER)
    if (OSAL_RETURN_OK == osal_ret)
    {
        if (OSAL_TEST_OK == test_ret)
        {
            osal_ret = R_OSAL_IoDeviceOpen(g_device_id, &device_handle);
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.", OSAL_TEST_CONFIG, 0);
            }
            else
            {
                /* R_OSAL_InterruptGetNumOfIrqChannels */
                osal_ret = R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannels);
                if (g_expected_numOfChannels != osal_ret)
                {
                    test_ret = OSAL_TEST_NG;
                }
                OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_numOfChannels), osal_ret_to_str(osal_ret));
                printf("Check R_OSAL_InterruptGetNumOfIrqChannels result\n");
                osal_test_result_output(OSAL_TEST_CONFIG,
                                        g_config_check,
                                        test_ret, opt_info);
                if (OSAL_RETURN_OK == osal_ret)
                {
                    /* R_OSAL_InterruptRegisterIsr */
                    osal_ret = R_OSAL_InterruptRegisterIsr(device_handle, g_irq_channel, OSAL_INTERRUPT_PRIORITY_LOWEST, osal_test_cb, (void*)&arg);
                    if (g_expected_register != osal_ret)
                    {
                        test_ret = OSAL_TEST_NG;
                    }
                    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_register), osal_ret_to_str(osal_ret));
                    printf("Check R_OSAL_InterruptRegisterIsr result\n");
                    osal_test_result_output(OSAL_TEST_CONFIG,
                                            g_config_check,
                                            test_ret, opt_info);
                    /* R_OSAL_InterruptEnableIsr */
                    osal_ret = R_OSAL_InterruptEnableIsr(device_handle, g_irq_channel);
                    if (g_expected_register != osal_ret)
                    {
                        test_ret = OSAL_TEST_NG;
                    }
                    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_register), osal_ret_to_str(osal_ret));
                    printf("Check R_OSAL_InterruptEnableIsr result\n");
                    osal_test_result_output(OSAL_TEST_CONFIG,
                                            g_config_check,
                                            test_ret, opt_info);
                    /* R_OSAL_InterruptDisableIsr */
                    osal_ret = R_OSAL_InterruptDisableIsr(device_handle, g_irq_channel);
                    if (g_expected_register != osal_ret)
                    {
                        test_ret = OSAL_TEST_NG;
                    }
                    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_register), osal_ret_to_str(osal_ret));
                    printf("Check R_OSAL_InterruptDisableIsr result\n");
                    osal_test_result_output(OSAL_TEST_CONFIG,
                                            g_config_check,
                                            test_ret, opt_info);
                    /* R_OSAL_InterruptUnregisterIsr */
                    osal_ret = R_OSAL_InterruptUnregisterIsr(device_handle, g_irq_channel, osal_test_cb);
                    if (g_expected_register != osal_ret)
                    {
                        test_ret = OSAL_TEST_NG;
                    }
                    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_register), osal_ret_to_str(osal_ret));
                    printf("Check R_OSAL_InterruptUnregisterIsr result\n");
                    osal_test_result_output(OSAL_TEST_CONFIG,
                                            g_config_check,
                                            test_ret, opt_info);
                    /* R_OSAL_InterruptSetIsrPriority */
                    osal_ret = R_OSAL_InterruptSetIsrPriority(device_handle, g_irq_channel, 0);
                    if (g_expected_register != osal_ret)
                    {
                        test_ret = OSAL_TEST_NG;
                    }
                    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_register), osal_ret_to_str(osal_ret));
                    printf("Check R_OSAL_InterruptSetIsrPriority result\n");
                    osal_test_result_output(OSAL_TEST_CONFIG,
                                            g_config_check,
                                            test_ret, opt_info);
                }
                osal_ret = R_OSAL_IoDeviceClose(device_handle);
                if (OSAL_RETURN_OK != osal_ret)
                {
                    OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", OSAL_TEST_CONFIG, 0);
                }
            }
#if defined (AFTER_ID)
            extern char    *g_device_id2;
            extern int32_t g_expected_numOfChannels2;
            extern int32_t g_expected_register2;
            g_config_check++;
            osal_ret = R_OSAL_IoDeviceOpen(g_device_id2, &device_handle);
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.", OSAL_TEST_CONFIG, 0);
            }
            else
            {
                /* R_OSAL_InterruptGetNumOfIrqChannels */
                osal_ret = R_OSAL_InterruptGetNumOfIrqChannels(device_handle, &numOfChannels);
                if (g_expected_numOfChannels2 != osal_ret)
                {
                    test_ret = OSAL_TEST_NG;
                }
                else
                {
                    test_ret = OSAL_TEST_OK;
                }
                OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_numOfChannels2), osal_ret_to_str(osal_ret));
                printf("Check R_OSAL_InterruptGetNumOfIrqChannels result\n");
                osal_test_result_output(OSAL_TEST_CONFIG,
                                        g_config_check,
                                        test_ret, opt_info);
                if (OSAL_RETURN_OK == osal_ret)
                {
                    /* R_OSAL_InterruptRegisterIsr */
                    osal_ret = R_OSAL_InterruptRegisterIsr(device_handle, 0, OSAL_INTERRUPT_PRIORITY_LOWEST, osal_test_cb, (void*)&arg);
                    if (g_expected_register2 != osal_ret)
                    {
                        test_ret = OSAL_TEST_NG;
                    }
                    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_register2), osal_ret_to_str(osal_ret));
                    printf("Check R_OSAL_InterruptRegisterIsr result\n");
                    osal_test_result_output(OSAL_TEST_CONFIG,
                                            g_config_check,
                                            test_ret, opt_info);
                    /* R_OSAL_InterruptEnableIsr */
                    osal_ret = R_OSAL_InterruptEnableIsr(device_handle, 0);
                    if (g_expected_register != osal_ret)
                    {
                        test_ret = OSAL_TEST_NG;
                    }
                    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_register), osal_ret_to_str(osal_ret));
                    printf("Check R_OSAL_InterruptEnableIsr result\n");
                    osal_test_result_output(OSAL_TEST_CONFIG,
                                            g_config_check,
                                            test_ret, opt_info);
                    /* R_OSAL_InterruptDisableIsr */
                    osal_ret = R_OSAL_InterruptDisableIsr(device_handle, 0);
                    if (g_expected_register != osal_ret)
                    {
                        test_ret = OSAL_TEST_NG;
                    }
                    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_register), osal_ret_to_str(osal_ret));
                    printf("Check R_OSAL_InterruptDisableIsr result\n");
                    osal_test_result_output(OSAL_TEST_CONFIG,
                                            g_config_check,
                                            test_ret, opt_info);
                    /* R_OSAL_InterruptUnregisterIsr */
                    osal_ret = R_OSAL_InterruptUnregisterIsr(device_handle, 0, osal_test_cb);
                    if (g_expected_register != osal_ret)
                    {
                        test_ret = OSAL_TEST_NG;
                    }
                    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_register), osal_ret_to_str(osal_ret));
                    printf("Check R_OSAL_InterruptUnregisterIsr result\n");
                    osal_test_result_output(OSAL_TEST_CONFIG,
                                            g_config_check,
                                            test_ret, opt_info);
                    /* R_OSAL_InterruptSetIsrPriority */
                    osal_ret = R_OSAL_InterruptSetIsrPriority(device_handle, 0, 0);
                    if (g_expected_register != osal_ret)
                    {
                        test_ret = OSAL_TEST_NG;
                    }
                    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_register), osal_ret_to_str(osal_ret));
                    printf("Check R_OSAL_InterruptSetIsrPriority result\n");
                    osal_test_result_output(OSAL_TEST_CONFIG,
                                            g_config_check,
                                            test_ret, opt_info);
                }
                osal_ret = R_OSAL_IoDeviceClose(device_handle);
                if (OSAL_RETURN_OK != osal_ret)
                {
                    OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", OSAL_TEST_CONFIG, 0);
                }
            }
#endif
            osal_ret = R_OSAL_Deinitialize();
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", OSAL_TEST_CONFIG, 0);
            }
        }
    }
#else
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_Deinitialize();
        if (g_expected_deinitialize != osal_ret)
        {
            test_ret = OSAL_TEST_NG;
        }
        OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_deinitialize), osal_ret_to_str(osal_ret));
        printf("Check R_OSAL_Deinitialize result\n");
        osal_test_result_output(OSAL_TEST_CONFIG,
                                g_config_check,
                                test_ret, opt_info);
    }
#endif

    return;
}
/* ---------------------------------------------------------------------------------------- */

/* ------------------ 10_035-068, 098-099 [Memory Configuration Check] -------------------- */
#elif (CHECK_MEMORY)
extern int32_t g_num_of_idx;
extern osal_memory_region_idx_t g_region_idx;
static void osal_test_config_check(void)
{
    int32_t                      test_ret = OSAL_TEST_OK;
    char                         opt_info[OSAL_TEST_OPT_INFO_BUFFER];
    e_osal_return_t              osal_ret = OSAL_RETURN_OK;
    st_osal_mmngr_config_t       config;
    osal_memory_manager_handle_t hndl_mmngr;
    osal_memory_region_idx_t     region_idx;
    bool                         initialize_ret = true;
    size_t                       loop_count;
    
    config.mode                        = OSAL_MMNGR_ALLOC_MODE_STACK;
    config.memory_limit                = 0x1000U;
    config.max_allowed_allocations     = 16U;
    config.max_registered_monitors_cbs = 100U;

/* R_OSAL_Initialize */
#if defined (CHECK_INITIALIZE)
    extern int32_t g_expected_initialize;
    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        initialize_ret = false;
    }
    if (g_expected_initialize != osal_ret)
    {
        test_ret = OSAL_TEST_NG;
    }
    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_initialize), osal_ret_to_str(osal_ret));
    printf("Check R_OSAL_Initialize result\n");
    osal_test_result_output(OSAL_TEST_CONFIG,
                            g_config_check,
                            test_ret, opt_info);
#endif

    if (OSAL_TEST_OK == test_ret)
    {
/* R_OSAL_MmngrOpen */
#if defined (CHECK_MMNGER_OPEN)
        extern int32_t g_expected_mmngr_open;
        osal_ret = R_OSAL_MmngrOpen(&config, &hndl_mmngr);
        if (g_expected_mmngr_open != osal_ret)
        {
            test_ret = OSAL_TEST_NG;
        }
        OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_mmngr_open), osal_ret_to_str(osal_ret));
        printf("Check R_OSAL_MmngrOpen result\n");
        osal_test_result_output(OSAL_TEST_CONFIG,
                                g_config_check,
                                test_ret, opt_info);
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_MmngrClose(hndl_mmngr);
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_TEST_ERR("R_OSAL_MmngrClose() error.", OSAL_TEST_CONFIG, 0);
            }
        }
        g_config_check++;
#endif

/* R_OSAL_MmngrOpenFromRegionIdx */
#if defined (CHECK_MMNGER_IDX_OPEN)
        extern int32_t g_expected_mmngr_open_region;
        region_idx = g_region_idx;
        for (loop_count = 0; g_num_of_idx > loop_count; loop_count++)
        {
            osal_ret = R_OSAL_MmngrOpenFromRegionIdx(&config, region_idx, &hndl_mmngr);
            if (g_expected_mmngr_open_region != osal_ret)
            {
                test_ret = OSAL_TEST_NG;
            }
            OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_mmngr_open_region), osal_ret_to_str(osal_ret));
            printf("Check R_OSAL_MmngrOpenFromRegionIdx result\n");
            osal_test_result_output(OSAL_TEST_CONFIG,
                                    g_config_check,
                                    test_ret, opt_info);
            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = R_OSAL_MmngrClose(hndl_mmngr);
                if (OSAL_RETURN_OK != osal_ret)
                {
                    OSAL_TEST_ERR("R_OSAL_MmngrClose() error.", OSAL_TEST_CONFIG, 0);
                }
            }
            g_config_check++;
            region_idx++;
        }
#endif

/* R_OSAL_MmngrGetConfig */
#if defined (CHECK_MMNGER_GET_CONFIG)
        extern int32_t g_expected_mmngr_get_config;
        osal_ret = R_OSAL_MmngrGetConfig(NULL, &config);
        if (g_expected_mmngr_get_config != osal_ret)
        {
            test_ret = OSAL_TEST_NG;
        }
        OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_mmngr_get_config), osal_ret_to_str(osal_ret));
        printf("Check R_OSAL_MmngrGetConfig result\n");
        osal_test_result_output(OSAL_TEST_CONFIG,
                                g_config_check,
                                test_ret, opt_info);
        g_config_check++;
#endif

/* R_OSAL_MmngrGetOsalMaxConfig */
#if defined (CHECK_MMNGER_GET_MAX_CONFIG)
        extern int32_t g_expected_mmngr_get_max_config;
        osal_ret = R_OSAL_MmngrGetOsalMaxConfig(&config);
        if (g_expected_mmngr_get_max_config != osal_ret)
        {
            test_ret = OSAL_TEST_NG;
        }
        OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_mmngr_get_max_config), osal_ret_to_str(osal_ret));
        printf("Check R_OSAL_MmngrGetOsalMaxConfig result\n");
        osal_test_result_output(OSAL_TEST_CONFIG,
                                g_config_check,
                                test_ret, opt_info);
        g_config_check++;
#endif

/* R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx */
#if defined (CHECK_MMNGER_GET_MAX_CONFIG_IDX)
        extern int32_t g_expected_mmngr_get_max_config_region;
        region_idx = g_region_idx;
        for (loop_count = 0; g_num_of_idx > loop_count; loop_count++)
        {
            osal_ret = R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx(region_idx, &config);
            if (g_expected_mmngr_get_max_config_region != osal_ret)
            {
                test_ret = OSAL_TEST_NG;
            }
            OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_mmngr_get_max_config_region), osal_ret_to_str(osal_ret));
            printf("Check R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx result\n");
            osal_test_result_output(OSAL_TEST_CONFIG,
                                    g_config_check,
                                    test_ret, opt_info);
            g_config_check++;
            region_idx++;
        }
#endif
    }

#if defined (CHECK_INITIALIZE)
    if (false != initialize_ret)
    {
        osal_ret = R_OSAL_Deinitialize();
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", OSAL_TEST_CONFIG, 0);
        }
    }
#endif

    return;
}
/* ---------------------------------------------------------------------------------------- */

/* -------------------------- 10_069-096 [Thread Setting Check] --------------------------- */
#elif (CHECK_THREAD_SETTING)
extern int32_t g_expected_initialize;
static void osal_test_config_check(void)
{
    int32_t                      test_ret = OSAL_TEST_OK;
    char                         opt_info[OSAL_TEST_OPT_INFO_BUFFER];
    e_osal_return_t              osal_ret = OSAL_RETURN_OK;

/* R_OSAL_Initialize */
    osal_ret = R_OSAL_Initialize();

    if (g_expected_initialize != osal_ret)
    {
        test_ret = OSAL_TEST_NG;
    }
    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_initialize), osal_ret_to_str(osal_ret));
    printf("Check R_OSAL_Initialize result\n");
    osal_test_result_output(OSAL_TEST_CONFIG,
                            g_config_check,
                            test_ret, opt_info);
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_Deinitialize();
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", OSAL_TEST_CONFIG, 0);
        }
    }
    return;
}
/* ---------------------------------------------------------------------------------------- */

#endif
