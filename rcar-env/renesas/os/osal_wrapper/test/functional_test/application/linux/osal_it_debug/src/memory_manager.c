/**
 * @file memory_manager.c
 * @brief OSAL Memory Manager evaluation file.
 */

/* include */
#include "osal_test.h"
#include "osal_test_common.h"
#include "memory_manager.h"

/* prototype */
static void osal_test_R_OSAL_MmngrDebugMmngrPrint();
static void osal_test_R_OSAL_MmngrDebugBufferPrint();

/* global */


void osal_test_memory_manager_display_menu()
{
	printf("*** OSAL Memory Manager ***\n");
	printf("***  08_018: R_OSAL_MmngrDebugMmngrPrint                ***\n");
	printf("***  08_019: R_OSAL_MmngrDebugBufferPrint               ***\n");
	printf("***  08_100: memory manager module all exec             ***\n\n");
}

void osal_test_memory_manager_menu(int child)
{
	int test_number = child;
	unsigned int test_count  = 1;
	unsigned int loop_count  = 0;

	if (child == OSAL_TEST_ALL_EXEC)
	{
		test_number = OSAL_TEST_MMNGROPEN;
		test_count  = OSAL_TEST_MEMORY_MAX;
	}

	for (loop_count = 0; loop_count < test_count; loop_count++)
	{
		switch (test_number)
		{
			case OSAL_TEST_MMNGROPEN :
			case OSAL_TEST_MMNGROPENFROMREGIONIDX :
			case OSAL_TEST_MMNGRCLOSE :
			case OSAL_TEST_MMNGRALLOC :
			case OSAL_TEST_MMNGRDEALLOC :
			case OSAL_TEST_MMNGRGETCONFIG :
			case OSAL_TEST_MMNGRGETOSALMAXCONFIG :
			case OSAL_TEST_MMNGRGETOSALMAXCONFIGFROMREGIONIDX :
			case OSAL_TEST_MMNGRINITIALIZEMMNGRCONFIGST :
			case OSAL_TEST_MMNGRREGISTERMONITOR :
			case OSAL_TEST_MMNGRGETCPUPTR :
			case OSAL_TEST_MMNGRGETCONSTCPUPTR :
			case OSAL_TEST_MMNGRFLUSH :
			case OSAL_TEST_MMNGRINVALIDATE :
			case OSAL_TEST_MMNGRGETHWADDR :
			case OSAL_TEST_MMNGRGETSIZE :
			case OSAL_TEST_MMNGRCHECKBUFFER :
			case OSAL_TEST_MMNGRINITIALIZEMEMORYMANAGEROBJ :
			case OSAL_TEST_MMNGRINITIALIZEMEMORYBUFFEROBJ :
				break;
			case OSAL_TEST_MMNGRDEBUGMMNGRPRINT :
				osal_test_R_OSAL_MmngrDebugMmngrPrint();
				break;
			case OSAL_TEST_MMNGRDEBUGBUFFERPRINT :
				osal_test_R_OSAL_MmngrDebugBufferPrint();
				break;
			default:
				printf("That menu is not available.\n\n");
				break;
		}
		test_number++;
	}
}

