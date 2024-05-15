
#define TEST_MSG_NAME "R_IMPDRV_SetIrqGroup"

#include "it_common.h"

#define CALLBACK_CODE		(0xAA)
#define CALLBACK_CODE_1		(0xAA)
#define CALLBACK_CODE_2		(0x55)

static int32_t wait_callback(void *msgcnf, st_impdrv_core_info_t *core_info, int32_t ercd);
static void set_state(st_impdrv_initdata_t *const p_data, e_impdrv_state_t state);
#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret);
static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret);
static void Mismatched_instance_group(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_instance(struct test_params_t *tparam, struct test_result_t *tret);
#endif
static void Core_exec_no_group_setting(struct test_params_t *tparam, struct test_result_t *tret);
static void Core_resume_no_group_setting(struct test_params_t *tparam, struct test_result_t *tret);
static void Core_exec_group_setting(struct test_params_t *tparam, struct test_result_t *tret);

static void *g_msgcnf;
static uint32_t testno;
static st_impdrv_coretypectl_t coretype_ctl_backup[IMPDRV_CORE_TYPE_MAX];
static bool g_callback_wait;

static int32_t wait_callback(void *msgcnf, st_impdrv_core_info_t *core_info, int32_t ercd)
{
	int32_t msg1, msg2, msg3, msg4;
	int32_t code;

	/* ercd */
	if (receive_msg(msgcnf, &msg1)) {
		LOGPRINT(("callback check : TIMEOUT\n"));
		return -1;
	}
	/* code */
	if (receive_msg(msgcnf, &msg2)) {
		LOGPRINT(("callback check : TIMEOUT\n"));
		return -1;
	}
	/* core type */
	if (receive_msg(msgcnf, &msg3)) {
		LOGPRINT(("callback check : TIMEOUT\n"));
		return -1;
	}
	else
	{
		core_info->core_type = msg3;
	}
	/* core num */
	if (receive_msg(msgcnf, &msg4)) {
		LOGPRINT(("callback check : TIMEOUT\n"));
		return -1;
	}
	else
	{
		core_info->core_num = msg4;
	}

	switch (core_info->core_type)
	{
		case IMPDRV_CORE_TYPE_IMP:
		case IMPDRV_CORE_TYPE_IMP_SLIM:
		{
			code = IMPDRV_CALLBACK_CODE_DEF;
			break;
		}
		case IMPDRV_CORE_TYPE_OCV:
		case IMPDRV_CORE_TYPE_DMAC:
		case IMPDRV_CORE_TYPE_DMAC_SLIM:
		case IMPDRV_CORE_TYPE_PSCEXE:
		{
			code = 0;
			break;
		}
		case IMPDRV_CORE_TYPE_CNN:
		{
			code = 0;
			break;
		}
		default:
		{
			return -1;
			break;
		}
	}

	return compare_callback(core_info, ercd, code, msg1, msg2);
}

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

static int32_t CbFunc(
	const st_impdrv_core_info_t *p_core_info,			/**< Core information					*/
	const e_impdrv_cb_ret_t		ercd,				/**< Error code							*/
	const int32_t				code,				/**< TRAP or INT code (8 bit is set)	*/
	const void					*p_callback_args
)
{
	if ((true ==  g_callback_wait) && (IMPDRV_CB_RET_OK == ercd))
	{
		return (send_msg(g_msgcnf, ercd) | send_msg(g_msgcnf, code) | send_msg(g_msgcnf, p_core_info->core_type) | send_msg(g_msgcnf, p_core_info->core_num));
	}
	else
	{
		return (send_msg(g_msgcnf, ercd) | send_msg(g_msgcnf, code));
	}
}

int32_t it_SetIrqGroup_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

	g_callback_wait = false;
	create_msg(&g_msgcnf, IT_OSAL_MQ_ID_SET_IRQ_GROUP);

#ifdef IT_ABNORMAL
	testno = 195;
	Parameter_error(tparam, &tret);
	Invalid_core(tparam, &tret);
	No_exec_core(tparam, &tret);
	Invalid_state(tparam, &tret);
	Mismatched_instance_group(tparam, &tret);
	Invalid_instance(tparam, &tret);
#endif

	Core_exec_no_group_setting(tparam, &tret);
	Core_resume_no_group_setting(tparam, &tret);
	Core_exec_group_setting(tparam, &tret);

	destroy_msg(g_msgcnf);

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
#endif

static void Core_exec_no_group_setting(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
    st_impdrv_initdata_t *p_initdata = &tparam->initdata;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;
	st_impdrv_irq_group_t irq_param = g_p_irq_param[IMPDRV_IRQ_GROUP_NONE];
	st_impdrv_irq_group_t *const p_irq_param = &irq_param;
	int32_t result = -1;

	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	
	if (IMPDRV_EC_OK != ercd)
	{
		testmsg("(No.%d) fail(test NG %d)\n", testno++, ercd);
	}
	else
	{
		/* driver_execute : output parameter */
		for (uint32_t i = 0; i < p_initdata->use_core_num; i++)
		{
			st_impdrv_core_info_t *p_core_info = &p_initdata->core_info[i];
			e_impdrv_core_type_t core_type = p_core_info->core_type;
			uint32_t core_num = p_core_info->core_num;
			st_impdrv_coretypectl_t *coretype_ctl = &p_impdrv_ctl->coretype_ctl[core_type];

            if (p_core_info->core_type == IMPDRV_CORE_TYPE_DSP)
			{
				continue;
			}

			result = -1;
			if (set_commandlist(p_core_info->core_type, tparam->long_claddr, CALLBACK_CODE))
			{
				core_testmsg(p_core_info, "set_commandlist error\n");
			}
			else if (IMPDRV_EC_OK != R_IMPDRV_AttrSetCl(handle, p_core_info, util_mem_get_paddr(tparam->long_claddr)))
			{
				core_testmsg(p_core_info, "Set CL data error\n");
			}
			else
			{
				ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);

				if (ercd != IMPDRV_EC_OK)
				{
					core_testmsg(p_core_info, "%s=%d\n", TEST_MSG_NAME, ercd);
				}
				else
				{
					/* Check state of the core */
					if (coretype_ctl->core_ctl[core_num].core_state != IMPDRV_STATE_EXEC)
					{
						core_testmsg(p_core_info, "core_type = %d, core_num = %u, state = %s, it is not EXEC\n",
										core_type,
										core_num,
										state_to_name(coretype_ctl->core_ctl[core_num].core_state));
					}
					if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_BEFORE_EXEC, -1))
					{
						core_testmsg(p_core_info, "callback(before exec) fail\n");
					}
					else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_OK, CALLBACK_CODE))
					{
						core_testmsg(p_core_info, "callback(trap) fail\n");
					}
					else if (coretype_ctl->core_ctl[core_num].core_state != IMPDRV_STATE_READY)
					{
						core_testmsg(p_core_info, "core_type = %d, core_num = %u, state = %s, it is not READY\n",
										core_type,
										core_num,
										state_to_name(coretype_ctl->core_ctl[core_num].core_state));
					}
					else
					{
						result = 0;
					}
				}

				util_mem_invalidate(tparam->long_claddr);
				if (result != 0)
				{
					break;
				}
			}
		}

		SuccessTestPrint(result == 0, *tret, "(No.%d) Core execution, no group setting ", testno++);
	}
}

