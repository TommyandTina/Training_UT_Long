#ifndef _TEST_CASE_H_
#define _TEST_CASE_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define DECLARE_TESTCASE_TABLE(func)	extern struct TestCase UT_##func##_All_Tests[]
#define TESTCASE_TABLE(category,func) { #func,	category,	UT_##func##_All_Tests	}
/* Function R_IMPDRV_*** */

DECLARE_TESTCASE_TABLE(impdrv_osdep_pm_get_policy);
DECLARE_TESTCASE_TABLE(R_IMPDRV_GetPMState);
DECLARE_TESTCASE_TABLE(R_IMPDRV_Init);
DECLARE_TESTCASE_TABLE(R_IMPDRV_Quit);
DECLARE_TESTCASE_TABLE(R_IMPDRV_AttrInit);
DECLARE_TESTCASE_TABLE(R_IMPDRV_AttrSetCoreMemInit);
DECLARE_TESTCASE_TABLE(R_IMPDRV_Start);
DECLARE_TESTCASE_TABLE(R_IMPDRV_Stop);
DECLARE_TESTCASE_TABLE(R_IMPDRV_AttrSetCoreMap);
DECLARE_TESTCASE_TABLE(R_IMPDRV_AttrSetCl);
DECLARE_TESTCASE_TABLE(R_IMPDRV_AttrSetIrqMask);
DECLARE_TESTCASE_TABLE(R_IMPDRV_SetIrqGroup);
DECLARE_TESTCASE_TABLE(R_IMPDRV_Execute);
DECLARE_TESTCASE_TABLE(R_IMPDRV_ResumeExecution);
DECLARE_TESTCASE_TABLE(R_IMPDRV_SetPMPolicy);
DECLARE_TESTCASE_TABLE(R_IMPDRV_GetPMPolicy);
DECLARE_TESTCASE_TABLE(R_IMPDRV_GetVersion);
DECLARE_TESTCASE_TABLE(R_IMPDRV_RegGetHwInfo);
DECLARE_TESTCASE_TABLE(R_IMPDRV_RegRead32);
DECLARE_TESTCASE_TABLE(R_IMPDRV_RegWrite32);
DECLARE_TESTCASE_TABLE(R_IMPDRV_RegRequired);
DECLARE_TESTCASE_TABLE(R_IMPDRV_SetMemProtect);
DECLARE_TESTCASE_TABLE(R_IMPDRV_BusIfCheck);
DECLARE_TESTCASE_TABLE(R_IMPDRV_ConfRegCheck);
DECLARE_TESTCASE_TABLE(R_IMPDRV_ModuleStopCheck);
DECLARE_TESTCASE_TABLE(R_IMPDRV_AttrSetDsp);
DECLARE_TESTCASE_TABLE(R_IMPDRV_AttrSetClBrkAddr);
DECLARE_TESTCASE_TABLE(R_IMPDRV_AttrSetGosubCond);
DECLARE_TESTCASE_TABLE(impdrv_api_chk_init_data);
DECLARE_TESTCASE_TABLE(impdrv_api_chk_core_info);
DECLARE_TESTCASE_TABLE(impdrv_api_chk_instance_num);
DECLARE_TESTCASE_TABLE(impdrv_api_cnv_error_code);
DECLARE_TESTCASE_TABLE(impdrv_api_cnv_ctrl_handle);
DECLARE_TESTCASE_TABLE(impdrv_api_chk_bus_if_check);
DECLARE_TESTCASE_TABLE(impdrv_api_chk_conf_reg_check);
DECLARE_TESTCASE_TABLE(impdrv_impctl_pm_get_state);
DECLARE_TESTCASE_TABLE(impdrv_ocvctl_pm_get_state);
DECLARE_TESTCASE_TABLE(impdrv_dmasctl_pm_get_state);
DECLARE_TESTCASE_TABLE(impdrv_pscctl_pm_get_state);
DECLARE_TESTCASE_TABLE(impdrv_dspctl_pm_get_state);
DECLARE_TESTCASE_TABLE(impdrv_osdep_pm_get_state);
DECLARE_TESTCASE_TABLE(impdrv_dmasctl_quit);
DECLARE_TESTCASE_TABLE(impdrv_impctl_quit);
DECLARE_TESTCASE_TABLE(impdrv_ocvctl_quit);
DECLARE_TESTCASE_TABLE(impdrv_pscctl_quit);
DECLARE_TESTCASE_TABLE(impdrv_osdep_dev_open_imp);
DECLARE_TESTCASE_TABLE(impdrv_osdep_dev_open_dsp);
DECLARE_TESTCASE_TABLE(impdrv_osdep_pow_on_imp);
DECLARE_TESTCASE_TABLE(impdrv_osdep_pm_set_policy);
DECLARE_TESTCASE_TABLE(impdrv_impctl_init_end);
DECLARE_TESTCASE_TABLE(impdrv_ocvctl_init_end);

DECLARE_TESTCASE_TABLE(impdrv_dspctl_load_dtcm);
DECLARE_TESTCASE_TABLE(impdrv_dspctl_resume_exe);
DECLARE_TESTCASE_TABLE(impdrv_dspctl_set_core_map);
DECLARE_TESTCASE_TABLE(impdrv_dspctl_set_dsp);
DECLARE_TESTCASE_TABLE(impdrv_dspctl_tcm_config_d);
DECLARE_TESTCASE_TABLE(impdrv_dspctl_tcm_config_p);

/* Function impdrv_genctl_*** */

DECLARE_TESTCASE_TABLE(impdrv_genctl_init);
DECLARE_TESTCASE_TABLE(impdrv_genctl_start);
DECLARE_TESTCASE_TABLE(impdrv_genctl_stop);
DECLARE_TESTCASE_TABLE(impdrv_genctl_execute);
DECLARE_TESTCASE_TABLE(impdrv_genctl_resume_exe);
DECLARE_TESTCASE_TABLE(impdrv_genctl_pm_set_policy);
DECLARE_TESTCASE_TABLE(impdrv_genctl_pm_get_policy);
DECLARE_TESTCASE_TABLE(impdrv_genctl_attr_init);
DECLARE_TESTCASE_TABLE(impdrv_genctl_set_core_mem_init);
DECLARE_TESTCASE_TABLE(impdrv_genctl_set_core_map);
DECLARE_TESTCASE_TABLE(impdrv_genctl_set_cl);
DECLARE_TESTCASE_TABLE(impdrv_genctl_set_irq_mask);
DECLARE_TESTCASE_TABLE(impdrv_genctl_set_irq_group);
DECLARE_TESTCASE_TABLE(impdrv_genctl_int_handler);
DECLARE_TESTCASE_TABLE(impdrv_genctl_reg_set_mem_prote);
DECLARE_TESTCASE_TABLE(impdrv_genctl_reg_get_hw_info);
DECLARE_TESTCASE_TABLE(impdrv_genctl_reg_read32);
DECLARE_TESTCASE_TABLE(impdrv_genctl_reg_write32);
DECLARE_TESTCASE_TABLE(impdrv_genctl_reg_required);
DECLARE_TESTCASE_TABLE(impdrv_genctl_check_intclear);
DECLARE_TESTCASE_TABLE(impdrv_genctl_bus_if_check);
DECLARE_TESTCASE_TABLE(impdrv_genctl_conf_reg_check);
#if defined(RCAR_V3H2) || defined(RCAR_V3M2) || defined(RCAR_V3H1)
DECLARE_TESTCASE_TABLE(impdrv_genctl_set_dsp_v3x);
#else
DECLARE_TESTCASE_TABLE(impdrv_genctl_set_dsp_v4x);
#endif
DECLARE_TESTCASE_TABLE(impdrv_genctl_chk_core_info);
DECLARE_TESTCASE_TABLE(impdrv_genctl_prologue);
DECLARE_TESTCASE_TABLE(impdrv_genctl_epilogue);
DECLARE_TESTCASE_TABLE(impdrv_genctl_chk_state_uninit);
DECLARE_TESTCASE_TABLE(impdrv_genctl_core_init);
DECLARE_TESTCASE_TABLE(impdrv_genctl_init_get_func);
DECLARE_TESTCASE_TABLE(impdrv_genctl_init_chk_state);
DECLARE_TESTCASE_TABLE(impdrv_genctl_init_set_state);
DECLARE_TESTCASE_TABLE(impdrv_genctl_init_attr_init);
DECLARE_TESTCASE_TABLE(impdrv_genctl_set_state_uninit);
DECLARE_TESTCASE_TABLE(impdrv_genctl_pm_get_state);
DECLARE_TESTCASE_TABLE(impdrv_genctl_chk_state_init);
DECLARE_TESTCASE_TABLE(impdrv_genctl_quit);
DECLARE_TESTCASE_TABLE(impdrv_genctl_get_worksize);
DECLARE_TESTCASE_TABLE(impdrv_genctl_dsp_int_handler);
DECLARE_TESTCASE_TABLE(impdrv_genctl_set_cl_brk_addr);


