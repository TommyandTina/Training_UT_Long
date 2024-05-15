
#define TEST_MSG_NAME "R_IMPDRV_Execute"

#include "it_common.h"

#define CALLBACK_CODE 0xAA

static void set_state(st_impdrv_initdata_t *const p_data, e_impdrv_state_t state);
#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret);
static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret);
#endif
static void Core_exec(struct test_params_t *tparam, struct test_result_t *tret);

static void *g_msgcnf;
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

static int32_t CbFunc(
	const st_impdrv_core_info_t *p_core_info,		/**< Core information					*/
	const e_impdrv_cb_ret_t		ercd,				/**< Error code							*/
	const int32_t				code,				/**< TRAP or INT code (8 bit is set)	*/
	const void					*p_callback_args
)
{
	return (send_msg(g_msgcnf, ercd) | send_msg(g_msgcnf, code));
}

int32_t it_Execute_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

	create_msg(&g_msgcnf, IT_OSAL_MQ_ID_CORE_EXEC1);

#ifdef IT_ABNORMAL
	testno = 212;
	Parameter_error(tparam, &tret);
	Invalid_core(tparam, &tret);
	No_exec_core(tparam, &tret);
	Invalid_state(tparam, &tret);
#endif
	Core_exec(tparam, &tret);

	destroy_msg(g_msgcnf);

	PrintTestResult(tret, test_result);

	return 0;
}

#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_core_info_t *p_core_info = tparam->initdata.core_info;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
    p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;
	uint32_t claddr_phys_backup;

	/* Parameter_error : handle == NULL */
	ercd = R_IMPDRV_Execute(NULL, p_core_info, callback_func, (void *)&callback_args);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) handle == NULL", testno++);

	/* Parameter_error : p_core_info == NULL */
	ercd = R_IMPDRV_Execute(handle, NULL, callback_func, (void *)&callback_args);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) p_core_info == NULL", testno++);

	/* Parameter_error : callback_func == NULL */
	ercd = R_IMPDRV_Execute(handle, p_core_info, NULL, (void *)&callback_args);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) callback_func == NULL", testno++);

	/* Parameter_error : claddr_phys for IMP0 is IMPDRV_INIT_CL_ADDR */
	claddr_phys_backup = p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].claddr_phys;
	p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].claddr_phys = IMPDRV_INIT_CL_ADDR;
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	FailTestPrint(ercd, IMPDRV_EC_NG_ATTRIBUTE, *tret, "(No.%d) claddr_phys for IMP0 is IMPDRV_INIT_CL_ADDR", testno++);
	p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].claddr_phys = claddr_phys_backup;
}

static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_IMP, 0 };
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;
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
		ercd = R_IMPDRV_Execute(handle, &core_info, callback_func, (void *)&callback_args);
		FailTestPrint(ercd, ((core_info.core_type == IMPDRV_CORE_TYPE_INVALID) ?
							IMPDRV_EC_NG_PARAM : IMPDRV_EC_NG_NOTSUPPORT),
							*tret, "(No.%d) Invalid core num for %s", testno++, type_to_name(core_info.core_type));
	}

	/* Invalid_error : Core_num = IMPDRV_CORE_NUM_MAX */
	core_info.core_type = IMPDRV_CORE_TYPE_IMP;
	core_info.core_num = IMPDRV_CORE_NUM_MAX;
	ercd = R_IMPDRV_Execute(handle, &core_info, callback_func, (void *)&callback_args);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) Core_num = IMPDRV_CORE_NUM_MAX", testno++);
}

static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_IMP, 0 };
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;
	tp_core_list_t no_exec_cores = get_core_list(IT_IMPDRV_CMD_NO_EXEC, 0);

	/* Not supported core: Core not supported */
	for (uint32_t i = 0; i < no_exec_cores.num; i++)
	{
		core_info = *(no_exec_cores.core_info + i);
		ercd = R_IMPDRV_Execute(handle, &core_info, callback_func, (void *)&callback_args);
		FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) Core not supported: %s", testno++, type_to_name(core_info.core_type));
	}
}

static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
	st_impdrv_core_info_t *p_core_info = p_initdata->core_info;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;
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
		ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
		FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) %s", testno++, state_to_name(invalid_state[i]));
	}

	/* Get backup data */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
}
#endif

