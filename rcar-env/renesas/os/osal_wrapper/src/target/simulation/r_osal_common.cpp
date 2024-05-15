/**
 * \addtogroup osal_wrapper
 * @{
 */

/**
 * @todo Clean up code and check for Coding Standard Compliance and rework Init concept for whole OSAL.
 */

#include "rcar-xos/osal/r_osal.h"
#include "target/simulation/soc_abstraction/soc_init.hpp"
#include "target/simulation/resource_managers/cond_var_manager.hpp"
#include "target/simulation/resource_managers/mq_manager.hpp"
#include "target/simulation/resource_managers/mutex_manager.hpp"
#include "target/simulation/resource_managers/thread_manager.hpp"
#include "target/simulation/resource_managers/device_manager.hpp"
#include "target/simulation/r_osal_common.hpp"

std::map<std::string, std::map<std::int8_t, std::uint8_t>> map_axi_id;
osal_axi_bus_id_t axi_id_max = 0; // current maximum value of axi id
extern std::multimap<osal_memory_region_idx_t, std::unique_ptr<st_osal_memory_handle>> g_osal_mmngr;

bool osal_internal_initialize_axi_id_table()
{
    bool osal_ret = true;
    std::uint8_t current_count = 2;
    std::vector<int8_t> utlb;
    const char      str_invalid[8] = "invalid";
    const char      str_mmipa[8]   = "mm(ipa)";
    axi_id_max = 1; // axi_id = 0 for "invalid" and 1 for "mm(ipa)"

    /* Reverse axi_id = OSAL_AXI_BUS_ID_INVALID and axi_id = OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY for "invalid" and "mm(ipa)"*/
    map_axi_id[str_invalid][UTLB_ID_INVALID] = OSAL_AXI_BUS_ID_INVALID;
    map_axi_id[str_mmipa][UTLB_ID_INVALID] = OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY;

    std::for_each(dev_cfg::devices().begin(), dev_cfg::devices().end(),
        [&utlb, str_invalid, str_mmipa](const Osal_Device & device) {
            if (device.get_busdomain() != std::string(str_invalid) && device.get_busdomain() != std::string(str_mmipa))
            {
                (void)device.get_utlb(utlb);
                for (auto it : utlb)
                {
                    map_axi_id[device.get_busdomain()][(int8_t)it] = OSAL_AXI_BUS_ID_INVALID;
                }
            }
        });
    if(osal_ret)
    {
        for(std::map<std::string,std::map<std::int8_t,std::uint8_t> >::const_iterator ptr = map_axi_id.begin(); ptr != map_axi_id.end(); ptr++)
        {
            if (ptr->first != std::string(str_invalid) && ptr->first != std::string(str_mmipa))
            {
                for(std::map<std::int8_t,std::uint8_t>::const_iterator eptr= ptr->second.begin(); eptr!=ptr->second.end(); eptr++)
                {
                    map_axi_id[ptr->first][eptr->first ] = current_count;
                    axi_id_max = current_count;
                    current_count++;
                }
            }
        }
    }

    return osal_ret;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_Initialize")
#endif
e_osal_return_t R_OSAL_Initialize(void)
{  
    if (r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if (!r_soc_init())
    {
        return OSAL_RETURN_FAIL;
    }
    if (!osal_internal_initialize_axi_id_table())
    {
        return OSAL_RETURN_FAIL;
    }

   device_manager::get().initialize_device_init_state();

    return OSAL_RETURN_OK;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_Deinitialize")
#endif
e_osal_return_t R_OSAL_Deinitialize(void)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if(!(thread_manager::get().isHandleEmpty())) return OSAL_RETURN_BUSY;
    if(!(mq_manager::get().isHandleEmpty())) return OSAL_RETURN_BUSY;
    if(!(cond_var_manager::get().isHandleEmpty())) return OSAL_RETURN_BUSY;
    if(!(mutex_manager::get().isHandleEmpty())) return OSAL_RETURN_BUSY;
    if(!(device_manager::get().isHandleEmpty())) return OSAL_RETURN_BUSY;

    for (auto i = g_osal_mmngr.begin(); i != g_osal_mmngr.end(); i++)
    {
        if (OSAL_MMNGR_MANAGER_OPENED == i->second->status)
        {
            (void)R_OSAL_MmngrClose(i->second->mmngr_handle);
        }
    }

    g_osal_mmngr.clear();

    if(!r_soc_deinit())
    {
        return OSAL_RETURN_FAIL;
    }
    return OSAL_RETURN_OK;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_GetVersion")
#endif
extern "C" const st_osal_version_t * R_OSAL_GetVersion(void)
{
    static const st_osal_version_t version = {
        {OSAL_VERSION_MAJOR, OSAL_VERSION_MINOR, OSAL_VERSION_PATCH},
        {_OSAL_WRAPPER_VERSION_MAJOR, _OSAL_WRAPPER_VERSION_MINOR, _OSAL_WRAPPER_VERSION_PATCH}};
    return &version;
}
/**
 * @}
 */
