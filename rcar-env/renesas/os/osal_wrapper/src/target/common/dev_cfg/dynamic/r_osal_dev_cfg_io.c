/***********************************************************************************************************************
* Copyright [2022-2024] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
*
* The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
* and/or its licensors ("Renesas") and subject to statutory and contractual protections.
*
* Unless otherwise expressly agreed in writing between Renesas and you: 1) you may not use, copy, modify, distribute,
* display, or perform the contents; 2) you may not use any name or mark of Renesas for advertising or publicity
* purposes or in connection with your use of the contents; 3) RENESAS MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE
* SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
* WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND
* NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR CONSEQUENTIAL DAMAGES,
* INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF CONTRACT OR TORT, ARISING
* OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents included in this file may
* be subject to different terms.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name :    r_osal_dev_cfg_io.c
* Version :      1.1.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Device Configuration Functions for IO Manager
***********************************************************************************************************************/

/***********************************************************************************************************************
 * includes
***********************************************************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "target/common/dev_cfg/dynamic/r_osal_dev_cfg.h"

/***********************************************************************************************************************
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global variable
***********************************************************************************************************************/
/***********************************************************************************************************************
 * @var     gs_io_init_state
 * @brief   Status of IO manager.
***********************************************************************************************************************/
OSAL_STATIC e_osal_init_status_t gs_io_init_state = OSAL_MANAGER_STATE_UNINITIALIZED;

/***********************************************************************************************************************
 * @var     gs_inner_io_mtx
 * @brief   Mutex of IO manager.
***********************************************************************************************************************/
OSAL_STATIC osal_os_mutex_t gs_inner_io_mtx;

/***********************************************************************************************************************
 * @var     gs_io_handle_count
 * @brief   Number of the opened handles.
***********************************************************************************************************************/
OSAL_STATIC int64_t gs_io_handle_count = 0;

/***********************************************************************************************************************
 * Prototypes
***********************************************************************************************************************/
/***********************************************************************************************************************
 * @brief            Get the AXI ID list from domain name.
 * @param[in]        p_domain                 A pointer to the domain information for axi API.
 * @param[in]        pst_axi_bus_name_info    A pointer to the domain name information for axi API.
 * @param[in,out]    pp_list_buffer           AXI Bus ID type assigned to the device.
 * @param[in,out]    p_num_of_axi_ids         To set the pointer that contains the number of available AXI IDs 
 *                                            for that AXI Bus Name
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_UNKNOWN_NAME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_check_axiidlist_for_domainname(const st_osal_io_domain_table_t* p_domain, const st_osal_axi_bus_name_info_t* pst_axi_bus_name_info, osal_axi_bus_id_t** pp_list_buffer, size_t* const p_num_of_axi_ids);

/***********************************************************************************************************************
 * @brief            Get the AXI ID list from device name.
 * @param[in]        p_device_name            A pointer to device information for axi API.
 * @param[in]        pst_axi_bus_name_info    A pointer to device name information for axi API.
 * @param[in,out]    pp_list_buffer           AXI Bus ID type assigned to the device.
 * @param[in,out]    p_num_of_axi_ids         To set the pointer that contains the number of available AXI IDs 
 *                                            for that AXI Bus Name
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_UNKNOWN_NAME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_check_axiidlist_for_devicename(const char* p_device_name, const st_osal_axi_bus_name_info_t* pst_axi_bus_name_info, osal_axi_bus_id_t** pp_list_buffer, size_t* const p_num_of_axi_ids);

/***********************************************************************************************************************
 * @brief            Allocate memory and add AXI ID.
 * @param[in,out]    pp_list_buffer    AXI Bus ID type assigned to the device.
 * @param[in,out]    p_dst_index       To set the pointer that contains the number of available AXI IDs 
 * @param[in]        count             Counter of condition variables used.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_add_axiid_to_list(osal_axi_bus_id_t** pp_list_buffer, size_t* p_dst_index, size_t count);

/***********************************************************************************************************************
 * @brief            Free the resources when the R_OSAL_DCFG_IoDeviceOpen() error.
 * @param[in]        handle    A handle of opened device.
 * @return           #void
***********************************************************************************************************************/
OSAL_STATIC void r_osal_dcfg_io_open_error(osal_device_handle_t handle);

