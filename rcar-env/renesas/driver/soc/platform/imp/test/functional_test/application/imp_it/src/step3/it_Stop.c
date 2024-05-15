#define TEST_MSG_NAME "R_IMPDRV_Stop"

#include "it_common.h"
#include "it_stub.h"

#define IT_ABNORMAL

static void set_state(st_impdrv_initdata_t *const p_data, e_impdrv_state_t state);
#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret);
static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret);
static void Return_error(struct test_params_t *tparam, struct test_result_t *tret);
#endif
static void Device_stop(struct test_params_t *tparam, struct test_result_t *tret);

static uint32_t testno;
static st_impdrv_coretypectl_t coretype_ctl_backup[IMPDRV_CORE_TYPE_MAX];
static void prepare_test_for_stop( st_impdrv_core_info_t *p_core_info);

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

static void prepare_test_for_stop( st_impdrv_core_info_t *p_core_info)
{
	uint32_t read_cnt = 0;

	if (IMPDRV_CORE_TYPE_CNN == p_core_info->core_type)
	{
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, read_cnt++, IMPDRV_CNN_CNF_CLR);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, read_cnt++, IMPDRV_CNN_CNF_CLR);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, read_cnt++, 0);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, read_cnt++, 1);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, read_cnt++, 0);
	}
	else if (IMPDRV_CORE_TYPE_DSP == p_core_info->core_type)
	{
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, read_cnt++, IMPDRV_REGVAL_VDSP_GPIO_STOP);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, read_cnt++, IMPDRV_REGVAL_VDSP_GPIO_STDBY);
		/* impdrv_dspctl_dsp_standby */
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, read_cnt++, IMPDRV_REGVAL_PSU_STATE_LSP);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, read_cnt++, IMPDRV_REGVAL_DMBE_IDLE);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, read_cnt++, IMPDRV_REGVAL_PSU_STATE_LSP | IMPDRV_REGVAL_PSU_STATE_SBY);
	}
	else
	{
		/* do nothing */
	}

	it_stub_set_R_OSAL_PmGetRequiredState(IMPDRV_EC_OK, OSAL_PM_REQUIRED_STATE_REQUIRED);
}

int32_t it_Stop_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};
	stop_test_flag = true;

#ifdef IT_ABNORMAL
	testno = 100;
	Parameter_error(tparam, &tret);
	Invalid_core(tparam, &tret);
	No_exec_core(tparam, &tret);
	Invalid_state(tparam, &tret);
	Return_error(tparam, &tret);
#endif
	Device_stop(tparam, &tret);
	stop_test_flag = false;

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

	/* Parameter_error : handle == NULL */
	ercd = R_IMPDRV_Stop(NULL, p_core_info);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) handle == NULL", testno++);

	/* Parameter_error : p_core_info == NULL */
	ercd = R_IMPDRV_Stop(handle, NULL);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) p_core_info == NULL", testno++);
}

static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_IMP, 0 };
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
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
		ercd = R_IMPDRV_Stop(handle, &core_info);
		FailTestPrint(ercd, ((core_info.core_type == IMPDRV_CORE_TYPE_INVALID) ?
							IMPDRV_EC_NG_PARAM : IMPDRV_EC_NG_NOTSUPPORT),
							*tret, "(No.%d) Invalid core num for %s", testno++, type_to_name(core_info.core_type));
	}

	/* Invalid_error : Core_num = IMPDRV_CORE_NUM_MAX */
	core_info.core_type = IMPDRV_CORE_TYPE_IMP;
	core_info.core_num = IMPDRV_CORE_NUM_MAX;
	ercd = R_IMPDRV_Stop(handle, &core_info);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) Core_num = IMPDRV_CORE_NUM_MAX", testno++);
}

static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_IMP, 0 };
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	tp_core_list_t no_exec_cores = get_core_list(IT_IMPDRV_CMD_NO_EXEC, 0);

	/* Not supported core: Core not supported */
	for (uint32_t i = 0; i < no_exec_cores.num; i++)
	{
		core_info = *(no_exec_cores.core_info + i);
		ercd = R_IMPDRV_Stop(handle, &core_info);
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
		ercd = R_IMPDRV_Stop(handle, p_core_info);
		FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) %s", testno++, state_to_name(invalid_state[i]));
	}

	/* Get backup data */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
}

