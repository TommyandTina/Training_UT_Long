#ifndef _UT_OSAL_STUB_H
#define _UT_OSAL_STUB_H

#ifndef IMPDRV_IRQ_PRIORITY_MAX
#define IMPDRV_IRQ_PRIORITY_MAX (15)
#endif /* IMPDRV_IRQ_PRIORITY_MAX */

#ifndef IMPDRV_REG_OFFSET_MAX
#define IMPDRV_REG_OFFSET_MAX       (1024U * 256U * 8U) /**< Maximum value for Register offset. */
#endif /* IMPDRV_REG_OFFSET_MAX */


#include "base.h"
#define UT_R_OSAL_IoGetNumOfDevices_MAX_SIZE (100)
#define UT_R_OSAL_IoGetDeviceList_MAX_SIZE (100)
#define UT_R_OSAL_IoDeviceOpen_MAX_SIZE (100)
#define UT_R_OSAL_IoDeviceClose_MAX_SIZE (100)
#define UT_R_OSAL_PmSetPolicy_MAX_SIZE (100)
#define UT_R_OSAL_InterruptDisableIsr_MAX_SIZE (100)
#define UT_R_OSAL_InterruptUnregisterIsr_MAX_SIZE (100)
#define UT_R_OSAL_InterruptGetNumOfIrqChannels_MAX_SIZE (100)
#define UT_R_OSAL_ThsyncMutexLockForTimePeriod_MAX_SIZE (100)
#define UT_R_OSAL_ThsyncMutexUnlock_MAX_SIZE (100)
#define UT_R_OSAL_ThsyncMutexCreate_MAX_SIZE (100)
#define UT_R_OSAL_ThsyncMutexDestroy_MAX_SIZE (100)
#define UT_R_OSAL_MqCreate_MAX_SIZE (100)
#define UT_R_OSAL_MqReset_MAX_SIZE (100)
#define UT_R_OSAL_MqDelete_MAX_SIZE (100)
#define UT_R_OSAL_MqSendForTimePeriod_MAX_SIZE (100)
#define UT_R_OSAL_MqReceiveForTimePeriod_MAX_SIZE (100)
#define UT_R_OSAL_ThreadCreate_MAX_SIZE (100)
#define UT_R_OSAL_ThreadJoin_MAX_SIZE (100)
#define UT_R_OSAL_InterruptRegisterIsr_MAX_SIZE (100)
#define UT_R_OSAL_InterruptEnableIsr_MAX_SIZE (100)
#define UT_R_OSAL_PmGetState_MAX_SIZE (100)
#define UT_R_OSAL_PmGetPolicy_MAX_SIZE (100)
#define UT_R_OSAL_IoWrite32_MAX_SIZE (5000)
#define UT_R_OSAL_IoRead32_MAX_SIZE (5000)
#define UT_R_OSAL_PmSetRequiredState_MAX_SIZE (100)
#define UT_R_OSAL_PmSetResetState_MAX_SIZE (100)
#define UT_R_OSAL_ThsyncMemoryBarrier_MAX_SIZE (100)
#define UT_R_OSAL_MmngrInvalidate_MAX_SIZE (100)
#define UT_R_OSAL_MmngrFlush_MAX_SIZE (100)
#define UT_R_OSAL_MmngrGetHwAddr_MAX_SIZE (100)
#define UT_R_OSAL_MmngrGetCpuPtr_MAX_SIZE (100)
#define UT_R_OSAL_MmngrGetSize_MAX_SIZE (100)
#define UT_R_OSAL_impdrv_cmnctl_get_corectl_func_MAX_SIZE  (100)
#define UT_impdrv_cnnctl_get_inten_val_MAX_SIZE  (100)
#define UT_impdrv_cnnctl_check_inten_MAX_SIZE    (100)
#define UT_impdrv_pscctl_get_inten_val_MAX_SIZE  (100)
#define UT_impdrv_cnnctl_set_state_MAX_SIZE  (100)
#define UT_impdrv_pscctl_set_state_MAX_SIZE  (100)
#define UT_impdrv_dmactl_get_inten_val_MAX_SIZE  (100)
#define UT_impdrv_ocvctl_get_inten_val_MAX_SIZE  (100)
#define UT_impdrv_impsctl_get_inten_val_MAX_SIZE  (100)
#define UT_impdrv_ocvctl_set_state_MAX_SIZE  (100)
#define UT_impdrv_osdep_pow_on_imp_MAX_SIZE  (100)
#define UT_impdrv_impsctl_set_state_MAX_SIZE  (100)
#define UT_impdrv_impctl_get_inten_val_MAX_SIZE  (100)
#define UT_impdrv_impctl_set_state_MAX_SIZE  (100)
#define UT_impdrv_cmnctl_judge_int_MAX_SIZE  (100)
#define UT_impdrv_osdep_memory_data_set_MAX_SIZE  (100)
#define UT_impdrv_dmactl_set_state_MAX_SIZE  (100)
#define UT_R_OSAL_impdrv_genctl_init_set_state_MAX_SIZE  (100) 
#define UT_R_OSAL_impdrv_cmnctl_set_checksum_MAX_SIZE  (100)
#define UT_R_OSAL_impdrv_cmnctl_cnv_int_bit_core_MAX_SIZE  (100)
#define UT_R_OSAL_impdrv_cmnctl_calc_checksum_MAX_SIZE  (100) 
#define UT_R_OSAL_impdrv_cmnctl_reg_find_resource_MAX_SIZE  (100)
#define UT_R_OSAL_impdrv_cmnctl_reg_set_mem_protect_MAX_SIZE  (100)
#define UT_R_OSAL_impdrv_cmnctl_get_io_imptop_MAX_SIZE  (100)
#define UT_R_OSAL_PmWaitForState_MAX_SIZE  (100)
#define UT_R_OSAL_PmGetResetState_MAX_SIZE  (100)
#define UT_R_OSAL_PmGetRequiredState_MAX_SIZE  (100)
#define UT_impdrv_osdep_get_dsp_irq_channel_MAX_SIZE (100)
#define UT_impdrv_genctl_dsp_int_handler_MAX_SIZE (100)

#define ut_init_v3u(bool) ut_init_v3u(bool)
#define ut_init_v3h2(bool) ut_init_v3h2(bool)
#define ut_init_v3h1(bool) ut_init_v3h1(bool)
#define ut_init_v3m2(bool) ut_init_v3m2(bool)

#define ut_deinit_v3u(bool) ut_deinit_v3u(bool)
#define ut_deinit_v3h2(bool) ut_deinit_v3h2(bool)
#define ut_deinit_v3h1(bool) ut_deinit_v3h1(bool)
#define ut_deinit_v3m2(bool) ut_deinit_v3m2(bool)

#define ut_init_impdrv(bool) ut_init_imp(bool)
#define ut_deinit_impdrv(bool) ut_deinit_imp(bool)

extern st_impdrv_hwrsc_def_t p_resource_null;

typedef struct
{
	e_impdrv_core_type_t	core_type;
	uint32_t				core_num;
	uint32_t				bit;
} st_impdrv_inten_tbl_t;

typedef uint32_t (*p_impdrv_cnnctl_get_inten_val)(const bool irq_mask[IMPDRV_IRQMASK_MAX]);

typedef e_impdrv_errorcode_t (*p_impdrv_cmnctl_reg_set_mem_protect)(st_impdrv_commonctl_t *const p_commonctl, const e_impdrv_protect_mode_t protect_mode);

typedef struct {
    uint32_t ReturnValue[UT_impdrv_cnnctl_get_inten_val_MAX_SIZE];
    uint32_t ut_index;
}s_impdrv_cnnctl_get_inten_val;

typedef uint32_t (*p_impdrv_cnnctl_check_inten)(
    const uint32_t          stat_val,
    uint32_t                *const p_mask_val,
    e_impdrv_cb_ret_t       cb_ercd[IMPDRV_CB_RET_MAX],
    uint32_t                *const p_cb_ercd_num,
    bool                    *const p_is_check_intclear,
    e_impdrv_state_t        *const p_state
);

typedef e_impdrv_errorcode_t (*p_impdrv_cmnctl_reg_set_mem_protect)(st_impdrv_commonctl_t *const p_commonctl, const e_impdrv_protect_mode_t protect_mode);

typedef struct {
    uint32_t ReturnValue[UT_impdrv_cnnctl_check_inten_MAX_SIZE];
    e_impdrv_state_t p_state_out[UT_impdrv_cnnctl_check_inten_MAX_SIZE];
    uint32_t p_mask_val_out[UT_impdrv_cnnctl_check_inten_MAX_SIZE];
    uint32_t ut_index;
}s_impdrv_cnnctl_check_inten;

typedef uint32_t (*p_impdrv_pscctl_get_inten_val)(const bool irq_mask[IMPDRV_IRQMASK_MAX]);

typedef struct {
    uint32_t ReturnValue[UT_impdrv_pscctl_get_inten_val_MAX_SIZE];
    uint32_t ut_index;
}s_impdrv_pscctl_get_inten_val;

typedef uint32_t (*p_impdrv_ocvctl_get_inten_val)(const bool irq_mask[IMPDRV_IRQMASK_MAX]);
typedef e_impdrv_errorcode_t (*p_impdrv_cmnctl_reg_find_resource_stub)(const char *const p_device_id, uint32_t *const p_index, st_impdrv_hwrsc_def_t *const p_resource);
typedef struct {
    uint32_t ReturnValue[UT_impdrv_ocvctl_get_inten_val_MAX_SIZE];
    uint32_t ut_index;
}s_impdrv_ocvctl_get_inten_val;

typedef struct {
    uint32_t p_index_out;
    st_impdrv_hwrsc_def_t p_resource_out;
    e_impdrv_errorcode_t ReturnValue[UT_R_OSAL_impdrv_cmnctl_reg_find_resource_MAX_SIZE];
    uint32_t ut_index;
}s_impdrv_cmnctl_reg_find_resource;

typedef struct {
    uint32_t ReturnValue[UT_R_OSAL_impdrv_cmnctl_reg_set_mem_protect_MAX_SIZE];
    uint32_t ut_index;
}s_impdrv_cmnctl_reg_set_mem_protect;

typedef e_impdrv_errorcode_t (*p_impdrv_cmnctl_get_io_imptop_stub)(st_impdrv_device_handle_t *const p_handle);

