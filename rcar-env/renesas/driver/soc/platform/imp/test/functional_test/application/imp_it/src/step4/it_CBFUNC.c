#define TEST_MSG_NAME "CBFUNC"

#include "it_common.h"

#define WUPCOVF_PAIR_NUM	(2)
#define IMP_EXE_CORE_NUM	(3)

#define CALLBACK_CODE1		(0x1C)
#define CALLBACK_CODE2		(0x5D)

extern int32_t cbfunc_fatal(
    const st_impdrv_core_info_t *const p_core_info,     /**< Core information                   */
    const e_impdrv_cb_ret_t     ercd,                   /**< Error code                         */
    const int32_t               code,                   /**< TRAP or INT code (8 bit is set)    */
    void                        *const p_callback_args  /**< User argument                      */
);

static void Core_Execute(struct test_params_t *tparam, struct test_result_t *tret);
static void INT_callback(struct test_params_t *tparam, struct test_result_t *tret);
static void Illegal_Callback(struct test_params_t *tparam, struct test_result_t *tret);
static void PBCOVF_INT_Masked_Callback(struct test_params_t *tparam, struct test_result_t *tret);
static void PBCOVF_INT_Enabled_Callback(struct test_params_t *tparam, struct test_result_t *tret);
static void PBCOVF_TRAP_Masked_Callback(struct test_params_t *tparam, struct test_result_t *tret);
static void PBCOVF_TRAP_Enabled_Callback(struct test_params_t *tparam, struct test_result_t *tret);
static void SBO0ME_INT_Callback(struct test_params_t *tparam, struct test_result_t *tret);
static void SBO0ME_TRAP_Callback(struct test_params_t *tparam, struct test_result_t *tret);
static void APIPINT_Callback(struct test_params_t *tparam, struct test_result_t *tret);
static void HPINT_Callback(struct test_params_t *tparam, struct test_result_t *tret);
static void MSCO_INT_Callback(struct test_params_t *tparam, struct test_result_t *tret);
static void MSCO_Callback(struct test_params_t *tparam, struct test_result_t *tret);
static void END_Callback(struct test_params_t *tparam, struct test_result_t *tret);
static void USIER_Callback(struct test_params_t *tparam, struct test_result_t *tret);
static void USINT_Callback(struct test_params_t *tparam, struct test_result_t *tret);
static void WUPCOVF_Callback(struct test_params_t *tparam, struct test_result_t *tret);
static void WUPCOVF_IMP_Callback(struct test_params_t *tparam, struct test_result_t *tret);

static void *g_msgcnf;
static void *g_msgcnf_imp_wupcovf[IMP_EXE_CORE_NUM];
static uint32_t testno;
static bool msco_flg;
static st_impdrv_device_handle_t dev_handle = {0};

int32_t it_CBFUNC_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

	msco_flg = false;
	msco_int_flg = false;

	create_msg(&g_msgcnf, IT_OSAL_MQ_ID_CBFUNC);

	for (uint32_t i = 0; i < IMP_EXE_CORE_NUM; i++)
	{
		create_msg(&g_msgcnf_imp_wupcovf[i], IT_OSAL_MQ_ID_CORE_EXEC2 + i);
	}

	testno = 48;
	Core_Execute(tparam, &tret);
	testno = 71;
	INT_callback(tparam, &tret);
	testno = 93;
	Illegal_Callback(tparam, &tret);
	testno = 115;
	PBCOVF_INT_Masked_Callback(tparam, &tret);
	testno = 120;
	PBCOVF_INT_Enabled_Callback(tparam, &tret);
	testno = 125;
	PBCOVF_TRAP_Masked_Callback(tparam, &tret);
	testno = 130;
	PBCOVF_TRAP_Enabled_Callback(tparam, &tret);
	testno = 135;
	SBO0ME_INT_Callback(tparam, &tret);
	testno = 140;
	SBO0ME_TRAP_Callback(tparam, &tret);
	testno = 145;
	APIPINT_Callback(tparam, &tret);
	testno = 150;
	HPINT_Callback(tparam, &tret);
	testno = 155;
	WUPCOVF_IMP_Callback(tparam, &tret);
	WUPCOVF_Callback(tparam, &tret);
#if !defined (V4H) && !defined (V4H2)
	testno = 153;
	MSCO_INT_Callback(tparam, &tret);
	testno = 154;
	MSCO_Callback(tparam, &tret);
#endif
	testno = 162;
	END_Callback(tparam, &tret);
	testno = 164;
	USINT_Callback(tparam, &tret);
	
	destroy_msg(g_msgcnf);

	for (uint32_t i = 0; i < IMP_EXE_CORE_NUM; i++)
	{
		destroy_msg(g_msgcnf_imp_wupcovf[i]);
	}

	PrintTestResult(tret, test_result);

	return 0;
}

int32_t it_USIER(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

	create_msg(&g_msgcnf, IT_OSAL_MQ_ID_CBFUNC);

	testno = 169;
	USIER_Callback(tparam, &tret);

	destroy_msg(g_msgcnf);

	PrintTestResult(tret, test_result);

	return 0;
}

