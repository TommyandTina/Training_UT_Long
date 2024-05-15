/**
 * @file pm_manager.c
 * @brief OSAL PM Manager evaluation file.
 * @author K.Takagi
 * @date 2019/12/02
 */

/* include */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "rcar-xos/osal/r_osal.h"
#include "osal_test.h"
#include "osal_test_common.h"
#include "pm_manager.h"
#include "../../../../../../src/include/target/common/pma/r_pma.h"

#define TEST07_001_001 OSAL_TEST_PM, OSAL_TEST_PMSETREQUIREDSTATE, OSAL_TEST_ID001
#define TEST07_001_002 OSAL_TEST_PM, OSAL_TEST_PMSETREQUIREDSTATE, OSAL_TEST_ID002
#define TEST07_001_003 OSAL_TEST_PM, OSAL_TEST_PMSETREQUIREDSTATE, OSAL_TEST_ID003
#define TEST07_001_004 OSAL_TEST_PM, OSAL_TEST_PMSETREQUIREDSTATE, OSAL_TEST_ID004
#define TEST07_001_005 OSAL_TEST_PM, OSAL_TEST_PMSETREQUIREDSTATE, OSAL_TEST_ID005
#define TEST07_001_006 OSAL_TEST_PM, OSAL_TEST_PMSETREQUIREDSTATE, OSAL_TEST_ID006
#define TEST07_001_007 OSAL_TEST_PM, OSAL_TEST_PMSETREQUIREDSTATE, OSAL_TEST_ID007
#define TEST07_001_008 OSAL_TEST_PM, OSAL_TEST_PMSETREQUIREDSTATE, OSAL_TEST_ID008
#define TEST07_001_009 OSAL_TEST_PM, OSAL_TEST_PMSETREQUIREDSTATE, OSAL_TEST_ID009
#define TEST07_001_010 OSAL_TEST_PM, OSAL_TEST_PMSETREQUIREDSTATE, OSAL_TEST_ID010
#define TEST07_001_011 OSAL_TEST_PM, OSAL_TEST_PMSETREQUIREDSTATE, OSAL_TEST_ID011

/* prototype */
//static osal_device_handle_t osal_test_io_init_local(int api_id, int test_id);
//static e_osal_return_t osal_test_io_deinit_local(int api_id, int test_id, osal_device_handle_t handle);
static void osal_test_R_OSAL_PmSetRequiredState();
static void osal_test_R_OSAL_PmGetRequiredState();
static void osal_test_R_OSAL_PmWaitForState();
static void osal_test_R_OSAL_PmGetState();
static void osal_test_R_OSAL_PmSetPolicy();
static void osal_test_R_OSAL_PmGetPolicy();
static void osal_test_R_OSAL_PmSetResetState();
static void osal_test_R_OSAL_PmGetResetState();
static void osal_test_R_PMA_Init();

void osal_test_pm_manager_display_menu()
{
	printf("*** OSAL PM Manager ***\n");
	printf("***  07_001: R_OSAL_PmSetRequiredState  ***\n");
	printf("***  07_002: R_OSAL_PmGetRequiredState  ***\n");
	printf("***  07_003: R_OSAL_PmWaitForState      ***\n");
	printf("***  07_004: R_OSAL_PmGetState          ***\n");
	printf("***  07_005: R_OSAL_PmSetPolicy         ***\n");
	printf("***  07_006: R_OSAL_PmGetPolicy         ***\n");
	printf("***  07_007: R_OSAL_PmSetResetState     ***\n");
	printf("***  07_008: R_OSAL_PmGetResetState     ***\n");
	printf("***  07_009: R_PMA_Init     ***\n");
	printf("***  07_100: pm manager module all exec ***\n\n");
}

void osal_test_pm_manager_menu(int child)
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
		test_number = OSAL_TEST_PMSETREQUIREDSTATE;
		test_count  = OSAL_TEST_PM_MAX;
	}

	for (loop_count = 0; loop_count < test_count; loop_count++)
	{
		switch (test_number)
		{
			case OSAL_TEST_PMSETREQUIREDSTATE:
				osal_test_R_OSAL_PmSetRequiredState();
				break;
			case OSAL_TEST_PMGETREQUIREDSTATE:
				osal_test_R_OSAL_PmGetRequiredState();
				break;
			case OSAL_TEST_PMSETANDWAITFORSTATE:
				osal_test_R_OSAL_PmWaitForState();
				break;
			case OSAL_TEST_PMGETSTATE:
				osal_test_R_OSAL_PmGetState();
				break;
			case OSAL_TEST_PMSETPOLICY:
				osal_test_R_OSAL_PmSetPolicy();
				break;
			case OSAL_TEST_PMGETPOLICY:
				osal_test_R_OSAL_PmGetPolicy();
				break;
			case OSAL_TEST_PMSETRESETSTATE:
				osal_test_R_OSAL_PmSetResetState();
				break;
			case OSAL_TEST_PMGETRESETSTATE:
				osal_test_R_OSAL_PmGetResetState();
				break;
                        case OSAL_TEST_PM_OSAL_PM_API_INIT:
                                osal_test_R_PMA_Init();
                                break;
			default:
				printf("That menu is not available.\n\n");
				break;
		}

		test_number++;
	}
}

int64_t test_thread(void *user_arg)
{
    osal_device_handle_t dev_handle;
    e_osal_return_t osal_ret;
    
    dev_handle = (osal_device_handle_t)user_arg;
    
    osal_ret = R_OSAL_PmWaitForState(dev_handle, OSAL_PM_STATE_READY, 10000);
    

    if (OSAL_RETURN_OK == osal_ret)
    {
        IT_DEBUG("----------- Wait State Done\n");
    }
    else if (OSAL_RETURN_TIME == osal_ret)
    {
        IT_DEBUG("=========== Wait State timeout\n");
    }

    return 0;
}


static void Test07_001_001(){

	int test_ret = OSAL_TEST_OK;
	e_osal_return_t osal_ret;
	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_required_state_t req_state;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
	osal_ret = R_OSAL_Initialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", TEST07_001_001);
		return;
	}

	/* Get expect data */
	expect_data = (e_osal_return_t)osal_test_get_expectdata(TEST07_001_001);

	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.", TEST07_001_001);
		(void)R_OSAL_Deinitialize();
		return;
	}

	/* Get require state */
	osal_ret = R_OSAL_PmGetRequiredState(dev_handle, &req_state);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmGetRequiredState() error.", TEST07_001_001);
		return;
	}

	if (req_state == OSAL_PM_REQUIRED_STATE_REQUIRED)
	{
		R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_PmSetRequiredState() error.", TEST07_001_001);
			return;
		}
	}
	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	target_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/
	osal_ret = R_OSAL_PmGetRequiredState(dev_handle, &req_state);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmGetRequiredState() error.", TEST07_001_001);
	}

	/****************************/
	/* Check result				*/
	/****************************/
	if ((target_ret == expect_data) && (req_state == OSAL_PM_REQUIRED_STATE_REQUIRED))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));

	osal_test_result_output(TEST07_001_001, test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", TEST07_001_001);
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", TEST07_001_001);
		return;
	}
}

static void Test07_001_002(){

	int test_ret = OSAL_TEST_OK;
	e_osal_return_t osal_ret;
	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_required_state_t req_state;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
	osal_ret = R_OSAL_Initialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", TEST07_001_002);
		return;
	}

	/* Get expect data */
	expect_data = (e_osal_return_t)osal_test_get_expectdata(TEST07_001_002);

	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.", TEST07_001_002);
		(void)R_OSAL_Deinitialize();
		return;
	}

	/* Get require state */
	osal_ret = R_OSAL_PmGetRequiredState(dev_handle, &req_state);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmGetRequiredState() error.", TEST07_001_002);
	}

	if (req_state == OSAL_PM_REQUIRED_STATE_REQUIRED)
	{
		R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_PmSetRequiredState() error.", TEST07_001_002);
			return;
		}
	}

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	target_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, false);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/
	osal_ret = R_OSAL_PmGetRequiredState(dev_handle, &req_state);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmGetRequiredState() error.", TEST07_001_002);
	}

	/****************************/
	/* Check result				*/
	/****************************/
	if ((target_ret == expect_data) && (req_state == OSAL_PM_REQUIRED_STATE_REQUIRED))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));

	osal_test_result_output(TEST07_001_002, test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", TEST07_001_002);
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", TEST07_001_002);
		return;
	}
}


static void Test07_001_003(){

	int test_ret = OSAL_TEST_OK;
	e_osal_return_t osal_ret;
	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_required_state_t req_state;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
	osal_ret = R_OSAL_Initialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", TEST07_001_003);
		return;
	}

	/* Get expect data */
	expect_data = (e_osal_return_t)osal_test_get_expectdata(TEST07_001_003);

	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.", TEST07_001_003);
		(void)R_OSAL_Deinitialize();
		return;
	}

	/* Get require state */
	osal_ret = R_OSAL_PmGetRequiredState(dev_handle, &req_state);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmGetRequiredState() error.", TEST07_001_003);
	}

	if (req_state == OSAL_PM_REQUIRED_STATE_RELEASED)
	{
		R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_PmSetRequiredState() error.", TEST07_001_003);
			return;
		}
	}

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	target_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/
	osal_ret = R_OSAL_PmGetRequiredState(dev_handle, &req_state);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmGetRequiredState() error.", TEST07_001_003);
	}

	/****************************/
	/* Check result				*/
	/****************************/
	if ((target_ret == expect_data) && (req_state == OSAL_PM_REQUIRED_STATE_RELEASED))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));

	osal_test_result_output(TEST07_001_003, test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", TEST07_001_003);
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", TEST07_001_003);
		return;
	}
}


static void Test07_001_004(){

	int test_ret = OSAL_TEST_OK;
	e_osal_return_t osal_ret;
	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
	osal_ret = R_OSAL_Initialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", TEST07_001_004);
		return;
	}

	/* Get expect data */
	expect_data = (e_osal_return_t)osal_test_get_expectdata(TEST07_001_004);

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	target_ret = R_OSAL_PmSetRequiredState(NULL, OSAL_PM_REQUIRED_STATE_RELEASED, true);
	TIME_MEASURE_STOP

	/****************************/
	/* Check result				*/
	/****************************/
	if (target_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));

	osal_test_result_output(TEST07_001_004, test_ret, opt_info);

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", TEST07_001_004);
		return;
	}
}


static void Test07_001_005(){

	int test_ret = OSAL_TEST_OK;
	e_osal_return_t osal_ret;
	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t dev_handle;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
	osal_ret = R_OSAL_Initialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", TEST07_001_005);
		return;
	}

	/* Get expect data */
	expect_data = (e_osal_return_t)osal_test_get_expectdata(TEST07_001_005);

	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.", TEST07_001_005);
		(void)R_OSAL_Deinitialize();
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", TEST07_001_005);
	}

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	target_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/

	/****************************/
	/* Check result				*/
	/****************************/
	if (target_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));

	osal_test_result_output(TEST07_001_005, test_ret, opt_info);

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", TEST07_001_005);
		return;
	}
}

static void Test07_001_006(){

	int test_ret = OSAL_TEST_OK;
	e_osal_return_t osal_ret;
	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	bool deviceopen = true;
	osal_device_handle_t dev_handle;

    printf("This test: Device handle without power control returns OSAL_RETURN_FAIL\n");
    printf("However, PoC implemetation is not check power control, so returns OSAL_RETURN_OK\n");


	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
	osal_ret = R_OSAL_Initialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", TEST07_001_006);
		return;
	}

	/* Get expect data */
	expect_data = (e_osal_return_t)osal_test_get_expectdata(TEST07_001_006);

	/* Open device */
	/* Device id "fbc_00" don't have pma_handle */
	osal_ret = R_OSAL_IoDeviceOpen("fbc_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		deviceopen = false;
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.", TEST07_001_006);
		//(void)R_OSAL_Deinitialize();
		//return;
	}
	
	if(deviceopen == true){
		/****************************/
		/* Test execution			*/

		/****************************/
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		target_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
		TIME_MEASURE_STOP

		/****************************/
		/* Post-process				*/
		/****************************/

		/****************************/
		/* Check result				*/
		/****************************/
		if (target_ret == expect_data)
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
			test_ret = OSAL_TEST_NG;
		}

		OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));

		osal_test_result_output(TEST07_001_006, test_ret, opt_info);

		osal_ret = R_OSAL_IoDeviceClose(dev_handle);
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", TEST07_001_006);
		}
	}
	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", TEST07_001_006);
		return;
	}
}


