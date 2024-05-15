#include "logger.h"
#include "ut_osal_stub.h"

/* Define function to stub*/
e_impdrv_errorcode_t stub_p_impdrvCorectlSyncStop(
    st_impdrv_coretypectl_t coretype_ctl[IMPDRV_CORE_TYPE_MAX],
    const uint32_t          core_num
);

e_impdrv_errorcode_t stub_p_impdrvCorectlSyncStart(
    st_impdrv_coretypectl_t coretype_ctl[IMPDRV_CORE_TYPE_MAX],
    const uint32_t          core_num
);

/* Declare stub global variable*/ 
s_R_OSAL_IoGetNumOfDevices g_R_OSAL_IoGetNumOfDevices;
s_R_OSAL_IoGetDeviceList g_R_OSAL_IoGetDeviceList;
s_R_OSAL_IoDeviceOpen g_R_OSAL_IoDeviceOpen;
s_R_OSAL_IoDeviceClose g_R_OSAL_IoDeviceClose;
s_R_OSAL_PmSetPolicy g_R_OSAL_PmSetPolicy;
s_R_OSAL_InterruptGetNumOfIrqChannels g_R_OSAL_InterruptGetNumOfIrqChannels;
s_R_OSAL_InterruptUnregisterIsr g_R_OSAL_InterruptUnregisterIsr;
s_R_OSAL_InterruptDisableIsr g_R_OSAL_InterruptDisableIsr;
s_R_OSAL_ThsyncMutexLockForTimePeriod g_R_OSAL_ThsyncMutexLockForTimePeriod;
s_R_OSAL_ThsyncMutexCreate g_R_OSAL_ThsyncMutexCreate;
s_R_OSAL_ThsyncMutexDestroy g_R_OSAL_ThsyncMutexDestroy;
s_R_OSAL_ThsyncMutexUnlock g_R_OSAL_ThsyncMutexUnlock;
s_R_OSAL_MqCreate g_R_OSAL_MqCreate;
s_R_OSAL_MqReset g_R_OSAL_MqReset;
s_R_OSAL_MqDelete g_R_OSAL_MqDelete;
s_R_OSAL_MqSendForTimePeriod g_R_OSAL_MqSendForTimePeriod;
s_R_OSAL_MqReceiveForTimePeriod g_R_OSAL_MqReceiveForTimePeriod;
s_R_OSAL_ThreadCreate g_R_OSAL_ThreadCreate;
s_R_OSAL_ThreadJoin g_R_OSAL_ThreadJoin;
s_R_OSAL_InterruptEnableIsr g_R_OSAL_InterruptEnableIsr;
s_R_OSAL_PmGetState g_R_OSAL_PmGetState;
s_R_OSAL_PmGetPolicy g_R_OSAL_PmGetPolicy;
s_R_OSAL_IoWrite32 g_R_OSAL_IoWrite32;
s_R_OSAL_IoRead32 g_R_OSAL_IoRead32;
s_R_OSAL_PmSetRequiredState g_R_OSAL_PmSetRequiredState;
s_R_OSAL_PmGetRequiredState g_R_OSAL_PmGetRequiredState;
s_R_OSAL_InterruptRegisterIsr g_R_OSAL_InterruptRegisterIsr;
s_R_OSAL_PmSetResetState g_R_OSAL_PmSetResetState;
s_R_OSAL_ThsyncMemoryBarrier g_R_OSAL_ThsyncMemoryBarrier;
s_R_OSAL_MmngrInvalidate g_R_OSAL_MmngrInvalidate;
s_R_OSAL_MmngrFlush g_R_OSAL_MmngrFlush;
s_R_OSAL_MmngrGetHwAddr g_R_OSAL_MmngrGetHwAddr;
s_R_OSAL_MmngrGetCpuPtr g_R_OSAL_MmngrGetCpuPtr;
s_R_OSAL_MmngrGetSize g_R_OSAL_MmngrGetSize;
s_R_OSAL_PmWaitForState g_R_OSAL_PmWaitForState;
s_R_OSAL_PmGetResetState g_R_OSAL_PmGetResetState;
s_impdrv_cmnctl_get_corectl_func g_impdrv_cmnctl_get_corectl_func;
s_impdrv_cmnctl_get_init_param g_impdrv_cmnctl_get_init_param;
s_impdrv_cmnctl_get_dsp_func g_impdrv_cmnctl_get_dsp_func;
s_impdrv_genctl_chk_state_init g_impdrv_genctl_chk_state_init;

s_impdrv_cnnctl_get_inten_val g_impdrv_cnnctl_get_inten_val;
s_impdrv_pscctl_get_inten_val g_impdrv_pscctl_get_inten_val;
s_impdrv_ocvctl_get_inten_val g_impdrv_ocvctl_get_inten_val;
s_impdrv_ocvctl_set_state g_impdrv_ocvctl_set_state;
st_impdrv_corectl_func_t g_p_funcs_null = {NULL};
st_impdrv_corectl_func_t* g_p_funcs;
s_impdrv_osdep_pow_on_imp g_impdrv_osdep_pow_on_imp;
s_impdrv_genctl_dsp_int_handler g_impdrv_genctl_dsp_int_handler;
s_impdrv_genctl_init_set_state g_impdrv_genctl_init_set_state;

s_impdrv_cmnctl_set_checksum g_impdrv_cmnctl_set_checksum;
s_impdrv_cmnctl_test_checksum g_impdrv_cmnctl_test_checksum;

s_impdrv_cmnctl_calc_checksum g_impdrv_cmnctl_calc_checksum;
s_impdrv_cmnctl_cnv_int_bit_core g_impdrv_cmnctl_cnv_int_bit_core;
s_impdrv_cmnctl_cnv_int_core_bit g_impdrv_cmnctl_cnv_int_core_bit;
s_impdrv_cmnctl_get_group g_impdrv_cmnctl_get_group;
s_impdrv_cmnctl_get_group g_impdrv_cmnctl_get_group;

s_impdrv_cmnctl_reg_find_resource g_impdrv_cmnctl_reg_find_resource;

s_impdrv_cmnctl_reg_set_mem_protect g_impdrv_cmnctl_reg_set_mem_protect;

s_impdrv_cmnctl_get_io_imptop g_impdrv_cmnctl_get_io_imptop;

s_impdrv_cmnctl_judge_int g_impdrv_cmnctl_judge_int;

#if defined(RCAR_V3H2) || defined(RCAR_V3H1) || defined(RCAR_V3M2)
st_impdrv_corectl_func_t g_p_funcs_error = { impdrv_impctl_is_valid_core, impdrv_impctl_check_state, impdrv_ut_impctl_set_state, impdrv_impctl_init_start, impdrv_impctl_init_end, impdrv_impctl_start, impdrv_impctl_stop, impdrv_impctl_attr_init, impdrv_impctl_set_mem_init, impdrv_impctl_set_core_map, impdrv_impctl_set_cl, impdrv_impctl_set_irq_mask, impdrv_impctl_execute, impdrv_impctl_int_handler, impdrv_impctl_resume_exe, impdrv_impctl_pm_set_policy, impdrv_impctl_pm_get_policy, NULL, impdrv_impctl_bus_if_check, impdrv_impctl_conf_reg_check };
#else
st_impdrv_corectl_func_t g_p_funcs_error = { impdrv_impctl_is_valid_core, impdrv_impctl_check_state, impdrv_ut_impctl_set_state, impdrv_impctl_init_start, impdrv_impctl_init_end, impdrv_impctl_start, impdrv_impctl_stop, impdrv_impctl_attr_init, impdrv_impctl_set_mem_init, impdrv_impctl_set_core_map, impdrv_impctl_set_cl, impdrv_impctl_set_irq_mask, impdrv_impctl_execute, impdrv_impctl_int_handler, impdrv_impctl_resume_exe, impdrv_impctl_pm_set_policy, impdrv_impctl_pm_get_policy, NULL, NULL, impdrv_impctl_conf_reg_check };
#endif

