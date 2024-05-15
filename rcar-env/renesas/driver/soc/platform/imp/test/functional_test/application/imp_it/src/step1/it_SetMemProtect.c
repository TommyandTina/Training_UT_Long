#define TEST_MSG_NAME "R_IMPDRV_SetMemProtect"

#include "it_common.h"
#include "it_stub.h"

#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret);
static void Return_error(struct test_params_t *tparam, struct test_result_t *tret);
#endif
static void Device_set_mem_protect(struct test_params_t *tparam, struct test_result_t *tret);

static uint32_t testno;

int32_t it_SetMemProtect_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
#if defined (V3H2)
	struct test_result_t tret = {0};

	/* Execute test */
#ifdef IT_ABNORMAL
	testno = 1;
	Parameter_error(tparam, &tret);
	Return_error(tparam, &tret);
#endif
	Device_set_mem_protect(tparam, &tret);

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

static void Return_error(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	e_impdrv_protect_mode_t   protect_mode = IMPDRV_PROTECT_EDC;

	/* Fail to lock mutex impdrv_cmnctl_mutex_lock returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_cmnctl_mutex_lock(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_cmnctl_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to lock mutex impdrv_cmnctl_mutex_lock returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_cmnctl_mutex_lock(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to lock mutex impdrv_cmnctl_mutex_lock returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_cmnctl_mutex_lock(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to check state for IMP0 impdrv_impctl_check_state returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_impctl_check_state(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_impctl_check_state returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to set memory protection mode impdrv_cmnctl_reg_set_mem_protect returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_cmnctl_reg_set_mem_protect(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_cmnctl_reg_set_mem_protect returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to set memory protection mode impdrv_cmnctl_reg_set_mem_protect returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_cmnctl_reg_set_mem_protect(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_reg_set_mem_protect returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to set memory protection mode impdrv_cmnctl_reg_set_mem_protect returns IMPDRV_EC_NG_PARAM */
	it_stub_set_impdrv_cmnctl_reg_set_mem_protect(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_reg_set_mem_protect returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to set memory protection mode impdrv_cmnctl_reg_set_mem_protect returns IMPDRV_EC_NG_CHECKFAIL */
	it_stub_set_impdrv_cmnctl_reg_set_mem_protect(IMPDRV_EC_NG_CHECKFAIL);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_CHECKFAIL, *tret, "(No.%d) impdrv_cmnctl_reg_set_mem_protect returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_CHECKFAIL));

	/* Fail to set memory protection mode impdrv_cmnctl_reg_set_mem_protect returns IMPDRV_EC_NG_NOTSUPPORT */
	it_stub_set_impdrv_cmnctl_reg_set_mem_protect(IMPDRV_EC_NG_NOTSUPPORT);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) impdrv_cmnctl_reg_set_mem_protect returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_NOTSUPPORT));

	/* Fail to set memory protection mode impdrv_cmnctl_reg_set_mem_protect returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_cmnctl_reg_set_mem_protect(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_reg_set_mem_protect returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to unlock mutex impdrv_cmnctl_mutex_unlock returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_cmnctl_mutex_unlock(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
#if defined (V3H2)
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_cmnctl_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));
#else
	FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) impdrv_cmnctl_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));
#endif

	/* Fail to unlock mutex impdrv_cmnctl_mutex_unlock returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_cmnctl_mutex_unlock(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
#if defined (V3H2)
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));
#else
	FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) impdrv_cmnctl_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));
#endif

	/* Fail to unlock mutex impdrv_cmnctl_mutex_unlock returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_cmnctl_mutex_unlock(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	it_stub_reset();
#if defined (V3H2)
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));
#else
	FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) impdrv_cmnctl_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));
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
	it_stub_reset();
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

