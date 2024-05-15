/**
 * \addtogroup osal_wrapper
 * @{
 */

// system includes
#include <algorithm>
#include <future>
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <mutex>
#ifdef RCAR_XOS_HAVE_PTHREAD
#include <limits.h>
#include <pthread.h>
#endif // RCAR_XOS_HAVE_PTHREAD

// project includes
#include "rcar-xos/osal/r_osal.h"
#include "target/simulation/resource_managers/device_manager.hpp"
#include "target/simulation/soc_abstraction/generic_soc.hpp"
#include "target/simulation/r_osal_common.hpp"
// file scope variable
std::vector<std::thread::id> runningISR;
std::mutex g_mtx;

#if defined __WIN32__
#define OSAL_SCHED_PRIORITY SCHED_OTHER
#else
#define OSAL_SCHED_PRIORITY SCHED_FIFO
#endif

// IrqControllerInterface for Osal Internal to check Irq condition
class OsalIrqControllerInterface
{
protected:
    std::map<uint32_t, p_osal_isr_func_t>             m_IrqHandler;
    std::map<uint32_t, bool>                          m_EnabledSPI;
    std::mutex                                        m_mtx;

public:
    OsalIrqControllerInterface() {}
    virtual ~OsalIrqControllerInterface() {}

    static OsalIrqControllerInterface & get(void)
    {
        // c++11 makes sure static variables are initialized only once even on concurrent calls.
        // see c++ standard section 6.7/4 "If control enters the declaration concurrently while the variable is being
        // initialized, the concurrent execution shall wait for completion of the initialization."
        static OsalIrqControllerInterface instance;
        return instance;
    }

    template<typename IrqHandler>
    void registerIrqHandler(uint32_t spi, p_osal_isr_func_t in_func, IrqHandler&& irqHandler)
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        m_IrqHandler[spi] = in_func;