typedef struct {
    e_impdrv_errorcode_t ReturnValue[UT_R_OSAL_impdrv_cmnctl_get_io_imptop_MAX_SIZE];
    uint32_t ut_index;
}s_impdrv_cmnctl_get_io_imptop;

typedef e_impdrv_errorcode_t (*p_impdrv_cmnctl_judge_int_stub)(const st_impdrv_commonctl_t *const p_commonctl,const uint64_t irq_channel,st_impdrv_core_info_t core_info[IMPDRV_INTSTS_BITNUM], uint32_t *const p_use_core_num, bool is_fb_callback);

typedef struct {
    e_impdrv_errorcode_t ReturnValue[UT_impdrv_cmnctl_judge_int_MAX_SIZE];
    st_impdrv_core_info_t core_info_out;
    uint32_t p_use_core_num_out;
    uint32_t ut_index;
}s_impdrv_cmnctl_judge_int;


//#if defined(RCAR_V3H2) | defined(RCAR_V3H1)

typedef uint32_t (*p_impdrv_impsctl_get_inten_val)(const bool irq_mask[IMPDRV_IRQMASK_MAX]);

typedef struct {
    uint32_t ReturnValue[UT_impdrv_impsctl_get_inten_val_MAX_SIZE];
    uint32_t ut_index;
}s_impdrv_impsctl_get_inten_val;

extern s_impdrv_impsctl_get_inten_val g_impdrv_impsctl_get_inten_val;

extern const uint32_t ut_stub_impdrv_impsctl_get_inten_val(const bool irq_mask[IMPDRV_IRQMASK_MAX]);
extern p_impdrv_impsctl_get_inten_val g_hook_impdrv_impsctl_get_inten_val;

const uint32_t ut_stub_impdrv_impsctl_get_inten_val(const bool irq_mask[IMPDRV_IRQMASK_MAX]);
const uint32_t impdrv_impsctl_get_inten_val_stub(const bool irq_mask[IMPDRV_IRQMASK_MAX]);

extern const e_impdrv_errorcode_t ut_stub_impdrv_impsctl_set_state(st_impdrv_corectl_t   *const p_corectl,const e_impdrv_state_t  state);
typedef e_impdrv_errorcode_t (*p_impdrv_impsctl_set_state)(st_impdrv_corectl_t   *const p_corectl,const e_impdrv_state_t  state);

typedef struct {
    e_impdrv_errorcode_t ReturnValue[UT_impdrv_impsctl_set_state_MAX_SIZE];
    uint32_t ut_index;
}s_impdrv_impsctl_set_state;

extern s_impdrv_impsctl_set_state g_impdrv_impsctl_set_state;

extern const e_impdrv_errorcode_t ut_stub_impdrv_impsctl_set_state(st_impdrv_corectl_t   *const p_corectl,const e_impdrv_state_t  state);

typedef e_impdrv_errorcode_t (*p_impdrv_impsctl_set_state)(st_impdrv_corectl_t   *const p_corectl,const e_impdrv_state_t  state);

extern p_impdrv_impsctl_set_state g_hook_impdrv_impsctl_set_state;

//#endif
typedef e_impdrv_errorcode_t (*p_impdrv_ocvctl_set_state)(st_impdrv_corectl_t   *const p_corectl,const e_impdrv_state_t  state);

typedef struct {
    e_impdrv_errorcode_t ReturnValue[UT_impdrv_ocvctl_set_state_MAX_SIZE];
    uint32_t ut_index;
}s_impdrv_ocvctl_set_state;

typedef e_impdrv_errorcode_t (*p_impdrv_genctl_init_set_state_stub)(st_impdrv_ctl_t *const p_impdrv_ctl, const uint32_t use_core_num, const st_impdrv_core_info_t *const p_core_info, const st_impdrv_corectl_func_t **const p_funcs);
typedef e_impdrv_errorcode_t (*p_impdrv_cmnctl_calc_checksum_stub)(const void* *const p_data,const size_t size,uint32_t *const p_checksum);

typedef e_impdrv_errorcode_t (*p_impdrv_cmnctl_set_checksum_stub)(void);
typedef e_impdrv_errorcode_t (*p_impdrv_cmnctl_test_checksum_stub)(void);

typedef e_impdrv_errorcode_t (*p_impdrv_cmnctl_cnv_int_bit_core_stub)(const st_impdrv_inten_tbl_t *const p_s_inten_cnv_table, const uint32_t cnv_table_num, const uint32_t stat, st_impdrv_core_info_t *core_info, uint32_t *const p_use_core_num);

typedef e_impdrv_errorcode_t (*p_impdrv_cmnctl_cnv_int_core_bit_stub)(const st_impdrv_core_info_t *const p_core_info, const uint32_t use_core_num, const st_impdrv_inten_tbl_t const* p_inten_cnv_table, const uint32_t cnv_table_num, uint32_t *const p_val);

typedef e_impdrv_errorcode_t (*p_impdrv_cmnctl_get_group_stub)(e_impdrv_irq_group_t *const p_irq_group, const e_impdrv_instance_t instance_num);

//#if defined(RCAR_V3H2)

#define UT_impdrv_dmasctl_get_inten_val_MAX_SIZE  (100)

typedef uint32_t (*p_impdrv_dmasctl_get_inten_val)(const bool irq_mask[IMPDRV_IRQMASK_MAX]);

typedef struct {
    uint32_t ReturnValue[UT_impdrv_dmasctl_get_inten_val_MAX_SIZE];
    uint32_t ut_index;
}s_impdrv_dmasctl_get_inten_val;

extern s_impdrv_dmasctl_get_inten_val g_impdrv_dmasctl_get_inten_val;

extern const uint32_t ut_stub_impdrv_dmasctl_get_inten_val(const bool irq_mask[IMPDRV_IRQMASK_MAX]);
extern p_impdrv_dmasctl_get_inten_val g_hook_impdrv_dmasctl_get_inten_val;

const uint32_t ut_stub_impdrv_dmasctl_get_inten_val(const bool irq_mask[IMPDRV_IRQMASK_MAX]);
const uint32_t impdrv_dmasctl_get_inten_val_stub(const bool irq_mask[IMPDRV_IRQMASK_MAX]);


#define UT_impdrv_dmasctl_set_state_MAX_SIZE  (100)

typedef uint32_t (*p_impdrv_dmasctl_set_state)(st_impdrv_corectl_t *const p_corectl,const e_impdrv_state_t  state);
typedef uint32_t (*p_impdrv_dmasctl_osal_core_num)(const uint32_t              core_num);
typedef uint32_t (*p_impdrv_dmactl_osal_core_num)(const uint32_t              core_num);

typedef struct {
    uint32_t ReturnValue[UT_impdrv_dmasctl_set_state_MAX_SIZE];
    uint32_t ut_index;
}s_impdrv_dmasctl_set_state;
typedef struct {
    uint32_t ReturnValue[10];
    uint32_t ut_index;
}s_impdrv_dmasctl_osal_core_num;
typedef struct {
    uint32_t ReturnValue[10];
    uint32_t ut_index;
}s_impdrv_dmactl_osal_core_num;
extern s_impdrv_dmasctl_set_state g_impdrv_dmasctl_set_state;
extern s_impdrv_dmasctl_osal_core_num g_impdrv_dmasctl_osal_core_num;
extern s_impdrv_dmactl_osal_core_num g_impdrv_dmactl_osal_core_num;

extern const e_impdrv_errorcode_t ut_stub_impdrv_dmasctl_set_state(st_impdrv_corectl_t *const p_corectl,const e_impdrv_state_t  state);
extern p_impdrv_dmasctl_set_state g_hook_impdrv_dmasctl_set_state;
extern const uint32_t ut_stub_impdrv_dmasctl_osal_core_num(const uint32_t              core_num);
extern p_impdrv_dmasctl_osal_core_num g_hook_impdrv_dmasctl_osal_core_num;
extern const uint32_t ut_stub_impdrv_dmactl_osal_core_num(const uint32_t              core_num);
extern p_impdrv_dmactl_osal_core_num g_hook_impdrv_dmactl_osal_core_num;
const e_impdrv_errorcode_t ut_stub_impdrv_dmasctl_set_state(st_impdrv_corectl_t *const p_corectl,const e_impdrv_state_t  state);
const e_impdrv_errorcode_t impdrv_dmasctl_set_state_stub(st_impdrv_corectl_t *const p_corectl,const e_impdrv_state_t  state);
const uint32_t ut_stub_impdrv_dmasctl_osal_core_num(const uint32_t              core_num);
const uint32_t impdrv_dmasctl_osal_core_num_stub(const uint32_t              core_num);
const uint32_t ut_stub_impdrv_dmactl_osal_core_num(const uint32_t              core_num);
const uint32_t impdrv_dmactl_osal_core_num_stub(const uint32_t              core_num);
typedef struct {
    uint32_t ReturnValue[UT_impdrv_osdep_get_dsp_irq_channel_MAX_SIZE];
    uint32_t ut_index;
}s_impdrv_osdep_get_dsp_irq_channel;

extern s_impdrv_osdep_get_dsp_irq_channel g_impdrv_osdep_get_dsp_irq_channel;
typedef uint32_t (*p_impdrv_osdep_get_dsp_irq_channel)(const bool irq_mask[IMPDRV_IRQMASK_MAX]);

extern const e_impdrv_errorcode_t ut_stub_impdrv_osdep_get_dsp_irq_channel(uint64_t *const irq_channel);
extern p_impdrv_osdep_get_dsp_irq_channel g_hook_impdrv_osdep_get_dsp_irq_channel;

//#endif

typedef e_impdrv_errorcode_t (*p_impdrv_cnnctl_set_state)(st_impdrv_corectl_t   *const p_corectl,const e_impdrv_state_t  state);

typedef struct {
    e_impdrv_errorcode_t ReturnValue[UT_impdrv_cnnctl_set_state_MAX_SIZE];
    uint32_t ut_index;
}s_impdrv_cnnctl_set_state;


typedef e_impdrv_errorcode_t (*p_impdrv_pscctl_set_state)(st_impdrv_corectl_t   *const p_corectl,const e_impdrv_state_t  state);

typedef struct {
    e_impdrv_errorcode_t ReturnValue[UT_impdrv_pscctl_set_state_MAX_SIZE];
    uint32_t ut_index;
}s_impdrv_pscctl_set_state;


