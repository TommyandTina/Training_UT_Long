#define TEST_MSG_NAME "DSP_CBFUNC"

#include "it_common.h"
#include "it_stub.h"

#define USED_CORES							\
	{ IMPDRV_CORE_TYPE_DSP,         0 },	\
	{ IMPDRV_CORE_TYPE_DSP,         1 },	\
	{ IMPDRV_CORE_TYPE_DSP,         2 },	\
	{ IMPDRV_CORE_TYPE_DSP,         3 }

#define CALLBACK_CODE 0xAA

extern int32_t cbfunc_fatal(
    const st_impdrv_core_info_t *const p_core_info,     /**< Core information                   */
    const e_impdrv_cb_ret_t     ercd,                   /**< Error code                         */
    const int32_t               code,                   /**< TRAP or INT code (8 bit is set)    */
    void                        *const p_callback_args  /**< User argument                      */
);

static void prepare_data();
static void prepare_before_init(uint32_t use_core_num);
static void prepare_test_for_dsp_cbfunc(uint32_t core_num);
static void Init();
static void AttrInit();
static void AttrSetDsp();
static void Start();
static void Execute();
#ifdef IT_ABNORMAL
static void Dsp_callback_return_error(struct test_result_t *tret);
#endif
static void Dsp_callback_exec(struct test_result_t *tret);
static void Stop();
static void Quit();

static int32_t check_callback_code;
static e_impdrv_cb_ret_t check_callback_ercd;
static uint32_t testno;

static st_impdrv_initdata_t initdata;
static st_impdrv_ctl_t drvctl;
static st_impdrv_ctl_t backup_drvctl;

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

static void prepare_before_init(uint32_t use_core_num)
{
	static const uint32_t read_value_for_spminit[] = {
		0x0003ffffU,	/* Set IMPSLV.DSP0CT for corresponding to interrupt mode */
		0x0003ffffU,	/* Set IMPSLV.DSP1CT for corresponding to interrupt mode */
		0x0003ffffU,	/* Set IMPSLV.DSP2CT for corresponding to interrupt mode */
		0x0003ffffU,	/* Set IMPSLV.DSP3CT for corresponding to interrupt mode */
		0x00000000U,	/* Read the INTEN00 register in 'impdrv_cmnctl_init()' */
		0x00000000U,	/* Write the INTEN00 register in 'impdrv_cmnctl_init()' */
		0x0000000aU,	/* Write the SPM preset register in 'impdrv_cmnctl_reg_spm_preset()' */
		0x0000000aU,	/* Write the SPM preset register in 'impdrv_cmnctl_reg_spm_preset()' */
		0x00000009U,	/* Write the SPM preset register in 'impdrv_cmnctl_reg_spm_preset()' */
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
	for (uint32_t i = 0; i < use_core_num; i++)
	{
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, k++, IMPDRV_VDSP_VERSION1_VAL);
	}

	it_stub_set_R_OSAL_PmGetRequiredState(IMPDRV_EC_OK, OSAL_PM_REQUIRED_STATE_RELEASED);
}

static void prepare_test_for_dsp_cbfunc(uint32_t core_num)
{
	uint32_t i;

	check_callback_ercd = 0xFFFFFFFF;
	check_callback_code = CALLBACK_CODE;

	if (core_num != 3U)
	{
		i = 0;
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_REGVAL_OUT_INT_TIMER00 | IMPDRV_VDSP_CHANNEL_OPE_BITS);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_REGVAL_VDSP_TRAP);
	}
	else
	{
		i = 0;
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_REGVAL_OUT_INT_TIMER00);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_REGVAL_VDSP_TRAP);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 1u);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_REGVAL_OUT_INT_TIMER01);
	}
}

static void pre_process()
{
	impdrv_ctrl_handle_t handle;

	Init();
	AttrInit();
	AttrSetDsp();
	Start();
	Execute();
}

static void post_process()
{
	impdrv_ctrl_handle_t handle;

	Stop();
	Quit(handle);
}

