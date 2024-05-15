#define TEST_MSG_NAME "R_IMPDRV_RegWrite32"

#include "it_common.h"
#include "it_stub.h"

static void set_state(st_impdrv_initdata_t *const p_data, e_impdrv_state_t state);
#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret);
static void Return_error(struct test_params_t *tparam, struct test_result_t *tret);
#endif
static void Device_reg_write32(struct test_params_t *tparam, struct test_result_t *tret);

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

int32_t it_RegWrite32_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

	/* Execute test */
#ifdef IT_ABNORMAL
	testno = 41;
	Parameter_error(tparam, &tret);
	Invalid_state(tparam, &tret);
	Return_error(tparam, &tret);
#endif
	Device_reg_write32(tparam, &tret);

	PrintTestResult(tret, test_result);

	return 0;
}

#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	const st_impdrv_hwrsc_def_t *p_hwrsc_def_table = (const st_impdrv_hwrsc_def_t *)p_impdrv_ctl->common_ctl.p_hwrsc_def_table;
	st_impdrv_reg_info_t reg_info;
	const char *device_id = "imp_spmi_00";
	uintptr_t offset;
	uint32_t  data = 5U;

	/* prepare parameters */
	get_correct_hw_info(p_hwrsc_def_table, device_id, &reg_info);
	offset = reg_info.addr_phys - OFFSETADDR_SPMI0;

	/* Parameter_error : handle == NULL */
	ercd = R_IMPDRV_RegWrite32(NULL, device_id, offset, data);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) handle = NULL", testno++);

	/* Parameter_error : p_device_id = NULL */
	ercd = R_IMPDRV_RegWrite32(handle, NULL, offset, data);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) p_device_id = NULL", testno++);

	/* Parameter_error : device_id[0] = '\0' */
	ercd = R_IMPDRV_RegWrite32(handle, "\0", offset, data);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) device_id[0] = \'\\0\'", testno++);

	/* Parameter_error : device_id = "dummy" */
	ercd = R_IMPDRV_RegWrite32(handle, "dummy", offset, data);
	FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) device_id = \"dummy\"", testno++);

	/* Parameter_error : offset is 3U (invalid alignment) */
	ercd = R_IMPDRV_RegWrite32(handle, device_id, 3U, data);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) offset = 3U", testno++);

	/* Parameter_error : offset is 5U (invalid alignment) */
	ercd = R_IMPDRV_RegWrite32(handle, device_id, 5U, data);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) offset = 5U", testno++);

	/* Parameter_error : offset is 2U (invalid alignment) */
	ercd = R_IMPDRV_RegWrite32(handle, device_id, 2U, data);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) offset = 2U", testno++);

	/* Parameter_error : offset exceeds size */
	ercd = R_IMPDRV_RegWrite32(handle, device_id, SIZE_SPMI0, data);
	FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) offset exceeds size", testno++);
}

static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	const st_impdrv_hwrsc_def_t *p_hwrsc_def_table = (const st_impdrv_hwrsc_def_t *)p_impdrv_ctl->common_ctl.p_hwrsc_def_table;
	st_impdrv_reg_info_t reg_info;
	const char *device_id = "imp_spmi_00";
	uintptr_t offset;
	uint32_t  data = 5U;
	e_impdrv_state_t invalid_state[] = {
		IMPDRV_STATE_UNINIT
	};

	/* prepare parameters */
	get_correct_hw_info(p_hwrsc_def_table, device_id, &reg_info);
	offset = reg_info.addr_phys - OFFSETADDR_SPMI0;

	/* Back up data */
	memcpy(coretype_ctl_backup, p_impdrv_ctl->coretype_ctl, sizeof(p_impdrv_ctl->coretype_ctl));

	for (uint32_t i = 0; i < ARRAY_SIZE(invalid_state); i++)
	{
		set_state(p_initdata, invalid_state[i]);
		ercd = R_IMPDRV_RegWrite32(handle, device_id, offset, data);
		FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) %s", testno++, state_to_name(invalid_state[i]));
	}

	/* Get backup data */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
}

