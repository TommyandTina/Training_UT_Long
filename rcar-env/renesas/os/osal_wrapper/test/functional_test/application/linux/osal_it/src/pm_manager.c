/**
 * @file pm_manager.c
 * @brief OSAL PM Manager evaluation file.
 * @author K.Takagi
 * @date 2019/12/02
 */

/* include */
#include "osal_test.h"
#include "osal_test_common.h"
#include "pm_manager.h"

/* prototype */
static void osal_test_R_OSAL_PmSetRequiredState();
static void osal_test_R_OSAL_PmGetRequiredState();
static void osal_test_R_OSAL_PmWaitForState();
static void osal_test_R_OSAL_PmGetState();
static void osal_test_R_OSAL_PmSetPolicy();
static void osal_test_R_OSAL_PmGetPolicy();
static void osal_test_R_OSAL_PmSetResetState();
static void osal_test_R_OSAL_PmGetResetState();
static void osal_test_R_OSAL_PmSetPostClock();
static void osal_test_R_OSAL_PmGetLowestIntermediatePowerState();

void osal_test_pm_manager_display_menu()
{
	printf("*** OSAL PM Manager ***\n");
	printf("***  07_001: R_OSAL_PmSetRequiredState                    ***\n");
	printf("***  07_002: R_OSAL_PmGetRequiredState                    ***\n");
	printf("***  07_003: R_OSAL_PmWaitForState                        ***\n");
	printf("***  07_004: R_OSAL_PmGetState                            ***\n");
	printf("***  07_005: R_OSAL_PmSetPolicy                           ***\n");
	printf("***  07_006: R_OSAL_PmGetPolicy                           ***\n");
	printf("***  07_007: R_OSAL_PmSetResetState                       ***\n");
	printf("***  07_008: R_OSAL_PmGetResetState                       ***\n");
	printf("***  07_009: R_OSAL_PmSetPostClock                        ***\n");
	printf("***  07_010: R_OSAL_PmGetLowestIntermediatePowerState     ***\n");
	printf("***  07_100: pm manager module all exec ***\n\n");
	return;
}

void osal_test_pm_manager_menu(int child)
{
	int test_number = child;
	unsigned int test_count = 1;
	unsigned int loop_count = 0;

	/* ALL check */
	/* when select all */
	if (child == OSAL_TEST_ALL_EXEC)
	{
		test_number	= OSAL_TEST_PMSETREQUIREDSTATE;
		test_count	= OSAL_TEST_PM_MAX;
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
			case OSAL_TEST_PMWAITFORSTATE:
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
			case OSAL_TEST_PMSETPOSTCLOCK:
				osal_test_R_OSAL_PmSetPostClock();
				break;
			case OSAL_TEST_PMGETLOWESTINTERMEDIATEPOWERSTATE:
				osal_test_R_OSAL_PmGetLowestIntermediatePowerState();
				break;
			default:
				printf("That menu is not available.\n\n");
				break;
		}
		test_number++;
	}
	return;
}

#if defined(RCAR_V4H) || defined(RCAR_V4M)
/* ------------------------07_003_001 [OSAL_RETURN_OK]------------------------ */
static void Test07_003_001()
{
	int						test_ret = OSAL_TEST_OK;
	e_osal_return_t			osal_ret;
	e_osal_return_t			expect_data;
	char					opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t	handle;
	const char				device_id[]	= "vdsp00_02";

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMWAITFORSTATE,
														OSAL_TEST_ID001);

	osal_ret = R_OSAL_Initialize();
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_PM, OSAL_TEST_PMWAITFORSTATE, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}

	if (OSAL_RETURN_OK == osal_ret)
	{
		osal_ret = R_OSAL_IoDeviceOpen(device_id, &handle);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
					OSAL_TEST_PM, OSAL_TEST_PMWAITFORSTATE, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	/* Initialize -> READY */
	{
		if (OSAL_RETURN_OK == osal_ret)
		{
			osal_ret = R_OSAL_PmSetResetState(handle, OSAL_PM_RESET_STATE_RELEASED);
			if (OSAL_RETURN_OK != osal_ret)
			{
				OSAL_TEST_ERR("R_OSAL_PmSetResetState() error.",
						OSAL_TEST_PM, OSAL_TEST_PMWAITFORSTATE, OSAL_TEST_ID001);
				test_ret = OSAL_TEST_NG;
			}
		}

		if (OSAL_RETURN_OK == osal_ret)
		{
			osal_ret = R_OSAL_PmWaitForState(handle, OSAL_PM_STATE_READY, 100);
			if (OSAL_RETURN_OK != osal_ret)
			{
				OSAL_TEST_ERR("R_OSAL_PmWaitForState() error.",
						OSAL_TEST_PM, OSAL_TEST_PMWAITFORSTATE, OSAL_TEST_ID001);
				test_ret = OSAL_TEST_NG;
			}
		}
	}

	/* READY -> RESET */
	{
		if (OSAL_RETURN_OK == osal_ret)
		{
			osal_ret = R_OSAL_PmSetResetState(handle, OSAL_PM_RESET_STATE_APPLIED);
			if (OSAL_RETURN_OK != osal_ret)
			{
				OSAL_TEST_ERR("R_OSAL_PmSetResetState() error.",
						OSAL_TEST_PM, OSAL_TEST_PMWAITFORSTATE, OSAL_TEST_ID001);
				test_ret = OSAL_TEST_NG;
			}
		}

		if (OSAL_RETURN_OK == osal_ret)
		{
			osal_ret = R_OSAL_PmWaitForState(handle, OSAL_PM_STATE_RESET, 100);
			if (OSAL_RETURN_OK != osal_ret)
			{
				OSAL_TEST_ERR("R_OSAL_PmWaitForState() error.",
						OSAL_TEST_PM, OSAL_TEST_PMWAITFORSTATE, OSAL_TEST_ID001);
				test_ret = OSAL_TEST_NG;
			}
		}
	}

	/* RESET -> READY */
	{
		if (OSAL_RETURN_OK == osal_ret)
		{
			osal_ret = R_OSAL_PmSetResetState(handle, OSAL_PM_RESET_STATE_RELEASED);
			if (OSAL_RETURN_OK != osal_ret)
			{
				OSAL_TEST_ERR("R_OSAL_PmSetResetState() error.",
						OSAL_TEST_PM, OSAL_TEST_PMWAITFORSTATE, OSAL_TEST_ID001);
				test_ret = OSAL_TEST_NG;
			}
		}

		if (OSAL_RETURN_OK == osal_ret)
		{
			osal_ret = R_OSAL_PmWaitForState(handle, OSAL_PM_STATE_READY, 100);
			if (OSAL_RETURN_OK != osal_ret)
			{
				OSAL_TEST_ERR("R_OSAL_PmWaitForState() error.",
						OSAL_TEST_PM, OSAL_TEST_PMWAITFORSTATE, OSAL_TEST_ID001);
				test_ret = OSAL_TEST_NG;
			}
		}
	}
	TIME_MEASURE_STOP
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_PM, OSAL_TEST_PMWAITFORSTATE, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_PM, OSAL_TEST_PMWAITFORSTATE, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}

	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMWAITFORSTATE,
							OSAL_TEST_ID001, test_ret, opt_info);
	return;
}
#endif

