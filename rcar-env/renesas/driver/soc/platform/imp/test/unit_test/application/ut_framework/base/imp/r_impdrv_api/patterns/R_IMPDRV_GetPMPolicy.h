#ifndef R_IMPDRV_GETPMPOLICY_H_
#define R_IMPDRV_GETPMPOLICY_H_

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

struct R_OSAL_PmGetPolicy_info {
	e_osal_return_t ReturnValue;
	e_osal_pm_policy_t p_policy;
	bool is_stub;
};

struct R_OSAL_IoDeviceClose_info {
	e_osal_return_t ReturnValue;
	bool is_stub;
};

struct R_IMPDRV_GetPMPolicy_input {
	uint32_t _handle;
	uint32_t _p_core_info;
	st_impdrv_core_info_t p_core_info;
	uint32_t _policy;
	st_impdrv_commonctl_chk_t g_impdrv_cmn_ctl_chk;
	struct R_OSAL_ThsyncMutexLockForTimePeriod_info R_OSAL_ThsyncMutexLockForTimePeriod;
	struct R_OSAL_ThsyncMutexUnlock_info R_OSAL_ThsyncMutexUnlock;
	struct R_OSAL_IoDeviceOpen_info R_OSAL_IoDeviceOpen;
	struct R_OSAL_PmGetPolicy_info R_OSAL_PmGetPolicy;
	struct R_OSAL_IoDeviceClose_info R_OSAL_IoDeviceClose;
};

struct R_IMPDRV_GetPMPolicy_expect {
	e_impdrv_errorcode_t ercd;
	uint32_t _policy;
	e_impdrv_pm_policy_t policy;
};

struct R_IMPDRV_GetPMPolicy_params {
	impdrv_ctrl_handle_t handle;
	st_impdrv_core_info_t *p_core_info;
	e_impdrv_pm_policy_t *policy;
};

struct TEST_R_IMPDRV_GetPMPolicy_Pattern {
	struct R_IMPDRV_GetPMPolicy_input input;
	struct R_IMPDRV_GetPMPolicy_expect expected;
	bool validator[2];
};

#endif /* R_IMPDRV_GETPMPOLICY_H_ */
