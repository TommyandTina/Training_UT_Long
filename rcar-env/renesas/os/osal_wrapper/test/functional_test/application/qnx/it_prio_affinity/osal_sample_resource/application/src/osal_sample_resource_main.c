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
* File Name :    osal_sample_resource_main.c
* Version :      3.3.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Sample application to use OSAL Thread/Thread Sync/Message API
***********************************************************************************************************************/
#include "osal_sample_resource_common.h"

static int32_t shared_data = 0;

/* OSAL Resource Application: Common Sample Code */
static int32_t resource_initialize_thread_config(size_t num_of_thread_config, st_osal_resource_thread_config_t *const list_thread_config);
static int32_t resource_create_thread(st_osal_resource_thread_config_t const *list_thread_config, size_t num_of_thread, osal_thread_handle_t *const list_thread_handle);
static int32_t resource_join_thread(size_t num_of_thread, osal_thread_handle_t *const list_thread_handle, int64_t *list_thread_ret);
static int32_t resource_get_set_timestamp(st_osal_time_t *const timestamp);

/* OSAL Resource Application: Sample for Message queue API */
static int32_t create_mq_main_task(void);
int64_t mq_main_task(void *arg);
static int32_t mq_create_queue(osal_mq_handle_t *mq_handle);
static int32_t mq_delete_queue(osal_mq_handle_t const *mq_handle);
int64_t mq_first_worker_func(void *arg);
static int32_t mq_first_worker_func_send_msg(osal_mq_handle_t const *mq_handle, st_osal_mq_config_t const *mq_config);
int64_t mq_second_worker_func(void *arg);
static int32_t mq_second_worker_func_receive_msg(osal_mq_handle_t const *mq_handle, st_osal_mq_config_t const *mq_config);

/* OSAL Resource Application: Sample for Thread API */
static int32_t create_thread_main_task(void);
int64_t thread_main_task(void *arg);
int64_t thread_dummy_task(void *arg);
int64_t thread_sub_task(void *arg);
static int32_t thread_sub_task_calculate_time_difference(void);
static int32_t thread_sub_task_sleep(void);
static int32_t thread_sub_task_get_compare_handle(osal_thread_handle_t const thread_handle);

/* OSAL Resource Application: Sample for Threadsync API */
static int32_t create_threadsync_main_task(void);
int64_t threadsync_main_task(void *arg);
static int32_t resource_create_cond(size_t num_of_cond, osal_cond_handle_t *const list_cond_handle);
static int32_t resource_create_mutex(size_t num_of_mutex, osal_mutex_handle_t *const list_mutex_handle);
static int32_t resource_create_resource_protection(st_osal_resource_mutex_config_t *mutex_config, st_osal_resource_cond_config_t *cond_config);
static int32_t resource_delete_cond(size_t num_of_cond, osal_cond_handle_t *const list_cond_handle);
static int32_t resource_delete_mutex(size_t num_of_mutex, osal_mutex_handle_t *const list_mutex_handle);
static int32_t resource_delete_resource_protection(size_t num_of_mutex, size_t num_of_cond, osal_mutex_handle_t *const list_mutex_handle, osal_cond_handle_t *const list_cond_handle);
static int32_t threadsync_trylock_unlock_mutex_sub_task(osal_mutex_handle_t mtx_handle);
int64_t threadsync_first_worker_func_condwait_timeperiod(void *arg);
int64_t threadsync_first_worker_func_condwait_timestamp(void *arg);
int64_t threadsync_second_worker_func_cond_signal(void *arg);
int64_t threadsync_second_worker_func_cond_broadcast(void *arg);
static int32_t threadsync_first_waiting_waking_up(st_arg_cond_mutex_t pass_arg);
static int32_t threadsync_second_waiting_waking_up(st_arg_cond_mutex_t pass_arg);

extern uint64_t g_osal_main_thread_cpu_affinity;

/**
 * @defgroup osal_sample_resource OSAL Application: Sample For OSAL Resource Usage
 * @{
 */
/**
 * @brief The main function of OSAL Sample Resource application
 * - Call R_OSAL_GetVersion and output OSAL version
 * - Call R_OSAL_Initialize to initialize OSAL and call the resource_task function.
 * - The resource_task is divided into 3 main tasks as below:
 *      -# @link create_mq_main_task @endlink describes the function of Message queue control
 *      -# @link create_thread_main_task @endlink describes the function of Thread creation
 *      -# @link create_threadsync_main_task @endlink describes the function of Mutex control and synchronize for conditional variables
 * - Call R_OSAL_Deinitialize to deinitialize OSAL
 * @return 0 on success
 * @return -1 on failure
 */
int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv; /* PRQA S 2956 # No problem because argv is not used */

    /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
    (void)printf("================== osal_sample_resource start ==================\n");
    /* local variable */
    e_osal_return_t osal_ret;
    /* PRQA S 5209 2 # This variable is used in the return value of the main function. */
    /* PRQA S 2981 1 # Substitute the initial value to prevent the return value from becoming indefinite. */
    int app_ret = 0;
    int64_t func_ret;
    bool is_osal_initialized = false;
    bool is_task_failed = false;
    unsigned int    test_pass = 0;
    unsigned int    test_fail = 0;
    unsigned int    test_skip = 0;
    const st_osal_version_t *version;

    version = R_OSAL_GetVersion();
    printf("OSAL API version %u.%u.%u\n", version->api.major, version->api.minor, version->api.patch );
    printf("OSAL Wrapper version %u.%u.%u\n", version->wrapper.major, version->wrapper.minor, version->wrapper.patch );

    OSAL_SAMPLE_INFO("Initializing OSAL by calling R_OSAL_Initialize\n");
    osal_ret = R_OSAL_Initialize();
    if(OSAL_RETURN_OK != osal_ret)
    {
        OSAL_SAMPLE_ERR("R_OSAL_Initialize was failed, osal_ret = %d\n", osal_ret);
    }
    else
    {
        if (0x0U != g_osal_main_thread_cpu_affinity)
        {
            /* Change the CPU Affinity of the main thread. */
            ThreadCtl(_NTO_TCTL_RUNMASK, (void *)g_osal_main_thread_cpu_affinity);
        }
        is_osal_initialized = true;
        OSAL_SAMPLE_INFO("OSAL initialization was successful\n");
    }

    if(true == is_osal_initialized)
    {
        OSAL_SAMPLE_INFO("Creating the main task for OSAL Message Queue\n");
        func_ret = create_mq_main_task();
        if(0 != func_ret)
        {
            is_task_failed = true;
            OSAL_SAMPLE_ERR("The main task for OSAL Message Queue failed\n");
        }
    }

    if(true == is_osal_initialized)
    {
        OSAL_SAMPLE_INFO("Creating the main task for OSAL Thread\n");
        func_ret = create_thread_main_task();
        if(0 != func_ret)
        {
            is_task_failed = true;
            OSAL_SAMPLE_ERR("The main task for OSAL Thread failed\n");
        }
    }

    if(true == is_osal_initialized)
    {
        OSAL_SAMPLE_INFO("Creating the main task for OSAL Threadsync\n");
        func_ret = create_threadsync_main_task();
        if(0 != func_ret)
        {
            is_task_failed = true;
            OSAL_SAMPLE_ERR("The main task for OSAL Threadsync failed\n");
        }
    }

    if(true == is_osal_initialized)
    {
        OSAL_SAMPLE_INFO("Deinitialize OSAL by calling R_OSAL_Deinitialize\n");
        osal_ret = R_OSAL_Deinitialize();
        if(OSAL_RETURN_OK != osal_ret)
        {
            OSAL_SAMPLE_ERR("R_OSAL_Deinitialize was failed, osal_ret = %d\n", osal_ret);
        }
        else
        {
            /*Do Nothing!*/
        }
    }

    if((false == is_osal_initialized) || (true == is_task_failed) || (OSAL_RETURN_OK != osal_ret))
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("osal_sample_resource was failed\n");
        test_fail++;
    }
    else
    {
        OSAL_SAMPLE_INFO("osal_sample_resource run successfully\n");
        test_pass++;
    }

    /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
    (void)printf("================== osal_sample_resource finish ==================\n");
    printf("< OSAL Tests OK = %u, NG = %u, NT = %u>\n", test_pass, test_fail, test_skip);

    return app_ret;
/* PRQA S 9104 2 # Sample Application aims to show how to use the OSAL API and the order in which it is called. Splitting functions reduces readability.
    There are no significant side effects to the behavior of the sample app code without modification. */
}

/**
 * @defgroup common_sample_code OSAL Application: Common Sample Code
 * @{
 */
/**
 * @brief Function for Initialize thread configurations.
 * - Loop num_of_thread_config
 *      -# Initializing the thread configurations by calling R_OSAL_ThreadInitializeThreadConfigSt
 * @param[in] num_of_thread_config The expected number of thread configuration structures to be initialized
 * @param[in, out] list_thread_config This parameter is to store the initialized thread configurations
 * @return 0 on success
 * @return -1 on failure
 */
/* PRQA S 9106 1 # We need to call this function many time to initialize multiple thread configuration */
static int32_t resource_initialize_thread_config(size_t num_of_thread_config, st_osal_resource_thread_config_t *const list_thread_config)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;
    st_osal_thread_config_t local_thread_config;

    OSAL_SAMPLE_INFO("Initializing %d thread configurations by calling R_OSAL_ThreadInitializeThreadConfigSt\n", (int32_t)num_of_thread_config);
    for(size_t i = 0; i < num_of_thread_config; i++)
    {
        osal_ret = R_OSAL_ThreadInitializeThreadConfigSt(&local_thread_config);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Initializing thread configurations failed, osal_ret = %d\n", osal_ret);
            break;
        }
        else
        {
            list_thread_config[i].thread_config = local_thread_config;
            OSAL_SAMPLE_INFO("Thread configurations no.%d was successfully initialized.\n", (int32_t)(i + 1));
        }
    }

    return app_ret;
}

