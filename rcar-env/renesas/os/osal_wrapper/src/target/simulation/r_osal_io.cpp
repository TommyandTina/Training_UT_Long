/**
 * \addtogroup osal_wrapper
 * @{
 */

// project includes
#include "rcar-xos/osal/r_osal.h"
#include "target/simulation/resource_managers/device_manager.hpp"
#include "target/simulation/soc_abstraction/generic_soc.hpp"
#include "target/simulation/r_osal_common.hpp"

// system includes
#include <string>
#include <regex>
#include <cstring>

void osal_internal_get_axi_id_list_for_device_name(const char* p_device_name, osal_axi_bus_id_t* p_array,
                                                 size_t* const p_num_of_written_axi_ids, bool* match_device_name);
extern std::map<std::string, std::map<std::int8_t, std::uint8_t>> map_axi_id;
extern osal_axi_bus_id_t axi_id_max;
// templates to avoid code duplication
template <typename T> e_osal_return_t IoReadOnce(osal_device_handle_t handle, uintptr_t offset, T * p_data)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if (nullptr == handle)
    {
        return OSAL_RETURN_HANDLE;
    }

    if (OSAL_RETURN_OK != device_manager::get().check_handle(handle))
    {
        return OSAL_RETURN_HANDLE;
    }
    
    if (nullptr == p_data)
    {
        return OSAL_RETURN_PAR;
    }

    if ((offset + sizeof(T)) > handle->dev.get_range())
    {
        return OSAL_RETURN_PAR; // prevent access to other devices
    }

	if (0U != (offset & (sizeof(T) - 1U)))
    {
        return OSAL_RETURN_PAR;
    }

    if (!GenericSoc::getInstance().readReg(handle->axi_id ,handle->dev.get_base() + offset, *p_data))
    {
        return OSAL_RETURN_IO;
    }
    return OSAL_RETURN_OK;
}

template <typename T> e_osal_return_t IoWriteOnce(osal_device_handle_t handle, uintptr_t offset, T data)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if (nullptr == handle)
    {
        return OSAL_RETURN_HANDLE;
    }

    if (OSAL_RETURN_OK != device_manager::get().check_handle(handle))
    {
        return OSAL_RETURN_HANDLE;
    }
    
    if ((offset + sizeof(T)) > handle->dev.get_range())
    {
        return OSAL_RETURN_PAR; // prevent access to other devices
    }

	if (0U != (offset & (sizeof(T) - 1U)))
    {
        return OSAL_RETURN_PAR;
    }

    if (!GenericSoc::getInstance().writeReg(handle->axi_id, handle->dev.get_base() + offset, data))
    {
        return OSAL_RETURN_IO;
    }
    return OSAL_RETURN_OK;
}

template <typename T>
e_osal_return_t IoReadMultiple(osal_device_handle_t handle, uintptr_t offset, T * p_data, size_t size)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if (nullptr == handle)
    {
        return OSAL_RETURN_HANDLE;
    }

    if (OSAL_RETURN_OK != device_manager::get().check_handle(handle))
    {
        return OSAL_RETURN_HANDLE;
    }
    
    if (nullptr == p_data)
    {
        return OSAL_RETURN_PAR;
    }

    const size_t step = sizeof(T);

    if (0 != (size % step))
    {
        // size is not a multiple of registers!
        return OSAL_RETURN_PAR;
    }
    else if (0U != (offset & (step - 1U)))
    {
        return OSAL_RETURN_PAR;
    }

    if ((offset + size) > handle->dev.get_range())
    {
        return OSAL_RETURN_PAR; // prevent access to other devices
    }
    for (size_t i = 0; i < size / step; i++)
    {
        if (!GenericSoc::getInstance().readReg(handle->axi_id, handle->dev.get_base() + offset + i * step, *(p_data + i)))
        {
            return OSAL_RETURN_IO;
        }
    }

    return OSAL_RETURN_OK;
}

template <typename T>
e_osal_return_t IoWriteMultiple(osal_device_handle_t handle, uintptr_t offset, const T * p_data, size_t size)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if (nullptr == handle)
    {
        return OSAL_RETURN_HANDLE;
    }

    if (OSAL_RETURN_OK != device_manager::get().check_handle(handle))
    {
        return OSAL_RETURN_HANDLE;
    }
    
    if (nullptr == p_data)
    {
        return OSAL_RETURN_PAR;
    }

    const size_t step = sizeof(T);

    if (0 != (size % step))
    {
        // size is not a multiple of registers!
        return OSAL_RETURN_PAR;
    }
    else if (0U != (offset & (step - 1U)))
    {
        return OSAL_RETURN_PAR;
    }

    if ((offset + size) > handle->dev.get_range())
    {
        return OSAL_RETURN_PAR; // prevent access to other devices
    }
    for (size_t i = 0; i < size / step; i++)
    {
        if (!GenericSoc::getInstance().writeReg(handle->axi_id, handle->dev.get_base() + offset + i * step, *(p_data + i)))
        {
            return OSAL_RETURN_IO;
        }
    }

    return OSAL_RETURN_OK;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_IoGetNumOfDevices")
