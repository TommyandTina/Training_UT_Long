#define TEST_MSG_NAME "R_IMPDRV_Init"

#include "it_common.h"
#include "it_stub.h"

void prepare_test_for_init(st_impdrv_initdata_t *p_cmn_init_data);
static void set_state(st_impdrv_initdata_t *const p_data, e_impdrv_state_t state);
#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret);
static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_state(struct test_params_t *tparam, struct test_result_t *tret);
static void Return_error(struct test_params_t *tparam, struct test_result_t *tret);
#endif
static void Device_init(struct test_params_t *tparam, struct test_result_t *tret);

static uint32_t testno;

static void set_state(st_impdrv_initdata_t *const p_data, e_impdrv_state_t state)
{
	st_impdrv_core_info_t    core_info[IMPDRV_INNER_FIXED_VALUE];
    uint32_t                cnt;
	e_impdrv_core_type_t core_type;
	uint32_t core_num;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_data->p_work_addr;

	cnt = 0U;

	while (cnt < p_data->use_core_num)
	{
		core_info[cnt] = p_data->core_info[cnt];
		core_type = core_info[cnt].core_type;
		core_num = core_info[cnt].core_num;
		p_impdrv_ctl->coretype_ctl[core_type].core_ctl[core_num].core_state = state;
		cnt++;
	}
}

void prepare_test_for_init(st_impdrv_initdata_t *p_cmn_init_data)
{
#if defined (V4H) || defined (V4H2)
	static const uint32_t read_value_for_spminit[] = {
		0x0003ffffU,	/* Set IMPSLV.DSP0CT for corresponding to interrupt mode */
		0x0003ffffU,	/* Set IMPSLV.DSP1CT for corresponding to interrupt mode */
		0x0003ffffU,	/* Set IMPSLV.DSP2CT for corresponding to interrupt mode */
		0x0003ffffU,	/* Set IMPSLV.DSP3CT for corresponding to interrupt mode */
		0x00000000U,	/* Read the INTEN00 register in 'impdrv_cmnctl_init()' */
		0x0003ffffU,	/* Wite the INTEN00 register in 'impdrv_cmnctl_init()' */
		0x0000000aU,	/* Wite the SPM preset register in 'impdrv_cmnctl_reg_spm_preset()' */
		0x0000000aU,	/* Wite the SPM preset register in 'impdrv_cmnctl_reg_spm_preset()' */
		0x00000009U,	/* Wite the SPM preset register in 'impdrv_cmnctl_reg_spm_preset()' */
		0x00000000U,	/* Read the SPMCTRL4 register in 'impdrv_cmnctl_reg_spm_ena_access()' */
		0x80000000U,	
		0x00000000U,	/* Read the SPMCTRL4 register in 'impdrv_cmnctl_reg_spm_ena_access()' */
		0x80000000U,	
		0x00000001U,	/* Read the Memory status register in 'impdrv_cmnctl_reg_spm_chk_init()' */
		0x00000003U,	/* Read the Memory status register in 'impdrv_cmnctl_reg_spm_chk_init()' */
		0x80000000U,	/* Read the SPMCTRL4 register in 'impdrv_cmnctl_reg_spm_ena_access()' */
		0x00000000U,	
		0x80000000U,	/* Read the SPMCTRL4 register in 'impdrv_cmnctl_reg_spm_ena_access()' */
		0x00000000U		
	};
	uint32_t read_index;
	uint32_t k = 0U;

	/* Register read values before the Version check function. */
	read_index = 0U;
	while (read_index < (sizeof(read_value_for_spminit) / sizeof(read_value_for_spminit[0])))
	{
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, k++, read_value_for_spminit[read_index]);
		read_index++;
	}

	/* Register read values of the Version check function. */
	read_index = 0U;
	while (read_index < (sizeof(read_value_for_init) / sizeof(read_value_for_init[0])))
	{
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, k++, read_value_for_init[read_index]);
		// read value for impdrv_cnnctl_force_reset
		// if (IMPDRV_CNN_VCR == read_value_for_init[read_index])
		// {
		// 	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, k++, 0);
		// 	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, k++, 1);
		// 	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, k++, 0);
		// }
		read_index++;
	}

	it_stub_set_R_OSAL_PmGetRequiredState(IMPDRV_EC_OK, OSAL_PM_REQUIRED_STATE_RELEASED);

