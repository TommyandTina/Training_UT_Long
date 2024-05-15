/**
 * @file memory_manager.c
 * @brief OSAL Memory Manager evaluation file.
 */

/* include */
#include "osal_test.h"
#include "osal_test_common.h"
#include "memory_manager.h"
#include "rcar-xos/osal_configuration/target/common/r_osal_mem_cfg_info.h"

/* prototype */
static void osal_test_R_OSAL_MmngrGetConfig();
static void osal_test_R_OSAL_MmngrGetOsalMaxConfig();
static void osal_test_R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx();
static void osal_test_R_OSAL_MmngrGetCpuPtr();
static void osal_test_R_OSAL_MmngrFlush();
static void osal_test_R_OSAL_MmngrInvalidate();
static void osal_test_R_OSAL_MmngrGetHwAddr();

/* global */


void osal_test_memory_manager_display_menu()
{
	printf("*** OSAL Memory Manager ***\n");
	printf("***  08_001: R_OSAL_MmngrOpen                           ***\n");
	printf("***  08_002: R_OSAL_MmngrOpenFromRegionIdx              ***\n");
	printf("***  08_003: R_OSAL_MmngrClose                          ***\n");
	printf("***  08_004: R_OSAL_MmngrAlloc                          ***\n");
	printf("***  08_005: R_OSAL_MmngrDealloc                        ***\n");
	printf("***  08_006: R_OSAL_MmngrGetConfig                      ***\n");
	printf("***  08_007: R_OSAL_MmngrGetOsalMaxConfig               ***\n");
	printf("***  08_008: R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx  ***\n");
	printf("***  08_009: R_OSAL_MmngrInitializeMmngrConfigSt        ***\n");
	printf("***  08_010: R_OSAL_MmngrRegisterMonitor                ***\n");
	printf("***  08_011: R_OSAL_MmngrGetCpuPtr                      ***\n");
	printf("***  08_012: R_OSAL_MmngrGetConstCpuPtr                 ***\n");
	printf("***  08_013: R_OSAL_MmngrFlush                          ***\n");
	printf("***  08_014: R_OSAL_MmngrInvalidate                     ***\n");
	printf("***  08_015: R_OSAL_MmngrGetHwAddr                      ***\n");
	printf("***  08_016: R_OSAL_MmngrGetSize                        ***\n");
	printf("***  08_017: R_OSAL_MmngrCheckBuffer                    ***\n");
	printf("***  08_018: R_OSAL_MmngrDebugMmngrPrint                ***\n");
	printf("***  08_019: R_OSAL_MmngrDebugBufferPrint               ***\n");
	printf("***  08_020: R_OSAL_MmngrInitializeMemoryManagerObj     ***\n");
	printf("***  08_021: R_OSAL_MmngrInitializeMemoryBufferObj      ***\n");
	printf("***  08_100: memory manager module all exec             ***\n\n");
	return;
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
			case OSAL_TEST_MMNGRGETCONFIG :
				osal_test_R_OSAL_MmngrGetConfig();
				break;

			case OSAL_TEST_MMNGRGETOSALMAXCONFIG :
				osal_test_R_OSAL_MmngrGetOsalMaxConfig();
				break;

			case OSAL_TEST_MMNGRGETOSALMAXCONFIGFROMREGIONIDX :
				osal_test_R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx();
				break;

			case OSAL_TEST_MMNGRGETCPUPTR :
				osal_test_R_OSAL_MmngrGetCpuPtr();
				break;

			case OSAL_TEST_MMNGRFLUSH :
				osal_test_R_OSAL_MmngrFlush();
				break;

			case OSAL_TEST_MMNGRINVALIDATE :
				osal_test_R_OSAL_MmngrInvalidate();
				break;

			case OSAL_TEST_MMNGRGETHWADDR :
				osal_test_R_OSAL_MmngrGetHwAddr();
				break;

			case OSAL_TEST_MMNGROPEN :
			case OSAL_TEST_MMNGROPENFROMREGIONIDX :
			case OSAL_TEST_MMNGRCLOSE :
			case OSAL_TEST_MMNGRALLOC :
			case OSAL_TEST_MMNGRDEALLOC :
			case OSAL_TEST_MMNGRINITIALIZEMMNGRCONFIGST :
			case OSAL_TEST_MMNGRREGISTERMONITOR :
			case OSAL_TEST_MMNGRGETCONSTCPUPTR :
			case OSAL_TEST_MMNGRGETSIZE :
			case OSAL_TEST_MMNGRCHECKBUFFER :
			case OSAL_TEST_MMNGRINITIALIZEMEMORYMANAGEROBJ :
			case OSAL_TEST_MMNGRINITIALIZEMEMORYBUFFEROBJ :
			case OSAL_TEST_MMNGRDEBUGMMNGRPRINT :
			case OSAL_TEST_MMNGRDEBUGBUFFERPRINT :
			default:
				break;
		}
		test_number++;
	}
}

