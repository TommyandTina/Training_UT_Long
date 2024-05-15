
#include "osal_test.h"

/* prototype */
/* Osal execution order test program */
static void osal_test_thread_create_after_initialize(void);             //1.R_OSAL_ThreadCreate(After calling R_OSAL_Initialize())
static void osal_test_thread_create_running_user_function(void);        //2.R_OSAL_ThreadCreate(Running user function)
static void osal_test_thread_create_after_user_function(void);          //3.R_OSAL_ThreadCreate(After user function)
static void osal_test_thread_create_after_deinitialize(void);           //4.R_OSAL_ThreadCreate(After calling R_OSAL_Deinitialize())
static void osal_test_thread_join_after_initialize(void);               //5.R_OSAL_ThreadJoin(After calling R_OSAL_Initialize())
static void osal_test_thread_join_running_user_function(void);          //6.R_OSAL_ThreadJoin(Running user function)
static void osal_test_thread_join_after_user_function(void);            //7.R_OSAL_ThreadJoin(After user function)
static void osal_test_thread_join_after_deinitialize(void);             //8.R_OSAL_ThreadJoin(After calling R_OSAL_Deinitialize())
static void osal_test_interrupt_register_after_initialize(void);        //9.R_OSAL_InterruptRegisterIsr(After calling R_OSAL_Initialize())
static void osal_test_interrupt_register_after_setting_isr(void);       //10.R_OSAL_InterruptRegisterIsr(After calling R_OSAL_InterruptRegisterIsr())
static void osal_test_interrupt_register_running_callback(void);        //11.R_OSAL_InterruptRegisterIsr(Running callback function)
static void osal_test_interrupt_register_after_deinitialize(void);      //12.R_OSAL_InterruptRegisterIsr(After calling R_OSAL_Deinitialize())
static void osal_test_interrupt_unregister_after_initialize(void);      //13.R_OSAL_InterruptUnregisterIsr(After calling R_OSAL_Initialize())
static void osal_test_interrupt_unegister_after_setting_isr(void);      //14.R_OSAL_InterruptUnregisterIsr(After calling R_OSAL_InterruptRegisterIsr()
static void osal_test_interrupt_unregister_running_callback(void);      //15.R_OSAL_InterruptUnregisterIsr(Running callback function)
static void osal_test_interrupt_unregister_after_deinitialize(void);    //16.R_OSAL_InterruptUnregisterIsr(After calling R_OSAL_Deinitialize())
static void osal_test_interrupt_enable_after_initialize(void);          //17.R_OSAL_InterruptEnableIsr(After calling R_OSAL_Initialize())
static void osal_test_interrupt_enable_after_setting_isr(void);         //18.R_OSAL_InterruptEnableIsr(After calling R_OSAL_InterruptRegisterIsr())
static void osal_test_interrupt_enable_running_callback(void);          //19.R_OSAL_InterruptEnableIsr(Running callback function)
static void osal_test_interrupt_enable_after_deinitialize(void);        //20.R_OSAL_InterruptEnableIsr(After calling R_OSAL_Deinitialize())
static void osal_test_interrupt_disable_after_initialize(void);         //21.R_OSAL_InterruptDisableIsr(After calling R_OSAL_Initialize())
static void osal_test_interrupt_disable_after_setting_isr(void);        //22.R_OSAL_InterruptDisableIsr(After calling R_OSAL_InterruptRegisterIsr())
static void osal_test_interrupt_disable_running_callback(void);         //23.R_OSAL_InterruptDisableIsr(Running callback function)
static void osal_test_interrupt_disable_after_deinitialize(void);       //24.R_OSAL_InterruptDisableIsr(After calling R_OSAL_Deinitialize())

/* Internal function */
/* Wait for specified state of user/callback function to be changed */
e_osal_return_t wait_function_state(e_function_state_t state);
/* Allocate Memory */
e_osal_return_t allocate_setup_data_memory(osal_memory_manager_handle_t* hndl_mmngr,
                                           uint32_t *hw_addr,
                                           osal_memory_buffer_handle_t* p_buffer_obj);

/* Deallocate Memory */
e_osal_return_t deallocate_data_memory(osal_memory_manager_handle_t hndl_mmngr,
                                       osal_memory_buffer_handle_t* buffer_obj);

/* Grobal variable */
static e_function_state_t g_function_active; // User/Callback function state for synchronization
static uint32_t g_test_pass = 0;             // Number of pass cases
static uint32_t g_test_fail = 0;             // Number of fail cases

/* Expected return value each test case */
static e_osal_return_t g_expected_return_value[TOTAL_TEST_COUNT + 1] = {
    OSAL_RETURN_UNSUPPORTED_OPERATION,           //Unused
    OSAL_RETURN_OK,                              //1.R_OSAL_ThreadCreate(After calling R_OSAL_Initialize())
    OSAL_RETURN_BUSY,                            //2.R_OSAL_ThreadCreate(Running user function)
    OSAL_RETURN_BUSY,                            //3.R_OSAL_ThreadCreate(After user function)
    OSAL_RETURN_STATE,                           //4.R_OSAL_ThreadCreate(After calling R_OSAL_Deinitialize())
    OSAL_RETURN_HANDLE,                          //5.R_OSAL_ThreadJoin(After calling R_OSAL_Initialize())
    OSAL_RETURN_OK,                              //6.R_OSAL_ThreadJoin(Running user function)
    OSAL_RETURN_OK,                              //7.R_OSAL_ThreadJoin(After user function)
    OSAL_RETURN_STATE,                           //8.R_OSAL_ThreadJoin(After calling R_OSAL_Deinitialize())
    OSAL_RETURN_OK,                              //9.R_OSAL_InterruptRegisterIsr(After calling R_OSAL_Initialize())
    OSAL_RETURN_BUSY,                            //10.R_OSAL_InterruptRegisterIsr(After calling R_OSAL_InterruptRegisterIsr())
    OSAL_RETURN_BUSY,                            //11.R_OSAL_InterruptRegisterIsr(Running callback function)
    OSAL_RETURN_STATE,                           //12.R_OSAL_InterruptRegisterIsr(After calling R_OSAL_Deinitialize())
    OSAL_RETURN_PAR,                             //13.R_OSAL_InterruptUnregisterIsr(After calling R_OSAL_Initialize())
    OSAL_RETURN_OK,                              //14.R_OSAL_InterruptUnregisterIsr(After calling R_OSAL_InterruptRegisterIsr())
    OSAL_RETURN_OK,                              //15.R_OSAL_InterruptUnregisterIsr(Running callback function)
    OSAL_RETURN_STATE,                           //16.R_OSAL_InterruptUnregisterIsr(After calling R_OSAL_Deinitialize())
    OSAL_RETURN_FAIL,                            //17.R_OSAL_InterruptEnableIsr(After calling R_OSAL_Initialize())
    OSAL_RETURN_OK,                              //18.R_OSAL_InterruptEnableIsr(After calling R_OSAL_InterruptRegisterIsr())
    OSAL_RETURN_OK,                              //19.R_OSAL_InterruptEnableIsr(Running callback function)
    OSAL_RETURN_STATE,                           //20.R_OSAL_InterruptEnableIsr(After calling R_OSAL_Deinitialize())
    OSAL_RETURN_FAIL,                            //21.R_OSAL_InterruptDisableIsr(After calling R_OSAL_Initialize())
    OSAL_RETURN_OK,                              //22.R_OSAL_InterruptDisableIsr(After calling R_OSAL_InterruptRegisterIsr())
    OSAL_RETURN_OK,                              //23.R_OSAL_InterruptDisableIsr(Running callback function)
    OSAL_RETURN_STATE                            //24.R_OSAL_InterruptDisableIsr(After calling R_OSAL_Deinitialize())
};

