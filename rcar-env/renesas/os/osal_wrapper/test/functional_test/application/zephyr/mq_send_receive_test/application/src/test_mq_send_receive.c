#include <pthread.h>
#include "test_mq_send_receive.h"

/* Macros to wrap the following functions:
    - Lock interrupts.              (mq_irq_lock)
    - Unlock interrupts.            (mq_irq_unlock)
    - Yield the current thread.     (mq_k_yield)
   In Zephyr, irq_lock, irq_unlock and k_yield are set respectively.
   Set the corresponding functions for QNX. (Initiail setting of each macro is 0.)
*/
#ifdef CONFIG_MQ_TEST_ZEPHYR
#define mq_irq_lock         irq_lock()
#define mq_irq_unlock(key)  irq_unlock(key)
#define mq_k_yield          k_yield();
#else
#define mq_irq_lock         0
#define mq_irq_unlock(key)  0
#define mq_k_yield          0
#endif

#define DEBUG_OUTPUT_FLAG 0
#if DEBUG_OUTPUT_FLAG
#define DEBUG_PRINT(...) {printf("%s(%d):", __func__, __LINE__); printf(__VA_ARGS__);}
#else
#define DEBUG_PRINT(...) {}
#endif

#define OSA_EXEC_FLAG 1
#if OSA_EXEC_FLAG
#define OSAL_EXEC(ret, exp, code) {                             \
    ret = code;                                                 \
    if (exp != ret)                                             \
    {                                                           \
        printf("%s(%d) Error: exp: %d, ret:%d\n", __func__, __LINE__, exp, ret);    \
    }                                                           \
}
#else
#define OSAL_EXEC(ret, exp, code) { ret = code; }
#endif

/**********************************************************/
/*                     global data                        */
/**********************************************************/
const char* osal_mq_device_name[] =
{
    "ims_01",
    "ims_00",
    "imr_00",
    "imr_01"
};

const char* osal_mq_result_process_string[] =
{
    "R",        /* Receive */
    "S",        /* Send */
    "Re"        /* Reset */
};

const char* osal_mq_result_context_string[] =
{
    "T, H",        /* Thread, High */
    "T, M",        /* Thread, Middle */
    "T, L",        /* Thread, Low */
    "I, H",        /* ISR, High */
    "I, M",        /* ISR, Middle */
    "I, L"         /* ISR, Low */
};

const osal_thread_id_t osal_mq_thread_id[] = 
{
    OSAL_THREAD_OSAL_00,
    OSAL_THREAD_OSAL_01,
    OSAL_THREAD_OSAL_02,
    OSAL_THREAD_OSAL_03,
    OSAL_THREAD_OSAL_04,
    OSAL_THREAD_OSAL_05,
    OSAL_THREAD_OSAL_06,
    OSAL_THREAD_OSAL_07,
    OSAL_THREAD_OSAL_08,
    OSAL_THREAD_OSAL_09,
    OSAL_THREAD_IMR_TASK01,
    OSAL_THREAD_IMR_TASK02
};

osal_thread_handle_t g_thread_handle[OSAL_MQ_PROCESS_NUM] = {
    OSAL_THREAD_HANDLE_INVALID,
    OSAL_THREAD_HANDLE_INVALID,
    OSAL_THREAD_HANDLE_INVALID,
    OSAL_THREAD_HANDLE_INVALID
};

osal_device_handle_t g_dev_handle[OSAL_MQ_PROCESS_NUM] = {0};
st_arg_mq_context_t g_context_userarg[OSAL_MQ_PROCESS_NUM] = {0};
st_arg_mq_context_t g_mq_msg_arg;
osal_mutex_handle_t g_mutex_sync_create;
osal_mutex_handle_t g_mutex_test_start;
osal_cond_handle_t g_cond_sync_create;
osal_cond_handle_t g_cond_test_start;
uint32_t init_clear_data = 0;
uint32_t init_data[3] = {0};
osal_memory_manager_handle_t g_memory_handle = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
osal_memory_buffer_handle_t g_buffer_handle[TEST_NUM_OF_BUFFERS];
uint32_t hw_addr[TEST_NUM_OF_BUFFERS] = {0};
bool g_test_start_cond_flag[OSAL_MQ_PROCESS_NUM] = {0};
uint32_t g_test_result[OSAL_MQ_PROCESS_NUM] = {0};

/**********************************************************/
/*                 prototype function                     */
/**********************************************************/
e_osal_return_t mq_test_main_thread(void *arg);
static e_osal_return_t mq_device_initialize();
static e_osal_return_t mq_device_deinitialize();
static int32_t mq_thread_create(uint32_t index, uint32_t priority, uint32_t process);
static int32_t mq_interrupt_register(uint32_t index, uint32_t priority, uint32_t process);
e_osal_return_t mq_test_thread(void *user_arg);
void mq_test_interrupt_callback(osal_device_handle_t dev_handle, uint64_t irq_channel, void * irq_arg);
static e_osal_return_t mq_test_send(st_arg_mq_context_t *arg);
static e_osal_return_t mq_test_receive(st_arg_mq_context_t *arg);
static e_osal_return_t mq_test_reset(st_arg_mq_context_t *arg);
static int32_t mq_test_setup_buffer(osal_memory_manager_handle_t* p_memory_handle, osal_memory_buffer_handle_t* p_buffer_handle);
static int32_t mq_test_setup_imr_register(uint32_t index, osal_device_handle_t dev_handle, osal_memory_buffer_handle_t buffer_handle);
static int32_t mq_test_get_axi_id_list_from_dev_name(const char* device_name, osal_axi_bus_id_t** axi_id_list);

