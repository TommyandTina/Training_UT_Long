/** \addtogroup soc_abstraction 
 *  @{
 */

#ifndef DEV_CFG_HPP
#define DEV_CFG_HPP

#include "rcar-xos/osal/r_osal_def.h"
#include "rcar-xos/osal/r_osal_types.h"

#include <array>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <mutex>

#if defined(RCAR_V3M2)
#define DEVICES_MAX 16
#elif defined(RCAR_V3H1)
#define DEVICES_MAX 25
#elif defined(RCAR_V3H2)
#define DEVICES_MAX 35
#elif defined(RCAR_V3U)
#define DEVICES_MAX 55
#elif defined(RCAR_V4H2) /* only V4H2 */
#define DEVICES_MAX 39
#elif defined(RCAR_V4H)  /* only V4H1 */
#define DEVICES_MAX 38
#elif defined(RCAR_V4M)
#define DEVICES_MAX 33
#else
#error                                                                                                                 \
    "unknown SoC. Only V3M2 and V3Hx.x are supported. Please set RCAR_V3M2, RCAR_V3H1, RCAR_V3H2, RCAR_V3U, RCAR_V4H or RCAR_V4M defintions."
#endif

#define UTLB_ID_INVALID (-2)
class Osal_Device
{
private:
    /** @todo identify device by id, type, range and number of irq_channels only. software hirarchy must be changed! */
    std::string                m_id;
    std::string                m_type;
    std::string                m_busdomain;
    uintptr_t                  m_base;
    size_t                     m_range;
    std::vector<std::uint32_t> m_irq_num;
    std::vector<std::int8_t>   m_utlb;
    mutable std::mutex         m_mtx;

public:
    // constructors
    Osal_Device()
    {
    }
    Osal_Device(const std::string & dev_id, const std::string & dev_type, const std::string & dev_busdomain,
                std::vector<std::int8_t> utlb, uintptr_t dev_base, size_t dev_range,
                std::vector<std::uint32_t> dev_irq_num = {})
        : m_id(dev_id), m_type(dev_type), m_busdomain(dev_busdomain), m_base(dev_base), m_range(dev_range),
          m_irq_num(dev_irq_num), m_utlb(utlb)
    {
    }

    // copy constructor
    Osal_Device(const Osal_Device & dev)
        : m_id(dev.m_id), m_type(dev.m_type), m_busdomain(dev.m_busdomain), m_base(dev.m_base), m_range(dev.m_range),
          m_irq_num(dev.m_irq_num), m_utlb(dev.m_utlb)
    {
    }

    void operator=(const Osal_Device & dev)
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        m_id        = dev.m_id;
        m_type      = dev.m_type;
        m_busdomain = dev.m_busdomain;
        m_base      = dev.m_base;
        m_range     = dev.m_range;
        m_irq_num   = dev.m_irq_num;
        m_utlb      = dev.m_utlb;
    }

    // getter functions
    const std::string & get_id(void) const
    {
        return m_id;
    }
    const std::string & get_type(void) const
    {
        return m_type;
    }

    const std::string & get_busdomain(void) const
    {
        return m_busdomain;
    }

    const std::vector<std::uint32_t> & get_irq_num_list(void) const
    {
        return m_irq_num;
    }

    uintptr_t get_base(void) const
    {
        return m_base;
    }
    size_t get_range(void) const
    {
        return m_range;
    }
    size_t get_irq_channels(void) const
    {
        return m_irq_num.size();
    }

    bool get_spi(uint64_t channel, uint32_t & spi) const
    {
        std::lock_guard<std::mutex> lock(m_mtx);

        if (channel >= m_irq_num.size())
        {
            return false;
        }
        try
        {
            spi = m_irq_num.at(channel);
        }
        catch (std::exception const & exc)
        {
            // if you get here, there is an implementation error!
            std::cout << "[FATAL_ERROR]accessing out of range irq channel. Did you configure device correctly?"
                      << exc.what() << std::endl;
            exit(-1);
        }
        return true;
    }

    bool get_channel(uint32_t spi, uint64_t & channel) const
    {
        uint32_t temp;
        
        std::lock_guard<std::mutex> lock(m_mtx);
        
        for (uint64_t i = 0; i < m_irq_num.size(); i++)
        {
            if (get_spi(i, temp) && temp == spi)
            {
                channel = i;
                return true;
            }
        }
        return false;
    }

    bool get_utlb(std::vector<std::int8_t> & utlb) const
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        
        utlb = m_utlb;
        if (m_utlb[0] != UTLB_ID_INVALID)
        {
            return true;
        }
        else /* utlb is UTLB_ID_INVALID */
        {
            return false;
        }
    }

    std::size_t get_num_utlb(void) const
    {
        std::lock_guard<std::mutex> lock(m_mtx);

        if (m_utlb[0] != UTLB_ID_INVALID)
        {
            return m_utlb.size();
        }
        else /* device doesn't support utlb */
        {
            return 0;
        }
    }
};

class dev_cfg
{
public:
    using DeviceArray = std::array<Osal_Device, DEVICES_MAX>;
    using BusDomainList = std::list<std::string>;

private:
    const static DeviceArray m_devices;
    BusDomainList     m_busdomains;

public:

    dev_cfg()
    {
        for (auto dev : m_devices)
        {
            m_busdomains.push_back(dev.get_busdomain());
        }

        // remove consecutive (adjacent) duplicates
        m_busdomains.unique();
        // sort followed by unique, to remove all duplicates
        m_busdomains.sort();
        m_busdomains.unique();

        m_busdomains.push_front("mm(ipa)"); /* Main Memory string. See OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY */
        m_busdomains.push_front("invalid"); /* Invalid IDX string. OSAL_AXI_BUS_ID_INVALID */
    }

    static bool find_device(const std::string & dev_id, Osal_Device & P_device)
    {
        for (Osal_Device i : m_devices)
        {
            if (i.get_id() == dev_id)
            {
                P_device = i;
                return true;
            }
        }
        return false;
    }

    static std::vector<Osal_Device> getListofDevices(const std::string & dev_type)
    {
        std::vector<Osal_Device> dev_list;
        for (Osal_Device i : m_devices)
        {
            if (i.get_type() == dev_type)
            {
                dev_list.push_back(i);
            }
        }
        return dev_list;
    }

    std::string available_devices(const std::string & dev_type, std::size_t & p_count)
    {
        std::stringstream dev_id;
        unsigned int      count = 0;
        for (Osal_Device i : m_devices)
        {
            if (i.get_type() == dev_type)
            {
                count++;
                dev_id << i.get_id() << std::endl;
            }
        }
        p_count = count;
        return dev_id.str();
    }

    static size_t size()
    {
        return m_devices.size();
    }

    static constexpr const DeviceArray & devices()
    {
        return m_devices;
    }

    osal_axi_bus_id_t axi_id(const std::string & bus_name) const
    {
        osal_axi_bus_id_t bus_id = OSAL_AXI_BUS_ID_INVALID;
        size_t            idx    = 0;
        for (auto bus : m_busdomains)
        {
            if (bus == bus_name)
            {
                bus_id = idx;
            }
            idx++;
        }
        return bus_id;
    }
    
    osal_axi_bus_id_t axi_id(const Osal_Device & dev) const
    {
        return axi_id(dev.get_busdomain());
    }

    const BusDomainList & bus_domain_list() const
    {
        return m_busdomains;
    }
};

#endif
/** @} */
