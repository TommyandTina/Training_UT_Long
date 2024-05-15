/***********************************************************************************************************************
* Copyright [2021-2022] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    r_osal_io.c
* Version :      0.5.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL wrapper IO Manager for eMCOS
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <mcos/pdev.h>

#include "target/emcos/r_osal_common.h"

st_pdev_entry_t g_pdev_entry; /* TODO: pdev information should be moved to dev_cfg */

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS
 * @defgroup OSAL_Private_Defines_IO IO Manager Private macro definitions
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @var gs_osal_io_is_initialized
 * Initialize flag
***********************************************************************************************************************/
OSAL_STATIC bool gs_osal_io_is_initialized = false;

/*******************************************************************************************************************//**
 * @var gsp_axi_bus_name_info
 * AXI Bus name information table
***********************************************************************************************************************/
OSAL_STATIC st_osal_axi_bus_name_info_t *gsp_axi_bus_name_info = NULL;

/*******************************************************************************************************************//**
 * @var gs_axi_bus_name_info_array_size
 * Size of the AXI Bus name information table
***********************************************************************************************************************/
OSAL_STATIC size_t gs_axi_bus_name_info_array_size = 0;

/** @} */

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS
 * @defgroup OSAL_Private_Defines_Static_Variables_IO IO Manager Private static variables
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
* @brief            Compare strings.
* @param[in]        p_str1  The first comparison string
* @param[in]        p_str2  The second comparison string
* @param[in]        len     Comparison string size
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_compare_string(const char *p_str1, const char *p_str2, size_t len);

/*******************************************************************************************************************//**
* @brief            Get domain name from axi_bus_name
* @param[in]        p_axi_bus_name      The string of axi_bus_name
* @param[out]       p_domain_name       Destination pointer for the domain name string
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_UNKNOWN_NAME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_get_domain_name(const char *p_axi_bus_name, char *p_domain_name);

/*******************************************************************************************************************//**
* @brief            Add uTLB number with the delimiter '_' to the list.
* @param[in,out]    parse_utlb      uTLB number
* @param[in]        p_domain_name   domain name string
* @param[in,out]    pp_list_buffer  The buffer address
* @param[in]        list_size       The value of pp_list_buffer size by byte
* @param[in,out]    match_count     uTLB match count
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_UNKNOWN_NAME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_add_utlb_list_single(int32_t utlb,
                                                               const char* p_domain_name,
                                                               osal_axi_bus_id_t* pp_list_buffer,
                                                               size_t list_size,
                                                               size_t *match_count);

/*******************************************************************************************************************//**
* @brief            Add uTLB number with the delimiter '-' to the list.
* @param[in,out]    parse_utlb      uTLB number
* @param[in]        p_domain_name   domain name string
* @param[in,out]    pp_list_buffer  The buffer address
* @param[in]        list_size       The value of pp_list_buffer size by byte
* @param[in,out]    match_count     uTLB match count
* @param[in]        current_utlb    current uTLB number
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_add_utlb_list_multi(int32_t utlb,
                                                              const char* p_domain_name,
                                                              osal_axi_bus_id_t* pp_list_buffer,
                                                              size_t list_size,
                                                              size_t *match_count,
                                                              const int32_t *current_utlb);

/*******************************************************************************************************************//**
* @brief            Add uTLB number to the list
* @param[in]        p_domain_name   domain name string
* @param[in,out]    pp_list_buffer  The buffer address
* @param[in]        list_size       The value of pp_list_buffer size by byte
* @param[in,out]    match_count     uTLB match count
* @param[in]        current_utlb    current uTLB number
* @param[in]        p_num_str       uTLB number with delimiter.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_UNKNOWN_NAME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_add_utlb_list(const char* p_domain_name,
                                                        osal_axi_bus_id_t* pp_list_buffer,
                                                        size_t list_size,
                                                        size_t *match_count,
                                                        int32_t *current_utlb,
                                                        const char *p_num_str);

/*******************************************************************************************************************//**
* @brief            Check axi list by domain name and utlb number
* @param[in]        p_axi_bus_name      The string of axi_bus_name
* @param[in]        p_domain_name       The string of domain name 
* @param[in,out]    pp_list_buffer      uTLB number list
* @param[in]        list_size           uTLB number list size
* @param[out]       p_num_of_axi_ids    Pointer to set the number of available AXI IDs.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_UNKNOWN_NAME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_check_axiidlist_for_domainname(const char* p_axi_bus_name,
                                                                         const char* p_domain_name,
                                                                         osal_axi_bus_id_t* pp_list_buffer,
                                                                         size_t list_size,
                                                                         size_t * const p_num_of_axi_ids);

/*******************************************************************************************************************//**
* @brief            Create the number of AXI IDs from the device name
* @param[in]        p_device_name           The string of device_name
* @param[in,out]    pp_list_buffer          uTLB number list
* @param[in]        list_size               uTLB number list size
* @param[out]       p_num_of_axi_ids        Pointer to set the number of available AXI IDs.
* @param[out]       p_num_of_invalid_ids    Pointer to set the number of invalid.
* @return           void
***********************************************************************************************************************/
OSAL_STATIC void osal_internal_create_axiidlist_for_devicename(const char* p_device_name,
                                                               osal_axi_bus_id_t *pp_list_buffer,
                                                               size_t list_size,
                                                               size_t * const p_num_of_axi_ids,
                                                               size_t * const p_num_of_invalid_ids);

/*******************************************************************************************************************//**
* @brief            Create AXI Bus name information
* @param[in]        p_device_control     The pointor of st_osal_device_control_t
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_create_axibus_info(const st_osal_device_control_t *p_device_control);

/*******************************************************************************************************************//**
* @brief            Register with AXI Bus name information
* @param[in]        p_bus_domain        The pointor of Bus domain
* @param[in]        p_device_name       The pointor of Device name
* @param[in]        utlb                The uTLB number
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_register_axi_bus_info(const char * const p_bus_domain,
                                                                const char* const p_device_name,
                                                                int32_t utlb);

/*******************************************************************************************************************//**
* @brief            Connecting device names
* @param[in]        p_device_name       The pointor of Device name
* @param[in]        axi_bus_info_num    Destination Index
* @param[in]        add_dev_name_len    The legth of Device name
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_device_name_connect(const char* const p_device_name,
                                                              size_t axi_bus_info_num,
                                                              size_t add_dev_name_len);

/*******************************************************************************************************************//**
* @brief            Register last data with AXI Bus name information
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_register_axi_bus_info_last_data(void);

/*******************************************************************************************************************//**
* @brief            Get the uTLB assignment number associated with device name.
* @param[in]        p_device_name       The pointor of Device name
* @param[in]        p_num_of_axi_ids    Pointer to set the number of available AXI IDs
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_UNKNOWN_NAME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_get_utlb_assignment_for_devicename(const char* p_device_name,
                                                                             size_t* const p_num_of_axi_ids);

/*******************************************************************************************************************//**
* @brief            Pre-check for Deinitialize IO Manager process.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_deinit_pre_proc(void);

/*******************************************************************************************************************//**
* @brief            Internal parameter check for get device list.
* @param[in]        p_device_type_name  The string of device type.
* @param[in]        p_buffer            The buffer address.
* @param[in]        buffer_size         The value of p_buffer size by byte.
* @param[in]        p_num_of_byte       The address of size_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_check_get_device_list(const char * p_device_type_name,
                                                                   const char * p_buffer,
                                                                   size_t buffer_size,
                                                                   const size_t *const p_num_of_byte);

/*******************************************************************************************************************//**
* @brief            Internal R_OSAL_IoGetDeviceList process.
* @param[in]        p_device_type_name  The string of device type.
* @param[in]        p_buffer            The buffer address.
* @param[in]        buffer_size         The value of p_buffer size by byte.
* @param[in]        p_num_of_byte       The address of size_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_OK_RQST
* @retval           OSAL_RETURN_UNKNOWN_NAME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_get_device_list_proc(const char * p_device_type_name,
                                                                  char * p_buffer,
                                                                  size_t buffer_size,
                                                                  size_t *const p_num_of_byte);

/*******************************************************************************************************************//**
* @brief            Internal create device list.
* @param[in]        device_control      The pointor of st_osal_device_control_t.
* @param[in]        p_device_type_name  The string of device type.
* @param[in]        buffer_size         The value of p_buffer size by byte.
* @param[in]        p_buffer            The buffer address.
* @param[in]        p_total_length      The value of device list total length.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_PAR
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_create_device_list(const st_osal_device_control_t *device_control,
                                                                const char *p_device_type_name,
                                                                size_t buffer_size,
                                                                char *p_buffer,
                                                                size_t *p_total_length);

/*******************************************************************************************************************//**
* @brief            Internal create device list for OKREQ.
* @param[in]        device_control      The pointor of st_osal_device_control_t.
* @param[in]        p_device_type_name  The string of device type.
* @param[in]        p_total_length      The value of device list total length.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK_RQST
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_create_device_list_okreq(const st_osal_device_control_t *device_control,
                                                                      const char *p_device_type_name,
                                                                      size_t *p_total_length);

/*******************************************************************************************************************//**
* @brief            Initialize mutex attributes..
* @param[in]        p_id               The value of target device name
* @param[in,out]    pp_device_control  The pointor of st_osal_device_control_t
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_ID
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_check_device_id(const char * p_id, st_osal_device_control_t **pp_device_control);

/*******************************************************************************************************************//**
* @brief            Set the initial information in the device handle.
* @param[in]        p_handle          The pointor of osal_device_handle_t.
* @param[in]        p_device_control  The pointor of st_osal_device_control_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_set_open_handle(osal_device_handle_t *const p_handle,
                                                             st_osal_device_control_t *p_device_control);

/*******************************************************************************************************************//**
* @brief            Internal R_OSAL_IoDeviceClose process.
* @param[in]        handle      The pointor of osal_device_handle_t
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_BUSY
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_device_close_proc(osal_device_handle_t handle);

/*******************************************************************************************************************//**
* @brief            Internal handle check for R_OSAL_IoRead / R_OSAL_IoWrite.
* @param[in]        handle         The pointor of osal_device_handle_t.
* @param[in]        unit_size      IO read / write unit size.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_HANDLE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_check_rw_handle(osal_device_handle_t const handle, size_t unit_size);

/*******************************************************************************************************************//**
* @brief            Internal parameter check for R_OSAL_IoRead.
* @param[in]        handle         The pointor of osal_device_handle_t.
* @param[in]        offset         The value of offset.
* @param[in]        unit_size      Read unit size.
* @param[in]        p_data         The register data or write data.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_check_read_param(osal_device_handle_t const handle,
                                                              uintptr_t offset, size_t unit_size, const void *const p_data);

/*******************************************************************************************************************//**
* @brief            Internal parameter check for R_OSAL_IoWrite.
* @param[in]        handle         The pointor of osal_device_handle_t.
* @param[in]        offset         The value of offset.
* @param[in]        unit_size      Write unit size.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_check_write_param(osal_device_handle_t const handle, uintptr_t offset, size_t unit_size);

/*******************************************************************************************************************//**
* @brief            Internal parameter check for R_OSAL_IoBlockRead / R_OSAL_IoBlockWrite.
* @param[in]        handle         The pointor of osal_device_handle_t.
* @param[in]        offset         The value of offset.
* @param[in]        size           The value of register data size.
* @param[in]        unit_size      Read/Write unit size.
* @param[in]        p_data         The register data or write data.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_check_block_rw_param(osal_device_handle_t const handle,
                                                                  uintptr_t offset,
                                                                  size_t size,
                                                                  size_t unit_size,
                                                                  const void *const p_data);

/*******************************************************************************************************************//**
* @brief            Internal parameter check for R_OSAL_IoGetDeviceAxiBusId.
* @param[in]        device         The pointor of osal_device_handle_t
* @param[in]        p_axi_id       Axi bus id
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_PAR
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_check_get_axi_bus_id(osal_device_handle_t const device, const osal_axi_bus_id_t *const p_axi_id);

/*******************************************************************************************************************//**
* @brief            Search for the specified device name.
* @param[in]        p_device_name      Device name to be searched.
* @param[in]        pp_device_control  The pointor of st_osal_device_control_t
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_UNKNOWN_NAME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_find_device_name(const char* p_device_name,
                                                              st_osal_device_control_t **pp_device_control);

/*******************************************************************************************************************//**
* @brief            Internal parameter check for axi bus name.
* @param[in]        p_buffer        The buffer address.
* @param[in]        buffer_size     The value of p_buffer size by byte.
* @param[in]        p_num_of_byte   The address of size_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_PAR
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_check_axi_bus_name(const char * p_buffer, size_t buffer_size, const size_t *const p_num_of_byte);

/*******************************************************************************************************************//**
* @brief            Internal create AXIBusNameList.
* @param[in]        total_length      Length of the list.
* @param[in]        buffer_size       Size of the provided buffer.
* @param[in]        p_buffer          Pointer to a buffer to write the string list to.
* @param[in]        axi_bus_control   The pointor of st_osal_axi_bus_control_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_create_axi_bus_list(size_t total_length, size_t buffer_size, char* p_buffer,
                                                                 const st_osal_axi_bus_control_t *axi_bus_control);

/*******************************************************************************************************************//**
* @brief            Get AxiBus name length.
* @param[in]        axi_id              AXI bus id
* @param[out]       pp_axi_bus_control  The pointor of st_osal_axi_bus_control_t
* @param[out]       p_name_length       AXI bus name length.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_get_axi_bus_name_len(osal_axi_bus_id_t axi_id,
                                                                  st_osal_axi_bus_control_t **pp_axi_bus_control,
                                                                  size_t *p_name_length);

/*******************************************************************************************************************//**
* @brief            Get pdev entry informations.
* @param[in/out]    p_pdev_entry    A pointer of pdev entry information
* @param[in]        num             The number of pdev entry
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_CONF
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_get_pdev_info(st_osal_pdev_entry_t *p_pdev_entry, size_t num);

/*******************************************************************************************************************//**
* @brief            Mmap a device of pdev entry.
* @param[in/out]    p_pdev_entry    A pointer of pdev entry information
* @param[in]        p_device_path   The device path
* @param[in]        entry_count     The pdev entry count
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_CONF
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_mmap_device_of_pdev(st_osal_pdev_entry_t *p_pdev_entry,
                                                                 const char *p_device_path,
                                                                 uint32_t entry_count);

/*******************************************************************************************************************//**
* @brief            Initialize mutex attributes.
* @param[in]        p_mutex_attr      The pointor of mutex attributes
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_init_mutex_attr(pthread_mutexattr_t *p_mutex_attr);

/*******************************************************************************************************************//**
* @brief            Internal initialize process
* @param[in]        p_mutex_attr        The pointor of mutex attributes
* @param[in]        p_osal_pdev_entry   A pointer of osal pdev entry
* @param[in]        pdev_entry_num      The number of pdev entry
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_CONF
* @retval           OSAL_RETURN_MEM
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_init_proc(pthread_mutexattr_t *p_mutex_attr,
                                                       const st_osal_pdev_entry_t *p_osal_pdev_entry,
                                                       size_t pdev_entry_num);

/*******************************************************************************************************************//**
* @brief            Internal initialize and allocate AXI bus list.
* @param[in]        dev_total_num       Total device number
* @param[in]        p_mutex_attr        The pointor of mutex attributes
* @param[in]        p_osal_pdev_entry   A pointer of osal pdev entry
* @param[in]        pdev_entry_num      The number of pdev entry
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_CONF
* @retval           OSAL_RETURN_MEM
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_malloc_axibus_list(size_t dev_total_num,
                                                             const pthread_mutexattr_t *p_mutex_attr,
                                                             const st_osal_pdev_entry_t *p_osal_pdev_entry,
                                                             size_t pdev_entry_num);

/*******************************************************************************************************************//**
* @brief            Initialize device control information.
* @param[in]        dev_total_num     Total device number
* @param[in]        p_mutex_attr      The pointor of mutex attributes
* @param[in]        p_axi_bus_control The pointor of axi bus control information
* @param[in]        p_osal_pdev_entry   A pointer of osal pdev entry
* @param[in]        pdev_entry_num      The number of pdev entry
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_CONF
* @retval           OSAL_RETURN_MEM
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_init_device_information(size_t dev_total_num,
                                                                     const pthread_mutexattr_t *p_mutex_attr,
                                                                     st_osal_axi_bus_control_t *p_axi_bus_control,
                                                                     const st_osal_pdev_entry_t *p_osal_pdev_entry,
                                                                     size_t pdev_entry_num);

/*******************************************************************************************************************//**
* @brief            Assign a mapped address
* @param[in]        p_device_control    The pointor of axi bus control information
* @param[in]        p_osal_pdev_entry   A pointer of osal pdev entry
* @param[in]        pdev_entry_num      The number of pdev entry
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_CONF
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_assign_mapped_addr(st_osal_device_control_t *p_device_control,
                                                                const st_osal_pdev_entry_t *p_osal_pdev_entry,
                                                                size_t pdev_entry_num);

/*******************************************************************************************************************//**
* @brief            Internal initialize AXI bus name list.
* @param[in]        p_axi_bus_control The pointor of axi bus control information
* @param[in]        p_device_control  The pointor of device control information
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_init_axibus_namelist(st_osal_axi_bus_control_t *p_axi_bus_control,
                                                               st_osal_device_control_t  *p_device_control);

/*******************************************************************************************************************//**
* @brief            Internal deinitialize for device inforamtion.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_deinit_device_info(void);

/*******************************************************************************************************************//**
* @brief            Internal deinitialize for pdev entry inforamtion.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_deinit_pdev_info(void);

/** @} */