static void prepare_data()
{
	uint32_t i, j;
	st_impdrv_core_info_t core_info[] = { USED_CORES };

	/* prepare used core */
	memset(&initdata, 0, sizeof(st_impdrv_initdata_t));
	initdata.use_core_num = ARRAY_SIZE(core_info);
	memcpy(initdata.core_info, core_info, sizeof(core_info));

	/* prepare OSAL resource */
	initdata.osal_resource.mutex_id = OSAL_MUTEX_IMP_DRV_LOCK;

	/* prepare IMP Driver Control */
	memset(&drvctl, 0, sizeof(drvctl));
	for (i = 0; i < IMPDRV_CORE_TYPE_MAX; i++)
	{
		for (j = 0; j < IMPDRV_CORE_NUM_MAX; j++)
		{
			drvctl.coretype_ctl[i].core_ctl[j].core_state = IMPDRV_STATE_UNINIT;
		}
	}

	initdata.p_work_addr = (void *)&drvctl;
	initdata.work_size = sizeof(drvctl);

	/* prepare basic data */
	initdata.instance_num = IMPDRV_INSTANCE_0;
	initdata.callback_func_fatal = (p_impdrv_cbfunc_fatal_t)&cbfunc_fatal;

	prepare_before_init(initdata.use_core_num);
}

static void Init()
{
	e_impdrv_errorcode_t ercd;
	impdrv_ctrl_handle_t handle;

	it_stub_reset();
	prepare_data();
	
	init_test_flag = true;

	/* Initialize DSP */
	ercd = R_IMPDRV_Init(&initdata, &handle);
	if (IMPDRV_EC_OK != ercd)
	{
		testmsg("Failed (ercd %s)\n", impdrv_err_to_name(ercd));
	}

	init_test_flag = false;

	return;
}

static void AttrInit()
{
	e_impdrv_errorcode_t ercd;
	impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)&drvctl;

	it_stub_reset();

	/* Initialize attributes */
	for (uint32_t i = 0; i < initdata.use_core_num; i++)
	{
		st_impdrv_core_info_t *p_core_info = &initdata.core_info[i];
		e_impdrv_core_type_t core_type = p_core_info->core_type;
		uint32_t core_num = p_core_info->core_num;

		ercd = R_IMPDRV_AttrInit(handle, p_core_info);
		if (IMPDRV_EC_OK != ercd)
		{
			testmsg("Fail at %s%d (ercd %s)\n", type_to_name(core_type), core_num, impdrv_err_to_name(ercd));
		}
	}

	return;
}

static void AttrSetDsp()
{
	e_impdrv_errorcode_t ercd;
	impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)&drvctl;
	st_impdrv_dsp_data_t dsp_data;
	st_impdrv_dsp_data_t dsp_fw;
	st_impdrv_dsp_data_t dsp_app;
	st_impdrv_dsp_data_t dsp_dtcm;

	it_stub_reset();

	/* prepare DSP info */
	memset(&dsp_data, 0, sizeof(dsp_data));
	memset(&dsp_fw, 0, sizeof(dsp_fw));
	memset(&dsp_app, 0, sizeof(dsp_app));
	memset(&dsp_dtcm, 0, sizeof(dsp_dtcm));

	dsp_app.addr_phys = 0x40000000U;
	dsp_app.size = 0x2000U;
	
	dsp_data.addr_phys = 0x50000000U;
	dsp_data.size = 0x2000U;
	
	dsp_fw.addr_phys = 0x60000000U;
	dsp_fw.size = 0x2000U;
	
	dsp_dtcm.addr_phys = 0x80000000U;
	dsp_dtcm.size = 0x2000U;

	/* Set DSP */
	for (uint32_t i = 0; i < initdata.use_core_num; i++)
	{
		st_impdrv_core_info_t *p_core_info = &initdata.core_info[i];
		e_impdrv_core_type_t core_type = p_core_info->core_type;
		uint32_t core_num = p_core_info->core_num;

		ercd = R_IMPDRV_AttrSetDsp(handle, p_core_info, &dsp_app, &dsp_fw, &dsp_data, &dsp_dtcm);
		if (IMPDRV_EC_OK != ercd)
		{
			testmsg("Fail at %s%d (ercd %s)\n", type_to_name(core_type), core_num, impdrv_err_to_name(ercd));
		}
	}

	return;
}

