#include "impdrv_util.h"
#include "r_impdrv_cnnctl.h"
#include "r_impdrv_dmactl.h"
#include "r_impdrv_impctl.h"
#include "r_impdrv_impsctl.h"
#include "r_impdrv_ocvctl.h"
#include "r_impdrv_pscctl.h"
#include "r_impdrv_dmasctl.h"

const st_impdrv_corectl_func_t* impdrv_ut_get_corectl_func(
    const e_impdrv_core_type_t  core_type
) 
{
    /** Private static variables */
    static st_impdrv_corectl_func_t s_corectl_func_tbl_ut[IMPDRV_CORE_TYPE_MAX] =
    {
        /** Not Assigned */
        {   NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL
        },
        /** IMPDRV_CORE_TYPE_IMP */
#if !defined(RCAR_V4H) && !defined(RCAR_V4H2)
        {   impdrv_impctl_is_valid_core,    impdrv_impctl_check_state,      impdrv_impctl_set_state,
            impdrv_impctl_init_start,       impdrv_impctl_init_end,         impdrv_impctl_start,
            impdrv_impctl_stop,             impdrv_impctl_attr_init,        impdrv_impctl_set_mem_init,
            impdrv_impctl_set_core_map,     impdrv_impctl_set_cl,           impdrv_impctl_set_irq_mask,
            impdrv_impctl_execute,          impdrv_impctl_int_handler,      impdrv_impctl_resume_exe,
            impdrv_impctl_pm_set_policy,    impdrv_impctl_pm_get_policy,    NULL,
            impdrv_impctl_bus_if_check,     impdrv_impctl_conf_reg_check
        },
#elif defined(RCAR_V4H2)
        {   impdrv_impctl_is_valid_core,    impdrv_impctl_check_state,      impdrv_impctl_set_state,
            impdrv_impctl_init_start,       impdrv_impctl_init_end,         impdrv_impctl_start,
            impdrv_impctl_stop,             impdrv_impctl_attr_init,        impdrv_impctl_set_mem_init,
            impdrv_impctl_set_core_map,     impdrv_impctl_set_cl,           impdrv_impctl_set_irq_mask,
            impdrv_impctl_execute,          impdrv_impctl_int_handler,      impdrv_impctl_resume_exe,
            impdrv_impctl_pm_set_policy,    impdrv_impctl_pm_get_policy,    NULL,
            NULL,                           NULL,                           impdrv_impctl_set_cl_brk_addr,
            impdrv_impctl_set_cond_gosub
        },
#else
        {   impdrv_impctl_is_valid_core,    impdrv_impctl_check_state,      impdrv_impctl_set_state,
            impdrv_impctl_init_start,       impdrv_impctl_init_end,         impdrv_impctl_start,
            impdrv_impctl_stop,             impdrv_impctl_attr_init,        impdrv_impctl_set_mem_init,
            impdrv_impctl_set_core_map,     impdrv_impctl_set_cl,           impdrv_impctl_set_irq_mask,
            impdrv_impctl_execute,          impdrv_impctl_int_handler,      impdrv_impctl_resume_exe,
            impdrv_impctl_pm_set_policy,    impdrv_impctl_pm_get_policy,    NULL,
            NULL,                           impdrv_impctl_conf_reg_check
        },
#endif
        /* IMPDRV_CORE_TYPE_IMP_SLIM */
#if defined(RCAR_V3H2) | defined(RCAR_V3H1)
        {   impdrv_impsctl_is_valid_core,   impdrv_impsctl_check_state,     impdrv_impsctl_set_state,
            impdrv_impsctl_init_start,      impdrv_impsctl_init_end,        impdrv_impsctl_start,
            impdrv_impsctl_stop,            impdrv_impsctl_attr_init,       impdrv_impsctl_set_mem_init,
            impdrv_impsctl_set_core_map,    impdrv_impsctl_set_cl,          impdrv_impsctl_set_irq_mask,
            impdrv_impsctl_execute,         impdrv_impsctl_int_handler,     impdrv_impsctl_resume_exe,
            impdrv_impsctl_pm_set_policy,   impdrv_impsctl_pm_get_policy,   NULL,
            impdrv_impsctl_bus_if_check,    impdrv_impsctl_conf_reg_check
        },
#else
        {   NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL
        },
#endif
        /** IMPDRV_CORE_TYPE_OCV */
        {   impdrv_ocvctl_is_valid_core,    impdrv_ocvctl_check_state,      impdrv_ocvctl_set_state,
            impdrv_ocvctl_init_start,       impdrv_ocvctl_init_end,         impdrv_ocvctl_start,
            impdrv_ocvctl_stop,             impdrv_ocvctl_attr_init,        impdrv_ocvctl_set_mem_init,
            impdrv_ocvctl_set_core_map,     impdrv_ocvctl_set_cl,           impdrv_ocvctl_set_irq_mask,
            impdrv_ocvctl_execute,          impdrv_ocvctl_int_handler,      impdrv_ocvctl_resume_exe,
            impdrv_ocvctl_pm_set_policy,    impdrv_ocvctl_pm_get_policy,    NULL,
            NULL,                           impdrv_ocvctl_conf_reg_check
        },
        /** IMPDRV_CORE_TYPE_DMAC */
#if !defined(RCAR_V4H) && !defined(RCAR_V4H2)
        {   impdrv_dmactl_is_valid_core,    impdrv_dmactl_check_state,      impdrv_dmactl_set_state,
            impdrv_dmactl_init_start,       impdrv_dmactl_init_end,         impdrv_dmactl_start,
            impdrv_dmactl_stop,             impdrv_dmactl_attr_init,        impdrv_dmactl_set_mem_init,
            impdrv_dmactl_set_core_map,     impdrv_dmactl_set_cl,           impdrv_dmactl_set_irq_mask,
            impdrv_dmactl_execute,          impdrv_dmactl_int_handler,      impdrv_dmactl_resume_exe,
            impdrv_dmactl_pm_set_policy,    impdrv_dmactl_pm_get_policy,    NULL,
            impdrv_dmactl_bus_if_check,     impdrv_dmactl_conf_reg_check
        },
#elif defined(RCAR_V4H2)
        {   impdrv_dmactl_is_valid_core,    impdrv_dmactl_check_state,      impdrv_dmactl_set_state,
            impdrv_dmactl_init_start,       impdrv_dmactl_init_end,         impdrv_dmactl_start,
            impdrv_dmactl_stop,             impdrv_dmactl_attr_init,        impdrv_dmactl_set_mem_init,
            impdrv_dmactl_set_core_map,     impdrv_dmactl_set_cl,           impdrv_dmactl_set_irq_mask,
            impdrv_dmactl_execute,          impdrv_dmactl_int_handler,      impdrv_dmactl_resume_exe,
            impdrv_dmactl_pm_set_policy,    impdrv_dmactl_pm_get_policy,    NULL,
            NULL,                           NULL,                           impdrv_dmactl_set_cl_brk_addr,
            impdrv_dmactl_set_cond_gosub
        },
#else
        {   impdrv_dmactl_is_valid_core,    impdrv_dmactl_check_state,      impdrv_dmactl_set_state,
            impdrv_dmactl_init_start,       impdrv_dmactl_init_end,         impdrv_dmactl_start,
            impdrv_dmactl_stop,             impdrv_dmactl_attr_init,        impdrv_dmactl_set_mem_init,
            impdrv_dmactl_set_core_map,     impdrv_dmactl_set_cl,           impdrv_dmactl_set_irq_mask,
            impdrv_dmactl_execute,          impdrv_dmactl_int_handler,      impdrv_dmactl_resume_exe,
            impdrv_dmactl_pm_set_policy,    impdrv_dmactl_pm_get_policy,    NULL,
            NULL,                           impdrv_dmactl_conf_reg_check
        },
#endif
        /* IMPDRV_CORE_TYPE_DMAC_SLIM */
#if RCAR_V3H2
        {   impdrv_dmasctl_is_valid_core,   impdrv_dmasctl_check_state,     impdrv_dmasctl_set_state,
            impdrv_dmasctl_init_start,      impdrv_dmasctl_init_end,        impdrv_dmasctl_start,
            impdrv_dmasctl_stop,            impdrv_dmasctl_attr_init,       impdrv_dmasctl_set_mem_init,
            impdrv_dmasctl_set_core_map,    impdrv_dmasctl_set_cl,          impdrv_dmasctl_set_irq_mask,
            impdrv_dmasctl_execute,         impdrv_dmasctl_int_handler,     impdrv_dmasctl_resume_exe,
            impdrv_dmasctl_pm_set_policy,   impdrv_dmasctl_pm_get_policy,   NULL,
            NULL,                           impdrv_dmasctl_conf_reg_check
        },
#else
        {   NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL
        },
#endif
        /** IMPDRV_CORE_TYPE_PSCEXE */
        {   impdrv_pscctl_is_valid_core,    impdrv_pscctl_check_state,      impdrv_pscctl_set_state,
            impdrv_pscctl_init_start,       impdrv_pscctl_init_end,         impdrv_pscctl_start,
            impdrv_pscctl_stop,             impdrv_pscctl_attr_init,        impdrv_pscctl_set_mem_init,
            impdrv_pscctl_set_core_map,     impdrv_pscctl_set_cl,           impdrv_pscctl_set_irq_mask,
            impdrv_pscctl_execute,          impdrv_pscctl_int_handler,      impdrv_pscctl_resume_exe,
            impdrv_pscctl_pm_set_policy,    impdrv_pscctl_pm_get_policy,    NULL,
            NULL,                           impdrv_pscctl_conf_reg_check
        },
        /** IMPDRV_CORE_TYPE_PSCOUT */
        {   NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL
        },
        /** IMPDRV_CORE_TYPE_CNN */
        {   impdrv_cnnctl_is_valid_core,    impdrv_cnnctl_check_state,      impdrv_cnnctl_set_state,
            impdrv_cnnctl_init_start,       impdrv_cnnctl_init_end,         impdrv_cnnctl_start,
            impdrv_cnnctl_stop,             impdrv_cnnctl_attr_init,        impdrv_cnnctl_set_mem_init,
            impdrv_cnnctl_set_core_map,     impdrv_cnnctl_set_cl,           impdrv_cnnctl_set_irq_mask,
            impdrv_cnnctl_execute,          impdrv_cnnctl_int_handler,      impdrv_cnnctl_resume_exe,
            impdrv_cnnctl_pm_set_policy,    impdrv_cnnctl_pm_get_policy,    NULL,
            NULL,                           impdrv_cnnctl_conf_reg_check
        },
        /** IMPDRV_CORE_TYPE_DSP */
        {   NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL
        },
        /** IMPDRV_CORE_TYPE_DTA */
        {   NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL
        }
    };
    
    /** initialize local variables */
    const st_impdrv_corectl_func_t  *ret_func = NULL;
    
    /** Input parameter confirmation process */
    if ((IMPDRV_CORE_TYPE_IMP == core_type)
        || (IMPDRV_CORE_TYPE_IMP_SLIM == core_type)
        || (IMPDRV_CORE_TYPE_OCV == core_type)
        || (IMPDRV_CORE_TYPE_DMAC == core_type)
        || (IMPDRV_CORE_TYPE_DMAC_SLIM == core_type)
        || (IMPDRV_CORE_TYPE_PSCEXE == core_type)
        || (IMPDRV_CORE_TYPE_PSCOUT == core_type)
        || (IMPDRV_CORE_TYPE_CNN == core_type)
        || (IMPDRV_CORE_TYPE_DTA == core_type)
        || (IMPDRV_CORE_TYPE_DSP == core_type))
    {
        ret_func = &s_corectl_func_tbl_ut[core_type];
    }
    
    /** Return address of the target function table */
    return ret_func;
}

