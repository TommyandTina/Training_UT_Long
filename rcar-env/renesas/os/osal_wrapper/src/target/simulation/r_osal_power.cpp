/**
 * \addtogroup osal_wrapper
 * @{
 */

// project includes
#include "rcar-xos/osal/r_osal.h"
#include "target/simulation/resource_managers/device_manager.hpp"
#include <thread>
#include "target/simulation/r_osal_common.hpp"

// system includes
#include <iostream>

//#define WARN() std::cout << "[WARNING] OSAL power Manager is ineffictive in SIL" << std::endl;
#define WARN() ;
#define OSAL_PM_WAIT_TIME   (333000L) /* 333us */


static e_osal_pm_state_t set_required(e_osal_pm_reset_state_t reset_state)
{
    e_osal_pm_state_t ret;
    switch (reset_state)
    {
        case OSAL_PM_RESET_STATE_APPLIED:
            ret = OSAL_PM_STATE_RESET;
            break;
        case OSAL_PM_RESET_STATE_RELEASED:
            ret = OSAL_PM_STATE_READY;
            break;
        default:
            ret = OSAL_PM_STATE_INVALID;
            break;
    }
    return ret;
}

static e_osal_pm_state_t set_released(e_osal_pm_policy_t policy, dev_power_domain_t power_domain)
{
    e_osal_pm_state_t ret;
    switch (policy)
    {
        case OSAL_PM_POLICY_PG:
            if(OSAL_POWER_DOMAIN_SHARED == power_domain)
            {
                ret = OSAL_PM_STATE_CG;
            }
            else /* OSAL_POWER_DOMAIN_NOT_SHARED. The OSAL_POWER_DOMAIN_INVALID have already been checked*/
            {
                ret = OSAL_PM_STATE_PG;
            }
            break;
        case OSAL_PM_POLICY_CG:
            ret = OSAL_PM_STATE_CG;
            break;
        case OSAL_PM_POLICY_HP:
            ret = OSAL_PM_STATE_ENABLED;
            break;
        default:
            ret = OSAL_PM_STATE_INVALID;
            break;
    }
    return ret;
}

static e_osal_return_t find_power_clock_reset_domain(osal_device_handle_t handle, dev_power_domain_t* power_domain,
                                                dev_clock_domain_t* clock_domain, dev_reset_domain_t* reset_domain)
{
    e_osal_return_t ret = OSAL_RETURN_OK;
    size_t count = 0;
    size_t i;
    std::lock_guard<std::mutex> lock(handle->pm_mtx);

    count = sizeof(gs_osal_power_clock_reset_info)/sizeof(gs_osal_power_clock_reset_info[0]);
    for(i = 0; i < count; i++)
    {
        if(!strcmp(handle->dev.get_id().c_str(), gs_osal_power_clock_reset_info[i].id))
        {
            *power_domain = gs_osal_power_clock_reset_info[i].power_domain;
            *clock_domain = gs_osal_power_clock_reset_info[i].clock_domain;
            *reset_domain = gs_osal_power_clock_reset_info[i].reset_domain;
            break;
        }
    }

    if(i >= count)
    {
        ret = OSAL_RETURN_HANDLE;
        *power_domain = OSAL_POWER_DOMAIN_INVALID;
        *clock_domain = OSAL_CLOCK_DOMAIN_INVALID;
        *reset_domain = OSAL_RESET_DOMAIN_INVALID;
    }

    return ret;
}

