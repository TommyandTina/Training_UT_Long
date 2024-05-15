/**
 * \addtogroup osal_wrapper
 * @{
 */

#ifndef THREAD_MANAGER_HPP
#define THREAD_MANAGER_HPP

// system includes
#include <future>
#include <map>
#include <memory>
#include <thread>
#include <tuple>
#ifdef RCAR_XOS_HAVE_PTHREAD
#include <limits.h>
#include <pthread.h>
#endif // RCAR_XOS_HAVE_PTHREAD
// project includes
#include "rcar-xos/osal/r_osal_types.h"

#include "target/simulation/soc_abstraction/soc_init.hpp"

#include "target/simulation/r_osal_common.hpp"
#if defined __WIN32__
#define OSAL_SCHED_PRIORITY SCHED_OTHER
#else
#define OSAL_SCHED_PRIORITY SCHED_FIFO
#endif

#define OSAL_THREAD_HANDLE_ID         (0x4F53414C5F544852ULL) /* ascii code: OSAL_THR */
#define OSAL_THREAD_HANDLE_ID_INVALID (0)


// thread handle structure
struct st_osal_thread_control
{
    std::thread              thread;
    std::thread::id          id;
    osal_thread_id_t         uid;
    e_osal_thread_priority_t priority;
    std::future<int64_t>     future;
    std::string              task_name;
    bool                     b_already_joined;
    uint64_t                 handle_id;

    st_osal_thread_control(const st_osal_thread_config_t * const p_config, osal_thread_id_t thread_id,
                           std::shared_ptr<std::mutex> mtx)
        : uid(thread_id), task_name((p_config->task_name == NULL)?"":p_config->task_name)
    {
        std::promise<int64_t> promise;
        future = promise.get_future();
        std::shared_ptr<st_osal_thread_config_t> sp_config = std::make_shared<st_osal_thread_config_t>();
        sp_config->func         = p_config->func;
        sp_config->priority     = p_config->priority;
        sp_config->stack_size   = p_config->stack_size;
        sp_config->task_name    = const_cast<char*>(task_name.c_str()); //config structure is defined with char* in osal api
        sp_config->userarg      = p_config->userarg; 

        auto lambda = [mtx, sp_config](std::promise<int64_t> && promise) {
#ifdef RCAR_XOS_HAVE_PTHREAD
            sched_param           sch;
            static auto get_pthread_param = [](void) {
                sched_param sch;
                int         policy;

                if (0 != pthread_getschedparam(pthread_self(), &policy, &sch))
                {
                    //throw std::runtime_error("failed to get pthread scheduling parameters");
                }

                int min_prio = sched_get_priority_min(OSAL_SCHED_PRIORITY);
                if (-1 == min_prio)
                {
                    throw std::runtime_error("failed to get pthread minimum scheduling priority");
                }
                return std::make_tuple(sch, OSAL_SCHED_PRIORITY, (min_prio - OSAL_THREAD_PRIORITY_LOWEST));
            };

            static const std::tuple<sched_param, int, int> pthread_param = get_pthread_param();

            sch                = std::get<0>(pthread_param);
            sch.sched_priority = sp_config->priority + std::get<2>(pthread_param);

            // if (pthread_setschedprio(pthread_self(), sp_config->priority - std::get<2>(pthread_param)) != 0)   //for
            // some reason MinGW doesn't defnine this function
            if (pthread_setschedparam(pthread_self(), std::get<1>(pthread_param), &sch) != 0)
            {
                //Ubuntu doesn't allow non root users to change scheduling parameteers
                OSAL_DEBUG("WARNING: failed to set thread priority. Thread priority will be ignored.");
                //throw std::runtime_error("failed to set thread priority");
            }

            if (pthread_setname_np(pthread_self(), const_cast<const char*>(sp_config->task_name)) != 0)
            {
                //Ubuntu doesn't allow non root users to change thread names
                OSAL_DEBUG("WARNING: failed to set thread name.");
                //throw std::runtime_error("failed to set task name");
            }

            // TODO: question: also consider stack??((bad?) idea recursive function call to grow stack to minimum system
            // required) std::cout << "stack min: " << PTHREAD_STACK_MIN << std::endl;     //MinGW doesn't define this
            // constant either

#endif // RCAR_XOS_HAVE_PTHREAD
            std::unique_lock<std::mutex> lck_lambda(
                *mtx); // user function may only start after handle has been registered in manager map
            
            if( NULL != sp_config->func)
            {
                promise.set_value(sp_config->func(sp_config->userarg));
            }
            else
            {
                promise.set_value(0);
            }
        };
        thread = std::thread(lambda, std::move(promise));
        id     = thread.get_id();
        b_already_joined = false;
    }

