
#define TEST_MSG_NAME "R_IMPDRV_AttrSetGosubCond"

#include "it_common.h"
#include "it_stub.h"

#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret);
static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret);
static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret);
static void Return_error(struct test_params_t *tparam, struct test_result_t *tret);
#endif
static void Device_attr_set_gosub_cond(struct test_params_t *tparam, struct test_result_t *tret);

static uint32_t testno;

int32_t it_AttrSetGosubCond_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

	/* Execute test */
#ifdef IT_ABNORMAL

	testno = 1;
	Parameter_error(tparam, &tret);
    Invalid_core(tparam, &tret);
    No_exec_core(tparam, &tret);
    Return_error(tparam, &tret);

#endif

	Device_attr_set_gosub_cond(tparam, &tret);
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
    e_impdrv_gosub_cond_t valid_cond = IMPDRV_GOSUB_COND_EXEC;
    e_impdrv_gosub_cond_t invalid_cond = IMPDRV_GOSUB_COND_INVALID;

    /* Parameter_error : handle == IMPDRV_HANDLE_INVALID */
    ercd = R_IMPDRV_AttrSetGosubCond(IMPDRV_HANDLE_INVALID, p_core_info, valid_cond);
    FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) handle == IMPDRV_HANDLE_INVALID", testno++);

    /* Parameter_error : p_core_info == NULL */
    ercd = R_IMPDRV_AttrSetGosubCond(handle, NULL, valid_cond);
    FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) p_core_info == NULL", testno++);

    /* Parameter_error : condition == IMPDRV_GOSUB_COND_INVALID */
    ercd = R_IMPDRV_AttrSetGosubCond(handle, p_core_info, invalid_cond);
    FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) condition == IMPDRV_GOSUB_COND_INVALID", testno++);

}

static void Invalid_core(struct test_params_t *tparam, struct test_result_t *tret)
{
    e_impdrv_errorcode_t ercd;
    st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_IMP, 0 };
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
    e_impdrv_gosub_cond_t valid_cond = IMPDRV_GOSUB_COND_EXEC;

    /* Invalid_error : Core_type = IMPDRV_CORE_TYPE_INVALID */
	core_info.core_type = IMPDRV_CORE_TYPE_INVALID;
    ercd = R_IMPDRV_AttrSetGosubCond(handle, &core_info, valid_cond);
    it_stub_reset();
    FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) Core_type = IMPDRV_CORE_TYPE_INVALID", testno++);

    /* Invalid_error : Core_num = IMPDRV_CORE_NUM_MAX */
	core_info.core_type = IMPDRV_CORE_TYPE_IMP;
	core_info.core_num = IMPDRV_CORE_NUM_MAX;
    ercd = R_IMPDRV_AttrSetGosubCond(handle, &core_info, valid_cond);
    it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) Core_num = IMPDRV_CORE_NUM_MAX", testno++);
}

static void No_exec_core(struct test_params_t *tparam, struct test_result_t *tret)
{
    e_impdrv_errorcode_t ercd;
	st_impdrv_core_info_t core_info = { IMPDRV_CORE_TYPE_IMP, 0 };
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
    e_impdrv_gosub_cond_t valid_cond = IMPDRV_GOSUB_COND_EXEC;
	tp_core_list_t no_exec_cores = get_core_list(IT_IMPDRV_CMD_NO_EXEC, 0);

    /* Not supported core: Core not supported */
	for (uint32_t i = 0; i < no_exec_cores.num; i++)
    {
        core_info = *(no_exec_cores.core_info + i);
        ercd = R_IMPDRV_AttrSetGosubCond(handle, &core_info, valid_cond);
        FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) Core not supported: %s", testno++, type_to_name(core_info.core_type));
    }
}