static void Core_Execute(struct test_params_t *tparam, struct test_result_t *tret)
{
	uint32_t i;
	e_impdrv_errorcode_t ercd;
	st_impdrv_core_info_t *p_core_info;
	e_impdrv_core_type_t core_type;
	uint32_t core_num;

	LOGPRINT(("**[info]The result of " TEST_MSG_NAME ":%s test uses the result of R_IMPDRV_Execute:Core_Exec test.\n", __func__));

	for (i = 0; i < tparam->initdata.use_core_num; i++)
	{
		p_core_info = &tparam->initdata.core_info[i];
		core_type = p_core_info->core_type;
		core_num = p_core_info->core_num;

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
#elif defined (V4H) || defined (V4H2)
		if ((IMPDRV_CORE_TYPE_PSCEXE == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno = 67;
		}
		else if ((IMPDRV_CORE_TYPE_DMAC == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno = 59;
		}
		else if ((IMPDRV_CORE_TYPE_OCV == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno = 54;
		}
		else if ((IMPDRV_CORE_TYPE_CNN == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno = 69;
		}
		else if ((IMPDRV_CORE_TYPE_DMAC_SLIM == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno = 63;
		}
		else if ((IMPDRV_CORE_TYPE_DSP == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno = 70;
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
#endif

		switch (hook_cbfunc_exec_ret[core_type][core_num])
		{
			case HOOK_TEST_RET_OK:
				print_tn(TEST_MSG_NAME, __func__, "(No.%d) %s%d success(test OK)\n", testno++, type_to_name(core_type), core_num);
				tret->passed_cnt++;
				break;
			case HOOK_TEST_RET_NG:
				print_tn(TEST_MSG_NAME, __func__, "(No.%d) %s%d fail(test NG)\n", testno++, type_to_name(core_type), core_num);
				tret->failed_cnt++;
				break;
			default:
				print_tn(TEST_MSG_NAME, __func__, "(No.%d) %s%d Not Tested(test Skip)\n", testno++, type_to_name(core_type), core_num);
				printf("   Execute the R_IMPDRV_Execute:Core_Exec test before this test.");
				tret->skipped_cnt++;
				break;
		}
	}
}

static void INT_callback(struct test_params_t *tparam, struct test_result_t *tret)
{
	uint32_t i;
	e_impdrv_errorcode_t ercd;
	st_impdrv_core_info_t *p_core_info;
	e_impdrv_core_type_t core_type;
	uint32_t core_num;

	LOGPRINT(("**[info]The result of " TEST_MSG_NAME ":%s test uses the result of R_IMPDRV_ResumeExecution:Core_Resume test.\n", __func__));

	for (i = 0; i < tparam->initdata.use_core_num; i++)
	{
		p_core_info = &tparam->initdata.core_info[i];
		core_type = p_core_info->core_type;
		core_num = p_core_info->core_num;

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
		if ((IMPDRV_CORE_TYPE_PSCEXE == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno = 90;
		}
		else if ((IMPDRV_CORE_TYPE_DMAC == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno = 82;
		}
		else if ((IMPDRV_CORE_TYPE_OCV == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno = 77;
		}
		else if ((IMPDRV_CORE_TYPE_CNN == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno = 92;
		}
		else if ((IMPDRV_CORE_TYPE_DMAC_SLIM == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno = 86;
		}
		else if ((IMPDRV_CORE_TYPE_DSP == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			continue;
		}
#endif

		switch (hook_cbfunc_int_ret[core_type][core_num])
		{
			case HOOK_TEST_RET_OK:
				print_tn(TEST_MSG_NAME, __func__, "(No.%d) %s%d success(test OK)\n", testno++, type_to_name(core_type), core_num);
				tret->passed_cnt++;
				break;
			case HOOK_TEST_RET_NG:
				print_tn(TEST_MSG_NAME, __func__, "(No.%d) %s%d fail(test NG)\n", testno++, type_to_name(core_type), core_num);
				tret->failed_cnt++;
				break;
			default:
				print_tn(TEST_MSG_NAME, __func__, "(No.%d) %s%d Not Tested(test Skip)\n", testno++, type_to_name(core_type), core_num);
				printf("   Execute the R_IMPDRV_ResumeExecution:Core_Resume test before this test.");
				tret->skipped_cnt++;
				break;
		}
	}
}

static int32_t CbFunc(
	const st_impdrv_core_info_t *p_core_info,		/**< Core information					*/
	const e_impdrv_cb_ret_t		ercd,				/**< Error code							*/
	const int32_t				code,				/**< TRAP or INT code (8 bit is set)	*/
	const void					*p_callback_args
)
{
	int32_t ret;

	ret = send_msg(g_msgcnf, ercd) | send_msg(g_msgcnf, code);

	if ((IMPDRV_CB_RET_BEFORE_EXEC == ercd) && (true == msco_flg))
	{
		R_OSAL_IoWrite32(dev_handle.handle, OFS_MAX_VAL_THRES, 0x0du);
		R_OSAL_IoWrite32(dev_handle.handle, OFS_MIN_VAL_THRES, 0x00u);
        R_OSAL_IoWrite32(dev_handle.handle, OFS_MAXSUM_CNT_THRES, 0x00001105u);
        R_OSAL_IoWrite32(dev_handle.handle, OFS_MINSUM_CNT_THRES, 0x00005505u);
        R_OSAL_IoWrite32(dev_handle.handle, OFS_MAXSUM_CNT, 0x00u);
        R_OSAL_IoWrite32(dev_handle.handle, OFS_MINSUM_CNT, 0x00u);
	}

	return ret;
}

static void Illegal_Callback(struct test_params_t *tparam, struct test_result_t *tret)
{
	uint32_t i;
	e_impdrv_errorcode_t ercd;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	st_impdrv_core_info_t *p_core_info;
	e_impdrv_core_type_t core_type;
	uint32_t core_num;
	p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;

	/* Illegal callback : IMP */
	/* Illegal callback : OCV */
	/* Illegal callback : DMA */
	/* Illegal callback : PSC */
	/* Illegal callback : CNN */
	/* Illegal callback : SIMP */
	for (i = 0; i < tparam->initdata.use_core_num; i++)
	{
		int32_t result = -1;

		p_core_info = &tparam->initdata.core_info[i];
		core_type = p_core_info->core_type;
		core_num = p_core_info->core_num;

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
		if ((IMPDRV_CORE_TYPE_PSCEXE == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno = 112;
		}
		else if ((IMPDRV_CORE_TYPE_DMAC == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno = 104;
		}
		else if ((IMPDRV_CORE_TYPE_OCV == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno = 99;
		}
		else if ((IMPDRV_CORE_TYPE_CNN == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno = 114;
		}
		else if ((IMPDRV_CORE_TYPE_DMAC_SLIM == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno = 108;
		}
		else if ((IMPDRV_CORE_TYPE_DSP == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			continue;
		}
#endif

		if (set_errcommandlist(core_type, tparam->claddr[0], CALLBACK_CODE1))
		{
			core_testmsg(p_core_info, "set_commandlist error(test NG)\n");
			goto error1;
		}

		if (IMPDRV_EC_OK != R_IMPDRV_AttrSetCl(handle, p_core_info, util_mem_get_paddr(tparam->claddr[0])))
		{
			core_testmsg(p_core_info, "Set CL data error\n");
			goto error2;
		}

		ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
		if (ercd != IMPDRV_EC_OK)
		{
			core_testmsg(p_core_info, "R_IMPDRV_Execute fail (%d)(test NG)\n", ercd);
			goto error2;
		}

		if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_BEFORE_EXEC, -1))
		{
			core_testmsg(p_core_info, "callback fail for IMPDRV_CB_RET_BEFORE_EXEC(test NG)\n");
			goto error2;
		}

		if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_ILLEGAL, (int32_t)0xFFFFFFFF))
		{
			core_testmsg(p_core_info, "callback fail for IMPDRV_CB_RET_ILLEGAL(test NG)\n");
			goto error2;
		}

		result = 0;

error2:
		util_mem_invalidate(tparam->claddr[0]);
error1:
		SuccessTestPrint(result == 0, *tret, "(No.%d) %s%d ", testno++, type_to_name(core_type), core_num);
	}
}

static void PBCOVF_INT_Masked_Callback(struct test_params_t *tparam, struct test_result_t *tret)
{
	uint32_t i;
	e_impdrv_errorcode_t ercd;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	st_impdrv_core_info_t *p_core_info;
	e_impdrv_core_type_t core_type;
	uint32_t core_num;
	p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;
	void *claddr = tparam->long_claddr;
	bool irq_mask[IMPDRV_IRQMASK_MAX];

	memset(irq_mask, true, sizeof(irq_mask));

	for (i = 0; i < tparam->initdata.use_core_num; i++)
	{
		if (IMPDRV_CORE_TYPE_OCV == tparam->initdata.core_info[i].core_type)
		{
			int32_t result = -1;

			p_core_info = &tparam->initdata.core_info[i];
			core_type = p_core_info->core_type;
			core_num = p_core_info->core_num;

			if (set_intpbcovf_commandlist(core_type, claddr, CALLBACK_CODE2))
			{
				core_testmsg(p_core_info, "set_pbcovfcommandlist error\n");
			}
			else if (IMPDRV_EC_OK != R_IMPDRV_AttrSetCl(handle, p_core_info, util_mem_get_paddr(claddr)))
			{
				core_testmsg(p_core_info, "Set CL data error\n");
			}
			else if (IMPDRV_EC_OK != R_IMPDRV_AttrSetIrqMask(handle, p_core_info, irq_mask))
			{
				core_testmsg(p_core_info, "Set IRQ mask error\n");
			}
			else
			{
				ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
				if (ercd != IMPDRV_EC_OK)
				{
					core_testmsg(p_core_info, "R_IMPDRV_Execute=%d error(test NG)\n", ercd);
				}
				else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_BEFORE_EXEC, -1))
				{
					core_testmsg(p_core_info, "callback(before exec) fail(test NG)\n");
				}
				else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_OK, CALLBACK_CODE2))
				{
					core_testmsg(p_core_info, "callback(trap) fail(test NG)\n");
				}
				else
				{
					result = 0;
				}
			}

			util_mem_invalidate(claddr);
			SuccessTestPrint(result == 0, *tret, "(No.%d)  %s%d ", testno++, type_to_name(core_type), core_num);
		}
	}
}

static void PBCOVF_INT_Enabled_Callback(struct test_params_t *tparam, struct test_result_t *tret)
{
	uint32_t i;
	e_impdrv_errorcode_t ercd;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	st_impdrv_core_info_t *p_core_info;
	e_impdrv_core_type_t core_type;
	uint32_t core_num;
	p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;
	void *claddr = tparam->long_claddr;
	bool irq_mask[IMPDRV_IRQMASK_MAX];

	memset(irq_mask, true, sizeof(irq_mask));

	irq_mask[IMPDRV_IRQMASK_PBCOVF] = false;

	for (i = 0; i < tparam->initdata.use_core_num; i++)
	{
		if (IMPDRV_CORE_TYPE_OCV == tparam->initdata.core_info[i].core_type)
		{
			int32_t result = -1;

			p_core_info = &tparam->initdata.core_info[i];
			core_type = p_core_info->core_type;
			core_num = p_core_info->core_num;

			if (set_intpbcovf_commandlist(core_type, claddr, CALLBACK_CODE2))
			{
				core_testmsg(p_core_info, "set_pbcovfcommandlist error\n");
			}
			else if (IMPDRV_EC_OK != R_IMPDRV_AttrSetCl(handle, p_core_info, util_mem_get_paddr(claddr)))
			{
				core_testmsg(p_core_info, "Set CL data error\n");
			}
			else if (IMPDRV_EC_OK != R_IMPDRV_AttrSetIrqMask(handle, p_core_info, irq_mask))
			{
				core_testmsg(p_core_info, "Set IRQ mask error\n");
			}
			else
			{
				ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
				if (ercd != IMPDRV_EC_OK)
				{
					core_testmsg(p_core_info, "R_IMPDRV_Execute=%d error(test NG)\n", ercd);
				}
				else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_BEFORE_EXEC, -1))
				{
					core_testmsg(p_core_info, "callback(before exec) fail(test NG)\n");
				}
				else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_PBCOVF, -1))
				{
					core_testmsg(p_core_info, "callback(pbcovf) fail(test NG)\n");
				}
				else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_INT_PBCOVF, CALLBACK_CODE2))
				{
					core_testmsg(p_core_info, "callback(pbcovf_int) fail(test NG)\n");
				}
				else
				{
					ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
					if (ercd != IMPDRV_EC_OK)
					{
						core_testmsg(p_core_info, "R_IMPDRV_ResumeExecution fail (%d)(test NG)\n", ercd);
					}
					else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_PBCOVF, -1))
					{
						core_testmsg(p_core_info, "callback(IMPDRV_CB_RET_PBCOVF) after resume fail(test NG)\n");
					}
					else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_PBCOVF, -1))
					{
						core_testmsg(p_core_info, "callback(IMPDRV_CB_RET_PBCOVF) after previous one fail(test NG)\n");
					}
					else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_OK, CALLBACK_CODE2))
					{
						core_testmsg(p_core_info, "callback(trap) fail(test NG)\n");
					}
					else
					{
						result = 0;
					}
				}
			}

			util_mem_invalidate(claddr);
			SuccessTestPrint(result == 0, *tret, "(No.%d)  %s%d ", testno++, type_to_name(core_type), core_num);
		}
	}
}

