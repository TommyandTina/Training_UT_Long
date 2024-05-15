#define TEST_MSG_NAME "R_IMPDRV_ResumeExecution"

#include "it_common.h"
#include "it_stub.h"

#define CALLBACK_CODE_1		(0xAA)
#define CALLBACK_CODE_2		(0x55)

extern uintptr_t clbrk_addr;

static void set_state(st_impdrv_initdata_t *const p_data, e_impdrv_state_t state);
static void prepare_test_for_resume(impdrv_ctrl_handle_t handle, const st_impdrv_core_info_t *const p_core_info);
#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret);
static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret);
static void Return_error(struct test_params_t *tparam, struct test_result_t *tret);
#endif
static void Device_resume_execution(struct test_params_t *tparam, struct test_result_t *tret);

static int32_t check_callback_code;
static e_impdrv_cb_ret_t check_callback_ercd;
static uint32_t testno;
static st_impdrv_coretypectl_t coretype_ctl_backup[IMPDRV_CORE_TYPE_MAX];
static e_impdrv_state_t state_bk;

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

static void prepare_test_for_resume(
	impdrv_ctrl_handle_t		handle,
	const st_impdrv_core_info_t	*const p_core_info
)
{
	//prepare_test_for_execute(handle, p_core_info, claddr_phys);
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
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, clbrk_addr);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_IMP_CLCNDGSBR_MASK);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_IMP_IFCFG_VAL);
		get_syncc_val(p_corectl, syncc_val);
		for (j = 0; IMPDRV_SYNCC_REGNUM > j; j++)
		{
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, syncc_val[j]);
		}
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, imp_get_inten_val(p_corectl->irq_mask));
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
	}
#endif
	else if (IMPDRV_CORE_TYPE_OCV == core_type)
	{
		i = 0;
		mask_val = get_irq_mask_val(core_type);

		/* Execute stub setting */
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, clbrk_addr);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_OCV_CLCNDGSBR_MASK);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, mask_val & ocv_get_inten_val(p_corectl->irq_mask));
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, (IMPDRV_OCV_MCR0_DCBANKSEL1 | IMPDRV_OCV_MCR0_DCBANKSEL0));

		/* ResumeExecute stub setting */
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, (ocv_get_inten_val(p_corectl->irq_mask) | 
					(IMPDRV_OCV_SR1_UDIPSBRK & 0u) | 
					(IMPDRV_OCV_SR1_UDIVSBRK & 0u)));
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, mask_val & ~ocv_get_inten_val(p_corectl->irq_mask));
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, (ocv_get_inten_val(p_corectl->irq_mask) | 
					(IMPDRV_OCV_SR1_UDIPSBRK & 0u) | 
					(IMPDRV_OCV_SR1_UDIVSBRK & 0u)));
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, mask_val & ~ocv_get_inten_val(p_corectl->irq_mask));
	}
	else if (IMPDRV_CORE_TYPE_DMAC == core_type)
	{
		i = 0;
		mask_val = get_irq_mask_val(core_type);

		/* Execute stub setting */
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, clbrk_addr);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_DMA_CLCNDGSBR_MASK);
		
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, (dmac_get_inten_val(p_corectl->irq_mask)) | IMPDRV_DMA_SER_FIX);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, (mask_val & ~dmac_get_inten_val(p_corectl->irq_mask)) | IMPDRV_DMA_SER_FIX);

		/* ResumeExecute stub setting */
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, (dmac_get_inten_val(p_corectl->irq_mask)) | IMPDRV_DMA_SER_FIX);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, (mask_val & ~dmac_get_inten_val(p_corectl->irq_mask)) | IMPDRV_DMA_SER_FIX);
	}
