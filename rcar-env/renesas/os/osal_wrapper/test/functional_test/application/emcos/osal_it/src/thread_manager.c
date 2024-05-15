/**
 * @file thread_manager.c
 * @brief OSAL Thread Manager evaluation file.
 * @author K.Takagi
 * @date 2019/11/12
 */

/* include */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "rcar-xos/osal/r_osal.h"
#include "osal_test.h"
#include "osal_test_common.h"
#include "thread_manager.h"
//#include "rcar-xos/tmu/tmu.h"


/* prototype */
static void osal_test_R_OSAL_ThreadCreate();
static void osal_test_R_OSAL_ThreadSleepUntilTimeStamp();
static void osal_test_R_OSAL_ThreadSleepForTimePeriod();
static void osal_test_R_OSAL_ThreadSelf();
static void osal_test_R_OSAL_ThreadEqual();
static void osal_test_R_OSAL_ThreadJoin();
static void osal_test_R_OSAL_ThreadInitializeThreadConfigSt();

/* global */
volatile osal_thread_handle_t g_thread_handle = NULL;
volatile osal_thread_handle_t g_join_handle = NULL;
volatile osal_thread_handle_t g_equal_handle1 = NULL;
volatile osal_thread_handle_t g_equal_handle2 = NULL;
volatile osal_mutex_handle_t g_equal_mutex = NULL;
volatile osal_cond_handle_t g_equal_cond = NULL;

typedef struct st_arg_cond_waitperiod {
	osal_milli_sec_t time;
	osal_cond_handle_t handle;
	osal_mutex_handle_t mutex_handle;
} st_arg_cond_waitperiod_t;


void osal_test_thread_manager_display_menu()
{

	printf("*** OSAL Thread Manager ***\n");
	printf("***  01_001: R_OSAL_ThreadCreate                  ***\n");
	printf("***  01_002: R_OSAL_ThreadSleepUntilTimeStamp     ***\n");
	printf("***  01_003: R_OSAL_ThreadSleepForTimePeriod      ***\n");
	printf("***  01_004: R_OSAL_ThreadSelf                    ***\n");
	printf("***  01_005: R_OSAL_ThreadEqual                   ***\n");
	printf("***  01_006: R_OSAL_ThreadJoin                    ***\n");
	printf("***  01_007: R_OSAL_ThreadInitializeThreadConfigSt***\n");
	printf("***  01_100: thread manager module all exec       ***\n\n");

}

void osal_test_thread_manager_menu(int child)
{

	int test_number = child;
	unsigned int test_count  = 1;
	unsigned int loop_count  = 0;

	/* Reset stub flags */
	//memset(&g_stub_flag, 0, sizeof(st_stub));

	/* ALL check */
	/* when select all */
	if (child == OSAL_TEST_ALL_EXEC)
	{
		test_number = OSAL_TEST_THREADCREATE;
		test_count  = OSAL_TEST_THREAD_MAX;
	}

	for (loop_count = 0; loop_count < test_count; loop_count++)
	{
		switch (test_number) 
		{
			case OSAL_TEST_THREADCREATE:
				osal_test_R_OSAL_ThreadCreate();
				break;
			case OSAL_TEST_THREADSLEEPUNTILTIMESTAMP:
				osal_test_R_OSAL_ThreadSleepUntilTimeStamp();
				break;
			case OSAL_TEST_THREADSLEEPFORTIMEPERIOD:
				osal_test_R_OSAL_ThreadSleepForTimePeriod();
				break;
			case OSAL_TEST_THREADSELF:
				osal_test_R_OSAL_ThreadSelf();
				break;
			case OSAL_TEST_THREADEQUAL:
				osal_test_R_OSAL_ThreadEqual();
				break;
			case OSAL_TEST_THREADJOIN:
				osal_test_R_OSAL_ThreadJoin();
				break;
			case OSAL_TEST_THREADINITIALIZETHREADCONFIGST:
				osal_test_R_OSAL_ThreadInitializeThreadConfigSt();
				break;
			default:
				printf("That menu is not available.\n\n");
				break;
		}

		test_number++;
	}
}

e_osal_return_t osal_init_thread(void)
{
	e_osal_return_t ret = OSAL_RETURN_OK;
	if(false == init_flag)
	{
		ret = R_OSAL_Initialize();
		if(ret == OSAL_RETURN_OK)
		{
			init_flag = true;
		}

	}

    return ret;
}

e_osal_return_t osal_deinit_thread(void)
{
	e_osal_return_t ret = OSAL_RETURN_OK;
	if(true == init_flag)
	{
		ret = R_OSAL_Deinitialize();
		if(ret == OSAL_RETURN_OK)
		{
			init_flag = false;
		}
	}

    return ret;
}

/* test thread */
int64_t osal_test_thread_sample(void *user_arg)
{
	(void)user_arg;
	//if(NULL != user_arg)

	/* wait until the main task calls R_OSAL_ThreadJoin() */
	R_OSAL_ThreadSleepForTimePeriod(2500);

	return 0;
}

/* test thread */
int64_t osal_test_thread_equal(void *user_arg)
{
	e_osal_return_t			osal_ret;
	osal_thread_handle_t	handle;
	bool					result = false;
	e_osal_return_t       	expect_data;
	int 					test_ret = OSAL_TEST_OK;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	(void)user_arg;

	/* wait until the main task calls R_OSAL_ThreadJoin() */
	R_OSAL_ThreadSleepForTimePeriod(2500);
	memset(&handle, 0x00, sizeof(osal_thread_handle_t));
	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADEQUAL,
														   OSAL_TEST_ID002);
	osal_ret = R_OSAL_ThreadSelf(&handle);
	if(OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadSelf() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, OSAL_TEST_ID002);
		osal_ret = 0;
	}
	else
	{
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_ThreadEqual(handle, g_equal_handle1, &result);
		TIME_MEASURE_STOP
		if((osal_ret == expect_data) && (result == false))
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
			test_ret = OSAL_TEST_NG;
		}

		OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)osal_ret));

		osal_test_result_output(OSAL_TEST_THREAD, 
								OSAL_TEST_THREADEQUAL,
								OSAL_TEST_ID002, 
								test_ret, opt_info);
	}

	return (int64_t)test_ret;
}

#if 0
void Test01_005_007_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType, drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	e_osal_return_t       		osal_ret;
	e_osal_return_t       		expect_data;
	bool						result = true;
	int 						test_ret = OSAL_TEST_OK;
	char                		opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADEQUAL,
														   OSAL_TEST_ID007);
	if (IntType == TMU_INT_UNDERFLOW)
	{
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_ThreadEqual(g_equal_handle1, g_equal_handle2, &result);
		TIME_MEASURE_STOP
		if ((osal_ret == expect_data) && (result == false))
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
			test_ret = OSAL_TEST_NG;
		}

		OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)osal_ret));

		osal_test_result_output(OSAL_TEST_THREAD, 
								OSAL_TEST_THREADEQUAL,
								OSAL_TEST_ID007, 
								test_ret, opt_info);
	}

	R_OSAL_ThsyncMutexLockForTimePeriod(g_equal_mutex, 10000);
	R_OSAL_ThsyncCondSignal(g_equal_cond);
	R_OSAL_ThsyncMutexUnlock(g_equal_mutex);
}

void Test01_005_008_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType, drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	e_osal_return_t       		osal_ret;
	e_osal_return_t       		expect_data;
	bool						result = true;
	int 						test_ret = OSAL_TEST_OK;
	char                		opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADEQUAL,
														   OSAL_TEST_ID008);
	if (IntType == TMU_INT_UNDERFLOW)
	{
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_ThreadEqual(g_equal_handle1, g_equal_handle2, &result);
		TIME_MEASURE_STOP
		if ((osal_ret == expect_data) && (result == true))
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
			test_ret = OSAL_TEST_NG;
		}

		OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)osal_ret));

		osal_test_result_output(OSAL_TEST_THREAD, 
								OSAL_TEST_THREADEQUAL,
								OSAL_TEST_ID008, 
								test_ret, opt_info);

		R_OSAL_ThsyncMutexLockForTimePeriod(g_equal_mutex, 10000);
		R_OSAL_ThsyncCondSignal(g_equal_cond);
		R_OSAL_ThsyncMutexUnlock(g_equal_mutex);
	}
}

int64_t osal_test_thread_sample_interrupt_005_007(void *user_arg)
{
	UINT16 Result;
	drvTMU_Time_t Period;

	(void)user_arg;

	Period.Sec = 2;
	Period.Usec = 0;

	Result = drvTMU_Start(0, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test01_005_007_CallbackFunc, TMU_EDGE_RISE);
	if(Result)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, OSAL_TEST_ID007);
	}

	R_OSAL_ThsyncMutexTryLock(g_equal_mutex);
	R_OSAL_ThsyncCondWaitForTimePeriod(g_equal_cond, g_equal_mutex, 10000);
	R_OSAL_ThsyncMutexUnlock(g_equal_mutex);

	return 0;
}

