/**
 * \addtogroup osal_wrapper
 * @{
 */

// project includes
#include "rcar-xos/osal/r_osal.h"
#include "target/simulation/r_osal_common.hpp"
#include "target/simulation/message_queue.hpp"
#include "target/simulation/resource_managers/mq_manager.hpp"
#include "target/simulation/soc_abstraction/soc_init.hpp"

// system includes

// api implementation

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_MqInitializeMqConfigSt")
#endif
e_osal_return_t R_OSAL_MqInitializeMqConfigSt(st_osal_mq_config_t* const p_config)
{
    if (p_config == nullptr)
    {
        return OSAL_RETURN_PAR;
    }

    p_config->max_num_msg = 0;
    p_config->msg_size = 0;

    return OSAL_RETURN_OK;
    
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_MqCreate")
#endif
e_osal_return_t R_OSAL_MqCreate(const st_osal_mq_config_t * const p_config, osal_mq_id_t mq_Id,
                                osal_mq_handle_t * const p_handle)
{
    e_osal_return_t osal_ret;

    osal_ret = mq_manager::get().register_new_mq(p_config, mq_Id, p_handle);

    /* Returns an error code other than OSAL_RETURN_BUSY, overwrite p_handle with OSAL_MQ_HANDLE_INVALID */
    if((OSAL_RETURN_OK != osal_ret)&&(OSAL_RETURN_BUSY != osal_ret))
    {
        if(NULL != p_handle)
        {
            *p_handle = OSAL_MQ_HANDLE_INVALID;
        }
    }

    return osal_ret;
}


#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_MqDelete")
#endif
e_osal_return_t R_OSAL_MqDelete(osal_mq_handle_t handle)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    return mq_manager::get().unregister_mq(handle);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_MqReset")
#endif
e_osal_return_t R_OSAL_MqReset(osal_mq_handle_t handle)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if (NULL == handle)
    {
        return OSAL_RETURN_HANDLE;
    }

    if (OSAL_RETURN_OK != mq_manager::get().check_handle(handle))
    {
        return OSAL_RETURN_HANDLE;
    }

    handle->mq.reset();
    return OSAL_RETURN_OK;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_MqSendUntilTimeStamp")
#endif
e_osal_return_t R_OSAL_MqSendUntilTimeStamp(osal_mq_handle_t handle, const st_osal_time_t * const p_time_stamp,
                                            const void * p_buffer, size_t buffer_size)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if (NULL == handle)
    {
        return OSAL_RETURN_HANDLE;
    }

    if (OSAL_RETURN_OK != mq_manager::get().check_handle(handle))
    {
        return OSAL_RETURN_HANDLE;
    }

    if (NULL == p_buffer || NULL == p_time_stamp)
    {
        return OSAL_RETURN_PAR;
    }

    if ((p_time_stamp->tv_sec < 0) || (p_time_stamp->tv_nsec < 0) || (OSAL_MAX_TV_NSEC < p_time_stamp->tv_nsec))
    {
        return OSAL_RETURN_PAR;
    }

    std::size_t config_msg_size;
    int64_t     max;
    bool        isr;

    handle->mq.get_conf(config_msg_size, max);
    
    if (buffer_size != config_msg_size)
    {
        return OSAL_RETURN_PAR;
    }

    osal_message_t msg(config_msg_size);
    msg.set(p_buffer, buffer_size);

    e_osal_return_t ret = R_OSAL_InterruptIsISRContext(&isr);
    if (OSAL_RETURN_OK != ret)
    {
        return OSAL_RETURN_FAIL;
    }

    if (isr)
    {
        if (handle->mq.try_send(msg))
        {
            return OSAL_RETURN_OK;
        }
        return OSAL_RETURN_BUSY;
    }

    using Clock = std::chrono::high_resolution_clock;
    using Ns    = std::chrono::nanoseconds;
    using Sec   = std::chrono::seconds;

    std::chrono::time_point<Clock, Ns> timepoint(Ns(p_time_stamp->tv_nsec) + Sec(p_time_stamp->tv_sec));

    if (handle->mq.try_send_until(msg, timepoint))
    {
        return OSAL_RETURN_OK;
    }
    return OSAL_RETURN_TIME;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_MqSendForTimePeriod")
#endif
e_osal_return_t R_OSAL_MqSendForTimePeriod(osal_mq_handle_t handle, osal_milli_sec_t time_period, const void * p_buffer,
                                           size_t buffer_size)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if (nullptr == handle)
    {
        return OSAL_RETURN_HANDLE;
    }

    if (OSAL_RETURN_OK != mq_manager::get().check_handle(handle))
    {
        return OSAL_RETURN_HANDLE;
    }
    
    if (nullptr == p_buffer)
    {
        return OSAL_RETURN_PAR;
    }

    if (0 > time_period)
    {
        return OSAL_RETURN_PAR;
    }

    std::size_t config_msg_size;
    int64_t     max;
    bool        isr;

    handle->mq.get_conf(config_msg_size, max);
        
    if (buffer_size != config_msg_size)
    {
        return OSAL_RETURN_PAR;
    }
    
    osal_message_t msg(config_msg_size);
    msg.set(p_buffer, buffer_size);

    e_osal_return_t ret = R_OSAL_InterruptIsISRContext(&isr);
    if (OSAL_RETURN_OK != ret)
    {
        return OSAL_RETURN_FAIL;
    }

    if (isr)
    {
        if (handle->mq.try_send(msg))
        {
            return OSAL_RETURN_OK;
        }
        return OSAL_RETURN_BUSY;
    }

    if (handle->mq.try_send_for(msg, std::chrono::milliseconds(time_period)))
    {
        return OSAL_RETURN_OK;
    }
    return OSAL_RETURN_TIME;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_MqReceiveUntilTimeStamp")
#endif
e_osal_return_t R_OSAL_MqReceiveUntilTimeStamp(osal_mq_handle_t handle, const st_osal_time_t * const p_time_stamp,
                                               void * p_buffer, size_t buffer_size)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if (NULL == handle)
    {
        return OSAL_RETURN_HANDLE;
    }

    if (OSAL_RETURN_OK != mq_manager::get().check_handle(handle))
    {
        return OSAL_RETURN_HANDLE;
    }

    if (NULL == p_buffer || NULL == p_time_stamp)
    {
        return OSAL_RETURN_PAR;
    }
    
    if ((p_time_stamp->tv_sec < 0) || (p_time_stamp->tv_nsec < 0) || (OSAL_MAX_TV_NSEC < p_time_stamp->tv_nsec))
    {
        return OSAL_RETURN_PAR;
    }

    std::size_t config_msg_size;
    int64_t     max;
    bool        isr;

    handle->mq.get_conf(config_msg_size, max);
        
    if (buffer_size != config_msg_size)
    {
        return OSAL_RETURN_PAR;
    }
    
    osal_message_t msg; 
    e_osal_return_t ret = R_OSAL_InterruptIsISRContext(&isr);
    if (OSAL_RETURN_OK != ret)
    {
        return OSAL_RETURN_FAIL;
    }

    if (isr)
    {
        if (handle->mq.try_receive(msg))
        {
            msg.get(p_buffer, buffer_size);
            return OSAL_RETURN_OK;
        }
        return OSAL_RETURN_BUSY;
    }

    using Clock = std::chrono::high_resolution_clock;
    using Ns    = std::chrono::nanoseconds;
    using Sec   = std::chrono::seconds;

    std::chrono::time_point<Clock, Ns> timepoint(Ns(p_time_stamp->tv_nsec) + Sec(p_time_stamp->tv_sec));

    if (handle->mq.try_receive_until(msg, timepoint))
    {
        msg.get(p_buffer, buffer_size);
        return OSAL_RETURN_OK;
    }
    return OSAL_RETURN_TIME;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_MqReceiveForTimePeriod")
#endif
e_osal_return_t R_OSAL_MqReceiveForTimePeriod(osal_mq_handle_t handle, osal_milli_sec_t time_period, void * p_buffer,
                                              size_t buffer_size)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if (NULL == handle)
    {
        return OSAL_RETURN_HANDLE;
    }

    if (OSAL_RETURN_OK != mq_manager::get().check_handle(handle))
    {
        return OSAL_RETURN_HANDLE;
    }

    if (NULL == p_buffer )
    {
        return OSAL_RETURN_PAR;
    }

    if (time_period < 0)
    {
        return OSAL_RETURN_PAR;
    }

    std::size_t config_msg_size;
    int64_t     max;
    bool        isr;

    handle->mq.get_conf(config_msg_size, max);
        
    if (buffer_size != config_msg_size)
    {
        return OSAL_RETURN_PAR;
    }

    osal_message_t msg;
    e_osal_return_t ret = R_OSAL_InterruptIsISRContext(&isr);
    if (OSAL_RETURN_OK != ret)
    {
        return OSAL_RETURN_FAIL;
    }

    if (isr)
    {
        if (handle->mq.try_receive(msg))
        {
            msg.get(p_buffer, buffer_size);
            return OSAL_RETURN_OK;
        }
        return OSAL_RETURN_BUSY;
    }

    if (handle->mq.try_receive_for(msg, std::chrono::milliseconds(time_period)))
    {
        msg.get(p_buffer, buffer_size);
        return OSAL_RETURN_OK;
    }
    return OSAL_RETURN_TIME;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_MqIsFull")
#endif
e_osal_return_t R_OSAL_MqIsFull(osal_mq_handle_t handle, bool * const p_result)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if (NULL == handle)
    {
        return OSAL_RETURN_HANDLE;
    }

    if (OSAL_RETURN_OK != mq_manager::get().check_handle(handle))
    {
        return OSAL_RETURN_HANDLE;
    }

    if (NULL == p_result)
    {
        return OSAL_RETURN_PAR;
    }
    *p_result = handle->mq.full();
    return OSAL_RETURN_OK;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_MqIsEmpty")
#endif
e_osal_return_t R_OSAL_MqIsEmpty(osal_mq_handle_t handle, bool * const p_result)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if (NULL == handle)
    {
        return OSAL_RETURN_HANDLE;
    }

    if (OSAL_RETURN_OK != mq_manager::get().check_handle(handle))
    {
        return OSAL_RETURN_HANDLE;
    }

    if (NULL == p_result)
    {
        return OSAL_RETURN_PAR;
    }
    *p_result = handle->mq.empty();
    return OSAL_RETURN_OK;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_MqGetConfig")
#endif
e_osal_return_t R_OSAL_MqGetConfig(osal_mq_handle_t handle, st_osal_mq_config_t * const p_config)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if (NULL == handle)
    {
        return OSAL_RETURN_HANDLE;
    }

    if (OSAL_RETURN_OK != mq_manager::get().check_handle(handle))
    {
        return OSAL_RETURN_HANDLE;
    }

    if (NULL == p_config)
    {
        return OSAL_RETURN_PAR;
    }

    std::size_t size;
    int64_t     max;
    handle->mq.get_conf(size, max);
    p_config->msg_size    = size;
    p_config->max_num_msg = max;
    return OSAL_RETURN_OK;
}
/**
 * @}
 */