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
* File Name :    r_osal_os_io.c
* Version :      1.1.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL OS IFs for IO
***********************************************************************************************************************/

/***********************************************************************************************************************
 * includes
***********************************************************************************************************************/
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <glob.h>
#include <sys/types.h>
#include <sys/file.h>
#include <libgen.h>
#include <arpa/inet.h>
#include <regex.h>
#include <unistd.h>
#include <sys/mman.h>
#include <assert.h>
#include <sys/ioctl.h>
#include <linux/rcar-ipmmu-domains.h>
#include <linux/renesas_uioctl.h>
#include "target/linux/r_osal_os_types.h"
#include <stdio.h> /* must be include under the r_osal_api.h */

/***********************************************************************************************************************
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Definition
***********************************************************************************************************************/
/***********************************************************************************************************************
 * @def     OSAL_OPEN_DEVICE_PATTERN
 * @brief   The number of the open device pattern.
***********************************************************************************************************************/
#define OSAL_OPEN_DEVICE_PATTERN        (4U)

/***********************************************************************************************************************
 * @def     OSAL_EXCLUDED_DEVICE_PATTERN
 * @brief   The number of the not open device pattern.
***********************************************************************************************************************/
#define OSAL_EXCLUDED_DEVICE_PATTERN    (3U)

/***********************************************************************************************************************
 * @def     OSAL_PD_ALWAYS_ON_TYPE1
 * @brief   R8AXXXX_PD_ALWAYS_ON bit from bsp_yocto/linux-bsp/-/blob/rcar-4.1.0.rc17/include/dt-bindings/power/
***********************************************************************************************************************/
#define OSAL_PD_ALWAYS_ON_TYPE1 ((uint64_t)0x2000000000000000)

/***********************************************************************************************************************
 * @def     OSAL_PD_ALWAYS_ON_TYPE2
 * @brief   R8AXXXX_PD_ALWAYS_ON bit from bsp_yocto/linux-bsp/-/blob/rcar-4.1.0.rc17/include/dt-bindings/power/
***********************************************************************************************************************/
#define OSAL_PD_ALWAYS_ON_TYPE2 ((uint64_t)0x4000000000000000)

#ifndef RCAR_IPMMU_VC
/* TODO: Added temporarily because this definition does not exist in Poky SDK 3.1.11 (Yocto 6.1.0) */
/* Used in V4H. If this definition is added, delete it. */
#define RCAR_IPMMU_VC 22
#endif

/***********************************************************************************************************************
 * Global variable
***********************************************************************************************************************/
/***********************************************************************************************************************
 * @var     gs_io_pd_always_on
 * @brief   Power shared bit.
***********************************************************************************************************************/
OSAL_STATIC uint64_t gs_io_pd_always_on;

/***********************************************************************************************************************
 * @var     gsp_device_list
 * @brief   Device list.
***********************************************************************************************************************/
OSAL_STATIC st_osal_device_list_t* gsp_device_list = NULL;

/***********************************************************************************************************************
 * @var     gsp_io_axi_bus_name_list
 * @brief   AXI bus name list.
***********************************************************************************************************************/
OSAL_STATIC char* gsp_io_axi_bus_name_list;

/***********************************************************************************************************************
 * @var     gs_io_axi_bus_name_list_len
 * @brief   Length of AXI bus name list.
***********************************************************************************************************************/
OSAL_STATIC size_t gs_io_axi_bus_name_list_len = 0;

/***********************************************************************************************************************
 * @var     gs_axi_bus_name_info
 * @brief   AXI bus name information.
***********************************************************************************************************************/
OSAL_STATIC st_osal_axi_bus_name_info_t* gs_axi_bus_name_info;

/***********************************************************************************************************************
 * @var     gs_axi_bus_name_info_array_size
 * @brief   Size of AXI bus name information.
***********************************************************************************************************************/
OSAL_STATIC size_t gs_axi_bus_name_info_array_size = 0;

/***********************************************************************************************************************
 * @var     gs_osal_device_axi_ids
 * @brief   AXI bus information.
***********************************************************************************************************************/
OSAL_STATIC st_osal_axi_bus_info_t gs_osal_device_axi_ids[] =
{
   /* ipmmu_id              axi_bus_name used        axi_bus_id              */
    { RCAR_IPMMU_INVALID,   "invalid",   true,       OSAL_AXI_BUS_ID_INVALID },
    { RCAR_IPMMU_MM,        "mm(ipa)",   true,       OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY },
    { RCAR_IPMMU_VI0,       "vi0",       false,      OSAL_AXI_BUS_ID_INVALID },
    { RCAR_IPMMU_VI1,       "vi1",       false,      OSAL_AXI_BUS_ID_INVALID },
    { RCAR_IPMMU_VP0,       "vp0",       false,      OSAL_AXI_BUS_ID_INVALID },
    { RCAR_IPMMU_VP1,       "vp1",       false,      OSAL_AXI_BUS_ID_INVALID },
    { RCAR_IPMMU_VC0,       "vc0",       false,      OSAL_AXI_BUS_ID_INVALID },
    { RCAR_IPMMU_VC1,       "vc1",       false,      OSAL_AXI_BUS_ID_INVALID },
    { RCAR_IPMMU_PV0,       "pv0",       false,      OSAL_AXI_BUS_ID_INVALID },
    { RCAR_IPMMU_PV1,       "pv1",       false,      OSAL_AXI_BUS_ID_INVALID },
    { RCAR_IPMMU_PV2,       "pv2",       false,      OSAL_AXI_BUS_ID_INVALID },
    { RCAR_IPMMU_PV3,       "pv3",       false,      OSAL_AXI_BUS_ID_INVALID },
    { RCAR_IPMMU_IR,        "ir",        false,      OSAL_AXI_BUS_ID_INVALID },
    { RCAR_IPMMU_HC,        "hc",        false,      OSAL_AXI_BUS_ID_INVALID },
    { RCAR_IPMMU_RT0,       "rt0",       false,      OSAL_AXI_BUS_ID_INVALID },
    { RCAR_IPMMU_RT1,       "rt1",       false,      OSAL_AXI_BUS_ID_INVALID },
    { RCAR_IPMMU_MP,        "mp",        false,      OSAL_AXI_BUS_ID_INVALID },
    { RCAR_IPMMU_DS0,       "ds0",       false,      OSAL_AXI_BUS_ID_INVALID },
    { RCAR_IPMMU_DS1,       "ds1",       false,      OSAL_AXI_BUS_ID_INVALID },
    { RCAR_IPMMU_VIP0,      "vip0",      false,      OSAL_AXI_BUS_ID_INVALID },
    { RCAR_IPMMU_VIP1,      "vip1",      false,      OSAL_AXI_BUS_ID_INVALID },
    { RCAR_IPMMU_3DG,       "3dg",       false,      OSAL_AXI_BUS_ID_INVALID },
    { RCAR_IPMMU_VC,        "vc",        false,      OSAL_AXI_BUS_ID_INVALID }
};

/***********************************************************************************************************************
 * Prototypes
***********************************************************************************************************************/
/***********************************************************************************************************************
 * @brief            Read the all UIO directory.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_read_uio_dir(void);

/***********************************************************************************************************************
 * @brief            Get the bit that is indicated power domain is shared.
 * @param[in]        p_dir    A pointer to the DIR object.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_set_always_on(DIR* p_dir);

/***********************************************************************************************************************
 * @brief            Get the power domain from UIO.
 * @param[in]        p_uio_symbolic_name    A pointer to the file name.
 * @param[in]        p_pm_domain            A pointer to the type of power domain.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_power_domain(const char* p_uio_symbolic_name, uint64_t* p_pm_domain);

/***********************************************************************************************************************
 * @brief            Initialize the AXI bus name information.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_initialize_axi_bus_info(void);

/***********************************************************************************************************************
 * @brief            Initialize the bus information.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_initialize_bus_information(void);

/***********************************************************************************************************************
 * @brief            Get the IPMMU-ID from UIO.
 * @param[in]        p_uio_name    A pointer to the uio name.
 * @param[in,out]    p_ipmmu_id    A pointer to the AXI Bus ID type.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_ipmmu_id(const char* const p_uio_name, osal_axi_bus_id_t* p_ipmmu_id);

/***********************************************************************************************************************
 * @brief            Initialize the AXI bus name table.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_initialize_axi_bus_name_table(void);

/***********************************************************************************************************************
 * @brief            Create the AXI bus name information.
 * @param[in]        p_uio_name    A pointer to the uio name.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_create_axi_bus_name_table(const char* const p_uio_name);

/***********************************************************************************************************************
 * @brief            Get the device name from UIO.
 * @param[in]        p_uio_name       A pointer to the uio name.
 * @param[in]        p_device_name    A pointer to the device name.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_device_name_from_uio(const char* const p_uio_name, char* p_device_name);

/***********************************************************************************************************************
 * @brief            Create the AXI bus name information.
 * @param[in]        p_uio_name     A pointer to the uio name.
 * @param[in]        device_name    A pointer to the device name.
 * @param[in,out]    utlbs          A pointer to the utlb.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_info_of_axi_bus_name(const char* const p_uio_name, const char* const device_name, uint32_t* utlbs);

/***********************************************************************************************************************
 * @brief            Get the domain name from UIO.
 * @param[in]        p_uio_symbolic_name    A pointer to the file name.
 * @param[in]        p_bus_domain           A pointer to the bus domain.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_bus_domain(const char* const p_uio_symbolic_name, char* p_bus_domain);

/***********************************************************************************************************************
 * @brief            Get the number of uTLB from UIO.
 * @param[in,out]    p_utlbs       A pointer to the utlb.
 * @param[in]        p_uio_name    A pointer to the uio name.
 * @param[in,out]    p_utlb_num    Number of the utlbs.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_utlbs_from_uio(uint32_t* p_utlbs, const char* const p_uio_name, size_t* p_utlb_num);

/***********************************************************************************************************************
 * @brief            Register data to the AXI bus name information.
 * @param[in]        p_bus_domain     A pointer to the bus domain.
 * @param[in]        p_device_name    A pointer to the device name.
 * @param[in]        utlb             Number of the utlb.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_register_axi_bus_info(const char* const p_bus_domain, const char* const p_device_name, int32_t utlb);

/***********************************************************************************************************************
 * @brief            Connect the device name to the device name list of AXI bus name information.
 * @param[in]        p_device_name       A pointer to the device name.
 * @param[in]        connect_index       Index of connect device name.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_device_name_connect(const char* const p_device_name, size_t connect_index);

/***********************************************************************************************************************
 * @brief            Register last data to the AXI bus name information.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_register_axi_bus_info_last_data(void);

/***********************************************************************************************************************
 * @brief            Create the AXI bus name list.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_create_bus_name_list(void);

/***********************************************************************************************************************
 * @brief            Add the AXI bus name with uTLB to the p_buffer.
 * @param[in]        p_axi_bus_name    A pointer to the axi bus name.
 * @param[in,out]    p_buffer          The name of generated axi bus.
 * @param[in]        buffer_size       Length of the axi bus name.
 * @return           #void
***********************************************************************************************************************/
OSAL_STATIC void r_osal_os_io_create_axi_bus_name(const char* p_axi_bus_name, char* p_buffer, size_t buffer_size);

/***********************************************************************************************************************
 * @brief            Open the all supported devices.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_initialize_device(void);

/***********************************************************************************************************************
 * @brief            Sub procedure of R_OSAL_OS_IoGetNumOfDevices().
 * @param[in]        p_deviceType      A pointer to the device type.
 * @param[in,out]    p_numOfDevices    The number of the devices.
 * @param[in]        p_glob_buffer     A pointer to the glob buffer.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_num_of_device(const char* p_deviceType, size_t* const p_numOfDevices, glob_t const* p_glob_buffer);

/***********************************************************************************************************************
 * @brief            Set the regular expression string to the p_pattern.
 * @param[in]        p_type       A pointer to the type of pattern.
 * @param[in,out]    p_pattern    A pointer to the regular expression string pattern.
 * @param[in]        size         The Size of regular expression string pattern.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_create_regex_pattern(const char* p_type, char* p_pattern, size_t size);

/***********************************************************************************************************************
 * @brief            Count the number of devices matching with the p_dev_pattern.
 * @param[in]        p_glob_buffer    A pointer to the glob buffer.
 * @param[in]        p_dev_pattern    A pointer to the device pattern.
 * @param[in,out]    p_match_count    A pointer to the match count.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_count_device(glob_t const* p_glob_buffer, char const* p_dev_pattern, size_t* p_match_count);

/***********************************************************************************************************************
 * @brief            Sub procedure of R_OSAL_OS_IoGetDeviceList().
 * @param[in]        p_device_type_name    A pointer to the name of device type.
 * @param[in,out]    p_array               A pointer to the device list.
 * @param[in]        array_size            Size of the device list.
 * @param[in,out]    p_num_of_byte         A pointer to the number of bytes in device list.
 * @param[in]        p_glob_buffer         A pointer to the glob buffer.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_devicelist(const char* p_device_type_name, char* p_array, size_t array_size, size_t* const p_num_of_byte, glob_t const* p_glob_buffer);

/***********************************************************************************************************************
 * @brief            Create the device list for return from R_OSAL_OS_IoGetDeviceList().
 * @param[in]        p_glob_buffer    A pointer to the glob buffer.
 * @param[in]        p_dev_pattern    A pointer to the device pattern.
 * @param[in,out]    pp_list_buffer   A pointer to the list buffer.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_UNKNOWN_NAME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_generate_list(glob_t const* p_glob_buffer, char const* p_dev_pattern, char** pp_list_buffer);

/***********************************************************************************************************************
 * @brief            Allocate memory and add device name.
 * @param[in]        p_glob_buffer    A pointer to the glob buffer.
 * @param[in,out]    pp_list_buffer   A pointer to the list buffer.
 * @param[in]        count            The index of the list.
 * @param[in,out]    p_dst_offset     A pointer to the list offset value.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_add_item_to_list(glob_t const* p_glob_buffer, char** pp_list_buffer, size_t count, size_t* p_dst_offset);

/***********************************************************************************************************************
 * @brief            Sub procedure of R_OSAL_OS_IoDeviceOpen().
 * @param[in,out]    handle    A handle of device information.
 * @param[in]        p_id      A pointer to the device id.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_ID
 * @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_open(osal_device_handle_t handle, const char* p_id);

/***********************************************************************************************************************
 * @brief            Lock the device file.
 * @param[in,out]    handle    A handle of device information.
 * @param[in]        p_id      A pointer to the device id.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_ID
 * @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_lock_device(osal_device_handle_t handle, const char* p_id);

/***********************************************************************************************************************
 * @brief            Get the UIO link path from the device name.
 * @param[in]        p_uio_dev_name     A pointer to the uio device name.
 * @param[in,out]    p_uio_link_name    A pointer to the uio link name.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_uio_link(char const* p_uio_dev_name, char* p_uio_link_name);

/***********************************************************************************************************************
 * @brief            Get the map information.
 * @param[in,out]    handle                 A handle of device information.
 * @param[in]        p_uio_symbolic_name    A pointer to uio symbolic name.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_map_information(osal_device_handle_t handle, const char* p_uio_symbolic_name);

/***********************************************************************************************************************
 * @brief            Get the map size from UIO.
 * @param[in,out]    handle                 A handle of device information.
 * @param[in]        p_uio_symbolic_name    A pointer to uio symbolic name.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_map_size(osal_device_handle_t handle, const char* p_uio_symbolic_name);

/***********************************************************************************************************************
 * @brief            Set the IRQ information.
 * @param[in,out]    handle                 A handle of device information.
 * @param[in]        p_uio_symbolic_name    A pointer to uio symbolic name.
 * @param[in]        p_id                   A pointer to the device id.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_set_irq_information(osal_device_handle_t handle, char* p_uio_symbolic_name, const char* p_id);

/***********************************************************************************************************************
 * @brief            Get the number of IRQ from UIO.
 * @param[in,out]    handle                 A handle of device information.
 * @param[in]        p_uio_symbolic_name    A pointer to uio symbolic name.
 * @param[in]        p_id                   A pointer to the device id.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_irq_num(osal_device_handle_t handle, char* p_uio_symbolic_name, const char* p_id);

/***********************************************************************************************************************
 * @brief            Initialize PM policy.
 * @param[in,out]    p_device    A pointer to the device information.
 * @return           #void
***********************************************************************************************************************/
OSAL_STATIC void r_osal_os_io_initialize_for_pm(st_osal_device_list_t* p_device);

