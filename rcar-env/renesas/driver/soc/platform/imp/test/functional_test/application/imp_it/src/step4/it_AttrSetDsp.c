
#define TEST_MSG_NAME   "R_IMPDRV_AttrSetDsp"
#define DSP_CORE_INDEX 	12U
#define ADDR_PHYS_MAX 	3U

#include "it_common.h"

static void set_state(st_impdrv_initdata_t *const p_data, e_impdrv_state_t state);
#ifdef IT_ABNORMAL
static void Parameter_error(struct test_params_t *tparam, struct test_result_t *tret);
#endif
static void Device_attr_set_dsp(struct test_params_t *tparam, struct test_result_t *tret);

static uint32_t testno;
static st_impdrv_coretypectl_t coretype_ctl_backup[IMPDRV_CORE_TYPE_MAX];
static uintptr_t addr_phys_backup[ADDR_PHYS_MAX];

char *num_to_name(uint32_t num)
{
	switch (num) {
		case 0:		return "p_dspapp";
		case 1:		return "p_dspfw";
		case 2:		return "p_dspdata";
		case 3:		return "p_dspdtcm";
	}

	return "Unknown";
}

int32_t it_AttrSetDsp_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

#ifdef IT_ABNORMAL
	testno = 46;
    Parameter_error(tparam, &tret);
#endif
	testno = 60;
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
	uintptr_t tparam_addr_phys[ADDR_PHYS_MAX] = {&tparam->dsp_app.addr_phys, &tparam->dsp_fw.addr_phys, &tparam->dsp_data.addr_phys};
	uintptr_t dsp_addr_phys[ADDR_PHYS_MAX];

	for (uint32_t cnt = 0; cnt < ADDR_PHYS_MAX; cnt++)
	{
		dsp_addr_phys[cnt] = util_mem_get_paddr(tparam_addr_phys[cnt]);
	}

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
	FailTestPrint(ercd, IMPDRV_EC_NG_ARGNULL, *tret, "(No.%d) p_dspdata == NULL", testno++);

	/* Backup data */
	for (uint32_t cnt = 0; cnt < ADDR_PHYS_MAX; cnt++)
	{
		memcpy(&addr_phys_backup[cnt], tparam_addr_phys[cnt], sizeof(tparam_addr_phys[cnt]));
	}

	/* Parameter_error : addr_phys = 5U */
	for (uint32_t cnt = 0; cnt < ADDR_PHYS_MAX; cnt++)
	{
		dsp_addr_phys[cnt] = 5U;
		memcpy(tparam_addr_phys[cnt], &dsp_addr_phys[cnt], sizeof(dsp_addr_phys[cnt]));
		ercd = R_IMPDRV_AttrSetDsp(handle, p_core_info, p_dspapp, p_dspfw, p_dspdata, p_dspdtcm);
		FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) Parameter_error: %s->addr_phys = 5U", testno++, num_to_name(cnt));
	}
	/* Parameter_error : addr_phys = 3U */
	for (uint32_t cnt = 0; cnt < ADDR_PHYS_MAX; cnt++)
	{
		dsp_addr_phys[cnt] = 3U;
		memcpy(tparam_addr_phys[cnt], &dsp_addr_phys[cnt], sizeof(dsp_addr_phys[cnt]));
		ercd = R_IMPDRV_AttrSetDsp(handle, p_core_info, p_dspapp, p_dspfw, p_dspdata, p_dspdtcm);
		FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) Parameter_error: %s->addr_phys = 3U", testno++, num_to_name(cnt));
	}
	/* Parameter_error : addr_phys = 2U */
	for (uint32_t cnt = 0; cnt < ADDR_PHYS_MAX; cnt++)
	{
		dsp_addr_phys[cnt] = 2U;
		memcpy(tparam_addr_phys[cnt], &dsp_addr_phys[cnt], sizeof(dsp_addr_phys[cnt]));
		ercd = R_IMPDRV_AttrSetDsp(handle, p_core_info, p_dspapp, p_dspfw, p_dspdata, p_dspdtcm);
		FailTestPrint(ercd, IMPDRV_EC_NG_PARAM, *tret, "(No.%d) Parameter_error: %s->addr_phys = 2U", testno++, num_to_name(cnt));
	}

	/* Get backup data */
	for (uint32_t cnt = 0; cnt < ADDR_PHYS_MAX; cnt++)
	{
		memcpy(tparam_addr_phys[cnt], &addr_phys_backup[cnt], sizeof(addr_phys_backup[cnt]));
	}

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
    st_impdrv_core_info_t *dsp_core_info;
	int32_t result = -1;
	
    /* Setting DSP app */
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

    CountResult(result == 0, tret);
    return;
}
