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
* File Name :    r_osal_dev_cfg_share.h
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Device Configurator Public Header
***********************************************************************************************************************/
#ifndef OSAL_DEV_CFG_SHARE_H
#define OSAL_DEV_CFG_SHARE_H

/***********************************************************************************************************************
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_COM_001
***********************************************************************************************************************/
/***********************************************************************************************************************
 * Definition
***********************************************************************************************************************/
/***********************************************************************************************************************
 * @def     OSAL_IO_HANDLE_ID
 * @brief   IO manager handle ID.
***********************************************************************************************************************/
#define OSAL_IO_HANDLE_ID       (0x4F53414C494FU) /* OSALIO */

/***********************************************************************************************************************
 * @def     OSAL_INVALID_HANDLE_ID
 * @brief   Invalid handle ID.
***********************************************************************************************************************/
#define OSAL_INVALID_HANDLE_ID  (0)

/***********************************************************************************************************************
 * @def     OSAL_AXI_BUS_NAME_LAST
 * @brief   Indicate the last of AXI bus information array.
***********************************************************************************************************************/
#define OSAL_AXI_BUS_NAME_LAST  (-1)

/***********************************************************************************************************************
 * @def     OSAL_INVALID_UTLB
 * @brief   Indicate the dummy device of AXI bus information array.
***********************************************************************************************************************/
#define OSAL_INVALID_UTLB       (-2)

/***********************************************************************************************************************
 * @def     OSAL_DEVICE_NAME_SIZE
 * @brief   Maximum length of the device name.
***********************************************************************************************************************/
#define OSAL_DEVICE_NAME_SIZE   (32U)

/***********************************************************************************************************************
 * @def     OSAL_BUS_DOMAIN_SIZE
 * @brief   Maximum length of the domain name.
***********************************************************************************************************************/
#define OSAL_BUS_DOMAIN_SIZE    (32U)

/***********************************************************************************************************************
 * @def     OSAL_MAX_UTLB_NUM
 * @brief   Maximum length of the uTLB.
***********************************************************************************************************************/
#define OSAL_MAX_UTLB_NUM       (100U * 2U)
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_COM_002]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_COM_002
***********************************************************************************************************************/
/***********************************************************************************************************************
 * @typedef e_osal_pm_domain_type_t
 * @brief   This type is the typedef definition of enum #e_osal_pm_domain_type.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @enum    e_osal_pm_domain_type
 * @brief   osal power domain type.
 **********************************************************************************************************************/
typedef enum e_osal_pm_domain_type
{
    OSAL_PM_DOMAIN_SHARED       = 0,    /*!< Power domain is shared with other devices or always on */
    OSAL_PM_DOMAIN_NOT_SHARED   = 1     /*!< Power domain is not shared with other devices */
} e_osal_pm_domain_type_t;
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_COM_003]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * @typedef st_osal_interrupt_control_t
 * @brief   This type is the typedef definition of struct #st_osal_interrupt_control.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @struct  st_osal_interrupt_control
 * @brief   structure to control interrupt information.
 **********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the user references the member after calling */
typedef struct st_osal_interrupt_control
{
    /* PRQA S 5209 1 # File descriptor must be int. */
    int                     dev_fd;             /*!< Store the file descriptor of device */
    uint64_t                irq_channel;        /*!< Store the interrupt channel of device */
    uint64_t                spi_num;            /*!< Store the SPI number of device */
    osal_os_thread_t        irq_thread;         /*!< Store handle of Interrupt thread */
    bool                    irq_thread_active;  /*!< Determine the status of interrupt thread that finished or not */
    osal_device_handle_t    dev_handle;         /*!< Store device handle associated to this interrupt */
    p_osal_isr_func_t       irq_func;           /*!< Pointer of Thread worker function registered by user */
    void*                   irq_arg;            /*!< Pointer of Argument for thread worker function */
    bool                    irq_enable;         /*!< Interrupt status flag. TRUE: Enable interrupt, FALSE: Disable interrupt */
    bool                    irq_use;            /*!< Interrupt status flag. TRUE: Using interrupt (registered), FALSE: Not using interrupt (not registered) */
    osal_os_mutex_t         irq_mtx;            /*!< Mutex of each interrupt channel */
    osal_os_cond_t          irq_cond;           /*!< Condition variable of each interrupt channel */
    osal_os_mutex_t         irq_thread_mtx;     /*!< Mutex for irq thread creation synchronization */
    st_osal_time_t          abs_timeout;        /*!< Timeout of thread start */
} st_osal_interrupt_control_t;

/***********************************************************************************************************************
 * @typedef st_osal_power_control_t
 * @brief   This type is the typedef definition of struct #st_osal_power_control.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @struct  st_osal_power_control
 * @brief   structure to control power information.
 **********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the user references the member after calling */
typedef struct st_osal_power_control
{
    e_osal_pm_required_state_t      pm_required_state;  /*!< Store the required status of device */
    e_osal_pm_policy_t              pm_policy;          /*!< Store the power policy of device */
    e_osal_pm_state_t               pm_lower_state;     /*!< Store the lowest intermediate power state of the device since the last change to not-required status. */
    e_osal_pm_domain_type_t         power_shared;       /*!< Store the power domain sharing status of the device */
    bool                            is_power_support;   /*!< Determine which device will be supported power */
    bool                            is_clock_support;   /*!< Determine which device will be supported clock */
    bool                            is_reset_support;   /*!< Determine which device will be supported reset */
    bool                            pm_wait_state;      /*!< Power manager wait status flag (true: waiting, false: non-waiting) */
} st_osal_power_control_t;

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_COM_003
***********************************************************************************************************************/
/***********************************************************************************************************************
 * @typedef st_osal_device_control_t
 * @brief   This type is the typedef definition of struct #st_osal_device_control.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @struct  st_osal_device_control
 * @brief   structure to control IO information.
 **********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the user references the member after calling */
typedef struct st_osal_device_control
{
    char*                           dev_id;             /*!< Store opened device id */
    /* PRQA S 5209 2 # File descriptor must be int. */
    int                             dev_fd;             /*!< Store file descriptor of opened device */
    int                             dev_post_fd;        /*!< Store file descriptor of opened device (Post Clock) */
    size_t                          dev_index;          /*!< Store the file descriptor index of device */
    size_t                          len;                /*!< Store register range */
    void*                           base_addr;          /*!< Store virtual address of calling thread when finishing the mapping */
    size_t                          irq_num;            /*!< Store the number of interrupt channels */
    st_osal_interrupt_control_t*    int_handle;         /*!< Store interrupt control information */
    osal_os_mutex_t                 int_mtx;            /*!< The mutex use to protect the data accessing to interrupt information */
    st_osal_power_control_t*        pm_handle;          /*!< Store power control information */
    osal_os_mutex_t                 pm_mtx;             /*!< The mutex use to protect the data accessing to power information */
    uint64_t                        handle_id;          /*!< Determine that the device is opened successfully. OSAL_IO_HANDLE_ID: Already called R_OSAL_IoDeviceOpen(), OSAL_INVALID_HANDLE_ID: No called R_OSAL_IoDeviceOpen() */
} st_osal_device_control_t;
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_COM_004]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * @typedef st_osal_device_list_t
 * @brief   This type is the typedef definition of struct #st_osal_device_list.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @struct  st_osal_device_list
 * @brief   structure to device information.
 **********************************************************************************************************************/
typedef struct st_osal_device_list
{
    /* PRQA S 5209 1 # File descriptor must be int. */
    int                     fd;         /*!< File descriptor of opened device */
    char*                   p_id;       /*!< Device name of opened */
    bool                    locked;     /*!< Determine that the device is locked */
    st_osal_power_control_t pm_control; /*!< Store power control information */
} st_osal_device_list_t;

/***********************************************************************************************************************
 * @typedef st_osal_axi_bus_info_t
 * @brief   This type is the typedef definition of struct #st_osal_axi_bus_info.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @struct  st_osal_axi_bus_info
 * @brief   structure to hold AXI bus information.
 **********************************************************************************************************************/
typedef struct st_osal_axi_bus_info
{
    osal_axi_bus_id_t   ipmmu_id;       /*!< Store ipmmu-id which related to domain AXI */
    const char*         axi_bus_name;   /*!< Name of AXI bus */
    bool                used;           /*!< Determine the AXI bus name has been available in the list or not. */
    osal_axi_bus_id_t   axi_bus_id;     /*!< The AXI bus ID */
} st_osal_axi_bus_info_t;

/***********************************************************************************************************************
 * @typedef st_osal_power_info_t
 * @brief   This type is the typedef definition of struct #st_osal_power_info.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @struct  st_osal_power_info
 * @brief   structure to hold power initialization information.
 **********************************************************************************************************************/
typedef struct st_osal_power_info
{
    const char*             device_id;      /*!< Store the device ID */
    e_osal_pm_policy_t      initial_policy; /*!< Initial policy of Power Manager of the device */
    e_osal_pm_domain_type_t power_shared;   /*!< Store the power domain sharing status of the device */
} st_osal_power_info_t;