int64_t osal_test_thread_sample_interrupt_005_008(void *user_arg)
{
	UINT16 Result;
	drvTMU_Time_t Period;
	osal_thread_handle_t self_handle;

	(void)user_arg;
	memset(&self_handle, 0x00, sizeof(osal_thread_handle_t));

	Period.Sec = 2;
	Period.Usec = 0;
	
	R_OSAL_ThreadSelf(&self_handle);
	g_equal_handle2 = self_handle;

	Result = drvTMU_Start(0, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test01_005_008_CallbackFunc, TMU_EDGE_RISE);
	if(Result)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, OSAL_TEST_ID007);
	}

	R_OSAL_ThsyncMutexTryLock(g_equal_mutex);
	R_OSAL_ThsyncCondWaitForTimePeriod(g_equal_cond, g_equal_mutex, 10000);
	R_OSAL_ThsyncMutexUnlock(g_equal_mutex);

	return 0;
}
#endif

/* test thread_self. use 01_004_001 */
int64_t osal_test_self_subthread(void *user_arg)
{
	e_osal_return_t       osal_ret = OSAL_RETURN_FAIL;
	osal_thread_handle_t *self_handle;

    if(NULL != user_arg)
	{
		self_handle = (osal_thread_handle_t *)user_arg;
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_ThreadSelf(self_handle);
		TIME_MEASURE_STOP
	}
	/* wait until the main task calls R_OSAL_ThreadJoin() */
	R_OSAL_ThreadSleepForTimePeriod(1000);

	return (int64_t)osal_ret;
}

/* test thread_join. use 01_006_004 */
int64_t osal_test_thread_join(void *user_arg)
{
	(void)user_arg;
	osal_thread_handle_t 	self_handle;
	e_osal_return_t       	osal_ret;
	int64_t					return_value;

	memset(&self_handle, 0x00, sizeof(osal_thread_handle_t));
	R_OSAL_ThreadSelf(&self_handle);
	osal_ret = R_OSAL_ThreadJoin(self_handle, &return_value);

	/* wait until the main task calls R_OSAL_ThreadJoin() */
	R_OSAL_ThreadSleepForTimePeriod(2000);

	return (int64_t)osal_ret;
}

int64_t osal_test_thread_join_1(void *user_arg)
{
	osal_thread_handle_t 	handle;
	e_osal_return_t       	osal_ret;
	int64_t					return_value;	
	st_arg_cond_waitperiod_t	*arg_cond;
	osal_mutex_handle_t 		mutex_handle;
	osal_cond_handle_t  		cond_handle;

	arg_cond = (st_arg_cond_waitperiod_t *)user_arg;
	mutex_handle = arg_cond->mutex_handle;
	cond_handle = arg_cond->handle;

	memset(&handle, 0x00, sizeof(osal_thread_handle_t));
	R_OSAL_ThsyncMutexTryLock(mutex_handle);
	osal_ret = R_OSAL_ThsyncCondWaitForTimePeriod(cond_handle, mutex_handle, arg_cond->time);
	R_OSAL_ThsyncMutexUnlock(mutex_handle);
	handle = g_join_handle;
	osal_ret = R_OSAL_ThreadJoin(handle, &return_value);

	return (int64_t)osal_ret;
}

int64_t osal_test_thread_join_2(void *user_arg)
{
	(void)user_arg;
	osal_thread_handle_t 	handle3;
	st_osal_thread_config_t config3;
	e_osal_return_t       	osal_ret;
	int						arg = 10;
	osal_cond_handle_t 		cond_handle;
	osal_mutex_handle_t 	mutex_handle;
	st_arg_cond_waitperiod_t	*arg_cond;


	arg_cond = (st_arg_cond_waitperiod_t *)user_arg;
	mutex_handle = arg_cond->mutex_handle;
	cond_handle = arg_cond->handle;

	memset(&config3, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle3, 0x00, sizeof(osal_thread_handle_t));

	config3.func       = osal_test_thread_sample;
	config3.userarg    = (void *)&arg;
	config3.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config3.task_name  = "task_01_006_006_03";
	config3.stack_size = 0x1000;

	osal_ret = R_OSAL_ThreadCreate(&config3, 9, &handle3);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADCREATE, OSAL_TEST_ID006);
		return (int64_t)osal_ret;
	}

	g_join_handle = handle3;
	/* mutex lock */
	R_OSAL_ThsyncMutexLockForTimePeriod(mutex_handle, 10000);

	/* cond signal */
	osal_ret = R_OSAL_ThsyncCondSignal(cond_handle);

	R_OSAL_ThsyncMutexUnlock(mutex_handle);
	return (int64_t)osal_ret;
}

int64_t osal_test_thread_join_3(void *user_arg)
{
	(void)user_arg;
	e_osal_return_t       	osal_ret;
	int64_t					return_value;

	/* wait until the main task calls R_OSAL_ThreadJoin() */
	R_OSAL_ThreadSleepForTimePeriod(3000);
	osal_ret = R_OSAL_ThreadJoin(g_join_handle, &return_value);

	return (int64_t)osal_ret;
}

int64_t osal_test_equal_subthread(void *user_arg)
{
	(void)user_arg;
	osal_thread_handle_t 	self_handle;
	e_osal_return_t       	osal_ret = OSAL_RETURN_FAIL;
	bool                	result = false;
    
	memset(&self_handle, 0x00, sizeof(osal_thread_handle_t));
	R_OSAL_ThreadSelf(&self_handle);

	R_OSAL_ThreadSleepForTimePeriod(1000); // need sleep to wait R_OSAL_ThreadCreate finish

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadEqual(g_thread_handle, self_handle, &result);
	TIME_MEASURE_STOP
	if ((result == true) && (osal_ret == OSAL_RETURN_OK))
	{
		osal_ret = OSAL_RETURN_OK;
	}
	else
	{
		printf("R_OSAL_ThreadEqual failed.\n");
		osal_ret = OSAL_RETURN_FAIL;
	}

	/* wait until the main task calls R_OSAL_ThreadJoin() */
	R_OSAL_ThreadSleepForTimePeriod(1000);
    
	return (int64_t)osal_ret;
}

