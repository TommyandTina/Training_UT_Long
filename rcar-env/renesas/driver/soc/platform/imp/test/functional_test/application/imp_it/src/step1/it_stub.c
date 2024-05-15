#include <stdio.h>
#include <string.h>
#include "it_stub.h"

// common macro for stub function definition
#define STUB_VAR_DEFINE(type, func)   static volatile type g_ret_##func;
#define STUB_SET_FUNC_DEFINE(type, func)  \
    void it_stub_set_##func(type ret)  \
    {   \
        g_ret_##func = ret;\
    }


#define STUB_COMMON_DEFINE(type, func)    \
    STUB_VAR_DEFINE(type, func)       \
    STUB_SET_FUNC_DEFINE(type, func)

STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cmnctl_reg_set_mem_protect)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cmnctl_reg_get_hw_info)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cmnctl_reg_read32)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cmnctl_reg_write32)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cmnctl_reg_required)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cmnctl_reg_quit)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cmnctl_mutex_create)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cmnctl_mutex_lock)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impctl_check_state)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impsctl_check_state)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_ocvctl_check_state)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmactl_check_state)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmasctl_check_state)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_pscctl_check_state)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cnnctl_check_state)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dspctl_check_state)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cmnctl_init)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cmnctl_reg_init)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cmnctl_reg_mem_init)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impctl_init_start)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impsctl_init_start)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_ocvctl_init_start)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmactl_init_start)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmasctl_init_start)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_pscctl_init_start)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cnnctl_init_start)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dspctl_init_start)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impctl_init_end)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impsctl_init_end)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_ocvctl_init_end)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmactl_init_end)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmasctl_init_end)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_pscctl_init_end)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cnnctl_init_end)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dspctl_init_end)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impctl_attr_init)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impsctl_attr_init)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_ocvctl_attr_init)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmactl_attr_init)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmasctl_attr_init)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_pscctl_attr_init)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cnnctl_attr_init)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dspctl_attr_init)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cmnctl_mutex_unlock)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cmnctl_quit)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cmnctl_mutex_destroy)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impctl_start)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impsctl_start)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_ocvctl_start)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmactl_start)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmasctl_start)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_pscctl_start)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cnnctl_start)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dspctl_dsp_start)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impctl_stop)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impsctl_stop)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_ocvctl_stop)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmactl_stop)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmasctl_stop)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_pscctl_stop)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cnnctl_stop)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dspctl_stop)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impctl_set_mem_init)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impsctl_set_mem_init)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_ocvctl_set_mem_init)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmactl_set_mem_init)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmasctl_set_mem_init)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_pscctl_set_mem_init)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cnnctl_set_mem_init)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dspctl_set_mem_init)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impctl_set_core_map)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impsctl_set_core_map)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_ocvctl_set_core_map)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmactl_set_core_map)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmasctl_set_core_map)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_pscctl_set_core_map)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cnnctl_set_core_map)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dspctl_set_core_map)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impctl_set_cl)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impsctl_set_cl)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_ocvctl_set_cl)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmactl_set_cl)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmasctl_set_cl)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_pscctl_set_cl)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cnnctl_set_cl)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dspctl_set_dsp)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impctl_set_irq_mask)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impsctl_set_irq_mask)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_ocvctl_set_irq_mask)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmactl_set_irq_mask)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmasctl_set_irq_mask)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_pscctl_set_irq_mask)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cnnctl_set_irq_mask)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impctl_execute)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impsctl_execute)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_ocvctl_execute)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmactl_execute)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmasctl_execute)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_pscctl_execute)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cnnctl_execute)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dspctl_dsp_execute)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impctl_int_handler)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impsctl_int_handler)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_ocvctl_int_handler)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmactl_int_handler)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmasctl_int_handler)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_pscctl_int_handler)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cnnctl_int_handler)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dspctl_int_handler)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impctl_resume_exe)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impsctl_resume_exe)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_ocvctl_resume_exe)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmactl_resume_exe)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmasctl_resume_exe)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_pscctl_resume_exe)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cnnctl_resume_exe)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dspctl_resume_exe)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impctl_pm_set_policy)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impsctl_pm_set_policy)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_ocvctl_pm_set_policy)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmactl_pm_set_policy)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmasctl_pm_set_policy)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_pscctl_pm_set_policy)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cnnctl_pm_set_policy)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dspctl_pm_set_policy)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impctl_pm_get_policy)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impsctl_pm_get_policy)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_ocvctl_pm_get_policy)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmactl_pm_get_policy)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmasctl_pm_get_policy)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_pscctl_pm_get_policy)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cnnctl_pm_get_policy)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dspctl_pm_get_policy)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impctl_pm_get_state)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_ocvctl_pm_get_state)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmactl_pm_get_state)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmasctl_pm_get_state)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_pscctl_pm_get_state)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cnnctl_pm_get_state)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dspctl_pm_get_state)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impctl_quit)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_ocvctl_quit)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmactl_quit)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmasctl_quit)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_pscctl_quit)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cnnctl_quit)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dspctl_quit)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cmnctl_get_syncc_val)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cmnctl_set_irq_group)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cmnctl_judge_int)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cmnctl_fatal_cb)
STUB_COMMON_DEFINE(bool, impdrv_impctl_is_valid_core)
STUB_COMMON_DEFINE(bool, impdrv_impsctl_is_valid_core)
STUB_COMMON_DEFINE(bool, impdrv_ocvctl_is_valid_core)
STUB_COMMON_DEFINE(bool, impdrv_dmactl_is_valid_core)
STUB_COMMON_DEFINE(bool, impdrv_dmasctl_is_valid_core)
STUB_COMMON_DEFINE(bool, impdrv_pscctl_is_valid_core)
STUB_COMMON_DEFINE(bool, impdrv_cnnctl_is_valid_core)
STUB_COMMON_DEFINE(bool, impdrv_dspctl_is_valid_core)
STUB_COMMON_DEFINE(st_impdrv_corectl_func_t *, impdrv_cmnctl_get_corectl_func)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmactl_mb_init)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cmnctl_save_irq_mask)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cmnctl_restore_irq_mask)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cmnctl_force_irq_mask)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cmnctl_conf_reg_check)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cmnctl_reg_chk_mem_protect)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impctl_bus_if_check)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impsctl_bus_if_check)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmactl_bus_if_check)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impctl_conf_reg_check)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impsctl_conf_reg_check)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_ocvctl_conf_reg_check)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmactl_conf_reg_check)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmasctl_conf_reg_check)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_pscctl_conf_reg_check)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cnnctl_conf_reg_check)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dspctl_conf_reg_check)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cmnctl_get_init_param)
STUB_COMMON_DEFINE(p_impdrv_dspctl_dsp_start_t, impdrv_cmnctl_get_dsp_func)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impctl_set_cl_brk_addr)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_ocvctl_set_cl_brk_addr)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmactl_set_cl_brk_addr)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmasctl_set_cl_brk_addr)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_pscctl_set_cl_brk_addr)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cnnctl_set_cl_brk_addr)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_impctl_set_cond_gosub)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_ocvctl_set_cond_gosub)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmactl_set_cond_gosub)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dmasctl_set_cond_gosub)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_pscctl_set_cond_gosub)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cnnctl_set_cond_gosub)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_dspctl_dsp_start_pre)

volatile bool is_valid_core_flag;
static volatile e_impdrv_errorcode_t g_ret_impdrv_impctl_set_state[20];
static volatile e_impdrv_errorcode_t g_ret_impdrv_impsctl_set_state[20];
static volatile e_impdrv_errorcode_t g_ret_impdrv_ocvctl_set_state[20];
static volatile e_impdrv_errorcode_t g_ret_impdrv_dmactl_set_state[20];
static volatile e_impdrv_errorcode_t g_ret_impdrv_dmasctl_set_state[20];
static volatile e_impdrv_errorcode_t g_ret_impdrv_pscctl_set_state[20];
static volatile e_impdrv_errorcode_t g_ret_impdrv_cnnctl_set_state[20];
static volatile e_impdrv_errorcode_t g_ret_impdrv_dspctl_set_state[20];
static uint32_t g_count_impdrv_impctl_set_state;
static uint32_t g_count_impdrv_impsctl_set_state;
static uint32_t g_count_impdrv_ocvctl_set_state;
static uint32_t g_count_impdrv_dmactl_set_state;
static uint32_t g_count_impdrv_dmasctl_set_state;
static uint32_t g_count_impdrv_pscctl_set_state;
static uint32_t g_count_impdrv_cnnctl_set_state;
static uint32_t g_count_impdrv_dspctl_set_state;

void it_stub_set_impdrv_impctl_set_state(e_impdrv_errorcode_t ret, uint32_t count)
{
	if (RCVDRV_IMP_ALL == count)
	{
		memset(g_ret_impdrv_impctl_set_state, ret, sizeof(g_ret_impdrv_impctl_set_state));
	}
	else
	{
		g_ret_impdrv_impctl_set_state[count] = ret;
	}
}

void it_stub_set_impdrv_impsctl_set_state(e_impdrv_errorcode_t ret, uint32_t count)
{
	if (RCVDRV_IMP_ALL == count)
	{
		memset(g_ret_impdrv_impsctl_set_state, ret, sizeof(g_ret_impdrv_impsctl_set_state));
	}
	else
	{
		g_ret_impdrv_impsctl_set_state[count] = ret;
	}
}

void it_stub_set_impdrv_ocvctl_set_state(e_impdrv_errorcode_t ret, uint32_t count)
{
	if (RCVDRV_IMP_ALL == count)
	{
		memset(g_ret_impdrv_ocvctl_set_state, ret, sizeof(g_ret_impdrv_ocvctl_set_state));
	}
	else
	{
		g_ret_impdrv_ocvctl_set_state[count] = ret;
	}
}

void it_stub_set_impdrv_dmactl_set_state(e_impdrv_errorcode_t ret, uint32_t count)
{
	if (RCVDRV_IMP_ALL == count)
	{
		memset(g_ret_impdrv_dmactl_set_state, ret, sizeof(g_ret_impdrv_dmactl_set_state));
	}
	else
	{
		g_ret_impdrv_dmactl_set_state[count] = ret;
	}
}

void it_stub_set_impdrv_dmasctl_set_state(e_impdrv_errorcode_t ret, uint32_t count)
{
	if (RCVDRV_IMP_ALL == count)
	{
		memset(g_ret_impdrv_dmasctl_set_state, ret, sizeof(g_ret_impdrv_dmasctl_set_state));
	}
	else
	{
		g_ret_impdrv_dmasctl_set_state[count] = ret;
	}
}

void it_stub_set_impdrv_pscctl_set_state(e_impdrv_errorcode_t ret, uint32_t count)
{
	if (RCVDRV_IMP_ALL == count)
	{
		memset(g_ret_impdrv_pscctl_set_state, ret, sizeof(g_ret_impdrv_pscctl_set_state));
	}
	else
	{
		g_ret_impdrv_pscctl_set_state[count] = ret;
	}
}

void it_stub_set_impdrv_cnnctl_set_state(e_impdrv_errorcode_t ret, uint32_t count)
{
	if (RCVDRV_IMP_ALL == count)
	{
		memset(g_ret_impdrv_cnnctl_set_state, ret, sizeof(g_ret_impdrv_cnnctl_set_state));
	}
	else
	{
		g_ret_impdrv_cnnctl_set_state[count] = ret;
	}
}

void it_stub_set_impdrv_dspctl_set_state(e_impdrv_errorcode_t ret, uint32_t count)
{
	if (RCVDRV_IMP_ALL == count)
	{
		memset(g_ret_impdrv_dspctl_set_state, ret, sizeof(g_ret_impdrv_dspctl_set_state));
	}
	else
	{
		g_ret_impdrv_dspctl_set_state[count] = ret;
	}
}

/* Stub declaration */

