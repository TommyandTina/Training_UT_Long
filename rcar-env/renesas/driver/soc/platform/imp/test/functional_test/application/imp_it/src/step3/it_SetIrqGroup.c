#define TEST_MSG_NAME "R_IMPDRV_SetIrqGroup"

#include "it_common.h"
#include "it_stub.h"

#define CALLBACK_CODE		(0xAA)
#define CALLBACK_CODE_1		(0xAA)
#define CALLBACK_CODE_2		(0x55)

static void set_state(st_impdrv_initdata_t *const p_data, e_impdrv_state_t state);
static void prepare_test_for_set_irq_group(const st_impdrv_commonctl_t *const p_commonctl, const st_impdrv_irq_group_t *const p_irq_param);
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

static void prepare_test_for_set_irq_group(
	const st_impdrv_commonctl_t *const p_commonctl,
    const st_impdrv_irq_group_t *const p_irq_param
)
{
#if defined (V4H) || defined (V4H2)
	const st_impdrv_group_tbl_t     *p_inten_group_table;
    const st_impdrv_inten_tbl_t     *p_inten_cnv_table;
    uint32_t                        cnv_table_num;
    uint32_t                        write_val;
	uint32_t						i = 0U;
	uint32_t						read_inten = 0U;
	uint32_t                		inten = 0U;
    uint32_t                		read_intsel = 0U;
    uint32_t                		intsel = 0U;
	e_impdrv_irq_group_t    		irq_group;

	p_inten_cnv_table = s_impdrv_inten_reg_tbl[p_commonctl->instance_num].p_inten_cnv_table;
	cnv_table_num = s_impdrv_inten_reg_tbl[p_commonctl->instance_num].cnv_table_num;

	cnv_int_core_bit(
		p_irq_param->group_core_info,
		p_irq_param->group_core_num,
		p_inten_cnv_table,
		cnv_table_num,
		&write_val
	);

	p_inten_group_table = s_impdrv_inten_reg_tbl[p_commonctl->instance_num].p_inten_group_table;

	if (IMPDRV_IRQ_GROUP_NONE == p_irq_param->irq_group)
	{
    	uint32_t                		write_mask_val;
		i = 0;

		util_get_group(&irq_group, p_commonctl->instance_num);

		/* Switching to No interrupt group */
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, read_inten);
		inten = read_inten;

		/* Care for group interrupt bit in INTerrupt Mask register */
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, read_intsel);
		intsel = read_intsel & ~(write_val);

		if (IMPDRV_REG_GROUP_INIT_VAL == intsel)
		{
			inten &= ~(p_inten_group_table[irq_group].group_bit);
		}

		if (intsel != read_intsel)
		{
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, intsel);
		}

		/* Set the INTerrupt Mask register */
		inten = inten | write_val;
		if (inten != read_inten)
		{
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, inten);
		}
	}
	else
	{
		i = 0;

		/* Switching to interrupt group */
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, read_inten);
		inten = read_inten;

		/* Care for group interrupt bit in INTerrupt Mask register */
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, read_intsel);

		/* Care for group interrupt bit in INTerrupt Mask register */
		intsel = read_intsel | write_val;
		inten |= p_inten_group_table[p_irq_param->irq_group].group_bit;
		if (intsel != read_intsel)
		{
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, intsel);
		}
		/** Set the INTerrupt Mask register */
        inten = inten & ~ (write_val);
		if (inten != read_inten)
		{
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, inten);
		}
	}
