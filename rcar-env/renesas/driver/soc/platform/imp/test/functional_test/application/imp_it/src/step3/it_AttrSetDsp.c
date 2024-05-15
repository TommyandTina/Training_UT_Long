
#define TEST_MSG_NAME "R_IMPDRV_AttrSetDsp"

#include "it_common.h"
#include "it_stub.h"

#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret);
#endif
static void Device_attr_set_dsp(struct test_params_t *tparam, struct test_result_t *tret);

static uint32_t testno;
static uintptr_t addr_phys_backup[3];

int32_t it_AttrSetDsp_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

#ifdef IT_ABNORMAL
	testno = 31;
	Parameter_error(tparam, &tret);

#endif
	Device_attr_set_dsp(tparam, &tret);

	PrintTestResult(tret, test_result);

	return 0;
}

#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
    const st_impdrv_initdata_t *const p_initdata = (const st_impdrv_initdata_t *const)&tparam->initdata;
    const st_impdrv_dsp_data_t *const p_dspdata = (const st_impdrv_dsp_data_t *const)&tparam->dsp_data;
    const st_impdrv_dsp_data_t *const p_dspfw = (const st_impdrv_dsp_data_t *const)&tparam->dsp_fw;
    const st_impdrv_dsp_data_t *const p_dspapp = (const st_impdrv_dsp_data_t *const)&tparam->dsp_app;
    const st_impdrv_dsp_data_t *const p_dspdtcm = (const st_impdrv_dsp_data_t *const)&tparam->dsp_dtcm;
	st_impdrv_core_info_t *p_core_info = tparam->initdata.core_info;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;

	/* Parameter_error : handle == NULL */
	ercd = R_IMPDRV_AttrSetDsp(NULL, p_core_info, p_dspapp, p_dspfw, p_dspdata, p_dspdtcm);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) handle == NULL", testno++);

	/* Parameter_error : p_core_info == NULL */
	ercd = R_IMPDRV_AttrSetDsp(handle, NULL, p_dspapp, p_dspfw, p_dspdata, p_dspdtcm);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) p_core_info == NULL", testno++);

	/* Parameter_error : p_dspapp == NULL */
	ercd = R_IMPDRV_AttrSetDsp(handle, p_core_info, NULL, p_dspfw, p_dspdata, p_dspdtcm);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) p_dspapp == NULL", testno++);

	/* Parameter_error : p_dspfw == NULL */
	ercd = R_IMPDRV_AttrSetDsp(handle, p_core_info, p_dspapp, NULL, p_dspdata, p_dspdtcm);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) p_dspfw == NULL", testno++);

	/* Parameter_error : p_dspdata == NULL */
	ercd = R_IMPDRV_AttrSetDsp(handle, p_core_info, p_dspapp, p_dspfw, NULL, p_dspdtcm);
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d)  p_dspdata == NULL", testno++);

	/* Back up data */
	memcpy(&addr_phys_backup[0], &tparam->dsp_app.addr_phys, sizeof(tparam->dsp_app.addr_phys));
	memcpy(&addr_phys_backup[1], &tparam->dsp_fw.addr_phys, sizeof(tparam->dsp_fw.addr_phys));
	memcpy(&addr_phys_backup[2], &tparam->dsp_data.addr_phys, sizeof(tparam->dsp_data.addr_phys));

	/* Parameter_error : p_dspapp->addr_phys = 5U */
	tparam->dsp_app.addr_phys = 5U;
	ercd = R_IMPDRV_AttrSetDsp(handle, p_core_info, p_dspapp, p_dspfw, p_dspdata, p_dspdtcm);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) p_dspapp->addr_phys = 5U", testno++);

	/* Parameter_error : p_dspapp->addr_phys = 3U */
	tparam->dsp_app.addr_phys = 3U;
	ercd = R_IMPDRV_AttrSetDsp(handle, p_core_info, p_dspapp, p_dspfw, p_dspdata, p_dspdtcm);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) p_dspapp->addr_phys = 3U", testno++);

	/* Parameter_error : p_dspapp->addr_phys = 2U */
	tparam->dsp_app.addr_phys = 2U;
	ercd = R_IMPDRV_AttrSetDsp(handle, p_core_info, p_dspapp, p_dspfw, p_dspdata, p_dspdtcm);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) p_dspapp->addr_phys = 2U", testno++);

	/* Parameter_error : p_dspfw->addr_phys = 5U */
	tparam->dsp_fw.addr_phys = 5U;
	ercd = R_IMPDRV_AttrSetDsp(handle, p_core_info, p_dspapp, p_dspfw, p_dspdata, p_dspdtcm);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) p_dspfw->addr_phys = 5U", testno++);

	/* Parameter_error : p_dspfw->addr_phys = 3U */
	tparam->dsp_fw.addr_phys = 3U;
	ercd = R_IMPDRV_AttrSetDsp(handle, p_core_info, p_dspapp, p_dspfw, p_dspdata, p_dspdtcm);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) p_dspfw->addr_phys = 3U", testno++);

	/* Parameter_error : p_dspfw->addr_phys = 2U */
	tparam->dsp_fw.addr_phys = 2U;
	ercd = R_IMPDRV_AttrSetDsp(handle, p_core_info, p_dspapp, p_dspfw, p_dspdata, p_dspdtcm);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) p_dspfw->addr_phys = 2U", testno++);

	/* Parameter_error : p_dspdata->addr_phys = 5U */
	tparam->dsp_data.addr_phys = 5U;
	ercd = R_IMPDRV_AttrSetDsp(handle, p_core_info, p_dspapp, p_dspfw, p_dspdata, p_dspdtcm);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) p_dspdata->addr_phys = 5U", testno++);

	/* Parameter_error : p_dspdata->addr_phys = 3U */
	tparam->dsp_data.addr_phys = 3U;
	ercd = R_IMPDRV_AttrSetDsp(handle, p_core_info, p_dspapp, p_dspfw, p_dspdata, p_dspdtcm);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) p_dspdata->addr_phys = 3U", testno++);

	/* Parameter_error : p_dspdata->addr_phys = 2U */
	tparam->dsp_data.addr_phys = 2U;
	ercd = R_IMPDRV_AttrSetDsp(handle, p_core_info, p_dspapp, p_dspfw, p_dspdata, p_dspdtcm);
	it_stub_reset();
	FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) p_dspdata->addr_phys = 2U", testno++);

	/* Get backup data */
	memcpy(&tparam->dsp_app.addr_phys, &addr_phys_backup[0], sizeof(addr_phys_backup[0]));
	memcpy(&tparam->dsp_fw.addr_phys, &addr_phys_backup[1], sizeof(addr_phys_backup[1]));
	memcpy(&tparam->dsp_data.addr_phys, &addr_phys_backup[2], sizeof(addr_phys_backup[2]));

    return;
}



