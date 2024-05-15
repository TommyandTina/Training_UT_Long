#define TEST_MSG_NAME "R_IMPDRV_Start"

#include "it_common.h"
#include "it_stub.h"

static void set_state(st_impdrv_initdata_t *const p_data, e_impdrv_state_t state);
#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret);
static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret);
static void Return_error(struct test_params_t *tparam, struct test_result_t *tret);
#endif
static void Device_start(struct test_params_t *tparam, struct test_result_t *tret);

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

int32_t it_Start_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};
	start_test_flag = true;

#ifdef IT_ABNORMAL
	testno = 111;
	Parameter_error(tparam, &tret);
	Invalid_core(tparam, &tret);
	No_exec_core(tparam, &tret);
	Invalid_state(tparam, &tret);
	Return_error(tparam, &tret);
#endif
	Device_start(tparam, &tret);
	start_test_flag = false;

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
	ercd = R_IMPDRV_Start(NULL, p_core_info);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) handle == NULL", testno++);

	/* Parameter_error : p_core_info == NULL */
	ercd = R_IMPDRV_Start(handle, NULL);
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
		ercd = R_IMPDRV_Start(handle, &core_info);
		FailTestPrint(ercd, ((core_info.core_type == IMPDRV_CORE_TYPE_INVALID) ?
							IMPDRV_EC_NG_PARAM : IMPDRV_EC_NG_NOTSUPPORT),
							*tret, "(No.%d) Invalid core num for %s", testno++, type_to_name(core_info.core_type));
	}

	/* Invalid_error : Core_num = IMPDRV_CORE_NUM_MAX */
	core_info.core_type = IMPDRV_CORE_TYPE_IMP;
	core_info.core_num = IMPDRV_CORE_NUM_MAX;
	ercd = R_IMPDRV_Start(handle, &core_info);
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
		ercd = R_IMPDRV_Start(handle, &core_info);
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
		IMPDRV_STATE_READY,
		IMPDRV_STATE_EXEC,
		IMPDRV_STATE_INT
	};

	/* Back up data */
	memcpy(coretype_ctl_backup, p_impdrv_ctl->coretype_ctl, sizeof(p_impdrv_ctl->coretype_ctl));

	for (uint32_t i = 0; i < ARRAY_SIZE(invalid_state); i++)
	{
		set_state(p_initdata, invalid_state[i]);
		ercd = R_IMPDRV_Start(handle, p_core_info);
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

	/* Back up data */
	memcpy(coretype_ctl_backup, p_impdrv_ctl->coretype_ctl, sizeof(p_impdrv_ctl->coretype_ctl));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_TIME */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_TIME);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_TIME));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_PAR */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_PAR);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_BUSY */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_BUSY);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_BUSY));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_HANDLE */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_STATE */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_FAIL */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Fail to open device for IMP: R_OSAL_IoDeviceOpen returns OSAL_RETURN_PAR */
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_PAR, "imp_00");
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to open device for IMP: R_OSAL_IoDeviceOpen returns OSAL_RETURN_MEM */
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_MEM, "imp_00");
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_MEM));

	/* Fail to open device for IMP: R_OSAL_IoDeviceOpen returns OSAL_RETURN_DEV */
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_DEV, "imp_00");
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to open device for IMP: R_OSAL_IoDeviceOpen returns OSAL_RETURN_CONF */
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_CONF, "imp_00");
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_CONF));

	/* Fail to open device for IMP: R_OSAL_IoDeviceOpen returns OSAL_RETURN_BUSY */
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_BUSY, "imp_00");
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_BUSY));

	/* Fail to open device for IMP: R_OSAL_IoDeviceOpen returns OSAL_RETURN_ID */
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_ID, "imp_00");
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_ID));

	/* Fail to open device for IMP: R_OSAL_IoDeviceOpen returns OSAL_RETURN_STATE */
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_STATE, "imp_00");
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to open device for IMP RAM: R_OSAL_IoDeviceOpen returns OSAL_RETURN_FAIL */
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_FAIL, "imp_00");
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Fail to set required state for IMP0 after open device: R_OSAL_PmSetRequiredState returns OSAL_RETURN_PAR */
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_PAR, "imp_00", OSAL_PM_REQUIRED_STATE_REQUIRED);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to set required state for IMP0 after open device: R_OSAL_PmSetRequiredState returns OSAL_RETURN_DEV */
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_DEV, "imp_00", OSAL_PM_REQUIRED_STATE_REQUIRED);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to set required state for IMP0 after open device: R_OSAL_PmSetRequiredState returns OSAL_RETURN_TIME */
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_TIME, "imp_00", OSAL_PM_REQUIRED_STATE_REQUIRED);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_TIME));

	/* Fail to set required state for IMP0 after open device: R_OSAL_PmSetRequiredState returns OSAL_RETURN_HANDLE */
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_HANDLE, "imp_00", OSAL_PM_REQUIRED_STATE_REQUIRED);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to set required state for IMP0 after open device: R_OSAL_PmSetRequiredState returns OSAL_RETURN_STATE */
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_STATE, "imp_00", OSAL_PM_REQUIRED_STATE_REQUIRED);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to set required state for IMP0 after open device: R_OSAL_PmSetRequiredState returns OSAL_RETURN_UNSUPPORTED_OPERATION */
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_UNSUPPORTED_OPERATION, "imp_00", OSAL_PM_REQUIRED_STATE_REQUIRED);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_UNSUPPORTED_OPERATION));

	/* Fail to set required state for IMP0 after open device: R_OSAL_PmSetRequiredState returns OSAL_RETURN_FAIL */
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_FAIL, "imp_00", OSAL_PM_REQUIRED_STATE_REQUIRED);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Fail to reset core for OCV0: R_OSAL_PmSetResetState returns OSAL_RETURN_PAR */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
	p_core_info = get_core_info(p_initdata->core_info, p_initdata->use_core_num, IMPDRV_CORE_TYPE_OCV, 0);
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_ON);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 0, IMPDRV_OCV_RESET_STATUS_VAL);
	it_stub_set_R_OSAL_PmSetResetState(OSAL_RETURN_PAR);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetResetState for OCV0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_OFF);
	p_core_info = tparam->initdata.core_info;

	/* Fail to reset core for OCV0: R_OSAL_PmSetResetState returns OSAL_RETURN_DEV */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
	p_core_info = get_core_info(p_initdata->core_info, p_initdata->use_core_num, IMPDRV_CORE_TYPE_OCV, 0);
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_ON);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 0, IMPDRV_OCV_RESET_STATUS_VAL);
	it_stub_set_R_OSAL_PmSetResetState(OSAL_RETURN_DEV);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetResetState for OCV0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_OFF);
	p_core_info = tparam->initdata.core_info;

	/* Fail to reset core for OCV0: R_OSAL_PmSetResetState returns OSAL_RETURN_HANDLE */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
	p_core_info = get_core_info(p_initdata->core_info, p_initdata->use_core_num, IMPDRV_CORE_TYPE_OCV, 0);
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_ON);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 0, IMPDRV_OCV_RESET_STATUS_VAL);
	it_stub_set_R_OSAL_PmSetResetState(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetResetState for OCV0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_OFF);
	p_core_info = tparam->initdata.core_info;

	/* Fail to reset core for OCV0: R_OSAL_PmSetResetState returns OSAL_RETURN_STATE */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
	p_core_info = get_core_info(p_initdata->core_info, p_initdata->use_core_num, IMPDRV_CORE_TYPE_OCV, 0);
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_ON);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 0, IMPDRV_OCV_RESET_STATUS_VAL);
	it_stub_set_R_OSAL_PmSetResetState(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetResetState for OCV0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_OFF);
	p_core_info = tparam->initdata.core_info;

	/* Fail to reset core for OCV0: R_OSAL_PmSetResetState returns OSAL_RETURN_UNSUPPORTED_OPERATION */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
	p_core_info = get_core_info(p_initdata->core_info, p_initdata->use_core_num, IMPDRV_CORE_TYPE_OCV, 0);
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_ON);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 0, IMPDRV_OCV_RESET_STATUS_VAL);
	it_stub_set_R_OSAL_PmSetResetState(OSAL_RETURN_UNSUPPORTED_OPERATION);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetResetState for OCV0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_UNSUPPORTED_OPERATION));
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_OFF);
	p_core_info = tparam->initdata.core_info;

	/* Fail to reset core for OCV0: R_OSAL_PmSetResetState returns OSAL_RETURN_FAIL */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
	p_core_info = get_core_info(p_initdata->core_info, p_initdata->use_core_num, IMPDRV_CORE_TYPE_OCV, 0);
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_ON);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 0, IMPDRV_OCV_RESET_STATUS_VAL);
	it_stub_set_R_OSAL_PmSetResetState(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetResetState for OCV0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_OFF);
	p_core_info = tparam->initdata.core_info;

	/* Fail to write register: R_OSAL_IoWrite32 returns OSAL_RETURN_PAR */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_ON);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_PAR, IMPDRV_REG_IMP_MEM_FUNC_SA, 0);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_IMP_MEM_FUNC_SA returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_OFF);

	/* Fail to write register: R_OSAL_IoWrite32 returns OSAL_RETURN_DEV */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_ON);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_DEV, IMPDRV_REG_IMP_MEM_FUNC_SA, 0);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_IMP_MEM_FUNC_SA returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_OFF);

	/* Fail to write register: R_OSAL_IoWrite32 returns OSAL_RETURN_IO */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_ON);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_IO, IMPDRV_REG_IMP_MEM_FUNC_SA, 0);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_IMP_MEM_FUNC_SA returns %s", testno++, osal_err_to_name(OSAL_RETURN_IO));
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_OFF);

	/* Fail to write register: R_OSAL_IoWrite32 returns OSAL_RETURN_HANDLE */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_ON);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_HANDLE, IMPDRV_REG_IMP_MEM_FUNC_SA, 0);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_IMP_MEM_FUNC_SA returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_OFF);

	/* Fail to write register: R_OSAL_IoWrite32 returns OSAL_RETURN_STATE */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_ON);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_STATE, IMPDRV_REG_IMP_MEM_FUNC_SA, 0);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_IMP_MEM_FUNC_SA returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_OFF);

	/* Fail to write register: R_OSAL_IoWrite32 returns OSAL_RETURN_FAIL */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_ON);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_FAIL, IMPDRV_REG_IMP_MEM_FUNC_SA, 0);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_IMP_MEM_FUNC_SA returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_OFF);

	/* Fail to read register: R_OSAL_IoRead32 returns OSAL_RETURN_PAR */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_ON);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_PAR, 0, 0);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for offset IMPDRV_REG_IMP_HMDATA returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_OFF);

	/* Fail to read register: R_OSAL_IoRead32 returns OSAL_RETURN_DEV */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_ON);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_DEV, 0, 0);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for offset IMPDRV_REG_IMP_HMDATA returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_OFF);

	/* Fail to read register: R_OSAL_IoRead32 returns OSAL_RETURN_IO */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_ON);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_IO, 0, 0);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for offset IMPDRV_REG_IMP_HMDATA returns %s", testno++, osal_err_to_name(OSAL_RETURN_IO));
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_OFF);

	/* Fail to read register: R_OSAL_IoRead32 returns OSAL_RETURN_HANDLE */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_ON);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_HANDLE, 0, 0);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for offset IMPDRV_REG_IMP_HMDATA returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_OFF);

	/* Fail to read register: R_OSAL_IoRead32 returns OSAL_RETURN_STATE */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_ON);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_STATE, 0, 0);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for offset IMPDRV_REG_IMP_HMDATA returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_OFF);

	/* Fail to read register: R_OSAL_IoRead32 returns OSAL_RETURN_FAIL */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_ON);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_FAIL, 0, 0);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for offset IMPDRV_REG_IMP_HMDATA returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_OFF);

	/* Read register returns unexpected value: R_OSAL_IoRead32 returns unexpected value */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_ON);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 0, IMPDRV_IMP_HMDATA_VAL + 1);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_CHECKFAIL, *tret, "(No.%d) R_OSAL_IoRead32 for offset IMPDRV_REG_IMP_HMDATA returns unexpected value", testno++);
	(void)R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_OFF);

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_HANDLE */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_STATE */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_FAIL */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_Start(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Get backup data */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
}
#endif

