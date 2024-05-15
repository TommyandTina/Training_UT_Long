/**
 * @file message_manager.c
 * @brief OSAL Message Manager evaluation file.
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
#include "message_manager.h"

/* extern */
// extern st_osal_mq_rcfg_config_t gs_osal_mq_config[OSAL_MQ_RESOURCE_NUM];

/* prototype */
static void osal_test_R_OSAL_MqCreate();
static void osal_test_R_OSAL_MqDelete();
static void osal_test_R_OSAL_MqSendUntilTimeStamp();
static void osal_test_R_OSAL_MqSendForTimePeriod();
static void osal_test_R_OSAL_MqReceiveUntilTimeStamp();
static void osal_test_R_OSAL_MqReceiveForTimePeriod();
static void osal_test_R_OSAL_MqIsFull();
static void osal_test_R_OSAL_MqIsEmpty();
static void osal_test_R_OSAL_MqGetConfig();
static void osal_test_R_OSAL_MqReset();
static void osal_test_R_OSAL_MqInitializeMqConfigSt();

volatile int g_msg_cb_flag = 0;
osal_mq_handle_t g_mq_handle;

void osal_test_message_manager_display_menu()
{

	printf("*** OSAL Message Manager ***\n");
	printf("***  03_001: R_OSAL_MqCreate                 ***\n");
	printf("***  03_002: R_OSAL_MqDeleate                ***\n");
	printf("***  03_003: R_OSAL_MqSendUntilTimeStamp     ***\n");
	printf("***  03_004: R_OSAL_MqSendForTimePeriod      ***\n");
	printf("***  03_005: R_OSAL_MqReceiveUntilTimeStamp  ***\n");
	printf("***  03_006: R_OSAL_MqReceiveForTimePeriod   ***\n");
	printf("***  03_007: R_OSAL_MqIsFull                 ***\n");
	printf("***  03_008: R_OSAL_MqIsEmpty                ***\n");
	printf("***  03_009: R_OSAL_MqGetConfig              ***\n");
	printf("***  03_010: R_OSAL_MqReset                  ***\n");
	printf("***  03_011: R_OSAL_MqInitializeMqConfigSt   ***\n");
	printf("***  03_100: message manager module all exec ***\n\n");
}

void osal_test_message_manager_menu(int child)
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
		test_number = OSAL_TEST_MQCREATE;
		test_count  = OSAL_TEST_MQ_MAX;
	}

	for (loop_count = 0; loop_count < test_count; loop_count++)
	{
		switch (test_number)
		{
			case OSAL_TEST_MQCREATE:
				osal_test_R_OSAL_MqCreate();
				break;
			case OSAL_TEST_MQDELETE:
				osal_test_R_OSAL_MqDelete();
				break;
			case OSAL_TEST_MQSENDUNTILTIMESTAMP:
				osal_test_R_OSAL_MqSendUntilTimeStamp();
				break;
			case OSAL_TEST_MQSENDFORTIMEPERIOD:
				osal_test_R_OSAL_MqSendForTimePeriod();
				break;
			case OSAL_TEST_MQRECEIVEUNTILTIMESTAMP:
				osal_test_R_OSAL_MqReceiveUntilTimeStamp();
				break;
			case OSAL_TEST_MQRECEIVEFORTIMEPERIOD:
				osal_test_R_OSAL_MqReceiveForTimePeriod();
				break;
			case OSAL_TEST_MQISFULL:
				osal_test_R_OSAL_MqIsFull();
				break;
			case OSAL_TEST_MQISEMPTY:
				osal_test_R_OSAL_MqIsEmpty();
				break;
			case OSAL_TEST_MQGETCONFIG:
				osal_test_R_OSAL_MqGetConfig();
				break;
			case OSAL_TEST_MQRESET:
				osal_test_R_OSAL_MqReset();
				break;
			case OSAL_TEST_MQINITIALIZEMQCONFIGST:
				osal_test_R_OSAL_MqInitializeMqConfigSt();
				break;
			default:
				printf("That menu is not available.\n\n");
				break;
		}

		test_number++;
	}
}

static void update_timestamp(st_osal_time_t *p_absolute_time, time_t sec_value, int32_t nsec_value)
{
    if(p_absolute_time->tv_nsec + nsec_value <= 999999999)
    {
        p_absolute_time->tv_sec += sec_value;
        p_absolute_time->tv_nsec += nsec_value;
    }
    else
    {
        p_absolute_time->tv_sec += (sec_value + 1);
        p_absolute_time->tv_nsec = nsec_value - (999999999 - p_absolute_time->tv_nsec);
    }
}

static void get_timestamp(st_osal_time_t *p_absolute_time, int parent_id, int child_id, int test_id)
{
	e_osal_return_t osal_ret;
	
	memset(p_absolute_time,  0x00, sizeof(st_osal_time_t));
	osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, p_absolute_time);
	if (osal_ret != OSAL_RETURN_OK)
	{
	    printf("osal_ret %d\n", osal_ret);
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.", parent_id, child_id, test_id);
	}
}

static void send_message(osal_mq_handle_t mq_handle, void* send_data, int parent_id, int child_id, int test_id)
{
	e_osal_return_t osal_ret;

	osal_ret = R_OSAL_MqSendForTimePeriod(mq_handle, 1, send_data, sizeof(uint32_t));
	if (osal_ret != OSAL_RETURN_OK)
	{
	    printf("osal_ret %d\n", osal_ret);
		OSAL_TEST_ERR("R_OSAL_MqSendForTimePeriod() error.", parent_id, child_id, test_id);
	}
}

static void receive_message(osal_mq_handle_t mq_handle, void* receive_data, size_t receive_size, int parent_id, int child_id, int test_id)
{
	e_osal_return_t osal_ret;

	osal_ret = R_OSAL_MqReceiveForTimePeriod(mq_handle, 10, receive_data, receive_size);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqReceiveForTimePeriod() error.", parent_id, child_id, test_id);
	}
}

static void send_msg_and_get_timestamp(osal_mq_handle_t mq_handle, void* send_data, st_osal_time_t *p_absolute_time, int parent_id, int child_id, int test_id)
{
	send_message(mq_handle, send_data, parent_id, child_id, test_id);
	get_timestamp(p_absolute_time, parent_id, child_id, test_id);
}

static e_osal_return_t exec_R_OSAL_MqCreate(const st_osal_mq_config_t *p_config, osal_mq_id_t mq_id, osal_mq_handle_t *const p_handle, int test_id, e_osal_return_t *expect_data, int *result)
{
	e_osal_return_t osal_ret;
	
	memset(p_handle, 0x00, sizeof(osal_mq_handle_t));

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret    = R_OSAL_MqCreate(p_config, mq_id, p_handle);
	TIME_MEASURE_STOP
	*expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MESSAGE, 
															OSAL_TEST_MQCREATE,
															test_id);
	if (osal_ret == *expect_data)
	{
		*result = OSAL_TEST_OK;
	}
	else
	{
		*result = OSAL_TEST_NG;
	}
	return osal_ret;
}

static e_osal_return_t exec_R_OSAL_MqCreate_HandleNull(const st_osal_mq_config_t *p_config, osal_mq_id_t mq_id, osal_mq_handle_t *const p_handle, int test_id, e_osal_return_t *expect_data, int *result)
{
	(void)p_handle;
	e_osal_return_t osal_ret;
	
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret    = R_OSAL_MqCreate(p_config, mq_id, NULL);
	TIME_MEASURE_STOP
	*expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MESSAGE, 
															OSAL_TEST_MQCREATE,
															test_id);
	if (osal_ret == *expect_data)
	{
		*result = OSAL_TEST_OK;
	}
	else
	{
		*result = OSAL_TEST_NG;
	}
	return osal_ret;
}


static e_osal_return_t exec_R_OSAL_MqDelete(osal_mq_handle_t handle, int test_id, e_osal_return_t *expect_data, int *result)
{
	e_osal_return_t osal_ret;
	
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret     = R_OSAL_MqDelete(handle);
	TIME_MEASURE_STOP
	*expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MESSAGE, 
															OSAL_TEST_MQDELETE,
															test_id);
	if (osal_ret == *expect_data)
	{
		*result = OSAL_TEST_OK;
	}
	else
	{
		*result = OSAL_TEST_NG;
	}
	return osal_ret;
}

static e_osal_return_t exec_R_OSAL_MqSendUntilTimeStamp(osal_mq_handle_t handle,
															const st_osal_time_t *const p_time_stamp,
															const void * p_buffer,
															size_t buffer_size,
															int test_id,
															e_osal_return_t *expect_data,
															int *result)
{
	e_osal_return_t osal_ret;
	
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret     = R_OSAL_MqSendUntilTimeStamp(handle, p_time_stamp, p_buffer, buffer_size);
	TIME_MEASURE_STOP
	*expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MESSAGE, 
															OSAL_TEST_MQSENDUNTILTIMESTAMP,
															test_id);
	if (osal_ret == *expect_data)
	{
		*result = OSAL_TEST_OK;
	}
	else
	{
		*result = OSAL_TEST_NG;
	}
	return osal_ret;
}

static e_osal_return_t exec_R_OSAL_MqSendForTimePeriod(osal_mq_handle_t handle,
															osal_milli_sec_t time_period,
															const void * p_buffer,
															size_t buffer_size,
															int test_id,
															e_osal_return_t *expect_data,
															int *result)
{
	e_osal_return_t osal_ret;
	
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret     = R_OSAL_MqSendForTimePeriod(handle, time_period, p_buffer, buffer_size);
	TIME_MEASURE_STOP
	*expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MESSAGE, 
															OSAL_TEST_MQSENDFORTIMEPERIOD,
															test_id);
	if (osal_ret == *expect_data)
	{
		*result = OSAL_TEST_OK;
	}
	else
	{
		*result = OSAL_TEST_NG;
	}
	return osal_ret;
}

static e_osal_return_t exec_R_OSAL_MqReceiveUntilTimeStamp(osal_mq_handle_t handle,
															const st_osal_time_t *const p_time_stamp,
															void * p_buffer,
															size_t buffer_size,
															int test_id,
															e_osal_return_t *expect_data,
															int *result)
{
	e_osal_return_t osal_ret;
	e_osal_return_t osal_ret_2;
	uint32_t recv_data[1];
	
	*result = OSAL_TEST_NG;
	
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_MqReceiveUntilTimeStamp(handle, p_time_stamp, p_buffer, buffer_size);
	TIME_MEASURE_STOP
	*expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MESSAGE, 
															OSAL_TEST_MQRECEIVEUNTILTIMESTAMP,
															test_id);
	if (osal_ret == *expect_data)
	{
		if (*expect_data == OSAL_RETURN_OK)
		{
			if ((*(uint32_t *)p_buffer) == (uint32_t)0xFFFF)
			{
				*result = OSAL_TEST_OK;
			}		
		}
		else
		{
			if ((handle != NULL) && (*expect_data != OSAL_RETURN_STATE))
			{
				osal_ret_2 = R_OSAL_MqReceiveForTimePeriod(handle, 10, recv_data, sizeof(recv_data));
				if (osal_ret_2 != OSAL_RETURN_OK)
				{
					OSAL_TEST_ERR("R_OSAL_MqReceiveForTimePeriod() error.", OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEUNTILTIMESTAMP, test_id);
				}
				*result = OSAL_TEST_OK;
			}
			*result = OSAL_TEST_OK;
		}
	}
	return osal_ret;
}

static e_osal_return_t exec_R_OSAL_MqReceiveForTimePeriod(osal_mq_handle_t handle,
															osal_milli_sec_t time_period,
															void * p_buffer,
															size_t buffer_size,
															int test_id,
															e_osal_return_t *expect_data,
															int *result)
{
	e_osal_return_t osal_ret;
	e_osal_return_t osal_ret_2;
	uint32_t recv_data[1];
	
	*result = OSAL_TEST_NG;
	
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret     = R_OSAL_MqReceiveForTimePeriod(handle, time_period, p_buffer, buffer_size);
	TIME_MEASURE_STOP
	*expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MESSAGE, 
															OSAL_TEST_MQRECEIVEFORTIMEPERIOD,
															test_id);
	if (osal_ret == *expect_data)
	{
		if (*expect_data == OSAL_RETURN_OK)
		{
			if ((*(uint32_t *)p_buffer) == (uint32_t)0xFFFF)
			{
				*result = OSAL_TEST_OK;
			}		
		}
		else
		{
			if ((handle != NULL) && (*expect_data != OSAL_RETURN_STATE))
			{
				osal_ret_2 = R_OSAL_MqReceiveForTimePeriod(handle, 10, recv_data, sizeof(recv_data));
				if (osal_ret_2 != OSAL_RETURN_OK)
				{
					OSAL_TEST_ERR("R_OSAL_MqReceiveForTimePeriod() error.", OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEFORTIMEPERIOD, test_id);
				}
				*result = OSAL_TEST_OK;
			}
			*result = OSAL_TEST_OK;
		}
	}
	return osal_ret;
}

/* ------------------------03_001_001 [success]------------------------ */
static void Test03_001_001(st_osal_mq_config_t mq_config, osal_mq_id_t mq_id, osal_mq_handle_t mq_handle)
{
	char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
	e_osal_return_t expect_data = OSAL_RETURN_OK;
	int  test_ret    = OSAL_TEST_OK;

	/* Start test function */
	osal_ret = exec_R_OSAL_MqCreate(&mq_config, mq_id, &mq_handle, OSAL_TEST_ID001, &expect_data, &test_ret);
	/* End test function */

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_MESSAGE, 
							OSAL_TEST_MQCREATE,
							OSAL_TEST_ID001, 
							test_ret, opt_info);

	osal_ret = R_OSAL_MqDelete(mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqDelete() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, OSAL_TEST_ID001);
		return;
	}
}

/* ------------------------03_001_002 [success]------------------------ */
static void Test03_001_002(st_osal_mq_config_t mq_config, osal_mq_id_t mq_id, osal_mq_handle_t mq_handle)
{
	char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
	e_osal_return_t expect_data = OSAL_RETURN_OK;
	int  test_ret    = OSAL_TEST_OK;

	/* Start test function */
	osal_ret = exec_R_OSAL_MqCreate(&mq_config, mq_id, &mq_handle, OSAL_TEST_ID002, &expect_data, &test_ret);
	/* End test function */

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_MESSAGE, 
							OSAL_TEST_MQCREATE,
							OSAL_TEST_ID002, 
							test_ret, opt_info);

	osal_ret = R_OSAL_MqDelete(mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqDelete() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, OSAL_TEST_ID002);
		return;
	}
}