static void Return_error(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
	st_impdrv_core_info_t *p_core_info = p_initdata->core_info;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_TIME */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_TIME);
	ercd = R_IMPDRV_Stop(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_TIME));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_PAR */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_PAR);
	ercd = R_IMPDRV_Stop(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_BUSY */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_BUSY);
	ercd = R_IMPDRV_Stop(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_BUSY));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_HANDLE */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_Stop(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_STATE */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_Stop(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_FAIL */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_Stop(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Fail to set released state for IMP: R_OSAL_PmSetRequiredState returns OSAL_RETURN_PAR */
	prepare_test_for_stop(p_core_info);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_PAR, "imp_00", OSAL_PM_REQUIRED_STATE_RELEASED);
	ercd = R_IMPDRV_Stop(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to set released state for IMP: R_OSAL_PmSetRequiredState returns OSAL_RETURN_DEV */
	prepare_test_for_stop(p_core_info);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_DEV, "imp_00", OSAL_PM_REQUIRED_STATE_RELEASED);
	ercd = R_IMPDRV_Stop(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to set released state for IMP: R_OSAL_PmSetRequiredState returns OSAL_RETURN_TIME */
	prepare_test_for_stop(p_core_info);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_TIME, "imp_00", OSAL_PM_REQUIRED_STATE_RELEASED);
	ercd = R_IMPDRV_Stop(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_TIME));

	/* Fail to set released state for IMP: R_OSAL_PmSetRequiredState returns OSAL_RETURN_HANDLE */
	prepare_test_for_stop(p_core_info);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_HANDLE, "imp_00", OSAL_PM_REQUIRED_STATE_RELEASED);
	ercd = R_IMPDRV_Stop(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to set released state for IMP: R_OSAL_PmSetRequiredState returns OSAL_RETURN_STATE */
	prepare_test_for_stop(p_core_info);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_STATE, "imp_00", OSAL_PM_REQUIRED_STATE_RELEASED);
	ercd = R_IMPDRV_Stop(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to set released state for IMP: R_OSAL_PmSetRequiredState returns OSAL_RETURN_UNSUPPORTED_OPERATION */
	prepare_test_for_stop(p_core_info);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_UNSUPPORTED_OPERATION, "imp_00", OSAL_PM_REQUIRED_STATE_RELEASED);
	ercd = R_IMPDRV_Stop(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_UNSUPPORTED_OPERATION));

	/* Fail to set released state for IMP: R_OSAL_PmSetRequiredState returns OSAL_RETURN_FAIL */
	prepare_test_for_stop(p_core_info);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_FAIL, "imp_00", OSAL_PM_REQUIRED_STATE_RELEASED);
	ercd = R_IMPDRV_Stop(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Fail to close device for IMP: R_OSAL_IoDeviceClose returns OSAL_RETURN_DEV */
	it_stub_set_R_OSAL_IoDeviceClose(OSAL_RETURN_DEV, "imp_00");
	ercd = R_IMPDRV_Stop(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceClose for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to close device for IMP: R_OSAL_IoDeviceClose returns OSAL_RETURN_BUSY */
	it_stub_set_R_OSAL_IoDeviceClose(OSAL_RETURN_BUSY, "imp_00");
	ercd = R_IMPDRV_Stop(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceClose for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_BUSY));

	/* Fail to close device for IMP: R_OSAL_IoDeviceClose returns OSAL_RETURN_HANDLE */
	it_stub_set_R_OSAL_IoDeviceClose(OSAL_RETURN_HANDLE, "imp_00");
	ercd = R_IMPDRV_Stop(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceClose for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to close device for IMP: R_OSAL_IoDeviceClose returns OSAL_RETURN_STATE */
	it_stub_set_R_OSAL_IoDeviceClose(OSAL_RETURN_STATE, "imp_00");
	ercd = R_IMPDRV_Stop(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceClose for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to close device for IMP: R_OSAL_IoDeviceClose returns OSAL_RETURN_FAIL */
	it_stub_set_R_OSAL_IoDeviceClose(OSAL_RETURN_FAIL, "imp_00");
	ercd = R_IMPDRV_Stop(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceClose for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_HANDLE */
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_Stop(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_STATE */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_Stop(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_FAIL */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_Stop(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Get backup data */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
}
#endif

static void Device_stop(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
    st_impdrv_initdata_t *p_initdata = &tparam->initdata;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	
	/* driver_stop : output parameter */
	for (uint32_t i = 0; i < p_initdata->use_core_num; i++)
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

		result = 0;
		stop_test_flag = true;
		it_stub_reset();
		prepare_test_for_stop(p_core_info);
		ercd = R_IMPDRV_Stop(handle, p_core_info);
		if (ercd != IMPDRV_EC_OK) {
			core_testmsg(p_core_info, "%s=%d\n", TEST_MSG_NAME, ercd);
			result = -1;
		} else {
			/* Check state of the core */
			if (coretype_ctl->core_ctl[core_num].core_state != IMPDRV_STATE_INIT) {
				core_testmsg(p_core_info, "state=%d, it is not INIT(test NG)\n", state_to_name(coretype_ctl->core_ctl[core_num].core_state));
				result = -1;
			} else {
				result = 0;
			}
		}
		stop_test_flag = false;

		SuccessTestPrint(result == 0, *tret, "(No.%d) %s%d ", testno++, type_to_name(core_type), core_num);
	}
}