
#define TEST_MSG_NAME "R_IMPDRV_SetIrqGroup"

#include "it_common.h"

#define CALLBACK_CODE		(0xAA)
#define CALLBACK_CODE_1		(0xAA)
#define CALLBACK_CODE_2		(0x55)

static void set_state(st_impdrv_initdata_t *const p_data, e_impdrv_state_t state);
#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret);
static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret);
static void Mismatched_instance_group(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_instance(struct test_params_t *tparam, struct test_result_t *tret);
static void Fault_injection_mutex(struct test_params_t *tparam, struct test_result_t *tret);
static void Fault_injection_no_group(struct test_params_t *tparam, struct test_result_t *tret);
static void Fault_injection_group(struct test_params_t *tparam, struct test_result_t *tret);
#endif
static void Device_set_irq_group(struct test_params_t *tparam, struct test_result_t *tret);

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

int32_t it_SetIrqGroup_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

#ifdef IT_ABNORMAL
	testno = 33;
	Parameter_error(tparam, &tret);
	Invalid_core(tparam, &tret);
	No_exec_core(tparam, &tret);
	Invalid_state(tparam, &tret);
	Mismatched_instance_group(tparam, &tret);
	Invalid_instance(tparam, &tret);
	Fault_injection_mutex(tparam, &tret);
	Fault_injection_no_group(tparam, &tret);
	Fault_injection_group(tparam, &tret);
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

	/* Parameter_error : handle == NULL */
	ercd = R_IMPDRV_SetIrqGroup(NULL, &irq_param);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) handle == NULL", testno++);

	/* Parameter_error : p_irq_param == NULL */
	ercd = R_IMPDRV_SetIrqGroup(handle, NULL);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) p_irq_param == NULL", testno++);

	/* Parameter_error : irq_param.irq_group == IMPDRV_IRQ_GROUP_INVALID */
	irq_param.irq_group = IMPDRV_IRQ_GROUP_INVALID;
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) irq_param.irq_group == IMPDRV_IRQ_GROUP_INVALID", testno++);

	/* Parameter_error : irq_param.irq_group == 5 */
	irq_param.irq_group = 5U;
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) irq_param.irq_group == 5", testno++);
	irq_param = g_p_irq_param[IMPDRV_IRQ_GROUP_NONE];

	/* Parameter_error : irq_param.group_core_num = 0 */
	irq_param.group_core_num = 0U;
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) irq_param.group_core_num = 0", testno++);

	/* Parameter_error : irq_param.group_core_num = 22U */
	irq_param.group_core_num = 22U;
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) irq_param.group_core_num = 22", testno++);
}

static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	st_impdrv_irq_group_t irq_param = g_p_irq_param[IMPDRV_IRQ_GROUP_0];
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
#elif defined (V3U)
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

		irq_param.group_core_info[0] = *(invalid_core_list.core_info + i);
		ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
		FailTestPrint(ercd, ((irq_param.group_core_info[0].core_type == IMPDRV_CORE_TYPE_INVALID) ?
							IMPDRV_EC_NG_PARAM : IMPDRV_EC_NG_NOTSUPPORT),
							*tret, "(No.%d) Invalid core num for %s", testno++, type_to_name(irq_param.group_core_info[0].core_type));
	}

	/* Invalid_error : Core_num = IMPDRV_CORE_NUM_MAX */
	irq_param.group_core_info[0].core_type = IMPDRV_CORE_TYPE_IMP;
	irq_param.group_core_info[0].core_num = IMPDRV_CORE_NUM_MAX;
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) Core_num = IMPDRV_CORE_NUM_MAX", testno++);

	/* Duplicate cores */
	irq_param.group_core_info[0].core_type = IMPDRV_CORE_TYPE_IMP;
	irq_param.group_core_info[0].core_num = 0U;
	irq_param.group_core_info[1].core_type = IMPDRV_CORE_TYPE_IMP;
	irq_param.group_core_info[1].core_num = 0U;
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) Duplicate cores", testno++);
}

static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	st_impdrv_irq_group_t irq_param = g_p_irq_param[IMPDRV_IRQ_GROUP_0];
	tp_core_list_t no_exec_cores = get_core_list(IT_IMPDRV_CMD_NO_EXEC, 0);

	/* Not supported core: Core not supported */
	for (uint32_t i = 0; i < no_exec_cores.num; i++)
	{
		irq_param.group_core_info[0] = *(no_exec_cores.core_info + i);
		ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
		FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) Core not supported: %s", testno++, type_to_name(irq_param.group_core_info[0].core_type));
	}
}