/***********************************************************************************************************************
 * @brief            Sub procedure of R_OSAL_DCFG_IoDeviceClose().
 * @param[in,out]    handle    A handle of opened device.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_BUSY
 * @retval           OSAL_RETURN_HANDLE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_close(osal_device_handle_t handle);

/***********************************************************************************************************************
 * @brief            Common function for destroy the mutex.
 * @param[in]        p_mutex    A pointer to the mutex information.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_mutex_destroy(osal_os_mutex_t* p_mutex);

/***********************************************************************************************************************
 * @brief            Common function for lock the mutex for time period.
 * @param[in]        mutex    A handle of mutex information.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_mutex_lock_for_time_period(osal_os_mutex_t mutex);

/***********************************************************************************************************************
 * @brief            Common function for lock the mutex until timestamp.
 * @param[in]        mutex            A handle of mutex information.
 * @param[in]        p_abs_timeout    Wait on a condition variable with timeout until time stamp.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_BUSY
 * @retval           OSAL_RETURN_HANDLE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_mutex_lock_until_time_stamp(osal_os_mutex_t mutex, const st_osal_time_t* p_abs_timeout);

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_001
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoGetInitStatus()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_init_status_t R_OSAL_DCFG_IoGetInitStatus(void)
{
    return gs_io_init_state;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoGetInitStatus()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_001]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_002
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoSetInitStatus()
***********************************************************************************************************************/
void R_OSAL_DCFG_IoSetInitStatus(e_osal_init_status_t status)
{
    gs_io_init_state = status;
    return;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoSetInitStatus()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_002]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_003
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoControlInit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoControlInit(void)
{
    e_osal_return_t     osal_ret;
    e_osal_return_t     mutex_ret;
    osal_os_mutexattr_t mutex_attr = NULL;

    /* Initialize the global variables */
    osal_ret = R_OSAL_OS_MutexInitAttr(&mutex_attr);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_MutexCreate(mutex_attr, &gs_inner_io_mtx);
    }

    if (NULL != mutex_attr)
    {
        mutex_ret = R_OSAL_OS_MutexDeinitAttr(mutex_attr);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_IoInit();
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        gs_io_handle_count = 0;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoControlInit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_003]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_004
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoControlDeinit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoControlDeinit(void)
{
    e_osal_return_t osal_ret;

    osal_ret = R_OSAL_OS_IoDeinit();

    /* Destroy the mutex */
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_MutexTryLock(gs_inner_io_mtx);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* No need to check return code because the trylock was successful */
            (void)R_OSAL_OS_MutexUnlock(gs_inner_io_mtx);

            osal_ret = r_osal_dcfg_io_mutex_destroy(&gs_inner_io_mtx);
            if (OSAL_RETURN_OK != osal_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
        }
    }

    /* Initialize the global variables */
    gs_io_handle_count = 0;

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoControlDeinit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_004]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_005
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoCheckBusy()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoCheckBusy(void)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;

    osal_ret = r_osal_dcfg_io_mutex_lock_for_time_period(gs_inner_io_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* If exist the opened handle, return BUSY */
        if (0 != gs_io_handle_count)
        {
            osal_ret = OSAL_RETURN_BUSY;
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_io_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoCheckBusy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_005]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_006
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoGetNumOfDevices()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoGetNumOfDevices(const char* p_deviceType, size_t* const p_numOfDevices)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    if (OSAL_DEVICE_TYPE_NAME_MAX_LENGTH < strlen(p_deviceType))
    {
        osal_ret = OSAL_RETURN_PAR;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_IoGetNumOfDevices(p_deviceType, p_numOfDevices);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoGetNumOfDevices()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_006]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_007
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoGetDeviceList()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoGetDeviceList(const char* p_device_type_name, char* p_buffer, size_t buffer_size, size_t* const p_num_of_byte)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    if (OSAL_DEVICE_TYPE_NAME_MAX_LENGTH < strlen(p_device_type_name))
    {
        osal_ret = OSAL_RETURN_PAR;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_IoGetDeviceList(p_device_type_name, p_buffer, buffer_size, p_num_of_byte);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoGetDeviceList()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_007]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_008
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoDeviceOpen()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoDeviceOpen(const char* p_id, osal_device_handle_t* const p_handle)
{
    e_osal_return_t         osal_ret = OSAL_RETURN_OK;
    e_osal_return_t         mutex_ret;
    osal_os_mutexattr_t     mutex_attr = NULL;
    osal_device_handle_t    local_handle;

    /* PRQA S 0316 1 # Required cast on system, not modifiable. */
    local_handle = (osal_device_handle_t)R_OSAL_OS_Malloc(sizeof(st_osal_device_control_t));
    if (NULL == local_handle)
    {
        osal_ret = OSAL_RETURN_MEM;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        local_handle->dev_fd = -1;
        local_handle->dev_post_fd = -1;

        /* PRQA S 0317 1 # Required cast on system, not modifiable. */
        local_handle->dev_id = R_OSAL_OS_Malloc((size_t)(sizeof(char) * (uint32_t)(PATH_MAX)));
        if (NULL == local_handle->dev_id)
        {
            osal_ret = OSAL_RETURN_MEM;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_MutexInitAttr(&mutex_attr);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_MutexCreate(mutex_attr, &local_handle->int_mtx);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_MutexCreate(mutex_attr, &local_handle->pm_mtx);
    }

    if (NULL != mutex_attr)
    {
        mutex_ret = R_OSAL_OS_MutexDeinitAttr(mutex_attr);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_dcfg_io_mutex_lock_for_time_period(gs_inner_io_mtx);

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_OS_IoDeviceOpen(p_id, &local_handle);

            if (OSAL_RETURN_OK == osal_ret)
            {
                (void)strncpy(local_handle->dev_id, p_id, PATH_MAX);
                local_handle->handle_id = OSAL_IO_HANDLE_ID;
                /* PRQA S 2812 1 # The pointer and size are checked in R_OSAL_OS_IoDeviceOpen. */
                *p_handle = local_handle;
                gs_io_handle_count++;
            }

            mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_io_mtx);
            if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = mutex_ret;
            }
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    if (OSAL_RETURN_OK != osal_ret)
    {
        r_osal_dcfg_io_open_error(local_handle);

        if (OSAL_RETURN_BUSY != osal_ret)
        {
            *p_handle = OSAL_DEVICE_HANDLE_INVALID;
        }
    }

    return osal_ret;
/* PRQA S 9104,9107 2 # The initialization function has already multiple sub-functions to reduce complexity.
   We have decided that further division is not desirable from perspective of maintainability and readability. */
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoDeviceOpen()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_008]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_009
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoDeviceClose()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoDeviceClose(osal_device_handle_t handle)
{
    e_osal_return_t osal_ret;
    e_osal_return_t mutex_ret;
    st_osal_time_t  abs_timeout;

    osal_ret = R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod(OSAL_INNER_TIMEOUT, &abs_timeout);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Lock the IO manager mutex */
        osal_ret = r_osal_dcfg_io_mutex_lock_until_time_stamp(gs_inner_io_mtx, &abs_timeout);

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* Lock the interrupt manager mutex */
            osal_ret = r_osal_dcfg_io_mutex_lock_until_time_stamp(handle->int_mtx, &abs_timeout);

            if (OSAL_RETURN_OK == osal_ret)
            {
                /* Lock the power manager mutex */
                osal_ret = r_osal_dcfg_io_mutex_lock_until_time_stamp(handle->pm_mtx, &abs_timeout);

                if (OSAL_RETURN_OK == osal_ret)
                {
                    osal_ret = r_osal_dcfg_io_close(handle);

                    mutex_ret = R_OSAL_OS_MutexUnlock(handle->pm_mtx);
                    if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
                    {
                        osal_ret = mutex_ret;
                    }
                }

                mutex_ret = R_OSAL_OS_MutexUnlock(handle->int_mtx);
                if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
                {
                    osal_ret = mutex_ret;
                }
            } /* end of if (OSAL_RETURN_OK == osal_ret) */

            if (OSAL_RETURN_OK == osal_ret)
            {
                gs_io_handle_count--;
            }

            mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_io_mtx);
            if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = mutex_ret;
            }
        } /* end of if (OSAL_RETURN_OK == osal_ret) osal_ret is return value from r_osal_dcfg_io_mutex_lock_until_time_stamp */
    } /* end of if (OSAL_RETURN_OK == osal_ret) osal_ret is return value from R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod */

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_dcfg_io_mutex_destroy(&handle->pm_mtx);

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_dcfg_io_mutex_destroy(&handle->int_mtx);
        }

        /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
        R_OSAL_OS_Free((void *)handle->dev_id);
        handle->dev_id = NULL;

        /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
        R_OSAL_OS_Free((void *)handle);
    }

    return osal_ret;