/***********************************************************************************
 * Stub functions
 **********************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_reg_init(
    st_impdrv_commonctl_t           *const p_commonctl
)
{
	return g_ret_impdrv_cmnctl_reg_init;
}

e_impdrv_errorcode_t impdrv_cmnctl_reg_mem_init(
    st_impdrv_commonctl_t       *const p_commonctl
)
{
	return g_ret_impdrv_cmnctl_reg_mem_init;
}

e_impdrv_errorcode_t impdrv_cmnctl_reg_set_mem_protect(
    st_impdrv_commonctl_t           *const p_commonctl,
    const e_impdrv_protect_mode_t   protect_mode
)
{
	return g_ret_impdrv_cmnctl_reg_set_mem_protect;
}

e_impdrv_errorcode_t impdrv_cmnctl_reg_get_hw_info(
    const st_impdrv_commonctl_t     *const p_commonctl,
    const char                      *const p_device_id,
    st_impdrv_reg_info_t            *const p_reg_info
)
{
	return g_ret_impdrv_cmnctl_reg_get_hw_info;
}

e_impdrv_errorcode_t impdrv_cmnctl_reg_read32(
    const st_impdrv_commonctl_t     *const p_commonctl,
    const char                      *const p_device_id,
    const uintptr_t                 offset,
    uint32_t                        *const p_data
)
{
	return g_ret_impdrv_cmnctl_reg_read32;
}

e_impdrv_errorcode_t impdrv_cmnctl_reg_write32(
    const st_impdrv_commonctl_t     *const p_commonctl,
    const char                      *const p_device_id,
    const uintptr_t                 offset,
    const uint32_t                  data
)
{
	return g_ret_impdrv_cmnctl_reg_write32;
}

e_impdrv_errorcode_t impdrv_cmnctl_reg_required(
    const st_impdrv_commonctl_t     *const p_commonctl,
    const char                      *const p_device_id,
    const e_impdrv_reg_req_state_t  new_state
)
{
	return g_ret_impdrv_cmnctl_reg_required;
}

e_impdrv_errorcode_t impdrv_cmnctl_reg_quit(
    st_impdrv_commonctl_t           *const p_commonctl
)
{
	return g_ret_impdrv_cmnctl_reg_quit;
}

e_impdrv_errorcode_t impdrv_cmnctl_mutex_create(
    const st_impdrv_commonctl_t *const p_commonctl,
    const osal_mutex_id_t       mutex_id,
    const osal_milli_sec_t      time_period
)
{
	return g_ret_impdrv_cmnctl_mutex_create;
}

e_impdrv_errorcode_t impdrv_cmnctl_mutex_lock(
    st_impdrv_commonctl_t   *const p_commonctl
)
{
	return g_ret_impdrv_cmnctl_mutex_lock;
}

e_impdrv_errorcode_t impdrv_impctl_check_state(
    const st_impdrv_corectl_t   *const p_corectl,
    const uint32_t              core_num,
    const e_impdrv_state_t      state
)
{
	return g_ret_impdrv_impctl_check_state;
}

e_impdrv_errorcode_t impdrv_impsctl_check_state(
    const st_impdrv_corectl_t   *const p_corectl,
    const uint32_t              core_num,
    const e_impdrv_state_t      state
)
{
	return g_ret_impdrv_impsctl_check_state;
}

e_impdrv_errorcode_t impdrv_ocvctl_check_state(
    const st_impdrv_corectl_t   *const p_corectl,
    const uint32_t              core_num,
    const e_impdrv_state_t      state
)
{
	return g_ret_impdrv_ocvctl_check_state;
}

e_impdrv_errorcode_t impdrv_dmactl_check_state(
    const st_impdrv_corectl_t   *const p_corectl,
    const uint32_t              core_num,
    const e_impdrv_state_t      state
)
{
	return g_ret_impdrv_dmactl_check_state;
}

e_impdrv_errorcode_t impdrv_dmasctl_check_state(
    const st_impdrv_corectl_t   *const p_corectl,
    const uint32_t              core_num,
    const e_impdrv_state_t      state
)
{
	return g_ret_impdrv_dmasctl_check_state;
}

e_impdrv_errorcode_t impdrv_pscctl_check_state(
    const st_impdrv_corectl_t   *const p_corectl,
    const uint32_t              core_num,
    const e_impdrv_state_t      state
)
{
	return g_ret_impdrv_pscctl_check_state;
}

e_impdrv_errorcode_t impdrv_cnnctl_check_state(
    const st_impdrv_corectl_t   *const p_corectl,
    const uint32_t              core_num,
    const e_impdrv_state_t      state
)
{
	return g_ret_impdrv_cnnctl_check_state;
}

e_impdrv_errorcode_t impdrv_dspctl_check_state(
    const st_impdrv_corectl_t   *const p_corectl,
    const uint32_t              core_num,
    const e_impdrv_state_t      state
)
{
	return g_ret_impdrv_dspctl_check_state;
}

e_impdrv_errorcode_t impdrv_cmnctl_init(
    st_impdrv_commonctl_t           *const p_commonctl,
    const st_impdrv_cmn_init_data_t *const p_cmn_init_data,
    bool                            *const p_is_dmac_mb_init
)
{
	if ((IMPDRV_EC_OK == g_ret_impdrv_cmnctl_init) && (NULL != p_is_dmac_mb_init))
	{
		*p_is_dmac_mb_init = true;
	}

	return g_ret_impdrv_cmnctl_init;
}

e_impdrv_errorcode_t impdrv_impctl_init_start(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_impctl_init_start;
}

e_impdrv_errorcode_t impdrv_impsctl_init_start(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_impsctl_init_start;
}

e_impdrv_errorcode_t impdrv_ocvctl_init_start(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_ocvctl_init_start;
}

e_impdrv_errorcode_t impdrv_dmactl_init_start(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_dmactl_init_start;
}

e_impdrv_errorcode_t impdrv_dmasctl_init_start(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_dmasctl_init_start;
}

e_impdrv_errorcode_t impdrv_pscctl_init_start(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_pscctl_init_start;
}

e_impdrv_errorcode_t impdrv_cnnctl_init_start(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_cnnctl_init_start;
}

e_impdrv_errorcode_t impdrv_dspctl_init_start(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_dspctl_init_start;
}

e_impdrv_errorcode_t impdrv_impctl_init_end(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_impctl_init_end;
}

e_impdrv_errorcode_t impdrv_impsctl_init_end(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_impsctl_init_end;
}

e_impdrv_errorcode_t impdrv_ocvctl_init_end(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_ocvctl_init_end;
}

e_impdrv_errorcode_t impdrv_dmactl_init_end(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_dmactl_init_end;
}

e_impdrv_errorcode_t impdrv_dmasctl_init_end(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_dmasctl_init_end;
}

e_impdrv_errorcode_t impdrv_pscctl_init_end(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_pscctl_init_end;
}

e_impdrv_errorcode_t impdrv_cnnctl_init_end(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_cnnctl_init_end;
}

e_impdrv_errorcode_t impdrv_dspctl_init_end(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_dspctl_init_end;
}

e_impdrv_errorcode_t impdrv_impctl_set_state(
    st_impdrv_corectl_t         *const p_corectl,
    const e_impdrv_state_t      state
)
{
	e_impdrv_errorcode_t ret;

	p_corectl->core_state = state;

	ret = g_ret_impdrv_impctl_set_state[g_count_impdrv_impctl_set_state];
	g_count_impdrv_impctl_set_state++;
	if (g_count_impdrv_impctl_set_state >= ARRAY_SIZE(g_ret_impdrv_impctl_set_state))
	{
		g_count_impdrv_impctl_set_state = 0;
	}

	return ret;
}

e_impdrv_errorcode_t impdrv_impsctl_set_state(
    st_impdrv_corectl_t         *const p_corectl,
    const e_impdrv_state_t      state
)
{
	e_impdrv_errorcode_t ret;

	p_corectl->core_state = state;

	ret = g_ret_impdrv_impsctl_set_state[g_count_impdrv_impsctl_set_state];
	g_count_impdrv_impsctl_set_state++;
	if (g_count_impdrv_impsctl_set_state >= ARRAY_SIZE(g_ret_impdrv_impsctl_set_state))
	{
		g_count_impdrv_impsctl_set_state = 0;
	}

	return ret;
}

e_impdrv_errorcode_t impdrv_ocvctl_set_state(
    st_impdrv_corectl_t         *const p_corectl,
    const e_impdrv_state_t      state
)
{
	e_impdrv_errorcode_t ret;

	p_corectl->core_state = state;

	ret = g_ret_impdrv_ocvctl_set_state[g_count_impdrv_ocvctl_set_state];
	g_count_impdrv_ocvctl_set_state++;
	if (g_count_impdrv_ocvctl_set_state >= ARRAY_SIZE(g_ret_impdrv_ocvctl_set_state))
	{
		g_count_impdrv_ocvctl_set_state = 0;
	}

	return ret;
}

e_impdrv_errorcode_t impdrv_dmactl_set_state(
    st_impdrv_corectl_t         *const p_corectl,
    const e_impdrv_state_t      state
)
{
	e_impdrv_errorcode_t ret;

	p_corectl->core_state = state;

	ret = g_ret_impdrv_dmactl_set_state[g_count_impdrv_dmactl_set_state];
	g_count_impdrv_dmactl_set_state++;
	if (g_count_impdrv_dmactl_set_state >= ARRAY_SIZE(g_ret_impdrv_dmactl_set_state))
	{
		g_count_impdrv_dmactl_set_state = 0;
	}

	return ret;
}

e_impdrv_errorcode_t impdrv_dmasctl_set_state(
    st_impdrv_corectl_t         *const p_corectl,
    const e_impdrv_state_t      state
)
{
	e_impdrv_errorcode_t ret;

	p_corectl->core_state = state;

	ret = g_ret_impdrv_dmasctl_set_state[g_count_impdrv_dmasctl_set_state];
	g_count_impdrv_dmasctl_set_state++;
	if (g_count_impdrv_dmasctl_set_state >= ARRAY_SIZE(g_ret_impdrv_dmasctl_set_state))
	{
		g_count_impdrv_dmasctl_set_state = 0;
	}

	return ret;
}

e_impdrv_errorcode_t impdrv_pscctl_set_state(
    st_impdrv_corectl_t         *const p_corectl,
    const e_impdrv_state_t      state
)
{
	e_impdrv_errorcode_t ret;

	p_corectl->core_state = state;

	ret = g_ret_impdrv_pscctl_set_state[g_count_impdrv_pscctl_set_state];
	g_count_impdrv_pscctl_set_state++;
	if (g_count_impdrv_pscctl_set_state >= ARRAY_SIZE(g_ret_impdrv_pscctl_set_state))
	{
		g_count_impdrv_pscctl_set_state = 0;
	}

	return ret;
}

e_impdrv_errorcode_t impdrv_cnnctl_set_state(
    st_impdrv_corectl_t         *const p_corectl,
    const e_impdrv_state_t      state
)
{
	e_impdrv_errorcode_t ret;

	p_corectl->core_state = state;

	ret = g_ret_impdrv_cnnctl_set_state[g_count_impdrv_cnnctl_set_state];
	g_count_impdrv_cnnctl_set_state++;
	if (g_count_impdrv_cnnctl_set_state >= ARRAY_SIZE(g_ret_impdrv_cnnctl_set_state))
	{
		g_count_impdrv_cnnctl_set_state = 0;
	}

	return ret;
}

e_impdrv_errorcode_t impdrv_dspctl_set_state(
    st_impdrv_corectl_t         *const p_corectl,
    const e_impdrv_state_t      state
)
{
	e_impdrv_errorcode_t ret;

	p_corectl->core_state = state;

	ret = g_ret_impdrv_dspctl_set_state[g_count_impdrv_dspctl_set_state];
	g_count_impdrv_dspctl_set_state++;
	if (g_count_impdrv_dspctl_set_state >= ARRAY_SIZE(g_ret_impdrv_dspctl_set_state))
	{
		g_count_impdrv_dspctl_set_state = 0;
	}

	return ret;
}

e_impdrv_errorcode_t impdrv_impctl_attr_init(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_impctl_attr_init;
}

e_impdrv_errorcode_t impdrv_impsctl_attr_init(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_impsctl_attr_init;
}

e_impdrv_errorcode_t impdrv_ocvctl_attr_init(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_ocvctl_attr_init;
}

e_impdrv_errorcode_t impdrv_dmactl_attr_init(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_dmactl_attr_init;
}

e_impdrv_errorcode_t impdrv_dmasctl_attr_init(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_dmasctl_attr_init;
}

e_impdrv_errorcode_t impdrv_pscctl_attr_init(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_pscctl_attr_init;
}

e_impdrv_errorcode_t impdrv_cnnctl_attr_init(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_cnnctl_attr_init;
}

e_impdrv_errorcode_t impdrv_dspctl_attr_init(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_dspctl_attr_init;
}

bool impdrv_impctl_is_valid_core(
    const uint32_t              core_num
)
{
	if (is_valid_core_flag == true)
	{
		g_ret_impdrv_impctl_is_valid_core = false;

		if (core_num < IMP_CORE_MAX)
		{
			g_ret_impdrv_impctl_is_valid_core = true;
		}
	}

	return g_ret_impdrv_impctl_is_valid_core;
}

bool impdrv_impsctl_is_valid_core(
    const uint32_t              core_num
)
{
	if (is_valid_core_flag == true)
	{
		g_ret_impdrv_impsctl_is_valid_core = false;

#if !defined (V3M2) && !defined (V3U)
		if (core_num < IMP_SLIM_CORE_MAX)
		{
			g_ret_impdrv_impsctl_is_valid_core = true;
		}
#endif
	}

	return g_ret_impdrv_impsctl_is_valid_core;
}

bool impdrv_ocvctl_is_valid_core(
    const uint32_t              core_num
)
{
	if (is_valid_core_flag == true)
	{
		g_ret_impdrv_ocvctl_is_valid_core = false;

		if (core_num < OCV_CORE_MAX)
		{
			g_ret_impdrv_ocvctl_is_valid_core = true;
		}
	}

	return g_ret_impdrv_ocvctl_is_valid_core;
}

bool impdrv_dmactl_is_valid_core(
    const uint32_t              core_num
)
{
	if (is_valid_core_flag == true)
	{
		g_ret_impdrv_dmactl_is_valid_core = false;

		if (core_num < DMA_CORE_MAX)
		{
			g_ret_impdrv_dmactl_is_valid_core = true;
		}
	}

	return g_ret_impdrv_dmactl_is_valid_core;
}

bool impdrv_dmasctl_is_valid_core(
    const uint32_t              core_num
)
{
	if (is_valid_core_flag == true)
	{
		g_ret_impdrv_dmasctl_is_valid_core = false;

		if (core_num < DMA_SLIM_CORE_MAX)
		{
			g_ret_impdrv_dmasctl_is_valid_core = true;
		}
	}

	return g_ret_impdrv_dmasctl_is_valid_core;
}

bool impdrv_pscctl_is_valid_core(
    const uint32_t              core_num
)
{
	if (is_valid_core_flag == true)
	{
		g_ret_impdrv_pscctl_is_valid_core = false;

		if (core_num < PSCEXE_CORE_MAX)
		{
			g_ret_impdrv_pscctl_is_valid_core = true;
		}
	}

	return g_ret_impdrv_pscctl_is_valid_core;
}

bool impdrv_cnnctl_is_valid_core(
    const uint32_t              core_num
)
{
	if (is_valid_core_flag == true)
	{
		g_ret_impdrv_cnnctl_is_valid_core = false;

		if (core_num < CNN_CORE_MAX)
		{
			g_ret_impdrv_cnnctl_is_valid_core = true;
		}
	}

	return g_ret_impdrv_cnnctl_is_valid_core;
}

bool impdrv_dspctl_is_valid_core(
    const uint32_t              core_num
)
{
	if (is_valid_core_flag == true)
	{
		g_ret_impdrv_dspctl_is_valid_core = false;

		if (core_num < DSP_CORE_MAX)
		{
			g_ret_impdrv_dspctl_is_valid_core = true;
		}
	}

	return g_ret_impdrv_dspctl_is_valid_core;
}

e_impdrv_errorcode_t impdrv_impctl_start(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_impctl_start;
}

e_impdrv_errorcode_t impdrv_impsctl_start(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_impsctl_start;
}

e_impdrv_errorcode_t impdrv_ocvctl_start(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_ocvctl_start;
}

e_impdrv_errorcode_t impdrv_dmactl_start(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_dmactl_start;
}

e_impdrv_errorcode_t impdrv_dmasctl_start(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_dmasctl_start;
}

e_impdrv_errorcode_t impdrv_pscctl_start(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_pscctl_start;
}

e_impdrv_errorcode_t impdrv_cnnctl_start(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_cnnctl_start;
}

e_impdrv_errorcode_t impdrv_dspctl_dsp_start(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_dspctl_dsp_start;
}

e_impdrv_errorcode_t impdrv_impctl_stop(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_impctl_stop;
}

e_impdrv_errorcode_t impdrv_impsctl_stop(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_impsctl_stop;
}

e_impdrv_errorcode_t impdrv_ocvctl_stop(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_ocvctl_stop;
}

e_impdrv_errorcode_t impdrv_dmactl_stop(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_dmactl_stop;
}

e_impdrv_errorcode_t impdrv_dmasctl_stop(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_dmasctl_stop;
}

e_impdrv_errorcode_t impdrv_pscctl_stop(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_pscctl_stop;
}

e_impdrv_errorcode_t impdrv_cnnctl_stop(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_cnnctl_stop;
}

e_impdrv_errorcode_t impdrv_dspctl_stop(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_dspctl_stop;
}

e_impdrv_errorcode_t impdrv_impctl_set_mem_init(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const e_impdrv_param_t  enable
)
{
	return g_ret_impdrv_impctl_set_mem_init;
}

e_impdrv_errorcode_t impdrv_impsctl_set_mem_init(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const e_impdrv_param_t  enable
)
{
	return g_ret_impdrv_impsctl_set_mem_init;
}

e_impdrv_errorcode_t impdrv_ocvctl_set_mem_init(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const e_impdrv_param_t  enable
)
{
	return g_ret_impdrv_ocvctl_set_mem_init;
}

e_impdrv_errorcode_t impdrv_dmactl_set_mem_init(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const e_impdrv_param_t  enable
)
{
	return g_ret_impdrv_dmactl_set_mem_init;
}

e_impdrv_errorcode_t impdrv_dmasctl_set_mem_init(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const e_impdrv_param_t  enable
)
{
	return g_ret_impdrv_dmasctl_set_mem_init;
}

e_impdrv_errorcode_t impdrv_pscctl_set_mem_init(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const e_impdrv_param_t  enable
)
{
	return g_ret_impdrv_pscctl_set_mem_init;
}

e_impdrv_errorcode_t impdrv_cnnctl_set_mem_init(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const e_impdrv_param_t  enable
)
{
	return g_ret_impdrv_cnnctl_set_mem_init;
}

e_impdrv_errorcode_t impdrv_dspctl_set_mem_init(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const e_impdrv_param_t  enable
)
{
	return g_ret_impdrv_dspctl_set_mem_init;
}

e_impdrv_errorcode_t impdrv_impctl_set_core_map(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const uint8_t           syncc_val[IMPDRV_COREMAP_MAXID]
)
{
	return g_ret_impdrv_impctl_set_core_map;
}

e_impdrv_errorcode_t impdrv_impsctl_set_core_map(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const uint8_t           syncc_val[IMPDRV_COREMAP_MAXID]
)
{
	return g_ret_impdrv_impsctl_set_core_map;
}

e_impdrv_errorcode_t impdrv_ocvctl_set_core_map(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const uint8_t           syncc_val[IMPDRV_COREMAP_MAXID]
)
{
	return g_ret_impdrv_ocvctl_set_core_map;
}

e_impdrv_errorcode_t impdrv_dmactl_set_core_map(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const uint8_t           syncc_val[IMPDRV_COREMAP_MAXID]
)
{
	return g_ret_impdrv_dmactl_set_core_map;
}

e_impdrv_errorcode_t impdrv_dmasctl_set_core_map(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const uint8_t           syncc_val[IMPDRV_COREMAP_MAXID]
)
{
	return g_ret_impdrv_dmasctl_set_core_map;
}

e_impdrv_errorcode_t impdrv_pscctl_set_core_map(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const uint8_t           syncc_val[IMPDRV_COREMAP_MAXID]
)
{
	return g_ret_impdrv_pscctl_set_core_map;
}

e_impdrv_errorcode_t impdrv_cnnctl_set_core_map(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const uint8_t           syncc_val[IMPDRV_COREMAP_MAXID]
)
{
	return g_ret_impdrv_cnnctl_set_core_map;
}

e_impdrv_errorcode_t impdrv_dspctl_set_core_map(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const uint8_t           syncc_val[IMPDRV_COREMAP_MAXID]
)
{
	return g_ret_impdrv_dspctl_set_core_map;
}

e_impdrv_errorcode_t impdrv_impctl_set_cl(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const uint32_t          claddr_phys
)
{
	return g_ret_impdrv_impctl_set_cl;
}

e_impdrv_errorcode_t impdrv_impsctl_set_cl(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const uint32_t          claddr_phys
)
{
	return g_ret_impdrv_impsctl_set_cl;
}

e_impdrv_errorcode_t impdrv_ocvctl_set_cl(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const uint32_t          claddr_phys
)
{
	return g_ret_impdrv_ocvctl_set_cl;
}

e_impdrv_errorcode_t impdrv_dmactl_set_cl(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const uint32_t          claddr_phys
)
{
	return g_ret_impdrv_dmactl_set_cl;
}

e_impdrv_errorcode_t impdrv_dmasctl_set_cl(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const uint32_t          claddr_phys
)
{
	return g_ret_impdrv_dmasctl_set_cl;
}

e_impdrv_errorcode_t impdrv_pscctl_set_cl(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const uint32_t          claddr_phys
)
{
	return g_ret_impdrv_pscctl_set_cl;
}

e_impdrv_errorcode_t impdrv_cnnctl_set_cl(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const uint32_t          claddr_phys
)
{
	return g_ret_impdrv_cnnctl_set_cl;
}

e_impdrv_errorcode_t impdrv_dspctl_set_dsp(
    st_impdrv_corectl_t         *p_corectl,
    const uint32_t              core_num,
    const st_impdrv_dsp_data_t  *const p_dsp_app,
    const st_impdrv_dsp_data_t  *const p_dsp_fw,
    const st_impdrv_dsp_data_t  *const p_dsp_data,
    const st_impdrv_dsp_data_t  *const p_dsp_dtcm
)
{
	return g_ret_impdrv_dspctl_set_dsp;
}

e_impdrv_errorcode_t impdrv_impctl_set_irq_mask(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const bool              irq_mask[IMPDRV_IRQMASK_MAX]
)
{
	return g_ret_impdrv_impctl_set_irq_mask;
}

e_impdrv_errorcode_t impdrv_impsctl_set_irq_mask(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const bool              irq_mask[IMPDRV_IRQMASK_MAX]
)
{
	return g_ret_impdrv_impsctl_set_irq_mask;
}

e_impdrv_errorcode_t impdrv_ocvctl_set_irq_mask(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const bool              irq_mask[IMPDRV_IRQMASK_MAX]
)
{
	return g_ret_impdrv_ocvctl_set_irq_mask;
}

e_impdrv_errorcode_t impdrv_dmactl_set_irq_mask(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const bool              irq_mask[IMPDRV_IRQMASK_MAX]
)
{
	return g_ret_impdrv_dmactl_set_irq_mask;
}

e_impdrv_errorcode_t impdrv_dmasctl_set_irq_mask(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const bool              irq_mask[IMPDRV_IRQMASK_MAX]
)
{
	return g_ret_impdrv_dmasctl_set_irq_mask;
}

e_impdrv_errorcode_t impdrv_pscctl_set_irq_mask(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const bool              irq_mask[IMPDRV_IRQMASK_MAX]
)
{
	return g_ret_impdrv_pscctl_set_irq_mask;
}

e_impdrv_errorcode_t impdrv_cnnctl_set_irq_mask(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const bool              irq_mask[IMPDRV_IRQMASK_MAX]
)
{
	return g_ret_impdrv_cnnctl_set_irq_mask;
}

e_impdrv_errorcode_t impdrv_impctl_execute(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num,
    const p_impdrv_cbfunc_t     callback_func,
    void                        *const p_callback_args
)
{
	st_impdrv_core_info_t   core_info;

	if (IMPDRV_EC_OK == g_ret_impdrv_impctl_execute)
	{
		p_corectl->core_state = IMPDRV_STATE_EXEC;

		if (NULL != callback_func)
		{
			core_info.core_type = IMPDRV_CORE_TYPE_IMP;
			core_info.core_num = core_num;
			(void)callback_func(&core_info, IMPDRV_CB_RET_BEFORE_EXEC, IMPDRV_CALLBACK_CODE_DEF, p_callback_args);
		}
	}

	return g_ret_impdrv_impctl_execute;
}

e_impdrv_errorcode_t impdrv_impsctl_execute(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num,
    const p_impdrv_cbfunc_t     callback_func,
    void                        *const p_callback_args
)
{
	st_impdrv_core_info_t   core_info;

	if (IMPDRV_EC_OK == g_ret_impdrv_impsctl_execute)
	{
		p_corectl->core_state = IMPDRV_STATE_EXEC;

		if (NULL != callback_func)
		{
			core_info.core_type = IMPDRV_CORE_TYPE_IMP_SLIM;
			core_info.core_num = core_num;
			(void)callback_func(&core_info, IMPDRV_CB_RET_BEFORE_EXEC, IMPDRV_CALLBACK_CODE_DEF, p_callback_args);
		}
	}

	return g_ret_impdrv_impsctl_execute;
}

e_impdrv_errorcode_t impdrv_ocvctl_execute(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num,
    const p_impdrv_cbfunc_t     callback_func,
    void                        *const p_callback_args
)
{
	st_impdrv_core_info_t   core_info;

	if (IMPDRV_EC_OK == g_ret_impdrv_ocvctl_execute)
	{
		p_corectl->core_state = IMPDRV_STATE_EXEC;

		if (NULL != callback_func)
		{
			core_info.core_type = IMPDRV_CORE_TYPE_OCV;
			core_info.core_num  = core_num;
			(void)callback_func(&core_info, IMPDRV_CB_RET_BEFORE_EXEC, IMPDRV_CALLBACK_CODE_DEF, p_callback_args);
		}
	}

	return g_ret_impdrv_ocvctl_execute;
}

e_impdrv_errorcode_t impdrv_dmactl_execute(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num,
    const p_impdrv_cbfunc_t     callback_func,
    void                        *const p_callback_args
)
{
	st_impdrv_core_info_t   core_info;

	if (IMPDRV_EC_OK == g_ret_impdrv_dmactl_execute)
	{
		p_corectl->core_state = IMPDRV_STATE_EXEC;

		if (NULL != callback_func)
		{
			core_info.core_type = IMPDRV_CORE_TYPE_DMAC;
			core_info.core_num  = core_num;
			(void)callback_func(&core_info, IMPDRV_CB_RET_BEFORE_EXEC, IMPDRV_CALLBACK_CODE_DEF, p_callback_args);
		}
	}

	return g_ret_impdrv_dmactl_execute;
}

e_impdrv_errorcode_t impdrv_dmasctl_execute(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num,
    const p_impdrv_cbfunc_t     callback_func,
    void                        *const p_callback_args
)
{
	st_impdrv_core_info_t   core_info;

	if (IMPDRV_EC_OK == g_ret_impdrv_dmasctl_execute)
	{
		p_corectl->core_state = IMPDRV_STATE_EXEC;

		if (NULL != callback_func)
		{
			core_info.core_type = IMPDRV_CORE_TYPE_DMAC_SLIM;
			core_info.core_num  = core_num;
			(void)callback_func(&core_info, IMPDRV_CB_RET_BEFORE_EXEC, IMPDRV_CALLBACK_CODE_DEF, p_callback_args);
		}
	}

	return g_ret_impdrv_dmasctl_execute;
}

e_impdrv_errorcode_t impdrv_pscctl_execute(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num,
    const p_impdrv_cbfunc_t     callback_func,
    void                        *const p_callback_args
)
{
	st_impdrv_core_info_t   core_info;

	if (IMPDRV_EC_OK == g_ret_impdrv_pscctl_execute)
	{
		p_corectl->core_state = IMPDRV_STATE_EXEC;

		if (NULL != callback_func)
		{
			core_info.core_type = IMPDRV_CORE_TYPE_PSCEXE;
			core_info.core_num  = core_num;
			(void)callback_func(&core_info, IMPDRV_CB_RET_BEFORE_EXEC, IMPDRV_CALLBACK_CODE_DEF, p_callback_args);
		}
	}

	return g_ret_impdrv_pscctl_execute;
}

e_impdrv_errorcode_t impdrv_cnnctl_execute(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num,
    const p_impdrv_cbfunc_t     callback_func,
    void                        *const p_callback_args
)
{
	st_impdrv_core_info_t   core_info;

	if (IMPDRV_EC_OK == g_ret_impdrv_cnnctl_execute)
	{
		p_corectl->core_state = IMPDRV_STATE_EXEC;

		if (NULL != callback_func)
		{
			core_info.core_type = IMPDRV_CORE_TYPE_CNN;
			core_info.core_num  = core_num;
			(void)callback_func(&core_info, IMPDRV_CB_RET_BEFORE_EXEC, IMPDRV_CALLBACK_CODE_DEF, p_callback_args);
		}
	}

	return g_ret_impdrv_cnnctl_execute;
}

e_impdrv_errorcode_t impdrv_dspctl_dsp_execute(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num,
    const p_impdrv_cbfunc_t     callback_func,
    void                        *const p_callback_args
)
{
	st_impdrv_core_info_t   core_info;

	if (IMPDRV_EC_OK == g_ret_impdrv_dspctl_dsp_execute)
	{
		p_corectl->core_state = IMPDRV_STATE_EXEC;

		if (NULL != callback_func)
		{
			core_info.core_type = IMPDRV_CORE_TYPE_DSP;
			core_info.core_num  = core_num;
			(void)callback_func(&core_info, IMPDRV_CB_RET_BEFORE_EXEC, IMPDRV_CALLBACK_CODE_DEF, p_callback_args);
		}
	}

	return g_ret_impdrv_dspctl_dsp_execute;
}

e_impdrv_errorcode_t impdrv_impctl_int_handler(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
)
{
	st_impdrv_core_info_t   core_info;
	e_impdrv_cb_ret_t       cb_ercd;
	e_impdrv_state_t        state = IMPDRV_STATE_EXEC;

	if (p_corectl->core_state != IMPDRV_STATE_EXEC)
	{
		g_ret_impdrv_impctl_int_handler = IMPDRV_EC_NG_SEQSTATE;
	}

#if 0
	if (IMPDRV_EC_OK == g_ret_impdrv_impctl_int_handler)
	{
		if (true == int_cl_flag)
		{
			cb_ercd = IMPDRV_CB_RET_INT;
			if (IMPDRV_STATE_EXEC == state)
			{
				state = IMPDRV_STATE_INT;
			}
		}
		else
		{
			cb_ercd = IMPDRV_CB_RET_OK;
			if (IMPDRV_STATE_EXEC == state)
			{
				state = IMPDRV_STATE_READY;
			}
		}

		p_corectl->core_state = state;

		if (NULL != p_corectl->cbinfo.cbfunc)
		{
			core_info.core_type = IMPDRV_CORE_TYPE_IMP;
			core_info.core_num  = core_num;
			(void)(p_corectl->cbinfo.cbfunc)(&core_info, cb_ercd, IMPDRV_CALLBACK_CODE_DEF, p_corectl->cbinfo.p_cbarg);
		}
	}
#endif

	return g_ret_impdrv_impctl_int_handler;
}

e_impdrv_errorcode_t impdrv_impsctl_int_handler(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_impsctl_int_handler;
}

e_impdrv_errorcode_t impdrv_ocvctl_int_handler(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_ocvctl_int_handler;
}

e_impdrv_errorcode_t impdrv_dmactl_int_handler(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_dmactl_int_handler;
}

e_impdrv_errorcode_t impdrv_dmasctl_int_handler(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_dmasctl_int_handler;
}

e_impdrv_errorcode_t impdrv_pscctl_int_handler(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_pscctl_int_handler;
}

e_impdrv_errorcode_t impdrv_cnnctl_int_handler(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_cnnctl_int_handler;
}

e_impdrv_errorcode_t impdrv_dspctl_int_handler(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
)
{
	return g_ret_impdrv_dspctl_int_handler;
}

e_impdrv_errorcode_t impdrv_impctl_resume_exe(
    st_impdrv_corectl_t *const p_corectl,
    const uint32_t      core_num
)
{
	return g_ret_impdrv_impctl_resume_exe;
}

e_impdrv_errorcode_t impdrv_impsctl_resume_exe(
    st_impdrv_corectl_t *const p_corectl,
    const uint32_t      core_num
)
{
	return g_ret_impdrv_impsctl_resume_exe;
}

e_impdrv_errorcode_t impdrv_ocvctl_resume_exe(
    st_impdrv_corectl_t *const p_corectl,
    const uint32_t      core_num
)
{
	return g_ret_impdrv_ocvctl_resume_exe;
}

e_impdrv_errorcode_t impdrv_dmactl_resume_exe(
    st_impdrv_corectl_t *const p_corectl,
    const uint32_t      core_num
)
{
	return g_ret_impdrv_dmactl_resume_exe;
}

e_impdrv_errorcode_t impdrv_dmasctl_resume_exe(
    st_impdrv_corectl_t *const p_corectl,
    const uint32_t      core_num
)
{
	return g_ret_impdrv_dmasctl_resume_exe;
}

e_impdrv_errorcode_t impdrv_pscctl_resume_exe(
    st_impdrv_corectl_t *const p_corectl,
    const uint32_t      core_num
)
{
	return g_ret_impdrv_pscctl_resume_exe;
}

e_impdrv_errorcode_t impdrv_cnnctl_resume_exe(
    st_impdrv_corectl_t *const p_corectl,
    const uint32_t      core_num
)
{
	return g_ret_impdrv_cnnctl_resume_exe;
}

e_impdrv_errorcode_t impdrv_dspctl_resume_exe(
    st_impdrv_corectl_t *const p_corectl,
    const uint32_t      core_num
)
{
	return g_ret_impdrv_dspctl_resume_exe;
}

e_impdrv_errorcode_t impdrv_impctl_pm_set_policy(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    const e_impdrv_pm_policy_t      policy
)
{
	return g_ret_impdrv_impctl_pm_set_policy;
}

e_impdrv_errorcode_t impdrv_impsctl_pm_set_policy(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    const e_impdrv_pm_policy_t      policy
)
{
	return g_ret_impdrv_impsctl_pm_set_policy;
}

e_impdrv_errorcode_t impdrv_ocvctl_pm_set_policy(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    const e_impdrv_pm_policy_t      policy
)
{
	return g_ret_impdrv_ocvctl_pm_set_policy;
}

e_impdrv_errorcode_t impdrv_dmactl_pm_set_policy(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    const e_impdrv_pm_policy_t      policy
)
{
	return g_ret_impdrv_dmactl_pm_set_policy;
}

e_impdrv_errorcode_t impdrv_dmasctl_pm_set_policy(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    const e_impdrv_pm_policy_t      policy
)
{
	return g_ret_impdrv_dmasctl_pm_set_policy;
}

e_impdrv_errorcode_t impdrv_pscctl_pm_set_policy(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    const e_impdrv_pm_policy_t      policy
)
{
	return g_ret_impdrv_pscctl_pm_set_policy;
}

e_impdrv_errorcode_t impdrv_cnnctl_pm_set_policy(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    const e_impdrv_pm_policy_t      policy
)
{
	return g_ret_impdrv_cnnctl_pm_set_policy;
}

e_impdrv_errorcode_t impdrv_dspctl_pm_set_policy(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    const e_impdrv_pm_policy_t      policy
)
{
	return g_ret_impdrv_dspctl_pm_set_policy;
}

e_impdrv_errorcode_t impdrv_impctl_pm_get_policy(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    e_impdrv_pm_policy_t            *const p_policy
)
{
	return g_ret_impdrv_impctl_pm_get_policy;
}

e_impdrv_errorcode_t impdrv_impsctl_pm_get_policy(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    e_impdrv_pm_policy_t            *const p_policy
)
{
	return g_ret_impdrv_impsctl_pm_get_policy;
}

e_impdrv_errorcode_t impdrv_ocvctl_pm_get_policy(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    e_impdrv_pm_policy_t            *const p_policy
)
{
	return g_ret_impdrv_ocvctl_pm_get_policy;
}

e_impdrv_errorcode_t impdrv_dmactl_pm_get_policy(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    e_impdrv_pm_policy_t            *const p_policy
)
{
	return g_ret_impdrv_dmactl_pm_get_policy;
}

e_impdrv_errorcode_t impdrv_dmasctl_pm_get_policy(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    e_impdrv_pm_policy_t            *const p_policy
)
{
	return g_ret_impdrv_dmasctl_pm_get_policy;
}

e_impdrv_errorcode_t impdrv_pscctl_pm_get_policy(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    e_impdrv_pm_policy_t            *const p_policy
)
{
	return g_ret_impdrv_pscctl_pm_get_policy;
}

e_impdrv_errorcode_t impdrv_cnnctl_pm_get_policy(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    e_impdrv_pm_policy_t            *const p_policy
)
{
	return g_ret_impdrv_cnnctl_pm_get_policy;
}

e_impdrv_errorcode_t impdrv_dspctl_pm_get_policy(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    e_impdrv_pm_policy_t            *const p_policy
)
{
	return g_ret_impdrv_dspctl_pm_get_policy;
}

e_impdrv_errorcode_t impdrv_impctl_pm_get_state(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    e_impdrv_pm_state_t             *const p_pmstate
)
{
	return g_ret_impdrv_impctl_pm_get_state;
}

e_impdrv_errorcode_t impdrv_ocvctl_pm_get_state(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    e_impdrv_pm_state_t             *const p_pmstate
)
{
	return g_ret_impdrv_ocvctl_pm_get_state;
}

e_impdrv_errorcode_t impdrv_dmactl_pm_get_state(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    e_impdrv_pm_state_t             *const p_pmstate
)
{
	return g_ret_impdrv_dmactl_pm_get_state;
}

e_impdrv_errorcode_t impdrv_dmasctl_pm_get_state(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    e_impdrv_pm_state_t             *const p_pmstate
)
{
	return g_ret_impdrv_dmasctl_pm_get_state;
}

e_impdrv_errorcode_t impdrv_pscctl_pm_get_state(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    e_impdrv_pm_state_t             *const p_pmstate
)
{
	return g_ret_impdrv_pscctl_pm_get_state;
}

e_impdrv_errorcode_t impdrv_cnnctl_pm_get_state(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    e_impdrv_pm_state_t             *const p_pmstate
)
{
	return g_ret_impdrv_cnnctl_pm_get_state;
}

e_impdrv_errorcode_t impdrv_dspctl_pm_get_state(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    e_impdrv_pm_state_t             *const p_pmstate
)
{
	return g_ret_impdrv_dspctl_pm_get_state;
}

e_impdrv_errorcode_t impdrv_impctl_quit(
    const uint32_t              core_num
)
{
	return g_ret_impdrv_impctl_quit;
}

e_impdrv_errorcode_t impdrv_ocvctl_quit(
    const uint32_t              core_num
)
{
	return g_ret_impdrv_ocvctl_quit;
}

e_impdrv_errorcode_t impdrv_dmactl_quit(
    const uint32_t              core_num
)
{
	return g_ret_impdrv_dmactl_quit;
}

e_impdrv_errorcode_t impdrv_dmasctl_quit(
    const uint32_t              core_num
)
{
	return g_ret_impdrv_dmasctl_quit;
}

e_impdrv_errorcode_t impdrv_pscctl_quit(
    const uint32_t              core_num
)
{
	return g_ret_impdrv_pscctl_quit;
}

e_impdrv_errorcode_t impdrv_cnnctl_quit(
    const uint32_t              core_num
)
{
	return g_ret_impdrv_cnnctl_quit;
}

e_impdrv_errorcode_t impdrv_dspctl_quit(
    const uint32_t              core_num
)
{
	return g_ret_impdrv_dspctl_quit;
}

e_impdrv_errorcode_t impdrv_cmnctl_get_syncc_val(
    uint8_t                     syncc_val[IMPDRV_COREMAP_MAXID],
    const st_impdrv_commonctl_t *const p_commonctl,
    const st_impdrv_core_info_t core_map[IMPDRV_COREMAP_MAXID]
)
{
    return g_ret_impdrv_cmnctl_get_syncc_val;
}

e_impdrv_errorcode_t impdrv_cmnctl_quit(
    st_impdrv_commonctl_t   *const p_commonctl
)
{
	return g_ret_impdrv_cmnctl_quit;
}

e_impdrv_errorcode_t impdrv_cmnctl_mutex_unlock(
    st_impdrv_commonctl_t   *const p_commonctl
)
{
	return g_ret_impdrv_cmnctl_mutex_unlock;
}

e_impdrv_errorcode_t impdrv_cmnctl_mutex_destroy(
    const st_impdrv_commonctl_t   *const p_commonctl
)
{
	return g_ret_impdrv_cmnctl_mutex_destroy;
}

e_impdrv_errorcode_t impdrv_cmnctl_set_irq_group(
    const st_impdrv_commonctl_t *const p_commonctl,
    const st_impdrv_irq_group_t *const p_irq_param
)
{
	return g_ret_impdrv_cmnctl_set_irq_group;
}

e_impdrv_errorcode_t impdrv_cmnctl_judge_int(
    const st_impdrv_commonctl_t *const p_commonctl,
    const uint64_t              irq_channel,
    st_impdrv_core_info_t       core_info[IMPDRV_INTSTS_BITNUM],
    uint32_t                    *const p_use_core_num,
    bool                        is_fb_callback
)
{
    return g_ret_impdrv_cmnctl_judge_int;
}

e_impdrv_errorcode_t impdrv_cmnctl_fatal_cb(
    const st_impdrv_commonctl_t     *const p_commonctl,
    const st_impdrv_core_info_t     *const p_core_info,
    const e_impdrv_fatalcode_t      fatal_code,
    const e_impdrv_errorcode_t      error_code
)
{
    return g_ret_impdrv_cmnctl_fatal_cb;
}

e_impdrv_errorcode_t impdrv_dmactl_mb_init(
    st_impdrv_corectl_t   *const p_corectl,
    const uint32_t        core_num
)
{
	return g_ret_impdrv_dmactl_mb_init;
}

e_impdrv_errorcode_t impdrv_impctl_bus_if_check(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    const st_impdrv_chk_resource_t  *const p_chk_resource
)
{
	return g_ret_impdrv_impctl_bus_if_check;
}

e_impdrv_errorcode_t impdrv_impsctl_bus_if_check(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    const st_impdrv_chk_resource_t  *const p_chk_resource
)
{
	return g_ret_impdrv_impsctl_bus_if_check;
}

e_impdrv_errorcode_t impdrv_dmactl_bus_if_check(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    const st_impdrv_chk_resource_t  *const p_chk_resource
)
{
	return g_ret_impdrv_dmactl_bus_if_check;
}

e_impdrv_errorcode_t impdrv_impctl_conf_reg_check(
    const st_impdrv_coretypectl_t   *const p_coretypectl,
    const uint32_t                  core_num,
    const st_impdrv_chk_param_t     *const p_chk_param,
    const uint32_t                  param_num
)
{
	return g_ret_impdrv_impctl_conf_reg_check;
}

e_impdrv_errorcode_t impdrv_impsctl_conf_reg_check(
    const st_impdrv_coretypectl_t   *const p_coretypectl,
    const uint32_t                  core_num,
    const st_impdrv_chk_param_t     *const p_chk_param,
    const uint32_t                  param_num
)
{
	return g_ret_impdrv_impsctl_conf_reg_check;
}

e_impdrv_errorcode_t impdrv_ocvctl_conf_reg_check(
    const st_impdrv_coretypectl_t   *const p_coretypectl,
    const uint32_t                  core_num,
    const st_impdrv_chk_param_t     *const p_chk_param,
    const uint32_t                  param_num
)
{
	return g_ret_impdrv_ocvctl_conf_reg_check;
}

e_impdrv_errorcode_t impdrv_dmactl_conf_reg_check(
    const st_impdrv_coretypectl_t   *const p_coretypectl,
    const uint32_t                  core_num,
    const st_impdrv_chk_param_t     *const p_chk_param,
    const uint32_t                  param_num
)
{
	return g_ret_impdrv_dmactl_conf_reg_check;
}

e_impdrv_errorcode_t impdrv_dmasctl_conf_reg_check(
    const st_impdrv_coretypectl_t   *const p_coretypectl,
    const uint32_t                  core_num,
    const st_impdrv_chk_param_t     *const p_chk_param,
    const uint32_t                  param_num
)
{
	return g_ret_impdrv_dmasctl_conf_reg_check;
}

e_impdrv_errorcode_t impdrv_pscctl_conf_reg_check(
    const st_impdrv_coretypectl_t   *const p_coretypectl,
    const uint32_t                  core_num,
    const st_impdrv_chk_param_t     *const p_chk_param,
    const uint32_t                  param_num
)
{
	return g_ret_impdrv_pscctl_conf_reg_check;
}

e_impdrv_errorcode_t impdrv_cnnctl_conf_reg_check(
    const st_impdrv_coretypectl_t   *const p_coretypectl,
    const uint32_t                  core_num,
    const st_impdrv_chk_param_t     *const p_chk_param,
    const uint32_t                  param_num
)
{
	return g_ret_impdrv_cnnctl_conf_reg_check;
}

e_impdrv_errorcode_t impdrv_dspctl_conf_reg_check(
    const st_impdrv_coretypectl_t   *const p_coretypectl,
    const uint32_t                  core_num,
    const st_impdrv_chk_param_t     *const p_chk_param,
    const uint32_t                  param_num
)
{
	return g_ret_impdrv_dspctl_conf_reg_check;
}

e_impdrv_errorcode_t impdrv_impctl_set_cl_brk_addr(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const uint32_t          cl_brk_addr
)
{
	return g_ret_impdrv_impctl_set_cl_brk_addr;
}

e_impdrv_errorcode_t impdrv_ocvctl_set_cl_brk_addr(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const uint32_t          cl_brk_addr
)
{
	return g_ret_impdrv_ocvctl_set_cl_brk_addr;
}

e_impdrv_errorcode_t impdrv_dmactl_set_cl_brk_addr(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const uint32_t          cl_brk_addr
)
{
	return g_ret_impdrv_dmactl_set_cl_brk_addr;
}

e_impdrv_errorcode_t impdrv_dmasctl_set_cl_brk_addr(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const uint32_t          cl_brk_addr
)
{
	return g_ret_impdrv_dmasctl_set_cl_brk_addr;
}

e_impdrv_errorcode_t impdrv_pscctl_set_cl_brk_addr(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const uint32_t          cl_brk_addr
)
{
	return g_ret_impdrv_pscctl_set_cl_brk_addr;
}

e_impdrv_errorcode_t impdrv_cnnctl_set_cl_brk_addr(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const uint32_t          cl_brk_addr
)
{
	return g_ret_impdrv_cnnctl_set_cl_brk_addr;
}

e_impdrv_errorcode_t impdrv_impctl_set_cond_gosub(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num,
    const e_impdrv_gosub_cond_t condition
)
{
	return g_ret_impdrv_impctl_set_cond_gosub;
}

e_impdrv_errorcode_t impdrv_ocvctl_set_cond_gosub(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num,
    const e_impdrv_gosub_cond_t condition
)
{
	return g_ret_impdrv_ocvctl_set_cond_gosub;
}

e_impdrv_errorcode_t impdrv_dmactl_set_cond_gosub(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num,
    const e_impdrv_gosub_cond_t condition
)
{
	return g_ret_impdrv_dmactl_set_cond_gosub;
}

e_impdrv_errorcode_t impdrv_dmasctl_set_cond_gosub(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num,
    const e_impdrv_gosub_cond_t condition
)
{
	return g_ret_impdrv_dmasctl_set_cond_gosub;
}

e_impdrv_errorcode_t impdrv_pscctl_set_cond_gosub(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num,
    const e_impdrv_gosub_cond_t condition
)
{
	return g_ret_impdrv_pscctl_set_cond_gosub;
}

e_impdrv_errorcode_t impdrv_cnnctl_set_cond_gosub(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num,
    const e_impdrv_gosub_cond_t condition
)
{
	return g_ret_impdrv_cnnctl_set_cond_gosub;
}

e_impdrv_errorcode_t impdrv_dspctl_dsp_start_pre(
	st_impdrv_corectl_t                 *p_corectl,
    const uint32_t                      core_num,
    const e_osal_interrupt_priority_t   irq_priority,
    const impdrv_ctrl_handle_t          osal_cb_args
)
{
	return g_ret_impdrv_dspctl_dsp_start_pre;
}

e_impdrv_errorcode_t impdrv_cmnctl_get_init_param(
    const st_impdrv_commonctl_t *const p_commonctl,
    e_osal_interrupt_priority_t *const p_irq_priority,
    impdrv_ctrl_handle_t        *const p_osal_cb_args
)
{
	return g_ret_impdrv_cmnctl_get_init_param;
}

p_impdrv_dspctl_dsp_start_t impdrv_cmnctl_get_dsp_func(
    void
)
{
	if (NULL != g_ret_impdrv_cmnctl_get_dsp_func)
	{
		g_ret_impdrv_cmnctl_get_dsp_func = (volatile p_impdrv_dspctl_dsp_start_t)&impdrv_dspctl_dsp_start_pre;
	}

	return g_ret_impdrv_cmnctl_get_dsp_func;
}

#if defined (V3H1)
static const st_impdrv_corectl_func_t s_corectl_func_tbl[IMPDRV_CORE_TYPE_MAX] =
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
	{   impdrv_impctl_is_valid_core,    impdrv_impctl_check_state,      impdrv_impctl_set_state,
		impdrv_impctl_init_start,       impdrv_impctl_init_end,         impdrv_impctl_start,
		impdrv_impctl_stop,             impdrv_impctl_attr_init,        impdrv_impctl_set_mem_init,
		impdrv_impctl_set_core_map,     impdrv_impctl_set_cl,           impdrv_impctl_set_irq_mask,
		impdrv_impctl_execute,          impdrv_impctl_int_handler,      impdrv_impctl_resume_exe,
		impdrv_impctl_pm_set_policy,    impdrv_impctl_pm_get_policy,    NULL,
		impdrv_impctl_bus_if_check,     impdrv_impctl_conf_reg_check
	},
	/* IMPDRV_CORE_TYPE_IMP_SLIM */
	{   impdrv_impsctl_is_valid_core,   impdrv_impsctl_check_state,     impdrv_impsctl_set_state,
		impdrv_impsctl_init_start,      impdrv_impsctl_init_end,        impdrv_impsctl_start,
		impdrv_impsctl_stop,            impdrv_impsctl_attr_init,       impdrv_impsctl_set_mem_init,
		impdrv_impsctl_set_core_map,    impdrv_impsctl_set_cl,          impdrv_impsctl_set_irq_mask,
		impdrv_impsctl_execute,         impdrv_impsctl_int_handler,     impdrv_impsctl_resume_exe,
		impdrv_impsctl_pm_set_policy,   impdrv_impsctl_pm_get_policy,   NULL,
		impdrv_impsctl_bus_if_check,    impdrv_impsctl_conf_reg_check
	},
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
	{   impdrv_dmactl_is_valid_core,    impdrv_dmactl_check_state,      impdrv_dmactl_set_state,
		impdrv_dmactl_init_start,       impdrv_dmactl_init_end,         impdrv_dmactl_start,
		impdrv_dmactl_stop,             impdrv_dmactl_attr_init,        impdrv_dmactl_set_mem_init,
		impdrv_dmactl_set_core_map,     impdrv_dmactl_set_cl,           impdrv_dmactl_set_irq_mask,
		impdrv_dmactl_execute,          impdrv_dmactl_int_handler,      impdrv_dmactl_resume_exe,
		impdrv_dmactl_pm_set_policy,    impdrv_dmactl_pm_get_policy,    NULL,
		impdrv_dmactl_bus_if_check,     impdrv_dmactl_conf_reg_check
	},
	/* IMPDRV_CORE_TYPE_DMAC_SLIM */
	{   NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL
	},
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
#elif defined (V3H2)
static const st_impdrv_corectl_func_t s_corectl_func_tbl[IMPDRV_CORE_TYPE_MAX] =
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
	{   impdrv_impctl_is_valid_core,    impdrv_impctl_check_state,      impdrv_impctl_set_state,
		impdrv_impctl_init_start,       impdrv_impctl_init_end,         impdrv_impctl_start,
		impdrv_impctl_stop,             impdrv_impctl_attr_init,        impdrv_impctl_set_mem_init,
		impdrv_impctl_set_core_map,     impdrv_impctl_set_cl,           impdrv_impctl_set_irq_mask,
		impdrv_impctl_execute,          impdrv_impctl_int_handler,      impdrv_impctl_resume_exe,
		impdrv_impctl_pm_set_policy,    impdrv_impctl_pm_get_policy,    NULL,
		impdrv_impctl_bus_if_check,     impdrv_impctl_conf_reg_check
	},
	/* IMPDRV_CORE_TYPE_IMP_SLIM */
	{   impdrv_impsctl_is_valid_core,   impdrv_impsctl_check_state,     impdrv_impsctl_set_state,
		impdrv_impsctl_init_start,      impdrv_impsctl_init_end,        impdrv_impsctl_start,
		impdrv_impsctl_stop,            impdrv_impsctl_attr_init,       impdrv_impsctl_set_mem_init,
		impdrv_impsctl_set_core_map,    impdrv_impsctl_set_cl,          impdrv_impsctl_set_irq_mask,
		impdrv_impsctl_execute,         impdrv_impsctl_int_handler,     impdrv_impsctl_resume_exe,
		impdrv_impsctl_pm_set_policy,   impdrv_impsctl_pm_get_policy,   NULL,
		impdrv_impsctl_bus_if_check,    impdrv_impsctl_conf_reg_check
	},
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
	{   impdrv_dmactl_is_valid_core,    impdrv_dmactl_check_state,      impdrv_dmactl_set_state,
		impdrv_dmactl_init_start,       impdrv_dmactl_init_end,         impdrv_dmactl_start,
		impdrv_dmactl_stop,             impdrv_dmactl_attr_init,        impdrv_dmactl_set_mem_init,
		impdrv_dmactl_set_core_map,     impdrv_dmactl_set_cl,           impdrv_dmactl_set_irq_mask,
		impdrv_dmactl_execute,          impdrv_dmactl_int_handler,      impdrv_dmactl_resume_exe,
		impdrv_dmactl_pm_set_policy,    impdrv_dmactl_pm_get_policy,    NULL,
		impdrv_dmactl_bus_if_check,     impdrv_dmactl_conf_reg_check
	},
	/* IMPDRV_CORE_TYPE_DMAC_SLIM */
	{   impdrv_dmasctl_is_valid_core,   impdrv_dmasctl_check_state,     impdrv_dmasctl_set_state,
		impdrv_dmasctl_init_start,      impdrv_dmasctl_init_end,        impdrv_dmasctl_start,
		impdrv_dmasctl_stop,            impdrv_dmasctl_attr_init,       impdrv_dmasctl_set_mem_init,
		impdrv_dmasctl_set_core_map,    impdrv_dmasctl_set_cl,          impdrv_dmasctl_set_irq_mask,
		impdrv_dmasctl_execute,         impdrv_dmasctl_int_handler,     impdrv_dmasctl_resume_exe,
		impdrv_dmasctl_pm_set_policy,   impdrv_dmasctl_pm_get_policy,   NULL,
		NULL,                           impdrv_dmasctl_conf_reg_check
	},
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
#elif defined (V3M2)
static const st_impdrv_corectl_func_t s_corectl_func_tbl[IMPDRV_CORE_TYPE_MAX] =
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
	{   impdrv_impctl_is_valid_core,    impdrv_impctl_check_state,      impdrv_impctl_set_state,
		impdrv_impctl_init_start,       impdrv_impctl_init_end,         impdrv_impctl_start,
		impdrv_impctl_stop,             impdrv_impctl_attr_init,        impdrv_impctl_set_mem_init,
		impdrv_impctl_set_core_map,     impdrv_impctl_set_cl,           impdrv_impctl_set_irq_mask,
		impdrv_impctl_execute,          impdrv_impctl_int_handler,      impdrv_impctl_resume_exe,
		impdrv_impctl_pm_set_policy,    impdrv_impctl_pm_get_policy,    NULL,
		impdrv_impctl_bus_if_check,     impdrv_impctl_conf_reg_check
	},
	/* IMPDRV_CORE_TYPE_IMP_SLIM */
	{   NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL
	},
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
	{   impdrv_dmactl_is_valid_core,    impdrv_dmactl_check_state,      impdrv_dmactl_set_state,
		impdrv_dmactl_init_start,       impdrv_dmactl_init_end,         impdrv_dmactl_start,
		impdrv_dmactl_stop,             impdrv_dmactl_attr_init,        impdrv_dmactl_set_mem_init,
		impdrv_dmactl_set_core_map,     impdrv_dmactl_set_cl,           impdrv_dmactl_set_irq_mask,
		impdrv_dmactl_execute,          impdrv_dmactl_int_handler,      impdrv_dmactl_resume_exe,
		impdrv_dmactl_pm_set_policy,    impdrv_dmactl_pm_get_policy,    NULL,
		impdrv_dmactl_bus_if_check,     impdrv_dmactl_conf_reg_check
	},
	/* IMPDRV_CORE_TYPE_DMAC_SLIM */
	{   NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL
	},
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
#elif defined (V3U)
static const st_impdrv_corectl_func_t s_corectl_func_tbl[IMPDRV_CORE_TYPE_MAX] =
{
	/** Not Assigned */
	{   NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL
	},
	/** IMPDRV_CORE_TYPE_IMP */
	{   impdrv_impctl_is_valid_core,    impdrv_impctl_check_state,      impdrv_impctl_set_state,
		impdrv_impctl_init_start,       impdrv_impctl_init_end,         impdrv_impctl_start,
		impdrv_impctl_stop,             impdrv_impctl_attr_init,        impdrv_impctl_set_mem_init,
		impdrv_impctl_set_core_map,     impdrv_impctl_set_cl,           impdrv_impctl_set_irq_mask,
		impdrv_impctl_execute,          impdrv_impctl_int_handler,      impdrv_impctl_resume_exe,
		impdrv_impctl_pm_set_policy,    impdrv_impctl_pm_get_policy
	},
	/* IMPDRV_CORE_TYPE_IMP_SLIM */
	{   NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL
	},
	/** IMPDRV_CORE_TYPE_OCV */
	{   impdrv_ocvctl_is_valid_core,    impdrv_ocvctl_check_state,      impdrv_ocvctl_set_state,
		impdrv_ocvctl_init_start,       impdrv_ocvctl_init_end,         impdrv_ocvctl_start,
		impdrv_ocvctl_stop,             impdrv_ocvctl_attr_init,        impdrv_ocvctl_set_mem_init,
		impdrv_ocvctl_set_core_map,     impdrv_ocvctl_set_cl,           impdrv_ocvctl_set_irq_mask,
		impdrv_ocvctl_execute,          impdrv_ocvctl_int_handler,      impdrv_ocvctl_resume_exe,
		impdrv_ocvctl_pm_set_policy,    impdrv_ocvctl_pm_get_policy
	},
	/** IMPDRV_CORE_TYPE_DMAC */
	{   impdrv_dmactl_is_valid_core,    impdrv_dmactl_check_state,      impdrv_dmactl_set_state,
		impdrv_dmactl_init_start,       impdrv_dmactl_init_end,         impdrv_dmactl_start,
		impdrv_dmactl_stop,             impdrv_dmactl_attr_init,        impdrv_dmactl_set_mem_init,
		impdrv_dmactl_set_core_map,     impdrv_dmactl_set_cl,           impdrv_dmactl_set_irq_mask,
		impdrv_dmactl_execute,          impdrv_dmactl_int_handler,      impdrv_dmactl_resume_exe,
		impdrv_dmactl_pm_set_policy,    impdrv_dmactl_pm_get_policy
	},
	/* IMPDRV_CORE_TYPE_DMAC_SLIM */
	{   NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL
	},
	/** IMPDRV_CORE_TYPE_PSCEXE */
	{   impdrv_pscctl_is_valid_core,    impdrv_pscctl_check_state,      impdrv_pscctl_set_state,
		impdrv_pscctl_init_start,       impdrv_pscctl_init_end,         impdrv_pscctl_start,
		impdrv_pscctl_stop,             impdrv_pscctl_attr_init,        impdrv_pscctl_set_mem_init,
		impdrv_pscctl_set_core_map,     impdrv_pscctl_set_cl,           impdrv_pscctl_set_irq_mask,
		impdrv_pscctl_execute,          impdrv_pscctl_int_handler,      impdrv_pscctl_resume_exe,
		impdrv_pscctl_pm_set_policy,    impdrv_pscctl_pm_get_policy
	},
	/** IMPDRV_CORE_TYPE_PSCOUT */
	{   NULL,   NULL,   NULL,
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
		impdrv_cnnctl_pm_set_policy,    impdrv_cnnctl_pm_get_policy
	},
	/** IMPDRV_CORE_TYPE_LDMAC */
	{   NULL,   NULL,   NULL,
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
		NULL,   NULL
	},
};
#elif defined (V4H) || defined (V4H2)
static const st_impdrv_corectl_func_t s_corectl_func_tbl[IMPDRV_CORE_TYPE_MAX] =
{
    /** Not Assigned */
	{   NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL
	},
	/** IMPDRV_CORE_TYPE_IMP */
	{   impdrv_impctl_is_valid_core,    impdrv_impctl_check_state,      impdrv_impctl_set_state,
		impdrv_impctl_init_start,       impdrv_impctl_init_end,         impdrv_impctl_start,
		impdrv_impctl_stop,             impdrv_impctl_attr_init,        impdrv_impctl_set_mem_init,
		impdrv_impctl_set_core_map,     impdrv_impctl_set_cl,           impdrv_impctl_set_irq_mask,
		impdrv_impctl_execute,          impdrv_impctl_int_handler,      impdrv_impctl_resume_exe,
		impdrv_impctl_pm_set_policy,    impdrv_impctl_pm_get_policy,    NULL,
		NULL,                           NULL,                           impdrv_impctl_set_cl_brk_addr,
		impdrv_impctl_set_cond_gosub,   impdrv_impctl_pm_get_state,     impdrv_impctl_quit
	},
	/* IMPDRV_CORE_TYPE_IMP_SLIM */
	{   NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL
	},
	/** IMPDRV_CORE_TYPE_OCV */
	{   impdrv_ocvctl_is_valid_core,    impdrv_ocvctl_check_state,      impdrv_ocvctl_set_state,
		impdrv_ocvctl_init_start,       impdrv_ocvctl_init_end,         impdrv_ocvctl_start,
		impdrv_ocvctl_stop,             impdrv_ocvctl_attr_init,        impdrv_ocvctl_set_mem_init,
		impdrv_ocvctl_set_core_map,     impdrv_ocvctl_set_cl,           impdrv_ocvctl_set_irq_mask,
		impdrv_ocvctl_execute,          impdrv_ocvctl_int_handler,      impdrv_ocvctl_resume_exe,
		impdrv_ocvctl_pm_set_policy,    impdrv_ocvctl_pm_get_policy,    NULL,
		NULL,                           NULL,                           impdrv_ocvctl_set_cl_brk_addr,
		impdrv_ocvctl_set_cond_gosub,   impdrv_ocvctl_pm_get_state,     impdrv_ocvctl_quit
	},
	/** IMPDRV_CORE_TYPE_DMAC */
	{   impdrv_dmactl_is_valid_core,    impdrv_dmactl_check_state,      impdrv_dmactl_set_state,
		impdrv_dmactl_init_start,       impdrv_dmactl_init_end,         impdrv_dmactl_start,
		impdrv_dmactl_stop,             impdrv_dmactl_attr_init,        impdrv_dmactl_set_mem_init,
		impdrv_dmactl_set_core_map,     impdrv_dmactl_set_cl,           impdrv_dmactl_set_irq_mask,
		impdrv_dmactl_execute,          impdrv_dmactl_int_handler,      impdrv_dmactl_resume_exe,
		impdrv_dmactl_pm_set_policy,    impdrv_dmactl_pm_get_policy,    NULL,
		NULL,                           NULL,                           impdrv_dmactl_set_cl_brk_addr,
		impdrv_dmactl_set_cond_gosub,   impdrv_dmactl_pm_get_state,     impdrv_dmactl_quit
	},
	/* IMPDRV_CORE_TYPE_DMAC_SLIM */
	{   impdrv_dmasctl_is_valid_core,   impdrv_dmasctl_check_state,     impdrv_dmasctl_set_state,
		impdrv_dmasctl_init_start,      impdrv_dmasctl_init_end,        impdrv_dmasctl_start,
		impdrv_dmasctl_stop,            impdrv_dmasctl_attr_init,       impdrv_dmasctl_set_mem_init,
		impdrv_dmasctl_set_core_map,    impdrv_dmasctl_set_cl,          impdrv_dmasctl_set_irq_mask,
		impdrv_dmasctl_execute,         impdrv_dmasctl_int_handler,     impdrv_dmasctl_resume_exe,
		impdrv_dmasctl_pm_set_policy,   impdrv_dmasctl_pm_get_policy,   NULL,
		NULL,                           NULL,                           impdrv_dmasctl_set_cl_brk_addr,
		impdrv_dmasctl_set_cond_gosub,  impdrv_dmasctl_pm_get_state,    impdrv_dmasctl_quit
	},
	/** IMPDRV_CORE_TYPE_PSCEXE */
	{   impdrv_pscctl_is_valid_core,    impdrv_pscctl_check_state,      impdrv_pscctl_set_state,
		impdrv_pscctl_init_start,       impdrv_pscctl_init_end,         impdrv_pscctl_start,
		impdrv_pscctl_stop,             impdrv_pscctl_attr_init,        impdrv_pscctl_set_mem_init,
		impdrv_pscctl_set_core_map,     impdrv_pscctl_set_cl,           impdrv_pscctl_set_irq_mask,
		impdrv_pscctl_execute,          impdrv_pscctl_int_handler,      impdrv_pscctl_resume_exe,
		impdrv_pscctl_pm_set_policy,    impdrv_pscctl_pm_get_policy,    NULL,
		NULL,                           NULL,                           impdrv_pscctl_set_cl_brk_addr,
		impdrv_pscctl_set_cond_gosub,   impdrv_pscctl_pm_get_state,     impdrv_pscctl_quit
	},
	/** IMPDRV_CORE_TYPE_PSCOUT */
	{   NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL
	},
	/** IMPDRV_CORE_TYPE_CNN */
	{   impdrv_cnnctl_is_valid_core,    impdrv_cnnctl_check_state,      impdrv_cnnctl_set_state,
		impdrv_cnnctl_init_start,       impdrv_cnnctl_init_end,         impdrv_cnnctl_start,
		impdrv_cnnctl_stop,             impdrv_cnnctl_attr_init,        impdrv_cnnctl_set_mem_init,
		impdrv_cnnctl_set_core_map,     impdrv_cnnctl_set_cl,           impdrv_cnnctl_set_irq_mask,
		impdrv_cnnctl_execute,          impdrv_cnnctl_int_handler,      impdrv_cnnctl_resume_exe,
		impdrv_cnnctl_pm_set_policy,    impdrv_cnnctl_pm_get_policy,    NULL,
		NULL,                           NULL,                           impdrv_cnnctl_set_cl_brk_addr,
		impdrv_cnnctl_set_cond_gosub,   impdrv_cnnctl_pm_get_state,     impdrv_cnnctl_quit
	},
	/** IMPDRV_CORE_TYPE_DSP */
	{   impdrv_dspctl_is_valid_core,     impdrv_dspctl_check_state,      impdrv_dspctl_set_state,
		impdrv_dspctl_init_start,        impdrv_dspctl_init_end,         impdrv_dspctl_dsp_start,
		impdrv_dspctl_stop,              impdrv_dspctl_attr_init,        impdrv_dspctl_set_mem_init,
		impdrv_dspctl_set_core_map,      NULL,                           NULL,
		impdrv_dspctl_dsp_execute,       impdrv_dspctl_int_handler,      impdrv_dspctl_resume_exe,
		impdrv_dspctl_pm_set_policy,     impdrv_dspctl_pm_get_policy,    impdrv_dspctl_set_dsp,
		NULL,                            impdrv_dspctl_conf_reg_check,   NULL,
		NULL,                            impdrv_dspctl_pm_get_state,     impdrv_dspctl_quit
	},
	/** IMPDRV_CORE_TYPE_DTA */
	{   NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL,
		NULL,   NULL,   NULL
	}
};