/**
 * @brief This function is main of MQ test application.
 * - Intialize by using R_OSAL_Initialize.
 * - Setup the device for interrupt by using mq_device_initialize.
 * - Determin the test pattern from funtion, context and priority.
 * - Output the test pattern.
 * - Clear the arguments for main test thread.
 * - Set the main test thread information.
 * - Wake up the main test thread by using R_OSAL_ThreadCreate.
 * - Wait until singal is sent after the process of the main test thread is completed by using R_OSAL_ThreadJoin.
 * - If the error occur, output the test pattern.
 * - Close and power off the device for interrupt by using mq_device_deinitialize.
 * - Deinitialize by using R_OSAL_Deinitialize.
 *
 * @param[in] argc unused.
 * @param[in] argv unused.
 * @return 0
 */
int main(int argc, char *argv[])
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint32_t idx_process[OSAL_MQ_PROCESS_NUM] = {0};
    uint32_t idx_pri_context[OSAL_MQ_PROCESS_NUM] = {0};
    int64_t thread_ret;
    st_arg_main_thread_t main_arg;
    st_osal_thread_config_t main_config;
    osal_thread_handle_t main_thread_handle = OSAL_THREAD_HANDLE_INVALID;

    (void)argc;
    (void)argv;

    (void)printf("================== mq_message_test start ==================\n");

    /* Initialize */
    OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_Initialize());
    OSAL_EXEC(osal_ret, OSAL_RETURN_OK, mq_device_initialize());

    /* MQ process loop (4th) */
    for (idx_process[0] = 0; OSAL_MQ_FUNCTION_MAX > idx_process[0]; idx_process[0]++)
    {
        for (idx_process[1] = 0; OSAL_MQ_FUNCTION_MAX > idx_process[1]; idx_process[1]++)
        {
            for (idx_process[2] = 0; OSAL_MQ_FUNCTION_MAX > idx_process[2]; idx_process[2]++)
            {
                for (idx_process[3] = 0; OSAL_MQ_FUNCTION_MAX > idx_process[3]; idx_process[3]++)
                {
                    /* MQ priority loop */
                    for (idx_pri_context[0] = 0; OSAL_MQ_CONTEXT_MAX > idx_pri_context[0]; idx_pri_context[0]++)
                    {
                        if ((OSAL_MQ_FUNCTION_RESET == idx_process[0]) && (OSAL_MQ_CONTEXT_ISR_HIGH <= idx_pri_context[0]))
                        {
                            /* do nothing because isr reset deny*/
                        }
                        else
                        {
                            for (idx_pri_context[1] = 0; OSAL_MQ_CONTEXT_MAX > idx_pri_context[1]; idx_pri_context[1]++)
                            {
                                if ((OSAL_MQ_FUNCTION_RESET == idx_process[1]) && (OSAL_MQ_CONTEXT_ISR_HIGH <= idx_pri_context[1]))
                                {
                                    /* do nothing because isr reset deny*/
                                }
                                else
                                {
                                    for (idx_pri_context[2] = 0; OSAL_MQ_CONTEXT_MAX > idx_pri_context[2]; idx_pri_context[2]++)
                                    {
                                        if ((OSAL_MQ_FUNCTION_RESET == idx_process[2]) && (OSAL_MQ_CONTEXT_ISR_HIGH <= idx_pri_context[2]))
                                        {
                                            /* do nothing because isr reset deny*/
                                        }
                                        else
                                        {
                                            for (idx_pri_context[3] = 0; OSAL_MQ_CONTEXT_MAX > idx_pri_context[3]; idx_pri_context[3]++)
                                            {
                                                if ((OSAL_MQ_FUNCTION_RESET == idx_process[3]) && (OSAL_MQ_CONTEXT_ISR_HIGH <= idx_pri_context[3]))
                                                {
                                                    /* do nothing because isr reset deny*/
                                                }
                                                else
                                                {
                                                    printf("pattern 1:%s(%s), 2:%s(%s), 3:%s(%s), 4:%s(%s)\n",
                                                        osal_mq_result_process_string[idx_process[0]], osal_mq_result_context_string[idx_pri_context[0]],
                                                        osal_mq_result_process_string[idx_process[1]], osal_mq_result_context_string[idx_pri_context[1]],
                                                        osal_mq_result_process_string[idx_process[2]], osal_mq_result_context_string[idx_pri_context[2]],
                                                        osal_mq_result_process_string[idx_process[3]], osal_mq_result_context_string[idx_pri_context[3]]
                                                    );

                                                    /* create mq test main thread */
                                                    memset(g_context_userarg, 0, sizeof(st_arg_mq_context_t)*OSAL_MQ_PROCESS_NUM);
                                                    memset(&main_arg, 0, sizeof(st_arg_main_thread_t));
                                                    memcpy(main_arg.idx_process, idx_process, sizeof(uint32_t)*OSAL_MQ_PROCESS_NUM);
                                                    memcpy(main_arg.idx_pri_context, idx_pri_context, sizeof(uint32_t)*OSAL_MQ_PROCESS_NUM);
                                                    main_config.func = mq_test_main_thread;
                                                    main_config.userarg = &main_arg;
                                                    main_config.priority = OSAL_THREAD_PRIORITY_TYPE1;
                                                    main_config.task_name = "mq_test_main_thread";
                                                    main_config.stack_size = THREAD_STACK_SIZE;
                                                    DEBUG_PRINT("create mq test main thread\n");
                                                    OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_ThreadCreate(&main_config, OSAL_THREAD_IMR_TASK00, &main_thread_handle));

                                                    /* wait the main thread process */
                                                    DEBUG_PRINT("join mq test main thread\n");
                                                    OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_ThreadJoin(main_thread_handle, &thread_ret));

                                                    if (OSAL_RETURN_OK != osal_ret)
                                                    {
                                                        break;
                                                    }
                                                }//if ((OSAL_MQ_FUNCTION_RESET == idx_process[3]) && (OSAL_MQ_CONTEXT_ISR_HIGH <= idx_pri_context[3]))
                                            }//for (idx_pri_context[3] = 0; OSAL_MQ_CONTEXT_MAX > idx_pri_context[3]; idx_pri_context[3]++)
                                            if (OSAL_RETURN_OK != osal_ret)
                                            {
                                                break;
                                            }
                                        }//if ((OSAL_MQ_FUNCTION_RESET == idx_process[2]) && (OSAL_MQ_CONTEXT_ISR_HIGH <= idx_pri_context[2]))
                                    }//for (idx_pri_context[2] = 0; OSAL_MQ_CONTEXT_MAX > idx_pri_context[2]; idx_pri_context[2]++)

                                    if (OSAL_RETURN_OK != osal_ret)
                                    {
                                        break;
                                    }
                                }//if ((OSAL_MQ_FUNCTION_RESET == idx_process[1]) && (OSAL_MQ_CONTEXT_ISR_HIGH <= idx_pri_context[1]))
                            }//for (idx_pri_context[1] = 0; OSAL_MQ_CONTEXT_MAX > idx_pri_context[1]; idx_pri_context[1]++)

                            if (OSAL_RETURN_OK != osal_ret)
                            {
                                break;
                            }
                        }//if ((OSAL_MQ_FUNCTION_RESET == idx_process[0]) && (OSAL_MQ_CONTEXT_ISR_HIGH <= idx_pri_context[0]))
                    }//for (idx_pri_context[0] = 0; OSAL_MQ_CONTEXT_MAX > idx_pri_context[0]; idx_pri_context[0]++)

                    if (OSAL_RETURN_OK != osal_ret)
                    {
                        printf("DeinitError ret:(%d)1:%s(%s), 2:%s(%s), 3:%s(%s), 4:%s(%s)\n",
                            osal_ret,
                            osal_mq_result_process_string[idx_process[0]], osal_mq_result_context_string[idx_pri_context[0]],
                            osal_mq_result_process_string[idx_process[1]], osal_mq_result_context_string[idx_pri_context[1]],
                            osal_mq_result_process_string[idx_process[2]], osal_mq_result_context_string[idx_pri_context[2]],
                            osal_mq_result_process_string[idx_process[3]], osal_mq_result_context_string[idx_pri_context[3]]
                        );
                        break;
                    }
                }//for (idx_process[3] = 0; OSAL_MQ_FUNCTION_MAX > idx_process[3]; idx_process[3]++)
                if (OSAL_RETURN_OK != osal_ret)
                {
                    break;
                }
            }//for (idx_process[2] = 0; OSAL_MQ_FUNCTION_MAX > idx_process[2]; idx_process[2]++)
            if (OSAL_RETURN_OK != osal_ret)
            {
                break;
            }
        }//for (idx_process[1] = 0; OSAL_MQ_FUNCTION_MAX > idx_process[1]; idx_process[1]++)
        if (OSAL_RETURN_OK != osal_ret)
        {
            break;
        }
    }//for (idx_process[0] = 0; OSAL_MQ_FUNCTION_MAX > idx_process[0]; idx_process[0]++)
    /* deinitialize */
    DEBUG_PRINT("deinitialize, test main thread\n");
    
    OSAL_EXEC(osal_ret, OSAL_RETURN_OK, mq_device_deinitialize());
    OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_Deinitialize());

    (void)printf("================== mq_message_test finish ==================\n");

    return 0;
}