void osal_test_execution_order(int test_id)
{
    switch(test_id)
    {
        case(0):printf("\n1.R_OSAL_ThreadCreate(After calling R_OSAL_Initialize())\n");
                osal_test_thread_create_after_initialize();
                break;
        case(1):printf("\n2.R_OSAL_ThreadCreate(Running user function)\n");
                osal_test_thread_create_running_user_function();
                break;
        case(2):printf("\n3.R_OSAL_ThreadCreate(After user function)\n");
                osal_test_thread_create_after_user_function();
                break;
        case(3):printf("\n4.R_OSAL_ThreadCreate(After calling R_OSAL_Deinitialize())\n");
                osal_test_thread_create_after_deinitialize();
                break;
        case(4):printf("\n5.R_OSAL_ThreadJoin(After calling R_OSAL_Initialize())\n");
                osal_test_thread_join_after_initialize();
                break;
        case(5):printf("\n6.R_OSAL_ThreadJoin(Running user function)\n");
                osal_test_thread_join_running_user_function();
                break;
        case(6):printf("\n7.R_OSAL_ThreadJoin(After user function)\n");
                osal_test_thread_join_after_user_function();
                break;
        case(7):printf("\n8.R_OSAL_ThreadJoin(After calling R_OSAL_Deinitialize())\n");
                osal_test_thread_join_after_deinitialize();
                break;
        case(8):printf("\n9.R_OSAL_InterruptRegisterIsr(After calling R_OSAL_Initialize())\n");
                osal_test_interrupt_register_after_initialize();
                break;
        case(9):printf("\n10.R_OSAL_InterruptRegisterIsr(After calling R_OSAL_InterruptRegisterIsr())\n");
                osal_test_interrupt_register_after_setting_isr();
                break;
        case(10):printf("\n11.R_OSAL_InterruptRegisterIsr(Running callback function)\n");
                 osal_test_interrupt_register_running_callback();
                 break;
        case(11):printf("\n12.R_OSAL_InterruptRegisterIsr(After calling R_OSAL_Deinitialize())\n");
                 osal_test_interrupt_register_after_deinitialize();
                 break;
        case(12):printf("\n13.R_OSAL_InterruptUnregisterIsr(After calling R_OSAL_Initialize())\n");
                 osal_test_interrupt_unregister_after_initialize();
                 break;
        case(13):printf("\n14.R_OSAL_InterruptUnregisterIsr(After calling R_OSAL_InterruptRegisterIsr()\n");
                 osal_test_interrupt_unegister_after_setting_isr();
                 break;
        case(14):printf("\n15.R_OSAL_InterruptUnregisterIsr(Running callback function)\n");
                 osal_test_interrupt_unregister_running_callback();
                 break;
        case(15):printf("\n16.R_OSAL_InterruptUnregisterIsr(After calling R_OSAL_Deinitialize())\n");
                 osal_test_interrupt_unregister_after_deinitialize();
                 break;
        case(16):printf("\n17.R_OSAL_InterruptEnableIsr(After calling R_OSAL_Initialize())\n");
                 osal_test_interrupt_enable_after_initialize();
                 break;
        case(17):printf("\n18.R_OSAL_InterruptEnableIsr(After calling R_OSAL_InterruptRegisterIsr())\n");
                 osal_test_interrupt_enable_after_setting_isr();
                 break;
        case(18):printf("\n19.R_OSAL_InterruptEnableIsr(Running callback function)\n");
                 osal_test_interrupt_enable_running_callback();
                 break;
        case(19):printf("\n20.R_OSAL_InterruptEnableIsr(After calling R_OSAL_Deinitialize())\n");
                 osal_test_interrupt_enable_after_deinitialize();
                 break;
        case(20):printf("\n21.R_OSAL_InterruptDisableIsr(After calling R_OSAL_Initialize())\n");
                 osal_test_interrupt_disable_after_initialize();
                 break;
        case(21):printf("\n22.R_OSAL_InterruptDisableIsr(After calling R_OSAL_InterruptRegisterIsr())\n");
                 osal_test_interrupt_disable_after_setting_isr();
                 break;
        case(22):printf("\n23.R_OSAL_InterruptDisableIsr(Running callback function)\n");
                 osal_test_interrupt_disable_running_callback();
                 break;
        case(23):printf("\n24.R_OSAL_InterruptDisableIsr(After calling R_OSAL_Deinitialize())\n");
                 osal_test_interrupt_disable_after_deinitialize();
                 break;
        default:break;
    }

    return;
}

/* Convert return value code to string */
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
    printf("***   Osal Internal Execution Order Test        ***\n");

    printf("***    1-4: Thread Create Test                  ***\n");
    printf("***    5-8: Thread Join Test                    ***\n");
    printf("***   9-12: Interrupt Register Test             ***\n");
    printf("***  13-16: Interrupt Unregister Test           ***\n");
    printf("***  17-20: Interrupt Enable Test               ***\n");
    printf("***  21-24: Interrupt Disable Test              ***\n");
}

/* Output test result */
static void osal_test_result_output(e_test_case_number_t test_case, int result, char *param) 
{
    if (param == NULL)
    {
        if (result == OSAL_TEST_OK)
        {
            printf("test case %d result OK.\n", test_case);
        }
        else
        {
            printf("test case %d result NG.\n", test_case);
        }
    }
    else
    {
        if (result == OSAL_TEST_OK)
        {
            printf("test case %d result OK.\n%s\n", test_case, param);
            g_test_pass++;
        } 
        else 
        {
            printf("test case %d result NG.\n%s\n", test_case, param);
            g_test_fail++;
        }
    }
}

/* Main Function */
int main(void)
{
    osal_test_configuration_display_menu();

    for (int test_count = 0; test_count < TOTAL_TEST_COUNT; test_count++)
    {
        osal_test_execution_order(test_count);
    }

    printf("\n< OSAL Tests OK = %d, NG = %d, NT = %d>\n", g_test_pass, g_test_fail, (TOTAL_TEST_COUNT - (g_test_pass + g_test_fail)));
    if (TOTAL_TEST_COUNT != g_test_pass)
    {
        return -1;
    }

    return 0;
}

/* User function */
static int64_t user_function(void* p_arg)
{
    return 0;
}

/* User function for synchronization test */
static int64_t user_function_for_synchronization(void* p_arg)
{
    osal_thread_handle_t self_handle;

    /* Start of user function */
    g_function_active = FUNCTION_RUNNING;

    printf("Start user function.\n");
    /* Busy Loop about 10msec */
    for (uint32_t i = 0U; TEST_CALLBACK_BUSY_LOOP_COUNT > i; i++)
    {
        self_handle = NULL;
        (void)R_OSAL_ThreadSelf(&self_handle);
    }

    /* End of user function */
    g_function_active = FUNCTION_RUN_AFTER;

    return 0;
}