#elif (defined (V3M2) || defined (V3H1) || defined (V3H2))
	uint32_t start_addr = IMPDRV_DMA_S0SAR_ODD_BASE_ADDR;
	uint32_t j = 0U;
	uint32_t i = 0U;
	uint32_t k = 0U;
	uint32_t n = 0U;				/* to store k to resume */
	uint32_t loop = 0U;
	uint32_t k_backup = 0U;
	uint32_t read_index = 1;		/* the anchor of which element in read_value_for_init array */
	uint32_t int_mask_reg_val = 0U;
	uint32_t write_int_mask_val = 0U;
	uint32_t mb_init_val[] = { IMPDRV_REG_MB_MEM_INIT_ODD, IMPDRV_REG_MB_MEM_INIT_EVEN };
	const st_impdrv_inten_tbl_t     *p_inten_cnv_table;
    uint32_t                        cnv_table_num;

	/**********************************************************************************************
	 * This method is applied for initialization only in case all the cores that are ordered as
	 * in platform_soc above are used. In another word, use_core_num = 19.
	 * *******************************************************************************************/

	p_inten_cnv_table = s_impdrv_inten_reg_tbl[p_cmn_init_data->instance_num].p_inten_cnv_table;
	cnv_table_num = s_impdrv_inten_reg_tbl[p_cmn_init_data->instance_num].cnv_table_num;
	cnv_int_core_bit(p_cmn_init_data->core_info, p_cmn_init_data->use_core_num, p_inten_cnv_table, cnv_table_num, &write_int_mask_val);

	/* Read Interrupt Mask in impdrv_cmnctl_init() */
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, k++, int_mask_reg_val);

	/* Write Interrupt Mask in impdrv_cmnctl_init() */
	write_int_mask_val = ~write_int_mask_val & IMPDRV_REG_IMR_INIT_VAL;
	write_int_mask_val |= int_mask_reg_val  & (~IMPDRV_REG_IMR_INIT_VAL);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, k++, write_int_mask_val);

	/* Initalize Multi-Bank memory then check */
	for (i = 0; i < (sizeof(mb_init_val) / sizeof(mb_init_val[0])); i++)
	{
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, k++, ~mb_init_val[i]);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, k++, 0);
	}
	
	/* Check vesion of cores in InitStart */
	read_index = (uint32_t)p_cmn_init_data->core_info[0].core_type;
	while (read_index < (sizeof(read_value_for_init) / sizeof(read_value_for_init[0])))
	{
		if (true == close_device_io_flg)
		{
			/* This case is applied to Init as device_io's failed to be closed recently */
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, k, read_value_for_init[read_index]);
			k++;
		}
		else
		{
			/* This cases is for normal Initialization: Read version registers of cores
			   before DMA multibank initialization */
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, k, read_value_for_init[read_index]);
			k++;
			if (IMPDRV_DMA_VCR == read_value_for_init[read_index])
			{
				read_index++;
				break;
			}
		}
		read_index++;
	}

	if (true == close_device_io_flg)
	{
		return;
	}

	/* Wait the register read 8 times as in impdrv_dmactl_init_core() */
	while (loop < IMPDRV_DMA_LOOPTIME_VAL)
	{
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, k, 0);
		k++;
		loop++;
	}

	/* Readback confirm the RAM Test Register(RAMTSTR) as in impdrv_dmactl_init_core() */
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, k, 0);
	k++;

	/* Checking busy as in impdrv_dmactl_cl_pre() for ODD */
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, k, 0);
	k++;

	k_backup = k;

	while (j < IMPDRV_DMA_MB_WRITE_NUM_ODD)
	{
		/* For impdrv_dmactl_mb_init_pre() */
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, k + (j * 18), IMPDRV_DMA_SR_TEND | IMPDRV_DMA_SER_FIX);
		k++;

		/* For impdrv_dmactl_mb_init_main() */
		for (i = 0; i < IMPDRV_DMA_MB_INFO_SIZE1; i++)
		{
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, k + (j * 18), s_mb_init_info1[i].chk_val);
			if (true == s_mb_init_info1[i].read_back)
			{
				k++;
			}
		}
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, k + (j * 18), start_addr);
		k++;
		for (i = 0; i < IMPDRV_DMA_MB_INFO_SIZE2; i++)
		{
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, k + (j * 18), s_mb_init_info2[i].chk_val);
			if (true == s_mb_init_info2[i].read_back)
			{
				k++;
			}
		}
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, k + (j * 18), start_addr);
		k++;
		for (i = 0; i < IMPDRV_DMA_MB_INFO_SIZE3; i++)
		{
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, k + (j * 18), s_mb_init_info3[i].chk_val);
			if (true == s_mb_init_info3[i].read_back)
			{
				k++;
			}
		}

		/* Checking offset IMPDRV_REG_DMA_SR in impdrv_dmactl_mb_init_main() */
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, k + (j * 18), IMPDRV_DMA_SR_TEND);
		k++;
		n = k + (j * 18);

		start_addr += IMPDRV_DMA_S0SAR_OFFSET;
		k = k_backup;
		j++;
	}

	/* Resume read-reg */
	k = n;

	/* Checking busy as in impdrv_dmactl_cl_pre() for EVEN */
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, k, 0);
	k++;

	k_backup = k;
	j = 0;
	start_addr = IMPDRV_DMA_S0SAR_EVEN_BASE_ADDR;

	while (j < IMPDRV_DMA_MB_WRITE_NUM_EVEN)
	{
		/* For impdrv_dmactl_mb_init_pre() */
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, k + (j * 18), IMPDRV_DMA_SR_TEND | IMPDRV_DMA_SER_FIX);
		k++;

		/* For impdrv_dmactl_mb_init_main() */
		for (i = 0; i < IMPDRV_DMA_MB_INFO_SIZE1; i++)
		{
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, k + (j * 18), s_mb_init_info1[i].chk_val);
			if (true == s_mb_init_info1[i].read_back)
			{
				k++;
			}
		}
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, k + (j * 18), start_addr);
		k++;
		for (i = 0; i < IMPDRV_DMA_MB_INFO_SIZE2; i++)
		{
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, k + (j * 18), s_mb_init_info2[i].chk_val);
			if (true == s_mb_init_info2[i].read_back)
			{
				k++;
			}
		}
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, k + (j * 18), start_addr);
		k++;
		for (i = 0; i < IMPDRV_DMA_MB_INFO_SIZE3; i++)
		{
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, k + (j * 18), s_mb_init_info3[i].chk_val);
			if (true == s_mb_init_info3[i].read_back)
			{
				k++;
			}
		}

		/* Checking offset IMPDRV_REG_DMA_SR in impdrv_dmactl_mb_init_main() */
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, k + (j * 18), IMPDRV_DMA_SR_TEND);
		k++;
		n = k + (j * 18);

		start_addr += IMPDRV_DMA_S0SAR_OFFSET;
		k = k_backup;
		j++;
	}

	/* Resume with versions of the remaining cores */
	while (read_index < (sizeof(read_value_for_init) / sizeof(read_value_for_init[0])))
	{
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, n, read_value_for_init[read_index]);
		n++;
		read_index++;
	}

#else
	#error 'Oops ...! Build option for unsupported products.'
#endif
}

int32_t it_Init_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};
	init_test_flag = true;

#ifdef IT_ABNORMAL
	testno = 158;
	Parameter_error(tparam, &tret);
	Invalid_core(tparam, &tret);
	No_exec_core(tparam, &tret);
	Invalid_state(tparam, &tret);
	Return_error(tparam, &tret);
#endif
	Device_init(tparam, &tret);
	init_test_flag = false;

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
		prepare_test_for_init(p_initdata);
		p_initdata->core_info[0] = *(invalid_core_list.core_info + i);
		ercd = R_IMPDRV_Init(p_initdata, &handle);
		FailTestPrint(ercd, ((p_initdata->core_info[0].core_type == IMPDRV_CORE_TYPE_INVALID) ?
							IMPDRV_EC_NG_PARAM : IMPDRV_EC_NG_NOTSUPPORT),
							*tret, "(No.%d) Invalid core num for %s", testno++, type_to_name(p_initdata->core_info[0].core_type));
	}

	/* Invalid_error : Duplicate core type and core num */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	p_initdata->core_info[0].core_type = IMPDRV_CORE_TYPE_IMP;
	p_initdata->core_info[0].core_num = 0;
	p_initdata->core_info[1].core_type = IMPDRV_CORE_TYPE_IMP;
	p_initdata->core_info[1].core_num = 0;
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) Duplicate core type and core num", testno++);

	/* Invalid_error : Core_num = IMPDRV_CORE_NUM_MAX */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
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
		prepare_test_for_init(p_initdata);
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
		prepare_test_for_init(p_initdata);
		set_state(p_initdata, invalid_state[i]);
		ercd = R_IMPDRV_Init(p_initdata, &handle);
		FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) %s", testno++, state_to_name(invalid_state[i]));
	}
}

