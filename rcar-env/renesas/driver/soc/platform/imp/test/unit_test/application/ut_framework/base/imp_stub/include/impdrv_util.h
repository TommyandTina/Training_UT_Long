#ifndef _IMPDRV_UTIL_H
#define _IMPDRV_UTIL_H

#include "ut_osal_stub.h"

void CbFunc(st_impdrv_core_info_t *core_info, e_impdrv_fatalcode_t code, e_impdrv_errorcode_t ercd, void* p_callback_args);
bool uninit_data_st_impdrv_initdata_t(st_impdrv_initdata_t *p_initdata);
bool init_data_st_impdrv_initdata_t(st_impdrv_initdata_t *p_initdata);
bool null_callback_func_st_impdrv_initdata_t(st_impdrv_initdata_t *p_initdata);
bool invalid_core_type_dta_st_impdrv_initdata_t(st_impdrv_initdata_t *p_initdata);
bool uninitialized_impdrv_ctrl_handle_t(st_impdrv_ctl_t* p_handle);
bool initialized_impdrv_ctrl_handle_t(st_impdrv_ctl_t* p_handle);
bool exec_impdrv_ctrl_handle_t(st_impdrv_ctl_t* p_handle);
bool ready_impdrv_ctrl_handle_t(st_impdrv_ctl_t* p_handle);
bool int_impdrv_ctrl_handle_t(st_impdrv_ctl_t* p_handle);
bool init_cl_addr_impdrv_ctrl_handle_t(st_impdrv_ctl_t* p_handle);
bool invalid_mutex_period_impdrv_ctrl_handle_t(st_impdrv_ctl_t* p_handle);
bool invalid_irq_mask_impdrv_ctrl_handle_t(st_impdrv_ctl_t* p_handle);
bool uninitialize_p_coretypectl(st_impdrv_coretypectl_t *p_coretypectl, uint32_t* core_num);
bool initialize_p_coretypectl(st_impdrv_coretypectl_t *p_coretypectl, uint32_t* core_num);
bool initialize_p_coretypectl_null_handle(st_impdrv_coretypectl_t *p_coretypectl, uint32_t* core_num);
bool initialize_p_coretypectl_param_off(st_impdrv_coretypectl_t *p_coretypectl, uint32_t* core_num);
bool ready_p_coretypectl_param_off(st_impdrv_coretypectl_t *p_coretypectl, uint32_t* core_num);
bool ready_p_coretypectl(st_impdrv_coretypectl_t *p_coretypectl, uint32_t* core_num);
bool ready_st_impdrv_commonctl_chk_t();
bool init_funcs_st_impdrv_corectl_func_t(st_impdrv_corectl_func_t ***p_funcs);
bool impdrv_ctl_st_impdrv_ctl_t(st_impdrv_ctl_t *p_impdrv_ctl);
bool unint_impdrv_ctl_st_impdrv_ctl_t(st_impdrv_ctl_t *p_impdrv_ctl);
bool ready_impdrv_ctl_st_impdrv_ctl_t(st_impdrv_ctl_t *p_impdrv_ctl);
bool ready1_impdrv_ctrl_handle_t(st_impdrv_ctl_t* p_handle);
bool pfuncinitialized_impdrv_ctrl_handle_t(st_impdrv_ctl_t *p_handle);
bool notreadyimpdrv_ctl_st_impdrv_ctl_t(st_impdrv_ctl_t *p_impdrv_ctl);
bool stateinitialized_impdrv_ctrl_handle_t(st_impdrv_ctl_t *p_handle);
bool instance_impdrv_ctl_st_impdrv_ctl_t(st_impdrv_ctl_t *p_impdrv_ctl);
bool instancehandle_impdrv_ctl_st_impdrv_ctl_t(st_impdrv_ctl_t *p_impdrv_ctl);
bool stateuninitialized_impdrv_ctrl_handle_t(st_impdrv_ctl_t *p_handle);
bool chkinitialized_impdrv_ctrl_handle_t(st_impdrv_ctl_t *handle);
bool pimpinit_impdrv_ctl_st_impdrv_ctl_t(st_impdrv_ctl_t *p_impdrv_ctl);
bool null_funcs_st_impdrv_corectl_func_t(st_impdrv_corectl_func_t ***p_funcs );
bool null2_funcs_st_impdrv_corectl_func_t(st_impdrv_corectl_func_t ***p_funcs );

bool pfunc1initialized_impdrv_ctrl_handle_t(st_impdrv_ctl_t *p_handle);
bool pimpinit1_impdrv_ctl_st_impdrv_ctl_t(st_impdrv_ctl_t *p_impdrv_ctl);
bool uninit_funcs_st_impdrv_corectl_func_t(st_impdrv_corectl_func_t ***p_funcs);
bool initialize_p_coretypectl_param_on(st_impdrv_coretypectl_t *p_coretypectl, uint32_t* core_num);
bool initialized_impdrv_ctrl_handle_t1(st_impdrv_ctl_t *p_handle);
bool stateinitialized_instance_impdrv_ctrl_handle_t(st_impdrv_ctl_t *p_handle);
bool initialized_instance_impdrv_ctrl_handle_t (st_impdrv_ctl_t *p_handle);
bool initialize_p_corectl(st_impdrv_corectl_t* p_corectl, uint32_t core_num);
bool uninitialize_p_corectl(st_impdrv_corectl_t* p_corectl, uint32_t core_num);
static st_impdrv_coretypectl_t* initialize_corectl(st_impdrv_corectl_t *p_corectl, e_impdrv_state_t state, e_impdrv_param_t param, bool value);
bool initialize_p_coretypectl_dsp_ext(st_impdrv_coretypectl_t *p_coretypectl, uint32_t* core_num);

bool initialize_p_coretypectl_param_off_nullhandle(st_impdrv_coretypectl_t *p_coretypectl, uint32_t* core_num);
bool initialize_p_coretypectl_param_off_nullhandle1(st_impdrv_coretypectl_t *p_coretypectl, uint32_t* core_num);
bool initialize_p_coretypectl_param_off_nullhandle2(st_impdrv_coretypectl_t *p_coretypectl, uint32_t* core_num);
bool ready_p_corectl_dsp_running(st_impdrv_corectl_t* p_corectl, uint32_t core_num);
bool ready_p_corectl_dsp_standby(st_impdrv_corectl_t* p_corectl, uint32_t core_num);
bool ready_corectl_dsp_not_update_app(st_impdrv_corectl_t* p_corectl, uint32_t core_num);

bool set_p_corectl_dsp_cbfunc(st_impdrv_corectl_t* p_corectl);
bool unset_p_corectl_dsp_cbfunc(st_impdrv_corectl_t* p_corectl);
bool uninit_p_corectl_dsp_execute_pre(st_impdrv_corectl_t* p_corectl);
bool init_p_corectl_dsp_execute_pre(st_impdrv_corectl_t* p_corectl);
bool ready_p_corectl_dsp_execute_pre(st_impdrv_corectl_t* p_corectl);
bool exec_p_corectl_dsp_execute_pre(st_impdrv_corectl_t* p_corectl);
bool int_p_corectl_dsp_execute_pre(st_impdrv_corectl_t* p_corectl);
bool invalid_p_corectl_dsp_execute_pre(st_impdrv_corectl_t* p_corectl);
bool valid_p_corectl_execute_data_dsp_info(st_impdrv_corectl_t* p_corectl);
bool invalid_p_corectl_execute_data_dsp_info(st_impdrv_corectl_t* p_corectl);
bool deinit_device_dsp_io_nth(st_impdrv_device_handle_t **p_device_io, uint32_t index);
bool deinit_device_dsp_io_nth_handle(st_impdrv_device_handle_t **p_device_io, uint32_t index);
bool p_coretypectl_dsp_nth_handle(st_impdrv_coretypectl_t *p_coretypectl, uint32_t core_num, e_impdrv_state_t state, uint32_t index, bool null_flag);
bool p_corectl_dsp_nth_handle(st_impdrv_corectl_t* p_corectl, e_impdrv_state_t state, uint32_t index, bool null_flag);
bool crc_impdrv_ctrl_handle_t(st_impdrv_ctl_t *p_handle);
extern st_impdrv_hwrsc_chk_t   g_hwrsc_chk_table;
extern e_impdrv_errorcode_t impdrv_cmnctl_set_checksum(void);
extern e_impdrv_errorcode_t impdrv_cmnctl_test_checksum(void);