/*------------------------- 1.R_OSAL_ThreadCreate(After calling R_OSAL_Initialize()) -------------------------------*/
static void osal_test_thread_create_after_initialize(void)
{
    e_osal_return_t osal_ret;
    int32_t         test_ret = OSAL_TEST_OK;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_thread_handle_t handle;
    st_osal_thread_config_t config;
    int64_t             arg = 1;

    config.func       = user_function;
    config.userarg    = (void*)&arg;
    config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config.task_name  = "test_task_01";
    config.stack_size = 0x1000;

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.", osal_ret_to_str(osal_ret));
    }
    else
    {
        /* Wait for finishing R_OSAL_Initialize(). */
        (void)sleep(10);

        /* Test case 1 */
        osal_ret = R_OSAL_ThreadCreate(&config, OSAL_TEST_THREAD_ID, &handle);
        if (g_expected_return_value[TEST_CASE_01] != osal_ret)
        {
            test_ret = OSAL_TEST_NG;
        }
        printf("Check R_OSAL_ThreadCreate result\n");
        OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_return_value[TEST_CASE_01]), osal_ret_to_str(osal_ret));
        osal_test_result_output(TEST_CASE_01, test_ret, opt_info);

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_ThreadJoin(handle, NULL);
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.", osal_ret_to_str(osal_ret));
            }
        }
        osal_ret = R_OSAL_Deinitialize();
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", osal_ret_to_str(osal_ret));
        }
    }

    return;
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------- 2.R_OSAL_ThreadCreate(Running user function) -------------------------------------*/
static void osal_test_thread_create_running_user_function(void)
{
    e_osal_return_t osal_ret;
    int32_t         test_ret = OSAL_TEST_OK;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_thread_handle_t        handle1;
    osal_thread_handle_t        handle2;
    st_osal_thread_config_t     config1;
    st_osal_thread_config_t     config2;
    int64_t                     arg = 1;

    config1.func       = user_function_for_synchronization;
    config1.userarg    = (void*)&arg;
    config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config1.task_name  = "test_task_01";
    config1.stack_size = 0x1000;

    config2.func       = user_function;
    config2.userarg    = (void*)&arg;
    config2.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config2.task_name  = "test_task_02";
    config2.stack_size = 0x1000;

    int              os_ret;
    struct timespec  time_stamp;
    uint32_t         elapsed_time = 0U;

    g_function_active = FUNCTION_RUN_BEFORE;
    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.", osal_ret_to_str(osal_ret));
    }
    else
    {
        /* Run user function */
        osal_ret = R_OSAL_ThreadCreate(&config1, OSAL_TEST_THREAD_ID, &handle1);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.", osal_ret_to_str(osal_ret));
        }
        else
        {
            /* Main thread waits for user function to be called. */
            osal_ret = wait_function_state(FUNCTION_RUNNING);
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_TEST_ERR("wait_function_state() error.", osal_ret_to_str(osal_ret));
            }
            else
            {
                /* Test case 2 */
                osal_ret = R_OSAL_ThreadCreate(&config2, OSAL_TEST_THREAD_ID, &handle2);
                if (g_expected_return_value[TEST_CASE_02] != osal_ret)
                {
                    test_ret = OSAL_TEST_NG;
                }
                printf("Check R_OSAL_ThreadCreate result\n");
                OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_return_value[TEST_CASE_02]), osal_ret_to_str(osal_ret));
                osal_test_result_output(TEST_CASE_02, test_ret, opt_info);

                osal_ret = R_OSAL_ThreadJoin(handle1, NULL);
                if (OSAL_RETURN_OK != osal_ret)
                {
                    OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.", osal_ret_to_str(osal_ret));
                }
            }
        }
        osal_ret = R_OSAL_Deinitialize();
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", osal_ret_to_str(osal_ret));
        }
    }

    return;
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------- 3.R_OSAL_ThreadCreate(After user function) --------------------------------------*/
static void osal_test_thread_create_after_user_function(void)
{
    e_osal_return_t osal_ret;
    int32_t         test_ret = OSAL_TEST_OK;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_thread_handle_t       handle1;
    osal_thread_handle_t       handle2;
    st_osal_thread_config_t    config1;
    st_osal_thread_config_t    config2;
    int64_t                    arg = 1;

    config1.func       = user_function_for_synchronization;
    config1.userarg    = (void*)&arg;
    config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config1.task_name  = "test_task_01";
    config1.stack_size = 0x1000;

    config2.func       = user_function;
    config2.userarg    = (void*)&arg;
    config2.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config2.task_name  = "test_task_02";
    config2.stack_size = 0x1000;

    int              os_ret;
    struct timespec  time_stamp;
    uint32_t         elapsed_time = 0U;

    g_function_active = FUNCTION_RUN_BEFORE;
    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.", osal_ret_to_str(osal_ret));
    }
    else
    {
        /* Run user function */
        osal_ret = R_OSAL_ThreadCreate(&config1, OSAL_TEST_THREAD_ID, &handle1);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.", osal_ret_to_str(osal_ret));
        }
        else
        {
            /* Main thread waits for user function to be finished. */
            osal_ret = wait_function_state(FUNCTION_RUN_AFTER);
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_TEST_ERR("wait_function_state() error.", osal_ret_to_str(osal_ret));
            }
            else
            {
                /* Test case 3 */
                osal_ret = R_OSAL_ThreadCreate(&config2, OSAL_TEST_THREAD_ID, &handle2);
                if (g_expected_return_value[TEST_CASE_03] != osal_ret)
                {
                    test_ret = OSAL_TEST_NG;
                }
                printf("Check R_OSAL_ThreadCreate result\n");
                OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_return_value[TEST_CASE_03]), osal_ret_to_str(osal_ret));
                osal_test_result_output(TEST_CASE_03, test_ret, opt_info);

                osal_ret = R_OSAL_ThreadJoin(handle1, NULL);
                if (OSAL_RETURN_OK != osal_ret)
                {
                    OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.", osal_ret_to_str(osal_ret));
                }
            }
        }
        osal_ret = R_OSAL_Deinitialize();
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", osal_ret_to_str(osal_ret));
        }
    }

    return;
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*------------------------- 4.R_OSAL_ThreadCreate(After calling R_OSAL_Deinitialize()) -------------------------------*/
static void osal_test_thread_create_after_deinitialize(void)
{
    e_osal_return_t osal_ret;
    int32_t         test_ret = OSAL_TEST_OK;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_thread_handle_t       handle;
    st_osal_thread_config_t    config;
    int64_t                    arg = 1;

    config.func       = user_function;
    config.userarg    = (void*)&arg;
    config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config.task_name  = "test_task_01";
    config.stack_size = 0x1000;

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.", osal_ret_to_str(osal_ret));
    }
    else
    {
        osal_ret = R_OSAL_Deinitialize();
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", osal_ret_to_str(osal_ret));
        }
        else
        {
            /* Test case 4 */
            osal_ret = R_OSAL_ThreadCreate(&config, OSAL_TEST_THREAD_ID, &handle);
            if (g_expected_return_value[TEST_CASE_04] != osal_ret)
            {
                test_ret = OSAL_TEST_NG;
            }
            printf("Check R_OSAL_ThreadCreate result\n");
            OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_return_value[TEST_CASE_04]), osal_ret_to_str(osal_ret));
            osal_test_result_output(TEST_CASE_04, test_ret, opt_info);
        }
    }

    return;
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*------------------------- 5.R_OSAL_ThreadJoin(After calling R_OSAL_Initialize()) -------------------------------*/
static void osal_test_thread_join_after_initialize(void)
{
    e_osal_return_t osal_ret;
    int32_t         test_ret = OSAL_TEST_OK;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_thread_handle_t    handle;
    int64_t                 return_value;

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.", osal_ret_to_str(osal_ret));
    }
    else
    {
        /* Wait for finishing R_OSAL_Initialize(). */
        (void)sleep(10);

        /* Test case 5 */
        osal_ret = R_OSAL_ThreadJoin(handle, &return_value);
        if (g_expected_return_value[TEST_CASE_05] != osal_ret)
        {
            test_ret = OSAL_TEST_NG;
        }
        printf("Check R_OSAL_ThreadJoin result\n");
        OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_return_value[TEST_CASE_05]), osal_ret_to_str(osal_ret));
        osal_test_result_output(TEST_CASE_05, test_ret, opt_info);

        osal_ret = R_OSAL_Deinitialize();
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", osal_ret_to_str(osal_ret));
        }
    }

    return;
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------- 6.R_OSAL_ThreadJoin(Running user function) --------------------------------------*/
static void osal_test_thread_join_running_user_function(void)
{
    e_osal_return_t    osal_ret;
    int32_t            test_ret = OSAL_TEST_OK;
    char               opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_thread_handle_t       handle;
    st_osal_thread_config_t    config;
    int64_t                    arg = 1;
    int64_t                    return_value;

    config.func       = user_function_for_synchronization;
    config.userarg    = (void*)&arg;
    config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config.task_name  = "test_task_01";
    config.stack_size = 0x1000;
    
    int              os_ret;
    struct timespec  time_stamp;
    uint32_t         elapsed_time = 0U;

    g_function_active = FUNCTION_RUN_BEFORE;
    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.", osal_ret_to_str(osal_ret));
    }
    else
    {
        /* Run user function */
        osal_ret = R_OSAL_ThreadCreate(&config, OSAL_TEST_THREAD_ID, &handle);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.", osal_ret_to_str(osal_ret));
        }
        else
        {
            /* Main thread waits for user function to be called. */
            osal_ret = wait_function_state(FUNCTION_RUNNING);
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_TEST_ERR("wait_function_state() error.", osal_ret_to_str(osal_ret));
            }
            else
            {
                /* Test case 6 */
                osal_ret = R_OSAL_ThreadJoin(handle, &return_value);
                if ((g_expected_return_value[TEST_CASE_06] != osal_ret) || (FUNCTION_RUN_AFTER != g_function_active))
                {
                    test_ret = OSAL_TEST_NG;
                }
                printf("Check R_OSAL_ThreadJoin result\n");
                OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_return_value[TEST_CASE_06]), osal_ret_to_str(osal_ret));
                osal_test_result_output(TEST_CASE_06, test_ret, opt_info);
            }
        }
        osal_ret = R_OSAL_Deinitialize();
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", osal_ret_to_str(osal_ret));
        }
    }

    return;
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------- 7.R_OSAL_ThreadJoin(After user function) ---------------------------------------*/
static void osal_test_thread_join_after_user_function(void)
{
    e_osal_return_t    osal_ret;
    int32_t            test_ret = OSAL_TEST_OK;
    char               opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_thread_handle_t       handle;
    st_osal_thread_config_t    config;
    int64_t                    arg = 1;
    int64_t                    return_value;

    config.func       = user_function_for_synchronization;
    config.userarg    = (void*)&arg;
    config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config.task_name  = "test_task_01";
    config.stack_size = 0x1000;

    int              os_ret;
    struct timespec  time_stamp;
    uint32_t         elapsed_time = 0U;

    g_function_active = FUNCTION_RUN_BEFORE;
    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.", osal_ret_to_str(osal_ret));
    }
    else
    {
        /* Run user function */
        osal_ret = R_OSAL_ThreadCreate(&config, OSAL_TEST_THREAD_ID, &handle);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.", osal_ret_to_str(osal_ret));
        }
        else
        {
            /* Main thread waits for user function to be finished. */
            osal_ret = wait_function_state(FUNCTION_RUN_AFTER);
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_TEST_ERR("wait_function_state() error.", osal_ret_to_str(osal_ret));
            }
            else
            {
                /* Test case 7 */
                osal_ret = R_OSAL_ThreadJoin(handle, &return_value);
                if ((g_expected_return_value[TEST_CASE_07] != osal_ret) || (FUNCTION_RUN_AFTER != g_function_active))
                {
                    test_ret = OSAL_TEST_NG;
                }
                printf("Check R_OSAL_ThreadJoin result\n");
                OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_return_value[TEST_CASE_07]), osal_ret_to_str(osal_ret));
                osal_test_result_output(TEST_CASE_07, test_ret, opt_info);
            }
        }
        osal_ret = R_OSAL_Deinitialize();
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", osal_ret_to_str(osal_ret));
        }
    }

    return;
}
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------- 8.R_OSAL_ThreadJoin(After calling R_OSAL_Deinitialize()) -------------------------------*/
static void osal_test_thread_join_after_deinitialize(void)
{
    e_osal_return_t osal_ret;
    int32_t         test_ret = OSAL_TEST_OK;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_thread_handle_t    handle;
    int64_t                 return_value;

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.", osal_ret_to_str(osal_ret));
    }
    else
    {
        osal_ret = R_OSAL_Deinitialize();
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", osal_ret_to_str(osal_ret));
        }
        else
        {
            /* Test case 8 */
            osal_ret = R_OSAL_ThreadJoin(handle, &return_value);
            if (g_expected_return_value[TEST_CASE_08] != osal_ret)
            {
                test_ret = OSAL_TEST_NG;
            }
            printf("Check R_OSAL_ThreadJoin result\n");
            OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_return_value[TEST_CASE_08]), osal_ret_to_str(osal_ret));
            osal_test_result_output(TEST_CASE_08, test_ret, opt_info);
        }
    }

    return;
}
/*--------------------------------------------------------------------------------------------------------------------*/

void osal_isr_test(osal_device_handle_t dev_handle,
                            uint64_t irq_channel, void *irq_arg)
{
    return;
}

/* CallBack Function */
void osal_isr_test_for_synchronization(osal_device_handle_t dev_handle,
                                       uint64_t irq_channel, void *irq_arg)
{
    uint32_t    init_clear_data;

    /* Start of callback function */
    g_function_active = FUNCTION_RUNNING;

    printf("Start callback function.\n");

    /* Disable interrupt */
    (void)R_OSAL_InterruptDisableIsr(dev_handle, irq_channel);

    /* Busy Loop about 10msec*/
    for (uint32_t i = 0U; TEST_ISR_BUSY_LOOP_COUNT > i; i++)
    {
        (void)R_OSAL_IoRead32(dev_handle, TEST_REG_CLEAR_REGISTER, &init_clear_data);
    }

    /* End of callback function */
    g_function_active = FUNCTION_RUN_AFTER;

    return;
}