#if !defined (V3M2)
	else if (IMPDRV_CORE_TYPE_DMAC_SLIM == core_type)
	{
		i = 0;
		mask_val = get_irq_mask_val(core_type);

		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, clbrk_addr);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_DMA_CLCNDGSBR_MASK);

		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, dmacs_get_inten_val(p_corectl->irq_mask) | IMPDRV_DMAS_SER_FIX);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, (mask_val & ~ dmacs_get_inten_val(p_corectl->irq_mask)) | IMPDRV_DMAS_SER_FIX);

		/* ResumeExecute stub setting */
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, (dmacs_get_inten_val(p_corectl->irq_mask)) | IMPDRV_DMAS_SER_FIX);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, (mask_val & ~dmacs_get_inten_val(p_corectl->irq_mask)) | IMPDRV_DMAS_SER_FIX);
	}
#endif
	else if (IMPDRV_CORE_TYPE_CNN == core_type)
	{
		i = 0;
		mask_val = get_irq_mask_val(core_type);

		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, clbrk_addr);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_CNN_CLCNDGSBR_MASK);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, cnn_get_inten_val(p_corectl->irq_mask));
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, mask_val & ~cnn_get_inten_val(p_corectl->irq_mask));
		/* ResumeExecute stub setting */
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, cnn_get_inten_val(p_corectl->irq_mask));

	}
	else if (IMPDRV_CORE_TYPE_PSCEXE == core_type)
	{
		i = 0;
		mask_val = get_irq_mask_val(core_type);

		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, clbrk_addr);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_PSC_CLCNDGSBR_MASK);
		
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, psc_get_inten_val(p_corectl->irq_mask));
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, mask_val & ~psc_get_inten_val(p_corectl->irq_mask));
		/* ResumeExecute stub setting */
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, psc_get_inten_val(p_corectl->irq_mask));
	}
#if defined (V4H) || defined (V4H2)
	else if (IMPDRV_CORE_TYPE_DSP == core_type)
	{
		/* prepare for IMPDRV_Execute */
		i = 0;
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_REGVAL_OUT_INT_TIMER00);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_REGVAL_CLR);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_REGVAL_VDSP_GPIO_RESUME);
	}
#endif
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

int32_t it_ResumeExecution_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

#ifdef IT_ABNORMAL
	testno = 103;
	Parameter_error(tparam, &tret);
	Invalid_core(tparam, &tret);
	No_exec_core(tparam, &tret);
	Invalid_state(tparam, &tret);
	Return_error(tparam, &tret);
#endif
	Device_resume_execution(tparam, &tret);

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

	/* Back up state */
	state_bk = p_impdrv_ctl->coretype_ctl[p_core_info->core_type].core_ctl[p_core_info->core_num].core_state;

	/* Parameter_error : handle == NULL */
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	ercd = R_IMPDRV_ResumeExecution(NULL, p_core_info);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) handle == NULL", testno++);

	/* Parameter_error : p_core_info == NULL */
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	ercd = R_IMPDRV_ResumeExecution(handle, NULL);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) p_core_info == NULL", testno++);

	/* Revert state */
	set_state(&tparam->initdata, state_bk);
}

static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_IMP, 0 };
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	tp_core_list_t invalid_core_list = get_core_list(IT_IMPDRV_CMD_INVALID_CORES, 0);

	/* Back up state */
	state_bk = p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].core_state;

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

		/* Adjust state to be valid */
		set_state(&tparam->initdata, IMPDRV_STATE_INT);
		core_info = *(invalid_core_list.core_info + i);
		ercd = R_IMPDRV_ResumeExecution(handle, &core_info);
		FailTestPrint(ercd, ((core_info.core_type == IMPDRV_CORE_TYPE_INVALID) ?
							IMPDRV_EC_NG_PARAM : IMPDRV_EC_NG_NOTSUPPORT),
							*tret, "(No.%d) Invalid core num for %s", testno++, type_to_name(core_info.core_type));
	}

	/* Invalid_error : Core_num = IMPDRV_CORE_NUM_MAX */
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	core_info.core_type = IMPDRV_CORE_TYPE_IMP;
	core_info.core_num = IMPDRV_CORE_NUM_MAX;
	ercd = R_IMPDRV_ResumeExecution(handle, &core_info);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) Core_num = IMPDRV_CORE_NUM_MAX", testno++);

	/* Revert state */
	set_state(&tparam->initdata, state_bk);
}

static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_IMP, 0 };
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	tp_core_list_t no_exec_cores = get_core_list(IT_IMPDRV_CMD_NO_EXEC, 0);

	/* Back up state */
	state_bk = p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].core_state;

	/* Not supported core: Core not supported */
	for (uint32_t i = 0; i < no_exec_cores.num; i++)
	{
		set_state(&tparam->initdata, IMPDRV_STATE_INT);
		core_info = *(no_exec_cores.core_info + i);
		ercd = R_IMPDRV_ResumeExecution(handle, &core_info);
		FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) Core not supported: %s", testno++, type_to_name(core_info.core_type));
	}

	/* Revert state */
	set_state(&tparam->initdata, state_bk);
}

static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
	st_impdrv_core_info_t *p_core_info = p_initdata->core_info;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	e_impdrv_state_t invalid_state[] = {
		IMPDRV_STATE_UNINIT,
		IMPDRV_STATE_INIT,
		IMPDRV_STATE_READY,
		IMPDRV_STATE_EXEC
	};

	/* Back up data */
	memcpy(coretype_ctl_backup, p_impdrv_ctl->coretype_ctl, sizeof(p_impdrv_ctl->coretype_ctl));

	for (uint32_t i = 0; i < ARRAY_SIZE(invalid_state); i++)
	{
		set_state(p_initdata, invalid_state[i]);
		ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
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

	/* Back up state */
	state_bk = p_impdrv_ctl->coretype_ctl[p_core_info->core_type].core_ctl[p_core_info->core_num].core_state;

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_TIME */
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_TIME);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_TIME));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_PAR */
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_PAR);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_BUSY */
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_BUSY);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_BUSY));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_HANDLE */
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_STATE */
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_FAIL */
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Failed to memory barrier: R_OSAL_ThsyncMemoryBarrier returns OSAL_RETURN_STATE */
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_ThsyncMemoryBarrier(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMemoryBarrier returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Failed to memory barrier: R_OSAL_ThsyncMemoryBarrier returns OSAL_RETURN_FAIL */
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_ThsyncMemoryBarrier(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMemoryBarrier returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Fail to write Interrupt Status register for IMP: R_OSAL_IoWrite32 returns OSAL_RETURN_PAR */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_PAR, IMPDRV_REG_IMP_INTSTS, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Interrupt Status register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to write Interrupt Status register for IMP: R_OSAL_IoWrite32 returns OSAL_RETURN_DEV */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_DEV, IMPDRV_REG_IMP_INTSTS, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Interrupt Status register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to write Interrupt Status register for IMP: R_OSAL_IoWrite32 returns OSAL_RETURN_IO */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_IO, IMPDRV_REG_IMP_INTSTS, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Interrupt Status register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_IO));

	/* Fail to write Interrupt Status register for IMP: R_OSAL_IoWrite32 returns OSAL_RETURN_HANDLE */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_HANDLE, IMPDRV_REG_IMP_INTSTS, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Interrupt Status register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to write Interrupt Status register for IMP: R_OSAL_IoWrite32 returns OSAL_RETURN_STATE */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_STATE, IMPDRV_REG_IMP_INTSTS, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Interrupt Status register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to write Interrupt Status register for IMP: R_OSAL_IoWrite32 returns OSAL_RETURN_FAIL */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_FAIL, IMPDRV_REG_IMP_INTSTS, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Interrupt Status register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_FAIL));

#if defined (V3H1) || defined (V3H2)
	/* Fail to write Interrupt Status register for Slim-IMP: R_OSAL_IoWrite32 returns OSAL_RETURN_PAR */
	prepare_test_for_resume(handle, p_core_info);
	p_core_info = get_core_info(p_initdata->core_info, p_initdata->use_core_num, IMPDRV_CORE_TYPE_IMP_SLIM, 0);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_PAR, IMPDRV_REG_IMPS_INTSTS, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Interrupt Status register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_PAR));