/* Function impdrv_cmnctl_*** */

DECLARE_TESTCASE_TABLE(impdrv_cmnctl_init);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_quit);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_judge_int);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_mutex_create);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_mutex_destroy);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_mutex_lock);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_mutex_unlock);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_set_irq_group);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_get_corectl_func);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_get_syncc_val);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_fatal_cb);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_chk_instance_num);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_get_io_imptop);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_get_io_dta);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_reg_init);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_reg_quit);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_reg_set_mem_prot);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_reg_mem_init);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_reg_get_hw_info);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_reg_read32);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_reg_write32);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_reg_required);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_save_irq_mask);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_force_irq_mask);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_restore_irq_mask);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_conf_reg_check);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_reg_chk_mem_prote);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_chk_core_info);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_cnv_int_core_bit);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_cnv_int_bit_core);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_execute_no_group);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_execute_group);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_chk_init_data);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_chk_group_data);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_get_group);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_calc_checksum);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_set_checksum);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_test_checksum);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_reg_hwrsc_open);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_reg_hwrsc_close);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_reg_pow_on_hwrsc);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_reg_pow_off_hwrsc);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_reg_find_resource);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_reg_chk_rsc_name);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_reg_set_checksum);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_reg_test_checksum);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_get_dsp_func);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_get_init_param);
DECLARE_TESTCASE_TABLE(ut_cmnctl_reg_spm_wait_init);
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_reg_spm_preset);
DECLARE_TESTCASE_TABLE(ut_cmnctl_reg_spm_initialize);
#if defined(RCAR_V4H)
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_reg_get_handle);
#endif
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_reg_spm_chk_init);
DECLARE_TESTCASE_TABLE(ut_cmnctl_reg_spm_ena_access);

/* Function impdrv_cnnctl_*** */

DECLARE_TESTCASE_TABLE(impdrv_cnnctl_check_core_busy);
DECLARE_TESTCASE_TABLE(impdrv_cnnctl_is_valid_core);
DECLARE_TESTCASE_TABLE(impdrv_cnnctl_check_state);
DECLARE_TESTCASE_TABLE(impdrv_cnnctl_set_state);
DECLARE_TESTCASE_TABLE(impdrv_cnnctl_init_start);
DECLARE_TESTCASE_TABLE(impdrv_cnnctl_init_end);
DECLARE_TESTCASE_TABLE(impdrv_cnnctl_start);
DECLARE_TESTCASE_TABLE(impdrv_cnnctl_stop);
DECLARE_TESTCASE_TABLE(impdrv_cnnctl_attr_init);
DECLARE_TESTCASE_TABLE(impdrv_cnnctl_set_mem_init);
DECLARE_TESTCASE_TABLE(impdrv_cnnctl_set_core_map);
DECLARE_TESTCASE_TABLE(impdrv_cnnctl_set_cl);
DECLARE_TESTCASE_TABLE(impdrv_cnnctl_set_irq_mask);
DECLARE_TESTCASE_TABLE(impdrv_cnnctl_execute);
DECLARE_TESTCASE_TABLE(impdrv_cnnctl_int_handler);
DECLARE_TESTCASE_TABLE(impdrv_cnnctl_resume_exe);
DECLARE_TESTCASE_TABLE(impdrv_cnnctl_pm_set_policy);
DECLARE_TESTCASE_TABLE(impdrv_cnnctl_pm_get_policy);
DECLARE_TESTCASE_TABLE(impdrv_cnnctl_conf_reg_check);
DECLARE_TESTCASE_TABLE(impdrv_cnnctl_soft_reset);
DECLARE_TESTCASE_TABLE(impdrv_cnnctl_set_syncc_config);
DECLARE_TESTCASE_TABLE(impdrv_cnnctl_get_inten_val);
DECLARE_TESTCASE_TABLE(impdrv_cnnctl_check_inten);
DECLARE_TESTCASE_TABLE(impdrv_cnnctl_int_cb);
DECLARE_TESTCASE_TABLE(impdrv_cnnctl_pm_get_state);
DECLARE_TESTCASE_TABLE(impdrv_cnnctl_quit);

/* Function impdrv_dmactl_*** */

DECLARE_TESTCASE_TABLE(impdrv_dmactl_is_valid_core);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_check_state);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_set_state);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_init_start);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_init_end);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_start);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_stop);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_attr_init);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_set_mem_init);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_set_core_map);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_set_cl);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_set_irq_mask);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_execute);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_int_handler);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_resume_exe);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_pm_set_policy);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_pm_get_policy);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_conf_reg_check);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_mb_init);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_is_sub_thread);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_osal_core_num);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_read_reg);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_write_reg);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_init_core);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_soft_reset);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_set_syncc_config);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_get_inten_val);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_check_inten);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_int_cb);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_check_core_busy);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_pm_get_state);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_quit);

#if defined(RCAR_V3H2) || defined(RCAR_V4H) || defined(RCAR_V4H2)
DECLARE_TESTCASE_TABLE(impdrv_dmasctl_is_valid_core);
DECLARE_TESTCASE_TABLE(impdrv_dmasctl_check_state);
DECLARE_TESTCASE_TABLE(impdrv_dmasctl_set_state);
DECLARE_TESTCASE_TABLE(impdrv_dmasctl_init_start);
DECLARE_TESTCASE_TABLE(impdrv_dmasctl_init_end);
DECLARE_TESTCASE_TABLE(impdrv_dspctl_pm_get_policy);
DECLARE_TESTCASE_TABLE(impdrv_dspctl_pm_set_policy);
DECLARE_TESTCASE_TABLE(impdrv_dspctl_load_ptcm);
DECLARE_TESTCASE_TABLE(impdrv_dspctl_stop_reg);
DECLARE_TESTCASE_TABLE(impdrv_dspctl_stop);
DECLARE_TESTCASE_TABLE(impdrv_osdep_pow_on_imp_dsp);
DECLARE_TESTCASE_TABLE(impdrv_genctl_set_gosub_cond);
DECLARE_TESTCASE_TABLE(impdrv_dspctl_dsp_start_pre);
DECLARE_TESTCASE_TABLE(impdrv_dspctl_init_end);
DECLARE_TESTCASE_TABLE(impdrv_dspctl_init_start);
DECLARE_TESTCASE_TABLE(impdrv_dspctl_init_core);
DECLARE_TESTCASE_TABLE(impdrv_dspctl_dsp_start);
DECLARE_TESTCASE_TABLE(impdrv_dspctl_dsp_update_app);
DECLARE_TESTCASE_TABLE(impdrv_dspctl_dsp_execute);
DECLARE_TESTCASE_TABLE(impdrv_dspctl_quit);
// DECLARE_TESTCASE_TABLE(impdrv_dspctl_is_valid_core);

DECLARE_TESTCASE_TABLE(impdrv_dmasctl_start);
DECLARE_TESTCASE_TABLE(impdrv_dmasctl_stop);
DECLARE_TESTCASE_TABLE(impdrv_dmasctl_set_mem_init);
DECLARE_TESTCASE_TABLE(impdrv_dmasctl_set_core_map);
DECLARE_TESTCASE_TABLE(impdrv_dmasctl_set_cl);
DECLARE_TESTCASE_TABLE(impdrv_dmasctl_set_irq_mask);
DECLARE_TESTCASE_TABLE(impdrv_dmasctl_execute);
DECLARE_TESTCASE_TABLE(impdrv_dmasctl_int_handler);
DECLARE_TESTCASE_TABLE(impdrv_dmasctl_resume_exe);
DECLARE_TESTCASE_TABLE(impdrv_dmasctl_pm_set_policy);
DECLARE_TESTCASE_TABLE(impdrv_dmasctl_pm_get_policy);
DECLARE_TESTCASE_TABLE(impdrv_dmasctl_conf_reg_check);
DECLARE_TESTCASE_TABLE(impdrv_dmasctl_is_sub_thread);
DECLARE_TESTCASE_TABLE(impdrv_dmasctl_osal_core_num);
DECLARE_TESTCASE_TABLE(impdrv_dmasctl_read_reg);
DECLARE_TESTCASE_TABLE(impdrv_dmasctl_write_reg);
DECLARE_TESTCASE_TABLE(impdrv_dmasctl_init_core);
DECLARE_TESTCASE_TABLE(impdrv_dmasctl_check_core_busy);
DECLARE_TESTCASE_TABLE(impdrv_dmasctl_soft_reset);
DECLARE_TESTCASE_TABLE(impdrv_dmasctl_set_syncc_config);
DECLARE_TESTCASE_TABLE(impdrv_dmasctl_get_inten_val);
DECLARE_TESTCASE_TABLE(impdrv_dmasctl_check_inten);
DECLARE_TESTCASE_TABLE(impdrv_dmasctl_int_cb);
DECLARE_TESTCASE_TABLE(impdrv_dmasctl_attr_init);
#endif 


