#define TEST_MSG_NAME "CoreExec"

#include "it_common.h"

#define CALLBACK_CODE0	(0xFD)
#define CALLBACK_CODE1	(0xDF)
#define CALLBACK_CODE2	(0xEE)
#define SYNC_NUM		(3)

static int32_t Core_Sync(struct test_params_t *tparam, struct test_result_t *tret, impdrv_ctrl_handle_t handle);
static void Device_core_sync(struct test_params_t *tparam, struct test_result_t *tret);

static uint32_t testno;
static void *g_msgcnf[SYNC_NUM];

static int32_t CbFunc_0(
	const st_impdrv_core_info_t *p_core_info,		/**< Core information					*/
	const e_impdrv_cb_ret_t		ercd,				/**< Error code							*/
	const int32_t				code,				/**< TRAP or INT code (8 bit is set)	*/
	const void					*p_callback_args
)
{
	return (send_msg(g_msgcnf[0], ercd) | send_msg(g_msgcnf[0], code));
}

static int32_t CbFunc_1(
	const st_impdrv_core_info_t *p_core_info,		/**< Core information					*/
	const e_impdrv_cb_ret_t		ercd,				/**< Error code							*/
	const int32_t				code,				/**< TRAP or INT code (8 bit is set)	*/
	const void					*p_callback_args
)
{
	return (send_msg(g_msgcnf[1], ercd) | send_msg(g_msgcnf[1], code));
}

static int32_t CbFunc_2(
	const st_impdrv_core_info_t *p_core_info,		/**< Core information					*/
	const e_impdrv_cb_ret_t		ercd,				/**< Error code							*/
	const int32_t				code,				/**< TRAP or INT code (8 bit is set)	*/
	const void					*p_callback_args
)
{
	return (send_msg(g_msgcnf[2], ercd) | send_msg(g_msgcnf[2], code));
}

typedef struct callback_info {
	p_impdrv_cbfunc_t cb_func;
	int32_t cb_val;
} callback_info_t;

static const callback_info_t csinfo[SYNC_NUM] = {
	{ (p_impdrv_cbfunc_t)&CbFunc_0, CALLBACK_CODE0 },
	{ (p_impdrv_cbfunc_t)&CbFunc_1, CALLBACK_CODE1 },
	{ (p_impdrv_cbfunc_t)&CbFunc_2, CALLBACK_CODE2 }
};

int32_t it_CoreExec_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

	for (int32_t i = 0; i < SYNC_NUM; i++)
	{
		create_msg(&g_msgcnf[i], IT_OSAL_MQ_ID_CORE_EXEC1 + i);
	}

	testno = 1;
	Device_core_sync(tparam, &tret);

	for (int32_t i = 0; i < SYNC_NUM; i++)
	{
		destroy_msg(g_msgcnf[i]);
	}

	PrintTestResult(tret, test_result);

	return 0;
}