/*------------------------- 9.R_OSAL_InterruptRegisterIsr(After calling R_OSAL_Initialize()) -------------------------*/
static void osal_test_interrupt_register_after_initialize(void)
{
    e_osal_return_t     osal_ret;
    int32_t             test_ret = OSAL_TEST_OK;
    char                opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_device_handle_t    device_handle;
    int64_t                 arg = 1;

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.", osal_ret_to_str(osal_ret));
    }
    else
    {
        /* Wait for finishing R_OSAL_Initialize(). */
        (void)sleep(10);

        osal_ret = R_OSAL_IoDeviceOpen(TEST_INTCTX_DEVICE_1, &device_handle);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.", osal_ret_to_str(osal_ret));
        }
        else
        {
            /* Test case 9 */
            osal_ret = R_OSAL_InterruptRegisterIsr(device_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
                                                   osal_isr_test, (void *)&arg);
            if (g_expected_return_value[TEST_CASE_09] != osal_ret)
            {
                test_ret = OSAL_TEST_NG;
            }
            printf("Check R_OSAL_InterruptRegisterIsr result\n");
            OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_return_value[TEST_CASE_09]), osal_ret_to_str(osal_ret));
            osal_test_result_output(TEST_CASE_09, test_ret, opt_info);

            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = R_OSAL_InterruptUnregisterIsr(device_handle, 0, osal_isr_test);
                if (OSAL_RETURN_OK != osal_ret)
                {
                    OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.", osal_ret_to_str(osal_ret));
                }
            }
            osal_ret = R_OSAL_IoDeviceClose(device_handle);
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", osal_ret_to_str(osal_ret));
            }
        }

        osal_ret = R_OSAL_Deinitialize();
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", osal_ret_to_str(osal_ret));
        }
    }

    return;
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*------------------- 10.R_OSAL_InterruptRegisterIsr(After calling R_OSAL_InterruptRegisterIsr()) --------------------*/
static void osal_test_interrupt_register_after_setting_isr(void)
{
    e_osal_return_t osal_ret;
    int32_t         test_ret = OSAL_TEST_OK;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_device_handle_t    device_handle;
    int64_t                 arg = 1;


    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.", osal_ret_to_str(osal_ret));
    }
    else
    {
        osal_ret = R_OSAL_IoDeviceOpen(TEST_INTCTX_DEVICE_1, &device_handle);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.", osal_ret_to_str(osal_ret));
        }
        else
        {
            /* Register isr */
            osal_ret = R_OSAL_InterruptRegisterIsr(device_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
                                                   osal_isr_test, (void *)&arg);
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.", osal_ret_to_str(osal_ret));
            }
            else
            {
                /* Test case 10 */
                osal_ret = R_OSAL_InterruptRegisterIsr(device_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
                                                       osal_isr_test, (void *)&arg);
                if (g_expected_return_value[TEST_CASE_10] != osal_ret)
                {
                    test_ret = OSAL_TEST_NG;
                }            
                printf("Check R_OSAL_InterruptRegisterIsr result\n");
                OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_return_value[TEST_CASE_10]), osal_ret_to_str(osal_ret));
                osal_test_result_output(TEST_CASE_10, test_ret, opt_info);

                osal_ret = R_OSAL_InterruptUnregisterIsr(device_handle, 0, osal_isr_test);
                if (OSAL_RETURN_OK != osal_ret)
                {
                    OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.", osal_ret_to_str(osal_ret));
                }
            }

            osal_ret = R_OSAL_IoDeviceClose(device_handle);
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", osal_ret_to_str(osal_ret));
            }
        }

        osal_ret = R_OSAL_Deinitialize();
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", osal_ret_to_str(osal_ret));
        }
    }

    return;
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*-------------------------- 11.R_OSAL_InterruptRegisterIsr(Running callback function) --------------------------*/
static void osal_test_interrupt_register_running_callback(void)
{
    e_osal_return_t osal_ret;
    int32_t         test_ret = OSAL_TEST_OK;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_device_handle_t device_handle;
    osal_device_handle_t device_handle2;
    int64_t              arg = 1;
    uint32_t             hw_addr[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    osal_memory_buffer_handle_t  buffer_obj[TEST_NUM_OF_BUFFERS];

    int              os_ret;
    struct timespec  time_stamp;
    uint32_t         elapsed_time = 0U;

    g_function_active = FUNCTION_RUN_BEFORE;
    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.", osal_ret_to_str(osal_ret));
    }
    else
    {
        /* Device Open */
        osal_ret = R_OSAL_IoDeviceOpen(TEST_INTCTX_DEVICE_1, &device_handle);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.", osal_ret_to_str(osal_ret));
        }
        else
        {
            osal_ret = R_OSAL_IoDeviceOpen(TEST_INTCTX_DEVICE_2, &device_handle2);
            if (OSAL_RETURN_OK != osal_ret)
            {
                (void)R_OSAL_IoDeviceClose(device_handle);
                OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.", osal_ret_to_str(osal_ret));
            }
        }

        /* SetUp */
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
            if (OSAL_RETURN_OK != osal_ret)
            {
                (void)R_OSAL_IoDeviceClose(device_handle2);
                (void)R_OSAL_IoDeviceClose(device_handle);
                OSAL_TEST_ERR("R_OSAL_PmSetRequiredState() error.", osal_ret_to_str(osal_ret));
            }
        }

        /* Allocate read/write addresses */
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = allocate_setup_data_memory(&hndl_mmngr, &hw_addr[0], &buffer_obj[0]);
            if (OSAL_RETURN_OK != osal_ret)
            {
                (void)R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_RELEASED, true);
                (void)R_OSAL_IoDeviceClose(device_handle2);
                (void)R_OSAL_IoDeviceClose(device_handle);
            }
        }

        /* Generate isr */
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_InterruptRegisterIsr(device_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE0, osal_isr_test_for_synchronization, (void *)&arg);
            if (OSAL_RETURN_OK != osal_ret)
            {
                (void)R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_RELEASED, true);
                (void)R_OSAL_IoDeviceClose(device_handle2);
                (void)R_OSAL_IoDeviceClose(device_handle);
                (void)deallocate_data_memory(hndl_mmngr, &buffer_obj[0]);
                OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.", osal_ret_to_str(osal_ret));
            }
            else
            {
                osal_ret = R_OSAL_InterruptEnableIsr(device_handle, 0);
                if (OSAL_RETURN_OK != osal_ret)
                {
                    (void)R_OSAL_InterruptUnregisterIsr(device_handle, 0, osal_isr_test_for_synchronization);
                    (void)R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_RELEASED, true);
                    (void)R_OSAL_IoDeviceClose(device_handle2);
                    (void)R_OSAL_IoDeviceClose(device_handle);
                    (void)deallocate_data_memory(hndl_mmngr, &buffer_obj[0]);
                    OSAL_TEST_ERR("R_OSAL_InterruptEnableIsr() error.", osal_ret_to_str(osal_ret));
                }
                else
                {
                    /* SetUp HW register */
                    /* Config DMA operation register */
                    (void)R_OSAL_IoWrite16(device_handle2, TEST_REG_RDMOR, TEST_TRANSFER_RDMOR_VALUE);
                    /* Config transfer src address */
                    (void)R_OSAL_IoWrite32(device_handle,  TEST_REG_RDMSAR, hw_addr[0]);
                    /* Config transfer dst address */
                    (void)R_OSAL_IoWrite32(device_handle,  TEST_REG_RDMDAR, hw_addr[1]);
                    /* Config transfer count */
                    (void)R_OSAL_IoWrite32(device_handle,  TEST_REG_RDMTCR, TEST_TRANSFER_COUNT_VALUE);
                    /* Config transfer steps */
                    (void)R_OSAL_IoWrite32(device_handle,  TEST_REG_RDMCHCRB, TEST_TRANSFER_STEP_VALUE);
                    /* Config DMA channel control register */
                    (void)R_OSAL_IoWrite32(device_handle,  TEST_REG_RDMCHCR, TEST_TRANSFER_RDMCHCR_VALUE);
                }
            }
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* Main thread waits for callback function to be called. */
            osal_ret = wait_function_state(FUNCTION_RUNNING);
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_TEST_ERR("wait_function_state() error.", osal_ret_to_str(osal_ret));
            }
            else
            {
                /* Test case 11 */
                osal_ret = R_OSAL_InterruptRegisterIsr(device_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE0, osal_isr_test, (void *)&arg);
                if ((g_expected_return_value[TEST_CASE_11] != osal_ret) || (FUNCTION_RUN_AFTER != g_function_active))
                {
                    test_ret = OSAL_TEST_NG;
                }
                printf("Check R_OSAL_InterruptRegisterIsr result\n");
                OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_return_value[TEST_CASE_11]), osal_ret_to_str(osal_ret));
                osal_test_result_output(TEST_CASE_11, test_ret, opt_info);
            }

            /* Disable Device Interrupt */
            (void)R_OSAL_InterruptDisableIsr(device_handle, 0);
            (void)R_OSAL_InterruptUnregisterIsr(device_handle, 0, osal_isr_test_for_synchronization);

            /* Deallocate read/write addresses */
            osal_ret = deallocate_data_memory(hndl_mmngr, &buffer_obj[0]);
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_TEST_ERR("deallocate_data_memory() error.", osal_ret_to_str(osal_ret));
            }

            /* TearDown */
            (void)R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_RELEASED, true);
            (void)R_OSAL_IoDeviceClose(device_handle2);
            (void)R_OSAL_IoDeviceClose(device_handle);
        }

        osal_ret = R_OSAL_Deinitialize();
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", osal_ret_to_str(osal_ret));
        }
    }

    return;
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*---------------------- 12.R_OSAL_InterruptRegisterIsr(After calling R_OSAL_Deinitialize()) -------------------------*/
static void osal_test_interrupt_register_after_deinitialize(void)
{
    e_osal_return_t osal_ret;
    int32_t         test_ret = OSAL_TEST_OK;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_device_handle_t device_handle;
    int64_t              arg = 1;

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.", osal_ret_to_str(osal_ret));
    }
    else
    {
        osal_ret = R_OSAL_Deinitialize();
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", osal_ret_to_str(osal_ret));
        }
        else
        {
            /* Test case 12 */
            osal_ret = R_OSAL_InterruptRegisterIsr(device_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE0, osal_isr_test, (void *)&arg);
            if (g_expected_return_value[TEST_CASE_12] != osal_ret)
            {
                test_ret = OSAL_TEST_NG;
            }
            printf("Check R_OSAL_InterruptRegisterIsr result\n");
            OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_return_value[TEST_CASE_12]), osal_ret_to_str(osal_ret));
            osal_test_result_output(TEST_CASE_12, test_ret, opt_info);
        }
    }

    return;
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*------------------------- 13.R_OSAL_InterruptUnregisterIsr(After calling R_OSAL_Initialize()) -------------------------*/
static void osal_test_interrupt_unregister_after_initialize(void)
{
    e_osal_return_t     osal_ret;
    int32_t             test_ret = OSAL_TEST_OK;
    char                opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_device_handle_t    device_handle;

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.", osal_ret_to_str(osal_ret));
    }
    else
    {
        /* Wait for finishing R_OSAL_Initialize(). */
        (void)sleep(10);

        osal_ret = R_OSAL_IoDeviceOpen(TEST_INTCTX_DEVICE_1, &device_handle);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.", osal_ret_to_str(osal_ret));
        }
        else
        {
            /* Test case 13 */
            osal_ret = R_OSAL_InterruptUnregisterIsr(device_handle, 0, osal_isr_test);
            if (g_expected_return_value[TEST_CASE_13] != osal_ret)
            {
                test_ret = OSAL_TEST_NG;
            }
            printf("Check R_OSAL_InterruptUnregisterIsr result\n");
            OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_return_value[TEST_CASE_13]), osal_ret_to_str(osal_ret));
            osal_test_result_output(TEST_CASE_13, test_ret, opt_info);

            osal_ret = R_OSAL_IoDeviceClose(device_handle);
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", osal_ret_to_str(osal_ret));
            }
        }

        osal_ret = R_OSAL_Deinitialize();
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", osal_ret_to_str(osal_ret));
        }
    }

    return;
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*------------------- 14.R_OSAL_InterruptUnregisterIsr(After calling R_OSAL_InterruptRegisterIsr() -------------------*/
static void osal_test_interrupt_unegister_after_setting_isr(void)
{
    e_osal_return_t osal_ret;
    int32_t         test_ret = OSAL_TEST_OK;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_device_handle_t    device_handle;
    int64_t                 arg = 1;


    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.", osal_ret_to_str(osal_ret));
    }
    else
    {
        osal_ret = R_OSAL_IoDeviceOpen(TEST_INTCTX_DEVICE_1, &device_handle);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.", osal_ret_to_str(osal_ret));
        }
        else
        {
            /* Register isr */
            osal_ret = R_OSAL_InterruptRegisterIsr(device_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
                                                   osal_isr_test, (void *)&arg);
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.", osal_ret_to_str(osal_ret));
            }
            else
            {
                /* Test case 14 */
                osal_ret = R_OSAL_InterruptUnregisterIsr(device_handle, 0, osal_isr_test);
                if (g_expected_return_value[TEST_CASE_14] != osal_ret)
                {
                    test_ret = OSAL_TEST_NG;
                }            
                printf("Check R_OSAL_InterruptUnregisterIsr result\n");
                OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_return_value[TEST_CASE_14]), osal_ret_to_str(osal_ret));
                osal_test_result_output(TEST_CASE_14, test_ret, opt_info);
            }

            osal_ret = R_OSAL_IoDeviceClose(device_handle);
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", osal_ret_to_str(osal_ret));
            }
        }

        osal_ret = R_OSAL_Deinitialize();
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", osal_ret_to_str(osal_ret));
        }
    }

    return;
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*------------------------- 15.R_OSAL_InterruptUnregisterIsr(Running callback function) -------------------------*/
static void osal_test_interrupt_unregister_running_callback(void)
{
    e_osal_return_t osal_ret;
    int32_t         test_ret = OSAL_TEST_OK;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_device_handle_t device_handle;
    osal_device_handle_t device_handle2;
    int64_t              arg = 1;
    uint32_t             hw_addr[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    osal_memory_buffer_handle_t  buffer_obj[TEST_NUM_OF_BUFFERS];

    int              os_ret;
    struct timespec  time_stamp;
    uint32_t         elapsed_time = 0U;

    g_function_active = FUNCTION_RUN_BEFORE;
    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.", osal_ret_to_str(osal_ret));
    }
    else
    {
        /* Device Open */
        osal_ret = R_OSAL_IoDeviceOpen(TEST_INTCTX_DEVICE_1, &device_handle);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.", osal_ret_to_str(osal_ret));
        }
        else
        {
            osal_ret = R_OSAL_IoDeviceOpen(TEST_INTCTX_DEVICE_2, &device_handle2);
            if (OSAL_RETURN_OK != osal_ret)
            {
                (void)R_OSAL_IoDeviceClose(device_handle);
                OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.", osal_ret_to_str(osal_ret));
            }
        }

        /* SetUp */
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
            if (OSAL_RETURN_OK != osal_ret)
            {
                (void)R_OSAL_IoDeviceClose(device_handle2);
                (void)R_OSAL_IoDeviceClose(device_handle);
                OSAL_TEST_ERR("R_OSAL_PmSetRequiredState() error.", osal_ret_to_str(osal_ret));
            }
        }

        /* Allocate read/write addresses */
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = allocate_setup_data_memory(&hndl_mmngr, &hw_addr[0], &buffer_obj[0]);
            if (OSAL_RETURN_OK != osal_ret)
            {
                (void)R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_RELEASED, true);
                (void)R_OSAL_IoDeviceClose(device_handle2);
                (void)R_OSAL_IoDeviceClose(device_handle);
            }
        }

        /* Generate isr */
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_InterruptRegisterIsr(device_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE0, osal_isr_test_for_synchronization, (void *)&arg);
            if (OSAL_RETURN_OK != osal_ret)
            {
                (void)R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_RELEASED, true);
                (void)R_OSAL_IoDeviceClose(device_handle2);
                (void)R_OSAL_IoDeviceClose(device_handle);
                (void)deallocate_data_memory(hndl_mmngr, &buffer_obj[0]);
                OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.", osal_ret_to_str(osal_ret));
            }
            else
            {
                osal_ret = R_OSAL_InterruptEnableIsr(device_handle, 0);
                if (OSAL_RETURN_OK != osal_ret)
                {
                    (void)R_OSAL_InterruptUnregisterIsr(device_handle, 0, osal_isr_test_for_synchronization);
                    (void)R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_RELEASED, true);
                    (void)R_OSAL_IoDeviceClose(device_handle2);
                    (void)R_OSAL_IoDeviceClose(device_handle);
                    (void)deallocate_data_memory(hndl_mmngr, &buffer_obj[0]);
                    OSAL_TEST_ERR("R_OSAL_InterruptEnableIsr() error.", osal_ret_to_str(osal_ret));
                }
                else
                {
                    /* SetUp HW register */
                    /* Config DMA operation register */
                    (void)R_OSAL_IoWrite16(device_handle2, TEST_REG_RDMOR, TEST_TRANSFER_RDMOR_VALUE);
                    /* Config transfer src address */
                    (void)R_OSAL_IoWrite32(device_handle,  TEST_REG_RDMSAR, hw_addr[0]);
                    /* Config transfer dst address */
                    (void)R_OSAL_IoWrite32(device_handle,  TEST_REG_RDMDAR, hw_addr[1]);
                    /* Config transfer count */
                    (void)R_OSAL_IoWrite32(device_handle,  TEST_REG_RDMTCR, TEST_TRANSFER_COUNT_VALUE);
                    /* Config transfer steps */
                    (void)R_OSAL_IoWrite32(device_handle,  TEST_REG_RDMCHCRB, TEST_TRANSFER_STEP_VALUE);
                    /* Config DMA channel control register */
                    (void)R_OSAL_IoWrite32(device_handle,  TEST_REG_RDMCHCR, TEST_TRANSFER_RDMCHCR_VALUE);
                }
            }
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* Main thread waits for callback function to be called. */
            osal_ret = wait_function_state(FUNCTION_RUNNING);
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_TEST_ERR("wait_function_state() error.", osal_ret_to_str(osal_ret));
            }
            else
            {
                /* Test case 15 */
                osal_ret = R_OSAL_InterruptUnregisterIsr(device_handle, 0, osal_isr_test_for_synchronization);
                if ((g_expected_return_value[TEST_CASE_15] != osal_ret) || (FUNCTION_RUN_AFTER != g_function_active))
                {
                    test_ret = OSAL_TEST_NG;
                }
                printf("Check R_OSAL_InterruptUnregisterIsr result\n");
                OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_return_value[TEST_CASE_15]), osal_ret_to_str(osal_ret));
                osal_test_result_output(TEST_CASE_15, test_ret, opt_info);
            }

            /* Disable Device Interrupt */
            (void)R_OSAL_InterruptDisableIsr(device_handle, 0);
            (void)R_OSAL_InterruptUnregisterIsr(device_handle, 0, osal_isr_test_for_synchronization);

            /* Deallocate read/write addresses */
            osal_ret = deallocate_data_memory(hndl_mmngr, &buffer_obj[0]);
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_TEST_ERR("deallocate_data_memory() error.", osal_ret_to_str(osal_ret));
            }

            /* TearDown */
            (void)R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_RELEASED, true);
            (void)R_OSAL_IoDeviceClose(device_handle2);
            (void)R_OSAL_IoDeviceClose(device_handle);
        }

        osal_ret = R_OSAL_Deinitialize();
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", osal_ret_to_str(osal_ret));
        }
    }

    return;
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------- 16.R_OSAL_InterruptUnregisterIsr(After calling R_OSAL_Deinitialize()) ------------------------*/
static void osal_test_interrupt_unregister_after_deinitialize(void)
{
    e_osal_return_t osal_ret;
    int32_t         test_ret = OSAL_TEST_OK;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_device_handle_t device_handle;

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.", osal_ret_to_str(osal_ret));
    }
    else
    {
        osal_ret = R_OSAL_Deinitialize();
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", osal_ret_to_str(osal_ret));
        }
        else
        {
            /* Test case 16 */
            osal_ret = R_OSAL_InterruptUnregisterIsr(device_handle, 0, osal_isr_test);
            if (g_expected_return_value[TEST_CASE_16] != osal_ret)
            {
                test_ret = OSAL_TEST_NG;
            }
            printf("Check R_OSAL_InterruptUnregisterIsr result\n");
            OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_return_value[TEST_CASE_16]), osal_ret_to_str(osal_ret));
            osal_test_result_output(TEST_CASE_16, test_ret, opt_info);
        }
    }

    return;
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*------------------------- 17.R_OSAL_InterruptEnableIsr(After calling R_OSAL_Initialize()) -------------------------*/
static void osal_test_interrupt_enable_after_initialize(void)
{
    e_osal_return_t     osal_ret;
    int32_t             test_ret = OSAL_TEST_OK;
    char                opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_device_handle_t    device_handle;

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.", osal_ret_to_str(osal_ret));
    }
    else
    {
        /* Wait for finishing R_OSAL_Initialize(). */
        (void)sleep(10);

        osal_ret = R_OSAL_IoDeviceOpen(TEST_INTCTX_DEVICE_1, &device_handle);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.", osal_ret_to_str(osal_ret));
        }
        else
        {
            /* Test case 17 */
            osal_ret = R_OSAL_InterruptEnableIsr(device_handle, 0);
            if (g_expected_return_value[TEST_CASE_17] != osal_ret)
            {
                test_ret = OSAL_TEST_NG;
            }
            printf("Check R_OSAL_InterruptEnableIsr result\n");
            OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_return_value[TEST_CASE_17]), osal_ret_to_str(osal_ret));
            osal_test_result_output(TEST_CASE_17, test_ret, opt_info);

            osal_ret = R_OSAL_IoDeviceClose(device_handle);
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", osal_ret_to_str(osal_ret));
            }
        }

        osal_ret = R_OSAL_Deinitialize();
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", osal_ret_to_str(osal_ret));
        }
    }

    return;
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*------------------- 18.R_OSAL_InterruptEnableIsr(After calling R_OSAL_InterruptRegisterIsr()) -------------------*/
static void osal_test_interrupt_enable_after_setting_isr(void)
{
    e_osal_return_t osal_ret;
    int32_t         test_ret = OSAL_TEST_OK;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_device_handle_t    device_handle;
    int64_t                 arg = 1;


    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.", osal_ret_to_str(osal_ret));
    }
    else
    {
        osal_ret = R_OSAL_IoDeviceOpen(TEST_INTCTX_DEVICE_1, &device_handle);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.", osal_ret_to_str(osal_ret));
        }
        else
        {
            /* Register isr */
            osal_ret = R_OSAL_InterruptRegisterIsr(device_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
                                                   osal_isr_test, (void *)&arg);
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.", osal_ret_to_str(osal_ret));
            }
            else
            {
                /* Test case 18 */
                osal_ret = R_OSAL_InterruptEnableIsr(device_handle, 0);
                if (g_expected_return_value[TEST_CASE_18] != osal_ret)
                {
                    test_ret = OSAL_TEST_NG;
                }            
                printf("Check R_OSAL_InterruptEnableIsr result\n");
                OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_return_value[TEST_CASE_18]), osal_ret_to_str(osal_ret));
                osal_test_result_output(TEST_CASE_18, test_ret, opt_info);

                osal_ret = R_OSAL_InterruptDisableIsr(device_handle, 0);
                if (OSAL_RETURN_OK != osal_ret)
                {
                    OSAL_TEST_ERR("R_OSAL_InterruptDisableIsr() error.", osal_ret_to_str(osal_ret));
                }
                osal_ret = R_OSAL_InterruptUnregisterIsr(device_handle, 0, osal_isr_test);
                if (OSAL_RETURN_OK != osal_ret)
                {
                    OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.", osal_ret_to_str(osal_ret));
                }
            }

            osal_ret = R_OSAL_IoDeviceClose(device_handle);
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", osal_ret_to_str(osal_ret));
            }
        }

        osal_ret = R_OSAL_Deinitialize();
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", osal_ret_to_str(osal_ret));
        }
    }

    return;
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*-------------------------- 19.R_OSAL_InterruptEnableIsr(Running callback function) --------------------------*/
static void osal_test_interrupt_enable_running_callback(void)
{
    e_osal_return_t osal_ret;
    int32_t         test_ret = OSAL_TEST_OK;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_device_handle_t device_handle;
    osal_device_handle_t device_handle2;
    int64_t              arg = 1;
    uint32_t             hw_addr[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    osal_memory_buffer_handle_t  buffer_obj[TEST_NUM_OF_BUFFERS];

    int              os_ret;
    struct timespec  time_stamp;
    uint32_t         elapsed_time = 0U;

    g_function_active = FUNCTION_RUN_BEFORE;
    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.", osal_ret_to_str(osal_ret));
    }
    else
    {
        /* Device Open */
        osal_ret = R_OSAL_IoDeviceOpen(TEST_INTCTX_DEVICE_1, &device_handle);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.", osal_ret_to_str(osal_ret));
        }
        else
        {
            osal_ret = R_OSAL_IoDeviceOpen(TEST_INTCTX_DEVICE_2, &device_handle2);
            if (OSAL_RETURN_OK != osal_ret)
            {
                (void)R_OSAL_IoDeviceClose(device_handle);
                OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.", osal_ret_to_str(osal_ret));
            }
        }

        /* SetUp */
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
            if (OSAL_RETURN_OK != osal_ret)
            {
                (void)R_OSAL_IoDeviceClose(device_handle2);
                (void)R_OSAL_IoDeviceClose(device_handle);
                OSAL_TEST_ERR("R_OSAL_PmSetRequiredState() error.", osal_ret_to_str(osal_ret));
            }
        }

        /* Allocate read/write addresses */
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = allocate_setup_data_memory(&hndl_mmngr, &hw_addr[0], &buffer_obj[0]);
            if (OSAL_RETURN_OK != osal_ret)
            {
                (void)R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_RELEASED, true);
                (void)R_OSAL_IoDeviceClose(device_handle2);
                (void)R_OSAL_IoDeviceClose(device_handle);
            }
        }

        /* Generate isr */
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_InterruptRegisterIsr(device_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE0, osal_isr_test_for_synchronization, (void *)&arg);
            if (OSAL_RETURN_OK != osal_ret)
            {
                (void)R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_RELEASED, true);
                (void)R_OSAL_IoDeviceClose(device_handle2);
                (void)R_OSAL_IoDeviceClose(device_handle);
                (void)deallocate_data_memory(hndl_mmngr, &buffer_obj[0]);
                OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.", osal_ret_to_str(osal_ret));
            }
            else
            {
                osal_ret = R_OSAL_InterruptEnableIsr(device_handle, 0);
                if (OSAL_RETURN_OK != osal_ret)
                {
                    (void)R_OSAL_InterruptUnregisterIsr(device_handle, 0, osal_isr_test_for_synchronization);
                    (void)R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_RELEASED, true);
                    (void)R_OSAL_IoDeviceClose(device_handle2);
                    (void)R_OSAL_IoDeviceClose(device_handle);
                    (void)deallocate_data_memory(hndl_mmngr, &buffer_obj[0]);
                    OSAL_TEST_ERR("R_OSAL_InterruptEnableIsr() error.", osal_ret_to_str(osal_ret));
                }
                else
                {
                    /* SetUp HW register */
                    /* Config DMA operation register */
                    (void)R_OSAL_IoWrite16(device_handle2, TEST_REG_RDMOR, TEST_TRANSFER_RDMOR_VALUE);
                    /* Config transfer src address */
                    (void)R_OSAL_IoWrite32(device_handle,  TEST_REG_RDMSAR, hw_addr[0]);
                    /* Config transfer dst address */
                    (void)R_OSAL_IoWrite32(device_handle,  TEST_REG_RDMDAR, hw_addr[1]);
                    /* Config transfer count */
                    (void)R_OSAL_IoWrite32(device_handle,  TEST_REG_RDMTCR, TEST_TRANSFER_COUNT_VALUE);
                    /* Config transfer steps */
                    (void)R_OSAL_IoWrite32(device_handle,  TEST_REG_RDMCHCRB, TEST_TRANSFER_STEP_VALUE);
                    /* Config DMA channel control register */
                    (void)R_OSAL_IoWrite32(device_handle,  TEST_REG_RDMCHCR, TEST_TRANSFER_RDMCHCR_VALUE);
                }
            }
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* Main thread waits for callback function to be called. */
            osal_ret = wait_function_state(FUNCTION_RUNNING);
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_TEST_ERR("wait_function_state() error.", osal_ret_to_str(osal_ret));
            }
            else
            {
                /* Test case 19 */
                osal_ret = R_OSAL_InterruptEnableIsr(device_handle, 0);
                if ((g_expected_return_value[TEST_CASE_19] != osal_ret) || (FUNCTION_RUN_AFTER != g_function_active))
                {
                    test_ret = OSAL_TEST_NG;
                }
                printf("Check R_OSAL_InterruptEnableIsr result\n");
                OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_return_value[TEST_CASE_19]), osal_ret_to_str(osal_ret));
                osal_test_result_output(TEST_CASE_19, test_ret, opt_info);
            }

            /* Disable Device Interrupt */
            (void)R_OSAL_InterruptDisableIsr(device_handle, 0);
            (void)R_OSAL_InterruptUnregisterIsr(device_handle, 0, osal_isr_test_for_synchronization);

            /* Deallocate read/write addresses */
            osal_ret = deallocate_data_memory(hndl_mmngr, &buffer_obj[0]);
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_TEST_ERR("deallocate_data_memory() error.", osal_ret_to_str(osal_ret));
            }

            /* TearDown */
            (void)R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_RELEASED, true);
            (void)R_OSAL_IoDeviceClose(device_handle2);
            (void)R_OSAL_IoDeviceClose(device_handle);
        }

        osal_ret = R_OSAL_Deinitialize();
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", osal_ret_to_str(osal_ret));
        }
    }

    return;
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*---------------------- 20.R_OSAL_InterruptEnableIsr(After calling R_OSAL_Deinitialize()) -------------------------*/
static void osal_test_interrupt_enable_after_deinitialize(void)
{
    e_osal_return_t osal_ret;
    int32_t         test_ret = OSAL_TEST_OK;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_device_handle_t device_handle;

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.", osal_ret_to_str(osal_ret));
    }
    else
    {
        osal_ret = R_OSAL_Deinitialize();
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", osal_ret_to_str(osal_ret));
        }
        else
        {
            /* Test case 20 */
            osal_ret = R_OSAL_InterruptEnableIsr(device_handle, 0);
            if (g_expected_return_value[TEST_CASE_20] != osal_ret)
            {
                test_ret = OSAL_TEST_NG;
            }
            printf("Check R_OSAL_InterruptEnableIsr result\n");
            OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_return_value[TEST_CASE_20]), osal_ret_to_str(osal_ret));
            osal_test_result_output(TEST_CASE_20, test_ret, opt_info);
        }
    }

    return;
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*------------------------- 21.R_OSAL_InterruptDisableIsr(After calling R_OSAL_Initialize()) -------------------------*/
static void osal_test_interrupt_disable_after_initialize(void)
{
    e_osal_return_t     osal_ret;
    int32_t             test_ret = OSAL_TEST_OK;
    char                opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_device_handle_t    device_handle;

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.", osal_ret_to_str(osal_ret));
    }
    else
    {
        /* Wait for finishing R_OSAL_Initialize(). */
        (void)sleep(10);

        osal_ret = R_OSAL_IoDeviceOpen(TEST_INTCTX_DEVICE_1, &device_handle);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.", osal_ret_to_str(osal_ret));
        }
        else
        {
            /* Test case 21 */
            osal_ret = R_OSAL_InterruptDisableIsr(device_handle, 0);
            if (g_expected_return_value[TEST_CASE_21] != osal_ret)
            {
                test_ret = OSAL_TEST_NG;
            }
            printf("Check R_OSAL_InterruptDisableIsr result\n");
            OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_return_value[TEST_CASE_21]), osal_ret_to_str(osal_ret));
            osal_test_result_output(TEST_CASE_21, test_ret, opt_info);

            osal_ret = R_OSAL_IoDeviceClose(device_handle);
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", osal_ret_to_str(osal_ret));
            }
        }

        osal_ret = R_OSAL_Deinitialize();
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", osal_ret_to_str(osal_ret));
        }
    }

    return;
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*------------------- 22.R_OSAL_InterruptDisableIsr(After calling R_OSAL_InterruptRegisterIsr()) -------------------*/
static void osal_test_interrupt_disable_after_setting_isr(void)
{
    e_osal_return_t osal_ret;
    int32_t         test_ret = OSAL_TEST_OK;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_device_handle_t    device_handle;
    int64_t                 arg = 1;


    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.", osal_ret_to_str(osal_ret));
    }
    else
    {
        osal_ret = R_OSAL_IoDeviceOpen(TEST_INTCTX_DEVICE_1, &device_handle);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.", osal_ret_to_str(osal_ret));
        }
        else
        {
            /* Register isr */
            osal_ret = R_OSAL_InterruptRegisterIsr(device_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
                                                   osal_isr_test, (void *)&arg);
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.", osal_ret_to_str(osal_ret));
            }
            else
            {
                /* Test case 22 */
                osal_ret = R_OSAL_InterruptDisableIsr(device_handle, 0);
                if (g_expected_return_value[TEST_CASE_22] != osal_ret)
                {
                    test_ret = OSAL_TEST_NG;
                }            
                printf("Check R_OSAL_InterruptDisableIsr result\n");
                OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_return_value[TEST_CASE_22]), osal_ret_to_str(osal_ret));
                osal_test_result_output(TEST_CASE_22, test_ret, opt_info);

                osal_ret = R_OSAL_InterruptUnregisterIsr(device_handle, 0, osal_isr_test);
                if (OSAL_RETURN_OK != osal_ret)
                {
                    OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.", osal_ret_to_str(osal_ret));
                }
            }

            osal_ret = R_OSAL_IoDeviceClose(device_handle);
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", osal_ret_to_str(osal_ret));
            }
        }

        osal_ret = R_OSAL_Deinitialize();
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", osal_ret_to_str(osal_ret));
        }
    }

    return;
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*-------------------------- 23.R_OSAL_InterruptDisableIsr(Running callback function) --------------------------*/
static void osal_test_interrupt_disable_running_callback(void)
{
    e_osal_return_t osal_ret;
    int32_t         test_ret = OSAL_TEST_OK;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_device_handle_t device_handle;
    osal_device_handle_t device_handle2;
    int64_t              arg = 1;
    uint32_t             hw_addr[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    osal_memory_buffer_handle_t  buffer_obj[TEST_NUM_OF_BUFFERS];

    int              os_ret;
    struct timespec  time_stamp;
    uint32_t         elapsed_time = 0U;

    g_function_active = FUNCTION_RUN_BEFORE;
    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.", osal_ret_to_str(osal_ret));
    }
    else
    {
        /* Device Open */
        osal_ret = R_OSAL_IoDeviceOpen(TEST_INTCTX_DEVICE_1, &device_handle);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.", osal_ret_to_str(osal_ret));
        }
        else
        {
            osal_ret = R_OSAL_IoDeviceOpen(TEST_INTCTX_DEVICE_2, &device_handle2);
            if (OSAL_RETURN_OK != osal_ret)
            {
                (void)R_OSAL_IoDeviceClose(device_handle);
                OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.", osal_ret_to_str(osal_ret));
            }
        }

        /* SetUp */
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
            if (OSAL_RETURN_OK != osal_ret)
            {
                (void)R_OSAL_IoDeviceClose(device_handle2);
                (void)R_OSAL_IoDeviceClose(device_handle);
                OSAL_TEST_ERR("R_OSAL_PmSetRequiredState() error.", osal_ret_to_str(osal_ret));
            }
        }

        /* Allocate read/write addresses */
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = allocate_setup_data_memory(&hndl_mmngr, &hw_addr[0], &buffer_obj[0]);
            if (OSAL_RETURN_OK != osal_ret)
            {
                (void)R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_RELEASED, true);
                (void)R_OSAL_IoDeviceClose(device_handle2);
                (void)R_OSAL_IoDeviceClose(device_handle);
            }
        }

        /* Generate isr */
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_InterruptRegisterIsr(device_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE0, osal_isr_test_for_synchronization, (void *)&arg);
            if (OSAL_RETURN_OK != osal_ret)
            {
                (void)R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_RELEASED, true);
                (void)R_OSAL_IoDeviceClose(device_handle2);
                (void)R_OSAL_IoDeviceClose(device_handle);
                (void)deallocate_data_memory(hndl_mmngr, &buffer_obj[0]);
                OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.", osal_ret_to_str(osal_ret));
            }
            else
            {
                osal_ret = R_OSAL_InterruptEnableIsr(device_handle, 0);
                if (OSAL_RETURN_OK != osal_ret)
                {
                    (void)R_OSAL_InterruptUnregisterIsr(device_handle, 0, osal_isr_test_for_synchronization);
                    (void)R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_RELEASED, true);
                    (void)R_OSAL_IoDeviceClose(device_handle2);
                    (void)R_OSAL_IoDeviceClose(device_handle);
                    (void)deallocate_data_memory(hndl_mmngr, &buffer_obj[0]);
                    OSAL_TEST_ERR("R_OSAL_InterruptEnableIsr() error.", osal_ret_to_str(osal_ret));
                }
                else
                {
                    /* SetUp HW register */
                    /* Config DMA operation register */
                    (void)R_OSAL_IoWrite16(device_handle2, TEST_REG_RDMOR, TEST_TRANSFER_RDMOR_VALUE);
                    /* Config transfer src address */
                    (void)R_OSAL_IoWrite32(device_handle,  TEST_REG_RDMSAR, hw_addr[0]);
                    /* Config transfer dst address */
                    (void)R_OSAL_IoWrite32(device_handle,  TEST_REG_RDMDAR, hw_addr[1]);
                    /* Config transfer count */
                    (void)R_OSAL_IoWrite32(device_handle,  TEST_REG_RDMTCR, TEST_TRANSFER_COUNT_VALUE);
                    /* Config transfer steps */
                    (void)R_OSAL_IoWrite32(device_handle,  TEST_REG_RDMCHCRB, TEST_TRANSFER_STEP_VALUE);
                    /* Config DMA channel control register */
                    (void)R_OSAL_IoWrite32(device_handle,  TEST_REG_RDMCHCR, TEST_TRANSFER_RDMCHCR_VALUE);
                }
            }
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* Main thread waits for callback function to be called. */
            osal_ret = wait_function_state(FUNCTION_RUNNING);
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_TEST_ERR("wait_function_state() error.", osal_ret_to_str(osal_ret));
            }
            else
            {
                /* Test case 23 */
                osal_ret = R_OSAL_InterruptDisableIsr(device_handle, 0);
                if ((g_expected_return_value[TEST_CASE_23] != osal_ret) || (FUNCTION_RUN_AFTER != g_function_active))
                {
                    test_ret = OSAL_TEST_NG;
                }
                printf("Check R_OSAL_InterruptDisableIsr result\n");
                OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_return_value[TEST_CASE_23]), osal_ret_to_str(osal_ret));
                osal_test_result_output(TEST_CASE_23, test_ret, opt_info);
            }

            /* Disable Device Interrupt */
            (void)R_OSAL_InterruptDisableIsr(device_handle, 0);
            (void)R_OSAL_InterruptUnregisterIsr(device_handle, 0, osal_isr_test_for_synchronization);

            /* Deallocate read/write addresses */
            osal_ret = deallocate_data_memory(hndl_mmngr, &buffer_obj[0]);
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_TEST_ERR("deallocate_data_memory() error.", osal_ret_to_str(osal_ret));
            }

            /* TearDown */
            (void)R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_RELEASED, true);
            (void)R_OSAL_IoDeviceClose(device_handle2);
            (void)R_OSAL_IoDeviceClose(device_handle);
        }

        osal_ret = R_OSAL_Deinitialize();
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", osal_ret_to_str(osal_ret));
        }
    }

    return;
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*---------------------- 24.R_OSAL_InterruptDisableIsr(After calling R_OSAL_Deinitialize()) -------------------------*/
static void osal_test_interrupt_disable_after_deinitialize(void)
{
    e_osal_return_t osal_ret;
    int32_t         test_ret = OSAL_TEST_OK;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_device_handle_t device_handle;

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.", osal_ret_to_str(osal_ret));
    }
    else
    {
        osal_ret = R_OSAL_Deinitialize();
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", osal_ret_to_str(osal_ret));
        }
        else
        {
            /* Test case 24 */
            osal_ret = R_OSAL_InterruptDisableIsr(device_handle, 0);
            if (g_expected_return_value[TEST_CASE_24] != osal_ret)
            {
                test_ret = OSAL_TEST_NG;
            }
            printf("Check R_OSAL_InterruptDisableIsr result\n");
            OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(g_expected_return_value[TEST_CASE_24]), osal_ret_to_str(osal_ret));
            osal_test_result_output(TEST_CASE_24, test_ret, opt_info);
        }
    }

    return;
}
/*--------------------------------------------------------------------------------------------------------------------*/


