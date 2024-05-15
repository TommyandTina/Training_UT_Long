#define TEST_MSG_NAME "R_IMPDRV_GetPMPolicy"

#include "it_common.h"
#include "it_stub.h"

/* pragma qas cantata testscript start */

static void set_state(st_impdrv_initdata_t *const p_data, e_impdrv_state_t state);
#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret);
static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret);
static void Return_error(struct test_params_t *tparam, struct test_result_t *tret);
#endif
static void Valid_state(struct test_params_t *tparam, struct test_result_t *tret);
static void Device_get_pm_policy(struct test_params_t *tparam, struct test_result_t *tret);

static uint32_t testno;
static st_impdrv_coretypectl_t coretype_ctl_backup[IMPDRV_CORE_TYPE_MAX];

static void set_state(st_impdrv_initdata_t *const p_data, e_impdrv_state_t state)
{
	uint32_t cnt;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_data->p_work_addr;

	for (cnt = 0U; cnt < p_data->use_core_num; cnt++)
	{
		st_impdrv_core_info_t *core_info = &p_data->core_info[cnt];
		e_impdrv_core_type_t core_type = core_info->core_type;
		uint32_t core_num = core_info->core_num;

		p_impdrv_ctl->coretype_ctl[core_type].core_ctl[core_num].core_state = state;
	}
}

int32_t it_GetPMPolicy_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

#ifdef IT_ABNORMAL
	testno = 103;
	Parameter_error(tparam, &tret);
	Invalid_core(tparam, &tret);
	No_exec_core(tparam, &tret);
	Invalid_state(tparam, &tret);
	Return_error(tparam, &tret);
#endif
	Valid_state(tparam, &tret);
	Device_get_pm_policy(tparam, &tret);

	PrintTestResult(tret, test_result);

	return 0;
}

#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	const st_impdrv_core_info_t *p_core_info = tparam->initdata.core_info;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	e_impdrv_pm_policy_t policy;

	/* Parameter_error : handle == NULL */
	ercd = R_IMPDRV_GetPMPolicy(NULL, p_core_info, &policy);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) handle == NULL", testno++);

	/* Parameter_error : p_core_info == NULL */
	ercd = R_IMPDRV_GetPMPolicy(handle, NULL, &policy);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) p_core_info == NULL", testno++);

	/* Parameter_error : p_policy == NULL */
	ercd = R_IMPDRV_GetPMPolicy(handle, p_core_info, NULL);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) p_policy == NULL", testno++);
}

static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_IMP, 0 };
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	e_impdrv_pm_policy_t policy;
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
		ercd = R_IMPDRV_GetPMPolicy(handle, &core_info, &policy);
		FailTestPrint(ercd, ((core_info.core_type == IMPDRV_CORE_TYPE_INVALID) ?
							IMPDRV_EC_NG_PARAM : IMPDRV_EC_NG_NOTSUPPORT),
							*tret, "(No.%d) Invalid core num for %s", testno++, type_to_name(core_info.core_type));
	}

	/* Invalid_error : Core_num = IMPDRV_CORE_NUM_MAX */
	core_info.core_type = IMPDRV_CORE_TYPE_IMP;
	core_info.core_num = IMPDRV_CORE_NUM_MAX;
	ercd = R_IMPDRV_GetPMPolicy(handle, &core_info, &policy);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) Core_num = IMPDRV_CORE_NUM_MAX", testno++);
}

static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_IMP, 0 };
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	e_impdrv_pm_policy_t policy;
	tp_core_list_t no_exec_cores = get_core_list(IT_IMPDRV_CMD_NO_EXEC, 0);

	/* Not supported core: Core not supported */
	for (uint32_t i = 0; i < no_exec_cores.num; i++)
	{
		core_info = *(no_exec_cores.core_info + i);
		ercd = R_IMPDRV_GetPMPolicy(handle, &core_info, &policy);
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
	e_impdrv_pm_policy_t policy;
	e_impdrv_state_t invalid_state[] = {
		IMPDRV_STATE_UNINIT
	};

	/* Back up data */
	memcpy(coretype_ctl_backup, p_impdrv_ctl->coretype_ctl, sizeof(p_impdrv_ctl->coretype_ctl));

	for (uint32_t i = 0; i < ARRAY_SIZE(invalid_state); i++)
	{
		set_state(p_initdata, invalid_state[i]);
		ercd = R_IMPDRV_GetPMPolicy(handle, p_core_info, &policy);
		FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) %s", testno++, state_to_name(invalid_state[i]));
	}

	/* Get backup data */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
}

