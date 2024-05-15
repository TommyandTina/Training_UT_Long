
#define TEST_MSG_NAME "R_IMPDRV_SetIrqGroup"

#include "it_common.h"
#include "it_stub.h"

#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret);
static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret);
static void Return_error(struct test_params_t *tparam, struct test_result_t *tret);
#endif
static void Device_set_irq_group(struct test_params_t *tparam, struct test_result_t *tret);

static uint32_t testno;

int32_t it_SetIrqGroup_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

#ifdef IT_ABNORMAL
	testno = 1;
	Parameter_error(tparam, &tret);
	Invalid_core(tparam, &tret);
	No_exec_core(tparam, &tret);
	Return_error(tparam, &tret);
#endif
	Device_set_irq_group(tparam, &tret);

	PrintTestResult(tret, test_result);

	return 0;
}

#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	st_impdrv_irq_group_t irq_param = g_p_irq_param[IMPDRV_IRQ_GROUP_NONE];
	st_impdrv_irq_group_t     *const p_irq_param = &irq_param;

	/* Parameter_error : handle == NULL */
	ercd = R_IMPDRV_SetIrqGroup(NULL, p_irq_param);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) handle == NULL", testno++);

	/* Parameter_error : p_irq_param == NULL */
	ercd = R_IMPDRV_SetIrqGroup(handle, NULL);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) p_irq_param == NULL", testno++);

	/* Parameter_error : p_irq_param->irq_group == IMPDRV_IRQ_GROUP_INVALID */
	p_irq_param->irq_group = IMPDRV_IRQ_GROUP_INVALID;
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) p_irq_param->irq_group == IMPDRV_IRQ_GROUP_INVALID", testno++);

	/* Parameter_error : p_irq_param->irq_group == 5 */
	p_irq_param->irq_group = 5U;
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) p_irq_param->irq_group == 5", testno++);
	memcpy(p_irq_param, &g_p_irq_param[IMPDRV_IRQ_GROUP_NONE], sizeof(st_impdrv_irq_group_t));

	/* Parameter_error : p_irq_param->group_core_num = 0 */
	p_irq_param->group_core_num = 0U;
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) p_irq_param->group_core_num = 0", testno++);

	/* Parameter_error : p_irq_param->group_core_num = 22U */
	p_irq_param->group_core_num = 22U;
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) p_irq_param->group_core_num = 22", testno++);
}

static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	st_impdrv_irq_group_t irq_param = g_p_irq_param[IMPDRV_IRQ_GROUP_0];
	st_impdrv_irq_group_t *const p_irq_param = &irq_param;

	/* Invalid_error : Core_type = IMPDRV_CORE_TYPE_INVALID */
	p_irq_param->group_core_info[0].core_type = IMPDRV_CORE_TYPE_INVALID;
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) Core_type = IMPDRV_CORE_TYPE_INVALID", testno++);

	/* Invalid_error : Core_num = IMPDRV_CORE_NUM_MAX */
	p_irq_param->group_core_info[0].core_type = IMPDRV_CORE_TYPE_IMP;
	p_irq_param->group_core_info[0].core_num = IMPDRV_CORE_NUM_MAX;
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) Core_num = IMPDRV_CORE_NUM_MAX", testno++);

	/* Duplicate cores */
	p_irq_param->group_core_info[0].core_type = IMPDRV_CORE_TYPE_IMP;
	p_irq_param->group_core_info[0].core_num = 0U;
	p_irq_param->group_core_info[1].core_type = IMPDRV_CORE_TYPE_IMP;
	p_irq_param->group_core_info[1].core_num = 0U;
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) Duplicate cores", testno++);
}