#if defined(RCAR_V3H2) || defined(RCAR_V3M2) || defined(RCAR_V3H1)
DECLARE_TESTCASE_TABLE(impdrv_cmnctl_reg_mb_initialize);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_bus_if_check);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_cl_pre);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_dma_transfer);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_int_main_func);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_int_safety_func);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_mb_init_main);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_mb_init_pre);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_tbl_write_reg);
DECLARE_TESTCASE_TABLE(impdrv_impctl_bus_if_check);
DECLARE_TESTCASE_TABLE(impdrv_impctl_dma_transfer);
DECLARE_TESTCASE_TABLE(impdrv_impctl_int_main_func);
DECLARE_TESTCASE_TABLE(impdrv_impctl_int_safety_func);
DECLARE_TESTCASE_TABLE(impdrv_impctl_tbl_write_reg);
DECLARE_TESTCASE_TABLE(impdrv_ocvctl_init_lwm);
#endif

/* Function impdrv_impctl_*** */

DECLARE_TESTCASE_TABLE(impdrv_impctl_is_valid_core);
DECLARE_TESTCASE_TABLE(impdrv_impctl_check_state);
DECLARE_TESTCASE_TABLE(impdrv_impctl_set_state);
DECLARE_TESTCASE_TABLE(impdrv_impctl_init_start);
DECLARE_TESTCASE_TABLE(impdrv_impctl_start);
DECLARE_TESTCASE_TABLE(impdrv_impctl_stop);
DECLARE_TESTCASE_TABLE(impdrv_impctl_attr_init);
DECLARE_TESTCASE_TABLE(impdrv_impctl_set_mem_init);
DECLARE_TESTCASE_TABLE(impdrv_impctl_set_core_map);
DECLARE_TESTCASE_TABLE(impdrv_impctl_set_cl);
DECLARE_TESTCASE_TABLE(impdrv_impctl_set_irq_mask);
DECLARE_TESTCASE_TABLE(impdrv_impctl_execute);
DECLARE_TESTCASE_TABLE(impdrv_impctl_int_handler);
DECLARE_TESTCASE_TABLE(impdrv_impctl_resume_exe);
DECLARE_TESTCASE_TABLE(impdrv_impctl_pm_set_policy);
DECLARE_TESTCASE_TABLE(impdrv_impctl_pm_get_policy);
DECLARE_TESTCASE_TABLE(impdrv_impctl_conf_reg_check);
DECLARE_TESTCASE_TABLE(impdrv_impctl_init_core);
DECLARE_TESTCASE_TABLE(impdrv_impctl_check_core_busy);
DECLARE_TESTCASE_TABLE(impdrv_impctl_soft_reset);
DECLARE_TESTCASE_TABLE(impdrv_impctl_set_syncc_config);
DECLARE_TESTCASE_TABLE(impdrv_impctl_get_inten_val);
DECLARE_TESTCASE_TABLE(impdrv_impctl_check_inten);
DECLARE_TESTCASE_TABLE(impdrv_impctl_int_cb);

/* Function impdrv_ocvctl_*** */

DECLARE_TESTCASE_TABLE(impdrv_ocvctl_is_valid_core);
DECLARE_TESTCASE_TABLE(impdrv_ocvctl_check_state);
DECLARE_TESTCASE_TABLE(impdrv_ocvctl_set_state);
DECLARE_TESTCASE_TABLE(impdrv_ocvctl_init_start);
DECLARE_TESTCASE_TABLE(impdrv_ocvctl_start);
DECLARE_TESTCASE_TABLE(impdrv_ocvctl_stop);
DECLARE_TESTCASE_TABLE(impdrv_ocvctl_attr_init);
DECLARE_TESTCASE_TABLE(impdrv_ocvctl_set_mem_init);
DECLARE_TESTCASE_TABLE(impdrv_ocvctl_set_core_map);
DECLARE_TESTCASE_TABLE(impdrv_ocvctl_set_cl);
DECLARE_TESTCASE_TABLE(impdrv_ocvctl_execute);
DECLARE_TESTCASE_TABLE(impdrv_ocvctl_int_handler);
DECLARE_TESTCASE_TABLE(impdrv_ocvctl_resume_exe);
DECLARE_TESTCASE_TABLE(impdrv_ocvctl_pm_set_policy);
DECLARE_TESTCASE_TABLE(impdrv_ocvctl_pm_get_policy);
DECLARE_TESTCASE_TABLE(impdrv_ocvctl_conf_reg_check);
DECLARE_TESTCASE_TABLE(impdrv_ocvctl_set_irq_mask);
DECLARE_TESTCASE_TABLE(impdrv_ocvctl_init_core);
DECLARE_TESTCASE_TABLE(impdrv_ocvctl_check_core_busy);
DECLARE_TESTCASE_TABLE(impdrv_ocvctl_soft_reset);
DECLARE_TESTCASE_TABLE(impdrv_ocvctl_set_syncc_config);
DECLARE_TESTCASE_TABLE(impdrv_ocvctl_irq_status_clear);
DECLARE_TESTCASE_TABLE(impdrv_ocvctl_get_inten_val);
DECLARE_TESTCASE_TABLE(impdrv_ocvctl_check_inten);
DECLARE_TESTCASE_TABLE(impdrv_ocvctl_check_inten_1st);
DECLARE_TESTCASE_TABLE(impdrv_ocvctl_check_inten_2nd);
DECLARE_TESTCASE_TABLE(impdrv_ocvctl_int_cb);

/* Function impdrv_pscctl_*** */

DECLARE_TESTCASE_TABLE(impdrv_pscctl_is_valid_core);
DECLARE_TESTCASE_TABLE(impdrv_pscctl_check_state);
DECLARE_TESTCASE_TABLE(impdrv_pscctl_set_state);
DECLARE_TESTCASE_TABLE(impdrv_pscctl_init_start);
DECLARE_TESTCASE_TABLE(impdrv_pscctl_start);
DECLARE_TESTCASE_TABLE(impdrv_pscctl_stop);
DECLARE_TESTCASE_TABLE(impdrv_pscctl_attr_init);
DECLARE_TESTCASE_TABLE(impdrv_pscctl_set_mem_init);
DECLARE_TESTCASE_TABLE(impdrv_pscctl_set_core_map);
DECLARE_TESTCASE_TABLE(impdrv_pscctl_set_cl);
DECLARE_TESTCASE_TABLE(impdrv_pscctl_set_irq_mask);
DECLARE_TESTCASE_TABLE(impdrv_pscctl_int_handler);
DECLARE_TESTCASE_TABLE(impdrv_pscctl_resume_exe);
DECLARE_TESTCASE_TABLE(impdrv_pscctl_pm_set_policy);
DECLARE_TESTCASE_TABLE(impdrv_pscctl_pm_get_policy);
DECLARE_TESTCASE_TABLE(impdrv_pscctl_conf_reg_check);
DECLARE_TESTCASE_TABLE(impdrv_pscctl_get_inten_val);
DECLARE_TESTCASE_TABLE(impdrv_pscctl_check_core_busy);
DECLARE_TESTCASE_TABLE(impdrv_pscctl_soft_reset);
DECLARE_TESTCASE_TABLE(impdrv_pscctl_set_syncc_config);
DECLARE_TESTCASE_TABLE(impdrv_pscctl_int_cb);
DECLARE_TESTCASE_TABLE(impdrv_pscctl_check_inten);
DECLARE_TESTCASE_TABLE(impdrv_pscctl_execute);
DECLARE_TESTCASE_TABLE(impdrv_pscctl_init_end);

/* Function impdrv_osdep_*** */