typedef uint32_t (*p_impdrv_dmactl_get_inten_val)(const bool irq_mask[IMPDRV_IRQMASK_MAX]);

typedef struct {
    uint32_t ReturnValue[UT_impdrv_dmactl_get_inten_val_MAX_SIZE];
    uint32_t ut_index;
}s_impdrv_dmactl_get_inten_val;
typedef uint32_t (*p_impdrv_impctl_get_inten_val)(const bool irq_mask[IMPDRV_IRQMASK_MAX]);

typedef struct {
    uint32_t ReturnValue[UT_impdrv_impctl_get_inten_val_MAX_SIZE];
    uint32_t ut_index;
}s_impdrv_impctl_get_inten_val;

typedef e_impdrv_errorcode_t (*p_impdrv_impctl_set_state)(st_impdrv_corectl_t   *const p_corectl,const e_impdrv_state_t  state);

typedef e_impdrv_errorcode_t (*p_impdrv_dspctl_set_state)(st_impdrv_corectl_t   *const p_corectl,const e_impdrv_state_t  state);

typedef struct {
    e_impdrv_errorcode_t ReturnValue[UT_impdrv_impctl_set_state_MAX_SIZE];
    uint32_t ut_index;
}s_impdrv_impctl_set_state;

typedef struct {
    e_impdrv_errorcode_t ReturnValue[UT_impdrv_impctl_set_state_MAX_SIZE];
    uint32_t ut_index;
}s_impdrv_dspctl_set_state;

typedef e_impdrv_errorcode_t (*p_impdrv_osdep_memory_data_set)(uint32_t *const p_buffer,const uint32_t  data,const size_t size);

typedef struct {
    e_impdrv_errorcode_t ReturnValue[UT_impdrv_osdep_memory_data_set_MAX_SIZE];
    uint32_t ut_index;
}s_impdrv_osdep_memory_data_set;
typedef e_impdrv_errorcode_t (*p_impdrv_dmactl_set_state)(st_impdrv_corectl_t   *const p_corectl,const e_impdrv_state_t  state);

typedef struct {
    e_impdrv_errorcode_t ReturnValue[UT_impdrv_dmactl_set_state_MAX_SIZE];
    uint32_t ut_index;
}s_impdrv_dmactl_set_state;

typedef struct {
    e_impdrv_errorcode_t ReturnValue[UT_R_OSAL_impdrv_genctl_init_set_state_MAX_SIZE];
    uint32_t ut_index;
}s_impdrv_genctl_init_set_state;

typedef struct {
    e_impdrv_errorcode_t ReturnValue[UT_R_OSAL_impdrv_cmnctl_set_checksum_MAX_SIZE];
    uint32_t ut_index;
}s_impdrv_cmnctl_set_checksum;
typedef struct {
    e_impdrv_errorcode_t ReturnValue[UT_R_OSAL_impdrv_cmnctl_set_checksum_MAX_SIZE];
    uint32_t ut_index;
}s_impdrv_cmnctl_test_checksum;

typedef e_impdrv_errorcode_t (*p_impdrv_osdep_pow_on_imp)(st_impdrv_device_handle_t   *const p_device_handle,const st_impdrv_core_info_t core_info);

typedef struct {
    e_impdrv_errorcode_t ReturnValue[UT_impdrv_osdep_pow_on_imp_MAX_SIZE];
    uint32_t ut_index;
}s_impdrv_osdep_pow_on_imp;

typedef e_impdrv_errorcode_t (*p_impdrv_genctl_dsp_int_handle)(st_impdrv_ctl_t *const p_impdrv_ctl, const uint64_t irq_channel, const e_impdrv_errorcode_t ercd, const st_impdrv_core_info_t core_info);
typedef struct {
    e_impdrv_errorcode_t ReturnValue[UT_impdrv_genctl_dsp_int_handler_MAX_SIZE];
    uint32_t ut_index;
}s_impdrv_genctl_dsp_int_handler;

typedef struct {
    e_impdrv_errorcode_t ReturnValue[UT_R_OSAL_impdrv_cmnctl_calc_checksum_MAX_SIZE];
    uint32_t p_checksum_out[UT_R_OSAL_impdrv_cmnctl_calc_checksum_MAX_SIZE];
    uint32_t ut_index;
}s_impdrv_cmnctl_calc_checksum;

typedef struct {
    e_impdrv_errorcode_t ReturnValue[UT_R_OSAL_impdrv_cmnctl_set_checksum_MAX_SIZE];
    uint32_t ut_index;
}s_impdrv_cmnctl_cnv_int_bit_core;

typedef struct {
    e_impdrv_errorcode_t ReturnValue[UT_R_OSAL_impdrv_cmnctl_set_checksum_MAX_SIZE];
    uint32_t ut_index;
}s_impdrv_cmnctl_cnv_int_core_bit;

typedef struct {
    e_impdrv_errorcode_t ReturnValue[UT_R_OSAL_impdrv_cmnctl_set_checksum_MAX_SIZE];
    uint32_t ut_index;
}s_impdrv_cmnctl_get_group;

typedef st_impdrv_corectl_func_t *(*p_impdrv_cmnctl_get_corectl_func_stub)(const e_impdrv_core_type_t core_type);

typedef e_impdrv_errorcode_t (*p_impdrv_cmnctl_get_init_param_stub)(
    const st_impdrv_commonctl_t *const p_commonctl,
    e_osal_interrupt_priority_t *const p_irq_priority,
    impdrv_ctrl_handle_t        *const p_osal_cb_args
);
typedef p_impdrv_dspctl_dsp_start_t (*p_impdrv_cmnctl_get_dsp_func_stub)(void);
typedef e_impdrv_errorcode_t (*p_impdrv_genctl_chk_state_init_stub)(
    const st_impdrv_ctl_t               *const p_impdrv_ctl,
    const st_impdrv_corectl_func_t      *p_funcs[IMPDRV_CORE_TYPE_MAX],
    st_impdrv_core_info_t               init_core_info[IMPDRV_INNER_FIXED_VALUE],
    uint32_t                            *const p_init_core_num
);

typedef struct {
    st_impdrv_corectl_func_t* ReturnValue[UT_R_OSAL_impdrv_cmnctl_get_corectl_func_MAX_SIZE];
    uint32_t ut_index;
}s_impdrv_cmnctl_get_corectl_func;

typedef struct {
    e_impdrv_errorcode_t ReturnValue[10];
    uint32_t ut_index;
}s_impdrv_cmnctl_get_init_param;
typedef struct {
    p_impdrv_dspctl_dsp_start_t ReturnValue[10];
    uint32_t ut_index;
}s_impdrv_cmnctl_get_dsp_func;
typedef struct {
    e_impdrv_errorcode_t ReturnValue[10];
    uint32_t p_init_core_num_out[10];
    uint32_t ut_index;
}s_impdrv_genctl_chk_state_init;

typedef uint32_t (*p_impdrv_udefctl_udef_crc_val)( uint32_t st, uint32_t sz);
typedef struct {
    uint32_t ReturnValue[10];
    uint32_t st_out;
    uint32_t sz_out;
    uint32_t ut_index;
}s_impdrv_udefctl_udef_crc_val;

typedef void (*p_impdrv_udefctl_set_ecm)( int32_t core_num, bool flag);
typedef struct {
    uint32_t ReturnValue[10];
    int32_t core_num;
    bool flag;
    uint32_t ut_index;
}s_impdrv_udefctl_set_ecm;

typedef struct {
    char* deviceType;
    size_t* p_numOfDevices;
    char deviceType_in[32];
    size_t p_numOfDevices_in;
    size_t p_numOfDevices_out;
    e_osal_return_t ReturnValue[UT_R_OSAL_IoGetNumOfDevices_MAX_SIZE];
    uint32_t ut_index;
} s_R_OSAL_IoGetNumOfDevices;

typedef struct {
    char* device_type_name;
    char device_type_name_in[32];
    char* p_buffer;
    char  p_buffer_in[32];
    uint32_t  p_buffer_out;
    size_t buffer_size_in;
    size_t* p_num_of_byte;
    size_t p_num_of_byte_in;
    size_t p_num_of_byte_out;
    e_osal_return_t ReturnValue;
    uint32_t ut_index;
} s_R_OSAL_IoGetDeviceList;

typedef struct {
    char* id;
    char id_in[32];
    osal_device_handle_t* p_handle;
    uint32_t p_handle_in;
    uint32_t p_handle_out[UT_R_OSAL_IoDeviceOpen_MAX_SIZE];
    e_osal_return_t ReturnValue[UT_R_OSAL_IoDeviceOpen_MAX_SIZE];
    uint32_t ut_index;
} s_R_OSAL_IoDeviceOpen;

typedef struct {
    osal_device_handle_t handle;
    e_osal_return_t ReturnValue[UT_R_OSAL_IoDeviceClose_MAX_SIZE];
    uint32_t ut_index;
} s_R_OSAL_IoDeviceClose;

typedef struct {
    e_osal_return_t ReturnValue[UT_R_OSAL_PmWaitForState_MAX_SIZE];
    uint32_t ut_index;
} s_R_OSAL_PmWaitForState;

typedef struct {
    osal_device_handle_t handle;
    e_osal_pm_reset_state_t p_state_out[UT_R_OSAL_PmGetResetState_MAX_SIZE];
    e_osal_return_t ReturnValue[UT_R_OSAL_PmGetResetState_MAX_SIZE];
    uint32_t ut_index;
} s_R_OSAL_PmGetResetState;

typedef struct {
    osal_device_handle_t handle;
    e_osal_pm_policy_t policy_in;
    e_osal_return_t ReturnValue[UT_R_OSAL_PmSetPolicy_MAX_SIZE];
    uint32_t ut_index;
} s_R_OSAL_PmSetPolicy;

typedef struct {
    osal_device_handle_t handle;
    size_t* p_numOfChannels;
    size_t p_numOfChannels_in;
    size_t p_numOfChannels_out;
    e_osal_return_t ReturnValue[UT_R_OSAL_InterruptGetNumOfIrqChannels_MAX_SIZE];
    uint32_t ut_index;
} s_R_OSAL_InterruptGetNumOfIrqChannels;

typedef struct {
    osal_device_handle_t handle;
    uint64_t irq_channel_in;
    p_osal_isr_func_t irqHandlerFunction_in;
    e_osal_return_t ReturnValue[UT_R_OSAL_InterruptUnregisterIsr_MAX_SIZE];
    uint32_t ut_index;
} s_R_OSAL_InterruptUnregisterIsr;

typedef struct {
    osal_device_handle_t handle;
    uint64_t irq_channel_in;
    e_osal_return_t ReturnValue[UT_R_OSAL_InterruptDisableIsr_MAX_SIZE];
    uint32_t ut_index;
} s_R_OSAL_InterruptDisableIsr;
typedef struct {
    osal_mutex_handle_t handle;
    osal_milli_sec_t time_period;
    e_osal_return_t ReturnValue[UT_R_OSAL_ThsyncMutexLockForTimePeriod_MAX_SIZE];
    uint32_t ut_index;
} s_R_OSAL_ThsyncMutexLockForTimePeriod;

typedef struct {
    osal_mutex_id_t mutex_id;
    osal_mutex_handle_t* p_handle;
    osal_mutex_handle_t p_handle_in;
    uint32_t p_handle_out;
    e_osal_return_t ReturnValue[UT_R_OSAL_ThsyncMutexCreate_MAX_SIZE];
    uint32_t ut_index; 
} s_R_OSAL_ThsyncMutexCreate;

typedef struct {
    osal_mutex_handle_t handle;
    e_osal_return_t ReturnValue[UT_R_OSAL_ThsyncMutexDestroy_MAX_SIZE];
    uint32_t ut_index; 
} s_R_OSAL_ThsyncMutexDestroy;
typedef struct {
    osal_mutex_handle_t handle;
    e_osal_return_t ReturnValue[UT_R_OSAL_ThsyncMutexUnlock_MAX_SIZE];
    uint32_t ut_index; 
} s_R_OSAL_ThsyncMutexUnlock;

typedef struct {
    const st_osal_mq_config_t *p_config; 
    osal_mq_id_t mq_Id; 
    osal_mq_handle_t *p_handle;
    
    osal_mq_handle_t *p_handle_in;
    osal_mq_handle_t *p_handle_out[UT_R_OSAL_MqCreate_MAX_SIZE];
    e_osal_return_t ReturnValue[UT_R_OSAL_MqCreate_MAX_SIZE];
    
    uint32_t ut_index; 
} s_R_OSAL_MqCreate;

typedef struct {
    osal_mq_handle_t handle;
    e_osal_return_t ReturnValue[UT_R_OSAL_MqReset_MAX_SIZE];
    uint32_t ut_index; 
} s_R_OSAL_MqReset;
typedef struct {
    osal_mq_handle_t handle;
    e_osal_return_t ReturnValue[UT_R_OSAL_MqDelete_MAX_SIZE];
    uint32_t ut_index; 
} s_R_OSAL_MqDelete;

typedef struct {
    osal_mq_handle_t handle;
    osal_milli_sec_t time_period;
    void * p_buffer; 
    size_t buffer_size;
    e_osal_return_t ReturnValue[UT_R_OSAL_MqSendForTimePeriod_MAX_SIZE];
    uint32_t ut_index; 
} s_R_OSAL_MqSendForTimePeriod;

typedef struct {
    osal_mq_handle_t handle;
    osal_milli_sec_t time_period;
    void *p_buffer;
    size_t buffer_size;
    uint32_t receive_buffer_out[UT_R_OSAL_MqReceiveForTimePeriod_MAX_SIZE];
    e_osal_return_t ReturnValue[UT_R_OSAL_MqReceiveForTimePeriod_MAX_SIZE];
    uint32_t ut_index; 
} s_R_OSAL_MqReceiveForTimePeriod;

typedef struct {

    st_osal_thread_config_t *p_config; 
    osal_thread_id_t thread_id;
    osal_thread_handle_t *p_handle;
    osal_thread_handle_t task_handle;
    osal_thread_handle_t task_handle_in;
    osal_thread_handle_t task_handle_out;
    e_osal_return_t ReturnValue[UT_R_OSAL_ThreadCreate_MAX_SIZE];
    uint32_t ut_index;
} s_R_OSAL_ThreadCreate;

typedef struct {
    osal_thread_handle_t handle;
    int64_t *p_return_value;
    e_osal_return_t ReturnValue[UT_R_OSAL_ThreadJoin_MAX_SIZE];
    uint32_t ut_index;
} s_R_OSAL_ThreadJoin;

typedef struct {
    osal_device_handle_t handle;
    uint64_t irq_channel;
    e_osal_interrupt_priority_t priority_value; 
    p_osal_isr_func_t irqHandlerFunction; 
    void* p_irq_arg;
    e_osal_return_t ReturnValue[UT_R_OSAL_InterruptRegisterIsr_MAX_SIZE];
    uint32_t ut_index;
} s_R_OSAL_InterruptRegisterIsr;

typedef struct {
    osal_device_handle_t handle;
    uint64_t irq_channel;
    e_osal_return_t ReturnValue[UT_R_OSAL_InterruptEnableIsr_MAX_SIZE];
    uint32_t ut_index;
} s_R_OSAL_InterruptEnableIsr;

typedef struct {
    osal_device_handle_t handle;
    e_osal_pm_state_t p_state_out;
    e_osal_return_t ReturnValue[UT_R_OSAL_PmGetState_MAX_SIZE];
    uint32_t ut_index;
} s_R_OSAL_PmGetState;

typedef struct {
    osal_device_handle_t handle;
    e_osal_pm_policy_t p_policy_out;
    e_osal_return_t ReturnValue[UT_R_OSAL_PmGetPolicy_MAX_SIZE];
    uint32_t ut_index;
} s_R_OSAL_PmGetPolicy;

typedef struct {
    osal_device_handle_t handle;
    uintptr_t offset;
    uint32_t data;
    e_osal_return_t ReturnValue[UT_R_OSAL_IoWrite32_MAX_SIZE];
    uint32_t ut_index;
} s_R_OSAL_IoWrite32;

typedef struct {
    osal_device_handle_t handle;
    uintptr_t offset;
    uint32_t * lRegData;
    uint32_t lRegData_in;
    uint32_t lRegData_out[UT_R_OSAL_IoRead32_MAX_SIZE];
    e_osal_return_t ReturnValue[UT_R_OSAL_IoRead32_MAX_SIZE];
    uint32_t ut_index;
} s_R_OSAL_IoRead32;

typedef struct {
    osal_device_handle_t handle;
    e_osal_pm_required_state_t state;
    bool applyImmediate;
    e_osal_return_t ReturnValue[UT_R_OSAL_PmSetRequiredState_MAX_SIZE];
    uint32_t ut_index;
} s_R_OSAL_PmSetRequiredState;

typedef struct {
    osal_device_handle_t handle;
    e_osal_pm_reset_state_t state;
    e_osal_return_t ReturnValue[UT_R_OSAL_PmSetResetState_MAX_SIZE];
    uint32_t ut_index;
} s_R_OSAL_PmSetResetState;
typedef struct {
    osal_device_handle_t handle;
    e_osal_pm_state_t p_state_out[UT_R_OSAL_PmGetRequiredState_MAX_SIZE];
    e_osal_return_t ReturnValue[UT_R_OSAL_PmGetRequiredState_MAX_SIZE];
    uint32_t ut_index;
} s_R_OSAL_PmGetRequiredState;

typedef struct {
    e_osal_return_t ReturnValue[UT_R_OSAL_ThsyncMemoryBarrier_MAX_SIZE];
    uint32_t ut_index;
} s_R_OSAL_ThsyncMemoryBarrier;


typedef struct {
    osal_memory_buffer_handle_t handle;
    uintptr_t offset;
    size_t size;
    e_osal_return_t ReturnValue[UT_R_OSAL_MmngrInvalidate_MAX_SIZE];
    uint32_t ut_index;
} s_R_OSAL_MmngrInvalidate;


typedef struct {
    osal_memory_buffer_handle_t handle;
    size_t offset;
    size_t size;
    e_osal_return_t ReturnValue[UT_R_OSAL_MmngrFlush_MAX_SIZE];
    uint32_t ut_index;
} s_R_OSAL_MmngrFlush;


typedef struct {
    osal_memory_buffer_handle_t handle;
    osal_axi_bus_id_t axi_id;
    uintptr_t p_physical_adrs;
    uintptr_t p_physical_adrs_in;
    uintptr_t p_physical_adrs_out[UT_R_OSAL_MmngrGetHwAddr_MAX_SIZE];
    e_osal_return_t ReturnValue[UT_R_OSAL_MmngrGetHwAddr_MAX_SIZE];
    uint32_t ut_index;
} s_R_OSAL_MmngrGetHwAddr;


typedef struct {
    osal_memory_buffer_handle_t handle;
    void **p_logical_adrs;
    void **p_logical_adrs_in;
    void **p_logical_adrs_out[UT_R_OSAL_MmngrGetCpuPtr_MAX_SIZE];
    e_osal_return_t ReturnValue[UT_R_OSAL_MmngrGetCpuPtr_MAX_SIZE];
    uint32_t ut_index;
} s_R_OSAL_MmngrGetCpuPtr;


typedef struct {
    osal_memory_buffer_handle_t handle;
    size_t *p_size;
    size_t p_size_in;
    size_t p_size_out[UT_R_OSAL_MmngrGetSize_MAX_SIZE];
    e_osal_return_t ReturnValue[UT_R_OSAL_MmngrGetSize_MAX_SIZE];
    uint32_t ut_index;
} s_R_OSAL_MmngrGetSize;

e_impdrv_errorcode_t impdrv_ut_impctl_set_state(

    st_impdrv_corectl_t         *const p_corectl,
    const e_impdrv_state_t      state

);

extern s_R_OSAL_IoGetNumOfDevices g_R_OSAL_IoGetNumOfDevices;
extern s_R_OSAL_IoGetDeviceList g_R_OSAL_IoGetDeviceList;
extern s_R_OSAL_IoDeviceOpen g_R_OSAL_IoDeviceOpen;
extern s_R_OSAL_PmSetPolicy g_R_OSAL_PmSetPolicy;
extern s_R_OSAL_InterruptGetNumOfIrqChannels g_R_OSAL_InterruptGetNumOfIrqChannels;
extern s_R_OSAL_InterruptUnregisterIsr g_R_OSAL_InterruptUnregisterIsr;
extern s_R_OSAL_InterruptDisableIsr g_R_OSAL_InterruptDisableIsr;
extern s_R_OSAL_IoDeviceClose g_R_OSAL_IoDeviceClose;
extern s_R_OSAL_ThsyncMutexLockForTimePeriod g_R_OSAL_ThsyncMutexLockForTimePeriod;
extern s_R_OSAL_ThsyncMutexCreate g_R_OSAL_ThsyncMutexCreate;
extern s_R_OSAL_ThsyncMutexDestroy g_R_OSAL_ThsyncMutexDestroy;
extern s_R_OSAL_ThsyncMutexUnlock g_R_OSAL_ThsyncMutexUnlock;
extern s_R_OSAL_MqCreate g_R_OSAL_MqCreate;
extern s_R_OSAL_MqReset g_R_OSAL_MqReset;
extern s_R_OSAL_MqDelete g_R_OSAL_MqDelete;
extern s_R_OSAL_MqSendForTimePeriod g_R_OSAL_MqSendForTimePeriod;
extern s_R_OSAL_MqReceiveForTimePeriod g_R_OSAL_MqReceiveForTimePeriod;
extern s_R_OSAL_ThreadCreate g_R_OSAL_ThreadCreate;
extern s_R_OSAL_ThreadJoin g_R_OSAL_ThreadJoin;
extern s_R_OSAL_InterruptEnableIsr g_R_OSAL_InterruptEnableIsr;
extern s_R_OSAL_PmGetState g_R_OSAL_PmGetState;
extern s_R_OSAL_PmGetPolicy g_R_OSAL_PmGetPolicy;
extern s_R_OSAL_PmSetPolicy g_R_OSAL_PmSetPolicy;
extern s_R_OSAL_IoWrite32 g_R_OSAL_IoWrite32;
extern s_R_OSAL_IoRead32 g_R_OSAL_IoRead32;
extern s_R_OSAL_PmSetRequiredState g_R_OSAL_PmSetRequiredState;
extern s_R_OSAL_InterruptRegisterIsr g_R_OSAL_InterruptRegisterIsr;
extern s_R_OSAL_PmSetResetState g_R_OSAL_PmSetResetState;
extern s_R_OSAL_ThsyncMemoryBarrier g_R_OSAL_ThsyncMemoryBarrier;
extern s_R_OSAL_MmngrInvalidate g_R_OSAL_MmngrInvalidate;
extern s_R_OSAL_MmngrFlush g_R_OSAL_MmngrFlush;
extern s_R_OSAL_MmngrGetHwAddr g_R_OSAL_MmngrGetHwAddr;
extern s_R_OSAL_MmngrGetCpuPtr g_R_OSAL_MmngrGetCpuPtr;
extern s_R_OSAL_MmngrGetSize g_R_OSAL_MmngrGetSize;
extern s_R_OSAL_PmWaitForState g_R_OSAL_PmWaitForState;
extern s_R_OSAL_PmGetResetState g_R_OSAL_PmGetResetState;
extern s_R_OSAL_PmGetRequiredState g_R_OSAL_PmGetRequiredState;




extern int32_t g_hook_R_OSAL_IoGetNumOfDevices;
extern int32_t ut_stub_R_OSAL_IoGetNumOfDevices;
extern int32_t g_hook_R_OSAL_IoGetDeviceList;
extern int32_t ut_stub_R_OSAL_IoGetDeviceList;
extern int32_t g_hook_R_OSAL_IoDeviceOpen;
extern int32_t ut_stub_R_OSAL_IoDeviceOpen;
extern int32_t g_hook_R_OSAL_IoDeviceClose;
extern int32_t ut_stub_R_OSAL_IoDeviceClose;
extern int32_t g_hook_R_OSAL_PmSetPolicy;
extern int32_t ut_stub_R_OSAL_PmSetPolicy;
extern int32_t g_hook_R_OSAL_InterruptGetNumOfIrqChannels;
extern int32_t ut_stub_R_OSAL_InterruptGetNumOfIrqChannels;
extern int32_t g_hook_R_OSAL_InterruptUnregisterIsr;
extern int32_t ut_stub_R_OSAL_InterruptUnregisterIsr;
extern int32_t g_hook_R_OSAL_InterruptDisableIsr;
extern int32_t ut_stub_R_OSAL_InterruptDisableIsr;
extern int32_t g_hook_R_OSAL_ThsyncMutexLockForTimePeriod;
extern int32_t ut_stub_R_OSAL_ThsyncMutexLockForTimePeriod;
extern int32_t g_hook_R_OSAL_ThsyncMutexCreate;
extern int32_t ut_stub_R_OSAL_ThsyncMutexCreate;
extern int32_t g_hook_R_OSAL_ThsyncMutexDestroy;
extern int32_t ut_stub_R_OSAL_ThsyncMutexDestroy;
extern int32_t g_hook_R_OSAL_ThsyncMutexUnlock;
extern int32_t ut_stub_R_OSAL_ThsyncMutexUnlock;
extern int32_t g_hook_R_OSAL_MqCreate;
extern int32_t ut_stub_R_OSAL_MqCreate;
extern int32_t g_hook_R_OSAL_MqReset;
extern int32_t ut_stub_R_OSAL_MqReset;
extern int32_t g_hook_R_OSAL_MqDelete;
extern int32_t ut_stub_R_OSAL_MqDelete;
extern int32_t g_hook_R_OSAL_MqSendForTimePeriod;
extern int32_t ut_stub_R_OSAL_MqSendForTimePeriod;
extern int32_t g_hook_R_OSAL_MqReceiveForTimePeriod;
extern int32_t ut_stub_R_OSAL_MqReceiveForTimePeriod;
extern int32_t g_hook_R_OSAL_ThreadCreate;
extern int32_t ut_stub_R_OSAL_ThreadCreate;
extern int32_t g_hook_R_OSAL_ThreadJoin;
extern int32_t ut_stub_R_OSAL_ThreadJoin;
extern int32_t g_hook_R_OSAL_InterruptRegisterIsr;
extern int32_t ut_stub_R_OSAL_InterruptRegisterIsr;
extern int32_t g_hook_R_OSAL_InterruptEnableIsr;
extern int32_t ut_stub_R_OSAL_InterruptEnableIsr;
extern int32_t g_hook_R_OSAL_PmGetState;
extern int32_t ut_stub_R_OSAL_PmGetState;
extern int32_t g_hook_R_OSAL_IoWrite32;
extern int32_t ut_stub_R_OSAL_IoWrite32;
extern int32_t g_hook_R_OSAL_IoRead32;
extern int32_t ut_stub_R_OSAL_IoRead32;
extern int32_t g_hook_R_OSAL_PmSetRequiredState;
extern int32_t ut_stub_R_OSAL_PmSetRequiredState;
extern int32_t g_hook_R_OSAL_PmGetPolicy;
extern int32_t ut_stub_R_OSAL_PmGetPolicy;
extern int32_t g_hook_R_OSAL_MmngrInvalidate;
extern int32_t ut_stub_R_OSAL_MmngrInvalidate;
extern int32_t g_hook_R_OSAL_MmngrFlush;
extern int32_t ut_stub_R_OSAL_MmngrFlush;
extern int32_t g_hook_R_OSAL_MmngrGetHwAddr;
extern int32_t ut_stub_R_OSAL_MmngrGetHwAddr;
extern int32_t g_hook_R_OSAL_MmngrGetCpuPtr;
extern int32_t ut_stub_R_OSAL_MmngrGetCpuPtr;
extern int32_t g_hook_R_OSAL_MmngrGetSize;
extern int32_t ut_stub_R_OSAL_MmngrGetSize;
extern int32_t g_hook_R_OSAL_ThsyncMemoryBarrier;
extern int32_t ut_stub_R_OSAL_ThsyncMemoryBarrier;
extern int32_t g_hook_R_OSAL_PmSetResetState;
extern int32_t ut_stub_R_OSAL_PmSetResetState;
extern int32_t g_hook_R_OSAL_PmWaitForState;
extern int32_t ut_stub_R_OSAL_PmWaitForState;
extern int32_t g_hook_R_OSAL_PmGetResetState;
extern int32_t ut_stub_R_OSAL_PmGetResetState;
extern int32_t g_hook_R_OSAL_PmGetRequiredState;
extern int32_t ut_stub_R_OSAL_PmGetRequiredState;
extern uint32_t g_p_uts_p_logical_adrs;
extern uint32_t g_p_logical_adrs[128];
extern st_impdrv_corectl_func_t *g_p_p_funcs;
extern st_impdrv_corectl_func_t *g_uts_p_funcs[22];
extern void* g_p_uts_p_funcs;
extern st_impdrv_corectl_func_t g_p_core_init_end;
extern uintptr_t *g_p_physical_adrs;
extern size_t g_uts_constp_size;
extern uint32_t g_uts_p_receive_buffer;
extern uint32_t g_uts_device_io_imptop;
extern uint32_t g_uts_p_work_size;
extern s_impdrv_cnnctl_get_inten_val g_impdrv_cnnctl_get_inten_val;
extern s_impdrv_pscctl_get_inten_val g_impdrv_pscctl_get_inten_val;
extern s_impdrv_ocvctl_get_inten_val g_impdrv_ocvctl_get_inten_val;

extern s_impdrv_ocvctl_set_state g_impdrv_ocvctl_set_state;
extern s_impdrv_osdep_pow_on_imp g_impdrv_osdep_pow_on_imp;
extern s_impdrv_cnnctl_set_state g_impdrv_cnnctl_set_state;
extern s_impdrv_pscctl_set_state g_impdrv_pscctl_set_state;

extern s_impdrv_dmactl_get_inten_val g_impdrv_dmactl_get_inten_val;
extern s_impdrv_impctl_get_inten_val g_impdrv_impctl_get_inten_val;
extern s_impdrv_cnnctl_check_inten g_impdrv_cnnctl_check_inten;
extern s_impdrv_impctl_set_state g_impdrv_impctl_set_state;
extern s_impdrv_dspctl_set_state g_impdrv_dspctl_set_state;
extern s_impdrv_dmactl_set_state g_impdrv_dmactl_set_state;
extern s_impdrv_genctl_init_set_state g_impdrv_genctl_init_set_state;
extern s_impdrv_genctl_dsp_int_handler g_impdrv_genctl_dsp_int_handler;
extern s_impdrv_osdep_memory_data_set g_impdrv_osdep_memory_data_set;
extern s_impdrv_cmnctl_set_checksum g_impdrv_cmnctl_set_checksum;
extern s_impdrv_cmnctl_test_checksum g_impdrv_cmnctl_test_checksum;


extern s_impdrv_cmnctl_cnv_int_bit_core g_impdrv_cmnctl_cnv_int_bit_core;
extern s_impdrv_cmnctl_cnv_int_core_bit g_impdrv_cmnctl_cnv_int_core_bit;
extern s_impdrv_cmnctl_get_group g_impdrv_cmnctl_get_group;

extern s_impdrv_cmnctl_get_corectl_func g_impdrv_cmnctl_get_corectl_func;
extern s_impdrv_cmnctl_get_init_param g_impdrv_cmnctl_get_init_param;
extern s_impdrv_cmnctl_get_dsp_func g_impdrv_cmnctl_get_dsp_func;
extern s_impdrv_genctl_chk_state_init g_impdrv_genctl_chk_state_init;
extern s_impdrv_cmnctl_calc_checksum g_impdrv_cmnctl_calc_checksum;
extern s_impdrv_cmnctl_reg_set_mem_protect g_impdrv_cmnctl_reg_set_mem_protect;
extern s_impdrv_cmnctl_reg_find_resource g_impdrv_cmnctl_reg_find_resource;
extern s_impdrv_cmnctl_get_io_imptop g_impdrv_cmnctl_get_io_imptop;
extern s_impdrv_cmnctl_judge_int g_impdrv_cmnctl_judge_int;
extern st_impdrv_corectl_func_t g_p_funcs_null;
extern st_impdrv_corectl_func_t *g_p_p_f_funcs_null_v2;
extern st_impdrv_commonctl_t g_uts_p_commonctl;
extern st_impdrv_corectl_func_t g_p_funcs_error;
extern st_impdrv_corectl_func_t g_p_funcs_dsp;
extern st_impdrv_corectl_func_t g_p_funcs_dsp_start;
extern st_impdrv_corectl_func_t g_p_p_funcs_null;
extern st_impdrv_corectl_func_t g_p_p_funcs_normal;
extern st_impdrv_corectl_func_t g_p_p_funcs_core_null;
extern st_impdrv_corectl_func_t g_p_p_funcs_core_invalid;
extern st_impdrv_corectl_func_t g_p_p_f_funcs_null;
extern st_impdrv_corectl_func_t g_p_p_end_null;
extern st_impdrv_corectl_func_t g_p_p_state_null;
extern st_impdrv_corectl_func_t g_p_p_attr_null;
extern s_impdrv_udefctl_udef_crc_val g_impdrv_udefctl_udef_crc;
extern s_impdrv_udefctl_set_ecm g_impdrv_udefctl_set_ecm;

extern e_osal_return_t R_OSAL_IoGetNumOfDevices(const char* deviceType, size_t* const p_numOfDevices);
extern e_osal_return_t R_OSAL_IoGetDeviceList(const char* device_type_name, char* p_buffer, size_t buffer_size, size_t* const p_num_of_byte);
extern e_osal_return_t R_OSAL_IoDeviceOpen(const char* id, osal_device_handle_t* const p_handle);
extern e_osal_return_t R_OSAL_IoDeviceClose(osal_device_handle_t handle);
extern e_osal_return_t R_OSAL_PmSetPolicy(osal_device_handle_t handle,e_osal_pm_policy_t policy,bool applyImmediate);
extern e_osal_return_t R_OSAL_InterruptGetNumOfIrqChannels(osal_device_handle_t handle,size_t* const p_numOfChannels);
extern e_osal_return_t R_OSAL_InterruptUnregisterIsr(osal_device_handle_t handle,uint64_t irq_channel,p_osal_isr_func_t irqHandlerFunction);
extern e_osal_return_t R_OSAL_InterruptDisableIsr(osal_device_handle_t handle,uint64_t irq_channel);
extern e_osal_return_t R_OSAL_ThsyncMutexLockForTimePeriod(osal_mutex_handle_t handle,osal_milli_sec_t time_period);
extern e_osal_return_t R_OSAL_ThsyncMutexCreate(osal_mutex_id_t mutex_id,osal_mutex_handle_t* const p_handle);
extern e_osal_return_t R_OSAL_ThsyncMutexDestroy(osal_mutex_handle_t handle);
extern e_osal_return_t R_OSAL_ThsyncMutexUnlock(osal_mutex_handle_t handle);
extern e_osal_return_t R_OSAL_MqCreate(const st_osal_mq_config_t *const p_config, osal_mq_id_t mq_Id,  osal_mq_handle_t *const p_handle);
extern e_osal_return_t R_OSAL_MqReset(osal_mq_handle_t handle);
extern e_osal_return_t R_OSAL_MqDelete(osal_mq_handle_t handle);
extern e_osal_return_t R_OSAL_MqSendForTimePeriod(osal_mq_handle_t handle, osal_milli_sec_t time_period, const void * p_buffer, size_t buffer_size);
extern e_osal_return_t R_OSAL_MqReceiveForTimePeriod(osal_mq_handle_t handle, osal_milli_sec_t time_period, void *p_buffer, size_t buffer_size);
extern e_osal_return_t R_OSAL_ThreadCreate(const st_osal_thread_config_t* const p_config, osal_thread_id_t thread_id, osal_thread_handle_t* const p_handle);
extern e_osal_return_t R_OSAL_ThreadJoin(osal_thread_handle_t handle, int64_t *const p_return_value);
extern e_osal_return_t R_OSAL_InterruptRegisterIsr(osal_device_handle_t handle, uint64_t irq_channel, e_osal_interrupt_priority_t priority_value, p_osal_isr_func_t irqHandlerFunction, void* p_irq_arg);
extern e_osal_return_t R_OSAL_InterruptEnableIsr(osal_device_handle_t handle, uint64_t irq_channel);
extern e_osal_return_t R_OSAL_PmGetState(osal_device_handle_t handle, e_osal_pm_state_t * const p_state);
extern e_osal_return_t R_OSAL_PmGetPolicy(osal_device_handle_t handle, e_osal_pm_policy_t* const p_policy);
extern e_osal_return_t R_OSAL_IoWrite32(osal_device_handle_t handle, uintptr_t offset, uint32_t data);
extern e_osal_return_t R_OSAL_IoRead32(osal_device_handle_t handle, uintptr_t offset, uint32_t * const p_data);
extern e_osal_return_t R_OSAL_PmSetRequiredState(osal_device_handle_t handle, e_osal_pm_required_state_t state, bool applyImmediate);
extern e_osal_return_t R_OSAL_PmGetRequiredState(osal_device_handle_t handle, e_osal_pm_required_state_t* const p_state);
extern e_osal_return_t R_OSAL_MmngrInvalidate(osal_memory_buffer_handle_t handle, uintptr_t offset, size_t size);
extern e_osal_return_t R_OSAL_MmngrFlush(osal_memory_buffer_handle_t handle, size_t offset, size_t size);
extern e_osal_return_t R_OSAL_MmngrGetHwAddr(osal_memory_buffer_handle_t handle, osal_axi_bus_id_t axi_id, uintptr_t *p_physical_adrs);
extern e_osal_return_t R_OSAL_MmngrGetCpuPtr(osal_memory_buffer_handle_t handle, void **p_logical_adrs);
extern e_osal_return_t R_OSAL_MmngrGetSize(osal_memory_buffer_handle_t handle, size_t *p_size);
extern e_osal_return_t R_OSAL_PmWaitForState(osal_device_handle_t handle, e_osal_pm_state_t state, osal_milli_sec_t time_period);
extern e_osal_return_t R_OSAL_PmGetResetState(osal_device_handle_t handle, e_osal_pm_reset_state_t* const p_state);

void ut_init_imp(bool enabled_real);
void ut_deinit_imp(void);

extern st_impdrv_ctl_t g_uts_handle;
extern st_impdrv_ctl_t g_p_uts_p_handle;
extern st_impdrv_ctl_t g_uts_p_impdrv_ctl;
extern st_impdrv_ctl_t *g_p_uts_p_impdrv_ctl;
extern bool g_uts_verify_flag;
extern int32_t g_uts_msg_data;
extern uint32_t g_uts_value;
extern osal_device_handle_t g_uts_device_handle;
extern uint32_t g_uts_state;
extern int32_t g_uts_end_ch;
extern int32_t g_uts_start_ch;
extern int32_t g_uts_p_osdep_work;
extern int32_t g_uts_task_handle;
extern int32_t g_uts_msg_handle;
extern st_impdrv_ctl_t g_p_main_work_addr;
extern st_impdrv_ctl_t g_p_main_work_addr_invalid;
extern int32_t g_p_attr_work_addr;
extern int32_t g_p_queue_work_addr;
extern int32_t g_uts_func;
extern int32_t g_uts_userarg;
extern int32_t g_uts_task_name;
extern int32_t g_uts_p_msg;
extern int32_t g_uts_callback_func;
extern int32_t g_uts_osal_cb_args;
extern int32_t g_uts_p_val;
extern int32_t g_uts_s_inten_cnv_table;
extern int32_t g_uts_cbfunc_fatal;
extern int32_t g_uts_p_cb_args_fatal;
extern int32_t g_uts_p_device_handle;
extern int32_t g_uts_p_callback_args;
extern int32_t g_p_handle;
extern int32_t g_p_uts_p_osal_cb_args;
extern int32_t g_uts_mutex_handle;
extern int32_t g_uts_dev_handle;
extern int32_t g_uts_policy;
extern int32_t g_uts_p_pmstate;
extern int32_t g_uts_p_use_core_num;
extern int32_t g_uts_p_init_core_num;
extern int32_t g_uts_p_inten_group_table;
extern uint8_t g_uts_syncc_val[16];
extern int32_t g_uts_irq_channel;
extern st_impdrv_ctl_t g_uts_irq_arg;
extern int32_t g_uts_p_pm_policy;
extern int32_t g_uts_p_hwrsc_def_table;
extern int32_t g_uts_p_hwrsc_mng_table;
extern int32_t g_uts_core_info;
extern st_impdrv_coretypectl_t g_uts_p_coretypectl;
extern int32_t g_uts_p_policy;
extern bool g_uts_p_is_dmac_mb_init;
extern int32_t g_uts_p_data;
extern char g_uts_p_device_id;
extern st_impdrv_device_handle_t g_uts_p_device_io[8];
extern int32_t g_uts_device_io;
extern bool g_uts_p_is_dmac_mb_init;
extern char g_uts_p_rsc_name;
extern char g_uts_p_reg_info;
extern char g_uts_p_index;
extern char g_uts_p_resource;
extern st_impdrv_corectl_t g_uts_p_corectl;
extern char g_uts_irq_mask;
extern e_impdrv_irq_group_t  g_uts_p_irq_group;
extern e_osal_interrupt_priority_t g_uts_p_irq_priority;
extern st_impdrv_device_handle_t g_uts_p_handle;
extern st_impdrv_dsp_data_t g_uts_p_dsp_app;
extern st_impdrv_dsp_data_t g_uts_p_dsp_fw;
extern st_impdrv_core_info_t g_uts_p_core_info;
extern st_impdrv_core_info_t g_uts_init_core_info[22];
extern st_impdrv_dsp_data_t g_uts_p_dsp_data;
extern st_impdrv_dsp_data_t g_uts_p_dsp_dtcm;
extern st_impdrv_hwrsc_mng_t g_hwrsc_mng_table;
extern uint32_t g_uts_p_use_core_info;
extern st_impdrv_hwrsc_def_t s_hwrsc_def_table;
extern uint32_t g_uts_p_mask_val;
extern e_impdrv_cb_ret_t g_uts_cb_ercd[18];
extern bool g_uts_p_is_check_intclear;
extern uint32_t g_uts_p_cb_ercd_num;
extern e_impdrv_state_t g_uts_p_state;
extern uintptr_t g_uts_p_physical_adrs;
extern uint32_t g_uts_constp_cb_ercd_num;
extern bool g_uts_constp_is_check_intclear;
extern e_impdrv_state_t g_uts_constp_state;
extern uint32_t g_uts_p_checksum;
extern uint32_t g_uts_constp_mask_val;
extern uint32_t g_p_uts_p_func_array;
extern st_impdrv_device_handle_t   device_io_imptop;
extern st_impdrv_chk_resource_t g_uts_p_chk_resource;
extern st_impdrv_ctl_t g_uts_R_OSAL_IoRead32;
extern st_impdrv_ctl_t g_uts_R_OSAL_IoWrite32;
extern st_impdrv_chk_param_t g_uts_p_chk_param;
extern size_t g_uts_constp_size;
extern st_impdrv_ctl_t *g_p_ut_p_impdrv_ctl;
extern st_impdrv_corectl_func_t* g_p_funcs;

extern const st_impdrv_corectl_func_t *ut_stub_impdrv_cmnctl_get_corectl_func(const e_impdrv_core_type_t core_type);
extern p_impdrv_cmnctl_get_corectl_func_stub g_hook_impdrv_cmnctl_get_corectl_func;

extern e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_get_init_param(
    const st_impdrv_commonctl_t *const p_commonctl,
    e_osal_interrupt_priority_t *const p_irq_priority,
    impdrv_ctrl_handle_t        *const p_osal_cb_args
);
extern p_impdrv_cmnctl_get_init_param_stub g_hook_impdrv_cmnctl_get_init_param;

extern p_impdrv_dspctl_dsp_start_t ut_stub_impdrv_cmnctl_get_dsp_func(void);
extern p_impdrv_cmnctl_get_dsp_func_stub g_hook_impdrv_cmnctl_get_dsp_func;

extern e_impdrv_errorcode_t ut_stub_impdrv_genctl_chk_state_init(
    const st_impdrv_ctl_t               *const p_impdrv_ctl,
    const st_impdrv_corectl_func_t      *p_funcs[IMPDRV_CORE_TYPE_MAX],
    st_impdrv_core_info_t               init_core_info[IMPDRV_INNER_FIXED_VALUE],
    uint32_t                            *const p_init_core_num
);
extern p_impdrv_genctl_chk_state_init_stub g_hook_impdrv_genctl_chk_state_init;

// const st_impdrv_corectl_func_t *ut_stub_impdrv_cmnctl_get_corectl_func(const e_impdrv_core_type_t core_type);
// const st_impdrv_corectl_func_t *impdrv_cmnctl_get_corectl_func_stub(const e_impdrv_core_type_t core_type);

extern e_impdrv_errorcode_t ut_stub_impdrv_genctl_init_set_state(st_impdrv_ctl_t *const p_impdrv_ctl, const uint32_t use_core_num, const st_impdrv_core_info_t *const p_core_info, const st_impdrv_corectl_func_t **const p_funcs);
extern p_impdrv_genctl_init_set_state_stub g_hook_impdrv_genctl_init_set_state;

e_impdrv_errorcode_t ut_stub_impdrv_genctl_init_set_state(st_impdrv_ctl_t *const p_impdrv_ctl, const uint32_t use_core_num, const st_impdrv_core_info_t *const p_core_info, const st_impdrv_corectl_func_t **const p_funcs);
e_impdrv_errorcode_t impdrv_genctl_init_set_state_stub(st_impdrv_ctl_t *const p_impdrv_ctl, const uint32_t use_core_num, const st_impdrv_core_info_t *const p_core_info, const st_impdrv_corectl_func_t **const p_funcs);

extern const uint32_t ut_stub_impdrv_ocvctl_get_inten_val(const bool irq_mask[IMPDRV_IRQMASK_MAX]);

extern const e_impdrv_errorcode_t ut_stub_impdrv_ocvctl_set_state(st_impdrv_corectl_t   *const p_corectl,const e_impdrv_state_t  state);
extern p_impdrv_ocvctl_get_inten_val g_hook_impdrv_ocvctl_get_inten_val;

extern p_impdrv_ocvctl_set_state g_hook_impdrv_ocvctl_set_state;


extern e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_set_checksum(void);
extern p_impdrv_cmnctl_set_checksum_stub g_hook_impdrv_cmnctl_set_checksum;

e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_set_checksum(void);
e_impdrv_errorcode_t impdrv_cmnctl_set_checksum_stub(void);

extern e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_test_checksum(void);
extern p_impdrv_cmnctl_test_checksum_stub g_hook_impdrv_cmnctl_test_checksum;

e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_test_checksum(void);
e_impdrv_errorcode_t impdrv_cmnctl_test_checksum_stub(void);

extern e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_cnv_int_bit_core(const st_impdrv_inten_tbl_t *const p_s_inten_cnv_table, const uint32_t cnv_table_num, const uint32_t stat, st_impdrv_core_info_t *core_info, uint32_t *const p_use_core_num);
extern p_impdrv_cmnctl_cnv_int_bit_core_stub g_hook_impdrv_cmnctl_cnv_int_bit_core;
extern e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_cnv_int_core_bit(const st_impdrv_core_info_t *const p_core_info, const uint32_t use_core_num, const st_impdrv_inten_tbl_t const* p_inten_cnv_table, const uint32_t cnv_table_num, uint32_t *const p_val);
extern p_impdrv_cmnctl_cnv_int_core_bit_stub g_hook_impdrv_cmnctl_cnv_int_core_bit;
extern e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_get_group(e_impdrv_irq_group_t *const p_irq_group, const e_impdrv_instance_t instance_num);
extern p_impdrv_cmnctl_get_group_stub g_hook_impdrv_cmnctl_get_group;

e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_cnv_int_bit_core(const st_impdrv_inten_tbl_t *const p_s_inten_cnv_table, const uint32_t cnv_table_num, const uint32_t stat, st_impdrv_core_info_t *core_info, uint32_t *const p_use_core_num);
e_impdrv_errorcode_t impdrv_cmnctl_cnv_int_bit_core_stub(const st_impdrv_inten_tbl_t *const p_s_inten_cnv_table, const uint32_t cnv_table_num, const uint32_t stat, st_impdrv_core_info_t *core_info, uint32_t *const p_use_core_num);

extern const e_impdrv_errorcode_t ut_stub_impdrv_osdep_pow_on_imp(st_impdrv_device_handle_t   *const p_device_handle,const st_impdrv_core_info_t core_info);
extern p_impdrv_osdep_pow_on_imp g_hook_impdrv_osdep_pow_on_imp;
const e_impdrv_errorcode_t ut_stub_impdrv_osdep_pow_on_imp(st_impdrv_device_handle_t   *const p_device_handle,const st_impdrv_core_info_t core_info);
const e_impdrv_errorcode_t impdrv_osdep_pow_on_imp_stub(st_impdrv_device_handle_t   *const p_device_handle,const st_impdrv_core_info_t core_info);

extern const e_impdrv_errorcode_t ut_stub_impdrv_genctl_dsp_int_handler(st_impdrv_ctl_t *const p_impdrv_ctl, const uint64_t irq_channel, const e_impdrv_errorcode_t ercd, const st_impdrv_core_info_t core_info);
extern p_impdrv_genctl_dsp_int_handle g_hook_impdrv_genctl_dsp_int_handler;
const e_impdrv_errorcode_t ut_stub_impdrv_genctl_dsp_int_handler(st_impdrv_ctl_t *const p_impdrv_ctl, const uint64_t irq_channel, const e_impdrv_errorcode_t ercd, const st_impdrv_core_info_t core_info);
const e_impdrv_errorcode_t impdrv_genctl_dsp_int_handler_stub(st_impdrv_ctl_t *const p_impdrv_ctl, const uint64_t irq_channel, const e_impdrv_errorcode_t ercd, const st_impdrv_core_info_t core_info);

extern const uint32_t ut_stub_impdrv_cnnctl_get_inten_val(const bool irq_mask[IMPDRV_IRQMASK_MAX]);
extern p_impdrv_cnnctl_get_inten_val g_hook_impdrv_cnnctl_get_inten_val;

const uint32_t ut_stub_impdrv_cnnctl_get_inten_val(const bool irq_mask[IMPDRV_IRQMASK_MAX]);
const uint32_t impdrv_cnnctl_get_inten_val_stub(const bool irq_mask[IMPDRV_IRQMASK_MAX]);

const uint32_t ut_stub_impdrv_ocvctl_get_inten_val(const bool irq_mask[IMPDRV_IRQMASK_MAX]);
const uint32_t impdrv_ocvctl_get_inten_val_stub(const bool irq_mask[IMPDRV_IRQMASK_MAX]);
const e_impdrv_errorcode_t ut_stub_impdrv_ocvctl_set_state(st_impdrv_corectl_t   *const p_corectl,const e_impdrv_state_t  state);
const e_impdrv_errorcode_t impdrv_ocvctl_set_state_stub(st_impdrv_corectl_t   *const p_corectl,const e_impdrv_state_t  state);