static void get_state(osal_device_handle_t handle, e_osal_pm_state_t* power_state)
{
    *power_state = handle->pm_state;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_PmSetRequiredState")
#endif
e_osal_return_t R_OSAL_PmSetRequiredState(osal_device_handle_t handle, e_osal_pm_required_state_t state,
                                          bool applyImmediate)
{
    (void)applyImmediate; /* unused */
    dev_power_domain_t power_domain = OSAL_POWER_DOMAIN_INVALID;
    dev_clock_domain_t clock_domain = OSAL_CLOCK_DOMAIN_INVALID;
    dev_reset_domain_t reset_domain = OSAL_RESET_DOMAIN_INVALID;
    WARN()
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }
    if (OSAL_DEVICE_HANDLE_INVALID == handle)
    {
        return OSAL_RETURN_HANDLE;
    }    
    if (OSAL_RETURN_OK != device_manager::get().check_handle(handle))
    {
        return OSAL_RETURN_HANDLE;
    }
    if(OSAL_RETURN_OK != find_power_clock_reset_domain(handle, &power_domain, &clock_domain, &reset_domain)) // not found the device name in gs_osal_power_clock_reset_info
    {
        return OSAL_RETURN_UNSUPPORTED_OPERATION;
    }

    if(OSAL_POWER_DOMAIN_INVALID == power_domain || OSAL_CLOCK_DOMAIN_INVALID == clock_domain)
    {
        return OSAL_RETURN_UNSUPPORTED_OPERATION;
    }

    std::lock_guard<std::mutex> lock(handle->pm_mtx);

    switch (state)
    {
        case OSAL_PM_REQUIRED_STATE_REQUIRED:
            if(OSAL_RESET_DOMAIN_INVALID == reset_domain)
            {
                handle->pm_reset_state = OSAL_PM_RESET_STATE_INVALID;
            }
            handle->pm_state = set_required(handle->pm_reset_state);
            break;
        case OSAL_PM_REQUIRED_STATE_RELEASED:
            handle->pm_state = set_released(handle->pm_policy, power_domain);
            handle->pm_lowest_intermediate_state = handle->pm_state;
            break;
        default:
            return OSAL_RETURN_PAR;
    }

    handle->pm_required_state = state;

    return OSAL_RETURN_OK;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_PmGetRequiredState")
#endif
e_osal_return_t R_OSAL_PmGetRequiredState(osal_device_handle_t handle, e_osal_pm_required_state_t * const p_state)
{
    WARN()
    dev_power_domain_t power_domain = OSAL_POWER_DOMAIN_INVALID;
    dev_clock_domain_t clock_domain = OSAL_CLOCK_DOMAIN_INVALID;
    dev_reset_domain_t reset_domain = OSAL_RESET_DOMAIN_INVALID;
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }
    if (OSAL_DEVICE_HANDLE_INVALID == handle)
    {
        return OSAL_RETURN_HANDLE;
    }
    if (OSAL_RETURN_OK != device_manager::get().check_handle(handle))
    {
        return OSAL_RETURN_HANDLE;
    }
    if(OSAL_RETURN_OK != find_power_clock_reset_domain(handle, &power_domain, &clock_domain, &reset_domain)) // not found the device name in gs_osal_power_clock_reset_info
    {
        return OSAL_RETURN_UNSUPPORTED_OPERATION;
    }

    if(OSAL_POWER_DOMAIN_INVALID == power_domain || OSAL_CLOCK_DOMAIN_INVALID == clock_domain)
    {
        return OSAL_RETURN_UNSUPPORTED_OPERATION;
    }
    if (NULL == p_state)
    {
        return OSAL_RETURN_PAR;
    }
    
    std::lock_guard<std::mutex> lock(handle->pm_mtx);

    *p_state = handle->pm_required_state;
    return OSAL_RETURN_OK;
}