static void Return_error(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
    impdrv_ctrl_handle_t handle;

	/* Failed to create mutex: R_OSAL_ThsyncMutexCreate returns OSAL_RETURN_FAIL */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_impdrv_cmnctl_reg_set_checksum(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_reg_set_checksum returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));
	
	/* Failed to create mutex: R_OSAL_ThsyncMutexCreate returns OSAL_RETURN_PAR */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_ThsyncMutexCreate(OSAL_RETURN_PAR);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexCreate returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Failed to create mutex: R_OSAL_ThsyncMutexCreate returns OSAL_RETURN_MEM */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_ThsyncMutexCreate(OSAL_RETURN_MEM);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexCreate returns %s", testno++, osal_err_to_name(OSAL_RETURN_MEM));

	/* Failed to create mutex: R_OSAL_ThsyncMutexCreate returns OSAL_RETURN_BUSY */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_ThsyncMutexCreate(OSAL_RETURN_BUSY);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexCreate returns %s", testno++, osal_err_to_name(OSAL_RETURN_BUSY));
	
	/* Failed to create mutex: R_OSAL_ThsyncMutexCreate returns OSAL_RETURN_ID */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_ThsyncMutexCreate(OSAL_RETURN_ID);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexCreate returns %s", testno++, osal_err_to_name(OSAL_RETURN_ID));

	/* Failed to create mutex: R_OSAL_ThsyncMutexCreate returns OSAL_RETURN_STATE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_ThsyncMutexCreate(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexCreate returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Failed to create mutex: R_OSAL_ThsyncMutexCreate returns OSAL_RETURN_FAIL */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_ThsyncMutexCreate(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexCreate returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_TIME */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_TIME);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_TIME));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_PAR */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_PAR);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_BUSY */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_BUSY);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_BUSY));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_HANDLE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_STATE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_FAIL */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Fail to open device for IMP RAM: R_OSAL_IoDeviceOpen returns OSAL_RETURN_PAR */
	set_input_data_before_init(tparam);
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_PAR, "imp_top_00");
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for IMP TOP returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to open device for IMP RAM: R_OSAL_IoDeviceOpen returns OSAL_RETURN_MEM */
	set_input_data_before_init(tparam);
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_MEM, "imp_top_00");
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for IMP TOP returns %s", testno++, osal_err_to_name(OSAL_RETURN_MEM));

	/* Fail to open device for IMP RAM: R_OSAL_IoDeviceOpen returns OSAL_RETURN_DEV */
	set_input_data_before_init(tparam);
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_DEV, "imp_top_00");
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for IMP TOP returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to open device for IMP RAM: R_OSAL_IoDeviceOpen returns OSAL_RETURN_CONF */
	set_input_data_before_init(tparam);
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_CONF, "imp_top_00");
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for IMP TOP returns %s", testno++, osal_err_to_name(OSAL_RETURN_CONF));

	/* Fail to open device for IMP RAM: R_OSAL_IoDeviceOpen returns OSAL_RETURN_BUSY */
	set_input_data_before_init(tparam);
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_BUSY, "imp_top_00");
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for IMP TOP returns %s", testno++, osal_err_to_name(OSAL_RETURN_BUSY));

	/* Fail to open device for IMP RAM: R_OSAL_IoDeviceOpen returns OSAL_RETURN_ID */
	set_input_data_before_init(tparam);
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_ID, "imp_top_00");
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for IMP TOP returns %s", testno++, osal_err_to_name(OSAL_RETURN_ID));

	/* Fail to open device for IMP RAM: R_OSAL_IoDeviceOpen returns OSAL_RETURN_STATE */
	set_input_data_before_init(tparam);
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_STATE, "imp_top_00");
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for IMP TOP returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to open device for IMP RAM: R_OSAL_IoDeviceOpen returns OSAL_RETURN_FAIL */
	set_input_data_before_init(tparam);
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_FAIL, "imp_top_00");
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for IMP TOP returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Fail to set required state for IMP RAM: R_OSAL_PmSetRequiredState returns OSAL_RETURN_PAR */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_PAR, "imp_top_00", OSAL_PM_REQUIRED_STATE_REQUIRED);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState for IMP TOP returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to set required state for IMP RAM: R_OSAL_PmSetRequiredState returns OSAL_RETURN_DEV */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_DEV, "imp_top_00", OSAL_PM_REQUIRED_STATE_REQUIRED);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState for IMP TOP returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to set required state for IMP RAM: R_OSAL_PmSetRequiredState returns OSAL_RETURN_TIME */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_TIME, "imp_top_00", OSAL_PM_REQUIRED_STATE_REQUIRED);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState for IMP TOP returns %s", testno++, osal_err_to_name(OSAL_RETURN_TIME));

	/* Fail to set required state for IMP RAM: R_OSAL_PmSetRequiredState returns OSAL_RETURN_HANDLE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_HANDLE, "imp_top_00", OSAL_PM_REQUIRED_STATE_REQUIRED);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState for IMP TOP returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to set required state for IMP RAM: R_OSAL_PmSetRequiredState returns OSAL_RETURN_STATE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_STATE, "imp_top_00", OSAL_PM_REQUIRED_STATE_REQUIRED);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState for IMP TOP returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to set required state for IMP RAM: R_OSAL_PmSetRequiredState returns OSAL_RETURN_UNSUPPORTED_OPERATION */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_UNSUPPORTED_OPERATION, "imp_top_00", OSAL_PM_REQUIRED_STATE_REQUIRED);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState for IMP TOP returns %s", testno++, osal_err_to_name(OSAL_RETURN_UNSUPPORTED_OPERATION));

	/* Fail to set required state for IMP RAM: R_OSAL_PmSetRequiredState returns OSAL_RETURN_FAIL */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_FAIL, "imp_top_00", OSAL_PM_REQUIRED_STATE_REQUIRED);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState for IMP TOP returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Fail to register interrupt for IMP RAM: R_OSAL_InterruptRegisterIsr returns OSAL_RETURN_PAR */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_InterruptRegisterIsr(OSAL_RETURN_PAR);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_InterruptRegisterIsr returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to register interrupt for IMP RAM: R_OSAL_InterruptRegisterIsr returns OSAL_RETURN_DEV */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_InterruptRegisterIsr(OSAL_RETURN_DEV);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_InterruptRegisterIsr returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to register interrupt for IMP RAM: R_OSAL_InterruptRegisterIsr returns OSAL_RETURN_BUSY */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_InterruptRegisterIsr(OSAL_RETURN_BUSY);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_InterruptRegisterIsr returns %s", testno++, osal_err_to_name(OSAL_RETURN_BUSY));

	/* Fail to register interrupt for IMP RAM: R_OSAL_InterruptRegisterIsr returns OSAL_RETURN_HANDLE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_InterruptRegisterIsr(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_InterruptRegisterIsr returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to register interrupt for IMP RAM: R_OSAL_InterruptRegisterIsr returns OSAL_RETURN_STATE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_InterruptRegisterIsr(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_InterruptRegisterIsr returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to register interrupt for IMP RAM: R_OSAL_InterruptRegisterIsr returns OSAL_RETURN_FAIL */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_InterruptRegisterIsr(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_InterruptRegisterIsr returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Fail to enable interrupt for IMP RAM: R_OSAL_InterruptEnableIsr returns OSAL_RETURN_PAR */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_InterruptEnableIsr(OSAL_RETURN_PAR);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_InterruptEnableIsr returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to enable interrupt for IMP RAM: R_OSAL_InterruptEnableIsr returns OSAL_RETURN_DEV */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_InterruptEnableIsr(OSAL_RETURN_DEV);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_InterruptEnableIsr returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to enable interrupt for IMP RAM: R_OSAL_InterruptEnableIsr returns OSAL_RETURN_HANDLE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_InterruptEnableIsr(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_InterruptEnableIsr returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to enable interrupt for IMP RAM: R_OSAL_InterruptEnableIsr returns OSAL_RETURN_STATE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_InterruptEnableIsr(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_InterruptEnableIsr returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to enable interrupt for IMP RAM: R_OSAL_InterruptEnableIsr returns OSAL_RETURN_FAIL */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_InterruptEnableIsr(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_InterruptEnableIsr returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Fail to read INTerrupt Enable register: R_OSAL_IoRead32 returns OSAL_RETURN_PAR */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
#if defined (V4H) || defined (V4H2)
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_PAR, 4, 0);
#elif (defined (V3M2) || defined (V3H1) || defined (V3H2))
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_PAR, 0, 0);
#endif
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for INTerrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to read INTerrupt Enable register: R_OSAL_IoRead32 returns OSAL_RETURN_DEV */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
#if defined (V4H) || defined (V4H2)
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_DEV, 4, 0);
#elif (defined (V3M2) || defined (V3H1) || defined (V3H2))
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_DEV, 0, 0);
#endif
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for INTerrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to read INTerrupt Enable register: R_OSAL_IoRead32 returns OSAL_RETURN_IO */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
#if defined (V4H) || defined (V4H2)
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_IO, 4, 0);
#elif (defined (V3M2) || defined (V3H1) || defined (V3H2))
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_IO, 0, 0);
#endif
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for INTerrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_IO));

	/* Fail to read INTerrupt Enable register: R_OSAL_IoRead32 returns OSAL_RETURN_HANDLE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
#if defined (V4H) || defined (V4H2)
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_HANDLE, 4, 0);
#elif (defined (V3M2) || defined (V3H1) || defined (V3H2))
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_HANDLE, 0, 0);
#endif
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for INTerrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to read INTerrupt Enable register: R_OSAL_IoRead32 returns OSAL_RETURN_STATE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
#if defined (V4H) || defined (V4H2)
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_STATE, 4, 0);
#elif (defined (V3M2) || defined (V3H1) || defined (V3H2))
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_STATE, 0, 0);
#endif
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for INTerrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to read INTerrupt Enable register: R_OSAL_IoRead32 returns OSAL_RETURN_FAIL */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
#if defined (V4H) || defined (V4H2)
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_FAIL, 4, 0);
#elif (defined (V3M2) || defined (V3H1) || defined (V3H2))
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_FAIL, 0, 0);
#endif
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for INTerrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Fail to write register for IMP RAM: R_OSAL_IoWrite32 returns OSAL_RETURN_PAR */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_PAR, s_impdrv_inten_addr[p_initdata->instance_num], 0);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for INTerrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to write register for IMP RAM: R_OSAL_IoWrite32 returns OSAL_RETURN_DEV */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_DEV, s_impdrv_inten_addr[p_initdata->instance_num], 0);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for INTerrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to write register for IMP RAM: R_OSAL_IoWrite32 returns OSAL_RETURN_IO */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_IO, s_impdrv_inten_addr[p_initdata->instance_num], 0);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for INTerrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_IO));

	/* Fail to write register for IMP RAM: R_OSAL_IoWrite32 returns OSAL_RETURN_HANDLE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_HANDLE, s_impdrv_inten_addr[p_initdata->instance_num], 0);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for INTerrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to write register for IMP RAM: R_OSAL_IoWrite32 returns OSAL_RETURN_STATE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_STATE, s_impdrv_inten_addr[p_initdata->instance_num], 0);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for INTerrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to write register for IMP RAM: R_OSAL_IoWrite32 returns OSAL_RETURN_FAIL */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_FAIL, s_impdrv_inten_addr[p_initdata->instance_num], 0);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 for INTerrupt Enable register returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Read back value of INTerrupt Enable for IMP RAM is incorrect: R_OSAL_IoRead32 returns incorrect value */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
