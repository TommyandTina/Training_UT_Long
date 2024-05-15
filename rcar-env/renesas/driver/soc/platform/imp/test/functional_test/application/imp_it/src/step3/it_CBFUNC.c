#define TEST_MSG_NAME "CBFUNC"

#include "it_common.h"
#include "it_stub.h"

#define CALLBACK_CODE 0xAA

static void set_state(st_impdrv_initdata_t *const p_data, e_impdrv_state_t state);
static void assign_cbfunc(const st_impdrv_core_info_t *const p_core_info, st_impdrv_coretypectl_t *const p_coretype_ctl, const uint32_t use_core_num);
static void prepare_test_for_cbfunc(const st_impdrv_core_info_t	*const p_core_info);
static void Callback_exec(struct test_params_t *tparam, struct test_result_t *tret);

static int32_t check_callback_code;
static e_impdrv_cb_ret_t check_callback_ercd;
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

static void prepare_test_for_cbfunc(const st_impdrv_core_info_t	*const p_core_info)
{
	e_impdrv_core_type_t	core_type = p_core_info->core_type;
    uint32_t                core_num = p_core_info->core_num;
	uint32_t i;

	check_callback_ercd = 0xFFFFFFFF;
	check_callback_code = CALLBACK_CODE;

	if (IMPDRV_CORE_TYPE_IMP == core_type)
	{
		i = 0;
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 1u);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_IMP_INTSTS_TRAP);
	}
#if defined (V3H2) || defined (V3H1)
    else if (IMPDRV_CORE_TYPE_IMP_SLIM == core_type)
	{
		i = 0;
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0x20u);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_IMPS_INTSTS_TRAP);
	}
#endif
	else if (IMPDRV_CORE_TYPE_OCV == core_type)
	{
		i = 0;
#if defined (V3H1) || defined (V3H2)
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0x40u);
#elif defined (V3M2)
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0x10u);
#elif defined (V4H) || defined (V4H2)
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0x400u);
#endif
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_OCV_SR1_TRAP);
	}
	else if (IMPDRV_CORE_TYPE_DMAC == core_type)
	{
		i = 0;
#if defined (V3H1) || defined (V3H2)
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0x800u);
#elif defined (V3M2)
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0x40u);
#elif defined (V4H) || defined (V4H2)
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0x20u);
#endif
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_DMA_SR_TRAP);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0x81u | IMPDRV_DMA_SER_FIX);
	}
#if defined (V3H2) || defined (V4H) || defined (V4H2)
	else if (IMPDRV_CORE_TYPE_DMAC_SLIM == core_type)
	{
		i = 0;
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0x8000u);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_DMAS_SR_TRAP);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0x81u | IMPDRV_DMAS_SER_FIX);
	}
#endif
	else if (IMPDRV_CORE_TYPE_PSCEXE == core_type)
	{
		i = 0;
#if defined (V3H2)
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0x20000u);
#elif defined (V3H1)
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0x8000u);
#elif defined (V3M2)
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0x100u);
#endif

		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_PSC_SR_TRAP);

#if defined (V3H2)
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0x10001u);
#elif defined (V3H1) || defined (V3M2)
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 1u);
#elif defined (V4H) || defined (V4H2)
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 2, 0x10001u);
#endif
	}
	else if (IMPDRV_CORE_TYPE_CNN == core_type)
	{
		i = 0;
#if defined (V3H2) 
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0x40000u);
#elif defined (V3H1) 
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0x20000u);
#elif defined (V3M2)
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0x200u);
#elif defined (V4H) || defined (V4H2)
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0x2000u);
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 2, 0x18u);
#endif

		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, IMPDRV_CNN_SR_TRAP);

#if defined (V3H2)
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 0x18u);
#elif defined (V3H1) || defined (V3M2)
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, i++, 8u);
#endif
	}
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

int32_t it_CBFUNC_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

	testno = 1;
	Callback_exec(tparam, &tret);

	PrintTestResult(tret, test_result);

	return 0;
}

static void assign_cbfunc(
	const st_impdrv_core_info_t	*const p_core_info,
	st_impdrv_coretypectl_t		*const p_coretype_ctl,
	const uint32_t				use_core_num
)
{
	uint32_t				i;
	e_impdrv_core_type_t    core_type;
	uint32_t                core_num;

	for (i = 0; i < use_core_num; i++)
	{
		core_type	= (p_core_info + i)->core_type;
		core_num	= (p_core_info + i)->core_num;
		(p_coretype_ctl + core_type)->core_ctl[core_num].cbinfo.cbfunc = (p_impdrv_cbfunc_t)&CbFunc;
	}
}