static void Test07_001_007(){

	int test_ret = OSAL_TEST_OK;
	e_osal_return_t osal_ret;
	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
	osal_ret = R_OSAL_Initialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", TEST07_001_007);
		return;
	}

	/* Get expect data */
	expect_data = (e_osal_return_t)osal_test_get_expectdata(TEST07_001_007);

	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.", TEST07_001_007);
		(void)R_OSAL_Deinitialize();
		return;
	}

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	target_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_INVALID, true);
	TIME_MEASURE_STOP

	/****************************/
	/* Check result				*/
	/****************************/
	if (target_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));

	osal_test_result_output(TEST07_001_007, test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", TEST07_001_007);
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", TEST07_001_007);
		return;
	}
}


static void Test07_001_008(){

	int test_ret = OSAL_TEST_OK;
	e_osal_return_t osal_ret;
	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
	osal_ret = R_OSAL_Initialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", TEST07_001_008);
		return;
	}

	/* Get expect data */
	expect_data = (e_osal_return_t)osal_test_get_expectdata(TEST07_001_008);

	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.", TEST07_001_008);
		(void)R_OSAL_Deinitialize();
		return;
	}

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	target_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED + 1, true);
	TIME_MEASURE_STOP

	/****************************/
	/* Check result				*/
	/****************************/
	if (target_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));

	osal_test_result_output(TEST07_001_008, test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", TEST07_001_008);
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", TEST07_001_008);
		return;
	}
}

static void Test07_001_009(){

	int test_ret = OSAL_TEST_OK;
	e_osal_return_t osal_ret;
	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
	osal_ret = R_OSAL_Initialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", TEST07_001_009);
		return;
	}
	memset(&dev_handle, 0, sizeof(osal_device_handle_t));
	/* Get expect data */
	expect_data = (e_osal_return_t)osal_test_get_expectdata(TEST07_001_009);

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", TEST07_001_009);
		return;
	}

//	IT_DEBUG("Before execution: Require state %u PM state %u\n",req_state, pm_state);

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	target_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/

	/****************************/
	/* Check result				*/
	/****************************/
	if (target_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));

	osal_test_result_output(TEST07_001_009, test_ret, opt_info);

}

// static void Test07_001_010(){

// 	int test_ret = OSAL_TEST_OK;
// 	e_osal_return_t osal_ret;
// 	e_osal_return_t target_ret;
// 	e_osal_return_t expect_data;
// 	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

// 	osal_device_handle_t dev_handle;

// 	/****************************/
// 	/* Pre-process				*/
// 	/****************************/
// 	/* Init device */
// 	osal_ret = R_OSAL_Initialize();
// 	if (osal_ret != OSAL_RETURN_OK)
// 	{
// 		OSAL_TEST_ERR("R_OSAL_Initialize() error.", TEST07_001_010);
// 		return;
// 	}

// 	/* Get expect data */
// 	expect_data = (e_osal_return_t)osal_test_get_expectdata(TEST07_001_010);

// 	/* Open device */
// 	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
// 	if (osal_ret != OSAL_RETURN_OK)
// 	{
// 		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.", TEST07_001_010);
// 		(void)R_OSAL_Deinitialize();
// 		return;
// 	}

// 	/****************************/
// 	/* Test execution			*/
// 	/****************************/
// 	TIME_MEASURE_INIT
// 	TIME_MEASURE_START
// 	g_stub_flag.R_PMA_RequestHwa = R_PMA_NG;
// 	target_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
// 	TIME_MEASURE_STOP
// 	g_stub_flag.R_PMA_RequestHwa = 0;

// 	/****************************/
// 	/* Check result				*/
// 	/****************************/
// 	if (target_ret == expect_data)
// 	{
// 		test_ret = OSAL_TEST_OK;
// 	}
// 	else
// 	{
// 		test_ret = OSAL_TEST_NG;
// 	}

// 	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));

// 	osal_test_result_output(TEST07_001_010, test_ret, opt_info);

// 	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
// 	if (osal_ret != OSAL_RETURN_OK)
// 	{
// 		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", TEST07_001_010);
// 	}

// 	osal_ret = R_OSAL_Deinitialize();
// 	if (osal_ret != OSAL_RETURN_OK)
// 	{
// 		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", TEST07_001_010);
// 		return;
// 	}
// }

// static void Test07_001_011(){

// 	int test_ret = OSAL_TEST_OK;
// 	e_osal_return_t osal_ret;
// 	e_osal_return_t target_ret;
// 	e_osal_return_t expect_data;
// 	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

// 	osal_device_handle_t dev_handle;

// 	/****************************/
// 	/* Pre-process				*/
// 	/****************************/
// 	/* Init device */
// 	osal_ret = R_OSAL_Initialize();
// 	if (osal_ret != OSAL_RETURN_OK)
// 	{
// 		OSAL_TEST_ERR("R_OSAL_Initialize() error.", TEST07_001_011);
// 		return;
// 	}

// 	/* Get expect data */
// 	expect_data = (e_osal_return_t)osal_test_get_expectdata(TEST07_001_011);

// 	/* Open device */
// 	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
// 	if (osal_ret != OSAL_RETURN_OK)
// 	{
// 		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.", TEST07_001_011);
// 		(void)R_OSAL_Deinitialize();
// 		return;
// 	}

// 	/****************************/
// 	/* Test execution			*/
// 	/****************************/
// 	g_stub_flag.R_PMA_RequestHwa = R_PMA_BUS_IF_CHK_NG;
// 	TIME_MEASURE_INIT
// 	TIME_MEASURE_START
// 	target_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
// 	TIME_MEASURE_STOP
// 	g_stub_flag.R_PMA_RequestHwa = 0;

// 	/****************************/
// 	/* Check result				*/
// 	/****************************/
// 	if (target_ret == expect_data)
// 	{
// 		test_ret = OSAL_TEST_OK;
// 	}
// 	else
// 	{
// 		test_ret = OSAL_TEST_NG;
// 	}

// 	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));

// 	osal_test_result_output(TEST07_001_011, test_ret, opt_info);

// 	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
// 	if (osal_ret != OSAL_RETURN_OK)
// 	{
// 		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", TEST07_001_011);
// 	}

// 	osal_ret = R_OSAL_Deinitialize();
// 	if (osal_ret != OSAL_RETURN_OK)
// 	{
// 		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", TEST07_001_011);
// 		return;
// 	}
// }

static void Test07_002_001(){

	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_required_state_t req_state;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID001);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID001);

	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID001);
		return;
	}

	/* Set state */
	osal_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmGetRequiredState(dev_handle, &req_state);
	TIME_MEASURE_STOP


	/****************************/
	/* Post-process				*/
	/****************************/


	/****************************/
	/* Check result				*/
	/****************************/
	if ((osal_ret == expect_data) && (req_state == OSAL_PM_REQUIRED_STATE_REQUIRED))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID001,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID001);
		return;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID001);
		return;
	}
}

static void Test07_002_002(){

	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_required_state_t req_state;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID002);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID002);

	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID002);
		return;
	}
	/* Set state */
	osal_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmGetRequiredState(dev_handle, &req_state);
	TIME_MEASURE_STOP


	/****************************/
	/* Post-process				*/
	/****************************/


	/****************************/
	/* Check result				*/
	/****************************/
	if ((osal_ret == expect_data) && (req_state == OSAL_PM_REQUIRED_STATE_RELEASED))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID002,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID002);
		return;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID002);
		return;
	}
}

static void Test07_002_003(){

	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	e_osal_pm_required_state_t req_state;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID003);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID003);

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmGetRequiredState(NULL, &req_state);
	TIME_MEASURE_STOP

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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID003,
							test_ret, opt_info);

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID003);
		return;
	}
}

static void Test07_002_004()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID004);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID004);

	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID004);
		return;
	}

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmGetRequiredState(dev_handle, NULL);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/

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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID004,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID004);
		return;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID004);
		return;
	}
}

static void Test07_002_005()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_required_state_t req_state;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID005);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID005);

	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID005);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID005);
		return;
	}
	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmGetRequiredState(dev_handle, &req_state);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/

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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID005,
							test_ret, opt_info);

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID005);
		return;
	}
}

static void Test07_002_006()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	bool deviceopen = true;
	osal_device_handle_t dev_handle;
	e_osal_pm_required_state_t req_state;

    printf("This test: Device handle without power control returns OSAL_RETURN_FAIL\n");
    printf("However, PoC implemetation is not check power control, so returns OSAL_RETURN_OK\n");

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID006);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID006);

	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("fbc_00", &dev_handle);	/*Check point*/
	if (osal_ret != OSAL_RETURN_OK)
	{
        deviceopen = false;
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID006);
		//return;
	}

    if(deviceopen == true){
        /****************************/
        /* Test execution			*/
        /****************************/
        TIME_MEASURE_INIT
        TIME_MEASURE_START
        osal_ret = R_OSAL_PmGetRequiredState(dev_handle, &req_state);
        TIME_MEASURE_STOP


        /****************************/
        /* Post-process				*/
        /****************************/


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
        
        OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

        osal_test_result_output(OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID006,
                                test_ret, opt_info);

        osal_ret = R_OSAL_IoDeviceClose(dev_handle);
        if (osal_ret != OSAL_RETURN_OK)
        {
            OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
                OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID006);
            //return;
        }
    }
	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID006);
		//return;
	}
}

static void Test07_002_007()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_required_state_t req_state;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID007);
		return;
	}
	memset(&dev_handle, 0, sizeof(osal_device_handle_t));
	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID007);

	R_OSAL_Deinitialize();

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmGetRequiredState(dev_handle, &req_state);
	TIME_MEASURE_STOP
	
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
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID007,
							test_ret, opt_info);
}

//static void Test07_002_008()
//{
	//int test_ret = OSAL_TEST_OK;

	//e_osal_return_t osal_ret;
	//e_osal_return_t expect_data;
	//char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	//osal_device_handle_t dev_handle;
	//e_osal_pm_required_state_t req_state;

	///****************************/
	///* Pre-process				*/
	///****************************/
	///* Init device */
    //osal_ret = osal_init();
	//if (osal_ret != OSAL_RETURN_OK)
	//{
		//OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			//OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID008);
		//return;
	//}

	///* Get expect data */
	//expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID008);

	///* Open device */
	//osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	//if (osal_ret != OSAL_RETURN_OK)
	//{
		//OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			//OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID008);
		//return;
	//}

	///* Set state */
	//osal_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);

	///****************************/
	///* Test execution			*/
	///****************************/
	//g_stub_flag.R_PMA_GetHwaState = R_PMA_NG;
	//TIME_MEASURE_INIT
	//TIME_MEASURE_START
	//osal_ret = R_OSAL_PmGetRequiredState(dev_handle, &req_state);
	//TIME_MEASURE_STOP
	//g_stub_flag.R_PMA_GetHwaState = 0;


	///****************************/
	///* Post-process				*/
	///****************************/


	///****************************/
	///* Check result				*/
	///****************************/
	//if (osal_ret == expect_data) 
	//{
		//test_ret = OSAL_TEST_OK;
	//}
	//else
	//{
		//test_ret = OSAL_TEST_NG;
	//}
	
	//OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	//osal_test_result_output(OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID008,
							//test_ret, opt_info);

	//osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	//if (osal_ret != OSAL_RETURN_OK)
	//{
		//OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			//OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID008);
		//return;
	//}

	//osal_ret = R_OSAL_Deinitialize();
	//if (osal_ret != OSAL_RETURN_OK)
	//{
		//OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			//OSAL_TEST_PM, OSAL_TEST_PMGETREQUIREDSTATE, OSAL_TEST_ID008);
		//return;
	//}
