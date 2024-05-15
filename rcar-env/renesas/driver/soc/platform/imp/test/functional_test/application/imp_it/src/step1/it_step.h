extern int32_t it_GetVersion_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_Init_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_RegGetHwInfo_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_RegRequired_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_RegWrite32_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_RegRead32_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_SetMemProtect_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_SetPMPolicy_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_GetPMPolicy_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_GetPMState_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_AttrInit_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_AttrSetCoreMemInit_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_AttrSetDsp_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_Start_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_SetIrqGroup_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_AttrSetCoreMap_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_AttrSetCl_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_AttrSetClBrkAddr_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_AttrSetIrqMask_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_Execute_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_ResumeExecution_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_BusIfCheck_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_ConfRegCheck_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_ModuleStopCheck_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_Stop_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_Quit_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_AttrSetGosubCond_Main(struct test_params_t *tparam, struct test_result_t *test_result);

#if defined (V4H) || defined (V4H2)
#define RUN_DSP_TEST    run_test(it_AttrSetDsp_Main, tparam, tret, test_result);/*TODO: DSP support V4H only*/
#else
#define RUN_DSP_TEST
#endif

#define RUN_TEST()		\
{	\
		run_test(it_GetVersion_Main,			tparam, tret, test_result); \
		run_test(it_Init_Main,					tparam, tret, test_result);	\
		run_test(it_RegGetHwInfo_Main,			tparam, tret, test_result);	\
		run_test(it_RegRequired_Main,			tparam, tret, test_result);	\
		run_test(it_RegWrite32_Main,			tparam, tret, test_result);	\
		run_test(it_RegRead32_Main,				tparam, tret, test_result);	\
		run_test(it_SetMemProtect_Main,			tparam, tret, test_result);	\
		run_test(it_SetPMPolicy_Main,			tparam, tret, test_result); \
		run_test(it_GetPMPolicy_Main,			tparam, tret, test_result); \
		run_test(it_AttrInit_Main,				tparam, tret, test_result);	\
		run_test(it_GetPMState_Main,			tparam, tret, test_result); \
		run_test(it_AttrSetCoreMemInit_Main,	tparam, tret, test_result);	\
		run_test(it_Start_Main,					tparam, tret, test_result);	\
		run_test(it_SetIrqGroup_Main,			tparam, tret, test_result);	\
		run_test(it_AttrSetCl_Main,				tparam, tret, test_result);	\
		run_test(it_AttrSetClBrkAddr_Main,				tparam, tret, test_result);	\
		run_test(it_AttrSetGosubCond_Main,		tparam, tret, test_result);	\
		RUN_DSP_TEST\
		run_test(it_Execute_Main,				tparam, tret, test_result);	\
		run_test(it_ResumeExecution_Main,		tparam, tret, test_result);	\
		run_test(it_AttrSetCoreMap_Main,		tparam, tret, test_result);	\
		run_test(it_AttrSetIrqMask_Main,		tparam, tret, test_result);	\
		run_test(it_BusIfCheck_Main,			tparam, tret, test_result);	\
		run_test(it_ConfRegCheck_Main,			tparam, tret, test_result);	\
		run_test(it_ModuleStopCheck_Main,		tparam, tret, test_result);	\
		run_test(it_Stop_Main,					tparam, tret, test_result);	\
		run_test(it_Quit_Main,					tparam, tret, test_result);	\
}
