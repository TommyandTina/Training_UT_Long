/**
 * \addtogroup osal_wrapper
 * @{
 */

#ifndef DEVICE_MANAGER_HPP
#define DEVICE_MANAGER_HPP

// system includes
#include <algorithm>
#include <map>
#include <memory>
#include <mutex>
// project includes
#include "target/simulation/soc_abstraction/soc_init.hpp"
#include "rcar-xos/osal/r_osal.h"
#include "target/simulation/soc_abstraction/dev_cfg.hpp"

#define OSAL_DEVICE_HANDLE_ID         (0x4F53414C5F444556ULL) /* ascii code: OSAL_DEV */
#define OSAL_DEVICE_HANDLE_ID_INVALID (0)
#define OSAL_BUS_DOMAIN_SIZE          (32U)
#define OSAL_STATIC static
    
struct st_osal_device_control
{
    st_osal_device_control()
    {
        axi_id            = OSAL_AXI_BUS_ID_INVALID;
        handle_id         = OSAL_DEVICE_HANDLE_ID_INVALID;
        pm_required_state = OSAL_PM_REQUIRED_STATE_INVALID;
        pm_state          = OSAL_PM_STATE_INVALID;
        pm_policy         = OSAL_PM_POLICY_INVALID;
        pm_reset_state    = OSAL_PM_RESET_STATE_INVALID;
        pm_lowest_intermediate_state = OSAL_PM_STATE_INVALID;
        irq_use.clear();
    }
    ~st_osal_device_control()
    {
        axi_id            = OSAL_AXI_BUS_ID_INVALID;
        handle_id         = OSAL_DEVICE_HANDLE_ID_INVALID;
        pm_required_state = OSAL_PM_REQUIRED_STATE_INVALID;
        pm_state          = OSAL_PM_STATE_INVALID;
        pm_policy         = OSAL_PM_POLICY_INVALID;
        pm_reset_state    = OSAL_PM_RESET_STATE_INVALID;
        pm_lowest_intermediate_state = OSAL_PM_STATE_INVALID;
        irq_use.clear();
    }

    Osal_Device dev;

    osal_axi_bus_id_t axi_id;

    uint64_t handle_id;
    
    e_osal_pm_required_state_t pm_required_state;
    e_osal_pm_state_t          pm_state;
    e_osal_pm_policy_t         pm_policy;
    e_osal_pm_reset_state_t    pm_reset_state;
    e_osal_pm_state_t          pm_lowest_intermediate_state;
    
    std::mutex int_mtx;
    std::mutex pm_mtx;

    std::map<uint32_t, bool> irq_use;
}; /**< Handle of opened device. */

// singleton condition variable manager
class device_manager final
{
private:
    device_manager()
    {
    }
    device_manager(const device_manager &) = delete;             // delte copy constructor
    device_manager & operator=(const device_manager &) = delete; // delete assignmnet operator
    ~device_manager()
    {
    }

    std::map<std::string, std::unique_ptr<st_osal_device_control>> m_handles;
    std::mutex m_mtx;
    bool device_init_state;
public:
    static device_manager & get(void)
    {
        // c++11 makes sure static variables are initialized only once even on concurrent calls.
        // see c++ standard section 6.7/4 "If control enters the declaration concurrently while the variable is being
        // initialized, the concurrent execution shall wait for completion of the initialization."
        static device_manager instance;
        return instance;
    }

    bool initialize_device_init_state()
    {
        device_init_state = true;
        return device_init_state;
    }

    e_osal_return_t register_new_dev(std::string dev_id, osal_device_handle_t * const p_handle)
    {
        if (m_handles.find(dev_id) != m_handles.end())
        {
            return OSAL_RETURN_BUSY;
        }

        dev_cfg     devcfg;
        Osal_Device osal_dev;
        if (!devcfg.find_device(dev_id, osal_dev))
        {
            return OSAL_RETURN_ID;
        }
        std::unique_ptr<st_osal_device_control> cntrl =
            std::unique_ptr<st_osal_device_control>(new st_osal_device_control());
        if (nullptr == cntrl)
        {
            return OSAL_RETURN_MEM;
        }
        cntrl->dev               = osal_dev;
        cntrl->pm_policy         = OSAL_PM_POLICY_PG;
        cntrl->pm_required_state = OSAL_PM_REQUIRED_STATE_RELEASED;
        cntrl->pm_reset_state    = OSAL_PM_RESET_STATE_RELEASED;
        cntrl->pm_state          = OSAL_PM_STATE_PG;
        cntrl->axi_id            = devcfg.axi_id(osal_dev);
        cntrl->handle_id         = OSAL_DEVICE_HANDLE_ID;

        if (OSAL_AXI_BUS_ID_INVALID == cntrl->axi_id)
        {
            return OSAL_RETURN_CONF;
        }

        *p_handle         = cntrl.get();
        m_handles[dev_id] = std::move(cntrl);

        return OSAL_RETURN_OK;
    }

    e_osal_return_t unregister_dev(osal_device_handle_t hndl)
    {
        std::string::size_type ret;

        ret = m_handles.erase(hndl->dev.get_id().c_str());
        if (ret == 1)
        {
            return OSAL_RETURN_OK;
        }

        return OSAL_RETURN_HANDLE;
    }

    bool get_device_init_state()
    {
        return device_init_state;
    }

    void reset(void)
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        device_init_state = false;
        m_handles.clear();
    }

    e_osal_return_t check_handle(osal_device_handle_t hndl)
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        if (OSAL_DEVICE_HANDLE_ID != hndl->handle_id)
        {
            return OSAL_RETURN_HANDLE;
        }
        return OSAL_RETURN_OK;
    }

    std::mutex & get_mutex()
    {
        return m_mtx;
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

/*******************************************************************************************************************//**
 * @struct io_domain_table
 * structure to hold domain information for axi API.
***********************************************************************************************************************/
typedef struct io_domain_table{
    char domain_name[OSAL_BUS_DOMAIN_SIZE];             /*!< domain name */
    int32_t *p_utlb_buffer;                             /*!< parsed utlb */
    size_t utlb_buffer_count;                           /*!< number of utlb */
}io_domain_table_t;

#endif /* DEVICE_MANAGER_HPP */

/**
 * @}
 */
