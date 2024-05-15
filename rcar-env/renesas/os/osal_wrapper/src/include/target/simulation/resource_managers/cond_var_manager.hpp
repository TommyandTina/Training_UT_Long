/**
 * \addtogroup osal_wrapper
 * @{
 */

#ifndef COND_VAR_MANAGER_HPP
#define COND_VAR_MANAGER_HPP

//system includes
#include <condition_variable>
#include <map>
#include <memory>

//project includes
#include "rcar-xos/osal/r_osal_types.h"

#include "target/simulation/soc_abstraction/soc_init.hpp"

#define OSAL_COND_HANDLE_ID         (0x4F53414C5F434E44ULL) /* ascii code: OSAL_CND */
#define OSAL_COND_HANDLE_ID_INVALID (0)

struct st_osal_cond_control
{
    st_osal_cond_control(osal_cond_id_t id) : uid(id) {}
    std::condition_variable_any condition_variable;
    osal_cond_id_t uid;
    uint64_t handle_id;
    bool active;

}; /**< Handle of a created condition variable. */

//singleton condition variable manager
class cond_var_manager final
{
private:
    cond_var_manager() {}
    cond_var_manager(const cond_var_manager &) = delete;            //delte copy constructor
    cond_var_manager &operator=(const cond_var_manager &) = delete; //delete assignmnet operator
    ~cond_var_manager() {}

    std::map<osal_cond_id_t, std::unique_ptr<st_osal_cond_control>> handles;
    std::mutex m_mtx;
    
public:
    static cond_var_manager &get(void)
    {
        //c++11 makes sure static variables are initialized only once even on concurrent calls.
        //see c++ standard section 6.7/4 "If control enters the declaration concurrently while the variable is being initialized, the concurrent execution shall wait for completion of the initialization."
        static cond_var_manager instance;
        return instance;
    }

    e_osal_return_t register_new_cond(osal_cond_id_t cond_id, osal_cond_handle_t *const p_handle)
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
        if (handles.find(cond_id) != handles.end())
        {
            m_mtx.unlock();
            return OSAL_RETURN_BUSY;
        }

        std::unique_ptr<st_osal_cond_control> cntrl = std::unique_ptr<st_osal_cond_control>(new st_osal_cond_control(cond_id));
        if (nullptr == cntrl)
        {
            m_mtx.unlock();
            return OSAL_RETURN_MEM;
        }

        cntrl->handle_id = OSAL_COND_HANDLE_ID;
        cntrl->active = false;
        *p_handle = cntrl.get();
        handles[cond_id] = std::move(cntrl);
        m_mtx.unlock();

        return OSAL_RETURN_OK;
    }

    e_osal_return_t unregister_cond(osal_cond_handle_t hndl)
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
        if (true == hndl->active)
        {
            return OSAL_RETURN_BUSY;
        }

        m_mtx.lock();
        hndl->handle_id = OSAL_COND_HANDLE_ID_INVALID;
        if (handles.erase(hndl->uid) == 1)
        {
            m_mtx.unlock();
            return OSAL_RETURN_OK;
        }
        m_mtx.unlock();
        return OSAL_RETURN_HANDLE;
    }

    void reset(void)
    {
        m_mtx.lock();
        handles.clear();
        m_mtx.unlock();
    }

    e_osal_return_t check_handle(osal_cond_handle_t hndl)
    {
        std::lock_guard<std::mutex> lock(m_mtx);

        if (OSAL_COND_HANDLE_ID != hndl->handle_id)
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

#endif /* COND_VAR_MANAGER_HPP */

/**
 * @}
 */