/**
 * @brief This function is to setup the test thread.
 * - Set the thread information.
 * - Wake up the mq test thread by using R_OSAL_ThreadCreate.
 * - Wait the signal is sent after preperation of mq test thread is completed by using R_OSAL_ThsyncCondWaitForTimePeriod.
 *
 * @param[in] index index of the test process.
 * @param[in] priority priority of interrupt.
 * @param[in] process send or receive.
 * @return OSAL_MQ_TEST_RESULT_OK on success
 * @return OSAL_MQ_TEST_RESULT_FAIL on failure
 */
static int32_t mq_thread_create(uint32_t index, uint32_t priority, uint32_t process)
{
    int32_t app_ret = OSAL_MQ_TEST_RESULT_OK;
    int32_t pri_index = 0;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_thread_config_t config;
    osal_milli_sec_t time = 1000;

    g_context_userarg[index].mq_handle = g_mq_msg_arg.mq_handle;
    g_context_userarg[index].time = (osal_milli_sec_t)TIMEOUT_MS;
    g_context_userarg[index].function = process;
    g_context_userarg[index].index = index;
    config.func       = mq_test_thread;
    config.userarg    = (void *)&g_context_userarg[index];
    config.priority   = priority;
    config.task_name  = "mq_test";
    config.stack_size = THREAD_STACK_SIZE;

    DEBUG_PRINT("create test thread index %d\n", index);
    if (OSAL_MQ_CONTEXT_THREAD_LOW < priority)
    {
        app_ret = OSAL_MQ_TEST_RESULT_FAIL;
    }
    else
    {
        pri_index = priority;
    }

    if (OSAL_MQ_TEST_RESULT_FAIL != app_ret)
    {
        OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_ThsyncMutexLockForTimePeriod(g_mutex_sync_create, time));
        OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_ThreadCreate(&config, osal_mq_thread_id[3*index+pri_index], &g_thread_handle[index]));
        if (OSAL_RETURN_OK != osal_ret)
        {
            app_ret = OSAL_MQ_TEST_RESULT_FAIL;
        }
        else
        {
            OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_ThsyncCondWaitForTimePeriod(g_cond_sync_create, g_mutex_sync_create, time));
        }
        OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_ThsyncMutexUnlock(g_mutex_sync_create));
    }

    return app_ret;
}