        GenericSoc::getInstance().gic().registerIrqHandler(spi, irqHandler);
    }

    void removeIrqHandler(uint32_t spi)
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        if (m_IrqHandler.find(spi) == m_IrqHandler.end())
        {
            // std::clog << __PRETTY_FUNCTION__ << " Not in list spi=" << spi << std::endl;
            return;
        }
        m_IrqHandler.erase(spi);

        GenericSoc::getInstance().gic().removeIrqHandler(spi);
    }

    virtual bool enableIrq(uint32_t spi)
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        m_EnabledSPI[spi] = true;

        if (!GenericSoc::getInstance().gic().enableIrq(spi))
        {
            return false; 
        }

        return true;
    }

    virtual bool disableIrq(uint32_t spi)
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        m_EnabledSPI[spi] = false;

        if (!GenericSoc::getInstance().gic().disableIrq(spi))
        {
            return false;
        }

        return true;
    }

    virtual bool isIrqEnabled(uint32_t spi)
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        return m_EnabledSPI[spi];
    }

    virtual bool isIrqHandlerRegistered(uint32_t spi)
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        if (m_IrqHandler.find(spi) != m_IrqHandler.end())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    virtual bool isIrqBusy(uint32_t spi)
    {
        if ((true == this->isIrqEnabled(spi)) ||
            (true == this->isIrqHandlerRegistered(spi)))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    virtual bool isIrqHandlerEqual(uint32_t spi, p_osal_isr_func_t in_func)
    {
        if (m_IrqHandler[spi] == in_func)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

//
#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_InterruptRegisterIsr")
#endif
e_osal_return_t R_OSAL_InterruptRegisterIsr(osal_device_handle_t handle, uint64_t irq_channel,
                                            e_osal_interrupt_priority_t priority_value,
                                            p_osal_isr_func_t irqHandlerFunction, void * irq_arg)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if (NULL == handle)
    {
        return OSAL_RETURN_HANDLE;
    }

    if (OSAL_RETURN_OK != device_manager::get().check_handle(handle))
    {
        return OSAL_RETURN_HANDLE;
    }

    if ((NULL == irqHandlerFunction)||
             (OSAL_INTERRUPT_PRIORITY_LOWEST > priority_value) ||
             (OSAL_INTERRUPT_PRIORITY_HIGHEST < priority_value))
    {
        return OSAL_RETURN_PAR;
    }
    
    std::lock_guard<std::mutex> lock(handle->int_mtx);

    if((size_t)0 == handle->dev.get_irq_channels())
    {
        return OSAL_RETURN_HANDLE;
    }

    uint32_t spi;
    if (!handle->dev.get_spi(irq_channel, spi))
    {
        return OSAL_RETURN_PAR;
    }

    if (true == OsalIrqControllerInterface::get().isIrqBusy(spi))
    {
        return OSAL_RETURN_BUSY;
    }

    // wrapper task around the actual callback function
    std::function<void(uint32_t)> callback([=](uint32_t spi) {
        (void)spi; /* unused */
        g_mtx.lock();
        runningISR.push_back(std::this_thread::get_id());
        g_mtx.unlock();
#ifdef RCAR_XOS_HAVE_PTHREAD
            sched_param           sch;
            const static auto get_pthread_param = [](void) {
                sched_param sch;
                int         policy;

                if (0 != pthread_getschedparam(pthread_self(), &policy, &sch))
                {
                    throw std::runtime_error("failed to get pthread scheduling parameters");
                }

                int max_prio = sched_get_priority_max(OSAL_SCHED_PRIORITY);
                if (-1 == max_prio)
                {
                    throw std::runtime_error("failed to get pthread minimum scheduling priority");
                }
                return std::make_tuple(sch, OSAL_SCHED_PRIORITY, max_prio - OSAL_INTERRUPT_PRIORITY_HIGHEST);
            };

            static const std::tuple<sched_param, int, int> pthread_param = get_pthread_param();

            sch                = std::get<0>(pthread_param);
            sch.sched_priority = priority_value + std::get<2>(pthread_param);

            // if (pthread_setschedprio(pthread_self(), sp_config->priority - std::get<2>(pthread_param)) != 0)   //for
            // some reason MinGW doesn't defnine this function
            if (pthread_setschedparam(pthread_self(), std::get<1>(pthread_param), &sch) != 0)
            {
                //Ubuntu doesn't allow non root users to change scheduling parameteers
                OSAL_DEBUG("WARNING: failed to set interrupt priority. Interrupt priority will be ignored.");
                //throw std::runtime_error("failed to set interrupt priority");
            }
            std::string thread_name = "INT:" + handle->dev.get_id() + "ch: " + std::to_string(irq_channel);
            if (pthread_setname_np(pthread_self(), thread_name.c_str()) != 0)
            {
                //Ubuntu doesn't allow non root users to change thread names
                // throw std::runtime_error("failed to set task name");
            }
#endif // RCAR_XOS_HAVE_PTHREAD
        irqHandlerFunction(handle, irq_channel, irq_arg);
        g_mtx.lock();
        auto pos = std::find(std::begin(runningISR), std::end(runningISR), std::this_thread::get_id());
        if (pos != std::end(runningISR))
        {
            runningISR.erase(pos);
        }
        g_mtx.unlock();
    });

    g_mtx.lock();
    handle->irq_use[spi] = true;
    OsalIrqControllerInterface::get().registerIrqHandler(spi, irqHandlerFunction, std::move(callback));
    g_mtx.unlock();

    return OSAL_RETURN_OK;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_InterruptUnregisterIsr")
#endif
e_osal_return_t R_OSAL_InterruptUnregisterIsr(osal_device_handle_t handle, uint64_t irq_channel,
                                              p_osal_isr_func_t irqHandlerFunction)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if (NULL == handle)
    {
        return OSAL_RETURN_HANDLE;
    }

    if (OSAL_RETURN_OK != device_manager::get().check_handle(handle))
    {
        return OSAL_RETURN_HANDLE;
    }
    
    if (NULL == irqHandlerFunction)
    {
        return OSAL_RETURN_PAR;
    }

    uint32_t spi;
    std::lock_guard<std::mutex> lock(handle->int_mtx);

    if((size_t)0 == handle->dev.get_irq_channels())
    {
        return OSAL_RETURN_HANDLE;
    }

    if (!handle->dev.get_spi(irq_channel, spi))
    {
        return OSAL_RETURN_PAR;
    }
    if (false == OsalIrqControllerInterface::get().isIrqHandlerRegistered(spi))
    {
        return OSAL_RETURN_PAR;
    }
    if (!(OsalIrqControllerInterface::get().isIrqHandlerEqual(spi,irqHandlerFunction)))
    {
        return OSAL_RETURN_PAR;
    }

    if (true == OsalIrqControllerInterface::get().isIrqEnabled(spi))
    {
        return OSAL_RETURN_BUSY;
    }

    g_mtx.lock();
    auto callback_id = std::find(std::begin(runningISR), std::end(runningISR), std::this_thread::get_id());
    if (callback_id != std::end(runningISR))
    {
        runningISR.erase(callback_id);
    }
    else
    {
        while(!runningISR.empty())
        {
            g_mtx.unlock();
            e_osal_return_t osal_ret = R_OSAL_ThreadSleepForTimePeriod(1U);
            if (OSAL_RETURN_OK != osal_ret)
            {
                return osal_ret;
            }
            g_mtx.lock();
        }
    }

    handle->irq_use[spi] = false;
    OsalIrqControllerInterface::get().removeIrqHandler(spi);
    g_mtx.unlock();

    return OSAL_RETURN_OK;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_InterruptEnableIsr")
#endif
e_osal_return_t R_OSAL_InterruptEnableIsr(osal_device_handle_t handle, uint64_t irq_channel)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if (NULL == handle)
    {
        return OSAL_RETURN_HANDLE;
    }

    if (OSAL_RETURN_OK != device_manager::get().check_handle(handle))
    {
        return OSAL_RETURN_HANDLE;
    }

    uint32_t spi;
    std::lock_guard<std::mutex> lock(handle->int_mtx);

    if((size_t)0 == handle->dev.get_irq_channels())
    {
        return OSAL_RETURN_HANDLE;
    }

    if (!handle->dev.get_spi(irq_channel, spi))
    {
        return OSAL_RETURN_PAR;
    }

    if (false == OsalIrqControllerInterface::get().isIrqHandlerRegistered(spi))
    {
        return OSAL_RETURN_FAIL;
    }

    if (true == OsalIrqControllerInterface::get().isIrqEnabled(spi))
    {
        return OSAL_RETURN_OK;
    }

    if (!OsalIrqControllerInterface::get().enableIrq(spi))
    {
        return OSAL_RETURN_FAIL;
    }

    return OSAL_RETURN_OK;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_InterruptDisableIsr")
#endif
e_osal_return_t R_OSAL_InterruptDisableIsr(osal_device_handle_t handle, uint64_t irq_channel)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if (NULL == handle)
    {
        return OSAL_RETURN_HANDLE;
    }

    if (OSAL_RETURN_OK != device_manager::get().check_handle(handle))
    {
        return OSAL_RETURN_HANDLE;
    }

    uint32_t spi;
    std::lock_guard<std::mutex> lock(handle->int_mtx);

    if((size_t)0 == handle->dev.get_irq_channels())
    {
        return OSAL_RETURN_HANDLE;
    }

    if (!handle->dev.get_spi(irq_channel, spi))
    {
        return OSAL_RETURN_PAR;
    }

    if (false == OsalIrqControllerInterface::get().isIrqHandlerRegistered(spi))
    {
        return OSAL_RETURN_FAIL;
    }

    if (false == OsalIrqControllerInterface::get().isIrqEnabled(spi))
    {
        return OSAL_RETURN_OK;
    }

    if (!OsalIrqControllerInterface::get().disableIrq(spi))
    {
        return OSAL_RETURN_FAIL;
    }

    return OSAL_RETURN_OK;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_InterruptIsISRContext")
#endif
e_osal_return_t R_OSAL_InterruptIsISRContext(bool * const p_is_isr)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if (NULL == p_is_isr)
    {
        return OSAL_RETURN_PAR;
    }
    g_mtx.lock();
    if (std::end(runningISR) == std::find(std::begin(runningISR), std::end(runningISR), std::this_thread::get_id()))
    {
        *p_is_isr = false;
    }
    else
    {
        *p_is_isr = true;
    }
    g_mtx.unlock();
    return OSAL_RETURN_OK;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_InterruptSetIsrPriority")
#endif
e_osal_return_t R_OSAL_InterruptSetIsrPriority(osal_device_handle_t handle, uint64_t irq_channel,
                                               e_osal_interrupt_priority_t priority_value)
{
    (void)priority_value; /* unused */

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

    if ( (priority_value < OSAL_INTERRUPT_PRIORITY_LOWEST) || (OSAL_INTERRUPT_PRIORITY_HIGHEST < priority_value) )
    {
        return OSAL_RETURN_PAR;
    }
    
    std::lock_guard<std::mutex> lock(handle->int_mtx);

    if((size_t)0 == handle->dev.get_irq_channels())
    {
        return OSAL_RETURN_HANDLE;
    }

    if (irq_channel >= handle->dev.get_irq_channels())
    {
        return OSAL_RETURN_PAR;
    }
    OSAL_DEBUG("[WARNING]: \"" << __func__ << "\" is ineffictive in SIL environment.");
    return OSAL_RETURN_OK;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_InterruptGetNumOfIrqChannels")
#endif
e_osal_return_t R_OSAL_InterruptGetNumOfIrqChannels(osal_device_handle_t handle, size_t * const p_numOfChannels)
{
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
    
    if (NULL == p_numOfChannels)
    {
        return OSAL_RETURN_PAR;
    }

    std::lock_guard<std::mutex> lock(handle->int_mtx);

    *p_numOfChannels = handle->dev.get_irq_channels();
    return OSAL_RETURN_OK;
}
/**
 * @}
 */
