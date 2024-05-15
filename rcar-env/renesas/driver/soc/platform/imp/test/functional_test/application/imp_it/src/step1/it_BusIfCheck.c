
#define TEST_MSG_NAME "R_IMPDRV_BusIfCheck"

#include "it_common.h"
#include "it_stub.h"

#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret);
static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret);
static void Return_error(struct test_params_t *tparam, struct test_result_t *tret);
#endif
static void Device_bus_if_check(struct test_params_t *tparam, struct test_result_t *tret);

static uint32_t testno;
static st_impdrv_chk_resource_t chk_resource_bk;

int32_t it_BusIfCheck_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

	memcpy(&chk_resource_bk, &tparam->chk_resource, sizeof(st_impdrv_chk_resource_t));

#ifdef IT_ABNORMAL
	testno = 1;
	Parameter_error(tparam, &tret);
	Invalid_core(tparam, &tret);
	No_exec_core(tparam, &tret);
	Return_error(tparam, &tret);
#endif
#if !defined (V4H) && !defined (V4H2)
	testno = 49;
	Device_bus_if_check(tparam, &tret);
#endif

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
	ercd = R_IMPDRV_BusIfCheck(NULL, p_core_info, p_chk_resource);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) handle == NULL", testno++);

	/* Parameter_error : p_core_info == NULL */
	ercd = R_IMPDRV_BusIfCheck(handle, NULL, p_chk_resource);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) p_core_info == NULL", testno++);

	/* Parameter_error : p_chk_resource == NULL */
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, NULL);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) p_chk_resource == NULL", testno++);

	/* Parameter_error : p_chk_resource->memory_handle == NULL */
	p_chk_resource->memory_handle = NULL;
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) memory_handle == NULL", testno++);

	/* Parameter_error : p_chk_resource->axi_id == OSAL_AXI_BUS_ID_INVALID */
	memcpy(p_chk_resource, &chk_resource_bk, sizeof(st_impdrv_chk_resource_t));
	p_chk_resource->axi_id = OSAL_AXI_BUS_ID_INVALID;
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) axi_id == OSAL_AXI_BUS_ID_INVALID", testno++);

	/* Parameter_error : p_chk_resource->mq_id == 0U */
	memcpy(p_chk_resource, &chk_resource_bk, sizeof(st_impdrv_chk_resource_t));
	p_chk_resource->mq_id = 0U;
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) mq_id == 0U", testno++);

	/* Parameter_error : p_chk_resource->mq_time_period == -1 */
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

	/* Invalid_error : Core_type = IMPDRV_CORE_TYPE_INVALID */
	core_info.core_type = IMPDRV_CORE_TYPE_INVALID;
	core_info.core_num = 0U;
	ercd = R_IMPDRV_BusIfCheck(handle, &core_info, p_chk_resource);
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) Core_type = IMPDRV_CORE_TYPE_INVALID", testno++);

	/* Invalid_error : Core_num = IMPDRV_CORE_NUM_MAX */
	core_info.core_type = IMPDRV_CORE_TYPE_IMP;
	core_info.core_num = IMPDRV_CORE_NUM_MAX;
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

		ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
		FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) Core not supported: %s", testno++, type_to_name(p_core_info->core_type));
	}
}

