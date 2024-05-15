/**
 * \addtogroup osal_wrapper
 * @{
 */

#ifndef MUTEX_MANAGER_HPP
#define MUTEX_MANAGER_HPP

//system includes
#include <mutex>
#include <map>
#include <memory>
#include <thread>
//project includes
#include "rcar-xos/osal/r_osal_types.h"

#define OSAL_MUTEX_HANDLE_ID         (0x4F53414C5F4D5458ULL) /* ascii code: OSAL_MTX */
#define OSAL_MUTEX_HANDLE_ID_INVALID (0)
    
struct st_osal_mutex_control
{
    st_osal_mutex_control(osal_mutex_id_t id) : uid(id) {}

    std::timed_mutex mutex;
    osal_mutex_id_t uid;
    bool isLocked;
    uint64_t handle_id;
    std::thread::id owner_thread_id;

}; /**< Handle of a created mutex. */

//singleton mutex manager
class mutex_manager final
{
private:
    mutex_manager() {}
    mutex_manager(const mutex_manager &) = delete;            //delte copy constructor
    mutex_manager &operator=(const mutex_manager &) = delete; //delete assignmnet operator
    ~mutex_manager() {}

    std::map<osal_mutex_id_t, std::unique_ptr<st_osal_mutex_control>> m_handles;
    std::mutex m_mtx;

public:
    static mutex_manager &get(void)
    {
        //c++11 makes sure static variables are initialized only once even on concurrent calls.
        //see c++ standard section 6.7/4 "If control enters the declaration concurrently while the variable is being initialized, the concurrent execution shall wait for completion of the initialization."
        static mutex_manager instance;
        return instance;
    }

    e_osal_return_t register_new_mutex(osal_mutex_id_t mutex_id, osal_mutex_handle_t *const p_handle)
    {
        if (!r_soc_initialized())
        {
            return OSAL_RETURN_STATE;
        }

        if (NULL == p_handle)
        {
            return OSAL_RETURN_PAR;
        }

        m_mtx.lock();
        if (m_handles.find(mutex_id) != m_handles.end())
        {
            m_mtx.unlock();
            return OSAL_RETURN_BUSY;
        }

        std::unique_ptr<st_osal_mutex_control> cntrl = std::unique_ptr<st_osal_mutex_control>(new st_osal_mutex_control(mutex_id));
        if (nullptr == cntrl)
        {
            m_mtx.unlock();
            return OSAL_RETURN_MEM;
        }

        cntrl->handle_id = OSAL_MUTEX_HANDLE_ID;
        cntrl->isLocked = false;
        cntrl->owner_thread_id = (std::thread::id)0;
        *p_handle = cntrl.get();
        m_handles[mutex_id] = std::move(cntrl);
        m_mtx.unlock();
        return OSAL_RETURN_OK;
    }

    e_osal_return_t unregister_mutex(osal_mutex_handle_t hndl)
    {
        if (!r_soc_initialized())
        {
            return OSAL_RETURN_STATE;
        }

        if (nullptr == hndl)
        {
            return OSAL_RETURN_HANDLE;
        }
        if (OSAL_RETURN_OK != check_handle(hndl))
        {
            return OSAL_RETURN_HANDLE;
        }
        if (true == hndl->isLocked)
        {
            return OSAL_RETURN_BUSY;
        }

        m_mtx.lock();
        hndl->handle_id = OSAL_MUTEX_HANDLE_ID_INVALID;
        if (m_handles.erase(hndl->uid) == 1)
        {
            m_mtx.unlock();
            return OSAL_RETURN_OK; //unique pointers are automatically destroyed when poped of the list
        }
        m_mtx.unlock();
        return OSAL_RETURN_HANDLE;
    }

    void reset(void)
    {
        m_mtx.lock();
        m_handles.clear(); //unique pointers are automatically destroyed when poped of the list
        m_mtx.unlock();
    }
    
    e_osal_return_t check_handle(osal_mutex_handle_t hndl)
    {
        std::lock_guard<std::mutex> lock(m_mtx);

        if (OSAL_MUTEX_HANDLE_ID != hndl->handle_id)
        {
            return OSAL_RETURN_HANDLE;
        }
        return OSAL_RETURN_OK;
    }

    bool isHandleEmpty(void)
    {
        if(m_handles.empty())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

#endif /* MUTEX_MANAGER_HPP */

/**
 * @}
 */