/* ------------------------03_001_003 [success]------------------------ */
static void Test03_001_003(st_osal_mq_config_t mq_config, osal_mq_id_t mq_id, osal_mq_handle_t mq_handle)
{
	char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
	e_osal_return_t expect_data = OSAL_RETURN_OK;
	int  test_ret    = OSAL_TEST_OK;

	/* Start test function */
	osal_ret = exec_R_OSAL_MqCreate(&mq_config, mq_id, &mq_handle, OSAL_TEST_ID003, &expect_data, &test_ret);
	/* End test function */

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_MESSAGE, 
							OSAL_TEST_MQCREATE,
							OSAL_TEST_ID003, 
							test_ret, opt_info);

	if(osal_ret == OSAL_RETURN_OK)
	{
		osal_ret = R_OSAL_MqDelete(mq_handle);
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_MqDelete() error.",
					OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, OSAL_TEST_ID003);
			return;
		}
	}
}

/* ------------------------03_001_004 [OSAL_RETURN_PAR]------------------------ */
static void Test03_001_004(st_osal_mq_config_t mq_config, osal_mq_id_t mq_id, osal_mq_handle_t mq_handle)
{
	char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
	e_osal_return_t expect_data = OSAL_RETURN_OK;
	int  test_ret    = OSAL_TEST_OK;

	/* Start test function */
	osal_ret = exec_R_OSAL_MqCreate_HandleNull(&mq_config, mq_id, &mq_handle, OSAL_TEST_ID004, &expect_data, &test_ret);
	/* End test function */

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_MESSAGE, 
							OSAL_TEST_MQCREATE,
							OSAL_TEST_ID004, 
							test_ret, opt_info);
}

/* ------------------------03_001_005 [OSAL_RETURN_ID]------------------------ */
static void Test03_001_005(st_osal_mq_config_t mq_config, osal_mq_id_t mq_id, osal_mq_handle_t mq_handle)
{
	char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
	e_osal_return_t expect_data = OSAL_RETURN_OK;
	int  test_ret    = OSAL_TEST_OK;

	/* Start test function */
	osal_ret = exec_R_OSAL_MqCreate(&mq_config, mq_id, &mq_handle, OSAL_TEST_ID005, &expect_data, &test_ret);
	/* End test function */

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_MESSAGE, 
							OSAL_TEST_MQCREATE,
							OSAL_TEST_ID005, 
							test_ret, opt_info);
}

/* ------------------------03_001_006 [OSAL_RETURN_ID]------------------------ */
static void Test03_001_006(st_osal_mq_config_t mq_config, osal_mq_id_t mq_id, osal_mq_handle_t mq_handle)
{
	char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
	e_osal_return_t expect_data = OSAL_RETURN_OK;
	int  test_ret    = OSAL_TEST_OK;

	/* Start test function */
	osal_ret = exec_R_OSAL_MqCreate(&mq_config, mq_id, &mq_handle, OSAL_TEST_ID006, &expect_data, &test_ret);
	/* End test function */

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_MESSAGE, 
							OSAL_TEST_MQCREATE,
							OSAL_TEST_ID006, 
							test_ret, opt_info);
}

/* ------------------------03_001_007 [OSAL_RETURN_BUSY]------------------------ */
static void Test03_001_007(st_osal_mq_config_t mq_config, osal_mq_id_t mq_id, osal_mq_handle_t mq_handle)
{
	osal_mq_handle_t mq_handle2;
	char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
	e_osal_return_t expect_data = OSAL_RETURN_OK;
	int  test_ret    = OSAL_TEST_OK;

	memset(&mq_handle, 0x00, sizeof(osal_mq_handle_t));
	
	osal_ret = R_OSAL_MqCreate(&mq_config, mq_id, &mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqCreate() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, OSAL_TEST_ID007);
		return;
	}

	/* Start test function */
	osal_ret = exec_R_OSAL_MqCreate(&mq_config, mq_id, &mq_handle2, OSAL_TEST_ID007, &expect_data, &test_ret);
	/* End test function */

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_MESSAGE,
							OSAL_TEST_MQCREATE,
							OSAL_TEST_ID007,
							test_ret, opt_info);

	osal_ret = R_OSAL_MqDelete(mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqDelete() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, OSAL_TEST_ID007);
		return;
	}
}

/* ------------------------03_001_008 [OSAL_RETURN_STATE]------------------------ */
static void Test03_001_008(st_osal_mq_config_t mq_config, osal_mq_id_t mq_id, osal_mq_handle_t mq_handle)
{
	char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
	e_osal_return_t expect_data = OSAL_RETURN_OK;
	int  test_ret    = OSAL_TEST_OK;

	/* Start test function */
	osal_ret = exec_R_OSAL_MqCreate(&mq_config, mq_id, &mq_handle, OSAL_TEST_ID008, &expect_data, &test_ret);
	/* End test function */

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_MESSAGE, 
							OSAL_TEST_MQCREATE,
							OSAL_TEST_ID008, 
							test_ret, opt_info);
}

/* ------------------------03_001_009 [OSAL_RETURN_STATE]------------------------ */
static void Test03_001_009(st_osal_mq_config_t mq_config, osal_mq_id_t mq_id, osal_mq_handle_t mq_handle)
{
	char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
	e_osal_return_t expect_data = OSAL_RETURN_OK;
	int  test_ret    = OSAL_TEST_OK;

	/* Start test function */
	osal_ret = exec_R_OSAL_MqCreate(&mq_config, mq_id, &mq_handle, OSAL_TEST_ID009, &expect_data, &test_ret);
	/* End test function */

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_MESSAGE, 
							OSAL_TEST_MQCREATE,
							OSAL_TEST_ID009, 
							test_ret, opt_info);
}

/* ------------------------03_001_010 [OSAL_RETURN_FAIL]------------------------ */

//static void Test03_001_010(st_osal_mq_config_t mq_config, osal_mq_id_t mq_id, osal_mq_handle_t mq_handle)
//{
//	char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];
//	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
//	e_osal_return_t expect_data = OSAL_RETURN_OK;
//	int  test_ret    = OSAL_TEST_OK;
//
//	/* Set stub flag */
//	g_stub_flag.SemaphoreGive = SEMAPHOREGIVE_pdFAIL;
//
//	/* Start test function */
//	osal_ret = exec_R_OSAL_MqCreate(&mq_config, mq_id, &mq_handle, OSAL_TEST_ID010, &expect_data, &test_ret);
//	/* End test function */
//
//	/* Clear stub flag */
//	g_stub_flag.SemaphoreGive = 0;
//
//	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
//
//	osal_test_result_output(OSAL_TEST_MESSAGE,
//							OSAL_TEST_MQCREATE,
//							OSAL_TEST_ID010,
//							test_ret, opt_info);
//}
/* ------------------------03_002_001 [success]------------------------ */
static void Test03_002_001(st_osal_mq_config_t mq_config, osal_mq_id_t mq_id, osal_mq_handle_t mq_handle)
{
	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
	e_osal_return_t expect_data = OSAL_RETURN_OK;
	int  test_ret    = OSAL_TEST_OK;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	memset(&mq_handle, 0x00, sizeof(osal_mq_handle_t));

	osal_ret = R_OSAL_MqCreate(&mq_config, mq_id, &mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqCreate() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQDELETE, OSAL_TEST_ID001);
		return;
	}

	/* Start test function */
	osal_ret = exec_R_OSAL_MqDelete(mq_handle, OSAL_TEST_ID001, &expect_data, &test_ret);
	/* End test function */

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_MESSAGE, 
							OSAL_TEST_MQDELETE,
							OSAL_TEST_ID001, 
							test_ret, opt_info);
}

/* ------------------------03_002_002 [OSAL_RETURN_HANDLE]------------------------ */
static void Test03_002_002(st_osal_mq_config_t mq_config, osal_mq_id_t mq_id, osal_mq_handle_t mq_handle)
{
	(void)mq_config;
	(void)mq_id;

	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
	e_osal_return_t expect_data = OSAL_RETURN_OK;
	int  test_ret    = OSAL_TEST_OK;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	/* Start test function */
	osal_ret = exec_R_OSAL_MqDelete(mq_handle, OSAL_TEST_ID002, &expect_data, &test_ret);
	/* End test function */

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_MESSAGE, 
							OSAL_TEST_MQDELETE, 
							OSAL_TEST_ID002, 
							test_ret, opt_info);
}

/* ------------------------03_002_003 [OSAL_RETURN_HANDLE]------------------------ */
static void Test03_002_003(st_osal_mq_config_t mq_config, osal_mq_id_t mq_id, osal_mq_handle_t mq_handle)
{
	(void)mq_config;
	(void)mq_id;
	
	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
	e_osal_return_t expect_data = OSAL_RETURN_OK;
	int  test_ret    = OSAL_TEST_OK;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	memset(&mq_handle, 0x00, sizeof(osal_mq_handle_t));

	/* Start test function */
	osal_ret = exec_R_OSAL_MqDelete(mq_handle, OSAL_TEST_ID003, &expect_data, &test_ret);
	/* End test function */

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_MESSAGE,
							OSAL_TEST_MQDELETE,
							OSAL_TEST_ID003,
							test_ret, opt_info);
}

/* ------------------------03_002_004 [OSAL_RETURN_FAIL]------------------------ */
static void Test03_002_004(st_osal_mq_config_t mq_config, osal_mq_id_t mq_id, osal_mq_handle_t mq_handle)
{
	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
	e_osal_return_t expect_data = OSAL_RETURN_OK;
	int  test_ret    = OSAL_TEST_OK;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int msg = 0xFFFF;

	memset(&mq_handle, 0x00, sizeof(osal_mq_handle_t));

	osal_ret = R_OSAL_MqCreate(&mq_config, mq_id, &mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqCreate() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQDELETE, OSAL_TEST_ID004);
		return;
	}

	osal_ret = R_OSAL_MqSendForTimePeriod(mq_handle, 20, (void*)&msg, sizeof(msg));
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqSendForTimePeriod() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQDELETE, OSAL_TEST_ID004);
		return;
	}

	/* Start test function */
	osal_ret = exec_R_OSAL_MqDelete(mq_handle, OSAL_TEST_ID004, &expect_data, &test_ret);
	/* End test function */
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_MESSAGE,
							OSAL_TEST_MQDELETE,
							OSAL_TEST_ID004,
							test_ret, opt_info);

	osal_ret = R_OSAL_MqReceiveForTimePeriod(mq_handle, 20, (void*)&msg, sizeof(msg));
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqReceiveForTimePeriod() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQDELETE, OSAL_TEST_ID004);
		return;
	}
	osal_ret = R_OSAL_MqDelete(mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqDelete() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQDELETE, OSAL_TEST_ID004);
		return;
	}

}

/* ------------------------03_002_005 [OSAL_RETURN_HANDLE]------------------------ */
static void Test03_002_005(st_osal_mq_config_t mq_config, osal_mq_id_t mq_id, osal_mq_handle_t mq_handle)
{
	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
	e_osal_return_t expect_data = OSAL_RETURN_OK;
	int  test_ret    = OSAL_TEST_OK;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	memset(&mq_handle, 0x00, sizeof(osal_mq_handle_t));

	osal_ret = R_OSAL_MqCreate(&mq_config, mq_id, &mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqCreate() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQDELETE, OSAL_TEST_ID005);
		return;
	}

	osal_ret = R_OSAL_MqDelete(mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqDelete() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQDELETE, OSAL_TEST_ID005);
		return;
	}

	/* Start test function */
	osal_ret = exec_R_OSAL_MqDelete(mq_handle, OSAL_TEST_ID005, &expect_data, &test_ret);
	/* End test function */

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_MESSAGE,
							OSAL_TEST_MQDELETE,
							OSAL_TEST_ID005,
							test_ret, opt_info);
}

/* ------------------------03_002_006 [OSAL_RETURN_BUSY]------------------------ */
// static void Test03_002_006()
// {
// 	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
// 	e_osal_return_t expect_data = OSAL_RETURN_OK;
// 	st_osal_mq_config_t mq_config;
// 	osal_mq_handle_t    mq_handle;
// 	osal_mq_id_t mq_id	= 1;
// 	int  test_ret    = OSAL_TEST_OK;
// 	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

// 	memset(&mq_handle, 0x00, sizeof(osal_mq_handle_t));
// 	memset(&mq_config, 0x00, sizeof(st_osal_mq_config_t));

// 	/* Message create */
// 	osal_ret = R_OSAL_MqCreate(&mq_config, mq_id, &mq_handle);
// 	if (osal_ret != OSAL_RETURN_OK)
// 	{
// 		OSAL_TEST_ERR("R_OSAL_MqCreate() error.",
// 				OSAL_TEST_MESSAGE, OSAL_TEST_MQDELETE, OSAL_TEST_ID006);
// 		return;
// 	}

// 	/* Set stub */
// 	g_stub_flag.QueueAvailable = QUEUEAVAILABLE_pdFAIL;

// 	/****************************/
// 	/* Test execution			*/
// 	/****************************/
// 	osal_ret = exec_R_OSAL_MqDelete(mq_handle, OSAL_TEST_ID006, &expect_data, &test_ret);

// 	/****************************/
// 	/* Post-process				*/
// 	/****************************/
// 	/* Clear stub */
// 	g_stub_flag.QueueAvailable = 0;

// 	/****************************/
// 	/* Check result				*/
// 	/****************************/
// 	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

// 	osal_test_result_output(OSAL_TEST_MESSAGE,
// 							OSAL_TEST_MQDELETE,
// 							OSAL_TEST_ID006,
// 							test_ret, opt_info);
// }


