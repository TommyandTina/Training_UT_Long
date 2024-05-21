#include "ut_osal_stub.h"

#if defined(RCAR_V3H1) ||( RCAR_V3M2) 
e_impdrv_errorcode_t impdrv_dmasctl_pm_set_policy(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    const e_impdrv_pm_policy_t      policy
)
{
   return 0;
}

e_impdrv_errorcode_t impdrv_dmasctl_pm_get_policy(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    e_impdrv_pm_policy_t            *const p_policy
)
{
   return 0;
}

e_impdrv_errorcode_t impdrv_dmasctl_set_mem_init(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const e_impdrv_param_t  enable
)
{
   return 0;
}

e_impdrv_errorcode_t impdrv_dmasctl_resume_exe(
    st_impdrv_corectl_t *const p_corectl,
    const uint32_t      core_num
)
{
   return 0;
}

e_impdrv_errorcode_t impdrv_dmasctl_execute(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num,
    const p_impdrv_cbfunc_t     callback_func,
    void                        *const p_callback_args
)
{
   return 0;
}

e_impdrv_errorcode_t impdrv_dmasctl_set_irq_mask(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const bool              irq_mask[IMPDRV_IRQMASK_MAX]
)
{
   return 0;
}

e_impdrv_errorcode_t impdrv_dmasctl_set_core_map(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const uint8_t           syncc_val[IMPDRV_COREMAP_MAXID]
)
{
   return 0;
}

bool impdrv_dmasctl_is_sub_thread(
    const uint32_t              core_num
)
{
   return 0;
}

uint32_t impdrv_dmasctl_osal_core_num(
    const uint32_t              core_num
)
{
   return 0;
}

uint32_t impdrv_dmactl_osal_core_num(
    const uint32_t              core_num
)
{
   return 0;
}

e_impdrv_errorcode_t impdrv_dmasctl_read_reg(
    const st_impdrv_device_handle_t *const p_device_handle,
    const uint32_t                  offset,
    uint32_t                        *p_val,
    const bool                      is_sub_thread
)
{
   return 0;
}

e_impdrv_errorcode_t impdrv_dmasctl_write_reg(
    const st_impdrv_dma_write_info_t *const p_write_info
)
{
   return 0;
}

e_impdrv_errorcode_t impdrv_dmasctl_set_cl(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const uint32_t          claddr_phys
)
{
   return 0;
}

uint32_t impdrv_dmasctl_get_inten_val(
    const bool      irq_mask[IMPDRV_IRQMASK_MAX]
)
{
   return 0;
}

e_impdrv_errorcode_t impdrv_dmasctl_int_handler(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
)
{
   return 0;
}

e_impdrv_errorcode_t impdrv_dmasctl_int_cb(
    const st_impdrv_corectl_t   *const p_corectl,
    const uint32_t              core_num,
    const e_impdrv_cb_ret_t     cb_ercd[IMPDRV_CB_RET_MAX],
    const uint32_t              cb_ercd_num,
    const bool                  is_check_intclear,
    const uint32_t              dtl_stat_val
)
{
   return 0;
}

e_impdrv_errorcode_t impdrv_dmasctl_set_syncc_config(
    st_impdrv_corectl_t         *const p_corectl,
    const bool                  is_sub_thread
)
{
   return 0;
}

e_impdrv_errorcode_t impdrv_dmasctl_soft_reset(
    st_impdrv_corectl_t         *const p_corectl,
    const bool                  is_sub_thread
)
{
   return 0;
}

e_impdrv_errorcode_t impdrv_dmasctl_check_core_busy(
    const st_impdrv_corectl_t   *const p_corectl,
    const bool                  is_sub_thread
)
{
   return 0;
}

e_impdrv_errorcode_t impdrv_dmasctl_conf_reg_check(
    const st_impdrv_coretypectl_t   *const p_coretypectl,
    const uint32_t                  core_num,
    const st_impdrv_chk_param_t     *const p_chk_param,
    const uint32_t                  param_num
)
{
   return 0;
}

e_impdrv_errorcode_t impdrv_dmasctl_check_inten(
    const uint32_t          stat_val,
    uint32_t                *const p_mask_val,
    e_impdrv_cb_ret_t       cb_ercd[IMPDRV_CB_RET_MAX],
    uint32_t                *const p_cb_ercd_num,
    bool                    *const p_is_check_intclear,
    e_impdrv_state_t        *const p_state
)
{
   return 0;
}

bool impdrv_dmasctl_is_valid_core(
    const uint32_t              core_num
)
{
   return 0;
}

e_impdrv_errorcode_t impdrv_dmasctl_check_state(
    const st_impdrv_corectl_t   *const p_corectl,
    const uint32_t              core_num,
    const e_impdrv_state_t      state
)
{
   return 0;
}

e_impdrv_errorcode_t impdrv_dmasctl_set_state(
    st_impdrv_corectl_t         *const p_corectl,
    const e_impdrv_state_t      state
)
{
   return 0;
}

e_impdrv_errorcode_t impdrv_dmasctl_init_start(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
   return 0;
}

e_impdrv_errorcode_t impdrv_dmasctl_init_end(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
   return 0;
}

e_impdrv_errorcode_t impdrv_dmasctl_start(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
   return 0;
}

e_impdrv_errorcode_t impdrv_dmasctl_stop(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
   return 0;
}

e_impdrv_errorcode_t impdrv_dmasctl_attr_init(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
)
{
   return 0;
}

e_impdrv_errorcode_t impdrv_dmasctl_init_core(
    st_impdrv_corectl_t   *const p_corectl
)
{
   return 0;
}
#endif

#if defined(RCAR_V3M2)

e_impdrv_errorcode_t impdrv_impsctl_pm_set_policy(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    const e_impdrv_pm_policy_t      policy
)
{
   return 0;
}
e_impdrv_errorcode_t impdrv_impsctl_pm_get_policy(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    e_impdrv_pm_policy_t            *const p_policy
)
{
   return 0;
}
e_impdrv_errorcode_t impdrv_impsctl_attr_init(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
)
{
   return 0;
}
e_impdrv_errorcode_t impdrv_impsctl_stop(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
   return 0;
}
e_impdrv_errorcode_t impdrv_impsctl_start(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
   return 0;
}
e_impdrv_errorcode_t impdrv_impsctl_init_end(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
   return 0;
}
e_impdrv_errorcode_t impdrv_impsctl_init_start(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
   return 0;
}
e_impdrv_errorcode_t impdrv_impsctl_set_state(
    st_impdrv_corectl_t         *const p_corectl,
    const e_impdrv_state_t      state
)
{
   return 0;
}
e_impdrv_errorcode_t impdrv_impsctl_check_state(
    const st_impdrv_corectl_t   *const p_corectl,
    const uint32_t              core_num,
    const e_impdrv_state_t      state
)
{
   return 0;
}
bool impdrv_impsctl_is_valid_core(
    const uint32_t              core_num
)
{
   return 0;
}
e_impdrv_errorcode_t impdrv_impsctl_set_irq_mask(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const bool              irq_mask[IMPDRV_IRQMASK_MAX]
)
{
   return 0;
}

uint32_t impdrv_impsctl_get_inten_val(
    const bool      irq_mask[IMPDRV_IRQMASK_MAX]
)
{
   return 0;
}
e_impdrv_errorcode_t impdrv_impsctl_set_cl(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const uint32_t          claddr_phys
)
{
   return 0;
}
e_impdrv_errorcode_t impdrv_impsctl_set_core_map(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const uint8_t           syncc_val[IMPDRV_COREMAP_MAXID]
)
{
   return 0;
}

e_impdrv_errorcode_t impdrv_impsctl_init_core(
    const st_impdrv_corectl_t   *const p_corectl
)
{
   return 0;
}
e_impdrv_errorcode_t impdrv_impsctl_execute(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num,
    const p_impdrv_cbfunc_t     callback_func,
    void                        *const p_callback_args
)
{
   return 0;
}
e_impdrv_errorcode_t impdrv_impsctl_resume_exe(
    st_impdrv_corectl_t *const p_corectl,
    const uint32_t      core_num
)
{
   return 0;
}
e_impdrv_errorcode_t impdrv_impsctl_set_mem_init(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const e_impdrv_param_t  enable
)
{
   return 0;
}