st_impdrv_corectl_func_t g_p_funcs_dsp = 
        {  impdrv_dspctl_is_valid_core,     impdrv_dspctl_check_state,      impdrv_dspctl_set_state,
           impdrv_dspctl_init_start,        impdrv_dspctl_init_end,         impdrv_dspctl_dsp_start,
           impdrv_dspctl_stop,              impdrv_dspctl_attr_init,        impdrv_dspctl_set_mem_init,
           impdrv_dspctl_set_core_map,      NULL,                           NULL,
           impdrv_dspctl_dsp_execute,       impdrv_dspctl_int_handler,      impdrv_dspctl_resume_exe,
           impdrv_dspctl_pm_set_policy,     impdrv_dspctl_pm_get_policy,    impdrv_dspctl_set_dsp,
           NULL,                            impdrv_dspctl_conf_reg_check,   NULL,
           NULL,                            impdrv_dspctl_pm_get_state,     impdrv_dspctl_quit,
           NULL,                            stub_p_impdrvCorectlSyncStop
        };
st_impdrv_corectl_func_t g_p_funcs_dsp_start = 
        {  impdrv_dspctl_is_valid_core,     impdrv_dspctl_check_state,      impdrv_dspctl_set_state,
           impdrv_dspctl_init_start,        impdrv_dspctl_init_end,         impdrv_dspctl_dsp_start,
           impdrv_dspctl_stop,              impdrv_dspctl_attr_init,        impdrv_dspctl_set_mem_init,
           impdrv_dspctl_set_core_map,      NULL,                           NULL,
           impdrv_dspctl_dsp_execute,       impdrv_dspctl_int_handler,      impdrv_dspctl_resume_exe,
           impdrv_dspctl_pm_set_policy,     impdrv_dspctl_pm_get_policy,    impdrv_dspctl_set_dsp,
           NULL,                            impdrv_dspctl_conf_reg_check,   NULL,
           NULL,                            impdrv_dspctl_pm_get_state,     impdrv_dspctl_quit,
           stub_p_impdrvCorectlSyncStart,   stub_p_impdrvCorectlSyncStop
        };
s_impdrv_cnnctl_set_state g_impdrv_cnnctl_set_state;
s_impdrv_pscctl_set_state g_impdrv_pscctl_set_state;
s_impdrv_dmactl_get_inten_val g_impdrv_dmactl_get_inten_val;
s_impdrv_impctl_get_inten_val g_impdrv_impctl_get_inten_val;
s_impdrv_impctl_set_state g_impdrv_impctl_set_state;
s_impdrv_dspctl_set_state g_impdrv_dspctl_set_state;
s_impdrv_osdep_memory_data_set g_impdrv_osdep_memory_data_set;
s_impdrv_dmactl_set_state g_impdrv_dmactl_set_state;
s_impdrv_udefctl_udef_crc_val g_impdrv_udefctl_udef_crc;
s_impdrv_udefctl_set_ecm g_impdrv_udefctl_set_ecm;
/*Declare hook global variable*/
int32_t g_hook_R_OSAL_IoGetNumOfDevices;
int32_t ut_stub_R_OSAL_IoGetNumOfDevices;
int32_t g_hook_R_OSAL_IoGetDeviceList;
int32_t ut_stub_R_OSAL_IoGetDeviceList;
int32_t g_hook_R_OSAL_IoDeviceOpen;
int32_t ut_stub_R_OSAL_IoDeviceOpen;
int32_t g_hook_R_OSAL_IoDeviceClose;
int32_t ut_stub_R_OSAL_IoDeviceClose;
int32_t g_hook_R_OSAL_PmSetPolicy;
int32_t ut_stub_R_OSAL_PmSetPolicy;
int32_t g_hook_R_OSAL_InterruptGetNumOfIrqChannels;
int32_t ut_stub_R_OSAL_InterruptGetNumOfIrqChannels;
int32_t g_hook_R_OSAL_InterruptUnregisterIsr;
int32_t ut_stub_R_OSAL_InterruptUnregisterIsr;
int32_t g_hook_R_OSAL_InterruptDisableIsr;
int32_t ut_stub_R_OSAL_InterruptDisableIsr;
int32_t g_hook_R_OSAL_ThsyncMutexLockForTimePeriod;
int32_t ut_stub_R_OSAL_ThsyncMutexLockForTimePeriod;
int32_t g_hook_R_OSAL_ThsyncMutexCreate;
int32_t ut_stub_R_OSAL_ThsyncMutexCreate;
int32_t g_hook_R_OSAL_ThsyncMutexDestroy;
int32_t ut_stub_R_OSAL_ThsyncMutexDestroy;
int32_t g_hook_R_OSAL_ThsyncMutexUnlock;
int32_t ut_stub_R_OSAL_ThsyncMutexUnlock;
int32_t g_hook_R_OSAL_MqCreate;
int32_t ut_stub_R_OSAL_MqCreate;
int32_t g_hook_R_OSAL_MqReset;
int32_t ut_stub_R_OSAL_MqReset;
int32_t g_hook_R_OSAL_MqDelete;
int32_t ut_stub_R_OSAL_MqDelete;
int32_t g_hook_R_OSAL_MqSendForTimePeriod;
int32_t ut_stub_R_OSAL_MqSendForTimePeriod;
int32_t g_hook_R_OSAL_MqReceiveForTimePeriod;
int32_t ut_stub_R_OSAL_MqReceiveForTimePeriod;
int32_t g_hook_R_OSAL_ThreadCreate;
int32_t ut_stub_R_OSAL_ThreadCreate;
int32_t g_hook_R_OSAL_ThreadJoin;
int32_t ut_stub_R_OSAL_ThreadJoin;
int32_t g_hook_R_OSAL_InterruptRegisterIsr;
int32_t ut_stub_R_OSAL_InterruptRegisterIsr;
int32_t g_hook_R_OSAL_InterruptEnableIsr;
int32_t ut_stub_R_OSAL_InterruptEnableIsr;
int32_t g_hook_R_OSAL_PmGetState;
int32_t ut_stub_R_OSAL_PmGetState;
int32_t g_hook_R_OSAL_IoWrite32;
int32_t ut_stub_R_OSAL_IoWrite32;
int32_t g_hook_R_OSAL_IoRead32;
int32_t ut_stub_R_OSAL_IoRead32;
int32_t g_hook_R_OSAL_PmSetRequiredState;
int32_t ut_stub_R_OSAL_PmSetRequiredState;
int32_t g_hook_R_OSAL_PmGetRequiredState;
int32_t ut_stub_R_OSAL_PmGetRequiredState;
int32_t g_hook_R_OSAL_PmGetPolicy;
int32_t ut_stub_R_OSAL_PmGetPolicy;
int32_t g_hook_R_OSAL_MmngrInvalidate;
int32_t ut_stub_R_OSAL_MmngrInvalidate;
int32_t g_hook_R_OSAL_MmngrFlush;
int32_t ut_stub_R_OSAL_MmngrFlush;
int32_t g_hook_R_OSAL_MmngrGetHwAddr;
int32_t ut_stub_R_OSAL_MmngrGetHwAddr;
int32_t g_hook_R_OSAL_MmngrGetCpuPtr;
int32_t ut_stub_R_OSAL_MmngrGetCpuPtr;
int32_t g_hook_R_OSAL_MmngrGetSize;
int32_t ut_stub_R_OSAL_MmngrGetSize;
int32_t g_hook_R_OSAL_ThsyncMemoryBarrier;
int32_t ut_stub_R_OSAL_ThsyncMemoryBarrier;
int32_t g_hook_R_OSAL_PmSetResetState;
int32_t ut_stub_R_OSAL_PmSetResetState;
int32_t g_hook_R_OSAL_PmWaitForState;
int32_t ut_stub_R_OSAL_PmWaitForState;
int32_t g_hook_R_OSAL_PmGetResetState;
int32_t ut_stub_R_OSAL_PmGetResetState;
uint32_t g_p_logical_adrs[128];
uint32_t g_uts_p_receive_buffer;
uint32_t g_uts_device_io_imptop;
uint32_t g_uts_p_work_size;
char g_uts_name[32];
st_impdrv_commonctl_t g_uts_p_commonctl;
st_impdrv_device_handle_t   device_io_imptop;
st_impdrv_hwrsc_def_t p_resource_null = {NULL};
p_impdrv_cmnctl_get_corectl_func_stub g_hook_impdrv_cmnctl_get_corectl_func;
p_impdrv_cmnctl_get_init_param_stub g_hook_impdrv_cmnctl_get_init_param;
p_impdrv_cmnctl_get_dsp_func_stub g_hook_impdrv_cmnctl_get_dsp_func;
p_impdrv_genctl_chk_state_init_stub g_hook_impdrv_genctl_chk_state_init;
const st_impdrv_corectl_func_t *ut_stub_impdrv_cmnctl_get_corectl_func(const e_impdrv_core_type_t core_type);
const uint32_t ut_stub_impdrv_cnnctl_get_inten_val(const bool irq_mask[IMPDRV_IRQMASK_MAX]);
p_impdrv_cnnctl_get_inten_val g_hook_impdrv_cnnctl_get_inten_val;
const uint32_t ut_stub_impdrv_pscctl_get_inten_val(const bool irq_mask[IMPDRV_IRQMASK_MAX]);
p_impdrv_pscctl_get_inten_val g_hook_impdrv_pscctl_get_inten_val;
p_impdrv_ocvctl_get_inten_val g_hook_impdrv_ocvctl_get_inten_val;
uint32_t crc_sub[256];
const e_impdrv_errorcode_t ut_stub_impdrv_cnnctl_check_inten(
    const uint32_t          stat_val,
    uint32_t                *const p_mask_val,
    e_impdrv_cb_ret_t       cb_ercd[IMPDRV_CB_RET_MAX],
    uint32_t                *const p_cb_ercd_num,
    bool                    *const p_is_check_intclear,
    e_impdrv_state_t        *const p_state
);
p_impdrv_cnnctl_check_inten g_hook_impdrv_cnnctl_check_inten;
//#if defined(RCAR_V3H2)