#endif
e_osal_return_t R_OSAL_IoGetNumOfDevices(const char * device_type_name, size_t * const p_numOfDevices)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if(nullptr == device_type_name)
    {
        return OSAL_RETURN_PAR;
    }

    if(OSAL_DEVICE_TYPE_NAME_MAX_LENGTH < strlen(device_type_name))
    {
        return OSAL_RETURN_PAR;
    }
    if (nullptr == p_numOfDevices)
    {
        return OSAL_RETURN_PAR;
    }
    const std::string device_type_str(device_type_name);
    size_t            numDevices = 0;
    std::for_each(dev_cfg::devices().begin(), dev_cfg::devices().end(),
                  [device_type_str, &numDevices](const Osal_Device & device) {
                      if (device.get_type() == device_type_str)
                      {
                          numDevices++;
                      }
                  });
    *p_numOfDevices = numDevices;
    return OSAL_RETURN_OK;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_IoGetDeviceList")
#endif
e_osal_return_t R_OSAL_IoGetDeviceList(const char * device_type_name, char * p_buffer, size_t buffer_size,
                                       size_t * const p_num_of_byte)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if ((nullptr == p_num_of_byte) || (nullptr == device_type_name))
    {
        return OSAL_RETURN_PAR;
    }
    if(OSAL_DEVICE_TYPE_NAME_MAX_LENGTH < strlen(device_type_name))
    {
        return OSAL_RETURN_PAR;
    }
    std::size_t requiredBufferSize = 1; // set to 1 to leave space to null termination
    const std::string device_type_str(device_type_name);
    std::for_each(dev_cfg::devices().begin(), dev_cfg::devices().end(),
                  [&requiredBufferSize, device_type_str](const Osal_Device & device) {
                      if (device.get_type() == device_type_str)
                      {
                          requiredBufferSize += device.get_id().size() + 1; // str.length + NewLine
                      }
                  });
    if (requiredBufferSize == 1)
    {
        return OSAL_RETURN_UNKNOWN_NAME;
    }

    requiredBufferSize--; // substrct last unnecessary '\n' count before '\0'
    
    if ((nullptr == p_buffer) && (0 == buffer_size)) // only request amount of simulators registered
    {
        *p_num_of_byte = requiredBufferSize;
        return OSAL_RETURN_OK_RQST;
    }

    if ((requiredBufferSize > buffer_size) || (nullptr == p_buffer)) // request buffersize or buffer too small
    {
        *p_num_of_byte = 0;
        return OSAL_RETURN_PAR;
    }
    char * ptr = p_buffer;
    std::for_each(dev_cfg::devices().begin(), dev_cfg::devices().end(),
                  [&ptr, device_type_str](const Osal_Device & device) {
                      if (device.get_type() == device_type_str)
                      {
                          memcpy(ptr, device.get_id().c_str(), device.get_id().size() * sizeof(char));
                          ptr += device.get_id().size();
                          *ptr = '\n';
                          ptr++;
                      }
                  });

    ptr--; // move pointer back for last unnecessary '\n' before '\0'
    *ptr           = '\0'; // terminate string
    *p_num_of_byte = requiredBufferSize;
    return OSAL_RETURN_OK;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_IoDeviceOpen")
#endif
e_osal_return_t R_OSAL_IoDeviceOpen(const char * id, osal_device_handle_t * const p_handle)
{
    e_osal_return_t osal_ret;

    if (!r_soc_initialized())
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (nullptr == p_handle)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if (id == nullptr)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        std::lock_guard<std::mutex> lock(device_manager::get().get_mutex());
        osal_ret = device_manager::get().register_new_dev(std::string(id), p_handle);        
    }

    /* Returns an error code other than OSAL_RETURN_BUSY, overwrite p_handle with OSAL_DEVICE_HANDLE_INVALID */
    if((OSAL_RETURN_OK != osal_ret)&&(OSAL_RETURN_BUSY != osal_ret))
    {
        if(NULL != p_handle)
        {
            *p_handle = OSAL_DEVICE_HANDLE_INVALID;
        }
    }
    return osal_ret;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_IoDeviceClose")