#else
	testno++;
#endif

#if defined (V3H1) || defined (V3H2)
	/* Fail to write Interrupt Status register for Slim-IMP: R_OSAL_IoWrite32 returns OSAL_RETURN_DEV */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_DEV, IMPDRV_REG_IMPS_INTSTS, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Interrupt Status register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_DEV));
#else
	testno++;
#endif

#if defined (V3H1) || defined (V3H2)
	/* Fail to write Interrupt Status register for Slim-IMP: R_OSAL_IoWrite32 returns OSAL_RETURN_IO */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_IO, IMPDRV_REG_IMPS_INTSTS, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Interrupt Status register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_IO));
#else
	testno++;
#endif

#if defined (V3H1) || defined (V3H2)
	/* Fail to write Interrupt Status register for Slim-IMP: R_OSAL_IoWrite32 returns OSAL_RETURN_HANDLE */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_HANDLE, IMPDRV_REG_IMPS_INTSTS, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Interrupt Status register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_HANDLE));
#else
	testno++;
#endif

#if defined (V3H1) || defined (V3H2)
	/* Fail to write Interrupt Status register for Slim-IMP: R_OSAL_IoWrite32 returns OSAL_RETURN_STATE */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_STATE, IMPDRV_REG_IMPS_INTSTS, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Interrupt Status register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_STATE));
#else
	testno++;
#endif

#if defined (V3H1) || defined (V3H2)
	/* Fail to write Interrupt Status register for Slim-IMP: R_OSAL_IoWrite32 returns OSAL_RETURN_FAIL */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_FAIL, IMPDRV_REG_IMPS_INTSTS, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Interrupt Status register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_FAIL));
#else
	testno++;
#endif

	/* Fail to read Interrupt Status register (SR1) for OCV0: R_OSAL_IoRead32 returns OSAL_RETURN_PAR */
	prepare_test_for_resume(handle, p_core_info);
	p_core_info = get_core_info(p_initdata->core_info, p_initdata->use_core_num, IMPDRV_CORE_TYPE_OCV, 0);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_PAR, 0, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for Interrupt Status register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to read Interrupt Status register (SR1) for OCV0: R_OSAL_IoRead32 returns OSAL_RETURN_DEV */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_DEV, 0, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for Interrupt Status register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to read Interrupt Status register (SR1) for OCV0: R_OSAL_IoRead32 returns OSAL_RETURN_IO */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_IO, 0, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for Interrupt Status register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_IO));

	/* Fail to read Interrupt Status register (SR1) for OCV0: R_OSAL_IoRead32 returns OSAL_RETURN_HANDLE */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_HANDLE, 0, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for Interrupt Status register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to read Interrupt Status register (SR1) for OCV0: R_OSAL_IoRead32 returns OSAL_RETURN_STATE */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_STATE, 0, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for Interrupt Status register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to read Interrupt Status register (SR1) for OCV0: R_OSAL_IoRead32 returns OSAL_RETURN_FAIL */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_FAIL, 0, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for Interrupt Status register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Fail to write Status Clear Register for DMAC0: R_OSAL_IoWrite32 returns OSAL_RETURN_PAR */
	p_core_info = get_core_info(p_initdata->core_info, p_initdata->use_core_num, IMPDRV_CORE_TYPE_DMAC, 0);
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_PAR, IMPDRV_REG_DMA_SCR, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Status Clear register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to write Status Clear Register for DMAC0: R_OSAL_IoWrite32 returns OSAL_RETURN_DEV */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_DEV, IMPDRV_REG_DMA_SCR, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Status Clear register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to write Status Clear Register for DMAC0: R_OSAL_IoWrite32 returns OSAL_RETURN_IO */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_IO, IMPDRV_REG_DMA_SCR, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Status Clear register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_IO));

	/* Fail to write Status Clear Register for DMAC0: R_OSAL_IoWrite32 returns OSAL_RETURN_HANDLE */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_HANDLE, IMPDRV_REG_DMA_SCR, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Status Clear register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to write Status Clear Register for DMAC0: R_OSAL_IoWrite32 returns OSAL_RETURN_STATE */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_STATE, IMPDRV_REG_DMA_SCR, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Status Clear register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to write Status Clear Register for DMAC0: R_OSAL_IoWrite32 returns OSAL_RETURN_FAIL */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_FAIL, IMPDRV_REG_DMA_SCR, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Status Clear register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_FAIL));