DECLARE_TESTCASE_TABLE(impdrv_osdep_pow_on_imp_top);
DECLARE_TESTCASE_TABLE(impdrv_osdep_pow_off_imp_top);
DECLARE_TESTCASE_TABLE(impdrv_osdep_register_irq);
DECLARE_TESTCASE_TABLE(impdrv_osdep_enable_irq);
DECLARE_TESTCASE_TABLE(impdrv_osdep_disable_irq);
DECLARE_TESTCASE_TABLE(impdrv_osdep_unregister_irq);
DECLARE_TESTCASE_TABLE(impdrv_osdep_pow_off_imp);
DECLARE_TESTCASE_TABLE(impdrv_osdep_write_reg);
DECLARE_TESTCASE_TABLE(impdrv_osdep_read_reg);
DECLARE_TESTCASE_TABLE(impdrv_osdep_mutex_create);
DECLARE_TESTCASE_TABLE(impdrv_osdep_mutex_destroy);
DECLARE_TESTCASE_TABLE(impdrv_osdep_mutex_lock);
DECLARE_TESTCASE_TABLE(impdrv_osdep_mutex_unlock);
DECLARE_TESTCASE_TABLE(impdrv_osdep_reset_core);
DECLARE_TESTCASE_TABLE(impdrv_osdep_pow_on_hwrsc);
DECLARE_TESTCASE_TABLE(impdrv_osdep_pow_off_hwrsc);
DECLARE_TESTCASE_TABLE(impdrv_osdep_memory_barrier);
DECLARE_TESTCASE_TABLE(impdrv_osdep_queue_create);
DECLARE_TESTCASE_TABLE(impdrv_osdep_queue_destroy);
DECLARE_TESTCASE_TABLE(impdrv_osdep_queue_wait_period);
DECLARE_TESTCASE_TABLE(impdrv_osdep_queue_send_period);
DECLARE_TESTCASE_TABLE(impdrv_osdep_memory_get_size);
DECLARE_TESTCASE_TABLE(impdrv_osdep_memory_get_logical);
DECLARE_TESTCASE_TABLE(impdrv_osdep_memory_get_physica);
DECLARE_TESTCASE_TABLE(impdrv_osdep_memory_cache_flush);
DECLARE_TESTCASE_TABLE(impdrv_osdep_memory_cache_clean);
DECLARE_TESTCASE_TABLE(impdrv_osdep_memory_data_set);
DECLARE_TESTCASE_TABLE(impdrv_osdep_memory_compare);
DECLARE_TESTCASE_TABLE(impdrv_osdep_dev_open);
DECLARE_TESTCASE_TABLE(impdrv_osdep_dev_close);
DECLARE_TESTCASE_TABLE(impdrv_osdep_pow_on);
DECLARE_TESTCASE_TABLE(impdrv_osdep_pow_off);
DECLARE_TESTCASE_TABLE(impdrv_osdep_int_handler);
DECLARE_TESTCASE_TABLE(impdrv_osdep_chk_core_info);
DECLARE_TESTCASE_TABLE(impdrv_osdep_chk_instance_num);
DECLARE_TESTCASE_TABLE(impdrv_osdep_get_irq_channel);
DECLARE_TESTCASE_TABLE(impdrv_osdep_register_dsp_irq);
DECLARE_TESTCASE_TABLE(impdrv_osdep_reset_core_dsp);
DECLARE_TESTCASE_TABLE(impdrv_osdep_unregister_dsp_irq);
DECLARE_TESTCASE_TABLE(impdrv_osdep_wait_for_req_state);
DECLARE_TESTCASE_TABLE(ut_osdep_get_dsp_irq_channel);
DECLARE_TESTCASE_TABLE(impdrv_osdep_enable_dsp_irq);
DECLARE_TESTCASE_TABLE(impdrv_osdep_dsp0_int_handler);
DECLARE_TESTCASE_TABLE(impdrv_osdep_dsp1_int_handler);
DECLARE_TESTCASE_TABLE(impdrv_osdep_dsp2_int_handler);
DECLARE_TESTCASE_TABLE(impdrv_osdep_dsp3_int_handler);
DECLARE_TESTCASE_TABLE(impdrv_osdep_disable_dsp_irq);
DECLARE_TESTCASE_TABLE(impdrv_osdep_dev_open_dsp_sub);

#if defined(RCAR_V4H)

DECLARE_TESTCASE_TABLE(impdrv_cnnctl_force_reset_v4h1);

#endif /* !defined(RCAR_V4H) */

#if defined(RCAR_V4H) || defined(RCAR_V4H2)
DECLARE_TESTCASE_TABLE(impdrv_cnnctl_set_extend_config);
DECLARE_TESTCASE_TABLE(impdrv_cnnctl_set_cl_brk_addr);
DECLARE_TESTCASE_TABLE(impdrv_cnnctl_set_cond_gosub);

DECLARE_TESTCASE_TABLE(impdrv_dmactl_set_extend_config);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_set_cl_brk_addr);
DECLARE_TESTCASE_TABLE(impdrv_dmactl_set_cond_gosub);

DECLARE_TESTCASE_TABLE(impdrv_dmasctl_set_extend_confi);
DECLARE_TESTCASE_TABLE(impdrv_dmasctl_set_cl_brk_addr);
DECLARE_TESTCASE_TABLE(impdrv_dmasctl_set_cond_gosub);

DECLARE_TESTCASE_TABLE(impdrv_impctl_set_extend_config);
DECLARE_TESTCASE_TABLE(impdrv_impctl_set_cl_brk_addr);
DECLARE_TESTCASE_TABLE(impdrv_impctl_set_cond_gosub);

DECLARE_TESTCASE_TABLE(impdrv_ocvctl_set_extend_config);
DECLARE_TESTCASE_TABLE(impdrv_ocvctl_set_cl_brk_addr);
DECLARE_TESTCASE_TABLE(impdrv_ocvctl_set_cond_gosub);

DECLARE_TESTCASE_TABLE(impdrv_pscctl_set_extend_config);
DECLARE_TESTCASE_TABLE(impdrv_pscctl_set_cl_brk_addr);
DECLARE_TESTCASE_TABLE(impdrv_pscctl_set_cond_gosub);

DECLARE_TESTCASE_TABLE(impdrv_dspctl_set_state);
DECLARE_TESTCASE_TABLE(impdrv_dspctl_set_mem_init);

DECLARE_TESTCASE_TABLE(impdrv_dspctl_attr_init);
DECLARE_TESTCASE_TABLE(impdrv_dspctl_check_state);
DECLARE_TESTCASE_TABLE(impdrv_dspctl_chk_execute_data);
DECLARE_TESTCASE_TABLE(impdrv_dspctl_chk_int_data);
DECLARE_TESTCASE_TABLE(impdrv_dspctl_conf_reg_check);
DECLARE_TESTCASE_TABLE(impdrv_dspctl_dsp_execute_pre);
DECLARE_TESTCASE_TABLE(impdrv_dspctl_dsp_foece_standby);
DECLARE_TESTCASE_TABLE(impdrv_dspctl_dsp_standby);
DECLARE_TESTCASE_TABLE(impdrv_dspctl_get_device_io);
DECLARE_TESTCASE_TABLE(impdrv_dspctl_init_dsp);
DECLARE_TESTCASE_TABLE(impdrv_dspctl_is_valid_core);
DECLARE_TESTCASE_TABLE(impdrv_dspctl_int_handler);
#if !defined(IMPDRV_DSPCRC_DETECT_ADDDEF)
DECLARE_TESTCASE_TABLE(impdrv_dspctl_crc_sub);
#else
DECLARE_TESTCASE_TABLE(impdrv_dspctl_crc_sub_macro);
#endif
/* Function for impdrv_udefctl_*/
DECLARE_TESTCASE_TABLE(impdrv_udefctl_udef_crc);
DECLARE_TESTCASE_TABLE(impdrv_udefctl_set_ecm);
#endif