/*st_impdrv_corectl_func_t *callbackfunction( )
{
        return g_p_p_funcs;
}*/

static impdrv_ctrl_handle_t* initialize_core_ctl(st_impdrv_ctl_t *p_handle, e_impdrv_state_t state, e_impdrv_param_t param);
static void set_device_io_dsp(st_impdrv_corectl_t* p_corectl);

static impdrv_ctrl_handle_t* initialize_core_ctl(st_impdrv_ctl_t *p_handle, e_impdrv_state_t state, e_impdrv_param_t param)
{
    e_impdrv_core_type_t core_type_idx = IMPDRV_CORE_TYPE_CNN;
    uint32_t core_num_idx = 0;
    uint32_t max_core_num = 0;

    for(core_type_idx = IMPDRV_CORE_TYPE_IMP; core_type_idx < IMPDRV_CORE_TYPE_MAX; core_type_idx++)
    {
        switch(core_type_idx) {
            case IMPDRV_CORE_TYPE_IMP:
                max_core_num = IMPDRV_IMP_CORENUM_VAL;
                break;
            case IMPDRV_CORE_TYPE_IMP_SLIM:
                max_core_num = IMPDRV_IMPS_CORENUM_VAL;
                break;
            case IMPDRV_CORE_TYPE_OCV:
                max_core_num = IMPDRV_OCV_CORENUM_VAL;
                break;
            case IMPDRV_CORE_TYPE_DMAC:
                max_core_num = IMPDRV_DMA_CORENUM_VAL;
                break;
            case IMPDRV_CORE_TYPE_DMAC_SLIM:
                max_core_num = IMPDRV_DMAS_CORENUM_VAL;
                break;
            case IMPDRV_CORE_TYPE_LDMAC:
                max_core_num = IMPDRV_LDMA_CORENUM_VAL;
                break;
            case IMPDRV_CORE_TYPE_PSCEXE:
            case IMPDRV_CORE_TYPE_PSCOUT:
                max_core_num = IMPDRV_PSC_CORENUM_VAL;
                break;
            case IMPDRV_CORE_TYPE_CNN:
                max_core_num = IMPDRV_CNN_CORENUM_VAL;
                break;
            case IMPDRV_CORE_TYPE_DTA:
                max_core_num = IMPDRV_DTA_CORENUM_VAL;
                break;
            case IMPDRV_CORE_TYPE_DSP:
                max_core_num = IMPDRV_DSP_CORENUM_VAL;
                break;
            default:
                max_core_num = 0;
                break;
        }

        if (max_core_num == 0)
        {
            /* Continue */
        }
        else 
        {
            for(core_num_idx = 0; core_num_idx < max_core_num; core_num_idx++)
            {
                p_handle->coretype_ctl[core_type_idx].core_ctl[core_num_idx].core_state = state;
                p_handle->coretype_ctl[core_type_idx].core_ctl[core_num_idx].core_mem_init_enable = param;
                p_handle->coretype_ctl[core_type_idx].core_ctl[core_num_idx].claddr_phys = 0x000000U;
            }
        }
    }

    return p_handle;
}

static st_impdrv_coretypectl_t* initialize_coretype_ctl(st_impdrv_coretypectl_t *p_coretypectl, uint32_t *core_num_idx, e_impdrv_state_t state, e_impdrv_param_t param, bool value)
{
    uint32_t core_num = **&core_num_idx;
    p_coretypectl->core_ctl[core_num].dsp_info.is_executed = true;/* for DSP control test case */
    p_coretypectl->core_ctl[core_num].dsp_info.dsp_fw.addr_phys = 0x40000000U;/* for DSP control test case */
    p_coretypectl->core_ctl[core_num].dsp_info.dsp_fw.size = 0x2000U;/* for DSP control test case */
    p_coretypectl->core_ctl[core_num].dsp_info.is_update = true;/* for DSP control test case */
    p_coretypectl->core_ctl[core_num].dsp_info.is_loaded = false;/* for DSP control test case */
    p_coretypectl->core_ctl[core_num].core_state = state;
    p_coretypectl->core_ctl[core_num].core_mem_init_enable = param;
    p_coretypectl->core_ctl[core_num].claddr_phys = 0x000080U;
    memset((void*)p_coretypectl->core_ctl[core_num].cur_core_map, 0, sizeof(p_coretypectl->core_ctl[core_num].cur_core_map));
    memset((void*)p_coretypectl->core_ctl[core_num].irq_mask, 0, sizeof(p_coretypectl->core_ctl[core_num].irq_mask));
    memset((void*)p_coretypectl->core_ctl[core_num].param, 0, sizeof(p_coretypectl->core_ctl[core_num].param));
    
    if (value == true)
    { 
        p_coretypectl->core_ctl[core_num].device_io.handle = NULL;
        p_coretypectl->core_ctl[core_num].device_io_dsp[0].handle = NULL;/* for DSP control test case */
        p_coretypectl->core_ctl[core_num].device_io_dsp[1].handle = NULL;/* for DSP control test case */
        p_coretypectl->core_ctl[core_num].device_io_dsp[2].handle = NULL;/* for DSP control test case */
        p_coretypectl->core_ctl[core_num].device_io_dsp[3].handle = NULL;/* for DSP control test case */
        p_coretypectl->core_ctl[core_num].device_io_dsp[4].handle = NULL;/* for DSP control test case */
        p_coretypectl->core_ctl[core_num].device_io_dsp[5].handle = NULL;/* for DSP control test case */
        p_coretypectl->core_ctl[core_num].device_io_dsp[6].handle = NULL;/* for DSP control test case */
        p_coretypectl->core_ctl[core_num].device_io_dsp[7].handle = NULL;/* for DSP control test case */
    }
    else
    {
        p_coretypectl->core_ctl[core_num].device_io.handle = &g_uts_handle;
        p_coretypectl->core_ctl[core_num].device_io_dsp[0].handle = &g_uts_handle;/* for DSP control test case */
        p_coretypectl->core_ctl[core_num].device_io_dsp[1].handle = &g_uts_handle;/* for DSP control test case */
        p_coretypectl->core_ctl[core_num].device_io_dsp[2].handle = &g_uts_handle;/* for DSP control test case */
        p_coretypectl->core_ctl[core_num].device_io_dsp[3].handle = &g_uts_handle;/* for DSP control test case */
        p_coretypectl->core_ctl[core_num].device_io_dsp[4].handle = &g_uts_handle;/* for DSP control test case */
        p_coretypectl->core_ctl[core_num].device_io_dsp[5].handle = &g_uts_handle;/* for DSP control test case */
        p_coretypectl->core_ctl[core_num].device_io_dsp[6].handle = &g_uts_handle;/* for DSP control test case */
        p_coretypectl->core_ctl[core_num].device_io_dsp[7].handle = &g_uts_handle;/* for DSP control test case */
    }

    return p_coretypectl;
}

static st_impdrv_coretypectl_t* initialize_coretype_ctl_nullhandle(st_impdrv_coretypectl_t *p_coretypectl, uint32_t *core_num_idx, e_impdrv_state_t state, e_impdrv_param_t param, bool value, bool flag)
{
    uint32_t core_num = **&core_num_idx;
    p_coretypectl->core_ctl[core_num].core_state = state;
    p_coretypectl->core_ctl[core_num].core_mem_init_enable = param;
    p_coretypectl->core_ctl[core_num].claddr_phys = 0x000080U;
    memset((void*)p_coretypectl->core_ctl[core_num].cur_core_map, 0, sizeof(p_coretypectl->core_ctl[core_num].cur_core_map));
    memset((void*)p_coretypectl->core_ctl[core_num].irq_mask, 0, sizeof(p_coretypectl->core_ctl[core_num].irq_mask));
    memset((void*)p_coretypectl->core_ctl[core_num].param, 0, sizeof(p_coretypectl->core_ctl[core_num].param));
    if (value == true)
    {
        if(core_num > 0)
        {
            p_coretypectl->core_ctl[core_num - 1].device_io.handle = NULL;
        }
        p_coretypectl->core_ctl[core_num].device_io.handle = NULL;
        p_coretypectl->core_ctl[core_num + 1].device_io.handle = NULL;
    }
    else
    {
        if(flag == true)
        {
             if(core_num > 0)
             {
                p_coretypectl->core_ctl[core_num - 1].device_io.handle = NULL;
             }
            p_coretypectl->core_ctl[core_num].device_io.handle = &g_uts_handle;
            p_coretypectl->core_ctl[core_num + 1].device_io.handle = &g_uts_handle;
        }
        else
        {
            if(core_num > 0)
            {
            p_coretypectl->core_ctl[core_num - 1].device_io.handle = &g_uts_handle;
            }
            p_coretypectl->core_ctl[core_num].device_io.handle = &g_uts_handle;
            p_coretypectl->core_ctl[core_num + 1].device_io.handle = &g_uts_handle;
        }
    }

    return p_coretypectl;
}

bool initialize_p_coretypectl_param_off_nullhandle(st_impdrv_coretypectl_t *p_coretypectl, uint32_t* core_num)
{
    p_coretypectl = initialize_coretype_ctl_nullhandle(p_coretypectl, core_num, IMPDRV_STATE_INIT, IMPDRV_PARAM_OFF, false, true);
    return true;
}

bool initialize_p_coretypectl_param_off_nullhandle1(st_impdrv_coretypectl_t *p_coretypectl, uint32_t* core_num)
{
    p_coretypectl = initialize_coretype_ctl_nullhandle(p_coretypectl, core_num, IMPDRV_STATE_INIT, IMPDRV_PARAM_OFF, false, false);
    return true;
}

bool initialize_p_coretypectl_param_off_nullhandle2(st_impdrv_coretypectl_t *p_coretypectl, uint32_t* core_num)
{
    p_coretypectl = initialize_coretype_ctl_nullhandle(p_coretypectl, core_num, IMPDRV_STATE_INIT, IMPDRV_PARAM_OFF, true, false);
    return true;
}

void CbFunc(st_impdrv_core_info_t *core_info, e_impdrv_fatalcode_t code, e_impdrv_errorcode_t ercd, void* p_callback_args)
{
    
}

bool uninit_data_st_impdrv_initdata_t(st_impdrv_initdata_t *p_initdata)
{
    p_initdata->callback_func_fatal = &CbFunc;
    p_initdata->p_work_addr = (void *)&g_p_main_work_addr;

    g_p_main_work_addr.coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].core_state = IMPDRV_STATE_UNINIT;

    return true;
}

bool init_data_st_impdrv_initdata_t(st_impdrv_initdata_t *p_initdata)
{
    p_initdata->callback_func_fatal = &CbFunc;
    p_initdata->p_work_addr = (void *)&g_p_main_work_addr;

    g_p_main_work_addr.coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].core_state = IMPDRV_STATE_INIT;

    return true;
}

bool null_callback_func_st_impdrv_initdata_t(st_impdrv_initdata_t *p_initdata)
{
    p_initdata->callback_func_fatal = NULL;
    p_initdata->p_work_addr = (void *)&g_p_main_work_addr;

    return true;
}

bool invalid_core_type_dta_st_impdrv_initdata_t(st_impdrv_initdata_t *p_initdata)
{
    p_initdata->callback_func_fatal = &CbFunc;
    p_initdata->p_work_addr = (void *)&g_p_main_work_addr;
    p_initdata->core_info->core_type = IMPDRV_CORE_TYPE_DTA;

    return true;
}

bool uninitialized_impdrv_ctrl_handle_t(st_impdrv_ctl_t *p_handle)
{
    p_handle = initialize_core_ctl(p_handle, IMPDRV_STATE_UNINIT, IMPDRV_PARAM_ON);

    g_impdrv_cmn_ctl_ins.mutex_handle.handle = NULL;
    g_impdrv_cmn_ctl_ins.device_io_imptop.handle = NULL;
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].device_io.handle = NULL;

    return true;
}

bool initialized_impdrv_ctrl_handle_t(st_impdrv_ctl_t *p_handle)
{
    p_handle = initialize_core_ctl(p_handle, IMPDRV_STATE_INIT, IMPDRV_PARAM_ON);
    
    g_impdrv_cmn_ctl_ins.mutex_handle.handle = &g_uts_mutex_handle;
    g_impdrv_cmn_ctl_ins.device_io_imptop.handle = &g_uts_dev_handle;
    g_impdrv_cmn_ctl_ins.mutex_time_period = 1;
    impdrv_cmnctl_set_checksum();
    p_handle->common_ctl.instance_num = IMPDRV_INSTANCE_0;
    p_handle->common_ctl.cbfunc_fatal = CbFunc;
    p_handle->common_ctl.osal_cb_args = &g_uts_handle;
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_DSP].core_ctl[0].dsp_info.is_executed = true;/* for DSP control test case */
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_DSP].core_ctl[0].dsp_info.dsp_fw.addr_phys = 0x40000000U;/* for DSP control test case */
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_DSP].core_ctl[0].dsp_info.dsp_fw.size = 0x2000U;/* for DSP control test case */
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_DSP].core_ctl[0].dsp_info.is_update = true;/* for DSP control test case */
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_DSP].core_ctl[0].dsp_info.is_loaded = false;/* for DSP control test case */
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].device_io.handle = NULL;
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].claddr_phys = 0xCCCCCCC;
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_DSP].core_ctl[0].device_io_dsp[0].handle = NULL;
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_DSP].core_ctl[0].device_io_dsp[1].handle = NULL;
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_DSP].core_ctl[0].device_io_dsp[2].handle = NULL;
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_DSP].core_ctl[0].device_io_dsp[3].handle = NULL;
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_DSP].core_ctl[0].device_io_dsp[4].handle = NULL;
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_DSP].core_ctl[0].device_io_dsp[5].handle = NULL;
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_DSP].core_ctl[0].device_io_dsp[6].handle = NULL;
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_DSP].core_ctl[0].device_io_dsp[7].handle = NULL;

    return true;
}

bool exec_impdrv_ctrl_handle_t(st_impdrv_ctl_t *p_handle)
{
    p_handle = initialize_core_ctl(p_handle, IMPDRV_STATE_EXEC, IMPDRV_PARAM_ON);
    
    g_impdrv_cmn_ctl_ins.mutex_handle.handle = &g_uts_mutex_handle;
    g_impdrv_cmn_ctl_ins.device_io_imptop.handle = &g_uts_dev_handle;
    impdrv_cmnctl_set_checksum();
    p_handle->common_ctl.cbfunc_fatal = CbFunc;
    p_handle->common_ctl.instance_num = IMPDRV_INSTANCE_0;

    return true;
}

bool ready_impdrv_ctrl_handle_t(st_impdrv_ctl_t *p_handle)
{
	uint32_t i,j;
    p_handle = initialize_core_ctl(p_handle, IMPDRV_STATE_READY, IMPDRV_PARAM_ON);


	for (i = 0; i < IMPDRV_CORE_TYPE_MAX; i++)
    {
        for (j = 0; j < IMPDRV_CORE_NUM_MAX; j ++)
        {
            p_handle->coretype_ctl[i].core_ctl[j].device_io.handle = &g_uts_dev_handle;
        }
    }
    
    p_handle->common_ctl.use_core_ins = 1;
	p_handle->common_ctl.protect_mode = IMPDRV_PROTECT_EDC;
    g_impdrv_cmn_ctl_ins.mutex_handle.handle = &g_uts_mutex_handle;
    g_impdrv_cmn_ctl_ins.device_io_imptop.handle = &g_uts_dev_handle;
    impdrv_cmnctl_set_checksum();
    p_handle->common_ctl.instance_num = IMPDRV_INSTANCE_0;
    
    for(uint32_t i = 0; i < 16; i++)
    {
        p_handle->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].cur_core_map[i] = 0;
    }
	for (i = 0; i < IMPDRV_CORE_TYPE_MAX; i++)
    {
        for (j = 0; j < IMPDRV_CORE_NUM_MAX; j ++)
        {
            p_handle->coretype_ctl[i].core_ctl[j].core_state = IMPDRV_STATE_READY;
        }
    }
    
    return true;
}

bool int_impdrv_ctrl_handle_t(st_impdrv_ctl_t *p_handle)
{
    p_handle = initialize_core_ctl(p_handle, IMPDRV_STATE_INT, IMPDRV_PARAM_ON);

    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].device_io.handle = &g_uts_dev_handle;
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[1].device_io.handle = &g_uts_dev_handle;
    g_impdrv_cmn_ctl_ins.mutex_handle.handle = &g_uts_mutex_handle;
    g_impdrv_cmn_ctl_ins.device_io_imptop.handle = &g_uts_dev_handle;
    impdrv_cmnctl_set_checksum();
    
    for(uint32_t i = 0; i < 16; i++)
    {
        p_handle->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].cur_core_map[i] = 0;
    }

    return true;
}

bool init_cl_addr_impdrv_ctrl_handle_t(st_impdrv_ctl_t *p_handle)
{
    p_handle = initialize_core_ctl(p_handle, IMPDRV_STATE_READY, IMPDRV_PARAM_ON);

    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].device_io.handle = &g_uts_dev_handle;
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[1].device_io.handle = &g_uts_dev_handle;
	p_handle->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[3].device_io.handle = &g_uts_dev_handle;
    p_handle->common_ctl.protect_mode=IMPDRV_PROTECT_EDC ;
    g_impdrv_cmn_ctl_ins.mutex_handle.handle = &g_uts_mutex_handle;
    g_impdrv_cmn_ctl_ins.device_io_imptop.handle = &g_uts_dev_handle;
    impdrv_cmnctl_set_checksum();
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].claddr_phys = IMPDRV_INIT_CL_ADDR;

    return true;
}

bool invalid_mutex_period_impdrv_ctrl_handle_t(st_impdrv_ctl_t *p_handle)
{
    p_handle = initialize_core_ctl(p_handle, IMPDRV_STATE_INIT, IMPDRV_PARAM_ON);
    g_impdrv_cmn_ctl_ins.mutex_handle.handle = &g_uts_mutex_handle;
    g_impdrv_cmn_ctl_ins.device_io_imptop.handle = &g_uts_dev_handle;
    p_handle->common_ctl.instance_num = IMPDRV_INSTANCE_0;
    g_impdrv_cmn_ctl_ins.mutex_time_period = -1;
    impdrv_cmnctl_set_checksum();

    return true;
}

bool invalid_irq_mask_impdrv_ctrl_handle_t(st_impdrv_ctl_t *p_handle)
{
    p_handle = initialize_core_ctl(p_handle, IMPDRV_STATE_READY, IMPDRV_PARAM_ON);

    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].irq_mask[0] = NULL;
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].device_io.handle = &g_uts_dev_handle;
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[1].device_io.handle = &g_uts_dev_handle;

    return true;
}

