/***********************************************************************************************************************
* Copyright [2022-2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    r_osal_dev_cfg_if.h
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Device Configurator Public Header
***********************************************************************************************************************/

#ifndef OSAL_DEV_CFG_IF_H
#define OSAL_DEV_CFG_IF_H

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/*======================================================================================================================
 Public global functions
======================================================================================================================*/
/*******************************************************************************************************************//**
* @brief            Returns the initialization status of the IO manager.
* @return           #e_osal_init_status_t
* @retval           OSAL_MANAGER_STATE_UNINITIALIZED
* @retval           OSAL_MANAGER_STATE_INITIALIZED
* @note             None.
* @see              None.
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_init_status_t R_OSAL_DCFG_IoGetInitStatus(void);

/*******************************************************************************************************************//**
* @brief            Sets the initialization status of the IO manager.
* @param[in]        status        Initialization status of the io manager.
* @return           void
* @note             None.
* @see              None.
***********************************************************************************************************************/
void R_OSAL_DCFG_IoSetInitStatus(e_osal_init_status_t status);

/*******************************************************************************************************************//**
* @brief            Initializes IO related controls in Device Configurator.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_Initialize
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoControlInit(void);

/*******************************************************************************************************************//**
* @brief            Deinitializes IO related controls in Device Configurator.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_Deinitialize
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoControlDeinit(void);

/*******************************************************************************************************************//**
* @brief            Returns the check result of whether there is active IO resource.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
* @note             This function is used to judge if the IO manager can be deinitialized.
* @see              Refer to following the function in this specification
*                   R_OSAL_Deinitialize
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoCheckBusy(void);

/*******************************************************************************************************************//**
* @brief            Get the number of devices identifier for a specific device type.
* @param[in]        p_deviceType    To set the string of device type.
* @param[in,out]    p_numOfDevices  To set the address of size_t.
*                                   The OSAL set the number of devices corresponding to deviceType.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_IoGetNumOfDevices
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoGetNumOfDevices (const char* p_deviceType,
                                               size_t* const p_numOfDevices);

/*******************************************************************************************************************//**
* @brief            Get the list of device identifier names for a specific device type.
* @param[in]        p_device_type_name  To set the string of device type.
* @param[in,out]    p_buffer            To set the buffer address.
*                                       The OSAL sets the device id corresponding to the device TYPE 
*                                       as a separated string to p_buffer.
*                                       Needs to acquire the memory area according to list_size.
*                                       The buffer shall use the '\\n' character as a delimiter for the string 
*                                       instances. The buffer shall be null-terminated, meaning it shall use '\0' 
*                                       at the end of the last instance.
* @param[in]        buffer_size         To set the value of p_buffer size by byte.
* @param[in,out]    p_num_of_byte       To set the address of size_t.
*                                       The OSAL sets the number of characters in p_buffer.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK
* @retval  OSAL_RETURN_FAIL
* @retval  OSAL_RETURN_STATE
* @retval  OSAL_RETURN_UNKNOWN_NAME    No device was found related to p_device_type_name.
* @retval  OSAL_RETURN_OK_RQST         on success. Only the number of characters in p_buffer could be obtained.
*                                      - p_buffer is NULL and buffer_size is 0.
* @note             1. This function shall return an error if the given buffer is too small to hold
*                      all device ID strings which must be returned.
*                   2. This function shall return the OSAL_RETURN_OK_RQST in
*                      case p_buffer==NULL && buffer_size==0 and update written_bytes to the required buffer size.
*                   3. This function shall modify the written_bytes output both on success and
*                      in the error case (if the provided buffer is too small).
*                   4. In case the provided buffer is too small this output variable shall be
*                      set to the required buffer size for the operation.
*                   Example: Requested device type is "abc" and it has 3 separate instances. 
*                   Output of the function:
*                   Buffer with content: "abc_00\nabc_01\nabc_02\0".
*                   Value of written bytes is 21.
* @see              Refer to following the function in this specification
*                   R_OSAL_IoGetDeviceList
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoGetDeviceList(const char* p_device_type_name,
                                            char* p_buffer,
                                            size_t buffer_size,
                                            size_t* const p_num_of_byte);

/*******************************************************************************************************************//**
* @brief           Open an IO device handle corresponding to p_id and set the handle to p_handle.
* @param[in]       p_id        To set the value of target device name (a.k.a device id) acquired by
*                              R_OSAL_IoGetDeviceList().
* @param[in,out]   p_handle    To set the address of osal_device_handle_t.
*                              The OSAL sets the handle.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and the IO handle was opened successfully. 
* @retval  OSAL_RETURN_PAR     Invalid argument.
*                              - p_id is NULL.
*                              - p_handle is NULL.
* @retval  OSAL_RETURN_MEM     Resource allocation failed.
* @retval  OSAL_RETURN_DEV     HW fault detected.
* @retval  OSAL_RETURN_CONF    Wrong configuration value.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_BUSY    Specified id is already handled.
*                              If this error occurs, p_handle will not be overwritten.
* @retval  OSAL_RETURN_ID      Specified id is unknown.
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    The setting items related to the Device ID are described the following.
*          <table>
*          <tr>
*          <th> elements </th> <th> description </th>
*          </tr>
*          <tr>
*          <td> Device Type </td> <td> One HW IP usually has multiple instances. 
*                                      Device ID is assigned to each instance, bundle with Device Type. 
*                                      For example, the device type is HW IP name.</td>
*          </tr>
*          <tr>
*          <td> Device ID </td> <td> device identifier </td>
*          </tr>
*          </table>
*          for example: IMR IP in the R-Car V3H, there are 6 different HW IP core. \n
*          Device Type: "imr" \n
*          Device ID : "imr_00", "imr_01", "imr_02", "imr_03", "imr_04", "imr_05" \n \n
*          After calling this function the OSAL Manager can access the target device via the device p_handle. \n
*          If this API returns an error code other than #OSAL_RETURN_BUSY, p_handle will be overwritten with 
*          #OSAL_DEVICE_HANDLE_INVALID.
* @see     Refer to following the function in this specification
*          R_OSAL_IoDeviceOpen
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoDeviceOpen(const char* p_id, osal_device_handle_t* const p_handle);

/*******************************************************************************************************************//**
* @brief           Close an IO device handle specified by handle.
* @param[in,out]   handle  To set the handle acquired at R_OSAL_IoDeviceOpen().
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and the IO handle was closed successfully.
* @retval  OSAL_RETURN_DEV     HW fault detected.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_BUSY    The specified handle is still in use by another manager.
* @retval  OSAL_RETURN_HANDLE  Invalid handle.
*                              - handle is not acquired by R_OSAL_IoDeviceOpen().
*                              - handle was already closed by R_OSAL_IoDeviceClose().
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    After calling this function, the handle is invalid and cannot be used.
*          This function works on the assumption that the interrupt is stopped before it is called.
*          Before calling this function, finish the interrupt processing and make sure that no interrupt occurs.
* @see     Refer to following the function in this specification
*          R_OSAL_IoDeviceClose
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoDeviceClose(osal_device_handle_t handle);

/*******************************************************************************************************************//**
* @brief    Read a register from the device.
* @param[in]       handle    To set the handle acquired at R_OSAL_IoDeviceOpen().
* @param[in]       offset    To set the value of offset from the base address of handle by byte.
* @param[in]       size      To set the value of register data size by byte.
* @param[in]       unit_size To set the access unit of the register.
* @param[in,out]   p_data    To set the address of data array to store the data.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and successfully read a value from register by 8bit alignment.
* @retval  OSAL_RETURN_PAR     Invalid argument.
*                              - p_data is NULL.
*                              - offset + sizeof(uint8_t) is more than register address range.
* @retval  OSAL_RETURN_DEV     HW fault detected.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_IO      Input / Output operation failed.
* @retval  OSAL_RETURN_HANDLE  Invalid handle.
*                              - handle is not acquired by R_OSAL_IoDeviceOpen().
*                              - handle was already closed by R_OSAL_IoDeviceClose().
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    None.
* @see     Refer to following the function in this specification
*          R_OSAL_IoRead8
*          R_OSAL_IoRead16
*          R_OSAL_IoRead32
*          R_OSAL_IoRead64
*          R_OSAL_IoBlockRead8
*          R_OSAL_IoBlockRead16
*          R_OSAL_IoBlockRead32
*          R_OSAL_IoBlockRead64
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoRead(osal_device_handle_t const handle,
                                   uintptr_t offset, 
                                   size_t size,
                                   size_t unit_size,
                                   void* p_data);

/*******************************************************************************************************************//**
* @brief   Write a register from the device.
* @param[in]       handle    To set the handle acquired at R_OSAL_IoDeviceOpen().
* @param[in]       offset    To set the value of offset from the base address of handle by byte.
* @param[in]       size      To set the value of register data size by byte.
* @param[in]       unit_size To set the access unit of the register.
* @param[in,out]   p_data    To set the address of data array to store the data.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and successfully write a value to register by 8bit alignment.
* @retval  OSAL_RETURN_PAR     Invalid argument.
*                              - offset + sizeof(uint8_t) is more than register address range.
* @retval  OSAL_RETURN_DEV     HW fault detected.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_IO      Input / Output operation failed.
* @retval  OSAL_RETURN_HANDLE  Invalid handle.
*                              - handle is not acquired by R_OSAL_IoDeviceOpen().
*                              - handle was already closed by R_OSAL_IoDeviceClose().
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    None.
* @see     Refer to following the function in this specification
*          R_OSAL_IoWrite8
*          R_OSAL_IoWrite16
*          R_OSAL_IoWrite32
*          R_OSAL_IoWrite64
*          R_OSAL_IoBlockWrite8
*          R_OSAL_IoBlockWrite16
*          R_OSAL_IoBlockWrite32
*          R_OSAL_IoBlockWrite64
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoWrite(osal_device_handle_t const handle,
                                    uintptr_t offset,
                                    size_t size,
                                    size_t unit_size,
                                    const void* p_data);

/*******************************************************************************************************************//**
* @brief   Returns the axi bus id for the given device. 
* @param[in]       device      Valid handle to a device.
* @param[in,out]   p_axi_id    This parameter is identifier of bus name about the device.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and the axi bus id for the given device was got successfully.
* @retval  OSAL_RETURN_FAIL
*                              -# An error occurred in OS or related API.
*                              -# if the axi id for the given device could not be acquired.
* @retval  OSAL_RETURN_HANDLE  Invalid handle.
*                              - device is not acquired by R_OSAL_IoDeviceOpen().
*                              - device was already closed by R_OSAL_IoDeviceClose().
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    None.
* @see     Refer to following the function in this specification
*          R_OSAL_IoGetDeviceAxiBusId
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoGetDeviceAxiBusId(osal_device_handle_t device,
                                                osal_axi_bus_id_t* const p_axi_id);

/*******************************************************************************************************************//**
* @brief           Returns the axi bus id for the given device.
* @param[in]       p_device_name   To set the value of target device name (a.k.a device id).
* @param[in,out]   p_axi_id        To set the address of osal_axi_bus_id_t.
*                                  The OSAL sets the identifier of bus name.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK              All arguments are valid, and the axi bus id for the given device name was 
*                                      got successfully.
* @retval  OSAL_RETURN_FAIL
*                                      -# An error occurred in OS or related API.
*                                      -# if the axi id for the given device name could not be acquired.
* @retval  OSAL_RETURN_STATE           Not initialized. (Before calling R_OSAL_Initialize().)
* @retval  OSAL_RETURN_UNKNOWN_NAME    No device was found related to p_device_name.
* @note    This function have limited usage in drivers or middleware to limit string compares and 
*          improved performance. It may be used for debugging/tracing purposes in drivers/middlewares. 
*          Drivers/Middlewares are typically owner of the device handles and 
*          shall use R_OSAL_IoGetDeviceAxiBusId() instead.
*          Also, "DeviceName" included in the API name is the device id.
* @see     Refer to following the function in this specification
*          R_OSAL_IoGetAxiBusIdFromDeviceName
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoGetAxiBusIdFromDeviceName(const char* p_device_name,
                                                        osal_axi_bus_id_t* const p_axi_id);

/*******************************************************************************************************************//**
* @brief            Get the string list of all axi bus names.
* @param[in,out]    p_buffer        Pointer to a buffer to write the string list to.
* @param[in]        buffer_size     Size of the provided buffer
* @param[in,out]    p_num_of_byte   Amount of bytes (to be) written to the buffer
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and the string list of all axi bus names was got successfully. 
* @retval  OSAL_RETURN_MEM     Resource allocation failed.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @retval  OSAL_RETURN_OK_RQST on success. Only the number of characters in p_buffer could be obtained.
*                              - p_buffer is NULL and buffer_size is 0.
* @note    None.
* @see     Refer to following the function in this specification
*          R_OSAL_IoGetAxiBusNameList
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoGetAxiBusNameList(char* p_buffer,
                                                size_t buffer_size,
                                                size_t* const p_num_of_byte);

/*******************************************************************************************************************//**
* @brief   Returns the axi bus name for the given axi id as a null terminated string.
* @param[in]       axi_id          This parameter is identifier of bus name.
* @param[in,out]   p_buffer        Pointer to a buffer to write the string list.
* @param[in]       buffer_size     Size of the provided buffer.
* @param[in,out]   p_num_of_byte   Amount of bytes (to be) written to the buffer.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and the string list of all axi bus names was got successfully.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @retval  OSAL_RETURN_OK_RQST on success. Only the number of characters in p_buffer could be obtained.
*                              - p_buffer is NULL and buffer_size is 0.
* @note    None.
* @see     Refer to following the function in this specification
*          R_OSAL_IoGetAxiBusName
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoGetAxiBusName(osal_axi_bus_id_t axi_id,
                                            char* p_buffer,
                                            size_t buffer_size,
                                            size_t* const p_num_of_byte);

/*******************************************************************************************************************//**
* @brief           Returns the number of all axi ids that correspond to the specified axi_bus_name.
* @param[in]       p_axi_bus_name      To set the pointer to character that identifies the AXI Bus Name.
* @param[in,out]   p_num_of_axi_ids    To set the pointer that contains the number of available AXI IDs 
*                                      for that AXI Bus Name
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK              All arguments are valid, and the number of all axi ids corresponding to 
*                                      the entered.
* @retval  OSAL_RETURN_STATE           Not initialized. (Before calling R_OSAL_Initialize().)
* @retval  OSAL_RETURN_UNKNOWN_NAME    If the entered axi_bus_name contains a combination of bus domain 
*                                      and uTLB to which axi_id is not assigned.
*                                      If the entered AXI Bus Name does not follow the format.
*                                      Please refer to this API "Note"
* @retval  OSAL_RETURN_FAIL            An error occurred in OS or related API.
* @note    axi_bus_name follows the format.
*          - bus domain and uTLB are connected by "_"
*          - If uTLBs are consecutive, connect the first and last uTLB values with "-"
*          - If uTLBs are discontinuous, connect with "_"
*          - uTLBs are described by 2 digit decimal number
*          - uTLB numbers are arranged in ascending order. \n
*          example) \n
*          When bus domain is "ir" and uTLBs are 0, 1, 2, 3, 4, 8, 9, 10,12 \n
*          "ir_00-04_08-10_12".
* @see     Refer to following the function in this specification
*          R_OSAL_IoGetNumOfAxiIdsForAxiBusName
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoGetNumOfAxiIdsForAxiBusName(const char* p_axi_bus_name,
                                                          size_t* p_num_of_axi_ids);

/*******************************************************************************************************************//**
* @brief           Returns the list of all axi ids that correspond to the specified axi_bus_name
* @param[in]       p_axi_bus_name              To set the pointer to character that identifies the AXI Bus Name.
* @param[in,out]   p_array                     To set the buffer address.
*                                              OSAL sets all axi ids that correspond to axi_bus_name
* @param[in]       array_size                  Maximum number of axi ids that can be written to p_array
* @param[in,out]   p_num_of_written_axi_ids    Number of axi ids stored in p_array
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK              All arguments are valid, and the list of all axi ids corresponding
*                                      to the entered.
* @retval  OSAL_RETURN_STATE           Not initialized. (Before calling R_OSAL_Initialize().)
* @retval  OSAL_RETURN_UNKNOWN_NAME    If the entered axi_bus_name contains a combination of bus domain
*                                      and uTLB to which axi_id is not assigned
*                                      If the entered AXI Bus Name does not follow the format.
*                                      Please refer to this API "Note"
* @retval  OSAL_RETURN_OK_RQST         on success. This API inputs the number of axi ids associated 
*                                      with p_axi_bus_name to p_num_of_written_axi_ids.
*                                      - only p_array is NULL and array_size is 0.
* @retval  OSAL_RETURN_FAIL            An error occurred in OS or related API.
* @note    axi_bus_name follows the format.
*          - bus domain and uTLB are connected by "_"
*          - If uTLBs are consecutive, connect the first and last uTLB values with "-"
*          - If uTLBs are discontinuous, connect with "_"
*          - uTLBs are described by 2 digit decimal number
*          - uTLB numbers are arranged in ascending order. \n
*          example) \n
*          When bus domain is "ir" and uTLBs are 0, 1, 2, 3, 4, 8, 9, 10,12 \n
*          "ir_00-04_08-10_12".
* @see     Refer to following the function in this specification
*          R_OSAL_IoGetAxiIdListForAxiBusName
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoGetAxiIdListForAxiBusName(const char * p_axi_bus_name,
                                                        osal_axi_bus_id_t * p_array,
                                                        size_t array_size,
                                                        size_t * const p_num_of_written_axi_ids);

/*******************************************************************************************************************//**
* @brief           Returns the number of all axi ids that correspond to the specified device handle. 
* @param[in]       device_hdl          To set the handle acquired at R_OSAL_IoDeviceOpen().
* @param[in,out]   p_num_of_axi_ids    To set the pointer that store the number of available AXI IDs
*                                      for that AXI Bus Name.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and the number of all axi ids corresponding to the entered.
* @retval  OSAL_RETURN_HANDLE  Invalid handle.
*                              - device is not acquired by R_OSAL_IoDeviceOpen().
*                              - device was already closed by R_OSAL_IoDeviceClose().
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @note    Even if use the same device, the value that can be obtained varies depending on the SoC.
* @see     Refer to following the function in this specification
*          R_OSAL_IoGetNumOfAxiIdsForDeviceHdl
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoGetNumOfAxiIdsForDeviceHdl(osal_device_handle_t device_hdl,
                                                         size_t * const p_num_of_axi_ids);

/*******************************************************************************************************************//**
* @brief           Returns the list of all axi ids that correspond to the specified device handle.
* @param[in]       device_hdl                  To set the handle acquired at R_OSAL_IoDeviceOpen().
* @param[in,out]   p_array                     To set the buffer address. OSAL sets all axi ids that correspond
*                                              to axi_bus_name. 
* @param[in]       array_size                  Maximum number of axi ids that can be written to p_array.
* @param[in,out]   p_num_of_written_axi_ids    To set the pointer that store number of axi ids stored in p_array.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and the number of all axi ids corresponding to the entered.
* @retval  OSAL_RETURN_HANDLE  Invalid handle.
*                              - device is not acquired by R_OSAL_IoDeviceOpen().
*                              - device was already closed by R_OSAL_IoDeviceClose().
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @retval  OSAL_RETURN_OK_RQST on success. This API inputs the number of axi ids associated 
*                              with device_hdl to p_num_of_written_axi_ids.
*                              - only p_array is NULL and array_size is 0.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @note    Even if use the same device, the value that can be obtained varies depending on the SoC.
* @see     Refer to following the function in this specification
*          R_OSAL_IoGetAxiIdListForDeviceHdl
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoGetAxiIdListForDeviceHdl(osal_device_handle_t device_hdl,
                                                       osal_axi_bus_id_t * p_array,
                                                       size_t array_size,
                                                       size_t * const p_num_of_written_axi_ids);

/*******************************************************************************************************************//**
* @brief   Returns the number of all axi ids that correspond to the specified device name.
* @param[in]       p_device_name       To set the value of target device name (a.k.a device id) acquired
*                                      by R_OSAL_IoGetDeviceList().
* @param[in,out]   p_num_of_axi_ids    To set the pointer that store the number of available AXI IDs 
*                                      for the device handle.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK              All arguments are valid, and the number of all axi ids corresponding
*                                      to the entered.
* @retval  OSAL_RETURN_STATE           Not initialized. (Before calling R_OSAL_Initialize().)
* @retval  OSAL_RETURN_UNKNOWN_NAME    If device name that is not supported by OSAL is entered.
* @retval  OSAL_RETURN_FAIL            An error occurred in OS or related API.
* @note    Even if use the same device, the value that can be obtained varies depending on the SoC.
           Also, "DeviceName" included in the API name is the device name.
           If device name supported by OSAL don't support axi id, OSAL_RETRUN_OK and p_num_of_axi_ids=0 are returned.
* @see     Refer to following the function in this specification
*          R_OSAL_IoGetNumOfAxiIdsForDeviceName
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoGetNumOfAxiIdsForDeviceName(const char* p_device_name,
                                                          size_t* const p_num_of_axi_ids);

/*******************************************************************************************************************//**
* @brief   Returns the list of all axi ids that correspond to the specified device name
* @param[in]       p_device_name               To set the value of target device name (a.k.a device id) acquired
*                                              by R_OSAL_IoGetDeviceList().
* @param[in,out]   p_array                     To set the buffer address. OSAL sets all axi ids
*                                              that correspond to axi_bus_name. 
* @param[in]       array_size                  Maximum number of axi ids that can be written to p_array.
* @param[in,out]   p_num_of_written_axi_ids    To set the pointer that store number of axi ids stored in p_array.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK              All arguments are valid, and the number of all axi ids corresponding to the entered.
* @retval  OSAL_RETURN_STATE           Not initialized. (Before calling R_OSAL_Initialize().)
* @retval  OSAL_RETURN_UNKNOWN_NAME    If device name that is not supported by OSAL is entered.
* @retval  OSAL_RETURN_OK_RQST         on success. This API inputs the number of axi ids associated 
*                                      with p_device_name to p_num_of_written_axi_ids.
*                                      - only p_array is NULL and array_size is 0.
* @retval  OSAL_RETURN_FAIL            An error occurred in OS or related API.
* @note    Even if use the same device, the value that can be obtained varies depending on the SoC.
           Also, "DeviceName" included in the API name is the device name.
           If device name supported by OSAL don't support axi id, OSAL_RETRUN_OK and p_num_of_axi_ids=0 are returned.
* @see     Refer to following the function in this specification
*          R_OSAL_IoGetAxiIdListForDeviceName
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoGetAxiIdListForDeviceName(const char * p_device_name,
                                                        osal_axi_bus_id_t * p_array,
                                                        size_t array_size,
                                                        size_t * const p_num_of_written_axi_ids );

/*******************************************************************************************************************//**
* @brief            Returns the initialization status of the Power manager.
* @return           #e_osal_init_status_t
* @retval           OSAL_MANAGER_STATE_UNINITIALIZED
* @retval           OSAL_MANAGER_STATE_INITIALIZED
* @note             None.
* @see              None.
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_init_status_t R_OSAL_DCFG_PmGetInitStatus(void);

/*******************************************************************************************************************//**
* @brief            Sets the initialization status of the Power manager.
* @param[in]        status        Initialization status of the power manager.
* @return           void
* @note             None.
* @see              None.
***********************************************************************************************************************/
void R_OSAL_DCFG_PmSetInitStatus(e_osal_init_status_t status);