/***********************************************************************************************************************
* Start of function R_OSAL_IoGetNumOfDevices()
* XOS3_OSAL_EMCOS_CD_CD_IO_001
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_044]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_045]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_IoGetNumOfDevices(const char* p_deviceType, size_t* const p_numOfDevices)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t           osal_ret        = OSAL_RETURN_OK;
    e_osal_return_t           api_ret;
    size_t                    dev_total_num;
    size_t                    dev_type_len;
    size_t                    loop_count      = 0;
    size_t                    device_count    = 0;
    st_osal_device_control_t  *device_control = NULL;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_io_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if ((NULL == p_deviceType) || (NULL == p_numOfDevices))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        dev_type_len = strnlen(p_deviceType, (size_t)OSAL_DEVICE_TYPE_NAME_MAX_LENGTH) + (size_t)1;
        if (OSAL_DEVICE_TYPE_NAME_MAX_LENGTH < (dev_type_len))
        {
            osal_ret = OSAL_RETURN_PAR;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        dev_total_num = R_OSAL_DCFG_GetNumOfDevice();

        while ((loop_count < dev_total_num) && (OSAL_RETURN_OK == osal_ret))
        {
            api_ret = R_OSAL_DCFG_GetDeviceInfo((uint32_t)loop_count, &device_control);
            if ((OSAL_RETURN_OK != api_ret) || (NULL == device_control) || (NULL == device_control->dev_info))
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                api_ret = osal_internal_compare_string(p_deviceType,
                                                       device_control->dev_info->type,
                                                       (size_t)OSAL_DEVICE_TYPE_NAME_MAX_LENGTH);
                if (OSAL_RETURN_OK == api_ret)
                {
                    device_count++;
                }
                else
                {
                    /* Do nothing */
                }
            }
            loop_count++;
        }   /* end of while((loop_count < dev_total_num) && (OSAL_RETURN_OK == osal_ret)). */
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    if (OSAL_RETURN_OK == osal_ret)
    {
        *p_numOfDevices = device_count;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_IoGetNumOfDevices()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_IoGetDeviceList()
* XOS3_OSAL_EMCOS_CD_CD_IO_002
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_036]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_037]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_IoGetDeviceList(const char * p_device_type_name,
                                       char * p_buffer,
                                       size_t buffer_size,
                                       size_t *const p_num_of_byte)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_return_t api_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_io_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_internal_io_check_get_device_list(p_device_type_name, p_buffer, buffer_size, p_num_of_byte);
    }
    else
    {
        /* Do nothing */
    }

    if ((OSAL_RETURN_OK == osal_ret) || (OSAL_RETURN_OK_RQST == osal_ret))
    {
        api_ret = osal_internal_io_get_device_list_proc(p_device_type_name, p_buffer, buffer_size, p_num_of_byte);

        if (OSAL_RETURN_OK != api_ret)
        {
            osal_ret = api_ret;
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_IoGetDeviceList()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_IoDeviceOpen()
* XOS3_OSAL_EMCOS_CD_CD_IO_003
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_029]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_030]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_IoDeviceOpen(const char * p_id, osal_device_handle_t *const p_handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret        = OSAL_RETURN_OK;
    st_osal_device_control_t *device_control = NULL;
    osal_device_handle_t     local_handle = NULL;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_io_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if ((NULL == p_id) || (NULL == p_handle))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_internal_io_check_device_id(p_id, &device_control);
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_internal_io_set_open_handle(&local_handle, device_control);
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK != osal_ret)
    {
        if ((OSAL_RETURN_BUSY != osal_ret) && (NULL != local_handle))
        {
            /* PRQA S 2812 1 # There is no problem because the pointer and size are checked in the this function function. */
            *p_handle = OSAL_DEVICE_HANDLE_INVALID;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
       /* PRQA S 2916 1 # The pointer refers to the Realities, so there is no problem. */
       *p_handle = local_handle;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_IoDeviceOpen()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_IoDeviceClose()
* XOS3_OSAL_EMCOS_CD_CD_IO_004
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_027]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_028]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_IoDeviceClose(osal_device_handle_t handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    e_osal_return_t api_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_io_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    else if ((OSAL_DEVICE_HANDLE_ID != handle->handle_id) || (false == handle->active))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        osal_ret = OSAL_RETURN_OK;
    }

    /* PRQA S 2996,2992 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        api_ret = R_OSAL_Internal_TimedlockMutex(&handle->inner_mtx, handle->inner_timeout);
        if (OSAL_RETURN_OK == api_ret)
        {
            osal_ret = osal_internal_io_device_close_proc(handle);

            os_ret = pthread_mutex_unlock(&handle->inner_mtx);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_IoDeviceClose()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_IoRead8()
* XOS3_OSAL_EMCOS_CD_CD_IO_005
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_052]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_053]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_054]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_IoRead8(osal_device_handle_t handle, uintptr_t offset, uint8_t *const p_data)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    const uint8_t   *buf;
    uintptr_t       divide_offset;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_io_check_rw_handle(handle, sizeof(uint8_t));

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0314 1 # The parameter must be (void *) and there is no side side-effect in this case. */
        osal_ret = osal_internal_io_check_read_param(handle, offset, sizeof(uint8_t), (void *const)p_data);
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0306 1 # Cast uint8_t* for a 1-byte read process from the IO register. No problem. */
        buf           = (uint8_t *)handle->base_addr;
        divide_offset = offset / sizeof(uint8_t);
        *p_data       = buf[divide_offset];
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_IoRead8()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_IoRead16()
* XOS3_OSAL_EMCOS_CD_CD_IO_006
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_046]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_047]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_IoRead16(osal_device_handle_t handle, uintptr_t offset, uint16_t *const p_data)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    const uint16_t  *buf;
    uintptr_t       divide_offset;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_io_check_rw_handle(handle, sizeof(uint16_t));

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0314 1 # The parameter must be (void *) and there is no side side-effect in this case. */
        osal_ret = osal_internal_io_check_read_param(handle, offset, sizeof(uint16_t), (void *const)p_data);
    }
    else
    {
        /* Do nothing */
    }

    /* PRQA S 2996,2992 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0306 1 # Cast uint16_t* for a 2-byte read process from the IO register. No problem. */
        buf           = (uint16_t *)handle->base_addr;
        divide_offset = offset / sizeof(uint16_t);
        *p_data       = buf[divide_offset];
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_IoRead16()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_IoRead32()
* XOS3_OSAL_EMCOS_CD_CD_IO_007
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_048]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_049]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_IoRead32(osal_device_handle_t handle, uintptr_t offset, uint32_t *const p_data)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    const uint32_t  *buf;
    uintptr_t       divide_offset;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_io_check_rw_handle(handle, sizeof(uint32_t));

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0314 1 # The parameter must be (void *) and there is no side side-effect in this case. */
        osal_ret = osal_internal_io_check_read_param(handle, offset, sizeof(uint32_t), (void *const)p_data);
    }
    else
    {
        /* Do nothing */
    }

    /* PRQA S 2996,2992 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0306 1 # Cast uint32_t* for a 4-byte read process from the IO register. No problem. */
        buf           = (uint32_t *)handle->base_addr;
        divide_offset = offset / sizeof(uint32_t);
        *p_data       = buf[divide_offset];
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_IoRead32()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_IoRead64()
* XOS3_OSAL_EMCOS_CD_CD_IO_008
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_050]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_051]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_IoRead64(osal_device_handle_t handle, uintptr_t offset, uint64_t *const p_data)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    const uint64_t  *buf;
    uintptr_t       divide_offset;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_io_check_rw_handle(handle, sizeof(uint64_t));

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0314 1 # The parameter must be (void *) and there is no side side-effect in this case. */
        osal_ret = osal_internal_io_check_read_param(handle, offset, sizeof(uint64_t), (void *const)p_data);
    }
    else
    {
        /* Do nothing */
    }
    /* PRQA S 2996,2992 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0306 1 # Cast uint64_t* for a 8-byte read process from the IO register. No problem. */
        buf           = (uint64_t *)handle->base_addr;
        divide_offset = offset / sizeof(uint64_t);
        *p_data       = buf[divide_offset];
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_IoRead64()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_IoWrite8()
* XOS3_OSAL_EMCOS_CD_CD_IO_009
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_061]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_062]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_063]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_IoWrite8(osal_device_handle_t handle, uintptr_t offset, uint8_t data)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    uint8_t         *buf;
    uintptr_t       divide_offset;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_io_check_rw_handle(handle, sizeof(uint8_t));

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_internal_io_check_write_param(handle, offset, sizeof(uint8_t));
    }
    else
    {
        /* Do nothing */
    }

    /* PRQA S 2996,2992 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0306 1 # Cast to uint8_t * for a 1-byte write process to the IO register. No problem.*/
        buf                = (uint8_t *)handle->base_addr;
        divide_offset      = offset / sizeof(uint8_t);
        buf[divide_offset] = data;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_IoWrite8()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_IoWrite16()
* XOS3_OSAL_EMCOS_CD_CD_IO_010
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_055]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_056]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_IoWrite16(osal_device_handle_t handle, uintptr_t offset, uint16_t data)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    uint16_t        *buf;
    uintptr_t       divide_offset;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_io_check_rw_handle(handle, sizeof(uint16_t));

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_internal_io_check_write_param(handle, offset, sizeof(uint16_t));
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0306 1 # Cast to uint16_t * for a 2-byte write process to the IO register. No problem. */
        buf                = (uint16_t *)handle->base_addr;
        divide_offset      = offset / sizeof(uint16_t);
        buf[divide_offset] = data;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_IoWrite16()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_IoWrite32()
* XOS3_OSAL_EMCOS_CD_CD_IO_011
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_057]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_058]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_IoWrite32(osal_device_handle_t handle, uintptr_t offset, uint32_t data)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    uint32_t        *buf;
    uintptr_t       divide_offset;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_io_check_rw_handle(handle, sizeof(uint32_t));
    
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_internal_io_check_write_param(handle, offset, sizeof(uint32_t));
    }
    else
    {
        /* Do nothing */
    }

    /* PRQA S 2996,2992 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0306 1 # Cast to uint32_t * for a 4-byte write process to the IO register. No problem. */
        buf                = (uint32_t *)handle->base_addr;
        divide_offset      = offset / sizeof(uint32_t);
        buf[divide_offset] = data;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_IoWrite32()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_IoWrite64()
