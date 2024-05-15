
#define TEST_MSG_NAME "R_IMPDRV_BusIfCheck"

#include "it_common.h"
#include "it_stub.h"

#define MEM_RANGE	(IMPDRV_IMP_MEM_SIZE_BUSCHK / sizeof(uint32_t))

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
static uint32_t mem[MEM_RANGE];
static uint32_t mem_hw_addr = 128U;
static uint32_t inten_val = 1U;
static uint32_t reg_val = 0U;

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
#if !defined (V4H) && !defined (V4H2)
	uint32_t i = 0U;
	uint32_t j = 0U;
	uint32_t read_index;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)handle;
	tp_arr_info_t transfer_config;
	st_impdrv_imp_write_info_tbl_t *imp_transfer_config;
	st_impdrv_imp_write_info_tbl_t *imps_transfer_config;
	st_impdrv_dma_write_info_tbl_t *dma_transfer_config;
	tp_arr_info_t test_pattern_info;
	tp_test_pattern_t *test_pattern_tbl;

	p_impdrv_ctl->common_ctl.irq_mask_value = 0U;

	test_pattern_info = get_data_info(p_core_info->core_type, IT_IMPDRV_CMD_BUSIF_TEST_PATTERN);
	test_pattern_tbl = (tp_test_pattern_t *)test_pattern_info.info;

	for (i = 0; i < test_pattern_info.range; i++)
	{
		memset(&mem[8U * i], (test_pattern_tbl + i)->pattern, (test_pattern_tbl + i)->size);
		memset(&mem[(MEM_RANGE / 2) + 8U * i], (test_pattern_tbl + i)->pattern, (test_pattern_tbl + i)->size);
	}

	if (IMPDRV_CORE_TYPE_IMP == p_core_info->core_type)
	{
		i = 0;
		transfer_config = get_data_info(p_core_info->core_type, IT_IMPDRV_CMD_DMA_TRANSFER);
		imp_transfer_config = (st_impdrv_imp_write_info_tbl_t *)transfer_config.info;

		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, inten_val);
		read_index = 0;
		while (IMPDRV_INTEN_CORE_MAX > read_index)
        {
            if ((s_impdrv_inten_cnv_table[read_index].core_type == p_core_info->core_type)
                && (s_impdrv_inten_cnv_table[read_index].core_num == p_core_info->core_num))
            {
                reg_val = IMPDRV_REG_IMR_INIT_VAL;
                reg_val &= ~(s_impdrv_inten_cnv_table[read_index].bit);
                break;
            }
            read_index++;
        }
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, reg_val);
		it_stub_set_R_OSAL_MmngrGetSize(OSAL_RETURN_OK, 0, IMPDRV_IMP_MEM_SIZE_BUSCHK);
		it_stub_set_R_OSAL_MmngrGetCpuPtr(OSAL_RETURN_OK, 0, mem);
		it_stub_set_R_OSAL_MmngrGetHwAddr(OSAL_RETURN_OK, 0, mem_hw_addr);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0);

		/* i = 3 */
		for (j = 0; j < transfer_config.range; j++)
		{
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i, (imp_transfer_config + j)->chk_val);
			i++;
		}

		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, mem_hw_addr);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, mem_hw_addr + (IMPDRV_IMP_MEM_SIZE_BUSCHK / 2U));
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, (((IMPDRV_IMP_MEM_SIZE_BUSCHK / 2U) / 256U) << 16U) | 64U);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, inten_val);

		it_stub_set_R_OSAL_MqReceiveForTimePeriod(OSAL_RETURN_OK, p_core_info->core_num);
	}