/* ------------------------08_006_001 [OSAL_RETURN_CONF]------------------------ */
static void Test08_006_001()
{
	e_osal_return_t					osal_ret;
	e_osal_return_t					expect_data;
	char							opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int								test_ret = OSAL_TEST_OK;

	size_t							num_of_memory_regions;
	st_osal_mmngr_config_t			mmngr_config;

	expect_data = (e_osal_return_t)osal_test_get_expectdata(OSAL_TEST_MEMORY,
															OSAL_TEST_MMNGRGETCONFIG,
															OSAL_TEST_ID001);

	osal_ret = R_OSAL_Initialize();
	if(OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRGETCONFIG, OSAL_TEST_ID001);
		return;
	}

	num_of_memory_regions = g_osal_memory_region_info_list.num_of_memory_regions;
	g_osal_memory_region_info_list.num_of_memory_regions = 0;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_MmngrGetConfig(OSAL_MEMORY_MANAGER_HANDLE_INVALID, &mmngr_config);
	TIME_MEASURE_STOP
	if (expect_data != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_MmngrGetConfig() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRGETCONFIG, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}

	g_osal_memory_region_info_list.num_of_memory_regions = num_of_memory_regions;

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_ret = R_OSAL_Deinitialize();
	if(OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRGETCONFIG, OSAL_TEST_ID001);
		return;
	}

	osal_test_result_output(OSAL_TEST_MEMORY,
							OSAL_TEST_MMNGRGETCONFIG,
							OSAL_TEST_ID001,
							test_ret, opt_info);
	return;
}

/* ------------------------08_007_001 [OSAL_RETURN_CONF]------------------------ */
static void Test08_007_001()
{
	e_osal_return_t					osal_ret;
	e_osal_return_t					expect_data;
	char							opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int								test_ret = OSAL_TEST_OK;

	size_t							num_of_memory_regions;
	st_osal_mmngr_config_t			mmngr_config;

	expect_data = (e_osal_return_t)osal_test_get_expectdata(OSAL_TEST_MEMORY,
															OSAL_TEST_MMNGRGETOSALMAXCONFIG,
															OSAL_TEST_ID001);

	osal_ret = R_OSAL_Initialize();
	if(OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRGETOSALMAXCONFIG, OSAL_TEST_ID001);
		return;
	}

	num_of_memory_regions = g_osal_memory_region_info_list.num_of_memory_regions;
	g_osal_memory_region_info_list.num_of_memory_regions = 0;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_MmngrGetOsalMaxConfig(&mmngr_config);
	TIME_MEASURE_STOP
	if (expect_data != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_MmngrGetOsalMaxConfig() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRGETOSALMAXCONFIG, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}

	g_osal_memory_region_info_list.num_of_memory_regions = num_of_memory_regions;

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_ret = R_OSAL_Deinitialize();
	if(OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRGETOSALMAXCONFIG, OSAL_TEST_ID001);
		return;
	}

	osal_test_result_output(OSAL_TEST_MEMORY,
							OSAL_TEST_MMNGRGETOSALMAXCONFIG,
							OSAL_TEST_ID001,
							test_ret, opt_info);
	return;
}

/* ------------------------08_008_001 [OSAL_RETURN_CONF]------------------------ */
static void Test08_008_001()
{
	e_osal_return_t					osal_ret;
	e_osal_return_t					expect_data;
	char							opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int								test_ret = OSAL_TEST_OK;

	size_t							num_of_memory_regions;
	st_osal_mmngr_config_t			mmngr_config;

	expect_data = (e_osal_return_t)osal_test_get_expectdata(OSAL_TEST_MEMORY,
															OSAL_TEST_MMNGRGETOSALMAXCONFIGFROMREGIONIDX,
															OSAL_TEST_ID001);

	osal_ret = R_OSAL_Initialize();
	if(OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRGETOSALMAXCONFIGFROMREGIONIDX, OSAL_TEST_ID001);
		return;
	}

	num_of_memory_regions = g_osal_memory_region_info_list.num_of_memory_regions;
	g_osal_memory_region_info_list.num_of_memory_regions = 0;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx(0, &mmngr_config);
	TIME_MEASURE_STOP
	if (expect_data != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRGETOSALMAXCONFIGFROMREGIONIDX, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}

	g_osal_memory_region_info_list.num_of_memory_regions = num_of_memory_regions;

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_ret = R_OSAL_Deinitialize();
	if(OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRGETOSALMAXCONFIGFROMREGIONIDX, OSAL_TEST_ID001);
		return;
	}

	osal_test_result_output(OSAL_TEST_MEMORY,
							OSAL_TEST_MMNGRGETOSALMAXCONFIGFROMREGIONIDX,
							OSAL_TEST_ID001,
							test_ret, opt_info);
	return;
}