#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_PmWaitForState")
#endif
e_osal_return_t R_OSAL_PmWaitForState(osal_device_handle_t handle, e_osal_pm_state_t state,
                                      osal_milli_sec_t time_period)
{
    e_osal_return_t ret = OSAL_RETURN_FAIL;
    dev_power_domain_t power_domain = OSAL_POWER_DOMAIN_INVALID;
    dev_clock_domain_t clock_domain = OSAL_CLOCK_DOMAIN_INVALID;
    dev_reset_domain_t reset_domain = OSAL_RESET_DOMAIN_INVALID;
    // no need to WARN()
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }
    if (OSAL_DEVICE_HANDLE_INVALID == handle)
    {
        return OSAL_RETURN_HANDLE;
    }
    if (OSAL_RETURN_OK != device_manager::get().check_handle(handle))
    {
        return OSAL_RETURN_HANDLE;
    }
    if(OSAL_RETURN_OK != find_power_clock_reset_domain(handle, &power_domain, &clock_domain, &reset_domain)) // not found the device name in gs_osal_power_clock_reset_info
    {
        return OSAL_RETURN_UNSUPPORTED_OPERATION;
    }

    if(OSAL_POWER_DOMAIN_INVALID == power_domain || OSAL_CLOCK_DOMAIN_INVALID == clock_domain)
    {
        return OSAL_RETURN_UNSUPPORTED_OPERATION;
    }

    if(OSAL_PM_REQUIRED_STATE_REQUIRED == handle->pm_required_state && OSAL_RESET_DOMAIN_INVALID == reset_domain)
    {
        if(OSAL_PM_STATE_READY == state || OSAL_PM_STATE_RESET == state)
        {
            return OSAL_RETURN_UNSUPPORTED_OPERATION;
        }
    }
    
    if (time_period < 0)
    {
        return OSAL_RETURN_PAR;
    }
    if ((OSAL_PM_STATE_PG != state) &&
             (OSAL_PM_STATE_CG != state) &&
             (OSAL_PM_STATE_ENABLED != state) &&
             (OSAL_PM_STATE_READY != state) &&
             (OSAL_PM_STATE_RESET != state))
    {
        return OSAL_RETURN_PAR;
    }

    st_osal_time_t    starttime, timenow;
    osal_nano_sec_t   elapsedtime_ns = 0;
    e_osal_pm_state_t current_state;
    osal_nano_sec_t sleep_time;
    
    std::lock_guard<std::mutex> lock(handle->pm_mtx);

    ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_PRECISION, &starttime);
    if (OSAL_RETURN_OK != ret)
    {
        return OSAL_RETURN_FAIL;
    }

    (void)get_state(handle, &current_state);
    if (current_state == state)
    {
        return OSAL_RETURN_OK;
    }

    while ((time_period * 1000000) > elapsedtime_ns)
    {
        ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_PRECISION, &timenow);
        if (OSAL_RETURN_OK != ret)
        {
            return OSAL_RETURN_FAIL;
        }

        ret = R_OSAL_ClockTimeCalculateTimeDifference(&timenow, &starttime, &elapsedtime_ns);
        if (OSAL_RETURN_OK != ret)
        {
            return OSAL_RETURN_FAIL;
        }

        if (elapsedtime_ns > (time_period * 1000000))
        {
            return OSAL_RETURN_TIME;
        }

        if(((time_period * 1000000) - elapsedtime_ns) > OSAL_PM_WAIT_TIME)
        {
            sleep_time = OSAL_PM_WAIT_TIME;
        }
        else
        {
            sleep_time = (time_period * 1000000) - elapsedtime_ns;
        }

        std::this_thread::sleep_for(std::chrono::nanoseconds(sleep_time));

        (void)get_state(handle, &current_state);
        if (current_state == state)
        {
            return OSAL_RETURN_OK;
        }
    }

    return OSAL_RETURN_TIME;
}


#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_PmGetState")
#endif
e_osal_return_t R_OSAL_PmGetState(osal_device_handle_t handle, e_osal_pm_state_t * const p_state)
{
    // no need to WARN()
    dev_power_domain_t power_domain = OSAL_POWER_DOMAIN_INVALID;
    dev_clock_domain_t clock_domain = OSAL_CLOCK_DOMAIN_INVALID;
    dev_reset_domain_t reset_domain = OSAL_RESET_DOMAIN_INVALID;
    e_osal_pm_state_t local_state;
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }
    if (OSAL_DEVICE_HANDLE_INVALID == handle)
    {
        return OSAL_RETURN_HANDLE;
    }
    if (OSAL_RETURN_OK != device_manager::get().check_handle(handle))
    {
        return OSAL_RETURN_HANDLE;
    }
    if(OSAL_RETURN_OK != find_power_clock_reset_domain(handle, &power_domain, &clock_domain, &reset_domain)) // not found the device name in gs_osal_power_clock_reset_info
    {
        return OSAL_RETURN_UNSUPPORTED_OPERATION;
    }

    if(OSAL_POWER_DOMAIN_INVALID == power_domain || OSAL_CLOCK_DOMAIN_INVALID == clock_domain)
    {
        return OSAL_RETURN_UNSUPPORTED_OPERATION;
    }

    if(OSAL_PM_REQUIRED_STATE_REQUIRED == handle->pm_required_state && OSAL_RESET_DOMAIN_INVALID == reset_domain)
    {
        return OSAL_RETURN_UNSUPPORTED_OPERATION;
    }
    
    if (NULL == p_state)
    {
        return OSAL_RETURN_PAR;
    }
    std::lock_guard<std::mutex> lock(handle->pm_mtx);
    (void)get_state(handle, &local_state);
    *p_state = local_state;
    return OSAL_RETURN_OK;
}