static void Return_error(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	const st_impdrv_hwrsc_def_t *p_hwrsc_def_table = (const st_impdrv_hwrsc_def_t *)p_impdrv_ctl->common_ctl.p_hwrsc_def_table;
	st_impdrv_reg_info_t reg_info;
	const char *device_id = "imp_spmi_00";
	uintptr_t offset;
	uint32_t  data = 5U;

	/* prepare parameters */
	get_correct_hw_info(p_hwrsc_def_table, device_id, &reg_info);
	offset = reg_info.addr_phys - OFFSETADDR_SPMI0;

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_TIME */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_TIME);
	ercd = R_IMPDRV_RegWrite32(handle, device_id, offset, data);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_TIME));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_PAR */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_PAR);
	ercd = R_IMPDRV_RegWrite32(handle, device_id, offset, data);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_BUSY */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_BUSY);
	ercd = R_IMPDRV_RegWrite32(handle, device_id, offset, data);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_BUSY));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_HANDLE */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_RegWrite32(handle, device_id, offset, data);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_STATE */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_RegWrite32(handle, device_id, offset, data);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_FAIL */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_RegWrite32(handle, device_id, offset, data);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Fail to read register version for IMP0: R_OSAL_IoWrite32 returns OSAL_RETURN_PAR */
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_PAR, offset, data);
	ercd = R_IMPDRV_RegWrite32(handle, device_id, offset, data);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to read register version for IMP0: R_OSAL_IoWrite32 returns OSAL_RETURN_DEV */
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_DEV, offset, data);
	ercd = R_IMPDRV_RegWrite32(handle, device_id, offset, data);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to read register version for IMP0: R_OSAL_IoWrite32 returns OSAL_RETURN_IO */
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_IO, offset, data);
	ercd = R_IMPDRV_RegWrite32(handle, device_id, offset, data);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 returns %s", testno++, osal_err_to_name(OSAL_RETURN_IO));

	/* Fail to read register version for IMP0: R_OSAL_IoWrite32 returns OSAL_RETURN_HANDLE */
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_HANDLE, offset, data);
	ercd = R_IMPDRV_RegWrite32(handle, device_id, offset, data);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to read register version for IMP0: R_OSAL_IoWrite32 returns OSAL_RETURN_STATE */
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_STATE, offset, data);
	ercd = R_IMPDRV_RegWrite32(handle, device_id, offset, data);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to read register version for IMP0: R_OSAL_IoWrite32 returns OSAL_RETURN_FAIL */
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_FAIL, offset, data);
	ercd = R_IMPDRV_RegWrite32(handle, device_id, offset, data);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_HANDLE */
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_RegWrite32(handle, device_id, offset, data);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_STATE */
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_RegWrite32(handle, device_id, offset, data);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_FAIL */
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_RegWrite32(handle, device_id, offset, data);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));
}
#endif

static void Device_reg_write32(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	const st_impdrv_hwrsc_def_t *p_hwrsc_def_table = (const st_impdrv_hwrsc_def_t *)p_impdrv_ctl->common_ctl.p_hwrsc_def_table;
	st_impdrv_reg_info_t reg_info;
	const char *device_id = "imp_spmi_00";
	uintptr_t offset;
	uint32_t  data = 5U;
	int32_t result = 0;

	/* prepare parameters */
	get_correct_hw_info(p_hwrsc_def_table, device_id, &reg_info);
	offset = reg_info.addr_phys - OFFSETADDR_SPMI0;
	
	/* driver_reg_write32 : output parameter */
	ercd = R_IMPDRV_RegWrite32(handle, device_id, offset, data);
	if (ercd != IMPDRV_EC_OK)
	{
		testmsg("(No.%d) fail(test NG %d)\n", testno++, ercd);
		result = -1;
	}
	else
	{
		testmsg("(No.%d) Success(test OK)\n", testno++);
	}

	CountResult(result == 0, tret);
}