/**
 * @brief This function is to setup interrupt hardware
 * - Clear the interrupt register by using R_OSAL_IoRead32.
 * - Allocate and setup buffer by using setup_buffer.
 * - Get the hardware address by using mq_test_setup_imr_register.
 * - Register the interrupt information by using R_OSAL_InterruptRegisterIsr.
 * - Set the priority of interrupt by using R_OSAL_InterruptSetIsrPriority.
 *
 * @param[in] index index of the test process.
 * @param[in] priority priority of interrupt.
 * @param[in] process send or receive.
 * @return OSAL_MQ_TEST_RESULT_OK on success
 * @return OSAL_MQ_TEST_RESULT_FAIL on failure
 */
static int32_t mq_interrupt_register(uint32_t index, uint32_t priority, uint32_t process)
{
    int32_t app_ret = OSAL_MQ_TEST_RESULT_OK;
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /* interrupt register clear */
    OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_IoRead32(g_dev_handle[index], TEST_REG_CLEAR_REGISTER, &init_clear_data));
    OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_IoRead32(g_dev_handle[index], TEST_IMR_REG_DLSAR, &init_data[0]));
    OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_IoRead32(g_dev_handle[index], TEST_IMR_REG_IMR, &init_data[1]));
    OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_IoRead32(g_dev_handle[index], TEST_IMR_REG_CR, &init_data[2]));

    /* setup buffer */
    app_ret = mq_test_setup_buffer(&g_memory_handle, &g_buffer_handle[index]);
    if (OSAL_MQ_TEST_RESULT_OK == app_ret)
    {
        app_ret = mq_test_setup_imr_register(index, g_dev_handle[index], g_buffer_handle[index]);
    }

    if (OSAL_MQ_TEST_RESULT_OK == app_ret)
    {
        /* interrupt register callback */
        g_context_userarg[index].mq_handle = g_mq_msg_arg.mq_handle;
        g_context_userarg[index].time = 0;
        g_context_userarg[index].function = process;
        g_context_userarg[index].priority = priority;
        g_context_userarg[index].index = index;
        OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_InterruptRegisterIsr(g_dev_handle[index], 0, priority, mq_test_interrupt_callback, (void *)&g_context_userarg[index]));

        /* set pritority of interrupt */
        OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_InterruptSetIsrPriority(g_dev_handle[index], 0, priority));
    }

    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = OSAL_MQ_TEST_RESULT_FAIL;
    }
    else
    {
        /* do nothing */
    }
    return app_ret;
}

/**
 * @brief This function is to execute mq test of thread context.
 * - Send signal to main test thread by using R_OSAL_ThsyncCondSignal.
 * - Wait until the signal that start test is sent from main test thread by using R_OSAL_ThsyncMutexLockForTimePeriod.
 * - Execute the function judging from argument(send/receive/reset).
 * - If argument is invalid, do nothing.
 *
 * @param[in] user_arg This parameter contains OSAL mq handle, test process and function passed from the parent thread.
 * @return OSAL_RETURN_OK on success
 * @return not OSAL_RETURN_OK on failure
 */
e_osal_return_t mq_test_thread(void *user_arg)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_arg_mq_context_t *arg = (st_arg_mq_context_t*)user_arg;
    osal_milli_sec_t time = 1000;

    DEBUG_PRINT("[S] start thread index = %d, function = %d\n", arg->index, arg->function);

    /* condition signal */
    OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_ThsyncMutexLockForTimePeriod(g_mutex_sync_create, (osal_milli_sec_t)time));
    DEBUG_PRINT("start cond signal index = %d\n", arg->index);
    OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_ThsyncCondSignal(g_cond_sync_create));
    OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_ThsyncMutexUnlock(g_mutex_sync_create));

    /* condition wait */
    DEBUG_PRINT("start cond wait index = %d\n", arg->index);
    OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_ThsyncMutexLockForTimePeriod(g_mutex_test_start, (osal_milli_sec_t)time));
    if (false == g_test_start_cond_flag[arg->index])
    {
        OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_ThsyncCondWaitForTimePeriod(g_cond_test_start, g_mutex_test_start, (osal_milli_sec_t)time));
    }
    OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_ThsyncMutexUnlock(g_mutex_test_start));

    switch(arg->function)
    {
    case OSAL_MQ_FUNCTION_RECEIVE:
        osal_ret = mq_test_receive(arg);
        break;
    case OSAL_MQ_FUNCTION_SEND:
        osal_ret = mq_test_send(arg);
        break;
    case OSAL_MQ_FUNCTION_RESET:
        osal_ret = mq_test_reset(arg);
        break;
    default:
        break;
    }

    DEBUG_PRINT("[E] end thread result : %d\n", osal_ret);
    return osal_ret;
}

/**
 * @brief This function is to execute mq test of isr context.
 * - Set device status clear register by using R_OSAL_IoWrite32.
 * - Disable enabled interrupt by using R_OSAL_InterruptDisableIsr.
 * - Execute the function judging from argument(send/receive).
 * - If argument is invalid, set return value to OSAL_RETURN_FAIL.
 * - Set global variable to the function result.
 *
 * @param[in] dev_handle the handle of device
 * @param[in] irq_channel the interrupt channel
 * @param[in] irq_arg the callback function argument which is a void pointer type
 */
