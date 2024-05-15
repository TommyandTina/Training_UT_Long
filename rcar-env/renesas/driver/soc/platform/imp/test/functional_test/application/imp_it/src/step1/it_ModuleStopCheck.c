#define TEST_MSG_NAME "R_IMPDRV_ModuleStopCheck"

#include "it_common.h"
#include "it_stub.h"

static void set_state(st_impdrv_initdata_t *const p_data, e_impdrv_state_t state);
#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret);
static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret);
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

int32_t it_ModuleStopCheck_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

	/* Execute test */
#ifdef IT_ABNORMAL
	testno = 1;
	Parameter_error(tparam, &tret);
	testno = 8;
	Invalid_core(tparam, &tret);
	testno = 10;
	No_exec_core(tparam, &tret);
	testno = 14;
	Return_error(tparam, &tret);
#endif
	testno = 37;
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
	st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_IMP, 0 };
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)tparam->initdata.p_work_addr;
	st_impdrv_chk_param_t chk_param;
	tp_arr_info_t data_info = get_data_info(core_info.core_type, IT_IMPDRV_CMD_MODULE_STOP_CHECK);
	st_impdrv_chk_param_t *p_chk_param = (st_impdrv_chk_param_t *)data_info.info;
	uint32_t param_num = data_info.range;

	/* Parameter_error : handle == NULL */
	ercd = R_IMPDRV_ModuleStopCheck(NULL, p_chk_param, param_num, &core_info);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) handle == NULL", testno++);

	/* Parameter_error : p_core_info == NULL */
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, NULL);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) p_core_info == NULL", testno++);

	/* Parameter_error : p_chk_param == NULL */
	ercd = R_IMPDRV_ModuleStopCheck(handle, NULL, param_num, &core_info);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) p_chk_param == NULL", testno++);

	/* Parameter_error : p_chk_param->offset == 3U (invalid alignment) */
	memcpy(&chk_param, p_chk_param, sizeof(st_impdrv_chk_param_t));
	chk_param.offset = 3U;
	ercd = R_IMPDRV_ModuleStopCheck(handle, &chk_param, param_num, &core_info);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) p_chk_param->offset == 3U", testno++);

	/* Parameter_error : p_chk_param->offset == 5U (invalid alignment) */
	memcpy(&chk_param, p_chk_param, sizeof(st_impdrv_chk_param_t));
	chk_param.offset = 5U;
	ercd = R_IMPDRV_ModuleStopCheck(handle, &chk_param, param_num, &core_info);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) p_chk_param->offset == 5U", testno++);

	/* Parameter_error : p_chk_param->offset == 2U (invalid alignment) */
	memcpy(&chk_param, p_chk_param, sizeof(st_impdrv_chk_param_t));
	chk_param.offset = 2U;
	ercd = R_IMPDRV_ModuleStopCheck(handle, &chk_param, param_num, &core_info);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) p_chk_param->offset == 2U", testno++);

	/* Parameter_error : param_num == 0U */
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, 0U, &core_info);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) param_num == 0U", testno++);
}

static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_IMP, 0 };
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)tparam->initdata.p_work_addr;
	tp_arr_info_t data_info = get_data_info(core_info.core_type, IT_IMPDRV_CMD_MODULE_STOP_CHECK);
	st_impdrv_chk_param_t *p_chk_param = (st_impdrv_chk_param_t *)data_info.info;
	uint32_t param_num = data_info.range;

	/* Invalid_error : Core_type = IMPDRV_CORE_TYPE_INVALID */
	core_info.core_type = IMPDRV_CORE_TYPE_INVALID;
	core_info.core_num = 0U;
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) Core_type = IMPDRV_CORE_TYPE_INVALID", testno++);

	/* Invalid_error : Core_num = IMPDRV_CORE_NUM_MAX */
	core_info.core_type = IMPDRV_CORE_TYPE_IMP;
	core_info.core_num = IMPDRV_CORE_NUM_MAX;
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) Core_num = IMPDRV_CORE_NUM_MAX", testno++);
}

static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_IMP, 0 };
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)tparam->initdata.p_work_addr;
	tp_arr_info_t data_info = get_data_info(core_info.core_type, IT_IMPDRV_CMD_MODULE_STOP_CHECK);
	st_impdrv_chk_param_t *p_chk_param = (st_impdrv_chk_param_t *)data_info.info;
	uint32_t param_num = data_info.range;
	tp_core_list_t no_exec_cores = get_core_list(IT_IMPDRV_CMD_NO_EXEC, 0);

	/* Not supported core: Core not supported */
	for (uint32_t i = 0; i < no_exec_cores.num; i++)
	{
		core_info = *(no_exec_cores.core_info + i);
		ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
		FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) Core not supported: %s", testno++, type_to_name(core_info.core_type));
	}
}