static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	st_impdrv_irq_group_t irq_param = g_p_irq_param[IMPDRV_IRQ_GROUP_0];
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
		ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
		FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) %s", testno++, state_to_name(invalid_state[i]));
	}

	/* Get backup data */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
}

static void Mismatched_instance_group(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	st_impdrv_irq_group_t irq_param;

	/* Back up instance */
	const e_impdrv_instance_t       instance_num_backup = p_impdrv_ctl->common_ctl.instance_num;

	/* Instance 0, group 1 */
	p_impdrv_ctl->common_ctl.instance_num = IMPDRV_INSTANCE_0;
	irq_param = g_p_irq_param[IMPDRV_IRQ_GROUP_1];
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) %s for %s", testno++, instance_to_name(p_impdrv_ctl->common_ctl.instance_num), irq_group_to_name(irq_param.irq_group));

	/* Instance 0, group 2 */
	p_impdrv_ctl->common_ctl.instance_num = IMPDRV_INSTANCE_0;
	irq_param = g_p_irq_param[IMPDRV_IRQ_GROUP_2];
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) %s for %s", testno++, instance_to_name(p_impdrv_ctl->common_ctl.instance_num), irq_group_to_name(irq_param.irq_group));

	/* Instance 1, group 0 */
	p_impdrv_ctl->common_ctl.instance_num = IMPDRV_INSTANCE_1;
	irq_param = g_p_irq_param[IMPDRV_IRQ_GROUP_0];
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) %s for %s", testno++, instance_to_name(p_impdrv_ctl->common_ctl.instance_num), irq_group_to_name(irq_param.irq_group));

	/* Instance 1, group 2 */
	p_impdrv_ctl->common_ctl.instance_num = IMPDRV_INSTANCE_1;
	irq_param = g_p_irq_param[IMPDRV_IRQ_GROUP_2];
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) %s for %s", testno++, instance_to_name(p_impdrv_ctl->common_ctl.instance_num), irq_group_to_name(irq_param.irq_group));

	/* Instance 2, group 0 */
	p_impdrv_ctl->common_ctl.instance_num = IMPDRV_INSTANCE_2;
	irq_param = g_p_irq_param[IMPDRV_IRQ_GROUP_0];
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) %s for %s", testno++, instance_to_name(p_impdrv_ctl->common_ctl.instance_num), irq_group_to_name(irq_param.irq_group));

	/* Instance 2, group 1 */
	p_impdrv_ctl->common_ctl.instance_num = IMPDRV_INSTANCE_2;
	irq_param = g_p_irq_param[IMPDRV_IRQ_GROUP_1];
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) %s for %s", testno++, instance_to_name(p_impdrv_ctl->common_ctl.instance_num), irq_group_to_name(irq_param.irq_group));

	/* Get backup data */
	p_impdrv_ctl->common_ctl.instance_num = instance_num_backup;
}

static void Invalid_instance(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	st_impdrv_irq_group_t irq_param;
	st_impdrv_irq_group_t *p_irq_param = &irq_param;
	const e_impdrv_instance_t       instance_num_backup = p_impdrv_ctl->common_ctl.instance_num;
	e_impdrv_instance_t instance_list[] = {
		IMPDRV_INSTANCE_5,
		IMPDRV_INSTANCE_6
	};

	for (uint32_t j = IMPDRV_IRQ_GROUP_NONE; j < ARRAY_SIZE(g_p_irq_param); j++)
	{
		irq_param = g_p_irq_param[j];
		p_irq_param = &irq_param;

		for (uint32_t i = 0; i < ARRAY_SIZE(instance_list); i++)
		{
			p_impdrv_ctl->common_ctl.instance_num = instance_list[i];
			ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
			FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) %s for %s", testno++, instance_to_name(instance_list[i]), irq_group_to_name(j));
		}

		p_impdrv_ctl->common_ctl.instance_num = IMPDRV_INSTANCE_6 + 1;
		ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
		FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) %s for %s", testno++, instance_to_name(IMPDRV_INSTANCE_6 + 1), irq_group_to_name(j));
	}

	/* Get backup data */
	p_impdrv_ctl->common_ctl.instance_num = instance_num_backup;
}