bool uninitialize_p_coretypectl(st_impdrv_coretypectl_t *p_coretypectl, uint32_t* core_num)
{
    p_coretypectl = initialize_coretype_ctl(p_coretypectl, core_num, IMPDRV_STATE_UNINIT, IMPDRV_PARAM_ON, true);
    return true;
}

bool initialize_p_coretypectl_dsp_ext(st_impdrv_coretypectl_t *p_coretypectl, uint32_t* core_num)
{
    uint32_t l_core_num = **&core_num;
    p_coretypectl = initialize_coretype_ctl(p_coretypectl, core_num, IMPDRV_STATE_INIT, IMPDRV_PARAM_ON, false);
    p_coretypectl->core_ctl[l_core_num].dsp_info.dsp_fw.addr_phys = 0x00000000U;/* for DSP control test case */
    p_coretypectl->core_ctl[l_core_num].dsp_info.dsp_fw.size = 0x0000U;/* for DSP control test case */
    p_coretypectl->core_ctl[l_core_num].dsp_info.dsp_app.addr_phys = 0x40000000U;/* for DSP control test case */
    p_coretypectl->core_ctl[l_core_num].dsp_info.dsp_app.size = 0x2000U;/* for DSP control test case */
    p_coretypectl->core_ctl[l_core_num].dsp_info.dsp_dtcm.size = 0x2000U;/* for DSP control test case */

    return true;
}
bool initialize_p_coretypectl_dsp_ext1(st_impdrv_coretypectl_t *p_coretypectl, uint32_t* core_num)
{
    uint32_t l_core_num = **&core_num;
    p_coretypectl = initialize_coretype_ctl(p_coretypectl, core_num, IMPDRV_STATE_INIT, IMPDRV_PARAM_ON, false);
    p_coretypectl->core_ctl[l_core_num].dsp_info.dsp_fw.addr_phys = 0xFFFFFFFFU;/* for DSP control test case */
    p_coretypectl->core_ctl[l_core_num].dsp_info.dsp_fw.size = 0x0000U;/* for DSP control test case */
    p_coretypectl->core_ctl[l_core_num].dsp_info.dsp_app.addr_phys = 0x40000000U;/* for DSP control test case */
    p_coretypectl->core_ctl[l_core_num].dsp_info.dsp_app.size = 0x2000U;/* for DSP control test case */
    p_coretypectl->core_ctl[l_core_num].dsp_info.dsp_dtcm.size = 0x2000U;/* for DSP control test case */
    p_coretypectl->core_ctl[l_core_num].dsp_info.is_update = false;/* for DSP control test case */
    p_coretypectl->core_ctl[l_core_num].dsp_info.is_loaded = false;/* for DSP control test case */

    return true;
}
bool initialize_p_coretypectl_dsp_ext2(st_impdrv_coretypectl_t *p_coretypectl, uint32_t* core_num)
{
    uint32_t l_core_num = **&core_num;
    p_coretypectl = initialize_coretype_ctl(p_coretypectl, core_num, IMPDRV_STATE_INIT, IMPDRV_PARAM_ON, false);
    p_coretypectl->core_ctl[l_core_num].dsp_info.dsp_fw.addr_phys = 0x00000000U;/* for DSP control test case */
    p_coretypectl->core_ctl[l_core_num].dsp_info.dsp_fw.size = 0x0000U;/* for DSP control test case */
    p_coretypectl->core_ctl[l_core_num].dsp_info.dsp_app.addr_phys = 0xFFFFFFFFU;/* for DSP control test case */
    p_coretypectl->core_ctl[l_core_num].dsp_info.dsp_app.size = 0x2000U;/* for DSP control test case */
    p_coretypectl->core_ctl[l_core_num].dsp_info.dsp_dtcm.size = 0x2000U;/* for DSP control test case */
    p_coretypectl->core_ctl[l_core_num].dsp_info.is_update = true;/* for DSP control test case */
    p_coretypectl->core_ctl[l_core_num].dsp_info.is_loaded = true;/* for DSP control test case */

    return true;
}
bool initialize_p_coretypectl(st_impdrv_coretypectl_t *p_coretypectl, uint32_t* core_num)
{
    p_coretypectl = initialize_coretype_ctl(p_coretypectl, core_num, IMPDRV_STATE_INIT, IMPDRV_PARAM_ON, false);
    return true;
}

bool initialize_p_coretypectl_null_handle(st_impdrv_coretypectl_t *p_coretypectl, uint32_t* core_num)
{
    p_coretypectl = initialize_coretype_ctl(p_coretypectl, core_num, IMPDRV_STATE_INIT, IMPDRV_PARAM_ON, true);
    return true;
}

bool initialize_p_coretypectl_param_off(st_impdrv_coretypectl_t *p_coretypectl, uint32_t* core_num)
{
    p_coretypectl = initialize_coretype_ctl(p_coretypectl, core_num, IMPDRV_STATE_INIT, IMPDRV_PARAM_OFF, true);
    return true;
}
bool initialize_p_coretypectl_param_off_device_handle(st_impdrv_coretypectl_t *p_coretypectl, uint32_t* core_num)
{
    p_coretypectl = initialize_coretype_ctl(p_coretypectl, core_num, IMPDRV_STATE_INIT, IMPDRV_PARAM_OFF, true);
    if(*core_num > 0)
    {
        p_coretypectl->core_ctl[*core_num - 1].device_io.handle = &g_uts_handle;
    }
    return true;
}
bool initialize_p_coretypectl_pair_thread_num(st_impdrv_coretypectl_t *p_coretypectl, uint32_t* core_num)
{
    p_coretypectl = initialize_coretype_ctl(p_coretypectl, core_num, IMPDRV_STATE_INIT, IMPDRV_PARAM_ON, true);
    if(*core_num > 0)
    {
        p_coretypectl->core_ctl[*core_num - 1].device_io.handle = &g_uts_handle;
    }
    return true;
}

bool ready_p_coretypectl_param_off(st_impdrv_coretypectl_t *p_coretypectl, uint32_t* core_num)
{
    p_coretypectl = initialize_coretype_ctl(p_coretypectl, core_num, IMPDRV_STATE_READY, IMPDRV_PARAM_OFF, false);
    return true;
}
bool ready_coretypectl_param_off_device_handle(st_impdrv_coretypectl_t *p_coretypectl, uint32_t* core_num)
{
    p_coretypectl = initialize_coretype_ctl(p_coretypectl, core_num, IMPDRV_STATE_READY, IMPDRV_PARAM_OFF, false);
    if(*core_num > 0)
    {
        p_coretypectl->core_ctl[*core_num - 1].device_io.handle = &g_uts_handle;
    }
    return true;
}
bool ready_p_coretypectl(st_impdrv_coretypectl_t *p_coretypectl, uint32_t* core_num)
{
    p_coretypectl = initialize_coretype_ctl(p_coretypectl, core_num, IMPDRV_STATE_READY, IMPDRV_PARAM_ON, false);
    return true;
}

bool ready_p_coretypectl_pair_thread_num(st_impdrv_coretypectl_t *p_coretypectl, uint32_t* core_num)
{
    p_coretypectl = initialize_coretype_ctl(p_coretypectl, core_num, IMPDRV_STATE_READY, IMPDRV_PARAM_ON, true);
    if(*core_num > 0)
    {
        p_coretypectl->core_ctl[*core_num - 1].device_io.handle = &g_uts_handle;
    }
    return true;
}

bool ready_st_impdrv_commonctl_chk_t()
{
   g_hwrsc_chk_table.checksum=0;
   g_impdrv_cmn_ctl_chk.checksum=0;

}


bool init_funcs_st_impdrv_corectl_func_t(st_impdrv_corectl_func_t ***p_funcs )
{
    p_funcs[0] = impdrv_ut_get_corectl_func(IMPDRV_CORE_TYPE_IMP);
    p_funcs[1] = impdrv_ut_get_corectl_func(IMPDRV_CORE_TYPE_IMP);
    p_funcs[2] = impdrv_ut_get_corectl_func(IMPDRV_CORE_TYPE_IMP);
    p_funcs[3] = impdrv_ut_get_corectl_func(IMPDRV_CORE_TYPE_IMP);
    return true; 

}
bool null_funcs_st_impdrv_corectl_func_t(st_impdrv_corectl_func_t ***p_funcs )
{
    p_funcs[0] = 0;
    p_funcs[1] = 0;
    p_funcs[2] = 0;
    p_funcs[3] = 0;
    return true; 

}

bool null2_funcs_st_impdrv_corectl_func_t(st_impdrv_corectl_func_t ***p_funcs )
{
    p_funcs[0] = impdrv_ut_get_corectl_func(IMPDRV_CORE_TYPE_IMP);
    p_funcs[0] = &g_p_p_f_funcs_null;
    p_funcs[1] = 0;
    p_funcs[2] = 0;
    p_funcs[3] = 0;
    return true; 

}
bool impdrv_ctl_st_impdrv_ctl_t(st_impdrv_ctl_t *p_impdrv_ctl)
{
    uint32_t i,j;
    uint32_t core_num = 0;

    g_impdrv_cmn_ctl_ins.mutex_handle.handle = &g_uts_mutex_handle;
    g_impdrv_cmn_ctl_ins.device_io_imptop.handle = &g_uts_dev_handle;
    g_impdrv_cmn_ctl_ins.mutex_time_period = 1;
    impdrv_cmnctl_set_checksum();

    p_impdrv_ctl = initialize_coretype_ctl(p_impdrv_ctl, &core_num, IMPDRV_CORE_TYPE_IMP, IMPDRV_PARAM_ON, false); 
    p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].core_mem_init_enable = IMPDRV_PARAM_ON;
    p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].claddr_phys = 0x000000U;
	p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_DMAC].core_ctl[0].device_io.handle = 0;
    p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].device_io.handle = 0;
    p_impdrv_ctl->common_ctl.instance_num = IMPDRV_INSTANCE_0; 
    for (i = 0; i < IMPDRV_CORE_TYPE_MAX; i++)
    {
        for (j = 0; j < IMPDRV_CORE_NUM_MAX; j ++)
        {
           p_impdrv_ctl->coretype_ctl[i].core_ctl[j].core_state = IMPDRV_STATE_INIT;
        }
    }
    return true;
}

