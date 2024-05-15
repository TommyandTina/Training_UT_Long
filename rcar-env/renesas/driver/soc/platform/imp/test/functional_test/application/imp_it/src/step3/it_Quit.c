#define TEST_MSG_NAME "R_IMPDRV_Quit"

#include "it_common.h"
#include "it_stub.h"

#if defined(IT_ABNORMAL) && ( defined (V4H) || defined (V4H2) )
  #undef IT_ABNORMAL    /* TODO: Workaround for V4H integration testing */
#endif

static void set_state(st_impdrv_initdata_t *const p_data, e_impdrv_state_t state);
static void prepare_test_for_quit();
#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret);
static void Return_error(struct test_params_t *tparam, struct test_result_t *tret);
#endif
static void Device_quit(struct test_params_t *tparam, struct test_result_t *tret);

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

static void prepare_test_for_quit()
{
#if defined (V4H) || defined (V4H2)
#elif (defined (V3M2) || defined (V3H1) || defined (V3H2))
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 0, IMPDRV_REG_GROUP_INIT_VAL);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 1, IMPDRV_REG_IMR_INIT_VAL);
#else
	#error 'Oops ...! Build option for unsupported products.'
#endif
}

int32_t it_Quit_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};
	quit_test_flag = true;

#ifdef IT_ABNORMAL
	testno = 56;
	Parameter_error(tparam, &tret);
	Invalid_state(tparam, &tret);
	Return_error(tparam, &tret);
#endif
	testno = 116;
	Device_quit(tparam, &tret);
	quit_test_flag = false;

	PrintTestResult(tret, test_result);

	return 0;
}

#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;

	/* Parameter_error : p_handle == NULL */
	ercd = R_IMPDRV_Quit(NULL);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) p_handle == NULL", testno++);
}

