/**
 * @file clock_time_manager.c
 * @brief OSAL Clock&Time Manager evaluation file.
 * @author K.Takagi
 * @date 2019/11/12
 */

/* include */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rcar-xos/osal/r_osal.h"
#include "osal_test.h"
#include "osal_test_common.h"
#include "clock_time_manager.h"
//#include "rcar-xos/tmu/tmu.h"

/* prototype */
static void osal_test_R_OSAL_ClockTimeGetTimeStamp();
static void osal_test_R_OSAL_ClockTimeCalculateTimeDifference();

volatile int g_clocktime_cb_flag = 0;

void osal_test_clock_time_manager_display_menu()
{

	printf("*** OSAL Clock & Time Manager ***\n");
	printf("***  04_001: R_OSAL_ClockTimeGetTimeStamp                ***\n");
	printf("***  04_002: R_OSAL_ClockTimeCalculateTimeDifference     ***\n");
	printf("***  04_100: clock & time manager module all exec ***\n\n");

}

void osal_test_clock_time_manager_menu(int child)
{

    unsigned int test_number = (unsigned int)child;
	unsigned int test_count  = 1;
	unsigned int loop_count  = 0;

	/* Reset stub flags */
	//memset(&g_stub_flag, 0, sizeof(st_stub));
	
	/* ALL check */
	/* when select all */
	if (child == OSAL_TEST_ALL_EXEC)
	{
		test_number = OSAL_TEST_CLOCKTIMEGETTIMESTAMP;
		test_count  = OSAL_TEST_CLOCKTIME_MAX;
	}

	for (loop_count = 0; loop_count < test_count; loop_count++)
	{
		switch (test_number)
		{
			case OSAL_TEST_CLOCKTIMEGETTIMESTAMP:
				osal_test_R_OSAL_ClockTimeGetTimeStamp();
				break;
			case OSAL_TEST_CLOCKTIMECALCULATETIMEDIFFERENCE:
				osal_test_R_OSAL_ClockTimeCalculateTimeDifference();
				break;
			default:
				printf("That menu is not available. \n\n");
				break;
		}

		test_number++;
	}
}

e_osal_return_t osal_init_clocktime(void)
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

e_osal_return_t osal_deinit_clocktime(void)
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

/* ------------------------04_001_001 [success]------------------------ */
static void Test04_001_001()
{
	e_osal_return_t    		osal_ret;
	e_osal_clock_type_t 	clock_type;
	st_osal_time_t      	absolute_time;
	e_osal_return_t   		expect_data;
	char             		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 					test_ret = OSAL_TEST_OK;


	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_CLOCK_TIME, 
														   OSAL_TEST_CLOCKTIMEGETTIMESTAMP,
														   OSAL_TEST_ID001);

	memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
	clock_type  = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ClockTimeGetTimeStamp(clock_type, &absolute_time);
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

	osal_test_result_output(OSAL_TEST_CLOCK_TIME, 
							OSAL_TEST_CLOCKTIMEGETTIMESTAMP, 
							OSAL_TEST_ID001, 
							test_ret, opt_info);
}

/* ------------------------04_001_002 [success]------------------------ */
static void Test04_001_002()
{
	e_osal_return_t    		osal_ret;
	e_osal_clock_type_t 	clock_type;
	st_osal_time_t      	absolute_time;
	e_osal_return_t   		expect_data;
	char             		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 					test_ret = OSAL_TEST_OK;


	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_CLOCK_TIME, 
														   OSAL_TEST_CLOCKTIMEGETTIMESTAMP,
														   OSAL_TEST_ID002);

	memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
	clock_type  = OSAL_CLOCK_TYPE_HIGH_PRECISION;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ClockTimeGetTimeStamp(clock_type, &absolute_time);
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

	osal_test_result_output(OSAL_TEST_CLOCK_TIME, 
							OSAL_TEST_CLOCKTIMEGETTIMESTAMP, 
							OSAL_TEST_ID002, 
							test_ret, opt_info);
}