#if !defined (V3M2)
	else if (IMPDRV_CORE_TYPE_IMP_SLIM == p_core_info->core_type)
	{
		i = 0;
		transfer_config = get_data_info(p_core_info->core_type, IT_IMPDRV_CMD_DMA_TRANSFER);
		imps_transfer_config = (st_impdrv_imp_write_info_tbl_t *)transfer_config.info;

		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, inten_val);
		read_index = 0;
		while (IMPDRV_INTEN_CORE_MAX > read_index)
        {
            if ((s_impdrv_inten_cnv_table[read_index].core_type == p_core_info->core_type)
                && (s_impdrv_inten_cnv_table[read_index].core_num == p_core_info->core_num))
            {
                reg_val = IMPDRV_REG_IMR_INIT_VAL;
                reg_val &= ~(s_impdrv_inten_cnv_table[read_index].bit);
                break;
            }
            read_index++;
        }
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, reg_val);
		it_stub_set_R_OSAL_MmngrGetSize(OSAL_RETURN_OK, 0, IMPDRV_IMPS_MEM_SIZE_BUSCHK);
		it_stub_set_R_OSAL_MmngrGetCpuPtr(OSAL_RETURN_OK, 0, mem);
		it_stub_set_R_OSAL_MmngrGetHwAddr(OSAL_RETURN_OK, 0, mem_hw_addr);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0);

		/* i = 3 */
		for (j = 0; j < transfer_config.range; j++)
		{
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i, (imps_transfer_config + j)->chk_val);
			i++;
		}

		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, mem_hw_addr);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, mem_hw_addr + (IMPDRV_IMPS_MEM_SIZE_BUSCHK / 2U));
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, (((IMPDRV_IMPS_MEM_SIZE_BUSCHK / 2U) / 256U) << 16U) | 64U);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, inten_val);

		it_stub_set_R_OSAL_MqReceiveForTimePeriod(OSAL_RETURN_OK, p_core_info->core_num);
	}
#endif
	else if (IMPDRV_CORE_TYPE_DMAC == p_core_info->core_type)
	{
		i = 0;
		transfer_config = get_data_info(p_core_info->core_type, IT_IMPDRV_CMD_DMA_TRANSFER);
		dma_transfer_config = (st_impdrv_dma_write_info_tbl_t *)transfer_config.info;

		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, inten_val);
		read_index = 0;
		while (IMPDRV_INTEN_CORE_MAX > read_index)
        {
            if ((s_impdrv_inten_cnv_table[read_index].core_type == p_core_info->core_type)
                && (s_impdrv_inten_cnv_table[read_index].core_num == p_core_info->core_num))
            {
                reg_val = IMPDRV_REG_IMR_INIT_VAL;
                reg_val &= ~(s_impdrv_inten_cnv_table[read_index].bit);
                break;
            }
            read_index++;
        }
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, reg_val);
		it_stub_set_R_OSAL_MmngrGetSize(OSAL_RETURN_OK, 0, IMPDRV_DMA_MEM_SIZE_BUSCHK);
		it_stub_set_R_OSAL_MmngrGetCpuPtr(OSAL_RETURN_OK, 0, mem);
		it_stub_set_R_OSAL_MmngrGetHwAddr(OSAL_RETURN_OK, 0, mem_hw_addr);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0x00000071U | IMPDRV_DMA_SER_FIX);

		/* i = 4 */
		for (j = 0; j < transfer_config.range; j++)
		{
			it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i, (dma_transfer_config + j)->chk_val);
			i++;
		}

		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, mem_hw_addr);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, mem_hw_addr + (IMPDRV_DMA_MEM_SIZE_BUSCHK / 2U));
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, ((IMPDRV_DMA_MEM_SIZE_BUSCHK / 2U) / 256U) | 0x01000000U);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, inten_val);

		it_stub_set_R_OSAL_MqReceiveForTimePeriod(OSAL_RETURN_OK, p_core_info->core_num);
	}
	else
	{
		/* do nothing */
	}
#endif /* !defined (V4H) && !defined (V4H2) */
}

int32_t it_BusIfCheck_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

	memcpy(&chk_resource_bk, &tparam->chk_resource, sizeof(st_impdrv_chk_resource_t));

#ifdef IT_ABNORMAL
	testno = 153;
	Parameter_error(tparam, &tret);
	testno = 160;
	Invalid_core(tparam, &tret);
	testno = 165;
	No_exec_core(tparam, &tret);
	testno = 179;
	Invalid_state(tparam, &tret);
	testno = 183;
	Return_error(tparam, &tret);
#endif
	testno = 251;
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
	st_impdrv_core_info_t *p_core_info = get_core_list(IT_IMPDRV_CMD_BUSIF, 0).core_info;

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_TIME */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_TIME);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_TIME));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_PAR */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_PAR);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_BUSY */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_BUSY);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_BUSY));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_HANDLE */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_STATE */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Failed to lock mutex: R_OSAL_ThsyncMutexLockForTimePeriod returns OSAL_RETURN_FAIL */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexLockForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