static void Return_error(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_core_info_t *p_core_info = tparam->initdata.core_info;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	e_impdrv_pm_policy_t policy;

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_TIME */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_TIME);
	ercd = R_IMPDRV_GetPMPolicy(handle, p_core_info, &policy);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_TIME));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_PAR */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_PAR);
	ercd = R_IMPDRV_GetPMPolicy(handle, p_core_info, &policy);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_BUSY */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_BUSY);
	ercd = R_IMPDRV_GetPMPolicy(handle, p_core_info, &policy);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_BUSY));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_HANDLE */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_GetPMPolicy(handle, p_core_info, &policy);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_STATE */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_GetPMPolicy(handle, p_core_info, &policy);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_FAIL */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_GetPMPolicy(handle, p_core_info, &policy);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Fail to open device for IMP: R_OSAL_IoDeviceOpen returns OSAL_RETURN_PAR */
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_PAR, "imp_00");
	ercd = R_IMPDRV_GetPMPolicy(handle, p_core_info, &policy);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to open device for IMP: R_OSAL_IoDeviceOpen returns OSAL_RETURN_MEM */
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_MEM, "imp_00");
	ercd = R_IMPDRV_GetPMPolicy(handle, p_core_info, &policy);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_MEM));

	/* Fail to open device for IMP: R_OSAL_IoDeviceOpen returns OSAL_RETURN_DEV */
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_DEV, "imp_00");
	ercd = R_IMPDRV_GetPMPolicy(handle, p_core_info, &policy);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to open device for IMP: R_OSAL_IoDeviceOpen returns OSAL_RETURN_CONF */
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_CONF, "imp_00");
	ercd = R_IMPDRV_GetPMPolicy(handle, p_core_info, &policy);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_CONF));

	/* Fail to open device for IMP: R_OSAL_IoDeviceOpen returns OSAL_RETURN_BUSY */
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_BUSY, "imp_00");
	ercd = R_IMPDRV_GetPMPolicy(handle, p_core_info, &policy);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_BUSY));

	/* Fail to open device for IMP: R_OSAL_IoDeviceOpen returns OSAL_RETURN_ID */
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_ID, "imp_00");
	ercd = R_IMPDRV_GetPMPolicy(handle, p_core_info, &policy);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_ID));

	/* Fail to open device for IMP: R_OSAL_IoDeviceOpen returns OSAL_RETURN_STATE */
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_STATE, "imp_00");
	ercd = R_IMPDRV_GetPMPolicy(handle, p_core_info, &policy);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to open device for IMP: R_OSAL_IoDeviceOpen returns OSAL_RETURN_FAIL */
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_FAIL, "imp_00");
	ercd = R_IMPDRV_GetPMPolicy(handle, p_core_info, &policy);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Fail to get PM policy for IMP0: R_OSAL_PmGetPolicy returns OSAL_RETURN_PAR */
	it_stub_set_R_OSAL_PmGetPolicy(OSAL_RETURN_PAR);
	ercd = R_IMPDRV_GetPMPolicy(handle, p_core_info, &policy);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmGetPolicy returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to get PM policy for IMP0: R_OSAL_PmGetPolicy returns OSAL_RETURN_DEV */
	it_stub_set_R_OSAL_PmGetPolicy(OSAL_RETURN_DEV);
	ercd = R_IMPDRV_GetPMPolicy(handle, p_core_info, &policy);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmGetPolicy returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to get PM policy for IMP0: R_OSAL_PmGetPolicy returns OSAL_RETURN_HANDLE */
	it_stub_set_R_OSAL_PmGetPolicy(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_GetPMPolicy(handle, p_core_info, &policy);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmGetPolicy returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to get PM policy for IMP0: R_OSAL_PmGetPolicy returns OSAL_RETURN_STATE */
	it_stub_set_R_OSAL_PmGetPolicy(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_GetPMPolicy(handle, p_core_info, &policy);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmGetPolicy returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to get PM policy for IMP0: R_OSAL_PmGetPolicy returns OSAL_RETURN_UNSUPPORTED_OPERATION */
	it_stub_set_R_OSAL_PmGetPolicy(OSAL_RETURN_UNSUPPORTED_OPERATION);
	ercd = R_IMPDRV_GetPMPolicy(handle, p_core_info, &policy);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmGetPolicy returns %s", testno++, osal_err_to_name(OSAL_RETURN_UNSUPPORTED_OPERATION));

	/* Fail to get PM policy for IMP0: R_OSAL_PmGetPolicy returns OSAL_RETURN_FAIL */
	it_stub_set_R_OSAL_PmGetPolicy(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_GetPMPolicy(handle, p_core_info, &policy);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmGetPolicy returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Fail to close device for IMP0: R_OSAL_IoDeviceClose returns OSAL_RETURN_DEV */
	it_stub_set_R_OSAL_IoDeviceClose(OSAL_RETURN_DEV, "imp_00");
	ercd = R_IMPDRV_GetPMPolicy(handle, p_core_info, &policy);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceClose for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to close device for IMP0: R_OSAL_IoDeviceClose returns OSAL_RETURN_BUSY */
	it_stub_set_R_OSAL_IoDeviceClose(OSAL_RETURN_BUSY, "imp_00");
	ercd = R_IMPDRV_GetPMPolicy(handle, p_core_info, &policy);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceClose for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_BUSY));

	/* Fail to close device for IMP0: R_OSAL_IoDeviceClose returns OSAL_RETURN_HANDLE */
	it_stub_set_R_OSAL_IoDeviceClose(OSAL_RETURN_HANDLE, "imp_00");
	ercd = R_IMPDRV_GetPMPolicy(handle, p_core_info, &policy);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceClose for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to close device for IMP0: R_OSAL_IoDeviceClose returns OSAL_RETURN_STATE */
	it_stub_set_R_OSAL_IoDeviceClose(OSAL_RETURN_STATE, "imp_00");
	ercd = R_IMPDRV_GetPMPolicy(handle, p_core_info, &policy);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceClose for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to close device for IMP0: R_OSAL_IoDeviceClose returns OSAL_RETURN_FAIL */
	it_stub_set_R_OSAL_IoDeviceClose(OSAL_RETURN_FAIL, "imp_00");
	ercd = R_IMPDRV_GetPMPolicy(handle, p_core_info, &policy);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceClose for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_HANDLE */
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_GetPMPolicy(handle, p_core_info, &policy);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_STATE */
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_GetPMPolicy(handle, p_core_info, &policy);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_FAIL */
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_GetPMPolicy(handle, p_core_info, &policy);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));
}
#endif