/* ------------------------04_001_003 [OSAL_RETURN_PAR]------------------------ */
static void Test04_001_003()
{
	e_osal_return_t    		osal_ret;
	e_osal_clock_type_t 	clock_type;
	st_osal_time_t      	absolute_time;
	e_osal_return_t   		expect_data;
	char             		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 					test_ret = OSAL_TEST_OK;


	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_CLOCK_TIME, 
														   OSAL_TEST_CLOCKTIMEGETTIMESTAMP,
														   OSAL_TEST_ID003);

	memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
	clock_type  = OSAL_CLOCK_TYPE_HIGH_RESOLUTION - 1;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ClockTimeGetTimeStamp(clock_type, &absolute_time);
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

	osal_test_result_output(OSAL_TEST_CLOCK_TIME, 
							OSAL_TEST_CLOCKTIMEGETTIMESTAMP, 
							OSAL_TEST_ID003, 
							test_ret, opt_info);
}

/* ------------------------04_001_004 [OSAL_RETURN_PAR]------------------------ */
static void Test04_001_004()
{
	e_osal_return_t    		osal_ret;
	e_osal_clock_type_t 	clock_type;
	st_osal_time_t      	absolute_time;
	e_osal_return_t   		expect_data;
	char             		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 					test_ret = OSAL_TEST_OK;


	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_CLOCK_TIME, 
														   OSAL_TEST_CLOCKTIMEGETTIMESTAMP,
														   OSAL_TEST_ID004);

	memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
	clock_type  = OSAL_CLOCK_TYPE_HIGH_PRECISION + 1;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ClockTimeGetTimeStamp(clock_type, &absolute_time);
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

	osal_test_result_output(OSAL_TEST_CLOCK_TIME, 
							OSAL_TEST_CLOCKTIMEGETTIMESTAMP, 
							OSAL_TEST_ID004, 
							test_ret, opt_info);
}

/* ------------------------04_001_005 [OSAL_RETURN_PAR]------------------------ */
static void Test04_001_005()
{
	e_osal_return_t    		osal_ret;
	e_osal_clock_type_t 	clock_type;
	e_osal_return_t   		expect_data;
	char             		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 					test_ret = OSAL_TEST_OK;


	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_CLOCK_TIME, 
														   OSAL_TEST_CLOCKTIMEGETTIMESTAMP,
														   OSAL_TEST_ID005);

	clock_type  = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ClockTimeGetTimeStamp(clock_type, NULL);
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

	osal_test_result_output(OSAL_TEST_CLOCK_TIME, 
							OSAL_TEST_CLOCKTIMEGETTIMESTAMP, 
							OSAL_TEST_ID005, 
							test_ret, opt_info);
}

#if 0
void Test04_001_006_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType,
						   drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	e_osal_return_t    		osal_ret;
	e_osal_clock_type_t 	clock_type;
	st_osal_time_t      	absolute_time;
	e_osal_return_t   		expect_data;
	char             		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 					test_ret = OSAL_TEST_OK;


	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_CLOCK_TIME, 
														   OSAL_TEST_CLOCKTIMEGETTIMESTAMP,
														   OSAL_TEST_ID006);

	memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
	clock_type  = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ClockTimeGetTimeStamp(clock_type, &absolute_time);
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

	osal_test_result_output(OSAL_TEST_CLOCK_TIME, 
							OSAL_TEST_CLOCKTIMEGETTIMESTAMP, 
							OSAL_TEST_ID006, 
							test_ret, opt_info);
	g_clocktime_cb_flag = 1;
}

