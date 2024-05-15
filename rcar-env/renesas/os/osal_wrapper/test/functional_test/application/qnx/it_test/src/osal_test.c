
/* include */

#include "osal_test.h"
#include "rcar-xos/osal_configuration/target/common/r_osal_res_cfg_info.h"
#include "target/common/dev_cfg/static/r_osal_dev_cfg.h"

int osal_test(int parent_number, int child_number)
{
    printf("\n\n");

    /* switch test case at manager number */
    switch (parent_number)
    {
        case OSAL_TEST_RETURN_CONFIRMATION:
            osal_test_return_confirmation_menu(child_number);
            break;
        case OSAL_TEST_TIMER_ACCURACY:
            osal_test_timer_accuracy_check_menu(child_number);
            break;
        case OSAL_TEST_ALL:
            osal_test_return_confirmation_menu(OSAL_TEST_ALL_EXEC);
            osal_test_timer_accuracy_check_menu(OSAL_TEST_ALL_EXEC);
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

/* prottype */
/* Return value confirmation */
static void osal_test_return_thread_id(void);
static void osal_test_return_thread_id_without_def(void);
static void osal_test_return_mutex_id(void);
static void osal_test_return_mutex_id_without_def(void);
static void osal_test_return_cond_id(void);
static void osal_test_return_cond_id_without_def(void);
static void osal_test_return_mq_id(void);
static void osal_test_return_mq_id_without_def(void);
static void osal_test_return_device_id(void);
static void osal_test_return_device_id_without_def(void);
#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
static void osal_test_return_mmngr_debug_print(void);
static void osal_test_return_buffer_debug_print(void);
#endif
static void osal_test_return_mmngr_open(void);
static void osal_test_return_mmngr_open_region_idx(void);
static void osal_test_return_int_num_error_handle(void);
static void osal_test_return_int_num_broken_handle(void);
static void osal_test_return_io_get_axibusname_out_range(void);

/* Timer accuracy check */
static void osal_test_timestamp_high_res(void);
static void osal_test_timestamp_high_res_float(void);
static void osal_test_timestamp_high_pre(void);
static void osal_test_timestamp_high_pre_float(void);
static void osal_test_timeperiod_high_res(void);
static void osal_test_timeperiod_high_pre(void);


/* Return value confirmation */
void osal_test_return_confirmation_display_menu(void)
{
    printf("*** Return value confirmation ***\n");
    printf("***  11_001: R_OSAL_ThreadCreate(OSAL_THREAD_INVALID_ID)        ***\n");
    printf("***  11_002: R_OSAL_ThreadCreate(ID without definition)         ***\n");
    printf("***  11_003: R_OSAL_ThsyncMutexCreate(OSAL_MUTEX_INVALID_ID)    ***\n");
    printf("***  11_004: R_OSAL_ThsyncMutexCreate(ID without definition)    ***\n");
    printf("***  11_005: R_OSAL_ThsyncCondCreate(OSAL_COND_INVALID_ID)      ***\n");
    printf("***  11_006: R_OSAL_ThsyncCondCreate(ID without definition)     ***\n");
    printf("***  11_007: R_OSAL_MqCreate(OSAL_MQ_INVALID_ID)                ***\n");
    printf("***  11_008: R_OSAL_MqCreate(ID without definition)             ***\n");
    printf("***  11_009: R_OSAL_IoDeviceOpen(NULL)                          ***\n");
    printf("***  11_010: R_OSAL_IoDeviceOpen(ID without definition)         ***\n");
#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
    printf("***  11_011: R_OSAL_MmngrDebugMmngrPrint(read only file)        ***\n");
    printf("***  11_012: R_OSAL_MmngrDebugBufferPrint(read only file)       ***\n");
#endif
    printf("***  11_013: R_OSAL_MmngrOpen()                                 ***\n");
    printf("***  11_014: R_OSAL_MmngrOpenFromRegionIdx()                    ***\n");
    printf("***  11_015: R_OSAL_InterruptGetNumOfIrqChannels(error handle)  ***\n");
    printf("***  11_016: R_OSAL_InterruptGetNumOfIrqChannels(broken handle) ***\n");
    printf("***  11_017: R_OSAL_IoGetAxiBusName(AXI ID out of range)        ***\n");
    printf("***  11_100: Return value confirmation module all exec          ***\n\n");
}

void osal_test_return_confirmation_menu(int child)
{
    int test_number = child;
    unsigned int test_count  = 1;
    unsigned int loop_count  = 0;

    /* ALL check */
    /* when select all */
    if (child == OSAL_TEST_ALL_EXEC)
    {
        test_number = OSAL_TEST_RETURN_THREAD_ID;
        test_count  = OSAL_TEST_RETURN_MAX;
    }

    for (loop_count = 0; loop_count < test_count; loop_count++)
    {
        switch (test_number)
        {
            case OSAL_TEST_RETURN_THREAD_ID:
                osal_test_return_thread_id();
                break;
            case OSAL_TEST_RETURN_THREAD_ID_WITHOUT_DEF:
                osal_test_return_thread_id_without_def();
                break;
            case OSAL_TEST_RETURN_MUTEX_ID:
                osal_test_return_mutex_id();
                break;
            case OSAL_TEST_RETURN_MUTEX_ID_WITHOUT_DEF:
                osal_test_return_mutex_id_without_def();
                break;
            case OSAL_TEST_RETURN_COND_ID:
                osal_test_return_cond_id();
                break;
            case OSAL_TEST_RETURN_COND_ID_WITHOUT_DEF:
                osal_test_return_cond_id_without_def();
                break;
            case OSAL_TEST_RETURN_MQ_ID:
                osal_test_return_mq_id();
                break;
            case OSAL_TEST_RETURN_MQ_ID_WITHOUT_DEF:
                osal_test_return_mq_id_without_def();
                break;
            case OSAL_TEST_RETURN_DEVICE_ID:
                osal_test_return_device_id();
                break;
            case OSAL_TEST_RETURN_DEVICE_ID_WITHOUT_DEF:
                osal_test_return_device_id_without_def();
                break;
#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
            case OSAL_TEST_RETURN_MMNGR_DEBUG_PRINT:
                osal_test_return_mmngr_debug_print();
                break;
            case OSAL_TEST_RETURN_BUFFER_DEBUG_PRINT:
                osal_test_return_buffer_debug_print();
                break;
#endif
            case OSAL_TEST_RETURN_MMNGR_OPEN:
                osal_test_return_mmngr_open();
                break;
            case OSAL_TEST_RETURN_MMNGR_OPEN_REGION_IDX:
                osal_test_return_mmngr_open_region_idx();
                break;
            case OSAL_TEST_RETURN_INT_NUM_ERROR_HANDLE:
                osal_test_return_int_num_error_handle();
                break;
            case OSAL_TEST_RETURN_INT_NUM_BROKEN_HANDLE:
                osal_test_return_int_num_broken_handle();
                break;
            case OSAL_TEST_RETURN_IO_GET_AXIBUSNAME_OUT_RANGE:
                osal_test_return_io_get_axibusname_out_range();
                break;
            default:
                printf("That menu is not available.\n\n");
                break;
        }
        test_number++;
    }
}

static int64_t call_function(void* p_arg)
{
    return 0;
}

/* ------------------------11_001 [OSAL_RETURN_ID]------------------------ */
static void osal_test_return_thread_id(void)
{
    int32_t test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    st_osal_thread_config_t config;
    osal_thread_handle_t handle;
    char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_THREAD_ID, 1);
        return ;
    }

    config.func       = call_function;
    config.userarg    = NULL;
    config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config.task_name  = NULL;
    config.stack_size = 0x1000;

    osal_ret = R_OSAL_ThreadCreate(&config, OSAL_THREAD_INVALID_ID, &handle);
    if (OSAL_RETURN_ID == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(OSAL_RETURN_ID), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_RETURN_CONFIRMATION,
                            OSAL_TEST_RETURN_THREAD_ID,
                            test_ret, opt_info);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_ThreadJoin(handle, NULL);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
                OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_THREAD_ID, 1);
        }
    }

    osal_ret = R_OSAL_Deinitialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_THREAD_ID, 1);
    }

    return ;
}

