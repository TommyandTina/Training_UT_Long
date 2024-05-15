#define TEST_MSG_NAME "R_IMPDRV_ConfRegCheck"

#include "it_common.h"

#define CALLBACK_CODE 0xAA

static void set_state(st_impdrv_initdata_t *const p_data, e_impdrv_state_t state);
#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret);
static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret);
#endif
static void Device_conf_reg_check_EDC(struct test_params_t *tparam, struct test_result_t *tret);
static void Device_conf_reg_check_ECC(struct test_params_t *tparam, struct test_result_t *tret);

static uint32_t testno;
static st_impdrv_coretypectl_t coretype_ctl_backup[IMPDRV_CORE_TYPE_MAX];

static void set_state(st_impdrv_initdata_t *const p_data, e_impdrv_state_t state)
{
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_data->p_work_addr;

	for (uint32_t cnt = 0U; cnt < p_data->use_core_num; cnt++)
	{
		st_impdrv_core_info_t    *core_info = &p_data->core_info[cnt];
		e_impdrv_core_type_t core_type = core_info->core_type;
		uint32_t core_num = core_info->core_num;

		p_impdrv_ctl->coretype_ctl[core_type].core_ctl[core_num].core_state = state;
	}
}

int32_t it_ConfRegCheck_EDC_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

	/* Execute test */
#ifdef IT_ABNORMAL
	testno = 151;
	Parameter_error(tparam, &tret);
	testno = 158;
	Invalid_core(tparam, &tret);
	testno = 168;
	No_exec_core(tparam, &tret);
	testno = 172;
	Invalid_state(tparam, &tret);
#endif
	testno = 176;
#if defined (V3H1) || defined (V3H2) || defined (V3M2)
	testno++;
#endif
	Device_conf_reg_check_EDC(tparam, &tret);

	PrintTestResult(tret, test_result);

	return 0;
}

int32_t it_ConfRegCheck_ECC_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
#if defined (V3H2)
	struct test_result_t tret = {0};

	/* Execute test */
	testno = 182;
	Device_conf_reg_check_ECC(tparam, &tret);

	PrintTestResult(tret, test_result);
#endif

	return 0;
}

#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
#if !defined (V4H) && !defined (V4H2)
	st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_IMP, 0 };
#else
	st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_DSP, 0 };
#endif
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)tparam->initdata.p_work_addr;
	st_impdrv_chk_param_t chk_param;
	tp_arr_info_t data_info = get_data_info(core_info.core_type, IT_IMPDRV_CMD_CONF_REG_CHECK);
	st_impdrv_chk_param_t *p_chk_param = (st_impdrv_chk_param_t *)data_info.info;
	uint32_t param_num = data_info.range;

	/* Parameter_error : handle == NULL */
	ercd = R_IMPDRV_ConfRegCheck(NULL, p_chk_param, param_num, &core_info);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) handle == NULL", testno++);

	/* Parameter_error : p_core_info == NULL */
	ercd = R_IMPDRV_ConfRegCheck(handle, p_chk_param, param_num, NULL);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) p_core_info == NULL", testno++);

	/* Parameter_error : p_chk_param == NULL */
	ercd = R_IMPDRV_ConfRegCheck(handle, NULL, param_num, &core_info);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) p_chk_param == NULL", testno++);

	/* Parameter_error : p_chk_param->offset == 3U (invalid alignment) */
	memcpy(&chk_param, p_chk_param, sizeof(st_impdrv_chk_param_t));
	chk_param.offset = 3U;
	ercd = R_IMPDRV_ConfRegCheck(handle, &chk_param, param_num, &core_info);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) p_chk_param->offset == 3U", testno++);

	/* Parameter_error : p_chk_param->offset == 5U (invalid alignment) */
	memcpy(&chk_param, p_chk_param, sizeof(st_impdrv_chk_param_t));
	chk_param.offset = 5U;
	ercd = R_IMPDRV_ConfRegCheck(handle, &chk_param, param_num, &core_info);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) p_chk_param->offset == 5U", testno++);

	/* Parameter_error : p_chk_param->offset == 2U (invalid alignment) */
	memcpy(&chk_param, p_chk_param, sizeof(st_impdrv_chk_param_t));
	chk_param.offset = 2U;
	ercd = R_IMPDRV_ConfRegCheck(handle, &chk_param, param_num, &core_info);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) p_chk_param->offset == 2U", testno++);

	/* Parameter_error : param_num == 0U */
	ercd = R_IMPDRV_ConfRegCheck(handle, p_chk_param, 0U, &core_info);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) param_num == 0U", testno++);
}

static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
#if !defined (V4H) && !defined (V4H2)
	st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_IMP, 0 };
#else
	st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_DSP, 0 };
#endif
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)tparam->initdata.p_work_addr;
	tp_arr_info_t data_info = get_data_info(core_info.core_type, IT_IMPDRV_CMD_CONF_REG_CHECK);
	st_impdrv_chk_param_t *p_chk_param = (st_impdrv_chk_param_t *)data_info.info;
	uint32_t param_num = data_info.range;
	tp_core_list_t invalid_core_list = get_core_list(IT_IMPDRV_CMD_INVALID_CORES, 0);

	/* Invalid_error : Invalid core num for coretypes */
	for (uint32_t i = 0; i < invalid_core_list.num; i++)
	{
#if defined (V3H1)
		if (IMPDRV_CORE_TYPE_PSCEXE == (invalid_core_list.core_info + i)->core_type)
		{
			testno++;
		}
#elif defined (V3M2)
		if ((IMPDRV_CORE_TYPE_OCV == (invalid_core_list.core_info + i)->core_type) || (IMPDRV_CORE_TYPE_PSCEXE == (invalid_core_list.core_info + i)->core_type))
		{
			testno++;
		}
#elif defined (V4H) || defined (V4H2)
		if (IMPDRV_CORE_TYPE_OCV == (invalid_core_list.core_info + i)->core_type)
		{
			testno++;
		}
#endif

		core_info = *(invalid_core_list.core_info + i);
		ercd = R_IMPDRV_ConfRegCheck(handle, p_chk_param, param_num, &core_info);
		FailTestPrint(ercd, ((core_info.core_type == IMPDRV_CORE_TYPE_INVALID) ?
							IMPDRV_EC_NG_PARAM : IMPDRV_EC_NG_NOTSUPPORT),
							*tret, "(No.%d) Invalid core num for %s", testno++, type_to_name(core_info.core_type));
	}

	/* Invalid_error : Core_num = IMPDRV_CORE_NUM_MAX */
	core_info.core_type = IMPDRV_CORE_TYPE_IMP;
	core_info.core_num = IMPDRV_CORE_NUM_MAX;
	ercd = R_IMPDRV_ConfRegCheck(handle, p_chk_param, param_num, &core_info);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) Core_num = IMPDRV_CORE_NUM_MAX", testno++);
}

static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
#if !defined (V4H) && !defined (V4H2)
	st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_IMP, 0 };
#else
	st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_DSP, 0 };
#endif
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)tparam->initdata.p_work_addr;
	tp_arr_info_t data_info = get_data_info(core_info.core_type, IT_IMPDRV_CMD_CONF_REG_CHECK);
	st_impdrv_chk_param_t *p_chk_param = (st_impdrv_chk_param_t *)data_info.info;
	uint32_t param_num = data_info.range;
	tp_core_list_t no_exec_cores = get_core_list(IT_IMPDRV_CMD_NO_EXEC, 0);

	/* Not supported core: Core not supported */
	for (uint32_t i = 0; i < no_exec_cores.num; i++)
	{
		core_info = *(no_exec_cores.core_info + i);
		ercd = R_IMPDRV_ConfRegCheck(handle, p_chk_param, param_num, &core_info);
		FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) Core not supported: %s", testno++, type_to_name(core_info.core_type));
	}
}

static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
#if !defined (V4H) && !defined (V4H2)
	st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_IMP, 0 };
#else
	st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_DSP, 0 };
#endif
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)tparam->initdata.p_work_addr;
	tp_arr_info_t data_info = get_data_info(core_info.core_type, IT_IMPDRV_CMD_CONF_REG_CHECK);
	st_impdrv_chk_param_t *p_chk_param = (st_impdrv_chk_param_t *)data_info.info;
	uint32_t param_num = data_info.range;
	e_impdrv_state_t invalid_state[] = {
		IMPDRV_STATE_UNINIT,
		IMPDRV_STATE_INIT,
		IMPDRV_STATE_EXEC,
		IMPDRV_STATE_INT
	};

	/* Back up data */
	memcpy(coretype_ctl_backup, p_impdrv_ctl->coretype_ctl, sizeof(p_impdrv_ctl->coretype_ctl));

	for (uint32_t i = 0; i < ARRAY_SIZE(invalid_state); i++)
	{
		set_state(p_initdata, invalid_state[i]);
		ercd = R_IMPDRV_ConfRegCheck(handle, p_chk_param, param_num, &core_info);
		FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) %s", testno++, state_to_name(invalid_state[i]));
	}

	/* Get backup data */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
}
#endif

static void Device_conf_reg_check_EDC(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)tparam->initdata.p_work_addr;
	st_impdrv_core_info_t *p_core_info;
	tp_arr_info_t data_info;
	st_impdrv_chk_param_t *p_chk_param;
	uint32_t param_num;
	int32_t result = 0;
	
	/* driver_start : output parameter */
	for (uint32_t i = 0; i < tparam->initdata.use_core_num; i++)
	{
		result = 0;
		p_core_info = &tparam->initdata.core_info[i];
#if !defined (V4H) && !defined (V4H2)
		if (IMPDRV_CORE_TYPE_IMP == p_core_info->core_type)
#else
	    if (IMPDRV_CORE_TYPE_DSP == p_core_info->core_type)
#endif
		{
			data_info = get_data_info(p_core_info->core_type, IT_IMPDRV_CMD_CONF_REG_CHECK);
			p_chk_param = (st_impdrv_chk_param_t *)data_info.info;
			param_num = data_info.range;

			ercd = R_IMPDRV_ConfRegCheck(handle, p_chk_param, param_num, p_core_info);
			if (IMPDRV_EC_OK != ercd) {
				core_testmsg(p_core_info, "%s=%d\n", TEST_MSG_NAME, ercd);
				result = -1;
			}

			SuccessTestPrint(result == 0, *tret, "(No.%d) %s%d ", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num);
		}
	}
}

static void Device_conf_reg_check_ECC(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)tparam->initdata.p_work_addr;
	st_impdrv_core_info_t *p_core_info;
	tp_arr_info_t data_info;
	st_impdrv_chk_param_t *p_chk_param;
	uint32_t param_num;
	int32_t result = 0;

	/* driver_start : output parameter */
	for (uint32_t i = 0; i < tparam->initdata.use_core_num; i++)
	{
		result = 0;
		p_core_info = &tparam->initdata.core_info[i];
#if !defined (V4H) && !defined (V4H2)
		if (IMPDRV_CORE_TYPE_IMP == p_core_info->core_type)
#else
	    if (IMPDRV_CORE_TYPE_DSP == p_core_info->core_type)
#endif
		{
			if (IMPDRV_EC_OK != R_IMPDRV_SetMemProtect(handle, IMPDRV_PROTECT_ECC))
			{
				core_testmsg(p_core_info, "SetMemProtect error\n");
				result = -1;
			}
			else if (IMPDRV_EC_OK != R_IMPDRV_Start(handle, p_core_info))
			{
				core_testmsg(p_core_info, "Start error\n");
				result = -1;
			}
			else
			{
				if (IMPDRV_EC_OK == ercd)
				{
					data_info = get_data_info(p_core_info->core_type, IT_IMPDRV_CMD_CONF_REG_CHECK);
					p_chk_param = (st_impdrv_chk_param_t *)data_info.info;
					param_num = data_info.range;

					ercd = R_IMPDRV_ConfRegCheck(handle, p_chk_param, param_num, p_core_info);
					if (IMPDRV_EC_OK != ercd) {
						core_testmsg(p_core_info, "%s=%d\n", TEST_MSG_NAME, ercd);
						result = -1;
					}
				}
			}
			
			(void)R_IMPDRV_Stop(handle, p_core_info);

			SuccessTestPrint(result == 0, *tret, "(No.%d) %s%d ", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num);
		}
	}
}