// static const p_impdrv_dspctl_dsp_start_t corectl_dsp_func(void)
// {
// 	return 0;
// }
#endif

const st_impdrv_corectl_func_t* impdrv_cmnctl_get_corectl_func(
    const e_impdrv_core_type_t  core_type
)
{
    if (g_ret_impdrv_cmnctl_get_corectl_func != NULL)
	{
        g_ret_impdrv_cmnctl_get_corectl_func = &s_corectl_func_tbl[core_type];
    }

    return g_ret_impdrv_cmnctl_get_corectl_func;
}

e_impdrv_errorcode_t impdrv_cmnctl_save_irq_mask(
    st_impdrv_commonctl_t       *const p_commonctl
)
{
	return g_ret_impdrv_cmnctl_save_irq_mask;
}

e_impdrv_errorcode_t impdrv_cmnctl_restore_irq_mask(
    st_impdrv_commonctl_t       *const p_commonctl
)
{
	return g_ret_impdrv_cmnctl_restore_irq_mask;
}

e_impdrv_errorcode_t impdrv_cmnctl_force_irq_mask(
    const st_impdrv_commonctl_t     *const p_commonctl,
    const st_impdrv_core_info_t     *const p_core_info
)
{
	return g_ret_impdrv_cmnctl_force_irq_mask;
}