#if defined (V4H) || defined (V4H2)
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 5, 5);
#elif (defined (V3M2) || defined (V3H1) || defined (V3H2))
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 1, 5);
#endif
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_CHECKFAIL, *tret, "(No.%d) R_OSAL_IoRead32 for INTerrupt Enable register returns invalid value", testno++);

	/* Fail to open device for HW resource: R_OSAL_IoDeviceOpen returns OSAL_RETURN_PAR */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_PAR, "imp_spmi_00");
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for HW resources register returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to open device for HW resource: R_OSAL_IoDeviceOpen returns OSAL_RETURN_MEM */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_MEM, "imp_spmi_00");
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for HW resources register returns %s", testno++, osal_err_to_name(OSAL_RETURN_MEM));

	/* Fail to open device for HW resource: R_OSAL_IoDeviceOpen returns OSAL_RETURN_DEV */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_DEV, "imp_spmi_00");
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for HW resources register returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to open device for HW resource: R_OSAL_IoDeviceOpen returns OSAL_RETURN_CONF */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_CONF, "imp_spmi_00");
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for HW resources register returns %s", testno++, osal_err_to_name(OSAL_RETURN_CONF));

	/* Fail to open device for HW resource: R_OSAL_IoDeviceOpen returns OSAL_RETURN_BUSY */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_BUSY, "imp_spmi_00");
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for HW resources register returns %s", testno++, osal_err_to_name(OSAL_RETURN_BUSY));

	/* Fail to open device for HW resource: R_OSAL_IoDeviceOpen returns OSAL_RETURN_ID */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_ID, "imp_spmi_00");
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for HW resources register returns %s", testno++, osal_err_to_name(OSAL_RETURN_ID));

	/* Fail to open device for HW resource: R_OSAL_IoDeviceOpen returns OSAL_RETURN_STATE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_STATE, "imp_spmi_00");
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for HW resources register returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to open device for HW resource: R_OSAL_IoDeviceOpen returns OSAL_RETURN_FAIL */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_FAIL, "imp_spmi_00");
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for HW resources register returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Fail to set required state for HW resource: R_OSAL_PmSetRequiredState returns OSAL_RETURN_PAR */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_PAR, "imp_spmi_00", OSAL_PM_REQUIRED_STATE_REQUIRED);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (Required) for HW resource returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to set required state for HW resource: R_OSAL_PmSetRequiredState returns OSAL_RETURN_DEV */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_DEV, "imp_spmi_00", OSAL_PM_REQUIRED_STATE_REQUIRED);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (Required) for HW resource returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to set required state for HW resource: R_OSAL_PmSetRequiredState returns OSAL_RETURN_TIME */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_TIME, "imp_spmi_00", OSAL_PM_REQUIRED_STATE_REQUIRED);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (Required) for HW resource returns %s", testno++, osal_err_to_name(OSAL_RETURN_TIME));

	/* Fail to set required state for HW resource: R_OSAL_PmSetRequiredState returns OSAL_RETURN_HANDLE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_HANDLE, "imp_spmi_00", OSAL_PM_REQUIRED_STATE_REQUIRED);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (Required) for HW resource returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to set required state for HW resource: R_OSAL_PmSetRequiredState returns OSAL_RETURN_STATE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_STATE, "imp_spmi_00", OSAL_PM_REQUIRED_STATE_REQUIRED);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (Required) for HW resource returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to set required state for HW resource: R_OSAL_PmSetRequiredState returns OSAL_RETURN_UNSUPPORTED_OPERATION */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_UNSUPPORTED_OPERATION, "imp_spmi_00", OSAL_PM_REQUIRED_STATE_REQUIRED);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (Required) for HW resource returns %s", testno++, osal_err_to_name(OSAL_RETURN_UNSUPPORTED_OPERATION));

	/* Fail to set required state for HW resource: R_OSAL_PmSetRequiredState returns OSAL_RETURN_FAIL */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_FAIL, "imp_spmi_00", OSAL_PM_REQUIRED_STATE_REQUIRED);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (Required) for HW resource returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Fail to open device for IMP0: R_OSAL_IoDeviceOpen returns OSAL_RETURN_PAR */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_PAR, "imp_00");
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for IMP0 register returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to open device for IMP0: R_OSAL_IoDeviceOpen returns OSAL_RETURN_MEM */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_MEM, "imp_00");
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for IMP0 register returns %s", testno++, osal_err_to_name(OSAL_RETURN_MEM));

	/* Fail to open device for IMP0: R_OSAL_IoDeviceOpen returns OSAL_RETURN_DEV */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_DEV, "imp_00");
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for IMP0 register returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to open device for IMP0: R_OSAL_IoDeviceOpen returns OSAL_RETURN_CONF */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_CONF, "imp_00");
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for IMP0 register returns %s", testno++, osal_err_to_name(OSAL_RETURN_CONF));

	/* Fail to open device for IMP0: R_OSAL_IoDeviceOpen returns OSAL_RETURN_BUSY */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_BUSY, "imp_00");
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for IMP0 register returns %s", testno++, osal_err_to_name(OSAL_RETURN_BUSY));

	/* Fail to open device for IMP0: R_OSAL_IoDeviceOpen returns OSAL_RETURN_ID */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_ID, "imp_00");
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for IMP0 register returns %s", testno++, osal_err_to_name(OSAL_RETURN_ID));

	/* Fail to open device for IMP0: R_OSAL_IoDeviceOpen returns OSAL_RETURN_STATE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_STATE, "imp_00");
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for IMP0 register returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to open device for IMP0: R_OSAL_IoDeviceOpen returns OSAL_RETURN_FAIL */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_IoDeviceOpen(OSAL_RETURN_FAIL, "imp_00");
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceOpen for IMP0 register returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Fail to set required state for IMP0 after open device: R_OSAL_PmSetRequiredState returns OSAL_RETURN_PAR */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_PAR, "imp_00", OSAL_PM_REQUIRED_STATE_REQUIRED);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (Required) for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to set required state for IMP0 after open device: R_OSAL_PmSetRequiredState returns OSAL_RETURN_DEV */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_DEV, "imp_00", OSAL_PM_REQUIRED_STATE_REQUIRED);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (Required) for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to set required state for IMP0 after open device: R_OSAL_PmSetRequiredState returns OSAL_RETURN_TIME */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_TIME, "imp_00", OSAL_PM_REQUIRED_STATE_REQUIRED);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (Required) for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_TIME));

	/* Fail to set required state for IMP0 after open device: R_OSAL_PmSetRequiredState returns OSAL_RETURN_HANDLE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_HANDLE, "imp_00", OSAL_PM_REQUIRED_STATE_REQUIRED);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (Required) for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to set required state for IMP0 after open device: R_OSAL_PmSetRequiredState returns OSAL_RETURN_STATE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_STATE, "imp_00", OSAL_PM_REQUIRED_STATE_REQUIRED);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (Required) for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to set required state for IMP0 after open device: R_OSAL_PmSetRequiredState returns OSAL_RETURN_UNSUPPORTED_OPERATION */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_UNSUPPORTED_OPERATION, "imp_00", OSAL_PM_REQUIRED_STATE_REQUIRED);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (Required) for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_UNSUPPORTED_OPERATION));

	/* Fail to set required state for IMP0 after open device: R_OSAL_PmSetRequiredState returns OSAL_RETURN_FAIL */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_FAIL, "imp_00", OSAL_PM_REQUIRED_STATE_REQUIRED);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (Required) for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Fail to read register version for IMP0: R_OSAL_IoRead32 returns OSAL_RETURN_PAR */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