#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_PmSetPolicy")
#endif
e_osal_return_t R_OSAL_PmSetPolicy(osal_device_handle_t handle, e_osal_pm_policy_t policy, bool applyImmediate)
{
    (void)applyImmediate; /* unused */
    dev_power_domain_t power_domain = OSAL_POWER_DOMAIN_INVALID;
    dev_clock_domain_t clock_domain = OSAL_CLOCK_DOMAIN_INVALID;
    dev_reset_domain_t reset_domain = OSAL_RESET_DOMAIN_INVALID;
    // no need to WARN()
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }
    if (OSAL_DEVICE_HANDLE_INVALID == handle)
    {
        return OSAL_RETURN_HANDLE;
    }
    if (OSAL_RETURN_OK != device_manager::get().check_handle(handle))
    {
        return OSAL_RETURN_HANDLE;
    }
    if((OSAL_PM_POLICY_PG != policy) && (OSAL_PM_POLICY_CG != policy) && (OSAL_PM_POLICY_HP != policy))
    {
        return OSAL_RETURN_PAR;
    }
    if(OSAL_RETURN_OK != find_power_clock_reset_domain(handle, &power_domain, &clock_domain, &reset_domain)) // not found the device name in gs_osal_power_clock_reset_info
    {
        return OSAL_RETURN_UNSUPPORTED_OPERATION;
    }

    if(OSAL_POWER_DOMAIN_INVALID == power_domain || OSAL_CLOCK_DOMAIN_INVALID == clock_domain)
    {
        return OSAL_RETURN_UNSUPPORTED_OPERATION;
    }

    std::lock_guard<std::mutex> lock(handle->pm_mtx);
    handle->pm_policy = policy;

    if (OSAL_PM_REQUIRED_STATE_RELEASED == handle->pm_required_state)
    {
        switch (policy)
        {
            case OSAL_PM_POLICY_PG :
                if(OSAL_POWER_DOMAIN_SHARED == power_domain)
                {
                    handle->pm_state = OSAL_PM_STATE_CG;
                }
                else /* OSAL_POWER_DOMAIN_NOT_SHARED. The OSAL_POWER_DOMAIN_INVALID have already been checked*/
                {
                    handle->pm_state = OSAL_PM_STATE_PG;
                }
                break;

            case OSAL_PM_POLICY_CG :
                handle->pm_state = OSAL_PM_STATE_CG;
                break;

            case OSAL_PM_POLICY_HP :
                handle->pm_state = OSAL_PM_STATE_ENABLED;
                break;

            default :
                return OSAL_RETURN_PAR;
        }

        if (OSAL_PM_STATE_INVALID == handle->pm_lowest_intermediate_state)
        {
            handle->pm_lowest_intermediate_state = handle->pm_state;
        }
        else if (handle->pm_state < handle->pm_lowest_intermediate_state)
        {
            handle->pm_lowest_intermediate_state = handle->pm_state;
        }
        else
        {
            /* do nothing */
        }
    }

    return OSAL_RETURN_OK;
}


