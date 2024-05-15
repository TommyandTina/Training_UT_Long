#include <stdio.h>
#include <string.h>
#include "it_common.h"
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

#define READ_REG_TIME       (100U)
#define MEMORY_NUM          (20U)

static volatile uint32_t g_chk_core_idx[READ_REG_TIME];
static volatile uint64_t g_chk_irq_ch[READ_REG_TIME];
static volatile uint32_t g_read_chk_offset[READ_REG_TIME];
static volatile uint32_t g_read_value[READ_REG_TIME];
static volatile uint32_t g_write_chk_offset;
volatile uint32_t g_read_reg_cnt;
static volatile st_impdrv_core_info_t g_core_info;
static volatile size_t g_mem_size[MEMORY_NUM];
volatile uint32_t g_mem_size_cnt;
static volatile void *g_cpu_addr[MEMORY_NUM];
volatile uint32_t g_logical_addr_cnt;
static volatile uintptr_t g_hw_addr[MEMORY_NUM];
volatile uint32_t g_hw_addr_cnt;
static uint32_t g_buffer;

static volatile e_impdrv_errorcode_t g_ret_impdrv_osdep_read_reg[READ_REG_TIME];
static volatile e_impdrv_errorcode_t g_ret_impdrv_osdep_write_reg;
static volatile e_impdrv_errorcode_t g_ret_impdrv_osdep_pow_on_imp;
static volatile e_impdrv_errorcode_t g_ret_impdrv_osdep_memory_get_size[MEMORY_NUM];
static volatile e_impdrv_errorcode_t g_ret_impdrv_osdep_memory_get_logical[MEMORY_NUM];
static volatile e_impdrv_errorcode_t g_ret_impdrv_osdep_memory_get_physical[MEMORY_NUM];
static volatile e_impdrv_errorcode_t g_ret_impdrv_osdep_queue_wait_period;

static uint32_t device_io_handle;
static uint32_t imp_top_handle;
static uint32_t hwrsc_handle;

STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_mutex_create)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_mutex_lock)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_pow_on_imp_top)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_register_irq)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_enable_irq)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_pow_off_imp)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_mutex_unlock)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_disable_irq)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_unregister_irq)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_pow_off_imp_top)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_mutex_destroy)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_pm_set_policy)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_pm_get_policy)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_reset_core)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_pow_on_hwrsc)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_pow_off_hwrsc)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_memory_barrier)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_queue_send_period)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_memory_data_set)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_queue_create)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_queue_destroy)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_memory_cache_clean)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_memory_compare)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_memory_cache_flush)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_dev_close)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_pm_get_state)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_dev_open_imp)
#if defined (V4H) || defined (V4H2)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_enable_dsp_irq)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_disable_dsp_irq)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_register_dsp_irq)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_unregister_dsp_irq)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_pow_on_dsp)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_pow_off_dsp)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_pow_on_imp_dsp)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_reset_core_dsp)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_osdep_dev_open_dsp)
#endif