/* ------------------------07_006_001 [OSAL_RETURN_OK]------------------------ */
static void Test07_006_001()
{
	int						test_ret = OSAL_TEST_OK;
	e_osal_return_t			osal_ret;
	e_osal_return_t			expect_data;
	char					opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t	handle;
	e_osal_pm_policy_t		policy;

#if defined(RCAR_S4)
	const char				device_id[]	= "caiplite_00";
#else
	const char				device_id[]	= "imp_top_00";
#endif

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_PM,
														OSAL_TEST_PMGETPOLICY,
														OSAL_TEST_ID001);

	osal_ret = R_OSAL_Initialize();
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}

	// SW policy:OSAL_PM_POLICY_CG, HW policy: OSAL_PM_POLICY_HP
	{
		osal_ret = R_OSAL_IoDeviceOpen(device_id, &handle);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
					OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}

		if (OSAL_RETURN_OK == osal_ret)
		{
			osal_ret = R_OSAL_PmSetRequiredState(handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
			if (OSAL_RETURN_OK != osal_ret)
			{
				OSAL_TEST_ERR("R_OSAL_PmSetRequiredState() error.",
						OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID001);
				test_ret = OSAL_TEST_NG;
			}
		}

		if (OSAL_RETURN_OK == osal_ret)
		{
			osal_ret = R_OSAL_PmSetPolicy(handle, OSAL_PM_POLICY_HP, true);
			if (OSAL_RETURN_OK != osal_ret)
			{
				OSAL_TEST_ERR("R_OSAL_PmSetPolicy() error.",
						OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID001);
				test_ret = OSAL_TEST_NG;
			}
		}

		if (OSAL_RETURN_OK == osal_ret)
		{
			osal_ret = R_OSAL_PmSetRequiredState(handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
			if (OSAL_RETURN_OK != osal_ret)
			{
				OSAL_TEST_ERR("R_OSAL_PmSetRequiredState() error.",
						OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID001);
				test_ret = OSAL_TEST_NG;
			}
		}

		if (OSAL_RETURN_OK == osal_ret)
		{
			osal_ret = R_OSAL_PmSetPolicy(handle, OSAL_PM_POLICY_CG, true);
			if (OSAL_RETURN_OK != osal_ret)
			{
				OSAL_TEST_ERR("R_OSAL_PmSetPolicy() error.",
						OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID001);
				test_ret = OSAL_TEST_NG;
			}
		}

		if (OSAL_RETURN_OK == osal_ret)
		{
			osal_ret = R_OSAL_IoDeviceClose(handle);
			if (OSAL_RETURN_OK != osal_ret)
			{
				OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
						OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID001);
				test_ret = OSAL_TEST_NG;
			}
		}
	}

	// Check the SW policy
	{
		osal_ret = R_OSAL_IoDeviceOpen(device_id, &handle);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
					OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}

		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_PmGetPolicy(handle, &policy);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_PmGetPolicy() error.",
					OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}

		if (OSAL_PM_POLICY_CG != policy)
		{
			OSAL_TEST_ERR("policy is not OSAL_PM_POLICY_CG",
					OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID001);
			printf("policy=%d\n", policy);
			test_ret = OSAL_TEST_NG;
		}
		TIME_MEASURE_STOP

		if (OSAL_RETURN_OK == osal_ret)
		{
			osal_ret = R_OSAL_IoDeviceClose(handle);
			if (OSAL_RETURN_OK != osal_ret)
			{
				OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
						OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID001);
				test_ret = OSAL_TEST_NG;
			}
		}
	}

	osal_ret = R_OSAL_Deinitialize();
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_PM, OSAL_TEST_PMGETPOLICY, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_PM,
							OSAL_TEST_PMGETPOLICY,
							OSAL_TEST_ID001, test_ret, opt_info);
	return;
}

/* 07_001: R_OSAL_PmSetRequiredState */
static void osal_test_R_OSAL_PmSetRequiredState()
{
	// There are no test cases for this API.
	return;
}

/* 07_002: R_OSAL_PmGetRequiredState */
static void osal_test_R_OSAL_PmGetRequiredState()
{
	// There are no test cases for this API.
	return;
}

/* 07_003: R_OSAL_PmWaitForState */
static void osal_test_R_OSAL_PmWaitForState()
{
#if defined(RCAR_V4H) || defined(RCAR_V4M)
	Test07_003_001();
#endif
	return;
}

/* 07_004: R_OSAL_PmGetState */
static void osal_test_R_OSAL_PmGetState()
{
	// There are no test cases for this API.
	return;
}

/* 07_005: R_OSAL_PmSetPolicy */
static void osal_test_R_OSAL_PmSetPolicy()
{
	// There are no test cases for this API.
	return;
}

/* 07_006: R_OSAL_PmGetPolicy */
static void osal_test_R_OSAL_PmGetPolicy()
{
	Test07_006_001();
	return;
}

/* 07_007: R_OSAL_PmSetResetState */
static void osal_test_R_OSAL_PmSetResetState()
{
	// There are no test cases for this API.
	return;
}

/* 07_008: R_OSAL_PmGetResetState */
static void osal_test_R_OSAL_PmGetResetState()
{
	// There are no test cases for this API.
	return;
}

/* 07_009: R_OSAL_PmSetPostClock */
static void osal_test_R_OSAL_PmSetPostClock()
{
	// There are no test cases for this API.
	return;
}

/* 07_010: R_OSAL_PmGetLowestIntermediatePowerState */
static void osal_test_R_OSAL_PmGetLowestIntermediatePowerState()
{
	// There are no test cases for this API.
	return;
}
