#define TEST_MSG_NAME "SM_IntHandler"

#include "it_common.h"
#include "it_stub.h"

static void prepare_test_for_sm_int_handler(const st_impdrv_core_info_t	*const p_core_info);
static void SM_IntHandler_exec(struct test_params_t *tparam, struct test_result_t *tret);

static uint32_t testno;

static void prepare_test_for_sm_int_handler(const st_impdrv_core_info_t	*const p_core_info)
{
	if (IMPDRV_CORE_TYPE_IMP == p_core_info->core_type)
	{
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 0, 1u);
	}
#if defined (V3H2) || defined (V3H1)
    else if (IMPDRV_CORE_TYPE_IMP_SLIM == p_core_info->core_type)
	{
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 0, 0x20u);
	}
#endif
	else if (IMPDRV_CORE_TYPE_DMAC == p_core_info->core_type)
	{
#if defined (V3H1) || defined (V3H2)
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 0, 0x800u);
#elif defined (V3M2)
		it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_OK, 0, 0x40u);
#endif
	}
	else
	{
		core_testmsg(p_core_info, "invalid core\n");
	}
}

int32_t it_SM_IntHandler_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

	testno = 1;
	SM_IntHandler_exec(tparam, &tret);

	PrintTestResult(tret, test_result);

	return 0;
}

static void SM_IntHandler_exec(struct test_params_t *tparam, struct test_result_t *tret)
{
	st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
	st_impdrv_core_info_t core_info;
	int32_t result;

	/* clear all stubs */
	it_stub_reset();

	/* Reading Interrupt Status register of IMP0 fails in int_safety_func */
	core_info.core_type	= IMPDRV_CORE_TYPE_IMP;
	core_info.core_num	= 0u;
	prepare_test_for_sm_int_handler(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_PAR, 1, 0);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = true;
	(*g_int_handler)(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = false;
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_safety_func returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_PAR));

	/* Reading Interrupt Status register of IMP0 fails in int_safety_func */
	prepare_test_for_sm_int_handler(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_DEV, 1, 0);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = true;
	(*g_int_handler)(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = false;
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_safety_func returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_DEV));

	/* Reading Interrupt Status register of IMP0 fails in int_safety_func */
	prepare_test_for_sm_int_handler(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_IO, 1, 0);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = true;
	(*g_int_handler)(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = false;
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_safety_func returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_IO));

	/* Reading Interrupt Status register of IMP0 fails in int_safety_func */
	prepare_test_for_sm_int_handler(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_HANDLE, 1, 0);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = true;
	(*g_int_handler)(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = false;
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_safety_func returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Reading Interrupt Status register of IMP0 fails in int_safety_func */
	prepare_test_for_sm_int_handler(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_STATE, 1, 0);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = true;
	(*g_int_handler)(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = false;
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_safety_func returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_STATE));

	/* Reading Interrupt Status register of IMP0 fails in int_safety_func */
	prepare_test_for_sm_int_handler(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_FAIL, 1, 0);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = true;
	(*g_int_handler)(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = false;
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_safety_func returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_FAIL));

#if defined (V3H1) || defined (V3H2)
	/* Reading Interrupt Status register of SlimIMP0 fails in int_safety_func */
	core_info.core_type	= IMPDRV_CORE_TYPE_IMP_SLIM;
	core_info.core_num	= 0u;
	prepare_test_for_sm_int_handler(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_PAR, 1, 0);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = true;
	(*g_int_handler)(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = false;
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_safety_func returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_PAR));
#else
	testno++;
#endif

#if defined (V3H1) || defined (V3H2)
	/* Reading Interrupt Status register of SlimIMP0 fails in int_safety_func */
	prepare_test_for_sm_int_handler(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_DEV, 1, 0);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = true;
	(*g_int_handler)(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = false;
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_safety_func returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_DEV));
#else
	testno++;
#endif

#if defined (V3H1) || defined (V3H2)
	/* Reading Interrupt Status register of SlimIMP0 fails in int_safety_func */
	prepare_test_for_sm_int_handler(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_IO, 1, 0);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = true;
	(*g_int_handler)(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = false;
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_safety_func returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_IO));
#else
	testno++;
#endif

#if defined (V3H1) || defined (V3H2)
	/* Reading Interrupt Status register of SlimIMP0 fails in int_safety_func */
	prepare_test_for_sm_int_handler(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_HANDLE, 1, 0);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = true;
	(*g_int_handler)(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = false;
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_safety_func returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_HANDLE));
#else
	testno++;
#endif

#if defined (V3H1) || defined (V3H2)
	/* Reading Interrupt Status register of SlimIMP0 fails in int_safety_func */
	prepare_test_for_sm_int_handler(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_STATE, 1, 0);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = true;
	(*g_int_handler)(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = false;
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_safety_func returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_STATE));
#else
	testno++;
#endif

#if defined (V3H1) || defined (V3H2)
	/* Reading Interrupt Status register of SlimIMP0 fails in int_safety_func */
	prepare_test_for_sm_int_handler(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_FAIL, 1, 0);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = true;
	(*g_int_handler)(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = false;
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_safety_func returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_FAIL));
#else
	testno++;
#endif

	/* Reading Interrupt Status register of DMAC00 fails in int_safety_func */
	core_info.core_type	= IMPDRV_CORE_TYPE_DMAC;
	core_info.core_num	= 0u;
	prepare_test_for_sm_int_handler(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_PAR, 0, 0);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = true;
	(*g_int_handler)(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = false;
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_safety_func returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_PAR));

	/* Reading Interrupt Status register of DMAC00 fails in int_safety_func */
	prepare_test_for_sm_int_handler(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_DEV, 0, 0);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = true;
	(*g_int_handler)(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = false;
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_safety_func returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_DEV));

	/* Reading Interrupt Status register of DMAC00 fails in int_safety_func */
	prepare_test_for_sm_int_handler(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_IO, 0, 0);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = true;
	(*g_int_handler)(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = false;
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_safety_func returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_IO));

	/* Reading Interrupt Status register of DMAC00 fails in int_safety_func */
	prepare_test_for_sm_int_handler(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_HANDLE, 0, 0);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = true;
	(*g_int_handler)(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = false;
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_safety_func returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_HANDLE));

	/* Reading Interrupt Status register of DMAC00 fails in int_safety_func */
	prepare_test_for_sm_int_handler(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_STATE, 0, 0);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = true;
	(*g_int_handler)(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = false;
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_safety_func returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_STATE));

	/* Reading Interrupt Status register of DMAC00 fails in int_safety_func */
	prepare_test_for_sm_int_handler(&core_info);
	it_stub_set_R_OSAL_IoRead32(OSAL_RETURN_FAIL, 0, 0);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = true;
	(*g_int_handler)(g_osal_device_handle, 0, (void *)p_impdrv_ctl);
	p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num].is_progress_bus_if_check = false;
	result = ((IMPDRV_FC_DRV_ERROR == fatal_code) && (IMPDRV_EC_NG_SYSTEMERROR == fatal_ercd)) ? 0 : (-1);
	it_stub_reset();
	FailTestPrint(result, 0, *tret, "(No.%d) Reading Interrupt Status register of %s%d fails in int_safety_func returns %s", testno++, type_to_name(core_info.core_type), core_info.core_num, osal_err_to_name(OSAL_RETURN_FAIL));
}