//}

static void Test07_003_001()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_required_state_t req_state;
	e_osal_pm_state_t pm_state;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
	osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID001);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID001);
	
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID001);
		return;
	}

	R_OSAL_PmGetRequiredState(dev_handle, &req_state);
	R_OSAL_PmGetState(dev_handle, &pm_state);
	if (pm_state == OSAL_PM_STATE_PG)
	{
		/* change pm state to CG */
		if(req_state == OSAL_PM_REQUIRED_STATE_REQUIRED)
		{
			R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_CG, true);
		}
		else
		{
			R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
			R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_CG, true);
		}
		R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
		R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_PG, true);
	}
	else
	{
		if (req_state == OSAL_PM_REQUIRED_STATE_RELEASED)
		{
			R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_PG, true);
		}
		else
		{
			R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
			R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_PG, true);
		}
	}
	
	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmWaitForState(dev_handle, OSAL_PM_STATE_PG, 1000);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/

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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID001,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID001);
		return;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID001);
		return;
	}

}  
 
static void Test07_003_002()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_required_state_t req_state;
	e_osal_pm_state_t pm_state;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
	osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID002);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID002);
	
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID002);
		return;
	}

	R_OSAL_PmGetRequiredState(dev_handle, &req_state);
	R_OSAL_PmGetState(dev_handle, &pm_state);
	if (pm_state == OSAL_PM_STATE_CG)
	{
		/* change pm state to PG */
		if(req_state == OSAL_PM_REQUIRED_STATE_REQUIRED)
		{
			R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_PG, true);
		}
		else
		{
			R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
			R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_PG, true);
		}
		R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
		R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_CG, true);
	}
	else
	{
		if (req_state == OSAL_PM_REQUIRED_STATE_RELEASED)
		{
			R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_CG, true);
		}
		else
		{
			R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
			R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_CG, true);
		}
	}

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmWaitForState(dev_handle, OSAL_PM_STATE_CG, 1000);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/

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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID002,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID002);
		return;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID002);
		return;
	}

}  
 
static void Test07_003_003()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_required_state_t req_state;
	e_osal_pm_state_t pm_state;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
	osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID003);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID003);
	
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID003);
		return;
	}

	R_OSAL_PmGetRequiredState(dev_handle, &req_state);
	R_OSAL_PmGetState(dev_handle, &pm_state);
	if (pm_state == OSAL_PM_STATE_ENABLED)
	{
		/* change pm state to PG */
		if(req_state == OSAL_PM_REQUIRED_STATE_REQUIRED)
		{
			R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_PG, true);
		}
		else
		{
			R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
			R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_PG, true);
		}
		R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
		R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_HP, true);
	}
	else
	{
		if (req_state == OSAL_PM_REQUIRED_STATE_RELEASED)
		{
			R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_HP, true);
		}
		else
		{
			R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
			R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_HP, true);
		}
	}

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmWaitForState(dev_handle, OSAL_PM_STATE_ENABLED, 1000);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/

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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID003,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID003);
		return;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID003);
		return;
	}

}  
 
static void Test07_003_004()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
	osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID004);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM, 
														OSAL_TEST_PMSETANDWAITFORSTATE , 
														OSAL_TEST_ID004);
	
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID004);
		return;
	}

	R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
	R_OSAL_PmSetResetState(dev_handle, OSAL_PM_RESET_STATE_APPLIED);

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmWaitForState(dev_handle, OSAL_PM_STATE_RESET, 10);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/

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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM, 
							OSAL_TEST_PMSETANDWAITFORSTATE , 
							OSAL_TEST_ID004,
							test_ret, opt_info);

	//IT_DEBUG("Done 07_003_00%d %d \n", test_no ,osal_ret);
	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID004);
		return;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID004);
		return;
	}

}  
 
static void Test07_003_005()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
	osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID005);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM, 
														OSAL_TEST_PMSETANDWAITFORSTATE , 
														OSAL_TEST_ID005);
	
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID005);
		return;
	}

	R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
	R_OSAL_PmSetResetState(dev_handle, OSAL_PM_RESET_STATE_RELEASED);

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmWaitForState(dev_handle,OSAL_PM_STATE_READY, 10);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/

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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM, 
							OSAL_TEST_PMSETANDWAITFORSTATE , 
							OSAL_TEST_ID005,
							test_ret, opt_info);

	//IT_DEBUG("Done 07_003_00%d %d \n", test_no ,osal_ret);
	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID005);
		return;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID005);
		return;
	}

}  
 
static void Test07_003_006()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_required_state_t req_state;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
	osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID006);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM, 
														OSAL_TEST_PMSETANDWAITFORSTATE , 
														OSAL_TEST_ID006);
	
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID006);
		return;
	}

	R_OSAL_PmGetRequiredState(dev_handle, &req_state);
	if(req_state == OSAL_PM_REQUIRED_STATE_REQUIRED)
	{
		R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_PG, true);
	}
	else
	{
		R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
		R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_PG, true);
	}
	
	R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmWaitForState(dev_handle, OSAL_PM_STATE_PG, 1000);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/

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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM, 
							OSAL_TEST_PMSETANDWAITFORSTATE , 
							OSAL_TEST_ID006,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID006);
		return;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID006);
		return;
	}

}  
 
static void Test07_003_007()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_required_state_t req_state;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
	osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID007);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM, 
														OSAL_TEST_PMSETANDWAITFORSTATE , 
														OSAL_TEST_ID007);
	
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID007);
		return;
	}

	R_OSAL_PmGetRequiredState(dev_handle, &req_state);
	if(req_state == OSAL_PM_REQUIRED_STATE_REQUIRED)
	{
		R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_CG, true);
	}
	else
	{
		R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
		R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_CG, true);
	}
	
	R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmWaitForState(dev_handle, OSAL_PM_STATE_CG, 10);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/

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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM, 
							OSAL_TEST_PMSETANDWAITFORSTATE , 
							OSAL_TEST_ID007,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID007);
		return;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID007);
		return;
	}

}  
 
static void Test07_003_008()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_required_state_t req_state;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
	osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID008);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM, 
														OSAL_TEST_PMSETANDWAITFORSTATE , 
														OSAL_TEST_ID008);
	
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID008);
		return;
	}

	R_OSAL_PmGetRequiredState(dev_handle, &req_state);
	if(req_state == OSAL_PM_REQUIRED_STATE_REQUIRED)
	{
		R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_HP, true);
	}
	else
	{
		R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
		R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_HP, true);
	}
	
	R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmWaitForState(dev_handle, OSAL_PM_STATE_ENABLED, 1000);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/

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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM, 
							OSAL_TEST_PMSETANDWAITFORSTATE , 
							OSAL_TEST_ID008,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID008);
		return;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID008);
		return;
	}

}  
 
static void Test07_003_009()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
	osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID009);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM, 
														OSAL_TEST_PMSETANDWAITFORSTATE , 
														OSAL_TEST_ID009);
	
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID009);
		return;
	}

	R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
	R_OSAL_PmSetResetState(dev_handle, OSAL_PM_RESET_STATE_APPLIED);

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmWaitForState(dev_handle, OSAL_PM_STATE_RESET, 10);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/

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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM, 
							OSAL_TEST_PMSETANDWAITFORSTATE , 
							OSAL_TEST_ID009,
							test_ret, opt_info);

	//IT_DEBUG("Done 07_003_00%d %d \n", test_no ,osal_ret);
	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID009);
		return;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID009);
		return;
	}

}  
 
static void Test07_003_010()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
	osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID010);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM, 
														OSAL_TEST_PMSETANDWAITFORSTATE , 
														OSAL_TEST_ID010);
	
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID010);
		return;
	}

	R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
	R_OSAL_PmSetResetState(dev_handle, OSAL_PM_RESET_STATE_APPLIED);

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmWaitForState(dev_handle,OSAL_PM_STATE_PG, 10);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/

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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM, 
							OSAL_TEST_PMSETANDWAITFORSTATE , 
							OSAL_TEST_ID010,
							test_ret, opt_info);

	//IT_DEBUG("Done 07_003_00%d %d \n", test_no ,osal_ret);
	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID010);
		return;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID010);
		return;
	}

}  
 
static void Test07_003_011()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
	osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID011);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM, 
														OSAL_TEST_PMSETANDWAITFORSTATE , 
														OSAL_TEST_ID011);
	
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID011);
		return;
	}

	R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
	R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_PG, true);

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmWaitForState(NULL,OSAL_PM_STATE_PG, 10);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/

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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM, 
							OSAL_TEST_PMSETANDWAITFORSTATE , 
							OSAL_TEST_ID011,
							test_ret, opt_info);

	//IT_DEBUG("Done 07_003_00%d %d \n", test_no ,osal_ret);
	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID011);
		return;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID011);
		return;
	}

}  
 
static void Test07_003_012()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
	osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID012);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM, 
														OSAL_TEST_PMSETANDWAITFORSTATE , 
														OSAL_TEST_ID012);
	
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID012);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID012);
		return;
	}


	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmWaitForState(dev_handle,OSAL_PM_STATE_PG, 10);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/

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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM, 
							OSAL_TEST_PMSETANDWAITFORSTATE , 
							OSAL_TEST_ID012,
							test_ret, opt_info);

	//IT_DEBUG("Done 07_003_00%d %d \n", test_no ,osal_ret);

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID012);
		return;
	}

}  
 
static void Test07_003_013()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;

    printf("This test: Device handle without power control returns OSAL_RETURN_FAIL\n");
    printf("However, PoC implemetation is not check power control, so returns OSAL_RETURN_OK\n");

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
	osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID013);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM, 
														OSAL_TEST_PMSETANDWAITFORSTATE , 
														OSAL_TEST_ID013);
	
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("fbc_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID013);
		(void)R_OSAL_Deinitialize();
		return;
	}

	R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
	R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_PG, true);

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmWaitForState(dev_handle,OSAL_PM_STATE_PG, 10);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/

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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM, 
							OSAL_TEST_PMSETANDWAITFORSTATE , 
							OSAL_TEST_ID013,
							test_ret, opt_info);

	//IT_DEBUG("Done 07_003_00%d %d \n", test_no ,osal_ret);
	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID013);
		//return;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID013);
		return;
	}

}

static void Test07_003_014()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
	osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID014);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM, 
														OSAL_TEST_PMSETANDWAITFORSTATE , 
														OSAL_TEST_ID014);
	
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID014);
		return;
	}

	R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
	R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_PG, true);

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmWaitForState(dev_handle, OSAL_PM_STATE_INVALID, 10); /* Check point */
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/

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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM, 
							OSAL_TEST_PMSETANDWAITFORSTATE , 
							OSAL_TEST_ID014,
							test_ret, opt_info);

	//IT_DEBUG("Done 07_003_00%d %d \n", test_no ,osal_ret);
	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID014);
		return;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID014);
		return;
	}

}  
 
static void Test07_003_015()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
	osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID015);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM, 
														OSAL_TEST_PMSETANDWAITFORSTATE , 
														OSAL_TEST_ID015);
	
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID015);
		return;
	}

	R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
	R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_PG, true);

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmWaitForState(dev_handle, OSAL_PM_STATE_READY + 1, 10);	/* Check point */
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/

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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM, 
							OSAL_TEST_PMSETANDWAITFORSTATE , 
							OSAL_TEST_ID015,
							test_ret, opt_info);

	//IT_DEBUG("Done 07_003_00%d %d \n", test_no ,osal_ret);
	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID015);
		return;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID015);
		return;
	}

}  
 
static void Test07_003_016()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
	osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID016);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM, 
														OSAL_TEST_PMSETANDWAITFORSTATE , 
														OSAL_TEST_ID016);
	
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID016);
		return;
	}

	R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
	R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_PG, true);

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmWaitForState(dev_handle,OSAL_PM_STATE_PG, (osal_milli_sec_t)-1);	/* Check point */
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/

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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM, 
							OSAL_TEST_PMSETANDWAITFORSTATE , 
							OSAL_TEST_ID016,
							test_ret, opt_info);

	//IT_DEBUG("Done 07_003_00%d %d \n", test_no ,osal_ret);
	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID016);
		return;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID016);
		return;
	}

}  
 