bool impdrv_ctl_dsp_int_handler(st_impdrv_ctl_t *p_impdrv_ctl)
{
    uint32_t i,j;
    uint32_t core_num = 1;
    p_corectl_dsp_nth_handle(p_impdrv_ctl,IMPDRV_STATE_EXEC,0,true);
    g_impdrv_cmn_ctl_ins.mutex_handle.handle = &g_uts_mutex_handle;
    g_impdrv_cmn_ctl_ins.device_io_imptop.handle = &g_uts_dev_handle;
    g_impdrv_cmn_ctl_ins.mutex_time_period = 1;
    impdrv_cmnctl_set_checksum();

    p_impdrv_ctl = initialize_coretype_ctl(p_impdrv_ctl, &core_num, IMPDRV_CORE_TYPE_DSP, IMPDRV_PARAM_ON, false); 
    p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_DSP].core_ctl[core_num].core_mem_init_enable = IMPDRV_PARAM_ON;
    p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_DSP].core_ctl[core_num].claddr_phys = 0x000000U;
	p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_DSP].core_ctl[core_num].device_io.handle = 0;
    p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_DSP].core_ctl[core_num].device_io.handle = 0;
    p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_DSP].core_ctl[core_num].cbinfo.cbfunc = &CbFunc;
    p_impdrv_ctl->common_ctl.instance_num = IMPDRV_INSTANCE_0; 
    p_impdrv_ctl->common_ctl.cbfunc_fatal = CbFunc;
    p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_DSP].core_ctl[core_num].device_io_dsp[0].handle = &g_uts_handle;
    p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_DSP].core_ctl[core_num].device_io_dsp[1].handle = &g_uts_handle;
    p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_DSP].core_ctl[core_num].device_io_dsp[2].handle = &g_uts_handle;
    p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_DSP].core_ctl[core_num].device_io_dsp[3].handle = &g_uts_handle;
    p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_DSP].core_ctl[core_num].device_io_dsp[4].handle = &g_uts_handle;
    p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_DSP].core_ctl[core_num].device_io_dsp[5].handle = &g_uts_handle;
    p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_DSP].core_ctl[core_num].device_io_dsp[6].handle = &g_uts_handle;
    p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_DSP].core_ctl[core_num].device_io_dsp[7].handle = &g_uts_handle;
    p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_DSP].core_ctl[core_num].core_state = IMPDRV_STATE_EXEC;
    for (i = 0; i < IMPDRV_CORE_TYPE_MAX; i++)
    {
        for (j = 0; j < IMPDRV_CORE_NUM_MAX; j ++)
        {
           p_impdrv_ctl->coretype_ctl[i].core_ctl[j].core_state = IMPDRV_STATE_EXEC;
        }
    }
    return true;
}

bool uninit_impdrv_ctl_st_impdrv_ctl_t(st_impdrv_ctl_t *p_impdrv_ctl)
{ 
    uint32_t i,j;
    uint32_t core_num = 0;
    
    g_impdrv_cmn_ctl_ins.mutex_handle.handle = &g_uts_mutex_handle;
    g_impdrv_cmn_ctl_ins.device_io_imptop.handle = &g_uts_dev_handle;
    g_impdrv_cmn_ctl_ins.mutex_time_period = 1;
    impdrv_cmnctl_set_checksum();

    p_impdrv_ctl = initialize_coretype_ctl(p_impdrv_ctl, &core_num, IMPDRV_STATE_READY, IMPDRV_PARAM_ON, false);
    p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].core_mem_init_enable = IMPDRV_PARAM_ON;
    p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].claddr_phys = 0x000000U;
          
    for (i = 0; i < IMPDRV_CORE_TYPE_MAX; i++)
    {
        for (j = 0; j < IMPDRV_CORE_NUM_MAX; j ++)
        {
            p_impdrv_ctl->coretype_ctl[i].core_ctl[j].core_state = IMPDRV_STATE_UNINIT;
        }
    }
    return true;
}

bool pfuncinitialized_impdrv_ctrl_handle_t(st_impdrv_ctl_t *p_handle)
{
    p_handle = initialize_core_ctl(p_handle, IMPDRV_STATE_UNINIT, IMPDRV_PARAM_ON);
    
    g_impdrv_cmn_ctl_ins.p_commonctl[1]=0;
    g_impdrv_cmn_ctl_ins.mutex_handle.handle = &g_uts_mutex_handle;
    g_impdrv_cmn_ctl_ins.device_io_imptop.handle = &g_uts_dev_handle;
    g_impdrv_cmn_ctl_ins.mutex_time_period = 0;
    impdrv_cmnctl_set_checksum();
    p_handle->common_ctl.instance_num = IMPDRV_INSTANCE_0;
    p_handle->common_ctl.cbfunc_fatal = CbFunc;
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].device_io.handle = NULL;
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].claddr_phys = 0xCCCCCCC;

    return true;
}

bool ready_impdrv_ctl_st_impdrv_ctl_t(st_impdrv_ctl_t *p_impdrv_ctl)
{ 
    uint32_t i,j;
    uint32_t core_num = 0;

    g_impdrv_cmn_ctl_ins.mutex_handle.handle = &g_uts_mutex_handle;
    g_impdrv_cmn_ctl_ins.device_io_imptop.handle = &g_uts_dev_handle;
    g_impdrv_cmn_ctl_ins.mutex_time_period = 1;
    impdrv_cmnctl_set_checksum();

    p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].device_io.handle = &g_uts_dev_handle;
    p_impdrv_ctl = initialize_coretype_ctl(p_impdrv_ctl, &core_num, IMPDRV_STATE_READY, IMPDRV_PARAM_ON, false);
    p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].core_mem_init_enable = IMPDRV_PARAM_ON;
    p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].claddr_phys = 0x000000U;
    p_impdrv_ctl->common_ctl.instance_num = IMPDRV_INSTANCE_0;      
    for (i = 0; i < IMPDRV_CORE_TYPE_MAX; i++)
    {
        for (j = 0; j < IMPDRV_CORE_NUM_MAX; j ++)
        {
            p_impdrv_ctl->coretype_ctl[i].core_ctl[j].core_state = IMPDRV_STATE_READY;
        }
    }
    return true;
}

bool ready_impdrv_ctl_st_impdrv_ctl_t_for_dsp(st_impdrv_ctl_t *p_impdrv_ctl)
{ 
    uint32_t i,j;
    uint32_t core_num = 0;

    g_impdrv_cmn_ctl_ins.mutex_handle.handle = &g_uts_mutex_handle;
    g_impdrv_cmn_ctl_ins.device_io_imptop.handle = &g_uts_dev_handle;
    g_impdrv_cmn_ctl_ins.mutex_time_period = 1;
    impdrv_cmnctl_set_checksum();

    p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_DSP].core_ctl[0].device_io.handle = &g_uts_dev_handle;
    p_impdrv_ctl = initialize_coretype_ctl(p_impdrv_ctl, &core_num, IMPDRV_STATE_READY, IMPDRV_PARAM_ON, false);
    p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_DSP].core_ctl[0].core_mem_init_enable = IMPDRV_PARAM_ON;
    p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_DSP].core_ctl[0].claddr_phys = 0x000000U;
    p_impdrv_ctl->common_ctl.instance_num = IMPDRV_INSTANCE_0;      
    for (i = 0; i < IMPDRV_CORE_TYPE_MAX; i++)
    {
        for (j = 0; j < IMPDRV_CORE_NUM_MAX; j ++)
        {
            p_impdrv_ctl->coretype_ctl[i].core_ctl[j].core_state = IMPDRV_STATE_READY;
            p_impdrv_ctl->coretype_ctl[i].core_ctl[j].device_io_dsp[0].handle = &g_uts_handle;
        }
    }
    return true;
}



bool ready1_impdrv_ctrl_handle_t(st_impdrv_ctl_t *p_handle)

{
    p_handle = initialize_core_ctl(p_handle, IMPDRV_STATE_READY, IMPDRV_PARAM_ON);
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].device_io.handle = &g_uts_dev_handle;
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[1].device_io.handle = &g_uts_dev_handle;
    p_handle->common_ctl.use_core_ins = 0xFFFFF;
    g_impdrv_cmn_ctl_ins.mutex_handle.handle = &g_uts_mutex_handle;
    g_impdrv_cmn_ctl_ins.device_io_imptop.handle = &g_uts_dev_handle;
    impdrv_cmnctl_set_checksum();
    p_handle->common_ctl.instance_num = IMPDRV_INSTANCE_0;
    for(uint32_t i = 0; i < 16; i++)
    {
         p_handle->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].cur_core_map[i] = 0;

    }
    return true;
}




bool notreadyimpdrv_ctl_st_impdrv_ctl_t(st_impdrv_ctl_t *p_impdrv_ctl)
{

    uint32_t i,j;
    uint32_t core_num = 0;
    p_impdrv_ctl = initialize_coretype_ctl(p_impdrv_ctl, &core_num, IMPDRV_STATE_READY, IMPDRV_PARAM_ON, false);
    p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].core_mem_init_enable = IMPDRV_PARAM_ON;
    g_impdrv_cmn_ctl_ins.mutex_handle.handle = &g_uts_mutex_handle;
    g_impdrv_cmn_ctl_ins.device_io_imptop.handle = &g_uts_dev_handle;
    g_impdrv_cmn_ctl_ins.mutex_time_period = 0;
    p_impdrv_ctl->common_ctl.instance_num = IMPDRV_INSTANCE_0;
    g_impdrv_cmn_ctl_ins.p_commonctl[1]=0;
    impdrv_cmnctl_set_checksum();
    p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].claddr_phys = 0x000000U;
    for (i = 0; i < IMPDRV_CORE_TYPE_MAX; i++)
    {
        for (j = 0; j < IMPDRV_CORE_NUM_MAX; j ++)
        {
           p_impdrv_ctl->coretype_ctl[i].core_ctl[j].core_state = IMPDRV_STATE_INIT;
        }
    }
    return true;
}