* XOS3_OSAL_EMCOS_CD_CD_IO_012
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_059]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_060]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_IoWrite64(osal_device_handle_t handle, uintptr_t offset, uint64_t data)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    uint64_t        *buf;
    uintptr_t       divide_offset;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_io_check_rw_handle(handle, sizeof(uint64_t));

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_internal_io_check_write_param(handle, offset, sizeof(uint64_t));
    }
    else
    {
        /* Do nothing */
    }

    /* PRQA S 2996,2992 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0306 1 # Cast to uint64_t * for a 8-byte write process to the IO register. No problem. */
        buf                = (uint64_t *)handle->base_addr;
        divide_offset      = offset / sizeof(uint64_t);
        buf[divide_offset] = data;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_IoWrite64()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_IoBlockRead8()
* XOS3_OSAL_EMCOS_CD_CD_IO_013
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_015]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_016]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_017]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_IoBlockRead8(osal_device_handle_t handle, uintptr_t offset, uint8_t * p_data, size_t size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    const uint8_t   *buf;
    uintptr_t       divide_offset;
    size_t          loop_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_io_check_rw_handle(handle, sizeof(uint8_t));

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0314 1 # The parameter must be (void *) and there is no side side-effect in this case. */
        osal_ret = osal_internal_io_check_block_rw_param(handle, offset, size, sizeof(uint8_t), (void *const)p_data);
    }
    else
    {
        /* Do nothing */
    }

    /* PRQA S 2996,2992 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0306 1 # Cast uint8_t* for a 1-byte read process from the IO register. No problem.  */
        buf           = (uint8_t *)handle->base_addr;
        divide_offset = offset / sizeof(uint8_t);

        for (loop_count = 0; loop_count < (size / sizeof(uint8_t)); loop_count++)
        {
            p_data[loop_count] = buf[divide_offset + loop_count];
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_IoBlockRead8()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_IoBlockRead16()
* XOS3_OSAL_EMCOS_CD_CD_IO_014
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_009]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_010]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_IoBlockRead16(osal_device_handle_t handle, uintptr_t offset, uint16_t * p_data, size_t size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    const uint16_t  *buf;
    uintptr_t       divide_offset;
    size_t          loop_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_io_check_rw_handle(handle, sizeof(uint16_t));

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0314 1 # The parameter must be (void *) and there is no side side-effect in this case. */
        osal_ret = osal_internal_io_check_block_rw_param(handle, offset, size, sizeof(uint16_t), (void *const)p_data);
    }
    else
    {
        /* Do nothing */
    }

    /* PRQA S 2996,2992 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0306 1 # Cast uint16_t* for a 2-byte read process from the IO register. No problem.  */
        buf           = (uint16_t *)handle->base_addr;
        divide_offset = offset / sizeof(uint16_t);

        for (loop_count = 0; loop_count < (size / sizeof(uint16_t)); loop_count++)
        {
            p_data[loop_count] = buf[divide_offset + loop_count];
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_IoBlockRead16()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_IoBlockRead32()
* XOS3_OSAL_EMCOS_CD_CD_IO_015
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_011]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_012]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_IoBlockRead32(osal_device_handle_t handle, uintptr_t offset, uint32_t * p_data, size_t size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    const uint32_t  *buf;
    uintptr_t       divide_offset;
    size_t          loop_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_io_check_rw_handle(handle, sizeof(uint32_t));

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0314 1 # The parameter must be (void *) and there is no side side-effect in this case. */
        osal_ret = osal_internal_io_check_block_rw_param(handle, offset, size, sizeof(uint32_t), (void *const)p_data);
    }
    else
    {
        /* Do nothing */
    }

    /* PRQA S 2996,2992 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0306 1 # Cast uint32_t* for a 4-byte read process from the IO register. No problem.  */
        buf           = (uint32_t *)handle->base_addr;
        divide_offset = offset / sizeof(uint32_t);

        for (loop_count = 0; loop_count < (size / sizeof(uint32_t)); loop_count++)
        {
            p_data[loop_count] = buf[divide_offset + loop_count];
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_IoBlockRead32()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_IoBlockRead64()
* XOS3_OSAL_EMCOS_CD_CD_IO_016
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_013]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_014]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_IoBlockRead64(osal_device_handle_t handle, uintptr_t offset, uint64_t * p_data, size_t size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    const uint64_t  *buf;
    uintptr_t       divide_offset;
    size_t          loop_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_io_check_rw_handle(handle, sizeof(uint64_t));

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0314 1 # The parameter must be (void *) and there is no side side-effect in this case. */
        osal_ret = osal_internal_io_check_block_rw_param(handle, offset, size, sizeof(uint64_t), (void *const)p_data);
    }
    else
    {
        /* Do nothing */
    }
    /* PRQA S 2996,2992 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0306 1 # Cast uint64_t* for a 8-byte read process from the IO register. No problem.  */
        buf           = (uint64_t *)handle->base_addr;
        divide_offset = offset / sizeof(uint64_t);

        for (loop_count = 0; loop_count < (size / sizeof(uint64_t)); loop_count++)
        {
            p_data[loop_count] = buf[divide_offset + loop_count];
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_IoBlockRead64()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_IoBlockWrite8()
* XOS3_OSAL_EMCOS_CD_CD_IO_017
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_024]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_025]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_026]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_IoBlockWrite8(osal_device_handle_t handle, uintptr_t offset, const uint8_t * p_data, size_t size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    uint8_t         *buf;
    uintptr_t       divide_offset;
    size_t          loop_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_io_check_rw_handle(handle, sizeof(uint8_t));

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0311,0314 1 # Void * and uint8_t * cast, integer pointer, no problem. */
        osal_ret = osal_internal_io_check_block_rw_param(handle, offset, size, sizeof(uint8_t), (void *const)p_data);
    }
    else
    {
        /* Do nothing */
    }
    /* PRQA S 2996,2992 2 # It is a check for robustness, because it is carried out based on the design is no problem.
      True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0306 1 # Cast to uint8_t * for a 1-byte write process to the IO register. No problem */
        buf           = (uint8_t *)handle->base_addr;
        divide_offset = offset / sizeof(uint8_t);

        for (loop_count = 0; loop_count < (size / sizeof(uint8_t)); loop_count++)
        {
            buf[divide_offset + loop_count] = p_data[loop_count];
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_IoBlockWrite8()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_IoBlockWrite16()
* XOS3_OSAL_EMCOS_CD_CD_IO_018
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_018]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_019]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_IoBlockWrite16(osal_device_handle_t handle, uintptr_t offset, const uint16_t * p_data, size_t size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    uint16_t        *buf;
    uintptr_t       divide_offset;
    size_t          loop_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_io_check_rw_handle(handle, sizeof(uint16_t));
    if (OSAL_RETURN_OK == osal_ret)
    {
       /* PRQA S 0311,0314 1 # Void * and uint16_t * cast, integer pointer, no problem. */
        osal_ret = osal_internal_io_check_block_rw_param(handle, offset, size, sizeof(uint16_t), (void *const)p_data);
    }
    else
    {
        /* Do nothing */
    }

    /* PRQA S 2996,2992 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0306 1 # Cast to uint16_t * for a 2-byte write process to the IO register. No problem */
        buf           = (uint16_t *)handle->base_addr;
        divide_offset = offset / sizeof(uint16_t);

        for (loop_count = 0; loop_count < (size / sizeof(uint16_t)); loop_count++)
        {
            buf[divide_offset + loop_count] = p_data[loop_count];
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_IoBlockWrite16()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_IoBlockWrite32()
* XOS3_OSAL_EMCOS_CD_CD_IO_019
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_020]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_021]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_IoBlockWrite32(osal_device_handle_t handle, uintptr_t offset, const uint32_t * p_data, size_t size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    uint32_t        *buf;
    uintptr_t       divide_offset;
    size_t          loop_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_io_check_rw_handle(handle, sizeof(uint32_t));

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0311,0314 1 # Void * and uint32_t * cast, integer pointer, no problem. */
        osal_ret = osal_internal_io_check_block_rw_param(handle, offset, size, sizeof(uint32_t), (void *const)p_data);
    }
    else
    {
        /* Do nothing */
    }
    /* PRQA S 2996,2992 1 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0306 1 # Cast to uint32_t * for a 4-byte write process to the IO register. No problem */
        buf           = (uint32_t *)handle->base_addr;
        divide_offset = offset / sizeof(uint32_t);

        for (loop_count = 0; loop_count < (size / sizeof(uint32_t)); loop_count++)
        {
            buf[divide_offset + loop_count] = p_data[loop_count];
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_IoBlockWrite32()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_IoBlockWrite64()
* XOS3_OSAL_EMCOS_CD_CD_IO_020
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_022]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_023]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_IoBlockWrite64(osal_device_handle_t handle, uintptr_t offset, const uint64_t * p_data, size_t size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    uint64_t        *buf;
    uintptr_t       divide_offset;
    size_t          loop_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0311 1 # No change on the required cast on the system. */
    osal_ret = osal_internal_io_check_rw_handle(handle, sizeof(uint64_t));

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0311,0314 1 # Void * and uint64_t * cast, integer pointer, no problem. */
        osal_ret = osal_internal_io_check_block_rw_param(handle, offset, size, sizeof(uint64_t), (void *const)p_data);
    }
    else
    {
        /* Do nothing */
    }
    /* PRQA S 2996,2992 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0306 1 # Cast to uint64_t * for a 8-byte write process to the IO register. No problem */
        buf           = (uint64_t *)handle->base_addr;
        divide_offset = offset / sizeof(uint64_t);

        for (loop_count = 0; loop_count < (size / sizeof(uint64_t)); loop_count++)
        {
            buf[divide_offset + loop_count] = p_data[loop_count];
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_IoBlockWrite64()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_IoGetDeviceAxiBusId()
* XOS3_OSAL_EMCOS_CD_CD_IO_021
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_035]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_IoGetDeviceAxiBusId(osal_device_handle_t device, osal_axi_bus_id_t *const p_axi_id)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_io_check_get_axi_bus_id(device, p_axi_id);

    if (OSAL_RETURN_OK == osal_ret)
    {
        *p_axi_id = device->axi_bus_id;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_IoGetDeviceAxiBusId()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_IoGetAxiBusIdFromDeviceName()
* XOS3_OSAL_EMCOS_CD_CD_IO_022
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_031]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_IoGetAxiBusIdFromDeviceName(const char* p_device_name, osal_axi_bus_id_t* const p_axi_id)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret;
    st_osal_device_control_t *device_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_io_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if ((NULL == p_device_name) || (NULL == p_axi_id))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        osal_ret = OSAL_RETURN_OK;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_internal_io_find_device_name(p_device_name, &device_control);
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        *p_axi_id = device_control->axi_bus_id;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_IoGetAxiBusIdFromDeviceName()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_IoGetAxiBusNameList()
* XOS3_OSAL_EMCOS_CD_CD_IO_023
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_033]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_034]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_IoGetAxiBusNameList(char * p_buffer, size_t buffer_size, size_t *const p_num_of_byte)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t           osal_ret;
    e_osal_return_t           api_ret;
    st_osal_axi_bus_control_t *axi_bus_control = NULL;
    size_t                    loop_count;
    size_t                    total_length     = 0;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_io_check_axi_bus_name(p_buffer, buffer_size, p_num_of_byte);

    if ((OSAL_RETURN_OK == osal_ret) || (OSAL_RETURN_OK_RQST == osal_ret))
    {
        api_ret = R_OSAL_DCFG_GetAxiBusInfo(&axi_bus_control);
        if ((OSAL_RETURN_OK == api_ret) && (NULL != axi_bus_control))
        {
            for (loop_count = 0; loop_count < axi_bus_control->num; loop_count++)
            {
                total_length += strnlen(axi_bus_control->axi_bus_list[loop_count].axi_bus_name,
                                        (size_t)OSAL_AXI_BUS_NAME_MAX_LENGTH) + (size_t)1;
            }
        }
        else
        {
            osal_ret = OSAL_RETURN_FAIL;
        }

        /* Process only for OSAL_RETURN_OK, not for OSAL_RETURN_OK_RQST. */
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = osal_internal_io_create_axi_bus_list(total_length, buffer_size, p_buffer, axi_bus_control);
        }
        else
        {
            /* Do nothing */
        }

        if ((OSAL_RETURN_OK == osal_ret) || (OSAL_RETURN_OK_RQST == osal_ret))
        {
            *p_num_of_byte = total_length;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if((OSAL_RETURN_OK == osal_ret) || (OSAL_RETURN_OK_RQST == osal_ret)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_IoGetAxiBusNameList()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_IoGetAxiBusName()
* XOS3_OSAL_EMCOS_CD_CD_IO_024
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_032]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_IoGetAxiBusName(osal_axi_bus_id_t axi_id, char * p_buffer, size_t buffer_size, size_t *const p_num_of_byte)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t           osal_ret;
    e_osal_return_t           api_ret;
    st_osal_axi_bus_control_t *axi_bus_control = NULL;
    size_t                    name_length;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_io_check_axi_bus_name(p_buffer, buffer_size, p_num_of_byte);

    if ((OSAL_RETURN_OK == osal_ret) || (OSAL_RETURN_OK_RQST == osal_ret))
    {
        api_ret = osal_internal_io_get_axi_bus_name_len(axi_id, &axi_bus_control, &name_length);
        if (OSAL_RETURN_OK != api_ret)
        {
            osal_ret = api_ret;
        }
        else
        {
            /* Do nothing */
        }

        /* Process only for OSAL_RETURN_OK, not for OSAL_RETURN_OK_RQST. */
        if(OSAL_RETURN_OK == osal_ret)
        {
            if ((name_length + 1U) <= buffer_size)
            {
                /* PRQA S 2812 2 # There is no problem because the pointer and size are checked 
                 in the osal_internal_io_check_axi_bus_name function. */
                (void)strncpy(p_buffer, axi_bus_control->axi_bus_list[axi_id].axi_bus_name, buffer_size);
                /* PRQA S 2822 2 # There is no problem because the pointer and size are checked
                 in the osal_internal_io_check_axi_bus_name function. */
                p_buffer[name_length] = '\0';
            }
            else
            {
                osal_ret = OSAL_RETURN_PAR;
            }
        }
        else
        {
            /* Do nothing */
        }

        if ((OSAL_RETURN_OK == osal_ret) || (OSAL_RETURN_OK_RQST == osal_ret))
        {
            *p_num_of_byte = name_length + (size_t)1;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if((OSAL_RETURN_OK == osal_ret) || (OSAL_RETURN_OK_RQST == osal_ret)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_IoGetAxiBusName()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_IoGetNumOfAxiIdsForAxiBusName()
* XOS3_OSAL_EMCOS_CD_CD_IO_025
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_041]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_IoGetNumOfAxiIdsForAxiBusName(const char* p_axi_bus_name, size_t *p_num_of_axi_ids)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    char            domain_name[OSAL_BUS_DOMAIN_SIZE] = "";

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_io_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if ((NULL == p_axi_bus_name) || (NULL == p_num_of_axi_ids))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        osal_ret = osal_internal_get_domain_name(p_axi_bus_name, domain_name);
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = osal_internal_check_axiidlist_for_domainname(p_axi_bus_name, domain_name, NULL, 0, p_num_of_axi_ids);
        }
        else
        {
            /* Do nothing */
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_IoGetNumOfAxiIdsForAxiBusName()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_IoGetAxiIdListForAxiBusName()
* XOS3_OSAL_EMCOS_CD_CD_IO_026
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_038]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_IoGetAxiIdListForAxiBusName(const char* p_axi_bus_name, osal_axi_bus_id_t* p_array,
                                                   size_t array_size, size_t* const p_num_of_written_axi_ids)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    size_t          numofaxiids                       = 0;
    char            domain_name[OSAL_BUS_DOMAIN_SIZE] = "";

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_io_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if ((NULL == p_axi_bus_name) || (NULL == p_num_of_written_axi_ids))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if ((NULL == p_array) && ((size_t)0 != array_size))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        osal_ret = osal_internal_get_domain_name(p_axi_bus_name, domain_name);
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = osal_internal_check_axiidlist_for_domainname(p_axi_bus_name, domain_name, p_array, array_size, &numofaxiids);
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if (false == gs_osal_io_is_initialized) */

    if (OSAL_RETURN_OK == osal_ret)
    {
        if ((NULL != p_array) && (array_size < numofaxiids))
        {
            osal_ret = OSAL_RETURN_PAR;
            /* PRQA S 0314 1 # The parameter must be void* and there is no side side-effect in this case. */
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
/***********************************************************************************************************************
* End of function R_OSAL_IoGetAxiIdListForAxiBusName()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_IoGetNumOfAxiIdsForDeviceHdl()
* XOS3_OSAL_EMCOS_CD_CD_IO_027
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_042]
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_IoGetNumOfAxiIdsForDeviceHdl(osal_device_handle_t device_hdl, size_t* const p_num_of_axi_ids)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_io_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (NULL == device_hdl)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        else if (OSAL_DEVICE_HANDLE_ID != device_hdl->handle_id)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        else if (false == device_hdl->active)
        {
            osal_ret = OSAL_RETURN_HANDLE;
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

    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        if (NULL == p_num_of_axi_ids)
        {
            osal_ret = OSAL_RETURN_PAR;
        }
        else
        {
            if (NULL == device_hdl->dev_info->utlb_assignment)
            {
                *p_num_of_axi_ids = 0U;
            }
            else
            {
                *p_num_of_axi_ids = device_hdl->dev_info->utlb_assignment->table_size;
            }
        }   /* end of if (false == gs_osal_io_is_initialized) */
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_IoGetNumOfAxiIdsForDeviceHdl()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_IoGetAxiIdListForDeviceHdl()
* XOS3_OSAL_EMCOS_CD_CD_IO_028
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_039]
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_IoGetAxiIdListForDeviceHdl(osal_device_handle_t device_hdl, osal_axi_bus_id_t* p_array,
                                                  size_t array_size, size_t* const p_num_of_written_axi_ids)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t          numofaxiids;
    size_t          invalid_axiids;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_io_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (NULL == device_hdl)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        else if (OSAL_DEVICE_HANDLE_ID != device_hdl->handle_id)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        else if (false == device_hdl->active)
        {
            osal_ret = OSAL_RETURN_HANDLE;
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

    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        if (NULL == p_num_of_written_axi_ids)
        {
            osal_ret = OSAL_RETURN_PAR;
        }
        else if ((NULL == p_array) && ((size_t)0 != array_size))
        {
            osal_ret = OSAL_RETURN_PAR;
        }
        else
        {
            osal_internal_create_axiidlist_for_devicename(device_hdl->dev_info->id, p_array, array_size, &numofaxiids, &invalid_axiids);
        }   /* end of if (false == gs_osal_io_is_initialized) */
    }
    else
    {
        /* Do nothing */
    }

    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        if ((NULL != p_array) && (array_size < numofaxiids))
        {
            osal_ret = OSAL_RETURN_PAR;
            /* PRQA S 0314 1 # The parameter must be void* and there is no side side-effect in this case. */
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
/***********************************************************************************************************************
* End of function R_OSAL_IoGetAxiIdListForDeviceHdl()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_IoGetNumOfAxiIdsForDeviceName()
* XOS3_OSAL_EMCOS_CD_CD_IO_029
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_043]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_IoGetNumOfAxiIdsForDeviceName(const char* p_device_name, size_t* const p_num_of_axi_ids)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_io_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if ((NULL == p_device_name) || (NULL == p_num_of_axi_ids))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        osal_ret = osal_internal_get_utlb_assignment_for_devicename(p_device_name, p_num_of_axi_ids);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_IoGetNumOfAxiIdsForDeviceName()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_IoGetAxiIdListForDeviceName()
* XOS3_OSAL_EMCOS_CD_CD_IO_030
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_040]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_IoGetAxiIdListForDeviceName(const char* p_device_name, osal_axi_bus_id_t* p_array,
                                                   size_t array_size, size_t* const p_num_of_written_axi_ids)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    size_t          numofaxiids;
    size_t          invalid_axiids;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_io_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if ((NULL == p_device_name) || (NULL == p_num_of_written_axi_ids))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if ((NULL == p_array) && ((size_t)0 != array_size))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        osal_ret = OSAL_RETURN_OK;
        osal_internal_create_axiidlist_for_devicename(p_device_name, p_array, array_size, &numofaxiids, &invalid_axiids);
    }   /* end of if (false == gs_osal_io_is_initialized) */

    if (OSAL_RETURN_OK == osal_ret)
    {
        if ((0U == numofaxiids) && (0U == invalid_axiids))
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
            /* PRQA S 0314 1 # The parameter must be void* and there is no side side-effect in this case. */
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
/***********************************************************************************************************************
* End of function R_OSAL_IoGetAxiIdListForDeviceName()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_io_initialize()
* XOS3_OSAL_EMCOS_CD_CD_IO_031
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_006]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_007]
***********************************************************************************************************************/
e_osal_return_t osal_io_initialize(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t           osal_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    pthread_mutexattr_t       mutex_attr;
    size_t                    pdev_entry_num;
    st_osal_pdev_entry_t      *osal_pdev_entry;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (true == gs_osal_io_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else
    {
        gs_osal_io_is_initialized = true;

        /* Get pdev entry */
        pdev_entry_num = R_OSAL_DCFG_GetNumOfPdevEntry();
        /* PRQA S 5118,2707 1 # Safty Manual, malloc use are allowed. No problem because it is called by Initilize.*/
        osal_pdev_entry = malloc(sizeof(st_osal_pdev_entry_t) * pdev_entry_num);
        if (NULL == osal_pdev_entry)
        {
            osal_ret = OSAL_RETURN_MEM;
        }
        else
        {
            osal_ret = osal_internal_io_get_pdev_info(osal_pdev_entry, pdev_entry_num);
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            g_pdev_entry.num   = pdev_entry_num;
            g_pdev_entry.entry = osal_pdev_entry;

            osal_ret = osal_internal_io_init_mutex_attr(&mutex_attr);
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = osal_internal_io_init_proc(&mutex_attr,
                                                  osal_pdev_entry,
                                                  pdev_entry_num);
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK != osal_ret)
        {
            /* For post-processing in the event of an error, do not perform a return value determination. */
            (void)osal_io_deinitialize();
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if(true == gs_osal_io_is_initialized). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_io_initialize()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_io_deinitialize()
* XOS3_OSAL_EMCOS_CD_CD_IO_032
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_004]
***********************************************************************************************************************/
e_osal_return_t osal_io_deinitialize(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t           osal_ret;
    st_osal_axi_bus_control_t *axi_bus_control = NULL;
    size_t                    count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    gs_osal_io_is_initialized = false;

    osal_ret = R_OSAL_DCFG_GetAxiBusInfo(&axi_bus_control);
    if ((OSAL_RETURN_OK == osal_ret) && (NULL != axi_bus_control))
    {
        if (NULL != axi_bus_control->axi_bus_list)
        {
            /* PRQA S 5118,0314 1 # Safty Manual, free use are allowed. No problem because it is called by DeInitilize. */
            free((void*)axi_bus_control->axi_bus_list);
            axi_bus_control->axi_bus_list = NULL;
        }
        else
        {
            /* Do nothing */
        }

        if (NULL != gsp_axi_bus_name_info)
        {
            for(count = 0; count < gs_axi_bus_name_info_array_size; count++)
            {
                if (NULL != gsp_axi_bus_name_info[count].dev_names)
                {
                    /* PRQA S 5118,0314 1 # Safty Manual, free use are allowed. No problem because it is called by DeInitilize. */
                    free((void*)gsp_axi_bus_name_info[count].dev_names);
                    gsp_axi_bus_name_info[count].dev_names        = NULL;
                    gsp_axi_bus_name_info[count].dev_names_length = (size_t)0;
                }
                else
                {
                    /* Do nothing */
                }
            }
            /* PRQA S 5118,0314 1 # Safty Manual, free use are allowed. No problem because it is called by DeInitilize. */
            free((void*)gsp_axi_bus_name_info);
            gsp_axi_bus_name_info = NULL;

            gs_axi_bus_name_info_array_size = 0;
        }
        else
        {
            /* Do nothing */
        }   /* end of if (NULL != gsp_axi_bus_name_info) */

        osal_ret = osal_internal_io_deinit_device_info();
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = osal_internal_io_deinit_pdev_info();
        }
    }
    else
    {
        osal_ret = OSAL_RETURN_FAIL;
    }   /* end of if ((OSAL_RETURN_OK == osal_ret) && (NULL != axi_bus_control)) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_io_deinitialize()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_io_deinitialize_pre()
* XOS3_OSAL_EMCOS_CD_CD_IO_033
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_005]
***********************************************************************************************************************/
e_osal_return_t osal_io_deinitialize_pre(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_io_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else
    {
        gs_osal_io_is_initialized = false;
        osal_ret = osal_internal_io_deinit_pre_proc();
    }

    if (OSAL_RETURN_BUSY == osal_ret)
    {
        gs_osal_io_is_initialized = true;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_io_deinitialize_pre()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_io_set_is_init()
* XOS3_OSAL_EMCOS_CD_CD_IO_034
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_008]
***********************************************************************************************************************/
void osal_io_set_is_init(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    gs_osal_io_is_initialized = true;

    return;
}
/***********************************************************************************************************************
* End of function osal_io_set_is_init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_io_get_pdev_info()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_get_pdev_info(st_osal_pdev_entry_t *p_pdev_entry, size_t num)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t           osal_ret = OSAL_RETURN_OK;
    uint32_t                  count;
    size_t                    name_count;
    size_t                    dev_length;
    char                      path[OSAL_DEVICE_NAME_MAX_LENGTH + 6UL] = "/pdev/";

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    for (count = 0; count < num; count++)
    {
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_DCFG_GetPdevEntryName(count, &p_pdev_entry[count].name);
            if (OSAL_RETURN_OK != osal_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
                p_pdev_entry[count].fd = -1;
            }
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            dev_length = strnlen(p_pdev_entry[count].name, (size_t)OSAL_DEVICE_NAME_MAX_LENGTH);
            for (name_count = 0; dev_length >= name_count; name_count++)
            {
                path[name_count+6U] = p_pdev_entry[count].name[name_count];
            }
            path[dev_length+6U+1U] = '\0';

            osal_ret = osal_internal_io_mmap_device_of_pdev(p_pdev_entry, path, count);
        }

        if (OSAL_RETURN_OK != osal_ret)
        {
            if (-1 != p_pdev_entry[count].fd)
            {
                (void)close(p_pdev_entry[count].fd);
            }
            p_pdev_entry[count].fd         = -1;
            p_pdev_entry[count].start_addr = (uintptr_t)NULL;
        }
    }   /* end of for(count = 0; count < num; count++). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_io_get_pdev_info()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_io_mmap_device_of_pdev()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_mmap_device_of_pdev(st_osal_pdev_entry_t *p_pdev_entry,
                                                                 const char *p_device_path,
                                                                 uint32_t entry_count)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t           osal_ret = OSAL_RETURN_OK;
    pdev_memrange_t           pdevmr;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                       os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 1291 1 # O_RDWR uses octal and is used in accordance with the specification of open(). */
    p_pdev_entry[entry_count].fd = open(p_device_path, O_RDWR);
    if (-1 != p_pdev_entry[entry_count].fd)
    {
        /* PRQA S 2855 2 # It is used according to the specifications defined in eMCOS. */
        /* PRQA S 0314 1 # The 3rd parameter must be (void *), and there is no side side-effect in this case. */
        os_ret = posix_devctl(p_pdev_entry[entry_count].fd, PDEV_MEMRANGE_GET_INFO(0), (void*)&pdevmr, sizeof(pdev_memrange_t), NULL);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /*  Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* PRQA S 0326 1 # No problem for casting from void * to object pointer.*/
            if ((((uintptr_t)NULL == pdevmr.base) && (0U != pdevmr.len)) ||
                /* PRQA S 0326 1 # No problem for casting from void * to object pointer.*/
                (((uintptr_t)NULL != pdevmr.base) && (0U == pdevmr.len)))
            {
                osal_ret = OSAL_RETURN_CONF;
            }
            else
            {
                p_pdev_entry[entry_count].start_addr = (uintptr_t)pdevmr.base;
                p_pdev_entry[entry_count].end_addr   = (uintptr_t)pdevmr.base + (uintptr_t)pdevmr.len;
            }
        }
        else
        {
            /*  Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            if (0U == (pdevmr.base & (4096U - 1U)))
            {
                /* PRQA S 0326 1 # No problem for casting from void * to object pointer. */
                p_pdev_entry[entry_count].maped_start_addr = (uintptr_t)mmap(NULL,
                                                                             pdevmr.len,
                                                                             /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
                                                                             (int)pdevmr.attrs,
                                                                             /* PRQA S 4447,0499 1 # It is used according to the specifications defined in eMCOS. */
                                                                             (MAP_SHARED | MAP_POPULATE),
                                                                             p_pdev_entry[entry_count].fd,
                                                                             (off_t)pdevmr.base);
                /* PRQA S 0326 1 # No problem for casting from void * to object pointer. */
                if ((uintptr_t)MAP_FAILED == p_pdev_entry[entry_count].maped_start_addr)
                {
                    osal_ret = OSAL_RETURN_MEM;
                }
                else
                {
                    /* Do nothing */
                }
            }
            else
            {
                (void)close(p_pdev_entry[entry_count].fd);
                p_pdev_entry[entry_count].fd         = -1;
                /* PRQA S 0326 1 # No problem for casting from void * to object pointer.*/
                p_pdev_entry[entry_count].start_addr = (uintptr_t)NULL;
            }   /* end of if(0U == (pdevmr.base & (4096U - 1U))). */
        }
        else
        {
            /* Do nothing */
        }   /* end of if(OSAL_RETURN_OK == osal_ret). */
    }   /* end of if(-1 != p_pdev_entry[entry_count].fd). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_io_mmap_device_of_pdev()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_io_init_mutex_attr()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_init_mutex_attr(pthread_mutexattr_t *p_mutex_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    os_ret = pthread_mutexattr_init(p_mutex_attr);
    if (0 != os_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
        os_ret = pthread_mutexattr_settype(p_mutex_attr, (int)PTHREAD_MUTEX_ERRORCHECK);
        if (0 != os_ret)
        {
            /* For post-processing in the event of an error, do not perform a return value determination. */
            (void)pthread_mutexattr_destroy(p_mutex_attr);
            osal_ret = OSAL_RETURN_FAIL;
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

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_io_init_mutex_attr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_io_init_proc()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_init_proc(pthread_mutexattr_t *p_mutex_attr,
                                                       const st_osal_pdev_entry_t *p_osal_pdev_entry,
                                                       size_t pdev_entry_num)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;
    size_t          dev_total_num;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    dev_total_num = R_OSAL_DCFG_GetNumOfDevice();
    if (0U == dev_total_num)
    {
        /* For post-processing in the event of an error, do not perform a return value determination. */
        (void)pthread_mutexattr_destroy(p_mutex_attr);
        osal_ret = OSAL_RETURN_CONF;
    }
    else
    {
        osal_ret = osal_internal_malloc_axibus_list(dev_total_num,
                                                    p_mutex_attr,
                                                    p_osal_pdev_entry,
                                                    pdev_entry_num);
        os_ret = pthread_mutexattr_destroy(p_mutex_attr);
        if ((OSAL_RETURN_OK == osal_ret) && (0 != os_ret))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if(0U == dev_total_num). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_io_init_proc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_malloc_axibus_list()
***********************************************************************************************************************/
/* PRQA S 2980 3 # Arguments are used.it is carried out based on the design is no problem. True if the correct handle was passed. False detection of the tool.*/
OSAL_STATIC e_osal_return_t osal_internal_malloc_axibus_list(size_t dev_total_num,
                                                             const pthread_mutexattr_t *p_mutex_attr,
                                                             const st_osal_pdev_entry_t *p_osal_pdev_entry,
                                                             size_t pdev_entry_num)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    const char      str_invalid[8] = "invalid";
    const char      str_mmipa[8]   = "mm(ipa)";
    const size_t    local_str_size = sizeof(str_invalid);
    st_osal_axi_bus_control_t *p_axi_bus_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_DCFG_GetAxiBusInfo(&p_axi_bus_control);
    if ((OSAL_RETURN_OK == osal_ret) && (NULL != p_axi_bus_control))
    {
        if (NULL == p_axi_bus_control->axi_bus_list)
        {
            /* PRQA S 5118 1 # Safty Manual, malloc use are allowed. No problem because it is called by Initilize.*/
            p_axi_bus_control->axi_bus_list = malloc(sizeof(st_osal_axi_bus_list_t) * dev_total_num);
            if (NULL == p_axi_bus_control->axi_bus_list)
            {
                osal_ret = OSAL_RETURN_MEM;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            osal_ret = OSAL_RETURN_STATE;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            (void)strncpy(p_axi_bus_control->axi_bus_list[0].axi_bus_name, str_invalid, local_str_size);
            (void)strncpy(p_axi_bus_control->axi_bus_list[1].axi_bus_name, str_mmipa,   local_str_size);
            p_axi_bus_control->num = 2;
            /* PRQA S 1338 2 # This variable is used in the return value.it is carried out based on the design is no problem.
             True if the correct handle was passed. False detection of the tool.*/
            osal_ret = osal_internal_io_init_device_information(dev_total_num,
                                                                p_mutex_attr,
                                                                p_axi_bus_control,
                                                                p_osal_pdev_entry,
                                                                pdev_entry_num);
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = osal_internal_register_axi_bus_info_last_data();
        }
        else
        {
            /* Do nothing */
        }

    }
    else
    {
        /* Do nothing */
    }   /* end of if((OSAL_RETURN_OK == osal_ret) && (NULL != p_axi_bus_control)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_malloc_axibus_list()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_io_init_device_information()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_init_device_information(size_t dev_total_num,
                                                                     const pthread_mutexattr_t *p_mutex_attr,
                                                                     st_osal_axi_bus_control_t *p_axi_bus_control,
                                                                     const st_osal_pdev_entry_t *p_osal_pdev_entry,
                                                                     size_t pdev_entry_num)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret        = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                      os_ret;
    size_t                   dev_count       = 0;
    st_osal_device_control_t *device_control = NULL;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    while ((dev_count < dev_total_num) && (OSAL_RETURN_OK == osal_ret))
    {
        osal_ret = R_OSAL_DCFG_GetDeviceInfo((uint32_t)dev_count, &device_control);
        if ((OSAL_RETURN_OK != osal_ret) || (NULL == device_control) || (NULL == device_control->dev_info))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* PRQA S 2736 1 # No problem because osal_internal_io_deinit_device_info is called pthread_mutex_destroy. */
            os_ret = pthread_mutex_init(&device_control->inner_mtx, p_mutex_attr);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = osal_internal_io_assign_mapped_addr(device_control, p_osal_pdev_entry, pdev_entry_num);
            }
            else
            {
                /* Do nothing */
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = osal_internal_init_axibus_namelist(p_axi_bus_control, device_control);
            }
            else
            {
                /* Do nothing */
            }

            if(OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = osal_internal_create_axibus_info(device_control);
            }
            else
            {
                /* Do nothing */
            }

            if(OSAL_RETURN_OK == osal_ret)
            {
                device_control->active = false;
            }
            else
            {
                /* Do nothing */
            }
            /* PRQA S 2053 2 #  Comments for coding rule EST3.1 compliance. */
        }   /* end of if((OSAL_RETURN_OK != osal_ret) || (NULL == device_control) ||
                         (NULL == device_control->dev_info)). */
        dev_count++;
    }   /* end of while((dev_count < dev_total_num) && (OSAL_RETURN_OK == osal_ret)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_io_init_device_information()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_io_assign_mapped_addr()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_assign_mapped_addr(st_osal_device_control_t *p_device_control,
                                                                const st_osal_pdev_entry_t *p_osal_pdev_entry,
                                                                size_t pdev_entry_num)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t          pdev_entry_count;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    for (pdev_entry_count = 0; pdev_entry_count < pdev_entry_num; pdev_entry_count++)
    {
        os_ret = strncmp(p_osal_pdev_entry[pdev_entry_count].name,
                         p_device_control->dev_info->pdev_group,
                         (size_t)OSAL_DCFG_PDEV_GROUP_MAX_NUM);
        if (0 == os_ret)
        {
            break;
        }
    }

    if ((pdev_entry_count == pdev_entry_num) || (-1 == p_osal_pdev_entry[pdev_entry_count].fd))
    {
        /* Not detect or open fail*/
        p_device_control->irq_channels = 0U;
        /* PRQA S 0326 1 # No problem for casting from void * to object pointer.*/
        p_device_control->base_addr    = (uintptr_t)NULL;
        p_device_control->len          = 0xFFFFFFFFU;
    }
    else
    {
        /* PRQA S 0326 1 # No problem for casting from void * to object pointer.*/
        if (((uintptr_t)NULL != p_device_control->dev_info->address) && (0U != p_device_control->dev_info->range))
        {
            if((p_osal_pdev_entry[pdev_entry_count].start_addr <= p_device_control->dev_info->address) &&
               (p_osal_pdev_entry[pdev_entry_count].end_addr >= (p_device_control->dev_info->address + p_device_control->dev_info->range)))
            {
                p_device_control->fd           = p_osal_pdev_entry[pdev_entry_count].fd;
                p_device_control->base_addr    = p_osal_pdev_entry[pdev_entry_count].maped_start_addr +
                                                 (p_device_control->dev_info->address - p_osal_pdev_entry[pdev_entry_count].start_addr);
                p_device_control->len          = (uint32_t)p_device_control->dev_info->range;
                p_device_control->irq_channels = (uint32_t)p_device_control->dev_info->irq_channels;
            }
        }
        /* PRQA S 0326 1 # No problem for casting from void * to object pointer.*/
        else if (((uintptr_t)NULL == p_device_control->dev_info->address) && (0U == p_device_control->dev_info->range))
        {
            p_device_control->fd           = p_osal_pdev_entry[pdev_entry_count].fd;
            /* PRQA S 0326 1 # No problem for casting from void * to object pointer.*/
            p_device_control->base_addr    = (uintptr_t)NULL;
            p_device_control->len          = 0U;
            p_device_control->irq_channels = (uint32_t)p_device_control->dev_info->irq_channels;
        }
        else
        {
            osal_ret = OSAL_RETURN_CONF;
        }   /* end of if(((uintptr_t)NULL != p_device_control->dev_info->address) && (0U != p_device_control->dev_info->range)). */
    }   /* end of if((pdev_entry_count == pdev_entry_num) || (-1 == p_osal_pdev_entry[pdev_entry_count].fd)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_io_assign_mapped_addr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_init_axibus_namelist()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_init_axibus_namelist(st_osal_axi_bus_control_t *p_axi_bus_control,
                                                               st_osal_device_control_t  *p_device_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret     = OSAL_RETURN_OK;
    e_osal_return_t int_osal_ret;
    size_t          loop_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL != p_device_control->dev_info->axi_bus_name)
    {
        for (loop_count = 0; loop_count < p_axi_bus_control->num; loop_count++)
        {
            int_osal_ret = osal_internal_compare_string(p_axi_bus_control->axi_bus_list[loop_count].axi_bus_name,
                                                        p_device_control->dev_info->axi_bus_name,
                                                        (size_t)OSAL_AXI_BUS_NAME_MAX_LENGTH);
            if (OSAL_RETURN_OK == int_osal_ret)
            {
                p_device_control->axi_bus_id = loop_count;
                break;
            }
            else
            {
                /* Do nothing */
            }
        }

        if (loop_count == p_axi_bus_control->num)
        {
            p_device_control->axi_bus_id = p_axi_bus_control->num;

            (void)strncpy(p_axi_bus_control->axi_bus_list[p_axi_bus_control->num].axi_bus_name,
                          p_device_control->dev_info->axi_bus_name,
                          (size_t)OSAL_AXI_BUS_NAME_MAX_LENGTH);
            p_axi_bus_control->num++;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if(NULL != p_device_control->dev_info->axi_bus_name). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_init_axibus_namelist()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_io_deinit_device_info()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_deinit_device_info(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t           osal_ret         = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                       os_ret;
    st_osal_device_control_t  *device_control  = NULL;
    size_t                    dev_total_num;
    size_t                    dev_count        = 0;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    dev_total_num = R_OSAL_DCFG_GetNumOfDevice();
    while ((dev_count < dev_total_num) && (OSAL_RETURN_OK == osal_ret))
    {
        osal_ret = R_OSAL_DCFG_GetDeviceInfo((uint32_t)dev_count, &device_control);
        if ((OSAL_RETURN_OK != osal_ret) ||
            (NULL == device_control) ||
            (NULL == device_control->dev_info))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            os_ret = pthread_mutex_destroy(&device_control->inner_mtx);
            if (0 == os_ret)
            {
                device_control->handle_id  = OSAL_INVALID_DEVICE_HANDLE_ID;
                device_control->active     = false;
                device_control->axi_bus_id = OSAL_AXI_BUS_ID_INVALID;
                /* PRQA S 0326 1 # No problem for casting from void * to object pointer.*/
                device_control->base_addr  = (uintptr_t)NULL;
                device_control->len        = 0U;
            }
            else
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
        }
        dev_count++;
    }   /* end of while((dev_count < dev_total_num) && (OSAL_RETURN_OK == osal_ret)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_io_deinit_device_info()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_io_deinit_pdev_info()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_deinit_pdev_info(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t          count;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    for (count = 0; count < g_pdev_entry.num; count++)
    {
        /* PRQA S 0326 1 # No problem for casting from void * to object pointer. */
        os_ret = munmap((void *)g_pdev_entry.entry[count].maped_start_addr,
                        (g_pdev_entry.entry[count].end_addr - g_pdev_entry.entry[count].start_addr));
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_MEM;
        }
        else
        {
            /* PRQA S 0326 1 # No problem for casting from void * to object pointer.*/
            g_pdev_entry.entry[count].maped_start_addr = (uintptr_t)NULL;
            /* PRQA S 0326 1 # No problem for casting from void * to object pointer.*/
            g_pdev_entry.entry[count].start_addr       = (uintptr_t)NULL;
            /* PRQA S 0326 1 # No problem for casting from void * to object pointer.*/
            g_pdev_entry.entry[count].end_addr         = (uintptr_t)NULL;

            if (-1 != g_pdev_entry.entry[count].fd)
            {
                os_ret = close(g_pdev_entry.entry[count].fd);
                if (0 != os_ret)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
                else
                {
                    g_pdev_entry.entry[count].fd = -1;
                }
            }
        }   /* end of if(0 != os_ret). */
    }   /* end of for(count = 0; count < g_pdev_entry.num; count++). */

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (NULL != g_pdev_entry.entry)
        {
            /* PRQA S 5118,0314 1 # Safty Manual, free use are allowed. No problem because it is called by DeInitilize. */
            free((void*)g_pdev_entry.entry);
        }
        g_pdev_entry.num   = 0U;
        g_pdev_entry.entry = NULL;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_io_deinit_pdev_info()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_compare_string()
* XOS3_OSAL_EMCOS_CD_CD_IO_035
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_001]
***********************************************************************************************************************/
/* PRQA S 9106 1 # This internal function is intended to be used in common. */
OSAL_STATIC e_osal_return_t osal_internal_compare_string(const char *p_str1, const char *p_str2, size_t len)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret   = OSAL_RETURN_OK;
    size_t          loop_count = 0;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    while ((loop_count < len) && ('\0' != p_str1[loop_count]) && ('\0' != p_str2[loop_count]) && (OSAL_RETURN_OK == osal_ret))
    {
        if (p_str1[loop_count] != p_str2[loop_count])
        {
            osal_ret = OSAL_RETURN_PAR;
        }
        else
        {
            /* Do nothing */
        }
        loop_count++;
    }

    if (loop_count != len)
    {
        if (('\0' != p_str1[loop_count]) || ('\0' != p_str2[loop_count]))
        {
            osal_ret = OSAL_RETURN_PAR;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        osal_ret = OSAL_RETURN_PAR;
    }

    return osal_ret;

}
/***********************************************************************************************************************
* End of function osal_internal_compare_string()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_get_domain_name()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_get_domain_name(const char* p_axi_bus_name, char *p_domain_name)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t           osal_ret;
    st_osal_axi_bus_control_t *axi_bus_control = NULL;
    size_t                    loop_count;
    size_t                    axi_bus_len;
    bool                      find_flg         = false;
    char                      local_axi_bus_name[OSAL_AXI_BUS_NAME_MAX_LENGTH + 1U] = "";
    int                       os_ret; /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_DCFG_GetAxiBusInfo(&axi_bus_control);
    if ((OSAL_RETURN_OK == osal_ret) && (NULL != axi_bus_control) && (NULL != axi_bus_control->axi_bus_list))
    {
        for (loop_count = 0; loop_count < axi_bus_control->num; loop_count++)
        {
            /* check for [bus domain and uTLB are connected by "_"] */
            axi_bus_len = strnlen(axi_bus_control->axi_bus_list[loop_count].axi_bus_name, (size_t)OSAL_AXI_BUS_NAME_MAX_LENGTH);
            (void)strncpy(local_axi_bus_name, axi_bus_control->axi_bus_list[loop_count].axi_bus_name, axi_bus_len);

            local_axi_bus_name[axi_bus_len] = '_';
            os_ret = strncmp(p_axi_bus_name, local_axi_bus_name, (axi_bus_len + (size_t)1));
            if (0 == os_ret)
            {
                find_flg = true;
                break;
            }
            else
            {
                /* Do nothing */
            }
            /* PRQA S 0314 1 # The parameter must be void* and there is no side side-effect in this case. */
            (void)memset((void*)local_axi_bus_name, 0, (size_t)(OSAL_AXI_BUS_NAME_MAX_LENGTH + 1U));
        }
    }
    else
    {
        osal_ret = OSAL_RETURN_FAIL;
    }   /* end of if ((OSAL_RETURN_OK == osal_ret) && (NULL != axi_bus_control) && (NULL != axi_bus_control->axi_bus_list)) */

    if ((OSAL_RETURN_OK == osal_ret) && (false == find_flg))
    {
        osal_ret = OSAL_RETURN_UNKNOWN_NAME;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        (void)strncpy(p_domain_name, axi_bus_control->axi_bus_list[loop_count].axi_bus_name, (size_t)OSAL_AXI_BUS_NAME_MAX_LENGTH);
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_get_domain_name()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_add_utlb_list_single()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_add_utlb_list_single(int32_t utlb,
                                                               const char* p_domain_name,
                                                               osal_axi_bus_id_t* pp_list_buffer,
                                                               size_t list_size,
                                                               size_t *match_count)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret    = OSAL_RETURN_OK;
    size_t          domain_len;
    size_t          count;
    size_t          local_count = *match_count;
    int             os_ret;  /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    domain_len = strnlen(p_domain_name, OSAL_BUS_DOMAIN_SIZE);
    count      = 0;

    while (OSAL_AXI_BUS_NAME_LAST != gsp_axi_bus_name_info[count].utlb)
    {
        os_ret = strncmp(gsp_axi_bus_name_info[count].domain_name, p_domain_name, domain_len);
        if (0 == os_ret)
        {
            if (gsp_axi_bus_name_info[count].utlb == utlb)
            {
                if ((NULL != pp_list_buffer) && (list_size > local_count))
                {
                    pp_list_buffer[local_count] = count; /* count is axi_id */
                }
                else
                {
                    /* Do nothing */
                }

                local_count++;
                break;
            }
            else
            {
                /* Do nothing */
            }
        }
        count++;
    }   /* end of while (OSAL_AXI_BUS_NAME_LAST != gsp_axi_bus_name_info[count].utlb) */

    if (OSAL_AXI_BUS_NAME_LAST == gsp_axi_bus_name_info[count].utlb)
    {
        osal_ret = OSAL_RETURN_UNKNOWN_NAME; /* domain name not found or utlb number is not assigned to the domain */
    }
    else
    {
        /* Do nothing */
    }

    *match_count = local_count;

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_add_utlb_list_single()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_add_utlb_list_multi()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_add_utlb_list_multi(int32_t utlb,
                                                              const char* p_domain_name,
                                                              osal_axi_bus_id_t* pp_list_buffer,
                                                              size_t list_size,
                                                              size_t *match_count,
                                                              const int32_t *current_utlb)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret          = OSAL_RETURN_OK;
    int32_t         current_val       = *current_utlb;
    size_t          local_match_count = *match_count;
    int32_t         loop_count  = 0;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    while ((loop_count < (utlb - current_val)) && (OSAL_RETURN_OK == osal_ret))
    {
        osal_ret = osal_internal_add_utlb_list_single((current_val+loop_count+1),
                                                      p_domain_name,
                                                      pp_list_buffer,
                                                      list_size,
                                                      &local_match_count);
        loop_count++;
    }
    *match_count = local_match_count;

    return osal_ret;
/* PRQA S 9108 2 # The number of parameters of this function is necessary and sufficient. verified OK. */
}
/***********************************************************************************************************************
* End of function osal_internal_add_utlb_list_multi()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_add_utlb_list()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_add_utlb_list(const char* p_domain_name,
                                                        osal_axi_bus_id_t* pp_list_buffer,
                                                        size_t list_size,
                                                        size_t *match_count,
                                                        int32_t *current_utlb,
                                                        const char *p_num_str)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    int32_t         num;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
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
                osal_ret = osal_internal_add_utlb_list_single(num, p_domain_name, pp_list_buffer, list_size, match_count);
                *current_utlb = num;
                break;

            case '-':
                if ((size_t)0 == *match_count)
                {
                    osal_ret = OSAL_RETURN_UNKNOWN_NAME;  /* axi_bus_name is invalid (e.g. ir0-04)*/
                }
                else if (num <= *current_utlb)
                {
                    osal_ret = OSAL_RETURN_UNKNOWN_NAME;  /* axi_bus_name is invalid (e.g. ir0_04-02)*/
                }
                else
                {
                    osal_ret = osal_internal_add_utlb_list_multi(num,
                                                                 p_domain_name,
                                                                 pp_list_buffer,
                                                                 list_size,
                                                                 match_count,
                                                                 current_utlb);
                }
                break;

            default:
                osal_ret = OSAL_RETURN_UNKNOWN_NAME;  /* axi_bus_name is invalid (e.g. ir0:04:05)*/
                break;
        }   /* end of switch (p_num_str[0]) */
    }   /* end of if (('0' > p_num_str[1]) || ('9' < p_num_str[1]) || ('0' > p_num_str[2]) || ('9' < p_num_str[2])) */

    return osal_ret;