/* ------------------------08_018_001 [OSAL_RETURN_IO]------------------------ */
static void Test08_018_001()
{
	e_osal_return_t					osal_ret;
	e_osal_return_t					expect_data;
	char							opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int								test_ret = OSAL_TEST_OK;

	osal_memory_manager_handle_t	mmngr_hndl;
	st_osal_mmngr_config_t			mmngr_config;
	FILE*							check_mmngr_r;

	expect_data = (e_osal_return_t)osal_test_get_expectdata(OSAL_TEST_MEMORY,
															OSAL_TEST_MMNGRDEBUGMMNGRPRINT,
															OSAL_TEST_ID001);

	osal_ret = R_OSAL_MmngrGetOsalMaxConfig(&mmngr_config);
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_MmngrGetOsalMaxConfig() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRDEBUGMMNGRPRINT, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}

	osal_ret = R_OSAL_MmngrOpen(&mmngr_config, &mmngr_hndl);
	if (OSAL_RETURN_OK != osal_ret)
	{
		printf("osal_ret=%d\n", osal_ret);
		OSAL_TEST_ERR("R_OSAL_MmngrOpen() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRDEBUGMMNGRPRINT, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}

	check_mmngr_r = fopen("check_mmngr.txt", "ra");
	if (NULL == check_mmngr_r)
	{
		OSAL_TEST_ERR("fopen() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRDEBUGBUFFERPRINT, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_MmngrDebugMmngrPrint(mmngr_hndl, check_mmngr_r);
	TIME_MEASURE_STOP
	if (expect_data != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_MmngrDebugMmngrPrint() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRDEBUGMMNGRPRINT, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	fclose(check_mmngr_r);

	osal_ret = R_OSAL_MmngrClose(mmngr_hndl);
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_MmngrClose() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRDEBUGMMNGRPRINT, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}

	osal_test_result_output(OSAL_TEST_MEMORY,
							OSAL_TEST_MMNGRDEBUGMMNGRPRINT,
							OSAL_TEST_ID001,
							test_ret, opt_info);
	return;
}

/* ------------------------08_019_001 [OSAL_RETURN_IO]------------------------ */
static void Test08_019_001()
{
	e_osal_return_t					osal_ret;
	e_osal_return_t					expect_data;
	char							opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int								test_ret = OSAL_TEST_OK;

	osal_memory_manager_handle_t	mmngr_hndl;
	st_osal_mmngr_config_t			mmngr_config;
	osal_memory_buffer_handle_t		mmngr_buf;
	FILE*							check_buffer_r;

	expect_data = (e_osal_return_t)osal_test_get_expectdata(OSAL_TEST_MEMORY,
															OSAL_TEST_MMNGRDEBUGBUFFERPRINT,
															OSAL_TEST_ID001);

	osal_ret = R_OSAL_MmngrGetOsalMaxConfig(&mmngr_config);
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_MmngrGetOsalMaxConfig() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRDEBUGBUFFERPRINT, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}

	osal_ret = R_OSAL_MmngrOpen(&mmngr_config, &mmngr_hndl);
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_MmngrOpen() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRDEBUGBUFFERPRINT, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}

	osal_ret = R_OSAL_MmngrAlloc(mmngr_hndl, 25 * 1024 * 1024, 64, &mmngr_buf);
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_MmngrAlloc() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRDEBUGBUFFERPRINT, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}

	check_buffer_r = fopen("check_buffer.txt", "ra");
	if (NULL == check_buffer_r)
	{
		OSAL_TEST_ERR("fopen() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRDEBUGBUFFERPRINT, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_MmngrDebugBufferPrint(mmngr_buf, check_buffer_r);
	TIME_MEASURE_STOP
	if (expect_data != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_MmngrDebugBufferPrint() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRDEBUGBUFFERPRINT, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	fclose(check_buffer_r);

	osal_ret = R_OSAL_MmngrDealloc(mmngr_hndl, mmngr_buf);
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_MmngrDealloc() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRDEBUGBUFFERPRINT, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}

	osal_ret = R_OSAL_MmngrClose(mmngr_hndl);
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_MmngrClose() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRDEBUGBUFFERPRINT, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}

	osal_test_result_output(OSAL_TEST_MEMORY,
							OSAL_TEST_MMNGRDEBUGBUFFERPRINT,
							OSAL_TEST_ID001,
							test_ret, opt_info);
	return;
}

/* 08_016: R_OSAL_MmngrDebugMmngrPrint */
static void osal_test_R_OSAL_MmngrDebugMmngrPrint()
{
	e_osal_return_t	osal_ret;
	FILE*			touch_file;

	osal_ret = R_OSAL_Initialize();
	if(OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRDEBUGMMNGRPRINT, 0);
		return;
	}

	touch_file = fopen("check_mmngr.txt", "a");
	if (NULL == touch_file)
	{
		OSAL_TEST_ERR("fopen() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRDEBUGBUFFERPRINT, 0);
		return;
	}
	fclose(touch_file);

	Test08_018_001();

	osal_ret = R_OSAL_Deinitialize();
	if(OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRDEBUGMMNGRPRINT, 0);
		return;
	}
	return;
}

/* 08_017: R_OSAL_MmngrDebugBufferPrint */
static void osal_test_R_OSAL_MmngrDebugBufferPrint()
{
	e_osal_return_t	osal_ret;
	FILE*			touch_file;

	osal_ret = R_OSAL_Initialize();
	if(OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRDEBUGMMNGRPRINT, 0);
		return;
	}

	touch_file = fopen("check_buffer.txt", "a");
	if (NULL == touch_file)
	{
		OSAL_TEST_ERR("fopen() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRDEBUGBUFFERPRINT, 0);
		return;
	}
	fclose(touch_file);

	Test08_019_001();

	osal_ret = R_OSAL_Deinitialize();
	if(OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRDEBUGMMNGRPRINT, 0);
		return;
	}
	return;
}