/* ------------------------08_011_001 [OSAL_RETURN_OK]------------------------ */
static void Test08_011_001()
{
	e_osal_return_t					osal_ret;
	e_osal_return_t					expect_data;
	char							opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int								test_ret = OSAL_TEST_OK;

	uint64_t						cnt;
	osal_memory_manager_handle_t	mmngr_hndl;
	st_osal_mmngr_config_t			mmngr_config;
	osal_memory_buffer_handle_t		mmngr_buf;
	void*							p_cpu_ptr;

	expect_data = (e_osal_return_t)osal_test_get_expectdata(OSAL_TEST_MEMORY,
															OSAL_TEST_MMNGRGETCPUPTR,
															OSAL_TEST_ID001);

	osal_ret = R_OSAL_Initialize();
	if(OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRGETCPUPTR, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	for (cnt = 0; cnt < g_osal_memory_region_info_list.num_of_memory_regions; cnt++)
	{
		osal_ret = R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx((osal_memory_region_idx_t)cnt, &mmngr_config);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx() error.",
					OSAL_TEST_MEMORY, OSAL_TEST_MMNGRGETCPUPTR, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}

		mmngr_config.mode = OSAL_MMNGR_ALLOC_MODE_STACK;

		osal_ret = R_OSAL_MmngrOpenFromRegionIdx(&mmngr_config, (osal_memory_region_idx_t)cnt, &mmngr_hndl);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_MmngrOpenFromRegionIdx() error.",
					OSAL_TEST_MEMORY, OSAL_TEST_MMNGRGETCPUPTR, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}

		osal_ret = R_OSAL_MmngrAlloc(mmngr_hndl, 64, 64, &mmngr_buf);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_MmngrAlloc() error.",
					OSAL_TEST_MEMORY, OSAL_TEST_MMNGRGETCPUPTR, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}

		osal_ret = R_OSAL_MmngrGetCpuPtr(mmngr_buf, &p_cpu_ptr);
		if (expect_data != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_MmngrGetCpuPtr() error.",
					OSAL_TEST_MEMORY, OSAL_TEST_MMNGRGETCPUPTR, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}
		printf("[Test08_011_001]: Region[%lu](virtual)=%p\n", cnt, (void *)p_cpu_ptr);

		memset(p_cpu_ptr, 0xAA, 64);

		osal_ret = R_OSAL_MmngrDealloc(mmngr_hndl, mmngr_buf);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_MmngrDealloc() error.",
					OSAL_TEST_MEMORY, OSAL_TEST_MMNGRGETCPUPTR, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}

		osal_ret = R_OSAL_MmngrClose(mmngr_hndl);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_MmngrClose() error.",
					OSAL_TEST_MEMORY, OSAL_TEST_MMNGRGETCPUPTR, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}
	}
	TIME_MEASURE_STOP

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_ret = R_OSAL_Deinitialize();
	if(OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRGETCPUPTR, OSAL_TEST_ID001);
		return;
	}

	osal_test_result_output(OSAL_TEST_MEMORY,
							OSAL_TEST_MMNGRGETCPUPTR,
							OSAL_TEST_ID001,
							test_ret, opt_info);
	return;
}