static void Test07_003_017()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
	osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID017);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM, 
														OSAL_TEST_PMSETANDWAITFORSTATE , 
														OSAL_TEST_ID017);
	
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID017);
		return;
	}

	R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
	R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_PG, true);

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmWaitForState(dev_handle,OSAL_PM_STATE_PG, 10);
	TIME_MEASURE_STOP
		

	/****************************/
	/* Post-process				*/
	/****************************/

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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM, 
							OSAL_TEST_PMSETANDWAITFORSTATE , 
							OSAL_TEST_ID017,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID017);
		return;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID017);
		return;
	}

}  

static void Test07_003_018()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
	osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.", 
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID018);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM, 
														OSAL_TEST_PMSETANDWAITFORSTATE , 
														OSAL_TEST_ID018);
	
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID018);
		return;
	}

	R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
	R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_PG, true);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	osal_ret = R_OSAL_Deinitialize();			/* Check point */

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmWaitForState(dev_handle,OSAL_PM_STATE_PG, 10);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/

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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM, 
							OSAL_TEST_PMSETANDWAITFORSTATE , 
							OSAL_TEST_ID018,
							test_ret, opt_info);

}

//static void Test07_003_019()
//{
	//int test_ret = OSAL_TEST_OK;

	//e_osal_return_t osal_ret;
	//e_osal_return_t expect_data;
	//char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	//osal_device_handle_t dev_handle;

	///****************************/
	///* Pre-process				*/
	///****************************/
	///* Init device */
	//osal_ret = osal_init();
	//if (osal_ret != OSAL_RETURN_OK)
	//{
		//OSAL_TEST_ERR("R_OSAL_Initialize() error.", 
			//OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID019);
		//return;
	//}

	///* Get expect data */
	//expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM, 
														//OSAL_TEST_PMSETANDWAITFORSTATE , 
														//OSAL_TEST_ID019);
	
	///* Open device */
	//osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	//if (osal_ret != OSAL_RETURN_OK)
	//{
		//OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			//OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID019);
		//return;
	//}

	//R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
	//R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_PG, true);


	///****************************/
	///* Test execution			*/
	///****************************/
	//g_stub_flag.R_PMA_GetHwaState = R_PMA_NG;
	//TIME_MEASURE_INIT
	//TIME_MEASURE_START
	//osal_ret = R_OSAL_PmWaitForState(dev_handle,OSAL_PM_STATE_PG, 10);
	//TIME_MEASURE_STOP
	//g_stub_flag.R_PMA_GetHwaState = 0;


	///****************************/
	///* Post-process				*/
	///****************************/

	///****************************/
	///* Check result				*/
	///****************************/
	//if (osal_ret == expect_data) 
	//{
		//test_ret = OSAL_TEST_OK;
	//}
	//else
	//{
		//test_ret = OSAL_TEST_NG;
	//}
	
	//OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	//osal_test_result_output(OSAL_TEST_PM, 
							//OSAL_TEST_PMSETANDWAITFORSTATE , 
							//OSAL_TEST_ID019,
							//test_ret, opt_info);

	//osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	//if (osal_ret != OSAL_RETURN_OK)
	//{
		//OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			//OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID019);
		//return;
	//}
	//osal_ret = R_OSAL_Deinitialize();
	//if (osal_ret != OSAL_RETURN_OK)
	//{
		//OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			//OSAL_TEST_PM, OSAL_TEST_PMSETANDWAITFORSTATE, OSAL_TEST_ID019);
		//return;
	//}

//}

static void Test07_004_001()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_state_t pm_state;
	e_osal_pm_required_state_t req_state;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID001);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMGETSTATE,
														OSAL_TEST_ID001);

	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID001);
		return;
	}

	R_OSAL_PmGetRequiredState(dev_handle, &req_state);
	if(req_state == OSAL_PM_REQUIRED_STATE_REQUIRED)
	{
		R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_PG, true);
	}
	else
	{
		R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
		R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_PG, true);
	}
	
	R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
	

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmGetState(dev_handle, &pm_state);
	TIME_MEASURE_STOP


	/****************************/
	/* Post-process				*/
	/****************************/


	/****************************/
	/* Check result				*/
	/****************************/
	if ((osal_ret == expect_data) && (pm_state == OSAL_PM_STATE_PG))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMGETSTATE,
							OSAL_TEST_ID001,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID001);
		return;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID001);
		return;
	}
}
 
 
static void Test07_004_002()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_state_t pm_state;
	e_osal_pm_required_state_t req_state;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID002);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMGETSTATE,
														OSAL_TEST_ID002);
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID002);
		return;
	}

	R_OSAL_PmGetRequiredState(dev_handle, &req_state);
	if(req_state == OSAL_PM_REQUIRED_STATE_REQUIRED)
	{
		R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_CG, true);
	}
	else
	{
		R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
		R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_CG, true);
	}
	
	R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
	
	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmGetState(dev_handle, &pm_state);
	TIME_MEASURE_STOP


	/****************************/
	/* Post-process				*/
	/****************************/


	/****************************/
	/* Check result				*/
	/****************************/
	if ((osal_ret == expect_data) && (pm_state == OSAL_PM_STATE_CG))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMGETSTATE,
							OSAL_TEST_ID002,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID002);
		return;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID002);
		return;
	}
}
 
 
static void Test07_004_003()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_state_t pm_state;
	e_osal_pm_required_state_t req_state;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID003);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMGETSTATE,
														OSAL_TEST_ID003);
	/* Open device*/
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID003);
		return;
	}

	R_OSAL_PmGetRequiredState(dev_handle, &req_state);
	if(req_state == OSAL_PM_REQUIRED_STATE_REQUIRED)
	{
		R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_HP, true);
	}
	else
	{
		R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
		R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_HP, true);
	}
	
	R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmGetState(dev_handle, &pm_state);
	TIME_MEASURE_STOP


	/****************************/
	/* Post-process				*/
	/****************************/


	/****************************/
	/* Check result				*/
	/****************************/
	if ((osal_ret == expect_data) && (pm_state == OSAL_PM_STATE_ENABLED))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMGETSTATE,
							OSAL_TEST_ID003,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID003);
		return;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID003);
		return;
	}
}
 
 
static void Test07_004_004()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_state_t pm_state;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID004);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMGETSTATE,
														OSAL_TEST_ID004);
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID004);
		return;
	}

	R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
	R_OSAL_PmSetResetState(dev_handle, OSAL_PM_RESET_STATE_APPLIED);

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmGetState(dev_handle, &pm_state);
	TIME_MEASURE_STOP


	/****************************/
	/* Post-process				*/
	/****************************/


	/****************************/
	/* Check result				*/
	/****************************/
	if ((osal_ret == expect_data) && (pm_state == OSAL_PM_STATE_RESET))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMGETSTATE,
							OSAL_TEST_ID004,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID004);
		return;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID004);
		return;
	}
}
 
 
static void Test07_004_005()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_state_t pm_state;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID005);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMGETSTATE,
														OSAL_TEST_ID005);
	/* Open device*/
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID005);
		return;
	}

	R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
	R_OSAL_PmSetResetState(dev_handle, OSAL_PM_RESET_STATE_RELEASED);

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmGetState(dev_handle, &pm_state);
	TIME_MEASURE_STOP


	/****************************/
	/* Post-process				*/
	/****************************/


	/****************************/
	/* Check result				*/
	/****************************/
	if ((osal_ret == expect_data) && (pm_state == OSAL_PM_STATE_READY))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMGETSTATE,
							OSAL_TEST_ID005,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID005);
		return;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID005);
		return;
	}
}
 
 
static void Test07_004_006()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	e_osal_pm_state_t pm_state;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID006);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMGETSTATE,
														OSAL_TEST_ID006);

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmGetState(NULL, &pm_state);
	TIME_MEASURE_STOP


	/****************************/
	/* Post-process				*/
	/****************************/


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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMGETSTATE,
							OSAL_TEST_ID006,
							test_ret, opt_info);

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID006);
		return;
	}
}
 
 
static void Test07_004_007()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_state_t pm_state;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID007);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMGETSTATE,
														OSAL_TEST_ID007);
	/* Open device*/
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID007);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID007);
		return;
	}

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmGetState(dev_handle, &pm_state);
	TIME_MEASURE_STOP


	/****************************/
	/* Post-process				*/
	/****************************/


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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMGETSTATE,
							OSAL_TEST_ID007,
							test_ret, opt_info);


	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID007);
		return;
	}
}

static void Test07_004_008()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];
    bool deviceopen = true;
	osal_device_handle_t dev_handle;
	e_osal_pm_state_t pm_state;

    printf("This test: Device handle without power control returns OSAL_RETURN_FAIL\n");
    printf("However, PoC implemetation is not check power control, so returns OSAL_RETURN_OK\n");

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID008);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMGETSTATE,
														OSAL_TEST_ID008);
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("fbc_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
        deviceopen = false;
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID008);
		//return;
	}
    if(deviceopen == true){
        /****************************/
        /* Test execution			*/
        /****************************/
        TIME_MEASURE_INIT
        TIME_MEASURE_START
        osal_ret = R_OSAL_PmGetState(dev_handle, &pm_state);
        TIME_MEASURE_STOP


        /****************************/
        /* Post-process				*/
        /****************************/


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
        
        OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

        osal_test_result_output(OSAL_TEST_PM,
                                OSAL_TEST_PMGETSTATE,
                                OSAL_TEST_ID008,
                                test_ret, opt_info);

        osal_ret = R_OSAL_IoDeviceClose(dev_handle);
        if (osal_ret != OSAL_RETURN_OK)
        {
            OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
                OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID008);
            return;
        }
    }
	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID008);
		return;
	}
}

static void Test07_004_009()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID009);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMGETSTATE,
														OSAL_TEST_ID009);
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID009);
		return;
	}


	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmGetState(dev_handle, NULL );
	TIME_MEASURE_STOP


	/****************************/
	/* Post-process				*/
	/****************************/


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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMGETSTATE,
							OSAL_TEST_ID009,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID009);
		return;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID009);
		return;
	}
}
 
static void Test07_004_010()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_state_t pm_state;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID010);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMGETSTATE,
														OSAL_TEST_ID010);
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID010);
		return;
	}
	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	osal_ret = R_OSAL_Deinitialize();

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmGetState(dev_handle, &pm_state);
	TIME_MEASURE_STOP


	/****************************/
	/* Post-process				*/
	/****************************/

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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMGETSTATE,
							OSAL_TEST_ID010,
							test_ret, opt_info);
}

//static void Test07_004_011()
//{
	//int test_ret = OSAL_TEST_OK;

	//e_osal_return_t osal_ret;
	//e_osal_return_t expect_data;
	//char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	//osal_device_handle_t dev_handle;
	//e_osal_pm_state_t pm_state;

	///****************************/
	///* Pre-process				*/
	///****************************/
	///* Init device */
    //osal_ret = osal_init();
	//if (osal_ret != OSAL_RETURN_OK)
	//{
		//OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			//OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID011);
		//return;
	//}

	///* Get expect data */
	//expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														//OSAL_TEST_PMGETSTATE,
														//OSAL_TEST_ID011);
	///* Open device */
	//osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	//if (osal_ret != OSAL_RETURN_OK)
	//{
		//OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			//OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID011);
		//return;
	//}

	//R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
	//R_OSAL_PmSetResetState(dev_handle, OSAL_PM_RESET_STATE_APPLIED);

	///****************************/
	///* Test execution			*/
	///****************************/
	//g_stub_flag.R_PMA_GetReset = R_PMA_NG;

	//TIME_MEASURE_INIT
	//TIME_MEASURE_START
	//osal_ret = R_OSAL_PmGetState(dev_handle, &pm_state);
	//TIME_MEASURE_STOP

	//g_stub_flag.R_PMA_GetReset = 0;


	///****************************/
	///* Post-process				*/
	///****************************/


	///****************************/
	///* Check result				*/
	///****************************/
	//if (osal_ret == expect_data) 
	//{
		//test_ret = OSAL_TEST_OK;
	//}
	//else
	//{
		//test_ret = OSAL_TEST_NG;
	//}
	
	//OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	//osal_test_result_output(OSAL_TEST_PM,
							//OSAL_TEST_PMGETSTATE,
							//OSAL_TEST_ID011,
							//test_ret, opt_info);

	//osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	//if (osal_ret != OSAL_RETURN_OK)
	//{
		//OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			//OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID011);
		//return;
	//}
	//osal_ret = R_OSAL_Deinitialize();
	//if (osal_ret != OSAL_RETURN_OK)
	//{
		//OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			//OSAL_TEST_PM, OSAL_TEST_PMGETSTATE, OSAL_TEST_ID011);
		//return;
	//}
