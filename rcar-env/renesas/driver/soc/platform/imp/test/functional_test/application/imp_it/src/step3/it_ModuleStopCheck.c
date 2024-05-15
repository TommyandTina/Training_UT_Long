#define TEST_MSG_NAME "R_IMPDRV_ModuleStopCheck"

#include "it_common.h"
#include "it_stub.h"

static void set_state(st_impdrv_initdata_t *const p_data, e_impdrv_state_t state);
#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret);
static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret);
static void Return_error(struct test_params_t *tparam, struct test_result_t *tret);
#endif
static void Device_module_stop_check(struct test_params_t *tparam, struct test_result_t *tret);

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

static void prepare_test(impdrv_ctrl_handle_t handle, const st_impdrv_core_info_t *const p_core_info)
{
	int8_t i = 0;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)handle;
	tp_arr_info_t data_info = get_data_info(p_core_info->core_type, IT_IMPDRV_CMD_MODULE_STOP_CHECK);
	st_impdrv_chk_param_t *p_chk_param = (st_impdrv_chk_param_t *)data_info.info;
	uint32_t param_num = data_info.range;

	/* for configuration registers */
	for (i = 0; i < param_num; i++)
	{
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i, (p_chk_param + i)->data);
	}

#if defined (V3H2) || defined (V3H1) || defined (V3M2)
	/* for distributor registers */
	for (int8_t n = i; i < (n + 4); i++)
	{
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i, 0U);
	}

	if (IMPDRV_PROTECT_EDC == p_impdrv_ctl->common_ctl.protect_mode)
	{
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i, 0U);
	}
	else
	{
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i, IMPDRV_REG_PROTECT_MASK);
	}
#endif
}

int32_t it_ModuleStopCheck_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

	/* Execute test */
#ifdef IT_ABNORMAL
	testno = 123;
	Parameter_error(tparam, &tret);
	testno = 130;
	Invalid_core(tparam, &tret);
	testno = 140;
	No_exec_core(tparam, &tret);
	testno = 144;
	Invalid_state(tparam, &tret);
	testno = 148;
	Return_error(tparam, &tret);
#endif
	testno = 175;
#if defined (V4H) || defined (V4H2)
	testno += 20;
#endif
	Device_module_stop_check(tparam, &tret);

	PrintTestResult(tret, test_result);

	return 0;
}

#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
#if defined (V4H) || defined (V4H2)
    st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_DSP, 0 };
#else
	st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_IMP, 0 };
#endif
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)tparam->initdata.p_work_addr;
	st_impdrv_chk_param_t chk_param;
	tp_arr_info_t data_info = get_data_info(core_info.core_type, IT_IMPDRV_CMD_MODULE_STOP_CHECK);
	st_impdrv_chk_param_t *p_chk_param = (st_impdrv_chk_param_t *)data_info.info;
	uint32_t param_num = data_info.range;

	/* Parameter_error : handle == NULL */
	it_stub_reset();
	prepare_test(handle, &core_info);
	ercd = R_IMPDRV_ModuleStopCheck(NULL, p_chk_param, param_num, &core_info);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) handle == NULL", testno++);

	/* Parameter_error : p_core_info == NULL */
	it_stub_reset();
	prepare_test(handle, &core_info);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, NULL);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) p_core_info == NULL", testno++);

	/* Parameter_error : p_chk_param == NULL */
	it_stub_reset();
	prepare_test(handle, &core_info);
	ercd = R_IMPDRV_ModuleStopCheck(handle, NULL, param_num, &core_info);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) p_chk_param == NULL", testno++);

	/* Parameter_error : p_chk_param->offset == 3U (invalid alignment) */
	it_stub_reset();
	prepare_test(handle, &core_info);
	memcpy(&chk_param, p_chk_param, sizeof(st_impdrv_chk_param_t));
	chk_param.offset = 3U;
	ercd = R_IMPDRV_ModuleStopCheck(handle, &chk_param, param_num, &core_info);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) p_chk_param->offset == 3U", testno++);

	/* Parameter_error : p_chk_param->offset == 5U (invalid alignment) */
	it_stub_reset();
	prepare_test(handle, &core_info);
	memcpy(&chk_param, p_chk_param, sizeof(st_impdrv_chk_param_t));
	chk_param.offset = 5U;
	ercd = R_IMPDRV_ModuleStopCheck(handle, &chk_param, param_num, &core_info);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) p_chk_param->offset == 5U", testno++);

	/* Parameter_error : p_chk_param->offset == 2U (invalid alignment) */
	it_stub_reset();
	prepare_test(handle, &core_info);
	memcpy(&chk_param, p_chk_param, sizeof(st_impdrv_chk_param_t));
	chk_param.offset = 2U;
	ercd = R_IMPDRV_ModuleStopCheck(handle, &chk_param, param_num, &core_info);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) p_chk_param->offset == 2U", testno++);

	/* Parameter_error : param_num == 0U */
	it_stub_reset();
	prepare_test(handle, &core_info);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, 0U, &core_info);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) param_num == 0U", testno++);
}