static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
	impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	e_impdrv_state_t invalid_state[] = {
		IMPDRV_STATE_READY,
		IMPDRV_STATE_EXEC,
		IMPDRV_STATE_INT
	};

	/* Back up data */
	memcpy(coretype_ctl_backup, p_impdrv_ctl->coretype_ctl, sizeof(p_impdrv_ctl->coretype_ctl));

	for (uint32_t i = 0; i < ARRAY_SIZE(invalid_state); i++)
	{
		set_state(p_initdata, invalid_state[i]);
		ercd = R_IMPDRV_Quit(handle);
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
	impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;

	/* R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_TIME */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_TIME);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_TIME));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_PAR */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_PAR);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_BUSY */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_BUSY);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_BUSY));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_HANDLE */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_STATE */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_FAIL */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Fail to set released state for HW resource: R_OSAL_PmSetRequiredState returns OSAL_RETURN_PAR */
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_PAR, "imp_spmi_00", OSAL_PM_REQUIRED_STATE_RELEASED);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (RELEASED) for HW resource returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to set released state for HW resource: R_OSAL_PmSetRequiredState returns OSAL_RETURN_DEV */
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_DEV, "imp_spmi_00", OSAL_PM_REQUIRED_STATE_RELEASED);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (RELEASED) for HW resource returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to set released state for HW resource: R_OSAL_PmSetRequiredState returns OSAL_RETURN_TIME */
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_TIME, "imp_spmi_00", OSAL_PM_REQUIRED_STATE_RELEASED);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (RELEASED) for HW resource returns %s", testno++, osal_err_to_name(OSAL_RETURN_TIME));

	/* Fail to set released state for HW resource: R_OSAL_PmSetRequiredState returns OSAL_RETURN_HANDLE */
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_HANDLE, "imp_spmi_00", OSAL_PM_REQUIRED_STATE_RELEASED);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (RELEASED) for HW resource returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to set released state for HW resource: R_OSAL_PmSetRequiredState returns OSAL_RETURN_STATE */
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_STATE, "imp_spmi_00", OSAL_PM_REQUIRED_STATE_RELEASED);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (RELEASED) for HW resource returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to set released state for HW resource: R_OSAL_PmSetRequiredState returns OSAL_RETURN_UNSUPPORTED_OPERATION */
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_UNSUPPORTED_OPERATION, "imp_spmi_00", OSAL_PM_REQUIRED_STATE_RELEASED);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (RELEASED) for HW resource returns %s", testno++, osal_err_to_name(OSAL_RETURN_UNSUPPORTED_OPERATION));

	/* Fail to set released state for HW resource after open device: R_OSAL_PmSetRequiredState returns OSAL_RETURN_FAIL */
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_FAIL, "imp_spmi_00", OSAL_PM_REQUIRED_STATE_RELEASED);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (RELEASED) for HW resource returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Fail to close device for HW resource: R_OSAL_IoDeviceClose returns OSAL_RETURN_DEV */
	it_stub_set_R_OSAL_IoDeviceClose(OSAL_RETURN_DEV, "imp_spmi_00");
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceClose for HW resource returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to close device for HW resource: R_OSAL_IoDeviceClose returns OSAL_RETURN_BUSY */
	it_stub_set_R_OSAL_IoDeviceClose(OSAL_RETURN_BUSY, "imp_spmi_00");
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceClose for HW resource returns %s", testno++, osal_err_to_name(OSAL_RETURN_BUSY));

	/* Fail to close device for HW resource: R_OSAL_IoDeviceClose returns OSAL_RETURN_HANDLE */
	it_stub_set_R_OSAL_IoDeviceClose(OSAL_RETURN_HANDLE, "imp_spmi_00");
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceClose for HW resource returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to close device for HW resource: R_OSAL_IoDeviceClose returns OSAL_RETURN_STATE */
	it_stub_set_R_OSAL_IoDeviceClose(OSAL_RETURN_STATE, "imp_spmi_00");
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceClose for HW resource returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to close device for HW resource: R_OSAL_IoDeviceClose returns OSAL_RETURN_FAIL */
	it_stub_set_R_OSAL_IoDeviceClose(OSAL_RETURN_FAIL, "imp_spmi_00");
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceClose for HW resource returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Fail to write INTerrupt Mask register: R_OSAL_IoWrite32 returns OSAL_RETURN_PAR */
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_PAR, s_impdrv_inten_addr[p_initdata->instance_num], IMPDRV_REG_IMR_INIT_VAL);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for masking INTerrupt Mask Register returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to write INTerrupt Mask register: R_OSAL_IoWrite32 returns OSAL_RETURN_DEV */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_DEV, s_impdrv_inten_addr[p_initdata->instance_num], IMPDRV_REG_IMR_INIT_VAL);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for masking INTerrupt Mask Register returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to write INTerrupt Mask register: R_OSAL_IoWrite32 returns OSAL_RETURN_IO */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_IO, s_impdrv_inten_addr[p_initdata->instance_num], IMPDRV_REG_IMR_INIT_VAL);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for masking INTerrupt Mask Register returns %s", testno++, osal_err_to_name(OSAL_RETURN_IO));

	/* Fail to write INTerrupt Mask register: R_OSAL_IoWrite32 returns OSAL_RETURN_HANDLE */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_HANDLE, s_impdrv_inten_addr[p_initdata->instance_num], IMPDRV_REG_IMR_INIT_VAL);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for masking INTerrupt Mask Register returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to write INTerrupt Mask register: R_OSAL_IoWrite32 returns OSAL_RETURN_STATE */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_STATE, s_impdrv_inten_addr[p_initdata->instance_num], IMPDRV_REG_IMR_INIT_VAL);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for masking INTerrupt Mask Register returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to write INTerrupt Mask register: R_OSAL_IoWrite32 returns OSAL_RETURN_FAIL */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_FAIL, s_impdrv_inten_addr[p_initdata->instance_num], IMPDRV_REG_IMR_INIT_VAL);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for masking INTerrupt Mask Register returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* R_OSAL_IoRead32 returns unexpected read-back value */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 0, IMPDRV_REG_GROUP_INIT_VAL + 1);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_CHECKFAIL, *tret, "(No.%d) R_OSAL_IoRead32 for group_reg returns unexpected read-back value", testno++);

	/* Fail to disable interrupt ISR: R_OSAL_InterruptDisableIsr returns OSAL_RETURN_PAR */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_InterruptDisableIsr(OSAL_RETURN_PAR);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_InterruptDisableIsr returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to disable interrupt ISR: R_OSAL_InterruptDisableIsr returns OSAL_RETURN_DEV */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_InterruptDisableIsr(OSAL_RETURN_DEV);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_InterruptDisableIsr returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to disable interrupt ISR: R_OSAL_InterruptDisableIsr returns OSAL_RETURN_HANDLE */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_InterruptDisableIsr(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_InterruptDisableIsr returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to disable interrupt ISR: R_OSAL_InterruptDisableIsr returns OSAL_RETURN_STATE */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_InterruptDisableIsr(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_InterruptDisableIsr returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to disable interrupt ISR: R_OSAL_InterruptDisableIsr returns OSAL_RETURN_FAIL */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_InterruptDisableIsr(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_InterruptDisableIsr returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Fail to unregister interrupt ISR: R_OSAL_InterruptUnregisterIsr returns OSAL_RETURN_PAR */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_InterruptUnregisterIsr(OSAL_RETURN_PAR);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_InterruptUnregisterIsr returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to unregister interrupt ISR: R_OSAL_InterruptUnregisterIsr returns OSAL_RETURN_DEV */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_InterruptUnregisterIsr(OSAL_RETURN_DEV);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_InterruptUnregisterIsr returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to unregister interrupt ISR: R_OSAL_InterruptUnregisterIsr returns OSAL_RETURN_BUSY */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_InterruptUnregisterIsr(OSAL_RETURN_BUSY);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_InterruptUnregisterIsr returns %s", testno++, osal_err_to_name(OSAL_RETURN_BUSY));

	/* Fail to unregister interrupt ISR: R_OSAL_InterruptUnregisterIsr returns OSAL_RETURN_HANDLE */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_InterruptUnregisterIsr(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_InterruptUnregisterIsr returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to unregister interrupt ISR: R_OSAL_InterruptUnregisterIsr returns OSAL_RETURN_STATE */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_InterruptUnregisterIsr(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_InterruptUnregisterIsr returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to unregister interrupt ISR: R_OSAL_InterruptUnregisterIsr returns OSAL_RETURN_FAIL */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_InterruptUnregisterIsr(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_InterruptUnregisterIsr returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Fail to set released state for IMP RAM after open device: R_OSAL_PmSetRequiredState returns OSAL_RETURN_PAR */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_PAR, "imp_top_00", OSAL_PM_REQUIRED_STATE_RELEASED);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (RELEASED) for RAM returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to set released state for IMP RAM after open device: R_OSAL_PmSetRequiredState returns OSAL_RETURN_DEV */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	close_device_io_flg = true;
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_DEV, "imp_top_00", OSAL_PM_REQUIRED_STATE_RELEASED);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	close_device_io_flg = false;
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (RELEASED) for RAM returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to set released state for IMP RAM after open device: R_OSAL_PmSetRequiredState returns OSAL_RETURN_TIME */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	close_device_io_flg = true;
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_TIME, "imp_top_00", OSAL_PM_REQUIRED_STATE_RELEASED);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	close_device_io_flg = false;
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (RELEASED) for RAM returns %s", testno++, osal_err_to_name(OSAL_RETURN_TIME));

	/* Fail to set released state for IMP RAM after open device: R_OSAL_PmSetRequiredState returns OSAL_RETURN_HANDLE */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	close_device_io_flg = true;
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_HANDLE, "imp_top_00", OSAL_PM_REQUIRED_STATE_RELEASED);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	close_device_io_flg = false;
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (RELEASED) for RAM returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to set released state for IMP RAM after open device: R_OSAL_PmSetRequiredState returns OSAL_RETURN_STATE */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	close_device_io_flg = true;
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_STATE, "imp_top_00", OSAL_PM_REQUIRED_STATE_RELEASED);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	close_device_io_flg = false;
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (RELEASED) for RAM returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to set released state for IMP RAM after open device: R_OSAL_PmSetRequiredState returns OSAL_RETURN_UNSUPPORTED_OPERATION */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	close_device_io_flg = true;
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_UNSUPPORTED_OPERATION, "imp_top_00", OSAL_PM_REQUIRED_STATE_RELEASED);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	close_device_io_flg = false;
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (RELEASED) for RAM returns %s", testno++, osal_err_to_name(OSAL_RETURN_UNSUPPORTED_OPERATION));

	/* Fail to set released state for IMP RAM after open device: R_OSAL_PmSetRequiredState returns OSAL_RETURN_FAIL */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	close_device_io_flg = true;
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_FAIL, "imp_top_00", OSAL_PM_REQUIRED_STATE_RELEASED);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	close_device_io_flg = false;
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (RELEASED) for RAM returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));
	
	/* Fail to close device for IMP RAM: R_OSAL_IoDeviceClose returns OSAL_RETURN_DEV */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	close_device_io_flg = true;
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_IoDeviceClose(OSAL_RETURN_DEV, "imp_top_00");
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	close_device_io_flg = false;
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceClose for IMP RAM returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to close device for IMP RAM: R_OSAL_IoDeviceClose returns OSAL_RETURN_BUSY */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	close_device_io_flg = true;
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_IoDeviceClose(OSAL_RETURN_BUSY, "imp_top_00");
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	close_device_io_flg = false;
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceClose for IMP RAM returns %s", testno++, osal_err_to_name(OSAL_RETURN_BUSY));

	/* Fail to close device for IMP RAM: R_OSAL_IoDeviceClose returns OSAL_RETURN_HANDLE */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	close_device_io_flg = true;
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_IoDeviceClose(OSAL_RETURN_HANDLE, "imp_top_00");
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	close_device_io_flg = false;
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceClose for IMP RAM returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to close device for IMP RAM: R_OSAL_IoDeviceClose returns OSAL_RETURN_STATE */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	close_device_io_flg = true;
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_IoDeviceClose(OSAL_RETURN_STATE, "imp_top_00");
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	close_device_io_flg = false;
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceClose for IMP RAM returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to close device for IMP RAM: R_OSAL_IoDeviceClose returns OSAL_RETURN_FAIL */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	close_device_io_flg = true;
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_IoDeviceClose(OSAL_RETURN_FAIL, "imp_top_00");
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	close_device_io_flg = false;
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceClose for IMP RAM returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_HANDLE */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	close_device_io_flg = true;
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	close_device_io_flg = false;
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_STATE */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_FAIL */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Failed to destroy mutex: R_OSAL_ThsyncMutexDestroy returns OSAL_RETURN_MEM */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_ThsyncMutexDestroy(OSAL_RETURN_MEM);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexDestroy returns %s", testno++, osal_err_to_name(OSAL_RETURN_MEM));

	/* Failed to destroy mutex: R_OSAL_ThsyncMutexDestroy returns OSAL_RETURN_BUSY */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_ThsyncMutexDestroy(OSAL_RETURN_BUSY);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexDestroy returns %s", testno++, osal_err_to_name(OSAL_RETURN_BUSY));

	/* Failed to destroy mutex: R_OSAL_ThsyncMutexDestroy returns OSAL_RETURN_HANDLE */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_ThsyncMutexDestroy(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexDestroy returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to destroy mutex: R_OSAL_ThsyncMutexDestroy returns OSAL_RETURN_STATE */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_ThsyncMutexDestroy(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexDestroy returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Failed to destroy mutex: R_OSAL_ThsyncMutexDestroy returns OSAL_RETURN_FAIL */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	prepare_test_for_quit();
	it_stub_set_R_OSAL_ThsyncMutexDestroy(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexDestroy returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

_error:
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
}
#endif

static void Device_quit(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	int32_t result = -1;

	/* driver_quit : output parameter */
	it_stub_reset();
	prepare_test_for_quit();

	ercd = R_IMPDRV_Quit(handle);
	if (ercd != IMPDRV_EC_OK) {
		testmsg("(No.%d) fail(test NG %d)\n", testno++, ercd);
	} else {
		testmsg("(No.%d) Success(test OK)\n", testno++);
		result = 0;
	}

	CountResult(result == 0, tret);
}