/***********************************************************************************************************************
 * @brief            Get the HW status of the power and clock.
 * @param[in,out]    p_device    A pointer to the device information.
 * @param[in,out]    p_pw_state       A pointer to the power state.
 * @param[in,out]    p_clock_state    A pointer to the clock state.
 * @return           #void
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
/* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
OSAL_STATIC void r_osal_os_io_get_pm_state(st_osal_device_list_t* p_device, int* p_pw_state, int* p_clock_state);

/***********************************************************************************************************************
 * @brief            Free the resources when the R_OSAL_OS_IoDeviceOpen() error.
 * @param[in,out]    handle                 A handle of device information.
 * @return           #void
***********************************************************************************************************************/
OSAL_STATIC void r_osal_os_io_open_error(osal_device_handle_t handle);

/***********************************************************************************************************************
 * @brief            Unset the IRQ information.
 * @param[in,out]    handle    A handle of device information.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_unset_irq_information(osal_device_handle_t handle);

/***********************************************************************************************************************
 * @brief            If file descripter for R_OSAL_OS_PmSetPostClock() is opened, close file descripter.
 * @param[in,out]    handle    A handle of device information.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_close_post_fd(osal_device_handle_t handle);

/***********************************************************************************************************************
 * @brief            Sub procedure of R_OSAL_OS_IoGetAxiBusInformation() with the OSAL_OS_IO_GET_DEVICE_AXI_BUS_ID.
 * @param[in,out]    p_param    A pointer to the axi bus information.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_device_axi_bus_id(st_osal_os_io_get_device_axi_bus_id_t* p_param);

/***********************************************************************************************************************
 * @brief            Sub procedure of R_OSAL_OS_IoGetAxiBusInformation() with the OSAL_OS_IO_GET_AXI_BUS_ID_FROM_DEVICE_NAME.
 * @param[in,out]    p_param    A pointer to the axi bus information.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_axi_bus_id_from_device_name(st_osal_os_io_get_axi_bus_id_from_device_name_t* p_param);

/***********************************************************************************************************************
 * @brief            Sub procedure of R_OSAL_OS_IoGetAxiBusInformation() with the OSAL_OS_IO_GET_AXI_BUS_NAME_LIST.
 * @param[in,out]    p_param    A pointer to the axi bus information.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_axi_bus_name_list(st_osal_os_io_get_axi_bus_name_list_t* p_param);

/***********************************************************************************************************************
 * @brief            Sub procedure of R_OSAL_OS_IoGetAxiBusInformation() with the OSAL_OS_IO_GET_AXI_BUS_NAME.
 * @param[in,out]    p_param    A pointer to the axi bus information.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_axi_bus_name(st_osal_os_io_get_axi_bus_name_t* p_param);

/***********************************************************************************************************************
 * @brief            Sub procedure of R_OSAL_OS_IoGetAxiBusInformation() with the OSAL_OS_IO_GET_NUM_OF_AXI_IDS_FOR_AXI_BUS_NAME.
 * @param[in,out]    p_param    A pointer to the axi bus information.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_UNKNOWN_NAME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_num_of_axi_ids_for_axi_bus_name(st_osal_os_io_get_num_of_axi_ids_for_axi_bus_name_t* p_param);

/***********************************************************************************************************************
 * @brief            Sub procedure of R_OSAL_OS_IoGetAxiBusInformation() with the OSAL_OS_IO_GET_AXI_ID_LIST_FOR_AXI_BUS_NAME.
 * @param[in,out]    p_param    A pointer to the axi bus information.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_UNKNOWN_NAME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_axi_id_list_for_axi_bus_name(st_osal_os_io_get_axi_id_list_for_axi_bus_name_t* p_param);

/***********************************************************************************************************************
 * @brief            Sub procedure of R_OSAL_OS_IoGetAxiBusInformation() with the OSAL_OS_IO_GET_NUM_OF_AXI_IDS_FOR_DEVICE_HDL.
 * @param[in,out]    p_param    A pointer to the axi bus information.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_num_of_axi_ids_for_device_hdl(st_osal_os_io_get_num_of_axi_ids_for_device_hdl_t* p_param);

/***********************************************************************************************************************
 * @brief            Sub procedure of R_OSAL_OS_IoGetAxiBusInformation() with the OSAL_OS_IO_GET_AXI_ID_LIST_FOR_DEVICE_HDL.
 * @param[in,out]    p_param    A pointer to the axi bus information.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_axi_id_list_for_device_hdl(st_osal_os_io_get_axi_id_list_for_device_hdl_t* p_param);

/***********************************************************************************************************************
 * @brief            Sub procedure of R_OSAL_OS_IoGetAxiBusInformation() with the OSAL_OS_IO_GET_NUM_OF_AXI_IDS_FOR_DEVICE_NAME.
 * @param[in,out]    p_param    A pointer to the axi bus information.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_num_of_axi_ids_for_device_name(st_osal_os_io_get_num_of_axi_ids_for_device_name_t* p_param);

/***********************************************************************************************************************
 * @brief            Sub procedure of R_OSAL_OS_IoGetAxiBusInformation() with the OSAL_OS_IO_GET_AXI_ID_LIST_FOR_DEVICE_NAME.
 * @param[in,out]    p_param    A pointer to the axi bus information.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_axi_id_list_for_device_name(st_osal_os_io_get_axi_id_list_for_device_name_t* p_param);

/***********************************************************************************************************************
 * @brief            Get the domain name from p_axi_bus_name.
 * @param[in]        p_axi_bus_name    A pointer to the axi bus name.
 * @param[in,out]    p_domain_name     A pointer to the domain name.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_UNKNOWN_NAME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_domain_name(const char* p_axi_bus_name, char* p_domain_name);

/***********************************************************************************************************************
 * @brief            Parse the uTLB included AXI bus name.
 * @param[in]        p_axi_bus_name    A pointer to the axi bus name.
 * @param[in]        len               length of the axi bus name.
 * @param[in,out]    pp_list           A pointer to the list.
 * @param[in,out]    p_numOfutlb       A pointer to the index.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_UNKNOWN_NAME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_utlb_parser(const char* p_axi_bus_name, size_t len, int32_t** pp_list, size_t* const p_numOfutlb);

/***********************************************************************************************************************
 * @brief            Sub procedure of r_osal_os_io_utlb_parser().
 * @param[in,out]    pp_list      A pointer to the list.
 * @param[in,out]    p_index      A pointer to the index.
 * @param[in]        p_num_str    A pointer to the index.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_UNKNOWN_NAME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_utlb_parser_sub(int32_t** pp_list, size_t* p_index, const char* p_num_str);

/***********************************************************************************************************************
 * @brief            Sub procedure of r_osal_os_io_utlb_parser().
 * @param[in,out]    pp_list    A pointer to the list.
 * @param[in,out]    p_index    A pointer to the index.
 * @param[in]        num        Number of the index.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_utlb_parser_sub1(int32_t** pp_list, size_t* p_index, int32_t num);

/***********************************************************************************************************************
 * @brief            Sub procedure of r_osal_os_io_utlb_parser().
 * @param[in,out]    pp_list    A pointer to the list.
 * @param[in,out]    p_index    A pointer to the index.
 * @param[in]        num        Number of the index.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_UNKNOWN_NAME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_utlb_parser_sub2(int32_t** pp_list, size_t* p_index, int32_t num);

/***********************************************************************************************************************
 * @brief            Get the SPI number from UIO.
 * @param[in]        p_irq_uio_name         A pointer to IRQ uio symbolic name.
 * @param[in,out]    p_spi_num              A pointer to the SPI number.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_spi_num(char* p_irq_uio_name, uint64_t* p_spi_num);

/***********************************************************************************************************************
 * @brief            Open the device file.
 * @param[in,out]    p_device    A pointer to the device information.
 * @param[in]        p_path      A pointer to the list of file names.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_open_device(st_osal_device_list_t* p_device, const char* p_path);

/***********************************************************************************************************************
 * @brief            File scan with the unsigned long format.
 * @param[in]        p_path_format          A pointer to the uio driver.
 * @param[in]        p_uio_symbolic_name    A pointer to the uio driver name.
 * @param[in,out]    p_data                 A pointer to the map size.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
/* PRQA S 5209 1 # System is requesting an unsigned long, so it cannot be changed. */
OSAL_STATIC e_osal_return_t r_osal_os_io_scan_ulong(const char* p_path_format, const char* p_uio_symbolic_name, unsigned long* p_data);