/* ------------------------01_001_001 [success]------------------------ */
static void Test01_001_001(st_osal_thread_config_t config, osal_thread_id_t thread_id, osal_thread_handle_t handle)
{
	int64_t             return_value;
	char                opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t     osal_ret;
	int64_t 			arg;
	e_osal_return_t 	expect_data;
	int 				test_ret = OSAL_TEST_OK;

	arg = 1;
	config.func       = osal_test_thread_sample;
	config.userarg    = (void *)&arg;
	config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config.task_name  = "task_01_001_001";
	config.stack_size = 0x1000;

	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADCREATE,
														   OSAL_TEST_ID001);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadCreate(&config, thread_id, &handle);
	TIME_MEASURE_STOP

	if ((osal_ret == expect_data) && (handle != NULL)) 
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADCREATE,
							OSAL_TEST_ID001, 
							test_ret, opt_info);

	osal_ret = R_OSAL_ThreadJoin(handle, &return_value);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADCREATE, OSAL_TEST_ID001);
		return;
	}
}
/* ------------------------01_001_002 [success]------------------------ */
static void Test01_001_002(st_osal_thread_config_t config,  osal_thread_id_t thread_id, osal_thread_handle_t handle)
{
	int64_t             return_value;
	char                opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t     osal_ret;
	e_osal_return_t 	expect_data;
	int 				test_ret = OSAL_TEST_OK;

	config.func       = osal_test_thread_sample;
	config.userarg    = NULL;
	config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config.task_name  = "task_01_001_002";
	config.stack_size = 0x1000;

	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADCREATE,
														   OSAL_TEST_ID002);
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadCreate(&config, thread_id, &handle);
	TIME_MEASURE_STOP

	if ((osal_ret == expect_data) && (handle != NULL)) 
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADCREATE,
							OSAL_TEST_ID002, 
							test_ret, opt_info);

	osal_ret = R_OSAL_ThreadJoin(handle, &return_value);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADCREATE, OSAL_TEST_ID002);
		return;
	}
}
/* ------------------------01_001_003 [success]------------------------ */
static void Test01_001_003(st_osal_thread_config_t config,  osal_thread_id_t thread_id, osal_thread_handle_t handle)
{
	int64_t             return_value;
	char                opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t     osal_ret;
	e_osal_return_t 	expect_data;
	int 				test_ret = OSAL_TEST_OK;

	config.func       = osal_test_thread_sample;
	config.userarg    = NULL;
	config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config.task_name  = "task_01_001_003";
	config.stack_size = 0x1000;

	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADCREATE,
														   OSAL_TEST_ID003);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadCreate(&config, thread_id, &handle);
	TIME_MEASURE_STOP

	if ((osal_ret == expect_data) && (handle != NULL)) 
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADCREATE,
							OSAL_TEST_ID003, 
							test_ret, opt_info);

	osal_ret = R_OSAL_ThreadJoin(handle, &return_value);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADCREATE, OSAL_TEST_ID003);
		return;
	}
}
/* ------------------------01_001_004 [success]------------------------ */
static void Test01_001_004(st_osal_thread_config_t config,  osal_thread_id_t thread_id, osal_thread_handle_t handle)
{
	int64_t             return_value;
	char                opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t     osal_ret;
	e_osal_return_t 	expect_data;
	int 				test_ret = OSAL_TEST_OK;

	config.func       = osal_test_thread_sample;
	config.userarg    = NULL;
	config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config.task_name  = "task_01_001_004";
	config.stack_size = 0x1000;

	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADCREATE,
														   OSAL_TEST_ID004);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadCreate(&config, thread_id, &handle);
	TIME_MEASURE_STOP

	if ((osal_ret == expect_data) && (handle != NULL)) 
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADCREATE,
							OSAL_TEST_ID004, 
							test_ret, opt_info);

	if(osal_ret == OSAL_RETURN_OK)
	{
		osal_ret = R_OSAL_ThreadJoin(handle, &return_value);
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
				OSAL_TEST_THREAD, OSAL_TEST_THREADCREATE, OSAL_TEST_ID004);
			return;
		}
	}
}
/* ------------------------01_001_005 [Err: OSAL_RETURN_OK]------------------------ */
static void Test01_001_005(st_osal_thread_config_t config,  osal_thread_id_t thread_id, osal_thread_handle_t handle)
{
	(void)config;
	char                opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t     osal_ret;
	e_osal_return_t 	expect_data;
	int64_t				return_value;
	int 				test_ret 	= OSAL_TEST_OK;

	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADCREATE,
														   OSAL_TEST_ID005);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadCreate(NULL, thread_id, &handle);
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

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADCREATE,
							OSAL_TEST_ID005, 
							test_ret, opt_info);

	if(osal_ret == OSAL_RETURN_OK)
	{
		osal_ret = R_OSAL_ThreadJoin(handle, &return_value);
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
				OSAL_TEST_THREAD, OSAL_TEST_THREADCREATE, OSAL_TEST_ID005);
			return;
		}
	}
}
/* ------------------------01_001_006 [Err: OSAL_RETURN_PAR]------------------------ */
static void Test01_001_006(st_osal_thread_config_t config,  osal_thread_id_t thread_id, osal_thread_handle_t handle)
{
	(void)handle;
	char                opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t     osal_ret;
	e_osal_return_t 	expect_data;
	int 				test_ret = OSAL_TEST_OK;

	config.func       = osal_test_thread_sample;
	config.userarg    = NULL;
	config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config.task_name  = "task_01_001_006";
	config.stack_size = 0x1000;

	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADCREATE,
														   OSAL_TEST_ID006);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadCreate(&config, thread_id, NULL);
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

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADCREATE,
							OSAL_TEST_ID006, 
							test_ret, opt_info);
}
/* ------------------------01_001_007 [Err: OSAL_RETURN_PAR]------------------------ */
static void Test01_001_007(st_osal_thread_config_t config,  osal_thread_id_t thread_id, osal_thread_handle_t handle)
{
	char                opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t     osal_ret;
	e_osal_return_t 	expect_data;
	int 				test_ret = OSAL_TEST_OK;

	config.func       = osal_test_thread_sample;
	config.userarg    = NULL;
	config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config.task_name  = "task_01_001_007";
	config.stack_size = 0x1000;

	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADCREATE,
														   OSAL_TEST_ID007);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadCreate(&config, thread_id, &handle);
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

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADCREATE,
							OSAL_TEST_ID007, 
							test_ret, opt_info);
}
/* ------------------------01_001_008 [Err: OSAL_RETURN_PAR]------------------------ */
static void Test01_001_008(st_osal_thread_config_t config,  osal_thread_id_t thread_id, osal_thread_handle_t handle)
{
	char                opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t     osal_ret;
	e_osal_return_t 	expect_data;
	int 				test_ret = OSAL_TEST_OK;

	config.func       = osal_test_thread_sample;
	config.userarg    = NULL;
	config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config.task_name  = "task_01_001_008";
	config.stack_size = 0x1000;

	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADCREATE,
														   OSAL_TEST_ID008);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadCreate(&config, thread_id, &handle);
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

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADCREATE,
							OSAL_TEST_ID008, 
							test_ret, opt_info);
}

/* ------------------------01_001_009 [Err: OSAL_RETURN_BUSY]------------------------ */
static void Test01_001_009(st_osal_thread_config_t config,  osal_thread_id_t thread_id, osal_thread_handle_t handle)
{
	char                opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t     osal_ret;
	e_osal_return_t		expect_data;
	int 				test_ret = OSAL_TEST_OK;
	int64_t				return_value;
	osal_thread_handle_t handle2;

	config.func       = osal_test_thread_sample;
	config.userarg    = NULL;
	config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config.task_name  = "task_01_001_010_1";
	config.stack_size = 0x1000;
	memset(&handle2, 0x00, sizeof(osal_thread_handle_t));

	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADCREATE,
														   OSAL_TEST_ID009);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadCreate(&config, thread_id, &handle);
	TIME_MEASURE_STOP

	config.func       = osal_test_thread_sample;
	config.task_name  = "task_01_001_010_2";

	osal_ret = R_OSAL_ThreadCreate(&config, thread_id, &handle2);

	if (osal_ret == expect_data) 
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADCREATE,
							OSAL_TEST_ID009, 
							test_ret, opt_info);

	osal_ret = R_OSAL_ThreadJoin(handle, &return_value);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADCREATE, OSAL_TEST_ID009);
		return;
	}
}

/* ------------------------01_001_010 [Err: OSAL_RETURN_STATE]------------------------ */
static void Test01_001_010(st_osal_thread_config_t config,  osal_thread_id_t thread_id, osal_thread_handle_t handle)
{
	char                opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t     osal_ret;
	e_osal_return_t		expect_data;
	int 				test_ret = OSAL_TEST_OK;

	config.func       = osal_test_thread_sample;
	config.userarg    = NULL;
	config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config.task_name  = "task_01_001_011";
	config.stack_size = 0x1000;

	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADCREATE,
														   OSAL_TEST_ID010);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadCreate(&config, thread_id, &handle);
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

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADCREATE,
							OSAL_TEST_ID010, 
							test_ret, opt_info);
}

/* ------------------------01_001_011 [Err: OSAL_RETURN_STATE]------------------------ */
static void Test01_001_011(st_osal_thread_config_t config,  osal_thread_id_t thread_id, osal_thread_handle_t handle)
{
	char                opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t     osal_ret;
	e_osal_return_t		expect_data;
	int 				test_ret = OSAL_TEST_OK;

	config.func       = osal_test_thread_sample;
	config.userarg    = NULL;
	config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config.task_name  = "task_01_001_011";
	config.stack_size = 0x1000;

	osal_ret = osal_deinit_thread();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADCREATE, OSAL_TEST_ID011);
		return;
	}

	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADCREATE,
														   OSAL_TEST_ID011);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadCreate(&config, thread_id, &handle);
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

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADCREATE,
							OSAL_TEST_ID011, 
							test_ret, opt_info);
}

#if 0
/* ------------------------01_001_012 [Err: OSAL_RETURN_FAIL]------------------------ */
static void Test01_001_012(st_osal_thread_config_t config,  osal_thread_id_t thread_id, osal_thread_handle_t handle)
{
	char                opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t     osal_ret;
	e_osal_return_t		expect_data;
	int64_t 			arg;
	int 				test_ret = OSAL_TEST_OK;

	arg = NULL;
	config.func       = osal_test_thread_sample;
	config.userarg    = (void *)&arg;
	config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config.task_name  = "task_01_001_012";
	config.stack_size = 0x1000;

	osal_ret = osal_init_thread();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADCREATE, OSAL_TEST_ID012);
		return;
	}

	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADCREATE,
														   OSAL_TEST_ID012);

	/* Set stub flag */
	g_stub_flag.TaskNotify = TASKNOTIFY_pdFAIL;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadCreate(&config, thread_id, &handle);
	TIME_MEASURE_STOP

	/* Clear stub flag */
	g_stub_flag.TaskNotify = 0;

	if (osal_ret == expect_data) 
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADCREATE,
							OSAL_TEST_ID012, 
							test_ret, opt_info);

	osal_ret = osal_deinit_thread();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADCREATE, OSAL_TEST_ID012);
		return;
	}
}
#endif

/* ------------------------01_002_001 [success]------------------------ */
static void Test01_002_001()
{
	char             	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t  	osal_ret, osal_ret1;
	st_osal_time_t	 	current_time;
	e_osal_return_t		expect_data;
	int					test_ret = OSAL_TEST_OK;
	st_osal_time_t 		absolute_time;
	osal_nano_sec_t		diff_time;

	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADSLEEPUNTILTIMESTAMP,
														   OSAL_TEST_ID001);
	memset(&current_time, 0x00, sizeof(st_osal_time_t));
	memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
	osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
				OSAL_TEST_THREAD, OSAL_TEST_THREADSLEEPUNTILTIMESTAMP, OSAL_TEST_ID001);
		return;
	}

	if(current_time.tv_nsec + 2000000 <= 999999999)
	{
		absolute_time.tv_nsec = current_time.tv_nsec + 2000000;
		absolute_time.tv_sec = current_time.tv_sec + 1;
	}
	else
	{
		absolute_time.tv_sec = current_time.tv_sec + 2;
		absolute_time.tv_nsec = 2000000 - (999999999 - current_time.tv_nsec);
	}
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadSleepUntilTimeStamp(&absolute_time);
	TIME_MEASURE_STOP

	osal_ret1 = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &absolute_time);
	if (osal_ret1 != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
				OSAL_TEST_THREAD, OSAL_TEST_THREADSLEEPUNTILTIMESTAMP, OSAL_TEST_ID001);
		return;
	}

	osal_ret1 = R_OSAL_ClockTimeCalculateTimeDifference(&absolute_time, &current_time, &diff_time);
	if (osal_ret1 != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeCalculateTimeDifference() error.",
				OSAL_TEST_THREAD, OSAL_TEST_THREADSLEEPUNTILTIMESTAMP, OSAL_TEST_ID001);
		return;
	}

	if ((osal_ret == expect_data) && (diff_time >= ((10^9) + 2 * (10^6))))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADSLEEPUNTILTIMESTAMP,
							OSAL_TEST_ID001, 
							test_ret, opt_info);
}

/* ------------------------01_002_002 [success]------------------------ */
static void Test01_002_002()
{
	char             	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t  	osal_ret, osal_ret1;
	st_osal_time_t	 	current_time;
	e_osal_return_t		expect_data;
	int					test_ret = OSAL_TEST_OK;
	st_osal_time_t 		absolute_time;
	osal_nano_sec_t		diff_time;

	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADSLEEPUNTILTIMESTAMP,
														   OSAL_TEST_ID002);
	memset(&current_time, 0x00, sizeof(st_osal_time_t));
	memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
	osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
				OSAL_TEST_THREAD, OSAL_TEST_THREADSLEEPUNTILTIMESTAMP, OSAL_TEST_ID002);
		return;
	}

	if(current_time.tv_nsec + 1000000 <= 999999999)
	{
		absolute_time.tv_nsec = current_time.tv_nsec + 1000000;
		absolute_time.tv_sec = current_time.tv_sec + 1;
	}
	else
	{
		absolute_time.tv_sec = current_time.tv_sec + 2;
		absolute_time.tv_nsec = 1000000 - (999999999 - current_time.tv_nsec);
	}
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadSleepUntilTimeStamp(&absolute_time);
	TIME_MEASURE_STOP

	osal_ret1 = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &absolute_time);
	if (osal_ret1 != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
				OSAL_TEST_THREAD, OSAL_TEST_THREADSLEEPUNTILTIMESTAMP, OSAL_TEST_ID002);
		return;
	}

	osal_ret1 = R_OSAL_ClockTimeCalculateTimeDifference(&absolute_time, &current_time, &diff_time);
	if (osal_ret1 != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeCalculateTimeDifference() error.",
				OSAL_TEST_THREAD, OSAL_TEST_THREADSLEEPUNTILTIMESTAMP, OSAL_TEST_ID002);
		return;
	}

	if ((osal_ret == expect_data) && (diff_time >= ((10^9) + (10^6))))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADSLEEPUNTILTIMESTAMP,
							OSAL_TEST_ID002, 
							test_ret, opt_info);
}

/* ------------------------01_002_003 [success]------------------------ */
static void Test01_002_003()
{
	char             	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t  	osal_ret, osal_ret1;
	e_osal_return_t		expect_data;
	st_osal_time_t	 	current_time;
	int					test_ret = OSAL_TEST_OK;
	st_osal_time_t 		absolute_time;
	osal_nano_sec_t		diff_time;

	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADSLEEPUNTILTIMESTAMP,
														   OSAL_TEST_ID003);	

	memset(&current_time, 0x00, sizeof(st_osal_time_t));
	memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
	osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
				OSAL_TEST_THREAD, OSAL_TEST_THREADSLEEPUNTILTIMESTAMP, OSAL_TEST_ID003);
		return;
	}

	absolute_time.tv_sec = current_time.tv_sec + 1;
	absolute_time.tv_nsec = 999999999;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadSleepUntilTimeStamp(&absolute_time);
	TIME_MEASURE_STOP

	osal_ret1 = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &absolute_time);
	if (osal_ret1 != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
				OSAL_TEST_THREAD, OSAL_TEST_THREADSLEEPUNTILTIMESTAMP, OSAL_TEST_ID003);
		return;
	}

	osal_ret1 = R_OSAL_ClockTimeCalculateTimeDifference(&absolute_time, &current_time, &diff_time);
	if (osal_ret1 != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeCalculateTimeDifference() error.",
				OSAL_TEST_THREAD, OSAL_TEST_THREADSLEEPUNTILTIMESTAMP, OSAL_TEST_ID003);
		return;
	}

	if ((osal_ret == expect_data) && (diff_time >= ((10^9) + 999999999)))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADSLEEPUNTILTIMESTAMP,
							OSAL_TEST_ID003, 
							test_ret, opt_info);
}

/* ------------------------01_002_004 [success]------------------------ */
static void Test01_002_004()
{
	char             	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t  	osal_ret, osal_ret1;
	e_osal_return_t		expect_data;
	st_osal_time_t	 	current_time;
	int					test_ret = OSAL_TEST_OK;
	st_osal_time_t 		absolute_time;
	osal_nano_sec_t		diff_time;

	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADSLEEPUNTILTIMESTAMP,
														   OSAL_TEST_ID004);
	memset(&current_time, 0x00, sizeof(st_osal_time_t));
	memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
	osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
				OSAL_TEST_THREAD, OSAL_TEST_THREADSLEEPUNTILTIMESTAMP, OSAL_TEST_ID004);
		return;
	}

	if(current_time.tv_nsec + 999999 <= 999999999)
	{
		absolute_time.tv_nsec = current_time.tv_nsec + 999999;
		absolute_time.tv_sec = current_time.tv_sec + 1;
	}
	else
	{
		absolute_time.tv_sec = current_time.tv_sec + 2;
		absolute_time.tv_nsec = 999999 - (999999999 - current_time.tv_nsec);
	}
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadSleepUntilTimeStamp(&absolute_time);
	TIME_MEASURE_STOP

	osal_ret1 = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &absolute_time);
	if (osal_ret1 != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
				OSAL_TEST_THREAD, OSAL_TEST_THREADSLEEPUNTILTIMESTAMP, OSAL_TEST_ID004);
		return;
	}

	osal_ret1 = R_OSAL_ClockTimeCalculateTimeDifference(&absolute_time, &current_time, &diff_time);
	if (osal_ret1 != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeCalculateTimeDifference() error.",
				OSAL_TEST_THREAD, OSAL_TEST_THREADSLEEPUNTILTIMESTAMP, OSAL_TEST_ID004);
		return;
	}

	if ((osal_ret == expect_data) && (diff_time >= ((10^9) + 999999)))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADSLEEPUNTILTIMESTAMP,
							OSAL_TEST_ID004, 
							test_ret, opt_info);
}

/* ------------------------01_002_005 [OSAL_RETURN_PAR]------------------------ */
static void Test01_002_005()
{
	char             	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t  	osal_ret;
	e_osal_return_t		expect_data;
	int					test_ret = OSAL_TEST_OK;

	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADSLEEPUNTILTIMESTAMP,
														   OSAL_TEST_ID005);
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadSleepUntilTimeStamp(NULL);
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

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADSLEEPUNTILTIMESTAMP,
							OSAL_TEST_ID005, 
							test_ret, opt_info);
}

/* ------------------------01_002_006 [OSAL_RETURN_PAR]------------------------ */
static void Test01_002_006()
{
	char             	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t  	osal_ret;
	e_osal_return_t		expect_data;
	st_osal_time_t	 	current_time;
	int					test_ret = OSAL_TEST_OK;
	st_osal_time_t 		absolute_time;

	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADSLEEPUNTILTIMESTAMP,
														   OSAL_TEST_ID006);
	memset(&current_time, 0x00, sizeof(st_osal_time_t));
	memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
	osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
				OSAL_TEST_THREAD, OSAL_TEST_THREADSLEEPUNTILTIMESTAMP, OSAL_TEST_ID006);
		return;
	}

	absolute_time.tv_sec = current_time.tv_sec + 1;
	absolute_time.tv_nsec = 1000000000;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadSleepUntilTimeStamp(&absolute_time);
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

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADSLEEPUNTILTIMESTAMP,
							OSAL_TEST_ID006, 
							test_ret, opt_info);
}

/* ------------------------01_002_007 [OSAL_RETURN_PAR]------------------------ */
static void Test01_002_007()
{
	char             	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t  	osal_ret;
	e_osal_return_t		expect_data;
	st_osal_time_t	 	current_time;
	int					test_ret = OSAL_TEST_OK;
	st_osal_time_t 		absolute_time;

	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADSLEEPUNTILTIMESTAMP,
														   OSAL_TEST_ID007);
	memset(&current_time, 0x00, sizeof(st_osal_time_t));
	memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
	osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
				OSAL_TEST_THREAD, OSAL_TEST_THREADSLEEPUNTILTIMESTAMP, OSAL_TEST_ID007);
		return;
	}

	if(current_time.tv_nsec >= 2000000)
	{
		absolute_time.tv_sec = current_time.tv_sec;
		absolute_time.tv_nsec = current_time.tv_nsec - 2000000;
	}
	else
	{
		absolute_time.tv_sec = current_time.tv_sec - 1;
		absolute_time.tv_nsec = 999999999 - (2000000 - current_time.tv_nsec);
	}
	
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadSleepUntilTimeStamp(&absolute_time);
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

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADSLEEPUNTILTIMESTAMP,
							OSAL_TEST_ID007, 
							test_ret, opt_info);
}

/* ------------------------01_003_001 [success]------------------------ */
static void Test01_003_001(osal_milli_sec_t time)
{
	e_osal_return_t   	osal_ret, osal_ret1;
	e_osal_return_t   	expect_data;
	st_osal_time_t	 	current_time_1st, current_time_2nd;
	char            	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int test_ret = OSAL_TEST_OK;
	osal_nano_sec_t		diff_time;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADSLEEPFORTIMEPERIOD,
														   OSAL_TEST_ID001);
	memset(&current_time_1st, 0x00, sizeof(st_osal_time_t));
	memset(&current_time_2nd, 0x00, sizeof(st_osal_time_t));
	osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_1st);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
				OSAL_TEST_THREAD, OSAL_TEST_THREADSLEEPFORTIMEPERIOD, OSAL_TEST_ID001);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadSleepForTimePeriod(time);
	TIME_MEASURE_STOP

	osal_ret1 = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_2nd);
	if (osal_ret1 != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
				OSAL_TEST_THREAD, OSAL_TEST_THREADSLEEPFORTIMEPERIOD, OSAL_TEST_ID001);
		return;
	}

	osal_ret1 = R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &diff_time);
	if (osal_ret1 != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeCalculateTimeDifference() error.",
				OSAL_TEST_THREAD, OSAL_TEST_THREADSLEEPFORTIMEPERIOD, OSAL_TEST_ID001);
		return;
	}

	if ((osal_ret == expect_data) && (diff_time >= 1000000))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_THREAD,
							OSAL_TEST_THREADSLEEPFORTIMEPERIOD,
							OSAL_TEST_ID001, 
							test_ret, opt_info);														   	
}

/* ------------------------01_003_002 [success]------------------------ */
static void Test01_003_002(osal_milli_sec_t time)
{
	e_osal_return_t   	osal_ret; 
	e_osal_return_t		osal_ret1;
	e_osal_return_t   	expect_data;
	st_osal_time_t	 	current_time_1st, current_time_2nd;
	char            	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int test_ret = OSAL_TEST_OK;
	osal_nano_sec_t		diff_time;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADSLEEPFORTIMEPERIOD,
														   OSAL_TEST_ID002);
	memset(&current_time_1st, 0x00, sizeof(st_osal_time_t));
	memset(&current_time_2nd, 0x00, sizeof(st_osal_time_t));
	osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_1st);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
				OSAL_TEST_THREAD, OSAL_TEST_THREADSLEEPFORTIMEPERIOD, OSAL_TEST_ID002);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadSleepForTimePeriod(time);
	TIME_MEASURE_STOP

	osal_ret1 = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_2nd);
	if (osal_ret1 != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
				OSAL_TEST_THREAD, OSAL_TEST_THREADSLEEPFORTIMEPERIOD, OSAL_TEST_ID002);
		return;
	}

	osal_ret1 = R_OSAL_ClockTimeCalculateTimeDifference(&current_time_2nd, &current_time_1st, &diff_time);
	if (osal_ret1 != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeCalculateTimeDifference() error.",
				OSAL_TEST_THREAD, OSAL_TEST_THREADSLEEPFORTIMEPERIOD, OSAL_TEST_ID002);
		return;
	}

	if ((osal_ret == expect_data) && (diff_time >= 0))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_THREAD,
							OSAL_TEST_THREADSLEEPFORTIMEPERIOD,
							OSAL_TEST_ID002, 
							test_ret, opt_info);
}

/* ------------------------01_003_003 [success]------------------------ */
static void Test01_003_003(int time)
{
	e_osal_return_t   	osal_ret, osal_ret1;
	e_osal_return_t   	expect_data;
	st_osal_time_t	 	current_time_1st, current_time_2nd;
	char            	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADSLEEPFORTIMEPERIOD,
														   OSAL_TEST_ID003);
	memset(&current_time_1st, 0x00, sizeof(st_osal_time_t));
	memset(&current_time_2nd, 0x00, sizeof(st_osal_time_t));
	osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_1st);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
				OSAL_TEST_THREAD, OSAL_TEST_THREADSLEEPFORTIMEPERIOD, OSAL_TEST_ID003);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadSleepForTimePeriod(time);
	TIME_MEASURE_STOP

	osal_ret1 = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &current_time_2nd);
	if (osal_ret1 != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
				OSAL_TEST_THREAD, OSAL_TEST_THREADSLEEPUNTILTIMESTAMP, OSAL_TEST_ID003);
		return;
	}

	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_THREAD,
							OSAL_TEST_THREADSLEEPFORTIMEPERIOD,
							OSAL_TEST_ID003, 
							test_ret, opt_info);														   	
}

/* ------------------------01_004_001 [success]------------------------ */						
static void Test01_004_001(osal_thread_handle_t *handle)
{
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t       	osal_ret;
	osal_thread_handle_t 	handle1;
	st_osal_thread_config_t config;
	osal_thread_id_t     	thread_id;
	int64_t      			return_value = 0;
	int 					test_ret = OSAL_TEST_OK;


	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADSELF,
														   OSAL_TEST_ID001);

	memset(&config, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle1, 0x00, sizeof(osal_thread_handle_t));

	config.func       = osal_test_self_subthread;
    config.userarg    = (void *)handle;
	config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config.task_name  = "task_01_004_001";
	config.stack_size = 1024;
	thread_id         = 1;

	osal_ret = R_OSAL_ThreadCreate(&config, thread_id, &handle1);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADSELF, OSAL_TEST_ID001);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle1, &return_value);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADSELF, OSAL_TEST_ID001);
		return;
	}

	if (return_value == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)return_value));

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADSELF,
							OSAL_TEST_ID001, 
							test_ret, opt_info);
}

/* ------------------------01_004_002 [OSAL_RETURN_PAR]------------------------ */						
static void Test01_004_002(osal_thread_handle_t *handle)
{
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t       	osal_ret;
	int 					test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADSELF,
														   OSAL_TEST_ID002);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadSelf(handle);
	TIME_MEASURE_STOP

	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)osal_ret));

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADSELF,
							OSAL_TEST_ID002, 
							test_ret, opt_info);
}

/* ------------------------01_004_003 [OSAL_RETURN_FAIL]------------------------ */						
static void Test01_004_003(osal_thread_handle_t *handle)
{
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t       	osal_ret;
	int 					test_ret = OSAL_TEST_OK;


	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADSELF,
														   OSAL_TEST_ID003);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadSelf(handle);
	TIME_MEASURE_STOP

	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)osal_ret));

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADSELF,
							OSAL_TEST_ID003, 
							test_ret, opt_info);
}

/* ------------------------01_004_004 [OSAL_RETURN_STATE]------------------------ */						
static void Test01_004_004(osal_thread_handle_t *handle)
{
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	int 					test_ret = OSAL_TEST_OK;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADSELF,
														   OSAL_TEST_ID004);

	osal_ret = osal_deinit_thread();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADSELF, OSAL_TEST_ID004);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadSelf(handle);
	TIME_MEASURE_STOP

	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)osal_ret));

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADSELF,
							OSAL_TEST_ID004, 
							test_ret, opt_info);
}

#if 0
/* ------------------------01_004_005 [OSAL_RETURN_FAIL]------------------------ */						
static void Test01_004_005()
{
	e_osal_return_t       	osal_ret;
	osal_thread_handle_t 	handle;
	e_osal_return_t       	expect_data;
	int 					test_ret = OSAL_TEST_OK;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	memset(&handle, 0x00, sizeof(osal_thread_handle_t));

	
	/****************************/
	/* Pre-process				*/
	/****************************/

	osal_ret = osal_init_thread();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADSELF, OSAL_TEST_ID005);
		return;
	}


	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADSELF,
														   OSAL_TEST_ID005);

	g_stub_flag.R_OSAL_RCFG_GetThreadInfo = R_OSAL_ERR_NULL_CONTROL;

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadSelf(&handle);
	TIME_MEASURE_STOP


	/****************************/
	/* Post-process				*/
	/****************************/
	g_stub_flag.R_OSAL_RCFG_GetThreadInfo = 0; 


	
	/****************************/
	/* Check result				*/
	/****************************/
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)osal_ret));

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADSELF,
							OSAL_TEST_ID005, 
							test_ret, opt_info);

	osal_ret = osal_deinit_thread();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADSELF, OSAL_TEST_ID005);
		return;
	}
}
#endif
/* ------------------------01_005_001 [success]------------------------ */						
static void Test01_005_001()
{
	e_osal_return_t       		osal_ret;
	e_osal_return_t       		expect_data;
	osal_thread_id_t     		thread_id;
	osal_thread_handle_t 		handle1;
	st_osal_thread_config_t 	config1;
	int 						test_ret     = OSAL_TEST_OK;
	int64_t      				return_value = 0;
	char                		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int64_t 					arg = 1234;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADEQUAL,
														   OSAL_TEST_ID001);

	memset(&config1, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle1, 0x00, sizeof(osal_thread_handle_t));

	config1.func       = osal_test_equal_subthread;
	config1.userarg    = (void *)&arg;
    config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config1.task_name  = "task01_005_001";
	config1.stack_size = 1024;
	thread_id = 0;

	osal_ret  = R_OSAL_ThreadCreate(&config1, thread_id, &handle1);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, OSAL_TEST_ID001);
		return;
	}

	/* Set to a global variable for use R_OSAL_ThreadEqual() */
	g_thread_handle = handle1;

	osal_ret = R_OSAL_ThreadJoin(handle1, &return_value);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, OSAL_TEST_ID001);
		return;
	}
    
	if (return_value == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)return_value));

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADEQUAL,
							OSAL_TEST_ID001, 
							test_ret, opt_info);
}

/* ------------------------01_005_002 [success]------------------------ */						
static void Test01_005_002()
{
	e_osal_return_t       		osal_ret;
	osal_thread_id_t     		thread_id1, thread_id2;
	osal_thread_handle_t 		handle1, handle2;
	st_osal_thread_config_t 	config1, config2;
	int64_t 					arg = 1234;
	int64_t						return_value;

	memset(&config1, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle1, 0x00, sizeof(osal_thread_handle_t));
	memset(&config2, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle2, 0x00, sizeof(osal_thread_handle_t));

	config1.func       = osal_test_thread_sample;
	config1.userarg    = (void *)&arg;
    config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config1.task_name  = "task01_005_002_01";
	config1.stack_size = 1024;
	thread_id1		   = 1;

	
	config2.func       = osal_test_thread_equal;
	config2.userarg    = (void *)&arg;
    config2.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config2.task_name  = "task01_005_002_02";
	config2.stack_size = 1024;
	thread_id2 		   = 2;

	osal_ret  = R_OSAL_ThreadCreate(&config1, thread_id1, &handle1);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, OSAL_TEST_ID002);
		return;
	}
	
	g_equal_handle1 = handle1;

	osal_ret  = R_OSAL_ThreadCreate(&config2, thread_id2, &handle2);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, OSAL_TEST_ID002);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle2, &return_value);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, OSAL_TEST_ID002);
		return;
	}
	
	osal_ret = R_OSAL_ThreadJoin(handle1, &return_value);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, OSAL_TEST_ID002);
		return;
	}

	g_equal_handle1 = NULL;
}

/* ------------------------01_005_003 [OSAL_RETURN_HANDLE]------------------------ */						
static void Test01_005_003()
{
	e_osal_return_t       		osal_ret;
	e_osal_return_t       		expect_data;
	osal_thread_id_t     		thread_id;
	osal_thread_handle_t 		handle;
	st_osal_thread_config_t 	config;
	int 						test_ret     = OSAL_TEST_OK;
	char                		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int64_t 					arg = 1234;
	int64_t						return_value;
	bool						result = false;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADEQUAL,
														   OSAL_TEST_ID003);

	memset(&config, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle, 0x00, sizeof(osal_thread_handle_t));

	config.func       = osal_test_thread_sample;
	config.userarg    = (void *)&arg;
    config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config.task_name  = "task01_005_003";
	config.stack_size = 1024;
	thread_id = 3;

	osal_ret  = R_OSAL_ThreadCreate(&config, thread_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, OSAL_TEST_ID003);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle, &return_value);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, OSAL_TEST_ID003);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadEqual(NULL, handle, &result);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)osal_ret));

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADEQUAL,
							OSAL_TEST_ID003, 
							test_ret, opt_info);
}

/* ------------------------01_005_004 [OSAL_RETURN_HANDLE]------------------------ */						
static void Test01_005_004()
{
	e_osal_return_t       		osal_ret;
	e_osal_return_t       		expect_data;
	osal_thread_id_t     		thread_id;
	osal_thread_handle_t 		handle;
	st_osal_thread_config_t 	config;
	int 						test_ret     = OSAL_TEST_OK;
	char                		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int64_t 					arg = 1234;
	int64_t						return_value;
	bool						result = false;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADEQUAL,
														   OSAL_TEST_ID004);

	memset(&config, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle, 0x00, sizeof(osal_thread_handle_t));

	config.func       = osal_test_thread_sample;
	config.userarg    = (void *)&arg;
    config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config.task_name  = "task01_005_004";
	config.stack_size = 1024;
	thread_id = 4;

	osal_ret  = R_OSAL_ThreadCreate(&config, thread_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, OSAL_TEST_ID004);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle, &return_value);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, OSAL_TEST_ID004);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadEqual(handle, NULL, &result);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)osal_ret));

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADEQUAL,
							OSAL_TEST_ID004, 
							test_ret, opt_info);
}

/* ------------------------01_005_005 [OSAL_RETURN_PAR]------------------------ */						
static void Test01_005_005()
{
	e_osal_return_t       		osal_ret;
	e_osal_return_t       		expect_data;
	osal_thread_id_t     		thread_id1, thread_id2;
	osal_thread_handle_t 		handle1, handle2;
	st_osal_thread_config_t 	config1, config2;
	int 						test_ret = OSAL_TEST_OK;
	char                		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int64_t 					arg = 1234;
	int64_t						return_value;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADEQUAL,
														   OSAL_TEST_ID005);

	memset(&config1, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle1, 0x00, sizeof(osal_thread_handle_t));
	memset(&config2, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle2, 0x00, sizeof(osal_thread_handle_t));

	config1.func       = osal_test_thread_sample;
	config1.userarg    = (void *)&arg;
    config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config1.task_name  = "task01_005_005_01";
	config1.stack_size = 1024;
	thread_id1		   = 5;

	
	config2.func       = osal_test_thread_sample;
	config2.userarg    = (void *)&arg;
    config2.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config2.task_name  = "task01_005_005_02";
	config2.stack_size = 1024;
	thread_id2 		   = 6;

	osal_ret  = R_OSAL_ThreadCreate(&config1, thread_id1, &handle1);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, OSAL_TEST_ID005);
		return;
	}

	osal_ret  = R_OSAL_ThreadCreate(&config2, thread_id2, &handle2);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, OSAL_TEST_ID005);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadEqual(handle1, handle2, NULL);
	TIME_MEASURE_STOP

	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)osal_ret));

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADEQUAL,
							OSAL_TEST_ID005, 
							test_ret, opt_info);

	osal_ret = R_OSAL_ThreadJoin(handle1, &return_value);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, OSAL_TEST_ID005);
		return;
	}
	
	osal_ret = R_OSAL_ThreadJoin(handle2, &return_value);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, OSAL_TEST_ID005);
		return;
	}
}

/* ------------------------01_005_006 [OSAL_RETURN_STATE]------------------------ */						
static void Test01_005_006()
{
	e_osal_return_t       		osal_ret;
	e_osal_return_t       		expect_data;
	osal_thread_id_t     		thread_id1, thread_id2;
	osal_thread_handle_t 		handle1, handle2;
	st_osal_thread_config_t 	config1, config2;
	int 						test_ret = OSAL_TEST_OK;
	char                		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int64_t 					arg = 1234;
	bool						result = true;
	int64_t						return_value;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADEQUAL,
														   OSAL_TEST_ID006);

	memset(&config1, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle1, 0x00, sizeof(osal_thread_handle_t));
	memset(&config2, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle2, 0x00, sizeof(osal_thread_handle_t));

	config1.func       = osal_test_thread_sample;
	config1.userarg    = (void *)&arg;
    config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config1.task_name  = "task01_005_006_01";
	config1.stack_size = 1024;
	thread_id1		   = 7;

	
	config2.func       = osal_test_thread_sample;
	config2.userarg    = (void *)&arg;
    config2.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config2.task_name  = "task01_005_006_02";
	config2.stack_size = 1024;
	thread_id2 		   = 8;

	osal_ret  = R_OSAL_ThreadCreate(&config1, thread_id1, &handle1);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, OSAL_TEST_ID006);
		return;
	}

	osal_ret  = R_OSAL_ThreadCreate(&config2, thread_id2, &handle2);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, OSAL_TEST_ID006);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle1, &return_value);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, OSAL_TEST_ID006);
		return;
	}
	
	osal_ret = R_OSAL_ThreadJoin(handle2, &return_value);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, OSAL_TEST_ID006);
		return;
	}

	osal_ret = osal_deinit_thread();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, OSAL_TEST_ID006);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadEqual(handle1, handle2, &result);
	TIME_MEASURE_STOP

	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)osal_ret));

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADEQUAL,
							OSAL_TEST_ID006, 
							test_ret, opt_info);
}