/* ------------------------11_002 [OSAL_RETURN_ID]------------------------ */
static void osal_test_return_thread_id_without_def(void)
{
    int32_t test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    st_osal_thread_config_t config;
    osal_thread_handle_t handle;
    char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_THREAD_ID_WITHOUT_DEF, 1);
        return ;
    }

    config.func       = call_function;
    config.userarg    = NULL;
    config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
    config.task_name  = NULL;
    config.stack_size = 0x1000;

    osal_ret = R_OSAL_ThreadCreate(&config, (OSAL_THREAD_INVALID_ID - 2U), &handle);
    if (OSAL_RETURN_ID == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(OSAL_RETURN_ID), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_RETURN_CONFIRMATION,
                            OSAL_TEST_RETURN_THREAD_ID_WITHOUT_DEF,
                            test_ret, opt_info);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_ThreadJoin(handle, NULL);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
                OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_THREAD_ID_WITHOUT_DEF, 1);
        }
    }

    osal_ret = R_OSAL_Deinitialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_THREAD_ID_WITHOUT_DEF, 1);
    }

    return ;
}

/* ------------------------11_003 [OSAL_RETURN_ID]------------------------ */
static void osal_test_return_mutex_id(void)
{
    int32_t test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    osal_mutex_handle_t handle;
    char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_MUTEX_ID, 1);
        return ;
    }

    osal_ret = R_OSAL_ThsyncMutexCreate(OSAL_MUTEX_INVALID_ID, &handle);
    if (OSAL_RETURN_ID == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(OSAL_RETURN_ID), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_RETURN_CONFIRMATION,
                            OSAL_TEST_RETURN_MUTEX_ID,
                            test_ret, opt_info);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_ThsyncMutexDestroy(handle);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
                OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_MUTEX_ID, 1);
        }
    }

    osal_ret = R_OSAL_Deinitialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_MUTEX_ID, 1);
    }

    return ;
}

/* ------------------------11_004 [OSAL_RETURN_ID]------------------------ */
static void osal_test_return_mutex_id_without_def(void)
{
    int32_t test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    osal_mutex_handle_t handle;
    char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_MUTEX_ID_WITHOUT_DEF, 1);
        return ;
    }

    osal_ret = R_OSAL_ThsyncMutexCreate((OSAL_MUTEX_INVALID_ID - 1U), &handle);
    if (OSAL_RETURN_ID == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(OSAL_RETURN_ID), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_RETURN_CONFIRMATION,
                            OSAL_TEST_RETURN_MUTEX_ID_WITHOUT_DEF,
                            test_ret, opt_info);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_ThsyncMutexDestroy(handle);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
                OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_MUTEX_ID_WITHOUT_DEF, 1);
        }
    }

    osal_ret = R_OSAL_Deinitialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_MUTEX_ID_WITHOUT_DEF, 1);
    }

    return ;
}