e_impdrv_errorcode_t impdrv_cmnctl_conf_reg_check(
    const st_impdrv_commonctl_t     *const p_commonctl
)
{
	return g_ret_impdrv_cmnctl_conf_reg_check;
}

e_impdrv_errorcode_t impdrv_cmnctl_reg_chk_mem_protect(
    const st_impdrv_commonctl_t     *const p_commonctl
)
{
	return g_ret_impdrv_cmnctl_reg_chk_mem_protect;
}

void it_stub_reset(void)
{
	g_ret_impdrv_cmnctl_get_corectl_func = &s_corectl_func_tbl;
	g_ret_impdrv_cmnctl_get_dsp_func = (volatile p_impdrv_dspctl_dsp_start_t)&impdrv_dspctl_dsp_start_pre;
	g_ret_impdrv_cmnctl_reg_init = IMPDRV_EC_OK;
	g_ret_impdrv_cmnctl_reg_mem_init = IMPDRV_EC_OK;
#if defined (V3H2)
	g_ret_impdrv_cmnctl_reg_set_mem_protect = IMPDRV_EC_OK;
#else
	g_ret_impdrv_cmnctl_reg_set_mem_protect = IMPDRV_EC_NG_NOTSUPPORT;
#endif
	g_ret_impdrv_cmnctl_reg_get_hw_info = IMPDRV_EC_OK;
	g_ret_impdrv_cmnctl_reg_read32 = IMPDRV_EC_OK;
	g_ret_impdrv_cmnctl_reg_write32 = IMPDRV_EC_OK;
	g_ret_impdrv_cmnctl_reg_required = IMPDRV_EC_OK;
	g_ret_impdrv_cmnctl_reg_quit = IMPDRV_EC_OK;
	g_ret_impdrv_cmnctl_mutex_create = IMPDRV_EC_OK;
	g_ret_impdrv_cmnctl_mutex_lock = IMPDRV_EC_OK;
	g_ret_impdrv_impctl_check_state = IMPDRV_EC_OK;
	g_ret_impdrv_impsctl_check_state = IMPDRV_EC_OK;
	g_ret_impdrv_ocvctl_check_state = IMPDRV_EC_OK;
	g_ret_impdrv_dmactl_check_state = IMPDRV_EC_OK;
	g_ret_impdrv_dmasctl_check_state = IMPDRV_EC_OK;
	g_ret_impdrv_pscctl_check_state = IMPDRV_EC_OK;
	g_ret_impdrv_cnnctl_check_state = IMPDRV_EC_OK;
	g_ret_impdrv_dspctl_check_state = IMPDRV_EC_OK;
	memset(g_ret_impdrv_impctl_set_state, IMPDRV_EC_OK, sizeof(g_ret_impdrv_impctl_set_state));
	g_count_impdrv_impctl_set_state = 0;
	memset(g_ret_impdrv_impsctl_set_state, IMPDRV_EC_OK, sizeof(g_ret_impdrv_impsctl_set_state));
	g_count_impdrv_impsctl_set_state = 0;
	memset(g_ret_impdrv_ocvctl_set_state, IMPDRV_EC_OK, sizeof(g_ret_impdrv_ocvctl_set_state));
	g_count_impdrv_ocvctl_set_state = 0;
	memset(g_ret_impdrv_dmactl_set_state, IMPDRV_EC_OK, sizeof(g_ret_impdrv_dmactl_set_state));
	g_count_impdrv_dmactl_set_state = 0;
	memset(g_ret_impdrv_dmasctl_set_state, IMPDRV_EC_OK, sizeof(g_ret_impdrv_dmasctl_set_state));
	g_count_impdrv_dmasctl_set_state = 0;
	memset(g_ret_impdrv_pscctl_set_state, IMPDRV_EC_OK, sizeof(g_ret_impdrv_pscctl_set_state));
	g_count_impdrv_pscctl_set_state = 0;
	memset(g_ret_impdrv_cnnctl_set_state, IMPDRV_EC_OK, sizeof(g_ret_impdrv_cnnctl_set_state));
	g_count_impdrv_cnnctl_set_state = 0;
	memset(g_ret_impdrv_dspctl_set_state, IMPDRV_EC_OK, sizeof(g_ret_impdrv_dspctl_set_state));
	g_count_impdrv_dspctl_set_state = 0;
	g_ret_impdrv_cmnctl_init = IMPDRV_EC_OK;
	g_ret_impdrv_impctl_init_start = IMPDRV_EC_OK;
	g_ret_impdrv_impsctl_init_start = IMPDRV_EC_OK;
	g_ret_impdrv_ocvctl_init_start = IMPDRV_EC_OK;
	g_ret_impdrv_dmactl_init_start = IMPDRV_EC_OK;
	g_ret_impdrv_dmasctl_init_start = IMPDRV_EC_OK;
	g_ret_impdrv_pscctl_init_start = IMPDRV_EC_OK;
	g_ret_impdrv_cnnctl_init_start = IMPDRV_EC_OK;
	g_ret_impdrv_dspctl_init_start = IMPDRV_EC_OK;
	g_ret_impdrv_impctl_init_end = IMPDRV_EC_OK;
	g_ret_impdrv_impsctl_init_end = IMPDRV_EC_OK;
	g_ret_impdrv_ocvctl_init_end = IMPDRV_EC_OK;
	g_ret_impdrv_dmactl_init_end = IMPDRV_EC_OK;
	g_ret_impdrv_dmasctl_init_end = IMPDRV_EC_OK;
	g_ret_impdrv_pscctl_init_end = IMPDRV_EC_OK;
	g_ret_impdrv_cnnctl_init_end = IMPDRV_EC_OK;
	g_ret_impdrv_dspctl_init_end = IMPDRV_EC_OK;
	g_ret_impdrv_impctl_attr_init = IMPDRV_EC_OK;
	g_ret_impdrv_impsctl_attr_init = IMPDRV_EC_OK;
	g_ret_impdrv_ocvctl_attr_init = IMPDRV_EC_OK;
	g_ret_impdrv_dmactl_attr_init = IMPDRV_EC_OK;
	g_ret_impdrv_dmasctl_attr_init = IMPDRV_EC_OK;
	g_ret_impdrv_pscctl_attr_init = IMPDRV_EC_OK;
	g_ret_impdrv_cnnctl_attr_init = IMPDRV_EC_OK;
	g_ret_impdrv_dspctl_attr_init = IMPDRV_EC_OK;
	g_ret_impdrv_cmnctl_mutex_unlock = IMPDRV_EC_OK;
	g_ret_impdrv_cmnctl_quit = IMPDRV_EC_OK;
	g_ret_impdrv_cmnctl_mutex_destroy = IMPDRV_EC_OK;
	is_valid_core_flag = true;
	g_ret_impdrv_impctl_is_valid_core = true;
	g_ret_impdrv_impsctl_is_valid_core = true;
	g_ret_impdrv_ocvctl_is_valid_core = true;
	g_ret_impdrv_dmactl_is_valid_core = true;
	g_ret_impdrv_dmasctl_is_valid_core = true;
	g_ret_impdrv_pscctl_is_valid_core = true;
	g_ret_impdrv_cnnctl_is_valid_core = true;
	g_ret_impdrv_dspctl_is_valid_core = true;
	g_ret_impdrv_impctl_start = IMPDRV_EC_OK;
	g_ret_impdrv_impsctl_start = IMPDRV_EC_OK;
	g_ret_impdrv_ocvctl_start = IMPDRV_EC_OK;
	g_ret_impdrv_dmactl_start = IMPDRV_EC_OK;
	g_ret_impdrv_dmasctl_start = IMPDRV_EC_OK;
	g_ret_impdrv_pscctl_start = IMPDRV_EC_OK;
	g_ret_impdrv_cnnctl_start = IMPDRV_EC_OK;
	g_ret_impdrv_dspctl_dsp_start = IMPDRV_EC_OK;
	g_ret_impdrv_impctl_stop = IMPDRV_EC_OK;
	g_ret_impdrv_impsctl_stop = IMPDRV_EC_OK;
	g_ret_impdrv_ocvctl_stop = IMPDRV_EC_OK;
	g_ret_impdrv_dmactl_stop = IMPDRV_EC_OK;
	g_ret_impdrv_dmasctl_stop = IMPDRV_EC_OK;
	g_ret_impdrv_pscctl_stop = IMPDRV_EC_OK;
	g_ret_impdrv_cnnctl_stop = IMPDRV_EC_OK;
	g_ret_impdrv_dspctl_stop = IMPDRV_EC_OK;
	g_ret_impdrv_impctl_set_mem_init = IMPDRV_EC_OK;
	g_ret_impdrv_impsctl_set_mem_init = IMPDRV_EC_OK;
	g_ret_impdrv_ocvctl_set_mem_init = IMPDRV_EC_OK;
	g_ret_impdrv_dmactl_set_mem_init = IMPDRV_EC_OK;
	g_ret_impdrv_dmasctl_set_mem_init = IMPDRV_EC_OK;
	g_ret_impdrv_pscctl_set_mem_init = IMPDRV_EC_OK;
	g_ret_impdrv_cnnctl_set_mem_init = IMPDRV_EC_OK;
	g_ret_impdrv_dspctl_set_mem_init = IMPDRV_EC_OK;
	g_ret_impdrv_impctl_set_core_map = IMPDRV_EC_OK;
	g_ret_impdrv_impsctl_set_core_map = IMPDRV_EC_OK;
	g_ret_impdrv_ocvctl_set_core_map = IMPDRV_EC_OK;
	g_ret_impdrv_dmactl_set_core_map = IMPDRV_EC_OK;
	g_ret_impdrv_dmasctl_set_core_map = IMPDRV_EC_OK;
	g_ret_impdrv_pscctl_set_core_map = IMPDRV_EC_OK;
	g_ret_impdrv_cnnctl_set_core_map = IMPDRV_EC_OK;
	g_ret_impdrv_dspctl_set_core_map = IMPDRV_EC_OK;
	g_ret_impdrv_impctl_set_cl = IMPDRV_EC_OK;
	g_ret_impdrv_impsctl_set_cl = IMPDRV_EC_OK;
	g_ret_impdrv_ocvctl_set_cl = IMPDRV_EC_OK;
	g_ret_impdrv_dmactl_set_cl = IMPDRV_EC_OK;
	g_ret_impdrv_dmasctl_set_cl = IMPDRV_EC_OK;
	g_ret_impdrv_pscctl_set_cl = IMPDRV_EC_OK;
	g_ret_impdrv_cnnctl_set_cl = IMPDRV_EC_OK;
	g_ret_impdrv_dspctl_set_dsp = IMPDRV_EC_OK;
	g_ret_impdrv_cmnctl_set_irq_group = IMPDRV_EC_OK;
	g_ret_impdrv_impctl_set_irq_mask = IMPDRV_EC_OK;
	g_ret_impdrv_impsctl_set_irq_mask = IMPDRV_EC_OK;
	g_ret_impdrv_ocvctl_set_irq_mask = IMPDRV_EC_OK;
	g_ret_impdrv_dmactl_set_irq_mask = IMPDRV_EC_OK;
	g_ret_impdrv_dmasctl_set_irq_mask = IMPDRV_EC_OK;
	g_ret_impdrv_pscctl_set_irq_mask = IMPDRV_EC_OK;
	g_ret_impdrv_cnnctl_set_irq_mask = IMPDRV_EC_OK;
	g_ret_impdrv_impctl_execute = IMPDRV_EC_OK;
	g_ret_impdrv_impsctl_execute = IMPDRV_EC_OK;
	g_ret_impdrv_ocvctl_execute = IMPDRV_EC_OK;
	g_ret_impdrv_dmactl_execute = IMPDRV_EC_OK;
	g_ret_impdrv_dmasctl_execute = IMPDRV_EC_OK;
	g_ret_impdrv_pscctl_execute = IMPDRV_EC_OK;
	g_ret_impdrv_cnnctl_execute = IMPDRV_EC_OK;
	g_ret_impdrv_dspctl_dsp_execute = IMPDRV_EC_OK;
	g_ret_impdrv_impctl_int_handler = IMPDRV_EC_OK;
	g_ret_impdrv_impsctl_int_handler = IMPDRV_EC_OK;
	g_ret_impdrv_ocvctl_int_handler = IMPDRV_EC_OK;
	g_ret_impdrv_dmactl_int_handler = IMPDRV_EC_OK;
	g_ret_impdrv_dmasctl_int_handler = IMPDRV_EC_OK;
	g_ret_impdrv_pscctl_int_handler = IMPDRV_EC_OK;
	g_ret_impdrv_cnnctl_int_handler = IMPDRV_EC_OK;
	g_ret_impdrv_dspctl_int_handler = IMPDRV_EC_OK;
	g_ret_impdrv_impctl_resume_exe = IMPDRV_EC_OK;
	g_ret_impdrv_impsctl_resume_exe = IMPDRV_EC_OK;
	g_ret_impdrv_ocvctl_resume_exe = IMPDRV_EC_OK;
	g_ret_impdrv_dmactl_resume_exe = IMPDRV_EC_OK;
	g_ret_impdrv_dmasctl_resume_exe = IMPDRV_EC_OK;
	g_ret_impdrv_pscctl_resume_exe = IMPDRV_EC_OK;
	g_ret_impdrv_cnnctl_resume_exe = IMPDRV_EC_OK;
	g_ret_impdrv_dspctl_resume_exe = IMPDRV_EC_OK;
	g_ret_impdrv_impctl_pm_set_policy = IMPDRV_EC_OK;
	g_ret_impdrv_impsctl_pm_set_policy = IMPDRV_EC_OK;
	g_ret_impdrv_ocvctl_pm_set_policy = IMPDRV_EC_OK;
	g_ret_impdrv_dmactl_pm_set_policy = IMPDRV_EC_OK;
	g_ret_impdrv_dmasctl_pm_set_policy = IMPDRV_EC_OK;
	g_ret_impdrv_pscctl_pm_set_policy = IMPDRV_EC_OK;
	g_ret_impdrv_cnnctl_pm_set_policy = IMPDRV_EC_OK;
	g_ret_impdrv_dspctl_pm_set_policy = IMPDRV_EC_OK;
	g_ret_impdrv_impctl_pm_get_policy = IMPDRV_EC_OK;
	g_ret_impdrv_impsctl_pm_get_policy = IMPDRV_EC_OK;
	g_ret_impdrv_ocvctl_pm_get_policy = IMPDRV_EC_OK;
	g_ret_impdrv_dmactl_pm_get_policy = IMPDRV_EC_OK;
	g_ret_impdrv_dmasctl_pm_get_policy = IMPDRV_EC_OK;
	g_ret_impdrv_pscctl_pm_get_policy = IMPDRV_EC_OK;
	g_ret_impdrv_cnnctl_pm_get_policy = IMPDRV_EC_OK;
	g_ret_impdrv_dspctl_pm_get_policy = IMPDRV_EC_OK;
    g_ret_impdrv_cmnctl_get_syncc_val = IMPDRV_EC_OK;
	g_ret_impdrv_dmactl_mb_init = IMPDRV_EC_OK;
	g_ret_impdrv_cmnctl_save_irq_mask = IMPDRV_EC_OK;
	g_ret_impdrv_cmnctl_restore_irq_mask = IMPDRV_EC_OK;
	g_ret_impdrv_cmnctl_force_irq_mask = IMPDRV_EC_OK;
	g_ret_impdrv_cmnctl_conf_reg_check = IMPDRV_EC_OK;
	g_ret_impdrv_cmnctl_reg_chk_mem_protect = IMPDRV_EC_OK;
	g_ret_impdrv_impctl_bus_if_check = IMPDRV_EC_OK;
	g_ret_impdrv_impsctl_bus_if_check = IMPDRV_EC_OK;
	g_ret_impdrv_dmactl_bus_if_check = IMPDRV_EC_OK;
	g_ret_impdrv_impctl_conf_reg_check = IMPDRV_EC_OK;
	g_ret_impdrv_impsctl_conf_reg_check = IMPDRV_EC_OK;
	g_ret_impdrv_ocvctl_conf_reg_check = IMPDRV_EC_OK;
	g_ret_impdrv_dmactl_conf_reg_check = IMPDRV_EC_OK;
	g_ret_impdrv_dmasctl_conf_reg_check = IMPDRV_EC_OK;
	g_ret_impdrv_pscctl_conf_reg_check = IMPDRV_EC_OK;
	g_ret_impdrv_cnnctl_conf_reg_check = IMPDRV_EC_OK;
	g_ret_impdrv_dspctl_conf_reg_check = IMPDRV_EC_OK;
    g_ret_impdrv_cmnctl_get_init_param = IMPDRV_EC_OK;

	g_ret_impdrv_impctl_set_cl_brk_addr = IMPDRV_EC_OK;
	g_ret_impdrv_ocvctl_set_cl_brk_addr = IMPDRV_EC_OK;
	g_ret_impdrv_dmactl_set_cl_brk_addr = IMPDRV_EC_OK;
	g_ret_impdrv_dmasctl_set_cl_brk_addr = IMPDRV_EC_OK;
	g_ret_impdrv_pscctl_set_cl_brk_addr = IMPDRV_EC_OK;
	g_ret_impdrv_cnnctl_set_cl_brk_addr = IMPDRV_EC_OK;
	g_ret_impdrv_impctl_set_cond_gosub = IMPDRV_EC_OK;
	g_ret_impdrv_ocvctl_set_cond_gosub = IMPDRV_EC_OK;
	g_ret_impdrv_dmactl_set_cond_gosub = IMPDRV_EC_OK;
	g_ret_impdrv_dmasctl_set_cond_gosub = IMPDRV_EC_OK;
	g_ret_impdrv_pscctl_set_cond_gosub = IMPDRV_EC_OK;
	g_ret_impdrv_cnnctl_set_cond_gosub = IMPDRV_EC_OK;
	g_ret_impdrv_dspctl_dsp_start_pre = IMPDRV_EC_OK;

	g_ret_impdrv_impctl_pm_get_state = IMPDRV_EC_OK;
	g_ret_impdrv_ocvctl_pm_get_state = IMPDRV_EC_OK;
	g_ret_impdrv_dmactl_pm_get_state = IMPDRV_EC_OK;
	g_ret_impdrv_dmasctl_pm_get_state = IMPDRV_EC_OK;
	g_ret_impdrv_pscctl_pm_get_state = IMPDRV_EC_OK;
	g_ret_impdrv_cnnctl_pm_get_state = IMPDRV_EC_OK;
	g_ret_impdrv_dspctl_pm_get_state = IMPDRV_EC_OK;

	g_ret_impdrv_impctl_quit = IMPDRV_EC_OK;
	g_ret_impdrv_ocvctl_quit = IMPDRV_EC_OK;
	g_ret_impdrv_dmactl_quit = IMPDRV_EC_OK;
	g_ret_impdrv_dmasctl_quit = IMPDRV_EC_OK;
	g_ret_impdrv_pscctl_quit = IMPDRV_EC_OK;
	g_ret_impdrv_cnnctl_quit = IMPDRV_EC_OK;
	g_ret_impdrv_dspctl_quit = IMPDRV_EC_OK;
}
