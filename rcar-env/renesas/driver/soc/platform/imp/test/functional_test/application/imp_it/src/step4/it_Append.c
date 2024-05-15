#define TEST_MSG_NAME "Append"

#include "it_common.h"

#define CALLBACK_CODE_1		(0xBE)
#define CALLBACK_CODE_2		(0xCF)

static void INT_TRAP_hung(struct test_params_t *tparam, struct test_result_t *tret);
static void INT_ERR_hung(struct test_params_t *tparam, struct test_result_t *tret);

static void *g_msgcnf;
static uint32_t testno;

static int32_t CbFunc(
	const st_impdrv_core_info_t *p_core_info,		/**< Core information					*/
	const e_impdrv_cb_ret_t		ercd,				/**< Error code							*/
	const int32_t				code,				/**< TRAP or INT code (8 bit is set)	*/
	const void					*p_callback_args
)
{
#if defined (TARGET_OS_LINUX)
	syscall(SYS_membarrier, MEMBARRIER_CMD_REGISTER_PRIVATE_EXPEDITED, 0, 0);
#endif
	return (send_msg(g_msgcnf, ercd) | send_msg(g_msgcnf, code));
}

int32_t it_Append_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

	create_msg(&g_msgcnf, IT_OSAL_MQ_ID_APPEND);

	testno = 1;
	INT_TRAP_hung(tparam, &tret);
	INT_ERR_hung(tparam, &tret);

	destroy_msg(g_msgcnf);

	PrintTestResult(tret, test_result);

	return 0;
}

