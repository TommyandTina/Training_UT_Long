
#define TEST_MSG_NAME "R_IMPDRV_Quit"

#include "it_common.h"
#include "it_stub.h"

#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret);
static void Return_error(struct test_params_t *tparam, struct test_result_t *tret);
#endif
static void Device_quit(struct test_params_t *tparam, struct test_result_t *tret);

static uint32_t testno;

int32_t it_Quit_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

#ifdef IT_ABNORMAL
	testno = 1;
	Parameter_error(tparam, &tret);
	Return_error(tparam, &tret);
#endif
	Device_quit(tparam, &tret);

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

static void Return_error(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;

	/* Fail to lock mutex impdrv_cmnctl_mutex_lock returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_cmnctl_mutex_lock(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_cmnctl_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to lock mutex impdrv_cmnctl_mutex_lock returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_cmnctl_mutex_lock(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to lock mutex impdrv_cmnctl_mutex_lock returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_cmnctl_mutex_lock(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* impdrv_impctl_check_state returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_impctl_check_state(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_impctl_check_state returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* impdrv_ocvctl_check_state returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_ocvctl_check_state(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_ocvctl_check_state returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* impdrv_dmactl_check_state returns IMPDRV_EC_NG_PARAM */
	it_stub_set_impdrv_dmactl_check_state(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_dmactl_check_state returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* impdrv_pscctl_check_state returns IMPDRV_EC_NG_NOTSUPPORT */
	it_stub_set_impdrv_pscctl_check_state(IMPDRV_EC_NG_NOTSUPPORT);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) impdrv_pscctl_check_state returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_NOTSUPPORT));

	/* impdrv_cmnctl_reg_quit returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_cmnctl_reg_quit(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_reg_quit returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* impdrv_cmnctl_quit returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_cmnctl_quit(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_quit returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* impdrv_cmnctl_quit returns IMPDRV_EC_NG_PARAM */
	it_stub_set_impdrv_cmnctl_quit(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_quit returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* impdrv_cmnctl_quit returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_cmnctl_quit(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_quit returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));
	
	/* impdrv_cmnctl_quit returns IMPDRV_EC_NG_PMSYSTEMERROR */
	it_stub_set_impdrv_cmnctl_quit(IMPDRV_EC_NG_PMSYSTEMERROR);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_quit returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PMSYSTEMERROR));

	/* impdrv_cmnctl_mutex_unlock returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_cmnctl_mutex_unlock(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_cmnctl_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* impdrv_cmnctl_mutex_unlock returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_cmnctl_mutex_unlock(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* impdrv_cmnctl_mutex_unlock returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_cmnctl_mutex_unlock(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* impdrv_cmnctl_mutex_destroy returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_cmnctl_mutex_destroy(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_destroy returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* impdrv_cmnctl_mutex_destroy returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_cmnctl_mutex_destroy(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_destroy returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));
}
#endif

static void Device_quit(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
    impdrv_ctrl_handle_t handle;
	int32_t result = -1;
	
	/* driver_quit : output parameter */
	handle = (impdrv_ctrl_handle_t)tparam->initdata.p_work_addr;
	it_stub_reset();
	ercd = R_IMPDRV_Quit(handle);
	if (ercd != IMPDRV_EC_OK) {
		testmsg("(No.%d) fail(test NG %d)\n", testno++, ercd);
	} else {
		testmsg("(No.%d) Success(test OK)\n", testno++);
		result = 0;
	}

	CountResult(result == 0, tret);

	/* driver_quit : impdrv_pscctl_is_valid_core returns FALSE */
	result = -1;
	handle = (impdrv_ctrl_handle_t)tparam->initdata.p_work_addr;
	it_stub_set_impdrv_pscctl_is_valid_core(false);
	is_valid_core_flag = false;
	ercd = R_IMPDRV_Quit(handle);
	if (ercd != IMPDRV_EC_OK) {
		testmsg("(No.%d) fail(test NG %d)\n", testno++, ercd);
	} else {
		testmsg("(No.%d) Success(test OK)\n", testno++);
		result = 0;
	}

	CountResult(result == 0, tret);
}