#endif
e_osal_return_t R_OSAL_IoDeviceClose(osal_device_handle_t handle)
{
    if (!r_soc_initialized())
    {
        return OSAL_RETURN_STATE;
    }

    if (nullptr == handle)
    {
        return OSAL_RETURN_HANDLE;
    }

    std::lock_guard<std::mutex> lock(device_manager::get().get_mutex());

    if (OSAL_DEVICE_HANDLE_ID != handle->handle_id)
    {
        return OSAL_RETURN_HANDLE;
    }

    if (!handle->int_mtx.try_lock())
    {
        return OSAL_RETURN_BUSY;
    }

    if (!handle->pm_mtx.try_lock())
    {
        handle->int_mtx.unlock();
        return OSAL_RETURN_BUSY;
    }

    if (true != handle->irq_use.empty())
    {
        for (std::map<uint32_t, bool>::iterator i = std::begin(handle->irq_use); i != std::end(handle->irq_use); i++)
        {
            if (true == i->second)
            {
                handle->pm_mtx.unlock();
                handle->int_mtx.unlock();
                return OSAL_RETURN_BUSY;
            }
        }
    }

    handle->handle_id = OSAL_DEVICE_HANDLE_ID_INVALID;

    handle->pm_mtx.unlock();
    handle->int_mtx.unlock();

    return device_manager::get().unregister_dev(handle);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_IoRead8")
#endif
e_osal_return_t R_OSAL_IoRead8(osal_device_handle_t handle, uintptr_t offset, uint8_t * const p_data)
{
    return IoReadOnce(handle, offset, p_data);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_IoRead16")
#endif
e_osal_return_t R_OSAL_IoRead16(osal_device_handle_t handle, uintptr_t offset, uint16_t * const p_data)
{
    return IoReadOnce(handle, offset, p_data);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_IoRead32")
#endif
e_osal_return_t R_OSAL_IoRead32(osal_device_handle_t handle, uintptr_t offset, uint32_t * const p_data)
{
    return IoReadOnce(handle, offset, p_data);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_IoRead64")
#endif
e_osal_return_t R_OSAL_IoRead64(osal_device_handle_t handle, uintptr_t offset, uint64_t * const p_data)
{
    return IoReadOnce(handle, offset, p_data);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_IoWrite8")
#endif
e_osal_return_t R_OSAL_IoWrite8(osal_device_handle_t handle, uintptr_t offset, uint8_t data)
{
    return IoWriteOnce(handle, offset, data);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_IoWrite16")
#endif
e_osal_return_t R_OSAL_IoWrite16(osal_device_handle_t handle, uintptr_t offset, uint16_t data)
{
    return IoWriteOnce(handle, offset, data);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_IoWrite32")
#endif
e_osal_return_t R_OSAL_IoWrite32(osal_device_handle_t handle, uintptr_t offset, uint32_t data)
{
    return IoWriteOnce(handle, offset, data);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_IoWrite64")
#endif
e_osal_return_t R_OSAL_IoWrite64(osal_device_handle_t handle, uintptr_t offset, uint64_t data)
{
    return IoWriteOnce(handle, offset, data);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_IoBlockRead8")
#endif
e_osal_return_t R_OSAL_IoBlockRead8(osal_device_handle_t handle, uintptr_t offset, uint8_t * p_data, size_t size)
{
    return IoReadMultiple(handle, offset, p_data, size);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_IoBlockRead16")
#endif
e_osal_return_t R_OSAL_IoBlockRead16(osal_device_handle_t handle, uintptr_t offset, uint16_t * p_data, size_t size)
{
    return IoReadMultiple(handle, offset, p_data, size);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_IoBlockRead32")
#endif
e_osal_return_t R_OSAL_IoBlockRead32(osal_device_handle_t handle, uintptr_t offset, uint32_t * p_data, size_t size)
{
    return IoReadMultiple(handle, offset, p_data, size);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_IoBlockRead64")
#endif
e_osal_return_t R_OSAL_IoBlockRead64(osal_device_handle_t handle, uintptr_t offset, uint64_t * p_data, size_t size)
{
    return IoReadMultiple(handle, offset, p_data, size);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_IoBlockWrite8")
#endif
e_osal_return_t R_OSAL_IoBlockWrite8(osal_device_handle_t handle, uintptr_t offset, const uint8_t * p_data, size_t size)
{
    return IoWriteMultiple(handle, offset, p_data, size);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_IoBlockWrite16")
#endif
e_osal_return_t R_OSAL_IoBlockWrite16(osal_device_handle_t handle, uintptr_t offset, const uint16_t * p_data,
                                      size_t size)
{
    return IoWriteMultiple(handle, offset, p_data, size);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_IoBlockWrite32")
#endif
e_osal_return_t R_OSAL_IoBlockWrite32(osal_device_handle_t handle, uintptr_t offset, const uint32_t * p_data,
                                      size_t size)
{
    return IoWriteMultiple(handle, offset, p_data, size);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_IoBlockWrite64")
#endif
e_osal_return_t R_OSAL_IoBlockWrite64(osal_device_handle_t handle, uintptr_t offset, const uint64_t * p_data,
                                      size_t size)
{
    return IoWriteMultiple(handle, offset, p_data, size);
}

/*****************************************
 * IO MNGR extension for Memory API
 *****************************************/

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_IoGetDeviceAxiBusId")
#endif
e_osal_return_t R_OSAL_IoGetDeviceAxiBusId(osal_device_handle_t device, osal_axi_bus_id_t * const axi_id)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    bool isInitialized = false;
    size_t numofaxiids = 0;
    bool match_device_name = false;
    Osal_Device osal_dev;
    std::size_t size  = 0;
    osal_axi_bus_id_t* axi_id_list = NULL;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    isInitialized = r_soc_initialized();

    if (false == isInitialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (nullptr == device)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (OSAL_RETURN_OK != device_manager::get().check_handle(device))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        if (!dev_cfg::find_device(device->dev.get_id(), osal_dev))
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else if (nullptr == axi_id)
        {
            osal_ret = OSAL_RETURN_PAR;
        }
        else
        {
            /* Do nothing */
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        size = osal_dev.get_num_utlb();
        axi_id_list = (osal_axi_bus_id_t*)std::malloc(sizeof(osal_axi_bus_id_t) * size);
        if (NULL == axi_id_list)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_internal_get_axi_id_list_for_device_name(device->dev.get_id().c_str(), axi_id_list, &numofaxiids, &match_device_name);
        if (false == match_device_name) // un-support axi bus device
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else
        {
            *axi_id = axi_id_list[0];
        }
    }

    if (NULL != axi_id_list)
    {
        std::free(axi_id_list);
    }

    return osal_ret;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_IoGetAxiBusIdFromDeviceName")
#endif
e_osal_return_t R_OSAL_IoGetAxiBusIdFromDeviceName(const char * device_name, osal_axi_bus_id_t * const axi_id)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    bool isInitialized = false;
    size_t numofaxiids = 0;
    bool match_device_name = false;
    Osal_Device osal_dev;
    std::size_t size  = 0;
    osal_axi_bus_id_t* axi_id_list = NULL;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    isInitialized = r_soc_initialized();

    if (false == isInitialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else
    {
        if (nullptr == axi_id)
        {
            osal_ret = OSAL_RETURN_PAR;
        }
        else if (nullptr == device_name)
        {
            osal_ret = OSAL_RETURN_PAR;
        }
        else if (!dev_cfg::find_device(device_name, osal_dev))
        {
            osal_ret = OSAL_RETURN_UNKNOWN_NAME;
        }
        else
        {
            /* Do nothing */
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        size = osal_dev.get_num_utlb();
        axi_id_list = (osal_axi_bus_id_t*)std::malloc(sizeof(osal_axi_bus_id_t) * size);
        if (NULL == axi_id_list)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_internal_get_axi_id_list_for_device_name(device_name, axi_id_list, &numofaxiids, &match_device_name);
        if (false == match_device_name) // un-support axi bus device
        {
            osal_ret = OSAL_RETURN_UNKNOWN_NAME;
        }
        else
        {
            *axi_id = axi_id_list[0];
        }
    }

    if (NULL != axi_id_list)
    {
        std::free(axi_id_list);
    }

    return osal_ret;
}

void  osal_internal_get_axibusname_from_axiid(osal_axi_bus_id_t axi_id, std::string * str_buffer, 
                                    size_t * const str_buffer_size)
{
    bool is_axi_id_detected = false;
    for (std::map<std::string,std::map<std::int8_t,std::uint8_t> >::const_iterator ptr = map_axi_id.begin(); ptr != map_axi_id.end(); ptr++)
    {
        for (std::map<std::int8_t, std::uint8_t> ::const_iterator eptr = ptr->second.begin(); eptr != ptr->second.end(); eptr++)
        {
            if (eptr->second == axi_id)
            {
                is_axi_id_detected = true;
                if ((OSAL_AXI_BUS_ID_INVALID != axi_id) && (OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY != axi_id))
                {
                    if(OSAL_MAX_UTLB_HAVE_ONE_DEGITS_NUMBER >= eptr->first) // utlb < 10 -> change string of utlb to 2 degits number, ie: utlb = 1 -> "01"
                    {
                        *str_buffer = ptr->first + '_' + '0' + std::to_string(eptr->first);
                    }
                    else
                    {
                        *str_buffer = ptr->first + '_' + std::to_string(eptr->first);
                    }
                }
                else
                {
                    *str_buffer = ptr->first;
                }
                *str_buffer_size = (*str_buffer).size();
                break;
            }
        }
        if (is_axi_id_detected)
        {
            break;
        }
    }
}

std::vector<std::string> osal_internal_format_consecutive_range_utlb(std::string domain_name, std::vector<std::int8_t> v_utlb)
{
    std::vector<std::string> tmp;
    std::vector<std::vector<std::string>> consecutive_utlb_list;
    std::vector<std::string> axi_bus_name;

    auto start_of_list = v_utlb.begin();
    auto end_of_list = v_utlb.end();

    axi_bus_name.push_back(domain_name);
    while (start_of_list != end_of_list)
    {
        auto end_of_consecutive_utlbs = std::adjacent_find(start_of_list, end_of_list, [](const int& v1, const int& v2) {return v2 != v1 + 1;});
        if (end_of_consecutive_utlbs != end_of_list)
        {
            std::advance(end_of_consecutive_utlbs, 1);
        }

        for (auto it = start_of_list; it != end_of_consecutive_utlbs; ++it)
        {
            if (OSAL_MAX_UTLB_HAVE_ONE_DEGITS_NUMBER >= (*it))
            {
                tmp.push_back('0' + std::to_string(*it));
            }
            else
            {
                tmp.push_back(std::to_string(*it));
            }
        }

        consecutive_utlb_list.push_back(tmp);
        start_of_list = end_of_consecutive_utlbs;
        tmp.clear();
    }

    for (size_t i = 0; i < consecutive_utlb_list.size(); i++)
    {
        if(consecutive_utlb_list[i].size() == 1)
        {
            axi_bus_name.push_back(consecutive_utlb_list[i][0]);
        }
        else
        {
            auto first = 0;
            auto last = consecutive_utlb_list[i].size() - 1;
            axi_bus_name.push_back(consecutive_utlb_list[i][first] + '-' + consecutive_utlb_list[i][last]);
        }
    }
    return axi_bus_name;
}

std::vector<std::string> osal_internal_get_axibusname_list(size_t * const p_num_of_byte)
{
    std::vector<std::string> axi_bus_name_components;
    std::vector<std::int8_t> utlb_list;
    std::vector<std::string> axi_bus_name_list;
    std::string tmp_str;
    const char str_invalid[8] = "invalid";
    const char str_mmipa[8]   = "mm(ipa)";
    size_t buffer_size = 0;

    axi_bus_name_list.push_back(str_invalid);
    axi_bus_name_list.push_back(str_mmipa);

    for (std::map<std::string, std::map<std::int8_t, std::uint8_t> >::const_iterator ptr = map_axi_id.begin(); ptr != map_axi_id.end(); ptr++)
    {
        if ((ptr->first != std::string(str_invalid)) && (ptr->first != std::string(str_mmipa)))
        {
            for (std::map<std::int8_t, std::uint8_t> ::const_iterator eptr = ptr->second.begin(); eptr != ptr->second.end(); eptr++)
            {
                utlb_list.push_back(eptr->first); /* Found then add the utlb number of specific domain name */
            }

            axi_bus_name_components = osal_internal_format_consecutive_range_utlb(ptr->first, utlb_list);

            for (size_t i = 0; i < axi_bus_name_components.size(); i++)
            {
                if (i != axi_bus_name_components.size() - 1)
                {
                    tmp_str += axi_bus_name_components[i] + '_';
                }
                else
                {
                    tmp_str += axi_bus_name_components[i];
                }
            }

            axi_bus_name_list.push_back(tmp_str);

            tmp_str.clear();
            axi_bus_name_components.clear();
            utlb_list.clear();
        }
        else
        {
            /* Do nothing */
        }
    }

    for (size_t i = 0; i < axi_bus_name_list.size(); i++)
    {
        buffer_size += (strlen(axi_bus_name_list[i].c_str()) + 1); /* Include '\0' after each axi bus name */
    }

    *p_num_of_byte = buffer_size;

    return axi_bus_name_list;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_IoGetAxiBusNameList")
#endif
e_osal_return_t R_OSAL_IoGetAxiBusNameList(char * p_buffer, size_t buffer_size, size_t * const p_num_of_byte)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t buffer_lenght = 0;
    std::size_t required_buffer_size = 0;
    std::vector<std::string> axi_bus_name_list;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (!r_soc_initialized())
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (nullptr == p_num_of_byte)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if ((nullptr == p_buffer) && (0 != buffer_size))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        axi_bus_name_list = osal_internal_get_axibusname_list(&required_buffer_size);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if ((p_buffer == NULL) && (buffer_size != 0U))
        {
            osal_ret = OSAL_RETURN_PAR;
        }
        else if (p_buffer == NULL)
        {
            *p_num_of_byte = required_buffer_size;
            osal_ret = OSAL_RETURN_OK_RQST;
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            if (buffer_size < required_buffer_size)
            {
                osal_ret = OSAL_RETURN_PAR;
            }
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            for (size_t i = 0; i < axi_bus_name_list.size(); i++)
            {
                memcpy(p_buffer + buffer_lenght, axi_bus_name_list[i].c_str(), strlen(axi_bus_name_list[i].c_str()));
                buffer_lenght += strlen(axi_bus_name_list[i].c_str());
                p_buffer[buffer_lenght] = '\0';
                buffer_lenght++;
            }

            *p_num_of_byte = required_buffer_size;
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_IoGetAxiBusName")
#endif
e_osal_return_t R_OSAL_IoGetAxiBusName(osal_axi_bus_id_t axi_id, char * p_buffer, size_t buffer_size,
                                        size_t * const p_num_of_byte)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    std::string l_buffer;
    std::size_t l_buffer_size = 0;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (!r_soc_initialized())
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (axi_id_max < axi_id) 
    {
        osal_ret = OSAL_RETURN_PAR;

    }
    else if (nullptr == p_num_of_byte)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if ((nullptr == p_buffer) && (0 != buffer_size))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        osal_internal_get_axibusname_from_axiid(axi_id, &l_buffer, &l_buffer_size );
        l_buffer_size += 1; // For null terminated string
    }
    if (OSAL_RETURN_OK == osal_ret)
    {
        if ((NULL != p_buffer) && (buffer_size < l_buffer_size))
        {
            (void)memset((void*)p_buffer, 0, buffer_size);
            osal_ret = OSAL_RETURN_PAR;
        }
        else
        {
            if (NULL == p_buffer)
            {
                osal_ret = OSAL_RETURN_OK_RQST;
            }
            else
            {
                /* Null terminated string will be added when calling c_str() method */
                strcpy(p_buffer, l_buffer.c_str());
            }
            *p_num_of_byte = l_buffer_size;
        }
    }
    else
    {
        /* Do nothing */
    }
    return osal_ret;
}

/***********************************************************************************************************************
* Start of function osal_internal_get_domain_name()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_get_domain_name(const char* p_axi_bus_name, char *p_domain_name)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    bool sil_ret = false;
    std::smatch smatch_res;
    size_t length = 0;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == p_domain_name)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        std::regex regex_expr("(^[0-9a-z]+)_([0-9]{2})");
        std::string tmp_axi_bus_name(p_axi_bus_name);
        sil_ret = std::regex_search(tmp_axi_bus_name, smatch_res, regex_expr);
        if (true == sil_ret)
        {
            length = smatch_res.str().length() - 3;
            (void)strncpy(p_domain_name, &p_axi_bus_name[0], length); /*remove "_[0-9]{2}"*/
            p_domain_name[length] = '\0';
        }
        else
        {
            osal_ret = OSAL_RETURN_UNKNOWN_NAME;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_get_domain_name()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_utlb_parser_sub1()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_utlb_parser_sub1(int32_t **pp_list, size_t *p_index, int32_t num)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int32_t * p_tmp_buffer;
    size_t tmp_index = *p_index;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    p_tmp_buffer = (int32_t *)realloc((void *)(*pp_list), sizeof(int32_t) * (tmp_index + 1U));
    if (NULL == p_tmp_buffer)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        *pp_list = p_tmp_buffer;
        p_tmp_buffer[tmp_index] = num;
        *p_index = tmp_index + 1U;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_utlb_parser_sub1()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_utlb_parser_sub2()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_utlb_parser_sub2(int32_t **pp_list, size_t *p_index, int32_t num)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int32_t * p_tmp_buffer  = *pp_list;
    size_t tmp_index;
    int32_t target_val;
    int32_t current_val;
    int32_t count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == p_index)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else if (0U == *p_index)
    {
        osal_ret = OSAL_RETURN_UNKNOWN_NAME;  /* axi_bus_name is invalid (e.g. ir0-04)*/
    }
    else
    {
        ; /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        tmp_index = *p_index;
        current_val = p_tmp_buffer[tmp_index - 1U];
        target_val = num;
        if (target_val <= current_val)
        {
            osal_ret = OSAL_RETURN_UNKNOWN_NAME;  /* axi_bus_name is invalid (e.g. ir0_04-02)*/
        }
        else
        {
            p_tmp_buffer = (int32_t *)realloc((void *)(*pp_list), sizeof(int32_t) * (tmp_index + (size_t)(target_val - current_val)));
            if (NULL == p_tmp_buffer)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                count = 0;
                while(count < (target_val -  current_val))
                {
                    p_tmp_buffer[tmp_index] = p_tmp_buffer[tmp_index - 1U] + 1;
                    tmp_index++;
                    count++;
                }

                *pp_list = p_tmp_buffer;
                *p_index = tmp_index;
            }
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_utlb_parser_sub2()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_utlb_parser_sub()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_utlb_parser_sub(int32_t **pp_list, size_t *p_index, const char *p_num_str)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int32_t num;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL != p_num_str)
    {
        if (('0' > p_num_str[1]) || ('9' < p_num_str[1]) || ('0' > p_num_str[2]) || ('9' < p_num_str[2]))
        {
            osal_ret = OSAL_RETURN_UNKNOWN_NAME;  /* axi_bus_name is invalid (e.g. ir0_a4, ir0_6d) */
        }
        else
        {
            num = (int32_t)((p_num_str[1] - '0') * 10) + (p_num_str[2] - '0');

            switch (p_num_str[0])
            {
                case '_':
                    osal_ret =  osal_internal_utlb_parser_sub1(pp_list, p_index, num);
                    break;

                case '-':
                    osal_ret =  osal_internal_utlb_parser_sub2(pp_list, p_index, num);
                    break;

                default:
                    osal_ret = OSAL_RETURN_UNKNOWN_NAME;  /* axi_bus_name is invalid (e.g. ir0:04:05)*/
                    break;
            }
        }
    }
    else
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_utlb_parser_sub2()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_utlb_parser()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_utlb_parser(const char* p_axi_bus_name, size_t len, int32_t **pp_list,
                                                       size_t* const p_numOfutlb)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    char tmp_str[4] = "";
    bool sil_ret = false;
    std::smatch smatch_res;
    size_t index = 0;
    size_t offset = 0;
    std::string tmp_axi_bus_name(p_axi_bus_name);

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == p_numOfutlb)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        ; /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        std::regex regex_expr("([_-][0-9]{2})");
        sil_ret = std::regex_search(tmp_axi_bus_name, smatch_res, regex_expr);
        while (true == sil_ret)
        {
            (void)strncpy(tmp_str, &p_axi_bus_name[smatch_res.prefix().length() + offset], (size_t)(smatch_res.str().length()));
            tmp_str[3]='\0';

            osal_ret = osal_internal_utlb_parser_sub(pp_list, &index, tmp_str);

            offset += (smatch_res.prefix().length() + smatch_res.str().length());
            if ((OSAL_RETURN_OK == osal_ret) && (len > offset))
            {
                /* next */
                tmp_axi_bus_name = smatch_res.suffix();
                sil_ret = std::regex_search(tmp_axi_bus_name, smatch_res, regex_expr);
            }
            else
            {
                /* end of loop */
                sil_ret = false;
            }
        }

        *p_numOfutlb = index;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_IoGetAxiIdListForDeviceName()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_add_axiid_to_list()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_add_axiid_to_list(osal_axi_bus_id_t **pp_list_buffer, size_t * p_dst_index,
                                                            size_t count)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_axi_bus_id_t *p_tmp_buffer;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL != pp_list_buffer)
    {
        if (NULL != p_dst_index)
        {
            p_tmp_buffer = (osal_axi_bus_id_t *)realloc((void *)(*pp_list_buffer), sizeof(osal_axi_bus_id_t) * (*p_dst_index + 1U));
            if (NULL == p_tmp_buffer)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                *pp_list_buffer = p_tmp_buffer;
                p_tmp_buffer[*p_dst_index] = count;  /* count is axi_id */
                *p_dst_index = *p_dst_index + 1U;
            }
        }
        else
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_add_axiid_to_list()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_check_axiidlist_for_domainname()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_check_axiidlist_for_domainname(const io_domain_table_t* p_domain,
                                                                         osal_axi_bus_id_t **pp_list_buffer,
                                                                         size_t * const p_num_of_axi_ids)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t match_count = 0;
    size_t dst_index = 0;
    size_t index;
    std::string get_domain_name(p_domain->domain_name);
    bool match_domain_name = false;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    index = 0;

    while ((index < p_domain->utlb_buffer_count) && (OSAL_RETURN_OK == osal_ret))
    {
        for (std::map<std::string,std::map<std::int8_t,std::uint8_t> >::const_iterator ptr = map_axi_id.begin(); ptr != map_axi_id.end(); ptr++)
        {
            if (get_domain_name == ptr->first)
            {
                match_domain_name = true;
                break;
            }
        }
        if (match_domain_name)
        {
            for (std::map<std::int8_t,std::uint8_t>::const_iterator eptr = map_axi_id[get_domain_name].begin(); eptr != map_axi_id[get_domain_name].end(); eptr++)
            {

                if ((int32_t)eptr->first == p_domain->p_utlb_buffer[index])
                {
                    osal_ret = osal_internal_add_axiid_to_list(pp_list_buffer, &dst_index, (size_t)eptr->second);
                    match_count++;
                    break;
                }
            }
        }
        else
        {
            osal_ret = OSAL_RETURN_UNKNOWN_NAME;
        }
        index++;
    }
    if (OSAL_RETURN_OK == osal_ret)
    {
        *p_num_of_axi_ids = match_count;

        if ((0U == match_count) || (match_count < p_domain->utlb_buffer_count))
        {
            osal_ret = OSAL_RETURN_UNKNOWN_NAME;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_IoGetAxiIdListForDeviceName()
***********************************************************************************************************************/

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_IoGetAxiIdListForAxiBusName")
#endif
e_osal_return_t R_OSAL_IoGetAxiIdListForAxiBusName(const char* p_axi_bus_name, osal_axi_bus_id_t* p_array,
                                                size_t array_size, size_t* const p_num_of_written_axi_ids)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    bool isInitialized = false;
    size_t count = 0;
    osal_axi_bus_id_t * p_local_buffer = NULL;
    size_t numofaxiids;
    io_domain_table_t table;
    int32_t * p_utlb_buffer = NULL;
    size_t utlb_buffer_count;
    size_t len;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    isInitialized = r_soc_initialized();

    if (false == isInitialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if ((nullptr == p_axi_bus_name) || (nullptr == p_num_of_written_axi_ids))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if ((nullptr == p_array) && ((size_t)0 != array_size))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        table.domain_name[0] = '\0';

        osal_ret = osal_internal_get_domain_name(p_axi_bus_name, table.domain_name);

        if (OSAL_RETURN_OK == osal_ret)
        {
            len = strlen(p_axi_bus_name);
            osal_ret = osal_internal_utlb_parser(p_axi_bus_name, len, &p_utlb_buffer, &utlb_buffer_count);
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            table.p_utlb_buffer = p_utlb_buffer;
            table.utlb_buffer_count = utlb_buffer_count;

            osal_ret = osal_internal_check_axiidlist_for_domainname(&table, &p_local_buffer, &numofaxiids);
        }
    }

    if ((OSAL_RETURN_OK == osal_ret) && (NULL != p_local_buffer))
    {
        if ((NULL != p_array) && (array_size < numofaxiids))
        {
            osal_ret = OSAL_RETURN_PAR;
        }
        else
        {
            if (NULL == p_array)  /*Already checked && (0U == array_size) */
            {
                osal_ret = OSAL_RETURN_OK_RQST;
            }
            else
            {
                while(count < numofaxiids)
                {
                    p_array[count] = p_local_buffer[count];
                    count++;
                }
            }
            *p_num_of_written_axi_ids = numofaxiids;
        }
    }

    else
    {
        if (NULL != p_num_of_written_axi_ids)
        {
            *p_num_of_written_axi_ids = 0;
        }
    }

    free((void *)p_utlb_buffer); /* Allocated in osal_internal_utlb_parser() */

    return osal_ret;
}

void osal_internal_get_axi_id_list_for_device_name(const char* p_device_name, osal_axi_bus_id_t* p_array,
                                                 size_t* const p_num_of_written_axi_ids, bool* match_device_name)
{
    std::vector<int8_t> get_utlb;
    std::string axi_bus_domain;
    std::string get_device_id(p_device_name);
    size_t match_count = 0;
    bool local_match_device_name = false;
    bool valid_utlb = false;

    for (auto it = dev_cfg::devices().begin(); it != dev_cfg::devices().end(); it ++)
    {
        if (get_device_id == it->get_id())
        {
            valid_utlb = it->get_utlb(get_utlb);
            axi_bus_domain = it->get_busdomain();
            local_match_device_name = true;
            break;
        }
    }

    if ((local_match_device_name) && (valid_utlb))
    {
        for (std::map<std::int8_t,std::uint8_t>::const_iterator eptr= map_axi_id[axi_bus_domain].begin();eptr!= map_axi_id[axi_bus_domain].end(); eptr++)
        {
            for (size_t i = 0; i < get_utlb.size(); i++)
            {
                if (get_utlb[i] == eptr->first)
                {
                    if ((NULL != p_array))
                    {
                        p_array[i] = eptr->second;
                    }
                    else
                    {
                        /* Do nothing */
                    }
                    match_count++;
                }
            }
        }
    }
    *p_num_of_written_axi_ids = match_count;
    *match_device_name = local_match_device_name;

    return;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_IoGetAxiIdListForDeviceHdl")
#endif
e_osal_return_t R_OSAL_IoGetAxiIdListForDeviceHdl(osal_device_handle_t device_hdl, osal_axi_bus_id_t* p_array,
                                                  size_t array_size, size_t* const p_num_of_written_axi_ids)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    bool isInitialized = false;
    size_t numofaxiids = 0;
    bool match_device_name = false;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    isInitialized = r_soc_initialized();

    if (false == isInitialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (nullptr == device_hdl)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (OSAL_RETURN_OK != device_manager::get().check_handle(device_hdl))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if ((nullptr == p_num_of_written_axi_ids) || ((nullptr == p_array) && ((size_t)0 != array_size)))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        osal_internal_get_axi_id_list_for_device_name(device_hdl->dev.get_id().c_str(), p_array, &numofaxiids, &match_device_name);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if ((NULL != p_array) && (array_size < numofaxiids))
        {
            osal_ret = OSAL_RETURN_PAR;
            (void)memset((void*)p_array, 0, array_size);
        }
        else
        {
            if (NULL == p_array)  /* Already checked && (0U == array_size) */
            {
                osal_ret = OSAL_RETURN_OK_RQST;
            }
            else
            {
                /* Do nothing */
            }

            *p_num_of_written_axi_ids = numofaxiids;
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_IoGetAxiIdListForDeviceName")
#endif
e_osal_return_t R_OSAL_IoGetAxiIdListForDeviceName(const char* p_device_name, osal_axi_bus_id_t* p_array,
                                                   size_t array_size, size_t* const p_num_of_written_axi_ids)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    bool isInitialized = false;
    size_t numofaxiids = 0;
    bool match_device_name = false;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    isInitialized = r_soc_initialized();

    if (false == isInitialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if ((nullptr == p_device_name) || (nullptr == p_num_of_written_axi_ids))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if ((nullptr == p_array) && ((size_t)0 != array_size))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        osal_internal_get_axi_id_list_for_device_name(p_device_name, p_array, &numofaxiids, &match_device_name);
    }
    if (OSAL_RETURN_OK == osal_ret)
    {
        if (false == match_device_name)
        {
            osal_ret = OSAL_RETURN_UNKNOWN_NAME;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if ((NULL != p_array) && (array_size < numofaxiids))
        {
            osal_ret = OSAL_RETURN_PAR;
            (void)memset((void*)p_array, 0, array_size);
        }
        else
        {
            if (NULL == p_array)  /* Already checked && (0U == array_size) */
            {
                osal_ret = OSAL_RETURN_OK_RQST;
            }
            else
            {
                /* Do nothing */
            }

            *p_num_of_written_axi_ids = numofaxiids;
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_IoGetNumOfAxiIdsForAxiBusName")
#endif
e_osal_return_t R_OSAL_IoGetNumOfAxiIdsForAxiBusName(const char* p_axi_bus_name, size_t *p_num_of_axi_ids)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    bool isInitialized = false;
    io_domain_table_t table;
    int32_t * p_utlb_buffer = NULL;
    size_t utlb_buffer_count;
    size_t len;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    isInitialized = r_soc_initialized();

    if (false == isInitialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if ((nullptr == p_axi_bus_name) || (nullptr == p_num_of_axi_ids))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        table.domain_name[0] = '\0';

        osal_ret = osal_internal_get_domain_name(p_axi_bus_name, table.domain_name);
        if (OSAL_RETURN_OK == osal_ret)
        {
            len = strlen(p_axi_bus_name);
            osal_ret = osal_internal_utlb_parser(p_axi_bus_name, len, &p_utlb_buffer, &utlb_buffer_count);
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            table.p_utlb_buffer = p_utlb_buffer;
            table.utlb_buffer_count = utlb_buffer_count;

            osal_ret = osal_internal_check_axiidlist_for_domainname(&table, NULL, p_num_of_axi_ids);
        }

        free((void *)p_utlb_buffer); /* Allocated in osal_internal_utlb_parser() */
    }
    return osal_ret;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_IoGetNumOfAxiIdsForDeviceHdl")
#endif
e_osal_return_t R_OSAL_IoGetNumOfAxiIdsForDeviceHdl(osal_device_handle_t device_hdl, size_t* const p_num_of_axi_ids)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    bool isInitialized = false;
    Osal_Device osal_dev;
    dev_cfg devcfg;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    isInitialized = r_soc_initialized();

    if (false == isInitialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (nullptr == device_hdl)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (OSAL_RETURN_OK != device_manager::get().check_handle(device_hdl))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (nullptr == p_num_of_axi_ids)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        if (!dev_cfg::find_device(device_hdl->dev.get_id().c_str(), osal_dev))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            std::size_t  size = osal_dev.get_num_utlb();
            *p_num_of_axi_ids = size;
        }
    }
    return osal_ret;
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:R_OSAL_IoGetNumOfAxiIdsForDeviceName")
#endif
e_osal_return_t R_OSAL_IoGetNumOfAxiIdsForDeviceName(const char* p_device_name, size_t* const p_num_of_axi_ids)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    bool isInitialized = false;
    Osal_Device osal_dev;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    isInitialized = r_soc_initialized();

    if (false == isInitialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if ((nullptr == p_device_name) || (nullptr == p_num_of_axi_ids))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        if (!dev_cfg::find_device(p_device_name, osal_dev))
        {
            osal_ret = OSAL_RETURN_UNKNOWN_NAME;
        }
        else
        {
            std::size_t  size = osal_dev.get_num_utlb();
            *p_num_of_axi_ids = size;
        }
    }
    return osal_ret;
}
/**
 * @}
 */
