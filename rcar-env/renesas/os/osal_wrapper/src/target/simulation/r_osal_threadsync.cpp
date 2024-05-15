/**
 * \addtogroup osal_wrapper
 * @{
 */

//project includes
#include "rcar-xos/osal/r_osal.h"
#include "target/simulation/resource_managers/cond_var_manager.hpp"
#include "target/simulation/resource_managers/mutex_manager.hpp"

//system includes
#include <mutex>
#include <condition_variable>

#define OSAL_MAX_SEC     (0x7FFFFFFFL)
#define OSAL_MAX_TV_NSEC (1000000000L - 1L)
#define MUTEX_INNER_TIMEOUT ((osal_milli_sec_t)500U)

static std::timed_mutex g_internal_mtx;

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_ThsyncMutexCreate")
#endif
e_osal_return_t R_OSAL_ThsyncMutexCreate(osal_mutex_id_t mutex_id, osal_mutex_handle_t *const p_handle)
{
    e_osal_return_t osal_ret;

    osal_ret = mutex_manager::get().register_new_mutex(mutex_id, p_handle);
    /* Returns an error code other than OSAL_RETURN_BUSY, overwrite p_handle with OSAL_MUTEX_HANDLE_INVALID */
    if((OSAL_RETURN_OK != osal_ret)&&(OSAL_RETURN_BUSY != osal_ret))
    {
        if(NULL != p_handle)
        {
            *p_handle = OSAL_MUTEX_HANDLE_INVALID;
        }
    }

    return osal_ret;
}


#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_ThsyncMutexDestroy")
#endif
e_osal_return_t R_OSAL_ThsyncMutexDestroy(osal_mutex_handle_t handle)
{
    return mutex_manager::get().unregister_mutex(handle);
}


#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_ThsyncMutexTryLock")
#endif
e_osal_return_t R_OSAL_ThsyncMutexTryLock(osal_mutex_handle_t handle)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if (NULL == handle)
    {
        return OSAL_RETURN_HANDLE;
    }
    
    if (OSAL_RETURN_OK != mutex_manager::get().check_handle(handle))
    {
        return OSAL_RETURN_HANDLE;
    }
    if (handle->mutex.try_lock())
    {
        g_internal_mtx.try_lock_for(std::chrono::milliseconds(MUTEX_INNER_TIMEOUT));
        handle->isLocked = true;
        handle->owner_thread_id = std::this_thread::get_id();
        g_internal_mtx.unlock();
        return OSAL_RETURN_OK;
    }
    return OSAL_RETURN_BUSY;
}


#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_ThsyncMutexLockUntilTimeStamp")
#endif
e_osal_return_t R_OSAL_ThsyncMutexLockUntilTimeStamp(osal_mutex_handle_t handle, const st_osal_time_t *const p_time_stamp)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if (NULL == handle)
    {
        return OSAL_RETURN_HANDLE;
    }
    if (OSAL_RETURN_OK != mutex_manager::get().check_handle(handle))
    {
        return OSAL_RETURN_HANDLE;
    }
    
    if (NULL == p_time_stamp)
    {
        return OSAL_RETURN_PAR;
    }
    else if ((OSAL_MAX_SEC < p_time_stamp->tv_sec) || (0L > p_time_stamp->tv_sec))
    {
        return OSAL_RETURN_PAR;
    }
    else if ((OSAL_MAX_TV_NSEC < p_time_stamp->tv_nsec) || (0L > p_time_stamp->tv_nsec))
    {
        return OSAL_RETURN_PAR;
    }

    bool isr, locked = false;
    e_osal_return_t ret = R_OSAL_InterruptIsISRContext(&isr);
    if (OSAL_RETURN_OK != ret)
    {
        return OSAL_RETURN_FAIL;
    }

    if (isr)
    {
        return R_OSAL_ThsyncMutexTryLock(handle);
    }

    if (handle->owner_thread_id == std::this_thread::get_id())
    {
        return OSAL_RETURN_FAIL;
    }

    using Clock = std::chrono::high_resolution_clock;
    using Ns    = std::chrono::nanoseconds;
    using Sec   = std::chrono::seconds;

    std::chrono::time_point<Clock, Ns> timepoint(Ns(p_time_stamp->tv_nsec) + Sec(p_time_stamp->tv_sec));

    locked = handle->mutex.try_lock_until(timepoint);
    if (locked)
    {
        g_internal_mtx.try_lock_for(std::chrono::milliseconds(MUTEX_INNER_TIMEOUT));
        handle->isLocked = true;
        handle->owner_thread_id = std::this_thread::get_id();
        g_internal_mtx.unlock();
        return OSAL_RETURN_OK;
    }
    return OSAL_RETURN_TIME;
}


