
#define TEST_MSG_NAME "R_IMPDRV_ResumeExecution"

#include "it_common.h"
#include "it_stub.h"

#define CALLBACK_CODE_1		(0xAA)
#define CALLBACK_CODE_2		(0x55)

static void set_state(st_impdrv_initdata_t *const p_data, e_impdrv_state_t state);
#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret);
static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret);
static void Return_error(struct test_params_t *tparam, struct test_result_t *tret);
#endif
static void Device_resume_execution(struct test_params_t *tparam, struct test_result_t *tret);

static int32_t check_callback_code;
static e_impdrv_cb_ret_t check_callback_ercd;
static uint32_t testno;
static st_impdrv_coretypectl_t coretype_ctl_backup[IMPDRV_CORE_TYPE_MAX];
static e_impdrv_state_t state_bk;

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
	check_callback_ercd = ercd;
	check_callback_code = code;

	return 0;
}

int32_t it_ResumeExecution_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

#ifdef IT_ABNORMAL
	testno = 44;
	Parameter_error(tparam, &tret);
	Invalid_core(tparam, &tret);
	No_exec_core(tparam, &tret);
	Invalid_state(tparam, &tret);
	Return_error(tparam, &tret);
#endif
	Device_resume_execution(tparam, &tret);

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

	/* Back up state */
	state_bk = p_impdrv_ctl->coretype_ctl[p_core_info->core_type].core_ctl[p_core_info->core_num].core_state;

	/* Parameter_error : handle == NULL */
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	ercd = R_IMPDRV_ResumeExecution(NULL, p_core_info);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) handle == NULL", testno++);

	/* Parameter_error : p_core_info == NULL */
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	ercd = R_IMPDRV_ResumeExecution(handle, NULL);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) p_core_info == NULL", testno++);

	/* Revert state */
	set_state(&tparam->initdata, state_bk);
}

static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_IMP, 0 };
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	tp_core_list_t invalid_core_list = get_core_list(IT_IMPDRV_CMD_INVALID_CORES, 0);

	/* Back up state */
	state_bk = p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].core_state;

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

		/* Adjust state to be valid */
		set_state(&tparam->initdata, IMPDRV_STATE_INT);
		core_info = *(invalid_core_list.core_info + i);
		ercd = R_IMPDRV_ResumeExecution(handle, &core_info);
		FailTestPrint(ercd, ((core_info.core_type == IMPDRV_CORE_TYPE_INVALID) ?
							IMPDRV_EC_NG_PARAM : IMPDRV_EC_NG_NOTSUPPORT),
							*tret, "(No.%d) Invalid core num for %s", testno++, type_to_name(core_info.core_type));
	}

	/* Invalid_error : Core_num = IMPDRV_CORE_NUM_MAX */
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	core_info.core_type = IMPDRV_CORE_TYPE_IMP;
	core_info.core_num = IMPDRV_CORE_NUM_MAX;
	ercd = R_IMPDRV_ResumeExecution(handle, &core_info);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) Core_num = IMPDRV_CORE_NUM_MAX", testno++);

	/* Revert state */
	set_state(&tparam->initdata, state_bk);
}

static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_IMP, 0 };
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	tp_core_list_t no_exec_cores = get_core_list(IT_IMPDRV_CMD_NO_EXEC, 0);

	/* Back up state */
	state_bk = p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].core_state;

	/* Not supported core: Core not supported */
	for (uint32_t i = 0; i < no_exec_cores.num; i++)
	{
		set_state(&tparam->initdata, IMPDRV_STATE_INT);
		core_info = *(no_exec_cores.core_info + i);
		ercd = R_IMPDRV_ResumeExecution(handle, &core_info);
		FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) Core not supported: %s", testno++, type_to_name(core_info.core_type));
	}

	/* Revert state */
	set_state(&tparam->initdata, state_bk);
}

static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
	st_impdrv_core_info_t *p_core_info = p_initdata->core_info;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	e_impdrv_state_t invalid_state[] = {
		IMPDRV_STATE_UNINIT,
		IMPDRV_STATE_INIT,
		IMPDRV_STATE_READY,
		IMPDRV_STATE_EXEC
	};

	/* Back up data */
	memcpy(coretype_ctl_backup, p_impdrv_ctl->coretype_ctl, sizeof(p_impdrv_ctl->coretype_ctl));

	for (uint32_t i = 0; i < ARRAY_SIZE(invalid_state); i++)
	{
		set_state(p_initdata, invalid_state[i]);
		ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
		FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) %s", testno++, state_to_name(invalid_state[i]));
	}

	/* Get backup data */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
}