#if 0
/* ------------------------01_005_007 [OSAL_RETURN_STATE]------------------------ */						
static void Test01_005_007()
{
	e_osal_return_t       		osal_ret;
	osal_thread_id_t     		thread_id;
	osal_thread_handle_t 		handle1;
	st_osal_thread_config_t 	config1;
	osal_thread_handle_t 		handle2;
	st_osal_thread_config_t 	config2;
	int							arg = 10;
	int64_t 					return_value = 0;
	osal_cond_handle_t 			cond_handle;
	osal_mutex_handle_t 		mutex_handle;

	osal_ret = osal_init_thread();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("osal_init_thread() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, OSAL_TEST_ID007);
		return;
	}


	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	osal_ret = R_OSAL_ThsyncMutexCreate(0, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL,
			OSAL_TEST_ID007);
		return;
	}

	osal_ret = R_OSAL_ThsyncCondCreate(OSAL_COND_RESERVE_00, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL,
			OSAL_TEST_ID007);
		return;
	}

	g_equal_mutex = mutex_handle;
	g_equal_cond = cond_handle;

	memset(&config1, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle1, 0x00, sizeof(osal_thread_handle_t));
	memset(&config2, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle2, 0x00, sizeof(osal_thread_handle_t));
	config1.func       = osal_test_thread_sample_interrupt_005_007;
	config1.userarg    = (void *)&arg;
	config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config1.task_name  = "task01_005_007";
	config1.stack_size = 1024;
	thread_id		   = 9;

	osal_ret  = R_OSAL_ThreadCreate(&config1, thread_id, &handle1);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, OSAL_TEST_ID007);
		return;
	}

	g_equal_handle1 =  handle1;
	config2.func       = osal_test_thread_sample;
	config2.userarg    = (void *)&arg;
	config2.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config2.task_name  = "task01_005_007";
	config2.stack_size = 1024;
	thread_id		   = 10;

	osal_ret  = R_OSAL_ThreadCreate(&config2, thread_id, &handle2);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, OSAL_TEST_ID007);
		return;
	}

	g_equal_handle2 =  handle2;
	osal_ret = R_OSAL_ThreadJoin(handle1, &return_value);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, OSAL_TEST_ID007);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle2, &return_value);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, OSAL_TEST_ID007);
		return;
	}
}


/* ------------------------01_005_008 [OSAL_RETURN_STATE]------------------------ */						
static void Test01_005_008()
{
	e_osal_return_t       		osal_ret;
	osal_thread_id_t     		thread_id;
	osal_thread_handle_t 		handle1;
	st_osal_thread_config_t 	config1;
	int							arg = 10;
	int64_t 					return_value = 0;
	osal_cond_handle_t 			cond_handle;
	osal_mutex_handle_t 		mutex_handle;

	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	osal_ret = R_OSAL_ThsyncMutexCreate(1, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL,
			OSAL_TEST_ID008);
		return;
	}

	osal_ret = R_OSAL_ThsyncCondCreate(OSAL_COND_RESERVE_01, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL,
			OSAL_TEST_ID008);
		return;
	}

	g_equal_mutex = mutex_handle;
	g_equal_cond = cond_handle;

	memset(&config1, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle1, 0x00, sizeof(osal_thread_handle_t));
	config1.func       = osal_test_thread_sample_interrupt_005_008;
	config1.userarg    = (void *)&arg;
	config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config1.task_name  = "task01_005_007";
	config1.stack_size = 1024;
	thread_id		   = 9;

	osal_ret  = R_OSAL_ThreadCreate(&config1, thread_id, &handle1);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, OSAL_TEST_ID007);
		return;
	}

	g_equal_handle1    =  handle1;

	osal_ret = R_OSAL_ThreadJoin(handle1, &return_value);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, OSAL_TEST_ID007);
		return;
	}

	osal_ret = osal_deinit_thread();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, OSAL_TEST_ID007);
		return;
	}
}
#endif

/* ------------------------01_006_001 [success]------------------------ */						
static void Test01_006_001()
{
	e_osal_return_t				osal_ret;
	e_osal_return_t       		expect_data;
	osal_thread_handle_t 		handle;
	st_osal_thread_config_t 	config;
	osal_thread_id_t     		thread_id;
	int64_t             		return_value;
	char                		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 						test_ret = OSAL_TEST_OK;

	memset(&config, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle, 0x00, sizeof(osal_thread_handle_t));

	config.func       = osal_test_thread_sample;
	config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config.task_name  = "task01_006_001";
	config.stack_size = 1024;
	thread_id         = 1;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADJOIN,
														   OSAL_TEST_ID001);

	osal_ret = R_OSAL_ThreadCreate(&config, thread_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADJOIN, OSAL_TEST_ID001);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadJoin(handle, &return_value);
	TIME_MEASURE_STOP
	if ((osal_ret == expect_data) && (return_value == 0))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADJOIN,
							OSAL_TEST_ID001, 
							test_ret, opt_info);
}

/* ------------------------01_006_002 [OSAL_RETURN_HANDLE]------------------------ */						
static void Test01_006_002()
{
	e_osal_return_t				osal_ret;
	e_osal_return_t       		expect_data;
	char                		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int64_t						return_value;
	int 						test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADJOIN,
														   OSAL_TEST_ID002);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadJoin(NULL, &return_value);
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

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADJOIN,
							OSAL_TEST_ID002, 
							test_ret, opt_info);
}

/* ------------------------01_006_003 [OSAL_RETURN_HANDLE]------------------------ */						
static void Test01_006_003()
{
	e_osal_return_t				osal_ret;
	osal_thread_handle_t 		handle;
	st_osal_thread_config_t 	config;
	osal_thread_id_t     		thread_id;
	e_osal_return_t       		expect_data;
	char                		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 						test_ret = OSAL_TEST_OK;
	int64_t						return_value;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADJOIN,
														   OSAL_TEST_ID003);

	memset(&config, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle, 0x00, sizeof(osal_thread_handle_t));

	config.func       = osal_test_thread_join;
	config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config.task_name  = "task01_006_003";
	config.stack_size = 1024;
	thread_id         = 4;

	osal_ret = R_OSAL_ThreadCreate(&config, thread_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADJOIN, OSAL_TEST_ID003);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadJoin(handle, &return_value);
	TIME_MEASURE_STOP
	if (return_value == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)return_value));

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADJOIN,
							OSAL_TEST_ID003, 
							test_ret, opt_info);
}

/* ------------------------01_006_004 [OSAL_RETURN_HANDLE]------------------------ */						
static void Test01_006_004()
{
	e_osal_return_t				osal_ret;
	e_osal_return_t       		expect_data;
	osal_thread_handle_t 		handle;
	st_osal_thread_config_t 	config;
	osal_thread_id_t     		thread_id;
	int64_t						return_value;
	char                		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 						test_ret = OSAL_TEST_OK;

	memset(&config, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle, 0x00, sizeof(osal_thread_handle_t));

	config.func       = osal_test_thread_sample;
	config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config.task_name  = "task01_006_004";
	config.stack_size = 1024;
	thread_id         = 5;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADJOIN,
														   OSAL_TEST_ID004);

	osal_ret = R_OSAL_ThreadCreate(&config, thread_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADJOIN, OSAL_TEST_ID004);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle, &return_value);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, OSAL_TEST_ID004);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_ThreadJoin(handle, &return_value);
		TIME_MEASURE_STOP
		if (osal_ret == expect_data)
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
			test_ret = OSAL_TEST_NG;
		}
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADJOIN,
							OSAL_TEST_ID004, 
							test_ret, opt_info);
}

/* ------------------------01_006_005 [OSAL_RETURN_STATE]------------------------ */						
static void Test01_006_005()
{
	e_osal_return_t				osal_ret;
	e_osal_return_t       		expect_data;
	osal_thread_handle_t 		handle1;
	osal_thread_handle_t 		handle2;
	st_osal_thread_config_t 	config1;
	st_osal_thread_config_t 	config2;
	osal_thread_id_t     		thread_id;
	int64_t						return_value;
	char                		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 						test_ret = OSAL_TEST_OK;
	osal_cond_handle_t  		cond_handle;
	osal_mutex_handle_t 		mutex_handle;
	st_arg_cond_waitperiod_t 	arg_cond;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADJOIN,
														   OSAL_TEST_ID005);

	memset(&config1, 0x00, sizeof(st_osal_thread_config_t));
	memset(&config2, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle1, 0x00, sizeof(osal_thread_handle_t));
	memset(&handle2, 0x00, sizeof(osal_thread_handle_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	memset(&cond_handle,  0x00, sizeof(osal_cond_handle_t));
	memset(&arg_cond, 0x00, sizeof(st_arg_cond_waitperiod_t));
	
	osal_ret = R_OSAL_ThsyncCondCreate(OSAL_COND_RESERVE_00, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADJOIN,
			OSAL_TEST_ID005);
		return;
	}

	osal_ret = R_OSAL_ThsyncMutexCreate(1, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADJOIN,
			OSAL_TEST_ID005);
		return;
	}
	
	arg_cond.mutex_handle = mutex_handle;
	arg_cond.handle = cond_handle;
	arg_cond.time = 10000;

	config1.func       	= osal_test_thread_join_1;
	config1.priority  	= OSAL_THREAD_PRIORITY_TYPE10;
	config1.userarg    = (void *)&arg_cond;
	config1.task_name  	= "task01_006_005_01";
	config1.stack_size 	= 1024;
	thread_id         	= 7;

	osal_ret = R_OSAL_ThreadCreate(&config1, thread_id, &handle1);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADJOIN, OSAL_TEST_ID005);
		return;
	}

	config2.func       	= osal_test_thread_join_2;
	config2.priority   	= OSAL_THREAD_PRIORITY_TYPE10;
	config2.userarg    = (void *)&arg_cond;
	config2.task_name  	= "task01_006_005_02";
	config2.stack_size 	= 1024;
	thread_id         	= 8;

	osal_ret = R_OSAL_ThreadCreate(&config2, thread_id, &handle2);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADJOIN, OSAL_TEST_ID005);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle2, &return_value);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADJOIN, OSAL_TEST_ID005);
		return;
	}
	
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadJoin(handle1, &return_value);
	TIME_MEASURE_STOP
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADJOIN, OSAL_TEST_ID005);
		return;
	}

	if (return_value == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)return_value));

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADJOIN,
							OSAL_TEST_ID005, 
							test_ret, opt_info);
}