static void PBCOVF_TRAP_Masked_Callback(struct test_params_t *tparam, struct test_result_t *tret)
{
	uint32_t i;
	e_impdrv_errorcode_t ercd;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	st_impdrv_core_info_t *p_core_info;
	e_impdrv_core_type_t core_type;
	uint32_t core_num;
	p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;
	void *claddr = tparam->long_claddr;
	bool irq_mask[IMPDRV_IRQMASK_MAX];

	memset(irq_mask, true, sizeof(irq_mask));

	for (i = 0; i < tparam->initdata.use_core_num; i++)
	{
		if (IMPDRV_CORE_TYPE_OCV == tparam->initdata.core_info[i].core_type)
		{
			int32_t result = -1;

			p_core_info = &tparam->initdata.core_info[i];
			core_type = p_core_info->core_type;
			core_num = p_core_info->core_num;

			if (set_pbcovf_commandlist(core_type, claddr, CALLBACK_CODE2))
			{
				core_testmsg(p_core_info, "set_pbcovfcommandlist error\n");
			}
			else if (IMPDRV_EC_OK != R_IMPDRV_AttrSetCl(handle, p_core_info, util_mem_get_paddr(claddr)))
			{
				core_testmsg(p_core_info, "Set CL data error\n");
			}
			else if (IMPDRV_EC_OK != R_IMPDRV_AttrSetIrqMask(handle, p_core_info, irq_mask))
			{
				core_testmsg(p_core_info, "Set IRQ mask error\n");
			}
			else
			{
				ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
				if (ercd != IMPDRV_EC_OK)
				{
					core_testmsg(p_core_info, "R_IMPDRV_Execute=%d error(test NG)\n", ercd);
				}
				else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_BEFORE_EXEC, -1))
				{
					core_testmsg(p_core_info, "callback(before exec) fail(test NG)\n");
				}
				else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_OK, CALLBACK_CODE2))
				{
					core_testmsg(p_core_info, "callback(trap) fail(test NG)\n");
				}
				else
				{
					result = 0;
				}
			}

			util_mem_invalidate(claddr);
			SuccessTestPrint(result == 0, *tret, "(No.%d)  %s%d ", testno++, type_to_name(core_type), core_num);
		}
	}
}

static void PBCOVF_TRAP_Enabled_Callback(struct test_params_t *tparam, struct test_result_t *tret)
{
	uint32_t i;
	e_impdrv_errorcode_t ercd;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	st_impdrv_core_info_t *p_core_info;
	e_impdrv_core_type_t core_type;
	uint32_t core_num;
	p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;
	void *claddr = tparam->long_claddr;
	bool irq_mask[IMPDRV_IRQMASK_MAX];

	memset(irq_mask, true, sizeof(irq_mask));

	irq_mask[IMPDRV_IRQMASK_PBCOVF] = false;

	for (i = 0; i < tparam->initdata.use_core_num; i++)
	{
		if (IMPDRV_CORE_TYPE_OCV == tparam->initdata.core_info[i].core_type)
		{
			int32_t result = -1;

			p_core_info = &tparam->initdata.core_info[i];
			core_type = p_core_info->core_type;
			core_num = p_core_info->core_num;

			if (set_pbcovf_commandlist(core_type, claddr, CALLBACK_CODE2))
			{
				core_testmsg(p_core_info, "set_pbcovfcommandlist error\n");
			}
			else if (IMPDRV_EC_OK != R_IMPDRV_AttrSetCl(handle, p_core_info, util_mem_get_paddr(claddr)))
			{
				core_testmsg(p_core_info, "Set CL data error\n");
			}
			else if (IMPDRV_EC_OK != R_IMPDRV_AttrSetIrqMask(handle, p_core_info, irq_mask))
			{
				core_testmsg(p_core_info, "Set IRQ mask error\n");
			}
			else
			{
				ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
				if (ercd != IMPDRV_EC_OK)
				{
					core_testmsg(p_core_info, "R_IMPDRV_Execute=%d error(test NG)\n", ercd);
				}
				else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_BEFORE_EXEC, -1))
				{
					core_testmsg(p_core_info, "callback(before exec) fail(test NG)\n");
				}
				else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_PBCOVF, -1))
				{
					core_testmsg(p_core_info, "callback(pbcovf_trap) fail(test NG)\n");
				}
				else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_PBCOVF, -1))
				{
					core_testmsg(p_core_info, "callback(pbcovf_trap) fail(test NG)\n");
				}
				else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_OK, CALLBACK_CODE2))
				{
					core_testmsg(p_core_info, "callback(trap) fail(test NG)\n");
				}
				else
				{
					result = 0;
				}
			}

			util_mem_invalidate(claddr);
			SuccessTestPrint(result == 0, *tret, "(No.%d)  %s%d ", testno++, type_to_name(core_type), core_num);
		}
	}
}

static void SBO0ME_INT_Callback(struct test_params_t *tparam, struct test_result_t *tret)
{
	uint32_t i;
	e_impdrv_errorcode_t ercd;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	st_impdrv_core_info_t *p_core_info;
	e_impdrv_core_type_t core_type;
	uint32_t core_num;
	p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;
	int8_t result;
	void *claddr;
	uint32_t claddr_phys;
	uint32_t maxsbo = 3;
	bool irq_mask[IMPDRV_IRQMASK_MAX];

	memset(irq_mask, true, sizeof(irq_mask));

	irq_mask[IMPDRV_IRQMASK_SBO0ME] = false;

	claddr = createCL_SBOtest(maxsbo);
	claddr_phys = util_mem_get_paddr(claddr);

	for (i = 0; i < tparam->initdata.use_core_num; i++)
	{
		if (IMPDRV_CORE_TYPE_OCV == tparam->initdata.core_info[i].core_type)
		{
			result = -1;
			p_core_info = &tparam->initdata.core_info[i];
			core_type = p_core_info->core_type;
			core_num = p_core_info->core_num;

			if (IMPDRV_EC_OK != R_IMPDRV_AttrSetCl(handle, p_core_info, claddr_phys))
			{
				core_testmsg(p_core_info, "Set CL data error\n");
			}
			else if (IMPDRV_EC_OK != R_IMPDRV_AttrSetIrqMask(handle, p_core_info, irq_mask))
			{
				core_testmsg(p_core_info, "Set IRQ mask error\n");
			}
			else
			{
				ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
				if (ercd != IMPDRV_EC_OK)
				{
					core_testmsg(p_core_info, "R_IMPDRV_Execute fail (%d)(test NG)\n", ercd);
				}
				else
				{
					if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_BEFORE_EXEC, -1))
					{
						core_testmsg(p_core_info, "callback(before exec) fail(test NG)\n");
					}
					else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_INT_SBO0ME, 0))
					{
						core_testmsg(p_core_info, "callback(int_sbo0me) fail(test NG)\n");
					}
					else
					{
						result = 0;
					}
				}

				SuccessTestPrint(result == 0, *tret, "(No.%d) %s%d INT_SBO0ME ", testno++, type_to_name(core_type), core_num);

				if (ercd == IMPDRV_EC_OK)
				{
					ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
					if (ercd != IMPDRV_EC_OK)
					{
						core_testmsg(p_core_info, "(No.%d) R_IMPDRV_ResumeExecution fail (%d)(test NG)\n", testno++, ercd);
					}
					else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_TRAP_SBO0ME, 0))
					{
						core_testmsg(p_core_info, "callback(trap) fail(test NG)\n");
					}
				}
			}
			
			util_mem_invalidate(claddr);
		}
	}
}

static void SBO0ME_TRAP_Callback(struct test_params_t *tparam, struct test_result_t *tret)
{
	uint32_t i;
	e_impdrv_errorcode_t ercd;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	st_impdrv_core_info_t *p_core_info;
	e_impdrv_core_type_t core_type;
	uint32_t core_num;
	p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;
	int8_t result;
	void *claddr;
	uint32_t claddr_phys;
	uint32_t maxsbo = 1;
	bool irq_mask[IMPDRV_IRQMASK_MAX];

	memset(irq_mask, true, sizeof(irq_mask));
	irq_mask[IMPDRV_IRQMASK_SBO0ME] = false;

	claddr = createCL_SBOtest(maxsbo);
	claddr_phys = util_mem_get_paddr(claddr);

	for (i = 0; i < tparam->initdata.use_core_num; i++)
	{
		if (IMPDRV_CORE_TYPE_OCV == tparam->initdata.core_info[i].core_type)
		{
			result = -1;
			p_core_info = &tparam->initdata.core_info[i];
			core_type = p_core_info->core_type;
			core_num = p_core_info->core_num;

			if (IMPDRV_EC_OK != R_IMPDRV_AttrSetCl(handle, p_core_info, claddr_phys))
			{
				core_testmsg(p_core_info, "Set CL data error\n");
			}
			else if (IMPDRV_EC_OK != R_IMPDRV_AttrSetIrqMask(handle, p_core_info, irq_mask))
			{
				core_testmsg(p_core_info, "Set IRQ mask error\n");
			}
			else
			{
				ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
				if (ercd != IMPDRV_EC_OK)
				{
					core_testmsg(p_core_info, "R_IMPDRV_Execute fail (%d)(test NG)\n", ercd);
				}
				else
				{
					if ((wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_BEFORE_EXEC, -1)) ||
						(wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_INT_SBO0ME, 0)))
					{
						core_testmsg(p_core_info, "callback fail(test NG)\n");
					}

					ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
					if (ercd != IMPDRV_EC_OK)
					{
						core_testmsg(p_core_info, "R_IMPDRV_ResumeExecution fail (%d)(test NG)\n", ercd);
					}
					else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_TRAP_SBO0ME, 0))
					{
						core_testmsg(p_core_info, "callback(trap) fail(test NG)\n");
					}
					else
					{
						result = 0;
					}
				}

				SuccessTestPrint(result == 0, *tret, "(No.%d) %s%d TRAP_SBO0ME ", testno++, type_to_name(core_type), core_num);	
			}

			util_mem_invalidate(claddr);
		}
	}
}