static void Return_error(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
	st_impdrv_core_info_t *p_core_info = p_initdata->core_info;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;

	/* Back up state */
	state_bk = p_impdrv_ctl->coretype_ctl[p_core_info->core_type].core_ctl[p_core_info->core_num].core_state;

	/* Fail to lock mutex impdrv_osdep_mutex_lock returns IMPDRV_EC_NG_SEQSTATE */
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_impdrv_osdep_mutex_lock(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to lock mutex impdrv_osdep_mutex_lock returns IMPDRV_EC_NG_ARGNULL */
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_impdrv_osdep_mutex_lock(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to lock mutex impdrv_osdep_mutex_lock returns IMPDRV_EC_NG_PARAM */
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_impdrv_osdep_mutex_lock(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to lock mutex impdrv_osdep_mutex_lock returns IMPDRV_EC_NG_SYSTEMERROR */
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_impdrv_osdep_mutex_lock(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to memory barrier impdrv_osdep_memory_barrier returns IMPDRV_EC_NG_SYSTEMERROR */
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_impdrv_osdep_memory_barrier(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_memory_barrier returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to write register impdrv_osdep_write_reg for Interrupt Status register returns IMPDRV_EC_NG_SEQSTATE */
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_SEQSTATE, IMPDRV_REG_IMP_INTSTS);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_write_reg for Interrupt Status register returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to write register impdrv_osdep_write_reg for Interrupt Status register returns IMPDRV_EC_NG_ARGNULL */
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_ARGNULL, IMPDRV_REG_IMP_INTSTS);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_write_reg for Interrupt Status register returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to write register impdrv_osdep_write_reg for Interrupt Status register returns IMPDRV_EC_NG_PARAM */
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_PARAM, IMPDRV_REG_IMP_INTSTS);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_write_reg for Interrupt Status register returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));
	
	/* Fail to write register impdrv_osdep_write_reg for Interrupt Status register returns IMPDRV_EC_NG_SYSTEMERROR */
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_SYSTEMERROR, IMPDRV_REG_IMP_INTSTS);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_write_reg for Interrupt Status register returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to write register impdrv_osdep_write_reg for Interrupt Enalbe register returns IMPDRV_EC_NG_SEQSTATE */
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_SEQSTATE, IMPDRV_REG_IMP_INTEN);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_write_reg for Interrupt Enalbe register returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to write register impdrv_osdep_write_reg for Interrupt Enalbe register returns IMPDRV_EC_NG_ARGNULL */
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_ARGNULL, IMPDRV_REG_IMP_INTEN);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_write_reg for Interrupt Enalbe register returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to write register impdrv_osdep_write_reg for Interrupt Enalbe register returns IMPDRV_EC_NG_PARAM */
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_PARAM, IMPDRV_REG_IMP_INTEN);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_write_reg for Interrupt Enalbe register returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));
	
	/* Fail to write register impdrv_osdep_write_reg for Interrupt Enalbe register returns IMPDRV_EC_NG_SYSTEMERROR */
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_SYSTEMERROR, IMPDRV_REG_IMP_INTEN);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_write_reg for Interrupt Enalbe register returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to unlock mutex impdrv_osdep_mutex_unlock returns IMPDRV_EC_NG_SEQSTATE */
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_impdrv_osdep_mutex_unlock(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to unlock mutex impdrv_osdep_mutex_unlock returns IMPDRV_EC_NG_ARGNULL */
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_impdrv_osdep_mutex_unlock(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to unlock mutex impdrv_osdep_mutex_unlock returns IMPDRV_EC_NG_SYSTEMERROR */
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_impdrv_osdep_mutex_unlock(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Revert state */
	set_state(&tparam->initdata, state_bk);
}
#endif

static void Device_resume_execution(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
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

	/* driver_resume_execution : output parameter */
	for (uint32_t i = 0; i < tparam->initdata.use_core_num; i++)
	{
		st_impdrv_core_info_t *p_core_info = &tparam->initdata.core_info[i];
		e_impdrv_core_type_t core_type = p_core_info->core_type;
		uint32_t core_num = p_core_info->core_num;
		st_impdrv_coretypectl_t *coretype_ctl = &p_impdrv_ctl->coretype_ctl[core_type];

#if defined (V3H1)
		if ((IMPDRV_CORE_TYPE_PSCEXE == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno += 4;
		}
#elif defined (V3H2)
		if ((IMPDRV_CORE_TYPE_CNN == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno += 1;
		}
		if ((IMPDRV_CORE_TYPE_PSCEXE == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno += 2;
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
			testno += 6;
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

		it_stub_reset();
		result = -1;

		if (IMPDRV_CORE_TYPE_DSP != p_core_info->core_type)
		{
			if (set_intcommandlist(p_core_info->core_type, tparam->long_claddr, CALLBACK_CODE_1, CALLBACK_CODE_2))
			{
				core_testmsg(p_core_info, "set_intcommandlist error\n");
				goto error1;
			}

			if (IMPDRV_EC_OK != R_IMPDRV_AttrSetCl(handle, p_core_info, util_mem_get_paddr(tparam->long_claddr)))
			{
				core_testmsg(p_core_info, "Set CL data error\n");
				goto error1;
			}
		}
		else {
			ercd = R_IMPDRV_AttrSetDsp(handle, p_core_info, p_dspapp, p_dspfw, p_dspdata, p_dspdtcm);
			if (IMPDRV_EC_OK != ercd)
			{
				core_testmsg(p_core_info, "Set DSP data error\n");
				goto error1;
			}
			/* prepare for IMPDRV_Execute */
			it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_OK, 0, IMPDRV_REGVAL_VDSP_GPIO_START);
			it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_OK, 1, IMPDRV_REGVAL_VDSP_GPIO_RESUME);
		}

		ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
		if (ercd != IMPDRV_EC_OK)
		{
			core_testmsg(p_core_info, "%s=%d\n", "R_IMPDRV_Execute", ercd);
			goto error2;
		}

		if ((check_callback_ercd != IMPDRV_CB_RET_BEFORE_EXEC) || (check_callback_code != -1))
		{
			core_testmsg(p_core_info, "callback(returns IMPDRV_CB_RET_BEFORE_EXEC) fail\n");
			goto error2;
		}

		coretype_ctl->core_ctl[core_num].core_state = IMPDRV_STATE_INT;

		ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
		if (ercd != IMPDRV_EC_OK)
		{
			core_testmsg(p_core_info, "%s=%d\n", TEST_MSG_NAME, ercd);
			goto error2;
		}

		coretype_ctl->core_ctl[core_num].core_state = IMPDRV_STATE_READY;

		result = 0;
error2:
		util_mem_invalidate(tparam->long_claddr);

error1:
		SuccessTestPrint(result == 0, *tret, "(No.%d) %s%d ", testno++, type_to_name(core_type), core_num);
	}
}