static void Return_error(struct test_params_t *tparam, struct test_result_t *tret)
{
    e_impdrv_errorcode_t ercd;
	const st_impdrv_initdata_t *const p_initdata = (const st_impdrv_initdata_t *const)&tparam->initdata;
	st_impdrv_core_info_t *p_core_info = tparam->initdata.core_info;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
    e_impdrv_gosub_cond_t valid_cond = IMPDRV_GOSUB_COND_EXEC;

    /* Fail to lock mutex impdrv_cmnctl_mutex_lock returns IMPDRV_EC_NG_SEQSTATE */
    it_stub_set_impdrv_cmnctl_mutex_lock(IMPDRV_EC_NG_SEQSTATE);
    ercd = R_IMPDRV_AttrSetGosubCond(handle, p_core_info, valid_cond);
    it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_cmnctl_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

    /* Fail to lock mutex impdrv_cmnctl_mutex_lock returns IMPDRV_EC_NG_ARGNULL */
    it_stub_set_impdrv_cmnctl_mutex_lock(IMPDRV_EC_NG_ARGNULL);
    ercd = R_IMPDRV_AttrSetGosubCond(handle, p_core_info, valid_cond);
    it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

    /* Fail to lock mutex impdrv_cmnctl_mutex_lock returns IMPDRV_EC_NG_PARAM */
    it_stub_set_impdrv_cmnctl_mutex_lock(IMPDRV_EC_NG_PARAM);
    ercd = R_IMPDRV_AttrSetGosubCond(handle, p_core_info, valid_cond);
    it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

    /* Fail to lock mutex impdrv_cmnctl_mutex_lock returns IMPDRV_EC_NG_SYSTEMERROR */
    it_stub_set_impdrv_cmnctl_mutex_lock(IMPDRV_EC_NG_SYSTEMERROR);
    ercd = R_IMPDRV_AttrSetGosubCond(handle, p_core_info, valid_cond);
    it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_lock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));

    /* Fail to get core function impdrv_cmnctl_get_corectl_func returns NULL */
    it_stub_set_impdrv_cmnctl_get_corectl_func(NULL);
	ercd = R_IMPDRV_AttrSetGosubCond(handle, p_core_info, valid_cond);
    it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) impdrv_cmnctl_get_corectl_func returns NULL", testno++);

    /* Fail to set attribute for IMP0 impdrv_impctl_set_cond_gosub returns IMPDRV_EC_NG_SEQSTATE */
    p_core_info = get_core_info(p_initdata->core_info, p_initdata->use_core_num, IMPDRV_CORE_TYPE_IMP, 0);
	it_stub_set_impdrv_impctl_set_cond_gosub(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_AttrSetGosubCond(handle, p_core_info, valid_cond);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_impctl_set_cond_gosub returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

    /* Fail to set attribute for IMP0 impdrv_ocvctl_set_cond_gosub returns IMPDRV_EC_NG_ARGNULL */
    p_core_info = get_core_info(p_initdata->core_info, p_initdata->use_core_num, IMPDRV_CORE_TYPE_OCV, 0);
	it_stub_set_impdrv_ocvctl_set_cond_gosub(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_AttrSetGosubCond(handle, p_core_info, valid_cond);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_ocvctl_set_cond_gosub returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

    /* Fail to set attribute for IMP0 impdrv_dmactl_set_cond_gosub returns IMPDRV_EC_NG_PARAM */
    p_core_info = get_core_info(p_initdata->core_info, p_initdata->use_core_num, IMPDRV_CORE_TYPE_DMAC, 0);
	it_stub_set_impdrv_dmactl_set_cond_gosub(IMPDRV_EC_NG_PARAM);
	ercd = R_IMPDRV_AttrSetGosubCond(handle, p_core_info, valid_cond);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_dmactl_set_cond_gosub returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_PARAM));

    /* Fail to set attribute for IMP0 impdrv_pscctl_set_cond_gosub returns IMPDRV_EC_NG_NOTSUPPORT */
    p_core_info = get_core_info(p_initdata->core_info, p_initdata->use_core_num, IMPDRV_CORE_TYPE_PSCEXE, 0);
	it_stub_set_impdrv_pscctl_set_cond_gosub(IMPDRV_EC_NG_NOTSUPPORT);
	ercd = R_IMPDRV_AttrSetGosubCond(handle, p_core_info, valid_cond);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_NOTSUPPORT, *tret, "(No.%d) impdrv_pscctl_set_cond_gosub returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_NOTSUPPORT));

    /* Fail to unlock mutex impdrv_cmnctl_mutex_unlock returns IMPDRV_EC_NG_SEQSTATE */
	p_core_info = p_initdata->core_info;
	it_stub_set_impdrv_cmnctl_mutex_unlock(IMPDRV_EC_NG_SEQSTATE);
	ercd = R_IMPDRV_AttrSetGosubCond(handle, p_core_info, valid_cond);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SEQSTATE, *tret, "(No.%d) impdrv_cmnctl_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SEQSTATE));

    /* Fail to unlock mutex impdrv_cmnctl_mutex_unlock returns IMPDRV_EC_NG_ARGNULL */
	it_stub_set_impdrv_cmnctl_mutex_unlock(IMPDRV_EC_NG_ARGNULL);
	ercd = R_IMPDRV_AttrSetGosubCond(handle, p_core_info, valid_cond);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_ARGNULL));

	/* Fail to unlock mutex impdrv_cmnctl_mutex_unlock returns IMPDRV_EC_NG_SYSTEMERROR */
	it_stub_set_impdrv_cmnctl_mutex_unlock(IMPDRV_EC_NG_SYSTEMERROR);
	ercd = R_IMPDRV_AttrSetGosubCond(handle, p_core_info, valid_cond);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_SYSTEMERROR, *tret, "(No.%d) impdrv_cmnctl_mutex_unlock returns %s", testno++, impdrv_err_to_name(IMPDRV_EC_NG_SYSTEMERROR));
}

#endif

static void Device_attr_set_gosub_cond(struct test_params_t *tparam, struct test_result_t *tret)
{
    e_impdrv_errorcode_t ercd;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
    e_impdrv_gosub_cond_t valid_cond = IMPDRV_GOSUB_COND_EXEC;
	int32_t result;

    for (uint32_t i = 0; i < tparam->initdata.use_core_num; i++)
    {
        st_impdrv_core_info_t *p_core_info = &tparam->initdata.core_info[i];
        result = 0;

        if(p_core_info->core_type != IMPDRV_CORE_TYPE_DSP)
        {
            it_stub_reset();
            ercd = R_IMPDRV_AttrSetGosubCond(handle, p_core_info, valid_cond);
            if (ercd != IMPDRV_EC_OK) {
                core_testmsg(p_core_info, "%s=%d\n", TEST_MSG_NAME, ercd);
                result = -1;
            }

            SuccessTestPrint(result == 0, *tret, "(No.%d) %s%d ", testno++, type_to_name(p_core_info->core_type), p_core_info->core_num);
        }
    }
}