static void Core_exec(struct test_params_t *tparam, struct test_result_t *tret)
{
	uint32_t i, j;
	e_impdrv_errorcode_t ercd;
    st_impdrv_initdata_t *p_initdata = &tparam->initdata;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;
	e_impdrv_pm_policy_t real_policy;
	e_impdrv_pm_policy_t target_policy[] = {
		IMPDRV_PM_POLICY_PG,
		IMPDRV_PM_POLICY_HP,
		IMPDRV_PM_POLICY_CG
	};

    /* Initialize for DSP core */
	tparam->dsp_app.addr_phys = 0x40000000U;
	tparam->dsp_app.size = 0x2000U;
	const st_impdrv_dsp_data_t *const p_dspdata = (const st_impdrv_dsp_data_t *const)&tparam->dsp_data;
	const st_impdrv_dsp_data_t *const p_dspfw = (const st_impdrv_dsp_data_t *const)&tparam->dsp_fw;
	const st_impdrv_dsp_data_t *const p_dspapp = (const st_impdrv_dsp_data_t *const)&tparam->dsp_app;
	const st_impdrv_dsp_data_t *const p_dspdtcm = (const st_impdrv_dsp_data_t *const)&tparam->dsp_dtcm;

	for (i = 0; i < IMPDRV_CORE_TYPE_MAX; i++)
	{
		for (j = 0; j < IMPDRV_CORE_NUM_MAX; j++)
		{
			hook_cbfunc_exec_ret[i][j] = HOOK_TEST_RET_OK;
		}
	}
	
	/* driver_set_pm_policy : output parameter */
	for (int8_t policy_idx = 0; policy_idx < ARRAY_SIZE(target_policy); policy_idx++)
	{
		/* driver_execute : output parameter */
		for (i = 0; i < p_initdata->use_core_num; i++)
		{
			st_impdrv_core_info_t *p_core_info = &p_initdata->core_info[i];
			e_impdrv_core_type_t core_type = p_core_info->core_type;
			uint32_t core_num = p_core_info->core_num;
			st_impdrv_coretypectl_t *coretype_ctl = &p_impdrv_ctl->coretype_ctl[core_type];
			int32_t result = -1;

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
			if ((IMPDRV_CORE_TYPE_OCV == p_core_info->core_type) && (0 == p_core_info->core_num))
			{
				testno += 2;
			}
			if ((IMPDRV_CORE_TYPE_DMAC == p_core_info->core_type) && (0 == p_core_info->core_num))
			{
				testno += 1;
			}
			if ((IMPDRV_CORE_TYPE_CNN == p_core_info->core_type) && (0 == p_core_info->core_num))
			{
				testno += 1;
			}
#endif

            if (p_core_info->core_type != IMPDRV_CORE_TYPE_DSP)
            {
                if (set_commandlist(p_core_info->core_type, tparam->long_claddr, CALLBACK_CODE))
                {
                    core_testmsg(p_core_info, "set_commandlist error\n");
                }
                else
                {
                    ercd = R_IMPDRV_SetPMPolicy(handle, p_core_info, target_policy[policy_idx]);
                    if (IMPDRV_EC_OK != ercd)
                    {
                        core_testmsg(p_core_info, "Set PM policy %s error\n", policy_to_name(target_policy[policy_idx]));
                    }
                    else
                    {
                        ercd = R_IMPDRV_GetPMPolicy(handle, p_core_info, &real_policy);
                        if (IMPDRV_EC_OK != ercd)
                        {
                            core_testmsg(p_core_info, "Get PM policy error\n");
                        }
                        else
                        {
                            if (target_policy[policy_idx] != real_policy)
                            {
                                core_testmsg(p_core_info, "Set PM policy error: expected %s, real %s\n", policy_to_name(target_policy[policy_idx]), policy_to_name(real_policy));
                            }
                        }
                    }
                    ercd = R_IMPDRV_AttrSetCl(handle, p_core_info, util_mem_get_paddr(tparam->long_claddr));
                    if (IMPDRV_EC_OK != ercd)
                    {
                        core_testmsg(p_core_info, "Set CL data error\n");
                    }
                }
            }
            else
            {
                /* Skip because cannot trigger GPOUT from IMPDRV IT test */
                // ercd = R_IMPDRV_AttrSetDsp(handle, p_core_info, p_dspapp, p_dspfw, p_dspdata, p_dspdtcm);
                // if (IMPDRV_EC_OK != ercd)
                // {
                //     core_testmsg(p_core_info, "Set DSP data error\n");
                // }
                continue;
            }



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
                else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_BEFORE_EXEC, -1))
                {
                    core_testmsg(p_core_info, "callback(before exec) fail\n");
                }
                else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_OK, CALLBACK_CODE))
                {
                    core_testmsg(p_core_info, "callback(trap) fail\n");
                }
                else
                {
                    result = 0;
                }
            }

            util_mem_invalidate(tparam->long_claddr);

			SuccessTestPrint(result == 0, *tret, "(No.%d) (PM policy %s) %s%d ", testno++, policy_to_name(target_policy[policy_idx]), type_to_name(core_type), core_num);

			/* for PCL_RCVIMPDRV_CBFUNC : Core_Execute */
			if ((HOOK_TEST_RET_OK == hook_cbfunc_exec_ret[core_type][core_num]) && (0 != result))
			{
				hook_cbfunc_exec_ret[core_type][core_num] = HOOK_TEST_RET_NG;
			}
		}
	}
}