/* ------------------------03_002_007 [OSAL_RETURN_STATE]------------------------ */
static void Test03_002_007(st_osal_mq_config_t mq_config, osal_mq_id_t mq_id, osal_mq_handle_t mq_handle)
{
	(void)mq_config;
	(void)mq_id;
	
	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
	e_osal_return_t expect_data = OSAL_RETURN_OK;
	int  test_ret    = OSAL_TEST_OK;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	memset(&mq_handle, 0x00, sizeof(osal_mq_handle_t));


	/****************************/
	/* Pre-process				*/
	/****************************/

	/* Message create */
	osal_ret = R_OSAL_MqCreate(&mq_config, mq_id, &mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqCreate() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQDELETE, OSAL_TEST_ID007);
		return;
	}
	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQDELETE, 0);
		return;
	}

	/****************************/
	/* Test execution			*/
	/****************************/
	osal_ret = exec_R_OSAL_MqDelete(mq_handle, OSAL_TEST_ID007, &expect_data, &test_ret);

	
	/****************************/
	/* Check result				*/
	/****************************/
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_MESSAGE,
							OSAL_TEST_MQDELETE,
							OSAL_TEST_ID007,
							test_ret, opt_info);
}

/* ------------------------03_002_008 [OSAL_RETURN_FAIL]------------------------ */
//static void Test03_002_008()
// {
// 	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
// 	e_osal_return_t expect_data = OSAL_RETURN_OK;
// 	st_osal_mq_config_t mq_config;
// 	osal_mq_handle_t    mq_handle;
// 	osal_mq_id_t mq_id	= 0;
// 	int  test_ret    = OSAL_TEST_OK;
// 	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

// 	memset(&mq_handle, 0x00, sizeof(osal_mq_handle_t));

// 	/****************************/
// 	/* Pre-process				*/
// 	/****************************/
// 	/* Init device */
// 	osal_ret = osal_init();
// 	if (osal_ret != OSAL_RETURN_OK)
// 	{
// 		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
// 				OSAL_TEST_MESSAGE, OSAL_TEST_MQDELETE, 0);
// 		return;
// 	}

// 	/* Message create */
// 	osal_ret = R_OSAL_MqCreate(&mq_config, mq_id, &mq_handle);
// 	if (osal_ret != OSAL_RETURN_OK)
// 	{
// 		OSAL_TEST_ERR("R_OSAL_MqCreate() error.",
// 				OSAL_TEST_MESSAGE, OSAL_TEST_MQDELETE, OSAL_TEST_ID008);
// 		return;
// 	}

// 	/* Set stub flag */
// 	g_stub_flag.SemaphoreGive = SEMAPHOREGIVE_pdFAIL;

// 	/****************************/
// 	/* Test execution			*/
// 	/****************************/
// 	osal_ret = exec_R_OSAL_MqDelete(mq_handle, OSAL_TEST_ID008, &expect_data, &test_ret);

// 	/****************************/
// 	/* Post-process				*/
// 	/****************************/
// 	/* Clear stub flag */
// 	g_stub_flag.SemaphoreGive = 0;


// 	/****************************/
// 	/* Check result				*/
// 	/****************************/
// 	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

// 	osal_test_result_output(OSAL_TEST_MESSAGE, 
// 							OSAL_TEST_MQDELETE,
// 							OSAL_TEST_ID008, 
// 							test_ret, opt_info);
							
// 	osal_ret = R_OSAL_Deinitialize();
// 	if (osal_ret != OSAL_RETURN_OK)
// 	{
// 		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
// 				OSAL_TEST_MESSAGE, OSAL_TEST_MQDELETE, 0);
// 		return;
// 	}
// }

static void Test03_003_common(osal_mq_handle_t mq_handle, st_osal_time_t *p_time_stamp, const void *send_data, size_t size, int test_id)
{
	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
	e_osal_return_t expect_data = OSAL_RETURN_OK;
	int             test_ret    = OSAL_TEST_OK;
	char                opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	/* Start test function */
	osal_ret = exec_R_OSAL_MqSendUntilTimeStamp(mq_handle, p_time_stamp, send_data, size, test_id, &expect_data, &test_ret);
	/* End test function */

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_MESSAGE, 
							OSAL_TEST_MQSENDUNTILTIMESTAMP,
							test_id, 
							test_ret, opt_info);
}

/* ------------------------03_003_001 [success]------------------------ */
static void Test03_003_001(osal_mq_handle_t mq_handle, st_osal_time_t *p_time_stamp, const void *send_data, size_t size)
{
	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
	char                rcv_data[128];

	/* Start test function */
	Test03_003_common(mq_handle, p_time_stamp, send_data, size, OSAL_TEST_ID001);
	/* End test function */

	osal_ret = R_OSAL_MqReceiveForTimePeriod(mq_handle, 10, rcv_data, size);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqReceiveForTimePeriod() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP,
				OSAL_TEST_ID001);
		return;
	}
}

/* ------------------------03_003_002 [success]------------------------ */
static void Test03_003_002(osal_mq_handle_t mq_handle, st_osal_time_t *p_time_stamp, const void *send_data, size_t size)
{
	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
	char                rcv_data[128];

	/* Start test function */
	Test03_003_common(mq_handle, p_time_stamp, send_data, size, OSAL_TEST_ID002);
	/* End test function */

	osal_ret = R_OSAL_MqReceiveForTimePeriod(mq_handle, 10, rcv_data, size);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqReceiveForTimePeriod() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP,
				OSAL_TEST_ID002);
		return;
	}
}

/* ------------------------03_003_003 [success]------------------------ */
static void Test03_003_003(osal_mq_handle_t mq_handle, st_osal_time_t *p_time_stamp, const void *send_data, size_t size)
{
	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
	char                rcv_data[128];

	/* Start test function */
	Test03_003_common(mq_handle, p_time_stamp, send_data, size, OSAL_TEST_ID003);
	/* End test function */

	osal_ret = R_OSAL_MqReceiveForTimePeriod(mq_handle, 10, rcv_data, size);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqReceiveForTimePeriod() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP,
				OSAL_TEST_ID003);
		return;
	}
}

/* ------------------------03_003_004 [success]------------------------ */
static void Test03_003_004(osal_mq_handle_t mq_handle, st_osal_time_t *p_time_stamp, const void *send_data, size_t size)
{
	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
	char                rcv_data[4];

	/* Start test function */
	Test03_003_common(mq_handle, p_time_stamp, send_data, size, OSAL_TEST_ID004);
	/* End test function */

	osal_ret = R_OSAL_MqReceiveForTimePeriod(mq_handle, 10, rcv_data, size);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqReceiveForTimePeriod() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP,
				OSAL_TEST_ID004);
		return;
	}
}

/* ------------------------03_003_005 [OSAL_RETURN_HANDLE]------------------------ */
static void Test03_003_005(osal_mq_handle_t mq_handle, st_osal_time_t *p_time_stamp, const void *send_data, size_t size)
{
	Test03_003_common(mq_handle, p_time_stamp, send_data, size, OSAL_TEST_ID005);
}

/* ------------------------03_003_006 [OSAL_RETURN_PAR]------------------------ */
static void Test03_003_006(osal_mq_handle_t mq_handle, st_osal_time_t *p_time_stamp, const void *send_data, size_t size)
{
	Test03_003_common(mq_handle, p_time_stamp, send_data, size, OSAL_TEST_ID006);
}

/* ------------------------03_003_007 [OSAL_RETURN_PAR]------------------------ */
static void Test03_003_007(osal_mq_handle_t mq_handle, st_osal_time_t *p_time_stamp, const void *send_data, size_t size)
{
	Test03_003_common(mq_handle, p_time_stamp, send_data, size, OSAL_TEST_ID007);
}

/* ------------------------03_003_008 [success]------------------------ */
static void Test03_003_008(osal_mq_handle_t mq_handle, st_osal_time_t *p_time_stamp, const void *send_data, size_t size)
{
	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
	char                rcv_data[4];

	Test03_003_common(mq_handle, p_time_stamp, send_data, size, OSAL_TEST_ID008);

	osal_ret = R_OSAL_MqReceiveForTimePeriod(mq_handle, 10, rcv_data, sizeof(rcv_data));
	//if (osal_ret != OSAL_RETURN_OK)
	//{
		//OSAL_TEST_ERR("R_OSAL_MqReceiveForTimePeriod() error.",
				//OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP,
				//OSAL_TEST_ID008);
		//return;
	//}
}

/* ------------------------03_003_009 [OSAL_RETURN_PAR]------------------------ */
static void Test03_003_009(osal_mq_handle_t mq_handle, st_osal_time_t *p_time_stamp, const void *send_data, size_t size)
{
	Test03_003_common(mq_handle, p_time_stamp, send_data, size, OSAL_TEST_ID009);
}

/* ------------------------03_003_010 [OSAL_RETURN_PAR]------------------------ */
static void Test03_003_010(osal_mq_handle_t mq_handle, st_osal_time_t *p_time_stamp, const void *send_data, size_t size)
{
	Test03_003_common(mq_handle, p_time_stamp, send_data, size, OSAL_TEST_ID010);
}

/* ------------------------03_003_011 [OSAL_RETURN_TIME]------------------------ */
static void Test03_003_011(osal_mq_handle_t mq_handle, st_osal_time_t *p_time_stamp, const void *send_data, size_t size)
{
	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	st_osal_time_t start_time;
	st_osal_time_t end_time;
	osal_nano_sec_t diff_time;
	int test_ret;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MESSAGE,
															OSAL_TEST_MQSENDUNTILTIMESTAMP,
															OSAL_TEST_ID011);

	osal_ret = R_OSAL_MqSendForTimePeriod(mq_handle, 1, send_data, size);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqSendForTimePeriod() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP,
				OSAL_TEST_ID011);
		return;
	}
	osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_PRECISION, p_time_stamp);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP,
				OSAL_TEST_ID011);
		return;
	}
	
	/* Start test function */
	start_time.tv_nsec = p_time_stamp->tv_nsec;
	start_time.tv_sec  = p_time_stamp->tv_sec;
    update_timestamp(p_time_stamp, 1, 2000000);
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_MqSendUntilTimeStamp(mq_handle, p_time_stamp, send_data, size);
	TIME_MEASURE_STOP
	/* End test function */

	if (osal_ret == expect_data)
	{
		(void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_PRECISION, &end_time);
		(void)R_OSAL_ClockTimeCalculateTimeDifference(&end_time, &start_time, &diff_time);
		if ((diff_time / 1000000) >= 1002)
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
			test_ret = OSAL_TEST_NG;
		}
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_MESSAGE, 
							OSAL_TEST_MQSENDUNTILTIMESTAMP,
							OSAL_TEST_ID011, 
							test_ret, opt_info);
}

/* ------------------------03_003_012 [OSAL_RETURN_STATE]------------------------ */
static void Test03_003_012(osal_mq_handle_t mq_handle, st_osal_time_t *p_time_stamp, const void *send_data, size_t size)
{
	Test03_003_common(mq_handle, p_time_stamp, send_data, size, OSAL_TEST_ID012);
}

#if 0
void Test03_003_013_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType,
						   drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	st_osal_time_t      abs_time;
	uint32_t            msg = 0xFFFF;

	get_timestamp(&abs_time, OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP, OSAL_TEST_ID013);
	abs_time.tv_sec  += 1;
	abs_time.tv_nsec += 2000000;

	Test03_003_common(g_mq_handle, &abs_time, &msg, sizeof(msg), OSAL_TEST_ID013);

	g_msg_cb_flag = 1;
}

/* ------------------------03_003_013 [ISR: success]------------------------ */
static void Test03_003_013()
{
	UINT16          Result;
	drvTMU_Time_t   Period;
	UINT8           Channel;
	char            rcv_data[128];
	e_osal_return_t osal_ret;

	Channel = 0U; /* Channel number */
	Period.Sec = 1; /* 1 second */
	Period.Usec = 500000; /* 500000 usec */

	Result = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test03_003_013_CallbackFunc, TMU_EDGE_RISE);
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP, OSAL_TEST_ID013);
		return;
	}

	while (g_msg_cb_flag == 0);
	g_msg_cb_flag = 0;

	osal_ret = R_OSAL_MqReceiveForTimePeriod(g_mq_handle, 10, rcv_data, sizeof(rcv_data));
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqReceiveForTimePeriod() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP,
				OSAL_TEST_ID013);
		return;
	}
}
#endif

#if 0
void Test03_003_014_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType,
						   drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	st_osal_time_t      abs_time;
	uint32_t            msg = 0xFFFF;

	get_timestamp(&abs_time, OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP, OSAL_TEST_ID014);
	abs_time.tv_sec  += 1;
	abs_time.tv_nsec += 2000000;

	Test03_003_common(g_mq_handle, &abs_time, &msg, sizeof(msg), OSAL_TEST_ID014);

	g_msg_cb_flag = 1;
}

/* ------------------------03_003_014 [ISR: OSAL_RETURN_BUSY]------------------------ */
static void Test03_003_014()
{
	UINT16          Result;
	drvTMU_Time_t   Period;
	UINT8           Channel;
	int32_t         msg = 0xFFFF;
	char            rcv_data[128];
	e_osal_return_t osal_ret;

	Channel = 0U; /* Channel number */
	Period.Sec = 1; /* 1 second */
	Period.Usec = 500000; /* 500000 usec */

	osal_ret = R_OSAL_MqSendForTimePeriod(g_mq_handle, 10, &msg, sizeof(msg));
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqSendForTimePeriod() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP,
				OSAL_TEST_ID014);
		return;
	}

	Result = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test03_003_014_CallbackFunc, TMU_EDGE_RISE);
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP, OSAL_TEST_ID014);
		return;
	}

	while (g_msg_cb_flag == 0);
	g_msg_cb_flag = 0;

	osal_ret = R_OSAL_MqReceiveForTimePeriod(g_mq_handle, 10, rcv_data, sizeof(rcv_data));
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqReceiveForTimePeriod() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP,
				OSAL_TEST_ID014);
		return;
	}
}
#endif

static void Test03_004_common(osal_mq_handle_t mq_handle, osal_milli_sec_t time_period, const void *send_data, size_t size, int test_id)
{
	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
	e_osal_return_t expect_data = OSAL_RETURN_OK;
	int             test_ret    = OSAL_TEST_OK;
	char                opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_ret = exec_R_OSAL_MqSendForTimePeriod(mq_handle, time_period, send_data, size, test_id, &expect_data, &test_ret);

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_MESSAGE, 
							OSAL_TEST_MQSENDFORTIMEPERIOD,
							test_id, 
							test_ret, opt_info);
}