void it_stub_reset(void)
{
    memset(g_chk_core_idx, 0, sizeof(g_chk_core_idx));
    memset(g_read_chk_offset, RCVDRV_IMP_OFFSET_ALL, sizeof(g_read_chk_offset));
    memset(g_chk_irq_ch, 0, sizeof(g_chk_irq_ch));
    memset(g_read_value, 0, sizeof(g_read_value));
    g_read_reg_cnt = 0;
    g_write_chk_offset = RCVDRV_IMP_OFFSET_ALL;
    g_mem_size_cnt = 0;
    g_logical_addr_cnt = 0;
    g_hw_addr_cnt = 0;

	g_ret_impdrv_osdep_mutex_create = IMPDRV_EC_OK;
	g_ret_impdrv_osdep_mutex_lock = IMPDRV_EC_OK;
	g_ret_impdrv_osdep_pow_on_imp_top = IMPDRV_EC_OK;
	g_ret_impdrv_osdep_register_irq = IMPDRV_EC_OK;
	g_ret_impdrv_osdep_enable_irq = IMPDRV_EC_OK;
	memset(g_ret_impdrv_osdep_read_reg, IMPDRV_EC_OK, sizeof(g_ret_impdrv_osdep_read_reg));
	g_ret_impdrv_osdep_write_reg = IMPDRV_EC_OK;
	g_ret_impdrv_osdep_pow_on_imp = IMPDRV_EC_OK;
    g_core_info.core_type = IMPDRV_CORE_TYPE_IMP;
    g_core_info.core_num = 0;
	g_ret_impdrv_osdep_pow_off_imp = IMPDRV_EC_OK;
	g_ret_impdrv_osdep_mutex_unlock = IMPDRV_EC_OK;
	g_ret_impdrv_osdep_disable_irq = IMPDRV_EC_OK;
	g_ret_impdrv_osdep_unregister_irq = IMPDRV_EC_OK;
	g_ret_impdrv_osdep_pow_off_imp_top = IMPDRV_EC_OK;
	g_ret_impdrv_osdep_mutex_destroy = IMPDRV_EC_OK;
	g_ret_impdrv_osdep_pm_set_policy = IMPDRV_EC_OK;
	g_ret_impdrv_osdep_pm_get_policy = IMPDRV_EC_OK;
    g_ret_impdrv_osdep_pm_get_state = IMPDRV_EC_OK;
	g_ret_impdrv_osdep_reset_core = IMPDRV_EC_OK;
    g_ret_impdrv_osdep_pow_on_hwrsc = IMPDRV_EC_OK;
    g_ret_impdrv_osdep_pow_off_hwrsc = IMPDRV_EC_OK;
    g_ret_impdrv_osdep_memory_barrier = IMPDRV_EC_OK;
    g_ret_impdrv_osdep_queue_send_period = IMPDRV_EC_OK;
    memset(g_ret_impdrv_osdep_memory_get_size, IMPDRV_EC_OK, sizeof(g_ret_impdrv_osdep_memory_get_size));
    memset(g_mem_size, 0, sizeof(g_mem_size));
    memset(g_ret_impdrv_osdep_memory_get_logical, IMPDRV_EC_OK, sizeof(g_ret_impdrv_osdep_memory_get_logical));
    for (int8_t i = 0; i < MEMORY_NUM; i++)
    {
        g_cpu_addr[i] = NULL;
    }
    memset(g_ret_impdrv_osdep_memory_get_physical, IMPDRV_EC_OK, sizeof(g_ret_impdrv_osdep_memory_get_physical));
    memset(g_hw_addr, 0, sizeof(g_hw_addr));
    g_ret_impdrv_osdep_memory_data_set = IMPDRV_EC_OK;
    g_ret_impdrv_osdep_queue_create = IMPDRV_EC_OK;
    g_ret_impdrv_osdep_queue_wait_period = IMPDRV_EC_OK;
    g_buffer = 0;
    g_ret_impdrv_osdep_queue_destroy = IMPDRV_EC_OK;
    g_ret_impdrv_osdep_memory_cache_clean = IMPDRV_EC_OK;
    g_ret_impdrv_osdep_memory_compare = IMPDRV_EC_OK;
    g_ret_impdrv_osdep_memory_cache_flush = IMPDRV_EC_OK;
    g_ret_impdrv_osdep_dev_close = IMPDRV_EC_OK;
    g_ret_impdrv_osdep_dev_open_imp = IMPDRV_EC_OK;

#if defined (V4H) || defined (V4H2)
    g_ret_impdrv_osdep_enable_dsp_irq = IMPDRV_EC_OK;
    g_ret_impdrv_osdep_disable_dsp_irq = IMPDRV_EC_OK;
    g_ret_impdrv_osdep_register_dsp_irq = IMPDRV_EC_OK;
    g_ret_impdrv_osdep_unregister_dsp_irq = IMPDRV_EC_OK;
    g_ret_impdrv_osdep_pow_on_dsp = IMPDRV_EC_OK;
    g_ret_impdrv_osdep_pow_off_dsp = IMPDRV_EC_OK;
    g_ret_impdrv_osdep_pow_on_imp_dsp = IMPDRV_EC_OK;
    g_ret_impdrv_osdep_reset_core_dsp = IMPDRV_EC_OK;
    g_ret_impdrv_osdep_dev_open_dsp = IMPDRV_EC_OK;
#endif
}