//}

static void Test07_005_001()
{
	int test_ret = OSAL_TEST_OK;
	e_osal_return_t osal_ret;
	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_policy_t policy;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = R_OSAL_Initialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID001);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMSETPOLICY,
														OSAL_TEST_ID001);
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("imp_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID001);
		(void)R_OSAL_Deinitialize();
		return;
	}

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	target_ret = R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_PG, true);
	TIME_MEASURE_STOP

	osal_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmSetRequiredState() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID001);
	}

	osal_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmSetRequiredState() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID001);
	}
	/****************************/
	/* Post-process				*/
	/****************************/
	osal_ret = R_OSAL_PmGetPolicy(dev_handle, &policy);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmGetPolicy() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID001);
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID001);
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID001);
	}

	/****************************/
	/* Check result				*/
	/****************************/
	if ((target_ret == expect_data) && (policy == OSAL_PM_POLICY_PG))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMSETPOLICY,
							OSAL_TEST_ID001,
							test_ret, opt_info);
}
 
 
static void Test07_005_002()
{
	int test_ret = OSAL_TEST_OK;
	e_osal_return_t osal_ret;
	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_policy_t policy;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = R_OSAL_Initialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID002);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMSETPOLICY,
														OSAL_TEST_ID002);
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("imp_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID002);
		(void)R_OSAL_Deinitialize();
		return;
	}

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	target_ret = R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_CG, true);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/
	osal_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmSetRequiredState() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID002);
	}

	osal_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmSetRequiredState() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID002);
	}

	osal_ret = R_OSAL_PmGetPolicy(dev_handle, &policy);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmGetPolicy() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID002);
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID002);
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID002);
	}

	/****************************/
	/* Check result				*/
	/****************************/
	if ((target_ret == expect_data) && (policy == OSAL_PM_POLICY_CG))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMSETPOLICY,
							OSAL_TEST_ID002,
							test_ret, opt_info);
}

static void Test07_005_003()
{
	int test_ret = OSAL_TEST_OK;
	e_osal_return_t osal_ret;
	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_policy_t policy;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = R_OSAL_Initialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID003);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMSETPOLICY,
														OSAL_TEST_ID003);
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("imp_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID003);
		(void)R_OSAL_Deinitialize();
		return;
	}

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	target_ret = R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_HP, true);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/

	osal_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmSetRequiredState() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID003);
	}

	osal_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmSetRequiredState() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID003);
	}

	osal_ret = R_OSAL_PmGetPolicy(dev_handle, &policy);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmGetPolicy() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID003);
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID003);
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID003);
	}

	/****************************/
	/* Check result				*/
	/****************************/
	if ((target_ret == expect_data) && (policy == OSAL_PM_POLICY_HP))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMSETPOLICY,
							OSAL_TEST_ID003,
							test_ret, opt_info);
}
 
 
static void Test07_005_004()
{
	int test_ret = OSAL_TEST_OK;
	e_osal_return_t osal_ret;
	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_policy_t policy;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = R_OSAL_Initialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID004);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMSETPOLICY,
														OSAL_TEST_ID004);
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("imp_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID004);
		(void)R_OSAL_Deinitialize();
		return;
	}

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	target_ret = R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_PG, false);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/

	osal_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmSetRequiredState() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID004);
	}

	osal_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmSetRequiredState() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID004);
	}

	osal_ret = R_OSAL_PmGetPolicy(dev_handle, &policy);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmGetPolicy() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID004);
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID004);
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID004);
	}

	/****************************/
	/* Check result				*/
	/****************************/
	if ((target_ret == expect_data) && (policy == OSAL_PM_POLICY_PG))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMSETPOLICY,
							OSAL_TEST_ID004,
							test_ret, opt_info);
}
 
 
static void Test07_005_005()
{
	int test_ret = OSAL_TEST_OK;
	e_osal_return_t osal_ret;
	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_policy_t policy;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = R_OSAL_Initialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID005);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMSETPOLICY,
														OSAL_TEST_ID005);
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("imp_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID005);
		(void)R_OSAL_Deinitialize();
		return;
	}

	osal_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmSetRequiredState() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID005);
	}

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	target_ret = R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_PG, true);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/

	osal_ret = R_OSAL_PmGetPolicy(dev_handle, &policy);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmGetPolicy() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID005);
	}

	/* Issue: R_OSAL_IoDeviceClose() return OSAL_RETURN_FAIL(R_PMA_UnlockHwa fail) */
	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID005);
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID005);
	}

	/****************************/
	/* Check result				*/
	/****************************/
	if ((target_ret == expect_data) && (policy == OSAL_PM_POLICY_PG))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMSETPOLICY,
							OSAL_TEST_ID005,
							test_ret, opt_info);
}

static void Test07_005_006()
{
	int test_ret = OSAL_TEST_OK;
	e_osal_return_t osal_ret;
	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = R_OSAL_Initialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID006);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMSETPOLICY,
														OSAL_TEST_ID006);

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	target_ret = R_OSAL_PmSetPolicy(NULL, OSAL_PM_POLICY_PG, true);
	TIME_MEASURE_STOP

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID006);
	}

	/****************************/
	/* Check result				*/
	/****************************/
	if (target_ret == expect_data) 
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMSETPOLICY,
							OSAL_TEST_ID006,
							test_ret, opt_info);
}
 
 
static void Test07_005_007()
{
	int test_ret = OSAL_TEST_OK;
	e_osal_return_t osal_ret;
	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = R_OSAL_Initialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID007);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMSETPOLICY,
														OSAL_TEST_ID007);
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("imp_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID007);
		(void)R_OSAL_Deinitialize();
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID007);
	}

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	target_ret = R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_PG, true);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/
	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID007);
	}

	/****************************/
	/* Check result				*/
	/****************************/
	if (target_ret == expect_data) 
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMSETPOLICY,
							OSAL_TEST_ID007,
							test_ret, opt_info);
}

static void Test07_005_008()
{
	int test_ret = OSAL_TEST_OK;
	e_osal_return_t osal_ret;
	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];
    bool deviceopen = true;
	osal_device_handle_t dev_handle;

    printf("This test: Device handle without power control returns OSAL_RETURN_FAIL \n");
    printf("However, PoC implemetation is not check power control, so returns OSAL_RETURN_OK\n");

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = R_OSAL_Initialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID008);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMSETPOLICY,
														OSAL_TEST_ID008);
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("fbc_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
        deviceopen = false;
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID008);
	}
    if(deviceopen == true){
        /****************************/
        /* Test execution			*/
        /****************************/
        TIME_MEASURE_INIT
        TIME_MEASURE_START
        target_ret = R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_PG, true);
        TIME_MEASURE_STOP

        /****************************/
        /* Check result				*/
        /****************************/
        if (target_ret == expect_data) 
        {
            test_ret = OSAL_TEST_OK;
        }
        else
        {
            test_ret = OSAL_TEST_NG;
        }
        
        OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));

        osal_test_result_output(OSAL_TEST_PM,
                                OSAL_TEST_PMSETPOLICY,
                                OSAL_TEST_ID008,
                                test_ret, opt_info);
                                
        /****************************/
        /* Post-process				*/
        /****************************/
        osal_ret = R_OSAL_IoDeviceClose(dev_handle);
        if (osal_ret != OSAL_RETURN_OK)
        {
            OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
                OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID008);
            return;
        }
    }
	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID008);
        return;
	}
}

static void Test07_005_009()
{
	int test_ret = OSAL_TEST_OK;
	e_osal_return_t osal_ret;
	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = R_OSAL_Initialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID009);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMSETPOLICY,
														OSAL_TEST_ID009);
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("imp_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID009);
		(void)R_OSAL_Deinitialize();
		return;
	}

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	target_ret = R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_INVALID, true);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/
	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID009);
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID009);
	}

	/****************************/
	/* Check result				*/
	/****************************/
	if (target_ret == expect_data) 
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMSETPOLICY,
							OSAL_TEST_ID009,
							test_ret, opt_info);
}

static void Test07_005_010()
{
	int test_ret = OSAL_TEST_OK;
	e_osal_return_t osal_ret;
	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = R_OSAL_Initialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID010);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMSETPOLICY,
														OSAL_TEST_ID010);
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("imp_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID010);
		(void)R_OSAL_Deinitialize();
		return;
	}

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	target_ret = R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_HP + 1, true);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/
	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID010);
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID010);
	}

	/****************************/
	/* Check result				*/
	/****************************/
	if (target_ret == expect_data) 
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMSETPOLICY,
							OSAL_TEST_ID010,
							test_ret, opt_info);
}

static void Test07_005_011()
{
	int test_ret = OSAL_TEST_OK;
	e_osal_return_t osal_ret;
	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = R_OSAL_Initialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID011);
		return;
	}
	memset(&dev_handle, 0, sizeof(osal_device_handle_t));
	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMSETPOLICY,
														OSAL_TEST_ID011);

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID011);
	}

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	target_ret = R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_PG, true);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/

	/****************************/
	/* Check result				*/
	/****************************/
	if (target_ret == expect_data) 
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMSETPOLICY,
							OSAL_TEST_ID011,
							test_ret, opt_info);
}

//static void Test07_005_012()
//{
	//int test_ret = OSAL_TEST_OK;
	//e_osal_return_t osal_ret;
	//e_osal_return_t target_ret;
	//e_osal_return_t expect_data;
	//char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	//osal_device_handle_t dev_handle;

	///****************************/
	///* Pre-process				*/
	///****************************/
	///* Init device */
    //osal_ret = R_OSAL_Initialize();
	//if (osal_ret != OSAL_RETURN_OK)
	//{
		//OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			//OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID012);
		//return;
	//}

	///* Get expect data */
	//expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														//OSAL_TEST_PMSETPOLICY,
														//OSAL_TEST_ID012);
	///* Open device */
	//osal_ret = R_OSAL_IoDeviceOpen("imp_00", &dev_handle);
	//if (osal_ret != OSAL_RETURN_OK)
	//{
		//OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			//OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID012);
		//(void)R_OSAL_Deinitialize();
		//return;
	//}

	///****************************/
	///* Test execution			*/
	///****************************/
	//g_stub_flag.R_PMA_SetPowerPolicy = R_PMA_NG;
	//TIME_MEASURE_INIT
	//TIME_MEASURE_START
	//target_ret = R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_PG, true);
	//TIME_MEASURE_STOP
	//g_stub_flag.R_PMA_SetPowerPolicy = 0;

	///****************************/
	///* Post-process				*/
	///****************************/
	//osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	//if (osal_ret != OSAL_RETURN_OK)
	//{
		//OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			//OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID012);
	//}

	//osal_ret = R_OSAL_Deinitialize();
	//if (osal_ret != OSAL_RETURN_OK)
	//{
		//OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			//OSAL_TEST_PM, OSAL_TEST_PMSETPOLICY, OSAL_TEST_ID012);
	//}

	///****************************/
	///* Check result				*/
	///****************************/
	//if (target_ret == expect_data) 
	//{
		//test_ret = OSAL_TEST_OK;
	//}
	//else
	//{
		//test_ret = OSAL_TEST_NG;
	//}
	
	//OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));

	//osal_test_result_output(OSAL_TEST_PM,
							//OSAL_TEST_PMSETPOLICY,
							//OSAL_TEST_ID012,
							//test_ret, opt_info);