#elif (defined (V3M2) || defined (V3H1) || defined (V3H2))
	const st_impdrv_group_tbl_t     *p_inten_group_table;
    const st_impdrv_inten_tbl_t     *p_inten_cnv_table;
    uint32_t                        cnv_table_num;
    uint32_t                        write_val;
	uint32_t						i = 0U;
	uint32_t						read_inten = 0U;
	uint32_t                		inten = 0U;
    uint32_t                		read_intsel = 0U;
    uint32_t                		intsel = 0U;
	e_impdrv_irq_group_t    		irq_group;

	p_inten_cnv_table = s_impdrv_inten_reg_tbl[p_commonctl->instance_num].p_inten_cnv_table;
	cnv_table_num = s_impdrv_inten_reg_tbl[p_commonctl->instance_num].cnv_table_num;

	cnv_int_core_bit(
		p_irq_param->group_core_info,
		p_irq_param->group_core_num,
		p_inten_cnv_table,
		cnv_table_num,
		&write_val
	);

	p_inten_group_table = s_impdrv_inten_reg_tbl[p_commonctl->instance_num].p_inten_group_table;

	if (IMPDRV_IRQ_GROUP_NONE == p_irq_param->irq_group)
	{
    	uint32_t                		write_mask_val;
		i = 0;

		write_mask_val = ~write_val & IMPDRV_REG_IMR_INIT_VAL;

		util_get_group(&irq_group, p_commonctl->instance_num);

		/* Switching to No interrupt group */
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, read_inten);
		inten = read_inten;

		/* Care for group interrupt bit in INTerrupt Mask register */
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, read_intsel);
		intsel = read_intsel & ~(write_val);

		if (IMPDRV_REG_GROUP_INIT_VAL == intsel)
		{
			inten |= p_inten_group_table[irq_group].group_bit;
		}

		if (intsel != read_intsel)
		{
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, intsel);
		}

		/* Set the INTerrupt Mask register */
		inten = inten & write_mask_val;
		if (inten != read_inten)
		{
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, inten);
		}
	}
	else
	{
		i = 0;

		/* Switching to interrupt group */
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, read_inten);
		inten = read_inten;

		/* Care for group interrupt bit in INTerrupt Mask register */
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, read_intsel);

		/* Care for group interrupt bit in INTerrupt Mask register */
		intsel = read_intsel | write_val;
		inten &= ~(p_inten_group_table[p_irq_param->irq_group].group_bit);
		if (intsel != read_intsel)
		{
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, intsel);
		}

		/** Set the INTerrupt Mask register */
        inten = inten | write_val;
		if (inten != read_inten)
		{
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, inten);
		}
	}
#else
	#error 'Oops ...! Build option for unsupported products.'
#endif
}

int32_t it_SetIrqGroup_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

#ifdef IT_ABNORMAL
	testno = 117;
	Parameter_error(tparam, &tret);
	testno = 123;
	Invalid_core(tparam, &tret);
	testno = 134;
	No_exec_core(tparam, &tret);
	testno = 138;
	Invalid_state(tparam, &tret);
	testno = 142;
	Mismatched_instance_group(tparam, &tret);
	testno = 148;
	Invalid_instance(tparam, &tret);
	testno = 160;
	Fault_injection_mutex(tparam, &tret);
	testno = 169;
	Fault_injection_no_group(tparam, &tret);
	testno = 181;
	Fault_injection_group(tparam, &tret);
#endif
	testno = 193;
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
	const e_impdrv_instance_t       instance_num_backup = p_impdrv_ctl->common_ctl.instance_num;
	e_impdrv_instance_t instance_list[] = {
		IMPDRV_INSTANCE_5,
		IMPDRV_INSTANCE_6
	};

	for (uint32_t j = IMPDRV_IRQ_GROUP_NONE; j < ARRAY_SIZE(g_p_irq_param); j++)
	{
		irq_param = g_p_irq_param[j];

		for (uint32_t i = 0; i < ARRAY_SIZE(instance_list); i++)
		{
			p_impdrv_ctl->common_ctl.instance_num = instance_list[i];
			ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
			FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) %s for %s", testno++, instance_to_name(instance_list[i]), irq_group_to_name(j));
		}

		p_impdrv_ctl->common_ctl.instance_num = IMPDRV_INSTANCE_6 + 1;
		ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
		FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) %s for %s", testno++, instance_to_name(IMPDRV_INSTANCE_6 + 1), irq_group_to_name(j));
	}

	/* Get backup data */
	p_impdrv_ctl->common_ctl.instance_num = instance_num_backup;
}