#if defined (V3H2) || defined (V4H) || defined (V4H2)
	/* Fail to write Interrupt Status register for Slim-DMAC: R_OSAL_IoWrite32 returns OSAL_RETURN_PAR */
	prepare_test_for_resume(handle, p_core_info);
	p_core_info = get_core_info(p_initdata->core_info, p_initdata->use_core_num, IMPDRV_CORE_TYPE_DMAC_SLIM, 0);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_PAR, IMPDRV_REG_DMAS_SCR, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Status Clear register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_PAR));
#else
	testno++;
#endif

#if defined (V3H2) || defined (V4H) || defined (V4H2)
	/* Fail to write Interrupt Status register for Slim-DMAC: R_OSAL_IoWrite32 returns OSAL_RETURN_DEV */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_DEV, IMPDRV_REG_DMAS_SCR, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Status Clear register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_DEV));
#else
	testno++;
#endif

#if defined (V3H2) || defined (V4H) || defined (V4H2)
	/* Fail to write Interrupt Status register for Slim-DMAC: R_OSAL_IoWrite32 returns OSAL_RETURN_IO */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_IO, IMPDRV_REG_DMAS_SCR, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Status Clear register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_IO));
#else
	testno++;
#endif

#if defined (V3H2) || defined (V4H) || defined (V4H2)
	/* Fail to write Interrupt Status register for Slim-DMAC: R_OSAL_IoWrite32 returns OSAL_RETURN_HANDLE */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_HANDLE, IMPDRV_REG_DMAS_SCR, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Status Clear register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_HANDLE));
#else
	testno++;
#endif

#if defined (V3H2) || defined (V4H) || defined (V4H2)
	/* Fail to write Interrupt Status register for Slim-DMAC: R_OSAL_IoWrite32 returns OSAL_RETURN_STATE */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_STATE, IMPDRV_REG_DMAS_SCR, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Status Clear register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_STATE));
#else
	testno++;
#endif

#if defined (V3H2) || defined (V4H) || defined (V4H2)
	/* Fail to write Interrupt Status register for Slim-DMAC: R_OSAL_IoWrite32 returns OSAL_RETURN_FAIL */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_FAIL, IMPDRV_REG_DMAS_SCR, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Status Clear register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_FAIL));
#else
	testno++;