/* ------------------------11_005 [OSAL_RETURN_ID]------------------------ */
static void osal_test_return_cond_id(void)
{
    int32_t test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    osal_cond_handle_t handle;
    char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_COND_ID, 1);
        return ;
    }

    osal_ret = R_OSAL_ThsyncCondCreate(OSAL_COND_INVALID_ID, &handle);
    if (OSAL_RETURN_ID == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(OSAL_RETURN_ID), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_RETURN_CONFIRMATION,
                            OSAL_TEST_RETURN_COND_ID,
                            test_ret, opt_info);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_ThsyncCondDestroy(handle);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_ThsyncCondDestroy() error.",
                OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_COND_ID, 1);
        }
    }

    osal_ret = R_OSAL_Deinitialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_COND_ID, 1);
    }

    return ;
}

/* ------------------------11_006 [OSAL_RETURN_ID]------------------------ */
static void osal_test_return_cond_id_without_def(void)
{
    int32_t test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    osal_cond_handle_t handle;
    char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_COND_ID_WITHOUT_DEF, 1);
        return ;
    }

    osal_ret = R_OSAL_ThsyncCondCreate((OSAL_COND_INVALID_ID - 1U), &handle);
    if (OSAL_RETURN_ID == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(OSAL_RETURN_ID), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_RETURN_CONFIRMATION,
                            OSAL_TEST_RETURN_COND_ID_WITHOUT_DEF,
                            test_ret, opt_info);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_ThsyncCondDestroy(handle);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_ThsyncCondDestroy() error.",
                OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_COND_ID_WITHOUT_DEF, 1);
        }
    }

    osal_ret = R_OSAL_Deinitialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_COND_ID_WITHOUT_DEF, 1);
    }

    return ;
}

/* ------------------------11_007 [OSAL_RETURN_ID]------------------------ */
static void osal_test_return_mq_id(void)
{
    int32_t test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    st_osal_mq_config_t config;
    osal_mq_handle_t handle;
    char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_MQ_ID, 1);
        return ;
    }

    config.max_num_msg = 1U;
    config.msg_size    = 4U;

    osal_ret = R_OSAL_MqCreate(&config, OSAL_MQ_INVALID_ID, &handle);
    if (OSAL_RETURN_ID == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(OSAL_RETURN_ID), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_RETURN_CONFIRMATION,
                            OSAL_TEST_RETURN_MQ_ID,
                            test_ret, opt_info);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_MqDelete(handle);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_MqDelete() error.",
                OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_MQ_ID, 1);
        }
    }

    osal_ret = R_OSAL_Deinitialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_MQ_ID, 1);
    }

    return ;
}

/* ------------------------11_008 [OSAL_RETURN_ID]------------------------ */
static void osal_test_return_mq_id_without_def(void)
{
    int32_t test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    st_osal_mq_config_t config;
    osal_mq_handle_t handle;
    char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_MQ_ID_WITHOUT_DEF, 1);
        return ;
    }

    config.max_num_msg = 1U;
    config.msg_size    = 4U;

    osal_ret = R_OSAL_MqCreate(&config, (OSAL_MQ_INVALID_ID - 1U), &handle);
    if (OSAL_RETURN_ID == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(OSAL_RETURN_ID), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_RETURN_CONFIRMATION,
                            OSAL_TEST_RETURN_MQ_ID_WITHOUT_DEF,
                            test_ret, opt_info);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_MqDelete(handle);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_MqDelete() error.",
                OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_MQ_ID_WITHOUT_DEF, 1);
        }
    }

    osal_ret = R_OSAL_Deinitialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_MQ_ID_WITHOUT_DEF, 1);
    }

    return ;
}

/* ------------------------11_009 [OSAL_RETURN_PAR]------------------------ */
static void osal_test_return_device_id(void)
{
    int32_t test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    osal_device_handle_t handle;
    char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_DEVICE_ID, 1);
        return ;
    }

    osal_ret = R_OSAL_IoDeviceOpen(NULL, &handle);
    if (OSAL_RETURN_PAR == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(OSAL_RETURN_PAR), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_RETURN_CONFIRMATION,
                            OSAL_TEST_RETURN_DEVICE_ID,
                            test_ret, opt_info);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_IoDeviceClose(handle);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
                OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_DEVICE_ID, 1);
        }
    }

    osal_ret = R_OSAL_Deinitialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_DEVICE_ID, 1);
    }

    return ;
}

/* ------------------------11_010 [OSAL_RETURN_ID]------------------------ */
static void osal_test_return_device_id_without_def(void)
{
    int32_t test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    osal_device_handle_t handle;
    char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_DEVICE_ID_WITHOUT_DEF, 1);
        return ;
    }

    osal_ret = R_OSAL_IoDeviceOpen("zyxwv_00", &handle);
    if (OSAL_RETURN_ID == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(OSAL_RETURN_ID), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_RETURN_CONFIRMATION,
                            OSAL_TEST_RETURN_DEVICE_ID_WITHOUT_DEF,
                            test_ret, opt_info);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_IoDeviceClose(handle);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
                OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_DEVICE_ID_WITHOUT_DEF, 1);
        }
    }

    osal_ret = R_OSAL_Deinitialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_DEVICE_ID_WITHOUT_DEF, 1);
    }

    return ;
}

#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
#define OSAL_DEBUG_LOG_PATH "mmngr_test.log"

