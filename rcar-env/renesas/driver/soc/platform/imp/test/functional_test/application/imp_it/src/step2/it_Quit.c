
#define TEST_MSG_NAME "R_IMPDRV_Quit"

#include "it_common.h"
#include "it_stub.h"

static void set_state(st_impdrv_initdata_t *const p_data, e_impdrv_state_t state);
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
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_data->p_work_addr;

	for (uint32_t cnt = 0U; cnt < p_data->use_core_num; cnt++)
	{
		st_impdrv_core_info_t    *core_info = &p_data->core_info[cnt];
		e_impdrv_core_type_t core_type = core_info->core_type;
		uint32_t core_num = core_info->core_num;

		p_impdrv_ctl->coretype_ctl[core_type].core_ctl[core_num].core_state = state;
	}
}

int32_t it_Quit_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

#ifdef IT_ABNORMAL
	testno = 21;
	Parameter_error(tparam, &tret);
	Invalid_state(tparam, &tret);
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

	/* Parameter_error : handle == NULL */
	ercd = R_IMPDRV_Quit(NULL);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) handle == NULL", testno++);
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

	/* Fail to lock mutex impdrv_osdep_mutex_lock returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_osdep_mutex_lock(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to lock mutex impdrv_osdep_mutex_lock returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_osdep_mutex_lock(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to lock mutex impdrv_osdep_mutex_lock returns IMPDRV_EC_NG_PARAM */
	it_stub_set_impdrv_osdep_mutex_lock(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to lock mutex impdrv_osdep_mutex_lock returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_osdep_mutex_lock(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to lock mutex impdrv_osdep_pow_off_hwrsc returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_osdep_pow_off_hwrsc(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_pow_off_hwrsc returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to lock mutex impdrv_osdep_pow_off_hwrsc returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_osdep_pow_off_hwrsc(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_pow_off_hwrsc returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to lock mutex impdrv_osdep_pow_off_hwrsc returns IMPDRV_EC_NG_PMSYSTEMERROR */
	it_stub_set_impdrv_osdep_pow_off_hwrsc(IMPDRV_EC_NG_PMSYSTEMERROR);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) impdrv_osdep_pow_off_hwrsc returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PMSYSTEMERROR));

	/* impdrv_osdep_write_reg returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_SEQSTATE, RCVDRV_IMP_OFFSET_ALL);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_write_reg returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* impdrv_osdep_write_reg returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_ARGNULL, RCVDRV_IMP_OFFSET_ALL);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_write_reg returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* impdrv_osdep_write_reg returns IMPDRV_EC_NG_PARAM */
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_PARAM, RCVDRV_IMP_OFFSET_ALL);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_write_reg returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* impdrv_osdep_write_reg returns IMPDRV_EC_NG_PARAM */
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_CHECKFAIL, RCVDRV_IMP_OFFSET_ALL);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_CHECKFAIL, *tret, "(No.%d) impdrv_osdep_write_reg returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_CHECKFAIL));

	/* impdrv_osdep_write_reg returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_SYSTEMERROR, RCVDRV_IMP_OFFSET_ALL);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_write_reg returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* impdrv_osdep_disable_irq returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_osdep_disable_irq(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_disable_irq returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* impdrv_osdep_disable_irq returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_osdep_disable_irq(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_disable_irq returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* impdrv_osdep_disable_irq returns IMPDRV_EC_NG_PARAM */
	it_stub_set_impdrv_osdep_disable_irq(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_disable_irq returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* impdrv_osdep_disable_irq returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_osdep_disable_irq(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_disable_irq returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* impdrv_osdep_unregister_irq returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_osdep_unregister_irq(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_unregister_irq returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* impdrv_osdep_unregister_irq returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_osdep_unregister_irq(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_unregister_irq returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* impdrv_osdep_unregister_irq returns IMPDRV_EC_NG_PARAM */
	it_stub_set_impdrv_osdep_unregister_irq(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_unregister_irq returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* impdrv_osdep_unregister_irq returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_osdep_unregister_irq(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_unregister_irq returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* impdrv_osdep_pow_off_imp_top returns IMPDRV_EC_NG_SEQSTATE */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	close_device_io_flg = true;
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	it_stub_set_impdrv_osdep_pow_off_imp_top(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	close_device_io_flg = false;
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_pow_off_imp_top returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* impdrv_osdep_pow_off_imp_top returns IMPDRV_EC_NG_ARGNULL */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	close_device_io_flg = true;
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	it_stub_set_impdrv_osdep_pow_off_imp_top(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	close_device_io_flg = false;
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_pow_off_imp_top returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* impdrv_osdep_pow_off_imp_top returns IMPDRV_EC_NG_SYSTEMERROR */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	close_device_io_flg = true;
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	it_stub_set_impdrv_osdep_pow_off_imp_top(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	close_device_io_flg = false;
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_pow_off_imp_top returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* impdrv_osdep_pow_off_imp_top returns IMPDRV_EC_NG_PMSYSTEMERROR */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	close_device_io_flg = true;
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	it_stub_set_impdrv_osdep_pow_off_imp_top(IMPDRV_EC_NG_PMSYSTEMERROR);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	close_device_io_flg = false;
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) impdrv_osdep_pow_off_imp_top returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PMSYSTEMERROR));

	/* Fail to unlock mutex impdrv_osdep_mutex_unlock returns IMPDRV_EC_NG_SEQSTATE */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	close_device_io_flg = true;
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	it_stub_set_impdrv_osdep_mutex_unlock(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	close_device_io_flg = false;
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to unlock mutex impdrv_osdep_mutex_unlock returns IMPDRV_EC_NG_ARGNULL */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	it_stub_set_impdrv_osdep_mutex_unlock(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to unlock mutex impdrv_osdep_mutex_unlock returns IMPDRV_EC_NG_SYSTEMERROR */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	it_stub_set_impdrv_osdep_mutex_unlock(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to unlock mutex impdrv_osdep_mutex_destroy returns IMPDRV_EC_NG_SEQSTATE */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	it_stub_set_impdrv_osdep_mutex_destroy(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_mutex_destroy returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to unlock mutex impdrv_osdep_mutex_destroy returns IMPDRV_EC_NG_ARGNULL */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	it_stub_set_impdrv_osdep_mutex_destroy(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_mutex_destroy returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to unlock mutex impdrv_osdep_mutex_destroy returns IMPDRV_EC_NG_SYSTEMERROR */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	(void)R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	it_stub_set_impdrv_osdep_mutex_destroy(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_Quit(handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_mutex_destroy returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));
	
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
	ercd = R_IMPDRV_Quit(handle);
	if (ercd != IMPDRV_EC_OK) {
		testmsg("(No.%d) fail(test NG %d)\n", testno++, ercd);
	} else {
		testmsg("(No.%d) Success(test OK)\n", testno++);
		result = 0;
	}

	CountResult(result == 0, tret);
}