/**
 * @brief Function for Create thread:<br>
 * (OSAL API UM reference: Chapter 2.2.1: Create & Join function)
 * - Loop num_of_thread
 *      -# Creating thread by calling R_OSAL_ThreadCreate
 * - If the thread creation failed, join the created thread by calling R_OSAL_ThreadJoin
 * @param[in] num_of_thread The expected number of OSAL threads to be created. User can change this number based on their expectation.
 * @param[in] list_thread_config The list of defined thread configurations for the new threads
 * @param[in, out] list_thread_handle This parameter is to store the OSAL thread handles of created OSAL threads
 * @return 0 on success
 * @return -1 on failure
 */
/* PRQA S 9106 1 # We need to call this function many time to create multiple thread */
static int32_t resource_create_thread(st_osal_resource_thread_config_t const *list_thread_config, size_t num_of_thread, osal_thread_handle_t *const list_thread_handle)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;
    osal_thread_handle_t local_thread_handle = OSAL_THREAD_HANDLE_INVALID;
    size_t thread_break;

    OSAL_SAMPLE_INFO("Creating %d threads by calling R_OSAL_ThreadCreate\n", (int32_t)num_of_thread);
    for(size_t i = 0; i < num_of_thread; i++)
    {
        osal_ret = R_OSAL_ThreadCreate(&list_thread_config[i].thread_config, list_thread_config[i].thread_id, &local_thread_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Thread creation failed, osal_ret = %d\n", osal_ret);
            thread_break = i;
            break;
        }
        else
        {
            list_thread_handle[i] = local_thread_handle;
            OSAL_SAMPLE_INFO("Thread no.%d was successfully created.\n", (int32_t)(i + 1));
        }
    }

    if((0 != app_ret) && (0U != thread_break))
    {
        OSAL_SAMPLE_INFO("Start joining %d created threads to prevent memory leaks\n", (int32_t)thread_break);
        for(size_t j = 0; j < thread_break; j++)
        {
            osal_ret = R_OSAL_ThreadJoin(list_thread_handle[j], NULL);
            if(OSAL_RETURN_OK != osal_ret)
            {
                app_ret = -1;
                OSAL_SAMPLE_ERR("Joining the thread failed, osal_ret = %d\n", osal_ret);
            }
        }
    }

    return app_ret;
}

/**
 * @brief Function for join the created thread:<br>
 * (OSAL API UM reference: Chapter 2.2.1: Create & Join function)
 * - Loop num_of_thread
 *      -# Joining the created thread by calling R_OSAL_ThreadJoin
 * @param[in] num_of_thread The expected number of thread to be joined
 * @param[in] list_thread_handle The list of OSAL thread handles which are going to be joined
 * @param[in, out] list_thread_ret This parameter is to store the return value of joined OSAL threads
 * @return 0 on success
 * @return -1 on failure
 */
/* PRQA S 9106 1 # We need to call this function many time to join thread */
static int32_t resource_join_thread(size_t num_of_thread, osal_thread_handle_t *const list_thread_handle, int64_t *list_thread_ret)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;
    int64_t thread_ret = 0;

    OSAL_SAMPLE_INFO("Start joining %d created threads\n",(int32_t)num_of_thread);
    for(size_t i = 0; i < num_of_thread; i++)
    {
        osal_ret = R_OSAL_ThreadJoin(list_thread_handle[i], &thread_ret);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Joining child thread no.%d failed, osal_ret = %d\n", (int32_t)(i + 1), osal_ret);
        }
        else
        {
            OSAL_SAMPLE_INFO("The operation on child thread no.%d was completed.\n", (int32_t)(i + 1));
            list_thread_ret[i] = thread_ret;
            list_thread_handle[i] = OSAL_THREAD_HANDLE_INVALID;
        }
    }

    return app_ret;
}

/**
 * @brief Function for get and set the time stamp
 * - Getting and setting the timestamp by calling @link resource_get_set_timestamp @endlink
 *      -# Getting the current time stamp by calling R_OSAL_ClockTimeGetTimeStamp
 *      -# Setting the time stamp by add the WAIT_TIME to the current timestamp.
 * @param[in, out] timestamp This parameter is to store the output timestamp after adjusting with WAIT_TIME_SEC and WAIT_TIME_NS
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t resource_get_set_timestamp(st_osal_time_t *const timestamp)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;
    st_osal_time_t current_timestamp;

    OSAL_SAMPLE_INFO("Start getting the current timestamp by calling R_OSAL_ClockTimeGetTimeStamp\n");
    osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_timestamp);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Getting the current timestamp failed, osal_ret = %d\n", osal_ret);
    }
    else
    {
        current_timestamp.tv_sec += WAIT_TIME_SEC;
        current_timestamp.tv_nsec += WAIT_TIME_NS;
        if(MAX_NS <= current_timestamp.tv_nsec)
        {
            current_timestamp.tv_sec += 1;
            current_timestamp.tv_nsec %= MAX_NS;
        }

        *timestamp = current_timestamp;
    }

    return app_ret;
}
/**@}End_of_common_function*/

/**
 * @defgroup mq_main_task OSAL Application: Sample for Message queue API
 * @{
 */
/**
 * @brief Create MQ main task:<br>
 * - Initializing thread configuration by calling @link resource_initialize_thread_config @endlink
 * - Creating thread about sequence of sample message queue which is @link mq_main_task @endlink
 * - Join created thread by calling @link resource_join_thread @endlink
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t create_mq_main_task(void)
{
    int32_t app_ret;
    st_osal_resource_thread_config_t list_thread_config[NUM_OF_MQ_MAIN_TASK];
    osal_thread_handle_t list_thread_handle[NUM_OF_MQ_MAIN_TASK];
    int64_t list_thread_ret[NUM_OF_MQ_MAIN_TASK];

    /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
    (void)printf("================== MQ main task start ==================\n");
    OSAL_SAMPLE_INFO("Initializing thread configuration of MQ main task\n");
    app_ret = resource_initialize_thread_config(NUM_OF_MQ_MAIN_TASK, list_thread_config);
    if(0 != app_ret)
    {
        OSAL_SAMPLE_ERR("Initializing thread configuration failed\n");
    }

    if(0 == app_ret)
    {
        list_thread_config[0].thread_id = MQ_MAIN_TASK_ID;
        list_thread_config[0].thread_config.func = mq_main_task;
        list_thread_config[0].thread_config.userarg = NULL;
        list_thread_config[0].thread_config.priority = OSAL_THREAD_PRIORITY_TYPE10;
        list_thread_config[0].thread_config.task_name = "Main task for MQ";
        list_thread_config[0].thread_config.stack_size = THREAD_STACKSIZE;

        OSAL_SAMPLE_INFO("Creating thread for MQ main task\n");
        app_ret = resource_create_thread(list_thread_config, NUM_OF_MQ_MAIN_TASK, list_thread_handle);
        if(0 != app_ret)
        {
            OSAL_SAMPLE_ERR("Thread creation for MQ main task  failed\n");
        }
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Start joining the MQ main task thread\n");
        app_ret = resource_join_thread(NUM_OF_MQ_MAIN_TASK, list_thread_handle, list_thread_ret);
        if(0 != app_ret)
        {
            OSAL_SAMPLE_ERR("Thread joining failed\n");
        }
    }

    /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
    (void)printf("================== MQ main task finish ==================\n");
    return app_ret;
}

/**
 * @brief MQ main task:<br>
 * (OSAL API UM reference: Chapter 2.4.1: Message send & receive)
 * - Creating message queue by calling @link mq_create_queue @endlink
 * - Initializing thread configurations by calling @link resource_initialize_thread_config @endlink
 * - Create 2 threads: the first thread has worker function is @link mq_first_worker_func @endlink is the sending thread:<br>
 * The second thread has worker function is @link mq_second_worker_func @endlink is the receiving thread.
 * - Joining thread in order: the first thread and then the second thread
 * - Deleting queue by calling @link mq_delete_queue @endlink
 * @param[in] arg a void pointer. In this sample, it's unused.
 * @return 0 on success
 * @return -1 on failure
 */
/* PRQA S 3673, 1505 1 # Implemented in accordance with the definition of p_osal_thread_func_t. */
int64_t mq_main_task(void *arg)
{
    (void)arg;
    int64_t app_ret;
    bool is_mq_created = false;
    osal_mq_handle_t mq_handle = OSAL_MQ_HANDLE_INVALID;
    st_osal_resource_thread_config_t list_thread_config[MQ_TASK_NUM_OF_THREAD];
    osal_thread_handle_t list_thread_handle[MQ_TASK_NUM_OF_THREAD];
    int64_t list_thread_ret[MQ_TASK_NUM_OF_THREAD];

    OSAL_SAMPLE_INFO("Starting MQ main task\n");
    app_ret = mq_create_queue(&mq_handle);
    if(0 != app_ret)
    {
        OSAL_SAMPLE_ERR("Creating MQ failed\n");
    }
    else
    {
        is_mq_created = true;
        OSAL_SAMPLE_INFO("MQ was successfully created\n")
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Initializing thread configurations for MQ task\n");
        app_ret = resource_initialize_thread_config(MQ_TASK_NUM_OF_THREAD, list_thread_config);
        if(0 != app_ret)
        {
            OSAL_SAMPLE_ERR("Initializing thread configurations failed\n");
        }
    }

    if(0 == app_ret)
    {
        list_thread_config[0].thread_id = (osal_thread_id_t)SUB_THREAD_ID;
        list_thread_config[0].thread_config.priority = OSAL_THREAD_PRIORITY_TYPE10;
        list_thread_config[0].thread_config.task_name = "Sending thread";
        list_thread_config[0].thread_config.func = mq_first_worker_func;
        /* PRQA S 0314 1 # userarg need to be a void pointer type so that there is no side-effect in this case. */
        list_thread_config[0].thread_config.userarg = (void *)&mq_handle;
        list_thread_config[0].thread_config.stack_size = THREAD_STACKSIZE;

        list_thread_config[1].thread_id = (osal_thread_id_t)SUB_THREAD_ID + 1U;
        list_thread_config[1].thread_config.priority = OSAL_THREAD_PRIORITY_TYPE10;
        list_thread_config[1].thread_config.task_name = "Receiving thread";
        list_thread_config[1].thread_config.func = mq_second_worker_func;
        /* PRQA S 0314 1 # userarg need to be a void pointer type so that there is no side-effect in this case. */
        list_thread_config[1].thread_config.userarg = (void *)&mq_handle;
        list_thread_config[1].thread_config.stack_size = THREAD_STACKSIZE;

        OSAL_SAMPLE_INFO("Creating 2 threads for sending and receiving message\n");
        app_ret = resource_create_thread(list_thread_config, MQ_TASK_NUM_OF_THREAD, list_thread_handle);
        if(0 != app_ret)
        {
            OSAL_SAMPLE_ERR("Thread creation failed\n");
        }
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Start joining the created threads of MQ task\n");
        app_ret = resource_join_thread(MQ_TASK_NUM_OF_THREAD, list_thread_handle, list_thread_ret);
        if(0 != app_ret)
        {
            OSAL_SAMPLE_ERR("Joining thread  failed\n");
        }
    }

    if(true == is_mq_created)
    {
        OSAL_SAMPLE_INFO("Deleting the created MQ\n");
        app_ret = mq_delete_queue(&mq_handle);
        if(0 != app_ret)
        {
            OSAL_SAMPLE_ERR("Deleting the MQ failed\n");
        }
    }

    return app_ret;
}

/**
 * @brief First worker function:<br>
 * (OSAL API UM reference: Chapter 2.4.1: Message send & receive)
 * - Getting the message queue config by calling R_OSAL_MqGetConfig
 * - Sending message by calling @link mq_first_worker_func_send_msg @endlink
 * @param[in] arg This parameter contains the OSAL mq handle passed from the parent thread
 * @return 0 on success
 * @return -1 on failure
 */
/* PRQA S 3673, 1505 1 # Implemented in accordance with the definition of p_osal_thread_func_t. */
int64_t mq_first_worker_func(void *arg)
{
    e_osal_return_t osal_ret;
    int64_t app_ret = 0;

    /* PRQA S 0316 2 # This casting arg is always compatible to the pointer of osal_mq_handle_t.
                      Please refer the userarg of this thread config in the mq_main_task() */
    osal_mq_handle_t const* p_mq_handle = (osal_mq_handle_t *)arg;
    st_osal_mq_config_t mq_config;

    OSAL_SAMPLE_INFO("Getting the configuration of MQ by calling R_OSAL_MqGetConfig\n");
    osal_ret = R_OSAL_MqGetConfig(*p_mq_handle, &mq_config);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Getting MQ configuration failed, osal_ret = %d\n", osal_ret);
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Start sending message to queue\n");
        app_ret = mq_first_worker_func_send_msg(p_mq_handle, &mq_config);
        if(0 != app_ret)
        {
            OSAL_SAMPLE_ERR("Message sending operation failed\n");
        }
    }

    return app_ret;
}

/**
 * @brief First worker function send message:<br>
 * (OSAL API UM reference: Chapter 2.4.1: Message send & receive)
 * - Loop in the max_num_msg (in this sample, max_num_msg = 4)
 *      -# Checking the queue is full or not by using R_OSAL_MqIsFull
 *      -# If the queue is not full, sending messages which content is "MSG_CONTENT" to queue
 *              -# If the order is an even number, sending message for time period by calling R_OSAL_MqSendForTimePeriod
 *              -# If the order is an odd number, getting & setting the time stamp by calling @link resource_get_set_timestamp @endlink and sending message by calling R_OSAL_MqSendUntilTimeStamp.
 * @param[in] mq_handle An address of OSAL message queue handle which is used for message sending sequence
 * @param[in] mq_config The configuration of OSAL message queue handle represented by mq_handle
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t mq_first_worker_func_send_msg(osal_mq_handle_t const *mq_handle, st_osal_mq_config_t const *mq_config)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;
    st_osal_time_t timestamp;
    bool isFull = false;
    uint32_t send_buffer = MSG_CONTENT;

    for(size_t i = 0; i < mq_config->max_num_msg; i++)
    {
        OSAL_SAMPLE_INFO("Start checking the MQ is full or not by calling R_OSAL_MqIsFull\n")
        osal_ret = R_OSAL_MqIsFull(*mq_handle, &isFull);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Message queue checking failed, osal_ret = %d\n", osal_ret);
        }
        else if(true == isFull)
        {
            OSAL_SAMPLE_INFO("MQ is full\n");
        }
        else
        {
            /* Do nothing */
        }

        if((0 == app_ret) && (true != isFull) && (0U == (i % 2U)))
        {
            OSAL_SAMPLE_INFO("Start sending the message to queue by calling R_OSAL_MqSendForTimePeriod\n")
            /* PRQA S 0314 1 # p_buffer need to be a void pointer type so that there is no side-effect in this case. */
            osal_ret = R_OSAL_MqSendForTimePeriod(*mq_handle, TIMEOUT_MS, (void *)&send_buffer, mq_config->msg_size);
            if(OSAL_RETURN_OK != osal_ret)
            {
                app_ret = -1;
                OSAL_SAMPLE_ERR("Sending the message to queue failed, osal_ret = %d\n", osal_ret);
            }
            else
            {
                OSAL_SAMPLE_INFO("No.%d message was successfully sent\n", (int32_t)(i + 1));
                send_buffer += 1U;
            }
        }

        if((0 == app_ret) && (true != isFull) && (0U != (i % 2U)))
        {
            OSAL_SAMPLE_INFO("Start getting and setting the timestamp\n");
            app_ret = resource_get_set_timestamp(&timestamp);
            if(0 != app_ret)
            {
                OSAL_SAMPLE_ERR("Getting and setting the time stamp failed\n");
            }

            OSAL_SAMPLE_INFO("Sending the message to queue by calling R_OSAL_MqSendUntilTimeStamp\n")
            /* PRQA S 0314 1 # p_buffer need to be a void pointer type so that there is no side-effect in this case. */
            osal_ret = R_OSAL_MqSendUntilTimeStamp(*mq_handle, &timestamp, (void *)&send_buffer, mq_config->msg_size);
            if(OSAL_RETURN_OK != osal_ret)
            {
                app_ret = -1;
                OSAL_SAMPLE_ERR("Sending message to MQ failed, osal_ret = %d\n", osal_ret);
            }
            else
            {
                OSAL_SAMPLE_INFO("No.%d message was successfully sent\n", (int32_t)(i + 1));
                send_buffer += 1U;
            }
        }
    }

    return app_ret;
}

/**
 * @brief Second worker function:<br>
 * (OSAL API UM reference: Chapter 2.4.1: Message send & receive)
 * - Getting the message queue configuration by calling R_OSAL_MqGetConfig
 * - Receiving message from the queue by calling @link mq_second_worker_func_receive_msg @endlink
 * @param[in] arg This parameter contains the OSAL mq handle passed from the parent thread
 * @return 0 on success
 * @return -1 on failure
 */
/* PRQA S 3673, 1505 1 # Implemented in accordance with the definition of p_osal_thread_func_t. */
int64_t mq_second_worker_func(void *arg)
{
    e_osal_return_t osal_ret;
    int64_t app_ret = 0;

    /* PRQA S 0316 2 # This casting arg is always compatible to the pointer of osal_mq_handle_t.
                      Please refer the userarg of this thread config in the mq_main_task() */
    osal_mq_handle_t const* p_mq_handle = (osal_mq_handle_t *)arg;
    st_osal_mq_config_t mq_config;

    OSAL_SAMPLE_INFO("Getting the configurations of MQ by calling R_OSAL_MqGetConfig\n");
    osal_ret = R_OSAL_MqGetConfig(*p_mq_handle, &mq_config);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Getting the MQ configurations failed, osal_ret = %d\n", osal_ret);
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Start receiving message from queue\n");
        app_ret = mq_second_worker_func_receive_msg(p_mq_handle, &mq_config);
        if(0 != app_ret)
        {
            OSAL_SAMPLE_ERR("The receiving message operation failed\n");
        }
    }

    return app_ret;
}

/**
 * @brief Second worker function receive message:<br>
 * (OSAL API UM reference: Chapter 2.4.1: Message send & receive)
 * - Loop in the max_num_msg (in this sample, max_num_msg = 4)
 *      -# Receiving message from the queue:
 *              -# If the order is an even number, receiving message for time period by calling R_OSAL_MqReceiveForTimePeriod
 *              -# If the order is an odd number, getting & setting time stamp by calling @link resource_get_set_timestamp @endlink and receiving message by calling R_OSAL_MqReceiveUntilTimeStamp.
 * @param[in] mq_handle An address of OSAL message queue handle which is used for message receiving sequence
 * @param[in] mq_config The configuration of OSAL message queue
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t mq_second_worker_func_receive_msg(osal_mq_handle_t const *mq_handle, st_osal_mq_config_t const *mq_config)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;
    st_osal_time_t local_timestamp;
    uint32_t receive_buffer = 0;

    for(size_t i = 0; i < mq_config->max_num_msg; i++)
    {
        if(0U == (i % 2U))
        {
            OSAL_SAMPLE_INFO("Start receiving no.%d message from queue.\n", (int32_t)(i + 1));
            /* PRQA S 0314 1 # p_buffer need to be a void pointer type so that there is no side-effect in this case. */
            osal_ret = R_OSAL_MqReceiveForTimePeriod(*mq_handle, (osal_milli_sec_t)TIMEOUT_MS, (void *)&receive_buffer, mq_config->msg_size);
            if(OSAL_RETURN_OK != osal_ret)
            {
                app_ret = -1;
                OSAL_SAMPLE_ERR("Receiving message from queue failed, osal_ret = %d\n", osal_ret);
            }
            else if((uint32_t)(MSG_CONTENT + i) != receive_buffer)
            {
                app_ret = -1;
                OSAL_SAMPLE_ERR("Received message no.(%d) was not correct with the sent message no.(%d)\n", receive_buffer, (int32_t)(MSG_CONTENT + i));
            }
            else
            {
                OSAL_SAMPLE_INFO("No.%d message was received successfully. The content of received buffer: %d\n", (int32_t)(i + 1), (int32_t)(MSG_CONTENT + i));    /* received message is correct with sent message */
            }
        }

        if(0U != (i % 2U))
        {
            OSAL_SAMPLE_INFO("Start getting and setting the timestamp\n");
            app_ret = resource_get_set_timestamp(&local_timestamp);
            if(0 != app_ret)
            {
                OSAL_SAMPLE_ERR("Getting and setting the time stamp failed\n");
            }
        }

        if((0 == app_ret) && (0U != (i % 2U)))
        {
            OSAL_SAMPLE_INFO("Start receiving no.%d message from queue.\n", (int32_t)(i + 1));
            /* PRQA S 0314 1 # p_buffer need to be a void pointer type so that there is no side-effect in this case. */
            osal_ret = R_OSAL_MqReceiveUntilTimeStamp(*mq_handle, &local_timestamp, (void *)&receive_buffer, mq_config->msg_size);
            if(OSAL_RETURN_OK != osal_ret)
            {
                app_ret = -1;
                OSAL_SAMPLE_ERR("Receiving message from queue failed, osal_ret = %d\n", osal_ret);
            }
            else if((uint32_t)(MSG_CONTENT + i) != receive_buffer)
            {
                app_ret = -1;
                OSAL_SAMPLE_ERR("Received message no.(%d) was not correct with sent message no.(%d)\n", receive_buffer, (int32_t)(MSG_CONTENT + i));
            }
            else
            {
                OSAL_SAMPLE_INFO("No.%d message received successfully. The content of received buffer: %d\n", (int32_t)(i + 1), (int32_t)(MSG_CONTENT + i));    /* received message is correct with sent message */
            }
        }
    }

    return app_ret;
}