/* ------------------------08_013_001 [OSAL_RETURN_OK]------------------------ */
static void Test08_013_001()
{
	e_osal_return_t					osal_ret;
	e_osal_return_t					expect_data;
	char							opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int								test_ret = OSAL_TEST_OK;

	uint64_t						cnt;
	osal_memory_manager_handle_t	mmngr_hndl;
	st_osal_mmngr_config_t			mmngr_config;
	osal_memory_buffer_handle_t		mmngr_buf;
	void*							p_cpu_ptr;

	expect_data = (e_osal_return_t)osal_test_get_expectdata(OSAL_TEST_MEMORY,
															OSAL_TEST_MMNGRFLUSH,
															OSAL_TEST_ID001);

	osal_ret = R_OSAL_Initialize();
	if(OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRFLUSH, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	for (cnt = 0; cnt < g_osal_memory_region_info_list.num_of_memory_regions; cnt++)
	{
		osal_ret = R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx((osal_memory_region_idx_t)cnt, &mmngr_config);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx() error.",
					OSAL_TEST_MEMORY, OSAL_TEST_MMNGRFLUSH, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}

		mmngr_config.mode = OSAL_MMNGR_ALLOC_MODE_STACK;

		osal_ret = R_OSAL_MmngrOpenFromRegionIdx(&mmngr_config, (osal_memory_region_idx_t)cnt, &mmngr_hndl);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_MmngrOpenFromRegionIdx() error.",
					OSAL_TEST_MEMORY, OSAL_TEST_MMNGRFLUSH, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}

		osal_ret = R_OSAL_MmngrAlloc(mmngr_hndl, 64, 64, &mmngr_buf);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_MmngrAlloc() error.",
					OSAL_TEST_MEMORY, OSAL_TEST_MMNGRFLUSH, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}

		osal_ret = R_OSAL_MmngrFlush(mmngr_buf, 0, 64);
		if (expect_data != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_MmngrFlush() error.",
					OSAL_TEST_MEMORY, OSAL_TEST_MMNGRFLUSH, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}

		osal_ret = R_OSAL_MmngrDealloc(mmngr_hndl, mmngr_buf);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_MmngrDealloc() error.",
					OSAL_TEST_MEMORY, OSAL_TEST_MMNGRFLUSH, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}

		osal_ret = R_OSAL_MmngrClose(mmngr_hndl);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_MmngrClose() error.",
					OSAL_TEST_MEMORY, OSAL_TEST_MMNGRFLUSH, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}
	}
	TIME_MEASURE_STOP

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_ret = R_OSAL_Deinitialize();
	if(OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRFLUSH, OSAL_TEST_ID001);
		return;
	}

	osal_test_result_output(OSAL_TEST_MEMORY,
							OSAL_TEST_MMNGRFLUSH,
							OSAL_TEST_ID001,
							test_ret, opt_info);
	return;
}

/* ------------------------08_014_001 [OSAL_RETURN_OK]------------------------ */
static void Test08_014_001()
{
	e_osal_return_t					osal_ret;
	e_osal_return_t					expect_data;
	char							opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int								test_ret = OSAL_TEST_OK;

	uint64_t						cnt;
	osal_memory_manager_handle_t	mmngr_hndl;
	st_osal_mmngr_config_t			mmngr_config;
	osal_memory_buffer_handle_t		mmngr_buf;
	void*							p_cpu_ptr;

	expect_data = (e_osal_return_t)osal_test_get_expectdata(OSAL_TEST_MEMORY,
															OSAL_TEST_MMNGRINVALIDATE,
															OSAL_TEST_ID001);

	osal_ret = R_OSAL_Initialize();
	if(OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRINVALIDATE, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	for (cnt = 0; cnt < g_osal_memory_region_info_list.num_of_memory_regions; cnt++)
	{
		osal_ret = R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx((osal_memory_region_idx_t)cnt, &mmngr_config);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx() error.",
					OSAL_TEST_MEMORY, OSAL_TEST_MMNGRINVALIDATE, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}

		mmngr_config.mode = OSAL_MMNGR_ALLOC_MODE_STACK;

		osal_ret = R_OSAL_MmngrOpenFromRegionIdx(&mmngr_config, (osal_memory_region_idx_t)cnt, &mmngr_hndl);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_MmngrOpenFromRegionIdx() error.",
					OSAL_TEST_MEMORY, OSAL_TEST_MMNGRINVALIDATE, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}

		osal_ret = R_OSAL_MmngrAlloc(mmngr_hndl, 64, 64, &mmngr_buf);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_MmngrAlloc() error.",
					OSAL_TEST_MEMORY, OSAL_TEST_MMNGRINVALIDATE, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}

		osal_ret = R_OSAL_MmngrInvalidate(mmngr_buf, 0, 64);
		if (expect_data != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_MmngrInvalidate() error.",
					OSAL_TEST_MEMORY, OSAL_TEST_MMNGRINVALIDATE, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}

		osal_ret = R_OSAL_MmngrDealloc(mmngr_hndl, mmngr_buf);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_MmngrDealloc() error.",
					OSAL_TEST_MEMORY, OSAL_TEST_MMNGRINVALIDATE, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}

		osal_ret = R_OSAL_MmngrClose(mmngr_hndl);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_MmngrClose() error.",
					OSAL_TEST_MEMORY, OSAL_TEST_MMNGRINVALIDATE, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}
	}
	TIME_MEASURE_STOP

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_ret = R_OSAL_Deinitialize();
	if(OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRINVALIDATE, OSAL_TEST_ID001);
		return;
	}

	osal_test_result_output(OSAL_TEST_MEMORY,
							OSAL_TEST_MMNGRINVALIDATE,
							OSAL_TEST_ID001,
							test_ret, opt_info);
	return;
}