static void Callback_exec(struct test_params_t *tparam, struct test_result_t *tret)
{
	int8_t i;
	e_impdrv_errorcode_t ercd = IMPDRV_EC_OK;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
	st_impdrv_core_info_t *p_core_info = p_initdata->core_info;
	st_impdrv_coretypectl_t *p_coretype_ctl = p_impdrv_ctl->coretype_ctl;
	int32_t result = 0;
	st_impdrv_core_info_t core_info;

	/* Back up data */
	memcpy(coretype_ctl_backup, p_impdrv_ctl->coretype_ctl, sizeof(p_impdrv_ctl->coretype_ctl));

	/* Cores must be in EXEC state */
	set_state(p_initdata, IMPDRV_STATE_EXEC);
	assign_cbfunc(p_core_info, p_coretype_ctl, p_initdata->use_core_num);
	it_stub_reset();

	/* Reading Interrupt Status register returns sts_val = 0 */
	prepare_test_for_cbfunc(p_core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 0, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_UNEXPECT_INT == fatal_code) && (IMPDRV_EC_OK == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register returns sts_val = 0", testno++);

	/* Reading Interrupt Status register of IMP0 fails in int_handler */
	core_info.core_type	= IMPDRV_CORE_TYPE_IMP;
	core_info.core_num	= 0u;
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_PAR, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_PAR));

	/* Reading Interrupt Status register of IMP0 fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_DEV, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_DEV));

	/* Reading Interrupt Status register of IMP0 fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_IO, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_IO));

	/* Reading Interrupt Status register of IMP0 fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_HANDLE, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Reading Interrupt Status register of IMP0 fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_STATE, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_STATE));

	/* Reading Interrupt Status register of IMP0 fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_FAIL, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_FAIL));

#if defined (V3H1) || defined (V3H2)
	/* Reading Interrupt Status register of Slim-IMP fails in int_handler */
	core_info.core_type	= IMPDRV_CORE_TYPE_IMP_SLIM;
	core_info.core_num	= 0u;
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_PAR, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_PAR));
#else
	testno++;
#endif

#if defined (V3H1) || defined (V3H2)
	/* Reading Interrupt Status register of Slim-IMP fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_DEV, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_DEV));
#else
	testno++;
#endif

#if defined (V3H1) || defined (V3H2)
	/* Reading Interrupt Status register of Slim-IMP fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_IO, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_IO));
#else
	testno++;
#endif

#if defined (V3H1) || defined (V3H2)
	/* Reading Interrupt Status register of Slim-IMP fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_HANDLE, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_HANDLE));
#else
	testno++;
#endif

#if defined (V3H1) || defined (V3H2)
	/* Reading Interrupt Status register of Slim-IMP fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_STATE, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_STATE));
#else
	testno++;
#endif

#if defined (V3H1) || defined (V3H2)
	/* Reading Interrupt Status register of Slim-IMP fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_FAIL, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_FAIL));
#else
	testno++;
#endif

	/* Reading Interrupt Status register of OCV0 fails in int_handler */
	core_info.core_type	= IMPDRV_CORE_TYPE_OCV;
	core_info.core_num	= 0u;
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_PAR, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_PAR));

	/* Reading Interrupt Status register of OCV0 fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_DEV, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_DEV));

	/* Reading Interrupt Status register of OCV0 fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_IO, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_IO));

	/* Reading Interrupt Status register of OCV0 fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_HANDLE, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Reading Interrupt Status register of OCV0 fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_STATE, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_STATE));

	/* Reading Interrupt Status register of OCV0 fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_FAIL, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Reading Interrupt Status register of DMAC0 fails in int_handler */
	core_info.core_type	= IMPDRV_CORE_TYPE_DMAC;
	core_info.core_num	= 0u;
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_PAR, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_PAR));

	/* Reading Interrupt Status register of DMAC0 fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_DEV, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_DEV));

	/* Reading Interrupt Status register of DMAC0 fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_IO, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_IO));

	/* Reading Interrupt Status register of DMAC0 fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_HANDLE, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Reading Interrupt Status register of DMAC0 fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_STATE, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_STATE));

	/* Reading Interrupt Status register of DMAC0 fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_FAIL, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_FAIL));

#if defined (V3H2) || defined (V4H) || defined (V4H2)
	/* Reading Interrupt Status register of Slim-DMAC0 fails in int_handler */
	core_info.core_type	= IMPDRV_CORE_TYPE_DMAC_SLIM;
	core_info.core_num	= 0u;
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_PAR, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_PAR));
#else
	testno++;
#endif