/**
 * @brief Create queue:<br>
 * - Initializing the MQ configurations object by using R_OSAL_MqInitializeMqConfigSt
 * - Creating a queue which size is "MSG_SIZE" and the maximum number of message is "MAX_NUM_OF_MSG".
 * @param[in, out] mq_handle An address of new OSAL message queue handle
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t mq_create_queue(osal_mq_handle_t *mq_handle)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;
    st_osal_mq_config_t mq_config;

    OSAL_SAMPLE_INFO("Initializing MQ configurations by calling R_OSAL_MqInitializeMqConfigSt\n");
    osal_ret = R_OSAL_MqInitializeMqConfigSt(&mq_config);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Initializing MQ configurations failed, osal_ret = %d\n", osal_ret);
    }
    
    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Start creating the message queue with max_num_msg is %d and msg_size is %d \n", (int32_t)MAX_NUM_OF_MSG, (int32_t)MSG_SIZE); /*in this sample, MAX_NUM_OF_MSG = 4, MSG_SIZE = 4*/
        mq_config.max_num_msg = MAX_NUM_OF_MSG;
        mq_config.msg_size = MSG_SIZE;
        osal_ret = R_OSAL_MqCreate(&mq_config, (osal_mq_id_t)MQ_ID_NO1, mq_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Message queue creation failed, osal_ret = %d\n", osal_ret);
        }
        else
        {
            OSAL_SAMPLE_INFO("Message queue was successfully created\n");
        }
    }

    return app_ret;
}

/**
 * @brief Delete queue
 * - Checking the queue is empty or not by using R_OSAL_MqIsEmpty
 * - If queue is not empty, deleting all remaining message in this queue by using R_OSAL_MqReset
 * - And finally, deleting this queue by using R_OSAL_MqDelete
 * @param[in] mq_handle An address of OSAL message queue handle which is going to be destroyed
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t mq_delete_queue(osal_mq_handle_t const *mq_handle)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;
    bool isEmpty = false;

    OSAL_SAMPLE_INFO("Checking the MQ is empty or not by calling R_OSAL_MqIsEmpty\n");
    osal_ret = R_OSAL_MqIsEmpty(*mq_handle, &isEmpty);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("MQ checking operation failed, osal_ret = %d\n", osal_ret);
    }

    if((0 == app_ret) && (false == isEmpty))
    {
        OSAL_SAMPLE_INFO("Deleting all remaining messages if MQ is not empty by calling R_OSAL_MqReset\n");
        osal_ret = R_OSAL_MqReset(*mq_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Deleting remaining messages in queue failed, osal_ret = %d\n", osal_ret);
        }
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Deleting MQ by calling R_OSAL_MqDelete\n");
        osal_ret = R_OSAL_MqDelete(*mq_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Deleting MQ failed, osal_ret = %d\n", osal_ret);
        }
    }

    return app_ret;
}
/**@}End_of_mq_main_task*/

/**
 * @defgroup thread_main_task OSAL Application: Sample for Thread API
 * @{
 */
/**
 * @brief Create thread main task.
 * - Initializing thread configurations by calling @link resource_initialize_thread_config @endlink
 * - Creating thread about sequence of sample thread which is @link thread_main_task @endlink
 * - Joining the created thread by calling @link resource_join_thread @endlink
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t create_thread_main_task(void)
{
    int32_t app_ret;
    st_osal_resource_thread_config_t list_thread_config[NUM_OF_THREAD_MAIN_TASK];
    osal_thread_handle_t list_thread_handle[NUM_OF_THREAD_MAIN_TASK];
    int64_t list_thread_ret[NUM_OF_THREAD_MAIN_TASK];

    /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
    (void)printf("================== Thread main task start ==================\n");
    OSAL_SAMPLE_INFO("Initializing the thread configurations of MQ main task\n");
    app_ret = resource_initialize_thread_config(NUM_OF_THREAD_MAIN_TASK, list_thread_config);
    if(0 != app_ret)
    {
        OSAL_SAMPLE_ERR("Initializing the thread configurations failed");
    }

    if(0 == app_ret)
    {
        list_thread_config[0].thread_id = THREAD_MAIN_TASK_ID;
        list_thread_config[0].thread_config.func = thread_main_task;
        list_thread_config[0].thread_config.userarg = NULL;
        list_thread_config[0].thread_config.priority = OSAL_THREAD_PRIORITY_TYPE10;
        list_thread_config[0].thread_config.task_name = "Main task for Thread manager";
        list_thread_config[0].thread_config.stack_size = THREAD_STACKSIZE;

        OSAL_SAMPLE_INFO("Creating thread for Thread main task\n");
        app_ret = resource_create_thread(list_thread_config, NUM_OF_THREAD_MAIN_TASK, list_thread_handle);
        if(0 != app_ret)
        {
            OSAL_SAMPLE_ERR("Thread creation failed\n");
        }
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Joining the thread of Thread main task\n");
        app_ret = resource_join_thread(NUM_OF_THREAD_MAIN_TASK, list_thread_handle, list_thread_ret);
        if(0 != app_ret)
        {
            OSAL_SAMPLE_ERR("Joining thread failed\n");
        }
    }

    /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
    (void)printf("================== Thread main task finish ==================\n");
    return app_ret;
}

/**
 * @brief Thread main task.
 * - Initializing thread configurations by calling @link resource_initialize_thread_config @endlink
 * - Creating 3 threads: the first thread has worker function is @link thread_dummy_task @endlink is the dummy thread:<br>
 * The second thread has worker function is @link thread_sub_task @endlink is the sub task thread:<br>
 * The third thread has worker function is @link thread_dummy_task @endlink is the dummy thread
 * - Joining the created thread in order: the first thread, the second thread then the third thread
 * @param[in] arg a void pointer. In this sample, it's unused parameter.
 * @return 0 on success
 * @return -1 on failure
 */
/* PRQA S 3673, 1505 1 # Implemented in accordance with the definition of p_osal_thread_func_t. */
int64_t thread_main_task(void *arg)
{
    (void)arg;
    int64_t app_ret;
    st_osal_resource_thread_config_t list_thread_config[THREAD_TASK_NUM_OF_THREAD];
    osal_thread_handle_t list_thread_handle[THREAD_TASK_NUM_OF_THREAD];
    int64_t list_thread_ret[THREAD_TASK_NUM_OF_THREAD];

    OSAL_SAMPLE_INFO("Start Thread main task\n");
    OSAL_SAMPLE_INFO("Initializing thread configurations for Thread task\n");
    app_ret = resource_initialize_thread_config(THREAD_TASK_NUM_OF_THREAD, list_thread_config);
    if(0 != app_ret)
    {
        OSAL_SAMPLE_ERR("Initializing thread configuration failed");
    }

    if(0 == app_ret)
    {
        list_thread_config[0].thread_id = (osal_thread_id_t)SUB_THREAD_ID;
        list_thread_config[0].thread_config.priority = OSAL_THREAD_PRIORITY_TYPE10;
        list_thread_config[0].thread_config.task_name = "First dummy thread";
        list_thread_config[0].thread_config.func = thread_dummy_task;
        list_thread_config[0].thread_config.userarg = NULL;
        list_thread_config[0].thread_config.stack_size = THREAD_STACKSIZE;

        list_thread_config[1].thread_id = (osal_thread_id_t)(SUB_THREAD_ID + 1U);
        list_thread_config[1].thread_config.priority = OSAL_THREAD_PRIORITY_TYPE10;
        list_thread_config[1].thread_config.task_name = "Sub thread";
        list_thread_config[1].thread_config.func = thread_sub_task;
        /* PRQA S 0314 1 # userarg need to be a void pointer type so that there is no side-effect in this case. */
        list_thread_config[1].thread_config.userarg = (void *)&list_thread_handle[2];
        list_thread_config[1].thread_config.stack_size = THREAD_STACKSIZE;

        list_thread_config[2].thread_id = (osal_thread_id_t)(SUB_THREAD_ID + 2U);
        list_thread_config[2].thread_config.priority = OSAL_THREAD_PRIORITY_TYPE10;
        list_thread_config[2].thread_config.task_name = "Second dummy thread";
        list_thread_config[2].thread_config.func = thread_dummy_task;
        list_thread_config[2].thread_config.userarg = NULL;
        list_thread_config[2].thread_config.stack_size = THREAD_STACKSIZE;

        OSAL_SAMPLE_INFO("Creating threads for dummy threads and sub thread\n");
        app_ret = resource_create_thread(list_thread_config, THREAD_TASK_NUM_OF_THREAD, list_thread_handle);
        if(0 != app_ret)
        {
            OSAL_SAMPLE_ERR("Creating thread failed\n");
        }
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Joining the created threads of Thread task\n");
        app_ret = resource_join_thread(THREAD_TASK_NUM_OF_THREAD, list_thread_handle, list_thread_ret);
        if(0 != app_ret)
        {
            OSAL_SAMPLE_ERR("Joining the created thread failed\n");
        }
    }

    return app_ret;
}

