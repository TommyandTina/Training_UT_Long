
#define TEST_MSG_NAME "R_IMPDRV_Init"

#include "it_common.h"

static void set_state(st_impdrv_initdata_t *const p_data, e_impdrv_state_t state);
#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret);
static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret);
#endif
static void Device_init(struct test_params_t *tparam, struct test_result_t *tret);
static void DeviceMemoryCorruption_init(struct test_params_t *tparam, struct test_result_t *tret);

static uint32_t testno;

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

int32_t it_InitMemoryCorruption_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	testno = 331;
	struct test_result_t tret = {0};
	DeviceMemoryCorruption_init(tparam, &tret);

	PrintTestResult(tret, test_result);

	return 0;
}

int32_t it_Init_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

#ifdef IT_ABNORMAL
	testno = 297;
	Parameter_error(tparam, &tret);
	Invalid_core(tparam, &tret);
	No_exec_core(tparam, &tret);
	Invalid_state(tparam, &tret);
#endif
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

	ercd = R_IMPDRV_Init(p_initdata, &handle);
	if (ercd != IMPDRV_EC_OK) {
		testmsg("(No.%d) fail(test NG %d)\n", testno++, ercd);
	} else if ((handle == NULL) || (handle != p_initdata->p_work_addr)) {
		testmsg("(No.%d) Parameter fail(test NG)\n", testno++);
	} else if (check_state(p_initdata, coretype_ctl, IMPDRV_STATE_INIT, use_core_num)) {
		testmsg("(No.%d) States fail(test NG)\n", testno++);
	} else if (p_initdata->out_work_size != (uint32_t)sizeof(st_impdrv_ctl_t)) {
		testmsg("(No.%d) Out work size invalid(test NG)\n", testno++);
	} else {
		testmsg("(No.%d) Success(test OK)\n", testno++);
		result = 0;
	}

	CountResult(result == 0, tret);
}

static void DeviceMemoryCorruption_init(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
    st_impdrv_initdata_t *p_initdata = &tparam->initdata;
    impdrv_ctrl_handle_t handle;
	st_impdrv_ctl_t *p_impdrv_ctl;
	st_impdrv_coretypectl_t	*coretype_ctl;
	uint32_t use_core_num;
	int32_t result = -1;
	uint8_t initValOfHandlerMem = 0xAA;
	uint8_t workarea_after;
	static uint8_t buffer[0x4000];

	/* driver_init : output parameter */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);

	p_initdata->p_work_addr = &buffer[0];
	p_initdata->work_size = sizeof(st_impdrv_ctl_t) + 1;
	(void)memset(p_initdata->p_work_addr, 0, p_initdata->work_size);
	((uint8_t*)p_initdata->p_work_addr)[p_initdata->work_size - 1] = initValOfHandlerMem;

	p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
	coretype_ctl = p_impdrv_ctl->coretype_ctl;
	use_core_num = p_initdata->use_core_num;

	ercd = R_IMPDRV_Init(p_initdata, &handle);
	if (ercd != IMPDRV_EC_OK) {
		testmsg("(No.%d) fail(test NG %d)\n", testno++, ercd);
	} else if ((handle == NULL) || (handle != p_initdata->p_work_addr)) {
		testmsg("(No.%d) Parameter fail(test NG)\n", testno++);
	} else if (check_state(p_initdata, coretype_ctl, IMPDRV_STATE_INIT, use_core_num)) {
		testmsg("(No.%d) States fail(test NG)\n", testno++);
	} else if (p_initdata->out_work_size != (uint32_t)sizeof(st_impdrv_ctl_t)) {
		testmsg("(No.%d) Out work size invalid(test NG)\n", testno++);
	} else {
		testmsg("(No.%d) Success(test OK)\n", testno++);
		result = 0;
	}

	workarea_after = ((uint8_t*)p_initdata->p_work_addr)[p_initdata->work_size - 1];
	if (workarea_after != initValOfHandlerMem) {
		printf("The index [work_size - 1] of (uint8_t*)p_work_addr is being violated. (0x%02X)\n", workarea_after);
	}

	(void)R_IMPDRV_Quit(handle);

	CountResult(result == 0, tret);
}