void mq_test_interrupt_callback(osal_device_handle_t dev_handle, uint64_t irq_channel, void * user_arg)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_arg_mq_context_t *arg = (st_arg_mq_context_t*)user_arg;

    DEBUG_PRINT("[S] start interrupt index = %d, function = %d\n", arg->index, arg->function);
    OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_InterruptDisableIsr(dev_handle, 0));
    OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_IoWrite32(dev_handle, TEST_REG_CLEAR_REGISTER, TEST_CLEAR_REGISTER_VALUE));

    switch(arg->function)
    {
    case OSAL_MQ_FUNCTION_RECEIVE:
        osal_ret = mq_test_receive(arg);
        break;
    case OSAL_MQ_FUNCTION_SEND:
        osal_ret = mq_test_send(arg);
        break;
    default:
        osal_ret = OSAL_RETURN_FAIL;
        break;
    }

    g_test_result[arg->index] = (uint32_t)osal_ret;
    DEBUG_PRINT("[E] send interrupt result data(%d) to main thread\n", osal_ret);
}

/**
 * @brief This function is to send the message.
 * - Send the message by using R_OSAL_MqSendForTimePeriod.
 *
 * @param[in] arg This parameter contains OSAL mq handle, test process and function passed from the parent thread.
 * @return OSAL_RETURN_OK on success
 * @return not OSAL_RETURN_OK on failure
 */
static e_osal_return_t mq_test_send(st_arg_mq_context_t *arg)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint32_t send_msg = OSAL_MQ_SEND_MSG_DATA;

    DEBUG_PRINT("[S] start send process\n");
    osal_ret = R_OSAL_MqSendForTimePeriod(arg->mq_handle, arg->time, &send_msg, OSAL_MQ_MSG_DATA_LEN);
    DEBUG_PRINT("[E] send data(%d) to receive context, send result:%d\n", send_msg, osal_ret);

    return osal_ret;
}

/**
 * @brief This function is to receive the message.
 * - Receive the message by using R_OSAL_MqReceiveForTimePeriod.
 * - Check that the receive data and the expected data are the same.
 *
 * @param[in] arg This parameter contains OSAL mq handle, test process and function passed from the parent thread.
 * @return OSAL_RETURN_OK on success
 * @return not OSAL_RETURN_OK on failure
 */
static e_osal_return_t mq_test_receive(st_arg_mq_context_t *arg)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_mq_handle_t* local_mq_handle = (osal_mq_handle_t*)arg->mq_handle;
    uint32_t receive_msg = 0x0;

    DEBUG_PRINT("[S] start receive process\n");
    osal_ret = R_OSAL_MqReceiveForTimePeriod(arg->mq_handle, arg->time, &receive_msg, OSAL_MQ_MSG_DATA_LEN);

    if (OSAL_RETURN_OK != osal_ret)
    {
        /* do nothing */
    }
    else if (OSAL_MQ_SEND_MSG_DATA != receive_msg)
    {
        osal_ret = OSAL_RETURN_FAIL;
        DEBUG_PRINT("the send data(%d) not equal to the receive data(%d)\n", OSAL_MQ_SEND_MSG_DATA, receive_msg);
    }
    else
    {
        /* send and receive ok */
        DEBUG_PRINT("the send data(%d) equal to the receive data(%d)\n", OSAL_MQ_SEND_MSG_DATA, receive_msg);
    }
    DEBUG_PRINT("[E] receive result:%d\n", osal_ret);
    return osal_ret;
}

/**
 * @brief This function is to reset the message queue.
 * - Reset the message queue by using R_OSAL_MqReset.
 *
 * @param[in] arg This parameter contains OSAL mq handle, test process and function passed from the parent thread.
 * @return OSAL_RETURN_OK on success
 * @return not OSAL_RETURN_OK on failure
 */
static e_osal_return_t mq_test_reset(st_arg_mq_context_t *arg)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_mq_handle_t* local_mq_handle = (osal_mq_handle_t*)arg->mq_handle;

    DEBUG_PRINT("[S] start reset process\n");
    osal_ret = R_OSAL_MqReset(local_mq_handle);
    DEBUG_PRINT("[E] reset result:%d\n", osal_ret);
    return osal_ret;
}

/**
 * @brief This function is to set data to the buffer object.
 * - Allocate buffer by using R_OSAL_MmngrAlloc.
 * - Get CPU pointer of the buffer by using R_OSAL_MmngrGetCpuPtr.
 * - Write data to the buffer by using memcpy.
 * - Get size of the buffer by using R_OSAL_MmngrGetSize.
 * - Synchronizes CPU memory of the buffer to hardware memory by using R_OSAL_MmngrFlush.
 *
 * @param[in, out] p_memory_handle the handle of memory manager
 * @param[in, out] p_buffer_handle the handle of buffer
 * @return OSAL_MQ_TEST_RESULT_OK on success
 * @return OSAL_MQ_TEST_RESULT_FAIL on failure
 */