static void Start()
{
	e_impdrv_errorcode_t ercd;
	impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)&drvctl;

	/* Start DSP cores */
	for (uint32_t i = 0; i < initdata.use_core_num; i++)
	{
		st_impdrv_core_info_t *p_core_info = &initdata.core_info[i];
		e_impdrv_core_type_t core_type = p_core_info->core_type;
		uint32_t core_num = p_core_info->core_num;
		st_impdrv_coretypectl_t *coretype_ctl = &drvctl.coretype_ctl[core_type];
		uint32_t cnt = 0U;

		it_stub_reset();

		dsp_register_cbfunc_flag[i] = true;
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, cnt++, 0);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, cnt++, 0);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, cnt++, coretype_ctl->core_ctl[core_num].dsp_info.dsp_fw.addr_phys);

		ercd = R_IMPDRV_Start(handle, p_core_info);
		if (IMPDRV_EC_OK != ercd)
		{
			testmsg("Fail at %s%d (ercd %s)\n", type_to_name(core_type), core_num, impdrv_err_to_name(ercd));
		}

		dsp_register_cbfunc_flag[i] = false;
	}

	return;
}

static void Execute()
{
	e_impdrv_errorcode_t ercd;
	impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)&drvctl;
	p_impdrv_cbfunc_t callback_func = (p_impdrv_cbfunc_t)&CbFunc;
	uint32_t callback_args;

	/* Execute DSP cores */
	for (uint32_t i = 0; i < initdata.use_core_num; i++)
	{
		st_impdrv_core_info_t *p_core_info = &initdata.core_info[i];
		e_impdrv_core_type_t core_type = p_core_info->core_type;
		uint32_t core_num = p_core_info->core_num;
		uint32_t cnt = 0U;

		it_stub_reset();

		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, cnt++, IMPDRV_REGVAL_OUT_INT_TIMER00);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, cnt++, IMPDRV_REGVAL_CLR);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, cnt++, IMPDRV_REGVAL_VDSP_GPIO_START);

		ercd = R_IMPDRV_Execute(handle, p_core_info, callback_func, (void *)&callback_args);
		if (IMPDRV_EC_OK != ercd)
		{
			testmsg("Fail at %s%d (ercd %s)\n", type_to_name(core_type), core_num, impdrv_err_to_name(ercd));
		}
		else if (drvctl.coretype_ctl[core_type].core_ctl[core_num].core_state != IMPDRV_STATE_EXEC)
		{
			core_testmsg(p_core_info, "core_type = %d, core_num = %u, state = %s, it is not EXEC\n",
							core_type,
							core_num,
							state_to_name(drvctl.coretype_ctl[core_type].core_ctl[core_num].core_state));
		}
	}

	return;
}

static void Stop()
{
	e_impdrv_errorcode_t ercd;
	impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)&drvctl;
	int32_t result = 0;

	/* Stop DSP cores */
	for (uint32_t i = 0; i < initdata.use_core_num; i++)
	{
		st_impdrv_core_info_t *p_core_info = &initdata.core_info[i];
		e_impdrv_core_type_t core_type = p_core_info->core_type;
		uint32_t core_num = p_core_info->core_num;
		uint32_t cnt = 0U;

		it_stub_reset();

		dsp_unregister_cbfunc_flag[i] = true;
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, cnt++, IMPDRV_REGVAL_VDSP_GPIO_STOP);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, cnt++, IMPDRV_REGVAL_VDSP_GPIO_STDBY);
		/* impdrv_dspctl_dsp_standby */
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, cnt++, IMPDRV_REGVAL_PSU_STATE_LSP);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, cnt++, IMPDRV_REGVAL_DMBE_IDLE);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, cnt++, IMPDRV_REGVAL_PSU_STATE_LSP | IMPDRV_REGVAL_PSU_STATE_SBY);

		ercd = R_IMPDRV_Stop(handle, p_core_info);
		if (IMPDRV_EC_OK != ercd)
		{
			testmsg("Fail at %s%d (ercd %s)\n", type_to_name(core_type), core_num, impdrv_err_to_name(ercd));
			result |= -1;
		}

		dsp_unregister_cbfunc_flag[i] = false;
	}

	return;
}