static int32_t Core_Sync(struct test_params_t *tparam, struct test_result_t *tret, impdrv_ctrl_handle_t handle)
{
	int32_t i;
	e_impdrv_errorcode_t ercd;
	st_impdrv_core_info_t core_map[IMPDRV_COREMAP_MAXID];
	sync_core_t *sync_core;
	char sync_name[32];
	uint32_t callback_args;
	int32_t result = 0;

	sync_core = get_sync_cores(CMD_LESS_THAN_6_CORES);
	if (NULL == sync_core)
	{
		testmsg("invalid test data(test SKIP)\n");
		result = -1;
	}

	for (; (NULL != sync_core) && (0 != sync_core->num); sync_core++)
	{
		int32_t len = 0;
		uint32_t syncbit = 0;
		result = 0;

		if (sync_core->num > SYNC_NUM) {
			testmsg("invalid test case(test SKIP)\n");
			result = -1;
			break;
		}

		memset(core_map, 0, sizeof(core_map));
		memset(sync_name, '\0', sizeof(sync_name));

		for (i = 0; i < sync_core->num; i++)
		{
			len += sprintf(sync_name + len, "%s%d-", type_to_name(sync_core->core[i].core_type), sync_core->core[i].core_num);
			core_map[i] = sync_core->core[i];
			syncbit |= (uint32_t)(1 << i);
		}
		sync_name[len - 1] = '\0';

		for (i = 0; i < sync_core->num; i++)
		{
			ercd = R_IMPDRV_AttrInit(handle, &sync_core->core[i]);
			if (IMPDRV_EC_OK != ercd)
			{
				core_testmsg(&sync_core->core[i], "set attr_init fail\n");
				result = -1;
				break;
			}
		}

		for (i = 0; i < sync_core->num; i++)
		{
			ercd = R_IMPDRV_Start(handle, &sync_core->core[i]);
			if (IMPDRV_EC_OK != ercd)
			{
				core_testmsg(&sync_core->core[i], "start fail\n");
				result = -1;
				goto error_start;
			}
		}

		for (i = 0; i < sync_core->num; i++)
		{
			ercd = R_IMPDRV_AttrSetCoreMap(handle, &sync_core->core[i], core_map);
			if (IMPDRV_EC_OK != ercd)
			{
				core_testmsg(&sync_core->core[i], "Set core_map fail\n");
				result = -1;
				goto error_start;
			}
		}

		for (i = 0; i < sync_core->num; i++)
		{
			if (set_synccommandlist(sync_core->core[i].core_type, tparam->claddr[i], (uint32_t)csinfo[i].cb_val, syncbit & (uint32_t)(~(1 << i))))
			{
				core_testmsg(&sync_core->core[i], "set_synccommandlist fail\n");
				result = -1;
				goto error_setcl;
			}
			else
			{
				ercd = R_IMPDRV_AttrSetCl(handle, &sync_core->core[i], util_mem_get_paddr(tparam->claddr[i]));
				if (IMPDRV_EC_OK != ercd)
				{
					core_testmsg(&sync_core->core[i], "Set core_map fail\n");
					result = -1;
					goto error_setcl;
				}
			}
		}

		for (i = 0; i < sync_core->num; i++)
		{
			ercd = R_IMPDRV_Execute(handle, &sync_core->core[i], csinfo[i].cb_func, (void *)&callback_args);
			if (IMPDRV_EC_OK != ercd)
			{
				core_testmsg(&sync_core->core[i], "R_IMPDRV_Execute fail(%d)\n", ercd);
				result = -1;
				goto error_exec;
			}
		}

		i = sync_core->num;
error_exec:
		for (i--; i >= 0; i--)
		{
			if (wait_and_check_callback(g_msgcnf[i], &sync_core->core[i], IMPDRV_CB_RET_BEFORE_EXEC, -1))
			{
				core_testmsg(&sync_core->core[i], "callback(before exec) fail\n");
				result = -1;
			}
			else if (wait_and_check_callback(g_msgcnf[i], &sync_core->core[i], IMPDRV_CB_RET_OK, csinfo[i].cb_val))
			{
				core_testmsg(&sync_core->core[i], "callback(trap) fail\n");
				result = -1;
			}
		}

		i = sync_core->num;
error_setcl:
		for (i--; i >= 0; i--) {
			util_mem_invalidate(tparam->claddr[i]);
		}

		i = sync_core->num;
error_start:
		for (i--; i >= 0; i--)
		{
			ercd = R_IMPDRV_Stop(handle, &sync_core->core[i]);
			if (IMPDRV_EC_OK != ercd)
			{
				core_testmsg(&sync_core->core[i], "R_IMPDRV_Stop fail(%d)\n", ercd);
				result = -1;
			}
		}

		SuccessTestPrint(result == 0, *tret, "(No.%d) %s ", testno++, sync_name);
	}

	return result;
}

static void Device_core_sync(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
	impdrv_ctrl_handle_t handle;
	int32_t result = 0;

	initialize_impdrvctl(tparam);
	set_input_data_before_init(tparam);
	memset(tparam->core_map, 0, sizeof(tparam->core_map));
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	if (IMPDRV_EC_OK != ercd)
	{
		testmsg("Init Fail(test NG %s)\n", impdrv_err_to_name(ercd));
		result = -1;
	}

	if (0 == result)
	{
		result = Core_Sync(tparam, tret, handle);
	}

	if (0 == result)
	{
		ercd = R_IMPDRV_Quit(handle);
		if (IMPDRV_EC_OK != ercd)
		{
			testmsg("Quit Fail(test NG %s)\n", impdrv_err_to_name(ercd));
			result = -1;
		}
	}
}