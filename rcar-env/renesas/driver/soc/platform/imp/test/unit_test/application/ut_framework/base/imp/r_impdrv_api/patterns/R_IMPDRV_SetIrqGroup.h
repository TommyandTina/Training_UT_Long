#ifndef R_IMPDRV_SETIRQGROUP_H_
#define R_IMPDRV_SETIRQGROUP_H_

struct R_OSAL_ThsyncMutexLockForTimePeriod_info {
	e_osal_return_t ReturnValue;
	bool is_stub;
};

struct R_OSAL_ThsyncMutexUnlock_info {
	e_osal_return_t ReturnValue;
	bool is_stub;
};

struct R_OSAL_IoRead32_info {
	e_osal_return_t ReturnValue[6];
	uint32_t lRegData[6];
	bool is_stub;
};

struct R_IMPDRV_SetIrqGroup_input {
	uint32_t _handle;
	uint32_t _p_irq_param;
	st_impdrv_irq_group_t p_irq_param;
	st_impdrv_commonctl_chk_t g_impdrv_cmn_ctl_chk;
	struct R_OSAL_ThsyncMutexLockForTimePeriod_info R_OSAL_ThsyncMutexLockForTimePeriod;
	struct R_OSAL_ThsyncMutexUnlock_info R_OSAL_ThsyncMutexUnlock;
	struct R_OSAL_IoRead32_info R_OSAL_IoRead32;
};

struct R_IMPDRV_SetIrqGroup_expect {
	e_impdrv_errorcode_t ercd;
};

struct R_IMPDRV_SetIrqGroup_params {
	impdrv_ctrl_handle_t handle;
	st_impdrv_irq_group_t *p_irq_param;
};

struct TEST_R_IMPDRV_SetIrqGroup_Pattern {
	struct R_IMPDRV_SetIrqGroup_input input;
	struct R_IMPDRV_SetIrqGroup_expect expected;
};

#endif /* R_IMPDRV_SETIRQGROUP_H_ */