static void Core_resume_no_group_setting(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
    st_impdrv_initdata_t *p_initdata = &tparam->initdata;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;
	st_impdrv_irq_group_t irq_param = g_p_irq_param[IMPDRV_IRQ_GROUP_NONE];
	st_impdrv_irq_group_t *const p_irq_param = &irq_param;
	int32_t result = -1;

	ercd = R_IMPDRV_SetIrqGroup(handle, p_irq_param);
	if (IMPDRV_EC_OK != ercd)
	{
		testmsg("(No.%d) fail(test NG %d)\n", testno++, ercd);
	}
	else
	{
		/* driver_execute : output parameter */
		for (uint32_t i = 0; i < p_initdata->use_core_num; i++)
		{
			st_impdrv_core_info_t *p_core_info = &p_initdata->core_info[i];
			e_impdrv_core_type_t core_type = p_core_info->core_type;
			uint32_t core_num = p_core_info->core_num;
			st_impdrv_coretypectl_t *coretype_ctl = &p_impdrv_ctl->coretype_ctl[core_type];

            if (p_core_info->core_type == IMPDRV_CORE_TYPE_DSP)
			{
				continue;
			}

			result = -1;
			if (set_intcommandlist(p_core_info->core_type, tparam->long_claddr, CALLBACK_CODE_1, CALLBACK_CODE_2))
			{
				core_testmsg(p_core_info, "set_intcommandlist error\n");
			}
			else if (IMPDRV_EC_OK != R_IMPDRV_AttrSetCl(handle, p_core_info, util_mem_get_paddr(tparam->long_claddr)))
			{
				core_testmsg(p_core_info, "Set CL data error\n");
			}
			else
			{
				ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
				if (ercd != IMPDRV_EC_OK)
				{
					core_testmsg(p_core_info, "%s=%d\n", "R_IMPDRV_Execute", ercd);
				}
				else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_BEFORE_EXEC, -1) ||
					wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_INT, CALLBACK_CODE_1))
				{
					core_testmsg(p_core_info, "callback(int) fail\n");
				}
				/* Check state of the core */
				else if (coretype_ctl->core_ctl[core_num].core_state != IMPDRV_STATE_INT)
				{
					core_testmsg(p_core_info, "core_type = %d, core_num = %u, state = %s, it is not EXEC\n",
									core_type,
									core_num,
									state_to_name(coretype_ctl->core_ctl[core_num].core_state));
				}
				else
				{
					ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
					if (ercd != IMPDRV_EC_OK)
					{
						core_testmsg(p_core_info, "%s=%d\n", TEST_MSG_NAME, ercd);
					}
					else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_OK, CALLBACK_CODE_2))
					{
						core_testmsg(p_core_info, "callback(trap) fail\n");
					}
					else if (coretype_ctl->core_ctl[core_num].core_state != IMPDRV_STATE_READY)
					{
						core_testmsg(p_core_info, "core_type = %d, core_num = %u, state = %s, it is not READY\n",
										core_type,
										core_num,
										state_to_name(coretype_ctl->core_ctl[core_num].core_state));
					}
					else
					{
						result = 0;
					}
				}
			}
			
			if (result != 0)
			{
				break;
			}
		}

		util_mem_invalidate(tparam->long_claddr);
		SuccessTestPrint(result == 0, *tret, "(No.%d) Core resume, no group setting ", testno++);
	}
}

