#define TEST_MSG_NAME "R_IMPDRV_Init"

#include "it_common.h"
#include "it_stub.h"

static void set_state(st_impdrv_initdata_t *const p_data, e_impdrv_state_t state);
void prepare_test_for_init(st_impdrv_initdata_t *p_cmn_init_data);
#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret);
static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret);
static void Return_error(struct test_params_t *tparam, struct test_result_t *tret);
#endif
static void Device_init(struct test_params_t *tparam, struct test_result_t *tret);

static uint32_t testno;
static uint32_t dmac_init_core_read_reg_index;

static void set_state(st_impdrv_initdata_t *const p_data, e_impdrv_state_t state)
{
	st_impdrv_core_info_t    core_info[IMPDRV_INNER_FIXED_VALUE];
    uint32_t                cnt;
	e_impdrv_core_type_t core_type;
	uint32_t core_num;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_data->p_work_addr;

	cnt = 0U;

	while (cnt < p_data->use_core_num)
	{
		core_info[cnt] = p_data->core_info[cnt];
		core_type = core_info[cnt].core_type;
		core_num = core_info[cnt].core_num;
		p_impdrv_ctl->coretype_ctl[core_type].core_ctl[core_num].core_state = state;
		cnt++;
	}
}

void prepare_test_for_init(st_impdrv_initdata_t *p_cmn_init_data)
{
#if defined (V4H) || defined (V4H2)
	static const uint32_t read_value_for_spminit[] = {
		0x00000000U,	/* Read the INTEN00 register in 'impdrv_cmnctl_init()' */
		0x80000000U,	/* Read the SPMCTRL4 register in 'impdrv_cmnctl_reg_spm_ena_access()' */
		0x80000000U,	/* Read the SPMCTRL4 register in 'impdrv_cmnctl_reg_spm_ena_access()' */
		0x00000001U,	/* Read the Memory status register in 'impdrv_cmnctl_reg_spm_chk_init()' */
		0x00000003U,	/* Read the Memory status register in 'impdrv_cmnctl_reg_spm_chk_init()' */
		0x80000000U,	/* Read the SPMCTRL4 register in 'impdrv_cmnctl_reg_spm_ena_access()' */
		0x80000000U		/* Read the SPMCTRL4 register in 'impdrv_cmnctl_reg_spm_ena_access()' */
	};
	uint32_t read_index;
	uint32_t k = 0U;

#if !defined(RUN_SIL)
	/* Register read values before the Version check function. */
	read_index = 0U;
	while (read_index < (sizeof(read_value_for_spminit) / sizeof(read_value_for_spminit[0])))
	{
		it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_OK, k++, read_value_for_spminit[read_index]);
		read_index++;
	}
#else
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_OK, k++, 0);
#endif

	/* Register read values of the Version check function. */
	read_index = 0U;
	while (read_index < (sizeof(read_value_for_init) / sizeof(read_value_for_init[0])))
	{
		it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_OK, k++, read_value_for_init[read_index]);
		// read value for impdrv_cnnctl_force_reset
		// if (IMPDRV_CNN_VCR == read_value_for_init[read_index])
		// {
		// 	it_stub_set_impdrv_osdep_read_reg(OSAL_RETURN_OK, k++, 0);
		// 	it_stub_set_impdrv_osdep_read_reg(OSAL_RETURN_OK, k++, 1);
		// 	it_stub_set_impdrv_osdep_read_reg(OSAL_RETURN_OK, k++, 0);
		// }
		read_index++;
	}

