
#define TEST_MSG_NAME "R_IMPDRV_AttrSetClBrkAddr"

#include "it_common.h"

static void set_state(st_impdrv_initdata_t *const p_data, e_impdrv_state_t state);
#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret);
static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret);
#endif
static void Device_attr_set_cl_brk_addr(struct test_params_t *tparam, struct test_result_t *tret);

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

int32_t it_AttrSetClBrkAddr_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

#ifdef IT_ABNORMAL
	testno = 174;
	Parameter_error(tparam, &tret);
	Invalid_core(tparam, &tret);
	No_exec_core(tparam, &tret);
	Invalid_state(tparam, &tret);
#endif
	Device_attr_set_cl_brk_addr(tparam, &tret);

	PrintTestResult(tret, test_result);

	return 0;
}

#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_core_info_t *p_core_info = tparam->initdata.core_info;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	uintptr_t claddr_phys = util_mem_get_paddr(tparam->claddr[0]);

	/* Parameter_error : handle == NULL */
	ercd = R_IMPDRV_AttrSetClBrkAddr(NULL, p_core_info, claddr_phys);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) handle == NULL", testno++);

	/* Parameter_error : p_core_info == NULL */
	ercd = R_IMPDRV_AttrSetClBrkAddr(handle, NULL, claddr_phys);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) p_core_info == NULL", testno++);

	/* Parameter_error : claddr_phys = 3U (invalid alignment) */
	claddr_phys = 3U;
	ercd = R_IMPDRV_AttrSetClBrkAddr(handle, p_core_info, claddr_phys);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) claddr_phys = 3U", testno++);

	/* Parameter_error : claddr_phys = 5U (invalid alignment) */
	claddr_phys = 5U;
	ercd = R_IMPDRV_AttrSetClBrkAddr(handle, p_core_info, claddr_phys);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) claddr_phys = 5U", testno++);

	/* Parameter_error : claddr_phys = 2U (invalid alignment) */
	claddr_phys = 2U;
	ercd = R_IMPDRV_AttrSetClBrkAddr(handle, p_core_info, claddr_phys);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) claddr_phys = 2U", testno++);
}

static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_IMP, 0 };
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	uintptr_t claddr_phys = util_mem_get_paddr(tparam->claddr[0]);
	tp_core_list_t invalid_core_list = get_core_list(IT_IMPDRV_CMD_INVALID_CORES, 0);

	/* Invalid_error : Invalid core num for coretypes */
	for (uint32_t i = 0; i < invalid_core_list.num; i++)
	{
		core_info = *(invalid_core_list.core_info + i);
		ercd = R_IMPDRV_AttrSetClBrkAddr(handle, &core_info, claddr_phys);
		FailTestPrint(ercd, ((core_info.core_type == IMPDRV_CORE_TYPE_INVALID) ?
							IMPDRV_EC_NG_PARAM : IMPDRV_EC_NG_NOTSUPPORT),
							*tret, "(No.%d) Invalid core num for %s", testno++, type_to_name(core_info.core_type));
	}

	/* Invalid_error : Core_num = IMPDRV_CORE_NUM_MAX */
	core_info.core_type = IMPDRV_CORE_TYPE_IMP;
	core_info.core_num = IMPDRV_CORE_NUM_MAX;
	ercd = R_IMPDRV_AttrSetClBrkAddr(handle, &core_info, claddr_phys);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) Core_num = IMPDRV_CORE_NUM_MAX", testno++);
}

static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_IMP, 0 };
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	uintptr_t claddr_phys = util_mem_get_paddr(tparam->claddr[0]);
	tp_core_list_t no_exec_cores = get_core_list(IT_IMPDRV_CMD_NO_EXEC, 0);

	/* Not supported core: Core not supported */
	for (uint32_t i = 0; i < no_exec_cores.num; i++)
	{
		core_info = *(no_exec_cores.core_info + i);
		ercd = R_IMPDRV_AttrSetClBrkAddr(handle, &core_info, claddr_phys);
		FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) Core not supported: %s", testno++, type_to_name(core_info.core_type));
	}
}

static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
	st_impdrv_core_info_t *p_core_info = p_initdata->core_info;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	uintptr_t claddr_phys = util_mem_get_paddr(tparam->claddr[0]);
	e_impdrv_state_t invalid_state[] = {
		IMPDRV_STATE_UNINIT,
		IMPDRV_STATE_INIT,
		IMPDRV_STATE_EXEC,
	};

	/* Back up data */
	memcpy(coretype_ctl_backup, p_impdrv_ctl->coretype_ctl, sizeof(p_impdrv_ctl->coretype_ctl));

	for (uint32_t i = 0; i < ARRAY_SIZE(invalid_state); i++)
	{
		set_state(p_initdata, invalid_state[i]);
		ercd = R_IMPDRV_AttrSetClBrkAddr(handle, p_core_info, claddr_phys);
		FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) %s", testno++, state_to_name(invalid_state[i]));
	}

	/* Get backup data */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
}
#endif

static void Device_attr_set_cl_brk_addr(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
    st_impdrv_initdata_t *p_initdata = &tparam->initdata;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	uintptr_t claddr_phys = util_mem_get_paddr(tparam->claddr[0]);
	int32_t result = 0;
	st_impdrv_core_info_t *p_core_info = &tparam->initdata.core_info[0];

	/* driver_attr_set_cl_brk_addr : check CL addr = 0U */
	ercd = R_IMPDRV_AttrSetClBrkAddr(handle, p_core_info, 0U);
	if (ercd != IMPDRV_EC_OK) {
		core_testmsg(p_core_info, "CL_physaddr = 0U, ercd = %s fail\n", impdrv_err_to_name(ercd));
		result = -1;
	}
	SuccessTestPrint(result == 0, *tret, "(No.%d) CL_physaddr = 0U ", testno++);
	
	/* driver_attr_set_cl_brk_addr : output parameter */
	for (uint32_t i = 0; i < p_initdata->use_core_num; i++)
	{
		e_impdrv_core_type_t core_type;
		uint32_t core_num;
		st_impdrv_coretypectl_t *coretype_ctl;

		p_core_info = &p_initdata->core_info[i];
		core_type = p_core_info->core_type;
		core_num = p_core_info->core_num;
		coretype_ctl = &p_impdrv_ctl->coretype_ctl[core_type];

		result = 0;
		if (p_core_info->core_type == IMPDRV_CORE_TYPE_DSP)
		{
			continue;
		}
		ercd = R_IMPDRV_AttrSetClBrkAddr(handle, p_core_info, claddr_phys);
		if (ercd != IMPDRV_EC_OK) {
			core_testmsg(p_core_info, "%s=%d\n", TEST_MSG_NAME, ercd);
			result = -1;
		} else {
			/* Check state of the core */
			if (coretype_ctl->core_ctl[core_num].core_state != IMPDRV_STATE_READY) {
				core_testmsg(p_core_info, "state=%d, it is not READY(test NG)\n", state_to_name(coretype_ctl->core_ctl[core_num].core_state));
				result = -1;
			} else if (coretype_ctl->core_ctl[core_num].param[0] != claddr_phys) {
				core_testmsg(p_core_info, "claddr_phys = %u, it is not %u(test NG)\n", coretype_ctl->core_ctl[core_num].claddr_phys, claddr_phys);
				result = -1;
			} else {
				result = 0;
			}
		}

		SuccessTestPrint(result == 0, *tret, "(No.%d) %s%d ", testno++, type_to_name(core_type), p_core_info->core_num);
	}
}