#if defined (V4H) || defined (V4H2)
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_PAR, 19, IMPDRV_IMP_VCR);
#elif (defined (V3M2) || defined (V3H1) || defined (V3H2))
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_PAR, 6, IMPDRV_IMP_VCR);
#endif
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for version register of IMP0, returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to read register version for IMP0: R_OSAL_IoRead32 returns OSAL_RETURN_DEV */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
#if defined (V4H) || defined (V4H2)
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_DEV, 19, IMPDRV_IMP_VCR);
#elif (defined (V3M2) || defined (V3H1) || defined (V3H2))
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_DEV, 6, IMPDRV_IMP_VCR);
#endif
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for version register of IMP0, returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to read register version for IMP0: R_OSAL_IoRead32 returns OSAL_RETURN_IO */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
#if defined (V4H) || defined (V4H2)
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_IO, 19, IMPDRV_IMP_VCR);
#elif (defined (V3M2) || defined (V3H1) || defined (V3H2))
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_IO, 6, IMPDRV_IMP_VCR);
#endif
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for version register of IMP0, returns %s", testno++, osal_err_to_name(OSAL_RETURN_IO));

	/* Fail to read register version for IMP0: R_OSAL_IoRead32 returns OSAL_RETURN_HANDLE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
#if defined (V4H) || defined (V4H2)
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_HANDLE, 19, IMPDRV_IMP_VCR);
#elif (defined (V3M2) || defined (V3H1) || defined (V3H2))
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_HANDLE, 6, IMPDRV_IMP_VCR);
#endif
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for version register of IMP0, returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to read register version for IMP0: R_OSAL_IoRead32 returns OSAL_RETURN_STATE */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
#if defined (V4H) || defined (V4H2)
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_STATE, 19, IMPDRV_IMP_VCR);
#elif (defined (V3M2) || defined (V3H1) || defined (V3H2))
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_STATE, 6, IMPDRV_IMP_VCR);
#endif
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for version register of IMP0, returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to read register version for IMP0: R_OSAL_IoRead32 returns OSAL_RETURN_FAIL */
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
#if defined (V4H) || defined (V4H2)
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_FAIL, 19, IMPDRV_IMP_VCR);
#elif (defined (V3M2) || defined (V3H1) || defined (V3H2))
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_FAIL, 6, IMPDRV_IMP_VCR);
#endif
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for version register of IMP0, returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Register version of IMP0 holds invalid value: R_OSAL_IoRead32 returns OSAL_RETURN_OK */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 19, 0);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 for version register of IMP0 returns invalid value", testno++);