#if defined(RCAR_V3U)
#define IMPDRV_IMP_CORENUM_VAL      (4U)                /**< Maximum number of cores for IMP. */
#define IMPDRV_IMPS_CORENUM_VAL     (0U)                /**< Maximum number of cores for Slim-IMP. */
#define IMPDRV_OCV_CORENUM_VAL      (8U)                /**< Maximum number of cores for OCV. */
#define IMPDRV_DMA_CORENUM_VAL      (4U)                /**< Maximum number of cores for DMA. */
#define IMPDRV_DMAS_CORENUM_VAL     (0U)                /**< Maximum number of cores for DMAS. */
#define IMPDRV_LDMA_CORENUM_VAL     (0U)                /**< Maximum number of cores for LDMA. */
#define IMPDRV_PSC_CORENUM_VAL      (2U)                /**< Maximum number of cores for PSC. */
#define IMPDRV_CNN_CORENUM_VAL      (3U)                /**< Maximum number of cores for CNN. */
#define IMPDRV_DTA_CORENUM_VAL      (1U)                /**< Maximum number of cores for DTA. */
#elif defined(RCAR_V3H2)
#define IMPDRV_IMP_CORENUM_VAL      (5U)                /**< Maximum number of cores for IMP. */
#define IMPDRV_IMPS_CORENUM_VAL     (1U)                /**< Maximum number of cores for Slim-IMP. */
#define IMPDRV_OCV_CORENUM_VAL      (5U)                /**< Maximum number of cores for OCV. */
#define IMPDRV_DMA_CORENUM_VAL      (4U)                /**< Maximum number of cores for DMA. */
#define IMPDRV_DMAS_CORENUM_VAL     (2U)                /**< Maximum number of cores for DMAS. */
#define IMPDRV_LDMA_CORENUM_VAL     (0U)                /**< Maximum number of cores for LDMA. */
#define IMPDRV_PSC_CORENUM_VAL      (1U)                /**< Maximum number of cores for PSC. */
#define IMPDRV_CNN_CORENUM_VAL      (1U)                /**< Maximum number of cores for CNN. */
#define IMPDRV_DTA_CORENUM_VAL      (0U)                /**< Maximum number of cores for DTA. */
#elif defined(RCAR_V3H1)
#define IMPDRV_IMP_CORENUM_VAL      (5U)                /**< Maximum number of cores for IMP. */
#define IMPDRV_IMPS_CORENUM_VAL     (1U)                /**< Maximum number of cores for Slim-IMP. */
#define IMPDRV_OCV_CORENUM_VAL      (5U)                /**< Maximum number of cores for OCV. */
#define IMPDRV_DMA_CORENUM_VAL      (4U)                /**< Maximum number of cores for DMA. */
#define IMPDRV_DMAS_CORENUM_VAL     (0U)                /**< Maximum number of cores for DMAS. */
#define IMPDRV_LDMA_CORENUM_VAL     (0U)                /**< Maximum number of cores for LDMA. */
#define IMPDRV_PSC_CORENUM_VAL      (2U)                /**< Maximum number of cores for PSC. */
#define IMPDRV_CNN_CORENUM_VAL      (1U)                /**< Maximum number of cores for CNN. */
#define IMPDRV_DTA_CORENUM_VAL      (0U)                /**< Maximum number of cores for DTA. */
#elif defined(RCAR_V3M2)
#define IMPDRV_IMP_CORENUM_VAL      (4U)                /**< Maximum number of cores for IMP. */
#define IMPDRV_IMPS_CORENUM_VAL     (0U)                /**< Maximum number of cores for Slim-IMP. */
#define IMPDRV_OCV_CORENUM_VAL      (2U)                /**< Maximum number of cores for OCV. */
#define IMPDRV_DMA_CORENUM_VAL      (2U)                /**< Maximum number of cores for DMA. */
#define IMPDRV_DMAS_CORENUM_VAL     (0U)                /**< Maximum number of cores for DMAS. */
#define IMPDRV_LDMA_CORENUM_VAL     (0U)                /**< Maximum number of cores for LDMA. */
#define IMPDRV_PSC_CORENUM_VAL      (1U)                /**< Maximum number of cores for PSC. */
#define IMPDRV_CNN_CORENUM_VAL      (1U)                /**< Maximum number of cores for CNN. */
#define IMPDRV_DTA_CORENUM_VAL      (0U)                /**< Maximum number of cores for DTA. */
#endif
#elif defined(RCAR_V4H) || defined(RCAR_V4H2) || defined(RCAR_V4M)
#define IMPDRV_IMP_CORENUM_VAL      (4U) 
#define IMPDRV_IMPS_CORENUM_VAL     (0U) 
#define IMPDRV_OCV_CORENUM_VAL      (4U) 
#define IMPDRV_DMA_CORENUM_VAL      (4U) 
#define IMPDRV_DMAS_CORENUM_VAL     (4U) 
#define IMPDRV_LDMA_CORENUM_VAL     (0U) 
#define IMPDRV_PSC_CORENUM_VAL      (1U) 
#define IMPDRV_CNN_CORENUM_VAL      (1U) 
#define IMPDRV_DTA_CORENUM_VAL      (0U) 
#define IMPDRV_DSP_CORENUM_VAL          (4U)

#endif /* _IMPDRV_UTIL_H */