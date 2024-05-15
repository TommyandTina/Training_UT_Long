/* include */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rcar-xos/osal/r_osal.h"
#include "osal_test.h"
#include "osal_test_common.h"
#include "common_function.h"
//#include "target/freertos/r_osal_common.h"

//extern st_osal_rcfg_thread_config_t  gs_osal_thread_config_stub[OSAL_THREAD_RESOURCE_NUM];
//extern st_osal_mq_rcfg_config_t gs_osal_mq_config_stub[OSAL_MQ_RESOURCE_NUM];
//extern st_osal_interrupt_thread_config_t  gs_osal_interrupt_thread_config_stub[39];
//extern st_osal_device_info_t gs_osal_device_info_stub[OSAL_DEVICE_ID_MAX];

#define osal_clock_time_deinitialize()	R_OSAL_Deinitialize();
#define osal_cond_deinitialize()		R_OSAL_Deinitialize();
#define osal_interrupt_deinitialize()	R_OSAL_Deinitialize();
#define osal_io_deinitialize()			R_OSAL_Deinitialize();
#define osal_mq_deinitialize()			R_OSAL_Deinitialize();
#define osal_mutex_deinitialize()		R_OSAL_Deinitialize();
#define osal_pm_deinitialize()			R_OSAL_Deinitialize();
#define osal_thread_deinitialize()		R_OSAL_Deinitialize();

/* prototype */
static void osal_test_R_OSAL_Initialize();
static void osal_test_R_OSAL_Deinitialize();

st_stub g_stub_flag;

void osal_test_thread_common_display_menu()
{

	printf("*** OSAL Thread Common Function ***\n");
	printf("***  08_001: R_OSAL_Initialize                  ***\n");
	printf("***  08_002: R_OSAL_Deinitialize                ***\n");
	printf("***  08_100: Common function all exec           ***\n\n");
}

void osal_test_thread_common_menu(int child)
{
    int test_number = child;
	unsigned int test_count  = 1;
	unsigned int loop_count  = 0;

	/* Reset stub flags */
	memset(&g_stub_flag, 0, sizeof(st_stub));

	/* ALL check */
	/* when select all */
	if (child == OSAL_TEST_ALL_EXEC)
	{
		test_number = OSAL_TEST_COMMON_INIT;
		test_count  = OSAL_TEST_COMMON_MAX;
	}

	for (loop_count = 0; loop_count < test_count; loop_count++)
	{
		switch (test_number) 
		{
			case OSAL_TEST_COMMON_INIT:
				osal_test_R_OSAL_Initialize();
				break;
			case OSAL_TEST_COMMON_DEINIT:
				osal_test_R_OSAL_Deinitialize();
				break;
			default:
				printf("That menu is not available.\n\n");
				break;
		}

		test_number++;
	}
}

static void Test08_001_common(int test_id)
{
    e_osal_return_t osal_ret;
    e_osal_return_t expect_data;
    int test_ret;
    char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_COMMON_FUNCTION, 
															OSAL_TEST_COMMON_INIT,
															test_id);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
    osal_ret = R_OSAL_Initialize();
	TIME_MEASURE_STOP
    if (osal_ret == expect_data)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_COMMON_FUNCTION, 
							OSAL_TEST_COMMON_INIT,
							test_id, 
							test_ret, opt_info);
}

/* ------------------------08_001_001 [success]------------------------ */
static void Test08_001_001()
{
    /* Step confirmation all API called normal after Init success is skipped */
    /* Can confirm by normal test case of each API */

    Test08_001_common(OSAL_TEST_ID001);

    (void)R_OSAL_Deinitialize();
}

/* ------------------------08_001_002 [OSAL_RETURN_FAIL]------------------------ */
static void Test08_001_002()
{
	/* Set stub flag */
	g_stub_flag.R_OSAL_RCFG_GetThreadInfo = R_OSAL_ERR_NULL_CONTROL;

    Test08_001_common(OSAL_TEST_ID002);

	/* Clear stub flag */
	g_stub_flag.R_OSAL_RCFG_GetThreadInfo = 0;
}

