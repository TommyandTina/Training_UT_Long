#define TEST_MSG_NAME "R_IMPDRV_SetMemProtect"

#include "it_common.h"

static void set_state(st_impdrv_initdata_t *const p_data, e_impdrv_state_t state);
#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret);
static void UNINIT_state(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret);
#endif
static void Device_set_mem_protect(struct test_params_t *tparam, struct test_result_t *tret);

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

int32_t it_SetMemProtect_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

	/* Execute test */
#ifdef IT_ABNORMAL
	testno = 64;
	Parameter_error(tparam, &tret);
	testno = 68;
	Invalid_state(tparam, &tret);
#endif
	Device_set_mem_protect(tparam, &tret);

	PrintTestResult(tret, test_result);

	return 0;
}

int32_t it_SetMemProtect_UNINIT_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

	/* Execute test */
#ifdef IT_ABNORMAL
	testno = 67;
	UNINIT_state(tparam, &tret);
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

static void UNINIT_state(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	e_impdrv_protect_mode_t   protect_mode = IMPDRV_PROTECT_EDC;

	ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) %s", testno++, state_to_name(p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].core_state));
}

static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
	st_impdrv_core_info_t *p_core_info = p_initdata->core_info;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	e_impdrv_protect_mode_t   protect_mode = IMPDRV_PROTECT_EDC;
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
		ercd = R_IMPDRV_SetMemProtect(handle, protect_mode);
		FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) %s", testno++, state_to_name(invalid_state[i]));
	}

	/* Get backup data */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
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