/* ------------------------08_015_001 [OSAL_RETURN_OK]------------------------ */
static void Test08_015_001()
{
	e_osal_return_t					osal_ret;
	e_osal_return_t					expect_data;
	char							opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int								test_ret = OSAL_TEST_OK;

	uint64_t						cnt;
	osal_memory_manager_handle_t	mmngr_hndl;
	st_osal_mmngr_config_t			mmngr_config;
	osal_memory_buffer_handle_t		mmngr_buf;
	uintptr_t						hw_addr;

	expect_data = (e_osal_return_t)osal_test_get_expectdata(OSAL_TEST_MEMORY,
															OSAL_TEST_MMNGRGETHWADDR,
															OSAL_TEST_ID001);

	osal_ret = R_OSAL_Initialize();
	if(OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRGETHWADDR, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	for (cnt = 0; cnt < g_osal_memory_region_info_list.num_of_memory_regions; cnt++)
	{
		hw_addr = NULL;

		osal_ret = R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx((osal_memory_region_idx_t)cnt, &mmngr_config);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx() error.",
					OSAL_TEST_MEMORY, OSAL_TEST_MMNGRGETHWADDR, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}

		mmngr_config.mode = OSAL_MMNGR_ALLOC_MODE_STACK;

		osal_ret = R_OSAL_MmngrOpenFromRegionIdx(&mmngr_config, (osal_memory_region_idx_t)cnt, &mmngr_hndl);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_MmngrOpenFromRegionIdx() error.",
					OSAL_TEST_MEMORY, OSAL_TEST_MMNGRGETHWADDR, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}

		osal_ret = R_OSAL_MmngrAlloc(mmngr_hndl, 64, 64, &mmngr_buf);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_MmngrAlloc() error.",
					OSAL_TEST_MEMORY, OSAL_TEST_MMNGRGETHWADDR, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}

		osal_ret = R_OSAL_MmngrGetHwAddr(mmngr_buf, OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY, &hw_addr);
		if (expect_data != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_MmngrGetHwAddr() error.",
					OSAL_TEST_MEMORY, OSAL_TEST_MMNGRGETHWADDR, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}
		printf("[Test08_015_001]: Region[%lu](physical)=%p\n", cnt, (void *)hw_addr);

		osal_ret = R_OSAL_MmngrDealloc(mmngr_hndl, mmngr_buf);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_MmngrDealloc() error.",
					OSAL_TEST_MEMORY, OSAL_TEST_MMNGRGETHWADDR, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}

		osal_ret = R_OSAL_MmngrClose(mmngr_hndl);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_MmngrClose() error.",
					OSAL_TEST_MEMORY, OSAL_TEST_MMNGRGETHWADDR, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}
	}
	TIME_MEASURE_STOP

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_ret = R_OSAL_Deinitialize();
	if(OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_MEMORY, OSAL_TEST_MMNGRGETHWADDR, OSAL_TEST_ID001);
		return;
	}

	osal_test_result_output(OSAL_TEST_MEMORY,
							OSAL_TEST_MMNGRGETHWADDR,
							OSAL_TEST_ID001,
							test_ret, opt_info);
	return;
}

/* 08_006: R_OSAL_MmngrGetConfig */
static void osal_test_R_OSAL_MmngrGetConfig()
{
	Test08_006_001();
	return;
}

/* 08_007: R_OSAL_MmngrGetOsalMaxConfig */
static void osal_test_R_OSAL_MmngrGetOsalMaxConfig()
{
	Test08_007_001();
	return;
}

/* 08_008: R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx */
static void osal_test_R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx()
{
	Test08_008_001();
	return;
}

/* 08_011: R_OSAL_MmngrGetCpuPtr */
static void osal_test_R_OSAL_MmngrGetCpuPtr()
{
	Test08_011_001();
	return;
}

/* 08_013: R_OSAL_MmngrFlush */
static void osal_test_R_OSAL_MmngrFlush()
{
	Test08_013_001();
	return;
}

/* 08_014: R_OSAL_MmngrInvalidate */
static void osal_test_R_OSAL_MmngrInvalidate()
{
	Test08_014_001();
	return;
}

/* 08_015: R_OSAL_MmngrGetHwAddr */
static void osal_test_R_OSAL_MmngrGetHwAddr()
{
	Test08_015_001();
	return;
}