static const struct TestInfo TEST_UT_Info[] = 
{
		/* Function R_IMPDRV_*** */

	TESTCASE_TABLE("imp", R_IMPDRV_GetPMState),
	TESTCASE_TABLE("imp", R_IMPDRV_Init),
	TESTCASE_TABLE("imp", R_IMPDRV_Quit),
	TESTCASE_TABLE("imp", R_IMPDRV_AttrInit),
	TESTCASE_TABLE("imp", R_IMPDRV_AttrSetCoreMemInit),
	TESTCASE_TABLE("imp", R_IMPDRV_Start),
	TESTCASE_TABLE("imp", R_IMPDRV_Stop),
	TESTCASE_TABLE("imp", R_IMPDRV_AttrSetCoreMap),
	TESTCASE_TABLE("imp", R_IMPDRV_AttrSetCl),
	TESTCASE_TABLE("imp", R_IMPDRV_AttrSetIrqMask),
	TESTCASE_TABLE("imp", R_IMPDRV_SetIrqGroup),
	TESTCASE_TABLE("imp", R_IMPDRV_Execute),
	TESTCASE_TABLE("imp", R_IMPDRV_ResumeExecution),
	TESTCASE_TABLE("imp", R_IMPDRV_SetPMPolicy),
	TESTCASE_TABLE("imp", R_IMPDRV_GetPMPolicy),
	TESTCASE_TABLE("imp", R_IMPDRV_GetVersion),
	TESTCASE_TABLE("imp", R_IMPDRV_RegGetHwInfo),
	TESTCASE_TABLE("imp", R_IMPDRV_RegRead32),
	TESTCASE_TABLE("imp", R_IMPDRV_RegWrite32),
	TESTCASE_TABLE("imp", R_IMPDRV_RegRequired),
	TESTCASE_TABLE("imp", R_IMPDRV_SetMemProtect),
	TESTCASE_TABLE("imp", R_IMPDRV_BusIfCheck),
	TESTCASE_TABLE("imp", R_IMPDRV_ConfRegCheck),
	TESTCASE_TABLE("imp", R_IMPDRV_ModuleStopCheck),
	TESTCASE_TABLE("imp", R_IMPDRV_AttrSetDsp),
	TESTCASE_TABLE("imp", R_IMPDRV_AttrSetClBrkAddr),
	TESTCASE_TABLE("imp", R_IMPDRV_AttrSetGosubCond),
	TESTCASE_TABLE("imp", impdrv_api_chk_init_data),
	TESTCASE_TABLE("imp", impdrv_api_chk_core_info),
	TESTCASE_TABLE("imp", impdrv_api_chk_instance_num),
	TESTCASE_TABLE("imp", impdrv_api_cnv_error_code),
	TESTCASE_TABLE("imp", impdrv_api_cnv_ctrl_handle),
	TESTCASE_TABLE("imp", impdrv_api_chk_bus_if_check),
	TESTCASE_TABLE("imp", impdrv_api_chk_conf_reg_check),
	TESTCASE_TABLE("imp", impdrv_dspctl_load_dtcm),
	TESTCASE_TABLE("imp", impdrv_dspctl_resume_exe),
	TESTCASE_TABLE("imp", impdrv_dspctl_set_core_map),
	TESTCASE_TABLE("imp", impdrv_dspctl_set_dsp),
	TESTCASE_TABLE("imp", impdrv_dspctl_tcm_config_d),
	TESTCASE_TABLE("imp", impdrv_dspctl_tcm_config_p),

	/* Function impdrv_genctl_*** */

	TESTCASE_TABLE("imp", impdrv_genctl_init),
	TESTCASE_TABLE("imp", impdrv_genctl_quit),
	TESTCASE_TABLE("imp", impdrv_genctl_start),
	TESTCASE_TABLE("imp", impdrv_genctl_stop),
	TESTCASE_TABLE("imp", impdrv_genctl_execute),
	TESTCASE_TABLE("imp", impdrv_genctl_resume_exe),
	TESTCASE_TABLE("imp", impdrv_genctl_pm_set_policy),
	TESTCASE_TABLE("imp", impdrv_genctl_pm_get_policy),
	TESTCASE_TABLE("imp", impdrv_genctl_attr_init),
	TESTCASE_TABLE("imp", impdrv_genctl_set_core_mem_init),
	TESTCASE_TABLE("imp", impdrv_genctl_set_core_map),
	TESTCASE_TABLE("imp", impdrv_genctl_set_cl),
	TESTCASE_TABLE("imp", impdrv_genctl_set_irq_mask),
	TESTCASE_TABLE("imp", impdrv_genctl_set_irq_group),
	TESTCASE_TABLE("imp", impdrv_genctl_int_handler),
	TESTCASE_TABLE("imp", impdrv_genctl_reg_set_mem_prote),
	TESTCASE_TABLE("imp", impdrv_genctl_reg_get_hw_info),
	TESTCASE_TABLE("imp", impdrv_genctl_reg_read32),
	TESTCASE_TABLE("imp", impdrv_genctl_reg_write32),
	TESTCASE_TABLE("imp", impdrv_genctl_reg_required),
	TESTCASE_TABLE("imp", impdrv_genctl_check_intclear),
	TESTCASE_TABLE("imp", impdrv_genctl_bus_if_check),
	TESTCASE_TABLE("imp", impdrv_genctl_conf_reg_check),
	TESTCASE_TABLE("imp", impdrv_genctl_pm_get_state),
#if defined(RCAR_V3H2) || defined(RCAR_V3M2) || defined(RCAR_V3H1)
	TESTCASE_TABLE("imp", impdrv_genctl_set_dsp_v3x),
#else
	TESTCASE_TABLE("imp", impdrv_genctl_set_dsp_v4x),
#endif
	TESTCASE_TABLE("imp", impdrv_genctl_chk_core_info),
	TESTCASE_TABLE("imp", impdrv_genctl_prologue),
	TESTCASE_TABLE("imp", impdrv_genctl_epilogue),
	TESTCASE_TABLE("imp", impdrv_genctl_chk_state_init),
	TESTCASE_TABLE("imp", impdrv_genctl_chk_state_uninit),
	TESTCASE_TABLE("imp", impdrv_genctl_core_init),
	TESTCASE_TABLE("imp", impdrv_genctl_init_get_func),
	TESTCASE_TABLE("imp", impdrv_genctl_init_chk_state),
	TESTCASE_TABLE("imp", impdrv_genctl_init_set_state),
	TESTCASE_TABLE("imp", impdrv_genctl_init_attr_init),
	TESTCASE_TABLE("imp", impdrv_genctl_set_state_uninit),
	TESTCASE_TABLE("imp", impdrv_genctl_set_gosub_cond),
	TESTCASE_TABLE("imp", impdrv_genctl_get_worksize),
	TESTCASE_TABLE("imp", impdrv_genctl_dsp_int_handler),
	TESTCASE_TABLE("imp", impdrv_genctl_set_cl_brk_addr),

	/* Function impdrv_cmnctl_*** */

	TESTCASE_TABLE("imp", impdrv_cmnctl_init),
	TESTCASE_TABLE("imp", impdrv_cmnctl_quit),
	TESTCASE_TABLE("imp", impdrv_cmnctl_judge_int),
	TESTCASE_TABLE("imp", impdrv_cmnctl_mutex_create),
	TESTCASE_TABLE("imp", impdrv_cmnctl_mutex_destroy),
	TESTCASE_TABLE("imp", impdrv_cmnctl_mutex_lock),
	TESTCASE_TABLE("imp", impdrv_cmnctl_mutex_unlock),
	TESTCASE_TABLE("imp", impdrv_cmnctl_set_irq_group),
	TESTCASE_TABLE("imp", impdrv_cmnctl_get_corectl_func),
	TESTCASE_TABLE("imp", impdrv_cmnctl_get_syncc_val),
	TESTCASE_TABLE("imp", impdrv_cmnctl_fatal_cb),
	TESTCASE_TABLE("imp", impdrv_cmnctl_chk_instance_num),
	TESTCASE_TABLE("imp", impdrv_cmnctl_get_io_imptop),
	TESTCASE_TABLE("imp", impdrv_cmnctl_get_io_dta),
	TESTCASE_TABLE("imp", impdrv_cmnctl_reg_init),
	TESTCASE_TABLE("imp", impdrv_cmnctl_reg_quit),
	TESTCASE_TABLE("imp", impdrv_cmnctl_reg_set_mem_prot),
	TESTCASE_TABLE("imp", impdrv_cmnctl_reg_mem_init),
	TESTCASE_TABLE("imp", impdrv_cmnctl_reg_get_hw_info),
	TESTCASE_TABLE("imp", impdrv_cmnctl_reg_read32),
	TESTCASE_TABLE("imp", impdrv_cmnctl_reg_write32),
	TESTCASE_TABLE("imp", impdrv_cmnctl_reg_required),
	TESTCASE_TABLE("imp", impdrv_cmnctl_save_irq_mask),
	TESTCASE_TABLE("imp", impdrv_cmnctl_force_irq_mask),
	TESTCASE_TABLE("imp", impdrv_cmnctl_restore_irq_mask),
	TESTCASE_TABLE("imp", impdrv_cmnctl_conf_reg_check),
	TESTCASE_TABLE("imp", impdrv_cmnctl_reg_chk_mem_prote),
	TESTCASE_TABLE("imp", impdrv_cmnctl_chk_core_info),
	TESTCASE_TABLE("imp", impdrv_cmnctl_cnv_int_core_bit),
	TESTCASE_TABLE("imp", impdrv_cmnctl_cnv_int_bit_core),
	TESTCASE_TABLE("imp", impdrv_cmnctl_execute_no_group),
	TESTCASE_TABLE("imp", impdrv_cmnctl_execute_group),
	TESTCASE_TABLE("imp", impdrv_cmnctl_chk_init_data),
	TESTCASE_TABLE("imp", impdrv_cmnctl_chk_group_data),
	TESTCASE_TABLE("imp", impdrv_cmnctl_get_group),
	TESTCASE_TABLE("imp", impdrv_cmnctl_calc_checksum),
	TESTCASE_TABLE("imp", impdrv_cmnctl_set_checksum),
	TESTCASE_TABLE("imp", impdrv_cmnctl_test_checksum),
	TESTCASE_TABLE("imp", impdrv_cmnctl_reg_hwrsc_open),
	TESTCASE_TABLE("imp", impdrv_cmnctl_reg_hwrsc_close),
	TESTCASE_TABLE("imp", impdrv_cmnctl_reg_pow_on_hwrsc),
	TESTCASE_TABLE("imp", impdrv_cmnctl_reg_pow_off_hwrsc),
	TESTCASE_TABLE("imp", impdrv_cmnctl_reg_find_resource),
	TESTCASE_TABLE("imp", impdrv_cmnctl_reg_chk_rsc_name),
	TESTCASE_TABLE("imp", impdrv_cmnctl_reg_set_checksum),
	TESTCASE_TABLE("imp", impdrv_cmnctl_reg_test_checksum),
	TESTCASE_TABLE("imp", impdrv_cmnctl_get_dsp_func),
	TESTCASE_TABLE("imp", impdrv_cmnctl_get_init_param),
	TESTCASE_TABLE("imp", ut_cmnctl_reg_spm_wait_init),
	TESTCASE_TABLE("imp", impdrv_cmnctl_reg_spm_preset),
	TESTCASE_TABLE("imp", ut_cmnctl_reg_spm_initialize),
	#if defined(RCAR_V4H)
	TESTCASE_TABLE("imp", impdrv_cmnctl_reg_get_handle),
	#endif
	TESTCASE_TABLE("imp", impdrv_cmnctl_reg_spm_chk_init),
	TESTCASE_TABLE("imp", ut_cmnctl_reg_spm_ena_access),
	/* Function impdrv_cnnctl_*** */

	TESTCASE_TABLE("imp", impdrv_cnnctl_check_core_busy),
	TESTCASE_TABLE("imp", impdrv_cnnctl_is_valid_core),
	TESTCASE_TABLE("imp", impdrv_cnnctl_check_state),
	TESTCASE_TABLE("imp", impdrv_cnnctl_set_state),
	TESTCASE_TABLE("imp", impdrv_cnnctl_init_start),
	TESTCASE_TABLE("imp", impdrv_cnnctl_init_end),
	TESTCASE_TABLE("imp", impdrv_cnnctl_start),
	TESTCASE_TABLE("imp", impdrv_cnnctl_stop),
	TESTCASE_TABLE("imp", impdrv_cnnctl_attr_init),
	TESTCASE_TABLE("imp", impdrv_cnnctl_set_mem_init),
	TESTCASE_TABLE("imp", impdrv_cnnctl_set_core_map),
	TESTCASE_TABLE("imp", impdrv_cnnctl_set_cl),
	TESTCASE_TABLE("imp", impdrv_cnnctl_set_irq_mask),
	TESTCASE_TABLE("imp", impdrv_cnnctl_execute),
	TESTCASE_TABLE("imp", impdrv_cnnctl_int_handler),
	TESTCASE_TABLE("imp", impdrv_cnnctl_resume_exe),
	TESTCASE_TABLE("imp", impdrv_cnnctl_pm_set_policy),
	TESTCASE_TABLE("imp", impdrv_cnnctl_pm_get_policy),
	TESTCASE_TABLE("imp", impdrv_cnnctl_conf_reg_check),
	TESTCASE_TABLE("imp", impdrv_cnnctl_soft_reset),
	TESTCASE_TABLE("imp", impdrv_cnnctl_set_syncc_config),
	TESTCASE_TABLE("imp", impdrv_cnnctl_get_inten_val),
	TESTCASE_TABLE("imp", impdrv_cnnctl_check_inten),
	TESTCASE_TABLE("imp", impdrv_cnnctl_int_cb),
	TESTCASE_TABLE("imp", impdrv_cnnctl_pm_get_state),
	TESTCASE_TABLE("imp", impdrv_cnnctl_quit),

	/* Function impdrv_dmactl_*** */

	TESTCASE_TABLE("imp", impdrv_dmactl_is_valid_core),
	TESTCASE_TABLE("imp", impdrv_dmactl_check_state),
	TESTCASE_TABLE("imp", impdrv_dmactl_set_state),
	TESTCASE_TABLE("imp", impdrv_dmactl_init_start),
	TESTCASE_TABLE("imp", impdrv_dmactl_init_end),
	TESTCASE_TABLE("imp", impdrv_dmactl_start),
	TESTCASE_TABLE("imp", impdrv_dmactl_stop),
	TESTCASE_TABLE("imp", impdrv_dmactl_attr_init),
	TESTCASE_TABLE("imp", impdrv_dmactl_set_mem_init),
	TESTCASE_TABLE("imp", impdrv_dmactl_set_core_map),
	TESTCASE_TABLE("imp", impdrv_dmactl_set_cl),
	TESTCASE_TABLE("imp", impdrv_dmactl_set_irq_mask),
	TESTCASE_TABLE("imp", impdrv_dmactl_execute),
	TESTCASE_TABLE("imp", impdrv_dmactl_int_handler),
	TESTCASE_TABLE("imp", impdrv_dmactl_resume_exe),
	TESTCASE_TABLE("imp", impdrv_dmactl_pm_set_policy),
	TESTCASE_TABLE("imp", impdrv_dmactl_pm_get_policy),
	TESTCASE_TABLE("imp", impdrv_dmactl_conf_reg_check),
	TESTCASE_TABLE("imp", impdrv_dmactl_mb_init),
	TESTCASE_TABLE("imp", impdrv_dmactl_is_sub_thread),
	TESTCASE_TABLE("imp", impdrv_dmactl_osal_core_num),
	TESTCASE_TABLE("imp", impdrv_dmactl_read_reg),
	TESTCASE_TABLE("imp", impdrv_dmactl_write_reg),
	TESTCASE_TABLE("imp", impdrv_dmactl_init_core),
	TESTCASE_TABLE("imp", impdrv_dmactl_check_core_busy),
	TESTCASE_TABLE("imp", impdrv_dmactl_soft_reset),
	TESTCASE_TABLE("imp", impdrv_dmactl_set_syncc_config),
	TESTCASE_TABLE("imp", impdrv_dmactl_get_inten_val),
	TESTCASE_TABLE("imp", impdrv_dmactl_check_inten),
	TESTCASE_TABLE("imp", impdrv_dmactl_int_cb),
	TESTCASE_TABLE("imp", impdrv_dmactl_pm_get_state),
	TESTCASE_TABLE("imp", impdrv_dmactl_quit),
	
#if defined(RCAR_V3H2) || defined(RCAR_V4H) || defined(RCAR_V4H2)
	
	/* Function impdrv_dspctl_*** */
	
	TESTCASE_TABLE("imp", impdrv_dspctl_pm_get_policy),
	TESTCASE_TABLE("imp", impdrv_dspctl_pm_set_policy),
	TESTCASE_TABLE("imp", impdrv_dspctl_load_ptcm),
	TESTCASE_TABLE("imp", impdrv_dspctl_stop_reg),
	TESTCASE_TABLE("imp", impdrv_dspctl_stop),
	TESTCASE_TABLE("imp", impdrv_dspctl_dsp_start_pre),
	TESTCASE_TABLE("imp", impdrv_dspctl_init_end),
	TESTCASE_TABLE("imp", impdrv_dspctl_init_start),
	TESTCASE_TABLE("imp", impdrv_dspctl_init_core),
	TESTCASE_TABLE("imp", impdrv_dspctl_dsp_start),
	TESTCASE_TABLE("imp", impdrv_dspctl_dsp_update_app),
	TESTCASE_TABLE("imp", impdrv_dspctl_dsp_execute),
	TESTCASE_TABLE("imp", impdrv_dspctl_quit),
	TESTCASE_TABLE("imp", impdrv_dspctl_pm_get_state),
	TESTCASE_TABLE("imp", impdrv_dspctl_is_valid_core),
	/* Function impdrv_dmasctl_*** */

	TESTCASE_TABLE("imp", impdrv_dmasctl_init_end),
	TESTCASE_TABLE("imp", impdrv_dmasctl_is_valid_core),
	TESTCASE_TABLE("imp", impdrv_dmasctl_check_state),
	TESTCASE_TABLE("imp", impdrv_dmasctl_set_state),
	TESTCASE_TABLE("imp", impdrv_dmasctl_init_start),
	TESTCASE_TABLE("imp", impdrv_dmasctl_start),
	TESTCASE_TABLE("imp", impdrv_dmasctl_stop),
	TESTCASE_TABLE("imp", impdrv_dmasctl_set_mem_init),
	TESTCASE_TABLE("imp", impdrv_dmasctl_set_core_map),
	TESTCASE_TABLE("imp", impdrv_dmasctl_set_cl),
	TESTCASE_TABLE("imp", impdrv_dmasctl_set_irq_mask),
	TESTCASE_TABLE("imp", impdrv_dmasctl_execute),
	TESTCASE_TABLE("imp", impdrv_dmasctl_int_handler),
	TESTCASE_TABLE("imp", impdrv_dmasctl_resume_exe),
	TESTCASE_TABLE("imp", impdrv_dmasctl_pm_set_policy),
	TESTCASE_TABLE("imp", impdrv_dmasctl_pm_get_policy),
	TESTCASE_TABLE("imp", impdrv_dmasctl_conf_reg_check),
	TESTCASE_TABLE("imp", impdrv_dmasctl_is_sub_thread),
	TESTCASE_TABLE("imp", impdrv_dmasctl_osal_core_num),
	TESTCASE_TABLE("imp", impdrv_dmasctl_read_reg),
	TESTCASE_TABLE("imp", impdrv_dmasctl_write_reg),
	TESTCASE_TABLE("imp", impdrv_dmasctl_init_core),
	TESTCASE_TABLE("imp", impdrv_dmasctl_check_core_busy),
	TESTCASE_TABLE("imp", impdrv_dmasctl_soft_reset),
	TESTCASE_TABLE("imp", impdrv_dmasctl_set_syncc_config),
	TESTCASE_TABLE("imp", impdrv_dmasctl_get_inten_val),
	TESTCASE_TABLE("imp", impdrv_dmasctl_check_inten),
	TESTCASE_TABLE("imp", impdrv_dmasctl_int_cb),
	TESTCASE_TABLE("imp", impdrv_dmasctl_attr_init),
	TESTCASE_TABLE("imp", impdrv_dmasctl_pm_get_state),
	TESTCASE_TABLE("imp", impdrv_dmasctl_quit),
#endif 

#if defined(RCAR_V3H2) || defined(RCAR_V3M2) || defined(RCAR_V3H1)
	TESTCASE_TABLE("imp", impdrv_cmnctl_reg_mb_initialize),
	TESTCASE_TABLE("imp", impdrv_dmactl_bus_if_check),
	TESTCASE_TABLE("imp", impdrv_dmactl_cl_pre),
	TESTCASE_TABLE("imp", impdrv_dmactl_dma_transfer),
	TESTCASE_TABLE("imp", impdrv_dmactl_int_main_func),
	TESTCASE_TABLE("imp", impdrv_dmactl_int_safety_func),
	TESTCASE_TABLE("imp", impdrv_dmactl_mb_init_main),
	TESTCASE_TABLE("imp", impdrv_dmactl_mb_init_pre),
	TESTCASE_TABLE("imp", impdrv_dmactl_tbl_write_reg),
	TESTCASE_TABLE("imp", impdrv_impctl_bus_if_check),
	TESTCASE_TABLE("imp", impdrv_impctl_dma_transfer),
	TESTCASE_TABLE("imp", impdrv_impctl_int_main_func),
	TESTCASE_TABLE("imp", impdrv_impctl_int_safety_func),
	TESTCASE_TABLE("imp", impdrv_impctl_tbl_write_reg),
	TESTCASE_TABLE("imp", impdrv_ocvctl_init_lwm),
#endif

	/* Function impdrv_impctl_*** */
	TESTCASE_TABLE("imp", impdrv_impctl_is_valid_core),
	TESTCASE_TABLE("imp", impdrv_impctl_check_state),
	TESTCASE_TABLE("imp", impdrv_impctl_set_state),
	TESTCASE_TABLE("imp", impdrv_impctl_init_start),
	TESTCASE_TABLE("imp", impdrv_impctl_init_end),
	TESTCASE_TABLE("imp", impdrv_impctl_start),
	TESTCASE_TABLE("imp", impdrv_impctl_stop),
	TESTCASE_TABLE("imp", impdrv_impctl_attr_init),
	TESTCASE_TABLE("imp", impdrv_impctl_set_mem_init),
	TESTCASE_TABLE("imp", impdrv_impctl_set_core_map),
	TESTCASE_TABLE("imp", impdrv_impctl_set_cl),
	TESTCASE_TABLE("imp", impdrv_impctl_set_irq_mask),
	TESTCASE_TABLE("imp", impdrv_impctl_execute),
	TESTCASE_TABLE("imp", impdrv_impctl_int_handler),
	TESTCASE_TABLE("imp", impdrv_impctl_resume_exe),
	TESTCASE_TABLE("imp", impdrv_impctl_pm_set_policy),
	TESTCASE_TABLE("imp", impdrv_impctl_pm_get_policy),
	TESTCASE_TABLE("imp", impdrv_impctl_conf_reg_check),
	TESTCASE_TABLE("imp", impdrv_impctl_init_core),
	TESTCASE_TABLE("imp", impdrv_impctl_check_core_busy),
	TESTCASE_TABLE("imp", impdrv_impctl_soft_reset),
	TESTCASE_TABLE("imp", impdrv_impctl_set_syncc_config),
	TESTCASE_TABLE("imp", impdrv_impctl_get_inten_val),
	TESTCASE_TABLE("imp", impdrv_impctl_check_inten),
	TESTCASE_TABLE("imp", impdrv_impctl_int_cb),
	TESTCASE_TABLE("imp", impdrv_impctl_pm_get_state),
	TESTCASE_TABLE("imp", impdrv_impctl_quit),

	/* Function impdrv_ocvctl_*** */

	TESTCASE_TABLE("imp", impdrv_ocvctl_is_valid_core),
	TESTCASE_TABLE("imp", impdrv_ocvctl_check_state),
	TESTCASE_TABLE("imp", impdrv_ocvctl_set_state),
	TESTCASE_TABLE("imp", impdrv_ocvctl_init_start),
	TESTCASE_TABLE("imp", impdrv_ocvctl_init_end),
	TESTCASE_TABLE("imp", impdrv_ocvctl_start),
	TESTCASE_TABLE("imp", impdrv_ocvctl_stop),
	TESTCASE_TABLE("imp", impdrv_ocvctl_attr_init),
	TESTCASE_TABLE("imp", impdrv_ocvctl_set_mem_init),
	TESTCASE_TABLE("imp", impdrv_ocvctl_set_core_map),
	TESTCASE_TABLE("imp", impdrv_ocvctl_set_cl),
	TESTCASE_TABLE("imp", impdrv_ocvctl_execute),
	TESTCASE_TABLE("imp", impdrv_ocvctl_int_handler),
	TESTCASE_TABLE("imp", impdrv_ocvctl_resume_exe),
	TESTCASE_TABLE("imp", impdrv_ocvctl_pm_set_policy),
	TESTCASE_TABLE("imp", impdrv_ocvctl_pm_get_policy),
	TESTCASE_TABLE("imp", impdrv_ocvctl_conf_reg_check),
	TESTCASE_TABLE("imp", impdrv_ocvctl_set_irq_mask),
	TESTCASE_TABLE("imp", impdrv_ocvctl_init_core),
	TESTCASE_TABLE("imp", impdrv_ocvctl_check_core_busy),
	TESTCASE_TABLE("imp", impdrv_ocvctl_soft_reset),
	TESTCASE_TABLE("imp", impdrv_ocvctl_set_syncc_config),
	TESTCASE_TABLE("imp", impdrv_ocvctl_irq_status_clear),
	TESTCASE_TABLE("imp", impdrv_ocvctl_get_inten_val),
	TESTCASE_TABLE("imp", impdrv_ocvctl_check_inten),
	TESTCASE_TABLE("imp", impdrv_ocvctl_check_inten_1st),
	TESTCASE_TABLE("imp", impdrv_ocvctl_check_inten_2nd),
	TESTCASE_TABLE("imp", impdrv_ocvctl_int_cb),
	TESTCASE_TABLE("imp", impdrv_ocvctl_pm_get_state),
	TESTCASE_TABLE("imp", impdrv_ocvctl_quit),

	/* Function impdrv_pscctl_*** */

	TESTCASE_TABLE("imp", impdrv_pscctl_is_valid_core),
	TESTCASE_TABLE("imp", impdrv_pscctl_check_state),
	TESTCASE_TABLE("imp", impdrv_pscctl_set_state),
	TESTCASE_TABLE("imp", impdrv_pscctl_init_start),
	TESTCASE_TABLE("imp", impdrv_pscctl_start),
	TESTCASE_TABLE("imp", impdrv_pscctl_stop),
	TESTCASE_TABLE("imp", impdrv_pscctl_attr_init),
	TESTCASE_TABLE("imp", impdrv_pscctl_set_mem_init),
	TESTCASE_TABLE("imp", impdrv_pscctl_set_core_map),
	TESTCASE_TABLE("imp", impdrv_pscctl_set_cl),
	TESTCASE_TABLE("imp", impdrv_pscctl_set_irq_mask),
	TESTCASE_TABLE("imp", impdrv_pscctl_int_handler),
	TESTCASE_TABLE("imp", impdrv_pscctl_resume_exe),
	TESTCASE_TABLE("imp", impdrv_pscctl_pm_set_policy),
	TESTCASE_TABLE("imp", impdrv_pscctl_pm_get_policy),
	TESTCASE_TABLE("imp", impdrv_pscctl_conf_reg_check),
	TESTCASE_TABLE("imp", impdrv_pscctl_get_inten_val),
	TESTCASE_TABLE("imp", impdrv_pscctl_check_core_busy),
	TESTCASE_TABLE("imp", impdrv_pscctl_soft_reset),
	TESTCASE_TABLE("imp", impdrv_pscctl_set_syncc_config),
	TESTCASE_TABLE("imp", impdrv_pscctl_int_cb),
	TESTCASE_TABLE("imp", impdrv_pscctl_check_inten),
	TESTCASE_TABLE("imp", impdrv_pscctl_execute),
	TESTCASE_TABLE("imp", impdrv_pscctl_init_end),
	TESTCASE_TABLE("imp", impdrv_pscctl_quit),
	TESTCASE_TABLE("imp", impdrv_pscctl_pm_get_state),

	/* Function impdrv_osdep_*** */

	TESTCASE_TABLE("imp", impdrv_osdep_pow_on_imp_top),
	TESTCASE_TABLE("imp", impdrv_osdep_pow_off_imp_top),
	TESTCASE_TABLE("imp", impdrv_osdep_register_irq),
	TESTCASE_TABLE("imp", impdrv_osdep_enable_irq),
	TESTCASE_TABLE("imp", impdrv_osdep_disable_irq),
	TESTCASE_TABLE("imp", impdrv_osdep_unregister_irq),
	TESTCASE_TABLE("imp", impdrv_osdep_pow_on_imp),
	TESTCASE_TABLE("imp", impdrv_osdep_pow_off_imp),
	TESTCASE_TABLE("imp", impdrv_osdep_pm_set_policy),
	TESTCASE_TABLE("imp", impdrv_osdep_pm_get_policy),
	TESTCASE_TABLE("imp", impdrv_osdep_write_reg),
	TESTCASE_TABLE("imp", impdrv_osdep_read_reg),
	TESTCASE_TABLE("imp", impdrv_osdep_mutex_create),
	TESTCASE_TABLE("imp", impdrv_osdep_mutex_destroy),
	TESTCASE_TABLE("imp", impdrv_osdep_mutex_lock),
	TESTCASE_TABLE("imp", impdrv_osdep_mutex_unlock),
	TESTCASE_TABLE("imp", impdrv_osdep_reset_core),
	TESTCASE_TABLE("imp", impdrv_osdep_pow_on_hwrsc),
	TESTCASE_TABLE("imp", impdrv_osdep_pow_off_hwrsc),
	TESTCASE_TABLE("imp", impdrv_osdep_memory_barrier),
	TESTCASE_TABLE("imp", impdrv_osdep_queue_create),
	TESTCASE_TABLE("imp", impdrv_osdep_queue_destroy),
	TESTCASE_TABLE("imp", impdrv_osdep_queue_wait_period),
	TESTCASE_TABLE("imp", impdrv_osdep_queue_send_period),
	TESTCASE_TABLE("imp", impdrv_osdep_memory_get_size),
	TESTCASE_TABLE("imp", impdrv_osdep_memory_get_logical),
	TESTCASE_TABLE("imp", impdrv_osdep_memory_get_physica),
	TESTCASE_TABLE("imp", impdrv_osdep_memory_cache_flush),
	TESTCASE_TABLE("imp", impdrv_osdep_memory_cache_clean),
	TESTCASE_TABLE("imp", impdrv_osdep_memory_data_set),
	TESTCASE_TABLE("imp", impdrv_osdep_memory_compare),
	TESTCASE_TABLE("imp", impdrv_osdep_dev_open),
	TESTCASE_TABLE("imp", impdrv_osdep_dev_close),
	TESTCASE_TABLE("imp", impdrv_osdep_pow_on),
	TESTCASE_TABLE("imp", impdrv_osdep_pow_off),
	TESTCASE_TABLE("imp", impdrv_osdep_int_handler),
	TESTCASE_TABLE("imp", impdrv_osdep_chk_core_info),
	TESTCASE_TABLE("imp", impdrv_osdep_chk_instance_num),
	TESTCASE_TABLE("imp", impdrv_osdep_get_irq_channel),
	TESTCASE_TABLE("imp", impdrv_osdep_dev_open_imp),
	TESTCASE_TABLE("imp", impdrv_osdep_dev_open_dsp),
	TESTCASE_TABLE("imp", impdrv_osdep_pm_get_state),
	TESTCASE_TABLE("imp", impdrv_osdep_register_dsp_irq),
	TESTCASE_TABLE("imp", impdrv_osdep_reset_core_dsp),
	TESTCASE_TABLE("imp", impdrv_osdep_unregister_dsp_irq),
	TESTCASE_TABLE("imp", impdrv_osdep_wait_for_req_state),
	TESTCASE_TABLE("imp", impdrv_osdep_pow_on_imp_dsp),
	TESTCASE_TABLE("imp", ut_osdep_get_dsp_irq_channel),
	TESTCASE_TABLE("imp", impdrv_osdep_enable_dsp_irq),
	TESTCASE_TABLE("imp", impdrv_osdep_dsp0_int_handler),
	TESTCASE_TABLE("imp", impdrv_osdep_dsp1_int_handler),
	TESTCASE_TABLE("imp", impdrv_osdep_dsp2_int_handler),
	TESTCASE_TABLE("imp", impdrv_osdep_dsp3_int_handler),
	TESTCASE_TABLE("imp", impdrv_osdep_disable_dsp_irq),
	TESTCASE_TABLE("imp", impdrv_osdep_dev_open_dsp_sub),
	
#if defined(RCAR_V4H)
	TESTCASE_TABLE("imp", impdrv_cnnctl_force_reset_v4h1),
#endif /* !defined(RCAR_V4H) */

#if defined(RCAR_V4H) || defined(RCAR_V4H2)
	TESTCASE_TABLE("imp", impdrv_cnnctl_set_extend_config),
	TESTCASE_TABLE("imp", impdrv_cnnctl_set_cl_brk_addr),
	TESTCASE_TABLE("imp", impdrv_cnnctl_set_cond_gosub),

	TESTCASE_TABLE("imp", impdrv_dmactl_set_extend_config),
	TESTCASE_TABLE("imp", impdrv_dmactl_set_cl_brk_addr),
	TESTCASE_TABLE("imp", impdrv_dmactl_set_cond_gosub),

	TESTCASE_TABLE("imp", impdrv_dmasctl_set_extend_confi),
	TESTCASE_TABLE("imp", impdrv_dmasctl_set_cl_brk_addr),
	TESTCASE_TABLE("imp", impdrv_dmasctl_set_cond_gosub),

	TESTCASE_TABLE("imp", impdrv_impctl_set_extend_config),
	TESTCASE_TABLE("imp", impdrv_impctl_set_cl_brk_addr),
	TESTCASE_TABLE("imp", impdrv_impctl_set_cond_gosub),

	TESTCASE_TABLE("imp", impdrv_ocvctl_set_extend_config),
	TESTCASE_TABLE("imp", impdrv_ocvctl_set_cl_brk_addr),
	TESTCASE_TABLE("imp", impdrv_ocvctl_set_cond_gosub),

	TESTCASE_TABLE("imp", impdrv_pscctl_set_extend_config),
	TESTCASE_TABLE("imp", impdrv_pscctl_set_cl_brk_addr),
	TESTCASE_TABLE("imp", impdrv_pscctl_set_cond_gosub),

	TESTCASE_TABLE("imp", impdrv_dspctl_set_state),
	TESTCASE_TABLE("imp", impdrv_dspctl_set_mem_init),

	TESTCASE_TABLE("imp", impdrv_dspctl_attr_init),
	TESTCASE_TABLE("imp", impdrv_dspctl_check_state),
	TESTCASE_TABLE("imp", impdrv_dspctl_chk_execute_data),
	TESTCASE_TABLE("imp", impdrv_dspctl_chk_int_data),
	TESTCASE_TABLE("imp", impdrv_dspctl_conf_reg_check),
	TESTCASE_TABLE("imp", impdrv_dspctl_dsp_execute_pre),
	TESTCASE_TABLE("imp", impdrv_dspctl_dsp_foece_standby),
	TESTCASE_TABLE("imp", impdrv_dspctl_dsp_standby),
	TESTCASE_TABLE("imp", impdrv_dspctl_get_device_io),
	TESTCASE_TABLE("imp", impdrv_dspctl_init_dsp),
	// TESTCASE_TABLE("imp", impdrv_dspctl_is_valid_core),
	TESTCASE_TABLE("imp", impdrv_dspctl_int_handler),
#if !defined(IMPDRV_DSPCRC_DETECT_ADDDEF)
	TESTCASE_TABLE("imp", impdrv_dspctl_crc_sub),
#else
	TESTCASE_TABLE("imp", impdrv_dspctl_crc_sub_macro),
#endif
	/* Function for impdrv_udefctl_*/
	TESTCASE_TABLE("imp", impdrv_udefctl_set_ecm),
#endif
	{ NULL, "", NULL }
};

#ifdef __cplusplus
}
#endif
#endif