/* ------------------------04_001_006 [ISR: success]------------------------ */
static void Test04_001_006()
{
	UINT16          Result;
	drvTMU_Time_t   Period;
	UINT8           Channel;

	Channel = 0U; /* Channel number */
	Period.Sec = 1; /* 1 second */
	Period.Usec = 500000; /* 500000 usec */

	Result = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test04_001_006_CallbackFunc, TMU_EDGE_RISE);
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_CLOCK_TIME, OSAL_TEST_CLOCKTIMEGETTIMESTAMP, OSAL_TEST_ID006);
		return;
	}

	while (g_clocktime_cb_flag == 0);
	g_clocktime_cb_flag = 0;
}
#endif

#if 0
void Test04_001_007_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType,
						   drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	e_osal_return_t    		osal_ret;
	e_osal_clock_type_t 	clock_type;
	st_osal_time_t      	absolute_time;
	e_osal_return_t   		expect_data;
	char             		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 					test_ret = OSAL_TEST_OK;


	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_CLOCK_TIME, 
														   OSAL_TEST_CLOCKTIMEGETTIMESTAMP,
														   OSAL_TEST_ID007);

	memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
	clock_type  = OSAL_CLOCK_TYPE_HIGH_PRECISION;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ClockTimeGetTimeStamp(clock_type, &absolute_time);
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

	osal_test_result_output(OSAL_TEST_CLOCK_TIME, 
							OSAL_TEST_CLOCKTIMEGETTIMESTAMP, 
							OSAL_TEST_ID007, 
							test_ret, opt_info);
	g_clocktime_cb_flag = 1;
}
/* ------------------------04_001_007 [ISR: success]------------------------ */
static void Test04_001_007()
{
	UINT16          Result;
	drvTMU_Time_t   Period;
	UINT8           Channel;

	Channel = 0U; /* Channel number */
	Period.Sec = 1; /* 1 second */
	Period.Usec = 500000; /* 500000 usec */

	Result = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test04_001_007_CallbackFunc, TMU_EDGE_RISE);
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_CLOCK_TIME, OSAL_TEST_CLOCKTIMEGETTIMESTAMP, OSAL_TEST_ID007);
		return;
	}

	while (g_clocktime_cb_flag == 0);
	g_clocktime_cb_flag = 0;
}
#endif
/* ------------------------04_002_001 [Success]------------------------ */
static void Test04_002_001()
{
	e_osal_return_t    		osal_ret;
	st_osal_time_t      	time2;
	st_osal_time_t      	time1;
	osal_nano_sec_t   		different_time;
	e_osal_return_t    		expect_data;
	e_osal_clock_type_t 	clock_type;
	char             		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 					test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_CLOCK_TIME, 
														   OSAL_TEST_CLOCKTIMECALCULATETIMEDIFFERENCE,
														   OSAL_TEST_ID001);

	memset(&time2, 0x00, sizeof(st_osal_time_t));
	memset(&time1, 0x00, sizeof(st_osal_time_t));
	memset(&different_time, 0x00, sizeof(osal_nano_sec_t));
	clock_type  = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;

	osal_ret = R_OSAL_ClockTimeGetTimeStamp(clock_type, &time1);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
				OSAL_TEST_CLOCK_TIME, OSAL_TEST_CLOCKTIMECALCULATETIMEDIFFERENCE, OSAL_TEST_ID001);
		return;
	}

	/* add 1sec to clarify the difference. */
	time2.tv_sec = time1.tv_sec + 1;
	time2.tv_nsec = time1.tv_nsec;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ClockTimeCalculateTimeDifference(&time2, &time1, &different_time);
	TIME_MEASURE_STOP
	if ((osal_ret == expect_data)&&(different_time == 1000000000))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_CLOCK_TIME, 
							OSAL_TEST_CLOCKTIMECALCULATETIMEDIFFERENCE,
							OSAL_TEST_ID001,
							test_ret, opt_info);
}