/*------------------------------- Internal function(Wait for user/callback function) ---------------------------------*/
e_osal_return_t wait_function_state(e_function_state_t state)
{
    e_osal_return_t    osal_ret = OSAL_RETURN_OK;
    int                os_ret;
    struct timespec    time_stamp;
    uint32_t           elapsed_time = 0U;

    while ((state != g_function_active) && (OSAL_RETURN_OK == osal_ret) && (TEST_CALLBACK_ACTIVE_TIMEOUT > elapsed_time))
    {
        if (FUNCTION_RUNNING == state)
        {
            if (FUNCTION_RUN_AFTER == g_function_active)
            {
                osal_ret = OSAL_RETURN_FAIL;
                break;
            }
        }
        os_ret = clock_gettime(CLOCK_MONOTONIC, &time_stamp);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            time_stamp.tv_nsec += (long)TEST_CALLBACK_MS_SLEEP_TIME;
            elapsed_time++;
            if (TEST_MAX_TV_NSEC < time_stamp.tv_nsec)
            {
                time_stamp.tv_sec++;
                time_stamp.tv_nsec = time_stamp.tv_nsec - (TEST_MAX_TV_NSEC);
            }
            os_ret = clock_nanosleep(CLOCK_MONOTONIC, (int)TIMER_ABSTIME, &time_stamp, NULL);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
        }
    }

    return osal_ret;
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------- Internal function(Allocate Memory) -----------------------------------------*/
e_osal_return_t allocate_setup_data_memory(osal_memory_manager_handle_t* hndl_mmngr,
                                           uint32_t *hw_addr,
                                           osal_memory_buffer_handle_t* p_buffer_obj)
{
    e_osal_return_t osal_ret;
    osal_memory_buffer_handle_t local_buffer_obj = OSAL_MEMORY_BUFFER_HANDLE_INVALID;
    osal_memory_manager_handle_t local_hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t data;
    void *cpu_ptr[TEST_NUM_OF_BUFFERS];
    size_t size = 0;
    uintptr_t local_hw_addr = 0;
    st_osal_mmngr_config_t config;
    size_t num_of_max_entry = 0;

    /* Open Memory manager */
    (void)R_OSAL_MmngrGetOsalMaxConfig(&config);
    osal_ret = R_OSAL_MmngrOpen(&config, &local_hndl_mmngr);
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrOpen() error.", osal_ret_to_str(osal_ret));
    }
    else
    {
        *hndl_mmngr = local_hndl_mmngr;

        /* Allocate buffer for Display list */
        for(size_t i = 0; i < TEST_NUM_OF_BUFFERS; i++)
        {
            osal_ret = R_OSAL_MmngrAlloc(local_hndl_mmngr, TEST_BUFFER_SIZE, TEST_BUFFER_ALIGNMENT, &local_buffer_obj);
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_TEST_ERR("R_OSAL_MmngrAlloc() error.", osal_ret_to_str(osal_ret));
                break;
            }
            *(p_buffer_obj + i) = local_buffer_obj;

            /* Get HW address of buffer */
            osal_ret = R_OSAL_MmngrGetHwAddr(local_buffer_obj, OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY, &local_hw_addr);
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_TEST_ERR("R_OSAL_MmngrGetHwAddr() error.", osal_ret_to_str(osal_ret));
                break;
            }
            hw_addr[i] = (uint32_t)local_hw_addr;

            /* Get CPU accessible pointer of the buffer */
            cpu_ptr[i] = NULL;
            osal_ret = R_OSAL_MmngrGetCpuPtr(local_buffer_obj, &cpu_ptr[i]);
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_TEST_ERR("R_OSAL_MmngrGetCpuPtr() error.", osal_ret_to_str(osal_ret));
                break;
            }

            /* Write display list data to the beginning of the buffer */
            data = TEST_DUMMY_BUFFER_DATA[i];
            (void)memcpy(cpu_ptr[i], (void*)&data, sizeof(uint32_t));
            osal_ret = R_OSAL_MmngrGetSize(local_buffer_obj, &size);
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_TEST_ERR("R_OSAL_MmngrGetSize() error.", osal_ret_to_str(osal_ret));
                break;
            }

            osal_ret = R_OSAL_MmngrFlush(local_buffer_obj, 0 , size);
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_TEST_ERR("R_OSAL_MmngrFlush() error.", osal_ret_to_str(osal_ret));
                break;
            }
        }

        if (OSAL_RETURN_OK != osal_ret)
        {
            (void)R_OSAL_MmngrClose(local_hndl_mmngr);
        }
    }

    return osal_ret;
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------- Internal function(Deallocate Memory) ----------------------------------------*/
e_osal_return_t deallocate_data_memory(osal_memory_manager_handle_t hndl_mmngr,
                                       osal_memory_buffer_handle_t* buffer_obj)
{
    e_osal_return_t osal_ret;

    for(int i = TEST_NUM_OF_BUFFERS - 1; i >= 0; i--)
    {
        osal_ret = R_OSAL_MmngrDealloc(hndl_mmngr, buffer_obj[i]);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_MmngrDealloc() error.", osal_ret_to_str(osal_ret));
            break;
        }
    }

    osal_ret = R_OSAL_MmngrClose(hndl_mmngr);
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrClose() error.", osal_ret_to_str(osal_ret));
    }

    return osal_ret;
}
/*--------------------------------------------------------------------------------------------------------------------*/