#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_ThsyncMutexLockForTimePeriod")
#endif
e_osal_return_t R_OSAL_ThsyncMutexLockForTimePeriod(osal_mutex_handle_t handle, osal_milli_sec_t time_period)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if (NULL == handle)
    {
        return OSAL_RETURN_HANDLE;
    }
    if (OSAL_RETURN_OK != mutex_manager::get().check_handle(handle))
    {
        return OSAL_RETURN_HANDLE;
    }
    
    if (0 > time_period)
    {
        return OSAL_RETURN_PAR;
    }

    bool isr, locked = false;
    e_osal_return_t ret = R_OSAL_InterruptIsISRContext(&isr);
    if (OSAL_RETURN_OK != ret)
    {
        return OSAL_RETURN_FAIL;
    }

    if (isr)
    {
        return R_OSAL_ThsyncMutexTryLock(handle);
    }

    if (handle->owner_thread_id == std::this_thread::get_id())
    {
        return OSAL_RETURN_FAIL;
    }

    locked = handle->mutex.try_lock_for(std::chrono::milliseconds(time_period));
    if (locked)
    {
        g_internal_mtx.try_lock_for(std::chrono::milliseconds(MUTEX_INNER_TIMEOUT));
        handle->isLocked = true;
        handle->owner_thread_id = std::this_thread::get_id();
        g_internal_mtx.unlock();
        return OSAL_RETURN_OK;
    }
    return OSAL_RETURN_TIME;
}


#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_ThsyncMutexUnlock")
#endif
e_osal_return_t R_OSAL_ThsyncMutexUnlock(osal_mutex_handle_t handle)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if (NULL == handle)
    {
        return OSAL_RETURN_HANDLE;
    }
    if (OSAL_RETURN_OK != mutex_manager::get().check_handle(handle))
    {
        return OSAL_RETURN_HANDLE;
    }
    g_internal_mtx.try_lock_for(std::chrono::milliseconds(MUTEX_INNER_TIMEOUT));
    if(( false == handle->isLocked)||(handle->owner_thread_id != std::this_thread::get_id()))
    {
        g_internal_mtx.unlock();
        return OSAL_RETURN_FAIL;
    }

    handle->mutex.unlock();
    handle->isLocked = false;
    handle->owner_thread_id = (std::thread::id)0;
    g_internal_mtx.unlock();
    return OSAL_RETURN_OK;
}


#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_ThsyncCondCreate")
#endif
e_osal_return_t R_OSAL_ThsyncCondCreate(osal_cond_id_t cond_id, osal_cond_handle_t *const p_handle)
{
    e_osal_return_t osal_ret;
    osal_ret = cond_var_manager::get().register_new_cond(cond_id, p_handle);

    /* Returns an error code other than OSAL_RETURN_BUSY, overwrite p_handle with OSAL_COND_HANDLE_INVALID */
    if((OSAL_RETURN_OK != osal_ret)&&(OSAL_RETURN_BUSY != osal_ret))
    {
        if(NULL != p_handle)
        {
            *p_handle = OSAL_COND_HANDLE_INVALID;
        }
    }
    return osal_ret;
}