p_impdrv_dmasctl_get_inten_val g_hook_impdrv_dmasctl_get_inten_val;
const uint32_t ut_stub_impdrv_dmasctl_get_inten_val(const bool irq_mask[IMPDRV_IRQMASK_MAX]);
s_impdrv_dmasctl_get_inten_val g_impdrv_dmasctl_get_inten_val;

s_impdrv_dmasctl_set_state g_impdrv_dmasctl_set_state;
const e_impdrv_errorcode_t ut_stub_impdrv_dmasctl_set_state(st_impdrv_corectl_t *const p_corectl,const e_impdrv_state_t  state);
p_impdrv_dmasctl_set_state g_hook_impdrv_dmasctl_set_state;

s_impdrv_dmasctl_osal_core_num g_impdrv_dmasctl_osal_core_num;
const uint32_t ut_stub_impdrv_dmasctl_osal_core_num(const uint32_t              core_num);
p_impdrv_dmasctl_osal_core_num g_hook_impdrv_dmasctl_osal_core_num;

s_impdrv_dmactl_osal_core_num g_impdrv_dmactl_osal_core_num;
const uint32_t ut_stub_impdrv_dmactl_osal_core_num(const uint32_t              core_num);
p_impdrv_dmactl_osal_core_num g_hook_impdrv_dmactl_osal_core_num;

p_impdrv_osdep_get_dsp_irq_channel g_hook_impdrv_osdep_get_dsp_irq_channel;
s_impdrv_osdep_get_dsp_irq_channel g_impdrv_osdep_get_dsp_irq_channel;

//#endif

//#if defined(RCAR_V3H2) || defined(RCAR_V3H1)
const uint32_t ut_stub_impdrv_impsctl_get_inten_val(const bool irq_mask[IMPDRV_IRQMASK_MAX]);
s_impdrv_impsctl_get_inten_val g_impdrv_impsctl_get_inten_val;
p_impdrv_impsctl_get_inten_val g_hook_impdrv_impsctl_get_inten_val;

s_impdrv_impsctl_set_state g_impdrv_impsctl_set_state;
const e_impdrv_errorcode_t ut_stub_impdrv_impsctl_set_state(st_impdrv_corectl_t *const p_corectl,const e_impdrv_state_t  state);
p_impdrv_impsctl_set_state g_hook_impdrv_impsctl_set_state;

//#endif

p_impdrv_cmnctl_reg_find_resource_stub g_hook_impdrv_cmnctl_reg_find_resource;
e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_reg_find_resource(const char *const p_device_id, uint32_t *const p_index, st_impdrv_hwrsc_def_t *const p_resource);
p_impdrv_cmnctl_get_io_imptop_stub g_hook_impdrv_cmnctl_get_io_imptop;
e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_get_io_imptop(st_impdrv_device_handle_t *const p_handle);
const e_impdrv_errorcode_t ut_stub_impdrv_ocvctl_set_state(st_impdrv_corectl_t *const p_corectl,const e_impdrv_state_t  state);
p_impdrv_ocvctl_set_state g_hook_impdrv_ocvctl_set_state;

p_impdrv_genctl_init_set_state_stub g_hook_impdrv_genctl_init_set_state;
e_impdrv_errorcode_t ut_stub_impdrv_genctl_init_set_state(st_impdrv_ctl_t *const p_impdrv_ctl, const uint32_t use_core_num, const st_impdrv_core_info_t *const p_core_info, const st_impdrv_corectl_func_t **const p_funcs);
const e_impdrv_errorcode_t ut_stub_impdrv_osdep_pow_on_imp(st_impdrv_device_handle_t   *const p_device_handle,const st_impdrv_core_info_t core_info);
p_impdrv_osdep_pow_on_imp g_hook_impdrv_osdep_pow_on_imp;

const e_impdrv_errorcode_t ut_stub_impdrv_genctl_dsp_int_handler(st_impdrv_ctl_t *const p_impdrv_ctl, const uint64_t irq_channel, const e_impdrv_errorcode_t ercd, const st_impdrv_core_info_t core_info);
p_impdrv_genctl_dsp_int_handle g_hook_impdrv_genctl_dsp_int_handler;

p_impdrv_cmnctl_set_checksum_stub g_hook_impdrv_cmnctl_set_checksum;
e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_set_checksum(void);

p_impdrv_cmnctl_test_checksum_stub g_hook_impdrv_cmnctl_test_checksum;
e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_test_checksum(void);

p_impdrv_cmnctl_cnv_int_bit_core_stub g_hook_impdrv_cmnctl_cnv_int_bit_core;
e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_cnv_int_bit_core(const st_impdrv_inten_tbl_t *const p_s_inten_cnv_table, const uint32_t cnv_table_num, const uint32_t stat, st_impdrv_core_info_t *core_info, uint32_t *const p_use_core_num);

p_impdrv_cmnctl_cnv_int_core_bit_stub g_hook_impdrv_cmnctl_cnv_int_core_bit;
e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_cnv_int_core_bit(const st_impdrv_core_info_t *const p_core_info, const uint32_t use_core_num, const st_impdrv_inten_tbl_t const* p_inten_cnv_table, const uint32_t cnv_table_num, uint32_t *const p_val);

p_impdrv_cmnctl_get_group_stub g_hook_impdrv_cmnctl_get_group;
e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_get_group(e_impdrv_irq_group_t *const p_irq_group, const e_impdrv_instance_t instance_num);

p_impdrv_cmnctl_calc_checksum_stub g_hook_impdrv_cmnctl_calc_checksum; 
e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_calc_checksum(const void* *const p_data,const size_t size,uint32_t *const p_checksum);

const e_impdrv_errorcode_t ut_stub_impdrv_cnnctl_set_state(st_impdrv_corectl_t *const p_corectl,const e_impdrv_state_t  state);
p_impdrv_cnnctl_set_state g_hook_impdrv_cnnctl_set_state;

const e_impdrv_errorcode_t ut_stub_impdrv_pscctl_set_state(st_impdrv_corectl_t *const p_corectl,const e_impdrv_state_t  state);
p_impdrv_pscctl_set_state g_hook_impdrv_pscctl_set_state;