bool stateinitialized_impdrv_ctrl_handle_t(st_impdrv_ctl_t *p_handle)
{

    p_handle = initialize_core_ctl(p_handle, IMPDRV_STATE_READY, IMPDRV_PARAM_ON);
    g_impdrv_cmn_ctl_ins.p_commonctl[1]=0;
    g_impdrv_cmn_ctl_ins.mutex_handle.handle = &g_uts_mutex_handle;
    g_impdrv_cmn_ctl_ins.device_io_imptop.handle = &g_uts_dev_handle;
    g_impdrv_cmn_ctl_ins.mutex_time_period = 0;
    impdrv_cmnctl_set_checksum();
    p_handle->common_ctl.instance_num = IMPDRV_INSTANCE_0;
    p_handle->common_ctl.cbfunc_fatal = CbFunc;
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].device_io.handle = NULL;
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].claddr_phys = 0xCCCCCCC;
    memset(&g_hwrsc_mng_table, 1, sizeof(st_impdrv_hwrsc_mng_t));
    g_hwrsc_mng_table.in_use[0]=1;
    return true;

}


bool instance_impdrv_ctl_st_impdrv_ctl_t(st_impdrv_ctl_t *p_impdrv_ctl)
{
    
    uint32_t i,j;
    uint32_t core_num = 0;
    p_impdrv_ctl = initialize_coretype_ctl(p_impdrv_ctl, &core_num, IMPDRV_STATE_READY, IMPDRV_PARAM_ON, false);
    p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].core_mem_init_enable = IMPDRV_PARAM_ON;
    g_impdrv_cmn_ctl_ins.mutex_handle.handle = &g_uts_mutex_handle;
    g_impdrv_cmn_ctl_ins.device_io_imptop.handle = &g_uts_dev_handle;
    g_impdrv_cmn_ctl_ins.mutex_time_period = 0;
    p_impdrv_ctl->common_ctl.instance_num = IMPDRV_INSTANCE_INVALID;
    g_impdrv_cmn_ctl_ins.p_commonctl[1]=0;
    impdrv_cmnctl_set_checksum();
    p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].claddr_phys = 0x000000U;
    for (i = 0; i < IMPDRV_CORE_TYPE_MAX; i++)
    {
        for (j = 0; j < IMPDRV_CORE_NUM_MAX; j ++)
        {
           p_impdrv_ctl->coretype_ctl[i].core_ctl[j].core_state = IMPDRV_STATE_INIT;
        }
    }
    return true;
}



bool instancehandle_impdrv_ctl_st_impdrv_ctl_t(st_impdrv_ctl_t *p_impdrv_ctl)
{

    uint32_t i,j;
    uint32_t core_num = 0;
    p_impdrv_ctl = initialize_coretype_ctl(p_impdrv_ctl, &core_num, IMPDRV_STATE_UNINIT, IMPDRV_PARAM_ON, false);
    p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].core_mem_init_enable = IMPDRV_PARAM_ON;
    g_impdrv_cmn_ctl_ins.mutex_handle.handle = &g_uts_mutex_handle;
    g_impdrv_cmn_ctl_ins.device_io_imptop.handle = &g_uts_dev_handle;
    g_impdrv_cmn_ctl_ins.mutex_time_period = 0;
    p_impdrv_ctl->common_ctl.instance_num = IMPDRV_INSTANCE_INVALID;
    g_impdrv_cmn_ctl_ins.p_commonctl[1]=0;
    impdrv_cmnctl_set_checksum();
    p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].claddr_phys = 0x000000U;
    for (i = 0; i < IMPDRV_CORE_TYPE_MAX; i++)
    {
        for (j = 0; j < IMPDRV_CORE_NUM_MAX; j ++)
        {
           p_impdrv_ctl->coretype_ctl[i].core_ctl[j].core_state = IMPDRV_STATE_INIT;
        }
    }
    return true;
}

bool stateuninitialized_impdrv_ctrl_handle_t(st_impdrv_ctl_t *p_handle)
{

    p_handle = initialize_core_ctl(p_handle, IMPDRV_STATE_UNINIT, IMPDRV_PARAM_ON);
    g_impdrv_cmn_ctl_ins.p_commonctl[1]=0;
    g_impdrv_cmn_ctl_ins.mutex_handle.handle = &g_uts_mutex_handle;
    g_impdrv_cmn_ctl_ins.device_io_imptop.handle = &g_uts_dev_handle;
    g_impdrv_cmn_ctl_ins.mutex_time_period = 0;
    impdrv_cmnctl_set_checksum();
    p_handle->common_ctl.instance_num = IMPDRV_INSTANCE_0;
    p_handle->common_ctl.cbfunc_fatal = CbFunc;
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].device_io.handle = NULL;
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].claddr_phys = 0xCCCCCCC;
    memset(&g_hwrsc_mng_table, 1, sizeof(st_impdrv_hwrsc_mng_t));
    g_hwrsc_mng_table.in_use[0]=1;
    return true;

}

bool chkinitialized_impdrv_ctrl_handle_t(st_impdrv_ctl_t *handle)

{

    handle = initialize_core_ctl(handle, IMPDRV_STATE_READY, IMPDRV_PARAM_ON);
    g_impdrv_cmn_ctl_ins.p_commonctl[1]=0;
    g_impdrv_cmn_ctl_ins.mutex_handle.handle = &g_uts_mutex_handle;
    g_impdrv_cmn_ctl_ins.device_io_imptop.handle = &g_uts_dev_handle;
    g_impdrv_cmn_ctl_ins.mutex_time_period = 0;
    impdrv_cmnctl_set_checksum();
    handle->common_ctl.instance_num = IMPDRV_INSTANCE_0;
    handle->common_ctl.cbfunc_fatal = CbFunc;
    handle->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].device_io.handle = NULL;
    handle->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].claddr_phys = 0xCCCCCCC;
    memset(&g_hwrsc_mng_table, 1, sizeof(st_impdrv_hwrsc_mng_t));
    return true;

}

bool pimpinit_impdrv_ctl_st_impdrv_ctl_t(st_impdrv_ctl_t *p_impdrv_ctl)

{

  uint32_t i,j;

    uint32_t core_num = 0;

    p_impdrv_ctl = initialize_coretype_ctl(p_impdrv_ctl, &core_num, IMPDRV_STATE_UNINIT, IMPDRV_PARAM_ON, false);

    p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].core_mem_init_enable = IMPDRV_PARAM_ON;

    g_impdrv_cmn_ctl_ins.mutex_handle.handle = &g_uts_mutex_handle;

    g_impdrv_cmn_ctl_ins.device_io_imptop.handle = &g_uts_dev_handle;

    g_impdrv_cmn_ctl_ins.mutex_time_period = 0;

    p_impdrv_ctl->common_ctl.instance_num = IMPDRV_INSTANCE_INVALID;

    g_impdrv_cmn_ctl_ins.p_commonctl[1]=0;
    impdrv_cmnctl_set_checksum();

    p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].claddr_phys = 0x000000U;

     memset(&g_hwrsc_mng_table, 1, sizeof(st_impdrv_hwrsc_mng_t));

    g_hwrsc_mng_table.in_use[0]=1;

    for (i = 0; i < IMPDRV_CORE_TYPE_MAX; i++)   
		{

        for (j = 0; j < IMPDRV_CORE_NUM_MAX; j ++)

        {

           p_impdrv_ctl->coretype_ctl[i].core_ctl[j].core_state = IMPDRV_STATE_UNINIT;

        }

    }

    return true;

}



bool pfunc1initialized_impdrv_ctrl_handle_t(st_impdrv_ctl_t *p_handle)
{
    p_handle = initialize_core_ctl(p_handle, IMPDRV_STATE_UNINIT, IMPDRV_PARAM_ON);
    g_impdrv_cmn_ctl_ins.p_commonctl[1]=0;
    g_impdrv_cmn_ctl_ins.mutex_handle.handle = &g_uts_mutex_handle;
    g_impdrv_cmn_ctl_ins.device_io_imptop.handle = &g_uts_dev_handle;
    g_impdrv_cmn_ctl_ins.mutex_time_period = 0;
    impdrv_cmnctl_set_checksum();
    g_hwrsc_mng_table.handle[0].handle = &g_uts_dev_handle;
    p_handle->common_ctl.instance_num = IMPDRV_INSTANCE_0;
    p_handle->common_ctl.cbfunc_fatal = CbFunc;
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].device_io.handle = NULL;
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].claddr_phys = 0xCCCCCCC;
    memset(&g_hwrsc_mng_table, 1, sizeof(st_impdrv_hwrsc_mng_t));
    g_hwrsc_mng_table.in_use[0]=1;
    return true;
}





bool pimpinit1_impdrv_ctl_st_impdrv_ctl_t(st_impdrv_ctl_t *p_impdrv_ctl)

{

  uint32_t i,j;

    uint32_t core_num = 0;

    p_impdrv_ctl = initialize_coretype_ctl(p_impdrv_ctl, &core_num, IMPDRV_STATE_UNINIT, IMPDRV_PARAM_ON, false);

    p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].core_mem_init_enable = IMPDRV_PARAM_ON;

    g_impdrv_cmn_ctl_ins.mutex_handle.handle = &g_uts_mutex_handle;
    g_impdrv_cmn_ctl_ins.device_io_imptop.handle = &g_uts_dev_handle;
    g_impdrv_cmn_ctl_ins.mutex_time_period = 0;
    g_hwrsc_mng_table.handle[0].handle = &g_uts_dev_handle;
    
    p_impdrv_ctl->common_ctl.instance_num = IMPDRV_INSTANCE_INVALID;

    g_impdrv_cmn_ctl_ins.p_commonctl[1]=0;

    impdrv_cmnctl_set_checksum();
    p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].claddr_phys = 0x000000U;

     memset(&g_hwrsc_mng_table, 1, sizeof(st_impdrv_hwrsc_mng_t));

    g_hwrsc_mng_table.in_use[0]=1;

    for (i = 0; i < IMPDRV_CORE_TYPE_MAX; i++)   
		{

        for (j = 0; j < IMPDRV_CORE_NUM_MAX; j ++)

        {

           p_impdrv_ctl->coretype_ctl[i].core_ctl[j].core_state = IMPDRV_STATE_UNINIT;

        }

    }

    return true;

}