/* ------------------------03_004_001 [success]------------------------ */
static void Test03_004_001(osal_mq_handle_t mq_handle, osal_milli_sec_t time_period, const void *send_data, size_t size)
{
	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
	char                rcv_data[128];

	Test03_004_common(mq_handle, time_period, send_data, size, OSAL_TEST_ID001);

	osal_ret = R_OSAL_MqReceiveForTimePeriod(mq_handle, time_period, rcv_data, size);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqReceiveForTimePeriod() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDFORTIMEPERIOD,
				OSAL_TEST_ID001);
		return;
	}
}

/* ------------------------03_004_002 [success]------------------------ */
static void Test03_004_002(osal_mq_handle_t mq_handle, osal_milli_sec_t time_period, const void *send_data, size_t size)
{
	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
	char                rcv_data[128];

	Test03_004_common(mq_handle, time_period, send_data, size, OSAL_TEST_ID002);

	osal_ret = R_OSAL_MqReceiveForTimePeriod(mq_handle, time_period, rcv_data, size);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqReceiveForTimePeriod() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDFORTIMEPERIOD,
				OSAL_TEST_ID002);
		return;
	}
}

/* ------------------------03_004_003 [OSAL_RETURN_HANDLE]------------------------ */
static void Test03_004_003(osal_mq_handle_t mq_handle, osal_milli_sec_t time_period, const void *send_data, size_t size)
{
	Test03_004_common(mq_handle, time_period, send_data, size, OSAL_TEST_ID003);
}

/* ------------------------03_004_004 [OSAL_RETURN_PAR]------------------------ */
static void Test03_004_004(osal_mq_handle_t mq_handle, osal_milli_sec_t time_period, const void *send_data, size_t size)
{
	Test03_004_common(mq_handle, time_period, send_data, size, OSAL_TEST_ID004);
}

/* ------------------------03_004_005 [OSAL_RETURN_PAR]------------------------ */
static void Test03_004_005(osal_mq_handle_t mq_handle, osal_milli_sec_t time_period, const void *send_data, size_t size)
{
	Test03_004_common(mq_handle, time_period, send_data, size, OSAL_TEST_ID005);
}

/* ------------------------03_004_006 [success]------------------------ */
static void Test03_004_006(osal_mq_handle_t mq_handle, osal_milli_sec_t time_period, const void *send_data, size_t size)
{
	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
	char                rcv_data[4];

	Test03_004_common(mq_handle, time_period, send_data, size, OSAL_TEST_ID006);

	osal_ret = R_OSAL_MqReceiveForTimePeriod(mq_handle, time_period, rcv_data, sizeof(rcv_data));
	//if (osal_ret != OSAL_RETURN_OK)
	//{
		//OSAL_TEST_ERR("R_OSAL_MqReceiveForTimePeriod() error.",
				//OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDFORTIMEPERIOD,
				//OSAL_TEST_ID006);
		//return;
	//}
}

/* ------------------------03_004_007 [OSAL_RETURN_PAR]------------------------ */
static void Test03_004_007(osal_mq_handle_t mq_handle, osal_milli_sec_t time_period, const void *send_data, size_t size)
{
	Test03_004_common(mq_handle, time_period, send_data, size, OSAL_TEST_ID007);
}

/* ------------------------03_004_008 [OSAL_RETURN_TIME]------------------------ */
static void Test03_004_008(osal_mq_handle_t mq_handle, osal_milli_sec_t time_period, const void *send_data, size_t size)
{
	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	st_osal_time_t start_time;
	st_osal_time_t end_time;
	osal_nano_sec_t diff_time;
	int test_ret;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MESSAGE,
															OSAL_TEST_MQSENDFORTIMEPERIOD,
															OSAL_TEST_ID008);

	osal_ret = R_OSAL_MqSendForTimePeriod(mq_handle, 10, send_data, size);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqSendForTimePeriod() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDFORTIMEPERIOD,
				OSAL_TEST_ID008);
		return;
	}
	osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &start_time);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDFORTIMEPERIOD,
				OSAL_TEST_ID008);
		return;
	}
	
	/* Start test function */
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_MqSendForTimePeriod(mq_handle, time_period, send_data, size);
	TIME_MEASURE_STOP
	/* End test function */

	if (osal_ret == expect_data)
	{
		(void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &end_time);
		(void)R_OSAL_ClockTimeCalculateTimeDifference(&end_time, &start_time, &diff_time);
		if ((diff_time / 1000000) >= 10)
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
			test_ret = OSAL_TEST_NG;
		}
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_MESSAGE, 
							OSAL_TEST_MQSENDFORTIMEPERIOD,
							OSAL_TEST_ID008, 
							test_ret, opt_info);
}

/* ------------------------03_003_009 [OSAL_RETURN_STATE]------------------------ */
static void Test03_004_009(osal_mq_handle_t mq_handle, osal_milli_sec_t time_period, const void *send_data, size_t size)
{
	Test03_004_common(mq_handle, time_period, send_data, size, OSAL_TEST_ID009);
}

#if 0
void Test03_004_010_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType,
						   drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	uint32_t msg = 0xFFFF;

	Test03_004_common(g_mq_handle, 10, &msg, sizeof(msg), OSAL_TEST_ID010);

	g_msg_cb_flag = 1;
}

/* ------------------------03_003_010 [ISR: success]------------------------ */
static void Test03_004_010()
{
	UINT16          Result;
	drvTMU_Time_t   Period;
	UINT8           Channel;
	char            rcv_data[128];
	e_osal_return_t osal_ret;

	Channel = 0U; /* Channel number */
	Period.Sec = 1; /* 1 second */
	Period.Usec = 500000; /* 500000 usec */

	Result = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test03_004_010_CallbackFunc, TMU_EDGE_RISE);
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDFORTIMEPERIOD, OSAL_TEST_ID010);
		return;
	}

	while (g_msg_cb_flag == 0);
	g_msg_cb_flag = 0;

	osal_ret = R_OSAL_MqReceiveForTimePeriod(g_mq_handle, 10, rcv_data, sizeof(rcv_data));
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqReceiveForTimePeriod() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDFORTIMEPERIOD,
				OSAL_TEST_ID010);
		return;
	}
}

void Test03_004_011_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType,
						   drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	uint32_t msg = 0xFFFF;

	Test03_004_common(g_mq_handle, 10, &msg, sizeof(msg), OSAL_TEST_ID011);

	g_msg_cb_flag = 1;
}

/* ------------------------03_004_011 [ISR: OSAL_RETURN_BUSY]------------------------ */
static void Test03_004_011()
{
	UINT16          Result;
	drvTMU_Time_t   Period;
	UINT8           Channel;
	int32_t         msg = 0xFFFF;
	char            rcv_data[128];
	e_osal_return_t osal_ret;

	Channel = 0U; /* Channel number */
	Period.Sec = 1; /* 1 second */
	Period.Usec = 500000; /* 500000 usec */

	osal_ret = R_OSAL_MqSendForTimePeriod(g_mq_handle, period, &msg, sizeof(msg));
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqSendForTimePeriod() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDFORTIMEPERIOD,
				OSAL_TEST_ID011);
		return;
	}

	Result = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test03_004_011_CallbackFunc, TMU_EDGE_RISE);
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDFORTIMEPERIOD, OSAL_TEST_ID011);
		return;
	}

	while (g_msg_cb_flag == 0);
	g_msg_cb_flag = 0;

	osal_ret = R_OSAL_MqReceiveForTimePeriod(g_mq_handle, 10, rcv_data, sizeof(rcv_data));
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqReceiveForTimePeriod() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDFORTIMEPERIOD,
				OSAL_TEST_ID011);
		return;
	}
}
#endif

static void Test03_005_common(osal_mq_handle_t mq_handle, st_osal_time_t *p_time_stamp, void *receive_data, size_t size, int test_id)
{
	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
	e_osal_return_t expect_data = OSAL_RETURN_OK;
	int             test_ret    = OSAL_TEST_OK;
	char                opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_ret = exec_R_OSAL_MqReceiveUntilTimeStamp(mq_handle, p_time_stamp, receive_data, size, test_id, &expect_data, &test_ret);

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_MESSAGE, 
							OSAL_TEST_MQRECEIVEUNTILTIMESTAMP,
							test_id, 
							test_ret, opt_info);	
}

/* ------------------------03_005_001 [success]------------------------ */
static void Test03_005_001(osal_mq_handle_t mq_handle, st_osal_time_t *p_time_stamp, void *receive_data, size_t size)
{
	Test03_005_common(mq_handle, p_time_stamp, receive_data, size, OSAL_TEST_ID001);
}

/* ------------------------03_005_002 [success]------------------------ */
static void Test03_005_002(osal_mq_handle_t mq_handle, st_osal_time_t *p_time_stamp, void *receive_data, size_t size)
{
	Test03_005_common(mq_handle, p_time_stamp, receive_data, size, OSAL_TEST_ID002);
}

/* ------------------------03_005_003 [success]------------------------ */
static void Test03_005_003(osal_mq_handle_t mq_handle, st_osal_time_t *p_time_stamp, void *receive_data, size_t size)
{
	Test03_005_common(mq_handle, p_time_stamp, receive_data, size, OSAL_TEST_ID003);
}

/* ------------------------03_005_004 [success]------------------------ */
static void Test03_005_004(osal_mq_handle_t mq_handle, st_osal_time_t *p_time_stamp, void *receive_data, size_t size)
{
	Test03_005_common(mq_handle, p_time_stamp, receive_data, size, OSAL_TEST_ID004);
}

/* ------------------------03_005_005 [OSAL_RETURN_HANDLE]------------------------ */
static void Test03_005_005(osal_mq_handle_t mq_handle, st_osal_time_t *p_time_stamp, void *receive_data, size_t size)
{
	Test03_005_common(mq_handle, p_time_stamp, receive_data, size, OSAL_TEST_ID005);
}

/* ------------------------03_005_006 [OSAL_RETURN_PAR]------------------------ */
static void Test03_005_006(osal_mq_handle_t mq_handle, st_osal_time_t *p_time_stamp, void *receive_data, size_t size)
{
	Test03_005_common(mq_handle, p_time_stamp, receive_data, size, OSAL_TEST_ID006);
}

/* ------------------------03_005_007 [OSAL_RETURN_PAR]------------------------ */
static void Test03_005_007(osal_mq_handle_t mq_handle, st_osal_time_t *p_time_stamp, void *receive_data, size_t size)
{
	Test03_005_common(mq_handle, p_time_stamp, receive_data, size, OSAL_TEST_ID007);
}

/* ------------------------03_005_008 [OSAL_RETURN_PAR]------------------------ */
static void Test03_005_008(osal_mq_handle_t mq_handle, st_osal_time_t *p_time_stamp, void *receive_data, size_t size)
{
	Test03_005_common(mq_handle, p_time_stamp, receive_data, size, OSAL_TEST_ID008);
}

/* ------------------------03_005_009 [OSAL_RETURN_PAR]------------------------ */
static void Test03_005_009(osal_mq_handle_t mq_handle, st_osal_time_t *p_time_stamp, void *receive_data, size_t size)
{
	Test03_005_common(mq_handle, p_time_stamp, receive_data, size, OSAL_TEST_ID009);
}

/* ------------------------03_005_012 [OSAL_RETURN_TIME]------------------------ */
static void Test03_005_010(osal_mq_handle_t mq_handle, st_osal_time_t *p_time_stamp, void *receive_data, size_t size)
{
	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	st_osal_time_t start_time;
	st_osal_time_t end_time;
	osal_nano_sec_t diff_time;
	int test_ret;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MESSAGE,
															OSAL_TEST_MQRECEIVEUNTILTIMESTAMP,
															OSAL_TEST_ID010);

	osal_ret = R_OSAL_MqReceiveForTimePeriod(mq_handle, 1, receive_data, size);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqReceiveForTimePeriod() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEUNTILTIMESTAMP,
				OSAL_TEST_ID010);
		return;
	}
	osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, p_time_stamp);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEUNTILTIMESTAMP,
				OSAL_TEST_ID010);
		return;
	}
	
	/* Start test function */
	start_time.tv_nsec = p_time_stamp->tv_nsec;
	start_time.tv_sec  = p_time_stamp->tv_sec;
    update_timestamp(p_time_stamp, 1, 2000000);
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_MqReceiveUntilTimeStamp(mq_handle, p_time_stamp, receive_data, size);
	TIME_MEASURE_STOP
	/* End test function */

	if (osal_ret == expect_data)
	{
		(void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &end_time);
		(void)R_OSAL_ClockTimeCalculateTimeDifference(&end_time, &start_time, &diff_time);
		if ((diff_time / 1000000) >= 1002)
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
			test_ret = OSAL_TEST_NG;
		}
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_MESSAGE, 
							OSAL_TEST_MQRECEIVEUNTILTIMESTAMP,
							OSAL_TEST_ID010, 
							test_ret, opt_info);
}

/* ------------------------03_005_011 [OSAL_RETURN_STATE]------------------------ */
static void Test03_005_011(osal_mq_handle_t mq_handle, st_osal_time_t *p_time_stamp, void *receive_data, size_t size)
{
	Test03_005_common(mq_handle, p_time_stamp, receive_data, size, OSAL_TEST_ID011);
}

#if 0
void Test03_005_012_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType,
						   drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	st_osal_time_t abs_time;
	uint32_t       msg = 0xFFFF;
	char           rcv_data[128];

	send_msg_and_get_timestamp(g_mq_handle, &msg, &abs_time, OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEUNTILTIMESTAMP, OSAL_TEST_ID012);
	abs_time.tv_sec  += 1;
	abs_time.tv_nsec += 2000000;

	Test03_005_common(g_mq_handle, &abs_time, rcv_data, sizeof(rcv_data), OSAL_TEST_ID012);

	g_msg_cb_flag = 1;
}

/* ------------------------03_005_012 [ISR: success]------------------------ */
static void Test03_005_012()
{
	UINT16          Result;
	drvTMU_Time_t   Period;
	UINT8           Channel;

	Channel = 0U; /* Channel number */
	Period.Sec = 1; /* 1 second */
	Period.Usec = 500000; /* 500000 usec */

	Result = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test03_005_012_CallbackFunc, TMU_EDGE_RISE);
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEUNTILTIMESTAMP, OSAL_TEST_ID012);
		return;
	}

	while (g_msg_cb_flag == 0);
	g_msg_cb_flag = 0;
}