//}

static void Test07_006_001()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_policy_t policy;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = R_OSAL_Initialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID001);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMGETPOLICY,
														OSAL_TEST_ID001);
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("imp_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID001);
		(void)R_OSAL_Deinitialize();
	}

	osal_ret = R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_PG, true);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmSetPolicy() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID001);
	}

	osal_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmSetRequiredState() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID001);
	}

	osal_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmSetRequiredState() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID001);
	}
	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	target_ret = R_OSAL_PmGetPolicy(dev_handle, &policy);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/
	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID001);
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID001);
	}

	/****************************/
	/* Check result				*/
	/****************************/
	if ((target_ret == expect_data) && (policy == OSAL_PM_POLICY_PG))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMGETPOLICY,
							OSAL_TEST_ID001,
							test_ret, opt_info);
} 
 
static void Test07_006_002()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_policy_t policy;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = R_OSAL_Initialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, 0);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMGETPOLICY,
														OSAL_TEST_ID002);
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("imp_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID002);
		(void)R_OSAL_Deinitialize();
	}
	
	osal_ret = R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_CG, true);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmSetPolicy() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID002);
	}

	osal_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmSetRequiredState() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID002);
	}

	osal_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmSetRequiredState() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID002);
	}

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	target_ret = R_OSAL_PmGetPolicy(dev_handle, &policy);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/
	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID002);
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID002);
	}

	/****************************/
	/* Check result				*/
	/****************************/
	if ((target_ret == expect_data) == (policy == OSAL_PM_POLICY_CG))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMGETPOLICY,
							OSAL_TEST_ID002,
							test_ret, opt_info);
} 
 
static void Test07_006_003()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_policy_t policy;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = R_OSAL_Initialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID003);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMGETPOLICY,
														OSAL_TEST_ID003);
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("imp_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID003);
		(void)R_OSAL_Deinitialize();
	}

	osal_ret = R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_HP, true);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmSetPolicy() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID003);
	}

	osal_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmSetRequiredState() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID003);
	}

	osal_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmSetRequiredState() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID003);
	}
	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	target_ret = R_OSAL_PmGetPolicy(dev_handle, &policy);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/
	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID003);
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID003);
	}

	/****************************/
	/* Check result				*/
	/****************************/
	if ((target_ret == expect_data) && (policy == OSAL_PM_POLICY_HP))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMGETPOLICY,
							OSAL_TEST_ID003,
							test_ret, opt_info);
}
 
static void Test07_006_004()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_policy_t policy;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = R_OSAL_Initialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID004);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMGETPOLICY,
														OSAL_TEST_ID004);
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("imp_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID004);
		(void)R_OSAL_Deinitialize();
	}

	osal_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmSetRequiredState() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID004);
	}

	osal_ret = R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_PG, true);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmSetPolicy() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID004);
	}

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	target_ret = R_OSAL_PmGetPolicy(dev_handle, &policy);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/
	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID004);
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID004);
	}

	/****************************/
	/* Check result				*/
	/****************************/
	if ((target_ret == expect_data) && (policy == OSAL_PM_POLICY_PG))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMGETPOLICY,
							OSAL_TEST_ID004,
							test_ret, opt_info);
}

static void Test07_006_005()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t		osal_ret;
	e_osal_return_t		expect_data;
	char				opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	e_osal_pm_policy_t policy;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID006);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMGETPOLICY,
														OSAL_TEST_ID005);

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmGetPolicy(NULL, &policy);
	TIME_MEASURE_STOP


	/****************************/
	/* Post-process				*/
	/****************************/


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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMGETPOLICY,
							OSAL_TEST_ID005,
							test_ret, opt_info);

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID006);
		return;
	}
} 
 
static void Test07_006_006()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t		osal_ret;
	e_osal_return_t		expect_data;
	char				opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_policy_t policy;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID006);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMGETPOLICY,
														OSAL_TEST_ID006);
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("imp_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID006);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmGetPolicy(dev_handle, &policy);
	TIME_MEASURE_STOP


	/****************************/
	/* Post-process				*/
	/****************************/


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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMGETPOLICY,
							OSAL_TEST_ID006,
							test_ret, opt_info);

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID006);
		return;
	}
} 

static void Test07_006_007()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t		osal_ret;
	e_osal_return_t		expect_data;
	char				opt_info[OSAL_TEST_OPT_INFO_BUFFER];
    bool deviceopen = true;
	osal_device_handle_t dev_handle;
	e_osal_pm_policy_t policy;

    printf("This test: Device handle without power control returns OSAL_RETURN_FAIL \n");
    printf("However, PoC implemetation is not check power control, so returns OSAL_RETURN_OK\n");

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID007);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMGETPOLICY,
														OSAL_TEST_ID007);
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("fbc_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
        deviceopen = false;
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID007);
		//return;
	}
    if(deviceopen == true){

        /****************************/
        /* Test execution			*/
        /****************************/
        TIME_MEASURE_INIT
        TIME_MEASURE_START
        osal_ret = R_OSAL_PmGetPolicy(dev_handle, &policy);
        TIME_MEASURE_STOP


        /****************************/
        /* Post-process				*/
        /****************************/


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
        
        OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

        osal_test_result_output(OSAL_TEST_PM,
                                OSAL_TEST_PMGETPOLICY,
                                OSAL_TEST_ID007,
                                test_ret, opt_info);

        osal_ret = R_OSAL_IoDeviceClose(dev_handle);
        if (osal_ret != OSAL_RETURN_OK)
        {
            OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
                OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID007);
            return;
        }
    }
	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID007);
		return;
	}
} 
 
static void Test07_006_008()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t		osal_ret;
	e_osal_return_t		expect_data;
	char				opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID008);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMGETPOLICY,
														OSAL_TEST_ID008);
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("imp_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID008);
		return;
	}

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmGetPolicy(dev_handle, NULL);
	TIME_MEASURE_STOP


	/****************************/
	/* Post-process				*/
	/****************************/


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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMGETPOLICY,
							OSAL_TEST_ID008,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID008);
		return;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID008);
		return;
	}
} 

static void Test07_006_009()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t		osal_ret;
	e_osal_return_t		expect_data;
	char				opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_policy_t policy;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID009);
		return;
	}
	memset(&dev_handle, 0, sizeof(osal_device_handle_t));
	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMGETPOLICY,
														OSAL_TEST_ID009);

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID009);
		return;
	}

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmGetPolicy(dev_handle, &policy);
	TIME_MEASURE_STOP


	/****************************/
	/* Post-process				*/
	/****************************/


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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMGETPOLICY,
							OSAL_TEST_ID009,
							test_ret, opt_info);
} 
 
//static void Test07_006_010()
//{
	//int test_ret = OSAL_TEST_OK;

	//e_osal_return_t		osal_ret;
	//e_osal_return_t		expect_data;
	//char				opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	//osal_device_handle_t dev_handle;
	//e_osal_pm_policy_t policy;

	///****************************/
	///* Pre-process				*/
	///****************************/
	///* Init device */
    //osal_ret = osal_init();
	//if (osal_ret != OSAL_RETURN_OK)
	//{
		//OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			//OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID010);
		//return;
	//}

	///* Get expect data */
	//expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														//OSAL_TEST_PMGETPOLICY,
														//OSAL_TEST_ID010);
	///* Open device */
	//osal_ret = R_OSAL_IoDeviceOpen("imp_00", &dev_handle);
	//if (osal_ret != OSAL_RETURN_OK)
	//{
		//OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			//OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID010);
		//return;
	//}

	///****************************/
	///* Test execution			*/
	///****************************/
	//g_stub_flag.R_PMA_GetPowerPolicy = R_PMA_NG;
	
	//TIME_MEASURE_INIT
	//TIME_MEASURE_START
	//osal_ret = R_OSAL_PmGetPolicy(dev_handle, &policy);
	//TIME_MEASURE_STOP

	//g_stub_flag.R_PMA_GetPowerPolicy = 0;

	///****************************/
	///* Post-process				*/
	///****************************/


	///****************************/
	///* Check result				*/
	///****************************/
	//if (osal_ret == expect_data) 
	//{
		//test_ret = OSAL_TEST_OK;
	//}
	//else
	//{
		//test_ret = OSAL_TEST_NG;
	//}
	
	//OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	//osal_test_result_output(OSAL_TEST_PM,
							//OSAL_TEST_PMGETPOLICY,
							//OSAL_TEST_ID010,
							//test_ret, opt_info);

	//osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	//if (osal_ret != OSAL_RETURN_OK)
	//{
		//OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			//OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID010);
		//return;
	//}
	//osal_ret = R_OSAL_Deinitialize();
	//if (osal_ret != OSAL_RETURN_OK)
	//{
		//OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			//OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID010);
		//return;
	//}
//}

static void Test07_007_001()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_reset_state_t reset_state;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID001);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMSETRESETSTATE,
														OSAL_TEST_ID001);
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID001);
		return;
	}

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmSetResetState(dev_handle, OSAL_PM_RESET_STATE_APPLIED);
	TIME_MEASURE_STOP


	/****************************/
	/* Post-process				*/
	/****************************/
	osal_ret = R_OSAL_PmGetResetState(dev_handle, &reset_state);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmGetResetState() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID001);
	}

	/****************************/
	/* Check result				*/
	/****************************/
	if ((osal_ret == expect_data) && (reset_state == OSAL_PM_RESET_STATE_APPLIED))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMSETRESETSTATE,
							OSAL_TEST_ID001,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID001);
		return;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID001);
		return;
	}
}
 
 
static void Test07_007_002()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_reset_state_t reset_state;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID002);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMSETRESETSTATE,
														OSAL_TEST_ID002);
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID002);
		return;
	}

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmSetResetState(dev_handle, OSAL_PM_RESET_STATE_RELEASED);
	TIME_MEASURE_STOP


	/****************************/
	/* Post-process				*/
	/****************************/
	osal_ret = R_OSAL_PmGetResetState(dev_handle, &reset_state);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmGetResetState() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID002);
	}

	/****************************/
	/* Check result				*/
	/****************************/
	if ((osal_ret == expect_data) && (reset_state == OSAL_PM_RESET_STATE_RELEASED))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMSETRESETSTATE,
							OSAL_TEST_ID002,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID002);
		return;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID002);
		return;
	}
}
 
 
static void Test07_007_003()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID003);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMSETRESETSTATE,
														OSAL_TEST_ID003);
	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmSetResetState(NULL, OSAL_PM_RESET_STATE_APPLIED);
	TIME_MEASURE_STOP


	/****************************/
	/* Post-process				*/
	/****************************/


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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMSETRESETSTATE,
							OSAL_TEST_ID003,
							test_ret, opt_info);

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID003);
		return;
	}
}
 
 
static void Test07_007_004()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t		osal_ret;
	e_osal_return_t		expect_data;
	char				opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID004);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMSETRESETSTATE,
														OSAL_TEST_ID004);
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID004);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);	/* Check pont */

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmSetResetState(dev_handle, OSAL_PM_RESET_STATE_APPLIED);
	TIME_MEASURE_STOP


	/****************************/
	/* Post-process				*/
	/****************************/


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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMSETRESETSTATE,
							OSAL_TEST_ID004,
							test_ret, opt_info);


	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID004);
		return;
	}
}
 
 
static void Test07_007_005()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t		osal_ret;
	e_osal_return_t		expect_data;
	char				opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	bool deviceopen = true;
	osal_device_handle_t dev_handle;

    printf("This test: Device handle without power control returns OSAL_RETURN_FAIL \n");
    printf("However, PoC implemetation is not check power control, so returns OSAL_RETURN_OK\n");

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID005);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMSETRESETSTATE,
														OSAL_TEST_ID005);
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("fbc_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		deviceopen = false;
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID005);
		//return;
	}
	if(deviceopen == true){
		/****************************/
		/* Test execution			*/
		/****************************/
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_PmSetResetState(dev_handle, OSAL_PM_RESET_STATE_APPLIED);
		TIME_MEASURE_STOP


		/****************************/
		/* Post-process				*/
		/****************************/


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
		
		OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

		osal_test_result_output(OSAL_TEST_PM,
								OSAL_TEST_PMSETRESETSTATE,
								OSAL_TEST_ID005,
								test_ret, opt_info);
		osal_ret = R_OSAL_IoDeviceClose(dev_handle);
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID005);
			return;
		}
	}
	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID005);
		return;
	}
}