#endif

static void Device_attr_set_dsp(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
    const st_impdrv_initdata_t *const p_initdata = (const st_impdrv_initdata_t *const)&tparam->initdata;
    const st_impdrv_dsp_data_t *const p_dspdata = (const st_impdrv_dsp_data_t *const)&tparam->dsp_data;
    const st_impdrv_dsp_data_t *const p_dspfw = (const st_impdrv_dsp_data_t *const)&tparam->dsp_fw;
    const st_impdrv_dsp_data_t *const p_dspapp = (const st_impdrv_dsp_data_t *const)&tparam->dsp_app;
    const st_impdrv_dsp_data_t *const p_dspdtcm = (const st_impdrv_dsp_data_t *const)&tparam->dsp_dtcm;
	st_impdrv_core_info_t *p_core_info = tparam->initdata.core_info;
    st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)p_initdata->p_work_addr;
    impdrv_ctrl_handle_t handle = (impdrv_ctrl_handle_t)p_impdrv_ctl;
	int32_t result = -1;
    st_impdrv_core_info_t *dsp_core_info;
	
	tparam->dsp_app.addr_phys = 0x40000000U;
    tparam->dsp_app.size = 0x2000U;

    for (uint32_t i = 0; i < p_initdata->use_core_num; i++)
    {
        if (p_initdata->core_info[i].core_type == IMPDRV_CORE_TYPE_DSP)
        {
            dsp_core_info = &p_initdata->core_info[i];
        }
    }

    ercd = R_IMPDRV_AttrSetDsp(handle, dsp_core_info, p_dspapp, p_dspfw, p_dspdata, p_dspdtcm);
    if (ercd != IMPDRV_EC_OK)
    {
        testmsg("(No.%d) fail(test NG %d)\n", testno++, ercd);
        result = -1;
    } else {
        testmsg("(No.%d) Success(test OK)\n", testno++);
        result = 0;
    }
    it_stub_reset();

    CountResult(result == 0, tret);

	return;
}