#endif

	/* Fail to write Status Clear Register for PSCEXE0: R_OSAL_IoWrite32 returns OSAL_RETURN_PAR */
	prepare_test_for_resume(handle, p_core_info);
	p_core_info = get_core_info(p_initdata->core_info, p_initdata->use_core_num, IMPDRV_CORE_TYPE_PSCEXE, 0);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_PAR, IMPDRV_REG_PSC_SCR, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Status Clear register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to write Status Clear Register for PSCEXE0: R_OSAL_IoWrite32 returns OSAL_RETURN_DEV */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_DEV, IMPDRV_REG_PSC_SCR, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Status Clear register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to write Status Clear Register for PSCEXE0: R_OSAL_IoWrite32 returns OSAL_RETURN_IO */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_IO, IMPDRV_REG_PSC_SCR, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Status Clear register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_IO));

	/* Fail to write Status Clear Register for PSCEXE0: R_OSAL_IoWrite32 returns OSAL_RETURN_HANDLE */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_HANDLE, IMPDRV_REG_PSC_SCR, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Status Clear register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to write Status Clear Register for PSCEXE0: R_OSAL_IoWrite32 returns OSAL_RETURN_STATE */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_STATE, IMPDRV_REG_PSC_SCR, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Status Clear register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to write Status Clear Register for PSCEXE0: R_OSAL_IoWrite32 returns OSAL_RETURN_FAIL */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_FAIL, IMPDRV_REG_PSC_SCR, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Status Clear register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Fail to write Status Clear Register for CNN0: R_OSAL_IoWrite32 returns OSAL_RETURN_PAR */
	prepare_test_for_resume(handle, p_core_info);
	p_core_info = get_core_info(p_initdata->core_info, p_initdata->use_core_num, IMPDRV_CORE_TYPE_CNN, 0);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_PAR, IMPDRV_REG_CNN_SRC, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Status Clear register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to write Status Clear Register for CNN0: R_OSAL_IoWrite32 returns OSAL_RETURN_DEV */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_DEV, IMPDRV_REG_CNN_SRC, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Status Clear register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to write Status Clear Register for CNN0: R_OSAL_IoWrite32 returns OSAL_RETURN_IO */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_IO, IMPDRV_REG_CNN_SRC, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Status Clear register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_IO));
	
	/* Fail to write Status Clear Register for CNN0: R_OSAL_IoWrite32 returns OSAL_RETURN_HANDLE */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_HANDLE, IMPDRV_REG_CNN_SRC, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Status Clear register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to write Status Clear Register for CNN0: R_OSAL_IoWrite32 returns OSAL_RETURN_STATE */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_STATE, IMPDRV_REG_CNN_SRC, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Status Clear register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to write Status Clear Register for CNN0: R_OSAL_IoWrite32 returns OSAL_RETURN_FAIL */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_FAIL, IMPDRV_REG_CNN_SRC, 0U);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for Status Clear register of %s%d returns %s", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_HANDLE */
	prepare_test_for_resume(handle, p_core_info);
	p_core_info = tparam->initdata.core_info;
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_STATE */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_FAIL */
	prepare_test_for_resume(handle, p_core_info);
	set_state(&tparam->initdata, IMPDRV_STATE_INT);
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Revert state */
	set_state(&tparam->initdata, state_bk);
}
#endif

