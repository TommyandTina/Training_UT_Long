
#define TEST_MSG_NAME "R_IMPDRV_Execute"

#include "it_common.h"
#include "it_stub.h"

#define CALLBACK_CODE 0xAA

#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret);
static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret);
static void Return_error(struct test_params_t *tparam, struct test_result_t *tret);
#endif
static void Device_execute(struct test_params_t *tparam, struct test_result_t *tret);

static uint32_t testno;
static int32_t check_callback_code;
static e_impdrv_cb_ret_t check_callback_ercd;

static int32_t CbFunc(
	const st_impdrv_core_info_t *p_core_info,			/**< Core information					*/
	const e_impdrv_cb_ret_t		ercd,				/**< Error code							*/
	const int32_t				code,				/**< TRAP or INT code (8 bit is set)	*/
	const void					*p_callback_args
)
{
	check_callback_ercd = ercd;
	check_callback_code = code;

	return 0;
}

int32_t it_Execute_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

#ifdef IT_ABNORMAL
	testno = 1;
	Parameter_error(tparam, &tret);
	Invalid_core(tparam, &tret);
	No_exec_core(tparam, &tret);
	Return_error(tparam, &tret);
#endif
	Device_execute(tparam, &tret);

	PrintTestResult(tret, test_result);

	return 0;
}

#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_core_info_t *p_core_info = tparam->initdata.core_info;
    impdrv_ctrl_handle_t handle;
    p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;

	/* Parameter_error : handle == NULL */
	ercd = R_IMPDRV_Execute(NULL, p_core_info, callback_func, (void *)&callback_args);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) handle == NULL", testno++);

	/* Parameter_error : p_core_info == NULL */
	ercd = R_IMPDRV_Execute(handle, NULL, callback_func, (void *)&callback_args);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) p_core_info == NULL", testno++);

	/* Parameter_error : callback_func == NULL */
	ercd = R_IMPDRV_Execute(handle, p_core_info, NULL, (void *)&callback_args);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) callback_func == NULL", testno++);
}

