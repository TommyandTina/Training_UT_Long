#define TEST_MSG_NAME "R_IMPDRV_RegRequired"

#include "it_common.h"
#include "it_stub.h"

static void set_state(st_impdrv_initdata_t *const p_data, e_impdrv_state_t state);
#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret);
static void Return_error(struct test_params_t *tparam, struct test_result_t *tret);
#endif
static void Device_reg_required(struct test_params_t *tparam, struct test_result_t *tret);

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

int32_t it_RegRequired_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

	/* Execute test */
#ifdef IT_ABNORMAL
	testno = 36;
	Parameter_error(tparam, &tret);
	Invalid_state(tparam, &tret);
	Return_error(tparam, &tret);
#endif
	Device_reg_required(tparam, &tret);

	PrintTestResult(tret, test_result);

	return 0;
}

#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	const char *device_id = "imp_spmi_00";
	e_impdrv_reg_req_state_t  new_state = IMPDRV_REG_REQ_STATE_REQUIRED;

	/* Parameter_error : handle == NULL */
	ercd = R_IMPDRV_RegRequired(NULL, device_id, new_state);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) handle = NULL", testno++);

	/* Parameter_error : p_device_id = NULL */
	ercd = R_IMPDRV_RegRequired(handle, NULL, new_state);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) p_device_id = NULL", testno++);

	/* Parameter_error : device_id[0] = '\0' */
	ercd = R_IMPDRV_RegRequired(handle, "\0", new_state);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) device_id[0] = \'\\0\'", testno++);

	/* Parameter_error : device_id = "dummy" */
	ercd = R_IMPDRV_RegRequired(handle, "dummy", new_state);
	FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) device_id = \"dummy\"", testno++);

	/* Parameter_error : new_state is IMPDRV_REG_REQ_STATE_INVALID */
	ercd = R_IMPDRV_RegRequired(handle, device_id, IMPDRV_REG_REQ_STATE_INVALID);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) new_state = IMPDRV_REG_REQ_STATE_INVALID", testno++);

	/* Parameter_error : new_state is 3 */
	ercd = R_IMPDRV_RegRequired(handle, device_id, 3U);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) new_state = 3", testno++);
}

static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	const char *device_id = "imp_spmi_00";
	e_impdrv_reg_req_state_t  new_state = IMPDRV_REG_REQ_STATE_REQUIRED;
	e_impdrv_state_t invalid_state[] = {
		IMPDRV_STATE_UNINIT
	};

	/* Back up data */
	memcpy(coretype_ctl_backup, p_impdrv_ctl->coretype_ctl, sizeof(p_impdrv_ctl->coretype_ctl));

	for (uint32_t i = 0; i < ARRAY_SIZE(invalid_state); i++)
	{
		set_state(p_initdata, invalid_state[i]);
		ercd = R_IMPDRV_RegRequired(handle, device_id, new_state);
		FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) %s", testno++, state_to_name(invalid_state[i]));
		(void)R_IMPDRV_RegRequired(handle, device_id, IMPDRV_REG_REQ_STATE_RELEASED);
	}

	/* Get backup data */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
}

static void Return_error(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	const char *device_id = "imp_spmi_00";
	e_impdrv_reg_req_state_t  new_state = IMPDRV_REG_REQ_STATE_REQUIRED;
	static st_impdrv_hwrsc_mng_t hwrsc_mng_table_backup_before;
	static st_impdrv_hwrsc_mng_t hwrsc_mng_table_backup_after;

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_TIME */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_TIME);
	ercd = R_IMPDRV_RegRequired(handle, device_id, new_state);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_TIME));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_PAR */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_PAR);
	ercd = R_IMPDRV_RegRequired(handle, device_id, new_state);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_BUSY */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_BUSY);
	ercd = R_IMPDRV_RegRequired(handle, device_id, new_state);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_BUSY));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_HANDLE */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_RegRequired(handle, device_id, new_state);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_STATE */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_RegRequired(handle, device_id, new_state);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_FAIL */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_RegRequired(handle, device_id, new_state);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_HANDLE */
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_RegRequired(handle, device_id, new_state);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));
	(void)R_IMPDRV_RegRequired(handle, device_id, IMPDRV_REG_REQ_STATE_RELEASED);

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_STATE */
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_RegRequired(handle, device_id, new_state);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));
	(void)R_IMPDRV_RegRequired(handle, device_id, IMPDRV_REG_REQ_STATE_RELEASED);

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_FAIL */
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_RegRequired(handle, device_id, new_state);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));
	(void)R_IMPDRV_RegRequired(handle, device_id, IMPDRV_REG_REQ_STATE_RELEASED);

	/* Broken memory occurs returns IMPDRV_EC_NG_SYSTEMERROR */
	memcpy(&hwrsc_mng_table_backup_before, p_impdrv_ctl->common_ctl.p_hwrsc_mng_table, sizeof(st_impdrv_hwrsc_mng_t));
	(void)R_IMPDRV_RegRequired(handle, device_id, new_state);
	memcpy(&hwrsc_mng_table_backup_after, p_impdrv_ctl->common_ctl.p_hwrsc_mng_table, sizeof(st_impdrv_hwrsc_mng_t)); // Back up status after RegRequire-ing (REQUIRED)
	memcpy(p_impdrv_ctl->common_ctl.p_hwrsc_mng_table, &hwrsc_mng_table_backup_before, sizeof(st_impdrv_hwrsc_mng_t));  // Broken memory occurs here
	ercd = R_IMPDRV_RegRequired(handle, device_id, IMPDRV_REG_REQ_STATE_RELEASED);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) Broken memory occurrence returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));
	memcpy(p_impdrv_ctl->common_ctl.p_hwrsc_mng_table, &hwrsc_mng_table_backup_after, sizeof(st_impdrv_hwrsc_mng_t)); // Get back data for normal RELEASED
	(void)R_IMPDRV_RegRequired(handle, device_id, IMPDRV_REG_REQ_STATE_RELEASED);
}
#endif

static void Device_reg_required(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	const char *device_id = "imp_spmi_00";
	e_impdrv_reg_req_state_t  new_state = IMPDRV_REG_REQ_STATE_REQUIRED;
	int32_t result = 0;
	
	/* driver_reg_required : output parameter */
	ercd = R_IMPDRV_RegRequired(handle, device_id, new_state);
	if (ercd != IMPDRV_EC_OK) {
		testmsg("(No.%d) fail(test NG %d)\n", testno++, ercd);
		result = -1;
	} else {
		testmsg("(No.%d) Success(test OK)\n", testno++);
	}

	CountResult(result == 0, tret);
}