/* ------------------------01_006_006 [OSAL_RETURN_HANDLE]------------------------ */
static void Test01_006_006() /* Add July 11, 2020 */
{
	e_osal_return_t				osal_ret;
	e_osal_return_t       		expect_data;
	osal_thread_handle_t 		handle;
	osal_thread_handle_t 		handle2;
	st_osal_thread_config_t 	config;
	osal_thread_id_t     		thread_id;
	int64_t						return_value;
	char                		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 						test_ret = OSAL_TEST_OK;

	memset(&config, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle, 0x00, sizeof(osal_thread_handle_t));



	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD,
														   OSAL_TEST_THREADJOIN,
														   OSAL_TEST_ID006);

	config.func       = osal_test_thread_sample;
	config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config.task_name  = "task01_006_006";
	config.stack_size = 1024;
	thread_id         = 5;

	osal_ret = R_OSAL_ThreadCreate(&config, thread_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADJOIN, OSAL_TEST_ID006);
		return;
	}

	config.func       = osal_test_thread_join_3;
	config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config.task_name  = "task01_006_006";
	config.stack_size = 1024;
	thread_id         = 6;
    g_join_handle = handle;

	osal_ret = R_OSAL_ThreadCreate(&config, thread_id, &handle2);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADJOIN, OSAL_TEST_ID006);
		return;
	}


	osal_ret = R_OSAL_ThreadJoin(handle, &return_value);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, OSAL_TEST_ID006);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_ThreadJoin(handle2, &return_value);
		TIME_MEASURE_STOP
		if (return_value == expect_data)
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
			test_ret = OSAL_TEST_NG;
		}
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)return_value));

	osal_test_result_output(OSAL_TEST_THREAD,
							OSAL_TEST_THREADJOIN,
							OSAL_TEST_ID006,
							test_ret, opt_info);
}

/* ------------------------01_006_007 [OSAL_RETURN_STATE]------------------------ */
static void Test01_006_007()
{
	e_osal_return_t				osal_ret;
	e_osal_return_t       		expect_data;
	osal_thread_handle_t 		handle;
	st_osal_thread_config_t 	config;
	osal_thread_id_t     		thread_id;
	int64_t						return_value;
	char                		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 						test_ret = OSAL_TEST_OK;

	memset(&config, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle, 0x00, sizeof(osal_thread_handle_t));

	config.func       = osal_test_thread_sample;
	config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config.task_name  = "task01_006_007";
	config.stack_size = 1024;
	thread_id         = 10;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADJOIN,
														   OSAL_TEST_ID007);

	osal_ret = R_OSAL_ThreadCreate(&config, thread_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADJOIN, OSAL_TEST_ID007);
		return;
	}

	R_OSAL_ThreadSleepForTimePeriod(2500);
	osal_ret = osal_deinit_thread();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADJOIN, OSAL_TEST_ID007);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadJoin(handle, &return_value);
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

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADJOIN,
							OSAL_TEST_ID007,
							test_ret, opt_info);
}

/* ------------------------01_007_001 [OSAL_RETURN_OK]------------------------ */
static void Test01_007_001()
{
	e_osal_return_t				osal_ret;
	e_osal_return_t       		expect_data;
	char                		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 						test_ret = OSAL_TEST_OK;
	st_osal_thread_config_t		config;
	
	
	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADINITIALIZETHREADCONFIGST,
														   OSAL_TEST_ID001);
	memset(&config, 0x00, sizeof(st_osal_thread_config_t));
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadInitializeThreadConfigSt(&config);
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

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADINITIALIZETHREADCONFIGST,
							OSAL_TEST_ID001,
							test_ret, opt_info);
}

/* ------------------------01_007_002 [OSAL_RETURN_PAR]------------------------ */
static void Test01_007_002()
{
	e_osal_return_t				osal_ret;
	e_osal_return_t       		expect_data;
	char                		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 						test_ret = OSAL_TEST_OK;	
	
	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_THREAD, 
														   OSAL_TEST_THREADINITIALIZETHREADCONFIGST,
														   OSAL_TEST_ID002);
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThreadInitializeThreadConfigSt(NULL);
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

	osal_test_result_output(OSAL_TEST_THREAD, 
							OSAL_TEST_THREADINITIALIZETHREADCONFIGST,
							OSAL_TEST_ID002,
							test_ret, opt_info);
}

/* 01_001: R_OSAL_ThreadCreate */
static void osal_test_R_OSAL_ThreadCreate()
{

	st_osal_thread_config_t config;
	osal_thread_handle_t 	handle;
    e_osal_return_t       	osal_ret;

/* ------------------------Initialize------------------------ */
	memset(&config, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle, 0x00, sizeof(osal_thread_handle_t));
	Test01_001_010(config,10,handle);

    osal_ret = osal_init_thread();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADCREATE, 0);
		return;
	}

	Test01_001_001(config,1,handle);
	Test01_001_002(config,2,handle);
	Test01_001_003(config,OSAL_THREAD_ICCOM_MGR_TASK,handle);
	Test01_001_004(config,OSAL_THREAD_IMP_DRV,handle);
	Test01_001_005(config,5,handle);
	Test01_001_006(config,6,handle);
//	Test01_001_007(config,9999,handle);
//	Test01_001_008(config,OSAL_THREAD_RESOURCE_NUM,handle);
	Test01_001_009(config,9,handle);
	Test01_001_011(config,11,handle);
	//Test01_001_012(config,12,handle);

	return;
}

/* 01_002: R_OSAL_ThreadSleepUntilTimeStamp */
static void osal_test_R_OSAL_ThreadSleepUntilTimeStamp()
{
	e_osal_return_t			osal_ret;

/* ------------------------Initialize------------------------ */
    osal_ret = osal_init_thread();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADSLEEPUNTILTIMESTAMP, 0);
		return;
	}

	Test01_002_001();
	Test01_002_002();
	Test01_002_003();
	Test01_002_004();
	Test01_002_005();
	Test01_002_006();
	Test01_002_007();

/* ------------------------Deinitialize------------------------ */
	osal_ret = osal_deinit_thread();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADSLEEPUNTILTIMESTAMP, 0);
		return;
	}

	return;
}

/* 01_003: R_OSAL_ThreadSleepForTimePeriod */
static void osal_test_R_OSAL_ThreadSleepForTimePeriod()
{
	e_osal_return_t   		osal_ret;

/* ------------------------Initialize------------------------ */
    osal_ret = osal_init_thread();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADSLEEPFORTIMEPERIOD, 0);
		return;
	}

	Test01_003_001(10);
	Test01_003_002(0);
	Test01_003_003(-10);

/* ------------------------Deinitialize------------------------ */
	osal_ret = osal_deinit_thread();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADSLEEPFORTIMEPERIOD, 0);
		return;
	}
	return;
}

/* 01_004: R_OSAL_ThreadSelf */
static void osal_test_R_OSAL_ThreadSelf()
{
	e_osal_return_t       	osal_ret;
	osal_thread_handle_t 	handle;

	memset(&handle, 0x00, sizeof(osal_thread_handle_t));

    osal_ret = osal_init_thread();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADSELF, 0);
		return;
	}

	Test01_004_001(&handle);
	Test01_004_002(0);
	Test01_004_003(&handle);
	Test01_004_004(&handle);
	//Test01_004_005();
}

/* 01_005: R_OSAL_ThreadEqual */
static void osal_test_R_OSAL_ThreadEqual()
{

	e_osal_return_t       	osal_ret;
	//UINT16 					Result;

    osal_ret = osal_init_thread();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, 0);
		return;
	}

	/*Result = drvTMU_Initialize();
	if (Result)
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, 0);
		return;
	}*/

	Test01_005_001();
	Test01_005_002();
	Test01_005_003();
	Test01_005_004();
	Test01_005_005();
	Test01_005_006();
	//Test01_005_007();
	//Test01_005_008();

	/*Result = drvTMU_Deinitialize();
	if (Result)
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADEQUAL, 0);
		return;
	}*/
}

/* 01_006: R_OSAL_ThreadJoin */
static void osal_test_R_OSAL_ThreadJoin()
{

	e_osal_return_t	osal_ret;

    osal_ret = osal_init_thread();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADJOIN, 0);
		return;
	}

	Test01_006_001();
	Test01_006_002();
	Test01_006_003();
	Test01_006_004();
	Test01_006_005();
	Test01_006_006(); /* Add July 11, 2020 */
	Test01_006_007();
}

static void osal_test_R_OSAL_ThreadInitializeThreadConfigSt()
{

	e_osal_return_t	osal_ret;

    osal_ret = osal_init_thread();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADINITIALIZETHREADCONFIGST, 0);
		return;
	}

	Test01_007_001();
	Test01_007_002();
	
	osal_ret = osal_deinit_thread();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THREADINITIALIZETHREADCONFIGST, 0);
		return;
	}
}