#if !defined (V4H) && !defined (V4H2)
	/* Fail to set required state for IMP0 when closing device: R_OSAL_PmSetRequiredState returns OSAL_RETURN_PAR */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_PAR, "imp_00", OSAL_PM_REQUIRED_STATE_RELEASED);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (Released) for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to set required state for IMP0 when closing device: R_OSAL_PmSetRequiredState returns OSAL_RETURN_DEV */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_DEV, "imp_00", OSAL_PM_REQUIRED_STATE_RELEASED);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (Released) for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to set required state for IMP0 when closing device: R_OSAL_PmSetRequiredState returns OSAL_RETURN_TIME */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_TIME, "imp_00", OSAL_PM_REQUIRED_STATE_RELEASED);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (Released) for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_TIME));

	/* Fail to set required state for IMP0 when closing device: R_OSAL_PmSetRequiredState returns OSAL_RETURN_HANDLE */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_HANDLE, "imp_00", OSAL_PM_REQUIRED_STATE_RELEASED);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (Released) for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to set required state for IMP0 when closing device: R_OSAL_PmSetRequiredState returns OSAL_RETURN_STATE */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_STATE, "imp_00", OSAL_PM_REQUIRED_STATE_RELEASED);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (Released) for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to set required state for IMP0 when closing device: R_OSAL_PmSetRequiredState returns OSAL_RETURN_UNSUPPORTED_OPERATION */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_UNSUPPORTED_OPERATION, "imp_00", OSAL_PM_REQUIRED_STATE_RELEASED);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (Released) for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_UNSUPPORTED_OPERATION));

	/* Fail to set required state for IMP0 when closing device: R_OSAL_PmSetRequiredState returns OSAL_RETURN_FAIL */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_FAIL, "imp_00", OSAL_PM_REQUIRED_STATE_RELEASED);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState (Released) for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));
