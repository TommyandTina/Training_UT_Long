
#define TEST_MSG_NAME "R_IMPDRV_Init"

#include "it_common.h"
#include "it_stub.h"

#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret);
static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret);
static void Return_error(struct test_params_t *tparam, struct test_result_t *tret);
#endif
static void Device_init_no_dma(struct test_params_t *tparam, struct test_result_t *tret);
static void Device_init(struct test_params_t *tparam, struct test_result_t *tret);

static uint32_t testno;

int32_t it_Init_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

	/* Execute test */
#ifdef IT_ABNORMAL
	testno = 1;
	Parameter_error(tparam, &tret);
	Invalid_core(tparam, &tret);
	No_exec_core(tparam, &tret);
	Return_error(tparam, &tret);
#endif
	Device_init_no_dma(tparam, &tret);
	Device_init(tparam, &tret);

	PrintTestResult(tret, test_result);

	return 0;
}

#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
    impdrv_ctrl_handle_t handle;

	/* Parameter_error : p_initdata == NULL */
	ercd = R_IMPDRV_Init(NULL, &handle);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) p_initdata == NULL", testno++);

	/* Parameter_error : p_handle == NULL */
	ercd = R_IMPDRV_Init(p_initdata, NULL);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) p_handle == NULL", testno++);

	/* Parameter_error : p_initdata->p_work_addr = NULL */
	set_input_data_before_init(tparam);
	p_initdata->p_work_addr = NULL;
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) p_initdata->p_work_addr = NULL", testno++);

	/* Parameter_error : p_initdata->p_work_addr = 7U */
	set_input_data_before_init(tparam);
	p_initdata->p_work_addr = (void *)7U;
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) p_initdata->p_work_addr = 7U", testno++);

	/* Parameter_error : p_initdata->p_work_addr = 9U */
	set_input_data_before_init(tparam);
	p_initdata->p_work_addr = (void *)9U;
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) p_initdata->p_work_addr = 9U", testno++);

	/* Parameter_error : p_initdata->p_work_addr = 4U */
	set_input_data_before_init(tparam);
	p_initdata->p_work_addr = (void *)4U;
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) p_initdata->p_work_addr = 4U", testno++);

	/* Parameter_error : p_initdata->callback_func_fatal = NULL */
	set_input_data_before_init(tparam);
	p_initdata->callback_func_fatal = NULL;
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) p_initdata->callback_func_fatal = NULL", testno++);

	/* Parameter_error : p_initdata.use_core_num = 0 */
	set_input_data_before_init(tparam);
	p_initdata->use_core_num = 0U;
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) p_initdata.use_core_num = 0", testno++);

	/* Parameter_error : p_initdata.use_core_num = 22 (V3H2) */
	set_input_data_before_init(tparam);
	p_initdata->use_core_num = 22U;
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) p_initdata.use_core_num = 22 (V3H2)", testno++);

	/* Parameter_error : p_initdata->osal_resource.mutex_time_period = -1 */
	set_input_data_before_init(tparam);
	p_initdata->osal_resource.mutex_time_period = -1;
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) p_initdata->osal_resource.mutex_time_period = -1", testno++);

	/* Parameter_error : p_initdata->osal_resource.mq_time_period = -1 */
	set_input_data_before_init(tparam);
	p_initdata->osal_resource.mq_time_period = -1;
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) p_initdata->osal_resource.mq_time_period = -1", testno++);

	/* Parameter_error : p_initdata->osal_resource.irq_priority = 15 */
	set_input_data_before_init(tparam);
	p_initdata->osal_resource.irq_priority = 15U;
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) p_initdata->osal_resource.irq_priority = 15", testno++);

	/* Parameter_error : p_initdata->instance_num = 8 */
	set_input_data_before_init(tparam);
	p_initdata->instance_num = 8;
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) p_initdata->instance_num = 8", testno++);
}

static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
    impdrv_ctrl_handle_t handle;

	/* Invalid_error : Core_type = IMPDRV_CORE_TYPE_INVALID */
	set_input_data_before_init(tparam);
	p_initdata->core_info[0].core_type = IMPDRV_CORE_TYPE_INVALID;
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) Core_type = IMPDRV_CORE_TYPE_INVALID", testno++);

	/* Invalid_error : Duplicate core type and core num */
	set_input_data_before_init(tparam);
	p_initdata->core_info[0].core_type = IMPDRV_CORE_TYPE_IMP;
	p_initdata->core_info[0].core_num = 0;
	p_initdata->core_info[1].core_type = IMPDRV_CORE_TYPE_IMP;
	p_initdata->core_info[1].core_num = 0;
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) Duplicate core type and core num", testno++);

	/* Invalid_error : Core_num = IMPDRV_CORE_NUM_MAX */
	set_input_data_before_init(tparam);
	p_initdata->core_info[0].core_num = IMPDRV_CORE_NUM_MAX;
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) Core_num = IMPDRV_CORE_NUM_MAX", testno++);
}

static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
    impdrv_ctrl_handle_t handle;
	tp_core_list_t no_exec_cores = get_core_list(IT_IMPDRV_CMD_NO_EXEC, 0);

	/* Not supported core: Core not supported */
	for (uint32_t i = 0; i < no_exec_cores.num; i++)
	{
		set_input_data_before_init(tparam);
		p_initdata->core_info[0] = *(no_exec_cores.core_info + i);
		ercd = R_IMPDRV_Init(p_initdata, &handle);
		FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) Core not supported: %s", testno++, type_to_name(p_initdata->core_info[0].core_type));
	}
}

