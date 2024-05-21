#include "base.h"
#include "util.h"
#include "patterns/R_IMPDRV_GetPMPolicy_PCL.h"

static bool TEST_GetAddr_R_IMPDRV_GetPMPolicy(uint32_t *flag, struct R_IMPDRV_GetPMPolicy_params *func_params)
{
	bool ret = true;

	switch (*flag)
	{
	case TEST_ADDR_NOT_NULL:
		ret = Test_set_validate_pointer_info((uint32_t)TEST_ADDR_NOT_NULL, "not null\0");
		break;
	case TEST_ADDR_NULL:
		ret = Test_set_validate_pointer_info((uint32_t)TEST_ADDR_NULL, "null\0");
		break;
	default:
		break;
	}

	return ret;
}

static bool TEST_Initialize_R_IMPDRV_GetPMPolicy(int32_t no)
{
	bool b = true;
	struct R_IMPDRV_GetPMPolicy_input *inputs = &R_IMPDRV_GetPMPolicy_PCL[no - 1].input;

	if (inputs->_handle == TEST_ADDR_UNINIT_HANDLE)
	{
		b &= uninitialized_impdrv_ctrl_handle_t(&g_uts_handle);
	}
	if (inputs->_handle == TEST_ADDR_INIT_HANDLE)
	{
		b &= initialized_impdrv_ctrl_handle_t(&g_uts_handle);
	}
	if (inputs->_handle == TEST_ADDR_READY_HANDLE)
	{
		b &= ready_impdrv_ctrl_handle_t(&g_uts_handle);
	}
	if (inputs->_handle == TEST_ADDR_EXEC_HANDLE)
	{
		b &= exec_impdrv_ctrl_handle_t(&g_uts_handle);
	}
	if (inputs->_handle == TEST_ADDR_INT_HANDLE)
	{
		b &= int_impdrv_ctrl_handle_t(&g_uts_handle);
	}
	if (inputs->_handle == TEST_ADDR_INIT_CL_ADDR)
	{
		b &= init_cl_addr_impdrv_ctrl_handle_t(&g_uts_handle);
	}
	if (inputs->_handle == TEST_ADDR_INVALID_MUTEX_PERIOD)
	{
		b &= invalid_mutex_period_impdrv_ctrl_handle_t(&g_uts_handle);
	}
	if (inputs->_handle == TEST_ADDR_INVALID_IRQ_MASK)
	{
		b &= invalid_irq_mask_impdrv_ctrl_handle_t(&g_uts_handle);
	}
	if (inputs->_handle == TEST_ADDR_READY1_HANDLE)
	{
		b &= ready1_impdrv_ctrl_handle_t(&g_uts_handle);
	}
	if (inputs->_handle == TEST_ADDR_STATE_HANDLE)
	{
		b &= stateinitialized_impdrv_ctrl_handle_t(&g_uts_handle);
	}
	if (inputs->_handle == TEST_ADDR_PFUNC)
	{
		b &= pfuncinitialized_impdrv_ctrl_handle_t(&g_uts_handle);
	}
	if (inputs->_handle == TEST_ADDR_STATE_UNHANDLE)
	{
		b &= stateuninitialized_impdrv_ctrl_handle_t(&g_uts_handle);
	}
	if (inputs->_handle == TEST_ADDR_CHECKSUM_HANDLE)
	{
		b &= chkinitialized_impdrv_ctrl_handle_t(&g_uts_handle);
	}
	if (inputs->_handle == TEST_ADDR_PFUNC1)
	{
		b &= pfunc1initialized_impdrv_ctrl_handle_t(&g_uts_handle);
	}
	if (inputs->_handle == TEST_ADDR_INIT_HANDLE1)
	{
		b &= initialized_impdrv_ctrl_handle_t1(&g_uts_handle);
	}
	if (inputs->_handle == TEST_ADDR_STATE_INSTANCE_HANDLE)
	{
		b &= stateinitialized_instance_impdrv_ctrl_handle_t(&g_uts_handle);
	}
	if (inputs->_handle == TEST_ADDR_INIT_INSTANCE_HANDLE)
	{
		b &= initialized_instance_impdrv_ctrl_handle_t(&g_uts_handle);
	}

	return b;
}

static bool TEST_Validate_R_IMPDRV_GetPMPolicy(struct R_IMPDRV_GetPMPolicy_expect *outputs, struct R_IMPDRV_GetPMPolicy_expect *expected, bool validator[])
{
	bool b = true;
	uint64_t index = 0;

	b &= TEST_ValidateOutput(&outputs->ercd, &expected->ercd, sizeof(outputs->ercd), "ercd", TEST_VALIDATOR_IMPDRV_RETCODE);

	if (TEST_CheckBitValidator(validator, index++))
	{
		b &= TEST_ValidateOutput(&outputs->_policy, &expected->_policy, sizeof(outputs->_policy), "*policy", TEST_VALIDATOR_ADDR);
	}

	if (TEST_CheckBitValidator(validator, index++))
	{
		b &= TEST_ValidateOutput(&outputs->policy, &expected->policy, sizeof(outputs->policy), "policy", TEST_VALIDATOR_IMPDRV_POLICY);
	}

	return b;
}