/* ------------------------08_001_003 [OSAL_RETURN_CONF]------------------------ */
//static void Test08_001_003()
//{
    //e_osal_thread_priority_t backup_val;

	///* Set stub flag */
	//g_stub_flag.R_OSAL_RCFG_GetThreadInfo = R_OSAL_ERR_OTHER_CONTROL;

    ///* Backup value */
    //backup_val = gs_osal_thread_config_stub[0].config.priority;

    ///* Change user configuration value to invalid */
    //gs_osal_thread_config_stub[0].config.priority = (e_osal_thread_priority_t)(OSAL_THREAD_PRIORITY_HIGHEST + 1);

    //Test08_001_common(OSAL_TEST_ID003);

    ///* Revert origin value */
    //gs_osal_thread_config_stub[0].config.priority = backup_val;

	///* Clear stub flag */
	//g_stub_flag.R_OSAL_RCFG_GetThreadInfo = 0;
//}

///* ------------------------08_001_004 [OSAL_RETURN_CONF]------------------------ */
//static void Test08_001_004()
//{
    //e_osal_thread_priority_t backup_val;

	///* Set stub flag */
	//g_stub_flag.R_OSAL_RCFG_GetThreadInfo = R_OSAL_ERR_OTHER_CONTROL;

    ///* Backup value */
    //backup_val = gs_osal_thread_config_stub[0].config.priority;

    ///* Change user configuration value to invalid */
    //gs_osal_thread_config_stub[0].config.priority = (e_osal_thread_priority_t)(OSAL_THREAD_PRIORITY_LOWEST - 1);

    //Test08_001_common(OSAL_TEST_ID004);

    ///* Revert origin value */
    //gs_osal_thread_config_stub[0].config.priority = backup_val;

	///* Clear stub flag */
	//g_stub_flag.R_OSAL_RCFG_GetThreadInfo = 0;
//}

///* ------------------------08_001_005 [OSAL_RETURN_CONF]------------------------ */
//static void Test08_001_005()
//{
    //size_t backup_val;

	///* Set stub flag */
	//g_stub_flag.R_OSAL_RCFG_GetThreadInfo = R_OSAL_ERR_OTHER_CONTROL;

    ///* Backup value */
    //backup_val = gs_osal_thread_config_stub[0].config.stack_size;

    ///* Change user configuration value to invalid */
    //gs_osal_thread_config_stub[0].config.stack_size = 0;

    //Test08_001_common(OSAL_TEST_ID005);

    ///* Revert origin value */
    //gs_osal_thread_config_stub[0].config.stack_size = backup_val;

	///* Clear stub flag */
	//g_stub_flag.R_OSAL_RCFG_GetThreadInfo = 0;
//}

/* ------------------------08_001_006 [OSAL_RETURN_FAIL]------------------------ */
static void Test08_001_006()
{
	/* Set stub flag */
	g_stub_flag.R_OSAL_RCFG_GetMutexInfo = R_OSAL_ERR_NULL_CONTROL;

    Test08_001_common(OSAL_TEST_ID006);
	
	/* Clear stub flag */
	g_stub_flag.R_OSAL_RCFG_GetMutexInfo = 0;

	osal_thread_deinitialize();
}

/* ------------------------08_001_007 [OSAL_RETURN_FAIL]------------------------ */
static void Test08_001_007()
{
	/* Set stub flag */
	g_stub_flag.R_OSAL_RCFG_GetCondInfo = R_OSAL_ERR_NULL_CONTROL;

    Test08_001_common(OSAL_TEST_ID007);

	/* Clear stub flag */
	g_stub_flag.R_OSAL_RCFG_GetCondInfo = 0;
	
	osal_thread_deinitialize();
	osal_mutex_deinitialize();
}

/* ------------------------08_001_008 [OSAL_RETURN_FAIL]------------------------ */
static void Test08_001_008()
{
	/* Set stub flag */
	g_stub_flag.R_OSAL_RCFG_GetMqInfo = R_OSAL_ERR_NULL_CONTROL;

    Test08_001_common(OSAL_TEST_ID008);
	
	/* Clear stub flag */
	g_stub_flag.R_OSAL_RCFG_GetMqInfo = 0;
	
	osal_thread_deinitialize();
	osal_mutex_deinitialize();
	osal_cond_deinitialize();
}

/* ------------------------08_001_009 [OSAL_RETURN_CONF]------------------------ */
//static void Test08_001_009()
//{
    //uint64_t backup_val;

	///* Set stub flag */
	//g_stub_flag.R_OSAL_RCFG_GetMqInfo = R_OSAL_ERR_OTHER_CONTROL;

    ///* Backup value */
    //backup_val = gs_osal_mq_config_stub[0].config.max_num_msg;

    ///* Change user configuration value to invalid */
    //gs_osal_mq_config_stub[0].config.max_num_msg = 0;

    //Test08_001_common(OSAL_TEST_ID009);

    ///* Revert origin value */
    //gs_osal_mq_config_stub[0].config.max_num_msg = backup_val;

	///* Clear stub flag */
	//g_stub_flag.R_OSAL_RCFG_GetMqInfo = 0;
	
	//osal_thread_deinitialize();
	//osal_mutex_deinitialize();
	//osal_cond_deinitialize();
//}

///* ------------------------08_001_010 [OSAL_RETURN_CONF]------------------------ */
//static void Test08_001_010()
//{
    //uint64_t backup_val;

	///* Set stub flag */
	//g_stub_flag.R_OSAL_RCFG_GetMqInfo = R_OSAL_ERR_OTHER_CONTROL;

    ///* Backup value */
    //backup_val = gs_osal_mq_config_stub[0].config.msg_size;

    ///* Change user configuration value to invalid */
    //gs_osal_mq_config_stub[0].config.msg_size = 0;

    //Test08_001_common(OSAL_TEST_ID010);

    ///* Revert origin value */
   //gs_osal_mq_config_stub[0].config.msg_size = backup_val;

	///* Clear stub flag */
	//g_stub_flag.R_OSAL_RCFG_GetMqInfo = 0;
	
	//osal_thread_deinitialize();
	//osal_mutex_deinitialize();
	//osal_cond_deinitialize();
//}

/* ------------------------08_001_011 [OSAL_RETURN_FAIL]------------------------ */
static void Test08_001_011()
{
	/* Set stub flag */
	g_stub_flag.R_OSAL_DCFG_GetDeviceInfo = R_OSAL_ERR_NULL_CONTROL;

    Test08_001_common(OSAL_TEST_ID011);

	/* Clear stub flag */
	g_stub_flag.R_OSAL_DCFG_GetDeviceInfo = 0;
	
	osal_thread_deinitialize();
	osal_mutex_deinitialize();
	osal_cond_deinitialize();
	osal_mq_deinitialize();

}

/* ------------------------08_001_012 [OSAL_RETURN_CONF]------------------------ */
//static void Test08_001_012()
//{
    //uintptr_t backup_val;

	///* Set stub flag */
	//g_stub_flag.R_OSAL_DCFG_GetDeviceInfo = R_OSAL_ERR_OTHER_CONTROL;

    ///* Backup value */
	//backup_val = gs_osal_device_info_stub[0].address;

    ///* Change user configuration value to invalid */
    //gs_osal_device_info_stub[0].address = 0xFE870000U;

    //Test08_001_common(OSAL_TEST_ID012);

    ///* Revert origin value */
    //gs_osal_device_info_stub[0].address = backup_val;

	///* Clear stub flag */
	//g_stub_flag.R_OSAL_DCFG_GetDeviceInfo = 0;
	
	//osal_thread_deinitialize();
	//osal_mutex_deinitialize();
	//osal_cond_deinitialize();
	//osal_mq_deinitialize();