static void APIPINT_Callback(struct test_params_t *tparam, struct test_result_t *tret)
{
	uint32_t i;
	e_impdrv_errorcode_t ercd;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	st_impdrv_core_info_t *p_core_info;
	e_impdrv_core_type_t core_type;
	uint32_t core_num;
	p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;
	int8_t result = 0;
	uint32_t *claddr;
	void *claddr_phys;
	ssize_t claddr_size;
	bool irq_mask[IMPDRV_IRQMASK_MAX];

	memset(irq_mask, true, sizeof(irq_mask));
	irq_mask[IMPDRV_IRQMASK_APIPINT] = false;

	/* allocate cpu addr CLmem */
	claddr = (uint32_t *)MemAlloc(osal_mmngr, APIPINT_CL_SIZE, 64u, IMP_MALLOC_HW);
	if (NULL == claddr)
	{
		LOGPRINT(("%s: Cannot allocate CL mem\n", __func__));
		goto _finish;
	}
	else
	{
		if (util_mem_get_size(claddr) < (INT_CL_SIZE + (APIPINT_IMAGE_SIZE * APIPINT_IMAGE_NUM)))
		{
			LOGPRINT(("%s: Size allocated invalid\n", __func__));
			goto _dealloc;
		}
	}
	
	/* clean up CL data */
	memset(claddr, 0, util_mem_get_size(claddr));

	/* create CL data */
	createCL_APIPINTtest(claddr);

	/* push to physical address for execution */
	util_mem_flush(claddr);

	/* get physical address of CL memory */
	claddr_phys = util_mem_get_paddr((void *)claddr);

	for (i = 0; i < tparam->initdata.use_core_num; i++)
	{
		if (IMPDRV_CORE_TYPE_IMP == tparam->initdata.core_info[i].core_type)
		{
			result = -1;
			p_core_info = &tparam->initdata.core_info[i];
			core_type = p_core_info->core_type;
			core_num = p_core_info->core_num;

			if (IMPDRV_EC_OK != R_IMPDRV_AttrSetCl(handle, p_core_info, claddr_phys))
			{
				core_testmsg(p_core_info, "Set CL data error\n");
			}
			else if (IMPDRV_EC_OK != R_IMPDRV_AttrSetIrqMask(handle, p_core_info, irq_mask))
			{
				core_testmsg(p_core_info, "Set IRQ mask error\n");
			}
			else
			{
				ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
				if (ercd != IMPDRV_EC_OK)
				{
					core_testmsg(p_core_info, "R_IMPDRV_Execute fail (%d)(test NG)\n", ercd);
				}
				else
				{
					if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_BEFORE_EXEC, -1))
					{
						core_testmsg(p_core_info, "callback (before exec) fail(test NG)\n");
					}
					else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_APIPINT, IMPDRV_CALLBACK_CODE_DEF))
					{
						core_testmsg(p_core_info, "callback (apipint) fail(test NG)\n");
					}
#ifdef TARGET_OS_QNX
					else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_APIPINT, IMPDRV_CALLBACK_CODE_DEF))
					{
						core_testmsg(p_core_info, "callback (apipint) fail(test NG)\n");
					}
#endif
					else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_OK, IMPDRV_CALLBACK_CODE_DEF))
					{
						core_testmsg(p_core_info, "callback(trap) fail(test NG)\n");
					}
					else
					{
						result = 0;
					}
				}
				SuccessTestPrint(result == 0, *tret, "(No.%d) %s%d APIPINT ", testno++, type_to_name(core_type), core_num);	
			}
		}
		(void)R_OSAL_MqReset(g_msgcnf);
	}

	util_mem_invalidate(claddr);

_dealloc:
	MemFree(osal_mmngr, claddr);

_finish:
	return;
}

static void HPINT_Callback(struct test_params_t *tparam, struct test_result_t *tret)
{
	uint32_t i;
	e_impdrv_errorcode_t ercd;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	st_impdrv_core_info_t *p_core_info;
	e_impdrv_core_type_t core_type;
	uint32_t core_num;
	p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;
	int8_t result = 0;
	uint32_t *claddr;
	void *claddr_phys;
	ssize_t claddr_size;
	bool irq_mask[IMPDRV_IRQMASK_MAX];

	memset(irq_mask, true, sizeof(irq_mask));
	irq_mask[IMPDRV_IRQMASK_HPINT] = false;

	/* allocate cpu addr CLmem */
	claddr = (uint32_t *)MemAlloc(osal_mmngr, HPINT_CL_SIZE, 64u, IMP_MALLOC_HW);
	if (NULL == claddr)
	{
		LOGPRINT(("%s: Cannot allocate CL mem\n", __func__));
		goto _finish;
	}
	else
	{
		if (util_mem_get_size(claddr) < (INT_CL_SIZE + (HPINT_IMAGE_SIZE * HPINT_IMAGE_NUM)))
		{
			LOGPRINT(("%s: Size allocated invalid\n", __func__));
			goto _dealloc;
		}
	}
	
	/* clean up CL data */
	memset(claddr, 0, util_mem_get_size(claddr));

	/* create CL data */
	createCL_HPINTtest(claddr);

	/* push to physical address for execution */
	util_mem_flush(claddr);

	/* get physical address of CL memory */
	claddr_phys = util_mem_get_paddr((void *)claddr);

	for (i = 0; i < tparam->initdata.use_core_num; i++)
	{
		if (IMPDRV_CORE_TYPE_IMP == tparam->initdata.core_info[i].core_type)
		{
			result = -1;
			p_core_info = &tparam->initdata.core_info[i];
			core_type = p_core_info->core_type;
			core_num = p_core_info->core_num;

			if (IMPDRV_EC_OK != R_IMPDRV_AttrSetCl(handle, p_core_info, claddr_phys))
			{
				core_testmsg(p_core_info, "Set CL data error\n");
			}
			else if (IMPDRV_EC_OK != R_IMPDRV_AttrSetIrqMask(handle, p_core_info, irq_mask))
			{
				core_testmsg(p_core_info, "Set IRQ mask error\n");
			}
			else
			{
				ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
				if (ercd != IMPDRV_EC_OK)
				{
					core_testmsg(p_core_info, "R_IMPDRV_Execute fail (%d)(test NG)\n", ercd);
				}
				else
				{
					if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_BEFORE_EXEC, -1))
					{
						core_testmsg(p_core_info, "callback (before exec) fail(test NG)\n");
					}
					else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_HPINT, IMPDRV_CALLBACK_CODE_DEF))
					{
						core_testmsg(p_core_info, "callback (hpint) fail(test NG)\n");
					}
#ifdef TARGET_OS_QNX
					else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_HPINT, IMPDRV_CALLBACK_CODE_DEF))
					{
						core_testmsg(p_core_info, "callback (hpint) fail(test NG)\n");
					}
#endif
					else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_OK, IMPDRV_CALLBACK_CODE_DEF))
					{
						core_testmsg(p_core_info, "callback(trap) fail(test NG)\n");
					}
					else
					{
						result = 0;
					}
				}
				SuccessTestPrint(result == 0, *tret, "(No.%d) %s%d HPINT ", testno++, type_to_name(core_type), core_num);	
			}
		}
		(void)R_OSAL_MqReset(g_msgcnf);
	}

	util_mem_invalidate(claddr);

_dealloc:
	MemFree(osal_mmngr, claddr);

_finish:
	return;
}

static void MSCO_INT_Callback(struct test_params_t *tparam, struct test_result_t *tret)
{
	uint32_t i;
	e_impdrv_errorcode_t ercd;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	st_impdrv_core_info_t *p_core_info;
	e_impdrv_core_type_t core_type;
	uint32_t core_num;
	p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;
	st_impdrv_irq_group_t irq_param;
	int8_t result = 0;
	uint32_t *claddr;
	void *claddr_phys;
	ssize_t claddr_size;
	bool irq_mask[IMPDRV_IRQMASK_MAX];

	msco_flg = true;
	msco_int_flg = true;

	memset(irq_mask, true, sizeof(irq_mask));
	irq_mask[IMPDRV_IRQMASK_MSCO] = false;

	/* allocate cpu addr CLmem */
	claddr = (uint32_t *)MemAlloc(osal_mmngr, MSCO_CL_SIZE, 64u, IMP_MALLOC_HW);
	if (NULL == claddr)
	{
		LOGPRINT(("%s: Cannot allocate CL mem\n", __func__));
		goto _finish;
	}
	else
	{
		if (util_mem_get_size(claddr) < (MSCO_INTERNAL_CL_SIZE + MSCO_INTERNAL_SUB_CL_SIZE * 3))
		{
			LOGPRINT(("%s: Size allocated invalid\n", __func__));
			goto _dealloc;
		}
	}

	/* clean up CL data */
	memset(claddr, 0, util_mem_get_size(claddr));

	/* create CL data */
	if (0 != createCL_MSCOtest(claddr))
	{
		LOGPRINT(("%s: Cannot set CL\n", __func__));
		goto _dealloc;
	}

	/* push to physical address for execution */
	util_mem_flush(claddr);

	/* get physical address of CL memory */
	claddr_phys = util_mem_get_paddr((void *)claddr);

	for (i = 0; i < tparam->initdata.use_core_num; i++)
	{
		if (IMPDRV_CORE_TYPE_CNN == tparam->initdata.core_info[i].core_type)
		{
			result = -1;
			p_core_info = &tparam->initdata.core_info[i];
			core_type = p_core_info->core_type;
			core_num = p_core_info->core_num;
			dev_handle = p_impdrv_ctl->coretype_ctl[p_core_info->core_type].core_ctl[p_core_info->core_num].device_io;

			irq_param.irq_group = IMPDRV_IRQ_GROUP_NONE;
			irq_param.group_core_num = 1U;
			irq_param.group_core_info[0].core_type = p_core_info->core_type;
			irq_param.group_core_info[0].core_num = p_core_info->core_num;

			if (IMPDRV_EC_OK != R_IMPDRV_SetIrqGroup(handle, &irq_param))
			{
				core_testmsg(p_core_info, "SetIrqGroup error\n");
			}
			else if (IMPDRV_EC_OK != R_IMPDRV_AttrSetCl(handle, p_core_info, claddr_phys))
			{
				core_testmsg(p_core_info, "Set CL data error\n");
			}
			else if (IMPDRV_EC_OK != R_IMPDRV_AttrSetIrqMask(handle, p_core_info, irq_mask))
			{
				core_testmsg(p_core_info, "Set IRQ mask error\n");
			}
			else
			{
				ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
				if (ercd != IMPDRV_EC_OK)
				{
					core_testmsg(p_core_info, "R_IMPDRV_Execute fail (%d)(test NG)\n", ercd);
				}
				else
				{
					if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_BEFORE_EXEC, -1))
					{
						core_testmsg(p_core_info, "callback (before exec) fail(test NG)\n");
					}
					else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_MSCO, IMPDRV_CALLBACK_CODE_DEF))
					{
						core_testmsg(p_core_info, "callback (msco before int) fail(test NG)\n");
					}
					else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_INT, 0))
					{
						core_testmsg(p_core_info, "callback (int) fail(test NG)\n");
					}
					else if (IMPDRV_EC_OK != R_IMPDRV_ResumeExecution(handle, p_core_info))
					{
						core_testmsg(p_core_info, "R_IMPDRV_ResumeExecution fail (%d)(test NG)\n", ercd);
					}
					else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_MSCO, IMPDRV_CALLBACK_CODE_DEF))
					{
						core_testmsg(p_core_info, "callback (msco after int) fail(test NG)\n");
					}
					else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_OK, 0x2a))
					{
						core_testmsg(p_core_info, "callback(trap) fail(test NG)\n");
					}
					else
					{
						result = 0;
					}
				}

				SuccessTestPrint(result == 0, *tret, "(No.%d) %s%d MSCO ", testno++, type_to_name(core_type), core_num);	
			}
		}
	}

	util_mem_invalidate(claddr);