static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_IMP, 0 };
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;

	/* Invalid_error : Core_type = IMPDRV_CORE_TYPE_INVALID */
	core_info.core_type = IMPDRV_CORE_TYPE_INVALID;
	ercd = R_IMPDRV_Execute(handle, &core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) Core_type = IMPDRV_CORE_TYPE_INVALID", testno++);

	/* Invalid_error : Core_num = IMPDRV_CORE_NUM_MAX */
	core_info.core_type = IMPDRV_CORE_TYPE_IMP;
	core_info.core_num = IMPDRV_CORE_NUM_MAX;
	ercd = R_IMPDRV_Execute(handle, &core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
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

static void Return_error(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	const st_impdrv_initdata_t *const p_initdata = (const st_impdrv_initdata_t *const)&tparam->initdata;
	st_impdrv_core_info_t *p_core_info = tparam->initdata.core_info;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;

	/* Fail to lock mutex impdrv_cmnctl_mutex_lock returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_cmnctl_mutex_lock(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_cmnctl_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to lock mutex impdrv_cmnctl_mutex_lock returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_cmnctl_mutex_lock(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to lock mutex impdrv_cmnctl_mutex_lock returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_cmnctl_mutex_lock(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to get core function impdrv_cmnctl_get_corectl_func returns NULL */
	it_stub_set_impdrv_cmnctl_get_corectl_func(NULL);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) impdrv_cmnctl_get_corectl_func returns NULL", testno++);

	/* Fail to set attribute for IMP0 impdrv_impctl_execute returns IMPDRV_EC_NG_SEQSTATE */
	p_core_info = get_core_info(p_initdata->core_info, p_initdata->use_core_num, IMPDRV_CORE_TYPE_IMP, 0);
	it_stub_set_impdrv_impctl_execute(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_impctl_execute returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to set attribute for OCV0 impdrv_ocvctl_execute returns IMPDRV_EC_NG_ARGNULL */
	p_core_info = get_core_info(p_initdata->core_info, p_initdata->use_core_num, IMPDRV_CORE_TYPE_OCV, 0);
	it_stub_set_impdrv_ocvctl_execute(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_ocvctl_execute returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to set attribute for DMAC00 impdrv_dmactl_execute returns IMPDRV_EC_NG_PARAM */
	p_core_info = get_core_info(p_initdata->core_info, p_initdata->use_core_num, IMPDRV_CORE_TYPE_DMAC, 0);
	it_stub_set_impdrv_dmactl_execute(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_dmactl_execute returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to set attribute for PSCEXE0 impdrv_pscctl_execute returns IMPDRV_EC_NG_ATTRIBUTE */
	p_core_info = get_core_info(p_initdata->core_info, p_initdata->use_core_num, IMPDRV_CORE_TYPE_PSCEXE, 0);
	it_stub_set_impdrv_pscctl_execute(IMPDRV_EC_NG_ATTRIBUTE);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_ATTRIBUTE, *tret, "(No.%d) impdrv_pscctl_execute returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ATTRIBUTE));

	/* Fail to set attribute for IMP0 impdrv_impctl_execute returns IMPDRV_EC_NG_NOTSUPPORT */
	p_core_info = get_core_info(p_initdata->core_info, p_initdata->use_core_num, IMPDRV_CORE_TYPE_IMP, 0);
	it_stub_set_impdrv_impctl_execute(IMPDRV_EC_NG_NOTSUPPORT);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) impdrv_impctl_execute returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_NOTSUPPORT));

	/* Fail to set attribute for CNN impdrv_cnnctl_execute returns IMPDRV_EC_NG_SYSTEMERROR */
	p_core_info = get_core_info(p_initdata->core_info, p_initdata->use_core_num, IMPDRV_CORE_TYPE_CNN, 0);
	it_stub_set_impdrv_cnnctl_execute(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cnnctl_execute returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to unlock mutex impdrv_cmnctl_mutex_unlock returns IMPDRV_EC_NG_SEQSTATE */
	p_core_info = p_initdata->core_info;
	it_stub_set_impdrv_cmnctl_mutex_unlock(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_cmnctl_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to unlock mutex impdrv_cmnctl_mutex_unlock returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_cmnctl_mutex_unlock(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to unlock mutex impdrv_cmnctl_mutex_unlock returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_cmnctl_mutex_unlock(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));
}
#endif

static void Device_execute(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;
	int32_t result;

	/* Initialize for DSP core */
	tparam->dsp_app.addr_phys = 0x40000000U;
	const st_impdrv_dsp_data_t *const p_dspdata = (const st_impdrv_dsp_data_t *const)&tparam->dsp_data;
	const st_impdrv_dsp_data_t *const p_dspfw = (const st_impdrv_dsp_data_t *const)&tparam->dsp_fw;
	const st_impdrv_dsp_data_t *const p_dspapp = (const st_impdrv_dsp_data_t *const)&tparam->dsp_app;
	const st_impdrv_dsp_data_t *const p_dspdtcm = (const st_impdrv_dsp_data_t *const)&tparam->dsp_dtcm;
	
	/* driver_execute : output parameter */
	for (uint32_t i = 0; i < tparam->initdata.use_core_num; i++)
	{
		st_impdrv_core_info_t *p_core_info = &tparam->initdata.core_info[i];
		e_impdrv_core_type_t core_type = p_core_info->core_type;
		uint32_t core_num = p_core_info->core_num;
		st_impdrv_coretypectl_t *coretype_ctl = &p_impdrv_ctl->coretype_ctl[core_type];

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

		result = -1;

		it_stub_reset();
		if (IMPDRV_CORE_TYPE_DSP != p_core_info->core_type)
		{
			if (set_commandlist(p_core_info->core_type, tparam->long_claddr, CALLBACK_CODE))
			{
				core_testmsg(p_core_info, "set_commandlist error\n");
			}
			if (IMPDRV_EC_OK != R_IMPDRV_AttrSetCl(handle, p_core_info, util_mem_get_paddr(tparam->long_claddr)))
			{
				core_testmsg(p_core_info, "Set CL data error\n");
			}
		}
		else 
		{
			ercd = R_IMPDRV_AttrSetDsp(handle, p_core_info, p_dspapp, p_dspfw, p_dspdata, p_dspdtcm);
			if (IMPDRV_EC_OK != ercd)
			{
				core_testmsg(p_core_info, "Set DSP data error\n");
			}
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
			else if ((check_callback_ercd != IMPDRV_CB_RET_BEFORE_EXEC) || (check_callback_code != -1))
			{
				core_testmsg(p_core_info, "callback(trap) fail\n");
			}
			else
			{
				result = 0;
				coretype_ctl->core_ctl[core_num].core_state = IMPDRV_STATE_READY;
			}
		}

			util_mem_invalidate(tparam->long_claddr);
		

		SuccessTestPrint(result == 0, *tret, "(No.%d) %s%d ", testno++, type_to_name(core_type), core_num);
	}
}