//}

/* ------------------------08_001_013 [OSAL_RETURN_FAIL]------------------------ */
static void Test08_001_013()
{
	/* Set stub flag */
	g_stub_flag.R_PMA_SetPowerPolicy = R_PMA_NG;

    Test08_001_common(OSAL_TEST_ID013);
	
	/* Clear stub flag */
	g_stub_flag.R_PMA_SetPowerPolicy = 0;
	
	osal_thread_deinitialize();
	osal_mutex_deinitialize();
	osal_cond_deinitialize();
	osal_mq_deinitialize();
	osal_io_deinitialize();
}

/* ------------------------08_001_014 [OSAL_RETURN_FAIL]------------------------ */
static void Test08_001_014()
{
	/* Set stub flag */
	g_stub_flag.R_OSAL_RCFG_GetInterruptThreadInfo = R_OSAL_ERR_NULL_CONTROL;

    Test08_001_common(OSAL_TEST_ID014);
	
	/* Clear stub flag */
	g_stub_flag.R_OSAL_RCFG_GetInterruptThreadInfo = 0;
	
	osal_thread_deinitialize();
	osal_mutex_deinitialize();
	osal_cond_deinitialize();
	osal_mq_deinitialize();
	osal_io_deinitialize();
	osal_pm_deinitialize();
}

/* ------------------------08_001_015 [OSAL_RETURN_CONF]------------------------ */
//static void Test08_001_015()
//{
    //e_osal_thread_priority_t backup_val;

	///* Set stub flag */
	//g_stub_flag.R_OSAL_RCFG_GetInterruptThreadInfo = R_OSAL_ERR_OTHER_CONTROL;

    ///* Backup value */
    //backup_val = gs_osal_interrupt_thread_config_stub[0].priority;

    ///* Change user configuration value to invalid */
    //gs_osal_interrupt_thread_config_stub[0].priority = (e_osal_thread_priority_t)(OSAL_INTERRUPT_PRIORITY_HIGHEST + 1);

    //Test08_001_common(OSAL_TEST_ID015);

    ///* Revert origin value */
    //gs_osal_interrupt_thread_config_stub[0].priority = backup_val;

	///* Clear stub flag */
	//g_stub_flag.R_OSAL_RCFG_GetInterruptThreadInfo = 0;
	
	//osal_thread_deinitialize();
	//osal_mutex_deinitialize();
	//osal_cond_deinitialize();
	//osal_mq_deinitialize();
	//osal_io_deinitialize();
	//osal_pm_deinitialize();
//}

///* ------------------------08_001_016 [OSAL_RETURN_CONF]------------------------ */
//static void Test08_001_016()
//{
    //e_osal_thread_priority_t backup_val;

	///* Set stub flag */
	//g_stub_flag.R_OSAL_RCFG_GetInterruptThreadInfo = R_OSAL_ERR_OTHER_CONTROL;

    ///* Backup value */
    //backup_val = gs_osal_interrupt_thread_config_stub[0].priority;

    ///* Change user configuration value to invalid */
    //gs_osal_interrupt_thread_config_stub[0].priority = (e_osal_thread_priority_t)(OSAL_INTERRUPT_PRIORITY_LOWEST - 1);

    //Test08_001_common(OSAL_TEST_ID016);

    ///* Revert origin value */
    //gs_osal_interrupt_thread_config_stub[0].priority = backup_val;

	///* Clear stub flag */
	//g_stub_flag.R_OSAL_RCFG_GetInterruptThreadInfo = 0;
	
	//osal_thread_deinitialize();
	//osal_mutex_deinitialize();
	//osal_cond_deinitialize();
	//osal_mq_deinitialize();
	//osal_io_deinitialize();
	//osal_pm_deinitialize();
//}