_dealloc:
	MemFree(osal_mmngr, claddr);

_finish:
	msco_flg = false;
	msco_int_flg = false;
}

static void MSCO_Callback(struct test_params_t *tparam, struct test_result_t *tret)
{
	uint32_t i;
	e_impdrv_errorcode_t ercd;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	st_impdrv_core_info_t *p_core_info;
	e_impdrv_core_type_t core_type;
	uint32_t core_num;
	p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;
	st_impdrv_irq_group_t irq_param;
	int8_t result = 0;
	uint32_t *claddr;
	void *claddr_phys;
	ssize_t claddr_size;
	bool irq_mask[IMPDRV_IRQMASK_MAX];

	msco_flg = true;

	memset(irq_mask, true, sizeof(irq_mask));
	irq_mask[IMPDRV_IRQMASK_MSCO] = false;

	/* allocate cpu addr CLmem */
	claddr = (uint32_t *)MemAlloc(osal_mmngr, MSCO_CL_SIZE, 64u, IMP_MALLOC_HW);
	if (NULL == claddr)
	{
		LOGPRINT(("%s: Cannot allocate CL mem\n", __func__));
		goto _finish;
	}
	else
	{
		if (util_mem_get_size(claddr) < (MSCO_INTERNAL_CL_SIZE + MSCO_INTERNAL_SUB_CL_SIZE * 3))
		{
			LOGPRINT(("%s: Size allocated invalid\n", __func__));
			goto _dealloc;
		}
	}

	/* clean up CL data */
	memset(claddr, 0, util_mem_get_size(claddr));

	/* create CL data */
	if (0 != createCL_MSCOtest(claddr))
	{
		LOGPRINT(("%s: Cannot set CL\n", __func__));
		goto _dealloc;
	}

	/* push to physical address for execution */
	util_mem_flush(claddr);

	/* get physical address of CL memory */
	claddr_phys = util_mem_get_paddr((void *)claddr);

	for (i = 0; i < tparam->initdata.use_core_num; i++)
	{
		if (IMPDRV_CORE_TYPE_CNN == tparam->initdata.core_info[i].core_type)
		{
			result = -1;
			p_core_info = &tparam->initdata.core_info[i];
			core_type = p_core_info->core_type;
			core_num = p_core_info->core_num;
			dev_handle = p_impdrv_ctl->coretype_ctl[p_core_info->core_type].core_ctl[p_core_info->core_num].device_io;

			irq_param.irq_group = IMPDRV_IRQ_GROUP_NONE;
			irq_param.group_core_num = 1U;
			irq_param.group_core_info[0].core_type = p_core_info->core_type;
			irq_param.group_core_info[0].core_num = p_core_info->core_num;

			if (IMPDRV_EC_OK != R_IMPDRV_SetIrqGroup(handle, &irq_param))
			{
				core_testmsg(p_core_info, "SetIrqGroup error\n");
			}
			else if (IMPDRV_EC_OK != R_IMPDRV_AttrSetCl(handle, p_core_info, claddr_phys))
			{
				core_testmsg(p_core_info, "Set CL data error\n");
			}
			else if (IMPDRV_EC_OK != R_IMPDRV_AttrSetIrqMask(handle, p_core_info, irq_mask))
			{
				core_testmsg(p_core_info, "Set IRQ mask error\n");
			}
			else
			{
				ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
				if (ercd != IMPDRV_EC_OK)
				{
					core_testmsg(p_core_info, "R_IMPDRV_Execute fail (%d)(test NG)\n", ercd);
				}
				else
				{
					if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_BEFORE_EXEC, -1))
					{
						core_testmsg(p_core_info, "callback (before exec) fail(test NG)\n");
					}
					else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_MSCO, IMPDRV_CALLBACK_CODE_DEF))
					{
						core_testmsg(p_core_info, "callback (msco) fail(test NG)\n");
					}
					else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_OK, 0x2a))
					{
						core_testmsg(p_core_info, "callback(trap) fail(test NG)\n");
					}
					else
					{
						result = 0;
					}
				}

				SuccessTestPrint(result == 0, *tret, "(No.%d) %s%d MSCO ", testno++, type_to_name(core_type), core_num);	
			}
		}
	}

	util_mem_invalidate(claddr);

_dealloc:
	MemFree(osal_mmngr, claddr);

_finish:
	msco_flg = false;
}

static void END_Callback(struct test_params_t *tparam, struct test_result_t *tret)
{
	uint32_t i;
	e_impdrv_errorcode_t ercd;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	st_impdrv_core_info_t *p_core_info;
	e_impdrv_core_type_t core_type;
	uint32_t core_num;
	p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;
	int8_t result = 0;
	bool irq_mask[IMPDRV_IRQMASK_MAX];
	END_cl_data_t end_cl_data;

	memset(irq_mask, true, sizeof(irq_mask));
	irq_mask[IMPDRV_IRQMASK_END] = false;

	if (0 != createCL_ENDtest(&end_cl_data))
	{
		LOGPRINT(("%s: Cannot set CL\n", __func__));
		goto _finish;
	}

	/* push to physical address for execution */
	util_mem_flush(end_cl_data.imga_cpu_addr);
	util_mem_flush(end_cl_data.dst0_cpu_addr);
	util_mem_flush(end_cl_data.dst1_cpu_addr);
	util_mem_flush(end_cl_data.dst2_cpu_addr);
	util_mem_flush(end_cl_data.dst3_cpu_addr);
	util_mem_flush(end_cl_data.cl_cpu_addr);

	for (i = 0; i < tparam->initdata.use_core_num; i++)
	{
		if (IMPDRV_CORE_TYPE_PSCEXE == tparam->initdata.core_info[i].core_type)
		{
			result = -1;
			p_core_info = &tparam->initdata.core_info[i];
			core_type = p_core_info->core_type;
			core_num = p_core_info->core_num;

			if (IMPDRV_EC_OK != R_IMPDRV_AttrSetCl(handle, p_core_info, util_mem_get_paddr(end_cl_data.cl_cpu_addr)))
			{
				core_testmsg(p_core_info, "Set CL data error\n");
			}
			else if (IMPDRV_EC_OK != R_IMPDRV_AttrSetIrqMask(handle, p_core_info, irq_mask))
			{
				core_testmsg(p_core_info, "Set IRQ mask error\n");
			}
			else
			{
				ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
				if (ercd != IMPDRV_EC_OK)
				{
					core_testmsg(p_core_info, "R_IMPDRV_Execute fail (%d)(test NG)\n", ercd);
				}
				else
				{
					if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_BEFORE_EXEC, -1))
					{
						core_testmsg(p_core_info, "callback (before exec) fail(test NG)\n");
					}
					else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_END, IMPDRV_CALLBACK_CODE_DEF))
					{
						core_testmsg(p_core_info, "callback (end) fail(test NG)\n");
					}
					else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_OK, 0))
					{
						core_testmsg(p_core_info, "callback(trap) fail(test NG)\n");
					}
					else
					{
						result = 0;
					}
				}

				SuccessTestPrint(result == 0, *tret, "(No.%d) %s%d END ", testno++, type_to_name(core_type), core_num);	
			}
		}
	}