/* ------------------------04_002_002 [OSAL_RETURN_FAIL]------------------------ */
static void Test04_002_002()
{
	e_osal_return_t    		osal_ret;
	st_osal_time_t      	time2;
	st_osal_time_t      	time1;
	osal_nano_sec_t   		different_time;
	e_osal_return_t    		expect_data;
	e_osal_clock_type_t 	clock_type;
	char             		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 					test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_CLOCK_TIME, 
														   OSAL_TEST_CLOCKTIMECALCULATETIMEDIFFERENCE,
														   OSAL_TEST_ID002);

	memset(&time2, 0x00, sizeof(st_osal_time_t));
	memset(&time1, 0x00, sizeof(st_osal_time_t));
	memset(&different_time, 0x00, sizeof(osal_nano_sec_t));
	clock_type  = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;

	osal_ret = R_OSAL_ClockTimeGetTimeStamp(clock_type, &time2);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
				OSAL_TEST_CLOCK_TIME, OSAL_TEST_CLOCKTIMECALCULATETIMEDIFFERENCE, OSAL_TEST_ID002);
		return;
	}

	/* add 1sec to clarify the difference. */
	time1.tv_sec = time2.tv_sec + 1;
	time1.tv_nsec = time2.tv_nsec;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ClockTimeCalculateTimeDifference(&time2, &time1, &different_time);
	TIME_MEASURE_STOP
	if ((osal_ret == expect_data)&&(different_time == (int64_t)-1000000000))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_CLOCK_TIME, 
							OSAL_TEST_CLOCKTIMECALCULATETIMEDIFFERENCE,
							OSAL_TEST_ID002,
							test_ret, opt_info);
}

/* ------------------------04_002_003 [OSAL_RETURN_PAR]------------------------ */
static void Test04_002_003()
{
	e_osal_return_t    		osal_ret;
	st_osal_time_t      	time;
	osal_nano_sec_t   		different_time;
	e_osal_return_t    		expect_data;
	e_osal_clock_type_t 	clock_type;
	char             		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 					test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_CLOCK_TIME, 
														   OSAL_TEST_CLOCKTIMECALCULATETIMEDIFFERENCE,
														   OSAL_TEST_ID003);

	memset(&time, 0x00, sizeof(st_osal_time_t));
	memset(&different_time, 0x00, sizeof(osal_nano_sec_t));
	clock_type  = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;

	osal_ret = R_OSAL_ClockTimeGetTimeStamp(clock_type, &time);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
				OSAL_TEST_CLOCK_TIME, OSAL_TEST_CLOCKTIMECALCULATETIMEDIFFERENCE, OSAL_TEST_ID003);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ClockTimeCalculateTimeDifference(NULL, &time, &different_time);
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

	osal_test_result_output(OSAL_TEST_CLOCK_TIME, 
							OSAL_TEST_CLOCKTIMECALCULATETIMEDIFFERENCE,
							OSAL_TEST_ID003,
							test_ret, opt_info);
}

/* ------------------------04_002_004 [OSAL_RETURN_PAR]------------------------ */
static void Test04_002_004()
{
	e_osal_return_t    		osal_ret;
	st_osal_time_t      	time;
	osal_nano_sec_t   		different_time;
	e_osal_return_t    		expect_data;
	e_osal_clock_type_t 	clock_type;
	char             		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 					test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_CLOCK_TIME, 
														   OSAL_TEST_CLOCKTIMECALCULATETIMEDIFFERENCE,
														   OSAL_TEST_ID004);

	memset(&time, 0x00, sizeof(st_osal_time_t));
	memset(&different_time, 0x00, sizeof(osal_nano_sec_t));
	clock_type  = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;

	osal_ret = R_OSAL_ClockTimeGetTimeStamp(clock_type, &time);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
				OSAL_TEST_CLOCK_TIME, OSAL_TEST_CLOCKTIMECALCULATETIMEDIFFERENCE, OSAL_TEST_ID004);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ClockTimeCalculateTimeDifference(&time, NULL, &different_time);
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

	osal_test_result_output(OSAL_TEST_CLOCK_TIME, 
							OSAL_TEST_CLOCKTIMECALCULATETIMEDIFFERENCE,
							OSAL_TEST_ID004,
							test_ret, opt_info);
}