/* ------------------------11_011 [OSAL_RETURN_IO]------------------------ */
static void osal_test_return_mmngr_debug_print(void)
{
    int32_t test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    st_osal_mmngr_config_t config;
    osal_memory_manager_handle_t hndl_mmngr;
    FILE *output;
    char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    /* ------------------------preprocessing------------------------ */
    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_MMNGR_DEBUG_PRINT, 1);
        return ;
    }

    config.mode                        = OSAL_MMNGR_ALLOC_MODE_STACK;
    config.memory_limit                = 0x1000U;
    config.max_allowed_allocations     = 16U;
    config.max_registered_monitors_cbs = 100U;

    osal_ret = R_OSAL_MmngrOpen(&config, &hndl_mmngr);
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrOpen() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_MMNGR_DEBUG_PRINT, 1);
        (void)R_OSAL_Deinitialize();
        return ;
    }
    /* ------------------------preprocessing------------------------ */

    /* -----------------------main processing----------------------- */
    output = fopen(OSAL_DEBUG_LOG_PATH, "a+");
    if (NULL == output)
    {
        OSAL_TEST_ERR("fopen() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_MMNGR_DEBUG_PRINT, 1);
        (void)R_OSAL_MmngrClose(hndl_mmngr);
        (void)R_OSAL_Deinitialize();
        return ;
    }
    fclose(output);

    output = fopen(OSAL_DEBUG_LOG_PATH, "r");
    if (NULL == output)
    {
        OSAL_TEST_ERR("fopen() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_MMNGR_DEBUG_PRINT, 1);
        (void)R_OSAL_MmngrClose(hndl_mmngr);
        (void)R_OSAL_Deinitialize();
        return ;
    }

    osal_ret = R_OSAL_MmngrDebugMmngrPrint(hndl_mmngr, output);
    if (OSAL_RETURN_IO == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(OSAL_RETURN_IO), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_RETURN_CONFIRMATION,
                            OSAL_TEST_RETURN_MMNGR_DEBUG_PRINT,
                            test_ret, opt_info);

    fclose(output);
    /* -----------------------main processing----------------------- */

    /* ------------------------postprocessing------------------------ */
    osal_ret = R_OSAL_MmngrClose(hndl_mmngr);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrClose() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_MMNGR_DEBUG_PRINT, 1);
    }

    osal_ret = R_OSAL_Deinitialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_MMNGR_DEBUG_PRINT, 1);
    }
    /* ------------------------postprocessing------------------------ */

    return ;
}

/* ------------------------11_012 [OSAL_RETURN_IO]------------------------ */
static void osal_test_return_buffer_debug_print(void)
{
    int32_t test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    st_osal_mmngr_config_t config;
    osal_memory_manager_handle_t hndl_mmngr;
    osal_memory_buffer_handle_t  buffer_obj;
    FILE *output;
    char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    /* ------------------------preprocessing------------------------ */
    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_BUFFER_DEBUG_PRINT, 1);
        return ;
    }

    config.mode                        = OSAL_MMNGR_ALLOC_MODE_STACK;
    config.memory_limit                = 0x1000U;
    config.max_allowed_allocations     = 16U;
    config.max_registered_monitors_cbs = 100U;

    osal_ret = R_OSAL_MmngrOpen(&config, &hndl_mmngr);
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrOpen() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_BUFFER_DEBUG_PRINT, 1);
        (void)R_OSAL_Deinitialize();
        return ;
    }

    osal_ret = R_OSAL_MmngrAlloc(hndl_mmngr, 0x80, 0x80, &buffer_obj);
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrAlloc() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_BUFFER_DEBUG_PRINT, 1);
        (void)R_OSAL_MmngrClose(hndl_mmngr);
        (void)R_OSAL_Deinitialize();
        return ;
    }
    /* ------------------------preprocessing------------------------ */

    /* -----------------------main processing----------------------- */
    output = fopen(OSAL_DEBUG_LOG_PATH, "a+");
    if (NULL == output)
    {
        OSAL_TEST_ERR("fopen() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_BUFFER_DEBUG_PRINT, 1);
        (void)R_OSAL_MmngrDealloc(hndl_mmngr, buffer_obj);
        (void)R_OSAL_MmngrClose(hndl_mmngr);
        (void)R_OSAL_Deinitialize();
        return ;
    }
    fclose(output);

    output = fopen(OSAL_DEBUG_LOG_PATH, "r");
    if (NULL == output)
    {
        OSAL_TEST_ERR("fopen() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_BUFFER_DEBUG_PRINT, 1);
        (void)R_OSAL_MmngrDealloc(hndl_mmngr, buffer_obj);
        (void)R_OSAL_MmngrClose(hndl_mmngr);
        (void)R_OSAL_Deinitialize();
        return ;
    }

    osal_ret = R_OSAL_MmngrDebugBufferPrint(buffer_obj, output);
    if (OSAL_RETURN_IO == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(OSAL_RETURN_IO), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_RETURN_CONFIRMATION,
                            OSAL_TEST_RETURN_BUFFER_DEBUG_PRINT,
                            test_ret, opt_info);

    fclose(output);
    /* -----------------------main processing----------------------- */

    /* ------------------------postprocessing------------------------ */
    osal_ret = R_OSAL_MmngrDealloc(hndl_mmngr, buffer_obj);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrDealloc() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_BUFFER_DEBUG_PRINT, 1);
    }

    osal_ret = R_OSAL_MmngrClose(hndl_mmngr);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrClose() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_BUFFER_DEBUG_PRINT, 1);
    }

    osal_ret = R_OSAL_Deinitialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_BUFFER_DEBUG_PRINT, 1);
    }
    /* ------------------------postprocessing------------------------ */

    return ;
}
#endif

/* ------------------------11_013 [OSAL_RETURN_OK]------------------------ */
static void osal_test_return_mmngr_open(void)
{
    int32_t test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    st_osal_mmngr_config_t config;
    osal_memory_manager_handle_t hndl_mmngr;
    char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_MMNGR_OPEN, 1);
        return ;
    }

    config.mode                        = OSAL_MMNGR_ALLOC_MODE_STACK;
    config.memory_limit                = OSAL_MEMORY_MANAGER_CFG_UNLIMITED;
    config.max_allowed_allocations     = OSAL_MEMORY_MANAGER_CFG_UNLIMITED;
    config.max_registered_monitors_cbs = OSAL_MEMORY_MANAGER_CFG_UNLIMITED;

    osal_ret = R_OSAL_MmngrOpen(&config, &hndl_mmngr);
    if (OSAL_RETURN_OK == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(OSAL_RETURN_OK), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_RETURN_CONFIRMATION,
                            OSAL_TEST_RETURN_MMNGR_OPEN,
                            test_ret, opt_info);

    osal_ret = R_OSAL_MmngrClose(hndl_mmngr);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrClose() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_MMNGR_OPEN, 1);
    }

    osal_ret = R_OSAL_Deinitialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_MMNGR_OPEN, 1);
    }

    return;
}