/**
 * @brief Dummy thread
 * @param[in] arg a void pointer. In this sample, it's unused parameter.
 * @return 0
 */
/* PRQA S 3673,1505 1 # Implemented in accordance with the definition of p_osal_thread_func_t. */
int64_t thread_dummy_task(void *arg)
{
    (void) arg;

    return (int32_t)0;
}

/**
 * @brief Sub task thread:<br>
 * - Calculating the time difference by calling @link thread_sub_task_calculate_time_difference @endlink
 * - Getting and comparing thread handle by calling @link thread_sub_task_get_compare_handle @endlink
 * @param[in] arg This parameter contains the OSAL thread handle passed from the parent thread
 * @return 0 on success
 * @return -1 on failure
 */
/* PRQA S 3673, 1505 1 # Implemented in accordance with the definition of p_osal_thread_func_t. */
int64_t thread_sub_task(void *arg)
{
    int64_t app_ret;
    /* PRQA S 0316 2 # This casting arg is always compatible to the pointer of osal_thread_handle_t.
                      Please refer the userarg of this thread config in the thread_main_task() */
    osal_thread_handle_t const* first_dummy_thread_handle = (osal_thread_handle_t *)arg;

    OSAL_SAMPLE_INFO("Calculating the different time in sub task thread\n");
    app_ret = thread_sub_task_calculate_time_difference();
    if(0 != app_ret)
    {
        OSAL_SAMPLE_ERR("Time difference calculation failed\n");
    }

    OSAL_SAMPLE_INFO("Comparing sub thread with dummy thread\n");
    app_ret = thread_sub_task_get_compare_handle(*first_dummy_thread_handle);
    if(0 != app_ret)
    {
        OSAL_SAMPLE_ERR("Comparing thread handle failed\n");
    }

    return app_ret;
}

/**
 * @brief Sub task thread calculate time different
 * - Getting the first time stamp by calling R_OSAL_ClockTimeGetTimeStamp
 * - Sleeping thread for 2500ms by calling @link thread_sub_task_sleep @endlink
 * - Getting the second time stamp by calling R_OSAL_ClockTimeGetTimeStamp
 * - Calculating the time difference between 2 timestamps by calling R_OSAL_ClockTimeCalculateTimeDifference
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t thread_sub_task_calculate_time_difference(void)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;
    st_osal_time_t first_time_stamp;
    st_osal_time_t second_time_stamp;
    osal_nano_sec_t timestamp;

    OSAL_SAMPLE_INFO("Getting the first timestamp by calling R_OSAL_ClockTimeGetTimeStamp\n");
    osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &first_time_stamp);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Getting the first timestamp failed, osal_ret = %d\n", osal_ret);
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Sleeping the thread for %d ms\n", TIMEOUT_MS + (WAIT_TIME_SEC*1000 + WAIT_TIME_NS/1000000));
        app_ret = thread_sub_task_sleep();
        if(0 != app_ret)
        {
            OSAL_SAMPLE_ERR("Sub task sleeping thread failed\n");
        }
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Getting the second timestamp by calling R_OSAL_ClockTimeGetTimeStamp\n");
        osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &second_time_stamp);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Getting the second time stamp failed, osal_ret = %d\n", osal_ret);
        }
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Calculating the time difference between 2 timestamps\n");
        osal_ret = R_OSAL_ClockTimeCalculateTimeDifference(&second_time_stamp, &first_time_stamp, &timestamp);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("The calculation of time difference failed, osal_ret = %d\n", osal_ret);
        }
        else
        {
            /* PRQA S 5209 1 # This casting to fix compiler-dependent. There is no side-effect in this case */
            OSAL_SAMPLE_INFO("The suspending time is %ldusec\n", (long)(timestamp/1000)); /* convert nsec to usec */
        }
    }

    return app_ret;
}

/**
 * @brief Sub task thread sleep
 * - Suspending thread first time by calling R_OSAL_ThreadSleepForTimePeriod
 * - Getting and setting time stamp by calling @link resource_get_set_timestamp @endlink
 * - Suspending thread second time by calling R_OSAL_ThreadSleepUntilTimeStamp
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t thread_sub_task_sleep(void)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;
    st_osal_time_t local_timestamp;

    OSAL_SAMPLE_INFO("Suspending thread %d ms by calling R_OSAL_ThreadSleepForTimePeriod\n", TIMEOUT_MS);
    osal_ret = R_OSAL_ThreadSleepForTimePeriod((osal_milli_sec_t)TIMEOUT_MS);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("The first suspending thread failed, osal_ret = %d\n", osal_ret);
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Start getting and setting the timestamp\n");
        app_ret = resource_get_set_timestamp(&local_timestamp);
        if(0 != app_ret)
        {
            OSAL_SAMPLE_ERR("Getting and setting timestamp failed\n");
        }
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Suspending thread %d ms by calling R_OSAL_ThreadSleepUntilTimeStamp\n", (WAIT_TIME_SEC*1000 + WAIT_TIME_NS/1000000));
        osal_ret = R_OSAL_ThreadSleepUntilTimeStamp(&local_timestamp);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("The second suspending thread failed, osal_ret = %d\n", osal_ret);
        }
    }

    return app_ret;
}

/**
 * @brief Sub task thread get compare handle
 * - Getting the thread handle by calling R_OSAL_ThreadSelf
 * - Comparing the thread handle with the handle of dummy thread by calling R_OSAL_ThreadEqual
 * @param[in] thread_handle OSAL thread handle of another thread which is going to be compared with this current thread handle
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t thread_sub_task_get_compare_handle(osal_thread_handle_t const thread_handle)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;
    osal_thread_handle_t self_thread_handle = OSAL_THREAD_HANDLE_INVALID;
    bool isEqual = false;

    OSAL_SAMPLE_INFO("Getting the thread handle by calling R_OSAL_ThreadSelf\n");
    osal_ret = R_OSAL_ThreadSelf(&self_thread_handle);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Getting self-handle failed, osal_ret = %d\n", osal_ret);
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Comparing the sub thread with the dummy thread by calling R_OSAL_ThreadEqual\n");
        osal_ret = R_OSAL_ThreadEqual(self_thread_handle, thread_handle, &isEqual);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("R_OSAL_ThreadEqual operation was failed, osal_ret = %d\n", osal_ret);
        }
        else if(true == isEqual)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Comparing thread handle was failed\n");
        }
        else
        {
            OSAL_SAMPLE_INFO("The result is false. That's the expectation\n");
        }
    }

    return app_ret;
}
/**@}End_of_thread_main_task*/

/**
 * @defgroup threadsync_main_task OSAL Application: Sample for Thread synchronization API
 * @{
 */
/**
 * @brief Create threadsync main task
 * - Initializing thread config by calling @link resource_initialize_thread_config @endlink
 * - Creating thread about sequence of sample threadsync which is @link threadsync_main_task @endlink
 * - Joining created thread by calling @link resource_join_thread @endlink
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t create_threadsync_main_task(void)
{
    st_osal_resource_thread_config_t list_thread_config[NUM_OF_THREADSYNC_MAIN_TASK];
    osal_thread_handle_t list_thread_handle[NUM_OF_THREADSYNC_MAIN_TASK];
    int64_t list_thread_ret[NUM_OF_THREADSYNC_MAIN_TASK];
    int32_t app_ret;

    /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
    (void)printf("================== Threadsync main task start ==================\n");
    OSAL_SAMPLE_INFO("Initializing the thread configurations for Threadsync main task\n");
    app_ret = resource_initialize_thread_config(NUM_OF_THREADSYNC_MAIN_TASK, list_thread_config);
    if(0 != app_ret)
    {
        OSAL_SAMPLE_ERR("Initializing thread configurations failed\n");
    }

    if(0 == app_ret)
    {
        list_thread_config[0].thread_id = THREADSYNC_MAIN_TASK_ID;
        list_thread_config[0].thread_config.priority = OSAL_THREAD_PRIORITY_TYPE10;
        list_thread_config[0].thread_config.task_name = "Threadsync main task";
        list_thread_config[0].thread_config.func = threadsync_main_task;
        list_thread_config[0].thread_config.userarg = NULL;
        list_thread_config[0].thread_config.stack_size = THREAD_STACKSIZE;

        OSAL_SAMPLE_INFO("Creating thread for Threadsync main task\n");
        app_ret = resource_create_thread(list_thread_config, NUM_OF_THREADSYNC_MAIN_TASK, list_thread_handle);
        if(0 != app_ret)
        {
            OSAL_SAMPLE_ERR("Creating thread for Threadsync main task failed\n");
        }
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Joining the thread of Threadsync main task\n");
        app_ret = resource_join_thread(NUM_OF_THREAD_MAIN_TASK, list_thread_handle, list_thread_ret);
        if(0 != app_ret)
        {
            OSAL_SAMPLE_ERR("Joining the thread failed\n");
        }
    }

    /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
    (void)printf("================== Threadsync main task finish ==================\n");
    return app_ret;
}

/**
 * @brief Thread synchronization main task:<br>
 * - Creating resource protection by calling resource_create_resource_protection by calling @link resource_create_resource_protection @endlink
 * - Start task trylock and unlock mutex by calling @link threadsync_trylock_unlock_mutex_sub_task @endlink
 * - Start the first task: Waiting for time period and waking up by signal by calling @link threadsync_first_waiting_waking_up @endlink
 * - Start the second task: Waiting until timestamp and waking up by broadcast by calling @link threadsync_second_waiting_waking_up @endlink
 * - Deleting resource protection by calling @link resource_delete_resource_protection @endlink
 * @param[in] arg a void pointer. In this sample, it's unused parameter.
 * @return 0 on success
 * @return -1 on failure
 */
