#define TEST_MSG_NAME "R_IMPDRV_SetMemProtect"

#include "it_common.h"
#include "it_stub.h"

static void set_state(st_impdrv_initdata_t *const p_data, e_impdrv_state_t state);
#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret);
static void UNINIT_state(struct test_params_t *tparam, struct test_result_t *tret);
static void Return_error(struct test_params_t *tparam, struct test_result_t *tret);
#endif
static void Device_set_mem_protect(struct test_params_t *tparam, struct test_result_t *tret);

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

int32_t it_SetMemProtect_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

	/* Execute test */
#ifdef IT_ABNORMAL
	testno = 34;
	Parameter_error(tparam, &tret);
	testno = 38; // Avoid deplicated with it_SetMemProtect_UNINIT_Main
	Invalid_state(tparam, &tret);
	Return_error(tparam, &tret);
#endif
	Device_set_mem_protect(tparam, &tret);

	PrintTestResult(tret, test_result);

	return 0;
}

int32_t it_SetMemProtect_UNINIT_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

	/* Execute test */
#ifdef IT_ABNORMAL
	testno = 37;
	UNINIT_state(tparam, &tret);
	PrintTestResult(tret, test_result);
#endif

	return 0;
}

#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	e_impdrv_protect_mode_t   protect_mode = IMPDRV_PROTECT_EDC;

	/* Parameter_error : handle == NULL */
	ercd = R_IMPDRV_SetMemProtect(NULL, protect_mode);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) handle = NULL", testno++);

	/* Parameter_error : protect_mode = IMPDRV_PROTECT_INVALID */
	protect_mode = IMPDRV_PROTECT_INVALID;
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) protect_mode = IMPDRV_PROTECT_INVALID", testno++);

	/* Parameter_error : protect_mode = 3 */
	protect_mode = 3;
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) protect_mode = 3", testno++);
}

static void UNINIT_state(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	e_impdrv_protect_mode_t   protect_mode = IMPDRV_PROTECT_EDC;

	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) %s", testno++, state_to_name(p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].core_state));
}

static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
	st_impdrv_core_info_t *p_core_info = p_initdata->core_info;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	e_impdrv_protect_mode_t   protect_mode = IMPDRV_PROTECT_EDC;
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
		ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
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
	e_impdrv_protect_mode_t   protect_mode = IMPDRV_PROTECT_EDC;

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_TIME */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_TIME);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_TIME));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_PAR */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_PAR);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_BUSY */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_BUSY);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_BUSY));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_HANDLE */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_STATE */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to lock mutex R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_FAIL */
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

#if defined (V3H2)
	/* Failed to write register: R_OSAL_IoWrite32 returns OSAL_RETURN_PAR */
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_PAR, RCVDRV_IMP_OFFSET_ALL, 0);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));
#else
	testno++;
#endif

#if defined (V3H2)
	/* Failed to write register: R_OSAL_IoWrite32 returns OSAL_RETURN_DEV */
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_DEV, RCVDRV_IMP_OFFSET_ALL, 0);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));
#else
	testno++;
#endif

#if defined (V3H2)
	/* Failed to write register: R_OSAL_IoWrite32 returns OSAL_RETURN_IO */
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_IO, RCVDRV_IMP_OFFSET_ALL, 0);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 returns %s", testno++, osal_err_to_name(OSAL_RETURN_IO));
#else
	testno++;
#endif

#if defined (V3H2)
	/* Failed to write register: R_OSAL_IoWrite32 returns OSAL_RETURN_HANDLE */
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_HANDLE, RCVDRV_IMP_OFFSET_ALL, 0);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));
#else
	testno++;
#endif

#if defined (V3H2)
	/* Failed to write register: R_OSAL_IoWrite32 returns OSAL_RETURN_STATE */
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_STATE, RCVDRV_IMP_OFFSET_ALL, 0);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));
#else
	testno++;
#endif

#if defined (V3H2)
	/* Failed to write register: R_OSAL_IoWrite32 returns OSAL_RETURN_FAIL */
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_FAIL, RCVDRV_IMP_OFFSET_ALL, 0);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));
#else
	testno++;
#endif

#if defined (V3H2)
	/* Failed to read register: R_OSAL_IoRead32 returns OSAL_RETURN_PAR */
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_PAR, 0, 0);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));
#else
	testno++;
#endif

#if defined (V3H2)
	/* Failed to read register: R_OSAL_IoRead32 returns OSAL_RETURN_DEV */
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_DEV, 0, 0);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));
#else
	testno++;
#endif

#if defined (V3H2)
	/* Failed to read register: R_OSAL_IoRead32 returns OSAL_RETURN_IO */
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_IO, 0, 0);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 returns %s", testno++, osal_err_to_name(OSAL_RETURN_IO));
#else
	testno++;
#endif

#if defined (V3H2)
	/* Failed to read register: R_OSAL_IoRead32 returns OSAL_RETURN_HANDLE */
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_HANDLE, 0, 0);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));
#else
	testno++;
#endif

#if defined (V3H2)
	/* Failed to read register: R_OSAL_IoRead32 returns OSAL_RETURN_STATE */
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_STATE, 0, 0);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));
#else
	testno++;
#endif

#if defined (V3H2)
	/* Failed to read register: R_OSAL_IoRead32 returns OSAL_RETURN_FAIL */
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_FAIL, 0, 0);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));
#else
	testno++;
#endif

#if defined (V3H2)
	/* Readback value is not as expected */
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_OK, RCVDRV_IMP_OFFSET_ALL, 0);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 0, 1);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_CHECKFAIL, *tret, "(No.%d) R_OSAL_IoRead32 read back wrong value, impdrv_osdep_write_reg returns  %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_CHECKFAIL));
#else
	testno++;
#endif

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_HANDLE */
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
#if defined (V3H2)
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));
#else
	FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));
#endif

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_STATE */
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
#if defined (V3H2)
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));
#else
	FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));
#endif

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_FAIL */
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
#if defined (V3H2)
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));
#else
	FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));
#endif
}
#endif

static void Device_set_mem_protect(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	e_impdrv_protect_mode_t   protect_mode = IMPDRV_PROTECT_EDC;
	int32_t result = 0;

	/* driver_set_mem_protect : output parameter */
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
#if defined (V3H2)
	if (IMPDRV_EC_OK != ercd)
	{
		testmsg("(No.%d) fail(test NG %d)\n", testno++, ercd);
		result = -1;
	}
	else
	{
		testmsg("(No.%d) Success(test OK)\n", testno++);
	}
#else
	if (IMPDRV_EC_NG_NOTSUPPORT != ercd)
	{
		testmsg("(No.%d) fail(test NG %d)\n", testno++, ercd);
		result = -1;
	}
	else
	{
		testmsg("(No.%d) Success(test OK)\n", testno++);
	}
#endif

	CountResult(result == 0, tret);
}