#elif (defined (V3M2) || defined (V3H1) || defined (V3H2))
	uint32_t i = 0U;
	uint32_t j = 0U;
	uint32_t k = 0U;
	uint32_t loop = 0U;
	uint32_t read_index;

	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_OK, k++, 0);
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_OK, k++, 0);
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_OK, k++, 0);

	read_index = (uint32_t)p_cmn_init_data->core_info[0].core_type;
	while (read_index < (sizeof(read_value_for_init) / sizeof(read_value_for_init[0])))
	{
		if (true == close_device_io_flg)
		{
			/* This case is applied to Init as device_io's failed to be closed recently */
			it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_OK, k++, read_value_for_init[read_index]);
		}
		else
		{
			/* This cases is for normal Initialization: Read version registers of cores
			   before DMA multibank initialization */
			it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_OK, k++, read_value_for_init[read_index]);
			if (IMPDRV_DMA_VCR == read_value_for_init[read_index])
			{
				read_index++;
				break;
			}
		}
		read_index++;
	}

	if (true == close_device_io_flg)
	{
		return;
	}

	/* Wait the register read 8 times as in impdrv_dmactl_init_core() */
	dmac_init_core_read_reg_index = k;
	while (loop < IMPDRV_DMA_LOOPTIME_VAL)
	{
		it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_OK, k, 0);
		k++;
		loop++;
	}

	/* Readback confirm the RAM Test Register(RAMTSTR) as in impdrv_dmactl_init_core() */
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_OK, k, 0);
	k++;

	/* Checking busy as in impdrv_dmactl_cl_pre() for ODD */
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_OK, k, 0);
	k++;

	while (j < IMPDRV_DMA_MB_WRITE_NUM_ODD)
	{
		/* Checking offset IMPDRV_REG_DMA_SR in impdrv_dmactl_mb_init_main() */
		it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_OK, k, IMPDRV_DMA_SR_TEND);
		k++;
		j++;
	}

	/* Checking busy as in impdrv_dmactl_cl_pre() for EVEN */
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_OK, k, 0);
	k++;

	j = 0;
	while (j < IMPDRV_DMA_MB_WRITE_NUM_EVEN)
	{
		/* Checking offset IMPDRV_REG_DMA_SR in impdrv_dmactl_mb_init_main() */
		it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_OK, k, IMPDRV_DMA_SR_TEND);
		k++;
		j++;
	}

	/* Resume with versions of the remaining cores */
	while (read_index < (sizeof(read_value_for_init) / sizeof(read_value_for_init[0])))
	{
		it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_OK, k, read_value_for_init[read_index]);
		k++;
		read_index++;
	}

#else
	#error 'Oops ...! Build option for unsupported products.'
#endif
}

int32_t it_Init_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

#ifdef IT_ABNORMAL
	testno = 66;
	Parameter_error(tparam, &tret);
	testno = 79;
	Invalid_core(tparam, &tret);
	testno = 90;
	No_exec_core(tparam, &tret);
	testno = 94;
	Invalid_state(tparam, &tret);
	testno = 99;
	Return_error(tparam, &tret);
#endif
	testno = 157;
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

		set_input_data_before_init(tparam);
		p_initdata->core_info[0] = *(invalid_core_list.core_info + i);
		ercd = R_IMPDRV_Init(p_initdata, &handle);
		FailTestPrint(ercd, ((p_initdata->core_info[0].core_type == IMPDRV_CORE_TYPE_INVALID) ?
							IMPDRV_EC_NG_PARAM : IMPDRV_EC_NG_NOTSUPPORT),
							*tret, "(No.%d) Invalid core num for %s", testno++, type_to_name(p_initdata->core_info[0].core_type));
	}

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

static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
    impdrv_ctrl_handle_t handle;
	e_impdrv_state_t invalid_state[] = {
		IMPDRV_STATE_INIT,
		IMPDRV_STATE_READY,
		IMPDRV_STATE_EXEC,
		IMPDRV_STATE_INT,
		IMPDRV_STATE_INT + 1
	};

	for (uint32_t i = 0; i < ARRAY_SIZE(invalid_state); i++)
	{
		set_input_data_before_init(tparam);
		set_state(p_initdata, invalid_state[i]);
		ercd = R_IMPDRV_Init(p_initdata, &handle);
		FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) %s", testno++, state_to_name(invalid_state[i]));
	}
}