    bool operator==(std::thread::id & rhs)
    {
        return id == rhs;
    }

}; /**< Handle of a created thread */

// singleton thread manager
class thread_manager final
{
private:
    thread_manager()
    {
    }
    thread_manager(const thread_manager &) = delete;             // delte copy constructor
    thread_manager & operator=(const thread_manager &) = delete; // delete assignmnet operator
    ~thread_manager()
    {
    }

    std::map<osal_thread_id_t, std::unique_ptr<st_osal_thread_control>> handles;
    std::mutex m_mtx;

public:
    static thread_manager & get(void)
    {
        // c++11 makes sure static variables are initialized only once even on concurrent calls.
        // see c++ standard section 6.7/4 "If control enters the declaration concurrently while the variable is being
        // initialized, the concurrent execution shall wait for completion of the initialization."
        static thread_manager instance;
        return instance;
    }

    osal_thread_handle_t get_thread_handle(std::thread::id && id)
    {
        for (auto const & handle : handles)
        {
            if (handle.second->id == id)
            {
                return handle.second.get();
            }
        }
        return nullptr;
    }

    e_osal_return_t register_new_thread(const st_osal_thread_config_t * const p_config, osal_thread_id_t thread_id,
                                        osal_thread_handle_t * const p_handle)
    {
        if (!r_soc_initialized())
        {
            return OSAL_RETURN_STATE;
        }

        if (NULL == p_config || NULL == p_handle)
        {
            return OSAL_RETURN_PAR;
        }
        if (p_config->priority < OSAL_THREAD_PRIORITY_LOWEST ||
            p_config->priority > OSAL_THREAD_PRIORITY_HIGHEST)
        {
            return OSAL_RETURN_PAR;
        }
        if (0U == p_config->stack_size)
        {
            return OSAL_RETURN_PAR;
        }

        m_mtx.lock();
        if (handles.find(thread_id) != handles.end())
        {
            m_mtx.unlock();
            return OSAL_RETURN_BUSY;
        }

        std::shared_ptr<std::mutex>             mtx(new std::mutex);
        std::unique_lock<std::mutex>            lck_mngr(*mtx);
        std::unique_ptr<st_osal_thread_control> cntrl =
        std::unique_ptr<st_osal_thread_control>(new st_osal_thread_control(p_config, thread_id, mtx));
        if (nullptr == cntrl)
        {
            m_mtx.unlock();
            return OSAL_RETURN_MEM;
        }

        cntrl->handle_id = OSAL_THREAD_HANDLE_ID;
        *p_handle          = cntrl.get();
        handles[thread_id] = std::move(cntrl);
        m_mtx.unlock();
        
        return OSAL_RETURN_OK;
    }

    bool unregister_thread(osal_thread_handle_t hndl)
    {
        m_mtx.lock();
        hndl->handle_id = OSAL_THREAD_HANDLE_ID_INVALID;
        if (handles.erase(hndl->uid) == 1)
        {
            m_mtx.unlock();
            return true;
        }
        m_mtx.unlock();
        return false;
    }

    void reset(void)
    {
        m_mtx.lock();
        handles.clear();
        m_mtx.unlock();
    }
    
    e_osal_return_t check_handle(osal_thread_handle_t hndl)
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        if (OSAL_THREAD_HANDLE_ID != hndl->handle_id)
        {
            return OSAL_RETURN_HANDLE;
        }
        return OSAL_RETURN_OK;
    }

    bool isHandleEmpty(void)
    {
        if(handles.empty())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};
#endif /* THREAD_MANAGER_HPP */

/**
 * @}
 */