/* ------------------------04_002_005 [OSAL_RETURN_PAR]------------------------ */
static void Test04_002_005()
{
	e_osal_return_t    		osal_ret;
	st_osal_time_t      	time2;
	st_osal_time_t      	time1;
	e_osal_return_t    		expect_data;
	e_osal_clock_type_t 	clock_type;
	char             		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 					test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_CLOCK_TIME, 
														   OSAL_TEST_CLOCKTIMECALCULATETIMEDIFFERENCE,
														   OSAL_TEST_ID005);

	memset(&time2, 0x00, sizeof(st_osal_time_t));
	memset(&time1, 0x00, sizeof(st_osal_time_t));
	clock_type  = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;

	osal_ret = R_OSAL_ClockTimeGetTimeStamp(clock_type, &time1);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
				OSAL_TEST_CLOCK_TIME, OSAL_TEST_CLOCKTIMECALCULATETIMEDIFFERENCE, OSAL_TEST_ID005);
		return;
	}

	/* add 1sec to clarify the difference. */
	time2.tv_sec = time1.tv_sec + 1;
	time2.tv_nsec = time1.tv_nsec;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ClockTimeCalculateTimeDifference(&time2, &time1, NULL);
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

	osal_test_result_output(OSAL_TEST_CLOCK_TIME, 
							OSAL_TEST_CLOCKTIMECALCULATETIMEDIFFERENCE,
							OSAL_TEST_ID005,
							test_ret, opt_info);
}

#if 0
void Test04_002_006_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType,
						   drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	e_osal_return_t    		osal_ret;
	st_osal_time_t      	time2;
	st_osal_time_t      	time1;
	osal_nano_sec_t   		different_time;
	e_osal_return_t    		expect_data;
	e_osal_clock_type_t 	clock_type;
	char             		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 					test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_CLOCK_TIME, 
														   OSAL_TEST_CLOCKTIMECALCULATETIMEDIFFERENCE,
														   OSAL_TEST_ID006);

	memset(&time2, 0x00, sizeof(st_osal_time_t));
	memset(&time1, 0x00, sizeof(st_osal_time_t));
	memset(&different_time, 0x00, sizeof(osal_nano_sec_t));
	clock_type  = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;

	osal_ret = R_OSAL_ClockTimeGetTimeStamp(clock_type, &time1);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
				OSAL_TEST_CLOCK_TIME, OSAL_TEST_CLOCKTIMECALCULATETIMEDIFFERENCE, OSAL_TEST_ID006);
		return;
	}

	/* add 1sec to clarify the difference. */
	time2.tv_sec = time1.tv_sec + 1;
	time2.tv_nsec = time1.tv_nsec;


	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ClockTimeCalculateTimeDifference(&time2, &time1, &different_time);
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

	osal_test_result_output(OSAL_TEST_CLOCK_TIME, 
							OSAL_TEST_CLOCKTIMECALCULATETIMEDIFFERENCE,
							OSAL_TEST_ID006,
							test_ret, opt_info);
	g_clocktime_cb_flag = 1;
}
/* ------------------------04_002_006 [ISR: Success]------------------------ */
static void Test04_002_006()
{
	UINT16          Result;
	drvTMU_Time_t   Period;
	UINT8           Channel;

	Channel = 0U; /* Channel number */
	Period.Sec = 1; /* 1 second */
	Period.Usec = 500000; /* 500000 usec */

	Result = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test04_002_006_CallbackFunc, TMU_EDGE_RISE);
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_CLOCK_TIME, OSAL_TEST_CLOCKTIMECALCULATETIMEDIFFERENCE, OSAL_TEST_ID006);
		return;
	}

	while (g_clocktime_cb_flag == 0);
	g_clocktime_cb_flag = 0;
}
#endif