void Test03_005_013_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType,
						   drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	st_osal_time_t  abs_time;
	uint32_t        msg = 0xFFFF;
	char            rcv_data[128];
	int test_ret;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MESSAGE,
															OSAL_TEST_MQRECEIVEUNTILTIMESTAMP,
															OSAL_TEST_ID013);

	osal_ret = R_OSAL_MqSendForTimePeriod(g_mq_handle, 1, &msg, sizeof(msg));
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqSendForTimePeriod() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEUNTILTIMESTAMP,
				OSAL_TEST_ID013);
		return;
	}
	osal_ret = R_OSAL_MqReceiveForTimePeriod(g_mq_handle, 1, rcv_data, sizeof(rcv_data));
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqReceiveForTimePeriod() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEUNTILTIMESTAMP,
				OSAL_TEST_ID013);
		return;
	}
	osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEUNTILTIMESTAMP,
				OSAL_TEST_ID013);
		return;
	}	
	abs_time.tv_sec  += 1;
	abs_time.tv_nsec += 2000000;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_MqReceiveUntilTimeStamp(g_mq_handle, &abs_time, rcv_data, sizeof(rcv_data));
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
	osal_test_result_output(OSAL_TEST_MESSAGE, 
							OSAL_TEST_MQRECEIVEUNTILTIMESTAMP,
							OSAL_TEST_ID013, 
							test_ret, opt_info);

	g_msg_cb_flag = 1;
}

/* ------------------------03_005_013 [ISR: OSAL_RETURN_BUSY]------------------------ */
static void Test03_005_013()
{
	UINT16          Result;
	drvTMU_Time_t   Period;
	UINT8           Channel;

	Channel = 0U; /* Channel number */
	Period.Sec = 1; /* 1 second */
	Period.Usec = 500000; /* 500000 usec */

	Result = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test03_005_013_CallbackFunc, TMU_EDGE_RISE);
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEUNTILTIMESTAMP, OSAL_TEST_ID013);
		return;
	}

	while (g_msg_cb_flag == 0);
	g_msg_cb_flag = 0;
}
#endif

static void Test03_006_common(osal_mq_handle_t mq_handle, osal_milli_sec_t time_period, void *receive_data, size_t size, int test_id)
{
	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
	e_osal_return_t expect_data = OSAL_RETURN_OK;
	int             test_ret    = OSAL_TEST_OK;
	char                opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_ret = exec_R_OSAL_MqReceiveForTimePeriod(mq_handle, time_period, receive_data, size, test_id, &expect_data, &test_ret);

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_MESSAGE, 
							OSAL_TEST_MQRECEIVEFORTIMEPERIOD,
							test_id, 
							test_ret, opt_info);	
}

/* ------------------------03_006_001 [success]------------------------ */
static void Test03_006_001(osal_mq_handle_t mq_handle, osal_milli_sec_t time_period, void *receive_data, size_t size)
{
	Test03_006_common(mq_handle, time_period, receive_data, size, OSAL_TEST_ID001);
}

/* ------------------------03_006_002 [success]------------------------ */
static void Test03_006_002(osal_mq_handle_t mq_handle, osal_milli_sec_t time_period, void *receive_data, size_t size)
{
	Test03_006_common(mq_handle, time_period, receive_data, size, OSAL_TEST_ID002);
}

/* ------------------------03_006_003 [OSAL_RETURN_HANDLE]------------------------ */
static void Test03_006_003(osal_mq_handle_t mq_handle, osal_milli_sec_t time_period, void *receive_data, size_t size)
{
	Test03_006_common(mq_handle, time_period, receive_data, size, OSAL_TEST_ID003);
}

/* ------------------------03_006_004 [OSAL_RETURN_PAR]------------------------ */
static void Test03_006_004(osal_mq_handle_t mq_handle, osal_milli_sec_t time_period, void *receive_data, size_t size)
{
	Test03_006_common(mq_handle, time_period, receive_data, size, OSAL_TEST_ID004);
}

/* ------------------------03_006_005 [OSAL_RETURN_PAR]------------------------ */
static void Test03_006_005(osal_mq_handle_t mq_handle, osal_milli_sec_t time_period, void *receive_data, size_t size)
{
	Test03_006_common(mq_handle, time_period, receive_data, size, OSAL_TEST_ID005);
}

/* ------------------------03_006_006 [OSAL_RETURN_PAR]------------------------ */
static void Test03_006_006(osal_mq_handle_t mq_handle, osal_milli_sec_t time_period, void *receive_data, size_t size)
{
	Test03_006_common(mq_handle, time_period, receive_data, size, OSAL_TEST_ID006);
}

/* ------------------------03_006_007 [OSAL_RETURN_TIME]------------------------ */
static void Test03_006_007(osal_mq_handle_t mq_handle, osal_milli_sec_t time_period, void *receive_data, size_t size)
{
	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	st_osal_time_t start_time;
	st_osal_time_t end_time;
	osal_nano_sec_t diff_time;
	int test_ret;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MESSAGE,
															OSAL_TEST_MQRECEIVEFORTIMEPERIOD,
															OSAL_TEST_ID007);

	osal_ret = R_OSAL_MqReceiveForTimePeriod(mq_handle, 1, receive_data, size);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqReceiveForTimePeriod() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEFORTIMEPERIOD,
				OSAL_TEST_ID007);
		return;
	}

	osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &start_time);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEFORTIMEPERIOD,
				OSAL_TEST_ID007);
		return;
	}

	/* Start test function */
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_MqReceiveForTimePeriod(mq_handle, time_period, receive_data, size);
	TIME_MEASURE_STOP
	/* End test function */

	if (osal_ret == expect_data)
	{
		(void)R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &end_time);
		(void)R_OSAL_ClockTimeCalculateTimeDifference(&end_time, &start_time, &diff_time);
		if ((diff_time / 1000000) >= 10)
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
			test_ret = OSAL_TEST_NG;
		}
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_MESSAGE, 
							OSAL_TEST_MQRECEIVEFORTIMEPERIOD,
							OSAL_TEST_ID007, 
							test_ret, opt_info);
}

/* ------------------------03_006_008 [OSAL_RETURN_STATE]------------------------ */
static void Test03_006_008(osal_mq_handle_t mq_handle, osal_milli_sec_t time_period, void *receive_data, size_t size)
{
	Test03_006_common(mq_handle, time_period, receive_data, size, OSAL_TEST_ID008);
}

#if 0
void Test03_006_009_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType,
						   drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	uint32_t       msg = 0xFFFF;
	char           rcv_data[128];

	send_message(g_mq_handle, &msg, OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEFORTIMEPERIOD, OSAL_TEST_ID009);
	Test03_006_common(g_mq_handle, 10, rcv_data, sizeof(rcv_data), OSAL_TEST_ID009);

	g_msg_cb_flag = 1;
}

/* ------------------------03_006_009 [ISR: success]------------------------ */
static void Test03_006_009()
{
	UINT16          Result;
	drvTMU_Time_t   Period;
	UINT8           Channel;

	Channel = 0U; /* Channel number */
	Period.Sec = 1; /* 1 second */
	Period.Usec = 500000; /* 500000 usec */

	Result = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test03_006_009_CallbackFunc, TMU_EDGE_RISE);
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEFORTIMEPERIOD, OSAL_TEST_ID009);
		return;
	}

	while (g_msg_cb_flag == 0);
	g_msg_cb_flag = 0;
}

void Test03_006_010_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType,
						   drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	uint32_t        msg = 0xFFFF;
	char            rcv_data[128];
	int test_ret;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MESSAGE,
															OSAL_TEST_MQRECEIVEFORTIMEPERIOD,
															OSAL_TEST_ID010);

	osal_ret = R_OSAL_MqSendForTimePeriod(g_mq_handle, 1, &msg, sizeof(msg));
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqSendForTimePeriod() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEFORTIMEPERIOD,
				OSAL_TEST_ID010);
		return;
	}
	osal_ret = R_OSAL_MqReceiveForTimePeriod(g_mq_handle, 1, rcv_data, sizeof(rcv_data));
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqReceiveForTimePeriod() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEFORTIMEPERIOD,
				OSAL_TEST_ID010);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_MqReceiveForTimePeriod(g_mq_handle, 10, rcv_data, sizeof(rcv_data));
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
	osal_test_result_output(OSAL_TEST_MESSAGE, 
							OSAL_TEST_MQRECEIVEFORTIMEPERIOD,
							OSAL_TEST_ID010, 
							test_ret, opt_info);

	g_msg_cb_flag = 1;
}

/* ------------------------03_006_010 [ISR: OSAL_RETURN_BUSY]------------------------ */
static void Test03_006_010()
{
	UINT16          Result;
	drvTMU_Time_t   Period;
	UINT8           Channel;

	Channel = 0U; /* Channel number */
	Period.Sec = 1; /* 1 second */
	Period.Usec = 500000; /* 500000 usec */

	Result = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test03_006_010_CallbackFunc, TMU_EDGE_RISE);
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEFORTIMEPERIOD, OSAL_TEST_ID010);
		return;
	}

	while (g_msg_cb_flag == 0);
	g_msg_cb_flag = 0;
}
#endif

static void Test03_007_common(osal_mq_handle_t mq_handle, bool *mq_result, bool expect_mq_result, int test_id)
{
	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
	e_osal_return_t expect_data = OSAL_RETURN_OK;
	int             test_ret    = OSAL_TEST_NG;
	char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MESSAGE, 
														   OSAL_TEST_MQISFULL,
														   test_id);

	/* Start test function */
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_MqIsFull(mq_handle, mq_result);
	TIME_MEASURE_STOP
	/* End test function */

	if (osal_ret == expect_data)
	{
		if (expect_data == OSAL_RETURN_OK)
		{
			if (*mq_result == expect_mq_result)
			{
				test_ret = OSAL_TEST_OK;
			}
		}
		else
		{
			test_ret = OSAL_TEST_OK;
		}
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_MESSAGE, 
							OSAL_TEST_MQISFULL,
							test_id, 
							test_ret, opt_info);
}

/* ------------------------03_007_001 [success]------------------------ */
static void Test03_007_001(osal_mq_handle_t mq_handle, bool *mq_result)
{
	Test03_007_common(mq_handle, mq_result, false, OSAL_TEST_ID001);
}

/* ------------------------03_007_002 [success]------------------------ */
static void Test03_007_002(osal_mq_handle_t mq_handle, bool *mq_result)
{
	uint32_t send_data = 0xFFFF;
	char     rcv_data[4];

	send_message(mq_handle, &send_data, OSAL_TEST_MESSAGE, OSAL_TEST_MQISFULL, OSAL_TEST_ID002);

	/* Start test function */
	Test03_007_common(mq_handle, mq_result, true, OSAL_TEST_ID002);
	/* End test function */

	receive_message(mq_handle, rcv_data, sizeof(rcv_data), OSAL_TEST_MESSAGE, OSAL_TEST_MQISFULL, OSAL_TEST_ID002);
}

/* ------------------------03_007_003 [OSAL_RETURN_PAR]------------------------ */
static void Test03_007_003(osal_mq_handle_t mq_handle, bool *mq_result)
{
	Test03_007_common(mq_handle, mq_result, NULL, OSAL_TEST_ID003);
}

/* ------------------------03_007_004 [OSAL_RETURN_HANDLE]------------------------ */
static void Test03_007_004(osal_mq_handle_t mq_handle, bool *mq_result)
{
	Test03_007_common(mq_handle, mq_result, NULL, OSAL_TEST_ID004);
}

/* ------------------------03_007_005 [OSAL_RETURN_STATE]------------------------ */
static void Test03_007_005(osal_mq_handle_t mq_handle, bool *mq_result)
{
	Test03_007_common(mq_handle, mq_result, NULL, OSAL_TEST_ID005);
}

#if 0
void Test03_007_006_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType,
						   drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	bool mq_result;

	Test03_007_common(g_mq_handle, &mq_result, false, OSAL_TEST_ID006);
	
	g_msg_cb_flag = 1;
}

/* ------------------------03_007_006 [ISR: success]------------------------ */
static void Test03_007_006()
{
	UINT16          Result;
	drvTMU_Time_t   Period;
	UINT8           Channel;

	Channel = 0U; /* Channel number */
	Period.Sec = 1; /* 1 second */
	Period.Usec = 500000; /* 500000 usec */

	Result = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test03_007_006_CallbackFunc, TMU_EDGE_RISE);
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQISFULL, OSAL_TEST_ID006);
		return;
	}

	while (g_msg_cb_flag == 0);
	g_msg_cb_flag = 0;
}

void Test03_007_007_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType,
						   drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	bool mq_result;
	uint32_t send_data = 0xFFFF;
	char     rcv_data[128];

	send_message(g_mq_handle, &send_data, OSAL_TEST_MESSAGE, OSAL_TEST_MQISFULL, OSAL_TEST_ID007);

	Test03_007_common(g_mq_handle, &mq_result, true, OSAL_TEST_ID007);
	
	receive_message(g_mq_handle, rcv_data, sizeof(rcv_data), OSAL_TEST_MESSAGE, OSAL_TEST_MQISFULL, OSAL_TEST_ID007);
	g_msg_cb_flag = 1;
}

/* ------------------------03_007_007 [ISR: success]------------------------ */
static void Test03_007_007()
{
	UINT16          Result;
	drvTMU_Time_t   Period;
	UINT8           Channel;

	Channel = 0U; /* Channel number */
	Period.Sec = 1; /* 1 second */
	Period.Usec = 500000; /* 500000 usec */

	Result = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test03_007_007_CallbackFunc, TMU_EDGE_RISE);
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQISFULL, OSAL_TEST_ID007);
		return;
	}

	while (g_msg_cb_flag == 0);
	g_msg_cb_flag = 0;
}
#endif

static void Test03_008_common(osal_mq_handle_t mq_handle, bool *mq_result, bool expect_mq_result, int test_id)
{
	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
	e_osal_return_t expect_data = OSAL_RETURN_OK;
	int             test_ret    = OSAL_TEST_NG;
	char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MESSAGE, 
														   OSAL_TEST_MQISEMPTY,
														   test_id);

	/* Start test function */
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_MqIsEmpty(mq_handle, mq_result);
	TIME_MEASURE_STOP
	/* End test function */

	if (osal_ret == expect_data)
	{
		if (expect_data == OSAL_RETURN_OK)
		{
			if (*mq_result == expect_mq_result)
			{
				test_ret = OSAL_TEST_OK;
			}
		}
		else
		{
			test_ret = OSAL_TEST_OK;
		}
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_MESSAGE, 
							OSAL_TEST_MQISEMPTY,
							test_id, 
							test_ret, opt_info);
}