#if defined (V3H2) || defined (V4H) || defined (V4H2)
	/* Reading Interrupt Status register of Slim-DMAC0 fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_DEV, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_DEV));
#else
	testno++;
#endif

#if defined (V3H2) || defined (V4H) || defined (V4H2)
	/* Reading Interrupt Status register of Slim-DMAC0 fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_IO, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_IO));
#else
	testno++;
#endif

#if defined (V3H2) || defined (V4H) || defined (V4H2)
	/* Reading Interrupt Status register of Slim-DMAC0 fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_HANDLE, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_HANDLE));
#else
	testno++;
#endif

#if defined (V3H2) || defined (V4H) || defined (V4H2)
	/* Reading Interrupt Status register of Slim-DMAC0 fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_STATE, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_STATE));
#else
	testno++;
#endif

#if defined (V3H2) || defined (V4H) || defined (V4H2)
	/* Reading Interrupt Status register of Slim-DMAC0 fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_FAIL, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_FAIL));
#else
	testno++;
#endif

	/* Reading Interrupt Status register of PSCEXE0 fails in int_handler */
	core_info.core_type	= IMPDRV_CORE_TYPE_PSCEXE;
	core_info.core_num	= 0u;
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_PAR, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_PAR));

	/* Reading Interrupt Status register of PSCEXE0 fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_DEV, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_DEV));

	/* Reading Interrupt Status register of PSCEXE0 fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_IO, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_IO));

	/* Reading Interrupt Status register of PSCEXE0 fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_HANDLE, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Reading Interrupt Status register of PSCEXE0 fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_STATE, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_STATE));

	/* Reading Interrupt Status register of PSCEXE0 fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_FAIL, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_FAIL));

	/* Reading Interrupt Status register of CNN fails in int_handler */
	core_info.core_type	= IMPDRV_CORE_TYPE_CNN;
	core_info.core_num	= 0u;
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_PAR, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_PAR));

	/* Reading Interrupt Status register of CNN fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_DEV, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_DEV));

	/* Reading Interrupt Status register of CNN fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_IO, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_IO));

	/* Reading Interrupt Status register of CNN fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_HANDLE, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Reading Interrupt Status register of CNN fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_STATE, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_STATE));

	/* Reading Interrupt Status register of CNN fails in int_handler */
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_FAIL, 1, 0);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_handler returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_FAIL));

#if defined (V3H1) || defined (V3H2) || defined (V4H) || defined (V4H2)
	/* Reading Interrupt Status register of DMAC0 returns IMPDRV_DMA_SR_WUP in int_handler */
	core_info.core_type	= IMPDRV_CORE_TYPE_DMAC;
	core_info.core_num	= 0u;
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 1, IMPDRV_DMA_SR_WUP);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_CB_RET_WUPCOVF == check_callback_ercd) && (IMPDRV_CALLBACK_CODE_DEF == check_callback_code)) ? (0) : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d returns value 0x%x in int_handler", testno++, type_to_name(core_info.core_type), core_info.core_num, IMPDRV_DMA_SR_WUP);
#else
	testno++;
#endif

#if defined (V3H2) || defined (V4H) || defined (V4H2)
	/* Reading Interrupt Status register of Slim-DMAC0 returns IMPDRV_DMAS_SR_WUP in int_handler */
	core_info.core_type	= IMPDRV_CORE_TYPE_DMAC_SLIM;
	core_info.core_num	= 0u;
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 1, IMPDRV_DMAS_SR_WUP);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_CB_RET_WUPCOVF == check_callback_ercd) && (IMPDRV_CALLBACK_CODE_DEF == check_callback_code)) ? (0) : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d returns value 0x%x in int_handler", testno++, type_to_name(core_info.core_type), core_info.core_num, IMPDRV_DMAS_SR_WUP);
#else
	testno++;
#endif

	/* Reading Interrupt Status register of PSCEXE0 returns IMPDRV_PSC_SR_TEND in int_handler */
	core_info.core_type	= IMPDRV_CORE_TYPE_PSCEXE;
	core_info.core_num	= 0u;
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 1, IMPDRV_PSC_SR_TEND);
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_CB_RET_END == check_callback_ercd) && (IMPDRV_CALLBACK_CODE_DEF == check_callback_code)) ? (0) : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d returns value 0x%x in int_handler", testno++, type_to_name(core_info.core_type), core_info.core_num, IMPDRV_PSC_SR_TEND);

	/* Reading Interrupt Status register of CNN returns IMPDRV_CNN_SR_MSCO in int_handler */
#if !defined (V4H2)
	core_info.core_type	= IMPDRV_CORE_TYPE_CNN;
	core_info.core_num	= 0u;
	prepare_test_for_cbfunc(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 1, IMPDRV_CNN_SR_MSCO);
#if defined (V3H1)
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 2, 0x4008u);
#endif
	(*g_int_handler[0])(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	result = ((IMPDRV_CB_RET_MSCO == check_callback_ercd) && (IMPDRV_CALLBACK_CODE_DEF == check_callback_code)) ? (0) : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d returns value 0x%x in int_handler", testno++, type_to_name(core_info.core_type), core_info.core_num, IMPDRV_CNN_SR_MSCO);
#endif

	/* Get backup data */
	memcpy(p_impdrv_ctl->coretype_ctl, coretype_ctl_backup, sizeof(p_impdrv_ctl->coretype_ctl));
}