const uint32_t ut_stub_impdrv_dmactl_get_inten_val(const bool irq_mask[IMPDRV_IRQMASK_MAX]);
p_impdrv_dmactl_get_inten_val g_hook_impdrv_dmactl_get_inten_val;
const uint32_t ut_stub_impdrv_impctl_get_inten_val(const bool irq_mask[IMPDRV_IRQMASK_MAX]);
p_impdrv_impctl_get_inten_val g_hook_impdrv_impctl_get_inten_val;
const e_impdrv_errorcode_t ut_stub_impdrv_impctl_set_state(st_impdrv_corectl_t *const p_corectl,const e_impdrv_state_t  state);
p_impdrv_impctl_set_state g_hook_impdrv_impctl_set_state;
const e_impdrv_errorcode_t ut_stub_impdrv_dspctl_set_state(st_impdrv_corectl_t *const p_corectl,const e_impdrv_state_t  state);
p_impdrv_dspctl_set_state g_hook_impdrv_dspctl_set_state;
const e_impdrv_errorcode_t ut_stub_impdrv_osdep_memory_data_set(uint32_t *const p_buffer,const uint32_t  data,const size_t size);
p_impdrv_osdep_memory_data_set g_hook_impdrv_osdep_memory_data_set;
const e_impdrv_errorcode_t ut_stub_impdrv_dmactl_set_state(st_impdrv_corectl_t *const p_corectl,const e_impdrv_state_t  state);
p_impdrv_dmactl_set_state g_hook_impdrv_dmactl_set_state;

e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_reg_set_mem_protect(st_impdrv_commonctl_t *const p_commonctl, const e_impdrv_protect_mode_t protect_mode);
p_impdrv_cmnctl_reg_set_mem_protect g_hook_impdrv_cmnctl_reg_set_mem_protect;

p_impdrv_cnnctl_check_inten g_hook_impdrv_cnnctl_check_inten;
s_impdrv_cnnctl_check_inten g_impdrv_cnnctl_check_inten;
e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_judge_int_stub(const st_impdrv_commonctl_t *const p_commonctl,const uint64_t irq_channel,st_impdrv_core_info_t core_info[IMPDRV_INTSTS_BITNUM], uint32_t *const p_use_core_num, bool is_fb_callback);
p_impdrv_cmnctl_judge_int_stub g_hook_impdrv_cmnctl_judge_int;


uint32_t impdrv_udefctl_udef_crc_stub(uint32_t st, uint32_t sz );
uint32_t ut_stub_impdrv_udefctl_udef_crc(uint32_t st, uint32_t sz );

void ut_stub_impdrv_udefctl_udef_ecm(int32_t core_num,bool flag );
void impdrv_udefctl_set_ecm_stub(int32_t core_num,bool flag);

p_impdrv_udefctl_udef_crc_val g_hook_impdrv_udefctl_udef_crc;
p_impdrv_udefctl_set_ecm g_hook_impdrv_udefctl_set_ecm;
/*Global variable using in stub*/
static bool osal_init = false;
st_impdrv_ctl_t g_uts_handle;
st_impdrv_ctl_t g_p_uts_p_handle;
st_impdrv_ctl_t g_uts_p_impdrv_ctl;
st_impdrv_ctl_t *g_p_uts_p_impdrv_ctl;
bool g_uts_verify_flag;
int32_t g_work_addr[0x20000];
int32_t g_uts_msg_data;
uint32_t g_uts_value;
osal_device_handle_t g_uts_device_handle;
uint32_t g_uts_state;
int32_t g_uts_end_ch;
int32_t g_uts_start_ch;
int32_t g_uts_addr;
int32_t g_uts_exe_param;
int32_t g_uts_p_osdep_work;
int32_t g_uts_task_handle;
int32_t g_uts_msg_handle;
st_impdrv_ctl_t g_p_main_work_addr __attribute__((aligned (8)));
st_impdrv_ctl_t g_p_main_work_addr_invalid __attribute__((aligned (4)));
int32_t g_p_attr_work_addr __attribute__((aligned (8)));
int32_t g_p_queue_work_addr __attribute__((aligned (8)));
int32_t g_uts_func;
int32_t g_uts_userarg;
int32_t g_uts_task_name;
int32_t g_uts_p_msg;
int32_t g_uts_callback_func;
int32_t g_uts_osal_cb_args;
int32_t g_uts_p_val;
int32_t g_uts_s_inten_cnv_table;
int32_t g_uts_cbfunc_fatal;
int32_t g_uts_p_cb_args_fatal;
int32_t g_uts_p_device_handle;
int32_t g_uts_p_callback_args;
int32_t g_p_handle;
int32_t g_p_uts_p_osal_cb_args;
int32_t g_uts_mutex_handle = TEST_ADDR_NOT_NULL;
int32_t g_uts_dev_handle = TEST_ADDR_NOT_NULL;
int32_t g_uts_policy;
int32_t g_uts_p_pmstate;
int32_t g_uts_p_use_core_num;
int32_t g_uts_p_init_core_num;
int32_t g_uts_p_inten_group_table;
uint8_t g_uts_syncc_val[16];
int32_t g_uts_irq_channel;
st_impdrv_ctl_t g_uts_irq_arg;
int32_t g_uts_p_pm_policy;
int32_t g_uts_p_hwrsc_def_table;
int32_t g_uts_p_hwrsc_mng_table;
int32_t g_uts_core_info;
st_impdrv_coretypectl_t g_uts_p_coretypectl;
int32_t g_uts_p_policy;
bool g_uts_p_is_dmac_mb_init;
int32_t g_uts_p_data;
char g_uts_p_device_id;
bool g_uts_p_is_dmac_mb_init;
char g_uts_p_rsc_name;
char g_uts_p_reg_info;
char g_uts_p_index;
char g_uts_p_resource;
st_impdrv_corectl_t g_uts_p_corectl;
char g_uts_irq_mask;
e_impdrv_irq_group_t g_uts_p_irq_group;
e_osal_interrupt_priority_t g_uts_p_irq_priority;
st_impdrv_device_handle_t g_uts_p_handle;
st_impdrv_dsp_data_t g_uts_p_dsp_app;
st_impdrv_dsp_data_t g_uts_p_dsp_fw;
st_impdrv_core_info_t g_uts_p_core_info;
st_impdrv_core_info_t g_uts_init_core_info[22];
st_impdrv_dsp_data_t g_uts_p_dsp_data;
st_impdrv_dsp_data_t g_uts_p_dsp_dtcm;
uint32_t g_uts_p_use_core_info;
uint32_t g_uts_p_mask_val;
e_impdrv_cb_ret_t g_uts_cb_ercd[18];
bool g_uts_p_is_check_intclear;
uint32_t g_uts_p_cb_ercd_num;
e_impdrv_state_t g_uts_p_state;
uintptr_t g_uts_p_physical_adrs;
uint32_t g_uts_constp_cb_ercd_num;
bool g_uts_constp_is_check_intclear;
e_impdrv_state_t g_uts_constp_state;
uint32_t g_uts_p_checksum;
uint32_t g_uts_constp_mask_val;
uint32_t g_p_uts_p_func_array;
uint32_t st;
uint32_t sz;