/* ------------------------11_014 [OSAL_RETURN_OK]------------------------ */
static void osal_test_return_mmngr_open_region_idx(void)
{
    int32_t test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    st_osal_mmngr_config_t config;
    osal_memory_manager_handle_t hndl_mmngr;
    char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_MMNGR_OPEN_REGION_IDX, 1);
        return ;
    }

    config.mode                        = OSAL_MMNGR_ALLOC_MODE_STACK;
    config.memory_limit                = OSAL_MEMORY_MANAGER_CFG_UNLIMITED;
    config.max_allowed_allocations     = OSAL_MEMORY_MANAGER_CFG_UNLIMITED;
    config.max_registered_monitors_cbs = OSAL_MEMORY_MANAGER_CFG_UNLIMITED;

    osal_ret = R_OSAL_MmngrOpenFromRegionIdx(&config, 0, &hndl_mmngr);
    if (OSAL_RETURN_OK == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(OSAL_RETURN_OK), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_RETURN_CONFIRMATION,
                            OSAL_TEST_RETURN_MMNGR_OPEN_REGION_IDX,
                            test_ret, opt_info);

    osal_ret = R_OSAL_MmngrClose(hndl_mmngr);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrClose() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_MMNGR_OPEN_REGION_IDX, 1);
    }

    osal_ret = R_OSAL_Deinitialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_MMNGR_OPEN_REGION_IDX, 1);
    }

    return;
}

/* ------------------------11_015 [OSAL_RETURN_HANDLE]------------------------ */
static void osal_test_return_int_num_error_handle(void)
{
    int32_t test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    osal_device_handle_t dev_hndl;
    size_t get_channels;
    char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_INT_NUM_ERROR_HANDLE, 1);
        return ;
    }

    dev_hndl = (osal_device_handle_t)malloc(sizeof(st_osal_device_control_t));
    if (NULL == dev_hndl)
    {
        OSAL_TEST_ERR("malloc() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_INT_NUM_ERROR_HANDLE, 1);
        R_OSAL_Deinitialize();
        return ;
    }

    osal_ret = R_OSAL_InterruptGetNumOfIrqChannels(dev_hndl, &get_channels);
    if (OSAL_RETURN_HANDLE == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(OSAL_RETURN_HANDLE), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_RETURN_CONFIRMATION,
                            OSAL_TEST_RETURN_INT_NUM_ERROR_HANDLE,
                            test_ret, opt_info);

    free(dev_hndl);

    osal_ret = R_OSAL_Deinitialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_INT_NUM_ERROR_HANDLE, 1);
    }

    return;
}

/* ------------------------11_016 [OSAL_RETURN_HANDLE]------------------------ */
static void osal_test_return_int_num_broken_handle(void)
{
    int32_t test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
#if defined (RCAR_S4)
    char* dev_id = "caiplite_00";
#else /* V3Hx, V3M2, V4H, V4M */
    char* dev_id = "ims_00";
#endif
    osal_device_handle_t dev_hndl;
    size_t get_channels;
    char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_INT_NUM_BROKEN_HANDLE, 1);
        return ;
    }

    osal_ret = R_OSAL_IoDeviceOpen(dev_id, &dev_hndl);
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_INT_NUM_BROKEN_HANDLE, 1);
        R_OSAL_Deinitialize();
        return ;
    }

    /* broken handle */
    dev_hndl->handle_id = 0xAAAAAAAA;

    osal_ret = R_OSAL_InterruptGetNumOfIrqChannels(dev_hndl, &get_channels);
    if (OSAL_RETURN_HANDLE == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(OSAL_RETURN_HANDLE), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_RETURN_CONFIRMATION,
                            OSAL_TEST_RETURN_INT_NUM_BROKEN_HANDLE,
                            test_ret, opt_info);

    dev_hndl->handle_id = OSAL_DEV_CFG_DEVICE_HANDLE_ID;

    osal_ret = R_OSAL_IoDeviceClose(dev_hndl);
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_INT_NUM_BROKEN_HANDLE, 1);
    }

    osal_ret = R_OSAL_Deinitialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_INT_NUM_BROKEN_HANDLE, 1);
    }

    return;
}

/* ------------------------11_017 [OSAL_RETURN_PAR]------------------------ */
static void osal_test_return_io_get_axibusname_out_range(void)
{
    int32_t test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    char buffer[30];
    size_t num_of_byte;
    char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_IO_GET_AXIBUSNAME_OUT_RANGE, 1);
        return ;
    }

    osal_ret = R_OSAL_IoGetAxiBusName(0xFFFFFFFFFFFFFFFF, buffer, sizeof(buffer), &num_of_byte);
    if (OSAL_RETURN_PAR == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(OSAL_RETURN_PAR), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_RETURN_CONFIRMATION,
                            OSAL_TEST_RETURN_IO_GET_AXIBUSNAME_OUT_RANGE,
                            test_ret, opt_info);

    osal_ret = R_OSAL_Deinitialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
            OSAL_TEST_RETURN_CONFIRMATION, OSAL_TEST_RETURN_IO_GET_AXIBUSNAME_OUT_RANGE, 1);
    }

    return;
}