bool TEST_R_IMPDRV_GetPMPolicy(const char *category, int32_t no)
{
	bool b = true;
	struct TestParams *params = TEST_CreateParam("R_IMPDRV_GetPMPolicy", category, no);
	struct TEST_R_IMPDRV_GetPMPolicy_Pattern *pattern = &R_IMPDRV_GetPMPolicy_PCL[no - 1];
	struct R_IMPDRV_GetPMPolicy_input	*inputs = &pattern->input;
	struct R_IMPDRV_GetPMPolicy_params	func_params = {0};
	struct R_IMPDRV_GetPMPolicy_expect	outputs = {0};
	bool is_stub = !inputs->R_OSAL_ThsyncMutexLockForTimePeriod.is_stub | !inputs->R_OSAL_ThsyncMutexUnlock.is_stub | !inputs->R_OSAL_IoDeviceOpen.is_stub | !inputs->R_OSAL_PmGetPolicy.is_stub | !inputs->R_OSAL_IoDeviceClose.is_stub;

	ut_init_imp(is_stub);

	b &= TEST_Initialize_R_IMPDRV_GetPMPolicy(no);

	if (inputs->_handle == TEST_ADDR_NULL)
	{
		func_params.handle = NULL;
	}
	else
	{
		func_params.handle = &g_uts_handle;
	}
	if (inputs->_p_core_info == TEST_ADDR_NULL)
	{
		func_params.p_core_info = NULL;
	}
	else
	{
		func_params.p_core_info = &inputs->p_core_info;
	}
	if (inputs->_policy == TEST_ADDR_NULL)
	{
		func_params.policy = NULL;
	}
	else
	{
		func_params.policy = &g_uts_policy;
	}
	memcpy(&g_impdrv_cmn_ctl_chk, &inputs->g_impdrv_cmn_ctl_chk, sizeof(g_impdrv_cmn_ctl_chk));
	if (inputs->R_OSAL_ThsyncMutexLockForTimePeriod.is_stub)
	{
		memcpy(&g_R_OSAL_ThsyncMutexLockForTimePeriod.ReturnValue, &inputs->R_OSAL_ThsyncMutexLockForTimePeriod.ReturnValue, sizeof(inputs->R_OSAL_ThsyncMutexLockForTimePeriod.ReturnValue));
		g_hook_R_OSAL_ThsyncMutexLockForTimePeriod = ut_stub_R_OSAL_ThsyncMutexLockForTimePeriod;
	}
	if (inputs->R_OSAL_ThsyncMutexUnlock.is_stub)
	{
		memcpy(&g_R_OSAL_ThsyncMutexUnlock.ReturnValue, &inputs->R_OSAL_ThsyncMutexUnlock.ReturnValue, sizeof(inputs->R_OSAL_ThsyncMutexUnlock.ReturnValue));
		g_hook_R_OSAL_ThsyncMutexUnlock = ut_stub_R_OSAL_ThsyncMutexUnlock;
	}
	if (inputs->R_OSAL_IoDeviceOpen.is_stub)
	{
		memcpy(&g_R_OSAL_IoDeviceOpen.ReturnValue, &inputs->R_OSAL_IoDeviceOpen.ReturnValue, sizeof(inputs->R_OSAL_IoDeviceOpen.ReturnValue));
		memcpy(&g_R_OSAL_IoDeviceOpen.p_handle_out, &inputs->R_OSAL_IoDeviceOpen.p_handle, sizeof(inputs->R_OSAL_IoDeviceOpen.p_handle));
		g_hook_R_OSAL_IoDeviceOpen = ut_stub_R_OSAL_IoDeviceOpen;
	}
	if (inputs->R_OSAL_PmGetPolicy.is_stub)
	{
		memcpy(&g_R_OSAL_PmGetPolicy.ReturnValue, &inputs->R_OSAL_PmGetPolicy.ReturnValue, sizeof(inputs->R_OSAL_PmGetPolicy.ReturnValue));
		memcpy(&g_R_OSAL_PmGetPolicy.p_policy_out, &inputs->R_OSAL_PmGetPolicy.p_policy, sizeof(inputs->R_OSAL_PmGetPolicy.p_policy));
		g_hook_R_OSAL_PmGetPolicy = ut_stub_R_OSAL_PmGetPolicy;
	}
	if (inputs->R_OSAL_IoDeviceClose.is_stub)
	{
		memcpy(&g_R_OSAL_IoDeviceClose.ReturnValue, &inputs->R_OSAL_IoDeviceClose.ReturnValue, sizeof(inputs->R_OSAL_IoDeviceClose.ReturnValue));
		g_hook_R_OSAL_IoDeviceClose = ut_stub_R_OSAL_IoDeviceClose;
	}

	outputs.ercd = R_IMPDRV_GetPMPolicy(func_params.handle, func_params.p_core_info, func_params.policy);

	if (pattern->expected._policy == TEST_ADDR_NULL || pattern->expected._policy == TEST_ADDR_NOT_NULL)
	{
		outputs._policy = (func_params.policy == NULL) ? TEST_ADDR_NULL : TEST_ADDR_NOT_NULL;
	}
	else
	{
		outputs._policy = (uint32_t)func_params.policy;
	}
	if (func_params.policy != NULL)
	{
		memcpy(&outputs.policy , func_params.policy, sizeof(outputs.policy));
	}
	b &= TEST_GetAddr_R_IMPDRV_GetPMPolicy(&pattern->expected._policy, &func_params);

	b &= TEST_Validate_R_IMPDRV_GetPMPolicy(&outputs, &pattern->expected, pattern->validator);

	TEST_ValidateResult(b, params);

	ut_deinit_imp();

	TEST_DestroyParam(params);

	return b;
}