_finish:
	util_mem_invalidate(end_cl_data.imga_cpu_addr);
	util_mem_invalidate(end_cl_data.dst0_cpu_addr);
	util_mem_invalidate(end_cl_data.dst1_cpu_addr);
	util_mem_invalidate(end_cl_data.dst2_cpu_addr);
	util_mem_invalidate(end_cl_data.dst3_cpu_addr);
	util_mem_invalidate(end_cl_data.cl_cpu_addr);

	MemFree(osal_mmngr, end_cl_data.imga_cpu_addr);
	MemFree(osal_mmngr, end_cl_data.dst0_cpu_addr);
	MemFree(osal_mmngr, end_cl_data.dst1_cpu_addr);
	MemFree(osal_mmngr, end_cl_data.dst2_cpu_addr);
	MemFree(osal_mmngr, end_cl_data.dst3_cpu_addr);
	MemFree(osal_mmngr, end_cl_data.cl_cpu_addr);
}

static void USIER_Callback(struct test_params_t *tparam, struct test_result_t *tret)
{
	uint32_t i, j;
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
    impdrv_ctrl_handle_t handle;
	st_impdrv_ctl_t impdrv_ctl;
	st_impdrv_core_info_t *p_core_info;
	e_impdrv_core_type_t core_type;
	uint32_t core_num;
	p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;
	int8_t result;
	void *claddr;
	uint32_t claddr_phys;
	uint32_t maxsbo = 1;
	st_impdrv_core_info_t *p_core_map;
	bool irq_mask[IMPDRV_IRQMASK_MAX];

	memset(&impdrv_ctl, 0, sizeof(impdrv_ctl));
	for (i = 0; i < IMPDRV_CORE_TYPE_MAX; i++)
	{
		for (j = 0; j < IMPDRV_CORE_NUM_MAX; j++)
		{
			impdrv_ctl.coretype_ctl[i].core_ctl[j].core_state = IMPDRV_STATE_UNINIT;
		}
	}

	memset(irq_mask, true, sizeof(irq_mask));
	irq_mask[IMPDRV_IRQMASK_USIER] = false;

	claddr = createCL_USIER(maxsbo);
	claddr_phys = util_mem_get_paddr(claddr);

	set_input_data_before_init(tparam);
	tparam->initdata.p_work_addr = (void *)&impdrv_ctl;
	tparam->initdata.work_size = sizeof(impdrv_ctl);
	p_core_map = tparam->core_map;

	if (IMPDRV_EC_OK != (ercd = R_IMPDRV_Init(p_initdata, &handle)))
	{
		testmsg("R_IMPDRV_Init fail (%d)(test NG)\n", ercd);
	}
	else
	{	
		for (i = 0; i < tparam->initdata.use_core_num; i++)
		{
			if (IMPDRV_CORE_TYPE_OCV == tparam->initdata.core_info[i].core_type)
			{
				result = -1;
				p_core_info = &tparam->initdata.core_info[i];
				core_type = p_core_info->core_type;
				core_num = p_core_info->core_num;

				if (IMPDRV_EC_OK != (ercd = R_IMPDRV_AttrInit(handle, p_core_info)))
				{
					core_testmsg(p_core_info, "R_IMPDRV_AttrInit fail (%d)(test NG)\n", ercd);
				}
				else if (IMPDRV_EC_OK != (ercd = R_IMPDRV_Start(handle, p_core_info)))
				{
					core_testmsg(p_core_info, "R_IMPDRV_Start fail (%d)(test NG)\n", ercd);
				}
				else if (IMPDRV_EC_OK != (ercd = R_IMPDRV_AttrSetCoreMap(handle, p_core_info, p_core_map)))
				{
					core_testmsg(p_core_info, "R_IMPDRV_AttrSetCoreMap fail (%d)(test NG)\n", ercd);
				}
				else if (IMPDRV_EC_OK != (ercd = R_IMPDRV_AttrSetCl(handle, p_core_info, claddr_phys)))
				{
					core_testmsg(p_core_info, "R_IMPDRV_AttrSetCl fail (%d)(test NG)\n", ercd);
				}
				else if (IMPDRV_EC_OK != (ercd = R_IMPDRV_AttrSetIrqMask(handle, p_core_info, irq_mask)))
				{
					core_testmsg(p_core_info, "R_IMPDRV_AttrSetIrqMask fail (%d)(test NG)\n", ercd);
				}
				else if (IMPDRV_EC_OK != (ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args)))
				{
					core_testmsg(p_core_info, "R_IMPDRV_Execute fail (%d)(test NG)\n", ercd);
				}
				else if ((wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_BEFORE_EXEC, -1)) ||
						(wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_USIER, -1)))
				{
					core_testmsg(p_core_info, "callback fail(test NG)\n");
				}
				else
				{
					result = 0;
				}

				if (IMPDRV_EC_OK != (ercd = R_IMPDRV_Stop(handle, p_core_info)))
				{
					core_testmsg(p_core_info, "R_IMPDRV_Stop fail (%d)(test NG)\n", ercd);
				}

				SuccessTestPrint(result == 0, *tret, "(No.%d) %s%d USIER ", testno++, type_to_name(core_type), core_num);	
			}
		}
	}

	ercd = R_IMPDRV_Quit(handle);
	if (IMPDRV_EC_OK != ercd)
	{
		testmsg("R_IMPDRV_Quit fail (%d)(test NG)\n", ercd);
	}

	util_mem_invalidate(claddr);
	MemFree(osal_mmngr, claddr);
}

static void USINT_Callback(struct test_params_t *tparam, struct test_result_t *tret)
{
	uint32_t i, j;
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	st_impdrv_core_info_t *p_core_info;
	e_impdrv_core_type_t core_type;
	uint32_t core_num;
	p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;
	int8_t result;
	void *claddr;
	uint32_t claddr_phys;
	bool irq_mask[IMPDRV_IRQMASK_MAX];

	memset(irq_mask, true, sizeof(irq_mask));
	irq_mask[IMPDRV_IRQMASK_USINT] = false;

	/* allocate cpu addr CLmem */
	claddr = (uint32_t *)MemAlloc(osal_mmngr, USINT_CL_SIZE, 64u, IMP_MALLOC_HW);
	if (NULL == claddr)
	{
		LOGPRINT(("%s: Cannot allocate CL mem\n", __func__));
		goto _finish;
	}
	else
	{
		if (util_mem_get_size(claddr) < 0x3000)
		{
			LOGPRINT(("%s: Size allocated invalid\n", __func__));
			goto _dealloc;
		}
	}
	
	/* clean up CL data */
	memset(claddr, 0, util_mem_get_size(claddr));

	/* create CL data */
	createCL_USINTtest(claddr);

	/* push to physical address for execution */
	util_mem_flush(claddr);

	/* get physical address of CL memory */
	claddr_phys = util_mem_get_paddr((void *)claddr);

	for (i = 0; i < tparam->initdata.use_core_num; i++)
	{
		if (IMPDRV_CORE_TYPE_OCV == tparam->initdata.core_info[i].core_type)
		{
			result = -1;
			p_core_info = &tparam->initdata.core_info[i];
			core_type = p_core_info->core_type;
			core_num = p_core_info->core_num;

			if (IMPDRV_EC_OK != (ercd = R_IMPDRV_AttrSetCl(handle, p_core_info, claddr_phys)))
			{
				core_testmsg(p_core_info, "R_IMPDRV_AttrSetCl fail (%d)(test NG)\n", ercd);
			}
			else if (IMPDRV_EC_OK != (ercd = R_IMPDRV_AttrSetIrqMask(handle, p_core_info, irq_mask)))
			{
				core_testmsg(p_core_info, "R_IMPDRV_AttrSetIrqMask fail (%d)(test NG)\n", ercd);
			}
			else if (IMPDRV_EC_OK != (ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args)))
			{
				core_testmsg(p_core_info, "R_IMPDRV_Execute fail (%d)(test NG)\n", ercd);
			}
			else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_BEFORE_EXEC, -1))
			{
				core_testmsg(p_core_info, "callback (before exec) fail(test NG)\n");
			}
			else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_USINT, -1))
			{
				core_testmsg(p_core_info, "callback (usint) fail(test NG)\n");
			}
			else if (IMPDRV_EC_OK != (ercd = R_IMPDRV_ResumeExecution(handle, p_core_info)))
			{
				core_testmsg(p_core_info, "R_IMPDRV_ResumeExecution fail (%d)(test NG)\n", ercd);
			}
			else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_OK, 0))
			{
				core_testmsg(p_core_info, "callback(trap) fail(test NG)\n");
			}
			else
			{
				result = 0;
			}

			SuccessTestPrint(result == 0, *tret, "(No.%d) %s%d USINT ", testno++, type_to_name(core_type), core_num);	
		}
	}

	util_mem_invalidate(claddr);

_dealloc:
	MemFree(osal_mmngr, claddr);

_finish:
	return;
}