static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
#if defined (V4H) || defined (V4H2)
    st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_DSP, 0 };
#else
	st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_IMP, 0 };
#endif
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)tparam->initdata.p_work_addr;
	tp_arr_info_t data_info = get_data_info(core_info.core_type, IT_IMPDRV_CMD_MODULE_STOP_CHECK);
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
		it_stub_reset();
		prepare_test(handle, &core_info);
		ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
		FailTestPrint(ercd, ((core_info.core_type == IMPDRV_CORE_TYPE_INVALID) ?
							IMPDRV_EC_NG_PARAM : IMPDRV_EC_NG_NOTSUPPORT),
							*tret, "(No.%d) Invalid core num for %s", testno++, type_to_name(core_info.core_type));
	}

	/* Invalid_error : Core_num = IMPDRV_CORE_NUM_MAX */
#if defined (V4H) || defined (V4H2)
	core_info.core_type = IMPDRV_CORE_TYPE_DSP;
#else
	core_info.core_type = IMPDRV_CORE_TYPE_IMP;
#endif
	core_info.core_num = IMPDRV_CORE_NUM_MAX;
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) Core_num = IMPDRV_CORE_NUM_MAX", testno++);
}

static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
#if defined (V4H) || defined (V4H2)
    st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_DSP, 0 };
#else
	st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_IMP, 0 };
#endif
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)tparam->initdata.p_work_addr;
	tp_arr_info_t data_info = get_data_info(core_info.core_type, IT_IMPDRV_CMD_MODULE_STOP_CHECK);
	st_impdrv_chk_param_t *p_chk_param = (st_impdrv_chk_param_t *)data_info.info;
	uint32_t param_num = data_info.range;
	tp_core_list_t no_exec_cores = get_core_list(IT_IMPDRV_CMD_NO_EXEC, 0);

	/* Not supported core: Core not supported */
	for (uint32_t i = 0; i < no_exec_cores.num; i++)
	{
		core_info = *(no_exec_cores.core_info + i);
		it_stub_reset();
		prepare_test(handle, &core_info);
		ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
		FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) Core not supported: %s", testno++, type_to_name(core_info.core_type));
	}
}

static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
#if defined (V4H) || defined (V4H2)
    st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_DSP, 0 };
#else
	st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_IMP, 0 };
#endif
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)tparam->initdata.p_work_addr;
	tp_arr_info_t data_info = get_data_info(core_info.core_type, IT_IMPDRV_CMD_MODULE_STOP_CHECK);
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
		it_stub_reset();
		prepare_test(handle, &core_info);
		set_state(p_initdata, invalid_state[i]);
		ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
		FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) %s", testno++, state_to_name(invalid_state[i]));
	}

	/* Get backup data */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
}

static void Return_error(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
#if defined (V4H) || defined (V4H2)
    st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_DSP, 0 };
#else
	st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_IMP, 0 };
#endif
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)tparam->initdata.p_work_addr;
	tp_arr_info_t data_info = get_data_info(core_info.core_type, IT_IMPDRV_CMD_MODULE_STOP_CHECK);
	st_impdrv_chk_param_t *p_chk_param = (st_impdrv_chk_param_t *)data_info.info;
	uint32_t param_num = data_info.range;

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_TIME */
	prepare_test(handle, &core_info);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_TIME);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_TIME));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_PAR */
	prepare_test(handle, &core_info);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_PAR);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_BUSY */
	prepare_test(handle, &core_info);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_BUSY);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_BUSY));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_HANDLE */
	prepare_test(handle, &core_info);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_STATE */
	prepare_test(handle, &core_info);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_FAIL */
	prepare_test(handle, &core_info);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Failed to check configuration register: R_OSAL_IoRead32 returns OSAL_RETURN_PAR */
	prepare_test(handle, &core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_PAR, 0, 0);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 (configuration register) returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Failed to check configuration register: R_OSAL_IoRead32 returns OSAL_RETURN_DEV */
	prepare_test(handle, &core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_DEV, 0, 0);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 (configuration register) returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Failed to check configuration register: R_OSAL_IoRead32 returns OSAL_RETURN_IO */
	prepare_test(handle, &core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_IO, 0, 0);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 (configuration register) returns %s", testno++, osal_err_to_name(OSAL_RETURN_IO));

	/* Failed to check configuration register: R_OSAL_IoRead32 returns OSAL_RETURN_HANDLE */
	prepare_test(handle, &core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_HANDLE, 0, 0);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 (configuration register) returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to check configuration register: R_OSAL_IoRead32 returns OSAL_RETURN_STATE */
	prepare_test(handle, &core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_STATE, 0, 0);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 (configuration register) returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Failed to check configuration register: R_OSAL_IoRead32 returns OSAL_RETURN_FAIL */
	prepare_test(handle, &core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_FAIL, 0, 0);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 (configuration register) returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

