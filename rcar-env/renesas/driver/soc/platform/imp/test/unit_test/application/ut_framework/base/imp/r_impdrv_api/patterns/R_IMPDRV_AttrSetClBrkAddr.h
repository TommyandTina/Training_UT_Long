#ifndef R_IMPDRV_ATTRSETCLBRKADDR_H_
#define R_IMPDRV_ATTRSETCLBRKADDR_H_

struct R_OSAL_ThsyncMutexLockForTimePeriod_info {
	e_osal_return_t ReturnValue;
	bool is_stub;
};

struct R_OSAL_ThsyncMutexUnlock_info {
	e_osal_return_t ReturnValue;
	bool is_stub;
};

struct R_IMPDRV_AttrSetClBrkAddr_input {
	uint32_t _handle;
	uint32_t _p_core_info;
	st_impdrv_core_info_t p_core_info;
	uintptr_t cl_brk_addr;
	st_impdrv_commonctl_ins_t g_impdrv_cmn_ctl_ins;
	st_impdrv_hwrsc_mng_t g_hwrsc_mng_table;
	st_impdrv_commonctl_chk_t g_impdrv_cmn_ctl_chk;
	struct R_OSAL_ThsyncMutexLockForTimePeriod_info R_OSAL_ThsyncMutexLockForTimePeriod;
	struct R_OSAL_ThsyncMutexUnlock_info R_OSAL_ThsyncMutexUnlock;
};

struct R_IMPDRV_AttrSetClBrkAddr_expect {
	e_impdrv_errorcode_t ercd;
};

struct R_IMPDRV_AttrSetClBrkAddr_params {
	impdrv_ctrl_handle_t handle;
	st_impdrv_core_info_t *p_core_info;
	uintptr_t cl_brk_addr;
};

struct TEST_R_IMPDRV_AttrSetClBrkAddr_Pattern {
	struct R_IMPDRV_AttrSetClBrkAddr_input input;
	struct R_IMPDRV_AttrSetClBrkAddr_expect expected;
};

#endif /* R_IMPDRV_ATTRSETCLBRKADDR_H_ */