static int32_t mq_test_setup_buffer(osal_memory_manager_handle_t* p_memory_handle, osal_memory_buffer_handle_t* p_buffer_handle)
{
    int32_t app_ret = OSAL_MQ_TEST_RESULT_OK;
    e_osal_return_t osal_ret;
    void* cpu_ptr = NULL;
    uint32_t* int_cpu_ptr;
    size_t size = 0;
    uint32_t dummy_data1 = TEST_DUMMY_BUFFER_DATA_1;
    uint32_t dummy_data2 = TEST_DUMMY_BUFFER_DATA_2;
    osal_memory_buffer_handle_t local_buffer_handle;

    OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_MmngrAlloc(g_memory_handle, TEST_BUFFER_SIZE, TEST_BUFFER_ALIGNMENT, &local_buffer_handle));
    if (OSAL_RETURN_OK != osal_ret)
    {
        app_ret = OSAL_MQ_TEST_RESULT_FAIL;
    }

    *p_buffer_handle = local_buffer_handle;
    OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_MmngrGetCpuPtr(local_buffer_handle, &cpu_ptr));
    if (OSAL_RETURN_OK != osal_ret)
    {
        app_ret = OSAL_MQ_TEST_RESULT_FAIL;
    }

    if(0 == app_ret)
    {
        int_cpu_ptr = (uint32_t*)cpu_ptr;

        (void)memcpy((void *)&int_cpu_ptr[TEST_BUFFER_ADDR_INDEX_1], &dummy_data1, sizeof(dummy_data1));

        (void)memcpy((void *)&int_cpu_ptr[TEST_BUFFER_ADDR_INDEX_2], &dummy_data2, sizeof(dummy_data2));

        OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_MmngrGetSize(local_buffer_handle, &size));
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = OSAL_MQ_TEST_RESULT_FAIL;
        }
    }
    if(0 == app_ret)
    {
        OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_MmngrFlush(local_buffer_handle, 0, size));
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = OSAL_MQ_TEST_RESULT_FAIL;
        }
    }

    return app_ret;
}

/**
 * @brief This function is to setup the IMR register.
 * - Get axi bus id of sample device by using mq_test_get_axi_id_list_from_dev_name.
 * - Get hardware address of the buffer by using R_OSAL_MmngrGetHwAddr.
 *
 * @param[in] dev_handle the handle of device
 * @param[in] buffer_handle the handle of buffer
 * @return OSAL_MQ_TEST_RESULT_OK on success
 * @return OSAL_MQ_TEST_RESULT_FAIL on failure
 */
static int32_t mq_test_setup_imr_register(uint32_t index, osal_device_handle_t dev_handle, osal_memory_buffer_handle_t buffer_handle)
{
    int32_t app_ret = OSAL_MQ_TEST_RESULT_OK;
    e_osal_return_t osal_ret;
    uintptr_t local_hw_addr = 0;
    osal_axi_bus_id_t* axi_id_list_from_device_name = NULL;

    app_ret = mq_test_get_axi_id_list_from_dev_name(osal_mq_device_name[index], &axi_id_list_from_device_name);
    if(OSAL_MQ_TEST_RESULT_OK != app_ret)
    {
        /* Do nothing */
    }
    else
    {
        if(NULL == axi_id_list_from_device_name)
        {
            app_ret = OSAL_MQ_TEST_RESULT_FAIL;
        }
        else
        {
            OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_MmngrGetHwAddr(buffer_handle, axi_id_list_from_device_name[0], &local_hw_addr));
            if(OSAL_RETURN_OK != osal_ret)
            {
                app_ret = OSAL_MQ_TEST_RESULT_FAIL;
            }
            else
            {
                hw_addr[index] = local_hw_addr;
            }
        }
    }
    if(NULL != axi_id_list_from_device_name)
    {
        free((void*)axi_id_list_from_device_name);
    }
    return app_ret;
}

/**
 * @brief This function is to get and show the list of AXI bus IDs with the given sample device name
 * - Get the number of AXI bus IDs related to the given sample device name by using R_OSAL_IoGetNumOfAxiIdsForDeviceName
 * - Allocate a buffer with the size is the number of AXI bus ID above
 * - Get the AXi bus ID list by using R_OSAL_IoGetAxiIdListForDeviceName
 *
 * @param[in] device_name Device name which is used for getting the AXI bus ID list
 * @param[in, out] axi_id_list This parameter is to store the AXI bus ID list corresponding with device_name
 * @return OSAL_MQ_TEST_RESULT_OK on success
 * @return OSAL_MQ_TEST_RESULT_FAIL on failure
 */
static int32_t mq_test_get_axi_id_list_from_dev_name(const char* device_name, osal_axi_bus_id_t** axi_id_list)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int32_t app_ret = OSAL_MQ_TEST_RESULT_OK;
    size_t num_axi_id = 0;
    osal_axi_bus_id_t* local_axi_id_list = NULL;
    bool unsupport_axi_bus_device = false;

    OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_IoGetNumOfAxiIdsForDeviceName(device_name, &num_axi_id));
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = OSAL_MQ_TEST_RESULT_FAIL;
    }
    else if(0U == num_axi_id)
    {
        app_ret = OSAL_MQ_TEST_RESULT_FAIL;
        unsupport_axi_bus_device = true;
    }
    else
    {
    }

    if(OSAL_MQ_TEST_RESULT_OK == app_ret)
    {
        local_axi_id_list = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_axi_id);
        if(NULL == local_axi_id_list)
        {
            app_ret = OSAL_MQ_TEST_RESULT_FAIL;
        }
    }

    if(OSAL_MQ_TEST_RESULT_OK == app_ret)
    {
        OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_IoGetAxiIdListForDeviceName(device_name, local_axi_id_list, num_axi_id, &num_axi_id));
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = OSAL_MQ_TEST_RESULT_FAIL;
        }
    }

    if(true == unsupport_axi_bus_device)
    {
        app_ret = OSAL_MQ_TEST_RESULT_OK;
    }

    if(OSAL_MQ_TEST_RESULT_OK == app_ret)
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
 * @brief This function is to execute mq test.
 * - Create the message queue by using R_OSAL_MqCreate.
 * - Get the maximum memory configuration by using R_OSAL_MmngrGetOsalMaxConfig.
 * - Open new memory manager by using R_OSAL_MmngrOpen.
 * - Create the mutex to synchronize main thread and test thread by using R_OSAL_ThsyncMutexCreate.
 * - Create the mutex to synchronize the start of test thread by using R_OSAL_ThsyncMutexCreate.
 * - Create the condition variable to synchronize main thread and test thread by using R_OSAL_ThsyncCondCreate.
 * - Create the condition variable to synchronize the start of test thread by using R_OSAL_ThsyncCondCreate.
 * - Setup the test thread by using mq_thread_create.
 * - Register the interrupt by using mq_interrupt_register.
 * - Lock the interrupt by using mq_irq_lock.
 * - Send the signal to start test by using R_OSAL_ThsyncCondSignal.
 * - Set hardware address to display list start register by using R_OSAL_IoWrite32.
 * - Clear device interrupt mask register by using R_OSAL_IoWrite32.
 * - Set device control register by using R_OSAL_IoWrite32.
 * - Enable the registered interrupt handler by using R_OSAL_InterruptEnableIsr.
 * - Unlock the interrupt by using mq_irq_unlock.
 * - Switch the context by using mq_k_yield.
 * - Wait until singal is sent after the test complete in test thread by using R_OSAL_ThreadJoin.
 * - Get the result of interrupt callback.
 * - Disable enabled interrupt by using R_OSAL_InterruptDisableIsr.
 * - Unregister the interrupt by using R_OSAL_InterruptUnregisterIsr.
 * - Deallocate memory buffer of the buffer_handle by using R_OSAL_MmngrDealloc.
 * - Delete created cond by using R_OSAL_ThsyncCondDestroy.
 * - Delete created mutex by using R_OSAL_ThsyncMutexDestroy.
 * - Delete memory manager handle by using R_OSAL_MmngrClose.
 * - Reset the message queue by using R_OSAL_MqReset.
 * - Delete the message queue by using R_OSAL_MqDelete.
 *
 * @param[in] user_arg This parameter contains test process, context and priority passed from the parent thread.
 * @return OSAL_RETURN_OK on success
 * @return not OSAL_RETURN_OK on failure
 */
