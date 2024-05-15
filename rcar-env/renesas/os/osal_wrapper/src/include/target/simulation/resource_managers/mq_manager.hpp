/**
 * \addtogroup osal_wrapper
 * @{
 */

#ifndef MQ_MANAGER_HPP
#define MQ_MANAGER_HPP

// system includes
#include <map>
#include <memory>

// project includes
#include "rcar-xos/osal/r_osal_types.h"
#include "target/simulation/message_queue.hpp"
#include "target/simulation/soc_abstraction/soc_init.hpp"

#define OSAL_MESSAGE_HANDLE_ID         (0x4F53414C5F4D5347ULL) /* ascii code: OSAL_MSG */
#define OSAL_MESSAGE_HANDLE_ID_INVALID (0)
 
struct alignas(16) st_osal_mq_control
{
    st_osal_mq_control(osal_mq_id_t id, std::size_t msg_size, uint64_t msg_max) : mq(msg_size, msg_max), uid(id)
    {
    } // constructor to initialize message_queue class

    message_queue<osal_message_t> mq;
    osal_mq_id_t                  uid;
    uint64_t                      handle_id;

}; /**< Handle of a created message queue. */

// singleton message_queue manager
class mq_manager final
{
private:
    mq_manager()
    {
    }
    mq_manager(const mq_manager &) = delete;             // delte copy constructor
    mq_manager & operator=(const mq_manager &) = delete; // delete assignmnet operator
    ~mq_manager()
    {
    }

    std::map<osal_mq_id_t, std::unique_ptr<st_osal_mq_control>> handles;
    std::mutex m_mtx;

public:
    static mq_manager & get(void)
    {
        // c++11 makes sure static variables are initialized only once even on concurrent calls.
        // see c++ standard section 6.7/4 "If control enters the declaration concurrently while the variable is being
        // initialized, the concurrent execution shall wait for completion of the initialization."
        static mq_manager instance;
        return instance;
    }

    e_osal_return_t register_new_mq(const st_osal_mq_config_t * const p_config, osal_mq_id_t mq_id,
                                    osal_mq_handle_t * const p_handle)
    {

        if (!r_soc_initialized())
        {
            return OSAL_RETURN_STATE;
        }

        if (NULL == p_handle)
        {
            return OSAL_RETURN_PAR;
        }

        if (NULL == p_config)
        {
            return OSAL_RETURN_PAR;
        }

        if ((0U == p_config->max_num_msg) || (0U == p_config->msg_size))
        {
            return OSAL_RETURN_PAR;
        }

        m_mtx.lock();
        if (handles.find(mq_id) != handles.end())
        {
            m_mtx.unlock();
            return OSAL_RETURN_BUSY;
        }

        std::unique_ptr<st_osal_mq_control> cntrl = std::unique_ptr<st_osal_mq_control>(
            new st_osal_mq_control(mq_id, p_config->msg_size, p_config->max_num_msg));
        if (nullptr == cntrl)
        {
            m_mtx.unlock();
            return OSAL_RETURN_MEM;
        }
        
        cntrl->handle_id = OSAL_MESSAGE_HANDLE_ID;
        *p_handle      = cntrl.get();
        handles[mq_id] = std::move(cntrl);
        m_mtx.unlock();

        return OSAL_RETURN_OK;
    }

    e_osal_return_t unregister_mq(osal_mq_handle_t hndl)
    {
        if (nullptr == hndl)
        {
            return OSAL_RETURN_HANDLE;
        }
        if (OSAL_RETURN_OK != check_handle(hndl))
        {
            return OSAL_RETURN_HANDLE;
        }
        
        if (hndl->mq.is_busy())
        {
            return OSAL_RETURN_BUSY;
        }

        m_mtx.lock();
        hndl->handle_id = OSAL_MESSAGE_HANDLE_ID_INVALID;
        if (handles.erase(hndl->uid) == 1)
        {
            m_mtx.unlock();
            // nested destructors take care of the stored messages
            return OSAL_RETURN_OK;
        }
        m_mtx.unlock();
        return OSAL_RETURN_HANDLE;
    }

    void reset(void)
    {
        m_mtx.lock();
        // nested destructors take care of the stored messages
        handles.clear();
        m_mtx.unlock();
    }
    
    e_osal_return_t check_handle(osal_mq_handle_t hndl)
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        if (OSAL_MESSAGE_HANDLE_ID != hndl->handle_id)
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

#endif /* MQ_MANAGER_HPP */

/**
 * @}
 */