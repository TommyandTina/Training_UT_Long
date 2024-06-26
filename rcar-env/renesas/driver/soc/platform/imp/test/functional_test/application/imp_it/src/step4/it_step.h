extern int32_t it_InitMemoryCorruption_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_Init_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_SetMemProtect_UNINIT_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_RegGetHwInfo_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_RegRequired_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_RegWrite32_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_RegRead32_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_SetMemProtect_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_GetVersion_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_SetPMPolicy_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_GetPMPolicy_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_AttrInit_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_GetPMState_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_AttrSetCoreMemInit_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_Start_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_AttrSetCoreMap_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_AttrSetCl_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_AttrSetClBrkAddr_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_AttrSetDsp_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_AttrSetIrqMask_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_SetIrqGroup_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_Execute_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_ResumeExecution_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_Append_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_CBFUNC_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_BusIfCheck_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_ConfRegCheck_EDC_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_ModuleStopCheck_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_Stop_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_ConfRegCheck_ECC_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_Quit_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_Multiple_Instance_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_CoreExec_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_Long_Time_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_CoreExec_TRAP_Main(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_USIER(struct test_params_t *tparam, struct test_result_t *test_result);
extern int32_t it_AttrSetGosubCond_Main(struct test_params_t *tparam, struct test_result_t *test_result);

#define RUN_TEST()		\
{	\
		run_test(it_InitMemoryCorruption_Main,      tparam, tret, test_result); \
		run_test(it_GetVersion_Main,				tparam, tret, test_result);	\
		run_test(it_SetMemProtect_UNINIT_Main,		tparam, tret, test_result);	\
		run_test(it_Init_Main,						tparam, tret, test_result);	\
		run_test(it_RegGetHwInfo_Main,				tparam, tret, test_result);	\
		run_test(it_RegRequired_Main,				tparam, tret, test_result);	\
		run_test(it_RegWrite32_Main,				tparam, tret, test_result);	\
		run_test(it_RegRead32_Main,					tparam, tret, test_result);	\
		run_test(it_SetMemProtect_Main,				tparam, tret, test_result);	\
		run_test(it_SetPMPolicy_Main,				tparam, tret, test_result);	\
		run_test(it_GetPMPolicy_Main,				tparam, tret, test_result);	\
		run_test(it_AttrInit_Main,					tparam, tret, test_result);	\
		run_test(it_GetPMState_Main,				tparam, tret, test_result); \
		run_test(it_AttrSetCoreMemInit_Main,		tparam, tret, test_result);	\
		run_test(it_AttrSetDsp_Main,				tparam, tret, test_result);	\
		run_test(it_Start_Main,						tparam, tret, test_result);	\
		run_test(it_ConfRegCheck_EDC_Main,			tparam, tret, test_result);	\
		run_test(it_ModuleStopCheck_Main,			tparam, tret, test_result);	\
		run_test(it_BusIfCheck_Main,				tparam, tret, test_result);	\
		run_test(it_AttrSetCl_Main,					tparam, tret, test_result);	\
		run_test(it_AttrSetCoreMap_Main,			tparam, tret, test_result);	\
		run_test(it_Execute_Main,					tparam, tret, test_result);	\
		run_test(it_ResumeExecution_Main,			tparam, tret, test_result);	\
		run_test(it_Append_Main,					tparam, tret, test_result);	\
		run_test(it_CBFUNC_Main,					tparam, tret, test_result);	\
		run_test(it_AttrSetClBrkAddr_Main,			tparam, tret, test_result);	\
		run_test(it_AttrSetGosubCond_Main,			tparam, tret, test_result);	\
		run_test(it_SetIrqGroup_Main,				tparam, tret, test_result); \
		run_test(it_AttrSetIrqMask_Main,			tparam, tret, test_result);	\
		run_test(it_Stop_Main,						tparam, tret, test_result);	\
		run_test(it_ConfRegCheck_ECC_Main,			tparam, tret, test_result);	\
		run_test(it_Quit_Main,						tparam, tret, test_result);	\
		run_test(it_Multiple_Instance_Main,			tparam, tret, test_result);	\
}

#define RUN_TEST_2()		\
{	\
		run_test(it_CoreExec_Main,					tparam, tret, test_result);	\
		run_test(it_CoreExec_TRAP_Main,				tparam, tret, test_result);	\
		run_test(it_Long_Time_Main,					tparam, tret, test_result);	\
}

#define RUN_TEST_3()		\
{	\
		run_test(it_USIER,							tparam, tret, test_result);	\
}