static void WUPCOVF_Callback(struct test_params_t *tparam, struct test_result_t *tret)
{
	int8_t i, j;
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	st_impdrv_irq_group_t irq_param;
	p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;
	int32_t result = 0;
	bool irq_mask[IMPDRV_IRQMASK_MAX];
	st_impdrv_core_info_t core_map[IMPDRV_COREMAP_MAXID];
	st_impdrv_core_info_t *p_pair_core;
	st_impdrv_core_info_t *p_core_info;
	void *claddr[WUPCOVF_CORE_NUM];
	st_impdrv_core_info_t exe_core[WUPCOVF_CORE_NUM] = {
        { IMPDRV_CORE_TYPE_CNN, 0u },
        { IMPDRV_CORE_TYPE_OCV, 0u },
        { IMPDRV_CORE_TYPE_PSCEXE, 0u },
        { IMPDRV_CORE_TYPE_DMAC, 0u },

        { IMPDRV_CORE_TYPE_IMP, 1u }	/* pair core */
    };

	memset(irq_mask, true, sizeof(irq_mask));
	irq_mask[IMPDRV_IRQMASK_WUPCOVF] = false;

	/******************************************************************************************
	 * Start of data preparation
	 *****************************************************************************************/
	/* initialize core_map */
	for (i = 0; i < IMPDRV_COREMAP_MAXID; i++)
	{
		core_map[i].core_type = IMPDRV_CORE_TYPE_INVALID;
		core_map[i].core_num = 0 ;
	}
	p_pair_core = &exe_core[WUPCOVF_CORE_NUM - 1];

	/******************************************************************************************
	 * End of data preparation
	 *****************************************************************************************/

	/******************************************************************************************
	 * Start of CL data preparation
	 *****************************************************************************************/
	for (i = 0; i < WUPCOVF_CORE_NUM; i++)
	{
		claddr[i] = MemAlloc(osal_mmngr, WUPCOVF_CL_SIZE, 64u, IMP_MALLOC_HW);
		if (NULL == claddr[i])
		{
			LOGPRINT(("%s: Cannot allocate CL[%d] mem\n", __func__, i));
			break;
		}
		else
		{
			memset(claddr[i], 0, util_mem_get_size(claddr[i]));
			if ((WUPCOVF_CORE_NUM - 1) == i)
			{
				result = createCLpair_WUPCOVFtest(claddr[i], 0x0001u);
			}
			else
			{
				result = createCL_WUPCOVFtest(claddr[i], exe_core[i].core_type, 0x0002u);
			}

			if (0 != result)
			{
				LOGPRINT(("<< [ERROR] %s: fail to set CL[%d] >>\n", __func__, i)); /* cast to int32_t */
				break;
			}
		}
	}

	if (0 != result)
	{
		goto _finish;
	}

	/* push to physical address for execution */
	for (i = 0; i < WUPCOVF_CORE_NUM; i++)
	{
		util_mem_flush(claddr[i]);
	}

	/******************************************************************************************
	 * End of data preparation
	 *****************************************************************************************/

	/******************************************************************************************
	 * Start of CL data preparation
	 *****************************************************************************************/
	for (i = 0; i < (WUPCOVF_CORE_NUM - 1); i++)
	{
		p_core_info = &exe_core[i];
		p_pair_core = &exe_core[WUPCOVF_CORE_NUM - 1];
		result = 0;

#if defined (V3H1)
		if ((IMPDRV_CORE_TYPE_IMP == p_core_info->core_type) ||
			(IMPDRV_CORE_TYPE_PSCEXE == p_core_info->core_type) ||
			(IMPDRV_CORE_TYPE_CNN == p_core_info->core_type))
		{
			testno++;
			continue;
		}
#elif defined (V4H2)
		if (IMPDRV_CORE_TYPE_CNN == p_core_info->core_type)
		{
			testno++;
			continue;
		}
#endif

		/* set IRQ group */
		irq_param.irq_group						= IMPDRV_IRQ_GROUP_NONE;
		irq_param.group_core_num				= 1u;
		irq_param.group_core_info[0].core_type	= p_core_info->core_type;
		irq_param.group_core_info[0].core_num	= p_core_info->core_num;
		ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
		if (IMPDRV_EC_OK != ercd)
		{
			core_testmsg(p_core_info, "set IQG group fail\n");
			result = -1;
		}

		/* set core_map for pair core */
		core_map[0].core_type	= exe_core[i].core_type;
		core_map[0].core_num	= exe_core[i].core_num;
		core_map[1].core_type	= p_pair_core->core_type;
		core_map[1].core_num	= p_pair_core->core_num;
		ercd = R_IMPDRV_AttrSetCoreMap(handle, p_pair_core, core_map);
		if (IMPDRV_EC_OK != ercd)
		{
			core_testmsg(p_pair_core, "set core_map fail at pair core %s%d\n", type_to_name(p_pair_core->core_type), p_pair_core->core_num);
			result = -1;
		}

		/* set IRQ mask for pair core */
		ercd = R_IMPDRV_AttrSetIrqMask(handle, p_pair_core, irq_mask);
		if (IMPDRV_EC_OK != ercd)
		{
			core_testmsg(p_pair_core, "set IRQ mask fail at pair core %s%d\n", type_to_name(p_pair_core->core_type), p_pair_core->core_num);
			result = -1;
		}

		/* Set CL for pair core */
		ercd = R_IMPDRV_AttrSetCl(handle, p_pair_core, util_mem_get_paddr(claddr[WUPCOVF_CORE_NUM - 1]));
		if (IMPDRV_EC_OK != ercd)
		{
			core_testmsg(p_pair_core, "set CL fail at pair core %s%d\n", type_to_name(p_pair_core->core_type), p_pair_core->core_num);
			result = -1;
		}

		/* execute pair core */
		ercd = R_IMPDRV_Execute(handle, p_pair_core, callback_func, (void *)&callback_args);
		if (IMPDRV_EC_OK != ercd)
		{
			core_testmsg(p_pair_core, "execute fail at pair core %s%d\n", type_to_name(p_pair_core->core_type), p_pair_core->core_num);
			result = -1;
		}
		else if (wait_and_check_callback(g_msgcnf, p_pair_core, IMPDRV_CB_RET_BEFORE_EXEC, -1))
		{
			core_testmsg(p_pair_core, "callback(before exec) fail at pair core %s%d\n", type_to_name(p_pair_core->core_type), p_pair_core->core_num);
			result = -1;
		}

		/* set core_map for the core */
		core_map[0].core_type	= exe_core[i].core_type;
		core_map[0].core_num	= exe_core[i].core_num;
		core_map[1].core_type	= p_pair_core->core_type;
		core_map[1].core_num	= p_pair_core->core_num;
		ercd = R_IMPDRV_AttrSetCoreMap(handle, p_core_info, core_map);
		if (IMPDRV_EC_OK != ercd)
		{
			core_testmsg(p_core_info, "set core_map fail at core %s%d\n", type_to_name(p_core_info->core_type), p_core_info->core_num);
			result = -1;
		}

		/* set core_map for core */
		ercd = R_IMPDRV_AttrSetIrqMask(handle, p_core_info, irq_mask);
		if (IMPDRV_EC_OK != ercd)
		{
			core_testmsg(p_core_info, "set IRQ mask fail at core %s%d\n", type_to_name(p_core_info->core_type), p_core_info->core_num);
			result = -1;
		}

		/* set CL for core */
		ercd = R_IMPDRV_AttrSetCl(handle, p_core_info, util_mem_get_paddr(claddr[i]));
		if (IMPDRV_EC_OK != ercd)
		{
			core_testmsg(p_core_info, "set CL fail at core %s%d\n", type_to_name(p_core_info->core_type), p_core_info->core_num);
			result = -1;
		}

		/* execute the core */
		ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
		if (IMPDRV_EC_OK != ercd)
		{
			core_testmsg(p_core_info, "execute fail at core %s%d\n", type_to_name(p_core_info->core_type), p_core_info->core_num);
			result = -1;
		}
		else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_BEFORE_EXEC, -1))
		{
			core_testmsg(p_core_info, "callback(before exec) fail at core %s%d\n", type_to_name(p_core_info->core_type), p_core_info->core_num);
			result = -1;
		}
		else if (wait_and_check_callback(g_msgcnf, p_pair_core, IMPDRV_CB_RET_OK, -1))
		{
			core_testmsg(p_pair_core, "callback(trap) fail at pair core %s%d\n", type_to_name(p_pair_core->core_type), p_pair_core->core_num);
			result = -1;
		}
		else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_WUPCOVF, -1))
		{
			core_testmsg(p_core_info, "callback(wupcovf) fail at core %s%d\n", type_to_name(p_core_info->core_type), p_core_info->core_num);
			result = -1;
		}
		else
		{
			int32_t code;

			switch (p_core_info->core_type)
			{
				case IMPDRV_CORE_TYPE_IMP:
				{
					code = -1;
					break;
				}
    			case IMPDRV_CORE_TYPE_OCV:
				case IMPDRV_CORE_TYPE_DMAC:
				case IMPDRV_CORE_TYPE_PSCEXE:
				{
					code = 0;
					break;
				}
				case IMPDRV_CORE_TYPE_CNN:
				{
					code = 2;
					break;
				}
				default:
				{
					result = -1;
					break;
				}
			}
			
			if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_OK, code))
			{
				core_testmsg(p_core_info, "callback(trap) fail at core %s%d\n", type_to_name(p_core_info->core_type), p_core_info->core_num);
				result = -1;
			}
		}

		SuccessTestPrint(result == 0, *tret, "(No.%d) %s%d ", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num);
	}

	/* stop cores */
_invalidate_cl:
	for (i = 0; i < WUPCOVF_CORE_NUM; i++)
	{
		util_mem_invalidate(claddr[i]);
	}

_dealloc:
	for (i = 0; i < WUPCOVF_CORE_NUM; i++)
	{
		MemFree(osal_mmngr, claddr[i]);
	}

_finish:
	return;
}

static int32_t CbFunc_IMP0(
	const st_impdrv_core_info_t *p_core_info,		/**< Core information					*/
	const e_impdrv_cb_ret_t		ercd,				/**< Error code							*/
	const int32_t				code,				/**< TRAP or INT code (8 bit is set)	*/
	const void					*p_callback_args
)
{
	int32_t ret;

	ret = send_msg(g_msgcnf_imp_wupcovf[0], ercd) | send_msg(g_msgcnf_imp_wupcovf[0], code);

	return ret;
}

static int32_t CbFunc_IMP1(
	const st_impdrv_core_info_t *p_core_info,		/**< Core information					*/
	const e_impdrv_cb_ret_t		ercd,				/**< Error code							*/
	const int32_t				code,				/**< TRAP or INT code (8 bit is set)	*/
	const void					*p_callback_args
)
{
	int32_t ret;

	ret = send_msg(g_msgcnf_imp_wupcovf[1], ercd) | send_msg(g_msgcnf_imp_wupcovf[1], code);

	return ret;
}

