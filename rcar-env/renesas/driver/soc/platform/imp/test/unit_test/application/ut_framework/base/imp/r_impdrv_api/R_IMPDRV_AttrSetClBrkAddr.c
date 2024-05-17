#include "base.h"
#include "util.h"
#include "patterns/R_IMPDRV_AttrSetClBrkAddr_PCL.h"

static bool TEST_Initialize_R_IMPDRV_AttrSetClBrkAddr(int32_t no)
{
	bool b = true;
	struct R_IMPDRV_AttrSetClBrkAddr_input *inputs = &R_IMPDRV_AttrSetClBrkAddr_PCL[no - 1].input;

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

static bool TEST_Validate_R_IMPDRV_AttrSetClBrkAddr(struct R_IMPDRV_AttrSetClBrkAddr_expect *outputs, struct R_IMPDRV_AttrSetClBrkAddr_expect *expected, bool validator[])
{
	bool b = true;
	b &= TEST_ValidateOutput(&outputs->ercd, &expected->ercd, sizeof(outputs->ercd), "ercd", TEST_VALIDATOR_IMPDRV_RETCODE);

	return b;
}

bool TEST_R_IMPDRV_AttrSetClBrkAddr(const char *category, int32_t no)
{
	bool b = true;
	struct TestParams *params = TEST_CreateParam("R_IMPDRV_AttrSetClBrkAddr", category, no);
	struct TEST_R_IMPDRV_AttrSetClBrkAddr_Pattern *pattern = &R_IMPDRV_AttrSetClBrkAddr_PCL[no - 1];
	struct R_IMPDRV_AttrSetClBrkAddr_input	*inputs = &pattern->input;
	struct R_IMPDRV_AttrSetClBrkAddr_params	func_params = {0};
	struct R_IMPDRV_AttrSetClBrkAddr_expect	outputs = {0};
	bool is_stub = !inputs->R_OSAL_ThsyncMutexLockForTimePeriod.is_stub | !inputs->R_OSAL_ThsyncMutexUnlock.is_stub;

	ut_init_imp(is_stub);

	b &= TEST_Initialize_R_IMPDRV_AttrSetClBrkAddr(no);

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
	memcpy(&func_params.cl_brk_addr, &inputs->cl_brk_addr, sizeof(func_params.cl_brk_addr));
	memcpy(&g_impdrv_cmn_ctl_ins, &inputs->g_impdrv_cmn_ctl_ins, sizeof(g_impdrv_cmn_ctl_ins));
	memcpy(&g_hwrsc_mng_table, &inputs->g_hwrsc_mng_table, sizeof(g_hwrsc_mng_table));
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

	outputs.ercd = R_IMPDRV_AttrSetClBrkAddr(func_params.handle, func_params.p_core_info, func_params.cl_brk_addr);

	b &= TEST_Validate_R_IMPDRV_AttrSetClBrkAddr(&outputs, &pattern->expected, NULL);

	TEST_ValidateResult(b, params);

	ut_deinit_imp();

	TEST_DestroyParam(params);

	return b;
}