static void INT_TRAP_hung(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	int32_t i, j, count;
    st_impdrv_initdata_t *p_initdata = &tparam->initdata;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;
	void *claddr = tparam->claddr[0];
	int32_t int_count;
	int32_t result = 0;
	
	/* driver_execute : output parameter */
	for (i = 0; i < p_initdata->use_core_num; i++)
	{
		st_impdrv_core_info_t *p_core_info = &p_initdata->core_info[i];
		e_impdrv_core_type_t core_type = p_core_info->core_type;
		uint32_t core_num = p_core_info->core_num;
		st_impdrv_coretypectl_t *coretype_ctl = &p_impdrv_ctl->coretype_ctl[core_type];
		result = 0;

#if defined (V3H1)
		if ((IMPDRV_CORE_TYPE_PSCEXE == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno += 2;
		}
#elif defined (V3H2)
		if ((IMPDRV_CORE_TYPE_CNN == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno += 1;
		}
#elif defined (V3M2)
		if ((IMPDRV_CORE_TYPE_OCV == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno += 2;
		}
		if ((IMPDRV_CORE_TYPE_DMAC == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno += 3;
		}
		if ((IMPDRV_CORE_TYPE_PSCEXE == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno += 4;
		}
		if ((IMPDRV_CORE_TYPE_CNN == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno += 1;
		}
#elif defined (V4H) || defined (V4H2)
        if (p_core_info->core_type == IMPDRV_CORE_TYPE_DSP)
		{
			continue;
		}
		if ((IMPDRV_CORE_TYPE_OCV == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno += 2;
		}
		if ((IMPDRV_CORE_TYPE_DMAC == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno += 1;
		}
		if ((IMPDRV_CORE_TYPE_CNN == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno += 1;
		}
#endif
		int_count = set_conint_commandlist(p_core_info->core_type, claddr, CALLBACK_CODE_1, CALLBACK_CODE_2);
		if (int_count <= 1)
		{
			core_testmsg(p_core_info, "set_con_intcommandlist error(test NG)\n");
			result = -1;
			goto error1;
		}

		for (j = 0; j < 256; j++)
		{
			printf("(%d/%d)\r", j, 256);

			if (IMPDRV_EC_OK != R_IMPDRV_AttrSetCl(handle, p_core_info, util_mem_get_paddr(claddr)))
			{
				core_testmsg(p_core_info, "Set CL data error\n");
				result = -1;
				break;
			}

			ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
			if (ercd != IMPDRV_EC_OK)
			{
				core_testmsg(p_core_info, "%s=%d\n", "R_IMPDRV_Execute", ercd);
				result = -1;
				break;
			}

			if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_BEFORE_EXEC, -1))
			{
				core_testmsg(p_core_info, "callback(before_exec) fail\n");
				result = -1;
				break;
			}

			for (count = 0; count < int_count; count++)
			{
				if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_INT, CALLBACK_CODE_1)) {
					core_testmsg(p_core_info, "callback(int) fail(test NG)\n");
					result = -1;
					break;
				}

				ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
				if (ercd != IMPDRV_EC_OK) {
					core_testmsg(p_core_info, "%s=%d\n", TEST_MSG_NAME, ercd);
					result = -1;
					break;
				}
			}

			if (0 != result)
			{
				break;
			}

			if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_OK, CALLBACK_CODE_2))
			{
				core_testmsg(p_core_info, "callback(trap) fail\n");
				result = -1;
				break;
			}
		}

		util_mem_invalidate(claddr);

error1:
		SuccessTestPrint(result == 0, *tret, "(No.%d) %s%d ", testno++, type_to_name(core_type), core_num);
	}
}

static void INT_ERR_hung(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	int32_t i, j, count;
    st_impdrv_initdata_t *p_initdata = &tparam->initdata;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;
	void *claddr = tparam->claddr[0];
	int32_t result = 0;
	
	/* driver_execute : output parameter */
	for (i = 0; i < p_initdata->use_core_num; i++)
	{
		st_impdrv_core_info_t *p_core_info = &p_initdata->core_info[i];
		e_impdrv_core_type_t core_type = p_core_info->core_type;
		uint32_t core_num = p_core_info->core_num;
		st_impdrv_coretypectl_t *coretype_ctl = &p_impdrv_ctl->coretype_ctl[core_type];
		result = 0;

#if defined (V3H1)
		if ((IMPDRV_CORE_TYPE_PSCEXE == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno += 2;
		}
#elif defined (V3H2)
		if ((IMPDRV_CORE_TYPE_CNN == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno += 1;
		}
#elif defined (V3M2)
		if ((IMPDRV_CORE_TYPE_OCV == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno += 2;
		}
		if ((IMPDRV_CORE_TYPE_DMAC == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno += 3;
		}
		if ((IMPDRV_CORE_TYPE_PSCEXE == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno += 4;
		}
		if ((IMPDRV_CORE_TYPE_CNN == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno += 1;
		}
#elif defined (V4H) || defined (V4H2)
        if (p_core_info->core_type == IMPDRV_CORE_TYPE_DSP)
		{
			continue;
		}
#endif

		if (set_interror_commandlist(p_core_info->core_type, claddr, CALLBACK_CODE_1, CALLBACK_CODE_2))
		{
			core_testmsg(p_core_info, "set_interror_commandlist error(test NG)\n");
			result = -1;
			goto error1;
		}

		for (j = 0; j < 1024; j++)
		{
			printf("(%d/%d)\r", j, 1024);

			if (IMPDRV_EC_OK != R_IMPDRV_AttrSetCl(handle, p_core_info, util_mem_get_paddr(claddr)))
			{
				core_testmsg(p_core_info, "Set CL data error\n");
				result = -1;
				break;
			}

			ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
			if (IMPDRV_EC_OK != ercd)
			{
				core_testmsg(p_core_info, "%s=%d\n", "R_IMPDRV_Execute", ercd);
				result = -1;
				break;
			}

			if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_BEFORE_EXEC, -1))
			{
				core_testmsg(p_core_info, "callback(before_exec) fail\n");
				result = -1;
				break;
			}

			for (count = 0; count < 2; count++)
			{
				if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_INT, CALLBACK_CODE_1))
				{
					core_testmsg(p_core_info, "callback(int) fail(test NG)\n");
					result = -1;
					break;
				}

				ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
				if (IMPDRV_EC_OK != ercd)
				{
					core_testmsg(p_core_info, "%s=%d\n", TEST_MSG_NAME, ercd);
					result = -1;
					break;
				}
			}

			if (0 != result)
			{
				break;
			}

			if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_ILLEGAL, (int32_t)0xFFFFFFFF))
			{
				core_testmsg(p_core_info, "callback(Illegal) fail\n");
				result = -1;
				break;
			}
		}

		util_mem_invalidate(claddr);

error1:
		SuccessTestPrint(result == 0, *tret, "(No.%d) %s%d ", testno++, type_to_name(core_type), core_num);
	}
}