bool uninit_funcs_st_impdrv_corectl_func_t(st_impdrv_corectl_func_t ***p_funcs)
{
    st_impdrv_corectl_func_t data={1};
    st_impdrv_corectl_func_t *pp_data=&data;
    st_impdrv_corectl_func_t *p_data=&pp_data;
    p_funcs[0]=&p_data;
    p_funcs[1]=&p_data;
    p_funcs[2]=&p_data;
    p_funcs[3]=&p_data;
    return true;

}




bool initialize_p_coretypectl_param_on(st_impdrv_coretypectl_t *p_coretypectl, uint32_t* core_num)
{

    p_coretypectl = initialize_coretype_ctl(p_coretypectl, core_num, IMPDRV_STATE_INIT, IMPDRV_PARAM_ON, true);

    return true;

}

bool initialized_impdrv_ctrl_handle_t1(st_impdrv_ctl_t *p_handle)

{

    p_handle = initialize_core_ctl(p_handle, IMPDRV_STATE_EXEC, IMPDRV_PARAM_ON);
    g_impdrv_cmn_ctl_ins.mutex_handle.handle = &g_uts_mutex_handle;
    g_impdrv_cmn_ctl_ins.device_io_imptop.handle = &g_uts_dev_handle;
    g_impdrv_cmn_ctl_ins.mutex_time_period = 1;
    impdrv_cmnctl_set_checksum();
    p_handle->common_ctl.instance_num = IMPDRV_INSTANCE_0;
    p_handle->common_ctl.cbfunc_fatal = CbFunc;
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].device_io.handle = &g_uts_dev_handle;
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].claddr_phys = 0xCCCCCCC;
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].cbinfo.cbfunc=CbFunc;
    return true;

}
bool stateinitialized_instance_impdrv_ctrl_handle_t(st_impdrv_ctl_t *p_handle)
{

    p_handle = initialize_core_ctl(p_handle, IMPDRV_STATE_READY, IMPDRV_PARAM_ON);
    g_impdrv_cmn_ctl_ins.p_commonctl[1] = 0;
    g_impdrv_cmn_ctl_ins.mutex_handle.handle = &g_uts_mutex_handle;
    g_impdrv_cmn_ctl_ins.device_io_imptop.handle = &g_uts_dev_handle;
    g_impdrv_cmn_ctl_ins.mutex_time_period = 0;
    impdrv_cmnctl_set_checksum();
    p_handle->common_ctl.instance_num = IMPDRV_INSTANCE_6;
    p_handle->common_ctl.cbfunc_fatal = CbFunc;
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].device_io.handle = NULL;
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].claddr_phys = 0xCCCCCCC;
    memset(&g_hwrsc_mng_table, 1, sizeof(st_impdrv_hwrsc_mng_t));
    g_hwrsc_mng_table.in_use[0] = 1;
    return true;
}

bool initialized_instance_impdrv_ctrl_handle_t (st_impdrv_ctl_t *p_handle)
{
     p_handle = initialize_core_ctl(p_handle, IMPDRV_STATE_INIT, IMPDRV_PARAM_ON);
    g_impdrv_cmn_ctl_ins.mutex_handle.handle = &g_uts_mutex_handle;
    g_impdrv_cmn_ctl_ins.device_io_imptop.handle = &g_uts_dev_handle;
    g_impdrv_cmn_ctl_ins.mutex_time_period = 1;
    impdrv_cmnctl_set_checksum();
    p_handle->common_ctl.instance_num = IMPDRV_INSTANCE_6;
    p_handle->common_ctl.cbfunc_fatal = CbFunc;
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].device_io.handle = NULL;
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_IMP].core_ctl[0].claddr_phys = 0xCCCCCCC;

    return true;
}

bool deinit_device_dsp_handle(st_impdrv_device_handle_t **p_device_io)
{
    p_device_io[0] = &g_uts_device_io;
    p_device_io[1] = &g_uts_device_io;
    p_device_io[2] = &g_uts_device_io;
    p_device_io[3] = &g_uts_device_io;
    p_device_io[4] = &g_uts_device_io;
    p_device_io[5] = &g_uts_device_io;
    p_device_io[6] = &g_uts_device_io;
    p_device_io[7] = &g_uts_device_io;
    
    p_device_io[0]->handle = NULL;
    p_device_io[1]->handle = NULL;
    p_device_io[2]->handle = NULL;
    p_device_io[3]->handle = NULL;
    p_device_io[4]->handle = NULL;
    p_device_io[5]->handle = NULL;
    p_device_io[6]->handle = NULL;
    p_device_io[7]->handle = NULL;
    return true;
}

bool init_device_dsp_handle(st_impdrv_device_handle_t **p_device_io)
{
    p_device_io[0] = &g_uts_device_io;
    p_device_io[1] = &g_uts_device_io;
    p_device_io[2] = &g_uts_device_io;
    p_device_io[3] = &g_uts_device_io;
    p_device_io[4] = &g_uts_device_io;
    p_device_io[5] = &g_uts_device_io;
    p_device_io[6] = &g_uts_device_io;
    p_device_io[7] = &g_uts_device_io;
    
    p_device_io[0]->handle = &g_uts_handle;
    p_device_io[1]->handle = &g_uts_handle;
    p_device_io[2]->handle = &g_uts_handle;
    p_device_io[3]->handle = &g_uts_handle;
    p_device_io[4]->handle = &g_uts_handle;
    p_device_io[5]->handle = &g_uts_handle;
    p_device_io[6]->handle = &g_uts_handle;
    p_device_io[7]->handle = &g_uts_handle;
    return true;
}

bool initialize_p_corectl(st_impdrv_corectl_t* p_corectl, uint32_t core_num)
{
    p_corectl = initialize_corectl(p_corectl, IMPDRV_STATE_INIT, IMPDRV_PARAM_ON, false);
    return true;
}

bool uninitialize_p_corectl(st_impdrv_corectl_t* p_corectl, uint32_t core_num)
{
    p_corectl = initialize_corectl(p_corectl, IMPDRV_STATE_UNINIT, IMPDRV_PARAM_ON, false);
    return true;
}

bool ready_p_corectl_dsp_standby(st_impdrv_corectl_t* p_corectl, uint32_t core_num)
{
    p_corectl = initialize_corectl(p_corectl, IMPDRV_STATE_READY, IMPDRV_PARAM_ON, true);
    p_corectl->device_io_dsp[0].handle = &g_uts_handle;
    p_corectl->device_io_dsp[1].handle = &g_uts_handle;
    p_corectl->device_io_dsp[2].handle = &g_uts_handle;
    p_corectl->device_io_dsp[3].handle = &g_uts_handle;
    p_corectl->device_io_dsp[4].handle = &g_uts_handle;
    p_corectl->device_io_dsp[5].handle = &g_uts_handle;
    p_corectl->device_io_dsp[6].handle = &g_uts_handle;
    p_corectl->device_io_dsp[7].handle = &g_uts_handle;
    p_corectl->dsp_info.dsp_fw.addr_phys = 0x40000000U;
    p_corectl->dsp_info.dsp_fw.size = 0x2000U;
    p_corectl->dsp_info.is_update = true;
    p_corectl->dsp_info.is_loaded = true;
    p_corectl->dsp_info.is_executed = true;
    return true;
}

bool ready_p_corectl_dsp_running(st_impdrv_corectl_t* p_corectl, uint32_t core_num)
{
    p_corectl = initialize_corectl(p_corectl, IMPDRV_STATE_READY, IMPDRV_PARAM_ON, true);
    p_corectl->device_io_dsp[0].handle = &g_uts_handle;
    p_corectl->device_io_dsp[1].handle = &g_uts_handle;
    p_corectl->device_io_dsp[2].handle = &g_uts_handle;
    p_corectl->device_io_dsp[3].handle = &g_uts_handle;
    p_corectl->device_io_dsp[4].handle = &g_uts_handle;
    p_corectl->device_io_dsp[5].handle = &g_uts_handle;
    p_corectl->device_io_dsp[6].handle = &g_uts_handle;
    p_corectl->device_io_dsp[7].handle = &g_uts_handle;
    p_corectl->dsp_info.dsp_fw.addr_phys = 0x40000000U;
    p_corectl->dsp_info.dsp_fw.size = 0x2000U;
    p_corectl->dsp_info.is_update = true;
    p_corectl->dsp_info.is_loaded = false;
    p_corectl->dsp_info.is_executed = false;
    return true;
}
bool ready_corectl_dsp_not_update_app(st_impdrv_corectl_t* p_corectl, uint32_t core_num)
{
    p_corectl = initialize_corectl(p_corectl, IMPDRV_STATE_READY, IMPDRV_PARAM_ON, true);
    p_corectl->device_io_dsp[0].handle = &g_uts_handle;
    p_corectl->device_io_dsp[1].handle = &g_uts_handle;
    p_corectl->device_io_dsp[2].handle = &g_uts_handle;
    p_corectl->device_io_dsp[3].handle = &g_uts_handle;
    p_corectl->device_io_dsp[4].handle = &g_uts_handle;
    p_corectl->device_io_dsp[5].handle = &g_uts_handle;
    p_corectl->device_io_dsp[6].handle = &g_uts_handle;
    p_corectl->device_io_dsp[7].handle = &g_uts_handle;
    p_corectl->dsp_info.dsp_fw.addr_phys = 0x40000000U;
    p_corectl->dsp_info.dsp_fw.size = 0x2000U;
    p_corectl->dsp_info.is_update = false;
    p_corectl->dsp_info.is_loaded = false;
    p_corectl->dsp_info.is_executed = true;
    return true;
}