static void Quit()
{
	e_impdrv_errorcode_t ercd;
	impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)&drvctl;

	it_stub_reset();
	
	quit_test_flag = true;

	/* Deinitialize DSP */
	ercd = R_IMPDRV_Quit(handle);
	if (IMPDRV_EC_OK != ercd)
	{
		testmsg("Failed (ercd %s)\n", impdrv_err_to_name(ercd));
	}

	quit_test_flag = false;

	return;
}

int32_t it_DSP_CBFUNC_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

	/* unused */
	(void)tparam;

	pre_process();

	testno = 1;
#ifdef IT_ABNORMAL
	Dsp_callback_return_error(&tret);
#endif
	Dsp_callback_exec(&tret);

	post_process();

	PrintTestResult(tret, test_result);

	return 0;
}

#ifdef IT_ABNORMAL
static void Dsp_callback_return_error(struct test_result_t *tret)
{
	st_impdrv_core_info_t core_info;
	int32_t result = 0;

	/* Back up data */
	memcpy((void *)&backup_drvctl, (void *)&drvctl, sizeof(st_impdrv_ctl_t));

	it_stub_reset();
	core_info.core_type = IMPDRV_CORE_TYPE_DSP;
	core_info.core_num = 0;
	prepare_test_for_dsp_cbfunc(core_info.core_num);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_FAIL, 0, IMPDRV_REGVAL_OUT_INT_TIMER00 | IMPDRV_VDSP_CHANNEL_OPE_BITS);
	(*g_int_handler[0])(&g_osal_device_handle[0], 0, (void *)&drvctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Recovery data */
	memcpy((void *)&drvctl, (void *)&backup_drvctl, sizeof(st_impdrv_ctl_t));
	it_stub_reset();
	core_info.core_type = IMPDRV_CORE_TYPE_DSP;
	core_info.core_num = 0;
	prepare_test_for_dsp_cbfunc(core_info.core_num);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 0, IMPDRV_REGVAL_OUT_INT_TIMER00 | IMPDRV_VDSP_CHANNEL_OPE_BITS);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 1, IMPDRV_REGVAL_VDSP_CRC_ERROR);
	(*g_int_handler[0])(&g_osal_device_handle[0], 0, (void *)&drvctl);
	result = ((IMPDRV_CB_DSP_CRC_ERROR == check_callback_ercd) && (IMPDRV_CALLBACK_CODE_DEF == check_callback_code)) ? 0 : (-1);
	FailTestPrint(result, 0, *tret, "(No.%d) Unexpected cb_ret(%d) is returned in int_handler of %s%d", testno++, check_callback_ercd, type_to_name(core_info.core_type), core_info.core_num);

	/* Recovery data */
	memcpy((void *)&drvctl, (void *)&backup_drvctl, sizeof(st_impdrv_ctl_t));
	it_stub_reset();
	core_info.core_type = IMPDRV_CORE_TYPE_DSP;
	core_info.core_num = 0;
	prepare_test_for_dsp_cbfunc(core_info.core_num);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 0, IMPDRV_REGVAL_OUT_INT_TIMER00 | IMPDRV_VDSP_CHANNEL_OPE_BITS);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 1, IMPDRV_REGVAL_VDSP_NORMAL_INT);
	(*g_int_handler[0])(&g_osal_device_handle[0], 0, (void *)&drvctl);
	result = ((IMPDRV_CB_DSP_NORMAL_INT == check_callback_ercd) && (IMPDRV_CALLBACK_CODE_DEF == check_callback_code)) ? 0 : (-1);
	FailTestPrint(result, 0, *tret, "(No.%d) Unexpected cb_ret(%d) is returned in int_handler of %s%d", testno++, check_callback_ercd, type_to_name(core_info.core_type), core_info.core_num);

	/* Recovery data */
	memcpy((void *)&drvctl, (void *)&backup_drvctl, sizeof(st_impdrv_ctl_t));
	it_stub_reset();
	core_info.core_type = IMPDRV_CORE_TYPE_DSP;
	core_info.core_num = 0;
	prepare_test_for_dsp_cbfunc(core_info.core_num);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 0, IMPDRV_REGVAL_OUT_INT_TIMER00 | IMPDRV_VDSP_CHANNEL_OPE_BITS);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 1, IMPDRV_REGVAL_VDSP_CRITICAL_INT);
	(*g_int_handler[0])(&g_osal_device_handle[0], 0, (void *)&drvctl);
	result = ((IMPDRV_CB_DSP_CRITICAL_INT == check_callback_ercd) && (IMPDRV_CALLBACK_CODE_DEF == check_callback_code)) ? 0 : (-1);
	FailTestPrint(result, 0, *tret, "(No.%d) Unexpected cb_ret(%d) is returned in int_handler of %s%d", testno++, check_callback_ercd, type_to_name(core_info.core_type), core_info.core_num);

	/* Recovery data */
	memcpy((void *)&drvctl, (void *)&backup_drvctl, sizeof(st_impdrv_ctl_t));
	it_stub_reset();
	core_info.core_type = IMPDRV_CORE_TYPE_DSP;
	core_info.core_num = 0;
	prepare_test_for_dsp_cbfunc(core_info.core_num);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 0, IMPDRV_REGVAL_OUT_INT_TIMER00 | IMPDRV_VDSP_CHANNEL_OPE_BITS);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 1, IMPDRV_REGVAL_VDSP_SAFETY_ERROR);
	(*g_int_handler[0])(&g_osal_device_handle[0], 0, (void *)&drvctl);
	result = ((IMPDRV_CB_DSP_SAFETY_ERROR == check_callback_ercd) && (IMPDRV_CALLBACK_CODE_DEF == check_callback_code)) ? 0 : (-1);
	FailTestPrint(result, 0, *tret, "(No.%d) Unexpected cb_ret(%d) is returned in int_handler of %s%d", testno++, check_callback_ercd, type_to_name(core_info.core_type), core_info.core_num);

	/* Recovery data */
	memcpy((void *)&drvctl, (void *)&backup_drvctl, sizeof(st_impdrv_ctl_t));
	it_stub_reset();
	core_info.core_type = IMPDRV_CORE_TYPE_DSP;
	core_info.core_num = 0;
	prepare_test_for_dsp_cbfunc(core_info.core_num);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 0, IMPDRV_REGVAL_OUT_INT_TIMER00 | IMPDRV_VDSP_CHANNEL_OPE_BITS);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 1, IMPDRV_REGVAL_VDSP_DIV0);
	(*g_int_handler[0])(&g_osal_device_handle[0], 0, (void *)&drvctl);
	result = ((IMPDRV_CB_DSP_DIV0 == check_callback_ercd) && (IMPDRV_CALLBACK_CODE_DEF == check_callback_code)) ? 0 : (-1);
	FailTestPrint(result, 0, *tret, "(No.%d) Unexpected cb_ret(%d) is returned in int_handler of %s%d", testno++, check_callback_ercd, type_to_name(core_info.core_type), core_info.core_num);

	/* Recovery data */
	memcpy((void *)&drvctl, (void *)&backup_drvctl, sizeof(st_impdrv_ctl_t));
	it_stub_reset();
	core_info.core_type = IMPDRV_CORE_TYPE_DSP;
	core_info.core_num = 0;
	prepare_test_for_dsp_cbfunc(core_info.core_num);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 0, IMPDRV_REGVAL_OUT_INT_TIMER00 | IMPDRV_VDSP_CHANNEL_OPE_BITS);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 1, IMPDRV_REGVAL_VDSP_WDOG_EXPIRE);
	(*g_int_handler[0])(&g_osal_device_handle[0], 0, (void *)&drvctl);
	result = ((IMPDRV_CB_DSP_WDOG_EXPIRE == check_callback_ercd) && (IMPDRV_CALLBACK_CODE_DEF == check_callback_code)) ? 0 : (-1);
	FailTestPrint(result, 0, *tret, "(No.%d) Unexpected cb_ret(%d) is returned in int_handler of %s%d", testno++, check_callback_ercd, type_to_name(core_info.core_type), core_info.core_num);

	/* Recovery data */
	memcpy((void *)&drvctl, (void *)&backup_drvctl, sizeof(st_impdrv_ctl_t));
	it_stub_reset();
	core_info.core_type = IMPDRV_CORE_TYPE_DSP;
	core_info.core_num = 0;
	prepare_test_for_dsp_cbfunc(core_info.core_num);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 0, IMPDRV_REGVAL_OUT_INT_TIMER00 | IMPDRV_VDSP_CHANNEL_OPE_BITS);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 1, IMPDRV_REGVAL_VDSP_STACK_VIOLATION);
	(*g_int_handler[0])(&g_osal_device_handle[0], 0, (void *)&drvctl);
	result = ((IMPDRV_CB_DSP_STACK_VIOLATION == check_callback_ercd) && (IMPDRV_CALLBACK_CODE_DEF == check_callback_code)) ? 0 : (-1);
	FailTestPrint(result, 0, *tret, "(No.%d) Unexpected cb_ret(%d) is returned in int_handler of %s%d", testno++, check_callback_ercd, type_to_name(core_info.core_type), core_info.core_num);

	/* Recovery data */
	memcpy((void *)&drvctl, (void *)&backup_drvctl, sizeof(st_impdrv_ctl_t));
	it_stub_reset();
	core_info.core_type = IMPDRV_CORE_TYPE_DSP;
	core_info.core_num = 0;
	prepare_test_for_dsp_cbfunc(core_info.core_num);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 0, IMPDRV_REGVAL_OUT_INT_TIMER00 | IMPDRV_VDSP_CHANNEL_OPE_BITS);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 1, IMPDRV_REGVAL_VDSP_TIM_FAULT);
	(*g_int_handler[0])(&g_osal_device_handle[0], 0, (void *)&drvctl);
	result = ((IMPDRV_CB_DSP_TIM_FAULT == check_callback_ercd) && (IMPDRV_CALLBACK_CODE_DEF == check_callback_code)) ? 0 : (-1);
	FailTestPrint(result, 0, *tret, "(No.%d) Unexpected cb_ret(%d) is returned in int_handler of %s%d", testno++, check_callback_ercd, type_to_name(core_info.core_type), core_info.core_num);

	/* Recovery data */
	memcpy((void *)&drvctl, (void *)&backup_drvctl, sizeof(st_impdrv_ctl_t));
}
#endif