/***********************************************************************************************************************
 * @brief            Common function for R_OSAL_OS_Malloc().
 * @param[in,out]    pp_ptr    A pointer to the allocated memory.
 * @param[in]        size      Size of memory to allocate.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_malloc(void** pp_ptr, size_t size);

/***********************************************************************************************************************
 * @brief            Common function for calloc().
 * @param[in,out]    pp_ptr    A pointer to the allocated memory.
 * @param[in]        nmemb     Number of memory to allocate.
 * @param[in]        size      Size of memory to allocate.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_calloc(void** pp_ptr, size_t nmemb, size_t size);

/***********************************************************************************************************************
 * @brief            Common function for destroy the mutex.
 * @param[in]        p_mutex    A pointer to the mutex information.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_mutex_destroy(osal_os_mutex_t* p_mutex);

/***********************************************************************************************************************
 * @brief            Common function for destroy the condition variable.
 * @param[in]        p_cond    A pointer to the condition variable information.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_cond_destroy(osal_os_cond_t* p_cond);

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_101
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_IoInit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_IoInit(void)
{
    e_osal_return_t osal_ret;

    osal_ret = r_osal_os_io_read_uio_dir();

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_io_initialize_axi_bus_info();
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_io_initialize_device();
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_IoInit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_025]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_102
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_IoDeinit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_IoDeinit(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t          count;
    size_t          local_num_axi_ids = (sizeof(gs_osal_device_axi_ids))/(sizeof(st_osal_axi_bus_info_t));

    /* Skip the "invalid" and "mm(ipa)" */
    for (count = 2; count < local_num_axi_ids; count++)
    {
        gs_osal_device_axi_ids[count].used = false;
        gs_osal_device_axi_ids[count].axi_bus_id = OSAL_AXI_BUS_ID_INVALID;
    }

    /* The all device close */
    if (NULL != gsp_device_list)
    {
        count = 0;
        while (-1 != gsp_device_list[count].fd)
        {
            osal_ret = R_OSAL_OS_Close(gsp_device_list[count].fd);

            /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
            R_OSAL_OS_Free((void *)gsp_device_list[count].p_id);
            gsp_device_list[count].p_id = NULL;

            count++;
        }

        /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
        R_OSAL_OS_Free((void *)gsp_device_list);
        gsp_device_list = NULL;
    }

    /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
    R_OSAL_OS_Free((void *)gsp_io_axi_bus_name_list);
    gsp_io_axi_bus_name_list = NULL;

    if (NULL != gs_axi_bus_name_info)
    {
        for (count = 0; count < gs_axi_bus_name_info_array_size; count++)
        {
            /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
            R_OSAL_OS_Free((void *)gs_axi_bus_name_info[count].dev_names);
            gs_axi_bus_name_info[count].dev_names = NULL;
        }

        /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
        R_OSAL_OS_Free((void *)gs_axi_bus_name_info);
        gs_axi_bus_name_info = NULL;

        gs_axi_bus_name_info_array_size = 0;
    }

    gs_io_axi_bus_name_list_len = 0;

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_IoDeinit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_026]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_103
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_IoGetNumOfDevices()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_IoGetNumOfDevices(const char* p_deviceType, size_t* const p_numOfDevices)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;
    char*           p_dev_pattern = NULL;
    glob_t          glob_buffer;

    if ((NULL == p_deviceType) ||
        (NULL == p_numOfDevices))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0310 1 # The parameter must be (void **) and there is no side-effect in this case. */
        osal_ret = r_osal_os_io_malloc((void **)&p_dev_pattern, (size_t)(sizeof(char) * (size_t)PATH_MAX));
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        linux_ret = snprintf(p_dev_pattern, (size_t)(sizeof(char) * (size_t)PATH_MAX), "/dev/%s_[0-9a-z]*", p_deviceType);
        if (linux_ret < 0)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        linux_ret = glob(p_dev_pattern, 0, NULL, &glob_buffer);
        if (0 != linux_ret)
        {
            *p_numOfDevices = 0; /* No device was found */
        }
        else
        {
            osal_ret = r_osal_os_io_get_num_of_device(p_deviceType, p_numOfDevices, &glob_buffer);
            globfree(&glob_buffer);
        }
    }

    /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
    R_OSAL_OS_Free((void *)p_dev_pattern);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_IoGetNumOfDevices()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_027]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_104
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_IoGetDeviceList()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_IoGetDeviceList(const char* p_device_type_name, char* p_buffer, size_t buffer_size, size_t* const p_num_of_byte)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;
    char*           p_dev_pattern = NULL;
    glob_t          glob_buffer;

    if ((NULL == p_device_type_name) ||
        (NULL == p_num_of_byte))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0310 1 # The parameter must be (void **) and there is no side-effect in this case. */
        osal_ret = r_osal_os_io_malloc((void **)&p_dev_pattern, (size_t)(sizeof(char) * (size_t)PATH_MAX));
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Primary classification and reduce candidates */
        linux_ret = snprintf(p_dev_pattern, (size_t)(sizeof(char) * (size_t)PATH_MAX), "/dev/%s_[0-9a-z]*", p_device_type_name);
        if (linux_ret < 0)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        linux_ret = glob(p_dev_pattern, 0, NULL, &glob_buffer);
        if (0 != linux_ret)
        {
            /* No device was found related to p_device_type_name */
            *p_num_of_byte = 0;
            osal_ret = OSAL_RETURN_UNKNOWN_NAME;
        }
        else
        {
            osal_ret = r_osal_os_io_get_devicelist(p_device_type_name, p_buffer, buffer_size, p_num_of_byte, &glob_buffer);
            globfree(&glob_buffer);
        }
    }

    /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
    R_OSAL_OS_Free((void *)p_dev_pattern);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_IoGetDeviceList()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_028]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_105
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_IoDeviceOpen()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_OS_IoDeviceOpen(const char* p_id, osal_device_handle_t* p_handle)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    if ((NULL == p_id) ||
        (NULL == p_handle))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_io_open(*p_handle, p_id);
    }

    if (OSAL_RETURN_OK != osal_ret)
    {
        /* PRQA S 2812 1 # The pointer and size are checked in caller function */
        r_osal_os_io_open_error(*p_handle);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_IoDeviceOpen()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_029]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_106
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_IoDeviceClose()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_IoDeviceClose(osal_device_handle_t handle)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;

    if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_io_unset_irq_information(handle);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (NULL != handle->base_addr)
        {
            linux_ret = munmap(handle->base_addr, handle->len);
            if (0 != linux_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        linux_ret = flock(handle->dev_fd, LOCK_UN);
        if (0 == linux_ret)
        {
            gsp_device_list[handle->dev_index].locked = false;
        }
        else
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        handle->dev_fd = -1;
        handle->base_addr = NULL;
        handle->handle_id = OSAL_INVALID_HANDLE_ID;

        if (-1 != handle->dev_post_fd)
        {
            osal_ret = r_osal_os_io_close_post_fd(handle);
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_IoDeviceClose()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_030]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_107
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_IoRead()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_OS_IoRead(osal_device_handle_t const handle, uintptr_t offset, size_t size, size_t unit_size, void* p_data)
{
    e_osal_return_t             osal_ret = OSAL_RETURN_OK;
    size_t                      count;
    volatile uint8_t const*     p_buf8;
    volatile uint16_t const*    p_buf16;
    volatile uint32_t const*    p_buf32;
    volatile uint64_t const*    p_buf64;
    /* PRQA S 0316 4 # Required cast on system, not modifiable. */
    uint8_t*                    p_data8     = (uint8_t *)p_data;
    uint16_t*                   p_data16    = (uint16_t *)p_data;
    uint32_t*                   p_data32    = (uint32_t *)p_data;
    uint64_t*                   p_data64    = (uint64_t *)p_data;
    size_t                      data_num    = size / unit_size;
    uintptr_t                   local_offset= offset / unit_size;

    if ((NULL == handle) ||
        (NULL == p_data))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 4 # This casting is mandatory for read operation. */
        p_buf8  = (uint8_t *)handle->base_addr;
        p_buf16 = (uint16_t *)handle->base_addr;
        p_buf32 = (uint32_t *)handle->base_addr;
        p_buf64 = (uint64_t *)handle->base_addr;

        for (count = 0; count < data_num; count++)
        {
            switch(unit_size)
            {
                case sizeof(uint8_t) :
                    p_data8[count] = p_buf8[local_offset + count];
                    break;

                case sizeof(uint16_t) :
                    p_data16[count] = p_buf16[local_offset + count];
                    break;

                case sizeof(uint32_t) :
                    p_data32[count] = p_buf32[local_offset + count];
                    break;

                case sizeof(uint64_t) :
                    p_data64[count] = p_buf64[local_offset + count];
                    break;

                default :
                    osal_ret = OSAL_RETURN_FAIL;
                    break;
            } /* end of switch(unit_size) */
        } /* end of for (count = 0; count < data_num; count++) */
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_IoRead()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_031]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_108
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_IoWrite()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_OS_IoWrite(osal_device_handle_t const handle, uintptr_t offset, size_t size, size_t unit_size, const void* p_data)
{
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    size_t              count;
    volatile uint8_t*   p_buf8;
    volatile uint16_t*  p_buf16;
    volatile uint32_t*  p_buf32;
    volatile uint64_t*  p_buf64;
    /* PRQA S 0316 4 # Required cast on system, not modifiable. */
    const uint8_t*      p_data8     = (const uint8_t*)p_data;
    const uint16_t*     p_data16    = (const uint16_t*)p_data;
    const uint32_t*     p_data32    = (const uint32_t*)p_data;
    const uint64_t*     p_data64    = (const uint64_t*)p_data;
    size_t              data_num    = size / unit_size;
    uintptr_t           local_offset= offset / unit_size;

    if ((NULL == handle) ||
        (NULL == p_data))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 4 # This casting is mandatory for write operation. */
        p_buf8  = (uint8_t *)handle->base_addr;
        p_buf16 = (uint16_t *)handle->base_addr;
        p_buf32 = (uint32_t *)handle->base_addr;
        p_buf64 = (uint64_t *)handle->base_addr;

        for (count = 0; count < data_num; count++)
        {
            switch(unit_size)
            {
                case sizeof(uint8_t) :
                    p_buf8[local_offset + count] = p_data8[count];
                    break;

                case sizeof(uint16_t) :
                    p_buf16[local_offset + count] = p_data16[count];
                    break;

                case sizeof(uint32_t) :
                    p_buf32[local_offset + count] = p_data32[count];
                    break;

                case sizeof(uint64_t) :
                    p_buf64[local_offset + count] = p_data64[count];
                    break;

                default :
                    osal_ret = OSAL_RETURN_FAIL;
                    break;
            } /* end of switch(unit_size) */
        } /* end of for (count = 0; count < data_num; count++) */
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_IoWrite()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_032]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_109
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_IoGetAxiBusInformation()
***********************************************************************************************************************/
/* PRQA S 9106 2 # This function is intended to be used in common. */
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_OS_IoGetAxiBusInformation(e_osal_get_axi_bus_type_t get_axi_bus_type, void* p_param)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    if (NULL == p_param)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        switch(get_axi_bus_type)
        {
            case OSAL_OS_IO_GET_DEVICE_AXI_BUS_ID :
                /* PRQA S 0316 1 # Required cast on system, not modifiable. */
                osal_ret = r_osal_os_io_get_device_axi_bus_id((st_osal_os_io_get_device_axi_bus_id_t *)p_param);
                break;

            case OSAL_OS_IO_GET_AXI_BUS_ID_FROM_DEVICE_NAME :
                /* PRQA S 0316 1 # Required cast on system, not modifiable. */
                osal_ret = r_osal_os_io_get_axi_bus_id_from_device_name((st_osal_os_io_get_axi_bus_id_from_device_name_t *)p_param);
                break;

            case OSAL_OS_IO_GET_AXI_BUS_NAME_LIST :
                /* PRQA S 0316 1 # Required cast on system, not modifiable. */
                osal_ret = r_osal_os_io_get_axi_bus_name_list((st_osal_os_io_get_axi_bus_name_list_t *)p_param);
                break;

            case OSAL_OS_IO_GET_AXI_BUS_NAME :
                /* PRQA S 0316 1 # Required cast on system, not modifiable. */
                osal_ret = r_osal_os_io_get_axi_bus_name((st_osal_os_io_get_axi_bus_name_t *)p_param);
                break;

            case OSAL_OS_IO_GET_NUM_OF_AXI_IDS_FOR_AXI_BUS_NAME :
                /* PRQA S 0316 1 # Required cast on system, not modifiable. */
                osal_ret = r_osal_os_io_get_num_of_axi_ids_for_axi_bus_name((st_osal_os_io_get_num_of_axi_ids_for_axi_bus_name_t *)p_param);
                break;

            case OSAL_OS_IO_GET_AXI_ID_LIST_FOR_AXI_BUS_NAME :
                /* PRQA S 0316 1 # Required cast on system, not modifiable. */
                osal_ret = r_osal_os_io_get_axi_id_list_for_axi_bus_name((st_osal_os_io_get_axi_id_list_for_axi_bus_name_t *)p_param);
                break;

            case OSAL_OS_IO_GET_NUM_OF_AXI_IDS_FOR_DEVICE_HDL :
                /* PRQA S 0316 1 # Required cast on system, not modifiable. */
                osal_ret = r_osal_os_io_get_num_of_axi_ids_for_device_hdl((st_osal_os_io_get_num_of_axi_ids_for_device_hdl_t *)p_param);
                break;

            case OSAL_OS_IO_GET_AXI_ID_LIST_FOR_DEVICE_HDL :
                /* PRQA S 0316 1 # Required cast on system, not modifiable. */
                osal_ret = r_osal_os_io_get_axi_id_list_for_device_hdl((st_osal_os_io_get_axi_id_list_for_device_hdl_t *)p_param);
                break;

            case OSAL_OS_IO_GET_NUM_OF_AXI_IDS_FOR_DEVICE_NAME :
                /* PRQA S 0316 1 # Required cast on system, not modifiable. */
                osal_ret = r_osal_os_io_get_num_of_axi_ids_for_device_name((st_osal_os_io_get_num_of_axi_ids_for_device_name_t *)p_param);
                break;

            case OSAL_OS_IO_GET_AXI_ID_LIST_FOR_DEVICE_NAME :
            default :
                /* PRQA S 0316 1 # Required cast on system, not modifiable. */
                osal_ret = r_osal_os_io_get_axi_id_list_for_device_name((st_osal_os_io_get_axi_id_list_for_device_name_t *)p_param);
                break;
        } /* end of switch(get_axi_bus_type) */
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
/* PRQA S 9107 2 # The termination function has already multiple sub-functions to reduce complexity.
   We have decided that further division is not desirable from perspective of maintainability and readability. */
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_IoGetAxiBusInformation()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_033]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_110
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_read_uio_dir()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_read_uio_dir(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;
    DIR*            p_dir;

    p_dir = opendir("/sys/class/uio/");
    if (NULL == p_dir)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_io_set_always_on(p_dir);

        linux_ret = closedir(p_dir);
        if (0 != linux_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_read_uio_dir()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_034]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_111
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_set_always_on()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_set_always_on(DIR* p_dir)
{
    e_osal_return_t         osal_ret = OSAL_RETURN_OK;
    struct dirent const*    pst_entry;
    /* PRQA S 5209 2 # Return values from linux must be int. Used according to the specifications. */
    int                     cmp_ret1;
    int                     cmp_ret2;
    uint64_t                pm_domain;

    gs_io_pd_always_on = OSAL_PD_ALWAYS_ON_TYPE1;

    do
    {
        pst_entry = readdir(p_dir);
        if (NULL != pst_entry)
        {
            cmp_ret1 = strncmp(pst_entry->d_name, ".", strlen(pst_entry->d_name));
            cmp_ret2 = strncmp(pst_entry->d_name, "..", strlen(pst_entry->d_name));

            if ((0 != cmp_ret1) && (0 != cmp_ret2))
            {
                osal_ret = r_osal_os_io_get_power_domain(pst_entry->d_name, &pm_domain);
                if ((OSAL_RETURN_OK == osal_ret) &&
                    ((uint64_t)0 != (pm_domain & OSAL_PD_ALWAYS_ON_TYPE2)))
                {
                    gs_io_pd_always_on = OSAL_PD_ALWAYS_ON_TYPE2;
                    break;
                }
            }
        }
    } while((NULL != pst_entry) && (OSAL_RETURN_OK == osal_ret));

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_set_always_on()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_035]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_112
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_get_power_domain()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_power_domain(const char* p_uio_symbolic_name, uint64_t* p_pm_domain)
{
    e_osal_return_t osal_ret;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;
    size_t          fread_ret;
    char*           p_file_name = NULL;
    FILE*           p_power_domain_fd;

    /* If the file can not open, OSAL_PD_ALWAYS_ON_TYPE1 returns */
    *p_pm_domain = gs_io_pd_always_on;

    /* PRQA S 0310 1 # The parameter must be (void **) and there is no side-effect in this case. */
    osal_ret = r_osal_os_io_malloc((void **)&p_file_name, PATH_MAX);

    if (OSAL_RETURN_OK == osal_ret)
    {
        linux_ret = snprintf(p_file_name, PATH_MAX, "/sys/class/uio/%s/device/of_node/power-domains", p_uio_symbolic_name);
        if (linux_ret < 0)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 5124, 2702 1 # Closed in this function. */
        p_power_domain_fd = fopen(p_file_name, "r");
        if (NULL != p_power_domain_fd)
        {
            /* PRQA S 5124 2 # Read one uint64_t data from the opened fd. No side-effect. */
            /* PRQA S 0314 1 # The 1st arguments must be (void *) but no side effects in this case. */
            fread_ret = fread((void *)p_pm_domain, sizeof(uint64_t), 1, p_power_domain_fd);
            if ((size_t)0 == fread_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }

            linux_ret = fclose(p_power_domain_fd); /* PRQA S 5124 # Close the fd opened in this function. */
            if (0 != linux_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
        }
    }

    /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
    R_OSAL_OS_Free((void *)p_file_name);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_get_power_domain()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_036]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_113
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_initialize_axi_bus_info()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_initialize_axi_bus_info(void)
{
    e_osal_return_t osal_ret;

    osal_ret = r_osal_os_io_initialize_bus_information();

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_io_initialize_axi_bus_name_table();
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_io_create_bus_name_list();
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_initialize_axi_bus_info()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_037]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_114
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_initialize_bus_information()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_initialize_bus_information(void)
{
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int                 linux_ret;
    size_t              cnt;
    glob_t              globbuf;
    osal_axi_bus_id_t   ipmmu_id;
    osal_axi_bus_id_t   local_axi_bus_id = OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY;
    size_t              local_num_axi_ids = sizeof(gs_osal_device_axi_ids) / sizeof(st_osal_axi_bus_info_t);

    /* Read available ipmmu-id from /sys/class/uio/%s/device/of_node/ipmmu-id */
    linux_ret = glob("/dev/uio*", 0, NULL, &globbuf);
    if (0 != linux_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        for (cnt = 0; cnt < globbuf.gl_pathc; cnt++)
        {
            osal_ret = r_osal_os_io_get_ipmmu_id(&((globbuf.gl_pathv[cnt])[5]), &ipmmu_id); /* to skip "/dev/" */

            if ((OSAL_RETURN_OK == osal_ret) && (local_num_axi_ids <= ipmmu_id))
            {
                osal_ret = OSAL_RETURN_FAIL;
            }

            if (OSAL_RETURN_OK != osal_ret)
            {
                break;
            }

            /* Set the AXI bus ID */
            if (false == gs_osal_device_axi_ids[ipmmu_id].used)
            {
                gs_osal_device_axi_ids[ipmmu_id].used = true;
                local_axi_bus_id++;
                gs_osal_device_axi_ids[ipmmu_id].axi_bus_id = local_axi_bus_id;
            }
        } /* end of for (cnt = 0; cnt < globbuf.gl_pathc; cnt++) */
        globfree(&globbuf);
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_initialize_bus_information()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_038]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_115
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_get_ipmmu_id()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_ipmmu_id(const char* const p_uio_name, osal_axi_bus_id_t* p_ipmmu_id)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;
    size_t          fread_ret;
    char*           p_sysfs_ipmmu_file = NULL;
    char const      sysfs_ipmmu_file_fmt[] = "/sys/class/uio/%s/device/of_node/ipmmu-id";
    FILE*           p_ipmmu_fd;
    uint32_t        local_ipmmu_id = 0;

    if ((NULL == p_uio_name) || (NULL == p_ipmmu_id))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0310 1 # The parameter must be (void **) and there is no side-effect in this case. */
        osal_ret = r_osal_os_io_malloc((void **)&p_sysfs_ipmmu_file, (size_t)(sizeof(char) * (size_t)PATH_MAX));

        if (OSAL_RETURN_OK == osal_ret)
        {
            linux_ret = snprintf(p_sysfs_ipmmu_file, (size_t)(sizeof(char) * (size_t)PATH_MAX), sysfs_ipmmu_file_fmt, p_uio_name);
            if (linux_ret < 0)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            p_ipmmu_fd = fopen(p_sysfs_ipmmu_file, "rb"); /* PRQA S 5124, 2702 # Closed in this function. */
            if (NULL == p_ipmmu_fd)
            {
                /* Do nothing. Some devices do not have ipmmu-id */
            }
            else
            {
                /* PRQA S 5124 2 # Read one uint32_t data from the opened fd. No side-effect. */
                /* PRQA S 0314 1 # The 1st arguments must be (void *) but no side effects in this case. */
                fread_ret = fread((void *) &local_ipmmu_id, sizeof(uint32_t), 1, p_ipmmu_fd);
                if ((size_t)0 == fread_ret)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }

                /* PRQA S 5124 1 # Close the opened fd. p_ipmmu_fd is opened in this function. */
                linux_ret = fclose(p_ipmmu_fd);
                if (0 != linux_ret)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
            }
        } /* end of if (OSAL_RETURN_OK == osal_ret) osal_ret is return value from snprintf */
    } /* end of if (OSAL_RETURN_OK == osal_ret) osal_ret is return value from if ((NULL == p_uio_name) || (NULL == p_ipmmu_id)) */

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (0U != local_ipmmu_id)
        {
            *p_ipmmu_id = (osal_axi_bus_id_t)ntohl(local_ipmmu_id);
        }
        else
        {
            *p_ipmmu_id = (osal_axi_bus_id_t)OSAL_AXI_BUS_ID_INVALID;
        }
    }
    else
    {
        if (NULL != p_ipmmu_id)
        {
            *p_ipmmu_id = (osal_axi_bus_id_t)OSAL_AXI_BUS_ID_INVALID;
        }
    }

    /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
    R_OSAL_OS_Free((void *)p_sysfs_ipmmu_file);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_get_ipmmu_id()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_039]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_116
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_initialize_axi_bus_name_table()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_initialize_axi_bus_name_table(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 2 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;
    int             cmp_ret[2];
    size_t          count;
    /* PRQA S 2981 1 # Since this variable is used in the judgment, it is necessary to initialize. Do not change. */
    size_t          start_count = 0;
    glob_t          glob_buffer;
    char            bus_domain[OSAL_BUS_DOMAIN_SIZE];
    bool            invalid_flag = false;
    size_t          invalid_index = 0;
    size_t          mm_index = 0;

    glob_buffer.gl_offs = 2; /* reserve 2 positions for invalid and mm(ipa) */

    /* PRQA S 4543 1 # GLOB_DOOFFS are used in accordance with the specification. */
    linux_ret = glob("/sys/class/uio/uio*", GLOB_DOOFFS, NULL, &glob_buffer);
    if (0 != linux_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    /* Skip the gl_offs */
    for (count = 2; (OSAL_RETURN_OK == osal_ret) && (count < glob_buffer.gl_pathc); count++)
    {
        osal_ret = r_osal_os_io_get_bus_domain(glob_buffer.gl_pathv[count], bus_domain);
        if (OSAL_RETURN_OK == osal_ret)
        {
            cmp_ret[0] = strcmp("invalid", bus_domain);
            cmp_ret[1] = strcmp("mm(ipa)", bus_domain);
            if ((0 == cmp_ret[0]) && (false == invalid_flag))
            {
                invalid_index   = count;
                invalid_flag    = true;
            }
            else if (0 == cmp_ret[1])
            {
                mm_index = count; /* no need flag because there is only one mm(ipa) */
            }
            else
            {
                /* Do nothing */
            }
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        glob_buffer.gl_pathv[0] = glob_buffer.gl_pathv[invalid_index];

        if (0U == mm_index)
        {
            osal_ret = r_osal_os_io_create_axi_bus_name_table(glob_buffer.gl_pathv[0]); /* register "invalid" */
            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = r_osal_os_io_register_axi_bus_info("mm(ipa)", "ipmmu_mm_00", OSAL_INVALID_UTLB); /* register "mm(ipa)" with dummy device */
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                /* Skip the invalid and mm(ipa) */
                start_count = 2;
            }
        }
        else
        {
            glob_buffer.gl_pathv[1] = glob_buffer.gl_pathv[mm_index];
            start_count = 0;
        }

        /* Does not skip the invalid and mm(ipa) */
        for (count = start_count; (OSAL_RETURN_OK == osal_ret) && (count < (glob_buffer.gl_pathc + 2U)); count++)
        {
            if ((invalid_index != count) && (mm_index != count))
            {
                osal_ret = r_osal_os_io_create_axi_bus_name_table(glob_buffer.gl_pathv[count]);
            }
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    if (0 == linux_ret)
    {
        globfree(&glob_buffer);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_io_register_axi_bus_info_last_data();
    }

    return osal_ret;
/* PRQA S 9104 1 # For better readability this rule is ignored, and this function is not split. */
}
/***********************************************************************************************************************
* End of function r_osal_os_io_initialize_axi_bus_name_table()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_040]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_117
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_create_axi_bus_name_table()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_create_axi_bus_name_table(const char* const p_uio_name)
{
    e_osal_return_t osal_ret;
    char            device_name[OSAL_DEVICE_NAME_SIZE];
    /* PRQA S 3673 1 # Since p_utlbs is actually rewritten, it is not necessary "const". */
    uint32_t*       p_utlbs = NULL;

    /* PRQA S 0314 1 # The 1st arguments must be (void *) but no side effects in this case. */
    (void)memset((void *)device_name, 0x00, (sizeof(device_name)/sizeof(char)));

    /* PRQA S 0310 1 # The parameter must be (void **) and there is no side-effect in this case. */
    osal_ret = r_osal_os_io_malloc((void **)&p_utlbs, sizeof(uint32_t) * OSAL_MAX_UTLB_NUM);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_io_get_device_name_from_uio(p_uio_name, device_name);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_io_get_info_of_axi_bus_name(p_uio_name, device_name, p_utlbs);
    }

    /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
    R_OSAL_OS_Free((void *)p_utlbs);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_create_axi_bus_name_table()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_041]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_118
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_get_device_name_from_uio()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_device_name_from_uio(const char* const p_uio_name, char* p_device_name)
{
    e_osal_return_t osal_ret;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;
    char const      sysfs_iommus_file_name[] = "%s/name";
    char*           p_sysfs_device_name_file = NULL;
    FILE*           p_dev_name_fd;

    /* PRQA S 0310 1 # The parameter must be (void **) and there is no side-effect in this case. */
    osal_ret = r_osal_os_io_malloc((void **)&p_sysfs_device_name_file, (size_t)(sizeof(char) * (size_t)PATH_MAX));

    if (OSAL_RETURN_OK == osal_ret)
    {
        linux_ret = snprintf(p_sysfs_device_name_file, (size_t)(sizeof(char) * (size_t)PATH_MAX), sysfs_iommus_file_name, p_uio_name);
        if (linux_ret < 0)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            p_dev_name_fd = fopen(p_sysfs_device_name_file, "rb"); /* PRQA S 5124, 2702 # Closed in this function. */
            if (NULL != p_dev_name_fd)
            {
                /* PRQA S 5124 1 # Read OSAL_DEVICE_NAME_SIZE data from the opened fd. No side-effect. */
                linux_ret = fscanf(p_dev_name_fd, "%s", p_device_name);
                if (1 != linux_ret)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }

                /* PRQA S 5124 1 # Close the opened fd. p_dev_name_fd is opened in this function. */
                linux_ret = fclose(p_dev_name_fd);
                if (0 != linux_ret)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
            }
            else
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
        } /* end of if (linux_ret < 0) */
    }

    /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
    R_OSAL_OS_Free((void *)p_sysfs_device_name_file);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_get_device_name_from_uio()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_042]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_119
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_get_info_of_axi_bus_name()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_info_of_axi_bus_name(const char* const p_uio_name, const char* const device_name, uint32_t* utlbs)
{
    e_osal_return_t osal_ret;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             cmp_ret;
    size_t          utlb_array_num;
    size_t          utlb_num = 0;
    int32_t         utlb;
    char            bus_domain[OSAL_BUS_DOMAIN_SIZE];

    osal_ret = r_osal_os_io_get_bus_domain(p_uio_name, bus_domain);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_io_get_utlbs_from_uio(utlbs, p_uio_name, &utlb_num);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        cmp_ret = strcmp("invalid", bus_domain);
        if ((0U == utlb_num) || (0 == cmp_ret)) /* set fix utlb OSAL_INVALID_UTLB in case bus_domain or utlb is invalid */
        {
            cmp_ret = strcmp("mm(ipa)", bus_domain);
            if (0 != cmp_ret)
            {
                (void)strcpy(bus_domain, "invalid"); /* set fix bus domain "invalid" for invalid utlb devices */
            }
            osal_ret = r_osal_os_io_register_axi_bus_info(bus_domain, device_name, OSAL_INVALID_UTLB);
        }
        else
        {
            for (utlb_array_num = 0; utlb_array_num < utlb_num; utlb_array_num++)
            {
                /* Example: iommus of imp_top_00 is <0x3a 0x0 0x3a 0x1>     */
                /*          Skip the 0x3a                                   */
                utlb = (int32_t)ntohl(utlbs[(utlb_array_num * 2U) + 1U]);
                osal_ret = r_osal_os_io_register_axi_bus_info(bus_domain, device_name, utlb);
            }
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_get_info_of_axi_bus_name()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_043]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_120
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_get_bus_domain()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_bus_domain(const char* const p_uio_symbolic_name, char* p_bus_domain)
{
    e_osal_return_t osal_ret;
    osal_axi_bus_id_t ipmmu_id = OSAL_AXI_BUS_ID_INVALID;
    size_t array_number;
    size_t bus_domain_number = 0;

    /* p_uio_symbolic_name is such as "/sys/class/uio/uioXXX"   */
    /* [15] is for skipping the "/sys/class/uio/"               */
    osal_ret = r_osal_os_io_get_ipmmu_id(&p_uio_symbolic_name[15], &ipmmu_id);

    if (OSAL_RETURN_OK == osal_ret)
    {
        for (array_number = 0; array_number < (sizeof(gs_osal_device_axi_ids) / sizeof(st_osal_axi_bus_info_t)); array_number++)
        {
            if (ipmmu_id == gs_osal_device_axi_ids[array_number].ipmmu_id)
            {
                bus_domain_number = array_number;
            }
        }

        (void)strcpy(p_bus_domain, gs_osal_device_axi_ids[bus_domain_number].axi_bus_name);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_get_bus_domain()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_044]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_121
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_get_utlbs_from_uio()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Since p_utlbs is actually rewritten, it is not necessary "const". */
OSAL_STATIC e_osal_return_t r_osal_os_io_get_utlbs_from_uio(uint32_t* p_utlbs, const char* const p_uio_name, size_t* p_utlb_num)
{
    e_osal_return_t osal_ret;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;
    char*           p_sysfs_iommus_file = NULL;
    FILE*           p_iommus_fd;
    char const      sysfs_iommus_file_fmt[] = "%s/device/of_node/iommus";
    size_t          get_utlb_number = 0;

    /* PRQA S 0310 1 # The parameter must be (void **) and there is no side-effect in this case. */
    osal_ret = r_osal_os_io_malloc((void **)&p_sysfs_iommus_file, (size_t)(sizeof(char) * (size_t)PATH_MAX));

    if (OSAL_RETURN_OK == osal_ret)
    {
        linux_ret = snprintf(p_sysfs_iommus_file, (size_t)(sizeof(char) * (size_t)PATH_MAX), sysfs_iommus_file_fmt, p_uio_name);
        if (linux_ret < 0)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            p_iommus_fd = fopen(p_sysfs_iommus_file, "rb"); /* PRQA S 5124, 2702 # Closed in this function. */
            if (NULL != p_iommus_fd)
            {
                /* PRQA S 5124 2 # Read OSAL_MAX_UTLB_NUM data from the opened fd. No side-effect. */
                /* PRQA S 0314 1 # The 1st arguments must be (void *) but no side effects in this case. */
                get_utlb_number = fread((void *)p_utlbs, sizeof(uint32_t), OSAL_MAX_UTLB_NUM, p_iommus_fd);

                /* PRQA S 5124 1 # Close the opened fd. p_iommus_fd is opened in this function. */
                linux_ret = fclose(p_iommus_fd);
                if (0 != linux_ret)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
            }
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* The number of uTLB is the number of iommus items / 2     */
        /* Example: iommus of imp_top_00 is <0x3a 0x0 0x3a 0x1>     */
        /*          Skip the 0x3a                                   */
        *p_utlb_num = get_utlb_number / 2U;
    }

    /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
    R_OSAL_OS_Free((void *)p_sysfs_iommus_file);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_get_utlbs_from_uio()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_045]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_122
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_register_axi_bus_info()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_register_axi_bus_info(const char* const p_bus_domain, const char* const p_device_name, int32_t utlb)
{
    e_osal_return_t                 osal_ret;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int                             linux_ret;
    size_t                          cnt;
    bool                            exist_flag = false;
    st_osal_axi_bus_name_info_t*    p_tmp_table;
    char const*                     p_strstr_ret;
    char*                           p_compare_str = NULL;

    /* Memory size is length of ('\n' + p_device_name + '\n' + '\0') */
    /* PRQA S 0310 1 # The parameter must be (void **) and there is no side-effect in this case. */
    osal_ret = r_osal_os_io_malloc((void **)&p_compare_str, strlen(p_device_name) + 3U);

    /* '\n' is separator of device name                         */
    /* The following Example, search the only "acf_00"          */
    /* Input parameter: acf_00                                  */
    /* Device name array: acf_00, irq_acf_00_01, fcprr_acf_00   */
    if (OSAL_RETURN_OK == osal_ret)
    {
        p_compare_str[0] = '\n';
        (void)strcpy(&p_compare_str[1], p_device_name);
        (void)strcat(p_compare_str, "\n");
    }

    for (cnt = 0; (OSAL_RETURN_OK == osal_ret) && (cnt < gs_axi_bus_name_info_array_size); cnt++)
    {
        linux_ret = strcmp(gs_axi_bus_name_info[cnt].domain_name, p_bus_domain);
        if ((0 == linux_ret) && (gs_axi_bus_name_info[cnt].utlb == utlb))
        {
            /* Element of AXI bus name information already exist */
            exist_flag = true;
            p_strstr_ret = strstr(gs_axi_bus_name_info[cnt].dev_names, p_compare_str);
            if (NULL == p_strstr_ret)
            {
                /* Add to dev_names */
                osal_ret = r_osal_os_io_device_name_connect(p_device_name, cnt);
            }
        }
    }

    if ((OSAL_RETURN_OK == osal_ret) && (false == exist_flag))
    {
        /* New entry to AXI bus name information */
        /* Use p_tmp_buffer to avoid memory leak in case realloc fail */
        /* PRQA S 0314 2 # The 1st arguments must be (void *) but no side effects in this case. */
        /* PRQA S 5118,2707 1 # Freed in deinitialize. */
        p_tmp_table = (st_osal_axi_bus_name_info_t *)realloc((void*)gs_axi_bus_name_info, (gs_axi_bus_name_info_array_size + 1U) * sizeof(st_osal_axi_bus_name_info_t));
        if (NULL != p_tmp_table)
        {
            gs_axi_bus_name_info = p_tmp_table;

            gs_axi_bus_name_info[gs_axi_bus_name_info_array_size].utlb = utlb;
            (void)strcpy(gs_axi_bus_name_info[gs_axi_bus_name_info_array_size].domain_name, p_bus_domain);

            /* Memory size is length of ('\n' + p_device_name + '\n' + '\0') */
            /* PRQA S 0310 1 # The parameter must be (void **) and there is no side-effect in this case. */
            osal_ret = r_osal_os_io_malloc((void **)&gs_axi_bus_name_info[gs_axi_bus_name_info_array_size].dev_names, strlen(p_device_name) + 3U);

            if (OSAL_RETURN_OK == osal_ret)
            {
                gs_axi_bus_name_info[gs_axi_bus_name_info_array_size].dev_names[0] = '\n';
                (void)strcpy(&gs_axi_bus_name_info[gs_axi_bus_name_info_array_size].dev_names[1], p_device_name);
                (void)strcat(gs_axi_bus_name_info[gs_axi_bus_name_info_array_size].dev_names, "\n");
            }

            gs_axi_bus_name_info_array_size++;
        }
        else
        {
            osal_ret = OSAL_RETURN_MEM;
        } /* end of if (NULL != p_tmp_table) */
    }

    /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
    R_OSAL_OS_Free((void *)p_compare_str);

    return osal_ret;
/* PRQA S 9107 2 # The termination function has already multiple sub-functions to reduce complexity.
   We have decided that further division is not desirable from perspective of maintainability and readability. */
}
/***********************************************************************************************************************
* End of function r_osal_os_io_register_axi_bus_info()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_046]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_123
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_device_name_connect()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_device_name_connect(const char* const p_device_name, size_t connect_index)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;
    size_t          axi_bus_name_len;
    char*           p_tmp_addr;
    char*           p_tmp_dev_names = NULL;

    /* Memory size is length of (dev_names + additional device name + '\n' + '\0') */
    axi_bus_name_len = strlen(gs_axi_bus_name_info[connect_index].dev_names) + strlen(p_device_name) + 2U;

    /* PRQA S 0314 2 # The 1st arguments must be (void *) but no side effects in this case. */
    /* PRQA S 5118,2706 1 # Freed in caller function() */
    p_tmp_addr = (char *)realloc((void *)gs_axi_bus_name_info[connect_index].dev_names, axi_bus_name_len);
    if (NULL == p_tmp_addr)
    {
        osal_ret = OSAL_RETURN_MEM;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0310 1 # The parameter must be (void **) and there is no side-effect in this case. */
        osal_ret = r_osal_os_io_malloc((void **)&p_tmp_dev_names, axi_bus_name_len);

        if (OSAL_RETURN_OK == osal_ret)
        {
            gs_axi_bus_name_info[connect_index].dev_names = p_tmp_addr;
            linux_ret = snprintf(p_tmp_dev_names, axi_bus_name_len, "%s%s\n", gs_axi_bus_name_info[connect_index].dev_names, p_device_name);
            if (linux_ret < 0)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                (void)strcpy(gs_axi_bus_name_info[connect_index].dev_names, p_tmp_dev_names);
            }

            /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
            R_OSAL_OS_Free((void *)p_tmp_dev_names);
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_device_name_connect()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_047]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_124
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_register_axi_bus_info_last_data()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_register_axi_bus_info_last_data(void)
{
    e_osal_return_t                 osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int                             cmp_ret;
    size_t                          cnt;
    st_osal_axi_bus_name_info_t*    p_tmp_table;
    bool                            exist_flag = false;
    size_t                          offset;

    for (cnt = 0; cnt < gs_axi_bus_name_info_array_size; cnt++)
    {
        cmp_ret = strcmp(gs_axi_bus_name_info[cnt].domain_name, "");
        if ((0 == cmp_ret) && (gs_axi_bus_name_info[cnt].utlb == OSAL_AXI_BUS_NAME_LAST))
        {
            /* Element of AXI bus name information already exist */
            exist_flag = true;
        }
    }

    if (false == exist_flag)
    {
        /* New entry to AXI bus name information */
        /* Use p_tmp_buffer to avoid memory leak in case realloc fail */
        /* PRQA S 0314 2 # The 1st arguments must be (void *) but no side effects in this case. */
        /* PRQA S 5118,2707 1 # Freed in deinitialize. */
        p_tmp_table = (st_osal_axi_bus_name_info_t *)realloc((void*)gs_axi_bus_name_info, (gs_axi_bus_name_info_array_size + 1U) * sizeof(st_osal_axi_bus_name_info_t));
        if (NULL == p_tmp_table)
        {
            osal_ret = OSAL_RETURN_MEM;
        }
        else
        {
            gs_axi_bus_name_info = p_tmp_table;

            gs_axi_bus_name_info[gs_axi_bus_name_info_array_size].utlb = OSAL_AXI_BUS_NAME_LAST;
            (void)strcpy(gs_axi_bus_name_info[gs_axi_bus_name_info_array_size].domain_name, "");
            gs_axi_bus_name_info[gs_axi_bus_name_info_array_size].dev_names = NULL;

            gs_axi_bus_name_info_array_size++;
        }
    } /* end of if (false == exist_flag) */

    for (cnt = 0; (OSAL_RETURN_OK == osal_ret) && (OSAL_AXI_BUS_NAME_LAST != gs_axi_bus_name_info[cnt].utlb); cnt++)
    {
        (void)strcpy(gs_axi_bus_name_info[cnt].domain_name_utlb, gs_axi_bus_name_info[cnt].domain_name);

        /* Connect uTLB to domain name */
        if (OSAL_INVALID_UTLB != gs_axi_bus_name_info[cnt].utlb)
        {
            offset = strlen(gs_axi_bus_name_info[cnt].domain_name);

            gs_axi_bus_name_info[cnt].domain_name_utlb[offset] = '_';
            offset++;

            gs_axi_bus_name_info[cnt].domain_name_utlb[offset] = (char)('0' + (gs_axi_bus_name_info[cnt].utlb / 10));
            offset++;

            gs_axi_bus_name_info[cnt].domain_name_utlb[offset] = (char)('0' + (gs_axi_bus_name_info[cnt].utlb % 10));
            offset++;

            gs_axi_bus_name_info[cnt].domain_name_utlb[offset] = '\0';
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_register_axi_bus_info_last_data()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_048]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_125
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_create_bus_name_list()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_create_bus_name_list(void)
{
    e_osal_return_t osal_ret;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             cmp_ret;
    size_t          cnt;
    size_t          cnt2;
    char*           p_domain_name_utlb = NULL;
    void*           p_addr;
    size_t          axi_list_offset = 0;

    gs_io_axi_bus_name_list_len = 0;

    /* PRQA S 0310 1 # The parameter must be (void **) and there is no side-effect in this case. */
    osal_ret = r_osal_os_io_malloc((void **)&p_domain_name_utlb, (size_t)(OSAL_BUS_DOMAIN_SIZE + OSAL_MAX_UTLB_NUM + 1U));

    for (cnt = 0; (OSAL_RETURN_OK == osal_ret) && (cnt < (sizeof(gs_osal_device_axi_ids) / sizeof(gs_osal_device_axi_ids[0]))); cnt++)
    {
        for (cnt2 = 0; cnt2 < gs_axi_bus_name_info_array_size; cnt2++)
        {
            cmp_ret = strcmp(gs_osal_device_axi_ids[cnt].axi_bus_name, gs_axi_bus_name_info[cnt2].domain_name);
            if (0 == cmp_ret)
            {
                r_osal_os_io_create_axi_bus_name(gs_osal_device_axi_ids[cnt].axi_bus_name, p_domain_name_utlb, (size_t)(OSAL_BUS_DOMAIN_SIZE + OSAL_MAX_UTLB_NUM + 1U));
                gs_io_axi_bus_name_list_len += (strlen(p_domain_name_utlb) + 1U);

                /* PRQA S 5118,2707 2 # Freed in caller function. */
                /* PRQA S 0315 1 # The 1st arguments must be (void *) but no side effects in this case. */
                p_addr = (char *)realloc(gsp_io_axi_bus_name_list, gs_io_axi_bus_name_list_len);
                if (NULL == p_addr)
                {
                    osal_ret = OSAL_RETURN_MEM;
                }

                if (OSAL_RETURN_OK == osal_ret)
                {
                    /* PRQA S 0316 1 # No problem because cast from void * to object is allowed. */
                    gsp_io_axi_bus_name_list = (char *)p_addr;

                    (void)strncpy(&(gsp_io_axi_bus_name_list[axi_list_offset]), p_domain_name_utlb, gs_io_axi_bus_name_list_len - axi_list_offset);
                    axi_list_offset += (strlen(p_domain_name_utlb) + 1U);
                }

                break;
            }
        }
    }

    /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
    R_OSAL_OS_Free((void *)p_domain_name_utlb);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_create_bus_name_list()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_049]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_126
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_create_axi_bus_name()
***********************************************************************************************************************/
OSAL_STATIC void r_osal_os_io_create_axi_bus_name(const char* p_axi_bus_name, char* p_buffer, size_t buffer_size)
{
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int     linux_ret;
    size_t  cnt = 0;
    size_t  cnt2;
    size_t  index;
    size_t  utlb_cnt = 0;
    /* The number of maximum uTLB is 64 */
    int32_t utlb_list[64];
    int32_t utlb;

    /* Example: p_buffer is vi0 */
    (void)strncpy(p_buffer, p_axi_bus_name, buffer_size);
    index = strlen(p_axi_bus_name);

    while (OSAL_AXI_BUS_NAME_LAST != gs_axi_bus_name_info[cnt].utlb)
    {
        linux_ret = strncmp(gs_axi_bus_name_info[cnt].domain_name, p_axi_bus_name, strlen(gs_axi_bus_name_info[cnt].domain_name));
        if (0 == linux_ret)
        {
            if (OSAL_INVALID_UTLB != gs_axi_bus_name_info[cnt].utlb)
            {
                /* Example: utlb_list is {10, 14, 11, 4, 13} */
                utlb_list[utlb_cnt] = gs_axi_bus_name_info[cnt].utlb;
                utlb_cnt++;
            }
        }
        cnt++;
    }

    if (0U != utlb_cnt)
    {
        /* Sort by ascending order                      */
        /* Example: utlb_list is {4, 10, 11, 13, 14}    */
        for (cnt = 0; cnt < (utlb_cnt - 1U); cnt++)
        {
            for (cnt2 = cnt + 1U; cnt2 < utlb_cnt; cnt2++)
            {
                if (utlb_list[cnt2] < utlb_list[cnt])
                {
                    utlb = utlb_list[cnt];
                    utlb_list[cnt] = utlb_list[cnt2];
                    utlb_list[cnt2] = utlb;
                }
            }
        }

        for (cnt = 0; cnt < utlb_cnt; cnt++)
        {
            /* Example: loop1 p_buffer is vi0_04            */
            /* Example: loop2 p_buffer is vi0_04_10         */
            /* Example: loop3 p_buffer is vi0_04_10-11_13   */
            p_buffer[index] = '_';
            index++;
            p_buffer[index] = (char)('0' + (utlb_list[cnt] / 10));
            index++;
            p_buffer[index] = (char)('0' + (utlb_list[cnt] % 10));
            index++;

            /* Search the maximum of continuous uTLB            */
            /* Example: loop1 uTLB is not continuous            */
            /* Example: loop2 maximum of continuous uTLB is 11  */
            /* Example: loop3 maximum of continuous uTLB is 14  */
            utlb = OSAL_INVALID_UTLB;
            for (cnt2 = cnt + 1U; cnt2 < utlb_cnt; cnt2++)
            {
                if ((utlb_list[cnt] + 1) == utlb_list[cnt2])
                {
                    utlb = utlb_list[cnt2];
                    /* PRQA S 2469 1 # It is a series of steps and is necessary so as not to reduce the readability */
                    cnt = cnt2;
                }
            }

            /* Add the maximum of continuous uTLB to p_buffer   */
            /* Example: loop1 p_buffer is vi0_04                */
            /* Example: loop2 p_buffer is vi0_04_10-11          */
            /* Example: loop3 p_buffer is vi0_04_10-11_13-14    */
            if (OSAL_INVALID_UTLB != utlb)
            {
                p_buffer[index] = '-';
                index++;
                p_buffer[index] = (char)('0' + (utlb / 10));
                index++;
                p_buffer[index] = (char)('0' + (utlb % 10));
                index++;
            }
        } /* end of for (cnt = 0; cnt < utlb_cnt; cnt++) */
    } /* end of if (0U != utlb_cnt) */

    p_buffer[index] = '\0';

    return;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_create_axi_bus_name()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_050]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_127
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_initialize_device()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_initialize_device(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 3 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;
    int             glob_ret;
    int             cmp_ret[OSAL_EXCLUDED_DEVICE_PATTERN];
    size_t          cnt;
    size_t          path_cnt;
    size_t          open_cnt = 0;
    glob_t          glob_buffer;
    regex_t         regex_buffer;
    regmatch_t*     p_match_buffer = NULL;
    /* Pattern of the device path to open */
    char*           p_dev_pattern[OSAL_OPEN_DEVICE_PATTERN];
    /* PRQA S 0286 4 # For regular expression string pattern. */
    const char      dev_pattern_01[] = "(^(/dev/){1}([a-z0-9_]+)_([0-9]{2}){1}$)";
    const char      dev_pattern_02[] = "(^(/dev/ipmmu_cache){1}_([a-z0-9]{2,}){1}$)";
    const char      dev_pattern_03[] = "(^(/dev/fcpra){1}_([a-z0-9]{2,}){1}(_[0-9]{2})?$)";
    const char      dev_pattern_04[] = "(^(/dev/fba){1}_([a-z0-9]+(_[a-z0-9]+)*)$)";
    /* Pattern of the excluded device path */
    char*           p_excluded_path[OSAL_EXCLUDED_DEVICE_PATTERN];
    const char      excluded_path_01[] = "/dev/irq";
    const char      excluded_path_02[] = "/dev/post";
    const char      excluded_path_03[] = "/dev/fcpra";

    /* PRQA S 0311 7 # For fixed strings do not use the stack */
    p_dev_pattern[0] = (char *)dev_pattern_01;
    p_dev_pattern[1] = (char *)dev_pattern_02;
    p_dev_pattern[2] = (char *)dev_pattern_03;
    p_dev_pattern[3] = (char *)dev_pattern_04;
    p_excluded_path[0] = (char *)excluded_path_01;
    p_excluded_path[1] = (char *)excluded_path_02;
    p_excluded_path[2] = (char *)excluded_path_03;

    /* PRQA S 0314 1 # The 1st arguments must be (void *) but no side effects in this case. */
    (void)memset((void *)&glob_buffer, 0x00, sizeof(glob_buffer));

    glob_ret = glob("/dev/*", 0, NULL, &glob_buffer);
    if (0 != glob_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0310 1 # The parameter must be (void **) and there is no side-effect in this case. */
        osal_ret = r_osal_os_io_calloc((void **)&gsp_device_list, glob_buffer.gl_pathc + 1U, sizeof(st_osal_device_list_t));
    }

    for (cnt = 0; (OSAL_RETURN_OK == osal_ret) && (cnt < (sizeof(p_dev_pattern) / sizeof(p_dev_pattern[0]))); cnt++)
    {
        /* PRQA S 4542,4543 1 # REG_EXTENDED and REG_NEWLINE are used in accordance with the specification. */
        linux_ret = regcomp(&regex_buffer, p_dev_pattern[cnt], REG_EXTENDED | REG_NEWLINE);
        if (0 != linux_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* PRQA S 0310 1 # The parameter must be (void **) and there is no side-effect in this case. */
            osal_ret = r_osal_os_io_calloc((void **)&p_match_buffer, regex_buffer.re_nsub + 1U, sizeof(regmatch_t));
        }

        for (path_cnt = 0; (OSAL_RETURN_OK == osal_ret) && (path_cnt < glob_buffer.gl_pathc); path_cnt++)
        {
            /* Exclude from the only dev_pattern_01 */
            if (0U == cnt)
            {
                cmp_ret[0] = strncmp(glob_buffer.gl_pathv[path_cnt], p_excluded_path[0], strlen(p_excluded_path[0]));
                cmp_ret[1] = strncmp(glob_buffer.gl_pathv[path_cnt], p_excluded_path[1], strlen(p_excluded_path[1]));
                cmp_ret[2] = strncmp(glob_buffer.gl_pathv[path_cnt], p_excluded_path[2], strlen(p_excluded_path[2]));
                if ((0 == cmp_ret[0]) || (0 == cmp_ret[1]) || (0 == cmp_ret[2]))
                {
                    continue;
                }
            }

            linux_ret = regexec(&regex_buffer, glob_buffer.gl_pathv[path_cnt], regex_buffer.re_nsub + 1U, p_match_buffer, 0);
            if (0 == linux_ret)
            {
                osal_ret = r_osal_os_io_open_device(&gsp_device_list[open_cnt], glob_buffer.gl_pathv[path_cnt]);
                open_cnt++;
            }
        }

        /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
        R_OSAL_OS_Free((void *)p_match_buffer);
        p_match_buffer = NULL;

        regfree(&regex_buffer);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        gsp_device_list[open_cnt].fd = -1;
    }

    if (0 == glob_ret)
    {
        globfree(&glob_buffer);
    }

    return osal_ret;
/* PRQA S 9104,9107 2 # The initialization function has already multiple sub-functions to reduce complexity.
   We have decided that further division is not desirable from perspective of maintainability and readability. */
}
/***********************************************************************************************************************
* End of function r_osal_os_io_initialize_device()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_051]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_128
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_get_num_of_device()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_num_of_device(const char* p_deviceType, size_t* const p_numOfDevices, glob_t const* p_glob_buffer)
{
    e_osal_return_t osal_ret;
    char*           p_dev_pattern = NULL;
    size_t          match_count = 0;

    /* PRQA S 0310 1 # The parameter must be (void **) and there is no side-effect in this case. */
    osal_ret = r_osal_os_io_malloc((void **)&p_dev_pattern, (size_t)(sizeof(char) * (size_t)PATH_MAX));
    if (OSAL_RETURN_OK != osal_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
       osal_ret = r_osal_os_io_create_regex_pattern(p_deviceType, p_dev_pattern, (size_t)(sizeof(char) * (size_t)PATH_MAX));
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_io_count_device(p_glob_buffer, p_dev_pattern, &match_count);
    }
    *p_numOfDevices = match_count;

    /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
    R_OSAL_OS_Free((void *)p_dev_pattern);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_get_num_of_device()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_052]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_129
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_create_regex_pattern()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_create_regex_pattern(const char* p_type, char* p_pattern, size_t size)
{
    /* PRQA S 5209 2 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;
    int             cmp_ret[5];
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /* Regular expressions for the R_OSAL_OS_IoGetNumOfDevices() and R_OSAL_OS_IoGetDeviceList() */
    cmp_ret[0] = strcmp(p_type, "ipmmu_cache");
    cmp_ret[1] = strcmp(p_type, "fcprr");
    cmp_ret[2] = strcmp(p_type, "fcprw");
    cmp_ret[3] = strcmp(p_type, "fcpra");
    cmp_ret[4] = strcmp(p_type, "fba");

    if (0 == cmp_ret[0]) /* ipmmu_cache is exception case. */
    {
        /* PRQA S 0286 1 # For regular expression string pattern */
        linux_ret = snprintf(p_pattern, size, "(^(/dev/%s){1}_([a-z0-9]{2,}){1}$)", p_type);
    }
    else if (0 == cmp_ret[1]) /* fcprr is exception case. It is the same regular expression as fcprw. */
    {                                      /* Separated for static analysis. */
        /* PRQA S 0286 1 # For regular expression string pattern */
        linux_ret = snprintf(p_pattern, size, "(^(/dev/%s){1}_([a-z0-9]+(_[a-z]+)*(_[0-9]{2}){1,2})$)", p_type);
    }
    else if (0 == cmp_ret[2]) /* fcprw is exception case. It is the same regular expression as fcprr. */
    {                                      /* Separated for static analysis. */
        /* PRQA S 0286 1 # For regular expression string pattern */
        linux_ret = snprintf(p_pattern, size, "(^(/dev/%s){1}_([a-z0-9]+(_[a-z]+)*(_[0-9]{2}){1,2})$)", p_type);
    }
    else if (0 == cmp_ret[3]) /* fcpra is exception case. */
    {
        /* PRQA S 0286 1 # For regular expression string pattern */
        linux_ret = snprintf(p_pattern, size, "(^(/dev/%s){1}_([a-z0-9]{2,}){1}(_[0-9]{2})?$)", p_type);
    }
    else if (0 == cmp_ret[4]) /* fba is exception case. */
    {
        /* PRQA S 0286 1 # For regular expression string pattern */
        linux_ret = snprintf(p_pattern, size, "(^(/dev/%s){1}_([a-z0-9]+(_[a-z0-9]+)*)$)", p_type);
    }
    else /* normal case */
    {
        /* PRQA S 0286 1 # For regular expression string pattern */
        linux_ret = snprintf(p_pattern, size, "(^(/dev/%s){1}_([0-9]{2}){1}$)", p_type);
    } /* end of if (0 == cmp_ret[0]) */

    if (linux_ret < 0)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    return osal_ret;
/* PRQA S 9110 1 # It is a series of processes and is necessary so as not to reduce the readability based on design. */
}
/***********************************************************************************************************************
* End of function r_osal_os_io_create_regex_pattern()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_053]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_130
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_count_device()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_count_device(glob_t const* p_glob_buffer, char const* p_dev_pattern, size_t* p_match_count)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;
    regex_t         regex_buffer;
    regmatch_t*     p_match_buffer = NULL;
    size_t          size;
    size_t          count = 0;

    /* p_dev_pattern set in the r_osal_os_io_create_regex_pattern() */
    /* PRQA S 4542,4543 1 # REG_EXTENDED and REG_NEWLINE are used in accordance with the specification. */
    linux_ret = regcomp(&regex_buffer, p_dev_pattern, REG_EXTENDED | REG_NEWLINE);
    if (0 != linux_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        size = regex_buffer.re_nsub + 1U;

        /* PRQA S 0310 1 # The parameter must be (void **) and there is no side-effect in this case. */
        osal_ret = r_osal_os_io_calloc((void **)&p_match_buffer, size, sizeof(regmatch_t));
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            while (count < p_glob_buffer->gl_pathc)
            {
                linux_ret = regexec(&regex_buffer, p_glob_buffer->gl_pathv[count], size, p_match_buffer, 0);
                if (0 == linux_ret)
                {
                    (*p_match_count)++;
                }
                count++;
            }

            /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
            R_OSAL_OS_Free((void *)p_match_buffer);
        }

        regfree(&regex_buffer);
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_count_device()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_054]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_131
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_get_devicelist()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_devicelist(const char* p_device_type_name, char* p_array, size_t array_size, size_t* const p_num_of_byte, glob_t const* p_glob_buffer)
{
    e_osal_return_t osal_ret;
    char*           p_dev_pattern = NULL;
    char*           p_local_array = NULL;
    size_t          local_array_size;

    /* PRQA S 0310 1 # The parameter must be (void **) and there is no side-effect in this case. */
    osal_ret = r_osal_os_io_malloc((void **)&p_dev_pattern, (size_t)(sizeof(char) * (size_t)PATH_MAX));
    if (OSAL_RETURN_OK != osal_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_io_create_regex_pattern(p_device_type_name, p_dev_pattern, (size_t)(sizeof(char) * (size_t)PATH_MAX));
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_io_generate_list(p_glob_buffer, p_dev_pattern, &p_local_array);
    }

    if (NULL != p_local_array)
    {
        if ((OSAL_RETURN_OK == osal_ret) &&
            ((NULL != p_array) || (0U != array_size)))
        {
            local_array_size = strlen(p_local_array);
            if ((NULL == p_array) || ((local_array_size + 1U) > array_size))
            {
                osal_ret = OSAL_RETURN_PAR;
            }
            else
            {
                (void)strncpy(p_array, p_local_array, array_size);
            }
        }
        else if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = OSAL_RETURN_OK_RQST;
        }
        else
        {
            /* Do nothing */
        }

        if ((OSAL_RETURN_OK == osal_ret) || (OSAL_RETURN_OK_RQST == osal_ret))
        {
            *p_num_of_byte = strlen(p_local_array) + 1U;
        }

        /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
        R_OSAL_OS_Free((void *)p_local_array);
    }

    /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
     R_OSAL_OS_Free((void *)p_dev_pattern);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_get_devicelist()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_055]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_132
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_generate_list()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_generate_list(glob_t const* p_glob_buffer, char const* p_dev_pattern, char** pp_list_buffer)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;
    size_t          count = 0;
    regex_t         regex_buffer;
    regmatch_t*     p_match_buffer = NULL;
    size_t          size;
    size_t          dst_offset = 0;
    size_t          match_count = 0;

    /* p_dev_pattern set from r_osal_os_io_create_regex_pattern() */
    /* PRQA S 4542,4543 1 # REG_EXTENDED and REG_NEWLINE are used in accordance with the specification. */
    linux_ret = regcomp(&regex_buffer, p_dev_pattern, REG_EXTENDED | REG_NEWLINE);
    if (0 != linux_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        size = regex_buffer.re_nsub + 1U;

        /* PRQA S 0310 1 # The parameter must be (void **) and there is no side-effect in this case. */
        osal_ret = r_osal_os_io_calloc((void **)&p_match_buffer, size, sizeof(regmatch_t));
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            while (count < p_glob_buffer->gl_pathc)
            {
                linux_ret = regexec(&regex_buffer, p_glob_buffer->gl_pathv[count], size, p_match_buffer, 0);
                if (0 == linux_ret)
                {
                    osal_ret = r_osal_os_io_add_item_to_list(p_glob_buffer, pp_list_buffer, count, &dst_offset);
                    match_count++;
                }

                if (OSAL_RETURN_OK != osal_ret)
                {
                    break;
                }
                count++;
            }

            /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
            R_OSAL_OS_Free((void *)p_match_buffer);
        }

        regfree(&regex_buffer);
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    if (0U != dst_offset)
    {
        (*pp_list_buffer)[dst_offset - 1U] = '\0';
    }

    if ((0U == match_count) && (OSAL_RETURN_OK == osal_ret))
    {
        osal_ret = OSAL_RETURN_UNKNOWN_NAME;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_generate_list()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_056]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_133
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_add_item_to_list()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_add_item_to_list(glob_t const* p_glob_buffer, char** pp_list_buffer, size_t count, size_t* p_dst_offset)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    char*           p_tmp_buffer;
    size_t          len;

    len = strlen(p_glob_buffer->gl_pathv[count]) - 5U; /* skip "/dev/" */
    /* Use p_tmp_buffer to avoid memory leak in case realloc fail */
    /* PRQA S 0314 2 # The 1st arguments must be (void *) but no side effects in this case. */
    /* PRQA S 5118,2707 1 # Freed in r_osal_os_io_get_devicelist. */
    p_tmp_buffer = (char *)realloc((void *)(*pp_list_buffer), *p_dst_offset + len + 1U);
    if (NULL == p_tmp_buffer)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        *pp_list_buffer = p_tmp_buffer;
        (void)strncpy(&((*pp_list_buffer)[*p_dst_offset]), &((p_glob_buffer->gl_pathv[count])[5]), len); /* skip "/dev/" */
        *p_dst_offset += len;
        (*pp_list_buffer)[(*p_dst_offset)] = '\n';
        (*p_dst_offset)++;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_add_item_to_list()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_057]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_134
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_open()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_open(osal_device_handle_t handle, const char* p_id)
{
    e_osal_return_t osal_ret;
    char*           p_uio_symbolic_name = NULL;

    /* Lock device related to valid device ID */
    osal_ret = r_osal_os_io_lock_device(handle, p_id);

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0310 1 # The parameter must be (void **) and there is no side-effect in this case. */
        osal_ret = r_osal_os_io_malloc((void **)&p_uio_symbolic_name, (size_t)(sizeof(char) * (size_t)PATH_MAX));
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_io_get_uio_link(p_id, p_uio_symbolic_name);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_io_get_map_information(handle, p_uio_symbolic_name);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_io_set_irq_information(handle, p_uio_symbolic_name, p_id);
    }

    /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
    R_OSAL_OS_Free((void *)p_uio_symbolic_name);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_open()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_058]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_135
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_lock_device()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_lock_device(osal_device_handle_t handle, const char* p_id)
{
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 2 # Return values from linux must be int. Used according to the specifications. */
    int                 linux_ret;
    int                 dev_fd = -1;
    size_t              cnt = 0;

    while (-1 != gsp_device_list[cnt].fd)
    {
        linux_ret = strcmp(p_id, gsp_device_list[cnt].p_id);
        if (0 == linux_ret)
        {
            dev_fd = gsp_device_list[cnt].fd;
            break;
        }
        cnt++;
    }

    /* Not found valid device ID */
    if (-1 == dev_fd)
    {
        osal_ret = OSAL_RETURN_ID;
    }

    /* Already locked device */
    if ((OSAL_RETURN_OK == osal_ret) && (false != gsp_device_list[cnt].locked))
    {
        osal_ret = OSAL_RETURN_BUSY;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 4542 1 # LOCK_SH is used in accordance with the specification, verified OK. */
        linux_ret = flock(dev_fd, LOCK_SH);
        if (0 == linux_ret)
        {
            handle->dev_fd      = dev_fd;
            handle->dev_index   = cnt;
            handle->pm_handle   = &gsp_device_list[handle->dev_index].pm_control;
            gsp_device_list[handle->dev_index].locked = true;
        }
        else
        {
            osal_ret = OSAL_RETURN_BUSY;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_lock_device()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_059]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_136
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_get_uio_link()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_uio_link(char const* p_uio_dev_name, char* p_uio_link_name)
{
    e_osal_return_t osal_ret;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;
    char*           p_device_path = NULL;
    char*           p_link_path = NULL;
    ssize_t         len;

    /* PRQA S 0310 1 # The parameter must be (void **) and there is no side-effect in this case. */
    osal_ret = r_osal_os_io_malloc((void **)&p_device_path, (size_t)(sizeof(char) * (size_t)PATH_MAX));

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0310 1 # The parameter must be (void **) and there is no side-effect in this case. */
        osal_ret = r_osal_os_io_malloc((void **)&p_link_path, (size_t)(sizeof(char) * (size_t)PATH_MAX));
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        linux_ret = snprintf(p_device_path, (size_t)(sizeof(char) * (size_t)PATH_MAX), "/dev/%s", p_uio_dev_name);
        if (linux_ret < 0)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        len = readlink(p_device_path, p_link_path, (size_t)((sizeof(char) * (size_t)PATH_MAX) - 1U));
        if ((-1) == len)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            p_link_path[len] = '\0';
            (void)strcpy(p_uio_link_name, basename(p_link_path));
        }
    }

    /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
    R_OSAL_OS_Free((void *)p_device_path);

    /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
    R_OSAL_OS_Free((void *)p_link_path);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_get_uio_link()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_060]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_137
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_get_map_information()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_map_information(osal_device_handle_t handle, const char* p_uio_symbolic_name)
{
    e_osal_return_t osal_ret;

    handle->base_addr = NULL;

    osal_ret = r_osal_os_io_get_map_size(handle, p_uio_symbolic_name);

    /* Create new mapping between device file and virtual address space*/
    if (OSAL_RETURN_OK == osal_ret)
    {
        if (0U != handle->len)
        {
            /* PRQA S 4542 1 # PROT_READ and PROT_WRITE are used in accordance with the specification, verified OK. */
            handle->base_addr = mmap(NULL, (size_t)handle->len, PROT_READ | PROT_WRITE, MAP_SHARED, handle->dev_fd, 0);
            /* PRQA S 0326 1 # MAP_FAILED is used in accordance with the specification. */
            if (MAP_FAILED == handle->base_addr)
            {
                handle->base_addr = NULL;
                osal_ret = OSAL_RETURN_FAIL;
            }
        }
        else
        {
            handle->base_addr = NULL;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_get_map_information()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_061]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_138
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_get_map_size()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_map_size(osal_device_handle_t handle, const char* p_uio_symbolic_name)
{
    e_osal_return_t osal_ret;
    size_t          size;
    uint64_t        addr;
    /* PRQA S 5209 1 # Used to set the return value of strtol(). */
    unsigned long   scan_ulong;

    osal_ret = r_osal_os_io_scan_ulong("/sys/class/uio/%s/maps/map0/size", p_uio_symbolic_name, &scan_ulong);

    if (OSAL_RETURN_OK == osal_ret)
    {
        size = (size_t)scan_ulong;

        osal_ret = r_osal_os_io_scan_ulong("/sys/class/uio/%s/maps/map0/addr", p_uio_symbolic_name, &scan_ulong);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        addr = (uint64_t)scan_ulong;

        if ((0U != addr) && (0U != size))
        {
            handle->len = size;
        }
        else if ((0U == addr) && (0U == size))
        {
            handle->len = 0;
        }
        else
        {
            osal_ret = OSAL_RETURN_CONF;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_get_map_size()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_062]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_139
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_set_irq_information()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_set_irq_information(osal_device_handle_t handle, char* p_uio_symbolic_name, const char* p_id)
{
    e_osal_return_t     osal_ret;
    e_osal_return_t     mutex_ret;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int                 linux_ret;
    size_t              cnt;
    osal_os_mutexattr_t mutex_attr = NULL;
    char*               p_tmp_irq_file_name = NULL;
    char*               p_irq_uio_name = NULL;

    osal_ret = r_osal_os_io_get_irq_num(handle, p_uio_symbolic_name, p_id);
    if ((OSAL_RETURN_OK == osal_ret) && (0U != handle->irq_num))
    {
        /* PRQA S 0310 1 # The parameter must be (void **) and there is no side-effect in this case. */
        osal_ret = r_osal_os_io_malloc((void **)&handle->int_handle, sizeof(st_osal_interrupt_control_t) * handle->irq_num);

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* PRQA S 0310 1 # The parameter must be (void **) and there is no side-effect in this case. */
            osal_ret = r_osal_os_io_malloc((void **)&p_tmp_irq_file_name, (size_t)(sizeof(char) * (size_t)PATH_MAX));
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* PRQA S 0310 1 # The parameter must be (void **) and there is no side-effect in this case. */
            osal_ret = r_osal_os_io_malloc((void **)&p_irq_uio_name, (size_t)(sizeof(char) * (size_t)PATH_MAX));
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_MutexInitAttr(&mutex_attr);
    }

    for (cnt = 0; (OSAL_RETURN_OK == osal_ret) && (cnt < handle->irq_num); cnt++)
    {
        handle->int_handle[cnt].irq_thread_mtx  = NULL;
        handle->int_handle[cnt].irq_mtx         = NULL;
        handle->int_handle[cnt].irq_cond        = NULL;

        if (0U == cnt)
        {
            handle->int_handle[cnt].dev_fd = handle->dev_fd;
            (void)strcpy(p_irq_uio_name, p_uio_symbolic_name);
        }
        else
        {
            linux_ret = snprintf(p_tmp_irq_file_name, (size_t)(sizeof(char) * (size_t)PATH_MAX), "/dev/irq_%s_%02zu", p_id, cnt);
            if (linux_ret < 0)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* PRQA S 0339 1 # O_RDWR uses octal and is used in accordance with the specification of open(). */
                handle->int_handle[cnt].dev_fd = R_OSAL_OS_Open(p_tmp_irq_file_name, O_RDWR);
                if (handle->int_handle[cnt].dev_fd < 0)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                /* [5] is for skipping the "/dev/" */
                osal_ret = r_osal_os_io_get_uio_link(&p_tmp_irq_file_name[5], p_irq_uio_name);
            }
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_os_io_get_spi_num(p_irq_uio_name, &handle->int_handle[cnt].spi_num);
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_OS_MutexCreate(mutex_attr, &handle->int_handle[cnt].irq_thread_mtx);
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_OS_MutexCreate(mutex_attr, &handle->int_handle[cnt].irq_mtx);
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_OS_CondCreate(&handle->int_handle[cnt].irq_cond);
        }

        handle->int_handle[cnt].irq_thread          = NULL;
        handle->int_handle[cnt].irq_channel         = cnt;
        handle->int_handle[cnt].irq_func            = NULL;
        handle->int_handle[cnt].irq_arg             = NULL;
        handle->int_handle[cnt].irq_enable          = false;
        handle->int_handle[cnt].irq_thread_active   = false;
        handle->int_handle[cnt].irq_use             = false;
        handle->int_handle[cnt].dev_handle          = handle;
    }

    if (NULL != mutex_attr)
    {
        mutex_ret = R_OSAL_OS_MutexDeinitAttr(mutex_attr);
        if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
        {
            osal_ret = mutex_ret;
        }
    }

    /* PRQA S 0314 2 # The arguments must be (void *) but no side effects in this case. */
    R_OSAL_OS_Free((void *)p_irq_uio_name);
    R_OSAL_OS_Free((void *)p_tmp_irq_file_name);

    return osal_ret;
/* PRQA S 9104,9107 2 # The initialization function has already multiple sub-functions to reduce complexity.
   We have decided that further division is not desirable from perspective of maintainability and readability. */
}
/***********************************************************************************************************************
* End of function r_osal_os_io_set_irq_information()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_063]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_140
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_get_irq_num()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_irq_num(osal_device_handle_t handle, char* p_uio_symbolic_name, const char* p_id)
{
    e_osal_return_t osal_ret;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;
    char*           p_sysfs_irq_file = NULL;
    char*           p_irq_file_name = NULL;
    glob_t          glob_buffer;

    /* PRQA S 0310 1 # The parameter must be (void **) and there is no side-effect in this case. */
    osal_ret = r_osal_os_io_malloc((void **)&p_sysfs_irq_file, (size_t)(sizeof(char) * (size_t)PATH_MAX));

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Check the existence of interrupt-specifiers on device, initialize for interrupt information */
        linux_ret = snprintf(p_sysfs_irq_file, (size_t)(sizeof(char) * (size_t)PATH_MAX), "/sys/class/uio/%s/device/of_node/interrupts", p_uio_symbolic_name);
        if (0 > linux_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        handle->irq_num = 0;

        linux_ret = access(p_sysfs_irq_file, F_OK);
        if (0 == linux_ret)
        {
            /* PRQA S 0310 1 # The parameter must be (void **) and there is no side-effect in this case. */
            osal_ret = r_osal_os_io_malloc((void **)&p_irq_file_name, (size_t)(sizeof(char) * (size_t)PATH_MAX));

            if (OSAL_RETURN_OK == osal_ret)
            {
                linux_ret = snprintf(p_irq_file_name, (size_t)(sizeof(char) * (size_t)PATH_MAX), "/dev/irq_%s_??", p_id);
                if (linux_ret < 0)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                handle->irq_num = 1; /* one interrupt is original device itself */

                linux_ret = glob(p_irq_file_name, 0, NULL, &glob_buffer);
                if (0 == linux_ret) /*for devices only have more than one interrupt channel */
                {
                    handle->irq_num += glob_buffer.gl_pathc;
                    globfree(&glob_buffer);
                }
            }
        } /* end of if (0 == linux_ret) */
    }

    /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
    R_OSAL_OS_Free((void *)p_sysfs_irq_file);

    /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
    R_OSAL_OS_Free((void *)p_irq_file_name);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_get_irq_num()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_064]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_141
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_initialize_for_pm()
***********************************************************************************************************************/
OSAL_STATIC void r_osal_os_io_initialize_for_pm(st_osal_device_list_t* p_device)
{
    e_osal_return_t osal_ret;
    char*           p_uio_symbolic_name = NULL;
    uint64_t        pm_domain;
    /* PRQA S 5209 2 # Used as the parameter of ioctl. */
    int             current_power_state;
    int             current_clock_state;

    /* Initialize for power management.
     * Basically, all devices can support power(is_power_support), clock(is_clock_support), and reset(is_reset_support) as default.
     * Initialize device policy according to current power and clock.
     * This code block should not return any API error even if ioctl is failed,
     * Because IO Manager should work well without power management.
     * If ioctl is failed, change is_power_support, is_clock_support, is_reset_support to false. */

    p_device->pm_control.is_power_support   = false;
    p_device->pm_control.is_clock_support   = false;
    p_device->pm_control.is_reset_support   = false;
    p_device->pm_control.pm_wait_state      = false;

    /* PRQA S 0310 1 # The parameter must be (void **) and there is no side-effect in this case. */
    osal_ret = r_osal_os_io_malloc((void **)&p_uio_symbolic_name, (size_t)(sizeof(char) * (size_t)PATH_MAX));

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_io_get_uio_link(p_device->p_id, p_uio_symbolic_name);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Set the power_shared */
        osal_ret = r_osal_os_io_get_power_domain(p_uio_symbolic_name, &pm_domain);
        if (OSAL_RETURN_OK == osal_ret)
        {
            /* Set the power_shared */
            if ((uint64_t)0 != (pm_domain & gs_io_pd_always_on))
            {
                p_device->pm_control.power_shared = OSAL_PM_DOMAIN_SHARED;
            }
            else
            {
                p_device->pm_control.power_shared = OSAL_PM_DOMAIN_NOT_SHARED;
            }

            p_device->pm_control.is_power_support   = true;
            p_device->pm_control.is_clock_support   = true;
            p_device->pm_control.is_reset_support   = true;

            r_osal_os_io_get_pm_state(p_device, &current_power_state, &current_clock_state);

            /* Set the required state */
            if ((false == p_device->pm_control.is_power_support) &&
            	(false == p_device->pm_control.is_clock_support))
            {
                p_device->pm_control.pm_required_state = OSAL_PM_REQUIRED_STATE_REQUIRED;
            }
            else
            {
                p_device->pm_control.pm_required_state = OSAL_PM_REQUIRED_STATE_RELEASED;
            }

            /* Set the policy */
            if ((1 == current_power_state) && (1 == current_clock_state))
            {
                p_device->pm_control.pm_policy      = OSAL_PM_POLICY_HP;
                p_device->pm_control.pm_lower_state = OSAL_PM_STATE_ENABLED;
            }
            else if ((0 == current_power_state) && (0 == current_clock_state))
            {
                p_device->pm_control.pm_policy      = OSAL_PM_POLICY_PG;
                p_device->pm_control.pm_lower_state = OSAL_PM_STATE_PG;
            }
            else
            {
                p_device->pm_control.pm_policy      = OSAL_PM_POLICY_CG;
                p_device->pm_control.pm_lower_state = OSAL_PM_STATE_CG;
            }
        } /* end of if (OSAL_RETURN_OK == osal_ret) */
    }

    /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
    R_OSAL_OS_Free((void *)p_uio_symbolic_name);

    return;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_initialize_for_pm()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_065]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_142
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_get_pm_state()
***********************************************************************************************************************/
/* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
OSAL_STATIC void r_osal_os_io_get_pm_state(st_osal_device_list_t* p_device, int* p_pw_state, int* p_clock_state)
{
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int linux_ret;
    /* PRQA S 5209 1 # Used as the parameter of ioctl. */
    int reset_state;

    /* PRQA S 5209 4 # Used as the parameter of ioctl. */
    _Static_assert( (
        sizeof(*p_pw_state)
        ) == (
        sizeof(int)
        ), "Structure must not have any padding");

    /* PRQA S 4542 2 # UIO_PDRV_GET_PWR is used in accordance with the specification, verified OK. */
    /* PRQA S 4513,0499,4543,1841 1 # UIO_PDRV_GET_PWR uses bit-shift, verified OK. */
    linux_ret = ioctl(p_device->fd, UIO_PDRV_GET_PWR, p_pw_state);
    if (0 == linux_ret)
    {
        if (OSAL_PM_DOMAIN_SHARED == p_device->pm_control.power_shared)
        {
            p_device->pm_control.is_power_support = false;
        }
    }
    else
    {
        *p_pw_state = 1;
        p_device->pm_control.is_power_support = false;
    }

    /* PRQA S 5209 4 # Used as the parameter of ioctl. */
    _Static_assert( (
        sizeof(*p_clock_state)
           ) == (
           sizeof(int)
           ), "Structure must not have any padding");

    /* PRQA S 4542 2 # UIO_PDRV_GET_CLK is used in accordance with the specification, verified OK. */
    /* PRQA S 4513,0499,4543,1841 1 # UIO_PDRV_GET_CLK uses bit-shift, verified OK. */
    linux_ret = ioctl(p_device->fd, UIO_PDRV_GET_CLK, p_clock_state);
    if (0 != linux_ret)
    {
        *p_clock_state = *p_pw_state;
        p_device->pm_control.is_clock_support = false;
    }

    /* PRQA S 5209 4 # Used as the parameter of ioctl. */
    _Static_assert( (
        sizeof(reset_state)
        ) == (
        sizeof(int)
        ), "Structure must not have any padding");

    /* PRQA S 4542 2 # UIO_PDRV_GET_RESET is used in accordance with the specification, verified OK. */
    /* PRQA S 4513,0499,4543,1841 1 # UIO_PDRV_GET_RESET uses bit-shift, verified OK. */
    linux_ret = ioctl(p_device->fd, UIO_PDRV_GET_RESET, &reset_state);
    if (0 != linux_ret)
    {
        p_device->pm_control.is_reset_support = false;
    }

    return;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_get_pm_state()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_066]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_143
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_open_error()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
OSAL_STATIC void r_osal_os_io_open_error(osal_device_handle_t handle)
{
    size_t  cnt;

    if (NULL != handle)
    {
        if (NULL != handle->int_handle)
        {
            for (cnt = 0; cnt < handle->irq_num; cnt++)
            {
                (void)r_osal_os_io_cond_destroy(&handle->int_handle[cnt].irq_cond);
                (void)r_osal_os_io_mutex_destroy(&handle->int_handle[cnt].irq_mtx);
                (void)r_osal_os_io_mutex_destroy(&handle->int_handle[cnt].irq_thread_mtx);
            }

            /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
            R_OSAL_OS_Free((void *)handle->int_handle);
            handle->int_handle = NULL;
        }

        if (NULL != handle->base_addr)
        {
            (void)munmap(handle->base_addr, handle->len);
        }

        if (-1 != handle->dev_fd)
        {
            (void)flock(handle->dev_fd, LOCK_UN);
            gsp_device_list[handle->dev_index].locked = false;
        }
    } /* end of if (NULL != handle) */

    return;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_open_error()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_067]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_144
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_unset_irq_information()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_unset_irq_information(osal_device_handle_t handle)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t          cnt;

    for (cnt = 0; (OSAL_RETURN_OK == osal_ret) && (cnt < handle->irq_num); cnt++)
    {
        /* Close if the interrupt fd is different from device fd */
        /* PRQA S 2822 1 # The pointer and size are checked in the R_OSAL_OS_IoDeviceOpen function. */
        if (handle->dev_fd != handle->int_handle[cnt].dev_fd)
        {
            osal_ret = R_OSAL_OS_Close(handle->int_handle[cnt].dev_fd);
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_os_io_cond_destroy(&handle->int_handle[cnt].irq_cond);
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_os_io_mutex_destroy(&handle->int_handle[cnt].irq_mtx);
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_os_io_mutex_destroy(&handle->int_handle[cnt].irq_thread_mtx);
        }
    }

    if ((OSAL_RETURN_OK == osal_ret) &&
        (0U != handle->irq_num))
    {
        /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
        R_OSAL_OS_Free((void *)handle->int_handle);
        handle->int_handle = NULL;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_unset_irq_information()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_068]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_145
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_close_post_fd()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_close_post_fd(osal_device_handle_t handle)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;

    /* Close the file descriptor opened by R_OSAL_OS_PmSetPostClock() */
    linux_ret = flock(handle->dev_post_fd, LOCK_UN);
    if (0 != linux_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_Close(handle->dev_post_fd);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        handle->dev_post_fd  = -1;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_close_post_fd()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_069]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_146
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_get_device_axi_bus_id()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_device_axi_bus_id(st_osal_os_io_get_device_axi_bus_id_t* p_param)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    p_param->pst_axi_bus_name_info = gs_axi_bus_name_info;

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_get_device_axi_bus_id()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_070]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_147
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_get_axi_bus_id_from_device_name()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_axi_bus_id_from_device_name(st_osal_os_io_get_axi_bus_id_from_device_name_t* p_param)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    p_param->pst_axi_bus_name_info = gs_axi_bus_name_info;

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_get_axi_bus_id_from_device_name()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_071]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_148
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_get_axi_bus_name_list()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_axi_bus_name_list(st_osal_os_io_get_axi_bus_name_list_t* p_param)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    p_param->axi_bus_name_list_len  = gs_io_axi_bus_name_list_len;
    p_param->p_axi_bus_name_list    = gsp_io_axi_bus_name_list;

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_get_axi_bus_name_list()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_072]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_149
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_get_axi_bus_name()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_axi_bus_name(st_osal_os_io_get_axi_bus_name_t* p_param)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    p_param->pst_axi_bus_name_info = gs_axi_bus_name_info;

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_get_axi_bus_name()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_073]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_150
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_get_num_of_axi_ids_for_axi_bus_name()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_num_of_axi_ids_for_axi_bus_name(st_osal_os_io_get_num_of_axi_ids_for_axi_bus_name_t* p_param)
{
    e_osal_return_t osal_ret;
    int32_t*        p_utlb_buffer = NULL;
    size_t          utlb_buffer_count;

    osal_ret = r_osal_os_io_get_domain_name(p_param->p_axi_bus_name, p_param->pst_domain_table->domain_name);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_io_utlb_parser(p_param->p_axi_bus_name, strlen(p_param->p_axi_bus_name), &p_utlb_buffer, &utlb_buffer_count);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        p_param->pst_domain_table->utlb_buffer_count = utlb_buffer_count;

        if (NULL != p_utlb_buffer)
        {
            /* PRQA S 0315 1 # The 1st, 2nd arguments must be (void *) but no side effects in this case. */
            (void)memcpy(p_param->pst_domain_table->utlb_buffer, p_utlb_buffer, sizeof(int32_t) * utlb_buffer_count);

            /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
            R_OSAL_OS_Free((void *)p_utlb_buffer);
        }

        p_param->pst_axi_bus_name_info = gs_axi_bus_name_info;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_get_num_of_axi_ids_for_axi_bus_name()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_074]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_151
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_get_axi_id_list_for_axi_bus_name()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_axi_id_list_for_axi_bus_name(st_osal_os_io_get_axi_id_list_for_axi_bus_name_t* p_param)
{
    e_osal_return_t osal_ret;
    int32_t*        p_utlb_buffer = NULL;
    size_t          utlb_buffer_count;

    osal_ret = r_osal_os_io_get_domain_name(p_param->p_axi_bus_name, p_param->pst_domain_table->domain_name);

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_io_utlb_parser(p_param->p_axi_bus_name, strlen(p_param->p_axi_bus_name), &p_utlb_buffer, &utlb_buffer_count);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        p_param->pst_domain_table->utlb_buffer_count = utlb_buffer_count;

        if (NULL != p_utlb_buffer)
        {
            /* PRQA S 0315 1 # The 1st, 2nd arguments must be (void *) but no side effects in this case. */
            (void)memcpy(p_param->pst_domain_table->utlb_buffer, p_utlb_buffer, sizeof(int32_t) * utlb_buffer_count);

            /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
            R_OSAL_OS_Free((void *)p_utlb_buffer);
        }

        p_param->pst_axi_bus_name_info = gs_axi_bus_name_info;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_get_axi_id_list_for_axi_bus_name()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_075]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_152
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_get_num_of_axi_ids_for_device_hdl()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_num_of_axi_ids_for_device_hdl(st_osal_os_io_get_num_of_axi_ids_for_device_hdl_t* p_param)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    p_param->pst_axi_bus_name_info = gs_axi_bus_name_info;

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_get_num_of_axi_ids_for_device_hdl()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_076]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_153
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_get_axi_id_list_for_device_hdl()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_axi_id_list_for_device_hdl(st_osal_os_io_get_axi_id_list_for_device_hdl_t* p_param)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    p_param->pst_axi_bus_name_info = gs_axi_bus_name_info;

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_get_axi_id_list_for_device_hdl()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_077]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_154
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_get_num_of_axi_ids_for_device_name()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_num_of_axi_ids_for_device_name(st_osal_os_io_get_num_of_axi_ids_for_device_name_t* p_param)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    p_param->pst_axi_bus_name_info = gs_axi_bus_name_info;

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_get_num_of_axi_ids_for_device_name()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_078]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_155
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_get_axi_id_list_for_device_name()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_axi_id_list_for_device_name(st_osal_os_io_get_axi_id_list_for_device_name_t* p_param)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    p_param->pst_axi_bus_name_info = gs_axi_bus_name_info;

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_get_axi_id_list_for_device_name()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_079]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_156
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_get_domain_name()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_domain_name(const char* p_axi_bus_name, char* p_domain_name)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;
    regex_t         regex_buffer;
    regmatch_t*     p_match = NULL;

    if (NULL == p_domain_name)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 4542,4543 1 # REG_EXTENDED and REG_NEWLINE are used in accordance with the specification. */
        linux_ret = regcomp(&regex_buffer, "(^[0-9a-z]+)_([0-9]{2})", REG_EXTENDED | REG_NEWLINE);
        if (0 != linux_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* PRQA S 0310 1 # The parameter must be (void **) and there is no side-effect in this case. */
            osal_ret = r_osal_os_io_calloc((void **)&p_match, 2, sizeof(regmatch_t));
            if (OSAL_RETURN_OK != osal_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                linux_ret = regexec(&regex_buffer, p_axi_bus_name, 1, p_match, 0);
                if (0 == linux_ret)
                {
                    /* PRQA S 4393 2 # It can cast to unsigned type because it always be a positive value */
                    (void)strncpy(p_domain_name, &p_axi_bus_name[p_match[0].rm_so], (size_t)(p_match[0].rm_eo - 3)); /*remove "_[0-9]{2}"*/
                    p_domain_name[p_match[0].rm_eo - 3] = '\0';
                }
                else
                {
                    osal_ret = OSAL_RETURN_UNKNOWN_NAME;
                }
            }

            /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
            R_OSAL_OS_Free((void *)p_match);
        } /* end of if (0 != linux_ret) */

        regfree(&regex_buffer);
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_get_domain_name()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_080]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_157
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_utlb_parser()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_utlb_parser(const char* p_axi_bus_name, size_t len, int32_t** pp_list, size_t* const p_numOfutlb)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;
    regex_t         regex_buffer;
    regmatch_t*     p_match = NULL;
    char            tmp_str[4] = "";
    size_t          index = 0;
    regoff_t        offset = 0;

    if (NULL == p_axi_bus_name)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else if (NULL == p_numOfutlb)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 4542,4543 1 # REG_EXTENDED and REG_NEWLINE are used in accordance with the specification. */
        linux_ret = regcomp(&regex_buffer, "([_-][0-9]{2})", REG_EXTENDED | REG_NEWLINE);
        if (0 != linux_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* PRQA S 0310 1 # The parameter must be (void **) and there is no side-effect in this case. */
            osal_ret = r_osal_os_io_calloc((void **)&p_match, 2, sizeof(regmatch_t));
            if (OSAL_RETURN_OK != osal_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            linux_ret = regexec(&regex_buffer, p_axi_bus_name, 1, p_match, 0);
            while (0 == linux_ret)
            {
                /* PRQA S 4393 1 # It can cast to unsigned type because it always be a positive value */
                (void)strncpy(tmp_str, &p_axi_bus_name[p_match[0].rm_so + offset], (size_t)(p_match[0].rm_eo - p_match[0].rm_so));
                tmp_str[3]='\0';

                osal_ret = r_osal_os_io_utlb_parser_sub(pp_list, &index, tmp_str);

                offset += p_match[0].rm_eo;
                if ((OSAL_RETURN_OK == osal_ret) && ((regoff_t)len > offset))
                {
                    /* next */
                    linux_ret = regexec(&regex_buffer, &p_axi_bus_name[offset], 1, p_match, 0);
                }
                else
                {
                    /* end of loop */
                    linux_ret = 1;
                }
            }

            *p_numOfutlb = index;
        }

        /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
        R_OSAL_OS_Free((void *)p_match);

        regfree(&regex_buffer);
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_utlb_parser()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_081]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_158
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_utlb_parser_sub()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_utlb_parser_sub(int32_t** pp_list, size_t* p_index, const char* p_num_str)
{
    e_osal_return_t osal_ret;
    int32_t         num;

    if ((p_num_str[1] < '0') || ('9' < p_num_str[1]) || (p_num_str[2] < '0') || ('9' < p_num_str[2]))
    {
        osal_ret = OSAL_RETURN_UNKNOWN_NAME; /* axi_bus_name is invalid (e.g., ir0_a4, ir0_6d) */
    }
    else
    {
        num = (int32_t)((p_num_str[1] - '0') * 10) + (p_num_str[2] - '0');

        switch (p_num_str[0])
        {
            case '_':
                osal_ret =  r_osal_os_io_utlb_parser_sub1(pp_list, p_index, num);
                break;

            case '-':
                osal_ret =  r_osal_os_io_utlb_parser_sub2(pp_list, p_index, num);
                break;

            default:
                osal_ret = OSAL_RETURN_UNKNOWN_NAME; /* axi_bus_name is invalid (e.g., ir0:04:05)*/
                break;
        }
    } /* end of if ((p_num_str[1] < '0') || ('9' < p_num_str[1]) || (p_num_str[2] < '0') || ('9' < p_num_str[2])) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_utlb_parser_sub()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_082]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_159
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_utlb_parser_sub1()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_utlb_parser_sub1(int32_t** pp_list, size_t* p_index, int32_t num)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int32_t*        p_tmp_buffer;
    size_t          tmp_index = *p_index;

    /* PRQA S 0314 2 # The 1st arguments must be (void *) but no side effects in this case. */
    /* PRQA S 5118,2706,2707 1 # Freed in caller function() */
    p_tmp_buffer = (int32_t *)realloc((void *)(*pp_list), sizeof(int32_t) * (tmp_index + 1U));
    if (NULL == p_tmp_buffer)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        *pp_list = p_tmp_buffer;
        p_tmp_buffer[tmp_index] = num;
        *p_index = tmp_index +1U;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_utlb_parser_sub1()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_083]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_160
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_utlb_parser_sub2()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_utlb_parser_sub2(int32_t** pp_list, size_t* p_index, int32_t num)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int32_t*        p_tmp_buffer = *pp_list;
    size_t          tmp_index;
    int32_t         target_val;
    int32_t         current_val;
    int32_t         count;

    if (0U == *p_index)
    {
        osal_ret = OSAL_RETURN_UNKNOWN_NAME; /* axi_bus_name is invalid (e.g., ir0-04)*/
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        tmp_index = *p_index;
        current_val = p_tmp_buffer[tmp_index - 1U];
        target_val = num;
        if (target_val <= current_val)
        {
            osal_ret = OSAL_RETURN_UNKNOWN_NAME; /* axi_bus_name is invalid (e.g., ir0_04-02)*/
        }
        else
        {
            /* PRQA S 4393 3 # It can cast to unsigned type because it always be a positive value */
            /* PRQA S 0314 2 # The 1st arguments must be (void *) but no side effects in this case. */
            /* PRQA S 5118,2707 1 # Freed in caller function() */
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
        } /* end of if (target_val <= current_val) */
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_utlb_parser_sub2()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_084]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_161
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_get_spi_num()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_get_spi_num(char* p_irq_uio_name, uint64_t* p_spi_num)
{
    e_osal_return_t osal_ret;
    e_osal_return_t close_ret;
    /* PRQA S 5209 2 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;
    int             fd;
    uint16_t const  read_data[4];
    char*           p_sysfs_irq_file = NULL;

    /* PRQA S 0310 1 # The parameter must be (void **) and there is no side-effect in this case. */
    osal_ret = r_osal_os_io_malloc((void **)&p_sysfs_irq_file, (size_t)(sizeof(char) * (size_t)PATH_MAX));

    if (OSAL_RETURN_OK == osal_ret)
    {
        linux_ret = snprintf(p_sysfs_irq_file, (size_t)(sizeof(char) * (size_t)PATH_MAX), "/sys/class/uio/%s/device/of_node/interrupts", p_irq_uio_name);
        if (linux_ret < 0)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0339 1 # O_RDONLY uses octal and is used in accordance with the specification of open(). */
        fd = R_OSAL_OS_Open(p_sysfs_irq_file, O_RDONLY);
        if (fd < 0)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* PRQA S 0311, 0314 1 # The 2nd arguments must be (void *) but no side effects in this case. */
            osal_ret = R_OSAL_OS_Read(fd, (void *)read_data, sizeof(read_data));
            if (OSAL_RETURN_OK == osal_ret)
            {
                *p_spi_num = (uint64_t)ntohs(read_data[3]);
            }

            close_ret = R_OSAL_OS_Close(fd);
            if ((OSAL_RETURN_OK != close_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
        }
    }

    /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
    R_OSAL_OS_Free((void *)p_sysfs_irq_file);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_get_spi_num()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_085]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_162
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_open_device()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_open_device(st_osal_device_list_t* p_device, const char* p_path)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /* PRQA S 0339 1 # O_RDWR uses octal and is used in accordance with the specification of open(). */
    p_device->fd = R_OSAL_OS_Open(p_path, O_RDWR);
    if (p_device->fd < 0)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    /* [5] is for skipping the "/dev/" */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0310 1 # The parameter must be (void **) and there is no side-effect in this case. */
        osal_ret = r_osal_os_io_malloc((void **)&p_device->p_id, strlen(&p_path[5]) + 1U);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        (void)strcpy(p_device->p_id, &p_path[5]);

        r_osal_os_io_initialize_for_pm(p_device);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_open_device()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_051]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_163
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_scan_ulong()
***********************************************************************************************************************/
/* PRQA S 5209 1 # System is requesting an unsigned long, so it cannot be changed. */
OSAL_STATIC e_osal_return_t r_osal_os_io_scan_ulong(const char* p_path_format, const char* p_uio_symbolic_name, unsigned long* p_data)
{
    e_osal_return_t osal_ret;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;
    char*           p_sysfs_file = NULL;
    FILE*           p_fd;

    /* PRQA S 0310 1 # The parameter must be (void **) and there is no side-effect in this case. */
    osal_ret = r_osal_os_io_malloc((void **)&p_sysfs_file, (size_t)(sizeof(char) * (size_t)PATH_MAX));

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Get register address range from /sys/class/uio/uioX/maps/map0/size */
        linux_ret = snprintf(p_sysfs_file, (size_t)(sizeof(char) * (size_t)PATH_MAX), p_path_format, p_uio_symbolic_name);
        if (linux_ret < 0)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* If device can not open, set 0 to p_data */
        *p_data = 0U;

        /* PRQA S 5124, 2702 1 # Closed in this function. */
        p_fd = fopen(p_sysfs_file, "r");
        if (NULL != p_fd)
        {
            /* PRQA S 5124 1 # Read one data from the opened fd. */
            linux_ret = fscanf(p_fd, "%lx", p_data);
            if (1 != linux_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }

            /* PRQA S 5124 1 # Close the opened fd. p_fd is opened in this function. */
            linux_ret = fclose(p_fd);
            if (0 != linux_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
        }
    }

    /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
    R_OSAL_OS_Free((void *)p_sysfs_file);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_scan_ulong()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_089]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_164
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_malloc()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
OSAL_STATIC e_osal_return_t r_osal_os_io_malloc(void** pp_ptr, size_t size)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    *pp_ptr = R_OSAL_OS_Malloc(size);
    if (NULL == *pp_ptr)
    {
        osal_ret = OSAL_RETURN_MEM;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_malloc()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_090]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_165
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_calloc()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
OSAL_STATIC e_osal_return_t r_osal_os_io_calloc(void** pp_ptr, size_t nmemb, size_t size)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /* PRQA S 5118 1 # Freed in caller function. */
    *pp_ptr = calloc(nmemb, size);
    if (NULL == *pp_ptr)
    {
        osal_ret = OSAL_RETURN_MEM;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_calloc()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_091]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_166
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_mutex_destroy()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_mutex_destroy(osal_os_mutex_t* p_mutex)
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
* End of function r_osal_os_io_mutex_destroy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_092]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_IO_167
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_cond_destroy()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_cond_destroy(osal_os_cond_t* p_cond)
{
    e_osal_return_t osal_ret;

    osal_ret = R_OSAL_OS_CondDestroy(*p_cond);
    if (OSAL_RETURN_OK == osal_ret)
    {
        *p_cond = NULL;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_cond_destroy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_IO_093]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */
