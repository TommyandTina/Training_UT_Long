
#define TEST_MSG_NAME "R_IMPDRV_BusIfCheck"

#include "it_common.h"
#include "it_stub.h"

static void prepare_test(impdrv_ctrl_handle_t handle, const st_impdrv_core_info_t *const p_core_info);
static void set_state(st_impdrv_initdata_t *const p_data, e_impdrv_state_t state);
#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret);
static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret);
static void Return_error(struct test_params_t *tparam, struct test_result_t *tret);
#endif
static void Device_bus_if_check(struct test_params_t *tparam, struct test_result_t *tret);

static uint32_t testno;
static st_impdrv_coretypectl_t coretype_ctl_backup[IMPDRV_CORE_TYPE_MAX];
static st_impdrv_chk_resource_t chk_resource_bk;
static uint32_t mem_cpu_addr;
static uint32_t mem_hw_addr = 128U;

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

static void prepare_test(impdrv_ctrl_handle_t handle, const st_impdrv_core_info_t *const p_core_info)
{
	uint32_t i = 0U;
	uint32_t j = 0U;
	uint32_t k = 0U;
	uint32_t loop = 0U;
	uint32_t read_index;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)handle;

	p_impdrv_ctl->common_ctl.irq_mask_value = 0U;

	if (IMPDRV_CORE_TYPE_IMP == p_core_info->core_type)
	{
		it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_OK, 0, 1U);
		it_stub_set_impdrv_osdep_memory_get_size(IMPDRV_EC_OK, 0, IMPDRV_IMP_MEM_SIZE_BUSCHK);
		it_stub_set_impdrv_osdep_memory_get_logical(IMPDRV_EC_OK, 0, &mem_cpu_addr);
		it_stub_set_impdrv_osdep_memory_get_physical(IMPDRV_EC_OK, 0, mem_hw_addr);
		it_stub_set_impdrv_osdep_queue_wait_period(IMPDRV_EC_OK, p_core_info->core_num);
	}
	else if (IMPDRV_CORE_TYPE_IMP_SLIM == p_core_info->core_type)
	{
		it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_OK, 0, 1U);
		it_stub_set_impdrv_osdep_memory_get_size(IMPDRV_EC_OK, 0, IMPDRV_IMP_MEM_SIZE_BUSCHK);
		it_stub_set_impdrv_osdep_memory_get_logical(IMPDRV_EC_OK, 0, &mem_cpu_addr);
		it_stub_set_impdrv_osdep_memory_get_physical(IMPDRV_EC_OK, 0, mem_hw_addr);
		it_stub_set_impdrv_osdep_queue_wait_period(IMPDRV_EC_OK, p_core_info->core_num);
	}
	else if (IMPDRV_CORE_TYPE_DMAC == p_core_info->core_type)
	{
		it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_OK, 0, 1U);
		it_stub_set_impdrv_osdep_memory_get_size(IMPDRV_EC_OK, 0, IMPDRV_IMP_MEM_SIZE_BUSCHK);
		it_stub_set_impdrv_osdep_memory_get_logical(IMPDRV_EC_OK, 0, &mem_cpu_addr);
		it_stub_set_impdrv_osdep_memory_get_physical(IMPDRV_EC_OK, 0, mem_hw_addr);
		it_stub_set_impdrv_osdep_queue_wait_period(IMPDRV_EC_OK, p_core_info->core_num);
	}
	else
	{
		/* do nothing */
	}
}

int32_t it_BusIfCheck_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

	memcpy(&chk_resource_bk, &tparam->chk_resource, sizeof(st_impdrv_chk_resource_t));

#ifdef IT_ABNORMAL
	testno = 59;
	Parameter_error(tparam, &tret);
	testno = 66;
	Invalid_core(tparam, &tret);
	testno = 71;
	No_exec_core(tparam, &tret);
	testno = 85;
	Invalid_state(tparam, &tret);
	testno = 89;
	Return_error(tparam, &tret);
#endif
	testno = 143;
	Device_bus_if_check(tparam, &tret);

	PrintTestResult(tret, test_result);

	return 0;
}