#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_PmGetPolicy")
#endif
e_osal_return_t R_OSAL_PmGetPolicy(osal_device_handle_t handle, e_osal_pm_policy_t * const p_policy)
{
    // no need to WARN()
    dev_power_domain_t power_domain = OSAL_POWER_DOMAIN_INVALID;
    dev_clock_domain_t clock_domain = OSAL_CLOCK_DOMAIN_INVALID;
    dev_reset_domain_t reset_domain = OSAL_RESET_DOMAIN_INVALID;
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }
    if (OSAL_DEVICE_HANDLE_INVALID == handle)
    {
        return OSAL_RETURN_HANDLE;
    }
    if (OSAL_RETURN_OK != device_manager::get().check_handle(handle))
    {
        return OSAL_RETURN_HANDLE;
    }
    if(OSAL_RETURN_OK != find_power_clock_reset_domain(handle, &power_domain, &clock_domain, &reset_domain)) // not found the device name in gs_osal_power_clock_reset_info
    {
        return OSAL_RETURN_UNSUPPORTED_OPERATION;
    }

    if(OSAL_POWER_DOMAIN_INVALID == power_domain || OSAL_CLOCK_DOMAIN_INVALID == clock_domain)
    {
        return OSAL_RETURN_UNSUPPORTED_OPERATION;
    }
    if (NULL == p_policy)
    {
        return OSAL_RETURN_PAR;
    }
    std::lock_guard<std::mutex> lock(handle->pm_mtx);
    *p_policy = handle->pm_policy;
    return OSAL_RETURN_OK;
}


#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_PmSetResetState")
#endif
e_osal_return_t R_OSAL_PmSetResetState(osal_device_handle_t handle, e_osal_pm_reset_state_t state)
{
    // no need to WARN()
    dev_power_domain_t power_domain = OSAL_POWER_DOMAIN_INVALID;
    dev_clock_domain_t clock_domain = OSAL_CLOCK_DOMAIN_INVALID;
    dev_reset_domain_t reset_domain = OSAL_RESET_DOMAIN_INVALID;
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }
    if (OSAL_DEVICE_HANDLE_INVALID == handle)
    {
        return OSAL_RETURN_HANDLE;
    }
    if (OSAL_RETURN_OK != device_manager::get().check_handle(handle))
    {
        return OSAL_RETURN_HANDLE;
    }
    if(OSAL_RETURN_OK != find_power_clock_reset_domain(handle, &power_domain, &clock_domain, &reset_domain)) // not found the device name in gs_osal_power_clock_reset_info
    {
        return OSAL_RETURN_UNSUPPORTED_OPERATION;
    }

    if(OSAL_POWER_DOMAIN_INVALID == power_domain || OSAL_CLOCK_DOMAIN_INVALID == clock_domain || OSAL_RESET_DOMAIN_INVALID == reset_domain)
    {
        return OSAL_RETURN_UNSUPPORTED_OPERATION;
    }

    if ((OSAL_PM_RESET_STATE_APPLIED != state) &&
            (OSAL_PM_RESET_STATE_RELEASED != state))
    {
        return OSAL_RETURN_PAR;
    }

    std::lock_guard<std::mutex> lock(handle->pm_mtx);
    handle->pm_reset_state = state;

    if (OSAL_PM_REQUIRED_STATE_REQUIRED == handle->pm_required_state)
    {
        if (OSAL_PM_RESET_STATE_APPLIED == handle->pm_reset_state)
        {
            handle->pm_state = OSAL_PM_STATE_RESET;
        }
        else if (OSAL_PM_RESET_STATE_RELEASED == handle->pm_reset_state)
        {
            handle->pm_state = OSAL_PM_STATE_READY;
        }
        else
        {
            return OSAL_RETURN_FAIL;
        }
    }

    return OSAL_RETURN_OK;
}