static void Device_resume_execution(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;
	int32_t result;
	uint32_t claddr_phys;
	
	/* Initialize for DSP core */
	tparam->dsp_app.addr_phys = 0x40000000U;
	tparam->dsp_app.size = 0x2000U;
	const st_impdrv_dsp_data_t *const p_dspdata = (const st_impdrv_dsp_data_t *const)&tparam->dsp_data;
	const st_impdrv_dsp_data_t *const p_dspfw = (const st_impdrv_dsp_data_t *const)&tparam->dsp_fw;
	const st_impdrv_dsp_data_t *const p_dspapp = (const st_impdrv_dsp_data_t *const)&tparam->dsp_app;
	const st_impdrv_dsp_data_t *const p_dspdtcm = (const st_impdrv_dsp_data_t *const)&tparam->dsp_dtcm;

	/* driver_resume_execution : output parameter */
	for (uint32_t i = 0; i < tparam->initdata.use_core_num; i++)
	{
		st_impdrv_core_info_t *p_core_info = &tparam->initdata.core_info[i];
		e_impdrv_core_type_t core_type = p_core_info->core_type;
		uint32_t core_num = p_core_info->core_num;
		st_impdrv_coretypectl_t *coretype_ctl = &p_impdrv_ctl->coretype_ctl[core_type];
		claddr_phys = p_impdrv_ctl->coretype_ctl[p_core_info->core_type].core_ctl[p_core_info->core_num].claddr_phys;


#if defined (V3H1)
		if ((IMPDRV_CORE_TYPE_PSCEXE == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno += 4;
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
			testno += 6;
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
		prepare_test_for_execute(handle, p_core_info,claddr_phys);
		st_impdrv_ctl_t			*p_impdrv_ctl = (st_impdrv_ctl_t *)handle;
		st_impdrv_corectl_t		*p_corectl = &p_impdrv_ctl->coretype_ctl[core_type].core_ctl[core_num];
		p_corectl->dsp_info.is_loaded   = true;
		p_corectl->dsp_info.is_update   = true;
		p_corectl->dsp_info.is_executed = true;
		result = -1;

		if (IMPDRV_CORE_TYPE_DSP != p_core_info->core_type)
		{
			if (set_intcommandlist(p_core_info->core_type, tparam->long_claddr, CALLBACK_CODE_1, CALLBACK_CODE_2))
			{
				core_testmsg(p_core_info, "set_intcommandlist error\n");
				goto error1;
			}

			if (IMPDRV_EC_OK != R_IMPDRV_AttrSetCl(handle, p_core_info, util_mem_get_paddr(tparam->long_claddr)))
			{
				core_testmsg(p_core_info, "Set CL data error\n");
				goto error1;
			}
		}
		else {
			ercd = R_IMPDRV_AttrSetDsp(handle, p_core_info, p_dspapp, p_dspfw, p_dspdata, p_dspdtcm);
			if (IMPDRV_EC_OK != ercd)
			{
				core_testmsg(p_core_info, "Set DSP data error\n");
				goto error1;
			}
			it_stub_reset();
			int read_cnt = 0;
			/* impdrv_dspctl_dsp_foece_standby */
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, read_cnt++, IMPDRV_REGVAL_VDSP_GPIO_STANDBY);
			/* impdrv_dspctl_dsp_standby */
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, read_cnt++, IMPDRV_REGVAL_PSU_STATE_LSP);
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, read_cnt++, IMPDRV_REGVAL_DMBE_IDLE);
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, read_cnt++, IMPDRV_REGVAL_PSU_STATE_LSP|IMPDRV_REGVAL_PSU_STATE_SBY);
			/* impdrv_dspctl_load_ptcm */
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, read_cnt++, 0);
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, read_cnt++, 0);
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, read_cnt++, p_impdrv_ctl->coretype_ctl[p_core_info->core_type].core_ctl[p_core_info->core_num].dsp_info.dsp_app.addr_phys);
			// impdrv_dspctl_tcm_config_p
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, read_cnt++, 0);
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, read_cnt++, 0);
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, read_cnt++, 0);
			/* impdrv_dspctl_load_dtcm */
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, read_cnt++, 0);
			// execute
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, read_cnt++, IMPDRV_REGVAL_OUT_INT_TIMER00);
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, read_cnt++, IMPDRV_REGVAL_CLR);
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, read_cnt++, IMPDRV_REGVAL_VDSP_GPIO_START);
		}

		ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
		if (ercd != IMPDRV_EC_OK)
		{
			core_testmsg(p_core_info, "%s=%d\n", "R_IMPDRV_Execute", ercd);
			goto error2;
		}

		if ((check_callback_ercd != IMPDRV_CB_RET_BEFORE_EXEC) || (check_callback_code != -1))
		{
			core_testmsg(p_core_info, "callback(returns IMPDRV_CB_RET_BEFORE_EXEC) fail\n");
			goto error2;
		}

		coretype_ctl->core_ctl[core_num].core_state = IMPDRV_STATE_INT;
		
		it_stub_reset();
		prepare_test_for_resume(handle, p_core_info);

		ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
		if (ercd != IMPDRV_EC_OK)
		{
			core_testmsg(p_core_info, "%s=%d\n", TEST_MSG_NAME, ercd);
			goto error2;
		}

		coretype_ctl->core_ctl[core_num].core_state = IMPDRV_STATE_READY;

		result = 0;
error2:
		util_mem_invalidate(tparam->long_claddr);

error1:
		SuccessTestPrint(result == 0, *tret, "(No.%d) %s%d ", testno++, type_to_name(core_type), core_num);
	}
}