#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_core_info_t *p_core_info = get_core_list(IT_IMPDRV_CMD_BUSIF, 0).core_info;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)tparam->initdata.p_work_addr;
	st_impdrv_chk_resource_t *p_chk_resource = &tparam->chk_resource;

	/* Parameter_error : handle == NULL */
	it_stub_reset();
	prepare_test(handle, p_core_info);
	ercd = R_IMPDRV_BusIfCheck(NULL, p_core_info, p_chk_resource);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) handle == NULL", testno++);

	/* Parameter_error : p_core_info == NULL */
	it_stub_reset();
	prepare_test(handle, p_core_info);
	ercd = R_IMPDRV_BusIfCheck(handle, NULL, p_chk_resource);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) p_core_info == NULL", testno++);

	/* Parameter_error : p_chk_resource == NULL */
	it_stub_reset();
	prepare_test(handle, p_core_info);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, NULL);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) p_chk_resource == NULL", testno++);

	/* Parameter_error : p_chk_resource->memory_handle == NULL */
	it_stub_reset();
	prepare_test(handle, p_core_info);
	p_chk_resource->memory_handle = NULL;
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) memory_handle == NULL", testno++);

	/* Parameter_error : p_chk_resource->axi_id == OSAL_AXI_BUS_ID_INVALID */
	it_stub_reset();
	prepare_test(handle, p_core_info);
	memcpy(p_chk_resource, &chk_resource_bk, sizeof(st_impdrv_chk_resource_t));
	p_chk_resource->axi_id = OSAL_AXI_BUS_ID_INVALID;
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) axi_id == OSAL_AXI_BUS_ID_INVALID", testno++);

	/* Parameter_error : p_chk_resource->mq_id == 0U */
	it_stub_reset();
	prepare_test(handle, p_core_info);
	memcpy(p_chk_resource, &chk_resource_bk, sizeof(st_impdrv_chk_resource_t));
	p_chk_resource->mq_id = 0U;
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) mq_id == 0U", testno++);

	/* Parameter_error : p_chk_resource->mq_time_period == -1 */
	it_stub_reset();
	prepare_test(handle, p_core_info);
	memcpy(p_chk_resource, &chk_resource_bk, sizeof(st_impdrv_chk_resource_t));
	p_chk_resource->mq_time_period = (-1);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) mq_time_period == -1", testno++);

	/* restore valid data */
	memcpy(p_chk_resource, &chk_resource_bk, sizeof(st_impdrv_chk_resource_t));
}

static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_core_info_t core_info;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)tparam->initdata.p_work_addr;
	st_impdrv_chk_resource_t *p_chk_resource = &tparam->chk_resource;
	tp_core_list_t invalid_core_list = get_core_list(IT_IMPDRV_CMD_INVALID_CORES, 0);

	/* Invalid_error : Invalid core num for coretypes */
	for (uint32_t i = 0; i < invalid_core_list.num; i++)
	{
		if ((IMPDRV_CORE_TYPE_INVALID != (invalid_core_list.core_info + i)->core_type) &&
			(IMPDRV_CORE_TYPE_IMP != (invalid_core_list.core_info + i)->core_type) &&
			(IMPDRV_CORE_TYPE_IMP_SLIM != (invalid_core_list.core_info + i)->core_type) &&
			(IMPDRV_CORE_TYPE_DMAC != (invalid_core_list.core_info + i)->core_type))
		{
			continue;
		}

#if defined (V3M2) || defined (V4H) || defined (V4H2)
		if (IMPDRV_CORE_TYPE_DMAC == (invalid_core_list.core_info + i)->core_type)
		{
			testno++;
		}
#endif

		core_info = *(invalid_core_list.core_info + i);
		it_stub_reset();
		prepare_test(handle, &core_info);
		ercd = R_IMPDRV_BusIfCheck(handle, &core_info, p_chk_resource);
		FailTestPrint(ercd, ((core_info.core_type == IMPDRV_CORE_TYPE_INVALID) ?
							IMPDRV_EC_NG_PARAM : IMPDRV_EC_NG_NOTSUPPORT),
							*tret, "(No.%d) Invalid core num for %s", testno++, type_to_name(core_info.core_type));
	}

	/* Invalid_error : Core_num = IMPDRV_CORE_NUM_MAX */
	core_info.core_type = IMPDRV_CORE_TYPE_IMP;
	core_info.core_num = IMPDRV_CORE_NUM_MAX;
	it_stub_reset();
	prepare_test(handle, &core_info);
	ercd = R_IMPDRV_BusIfCheck(handle, &core_info, p_chk_resource);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) Core_num = IMPDRV_CORE_NUM_MAX", testno++);
}