/* PRQA S 9110 1 # It is a series of processes and is necessary so as not to reduce the readability based on design. */
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoDeviceClose()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_009]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_010
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoRead()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoRead(osal_device_handle_t const handle, uintptr_t offset, size_t size, size_t unit_size, void* p_data)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /* Check the handle and parameter */
    if ((OSAL_IO_HANDLE_ID != handle->handle_id) ||
        (NULL == handle->base_addr))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (handle->len < (offset + size))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_IoRead(handle, offset, size, unit_size, p_data);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoRead()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_010]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_011
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoWrite()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoWrite(osal_device_handle_t const handle, uintptr_t offset, size_t size, size_t unit_size, const void* p_data)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /* Check the handle and parameter */
    if ((OSAL_IO_HANDLE_ID != handle->handle_id) ||
        (NULL == handle->base_addr))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (handle->len < (offset + size))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_IoWrite(handle, offset, size, unit_size, p_data);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoWrite()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_011]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_012
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoGetDeviceAxiBusId()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_DCFG_IoGetDeviceAxiBusId(osal_device_handle_t device, osal_axi_bus_id_t* const p_axi_id)
{
    e_osal_return_t                         osal_ret;
    e_osal_return_t                         mutex_ret;
    osal_axi_bus_id_t*                      p_local_buffer = NULL;
    size_t                                  numofaxiids;
    st_osal_os_io_get_device_axi_bus_id_t   st_param;

    osal_ret = r_osal_dcfg_io_mutex_lock_for_time_period(gs_inner_io_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check the handle */
        if (OSAL_IO_HANDLE_ID != device->handle_id)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }

        /* Get the information from OSIF */
        if (OSAL_RETURN_OK == osal_ret)
        {
            /* PRQA S 0315 1 # The 2nd arguments must be (void *) but no side effects in this case. */
            osal_ret = R_OSAL_OS_IoGetAxiBusInformation(OSAL_OS_IO_GET_DEVICE_AXI_BUS_ID, &st_param);
        }

        /* Convert the OSIF information to the output parameter */
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_dcfg_io_check_axiidlist_for_devicename(device->dev_id, st_param.pst_axi_bus_name_info, &p_local_buffer, &numofaxiids);

            if ((OSAL_RETURN_OK == osal_ret) && (0U != numofaxiids))
            {
                *p_axi_id = p_local_buffer[0];
            }
            else
            {
                *p_axi_id = OSAL_AXI_BUS_ID_INVALID;

                if (OSAL_RETURN_UNKNOWN_NAME == osal_ret)
                {
                    osal_ret = OSAL_RETURN_HANDLE; /* device does not support AXI bus */
                }
            }
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_io_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    }

    /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
    R_OSAL_OS_Free((void *)p_local_buffer);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoGetDeviceAxiBusId()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_012]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_013
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoGetAxiBusIdFromDeviceName()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoGetAxiBusIdFromDeviceName(const char* p_device_name, osal_axi_bus_id_t* const p_axi_id)
{
    e_osal_return_t                                 osal_ret;
    e_osal_return_t                                 mutex_ret;
    osal_axi_bus_id_t*                              p_local_buffer = NULL;
    size_t                                          numofaxiids;
    st_osal_os_io_get_axi_bus_id_from_device_name_t st_param;

    osal_ret = r_osal_dcfg_io_mutex_lock_for_time_period(gs_inner_io_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Get the information from OSIF */
        /* PRQA S 0315 1 # The 2nd arguments must be (void *) but no side effects in this case. */
        osal_ret = R_OSAL_OS_IoGetAxiBusInformation(OSAL_OS_IO_GET_AXI_BUS_ID_FROM_DEVICE_NAME, &st_param);

        /* Convert the OSIF information to the output parameter */
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_dcfg_io_check_axiidlist_for_devicename(p_device_name, st_param.pst_axi_bus_name_info, &p_local_buffer, &numofaxiids);

            if ((OSAL_RETURN_OK == osal_ret) && (0U != numofaxiids))
            {
                *p_axi_id = p_local_buffer[0];
            }
            else
            {
                *p_axi_id = OSAL_AXI_BUS_ID_INVALID;
            }
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_io_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    }

    /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
    R_OSAL_OS_Free((void *)p_local_buffer);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoGetAxiBusIdFromDeviceName()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_013]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_014
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoGetAxiBusNameList()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoGetAxiBusNameList(char* p_buffer, size_t buffer_size, size_t* const p_num_of_byte)
{
    e_osal_return_t                         osal_ret;
    e_osal_return_t                         mutex_ret;
    st_osal_os_io_get_axi_bus_name_list_t   st_param;

    osal_ret = r_osal_dcfg_io_mutex_lock_for_time_period(gs_inner_io_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Get the information from OSIF */
        /* PRQA S 0315 1 # The 2nd arguments must be (void *) but no side effects in this case. */
        osal_ret = R_OSAL_OS_IoGetAxiBusInformation(OSAL_OS_IO_GET_AXI_BUS_NAME_LIST, &st_param);

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* Check the parameter using the information from OSIF */
            if ((NULL != p_buffer) &&
                (buffer_size < st_param.axi_bus_name_list_len))
            {
                osal_ret = OSAL_RETURN_PAR;
            }

            /* Convert the OSIF information to the output parameter */
            if (OSAL_RETURN_OK == osal_ret)
            {
                *p_num_of_byte = st_param.axi_bus_name_list_len;

                if (NULL != p_buffer)
                {
                    /* PRQA S 0315 1 # The 1st, 2nd arguments must be (void *) but no side effects in this case. */
                    (void)memcpy(p_buffer, st_param.p_axi_bus_name_list, st_param.axi_bus_name_list_len);
                }
            }
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_io_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoGetAxiBusNameList()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_014]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_015
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoGetAxiBusName()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoGetAxiBusName(osal_axi_bus_id_t axi_id, char* p_buffer, size_t buffer_size, size_t* const p_num_of_byte)
{
    e_osal_return_t                     osal_ret;
    e_osal_return_t                     mutex_ret;
    size_t                              max_count_axi_id = 0;
    size_t                              local_required_size;
    st_osal_os_io_get_axi_bus_name_t    st_param;

    osal_ret = r_osal_dcfg_io_mutex_lock_for_time_period(gs_inner_io_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Get the information from OSIF */
        /* PRQA S 0315 1 # The 2nd arguments must be (void *) but no side effects in this case. */
        osal_ret = R_OSAL_OS_IoGetAxiBusInformation(OSAL_OS_IO_GET_AXI_BUS_NAME, &st_param);

        /* Check the parameter using the information from OSIF */
        if (OSAL_RETURN_OK == osal_ret)
        {
            while (OSAL_AXI_BUS_NAME_LAST != st_param.pst_axi_bus_name_info[max_count_axi_id].utlb)
            {
                max_count_axi_id++;
            }

            if (max_count_axi_id <= axi_id)
            {
                osal_ret = OSAL_RETURN_PAR;
            }
            else
            {
                local_required_size = strlen(st_param.pst_axi_bus_name_info[axi_id].domain_name_utlb) + 1U;
            }

            if ((OSAL_RETURN_OK == osal_ret) && ((NULL != p_buffer) && (buffer_size < local_required_size)))
            {
                osal_ret = OSAL_RETURN_PAR;
            }
            else
            {
                /* Do nothing */
            }
        }

        /* Convert the OSIF information to the output parameter */
        if (OSAL_RETURN_OK == osal_ret)
        {
            *p_num_of_byte = local_required_size;

            if (NULL != p_buffer)
            {
                (void)strncpy(p_buffer, st_param.pst_axi_bus_name_info[axi_id].domain_name_utlb, buffer_size);
            }
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_io_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoGetAxiBusName()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_015]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_016
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoGetNumOfAxiIdsForAxiBusName()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoGetNumOfAxiIdsForAxiBusName(const char* p_axi_bus_name, size_t* p_num_of_axi_ids)
{
    e_osal_return_t                                     osal_ret;
    st_osal_io_domain_table_t                           domain_table;
    st_osal_os_io_get_num_of_axi_ids_for_axi_bus_name_t st_param;

    domain_table.domain_name[0] = '\0';

    st_param.p_axi_bus_name     = p_axi_bus_name;
    st_param.pst_domain_table   = &domain_table;

    /* Get the information from OSIF */
    /* PRQA S 0315 1 # The 2nd arguments must be (void *) but no side effects in this case. */
    osal_ret = R_OSAL_OS_IoGetAxiBusInformation(OSAL_OS_IO_GET_NUM_OF_AXI_IDS_FOR_AXI_BUS_NAME, &st_param);

    /* Convert the OSIF information to the output parameter */
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_dcfg_io_check_axiidlist_for_domainname(st_param.pst_domain_table, st_param.pst_axi_bus_name_info, NULL, p_num_of_axi_ids);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoGetNumOfAxiIdsForAxiBusName()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_016]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_017
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoGetAxiIdListForAxiBusName()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoGetAxiIdListForAxiBusName(const char* p_axi_bus_name, osal_axi_bus_id_t* p_array, size_t array_size, size_t* const p_num_of_written_axi_ids)
{
    e_osal_return_t                                     osal_ret;
    st_osal_io_domain_table_t                           domain_table;
    osal_axi_bus_id_t*                                  p_local_buffer = NULL;
    size_t                                              numofaxiids;
    st_osal_os_io_get_axi_id_list_for_axi_bus_name_t    st_param;

    domain_table.domain_name[0] = '\0';

    st_param.p_axi_bus_name     = p_axi_bus_name;
    st_param.pst_domain_table   = &domain_table;

    /* Get the information from OSIF */
    /* PRQA S 0315 1 # The 2nd arguments must be (void *) but no side effects in this case. */
    osal_ret = R_OSAL_OS_IoGetAxiBusInformation(OSAL_OS_IO_GET_AXI_ID_LIST_FOR_AXI_BUS_NAME, &st_param);

    /* Convert the OSIF information */
    if (OSAL_RETURN_OK == osal_ret)
    {
        *p_num_of_written_axi_ids = 0;
        osal_ret = r_osal_dcfg_io_check_axiidlist_for_domainname(st_param.pst_domain_table, st_param.pst_axi_bus_name_info, &p_local_buffer, &numofaxiids);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check the parameter using the converted information */
        if ((NULL != p_array) &&
            (array_size < numofaxiids))
        {
            osal_ret = OSAL_RETURN_PAR;
        }

        /* The OSIF information to the output parameter */
        if (OSAL_RETURN_OK == osal_ret)
        {
            *p_num_of_written_axi_ids = numofaxiids;

            if (NULL != p_array)
            {
                /* PRQA S 0315 2 # The 1st, 2nd arguments must be (void *) but no side effects in this case. */
                /* PRQA S 2812 1 # The pointer and size are checked in r_osal_dcfg_io_check_axiidlist_for_domainname. */
                (void)memcpy(p_array, p_local_buffer, sizeof(osal_axi_bus_id_t) * numofaxiids);
            }
        }
    }

    /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
    R_OSAL_OS_Free((void *)p_local_buffer);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoGetAxiIdListForAxiBusName()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_017]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_018
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoGetNumOfAxiIdsForDeviceHdl()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_DCFG_IoGetNumOfAxiIdsForDeviceHdl(osal_device_handle_t device_hdl, size_t* const p_num_of_axi_ids)
{
    e_osal_return_t                                     osal_ret;
    e_osal_return_t                                     mutex_ret;
    st_osal_os_io_get_num_of_axi_ids_for_device_hdl_t   st_param;

    osal_ret = r_osal_dcfg_io_mutex_lock_for_time_period(gs_inner_io_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check the handle */
        if (OSAL_IO_HANDLE_ID != device_hdl->handle_id)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }

        /* Get the information from OSIF */
        if (OSAL_RETURN_OK == osal_ret)
        {
            /* PRQA S 0315 1 # The 2nd arguments must be (void *) but no side effects in this case. */
            osal_ret = R_OSAL_OS_IoGetAxiBusInformation(OSAL_OS_IO_GET_NUM_OF_AXI_IDS_FOR_DEVICE_HDL, &st_param);
        }

        /* Convert the OSIF information to the output parameter */
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_dcfg_io_check_axiidlist_for_devicename(device_hdl->dev_id, st_param.pst_axi_bus_name_info, NULL, p_num_of_axi_ids);
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_io_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoGetNumOfAxiIdsForDeviceHdl()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_018]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_019
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoGetAxiIdListForDeviceHdl()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_DCFG_IoGetAxiIdListForDeviceHdl(osal_device_handle_t device_hdl, osal_axi_bus_id_t* p_array, size_t array_size, size_t* const p_num_of_written_axi_ids)
{
    e_osal_return_t                                 osal_ret;
    e_osal_return_t                                 mutex_ret;
    osal_axi_bus_id_t*                              p_local_buffer = NULL;
    size_t                                          numofaxiids;
    st_osal_os_io_get_axi_id_list_for_device_hdl_t  st_param;

    osal_ret = r_osal_dcfg_io_mutex_lock_for_time_period(gs_inner_io_mtx);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check the handle */
        if (OSAL_IO_HANDLE_ID != device_hdl->handle_id)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }

        /* Get the information from OSIF */
        if (OSAL_RETURN_OK == osal_ret)
        {
            /* PRQA S 0315 1 # The 2nd arguments must be (void *) but no side effects in this case. */
            osal_ret = R_OSAL_OS_IoGetAxiBusInformation(OSAL_OS_IO_GET_AXI_ID_LIST_FOR_DEVICE_HDL, &st_param);
        }

        /* Convert the OSIF information */
        if (OSAL_RETURN_OK == osal_ret)
        {
            *p_num_of_written_axi_ids = 0;
            osal_ret = r_osal_dcfg_io_check_axiidlist_for_devicename(device_hdl->dev_id, st_param.pst_axi_bus_name_info, &p_local_buffer, &numofaxiids);
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* Check the parameter using the converted information */
            if ((NULL != p_array) &&
                (array_size < numofaxiids))
            {
                osal_ret = OSAL_RETURN_PAR;
            }

            /* The OSIF information to the output parameter */
            if (OSAL_RETURN_OK == osal_ret)
            {
                *p_num_of_written_axi_ids = numofaxiids;

                if (NULL != p_array)
                {
                    /* PRQA S 0315 1 # The 1st, 2nd arguments must be (void *) but no side effects in this case. */
                    (void)memcpy(p_array, p_local_buffer, sizeof(osal_axi_bus_id_t) * numofaxiids);
                }
            }
        }

        mutex_ret = R_OSAL_OS_MutexUnlock(gs_inner_io_mtx);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    }

    /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
    R_OSAL_OS_Free((void *)p_local_buffer);

    return osal_ret;
/* PRQA S 9104 1 # For better readability this rule is ignored, and this function is not split. */
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoGetAxiIdListForDeviceHdl()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_019]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_020
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoGetNumOfAxiIdsForDeviceName()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoGetNumOfAxiIdsForDeviceName(const char* p_device_name, size_t* const p_num_of_axi_ids)
{
    e_osal_return_t                                     osal_ret;
    st_osal_os_io_get_num_of_axi_ids_for_device_name_t  st_param;

    /* Get the information from OSIF */
    /* PRQA S 0315 1 # The 2nd arguments must be (void *) but no side effects in this case. */
    osal_ret = R_OSAL_OS_IoGetAxiBusInformation(OSAL_OS_IO_GET_NUM_OF_AXI_IDS_FOR_DEVICE_NAME, &st_param);

    /* Convert the OSIF information to the output parameter */
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_dcfg_io_check_axiidlist_for_devicename(p_device_name, st_param.pst_axi_bus_name_info, NULL, p_num_of_axi_ids);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoGetNumOfAxiIdsForDeviceName()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_020]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_021
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoGetAxiIdListForDeviceName()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoGetAxiIdListForDeviceName(const char* p_device_name, osal_axi_bus_id_t* p_array, size_t array_size, size_t* const p_num_of_written_axi_ids)
{
    e_osal_return_t                                 osal_ret;
    osal_axi_bus_id_t*                              p_local_buffer = NULL;
    size_t                                          numofaxiids;
    st_osal_os_io_get_axi_id_list_for_device_name_t st_param;

    /* Get the information from OSIF */
    /* PRQA S 0315 1 # The 2nd arguments must be (void *) but no side effects in this case. */
    osal_ret = R_OSAL_OS_IoGetAxiBusInformation(OSAL_OS_IO_GET_AXI_ID_LIST_FOR_DEVICE_NAME, &st_param);

    /* Convert the OSIF information */
    if (OSAL_RETURN_OK == osal_ret)
    {
        *p_num_of_written_axi_ids = 0;
        osal_ret = r_osal_dcfg_io_check_axiidlist_for_devicename(p_device_name, st_param.pst_axi_bus_name_info, &p_local_buffer, &numofaxiids);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check the parameter using the converted information */
        if ((NULL != p_array) &&
            (array_size < numofaxiids))
        {
            osal_ret = OSAL_RETURN_PAR;
        }

        /* The OSIF information to the output parameter */
        if (OSAL_RETURN_OK == osal_ret)
        {
            *p_num_of_written_axi_ids = numofaxiids;

            if (NULL != p_array)
            {
                /* PRQA S 0315 2 # The 1st, 2nd arguments must be (void *) but no side effects in this case. */
                /* PRQA S 2812 1 # The pointer and size are checked in r_osal_dcfg_io_check_axiidlist_for_devicename. */
                (void)memcpy(p_array, p_local_buffer, sizeof(osal_axi_bus_id_t) * numofaxiids);
            }
        }
    }

    /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
    R_OSAL_OS_Free((void *)p_local_buffer);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoGetAxiIdListForDeviceName()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_021]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_022
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_io_check_axiidlist_for_domainname()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_check_axiidlist_for_domainname(const st_osal_io_domain_table_t* p_domain, const st_osal_axi_bus_name_info_t* pst_axi_bus_name_info, osal_axi_bus_id_t** pp_list_buffer, size_t* const p_num_of_axi_ids)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;
    size_t          index;
    size_t          count;
    size_t          match_count = 0;
    size_t          dst_index = 0;

    index = 0;
    while ((index < p_domain->utlb_buffer_count) && (OSAL_RETURN_OK == osal_ret))
    {
        count = 0;
        while (OSAL_AXI_BUS_NAME_LAST != pst_axi_bus_name_info[count].utlb)
        {
            /* Compare the domain name of input parameter and the domain name got from the UIO */
            linux_ret = strncmp(pst_axi_bus_name_info[count].domain_name, p_domain->domain_name, strlen(p_domain->domain_name));
            if (0 == linux_ret)
            {
                /* Compare the uTLB of input parameter and the uTLB got from the UIO */
                if (pst_axi_bus_name_info[count].utlb == p_domain->utlb_buffer[index])
                {
                    /* If needed, Copy the AXI ID list to the output parameter */
                    osal_ret = r_osal_dcfg_io_add_axiid_to_list(pp_list_buffer, &dst_index, count);

                    /* Count the number of matching */
                    match_count++;
                    break;
                }
            }
            count++;
        }

        if (OSAL_AXI_BUS_NAME_LAST == pst_axi_bus_name_info[count].utlb)
        {
            osal_ret = OSAL_RETURN_UNKNOWN_NAME; /* domain name not found or utlb number is not assigned to the domain */
        }
        index++;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        *p_num_of_axi_ids = match_count;

        if (0U == match_count)
        {
            osal_ret = OSAL_RETURN_UNKNOWN_NAME;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_io_check_axiidlist_for_domainname()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_022]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_023
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_io_check_axiidlist_for_devicename()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_check_axiidlist_for_devicename(const char* p_device_name, const st_osal_axi_bus_name_info_t* pst_axi_bus_name_info, osal_axi_bus_id_t** pp_list_buffer, size_t* const p_num_of_axi_ids)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    char const*     p_linux_ret;
    size_t          count = 0;
    char*           p_compare_str;
    size_t          match_count = 0;
    size_t          invalid_utlb_count = 0;
    size_t          dst_index = 0;

    /* Memory size is length of ('\n' + p_device_name + '\n' + '\0') */
    /* PRQA S 0316 1 # Required cast on system, not modifiable. */
    p_compare_str = (char *)R_OSAL_OS_Malloc(strlen(p_device_name) + 3U);
    if (NULL == p_compare_str)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    /* '\n' is separator of device name.                        */
    /* The following example, search the only "acf_00".         */
    /* Input parameter: acf_00                                  */
    /* Device name array: acf_00, irq_acf_00_01, fcprr_acf_00   */
    if (OSAL_RETURN_OK == osal_ret)
    {
        p_compare_str[0] = '\n';
        (void)strcpy(&p_compare_str[1], p_device_name);
        (void)strcat(p_compare_str, "\n");
    }

    while ((OSAL_RETURN_OK == osal_ret) && (OSAL_AXI_BUS_NAME_LAST != pst_axi_bus_name_info[count].utlb))
    {
        /* Search the device name of input parameter from the device name list from the UIO */
        p_linux_ret = strstr(pst_axi_bus_name_info[count].dev_names, p_compare_str);
        if (NULL != p_linux_ret)
        {
            if (OSAL_INVALID_UTLB != pst_axi_bus_name_info[count].utlb)
            {
                /* If needed, Copy the AXI ID list to the output parameter */
                osal_ret = r_osal_dcfg_io_add_axiid_to_list(pp_list_buffer, &dst_index, count);

                /* Count the number of matching */
                match_count++;
            }
            else
            {
                invalid_utlb_count++;
            }
        }

        count++;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        *p_num_of_axi_ids = match_count;

        if ((0U == match_count) && (0U == invalid_utlb_count))
        {
            osal_ret = OSAL_RETURN_UNKNOWN_NAME;
        }
    }

    /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
    R_OSAL_OS_Free((void *)p_compare_str);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_io_check_axiidlist_for_devicename()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_023]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_024
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_io_add_axiid_to_list()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_add_axiid_to_list(osal_axi_bus_id_t** pp_list_buffer, size_t* p_dst_index, size_t count)
{
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    osal_axi_bus_id_t*  p_tmp_buffer;

    if (NULL != pp_list_buffer)
    {
        /* PRQA S 0314 2 # The 1st arguments must be (void *) but no side effects in this case. */
        /* PRQA S 5118,2707 1 # Freed in caller function. */
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

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_io_add_axiid_to_list()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_024]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_025
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_io_open_error()
***********************************************************************************************************************/
OSAL_STATIC void r_osal_dcfg_io_open_error(osal_device_handle_t handle)
{
    if (NULL != handle)
    {
        (void)r_osal_dcfg_io_mutex_destroy(&handle->pm_mtx);
        (void)r_osal_dcfg_io_mutex_destroy(&handle->int_mtx);

        /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
        R_OSAL_OS_Free((void *)handle->dev_id);

        /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
        R_OSAL_OS_Free((void *)handle);
    }

    return;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_io_open_error()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_008]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_026
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_io_close()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_close(osal_device_handle_t handle)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t          cnt;

    /* R_OSAL_DCFG_PmWaitForState() is running */
    if (true == handle->pm_handle->pm_wait_state)
    {
        osal_ret = OSAL_RETURN_BUSY;
    }

    /* Check the handle */
    if (OSAL_RETURN_OK == osal_ret)
    {
        if ((OSAL_IO_HANDLE_ID != handle->handle_id) ||
            ((-1) == handle->dev_fd) ||
            ((NULL == handle->base_addr) && (0U != handle->len)) ||
            ((NULL != handle->base_addr) && (0U == handle->len)))
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
    }

    /* The interrupt thread does not unregister */
    for (cnt = 0; (OSAL_RETURN_OK == osal_ret) && (cnt < handle->irq_num); cnt++)
    {
        if (true == handle->int_handle[cnt].irq_use)
        {
            osal_ret = OSAL_RETURN_BUSY;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_IoDeviceClose(handle);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_io_close()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_009]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_027
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_io_mutex_destroy()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_mutex_destroy(osal_os_mutex_t* p_mutex)
{
    e_osal_return_t osal_ret;

    osal_ret = R_OSAL_OS_MutexDestroy(*p_mutex);
    if (OSAL_RETURN_OK == osal_ret)
    {
        *p_mutex = NULL;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_io_mutex_destroy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_086]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_028
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_io_mutex_lock_for_time_period()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_mutex_lock_for_time_period(osal_os_mutex_t mutex)
{
    e_osal_return_t osal_ret;

    osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(mutex, OSAL_INNER_TIMEOUT);
    if (OSAL_RETURN_OK != osal_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_io_mutex_lock_for_time_period()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_087]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_029
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_io_mutex_lock_until_time_stamp()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_mutex_lock_until_time_stamp(osal_os_mutex_t mutex, const st_osal_time_t* p_abs_timeout)
{
    e_osal_return_t osal_ret;

    osal_ret = R_OSAL_HIF_MutexLockUntilTimeStamp(mutex, p_abs_timeout);
    if (OSAL_RETURN_TIME == osal_ret)
    {
        osal_ret = OSAL_RETURN_BUSY;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_io_mutex_lock_until_time_stamp()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_088]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */
