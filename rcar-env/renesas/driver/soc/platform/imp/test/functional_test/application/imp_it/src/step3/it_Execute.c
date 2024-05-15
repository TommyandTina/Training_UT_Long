#define TEST_MSG_NAME "R_IMPDRV_Execute"

#include "it_common.h"
#include "it_stub.h"

#define CALLBACK_CODE 0xAA

static void set_state(st_impdrv_initdata_t *const p_data, e_impdrv_state_t state);
void prepare_test_for_execute(impdrv_ctrl_handle_t handle, const st_impdrv_core_info_t *const p_core_info, const uint32_t claddr_phys);
#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret);
static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret);
static void Return_error(struct test_params_t *tparam, struct test_result_t *tret);
#endif
static void Core_exec(struct test_params_t *tparam, struct test_result_t *tret);

static int32_t check_callback_code;
static e_impdrv_cb_ret_t check_callback_ercd;
static uint32_t testno;
static st_impdrv_coretypectl_t coretype_ctl_backup[IMPDRV_CORE_TYPE_MAX];
extern uintptr_t clbrk_addr;
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

void prepare_test_for_execute(
	impdrv_ctrl_handle_t		handle,
	const st_impdrv_core_info_t	*const p_core_info,
	const uint32_t				claddr_phys
)
{
	st_impdrv_ctl_t			*p_impdrv_ctl = (st_impdrv_ctl_t *)handle;
	e_impdrv_core_type_t	core_type = p_core_info->core_type;
    uint32_t                core_num = p_core_info->core_num;
	st_impdrv_corectl_t		*p_corectl = &p_impdrv_ctl->coretype_ctl[core_type].core_ctl[core_num];
	uint32_t mask_val;
	uint32_t i;
	uint32_t j;
	uint32_t syncc_val[IMPDRV_SYNCC_REGNUM];

	if (IMPDRV_CORE_TYPE_IMP == core_type)
	{
		i = 0;
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_IMP_IFCFG_VAL);
		get_syncc_val(p_corectl, syncc_val);
		for (j = 0; IMPDRV_SYNCC_REGNUM > j; j++)
		{
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, syncc_val[j]);
		}
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, clbrk_addr);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_IMP_CLCNDGSBR_MASK);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, imp_get_inten_val(p_corectl->irq_mask));
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, claddr_phys);
	}
#if defined (V3H2) || defined (V3H1)
    else if (IMPDRV_CORE_TYPE_IMP_SLIM == core_type)
	{
		i = 0;
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_IMPS_IFCFG_VAL);
		get_syncc_val(p_corectl, syncc_val);
		for (j = 0; IMPDRV_SYNCC_REGNUM > j; j++)
		{
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, syncc_val[j]);
		}
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, imps_get_inten_val(p_corectl->irq_mask));
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, claddr_phys);
	}
#endif
	else if (IMPDRV_CORE_TYPE_OCV == core_type)
	{
		i = 0;
		mask_val = get_irq_mask_val(core_type);

		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0);
		get_syncc_val(p_corectl, syncc_val);
		for (j = 0; IMPDRV_SYNCC_REGNUM > j; j++)
		{
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, syncc_val[j]);
		}
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, clbrk_addr);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_OCV_CLCNDGSBR_MASK);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_OCV_ICR2_MASK);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, ocv_get_inten_val(p_corectl->irq_mask));
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, claddr_phys);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, (IMPDRV_OCV_MCR0_DCBANKSEL1 | IMPDRV_OCV_MCR0_DCBANKSEL0));
	}
	else if (IMPDRV_CORE_TYPE_DMAC == core_type)
	{
		i = 0;
		mask_val = get_irq_mask_val(core_type);

		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0);
		get_syncc_val(p_corectl, syncc_val);
		for (j = 0; IMPDRV_SYNCC_REGNUM > j; j++)
		{
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, syncc_val[j]);
		}
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, clbrk_addr);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_DMA_CLCNDGSBR_MASK);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, (dmac_get_inten_val(p_corectl->irq_mask)) | IMPDRV_DMA_SER_FIX);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, (mask_val & ~dmac_get_inten_val(p_corectl->irq_mask)) | IMPDRV_DMA_SER_FIX);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, claddr_phys);
	}
#if defined (V3H2)
	else if (IMPDRV_CORE_TYPE_DMAC_SLIM == core_type)
	{
		i = 0;
		mask_val = get_irq_mask_val(core_type);

		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0);
		get_syncc_val(p_corectl, syncc_val);
		for (j = 0; IMPDRV_SYNCC_REGNUM > j; j++)
		{
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, syncc_val[j]);
		}
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, clbrk_addr);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_DMA_CLCNDGSBR_MASK);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, (mask_val & ~IMPDRV_DMAS_SR_TEND) | IMPDRV_DMAS_SER_FIX);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, (mask_val & ~dmacs_get_inten_val(p_corectl->irq_mask)) | IMPDRV_DMAS_SER_FIX);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, claddr_phys);
	}
#endif
	else if (IMPDRV_CORE_TYPE_PSCEXE == core_type)
	{
		i = 0;
		mask_val = get_irq_mask_val(core_type);

		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0);
		get_syncc_val(p_corectl, syncc_val);
		for (j = 0; IMPDRV_SYNCC_REGNUM > j; j++)
		{
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, syncc_val[j]);
		}
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, clbrk_addr);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_PSC_CLCNDGSBR_MASK);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, psc_get_inten_val(p_corectl->irq_mask));
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, mask_val & ~psc_get_inten_val(p_corectl->irq_mask));
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, claddr_phys);
	}
	else if (IMPDRV_CORE_TYPE_CNN == core_type)
	{
		i = 0;
		mask_val = get_irq_mask_val(core_type);

		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0);
		get_syncc_val(p_corectl, syncc_val);
		for (j = 0; IMPDRV_SYNCC_REGNUM > j; j++)
		{
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, syncc_val[j]);
		}
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, clbrk_addr);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_CNN_CLCNDGSBR_MASK);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, cnn_get_inten_val(p_corectl->irq_mask));
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, mask_val & ~cnn_get_inten_val(p_corectl->irq_mask));
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, claddr_phys);
	}
#if !defined (V3M2)
	else if (IMPDRV_CORE_TYPE_DMAC_SLIM == core_type)
	{
		i = 0;
		mask_val = get_irq_mask_val(core_type);

		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0);
		get_syncc_val(p_corectl, syncc_val);
		for (j = 0; IMPDRV_SYNCC_REGNUM > j; j++)
		{
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, syncc_val[j]);
		}
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, clbrk_addr);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_DMA_CLCNDGSBR_MASK);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, dmacs_get_inten_val(p_corectl->irq_mask) | IMPDRV_DMAS_SER_FIX);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, (mask_val & ~ dmacs_get_inten_val(p_corectl->irq_mask)) | IMPDRV_DMAS_SER_FIX);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, claddr_phys);
	}
#endif
#if defined (V4H) || defined (V4H2)
	else if (IMPDRV_CORE_TYPE_DSP == core_type)
	{
		/* prepare for IMPDRV_Execute */
		i = 0;

		/* impdrv_dspctl_load_ptcm */
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0x00000000U);								/* IMPDRV_REGOFS_VDSP_CONTROL */
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0x00000000U);								/* IMPDRV_REGOFS_VDSP_CONTROL (readback) */
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, p_corectl->dsp_info.dsp_app.addr_phys);	/* IMPDRV_REGOFS_VDSP_RST_VEC_ADR (readback) */
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_VDSP_REGVAL_PDMA_BUSY);				/* IMPDRV_REGOFS_P_PDTC */

		/* impdrv_dspctl_tcm_config_p */
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0x00000000U);								/* IMPDRV_REGOFS_P_CCOCR */
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0x00000000U);								/* IMPDRV_REGOFS_P_CCOCR */
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0xFFFFFFFFU);								/* IMPDRV_REGOFS_MSS_PCR */

		/* impdrv_dspctl_load_dtcm */
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0x00000000U);								/* IMPDRV_REGOFS_D_DDCL */

		/* impdrv_dspctl_dsp_execute */
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_REGVAL_OUT_INT_TIMER00);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_REGVAL_CLR);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_REGVAL_VDSP_GPIO_START);

		/* Initialize DSP info */
		p_corectl->dsp_info.is_loaded   = false;
		p_corectl->dsp_info.is_update   = false;
		p_corectl->dsp_info.is_executed = false;
	}
#endif
	else
	{
		core_testmsg(p_core_info, "invalid core\n");
	}
}

static int32_t CbFunc(
	const st_impdrv_core_info_t *p_core_info,			/**< Core information					*/
	const e_impdrv_cb_ret_t		ercd,				/**< Error code							*/
	const int32_t				code,				/**< TRAP or INT code (8 bit is set)	*/
	const void					*p_callback_args
)
{
	check_callback_ercd = ercd;
	check_callback_code = code;

	return 0;
}

int32_t it_Execute_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

#ifdef IT_ABNORMAL
	testno = 108;
	Parameter_error(tparam, &tret);
	Invalid_core(tparam, &tret);
	No_exec_core(tparam, &tret);
	Invalid_state(tparam, &tret);
	Return_error(tparam, &tret);
#endif
	Core_exec(tparam, &tret);

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
    p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;
	uint32_t claddr_phys_backup;

	/* Parameter_error : handle == NULL */
	ercd = R_IMPDRV_Execute(NULL, p_core_info, callback_func, (void *)&callback_args);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) handle == NULL", testno++);

	/* Parameter_error : p_core_info == NULL */
	ercd = R_IMPDRV_Execute(handle, NULL, callback_func, (void *)&callback_args);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) p_core_info == NULL", testno++);

	/* Parameter_error : callback_func == NULL */
	ercd = R_IMPDRV_Execute(handle, p_core_info, NULL, (void *)&callback_args);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) callback_func == NULL", testno++);

	/* Parameter_error : claddr_phys for IMP0 is IMPDRV_INIT_CL_ADDR */
	claddr_phys_backup = p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].claddr_phys;
	p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].claddr_phys = IMPDRV_INIT_CL_ADDR;
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	FailTestPrint(ercd, IMPDRV_EC_NG_ATTRIBUTE, *tret, "(No.%d) claddr_phys for IMP0 is IMPDRV_INIT_CL_ADDR", testno++);
	p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].claddr_phys = claddr_phys_backup;
}

static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_IMP, 0 };
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
	impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;
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

		core_info = *(invalid_core_list.core_info + i);
		ercd = R_IMPDRV_Execute(handle, &core_info, callback_func, (void *)&callback_args);
		FailTestPrint(ercd, ((core_info.core_type == IMPDRV_CORE_TYPE_INVALID) ?
							IMPDRV_EC_NG_PARAM : IMPDRV_EC_NG_NOTSUPPORT),
							*tret, "(No.%d) Invalid core num for %s", testno++, type_to_name(core_info.core_type));
	}

	/* Invalid_error : Core_num = IMPDRV_CORE_NUM_MAX */
	core_info.core_type = IMPDRV_CORE_TYPE_IMP;
	core_info.core_num = IMPDRV_CORE_NUM_MAX;
	ercd = R_IMPDRV_Execute(handle, &core_info, callback_func, (void *)&callback_args);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) Core_num = IMPDRV_CORE_NUM_MAX", testno++);
}

static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_IMP, 0 };
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
	impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;
	tp_core_list_t no_exec_cores = get_core_list(IT_IMPDRV_CMD_NO_EXEC, 0);

	/* Not supported core: Core not supported */
	for (uint32_t i = 0; i < no_exec_cores.num; i++)
	{
		core_info = *(no_exec_cores.core_info + i);
		ercd = R_IMPDRV_Execute(handle, &core_info, callback_func, (void *)&callback_args);
		FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) Core not supported: %s", testno++, type_to_name(core_info.core_type));
	}
}

static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
	st_impdrv_core_info_t *p_core_info = p_initdata->core_info;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;
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
		ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
		FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) %s", testno++, state_to_name(invalid_state[i]));
	}

	/* Get backup data */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
}

static void Return_error(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
	st_impdrv_core_info_t *p_core_info = p_initdata->core_info;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;
	uint32_t claddr_phys = p_impdrv_ctl->coretype_ctl[p_core_info->core_type].core_ctl[p_core_info->core_num].claddr_phys;

	/* Back up data */
	memcpy(coretype_ctl_backup, p_impdrv_ctl->coretype_ctl, sizeof(p_impdrv_ctl->coretype_ctl));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_TIME */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_TIME);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_TIME));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_PAR */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_PAR);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_BUSY */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_BUSY);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_BUSY));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_HANDLE */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_STATE */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_FAIL */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Fail to read register IMPDRV_REG_IMP_IFCTL for IMP0: R_OSAL_IoRead32 returns OSAL_RETURN_PAR */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_PAR, 0, 0);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 offset IMPDRV_REG_IMP_IFCTL for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to read register IMPDRV_REG_IMP_IFCTL for IMP0: R_OSAL_IoRead32 returns OSAL_RETURN_DEV */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_DEV, 0, 0);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 offset IMPDRV_REG_IMP_IFCTL for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to read register IMPDRV_REG_IMP_IFCTL for IMP0: R_OSAL_IoRead32 returns OSAL_RETURN_IO */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_IO, 0, 0);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 offset IMPDRV_REG_IMP_IFCTL for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_IO));

	/* Fail to read register IMPDRV_REG_IMP_IFCTL for IMP0: R_OSAL_IoRead32 returns OSAL_RETURN_HANDLE */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_HANDLE, 0, 0);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 offset IMPDRV_REG_IMP_IFCTL for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to read register IMPDRV_REG_IMP_IFCTL for IMP0: R_OSAL_IoRead32 returns OSAL_RETURN_STATE */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_STATE, 0, 0);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 offset IMPDRV_REG_IMP_IFCTL for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to read register IMPDRV_REG_IMP_IFCTL for IMP0: R_OSAL_IoRead32 returns OSAL_RETURN_FAIL */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_FAIL, 0, 0);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 offset IMPDRV_REG_IMP_IFCTL for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Read register return invalid value for IMP: R_OSAL_IoRead32 reading offset IMPDRV_REG_IMP_IFCTL returns invalid value (returns IMPDRV_IMP_IFCTL_EXE) */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 0, IMPDRV_IMP_IFCTL_EXE);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) R_OSAL_IoRead32 offset IMPDRV_REG_IMP_IFCTL for IMP0 returns invalid value", testno++);

	/* Fail to write register IMPDRV_REG_IMP_CNF for IMP: R_OSAL_IoWrite32 returns OSAL_RETURN_PAR */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_PAR, IMPDRV_REG_IMP_CNF, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_IMP_CNF returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to write register IMPDRV_REG_IMP_CNF for IMP: R_OSAL_IoWrite32 returns OSAL_RETURN_DEV */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_DEV, IMPDRV_REG_IMP_CNF, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_IMP_CNF returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to write register IMPDRV_REG_IMP_CNF for IMP: R_OSAL_IoWrite32 returns OSAL_RETURN_IO */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_IO, IMPDRV_REG_IMP_CNF, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_IMP_CNF returns %s", testno++, osal_err_to_name(OSAL_RETURN_IO));

	/* Fail to write register IMPDRV_REG_IMP_CNF for IMP: R_OSAL_IoWrite32 returns OSAL_RETURN_HANDLE */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_HANDLE, IMPDRV_REG_IMP_CNF, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_IMP_CNF returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to write register IMPDRV_REG_IMP_CNF for IMP: R_OSAL_IoWrite32 returns OSAL_RETURN_STATE */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_STATE, IMPDRV_REG_IMP_CNF, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_IMP_CNF returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to write register IMPDRV_REG_IMP_CNF for IMP: R_OSAL_IoWrite32 returns OSAL_RETURN_FAIL */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_FAIL, IMPDRV_REG_IMP_CNF, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_IMP_CNF returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Read register return unexpected value for IMP: R_OSAL_IoRead32 reading offset IMPDRV_REG_IMP_IFCFG returns invalid value (returns IMPDRV_IMP_IFCFG_VAL + 1) */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 1, IMPDRV_IMP_IFCFG_VAL + 1);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_CHECKFAIL, *tret, "(No.%d) R_OSAL_IoRead32 offset IMPDRV_REG_IMP_IFCFG for IMP0 returns unexpected value", testno++);

#if defined (V3H1) || defined (V3H2)
	/* Fail to write register IMPDRV_REG_IMPS_CNF for Slim-IMP: R_OSAL_IoWrite32 returns OSAL_RETURN_PAR */
	p_core_info = get_core_info(p_initdata->core_info, p_initdata->use_core_num, IMPDRV_CORE_TYPE_IMP_SLIM, 0);
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_PAR, IMPDRV_REG_IMPS_CNF, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_IMPS_CNF returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));
#else
	testno++;
#endif

#if defined (V3H1) || defined (V3H2)
	/* Fail to write register IMPDRV_REG_IMPS_CNF for Slim-IMP: R_OSAL_IoWrite32 returns OSAL_RETURN_DEV */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_DEV, IMPDRV_REG_IMPS_CNF, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_IMPS_CNF returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));
#else
	testno++;
#endif

#if defined (V3H1) || defined (V3H2)
	/* Fail to write register IMPDRV_REG_IMPS_CNF for Slim-IMP: R_OSAL_IoWrite32 returns OSAL_RETURN_IO */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_IO, IMPDRV_REG_IMPS_CNF, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_IMPS_CNF returns %s", testno++, osal_err_to_name(OSAL_RETURN_IO));
#else
	testno++;
#endif

#if defined (V3H1) || defined (V3H2)
	/* Fail to write register IMPDRV_REG_IMPS_CNF for Slim-IMP: R_OSAL_IoWrite32 returns OSAL_RETURN_HANDLE */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_HANDLE, IMPDRV_REG_IMPS_CNF, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_IMPS_CNF returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));
#else
	testno++;
#endif

#if defined (V3H1) || defined (V3H2)
	/* Fail to write register IMPDRV_REG_IMPS_CNF for Slim-IMP: R_OSAL_IoWrite32 returns OSAL_RETURN_STATE */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_STATE, IMPDRV_REG_IMPS_CNF, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_IMPS_CNF returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));
#else
	testno++;
#endif

#if defined (V3H1) || defined (V3H2)
	/* Fail to write register IMPDRV_REG_IMPS_CNF for Slim-IMP: R_OSAL_IoWrite32 returns OSAL_RETURN_FAIL */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_FAIL, IMPDRV_REG_IMPS_CNF, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_IMPS_CNF returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));
#else
	testno++;
#endif

	/* Fail to write register IMPDRV_REG_OCV_SCR1 for OCV: R_OSAL_IoWrite32 returns OSAL_RETURN_PAR */
	p_core_info = get_core_info(p_initdata->core_info, p_initdata->use_core_num, IMPDRV_CORE_TYPE_OCV, 0);
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_PAR, IMPDRV_REG_OCV_SCR1, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_OCV_SCR1 returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to write register IMPDRV_REG_OCV_SCR1 for OCV: R_OSAL_IoWrite32 returns OSAL_RETURN_DEV */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_DEV, IMPDRV_REG_OCV_SCR1, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_OCV_SCR1 returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to write register IMPDRV_REG_OCV_SCR1 for OCV: R_OSAL_IoWrite32 returns OSAL_RETURN_IO */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_IO, IMPDRV_REG_OCV_SCR1, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_OCV_SCR1 returns %s", testno++, osal_err_to_name(OSAL_RETURN_IO));

	/* Fail to write register IMPDRV_REG_OCV_SCR1 for OCV: R_OSAL_IoWrite32 returns OSAL_RETURN_HANDLE */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_HANDLE, IMPDRV_REG_OCV_SCR1, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_OCV_SCR1 returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to write register IMPDRV_REG_OCV_SCR1 for OCV: R_OSAL_IoWrite32 returns OSAL_RETURN_STATE */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_STATE, IMPDRV_REG_OCV_SCR1, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_OCV_SCR1 returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to write register IMPDRV_REG_OCV_SCR1 for OCV: R_OSAL_IoWrite32 returns OSAL_RETURN_FAIL */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_FAIL, IMPDRV_REG_OCV_SCR1, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_OCV_SCR1 returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Fail to write register IMPDRV_REG_DMA_SYNCCR0 for DMAC: R_OSAL_IoWrite32 returns OSAL_RETURN_PAR */
	p_core_info = get_core_info(p_initdata->core_info, p_initdata->use_core_num, IMPDRV_CORE_TYPE_DMAC, 0);
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_PAR, IMPDRV_REG_DMA_SYNCCR0, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_DMA_SYNCCR0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to write register IMPDRV_REG_DMA_SYNCCR0 for DMAC: R_OSAL_IoWrite32 returns OSAL_RETURN_DEV */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_DEV, IMPDRV_REG_DMA_SYNCCR0, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_DMA_SYNCCR0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to write register IMPDRV_REG_DMA_SYNCCR0 for DMAC: R_OSAL_IoWrite32 returns OSAL_RETURN_IO */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_IO, IMPDRV_REG_DMA_SYNCCR0, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_DMA_SYNCCR0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_IO));

	/* Fail to write register IMPDRV_REG_DMA_SYNCCR0 for DMAC: R_OSAL_IoWrite32 returns OSAL_RETURN_HANDLE */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_HANDLE, IMPDRV_REG_DMA_SYNCCR0, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_DMA_SYNCCR0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to write register IMPDRV_REG_DMA_SYNCCR0 for DMAC: R_OSAL_IoWrite32 returns OSAL_RETURN_STATE */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_STATE, IMPDRV_REG_DMA_SYNCCR0, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_DMA_SYNCCR0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to write register IMPDRV_REG_DMA_SYNCCR0 for DMAC: R_OSAL_IoWrite32 returns OSAL_RETURN_FAIL */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_FAIL, IMPDRV_REG_DMA_SYNCCR0, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_DMA_SYNCCR0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

#if defined (V3H2) || defined (V4H) || defined (V4H2)
	/* Fail to write register IMPDRV_REG_DMAS_SYNCCR0 for Slim-DMAC: R_OSAL_IoWrite32 returns OSAL_RETURN_PAR */
	p_core_info = get_core_info(p_initdata->core_info, p_initdata->use_core_num, IMPDRV_CORE_TYPE_DMAC_SLIM, 0);
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_PAR, IMPDRV_REG_DMAS_SYNCCR0, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_DMAS_SYNCCR0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));
#else
	testno++;
#endif

#if defined (V3H2) || defined (V4H) || defined (V4H2)
	/* Fail to write register IMPDRV_REG_DMAS_SYNCCR0 for Slim-DMAC: R_OSAL_IoWrite32 returns OSAL_RETURN_DEV */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_DEV, IMPDRV_REG_DMAS_SYNCCR0, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_DMAS_SYNCCR0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));
#else
	testno++;
#endif

#if defined (V3H2) || defined (V4H) || defined (V4H2)
	/* Fail to write register IMPDRV_REG_DMAS_SYNCCR0 for Slim-DMAC: R_OSAL_IoWrite32 returns OSAL_RETURN_IO */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_IO, IMPDRV_REG_DMAS_SYNCCR0, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_DMAS_SYNCCR0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_IO));
#else
	testno++;
#endif

#if defined (V3H2) || defined (V4H) || defined (V4H2)
	/* Fail to write register IMPDRV_REG_DMAS_SYNCCR0 for Slim-DMAC: R_OSAL_IoWrite32 returns OSAL_RETURN_HANDLE */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_HANDLE, IMPDRV_REG_DMAS_SYNCCR0, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_DMAS_SYNCCR0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));
#else
	testno++;
#endif

#if defined (V3H2) || defined (V4H) || defined (V4H2)
	/* Fail to write register IMPDRV_REG_DMAS_SYNCCR0 for Slim-DMAC: R_OSAL_IoWrite32 returns OSAL_RETURN_STATE */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_STATE, IMPDRV_REG_DMAS_SYNCCR0, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_DMAS_SYNCCR0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));
#else
	testno++;
#endif

#if defined (V3H2) || defined (V4H) || defined (V4H2)
	/* Fail to write register IMPDRV_REG_DMAS_SYNCCR0 for Slim-DMAC: R_OSAL_IoWrite32 returns OSAL_RETURN_FAIL */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_FAIL, IMPDRV_REG_DMAS_SYNCCR0, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_DMAS_SYNCCR0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));
#else
	testno++;
#endif

	/* Fail to write register IMPDRV_REG_PSC_SCTLR0 for PSCEXE: R_OSAL_IoWrite32 returns OSAL_RETURN_PAR */
	p_core_info = get_core_info(p_initdata->core_info, p_initdata->use_core_num, IMPDRV_CORE_TYPE_PSCEXE, 0);
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_PAR, IMPDRV_REG_PSC_SCTLR0, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_PSC_SCTLR0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to write register IMPDRV_REG_PSC_SCTLR0 for PSCEXE: R_OSAL_IoWrite32 returns OSAL_RETURN_DEV */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_DEV, IMPDRV_REG_PSC_SCTLR0, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_PSC_SCTLR0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to write register IMPDRV_REG_PSC_SCTLR0 for PSCEXE: R_OSAL_IoWrite32 returns OSAL_RETURN_IO */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_IO, IMPDRV_REG_PSC_SCTLR0, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_PSC_SCTLR0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_IO));

	/* Fail to write register IMPDRV_REG_PSC_SCTLR0 for PSCEXE: R_OSAL_IoWrite32 returns OSAL_RETURN_HANDLE */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_HANDLE, IMPDRV_REG_PSC_SCTLR0, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_PSC_SCTLR0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to write register IMPDRV_REG_PSC_SCTLR0 for PSCEXE: R_OSAL_IoWrite32 returns OSAL_RETURN_STATE */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_STATE, IMPDRV_REG_PSC_SCTLR0, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_PSC_SCTLR0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to write register IMPDRV_REG_PSC_SCTLR0 for PSCEXE: R_OSAL_IoWrite32 returns OSAL_RETURN_FAIL */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_FAIL, IMPDRV_REG_PSC_SCTLR0, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_PSC_SCTLR0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Fail to write register IMPDRV_REG_CNN_SWRST for CNN: R_OSAL_IoWrite32 returns OSAL_RETURN_PAR */
	p_core_info = get_core_info(p_initdata->core_info, p_initdata->use_core_num, IMPDRV_CORE_TYPE_CNN, 0);
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_PAR, IMPDRV_REG_CNN_SWRST, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_CNN_SWRST returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to write register IMPDRV_REG_CNN_SWRST for CNN: R_OSAL_IoWrite32 returns OSAL_RETURN_DEV */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_DEV, IMPDRV_REG_CNN_SWRST, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_CNN_SWRST returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to write register IMPDRV_REG_CNN_SWRST for CNN: R_OSAL_IoWrite32 returns OSAL_RETURN_IO */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_IO, IMPDRV_REG_CNN_SWRST, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_CNN_SWRST returns %s", testno++, osal_err_to_name(OSAL_RETURN_IO));

	/* Fail to write register IMPDRV_REG_CNN_SWRST for CNN: R_OSAL_IoWrite32 returns OSAL_RETURN_HANDLE */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_HANDLE, IMPDRV_REG_CNN_SWRST, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_CNN_SWRST returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to write register IMPDRV_REG_CNN_SWRST for CNN: R_OSAL_IoWrite32 returns OSAL_RETURN_STATE */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_STATE, IMPDRV_REG_CNN_SWRST, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_CNN_SWRST returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to write register IMPDRV_REG_CNN_SWRST for CNN: R_OSAL_IoWrite32 returns OSAL_RETURN_FAIL */
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_FAIL, IMPDRV_REG_CNN_SWRST, 0U);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for offset IMPDRV_REG_CNN_SWRST returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_HANDLE */
	p_core_info = tparam->initdata.core_info;
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_STATE */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_FAIL */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
	prepare_test_for_execute(handle, p_core_info, claddr_phys);
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Get backup data */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
}
#endif