/* Timer accuracy check */
void osal_test_timer_accuracy_check_display_menu(void)
{
    printf("*** Timer accuracy check ***\n");
    printf("***  12_001: R_OSAL_ThreadSleepUntilTimeStamp(HIGH_RESOLUTION)   ***\n");
    printf("***  12_002: R_OSAL_ThreadSleepUntilTimeStamp(HIGH_RESOLUTION)   ***\n");
    printf("***  12_003: R_OSAL_ThreadSleepUntilTimeStamp(HIGH_PRECISION)    ***\n");
    printf("***  12_004: R_OSAL_ThreadSleepUntilTimeStamp(HIGH_PRECISION)    ***\n");
    printf("***  12_005: R_OSAL_ThreadSleepForTimePeriod(HIGH_RESOLUTION)    ***\n");
    printf("***  12_006: R_OSAL_ThreadSleepForTimePeriod(HIGH_PRECISION)     ***\n");
    printf("***  12_100: Timer accuracy check module all exec                ***\n\n");
}

void osal_test_timer_accuracy_check_menu(int child)
{
    int test_number = child;
    unsigned int test_count  = 1;
    unsigned int loop_count  = 0;

    /* ALL check */
    /* when select all */
    if (child == OSAL_TEST_ALL_EXEC)
    {
        test_number = OSAL_TEST_TIMESTAMP_HIGH_RES;
        test_count  = OSAL_TEST_TIMER_MAX;
    }

    for (loop_count = 0; loop_count < test_count; loop_count++)
    {
        switch (test_number)
        {
            case OSAL_TEST_TIMESTAMP_HIGH_RES:
                osal_test_timestamp_high_res();
                break;
            case OSAL_TEST_TIMESTAMP_HIGH_RES_FLOAT:
                osal_test_timestamp_high_res_float();
                break;
            case OSAL_TEST_TIMESTAMP_HIGH_PRE:
                osal_test_timestamp_high_pre();
                break;
            case OSAL_TEST_TIMESTAMP_HIGH_PRE_FLOAT:
                osal_test_timestamp_high_pre_float();
                break;
            case OSAL_TEST_TIMEPERIOD_HIGH_RES:
                osal_test_timeperiod_high_res();
                break;
            case OSAL_TEST_TIMEPERIOD_HIGH_PRE:
                osal_test_timeperiod_high_pre();
                break;

            default:
                printf("That menu is not available.\n\n");
                break;
        }
        test_number++;
    }
}

#define TIME_LOOP_COUNT (10)
/* ------------------------12_001 [OSAL_RETURN_OK]------------------------ */
static void osal_test_timestamp_high_res(void)
{
    int32_t test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    st_osal_time_t before_timestmp;
    st_osal_time_t after_timestmp;
    st_osal_time_t sleep_timestmp;
    osal_nano_sec_t diff_time;
    int32_t i;
    char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.",
            OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMESTAMP_HIGH_RES, 1);
        return ;
    }

    for (i = 0; i < TIME_LOOP_COUNT; i++)
    {
        osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &before_timestmp);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
                OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMESTAMP_HIGH_RES, 1);
            (void)R_OSAL_Deinitialize();
            return ;
        }

        /* Add 3[msec] */
        if (999999999 >= (before_timestmp.tv_nsec + 3000000))
        {
            sleep_timestmp.tv_sec  = before_timestmp.tv_sec;
            sleep_timestmp.tv_nsec = before_timestmp.tv_nsec + 3000000;
        }
        else
        {
            sleep_timestmp.tv_sec  = before_timestmp.tv_sec + 1;
            sleep_timestmp.tv_nsec = 3000000 - (1000000000 - before_timestmp.tv_nsec);
        }

        osal_ret = R_OSAL_ThreadSleepUntilTimeStamp(&sleep_timestmp);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
                OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMESTAMP_HIGH_RES, 1);
            (void)R_OSAL_Deinitialize();
            return ;
        }

        osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &after_timestmp);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
                OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMESTAMP_HIGH_RES, 1);
            (void)R_OSAL_Deinitialize();
            return ;
        }

        osal_ret = R_OSAL_ClockTimeCalculateTimeDifference(&after_timestmp, &before_timestmp, &diff_time);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_ClockTimeCalculateTimeDifference() error.",
                OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMESTAMP_HIGH_RES, 1);
            (void)R_OSAL_Deinitialize();
            return ;
        }

        if (3000000 <= diff_time)
        {
            test_ret = OSAL_TEST_OK;
        }
        else
        {
            test_ret = OSAL_TEST_NG;
        }

        OSAL_TEST_MAKE_TIMER(opt_info, 3000000, diff_time);
        osal_test_result_timer_output(OSAL_TEST_TIMER_ACCURACY,
                                      OSAL_TEST_TIMESTAMP_HIGH_RES,
                                      test_ret, (i+1), opt_info,
                                      &before_timestmp, &after_timestmp);
    }

    osal_ret = R_OSAL_Deinitialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
            OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMESTAMP_HIGH_RES, 1);
    }

    return ;
}