static int32_t CbFunc_IMP2(
	const st_impdrv_core_info_t *p_core_info,		/**< Core information					*/
	const e_impdrv_cb_ret_t		ercd,				/**< Error code							*/
	const int32_t				code,				/**< TRAP or INT code (8 bit is set)	*/
	const void					*p_callback_args
)
{
	int32_t ret;

	ret = send_msg(g_msgcnf_imp_wupcovf[2], ercd) | send_msg(g_msgcnf_imp_wupcovf[2], code);

	return ret;
}

static int32_t (*p_CbFunc[IMP_EXE_CORE_NUM])(
	const st_impdrv_core_info_t *p_core_info,		/**< Core information					*/
	const e_impdrv_cb_ret_t		ercd,				/**< Error code							*/
	const int32_t				code,				/**< TRAP or INT code (8 bit is set)	*/
	const void					*p_callback_args
) = {
	[0]	=	&CbFunc_IMP0,
	[1]	=	&CbFunc_IMP1,
	[2]	=	&CbFunc_IMP2
};

static void WUPCOVF_IMP_Callback(struct test_params_t *tparam, struct test_result_t *tret)
{
	int8_t i, j;
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	st_impdrv_irq_group_t irq_param;
	uint32_t callback_args;
	int32_t result = 0;
	bool irq_mask[IMPDRV_IRQMASK_MAX];
	st_impdrv_core_info_t core_map[IMPDRV_COREMAP_MAXID];
	st_impdrv_core_info_t *p_core_info;
	void *claddr[IMP_EXE_CORE_NUM];
	st_impdrv_core_info_t exe_core[IMP_EXE_CORE_NUM] = {
        { IMPDRV_CORE_TYPE_IMP, 0u },
        { IMPDRV_CORE_TYPE_IMP, 1u },
		{ IMPDRV_CORE_TYPE_IMP, 2u }
    };
	size_t cl_size = 1024U;

	memset(&irq_param, 0, sizeof(irq_param));
	memset(irq_mask, true, sizeof(irq_mask));
	irq_mask[IMPDRV_IRQMASK_WUPCOVF] = false;

	/******************************************************************************************
	 * Start of data preparation
	 *****************************************************************************************/
	/* initialize core_map */
	for (i = 0; i < IMPDRV_COREMAP_MAXID; i++)
	{
		core_map[i].core_type = IMPDRV_CORE_TYPE_INVALID;
		core_map[i].core_num = 0 ;
	}

	for (i = 0; i < IMP_EXE_CORE_NUM; i++)
	{
		core_map[i].core_type = IMPDRV_CORE_TYPE_IMP;
		core_map[i].core_num = (uint32_t)i ;
	}

	irq_param.irq_group			= IMPDRV_IRQ_GROUP_NONE;
	irq_param.group_core_num	= 1u;

	/******************************************************************************************
	 * End of data preparation
	 *****************************************************************************************/

	/******************************************************************************************
	 * Start of CL data preparation
	 *****************************************************************************************/
	for (i = 0; i < IMP_EXE_CORE_NUM; i++)
	{
		claddr[i] = MemAlloc(osal_mmngr, cl_size, 64u, IMP_MALLOC_HW);
		if (NULL == claddr[i])
		{
			LOGPRINT(("%s: Cannot allocate CL[%d] mem\n", __func__, i));
			break;
		}
		else
		{
			memset(claddr[i], 0, util_mem_get_size(claddr[i]));
			result = set_cl_data_for_wupcovf_imp(&exe_core[i], (uint32_t *)claddr[i]);
			if (0 != result)
			{
				break;
			}
		}
	}

	if (0 != result)
	{
		goto _finish;
	}

	/* push to physical address for execution */
	for (i = 0; i < IMP_EXE_CORE_NUM; i++)
	{
		util_mem_flush(claddr[i]);
	}

	/******************************************************************************************
	 * End of data preparation
	 *****************************************************************************************/

	/******************************************************************************************
	 * Start of CL data preparation
	 *****************************************************************************************/
	for (i = 0; i < IMP_EXE_CORE_NUM; i++)
	{
		p_core_info = &exe_core[i];
		result = 0;

		/* set IRQ group */
		for (j = 0; j < irq_param.group_core_num; j++)
		{
			irq_param.group_core_info[j].core_type	= p_core_info->core_type;
			irq_param.group_core_info[j].core_num	= p_core_info->core_num;
		}
		ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
		if (IMPDRV_EC_OK != ercd)
		{
			core_testmsg(p_core_info, "set IQG group fail\n");
			result = -1;
		}

		/* set core_map for core */
		ercd = R_IMPDRV_AttrSetCoreMap(handle, p_core_info, core_map);
		if (IMPDRV_EC_OK != ercd)
		{
			core_testmsg(p_core_info, "set core_map fail at core %s%d\n", type_to_name(p_core_info->core_type), p_core_info->core_num);
			result = -1;
		}

		/* set IRQ mask for core */
		ercd = R_IMPDRV_AttrSetIrqMask(handle, p_core_info, irq_mask);
		if (IMPDRV_EC_OK != ercd)
		{
			core_testmsg(p_core_info, "set IRQ mask fail at core %s%d\n", type_to_name(p_core_info->core_type), p_core_info->core_num);
			result = -1;
		}

		/* Set CL for core */
		ercd = R_IMPDRV_AttrSetCl(handle, p_core_info, util_mem_get_paddr(claddr[i]));
		if (IMPDRV_EC_OK != ercd)
		{
			core_testmsg(p_core_info, "set CL fail at core %s%d\n", type_to_name(p_core_info->core_type), p_core_info->core_num);
			result = -1;
		}
		// waiting for WUPCOVF of IMP1 
		if (2 == p_core_info->core_num)
		{
			R_OSAL_ThreadSleepForTimePeriod(400);
		}

		/* execute core */
		ercd = R_IMPDRV_Execute(handle, p_core_info, (p_impdrv_cbfunc_t)p_CbFunc[i], (void *)&callback_args);
		if (IMPDRV_EC_OK != ercd)
		{
			core_testmsg(p_core_info, "execute fail at core %s%d\n", type_to_name(p_core_info->core_type), p_core_info->core_num);
			result = -1;
		}
		else
		{
			result = 0;
		}

		if (0 != result)
		{
			break;
		}
	}

	if (0 == result)
	{
		for (i = 0; i < IMP_EXE_CORE_NUM; i++)
		{
			p_core_info = &exe_core[i];

			if (0U == p_core_info->core_num)
			{
				if (wait_and_check_callback(g_msgcnf_imp_wupcovf[i], p_core_info, IMPDRV_CB_RET_BEFORE_EXEC, IMPDRV_CALLBACK_CODE_DEF))
				{
					core_testmsg(p_core_info, "callback(before exec) fail at core %s%d\n", type_to_name(p_core_info->core_type), p_core_info->core_num);
					result |= -1;
				}
				else if (wait_and_check_callback(g_msgcnf_imp_wupcovf[i], p_core_info, IMPDRV_CB_RET_WUPCOVF, IMPDRV_CALLBACK_CODE_DEF))
				{
					core_testmsg(p_core_info, "callback(wupcovf) fail at core %s%d\n", type_to_name(p_core_info->core_type), p_core_info->core_num);
					result |= -1;
				}
				else if (wait_and_check_callback(g_msgcnf_imp_wupcovf[i], p_core_info, IMPDRV_CB_RET_WUPCOVF, IMPDRV_CALLBACK_CODE_DEF))
				{
					core_testmsg(p_core_info, "callback(wupcovf) fail at core %s%d\n", type_to_name(p_core_info->core_type), p_core_info->core_num);
					result |= -1;
				}
				else if (wait_and_check_callback(g_msgcnf_imp_wupcovf[i], p_core_info, IMPDRV_CB_RET_OK, IMPDRV_CALLBACK_CODE_DEF))
				{
					core_testmsg(p_core_info, "callback(trap) fail at core %s%d\n", type_to_name(p_core_info->core_type), p_core_info->core_num);
					result |= -1;
				}
				else
				{
					result |= 0;
				}
			}
			else
			{
				if (wait_and_check_callback(g_msgcnf_imp_wupcovf[i], p_core_info, IMPDRV_CB_RET_BEFORE_EXEC, IMPDRV_CALLBACK_CODE_DEF))
				{
					core_testmsg(p_core_info, "callback(before exec) fail at core %s%d\n", type_to_name(p_core_info->core_type), p_core_info->core_num);
					result |= -1;
				}
				else if (wait_and_check_callback(g_msgcnf_imp_wupcovf[i], p_core_info, IMPDRV_CB_RET_OK, IMPDRV_CALLBACK_CODE_DEF))
				{
					core_testmsg(p_core_info, "callback(trap) fail at core %s%d\n", type_to_name(p_core_info->core_type), p_core_info->core_num);
					result |= -1;
				}
				else
				{
					result |= 0;
				}
			}
		}
	}

	SuccessTestPrint(result == 0, *tret, "(No.%d) %s ", testno++, type_to_name(p_core_info->core_type));

	/* stop cores */
_invalidate_cl:
	for (i = 0; i < IMP_EXE_CORE_NUM; i++)
	{
		util_mem_invalidate(claddr[i]);
	}

_dealloc:
	for (i = 0; i < IMP_EXE_CORE_NUM; i++)
	{
		MemFree(osal_mmngr, claddr[i]);
	}

_finish:
	return;
}