static void Fault_injection_mutex(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	st_impdrv_irq_group_t irq_param = g_p_irq_param[IMPDRV_IRQ_GROUP_0];
	st_impdrv_irq_group_t *const p_irq_param = &irq_param;

	/* Fail to lock mutex impdrv_osdep_mutex_lock returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_osdep_mutex_lock(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to lock mutex impdrv_osdep_mutex_lock returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_osdep_mutex_lock(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to lock mutex impdrv_osdep_mutex_lock returns IMPDRV_EC_NG_PARAM */
	it_stub_set_impdrv_osdep_mutex_lock(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to lock mutex impdrv_osdep_mutex_lock returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_osdep_mutex_lock(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to unlock mutex impdrv_osdep_mutex_unlock returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_osdep_mutex_unlock(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to unlock mutex impdrv_osdep_mutex_unlock returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_osdep_mutex_unlock(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to unlock mutex impdrv_osdep_mutex_unlock returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_osdep_mutex_unlock(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));
}

static void Fault_injection_no_group(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	st_impdrv_irq_group_t irq_param = g_p_irq_param[IMPDRV_IRQ_GROUP_NONE];
	st_impdrv_irq_group_t *const p_irq_param = &irq_param;
	st_impdrv_group_tbl_t *p_inten_group_table = s_impdrv_inten_reg_tbl[p_initdata->instance_num].p_inten_group_table;
	e_impdrv_irq_group_t irq_group = -1;

	if ((IMPDRV_EC_OK != util_get_group(&irq_group, p_initdata->instance_num)) || (-1 == irq_group)) {
		printf("Instance number is not supported\n");
		goto _error;
	}

	/* Fail to read register impdrv_osdep_read_reg for Interrupt Enable returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_SEQSTATE, 0, 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_read_reg for Interrupt Enable returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to read register impdrv_osdep_read_reg for Interrupt Enable returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_ARGNULL, 0, 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_read_reg for Interrupt Enable returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to read register impdrv_osdep_read_reg for Interrupt Enable returns IMPDRV_EC_NG_PARAM */
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_PARAM, 0, 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_read_reg for Interrupt Enable returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));
	
	/* Fail to read register impdrv_osdep_read_reg for Interrupt Enable returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_SYSTEMERROR, 0, 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_read_reg for Interrupt Enable returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to read register impdrv_osdep_read_reg for Interrupt Mask returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_SEQSTATE, 1, 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_read_reg for Interrupt Mask returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to read register impdrv_osdep_read_reg for Interrupt Mask returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_ARGNULL, 1, 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_read_reg for Interrupt Mask returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to read register impdrv_osdep_read_reg for Interrupt Mask returns IMPDRV_EC_NG_PARAM */
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_PARAM, 1, 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_read_reg for Interrupt Mask returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));
	
	/* Fail to read register impdrv_osdep_read_reg for Interrupt Mask returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_SYSTEMERROR, 1, 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_read_reg for Interrupt Mask returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to read register impdrv_osdep_write_reg for Interrupt Select returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_OK, 1, 5);
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_SEQSTATE, p_inten_group_table[irq_group].group_reg);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_write_reg for Interrupt Select returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to read register impdrv_osdep_write_reg for Interrupt Select returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_OK, 1, 5);
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_ARGNULL, p_inten_group_table[irq_group].group_reg);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_write_reg for Interrupt Select returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to read register impdrv_osdep_write_reg for Interrupt Select returns IMPDRV_EC_NG_PARAM */
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_OK, 1, 5);
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_PARAM, p_inten_group_table[irq_group].group_reg);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_write_reg for Interrupt Select returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));
	
	/* Fail to read register impdrv_osdep_write_reg for Interrupt Select returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_OK, 1, 5);
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_SYSTEMERROR, p_inten_group_table[irq_group].group_reg);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_write_reg for Interrupt Select returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to read register impdrv_osdep_write_reg for Interrupt Enable returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_SEQSTATE, s_impdrv_inten_addr[p_initdata->instance_num]);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_write_reg for Interrupt Enable returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to read register impdrv_osdep_write_reg for Interrupt Enable returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_ARGNULL, s_impdrv_inten_addr[p_initdata->instance_num]);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_write_reg for Interrupt Enable returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to read register impdrv_osdep_write_reg for Interrupt Enable returns IMPDRV_EC_NG_PARAM */
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_PARAM, s_impdrv_inten_addr[p_initdata->instance_num]);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_write_reg for Interrupt Enable returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));
	
	/* Fail to read register impdrv_osdep_write_reg for Interrupt Enable returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_SYSTEMERROR, s_impdrv_inten_addr[p_initdata->instance_num]);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_write_reg for Interrupt Enable returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

_error:
	return;
}

static void Fault_injection_group(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	st_impdrv_irq_group_t irq_param = g_p_irq_param[IMPDRV_IRQ_GROUP_0];
	st_impdrv_irq_group_t *const p_irq_param = &irq_param;
	st_impdrv_group_tbl_t *p_inten_group_table = s_impdrv_inten_reg_tbl[p_initdata->instance_num].p_inten_group_table;
	e_impdrv_irq_group_t irq_group = -1;

	if ((IMPDRV_EC_OK != util_get_group(&irq_group, p_initdata->instance_num)) || (-1 == irq_group)) {
		printf("Instance number is not supported\n");
		goto _error;
	}

	/* Fail to read register impdrv_osdep_read_reg for Interrupt Enable returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_SEQSTATE, 0, 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_read_reg for Interrupt Enable returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to read register impdrv_osdep_read_reg for Interrupt Enable returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_ARGNULL, 0, 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_read_reg for Interrupt Enable returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to read register impdrv_osdep_read_reg for Interrupt Enable returns IMPDRV_EC_NG_PARAM */
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_PARAM, 0, 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_read_reg for Interrupt Enable returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));
	
	/* Fail to read register impdrv_osdep_read_reg for Interrupt Enable returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_SYSTEMERROR, 0, 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_read_reg for Interrupt Enable returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to read register impdrv_osdep_read_reg for Interrupt Mask returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_SEQSTATE, 1, 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_read_reg for Interrupt Mask returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to read register impdrv_osdep_read_reg for Interrupt Mask returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_ARGNULL, 1, 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_read_reg for Interrupt Mask returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to read register impdrv_osdep_read_reg for Interrupt Mask returns IMPDRV_EC_NG_PARAM */
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_PARAM, 1, 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_read_reg for Interrupt Mask returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));
	
	/* Fail to read register impdrv_osdep_read_reg for Interrupt Mask returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_SYSTEMERROR, 1, 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_read_reg for Interrupt Mask returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to read register impdrv_osdep_write_reg for Interrupt Select returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_OK, 1, 5);
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_SEQSTATE, p_inten_group_table[irq_group].group_reg);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_write_reg for Interrupt Select returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to read register impdrv_osdep_write_reg for Interrupt Select returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_OK, 1, 5);
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_ARGNULL, p_inten_group_table[irq_group].group_reg);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_write_reg for Interrupt Select returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to read register impdrv_osdep_write_reg for Interrupt Select returns IMPDRV_EC_NG_PARAM */
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_OK, 1, 5);
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_PARAM, p_inten_group_table[irq_group].group_reg);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_write_reg for Interrupt Select returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));
	
	/* Fail to read register impdrv_osdep_write_reg for Interrupt Select returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_OK, 1, 5);
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_SYSTEMERROR, p_inten_group_table[irq_group].group_reg);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_write_reg for Interrupt Select returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to read register impdrv_osdep_write_reg for Interrupt Enable returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_SEQSTATE, s_impdrv_inten_addr[p_initdata->instance_num]);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_write_reg for Interrupt Enable returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to read register impdrv_osdep_write_reg for Interrupt Enable returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_ARGNULL, s_impdrv_inten_addr[p_initdata->instance_num]);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_write_reg for Interrupt Enable returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to read register impdrv_osdep_write_reg for Interrupt Enable returns IMPDRV_EC_NG_PARAM */
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_PARAM, s_impdrv_inten_addr[p_initdata->instance_num]);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_write_reg for Interrupt Enable returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));
	
	/* Fail to read register impdrv_osdep_write_reg for Interrupt Enable returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_SYSTEMERROR, s_impdrv_inten_addr[p_initdata->instance_num]);
	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_write_reg for Interrupt Enable returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

_error:
	return;
}
#endif

static void Device_set_irq_group(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	st_impdrv_irq_group_t irq_param;
	e_impdrv_irq_group_t group_idx[] = {
		IMPDRV_IRQ_GROUP_0,
		IMPDRV_IRQ_GROUP_NONE
	};
	int32_t result = 0;

	for (uint32_t i = 0; i < ARRAY_SIZE(group_idx); i++)
	{
		irq_param = g_p_irq_param[group_idx[i]];
		result = 0;

		ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
		if (IMPDRV_EC_OK != ercd)
		{
			testmsg("fail(test NG %d)\n", ercd);
			result = -1;
		}

		SuccessTestPrint(result == 0, *tret, "(No.%d) %s ", testno++, irq_group_to_name(group_idx[i]));
	}
}