#if !defined (V4H) && !defined (V4H2)
	/* Fail to read register: R_OSAL_IoRead32 returns OSAL_RETURN_PAR */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_PAR, 0, 0);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to read register: R_OSAL_IoRead32 returns OSAL_RETURN_DEV */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_DEV, 0, 0);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to read register: R_OSAL_IoRead32 returns OSAL_RETURN_IO */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_IO, 0, 0);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 returns %s", testno++, osal_err_to_name(OSAL_RETURN_IO));

	/* Fail to read register: R_OSAL_IoRead32 returns OSAL_RETURN_HANDLE */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_HANDLE, 0, 0);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to read register: R_OSAL_IoRead32 returns OSAL_RETURN_STATE */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_STATE, 0, 0);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to read register: R_OSAL_IoRead32 returns OSAL_RETURN_FAIL */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_FAIL, 0, 0);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoRead32 returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Fail to write register: R_OSAL_IoWrite32 returns OSAL_RETURN_PAR */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_PAR, RCVDRV_IMP_OFFSET_ALL, 0U);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Fail to write register: R_OSAL_IoWrite32 returns OSAL_RETURN_DEV */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_DEV, RCVDRV_IMP_OFFSET_ALL, 0U);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 returns %s", testno++, osal_err_to_name(OSAL_RETURN_DEV));

	/* Fail to write register: R_OSAL_IoWrite32 returns OSAL_RETURN_IO */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_IO, RCVDRV_IMP_OFFSET_ALL, 0U);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 returns %s", testno++, osal_err_to_name(OSAL_RETURN_IO));

	/* Fail to write register: R_OSAL_IoWrite32 returns OSAL_RETURN_HANDLE */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_HANDLE, RCVDRV_IMP_OFFSET_ALL, 0U);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Fail to write register: R_OSAL_IoWrite32 returns OSAL_RETURN_STATE */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_STATE, RCVDRV_IMP_OFFSET_ALL, 0U);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Fail to write register: R_OSAL_IoWrite32 returns OSAL_RETURN_FAIL */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_IoWrite32(OSAL_RETURN_FAIL, RCVDRV_IMP_OFFSET_ALL, 0U);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_IoWrite32 returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Failed to get memory size: R_OSAL_MmngrGetSize returns OSAL_RETURN_PAR */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MmngrGetSize(OSAL_RETURN_PAR, 0, 512U);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MmngrGetSize returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Failed to get memory size: R_OSAL_MmngrGetSize returns OSAL_RETURN_HANDLE */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MmngrGetSize(OSAL_RETURN_HANDLE, 0, 512U);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MmngrGetSize returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to get memory size: R_OSAL_MmngrGetSize returns OSAL_RETURN_UNSUPPORTED_OPERATION */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MmngrGetSize(OSAL_RETURN_UNSUPPORTED_OPERATION, 0, 512U);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MmngrGetSize returns %s", testno++, osal_err_to_name(OSAL_RETURN_UNSUPPORTED_OPERATION));

	/* Failed to get memory size: R_OSAL_MmngrGetSize returns OSAL_RETURN_FAIL */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MmngrGetSize(OSAL_RETURN_FAIL, 0, 512U);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MmngrGetSize returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Failed to get CPU address: R_OSAL_MmngrGetCpuPtr returns OSAL_RETURN_PAR */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MmngrGetCpuPtr(OSAL_RETURN_PAR, 0, mem);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MmngrGetCpuPtr returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Failed to get CPU address: R_OSAL_MmngrGetCpuPtr returns OSAL_RETURN_MEM */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MmngrGetCpuPtr(OSAL_RETURN_MEM, 0, mem);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MmngrGetCpuPtr returns %s", testno++, osal_err_to_name(OSAL_RETURN_MEM));

	/* Failed to get CPU address: R_OSAL_MmngrGetCpuPtr returns OSAL_RETURN_HANDLE */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MmngrGetCpuPtr(OSAL_RETURN_HANDLE, 0, mem);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MmngrGetCpuPtr returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to get CPU address: R_OSAL_MmngrGetCpuPtr returns OSAL_RETURN_PROHIBITED */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MmngrGetCpuPtr(OSAL_RETURN_PROHIBITED, 0, mem);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MmngrGetCpuPtr returns %s", testno++, osal_err_to_name(OSAL_RETURN_PROHIBITED));

	/* Failed to get CPU address: R_OSAL_MmngrGetCpuPtr returns OSAL_RETURN_UNSUPPORTED_OPERATION */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MmngrGetCpuPtr(OSAL_RETURN_UNSUPPORTED_OPERATION, 0, mem);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MmngrGetCpuPtr returns %s", testno++, osal_err_to_name(OSAL_RETURN_UNSUPPORTED_OPERATION));

	/* Failed to get CPU address: R_OSAL_MmngrGetCpuPtr returns OSAL_RETURN_FAIL */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MmngrGetCpuPtr(OSAL_RETURN_FAIL, 0, mem);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MmngrGetCpuPtr returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Failed to get HW address: R_OSAL_MmngrGetHwAddr returns OSAL_RETURN_PAR */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MmngrGetHwAddr(OSAL_RETURN_PAR, 0, mem_hw_addr);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MmngrGetHwAddr returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Failed to get HW address: R_OSAL_MmngrGetHwAddr returns OSAL_RETURN_HANDLE */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MmngrGetHwAddr(OSAL_RETURN_HANDLE, 0, mem_hw_addr);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MmngrGetHwAddr returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to get HW address: R_OSAL_MmngrGetHwAddr returns OSAL_RETURN_PROHIBITED */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MmngrGetHwAddr(OSAL_RETURN_PROHIBITED, 0, mem_hw_addr);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MmngrGetHwAddr returns %s", testno++, osal_err_to_name(OSAL_RETURN_PROHIBITED));

	/* Failed to get HW address: R_OSAL_MmngrGetHwAddr returns OSAL_RETURN_UNSUPPORTED_OPERATION */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MmngrGetHwAddr(OSAL_RETURN_UNSUPPORTED_OPERATION, 0, mem_hw_addr);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MmngrGetHwAddr returns %s", testno++, osal_err_to_name(OSAL_RETURN_UNSUPPORTED_OPERATION));

	/* Failed to get HW address: R_OSAL_MmngrGetHwAddr returns OSAL_RETURN_FAIL */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MmngrGetHwAddr(OSAL_RETURN_FAIL, 0, mem_hw_addr);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MmngrGetHwAddr returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Failed to flush data: R_OSAL_MmngrFlush returns OSAL_RETURN_PAR */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MmngrFlush(OSAL_RETURN_PAR);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MmngrFlush returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Failed to flush data: R_OSAL_MmngrFlush returns OSAL_RETURN_HANDLE */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MmngrFlush(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MmngrFlush returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to flush data: R_OSAL_MmngrFlush returns OSAL_RETURN_PROHIBITED */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MmngrFlush(OSAL_RETURN_PROHIBITED);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MmngrFlush returns %s", testno++, osal_err_to_name(OSAL_RETURN_PROHIBITED));

	/* Failed to flush data: R_OSAL_MmngrFlush returns OSAL_RETURN_UNSUPPORTED_OPERATION */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MmngrFlush(OSAL_RETURN_UNSUPPORTED_OPERATION);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MmngrFlush returns %s", testno++, osal_err_to_name(OSAL_RETURN_UNSUPPORTED_OPERATION));

	/* Failed to flush data: R_OSAL_MmngrFlush returns OSAL_RETURN_FAIL */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MmngrFlush(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MmngrFlush returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Failed to create message queue: R_OSAL_MqCreate returns OSAL_RETURN_PAR */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MqCreate(OSAL_RETURN_PAR);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MqCreate returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Failed to create message queue: R_OSAL_MqCreate returns OSAL_RETURN_MEM */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MqCreate(OSAL_RETURN_MEM);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MqCreate returns %s", testno++, osal_err_to_name(OSAL_RETURN_MEM));

	/* Failed to create message queue: R_OSAL_MqCreate returns OSAL_RETURN_BUSY */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MqCreate(OSAL_RETURN_BUSY);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MqCreate returns %s", testno++, osal_err_to_name(OSAL_RETURN_BUSY));

	/* Failed to create message queue: R_OSAL_MqCreate returns OSAL_RETURN_ID */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MqCreate(OSAL_RETURN_ID);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MqCreate returns %s", testno++, osal_err_to_name(OSAL_RETURN_ID));

	/* Failed to create message queue: R_OSAL_MqCreate returns OSAL_RETURN_STATE */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MqCreate(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MqCreate returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Failed to create message queue: R_OSAL_MqCreate returns OSAL_RETURN_FAIL */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MqCreate(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MqCreate returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Failed to memory barrier: R_OSAL_ThsyncMemoryBarrier returns OSAL_RETURN_STATE */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_ThsyncMemoryBarrier(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMemoryBarrier returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Failed to memory barrier: R_OSAL_ThsyncMemoryBarrier returns OSAL_RETURN_FAIL */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_ThsyncMemoryBarrier(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMemoryBarrier returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Failed to receive message: R_OSAL_MqReceiveForTimePeriod returns OSAL_RETURN_TIME */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MqReceiveForTimePeriod(OSAL_RETURN_TIME, p_core_info->core_num);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_CHECKFAIL, *tret, "(No.%d) R_OSAL_MqReceiveForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_TIME));

	/* Failed to receive message: R_OSAL_MqReceiveForTimePeriod returns OSAL_RETURN_PAR */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MqReceiveForTimePeriod(OSAL_RETURN_PAR, p_core_info->core_num);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MqReceiveForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Failed to receive message: R_OSAL_MqReceiveForTimePeriod returns OSAL_RETURN_BUSY */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MqReceiveForTimePeriod(OSAL_RETURN_BUSY, p_core_info->core_num);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MqReceiveForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_BUSY));

	/* Failed to receive message: R_OSAL_MqReceiveForTimePeriod returns OSAL_RETURN_HANDLE */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MqReceiveForTimePeriod(OSAL_RETURN_HANDLE, p_core_info->core_num);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MqReceiveForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to receive message: R_OSAL_MqReceiveForTimePeriod returns OSAL_RETURN_STATE */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MqReceiveForTimePeriod(OSAL_RETURN_STATE, p_core_info->core_num);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MqReceiveForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Failed to receive message: R_OSAL_MqReceiveForTimePeriod returns OSAL_RETURN_FAIL */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MqReceiveForTimePeriod(OSAL_RETURN_FAIL, p_core_info->core_num);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MqReceiveForTimePeriod returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Failed to reset message queue: R_OSAL_MqReset returns OSAL_RETURN_HANDLE */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MqReset(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MqReset returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to reset message queue: R_OSAL_MqReset returns OSAL_RETURN_STATE */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MqReset(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MqReset returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Failed to reset message queue: R_OSAL_MqReset returns OSAL_RETURN_FAIL */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MqReset(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MqReset returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Failed to delete message queue: R_OSAL_MqDelete returns OSAL_RETURN_MEM */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MqDelete(OSAL_RETURN_MEM);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MqDelete returns %s", testno++, osal_err_to_name(OSAL_RETURN_MEM));

	/* Failed to delete message queue: R_OSAL_MqDelete returns OSAL_RETURN_BUSY */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MqDelete(OSAL_RETURN_BUSY);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MqDelete returns %s", testno++, osal_err_to_name(OSAL_RETURN_BUSY));

	/* Failed to delete message queue: R_OSAL_MqDelete returns OSAL_RETURN_HANDLE */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MqDelete(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MqDelete returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to delete message queue: R_OSAL_MqDelete returns OSAL_RETURN_STATE */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MqDelete(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MqDelete returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Failed to delete message queue: R_OSAL_MqDelete returns OSAL_RETURN_FAIL */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MqDelete(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MqDelete returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Failed to clean cache memory: R_OSAL_MmngrInvalidate returns OSAL_RETURN_PAR */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MmngrInvalidate(OSAL_RETURN_PAR);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MmngrInvalidate returns %s", testno++, osal_err_to_name(OSAL_RETURN_PAR));

	/* Failed to clean cache memory: R_OSAL_MmngrInvalidate returns OSAL_RETURN_HANDLE */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MmngrInvalidate(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MmngrInvalidate returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to clean cache memory: R_OSAL_MmngrInvalidate returns OSAL_RETURN_PROHIBITED */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MmngrInvalidate(OSAL_RETURN_PROHIBITED);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MmngrInvalidate returns %s", testno++, osal_err_to_name(OSAL_RETURN_PROHIBITED));

	/* Failed to clean cache memory: R_OSAL_MmngrInvalidate returns OSAL_RETURN_UNSUPPORTED_OPERATION */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MmngrInvalidate(OSAL_RETURN_UNSUPPORTED_OPERATION);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MmngrInvalidate returns %s", testno++, osal_err_to_name(OSAL_RETURN_UNSUPPORTED_OPERATION));

	/* Failed to clean cache memory: R_OSAL_MmngrInvalidate returns OSAL_RETURN_FAIL */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_MmngrInvalidate(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_MmngrInvalidate returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_HANDLE */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_HANDLE);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_STATE */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_STATE);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_STATE));

	/* Failed to unlock mutex: R_OSAL_ThsyncMutexUnlock returns OSAL_RETURN_FAIL */
	prepare_test(handle, p_core_info);
	it_stub_set_R_OSAL_ThsyncMutexUnlock(OSAL_RETURN_FAIL);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) R_OSAL_ThsyncMutexUnlock returns %s", testno++, osal_err_to_name(OSAL_RETURN_FAIL));
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