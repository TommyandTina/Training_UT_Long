#define TEST_MSG_NAME "R_IMPDRV_RegRequired"

#include "it_common.h"
#include "it_stub.h"

#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret);
static void Return_error(struct test_params_t *tparam, struct test_result_t *tret);
#endif
static void Device_reg_required(struct test_params_t *tparam, struct test_result_t *tret);

static uint32_t testno;

int32_t it_RegRequired_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

	/* Execute test */
#ifdef IT_ABNORMAL
	testno = 1;
	Parameter_error(tparam, &tret);
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
	it_stub_set_impdrv_impctl_check_state(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_RegRequired(NULL, device_id, new_state);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) handle = NULL", testno++);

	/* Parameter_error : p_device_id = NULL */
	it_stub_set_impdrv_impctl_check_state(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_RegRequired(handle, NULL, new_state);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) p_device_id = NULL", testno++);

	/* Parameter_error : device_id[0] = '\0' */
	it_stub_set_impdrv_impctl_check_state(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_RegRequired(handle, "\0", new_state);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) device_id[0] = \'\\0\'", testno++);

	/* Parameter_error : new_state is IMPDRV_REG_REQ_STATE_INVALID */
	it_stub_set_impdrv_impctl_check_state(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_RegRequired(handle, device_id, IMPDRV_REG_REQ_STATE_INVALID);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) new_state = IMPDRV_REG_REQ_STATE_INVALID", testno++);

	/* Parameter_error : new_state is 3 */
	it_stub_set_impdrv_impctl_check_state(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_RegRequired(handle, device_id, 3U);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) new_state = 3", testno++);
}

static void Return_error(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	const char *device_id = "imp_spmi_00";
	e_impdrv_reg_req_state_t  new_state = IMPDRV_REG_REQ_STATE_REQUIRED;

	/* Fail to lock mutex impdrv_cmnctl_mutex_lock returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_impctl_check_state(IMPDRV_EC_NG_SEQSTATE);
	it_stub_set_impdrv_cmnctl_mutex_lock(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_RegRequired(handle, device_id, new_state);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_cmnctl_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to lock mutex impdrv_cmnctl_mutex_lock returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_impctl_check_state(IMPDRV_EC_NG_SEQSTATE);
	it_stub_set_impdrv_cmnctl_mutex_lock(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_RegRequired(handle, device_id, new_state);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to lock mutex impdrv_cmnctl_mutex_lock returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_impctl_check_state(IMPDRV_EC_NG_SEQSTATE);
	it_stub_set_impdrv_cmnctl_mutex_lock(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_RegRequired(handle, device_id, new_state);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to check state for IMP0 impdrv_impctl_check_state returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_impctl_check_state(IMPDRV_EC_OK);
	ercd = R_IMPDRV_RegRequired(handle, device_id, new_state);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_impctl_check_state returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_OK));

	/* Fail to set memory protection mode impdrv_cmnctl_reg_required returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_impctl_check_state(IMPDRV_EC_NG_SEQSTATE);
	it_stub_set_impdrv_cmnctl_reg_required(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_RegRequired(handle, device_id, new_state);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_cmnctl_reg_required returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to set memory protection mode impdrv_cmnctl_reg_required returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_impctl_check_state(IMPDRV_EC_NG_SEQSTATE);
	it_stub_set_impdrv_cmnctl_reg_required(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_RegRequired(handle, device_id, new_state);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_reg_required returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to set memory protection mode impdrv_cmnctl_reg_required returns IMPDRV_EC_NG_PARAM */
	it_stub_set_impdrv_impctl_check_state(IMPDRV_EC_NG_SEQSTATE);
	it_stub_set_impdrv_cmnctl_reg_required(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_RegRequired(handle, device_id, new_state);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_reg_required returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to set memory protection mode impdrv_cmnctl_reg_required returns IMPDRV_EC_NG_NOTSUPPORT */
	it_stub_set_impdrv_impctl_check_state(IMPDRV_EC_NG_SEQSTATE);
	it_stub_set_impdrv_cmnctl_reg_required(IMPDRV_EC_NG_NOTSUPPORT);
	ercd = R_IMPDRV_RegRequired(handle, device_id, new_state);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) impdrv_cmnctl_reg_required returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_NOTSUPPORT));

	/* Fail to set memory protection mode impdrv_cmnctl_reg_required returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_impctl_check_state(IMPDRV_EC_NG_SEQSTATE);
	it_stub_set_impdrv_cmnctl_reg_required(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_RegRequired(handle, device_id, new_state);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_reg_required returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to set memory protection mode impdrv_cmnctl_reg_required returns IMPDRV_EC_NG_PMSYSTEMERROR */
	it_stub_set_impdrv_impctl_check_state(IMPDRV_EC_NG_SEQSTATE);
	it_stub_set_impdrv_cmnctl_reg_required(IMPDRV_EC_NG_PMSYSTEMERROR);
	ercd = R_IMPDRV_RegRequired(handle, device_id, new_state);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_reg_required returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PMSYSTEMERROR));

	/* Fail to unlock mutex impdrv_cmnctl_mutex_unlock returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_impctl_check_state(IMPDRV_EC_NG_SEQSTATE);
	it_stub_set_impdrv_cmnctl_mutex_unlock(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_RegRequired(handle, device_id, new_state);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_cmnctl_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to unlock mutex impdrv_cmnctl_mutex_unlock returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_impctl_check_state(IMPDRV_EC_NG_SEQSTATE);
	it_stub_set_impdrv_cmnctl_mutex_unlock(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_RegRequired(handle, device_id, new_state);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to unlock mutex impdrv_cmnctl_mutex_unlock returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_impctl_check_state(IMPDRV_EC_NG_SEQSTATE);
	it_stub_set_impdrv_cmnctl_mutex_unlock(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_RegRequired(handle, device_id, new_state);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));
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
	it_stub_reset();
	it_stub_set_impdrv_impctl_check_state(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_RegRequired(handle, device_id, new_state);
	if (ercd != IMPDRV_EC_OK) {
		testmsg("(No.%d) fail(test NG %d)\n", testno++, ercd);
		result = -1;
	} else {
		testmsg("(No.%d) Success(test OK)\n", testno++);
	}

	CountResult(result == 0, tret);
}