/* PRQA S 3673, 1505 1 # Implemented in accordance with the definition of p_osal_thread_func_t. */
int64_t threadsync_main_task(void *arg)
{
    (void)arg;
    int64_t app_ret;
    st_arg_cond_mutex_t pass_arg = {OSAL_COND_HANDLE_INVALID, OSAL_MUTEX_HANDLE_INVALID};
    st_osal_resource_mutex_config_t mutex_config;
    st_osal_resource_cond_config_t cond_config;

    OSAL_SAMPLE_INFO("Start the thread synchronization main task\n");
    OSAL_SAMPLE_INFO("Start creating resource protection\n");
    app_ret = resource_create_resource_protection(&mutex_config, &cond_config);
    if(0 != app_ret)
    {
        OSAL_SAMPLE_ERR("Resource protection creation failed\n");
    }
    else
    {
        pass_arg.mtx_handle = mutex_config.list_mutex_handle[0];
        pass_arg.cond_handle = cond_config.list_cond_handle[0];
        OSAL_SAMPLE_INFO("Resource protection was successfully created\n");
    }

    if(true == mutex_config.is_mutex_created)
    {
        OSAL_SAMPLE_INFO("Start sub task trylock and unlock mutex\n");
        app_ret = threadsync_trylock_unlock_mutex_sub_task(mutex_config.list_mutex_handle[0]);
        if(0 != app_ret)
        {
            OSAL_SAMPLE_ERR("Subtask trylock and unlock mutex failed\n");
        }
        else
        {
            OSAL_SAMPLE_INFO("Subtask trylock and unlock mutex successful\n");
        }
    }

    if((true == mutex_config.is_mutex_created) && (true == cond_config.is_cond_created))
    {
        OSAL_SAMPLE_INFO("Do the synchronization between 2 threads using R_OSAL_ThsyncCondWaitForTimePeriod - R_OSAL_ThsyncCondSignal\n");
        app_ret = threadsync_first_waiting_waking_up(pass_arg);
        if(0 != app_ret)
        {
            OSAL_SAMPLE_ERR("First task waiting - waking up operation failed\n");
        }
        else
        {
            OSAL_SAMPLE_INFO("First task waiting - waking up completed successfully\n");
        }
    }

    if((true == mutex_config.is_mutex_created) && (true == cond_config.is_cond_created))
    {
        OSAL_SAMPLE_INFO("Do the synchronization between 2 threads using R_OSAL_ThsyncCondWaitUntilTimeStamp - R_OSAL_ThsyncCondBroadcast\n");
        app_ret = threadsync_second_waiting_waking_up(pass_arg);
        if(0 != app_ret)
        {
            OSAL_SAMPLE_ERR("Second task waiting - waking up operation failed\n");
        }
        else
        {
            OSAL_SAMPLE_INFO("Second task waiting - waking up completed successfully\n");
        }
    }

    if((true == mutex_config.is_mutex_created) && (true == cond_config.is_cond_created))
    {
        OSAL_SAMPLE_INFO("Deleting the resource protection\n");
        app_ret = resource_delete_resource_protection(THSYNC_MTX_NUM, THSYNC_COND_NUM, mutex_config.list_mutex_handle, cond_config.list_cond_handle);
        if(0 != app_ret)
        {
            OSAL_SAMPLE_ERR("Resource protection deletion failed\n");
        }
    }

    return app_ret;
}

/**
 * @brief First task waiting and waking up
 * - Initializing thread configurations by calling @link resource_initialize_thread_config @endlink
 * - Creating 2 threads by calling @link resource_create_thread @endlink
 *      -# The first created thread has the function is @link threadsync_first_worker_func_condwait_timeperiod @endlink
 *      -# The second created thread has the function is @link threadsync_second_worker_func_cond_signal @endlink
 * - Joining the created thread by calling @link resource_join_thread @endlink
 * @param[in] pass_arg This parameter contains OSAL mutex and condition handle passed from parent thread
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t threadsync_first_waiting_waking_up(st_arg_cond_mutex_t pass_arg)
{
    int32_t app_ret;
    st_arg_cond_mutex_t local_arg = pass_arg;
    st_osal_resource_thread_config_t list_thread_config[THSYNC_THREAD_NUM];
    osal_thread_handle_t list_thread_handle[THSYNC_THREAD_NUM] = {OSAL_THREAD_HANDLE_INVALID,OSAL_THREAD_HANDLE_INVALID };
    int64_t list_thread_ret[THSYNC_THREAD_NUM];

    OSAL_SAMPLE_INFO("Initializing the thread configurations for Threadsync task\n");
    app_ret = resource_initialize_thread_config(THSYNC_THREAD_NUM, list_thread_config);
    if(0 != app_ret)
    {
        OSAL_SAMPLE_ERR("Thread configurations initialization failed\n");
    }

    if(0 == app_ret)
    {
        list_thread_config[0].thread_id = (osal_thread_id_t)SUB_THREAD_ID;
        list_thread_config[0].thread_config.priority = OSAL_THREAD_PRIORITY_TYPE10;
        list_thread_config[0].thread_config.task_name = "Waiting thread";
        list_thread_config[0].thread_config.func = threadsync_first_worker_func_condwait_timeperiod;
        /* PRQA S 0314 1 # userarg need to be a void pointer type so that there is no side-effect in this case. */
        list_thread_config[0].thread_config.userarg = (void *)&local_arg;
        list_thread_config[0].thread_config.stack_size = THREAD_STACKSIZE;

        list_thread_config[1].thread_id = (osal_thread_id_t)SUB_THREAD_ID + 1U;
        list_thread_config[1].thread_config.priority = OSAL_THREAD_PRIORITY_TYPE10;
        list_thread_config[1].thread_config.task_name = "Waking up thread";
        list_thread_config[1].thread_config.func = threadsync_second_worker_func_cond_signal;
        /* PRQA S 0314 1 # userarg need to be a void pointer type so that there is no side-effect in this case. */
        list_thread_config[1].thread_config.userarg = (void *)&local_arg;
        list_thread_config[1].thread_config.stack_size = THREAD_STACKSIZE;

        OSAL_SAMPLE_INFO("Creating 2 threads for waiting thread and waking up thread\n");
        app_ret = resource_create_thread(list_thread_config, THSYNC_THREAD_NUM, list_thread_handle);
        if(0 != app_ret)
        {
            OSAL_SAMPLE_ERR("Thread creation failed\n");
        }
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Start joining the first waiting and waking up threads\n");
        app_ret = resource_join_thread(THSYNC_THREAD_NUM, list_thread_handle, list_thread_ret);
        if(0 != app_ret)
        {
            OSAL_SAMPLE_ERR("Joining the created threads failed\n");
        }
    }

    return app_ret;
}

/**
 * @brief Second task waiting and waking up
 * - Initializing thread configurations by calling @link resource_initialize_thread_config @endlink
 * - Creating 2 threads by calling @link resource_create_thread @endlink
 *      -# The first created thread has the function is @link threadsync_first_worker_func_condwait_timestamp @endlink
 *      -# The second created thread has the function is @link threadsync_second_worker_func_cond_broadcast @endlink
 * - Joining the created thread by calling @link resource_join_thread @endlink
 * @param[in] pass_arg This parameter contains OSAL mutex and condition handle passed from parent thread
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t threadsync_second_waiting_waking_up(st_arg_cond_mutex_t pass_arg)
{
    int32_t app_ret;
    st_arg_cond_mutex_t local_arg = pass_arg;
    st_osal_resource_thread_config_t list_thread_config[THSYNC_THREAD_NUM];
    osal_thread_handle_t list_thread_handle[THSYNC_THREAD_NUM] = {OSAL_THREAD_HANDLE_INVALID,OSAL_THREAD_HANDLE_INVALID };
    int64_t list_thread_ret[THSYNC_THREAD_NUM];

    OSAL_SAMPLE_INFO("Initializing thread configurations for Threadsync task\n");
    app_ret = resource_initialize_thread_config(THSYNC_THREAD_NUM, list_thread_config);
    if(0 != app_ret)
    {
        OSAL_SAMPLE_ERR("Thread configurations initialization failed\n");
    }

    if(0 == app_ret)
    {
        list_thread_config[0].thread_id = (osal_thread_id_t)SUB_THREAD_ID + 2U;
        list_thread_config[0].thread_config.priority = OSAL_THREAD_PRIORITY_TYPE10;
        list_thread_config[0].thread_config.task_name = "Waiting thread";
        list_thread_config[0].thread_config.func = threadsync_first_worker_func_condwait_timestamp;
        /* PRQA S 0314 1 # userarg need to be a void pointer type so that there is no side-effect in this case. */
        list_thread_config[0].thread_config.userarg = (void *)&local_arg;
        list_thread_config[0].thread_config.stack_size = THREAD_STACKSIZE;

        list_thread_config[1].thread_id = (osal_thread_id_t)SUB_THREAD_ID + 3U;
        list_thread_config[1].thread_config.priority = OSAL_THREAD_PRIORITY_TYPE10;
        list_thread_config[1].thread_config.task_name = "Waking up thread";
        list_thread_config[1].thread_config.func = threadsync_second_worker_func_cond_broadcast;
        /* PRQA S 0314 1 # userarg need to be a void pointer type so that there is no side-effect in this case. */
        list_thread_config[1].thread_config.userarg = (void *)&local_arg;
        list_thread_config[1].thread_config.stack_size = THREAD_STACKSIZE;

        OSAL_SAMPLE_INFO("Creating 2 threads for waiting thread and waking up thread\n");
        app_ret = resource_create_thread(list_thread_config, THSYNC_THREAD_NUM, list_thread_handle);
        if(0 != app_ret)
        {
            OSAL_SAMPLE_ERR("Thread creation failed\n");
        }
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Start joining the second waiting and waking up threads\n");
        app_ret = resource_join_thread(THSYNC_THREAD_NUM, list_thread_handle, list_thread_ret);
        if(0 != app_ret)
        {
            OSAL_SAMPLE_ERR("Joining thread failed\n");
        }
    }

    return app_ret;
}