/***********************************************************************************************************************
 * @typedef st_osal_axi_bus_name_info_t
 * @brief   This type is the typedef definition of struct #st_osal_axi_bus_name_info.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @struct  st_osal_axi_bus_name_info
 * @brief   structure to hold domain name, utlb and related device name information for AXI API.
 **********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the user references the member after calling */
typedef struct st_osal_axi_bus_name_info
{
    char    domain_name[OSAL_BUS_DOMAIN_SIZE];                          /*!< domain name */
    char    domain_name_utlb[OSAL_BUS_DOMAIN_SIZE + OSAL_MAX_UTLB_NUM]; /*!< domain name with uTLB */
    int32_t utlb;                                                       /*!< parsed utlb */
    char*   dev_names;                                                  /*!< device name list */
} st_osal_axi_bus_name_info_t;

/***********************************************************************************************************************
 * @typedef st_osal_io_domain_table_t
 * @brief   This type is the typedef definition of struct #st_osal_io_domain_table.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @struct  st_osal_io_domain_table
 * @brief   structure to hold domain information for AXI API.
 **********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the user references the member after calling */
typedef struct st_osal_io_domain_table
{
    char        domain_name[OSAL_BUS_DOMAIN_SIZE];  /*!< domain name */
    int32_t     utlb_buffer[OSAL_MAX_UTLB_NUM];     /*!< parsed utlb */
    size_t      utlb_buffer_count;                  /*!< number of utlb */
} st_osal_io_domain_table_t;

/***********************************************************************************************************************
 * @typedef st_osal_os_io_get_device_axi_bus_id_t
 * @brief   This type is the typedef definition of struct #st_osal_os_io_get_device_axi_bus_id.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @struct  st_osal_os_io_get_device_axi_bus_id
 * @brief   Parameter of R_OSAL_OS_IoGetAxiBusInformation.
 **********************************************************************************************************************/
typedef struct st_osal_os_io_get_device_axi_bus_id
{
    st_osal_axi_bus_name_info_t*    pst_axi_bus_name_info;  /*!< Domain name, uTLB and related device name information */
} st_osal_os_io_get_device_axi_bus_id_t;

/***********************************************************************************************************************
 * @typedef st_osal_os_io_get_axi_bus_id_from_device_name_t
 * @brief   This type is the typedef definition of struct #st_osal_os_io_get_axi_bus_id_from_device_name.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @struct  st_osal_os_io_get_axi_bus_id_from_device_name
 * @brief   Parameter of R_OSAL_OS_IoGetAxiBusInformation.
 **********************************************************************************************************************/
typedef struct st_osal_os_io_get_axi_bus_id_from_device_name
{
    st_osal_axi_bus_name_info_t*    pst_axi_bus_name_info;  /*!< Domain name, uTLB and related device name information */
} st_osal_os_io_get_axi_bus_id_from_device_name_t;

/***********************************************************************************************************************
 * @typedef st_osal_os_io_get_axi_bus_name_list_t
 * @brief   This type is the typedef definition of struct #st_osal_os_io_get_axi_bus_name_list.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @struct  st_osal_os_io_get_axi_bus_name_list
 * @brief   Parameter of R_OSAL_OS_IoGetAxiBusInformation.
 **********************************************************************************************************************/
typedef struct st_osal_os_io_get_axi_bus_name_list
{
    size_t  axi_bus_name_list_len;  /*!< Size of the AXI bus name list */
    char*   p_axi_bus_name_list;    /*!< AXI bus name list */
} st_osal_os_io_get_axi_bus_name_list_t;

/***********************************************************************************************************************
 * @typedef st_osal_os_io_get_axi_bus_name_t
 * @brief   This type is the typedef definition of struct #st_osal_os_io_get_axi_bus_name.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @struct  st_osal_os_io_get_axi_bus_name
 * @brief   Parameter of R_OSAL_OS_IoGetAxiBusInformation.
 **********************************************************************************************************************/
typedef struct st_osal_os_io_get_axi_bus_name
{
    st_osal_axi_bus_name_info_t*    pst_axi_bus_name_info;  /*!< Domain name, uTLB and related device name information */
} st_osal_os_io_get_axi_bus_name_t;

/***********************************************************************************************************************
 * @typedef st_osal_os_io_get_num_of_axi_ids_for_axi_bus_name_t
 * @brief   This type is the typedef definition of struct #st_osal_os_io_get_num_of_axi_ids_for_axi_bus_name.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @struct  st_osal_os_io_get_num_of_axi_ids_for_axi_bus_name
 * @brief   Parameter of R_OSAL_OS_IoGetAxiBusInformation.
 **********************************************************************************************************************/