static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_core_info_t *p_core_info;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)tparam->initdata.p_work_addr;
	st_impdrv_chk_resource_t *p_chk_resource = &tparam->chk_resource;
	tp_core_list_t no_exec_cores = get_core_list(IT_IMPDRV_CMD_NO_EXEC_BUSIF, 0);

	/* Not supported core: Core not supported */
	for (uint32_t i = 0; i < no_exec_cores.num; i++)
	{
		p_core_info = no_exec_cores.core_info + i;

#if defined (V3H1)
		if ((IMPDRV_CORE_TYPE_PSCEXE == p_core_info->core_type) && (0u == p_core_info->core_num))
		{
			testno += 2;
		}
#elif defined (V3H2)
		if ((IMPDRV_CORE_TYPE_CNN == p_core_info->core_type) && (0u == p_core_info->core_num))
		{
			testno += 1;
		}
#elif defined (V3M2)
		if ((IMPDRV_CORE_TYPE_PSCEXE == p_core_info->core_type) && (0u == p_core_info->core_num))
		{
			testno += 5;
		}
		else if ((IMPDRV_CORE_TYPE_CNN == p_core_info->core_type) && (0u == p_core_info->core_num))
		{
			testno += 1;
		}
#endif

		it_stub_reset();
		prepare_test(handle, p_core_info);
		ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
		FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) Core not supported: %s%d", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num);
	}
}

static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	st_impdrv_chk_resource_t *p_chk_resource = &tparam->chk_resource;
	st_impdrv_core_info_t *p_core_info = get_core_list(IT_IMPDRV_CMD_BUSIF, 0).core_info;
	e_impdrv_state_t invalid_state[] = {
		IMPDRV_STATE_UNINIT,
		IMPDRV_STATE_INIT,
		IMPDRV_STATE_EXEC,
		IMPDRV_STATE_INT
	};

	/* Back up data */
	memcpy(coretype_ctl_backup, p_impdrv_ctl->coretype_ctl, sizeof(p_impdrv_ctl->coretype_ctl));

	for (uint32_t i = 0; i < ARRAY_SIZE(invalid_state); i++)
	{
		it_stub_reset();
		prepare_test(handle, p_core_info);
		set_state(p_initdata, invalid_state[i]);
		ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
#if !defined (V4H) && !defined (V4H2)
		FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) %s", testno++, state_to_name(invalid_state[i]));
#else
		FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) %s", testno++, state_to_name(invalid_state[i]));
#endif
	}

	/* Get backup data */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
}

static void Return_error(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	st_impdrv_chk_resource_t *p_chk_resource = &tparam->chk_resource;
	tp_core_list_t exec_cores = get_core_list(IT_IMPDRV_CMD_BUSIF, 0);
	st_impdrv_core_info_t *p_core_info = exec_cores.core_info;

	/* Fail to lock mutex impdrv_osdep_mutex_lock returns IMPDRV_EC_NG_SEQSTATE */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_mutex_lock(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to lock mutex impdrv_osdep_mutex_lock returns IMPDRV_EC_NG_ARGNULL */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_mutex_lock(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to lock mutex impdrv_osdep_mutex_lock returns IMPDRV_EC_NG_PARAM */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_mutex_lock(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to lock mutex impdrv_osdep_mutex_lock returns IMPDRV_EC_NG_SYSTEMERROR */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_mutex_lock(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

#if !defined (V4H) && !defined (V4H2)
	/* Fail to read register impdrv_osdep_read_reg returns IMPDRV_EC_NG_SEQSTATE */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_SEQSTATE, 0, 0);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_read_reg returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to read register impdrv_osdep_read_reg returns IMPDRV_EC_NG_ARGNULL */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_ARGNULL, 0, 0);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_read_reg returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to read register impdrv_osdep_read_reg returns IMPDRV_EC_NG_ARGNULL */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_PARAM, 0, 0);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_read_reg returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to read register impdrv_osdep_read_reg returns IMPDRV_EC_NG_ARGNULL */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_read_reg(IMPDRV_EC_NG_SYSTEMERROR, 0, 0);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_read_reg returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to write register impdrv_osdep_write_reg returns IMPDRV_EC_NG_SEQSTATE */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_SEQSTATE, RCVDRV_IMP_OFFSET_ALL);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_write_reg returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to write register impdrv_osdep_write_reg returns IMPDRV_EC_NG_ARGNULL */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_ARGNULL, RCVDRV_IMP_OFFSET_ALL);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_write_reg returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to write register impdrv_osdep_write_reg returns IMPDRV_EC_NG_PARAM */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_PARAM, RCVDRV_IMP_OFFSET_ALL);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_write_reg returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to write register impdrv_osdep_write_reg returns IMPDRV_EC_NG_SYSTEMERROR */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_write_reg(IMPDRV_EC_NG_SYSTEMERROR, RCVDRV_IMP_OFFSET_ALL);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_write_reg returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to get size of memory impdrv_osdep_memory_get_size returns IMPDRV_EC_NG_SEQSTATE */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_memory_get_size(IMPDRV_EC_NG_SEQSTATE, 0, IMPDRV_IMP_MEM_SIZE_BUSCHK);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_memory_get_size returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to get size of memory impdrv_osdep_memory_get_size returns IMPDRV_EC_NG_ARGNULL */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_memory_get_size(IMPDRV_EC_NG_ARGNULL, 0, IMPDRV_IMP_MEM_SIZE_BUSCHK);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_memory_get_size returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to get size of memory impdrv_osdep_memory_get_size returns IMPDRV_EC_NG_PARAM */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_memory_get_size(IMPDRV_EC_NG_PARAM, 0, IMPDRV_IMP_MEM_SIZE_BUSCHK);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_memory_get_size returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to get size of memory impdrv_osdep_memory_get_size returns IMPDRV_EC_NG_SYSTEMERROR */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_memory_get_size(IMPDRV_EC_NG_SYSTEMERROR, 0, IMPDRV_IMP_MEM_SIZE_BUSCHK);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_memory_get_size returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to get CPU address of memory impdrv_osdep_memory_get_logical returns IMPDRV_EC_NG_SEQSTATE */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_memory_get_logical(IMPDRV_EC_NG_SEQSTATE, 0, &mem_cpu_addr);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_memory_get_logical returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to get CPU address of memory impdrv_osdep_memory_get_logical returns IMPDRV_EC_NG_ARGNULL */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_memory_get_logical(IMPDRV_EC_NG_ARGNULL, 0, &mem_cpu_addr);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_memory_get_logical returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to get CPU address of memory impdrv_osdep_memory_get_logical returns IMPDRV_EC_NG_SYSTEMERROR */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_memory_get_logical(IMPDRV_EC_NG_SYSTEMERROR, 0, &mem_cpu_addr);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_memory_get_logical returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to get physical of memory impdrv_osdep_memory_get_physical returns IMPDRV_EC_NG_SEQSTATE */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_memory_get_physical(IMPDRV_EC_NG_SEQSTATE, 0, mem_hw_addr);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_memory_get_physical returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to get physical of memory impdrv_osdep_memory_get_physical returns IMPDRV_EC_NG_ARGNULL */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_memory_get_physical(IMPDRV_EC_NG_ARGNULL, 0, mem_hw_addr);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_memory_get_physical returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to get physical of memory impdrv_osdep_memory_get_physical returns IMPDRV_EC_NG_PARAM */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_memory_get_physical(IMPDRV_EC_NG_PARAM, 0, mem_hw_addr);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_memory_get_physical returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to get physical of memory impdrv_osdep_memory_get_physical returns IMPDRV_EC_NG_SYSTEMERROR */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_memory_get_physical(IMPDRV_EC_NG_SYSTEMERROR, 0, mem_hw_addr);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_memory_get_physical returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to set data impdrv_osdep_memory_data_set returns IMPDRV_EC_NG_ARGNULL */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_memory_data_set(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_memory_data_set returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to set data impdrv_osdep_memory_data_set returns IMPDRV_EC_NG_PARAM */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_memory_data_set(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_memory_data_set returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to flush cache impdrv_osdep_memory_cache_flush returns IMPDRV_EC_NG_SEQSTATE */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_memory_cache_flush(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_memory_cache_flush returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to flush cache impdrv_osdep_memory_cache_flush returns IMPDRV_EC_NG_ARGNULL */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_memory_cache_flush(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_memory_cache_flush returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to flush cache impdrv_osdep_memory_cache_flush returns IMPDRV_EC_NG_PARAM */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_memory_cache_flush(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_memory_cache_flush returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to flush cache impdrv_osdep_memory_cache_flush returns IMPDRV_EC_NG_SYSTEMERROR */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_memory_cache_flush(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_memory_cache_flush returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to create message queue impdrv_osdep_queue_create returns IMPDRV_EC_NG_SEQSTATE */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_queue_create(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_queue_create returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to create message queue impdrv_osdep_queue_create returns IMPDRV_EC_NG_ARGNULL */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_queue_create(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_queue_create returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to create message queue impdrv_osdep_queue_create returns IMPDRV_EC_NG_PARAM */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_queue_create(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_queue_create returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to create message queue impdrv_osdep_queue_create returns IMPDRV_EC_NG_SYSTEMERROR */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_queue_create(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_queue_create returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to memory barrier impdrv_osdep_memory_barrier for IMP0 returns IMPDRV_EC_NG_SYSTEMERROR */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_memory_barrier(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_memory_barrier for IMP0 returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

#if defined (V3H1) || defined (V3H2)
	/* Fail to memory barrier impdrv_osdep_memory_barrier for SlimIMP returns IMPDRV_EC_NG_SYSTEMERROR */
	p_core_info = get_core_info(tparam->initdata.core_info, tparam->initdata.use_core_num, IMPDRV_CORE_TYPE_IMP_SLIM, 0);
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_memory_barrier(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_memory_barrier for SlimIMP returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));
#else
	testno++;
#endif

	/* Fail to memory barrier impdrv_osdep_memory_barrier for DMAC00 returns IMPDRV_EC_NG_SYSTEMERROR */
	p_core_info = get_core_info(tparam->initdata.core_info, tparam->initdata.use_core_num, IMPDRV_CORE_TYPE_DMAC, 0);
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_memory_barrier(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_memory_barrier for DMAC00 returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to wait for message impdrv_osdep_queue_wait_period returns IMPDRV_EC_NG_CHECKFAIL */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_queue_wait_period(IMPDRV_EC_NG_CHECKFAIL, p_core_info->core_num);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_CHECKFAIL, *tret, "(No.%d) impdrv_osdep_queue_wait_period returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_CHECKFAIL));

	/* Fail to wait for message impdrv_osdep_queue_wait_period returns IMPDRV_EC_NG_SEQSTATE */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_queue_wait_period(IMPDRV_EC_NG_SEQSTATE, p_core_info->core_num);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_queue_wait_period returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to wait for message impdrv_osdep_queue_wait_period returns IMPDRV_EC_NG_ARGNULL */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_queue_wait_period(IMPDRV_EC_NG_ARGNULL, p_core_info->core_num);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_queue_wait_period returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to wait for message impdrv_osdep_queue_wait_period returns IMPDRV_EC_NG_PARAM */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_queue_wait_period(IMPDRV_EC_NG_PARAM, p_core_info->core_num);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_queue_wait_period returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to wait for message impdrv_osdep_queue_wait_period returns IMPDRV_EC_NG_SYSTEMERROR */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_queue_wait_period(IMPDRV_EC_NG_SYSTEMERROR, p_core_info->core_num);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_queue_wait_period returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to destroy message queue impdrv_osdep_queue_destroy returns IMPDRV_EC_NG_SEQSTATE */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_queue_destroy(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_queue_destroy returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to destroy message queue impdrv_osdep_queue_destroy returns IMPDRV_EC_NG_ARGNULL */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_queue_destroy(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_queue_destroy returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to destroy message queue impdrv_osdep_queue_destroy returns IMPDRV_EC_NG_SYSTEMERROR */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_queue_destroy(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_queue_destroy returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to clean cache memory impdrv_osdep_memory_cache_clean returns IMPDRV_EC_NG_SEQSTATE */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_memory_cache_clean(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_memory_cache_clean returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to clean cache memory impdrv_osdep_memory_cache_clean returns IMPDRV_EC_NG_ARGNULL */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_memory_cache_clean(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_memory_cache_clean returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to clean cache memory impdrv_osdep_memory_cache_clean returns IMPDRV_EC_NG_PARAM */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_memory_cache_clean(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_memory_cache_clean returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to clean cache memory impdrv_osdep_memory_cache_clean returns IMPDRV_EC_NG_SYSTEMERROR */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_memory_cache_clean(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_memory_cache_clean returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to compare memory regions impdrv_osdep_memory_compare returns IMPDRV_EC_NG_ARGNULL */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_memory_compare(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_memory_compare returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to compare memory regions impdrv_osdep_memory_compare returns IMPDRV_EC_NG_PARAM */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_memory_compare(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_memory_compare returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to compare memory regions impdrv_osdep_memory_compare returns IMPDRV_EC_NG_CHECKFAIL */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_memory_compare(IMPDRV_EC_NG_CHECKFAIL);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_CHECKFAIL, *tret, "(No.%d) impdrv_osdep_memory_compare returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_CHECKFAIL));

	/* Fail to unlock mutex impdrv_osdep_mutex_unlock returns IMPDRV_EC_NG_SEQSTATE */
	p_core_info = tparam->initdata.core_info;
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_mutex_unlock(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_osdep_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to unlock mutex impdrv_osdep_mutex_unlock returns IMPDRV_EC_NG_ARGNULL */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_mutex_unlock(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to unlock mutex impdrv_osdep_mutex_unlock returns IMPDRV_EC_NG_SYSTEMERROR */
	prepare_test(handle, p_core_info);
	it_stub_set_impdrv_osdep_mutex_unlock(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_osdep_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));
#endif
}
#endif

static void Device_bus_if_check(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_core_info_t *p_core_info;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)tparam->initdata.p_work_addr;
	st_impdrv_chk_resource_t *p_chk_resource = &tparam->chk_resource;
	tp_core_list_t exec_cores = get_core_list(IT_IMPDRV_CMD_BUSIF, 0);
	int32_t result;

	/* driver_bus_if_check : output parameter */
	for (uint32_t i = 0; i < exec_cores.num; i++)
	{
		p_core_info = (exec_cores.core_info + i);

#if defined (V3M2)
		if ((IMPDRV_CORE_TYPE_DMAC == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno += 2;
		}
#endif

		result = 0;

		it_stub_reset();
		prepare_test(handle, p_core_info);
		ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
#if !defined (V4H) && !defined (V4H2)
		if (ercd != IMPDRV_EC_OK)
#else
		if (ercd != IMPDRV_EC_NG_NOTSUPPORT)
#endif
		{
			core_testmsg(p_core_info, "%s=%d\n", TEST_MSG_NAME, ercd);
			result = -1;
		}

		SuccessTestPrint(result == 0, *tret, "(No.%d) %s%d ", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num);
	}
}