static void Valid_state(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
	st_impdrv_core_info_t *p_core_info = p_initdata->core_info;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	e_impdrv_pm_policy_t policy;
	int32_t result = 0;
	e_impdrv_state_t chk_state[] = {
		IMPDRV_STATE_INIT,
		IMPDRV_STATE_READY,
		IMPDRV_STATE_EXEC,
		IMPDRV_STATE_INT,
	};

	/* Back up data */
	memcpy(coretype_ctl_backup, p_impdrv_ctl->coretype_ctl, sizeof(p_impdrv_ctl->coretype_ctl));

	for (int8_t i = 0; i < ARRAY_SIZE(chk_state); i++)
	{
		set_state(p_initdata, chk_state[i]);
		ercd = R_IMPDRV_GetPMPolicy(handle, p_core_info, &policy);
		if (ercd != IMPDRV_EC_OK)
		{
			core_testmsg(p_core_info, "(No.%d) Test core_type = %s, core_num = %d, ercd = %s fail(test NG)\n",
							testno,
							type_to_name(p_core_info->core_type),
							p_core_info->core_num,
							impdrv_err_to_name(ercd));
			result = -1;
		}
		SuccessTestPrint(result == 0, *tret, "(No.%d) State %s: %s%d ", testno++, state_to_name(chk_state[i]), type_to_name(p_core_info->core_type), p_core_info->core_num);
	}

	/* Get backup data */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
}

static void Device_get_pm_policy(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
    st_impdrv_initdata_t *p_initdata = &tparam->initdata;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	e_impdrv_pm_policy_t policy;
	
	/* driver_get_pm_policy : output parameter */
	for (uint32_t i = 0; i < p_initdata->use_core_num; i++)
	{
		st_impdrv_core_info_t *p_core_info = &p_initdata->core_info[i];
		int32_t result = 0;

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

		ercd = R_IMPDRV_GetPMPolicy(handle, p_core_info, &policy);
		if (ercd != IMPDRV_EC_OK)
		{
			core_testmsg(p_core_info, "Test core_type = %s, core_num = %d, ercd = %s fail\n",
							type_to_name(p_core_info->core_type),
							p_core_info->core_num,
							impdrv_err_to_name(ercd));
			result = -1;
		}

		SuccessTestPrint(result == 0, *tret, "(No.%d) %s%d ", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num);
	}
}