static void Fault_injection_mutex(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
	st_impdrv_commonctl_t *p_commonctl = &p_impdrv_ctl->common_ctl;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	st_impdrv_irq_group_t irq_param = g_p_irq_param[IMPDRV_IRQ_GROUP_0];
	st_impdrv_irq_group_t *const p_irq_param = &irq_param;

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_TIME */
	prepare_test_for_set_irq_group(p_commonctl, &irq_param);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_TIME);
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_TIME));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_PAR */
	prepare_test_for_set_irq_group(p_commonctl, &irq_param);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_PAR);
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_BUSY */
	prepare_test_for_set_irq_group(p_commonctl, &irq_param);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_BUSY);
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_BUSY));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_HANDLE */
	prepare_test_for_set_irq_group(p_commonctl, &irq_param);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_STATE */
	prepare_test_for_set_irq_group(p_commonctl, &irq_param);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_FAIL */
	prepare_test_for_set_irq_group(p_commonctl, &irq_param);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_HANDLE */
	prepare_test_for_set_irq_group(p_commonctl, &irq_param);
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_STATE */
	prepare_test_for_set_irq_group(p_commonctl, &irq_param);
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_FAIL */
	prepare_test_for_set_irq_group(p_commonctl, &irq_param);
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));
}

static void Fault_injection_no_group(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
	st_impdrv_commonctl_t *p_commonctl = &p_impdrv_ctl->common_ctl;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	st_impdrv_irq_group_t irq_param = g_p_irq_param[IMPDRV_IRQ_GROUP_NONE];
	st_impdrv_group_tbl_t *p_inten_group_table = s_impdrv_inten_reg_tbl[p_initdata->instance_num].p_inten_group_table;

	/* Fail to read Interrupt Enable register (No group): R_OSAL_IoRead32 returns OSAL_RETURN_PAR */
	prepare_test_for_set_irq_group(p_commonctl, &irq_param);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_PAR, 0, 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for Interrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to read Interrupt Enable register (No group): R_OSAL_IoRead32 returns OSAL_RETURN_DEV */
	prepare_test_for_set_irq_group(p_commonctl, &irq_param);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_DEV, 0, 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for Interrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to read Interrupt Enable register (No group): R_OSAL_IoRead32 returns OSAL_RETURN_IO */
	prepare_test_for_set_irq_group(p_commonctl, &irq_param);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_IO, 0, 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for Interrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_IO));

	/* Fail to read Interrupt Enable register (No group): R_OSAL_IoRead32 returns OSAL_RETURN_HANDLE */
	prepare_test_for_set_irq_group(p_commonctl, &irq_param);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_HANDLE, 0, 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for Interrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to read Interrupt Enable register (No group): R_OSAL_IoRead32 returns OSAL_RETURN_STATE */
	prepare_test_for_set_irq_group(p_commonctl, &irq_param);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_STATE, 0, 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for Interrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to read Interrupt Enable register (No group): R_OSAL_IoRead32 returns OSAL_RETURN_FAIL */
	prepare_test_for_set_irq_group(p_commonctl, &irq_param);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_FAIL, 0, 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for Interrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Fail to read Interrupt Enable register (No group): R_OSAL_IoWrite32 returns OSAL_RETURN_PAR */
	prepare_test_for_set_irq_group(p_commonctl, &irq_param);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_PAR, s_impdrv_inten_addr[p_impdrv_ctl->common_ctl.instance_num], 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Interrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to read Interrupt Enable register (No group): R_OSAL_IoWrite32 returns OSAL_RETURN_DEV */
	prepare_test_for_set_irq_group(p_commonctl, &irq_param);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_DEV, s_impdrv_inten_addr[p_impdrv_ctl->common_ctl.instance_num], 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Interrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to read Interrupt Enable register (No group): R_OSAL_IoWrite32 returns OSAL_RETURN_IO */
	prepare_test_for_set_irq_group(p_commonctl, &irq_param);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_IO, s_impdrv_inten_addr[p_impdrv_ctl->common_ctl.instance_num], 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Interrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_IO));

	/* Fail to read Interrupt Enable register (No group): R_OSAL_IoWrite32 returns OSAL_RETURN_HANDLE */
	prepare_test_for_set_irq_group(p_commonctl, &irq_param);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_HANDLE, s_impdrv_inten_addr[p_impdrv_ctl->common_ctl.instance_num], 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Interrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to read Interrupt Enable register (No group): R_OSAL_IoWrite32 returns OSAL_RETURN_STATE */
	prepare_test_for_set_irq_group(p_commonctl, &irq_param);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_STATE, s_impdrv_inten_addr[p_impdrv_ctl->common_ctl.instance_num], 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Interrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to read Interrupt Enable register (No group): R_OSAL_IoWrite32 returns OSAL_RETURN_FAIL */
	prepare_test_for_set_irq_group(p_commonctl, &irq_param);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_FAIL, s_impdrv_inten_addr[p_impdrv_ctl->common_ctl.instance_num], 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Interrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));
}