#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_ThsyncCondDestroy")
#endif
e_osal_return_t R_OSAL_ThsyncCondDestroy(osal_cond_handle_t handle)
{
    return cond_var_manager::get().unregister_cond(handle);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_ThsyncCondWaitForTimePeriod")
#endif
e_osal_return_t R_OSAL_ThsyncCondWaitForTimePeriod(osal_cond_handle_t cond_handle, osal_mutex_handle_t mutex_handle, osal_milli_sec_t time_period)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if (NULL == cond_handle || NULL == mutex_handle)
    {
        return OSAL_RETURN_HANDLE;
    }
    if (OSAL_RETURN_OK != cond_var_manager::get().check_handle(cond_handle))
    {
        return OSAL_RETURN_HANDLE;
    }
    if (OSAL_RETURN_OK != mutex_manager::get().check_handle(mutex_handle))
    {
        return OSAL_RETURN_HANDLE;
    }
    if (0 > time_period)
    {
        return OSAL_RETURN_PAR;
    }
    if (false == mutex_handle->isLocked)
    {
        return OSAL_RETURN_FAIL;
    }

    auto time = std::chrono::milliseconds(time_period);
    g_internal_mtx.try_lock_for(std::chrono::milliseconds(MUTEX_INNER_TIMEOUT));
    mutex_handle->isLocked =false;
    cond_handle->active = true;
    g_internal_mtx.unlock();
    std::unique_lock<std::timed_mutex> lck(mutex_handle->mutex, std::adopt_lock);
    std::cv_status status = cond_handle->condition_variable.wait_for(lck, time);
    lck.release();
    //update the owner_thread_id again ater finihing the wait
    g_internal_mtx.try_lock_for(std::chrono::milliseconds(MUTEX_INNER_TIMEOUT));
    mutex_handle->owner_thread_id = std::this_thread::get_id();
    mutex_handle->isLocked =true;
    cond_handle->active = false;
    g_internal_mtx.unlock();

    if (std::cv_status::timeout == status)
    {
        return OSAL_RETURN_TIME;
    }
    else if (std::cv_status::no_timeout == status)
    {
        return OSAL_RETURN_OK;
    }
    return OSAL_RETURN_FAIL; //should not reach this point
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_ThsyncCondWaitUntilTimeStamp")
#endif
e_osal_return_t R_OSAL_ThsyncCondWaitUntilTimeStamp(osal_cond_handle_t cond_handle, osal_mutex_handle_t mutex_handle, const st_osal_time_t *const p_time_stamp)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if (NULL == cond_handle || NULL == mutex_handle)
    {
        return OSAL_RETURN_HANDLE;
    }
    if (OSAL_RETURN_OK != cond_var_manager::get().check_handle(cond_handle))
    {
        return OSAL_RETURN_HANDLE;
    }
    if (OSAL_RETURN_OK != mutex_manager::get().check_handle(mutex_handle))
    {
        return OSAL_RETURN_HANDLE;
    }
    if (NULL == p_time_stamp)
    {
        return OSAL_RETURN_PAR;
    }
    if ((OSAL_MAX_SEC < p_time_stamp->tv_sec) || (0L > p_time_stamp->tv_sec))
    {
        return OSAL_RETURN_PAR;
    }
    if ((OSAL_MAX_TV_NSEC < p_time_stamp->tv_nsec) || (0L > p_time_stamp->tv_nsec))
    {
        return OSAL_RETURN_PAR;
    }
    if (false == mutex_handle->isLocked)
    {
        return OSAL_RETURN_FAIL;
    }

    using Clock = std::chrono::high_resolution_clock;
    using Ns    = std::chrono::nanoseconds;
    using Sec   = std::chrono::seconds;
    std::chrono::time_point<Clock, Ns> timepoint(Ns(p_time_stamp->tv_nsec) + Sec(p_time_stamp->tv_sec));

    g_internal_mtx.try_lock_for(std::chrono::milliseconds(MUTEX_INNER_TIMEOUT));
    mutex_handle->isLocked =false;
    cond_handle->active = true;
    g_internal_mtx.unlock();
    std::unique_lock<std::timed_mutex> lck(mutex_handle->mutex, std::adopt_lock);
    std::cv_status                     status = cond_handle->condition_variable.wait_until(lck, timepoint);
    lck.release();
    g_internal_mtx.try_lock_for(std::chrono::milliseconds(MUTEX_INNER_TIMEOUT));
    mutex_handle->owner_thread_id = std::this_thread::get_id();
    mutex_handle->isLocked =true;
    cond_handle->active = false;
    g_internal_mtx.unlock();

    if (std::cv_status::timeout == status)
    {
        return OSAL_RETURN_TIME;
    }
    else if (std::cv_status::no_timeout == status)
    {
        return OSAL_RETURN_OK;
    }
    return OSAL_RETURN_FAIL;   //should not reach this point
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_ThsyncCondBroadcast")
#endif
e_osal_return_t R_OSAL_ThsyncCondBroadcast(osal_cond_handle_t handle)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if (NULL == handle)
    {
        return OSAL_RETURN_HANDLE;
    }
    if (OSAL_RETURN_OK != cond_var_manager::get().check_handle(handle))
    {
        return OSAL_RETURN_HANDLE;
    }
    
    handle->condition_variable.notify_all();
    return OSAL_RETURN_OK;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_ThsyncCondSignal")
#endif
e_osal_return_t R_OSAL_ThsyncCondSignal(osal_cond_handle_t handle)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if (NULL == handle)
    {
        return OSAL_RETURN_HANDLE;
    }
    if (OSAL_RETURN_OK != cond_var_manager::get().check_handle(handle))
    {
        return OSAL_RETURN_HANDLE;
    }
    
    handle->condition_variable.notify_one();
    return OSAL_RETURN_OK;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_ThsyncMemoryBarrier")
#endif
e_osal_return_t R_OSAL_ThsyncMemoryBarrier(void)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    /* dummy implementation */
    return OSAL_RETURN_OK;
}

/**
 * @}
 */