static void Return_error(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
    impdrv_ctrl_handle_t handle;
	st_impdrv_core_info_t core_info = {IMPDRV_CORE_TYPE_IMP, 0};
	int order_call;

	/* Fail to create mutex impdrv_osdep_mutex_create returns IMPDRV_EC_NG_SEQSTATE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_mutex_create(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_mutex_create returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to create mutex impdrv_osdep_mutex_create returns IMPDRV_EC_NG_ARGNULL */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_mutex_create(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_mutex_create returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to create mutex impdrv_osdep_mutex_create returns IMPDRV_EC_NG_SYSTEMERROR */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_mutex_create(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_mutex_create returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to lock mutex impdrv_osdep_mutex_lock returns IMPDRV_EC_NG_SEQSTATE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_mutex_lock(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to lock mutex impdrv_osdep_mutex_lock returns IMPDRV_EC_NG_ARGNULL */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_mutex_lock(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to lock mutex impdrv_osdep_mutex_lock returns IMPDRV_EC_NG_PARAM */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_mutex_lock(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to lock mutex impdrv_osdep_mutex_lock returns IMPDRV_EC_NG_SYSTEMERROR */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_mutex_lock(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to turn on power impdrv_osdep_pow_on_imp_top returns IMPDRV_EC_NG_SEQSTATE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_pow_on_imp_top(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_pow_on_imp_top returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to turn on power impdrv_osdep_pow_on_imp_top returns IMPDRV_EC_NG_ARGNULL */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_pow_on_imp_top(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_pow_on_imp_top returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to turn on power impdrv_osdep_pow_on_imp_top returns IMPDRV_EC_NG_SYSTEMERROR */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_pow_on_imp_top(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_pow_on_imp_top returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to turn on power impdrv_osdep_pow_on_imp_top returns IMPDRV_EC_NG_PMSYSTEMERROR */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_pow_on_imp_top(IMPDRV_EC_NG_PMSYSTEMERROR);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) impdrv_osdep_pow_on_imp_top returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PMSYSTEMERROR));

	/* Fail to set interrupt setting impdrv_osdep_register_irq returns IMPDRV_EC_NG_SEQSTATE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_register_irq(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_register_irq returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to set interrupt setting impdrv_osdep_register_irq returns IMPDRV_EC_NG_ARGNULL */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_register_irq(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_register_irq returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to set interrupt setting impdrv_osdep_register_irq returns IMPDRV_EC_NG_PARAM */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_register_irq(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_register_irq returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to set interrupt setting impdrv_osdep_register_irq returns IMPDRV_EC_NG_SYSTEMERROR */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_register_irq(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_register_irq returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to set interrupt setting impdrv_osdep_enable_irq returns IMPDRV_EC_NG_SEQSTATE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_enable_irq(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_enable_irq returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to set interrupt setting impdrv_osdep_enable_irq returns IMPDRV_EC_NG_ARGNULL */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_enable_irq(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_enable_irq returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to set interrupt setting impdrv_osdep_enable_irq returns IMPDRV_EC_NG_PARAM */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_enable_irq(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_enable_irq returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to enable interrupt setting impdrv_osdep_enable_irq returns IMPDRV_EC_NG_SYSTEMERROR */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_enable_irq(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_enable_irq returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to read register impdrv_osdep_read_reg for INTerrupt Enable returns IMPDRV_EC_NG_SEQSTATE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_SEQSTATE, 0, 0);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_read_reg for INTerrupt Enable returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to read register impdrv_osdep_read_reg for INTerrupt Enable returns IMPDRV_EC_NG_ARGNULL */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_ARGNULL, 0, 0);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_read_reg for INTerrupt Enable returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to read register impdrv_osdep_read_reg for INTerrupt Enable returns IMPDRV_EC_NG_PARAM */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_PARAM, 0, 0);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_read_reg for INTerrupt Enable returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));
	
	/* Fail to read register impdrv_osdep_read_reg for INTerrupt Enable returns IMPDRV_EC_NG_SYSTEMERROR */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_SYSTEMERROR, 0, 0);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_read_reg for INTerrupt Enable returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to write register impdrv_osdep_write_reg for INTerrupt Enable returns IMPDRV_EC_NG_SEQSTATE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_SEQSTATE, s_impdrv_inten_addr[p_initdata->instance_num]);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_write_reg for INTerrupt Enable returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to write register impdrv_osdep_write_reg for INTerrupt Enable returns IMPDRV_EC_NG_ARGNULL */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_ARGNULL, s_impdrv_inten_addr[p_initdata->instance_num]);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_write_reg for INTerrupt Enable returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to write register impdrv_osdep_write_reg for INTerrupt Enable returns IMPDRV_EC_NG_PARAM */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_PARAM, s_impdrv_inten_addr[p_initdata->instance_num]);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_write_reg for INTerrupt Enable returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to write register impdrv_osdep_write_reg for INTerrupt Enable returns IMPDRV_EC_NG_CHECKFAIL */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_CHECKFAIL, s_impdrv_inten_addr[p_initdata->instance_num]);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_CHECKFAIL, *tret, "(No.%d) impdrv_osdep_write_reg for INTerrupt Enable returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_CHECKFAIL));

	/* Fail to write register impdrv_osdep_write_reg for INTerrupt Enable returns IMPDRV_EC_NG_SYSTEMERROR */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_SYSTEMERROR, s_impdrv_inten_addr[p_initdata->instance_num]);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_write_reg for INTerrupt Enable returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to turn on power impdrv_osdep_pow_on_hwrsc returns IMPDRV_EC_NG_SEQSTATE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_pow_on_hwrsc(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_pow_on_hwrsc returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to turn on power impdrv_osdep_pow_on_hwrsc returns IMPDRV_EC_NG_SYSTEMERROR */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_pow_on_hwrsc(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_pow_on_hwrsc returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to turn on power impdrv_osdep_pow_on_hwrsc returns IMPDRV_EC_NG_PMSYSTEMERROR */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_pow_on_hwrsc(IMPDRV_EC_NG_PMSYSTEMERROR);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) impdrv_osdep_pow_on_hwrsc returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PMSYSTEMERROR));

	/* Fail to turn on power impdrv_osdep_pow_on_imp returns IMPDRV_EC_NG_SEQSTATE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_pow_on_imp(IMPDRV_EC_NG_SEQSTATE, &core_info);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_pow_on_imp returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to turn on power impdrv_osdep_pow_on_imp returns IMPDRV_EC_NG_ARGNULL */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_pow_on_imp(IMPDRV_EC_NG_ARGNULL, &core_info);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_pow_on_imp returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to turn on power impdrv_osdep_pow_on_imp returns IMPDRV_EC_NG_SYSTEMERROR */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_pow_on_imp(IMPDRV_EC_NG_SYSTEMERROR, &core_info);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_pow_on_imp returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to turn on power impdrv_osdep_pow_on_imp returns IMPDRV_EC_NG_PMSYSTEMERROR */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_pow_on_imp(IMPDRV_EC_NG_PMSYSTEMERROR, &core_info);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) impdrv_osdep_pow_on_imp returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PMSYSTEMERROR));

	/* Fail to read register impdrv_osdep_read_reg for reading version of IMP0 returns IMPDRV_EC_NG_SEQSTATE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
#if !defined(RUN_SIL)
	order_call = 7;
#else
	order_call = 0;
#endif
#if defined (V4H) || defined (V4H2)
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_SEQSTATE, order_call + 1, 0);
#else
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_SEQSTATE, 3, 0);
#endif
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_read_reg for reading version of IMP0 returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to read register impdrv_osdep_read_reg for reading version of IMP0 returns IMPDRV_EC_NG_ARGNULL */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
#if defined (V4H) || defined (V4H2)
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_ARGNULL, order_call + 1, 0);
#else
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_ARGNULL, 3, 0);
#endif
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_read_reg for reading version of IMP0 returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to read register impdrv_osdep_read_reg for reading version of IMP0 returns IMPDRV_EC_NG_PARAM */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
#if defined (V4H) || defined (V4H2)
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_PARAM, order_call + 1, 0);
#else
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_PARAM, 3, 0);
#endif
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_read_reg for reading version of IMP0 returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));
	
	/* Fail to read register impdrv_osdep_read_reg for reading version of IMP0 returns IMPDRV_EC_NG_SYSTEMERROR */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