static void Test04_002_007()
{
	e_osal_return_t    		osal_ret;
	st_osal_time_t      	time2;
	st_osal_time_t      	time1;
	osal_nano_sec_t   		different_time;
	e_osal_return_t    		expect_data;
	e_osal_clock_type_t 	clock_type;
	char             		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 					test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_CLOCK_TIME, 
														   OSAL_TEST_CLOCKTIMECALCULATETIMEDIFFERENCE,
														   OSAL_TEST_ID007);

	memset(&time2, 0x00, sizeof(st_osal_time_t));
	memset(&time1, 0x00, sizeof(st_osal_time_t));
	memset(&different_time, 0x00, sizeof(osal_nano_sec_t));
	clock_type  = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;

	osal_ret = R_OSAL_ClockTimeGetTimeStamp(clock_type, &time1);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
				OSAL_TEST_CLOCK_TIME, OSAL_TEST_CLOCKTIMECALCULATETIMEDIFFERENCE, OSAL_TEST_ID007);
		return;
	}

	/* add 1sec to clarify the difference. */
	time2.tv_sec = time1.tv_sec;
	time2.tv_nsec = time1.tv_nsec;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ClockTimeCalculateTimeDifference(&time2, &time1, &different_time);
	TIME_MEASURE_STOP
	
	if ((osal_ret == expect_data)&&(different_time == 0))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_CLOCK_TIME, 
							OSAL_TEST_CLOCKTIMECALCULATETIMEDIFFERENCE,
							OSAL_TEST_ID007,
							test_ret, opt_info);
}

/* 04_001: R_OSAL_ClockTimeGetTimeStamp */
static void osal_test_R_OSAL_ClockTimeGetTimeStamp()
{
	e_osal_return_t    osal_ret;
	//UINT16	Result;

	osal_ret = osal_init_clocktime();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_CLOCK_TIME, OSAL_TEST_CLOCKTIMEGETTIMESTAMP, 0);
		return;
	}
	/*Result = drvTMU_Initialize();
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
				OSAL_TEST_CLOCK_TIME, OSAL_TEST_CLOCKTIMEGETTIMESTAMP, 0);
		return;
	}*/

	Test04_001_001();
	Test04_001_002();
	Test04_001_003();
	Test04_001_004();
	Test04_001_005();
	//Test04_001_006(); Interrupt Context
	//Test04_001_007();

	/*Result = drvTMU_Deinitialize();
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
				OSAL_TEST_CLOCK_TIME, OSAL_TEST_CLOCKTIMEGETTIMESTAMP, 0);
		return;
	}*/
	osal_ret = osal_deinit_clocktime();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_CLOCK_TIME, OSAL_TEST_CLOCKTIMEGETTIMESTAMP, 0);
		return;
	}

}

/* 04_002: R_OSAL_ClockTimeCalculateTimeDifference */
static void osal_test_R_OSAL_ClockTimeCalculateTimeDifference()
{
	e_osal_return_t    		osal_ret;
	//UINT16	Result;

	osal_ret = osal_init_clocktime();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_CLOCK_TIME, OSAL_TEST_CLOCKTIMECALCULATETIMEDIFFERENCE, 0);
		return;
	}
	/*Result = drvTMU_Initialize();
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
				OSAL_TEST_CLOCK_TIME, OSAL_TEST_CLOCKTIMECALCULATETIMEDIFFERENCE, 0);
		return;
	}*/

	Test04_002_001();
	Test04_002_002();
	Test04_002_003();
	Test04_002_004();
	Test04_002_005();
	//Test04_002_006(); Interrupt Context
	Test04_002_007();

	/*Result = drvTMU_Deinitialize();
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
				OSAL_TEST_CLOCK_TIME, OSAL_TEST_CLOCKTIMECALCULATETIMEDIFFERENCE, 0);
		return;
	}*/
	osal_ret = osal_deinit_clocktime();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_CLOCK_TIME, OSAL_TEST_CLOCKTIMECALCULATETIMEDIFFERENCE, 0);
		return;
	}
}