static void Device_start(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
    st_impdrv_initdata_t *p_initdata = &tparam->initdata;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	uint32_t cnt = 0;
	
	/* driver_start : output parameter */
	for (uint32_t i = 0; i < p_initdata->use_core_num; i++)
	{
		st_impdrv_core_info_t *p_core_info = &p_initdata->core_info[i];
		e_impdrv_core_type_t core_type = p_core_info->core_type;
		uint32_t core_num = p_core_info->core_num;
		st_impdrv_coretypectl_t *coretype_ctl = &p_impdrv_ctl->coretype_ctl[core_type];
		int32_t result = -1;
		uint32_t *dtcm_addr = NULL;

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
		if( IMPDRV_CORE_TYPE_DSP == core_type)
		{
			cnt = 0;
			coretype_ctl->core_ctl[0].dsp_info.dsp_fw.addr_phys = 0x40000000U;
			coretype_ctl->core_ctl[0].dsp_info.dsp_fw.size = 0x2000U;

			/* allocate cpu addr dtcm */
			dtcm_addr = (uint32_t *)MemAlloc(osal_mmngr, 2u, 64u, IMP_MALLOC_HW);
			dtcm_addr[0] = 0x12345678U;
			dtcm_addr[1] = 0x87654321U;
			coretype_ctl->core_ctl[0].dsp_info.dsp_dtcm.addr_phys = util_mem_get_paddr(dtcm_addr);
			coretype_ctl->core_ctl[0].dsp_info.dsp_dtcm.size = 2u;
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, cnt++, 0);
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, cnt++, 0);
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, cnt++, coretype_ctl->core_ctl[0].dsp_info.dsp_fw.addr_phys);
		}
		
		ercd = R_IMPDRV_Start(handle, p_core_info);
		if (ercd != IMPDRV_EC_OK) {
			core_testmsg(p_core_info, "%s=%d\n", TEST_MSG_NAME, ercd);
			result = -1;
		} else {
			/* Check state of the core */
			if (coretype_ctl->core_ctl[core_num].core_state != IMPDRV_STATE_READY) {
				core_testmsg(p_core_info, "state=%d, it is not READY(test NG)\n", state_to_name(coretype_ctl->core_ctl[core_num].core_state));
				result = -1;
			} else {
				result = 0;
			}
		}

		if ((IMPDRV_CORE_TYPE_DSP == core_type) && (result == 0))
		{
			/* DSP Register Check */
			/* MSS_DMBA */
			if (g_write_value[WRITE_REG_IDX_MSS_DMBA] != 0x00000000U)
			{
				core_testmsg(p_core_info, "%s[%08x] is not [%08x]\n", "MSS_DMBA", g_write_value[WRITE_REG_IDX_MSS_DMBA], 0x00000000U);
				result = -1;
			}
			/* P_ADD0_ATT0 */
			if (g_write_value[WRITE_REG_IDX_P_ADD0_ATT0] != 0x00000260U)
			{
				core_testmsg(p_core_info, "%s[%08x] is not [%08x]\n", "P_ADD0_ATT0", g_write_value[WRITE_REG_IDX_P_ADD0_ATT0], 0x00000260U);
				result = -1;
			}
			/* P_ADD2_ATT0 */
			if (g_write_value[WRITE_REG_IDX_P_ADD2_ATT0] != 0x00000260U)
			{
				core_testmsg(p_core_info, "%s[%08x] is not [%08x]\n", "P_ADD2_ATT0", g_write_value[WRITE_REG_IDX_P_ADD2_ATT0], 0x00000260U);
				result = -1;
			}
			/* P_ADD3_START */
			if (g_write_value[WRITE_REG_IDX_P_ADD3_START] != 0x00040000U)
			{
				core_testmsg(p_core_info, "%s[%08x] is not [%08x]\n", "P_ADD3_START", g_write_value[WRITE_REG_IDX_P_ADD3_START], 0x00040000U);
				result = -1;
			}
			/* P_ADD3_ATT0 */
			if (g_write_value[WRITE_REG_IDX_P_ADD3_ATT0] != 0x00000201U)
			{
				core_testmsg(p_core_info, "%s[%08x] is not [%08x]\n", "P_ADD3_ATT0", g_write_value[WRITE_REG_IDX_P_ADD3_ATT0], 0x00000201U);
				result = -1;
			}
			/* P_ADD4_START */
			if (g_write_value[WRITE_REG_IDX_P_ADD4_START] != 0x10040002U)
			{
				core_testmsg(p_core_info, "%s[%08x] is not [%08x]\n", "P_ADD4_START", g_write_value[WRITE_REG_IDX_P_ADD4_START], 0x10040002U);
				result = -1;
			}
			/* D_DDCL */
			if (g_write_value[WRITE_REG_IDX_D_DDCL] != 0x000001B0U)
			{
				core_testmsg(p_core_info, "%s[%08x] is not [%08x]\n", "D_DDCL", g_write_value[WRITE_REG_IDX_D_DDCL], 0x000001B0U);
				result = -1;
			}
#if defined (IMPDRV_DSPCRC_DETECT_ADDDEF)
			uint32_t exp_d_sft_scpd_3;

			/* Register check related CRC */
			/* D_SFT_SCPD_0 (1st write) */
			if (g_write_value[WRITE_REG_IDX_D_SFT_SCPD_0] != 0x00000001U)
			{
				core_testmsg(p_core_info, "%s[%08x] is not [%08x]\n", " D_SFT_SCPD_0 (1st write)", g_write_value[WRITE_REG_IDX_D_SFT_SCPD_0], 0x00000001U);
				result = -1;
			}
			/* D_SFT_SCPD_0 (2nd write) */
			if (g_write_value[WRITE_REG_IDX_D_SFT_SCPD_0_2ND] != 0x00000081U)
			{
				core_testmsg(p_core_info, "%s[%08x] is not [%08x]\n", "D_SFT_SCPD_0 (2nd write)", g_write_value[WRITE_REG_IDX_D_SFT_SCPD_0_2ND], 0x00000081U);
				result = -1;
			}
			/* D_SFT_SCPD_1 */
			if (g_write_value[WRITE_REG_IDX_D_SFT_SCPD_1] != util_mem_get_paddr(dtcm_addr))
			{
				core_testmsg(p_core_info, "%s[%08x] is not [%08x]\n", "D_SFT_SCPD_1", g_write_value[WRITE_REG_IDX_D_SFT_SCPD_1], util_mem_get_paddr(dtcm_addr));
				result = -1;
			}
			/* D_SFT_SCPD_2 */
			if (g_write_value[WRITE_REG_IDX_D_SFT_SCPD_2] != 0x00000002U)
			{
				core_testmsg(p_core_info, "%s[%08x] is not [%08x]\n", "D_SFT_SCPD_2", g_write_value[WRITE_REG_IDX_D_SFT_SCPD_2], 0x00000002U);
				result = -1;
			}
			/* D_SFT_SCPD_3 */
#ifdef IMPDRV_DSPCRC_DETECT_ADDDEF == 1
			exp_d_sft_scpd_3 = 0xFFFFFFFFU;
#elif IMPDRV_DSPCRC_DETECT_ADDDEF == 0
			exp_d_sft_scpd_3 = 0xF0A1BB94U;
#endif
			if (g_write_value[WRITE_REG_IDX_D_SFT_SCPD_3] != exp_d_sft_scpd_3)
			{
				core_testmsg(p_core_info, "%s[%08x] is not [%08x]\n", "D_SFT_SCPD_3", g_write_value[WRITE_REG_IDX_D_SFT_SCPD_3], exp_d_sft_scpd_3);
				result = -1;
			}
#endif
			/* For debug */
			if (result == 0)
			{
				printf("Register check is ALL OK !!!\n");
			}
		}

		if (dtcm_addr != NULL)
		{
			MemFree(osal_mmngr, dtcm_addr);
		}
		it_stub_reset();
		SuccessTestPrint(result == 0, *tret, "(No.%d) %s%d ", testno++, type_to_name(core_type), core_num);
	}
}