///* ------------------------08_001_017 [OSAL_RETURN_CONF]------------------------ */
//static void Test08_001_017()
//{
    //size_t backup_val;

	///* Set stub flag */
	//g_stub_flag.R_OSAL_RCFG_GetInterruptThreadInfo = R_OSAL_ERR_OTHER_CONTROL;

    ///* Backup value */
    //backup_val = gs_osal_interrupt_thread_config_stub[0].stack_size;

    ///* Change user configuration value to invalid */
    //gs_osal_interrupt_thread_config_stub[0].stack_size = 0;

    //Test08_001_common(OSAL_TEST_ID017);

    ///* Revert origin value */
    //gs_osal_interrupt_thread_config_stub[0].stack_size = backup_val;

	///* Clear stub flag */
	//g_stub_flag.R_OSAL_RCFG_GetInterruptThreadInfo = 0;
	
	//osal_thread_deinitialize();
	//osal_mutex_deinitialize();
	//osal_cond_deinitialize();
	//osal_mq_deinitialize();
	//osal_io_deinitialize();
	//osal_pm_deinitialize();
//}

/* ------------------------08_001_018 [OSAL_RETURN_STATE]------------------------ */
static void Test08_001_018()
{
    e_osal_return_t osal_ret;

    osal_ret = R_OSAL_Initialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_COMMON_INIT,
				OSAL_TEST_ID018);
		return;
	}
    Test08_001_common(OSAL_TEST_ID018);
	
    (void)R_OSAL_Deinitialize();
}

/* ------------------------08_001_019 [success]------------------------ */
static void Test08_001_019()
{
    e_osal_return_t osal_ret;

    osal_ret = R_OSAL_Initialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_COMMON_INIT,
				OSAL_TEST_ID019);
		return;
	}
    osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_COMMON_INIT,
				OSAL_TEST_ID019);
		return;
	}
    Test08_001_common(OSAL_TEST_ID019);
	
    (void)R_OSAL_Deinitialize();
}

/* ------------------------08_001_020 [OSAL_RETURN_MEM]------------------------ */
static void Test08_001_020()
{
	/* Set stub flag */
	g_stub_flag.TaskCreate = TASKCREATE_MEMORY;

    Test08_001_common(OSAL_TEST_ID020);
	
	/* Clear stub flag */
	g_stub_flag.TaskCreate = 0;
}

/* ------------------------08_001_021 [OSAL_RETURN_DEV]------------------------ */
static void Test08_001_021()
{
	/* Set stub flag */
	g_stub_flag.R_PMA_Init = R_PMA_RESET_TOGGLE_CHK_NG;

    Test08_001_common(OSAL_TEST_ID021);
	
	/* Clear stub flag */
	g_stub_flag.R_PMA_Init = 0;
	
	osal_thread_deinitialize();
	osal_mutex_deinitialize();
	osal_cond_deinitialize();
	osal_mq_deinitialize();
	osal_io_deinitialize();
}

/* ------------------------08_001_02 [OSAL_RETURN_OK]------------------------ */
static void Test08_001_022()
{	
	e_osal_return_t 		osal_ret;
	st_osal_thread_config_t config;
	osal_thread_handle_t 	handle;
	osal_thread_id_t 		thread_id = 1;

	/****************************/
	/* Pre process				*/
	/****************************/
	/* Set stub flag */
	g_stub_flag.R_OSAL_RCFG_GetNumOfThread = R_OSAL_ERR_INFO_DEVICE;

	/****************************/
	/* Test execution			*/
	/****************************/
    Test08_001_common(OSAL_TEST_ID022);

	/****************************/
	/* Post process				*/
	/****************************/
	/* Clear stub flag */
	g_stub_flag.R_OSAL_RCFG_GetNumOfThread = 0;

	/* Check thread resource */
	osal_ret = R_OSAL_ThreadCreate(&config, thread_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		printf("Thread resource is unavailable with error: %d \n", (int)osal_ret);
	}

	osal_clock_time_deinitialize();
	osal_mq_deinitialize();
	osal_mutex_deinitialize();
	osal_cond_deinitialize();
	osal_interrupt_deinitialize();
	osal_io_deinitialize();
	osal_pm_deinitialize();

}