/* ------------------------03_008_001 [success]------------------------ */
static void Test03_008_001(osal_mq_handle_t mq_handle, bool *mq_result)
{
	Test03_008_common(mq_handle, mq_result, true, OSAL_TEST_ID001);
}

/* ------------------------03_008_002 [success]------------------------ */
static void Test03_008_002(osal_mq_handle_t mq_handle, bool *mq_result)
{
	uint32_t send_data = 0xFFFF;
	char     rcv_data[4];

	send_message(mq_handle, &send_data, OSAL_TEST_MESSAGE, OSAL_TEST_MQISEMPTY, OSAL_TEST_ID002);

	/* Start test function */
	Test03_008_common(mq_handle, mq_result, false, OSAL_TEST_ID002);
	/* End test function */

	receive_message(mq_handle, rcv_data, sizeof(rcv_data), OSAL_TEST_MESSAGE, OSAL_TEST_MQISEMPTY, OSAL_TEST_ID002);
}

/* ------------------------03_008_003 [OSAL_RETURN_PAR]------------------------ */
static void Test03_008_003(osal_mq_handle_t mq_handle, bool *mq_result)
{
	Test03_008_common(mq_handle, mq_result, NULL, OSAL_TEST_ID003);
}

/* ------------------------03_008_004 [OSAL_RETURN_HANDLE]------------------------ */
static void Test03_008_004(osal_mq_handle_t mq_handle, bool *mq_result)
{
	Test03_008_common(mq_handle, mq_result, NULL, OSAL_TEST_ID004);
}

/* ------------------------03_008_005 [OSAL_RETURN_STATE]------------------------ */
static void Test03_008_005(osal_mq_handle_t mq_handle, bool *mq_result)
{
	Test03_008_common(mq_handle, mq_result, NULL, OSAL_TEST_ID005);
}

#if 0
void Test03_008_006_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType,
						   drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	bool mq_result;

	Test03_008_common(g_mq_handle, &mq_result, true, OSAL_TEST_ID006);
	
	g_msg_cb_flag = 1;
}

/* ------------------------03_008_006 [ISR: success]------------------------ */
static void Test03_008_006()
{
	UINT16          Result;
	drvTMU_Time_t   Period;
	UINT8           Channel;

	Channel = 0U; /* Channel number */
	Period.Sec = 1; /* 1 second */
	Period.Usec = 500000; /* 500000 usec */

	Result = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test03_008_006_CallbackFunc, TMU_EDGE_RISE);
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQISEMPTY, OSAL_TEST_ID006);
		return;
	}

	while (g_msg_cb_flag == 0);
	g_msg_cb_flag = 0;
}

void Test03_008_007_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType,
						   drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	bool mq_result;
	uint32_t send_data = 0xFFFF;
	char     rcv_data[128];

	send_message(g_mq_handle, &send_data, OSAL_TEST_MESSAGE, OSAL_TEST_MQISEMPTY, OSAL_TEST_ID007);

	Test03_008_common(g_mq_handle, &mq_result, false, OSAL_TEST_ID007);
	
	receive_message(g_mq_handle, rcv_data, sizeof(rcv_data), OSAL_TEST_MESSAGE, OSAL_TEST_MQISEMPTY, OSAL_TEST_ID007);
	g_msg_cb_flag = 1;
}

/* ------------------------03_008_007 [ISR: success]------------------------ */
static void Test03_008_007()
{
	UINT16          Result;
	drvTMU_Time_t   Period;
	UINT8           Channel;

	Channel = 0U; /* Channel number */
	Period.Sec = 1; /* 1 second */
	Period.Usec = 500000; /* 500000 usec */

	Result = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test03_008_007_CallbackFunc, TMU_EDGE_RISE);
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQISEMPTY, OSAL_TEST_ID007);
		return;
	}

	while (g_msg_cb_flag == 0);
	g_msg_cb_flag = 0;
}
#endif

static void Test03_009_common(osal_mq_handle_t mq_handle, st_osal_mq_config_t *p_config, int test_id)
{
	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
	e_osal_return_t expect_data = OSAL_RETURN_OK;
	int             test_ret    = OSAL_TEST_NG;
	char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MESSAGE, 
														   OSAL_TEST_MQGETCONFIG,
														   test_id);

	/* Start test function */
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_MqGetConfig(mq_handle, p_config);
	TIME_MEASURE_STOP
	/* End test function */

	if (osal_ret == expect_data)
	{
		if (expect_data == OSAL_RETURN_OK)
		{
			if ((p_config->max_num_msg == 1) && (p_config->msg_size == 4))
			{
				test_ret = OSAL_TEST_OK;
			}
		}
		else
		{
			test_ret = OSAL_TEST_OK;
		}
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_MESSAGE, 
							OSAL_TEST_MQGETCONFIG,
							test_id, 
							test_ret, opt_info);
}

/* ------------------------03_009_001 [success]------------------------ */
static void Test03_009_001(osal_mq_handle_t mq_handle, st_osal_mq_config_t *p_config)
{
	Test03_009_common(mq_handle, p_config, OSAL_TEST_ID001);
}

/* ------------------------03_009_002 [OSAL_RETURN_PAR]------------------------ */
static void Test03_009_002(osal_mq_handle_t mq_handle, st_osal_mq_config_t *p_config)
{
	Test03_009_common(mq_handle, p_config, OSAL_TEST_ID002);
}

/* ------------------------03_009_003 [OSAL_RETURN_HANDLE]------------------------ */
static void Test03_009_003(osal_mq_handle_t mq_handle, st_osal_mq_config_t *p_config)
{
	Test03_009_common(mq_handle, p_config, OSAL_TEST_ID003);
}

/* ------------------------03_009_004 [OSAL_RETURN_STATE]------------------------ */
static void Test03_009_004(osal_mq_handle_t mq_handle, st_osal_mq_config_t *p_config)
{
	Test03_009_common(mq_handle, p_config, OSAL_TEST_ID004);
}

#if 0
void Test03_009_005_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType,
						   drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	st_osal_mq_config_t config;

	Test03_009_common(g_mq_handle, &config, OSAL_TEST_ID005);

	g_msg_cb_flag = 1;
}

/* ------------------------03_009_005 [ISR: success]------------------------ */
static void Test03_009_005()
{
	UINT16          Result;
	drvTMU_Time_t   Period;
	UINT8           Channel;

	Channel = 0U; /* Channel number */
	Period.Sec = 1; /* 1 second */
	Period.Usec = 500000; /* 500000 usec */

	Result = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test03_009_005_CallbackFunc, TMU_EDGE_RISE);
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQGETCONFIG, OSAL_TEST_ID005);
		return;
	}

	while (g_msg_cb_flag == 0);
	g_msg_cb_flag = 0;
}
#endif

static void Test03_010_common(osal_mq_handle_t mq_handle, int test_id)
{
	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
	e_osal_return_t expect_data = OSAL_RETURN_OK;
	int             test_ret    = OSAL_TEST_NG;
	char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	bool            mq_result;
	uint32_t send_data = 0xFFFF;
	uint32_t receive_data;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MESSAGE, 
														   OSAL_TEST_MQRESET,
														   test_id);

	/* Start test function */
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_MqReset(mq_handle);
	TIME_MEASURE_STOP
	/* End test function */

	if (osal_ret == expect_data)
	{
		if (expect_data == OSAL_RETURN_OK)
		{
			osal_ret = R_OSAL_MqIsEmpty(mq_handle, &mq_result);
			if (osal_ret != OSAL_RETURN_OK)
			{
				OSAL_TEST_ERR("R_OSAL_MqIsEmpty() error.",
						OSAL_TEST_MESSAGE, OSAL_TEST_MQRESET,
						test_id);
				return;
			}
			else if (mq_result == true)
			{
				osal_ret = R_OSAL_MqSendForTimePeriod(mq_handle, 10, &send_data, sizeof(send_data));
				if (osal_ret != OSAL_RETURN_OK)
				{
					OSAL_TEST_ERR("R_OSAL_MqSendForTimePeriod() error.",
							OSAL_TEST_MESSAGE, OSAL_TEST_MQRESET,
							test_id);
					return;
				}
				osal_ret = R_OSAL_MqReceiveForTimePeriod(mq_handle, 10, &receive_data, sizeof(send_data));
				if (osal_ret != OSAL_RETURN_OK)
				{
					OSAL_TEST_ERR("R_OSAL_MqReceiveForTimePeriod() error.",
							OSAL_TEST_MESSAGE, OSAL_TEST_MQRESET,
							test_id);
					return;
				}
				test_ret = OSAL_TEST_OK;
			}
			else
			{
				/* Result is fail because message queue is not empty */
			}
		}
		else
		{
			test_ret = OSAL_TEST_OK;
		}
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_MESSAGE, 
							OSAL_TEST_MQRESET,
							test_id, 
							test_ret, opt_info);
}

/* ------------------------03_010_001 [success]------------------------ */
static void Test03_010_001(osal_mq_handle_t mq_handle)
{
	uint32_t send_data = 0xFFFF;
	
	send_message(mq_handle, &send_data, OSAL_TEST_MESSAGE, OSAL_TEST_MQRESET, OSAL_TEST_ID001);

	Test03_010_common(mq_handle, OSAL_TEST_ID001);
}

/* ------------------------03_010_002 [OSAL_RETURN_HANDLE]------------------------ */
static void Test03_010_002(osal_mq_handle_t mq_handle)
{
	Test03_010_common(mq_handle, OSAL_TEST_ID002);
}

/* ------------------------03_010_003 [OSAL_RETURN_STATE]------------------------ */
static void Test03_010_003(osal_mq_handle_t mq_handle)
{
	Test03_010_common(mq_handle, OSAL_TEST_ID003);
}

/* ------------------------03_010_004 [OSAL_RETURN_FAIL]------------------------ */
// static void Test03_010_004(osal_mq_handle_t mq_handle)
// {
// 	/* Set stub flag */
// 	g_stub_flag.QueueReset = QUEUERESET_pdFAIL;

// 	Test03_010_common(mq_handle, OSAL_TEST_ID004);

// 	/* Clear stub flag */
// 	g_stub_flag.QueueReset = 0;
// }

static void Test03_011_001()
{
	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
	e_osal_return_t expect_data = OSAL_RETURN_OK;
	int             test_ret    = OSAL_TEST_NG;
	char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];
    st_osal_mq_config_t config;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MESSAGE, 
														   OSAL_TEST_MQINITIALIZEMQCONFIGST,
														   OSAL_TEST_ID001);

	/* Start test function */
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_MqInitializeMqConfigSt(&config);
	TIME_MEASURE_STOP
	/* End test function */

	if (osal_ret == expect_data)
	{
        test_ret = OSAL_TEST_OK;
	}
    else
    {
        test_ret = OSAL_TEST_NG;
    }

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_MESSAGE, 
							OSAL_TEST_MQINITIALIZEMQCONFIGST,
							OSAL_TEST_ID001, 
							test_ret, opt_info);
}

static void Test03_011_002()
{
	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
	e_osal_return_t expect_data = OSAL_RETURN_OK;
	int             test_ret    = OSAL_TEST_NG;
	char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MESSAGE, 
														   OSAL_TEST_MQINITIALIZEMQCONFIGST,
														   OSAL_TEST_ID002);

	/* Start test function */
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_MqInitializeMqConfigSt(NULL);
	TIME_MEASURE_STOP
	/* End test function */

	if (osal_ret == expect_data)
	{
        test_ret = OSAL_TEST_OK;
	}
    else
    {
        test_ret = OSAL_TEST_NG;
    }

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_MESSAGE, 
							OSAL_TEST_MQINITIALIZEMQCONFIGST,
							OSAL_TEST_ID002, 
							test_ret, opt_info);
}

/* test thread */
int64_t osal_test_mq1(void *user_arg)
{

	osal_mq_handle_t mq_handle = NULL;
	e_osal_return_t  osal_ret1;
	e_osal_return_t  osal_ret2;
	osal_milli_sec_t time;
	uint32_t         send_data;

	memcpy(&mq_handle, (osal_mq_handle_t *)user_arg, sizeof(mq_handle));
	time = 5000;

	R_OSAL_ThreadSleepForTimePeriod(1);

	/* message send */
	send_data = 0x1111;
	osal_ret1 = R_OSAL_MqSendForTimePeriod(mq_handle, time, &send_data, sizeof(send_data));
	if (osal_ret1 != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqSendForTimePeriod() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEFORTIMEPERIOD,
				OSAL_TEST_ID011);
	}
	send_data = 0x2222;
	osal_ret2 = R_OSAL_MqSendForTimePeriod(mq_handle, time, &send_data, sizeof(send_data));
	if (osal_ret2 != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqSendForTimePeriod() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEFORTIMEPERIOD,
				OSAL_TEST_ID011);
	}

	if (osal_ret2 != OSAL_RETURN_OK)
	{
		osal_ret1 = osal_ret2;
	}
	return (int64_t)osal_ret1;
}

/* test thread */
int64_t osal_test_mq2(void *user_arg)
{

	osal_mq_handle_t mq_handle = NULL;
	e_osal_return_t   osal_ret;
	bool             diff_msg = false;
	osal_milli_sec_t time;
	uint32_t         recv_data;

	R_OSAL_ThreadSleepForTimePeriod(1);

	memcpy(&mq_handle, (osal_mq_handle_t *)user_arg, sizeof(mq_handle));
	time = 5000;

	/* wait for message send osal_test_mq2(). */
	R_OSAL_ThreadSleepForTimePeriod(1000);

	/* message receive */
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_MqReceiveForTimePeriod(mq_handle, time, &recv_data, sizeof(recv_data));
	TIME_MEASURE_STOP
	if ((osal_ret != OSAL_RETURN_OK) || (recv_data != 0x1111))
	{
		diff_msg = true;
	}
	osal_ret = R_OSAL_MqReceiveForTimePeriod(mq_handle, time, &recv_data, sizeof(recv_data));
	if ((osal_ret != OSAL_RETURN_OK) || (recv_data != 0x2222))
	{
		diff_msg = true;
	}

	if ((diff_msg == true) && (osal_ret == OSAL_RETURN_OK))
	{
		osal_ret = OSAL_RETURN_FAIL;
	}
	return (int64_t)osal_ret;
}