e_impdrv_errorcode_t impdrv_impsctl_int_main_func(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
)
{
   return 0;
}
e_impdrv_errorcode_t impdrv_impsctl_bus_if_check(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    const st_impdrv_chk_resource_t  *const p_chk_resource
)
{
   return 0;
}

e_impdrv_errorcode_t impdrv_impsctl_check_inten(
    const uint32_t          stat_val,
    uint32_t                *const p_mask_val,
    e_impdrv_cb_ret_t       cb_ercd[IMPDRV_CB_RET_MAX],
    uint32_t                *const p_cb_ercd_num,
    bool                    *const p_is_check_intclear,
    e_impdrv_state_t        *const p_state
)
{
   return 0;
}
e_impdrv_errorcode_t impdrv_impsctl_conf_reg_check(
    const st_impdrv_coretypectl_t   *const p_coretypectl,
    const uint32_t                  core_num,
    const st_impdrv_chk_param_t     *const p_chk_param,
    const uint32_t                  param_num
)
{
   return 0;
}

e_impdrv_errorcode_t impdrv_impsctl_check_core_busy(
    const st_impdrv_corectl_t   *const p_corectl
)
{
   return 0;
}

e_impdrv_errorcode_t impdrv_impsctl_soft_reset(
    const st_impdrv_corectl_t   *const p_corectl
)
{
   return 0;
}

e_impdrv_errorcode_t impdrv_impsctl_set_syncc_config(
    const st_impdrv_corectl_t   *const p_corectl
)
{
   return 0;
}

e_impdrv_errorcode_t impdrv_impsctl_int_safety_func(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
)
{
   return 0;
}

e_impdrv_errorcode_t impdrv_impsctl_dma_transfer(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    const st_impdrv_chk_resource_t  *const p_chk_resource,
    const uintptr_t                 physical_adrs,
    const uint32_t                  transfer_size
)
{
   return 0;
}

e_impdrv_errorcode_t impdrv_impsctl_int_cb(
    const st_impdrv_corectl_t   *const p_corectl,
    const uint32_t              core_num,
    const e_impdrv_cb_ret_t     cb_ercd[IMPDRV_CB_RET_MAX],
    const uint32_t              cb_ercd_num,
    const bool                  is_check_intclear
)
{
   return 0;
}

e_impdrv_errorcode_t impdrv_impsctl_int_handler(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
)
{
   return 0;
}

#endif

void impdrv_ut_memory_data_set(
    uint32_t                    *const p_buffer,
    const uint32_t              data,
    const size_t                size
)
{
    uint32_t                count;
    uint32_t                *p_pointer;

    p_pointer = p_buffer;
    count = (uint32_t)(size / sizeof(uint32_t));
    for (; 0U < count; count--)
    {
        *p_pointer = data;
        p_pointer++;
    }
}

e_osal_return_t R_OSAL_IoGetNumOfDevices(const char* deviceType,size_t * const p_numOfDevices)
{
    s_R_OSAL_IoGetNumOfDevices *IO = &g_R_OSAL_IoGetNumOfDevices;
    e_osal_return_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    if (p_numOfDevices != NULL)
    {
        *p_numOfDevices = IO->p_numOfDevices_out;
    }
    IO->ut_index++;
    return ReturnValue;
}

e_osal_return_t R_OSAL_IoGetDeviceList(const char* device_type_name, char* p_buffer, size_t buffer_size, size_t* const p_num_of_byte)
{
    s_R_OSAL_IoGetDeviceList *IO = &g_R_OSAL_IoGetDeviceList;
    e_osal_return_t ReturnValue;
    ReturnValue = IO->ReturnValue;
    if (p_num_of_byte != NULL)
    {
        *p_num_of_byte = IO->p_num_of_byte_out;
    }
    if (p_buffer != NULL)
    {
        
    }
    if (p_num_of_byte != NULL)
    {
        *p_num_of_byte = IO->p_num_of_byte_out;
    }
    IO->ut_index++;
    return ReturnValue;
}

e_osal_return_t R_OSAL_IoDeviceOpen(const char* id, osal_device_handle_t* const p_handle)
{
    s_R_OSAL_IoDeviceOpen *IO = &g_R_OSAL_IoDeviceOpen;
    e_osal_return_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    if (p_handle != NULL)
    {
       if (IO->p_handle_out[IO->ut_index] == VALID_HANDLE)
        {
            *p_handle = &g_uts_handle;
        }
        else if (IO->p_handle_out[IO->ut_index] == INVALID_HANDLE)
        {
            *p_handle = NULL;
        }
    }
    IO->ut_index++;
    return ReturnValue;
}

e_osal_return_t R_OSAL_IoDeviceClose(osal_device_handle_t handle)
{
    s_R_OSAL_IoDeviceClose *IO = &g_R_OSAL_IoDeviceClose;
    e_osal_return_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

e_osal_return_t R_OSAL_PmSetPolicy(osal_device_handle_t handle,e_osal_pm_policy_t policy,bool applyImmediate)
{
    s_R_OSAL_PmSetPolicy *IO = &g_R_OSAL_PmSetPolicy;
    e_osal_return_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

e_osal_return_t R_OSAL_InterruptGetNumOfIrqChannels(osal_device_handle_t handle,size_t*const p_numOfChannels)
{
    s_R_OSAL_InterruptGetNumOfIrqChannels *IO = &g_R_OSAL_InterruptGetNumOfIrqChannels;
    e_osal_return_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    if (p_numOfChannels != NULL)
    {
        memcpy(p_numOfChannels, &IO->p_numOfChannels_out, sizeof(IO->p_numOfChannels_out));
    }
    IO->ut_index++;
    return ReturnValue;
}

e_osal_return_t R_OSAL_InterruptUnregisterIsr(osal_device_handle_t handle,uint64_t irq_channel,p_osal_isr_func_t irqHandlerFunction)
{
    s_R_OSAL_InterruptUnregisterIsr *IO = &g_R_OSAL_InterruptUnregisterIsr;
    e_osal_return_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

e_osal_return_t R_OSAL_InterruptDisableIsr(osal_device_handle_t handle,uint64_t irq_channel)
{
    s_R_OSAL_InterruptDisableIsr *IO = &g_R_OSAL_InterruptDisableIsr;
    e_osal_return_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

e_osal_return_t R_OSAL_ThsyncMutexLockForTimePeriod(osal_mutex_handle_t handle,osal_milli_sec_t time_period)
{
    s_R_OSAL_ThsyncMutexLockForTimePeriod *IO = &g_R_OSAL_ThsyncMutexLockForTimePeriod;
    e_osal_return_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

e_osal_return_t R_OSAL_ThsyncMutexCreate(osal_mutex_id_t mutex_id,osal_mutex_handle_t* const p_handle)
{
    s_R_OSAL_ThsyncMutexCreate *IO = &g_R_OSAL_ThsyncMutexCreate;
    e_osal_return_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    if (OSAL_RETURN_OK == ReturnValue)
    {
        if (NULL != p_handle)
        {
            if (IO->p_handle_out == VALID_HANDLE)
            {
                *p_handle = &g_p_handle;
            }
            else
            {
                *p_handle = NULL;
            }
        }
    }
    IO->ut_index++;
    return ReturnValue;
}

e_osal_return_t R_OSAL_ThsyncMutexDestroy(osal_mutex_handle_t handle)
{
    s_R_OSAL_ThsyncMutexDestroy *IO = &g_R_OSAL_ThsyncMutexDestroy;
    e_osal_return_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

e_osal_return_t R_OSAL_ThsyncMutexUnlock(osal_mutex_handle_t handle)
{
    s_R_OSAL_ThsyncMutexUnlock *IO = &g_R_OSAL_ThsyncMutexUnlock;
    e_osal_return_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

e_osal_return_t R_OSAL_MqCreate(const st_osal_mq_config_t *const p_config, osal_mq_id_t mq_Id, osal_mq_handle_t *const p_handle)
{
    s_R_OSAL_MqCreate *IO = &g_R_OSAL_MqCreate;
    e_osal_return_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    if (OSAL_RETURN_OK == ReturnValue)
    {
        if (p_handle != NULL)
        {
            *p_handle = IO->p_handle_out[IO->ut_index];
            
        }
    }
    IO->ut_index++;
    return ReturnValue;
}

e_osal_return_t R_OSAL_MqReset(osal_mq_handle_t handle)
{
    s_R_OSAL_MqReset *IO = &g_R_OSAL_MqReset;
    e_osal_return_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

e_osal_return_t R_OSAL_MqDelete(osal_mq_handle_t handle)
{
    s_R_OSAL_MqDelete *IO = &g_R_OSAL_MqDelete;
    e_osal_return_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

e_osal_return_t R_OSAL_MqSendForTimePeriod(osal_mq_handle_t handle, osal_milli_sec_t time_period, const void * p_buffer, size_t buffer_size)
{
    s_R_OSAL_MqSendForTimePeriod *IO = &g_R_OSAL_MqSendForTimePeriod;
    e_osal_return_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

e_osal_return_t R_OSAL_MqReceiveForTimePeriod(osal_mq_handle_t handle, osal_milli_sec_t time_period, void *p_buffer, size_t buffer_size)
{
    s_R_OSAL_MqReceiveForTimePeriod *IO = &g_R_OSAL_MqReceiveForTimePeriod;
    e_osal_return_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
     uint32_t *buffer;
    buffer = (uint32_t *)p_buffer;
    if (OSAL_RETURN_OK == ReturnValue)
    {   
        if (NULL != buffer)
        {   
            *buffer = IO->receive_buffer_out[IO->ut_index];
        }
    }
    IO->ut_index++;
    return ReturnValue;
}

e_osal_return_t R_OSAL_ThreadCreate(const st_osal_thread_config_t* const p_config, osal_thread_id_t thread_id, osal_thread_handle_t* const p_handle)
{
    s_R_OSAL_ThreadCreate *IO = &g_R_OSAL_ThreadCreate;
    e_osal_return_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    if (OSAL_RETURN_OK == ReturnValue)
    {
        if (p_handle != NULL)
        {
            *p_handle = IO->task_handle_out;
        }
    }
    IO->ut_index++;
    return ReturnValue;
}

e_osal_return_t R_OSAL_ThreadJoin(osal_thread_handle_t handle, int64_t *p_return_value)
{
    s_R_OSAL_ThreadJoin *IO = &g_R_OSAL_ThreadJoin;
    e_osal_return_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

e_osal_return_t R_OSAL_InterruptRegisterIsr(osal_device_handle_t handle, uint64_t irq_channel, e_osal_interrupt_priority_t priority_value, p_osal_isr_func_t irqHandlerFunction, void* p_irq_arg)
{
    s_R_OSAL_InterruptRegisterIsr *IO = &g_R_OSAL_InterruptRegisterIsr;
    e_osal_return_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

e_osal_return_t R_OSAL_InterruptEnableIsr(osal_device_handle_t handle, uint64_t irq_channel)
{
    s_R_OSAL_InterruptEnableIsr *IO = &g_R_OSAL_InterruptEnableIsr;
    e_osal_return_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

e_osal_return_t R_OSAL_PmGetState(osal_device_handle_t handle, e_osal_pm_state_t *p_state)
{
    s_R_OSAL_PmGetState *IO = &g_R_OSAL_PmGetState;
    e_osal_return_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    if (OSAL_RETURN_OK == ReturnValue)
    {
        *p_state = IO->p_state_out;
    }
    IO->ut_index++;
    return ReturnValue;
}

e_osal_return_t R_OSAL_PmGetPolicy(osal_device_handle_t handle, e_osal_pm_policy_t* const p_policy)
{
    s_R_OSAL_PmGetPolicy *IO = &g_R_OSAL_PmGetPolicy;
    e_osal_return_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    if (OSAL_RETURN_OK == ReturnValue)
    {
        *p_policy = IO->p_policy_out;
    }
    IO->ut_index++;
    return ReturnValue;
}

e_osal_return_t R_OSAL_IoWrite32(osal_device_handle_t handle, uintptr_t offset, uint32_t data)
{
    s_R_OSAL_IoWrite32 *IO = &g_R_OSAL_IoWrite32;
    e_osal_return_t ReturnValue;

    if (IO->ut_index >= UT_R_OSAL_IoWrite32_MAX_SIZE)
    {
        IO->ut_index = 0;
        printf("[Warning] Exceed array size in %s -> %s function\n", __FILE__, __func__);
    }

    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

e_osal_return_t R_OSAL_IoRead32(osal_device_handle_t handle, uintptr_t offset, uint32_t * const lRegData)
{
    s_R_OSAL_IoRead32 *IO = &g_R_OSAL_IoRead32;
    e_osal_return_t ReturnValue;

    if (IO->ut_index >= UT_R_OSAL_IoRead32_MAX_SIZE)
    {
        IO->ut_index = 0;
        *lRegData = 0;
        ReturnValue = OSAL_RETURN_OK;
        printf("[Warning] Exceed array size in %s -> %s function\n", __FILE__, __func__);
    }
    else
    {
        ReturnValue = IO->ReturnValue[IO->ut_index];
        if (OSAL_RETURN_OK == ReturnValue)
        {
            if (lRegData != NULL)
            {
                *lRegData = IO->lRegData_out[IO->ut_index];
                
            }
        }
        IO->ut_index++;
    }
    return ReturnValue;
}

e_osal_return_t R_OSAL_PmSetRequiredState(osal_device_handle_t handle, e_osal_pm_required_state_t state, bool applyImmediate)
{   
    s_R_OSAL_PmSetRequiredState *IO = &g_R_OSAL_PmSetRequiredState;
    e_osal_return_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}
e_osal_return_t R_OSAL_PmGetRequiredState(osal_device_handle_t handle, e_osal_pm_required_state_t* const p_state)
{   
    s_R_OSAL_PmGetRequiredState *IO = &g_R_OSAL_PmGetRequiredState;
    e_osal_return_t ReturnValue;
    *p_state = IO->p_state_out[IO->ut_index];
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

e_osal_return_t R_OSAL_PmSetResetState(osal_device_handle_t handle, e_osal_pm_reset_state_t state)
{   
    s_R_OSAL_PmSetResetState *IO = &g_R_OSAL_PmSetResetState;
    e_osal_return_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

e_osal_return_t R_OSAL_ThsyncMemoryBarrier(void)
{
    s_R_OSAL_ThsyncMemoryBarrier *IO = &g_R_OSAL_ThsyncMemoryBarrier;
    e_osal_return_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

e_osal_return_t R_OSAL_MmngrInvalidate(osal_memory_buffer_handle_t handle, uintptr_t offset, size_t size)
{
    s_R_OSAL_MmngrInvalidate *IO = &g_R_OSAL_MmngrInvalidate;
    e_osal_return_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}


e_osal_return_t R_OSAL_MmngrFlush(osal_memory_buffer_handle_t handle, size_t offset, size_t size)
{
    s_R_OSAL_MmngrFlush *IO = &g_R_OSAL_MmngrFlush;
    e_osal_return_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}


e_osal_return_t R_OSAL_MmngrGetHwAddr(osal_memory_buffer_handle_t handle, osal_axi_bus_id_t axi_id, uintptr_t *p_physical_adrs)
{
    s_R_OSAL_MmngrGetHwAddr *IO = &g_R_OSAL_MmngrGetHwAddr;
    e_osal_return_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    
    if (OSAL_RETURN_OK == ReturnValue)
    {

        if (p_physical_adrs != NULL)

        {
            *p_physical_adrs = IO->p_physical_adrs_out[IO->ut_index];
        }
        
    }
    IO->ut_index++;
    return ReturnValue;
}


e_osal_return_t R_OSAL_MmngrGetCpuPtr(osal_memory_buffer_handle_t handle, void **p_logical_adrs)
{
    s_R_OSAL_MmngrGetCpuPtr *IO = &g_R_OSAL_MmngrGetCpuPtr;
    e_osal_return_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    uint8_t *p_work;
    uint32_t count,size,index;
    int i;
    const struct {
        uint32_t                pattern;
        size_t                  size;
    } s_test_pattern_tbl[] = {
        { 0x55555555U, (sizeof(uint32_t) * 8U) },
        { 0xAAAAAAAAU, (sizeof(uint32_t) * 8U) },
        { 0x33333333U, (sizeof(uint32_t) * 8U) },
        { 0xCCCCCCCCU, (sizeof(uint32_t) * 8U) },
        { 0x0F0F0F0FU, (sizeof(uint32_t) * 8U) },
        { 0xF0F0F0F0U, (sizeof(uint32_t) * 8U) },
        { 0xFFFFFFFFU, (sizeof(uint32_t) * 8U) },
        { 0x00000000U, (sizeof(uint32_t) * 8U) }
    };

    if (OSAL_RETURN_OK == ReturnValue)
    {

        if (p_logical_adrs != NULL)

        {
            if (0 != IO->p_logical_adrs_out[IO->ut_index])
            {
                p_logical_adrs = &g_p_logical_adrs;
            }

            else
            {
                *p_logical_adrs = NULL;
            }

            count = (sizeof(s_test_pattern_tbl) / sizeof(s_test_pattern_tbl[0]));
            index = 0;
            p_work = (uint8_t*)g_p_logical_adrs;
            p_work += 256;
            
            while (0U < count)
            {
                impdrv_ut_memory_data_set(
                    (uint32_t*)p_work,
                    s_test_pattern_tbl[index].pattern,
                    s_test_pattern_tbl[index].size
                );
                p_work += s_test_pattern_tbl[index].size;
                index++;
                count--;
            }
        }

    }
    IO->ut_index++;
    return ReturnValue;
}


e_osal_return_t R_OSAL_MmngrGetSize(osal_memory_buffer_handle_t handle, size_t *p_size)
{
    s_R_OSAL_MmngrGetSize *IO = &g_R_OSAL_MmngrGetSize;
    e_osal_return_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    if (OSAL_RETURN_OK == ReturnValue)
    {
        if (p_size != NULL)
        {
            *p_size = IO->p_size_out[IO->ut_index];
        }
    }
    IO->ut_index++;    
    return ReturnValue;
}

e_osal_return_t R_OSAL_PmWaitForState(osal_device_handle_t handle, e_osal_pm_state_t state,
                                            osal_milli_sec_t time_period)
{
    s_R_OSAL_PmWaitForState *IO = &g_R_OSAL_PmWaitForState;
    e_osal_return_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

e_osal_return_t R_OSAL_PmGetResetState(osal_device_handle_t handle, e_osal_pm_reset_state_t* const p_state)
{
    s_R_OSAL_PmGetResetState *IO = &g_R_OSAL_PmGetResetState;
    e_osal_return_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    *p_state = IO->p_state_out[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

const st_impdrv_corectl_func_t *ut_stub_impdrv_cmnctl_get_corectl_func(const e_impdrv_core_type_t core_type)
{
    s_impdrv_cmnctl_get_corectl_func *IO = &g_impdrv_cmnctl_get_corectl_func;
    st_impdrv_corectl_func_t *ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

const st_impdrv_corectl_func_t *impdrv_cmnctl_get_corectl_func_stub(const e_impdrv_core_type_t core_type)
{
    const st_impdrv_corectl_func_t *ret_func = NULL;
    if ( NULL != g_hook_impdrv_cmnctl_get_corectl_func)
    {
        ret_func = g_hook_impdrv_cmnctl_get_corectl_func(core_type);
    }
    else
    {
        ret_func = impdrv_cmnctl_get_corectl_func(core_type);
    }
    return ret_func;
}

e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_get_init_param(
    const st_impdrv_commonctl_t *const p_commonctl,
    e_osal_interrupt_priority_t *const p_irq_priority,
    impdrv_ctrl_handle_t        *const p_osal_cb_args
)
{
    s_impdrv_cmnctl_get_init_param *IO = &g_impdrv_cmnctl_get_init_param;
    e_impdrv_errorcode_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

e_impdrv_errorcode_t impdrv_cmnctl_get_init_param_stub(
    const st_impdrv_commonctl_t *const p_commonctl,
    e_osal_interrupt_priority_t *const p_irq_priority,
    impdrv_ctrl_handle_t        *const p_osal_cb_args
)
{
    e_impdrv_errorcode_t ret_func = 0;
    if ( NULL != g_hook_impdrv_cmnctl_get_init_param)
    {
        ret_func = g_hook_impdrv_cmnctl_get_init_param(p_commonctl, p_irq_priority, p_osal_cb_args);
    }
    else
    {
        ret_func = impdrv_cmnctl_get_init_param(p_commonctl, p_irq_priority, p_osal_cb_args);
    }
    return ret_func;
}

p_impdrv_dspctl_dsp_start_t ut_stub_impdrv_cmnctl_get_dsp_func(void)
{
    s_impdrv_cmnctl_get_dsp_func *IO = &g_impdrv_cmnctl_get_dsp_func;
    p_impdrv_dspctl_dsp_start_t *ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

p_impdrv_dspctl_dsp_start_t impdrv_cmnctl_get_dsp_func_stub(void)
{
    if ( NULL != g_hook_impdrv_cmnctl_get_dsp_func)
    {
        return g_hook_impdrv_cmnctl_get_dsp_func();
    }
    else
    {
        return impdrv_cmnctl_get_dsp_func();
    }

}

e_impdrv_errorcode_t ut_stub_impdrv_genctl_chk_state_init(
    const st_impdrv_ctl_t               *const p_impdrv_ctl,
    const st_impdrv_corectl_func_t      *p_funcs[IMPDRV_CORE_TYPE_MAX],
    st_impdrv_core_info_t               init_core_info[IMPDRV_INNER_FIXED_VALUE],
    uint32_t                            *const p_init_core_num
)
{
    s_impdrv_genctl_chk_state_init *IO = &g_impdrv_genctl_chk_state_init;
    e_impdrv_errorcode_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    *p_init_core_num = IO->p_init_core_num_out[IO->ut_index];
    p_funcs[0] = &g_p_uts_p_handle;
    // p_funcs[*p_init_core_num]->p_impdrvCorectlQuit = 0x00;
    memset(init_core_info, 0, 22*sizeof(st_impdrv_core_info_t));
    IO->ut_index++;
    return ReturnValue;
}

e_impdrv_errorcode_t impdrv_genctl_chk_state_init_stub(
    const st_impdrv_ctl_t               *const p_impdrv_ctl,
    const st_impdrv_corectl_func_t      *p_funcs[IMPDRV_CORE_TYPE_MAX],
    st_impdrv_core_info_t               init_core_info[IMPDRV_INNER_FIXED_VALUE],
    uint32_t                            *const p_init_core_num
)
{
    e_impdrv_errorcode_t ret_func = 0;
    if ( NULL != g_hook_impdrv_genctl_chk_state_init)
    {
        ret_func = g_hook_impdrv_genctl_chk_state_init(p_impdrv_ctl, p_funcs, init_core_info, p_init_core_num);
    }
    else
    {
        ret_func = impdrv_genctl_chk_state_init(p_impdrv_ctl, p_funcs, init_core_info, p_init_core_num);
    }
    return ret_func;
}

const uint32_t ut_stub_impdrv_cnnctl_get_inten_val(const bool irq_mask[IMPDRV_IRQMASK_MAX])
{
    s_impdrv_cnnctl_get_inten_val *IO = &g_impdrv_cnnctl_get_inten_val;
    uint32_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

const uint32_t impdrv_cnnctl_get_inten_val_stub(const bool irq_mask[IMPDRV_IRQMASK_MAX])
{
    uint32_t inten_val = 0;

    if ( NULL != g_hook_impdrv_cnnctl_get_inten_val)
    {
        inten_val = g_hook_impdrv_cnnctl_get_inten_val(irq_mask);
    }
    else
    {
        inten_val = impdrv_cnnctl_get_inten_val(irq_mask);
    }
    return inten_val;
}

const uint32_t ut_stub_impdrv_pscctl_get_inten_val(const bool irq_mask[IMPDRV_IRQMASK_MAX])
{
    s_impdrv_pscctl_get_inten_val *IO = &g_impdrv_pscctl_get_inten_val;
    uint32_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

const uint32_t impdrv_pscctl_get_inten_val_stub(const bool irq_mask[IMPDRV_IRQMASK_MAX])
{
    uint32_t inten_val = 0;

    if ( NULL != g_hook_impdrv_pscctl_get_inten_val)
    {
        inten_val = g_hook_impdrv_pscctl_get_inten_val(irq_mask);
    }
    else
    {
        inten_val = impdrv_pscctl_get_inten_val(irq_mask);
    }
    return inten_val;
}

const uint32_t ut_stub_impdrv_ocvctl_get_inten_val(const bool irq_mask[IMPDRV_IRQMASK_MAX])
{
    s_impdrv_ocvctl_get_inten_val *IO = &g_impdrv_ocvctl_get_inten_val;
    uint32_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

const uint32_t impdrv_ocvctl_get_inten_val_stub(const bool irq_mask[IMPDRV_IRQMASK_MAX])
{
    uint32_t inten_val = 0;

    if ( NULL != g_hook_impdrv_ocvctl_get_inten_val)
    {
        inten_val = g_hook_impdrv_ocvctl_get_inten_val(irq_mask);
    }
    else
    {
        inten_val = impdrv_ocvctl_get_inten_val(irq_mask);
    }
    return inten_val;
}

const e_impdrv_errorcode_t ut_stub_impdrv_ocvctl_set_state( st_impdrv_corectl_t *const p_corectl,const e_impdrv_state_t  state)
{
    s_impdrv_ocvctl_set_state *IO = &g_impdrv_ocvctl_set_state;
    e_impdrv_errorcode_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

const e_impdrv_errorcode_t impdrv_ocvctl_set_state_stub(st_impdrv_corectl_t *const p_corectl,const e_impdrv_state_t  state)
{
    e_impdrv_errorcode_t ercd = IMPDRV_EC_NG_PARAM;
  
    if ( NULL != g_hook_impdrv_ocvctl_set_state)
    {
        ercd = g_hook_impdrv_ocvctl_set_state(p_corectl,state);
        
    }
    else
    {
        ercd = impdrv_ocvctl_set_state(p_corectl,state);
         
    }
    return ercd;
}


//#if defined(RCAR_V3H2)
const uint32_t ut_stub_impdrv_dmasctl_get_inten_val(const bool irq_mask[IMPDRV_IRQMASK_MAX])
{
    s_impdrv_dmasctl_get_inten_val *IO = &g_impdrv_dmasctl_get_inten_val;
    uint32_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

const uint32_t impdrv_dmasctl_get_inten_val_stub(const bool irq_mask[IMPDRV_IRQMASK_MAX])
{
    uint32_t inten_val = 0;

    if ( NULL != g_hook_impdrv_dmasctl_get_inten_val)
    {
        inten_val = g_hook_impdrv_dmasctl_get_inten_val(irq_mask);
    }
    else
    {
        inten_val = impdrv_dmasctl_get_inten_val(irq_mask);
    }
    return inten_val;
}

const e_impdrv_errorcode_t ut_stub_impdrv_dmasctl_set_state( st_impdrv_corectl_t *const p_corectl,const e_impdrv_state_t  state)
{
    s_impdrv_dmasctl_set_state *IO = &g_impdrv_dmasctl_set_state;
    e_impdrv_errorcode_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

const e_impdrv_errorcode_t impdrv_dmasctl_set_state_stub(st_impdrv_corectl_t *const p_corectl,const e_impdrv_state_t  state)
{
    e_impdrv_errorcode_t ercd = IMPDRV_EC_NG_PARAM;
  
    if ( NULL != g_hook_impdrv_dmasctl_set_state)
    {
        ercd = g_hook_impdrv_dmasctl_set_state(p_corectl,state);
        
    }
    else
    {
        ercd = impdrv_dmasctl_set_state(p_corectl,state);
         
    }
    return ercd;
}

const uint32_t ut_stub_impdrv_dmasctl_osal_core_num(const uint32_t              core_num)
{
    s_impdrv_dmasctl_osal_core_num *IO = &g_impdrv_dmasctl_osal_core_num;
    uint32_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

const uint32_t impdrv_dmasctl_osal_core_num_stub(const uint32_t              core_num)
{
    uint32_t ercd = IMPDRV_EC_NG_PARAM;
  
    if ( NULL != g_hook_impdrv_dmasctl_osal_core_num)
    {
        ercd = g_hook_impdrv_dmasctl_osal_core_num(core_num);
        
    }
    else
    {
        ercd = impdrv_dmasctl_osal_core_num(core_num);
         
    }
    return ercd;
}

const uint32_t ut_stub_impdrv_dmactl_osal_core_num(const uint32_t              core_num)
{
    s_impdrv_dmactl_osal_core_num *IO = &g_impdrv_dmactl_osal_core_num;
    uint32_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

const uint32_t impdrv_dmactl_osal_core_num_stub(const uint32_t              core_num)
{
    uint32_t ercd = IMPDRV_EC_NG_PARAM;
  
    if ( NULL != g_hook_impdrv_dmactl_osal_core_num)
    {
        ercd = g_hook_impdrv_dmactl_osal_core_num(core_num);
        
    }
    else
    {
        ercd = impdrv_dmactl_osal_core_num(core_num);
         
    }
    return ercd;
}
//#endif

#if defined(RCAR_V3H2) || defined(RCAR_V3H1)
const uint32_t ut_stub_impdrv_impsctl_get_inten_val(const bool irq_mask[IMPDRV_IRQMASK_MAX])
{
    s_impdrv_impsctl_get_inten_val *IO = &g_impdrv_impsctl_get_inten_val;
    uint32_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

const uint32_t impdrv_impsctl_get_inten_val_stub(const bool irq_mask[IMPDRV_IRQMASK_MAX])
{
    uint32_t inten_val = 0;

    if ( NULL != g_hook_impdrv_impsctl_get_inten_val)
    {
        inten_val = g_hook_impdrv_impsctl_get_inten_val(irq_mask);
    }
    else
    {
        inten_val = impdrv_impsctl_get_inten_val(irq_mask);
    }
    return inten_val;
}


const e_impdrv_errorcode_t ut_stub_impdrv_impsctl_set_state( st_impdrv_corectl_t *const p_corectl,const e_impdrv_state_t  state)
{
    s_impdrv_impsctl_set_state *IO = &g_impdrv_impsctl_set_state;
    e_impdrv_errorcode_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

const e_impdrv_errorcode_t impdrv_impsctl_set_state_stub(st_impdrv_corectl_t *const p_corectl,const e_impdrv_state_t  state)
{
    e_impdrv_errorcode_t ercd = IMPDRV_EC_NG_PARAM;
  
    if ( NULL != g_hook_impdrv_impsctl_set_state)
    {
        ercd = g_hook_impdrv_impsctl_set_state(p_corectl,state);
        
    }
    else
    {
        ercd = impdrv_impsctl_set_state(p_corectl,state);
         
    }
    return ercd;
}
#endif


const e_impdrv_errorcode_t ut_stub_impdrv_cnnctl_set_state( st_impdrv_corectl_t *const p_corectl,const e_impdrv_state_t  state)
{
    s_impdrv_cnnctl_set_state *IO = &g_impdrv_cnnctl_set_state;
    e_impdrv_errorcode_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

const e_impdrv_errorcode_t impdrv_cnnctl_set_state_stub(st_impdrv_corectl_t *const p_corectl,const e_impdrv_state_t  state)
{
    e_impdrv_errorcode_t ercd = IMPDRV_EC_NG_PARAM;
  
    if ( NULL != g_hook_impdrv_cnnctl_set_state)
    {
        ercd = g_hook_impdrv_cnnctl_set_state(p_corectl,state);
        
    }
    else
    {
        ercd = impdrv_cnnctl_set_state(p_corectl,state);
         
    }
    return ercd;
}

const e_impdrv_errorcode_t ut_stub_impdrv_pscctl_set_state( st_impdrv_corectl_t *const p_corectl,const e_impdrv_state_t  state)
{
    s_impdrv_pscctl_set_state *IO = &g_impdrv_pscctl_set_state;
    e_impdrv_errorcode_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

const e_impdrv_errorcode_t impdrv_pscctl_set_state_stub(st_impdrv_corectl_t *const p_corectl,const e_impdrv_state_t  state)
{
    e_impdrv_errorcode_t ercd = IMPDRV_EC_NG_PARAM;
  
    if ( NULL != g_hook_impdrv_pscctl_set_state)
    {
        ercd = g_hook_impdrv_pscctl_set_state(p_corectl,state);
        
    }
    else
    {
        ercd = impdrv_pscctl_set_state(p_corectl,state);
         
    }
    return ercd;
}

const uint32_t ut_stub_impdrv_dmactl_get_inten_val(const bool irq_mask[IMPDRV_IRQMASK_MAX])
{
    s_impdrv_dmactl_get_inten_val *IO = &g_impdrv_dmactl_get_inten_val;
    uint32_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

const uint32_t impdrv_dmactl_get_inten_val_stub(const bool irq_mask[IMPDRV_IRQMASK_MAX])
{
    uint32_t inten_val = 0;

    if ( NULL != g_hook_impdrv_dmactl_get_inten_val)
    {
        inten_val = g_hook_impdrv_dmactl_get_inten_val(irq_mask);
    }
    else
    {
        inten_val = impdrv_dmactl_get_inten_val(irq_mask);
    }
    return inten_val;
}

const uint32_t ut_stub_impdrv_impctl_get_inten_val(const bool irq_mask[IMPDRV_IRQMASK_MAX])
{
    s_impdrv_impctl_get_inten_val *IO = &g_impdrv_impctl_get_inten_val;
    uint32_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

const uint32_t impdrv_impctl_get_inten_val_stub(const bool irq_mask[IMPDRV_IRQMASK_MAX])
{
    uint32_t inten_val = 0;

    if ( NULL != g_hook_impdrv_impctl_get_inten_val)
    {
        inten_val = g_hook_impdrv_impctl_get_inten_val(irq_mask);
    }
    else
    {
        inten_val = impdrv_impctl_get_inten_val(irq_mask);
    }
    return inten_val;
}

const e_impdrv_errorcode_t ut_stub_impdrv_impctl_set_state( st_impdrv_corectl_t *const p_corectl,const e_impdrv_state_t  state)
{
    s_impdrv_impctl_set_state *IO = &g_impdrv_impctl_set_state;
    e_impdrv_errorcode_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

const e_impdrv_errorcode_t ut_stub_impdrv_dspctl_set_state( st_impdrv_corectl_t *const p_corectl,const e_impdrv_state_t  state)
{
    s_impdrv_dspctl_set_state *IO = &g_impdrv_dspctl_set_state;
    e_impdrv_errorcode_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

const e_impdrv_errorcode_t impdrv_impctl_set_state_stub(st_impdrv_corectl_t *const p_corectl,const e_impdrv_state_t  state)
{
    e_impdrv_errorcode_t ercd = IMPDRV_EC_NG_PARAM;
    
    if ( NULL != g_hook_impdrv_impctl_set_state)
    {
        ercd = g_hook_impdrv_impctl_set_state(p_corectl,state);
  
    }
    else
    {
        ercd = impdrv_impctl_set_state(p_corectl,state);
         
    }
    return ercd;
}

const e_impdrv_errorcode_t impdrv_dspctl_set_state_stub(st_impdrv_corectl_t *const p_corectl,const e_impdrv_state_t  state)
{
    e_impdrv_errorcode_t ercd = IMPDRV_EC_NG_PARAM;
    
    if ( NULL != g_hook_impdrv_dspctl_set_state)
    {
        ercd = g_hook_impdrv_dspctl_set_state(p_corectl,state);
  
    }
    else
    {
        ercd = impdrv_dspctl_set_state(p_corectl,state);
         
    }
    return ercd;
}

const e_impdrv_errorcode_t ut_stub_impdrv_osdep_memory_data_set( uint32_t *const p_buffer,const uint32_t  data,const size_t size)
{
    s_impdrv_osdep_memory_data_set *IO = &g_impdrv_osdep_memory_data_set;
    e_impdrv_errorcode_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    //IO->ut_index++;
    
    return ReturnValue;
}

const e_impdrv_errorcode_t impdrv_osdep_memory_data_set_stub(uint32_t *const p_buffer,const uint32_t  data,const size_t size)
{
    e_impdrv_errorcode_t ercd = IMPDRV_EC_NG_PARAM;
    
    if ( NULL != g_hook_impdrv_osdep_memory_data_set)
    {
        ercd = g_hook_impdrv_osdep_memory_data_set(p_buffer,data,size);
  
    }
    else
    {
        ercd = impdrv_osdep_memory_data_set(p_buffer,data,size);
         
    }
    return ercd;
}
const e_impdrv_errorcode_t ut_stub_impdrv_dmactl_set_state( st_impdrv_corectl_t *const p_corectl,const e_impdrv_state_t  state)
{
    s_impdrv_dmactl_set_state *IO = &g_impdrv_dmactl_set_state;
    e_impdrv_errorcode_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

const e_impdrv_errorcode_t impdrv_dmactl_set_state_stub(st_impdrv_corectl_t *const p_corectl,const e_impdrv_state_t  state)
{
    e_impdrv_errorcode_t ercd = IMPDRV_EC_NG_PARAM;
    
    if ( NULL != g_hook_impdrv_dmactl_set_state)
    {
        ercd = g_hook_impdrv_dmactl_set_state(p_corectl,state);
  
    }
    else
    {
        ercd = impdrv_dmactl_set_state(p_corectl,state);
         
    }
    return ercd;
}

e_impdrv_errorcode_t ut_stub_impdrv_genctl_init_set_state(st_impdrv_ctl_t *const p_impdrv_ctl, const uint32_t use_core_num, const st_impdrv_core_info_t *const p_core_info, const st_impdrv_corectl_func_t **const p_funcs)
{
    s_impdrv_genctl_init_set_state *IO = &g_impdrv_genctl_init_set_state;
    e_impdrv_errorcode_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

e_impdrv_errorcode_t impdrv_genctl_init_set_state_stub(st_impdrv_ctl_t *const p_impdrv_ctl, const uint32_t use_core_num, const st_impdrv_core_info_t *const p_core_info, const st_impdrv_corectl_func_t **const p_funcs)
{
    e_impdrv_errorcode_t ercd = IMPDRV_EC_NG_PARAM;
    if ( NULL != g_hook_impdrv_genctl_init_set_state)
    {
        
        ercd = g_hook_impdrv_genctl_init_set_state(p_impdrv_ctl, use_core_num, p_core_info, p_funcs);
    }
    else
    {
        ercd = impdrv_genctl_init_set_state(p_impdrv_ctl, use_core_num, p_core_info, p_funcs);
    }
    return ercd;
}

e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_set_checksum(void)
{
    s_impdrv_cmnctl_set_checksum *IO = &g_impdrv_cmnctl_set_checksum;
    e_impdrv_errorcode_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    //IO->ut_index++;
    return ReturnValue;
}

e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_test_checksum(void)
{
    s_impdrv_cmnctl_test_checksum *IO = &g_impdrv_cmnctl_test_checksum;
    e_impdrv_errorcode_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    //IO->ut_index++;
    return ReturnValue;
}

e_impdrv_errorcode_t impdrv_cmnctl_set_checksum_stub(void)
{
    e_impdrv_errorcode_t ercd; 
    if ( NULL != g_hook_impdrv_cmnctl_set_checksum)
    {
        
        ercd = g_hook_impdrv_cmnctl_set_checksum();
        
    }
    else
    {
        ercd = impdrv_cmnctl_set_checksum();
    }
    return ercd;
}

e_impdrv_errorcode_t impdrv_cmnctl_test_checksum_stub(void)
{
    e_impdrv_errorcode_t ercd; 
    if ( NULL != g_hook_impdrv_cmnctl_test_checksum)
    {
        
        ercd = g_hook_impdrv_cmnctl_test_checksum();
        
    }
    else
    {
        ercd = impdrv_cmnctl_test_checksum();
    }
    return ercd;
}

e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_cnv_int_bit_core(const st_impdrv_inten_tbl_t *const p_s_inten_cnv_table, const uint32_t cnv_table_num, const uint32_t stat, st_impdrv_core_info_t *core_info, uint32_t *const p_use_core_num)
{
    s_impdrv_cmnctl_cnv_int_bit_core *IO = &g_impdrv_cmnctl_cnv_int_bit_core;
    e_impdrv_errorcode_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_cnv_int_core_bit(const st_impdrv_core_info_t *const p_core_info, const uint32_t use_core_num, const st_impdrv_inten_tbl_t const* p_inten_cnv_table, const uint32_t cnv_table_num, uint32_t *const p_val)
{
    s_impdrv_cmnctl_cnv_int_core_bit *IO = &g_impdrv_cmnctl_cnv_int_core_bit;
    e_impdrv_errorcode_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_get_group(e_impdrv_irq_group_t *const p_irq_group, const e_impdrv_instance_t instance_num)
{
    s_impdrv_cmnctl_get_group *IO = &g_impdrv_cmnctl_get_group;
    e_impdrv_errorcode_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    //IO->ut_index++;
    return ReturnValue;
}

e_impdrv_errorcode_t impdrv_cmnctl_cnv_int_core_bit_stub(const st_impdrv_core_info_t *const p_core_info, const uint32_t use_core_num, const st_impdrv_inten_tbl_t const* p_inten_cnv_table, const uint32_t cnv_table_num, uint32_t *const p_val)
{
    e_impdrv_errorcode_t ercd = IMPDRV_EC_NG_PARAM;
    if ( NULL != g_hook_impdrv_cmnctl_cnv_int_core_bit)
    {
        
        ercd = g_hook_impdrv_cmnctl_cnv_int_core_bit(p_core_info, use_core_num, p_inten_cnv_table, cnv_table_num, p_val);
    }
    else
    {
        ercd = impdrv_cmnctl_cnv_int_core_bit(p_core_info, use_core_num, p_inten_cnv_table, cnv_table_num, p_val);
    }
    return ercd;
}

e_impdrv_errorcode_t impdrv_cmnctl_get_group_stub(e_impdrv_irq_group_t *const p_irq_group, const e_impdrv_instance_t instance_num)
{
    e_impdrv_errorcode_t ercd = IMPDRV_EC_NG_PARAM;
    if ( NULL != g_hook_impdrv_cmnctl_get_group)
    {
        
        ercd = g_hook_impdrv_cmnctl_get_group(p_irq_group, instance_num);
    }
    else
    {
        ercd = impdrv_cmnctl_get_group(p_irq_group, instance_num);
    }
    return ercd;
}
e_impdrv_errorcode_t impdrv_cmnctl_cnv_int_bit_core_stub(const st_impdrv_inten_tbl_t *const p_s_inten_cnv_table, const uint32_t cnv_table_num, const uint32_t stat, st_impdrv_core_info_t *core_info, uint32_t *const p_use_core_num)
{
    e_impdrv_errorcode_t ercd = IMPDRV_EC_NG_PARAM;
    if ( NULL != g_hook_impdrv_cmnctl_cnv_int_bit_core)
    {
        
        ercd = g_hook_impdrv_cmnctl_cnv_int_bit_core(p_s_inten_cnv_table, cnv_table_num, stat, core_info, p_use_core_num);
    }
    else
    {
        ercd = impdrv_cmnctl_cnv_int_bit_core(p_s_inten_cnv_table, cnv_table_num, stat, core_info, p_use_core_num);
    }
    return ercd;
}

e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_reg_set_mem_protect(st_impdrv_commonctl_t *const p_commonctl, const e_impdrv_protect_mode_t protect_mode)
{
    s_impdrv_cmnctl_reg_set_mem_protect *IO = &g_impdrv_cmnctl_reg_set_mem_protect;
    e_impdrv_errorcode_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

e_impdrv_errorcode_t impdrv_cmnctl_reg_set_mem_protect_stub(st_impdrv_commonctl_t *const p_commonctl, const e_impdrv_protect_mode_t protect_mode)
{
    e_impdrv_errorcode_t ercd = IMPDRV_EC_OK;
    if ( NULL != g_hook_impdrv_cmnctl_reg_set_mem_protect)
    {
        
        ercd = g_hook_impdrv_cmnctl_reg_set_mem_protect(p_commonctl, protect_mode);
    }
    else
    {
        ercd = impdrv_cmnctl_reg_set_mem_protect(p_commonctl, protect_mode);
    }
    return ercd;
}

e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_reg_find_resource(const char *const p_device_id, uint32_t *const p_index, st_impdrv_hwrsc_def_t *const p_resource)
{
    s_impdrv_cmnctl_reg_find_resource *IO = &g_impdrv_cmnctl_reg_find_resource;
    e_impdrv_errorcode_t ReturnValue;
    *p_index = IO->p_index_out;
    p_resource->size = IO->p_resource_out.size;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

e_impdrv_errorcode_t impdrv_cmnctl_reg_find_resource_stub(const char *const p_device_id, uint32_t *const p_index, st_impdrv_hwrsc_def_t *const p_resource)
{
    e_impdrv_errorcode_t ercd = IMPDRV_EC_NG_PARAM;
    *p_index = 2;
    if ( NULL != g_hook_impdrv_cmnctl_reg_find_resource)
    {
        
        ercd = g_hook_impdrv_cmnctl_reg_find_resource(p_device_id, p_index, p_resource);
    }
    else
    {
        ercd = impdrv_cmnctl_reg_find_resource(p_device_id, p_index, p_resource);
    }
    return ercd;
}

e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_get_io_imptop(st_impdrv_device_handle_t *const p_handle)
{
    s_impdrv_cmnctl_get_io_imptop *IO = &g_impdrv_cmnctl_get_io_imptop;
    e_impdrv_errorcode_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
   // IO->ut_index++;
    return ReturnValue;
}


e_impdrv_errorcode_t impdrv_cmnctl_get_io_imptop_stub(st_impdrv_device_handle_t *const p_handle)
{
    e_impdrv_errorcode_t ercd = IMPDRV_EC_NG_PARAM;
    
    if ( NULL != g_hook_impdrv_cmnctl_get_io_imptop)
    {
        
        ercd = g_hook_impdrv_cmnctl_get_io_imptop(p_handle);
        *p_handle = device_io_imptop;
    }
    else
    {
        ercd = impdrv_cmnctl_get_io_imptop(p_handle);
    }
    return ercd;
}

e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_calc_checksum(const void* *const p_data,const size_t size,uint32_t *const p_checksum)
{
    s_impdrv_cmnctl_calc_checksum *IO = &g_impdrv_cmnctl_calc_checksum;
    e_impdrv_errorcode_t ReturnValue;
    *p_checksum = IO->p_checksum_out[IO->ut_index];
    ReturnValue = IO->ReturnValue[IO->ut_index];
    //IO->ut_index++;
    return ReturnValue;
}

e_impdrv_errorcode_t impdrv_cmnctl_calc_checksum_stub(const void* *const p_data,const size_t size,uint32_t *const p_checksum)
{
    e_impdrv_errorcode_t ercd = IMPDRV_EC_NG_ARGNULL;
    if ( NULL != g_hook_impdrv_cmnctl_calc_checksum)
    {
        
        ercd = g_hook_impdrv_cmnctl_calc_checksum( p_data,size,p_checksum);
        
    }
    else
    {
        ercd = impdrv_cmnctl_calc_checksum(p_data,size,p_checksum);
    }
    return ercd;
}

const e_impdrv_errorcode_t ut_stub_impdrv_osdep_pow_on_imp(st_impdrv_device_handle_t   *const p_device_handle,const st_impdrv_core_info_t core_info)
{
    s_impdrv_osdep_pow_on_imp *IO = &g_impdrv_osdep_pow_on_imp;
    e_impdrv_errorcode_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    //IO->ut_index++;
    return ReturnValue;
}
const e_impdrv_errorcode_t impdrv_osdep_pow_on_imp_stub(st_impdrv_device_handle_t   *const p_device_handle,const st_impdrv_core_info_t core_info)
{
    e_impdrv_errorcode_t ercd; 
    
    if ( NULL != g_hook_impdrv_osdep_pow_on_imp)
    {
        
        ercd = g_hook_impdrv_osdep_pow_on_imp(p_device_handle,core_info);
  
    }
    else
    {
        ercd = impdrv_osdep_pow_on_imp(p_device_handle,core_info);
         
    }
    return ercd;
}

const e_impdrv_errorcode_t ut_stub_impdrv_genctl_dsp_int_handler(st_impdrv_ctl_t *const p_impdrv_ctl, const uint64_t irq_channel, const e_impdrv_errorcode_t ercd, const st_impdrv_core_info_t core_info)
{
    s_impdrv_genctl_dsp_int_handler *IO = &g_impdrv_genctl_dsp_int_handler;
    e_impdrv_errorcode_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    //IO->ut_index++;
    return ReturnValue;
}

const e_impdrv_errorcode_t impdrv_genctl_dsp_int_handler_stub(st_impdrv_ctl_t *const p_impdrv_ctl, const uint64_t irq_channel, const e_impdrv_errorcode_t ercd, const st_impdrv_core_info_t core_info)
{
    e_impdrv_errorcode_t ercd_t; 
    
    if ( NULL != g_hook_impdrv_genctl_dsp_int_handler)
    {
        
        ercd_t = g_hook_impdrv_genctl_dsp_int_handler(p_impdrv_ctl, irq_channel, ercd, core_info);
  
    }
    else
    {
        (void)impdrv_genctl_dsp_int_handler(p_impdrv_ctl, irq_channel, ercd, core_info);
         
    }
    return ercd_t;
}

e_impdrv_errorcode_t impdrv_ut_impctl_set_state(
    st_impdrv_corectl_t         *const p_corectl,
    const e_impdrv_state_t      state
)

{
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_NG_PARAM;
    return ercd;
}
e_impdrv_errorcode_t impdrv_osdep_get_dsp_irq_channel_stub(
    uint64_t                     *const irq_channel
)
{
    e_impdrv_errorcode_t ercd = 0;

    if ( NULL != g_hook_impdrv_osdep_get_dsp_irq_channel)
    {
        ercd = g_hook_impdrv_osdep_get_dsp_irq_channel(irq_channel);
    }
    else
    {
        ercd = impdrv_osdep_get_dsp_irq_channel(irq_channel);
    }
    return ercd;

}
const e_impdrv_errorcode_t ut_stub_impdrv_osdep_get_dsp_irq_channel(
    uint64_t                     *const irq_channel
)
{
    s_impdrv_osdep_get_dsp_irq_channel *IO = &g_impdrv_osdep_get_dsp_irq_channel;
    e_impdrv_errorcode_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    IO->ut_index++;
    return ReturnValue;
}

const e_impdrv_errorcode_t ut_stub_impdrv_cnnctl_check_inten(
    const uint32_t          stat_val,
    uint32_t                *const p_mask_val,
    e_impdrv_cb_ret_t       cb_ercd[IMPDRV_CB_RET_MAX],
    uint32_t                *const p_cb_ercd_num,
    bool                    *const p_is_check_intclear,
    e_impdrv_state_t        *const p_state
)
{
    s_impdrv_cnnctl_check_inten *IO = &g_impdrv_cnnctl_check_inten;
    e_impdrv_errorcode_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    *p_state = IO->p_state_out[IO->ut_index];
    *p_mask_val = IO->p_mask_val_out[IO->ut_index];
    *p_cb_ercd_num = 1;
    IO->ut_index++;
    return ReturnValue;
}

const e_impdrv_errorcode_t impdrv_cnnctl_check_inten_stub(
    const uint32_t          stat_val,
    uint32_t                *const p_mask_val,
    e_impdrv_cb_ret_t       cb_ercd[IMPDRV_CB_RET_MAX],
    uint32_t                *const p_cb_ercd_num,
    bool                    *const p_is_check_intclear,
    e_impdrv_state_t        *const p_state
)
{
    e_impdrv_errorcode_t ercd_t; 
    
    if ( NULL != g_hook_impdrv_cnnctl_check_inten)
    {
        
        ercd_t = g_hook_impdrv_cnnctl_check_inten(stat_val, p_mask_val, cb_ercd, p_cb_ercd_num, p_is_check_intclear, p_state);
  
    }
    else
    {
        ercd_t = impdrv_cnnctl_check_inten(stat_val, p_mask_val, cb_ercd, p_cb_ercd_num, p_is_check_intclear, p_state);
         
    }
    return ercd_t;
}

e_impdrv_errorcode_t ut_stub_impdrv_cmnctl_judge_int(const st_impdrv_commonctl_t *const p_commonctl,
    const uint64_t              irq_channel,
    st_impdrv_core_info_t       core_info[IMPDRV_INTSTS_BITNUM],
    uint32_t                    *const p_use_core_num,
    bool                        is_fb_callback)
{
    s_impdrv_cmnctl_judge_int *IO = &g_impdrv_cmnctl_judge_int;
    e_impdrv_errorcode_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    core_info->core_type = IO->core_info_out.core_type;
    core_info->core_num = IO->core_info_out.core_num;  
    *p_use_core_num = IO->p_use_core_num_out;
    //IO->ut_index++;
    return ReturnValue;
}

e_impdrv_errorcode_t impdrv_cmnctl_judge_int_stub(const st_impdrv_commonctl_t *const p_commonctl,
    const uint64_t              irq_channel,
    st_impdrv_core_info_t       core_info[IMPDRV_INTSTS_BITNUM],
    uint32_t                    *const p_use_core_num,
    bool                        is_fb_callback)
{
    e_impdrv_errorcode_t ercd = IMPDRV_EC_NG_ARGNULL;
    if ( NULL != g_hook_impdrv_cmnctl_judge_int)
    {
        
        ercd = g_hook_impdrv_cmnctl_judge_int(p_commonctl, irq_channel, core_info, p_use_core_num, is_fb_callback);
    }
    else
    {
        ercd = impdrv_cmnctl_judge_int(p_commonctl, irq_channel, core_info, p_use_core_num, is_fb_callback);
    }
    return ercd;
}

uint32_t ut_stub_impdrv_udefctl_udef_crc(uint32_t st, uint32_t sz )
{
    s_impdrv_udefctl_udef_crc_val *IO = &g_impdrv_udefctl_udef_crc;
    uint32_t ReturnValue;
    ReturnValue = IO->ReturnValue[IO->ut_index];
    st = IO->st_out;
    sz = IO->sz_out;
    IO->ut_index++;
    return ReturnValue;
}
uint32_t impdrv_udefctl_udef_crc_stub(uint32_t st, uint32_t sz )
{
    uint32_t ercd = 0xFFFFFFFFU;

    if(NULL!= g_hook_impdrv_udefctl_udef_crc)
    {
        ercd = g_hook_impdrv_udefctl_udef_crc(st, sz);
    }
    else 
    {
        ercd = impdrv_udefctl_udef_crc(st, sz);
    }

    return ercd;

}

void ut_stub_impdrv_udefctl_udef_ecm(int32_t core_num,bool flag )
{
    s_impdrv_udefctl_set_ecm *IO = &g_impdrv_udefctl_set_ecm;
    // uint32_t ReturnValue;
    // ReturnValue = IO->ReturnValue[IO->ut_index];
    flag = IO->flag;
    core_num = IO->core_num;
    // IO->ut_index++;
    // return ReturnValue;
}

void impdrv_udefctl_set_ecm_stub(int32_t core_num, bool flag)
{

    if( NULL!= g_hook_impdrv_udefctl_set_ecm)
    {
       g_hook_impdrv_udefctl_set_ecm(core_num,flag);
    }
    else 
    {
        impdrv_udefctl_set_ecm(core_num,flag);
    }

}