#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_PmGetResetState")
#endif
e_osal_return_t R_OSAL_PmGetResetState(osal_device_handle_t handle, e_osal_pm_reset_state_t * const p_state)
{
    // no need to WARN()
    dev_power_domain_t power_domain = OSAL_POWER_DOMAIN_INVALID;
    dev_clock_domain_t clock_domain = OSAL_CLOCK_DOMAIN_INVALID;
    dev_reset_domain_t reset_domain = OSAL_RESET_DOMAIN_INVALID;
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }
    if (OSAL_DEVICE_HANDLE_INVALID == handle)
    {
        return OSAL_RETURN_HANDLE;
    }
    if (OSAL_RETURN_OK != device_manager::get().check_handle(handle))
    {
        return OSAL_RETURN_HANDLE;
    }
    if(OSAL_RETURN_OK != find_power_clock_reset_domain(handle, &power_domain, &clock_domain, &reset_domain)) // not found the device name in gs_osal_power_clock_reset_info
    {
        return OSAL_RETURN_UNSUPPORTED_OPERATION;
    }

    if(OSAL_POWER_DOMAIN_INVALID == power_domain || OSAL_CLOCK_DOMAIN_INVALID == clock_domain || OSAL_RESET_DOMAIN_INVALID == reset_domain)
    {
        return OSAL_RETURN_UNSUPPORTED_OPERATION;
    }
    if (NULL == p_state)
    {
        return OSAL_RETURN_PAR;
    }
    std::lock_guard<std::mutex> lock(handle->pm_mtx);
    *p_state = handle->pm_reset_state;
    return OSAL_RETURN_OK;
}

e_osal_return_t R_OSAL_PmSetPostClock(osal_device_handle_t device, bool clock_enable, uint32_t clock_divider)
{
    (void) clock_enable;
    dev_power_domain_t power_domain = OSAL_POWER_DOMAIN_INVALID;
    dev_clock_domain_t clock_domain = OSAL_CLOCK_DOMAIN_INVALID;
    dev_reset_domain_t reset_domain = OSAL_RESET_DOMAIN_INVALID;

    // no need to WARN()
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }
    if (OSAL_DEVICE_HANDLE_INVALID == device)
    {
        return OSAL_RETURN_HANDLE;
    }
    if (OSAL_RETURN_OK != device_manager::get().check_handle(device))
    {
        return OSAL_RETURN_HANDLE;
    }
    if(OSAL_RETURN_OK != find_power_clock_reset_domain(device, &power_domain, &clock_domain, &reset_domain)) // not found the device name in gs_osal_power_clock_reset_info
    {
        return OSAL_RETURN_UNSUPPORTED_OPERATION;
    }

    if(OSAL_POWER_DOMAIN_INVALID == power_domain || OSAL_CLOCK_DOMAIN_INVALID == clock_domain)
    {
        return OSAL_RETURN_UNSUPPORTED_OPERATION;
    }
    if (64U <= clock_divider)
    {
        return OSAL_RETURN_PAR;
    }

    std::lock_guard<std::mutex> lock(device->pm_mtx);

    if ((0 != device->dev.get_type().compare(0, 2, "fbc")) &&
        (0 != device->dev.get_type().compare(0, 2, "fba")))
    {
        return OSAL_RETURN_UNSUPPORTED_OPERATION;
    }

    return OSAL_RETURN_OK;
}

e_osal_return_t R_OSAL_PmGetLowestIntermediatePowerState(osal_device_handle_t handle, e_osal_pm_state_t * const p_state)
{
    // no need to WARN()
    dev_power_domain_t power_domain = OSAL_POWER_DOMAIN_INVALID;
    dev_clock_domain_t clock_domain = OSAL_CLOCK_DOMAIN_INVALID;
    dev_reset_domain_t reset_domain = OSAL_RESET_DOMAIN_INVALID;

    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }
    if (OSAL_DEVICE_HANDLE_INVALID == handle)
    {
        return OSAL_RETURN_HANDLE;
    }
    if (OSAL_RETURN_OK != device_manager::get().check_handle(handle))
    {
        return OSAL_RETURN_HANDLE;
    }

    if(OSAL_RETURN_OK != find_power_clock_reset_domain(handle, &power_domain, &clock_domain, &reset_domain)) // not found the device name in gs_osal_power_clock_reset_info
    {
        return OSAL_RETURN_UNSUPPORTED_OPERATION;
    }

    if(OSAL_POWER_DOMAIN_INVALID == power_domain || OSAL_CLOCK_DOMAIN_INVALID == clock_domain)
    {
        return OSAL_RETURN_UNSUPPORTED_OPERATION;
    }

    if (NULL == p_state)
    {
        return OSAL_RETURN_PAR;
    }

    std::lock_guard<std::mutex> lock(handle->pm_mtx);

    *p_state = handle->pm_lowest_intermediate_state;
    return OSAL_RETURN_OK;
}
/**
 * @}
 */