static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	st_impdrv_irq_group_t irq_param = g_p_irq_param[IMPDRV_IRQ_GROUP_0];
	st_impdrv_irq_group_t *const p_irq_param = &irq_param;
	tp_core_list_t no_exec_cores = get_core_list(IT_IMPDRV_CMD_NO_EXEC, 0);

	/* Not supported core: Core not supported */
	for (uint32_t i = 0; i < no_exec_cores.num; i++)
	{
		irq_param.group_core_info[0] = *(no_exec_cores.core_info + i);
		ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
		FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) Core not supported: %s", testno++, type_to_name(p_irq_param->group_core_info[0].core_type));
	}
}

static void Return_error(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	st_impdrv_irq_group_t irq_param = g_p_irq_param[IMPDRV_IRQ_GROUP_0];
	st_impdrv_irq_group_t *const p_irq_param = &irq_param;

	/* Fail to lock mutex impdrv_cmnctl_mutex_lock returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_cmnctl_mutex_lock(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_cmnctl_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to lock mutex impdrv_cmnctl_mutex_lock returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_cmnctl_mutex_lock(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to lock mutex impdrv_cmnctl_mutex_lock returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_cmnctl_mutex_lock(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to get core function impdrv_cmnctl_get_corectl_func returns NULL */
	it_stub_set_impdrv_cmnctl_get_corectl_func(NULL);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) impdrv_cmnctl_get_corectl_func returns NULL", testno++);

	/* Fail to lock mutex impdrv_impctl_check_state returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_impctl_check_state(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_impctl_check_state returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to lock mutex impdrv_impctl_check_state returns IMPDRV_EC_NG_PARAM */
	it_stub_set_impdrv_impctl_check_state(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_impctl_check_state returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to lock mutex impdrv_impctl_check_state returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_impctl_check_state(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_impctl_check_state returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to lock mutex impdrv_impctl_check_state returns IMPDRV_EC_NG_NOTSUPPORT */
	it_stub_set_impdrv_impctl_check_state(IMPDRV_EC_NG_NOTSUPPORT);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) impdrv_impctl_check_state returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_NOTSUPPORT));

	/* Fail to set attribute for IMP0 impdrv_cmnctl_set_irq_group returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_cmnctl_set_irq_group(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_cmnctl_set_irq_group returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to set attribute for IMP0 impdrv_cmnctl_set_irq_group returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_cmnctl_set_irq_group(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_set_irq_group returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to set attribute for IMP0 impdrv_cmnctl_set_irq_group returns IMPDRV_EC_NG_PARAM */
	it_stub_set_impdrv_cmnctl_set_irq_group(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_set_irq_group returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to set attribute for IMP0 impdrv_cmnctl_set_irq_group returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_cmnctl_set_irq_group(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_set_irq_group returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to unlock mutex impdrv_cmnctl_mutex_unlock returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_cmnctl_mutex_unlock(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_cmnctl_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to unlock mutex impdrv_cmnctl_mutex_unlock returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_cmnctl_mutex_unlock(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to unlock mutex impdrv_cmnctl_mutex_unlock returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_cmnctl_mutex_unlock(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));
}
#endif

static void Device_set_irq_group(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	st_impdrv_irq_group_t irq_param = g_p_irq_param[IMPDRV_IRQ_GROUP_NONE];
	e_impdrv_irq_group_t group_idx[] = {
		IMPDRV_IRQ_GROUP_NONE,
		IMPDRV_IRQ_GROUP_0,
		IMPDRV_IRQ_GROUP_1,
		IMPDRV_IRQ_GROUP_2
	};
	int32_t result = 0;

	for (uint32_t i = 0; i < ARRAY_SIZE(group_idx); i++)
	{
		irq_param = g_p_irq_param[group_idx[i]];
		result = 0;

		it_stub_reset();
		ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
		if (IMPDRV_EC_OK != ercd)
		{
			testmsg("(No.%d) fail(test NG %d)\n", testno++, ercd);
			result = -1;
		}

		SuccessTestPrint(result == 0, *tret, "(No.%d) %s ", testno++, irq_group_to_name(group_idx[i]));
	}
}