static void Return_error(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
    impdrv_ctrl_handle_t handle;

	/* Fail to create mutex impdrv_cmnctl_get_corectl_func returns NULL */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_cmnctl_get_corectl_func(NULL);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) impdrv_cmnctl_get_corectl_func returns NULL", testno++);

	/* Fail to create mutex impdrv_cmnctl_mutex_create returns IMPDRV_EC_NG_SEQSTATE */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_cmnctl_mutex_create(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_cmnctl_mutex_create returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to create mutex impdrv_cmnctl_mutex_create returns IMPDRV_EC_NG_ARGNULL */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_cmnctl_mutex_create(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_create returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to create mutex impdrv_cmnctl_mutex_create returns IMPDRV_EC_NG_PARAM */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_cmnctl_mutex_create(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_create returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to create mutex impdrv_cmnctl_mutex_create returns IMPDRV_EC_NG_SYSTEMERROR */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_cmnctl_mutex_create(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_create returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to lock mutex impdrv_cmnctl_mutex_lock returns IMPDRV_EC_NG_SEQSTATE */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_cmnctl_mutex_lock(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_cmnctl_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to lock mutex impdrv_cmnctl_mutex_lock returns IMPDRV_EC_NG_ARGNULL */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_cmnctl_mutex_lock(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to lock mutex impdrv_cmnctl_mutex_lock returns IMPDRV_EC_NG_SYSTEMERROR */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_cmnctl_mutex_lock(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* The state is wrong impdrv_impctl_check_state returns IMPDRV_EC_NG_SEQSTATE */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_impctl_check_state(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_impctl_check_state returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* The state is wrong impdrv_impctl_check_state returns IMPDRV_EC_NG_ARGNULL */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_impctl_check_state(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_impctl_check_state returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* The state is wrong impdrv_impctl_check_state returns IMPDRV_EC_NG_PARAM */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_impctl_check_state(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_impctl_check_state returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* The state is wrong impdrv_impctl_check_state returns IMPDRV_EC_NG_NOTSUPPORT */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_impctl_check_state(IMPDRV_EC_NG_NOTSUPPORT);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) impdrv_impctl_check_state returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_NOTSUPPORT));

	/* Fail to initialize for osdep impdrv_cmnctl_init returns IMPDRV_EC_NG_ARGNULL */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_cmnctl_init(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_init returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to initialize for osdep impdrv_cmnctl_init returns IMPDRV_EC_NG_PARAM */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_cmnctl_init(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_init returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to initialize for osdep impdrv_cmnctl_init returns IMPDRV_EC_NG_SYSTEMERROR */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_cmnctl_init(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_init returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));
	
	/* Fail to initialize for osdep impdrv_cmnctl_init returns IMPDRV_EC_NG_PMSYSTEMERROR */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_cmnctl_init(IMPDRV_EC_NG_PMSYSTEMERROR);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_init returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PMSYSTEMERROR));

	/* Fail to initialize for osdep impdrv_cmnctl_init returns IMPDRV_EC_NG_INSTANCE */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_cmnctl_init(IMPDRV_EC_NG_INSTANCE);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_INSTANCE, *tret, "(No.%d) impdrv_cmnctl_init returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_INSTANCE));

	/* Fail to initialize for osdep impdrv_cmnctl_reg_init returns IMPDRV_EC_NG_ARGNULL */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_cmnctl_reg_init(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_reg_init returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

#if !defined(RUN_SIL)
	/* Fail to initialize for osdep impdrv_cmnctl_reg_mem_init returns IMPDRV_EC_NG_SEQSTATE */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_cmnctl_reg_mem_init(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_cmnctl_reg_mem_init returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to initialize for osdep impdrv_cmnctl_reg_mem_init returns IMPDRV_EC_NG_ARGNULL */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_cmnctl_reg_mem_init(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_reg_mem_init returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to initialize for osdep impdrv_cmnctl_reg_mem_init returns IMPDRV_EC_NG_PARAM */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_cmnctl_reg_mem_init(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_reg_mem_init returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to initialize for osdep impdrv_cmnctl_reg_mem_init returns IMPDRV_EC_NG_SYSTEMERROR */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_cmnctl_reg_mem_init(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_reg_mem_init returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));
#endif

	/* Fail to start init ocv core impdrv_ocvctl_init_start returns IMPDRV_EC_NG_PARAM */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_ocvctl_init_start(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_ocvctl_init_start returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to start init psc core impdrv_pscctl_init_start returns IMPDRV_EC_NG_ARGNULL */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_pscctl_init_start(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_pscctl_init_start returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* OSAL fail in init imp core impdrv_impctl_init_start returns IMPDRV_EC_NG_SYSTEMERROR */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_impctl_init_start(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_impctl_init_start returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to turn on power imp module in start init ocv core impdrv_***ctl_init_start returns IMPDRV_EC_NG_PMSYSTEMERROR */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_ocvctl_init_start(IMPDRV_EC_NG_PMSYSTEMERROR);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) impdrv_ocvctl_init_start returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PMSYSTEMERROR));

#if !defined(RUN_SIL)
	/* Fail to init DMA multi-bank impdrv_dmactl_mb_init returns IMPDRV_EC_NG_SEQSTATE */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_dmactl_mb_init(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) it_stub_set_impdrv_dmactl_mb_init returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to init DMA multi-bank impdrv_dmactl_mb_init returns IMPDRV_EC_NG_ARGNULL */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_dmactl_mb_init(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) it_stub_set_impdrv_dmactl_mb_init returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to init DMA multi-bank impdrv_dmactl_mb_init returns IMPDRV_EC_NG_PARAM */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_dmactl_mb_init(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) it_stub_set_impdrv_dmactl_mb_init returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to init DMA multi-bank impdrv_dmactl_mb_init returns IMPDRV_EC_NG_SYSTEMERROR */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_dmactl_mb_init(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) it_stub_set_impdrv_dmactl_mb_init returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));
#endif

	/* Fail to close device of imp module impdrv_***ctl_init_end returns IMPDRV_EC_NG_ARGNULL */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_pscctl_init_end(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_pscctl_init_end returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to close device of imp module impdrv_***ctl_init_end returns IMPDRV_EC_NG_PARAM */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_pscctl_init_end(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_pscctl_init_end returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to close device of imp module impdrv_***ctl_init_end returns IMPDRV_EC_NG_SYSTEMERROR */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_pscctl_init_end(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_pscctl_init_end returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to turn off power of imp module impdrv_***ctl_init_end returns IMPDRV_EC_NG_PMSYSTEMERROR */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_impctl_init_end(IMPDRV_EC_NG_PMSYSTEMERROR);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) impdrv_impctl_init_end returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PMSYSTEMERROR));

	/* Fail to unlock mutex impdrv_cmnctl_mutex_unlock returns IMPDRV_EC_NG_SEQSTATE */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_cmnctl_mutex_unlock(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_cmnctl_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to unlock mutex impdrv_cmnctl_mutex_unlock returns IMPDRV_EC_NG_ARGNULL */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_cmnctl_mutex_unlock(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to unlock mutex impdrv_cmnctl_mutex_unlock returns IMPDRV_EC_NG_SYSTEMERROR */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_cmnctl_mutex_unlock(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));
	
	/* Fail to unlock mutex impdrv_ocvctl_set_state(INIT) returns IMPDRV_EC_NG_ARGNULL */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_ocvctl_set_state(IMPDRV_EC_NG_ARGNULL, 0);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_ocvctl_set_state returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to unlock mutex impdrv_ocvctl_set_state(INIT) returns IMPDRV_EC_NG_PARAM */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_ocvctl_set_state(IMPDRV_EC_NG_PARAM, 0);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_ocvctl_set_state returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to unlock mutex impdrv_pscctl_attr_init returns IMPDRV_EC_NG_PARAM */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_pscctl_attr_init(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_pscctl_attr_init returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to unlock mutex impdrv_pscctl_attr_init returns IMPDRV_EC_NG_SEQSTATE */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_pscctl_attr_init(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_pscctl_attr_init returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to unlock mutex impdrv_pscctl_attr_init returns IMPDRV_EC_NG_ARGNULL */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_pscctl_attr_init(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_pscctl_attr_init returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to unlock mutex impdrv_pscctl_attr_init returns IMPDRV_EC_NG_NOTSUPPORT */
	set_input_data_before_init(tparam);
	it_stub_set_impdrv_pscctl_attr_init(IMPDRV_EC_NG_NOTSUPPORT);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) impdrv_pscctl_attr_init returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_NOTSUPPORT));
}
#endif
static void Device_init_no_dma(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
    st_impdrv_initdata_t *p_initdata = &tparam->initdata;
    impdrv_ctrl_handle_t handle;
	st_impdrv_ctl_t *p_impdrv_ctl;
	st_impdrv_coretypectl_t	*coretype_ctl;
	int32_t result = -1;
	tparam->check_no_dmac_flag = true;
	/* driver_init : output parameter */
	it_stub_reset();
	set_input_data_before_init(tparam);

	p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
	coretype_ctl = p_impdrv_ctl->coretype_ctl;
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	if (ercd != IMPDRV_EC_OK) {
		testmsg("(No.%d) fail(test NG %d)\n", testno++, ercd);
	} else if ((handle == NULL) || (handle != p_initdata->p_work_addr)) {
		testmsg("(No.%d) Parameter fail(test NG)\n", testno++);
	} else if (check_state(p_initdata, coretype_ctl, IMPDRV_STATE_INIT, p_initdata->use_core_num)) {
		testmsg("(No.%d) States fail(test NG)\n", testno++);
	} else {
		testmsg("(No.%d) Success(test OK)\n", testno++);
		result = 0;
	}
	// R_IMPDRV_Quit(&handle);
	CountResult(result == 0, tret);
}

static void Device_init(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
    st_impdrv_initdata_t *p_initdata = &tparam->initdata;
    impdrv_ctrl_handle_t handle;
	st_impdrv_ctl_t *p_impdrv_ctl;
	st_impdrv_coretypectl_t	*coretype_ctl;
	int32_t result = -1;
	tparam->check_no_dmac_flag = false;
	/* driver_init : output parameter */
	it_stub_reset();
	set_input_data_before_init(tparam);

	p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
	coretype_ctl = p_impdrv_ctl->coretype_ctl;
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	if (ercd != IMPDRV_EC_OK) {
		testmsg("(No.%d) fail(test NG %d)\n", testno++, ercd);
	} else if ((handle == NULL) || (handle != p_initdata->p_work_addr)) {
		testmsg("(No.%d) Parameter fail(test NG)\n", testno++);
	} else if (check_state(p_initdata, coretype_ctl, IMPDRV_STATE_INIT, p_initdata->use_core_num)) {
		testmsg("(No.%d) States fail(test NG)\n", testno++);
	} else {
		testmsg("(No.%d) Success(test OK)\n", testno++);
		result = 0;
	}

	CountResult(result == 0, tret);
}