#if defined (V4H) || defined (V4H2)
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_SYSTEMERROR, order_call + 1, 0);
#else
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_SYSTEMERROR, 3, 0);
#endif
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_read_reg for reading version of IMP0 returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to read register impdrv_osdep_read_reg for reading version of IMP0 returns IMPDRV_EC_OK but read value is invalid */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
#if defined (V4H) || defined (V4H2)
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_OK, order_call + 1, 0);
#else
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_OK, 3, 0);
#endif
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_read_reg for reading version of IMP0 gets invalid value", testno++);

#if !defined (V4H) && !defined (V4H2)
	/* Fail to initialize DMA core memory impdrv_osdep_write_reg for DMA_RAMTSTR register returns IMPDRV_EC_NG_SEQSTATE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_SEQSTATE, IMPDRV_DMA_RAMTSTR);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_write_reg for DMA_RAMTSTR register returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to initialize DMA core memory impdrv_osdep_write_reg for DMA_RAMTSTR register returns IMPDRV_EC_NG_ARGNULL */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_ARGNULL, IMPDRV_DMA_RAMTSTR);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_write_reg for DMA_RAMTSTR register returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to initialize DMA core memory impdrv_osdep_write_reg for DMA_RAMTSTR register returns IMPDRV_EC_NG_PARAM */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_PARAM, IMPDRV_DMA_RAMTSTR);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_write_reg for DMA_RAMTSTR register returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to initialize DMA core memory impdrv_osdep_write_reg for DMA_RAMTSTR register returns IMPDRV_EC_NG_SYSTEMERROR */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_SYSTEMERROR, IMPDRV_DMA_RAMTSTR);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_write_reg for DMA_RAMTSTR register returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to read DMA core memory impdrv_osdep_read_reg for DMA_RAMTSTR register returns IMPDRV_EC_NG_SEQSTATE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_SEQSTATE, dmac_init_core_read_reg_index, 0);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_read_reg for DMA_RAMTSTR register returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to read DMA core memory impdrv_osdep_read_reg for DMA_RAMTSTR register returns IMPDRV_EC_NG_ARGNULL */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_ARGNULL, dmac_init_core_read_reg_index, 0);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_read_reg for DMA_RAMTSTR register returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to read DMA core memory impdrv_osdep_read_reg for DMA_RAMTSTR register returns IMPDRV_EC_NG_PARAM */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_PARAM, dmac_init_core_read_reg_index, 0);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_read_reg for DMA_RAMTSTR register returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to read DMA core memory impdrv_osdep_read_reg for DMA_RAMTSTR register returns IMPDRV_EC_NG_SYSTEMERROR */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_SYSTEMERROR, dmac_init_core_read_reg_index, 0);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_read_reg for DMA_RAMTSTR register returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to power off imp impdrv_osdep_pow_off_imp returns IMPDRV_EC_NG_SEQSTATE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_pow_off_imp(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_pow_off_imp returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to power off imp impdrv_osdep_pow_off_imp returns IMPDRV_EC_NG_ARGNULL */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_pow_off_imp(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_pow_off_imp returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to power off imp impdrv_osdep_pow_off_imp returns IMPDRV_EC_NG_SYSTEMERROR */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_pow_off_imp(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_pow_off_imp returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));
	
	/* Fail to power off imp impdrv_osdep_pow_off_imp returns IMPDRV_EC_NG_PMSYSTEMERROR */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_pow_off_imp(IMPDRV_EC_NG_PMSYSTEMERROR);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) impdrv_osdep_pow_off_imp returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PMSYSTEMERROR));