static void Return_error(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_core_info_t *p_core_info;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)tparam->initdata.p_work_addr;
	st_impdrv_chk_resource_t *p_chk_resource = &tparam->chk_resource;
	tp_core_list_t exec_cores = get_core_list(IT_IMPDRV_CMD_BUSIF, 0);

	p_core_info = exec_cores.core_info;

	/* Fail to lock mutex impdrv_cmnctl_mutex_lock returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_cmnctl_mutex_lock(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_cmnctl_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to lock mutex impdrv_cmnctl_mutex_lock returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_cmnctl_mutex_lock(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to lock mutex impdrv_cmnctl_mutex_lock returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_cmnctl_mutex_lock(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

#if !defined (V4H) && !defined (V4H2)
	/* Fail to save IRQ mask impdrv_cmnctl_save_irq_mask returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_cmnctl_save_irq_mask(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_cmnctl_save_irq_mask returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to save IRQ mask impdrv_cmnctl_save_irq_mask returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_cmnctl_save_irq_mask(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_save_irq_mask returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to save IRQ mask impdrv_cmnctl_save_irq_mask returns IMPDRV_EC_NG_PARAM */
	it_stub_set_impdrv_cmnctl_save_irq_mask(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_save_irq_mask returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to save IRQ mask impdrv_cmnctl_save_irq_mask returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_cmnctl_save_irq_mask(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_save_irq_mask returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to force IRQ mask impdrv_cmnctl_force_irq_mask returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_cmnctl_force_irq_mask(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_cmnctl_force_irq_mask returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to force IRQ mask impdrv_cmnctl_force_irq_mask returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_cmnctl_force_irq_mask(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_force_irq_mask returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to force IRQ mask impdrv_cmnctl_force_irq_mask returns IMPDRV_EC_NG_PARAM */
	it_stub_set_impdrv_cmnctl_force_irq_mask(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_force_irq_mask returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to force IRQ mask impdrv_cmnctl_force_irq_mask returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_cmnctl_force_irq_mask(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_force_irq_mask returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));
#else
    testno += 8;
#endif

	/* Fail to get core function impdrv_cmnctl_get_corectl_func returns NULL */
	it_stub_set_impdrv_cmnctl_get_corectl_func(NULL);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) impdrv_cmnctl_get_corectl_func returns NULL", testno++);

#if !defined (V4H) && !defined (V4H2)
	/* Fail to execute Bus Interface Check for IMP0 impdrv_impctl_bus_if_check returns IMPDRV_EC_NG_CHECKFAIL */
	it_stub_set_impdrv_impctl_bus_if_check(IMPDRV_EC_NG_CHECKFAIL);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_CHECKFAIL, *tret, "(No.%d) impdrv_impctl_bus_if_check for IMP0 returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_CHECKFAIL));

	/* Fail to execute Bus Interface Check for IMP0 impdrv_impctl_bus_if_check returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_impctl_bus_if_check(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_impctl_bus_if_check for IMP0 returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to execute Bus Interface Check for IMP0 impdrv_impctl_bus_if_check returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_impctl_bus_if_check(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_impctl_bus_if_check for IMP0 returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to execute Bus Interface Check for DMAC00 impdrv_impctl_bus_if_check returns IMPDRV_EC_NG_PARAM */
	p_core_info = get_core_info(exec_cores.core_info, exec_cores.num, IMPDRV_CORE_TYPE_DMAC, 0);
	it_stub_set_impdrv_dmactl_bus_if_check(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_dmactl_bus_if_check for DMAC00 returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to execute Bus Interface Check for IMP0 impdrv_impctl_bus_if_check returns IMPDRV_EC_NG_SYSTEMERROR */
	p_core_info = get_core_info(exec_cores.core_info, exec_cores.num, IMPDRV_CORE_TYPE_DMAC, 0);
	it_stub_set_impdrv_dmactl_bus_if_check(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_impctl_bus_if_check for DMAC00 returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to execute Bus Interface Check for IMP0 impdrv_impctl_bus_if_check returns IMPDRV_EC_NG_NOTSUPPORT */
	p_core_info = exec_cores.core_info;
	it_stub_set_impdrv_impctl_bus_if_check(IMPDRV_EC_NG_NOTSUPPORT);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) impdrv_impctl_bus_if_check for IMP0 returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_NOTSUPPORT));

	/* Fail to restore IRQ mask impdrv_cmnctl_restore_irq_mask returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_cmnctl_restore_irq_mask(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_cmnctl_restore_irq_mask returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to restore IRQ mask impdrv_cmnctl_restore_irq_mask returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_cmnctl_restore_irq_mask(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_restore_irq_mask returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to restore IRQ mask impdrv_cmnctl_restore_irq_mask returns IMPDRV_EC_NG_PARAM */
	it_stub_set_impdrv_cmnctl_restore_irq_mask(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_restore_irq_mask returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

	/* Fail to restore IRQ mask impdrv_cmnctl_restore_irq_mask returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_cmnctl_restore_irq_mask(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_restore_irq_mask returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

	/* Fail to unlock mutex impdrv_cmnctl_mutex_unlock returns IMPDRV_EC_NG_SEQSTATE */
	it_stub_set_impdrv_cmnctl_mutex_unlock(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_cmnctl_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

	/* Fail to unlock mutex impdrv_cmnctl_mutex_unlock returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_cmnctl_mutex_unlock(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to unlock mutex impdrv_cmnctl_mutex_unlock returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_cmnctl_mutex_unlock(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));
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

	p_core_info = exec_cores.core_info;
	
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
		ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, p_chk_resource);
		if (ercd != IMPDRV_EC_OK) {
			core_testmsg(p_core_info, "%s=%d\n", TEST_MSG_NAME, ercd);
			result = -1;
		}

		SuccessTestPrint(result == 0, *tret, "(No.%d) %s%d ", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num);
	}
}