e_osal_return_t mq_test_main_thread(void *user_arg)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_return_t expect_ret = OSAL_RETURN_OK;
    e_osal_return_t test_ret = OSAL_RETURN_OK;
    int32_t  app_ret = OSAL_MQ_TEST_RESULT_OK;
    st_arg_main_thread_t *arg = (st_arg_main_thread_t*)user_arg;
    st_osal_mq_config_t mq_config = {1, 4};
    st_osal_mmngr_config_t memory_max_config;
    osal_milli_sec_t time = 1000;
    int32_t process;
    uint32_t irq_key;
    int64_t thread_ret;

    DEBUG_PRINT("start main thread\n");
    g_memory_handle = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    memset(g_buffer_handle, 0, sizeof(osal_memory_buffer_handle_t)*TEST_NUM_OF_BUFFERS);
    memset(g_test_start_cond_flag, false, sizeof(bool)*OSAL_MQ_PROCESS_NUM);
    memset(g_test_result, 0xff, sizeof(int)*OSAL_MQ_PROCESS_NUM);

    OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_MqCreate(&mq_config, OSAL_MQ_MSG_ID_01, &(g_mq_msg_arg.mq_handle)));

    OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_MmngrGetOsalMaxConfig(&memory_max_config));
    OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_MmngrOpen(&memory_max_config, &g_memory_handle));

    g_mutex_sync_create = OSAL_MUTEX_HANDLE_INVALID;
    g_mutex_test_start  = OSAL_MUTEX_HANDLE_INVALID;
    g_cond_sync_create  = OSAL_COND_HANDLE_INVALID;
    g_cond_test_start   = OSAL_COND_HANDLE_INVALID;

    OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_ThsyncMutexCreate(OSAL_MUTEX_OSAL_00, &g_mutex_sync_create));
    OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_ThsyncMutexCreate(OSAL_MUTEX_OSAL_01, &g_mutex_test_start));
    OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_ThsyncCondCreate(OSAL_COND_OSAL_00, &g_cond_sync_create));
    OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_ThsyncCondCreate(OSAL_COND_OSAL_01, &g_cond_test_start));

    process = 0;
    while ((OSAL_MQ_PROCESS_NUM > process) && (OSAL_MQ_TEST_RESULT_OK == app_ret))
    {
        switch(arg->idx_pri_context[process])
        {
        case OSAL_MQ_CONTEXT_THREAD_HIGH:
        case OSAL_MQ_CONTEXT_THREAD_MIDDLE:
        case OSAL_MQ_CONTEXT_THREAD_LOW:
            /* Thread create */
            g_thread_handle[process] = OSAL_THREAD_HANDLE_INVALID;
            DEBUG_PRINT("thread create index %d\n", process);
            app_ret = mq_thread_create(process, arg->idx_pri_context[process], arg->idx_process[process]);
            break;
        case OSAL_MQ_CONTEXT_ISR_HIGH:
        case OSAL_MQ_CONTEXT_ISR_MIDDLE:
        case OSAL_MQ_CONTEXT_ISR_LOW:
            /* Interrupt register */
            DEBUG_PRINT("register interrupt index %d\n", process);
            app_ret = mq_interrupt_register(process, arg->idx_pri_context[process], arg->idx_process[process]);
            break;
        default:
            app_ret = OSAL_MQ_TEST_RESULT_FAIL;
            break;
        }

        process++;
    }

    /* test start */
    irq_key = mq_irq_lock;
    process = 0;
    while ((OSAL_MQ_PROCESS_NUM > process) && (OSAL_MQ_TEST_RESULT_OK == app_ret))
    {
        switch(arg->idx_pri_context[process])
        {
        case OSAL_MQ_CONTEXT_THREAD_HIGH:
        case OSAL_MQ_CONTEXT_THREAD_MIDDLE:
        case OSAL_MQ_CONTEXT_THREAD_LOW:
            /* CondSignal */
            OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_ThsyncMutexLockForTimePeriod(g_mutex_test_start, time));
            DEBUG_PRINT("cond signal index %d\n", process);
            OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_ThsyncCondSignal(g_cond_test_start));
            g_test_start_cond_flag[process] = true;
            OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_ThsyncMutexUnlock(g_mutex_test_start));
            break;
        case OSAL_MQ_CONTEXT_ISR_HIGH:
        case OSAL_MQ_CONTEXT_ISR_MIDDLE:
        case OSAL_MQ_CONTEXT_ISR_LOW:
            /* write interrupt register */
            OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_IoWrite32(g_dev_handle[process], TEST_IMR_REG_DLSAR, hw_addr[process]));
            OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_IoWrite32(g_dev_handle[process], TEST_IMR_REG_IMR, TEST_IMR_IMR_VALUE));
            OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_IoWrite32(g_dev_handle[process], TEST_IMR_REG_CR, TEST_IMR_CR_VALUE));
            OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_InterruptEnableIsr(g_dev_handle[process], 0));
            break;
        default:
            app_ret = OSAL_MQ_TEST_RESULT_FAIL;
            break;
        }
        process++;
    }
    mq_irq_unlock(irq_key);
    mq_k_yield;

    /* test end */
    process = 0;
    while (OSAL_MQ_PROCESS_NUM > process)
    {
        switch(arg->idx_pri_context[process])
        {
        case OSAL_MQ_CONTEXT_THREAD_HIGH:
        case OSAL_MQ_CONTEXT_THREAD_MIDDLE:
        case OSAL_MQ_CONTEXT_THREAD_LOW:
            /* join */
            DEBUG_PRINT("[S] Thread join\n");
            OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_ThreadJoin(g_thread_handle[process], &thread_ret));
            if (OSAL_RETURN_OK != osal_ret)
            {
                DEBUG_PRINT("thread join error(%d) i:%d, p:%d, t:%lld\n", osal_ret, process, arg->idx_pri_context[process], thread_ret);
            }
            test_ret = (e_osal_return_t)thread_ret;
            break;
        case OSAL_MQ_CONTEXT_ISR_HIGH:
        case OSAL_MQ_CONTEXT_ISR_MIDDLE:
        case OSAL_MQ_CONTEXT_ISR_LOW:
            while(1)
            {
                if (0xff != g_test_result[process])
                {
                    test_ret = (e_osal_return_t)g_test_result[process];
                    break;
                }
                else
                {
                    DEBUG_PRINT("irq process not complete\n");
                }
                OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_ThreadSleepForTimePeriod(1));
            }
            DEBUG_PRINT("main thread receive data from isr context : %d\n", test_ret);
            /* interrupt unregister */
            OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_InterruptDisableIsr(g_dev_handle[process], 0));
            OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_InterruptUnregisterIsr(g_dev_handle[process], 0, mq_test_interrupt_callback));
            break;
        default:
            app_ret = OSAL_MQ_TEST_RESULT_FAIL;
            break;
        }
        process++;

        if ((OSAL_RETURN_OK != test_ret) && (OSAL_RETURN_OK == expect_ret))
        {
            expect_ret = test_ret;
        }
    }

    /* deinitialize */
    process = OSAL_MQ_PROCESS_NUM-1;
    while (0 <= process)
    {
        switch (arg->idx_pri_context[process])
        {
        case OSAL_MQ_CONTEXT_ISR_HIGH:
        case OSAL_MQ_CONTEXT_ISR_MIDDLE:
        case OSAL_MQ_CONTEXT_ISR_LOW:
            OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_MmngrDealloc(g_memory_handle, g_buffer_handle[process]));
            break;
        default:
            break;
        }
        process--;
    }

    OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_ThsyncCondDestroy(g_cond_sync_create));
    OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_ThsyncCondDestroy(g_cond_test_start));
    OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_ThsyncMutexDestroy(g_mutex_sync_create));
    OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_ThsyncMutexDestroy(g_mutex_test_start));
    OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_MmngrClose(g_memory_handle));
    OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_MqReset(g_mq_msg_arg.mq_handle));
    OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_MqDelete(g_mq_msg_arg.mq_handle));

    return osal_ret;
}

