
#define TEST_MSG_NAME "R_IMPDRV_BusIfCheck"

#include "it_common.h"

#define CALLBACK_CODE 0xAA

static void set_state(st_impdrv_initdata_t *const p_data, e_impdrv_state_t state);
#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret);
static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret);
#endif
static void Device_bus_if_check(struct test_params_t *tparam, struct test_result_t *tret);

static uint32_t testno;
static st_impdrv_coretypectl_t coretype_ctl_backup[IMPDRV_CORE_TYPE_MAX];
static st_impdrv_chk_resource_t chk_resource_bk;

typedef struct {
	impdrv_ctrl_handle_t		dev_handle;
	st_impdrv_chk_resource_t	chk_resource
} tp_busif_data_t;

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

int32_t it_BusIfCheck_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

	memcpy(&chk_resource_bk, &tparam->chk_resource, sizeof(st_impdrv_chk_resource_t));

#ifdef IT_ABNORMAL
	testno = 261;
	Parameter_error(tparam, &tret);
	testno = 268;
	Invalid_core(tparam, &tret);
	testno = 273;
	No_exec_core(tparam, &tret);
	testno = 287;
	Invalid_state(tparam, &tret);
#endif
	testno = 291;
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
	ercd = R_IMPDRV_BusIfCheck(NULL, p_core_info, p_chk_resource);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) handle == NULL", testno++);

	/* Parameter_error : p_core_info == NULL */
	ercd = R_IMPDRV_BusIfCheck(handle, NULL, p_chk_resource);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) p_core_info == NULL", testno++);

	/* Parameter_error : p_chk_resource == NULL */
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, NULL);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) p_chk_resource == NULL", testno++);

	/* Parameter_error : p_chk_resource->memory_handle == NULL */
	p_chk_resource->memory_handle = NULL;
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) memory_handle == NULL", testno++);

	/* Parameter_error : p_chk_resource->axi_id == OSAL_AXI_BUS_ID_INVALID */
	memcpy(p_chk_resource, &chk_resource_bk, sizeof(st_impdrv_chk_resource_t));
	p_chk_resource->axi_id = OSAL_AXI_BUS_ID_INVALID;
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) axi_id == OSAL_AXI_BUS_ID_INVALID", testno++);

	/* Parameter_error : p_chk_resource->mq_id == 0U */
	memcpy(p_chk_resource, &chk_resource_bk, sizeof(st_impdrv_chk_resource_t));
	p_chk_resource->mq_id = 0U;
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) mq_id == 0U", testno++);

	/* Parameter_error : p_chk_resource->mq_time_period == -1 */
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
		ercd = R_IMPDRV_BusIfCheck(handle, &core_info, p_chk_resource);
		FailTestPrint(ercd, ((core_info.core_type == IMPDRV_CORE_TYPE_INVALID) ?
							IMPDRV_EC_NG_PARAM : IMPDRV_EC_NG_NOTSUPPORT),
							*tret, "(No.%d) Invalid core num for %s", testno++, type_to_name(core_info.core_type));
	}

	/* Invalid_error : Core_num = IMPDRV_CORE_NUM_MAX */
	core_info.core_type = IMPDRV_CORE_TYPE_IMP;
	core_info.core_num = IMPDRV_CORE_NUM_MAX;
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