/**
 * @brief Create mutex
 * - Loop num_of_mutex:
 *      -# Creating mutex by calling R_OSAL_ThsyncMutexCreate
 * - If the creation multiple mutex failed, delete created mutex by calling R_OSAL_ThsyncMutexDestroy
 * @param[in] num_of_mutex The expected number of new OSAL mutexes which are going to be created
 * @param[in, out] list_mutex_handle A list which is used for storing the new OSAL mutex handles
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t resource_create_mutex(size_t num_of_mutex, osal_mutex_handle_t *const list_mutex_handle)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;
    size_t mutex_break;
    osal_mutex_handle_t local_mtx_handle = OSAL_MUTEX_HANDLE_INVALID;

    OSAL_SAMPLE_INFO("Creating %d mutex by calling R_OSAL_ThsyncMutexCreate\n",(int32_t)num_of_mutex);
    for(size_t i = 0; i < num_of_mutex; i++)
    {
        osal_ret = R_OSAL_ThsyncMutexCreate((osal_mutex_id_t)MUTEX_ID_NO1 + i, &local_mtx_handle); //Created mutex has ID in order created mutex
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Mutex creation failed, osal_ret = %d\n", osal_ret);
            mutex_break = i;
            break;
        }
        else
        {
            list_mutex_handle[i] = local_mtx_handle;
            OSAL_SAMPLE_INFO("Mutex no.%d was created\n", (int32_t)(i + 1));
        }
    }

    if(0 != app_ret)
    {
        OSAL_SAMPLE_INFO("Deleting %d created mutexes to prevent memory leaks\n", (int32_t)mutex_break);
        for (size_t j = 0; j < mutex_break; j++)
        {
            osal_ret = R_OSAL_ThsyncMutexDestroy(list_mutex_handle[j]);
            if(OSAL_RETURN_OK != osal_ret)
            {
                app_ret = -1;
                OSAL_SAMPLE_ERR("Deleting mutex failed, osal_ret = %d\n", osal_ret);
            }
        }
    }

    return app_ret;
}

/**
 * @brief Create condition variables
 * - Loop num_of_cond:
 *      -# Creating cond by calling R_OSAL_ThsyncCondCreate
 * - If the creation multiple condition variables failed, delete created cond by calling R_OSAL_ThsyncCondDestroy
 * @param[in] num_of_cond The expected number of new OSAL condition variables which are going to be created
 * @param[in, out] list_cond_handle A list which is used for storing the new OSAL condition variable handles
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t resource_create_cond(size_t num_of_cond, osal_cond_handle_t *const list_cond_handle)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;
    size_t cond_break;
    osal_cond_handle_t local_cond_handle = OSAL_COND_HANDLE_INVALID;

    OSAL_SAMPLE_INFO("Creating %d condition variable by calling R_OSAL_ThsyncCondCreate\n", (int32_t)num_of_cond);
    for(size_t i = 0; i < num_of_cond; i++)
    {
        osal_ret = R_OSAL_ThsyncCondCreate((osal_cond_id_t)COND_ID_NO1 + i, &local_cond_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Condition variable creation failed, osal_ret = %d\n", osal_ret);
            cond_break = i;
            break;
        }
        else
        {
            list_cond_handle[i] = local_cond_handle;
            OSAL_SAMPLE_INFO("Condition variable no.%d was successfully created\n", (int32_t)(i + 1));
        }
    }

    if(0 != app_ret)
    {
        OSAL_SAMPLE_INFO("Deleting %d condition variables to prevent memory leaks\n", (int32_t)cond_break);
        for (size_t j = 0; j < cond_break; j++)
        {
            osal_ret = R_OSAL_ThsyncCondDestroy(list_cond_handle[j]);
            if(OSAL_RETURN_OK != osal_ret)
            {
                app_ret = -1;
                OSAL_SAMPLE_ERR("Deleting condition variables failed, osal_ret = %d\n", osal_ret);
            }
        }
    }

    return app_ret;
}

/**
 * @brief Create resource protection
 * - Creating mutex by calling @link resource_create_mutex @endlink
 * - Creating condition variables by calling @link resource_create_cond @endlink
 * @param[in, out] mutex_config This parameter contains the information of OSAL mutexes which are going to be created
 * @param[in, out] cond_config This parameter contains the information of OSAL condition variables which are going to be created
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t resource_create_resource_protection(st_osal_resource_mutex_config_t* mutex_config, st_osal_resource_cond_config_t* cond_config)
{
    int32_t app_ret;
    size_t num_of_mutex = sizeof(mutex_config->list_mutex_handle)/sizeof(mutex_config->list_mutex_handle[0]);
    size_t num_of_cond = sizeof(cond_config->list_cond_handle)/sizeof(cond_config->list_cond_handle[0]);

    OSAL_SAMPLE_INFO("Start creating %d mutex\n", (int32_t)num_of_mutex);
    app_ret = resource_create_mutex(num_of_mutex, mutex_config->list_mutex_handle);
    if(0 != app_ret)
    {
        mutex_config->is_mutex_created = false;
        OSAL_SAMPLE_ERR("Mutex creation failed\n");
    }
    else
    {
        mutex_config->is_mutex_created = true;
    }

    OSAL_SAMPLE_INFO("Start creating %d condition variables\n", (int32_t)num_of_cond);
    app_ret = resource_create_cond(num_of_cond, cond_config->list_cond_handle);
    if(0 != app_ret)
    {
        cond_config->is_cond_created = false;
        OSAL_SAMPLE_ERR("Condition variables creation failed\n");
    }
    else
    {
        cond_config->is_cond_created = true;
    }

    if((true == mutex_config->is_mutex_created) && (true == cond_config->is_cond_created))
    {
        app_ret = 0;
    }
    else
    {
        app_ret = -1;
    }

    return app_ret;
}

/**
 * @brief Delete mutex
 * - Loop num_of_mutex:
 *      -# Deleting the mutex by calling R_OSAL_ThsyncMutexDestroy
 * @param[in] num_of_mutex The expected number of mutexes that user want to destroy
 * @param[in] list_mutex_handle A list contains OSAL mutex handles which are going to be destroyed
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t resource_delete_mutex(size_t num_of_mutex, osal_mutex_handle_t *const list_mutex_handle)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;

    OSAL_SAMPLE_INFO("Deleting %d mutex by calling R_OSAL_ThsyncMutexDestroy\n", (int32_t)num_of_mutex);
    for(size_t i = 0; i < num_of_mutex; i++)
    {
        osal_ret = R_OSAL_ThsyncMutexDestroy(list_mutex_handle[i]);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Deleting mutex no.%d failed, osal_ret = %d\n", (int32_t)(i + 1), osal_ret);
        }
        else
        {
            list_mutex_handle[i] = OSAL_MUTEX_HANDLE_INVALID;
        }
    }

    return app_ret;
}

/**
 * @brief Delete condition variables
 * - Loop num_of_cond:
 *      -# Deleting condition variables by calling R_OSAL_ThsyncCondDestroy
 * @param[in] num_of_cond The expected number of condition variables that user want to destroy
 * @param[in] list_cond_handle A list contains OSAL condition variable handles which are going to be destroyed
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t resource_delete_cond(size_t num_of_cond, osal_cond_handle_t *const list_cond_handle)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;

    OSAL_SAMPLE_INFO("Deleting %d condition variables by calling R_OSAL_ThsyncCondDestroy\n", (int32_t)num_of_cond);
    for(size_t i = 0; i < num_of_cond; i++)
    {
        osal_ret = R_OSAL_ThsyncCondDestroy(list_cond_handle[i]);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Delete condition variables no.%d failed, osal_ret = %d\n", (int32_t)(i + 1), osal_ret);
        }
        else
        {
            list_cond_handle[i] = OSAL_COND_HANDLE_INVALID;
        }
    }

    return app_ret;
}

/**
 * @brief Delete resource protection
 * - Deleting mutex by calling @link resource_delete_mutex @endlink
 * - Deleting condition variables by calling @link resource_delete_cond @endlink
 * @param[in] num_of_mutex The expected number of mutexes that user want to destroy
 * @param[in] num_of_cond The expected number of condition variables that user want to destroy
 * @param[in] list_mutex_handle A list contains OSAL mutex handles which are going to be destroyed
 * @param[in] list_cond_handle A list contains OSAL condition variable handles which are going to be destroyed
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t resource_delete_resource_protection(size_t num_of_mutex, size_t num_of_cond, osal_mutex_handle_t *const list_mutex_handle, osal_cond_handle_t *const list_cond_handle)
{
    int32_t app_ret;

    OSAL_SAMPLE_INFO("Deleting %d mutexes\n", (int32_t)num_of_mutex);
    app_ret = resource_delete_mutex(num_of_mutex, list_mutex_handle);
    if(0 != app_ret)
    {
        OSAL_SAMPLE_ERR("Mutex deletion failed\n");
    }

    OSAL_SAMPLE_INFO("Delete %d condition variable\n", (int32_t)num_of_cond);
    app_ret = resource_delete_cond(num_of_cond, list_cond_handle);
    if(0 != app_ret)
    {
        OSAL_SAMPLE_ERR("Condition variable deletion failed\n");
    }

    return app_ret;
}

/**
 * @brief Sub task trylock and unlock mutex
 * - Locking the mutex by calling R_OSAL_ThsyncMutexTryLock
 * - Unlocking the mutex by calling R_OSAL_ThsyncMutexUnlock
 * @param[in] mtx_handle OSAL mutex handle
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t threadsync_trylock_unlock_mutex_sub_task(osal_mutex_handle_t mtx_handle)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;

    OSAL_SAMPLE_INFO("Locking the by calling R_OSAL_ThsyncMutexTryLock\n");
    osal_ret = R_OSAL_ThsyncMutexTryLock(mtx_handle);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Locking mutex failed, osal_ret = %d\n", osal_ret);
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Unlocking the mutex by calling R_OSAL_ThsyncMutexUnlock\n");
        osal_ret = R_OSAL_ThsyncMutexUnlock(mtx_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Unlocking mutex failed, osal_ret = %d\n", osal_ret);
        }
    }

    return app_ret;
}

/**
 * @brief First worker function wait for time period
 * - Locking the mutex for 1000ms by calling R_OSAL_ThsyncMutexLockForTimePeriod
 * - Waiting the condition variable by calling R_OSAL_ThsyncCondWaitForTimePeriod until the waiting time is up
 * - Unlocking mutex by using R_OSAL_ThsyncMutexUnlock
 * @param[in] arg This parameter contains OSAL mutex and condition variable handle passed from the parent thread
 * @return 0 on success
 * @return -1 on failure
 */