extern e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_calc_checksum(const void* *const p_data,const size_t size,uint32_t *const p_checksum);
extern p_impdrv_cmnctl_calc_checksum_stub g_hook_impdrv_cmnctl_calc_checksum; 

e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_calc_checksum(const void* *const p_data,const size_t size,uint32_t *const p_checksum);
e_impdrv_errorcode_t impdrv_cmnctl_calc_checksum_stub(const void* *const p_data,const size_t size,uint32_t *const p_checksum);
extern const uint32_t ut_stub_impdrv_pscctl_get_inten_val(const bool irq_mask[IMPDRV_IRQMASK_MAX]);
extern p_impdrv_pscctl_get_inten_val g_hook_impdrv_pscctl_get_inten_val;

const uint32_t ut_stub_impdrv_pscctl_get_inten_val(const bool irq_mask[IMPDRV_IRQMASK_MAX]);
const uint32_t impdrv_pscctl_get_inten_val_stub(const bool irq_mask[IMPDRV_IRQMASK_MAX]);


extern const e_impdrv_errorcode_t ut_stub_impdrv_cnnctl_set_state(st_impdrv_corectl_t   *const p_corectl,const e_impdrv_state_t  state);
extern p_impdrv_cnnctl_set_state g_hook_impdrv_cnnctl_set_state;

const e_impdrv_errorcode_t ut_stub_impdrv_cnnctl_set_state(st_impdrv_corectl_t   *const p_corectl,const e_impdrv_state_t  state);
const e_impdrv_errorcode_t impdrv_cnnctl_set_state_stub(st_impdrv_corectl_t   *const p_corectl,const e_impdrv_state_t  state);

extern const e_impdrv_errorcode_t ut_stub_impdrv_pscctl_set_state(st_impdrv_corectl_t   *const p_corectl,const e_impdrv_state_t  state);
extern p_impdrv_pscctl_set_state g_hook_impdrv_pscctl_set_state;

const e_impdrv_errorcode_t ut_stub_impdrv_pscctl_set_state(st_impdrv_corectl_t   *const p_corectl,const e_impdrv_state_t  state);
const e_impdrv_errorcode_t impdrv_pscctl_set_state_stub(st_impdrv_corectl_t   *const p_corectl,const e_impdrv_state_t  state);

extern const uint32_t ut_stub_impdrv_dmactl_get_inten_val(const bool irq_mask[IMPDRV_IRQMASK_MAX]);
extern p_impdrv_dmactl_get_inten_val g_hook_impdrv_dmactl_get_inten_val;

const uint32_t ut_stub_impdrv_dmactl_get_inten_val(const bool irq_mask[IMPDRV_IRQMASK_MAX]);
const uint32_t impdrv_dmactl_get_inten_val_stub(const bool irq_mask[IMPDRV_IRQMASK_MAX]);

extern const uint32_t ut_stub_impdrv_impctl_get_inten_val(const bool irq_mask[IMPDRV_IRQMASK_MAX]);
extern p_impdrv_impctl_get_inten_val g_hook_impdrv_impctl_get_inten_val;

extern const e_impdrv_errorcode_t ut_stub_impdrv_cnnctl_check_inten(
    const uint32_t          stat_val,
    uint32_t                *const p_mask_val,
    e_impdrv_cb_ret_t       cb_ercd[IMPDRV_CB_RET_MAX],
    uint32_t                *const p_cb_ercd_num,
    bool                    *const p_is_check_intclear,
    e_impdrv_state_t        *const p_state
);
extern p_impdrv_cnnctl_check_inten g_hook_impdrv_cnnctl_check_inten;
const e_impdrv_errorcode_t ut_stub_impdrv_cnnctl_check_inten(const uint32_t stat_val, uint32_t *const p_mask_val, e_impdrv_cb_ret_t cb_ercd[IMPDRV_CB_RET_MAX], uint32_t *const p_cb_ercd_num, bool *const p_is_check_intclear, e_impdrv_state_t *const p_state);
const e_impdrv_errorcode_t impdrv_cnnctl_check_inten_stub(
    const uint32_t          stat_val,
    uint32_t                *const p_mask_val,
    e_impdrv_cb_ret_t       cb_ercd[IMPDRV_CB_RET_MAX],
    uint32_t                *const p_cb_ercd_num,
    bool                    *const p_is_check_intclear,
    e_impdrv_state_t        *const p_state
);

extern const e_impdrv_errorcode_t ut_stub_impdrv_impctl_set_state(st_impdrv_corectl_t   *const p_corectl,const e_impdrv_state_t  state);
extern p_impdrv_impctl_set_state g_hook_impdrv_impctl_set_state;

extern const e_impdrv_errorcode_t ut_stub_impdrv_dspctl_set_state(st_impdrv_corectl_t   *const p_corectl,const e_impdrv_state_t  state);
extern p_impdrv_dspctl_set_state g_hook_impdrv_dspctl_set_state;
const e_impdrv_errorcode_t impdrv_dspctl_set_state_stub(st_impdrv_corectl_t   *const p_corectl,const e_impdrv_state_t  state);

const e_impdrv_errorcode_t ut_stub_impdrv_impctl_set_state(st_impdrv_corectl_t   *const p_corectl,const e_impdrv_state_t  state);
const e_impdrv_errorcode_t impdrv_impctl_set_state_stub(st_impdrv_corectl_t   *const p_corectl,const e_impdrv_state_t  state);
extern const e_impdrv_errorcode_t ut_stub_impdrv_osdep_memory_data_set(uint32_t *const p_buffer,const uint32_t  data,const size_t size);
extern p_impdrv_osdep_memory_data_set g_hook_impdrv_osdep_memory_data_set;

const e_impdrv_errorcode_t ut_stub_impdrv_osdep_memory_data_set(uint32_t *const p_buffer,const uint32_t  data,const size_t size);
const e_impdrv_errorcode_t impdrv_osdep_memory_data_set_stub(uint32_t *const p_buffer,const uint32_t  data,const size_t size);

extern const e_impdrv_errorcode_t ut_stub_impdrv_dmactl_set_state(st_impdrv_corectl_t   *const p_corectl,const e_impdrv_state_t  state);
extern p_impdrv_dmactl_set_state g_hook_impdrv_dmactl_set_state;

const e_impdrv_errorcode_t ut_stub_impdrv_dmactl_set_state(st_impdrv_corectl_t   *const p_corectl,const e_impdrv_state_t  state);
const e_impdrv_errorcode_t impdrv_dmactl_set_state_stub(st_impdrv_corectl_t   *const p_corectl,const e_impdrv_state_t  state);

extern e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_reg_set_mem_protect(st_impdrv_commonctl_t *const p_commonctl, const e_impdrv_protect_mode_t protect_mode);
extern p_impdrv_cmnctl_reg_set_mem_protect g_hook_impdrv_cmnctl_reg_set_mem_protect;

e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_reg_set_mem_protect(st_impdrv_commonctl_t *const p_commonctl, const e_impdrv_protect_mode_t protect_mode);
e_impdrv_errorcode_t impdrv_cmnctl_reg_set_mem_protect_stub(st_impdrv_commonctl_t *const p_commonctl, const e_impdrv_protect_mode_t protect_mode);

extern e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_reg_find_resource(const char *const p_device_id, uint32_t *const p_index, st_impdrv_hwrsc_def_t *const p_resource);
extern p_impdrv_cmnctl_reg_find_resource_stub g_hook_impdrv_cmnctl_reg_find_resource;

e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_reg_find_resource(const char *const p_device_id, uint32_t *const p_index, st_impdrv_hwrsc_def_t *const p_resource);
e_impdrv_errorcode_t impdrv_cmnctl_reg_find_resource_stub(const char *const p_device_id, uint32_t *const p_index, st_impdrv_hwrsc_def_t *const p_resource);

extern e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_get_io_imptop(st_impdrv_device_handle_t *const p_handle);
extern p_impdrv_cmnctl_get_io_imptop_stub g_hook_impdrv_cmnctl_get_io_imptop;

e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_get_io_imptop(st_impdrv_device_handle_t *const p_handle);
e_impdrv_errorcode_t impdrv_cmnctl_get_io_imptop_stub(st_impdrv_device_handle_t *const p_handle);

extern e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_judge_int(const st_impdrv_commonctl_t *const p_commonctl,const uint64_t irq_channel,st_impdrv_core_info_t core_info[IMPDRV_INTSTS_BITNUM], uint32_t *const p_use_core_num, bool is_fb_callback);
extern p_impdrv_cmnctl_judge_int_stub g_hook_impdrv_cmnctl_judge_int;

e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_judge_int(const st_impdrv_commonctl_t *const p_commonctl,const uint64_t irq_channel,st_impdrv_core_info_t core_info[IMPDRV_INTSTS_BITNUM], uint32_t *const p_use_core_num, bool is_fb_callback);
e_impdrv_errorcode_t impdrv_cmnctl_judge_int_stub(const st_impdrv_commonctl_t *const p_commonctl,const uint64_t irq_channel,st_impdrv_core_info_t core_info[IMPDRV_INTSTS_BITNUM], uint32_t *const p_use_core_num, bool is_fb_callback);
p_impdrv_dspctl_dsp_start_t impdrv_cmnctl_get_dsp_func_stub(void);

extern uint32_t  impdrv_udefctl_udef_crc_stub(uint32_t st, uint32_t sz );
extern uint32_t ut_stub_impdrv_udefctl_udef_crc(uint32_t st, uint32_t sz );
uint32_t ut_stub_impdrv_udefctl_udef_crc(uint32_t st, uint32_t sz );
extern p_impdrv_udefctl_udef_crc_val g_hook_impdrv_udefctl_udef_crc;
extern p_impdrv_udefctl_set_ecm g_hook_impdrv_udefctl_set_ecm;
extern char g_uts_device_id_ch0[32];
extern char g_uts_device_id_ch1[32];
extern char g_uts_device_id_ch2[32];
extern char g_uts_device_id_ch3[32];
extern char g_uts_device_id_ch4[32];
extern char g_uts_device_id_ch5[32];
extern int32_t g_uts_exe_param;
extern st_impdrv_device_handle_t *g_p_uts_p_device_io[8];
extern uint32_t index;
extern e_impdrv_state_t state;
extern bool null_flag;
extern char g_uts_name[32];
extern uint32_t st;
extern uint32_t sz;

#endif /* _UT_OSAL_STUB_H */