#else
    testno += 12;
#endif

	/* Fail to power off imp impdrv_osdep_dev_close returns IMPDRV_EC_NG_SEQSTATE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_dev_close(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_dev_close returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to power off imp impdrv_osdep_dev_close returns IMPDRV_EC_NG_ARGNULL */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_dev_close(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_dev_close returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to power off imp impdrv_osdep_dev_close returns IMPDRV_EC_NG_SYSTEMERROR */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_dev_close(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_dev_close returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to unlock mutex impdrv_osdep_mutex_unlock returns IMPDRV_EC_NG_SEQSTATE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_mutex_unlock(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to unlock mutex impdrv_osdep_mutex_unlock returns IMPDRV_EC_NG_ARGNULL */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_mutex_unlock(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to unlock mutex impdrv_osdep_mutex_unlock returns IMPDRV_EC_NG_SYSTEMERROR */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_osdep_mutex_unlock(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));
}
#endif

static void Device_init(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
    st_impdrv_initdata_t *p_initdata = &tparam->initdata;
    impdrv_ctrl_handle_t handle;
	st_impdrv_ctl_t *p_impdrv_ctl;
	st_impdrv_coretypectl_t	*coretype_ctl;
	uint32_t use_core_num;
	int32_t result = -1;
	
	/* driver_init : output parameter */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);

	p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
	coretype_ctl = p_impdrv_ctl->coretype_ctl;
	use_core_num = p_initdata->use_core_num;

	prepare_test_for_init(p_initdata);

	ercd = R_IMPDRV_Init(p_initdata, &handle);
	if (ercd != IMPDRV_EC_OK) {
		testmsg("(No.%d) fail(test NG %d)\n", testno++, ercd);
	} else if ((handle == NULL) || (handle != p_initdata->p_work_addr)) {
		testmsg("(No.%d) Parameter fail(test NG)\n", testno++);
	} else if (check_state(p_initdata, coretype_ctl, IMPDRV_STATE_INIT, use_core_num)) {
		testmsg("(No.%d) States fail(test NG)\n", testno++);
	} else {
		testmsg("(No.%d) Success(test OK)\n", testno++);
		result = 0;
	}

	CountResult(result == 0, tret);
}