static void Return_error(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_IMP, 0 };
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)tparam->initdata.p_work_addr;
	tp_arr_info_t data_info = get_data_info(core_info.core_type, IT_IMPDRV_CMD_MODULE_STOP_CHECK);
	st_impdrv_chk_param_t *p_chk_param = (st_impdrv_chk_param_t *)data_info.info;
	uint32_t param_num = data_info.range;

	/* Fail to lock mutex impdrv_cmnctl_mutex_lock returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_cmnctl_mutex_lock(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_cmnctl_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to lock mutex impdrv_cmnctl_mutex_lock returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_cmnctl_mutex_lock(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to lock mutex impdrv_cmnctl_mutex_lock returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_cmnctl_mutex_lock(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to get core function impdrv_cmnctl_get_corectl_func returns NULL */
	it_stub_set_impdrv_cmnctl_get_corectl_func(NULL);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) impdrv_cmnctl_get_corectl_func returns NULL", testno++);

#if defined (V3H1) || defined (V3H2) || defined (V3M2)
	/* Fail to execute Configuration Register Check for IMP0 impdrv_impctl_conf_reg_check returns IMPDRV_EC_NG_CHECKFAIL */
	it_stub_set_impdrv_impctl_conf_reg_check(IMPDRV_EC_NG_CHECKFAIL);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_CHECKFAIL, *tret, "(No.%d) impdrv_impctl_conf_reg_check for %s%d returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, impdrv_err_to_name(IMPDRV_EC_NG_CHECKFAIL));

	/* Fail to execute Configuration Register Check for OCV0 impdrv_ocvctl_conf_reg_check returns IMPDRV_EC_NG_SEQSTATE */
	core_info.core_type = IMPDRV_CORE_TYPE_OCV;
	core_info.core_num = 0U;
	data_info = get_data_info(core_info.core_type, IT_IMPDRV_CMD_MODULE_STOP_CHECK);
	p_chk_param = (st_impdrv_chk_param_t *)data_info.info;
	param_num = data_info.range;
	it_stub_set_impdrv_ocvctl_conf_reg_check(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_ocvctl_conf_reg_check for %s%d returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to execute Configuration Register Check for DMAC00 impdrv_dmactl_conf_reg_check returns IMPDRV_EC_NG_ARGNULL */
	core_info.core_type = IMPDRV_CORE_TYPE_DMAC;
	core_info.core_num = 0U;
	data_info = get_data_info(core_info.core_type, IT_IMPDRV_CMD_MODULE_STOP_CHECK);
	p_chk_param = (st_impdrv_chk_param_t *)data_info.info;
	param_num = data_info.range;
	it_stub_set_impdrv_dmactl_conf_reg_check(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_dmactl_conf_reg_check for %s%d returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to execute Configuration Register Check for PSCEXE0 impdrv_pscctl_conf_reg_check returns IMPDRV_EC_NG_PARAM */
	core_info.core_type = IMPDRV_CORE_TYPE_PSCEXE;
	core_info.core_num = 0U;
	data_info = get_data_info(core_info.core_type, IT_IMPDRV_CMD_MODULE_STOP_CHECK);
	p_chk_param = (st_impdrv_chk_param_t *)data_info.info;
	param_num = data_info.range;
	it_stub_set_impdrv_pscctl_conf_reg_check(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_pscctl_conf_reg_check for %s%d returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to execute Configuration Register Check for CNN impdrv_cnnctl_conf_reg_check returns IMPDRV_EC_NG_SYSTEMERROR */
	core_info.core_type = IMPDRV_CORE_TYPE_CNN;
	core_info.core_num = 0U;
	data_info = get_data_info(core_info.core_type, IT_IMPDRV_CMD_MODULE_STOP_CHECK);
	p_chk_param = (st_impdrv_chk_param_t *)data_info.info;
	param_num = data_info.range;
	it_stub_set_impdrv_cnnctl_conf_reg_check(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cnnctl_conf_reg_check for %s%d returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to execute Configuration Register Check for IMP0 impdrv_impctl_conf_reg_check returns IMPDRV_EC_NG_NOTSUPPORT */
	core_info.core_type = IMPDRV_CORE_TYPE_IMP;
	core_info.core_num = 0U;
	data_info = get_data_info(core_info.core_type, IT_IMPDRV_CMD_MODULE_STOP_CHECK);
	p_chk_param = (st_impdrv_chk_param_t *)data_info.info;
	param_num = data_info.range;
	it_stub_set_impdrv_impctl_conf_reg_check(IMPDRV_EC_NG_NOTSUPPORT);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) impdrv_impctl_conf_reg_check for %s%d returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, impdrv_err_to_name(IMPDRV_EC_NG_NOTSUPPORT));

#elif defined (V4H) || defined (V4H2)
	core_info.core_type = IMPDRV_CORE_TYPE_DSP;
	core_info.core_num = 0;
	data_info = get_data_info(core_info.core_type, IT_IMPDRV_CMD_MODULE_STOP_CHECK);
	p_chk_param = (st_impdrv_chk_param_t *)data_info.info;
	param_num = data_info.range;

	/* Fail to execute Configuration Register Check for DSP0 impdrv_dspctl_conf_reg_check returns IMPDRV_EC_NG_CHECKFAIL */
	it_stub_set_impdrv_dspctl_conf_reg_check(IMPDRV_EC_NG_CHECKFAIL);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_CHECKFAIL, *tret, "(No.%d) impdrv_dspctl_conf_reg_check for %s%d returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, impdrv_err_to_name(IMPDRV_EC_NG_CHECKFAIL));

	/* Fail to execute Configuration Register Check for DSP0 impdrv_dspctl_conf_reg_check returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_dspctl_conf_reg_check(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_dspctl_conf_reg_check for %s%d returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to execute Configuration Register Check for DSP0 impdrv_dspctl_conf_reg_check returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_dspctl_conf_reg_check(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_dspctl_conf_reg_check for %s%d returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to execute Configuration Register Check for DSP0 impdrv_dspctl_conf_reg_check returns IMPDRV_EC_NG_PARAM */
	it_stub_set_impdrv_dspctl_conf_reg_check(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_dspctl_conf_reg_check for %s%d returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to execute Configuration Register Check for DSP0 impdrv_dspctl_conf_reg_check returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_dspctl_conf_reg_check(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_dspctl_conf_reg_check for %s%d returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to execute Configuration Register Check for DSP0 impdrv_dspctl_conf_reg_check returns IMPDRV_EC_NG_NOTSUPPORT */
	it_stub_set_impdrv_dspctl_conf_reg_check(IMPDRV_EC_NG_NOTSUPPORT);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) impdrv_dspctl_conf_reg_check for %s%d returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, impdrv_err_to_name(IMPDRV_EC_NG_NOTSUPPORT));
#endif

	/* Fail to execute common Configuration Register Check impdrv_cmnctl_conf_reg_check returns IMPDRV_EC_NG_CHECKFAIL */
	it_stub_set_impdrv_cmnctl_conf_reg_check(IMPDRV_EC_NG_CHECKFAIL);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_CHECKFAIL, *tret, "(No.%d) impdrv_cmnctl_conf_reg_check returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_CHECKFAIL));

	/* Fail to execute common Configuration Register Check impdrv_cmnctl_conf_reg_check returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_cmnctl_conf_reg_check(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_cmnctl_conf_reg_check returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to execute common Configuration Register Check impdrv_cmnctl_conf_reg_check returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_cmnctl_conf_reg_check(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_conf_reg_check returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to execute common Configuration Register Check impdrv_cmnctl_conf_reg_check returns IMPDRV_EC_NG_PARAM */
	it_stub_set_impdrv_cmnctl_conf_reg_check(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_conf_reg_check returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to execute common Configuration Register Check impdrv_cmnctl_conf_reg_check returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_cmnctl_conf_reg_check(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_conf_reg_check returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to check memory protection impdrv_cmnctl_reg_chk_mem_protect returns IMPDRV_EC_NG_CHECKFAIL */
	it_stub_set_impdrv_cmnctl_reg_chk_mem_protect(IMPDRV_EC_NG_CHECKFAIL);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_CHECKFAIL, *tret, "(No.%d) impdrv_cmnctl_reg_chk_mem_protect returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_CHECKFAIL));

	/* Fail to check memory protection impdrv_cmnctl_reg_chk_mem_protect returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_cmnctl_reg_chk_mem_protect(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_cmnctl_reg_chk_mem_protect returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to check memory protection impdrv_cmnctl_reg_chk_mem_protect returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_cmnctl_reg_chk_mem_protect(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_reg_chk_mem_protect returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to check memory protection impdrv_cmnctl_reg_chk_mem_protect returns IMPDRV_EC_NG_PARAM */
	it_stub_set_impdrv_cmnctl_reg_chk_mem_protect(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_reg_chk_mem_protect returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to check memory protection impdrv_cmnctl_reg_chk_mem_protect returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_cmnctl_reg_chk_mem_protect(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_reg_chk_mem_protect returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to unlock mutex impdrv_cmnctl_mutex_unlock returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_cmnctl_mutex_unlock(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_cmnctl_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to unlock mutex impdrv_cmnctl_mutex_unlock returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_cmnctl_mutex_unlock(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to unlock mutex impdrv_cmnctl_mutex_unlock returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_cmnctl_mutex_unlock(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, &core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));
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
#elif defined (V4H) || defined (V4H2)
		if (IMPDRV_CORE_TYPE_DSP == p_core_info->core_type)
			
#endif
		{
			ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, p_core_info);
			if (IMPDRV_EC_OK != ercd) {
				core_testmsg(p_core_info, "%s=%d\n", TEST_MSG_NAME, ercd);
				result = -1;
			}

			SuccessTestPrint(result == 0, *tret, "(No.%d) %s%d ", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num);
		}
	}
}