/*******************************************************************************************************************//**
* @brief            Initializes Power related controls in Device Configurator.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_Initialize
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_PmControlInit(void);

/*******************************************************************************************************************//**
* @brief            Deinitializes Power related controls in Device Configurator.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_Deinitialize
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_PmControlDeinit(void);

/*******************************************************************************************************************//**
* @brief            Returns the check result of whether there is active Power resource.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_Deinitialize
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_PmCheckBusy(void);

/*******************************************************************************************************************//**
* @brief   Get the value of required-state of a device.
* @param[in]       handle  To set the handle acquired at R_OSAL_IoDeviceOpen().
* @param[in,out]   p_state To set the address of #e_osal_pm_required_state_t.
*                          The OSAL sets the device's required status.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK                      All arguments are valid, and required status was got from device 
*                                              successfully.
* @retval  OSAL_RETURN_DEV                     HW fault detected.
* @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE                  Invalid handle.
*                                              - handle is not acquired by R_OSAL_IoDeviceOpen().
*                                              - handle was already closed by R_OSAL_IoDeviceClose().
* @retval  OSAL_RETURN_STATE                   Not initialized. (Before calling R_OSAL_Initialize().)
* @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The device assigned to handle is not supported by Power Manager.
* @note    None.
* @see     Refer to following the function in this specification
*          R_OSAL_PmGetRequiredState
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_PmGetRequiredState(osal_device_handle_t handle,
                                               e_osal_pm_required_state_t* const p_state);

/*******************************************************************************************************************//**
* @brief   Set the value of required-state to a device.
* @param[in,out]   handle          To set the handle acquired at R_OSAL_IoDeviceOpen().
* @param[in]       state           To set the value of required status. \n
*                                  - OSAL_PM_REQUIRED: Switching power state to required. \n
*                                  - OSAL_PM_RELEASED: Switching power state to released.
* @param[in]       applyImmediate  To set the value of the function's role. \n
*                                  - true: Sync function, the state is applied immediately on the HW. \n
*                                  - false: Async function, the state is applied late to the HW.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK                      All arguments are valid, and required status was set to a device 
*                                              successfully.
* @retval  OSAL_RETURN_DEV                     HW fault detected.
* @retval  OSAL_RETURN_TIME                    applyImmediate is true and power state did not change during 
*                                              the specified time.
*                                              Please refer to each Wrapper User Manual for how to specify time.
* @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE                  Invalid handle.
*                                              - handle is not acquired by R_OSAL_IoDeviceOpen().
*                                              - handle was already closed by R_OSAL_IoDeviceClose().
* @retval  OSAL_RETURN_STATE                   Not initialized. (Before calling R_OSAL_Initialize().)
* @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The device assigned to handle is not supported by Power Manager.
* @note    When control the same handle from different threads and interrupts,
           ensure that the power state do not conflict on the user side.
* @see     Refer to following the function in this specification
*          R_OSAL_PmSetRequiredState
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_PmSetRequiredState(osal_device_handle_t handle,
                                               e_osal_pm_required_state_t state,
                                               bool applyImmediate);

/*******************************************************************************************************************//**
* @brief   Wait until to apply the specified state or the specified time has elapsed.
* @param[in]   handle      To set the handle acquired at R_OSAL_IoDeviceOpen().
* @param[in]   state       To set the value of power status.
* @param[in]   time_period Sets the value of time to wait for the power state is changed, in msec.
*                          (valid values are >= 0)
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK                      All arguments are valid, and the PmState was changed to the specified 
*                                              value successfully.
* @retval  OSAL_RETURN_PAR                     Invalid argument.
*                                              - time_period is less than 0.
*                                              - state is out of range. please see also #e_osal_pm_state_t.
* @retval  OSAL_RETURN_DEV                     HW fault detected.
* @retval  OSAL_RETURN_TIME                    Not changed state within the specified time.
* @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE                  Invalid handle.
*                                              - handle is not acquired by R_OSAL_IoDeviceOpen().
*                                              - handle was already closed by R_OSAL_IoDeviceClose().
* @retval  OSAL_RETURN_STATE                   Not initialized. (Before calling R_OSAL_Initialize().)
* @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The device assigned to handle is not supported by Power Manager.
* @note    The waiting time may be longer than specified due to the influence of other threads.
* @see     Refer to following the function in this specification
*          R_OSAL_PmWaitForState
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_PmWaitForState(osal_device_handle_t handle,
                                           e_osal_pm_state_t state,
                                           osal_milli_sec_t time_period);

/*******************************************************************************************************************//**
* @brief   Wait until to apply the specified state or the specified time has elapsed.
* @param[in]   handle      To set the handle acquired at R_OSAL_IoDeviceOpen().
* @param[in]   state       To set the value of power status.
* @param[in]   time_period Sets the value of time to wait for the power state is changed, in msec.
*                          (valid values are >= 0)
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK                      All arguments are valid, and the PmState was changed to the specified 
*                                              value successfully.
* @retval  OSAL_RETURN_PAR                     Invalid argument.
*                                              - time_period is less than 0.
*                                              - state is out of range. please see also #e_osal_pm_state_t.
* @retval  OSAL_RETURN_DEV                     HW fault detected.
* @retval  OSAL_RETURN_TIME                    Not changed state within the specified time.
* @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE                  Invalid handle.
*                                              - handle is not acquired by R_OSAL_IoDeviceOpen().
*                                              - handle was already closed by R_OSAL_IoDeviceClose().
* @retval  OSAL_RETURN_STATE                   Not initialized. (Before calling R_OSAL_Initialize().)
* @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The device assigned to handle is not supported by Power Manager.
* @note    None.
* @see     Refer to following the function in this specification
*          R_OSAL_PmGetState
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_PmGetState(osal_device_handle_t handle,
                                       e_osal_pm_state_t* const p_state);

/*******************************************************************************************************************//**
* @brief   Get the lowest intermediate all possible states of the Power Management State Machine (power/clock) 
*          that was reached since the last change to non-required status and set to p_state.
* @param[in]       handle  To set the handle acquired at R_OSAL_IoDeviceOpen().
* @param[in,out]   p_state To set the address #e_osal_pm_state_t.
*                          The OSAL sets the device's power status.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK                      All arguments are valid, and the power status was got from device 
*                                              successfully.
* @retval  OSAL_RETURN_DEV                     HW fault detected.
* @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE                  Invalid handle.
*                                              - handle is not acquired by R_OSAL_IoDeviceOpen().
*                                              - handle was already closed by R_OSAL_IoDeviceClose().
* @retval  OSAL_RETURN_STATE                   Not initialized. (Before calling R_OSAL_Initialize().)
* @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The device assigned to handle is not supported by Power Manager.
* @note    The state of the Power Management State Machine is defined in e_osal_pm_state_t. 
           Higher values mean higher states.
           Example: The OSAL_PM_STATE_PG is lower than the OSAL_PM_STATE_CG state.
* @see     Refer to following the function in this specification
*          R_OSAL_PmGetLowestIntermediatePowerState
**********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_PmGetLowestIntermediatePowerState(osal_device_handle_t handle,
                                                              e_osal_pm_state_t* const p_state);

/*******************************************************************************************************************//**
* @brief   Get the power policy of a device and set the value to p_policy.
* @param[in]       handle      To set the handle acquired at R_OSAL_IoDeviceOpen().
* @param[in,out]   p_policy    To set the address #e_osal_pm_policy_t.
*                              The OSAL sets the device's power policy.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK                      All arguments are valid, and the power policy was set to device 
*                                              successfully.
* @retval  OSAL_RETURN_DEV                     HW fault detected.
* @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE                  Invalid handle.
*                                              - handle is not acquired by R_OSAL_IoDeviceOpen().
*                                              - handle was already closed by R_OSAL_IoDeviceClose().
* @retval  OSAL_RETURN_STATE                   Not initialized. (Before calling R_OSAL_Initialize().)
* @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The device assigned to handle is not supported by Power Manager.
* @note    None.
* @see     Refer to following the function in this specification
*          R_OSAL_PmGetPolicy
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_PmGetPolicy(osal_device_handle_t handle,
                                        e_osal_pm_policy_t* const p_policy);

/*******************************************************************************************************************//**
* @brief   Set the requested power policy for a device.
* @param[in]   handle          To set the handle acquired at R_OSAL_IoDeviceOpen().
* @param[in]   policy          To set the value of device power policy.
*                              - #OSAL_PM_POLICY_HP: Clock domain ON / Power domain ON
*                              - #OSAL_PM_POLICY_CG: Clock domain OFF / Power domain ON
*                              - #OSAL_PM_POLICY_PG: Clock domain OFF / Power domain OFF
* @param[in]   applyImmediate  To set the value of the function's role.
*                              - true: Sync function, the policy is applied immediately on the HW.
*                              - false: Async function, the policy is applied late to the HW.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK                      All arguments are valid, and the power policy was set to device 
*                                              successfully.
* @retval  OSAL_RETURN_DEV                     HW fault detected.
* @retval  OSAL_RETURN_TIME                    When applyImmediate is true, power state did not change during 
*                                              the specified time.
*                                              Please refer to each Wrapper User Manual for how to specify time.
* @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE                  Invalid handle.
*                                              - handle is not acquired by R_OSAL_IoDeviceOpen().
*                                              - handle was already closed by R_OSAL_IoDeviceClose().
* @retval  OSAL_RETURN_STATE                   Not initialized. (Before calling R_OSAL_Initialize().)
* @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The device assigned to handle is not supported by Power Manager.
* @note    None.
* @see     Refer to following the function in this specification
*          R_OSAL_PmSetPolicy
**********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_PmSetPolicy(osal_device_handle_t handle,
                                        e_osal_pm_policy_t policy,
                                        bool applyImmediate);

/*******************************************************************************************************************//**
* @brief   Get the reset state value of a device and set the value to p_state.
* @param[in]       handle  To set the handle acquired at R_OSAL_IoDeviceOpen().
* @param[in,out]   p_state To set the address of #e_osal_pm_reset_state_t.
*                          The OSAL sets the reset status.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK                      All arguments are valid, and the reset status was got from device 
*                                              successfully.
* @retval  OSAL_RETURN_DEV                     HW fault detected.
* @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE                  Invalid handle.
*                                              - handle is not acquired by R_OSAL_IoDeviceOpen().
*                                              - handle was already closed by R_OSAL_IoDeviceClose().
* @retval  OSAL_RETURN_STATE                   Not initialized. (Before calling R_OSAL_Initialize().)
* @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The device assigned to handle is not supported by Power Manager.
* @note    None.
* @see     Refer to following the function in this specification
*          R_OSAL_PmGetResetState
**********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_PmGetResetState(osal_device_handle_t handle,
                                            e_osal_pm_reset_state_t* const p_state);

/*******************************************************************************************************************//**
* @brief   Set the reset state value for a device.
* @param[in]   handle  To set the handle acquired at R_OSAL_IoDeviceOpen().
* @param[in]   state   To set value of type #e_osal_pm_reset_state_t that identifies.
*                      the requested reset status for the device.
*                      - #OSAL_PM_RESET_STATE_APPLIED: Reset.
*                      - #OSAL_PM_RESET_STATE_RELEASED: Release reset.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK                      All arguments are valid, and reset status was set to a device 
*                                              successfully.
* @retval  OSAL_RETURN_DEV                     HW fault detected.
* @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE                  Invalid handle.
*                                              - handle is not acquired by R_OSAL_IoDeviceOpen().
*                                              - handle was already closed by R_OSAL_IoDeviceClose().
* @retval  OSAL_RETURN_STATE                   Not initialized. (Before calling R_OSAL_Initialize().)
* @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The device assigned to handle is not supported by Power Manager.
* @note    When control the same handle from different threads and interrupts,
           ensure that the power state do not conflict on the user side.
* @see     Refer to following the function in this specification
*          R_OSAL_PmSetResetState
**********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_PmSetResetState(osal_device_handle_t handle,
                                            e_osal_pm_reset_state_t state);

/*******************************************************************************************************************//**
* @brief   Set the POST clock and modify its clock divider for a given hierarchy the device is in.
* @param[in]   device          Device to configure the POST Clock for
* @param[in]   clock_enable    Enable POST Clock for the hierarchy the device is in.
* @param[in]   clock_divider   Clock divider setting. [clock_divider < 64]
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK                      All arguments are valid, and the POST clock was set and modify 
*                                              its clock divider successfully.
* @retval  OSAL_RETURN_DEV                     in case the underlying device cannot be accessed.
* @retval  OSAL_RETURN_IO                      in case IO operation failed
* @retval  OSAL_RETURN_FAIL                    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE                  Invalid handle.
*                                              - device is not acquired by R_OSAL_IoDeviceOpen().
*                                              - device was already closed by R_OSAL_IoDeviceClose().
* @retval  OSAL_RETURN_STATE                   Not initialized. (Before calling R_OSAL_Initialize().)
* @retval  OSAL_RETURN_UNSUPPORTED_OPERATION   The device does not support POST Clock settings.
* @note    Only FBC/FBA devices do support this operation. Any other device will return OSAL_RETURN_UNSUPPORTED_OPERATION.
           Due to HW the POST Clock settings may be shared across multiple devices if they belong to the same Clock hierarchy.
           It is users Responsibility to ensure FFI.
* @see     Refer to following the function in this specification
*          R_OSAL_PmSetPostClock
 **********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_PmSetPostClock(osal_device_handle_t device,
                                           bool clock_enable,
                                           uint32_t clock_divider);

/*******************************************************************************************************************//**
* @brief            Returns the initialization status of the Interrupt manager.
* @return           #e_osal_init_status_t
* @retval           OSAL_MANAGER_STATE_UNINITIALIZED
* @retval           OSAL_MANAGER_STATE_INITIALIZED
* @note             None.
* @see              None.
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_init_status_t R_OSAL_DCFG_InterruptGetInitStatus(void);

/*******************************************************************************************************************//**
* @brief            Sets the initialization status of the Interrupt manager.
* @param[in]        status        Initialization status of the interrupt manager.
* @return           void
* @note             None.
* @see              None.
***********************************************************************************************************************/
void R_OSAL_DCFG_InterruptSetInitStatus(e_osal_init_status_t status);

/*******************************************************************************************************************//**
* @brief            Initializes Interrupt related controls in Device Configurator.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @note             For static resource environment, this function prepares all interrupt threads in
                    case that interrupt thread is used in the environment.
                    Both for static resource environment and for dynamic resource environment,
                    resources to control message queues are also prepared.
* @see              R_OSAL_Initialize
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_InterruptControlInit(void);

/*******************************************************************************************************************//**
* @brief            Deinitializes Interrupt related controls in Device Configurator.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @note             For static resource environment, this function finishes all interrupt threads in
                    case that interrupt thread is used in the environment.
                    Both for static resource environment and for dynamic resource environment,
                    resources to control message queues are destroyed in this function.
* @see              R_OSAL_Deinitialize
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_InterruptControlDeinit(void);

/*******************************************************************************************************************//**
* @brief            Returns the check result of whether there is active Interrupt resource.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
* @note             This function is used to judge if the interrupt manager can be deinitialized.
* @see              R_OSAL_Deinitialize
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_InterruptCheckBusy(void);

/*******************************************************************************************************************//**
* @brief   Registers the interrupt handler service routine called when an interrupt occurs from device.
* @param[in,out]   handle              To set the handle unique value of device handle acquired at 
                                       R_OSAL_IoDeviceOpen().
* @param[in]       irq_channel         To set the value of Irq channel number.
                                       This value is from 0 to less than the value
                                       acquired with R_OSAL_InterruptGetNumOfIrqChannels().
* @param[in]       priority_value      the value of interrupt priority.
* @param[in]       irqHandlerFunction  pointer of CallBackFunction.
* @param[in]       p_irq_arg           interrupt handler function's argument.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and the function to be called when the interrupt occurs 
*                              has been successfully registered.
* @retval  OSAL_RETURN_DEV     HW fault detected.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_BUSY    Already registered or interrupt remains enabled.
* @retval  OSAL_RETURN_HANDLE  Invalid handle.
*                              - handle is not acquired by R_OSAL_IoDeviceOpen().
*                              - the number of interrupt channel is 0.
*                              - handle was already closed by R_OSAL_IoDeviceClose().
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    This function works on the assumption that the interrupt is stopped before it is called.
           Before calling this function, finish the interrupt processing and make sure that no interrupt occurs. 
           When setting the same interrupt resource from different threads and interrupts,
           ensure that the settings do not conflict on the user side.
* @see     R_OSAL_InterruptRegisterIsr
**********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_InterruptRegisterIsr(osal_device_handle_t handle,
                                                 uint64_t irq_channel,
                                                 e_osal_interrupt_priority_t priority_value,
                                                 p_osal_isr_func_t irqHandlerFunction,
                                                 void* p_irq_arg);

/*******************************************************************************************************************//**
* @brief   Unregisters the interrupt handler service routine called when an interrupt occurs from device.
* @param[in]   handle              To set the handle unique value of device handle acquired at R_OSAL_IoDeviceOpen().
* @param[in]   irq_channel         To set the value of Irq channel number.
*                                  This value is from 0 to less than the value
*                                  acquired with R_OSAL_InterruptGetNumOfIrqChannels().
* @param[in]   irqHandlerFunction  pointer of CallBackFunction.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and the function to be called when the interrupt occurs 
*                              has been successfully unregistered.
* @retval  OSAL_RETURN_DEV     HW fault detected.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_BUSY    Interrupt remains enabled.
* @retval  OSAL_RETURN_HANDLE  Invalid handle.
*                              - handle is not acquired by R_OSAL_IoDeviceOpen().
*                              - the number of interrupt channel is 0.
*                              - handle was already closed by R_OSAL_IoDeviceClose().
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    This function works on the assumption that the interrupt is stopped before it is called.
           Before calling this function, finish the interrupt processing and make sure that no interrupt occurs.
           When setting the same interrupt resource from different threads and interrupts,
           ensure that the settings do not conflict on the user side.
* @see     R_OSAL_InterruptUnregisterIsr
**********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_InterruptUnregisterIsr(osal_device_handle_t handle,
                                                   uint64_t irq_channel, 
                                                   p_osal_isr_func_t irqHandlerFunction);

/*******************************************************************************************************************//**
* @brief   Enables the interrupt handler already registered to the specified irq_channel.
* @param[in]   handle      To set the handle unique value of device handle acquired at R_OSAL_IoDeviceOpen().
* @param[in]   irq_channel To set the value of Irq channel number.
*                          This value is from 0 to less than the value
*                          acquired with R_OSAL_InterruptGetNumOfIrqChannels().
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and the handler already registered was successfully enabled.
* @retval  OSAL_RETURN_PAR     irq_channel is larger or equal to the number of irq channel of the target device.
* @retval  OSAL_RETURN_DEV     HW fault detected.
* @retval  OSAL_RETURN_FAIL
*                              -# An error occurred in OS or related API.
*                              -# If the function is not registered.
* @retval  OSAL_RETURN_HANDLE  Invalid handle.
*                              - handle is not acquired by R_OSAL_IoDeviceOpen().
*                              - the number of interrupt channel is 0.
*                              - handle was already closed by R_OSAL_IoDeviceClose().
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    When setting the same interrupt resource from different threads and interrupts,
           ensure that the settings do not conflict on the user side.
* @see     R_OSAL_InterruptEnableIsr
**********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_InterruptEnableIsr(osal_device_handle_t handle,
                                               uint64_t irq_channel);

/*******************************************************************************************************************//**
* @brief   Disables the interrupt handler already registered to the specified irq_channel.
* @param[in]   handle      To set the handle unique value of device handle acquired at R_OSAL_IoDeviceOpen().
* @param[in]   irq_channel To set the value of Irq channel number.
*                          This value is from 0 to less than the value
*                          acquired with R_OSAL_InterruptGetNumOfIrqChannels().
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and the handler already registered was successfully disabled.
* @retval  OSAL_RETURN_PAR     irq_channel is larger or equal to the number of irq channel of the target device.
* @retval  OSAL_RETURN_DEV     HW fault detected.
* @retval  OSAL_RETURN_FAIL
*                              -# An error occurred in OS or related API.
*                              -# If the function is not registered.
* @retval  OSAL_RETURN_HANDLE  Invalid handle.
*                              - handle is not acquired by R_OSAL_IoDeviceOpen().
*                              - the number of interrupt channel is 0.
*                              - handle was already closed by R_OSAL_IoDeviceClose().
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    When setting the same interrupt resource from different threads and interrupts,
           ensure that the settings do not conflict on the user side. 
           This function needs to use before register/unregister
           the function with R_OSAL_InterruptRegisterIsr() and R_OSAL_InterruptUnregisterIsr().
* @see     R_OSAL_InterruptDisableIsr
**********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_InterruptDisableIsr(osal_device_handle_t handle,
                                                uint64_t irq_channel);

/*******************************************************************************************************************//**
* @brief   Checks where the context was called and set the context to p_is_isr.
* @param[in,out]   p_is_isr    To set the address of bool. The OSAL sets the context.
*                              p_is_isr is set the result of judge.
*                              - true: interrupt context
*                              - false: not interrupt context
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and successfully confirm the context of original calling.
* @retval  OSAL_RETURN_PAR     Invalid argument.
*                              - p_is_isr is NULL.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    None.
* @see     R_OSAL_InterruptIsISRContext
**********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_InterruptIsISRContext(bool *const p_is_isr);

/*******************************************************************************************************************//**
* @brief   Changes priority already registered handler to priority_value.
* @param[in]   handle          To set the handle unique value of device handle acquired at R_OSAL_IoDeviceOpen().
* @param[in]   irq_channel     To set the value of Irq channel number.
*                              This value is from 0 to less than the value 
*                              acquired with R_OSAL_InterruptGetNumOfIrqChannels().
* @param[in]   priority_value  To set the value of priority.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and successfully changed priority already registered handler.
* @retval  OSAL_RETURN_PAR     Invalid argument.
*                               - irq_channel is the number of interrupt channel or more.
*                               - priority_value is out of range. please see also #e_osal_interrupt_priority_t.
* @retval  OSAL_RETURN_DEV     HW fault detected.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE  Invalid handle.
*                              - handle is not acquired by R_OSAL_IoDeviceOpen().
*                              - the number of interrupt channel is 0.
*                              - handle was already closed by R_OSAL_IoDeviceClose().
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    None.
* @see     R_OSAL_InterruptSetIsrPriority
**********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_InterruptSetIsrPriority(osal_device_handle_t handle,
                                                    uint64_t irq_channel,
                                                    e_osal_interrupt_priority_t priority_value);

/*******************************************************************************************************************//**
* @brief   Get the number of interrupt channels for a specific device and set the value to p_numOfChannels. \n
*          The interrupt channel is used to specify the interrupt information associated with handle in each 
*          OSAL interrupt API.
* @param[in]       handle          To set the handle unique value of device handle acquired at R_OSAL_IoDeviceOpen().
* @param[in,out]   p_numOfChannels To set the address of size_t. The OSAL set the number of irq channels.
* @return  #e_osal_return_t
* @retval  OSAL_RETURN_OK      All arguments are valid, and the number of interrupt channels for a specific device was 
*                              got successfully.
* @retval  OSAL_RETURN_FAIL    An error occurred in OS or related API.
* @retval  OSAL_RETURN_HANDLE  Invalid handle.
*                              - handle is not acquired by R_OSAL_IoDeviceOpen().
*                              - handle is corrupted.
*                              - handle was already closed by R_OSAL_IoDeviceClose().
* @retval  OSAL_RETURN_STATE   Not initialized. (Before calling R_OSAL_Initialize().)
* @note    None.
* @see     R_OSAL_InterruptGetNumOfIrqChannels
**********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_InterruptGetNumOfIrqChannels(osal_device_handle_t handle, size_t *const p_numOfChannels);

/** @} OSAL_WRAPPER */

#endif /* OSAL_DEV_CFG_IF_H */
/*======================================================================================================================
End of File
======================================================================================================================*/