/* ------------------------12_002 [OSAL_RETURN_OK]------------------------ */
static void osal_test_timestamp_high_res_float(void)
{
    int32_t test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    st_osal_time_t before_timestmp;
    st_osal_time_t after_timestmp;
    st_osal_time_t sleep_timestmp;
    osal_nano_sec_t diff_time;
    uint32_t i;
    char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.",
            OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMESTAMP_HIGH_RES_FLOAT, 1);
        return ;
    }

    for (i = 0; i < TIME_LOOP_COUNT; i++)
    {
        osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &before_timestmp);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
                OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMESTAMP_HIGH_RES_FLOAT, 1);
            (void)R_OSAL_Deinitialize();
            return ;
        }

        /* Add 2.5[msec] */
        if (999999999 >= (before_timestmp.tv_nsec + 2500000))
        {
            sleep_timestmp.tv_sec  = before_timestmp.tv_sec;
            sleep_timestmp.tv_nsec = before_timestmp.tv_nsec + 2500000;
        }
        else
        {
            sleep_timestmp.tv_sec  = before_timestmp.tv_sec + 1;
            sleep_timestmp.tv_nsec = 2500000 - (1000000000 - before_timestmp.tv_nsec);
        }

        osal_ret = R_OSAL_ThreadSleepUntilTimeStamp(&sleep_timestmp);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
                OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMESTAMP_HIGH_RES_FLOAT, 1);
            (void)R_OSAL_Deinitialize();
            return ;
        }

        osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &after_timestmp);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
                OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMESTAMP_HIGH_RES_FLOAT, 1);
            (void)R_OSAL_Deinitialize();
            return ;
        }

        osal_ret = R_OSAL_ClockTimeCalculateTimeDifference(&after_timestmp, &before_timestmp, &diff_time);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_ClockTimeCalculateTimeDifference() error.",
                OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMESTAMP_HIGH_RES_FLOAT, 1);
            (void)R_OSAL_Deinitialize();
            return ;
        }

        if (2500000 <= diff_time)
        {
            test_ret = OSAL_TEST_OK;
        }
        else
        {
            test_ret = OSAL_TEST_NG;
        }

        OSAL_TEST_MAKE_TIMER(opt_info, 2500000, diff_time);
        osal_test_result_timer_output(OSAL_TEST_TIMER_ACCURACY,
                                      OSAL_TEST_TIMESTAMP_HIGH_RES_FLOAT,
                                      test_ret, (i+1), opt_info,
                                      &before_timestmp, &after_timestmp);
    }

    osal_ret = R_OSAL_Deinitialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
            OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMESTAMP_HIGH_RES_FLOAT, 1);
    }

    return ;
}

/* ------------------------12_003 [OSAL_RETURN_OK]------------------------ */
static void osal_test_timestamp_high_pre(void)
{
    int32_t test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    st_osal_time_t before_timestmp;
    st_osal_time_t after_timestmp;
    st_osal_time_t sleep_timestmp;
    osal_nano_sec_t diff_time;
    uint32_t i;
    char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.",
            OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMESTAMP_HIGH_PRE, 1);
        return ;
    }

    for (i = 0; i < TIME_LOOP_COUNT; i++)
    {
        osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_PRECISION, &before_timestmp);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
                OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMESTAMP_HIGH_PRE, 1);
            (void)R_OSAL_Deinitialize();
            return ;
        }

        /* Add 3[msec] */
        if (999999999 >= (before_timestmp.tv_nsec + 3000000))
        {
            sleep_timestmp.tv_sec  = before_timestmp.tv_sec;
            sleep_timestmp.tv_nsec = before_timestmp.tv_nsec + 3000000;
        }
        else
        {
            sleep_timestmp.tv_sec  = before_timestmp.tv_sec + 1;
            sleep_timestmp.tv_nsec = 3000000 - (1000000000 - before_timestmp.tv_nsec);
        }

        osal_ret = R_OSAL_ThreadSleepUntilTimeStamp(&sleep_timestmp);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
                OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMESTAMP_HIGH_PRE, 1);
            (void)R_OSAL_Deinitialize();
            return ;
        }

        osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_PRECISION, &after_timestmp);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
                OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMESTAMP_HIGH_PRE, 1);
            (void)R_OSAL_Deinitialize();
            return ;
        }

        osal_ret = R_OSAL_ClockTimeCalculateTimeDifference(&after_timestmp, &before_timestmp, &diff_time);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_ClockTimeCalculateTimeDifference() error.",
                OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMESTAMP_HIGH_PRE, 1);
            (void)R_OSAL_Deinitialize();
            return ;
        }

        if (3000000 <= diff_time)
        {
            test_ret = OSAL_TEST_OK;
        }
        else
        {
            test_ret = OSAL_TEST_NG;
        }

        OSAL_TEST_MAKE_TIMER(opt_info, 3000000, diff_time);
        osal_test_result_timer_output(OSAL_TEST_TIMER_ACCURACY,
                                      OSAL_TEST_TIMESTAMP_HIGH_PRE,
                                      test_ret, (i+1), opt_info,
                                      &before_timestmp, &after_timestmp);
    }

    osal_ret = R_OSAL_Deinitialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
            OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMESTAMP_HIGH_PRE, 1);
    }

    return ;
}

/* ------------------------12_004 [OSAL_RETURN_OK]------------------------ */
static void osal_test_timestamp_high_pre_float(void)
{
    int32_t test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    st_osal_time_t before_timestmp;
    st_osal_time_t after_timestmp;
    st_osal_time_t sleep_timestmp;
    osal_nano_sec_t diff_time;
    uint32_t i;
    char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.",
            OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMESTAMP_HIGH_PRE_FLOAT, 1);
        return ;
    }

    for (i = 0; i < TIME_LOOP_COUNT; i++)
    {
        osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_PRECISION, &before_timestmp);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
                OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMESTAMP_HIGH_PRE_FLOAT, 1);
            (void)R_OSAL_Deinitialize();
            return ;
        }

        /* Add 2.5[msec] */
        if (999999999 >= (before_timestmp.tv_nsec + 2500000))
        {
            sleep_timestmp.tv_sec  = before_timestmp.tv_sec;
            sleep_timestmp.tv_nsec = before_timestmp.tv_nsec + 2500000;
        }
        else
        {
            sleep_timestmp.tv_sec  = before_timestmp.tv_sec + 1;
            sleep_timestmp.tv_nsec = 2500000 - (1000000000 - before_timestmp.tv_nsec);
        }

        osal_ret = R_OSAL_ThreadSleepUntilTimeStamp(&sleep_timestmp);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
                OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMESTAMP_HIGH_PRE_FLOAT, 1);
            (void)R_OSAL_Deinitialize();
            return ;
        }

        osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_PRECISION, &after_timestmp);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
                OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMESTAMP_HIGH_PRE_FLOAT, 1);
            (void)R_OSAL_Deinitialize();
            return ;
        }

        osal_ret = R_OSAL_ClockTimeCalculateTimeDifference(&after_timestmp, &before_timestmp, &diff_time);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_ClockTimeCalculateTimeDifference() error.",
                OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMESTAMP_HIGH_PRE_FLOAT, 1);
            (void)R_OSAL_Deinitialize();
            return ;
        }

        if (2500000 <= diff_time)
        {
            test_ret = OSAL_TEST_OK;
        }
        else
        {
            test_ret = OSAL_TEST_NG;
        }

        OSAL_TEST_MAKE_TIMER(opt_info, 2500000, diff_time);
        osal_test_result_timer_output(OSAL_TEST_TIMER_ACCURACY,
                                      OSAL_TEST_TIMESTAMP_HIGH_PRE_FLOAT,
                                      test_ret, (i+1), opt_info,
                                      &before_timestmp, &after_timestmp);
    }

    osal_ret = R_OSAL_Deinitialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
            OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMESTAMP_HIGH_PRE_FLOAT, 1);
    }

    return ;
}

/* ------------------------12_005 [OSAL_RETURN_OK]------------------------ */
static void osal_test_timeperiod_high_res(void)
{
    int32_t test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    st_osal_time_t before_timestmp;
    st_osal_time_t after_timestmp;
    osal_nano_sec_t diff_time;
    uint32_t i;
    char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.",
            OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMEPERIOD_HIGH_RES, 1);
        return ;
    }

    for (i = 0; i < TIME_LOOP_COUNT; i++)
    {
        osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &before_timestmp);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
                OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMEPERIOD_HIGH_RES, 1);
            (void)R_OSAL_Deinitialize();
            return ;
        }

        osal_ret = R_OSAL_ThreadSleepForTimePeriod(3);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_ThreadSleepForTimePeriod() error.",
                OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMEPERIOD_HIGH_RES, 1);
            (void)R_OSAL_Deinitialize();
            return ;
        }

        osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &after_timestmp);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
                OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMEPERIOD_HIGH_RES, 1);
            (void)R_OSAL_Deinitialize();
            return ;
        }

        osal_ret = R_OSAL_ClockTimeCalculateTimeDifference(&after_timestmp, &before_timestmp, &diff_time);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_ClockTimeCalculateTimeDifference() error.",
                OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMEPERIOD_HIGH_RES, 1);
            (void)R_OSAL_Deinitialize();
            return ;
        }

        if (3000000 <= diff_time)
        {
            test_ret = OSAL_TEST_OK;
        }
        else
        {
            test_ret = OSAL_TEST_NG;
        }

        OSAL_TEST_MAKE_TIMER(opt_info, 3000000, diff_time);
        osal_test_result_timer_output(OSAL_TEST_TIMER_ACCURACY,
                                      OSAL_TEST_TIMEPERIOD_HIGH_RES,
                                      test_ret, (i+1), opt_info,
                                      &before_timestmp, &after_timestmp);
    }

    osal_ret = R_OSAL_Deinitialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
            OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMEPERIOD_HIGH_RES, 1);
    }

    return ;
}

/* ------------------------12_006 [OSAL_RETURN_OK]------------------------ */
static void osal_test_timeperiod_high_pre(void)
{
    int32_t test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    st_osal_time_t before_timestmp;
    st_osal_time_t after_timestmp;
    osal_nano_sec_t diff_time;
    uint32_t i;
    char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.",
            OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMEPERIOD_HIGH_PRE, 1);
        return ;
    }

    for (i = 0; i < TIME_LOOP_COUNT; i++)
    {
        osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_PRECISION, &before_timestmp);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
                OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMEPERIOD_HIGH_PRE, 1);
            (void)R_OSAL_Deinitialize();
            return ;
        }

        osal_ret = R_OSAL_ThreadSleepForTimePeriod(3);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_ThreadSleepForTimePeriod() error.",
                OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMEPERIOD_HIGH_PRE, 1);
            (void)R_OSAL_Deinitialize();
            return ;
        }

        osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_PRECISION, &after_timestmp);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
                OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMEPERIOD_HIGH_PRE, 1);
            (void)R_OSAL_Deinitialize();
            return ;
        }

        osal_ret = R_OSAL_ClockTimeCalculateTimeDifference(&after_timestmp, &before_timestmp, &diff_time);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_TEST_ERR("R_OSAL_ClockTimeCalculateTimeDifference() error.",
                OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMEPERIOD_HIGH_PRE, 1);
            (void)R_OSAL_Deinitialize();
            return ;
        }

        if (3000000 <= diff_time)
        {
            test_ret = OSAL_TEST_OK;
        }
        else
        {
            test_ret = OSAL_TEST_NG;
        }

        OSAL_TEST_MAKE_TIMER(opt_info, 3000000, diff_time);
        osal_test_result_timer_output(OSAL_TEST_TIMER_ACCURACY,
                                      OSAL_TEST_TIMEPERIOD_HIGH_PRE,
                                      test_ret, (i+1), opt_info,
                                      &before_timestmp, &after_timestmp);
    }

    osal_ret = R_OSAL_Deinitialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
            OSAL_TEST_TIMER_ACCURACY, OSAL_TEST_TIMEPERIOD_HIGH_PRE, 1);
    }

    return ;
}