typedef struct st_osal_os_io_get_num_of_axi_ids_for_axi_bus_name
{
    const char*                     p_axi_bus_name;         /*!< AXI bus name */
    st_osal_io_domain_table_t*      pst_domain_table;       /*!< Domain information associated with the AXI bus name */
    st_osal_axi_bus_name_info_t*    pst_axi_bus_name_info;  /*!< Domain name, uTLB and related device name information */
} st_osal_os_io_get_num_of_axi_ids_for_axi_bus_name_t;

/***********************************************************************************************************************
 * @typedef st_osal_os_io_get_axi_id_list_for_axi_bus_name_t
 * @brief   This type is the typedef definition of struct #st_osal_os_io_get_axi_id_list_for_axi_bus_name.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @struct  st_osal_os_io_get_axi_id_list_for_axi_bus_name
 * @brief   Parameter of R_OSAL_OS_IoGetAxiBusInformation.
 **********************************************************************************************************************/
typedef struct st_osal_os_io_get_axi_id_list_for_axi_bus_name
{
    const char*                     p_axi_bus_name;         /*!< AXI bus name */
    st_osal_io_domain_table_t*      pst_domain_table;       /*!< Domain information associated with the AXI bus name */
    st_osal_axi_bus_name_info_t*    pst_axi_bus_name_info;  /*!< Domain name, uTLB and related device name information */
} st_osal_os_io_get_axi_id_list_for_axi_bus_name_t;

/***********************************************************************************************************************
 * @typedef st_osal_os_io_get_num_of_axi_ids_for_device_hdl_t
 * @brief   This type is the typedef definition of struct #st_osal_os_io_get_num_of_axi_ids_for_device_hdl.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @struct  st_osal_os_io_get_num_of_axi_ids_for_device_hdl
 * @brief   Parameter of R_OSAL_OS_IoGetAxiBusInformation.
 **********************************************************************************************************************/
typedef struct st_osal_os_io_get_num_of_axi_ids_for_device_hdl
{
    st_osal_axi_bus_name_info_t*    pst_axi_bus_name_info;  /*!< Domain name, uTLB and related device name information */
} st_osal_os_io_get_num_of_axi_ids_for_device_hdl_t;

/***********************************************************************************************************************
 * @typedef st_osal_os_io_get_axi_id_list_for_device_hdl_t
 * @brief   This type is the typedef definition of struct #st_osal_os_io_get_axi_id_list_for_device_hdl.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @struct  st_osal_os_io_get_axi_id_list_for_device_hdl
 * @brief   Parameter of R_OSAL_OS_IoGetAxiBusInformation.
 **********************************************************************************************************************/
typedef struct st_osal_os_io_get_axi_id_list_for_device_hdl
{
    st_osal_axi_bus_name_info_t*    pst_axi_bus_name_info;  /*!< Domain name, uTLB and related device name information */
} st_osal_os_io_get_axi_id_list_for_device_hdl_t;

/***********************************************************************************************************************
 * @typedef st_osal_os_io_get_num_of_axi_ids_for_device_name_t
 * @brief   This type is the typedef definition of struct #st_osal_os_io_get_num_of_axi_ids_for_device_name.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @struct  st_osal_os_io_get_num_of_axi_ids_for_device_name
 * @brief   Parameter of R_OSAL_OS_IoGetAxiBusInformation.
 **********************************************************************************************************************/
typedef struct st_osal_os_io_get_num_of_axi_ids_for_device_name
{
    st_osal_axi_bus_name_info_t*    pst_axi_bus_name_info;  /*!< Domain name, uTLB and related device name information */
} st_osal_os_io_get_num_of_axi_ids_for_device_name_t;

/***********************************************************************************************************************
 * @typedef st_osal_os_io_get_axi_id_list_for_device_name_t
 * @brief   This type is the typedef definition of struct #st_osal_os_io_get_axi_id_list_for_device_name.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @struct  st_osal_os_io_get_axi_id_list_for_device_name
 * @brief   Parameter of R_OSAL_OS_IoGetAxiBusInformation.
 **********************************************************************************************************************/
typedef struct st_osal_os_io_get_axi_id_list_for_device_name
{
    st_osal_axi_bus_name_info_t*    pst_axi_bus_name_info;  /*!< Domain name, uTLB and related device name information */
} st_osal_os_io_get_axi_id_list_for_device_name_t;

/** @} OSAL_WRAPPER */
#endif /* OSAL_DEV_CFG_SHARE_H */