#if !defined (V4H) && !defined (V4H2)
	/* Failed to check distributor register: R_OSAL_IoRead32 returns OSAL_RETURN_PAR */
	prepare_test(handle, &core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_PAR, 1, 0);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 (distributor register) returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Failed to check distributor register: R_OSAL_IoRead32 returns OSAL_RETURN_DEV */
	prepare_test(handle, &core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_DEV, 1, 0);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 (distributor register) returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Failed to check distributor register: R_OSAL_IoRead32 returns OSAL_RETURN_IO */
	prepare_test(handle, &core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_IO, 1, 0);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 (distributor register) returns %s", testno++, osal_err_to_name(OSAL_RETURN_IO));

	/* Failed to check distributor register: R_OSAL_IoRead32 returns OSAL_RETURN_HANDLE */
	prepare_test(handle, &core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_HANDLE, 1, 0);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 (distributor register) returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to check distributor register: R_OSAL_IoRead32 returns OSAL_RETURN_STATE */
	prepare_test(handle, &core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_STATE, 1, 0);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 (distributor register) returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Failed to check distributor register: R_OSAL_IoRead32 returns OSAL_RETURN_FAIL */
	prepare_test(handle, &core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_FAIL, 1, 0);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 (distributor register) returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Failed to check memory protection setting: R_OSAL_IoRead32 returns OSAL_RETURN_PAR */
	prepare_test(handle, &core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_PAR, 2, 0);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 (memory protection setting) returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Failed to check memory protection setting: R_OSAL_IoRead32 returns OSAL_RETURN_DEV */
	prepare_test(handle, &core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_DEV, 2, 0);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 (memory protection setting) returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Failed to check memory protection setting: R_OSAL_IoRead32 returns OSAL_RETURN_IO */
	prepare_test(handle, &core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_IO, 2, 0);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 (memory protection setting) returns %s", testno++, osal_err_to_name(OSAL_RETURN_IO));

	/* Failed to check memory protection setting: R_OSAL_IoRead32 returns OSAL_RETURN_HANDLE */
	prepare_test(handle, &core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_HANDLE, 2, 0);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 (memory protection setting) returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to check memory protection setting: R_OSAL_IoRead32 returns OSAL_RETURN_STATE */
	prepare_test(handle, &core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_STATE, 2, 0);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 (memory protection setting) returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Failed to check memory protection setting: R_OSAL_IoRead32 returns OSAL_RETURN_FAIL */
	prepare_test(handle, &core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_FAIL, 2, 0);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 (memory protection setting) returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));
#else
	testno += 12;
#endif
	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_HANDLE */
	prepare_test(handle, &core_info);
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_STATE */
	prepare_test(handle, &core_info);
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_FAIL */
	prepare_test(handle, &core_info);
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));
}
#endif

static void Device_module_stop_check(struct test_params_t *tparam, struct test_result_t *tret)
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
		data_info = get_data_info(p_core_info->core_type, IT_IMPDRV_CMD_MODULE_STOP_CHECK);
		p_chk_param = (st_impdrv_chk_param_t *)data_info.info;
		param_num = data_info.range;
#if defined (V4H) || defined (V4H2)
		if (IMPDRV_CORE_TYPE_DSP != p_core_info->core_type)
		{
			continue;
		}
#endif

#if defined (V3H1)
		if ((IMPDRV_CORE_TYPE_PSCEXE == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno += 2;
		}
#elif defined (V3H2)
		if ((IMPDRV_CORE_TYPE_CNN == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno += 1;
		}
#elif defined (V3M2)
		if ((IMPDRV_CORE_TYPE_OCV == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno += 2;
		}
		if ((IMPDRV_CORE_TYPE_DMAC == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno += 3;
		}
		if ((IMPDRV_CORE_TYPE_PSCEXE == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno += 4;
		}
		if ((IMPDRV_CORE_TYPE_CNN == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno += 1;
		}
#endif

		it_stub_reset();
		prepare_test(handle, p_core_info);

		ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, p_core_info);
		if (IMPDRV_EC_OK != ercd) {
			core_testmsg(p_core_info, "%s=%d\n", TEST_MSG_NAME, ercd);
			result = -1;
		}

		SuccessTestPrint(result == 0, *tret, "(No.%d) %s%d ", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num);
	}
}