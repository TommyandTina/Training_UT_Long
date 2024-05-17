#ifndef R_IMPDRV_SETPMPOLICY_H_
#define R_IMPDRV_SETPMPOLICY_H_

struct R_OSAL_ThsyncMutexLockForTimePeriod_info {
	e_osal_return_t ReturnValue;
	bool is_stub;
};

struct R_OSAL_ThsyncMutexUnlock_info {
	e_osal_return_t ReturnValue;
	bool is_stub;
};

struct R_OSAL_IoDeviceOpen_info {
	e_osal_return_t ReturnValue[2];
	uint32_t p_handle[2];
	bool is_stub;
};

struct R_OSAL_PmSetPolicy_info {
	e_osal_return_t ReturnValue;
	bool is_stub;
};

struct R_OSAL_PmSetRequiredState_info {
	e_osal_return_t ReturnValue[2];
	bool is_stub;
};

struct R_OSAL_IoDeviceClose_info {
	e_osal_return_t ReturnValue;
	bool is_stub;
};

struct R_IMPDRV_SetPMPolicy_input {
	uint32_t _handle;
	uint32_t _p_core_info;
	st_impdrv_core_info_t p_core_info;
	e_impdrv_pm_policy_t policy;
	st_impdrv_commonctl_ins_t g_impdrv_cmn_ctl_ins;
	st_impdrv_hwrsc_mng_t g_hwrsc_mng_table;
	struct R_OSAL_ThsyncMutexLockForTimePeriod_info R_OSAL_ThsyncMutexLockForTimePeriod;
	struct R_OSAL_ThsyncMutexUnlock_info R_OSAL_ThsyncMutexUnlock;
	struct R_OSAL_IoDeviceOpen_info R_OSAL_IoDeviceOpen;
	struct R_OSAL_PmSetPolicy_info R_OSAL_PmSetPolicy;
	struct R_OSAL_PmSetRequiredState_info R_OSAL_PmSetRequiredState;
	struct R_OSAL_IoDeviceClose_info R_OSAL_IoDeviceClose;
};

struct R_IMPDRV_SetPMPolicy_expect {
	e_impdrv_errorcode_t ercd;
};

struct R_IMPDRV_SetPMPolicy_params {
	impdrv_ctrl_handle_t handle;
	st_impdrv_core_info_t *p_core_info;
	e_impdrv_pm_policy_t policy;
};

struct TEST_R_IMPDRV_SetPMPolicy_Pattern {
	struct R_IMPDRV_SetPMPolicy_input input;
	struct R_IMPDRV_SetPMPolicy_expect expected;
};

#endif /* R_IMPDRV_SETPMPOLICY_H_ */