/* PRQA S 3673, 1505 1 # Implemented in accordance with the definition of p_osal_thread_func_t. */
int64_t threadsync_first_worker_func_condwait_timeperiod(void *arg)
{
    /* PRQA S 0316 2 # This casting arg is always compatible to the pointer of st_arg_cond_mutex_t.
                      Please refer the userarg of this thread config in the threadsync_main_task() */
    st_arg_cond_mutex_t const* p_local_arg = (st_arg_cond_mutex_t *)arg;
    e_osal_return_t osal_ret;
    int64_t app_ret = 0;
    int32_t cond_wait_count = 0;
    bool locked = false;

    OSAL_SAMPLE_INFO("Locking mutex by calling R_OSAL_ThsyncMutexLockForTimePeriod\n");
    osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(p_local_arg->mtx_handle, (osal_milli_sec_t)TIMEOUT_MS);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Mutex locking failed, osal_ret = %d\n", osal_ret);
    }
    else
    {
        locked = true;
    }

    while((0 == shared_data) && (0 == app_ret))
    {
        OSAL_SAMPLE_INFO("Waiting thread, 1st waiting with condition variable\n");
        osal_ret = R_OSAL_ThsyncCondWaitForTimePeriod(p_local_arg->cond_handle, p_local_arg->mtx_handle, (osal_milli_sec_t)TIMEOUT_MS);
        switch (osal_ret)
        {
            case OSAL_RETURN_OK:
                break;
            case OSAL_RETURN_TIME:
                cond_wait_count += 1;
                if(OSAL_COND_WAIT_MAX_TRY == cond_wait_count) /* Still timeout after retried 5 times */
                {
                    app_ret = -1;
                    OSAL_SAMPLE_ERR("Condition variable waiting failed, osal_ret = %d\n", osal_ret);
                }
                else
                {
                    OSAL_SAMPLE_INFO("Time out restart waiting\n");
                }
                break;
            default:
                app_ret = -1;
                OSAL_SAMPLE_ERR("Condition variable waiting failed, osal_ret = %d\n", osal_ret);
                break;
        }
    }

    if(true == locked)
    {
        OSAL_SAMPLE_INFO("Waiting thread, unlocking mutex\n");
        osal_ret = R_OSAL_ThsyncMutexUnlock(p_local_arg->mtx_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Mutex unlocking failed, osal_ret = %d\n", osal_ret);
        }
    }

    return app_ret;
}

/**
 * @brief Sub task signal waking up thread
 * - Locking the mutex for time period by calling R_OSAL_ThsyncMutexLockForTimePeriod
 * - Waking up the condition by using R_OSAL_ThsyncCondSignal
 * - Unlock mutex by using R_OSAL_ThsyncMutexUnlock
 * @param[in] arg This parameter contains OSAL mutex and condition variable handle passed from the parent thread
 * @return 0 on success
 * @return -1 on failure
 */
/* PRQA S 3673, 1505 1 # Implemented in accordance with the definition of p_osal_thread_func_t. */
int64_t threadsync_second_worker_func_cond_signal(void *arg)
{
    /* PRQA S 0316 2 # This casting arg is always compatible to the pointer of st_arg_cond_mutex_t.
                      Please refer the userarg of this thread config in the threadsync_main_task() */
    st_arg_cond_mutex_t const* p_local_arg = (st_arg_cond_mutex_t *)arg;
    e_osal_return_t osal_ret;
    int64_t app_ret = 0;
    bool locked = false;

    OSAL_SAMPLE_INFO("Waking up thread, locking mutex\n");
    osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(p_local_arg->mtx_handle, (osal_milli_sec_t)TIMEOUT_MS);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Mutex locking failed, osal_ret = %d\n", osal_ret);
    }
    else
    {
        locked = true;
    }

    if(0 == app_ret)
    {
        shared_data += 1;
        OSAL_SAMPLE_INFO("Waking up thread, 1st waking up the waiting thread\n");
        osal_ret = R_OSAL_ThsyncCondSignal(p_local_arg->cond_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Condition signal failed, osal_ret = %d\n", osal_ret);
        }
    }

    if(true == locked)
    {
        OSAL_SAMPLE_INFO("Waking up thread, unlocking mutex\n");
        osal_ret = R_OSAL_ThsyncMutexUnlock(p_local_arg->mtx_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Mutex unlocking failed, osal_ret = %d\n", osal_ret);
        }
    }

    return app_ret;
}

/**
 * @brief First worker function wait until time stamp
 * - Getting and setting the first time stamp by calling @link resource_get_set_timestamp @endlink
 * - Locking mutex by using R_OSAL_ThsyncMutexLockUntilTimeStamp
 * - Getting and setting the second time stamp by calling @link resource_get_set_timestamp @endlink
 * - Waiting on the condition by using R_OSAL_ThsyncCondWaitUntilTimeStamp until the waiting time is up
 * - Unlocking mutex by using R_OSAL_ThsyncMutexUnlock
 * @param[in] arg This parameter contains OSAL mutex and condition variable handle passed from the parent thread
 * @return 0 on success
 * @return -1 on failure
 */
/* PRQA S 3673, 1505 1 # Implemented in accordance with the definition of p_osal_thread_func_t. */
int64_t threadsync_first_worker_func_condwait_timestamp(void *arg)
{
    /* PRQA S 0316 2 # This casting arg is always compatible to the pointer of st_arg_cond_mutex_t.
                      Please refer the userarg of this thread config in the threadsync_main_task() */
    st_arg_cond_mutex_t const* p_local_arg = (st_arg_cond_mutex_t *)arg;
    e_osal_return_t osal_ret;
    int64_t app_ret;
    int32_t cond_wait_count = 0;
    st_osal_time_t first_timestamp;
    st_osal_time_t second_timestamp;
    bool locked = false;

    OSAL_SAMPLE_INFO("Start getting and setting the first timestamp for mutex lock\n");
    app_ret = resource_get_set_timestamp(&first_timestamp);
    if(0 != app_ret)
    {
        OSAL_SAMPLE_ERR("Getting and setting the first timestamp failed\n");
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Waiting thread, locking mutex\n");
        osal_ret = R_OSAL_ThsyncMutexLockUntilTimeStamp(p_local_arg->mtx_handle, &first_timestamp);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Mutex locking failed, osal_ret = %d\n", osal_ret);
        }
        else
        {
            locked = true;
        }
    }

    while((0 != shared_data) && (0 == app_ret))
    {
        OSAL_SAMPLE_INFO("Start getting and setting the second timestamp for cond wait\n");
        app_ret = resource_get_set_timestamp(&second_timestamp);
        if(0 != app_ret)
        {
            OSAL_SAMPLE_ERR("Getting and setting the timestamp failed\n");
        }

        if(0 == app_ret)
        {
            OSAL_SAMPLE_INFO("Waiting thread, 2nd waiting with condition variable\n");
            osal_ret = R_OSAL_ThsyncCondWaitUntilTimeStamp(p_local_arg->cond_handle, p_local_arg->mtx_handle, &second_timestamp);
            switch (osal_ret)
            {
                case OSAL_RETURN_OK:
                    break;
                case OSAL_RETURN_TIME:
                    cond_wait_count += 1;
                    if(OSAL_COND_WAIT_MAX_TRY == cond_wait_count) /* Still timeout after retried 5 times */
                    {
                        app_ret = -1;
                        OSAL_SAMPLE_ERR("condition wait failed, osal_ret = %d\n", osal_ret);
                    }
                    else
                    {
                        OSAL_SAMPLE_INFO("Time out restart waiting\n");
                    }
                    break;
                default:
                    app_ret = -1;
                    OSAL_SAMPLE_ERR("Condition wait failed, osal_ret = %d\n", osal_ret);
                    break;
            }
        }
    }

    if(true == locked)
    {
        OSAL_SAMPLE_INFO("Waiting thread, unlocking mutex\n");
        osal_ret = R_OSAL_ThsyncMutexUnlock(p_local_arg->mtx_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Mutex unlocking failed, osal_ret = %d\n", osal_ret);
        }
    }

    return app_ret;
}

/**
 * @brief Sub task broadcast waking up thread
 * - Locking mutex by using R_OSAL_ThsyncMutexLockUntilTimeStamp
 * - Waking up on the condition by using R_OSAL_ThsyncCondBroadcast
 * - Unlocking mutex by using R_OSAL_ThsyncMutexUnlock
 * @param[in] arg This parameter contains OSAL mutex and condition variable handle passed from the parent thread
 * @return 0 on success
 * @return -1 on failure
 */
/* PRQA S 3673, 1505 1 # Implemented in accordance with the definition of p_osal_thread_func_t. */
int64_t threadsync_second_worker_func_cond_broadcast(void *arg)
{
    /* PRQA S 0316 2 # This casting arg is always compatible to the pointer of st_arg_cond_mutex_t.
                      Please refer the userarg of this thread config in the threadsync_main_task() */
    st_arg_cond_mutex_t const* p_local_arg = (st_arg_cond_mutex_t *)arg;
    e_osal_return_t osal_ret;
    int64_t app_ret;
    st_osal_time_t timestamp;
    bool locked = false;

    OSAL_SAMPLE_INFO("Start getting the the timestamp\n");
    app_ret = resource_get_set_timestamp(&timestamp);
    if(0 != app_ret)
    {
        OSAL_SAMPLE_ERR("Getting the timestamp failed\n");
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Waking up thread, locking mutex\n");
        osal_ret = R_OSAL_ThsyncMutexLockUntilTimeStamp(p_local_arg->mtx_handle, &timestamp);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Mutex locking failed, osal_ret = %d\n", osal_ret);
        }
        else
        {
            locked = true;
            shared_data -= 1;
        }
    }

    if(0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Waking up thread, 2nd waking up the waiting thread\n");
        osal_ret = R_OSAL_ThsyncCondBroadcast(p_local_arg->cond_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Condition broadcast failed, osal_ret = %d\n", osal_ret);
        }
    }

    if(true == locked)
    {
        OSAL_SAMPLE_INFO("Waking up thread, unlocking mutex\n");
        osal_ret = R_OSAL_ThsyncMutexUnlock(p_local_arg->mtx_handle);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Mutex unlocking failed, osal_ret = %d\n", osal_ret);
        }
    }

    return app_ret;
}
/**@}End_of_threadsync_main_task*/
/**@}End_of_osal_sample_resource*/