/*************************************************
 * IMP Driver Stub function setting
 * **********************************************/
void it_stub_set_impdrv_osdep_read_reg(e_impdrv_errorcode_t ret, uint32_t index, uint32_t read_value)
{
    g_ret_impdrv_osdep_read_reg[index] = ret;
    g_read_value[index] = read_value;
}

void it_stub_set_impdrv_osdep_write_reg(e_impdrv_errorcode_t ret, uint32_t offset)
{
	g_write_chk_offset = offset;
	g_ret_impdrv_osdep_write_reg = ret;
}

void it_stub_set_impdrv_osdep_pow_on_imp(e_impdrv_errorcode_t ret, st_impdrv_core_info_t *core_info)
{
    memcpy(&g_core_info, core_info, sizeof(st_impdrv_core_info_t));
    g_ret_impdrv_osdep_pow_on_imp = ret;
}

void it_stub_set_impdrv_osdep_memory_get_size(e_impdrv_errorcode_t ret, uint32_t index, size_t mem_size)
{
    g_ret_impdrv_osdep_memory_get_size[index] = ret;
    g_mem_size[index] = mem_size;
}

void it_stub_set_impdrv_osdep_memory_get_logical(e_impdrv_errorcode_t ret, uint32_t index, void *cpu_addr)
{
    g_ret_impdrv_osdep_memory_get_logical[index] = ret;
    g_cpu_addr[index] = cpu_addr;
}

void it_stub_set_impdrv_osdep_memory_get_physical(e_impdrv_errorcode_t ret, uint32_t index, uint32_t phys_addr)
{
    g_ret_impdrv_osdep_memory_get_physical[index] = ret;
    g_hw_addr[index] = (uintptr_t)phys_addr;
}

void it_stub_set_impdrv_osdep_queue_wait_period(e_impdrv_errorcode_t ret, uint32_t core_num)
{
    g_ret_impdrv_osdep_queue_wait_period = ret;
    g_buffer = core_num;
}