static void Core_exec(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
    st_impdrv_initdata_t *p_initdata = &tparam->initdata;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;
	uint32_t *dtcm_addr = NULL;
	
	/* Initialize for DSP core */
	tparam->dsp_app.addr_phys = 0x40000000U;
	tparam->dsp_app.size = 0x2000U;
	/* allocate cpu addr dtcm */
	dtcm_addr = (uint32_t *)MemAlloc(osal_mmngr, 2u, 64u, IMP_MALLOC_HW);
	dtcm_addr[0] = 0x12345678U;
	dtcm_addr[1] = 0x87654321U;
	tparam->dsp_dtcm.addr_phys = util_mem_get_paddr(dtcm_addr);
	tparam->dsp_dtcm.size = 2u;
	const st_impdrv_dsp_data_t *const p_dspdata = (const st_impdrv_dsp_data_t *const)&tparam->dsp_data;
	const st_impdrv_dsp_data_t *const p_dspfw = (const st_impdrv_dsp_data_t *const)&tparam->dsp_fw;
	const st_impdrv_dsp_data_t *const p_dspapp = (const st_impdrv_dsp_data_t *const)&tparam->dsp_app;
	const st_impdrv_dsp_data_t *const p_dspdtcm = (const st_impdrv_dsp_data_t *const)&tparam->dsp_dtcm;

	/* driver_execute : output parameter */
	for (uint32_t i = 0; i < p_initdata->use_core_num; i++)
	{
		st_impdrv_core_info_t *p_core_info = &p_initdata->core_info[i];
		e_impdrv_core_type_t core_type = p_core_info->core_type;
		uint32_t core_num = p_core_info->core_num;
		st_impdrv_coretypectl_t *coretype_ctl = &p_impdrv_ctl->coretype_ctl[core_type];
		int32_t result = -1;

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
		it_stub_reset();
		prepare_test_for_execute(handle, p_core_info, util_mem_get_paddr(tparam->long_claddr));
		if (IMPDRV_CORE_TYPE_DSP != p_core_info->core_type)
		{
			if (set_commandlist(p_core_info->core_type, tparam->long_claddr, CALLBACK_CODE))
			{
				core_testmsg(p_core_info, "set_commandlist error\n");
			}
			ercd = R_IMPDRV_AttrSetCl(handle, p_core_info, util_mem_get_paddr(tparam->long_claddr));
			if (IMPDRV_EC_OK != ercd)
			{
				core_testmsg(p_core_info, "Set CL data error\n");
			}
		}
		else {
			ercd = R_IMPDRV_AttrSetDsp(handle, p_core_info, p_dspapp, p_dspfw, p_dspdata, p_dspdtcm);
			if (IMPDRV_EC_OK != ercd)
			{
				core_testmsg(p_core_info, "Set DSP data error\n");
			}
		}

		ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);

		if (ercd != IMPDRV_EC_OK)
		{
			core_testmsg(p_core_info, "%s=%d\n", TEST_MSG_NAME, ercd);
		}
		else
		{
			/* Check state of the core */
			if (coretype_ctl->core_ctl[core_num].core_state != IMPDRV_STATE_EXEC)
			{
				core_testmsg(p_core_info, "core_type = %d, core_num = %u, state = %s, it is not EXEC\n",
								core_type,
								core_num,
								state_to_name(coretype_ctl->core_ctl[core_num].core_state));
			}
			else if ((check_callback_ercd != IMPDRV_CB_RET_BEFORE_EXEC) || (check_callback_code != -1))
			{
				core_testmsg(p_core_info, "callback(trap) fail\n");
			}
			else
			{
				result = 0;
				coretype_ctl->core_ctl[core_num].core_state = IMPDRV_STATE_READY;
			}
		}
		if ((IMPDRV_CORE_TYPE_DSP == core_type) && (result == 0))
		{
			/* DSP Register Check */
			/* VDSP_ICU_ACU_LOCK (3rd write) */
			if (g_write_value[WRITE_REG_IDX_VDSP_ICU_ACU_LOCK_3RD] != 0x00000000U)
			{
				core_testmsg(p_core_info, "%s[%08x] is not [%08x]\n", "VDSP_ICU_ACU_LOCK (1st write)", g_write_value[WRITE_REG_IDX_VDSP_ICU_ACU_LOCK_3RD], 0x00000000U);
				result = -1;
			}
			/* VDSP_ICU_ACU_LOCK (4th write) */
			if (g_write_value[WRITE_REG_IDX_VDSP_ICU_ACU_LOCK_4TH] != 0x00000001U)
			{
				core_testmsg(p_core_info, "%s[%08x] is not [%08x]\n", "VDSP_ICU_ACU_LOCK (2nd write)", g_write_value[WRITE_REG_IDX_VDSP_ICU_ACU_LOCK_4TH], 0x00000001U);
				result = -1;
			}
			/* P_ADD0_ATT0 */
			if (g_write_value[WRITE_REG_IDX_P_ADD0_ATT0] != 0x00000260U)
			{
				core_testmsg(p_core_info, "%s[%08x] is not [%08x]\n", "P_ADD0_ATT0", g_write_value[WRITE_REG_IDX_P_ADD0_ATT0], 0x00000260U);
				result = -1;
			}
			/* P_ADD2_ATT0 */
			if (g_write_value[WRITE_REG_IDX_P_ADD2_ATT0] != 0x00000260U)
			{
				core_testmsg(p_core_info, "%s[%08x] is not [%08x]\n", "P_ADD2_ATT0", g_write_value[WRITE_REG_IDX_P_ADD2_ATT0], 0x00000260U);
				result = -1;
			}
			/* P_ADD3_START */
			if (g_write_value[WRITE_REG_IDX_P_ADD3_START] != 0x00000000U)
			{
				core_testmsg(p_core_info, "%s[%08x] is not [%08x]\n", "P_ADD3_START", g_write_value[WRITE_REG_IDX_P_ADD3_START], 0x00000000U);
				result = -1;
			}
			/* P_ADD3_ATT0 */
			if (g_write_value[WRITE_REG_IDX_P_ADD3_ATT0] != 0x00000201U)
			{
				core_testmsg(p_core_info, "%s[%08x] is not [%08x]\n", "P_ADD3_ATT0", g_write_value[WRITE_REG_IDX_P_ADD3_ATT0], 0x00000201U);
				result = -1;
			}
			/* P_ADD4_START */
			if (g_write_value[WRITE_REG_IDX_P_ADD4_START] != 0x10000000U)
			{
				core_testmsg(p_core_info, "%s[%08x] is not [%08x]\n", "P_ADD4_START", g_write_value[WRITE_REG_IDX_P_ADD4_START], 0x10000000U);
				result = -1;
			}
			/* D_DDCL */
			if (g_write_value[WRITE_REG_IDX_D_DDCL] != 0x000001B0U)
			{
				core_testmsg(p_core_info, "%s[%08x] is not [%08x]\n", "D_DDCL", g_write_value[WRITE_REG_IDX_D_DDCL], 0x000001B0U);
				result = -1;
			}
#if defined (IMPDRV_DSPCRC_DETECT_ADDDEF)
			uint32_t exp_d_sft_scpd_3;

			/* Register check related CRC */
			/* D_SFT_SCPD_0 (1st write) */
			if (g_write_value[WRITE_REG_IDX_D_SFT_SCPD_0] != 0x00000001U)
			{
				core_testmsg(p_core_info, "%s[%08x] is not [%08x]\n", " D_SFT_SCPD_0 (1st write)", g_write_value[WRITE_REG_IDX_D_SFT_SCPD_0], 0x00000001U);
				result = -1;
			}
			/* D_SFT_SCPD_0 (2nd write) */
			if (g_write_value[WRITE_REG_IDX_D_SFT_SCPD_0_2ND] != 0x00000081U)
			{
				core_testmsg(p_core_info, "%s[%08x] is not [%08x]\n", "D_SFT_SCPD_0 (2nd write)", g_write_value[WRITE_REG_IDX_D_SFT_SCPD_0_2ND], 0x00000081U);
				result = -1;
			}
			/* D_SFT_SCPD_1 */
			if (g_write_value[WRITE_REG_IDX_D_SFT_SCPD_1] != util_mem_get_paddr(dtcm_addr))
			{
				core_testmsg(p_core_info, "%s[%08x] is not [%08x]\n", "D_SFT_SCPD_1", g_write_value[WRITE_REG_IDX_D_SFT_SCPD_1], util_mem_get_paddr(dtcm_addr));
				result = -1;
			}
			/* D_SFT_SCPD_2 */
			if (g_write_value[WRITE_REG_IDX_D_SFT_SCPD_2] != 0x00000002U)
			{
				core_testmsg(p_core_info, "%s[%08x] is not [%08x]\n", "D_SFT_SCPD_2", g_write_value[WRITE_REG_IDX_D_SFT_SCPD_2], 0x00000002U);
				result = -1;
			}
			/* D_SFT_SCPD_3 */
#if defined (IMPDRV_DSPCRC_DETECT_UDEF)
			exp_d_sft_scpd_3 = 0xFFFFFFFFU;
#elif defined(IMPDRV_DSPCRC_DETECT_REF)
			exp_d_sft_scpd_3 = 0xF0A1BB94U;
#endif
			if (g_write_value[WRITE_REG_IDX_D_SFT_SCPD_3] != exp_d_sft_scpd_3)
			{
				core_testmsg(p_core_info, "%s[%08x] is not [%08x]\n", "D_SFT_SCPD_3", g_write_value[WRITE_REG_IDX_D_SFT_SCPD_3], exp_d_sft_scpd_3);
				result = -1;
			}

			printf("exp_d_sft_scpd_3 : 0x%08x\n", exp_d_sft_scpd_3);
#endif
			/* For debug */
			if (result == 0)
			{
				printf("Register check is ALL OK !!!\n");
			}
		}

		util_mem_invalidate(tparam->long_claddr);

		SuccessTestPrint(result == 0, *tret, "(No.%d) %s%d ", testno++, type_to_name(core_type), core_num);
	}

	if (dtcm_addr != NULL)
	{
		MemFree(osal_mmngr, dtcm_addr);
	}
}