/**
 * @brief This function is to setup the devive for interrupt.
 * - Open the device by using R_OSAL_IoDeviceOpen.
 * - Power on the opened device by using R_OSAL_PmSetRequiredState.
 *
 * @return OSAL_RETURN_OK on success
 * @return not OSAL_RETURN_OK on failure
 */
static e_osal_return_t mq_device_initialize()
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint32_t index = 0;

    for (index = 0; index < OSAL_MQ_PROCESS_NUM; index++)
    {
        /* device open */
        OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_IoDeviceOpen(osal_mq_device_name[index], &g_dev_handle[index]));
        /* power on */
        OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_PmSetRequiredState(g_dev_handle[index], OSAL_PM_REQUIRED_STATE_REQUIRED, true));
    }

    return osal_ret;
}

/**
 * @brief This function is to close the devive for interrupt.
 * - Power off the opened device by using R_OSAL_PmSetRequiredState.
 * - Close the device by using R_OSAL_IoDeviceClose.
 *
 * @return OSAL_RETURN_OK on success
 * @return not OSAL_RETURN_OK on failure
 */
static e_osal_return_t mq_device_deinitialize()
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint32_t index = 0;

    for (index = 0; index < OSAL_MQ_PROCESS_NUM; index++)
    {
        /* power off */
        OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_PmSetRequiredState(g_dev_handle[index], OSAL_PM_REQUIRED_STATE_RELEASED, true));
        /* device close */
        OSAL_EXEC(osal_ret, OSAL_RETURN_OK, R_OSAL_IoDeviceClose(g_dev_handle[index]));
    }

    return osal_ret;
}
