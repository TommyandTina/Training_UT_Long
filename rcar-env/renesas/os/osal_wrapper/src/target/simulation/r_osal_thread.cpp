/**
 * \addtogroup osal_wrapper
 * @{
 */

// project includes
#include "rcar-xos/osal/r_osal.h"
#include "target/simulation/resource_managers/thread_manager.hpp"
#include "target/simulation/r_osal_common.hpp"

// system includes
#include <chrono>
#include <thread>

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_ThreadInitializeThreadConfigSt")
#endif
e_osal_return_t R_OSAL_ThreadInitializeThreadConfigSt(st_osal_thread_config_t* const p_config)
{
    if (p_config == nullptr)
    {
        return OSAL_RETURN_PAR;
    }

    p_config->func = nullptr;
    p_config->priority = OSAL_THREAD_PRIORITY_LOWEST;
    p_config->stack_size = 0;   //ignored in simulator env anyway.
    p_config->task_name = "";
    p_config->userarg = nullptr;

    return OSAL_RETURN_OK;
}


#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_ThreadCreate")
#endif
e_osal_return_t R_OSAL_ThreadCreate(const st_osal_thread_config_t * const p_config, osal_thread_id_t thread_id,
                                    osal_thread_handle_t * const p_handle)
{
    e_osal_return_t osal_ret;

    osal_ret = thread_manager::get().register_new_thread(p_config, thread_id, p_handle);
    /* Returns an error code other than OSAL_RETURN_BUSY, overwrite p_handle with OSAL_THREAD_HANDLE_INVALID */
    if((OSAL_RETURN_OK != osal_ret)&&(OSAL_RETURN_BUSY != osal_ret))
    {
        if(NULL != p_handle)
        {
            *p_handle = OSAL_THREAD_HANDLE_INVALID;
        }
    }

    return osal_ret;
}


#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_ThreadSleepUntilTimeStamp")
#endif
e_osal_return_t R_OSAL_ThreadSleepUntilTimeStamp(const st_osal_time_t * const p_time_stamp)
{
    st_osal_time_t local_time_stamp  = {0, 0};

    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if (NULL == p_time_stamp)
    {
        return OSAL_RETURN_PAR;
    }

    if ((OSAL_MAX_SEC < p_time_stamp->tv_sec) || (0L > p_time_stamp->tv_sec))
    {
        return OSAL_RETURN_PAR;
    }
    else if ((OSAL_MAX_TV_NSEC < p_time_stamp->tv_nsec) || (0L > p_time_stamp->tv_nsec))
    {
        return OSAL_RETURN_PAR;
    }

    using Clock = std::chrono::high_resolution_clock;
    using Ns    = std::chrono::nanoseconds;
    using Sec   = std::chrono::seconds;

    local_time_stamp.tv_sec = p_time_stamp->tv_sec;
    local_time_stamp.tv_nsec = p_time_stamp->tv_nsec;

    /* The minimum unit of time is 1 milli second. */
    /* Digits less than 1 milli second is rounded up to 1 milli second. */
    if ((local_time_stamp.tv_nsec % (int32_t)1000000) != (int32_t)0)
    {
        local_time_stamp.tv_nsec = ((local_time_stamp.tv_nsec / (int32_t)1000000) + (int32_t)1) * (int32_t)1000000;
        if (local_time_stamp.tv_nsec > OSAL_MAX_TV_NSEC)
        {
            local_time_stamp.tv_sec++;
            local_time_stamp.tv_nsec = int32_t((long)local_time_stamp.tv_nsec - (OSAL_MAX_TV_NSEC + (int32_t)1));
        }
    }

    std::chrono::time_point<Clock, Ns> timepoint(Ns(local_time_stamp.tv_nsec) + Sec(local_time_stamp.tv_sec));
    std::this_thread::sleep_until(timepoint);

    return OSAL_RETURN_OK;
}


#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_ThreadSleepForTimePeriod")
#endif
e_osal_return_t R_OSAL_ThreadSleepForTimePeriod(osal_milli_sec_t time_period)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if (0 > time_period)
    {
        return OSAL_RETURN_PAR;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(time_period));
    return OSAL_RETURN_OK;
}


#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_ThreadSelf")
#endif
e_osal_return_t R_OSAL_ThreadSelf(osal_thread_handle_t * const p_handle)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if (NULL == p_handle)
    {
        return OSAL_RETURN_PAR;
    }

    osal_thread_handle_t hndl = thread_manager::get().get_thread_handle(std::this_thread::get_id());

    if (nullptr == hndl)
    {
        return OSAL_RETURN_FAIL; // Wrapper of other OS returns OSAL_RETURN_FAIL. Set to OSAL_RETURN_FAIL in OSAL SIL, too.
    }
    if(OSAL_RETURN_OK != thread_manager::get().check_handle(hndl))
    {
        return OSAL_RETURN_FAIL; // Wrapper of other OS returns OSAL_RETURN_FAIL. Set to OSAL_RETURN_FAIL in OSAL SIL, too.
    }

    *p_handle = hndl;
    return OSAL_RETURN_OK;
}


#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_ThreadEqual")
#endif
e_osal_return_t R_OSAL_ThreadEqual(osal_thread_handle_t handle1, osal_thread_handle_t handle2, bool * const p_result)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if (NULL == handle1 || NULL == handle2)
    {
        return OSAL_RETURN_HANDLE;
    }
    
    if (OSAL_RETURN_OK != thread_manager::get().check_handle(handle1))
    {
        return OSAL_RETURN_HANDLE;
    }
    if (OSAL_RETURN_OK != thread_manager::get().check_handle(handle2))
    {
        return OSAL_RETURN_HANDLE;
    }
    
    if (NULL == p_result)
    {
        return OSAL_RETURN_PAR;
    }

    *p_result = ((handle1->id == handle2->id) && (handle1->uid == handle2->uid));

    return OSAL_RETURN_OK;
}


#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_ThreadJoin")
#endif
e_osal_return_t R_OSAL_ThreadJoin(osal_thread_handle_t handle, int64_t * const return_value)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if(NULL == handle)
    {
        return OSAL_RETURN_HANDLE;
    }
    
    if(OSAL_RETURN_OK != thread_manager::get().check_handle(handle))
    {
        return OSAL_RETURN_HANDLE;
    }
    
    if(NULL == return_value)
    {
        return OSAL_RETURN_PAR;
    }
    
    if(handle->thread.get_id() == std::this_thread::get_id())
    {
        return OSAL_RETURN_HANDLE;
    }

    if (!handle->thread.joinable())
    {
        return OSAL_RETURN_HANDLE;
    }

    if( true == handle->b_already_joined )
    {
        return OSAL_RETURN_HANDLE;
    }

    handle->b_already_joined = true;

    handle->thread.join();
    *return_value = handle->future.get();
    if (thread_manager::get().unregister_thread(handle))
    {
        handle->b_already_joined = false;

        return OSAL_RETURN_OK;
    }
    return OSAL_RETURN_FAIL; // didn't find handle in manager
}
/**
 * @}
 */