/* 03_013: send / receive between threads */
static void Test03_006_011()
{

	e_osal_return_t osal_ret    = OSAL_RETURN_OK;
	e_osal_return_t expect_data = OSAL_RETURN_OK;
	int  test_ret    = OSAL_TEST_OK;
	int64_t       thread_ret1 = 0;
	int64_t       thread_ret2 = 0;

	st_osal_thread_config_t config1;
	st_osal_thread_config_t config2;
	osal_thread_handle_t handle1;
	osal_thread_handle_t handle2;
	osal_thread_id_t     thread_id;

	st_osal_mq_config_t mq_config;
	osal_mq_handle_t mq_handle = NULL;
	osal_mq_id_t     mq_id;
	char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	/* 03_013_001 [success] */
	memset(&handle1,  0x00, sizeof(osal_thread_handle_t));
	memset(&handle2,  0x00, sizeof(osal_thread_handle_t));
	memset(&mq_handle, 0x00, sizeof(mq_handle));

	mq_config.max_num_msg = 2;
	mq_config.msg_size = 4;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MESSAGE, 
														   OSAL_TEST_MQRECEIVEFORTIMEPERIOD,
														   OSAL_TEST_ID011);

	mq_id = 0xf000;
	osal_ret = R_OSAL_MqCreate(&mq_config, mq_id, &mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqCreate() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEFORTIMEPERIOD,
				OSAL_TEST_ID011);
		return;
	}

	config1.func       = osal_test_mq1;
	config1.userarg    = (void *)&mq_handle;
	config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config1.task_name  = "sub_thread1";
	config1.stack_size = 1024;

	config2.func       = osal_test_mq2;
	config2.userarg    = (void *)&mq_handle;
	config2.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config2.task_name  = "sub_thread2";
	config2.stack_size = 1024;

	thread_id = 0xf000 + 0;
	osal_ret  = R_OSAL_ThreadCreate(&config1, thread_id, &handle1);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEFORTIMEPERIOD,
			OSAL_TEST_ID011);
		return;
	}

	thread_id = 0xf000 + 1;
	osal_ret  = R_OSAL_ThreadCreate(&config2, thread_id, &handle2);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEFORTIMEPERIOD,
			OSAL_TEST_ID011);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle1, &thread_ret1);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEFORTIMEPERIOD,
			OSAL_TEST_ID011);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle2, &thread_ret2);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEFORTIMEPERIOD,
			OSAL_TEST_ID011);
		return;
	}

	if ((thread_ret1 == expect_data) && (thread_ret2 == expect_data))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)thread_ret1));

	osal_test_result_output(OSAL_TEST_MESSAGE,
							OSAL_TEST_MQRECEIVEFORTIMEPERIOD,
							OSAL_TEST_ID011,
							test_ret, opt_info);

	osal_ret = R_OSAL_MqDelete(mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqDelete() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEFORTIMEPERIOD,
				OSAL_TEST_ID011);
		return;
	}
}

/* 03_001: R_OSAL_MqCreate */
static void osal_test_R_OSAL_MqCreate()
{
	st_osal_mq_config_t mq_config;
	osal_mq_handle_t    mq_handle;
	e_osal_return_t     osal_ret;
	/* ------------------------Initialize------------------------ */
	memset(&mq_handle, 0x00, sizeof(osal_mq_handle_t));
	memset(&mq_config, 0x00, sizeof(st_osal_mq_config_t));
	
	mq_config.max_num_msg = 1;
	mq_config.msg_size = 4;
	
	Test03_001_008(mq_config, 0x1000 + 0, mq_handle); // OSAL_MQ_IMR_MSG_00

    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
		return;
	}

	Test03_001_001(mq_config, 0x1000 + 0          , mq_handle);
	Test03_001_002(mq_config, 0x1000 + 0          , mq_handle);
//	Test03_001_003(mq_config, OSAL_MQ_IMP_DRV_TASK_DELTASK, mq_handle);
	Test03_001_004(mq_config, 0x1000 + 0          , NULL);
//	Test03_001_005(mq_config, 9999          , mq_handle);
//	Test03_001_006(mq_config, OSAL_MQ_RESOURCE_NUM        , mq_handle);
	Test03_001_007(mq_config, 0x1000 + 0          , mq_handle);
	//Test03_001_010(mq_config, 40                        , mq_handle);
	
	osal_ret = osal_deinit();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
		return;
	}
	
	Test03_001_009(mq_config, 0x1000 + 0, mq_handle);
}

/* 03_002: R_OSAL_MqDelete */
static void osal_test_R_OSAL_MqDelete()
{
	st_osal_mq_config_t mq_config;
	osal_mq_handle_t    mq_handle;
	e_osal_return_t     osal_ret;
/* ------------------------Initialize------------------------ */
	memset(&mq_handle, 0x00, sizeof(osal_mq_handle_t));
	memset(&mq_config, 0x00, sizeof(st_osal_mq_config_t));

	mq_config.max_num_msg = 1;
	mq_config.msg_size = 4;
	
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQDELETE, 0);
		return;
	}

	Test03_002_001(mq_config, 0x1000 + 5, mq_handle);
	Test03_002_002(mq_config, 0x1000 + 5, NULL);
	Test03_002_003(mq_config, 0x1000 + 5, mq_handle);
	Test03_002_004(mq_config, 0x1000 + 5, mq_handle);
	Test03_002_005(mq_config, 0x1000 + 5, mq_handle);
	//Test03_002_006();//using stubb
	Test03_002_007(mq_config, 0x1000 + 5, mq_handle);
	//Test03_002_008();//interrupt context
}