static void Fault_injection_group(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
	st_impdrv_commonctl_t *p_commonctl = &p_impdrv_ctl->common_ctl;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	st_impdrv_irq_group_t irq_param = g_p_irq_param[IMPDRV_IRQ_GROUP_0];
	st_impdrv_group_tbl_t *p_inten_group_table = s_impdrv_inten_reg_tbl[p_initdata->instance_num].p_inten_group_table;

	/* Fail to read Interrupt Enable register (No group): R_OSAL_IoRead32 returns OSAL_RETURN_PAR */
	prepare_test_for_set_irq_group(p_commonctl, &irq_param);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_PAR, 0, 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for Interrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to read Interrupt Enable register (No group): R_OSAL_IoRead32 returns OSAL_RETURN_DEV */
	prepare_test_for_set_irq_group(p_commonctl, &irq_param);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_DEV, 0, 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for Interrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to read Interrupt Enable register (No group): R_OSAL_IoRead32 returns OSAL_RETURN_IO */
	prepare_test_for_set_irq_group(p_commonctl, &irq_param);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_IO, 0, 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for Interrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_IO));

	/* Fail to read Interrupt Enable register (No group): R_OSAL_IoRead32 returns OSAL_RETURN_HANDLE */
	prepare_test_for_set_irq_group(p_commonctl, &irq_param);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_HANDLE, 0, 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for Interrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to read Interrupt Enable register (No group): R_OSAL_IoRead32 returns OSAL_RETURN_STATE */
	prepare_test_for_set_irq_group(p_commonctl, &irq_param);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_STATE, 0, 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for Interrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to read Interrupt Enable register (No group): R_OSAL_IoRead32 returns OSAL_RETURN_FAIL */
	prepare_test_for_set_irq_group(p_commonctl, &irq_param);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_FAIL, 0, 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for Interrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Fail to read Interrupt Enable register (No group): R_OSAL_IoWrite32 returns OSAL_RETURN_PAR */
	prepare_test_for_set_irq_group(p_commonctl, &irq_param);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_PAR, s_impdrv_inten_addr[p_impdrv_ctl->common_ctl.instance_num], 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Interrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to read Interrupt Enable register (No group): R_OSAL_IoWrite32 returns OSAL_RETURN_DEV */
	prepare_test_for_set_irq_group(p_commonctl, &irq_param);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_DEV, s_impdrv_inten_addr[p_impdrv_ctl->common_ctl.instance_num], 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Interrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to read Interrupt Enable register (No group): R_OSAL_IoWrite32 returns OSAL_RETURN_IO */
	prepare_test_for_set_irq_group(p_commonctl, &irq_param);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_IO, s_impdrv_inten_addr[p_impdrv_ctl->common_ctl.instance_num], 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Interrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_IO));

	/* Fail to read Interrupt Enable register (No group): R_OSAL_IoWrite32 returns OSAL_RETURN_HANDLE */
	prepare_test_for_set_irq_group(p_commonctl, &irq_param);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_HANDLE, s_impdrv_inten_addr[p_impdrv_ctl->common_ctl.instance_num], 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Interrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to read Interrupt Enable register (No group): R_OSAL_IoWrite32 returns OSAL_RETURN_STATE */
	prepare_test_for_set_irq_group(p_commonctl, &irq_param);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_STATE, s_impdrv_inten_addr[p_impdrv_ctl->common_ctl.instance_num], 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Interrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to read Interrupt Enable register (No group): R_OSAL_IoWrite32 returns OSAL_RETURN_FAIL */
	prepare_test_for_set_irq_group(p_commonctl, &irq_param);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_FAIL, s_impdrv_inten_addr[p_impdrv_ctl->common_ctl.instance_num], 0);
	ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Interrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));
}
#endif

static void Device_set_irq_group(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
	st_impdrv_commonctl_t *p_commonctl = &p_impdrv_ctl->common_ctl;
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

		it_stub_reset();
		prepare_test_for_set_irq_group(p_commonctl, &irq_param);
		ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
		if (IMPDRV_EC_OK != ercd)
		{
			testmsg("fail(test NG %d)\n", ercd);
			result = -1;
		}

		SuccessTestPrint(result == 0, *tret, "(No.%d) %s ", testno++, irq_group_to_name(group_idx[i]));
	}
}