static void Core_exec_group_setting(struct test_params_t *tparam, struct test_result_t *tret)
{
	int8_t i;
	e_impdrv_errorcode_t ercd;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;
	st_impdrv_core_info_t core_map[IMPDRV_COREMAP_MAXID];
	st_impdrv_irq_group_t irq_param = g_p_irq_param[IMPDRV_IRQ_GROUP_0];
	int32_t result = 0;
	int32_t ret = 0;
	uint32_t claddr_phys;
	uint32_t clsize;
	st_buffer_t buffer;

	/* create CL data */
	memset(&buffer, 0, sizeof(buffer));
	irq_group_createCLdata(&buffer);
	if (NULL == buffer.cpu_addr)
	{
		LOGPRINT(("<< [ERROR] Cannot create CL data>>\n"));
		result = -1;
	}
	else
	{
		/* push to hardware for execution */
		util_mem_flush(buffer.cpu_addr);
	}

	/* initialize core_map */
	for (i = 0; i < IMPDRV_COREMAP_MAXID; i++)
	{
		core_map[i].core_type = IMPDRV_CORE_TYPE_INVALID;
		core_map[i].core_num = 0 ;
	}

	if (0 == result)
	{
		ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
		if (IMPDRV_EC_OK != ercd)
		{
			testmsg("(No.%d) fail(test NG %d)\n", testno++, ercd);
			result = -1;
		}
	}

	if (0 == result)
	{
		g_callback_wait = true;
		for (i = 0; i < irq_param.group_core_num; i++)
		{
			e_impdrv_core_type_t core_type = irq_param.group_core_info[i].core_type;
			uint32_t core_num = irq_param.group_core_info[i].core_num;
			st_impdrv_coretypectl_t *coretype_ctl = &p_impdrv_ctl->coretype_ctl[core_type];

			/* prepare parameter for cl */
            ret = CLgethw_addr(&buffer, &irq_param.group_core_info[i], &claddr_phys, &clsize);
            if (0 != ret)
            {
				LOGPRINT(("<< [ERROR] Cannot HW address of CL for core %s%d>>\n", type_to_name(core_type), core_num));
				if (0 == result)
				{
					result = ret;
				}
            }
			else if (IMPDRV_EC_OK != R_IMPDRV_AttrSetCoreMap(handle, &irq_param.group_core_info[i], core_map))
			{
				core_testmsg(&irq_param.group_core_info[i], "Set CL data error for core %s%d\n", type_to_name(core_type), core_num);
				result = -1;
			}
			else if (IMPDRV_EC_OK != R_IMPDRV_AttrSetCl(handle, &irq_param.group_core_info[i], (uintptr_t)claddr_phys))
			{
				core_testmsg(&irq_param.group_core_info[i], "Set CL data error\n");
				result = -1;
			}
			else
			{
				ercd = R_IMPDRV_Execute(handle, &irq_param.group_core_info[i], callback_func, (void *)&callback_args);
				if (ercd != IMPDRV_EC_OK)
				{
					core_testmsg(&irq_param.group_core_info[i], "%s=%d\n", TEST_MSG_NAME, ercd);
					result = -1;
				}
				else
				{
					/* Check state of the core */
					if (coretype_ctl->core_ctl[core_num].core_state != IMPDRV_STATE_EXEC)
					{
						core_testmsg(&irq_param.group_core_info[i], "core_type = %d, core_num = %u, state = %s, it is not EXEC\n",
										core_type,
										core_num,
										state_to_name(coretype_ctl->core_ctl[core_num].core_state));
						result = -1;
					}
					else
					{
						if (wait_and_check_callback(g_msgcnf, &irq_param.group_core_info[i], IMPDRV_CB_RET_BEFORE_EXEC, -1))
						{
							core_testmsg(&irq_param.group_core_info[i], "callback(before exec) fail\n");
							result = -1;
						}
					}
				}
			}
		}
	}

	if (0 == result)
	{
		i = 0;
		while(i < irq_param.group_core_num)
		{
			st_impdrv_core_info_t callback_coreinfo;
			int cnt;
			
			if (0 == result)
			{
				if (wait_callback(g_msgcnf, &callback_coreinfo, IMPDRV_CB_RET_OK))
				{
					core_testmsg(&callback_coreinfo, "callback(trap) fail for core %s%d\n", type_to_name(callback_coreinfo.core_type), callback_coreinfo.core_num);
					result = -1;
				}
				else
				{
					for (cnt = 0; cnt < irq_param.group_core_num; cnt++)
					{
						if ((irq_param.group_core_info[cnt].core_type == callback_coreinfo.core_type)
							&& (irq_param.group_core_info[cnt].core_num == callback_coreinfo.core_num))
						{
							break;
						}
					}
				}
				if (cnt < irq_param.group_core_num)
				{
					e_impdrv_core_type_t core_type = irq_param.group_core_info[cnt].core_type;
					uint32_t core_num = irq_param.group_core_info[cnt].core_num;
					st_impdrv_coretypectl_t *coretype_ctl = &p_impdrv_ctl->coretype_ctl[core_type];

					if (coretype_ctl->core_ctl[core_num].core_state != IMPDRV_STATE_READY)
					{
						core_testmsg(&irq_param.group_core_info[cnt], "core_type = %d, core_num = %u, state = %s, it is not READY\n",
										core_type,
										core_num,
										state_to_name(coretype_ctl->core_ctl[core_num].core_state));
						result = -1;
					}
					else
					{
						result = 0;
					}
				}
				else
				{
					core_testmsg(&callback_coreinfo, "Core not found %s%d\n", type_to_name(callback_coreinfo.core_type), callback_coreinfo.core_num);
					result = -1;
				}
			}
			i++;
		}
	}
	g_callback_wait = false;

	util_mem_invalidate(buffer.cpu_addr);
	SuccessTestPrint(result == 0, *tret, "(No.%d) Core exec, group 0 selected ", testno++);
}