/* 08_001: R_OSAL_Initialize */
static void osal_test_R_OSAL_Initialize()
{
    Test08_001_001();
    Test08_001_002();
    //Test08_001_003();
    //Test08_001_004();
    //Test08_001_005();
    Test08_001_006();
    Test08_001_007();
    Test08_001_008();
    //Test08_001_009();
    //Test08_001_010();
    Test08_001_011();
    //Test08_001_012();
    Test08_001_013();
    Test08_001_014();
    //Test08_001_015();
    //Test08_001_016();
    //Test08_001_017();
    Test08_001_018();
    Test08_001_019();
    Test08_001_020();
    Test08_001_021();
	Test08_001_022();
}

/* test CB func */
static void osal_test_cb_sample(osal_device_handle_t device_handle, uint64_t irq_channel, void *irq_arg)
{

	printf("%s() user call back function exec!! dev_handle:%p irq_channel:%ld  irq_arg:%p \n", 
	    __func__, (void *)device_handle, irq_channel, irq_arg);

	return;
}

static void Test08_002_common(int test_id)
{
    e_osal_return_t osal_ret;
    e_osal_return_t expect_data;
    int test_ret;
    char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_COMMON_FUNCTION, 
															OSAL_TEST_COMMON_DEINIT,
															test_id);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
    osal_ret = R_OSAL_Deinitialize();
	TIME_MEASURE_STOP
    if (osal_ret == expect_data)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_COMMON_FUNCTION, 
							OSAL_TEST_COMMON_DEINIT,
							test_id, 
							test_ret, opt_info);
}

/* ------------------------08_002_001 [success]------------------------ */
static void Test08_002_001()
{
    e_osal_return_t       	osal_ret;

    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", OSAL_TEST_COMMON_FUNCTION,
                             OSAL_TEST_COMMON_DEINIT, OSAL_TEST_ID001);
		return;
	}

	Test08_002_common(OSAL_TEST_ID001);
}

/* ------------------------08_002_002 [OSAL_RETURN_FAIL]------------------------ */
static void Test08_002_002()
{
    e_osal_return_t       	osal_ret;

    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", OSAL_TEST_COMMON_FUNCTION,
                             OSAL_TEST_COMMON_DEINIT, OSAL_TEST_ID002);
	}

	/* Set stub flag */
	g_stub_flag.R_OSAL_RCFG_GetMqInfo = R_OSAL_ERR_NULL_CONTROL;


	/****************************/
	/* Test execution			*/
	/****************************/
	Test08_002_common(OSAL_TEST_ID002);

	/* Clear stub flag */
	g_stub_flag.R_OSAL_RCFG_GetMqInfo = 0;

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", OSAL_TEST_COMMON_FUNCTION,
                             OSAL_TEST_COMMON_DEINIT, OSAL_TEST_ID002);
	}
}

/* ------------------------08_002_003 [OSAL_RETURN_FAIL]------------------------ */
static void Test08_002_003()
{
    e_osal_return_t       	osal_ret;

    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", OSAL_TEST_COMMON_FUNCTION,
                             OSAL_TEST_COMMON_DEINIT, OSAL_TEST_ID003);
	}

	/* Set stub flag */
	g_stub_flag.R_OSAL_RCFG_GetMutexInfo = R_OSAL_ERR_NULL_CONTROL;

	/****************************/
	/* Test execution			*/
	/****************************/
	Test08_002_common(OSAL_TEST_ID003);

	/* Clear stub flag */
	g_stub_flag.R_OSAL_RCFG_GetMutexInfo = 0;

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", OSAL_TEST_COMMON_FUNCTION,
                             OSAL_TEST_COMMON_DEINIT, OSAL_TEST_ID003);
	}
}