static void Test07_007_006()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t		osal_ret;
	e_osal_return_t		expect_data;
	char				opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID006);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMSETRESETSTATE,
														OSAL_TEST_ID006);
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID006);
		return;
	}

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmSetResetState(dev_handle, OSAL_PM_RESET_STATE_INVALID);	/* Check point */
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/


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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMSETRESETSTATE,
							OSAL_TEST_ID006,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID006);
		return;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID006);
		return;
	}
}
 
 
static void Test07_007_007()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t		osal_ret;
	e_osal_return_t		expect_data;
	char				opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID007);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMSETRESETSTATE,
														OSAL_TEST_ID007);
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID007);
		return;
	}

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmSetResetState(dev_handle, OSAL_PM_RESET_STATE_RELEASED + 1); /* Check point */
	TIME_MEASURE_STOP


	/****************************/
	/* Post-process				*/
	/****************************/


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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMSETRESETSTATE,
							OSAL_TEST_ID007,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID007);
		return;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID007);
		return;
	}
}
 
static void Test07_007_008()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t		osal_ret;
	e_osal_return_t		expect_data;
	char				opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID008);
		return;
	}
	memset(&dev_handle, 0, sizeof(osal_device_handle_t));
	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMSETRESETSTATE,
														OSAL_TEST_ID008);
	osal_ret = R_OSAL_Deinitialize();	/* Check point */

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmSetResetState(dev_handle, OSAL_PM_RESET_STATE_APPLIED);
	TIME_MEASURE_STOP


	/****************************/
	/* Post-process				*/
	/****************************/


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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMSETRESETSTATE,
							OSAL_TEST_ID008,
							test_ret, opt_info);

}
 
//static void Test07_007_009()
//{
	//int test_ret = OSAL_TEST_OK;

	//e_osal_return_t		osal_ret;
	//e_osal_return_t		expect_data;
	//char				opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	//osal_device_handle_t dev_handle;

	///****************************/
	///* Pre-process				*/
	///****************************/
	///* Init device */
    //osal_ret = osal_init();
	//if (osal_ret != OSAL_RETURN_OK)
	//{
		//OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			//OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID009);
		//return;
	//}

	///* Get expect data */
	//expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														//OSAL_TEST_PMSETRESETSTATE,
														//OSAL_TEST_ID009);
	///* Open device */
	//osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	//if (osal_ret != OSAL_RETURN_OK)
	//{
		//OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			//OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID009);
		//return;
	//}

	///****************************/
	///* Test execution			*/
	///****************************/
	//g_stub_flag.SemaphoreTake = SEMAPHORETAKE_pdFAIL;

	//TIME_MEASURE_INIT
	//TIME_MEASURE_START
	//osal_ret = R_OSAL_PmSetResetState(dev_handle, OSAL_PM_RESET_STATE_APPLIED);
	//TIME_MEASURE_STOP

	//g_stub_flag.SemaphoreTake = 0;

	///****************************/
	///* Post-process				*/
	///****************************/


	///****************************/
	///* Check result				*/
	///****************************/
	//if (osal_ret == expect_data) 
	//{
		//test_ret = OSAL_TEST_OK;
	//}
	//else
	//{
		//test_ret = OSAL_TEST_NG;
	//}
	
	//OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	//osal_test_result_output(OSAL_TEST_PM,
							//OSAL_TEST_PMSETRESETSTATE,
							//OSAL_TEST_ID009,
							//test_ret, opt_info);

	//osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	//if (osal_ret != OSAL_RETURN_OK)
	//{
		//OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			//OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID009);
		//return;
	//}
	//osal_ret = R_OSAL_Deinitialize();
	//if (osal_ret != OSAL_RETURN_OK)
	//{
		//OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			//OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID009);
		//return;
	//}
//}

//static void Test07_007_010()
//{
	//int test_ret = OSAL_TEST_OK;

	//e_osal_return_t		osal_ret;
	//e_osal_return_t		expect_data;
	//char				opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	//osal_device_handle_t dev_handle;

	///****************************/
	///* Pre-process				*/
	///****************************/
	///* Init device */
    //osal_ret = osal_init();
	//if (osal_ret != OSAL_RETURN_OK)
	//{
		//OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			//OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID010);
		//return;
	//}

	///* Get expect data */
	//expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														//OSAL_TEST_PMSETRESETSTATE,
														//OSAL_TEST_ID010);
	///* Open device */
	//osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	//if (osal_ret != OSAL_RETURN_OK)
	//{
		//OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			//OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID010);
		//return;
	//}

	///****************************/
	///* Test execution			*/
	///****************************/
	//g_stub_flag.R_PMA_ApplyResetHwa = R_PMA_RESET_CHK_NG;

	//TIME_MEASURE_INIT
	//TIME_MEASURE_START
	//osal_ret = R_OSAL_PmSetResetState(dev_handle, OSAL_PM_RESET_STATE_APPLIED);
	//TIME_MEASURE_STOP

	//g_stub_flag.R_PMA_ApplyResetHwa = 0;

	///****************************/
	///* Post-process				*/
	///****************************/


	///****************************/
	///* Check result				*/
	///****************************/
	//if (osal_ret == expect_data) 
	//{
		//test_ret = OSAL_TEST_OK;
	//}
	//else
	//{
		//test_ret = OSAL_TEST_NG;
	//}
	
	//OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	//osal_test_result_output(OSAL_TEST_PM,
							//OSAL_TEST_PMSETRESETSTATE,
							//OSAL_TEST_ID010,
							//test_ret, opt_info);

	//osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	//if (osal_ret != OSAL_RETURN_OK)
	//{
		//OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			//OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID010);
		//return;
	//}
	//osal_ret = R_OSAL_Deinitialize();
	//if (osal_ret != OSAL_RETURN_OK)
	//{
		//OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			//OSAL_TEST_PM, OSAL_TEST_PMSETRESETSTATE, OSAL_TEST_ID010);
		//return;
	//}
//}

static void Test07_008_001()
{
	int test_ret = OSAL_TEST_OK;
	e_osal_return_t osal_ret;
	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_reset_state_t reset_state;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = R_OSAL_Initialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETRESETSTATE, OSAL_TEST_ID001);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMGETRESETSTATE,
														OSAL_TEST_ID001);
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETRESETSTATE, OSAL_TEST_ID001);
		(void)R_OSAL_Deinitialize();
		return;
	}

	IT_DEBUG("R_OSAL_PmSetRequiredState() with OSAL_PM_REQUIRED_STATE_REQUIRED\n");
	osal_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmSetRequiredState() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETRESETSTATE, OSAL_TEST_ID001);
	}
	IT_DEBUG("R_OSAL_PmSetResetState() with OSAL_PM_RESET_STATE_APPLIED\n");
	osal_ret = R_OSAL_PmSetResetState(dev_handle, OSAL_PM_RESET_STATE_APPLIED);
		if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_PmSetResetState() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETRESETSTATE, OSAL_TEST_ID001);
	}

	/****************************/
	/* Test execution			*/
	/****************************/
	IT_DEBUG("R_OSAL_PmGetResetState() test execution\n");
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	target_ret = R_OSAL_PmGetResetState(dev_handle, &reset_state);
	TIME_MEASURE_STOP
	IT_DEBUG("R_OSAL_PmGetResetState() *p_state:%d\n", reset_state);
	/****************************/
	/* Post-process				*/
	/****************************/
	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETRESETSTATE, OSAL_TEST_ID001);
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETRESETSTATE, OSAL_TEST_ID001);
	}

	/****************************/
	/* Check result				*/
	/****************************/
	if ((target_ret == expect_data) && (reset_state == OSAL_PM_RESET_STATE_APPLIED))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMGETRESETSTATE,
							OSAL_TEST_ID001,
							test_ret, opt_info);
}

static void Test07_008_002()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t		osal_ret;
	e_osal_return_t		expect_data;
	char				opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_reset_state_t reset_state;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETRESETSTATE, OSAL_TEST_ID002);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMGETRESETSTATE,
														OSAL_TEST_ID002);
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETRESETSTATE, OSAL_TEST_ID002);
		return;
	}

	R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
	R_OSAL_PmSetResetState(dev_handle, OSAL_PM_RESET_STATE_RELEASED);

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmGetResetState(dev_handle, &reset_state);
	TIME_MEASURE_STOP


	/****************************/
	/* Post-process				*/
	/****************************/


	/****************************/
	/* Check result				*/
	/****************************/
	if ((osal_ret == expect_data) && (reset_state == OSAL_PM_RESET_STATE_RELEASED))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMGETRESETSTATE,
							OSAL_TEST_ID002,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETRESETSTATE, OSAL_TEST_ID002);
		return;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETRESETSTATE, OSAL_TEST_ID002);
		return;
	}
}

 
 
static void Test07_008_003()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t		osal_ret;
	e_osal_return_t		expect_data;
	char				opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_reset_state_t reset_state;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETRESETSTATE, OSAL_TEST_ID003);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMGETRESETSTATE,
														OSAL_TEST_ID003);
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETRESETSTATE, OSAL_TEST_ID003);
		return;
	}

	R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
	R_OSAL_PmSetResetState(dev_handle, OSAL_PM_RESET_STATE_RELEASED);

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmGetResetState(dev_handle, &reset_state);
	TIME_MEASURE_STOP


	/****************************/
	/* Post-process				*/
	/****************************/


	/****************************/
	/* Check result				*/
	/****************************/
	if ((osal_ret == expect_data) && (reset_state == OSAL_PM_RESET_STATE_RELEASED))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMGETRESETSTATE,
							OSAL_TEST_ID003,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETRESETSTATE, OSAL_TEST_ID003);
		return;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETRESETSTATE, OSAL_TEST_ID003);
		return;
	}
}

 
 
static void Test07_008_004()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t		osal_ret;
	e_osal_return_t		expect_data;
	char				opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	e_osal_pm_reset_state_t reset_state;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETRESETSTATE, OSAL_TEST_ID004);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMGETRESETSTATE,
														OSAL_TEST_ID004);

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmGetResetState(NULL, &reset_state); /* Check point */
	TIME_MEASURE_STOP


	/****************************/
	/* Post-process				*/
	/****************************/


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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMGETRESETSTATE,
							OSAL_TEST_ID004,
							test_ret, opt_info);

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETRESETSTATE, OSAL_TEST_ID004);
		return;
	}
}

static void Test07_008_005()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t		osal_ret;
	e_osal_return_t		expect_data;
	char				opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_reset_state_t reset_state;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETRESETSTATE, OSAL_TEST_ID005);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMGETRESETSTATE,
														OSAL_TEST_ID005);
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETRESETSTATE, OSAL_TEST_ID005);
		return;
	}
	osal_ret = R_OSAL_IoDeviceClose(dev_handle);	/* Check point */

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmGetResetState(dev_handle, &reset_state);
	TIME_MEASURE_STOP


	/****************************/
	/* Post-process				*/
	/****************************/


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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMGETRESETSTATE,
							OSAL_TEST_ID005,
							test_ret, opt_info);

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETRESETSTATE, OSAL_TEST_ID005);
		return;
	}
}