/***********************************************************************************
 * Stub functions
 **********************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_mutex_create(
    st_impdrv_mutex_handle_t            *const p_mutex_handle,
    const uint64_t                      mutex_id
)
{
	return g_ret_impdrv_osdep_mutex_create;
}

e_impdrv_errorcode_t impdrv_osdep_mutex_lock(
    const st_impdrv_mutex_handle_t      *const p_mutex_handle,
    const int64_t                       time_period
)
{
	return g_ret_impdrv_osdep_mutex_lock;
}

e_impdrv_errorcode_t impdrv_osdep_pow_on_imp_top(
    st_impdrv_device_handle_t           *const p_device_handle
)
{
    if (IMPDRV_EC_OK == g_ret_impdrv_osdep_pow_on_imp_top)
    {
        p_device_handle->handle = (osal_device_handle_t)&imp_top_handle;
    }

	return g_ret_impdrv_osdep_pow_on_imp_top;
}

e_impdrv_errorcode_t impdrv_osdep_register_irq(
    const st_impdrv_device_handle_t     *const p_device_handle,
    const e_impdrv_instance_t           instance_num,
    const uint32_t                      irq_priority,
    const impdrv_ctrl_handle_t          osal_cb_args
)
{
	return g_ret_impdrv_osdep_register_irq;
}

e_impdrv_errorcode_t impdrv_osdep_enable_irq(
    const st_impdrv_device_handle_t     *const p_device_handle,
    const e_impdrv_instance_t           instance_num
)
{
	return g_ret_impdrv_osdep_enable_irq;
}

e_impdrv_errorcode_t impdrv_osdep_read_reg(
    const st_impdrv_device_handle_t     *const p_device_handle,
    const uint32_t                      offset,
    uint32_t                            *p_val
)
{
    e_impdrv_errorcode_t ret = g_ret_impdrv_osdep_read_reg[g_read_reg_cnt];

    if (g_ret_impdrv_osdep_read_reg[g_read_reg_cnt] == IMPDRV_EC_OK)
    {
        if (p_val) {
            *p_val = g_read_value[g_read_reg_cnt];
        }
    }
    g_read_reg_cnt++;

    return ret;
}

e_impdrv_errorcode_t impdrv_osdep_write_reg(
    const st_impdrv_device_handle_t     *const p_device_handle,
    const uint32_t                      offset,
    const uint32_t                      val,
    const bool                          read_back,
    const uint32_t                      chk_val
)
{
	e_impdrv_errorcode_t ret = IMPDRV_EC_OK;

	if ((g_write_chk_offset == RCVDRV_IMP_OFFSET_ALL) || (g_write_chk_offset == offset))
	{
		ret = g_ret_impdrv_osdep_write_reg;
	}

	return ret;
}

e_impdrv_errorcode_t impdrv_osdep_pow_on_imp(
    st_impdrv_device_handle_t           *const p_device_handle,
    const st_impdrv_core_info_t         core_info
)
{
    e_impdrv_errorcode_t ret = IMPDRV_EC_OK;

    if ((g_core_info.core_type == core_info.core_type) && (g_core_info.core_num == core_info.core_num))
    {
        ret = g_ret_impdrv_osdep_pow_on_imp;
    }

    if (IMPDRV_EC_OK == ret)
    {
        p_device_handle->handle = (osal_device_handle_t)&device_io_handle;
    }

	return ret;
}

e_impdrv_errorcode_t impdrv_osdep_pow_off_imp(
    st_impdrv_device_handle_t           *const p_device_handle
)
{
    if (IMPDRV_EC_OK == g_ret_impdrv_osdep_pow_off_imp)
    {
        p_device_handle->handle = NULL;
    }

	return g_ret_impdrv_osdep_pow_off_imp;
}

e_impdrv_errorcode_t impdrv_osdep_mutex_unlock(
    const st_impdrv_mutex_handle_t      *const p_mutex_handle
)
{
    return g_ret_impdrv_osdep_mutex_unlock;
}

e_impdrv_errorcode_t impdrv_osdep_disable_irq(
    const st_impdrv_device_handle_t     *const p_device_handle,
    const e_impdrv_instance_t           instance_num
)
{
	return g_ret_impdrv_osdep_disable_irq;
}

e_impdrv_errorcode_t impdrv_osdep_unregister_irq(
    const st_impdrv_device_handle_t     *const p_device_handle,
    const e_impdrv_instance_t           instance_num
)
{
	return g_ret_impdrv_osdep_unregister_irq;
}

e_impdrv_errorcode_t impdrv_osdep_pow_off_imp_top(
    st_impdrv_device_handle_t           *const p_device_handle
)
{
    if (IMPDRV_EC_OK == g_ret_impdrv_osdep_pow_off_imp_top)
    {
        p_device_handle->handle = NULL;
    }

	return g_ret_impdrv_osdep_pow_off_imp_top;
}

e_impdrv_errorcode_t impdrv_osdep_mutex_destroy(
    st_impdrv_mutex_handle_t            *const p_mutex_handle
)
{
	return g_ret_impdrv_osdep_mutex_destroy;
}

e_impdrv_errorcode_t impdrv_osdep_pm_set_policy(
    st_impdrv_device_handle_t           *const p_device_handle,
    const st_impdrv_core_info_t         core_info,
    const e_impdrv_pm_policy_t          policy
)
{
	return g_ret_impdrv_osdep_pm_set_policy;
}

e_impdrv_errorcode_t impdrv_osdep_pm_get_policy(
    st_impdrv_device_handle_t           *const p_device_handle,
    const st_impdrv_core_info_t         core_info,
    e_impdrv_pm_policy_t                *const p_policy
)
{
	return g_ret_impdrv_osdep_pm_get_policy;
}

e_impdrv_errorcode_t impdrv_osdep_pm_get_state(
    st_impdrv_device_handle_t           *const p_device_handle,
    const st_impdrv_core_info_t         core_info,
    e_impdrv_pm_state_t                 *const p_pmstate
)
{
    return g_ret_impdrv_osdep_pm_get_state;
}

e_impdrv_errorcode_t impdrv_osdep_reset_core(
    const st_impdrv_device_handle_t     *const p_device_handle
)
{
	return g_ret_impdrv_osdep_reset_core;
}

e_impdrv_errorcode_t impdrv_osdep_pow_on_hwrsc(
    st_impdrv_device_handle_t           *const p_device_handle,
    const char                          *const p_device_id
)
{
    if (IMPDRV_EC_OK == g_ret_impdrv_osdep_pow_on_hwrsc)
    {
        p_device_handle->handle = (osal_device_handle_t)&hwrsc_handle;
    }

    return g_ret_impdrv_osdep_pow_on_hwrsc;
}

e_impdrv_errorcode_t impdrv_osdep_pow_off_hwrsc(
    st_impdrv_device_handle_t           *const p_device_handle
)
{
    if (IMPDRV_EC_OK == g_ret_impdrv_osdep_pow_off_hwrsc)
    {
        p_device_handle->handle = NULL;
    }

    return g_ret_impdrv_osdep_pow_off_hwrsc;
}

e_impdrv_errorcode_t impdrv_osdep_memory_barrier(
    void
)
{
    return g_ret_impdrv_osdep_memory_barrier;
}

e_impdrv_errorcode_t impdrv_osdep_queue_send_period(
    const st_impdrv_queue_handle_t  *const p_queue_handle,
    const osal_milli_sec_t          time_period,
    const uint32_t                  *const p_send_buffer
)
{
    return g_ret_impdrv_osdep_queue_send_period;
}

e_impdrv_errorcode_t impdrv_osdep_memory_get_size(
    const st_impdrv_memory_handle_t *const p_memory_handle,
    size_t                          *const p_size
)
{
    e_impdrv_errorcode_t ret = g_ret_impdrv_osdep_memory_get_size[g_mem_size_cnt];

    if (IMPDRV_EC_OK == g_ret_impdrv_osdep_memory_get_size[g_mem_size_cnt])
    {
        if (p_size) {
            *p_size = g_mem_size[g_mem_size_cnt];
        }
    }

    g_mem_size_cnt++;

    return ret;
}

e_impdrv_errorcode_t impdrv_osdep_memory_get_logical(
    const st_impdrv_memory_handle_t *const p_memory_handle,
    void                            **const p_logical_adrs
)
{
    e_impdrv_errorcode_t ret = g_ret_impdrv_osdep_memory_get_logical[g_logical_addr_cnt];

    if (IMPDRV_EC_OK == g_ret_impdrv_osdep_memory_get_logical[g_logical_addr_cnt])
    {
        if (p_logical_adrs) {
            *p_logical_adrs = g_cpu_addr[g_logical_addr_cnt];
        }
    }

    g_logical_addr_cnt++;

    return ret;
}

e_impdrv_errorcode_t impdrv_osdep_memory_get_physical(
    const st_impdrv_memory_handle_t *const p_memory_handle,
    const osal_axi_bus_id_t         axi_id,
    uintptr_t                       *const p_physical_adrs
)
{
    e_impdrv_errorcode_t ret = g_ret_impdrv_osdep_memory_get_physical[g_hw_addr_cnt];

    if (IMPDRV_EC_OK == g_ret_impdrv_osdep_memory_get_physical[g_hw_addr_cnt])
    {
        if (p_physical_adrs) {
            *p_physical_adrs = g_hw_addr[g_hw_addr_cnt];
        }
    }

    g_hw_addr_cnt++;

    return ret;
}

e_impdrv_errorcode_t impdrv_osdep_memory_data_set(
    uint32_t                    *const p_buffer,
    const uint32_t              data,
    const size_t                size
)
{
    return g_ret_impdrv_osdep_memory_data_set;
}

e_impdrv_errorcode_t impdrv_osdep_queue_create(
    st_impdrv_queue_handle_t    *const p_queue_handle,
    const osal_mq_id_t          queue_id
)
{
    return g_ret_impdrv_osdep_queue_create;
}

e_impdrv_errorcode_t impdrv_osdep_queue_wait_period(
    const st_impdrv_queue_handle_t  *const p_queue_handle,
    const osal_milli_sec_t          time_period,
    uint32_t                        *const p_receive_buffer
)
{
    if (IMPDRV_EC_OK == g_ret_impdrv_osdep_queue_wait_period)
    {
        if (p_receive_buffer) {
            *p_receive_buffer = g_buffer;
        }
    }

    return g_ret_impdrv_osdep_queue_wait_period;
}

e_impdrv_errorcode_t impdrv_osdep_queue_destroy(
    st_impdrv_queue_handle_t    *const p_queue_handle
)
{
    return g_ret_impdrv_osdep_queue_destroy;
}

e_impdrv_errorcode_t impdrv_osdep_memory_cache_clean(
    const st_impdrv_memory_handle_t *const p_memory_handle,
    const uintptr_t                 offset,
    const size_t                    size
)
{
    return g_ret_impdrv_osdep_memory_cache_clean;
}

e_impdrv_errorcode_t impdrv_osdep_memory_compare(
    const uint32_t              *const p_expected_area,
    const uint32_t              *const p_result_area,
    const size_t                size
)
{
    return g_ret_impdrv_osdep_memory_compare;
}

e_impdrv_errorcode_t impdrv_osdep_memory_cache_flush(
    const st_impdrv_memory_handle_t *const p_memory_handle,
    const uintptr_t                 offset,
    const size_t                    size
)
{
    return g_ret_impdrv_osdep_memory_cache_flush;
}

e_impdrv_errorcode_t impdrv_osdep_dev_close(
    st_impdrv_device_handle_t   *const p_device_handle
)
{
    return g_ret_impdrv_osdep_dev_close;
}

e_impdrv_errorcode_t impdrv_osdep_dev_open_imp(
    st_impdrv_device_handle_t   *const p_device_handle,
    const st_impdrv_core_info_t core_info
)
{
    return g_ret_impdrv_osdep_dev_open_imp;
}

#if defined (V4H) || defined (V4H2)
e_impdrv_errorcode_t impdrv_osdep_enable_dsp_irq(
    const st_impdrv_device_handle_t     *const p_device_handle
)
{
    return g_ret_impdrv_osdep_enable_dsp_irq;
}

e_impdrv_errorcode_t impdrv_osdep_disable_dsp_irq(
    const st_impdrv_device_handle_t     *const p_device_handle
)
{
    return g_ret_impdrv_osdep_disable_dsp_irq;
}

e_impdrv_errorcode_t impdrv_osdep_register_dsp_irq(
    const st_impdrv_device_handle_t     *const p_device_handle,
    uint32_t                            const core_num,
    const e_osal_interrupt_priority_t   irq_priority,
    const impdrv_ctrl_handle_t          osal_cb_args
)
{
    return g_ret_impdrv_osdep_register_dsp_irq;
}

e_impdrv_errorcode_t impdrv_osdep_unregister_dsp_irq(
    const st_impdrv_device_handle_t     *const p_device_handle,
    const uint32_t                      core_num
)
{
    return g_ret_impdrv_osdep_unregister_dsp_irq;
}

e_impdrv_errorcode_t impdrv_osdep_pow_on_dsp(
    st_impdrv_device_handle_t           *const p_device_handle
)
{
    return g_ret_impdrv_osdep_pow_on_dsp;
}

e_impdrv_errorcode_t impdrv_osdep_pow_off_dsp(
    st_impdrv_device_handle_t           *const p_device_handle
)
{
    return g_ret_impdrv_osdep_pow_off_dsp;
}

e_impdrv_errorcode_t impdrv_osdep_pow_on_imp_dsp(
    st_impdrv_device_handle_t   *const p_device_handle,
    const st_impdrv_core_info_t core_info,
    const uint32_t              sub_dev_num
)
{
    return g_ret_impdrv_osdep_pow_on_imp_dsp;
}

e_impdrv_errorcode_t impdrv_osdep_reset_core_dsp(
    const st_impdrv_device_handle_t     *const p_device_handle,
    const bool    reset
)
{
    return g_ret_impdrv_osdep_reset_core_dsp;
}

e_impdrv_errorcode_t impdrv_osdep_dev_open_dsp(
    st_impdrv_device_handle_t   *const p_device_handle,
    const st_impdrv_core_info_t core_info,
    const uint32_t              sub_dev_num
)
{
    return g_ret_impdrv_osdep_dev_open_dsp;
}
#endif