/* ------------------------08_002_004 [OSAL_RETURN_FAIL]------------------------ */
static void Test08_002_004()
{
    e_osal_return_t       	osal_ret;

    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", OSAL_TEST_COMMON_FUNCTION,
                             OSAL_TEST_COMMON_DEINIT, OSAL_TEST_ID004);
	}

	/* Set stub flag */
	g_stub_flag.R_OSAL_RCFG_GetCondInfo = R_OSAL_ERR_NULL_CONTROL;

	/****************************/
	/* Test execution			*/
	/****************************/
	Test08_002_common(OSAL_TEST_ID004);

	/* Clear stub flag */
	g_stub_flag.R_OSAL_RCFG_GetCondInfo = 0;

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", OSAL_TEST_COMMON_FUNCTION,
                             OSAL_TEST_COMMON_DEINIT, OSAL_TEST_ID004);
	}
}

/* ------------------------08_002_005 [OSAL_RETURN_FAIL]------------------------ */
static void Test08_002_005()
{
    e_osal_return_t       	osal_ret;

    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", OSAL_TEST_COMMON_FUNCTION,
                             OSAL_TEST_COMMON_DEINIT, OSAL_TEST_ID005);
	}

	/* Set stub flag */
	g_stub_flag.R_OSAL_RCFG_GetInterruptThreadInfo = R_OSAL_ERR_NULL_CONTROL;

	/****************************/
	/* Test execution			*/
	/****************************/
	Test08_002_common(OSAL_TEST_ID005);

	/* Clear stub flag */
	g_stub_flag.R_OSAL_RCFG_GetInterruptThreadInfo = 0;

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", OSAL_TEST_COMMON_FUNCTION,
                             OSAL_TEST_COMMON_DEINIT, OSAL_TEST_ID005);
	}
}

/* ------------------------08_002_006 [OSAL_RETURN_FAIL]------------------------ */
static void Test08_002_006()
{
    e_osal_return_t       	osal_ret;
	osal_device_handle_t 	dev_handle;
	int						arg = 10;

    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", OSAL_TEST_COMMON_FUNCTION,
                             OSAL_TEST_COMMON_DEINIT, OSAL_TEST_ID006);
	}
	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));
	osal_ret = R_OSAL_IoDeviceOpen("ims_01", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_COMMON_FUNCTION, OSAL_TEST_COMMON_DEINIT, OSAL_TEST_ID006);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
					       (p_osal_isr_func_t)osal_test_cb_sample,
					       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_COMMON_FUNCTION, OSAL_TEST_COMMON_DEINIT, OSAL_TEST_ID006);
		return;
	}
	osal_ret = R_OSAL_InterruptEnableIsr(dev_handle, 0);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptEnableIsr() error.",
				OSAL_TEST_COMMON_FUNCTION, OSAL_TEST_COMMON_DEINIT, OSAL_TEST_ID006);
		return;
	}


	/****************************/
	/* Test execution			*/
	/****************************/
	Test08_002_common(OSAL_TEST_ID006);

	

	osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, 0);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptDisableIsr() error.",
				OSAL_TEST_COMMON_FUNCTION, OSAL_TEST_COMMON_DEINIT,
				OSAL_TEST_ID006);
		return;
	}
	
	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_COMMON_FUNCTION, OSAL_TEST_COMMON_DEINIT,
				OSAL_TEST_ID006);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_COMMON_FUNCTION, OSAL_TEST_COMMON_DEINIT, OSAL_TEST_ID006);
		return;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", OSAL_TEST_COMMON_FUNCTION,
							 OSAL_TEST_COMMON_DEINIT, OSAL_TEST_ID006);
	}
}

/* ------------------------08_002_007 [OSAL_RETURN_FAIL]------------------------ */
static void Test08_002_007()
{
    e_osal_return_t       	osal_ret;

    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", OSAL_TEST_COMMON_FUNCTION,
                             OSAL_TEST_COMMON_DEINIT, OSAL_TEST_ID007);
	}

	/* Set stub flag */
	g_stub_flag.R_OSAL_RCFG_GetThreadInfo = R_OSAL_ERR_NULL_CONTROL;

	/****************************/
	/* Test execution			*/
	/****************************/
	Test08_002_common(OSAL_TEST_ID007);


	/* Clear stub flag */
	g_stub_flag.R_OSAL_RCFG_GetThreadInfo = 0;

	osal_ret = osal_thread_deinitialize();
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_io_deinitialize();
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_pm_deinitialize();
    }

	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", OSAL_TEST_COMMON_FUNCTION,
                             OSAL_TEST_COMMON_DEINIT, OSAL_TEST_ID007);
	}
}