/* 03_003: R_OSAL_MqSendUntilTimeStamp */
static void osal_test_R_OSAL_MqSendUntilTimeStamp()
{
	st_osal_mq_config_t mq_config;
	osal_mq_handle_t    mq_handle;
	e_osal_return_t     osal_ret;
	st_osal_time_t      abs_time;
	uint32_t            msg = 0xFFFF;
	//UINT16              Result;
	/* ------------------------Initialize------------------------ */
	memset(&mq_handle, 0x00, sizeof(osal_mq_handle_t));
	memset(&g_mq_handle, 0x00, sizeof(osal_mq_handle_t));
	memset(&mq_config, 0x00, sizeof(st_osal_mq_config_t));

	mq_config.max_num_msg = 1;
	mq_config.msg_size = 4;
	
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP, 0);
		return;
	}
    osal_ret = R_OSAL_MqCreate(&mq_config, 0x1000 + 5, &mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqCreate() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
    osal_ret = R_OSAL_MqCreate(&mq_config, 0x1000 + 1, &g_mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqCreate() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
	/*Result = drvTMU_Initialize();
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP, 0);
		return;
	}*/

	get_timestamp(&abs_time, OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP, OSAL_TEST_ID001);
    update_timestamp(&abs_time, 1, 2000000);
	Test03_003_001(mq_handle, &abs_time, &msg, sizeof(msg));

	get_timestamp(&abs_time, OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP, OSAL_TEST_ID002);
    update_timestamp(&abs_time, 1, 1000000);
	Test03_003_002(mq_handle, &abs_time, &msg, sizeof(msg));

	get_timestamp(&abs_time, OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP, OSAL_TEST_ID003);
	abs_time.tv_sec  += 1;
	abs_time.tv_nsec = 999999999;
	Test03_003_003(mq_handle, &abs_time, &msg, sizeof(msg));

	get_timestamp(&abs_time, OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP, OSAL_TEST_ID004);
    update_timestamp(&abs_time, 1, 999999);
	Test03_003_004(mq_handle, &abs_time, &msg, sizeof(msg));

	get_timestamp(&abs_time, OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP, OSAL_TEST_ID005);
    update_timestamp(&abs_time, 1, 2000000);
	Test03_003_005(NULL, &abs_time, &msg, sizeof(msg));

	Test03_003_006(mq_handle, NULL, &msg, sizeof(msg));

	get_timestamp(&abs_time, OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP, OSAL_TEST_ID007);
    update_timestamp(&abs_time, 1, 2000000);
	Test03_003_007(mq_handle, &abs_time, NULL, sizeof(msg));

	get_timestamp(&abs_time, OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP, OSAL_TEST_ID008);
    update_timestamp(&abs_time, 1, 2000000);
	Test03_003_008(mq_handle, &abs_time, &msg, 0);

	get_timestamp(&abs_time, OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP, OSAL_TEST_ID009);
	abs_time.tv_sec  += 1;
	abs_time.tv_nsec += 1000000000;
	Test03_003_009(mq_handle, &abs_time, &msg, sizeof(msg));

	get_timestamp(&abs_time, OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP, OSAL_TEST_ID010);
    update_timestamp(&abs_time, 1, 2000000);

	Test03_003_010(mq_handle, &abs_time, &msg, 20);

	get_timestamp(&abs_time, OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP, OSAL_TEST_ID011);
    update_timestamp(&abs_time, 1, 2000000);
	Test03_003_011(mq_handle, &abs_time, &msg, sizeof(msg));

	/* Interrupt context test */
	//Test03_003_013();
	//Test03_003_014();

	osal_ret = R_OSAL_MqReset(mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqReset() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
	osal_ret = R_OSAL_MqReset(g_mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqReset() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
    osal_ret = R_OSAL_MqDelete(mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqDelete() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
    osal_ret = R_OSAL_MqDelete(g_mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqDelete() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
	/*Result = drvTMU_Deinitialize();
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP, 0);
		return;
	}*/
	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP, 0);
		return;
	}

//	get_timestamp(&abs_time, OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDUNTILTIMESTAMP, OSAL_TEST_ID012);
    update_timestamp(&abs_time, 1, 2000000);
	Test03_003_012(mq_handle, &abs_time, &msg, sizeof(msg));
}

/* 03_004: R_OSAL_MqSendForTimePeriod */
static void osal_test_R_OSAL_MqSendForTimePeriod()
{
	st_osal_mq_config_t mq_config;
	osal_mq_handle_t    mq_handle;
	e_osal_return_t     osal_ret;
	uint32_t            msg = 0xFFFF;
	//UINT16              Result;

	/* ------------------------Initialize------------------------ */
	memset(&mq_handle, 0x00, sizeof(osal_mq_handle_t));
	memset(&g_mq_handle, 0x00, sizeof(osal_mq_handle_t));
	memset(&mq_config, 0x00, sizeof(st_osal_mq_config_t));

	mq_config.max_num_msg = 1;
	mq_config.msg_size = 4;
	
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDFORTIMEPERIOD, 0);
		return;
	}
    osal_ret = R_OSAL_MqCreate(&mq_config, 0x1000 + 5, &mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqCreate() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDFORTIMEPERIOD, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
    osal_ret = R_OSAL_MqCreate(&mq_config, 0x1000 + 1, &g_mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqCreate() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDFORTIMEPERIOD, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
	/*Result = drvTMU_Initialize();
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDFORTIMEPERIOD, 0);
		return;
	}*/

	Test03_004_001(mq_handle, 10,   &msg,  sizeof(msg));
	Test03_004_002(mq_handle, 0,    &msg,  sizeof(msg));
	Test03_004_003(NULL,      10,   &msg,  sizeof(msg));
	Test03_004_004(mq_handle, -100, &msg,  sizeof(msg));
	Test03_004_005(mq_handle, 10,   NULL, sizeof(msg));
	Test03_004_006(mq_handle, 10,   &msg,  0);
	Test03_004_007(mq_handle, 10,   &msg,  20);
	Test03_004_008(mq_handle, 10,   &msg,  sizeof(msg));
	/* Interrupt context test */
	//Test03_004_010();
	//Test03_004_011();

	osal_ret = R_OSAL_MqReset(mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqReset() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDFORTIMEPERIOD, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
	osal_ret = R_OSAL_MqReset(g_mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqReset() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDFORTIMEPERIOD, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}

    osal_ret = R_OSAL_MqDelete(mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqDelete() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDFORTIMEPERIOD, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
    osal_ret = R_OSAL_MqDelete(g_mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqDelete() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDFORTIMEPERIOD, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
	/*Result = drvTMU_Deinitialize();
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDFORTIMEPERIOD, 0);
		return;
	}*/
	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQSENDFORTIMEPERIOD, 0);
		return;
	}

	Test03_004_009(mq_handle, 10, &msg, sizeof(msg));
}

/* 03_005: R_OSAL_MqReceiveUntilTimeStamp */
static void osal_test_R_OSAL_MqReceiveUntilTimeStamp()
{
	st_osal_mq_config_t mq_config;
	osal_mq_handle_t    mq_handle;
	e_osal_return_t     osal_ret;
	st_osal_time_t      abs_time;
	uint32_t            send_msg = 0xFFFF;
	uint32_t            receive_msg;
	//UINT16              Result;
	/* ------------------------Initialize------------------------ */
	memset(&mq_handle, 0x00, sizeof(osal_mq_handle_t));
	memset(&g_mq_handle, 0x00, sizeof(osal_mq_handle_t));
	memset(&mq_config, 0x00, sizeof(st_osal_mq_config_t));


	mq_config.max_num_msg = 1;
	mq_config.msg_size = 4;

    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEUNTILTIMESTAMP, 0);
		return;
	}
    osal_ret = R_OSAL_MqCreate(&mq_config, 0x1000 + 5, &mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqCreate() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEUNTILTIMESTAMP, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
    osal_ret = R_OSAL_MqCreate(&mq_config, 0x1000 + 1, &g_mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqCreate() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEUNTILTIMESTAMP, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
	/*Result = drvTMU_Initialize();
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEUNTILTIMESTAMP, 0);
		return;
	}*/

	send_msg_and_get_timestamp(mq_handle, &send_msg, &abs_time, OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEUNTILTIMESTAMP, OSAL_TEST_ID001);
    update_timestamp(&abs_time, 1, 2000000);
	Test03_005_001(mq_handle, &abs_time, &receive_msg, sizeof(receive_msg));

	send_msg_and_get_timestamp(mq_handle, &send_msg, &abs_time, OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEUNTILTIMESTAMP, OSAL_TEST_ID002);
    update_timestamp(&abs_time, 1, 1000000);
	Test03_005_002(mq_handle, &abs_time, &receive_msg, sizeof(receive_msg));

	send_msg_and_get_timestamp(mq_handle, &send_msg, &abs_time, OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEUNTILTIMESTAMP, OSAL_TEST_ID003);
	abs_time.tv_sec  += 1;
	abs_time.tv_nsec = 999999999;
	Test03_005_003(mq_handle, &abs_time, &receive_msg, sizeof(receive_msg));

	send_msg_and_get_timestamp(mq_handle, &send_msg, &abs_time, OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEUNTILTIMESTAMP, OSAL_TEST_ID004);
    update_timestamp(&abs_time, 1, 999999);
	Test03_005_004(mq_handle, &abs_time, &receive_msg, sizeof(receive_msg));

	send_msg_and_get_timestamp(mq_handle, &send_msg, &abs_time, OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEUNTILTIMESTAMP, OSAL_TEST_ID005);
	update_timestamp(&abs_time, 1, 2000000);
	Test03_005_005(NULL, &abs_time, &receive_msg, sizeof(receive_msg));

	Test03_005_006(mq_handle, NULL, &receive_msg, sizeof(receive_msg));

	send_msg_and_get_timestamp(mq_handle, &send_msg, &abs_time, OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEUNTILTIMESTAMP, OSAL_TEST_ID007);
	update_timestamp(&abs_time, 1, 2000000);
	Test03_005_007(mq_handle, &abs_time, NULL, sizeof(receive_msg));

	send_msg_and_get_timestamp(mq_handle, &send_msg, &abs_time, OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEUNTILTIMESTAMP, OSAL_TEST_ID008);
	abs_time.tv_sec  += 1;
	abs_time.tv_nsec += 1000000000;
	Test03_005_008(mq_handle, &abs_time, &receive_msg, sizeof(receive_msg));

	send_msg_and_get_timestamp(mq_handle, &send_msg, &abs_time, OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEUNTILTIMESTAMP, OSAL_TEST_ID009);
	update_timestamp(&abs_time, 1, 2000000);
	Test03_005_009(mq_handle, &abs_time, &receive_msg, 1);

	send_msg_and_get_timestamp(mq_handle, &send_msg, &abs_time, OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEUNTILTIMESTAMP, OSAL_TEST_ID010);
	update_timestamp(&abs_time, 1, 2000000);
	Test03_005_010(mq_handle, &abs_time, &receive_msg, sizeof(receive_msg));

	/* Interrupt context test */
	//Test03_005_012();
	//Test03_005_013();

	osal_ret = R_OSAL_MqReset(mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqReset() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEUNTILTIMESTAMP, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
	osal_ret = R_OSAL_MqReset(g_mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqReset() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEUNTILTIMESTAMP, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}

    osal_ret = R_OSAL_MqDelete(mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqCreate() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEUNTILTIMESTAMP, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
    osal_ret = R_OSAL_MqDelete(g_mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqDelete() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEUNTILTIMESTAMP, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
	/*Result = drvTMU_Deinitialize();
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEUNTILTIMESTAMP, 0);
		return;
	}*/
	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEUNTILTIMESTAMP, 0);
		return;
	}

//	get_timestamp(&abs_time, OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEUNTILTIMESTAMP, OSAL_TEST_ID011);
    update_timestamp(&abs_time, 1, 2000000);
	Test03_005_011(mq_handle, &abs_time, &receive_msg, sizeof(receive_msg));
}

/* 03_006: R_OSAL_MqReceiveForTimePeriod */
static void osal_test_R_OSAL_MqReceiveForTimePeriod()
{
	st_osal_mq_config_t mq_config;
	osal_mq_handle_t    mq_handle;
	e_osal_return_t     osal_ret;
	uint32_t            send_msg = 0xFFFF;
	uint32_t            receive_msg;
	//UINT16              Result;

	/* ------------------------Initialize------------------------ */
	memset(&mq_handle, 0x00, sizeof(osal_mq_handle_t));
	memset(&g_mq_handle, 0x00, sizeof(osal_mq_handle_t));
	memset(&mq_config, 0x00, sizeof(st_osal_mq_config_t));

	mq_config.max_num_msg = 1;
	mq_config.msg_size = 4;
	
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEFORTIMEPERIOD, 0);
		return;
	}
    osal_ret = R_OSAL_MqCreate(&mq_config, 0x1000 + 5, &mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqCreate() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEFORTIMEPERIOD, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
    osal_ret = R_OSAL_MqCreate(&mq_config, 0x1000 + 1, &g_mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqCreate() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEFORTIMEPERIOD, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
	/*Result = drvTMU_Initialize();
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEFORTIMEPERIOD, 0);
		return;
	}*/

	send_message(mq_handle, &send_msg, OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEFORTIMEPERIOD, OSAL_TEST_ID001);
	Test03_006_001(mq_handle, 10,   &receive_msg, sizeof(receive_msg));
	send_message(mq_handle, &send_msg, OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEFORTIMEPERIOD, OSAL_TEST_ID002);
	Test03_006_002(mq_handle, 0,    &receive_msg, sizeof(receive_msg));
	
	Test03_006_003(NULL,      10,   &receive_msg, sizeof(receive_msg));
	send_message(mq_handle, &send_msg, OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEFORTIMEPERIOD, OSAL_TEST_ID004);
	Test03_006_004(mq_handle, -100, &receive_msg, sizeof(receive_msg));
	send_message(mq_handle, &send_msg, OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEFORTIMEPERIOD, OSAL_TEST_ID005);
	Test03_006_005(mq_handle, 10,   NULL,         sizeof(receive_msg));
	send_message(mq_handle, &send_msg, OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEFORTIMEPERIOD, OSAL_TEST_ID006);
	Test03_006_006(mq_handle, 10,   &receive_msg, 1);
	send_message(mq_handle, &send_msg, OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEFORTIMEPERIOD, OSAL_TEST_ID007);
	Test03_006_007(mq_handle, 10,   &receive_msg, sizeof(receive_msg));
	/* Interrupt context test */
	//Test03_006_009();
	send_message(mq_handle, &send_msg, OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEFORTIMEPERIOD, OSAL_TEST_ID011);
	//Test03_006_010();
	/* Receive message FIFO */
	
	Test03_006_011();

	osal_ret = R_OSAL_MqReset(mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqReset() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEFORTIMEPERIOD, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
	osal_ret = R_OSAL_MqReset(g_mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqReset() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEFORTIMEPERIOD, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
	
    osal_ret = R_OSAL_MqDelete(mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqCreate() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEFORTIMEPERIOD, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
    osal_ret = R_OSAL_MqDelete(g_mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqDelete() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEFORTIMEPERIOD, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
	/*Result = drvTMU_Deinitialize();
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEFORTIMEPERIOD, 0);
		return;
	}*/
	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRECEIVEFORTIMEPERIOD, 0);
		return;
	}

	Test03_006_008(mq_handle, 10, &receive_msg, sizeof(receive_msg));
}

/* 03_007: R_OSAL_MqIsFull */
static void osal_test_R_OSAL_MqIsFull()
{
	st_osal_mq_config_t mq_config;
	osal_mq_handle_t    mq_handle;
	e_osal_return_t     osal_ret;
	bool                result;
	//UINT16              Result;

	/* ------------------------Initialize------------------------ */
	memset(&mq_handle, 0x00, sizeof(osal_mq_handle_t));
	memset(&g_mq_handle, 0x00, sizeof(osal_mq_handle_t));
	memset(&mq_config, 0x00, sizeof(st_osal_mq_config_t));
	
	mq_config.max_num_msg = 1;
	mq_config.msg_size = 4;

    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQISFULL, 0);
		return;
	}
    osal_ret = R_OSAL_MqCreate(&mq_config, 0x1000 + 5, &mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqCreate() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQISFULL, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
    osal_ret = R_OSAL_MqCreate(&mq_config, 0x1000 + 1, &g_mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqCreate() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQISFULL, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
	/*Result = drvTMU_Initialize();
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQISFULL, 0);
		return;
	}*/

	Test03_007_001(mq_handle, &result);
	Test03_007_002(mq_handle, &result);
	Test03_007_003(mq_handle, NULL);
	Test03_007_004(NULL,      &result);
	/* Interrupt context test */
	//Test03_007_006();
	//Test03_007_007();

	osal_ret = R_OSAL_MqReset(mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqReset() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQISFULL, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
	osal_ret = R_OSAL_MqReset(g_mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqReset() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQISFULL, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
	osal_ret = R_OSAL_MqDelete(mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqCreate() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQISFULL, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
    osal_ret = R_OSAL_MqDelete(g_mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqDelete() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQISFULL, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
	/*Result = drvTMU_Deinitialize();
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQISFULL, 0);
		return;
	}*/
	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQISFULL, 0);
		return;
	}
	
	Test03_007_005(mq_handle, &result);
}

/* 03_008: R_OSAL_MqIsEmpty */
static void osal_test_R_OSAL_MqIsEmpty()
{
	st_osal_mq_config_t mq_config;
	osal_mq_handle_t    mq_handle;
	e_osal_return_t     osal_ret;
	bool                result;
	//UINT16              Result;

	/* ------------------------Initialize------------------------ */
	memset(&mq_handle, 0x00, sizeof(osal_mq_handle_t));
	memset(&g_mq_handle, 0x00, sizeof(osal_mq_handle_t));
	memset(&mq_config, 0x00, sizeof(st_osal_mq_config_t));
	
	mq_config.max_num_msg = 1;
	mq_config.msg_size = 4;
	
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQISEMPTY, 0);
		return;
	}
    osal_ret = R_OSAL_MqCreate(&mq_config, 0x1000 + 5, &mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqCreate() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQISEMPTY, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
    osal_ret = R_OSAL_MqCreate(&mq_config, 0x1000 + 1, &g_mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqCreate() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQISEMPTY, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
	/*Result = drvTMU_Initialize();
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQISEMPTY, 0);
		return;
	}*/

	Test03_008_001(mq_handle, &result);
	Test03_008_002(mq_handle, &result);
	Test03_008_003(mq_handle, NULL);
	Test03_008_004(NULL,      &result);
	/* Interrupt context test */
	//Test03_008_006();
	//Test03_008_007();

	osal_ret = R_OSAL_MqReset(mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqReset() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQISEMPTY, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
	osal_ret = R_OSAL_MqReset(g_mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqReset() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQISEMPTY, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
	osal_ret = R_OSAL_MqDelete(mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqCreate() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQISEMPTY, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
    osal_ret = R_OSAL_MqDelete(g_mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqDelete() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQISEMPTY, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
	/*Result = drvTMU_Deinitialize();
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQISEMPTY, 0);
		return;
	}*/
	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQISEMPTY, 0);
		return;
	}
	
	Test03_008_005(mq_handle, &result);
}

/* 03_009: R_OSAL_MqGetConfig */
static void osal_test_R_OSAL_MqGetConfig()
{
	st_osal_mq_config_t mq_config;
	osal_mq_handle_t    mq_handle;
	e_osal_return_t     osal_ret;
	//UINT16              Result;

	/* ------------------------Initialize------------------------ */
	memset(&mq_handle, 0x00, sizeof(osal_mq_handle_t));
	memset(&g_mq_handle, 0x00, sizeof(osal_mq_handle_t));
	memset(&mq_config, 0x00, sizeof(st_osal_mq_config_t));
	
	mq_config.max_num_msg = 1;
	mq_config.msg_size = 4;
	
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQGETCONFIG, 0);
		return;
	}
    osal_ret = R_OSAL_MqCreate(&mq_config, 0x1000 + 5, &mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqCreate() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQGETCONFIG, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
    osal_ret = R_OSAL_MqCreate(&mq_config, 0x1000 + 1, &g_mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqCreate() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQGETCONFIG, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
	/*Result = drvTMU_Initialize();
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQGETCONFIG, 0);
		return;
	}*/

	Test03_009_001(mq_handle, &mq_config);
	Test03_009_002(mq_handle, NULL);
	Test03_009_003(NULL,      &mq_config);
	/* Interrupt context test */
	//Test03_009_005();

	osal_ret = R_OSAL_MqDelete(mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqCreate() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQGETCONFIG, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
    osal_ret = R_OSAL_MqDelete(g_mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqDelete() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQGETCONFIG, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
	/*Result = drvTMU_Deinitialize();
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQGETCONFIG, 0);
		return;
	}*/
	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQGETCONFIG, 0);
		return;
	}
	
	Test03_009_004(mq_handle, &mq_config);
}

/* 03_010: R_OSAL_MqReset */
static void osal_test_R_OSAL_MqReset()
{
	st_osal_mq_config_t mq_config;
	osal_mq_handle_t    mq_handle;
	e_osal_return_t     osal_ret;
	//UINT16              Result;

	/* ------------------------Initialize------------------------ */
	memset(&mq_handle, 0x00, sizeof(osal_mq_handle_t));
	memset(&g_mq_handle, 0x00, sizeof(osal_mq_handle_t));
	memset(&mq_config, 0x00, sizeof(st_osal_mq_config_t));
	
	mq_config.max_num_msg = 1;
	mq_config.msg_size = 4;
	
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRESET, 0);
		return;
	}
    osal_ret = R_OSAL_MqCreate(&mq_config, 0x1000 + 5, &mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqCreate() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRESET, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
    osal_ret = R_OSAL_MqCreate(&mq_config, 0x1000 + 1, &g_mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqCreate() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRESET, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
	/*Result = drvTMU_Initialize();
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRESET, 0);
		return;
	}*/

	Test03_010_001(mq_handle);
	Test03_010_002(NULL);
	/* Fault Injection test */
	//Test03_010_004(mq_handle);

	osal_ret = R_OSAL_MqDelete(mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqCreate() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRESET, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
    osal_ret = R_OSAL_MqDelete(g_mq_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_MqDelete() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRESET, 0);
		osal_ret = osal_deinit();
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQCREATE, 0);
			return;
		}
		return;
	}
	/*Result = drvTMU_Deinitialize();
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRESET, 0);
		return;
	}*/
	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQRESET, 0);
		return;
	}
	
	Test03_010_003(mq_handle);
}

/* 03_011: R_OSAL_MqInitializeMqConfigSt */
static void osal_test_R_OSAL_MqInitializeMqConfigSt()
{
	e_osal_return_t     osal_ret;

    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQINITIALIZEMQCONFIGST, 0);
		return;
	}

	Test03_011_001();
	Test03_011_002();


	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MESSAGE, OSAL_TEST_MQINITIALIZEMQCONFIGST, 0);
		return;
	}
}