#else
	testno += 7;
#endif

	/* Fail to close device for IMP RAM: R_OSAL_IoDeviceClose returns OSAL_RETURN_DEV */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_IoDeviceClose(OSAL_RETURN_DEV, "imp_00");
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceClose for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to close device for IMP RAM: R_OSAL_IoDeviceClose returns OSAL_RETURN_BUSY */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_IoDeviceClose(OSAL_RETURN_BUSY, "imp_00");
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceClose for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_BUSY));

	/* Fail to close device for IMP RAM: R_OSAL_IoDeviceClose returns OSAL_RETURN_HANDLE */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_IoDeviceClose(OSAL_RETURN_HANDLE, "imp_00");
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceClose for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to close device for IMP RAM: R_OSAL_IoDeviceClose returns OSAL_RETURN_STATE */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_IoDeviceClose(OSAL_RETURN_STATE, "imp_00");
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceClose for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to close device for IMP RAM: R_OSAL_IoDeviceClose returns OSAL_RETURN_FAIL */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_IoDeviceClose(OSAL_RETURN_FAIL, "imp_00");
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoDeviceClose for IMP0 returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_HANDLE */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_STATE */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_FAIL */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_FAIL */
	initialize_impdrvctl();
	set_input_data_before_init(tparam);
	prepare_test_for_init(p_initdata);
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_FAIL);
	it_stub_set_R_OSAL_PmSetRequiredState(OSAL_RETURN_PAR, "vdsp00_00", OSAL_PM_REQUIRED_STATE_REQUIRED);
	ercd = R_IMPDRV_Init(p_initdata, &handle);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PMSYSTEMERROR, *tret, "(No.%d) R_OSAL_PmSetRequiredState for VDSP returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));
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

	prepare_test_for_init(p_initdata);

	ercd = R_IMPDRV_Init(p_initdata, &handle);
	if (ercd != IMPDRV_EC_OK) {
		testmsg("(No.%d) fail(test NG %d)\n", testno++, ercd);
	} else if ((handle == NULL) || (handle != p_initdata->p_work_addr)) {
		testmsg("(No.%d) Parameter fail(test NG)\n", testno++);
	} else if (check_state(p_initdata, coretype_ctl, IMPDRV_STATE_INIT, use_core_num)) {
		testmsg("(No.%d) States fail(test NG)\n", testno++);
	} else {
		testmsg("(No.%d) Success(test OK)\n", testno++);
		result = 0;
	}

	CountResult(result == 0, tret);
}