static void Dsp_callback_exec(struct test_result_t *tret)
{
	int32_t result = 0;

	/* Callback from DSP cores */
	for (uint32_t i = 0; i < initdata.use_core_num; i++)
	{
		st_impdrv_core_info_t *p_core_info = &initdata.core_info[i];
		e_impdrv_core_type_t core_type = p_core_info->core_type;
		uint32_t core_num = p_core_info->core_num;

		it_stub_reset();
		prepare_test_for_dsp_cbfunc(i);
		(*g_int_handler[i])(&g_osal_device_handle[i], 0, (void *)&drvctl);
		if (IMPDRV_VDSP_CB_CODE_TRAP != check_callback_code)
		{
			core_testmsg(p_core_info, "callback_code = %d (test NG)\n", check_callback_code);
			result = -1;
		}
		else if (IMPDRV_CB_RET_OK != check_callback_ercd)
		{
			core_testmsg(p_core_info, "callback_ercd = %d (test NG)\n", check_callback_ercd);
			result = -1;
		}
		else
		{
			result = 0;
		}

		if ((result == 0) && (i == 3U))
		{
			/* DSP Register Check */
			/* D_DDCL */
			if (g_write_value[WRITE_REG_IDX_D_DDCL] != 0x00000000U)
			{
				core_testmsg(p_core_info, "%s[%08x] is not [%08x]\n", "D_DDCL", g_write_value[WRITE_REG_IDX_D_DDCL], 0x00000000U);
				result = -1;
			}
			/* VDSP_OUT_INT_CLR */
			if (g_write_value[WRITE_REG_IDX_VDSP_OUT_INT_CLR] != 0x00000001U)
			{
				core_testmsg(p_core_info, "%s[%08x] is not [%08x]\n", "VDSP_OUT_INT_CLR", g_write_value[WRITE_REG_IDX_VDSP_OUT_INT_CLR], 0x00000001U);
				result = -1;
			}
			/* For debug */
			if (result == 0)
			{
				printf("Register check is ALL OK !!!\n");
			}
		}
		SuccessTestPrint(result == 0, *tret, "(No.%d) %s%d ", testno++, type_to_name(core_type), core_num);
	}
}