st_impdrv_chk_resource_t g_uts_p_chk_resource;
st_impdrv_ctl_t g_uts_R_OSAL_IoRead32;
st_impdrv_ctl_t g_uts_R_OSAL_IoWrite32;
st_impdrv_chk_param_t g_uts_p_chk_param;
st_impdrv_corectl_func_t g_p_p_funcs_null = {1, NULL,NULL};
st_impdrv_corectl_func_t g_p_p_funcs_normal = {1, 1,1};
st_impdrv_corectl_func_t g_p_p_funcs_core_null = {NULL, 1,1};
st_impdrv_corectl_func_t g_p_p_funcs_core_invalid = {0, NULL,NULL};
st_impdrv_corectl_func_t g_p_p_f_funcs_null = {1, 1, NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
st_impdrv_corectl_func_t *g_p_p_f_funcs_null_v2 = {NULL, 1, NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
st_impdrv_corectl_func_t g_p_p_end_null = {1, 1,1,1, NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
st_impdrv_corectl_func_t g_p_p_state_null = {1, 1,1,1, 1,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
st_impdrv_corectl_func_t g_p_p_attr_null = {1, 1, NULL,1,1,NULL,NULL,NULL,NULL,NULL,NULL};
st_impdrv_corectl_func_t *g_p_p_funcs;
st_impdrv_corectl_func_t *g_uts_p_funcs[22];
void* g_p_uts_p_funcs;
st_impdrv_ctl_t *g_p_ut_p_impdrv_ctl;
st_impdrv_device_handle_t g_uts_p_device_io[8];
st_impdrv_device_handle_t *g_p_uts_p_device_io[8];
int32_t g_uts_device_io;
st_impdrv_corectl_func_t g_p_core_init_end =         {   impdrv_pscctl_is_valid_core,    impdrv_pscctl_check_state,      impdrv_pscctl_set_state,
            impdrv_pscctl_init_start,       NULL,         impdrv_pscctl_start,
            impdrv_pscctl_stop,             impdrv_pscctl_attr_init,        impdrv_pscctl_set_mem_init,
            impdrv_pscctl_set_core_map,     impdrv_pscctl_set_cl,           impdrv_pscctl_set_irq_mask,
            impdrv_pscctl_execute,          impdrv_pscctl_int_handler,      impdrv_pscctl_resume_exe,
            impdrv_pscctl_pm_set_policy,    impdrv_pscctl_pm_get_policy,    NULL,
            NULL,                           impdrv_pscctl_conf_reg_check
        };

size_t g_uts_constp_size;
uint32_t index;
e_impdrv_state_t state;
bool null_flag;


uint32_t g_p_uts_p_logical_adrs;

void ut_init_v3u(bool enabled_real)
{
    ut_init_imp(enabled_real);
}

void ut_init_v3h2(bool enabled_real)
{
    ut_init_imp(enabled_real);
}

void ut_init_v3h1(bool enabled_real)
{
    ut_init_imp(enabled_real);
}

void ut_init_v3m2(bool enabled_real)
{
    ut_init_imp(enabled_real);
}

void ut_deinit_v3u(void)
{
    ut_deinit_imp();
}

void ut_deinit_v3h2(void)
{
    ut_deinit_imp();
}

void ut_deinit_v3h1(void)
{
    ut_deinit_imp();
}

void ut_deinit_v3m2(void)
{
    ut_deinit_imp();
}

/*imp INIT function*/
void ut_init_imp(bool enabled_real)
{
    uint32_t i;
    g_p_uts_p_funcs = g_uts_p_funcs;
    g_p_uts_p_impdrv_ctl = (st_impdrv_ctl_t*)malloc(sizeof(st_impdrv_ctl_t));
    memset(&g_hwrsc_chk_table, 0, sizeof(g_hwrsc_chk_table));
    memset(&g_hwrsc_mng_table, 0, sizeof(g_hwrsc_mng_table));

    memset(&g_R_OSAL_IoGetNumOfDevices, 0, sizeof(s_R_OSAL_IoGetNumOfDevices));
    memset(&g_R_OSAL_IoGetDeviceList, 0, sizeof(s_R_OSAL_IoGetDeviceList));
    memset(&g_R_OSAL_IoDeviceOpen, 0, sizeof(s_R_OSAL_IoDeviceOpen));
    memset(&g_R_OSAL_IoDeviceClose, 0, sizeof(s_R_OSAL_IoDeviceClose));
    memset(&g_R_OSAL_PmSetPolicy, 0, sizeof(s_R_OSAL_PmSetPolicy));
    memset(&g_R_OSAL_InterruptGetNumOfIrqChannels, 0, sizeof(s_R_OSAL_InterruptGetNumOfIrqChannels));
    memset(&g_R_OSAL_InterruptUnregisterIsr, 0, sizeof(s_R_OSAL_InterruptUnregisterIsr));
    memset(&g_R_OSAL_InterruptDisableIsr, 0, sizeof(s_R_OSAL_InterruptDisableIsr));
    memset(&g_R_OSAL_ThsyncMutexLockForTimePeriod, 0, sizeof(s_R_OSAL_ThsyncMutexLockForTimePeriod));
    memset(&g_R_OSAL_ThsyncMutexCreate, 0, sizeof(s_R_OSAL_ThsyncMutexCreate));
    memset(&g_R_OSAL_ThsyncMutexDestroy, 0, sizeof(s_R_OSAL_ThsyncMutexDestroy));
    memset(&g_R_OSAL_ThsyncMutexUnlock, 0, sizeof(s_R_OSAL_ThsyncMutexUnlock));
    memset(&g_R_OSAL_MqCreate, 0, sizeof(s_R_OSAL_MqCreate));
    memset(&g_R_OSAL_MqReset, 0, sizeof(s_R_OSAL_MqReset));
    memset(&g_R_OSAL_MqDelete, 0, sizeof(s_R_OSAL_MqDelete));
    memset(&g_R_OSAL_MqSendForTimePeriod, 0, sizeof(s_R_OSAL_MqSendForTimePeriod));
    memset(&g_R_OSAL_MqReceiveForTimePeriod, 0, sizeof(s_R_OSAL_MqReceiveForTimePeriod));
    memset(&g_R_OSAL_ThreadCreate, 0, sizeof(s_R_OSAL_ThreadCreate));
    memset(&g_R_OSAL_ThreadJoin, 0, sizeof(s_R_OSAL_ThreadJoin));
    memset(&g_R_OSAL_InterruptRegisterIsr, 0, sizeof(s_R_OSAL_InterruptRegisterIsr));
    memset(&g_R_OSAL_InterruptEnableIsr, 0, sizeof(s_R_OSAL_InterruptEnableIsr));
    memset(&g_R_OSAL_PmGetState, 0, sizeof(s_R_OSAL_PmGetState));
    memset(&g_R_OSAL_PmGetPolicy, 0, sizeof(s_R_OSAL_PmGetPolicy));
    memset(&g_R_OSAL_IoWrite32, 0, sizeof(s_R_OSAL_IoWrite32));
    memset(&g_R_OSAL_IoRead32, 0, sizeof(s_R_OSAL_IoRead32));
    memset(&g_R_OSAL_PmSetRequiredState, 0, sizeof(s_R_OSAL_PmSetRequiredState));
    memset(&g_R_OSAL_PmGetRequiredState, 0, sizeof(s_R_OSAL_PmGetRequiredState));
    memset(&g_R_OSAL_PmSetResetState, 0, sizeof(s_R_OSAL_PmSetResetState));
    memset(&g_R_OSAL_ThsyncMemoryBarrier, 0, sizeof(s_R_OSAL_ThsyncMemoryBarrier));
    memset(&g_R_OSAL_MmngrInvalidate, 0, sizeof(s_R_OSAL_MmngrInvalidate));
    memset(&g_R_OSAL_MmngrFlush, 0, sizeof(s_R_OSAL_MmngrFlush));
    memset(&g_R_OSAL_MmngrGetHwAddr, 0, sizeof(s_R_OSAL_MmngrGetHwAddr));
    memset(&g_R_OSAL_MmngrGetCpuPtr, 0, sizeof(s_R_OSAL_MmngrGetCpuPtr));
    memset(&g_R_OSAL_MmngrGetSize, 0, sizeof(s_R_OSAL_MmngrGetSize));
    memset(&g_R_OSAL_PmWaitForState, 0, sizeof(s_R_OSAL_PmWaitForState));
    memset(&g_R_OSAL_PmGetResetState, 0, sizeof(s_R_OSAL_PmGetResetState));

    memset(&g_p_main_work_addr, 0, sizeof(st_impdrv_ctl_t));
    memset(&g_uts_handle, 0, sizeof(st_impdrv_ctl_t));
    memset(&g_p_uts_p_handle, 0, sizeof(st_impdrv_ctl_t));
    memset(&g_uts_p_handle, 0, sizeof(st_impdrv_ctl_t));
    memset(&g_uts_p_impdrv_ctl, 0, sizeof(st_impdrv_ctl_t));
    memset(&g_uts_policy, 0, sizeof(int32_t));
    memset(&g_uts_p_pmstate, 0, sizeof(int32_t));
    memset(&g_impdrv_cmn_ctl_chk, 0, sizeof(st_impdrv_commonctl_chk_t));
    memset(&g_impdrv_cmn_ctl_ins, 0, sizeof(st_impdrv_commonctl_ins_t));
    g_hook_R_OSAL_IoGetNumOfDevices = NULL;
    g_hook_R_OSAL_IoGetDeviceList = NULL;
    g_hook_R_OSAL_IoDeviceOpen = NULL;
    g_hook_R_OSAL_IoDeviceClose = NULL;
 	g_hook_R_OSAL_PmSetPolicy = NULL;
    g_hook_R_OSAL_InterruptGetNumOfIrqChannels = NULL;
    g_hook_R_OSAL_InterruptUnregisterIsr = NULL;
    g_hook_R_OSAL_InterruptDisableIsr = NULL;
    g_hook_R_OSAL_ThsyncMutexLockForTimePeriod = NULL;
    g_hook_R_OSAL_ThsyncMutexCreate = NULL;
    g_hook_R_OSAL_ThsyncMutexDestroy = NULL;
    g_hook_R_OSAL_ThsyncMutexUnlock = NULL;
    g_hook_R_OSAL_MqCreate = NULL;
    g_hook_R_OSAL_MqReset = NULL;
    g_hook_R_OSAL_MqDelete = NULL;
    g_hook_R_OSAL_MqSendForTimePeriod = NULL;
    g_hook_R_OSAL_MqReceiveForTimePeriod = NULL;
    g_hook_R_OSAL_ThreadCreate = NULL;
    g_hook_R_OSAL_ThreadJoin = NULL;
    g_hook_R_OSAL_PmWaitForState = NULL;
    g_hook_R_OSAL_PmGetResetState = NULL;

	g_hook_impdrv_cmnctl_get_corectl_func = NULL;
	memset (&g_impdrv_cmnctl_get_corectl_func, 0, sizeof(g_impdrv_cmnctl_get_corectl_func));
	g_hook_impdrv_cmnctl_get_init_param = NULL;
	memset (&g_impdrv_cmnctl_get_init_param, 0, sizeof(g_impdrv_cmnctl_get_init_param));
	g_hook_impdrv_cmnctl_get_dsp_func = NULL;
	memset (&g_impdrv_cmnctl_get_dsp_func, 0, sizeof(g_impdrv_cmnctl_get_dsp_func));
	g_hook_impdrv_genctl_chk_state_init = NULL;
	memset (&g_impdrv_genctl_chk_state_init, 0, sizeof(g_impdrv_genctl_chk_state_init));
	g_hook_impdrv_genctl_init_set_state = NULL;
	memset (&g_impdrv_genctl_init_set_state, 0, sizeof(g_impdrv_genctl_init_set_state));
	g_hook_impdrv_cmnctl_reg_find_resource = NULL;
    memset(&g_hook_impdrv_cmnctl_reg_find_resource, 0, sizeof(g_hook_impdrv_cmnctl_reg_find_resource));
	g_hook_impdrv_cmnctl_get_io_imptop = NULL; 
    memset(&g_hook_impdrv_cmnctl_get_io_imptop, 0, sizeof(g_hook_impdrv_cmnctl_get_io_imptop)); 

    g_hook_impdrv_cmnctl_set_checksum = NULL;
    memset(&g_hook_impdrv_cmnctl_set_checksum, 0, sizeof(g_hook_impdrv_cmnctl_set_checksum));

    g_hook_impdrv_cmnctl_test_checksum = NULL;
    memset(&g_hook_impdrv_cmnctl_test_checksum, 0, sizeof(g_hook_impdrv_cmnctl_test_checksum));

    g_hook_impdrv_cmnctl_cnv_int_bit_core = NULL;
    memset(&g_hook_impdrv_cmnctl_cnv_int_bit_core, 0, sizeof(g_hook_impdrv_cmnctl_cnv_int_bit_core));

    g_hook_impdrv_cmnctl_cnv_int_core_bit = NULL;
    memset(&g_hook_impdrv_cmnctl_cnv_int_core_bit, 0, sizeof(g_hook_impdrv_cmnctl_cnv_int_core_bit));

    g_hook_impdrv_cmnctl_get_group = NULL;
    memset(&g_hook_impdrv_cmnctl_get_group, 0, sizeof(g_hook_impdrv_cmnctl_get_group));

	g_hook_impdrv_cmnctl_calc_checksum = NULL; 
    memset(&g_hook_impdrv_cmnctl_calc_checksum, 0, sizeof(g_hook_impdrv_cmnctl_calc_checksum)); 
    
    memset(&g_impdrv_cmnctl_reg_find_resource, 0, sizeof(g_impdrv_cmnctl_reg_find_resource)); 
    /* Initialize value*/
    g_uts_msg_data    = 0xCCCCCCCC;
    g_uts_verify_flag = false;
    g_uts_state       = 0xCCCCCCCC;
    g_uts_end_ch      = 0xCCCCCCCC;
    g_uts_start_ch    = 0xCCCCCCCC;


    memset (&g_impdrv_cmnctl_set_checksum, 0, sizeof(g_impdrv_cmnctl_set_checksum));
    memset (&g_impdrv_cmnctl_calc_checksum, 0, sizeof(g_impdrv_cmnctl_calc_checksum));
    memset (&g_impdrv_cmnctl_test_checksum, 0, sizeof(g_impdrv_cmnctl_test_checksum));
 
	g_hook_impdrv_cnnctl_get_inten_val = NULL;
    memset (&g_impdrv_cnnctl_get_inten_val, 0, sizeof(g_impdrv_cnnctl_get_inten_val));
	
    memset (&g_impdrv_cmnctl_cnv_int_bit_core, 0, sizeof(g_impdrv_cmnctl_cnv_int_bit_core));
    memset (&g_impdrv_cmnctl_cnv_int_core_bit, 0, sizeof(g_impdrv_cmnctl_cnv_int_core_bit));
    g_hook_impdrv_ocvctl_get_inten_val = NULL;
    memset (&g_impdrv_ocvctl_get_inten_val, 0, sizeof(g_impdrv_ocvctl_get_inten_val));
    //#if defined(RCAR_V3H2) || defined(RCAR_V3H1)
    g_hook_impdrv_impsctl_get_inten_val = NULL;
    memset (&g_impdrv_impsctl_get_inten_val, 0, sizeof(g_impdrv_impsctl_get_inten_val));
    g_hook_impdrv_impsctl_set_state = NULL;
    memset (&g_impdrv_impsctl_set_state, 0, sizeof(g_impdrv_impsctl_set_state));
    //#endif
    g_hook_impdrv_ocvctl_set_state = NULL;
    memset (&g_impdrv_ocvctl_set_state, 0, sizeof(g_impdrv_ocvctl_set_state));

	g_hook_impdrv_osdep_pow_on_imp = NULL;
    memset (&g_impdrv_osdep_pow_on_imp, 0, sizeof(g_impdrv_osdep_pow_on_imp));
	g_hook_impdrv_genctl_dsp_int_handler = NULL;
    memset (&g_impdrv_genctl_dsp_int_handler, 0, sizeof(g_impdrv_genctl_dsp_int_handler));
    g_hook_impdrv_pscctl_get_inten_val = NULL;
    memset (&g_impdrv_pscctl_get_inten_val, 0, sizeof(g_impdrv_pscctl_get_inten_val));

    //#if defined (RCAR_V3H2)

    g_hook_impdrv_dmasctl_get_inten_val = NULL;
    memset (&g_impdrv_dmasctl_get_inten_val, 0, sizeof(g_impdrv_dmasctl_get_inten_val));

    g_hook_impdrv_dmasctl_set_state = NULL;
    memset (&g_impdrv_dmasctl_set_state, 0, sizeof(g_impdrv_dmasctl_set_state));
    g_hook_impdrv_dmasctl_osal_core_num = NULL;
    memset (&g_impdrv_dmasctl_osal_core_num, 0, sizeof(g_impdrv_dmasctl_osal_core_num));
    g_hook_impdrv_dmactl_osal_core_num = NULL;
    memset (&g_impdrv_dmactl_osal_core_num, 0, sizeof(g_impdrv_dmactl_osal_core_num));    
    //#endif
	g_hook_impdrv_cnnctl_set_state = NULL;
    memset (&g_impdrv_cnnctl_set_state, 0, sizeof(g_impdrv_cnnctl_set_state));

    g_hook_impdrv_pscctl_set_state = NULL;
    memset (&g_impdrv_pscctl_set_state, 0, sizeof(g_impdrv_pscctl_set_state));
    
    g_hook_impdrv_dmactl_get_inten_val = NULL;
    memset (&g_impdrv_dmactl_get_inten_val, 0, sizeof(g_impdrv_dmactl_get_inten_val));
    
    g_hook_impdrv_impctl_get_inten_val = NULL;
    memset (&g_impdrv_impctl_get_inten_val, 0, sizeof(g_impdrv_impctl_get_inten_val));
    
    g_hook_impdrv_impctl_set_state = NULL;
    memset (&g_impdrv_impctl_set_state, 0, sizeof(g_impdrv_impctl_set_state));

    g_hook_impdrv_dspctl_set_state = NULL;
    memset (&g_impdrv_dspctl_set_state, 0, sizeof(g_impdrv_dspctl_set_state));
    
	g_hook_impdrv_osdep_memory_data_set = NULL;
    memset (&g_impdrv_osdep_memory_data_set, 0, sizeof(g_impdrv_osdep_memory_data_set));
    
    g_hook_impdrv_dmactl_set_state = NULL;
    memset (&g_impdrv_dmactl_set_state, 0, sizeof(g_impdrv_dmactl_set_state));

    g_hook_impdrv_cmnctl_reg_set_mem_protect = NULL;
    memset (&g_impdrv_cmnctl_reg_set_mem_protect, 0, sizeof(g_impdrv_cmnctl_reg_set_mem_protect));
    
    g_hook_impdrv_cmnctl_judge_int = NULL;
    memset (&g_impdrv_cmnctl_judge_int, 0, sizeof(g_impdrv_cmnctl_judge_int));

    g_hook_impdrv_osdep_get_dsp_irq_channel = NULL;
    memset (&g_impdrv_osdep_get_dsp_irq_channel, 0, sizeof(g_impdrv_osdep_get_dsp_irq_channel));

    g_hook_impdrv_udefctl_udef_crc = NULL;
    memset (&g_impdrv_udefctl_udef_crc, 0, sizeof(g_impdrv_udefctl_udef_crc));
    
    g_hook_impdrv_udefctl_set_ecm = NULL;
    memset (&g_impdrv_udefctl_set_ecm, 0, sizeof(g_impdrv_udefctl_set_ecm));

    g_R_OSAL_IoGetNumOfDevices.deviceType = 0xCCCCCCCC;
    strcpy(g_R_OSAL_IoGetNumOfDevices.deviceType_in, "0xCCCCCCCC");
    g_R_OSAL_IoGetNumOfDevices.p_numOfDevices = 0xCCCCCCCC;
    g_R_OSAL_IoGetNumOfDevices.p_numOfDevices_in = 0xCCCCCCCC;
    g_R_OSAL_IoGetNumOfDevices.p_numOfDevices_out = 0xCCCCCCCC;

    g_R_OSAL_IoGetDeviceList.device_type_name = 0xCCCCCCCC;
    strcpy(g_R_OSAL_IoGetDeviceList.device_type_name_in, "0xCCCCCCCC");
    g_R_OSAL_IoGetDeviceList.p_buffer = 0xCCCCCCCC;
    strcpy(g_R_OSAL_IoGetDeviceList.p_buffer_in, "0xCCCCCCCC");
    g_R_OSAL_IoGetDeviceList.p_buffer_out = 0xCCCCCCCC;
    g_R_OSAL_IoGetDeviceList.buffer_size_in = 0xCCCCCCCC;
    g_R_OSAL_IoGetDeviceList.p_num_of_byte = 0xCCCCCCCC;
    g_R_OSAL_IoGetDeviceList.p_num_of_byte_in = 0xCCCCCCCC;
    g_R_OSAL_IoGetDeviceList.p_num_of_byte_out = 0xCCCCCCCC;

    g_R_OSAL_IoDeviceClose.handle = 0xCCCCCCCC;

    g_R_OSAL_IoDeviceOpen.id = 0xCCCCCCCC;
    strcpy(g_R_OSAL_IoDeviceOpen.id_in, "0xCCCCCCCC");
    g_R_OSAL_IoDeviceOpen.p_handle = 0xCCCCCCCC;
    g_R_OSAL_IoDeviceOpen.p_handle_in = 0xCCCCCCCC;
    for (i = 0; i < UT_R_OSAL_IoDeviceOpen_MAX_SIZE; i ++ )
    {
        g_R_OSAL_IoDeviceOpen.p_handle_out[i] = 0xCCCCCCCC;
    }
    g_R_OSAL_PmSetPolicy.handle = 0xCCCCCCCC;
    g_R_OSAL_PmSetPolicy.policy_in = 0xCCCCCCCC;

    g_R_OSAL_InterruptGetNumOfIrqChannels.handle = 0xCCCCCCCC;
    g_R_OSAL_InterruptGetNumOfIrqChannels.p_numOfChannels = 0xCCCCCCCC;
    g_R_OSAL_InterruptGetNumOfIrqChannels.p_numOfChannels_in = 0xCCCCCCCC;
    g_R_OSAL_InterruptGetNumOfIrqChannels.p_numOfChannels_out = 0xCCCCCCCC;

    g_R_OSAL_InterruptUnregisterIsr.handle = 0xCCCCCCCC;
    g_R_OSAL_InterruptUnregisterIsr.irq_channel_in = 0xCCCCCCCC;
    g_R_OSAL_InterruptUnregisterIsr.irqHandlerFunction_in = 0xCCCCCCCC;

    g_R_OSAL_InterruptDisableIsr.handle = 0xCCCCCCCC;
    g_R_OSAL_InterruptDisableIsr.irq_channel_in = 0xCCCCCCCC;

    g_R_OSAL_ThsyncMutexLockForTimePeriod.handle = 0xCCCCCCCC;
    g_R_OSAL_ThsyncMutexLockForTimePeriod.time_period = 0xCCCCCCCC;

    g_R_OSAL_ThsyncMutexCreate.mutex_id = 0xCCCCCCCC;
    g_R_OSAL_ThsyncMutexCreate.p_handle = 0xCCCCCCCC;
    g_R_OSAL_ThsyncMutexCreate.p_handle_in = 0xCCCCCCCC;
    g_R_OSAL_ThsyncMutexCreate.p_handle_out = 0xCCCCCCCC;

    g_R_OSAL_MqCreate.p_config = 0xCCCCCCCC;
    g_R_OSAL_MqCreate.mq_Id = 0xCCCCCCCC;
    g_R_OSAL_MqCreate.p_handle = 0xCCCCCCCC;
    g_R_OSAL_MqCreate.p_handle_in = 0xCCCCCCCC;
    for(i = 0; i < UT_R_OSAL_MqCreate_MAX_SIZE; i++)
    {
        g_R_OSAL_MqCreate.p_handle_out[i] = 0xCCCCCCCC;
    }

    g_R_OSAL_MqReset.handle = 0xCCCCCCCC;

    g_R_OSAL_ThreadCreate.p_config = 0xCCCCCCCC;
    g_R_OSAL_ThreadCreate.thread_id = 0xCCCCCCCC;
    g_R_OSAL_ThreadCreate.task_handle = 0xCCCCCCCC;
    g_R_OSAL_ThreadCreate.task_handle_in = 0xCCCCCCCC;
    g_R_OSAL_ThreadCreate.task_handle_out = 0xCCCCCCCC;

    g_R_OSAL_MqSendForTimePeriod.handle = 0xCCCCCCCC;
    g_R_OSAL_MqSendForTimePeriod.time_period = 0xCCCCCCCC;
    g_R_OSAL_MqSendForTimePeriod.p_buffer = 0xCCCCCCCC;
    g_R_OSAL_MqSendForTimePeriod.buffer_size = 0xCCCCCCCC;

    g_R_OSAL_MqReceiveForTimePeriod.handle = 0xCCCCCCCC;
    g_R_OSAL_MqReceiveForTimePeriod.time_period = 0xCCCCCCCC;
    for(i = 0; i < UT_R_OSAL_MqReceiveForTimePeriod_MAX_SIZE; i++)
    {
        g_R_OSAL_MqReceiveForTimePeriod.receive_buffer_out[i] = 0xCCCCCCCC;
    }
    g_R_OSAL_MqReceiveForTimePeriod.p_buffer = 0xCCCCCCCC;
    g_R_OSAL_MqReceiveForTimePeriod.buffer_size = 0xCCCCCCCC;

    g_R_OSAL_ThsyncMutexUnlock.handle = 0xCCCCCCCC;

    g_R_OSAL_ThreadJoin.handle = 0xCCCCCCCC;
    g_R_OSAL_ThreadJoin.p_return_value = 0xCCCCCCCC;

    g_R_OSAL_MqDelete.handle = 0xCCCCCCCC;

    g_R_OSAL_ThsyncMutexDestroy.handle = 0xCCCCCCCC;

    g_R_OSAL_InterruptRegisterIsr.handle = 0xCCCCCCCC;
    g_R_OSAL_InterruptRegisterIsr.irq_channel = 0xCCCCCCCC;
    g_R_OSAL_InterruptRegisterIsr.priority_value = 0xCCCCCCCC;
    g_R_OSAL_InterruptRegisterIsr.irqHandlerFunction = 0xCCCCCCCC;
    g_R_OSAL_InterruptRegisterIsr.p_irq_arg = 0xCCCCCCCC;

    g_R_OSAL_InterruptEnableIsr.handle = 0xCCCCCCCC;
    g_R_OSAL_InterruptEnableIsr.irq_channel = 0xCCCCCCCC;

    g_R_OSAL_PmGetState.handle = 0xCCCCCCCC;
    g_R_OSAL_PmGetState.p_state_out = OSAL_PM_STATE_CG;

    g_R_OSAL_PmGetPolicy.handle = 0xCCCCCCCC;
    g_R_OSAL_PmGetPolicy.p_policy_out = 0xCCCCCCCC;

    g_R_OSAL_IoWrite32.handle = 0xCCCCCCCC;
    g_R_OSAL_IoWrite32.offset = 0xCCCCCCCC;
    g_R_OSAL_IoWrite32.data = 0xCCCCCCCC;

    g_R_OSAL_MmngrInvalidate.handle = 0xCCCCCCCC;
    g_R_OSAL_MmngrInvalidate.offset = 0xCCCCCCCC;
    g_R_OSAL_MmngrInvalidate.size = 0xCCCCCCCC;

    g_R_OSAL_IoRead32.handle = 0xCCCCCCCC;
    g_R_OSAL_IoRead32.offset = 0xCCCCCCCC;
    g_R_OSAL_IoRead32.lRegData = 0xCCCCCCCC;
    g_R_OSAL_IoRead32.lRegData_in = 0xCCCCCCCC;
    for(i = 0; i < UT_R_OSAL_IoRead32_MAX_SIZE; i++)
    {
        g_R_OSAL_IoRead32.lRegData_out[i] = 0xCCCCCCCC;
    }

    g_R_OSAL_PmSetRequiredState.handle = 0xCCCCCCCC;
    g_R_OSAL_PmSetRequiredState.state = 0xCCCCCCCC;

    g_R_OSAL_PmSetResetState.handle = 0xCCCCCCCC;
    g_R_OSAL_PmSetResetState.state = 0xCCCCCCCC;

    g_impdrv_cmn_ctl_ins.device_io_imptop.handle = NULL;
    g_impdrv_cmn_ctl_ins.mutex_handle.handle = NULL;
    g_impdrv_cmn_ctl_ins.mutex_id = 0;
    g_impdrv_cmn_ctl_ins.mutex_time_period = 0;
    g_impdrv_cmn_ctl_ins.device_io_dta.handle = 0;
    memset(g_impdrv_cmn_ctl_ins.p_commonctl, 0, sizeof(st_impdrv_commonctl_t));
    
    g_R_OSAL_MmngrGetSize.p_size = 0xCCCCCCCC;
    g_R_OSAL_MmngrGetSize.p_size_in = 0xCCCCCCCC;
    for(i = 0; i < UT_R_OSAL_MmngrGetSize_MAX_SIZE; i++)
    {

        g_R_OSAL_MmngrGetSize.p_size_out[i] = 0xCCCCCCCC;

    }

    g_R_OSAL_MmngrGetCpuPtr.p_logical_adrs = 0xCCCCCCCC;
    g_R_OSAL_MmngrGetCpuPtr.p_logical_adrs_in = 0xCCCCCCCC;
    for(i = 0; i < UT_R_OSAL_MmngrGetCpuPtr_MAX_SIZE; i++)
    {

        g_R_OSAL_MmngrGetCpuPtr.p_logical_adrs_out[i] = 0xCCCCCCCC;

    }

    g_R_OSAL_MmngrGetHwAddr.p_physical_adrs = 0xCCCCCCCC;
    g_R_OSAL_MmngrGetHwAddr.p_physical_adrs_in = 0xCCCCCCCC;
    for(i = 0; i < UT_R_OSAL_MmngrGetHwAddr_MAX_SIZE; i++)
    {

        g_R_OSAL_MmngrGetHwAddr.p_physical_adrs_out[i] = 0xCCCCCCCC;

    }
    
    memset(&g_uts_p_corectl, 0, sizeof(g_uts_p_corectl));
    memset(&g_uts_cb_ercd, 0, sizeof(g_uts_cb_ercd));
    memset(&g_uts_p_state, 0, sizeof(g_uts_p_state));
    memset(&g_uts_p_mask_val, 0, sizeof(g_uts_p_mask_val));
    memset(&g_uts_p_cb_ercd_num, 0, sizeof(g_uts_p_cb_ercd_num));
    g_uts_p_is_check_intclear = false;
    memset(&g_uts_p_coretypectl, 0, sizeof(g_uts_p_coretypectl));
    //memset(&g_ut_p_corectl, 0, sizeof(g_ut_p_corectl));
    g_p_ut_p_impdrv_ctl = (st_impdrv_ctl_t*)malloc(sizeof(st_impdrv_ctl_t));
    // g_p_ut_p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].core_state = IMPDRV_STATE_UNINIT;
    g_uts_p_init_core_num = 0;
    memset(g_uts_init_core_info, 0, sizeof(g_uts_init_core_info));

    g_hook_impdrv_osdep_get_dsp_irq_channel = NULL;
    memset (&g_impdrv_osdep_get_dsp_irq_channel, 0, sizeof(g_impdrv_osdep_get_dsp_irq_channel));

    g_hook_impdrv_cnnctl_check_inten = NULL;
    memset(&g_impdrv_cnnctl_check_inten, 0, sizeof(g_impdrv_cnnctl_check_inten));
    g_p_funcs = (st_impdrv_corectl_func_t*)malloc(sizeof(st_impdrv_corectl_func_t));
}

/*imp DEINIT function*/
void ut_deinit_imp(void){

    if (osal_init)
    {
        free(g_p_uts_p_impdrv_ctl);
        free(g_p_ut_p_impdrv_ctl);
    }
    g_impdrv_cmn_ctl_chk.checksum = NULL;
}

e_impdrv_errorcode_t stub_p_impdrvCorectlSyncStop(
    st_impdrv_coretypectl_t coretype_ctl[IMPDRV_CORE_TYPE_MAX],
    const uint32_t          core_num
)
{
    return IMPDRV_EC_OK;
}

e_impdrv_errorcode_t stub_p_impdrvCorectlSyncStart(
    st_impdrv_coretypectl_t coretype_ctl[IMPDRV_CORE_TYPE_MAX],
    const uint32_t          core_num
)
{
    return IMPDRV_EC_OK;
}