static void Test07_008_006()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t		osal_ret;
	e_osal_return_t		expect_data;
	char				opt_info[OSAL_TEST_OPT_INFO_BUFFER];
    bool deviceopen = true;
	osal_device_handle_t dev_handle;
	e_osal_pm_reset_state_t reset_state;

    printf("This test: Device handle without power control returns OSAL_RETURN_FAIL\n");
    printf("However, PoC implemetation is not check power control, so returns OSAL_RETURN_OK\n");

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETRESETSTATE, OSAL_TEST_ID006);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMGETRESETSTATE,
														OSAL_TEST_ID006);
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("fbc_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
        deviceopen = false;
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETRESETSTATE, OSAL_TEST_ID006);
		//return;
	}
    if(deviceopen == true){
        /****************************/
        /* Test execution			*/
        /****************************/
        TIME_MEASURE_INIT
        TIME_MEASURE_START
        osal_ret = R_OSAL_PmGetResetState(dev_handle, &reset_state); /* Check point */
        TIME_MEASURE_STOP


        /****************************/
        /* Post-process				*/
        /****************************/


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
        
        OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

        osal_test_result_output(OSAL_TEST_PM,
                                OSAL_TEST_PMGETRESETSTATE,
                                OSAL_TEST_ID006,
                                test_ret, opt_info);

        osal_ret = R_OSAL_IoDeviceClose(dev_handle);
        if (osal_ret != OSAL_RETURN_OK)
        {
            OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
                OSAL_TEST_PM, OSAL_TEST_PMGETRESETSTATE, OSAL_TEST_ID006);
            return;
        }
    }
	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETRESETSTATE, OSAL_TEST_ID006);
		return;
	}
}

static void Test07_008_007()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t		osal_ret;
	e_osal_return_t		expect_data;
	char				opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	// e_osal_pm_reset_state_t reset_state;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETRESETSTATE, OSAL_TEST_ID007);
		return;
	}

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMGETRESETSTATE,
														OSAL_TEST_ID007);
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETRESETSTATE, OSAL_TEST_ID007);
		return;
	}
	
	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmGetResetState(dev_handle, NULL);
	TIME_MEASURE_STOP


	/****************************/
	/* Post-process				*/
	/****************************/


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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMGETRESETSTATE,
							OSAL_TEST_ID007,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETRESETSTATE, OSAL_TEST_ID007);
		return;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETRESETSTATE, OSAL_TEST_ID007);
		return;
	}
}


static void Test07_008_008()
{
	int test_ret = OSAL_TEST_OK;

	e_osal_return_t		osal_ret;
	e_osal_return_t		expect_data;
	char				opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_reset_state_t reset_state;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
    osal_ret = osal_init();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_PM, OSAL_TEST_PMGETRESETSTATE, OSAL_TEST_ID009);
		return;
	}
	
	memset(&dev_handle, 0, sizeof(osal_device_handle_t));

	/* Get expect data */
	expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMGETRESETSTATE,
														OSAL_TEST_ID008);
	osal_ret = R_OSAL_Deinitialize();	/* Check point */

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_PmGetResetState(dev_handle, &reset_state);
	TIME_MEASURE_STOP


	/****************************/
	/* Post-process				*/
	/****************************/


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
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMGETRESETSTATE,
							OSAL_TEST_ID008,
							test_ret, opt_info);
}

//static void Test07_008_009()
//{
	//int test_ret = OSAL_TEST_OK;

	//e_osal_return_t		osal_ret;
	//e_osal_return_t		expect_data;
	//char				opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	//osal_device_handle_t dev_handle;
	//e_osal_pm_reset_state_t reset_state;

	///****************************/
	///* Pre-process				*/
	///****************************/
	///* Init device */
    //osal_ret = osal_init();
	//if (osal_ret != OSAL_RETURN_OK)
	//{
		//OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			//OSAL_TEST_PM, OSAL_TEST_PMGETRESETSTATE, OSAL_TEST_ID009);
		//return;
	//}

	///* Get expect data */
	//expect_data =  (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														//OSAL_TEST_PMGETRESETSTATE,
														//OSAL_TEST_ID009);
	///* Open device */
	//osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	//if (osal_ret != OSAL_RETURN_OK)
	//{
		//OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
			//OSAL_TEST_PM, OSAL_TEST_PMGETRESETSTATE, OSAL_TEST_ID009);
		//return;
	//}

	///****************************/
	///* Test execution			*/
	///****************************/
	//g_stub_flag.R_PMA_GetReset = R_PMA_RESET_CHK_NG;

	//TIME_MEASURE_INIT
	//TIME_MEASURE_START
	//osal_ret = R_OSAL_PmGetResetState(dev_handle, &reset_state);
	//TIME_MEASURE_STOP

	//g_stub_flag.R_PMA_GetReset = 0;

	///****************************/
	///* Post-process				*/
	///****************************/


	///****************************/
	///* Check result				*/
	///****************************/
	//if (osal_ret == expect_data) 
	//{
		//test_ret = OSAL_TEST_OK;
	//}
	//else
	//{
		//test_ret = OSAL_TEST_NG;
	//}
	
	//OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	//osal_test_result_output(OSAL_TEST_PM,
							//OSAL_TEST_PMGETRESETSTATE,
							//OSAL_TEST_ID009,
							//test_ret, opt_info);

	//osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	//osal_ret = R_OSAL_Deinitialize();
//}

static st_pma_handle_t pm_handle;


static void Test07_009_001(){

	int test_ret = OSAL_TEST_OK;

        e_pma_return_t osal_ret;
//        static st_pma_handle_t pm_handle;
        static st_pma_config_t pm_config;

	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t dev_handle;
	e_osal_pm_required_state_t req_state;

	/****************************/
	/* Pre-process				*/
	/****************************/
        target_ret = R_OSAL_Initialize();
        if (target_ret != OSAL_RETURN_OK) {
            printf("R_OSAL_Initialize NG : %d\n", target_ret);
        }

        pm_config.task_priority = OSAL_THREAD_PRIORITY_TYPE10;
        pm_config.timeout_ms = 500;
        pm_config.polling_max_num = 10;
        pm_config.loop_num_for_rclk_cycle = 3;

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
        osal_ret = R_PMA_Init(&pm_handle, &pm_config);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/

	/****************************/
	/* Check result				*/
	/****************************/
//	if ((target_ret == expect_data) && (req_state == OSAL_PM_REQUIRED_STATE_REQUIRED))
	if (osal_ret == R_PMA_RESULT_OK)
	{
		test_ret = OSAL_TEST_OK;
                target_ret = OSAL_RETURN_OK;

	}
	else
	{
		test_ret = OSAL_TEST_NG;
                target_ret = OSAL_RETURN_PAR;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));

//	osal_test_result_output(TEST07_009_001, test_ret, opt_info);
        osal_test_result_output(OSAL_TEST_PM,
                                                        OSAL_TEST_PM_OSAL_PM_API_INIT,
                                                        OSAL_TEST_ID001,
                                                        test_ret, opt_info);

}



static void Test07_009_002(){

        int test_ret = OSAL_TEST_OK;

        e_pma_return_t osal_ret;
        static st_pma_config_t pm_config;

        e_osal_return_t target_ret;
        e_osal_return_t expect_data;
        char opt_info[OSAL_TEST_OPT_INFO_BUFFER];

        osal_device_handle_t dev_handle;
        e_osal_pm_required_state_t req_state;

        /****************************/
        /* Pre-process                          */
        /****************************/
//        target_ret = R_OSAL_Initialize();
//        if (target_ret != OSAL_RETURN_OK) {
//            printf("R_OSAL_Initialize NG : %d\n", target_ret);
//        }

//        pm_config.task_priority = 10;
//        pm_config.timeout_ms = 500;
//        pm_config.polling_max_num = 10;
//        pm_config.loop_num_for_rclk_cycle = 3;

        /****************************/
        /* Test execution                       */
        /****************************/
        TIME_MEASURE_INIT
        TIME_MEASURE_START
        osal_ret = R_PMA_Quit(pm_handle);
        TIME_MEASURE_STOP

        /****************************/
        /* Post-process                         */
        /****************************/

        /****************************/
        /* Check result                         */
        /****************************/
//      if ((target_ret == expect_data) && (req_state == OSAL_PM_REQUIRED_STATE_REQUIRED))
        if (osal_ret == R_PMA_RESULT_OK)
        {
                test_ret = OSAL_TEST_OK;
                target_ret = OSAL_RETURN_OK;

        }
        else
        {
                test_ret = OSAL_TEST_NG;
                target_ret = OSAL_RETURN_PAR;
        }

        OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));

//      osal_test_result_output(TEST07_009_001, test_ret, opt_info);
        osal_test_result_output(OSAL_TEST_PM,
                                                        OSAL_TEST_PM_OSAL_PM_API_INIT,
                                                        OSAL_TEST_ID002,
                                                        test_ret, opt_info);
}



/* 07_001: R_OSAL_PmSetRequiredState */
static void osal_test_R_OSAL_PmSetRequiredState()
{
	Test07_001_001();
	Test07_001_002();
	Test07_001_003();
	Test07_001_004();
	Test07_001_005();
	Test07_001_006();
	Test07_001_007();
	Test07_001_008();
	Test07_001_009();
	//Test07_001_010();
	//Test07_001_011();

}

/* 07_002: R_OSAL_PmGetRequiredState */
static void osal_test_R_OSAL_PmGetRequiredState()
{
	Test07_002_001();
	Test07_002_002();
	Test07_002_003();
	Test07_002_004();
	Test07_002_005();
	Test07_002_006();
	Test07_002_007();
	//Test07_002_008();

}

/* 07_003: R_OSAL_PmWaitForState */
static void osal_test_R_OSAL_PmWaitForState()
{
	Test07_003_001();
	Test07_003_002();
	Test07_003_003();
	Test07_003_004();
	Test07_003_005();
	Test07_003_006();
	Test07_003_007();
	Test07_003_008();
	Test07_003_009();
	Test07_003_010();
	Test07_003_011();
	Test07_003_012();
	Test07_003_013();
	Test07_003_014();
	Test07_003_015();
	Test07_003_016();
	Test07_003_017();
	Test07_003_018();
	//Test07_003_019();
}

/* 07_004: R_OSAL_PmGetState */
static void osal_test_R_OSAL_PmGetState()
{

	Test07_004_001();
	Test07_004_002();
	Test07_004_003();
	Test07_004_004();
	Test07_004_005();
	Test07_004_006();
	Test07_004_007();
	Test07_004_008();
	Test07_004_009();
	Test07_004_010();
	//Test07_004_011();
}

/* 07_005: R_OSAL_PmSetPolicy */
static void osal_test_R_OSAL_PmSetPolicy()
{
	Test07_005_001();
	Test07_005_002();
	Test07_005_003();
	Test07_005_004();
	Test07_005_005();
	Test07_005_006();
	Test07_005_007();
	Test07_005_008();
	Test07_005_009();
	Test07_005_010();
	Test07_005_011();
	//Test07_005_012();
}

/* 07_006: R_OSAL_PmGetPolicy */
static void osal_test_R_OSAL_PmGetPolicy()
{
	Test07_006_001();
	Test07_006_002();
	Test07_006_003();
	Test07_006_004();
	Test07_006_005();
	Test07_006_006();
	Test07_006_007();
	Test07_006_008();
	Test07_006_009();
	//Test07_006_010();

}

/* 07_007: R_OSAL_PmSetResetState */
static void osal_test_R_OSAL_PmSetResetState()
{
	Test07_007_001();
	Test07_007_002();
	Test07_007_003();
	Test07_007_004();
	Test07_007_005();
	Test07_007_006();
	Test07_007_007();
	Test07_007_008();
	//Test07_007_009();
	//Test07_007_010();
}

/* 07_008: R_OSAL_PmGetResetState */
static void osal_test_R_OSAL_PmGetResetState()
{
	Test07_008_001();
	Test07_008_002();
	Test07_008_003();
	Test07_008_004();
	Test07_008_005();
	Test07_008_006();
	Test07_008_007();
	Test07_008_008();
	//Test07_008_009();
}

static void osal_test_R_PMA_Init()
{

// tentative
//        Test07_009_001();
// R_OSAL_ThreadSleepForTimePeriod(1200);
//        Test07_009_002();

}