/* PRQA S 9108 2 # The number of parameters of this function is necessary and sufficient. verified OK. */
}
/***********************************************************************************************************************
* End of function osal_internal_add_utlb_list()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_check_axiidlist_for_domainname()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_check_axiidlist_for_domainname(const char* p_axi_bus_name,
                                                                         const char* p_domain_name,
                                                                         osal_axi_bus_id_t* pp_list_buffer,
                                                                         size_t list_size,
                                                                         size_t * const p_num_of_axi_ids)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret     = OSAL_RETURN_OK;
    size_t          match_count  = 0;
    size_t          domain_len;
    size_t          axi_bus_name_len;
    char            tmp_str[4]   = "";
    size_t          offset;
    int32_t         current_utlb = 0;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    domain_len       = strnlen(p_domain_name,  OSAL_BUS_DOMAIN_SIZE);
    axi_bus_name_len = strnlen(p_axi_bus_name, (size_t)OSAL_DOMAIN_UTLB_MAX_STRINGS);
    offset           = domain_len;

    while ((OSAL_RETURN_OK == osal_ret) && (axi_bus_name_len >= (offset + (size_t)3U)))
    {
        /* Extracts uTLB information including delimiters. '_xx' or '-xx'  */
        (void)strncpy(tmp_str, &p_axi_bus_name[offset], (size_t)3U);
        tmp_str[3] = '\0';

        osal_ret = osal_internal_add_utlb_list(p_domain_name, pp_list_buffer, list_size, &match_count, &current_utlb, tmp_str);
        if (OSAL_RETURN_OK == osal_ret)
        {
            /* uTLB one block size */
            offset += (size_t)3U;
        }
        else
        {
            /* Do nothing */
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        *p_num_of_axi_ids = match_count;

        if (0U == match_count)
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

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_check_axiidlist_for_domainname()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_create_axiidlist_for_devicename()
***********************************************************************************************************************/
OSAL_STATIC void osal_internal_create_axiidlist_for_devicename(const char* p_device_name,
                                                               osal_axi_bus_id_t *pp_list_buffer,
                                                               size_t list_size,
                                                               size_t * const p_num_of_axi_ids,
                                                               size_t * const p_num_of_invalid_ids)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    char const *  os_ret;
    size_t        match_count        = 0;
    size_t        invalid_utlb_count = 0;
    size_t        count              = 0;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    while (OSAL_AXI_BUS_NAME_LAST != gsp_axi_bus_name_info[count].utlb)
    {
        os_ret = strstr(gsp_axi_bus_name_info[count].dev_names, p_device_name);
        if (NULL != os_ret)
        {
            if (OSAL_INVALID_UTLB != gsp_axi_bus_name_info[count].utlb)
            {
                if ((NULL != pp_list_buffer) && (list_size > match_count))
                {
                    pp_list_buffer[match_count] = count;  /* count is axi_id */
                }
                else
                {
                    /* Do nothing */
                }
                match_count++;
            }
            else
            {
                invalid_utlb_count++;
            }
        }
        else
        {
            /* Do nothing */
        }   /* end of if (NULL != os_ret) */
        count++;
    }   /* end of while (OSAL_AXI_BUS_NAME_LAST != gsp_axi_bus_name_info[count].utlb) */

    *p_num_of_axi_ids     = match_count;
    *p_num_of_invalid_ids = invalid_utlb_count;

    return;
}
/***********************************************************************************************************************
* End of function osal_internal_create_axiidlist_for_devicename()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_create_axibus_info()
* XOS3_OSAL_EMCOS_CD_CD_IO_036
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_002]
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_create_axibus_info(const st_osal_device_control_t *p_device_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint32_t        table_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL != p_device_control->dev_info->utlb_assignment)
    {
        for (table_count = 0; (table_count < p_device_control->dev_info->utlb_assignment->table_size) && (OSAL_RETURN_OK == osal_ret); table_count++)
        {
            osal_ret = osal_internal_register_axi_bus_info(p_device_control->dev_info->axi_bus_name,
                                                           p_device_control->dev_info->id,
                                                           p_device_control->dev_info->utlb_assignment->table[table_count]);
        }
    }
    else
    {
        osal_ret = osal_internal_register_axi_bus_info(p_device_control->dev_info->axi_bus_name,
                                                       p_device_control->dev_info->id,
                                                       OSAL_INVALID_UTLB);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_create_axibus_info()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_register_axi_bus_info()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_register_axi_bus_info(const char * const p_bus_domain, const char* const p_device_name, int32_t utlb)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret = OSAL_RETURN_OK;
    uint32_t                    match_flag = 0;
    size_t                      device_name_len;
    st_osal_axi_bus_name_info_t *p_tmp_table;
    char const                  *strstr_ret;
    int                         os_ret; /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */
    size_t                      array_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    device_name_len = strnlen(p_device_name, (size_t)OSAL_DEVICE_NAME_MAX_LENGTH) + 1U;

    for(array_count = 0; array_count < gs_axi_bus_name_info_array_size; array_count++)
    {
        os_ret = strncmp(gsp_axi_bus_name_info[array_count].domain_name, p_bus_domain, (size_t)OSAL_AXI_BUS_NAME_MAX_LENGTH);
        if ((0 == os_ret) && (gsp_axi_bus_name_info[array_count].utlb == utlb))
        {
            match_flag = 1U;
            strstr_ret = strstr(gsp_axi_bus_name_info[array_count].dev_names, p_device_name);
            if (NULL == strstr_ret)
            {
                osal_ret = osal_internal_device_name_connect(p_device_name, array_count, device_name_len);
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
    }

    if (0U == match_flag)
    {
        /* Use p_tmp_buffer to avoid memory leak in case realloc fail */
        /* PRQA S 0314 2 # The 1st parameter must be (void *) and there is no side-effect in this case. */
        /* PRQA S 5118,2707 1 # Freed in deinitialize. */
        p_tmp_table = (st_osal_axi_bus_name_info_t *)realloc((void*)gsp_axi_bus_name_info,
                        (gs_axi_bus_name_info_array_size + 1U) * sizeof(st_osal_axi_bus_name_info_t));
        if (NULL != p_tmp_table)
        {
            gsp_axi_bus_name_info = p_tmp_table;

            gsp_axi_bus_name_info[gs_axi_bus_name_info_array_size].utlb = utlb;
            (void)strncpy(gsp_axi_bus_name_info[gs_axi_bus_name_info_array_size].domain_name, p_bus_domain, (size_t)OSAL_AXI_BUS_NAME_MAX_LENGTH);

            /* PRQA S 5118,2706 1 # Freed in deinitialize. */
            gsp_axi_bus_name_info[gs_axi_bus_name_info_array_size].dev_names = (char *)malloc(device_name_len + 1U);
            if (NULL == gsp_axi_bus_name_info[gs_axi_bus_name_info_array_size].dev_names)
            {
                osal_ret = OSAL_RETURN_MEM;
            }
            else
            {
                (void)strncpy(gsp_axi_bus_name_info[gs_axi_bus_name_info_array_size].dev_names, p_device_name, device_name_len);
                gsp_axi_bus_name_info[gs_axi_bus_name_info_array_size].dev_names_length = device_name_len + 1U;
            }

            gs_axi_bus_name_info_array_size += 1U;
        }
        else
        {
            osal_ret = OSAL_RETURN_MEM;
        }   /* end of if (NULL != p_tmp_table) */
    }
    else
    {
        /* Do nothing */
    }   /* end of if (0U == match_flag) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_register_axi_bus_info()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_device_name_connect()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_device_name_connect(const char* const p_device_name, size_t axi_bus_info_num, size_t add_dev_name_len)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t          axi_bus_name_len;
    char            *p_tmp_dev_names;
    char            *dummy_tmp_dev_names;
    int             os_ret; /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    axi_bus_name_len = gsp_axi_bus_name_info[axi_bus_info_num].dev_names_length + add_dev_name_len + 1U;

    /* PRQA S 0314 2 # The 1st parameter must be (void *) and there is no side-effect in this case. */
    /* PRQA S 5118,2706 1 # Freed in caller function() */
    p_tmp_dev_names = (char *)realloc((void *)gsp_axi_bus_name_info[axi_bus_info_num].dev_names, axi_bus_name_len);
    if (NULL == p_tmp_dev_names)
    {
        osal_ret = OSAL_RETURN_MEM;
    }
    else
    {
        /* PRQA S 5118,2707 1 # Freed in this function */
        dummy_tmp_dev_names = malloc(axi_bus_name_len);
        if (NULL == dummy_tmp_dev_names)
        {
            osal_ret = OSAL_RETURN_MEM;
        }
        else
        {
            gsp_axi_bus_name_info[axi_bus_info_num].dev_names = p_tmp_dev_names;
            os_ret = snprintf(dummy_tmp_dev_names, axi_bus_name_len, "%s\n%s", gsp_axi_bus_name_info[axi_bus_info_num].dev_names, p_device_name);
            if (os_ret < 0)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                (void)strncpy(gsp_axi_bus_name_info[axi_bus_info_num].dev_names, dummy_tmp_dev_names, axi_bus_name_len);
                gsp_axi_bus_name_info[axi_bus_info_num].dev_names_length = axi_bus_name_len;
            }

            /* PRQA S 0314 1 # The parameter must be (void *) and there is no side-effect in this case. */
            free((void *)dummy_tmp_dev_names); /* PRQA S 5118 # Allocated in osal_internal_device_name_connect() */
        }
    }   /* end of if (NULL == p_tmp_dev_names) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_device_name_connect()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_register_axi_bus_info_last_data()
* XOS3_OSAL_EMCOS_CD_CD_IO_037
* [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_003]
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_register_axi_bus_info_last_data(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret     = OSAL_RETURN_OK;
    st_osal_axi_bus_name_info_t *p_tmp_table;
    uint32_t                    match_flag   = 0;
    size_t                      array_count;
    int                         cmp_result; /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */
    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    for(array_count = 0; array_count < gs_axi_bus_name_info_array_size; array_count++)
    {
        cmp_result = strncmp(gsp_axi_bus_name_info[array_count].domain_name, "", 1U);
        if ((0 == cmp_result) && (gsp_axi_bus_name_info[array_count].utlb == OSAL_AXI_BUS_NAME_LAST))
        {
            match_flag = 1U;
        }
        else
        {
            /* Do nothing */
        }
    }

    if (0U == match_flag)
    {
        /* Use p_tmp_buffer to avoid memory leak in case realloc fail */
        /* PRQA S 0314 2 # The 1st parameter must be (void *) and there is no side-effect in this case. */
        /* PRQA S 5118,2707 1 # Freed in deinitialize. */
        p_tmp_table = (st_osal_axi_bus_name_info_t *)realloc((void*)gsp_axi_bus_name_info,
                        (gs_axi_bus_name_info_array_size + 1U) * sizeof(st_osal_axi_bus_name_info_t));
        if (NULL == p_tmp_table)
        {
            osal_ret = OSAL_RETURN_MEM;
        }
        else
        {
            gsp_axi_bus_name_info = p_tmp_table;

            gsp_axi_bus_name_info[gs_axi_bus_name_info_array_size].utlb = OSAL_AXI_BUS_NAME_LAST;
            (void)strncpy(gsp_axi_bus_name_info[gs_axi_bus_name_info_array_size].domain_name, "", 1U);
            gsp_axi_bus_name_info[gs_axi_bus_name_info_array_size].dev_names = NULL;

            gs_axi_bus_name_info_array_size += 1U;
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if (0U == match_flag) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_register_axi_bus_info_last_data()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_get_utlb_assignment_for_devicename()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_get_utlb_assignment_for_devicename(const char* p_device_name, size_t* const p_num_of_axi_ids)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t           osal_ret        = OSAL_RETURN_OK;
    e_osal_return_t           api_ret;
    size_t                    dev_total_num;
    size_t                    loop_count      = 0;
    st_osal_device_control_t  *device_control = NULL;
    bool                      find_flg        = false;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    dev_total_num = R_OSAL_DCFG_GetNumOfDevice();

    while ((loop_count < dev_total_num) && (OSAL_RETURN_OK == osal_ret))
    {
        api_ret = R_OSAL_DCFG_GetDeviceInfo((uint32_t)loop_count, &device_control);
        if ((OSAL_RETURN_OK != api_ret) || (NULL == device_control) || (NULL == device_control->dev_info))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            api_ret = osal_internal_compare_string(p_device_name,
                                                   device_control->dev_info->id,
                                                   (size_t)OSAL_DEVICE_NAME_MAX_LENGTH);
            if (OSAL_RETURN_OK == api_ret)
            {
                find_flg = true;
                if (NULL == device_control->dev_info->utlb_assignment)
                {
                    *p_num_of_axi_ids = 0;
                }
                else
                {
                    *p_num_of_axi_ids = device_control->dev_info->utlb_assignment->table_size;
                }
                break;
            }
            else
            {
                /* Do nothing */
            }
        }
        loop_count++;
    }   /* end of while((loop_count < dev_total_num) && (OSAL_RETURN_OK == osal_ret)). */

    if ((false == find_flg) && (OSAL_RETURN_OK == osal_ret))
    {
        osal_ret = OSAL_RETURN_UNKNOWN_NAME;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_get_utlb_assignment_for_devicename()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_io_deinit_pre_proc()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_deinit_pre_proc(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret        = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                      os_ret;
    size_t                   dev_total_num;
    size_t                   dev_count       = 0;
    st_osal_device_control_t *device_control = NULL;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    dev_total_num = R_OSAL_DCFG_GetNumOfDevice();
    while ((dev_count < dev_total_num) && (OSAL_RETURN_OK == osal_ret))
    {
        osal_ret = R_OSAL_DCFG_GetDeviceInfo((uint32_t)dev_count, &device_control);
        if ((OSAL_RETURN_OK != osal_ret) ||
            (NULL == device_control) ||
            (NULL == device_control->dev_info))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_Internal_TimedlockMutex(&device_control->inner_mtx, device_control->inner_timeout);
            if (OSAL_RETURN_OK == osal_ret)
            {
                if (true == device_control->active)
                {
                    osal_ret = OSAL_RETURN_BUSY;
                }
                else
                {
                    /* Do nothing */
                }

                os_ret = pthread_mutex_unlock(&device_control->inner_mtx);
                if (0 != os_ret)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
                else
                {
                    /* Do nothing */
                }
            }
            else
            {
                osal_ret = OSAL_RETURN_FAIL;
            }   /* end of if(OSAL_RETURN_OK == osal_ret). */
        }
        else
        {
            /* Do nothing */
        }   /* end of if(OSAL_RETURN_OK == osal_ret). */
        dev_count++;
    }   /* end of while((dev_count < dev_total_num) && (OSAL_RETURN_OK == osal_ret)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_io_deinit_pre_proc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_io_check_get_device_list()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_check_get_device_list(const char * p_device_type_name,
                                                                   const char * p_buffer,
                                                                   size_t buffer_size,
                                                                   const size_t *const p_num_of_byte)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret     = OSAL_RETURN_OK;
    size_t          dev_type_len;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if ((NULL == p_device_type_name) || (NULL == p_num_of_byte))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        dev_type_len = strnlen(p_device_type_name, (size_t)OSAL_DEVICE_TYPE_NAME_MAX_LENGTH) + (size_t)1;
        if (OSAL_DEVICE_TYPE_NAME_MAX_LENGTH < (dev_type_len))
        {
            osal_ret = OSAL_RETURN_PAR;
        }
        else
        {
            if (NULL == p_buffer)
            {
                if (0U != buffer_size)
                {
                    osal_ret = OSAL_RETURN_PAR;
                }
                else
                {
                    osal_ret = OSAL_RETURN_OK_RQST;
                }
            }
            else
            {
                if (0U == buffer_size)
                {
                    osal_ret = OSAL_RETURN_PAR;
                }
            }
        }   /* end of if (OSAL_DEVICE_TYPE_NAME_MAX_LENGTH < (dev_type_len)) */
    }   /* end of if((NULL == p_device_type_name) || (NULL == p_num_of_byte)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_io_check_get_device_list()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_io_get_device_list_proc()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_get_device_list_proc(const char * p_device_type_name,
                                                                  char * p_buffer,
                                                                  size_t buffer_size,
                                                                  size_t *const p_num_of_byte)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t           osal_ret        = OSAL_RETURN_OK;
    e_osal_return_t           api_ret;
    size_t                    dev_total_num;
    size_t                    loop_count      = 0;
    size_t                    total_length    = 0;
    st_osal_device_control_t  *device_control = NULL;
    const char                eos[2]          = "\0";

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL != p_buffer)
    {
        /* PRQA S 0314,2812 2 # Required cast on system, not modifiable.NULL checked and no problem.
        There is no problem because the pointer and size are checked in the osal_internal_io_check_get_device_list function. */
        (void)memset((void*)p_buffer, 0x00, buffer_size);
    }

    dev_total_num = R_OSAL_DCFG_GetNumOfDevice();

    while ((loop_count < dev_total_num) && (OSAL_RETURN_OK == osal_ret))
    {
        api_ret = R_OSAL_DCFG_GetDeviceInfo((uint32_t)loop_count, &device_control);
        if ((OSAL_RETURN_OK != api_ret) || (NULL == device_control) || (NULL == device_control->dev_info))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            if (NULL != p_buffer)
            {
                osal_ret = osal_internal_io_create_device_list(device_control,
                                                               p_device_type_name,
                                                               buffer_size,
                                                               p_buffer,
                                                               &total_length);
            }
            else /*  In case of OSAL_RETURN_OK_RQST */
            {
                osal_ret = osal_internal_io_create_device_list_okreq(device_control,
                                                               p_device_type_name,
                                                               &total_length);
            }
        }
        loop_count++;
    }

    if ((OSAL_RETURN_OK == osal_ret) && (NULL != p_buffer))
    {
        p_buffer[total_length - (size_t)1] = eos[0];  /* p_buffer is not NULL, because it is not OSAL_RETURN_OK_RQST */
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (0U == total_length)
        {
            osal_ret = OSAL_RETURN_UNKNOWN_NAME;
        }
        else
        {
            *p_num_of_byte = total_length;
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_io_get_device_list_proc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_io_create_device_list()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_create_device_list(const st_osal_device_control_t *device_control,
                                                                const char *p_device_type_name,
                                                                size_t buffer_size,
                                                                char *p_buffer,
                                                                size_t *p_total_length)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret           = OSAL_RETURN_OK;
    e_osal_return_t api_ret;
    size_t          name_length;
    size_t          local_total_length = *p_total_length;
    const char      ch[2]              = "\n";

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    api_ret = osal_internal_compare_string(p_device_type_name,
                                           device_control->dev_info->type,
                                           (size_t)OSAL_DEVICE_TYPE_NAME_MAX_LENGTH);
    if (OSAL_RETURN_OK == api_ret)
    {
        name_length = strnlen(device_control->dev_info->id, (size_t)OSAL_DEVICE_NAME_MAX_LENGTH + 1U);
        if (OSAL_DEVICE_NAME_MAX_LENGTH < name_length)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            local_total_length += name_length;
            if (buffer_size >= (local_total_length + (size_t)1))
            {
                (void)strncat(p_buffer, device_control->dev_info->id, name_length);
                p_buffer[local_total_length] = ch[0];
            }
            else
            {
                osal_ret = OSAL_RETURN_PAR;
                /* PRQA S 0314 1 # The parameter must be (void *) and there is no side side-effect in this case. */
                (void)memset((void*)p_buffer, 0x00, buffer_size);
            }
            local_total_length++;

            *p_total_length = local_total_length;
        }   /* end of if(OSAL_DEVICE_NAME_MAX_LENGTH < name_length). */
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == api_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_io_create_device_list()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_io_create_device_list_okreq()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_create_device_list_okreq(const st_osal_device_control_t *device_control,
                                                                      const char *p_device_type_name,
                                                                      size_t *p_total_length)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret           = OSAL_RETURN_OK;
    e_osal_return_t api_ret;
    size_t          name_length;
    size_t          local_total_length = *p_total_length;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    api_ret = osal_internal_compare_string(p_device_type_name,
                                           device_control->dev_info->type,
                                           (size_t)OSAL_DEVICE_TYPE_NAME_MAX_LENGTH);
    if (OSAL_RETURN_OK == api_ret)
    {
        name_length = strnlen(device_control->dev_info->id, (size_t)OSAL_DEVICE_NAME_MAX_LENGTH + 1U);
        if (OSAL_DEVICE_NAME_MAX_LENGTH < name_length)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* For OSAL_RETURN_OK_RQST, only the length is calculated. */
            local_total_length += name_length;
            local_total_length++;

            *p_total_length = local_total_length;
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_io_create_device_list_okreq()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_io_check_device_id()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_check_device_id(const char * p_id, st_osal_device_control_t **pp_device_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret          = OSAL_RETURN_OK;
    e_osal_return_t          api_ret;
    size_t                   dev_total_num;
    size_t                   dev_count         = 0U;
    bool                     find_flg          = false;
    st_osal_device_control_t *p_device_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    dev_total_num = R_OSAL_DCFG_GetNumOfDevice();
    while ((dev_count < dev_total_num) && (false == find_flg) && (OSAL_RETURN_OK == osal_ret))
    {
        api_ret = R_OSAL_DCFG_GetDeviceInfo((uint32_t)dev_count, &p_device_control);
        if ((OSAL_RETURN_OK != api_ret) || (NULL == p_device_control) || (NULL == p_device_control->dev_info))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* PRQA S 2916 1 # The pointer refers to the Realities, so there is no problem. */
            api_ret = osal_internal_compare_string(p_device_control->dev_info->id,
                                                   p_id,
                                                   (size_t)OSAL_DEVICE_NAME_MAX_LENGTH);
            if (OSAL_RETURN_OK == api_ret)
            {
                /* PRQA S 0326 3 # No problem for casting from void * to object pointer.*/
                if (((uintptr_t)NULL == p_device_control->base_addr) && (0U != p_device_control->len))
                {
                    /* Devices that do not exist in the pdev entry */
                    osal_ret = OSAL_RETURN_ID;
                }
                else
                {
                    find_flg = true;
                    /* PRQA S 2916 1 # The pointer refers to the Realities, so there is no problem. */
                    *pp_device_control = p_device_control;
                }
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if((OSAL_RETURN_OK != api_ret) || (NULL == p_device_control) || (NULL == p_device_control->dev_info)). */
        dev_count++;
    }   /* end of while((dev_count < dev_total_num) && (false == find_flg) && (OSAL_RETURN_OK == osal_ret)) */

    if ((false == find_flg) && (OSAL_RETURN_OK == osal_ret))
    {
        osal_ret = OSAL_RETURN_ID;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_io_check_device_id()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_io_set_open_handle()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_set_open_handle(osal_device_handle_t *const p_handle,
                                                             st_osal_device_control_t *p_device_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_return_t api_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    api_ret = R_OSAL_Internal_TimedlockMutex(&p_device_control->inner_mtx, p_device_control->inner_timeout);
    if (OSAL_RETURN_OK == api_ret)
    {
        /* Check if the block is released after the de-initialization. */
        if (false == gs_osal_io_is_initialized)
        {
            osal_ret = OSAL_RETURN_STATE;
        }
        else
        {
            if (true == p_device_control->active)
            {
                osal_ret = OSAL_RETURN_BUSY;
            }
            else
            {
                /* Do nothing*/
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                p_device_control->handle_id = (uint64_t)OSAL_DEVICE_HANDLE_ID;
                p_device_control->active    = true;
                *p_handle = p_device_control;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if(false == gs_osal_io_is_initialized). */

        os_ret = pthread_mutex_unlock(&p_device_control->inner_mtx);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        osal_ret = OSAL_RETURN_FAIL;
    }   /* end of if(OSAL_RETURN_OK == api_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_io_set_open_handle()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_io_device_close_proc()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_device_close_proc(osal_device_handle_t handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                    osal_ret = OSAL_RETURN_OK;
    e_osal_return_t                    api_ret;
    st_osal_interrupt_thread_control_t *interrupt_control;
    size_t                             loop_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (0U != handle->irq_channels)
    {
        loop_count = 0U;
        while (loop_count < handle->irq_channels)
        {
            api_ret = R_OSAL_RCFG_GetInterruptThreadInfo((uint32_t)handle->interrupt_index[loop_count], &interrupt_control);
            if ((OSAL_RETURN_OK != api_ret) ||
                /* PRQA S 2991 2 # It is a check for robustness, because it is carried out based on the design is no problem.
                 True if the correct handle was passed. False detection of the tool. */
                (NULL == interrupt_control) ||
                /* PRQA S 2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
                 True if the correct handle was passed. False detection of the tool. */
                (OSAL_INTERRUPT_HANDLE_ID != interrupt_control->handle_id))
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
                 True if the correct handle was passed. False detection of the tool. */
                if (NULL != interrupt_control->irq_handler)
                {
                    osal_ret = OSAL_RETURN_BUSY;
                }
                else
                {
                    /* Do nothing */
                }
            } /* end of if((OSAL_RETURN_OK != api_ret)||(NULL == interrupt_control) ||(OSAL_INTERRUPT_HANDLE_ID != interrupt_control->handle_id)). */
            loop_count++;
        }   /* end of while(loop_count < handle->dev_info->irq_channels). */
    }
    else
    {
        /* Do nothing */
    }   /* end of if(0U != handle->dev_info->irq_channels). */

    if ((OSAL_RETURN_OK == osal_ret) && (true == handle->pm_wait_state))
    {
        osal_ret = OSAL_RETURN_BUSY;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        handle->handle_id = OSAL_INVALID_DEVICE_HANDLE_ID;
        handle->active    = false;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_io_device_close_proc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_io_check_rw_handle()
***********************************************************************************************************************/
/* PRQA S 3673 2 # False warning. This handle type is a pointer to an internal data structure. */
/* PRQA S 9106 1 # This internal function is intended to be used in common. */
OSAL_STATIC e_osal_return_t osal_internal_io_check_rw_handle(osal_device_handle_t const handle, size_t unit_size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_io_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (NULL == handle)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        else if ((OSAL_DEVICE_HANDLE_ID != handle->handle_id) || (false == handle->active))
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        /* PRQA S 0326 3 # No problem for casting from void * to object pointer.*/
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        else if (((uintptr_t)NULL == handle->base_addr) || (NULL == handle->dev_info) || (0U != (handle->base_addr & (unit_size - 1U))))
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_io_check_rw_handle()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_io_check_read_param()
***********************************************************************************************************************/
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t osal_internal_io_check_read_param(osal_device_handle_t const handle,
                                                              uintptr_t offset, size_t unit_size, const void *const p_data)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == p_data)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if (0U != (offset & (unit_size - 1U)))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if (handle->len < (offset + unit_size))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_io_check_read_param()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_io_check_write_param()
***********************************************************************************************************************/
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t osal_internal_io_check_write_param(osal_device_handle_t const handle, uintptr_t offset, size_t unit_size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (0U != (offset & (unit_size - 1U)))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if (handle->len < (offset + unit_size))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_io_check_write_param()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_io_check_block_rw_param()
***********************************************************************************************************************/
/* PRQA S 3673 2 # False warning. This handle type is a pointer to an internal data structure. */
/* PRQA S 9106 1 # This internal function is intended to be used in common. */
OSAL_STATIC e_osal_return_t osal_internal_io_check_block_rw_param(osal_device_handle_t const handle,
                                                                  uintptr_t offset,
                                                                  size_t size,
                                                                  size_t unit_size,
                                                                  const void *const p_data)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == p_data)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if ((0U != (offset & (unit_size - 1U))) || (0U != (size & (unit_size - 1U))))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if ((handle->len <= offset) || (handle->len < size))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if ((handle->len - offset) < size)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_io_check_block_rw_param()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_io_check_get_axi_bus_id()
***********************************************************************************************************************/
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t osal_internal_io_check_get_axi_bus_id(osal_device_handle_t const device, const osal_axi_bus_id_t *const p_axi_id)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_io_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (NULL == device)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        else if (OSAL_DEVICE_HANDLE_ID != device->handle_id)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        else if (device->active == false)
        {
            osal_ret = OSAL_RETURN_HANDLE;
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
    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        if (NULL == p_axi_id)
        {
            osal_ret = OSAL_RETURN_PAR;
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

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_io_check_get_axi_bus_id()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_io_find_device_name()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_find_device_name(const char* p_device_name,
                                                              st_osal_device_control_t **pp_device_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret       = OSAL_RETURN_OK;
    e_osal_return_t          api_ret;
    size_t                   dev_total_num;
    size_t                   dev_count      = 0U;
    bool                     find_flg       = false;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    dev_total_num = R_OSAL_DCFG_GetNumOfDevice();
    while ((dev_count < dev_total_num) && (false == find_flg) && (OSAL_RETURN_OK == osal_ret))
    {
        api_ret = R_OSAL_DCFG_GetDeviceInfo((uint32_t)dev_count, pp_device_control);
        if ((OSAL_RETURN_OK != api_ret) || (NULL == *pp_device_control) || (NULL == (*pp_device_control)->dev_info))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            api_ret = osal_internal_compare_string((*pp_device_control)->dev_info->id,
                                                   p_device_name,
                                                   (size_t)OSAL_DEVICE_NAME_MAX_LENGTH);
            if (OSAL_RETURN_OK == api_ret)
            {
                find_flg = true;
            }
            else
            {
                /* Do nothing */
            }
        }
        dev_count++;
    }   /* end of while ((dev_count < dev_total_num) && (false == find_flg) && (OSAL_RETURN_OK == osal_ret)) */

    if ((false == find_flg) && (OSAL_RETURN_OK == osal_ret))
    {
        osal_ret = OSAL_RETURN_UNKNOWN_NAME;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_io_find_device_name()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_io_check_axi_bus_name()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_check_axi_bus_name(const char * p_buffer, size_t buffer_size, const size_t *const p_num_of_byte)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_io_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == p_num_of_byte)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if (NULL == p_buffer)
    {
        if (0U != buffer_size)
        {
            osal_ret = OSAL_RETURN_PAR;
        }
        else
        {
            osal_ret = OSAL_RETURN_OK_RQST;
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if (false == gs_osal_io_is_initialized) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_io_check_axi_bus_name()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_io_create_axi_bus_list()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_create_axi_bus_list(size_t total_length, size_t buffer_size, char* p_buffer,
                                                                 const st_osal_axi_bus_control_t *axi_bus_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret     = OSAL_RETURN_OK;
    size_t          loop_count;
    size_t          local_length = 0;
    const char      ch[2]        = "\n";
    const char      eos[2]       = "\0";

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (total_length <= buffer_size)
    {
        /* The p_buffer is not NULL. Because it has already checked with caller */
        /* PRQA S 0314 1 # The parameter must be (void *) and there is no side side-effect in this case. */
        (void)memset((void*)p_buffer, 0x00, buffer_size);

        for (loop_count = 0; loop_count < axi_bus_control->num; loop_count++)
        {
            /* PRQA S 2812 1 # There is no problem because the pointer and size are checked in the R_OSAL_IoGetAxiBusNameList function. */
            (void)strncat(p_buffer,
                          axi_bus_control->axi_bus_list[loop_count].axi_bus_name,
                          (size_t)OSAL_AXI_BUS_NAME_MAX_LENGTH);

            local_length += strnlen(axi_bus_control->axi_bus_list[loop_count].axi_bus_name,
                                    (size_t)OSAL_AXI_BUS_NAME_MAX_LENGTH);
            if ((axi_bus_control->num - 1U) != loop_count)
            {
                /* PRQA S 2822 1 # There is no problem because the pointer and size are checked in the R_OSAL_IoGetAxiBusNameList function. */
                p_buffer[local_length] = ch[0];
                local_length++;
            }
            else
            {
                p_buffer[local_length] = eos[0];
            }
        }
    }
    else
    {
        osal_ret = OSAL_RETURN_PAR;
    }   /* end of if(total_length <= buffer_size). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_io_create_axi_bus_list()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_io_get_axi_bus_name_len()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_get_axi_bus_name_len(osal_axi_bus_id_t axi_id,
                                                                  st_osal_axi_bus_control_t **pp_axi_bus_control,
                                                                  size_t *p_name_length)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_return_t api_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    api_ret = R_OSAL_DCFG_GetAxiBusInfo(pp_axi_bus_control);
    if ((OSAL_RETURN_OK == api_ret) && (NULL != *pp_axi_bus_control))
    {
        if ((*pp_axi_bus_control)->num <= axi_id)
        {
            osal_ret = OSAL_RETURN_PAR;
        }
        else
        {
            *p_name_length = strnlen((*pp_axi_bus_control)->axi_bus_list[axi_id].axi_bus_name,
                                     (size_t)OSAL_AXI_BUS_NAME_MAX_LENGTH);
        }
    }
    else
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_io_get_axi_bus_name_len()
***********************************************************************************************************************/