static st_impdrv_coretypectl_t* initialize_corectl(st_impdrv_corectl_t *p_corectl, e_impdrv_state_t state, e_impdrv_param_t param, bool value)
{
    p_corectl->dsp_info.is_executed = true;/* for DSP control test case */
    p_corectl->device_io_dsp[0].handle = NULL;/* for DSP control test case */
    p_corectl->device_io_dsp[1].handle = NULL;/* for DSP control test case */
    p_corectl->device_io_dsp[2].handle = NULL;/* for DSP control test case */
    p_corectl->device_io_dsp[3].handle = NULL;/* for DSP control test case */
    p_corectl->device_io_dsp[4].handle = NULL;/* for DSP control test case */
    p_corectl->device_io_dsp[5].handle = NULL;/* for DSP control test case */
    p_corectl->device_io_dsp[6].handle = NULL;/* for DSP control test case */
    p_corectl->device_io_dsp[7].handle = NULL;/* for DSP control test case */
    p_corectl->core_state = state;
    p_corectl->core_mem_init_enable = param;
    p_corectl->claddr_phys = 0x000080U;
    memset((void*)p_corectl->cur_core_map, 0, sizeof(p_corectl->cur_core_map));
    memset((void*)p_corectl->irq_mask, 0, sizeof(p_corectl->irq_mask));
    memset((void*)p_corectl->param, 0, sizeof(p_corectl->param));
    
    if (value == true)
    { 
        p_corectl->device_io.handle = NULL;
    }
    else
    {
        p_corectl->device_io.handle = &g_uts_handle;
    }

    return p_corectl;
}
#if RCAR_V3H2
e_impdrv_errorcode_t impdrv_ocvctl_init_lwm(
    const st_impdrv_corectl_t   *const p_corectl
)
{
    e_impdrv_errorcode_t ercd = IMPDRV_EC_OK;
    return ercd;
}
#endif

bool set_p_corectl_dsp_cbfunc(st_impdrv_corectl_t* p_corectl)
{
    p_corectl->cbinfo.cbfunc = &CbFunc;

    return true;
}

bool unset_p_corectl_dsp_cbfunc(st_impdrv_corectl_t* p_corectl)
{
    p_corectl->cbinfo.cbfunc = NULL;

    return true;
}

static void set_device_io_dsp(st_impdrv_corectl_t* p_corectl)
{
    p_corectl->device_io_dsp[0].handle = &g_uts_handle;
    p_corectl->device_io_dsp[1].handle = &g_uts_handle;
    p_corectl->device_io_dsp[2].handle = &g_uts_handle;
    p_corectl->device_io_dsp[3].handle = &g_uts_handle;
    p_corectl->device_io_dsp[4].handle = &g_uts_handle;
    p_corectl->device_io_dsp[5].handle = &g_uts_handle;
    p_corectl->device_io_dsp[6].handle = &g_uts_handle;
    p_corectl->device_io_dsp[7].handle = &g_uts_handle;
}

bool uninit_p_corectl_dsp_execute_pre(st_impdrv_corectl_t* p_corectl)
{
    p_corectl->core_state = IMPDRV_STATE_UNINIT;
    set_device_io_dsp(p_corectl);

    return true;
}

bool init_p_corectl_dsp_execute_pre(st_impdrv_corectl_t* p_corectl)
{
    p_corectl->core_state = IMPDRV_STATE_INIT;
    set_device_io_dsp(p_corectl);

    return true;
}

bool ready_p_corectl_dsp_execute_pre(st_impdrv_corectl_t* p_corectl)
{
    p_corectl->core_state = IMPDRV_STATE_READY;
    set_device_io_dsp(p_corectl);

    return true;
}

bool exec_p_corectl_dsp_execute_pre(st_impdrv_corectl_t* p_corectl)
{
    p_corectl->core_state = IMPDRV_STATE_EXEC;
    set_device_io_dsp(p_corectl);

    return true;
}

bool int_p_corectl_dsp_execute_pre(st_impdrv_corectl_t* p_corectl)
{
    p_corectl->core_state = IMPDRV_STATE_INT;
    set_device_io_dsp(p_corectl);

    return true;
}

bool invalid_p_corectl_dsp_execute_pre(st_impdrv_corectl_t* p_corectl)
{
    p_corectl->core_state = (-1);
    set_device_io_dsp(p_corectl);

    return true;
}

bool valid_p_corectl_execute_data_dsp_info(st_impdrv_corectl_t* p_corectl)
{
    set_device_io_dsp(p_corectl);
    p_corectl->dsp_info.dsp_app.addr_phys   = 0x40000000U;
    p_corectl->dsp_info.dsp_app.size        = 0x2000U;

    return true;
}

bool invalid_p_corectl_execute_data_dsp_info(st_impdrv_corectl_t* p_corectl)
{
    set_device_io_dsp(p_corectl);
    p_corectl->dsp_info.dsp_app.addr_phys   = 0xFFFFFFFFU;
    p_corectl->dsp_info.dsp_app.size        = 0x2000U;

    return true;
}

bool deinit_device_dsp_io_nth(st_impdrv_device_handle_t **p_device_io, uint32_t index)
{
    p_device_io[0] = &g_uts_device_io;
    p_device_io[1] = &g_uts_device_io;
    p_device_io[2] = &g_uts_device_io;
    p_device_io[3] = &g_uts_device_io;
    p_device_io[4] = &g_uts_device_io;
    p_device_io[5] = &g_uts_device_io;
    p_device_io[6] = &g_uts_device_io;
    p_device_io[7] = &g_uts_device_io;

    p_device_io[index] = NULL;

    return true;
}

bool deinit_device_dsp_io_nth_handle(st_impdrv_device_handle_t **p_device_io, uint32_t index)
{
    p_device_io[0] = &g_uts_device_io;
    p_device_io[1] = &g_uts_device_io;
    p_device_io[2] = &g_uts_device_io;
    p_device_io[3] = &g_uts_device_io;
    p_device_io[4] = &g_uts_device_io;
    p_device_io[5] = &g_uts_device_io;
    p_device_io[6] = &g_uts_device_io;
    p_device_io[7] = &g_uts_device_io;
    
    p_device_io[0]->handle = &g_uts_handle;
    p_device_io[1]->handle = &g_uts_handle;
    p_device_io[2]->handle = &g_uts_handle;
    p_device_io[3]->handle = &g_uts_handle;
    p_device_io[4]->handle = &g_uts_handle;
    p_device_io[5]->handle = &g_uts_handle;
    p_device_io[6]->handle = &g_uts_handle;
    p_device_io[7]->handle = &g_uts_handle;

    p_device_io[index]->handle = NULL;

    return true;
}

bool p_coretypectl_dsp_nth_handle(st_impdrv_coretypectl_t *p_coretypectl, uint32_t core_num, e_impdrv_state_t state, uint32_t index, bool null_flag)
{
    p_coretypectl->core_ctl[core_num].core_state = state;

    for (int32_t i = 0; i < (sizeof(p_coretypectl->core_ctl[core_num].device_io_dsp) / sizeof(p_coretypectl->core_ctl[core_num].device_io_dsp[0])); i++)
    {
        p_coretypectl->core_ctl[core_num].device_io_dsp[i].handle   = &g_uts_handle;
    }

    if (true == null_flag)
    {
        p_coretypectl->core_ctl[core_num].device_io_dsp[index].handle   = NULL;
    }

    return true;
}

bool p_corectl_dsp_nth_handle(st_impdrv_corectl_t* p_corectl, e_impdrv_state_t state, uint32_t index, bool null_flag)
{
    p_corectl->core_state = state;
    p_corectl->cbinfo.cbfunc = &CbFunc;
    for (int32_t i = 0; i < (sizeof(p_corectl->device_io_dsp) / sizeof(p_corectl->device_io_dsp[0])); i++)
    {
        p_corectl->device_io_dsp[i].handle   = &g_uts_handle;
    }

    if (true == null_flag)
    {
        p_corectl->device_io_dsp[index].handle   = NULL;
    }

    return true;
}

bool p_coretypectl_dsp_nth_nullhandle(st_impdrv_coretypectl_t *p_coretypectl, uint32_t core_num, e_impdrv_state_t state)
{
    p_coretypectl->core_ctl[core_num].core_state = state;

    for (int32_t i = 0; i < (sizeof(p_coretypectl->core_ctl[core_num].device_io_dsp) / sizeof(p_coretypectl->core_ctl[core_num].device_io_dsp[0])); i++)
    {
        p_coretypectl->core_ctl[core_num].device_io_dsp[i].handle   = NULL;
    }

    return true;
}

bool crc_impdrv_ctrl_handle_t(st_impdrv_ctl_t *p_handle)
{

    uint32_t i,j;
    uint32_t core_num = 0;

    g_impdrv_cmn_ctl_ins.mutex_handle.handle = &g_uts_mutex_handle;
    g_impdrv_cmn_ctl_ins.device_io_imptop.handle = &g_uts_dev_handle;
    g_impdrv_cmn_ctl_ins.mutex_time_period = 1;
    // impdrv_cmnctl_set_checksum();

    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_DSP].core_ctl[0].device_io.handle = &g_uts_dev_handle;
    p_handle = initialize_coretype_ctl(p_handle, &core_num, IMPDRV_STATE_READY, IMPDRV_PARAM_ON, false);
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_DSP].core_ctl[0].core_mem_init_enable = IMPDRV_PARAM_ON;
    p_handle->coretype_ctl[IMPDRV_CORE_TYPE_DSP].core_ctl[0].claddr_phys = 0x000000U;
    p_handle->common_ctl.instance_num = IMPDRV_INSTANCE_0;      
    for (i = 0; i < IMPDRV_CORE_TYPE_MAX; i++)
    {
        for (j = 0; j < IMPDRV_CORE_NUM_MAX; j ++)
        {
            p_handle->coretype_ctl[i].core_ctl[j].core_state = IMPDRV_STATE_READY;
            p_handle->coretype_ctl[i].core_ctl[j].device_io_dsp[0].handle = &g_uts_handle;
        }
    }
    return true;

}

bool dsp_io_null_handle(st_impdrv_device_handle_t **p_device_io )
{
    p_device_io[0]->handle = NULL;
    p_device_io[1]->handle = NULL;
    p_device_io[2]->handle = NULL;
    p_device_io[3]->handle = NULL;
    p_device_io[4]->handle = NULL;
    p_device_io[5]->handle = NULL;
    p_device_io[6]->handle = NULL;
    p_device_io[7]->handle = NULL;

    return true;
}