/* ------------------------08_002_008 [OSAL_RETURN_FAIL]------------------------ */
static void Test08_002_008()
{
    e_osal_return_t       	osal_ret;

    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", OSAL_TEST_COMMON_FUNCTION,
                             OSAL_TEST_COMMON_DEINIT, OSAL_TEST_ID008);
	}

	/* Set stub flag */
	g_stub_flag.R_OSAL_DCFG_GetDeviceInfo = R_OSAL_ERR_NULL_CONTROL;


	/****************************/
	/* Test execution			*/
	/****************************/
	Test08_002_common(OSAL_TEST_ID008);

	/* Clear stub flag */
	g_stub_flag.R_OSAL_DCFG_GetDeviceInfo = 0;

	osal_ret = osal_io_deinitialize();
	if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_pm_deinitialize();
    }

	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", OSAL_TEST_COMMON_FUNCTION,
                             OSAL_TEST_COMMON_DEINIT, OSAL_TEST_ID008);
	}
}

/* ------------------------08_002_009 [OSAL_RETURN_DEV]------------------------ */
static void Test08_002_009()
{
    e_osal_return_t       	osal_ret;
	osal_device_handle_t    dev_handle;
 	int		                arg = 10;

    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", OSAL_TEST_COMMON_FUNCTION,
                             OSAL_TEST_COMMON_DEINIT, OSAL_TEST_ID009);
	}

	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));
	osal_ret = R_OSAL_IoDeviceOpen("ims_01", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{

			OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
							OSAL_TEST_COMMON_FUNCTION, OSAL_TEST_COMMON_DEINIT, OSAL_TEST_ID009);
			return;
	}
	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
											(p_osal_isr_func_t)osal_test_cb_sample,
											(void *)&arg);


	/* Set stub flag */
	g_stub_flag.drvISR_RemoveSPI = drvISR_BUS_INTERFACE_FAULT;

	/****************************/
	/* Test execution			*/
	/****************************/
	Test08_002_common(OSAL_TEST_ID009);


	/* Clear stub flag */
	g_stub_flag.drvISR_RemoveSPI = 0;

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", OSAL_TEST_COMMON_FUNCTION,
                             OSAL_TEST_COMMON_DEINIT, OSAL_TEST_ID009);
	}
}

/* ------------------------08_002_010 [OSAL_RETURN_FAIL]------------------------ */
static void Test08_002_010()
{
    e_osal_return_t       	osal_ret;

    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", OSAL_TEST_COMMON_FUNCTION,
                             OSAL_TEST_COMMON_DEINIT, OSAL_TEST_ID010);
	}

	/* Set stub flag */
	g_stub_flag.R_PMA_Quit = R_PMA_NG;


	/****************************/
	/* Test execution			*/
	/****************************/
	Test08_002_common(OSAL_TEST_ID010);

	/* Clear stub flag */
	g_stub_flag.R_PMA_Quit = 0;

	osal_ret = osal_pm_deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", OSAL_TEST_COMMON_FUNCTION,
                             OSAL_TEST_COMMON_DEINIT, OSAL_TEST_ID010);
	}
}

/* 08_002: R_OSAL_Deinitialize */
static void osal_test_R_OSAL_Deinitialize()
{
	Test08_002_001();
	Test08_002_002();
	Test08_002_003();
	Test08_002_004();
	Test08_002_005();
	Test08_002_006();
	Test08_002_007();
	Test08_002_008();
	Test08_002_009();
	Test08_002_010();
}
